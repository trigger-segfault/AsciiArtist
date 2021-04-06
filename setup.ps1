## setup.ps1
#
# Download and prepare dependency libraries used by projects in this repository

## INSTRUCTIONS ##
# First choose any configuration options you want changed in the section
# below labeled CONFIGURATION. Next ensure any settings changes to output
# directories are also reflected in makelibs.bat
#
# Search for and open "Developer Command Prompt for VS 20xx" (replacing xx with your VS year)
#
# >cd "<REPOPATH>"
# >powershell ./setup.ps1
# >makelibs.bat
#


################################################################################
## CONFIGURATION

# destination for output libraries
# current directory should be: "<REPO>"
$Dest = ".\lib";
$DestTemp = "\_tmp";  # appending to $Dest

# architectures to prepare
$x86 = $true;
$x64 = $true;

# copy over and merge all contents of downloaded libraries (even unused files)
$OptionalExtras = $false;

# output directories for libraries
$FFmpeg = "ffmpeg-3.2.4";
$FFTW   = "fftw-3.3.5";
$Mpg123 = "mpg123-1.25.4";
$SFML   = "SFML-2.4.2";
#$FFmpeg = "ffmpeg";
#$FFTW   = "fftw";
#$Mpg123 = "mpg123";
#$SFML   = "SFML";


################################################################################
## SETUP FUNCTIONS


## LIBRARY ACQUIRE ##

# specify -Subdir $true if the downloaded archive has no root folder within
#  [string] parameter type excluded from $Name to allow passing $null
function Acquire([string]$Path, [boolean]$Subdir, $Name, [string]$Url) {
	Add-Type -assembly "System.IO.Compression.Filesystem";

	# download zip file to extract dir (before adding subdir)
	$ExtractPath = "$Path$DestTemp";
	New-Item -ItemType directory -Path "$ExtractPath" -Force | Out-Null;
	$ZipFile     = "$ExtractPath\" + $(Split-Path -Path $Url -Leaf);
	Write-Host "Downloading $(Split-Path -Path $Url -Leaf)";
	Invoke-WebRequest -Uri $Url -OutFile "$ZipFile";

	# extract zip file to extract dir (after adding subdir)
	if ($Name -ne $null) {
		$OutputPath  = "$ExtractPath\$Name";
	} else {
		$OutputPath  = "$ExtractPath\" + (Get-Item $ZipFile).BaseName;
	}
	if ($Subdir) {
		$ExtractPath = "$ExtractPath\" + (Get-Item $ZipFile).BaseName;
		New-Item -ItemType directory -Path "$ExtractPath" -Force | Out-Null;
	}
	if (Test-Path -Path "$OutputPath") {
		# cleanup extracted dir ahead of time (because ZipFile::ExtractToDirectory wants you to suffer)
		Get-ChildItem -Path "$OutputPath" -Recurse | Remove-Item -Force -Recurse;
	}
	[IO.Compression.Zipfile]::ExtractToDirectory("$ZipFile", "$ExtractPath");

	# cleanup zip file, now that it's extracted
	Remove-Item "$ZipFile" -Force;
	return "$OutputPath";
}

## LIBRARY-SPECIFIC SETUP ##

