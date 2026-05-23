find_package(Catch2 CONFIG REQUIRED)

add_executable(nt219_common_tests
    common/test_hex.cpp
    common/test_base64.cpp
    common/test_encoding.cpp
    common/test_json_utils.cpp
    common/test_random.cpp
    common/test_sample_generator.cpp
    common/test_stats.cpp
    lab1/test_aes_crypto.cpp
    lab1/test_aes_crypto_vectors.cpp
    lab1/test_aes_crypto_negative.cpp
    lab1/test_aes_crypto_completion.cpp
)

target_link_libraries(nt219_common_tests
    PRIVATE
        nt219_common
        nt219_lab1_aes
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(nt219_common_tests)
