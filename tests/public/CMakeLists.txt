include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

file(GLOB_RECURSE PUBLIC_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(public_tests ${PUBLIC_TEST_SOURCES})
target_link_libraries(public_tests PRIVATE student_solution GTest::gtest_main)

gtest_discover_tests(public_tests)