@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

echo ========================================================================
echo    CASE CLOSED -- Detective Mystery Simulator :: Master Build
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

if not exist "saves" (
    mkdir saves
)

set TARGET=%1
if "%TARGET%"=="" set TARGET=all

if /i "%TARGET%"=="console" goto build_console
if /i "%TARGET%"=="gui" goto build_gui
if /i "%TARGET%"=="all" goto build_all

echo [ERROR] Unknown build target: %TARGET%
echo Usage: build.bat [all | console | gui]
pause
exit /b 1

:build_all
echo [BUILD 1/2] Compiling Classic Console Edition (case_closed_console.exe)...
set CONSOLE_SRCS=src\Character.cpp src\Detective.cpp src\Suspect.cpp src\Witness.cpp src\Evidence.cpp src\CrimeScene.cpp src\ClueBoard.cpp src\Notebook.cpp src\CaseFile.cpp src\SaveManager.cpp src\TerminalUI.cpp src\GameEngine.cpp src\main.cpp
g++ -std=c++17 -Wall -Wextra %CONSOLE_SRCS% -Iinclude -o case_closed_console.exe
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Console build failed!
    pause
    exit /b 1
)
copy /y case_closed_console.exe case_closed.exe >nul
echo [OK] Console Edition built successfully.
echo.

:build_gui
echo [BUILD 2/2] Compiling 2D Visual Novel Edition (case_closed_gui.exe)...
set GUI_SRCS=src\Character.cpp src\Detective.cpp src\Suspect.cpp src\Witness.cpp src\Evidence.cpp src\CrimeScene.cpp src\ClueBoard.cpp src\Notebook.cpp src\CaseFile.cpp src\SaveManager.cpp src\TerminalUI.cpp src\VNAssetManager.cpp src\VNRenderer.cpp src\DialogueSystem.cpp src\VNEngine.cpp src\main_gui.cpp
g++ -std=c++17 -Wall -Wextra %GUI_SRCS% -Iinclude -lgdiplus -lgdi32 -luser32 -lwinmm -mwindows -o case_closed_gui.exe
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Graphical build failed!
    pause
    exit /b 1
)
echo [OK] Graphical Edition built successfully.
echo.

if /i "%TARGET%"=="gui" goto finished

:finished
echo ========================================================================
echo    ALL EDITIONS SUCCESSFULLY COMPILED!
echo ========================================================================
echo  - Console Edition:   case_closed_console.exe (Run via run_console.bat)
echo  - Visual Novel GUI:  case_closed_gui.exe     (Run via run_graphical.bat)
echo  - Unified Launcher:  run.bat
echo ========================================================================
echo.
pause
exit /b 0

:build_console
echo [BUILD] Compiling Classic Console Edition (case_closed_console.exe)...
set CONSOLE_SRCS=src\Character.cpp src\Detective.cpp src\Suspect.cpp src\Witness.cpp src\Evidence.cpp src\CrimeScene.cpp src\ClueBoard.cpp src\Notebook.cpp src\CaseFile.cpp src\SaveManager.cpp src\TerminalUI.cpp src\GameEngine.cpp src\main.cpp
g++ -std=c++17 -Wall -Wextra %CONSOLE_SRCS% -Iinclude -o case_closed_console.exe
if %ERRORLEVEL% equ 0 (
    copy /y case_closed_console.exe case_closed.exe >nul
    echo [OK] Console Edition built successfully.
) else (
    echo [ERROR] Console build failed!
)
pause
exit /b %ERRORLEVEL%
