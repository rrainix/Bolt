@echo off
echo Building instanced rendering shaders for DirectX11...

REM Set paths for local shaderc and local bgfx includes
set SHADERC=shadercRelease.exe
set INCLUDE_PATH=.

REM Check if shaderc exists
if not exist "%SHADERC%" (
    echo Error: shadercRelease.exe not found in current directory
    pause
    exit /b 1
)

REM Check if bgfx_shader.sh exists locally
if exist "bgfx_shader.sh" (
    echo Found bgfx_shader.sh in current directory
) else (
    echo bgfx_shader.sh not found in current directory
)

REM Create output directory if it doesn't exist
if not exist "shaders\dx11" mkdir shaders\dx11

REM Build vertex shader for DirectX11
echo Building vertex shader...
shadercRelease.exe ^
    -f vs_instanced.sc ^
    -o shaders\dx11\vs_instanced.bin ^
    --platform windows ^
    --type vertex ^
    --profile s_5_0 ^
    -i "." ^
    --varyingdef varying.def.sc

if %ERRORLEVEL% neq 0 (
    echo Error building vertex shader!
    pause
    exit /b 1
)

REM Build fragment shader for DirectX11
echo Building fragment shader...
shadercRelease.exe ^
    -f fs_instanced.sc ^
    -o shaders\dx11\fs_instanced.bin ^
    --platform windows ^
    --type fragment ^
    --profile s_5_0 ^
    -i "." ^
    --varyingdef varying.def.sc

if %ERRORLEVEL% neq 0 (
    echo Error building fragment shader!
    pause
    exit /b 1
)

echo.
echo Success! Shaders built to:
echo - shaders\dx11\vs_instanced.bin
echo - shaders\dx11\fs_instanced.bin
echo.
pause
