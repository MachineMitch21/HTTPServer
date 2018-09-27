@echo off

setlocal EnableDelayedExpansion
set files=

mkdir build

pushd build 
for /R "./" %%f in (*.obj) DO (
    del %%f
)
popd build

pushd src
for /R "./" %%f in (*.c *.cpp *.cc *.cxx) DO (
    set files=!files! %%f
)
popd src

pushd build
cl /Zi /Fewebserver %files% Ws2_32.lib 
popd build
endlocal