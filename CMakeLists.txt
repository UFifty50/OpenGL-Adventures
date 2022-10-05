cmake_minimum_required(VERSION 3.10)

#if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
#    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory and run CMake from there. You may need to remove CMakeCache.txt." )
#endif()

set(PROJECT_NAME AnEngine)
project(${PROJECT_NAME} VERSION 0.0.1)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin
        CACHE PATH
        "Single Directory for all"
)
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin
        CACHE PATH
        "Single Directory for all"
)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/lib
        CACHE PATH
        "Single Directory for all"
)

set(objloader "OBJLOADER")

if(${objloader} STREQUAL "ASSIMP")
        add_library(assimp::assimp ALIAS assimp)
        set(assimp_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparty/assimp/include)
        add_dependencies(assimp glfw::glfw glm::glm glew::glew)
        add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/assimp EXCLUDE_FROM_ALL)
        set(assimp assimp::assimp)
endif()
#add_definitions(-DGLEW_STATIC)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glew/build/cmake EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glfw EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glm EXCLUDE_FROM_ALL)

set(${PROJECT_NAME}_sources
    ${SOURCE_DIR}/cpp/main.cpp
    ${SOURCE_DIR}/cpp/objloader.cpp
    ${SOURCE_DIR}/cpp/shader.cpp
    ${SOURCE_DIR}/cpp/controls.cpp
    ${SOURCE_DIR}/cpp/texture.cpp
    ${SOURCE_DIR}/cpp/window.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources})

set(glew_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparty/glew/include)
set(glfw_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparty/glfw/include)
set(glm_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparty/glm/glm)
set(INCLUDE_DIRS ${SOURCE_DIR}/include ${assimp_INCLUDE_DIRS} ${glew_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})
include_directories(${SOURCE_DIR}/include ${assimp_INCLUDE_DIRS} ${glew_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})

# TODO: uncomment if have unit tests
# enable_testing()
find_program(GCC NAMES gcc)
find_program(CXX NAMES c++)
# NOTE: you can add search paths for example `PATHS /usr/local/opt/llvm/bin/`
message("\n--------------------------------\n")
message("PROJECT NAME:\t\t${PROJECT_NAME}")
message("CMAKE_SYSTEM_NAME:\t${CMAKE_SYSTEM_NAME}")
message("CMAKE_C_COMPILER:\t${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER:\t${CMAKE_CXX_COMPILER}")
message("CMAKE_GENERATOR:\t${CMAKE_GENERATOR}")
message("CMAKE_BUILD_TYPE:\t${CMAKE_BUILD_TYPE}")
message("CMAKE_BINARY_DIR:\t${CMAKE_BINARY_DIR}")
message("SOURCE_DIR:\t${SOURCE_DIR}")
message("INCLUDE_DIRS:\t${INCLUDE_DIRS}")
message("gcc:\t\t${GCC}")
message("g++:\t\t${CXX}")
message("\n--------------------------------\n")

# NOTE: this will slow down compilation, but you'll have static code analysis :)
if(GCC)
    set_property(
        TARGET ${PROJECT_NAME}
        PROPERTY CXX_GCC "${GCC}")
endif()
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)


add_library(glew::glew ALIAS glew)
add_library(glfw::glfw ALIAS glfw)
add_library(glm::glm ALIAS glm)

add_dependencies(${PROJECT_NAME} ${assimp} glfw::glfw glm::glm glew::glew)
target_link_libraries(${PROJECT_NAME} ${assimp} glfw::glfw glm::glm glew::glew)

configure_file(${SOURCE_DIR}/include/AnEngineConfig.h.in ${SOURCE_DIR}/include/AnEngineConfig.hpp)

# enable debug systems
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug mode")
    add_definitions(-DDEBUG)
endif()

# after compilation, copy shaders, textures and objects into into build directory
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/bin/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_DIR}/textures
        ${CMAKE_BINARY_DIR}/bin/textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOURCE_DIR}/obj
        ${CMAKE_BINARY_DIR}/bin/obj
)
