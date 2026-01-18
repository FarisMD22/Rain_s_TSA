cmake_minimum_required(VERSION 3.28)
project(Rain_s_TSA)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Source files
set(SOURCES
        src/main.cpp
        src/Utils.cpp
        src/UserManager.cpp
        src/QueueADT.cpp
        src/LinkedListADT.cpp
)

# Create executable
add_executable(Rain_s_TSA ${SOURCES})