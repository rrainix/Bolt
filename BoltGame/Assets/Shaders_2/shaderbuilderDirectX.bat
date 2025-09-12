@echo off
rem shadercRelease.exe liegt im gleichen Ordner wie diese Batch-Datei
set BGFX_PATH=.

rem Shader-Verzeichnisse (aktueller Ordner für Shader-Dateien)
set SHADER_SRC=.
set SHADER_OUT=compiled_shaders

rem Ausgabeverzeichnis erstellen falls nicht vorhanden
if not exist %SHADER_OUT% mkdir %SHADER_OUT%

echo Building Simple2D Shaders für DirectX11...

rem Vertex Shader für DirectX11 kompilieren
%BGFX_PATH%\shadercRelease.exe -f %SHADER_SRC%\vs_simple2d.sc -o %SHADER_OUT%\vs_simple2dDx.bin --type vertex --platform windows -p s_5_0 -i %SHADER_SRC% --varyingdef %SHADER_SRC%\varying.def.sc

if errorlevel 1 (
    echo Fehler beim Kompilieren des Vertex Shaders!
    pause
    exit /b 1
)

rem Fragment Shader für DirectX11 kompilieren
%BGFX_PATH%\shadercRelease.exe -f %SHADER_SRC%\fs_simple2d.sc -o %SHADER_OUT%\fs_simple2dDx.bin --type fragment --platform windows -p s_5_0 -i %SHADER_SRC% --varyingdef %SHADER_SRC%\varying.def.sc

if errorlevel 1 (
    echo Fehler beim Kompilieren des Fragment Shaders!
    pause
    exit /b 1
)

echo Shader erfolgreich für DirectX11 kompiliert!
echo - vs_simple2d.bin (DirectX11 s_5_0)
echo - fs_simple2d.bin (DirectX11 s_5_0)
pause