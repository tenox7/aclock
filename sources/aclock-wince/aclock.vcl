<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: aclock - Win32 (WCE MIPS16) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"MIPS16Dbg/aclock.res" /d UNDER_CE=400 /d _WIN32_WCE=400 /d "MIPS16SUPPORT" /d "UNICODE" /d "_UNICODE" /d "DEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "MIPS" /d "_MIPS_" /d "R4000" /d "MIPSII" /d "MIPS16" /r "C:\wince\aclock\aclock.rc"" 
Creating temporary file "C:\DOCUME~1\admin1\LOCALS~1\Temp\RSPA7.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "R4000" /D "MIPSII" /D "MIPS16" /D UNDER_CE=400 /D _WIN32_WCE=400 /D "WCE_PLATFORM_STANDARDSDK" /D "UNICODE" /D "_UNICODE" /D "_MIPS16_" /D "MIPS16SUPPORT" /Fp"MIPS16Dbg/aclock.pch" /Yu"stdafx.h" /Fo"MIPS16Dbg/" /Fd"MIPS16Dbg/" /QMmips16 /MC /c 
"C:\wince\aclock\aclock.cpp"
]
Creating command line "clmips.exe @C:\DOCUME~1\admin1\LOCALS~1\Temp\RSPA7.tmp" 
Creating temporary file "C:\DOCUME~1\admin1\LOCALS~1\Temp\RSPA8.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "R4000" /D "MIPSII" /D "MIPS16" /D UNDER_CE=400 /D _WIN32_WCE=400 /D "WCE_PLATFORM_STANDARDSDK" /D "UNICODE" /D "_UNICODE" /D "_MIPS16_" /D "MIPS16SUPPORT" /Fp"MIPS16Dbg/aclock.pch" /Yc"stdafx.h" /Fo"MIPS16Dbg/" /Fd"MIPS16Dbg/" /QMmips16 /MC /c 
"C:\wince\aclock\StdAfx.cpp"
]
Creating command line "clmips.exe @C:\DOCUME~1\admin1\LOCALS~1\Temp\RSPA8.tmp" 
Creating temporary file "C:\DOCUME~1\admin1\LOCALS~1\Temp\RSPA9.tmp" with contents
[
commctrl.lib coredll.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:yes /pdb:"MIPS16Dbg/aclock.pdb" /debug /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"MIPS16Dbg/aclock.exe" /subsystem:windowsce,4.00 /MACHINE:MIPS16 /ALIGN:4096 
.\MIPS16Dbg\aclock.obj
.\MIPS16Dbg\StdAfx.obj
.\MIPS16Dbg\aclock.res
]
Creating command line "link.exe @C:\DOCUME~1\admin1\LOCALS~1\Temp\RSPA9.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
aclock.cpp
C:\wince\aclock\aclock.cpp(123) : warning C4244: '=' : conversion from 'double' to 'long', possible loss of data
C:\wince\aclock\aclock.cpp(124) : warning C4244: '=' : conversion from 'double' to 'long', possible loss of data
C:\wince\aclock\aclock.cpp(152) : warning C4244: 'initializing' : conversion from 'double' to 'float', possible loss of data
C:\wince\aclock\aclock.cpp(155) : warning C4244: '=' : conversion from 'double' to 'long', possible loss of data
C:\wince\aclock\aclock.cpp(156) : warning C4244: '=' : conversion from 'double' to 'long', possible loss of data
Linking...




<h3>Results</h3>
aclock.exe - 0 error(s), 5 warning(s)
</pre>
</body>
</html>
