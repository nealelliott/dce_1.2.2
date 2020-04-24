#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
# HISTORY
# $Log: cds.mk,v $
# Revision 1.2.31.2  1996/03/09  23:11:27  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:50  marty]
#
# Revision 1.2.31.1  1995/12/08  00:11:05  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:53:47  root]
# 
# Revision 1.2.28.8  1994/09/22  18:35:27  mccann
# 	move defines to cds.mk
# 	[1994/09/22  18:34:22  mccann]
# 
# Revision 1.2.28.7  1994/08/24  20:09:52  sommerfeld
# 	Fix build of parser_aid
# 	[1994/08/24  18:13:24  sommerfeld]
# 
# Revision 1.2.28.6  1994/08/16  18:27:40  jd
# 	drop from janet
# 	[1994/08/13  16:04:54  jd]
# 
# Revision 1.2.28.5  1994/08/03  18:59:05  mccann
# 	fix assert
# 	[1994/07/21  20:14:52  mccann]
# 
# Revision 1.2.28.4  1994/06/30  19:15:14  mccann
# 	sams cleanup drop2
# 	[1994/06/30  18:49:51  mccann]
# 
# 	sams cleanup drop2
# 
# 	sams cleanup drop 2
# 
# Revision 1.2.28.3  1994/05/20  22:03:09  sommerfeld
# 	fix IDLFLAGS.
# 	[1994/05/20  21:30:39  sommerfeld]
# 
# 	fix IDLFLAGS.
# 
# Revision 1.2.28.2  1994/04/29  15:49:41  peckham
# 	Add -lcdsutil to all cds components.
# 	[1994/04/29  13:56:08  peckham]
# 
# Revision 1.2.28.1  1994/04/05  20:44:04  ohara
# 	svr4 changes, temp drop of named-pipes
# 	[1994/04/05  20:43:21  ohara]
# 
# Revision 1.2.26.2  1993/10/20  20:29:06  peckham
# 	CR#4320:Fix ":" to go to configurable junction point.
# 	Added DFS conditional for this code.
# 	[1993/10/20  20:26:14  peckham]
# 
# Revision 1.2.26.1  1993/10/07  21:26:01  pwang
# 	Added HP800_INCFLAGS and HP800_LIBFLAGS [OT#9045]
# 	[1993/10/07  21:25:37  pwang]
# 
# Revision 1.2.19.9  1993/08/11  15:12:09  jd
# 	Made the HP800 dependent parser_aid_LIBS definition better. The first try
# 	created a problem on the RIOS.
# 	[1993/08/11  14:46:08  jd]
# 
# Revision 1.2.19.8  1993/07/30  14:56:29  blurie
# 	#8286 ( acl_edit hangs because the cdsadv never responds).
# 
# 		For SVR4 platforms, put '-D SNI_SVR4_NAMED_PIPES' on the
# 		compile line so that named pipes are used for communication
# 		between the advertiser/clerk processes and CDS clients.  This is
# 		instead of UNIX domain sockets.
# 	[1993/07/30  13:38:53  blurie]
# 
# Revision 1.2.19.7  1993/07/26  18:53:24  jd
# 	Pulled out Rich's change to include Motif1.2 headers and libraries. The
# 	change isn't needed if motif has been properly installed. Did tell
# 	build where to find X11's xt library on HP platform.
# 	[1993/07/26  18:52:23  jd]
# 
# Revision 1.2.19.6  1993/07/20  20:23:11  jd
# 	Moved parser_aid_LIBS definition for hp800 from control/Makefile to here.
# 	[1993/07/20  20:21:47  jd]
# 
# Revision 1.2.19.5  1993/07/16  16:45:37  zeliff
# 	Add Motif1.2 & X11R5 for cdsbrowser on HP800
# 	[1993/07/16  15:06:45  zeliff]
# 
# Revision 1.2.19.4  1993/06/24  19:46:19  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:35:48  hinman]
# 
# Revision 1.2.19.3  1993/06/10  19:13:37  sommerfeld
# 	Remove spurious -I of /usr/include/X11R4/X11 for HP-UX for cdsbrowser.
# 	[1993/06/08  17:45:54  sommerfeld]
# 
# Revision 1.2.19.2  1993/05/24  20:40:32  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:11:56  cjd]
# 
# Revision 1.2.16.3  1993/05/14  02:57:25  jd
# 	Add -lmach to AT386 LIBS.
# 	[1993/05/14  02:57:07  jd]
# 
# Revision 1.2.7.6  1993/02/03  22:31:56  blurie
# 	Fix for OT #7106.  For SVR4, make sure that parser_aid is built
# 	in the "setup" pass.
# 
# 	Fix for OT #7105.  Remove '-D' definition in SVR4_CFLAGS which
# 	turned off chmod() on sockets.
# 
# 	Fix for OT #7111.  For SVR4, set up variable NO_CDSBROWSER so upper
# 	level makefiles can control building cdsbrowser binary.
# 	[1993/02/03  20:52:36  blurie]
# 
# Revision 1.2.7.5  1993/02/01  20:26:22  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  15:57:59  hinman]
# 
# Revision 1.2.7.4  1992/12/30  12:37:46  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:21:24  zeliff]
# 	Revision 1.2.11.2  1993/01/11  16:09:02  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 9.5.1.23  92/12/23  09:33:45  tikku
# 	[hinman for tikku] - Conditionalize on TARGET_OS_PLATFORM, not MACHINE
# 
# Revision 9.5.12.2  92/12/23  09:32:35  tikku
# 	[hinman for tikku] - Conditionalize on TARGET_OS_PLATFORM, not MACHINE
# 
# Revision 9.5.1.22  92/12/22  13:47:19  raj
# 	[raj] Change rules for parser_aid to use ${EX{ORTBASE} instead of hard-coded path
# 	[92/12/22  13:46:22  raj]
# 
# Revision 9.5.1.21  92/12/17  13:11:11  blurie
# 	Reentrant library changes:
# 
# 		Fix up link line for parser_aid.
# 	[92/12/03  15:49:42  blurie]
# 
# 	[bein] - for PYRMIPS, use of -lelf_r requires -lelf.
# 	[92/11/22  08:01:13  bein]
# 
# Revision 9.5.1.20  92/12/08  16:53:07  blurie
# 	Move change for pyrmips libraries for cdsbrowser from the
# 	cdsbrowser makefile to here.
# 	[92/12/08  16:41:58  blurie]
# 
# Revision 9.5.1.19  92/11/22  08:38:45  bein
# 	[bein] - Need -lelf for PYRMIPS if using -lelf_r.
# 
# Revision 9.5.1.18  92/11/17  16:21:25  blurie
# 	Change SVR4_CFLAGS to not include -DDEBUG and -DSNI_TEST by default.
# 	[92/11/17  16:20:42  blurie]
# 
# Revision 9.5.1.17  92/10/20  10:54:13  blurie
# 	Merge up to OSF REV 1.2.5.5.
# 	[92/10/20  10:29:57  blurie]
# 
# Revision 9.5.1.15  92/10/09  17:11:34  chen
# 	removed lines that caused libcdscache.so to be built shared -- rcc
# 
# Revision 9.5.1.14  92/10/08  11:43:01  tikku
# 	Added INTERMED_SHLIBS = libcdscache.so
# 
# Revision 9.5.1.13  92/10/06  16:12:53  chen
# 	changed ODE_CROSS_MACHINE test to test for CROSS_MACHINE instead
# 	(which will be defined by mipstosinix.mk) and added changes for svr4v3.
# 	we want to not build the control program or browser on a mipstosinix build,
# 	and not build the control program only on an svr4v3 build.  these changes
# 	should go back to OSF -- rcc
# 
# Revision 9.5.1.12  92/10/05  13:31:08  hinman
# 	[hinman] - Whoops! Try this version instead!
# 
# Revision 9.5.1.11  92/10/05  13:15:23  hinman
# 	[hinman] - Short term fix for parser_aid problem
# 
# Revision 9.5.1.10  92/10/01  13:19:37  tikku
# 	Removed prs_LIBS and msg_LIBS because when building these
# 	two -ldce cannot be used.
# 
# Revision 9.5.1.9  92/09/30  14:22:26  root
# 	Acceptance of OSF rev 1.2.7.2
# 
# Revision 9.5.1.19  92/11/22  08:38:45  bein
# 	[bein] - Need -lelf for PYRMIPS if using -lelf_r.
# 
# Revision 9.5.1.18  92/11/17  16:21:25  blurie
# 	Change SVR4_CFLAGS to not include -DDEBUG and -DSNI_TEST by default.
# 	[92/11/17  16:20:42  blurie]
# 
# Revision 9.5.1.17  92/10/20  10:54:13  blurie
# 	Merge up to OSF REV 1.2.5.5.
# 	[92/10/20  10:29:57  blurie]
# 
# Revision 9.12.1.2  92/10/19  14:44:38  hinman
# 	Auto checkin of OSF rev 1.2.7.3
# 
# Revision 1.2.7.3  1992/10/13  18:07:54  marty
# 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
# 	[1992/10/13  17:05:39  marty]
# 
# Revision 1.2.8.2  1992/09/30  16:10:26  garyf
# 	Merge up to latest dce1.0.2 (post SVR4).
# 	[1992/09/30  16:05:38  garyf]
# 	Revision 9.5.5.2  92/10/08  11:42:02  tikku
# 	Added INTERMED_SHLIBS = libcdscache.so
# 
# Revision 1.2.7.2  1992/09/29  19:07:50  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:09:34  weisman]
# 	Revision 1.2.4.2  1992/09/29  20:02:04  garyf
# 	OSC1.1 updates
# 
# Revision 9.5.4.2  92/10/06  16:11:59  chen
# 	changed ODE_CROSS_MACHINE test to test for CROSS_MACHINE instead
# 	(which will be defined by mipstosinix.mk) and added changes for svr4v3.
# 	we want to not build the control program or browser on a mipstosinix build,
# 	and not build the control program only on an svr4v3 build.  these changes
# 	should go back to OSF -- rcc
# 
# Revision 9.5.4.2  92/10/05  13:29:21  hinman
# 	[hinman] - Correct typo in previous bsubmit
# 
# Revision 9.5.3.2  92/10/01  13:18:17  tikku
# 	Removed prs_LIBS and msg_LIBS because when building these
# 	two -ldce cannot be used.
# 
# Revision 9.12.1.2  92/09/30  09:35:25  hinman
# 	Auto checkin of OSF rev 1.2.7.2
# 
# Revision 1.2.7.2  1992/09/29  19:07:50  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:09:34  weisman]
# 
# Revision 1.2.2.9  1992/06/10  21:21:12  grober
# 	Added HP800 compile and link flags.
# 	[1992/06/09  21:00:42  grober]
# 
# Revision 1.2.2.8  1992/04/29  13:23:53  mfox
# 	Shared library problem on RIOS and PMAX - rpc_ss_allocate &
# 	rpc_ss_free bound to wrong functions.
# 	[1992/04/29  13:17:50  mfox]
# 
# Revision 1.2.2.7  1992/04/23  20:00:22  mfox
# 	TEMPORARY: AIX ANSI C cannot compile cds_dacl.c until V3.2 is available
# 	[1992/04/23  19:59:52  mfox]
# 
# Revision 1.2.2.6  1992/04/21  20:16:55  mfox
# 	Use K&R C to build cdsbrowser on RIOS platforms -- ANSI C generates errors
# 	[1992/04/21  20:16:20  mfox]
# 
# Revision 1.2.2.5  1992/04/14  20:49:41  mfox
# 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
# 	[1992/04/14  19:39:01  mfox]
# 
# Revision 1.2.2.4  1992/04/14  15:26:31  mishkin
# 	Merge up from DCE1_0_1
# 	[1992/04/14  15:25:05  mishkin]
# 
# 	      APOLLO68K porting changes
# 	[1992/04/13  18:32:08  mishkin]
# 
# Revision 1.2.2.3  1992/03/22  21:06:47  weisman
# 		Big merge with post-1.0 development at Digital.
# 	[1992/03/22  20:02:44  weisman]
# 
# Revision 1.2.2.2  1992/01/29  20:51:57  eperkins
# 	Added PMAX_OFILES
# 	[1992/01/29  20:50:44  eperkins]
# 
# Revision 1.2  1992/01/19  22:12:53  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# 
#  OSF DCE Version 1.0 
# 
#  COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991. ALL RIGHTS RESERVED.
#
#  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
#  ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
#  INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
#  COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
#  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
#  TRANSFERRED.
#
#  THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
#  AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
#  CORPORATION.
#
#  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
#  SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
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
# This makefile needs to allow multiple accesses for the libdce build.  The
# multiple accesses should not be idempontent, since we want different settings
# on each access.
#
.if !defined(_CDS_MK_) || defined (LIBDCE_LIBRARY_MAKEFILE)
_CDS_MK_=

