cmake_minimum_required(VERSION 3.23)
project(opengl1)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)
include_directories(OpenGL)
include_directories(OpenGL/include)
include_directories(OpenGL/lib)
include_directories(OpenGL/include/glm)

add_library(LibsModule lib/glad.c src/io/Keyboard.cpp src/io/Keyboard.h src/io/Mouse.cpp src/io/Mouse.h src/io/Joystick.cpp src/io/Joystick.h)

target_link_libraries(LibsModule libglfw3.a libglfw3dll.a)
target_link_libraries(LibsModule -L${PROJECT_SOURCE_DIR}/OpenGL/lib/)

add_executable(opengl1 src/main.cpp src/Shader.cpp src/Shader.h)
target_link_libraries(opengl1 LibsModule)

target_link_libraries(opengl1 ${OPENGL_LIBRARIES} libglfw3.a libglfw3dll.a)