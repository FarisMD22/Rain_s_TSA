cmake_minimum_required(VERSION 3.28)
project(Rain_s_TSA)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
set(SOURCES
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/Utils.cpp
        ${CMAKE_SOURCE_DIR}/src/UserManager.cpp
        ${CMAKE_SOURCE_DIR}/src/QueueADT.cpp
        ${CMAKE_SOURCE_DIR}/src/LinkedListADT.cpp
)

# Create executable
add_executable(Rain_s_TSA ${SOURCES})