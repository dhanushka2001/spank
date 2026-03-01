cmake_minimum_required(VERSION 3.16)
project(slap-daemon VERSION 1.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional: show all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

# Add executable
add_executable(slap
    src/main.cpp
    src/miniaudio.cpp
)

# Threads are required for miniaudio
find_package(Threads REQUIRED)
target_link_libraries(slap PRIVATE Threads::Threads)

# Copy audio files to the build directory
# This assumes your wavs are in a folder named 'audio' in your project root
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/audio" 
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