function Setup-FFmpeg-Dev([string]$Path, [string]$Name, [string]$Arch, [string]$Url) {
	$Dev = $(Acquire "$Path" $false $null "$Url");
	$Path = "$Path\$Name";
	# required items
	New-Item -ItemType directory -Path "$Path\lib\$Arch" -Force | Out-Null
	New-Item -ItemType directory -Path "$Path\include"   -Force | Out-Null
	New-Item -ItemType directory -Path "$Path\licenses"  -Force | Out-Null
	Copy-Item -Path "$Dev\lib\*.def"    -Destination "$Path\lib\$Arch" -Force -Recurse
	Copy-Item -Path "$Dev\lib\*.lib"    -Destination "$Path\lib\$Arch" -Force -Recurse
	Copy-Item -Path "$Dev\include\*"    -Destination "$Path\include"   -Force -Recurse
	Copy-Item -Path "$Dev\licenses\*"   -Destination "$Path\licenses"  -Force -Recurse
	Copy-Item -Path "$Dev\README.txt"   -Destination "$Path"           -Force

	# optional items
	if ($OptionalExtras) {
		New-Item -ItemType directory -Path "$Path\doc"       -Force | Out-Null
		Copy-Item -Path "$Dev\lib\*.a"      -Destination "$Path\lib\$Arch" -Force -Recurse
		Copy-Item -Path "$Dev\doc\*"        -Destination "$Path\doc"       -Force -Recurse
	}
	# cleanup
	Get-ChildItem -Path "$Dev" -Recurse | Remove-Item -Force -Recurse
	Remove-Item "$Dev" -Force
}

function Setup-FFmpeg-Shared([string]$Path, [string]$Name, [string]$Arch, [string]$Url) {
	$Shared = $(Acquire "$Path" $false $null "$Url");
	$Path = "$Path\$Name";
	# required items
	New-Item -ItemType directory -Path "$Path\bin\$Arch" -Force | Out-Null
	Copy-Item -Path "$Shared\bin\*.dll" -Destination "$Path\bin\$Arch" -Force -Recurse

	# optional items
	if ($OptionalExtras) {
		New-Item -ItemType directory -Path "$Path\presets"   -Force | Out-Null
		Copy-Item -Path "$Shared\bin\*.exe" -Destination "$Path\bin\$Arch" -Force -Recurse
		Copy-Item -Path "$Shared\presets\*" -Destination "$Path\presets"   -Force -Recurse
	}
	# cleanup
	Get-ChildItem -Path "$Shared" -Recurse | Remove-Item -Force -Recurse
	Remove-Item "$Shared" -Force
}

function Setup-FFmpeg-Static([string]$Path, [string]$Name, [string]$Arch, [string]$Url) {
	# not used for build process
	if (-Not $OptionalExtras) { return; }

	$Static = $(Acquire "$Path" $false $null "$Url");
	$Path = "$Path\$Name";
	# required items
	New-Item -ItemType directory -Path "$Path\bin\$Arch" -Force | Out-Null
	Copy-Item -Path "$Static\bin\*.exe" -Destination "$Path\bin\$Arch" -Force -Recurse

	# cleanup
	Get-ChildItem -Path "$Static" -Recurse | Remove-Item -Force -Recurse
	Remove-Item "$Static" -Force
}

function Setup-FFTW([string]$Path, [string]$Name, [string]$Arch, [string]$Url) {
	$Source = $(Acquire "$Path" $true $null "$Url");
	$Path = "$Path\$Name";
	# required items
	New-Item -ItemType directory -Path "$Path\$Arch" -Force | Out-Null
	Copy-Item -Path "$Source\*.dll"          -Destination "$Path\$Arch" -Force
	Copy-Item -Path "$Source\*.def"          -Destination "$Path\$Arch" -Force
	Copy-Item -Path "$Source\*.h"            -Destination "$Path"       -Force
	Copy-Item -Path "$Source\README"         -Destination "$Path"       -Force
	Copy-Item -Path "$Source\COPYING"        -Destination "$Path"       -Force
	Copy-Item -Path "$Source\COPYRIGHT"      -Destination "$Path"       -Force
	
	# optional items
	if ($OptionalExtras) {
		Copy-Item -Path "$Source\*.exe"          -Destination "$Path\$Arch" -Force
		Copy-Item -Path "$Source\README-WINDOWS" -Destination "$Path\$Arch" -Force
		Copy-Item -Path "$Source\README-bench"   -Destination "$Path"       -Force
		Copy-Item -Path "$Source\NEWS"           -Destination "$Path"       -Force
	}
	# cleanup
	Get-ChildItem -Path "$Source" -Recurse | Remove-Item -Force -Recurse
	Remove-Item "$Source" -Force
}

