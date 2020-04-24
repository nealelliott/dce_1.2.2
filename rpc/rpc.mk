#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# (c) Copyright 1991, 1992 
#     Siemens-Nixdorf Information Systems, Burlington, MA, USA
#     All Rights Reserved
#
# HISTORY         
# $Log: rpc.mk,v $
# Revision 1.2.39.2  1996/03/09  23:26:17  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:54  marty]
#
# Revision 1.2.39.1  1995/12/08  00:15:39  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/8  1995/08/21  17:45 UTC  sommerfeld
# 	Compile inner-loop crypto routines +O3.
# 	[1995/07/30  19:18 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts15483/1]
# 
# 	HP revision /main/HPDCE02/7  1995/01/30  22:40 UTC  sommerfeld
# 	link csrc static.
# 	[1995/01/27  03:53 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
# 
# 	HP revision /main/HPDCE02/6  1994/11/23  18:01 UTC  sommerfeld
# 	Go to file/${TARGET_MACHINE}/machdep.mk for kernel CFLAGS.
# 	[1994/11/07  20:38 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_1/1]
# 
# 	HP revision /main/HPDCE02/5  1994/10/04  14:44 UTC  tatsu_s
# 	Merged Mothra upto Hpdce02_03.
# 	[1995/12/07  23:56:36  root]
# 
# Revision 1.2.35.7  1994/09/16  16:03:49  rsalz
# 	Stop building rpcd (OT CR 11488).
# 	Remove flags for xxx_15.o files.
# 	[1994/09/15  19:39:17  rsalz]
# 
# Revision 1.2.35.6  1994/09/15  16:04:55  annie
# 	CR12121- - enable capabilities to set -g
# 	[1994/09/15  14:35:40  annie]
# 
# 	HP revision /main/HPDCE02/3  1994/08/30  23:07 UTC  sommerfeld
# 	foo
# 	[1994/07/18  19:45 UTC  sommerfeld  /main/dce1.1_clearmakefix/1]
# 
# 	HP revision /main/HPDCE02/2  1994/08/03  16:31 UTC  tatsu_s
# 	Merged mothra up to dce 1.1.
# 
# 	HP revision /main/HPDCE01/5  1994/05/26  13:29 UTC  kissel
# 	Add KERNEL_DEBUG_FLAGS in case you want to build with extra kernel flags
# 	turned on (usually for debugging).
# 
# 	HP revision /main/HPDCE01/4  1994/05/02  20:59 UTC  brezak
# 	Remove the KERNEL_CFLAGS that pertain to options. The headers we
# 	use already are unifdefed to match the correct ones.
# 	Unify 700 and 800 build flags for 10.0
# 
# 	HP revision /main/HPDCE01/3  1994/03/31  22:09 UTC  mort
# 	update HP800_KERNEL_CFLAGS from IC1 /stand/build/config.mk
# 
# 	HP revision /main/HPDCE01/mort_10.0-port/1  1994/03/31  20:52 UTC  mort
# 	update HP800_KERNEL_CFLAGS from IC1 /stand/build/config.mk
# 
# 	HP revision /main/HPDCE01/2  1994/02/15  22:14  kissel
# 	Fix the kernel CFLAGS for HPUX.  They should match src/file/HP800/machdep.mk.
# 
# Revision 1.1.11.10  1993/12/14  12:19:15  robinson
# 	Make parallel "file/HP800/machdep.mk".
# 	New UX 10.0 defines when building 10.0.
# 	[1993/12/10  21:52:18  robinson]
# 
# Revision 1.1.11.5  1993/08/27  13:28:22  tmm
# 	UX 10 merge
# 	[1993/08/27  12:38:59  tmm]
# 
# Revision 1.1.11.9  1993/12/07  18:53:25  hopkins
# 	Change usage of ux_release to expect
# 	more precision (1000/900 instead of
# 	current (10/9).
# 	[1993/11/03  19:43:31  hopkins]
# 
# Revision 1.1.11.8  1993/10/14  19:15:25  robinson
# 	Change hp800r defines to match standard kernel
# 	[1993/10/14  19:05:35  robinson]
# 
# Revision 1.1.11.7  1993/10/05  19:58:47  tatsu_s
# 	Merged up to DCE1_0_3b931005.
# 
# Revision 1.1.11.4  1993/08/10  19:38:04  kissel
# 	Eliminate the OSDEBUG define from kernel compilation.
# 	[1993/08/10  19:37:03  kissel]
# 
# 	1. Change kernel include specification to use EV
# 	   rather than hard-coded path through "external".
# 	2. Add kernel defines for hp800r
# 	[1993/07/16  16:48:00  robinson]
# 
# Revision 1.1.14.4  1993/10/05  17:58:57  tatsu_s
# 	    Loading drop DCE1_0_3b931005
# 	Revision 1.1.11.6  1993/09/20  18:00:31  tatsu_s
# 	KK merged upto DCE1_0_3b03.
# 
# Revision 1.2.35.5  1994/07/13  22:33:20  devsrc
# 	merged with DCE1.1
# 	[1994/07/11  15:18:24  devsrc]
# 
# 	merged with bl-10
# 	[1994/06/29  12:18:08  devsrc]
# 
# 	Removed OSDEBUG from HP kernel flags
# 	[1994/06/27  18:59:33  mckeen]
# 
# Revision 1.2.35.4  1994/06/29  20:55:21  ohara
# 	update SVR4_libnck_OFILES
# 	[1994/06/27  19:50:37  ohara]
# 
# Revision 1.2.35.3  1994/06/23  17:40:00  tom
# 	Remove uuidsys references.
# 	[1994/06/22  20:40:43  tom]
# 
# Revision 1.2.35.2  1994/02/22  18:38:53  ohara
# 	change some SVR4 flags
# 	[1994/02/18  16:04:54  ohara]
# 
# Revision 1.2.35.1  1994/02/09  20:56:07  mori_m
# 	CR 9912:  Added machine dependent module (cs_s_conv.o)
# 	[1994/02/09  20:55:33  mori_m]
# 
# Revision 1.2.33.3  1993/09/29  15:09:47  tom
# 	Bug 8365 - Move ipnaf_hpux.c to HP800/ipnaf_sys.c.
# 	Also move uuid_hpux.c to HP800/uuidsys.c.
# 	[1993/09/29  15:09:20  tom]
# 
# Revision 1.2.33.2  1993/09/22  20:34:07  tom
# 	Use OSF1_KERNEL_INCDIRS to set krpc include dirs for DFS.
# 	[1993/09/22  20:27:14  tom]
# 
# Revision 1.2.25.3  1993/07/19  19:38:17  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:29:39  zeliff]
# 
# Revision 1.2.30.3  1993/07/16  21:15:45  kissel
# 	Change kernel header file handling for HPUX.
# 	[1993/06/21  14:46:41  kissel]
# 
# Revision 1.2.25.2  1993/05/24  20:47:01  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:14:59  cjd]
# 
# Revision 1.2.26.2  1993/06/04  21:14:23  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  22:36:20  kissel]
# 
# Revision 1.2.21.2  1993/05/12  17:34:23  jd
# 	Initial 486 work.
# 	[1993/05/12  17:21:17  jd]
# 
# Revision 1.1.8.2  1993/03/24  16:39:09  tatsu_s
# 	Added CFLAGS options for compiling NCS1.5.1 compatibility
# 	code modules in ./src/rpc/runtime and compiling nidl_to_idl
# 	in ./src/rpc/idl/nidl_to_idl .
# 	[1992/10/30  15:00:48  shu]
# 
# 	Tweaks to KERNELCFLAGS and other things for KRPC on HPUX and HPUX9.0.
# 	[1992/10/20  21:02:07  toml]
# 
# 	06/16/92 tmm  For kernel-resident HP800 runtime libraries, add kernel
# 	defines and include path.
# 	[1992/06/18  18:38:21  tmm]
# 
# Revision 1.1.3.7  1992/12/02  20:52:08  toml
# 	Added -DHP_DFS for HPUX KRPC builds.
# 	[1992/12/02  20:51:36  toml]
# 
# Revision 1.2.14.8  1993/02/01  20:35:32  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  16:13:42  hinman]
# 
# Revision 1.2.14.7  1993/01/03  22:37:07  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:53:54  bbelch]
# 	Revision 1.2.19.2  1993/01/11  16:11:01  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 1.2.14.6  1992/12/23  19:40:46  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:54:42  zeliff]
# 
# Revision 1.2.14.5  1992/12/10  22:28:15  marty
# 	Upgrade DFS to OSF/1 1.1.1.  [OT defect 5236]
# 	[1992/12/10  22:16:50  marty]
# 
# 	updated PMAX_KERNEL_CFLAGS for OSF 1.1.1
# 	[1992/10/22  14:57:21  marty]
# 
# Revision 1.2.14.4  1992/10/07  20:08:53  markar
# 	  OT CR 3932 fix: ifdef out subcomponent defs from test tree builds
# 	[1992/10/07  18:18:45  markar]
# 
# Revision 1.2.14.3  1992/10/02  19:29:02  markar
# 	  OT CR 2786 fix: Relocated information from here to runtime/Makefile
# 	[1992/09/22  18:38:24  markar]
# 
# Revision 1.2.14.2  1992/09/29  21:12:58  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:35:44  weisman]
# 
# Revision 1.2.3.6  1992/06/26  06:24:00  jim
# 	Changes to compile on AIX 3.2.
# 	[1992/06/26  06:23:14  jim]
# 
# Revision 1.2.3.5  1992/05/26  21:41:14  garyf
# 	add compilation options for PMAX
# 	[1992/05/26  20:56:47  garyf]
# 
# Revision 1.2.3.4  1992/05/20  18:31:19  garyf
# 	change HP_PA to use __OSF__ instead of OSF
# 	[1992/05/20  18:24:52  garyf]
# 
# Revision 1.2.3.3  1992/05/11  18:05:07  mhickey
# 	Moved conditionals that switch off USE_SHARED_LIBRARIES for kernel
# 	pieces of the build from osf.dce.mk to the proper
# 	component .mk files.
# 	Defect 3332
# 	[1992/05/11  18:03:58  mhickey]
# 
# Revision 1.2.3.2  1992/05/01  17:36:51  rsalz
# 	 31-mar-92 nm Fix HP_PA_KERNEL_CFLAGS.
# 	 27-mar-92 nm APOLLO68K fixes for idl.
# 	 20-mar-92 nm Misc reorg'ing and cleaning up, esp. of lib stuff.
# 	 16-mar-92 pf Add MIPS flags; add variable for lib list
# 	 16-mar-92 nm Add HPUX stuff.
# 	 10-mar-92 pf	DCE 1.0.1 merge
# 	              Restructure so that only 1 copy is kept of shared
# 	              platform-related files
# 	[1992/05/01  17:34:31  rsalz]
# 
# Revision 1.2  1992/01/19  22:13:38  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 
#
##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. & 
##      Digital Equipment Corporation, Maynard, Mass.
##  NAME:
##
##      makefile.rpcmk (src/rpc/rpc.mk)
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile fragment for RPC components.  This file gets .include'd
##  (indirectly) by all RPC component makefiles.  Note that
##  src/test/rpc/rpc.mk explicitly includes this file as well.
##
##

