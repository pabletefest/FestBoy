# CMakeList.txt : CMake project for FestBoy, include source and define
# project specific logic here.
#

cmake_minimum_required (VERSION 3.8)

find_package(SDL2 REQUIRED)

# Add source to this project's executable.
add_executable (FestBoy "FestBoy.cpp" )

target_link_libraries(FestBoy PRIVATE SDL2::SDL2 SDL2::SDL2main)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FestBoy PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
