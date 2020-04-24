#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
# All Rights Reserved
#
#
# HISTORY
# $Log: time.mk,v $
# Revision 1.2.27.2  1996/03/09  23:31:54  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:47:01  marty]
#
# Revision 1.2.27.1  1995/12/08  18:14:29  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/6  1995/05/18  20:31 UTC  pare
# 	Define -DDCE_ASSERT
# 
# 	HP revision /main/HPDCE02/pare_mothra/1  1995/05/18  20:29 UTC  pare
# 	Define DCE_ASSERT
# 
# 	HP revision /main/HPDCE02/5  1994/10/10  19:21 UTC  pare
# 	Merge bl21 changes.
# 	[1995/12/08  17:26:25  root]
# 
# Revision 1.2.24.4  1994/09/28  19:19:17  tom
# 	Remove libdbm from HP800 LIBS.
# 	[1994/09/28  19:19:02  tom]
# 
# 	HP revision /main/HPDCE02/4  1994/09/28  19:14 UTC  pare
# 	Remove -ldbm from HP800_LIBS
# 
# 	HP revision /main/HPDCE02/3  1994/09/26  20:21 UTC  pare
# 	Merge bl18 changes
# 
# Revision 1.2.24.3  1994/08/23  20:32:46  cbrooks
# 	Code Cleanup Checkpoint
# 	[1994/08/23  13:57:19  cbrooks]
# 
# 	Code Cleanup
# 	[1994/08/19  17:53:19  cbrooks]
# 
# Revision 1.2.24.2  1994/08/16  18:13:18  cbrooks
# 	CR 11494 - 64 bit changes
# 	[1994/08/09  18:49:49  cbrooks]
# 
# Revision 1.2.21.4  1993/07/29  21:52:59  tom
# 	Bug 8379 - HP800 are Big endian machines.
# 	[1993/07/29  21:52:29  tom]
# 
# Revision 1.2.21.3  1993/06/24  21:31:40  hinman
# 	[hinman@sni] - Check in SNI merged version
# 	(these files have no tainted or private code
# 	[1993/06/17  14:49:27  hinman]
# 
# Revision 1.2.21.2  1993/05/24  20:51:56  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:20:46  cjd]
# 
# Revision 1.2.19.2  1993/05/12  18:02:10  jd
# 	Initial 486 port.
# 	[1993/05/12  18:01:31  jd]
# 
# Revision 1.2.16.8  1993/03/19  20:25:17  tom
# 	Bug 7538 - Don't compile time service with -g option.
# 	[1993/03/19  20:24:51  tom]
# 
# Revision 1.2.16.7  1993/02/01  22:43:25  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  17:40:43  hinman]
# 
# Revision 1.2.16.5  1993/01/31  00:39:17  zeliff
# 	Temporarily backing out submission for defect 6991
# 	[1993/01/30  22:27:50  zeliff]
# 	Revision 1.2.17.2  1993/01/11  16:25:14  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 1.2.16.3  1992/12/30  20:43:09  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:23:11  htf]
# 
# Revision 1.2.16.2  1992/12/18  22:46:02  sommerfeld
# 	Add -D_POSIX_SOURCE for HPUX.
# 	[1992/12/18  20:07:23  sommerfeld]
# 
# Revision 1.2.11.4  1992/12/15  12:45:11  yankes
# 	This bci fixes OT 5145 and properly defines "mips" on mips platforms.
# 	[1992/12/15  12:44:49  yankes]
# 
# Revision 1.2.11.2  1992/09/29  21:28:28  devsrc
# 	[OT 5373]  SNI/SVR4 Merge.
# 	[1992/09/14  20:08:27  sekhar]
# 
# Revision 1.2.2.11  1992/07/02  15:34:46  bmw
# 	removed -DDEBUG flag from RIOS_CFLAGS
# 	[1992/07/02  15:34:02  bmw]
# 
# Revision 1.2.2.10  1992/06/24  15:55:39  grober
# 	Changes necessary to build on hpux.
# 	[1992/06/22  15:53:48  grober]
# 
# Revision 1.2.2.9  1992/06/10  19:18:25  ko
# 	Back out temporary workaround to WEXITSTATUS.  Get us back to version
# 	1.2.2.7.
# 	[1992/06/10  15:48:05  ko]
# 
# Revision 1.2.2.8  1992/06/08  20:32:40  jim
# 	OT CR 4410.  This is a temporary workaround to insure that the BSD
# 	version of the WEXITSTATUS macro is not used on AIX systems.  If
# 	there is another solution it will be implemented under OT 4121.
# 	[1992/06/08  20:32:06  jim]
# 
# Revision 1.2.2.7  1992/05/11  19:04:46  comuzzi
# 	Build user mode time service library on PMAX for testing.
# 	[1992/05/11  16:15:53  comuzzi]
# 
# Revision 1.2.2.6  1992/05/06  21:56:39  comuzzi
# 	Fix OT defect 2790, Makefile changes.
# 	[1992/05/06  21:08:32  comuzzi]
# 
# Revision 1.2.2.5  1992/04/14  15:10:23  mishkin
# 	APOLLO68K porting changes
# 	[1992/04/13  18:31:02  mishkin]
# 
# Revision 1.2.2.4  1992/02/25  19:11:30  jim
# 	Changed to remove the DATAFILES from time.mk
# 	[1992/02/25  17:31:40  jim]
# 
# Revision 1.2.2.3  1992/02/04  22:08:08  mhickey
# 	Replace with new version , add default for DTS_LIBUTC_MODE,
# 	set DTS_LIBUTC_MODE on MIPS depending on whether we are \
# 	building libdce or not.
# 	[1992/02/04  22:07:34  mhickey]
# 
# Revision 1.2.3.2  1992/02/02  04:42:08  mhickey
# 	Replaced with Latest version for DEC, with following
# 	changes:
# 	   1) default DTS_LIBUTC_MODE is user
# 	   2) DTS_LIBUTC_MODE for MIPS is kernel if LIBDCE_LIBRARY_MAKEFILE
# 	      is defined, else it is "user kernel"
# 
# Revision 1.2.2.2  1992/01/31  20:25:37  mhickey
# 	Moved the definitions of DTS_LIBUTC_MODE to Makeconf.
# 	[1992/01/31  20:24:47  mhickey]
# 
# 	HP revision /main/HPDCE02/2  1994/08/18  14:24 UTC  pare
# 	Merge changes from Hpdce02
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  18:53 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  19:42  pare
# 	merge kk and hpdce01
# 
# Revision 1.1.8.3  1993/09/17  16:34:52  pare
# 	DCE1_0_3b03 Merge
# 	[1993/09/17  16:32:21  pare]
# 
# Revision 1.1.9.3  1993/09/15  17:49:09  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.8.2  1993/07/08  20:30:01  root
# 	Initial King Kong branch
# 	[1993/07/08  20:29:35  root]
# 
# Revision 1.1.6.5  1993/04/23  12:37:28  truitt
# 	Merge DCE1_0_2B23 with GAMERA.
# 	[1993/04/22  15:29:18  truitt]
# 
# Revision 1.1.1.9  1993/04/21  20:53:49  root
# 	    Loading drop DCE1_0_2B23
# 
# Revision 1.1.6.4  1993/04/01  19:11:51  truitt
# 	OT 7538: Don't compile time services with -g option.
# 	[1993/04/01  19:11:30  truitt]
# 
# Revision 1.2.16.8  1993/03/19  20:25:17  tom
# 	Bug 7538 - Don't compile time service with -g option.
# 	[1993/03/19  20:24:51  tom]
# 
# Revision 1.1.6.3  1993/03/24  19:32:44  truitt
# 	Cleaned up the file, to make it more readable.
# 	[1993/03/24  19:32:25  truitt]
# 
# Revision 1.1.6.2  1993/03/17  19:24:39  truitt
# 	Merge Hp_dce1_0_1 with DCE1_0_2
# 	[1993/03/17  19:23:49  truitt]
# 
# Revision 1.1.1.8  1993/03/11  22:35:50  osfrcs
# 	    Loading drop DCE1_0_2B18
# 
# Revision 1.1.3.6  1993/02/26  14:40:14  truitt
# 	remove HP300_LIBS information line
# 	[1993/02/26  14:39:52  truitt]
# 
# Revision 1.2.16.7  1993/02/01  22:43:25  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  17:40:43  hinman]
# 
# Revision 1.2.16.5  1993/01/31  00:39:17  zeliff
# 	Temporarily backing out submission for defect 6991
# 	[1993/01/30  22:27:50  zeliff]
# 	Revision 1.2.17.2  1993/01/11  16:25:14  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 1.2.16.3  1992/12/30  20:43:09  htf
# 	Embedding copyright notice
# 	[1992/12/30  19:23:11  htf]
# 
# Revision 1.2.16.2  1992/12/18  22:46:02  sommerfeld
# 	Add -D_POSIX_SOURCE for HPUX.
# 	[1992/12/18  20:07:23  sommerfeld]
# 
# Revision 1.2.11.4  1992/12/15  12:45:11  yankes
# 	This bci fixes OT 5145 and properly defines "mips" on mips platforms.
# 	[1992/12/15  12:44:49  yankes]
# 
# Revision 1.2.11.2  1992/09/29  21:28:28  devsrc
# 	[OT 5373]  SNI/SVR4 Merge.
# 	[1992/09/14  20:08:27  sekhar]
# 
# Revision 1.2.2.11  1992/07/02  15:34:46  bmw
# 	removed -DDEBUG flag from RIOS_CFLAGS
# 	[1992/07/02  15:34:02  bmw]
# 
# Revision 1.1.3.5  1992/06/30  15:56:06  rlr
# 	Add code for the 300
# 	[1992/06/30  15:53:30  rlr]
# 
# Revision 1.2.2.10  1992/06/24  15:55:39  grober
# 	Changes necessary to build on hpux.
# 	[1992/06/22  15:53:48  grober]
# 
# Revision 1.1.3.4  1992/06/22  17:49:53  smythe
# 	deleted HP800_CFLAGS ?= -DDEBUG
# 	[1992/06/22  17:48:54  smythe]
# 
# Revision 1.1.3.3  1992/06/18  20:56:44  smythe
# 	deleted CC_OPT_LEVEL = -g for HP800 in DTS
# 	expect to pick this up from shared rc_file
# 	[1992/06/18  15:56:01  smythe]
# 
# 	added HP800-specific build and link flags
# 	[1992/06/16  21:28:00  smythe]
# 
# $EndLog$
#
#
#  OSF DCE Version 1.0 
#
#
#
# 10-22-91; mhickey: Modified for the new model for building libdce.a.  This
#                    means that:
#                       - This makefile allows 'make' to pick up the
#                         definitions within it if it is being read as part
#                         of the parsing of the libdce.a Makefile; ie: the
#                         variable LIBDCE_LIBRARY_MAKEFILE is defined.
#
#                       - The '.if' conditions that determine the settings of
#                         variables for directories where pieces of libdce.a
#                         are built are conditional on the value of the
#                         variable LIBDCE_CURR_INCDIR if the variable
#                         LIBDCE_LIBRARY_MAKEFILE is set.
#
#                       - ${${TARGET_MACHINE}_OFILES} settings for directories
#                         where pieces of libdce.a are built are changed to
#                         ${${TARGET_MACHINE}_<libname>_OFILES}.
#
#
#
# 10-22-91; mhickey: Added PMAX_OFILES variable settings,PMAX_LIBS variable
#                    settings, etc.  The PMAX build also has the following
#                    differences from the RIOS build, which are reflected in
#                    this makefile: time/kernel does not get built, but
#                    time /libutc/kernel does get built.
#


