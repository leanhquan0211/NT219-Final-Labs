cmake_minimum_required(VERSION 3.22)

project(NT219_Final_Cryptography_Labs
    VERSION 0.1.0
    DESCRIPTION "NT219 Cryptography and Applications Final Labs"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NT219_BUILD_TESTS "Build unit tests" ON)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

find_package(cryptopp CONFIG QUIET)

if(TARGET cryptopp::cryptopp)
    set(NT219_CRYPTOPP_TARGET cryptopp::cryptopp)
else()
    find_path(CRYPTOPP_INCLUDE_DIR
        NAMES cryptlib.h
        PATH_SUFFIXES crypto++ cryptopp
    )

    find_library(CRYPTOPP_LIBRARY
        NAMES cryptopp crypto++
    )

    if(NOT CRYPTOPP_INCLUDE_DIR OR NOT CRYPTOPP_LIBRARY)
        message(FATAL_ERROR "Crypto++ was not found. Please install libcrypto++-dev on Ubuntu or cryptopp via vcpkg on Windows.")
    endif()

    add_library(nt219_cryptopp UNKNOWN IMPORTED)
    set_target_properties(nt219_cryptopp PROPERTIES
        IMPORTED_LOCATION "${CRYPTOPP_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${CRYPTOPP_INCLUDE_DIR}"
    )

    set(NT219_CRYPTOPP_TARGET nt219_cryptopp)
endif()

add_subdirectory(common)
add_subdirectory(lab1_aes_cryptopp)
add_subdirectory(lab2_aes_pure_cpp)
add_subdirectory(lab3_rsa_hybrid)
add_subdirectory(lab4_hash_pki_attacks)
add_subdirectory(lab5_digital_signatures)
add_subdirectory(lab6_pqc)

if(NT219_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
