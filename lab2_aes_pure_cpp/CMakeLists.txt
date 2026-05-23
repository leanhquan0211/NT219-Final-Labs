add_executable(aesctrtool
    src/main.cpp
)

target_include_directories(aesctrtool
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(aesctrtool
    PRIVATE
        nt219_common
        OpenSSL::SSL
        OpenSSL::Crypto
        ${NT219_CRYPTOPP_TARGET}
)

target_compile_features(aesctrtool PRIVATE cxx_std_17)

