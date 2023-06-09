﻿# CMakeList.txt : CMake project for FestBoy, include source and define
# project specific logic here.
#

cmake_minimum_required (VERSION 3.8)

include(FetchContent)

find_package(SDL2 REQUIRED)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "main.cpp"  "src/cpu_sm83.cpp" "inc/instructions.h" "inc/gb.h" "src/gb.cpp" "inc/game_pack.h" "src/game_pack.cpp" "inc/mapper.h" "src/mapper.cpp" "inc/no_mbc.h" "src/no_mbc.cpp" "inc/opcodesTests.h" "inc/timer.h" "src/timer.cpp" "src/ppu.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE inc/)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main nlohmann_json::nlohmann_json)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
