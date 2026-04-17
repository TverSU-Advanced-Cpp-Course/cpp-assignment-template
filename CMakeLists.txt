cmake_minimum_required(VERSION 3.24)
project(cpp_assignment_template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_CLANG_TIDY "Run clang-tidy for student target" OFF)
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UBSanitizer" OFF)
option(BUILD_PUBLIC_TESTS "Build public tests" ON)
set(EXTRA_TESTS_DIR "" CACHE PATH "Path to extra private tests")

if(ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_subdirectory(solution)

if(NOT TARGET student_solution)
    message(FATAL_ERROR "solution/CMakeLists.txt must define target 'student_solution'")
endif()

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if(CLANG_TIDY_EXE)
        set_target_properties(student_solution PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
        )
    else()
        message(WARNING "ENABLE_CLANG_TIDY=ON, but clang-tidy was not found. Continuing without it.")
    endif()
endif()

if(BUILD_PUBLIC_TESTS OR EXTRA_TESTS_DIR)
    enable_testing()
endif()

if(BUILD_PUBLIC_TESTS)
    add_subdirectory(tests/public)
endif()

if(EXTRA_TESTS_DIR)
    if(EXISTS "${EXTRA_TESTS_DIR}/CMakeLists.txt")
        add_subdirectory("${EXTRA_TESTS_DIR}" tests_private)
    else()
        message(FATAL_ERROR "EXTRA_TESTS_DIR does not contain CMakeLists.txt")
    endif()
endif()