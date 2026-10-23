@echo off
setlocal
cd /d "%~dp0"

echo ========================================================================
echo    Launching CASE CLOSED -- 2D Visual Novel Edition
echo ========================================================================
echo.

if not exist "case_closed_gui.exe" (
    echo [INFO] case_closed_gui.exe not found. Running build_gui.bat first...
    call build_gui.bat
    if not exist "case_closed_gui.exe" (
        echo [ERROR] Build failed. Cannot launch graphical game.
        pause
        exit /b 1
    )
)

echo [INFO] Launching Case Closed Anime Visual Novel GUI...
start "" "case_closed_gui.exe"
exit /b 0
