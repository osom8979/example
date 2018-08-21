@echo off

call setup.bat --quiet

md5sum.exe %*
