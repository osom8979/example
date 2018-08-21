@echo off

if "%1" == "" (
echo Please enter the shutdown minute: 
set /p SHUTDOWN_MINUTE=
) else (
set SHUTDOWN_MINUTE=%1
)
set /a SHUTDOWN_SECOND=%SHUTDOWN_MINUTE%*60

echo Reservation shutdown: %SHUTDOWN_MINUTE%m (%SHUTDOWN_SECOND%s)
rem echo Shutdown cancel command:
rem echo  $ shutdown /a

shutdown /s /t %SHUTDOWN_SECOND%
