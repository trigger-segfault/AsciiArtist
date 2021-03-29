# Build Instructions
*because ~~you'll~~ I'll need them...*

**IDE:** VS 2019 (built in VS 2015, but upgrading seems to work without a hitch)

**Configuration:** Release, x86

## Libraries

* [ffmpeg-3.2.4-win32-dev](https://web.archive.org/web/20170419055315/https://ffmpeg.zeranoe.com/builds/win32/dev/ffmpeg-3.2.4-win32-dev.zip) <sup>(webarchive, [landing page](https://web.archive.org/web/20170419055315/https://ffmpeg.zeranoe.com/builds/win32/dev/))</sup>
* [fftw-3.3.5-dll32](ftp://ftp.fftw.org/pub/fftw/fftw-3.3.5-dll32.zip) <sup>([landing page](http://fftw.org/install/windows.html))</sup>
* [mpg123-1.24.0-x86](https://www.mpg123.de/download/win32/1.24.0/mpg123-1.24.0-x86.zip) <sup>([landing page](https://www.mpg123.de/download/win32/1.24.0/))</sup>
* [SFML-2.4.2-windows-vc14-32-bit](https://www.sfml-dev.org/files/SFML-2.4.2-windows-vc14-32-bit.zip) <sup>([landing page](https://www.sfml-dev.org/download/sfml/2.4.2/))</sup>

### Library folder structure

Create the following folder structure, and place the 3 libraries listed above in each.

```
+ C:/
+-+ Libraries/
  +-- ffmpeg-3.2.4/
  +-- fftw-3.3.5/
  +-- mpg123-1.24.0/
  +-- SFML-2.4.2/
```

### Creating `.lib` files

For `mpg123-1.24.0/`, you'll need to create a `.lib` file, which can be generated with the `.def` and `.dll` file *(as described in [this answer](https://stackoverflow.com/a/15226566/7517185))*.

First create copies of the files `libmpg123-0.dll.def` and `libout123-0.dll.def` files with the `.dll` part of the names removed. (I'm not sure if this is required, but the generated files will all contain that same `.dll` name part, so better be safe)

Search `Developer Command Prompt for VS 2019` in Windows and open. Then run the following commands.

```bat
cd "C:\Libraries\mpg123-1.24.0"
lib /def:libmpg123-0.def
lib /def:libout123-0.def

cd "C:\Libraries\fftw-3.3.5"
lib /def:libfftw3-3.dll
lib /def:libfftw3f-3.def
lib /def:libfftw3l-3.def
```

The output will look something like this for each file.
```bat
Microsoft (R) Library Manager Version 14.21.27702.2
Copyright (C) Microsoft Corporation.  All rights reserved.

LINK : warning LNK4068: /MACHINE not specified; defaulting to X86
   Creating library libout123-0.lib and object libout123-0.exp
```

***

## Programs

### AsciiTetris

**Required libraries:**

* `C:/Libraries/mpg123-1.24.0/`
* `C:/Libraries/SFML-2.4.2/`


**Files/folders to place in `Release/` after building:**

All files and folders are present in the `AsciiTetris/` project directory.

* `Resources/` folder (Otherwise you'll get an error claiming `libgcc_s_sjlj-1.dll` is missing)
* `libmpg123-0.dll`
* `libout123-0.dll`
* `openal32.dll`



### AsciiVis

**Note:** There's likely a lot of hardcoded filepaths in here that'll need changing.

**Required libraries:**
* `C:/Libraries/ffmpeg-3.2.4/`
* `C:/Libraries/fftw-3.3.5/`
* `C:/Libraries/mpg123-1.24.0/`
* `openal32.*` (see notes below)

For OpenAL32, there are no folders linked that would normally contain it... *whoops*. To fix this, take the following files:

* `C:/Libraries/SFML-2.4.2/bin/openal32.dll`
* `C:/Libraries/SFML-2.4.2/lib/openal32.lib`

And copy these to a library folder the AsciiVis project actually scans for *(or fix it the proper way by adding a new search path)*.

For example, place both these files in `fftw-3.3.5/`.


**Files/folders to place in `Release/` after building:**

All files and folders are present in the `AsciiTetris/` project directory.

* `Resources/` folder
* *probably more, TODO...*
