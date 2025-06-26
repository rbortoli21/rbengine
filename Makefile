CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
INCLUDES = -I/usr/include/SDL2 -I/usr/include/SDL2_image -I/usr/include/SDL2_ttf
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf

# Source files
SOURCES = src/main.cpp \
          src/core/Player.cpp \
          src/core/Game.cpp \
          src/core/Camera.cpp \
          src/core/InputManager.cpp \
          src/core/TextureManager.cpp \
          src/core/Arrow.cpp \
          src/core/Menu.cpp \
          src/core/Inventory.cpp \
          src/core/CharacterProgression.cpp \
          src/core/UIFeedback.cpp \
          src/core/Enemy.cpp \
          src/core/WoodenSentinel.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = rbengine

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install dependencies (for Manjaro/Arch)
install-deps:
	sudo pacman -S sdl2 sdl2_image sdl2_ttf gcc cmake --noconfirm

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean install-deps run 