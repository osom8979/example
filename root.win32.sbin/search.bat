@echo off

call setup.bat --quiet

if "%1"=="" goto EXIT_ERROR

set "CHROME_HOME=C:\Program Files (x86)\Google\Chrome\Application"
set "CHROME_NAME=chrome.exe"
set "CHROME_PATH=%CHROME_HOME%\%CHROME_NAME%"

rem Global web.
set GOOGLE_URL=https://www.google.com/search?q=%%s^&ie=UTF-8
set YAHOO_URL=http://search.yahoo.com/search?p=%%s^&ei=UTF-8
set ABOUT_URL=http://search.about.com/?q=%%s
set EXCITE_URL=http://msxml.excite.com/search/web?q=%%s
rem Korea web.
set NAVER_URL=http://search.naver.com/search.naver?ie=UTF-8^&query=%%s
set DAUM_URL=http://search.daum.net/search?q=%%s
set NATE_URL=http://nate.search.empas.com/search/all.html?q=%%s
rem Dictionary.
set WIKIPEDIA_EN_URL=http://en.wikipedia.org/wiki/Special:Search?search=%%s
set WIKIPEDIA_KO_URL=http://ko.wikipedia.org/wiki/Special:Search?search=%%s
set NAVER_DIC=http://endic.naver.com/search.nhn?query=%%s
rem Translate.
set GOOGLE_TRANSLATE_URL=http://translate.google.co.kr/?source=osdd#auto^|auto^|%%s
rem Torrent.
set BITSNOOP_URL=http://bitsnoop.com/search/all/%%s/
rem Developer.
set CODEPROJECT_URL=http://www.codeproject.com/search.aspx?q=%%s^&doctypeid=1
set GITHUB_URL=https://github.com/search?q=%%s^&ref=cmdform
set STACKOVERFLOW_URL=http://stackoverflow.com/search?q=%%s

set SEARCH_GLOBAL="%GOOGLE_URL%" "%YAHOO_URL%" "%ABOUT_URL%" "%EXCITE_URL%"
set SEARCH_KOREA="%NAVER_URL%" "%DAUM_URL%" "%NATE_URL%"
set SEARCH_DICTIONARY="%WIKIPEDIA_EN_URL%" "%WIKIPEDIA_KO_URL%" "%NAVER_DIC%"
set SEARCH_TRANSLATE="%GOOGLE_TRANSLATE_URL%"
set SEARCH_TORRENT="%BITSNOOP_URL%"
set SEARCH_DEVELOPER="%CODEPROJECT_URL%" "%GITHUB_URL%" "%STACKOVERFLOW_URL%"

set SEARCH_ALL_URL=%SEARCH_GLOBAL% %SEARCH_KOREA% %SEARCH_DICTIONARY% %SEARCH_TRANSLATE% %SEARCH_TORRENT% %SEARCH_DEVELOPER%
set SEARCH_DEFAULT_URL="%GOOGLE_URL%" "%NAVER_URL%"


::------------
::Option list.
::------------
set OPTION_SEARCH_ALL=-a
set OPTION_SEARCH_GLOBAL=-g
set OPTION_SEARCH_KOREA=-k
set OPTION_SEARCH_DICTIONARY=-c
set OPTION_SEARCH_TRANSLATE=-t
set OPTION_SEARCH_TORRENT=-r
set OPTION_SEARCH_DEVELOPER=-d
set OPTION_PRINT_HELP=-h

::----------
::Flag list.
::----------
set OPTION_FLAG_ALL=0
set OPTION_FLAG_GLOBAL=0
set OPTION_FLAG_KOREA=0
set OPTION_FLAG_DICTIONARY=0
set OPTION_FLAG_TRANSLATE=0
set OPTION_FLAG_TORRENT=0
set OPTION_FLAG_DEVELOPER=0

