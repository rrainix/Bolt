@echo off
setlocal

rem === Parameter: Debug oder Release (Default Release) ===
set "CONFIG=Release"
if not "%~1"=="" set "CONFIG=%~1"

rem === Build‑Ordner relativ zum Script ===
set "BUILD_DIR=%~dp0build"
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

rem === In Build wechseln ===
pushd "%BUILD_DIR%"

rem === CMake Configure & Generate via .. (Source ist Parent) ===
cmake .. -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
  echo [ERROR] CMake-Konfiguration fehlgeschlagen!
  exit /b %errorlevel%
)

rem === Build starten ===
cmake --build . --config %CONFIG%
if errorlevel 1 (
  echo [ERROR] Build fehlgeschlagen!
  exit /b %errorlevel%
)

popd

echo [OK] Build %CONFIG% abgeschlossen.
pause
endlocal