#
# This makefile needs to allow multiple accesses for the libdce build.  The
# multiple accesses should not be idempontent, since we want different settings
# on each access.
#
.if !defined(_TIME_MK_) || defined(LIBDCE_LIBRARY_MAKEFILE)
_TIME_MK_=

# stuff for SVR4
.if ${TARGET_MACHINE}=="SVR4"
.if defined(BLD_USR_OR_KERNEL) && ${BLD_USR_OR_KERNEL}=="kernel"
DTS_LIBUTC_MODE = kernel
.else
DTS_LIBUTC_MODE = user
.endif

CC_OPT_LEVEL    = 

.if ${MAKESUB}=="time/"
KERNEL          ?= kernel
.endif

.if ${MAKESUB}=="time/zoneinfo/"
SUBDIRS = Australia Brazil Canada Chile Mexico US SystemV
.endif

.endif			# SVR4


.if ${TARGET_MACHINE}=="VAX"
DTS_LIBUTC_MODE = kernel
.endif

.if ${TARGET_MACHINE}=="MIPS"
DTS_LIBUTC_MODE = kernel
CFLAGS          = -D__mips__ -Dmips
.endif

.if ${TARGET_OS}=="OSF1"

DTS_LIBUTC_MODE = kernel
.endif

.if ${TARGET_MACHINE}=="RIOS"
DTS_LIBUTC_MODE = kernel