function Setup-Mpg123([string]$Path, [string]$Name, [string]$Arch, [string]$Url) {
	$Source = $(Acquire "$Path" $false $null "$Url");
	$Path = "$Path\$Name";
	# required items
	New-Item -ItemType directory -Path "$Path\$Arch" -Force | Out-Null
	Copy-Item -Path "$Source\*.dll"          -Destination "$Path\$Arch" -Force
	Copy-Item -Path "$Source\*.def"          -Destination "$Path\$Arch" -Force
	Copy-Item -Path "$Source\*.h"            -Destination "$Path"       -Force
	Copy-Item -Path "$Source\README.txt"     -Destination "$Path"       -Force
	Copy-Item -Path "$Source\COPYING.txt"    -Destination "$Path"       -Force

	# correct ".dll.def" extensions to ".def"
	# <https://stackoverflow.com/a/32314505/7517185>
	Get-ChildItem -Path "$Path\$Arch\*.dll.def" | %{
		# extra handling because Rename-Item will fail when the file already exists.  more headaches...
		$NewName = $_.Name -Replace ".dll.def", ".def"
		$NewDestination = Join-Path -Path $_.Directory.FullName -ChildPath $NewName
		Move-Item -Path $_.FullName -Destination $NewDestination -Force
	}

	# optional items
	if ($OptionalExtras) {
		New-Item -ItemType directory -Path "$Path\$Arch\plugins" -Force | Out-Null
		Copy-Item -Path "$Source\plugins\*"      -Destination "$Path\$Arch\plugins" -Force -Recurse
		Copy-Item -Path "$Source\*.exe"          -Destination "$Path\$Arch" -Force
		Copy-Item -Path "$Source\NEWS.txt"       -Destination "$Path"       -Force
	}
	# cleanup
	Get-ChildItem -Path "$Source" -Recurse | Remove-Item -Force -Recurse
	Remove-Item "$Source" -Force
}

function Setup-SFML([string]$Path, [string]$Name, [string]$Arch, [string]$ExtractName, [string]$Url) {
	$Source = $(Acquire "$Path" $false "$ExtractName" "$Url");
	$Path = "$Path\$Name";
	# required items
	New-Item -ItemType directory -Path "$Path\bin\$Arch"      -Force | Out-Null
	New-Item -ItemType directory -Path "$Path\lib\$Arch"      -Force | Out-Null
	New-Item -ItemType directory -Path "$Path\include"        -Force | Out-Null
	Copy-Item -Path "$Source\bin\*"       -Destination "$Path\bin\$Arch" -Force -Recurse
	Copy-Item -Path "$Source\lib\*"       -Destination "$Path\lib\$Arch" -Force -Recurse
	Copy-Item -Path "$Source\include\*"   -Destination "$Path\include"   -Force -Recurse
	Copy-Item -Path "$Source\license.txt" -Destination "$Path"           -Force
	Copy-Item -Path "$Source\readme.txt"  -Destination "$Path"           -Force

	# optional items
	if ($OptionalExtras) {
		New-Item -ItemType directory -Path "$Path\examples\$Arch" -Force | Out-Null
		New-Item -ItemType directory -Path "$Path\cmake"          -Force | Out-Null
		New-Item -ItemType directory -Path "$Path\doc"            -Force | Out-Null
		Copy-Item -Path "$Source\examples\*" -Destination "$Path\examples\$Arch" -Force -Recurse
		Copy-Item -Path "$Source\cmake\*"    -Destination "$Path\cmake"          -Force -Recurse
		Copy-Item -Path "$Source\doc\*"      -Destination "$Path\doc"            -Force -Recurse
	}
	# cleanup
	Get-ChildItem -Path "$Source" -Recurse | Remove-Item -Force -Recurse
	Remove-Item "$Source" -Force
}


