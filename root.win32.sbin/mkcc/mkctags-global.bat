@echo off

call setup.bat --quiet

rem ------------------
rem Make ctags script.
rem ------------------

if "%1" == "--noremove-temp" (
set REMOVE_TEMP=0
) else (
set REMOVE_TEMP=1
)

set "TAGSHOME=%HOME%\.cache\ctags"
set CTAGSSRC=tags.files.bat
set TAGSNAME=tags
set TAGNUMBER=0
rem set FINDPATH="$C_INCLUDE_PATH;."

set "FLAGS=-a"
set "FLAGS=%FLAGS% --c++-kinds=+p --fields=+iaS --extra=+q"
set "FLAGS=%FLAGS% --exclude=boost"
set "FLAGS=%FLAGS% --tag-relative=no"
set "FLAGS=%FLAGS% -R"

rem cleanup.
if exist %CTAGSSRC% rm -f %CTAGSSRC%
if exist %TAGSNAME% rm -f %TAGSNAME%

if not exist %TAGSHOME% mkdir %TAGSHOME%

call include-path.bat --list ^
    | sed 's/^^\\(.*\\)/ctags %FLAGS% -f %%TAGSHOME%%\\/%%TAGNUMBER%%.%%TAGSNAME%% \\1 \\^&\\^&set \\/a TAGNUMBER=TAGNUMBER+1/g' ^
    > %CTAGSSRC%

if not exist %CTAGSSRC% (
    echo not found %CTAGSSRC% error.
    exit /b 1
) ELSE (
    echo create %TAGSNAME%
    call %CTAGSSRC%
)

rem Remove temp files.
if "%REMOVE_TEMP%" == "1" (
    if exist %CTAGSSRC% rm -f %CTAGSSRC%
)
