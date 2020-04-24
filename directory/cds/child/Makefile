#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.8.2  1996/03/09  23:11:39  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:58  marty]
#
# Revision 1.2.8.1  1995/12/08  15:03:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:37:35  root]
# 
# Revision 1.2.6.8  1994/06/23  18:28:38  mccann
# 	sams cleanup drop1
# 	[1994/06/22  18:53:27  mccann]
# 
# Revision 1.2.6.7  1994/06/09  16:07:26  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:52:04  devsrc]
# 
# Revision 1.2.6.6  1994/05/20  22:03:11  sommerfeld
# 	fix build of svc files.
# 	[1994/05/20  21:30:47  sommerfeld]
# 
# 	fix build of svc files.
# 
# Revision 1.2.6.5  1994/04/14  14:41:01  peckham
# 	New module clerk_socket.c to isolate SVR4 named pipes.
# 	Remove last vestige of clerk_trace().
# 	[1994/04/11  19:30:32  peckham]
# 
# Revision 1.2.6.4  1994/04/04  20:05:53  peckham
# 	Remove sams -f flag
# 	[1994/04/04  17:57:41  peckham]
# 
# Revision 1.2.6.3  1994/03/14  21:38:28  jd
# 	SAMSFLAGS didn;t work. Added -f to sams invocation.
# 	[1994/03/14  21:34:42  jd]
# 
# Revision 1.2.6.2  1994/03/14  21:22:10  jd
# 	Add SAMSFLAGS=-f
# 	[1994/03/14  21:21:17  jd]
# 
# Revision 1.2.6.1  1994/03/12  21:56:08  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  13:49:18  peckham]
# 
# Revision 1.2.4.2  1992/12/30  12:40:11  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:25:30  zeliff]
# 
# Revision 1.2.2.2  1992/04/29  13:24:00  mfox
# 	Makefile for child links in library .o's on all platforms
# 	[1992/04/29  13:19:16  mfox]
# 
# Revision 1.2  1992/01/19  22:12:55  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
#
# THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
# ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
# INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
# COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
# OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
# TRANSFERRED.
#
# THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
# AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
# CORPORATION.
#
# DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
# SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
#
# Makefile for naming clerk program
#

VPATH		= ${TARGET_OS}:../stubs:../library:../includes

# programs to build
PROGRAMS	= cdsclerk

# install directory
IDIR		= /bin/

# list of things to install
ILIST		= ${PROGRAMS}

# list of libraries used to bind programs
LIBS		= -lcdscache ${${TARGET_MACHINE}_LIBS}

# Compiler flags for cdsclerk
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS} 

INCFLAGS	= -I../stubs -I../includes

LIBFLAGS    = -L../cache
LDFLAGS     = ${${TARGET_MACHINE}_LDFLAGS}

# List of objects needed for cdsclerk

IDLFILES	= cds_clerkserver.idl cds_types.idl
cds_types_ACF =
cds_clerkserver_ACF =

OFILES = clerk_main.o clerk_listener.o clerk_client.o clerk_socket.o \
	clerk_gen_msg.o clerk_find_ch.o \
	clerk_tree_walk.o clerk_choose_ch.o clerk_miscel.o \
	clerk_progress.o clerk_bind.o createuid.o \
	clerkaddreplica.o \
	clerkcreatects.o clerkcreatedirectory.o clerkcreatelink.o \
	clerkcreateobject.o clerkdeletedirectory.o clerkdeletelink.o \
	clerkdeleteobject.o \
	clerkenumerateattributes.o clerkenumeratechildren.o \
	clerkenumerateobjects.o clerkenumeratesoftlinks.o \
	clerkmodifyattribute.o clerknewepoch.o clerkreadattribute.o \
	clerkremovereplica.o clerkresolvename.o clerkskulk.o \
	clerktestattribute.o clerktestgroup.o nsclerkcombine.o \
	nsclerkcreatechildentry.o nsclerkdeletechildentry.o \
	nsclerkdoupdate.o nsclerklinkreplica.o \
	nsclerkmodifyreplica.o clerk_prm.o clerkprogressrpc.o \
	cds_clerkserver_cstub.o cds_types_caux.o \
        dcecdssvc.o dcecdsmsg.o \
	rpc_cvt.o dpeacl.o dpeaclconvert.o dpeaclstore.o dpeaclaccess.o

SAMSFILES=cds.sams
SAMS_TARGETS=dcecdssvc.c dcecdsmsg.c


.include <${RULES_MK}>

GARBAGE	= cds_clerkserver_cstub.c cds_types_caux.c