.if !defined(_RPC_MK_)  || defined (LIBDCE_LIBRARY_MAKEFILE) 
_RPC_MK_=

# added capabalities for security component to compile with
# the -g option, put this at the top so that we DO NOT miss
# any embedded copyrights
.ifdef RPC_OPT_LEVEL 
# Only the HP platform's loader recognizes the -g switch
.if ${TARGET_MACHINE} == "HP800"
OPT_LEVEL=${RPC_OPT_LEVEL}
.else
CC_OPT_LEVEL=${RPC_OPT_LEVEL}
.endif
.endif

#
# Definitions common to all RPC subcomponents.
#

RIOS_CFLAGS_COMMON      ?= -Dunix -D_ALL_SOURCE -DNO_SIOCGIFADDR
ULTRIX_CFLAGS_COMMON    ?= -Dunix -Dultrix
VAX_CFLAGS_COMMON       ?= -Mg ${ULTRIX_CFLAGS_COMMON}
MIPS_CFLAGS_COMMON      ?= ${ULTRIX_CFLAGS_COMMON} -Dmips
HP_PA_CFLAGS_COMMON     ?= -Dunix -D__OSF__ -D_REENTRANT -D_THREAD_SAFE -D__hp_osf -q no_use_dff
HPUX_CFLAGS_COMMON      ?= 
HP800_CFLAGS_COMMON 	?= ${HPUX_CFLAGS_COMMON}
HP300_CFLAGS_COMMON 	?= ${HPUX_CFLAGS_COMMON}
APOLLO68K_CFLAGS_COMMON ?=
PMAX_CFLAGS_COMMON      ?= -D_BSD
PMAX_INCFLAGS           ?= -I.
AT386_CFLAGS_COMMON     ?= -D_BSD
AT386_INCFLAGS          ?= -I.
SVR4_CFLAGS_COMMON	?=

