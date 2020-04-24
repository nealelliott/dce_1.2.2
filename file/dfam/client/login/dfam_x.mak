# Microsoft Visual C++ generated build script - Do not modify

PROJ = DFAM_X
DEBUG = 0
PROGTYPE = 4
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
FIRSTC = DCHK.C      
FIRSTCPP =             
RC = rc
CFLAGS_D_LIB = /nologo /G2 /Zp1 /Gy /W3 /WX /Gf /Zi /AL /Od /D "_DEBUG" /D "JAPANESE" /D "_DOS" /Fc /FR /GA /Fd"DFAM_X.PDB"
CFLAGS_R_LIB = /nologo /Gs /G3 /Zp1 /Gy /W3 /Gf /AL /Ox /D "NDEBUG" /D "_DOS" /FR /GA 
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
SBRS = DCHK.SBR \
		DLIST.SBR \
		DMSG.SBR \
		PARM_PRU.SBR


DCHK_DEP = c:\client\login\dfam_cli.h \
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


DLIST_DEP = c:\client\login\dfam_cli.h \
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


DMSG_DEP = c:\client\login\dfam_cli.h \
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
	c:\sdkcd7\legacy\client\include\diag.h \
	c:\client\login\cli_msg.h


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

DCHK.OBJ:	DCHK.C $(DCHK_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c DCHK.C

DLIST.OBJ:	DLIST.C $(DLIST_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DLIST.C

DMSG.OBJ:	DMSG.C $(DMSG_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DMSG.C

PARM_PRU.OBJ:	PARM_PRU.C $(PARM_PRU_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PARM_PRU.C

$(PROJ).LIB::	DCHK.OBJ DLIST.OBJ DMSG.OBJ PARM_PRU.OBJ $(OBJS_EXT)
	echo >NUL @<<$(PROJ).CRF
$@ /PAGESIZE:64
y
+DCHK.OBJ &
+DLIST.OBJ &
+DMSG.OBJ &
+PARM_PRU.OBJ &
;
<<
	if exist $@ del $@
	lib @$(PROJ).CRF

$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
