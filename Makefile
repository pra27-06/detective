# Makefile for CASE CLOSED — Detective Mystery Simulator
# Standard C++17 project

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -O2
LDFLAGS = 

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = case_closed.exe

.PHONY: all clean run dirs

all: dirs $(TARGET)

dirs:
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	@if not exist saves mkdir saves

$(TARGET): $(OBJS)
	@echo [LINK] Linking $@...
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo [DONE] Build complete!

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CXX] Compiling $<...
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	@echo [CLEAN] Removing build artifacts...
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(TARGET) del /f /q $(TARGET)
	@echo [CLEAN] Clean complete.