BLD_SEC   = 1 

IDLINCFLAGS	=${INCFLAGS}
IDLFLAGS	?= -v -space_opt -keep c_source ${INCDIRS}

AT386_LIB_DCE ?= -lcdsutil -ldce
AT386_LIB_COM ?= -lm -lbsd -lld
AT386_CFLAGS  ?=  -Dunix -DDCE_ASSERT

RIOS_CFLAGS ?= -Dunix  -D_ALL_SOURCE -DDCE_ASSERT

HP800_CFLAGS ?= -Dunix -DDCE_ASSERT 
HP_LIB_DCE   ?= -lcdsutil -ldce
HP_LIB_COM   ?= -ldbm -lM

#If this definition changes, you must change the one farther down
###SVR4_CFLAGS  ?= -Dunix  \
SVR4_CFLAGS  ?= -Dunix -DSYSV -DDCE_ASSERT
SVR4_LIB_DCE    ?= -lcdsutil -ldce

ALPHA_LIB_DCE	?=  -lcdsutil -ldce -lpthreads -lmach -lc_r 
.ifdef DEC_LMF
ALPHA_LIB_COM	?= -llmf
.else
ALPHA_LIB_COM	?=
.endif

ALPHA_CFLAGS	?= -Dalpha -Dunix -D_REENTRANT -DDCE_ASSERT
ALPHA_IDLFLAGS  ?= -cc_cmd "${ANSI_C:C} -c ${${TARGET_MACHINE}_CFLAGS}"

