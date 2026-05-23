add_library(nt219_common
    src/cli.cpp
    src/file_utils.cpp
    src/hex.cpp
    src/base64.cpp
    src/timer.cpp
    src/error.cpp
)

target_include_directories(nt219_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(nt219_common
    PUBLIC
        nlohmann_json::nlohmann_json
)

target_compile_features(nt219_common PUBLIC cxx_std_17)
