cmake_minimum_required(VERSION 3.25)
include (FetchContent)

project(wtop VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v7.0.3
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)
set(FETCHCONTENT_QUIET OFF)

FetchContent_MakeAvailable(ftxui)

add_executable(wtop
    src/main.cpp
    src/memory.cpp
    src/cpu.cpp
    src/history.cpp
    src/state.cpp
)

target_link_libraries(wtop
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

target_include_directories(wtop PRIVATE src)

target_compile_features(wtop PRIVATE cxx_std_26)

set_target_properties(wtop PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
