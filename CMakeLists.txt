cmake_minimum_required(VERSION 3.25)

project(wtop VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(wtop
    src/main.cpp
    src/memory.cpp
)

target_include_directories(wtop PRIVATE src)

target_compile_features(wtop PRIVATE cxx_std_26)

set_target_properties(wtop PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
