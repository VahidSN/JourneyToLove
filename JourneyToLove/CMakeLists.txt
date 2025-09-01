cmake_minimum_required(VERSION 3.15)
project(JourneyToLove)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable vcpkg support
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

# Find Raylib
find_package(raylib CONFIG REQUIRED)

# List sources (including .rc!)
set(SOURCES
    src/main.cpp
    src/game/Player.cpp
    src/game/Player.h
    src/game/Scene.cpp
    src/game/Scene.h
    src/core/GameObject.cpp
    src/core/GameObject.h
    src/core/Renderer.cpp
    src/core/Renderer.h
    src/game/Level.cpp
    src/game/Level.h
    src/core/GameSetting.cpp
    src/core/GameSetting.h
    src/game/CollisionUtils.cpp
    src/game/CollisionUtils.h
    src/core/CSVReader.cpp
    src/core/CSVReader.h
    src/core/Button.cpp
    src/core/Button.h
    src/game/UIController.cpp
    src/game/UIController.h
    resources.rc
)

# Add executable with sources
add_executable(JourneyToLove ${SOURCES})

# Copy assets folder to the build directory
add_custom_command(TARGET JourneyToLove POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:JourneyToLove>/Assets)

# Link Raylib
target_link_libraries(JourneyToLove PRIVATE raylib)

# Add include path
target_include_directories(JourneyToLove PUBLIC
    ${CMAKE_SOURCE_DIR}/src)

