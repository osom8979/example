@echo off

rem echo param1: %1
rem echo param2: %2

if "%1" == "" goto PARAMETER_ERROR_EXIT
if "%2" == "" goto PARAMETER_ERROR_EXIT

set PROCESS_HOME_PARAM=%1
set PROCESS_PATH_PARAM=%2

if not exist %PROCESS_PATH_PARAM% goto PARAMETER_ERROR_EXIT

:MAIN_BLOCK
start /d "%PROCESS_HOME_PARAM%" /b %PROCESS_PATH_PARAM% %3 %4 %5 %6 %7 %8 %9
exit /b 0

:PARAMETER_ERROR_EXIT
echo parameter error.
exit /b 1
