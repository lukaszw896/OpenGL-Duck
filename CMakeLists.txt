#===============================================================================
# File assures cross-platform compilation. All external libraries are compiled
# in the process. Resources folder is copied to the output directory.
#==============================================================================
# author: dybisz
#-------------------------------------------------------------------------------

#===============================================================================
# General Settings
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.4)
project(Duck)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=gnu++0x")

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib)

#===============================================================================
# External Libraries
#-------------------------------------------------------------------------------
add_subdirectory (external)


include_directories(
        external/AntTweakBar-1.16/include/
        external/glew-1.13.0/include/
        external/glfw-3.1.2/include/GLFW/
        external/glm-0.9.7.1/
        external/inih/cpp/
        external/inih/
        external/SOIL_/
)

set(EXTERNAL_LIBS
        ${OPENGL_LIBRARY}
        SOIL
        glfw
        GLEW_1130
        ANTTWEAKBAR_116_OGLCORE_GLFW
        INIH
        )

#===============================================================================
# Project's Includes
#-------------------------------------------------------------------------------
include_directories(
    include/
)

#===============================================================================
# Project's Sources
#-------------------------------------------------------------------------------
set(
        SOURCE_FILES
        src/main.cpp
)

#===============================================================================
# Executable
#-------------------------------------------------------------------------------
add_executable(duck ${SOURCE_FILES})

#===============================================================================
# Linking
#-------------------------------------------------------------------------------
target_link_libraries (duck ${EXTERNAL_LIBS})

#===============================================================================
# Definitions
#-------------------------------------------------------------------------------
add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

#===============================================================================
# Copy Resources
#-------------------------------------------------------------------------------
add_custom_command(
        TARGET duck PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory   # Copy resources
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:duck>/res
)

#===============================================================================
# Copy Configuration Files
#-------------------------------------------------------------------------------
file(GLOB ConfigFiles ${CMAKE_SOURCE_DIR}/config/*.ini)

foreach(ConfigFiles ${ConfigFiles})
    add_custom_command(
            TARGET duck PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
            copy  ${ConfigFiles} $<TARGET_FILE_DIR:duck>
    )
endforeach()