#include "common/cli.hpp"
#include "common/encoding.hpp"
#include "common/file_utils.hpp"
#include "common/hex.hpp"
#include "common/json_utils.hpp"
#include "common/random.hpp"
#include "common/stats.hpp"
#include "lab1/aes_crypto.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using ByteVector = std::vector<std::uint8_t>;

void print_aestool_help() {
    std::cout
        << "aestool\n"
        << "Lab 1 - Symmetric Encryption with Crypto++\n\n"
        << "Commands:\n"
        << "  keygen   --size 16|24|32 --out key.bin [--encode raw|hex|base64]\n"
        << "  encrypt  --mode ecb|cbc|cfb|ofb|ctr|gcm|ccm --key KEYFILE|--key-hex HEX [--iv-hex HEX|--nonce-hex HEX] [--in FILE|--text TEXT] [--out FILE] [--meta FILE] [--encode hex|base64|raw]\n"
        << "  decrypt  --mode ecb|cbc|cfb|ofb|ctr|gcm|ccm --key KEYFILE|--key-hex HEX [--iv-hex HEX|--nonce-hex HEX|--meta FILE] --in FILE [--out FILE]\n"
        << "  kat      --kat vectors.json\n"
        << "  bench    --mode MODE --key KEYFILE|--key-hex HEX [--size BYTES] [--runs N]\n"
        << "  selftest\n"
        << "  version\n\n"
        << "Notes:\n"
        << "  - CBC/CFB/OFB/CTR use 16-byte IV.\n"
        << "  - GCM/CCM use 12-byte nonce by default.\n"
        << "  - GCM/CCM ciphertext files contain ciphertext || authentication tag.\n"
        << "  - If IV/nonce is omitted during encryption, aestool generates it securely and saves it to --meta JSON.\n"
        << "  - ECB prints a warning and blocks inputs larger than 16 KiB unless --allow-ecb is used.\n"
        << "  - XTS is reserved for the next Lab 1 patch.\n";
}

std::size_t parse_size_value(const std::string& text, std::size_t default_value) {
    if (text.empty()) return default_value;
    std::size_t position = 0;
    const auto value = std::stoull(text, &position);
    if (position != text.size()) throw std::runtime_error("Invalid numeric value: " + text);
    return static_cast<std::size_t>(value);
}

ByteVector string_to_bytes(const std::string& text) { return ByteVector(text.begin(), text.end()); }

ByteVector load_input_bytes(const nt219::CliParser& cli) {
    const auto in_path = cli.get_option("in");
    const auto text = cli.get_option("text");
    if (!in_path.empty() && !text.empty()) throw std::runtime_error("Use either --in or --text, not both");
    if (!in_path.empty()) return nt219::read_binary_file(in_path);
    if (!text.empty()) return string_to_bytes(text);
    throw std::runtime_error("Missing input. Use --in FILE or --text TEXT");
}

ByteVector load_key(const nt219::CliParser& cli, nt219::lab1::AesMode mode) {
    const auto key_path = cli.get_option("key");
    const auto key_hex = cli.get_option("key-hex");
    if (!key_path.empty() && !key_hex.empty()) throw std::runtime_error("Use either --key or --key-hex, not both");
    ByteVector key;
    if (!key_path.empty()) key = nt219::read_binary_file(key_path);
    else if (!key_hex.empty()) key = nt219::hex_decode(key_hex);
    else throw std::runtime_error("Missing key. Use --key KEYFILE or --key-hex HEX");
    nt219::lab1::validate_aes_key(key, mode);
    return key;
}

ByteVector load_aad(const nt219::CliParser& cli) {
    const auto aad_path = cli.get_option("aad");
    const auto aad_text = cli.get_option("aad-text");
    if (!aad_path.empty() && !aad_text.empty()) throw std::runtime_error("Use either --aad or --aad-text, not both");
    if (!aad_path.empty()) return nt219::read_binary_file(aad_path);
    if (!aad_text.empty()) return string_to_bytes(aad_text);
    return {};
}

