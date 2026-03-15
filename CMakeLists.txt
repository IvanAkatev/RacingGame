cmake_minimum_required(VERSION 3.16)

# Use vcpkg toolchain if VCPKG_ROOT is set (must be BEFORE project()).
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(racing_sfml3_vcpkg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
  set(VCPKG_APPLOCAL_DEPS ON)
endif()

# ---- SFML 3.x (CONFIG mode through vcpkg) ----
find_package(SFML CONFIG REQUIRED COMPONENTS Graphics Window System)

add_executable(racing
  src/main.cpp
  src/app/Application.cpp
  src/app/RendererSFML.cpp

  src/core/GameModel.cpp
  src/core/track/CircleTrack.cpp
  src/core/track/StraightTrack.cpp
  src/core/vehicle/Vehicle.cpp
)

target_include_directories(racing PRIVATE src)
target_link_libraries(racing PRIVATE SFML::Graphics SFML::Window SFML::System)

if (MSVC)
  target_compile_options(racing PRIVATE /W4)
else()
  target_compile_options(racing PRIVATE -Wall -Wextra -Wpedantic)
endif()
