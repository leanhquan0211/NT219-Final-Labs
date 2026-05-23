find_package(Catch2 CONFIG REQUIRED)

add_executable(nt219_common_tests
    common/test_hex.cpp
    common/test_base64.cpp
)

target_link_libraries(nt219_common_tests
    PRIVATE
        nt219_common
        Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(nt219_common_tests)
