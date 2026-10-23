@echo off
setlocal
cd /d "%~dp0"

echo ========================================================================
echo    CASE CLOSED: THE POISONED GLASS OF STERLING MANOR
echo    A Very Easy, Enjoyable, and Suspicious Detective Mystery in C++
echo ========================================================================
echo.

where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] MinGW g++ compiler not found in PATH!
    pause
    exit /b 1
)

echo [BUILD] Compiling Simple Detective Mystery (C++14 / OOPs)...
g++ -std=c++14 -Wall -Wextra -Iinclude src\SimpleDetectiveGame.cpp -o simple_detective.exe

if %ERRORLEVEL% equ 0 (
    echo [SUCCESS] Build succeeded! Launching game...
    echo.
    simple_detective.exe
) else (
    echo [ERROR] Compilation failed.
    pause
)
