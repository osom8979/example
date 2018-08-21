@echo off

call setup.bat --quiet

rem -------------------
rem Make cscope script.
rem -------------------

if "%1" == "--noremove-temp" (
set REMOVE_TEMP=0
) else (
set REMOVE_TEMP=1
)

rem c source files.
set "FFLAGS=-iname *.c"
set "FFLAGS=%FFLAGS% -o -iname *.cc"
rem c++ source files.
set "FFLAGS=%FFLAGS% -o -iname *.cp"
set "FFLAGS=%FFLAGS% -o -iname *.cxx"
set "FFLAGS=%FFLAGS% -o -iname *.cpp"
set "FFLAGS=%FFLAGS% -o -iname *.c++"
rem c/c++ header files.
set "FFLAGS=%FFLAGS% -o -iname *.h"
set "FFLAGS=%FFLAGS% -o -iname *.hpp"
set "FFLAGS=%FFLAGS% -o -iname *.hxx"
rem inline files.
set "FFLAGS=%FFLAGS% -o -iname *.inl"
rem assembler code files.
set "FFLAGS=%FFLAGS% -o -iname *.s"

set FIND_SCRIPT=cscope.finder.bat
set LIST_FILE=cscope.files
set "CSCOPE_HOME=%HOME%\.cache\cscope"
set "CSCOPE_DB=%CSCOPE_HOME%\cscope.out"

if not exist %CSCOPE_HOME% mkdir %CSCOPE_HOME%

rem cleanup.
if exist %FIND_SCRIPT% del %FIND_SCRIPT%
if exist %LIST_FILE% del %LIST_FILE%
if exist %CSCOPE_DB% del %CSCOPE_DB%

rem create find script.
call include-path.bat --list ^
    | sed 's/^\\(.*\\)/find \\1 %FFLAGS% ^>^> %LIST_FILE%/g' ^
    > %FIND_SCRIPT%

if not exist %FIND_SCRIPT% (
    echo Not found %FIND_SCRIPT% error.
    exit /b 1
) else (
    call %FIND_SCRIPT%
)

if not exist %LIST_FILE% (
    echo Not found %LIST_FILE% error.
    exit /b 1
) else (
    echo create %CSCOPE_DB%
    cscope -b -i %LIST_FILE% -f %CSCOPE_DB%
)

rem Remove temp files.
if "%REMOVE_TEMP%" == "1" (
    if exist %FIND_SCRIPT% del %FIND_SCRIPT%
    if exist %LIST_FILE% del %LIST_FILE%
)
