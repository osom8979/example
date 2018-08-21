@echo off

call setup.bat --quiet

if not exist "%1" exit /b 2

rem 7z.
set ARCHIVE_TYPE=7z

rem time.
set BACKUP_DATE=%date:~0,4%%date:~5,2%%date:~8,2%
set /a BACKUP_TIME_HOUR=%time:~0,2%
if %BACKUP_TIME_HOUR%==0 set BACKUP_TIME_HOUR=00
set BACKUP_TIME=%BACKUP_TIME_HOUR%%time:~3,2%%time:~6,2%%time:~9,2%
set BACKUP_NOW=%BACKUP_DATE%-%BACKUP_TIME%

if exist "%2" (
set "OUTPUT_PATH=%2"
) else (
set "OUTPUT_PATH=backup.%BACKUP_NOW%.%ARCHIVE_TYPE%"
)

7za a -t%ARCHIVE_TYPE% "%OUTPUT_PATH%" "%1\"
