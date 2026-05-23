add_executable(hashtool
    src/main.cpp
)

target_include_directories(hashtool
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hashtool
    PRIVATE
        nt219_common
        OpenSSL::SSL
        OpenSSL::Crypto
        ${NT219_CRYPTOPP_TARGET}
)

target_compile_features(hashtool PRIVATE cxx_std_17)

