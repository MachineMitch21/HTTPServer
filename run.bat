@echo off

setlocal enabledelayedexpansion

set args=

set argCount=0

for %%x in (%*) do (
    set /A argCount+=1
    set args=!args! %%x
)

pushd build
webserver %args%
popd build

endlocal