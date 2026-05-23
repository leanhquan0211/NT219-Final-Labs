add_library(nt219_lab1_aes
    src/aes_crypto.cpp
)

target_include_directories(nt219_lab1_aes
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(nt219_lab1_aes
    PUBLIC
        nt219_common
        ${NT219_CRYPTOPP_TARGET}
)

target_compile_features(nt219_lab1_aes PUBLIC cxx_std_17)

add_executable(aestool
    src/main.cpp
)

target_link_libraries(aestool
    PRIVATE
        nt219_lab1_aes
        nt219_common
        OpenSSL::SSL
        OpenSSL::Crypto
        ${NT219_CRYPTOPP_TARGET}
)

target_compile_features(aestool PRIVATE cxx_std_17)
