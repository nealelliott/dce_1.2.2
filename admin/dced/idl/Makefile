#
# @OSF_COPYRIGHT@
# Copyright (c) 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:06:53  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:25:42  marty]
#
# Revision 1.1.4.1  1995/12/08  21:27:16  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/01/13  19:53 UTC  mk
# 	Merge in changes from mk_mothra_dced/2
# 
# 	HP revision /main/mk_mothra_dced/2  1995/01/13  19:49 UTC  mk
# 	Add rrpc.idl to IDLFILES.
# 
# 	HP revision /main/HPDCE02/1  1994/12/07  16:03 UTC  mk
# 	Merge in changes from mk_mothra_dced/1
# 
# 	HP revision /main/mk_mothra_dced/1  1994/11/28  22:42 UTC  mk
# 	Add rutc.idl to IDLFILES and rutc.acf to INCLUDES.
# 	[1995/12/08  17:59:30  root]
# 
# Revision 1.1.2.15  1994/07/13  15:14:59  marty
# 	CR # 10017 - Set permissions on header files to 444 during install pass
# 	[1994/07/12  15:53:18  marty]
# 
# Revision 1.1.2.14  1994/05/24  12:33:05  rsalz
# 	10724
# 	[1994/05/24  12:32:02  rsalz]
# 
# 	Update SAMIDL_TARGETS to reflect new output rules (OT CR 10724)
# 	[1994/05/24  12:26:32  rsalz]
# 
# Revision 1.1.2.13  1994/05/05  20:07:18  rsalz
# 	Initial server functionality drop.
# 	[1994/05/05  20:00:45  rsalz]
# 
# Revision 1.1.2.12  1994/04/19  20:31:34  rsalz
# 	Add dced to SAMIDL pass (OT CR 10364)
# 	[1994/04/19  17:12:49  rsalz]
# 
# Revision 1.1.2.11  1994/04/18  00:15:32  pwang
# 	Exported dcedhdsvc.h [OT#10364]
# 	[1994/04/17  22:00:51  pwang]
# 
# Revision 1.1.2.10  1994/04/11  18:45:49  rsalz
# 	Update to final RFC revision.
# 	[1994/04/11  18:26:18  rsalz]
# 
# Revision 1.1.2.9  1994/03/18  19:57:18  rsalz
# 	Put permset bits in dced_aclbits.idl (OT CR 9768)
# 	[1994/03/18  19:55:37  rsalz]
# 
# Revision 1.1.2.8  1994/03/11  00:07:20  pwang
# 	Moved the building of dcedhdmsg.c from here to api dir.
# 	[1994/03/11  00:02:15  pwang]
# 
# Revision 1.1.2.7  1994/03/09  20:37:54  pwang
# 	Merged with changes from 1.1.2.6
# 	[1994/03/09  20:37:48  pwang]
# 
# 	Only export dced_base.[idl,h] and dcedhdmsg.h
# 	[1994/03/09  20:31:23  pwang]
# 
# Revision 1.1.2.6  1994/03/07  20:36:37  rsalz
# 	Updating to second edition of functional spec.
# 	Moved ACF files to API directory.
# 	[1994/03/07  20:33:05  rsalz]
# 
# Revision 1.1.2.5  1994/02/28  18:38:19  pwang
# 	Added the missed dcedhdmsg.o
# 	[1994/02/28  18:37:39  pwang]
# 
# Revision 1.1.2.4  1994/02/28  16:26:51  pwang
# 	Moved dced library build to api.
# 	[1994/02/28  15:57:04  pwang]
# 
# Revision 1.1.2.3  1994/02/22  21:40:24  bowe
# 	1. Added xxx_ACF for each idl file. 2. Only produce server stubs, since
# 	clients should not (and really can not) call the interfaces directly.
# 	3. Surround meat of Makefile inside .if !defined(LIBDCE_LIBRARY_MAKEFILE).
# 	[1994/02/22  21:39:13  bowe]
# 
# Revision 1.1.2.2  1994/01/21  19:07:39  bowe
# 	Changed references from "keymgmt" to "rkeytab".  The filename "keymgmt.idl"
# 	was already in use.
# 	[1994/01/21  19:03:29  bowe]
# 
# Revision 1.1.2.1  1994/01/21  18:03:25  bowe
# 	Initial checkin
# 	[1994/01/21  18:00:31  bowe]
# 
# $EndLog$
#

# SAMS files and its targets
SAMSFILES		= dhd.sams
SAMIDL_INCLUDES		= dcedhdmsg.h
SAMS_TARGETS		= dcedhdmsg.h dcedhdmsg.c dcedhdmac.h \
			dcedhdsvc.h dcedhdsvc.c dcedhd.cat

# IDL files to build and export and idl flags
IDLFILES		= dced_base.idl dced_aclbits.idl rutc.idl rrpc.idl
IDLFLAGS		= -keep none

# Header files and exported files
HEADERS			= ${IDLFILES:S/.idl$/.h/g} ${SAMIDL_INCLUDES}
INCLUDES		= ${IDLFILES} ${HEADERS} rutc.acf

# Files to install
IMODE			= 0444
ILIST			= ${INCLUDES} dcedhd.cat

# Directory in which to install and export the files
EXPDIR			= /usr/include/dce/
IDIR			= /share/include/dce/
dcedhd.cat_IDIR		= /nls/msg/en_US.ASCII/

.include <${RULES_MK}>
