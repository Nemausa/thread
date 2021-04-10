cmake_minimum_required(VERSION 2.8)

# project name
STRING( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} )
PROJECT(${CURRENT_FOLDER})

# select system
if (CMAKE_HOST_WIN32)
    message("-- This is Windows")
    set(WINDOWS 1)
elseif (CMAKE_HOST_APPLE)
    message("-- This is Apple")
    set(MACOS 1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread")
elseif (CMAKE_HOST_UNIX)
    message("-- This is Linux")
    set(LINUX 1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread")
endif ()

# include directory
include_directories(vsomeip)

# source directory
FILE(GLOB SRC_LIST_CPP ${PROJECT_SOURCE_DIR}/vsomeip/*.cpp)

AUX_SOURCE_DIRECTORY(. SRC_LIST_CPP)
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST_CPP})
