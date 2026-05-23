#include "lab1/aes_crypto.hpp"

#include <cryptopp/aes.h>
#include <cryptopp/ccm.h>
#include <cryptopp/filters.h>
#include <cryptopp/gcm.h>
#include <cryptopp/modes.h>
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace nt219::lab1 {
namespace {
using ByteVector = std::vector<std::uint8_t>;

std::string lower_copy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

const CryptoPP::byte* bytes_ptr(const ByteVector& data) {
    return reinterpret_cast<const CryptoPP::byte*>(data.data());
}

template <typename EncryptionType>
ByteVector encrypt_stream_mode(const AesRequest& request) {
    EncryptionType enc;
    if (mode_requires_iv(request.mode)) {
        enc.SetKeyWithIV(bytes_ptr(request.key), request.key.size(), bytes_ptr(request.iv), request.iv.size());
    } else {
        enc.SetKey(bytes_ptr(request.key), request.key.size());
    }
    std::string output;
    CryptoPP::StringSource source(bytes_ptr(request.input), request.input.size(), true,
        new CryptoPP::StreamTransformationFilter(enc, new CryptoPP::StringSink(output)));
    return ByteVector(output.begin(), output.end());
}

template <typename DecryptionType>
ByteVector decrypt_stream_mode(const AesRequest& request) {
    DecryptionType dec;
    if (mode_requires_iv(request.mode)) {
        dec.SetKeyWithIV(bytes_ptr(request.key), request.key.size(), bytes_ptr(request.iv), request.iv.size());
    } else {
        dec.SetKey(bytes_ptr(request.key), request.key.size());
    }
    std::string output;
    CryptoPP::StringSource source(bytes_ptr(request.input), request.input.size(), true,
        new CryptoPP::StreamTransformationFilter(dec, new CryptoPP::StringSink(output)));
    return ByteVector(output.begin(), output.end());
}

ByteVector encrypt_gcm(const AesRequest& request) {
    CryptoPP::GCM<CryptoPP::AES>::Encryption enc;
    enc.SetKeyWithIV(bytes_ptr(request.key), request.key.size(), bytes_ptr(request.iv), request.iv.size());
    std::string output;
    CryptoPP::AuthenticatedEncryptionFilter filter(enc, new CryptoPP::StringSink(output), false, static_cast<int>(request.tag_size));
    filter.ChannelPut(CryptoPP::AAD_CHANNEL, bytes_ptr(request.aad), request.aad.size());
    filter.ChannelMessageEnd(CryptoPP::AAD_CHANNEL);
    filter.ChannelPut(CryptoPP::DEFAULT_CHANNEL, bytes_ptr(request.input), request.input.size());
    filter.ChannelMessageEnd(CryptoPP::DEFAULT_CHANNEL);
    return ByteVector(output.begin(), output.end());
}

ByteVector decrypt_gcm(const AesRequest& request) {
    if (request.input.size() < request.tag_size) {
        throw std::runtime_error("GCM ciphertext is shorter than authentication tag");
    }
    CryptoPP::GCM<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(bytes_ptr(request.key), request.key.size(), bytes_ptr(request.iv), request.iv.size());
    std::string output;
    CryptoPP::AuthenticatedDecryptionFilter filter(dec, new CryptoPP::StringSink(output), CryptoPP::AuthenticatedDecryptionFilter::THROW_EXCEPTION, static_cast<int>(request.tag_size));
    filter.ChannelPut(CryptoPP::AAD_CHANNEL, bytes_ptr(request.aad), request.aad.size());
    filter.ChannelMessageEnd(CryptoPP::AAD_CHANNEL);
    filter.ChannelPut(CryptoPP::DEFAULT_CHANNEL, bytes_ptr(request.input), request.input.size());
    filter.ChannelMessageEnd(CryptoPP::DEFAULT_CHANNEL);
    return ByteVector(output.begin(), output.end());
}

ByteVector encrypt_ccm(const AesRequest& request) {
    CryptoPP::CCM<CryptoPP::AES, 16>::Encryption enc;
    enc.SetKeyWithIV(bytes_ptr(request.key), request.key.size(), bytes_ptr(request.iv), request.iv.size());
    enc.SpecifyDataLengths(request.aad.size(), request.input.size(), 0);
    std::string output;
    CryptoPP::AuthenticatedEncryptionFilter filter(enc, new CryptoPP::StringSink(output), false, static_cast<int>(request.tag_size));
    filter.ChannelPut(CryptoPP::AAD_CHANNEL, bytes_ptr(request.aad), request.aad.size());
    filter.ChannelMessageEnd(CryptoPP::AAD_CHANNEL);
    filter.ChannelPut(CryptoPP::DEFAULT_CHANNEL, bytes_ptr(request.input), request.input.size());
    filter.ChannelMessageEnd(CryptoPP::DEFAULT_CHANNEL);
    return ByteVector(output.begin(), output.end());
}

ByteVector decrypt_ccm(const AesRequest& request) {
    if (request.input.size() < request.tag_size) {
        throw std::runtime_error("CCM ciphertext is shorter than authentication tag");
    }
    const std::size_t ciphertext_size = request.input.size() - request.tag_size;
    CryptoPP::CCM<CryptoPP::AES, 16>::Decryption dec;
    dec.SetKeyWithIV(bytes_ptr(request.key), request.key.size(), bytes_ptr(request.iv), request.iv.size());
    dec.SpecifyDataLengths(request.aad.size(), ciphertext_size, 0);
    std::string output;
    CryptoPP::AuthenticatedDecryptionFilter filter(dec, new CryptoPP::StringSink(output), CryptoPP::AuthenticatedDecryptionFilter::THROW_EXCEPTION, static_cast<int>(request.tag_size));
    filter.ChannelPut(CryptoPP::AAD_CHANNEL, bytes_ptr(request.aad), request.aad.size());
    filter.ChannelMessageEnd(CryptoPP::AAD_CHANNEL);
    filter.ChannelPut(CryptoPP::DEFAULT_CHANNEL, bytes_ptr(request.input), request.input.size());
    filter.ChannelMessageEnd(CryptoPP::DEFAULT_CHANNEL);
    return ByteVector(output.begin(), output.end());
}
}

AesMode parse_aes_mode(const std::string& value) {
    const auto v = lower_copy(value);
    if (v == "ecb") return AesMode::Ecb;
    if (v == "cbc") return AesMode::Cbc;
    if (v == "cfb") return AesMode::Cfb;
    if (v == "ofb") return AesMode::Ofb;
    if (v == "ctr") return AesMode::Ctr;
    if (v == "gcm") return AesMode::Gcm;
    if (v == "ccm") return AesMode::Ccm;
    if (v == "xts") return AesMode::Xts;
    throw std::runtime_error("Unsupported AES mode: " + value);
}

std::string aes_mode_to_string(AesMode mode) {
    switch (mode) {
        case AesMode::Ecb: return "ecb";
        case AesMode::Cbc: return "cbc";
        case AesMode::Cfb: return "cfb";
        case AesMode::Ofb: return "ofb";
        case AesMode::Ctr: return "ctr";
        case AesMode::Gcm: return "gcm";
        case AesMode::Ccm: return "ccm";
        case AesMode::Xts: return "xts";
    }
    throw std::runtime_error("Unknown AES mode");
}

bool is_aead_mode(AesMode mode) { return mode == AesMode::Gcm || mode == AesMode::Ccm; }
bool mode_requires_iv(AesMode mode) { return mode != AesMode::Ecb; }

std::size_t required_iv_size(AesMode mode) {
    switch (mode) {
        case AesMode::Ecb: return 0;
        case AesMode::Gcm:
        case AesMode::Ccm: return 12;
        case AesMode::Cbc:
        case AesMode::Cfb:
        case AesMode::Ofb:
        case AesMode::Ctr:
        case AesMode::Xts: return CryptoPP::AES::BLOCKSIZE;
    }
    throw std::runtime_error("Unknown AES mode");
}

void validate_aes_key(const ByteVector& key, AesMode mode) {
    if (mode == AesMode::Xts) {
        if (key.size() != 32 && key.size() != 64) throw std::runtime_error("AES-XTS key must be 32 or 64 bytes");
        return;
    }
    if (key.size() != 16 && key.size() != 24 && key.size() != 32) throw std::runtime_error("AES key must be 16, 24, or 32 bytes");
}

void validate_iv(const ByteVector& iv, AesMode mode) {
    const auto required = required_iv_size(mode);
    if (required == 0) {
        if (!iv.empty()) throw std::runtime_error("ECB mode must not use IV/nonce");
        return;
    }
    if (iv.size() != required) throw std::runtime_error("Invalid IV/nonce size for AES-" + aes_mode_to_string(mode) + ": expected " + std::to_string(required) + " bytes, got " + std::to_string(iv.size()) + " bytes");
}

ByteVector aes_encrypt(const AesRequest& request) {
    validate_aes_key(request.key, request.mode);
    validate_iv(request.iv, request.mode);
    if (!is_aead_mode(request.mode) && !request.aad.empty()) throw std::runtime_error("AAD is only supported for AEAD modes GCM and CCM");
    if (request.mode == AesMode::Ecb && !request.allow_ecb && request.input.size() > 16 * 1024) throw std::runtime_error("ECB is blocked for inputs larger than 16 KiB unless --allow-ecb is used");
    switch (request.mode) {
        case AesMode::Ecb: return encrypt_stream_mode<CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption>(request);
        case AesMode::Cbc: return encrypt_stream_mode<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption>(request);
        case AesMode::Cfb: return encrypt_stream_mode<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption>(request);
        case AesMode::Ofb: return encrypt_stream_mode<CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption>(request);
        case AesMode::Ctr: return encrypt_stream_mode<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption>(request);
        case AesMode::Gcm: return encrypt_gcm(request);
        case AesMode::Ccm: return encrypt_ccm(request);
        case AesMode::Xts: throw std::runtime_error("AES-XTS is reserved for the next Lab 1 patch");
    }
    throw std::runtime_error("Unknown AES mode");
}

ByteVector aes_decrypt(const AesRequest& request) {
    validate_aes_key(request.key, request.mode);
    validate_iv(request.iv, request.mode);
    if (!is_aead_mode(request.mode) && !request.aad.empty()) throw std::runtime_error("AAD is only supported for AEAD modes GCM and CCM");
    switch (request.mode) {
        case AesMode::Ecb: return decrypt_stream_mode<CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption>(request);
        case AesMode::Cbc: return decrypt_stream_mode<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption>(request);
        case AesMode::Cfb: return decrypt_stream_mode<CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption>(request);
        case AesMode::Ofb: return decrypt_stream_mode<CryptoPP::OFB_Mode<CryptoPP::AES>::Decryption>(request);
        case AesMode::Ctr: return decrypt_stream_mode<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption>(request);
        case AesMode::Gcm: return decrypt_gcm(request);
        case AesMode::Ccm: return decrypt_ccm(request);
        case AesMode::Xts: throw std::runtime_error("AES-XTS is reserved for the next Lab 1 patch");
    }
    throw std::runtime_error("Unknown AES mode");
}

bool run_lab1_basic_selftest() {
    const ByteVector key = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    const ByteVector iv = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    const ByteVector plaintext = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
    for (AesMode mode : {AesMode::Cbc, AesMode::Cfb, AesMode::Ofb, AesMode::Ctr}) {
        AesRequest r;
        r.mode = mode; r.key = key; r.iv = iv; r.input = plaintext;
        const auto c = aes_encrypt(r);
        r.input = c;
        if (aes_decrypt(r) != plaintext) return false;
    }
    AesRequest gcm;
    gcm.mode = AesMode::Gcm;
    gcm.key = std::vector<std::uint8_t>(32, 0x11);
    gcm.iv = std::vector<std::uint8_t>(12, 0x22);
    gcm.aad = {'a','a','d'};
    gcm.input = {'h','e','l','l','o'};
    const auto c = aes_encrypt(gcm);
    gcm.input = c;
    return aes_decrypt(gcm) == std::vector<std::uint8_t>({'h','e','l','l','o'});
}

}
