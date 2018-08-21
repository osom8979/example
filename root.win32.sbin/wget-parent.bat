@echo off

call setup.bat --quiet

set "WGET=wget.exe"
set "FLAGS=-c -m -U -nc -np -r -k -l 0"

%WGET% %FLAGS% %1
