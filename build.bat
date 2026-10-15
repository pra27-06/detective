@echo off
setlocal enabledelayedexpansion

echo ========================================================================
echo    CASE CLOSED -- Detective Mystery Simulator :: Build Script
echo ========================================================================
echo.

where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] MinGW g++ compiler was not found in your system PATH!
    echo Please install MinGW-w64 or add g++.exe to your PATH environment variable.
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

echo [BUILD] Compiling Case Closed (C++17, -Wall, -Wextra)...
g++ -std=c++17 -Wall -Wextra src\*.cpp -Iinclude -o case_closed.exe

if %ERRORLEVEL% equ 0 (
    echo.
    echo [SUCCESS] Compilation finished successfully!
    echo [OUTPUT]  case_closed.exe has been generated.
    echo.
    echo To play, run: case_closed.exe or run.bat
) else (
    echo.
    echo [ERROR] Build failed! Check the compiler error messages above.
)

echo.
pause
