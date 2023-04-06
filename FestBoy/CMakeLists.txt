# CMakeList.txt : CMake project for FestBoy, include source and define
# project specific logic here.
#

cmake_minimum_required (VERSION 3.8)

find_package(SDL2 REQUIRED)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "main.cpp"  "src/cpuSM83.cpp" "inc/instructions.h" "inc/gb.h" "src/gb.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE inc/)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
