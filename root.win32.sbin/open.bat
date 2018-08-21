@echo off

set EXPLORER=explorer.exe

if "%1" == "" (
%EXPLORER% %CD%
) else (
%EXPLORER% %1
)
