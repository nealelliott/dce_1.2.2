#-
# @TRANSARC_COPYRIGHT@
#
# $TALog: Makefile,v $
# Revision 4.15  1995/06/11  23:44:26  dlc
# Uniqify the names of the message catalogs (and the sams "component" names
# and et "error_table" names).  Make sure all of the catalog files which
# are generated are installed.  Cosmetic changes to omit sams generated
# catalogs from the OTHERS macro in Makefiles which are already mentioned in
# the SAMS_TARGETS macro.  They belong in SAMS_TARGETS and not OTHERS.
# Updated the ILIST macros accordingly.
#
# Removed the sams generated catalog file, dfscmc.cat, from OTHERS
# Updated ILIST.
# [from r4.14 by delta dlc-db6625-uniquify-message-catalog-names, r1.1]
#
# Revision 4.14  1995/04/25  17:56:43  bab
# [merge of changes from 4.8 to 4.12 into 4.13]
#
# [*** log entries omitted ***]
#
# $EndTALog$

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.944.1  1996/10/02  18:47:22  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:48:50  damon]
#
# Revision 1.1.939.6  1994/09/29  19:48:03  mckeen
# 	Fixed ILIST/IDIR to properly install cat files
# 	[1994/09/29  19:04:00  mckeen]
# 
# Revision 1.1.939.5  1994/08/30  15:56:13  sasala
# 	DFS S12Y general cleanup - rename ftutil and tools cmd catalogs, add ftutil msgs to xaggr dfsexport, and episode salvage and vnops subcomponents
# 	[1994/08/30  00:19:54  sasala]
# 
# Revision 1.1.939.4  1994/08/03  16:25:38  ruby
# 	Implement partial S12Y (Internationalization) for DFS commands and daemons
# 	[1994/08/02  20:24:05  ruby]
# 
# Revision 1.1.939.3  1994/06/09  14:22:48  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:34:19  annie]
# 
# Revision 1.1.939.2  1994/02/04  20:32:59  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:09  devsrc]
# 
# Revision 1.1.938.1  1993/12/07  17:35:49  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  17:17:56  jaffe]
# 
# Revision 1.1.6.5  1993/01/21  15:54:39  cjd
# 	embedded copyright notice
# 	[1993/01/20  17:37:57  cjd]
# 
# Revision 1.1.6.4  1992/11/24  19:47:45  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:32:04  bolinger]
# 
# Revision 1.1.6.3  1992/09/15  13:19:26  jaffe
# 	Transarc delta: jaffe-sync-with-aug-31-osf-sources 1.4
# 	  Selected comments:
# 	    Pick up changes from last submission to the OSF.
# 	    make this dir use USERCFLAGS, instead of KERNELCFLAGS
# 	    remove commondefs.idl.  This was done incompletely in another delta
# 	[1992/09/14  20:39:37  jaffe]
# 
# Revision 1.1.6.2  1992/08/31  21:33:28  jaffe
# 	Replace missing RCS ids
# 	[1992/08/31  16:10:04  jaffe]
# 
# 	minor changes
# 	[1992/08/30  12:56:37  jaffe]
# 
# Revision 1.1.3.9  1992/05/20  20:18:18  mason
# 	Transarc delta: cfe-ot3085-fix-msf-dependencies 1.1
# 	  Files modified:
# 	    bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	    bubasics: Makefile; flserver: Makefile
# 	    flserver.klib: Makefile; fshost: Makefile; fsint: Makefile
# 	    ftserver: Makefile; host: Makefile; ncscompat: Makefile
# 	    ncsubik: Makefile; security/dacl: Makefile
# 	    security/dfsauth: Makefile; sysacl: Makefile; tkm: Makefile
# 	    tools/cmd: Makefile; update: Makefile; xcred: Makefile
# 	    xvolume: Makefile
# 	  Selected comments:
# 	    Make explicit the dependency that builds an .msf file from an .et file.
# 	    Add dependency to build .msf file.
# 	[1992/05/20  16:44:41  mason]
# 
# Revision 1.1.3.8  1992/04/16  17:46:04  garyf
# 	fix .p.h rules
# 	[1992/04/16  17:45:37  garyf]
# 
# Revision 1.1.3.7  1992/04/14  04:10:49  mason
# 	Transarc delta: bab-ot2394-change-catalog-names 1.3
# 	   Files modified:
# 	     bak: Makefile; bakserver: Makefile; bosserver: Makefile
# 	     bubasics: Makefile; flserver: Makefile; fshost: Makefile
# 	     fsint: Makefile; ftserver: Makefile; host: Makefile
# 	     ncscompat: Makefile; ncsubik: Makefile
# 	     security/dacl: Makefile; security/dfsauth: Makefile
# 	     tkm: Makefile; tools/cmd: Makefile; update: Makefile
# 	     xcred: Makefile; xvolume: Makefile
# 	   Selected comments:
# 	     The dfs error catalogs were generated with names that made them
# 	     dce_error_inq_text unable to locate them. (dce_error_inq_text pulls
# 	     a facility code out of the error code and uses this to construct a
# 	     filename in which to look for the error message.  Our (dfs) facility
# 	     code is such that our error catalog filenames must start with "dfs",
# 	     rather then "dce".)
# 	     Change occurrences of "dce...\.cat" to "dfs...\.cat"
# 	     ot 2394
# 	     The sed script missed one occurrence of "dfs...\.cat"
# 	     the first time through because it was hidden by another
# 	     occurrence earlier on the line.
# 	[1992/04/13  18:01:43  mason]
# 
# Revision 1.1.3.6  1992/03/30  17:36:19  garyf
# 	user USERCFLAGS instead of KERNEL_CFLAGS
# 	[1992/03/30  17:36:01  garyf]
# 
# Revision 1.1.3.5  1992/01/31  16:57:51  delgado
# 	Add modifications to build libdcedfs.a
# 	[1992/01/31  16:57:31  delgado]
# 
# Revision 1.1.3.4  1992/01/25  20:50:23  zeliff
# 	dfs6.3 merge, part2
# 	[1992/01/25  20:09:59  zeliff]
# 
# $EndLog$


