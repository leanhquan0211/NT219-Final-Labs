add_executable(pqtool
    src/main.cpp
)

target_include_directories(pqtool
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(pqtool
    PRIVATE
        nt219_common
        OpenSSL::SSL
        OpenSSL::Crypto
        ${NT219_CRYPTOPP_TARGET}
)

target_compile_features(pqtool PRIVATE cxx_std_17)

