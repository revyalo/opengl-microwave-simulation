cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME practica_micro)
set(CODE_PATH src)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ../binary)

project(${PROJECT_NAME})
set(GLEW_PATH      /usr/local/Cellar/glew/2.2.0_1)
set(GLFW_PATH      /usr/local/Cellar/glfw/3.4)
set(GLM_PATH       /usr/local/Cellar/glm/1.0.1)
set(ASSIMP_PATH    /usr/local/Cellar/assimp/6.0.2)
set(FREEIMAGE_PATH /usr/local/Cellar/freeimage/3.18.0)
include_directories(${CODE_PATH} ${GLEW_PATH}/include ${GLFW_PATH}/include ${GLM_PATH}/include ${ASSIMP_PATH}/include ${FREEIMAGE_PATH}/include)
link_directories(${GLEW_PATH}/lib ${GLFW_PATH}/lib ${GLM_PATH}/lib ${ASSIMP_PATH}/lib ${FREEIMAGE_PATH}/lib)
file(GLOB_RECURSE CODE_FILES ${CODE_PATH}/*.*)
add_executable(${PROJECT_NAME} ${CODE_FILES}
        src/Texture.cpp)
target_link_libraries(${PROJECT_NAME} "-framework OpenGL" libGLEW.2.2.0.dylib libglfw.3.4.dylib libassimp.6.0.2.dylib libfreeimage.3.18.0.dylib)
