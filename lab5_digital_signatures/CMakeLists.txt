add_executable(sigtool
    src/main.cpp
)

target_include_directories(sigtool
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(sigtool
    PRIVATE
        nt219_common
        OpenSSL::SSL
        OpenSSL::Crypto
        ${NT219_CRYPTOPP_TARGET}
)

target_compile_features(sigtool PRIVATE cxx_std_17)

