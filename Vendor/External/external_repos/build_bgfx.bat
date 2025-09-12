@echo off
setlocal

set GENIE=..\bx\tools\bin\windows\genie.exe

cd bx
git submodule update --init --recursive
cd ..

cd bimg
git submodule update --init --recursive
cd ..

cd bgfx
git submodule update --init --recursive
%GENIE% --with-tools vs2022
cd ..

endlocal