@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

echo ========================================================================
echo    CASE CLOSED -- 2D Visual Novel Edition :: Build Script
echo ========================================================================
echo.

where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] MinGW g++ compiler was not found in your system PATH!
    echo Please install MinGW or add g++.exe to your PATH environment variable.
    echo.
    pause
    exit /b 1
)

echo [INFO] Found g++ compiler:
g++ --version | findstr /i "g++"
echo.

if not exist "saves" (
    echo [INFO] Creating saves directory...
    mkdir saves
)

echo [BUILD] Compiling Case Closed 2D Visual Novel Edition (C++17, GDI+)...
set GUI_SRCS=src\Character.cpp src\Detective.cpp src\Suspect.cpp src\Witness.cpp src\Evidence.cpp src\CrimeScene.cpp src\ClueBoard.cpp src\Notebook.cpp src\CaseFile.cpp src\SaveManager.cpp src\TerminalUI.cpp src\ForensicLab.cpp src\MindPalace.cpp src\PsychologicalProfile.cpp src\VNAssetManager.cpp src\VNRenderer.cpp src\DialogueSystem.cpp src\VNEngine.cpp src\main_gui.cpp

g++ -std=c++17 -Wall -Wextra %GUI_SRCS% -Iinclude -lgdiplus -lgdi32 -luser32 -lwinmm -mwindows -o case_closed_gui.exe

if %ERRORLEVEL% equ 0 (
    echo.
    echo [SUCCESS] Graphical Edition compiled successfully!
    echo [OUTPUT]  case_closed_gui.exe generated.
    echo.
    echo To play, run: run_graphical.bat
) else (
    echo.
    echo [ERROR] Graphical build failed! Check compiler messages above.
)

echo.
pause
