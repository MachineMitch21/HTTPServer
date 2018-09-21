@echo off

setlocal EnableDelayedExpansion
set files=

del /q build

pushd src
for /R "./" %%f in (*.c *.cpp *.cc *.cxx) DO (
    set files=!files! %%f
)
popd src

pushd build
cl /Zi /Fewebserver %files% Ws2_32.lib 
popd build
endlocal