.if ${MAKESUB}=="time/"
KERNEL          ?= kernel
.endif
.endif		

.if ${TARGET_MACHINE}=="AT386"
CFLAGS += ${STRICT_COMPILER_WARNINGS}
.endif 		#AT386

DTS_LIBUTC_MODE ?= user

.if ${MAKESUB}=="time/control/"
PMAX_LIBS = -lbsd
.if ${TARGET_MACHINE}=="SVR4"
INTERMED_SHLIBS		= libprs_threaded.so
libprs_threaded.so_IDIR		= /usr/lib/
libprs_threaded.so_EXPDIR	= /usr/lib/
.endif
.endif

.if ${MAKESUB}=="time/common/"
.if ${TARGET_MACHINE}=="SVR4"
INTERMED_SHLIBS		= libdpeacl.so
libdpeacl.so_IDIR		= /usr/lib/
libdpeacl.so_EXPDIR		= /usr/lib/
.endif
.endif

.if ${MAKESUB}=="time/service/"
.if ${TARGET_MACHINE}=="SVR4"
ILIST		= dtsd
.endif
.endif

AT386_ENDIAN    = LITTLE
PMAX_ENDIAN     = LITTLE
RIOS_ENDIAN     = BIG
SVR4_ENDIAN	= LITTLE
VAX_ENDIAN	= LITTLE
MIPS_ENDIAN	= LITTLE
HP800_ENDIAN	= BIG
ALPHA_ENDIAN    = LITTLE

.if ${MAKESUB} =="time/libutc/kernel/"
utcapi.o_CFLAGS = -D${${TARGET_MACHINE}_ENDIAN}_ENDIAN_ARCHITECTURE ${CFLAGS}
.endif

.if ${MAKESUB}=="time/libutc/user/"
utcapi.o_CFLAGS = -D${${TARGET_MACHINE}_ENDIAN}_ENDIAN_ARCHITECTURE ${CFLAGS}
.endif

RIOS_CFLAGS     ?= -Dunix -D_BSD -D_ALL_SOURCE
VAX_CFLAGS      ?= -Mg
HP800_CFLAGS	?= -Dunix -D_POSIX_SOURCE -DADJTIME_NOT_SUPPORTED -DDCE_ASSERT
ALPHA_CFLAGS    ?= -Dunix -D_REENTRANT -DK_DTSS_STACKSIZE=65536

RIOS_LIBS       ?= -lm -lcurses -lbsd
MIPS_LIBS       ?= -li
VAX_LIBS        ?= -li
HP800_LIBS      ?= -lM
ALPHA_LIBS      ?= -threads

.endif

