#
# @OSF_COPYRIGHT@
# Copyright (c) 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:10:36  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:24  marty]
#
# Revision 1.1.4.1  1995/12/08  21:35:13  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:07:06  root]
# 
# Revision 1.1.2.7  1994/07/13  15:04:41  marty
# 	CR # 10017 - Set permissions on header files to 444 during install pass
# 	[1994/07/12  15:53:19  marty]
# 
# Revision 1.1.2.6  1994/04/26  19:11:23  bowe
# 	New files: aclimpl.o, aclconv.o, acleval.o
# 	[1994/04/26  17:37:31  bowe]
# 
# Revision 1.1.2.5  1994/04/19  13:21:34  bowe
# 	Set IMODE to 444
# 	[1994/04/19  13:19:31  bowe]
# 
# Revision 1.1.2.4  1994/03/09  22:56:40  bowe
# 	Added IDLINCFLAGS and database_ACF definitions.
# 	[1994/03/09  22:50:29  bowe]
# 
# Revision 1.1.2.3  1994/01/25  22:53:10  bowe
# 	Set libacldb.a_OFILES, EXPSHLIB_TARGETS, LIBRARIES variables.
# 	Fixed value of IDIR.  Removed dependency on dcelibmsg.h.
# 	(CR 9814)
# 	[1994/01/25  22:42:33  bowe]
# 
# Revision 1.1.2.2  1994/01/25  14:58:34  bowe
# 	Set INCLUDES to right value.  It had some dups.
# 	[1994/01/25  13:40:31  bowe]
# 
# Revision 1.1.2.1  1994/01/24  22:01:31  bowe
# 	Initial checkin
# 	[1994/01/24  21:59:23  bowe]
# 
# $EndLog$
#

libacldb_OFILES		= ${DB_OFILES} ${ACL_OFILES}
libacldb.a_OFILES	= ${libacldb_OFILES}
DB_OFILES		= dbdelete.o dbfetch.o dbfree.o dbiterate.o \
			dblock.o dbopen.o dbstore.o dbheader.o database_cstub.o
ACL_OFILES		= aclimpl.o aclconv.o acleval.o \
			dceacl_cstub.o rdaclif_sstub.o

.if !defined(LIBDCE_LIBRARY_MAKEFILE)

IDLFILES		= database.idl dceacl.idl rdaclif.idl
IDLFLAGS		= -keep c_source -no_mepv
IDLINCFLAGS		= -I.
database_ACF		=

OFILES			= ${libacldb_OFILES}

EXPDIR			= /usr/include/dce/
IDIR			= /share/include/dce/
IMODE			= 444
libacldb.a_EXPDIR	= /usr/lib/
# Note that rdaclif.{h,idl} are exported from the .../security/idl,
# so we can't use the IDLFILES variable.
INCLUDES		= database.idl database.h dceacl.idl dceacl.h \
			dbif.h aclif.h
IMODE			= 444
ILIST			= ${INCLUDES}

VPATH			= ../../../security/idl

.if ${USE_SHARED_LIBRARIES} == 0
LIBRARIES		= libacldb.a
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}
.endif
LIBRARIES		= libacldb.a
EXPSHLIB_TARGETS	= ${libacldb_OFILES}

.include <${RULES_MK}>

${DB_OFILES}:	dbimpl.h database.h
${ACL_OFILES}:	dceacl.h aclif.h dbimpl.h

.endif
