# Microsoft Visual C++ generated build script - Do not modify

PROJ = DSETPASS
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\CLIENT\LOGIN\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = DSETPASS.C  
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /Zp1 /W3 /WX /Zi /AL /Od /D "_DEBUG" /D "_DOS" /D "OSFW" /Fc /FR /Zn /Fd"DSETPASS.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G3 /Zp1 /W3 /Gf /AL /Ox /D "NDEBUG" /D "_DOS" /D "OSFW" /FR
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:35000 /ONERROR:NOEXE /CO /MAP
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:35000 /ONERROR:NOEXE
LIBS_D_DEXE = lafxcwd oldnames llibce nwcalls lnwipxsp dfam_x dump_l 
LIBS_R_DEXE = lafxcwd llibce nwcalls lnwipxsp dfam_x oldnames 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
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
SBRS = DSETPASS.SBR \
		M_SETPS.SBR \
		PS_INPUT.SBR \
		GENCRYPT.SBR


DSETPASS_DEP = c:\client\login\dfam_cli.h \
	c:\sdkcd7\legacy\client\include\nwcaldef.h \
	c:\sdkcd7\legacy\client\include\nwncpext.h \
	c:\sdkcd7\legacy\client\include\nwmisc.h \
	c:\sdkcd7\legacy\client\include\nwcalls.h \
	c:\sdkcd7\legacy\client\include\nwalias.h \
	c:\sdkcd7\legacy\client\include\nwafp.h \
	c:\sdkcd7\legacy\client\include\nwbindry.h \
	c:\sdkcd7\legacy\client\include\nwconnec.h \
	c:\sdkcd7\legacy\client\include\nwdel.h \
	c:\sdkcd7\legacy\client\include\nwdentry.h \
	c:\sdkcd7\legacy\client\include\nwdirect.h \
	c:\sdkcd7\legacy\client\include\nwdpath.h \
	c:\sdkcd7\legacy\client\include\nwea.h \
	c:\sdkcd7\legacy\client\include\nwnamspc.h \
	c:\sdkcd7\legacy\client\include\nwerror.h \
	c:\sdkcd7\legacy\client\include\nwfile.h \
	c:\sdkcd7\legacy\client\include\nwmsg.h \
	c:\sdkcd7\legacy\client\include\nwprint.h \
	c:\sdkcd7\legacy\client\include\nwqms.h \
	c:\sdkcd7\legacy\client\include\nwserver.h \
	c:\sdkcd7\legacy\client\include\nwsync.h \
	c:\sdkcd7\legacy\client\include\nwtts.h \
	c:\sdkcd7\legacy\client\include\nwvol.h \
	c:\sdkcd7\legacy\client\include\nwacct.h \
	c:\sdkcd7\legacy\client\include\nwfse.h \
	c:\sdkcd7\legacy\client\include\nwmigrat.h \
	c:\sdkcd7\legacy\client\include\nwipxspx.h \
	c:\sdkcd7\legacy\client\include\nxtw.h \
	c:\sdkcd7\legacy\client\include\nwsap.h \
	c:\sdkcd7\legacy\client\include\nwdiag.h \
	c:\sdkcd7\legacy\client\include\nxtd.h \
	c:\sdkcd7\legacy\client\include\sap.h \
	c:\sdkcd7\legacy\client\include\diag.h