RIOS_YFLAGS_COMMON      ?= -l

ULTRIX_LIBS_COMMON      ?= -li
VAX_LIBS_COMMON         ?= ${ULTRIX_LIBS_COMMON}
MIPS_LIBS_COMMON        ?= ${ULTRIX_LIBS_COMMON}
RIOS_LIBS_COMMON        ?= -lbsd
OSF1_LIBS_COMMON	?= -lm -lbsd
PMAX_LIBS_COMMON        ?= ${OSF1_LIBS_COMMON}
AT386_LIBS_COMMON	?= ${OSF1_LIBS_COMMON}
APOLLO68K_LIBS_COMMON   ?= -lintl
HP_PA_LIBS_COMMON       ?= -L/usr/lib -lpthreads -lmach -lc_r -lc
SVR4_LIBS_COMMON        ?= 

LIBS_DCE                ?= -ldce

HP_PA_KERNEL_CFLAGS	?= -W0,-ndb -D__OSF__ -DCOMPAT_43 -DMACH -DDBG -D_BSD -Dunix \
		 	   -W0,-natural -W0,-gb,0,-no_fp,-ncompress,-bss -W0,-opt,2 \
			   ${KERNEL_INCFLAGS}
PMAX_KERNEL_CFLAGS	?= -DCOMPAT_43 -DMACH -D__OSF__ -DNIDL_PROTOTYPES -DIDL_PROTOTYPES \
			   -DPMAX -G 0 -fomit-frame-pointer -Wtrigraphs -Wcomment \
			   -pic-none -nostdinc -pedantic -Wpointer-arith
