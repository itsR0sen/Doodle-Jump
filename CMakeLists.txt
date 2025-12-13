cmake_minimum_required(VERSION 3.0)
project(CarDodgeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML Path (update if needed)
set(SFML_ROOT "D:/Car-Dodge-Game/external/SFML-2.4.1")

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${SFML_ROOT}/include
)

# Source Files
file(GLOB SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(CarDodge ${SRC_FILES})

# Link SFML
target_link_libraries(CarDodge
    ${SFML_ROOT}/lib/libsfml-graphics.a
    ${SFML_ROOT}/lib/libsfml-window.a
    ${SFML_ROOT}/lib/libsfml-system.a
)

# Copy DLLs after build
add_custom_command(TARGET CarDodge POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${SFML_ROOT}/bin"
    $<TARGET_FILE_DIR:CarDodge>
)