.if ${TARGET_MACHINE}=="ALPHA"
CARGS		?= -std1 
.endif


# Bug - make doesn't expand ${TARGET_MACHINE}_CFLAGS correctly in our version
#       of ODE

.if defined(BLD_SEC)
RIOS_CFLAGS	+= -DDCE_SEC
AT386_CFLAGS	+= -DDCE_SEC
SVR4_CFLAGS     += -DDCE_SEC
IDLFLAGS	+= -DDCE_SEC
HP800_CFLAGS    += -DDCE_SEC
ALPHA_CFLAGS    += -DDCE_SEC
.endif

# -DDNS_MALLOC = for malloc instrumentation
# -DDTHREAD_WRAPPERS for thread wrappers
# -DCACHE_PARANOID for lots of checking in cache code
# -DDIAG for unsuppored diagnotic program interface in server
RIOS_CFLAGS	+= -DDTHREAD_WRAPPERS -DDNS_MALLOC
AT386_CFLAGS	+= -DDTHREAD_WRAPPERS -DDNS_MALLOC
SVR4_CFLAGS     += -DDTHREAD_WRAPPERS -DDNS_MALLOC
IDLFLAGS	+= 
HP800_CFLAGS    += -DDTHREAD_WRAPPERS -DDNS_MALLOC
ALPHA_CFLAGS    += 