AT386_KERNEL_CFLAGS	?= -DCOMPAT_43 -DMACH -D__OSF__ -DNIDL_PROTOTYPES -DIDL_PROTOTYPES \
			   -DAT386 -Wtrigraphs -Wcomment \
			   -pic-none -nostdinc -pedantic -Wpointer-arith
PMAX_SFLAGS		?= -g0
MMAX_KERNEL_CFLAGS	?= -DCOMPAT_43 -DMACH -D__OSF__ -DNIDL_PROTOTYPES -DIDL_PROTOTYPES \
			   -traditional -Dvolatile=__volatile__ -mregparm -mnosb

.if ${TARGET_MACHINE}=="SVR4"
.if defined(BLD_USR_OR_KERNEL) && (${BLD_USR_OR_KERNEL}=="kernel")
 RPC_UUID_KERNEL ?=
.endif
.endif

.if defined(RPC_UUID_KERNEL) && ${MAKESUB}=="rpc/"
KSUBDIRS		+= kernel
.endif

#
# The following definitions are not germane to the rpc test tree.
#
.if !defined(_TESTRPC_MK_)

#
# Per-subcomponent definitions.
#

.if ${MAKESUB}=="rpc/runtime/" || (defined(LIBDCE_LIBRARY_MAKEFILE) && ${LIBDCE_CURR_INCDIR} == "rpc/runtime/")
RIOS_CFLAGS	?= ${RIOS_CFLAGS_COMMON} -D_BSD=44 -D_NONSTD_TYPES
ULTRIX_CFLAGS	?= -DBSD -YPOSIX
MIPS_CFLAGS	?= ${MIPS_CFLAGS_COMMON} ${ULTRIX_CFLAGS} -DAUTH_DUMMY
VAX_CFLAGS	?= ${VAX_CFLAGS_COMMON} ${ULTRIX_CFLAGS}
APOLLO68K_CFLAGS?= ${APOLLO68K_CFLAGS_COMMON} -DBSD -DUNIX -DCONVENTIONAL_ALIGNMENT 
HP_PA_CFLAGS	?= ${HP_PA_CFLAGS_COMMON} -DBSD -DUNIX -DCONVENTIONAL_ALIGNMENT
HPUX_CFLAGS     ?= ${HPUX_CFLAGS_COMMON}
HP800_CFLAGS	?= ${HPUX_CFLAGS} ${HP800_CFLAGS_COMMON}
HP300_CFLAGS	?= ${HPUX_CFLAGS} ${HP300_CFLAGS_COMMON}
SVR4_CFLAGS	?= ${SVR4_CFLAGS_COMMON} -DSYS5 -DAUTH_KRB ${SNI_NO_PRIVACY_FLAG}
.if defined(RPC_UUID_KERNEL)
SVR4_CFLAGS	+= -DRPC_UUID_KERNEL
.endif

