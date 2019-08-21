# Microsoft Developer Studio Generated NMAKE File, Based on ChatServer.dsp
!IF "$(CFG)" == ""
CFG=ChatServer - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ChatServer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ChatServer - Win32 Release" && "$(CFG)" != "ChatServer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ChatServer.mak" CFG="ChatServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ChatServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ChatServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ChatServer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ChatServer.exe"


CLEAN :
	-@erase "$(INTDIR)\ChatServer.obj"
	-@erase "$(INTDIR)\ChatServer.pch"
	-@erase "$(INTDIR)\ChatServer.res"
	-@erase "$(INTDIR)\ChatServerDlg.obj"
	-@erase "$(INTDIR)\ListenSocket.obj"
	-@erase "$(INTDIR)\ServerSocket.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ChatServer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ChatServer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\ChatServer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ChatServer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\ChatServer.pdb" /machine:I386 /out:"$(OUTDIR)\ChatServer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ChatServer.obj" \
	"$(INTDIR)\ChatServerDlg.obj" \
	"$(INTDIR)\ListenSocket.obj" \
	"$(INTDIR)\ServerSocket.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ChatServer.res"

"$(OUTDIR)\ChatServer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ChatServer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ChatServer.exe"


CLEAN :
	-@erase "$(INTDIR)\ChatServer.obj"
	-@erase "$(INTDIR)\ChatServer.pch"
	-@erase "$(INTDIR)\ChatServer.res"
	-@erase "$(INTDIR)\ChatServerDlg.obj"
	-@erase "$(INTDIR)\ListenSocket.obj"
	-@erase "$(INTDIR)\ServerSocket.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ChatServer.exe"
	-@erase "$(OUTDIR)\ChatServer.ilk"
	-@erase "$(OUTDIR)\ChatServer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ChatServer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\ChatServer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ChatServer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ChatServer.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ChatServer.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ChatServer.obj" \
	"$(INTDIR)\ChatServerDlg.obj" \
	"$(INTDIR)\ListenSocket.obj" \
	"$(INTDIR)\ServerSocket.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ChatServer.res"

"$(OUTDIR)\ChatServer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ChatServer.dep")
!INCLUDE "ChatServer.dep"
!ELSE 
!MESSAGE Warning: cannot find "ChatServer.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ChatServer - Win32 Release" || "$(CFG)" == "ChatServer - Win32 Debug"
SOURCE=.\ChatServer.cpp

"$(INTDIR)\ChatServer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ChatServer.pch"


SOURCE=.\ChatServer.rc

"$(INTDIR)\ChatServer.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\ChatServerDlg.cpp

"$(INTDIR)\ChatServerDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ChatServer.pch"


SOURCE=.\ListenSocket.cpp

"$(INTDIR)\ListenSocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ChatServer.pch"


SOURCE=.\ServerSocket.cpp

"$(INTDIR)\ServerSocket.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ChatServer.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ChatServer - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ChatServer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ChatServer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ChatServer - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ChatServer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ChatServer.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

