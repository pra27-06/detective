# Makefile for CASE CLOSED — Detective Mystery Simulator
# Standard C++17 project with Dual Editions (Console & 2D Visual Novel GUI)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -O2

COMMON_SRCS = src/Character.cpp src/Detective.cpp src/Suspect.cpp src/Witness.cpp \
              src/Evidence.cpp src/CrimeScene.cpp src/ClueBoard.cpp src/Notebook.cpp \
              src/CaseFile.cpp src/SaveManager.cpp src/TerminalUI.cpp \
              src/ForensicLab.cpp src/MindPalace.cpp src/PsychologicalProfile.cpp

CONSOLE_SRCS = $(COMMON_SRCS) src/GameEngine.cpp src/main.cpp
GUI_SRCS     = $(COMMON_SRCS) src/VNAssetManager.cpp src/VNRenderer.cpp \
               src/DialogueSystem.cpp src/VNEngine.cpp src/main_gui.cpp

CONSOLE_TARGET = case_closed_console.exe
GUI_TARGET     = case_closed_gui.exe
LEGACY_TARGET  = case_closed.exe

GUI_LDFLAGS = -lgdiplus -lgdi32 -luser32 -lwinmm -mwindows

.PHONY: all console gui clean dirs

all: dirs console gui

dirs:
	@if not exist saves mkdir saves

console: dirs $(CONSOLE_TARGET)

$(CONSOLE_TARGET): $(CONSOLE_SRCS)
	@echo [CXX] Building Console Edition: $@...
	$(CXX) $(CXXFLAGS) -o $@ $(CONSOLE_SRCS)
	@copy /y $(CONSOLE_TARGET) $(LEGACY_TARGET) >nul
	@echo [DONE] Console Edition ready.

gui: dirs $(GUI_TARGET)

$(GUI_TARGET): $(GUI_SRCS)
	@echo [CXX] Building 2D Visual Novel Edition: $@...
	$(CXX) $(CXXFLAGS) -o $@ $(GUI_SRCS) $(GUI_LDFLAGS)
	@echo [DONE] Visual Novel Edition ready.

clean:
	@echo [CLEAN] Removing executables...
	@if exist $(CONSOLE_TARGET) del /f /q $(CONSOLE_TARGET)
	@if exist $(GUI_TARGET) del /f /q $(GUI_TARGET)
	@if exist $(LEGACY_TARGET) del /f /q $(LEGACY_TARGET)
	@echo [CLEAN] Done.
