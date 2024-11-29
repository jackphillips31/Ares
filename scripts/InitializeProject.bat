@echo off
setlocal enabledelayedexpansion

:: Define paths
set TemplateFile=template.lua
set OutputFile=..\premake5.lua
set PremakeExe=premake5.exe

:: Define time variable
set CompleteTime=""
set OnStep=0

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

:: Get time for premake file
set OnStep=0
goto GetDateAndTime
:datetime_step0

:: Get time from template
set TemplateLineNumber=0
set TemplateDateTime=
for /f "usebackq delims=" %%A in ("%TemplateFile%") do (
	set /a TemplateLineNumber+=1
	if !TemplateLineNumber! equ 2 (
		set Line=%%A
		set TemplateDateTime=!Line:-- =!
		goto template_time_retrieved
	)
)
:template_time_retrieved

:: Read and replace placeholders in template.lua
echo Processing template...
echo -- premake5.lua was generated on: %CompleteTime% > "%OutputFile%"
echo -- Using template.lua from:       %TemplateDateTime% >> "%OutputFile%"
set Counter=0
(for /f "usebackq delims=" %%A in ("%TemplateFile%") do (
	set /a Counter+=1
	if !Counter! gtr 2 (
		set Line=%%A
		set Line=!Line:TEMPLATENAME=%ProjectName%!
		set Line=!Line:TEMPLATEINTERNALNAME=%InternalName%!
		echo !Line!
	)
)) >> "%OutputFile%"

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

:: Get time for batch script
set OnStep=1
goto GetDateAndTime
:datetime_step1

:: Create the batch script for Premake
cls
set PremakeScript=GenerateProjectFiles.bat
echo :: GenerateProjectFiles.bat was generated on: > "%PremakeScript%"
echo :: %CompleteTime% >> "%PremakeScript%"
echo @echo off >> "%PremakeScript%"
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

exit /b 0

:GetDateAndTime
:: Fetch the raw output of wmic and parse it
for /f "skip=1 tokens=1 delims=." %%A in ('wmic os get localdatetime') do (
    set "DateTime=%%A"
    goto :ParseDateTime
)

:ParseDateTime
:: Parse the DateTime string into components
set "YYYY=%DateTime:~0,4%"
set "MM=%DateTime:~4,2%"
set "DD=%DateTime:~6,2%"
set "HH=%DateTime:~8,2%"
set "MI=%DateTime:~10,2%"

:: Adjust the hour for UTC (Arizona is UTC-7)
set /a HH=1%HH%-100+7
if %HH% geq 24 (
    set /a HH-=24
    set /a DD+=1
)
if %HH% lss 0 (
    set /a HH+=24
    set /a DD-=1
)

:: Add leading zero to single-digit hours
if %HH% lss 10 set HH=0%HH%

:: Determine days in the current month
set "DaysInMonth=31"
if %MM%==04 set "DaysInMonth=30"
if %MM%==06 set "DaysInMonth=30"
if %MM%==09 set "DaysInMonth=30"
if %MM%==11 set "DaysInMonth=30"
if %MM%==02 (
	:: Check for leap year
	set "DaysInMonth=28"
	set /a LeapYear=YYYY %% 4
	if !LeapYear!==0 (
		set /a LeapYear=YYYY %% 100
		if not !LeapYear!==0 set "DaysInMonth=29"
		set /a LeapYear=YYYY %% 400
		if !LeapYear!==0 set "DaysInMonth=29"
	)
)

:: Handle month/year rollover for increasing day
if %DD% gtr %DaysInMonth% (
	set DD=1
	set /a MM+=1
	if !MM! gtr 12 (
		set MM=01
		set /a YYYY+=1
	)
)

:: Handle month/year rollover for decreasing day
if %DD% lss 1 (
	set /a MM-=1
	if !MM! lss 1 (
		set MM=12
		set /a YYYY-=1
	)
	:: Recalculate days in the new month
	set "DaysInMonth=31"
	if %MM%==04 set "DaysInMonth=30"
  if %MM%==06 set "DaysInMonth=30"
  if %MM%==09 set "DaysInMonth=30"
  if %MM%==11 set "DaysInMonth=30"
	if !MM!==2 (
		set "DaysInMonth=28"
		set /a LeapYear=YYYY %% 4
		if !LeapYear!==0 (
			set /a LeapYear=YYYY %% 100
			if not !LeapYear!==0 set "DaysInMonth=29"
			set /a LeapYear=YYYY %% 400
			if !LeapYear!==0 set "DaysInMonth=29"
		)
	)
	set DD=%DaysInMonth%
)

:: Convert numeric month to abbreviated month name
for %%M in (Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec) do (
	set /a Index+=1
	if !Index! equ %MM% set "Month=%%M"
)

:: Convert hour from 24-hour format to 12-hour format
set /a Hour12=%HH% %% 12
if %Hour12%==0 set Hour12=12
set "AMPM=AM"
if %HH% geq 12 set "AMPM=PM"
if %Hour12% lss 10 set "Hour12=0%Hour12%"

:: Format the date and time
set "FormattedDate=%Month%. %DD%, %YYYY%"
set "FormattedTime=%Hour12%:%MI%%AMPM% UTC"
set "CompleteTime=%FormattedDate% - %FormattedTime%"

if !OnStep!==0 goto datetime_step0
if !OnStep!==1 goto datetime_step1

endlocal
exit /b 0