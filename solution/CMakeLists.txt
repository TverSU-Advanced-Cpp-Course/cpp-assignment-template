add_library(student_solution STATIC
    placeholder.cpp
    placeholder.hpp
)

target_include_directories(student_solution PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(student_solution PUBLIC cxx_std_20)