add_library(student_solution INTERFACE
    vector.hpp
)

target_include_directories(student_solution INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(student_solution INTERFACE cxx_std_20)