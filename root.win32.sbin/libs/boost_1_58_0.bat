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

unzip boost_1_58_0.zip
cd %WORKING_DIR%\boost_1_58_0

echo Run bootstrap
bootstrap.bat mingw

echo Build boost.
b2 --prefix=%PREFIX% --layout=system toolset=gcc address-model=%BOOST_ADDRESS_MODEL% variant=release link=shared threading=multi install > build.boost.log

if "%ERRORLEVEL%" NEQ "0" (
echo SIGERR: %ERRORLEVEL%
) else (
echo Complete!
)

cd %WORKING_DIR%
