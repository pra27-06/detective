@echo off
setlocal
cd /d "%~dp0"

echo ========================================================================
echo    Launching CASE CLOSED -- Classic Console Edition
echo ========================================================================
echo.

if not exist "case_closed_console.exe" (
    echo [INFO] case_closed_console.exe not found. Running build_console.bat first...
    call build_console.bat
    if not exist "case_closed_console.exe" (
        echo [ERROR] Build failed. Cannot launch game.
        pause
        exit /b 1
    )
)

case_closed_console.exe

echo.
echo ========================================================================
echo    Session Concluded. Press any key to close this terminal.
echo ========================================================================
pause >nul
