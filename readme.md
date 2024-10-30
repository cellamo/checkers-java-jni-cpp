# Checkers Game

A cross-platform Checkers (Draughts) game implementation using Java Swing for the UI and C++ for the core game logic, connected via JNI.

## Features

- Classic 8x8 checkers board
- Full game rules implementation including:
  - Regular moves
  - Captures
  - King pieces
  - Mandatory captures
- Keyboard and mouse controls
- Visual move highlighting
- Turn indicators
- Win condition detection

## Technical Stack

- Java 17
- C++11
- JNI (Java Native Interface)
- Swing GUI
- Maven for Java build
- CMake for C++ build

## Prerequisites

- JDK 17 or higher
- C++ compiler
- CMake 3.10 or higher
- Maven

## Building the Project

1. Build the C++ library:
```bash
mkdir build
cd build
cmake ..
make
```

2. Build the Java project:

```bash
mvn clean package
```

## Running the Game

```bash
mvn exec:java
```

## Project Structure

- `src/main/java` - Java source files
  - UI components
  - JNI interface
  - Main application
- `src/main/cpp` - C++ source files
  - Core game logic
  - Board implementation
  - JNI bridge
- `src/test` - Test files for both Java and C++

## Testing

Run Java tests:

```bash
mvn test
```

Run C++ tests:

```bash
./build/CheckersTest
```

## Controls

- Mouse: Click to select and move pieces
- Keyboard:
  - Arrow keys: Move cursor
  - Space/Enter: Select/move pieces

## License

MIT License