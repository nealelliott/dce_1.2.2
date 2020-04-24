# Microsoft Visual C++ generated build script - Do not modify

PROJ = CLIFIL_L
DEBUG = 0
PROGTYPE = 4
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\CLIENT\FILER\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = MSG_LST.C   
FIRSTCPP =             
RC = rc
CFLAGS_D_LIB = /nologo /Zr /G2 /Zp1 /Gy /W3 /Gf /Z7 /AL /Od /D "_DEBUG" /D "_DOS" /D "JAPANESE" /FR /GA 
CFLAGS_R_LIB = /nologo /Gs /G3 /Zp1 /Gy /W3 /Gf /AL /Ox /D "NDEBUG" /D "_DOS" /FR 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_LIB)
LFLAGS = 
LIBS = 
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_LIB)
LFLAGS = 
LIBS = 
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = CHKPATH.SBR \
		CHKVOL.SBR \
		MSG_LST.SBR \
		TRST_CHR.SBR \
		PARM_PRU.SBR


CHKPATH_DEP = c:\gateway\gfiler.h \
	c:\gateway\err.h \
	c:\client\filer\prot.h


CHKVOL_DEP = c:\sdkcd7\legacy\client\include\nwcaldef.h \
	c:\sdkcd7\legacy\client\include\nwdpath.h \
	c:\sdkcd7\legacy\client\include\nwcalls.h \
	c:\sdkcd7\legacy\client\include\nwalias.h \
	c:\sdkcd7\legacy\client\include\nwafp.h \
	c:\sdkcd7\legacy\client\include\nwbindry.h \
	c:\sdkcd7\legacy\client\include\nwconnec.h \
	c:\sdkcd7\legacy\client\include\nwdel.h \
	c:\sdkcd7\legacy\client\include\nwdentry.h \
	c:\sdkcd7\legacy\client\include\nwdirect.h \
	c:\sdkcd7\legacy\client\include\nwea.h \
	c:\sdkcd7\legacy\client\include\nwnamspc.h \
	c:\sdkcd7\legacy\client\include\nwerror.h \
	c:\sdkcd7\legacy\client\include\nwfile.h \
	c:\sdkcd7\legacy\client\include\nwmisc.h \
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
	c:\gateway\err.h \
	c:\gateway\gfiler.h \
	c:\client\filer\prot.h


MSG_LST_DEP = c:\client\filer\cli_filr.h \
	c:\sdkcd7\legacy\client\include\nwcaldef.h \
	c:\sdkcd7\legacy\client\include\nwdpath.h \
	c:\sdkcd7\legacy\client\include\nwcalls.h \
	c:\sdkcd7\legacy\client\include\nwalias.h \
	c:\sdkcd7\legacy\client\include\nwafp.h \
	c:\sdkcd7\legacy\client\include\nwbindry.h \
	c:\sdkcd7\legacy\client\include\nwconnec.h \
	c:\sdkcd7\legacy\client\include\nwdel.h \
	c:\sdkcd7\legacy\client\include\nwdentry.h \
	c:\sdkcd7\legacy\client\include\nwdirect.h \
	c:\sdkcd7\legacy\client\include\nwea.h \
	c:\sdkcd7\legacy\client\include\nwnamspc.h \
	c:\sdkcd7\legacy\client\include\nwerror.h \
	c:\sdkcd7\legacy\client\include\nwfile.h \
	c:\sdkcd7\legacy\client\include\nwmisc.h \
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
	c:\gateway\gfiler.h \
	c:\gateway\err.h \
	c:\client\filer\prot.h


TRST_CHR_DEP = c:\client\filer\cli_filr.h \
	c:\sdkcd7\legacy\client\include\nwcaldef.h \
	c:\sdkcd7\legacy\client\include\nwdpath.h \
	c:\sdkcd7\legacy\client\include\nwcalls.h \
	c:\sdkcd7\legacy\client\include\nwalias.h \
	c:\sdkcd7\legacy\client\include\nwafp.h \
	c:\sdkcd7\legacy\client\include\nwbindry.h \
	c:\sdkcd7\legacy\client\include\nwconnec.h \
	c:\sdkcd7\legacy\client\include\nwdel.h \
	c:\sdkcd7\legacy\client\include\nwdentry.h \
	c:\sdkcd7\legacy\client\include\nwdirect.h \
	c:\sdkcd7\legacy\client\include\nwea.h \
	c:\sdkcd7\legacy\client\include\nwnamspc.h \
	c:\sdkcd7\legacy\client\include\nwerror.h \
	c:\sdkcd7\legacy\client\include\nwfile.h \
	c:\sdkcd7\legacy\client\include\nwmisc.h \
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
	c:\gateway\gfiler.h \
	c:\gateway\err.h \
	c:\client\filer\prot.h


PARM_PRU_DEP = c:\client\login\dfam_cli.h \
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


all:	$(PROJ).LIB $(PROJ).BSC

CHKPATH.OBJ:	CHKPATH.C $(CHKPATH_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c CHKPATH.C

CHKVOL.OBJ:	CHKVOL.C $(CHKVOL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c CHKVOL.C

MSG_LST.OBJ:	MSG_LST.C $(MSG_LST_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c MSG_LST.C

TRST_CHR.OBJ:	TRST_CHR.C $(TRST_CHR_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c TRST_CHR.C

PARM_PRU.OBJ:	..\LOGIN\PARM_PRU.C $(PARM_PRU_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\LOGIN\PARM_PRU.C

$(PROJ).LIB::	CHKPATH.OBJ CHKVOL.OBJ MSG_LST.OBJ TRST_CHR.OBJ PARM_PRU.OBJ $(OBJS_EXT)
	echo >NUL @<<$(PROJ).CRF
$@ /PAGESIZE:64
y
+CHKPATH.OBJ &
+CHKVOL.OBJ &
+MSG_LST.OBJ &
+TRST_CHR.OBJ &
+PARM_PRU.OBJ &
;
<<
	if exist $@ del $@
	lib @$(PROJ).CRF

$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
