@echo off

:predictionChoice
	call :selectProgram
	set /p c=Enter training mode [T] or prediction mode [P]? 
	if /I "%c%" EQU "T" goto :trainingMode
	if /I "%c%" EQU "P" goto :predictionMode
goto :predictionChoice
	
:selectProgram
	ECHO %PROCESSOR_ARCHITECTURE%|FINDSTR AMD64>NUL && SET Program="Program/WhiskeyNN.exe" || SET Program="Program/WhiskeyNN.exe"
goto :eof
	
:userInput
	SET "%~1=%~3"
	set /p "%~1=%~2 [default %~3]: "
goto :eof
	
:trainingMode	
	call :userInput inIonMode "Ion Mode" pos
	call :userInput inpH "pH" neut
	call :userInput inTarget "Input target file in CSV format [target or target_brand]" sem2target
	

:trainingModeParams
	call :userInput inData "Input unknown data file in CSV format" sem2data
	call :userInput outNetwork "Output network name" "network"
	call :userInput nHidden "Number of hidden neurons" 100
	call :userInput nLearn "Learning rate" 0.16
	call :userInput nAnneal "Annealing rate" 1000
	call :userInput nMomentum "Learning momentum" 0.9

	call :userInput nRepeats "Number of repeats" 1
	
	@echo on
		call %Program% -k %nHidden% -l %nLearn% -a %nAnneal% -m %nMomentum% -d Data/%inIonMode%/%inpH%/%inData%.csv -c Data/%inIonMode%/%inpH%/%inTarget%.csv -n Models/%inIonMode%/%inpH%/%outNetwork% -r %nRepeats%
	@echo off
	
	set /p cont=Continue [Y/N]? 
	if /I "%cont%" EQU "N" goto :programEnd
goto :trainingModeParams

:predictionMode

	call :userInput inIonMode "Ion Mode" pos
	call :userInput inpH "pH" neut
	call :userInput inNetwork "Input network file in DATA format" network
	call :userInput inData "Input unknown data file in CSV format" data
	
	@echo on
		call %Program% -t -n Models/%inIonMode%/%inpH%/%inNetwork% -d Data/%inIonMode%/%inpH%/%inData%.csv
	@echo off
	
	set /p cont=Continue [Y/N]? 
	if /I "%cont%" EQU "Y" goto :predictionMode
	goto :programEnd
goto :eof

:programEnd
	pause
goto :eof