M_SETPS_DEP = c:\client\login\dfam_cli.h \
	c:\sdkcd7\legacy\client\include\nwcaldef.h \
	c:\sdkcd7\legacy\client\include\nwncpext.h \
	c:\sdkcd7\legacy\client\include\nwmisc.h \
	c:\sdkcd7\legacy\client\include\nwcalls.h \
	c:\sdkcd7\legacy\client\include\nwalias.h \
	c:\sdkcd7\legacy\client\include\nwafp.h \
	c:\sdkcd7\legacy\client\include\nwbindry.h \
	c:\sdkcd7\legacy\client\include\nwconnec.h \
	c:\sdkcd7\legacy\client\include\nwdel.h \
	c:\sdkcd7\legacy\client\include\nwdentry.h \
	c:\sdkcd7\legacy\client\include\nwdirect.h \
	c:\sdkcd7\legacy\client\include\nwdpath.h \
	c:\sdkcd7\legacy\client\include\nwea.h \
	c:\sdkcd7\legacy\client\include\nwnamspc.h \
	c:\sdkcd7\legacy\client\include\nwerror.h \
	c:\sdkcd7\legacy\client\include\nwfile.h \
	c:\sdkcd7\legacy\client\include\nwmsg.h \
	c:\sdkcd7\legacy\client\include\nwprint.h \
	c:\sdkcd7\legacy\client\include\nwqms.h \
	c:\sdkcd7\legacy\client\include\nwserver.h \
	c:\sdkcd7\legacy\client\include\nwsync.h \
	c:\sdkcd7\legacy\client\include\nwtts.h \
	c:\sdkcd7\legacy\client\include\nwvol.h \
	c:\sdkcd7\legacy\client\include\nwacct.h \
	c:\sdkcd7\legacy\client\include\nwfse.h \
	c:\sdkcd7\legacy\client\include\nwmigrat.h \
	c:\sdkcd7\legacy\client\include\nwipxspx.h \
	c:\sdkcd7\legacy\client\include\nxtw.h \
	c:\sdkcd7\legacy\client\include\nwsap.h \
	c:\sdkcd7\legacy\client\include\nwdiag.h \
	c:\sdkcd7\legacy\client\include\nxtd.h \
	c:\sdkcd7\legacy\client\include\sap.h \
	c:\sdkcd7\legacy\client\include\diag.h


PS_INPUT_DEP = c:\client\login\dfam_cli.h \
	c:\sdkcd7\legacy\client\include\nwcaldef.h \
	c:\sdkcd7\legacy\client\include\nwncpext.h \
	c:\sdkcd7\legacy\client\include\nwmisc.h \
	c:\sdkcd7\legacy\client\include\nwcalls.h \
	c:\sdkcd7\legacy\client\include\nwalias.h \
	c:\sdkcd7\legacy\client\include\nwafp.h \
	c:\sdkcd7\legacy\client\include\nwbindry.h \
	c:\sdkcd7\legacy\client\include\nwconnec.h \
	c:\sdkcd7\legacy\client\include\nwdel.h \
	c:\sdkcd7\legacy\client\include\nwdentry.h \
	c:\sdkcd7\legacy\client\include\nwdirect.h \
	c:\sdkcd7\legacy\client\include\nwdpath.h \
	c:\sdkcd7\legacy\client\include\nwea.h \
	c:\sdkcd7\legacy\client\include\nwnamspc.h \
	c:\sdkcd7\legacy\client\include\nwerror.h \
	c:\sdkcd7\legacy\client\include\nwfile.h \
	c:\sdkcd7\legacy\client\include\nwmsg.h \
	c:\sdkcd7\legacy\client\include\nwprint.h \
	c:\sdkcd7\legacy\client\include\nwqms.h \
	c:\sdkcd7\legacy\client\include\nwserver.h \
	c:\sdkcd7\legacy\client\include\nwsync.h \
	c:\sdkcd7\legacy\client\include\nwtts.h \
	c:\sdkcd7\legacy\client\include\nwvol.h \
	c:\sdkcd7\legacy\client\include\nwacct.h \
	c:\sdkcd7\legacy\client\include\nwfse.h \
	c:\sdkcd7\legacy\client\include\nwmigrat.h \
	c:\sdkcd7\legacy\client\include\nwipxspx.h \
	c:\sdkcd7\legacy\client\include\nxtw.h \
	c:\sdkcd7\legacy\client\include\nwsap.h \
	c:\sdkcd7\legacy\client\include\nwdiag.h \
	c:\sdkcd7\legacy\client\include\nxtd.h \
	c:\sdkcd7\legacy\client\include\sap.h \
	c:\sdkcd7\legacy\client\include\diag.h


GENCRYPT_DEP = c:\gateway\encrypt.h


all:	$(PROJ).EXE $(PROJ).BSC

DSETPASS.OBJ:	DSETPASS.C $(DSETPASS_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c DSETPASS.C

M_SETPS.OBJ:	M_SETPS.C $(M_SETPS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c M_SETPS.C

PS_INPUT.OBJ:	PS_INPUT.C $(PS_INPUT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PS_INPUT.C

GENCRYPT.OBJ:	..\..\GATEWAY\GENCRYPT.C $(GENCRYPT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\..\GATEWAY\GENCRYPT.C

$(PROJ).EXE::	DSETPASS.OBJ M_SETPS.OBJ PS_INPUT.OBJ GENCRYPT.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
DSETPASS.OBJ +
M_SETPS.OBJ +
PS_INPUT.OBJ +
GENCRYPT.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
c:\sdkcd7\legacy\client\dos\msc\+
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
