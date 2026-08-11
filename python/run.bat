@echo off

set "PYTHONPATH=%~dp0main"

.\python_embedded\python.exe -s main\test.py

pause