#
# You should use the file ipnaf_bsd.c as a porting template, 
# and place your machine/OS specific file in the machine/OS subdirectory.
#
# If you can, you should use ipnaf_bsd.c with no changes (like OSF/1).
#
HP_PA_libnck_OFILES	?= sysconf.o comsoc_bsd.o ipnaf_bsd.o
HPUX_libnck_OFILES	?= comsoc_bsd.o ipnaf_sys.o cs_s_conv.o
HP800_libnck_OFILES	?= ${HPUX_libnck_OFILES}
HP300_libnck_OFILES	?= ${HPUX_libnck_OFILES}
OSF1_libnck_OFILES	?= comsoc_bsd.o ipnaf_bsd.o cs_s_conv.o
PMAX_libnck_OFILES	?= ${OSF1_libnck_OFILES}
AT386_libnck_OFILES	?= ${OSF1_libnck_OFILES}
ULTRIX_libnck_OFILES	?= comsoc_bsd.o ipnaf_bsd.o
MIPS_libnck_OFILES	?= ${ULTRIX_libnck_OFILES}
VAX_libnck_OFILES	?= ${ULTRIX_libnck_OFILES}
APOLLO68K_libnck_OFILES	?= comsoc_bsd.o ipnaf_bsd.o
RIOS_libnck_OFILES	?= comsoc_bsd.o ipnaf_sys.o cs_s_conv.o
SVR4_libnck_OFILES	?= comsoc_bsd.o ipnaf_sys.o
.if defined(RPC_UUID_KERNEL)
SVR4_libnck_OFILES	+=  get_unique_addr.o
.endif
RIOS_ipnaf_sys	?= -DBROADCAST_NEEDS_LOOPBACK 
RIOS_krbreq	?= -D_NO_PROTO 

.if ${TARGET_MACHINE} == "HP300" || ${TARGET_MACHINE} == "HP800"
compat.o_CFLAGS         = ${CFLAGS} +e -DUNIX -DSYS5 -DSYS5_SOCKETS_TYPE5
.endif

.endif

#
# Turn off shared library building for the kernel pieces on OSF1
#
.if ${MAKESUB} == "rpc/kdes/" || ${MAKESUB} == "rpc/idl/klib/" || ${MAKESUB} == "rpc/kruntime/"
USE_SHARED_LIBRARIES = 0
NO_ZFLAG=1
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"

.if ${TARGET_OS} == "OSF1"
   USE_SHARED_LIBRARIES = 0
KERNEL_INCDIRS=${OSF1_KERNEL_INCDIRS}
parse_ext_args = action_ADDEXISTS list_INCDIRS dirs_KERNEL_INCDIRS addhow_PREPEND prefix_-I
.include <osf.parse_ext.mk>
.endif
.endif

.if ${MAKESUB}=="rpc/kdes/" && ${TARGET_OS}=="HPUX"
OPT_LEVEL=+O3
.endif