.if !defined(NO_DFS)
RIOS_CFLAGS   += -DDNS_DFS
AT386_CFLAGS  += -DDNS_DFS
SVR4_CFLAGS   += -DDNS_DFS
IDLFLAGS      += -DDNS_DFS
HP800_CFLAGS  += -DDNS_DFS
ALPHA_CFLAGS  += -DDNS_DFS
.endif

.if ${MAKESUB}=="directory/cds/"
.if (${TARGET_MACHINE}=="SVR4")
.if (defined(CROSS_MACHINE))
.if (${CROSS_MACHINE} == "mipstosinix")
# Turn off cross building of control and cdsbrowser
SUBDIRS         = includes stubs library cache child server \
                  adver gda
EXPINC_SUBDIRS         = includes stubs library cache child server \
                  adver gda control
EXPLIB_SUBDIRS         = includes stubs library cache child server \
                  adver gda control
.endif
.endif
.if (defined(NO_CDSBROWSER))
# Turn off building of cdsbrowser only
SUBDIRS         = includes stubs library cache child server \
                  adver gda control
EXPINC_SUBDIRS         = ${SUBDIRS}
EXPLIB_SUBDIRS         = ${SUBDIRS}
.endif
.endif
.endif

.if ${MAKESUB}=="directory/cds/adver/"
RIOS_LIBS	+= -lcdsutil -ldce -lbsd -lm -lcfg -lodm
AT386_LIBS	?= -lmach ${AT386_LIB_DCE} ${AT386_LIB_COM}
HP800_LIBS      ?= ${HP_LIB_DCE} ${HP_LIB_COM} 
ALPHA_LIBS      ?= ${ALPHA_LIB_DCE} ${ALPHA_LIB_COM} 
.if defined(SNI_DCOSX_NOREENTRANT)
SVR4_LIBS       ?= ${SVR4_LIB_DCE}   -lelf
.else
SVR4_LIBS       ?= ${SVR4_LIB_DCE}   -lelf_r
.endif
.if ${TARGET_MACHINE}=="SVR4"
.if ${TARGET_OS_PLATFORM}=="PYRMIPS"
SVR4_LIBS += -lelf
.endif
.endif
.endif

