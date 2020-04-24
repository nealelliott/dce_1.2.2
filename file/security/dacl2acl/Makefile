#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.167.1  1996/10/02  20:58:24  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:47:01  damon]
#
# Revision 1.1.162.4  1994/09/21  15:45:00  rsarbo
# 	compile with DACL_EPISODE
# 	[1994/09/20  19:56:13  rsarbo]
# 
# Revision 1.1.162.3  1994/06/09  14:19:06  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:31:06  annie]
# 
# Revision 1.1.162.2  1994/02/04  20:29:13  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:17:40  devsrc]
# 
# Revision 1.1.161.1  1993/12/07  17:33:03  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:51:02  jaffe]
# 
# Revision 1.1.4.4  1993/01/21  15:18:15  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:55:39  cjd]
# 
# Revision 1.1.4.3  1992/11/12  18:10:50  rsarbo
# 	incorporate additional i/f's to libdce on PMAX
# 	for Episode ACL support.
# 	[1992/11/12  18:04:01  rsarbo]
# 
# 	add object list for libdacl2acl; assign EXPSHLIB_TARGETS
# 	[1992/11/09  14:58:41  rsarbo]
# 
# Revision 1.1.4.2  1992/08/31  21:12:30  jaffe
# 	minor changes
# 	[1992/08/30  12:21:54  jaffe]
# 
# Revision 1.1.2.5  1992/07/07  21:57:39  mason
# 	Transarc delta: bab-ot4457-security-dacl2acl-more-files 1.1
# 	  Selected comments:
# 	    More routines need to be moved into the dacl2acl library to
# 	    support the new acl_edit work.
# 	    ot 4457
# 	    See above.
# 	[1992/07/07  21:36:18  mason]
# 
# Revision 1.1.2.4  1992/04/15  19:34:57  garyf
# 	remove auth5b definitions
# 	let AFS_DEBUG be controlled by normal mechanisms
# 	[1992/04/15  19:34:28  garyf]
# 
# Revision 1.1.2.3  1992/04/14  03:35:12  mason
# 	Transarc delta: bab-ot2288-dacl-source-id-size 1.4
# 	  Files modified:
# 	    security/dacl: epi_id.h; security/dacl.klib: Makefile
# 	    security/dacl2acl: Makefile; security/dacl_lfs: Makefile
# 	    security/dacl_lfs.klib: Makefile
# 	  Selected comments:
# 	    Specification of source principal id size needs to be separated from
# 	    size of stored principal ids, since they aren't always the same.
# 	    Also, should only compare as many bits in a principal id comparison
# 	    as are passed from the external source.
# 	    Make sure that the affected Makefiles set the appropriate flag to indicate
# 	    the size of the source ids.
# 	    Add -DEPI_SOURCE_IDS_ARE_32BITS to list of CFLAGS.
# 	    Missed one of the Makefiles that need this flag set on the last pass.
# 	    NB: if you import this delta, you MUST also import bab-ot2370-dacl-use-all-bits
# 	    if you want the dacl package to work in non-Episode user-space (i.e., in any
# 	    of our servers)!
# 	[1992/04/13  17:13:43  mason]
# 
# Revision 1.1.2.2  1992/01/23  14:59:45  jim
# 	Create this Makefile for RIOS builds.
# 	[1992/01/23  14:59:26  jim]
# 
# Revision 1.2  1992/01/19  22:16:06  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#
libdacl2acl_OFILES = \
        dacl_sec_acl.o     dacl_debug.o        dacl_uuidxlat.o \
        dacl_parse.o       dacl_flatten.o      dacl_mgruuids.o \
        dacl_access.o      dacl_permbits.o     epi_id.o \
        dacl_getmode.o     dacl_pac.o          dacl_strings.o \
        dfssec_errorStrings.o        ${${TARGET_MACHINE}_libdacl2acl_OFILES} \
	iclstubs.o

libdacl2acl.a_OFILES = ${libdacl2acl_OFILES}

#
# the rest of the makefile is "private"
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)

VPATH			= ../dacl:..

LIBRARIES		= libdacl2acl.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
EXPSHLIB_TARGETS        = ${libdacl2acl_OFILES}

CFLAGS                  = -DAFS_DEBUG ${USERCFLAGS} -DDACL2ACL -DEPI_SOURCE_IDS_ARE_32BITS -DDACL_EPISODE
epi_id.o_CFLAGS		= ${USERCFLAGS} -DEPI_USE_FULL_ID -DDACL_EPISODE -DDACL2ACL
INCFLAGS		= -I../dacl

OFILES			= dacl_sec_acl.o dacl_debug.o dacl_uuidxlat.o \
			  dacl_parse.o dacl_flatten.o dacl_mgruuids.o \
			  dacl_access.o dacl_permbits.o epi_id.o      \
			  dacl_getmode.o dacl_pac.o dacl_strings.o      \
			  dfssec_errorStrings.o

.if ${TARGET_OS} == "HPUX"
IN_SHLIB		= 1
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif
.if exists (${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif
.include <${RULES_MK}>
.endif