.if ${MAKESUB}=="rpc/kruntime/"
.if ${TARGET_OS}=="HPUX"
md5_krpc.o_OPT_LEVEL=+O3
.endif

RIOS_CFLAGS	?= ${RIOS_CFLAGS_COMMON} -D_BSD=44 -D_SOCKADDR_LEN -DCOMPAT_43
.if ${TARGET_OS} == "OSF1"
   USE_SHARED_LIBRARIES = 0
KERNEL_INCDIRS := ${INCDIRS:S;${target_machine}/usr/include;&/kernel;g:S/^-I//g}
parse_ext_args = action_ADDEXISTS list_INCDIRS dirs_KERNEL_INCDIRS addhow_PREPEND prefix_-I
.include <osf.parse_ext.mk>
.endif

.endif

.if ${MAKESUB}=="rpc/idl/nidl_to_idl/"
RIOS_CFLAGS	?= ${RIOS_CFLAGS_COMMON} -D_BSD=44 -DBSD
SVR4_CFLAGS	?= ${SVR4_CFLAGS_COMMON} -DSYS5
HP800_CFLAGS    ?= ${HPUX_CFLAGS} ${HP800_CFLAGS_COMMON} -DSYS5 +e
HP300_CFLAGS    ?= ${HPUX_CFLAGS} ${HP300_CFLAGS_COMMON} -DSYS5 +e
LIBS_DCE	=
.endif

.if ${MAKESUB}=="rpc/idl/idl_compiler/"
HP_PA_CFLAGS	?= -DAPOLLO_LEX_YACC -Dunix -D__OSF__
HPUX_CFLAGS     ?= ${HPUX_CFLAGS_COMMON} -DBSD
HP800_CFLAGS	?= ${HPUX_CFLAGS} ${HP800_CFLAGS_COMMON}
HP300_CFLAGS	?= ${HPUX_CFLAGS} ${HP300_CFLAGS_COMMON}
APOLLO68K_CFLAGS?= -Dunix -DBSD
LIBS_DCE	=
.endif

.if ${MAKESUB} == "rpc/csrc/"
LINK_STATIC=1	
.endif

#
# Defaults for standard target-specific definitions (in case they weren't
# defined above).
#

RIOS_CFLAGS     ?= ${RIOS_CFLAGS_COMMON}
VAX_CFLAGS      ?= ${VAX_CFLAGS_COMMON}
HP_PA_CFLAGS    ?= ${HP_PA_CFLAGS_COMMON}
HP800_CFLAGS 	?= ${HP800_CFLAGS_COMMON}
HP300_CFLAGS	?= ${HP300_CFLAGS_COMMON}
PMAX_CFLAGS     ?= ${PMAX_CFLAGS_COMMON}
AT386_CFLAGS    ?= ${AT386_CFLAGS_COMMON}
SVR4_CFLAGS	?= ${SVR4_CFLAGS_COMMON}

RIOS_YFLAGS     ?= ${RIOS_YFLAGS_COMMON}

RIOS_LIBS       ?= ${RIOS_LIBS_COMMON}
VAX_LIBS        ?= ${VAX_LIBS_COMMON}
PMAX_LIBS       ?= ${PMAX_LIBS_COMMON}
AT386_LIBS      ?= ${AT386_LIBS_COMMON}
MIPS_LIBS       ?= ${MIPS_LIBS_COMMON}
APOLLO68K_LIBS  ?= ${APOLLO68K_LIBS_COMMON}
HP_PA_LIBS      ?= ${HP_PA_LIBS_COMMON}
HP800_LIBS 	?= ${HP800_LIBS_COMMON}
HP300_LIBS 	?= ${HP300_LIBS_COMMON}
#
# Defaults for standard definitions.  These may be overriden in some RPC
# subcomponent makefiles.
#

CFLAGS  ?= ${${TARGET_MACHINE}_CFLAGS}
YFLAGS  ?= ${${TARGET_MACHINE}_YFLAGS}
LIBS    ?= ${LIBS_DCE} ${${TARGET_MACHINE}_LIBS}

.endif

.endif