.if ${MAKESUB}=="directory/cds/cache/"
RIOS_LIBS	+= -lcdsutil -ldce -lbsd -lm -lcfg -lodm
AT386_LIBS	?= -lmach ${AT386_LIB_DCE} ${AT386_LIB_COM}
HP800_LIBS      ?= ${HP_LIB_DCE} ${HP_LIB_COM}
ALPHA_LIBS      ?= ${ALPHA_LIB_DCE} ${ALPHA_LIB_COM} 
.if defined(SNI_DCOSX_NOREENTRANT)
SVR4_LIBS       ?= ${SVR4_LIB_DCE} -lelf
.else
SVR4_LIBS       ?= ${SVR4_LIB_DCE} -lelf_r
.endif
.if ${TARGET_MACHINE}=="SVR4"
.if ${TARGET_OS_PLATFORM}=="PYRMIPS"
SVR4_LIBS += -lelf
.endif
.endif
.endif

.if ${MAKESUB}=="directory/cds/cdsbrowser/"
RIOS_LIBS	?= -lcdsutil -ldce -lm -lbsd -lMrm -lXm -lXt -lX11
.if ${TARGET_MACHINE} == "RIOS"
  CCTYPE ?= traditional
.endif
AT386_LIBS      ?= ${AT386_LIB_DCE} -lMrm -lXm -lXt -lX11 ${AT386_LIB_COM}
SVR4_LIBS	?= ${SVR4_LIB_DCE} -lMrm -lXm -lXt -lX11 
HP800_INCFLAGS	?= -I${EXTERNAL_BASE}/usr/include/Motif1.2 \
		   -I${EXTERNAL_BASE}/usr/include/X11R5
HP800_LIBFLAGS  ?= -L${EXTERNAL_BASE}/usr/lib/Motif1.2 \
		   -L${EXTERNAL_BASE}/usr/lib/X11R5
HP800_LIBS      ?= ${HP_LIB_DCE} -lMrm -lXm -lXt -lX11 ${HP_LIB_COM}
ALPHA_LIBS      ?= -lMrm -lDXm -lXm -lXt -lX11 ${ALPHA_LIB_DCE} ${ALPHA_LIB_COM}
.if (${TARGET_MACHINE}=="SVR4") && (${TARGET_OS_PLATFORM}=="PYRMIPS")
cdsbrowser_LIBS += -lgen
# To not mix up different versions of the X11 include/libs it is
# not a good idea to assume /usr/include/X11 is a sane place to
# get your headers. Use X11_INC_OPTIONS to set it correctly.
# This might be -I${ROOT}/usr/src/head/X11 for example.

INCFLAGS        = -I../includes ${${TARGET_MACHINE}_INCFLAGS} ${X11_INC_OPTIONS} 
.endif
.endif

.if ${MAKESUB}=="directory/cds/child/"
RIOS_LDFLAGS += -bnodelcsect
RIOS_LIBS	+= -lcdsutil -ldce -lbsd -lm -lcfg -lodm
AT386_LIBS	?= -lmach ${AT386_LIB_DCE} ${AT386_LIB_COM}
HP800_LIBS      ?= ${HP_LIB_DCE} ${HP_LIB_COM}
ALPHA_LIBS      ?= ${ALPHA_LIB_DCE} ${ALPHA_LIB_COM} 
.if defined(SNI_DCOSX_NOREENTRANT)
SVR4_LIBS       ?= ${SVR4_LIB_DCE} -lelf
.else
SVR4_LIBS       ?= ${SVR4_LIB_DCE} -lelf_r
.endif
.if ${TARGET_MACHINE}=="SVR4"
.if ${TARGET_OS_PLATFORM}=="PYRMIPS"
SVR4_LIBS += -lelf
.endif
.endif
.endif

.if ${MAKESUB}=="directory/cds/control/"
RIOS_LIBS	+= -lcdscache -lcdsutil -ldce -lcurses -lm -lbsd -lcfg -lodm
AT386_LIBS	?= -lmach ${AT386_LIB_DCE} -lcurses ${AT386_LIB_COM}
HP800_LIBS      ?= ${HP_LIB_DCE} ${HP_LIB_COM}
ALPHA_LIBS      ?= ${ALPHA_LIB_DCE} ${ALPHA_LIB_COM} 
.if ${TARGET_MACHINE}=="HP800"
parser_aid_LIBS	=
.else
parser_aid_LIBS = ${${TARGET_MACHINE}_LIBS}
.endif
SVR4_LIBS    += ${SVR4_LIB_DCE} -ltermcap -lelf_r
.if  ${TARGET_MACHINE}=="SVR4"
parser_aid_LIBS += -lucb_r
.endif