################################################################################
## SETUP LIBRARIES

New-Item -ItemType directory -Path "$Dest$DestTemp" -Force | Out-Null;

Write-Host "Preparing FFmpeg libraries..."
# 3rd commented-out item: optional full-featured ffmpeg executables
if ($x86) {	Setup-FFmpeg-Dev    "$Dest" "$FFmpeg" "x86" "https://web.archive.org/web/20200819162211if_/https://ffmpeg.zeranoe.com/builds/win32/dev/ffmpeg-3.2.4-win32-dev.zip"
			Setup-FFmpeg-Shared "$Dest" "$FFmpeg" "x86" "https://web.archive.org/web/20200305154853if_/https://ffmpeg.zeranoe.com/builds/win32/shared/ffmpeg-3.2.4-win32-shared.zip"
			Setup-FFmpeg-Static "$Dest" "$FFmpeg" "x86" "https://web.archive.org/web/20170419055331if_/https://ffmpeg.zeranoe.com/builds/win32/static/ffmpeg-3.2.4-win32-static.zip"
}
if ($x64) {	Setup-FFmpeg-Dev    "$Dest" "$FFmpeg" "x64" "https://web.archive.org/web/20200819182628if_/https://ffmpeg.zeranoe.com/builds/win64/dev/ffmpeg-3.2.4-win64-dev.zip"
			Setup-FFmpeg-Shared "$Dest" "$FFmpeg" "x64" "https://web.archive.org/web/20200414063958if_/https://ffmpeg.zeranoe.com/builds/win64/shared/ffmpeg-3.2.4-win64-shared.zip"
			Setup-FFmpeg-Static "$Dest" "$FFmpeg" "x64" "https://web.archive.org/web/20180408233456if_/https://ffmpeg.zeranoe.com/builds/win64/static/ffmpeg-3.2.4-win64-static.zip"
}

Write-Host ""
Write-Host "Preparing FFTW libraries..."
if ($x86) {	Setup-FFTW "$Dest" "$FFTW" "x86" "ftp://ftp.fftw.org/pub/fftw/fftw-3.3.5-dll32.zip"
}
if ($x64) {	Setup-FFTW "$Dest" "$FFTW" "x64" "ftp://ftp.fftw.org/pub/fftw/fftw-3.3.5-dll64.zip"
}

Write-Host ""
Write-Host "Preparing mpg123 libraries..."
# 2nd commented-out item: optional debug libraries
if ($x86) {	Setup-Mpg123 "$Dest" "$Mpg123" "x86" "https://www.mpg123.de/download/win32/1.25.4/mpg123-1.25.4-x86.zip"
			Setup-Mpg123 "$Dest" "$Mpg123" "x86-debug" "https://www.mpg123.de/download/win32/1.25.4/mpg123-1.25.4-x86-debug.zip"
}
if ($x64) {	Setup-Mpg123 "$Dest" "$Mpg123" "x64" "https://www.mpg123.de/download/win64/1.25.4/mpg123-1.25.4-x86-64.zip"
			Setup-Mpg123 "$Dest" "$Mpg123" "x64-debug" "https://www.mpg123.de/download/win64/1.25.4/mpg123-1.25.4-x86-64-debug.zip"
}

Write-Host ""
Write-Host "Preparing SFML libraries..."
# SFML extracted folder is named "SFML-<ver>", not the same as the zip
if ($x86) {	Setup-SFML "$Dest" "$SFML" "x86" "SFML-2.4.2" "https://www.sfml-dev.org/files/SFML-2.4.2-windows-vc14-32-bit.zip"
}
if ($x64) {	Setup-SFML "$Dest" "$SFML" "x64" "SFML-2.4.2" "https://www.sfml-dev.org/files/SFML-2.4.2-windows-vc14-64-bit.zip"
}

Remove-Item "$Dest$DestTemp" -Force
Write-Host ""
Write-Host "Complete!"

################################################################################
