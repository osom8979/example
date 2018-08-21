@echo off

call setup.bat --quiet

python %OPMRUNTIME%\opm-alter.py %*
