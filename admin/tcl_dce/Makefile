#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.13.3  1996/03/09  23:09:24  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:27:31  marty]
#
# Revision 1.1.13.2  1995/12/08  21:33:46  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/4  1994/12/06  15:35 UTC  truitt
# 	Turn off debug as the default build.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_01/1  1994/12/06  15:33 UTC  truitt
# 	Turn off debug as the default build.
# 
# 	HP revision /main/HPDCE02/3  1994/11/18  15:26 UTC  truitt
# 	Merge unintegrated
# 	[1995/12/08  18:05:01  root]
# 
# Revision 1.1.9.1  1994/10/21  21:38:40  mori_m
# 	CR 11259: Tcl I18N work.  Added tclWCmdAH.o, tclWCmdIL.o, tclWCmdMZ.o,
# 		  tclWParse.o, and tclWUtil.o.
# 	[1994/10/21  21:27:50  mori_m]
# 
# 	HP revision /main/HPDCE02/2  1994/10/19  14:06 UTC  mothra
# 	Build debug until fix is found
# 
# 	HP revision /main/HPDCE02/1  1994/08/25  13:55 UTC  robinson
# 	remove local rules since redundant
# 
# Revision 1.1.5.6  1994/07/18  20:16:20  mori_m
# 	CR: 11271  Export dcetclmsg.h
# 	[1994/07/18  20:10:25  mori_m]
# 
# Revision 1.1.5.5  1994/07/12  21:18:45  mori_m
# 	CR 11259:  First drop for Tcl I18N (messaging work).
# 	[1994/07/12  20:52:14  mori_m]
# 
# Revision 1.1.5.4  1994/06/09  16:05:18  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:50:02  devsrc]
# 
# Revision 1.1.5.3  1994/03/07  14:27:23  rousseau
# 	Merged with changes from 1.1.5.2
# 	[1994/03/07  14:27:19  rousseau]
# 
# 	Pulled out Tcl test stuff, and changed install path for library files (CR 9952).
# 	[1994/03/07  14:23:27  rousseau]
# 
# Revision 1.1.5.2  1994/03/02  18:10:48  melman
# 	Don't build TCL_TEST anymore
# 	[1994/03/02  18:10:37  melman]
# 
# Revision 1.1.5.1  1994/02/03  19:51:40  rousseau
# 	Changes for CR 9880.
# 	[1994/02/03  19:51:27  rousseau]
# 
# Revision 1.1.2.4  1993/12/12  15:43:14  rousseau
# 	Use tcl7.3 now.
# 	[1993/12/12  15:43:08  rousseau]
# 
# Revision 1.1.2.3  1993/11/08  20:17:25  rousseau
# 	Upgraded to Tcl7.1 .
# 	Stopped building tcltest by default.
# 	Added cov3.0 library to Vista builds.
# 	[1993/11/08  20:17:20  rousseau]
# 
# Revision 1.1.2.2  1993/10/22  11:16:36  kevins
# 	Now exports both libtcl.a and tcl.h
# 	[1993/10/22  11:16:15  kevins]
# 
# Revision 1.1.2.1  1993/10/14  14:33:15  rousseau
# 	Initial Version
# 	[1993/10/14  14:23:56  rousseau]
# 
# $EndLog$
#


TCL_VERSION		 = 7.3
ADMIN_DIR		 = ..
TCL_ORIG_DIR		 = ${ADMIN_DIR}/tcl${TCL_VERSION}
TCL_ORIG_PATH		 = ${TCL_ORIG_DIR}/
TCL_DIR			 = ${ADMIN_DIR}/tcl_dce
TCL_PATH		 = ${TCL_DIR}/
TCL_LIB_DIR		 = ${TCL_ORIG_DIR}/library

.include "${TARGET_MACHINE}/machdep.mk"

VPATH		 = ${TCL_ORIG_DIR}:${TCL_TEST_DIR}:${TCL_LIB_DIR}
INCFLAGS	 = -I. -I${TCL_ORIG_DIR}
PROGRAMS	 = tclsh tcltest
LIBRARIES	 = libtcl.a
INCLUDES         = tcl.h dcetclmsg.h
libtcl.a_EXPDIR	 = /usr/lib/
tcl.h_EXPDIR     = /usr/include/dce/
dcetclmsg.h_EXPDIR     = /usr/include/dce/
SAMSFILES	 = tcl.sams
SAMIDL_INCLUDES	 = dcetclmsg.h
SAMS_TARGETS	 = dcetclmsg.h dcetclsvc.c dcetclmsg.c dcetcl.cat

TCL_AUX		 = init.tcl \
		   parray.tcl \
		   tclIndex

DATAFILES	 = ${TCL_AUX}
ILIST		 = ${TCL_AUX} dcetcl.cat
IDIR		 = /tcl/
dcetcl.cat_IDIR	 = /nls/msg/en_US.ASCII/

# MACHINE_CFLAGS is set in machdep.mk
CFLAGS		 = ${MACHINE_CFLAGS}

# To avoid dragging in -ldce
# LIBS		 = 

HFILES		 = patchlevel.h tcl.h tclInt.h tclRegexp.h tclUnix.h

libtcl.a_OFILES	 = panic.o \
		   regexp.o \
		   tclAsync.o \
		   tclBasic.o \
		   tclCkalloc.o \
		   tclCmdAH.o \
		   tclCmdIL.o \
		   tclCmdMZ.o \
		   tclEnv.o \
		   tclExpr.o \
		   tclGet.o \
		   tclGlob.o \
		   tclHash.o \
		   tclHistory.o \
		   tclLink.o \
		   tclMain.o \
		   tclMtherr.o \
		   tclParse.o \
		   tclProc.o \
		   tclUnixAZ.o \
		   tclUnixStr.o \
		   tclUnixUtil.o \
		   tclUtil.o \
		   tclVar.o \
		   tclWCmdAH.o \
		   tclWCmdIL.o \
		   tclWCmdMZ.o \
		   tclWParse.o \
		   tclWUtil.o \
		   dcetclmsg.o

tcltest_OFILES	 = tclTest.o
tclsh_OFILES	 = tclAppInit.o

LIBS		 = -ltcl ${${TARGET_MACHINE}_LIBS} -ldce


# Global rules
.include <${RULES_MK}>