ByteVector load_iv_from_cli_or_meta(const nt219::CliParser& cli, nt219::lab1::AesMode mode, bool encrypting) {
    if (!nt219::lab1::mode_requires_iv(mode)) return {};
    const auto iv_hex = cli.get_option("iv-hex");
    const auto nonce_hex = cli.get_option("nonce-hex");
    const auto iv_path = cli.get_option("iv");
    const auto nonce_path = cli.get_option("nonce");
    const auto meta_path = cli.get_option("meta");
    int supplied = 0;
    supplied += !iv_hex.empty() ? 1 : 0;
    supplied += !nonce_hex.empty() ? 1 : 0;
    supplied += !iv_path.empty() ? 1 : 0;
    supplied += !nonce_path.empty() ? 1 : 0;
    if (supplied > 1) throw std::runtime_error("Use only one of --iv-hex, --nonce-hex, --iv, or --nonce");
    ByteVector iv;
    if (!iv_hex.empty()) iv = nt219::hex_decode(iv_hex);
    else if (!nonce_hex.empty()) iv = nt219::hex_decode(nonce_hex);
    else if (!iv_path.empty()) iv = nt219::read_binary_file(iv_path);
    else if (!nonce_path.empty()) iv = nt219::read_binary_file(nonce_path);
    else if (!meta_path.empty() && !encrypting) {
        const auto meta = nt219::read_json_file(meta_path);
        if (meta.contains("iv_hex")) iv = nt219::hex_decode(meta.at("iv_hex").get<std::string>());
        else if (meta.contains("nonce_hex")) iv = nt219::hex_decode(meta.at("nonce_hex").get<std::string>());
        else throw std::runtime_error("Metadata file does not contain iv_hex or nonce_hex");
    } else if (encrypting) iv = nt219::random_bytes_cryptopp(nt219::lab1::required_iv_size(mode));
    else throw std::runtime_error("Missing IV/nonce. Use --iv-hex, --nonce-hex, --iv, --nonce, or --meta");
    nt219::lab1::validate_iv(iv, mode);
    return iv;
}

std::size_t load_tag_size(const nt219::CliParser& cli, nt219::lab1::AesMode mode, bool decrypting) {
    if (!nt219::lab1::is_aead_mode(mode)) return 16;

    const auto tag_size_text = cli.get_option("tag-size");
    if (!tag_size_text.empty()) {
        const auto tag_size = parse_size_value(tag_size_text, 16);
        if (tag_size < 8 || tag_size > 16) throw std::runtime_error("AEAD tag size must be between 8 and 16 bytes");
        return tag_size;
    }

    const auto meta_path = cli.get_option("meta");
    if (decrypting && !meta_path.empty()) {
        const auto meta = nt219::read_json_file(meta_path);
        if (meta.contains("tag_size")) return meta.at("tag_size").get<std::size_t>();
    }

    return 16;
}

void write_output_or_console(const nt219::CliParser& cli, const ByteVector& output, nt219::Encoding default_console_encoding) {
    const auto out_path = cli.get_option("out");
    const auto encode_text = cli.get_option("encode");
    if (!out_path.empty()) {
        if (encode_text.empty() || encode_text == "raw") nt219::write_binary_file(out_path, output);
        else nt219::write_text_file(out_path, nt219::encode_bytes(output, nt219::parse_encoding(encode_text)));
        return;
    }
    const auto encoding = encode_text.empty() ? default_console_encoding : nt219::parse_encoding(encode_text);
    std::cout << nt219::encode_bytes(output, encoding) << "\n";
}

