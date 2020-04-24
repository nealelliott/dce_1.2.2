# Microsoft Visual C++ generated build script - Do not modify

PROJ = DLOGOUT
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
FIRSTC = DLOGOUT.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /Zp1 /Gy /W3 /WX /Gf /Zi /AL /Od /D "_DEBUG" /D "_DOS" /D "JAPANESE" /Fc /FR /Zn /Fd"DLOGOUT.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G3 /Zp1 /Gy /W3 /Gf /AL /Ox /D "NDEBUG" /D "_DOS" /FR 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:30000 /ONERROR:NOEXE /CO /MAP
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:30000 /ONERROR:NOEXE
LIBS_D_DEXE = llibce oldnames nwcalls dfam_x 
LIBS_R_DEXE = llibce oldnames nwcalls dfam_x 
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
SBRS = DLOGOUT.SBR \
		M_DLGOUT.SBR


DLOGOUT_DEP = c:\client\login\dfam_cli.h \
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


M_DLGOUT_DEP = c:\client\login\dfam_cli.h \
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


all:	$(PROJ).EXE $(PROJ).BSC

DLOGOUT.OBJ:	DLOGOUT.C $(DLOGOUT_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c DLOGOUT.C

M_DLGOUT.OBJ:	M_DLGOUT.C $(M_DLGOUT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c M_DLGOUT.C

$(PROJ).EXE::	DLOGOUT.OBJ M_DLGOUT.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
DLOGOUT.OBJ +
M_DLGOUT.OBJ +
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