libcmd.a_OFILES		= cmd.o cmd_paramChecking.o cmd_help.o  \
			  dfscmcsvc.o dfscmcmsg.o

.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
# Information to build components when we are not building libdcedfs.a
#

PROGRAMS		= interactive_test

interactive_test_OFILES	= interactive_test.o dceback.hlp.o
interactive_test_LIBS	= -lcmd -lcmdAux -ldce ${${TARGET_OS}_LIBS}

INCLUDES		= cmd.h cmdAux.h cmd_help.h

LIBRARIES		= libcmd.a libcmdAux.a

EXPDIR			= /usr/include/dcedfs/
libcmd.a_EXPDIR		= /usr/lib/
libcmdAux.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfscmd.cat
TECHNOLOGY=dfs

SAMSFILES = cmc.sams
SAMS_TARGETS = dfscmcmsg.h dfscmcmsg.c dfscmcmac.h dfscmcsvc.c dfscmcsvc.h \
    dfscmc.cat

ILIST			= ${OTHERS} dfscmc.cat
dfscmd.cat_IDIR		= /nls/msg/en_US.ASCII/
dfscmc.cat_IDIR		= /nls/msg/en_US.ASCII/


CFLAGS			= ${USERCFLAGS}
libcmdAux.a_OFILES	= cmdAux.o

GARBAGE			= cmd.h

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

cmd.h cmd_errors.msf: cmd_errors.et cmd.p.h
	-${CP} ${cmd.p.h:P} .
	${COMPILE_ET} ${cmd_errors.et:P} -h cmd
	${RM} -f cmd.p.h

dfscmd.cat: cmd_errors.msf
	 ${GENCAT} $@ $>

.endif
