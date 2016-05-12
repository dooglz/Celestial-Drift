echo off
where /q cmake
IF %ERRORLEVEL%==1 (
    Echo cmake not in path
	SET p86=C:\Program Files (x86)
	PATH=%path%;%ProgramFiles(x86)%\CMake\bin
	PATH=%path%;%ProgramFiles%\\CMake\bin
 )
where /q cmake
IF %ERRORLEVEL%==1 (
    Echo Couldn't find cmake.exe
) ELSE ( 
   echo Cmake building for Visual Studio 12 2013 Win64
   cmake -G "Visual Studio 12 2013 Win64"
   echo cmake complete, open the sln
 )
pause > nul