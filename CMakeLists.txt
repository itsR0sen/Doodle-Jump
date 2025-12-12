cmake_minimum_required(VERSION 3.10)
project(CarDodgeGame)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML 2.4+
find_package(SFML 2.4 REQUIRED COMPONENTS graphics window system)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/SceneManager.cpp
    src/Utils.cpp
)

# Header files
set(HEADERS
    include/Game.h
    include/Player.h
    include/Enemy.h
    include/SceneManager.h
    include/Utils.h
)

# Main executable
add_executable(CarDodgeGame ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(CarDodgeGame PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Include directories
target_include_directories(CarDodgeGame PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Copy assets to build directory
add_custom_command(TARGET CarDodgeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Assets
    $<TARGET_FILE_DIR:CarDodgeGame>/Assets
    COMMENT "Copying assets..."
)