# Use of -lelf_r requires use of -lelf also.
.if ${TARGET_MACHINE}=="SVR4"
.if ${TARGET_OS_PLATFORM}=="PYRMIPS"
SVR4_LIBS += -lelf
.endif
.endif

.if ${TARGET_MACHINE}=="SVR4"
#These rules allow these programs to be built in a "setup" environment.
#We have to take care of the cases:
#	build all
#	build cdscp
#
#In the first case, 'MAKEFILE_PASS' is set.  In the second, it is not.
#
#The "setup" environment includes special compile and link time
#flags.
#
#One drawback is that 'make' gets invoked for each '.o' and program.
#A small price to pay for having parser_aid not depend on 
#reentrant libraries.

.ifdef MAKEFILE_PASS
.if ${MAKEFILE_PASS} == "SETUP"
SVR4_CFLAGS  = -Dunix -DSYSV 
SVR4_LIBS = 
.else 
prs msg parser_aid ${parser_aid_OFILES} ${msg_OFILES} ${prs_OFILES}:
	${MAKE} MAKEFILE_PASS=SETUP ${.TARGET}
.endif	# SETUP
.else
prs msg parser_aid ${parser_aid_OFILES} ${msg_OFILES} ${prs_OFILES}:
	${MAKE} MAKEFILE_PASS=SETUP ${.TARGET}
.endif	# MAKEFILE_PASS
.endif	# SVR4

.endif

.if ${MAKESUB}=="directory/cds/gda/"
RIOS_LIBS	+= -lcdsutil -ldce -lbsd -lm
AT386_LIBS	?= ${AT386_LIB_DCE} ${AT386_LIB_COM}
SVR4_LIBS       ?= ${SVR4_LIB_DCE}
RIOS_CFLAGS	+= -DBIT_ZERO_ON_LEFT
HP800_LIBS      ?= ${HP_LIB_DCE} ${HP_LIB_COM}
ALPHA_LIBS      ?= ${ALPHA_LIB_DCE} ${ALPHA_LIB_COM} 
.endif

.if ${MAKESUB}=="directory/cds/includes/"
RIOS_NLSPATH	?= /nls/msg/en_US.ASCII/
AT386_NLSPATH	?= /nls/msg/en_US.ASCII/
HP800_NLSPATH   ?= /nls/msg/en_US.ASCII/
SVR4_NLSPATH    ?= /nls/msg/en_US.ASCII/
ALPHA_NLSPATH   ?= /usr/lib/nls/msg/en_US.ISO8859-1
.endif

#
# The condition on this directory needs to allow parsing when 'make' is
# building the library, and when the libdce Makefile is including this
# Makefile.
#
.if ${MAKESUB}=="directory/cds/library/" || (defined(LIBDCE_LIBRARY_MAKEFILE) &&${LIBDCE_CURR_INCDIR} == "directory/cds/library/")
RIOS_LIBS	+= -lcdsutil -ldce -lm -lbsd
AT386_LIBS	?= ${AT386_LIB_DCE} ${AT386_LIB_COM}
HP800_LIBS      ?= ${HP_LIB_DCE} ${HP_LIB_COM}
ALPHA_LIBS      ?= ${ALPHA_LIB_DCE} ${ALPHA_LIB_COM} 
.endif

.if ${MAKESUB}=="directory/cds/server/"
.if ${TARGET_MACHINE}=="RIOS"
#
# Temporary hack until AIX 3.2 is used to build cds
#
cds_dacl.o_CCTYPE=traditional
.endif
RIOS_LIBS	+= -lcdsutil -ldce -lm -lbsd
AT386_LIBS	?= ${AT386_LIB_DCE} ${AT386_LIB_COM}
HP800_LIBS      ?= ${HP_LIB_DCE} ${HP_LIB_COM}
SVR4_LIBS       ?= ${SVR4_LIB_DCE}
ALPHA_LIBS      ?= ${ALPHA_LIB_DCE} ${ALPHA_LIB_COM} 
.endif

.if ${MAKESUB}=="directory/cds/stubs/"
RIOS_CFLAGS	+= -D_BSD -Dunix
.endif

.endif





