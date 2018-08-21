@echo off
REM Run this batch file from any directory to build gvim.exe and vim.exe.
REM But first edit the paths and Python version number.
REM http://vim.wikia.com/wiki/Build_Python-enabled_Vim_on_Windows_with_MinGW

if "%1" == "" (
echo Parameter 01 error.
echo Specify Vim /src folder.
exit /b 1
)
if "%2" == "" (
REM echo Parameter 02 error.
REM echo Specify Python27 folder.
SET "PYTHON27_DIR=%PYTHONHOME%"
) ELSE (
SET "PYTHON27_DIR=%2"
)

REM --- Specify Vim /src folder ---
set "VIMSRC=%1"
REM --- Add MinGW /bin directory to PATH ---
REM PATH = \MinGW\bin\path;%PATH%
REM --- Also make sure that PYTHON, PYTHON_VER below are correct. ---

REM get location of this batch file
set WORKDIR=%~dp0
set LOGFILE=%WORKDIR%log.txt

echo Work directory: %WORKDIR%
echo Vim source directory: %VIMSRC%

REM change to Vim /src folder
cd /d %VIMSRC%

REM --- Build GUI version (gvim.exe) ---
echo Building gvim.exe ...
REM The following command will compile with both Python 2.7 and Python 3.3
REM mingw32-make.exe -f Make_ming.mak PYTHON="C:/Python27" PYTHON_VER=27 DYNAMIC_PYTHON=yes PYTHON3="C:/Python33" PYTHON3_VER=33 DYNAMIC_PYTHON3=yes FEATURES=HUGE GUI=yes gvim.exe > "%LOGFILE%"
mingw32-make.exe -f Make_ming.mak PYTHON="%PYTHON27_DIR%" PYTHON_VER=27 DYNAMIC_PYTHON=yes FEATURES=HUGE GUI=yes gvim.exe > "%LOGFILE%"

REM --- Build console version (vim.exe) ---
echo Building vim.exe ...
REM The following command will compile with both Python 2.7 and Python 3.3
REM mingw32-make.exe -f Make_ming.mak PYTHON="C:/Python27" PYTHON_VER=27 DYNAMIC_PYTHON=yes PYTHON3="C:/Python33" PYTHON3_VER=33 DYNAMIC_PYTHON3=yes FEATURES=HUGE GUI=no vim.exe >> "%LOGFILE%"
mingw32-make.exe -f Make_ming.mak PYTHON="%PYTHON27_DIR%" PYTHON_VER=27 DYNAMIC_PYTHON=yes FEATURES=HUGE GUI=no vim.exe >> "%LOGFILE%"

echo Moving files ...
move gvim.exe "%WORKDIR%"
move vim.exe "%WORKDIR%"

echo Cleaning Vim source directory ...
REM NOTE: "mingw32-make.exe -f Make_ming.mak clean" does not finish the job
IF NOT %CD%==%VIMSRC% GOTO THEEND
IF NOT EXIST vim.h GOTO THEEND
IF EXIST pathdef.c DEL pathdef.c
IF EXIST obj\NUL      RMDIR /S /Q obj
IF EXIST obji386\NUL  RMDIR /S /Q obji386
IF EXIST gobj\NUL     RMDIR /S /Q gobj
IF EXIST gobji386\NUL RMDIR /S /Q gobji386
IF EXIST gvim.exe DEL gvim.exe
IF EXIST vim.exe  DEL vim.exe
:THEEND

cd /d %WORKDIR%
