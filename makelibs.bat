@echo off
setlocal EnableExtensions
:::: makelibs.bat
::
:: Create .lib files from .dll + .def files (for libraries that do not provide their own lib files)

:::: INSTRUCTIONS ::::
:: First read instructions to run and then run "setup.ps1", which downloads
:: and sets up the library folder structures.
:: Next ensure any settings changes to output directories are also reflected in this batch file
::
:: Search for and open "Developer Command Prompt for VS 20xx" (replacing xx with your VS year)
::
:: >cd "<REPOPATH>"
:: >powershell ./setup.ps1
:: >makelibs.bat
::


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: CONFIGURATION ::

:: output directories for libraries (should be the same as in setup.ps1, if this was used)
set "FFmpeg=ffmpeg-3.2.4"
set   "FFTW=fftw-3.3.5"
set "Mpg123=mpg123-1.25.4"
set   "SFML=SFML-2.4.2"
REM set "FFmpeg=ffmpeg"
REM set   "FFTW=fftw"
REM set "Mpg123=mpg123"
REM set   "SFML=SFML"


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: RUN LIB SETUP ::

:: libs are only built if their directories are found

call :makelibs_arch "lib\%FFTW%" "x86"
call :makelibs_arch "lib\%FFTW%" "x64"
call :makelibs_arch "lib\%Mpg123%" "x86"
call :makelibs_arch "lib\%Mpg123%" "x64"
call :makelibs_arch "lib\%Mpg123%" "x86" "-debug"
call :makelibs_arch "lib\%Mpg123%" "x64" "-debug"

echo Finished!

endlocal
goto :eof


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: LIB SETUP ::

:: usage: call :makelibs_arch <Mpg123Dir> <Arch> [ArchPostfix]
:makelibs_arch
	setlocal EnableExtensions EnableDelayedExpansion
	if exist "%~1\%~2%~3" (
		for /r %%f in ("%~1\%~2%~3\*.dll.def") do (
			:: change any extensions from ".dll.def" to just ".def"
			set "file_=%%f"
			REM set "file_=!file_:~0,-8!.def"
			REM rename "%~1\%~2%~3\%%~nxf" "%~1\%~2%~3\!file_!"
			rename "%~1\%~2%~3\%%~nxf" "%~1\%~2%~3\!file_:~0,-8!.def"
		)

		for /r %%f in ("%~1\%~2%~3\*.def") do lib /NOLOGO /MACHINE:%~2 "/DEF:%~1\%~2%~3\%%~nxf" "/OUT:%~1\%~2%~3\%%~nf.lib"
	)
	endlocal
	goto :eof

