@echo off

:: WARNING:
:: Use 'visual studio command prompt'

set WORKING_DIR=%CD%

::tar xzf %WORKING_DIR%\.packages\boost_1_56_0.tar.gz
::tar vxzf boost_1_56_0.tar.gz
cd boost_1_56_0\tools\build\src\engine

echo Build bjam.
call build.bat gcc

cd %WORKING_DIR%
