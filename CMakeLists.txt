cmake_minimum_required(VERSION 3.22)

project(OpenGLMicrowaveSimulation VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(
    MICROWAVE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/binary"
    CACHE PATH
    "Directory where the practica_micro executable is written"
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

find_path(FREEIMAGE_INCLUDE_DIR NAMES FreeImage.h REQUIRED)
find_library(FREEIMAGE_LIBRARY NAMES freeimage FreeImage REQUIRED)

if(NOT TARGET FreeImage::FreeImage)
    add_library(FreeImage::FreeImage UNKNOWN IMPORTED)
    set_target_properties(
        FreeImage::FreeImage
        PROPERTIES
            IMPORTED_LOCATION "${FREEIMAGE_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${FREEIMAGE_INCLUDE_DIR}"
    )
endif()

if(TARGET glfw)
    set(GLFW_TARGET glfw)
elseif(TARGET glfw3::glfw)
    set(GLFW_TARGET glfw3::glfw)
else()
    message(FATAL_ERROR "The installed glfw3 package does not provide a supported CMake target")
endif()

if(TARGET assimp::assimp)
    set(ASSIMP_TARGET assimp::assimp)
elseif(TARGET assimp)
    set(ASSIMP_TARGET assimp)
else()
    message(FATAL_ERROR "The installed Assimp package does not provide a supported CMake target")
endif()

add_executable(
    practica_micro
    src/main.cpp
    src/Model.cpp
    src/Shaders.cpp
    src/Texture.cpp
)

target_include_directories(practica_micro PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(
    practica_micro
    PRIVATE
        OpenGL::GL
        GLEW::GLEW
        ${GLFW_TARGET}
        glm::glm
        ${ASSIMP_TARGET}
        FreeImage::FreeImage
)

set_target_properties(
    practica_micro
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${MICROWAVE_RUNTIME_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MICROWAVE_RUNTIME_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MICROWAVE_RUNTIME_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MICROWAVE_RUNTIME_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${MICROWAVE_RUNTIME_OUTPUT_DIRECTORY}"
)
