cmake_minimum_required(VERSION 3.16)
project(a_life_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(main
  main.cpp
  ${SRC_FILES}
)

target_include_directories(main PRIVATE
  "${CMAKE_SOURCE_DIR}/src/headers"
  "${CMAKE_SOURCE_DIR}/src/interfaces"
  "${CMAKE_SOURCE_DIR}/src/util"
)

target_compile_options(main PRIVATE -Wall -Wextra)

target_link_libraries(main PRIVATE
  ftxui::component
  ftxui::dom
  ftxui::screen
)