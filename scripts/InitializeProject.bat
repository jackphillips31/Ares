@echo off
setlocal enabledelayedexpansion

:: Define paths
set TemplateFile=template.lua
set OutputFile=..\premake5.lua
set PremakeExe=premake5.exe

:: Check if template.lua exists
if not exist "%TemplateFile%" (
	echo Error: %TemplateFile% does not exist in the current folder.
	pause
	exit /b 1
)

:: Check if premake5.exe exists
if not exist "%PremakeExe%" (
	echo Error: %PremakeExe% does not exist in the current folder.
	pause
	exit /b 1
)

:: Ask the user for project name
echo What is the name of your project?
set /p ProjectName=
cls

:: Ask the user for internal development name
echo What is the internal development name of your project?
set /p InternalName=
cls

:: Read and replace placeholders in template.lua
echo Processing template...
(for /f "usebackq delims=" %%A in ("%TemplateFile%") do (
    set Line=%%A
    set Line=!Line:TEMPLATENAME=%ProjectName%!
    set Line=!Line:TEMPLATEINTERNALNAME=%InternalName%!
    echo !Line!
)) > "%OutputFile%"

echo Template processed. Created "%OutputFile%".

:: Ask if user wants to create a Premake script
:ask_create_premake_script
echo Do you want to create a script for running premake? (y/n)
set /p CreateScript=

if /i "!CreateScript!"=="y" goto ask_dev_env
if /i "!CreateScript!"=="n" exit /b 0

:: Repeat question if answer is neither y nor n
cls
echo Invalid input. Please enter 'y' or 'n'.
goto ask_create_premake_script

:: Ask the user for the development enviroment
:ask_dev_env
cls
echo Select the development environment from the list below:
%PremakeExe% --help | findstr /r /c:"^ [a-z]"
set /p DevEnv=Enter the environment name:

:: Create the batch script for Premake
cls
set PremakeScript=GenerateProjectFiles.bat
echo @echo off > "%PremakeScript%"
echo pushd %%~dp0\..\ >> "%PremakeScript%"
echo call scripts\premake5.exe %DevEnv% >> "%PremakeScript%"
echo popd >> "%PremakeScript%"
echo pause >> "%PremakeScript%"

echo Premake script "%PremakeScript%" created with environment "%DevEnv%".

:: Ask if the user wants to run the script
:ask_run_script
echo Do you want to run the script now? (y/n)
set /p RunScript=

if /i "!RunScript!"=="y" (
	cls
	echo Running the Premake script...
	call "%PremakeScript%"
) else if /i "!RunScript!"=="n" (
	exit b/ 0
) else (
	cls
	echo Invalid input. Please enter 'y' or 'n'.
	goto ask_run_script
)