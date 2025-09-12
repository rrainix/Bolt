@echo off
rem shadercRelease.exe liegt im gleichen Ordner wie diese Batch-Datei
set BGFX_PATH=.

rem Shader-Verzeichnisse (aktueller Ordner für Shader-Dateien)
set SHADER_SRC=.
set SHADER_OUT=compiled_shaders

rem Ausgabeverzeichnis erstellen falls nicht vorhanden
if not exist %SHADER_OUT% mkdir %SHADER_OUT%

echo Building Simple2D Shaders für Vulkan/SPIRV...

rem Vertex Shader für Vulkan/SPIRV kompilieren
%BGFX_PATH%\shadercRelease.exe -f %SHADER_SRC%\vs_simple2d.sc -o %SHADER_OUT%\vs_simple2d.bin --type vertex --platform linux -p spirv -i %SHADER_SRC% --varyingdef %SHADER_SRC%\varying.def.sc
if errorlevel 1 (
    echo Fehler beim Kompilieren des Vertex Shaders!
    pause
    exit /b 1
)

rem Fragment Shader für Vulkan/SPIRV kompilieren
%BGFX_PATH%\shadercRelease.exe -f %SHADER_SRC%\fs_simple2d.sc -o %SHADER_OUT%\fs_simple2d.bin --type fragment --platform linux -p spirv -i %SHADER_SRC% --varyingdef %SHADER_SRC%\varying.def.sc
if errorlevel 1 (
    echo Fehler beim Kompilieren des Fragment Shaders!
    pause
    exit /b 1
)

echo Shader erfolgreich für Vulkan/SPIRV kompiliert!
echo - vs_simple2d.bin (SPIRV)
echo - fs_simple2d.bin (SPIRV)

pause