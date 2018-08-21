@echo off

if "%BLACKHOLE_SETUP_COMPLETE%" == "1" exit /b 0
set "BLACKHOLE_SETUP_COMPLETE=1"

rem check the parameters...
set "AM64=0"
set "QUIET=0"

if not defined BHROOT (
set "BHROOT=%CD%\.."
)

:PARAM_LOOP_BEGIN
set "CURSOR=%1"
if "%CURSOR%" neq "" (
    if "%CURSOR:~0,1%" == "-" (
        rem check command-line options.
        if "%CURSOR%" == "--32bit"  set "AM64=0"
        if "%CURSOR%" == "--64bit"  set "AM64=1"
        if "%CURSOR%" == "--quiet"  set "QUIET=1"
        if "%CURSOR%" == "--prefix" set "BHROOT=%2"&&shift
    )
    shift
    goto PARAM_LOOP_BEGIN
)
:PARAM_LOOP_END

rem blackhole root parameter: normalize path.
if "%BHROOT:~-1%" == "\" (
set "BHROOT=%BHROOT:~0,-1%"
)


rem setup.
set "USER=zer0"
set "ROOT=%BHROOT%"
set "BIN=%ROOT%\bin"
set "ETC=%ROOT%\etc"
set "HOME=%ROOT%\home\%USER%"
set "OPT=%ROOT%\opt"
set "SBIN=%ROOT%\sbin"
set "VAR=%ROOT%\var"
set "TMP=%ROOT%\tmp"

if "%AM64%" == "0" (
set "LOCAL=%ROOT%\local"
) else (
set "LOCAL=%ROOT%\local64"
)
set "LBIN=%LOCAL%\bin"
set "LINC=%LOCAL%\include"
set "LLIB=%LOCAL%\lib"
set "PREFIX=%LOCAL%"

if not exist "%TMP%"   mkdir "%TMP%"
if not exist "%LOCAL%" mkdir "%LOCAL%"
if not exist "%LBIN%"  mkdir "%LBIN%"
if not exist "%LINC%"  mkdir "%LINC%"
if not exist "%LLIB%"  mkdir "%LLIB%"

rem setup (for windows).
SET "APPDATA=%VAR%"
rem SET "USERPROFILE=%HOME%"

rem set "SHELL=%BIN%\sh.exe"
rem set "EDITOR=vim"

rem default applications.
set "PYTHONHOME=%ROOT%\python27"
set "PYTHONSCRIPT=%PYTHONHOME%\Scripts"
set "PYTHONPATH=%PYTHONHOME%;%PYTHONSCRIPT%"
set "GITHOME=%ROOT%\git"
set "GITPATH=%GITHOME%\cmd"
set "CURLPATH=%SBIN%\curl"
set "MKCCPATH=%SBIN%\mkcc"

rem OPM HELPER.
set "OPMRUNTIME=%SBIN%\opm"
set "OPMHOME=%VAR%\opm"
set "OPMBIN=%OPMHOME%\bin"
set "OPMALTER=%OPMHOME%\alter"
set "OPMAPPS=%OPMHOME%\apps"
set "OPMCACHE=%OPMHOME%\cache"
set "OPMSETUP=%OPMHOME%\setup"
set "OPMMSVC=%OPMHOME%\msvc"
set "OPMSETUP32=%OPMHOME%\setup-x86"
set "OPMSETUP64=%OPMHOME%\setup-x64"

rem OPM Local.
set "OPMLOCAL32=%OPMHOME%\local"
set "OPMLOCAL64=%OPMHOME%\local64"
if "%AM64%" == "0" (
set "OPMLOCAL=%OPMLOCAL32%"
) else (
set "OPMLOCAL=%OPMLOCAL64%"
)
set "OPMLBIN=%OPMLOCAL%\bin"
set "OPMLINC=%OPMLOCAL%\include"
set "OPMLLIB=%OPMLOCAL%\lib"

rem OPM AUTO SETUP.
for /r "%OPMSETUP%" %%i in (*.bat) do call %%i
if "%AM64%" == "0" (
for /r "%OPMSETUP32%" %%i in (*.bat) do call %%i
) else (
for /r "%OPMSETUP64%" %%i in (*.bat) do call %%i
)

rem GCC Environment Variable.
set "CPATH=%OPMLINC%;%LINC%;%CPATH%"
set "LIBRARY_PATH=%OPMLLIB%;%LLIB%;%LIBRARY_PATH%"

rem environment valuables.
set "PATH=%PYTHONPATH%;%GITPATH%;%CURLPATH%;%MKCCPATH%;%PATH%"
set "PATH=%OPMBIN%;%OPMALTER%;%OPMLBIN%;%PATH%"
set "PATH=%SBIN%;%LBIN%;%BIN%;%PATH%"

:ECHO_BEGIN
if "%QUIET%" == "0" (
echo Setup blackhole project environment.
echo ROOT=%ROOT%
echo HOME=%HOME%
rem echo PREFIX=%PREFIX%
echo PARAM[AM64=%AM64%]
)
:ECHO_END

exit /b 0
