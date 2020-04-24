#
# @OSF_COPYRIGHT@
# Copyright (c) 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:11:09  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:36  marty]
#
# Revision 1.1.4.1  1995/12/08  21:36:58  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/07/11  21:55 UTC  mk
# 	Merge in change to CFLAGS from mk_dced_as_rpcd.
# 
# 	HP revision /main/HPDCE02/mk_dced_as_rpcd/1  1995/07/11  21:53 UTC  mk
# 	Delete -g from CFLAGS.
# 
# 	HP revision /main/HPDCE02/2  1994/09/21  12:44 UTC  mothra
# 	Merge from Hpdce02_02
# 	[1995/12/08  18:08:46  root]
# 
# Revision 1.1.2.15  1994/09/02  18:34:33  pwang
# 	Don't built libdceutils.a since it's in libdce [OT#11402]
# 	[1994/09/02  18:17:48  pwang]
# 
# Revision 1.1.2.14  1994/08/28  14:40:27  rsalz
# 	Add dce_gettimeofday (OT CR 11931).
# 	[1994/08/28  14:20:04  rsalz]
# 
# Revision 1.1.2.13  1994/08/28  13:21:05  rsalz
# 	Add tilde expansion to filename completion (OT CR 11790).
# 	[1994/08/28  13:16:16  rsalz]
# 
# 	HP revision /main/HPDCE02/1  1994/09/01  12:00 UTC  mothra
# 	add dcelib.cat to SAMS_TARGETS so it gets built and therefore installed
# 
# Revision 1.1.2.12  1994/07/27  19:19:52  rsalz
# 	Add bvfile (OT CR 11466).
# 	[1994/07/27  19:18:27  rsalz]
# 
# Revision 1.1.2.11  1994/07/27  17:12:49  rsalz
# 	Use dce_strdup (OT CR 11341) and do code-cleanup.
# 	[1994/07/27  17:02:53  rsalz]
# 
# Revision 1.1.2.10  1994/07/26  22:38:53  pwang
# 	Build debug symbols in by now (i.e add -g to CFLAGS).
# 	[1994/07/26  22:38:09  pwang]
# 
# Revision 1.1.2.9  1994/07/23  17:34:05  pwang
# 	Build libdceutils.a (until others remove -ldceutils
# 	from their Makefile since libdceutils is in libdce).
# 	[1994/07/23  17:31:44  pwang]
# 
# Revision 1.1.2.8  1994/07/22  21:39:48  pwang
# 	Added attr into dceutils and some cleanup here [OT#11287]
# 	[1994/07/22  20:31:27  pwang]
# 
# Revision 1.1.2.7  1994/07/18  14:50:26  rsalz
# 	Add dce_strdup (OT CR 11341).
# 	[1994/07/18  14:49:01  rsalz]
# 
# Revision 1.1.2.6  1994/06/30  19:37:01  ohara
# 	Merged with changes from 1.1.2.5
# 	[1994/06/30  19:36:35  ohara]
# 
# 	include machdep.mk
# 	[1994/06/27  19:45:38  ohara]
# 
# Revision 1.1.2.5  1994/06/29  13:15:04  melman
# 	style suggestions from John Bowe
# 	[1994/06/29  12:14:42  melman]
# 
# 	Implemented isdcepcs()
# 	[1994/06/28  22:39:10  melman]
# 
# Revision 1.1.2.4  1994/06/22  17:51:42  bowe
# 	Added reference to dce_802_addr.o [CR 10004]
# 	Added missing .h files to ILIST and INCLUDES [CR 11028]
# 	[1994/06/22  16:51:12  bowe]
# 
# Revision 1.1.2.3  1994/04/19  13:21:40  bowe
# 	Set IDIR, ILIST, IMODE [CR 10355]
# 	[1994/04/19  13:21:03  bowe]
# 
# Revision 1.1.2.2  1994/03/16  21:56:55  bowe
# 	Added dce64.h to INCLUDES
# 	[1994/03/16  21:20:38  bowe]
# 
# Revision 1.1.2.1  1994/01/25  22:53:11  bowe
# 	Initial checkin - moved here from ./dce/utils
# 	[1994/01/25  22:38:47  bowe]
# 
# $EndLog$
#

libdceutils_OFILES	= ${MSG_OFILES} ${EDITLINE_OFILES} ${MISC_OFILES} \
			  ${DCE_ATTR_OFILES}
libdceutils.a_OFILES	= ${libdceutils_OFILES}

EDITLINE_OFILES		= editline.o complete.o sysunix.o 
MSG_OFILES		= dcelibmsg.o
MISC_OFILES		= localdir.o dce_priv_proc.o dce_802_addr.o \
			  isdcepcs.o strdup.o bvfile.o gettime.o
DCE_ATTR_OFILES		= dce_attr_bind.o dce_attr_sch.o

VPATH			= ../attr:${TARGET_MACHINE}

# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS =
LIBS    =

#
#
# Don't want to build any '.a' here if we are using shared libraries
#
.if ${USE_SHARED_LIBRARIES} == 0
LIBRARIES		= libdceutils.a
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

# things to install & export
libdceutils.a_IDIR        = /usr/lib/
libdceutils.a_EXPDIR      = /usr/lib/
.endif

EXPSHLIB_TARGETS	= ${libdceutils_OFILES}

# sams file and target
SAMSFILES              = lib.sams
SAMSFILES_INCLUDES     = dcelibmsg.h
SAMS_TARGETS           = dcelibmsg.c dcelib.cat

EXPDIR			= /usr/include/dce/
INCLUDES		= ${SAMSFILES_INCLUDES} editline.h dce_priv_proc.h \
			  dce_utils.h dce64.h


dcelib.cat_IDIR		= /nls/msg/en_US.ASCII/
IDIR			= /share/include/dce/
ILIST			= dce_utils.h dce64.h dcelib.cat dcelibmsg.h
IMODE			= 444

# Set your options for editlines modules:
#	-DANSI_ARROWS		ANSI arrows keys work like emacs.
#	-DHIST_SIZE=n		History size.
#	-DUNIQUE_HISTORY	Don't save command if same as last one.
#	-DEXPAND_TILDE		Do ~ expansion in file completion
editline.o_CFLAGS	+= -DANSI_ARROWS
complete.o_CFLAGS	+= -DEXPAND_TILDE

CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} ${STRICT_COMPILER_WARNINGS}
INCFLAGS	= -I. -I../attr

.include <${RULES_MK}>

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

$(EDITLINE_OFILES):	editline.h
${DCE_ATTR_OFILES}:	attrimpl.h

.endif


