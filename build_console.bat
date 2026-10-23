@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

echo ========================================================================
echo    CASE CLOSED -- Classic Console Edition :: Build Script
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

echo [BUILD] Compiling Case Closed Console Edition (C++17)...
set CONSOLE_SRCS=src\Character.cpp src\Detective.cpp src\Suspect.cpp src\Witness.cpp src\Evidence.cpp src\CrimeScene.cpp src\ClueBoard.cpp src\Notebook.cpp src\CaseFile.cpp src\SaveManager.cpp src\TerminalUI.cpp src\ForensicLab.cpp src\MindPalace.cpp src\PsychologicalProfile.cpp src\GameEngine.cpp src\main.cpp

g++ -std=c++17 -Wall -Wextra %CONSOLE_SRCS% -Iinclude -o case_closed_console.exe

if %ERRORLEVEL% equ 0 (
    copy /y case_closed_console.exe case_closed.exe >nul
    echo.
    echo [SUCCESS] Console Edition compiled successfully!
    echo [OUTPUT]  case_closed_console.exe and case_closed.exe generated.
    echo.
    echo To play, run: run_console.bat
) else (
    echo.
    echo [ERROR] Console build failed! Check compiler messages above.
)

echo.
pause
