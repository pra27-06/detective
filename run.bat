@echo off
setlocal

echo ========================================================================
echo    Launching CASE CLOSED -- Detective Mystery Simulator
echo ========================================================================
echo.

if not exist "case_closed.exe" (
    echo [INFO] case_closed.exe not found. Running build script first...
    call build.bat
    if not exist "case_closed.exe" (
        echo [ERROR] Build failed. Cannot launch game.
        pause
        exit /b 1
    )
)

case_closed.exe

echo.
echo ========================================================================
echo    Session Concluded. Press any key to close this terminal.
echo ========================================================================
pause >nul
