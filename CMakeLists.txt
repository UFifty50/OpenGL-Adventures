cmake_minimum_required(VERSION 3.10)
include(ExternalProject)
#if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
#    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory and run CMake from there. You may need to remove CMakeCache.txt." )
#endif()

set(PROJECT_NAME AnEngine)
project(${PROJECT_NAME} VERSION 0.0.1)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)


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

set(objloader "ASSIMP")

add_definitions(-DGLEW_STATIC)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/assimp EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glew EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glfw EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/glm EXCLUDE_FROM_ALL)

set(${PROJECT_NAME}_sources
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/objloader.cpp
    ${SOURCE_DIR}/shader.cpp
    ${SOURCE_DIR}/controls.cpp
    ${SOURCE_DIR}/texture.cpp
    ${SOURCE_DIR}/window.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources})

include_directories(${SOURCE_DIR}/include ${assimp_INCLUDE_DIRS} ${glew_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS})

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
message("gcc:\t\t${GCC}")
message("g++:\t\t${CXX}")
message("\n--------------------------------\n")

# NOTE: this will slow down compilation, but you'll have static code analysis :)
if(GCC)
    set_property(
        TARGET ${PROJECT_NAME}
        PROPERTY CXX_GCC "${GCC}")
endif()
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)


# include the glew submodule
#ExternalProject_Add(project_glew
#  PREFIX ${CMAKE_SOURCE_DIR}/thirdparty/glew
#  SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/glew
#  TMP_DIR ${CMAKE_SOURCE_DIR}/obj/
#  BINARY_DIR ${CMAKE_SOURCE_DIR}/obj/
#  INSTALL_DIR ${CMAKE_SOURCE_DIR}/obj/
#  CONFIGURE_COMMAND ""
#  BUILD_COMMAND cmake . -G "Ninja"
#  BUILD_IN_SOURCE false
#  INSTALL_COMMAND ninja
#  IMPORTED_LOCATION
#)
add_library(glew::glew ALIAS glew)
add_library(glfw::glfw ALIAS glfw)
add_library(glm::assimp ALIAS glm)
add_library(assimp::assimp ALIAS assimp)

add_dependencies(assimp glfw::glfw glm::glm glew::glew)
add_dependencies(${PROJECT_NAME} assimp::assimp glfw::glfw glm::glm glew::glew)
target_link_libraries(${PROJECT_NAME} assimp::assimp glfw::glfw glm::glm glew::glew)

configure_file(${SOURCE_DIR}/include/AnEngineConfig.h.in ${SOURCE_DIR}/include/AnEngineConfig.hpp)
