# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.10.2  1996/03/09  23:11:24  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:47  marty]
#
# Revision 1.2.10.1  1995/12/08  00:09:53  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:53:05  root]
# 
# Revision 1.2.8.14  1994/07/25  15:11:21  proulx
# 	Use common ACL manager for subcomponent mgmt ACLs
# 	[1994/07/25  14:02:40  proulx]
# 
# Revision 1.2.8.13  1994/06/30  19:14:54  mccann
# 	sams cleanup drop2
# 	[1994/06/30  18:41:38  mccann]
# 
# Revision 1.2.8.12  1994/05/24  22:49:09  jd
# 	Remove the -D_BSD from compile of adver_clerk.o. It causes wrong definition
# 	of WIFEXIT to be picked up for RIOS.
# 	[1994/05/24  22:06:35  jd]
# 
# Revision 1.2.8.11  1994/05/20  22:03:08  sommerfeld
# 	Merged with changes from 1.2.8.10
# 	[1994/05/20  22:02:29  sommerfeld]
# 
# 	fix sams stuff.
# 	[1994/05/20  21:30:30  sommerfeld]
# 
# 	fix sams stuff.
# 
# Revision 1.2.8.9  1994/05/12  21:09:46  peckham
# 	Change adver_sysdep_unix.o to os-dependent adver_socket.o
# 	Add TARGET_OS to VPATH.
# 	Add adver_clerk.o and adver_server.o
# 	[1994/05/12  19:06:37  peckham]
# 
# Revision 1.2.8.8  1994/04/19  20:15:36  jd
# 	Undo BSD for adver_clerk_events.o. It causes problems with
# 	WIFEXITED.
# 	[1994/04/19  19:27:13  jd]
# 
# Revision 1.2.8.7  1994/04/19  17:03:43  mccann
# 	More code cleanup
# 	[1994/04/19  16:22:13  mccann]
# 
# Revision 1.2.8.5  1994/03/16  23:41:15  jd
# 	Removed the _BSD from build of adver_clerk_event.o. It caused problems
# 	with WIFEXITED and friends.
# 	[1994/03/16  22:37:25  jd]
# 
# Revision 1.2.8.4  1994/03/14  21:38:27  jd
# 	SAMSFLAGS didn;t work. Added -f to the sams invocation.
# 	[1994/03/14  21:33:17  jd]
# 
# Revision 1.2.8.3  1994/03/14  21:22:09  jd
# 	Add SAMSFLAGS=-f
# 	[1994/03/14  21:21:08  jd]
# 
# Revision 1.2.8.2  1994/03/12  21:54:40  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  13:43:34  peckham]
# 
# Revision 1.2.8.1  1994/01/28  23:08:56  burati
# 	Delegation/EPAC changes - use rdaclifv0 until upgraded (dlg_bl1)
# 	[1994/01/18  21:17:50  burati]
# 
# Revision 1.2.6.1  1993/10/15  21:19:01  ohara
# 	expand the VPATH to use context, needed for ODE2.3 on SVR4
# 	[1993/10/15  16:12:48  ohara]
# 
# Revision 1.2.4.2  1992/12/30  12:34:26  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:16:21  zeliff]
# 
# Revision 1.2.2.2  1992/03/22  21:07:35  weisman
# 		Big merge with post-1.0 development at Digital.
# 	[1992/03/22  20:02:56  weisman]
# 
# Revision 1.2  1992/01/19  22:12:52  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#  OSF DCE Version 1.0 
#
# Makefile for building the advertiser
#

VPATH       = ${TARGET_OS}:../stubs:../../../../export/${target_machine}/usr/include/dce:../../../../export/${context}/usr/include/dce:../includes

# programs to build
PROGRAMS	= cdsadv

# install directory
IDIR		= /bin/

# list of things to install
ILIST		= ${PROGRAMS}

# list of libraries used to bind programs
cdsadv_LIBS		= -lcdscache ${${TARGET_MACHINE}_LIBS}

# compiler flags
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS        = -I../stubs -I../includes

LDFLAGS		= ${${TARGET_MACHINE}_LDFLAGS}
LIBFLAGS    = -L../cache

rdaclif_sstub.c_IDLFLAGS = -no_mepv
rdaclifv0_sstub.c_IDLFLAGS = -no_mepv

IDLFILES    = cds_solicit.idl cds_types.idl cprpc_clerk.idl \
			  rdaclifv0.idl rdaclif.idl

# List of objects to build cdsadv
cdsadv_OFILES = adver_main.o \
		adver_cache.o \
		adver_cds_event.o \
		adver_clerk.o \
		adver_server.o \
		adver_service_rpc.o \
		adver_socket.o \
		adver_svc.o \
		cds_solicit_sstub.o \
		cds_solicit_cstub.o \
		cds_types_saux.o \
		cds_types_caux.o \
		dcecdssvc.o dcecdsmsg.o \
		cprpc_clerk_sstub.o  ${ADVER_SEC_OFILES}


GARBAGE = cds_solicit_sstub.c cds_types_saux.c cprpc_clerk_sstub.c \
		  cds_types_caux.c cprpc_clerk_sstub.c \
		  rdaclifv0_sstub.c rdaclif_sstub.c

SAMSFILES=cds.sams
SAMS_TARGETS=dcecdssvc.c dcecdsmsg.c

.include <${RULES_MK}>

.ifdef BLD_SEC
ADVER_SEC_OFILES = adver_dacl.o \
              rdaclifv0_sstub.o rdaclif_sstub.o
.endif

