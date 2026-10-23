@echo off
setlocal
cd /d "%~dp0"

if /i "%1"=="gui" goto launch_gui
if /i "%1"=="console" goto launch_console

:menu
cls
echo ========================================================================
echo         CASE CLOSED -- Detective Mystery Simulator Launcher
echo ========================================================================
echo.
echo   Please select the edition you wish to play:
echo.
echo     [1] 2D Anime Visual Novel Edition (Graphical Window, Portraits, SFX)
echo     [2] Classic Detective Terminal Edition (Console Text, ANSI Colors)
echo     [3] Build / Recompile All Editions
echo     [4] Exit
echo.
echo ========================================================================
set /p CHOICE="Enter your choice (1-4): "

if "%CHOICE%"=="1" goto launch_gui
if "%CHOICE%"=="2" goto launch_console
if "%CHOICE%"=="3" goto do_build
if "%CHOICE%"=="4" exit /b 0

echo Invalid choice. Please enter 1, 2, 3, or 4.
timeout /t 2 >nul
goto menu

:launch_gui
if not exist "case_closed_gui.exe" (
    echo [INFO] case_closed_gui.exe not found. Building now...
    call build_gui.bat
    if not exist "case_closed_gui.exe" (
        echo [ERROR] Build failed.
        pause
        exit /b 1
    )
)
echo [LAUNCH] Starting 2D Visual Novel Edition...
start "" "case_closed_gui.exe"
exit /b 0

:launch_console
if not exist "case_closed_console.exe" (
    echo [INFO] case_closed_console.exe not found. Building now...
    call build_console.bat
    if not exist "case_closed_console.exe" (
        echo [ERROR] Build failed.
        pause
        exit /b 1
    )
)
echo [LAUNCH] Starting Classic Terminal Edition...
case_closed_console.exe
echo.
echo Press any key to return to launcher menu...
pause >nul
goto menu

:do_build
call build.bat all
goto menu
