@echo off

call setup.bat --quiet

rem ------------------
rem Make ctags script.
rem ------------------

set TAGSNAME=tags

rem set "FLAGS=-a"
set "FLAGS="
set "FLAGS=%FLAGS% --c++-kinds=+p --fields=+iaS --extra=+q"
set "FLAGS=%FLAGS% --exclude=boost"
set "FLAGS=%FLAGS% --tag-relative=no"
set "FLAGS=%FLAGS% -f %TAGSNAME%"
set "FLAGS=%FLAGS% -R"

rem cleanup.
if exist %TAGSNAME% del %TAGSNAME%

echo create %TAGSNAME%
ctags %FLAGS% .