void write_metadata(const nt219::CliParser& cli, nt219::lab1::AesMode mode, const ByteVector& key, const ByteVector& iv, const ByteVector& aad, std::size_t tag_size) {
    const auto meta_path = cli.get_option("meta");
    if (meta_path.empty()) return;
    nlohmann::json meta;
    meta["tool"] = "aestool";
    meta["lab"] = 1;
    meta["algorithm"] = "AES";
    meta["mode"] = nt219::lab1::aes_mode_to_string(mode);
    meta["key_bits"] = key.size() * 8;
    meta["aead"] = nt219::lab1::is_aead_mode(mode);
    if (nt219::lab1::is_aead_mode(mode)) meta["tag_size"] = tag_size;
    if (nt219::lab1::mode_requires_iv(mode)) {
        if (nt219::lab1::is_aead_mode(mode)) meta["nonce_hex"] = nt219::hex_encode(iv);
        else meta["iv_hex"] = nt219::hex_encode(iv);
    }
    if (!aad.empty()) meta["aad_hex"] = nt219::hex_encode(aad);
    nt219::write_json_file(meta_path, meta);
}

int command_keygen(const nt219::CliParser& cli) {
    const auto size = parse_size_value(cli.get_option("size"), 32);
    if (size != 16 && size != 24 && size != 32) throw std::runtime_error("AES key size must be 16, 24, or 32 bytes");
    const auto key = nt219::random_bytes_cryptopp(size);
    write_output_or_console(cli, key, nt219::Encoding::Hex);
    return 0;
}

int command_encrypt(const nt219::CliParser& cli) {
    const auto mode = nt219::lab1::parse_aes_mode(cli.get_option("mode", "gcm"));
    if (mode == nt219::lab1::AesMode::Ecb) std::cerr << "WARNING: ECB mode is insecure and leaks plaintext patterns.\n";
    nt219::lab1::AesRequest request;
    request.mode = mode;
    request.key = load_key(cli, mode);
    request.iv = load_iv_from_cli_or_meta(cli, mode, true);
    request.aad = load_aad(cli);
    request.input = load_input_bytes(cli);
    request.allow_ecb = cli.has_flag("allow-ecb");
    request.tag_size = load_tag_size(cli, mode, false);
    const auto ciphertext = nt219::lab1::aes_encrypt(request);
    write_metadata(cli, mode, request.key, request.iv, request.aad, request.tag_size);
    write_output_or_console(cli, ciphertext, nt219::Encoding::Hex);
    if (cli.has_flag("verbose")) {
        std::cerr << "mode=" << nt219::lab1::aes_mode_to_string(mode) << "\n";
        std::cerr << "key_bits=" << request.key.size() * 8 << "\n";
        if (!request.iv.empty()) std::cerr << "iv_or_nonce_hex=" << nt219::hex_encode(request.iv) << "\n";
        std::cerr << "input_bytes=" << request.input.size() << "\n";
        std::cerr << "output_bytes=" << ciphertext.size() << "\n";
    }
    return 0;
}

int command_decrypt(const nt219::CliParser& cli) {
    const auto mode = nt219::lab1::parse_aes_mode(cli.get_option("mode", "gcm"));
    nt219::lab1::AesRequest request;
    request.mode = mode;
    request.key = load_key(cli, mode);
    request.iv = load_iv_from_cli_or_meta(cli, mode, false);
    request.aad = load_aad(cli);
    request.input = load_input_bytes(cli);
    request.allow_ecb = true;
    request.tag_size = load_tag_size(cli, mode, true);
    const auto plaintext = nt219::lab1::aes_decrypt(request);
    write_output_or_console(cli, plaintext, nt219::Encoding::Raw);
    return 0;
}

