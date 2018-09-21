@echo off

pushd build
devenv /debugexe webserver.exe
popd build
