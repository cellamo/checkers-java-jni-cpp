cmake_minimum_required(VERSION 3.10)
project(CheckersGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set JAVA_HOME
set(JAVA_HOME $ENV{JAVA_HOME})
if(NOT JAVA_HOME)
    # Default macOS Java location
    execute_process(
        COMMAND /usr/libexec/java_home
        OUTPUT_VARIABLE JAVA_HOME
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

# Set JNI paths explicitly
set(JAVA_AWT_INCLUDE_PATH ${JAVA_HOME}/include)
set(JAVA_INCLUDE_PATH ${JAVA_HOME}/include)
set(JAVA_INCLUDE_PATH2 ${JAVA_HOME}/include/darwin)

# Find JNI package
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

# Include directories
include_directories(src/main/cpp/include)

# Source files
set(SOURCES
    src/main/cpp/src/Board.cpp
    src/main/cpp/src/Game.cpp
    src/main/cpp/src/jni_bridge.cpp
)

# Create shared library
add_library(CheckersGame SHARED ${SOURCES})

# Link against JNI
target_link_libraries(CheckersGame ${JNI_LIBRARIES})

# Set output directory
set_target_properties(CheckersGame PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Platform-specific settings
if(WIN32)
    set_target_properties(CheckersGame PROPERTIES PREFIX "")
endif()

# Add test executable
add_executable(CheckersTest src/test/cpp/GameTest.cpp ${SOURCES})
