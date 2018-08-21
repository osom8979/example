@echo off

:: WARNING:
:: Use 'visual studio command prompt'

if not defined AM64 (
set AM64=1
)

if "%AM64%" == "0" (
set BJAM_ARCH_NAME=ntx86
set BOOST_ADDRESS_MODEL=32
) ELSE (
set BJAM_ARCH_NAME=ntx86_64
set BOOST_ADDRESS_MODEL=64
)

set WORKING_DIR=%CD%

if not exist "%PREFIX%" (
mkdir "%PREFIX%"
)

cd %WORKING_DIR%\boost_1_56_0

set BJAM_PATH=%WORKING_DIR%\boost_1_56_0\tools\build\src\engine\bin.%BJAM_ARCH_NAME%\bjam.exe

if not exist "%BJAM_PATH%" (
echo not found bjam.exe
exit /b 1
)

echo Build boost.
%BJAM_PATH% --prefix=%PREFIX% --layout=system toolset=gcc address-model=%BOOST_ADDRESS_MODEL% variant=release link=static,shared threading=multi install > build.boost.log

if "%ERRORLEVEL%" NEQ "0" (
echo SIGERR: %ERRORLEVEL%
) else (
echo Complete!
)

cd %WORKING_DIR%
