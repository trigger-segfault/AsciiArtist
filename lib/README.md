# External libraries

This page is a work in-progress, and **does not yet reflect** changes made to the project library loading.

### External library usage

|                 |PowerCon.|AsciiArtist|AsciiTetris|AsciiVis|SFMLCust.|
|:----------------|:-------:|:---------:|:---------:|:------:|:--------:|
|[FFmpeg](#ffmpeg)|         |&check;&sup1;|         |&check; |&check;   |
|[FFTW](#fftw)    |         |           |           |&check; |          |
|[mpg123](#mpg123)|         |           |&check;    |&check; |          |
|[SFML](#sfml)    |         |           |&check;    |&check;&sup2;|&check;|

<sup>\[1\] : (optional) FFmpeg main binary used at runtime for optional feature</sup><br>
<sup>\[2\] : SFML borrowed only for openal32 `.dll` and `.lib` files</sup>


***

## Folder structure

#### x86 platform folder structure

`lib/`
* `ffmpeg-3.2.4-win32/`
    * `bin/` &emsp; (*from <kbd>[ffmpeg-3.2.4-win32-shared.zip][]</kbd>*)
    * `include/` &emsp; (*from <kbd>[ffmpeg-3.2.4-win32-dev.zip][]</kbd>*)
    * `lib/` &emsp; (*from <kbd>[ffmpeg-3.2.4-win32-dev.zip][]</kbd>*)
* `fftw-3.3.5-dll32/`
    * `fftw3.h`
    * `libfftw3f-3.dll`
    * `libfftw3f-3.def`
    * `libfftw3f-3.lib` &emsp; (*created: <kbd>LIB /MACHINE:X86 /DEF:libfftw3f-3.def</kbd> in VS dev console*)
* `mpg123-1.24.0-x86/` *and* `mpg123-1.24.0-x86-debug/`
    * `fmt123.h`
    * `mpg123.h`
    * `libmpg123-0.dll`
    * `libmpg123-0.def` &emsp; (*renamed: <kbd>rename libmpg123-0.dll.def libmpg123-0.def</kbd>*)
    * `libmpg123-0.lib` &emsp; (*created: <kbd>LIB /MACHINE:X86 /DEF:libmpg123-0.def</kbd> in VS dev console*)
* `SFML-2.4.2-windows-vc14-32-bit/`
    * `bin/`
    * `include/`
    * `lib/`

#### x64 platform folder structure

`lib/`
* `ffmpeg-3.2.4-win64/`
    * `bin/` &emsp; (*from <kbd>[ffmpeg-3.2.4-win64-shared.zip][]</kbd>*)
    * `include/` &emsp; (*from <kbd>[ffmpeg-3.2.4-win64-dev.zip][]</kbd>*)
    * `lib/` &emsp; (*from <kbd>[ffmpeg-3.2.4-win64-dev.zip][]</kbd>*)
* `fftw-3.3.5-dll64/`
    * `fftw3.h`
    * `libfftw3f-3.dll`
    * `libfftw3f-3.def`
    * `libfftw3f-3.lib` &emsp; (*created: <kbd>LIB /MACHINE:X64 /DEF:libfftw3f-3.def</kbd> in VS dev console*)
* `mpg123-1.24.0-x86-64/` *and* `mpg123-1.24.0-x86-64-debug/`
    * `fmt123.h`
    * `mpg123.h`
    * `libmpg123-0.dll`
    * `libmpg123-0.def` &emsp; (*renamed: <kbd>rename libmpg123-0.dll.def libmpg123-0.def</kbd>*)
    * `libmpg123-0.lib` &emsp; (*created: <kbd>LIB /MACHINE:X64 /DEF:libmpg123-0.def</kbd> in VS dev console*)
* `SFML-2.4.2-windows-vc14-64-bit/`
    * `bin/`
    * `include/`
    * `lib/`


***

<!-- Answer explaining `.lib` creation: https://stackoverflow.com/a/15226566/7517185 -->


## FFmpeg

[Homepage][FFmpeg Homepage] **\|** [GitHub][FFmpeg GitHub]

C library (and tools) for all kinds of audio handling and manipulation.

**All links for FFmpeg below are web-archived.**
Zeranoe FFmpeg Builds, which had been an essential hub for prebuilt Windows binaries, shutdown on September 18th, 2020.
Thankfully everything of importance was heavily scraped and web-archived beforehand.

> ffmpeg.zeranoe.com will close on Sep 18, 2020, and all builds will be removed.
> If you're using Zeranoe FFmpeg Builds in your product, please ensure that you have a hosting strategy in place by the 18th.
> Thank you to everyone who supported the project along the way!
> <sub>([source][Zeranoe FFmpeg Builds shutdown])</sub>

* [Builds][FFmpeg builds] **&gt;** [Windows x86][FFmpeg Windows x86 builds] **&gt;** [dev][FFmpeg Windows x86 dev builds] \| [shared][FFmpeg Windows x86 shared builds] \| [static][FFmpeg Windows x86 static builds]
    * [ffmpeg-3.2.4-win32-dev.zip][] <kbd>.lib</kbd>
    * [ffmpeg-3.2.4-win32-shared.zip][] <kbd>.dll</kbd>
    * [ffmpeg-3.2.4-win32-static.zip][] <kbd>.exe</kbd> <sub>(not required)</sub>
* [Builds][FFmpeg builds] **&gt;** [Windows x64][FFmpeg Windows x64 builds] **&gt;** [dev][FFmpeg Windows x64 dev builds] \| [shared][FFmpeg Windows x64 shared builds] \| [static][FFmpeg Windows x64 static builds]
    * [ffmpeg-3.2.4-win64-dev.zip][] <kbd>.lib</kbd>
    * [ffmpeg-3.2.4-win64-shared.zip][] <kbd>.dll</kbd>
    * [ffmpeg-3.2.4-win64-static.zip][] <kbd>.exe</kbd> <sub>(not required)</sub>



<!--

* [Builds][FFmpeg builds] **&gt;** [Windows x86][FFmpeg Windows x86 builds] **&gt;** [dev][Windows x86 dev builds] \| [shared][Windows x86 shared builds] \| [static][Windows x86 static builds]
    * [ffmpeg-3.2.4-win32-dev.zip][] <kbd>.lib</kbd>
    * [ffmpeg-3.2.4-win32-shared.zip][] <kbd>.dll</kbd>
    * [ffmpeg-3.2.4-win32-static.zip][] <kbd>.exe</kbd> <sub>(not required)</sub>
* [Builds][FFmpeg builds] **&gt;** [Windows x64][FFmpeg Windows x64 builds] **&gt;** [dev][Windows x64 dev builds] \| [shared][Windows x64 shared builds] \| [static][Windows x64 static builds]
    * [ffmpeg-3.2.4-win64-dev.zip][] <kbd>.lib</kbd>
    * [ffmpeg-3.2.4-win64-shared.zip][] <kbd>.dll</kbd>
    * [ffmpeg-3.2.4-win64-static.zip][] <kbd>.exe</kbd> <sub>(not required)</sub>





[Windows x86 dev builds][FFmpeg Windows x86 dev builds]
[Windows x86 shared builds][FFmpeg Windows x86 shared builds]
[Windows x86 static builds][FFmpeg Windows x86 static builds]

[Windows x64 dev builds][FFmpeg Windows x64 dev builds]
[Windows x64 shared builds][FFmpeg Windows x64 shared builds]
[Windows x64 static builds][FFmpeg Windows x64 static builds]

* [Windows x86 dev builds](https://web.archive.org/web/20170419055315/https://ffmpeg.zeranoe.com/builds/win32/dev/)
* [Windows x86 shared builds](https://web.archive.org/web/20170419055326/https://ffmpeg.zeranoe.com/builds/win32/shared/)
* [Windows x86 static builds](https://web.archive.org/web/20170419055331/https://ffmpeg.zeranoe.com/builds/win32/static/)
* [Windows x64 dev builds](https://web.archive.org/web/20170419055356/https://ffmpeg.zeranoe.com/builds/win64/dev/)
* [Windows x64 shared builds](https://web.archive.org/web/20170418111952/https://ffmpeg.zeranoe.com/builds/win64/shared/)
* [Windows x64 static builds](https://web.archive.org/web/20170419055406/https://ffmpeg.zeranoe.com/builds/win64/static/)
-->

## FFTW

[Homepage][FFTW Homepage] **\|** [GitHub][FFTW GitHub]

C library for computing the Discrete Fourier Transform, which is used in audio visualization.

* [Builds][FFTW Builds] **&gt;** [Windows][FFTW Windows builds]
    * [fftw-3.3.5-dll32.zip][] <kbd>.dll</kbd> <kbd>.def</kbd>
    * [fftw-3.3.5-dll64.zip][] <kbd>.dll</kbd> <kbd>.def</kbd>


## mpg123

[Homepage][mpg123 Homepage]

C library for reading MPEG-3 (`.mp3`) audio files.

<!-- Link with index.shtml (for webarchive needs): https://www.mpg123.de/index.shtml -->


* [Windows x86 builds][mpg123 Windows x86 builds] **&gt;** [v1.24.0][mpg123 Windows x86 builds, v1.24.0]
    * [mpg123-1.24.0-x86.zip][] <kbd>.dll</kbd> <kbd>.def</kbd>
    * [mpg123-1.24.0-x86-debug.zip][] <kbd>.dll</kbd> <kbd>.def</kbd>
* [Windows x64 builds][mpg123 Windows x64 builds] **&gt;** [v1.24.0][mpg123 Windows x64 builds, v1.24.0]
    * [mpg123-1.24.0-x86-64.zip][] <kbd>.dll</kbd> <kbd>.def</kbd>
    * [mpg123-1.24.0-x86-64-debug.zip][] <kbd>.dll</kbd> <kbd>.def</kbd>

<!-- unused
    * [mpg123-1.24.0-static-x86.zip][] <kbd>.exe</kbd> <sub>(unused)</sub>
    * [mpg123-1.24.0-static-x86-64.zip][] <kbd>.exe</kbd> <sub>(unused)</sub>
-->


## SFML

[Homepage][SFML Homepage] **\|** [GitHub][SFML GitHub]

C++ library for easy use of audio in games and applications. Used for sound effect and music playback.

* [Builds][SFML Builds] **&gt;** [v2.4.2][SFML Builds, v2.4.2]
    * [SFML-2.4.2-windows-vc14-32-bit.zip][] <kbd>.dll</kbd> <kbd>.lib</kbd>
    * [SFML-2.4.2-windows-vc14-64-bit.zip][] <kbd>.dll</kbd> <kbd>.lib</kbd>



<!-- MARKDOWN REFERENCES -->

<!-- FFmpeg -->

[FFmpeg Homepage]: <https://www.ffmpeg.org/>
[FFmpeg GitHub]:   <https://github.com/FFmpeg/FFmpeg>

[Zeranoe FFmpeg Builds shutdown]: <https://web.archive.org/web/20200901102620/https://ffmpeg.zeranoe.com/builds/>

[FFmpeg builds]: <https://web.archive.org/web/20170329144433/https://ffmpeg.zeranoe.com/builds/>
[FFmpeg Windows x86 builds]: <https://web.archive.org/web/20170419055315/https://ffmpeg.zeranoe.com/builds/win32/>
[FFmpeg Windows x64 builds]: <https://web.archive.org/web/20170419055356/https://ffmpeg.zeranoe.com/builds/win64/>

[FFmpeg Windows x86 dev builds]:    <https://web.archive.org/web/20170419055315/https://ffmpeg.zeranoe.com/builds/win32/dev/>
[FFmpeg Windows x86 shared builds]: <https://web.archive.org/web/20170419055326/https://ffmpeg.zeranoe.com/builds/win32/shared/>
[FFmpeg Windows x86 static builds]: <https://web.archive.org/web/20170419055331/https://ffmpeg.zeranoe.com/builds/win32/static/>
[FFmpeg Windows x64 dev builds]:    <https://web.archive.org/web/20170419055356/https://ffmpeg.zeranoe.com/builds/win64/dev/>
[FFmpeg Windows x64 shared builds]: <https://web.archive.org/web/20170418111952/https://ffmpeg.zeranoe.com/builds/win64/shared/>
[FFmpeg Windows x64 static builds]: <https://web.archive.org/web/20170419055406/https://ffmpeg.zeranoe.com/builds/win64/static/>

[ffmpeg-3.2.4-win32-dev.zip]:    <https://web.archive.org/web/20200819162211/https://ffmpeg.zeranoe.com/builds/win32/dev/ffmpeg-3.2.4-win32-dev.zip>
[ffmpeg-3.2.4-win32-shared.zip]: <https://web.archive.org/web/20200305154853/https://ffmpeg.zeranoe.com/builds/win32/shared/ffmpeg-3.2.4-win32-shared.zip>
[ffmpeg-3.2.4-win32-static.zip]: <https://web.archive.org/web/20170419055331/https://ffmpeg.zeranoe.com/builds/win32/static/ffmpeg-3.2.4-win32-static.zip>
[ffmpeg-3.2.4-win64-dev.zip]:    <https://web.archive.org/web/20200819182628/https://ffmpeg.zeranoe.com/builds/win64/dev/ffmpeg-3.2.4-win64-dev.zip>
[ffmpeg-3.2.4-win64-shared.zip]: <https://web.archive.org/web/20200414063958/https://ffmpeg.zeranoe.com/builds/win64/shared/ffmpeg-3.2.4-win64-shared.zip>
[ffmpeg-3.2.4-win64-static.zip]: <https://web.archive.org/web/20180408233456/https://ffmpeg.zeranoe.com/builds/win64/static/ffmpeg-3.2.4-win64-static.zip>


<!-- FFTW -->

[FFTW Homepage]: <http://fftw.org/>
[FFTW GitHub]:   <https://github.com/FFTW/fftw3>

[FFTW Builds]: <http://fftw.org/download.html>

[FFTW Windows builds]: <http://fftw.org/install/windows.html>

[fftw-3.3.5-dll32.zip]: <ftp://ftp.fftw.org/pub/fftw/fftw-3.3.5-dll32.zip>
[fftw-3.3.5-dll64.zip]: <ftp://ftp.fftw.org/pub/fftw/fftw-3.3.5-dll64.zip>


<!-- mpg123 -->

[mpg123 Homepage]: <https://www.mpg123.de/>

[mpg123 Windows x86 builds]: <https://www.mpg123.de/download/win32/>
[mpg123 Windows x64 builds]: <https://www.mpg123.de/download/win64/>
[mpg123 Windows x86 builds, v1.24.0]: <https://www.mpg123.de/download/win32/1.24.0/>
[mpg123 Windows x64 builds, v1.24.0]: <https://www.mpg123.de/download/win64/1.24.0/>

[mpg123-1.24.0-x86.zip]:        <https://www.mpg123.de/download/win32/1.24.0/mpg123-1.24.0-x86.zip>
[mpg123-1.24.0-x86-debug.zip]:  <https://www.mpg123.de/download/win32/1.24.0/mpg123-1.24.0-x86-debug.zip>
[mpg123-1.24.0-static-x86.zip]: <https://www.mpg123.de/download/win32/1.24.0/mpg123-1.24.0-static-x86.zip>
[mpg123-1.24.0-x86-64.zip]:        <https://www.mpg123.de/download/win64/1.24.0/mpg123-1.24.0-x86-64.zip>
[mpg123-1.24.0-x86-64-debug.zip]:  <https://www.mpg123.de/download/win64/1.24.0/mpg123-1.24.0-x86-64-debug.zip>
[mpg123-1.24.0-static-x86-64.zip]: <https://www.mpg123.de/download/win64/1.24.0/mpg123-1.24.0-static-x86-64.zip>


<!-- SFML -->

[SFML Homepage]: <https://www.sfml-dev.org/>
[SFML GitHub]:   <https://github.com/SFML/SFML>

[SFML Builds]: <https://www.sfml-dev.org/download.php>
[SFML Builds, v2.4.2]: <https://www.sfml-dev.org/download/sfml/2.4.2/>

[SFML-2.4.2-windows-vc14-32-bit.zip]: <https://www.sfml-dev.org/files/SFML-2.4.2-windows-vc14-32-bit.zip>
[SFML-2.4.2-windows-vc14-64-bit.zip]: <https://www.sfml-dev.org/files/SFML-2.4.2-windows-vc14-64-bit.zip>