rem Check option list...
:CHECK_OPTION_LOOP
set NOWPARAM=%1
if "%NOWPARAM:~0,1%" == "-" (
if "%NOWPARAM%" equ "%OPTION_PRINT_HELP%" goto PRINT_HELP
if "%NOWPARAM%" equ "%OPTION_SEARCH_ALL%"        set OPTION_FLAG_ALL=1
if "%NOWPARAM%" equ "%OPTION_SEARCH_GLOBAL%"     set OPTION_FLAG_GLOBAL=1
if "%NOWPARAM%" equ "%OPTION_SEARCH_KOREA%"      set OPTION_FLAG_KOREA=1
if "%NOWPARAM%" equ "%OPTION_SEARCH_DICTIONARY%" set OPTION_FLAG_DICTIONARY=1
if "%NOWPARAM%" equ "%OPTION_SEARCH_TRANSLATE%"  set OPTION_FLAG_TRANSLATE=1
if "%NOWPARAM%" equ "%OPTION_SEARCH_TORRENT%"    set OPTION_FLAG_TORRENT=1
if "%NOWPARAM%" equ "%OPTION_SEARCH_DEVELOPER%"  set OPTION_FLAG_DEVELOPER=1
SHIFT
)

rem echo OPTION_FLAG_ALL=%OPTION_FLAG_ALL%
rem echo OPTION_FLAG_GLOBAL=%OPTION_FLAG_GLOBAL%
rem echo OPTION_FLAG_KOREA=%OPTION_FLAG_KOREA%
rem echo OPTION_FLAG_DICTIONARY=%OPTION_FLAG_DICTIONARY%
rem echo OPTION_FLAG_TRANSLATE=%OPTION_FLAG_TRANSLATE%
rem echo OPTION_FLAG_TORRENT=%OPTION_FLAG_TORRENT%
rem echo OPTION_FLAG_DEVELOPER=%OPTION_FLAG_DEVELOPER%

rem Search!!!
:SEARCH_START

set /a CHECK_YES_FLAG=%OPTION_FLAG_ALL% + %OPTION_FLAG_GLOBAL% + %OPTION_FLAG_KOREA% + %OPTION_FLAG_DICTIONARY% + %OPTION_FLAG_TRANSLATE% + %OPTION_FLAG_TORRENT% + %OPTION_FLAG_DEVELOPER%

set SEARCH_SELECT=
if %CHECK_YES_FLAG% neq 0 (
if %OPTION_FLAG_GLOBAL%     equ 1 set SEARCH_SELECT=%SEARCH_GLOBAL%     %SEARCH_SELECT%
if %OPTION_FLAG_KOREA%      equ 1 set SEARCH_SELECT=%SEARCH_KOREA%      %SEARCH_SELECT%
if %OPTION_FLAG_DICTIONARY% equ 1 set SEARCH_SELECT=%SEARCH_DICTIONARY% %SEARCH_SELECT%
if %OPTION_FLAG_TRANSLATE%  equ 1 set SEARCH_SELECT=%SEARCH_TRANSLATE%  %SEARCH_SELECT%
if %OPTION_FLAG_TORRENT%    equ 1 set SEARCH_SELECT=%SEARCH_TORRENT%    %SEARCH_SELECT%
if %OPTION_FLAG_DEVELOPER%  equ 1 set SEARCH_SELECT=%SEARCH_DEVELOPER%  %SEARCH_SELECT%
if %OPTION_FLAG_ALL%        equ 1 set SEARCH_SELECT=%SEARCH_ALL_URL%
) ELSE (
set SEARCH_SELECT=%SEARCH_DEFAULT_URL%
)

if "%1" == "" goto EXIT_ERROR
set INPUT_SEARCH=%1
echo.
echo SEARCH STRING: %INPUT_SEARCH%
echo SEARCH_SELECT: %SEARCH_SELECT%
echo %SEARCH_SELECT% | sed "s/\%%s/%INPUT_SEARCH%/g" | xargs "%CHROME_PATH%"

goto EXIT_SUCCESS

:PRINT_HELP
echo instruction search script.
echo  search.bat [option] [search_string]
echo  search.bat -h
echo.
echo Option list.
echo  %OPTION_SEARCH_ALL% All search.
echo  %OPTION_SEARCH_GLOBAL% search for Global.
echo  %OPTION_SEARCH_KOREA% search for Koread .
echo  %OPTION_SEARCH_DICTIONARY% search for diCtionary.
echo  %OPTION_SEARCH_TRANSLATE% Translate string.
echo  %OPTION_SEARCH_TORRENT% search for toRrent.
echo  %OPTION_SEARCH_DEVELOPER% search for Developer.
echo  %OPTION_PRINT_HELP% print help message.
goto EXIT_SUCCESS

:EXIT_SUCCESS
exit /B 0

:EXIT_ERROR
echo.
echo Illegal argument exception...
echo instruction search script.
echo  search.bat [option] [search_string]
echo  search.bat -h
exit /b 2
