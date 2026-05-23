add_executable(rsatool
    src/main.cpp
)

target_include_directories(rsatool
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(rsatool
    PRIVATE
        nt219_common
        OpenSSL::SSL
        OpenSSL::Crypto
        ${NT219_CRYPTOPP_TARGET}
)

target_compile_features(rsatool PRIVATE cxx_std_17)