int command_kat(const nt219::CliParser& cli) {
    const auto kat_path = cli.get_option("kat");
    if (kat_path.empty()) throw std::runtime_error("Missing --kat vectors.json");
    const auto vectors = nt219::read_json_file(kat_path);
    const auto cases = vectors.at("cases");
    std::size_t passed = 0, failed = 0;
    for (const auto& item : cases) {
        const auto name = item.value("name", "unnamed");
        const auto mode = nt219::lab1::parse_aes_mode(item.at("mode").get<std::string>());
        nt219::lab1::AesRequest request;
        request.mode = mode;
        request.key = nt219::hex_decode(item.at("key").get<std::string>());
        request.input = nt219::hex_decode(item.at("plaintext").get<std::string>());
        request.allow_ecb = true;
        if (item.contains("iv")) request.iv = nt219::hex_decode(item.at("iv").get<std::string>());
        if (item.contains("aad")) request.aad = nt219::hex_decode(item.at("aad").get<std::string>());
        if (item.contains("tag_size")) request.tag_size = item.at("tag_size").get<std::size_t>();
        const auto expected = nt219::hex_decode(item.at("ciphertext").get<std::string>());
        const auto actual = nt219::lab1::aes_encrypt(request);
        if (actual == expected) { ++passed; std::cout << "[PASS] " << name << "\n"; }
        else { ++failed; std::cout << "[FAIL] " << name << "\n"; std::cout << "  expected: " << nt219::hex_encode(expected) << "\n"; std::cout << "  actual:   " << nt219::hex_encode(actual) << "\n"; }
    }
    std::cout << "Summary: passed=" << passed << " failed=" << failed << "\n";
    return failed == 0 ? 0 : 1;
}

int command_bench(const nt219::CliParser& cli) {
    const auto mode = nt219::lab1::parse_aes_mode(cli.get_option("mode", "gcm"));
    const auto size = parse_size_value(cli.get_option("size"), 1024 * 1024);
    const auto runs = parse_size_value(cli.get_option("runs"), 30);
    if (runs == 0) throw std::runtime_error("--runs must be greater than zero");
    nt219::lab1::AesRequest request;
    request.mode = mode;
    request.key = load_key(cli, mode);
    request.iv = load_iv_from_cli_or_meta(cli, mode, true);
    request.input = nt219::random_bytes_openssl(size);
    request.allow_ecb = cli.has_flag("allow-ecb");
    request.tag_size = load_tag_size(cli, mode, false);
    std::vector<double> samples_ms;
    samples_ms.reserve(runs);
    for (std::size_t i = 0; i < runs; ++i) {
        const auto start = std::chrono::steady_clock::now();
        const auto output = nt219::lab1::aes_encrypt(request);
        const auto end = std::chrono::steady_clock::now();
        (void)output;
        samples_ms.push_back(std::chrono::duration<double, std::milli>(end - start).count());
    }
    const auto summary = nt219::summarize_samples(samples_ms);
    const double mib = static_cast<double>(size) / (1024.0 * 1024.0);
    const double throughput_mib_s = mib / (summary.mean / 1000.0);
    std::cout << "mode,size_bytes,runs,mean_ms,median_ms,stddev_ms,ci95_low_ms,ci95_high_ms,throughput_mib_s\n";
    std::cout << nt219::lab1::aes_mode_to_string(mode) << "," << size << "," << runs << "," << summary.mean << "," << summary.median << "," << summary.stddev << "," << summary.ci95_low << "," << summary.ci95_high << "," << throughput_mib_s << "\n";
    return 0;
}
}

int main(int argc, char** argv) {
    try {
        nt219::CliParser cli(argc, argv);
        if (argc == 1 || cli.has_flag("help")) { print_aestool_help(); return 0; }
        const std::string command = cli.command();
        if (command == "version") { std::cout << "aestool version 0.2.0\n"; return 0; }
        if (command == "selftest") {
            const bool ok = nt219::lab1::run_lab1_basic_selftest();
            std::cout << "aestool selftest: " << (ok ? "PASS" : "FAIL") << "\n";
            return ok ? 0 : 1;
        }
        if (command == "keygen") return command_keygen(cli);
        if (command == "encrypt") return command_encrypt(cli);
        if (command == "decrypt") return command_decrypt(cli);
        if (command == "kat") return command_kat(cli);
        if (command == "bench") return command_bench(cli);
        std::cerr << "Unsupported command: " << command << "\n";
        std::cerr << "Use --help to see available commands.\n";
        return 1;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
}
