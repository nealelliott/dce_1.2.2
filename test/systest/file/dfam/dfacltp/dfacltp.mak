# Microsoft Visual C++ generated build script - Do not modify

PROJ = DFACLTP
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\DFACLTP\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = DFACLTP.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /W3 /Zi /AM /Od /D "_DEBUG" /D "_DOS" /FR /Fd"DFACLTP.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /Zp1 /W3 /AL /Ox /D "NDEBUG" /D "_DOS" /FR
LFLAGS_D_DEXE = /NOLOGO /ONERROR:NOEXE /NOI /CO /STACK:5120
LFLAGS_R_DEXE = /NOLOGO /ONERROR:NOEXE /NOI /STACK:5120
LIBS_D_DEXE = oldnames mlibce
LIBS_R_DEXE = oldnames llibce
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = ..\SDKCD_4\CLIENT\DOS\MSC\NWCALLS.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = DFACLTP.SBR \
		DFACLCOM.SBR


NWCALLS_DEP = 

DFACLTP_DEP = c:\dfacltp\dfacltp.h \
	c:\sdkcd_4\client\include\nwcalls.h \
	c:\sdkcd_4\client\include\nwcaldef.h \
	c:\sdkcd_4\client\include\nwalias.h \
	c:\sdkcd_4\client\include\nwafp.h \
	c:\sdkcd_4\client\include\nwbindry.h \
	c:\sdkcd_4\client\include\nwconnec.h \
	c:\sdkcd_4\client\include\nwdel.h \
	c:\sdkcd_4\client\include\nwdentry.h \
	c:\sdkcd_4\client\include\nwdirect.h \
	c:\sdkcd_4\client\include\nwdpath.h \
	c:\sdkcd_4\client\include\nwea.h \
	c:\sdkcd_4\client\include\nwnamspc.h \
	c:\sdkcd_4\client\include\nwerror.h \
	c:\sdkcd_4\client\include\nwfile.h \
	c:\sdkcd_4\client\include\nwmisc.h \
	c:\sdkcd_4\client\include\nwmsg.h \
	c:\sdkcd_4\client\include\nwprint.h \
	c:\sdkcd_4\client\include\nwqms.h \
	c:\sdkcd_4\client\include\nwserver.h \
	c:\sdkcd_4\client\include\nwsync.h \
	c:\sdkcd_4\client\include\nwtts.h \
	c:\sdkcd_4\client\include\nwvol.h \
	c:\sdkcd_4\client\include\nwacct.h \
	c:\sdkcd_4\client\include\nwfse.h \
	c:\sdkcd_4\client\include\nwmigrat.h \
	c:\sdkcd_4\client\include\nwncpext.h \
	c:\dfacltp\dfaclmsg.h


DFACLCOM_DEP = c:\dfacltp\dfacltp.h \
	c:\sdkcd_4\client\include\nwcalls.h \
	c:\sdkcd_4\client\include\nwcaldef.h \
	c:\sdkcd_4\client\include\nwalias.h \
	c:\sdkcd_4\client\include\nwafp.h \
	c:\sdkcd_4\client\include\nwbindry.h \
	c:\sdkcd_4\client\include\nwconnec.h \
	c:\sdkcd_4\client\include\nwdel.h \
	c:\sdkcd_4\client\include\nwdentry.h \
	c:\sdkcd_4\client\include\nwdirect.h \
	c:\sdkcd_4\client\include\nwdpath.h \
	c:\sdkcd_4\client\include\nwea.h \
	c:\sdkcd_4\client\include\nwnamspc.h \
	c:\sdkcd_4\client\include\nwerror.h \
	c:\sdkcd_4\client\include\nwfile.h \
	c:\sdkcd_4\client\include\nwmisc.h \
	c:\sdkcd_4\client\include\nwmsg.h \
	c:\sdkcd_4\client\include\nwprint.h \
	c:\sdkcd_4\client\include\nwqms.h \
	c:\sdkcd_4\client\include\nwserver.h \
	c:\sdkcd_4\client\include\nwsync.h \
	c:\sdkcd_4\client\include\nwtts.h \
	c:\sdkcd_4\client\include\nwvol.h \
	c:\sdkcd_4\client\include\nwacct.h \
	c:\sdkcd_4\client\include\nwfse.h \
	c:\sdkcd_4\client\include\nwmigrat.h \
	c:\sdkcd_4\client\include\nwncpext.h \
	c:\dfacltp\dfaclmsg.h


all:	$(PROJ).EXE $(PROJ).BSC

DFACLTP.OBJ:	DFACLTP.C $(DFACLTP_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c DFACLTP.C

DFACLCOM.OBJ:	DFACLCOM.C $(DFACLCOM_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DFACLCOM.C

$(PROJ).EXE::	DFACLTP.OBJ DFACLCOM.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
DFACLTP.OBJ +
DFACLCOM.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
c:\sdkcd_4\client\dos\msc\+
..\SDKCD_4\CLIENT\DOS\MSC\NWCALLS.LIB+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
