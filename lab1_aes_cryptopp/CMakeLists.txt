add_library(nt219_lab1_aes
    src/aes_crypto.cpp
)
set_target_properties(nt219_lab1_aes PROPERTIES POSITION_INDEPENDENT_CODE ON)

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

add_library(nt219_lab1_aes_shared SHARED
    src/lab1_c_api.cpp
)

target_include_directories(nt219_lab1_aes_shared
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(nt219_lab1_aes_shared
    PRIVATE
        nt219_lab1_aes
        nt219_common
        ${NT219_CRYPTOPP_TARGET}
)

target_compile_features(nt219_lab1_aes_shared PRIVATE cxx_std_17)

target_compile_definitions(nt219_lab1_aes_shared
    PRIVATE
        NT219_LAB1_AES_SHARED_EXPORTS
)

set_target_properties(nt219_lab1_aes_shared PROPERTIES
    OUTPUT_NAME nt219_lab1_aes_shared
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)

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


set_target_properties(nt219_lab1_aes_shared PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
