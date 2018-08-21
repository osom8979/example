@echo off

call setup.bat --quiet
 
if "%1" == "" (
echo Parameter error.
exit /B 1
)
 
set SITE=%1
set "WORKING=%CD%"
set "TMPDIR=%WORKING%\tmp"
set "IMGDIR=%WORKING%\img"
set "MVSOURCE=%TMPDIR%\mv-list.bat"
set TMPNUM=1

set "WGET_FLAGS=-E -H -k -K -p"

if not exist "%TMPDIR%" mkdir "%TMPDIR%"
if not exist "%IMGDIR%" mkdir "%IMGDIR%"
 
cd "%TMPDIR%"
wget.exe %WGET_FLAGS% %SITE%
find.exe . -iname *.jpg -o -iname *.jpeg -o -iname *.png ^
    | sed -e 's/^^\\(.*\\)\\(\\.jpg\\^|\\.jpeg\\^|\\.png\\)^$/mv \\1\\2 %%IMGDIR%%\\/%%TMPNUM%%\\2\\^&\\^&set \\/a TMPNUM=TMPNUM+1/g' ^
    > %MVSOURCE%
call %MVSOURCE%
cd "%IMGDIR%"
 
set ARCHIVE_TYPE=zip

rem time.
set BACKUP_DATE=%date:~0,4%%date:~5,2%%date:~8,2%
set /a BACKUP_TIME_HOUR=%time:~0,2%
if %BACKUP_TIME_HOUR%==0 set BACKUP_TIME_HOUR=00
set BACKUP_TIME=%BACKUP_TIME_HOUR%%time:~3,2%%time:~6,2%%time:~9,2%
set BACKUP_NOW=%BACKUP_DATE%-%BACKUP_TIME%
 
if "%2" == "" (
set "OUTPUT_PATH=%WORKING%\image.%BACKUP_NOW%.%ARCHIVE_TYPE%"
) else (
set "OUTPUT_PATH=%2"
)

7za.exe a -t%ARCHIVE_TYPE% "%OUTPUT_PATH%" "*"

rem clear.
cd %WORKING%
rm.exe -rf image temp
echo END (%ERRORLEVEL%).
