#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.13.1  1996/10/03  14:33:38  arvind
# 	Better NO_GDS support.
# 	[1996/09/17  06:10 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
#
# Revision 1.1.11.2  1996/03/09  23:11:45  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:03  marty]
# 
# Revision 1.1.11.1  1995/12/08  15:12:20  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:41:12  root]
# 
# Revision 1.1.9.10  1994/08/16  18:27:45  jd
# 	drop from janet
# 	[1994/08/13  16:05:14  jd]
# 
# Revision 1.1.9.9  1994/07/25  15:13:00  proulx
# 	Use common ACL manager for subcomponent mgmt ACLs
# 	[1994/07/25  14:00:43  proulx]
# 
# Revision 1.1.9.8  1994/06/23  18:29:55  mccann
# 	sams cleanup drop1
# 	[1994/06/22  19:26:44  mccann]
# 
# Revision 1.1.9.7  1994/06/09  18:37:55  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:09:59  devsrc]
# 
# Revision 1.1.9.6  1994/05/20  22:03:13  sommerfeld
# 	Fix build of SAMS files.
# 	[1994/05/17  19:20:47  sommerfeld]
# 
# Revision 1.1.9.5  1994/04/04  20:06:17  peckham
# 	Remove sams -f flag
# 	[1994/04/04  17:57:45  peckham]
# 
# Revision 1.1.9.4  1994/03/15  23:13:59  jd
# 	Removed the reference to -lxda for x500_addcell.
# 	[1994/03/15  23:12:26  jd]
# 
# Revision 1.1.9.3  1994/03/14  21:38:30  jd
# 	SAMSFLAGS didn;t work. Added -f to sams invocation.
# 	[1994/03/14  21:35:54  jd]
# 
# Revision 1.1.9.2  1994/03/14  21:22:11  jd
# 	Add SAMSFLAGS=-f
# 	[1994/03/14  21:21:28  jd]
# 
# Revision 1.1.9.1  1994/03/12  22:01:25  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  14:07:50  peckham]
# 
# Revision 1.1.7.2  1993/05/24  20:40:52  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:12:04  cjd]
# 
# Revision 1.1.5.2  1993/05/12  02:59:00  jd
# 	Initial 486 port.
# 	[1993/05/12  02:58:40  jd]
# 
# Revision 1.1.3.2  1992/12/30  13:26:53  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:43:16  zeliff]
# 
# Revision 1.1  1992/01/19  15:24:29  devrcs
# 	Initial revision
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
# Makefile for building gda

VPATH		= ../stubs:../includes:../../../../export/${target_machine}/usr/include/dce

# programs to build
#PROGRAMS	= gdad gda_clerk x500_addcell
PROGRAMS	= gdad

.if !defined(NO_GDS)
PROGRAMS += x500_addcell
.endif

IDIR		= /bin/

# list of things to install
ILIST		= gdad

# List of libraries used to bind programs
.if ${TARGET_MACHINE}=="ALPHA"
LIBS		= ${${TARGET_MACHINE}_LIBS}  -lxds 
.else
LIBS 	 	= ${${TARGET_MACHINE}_LIBS} 
.endif

# compiler flags
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS        = -I../stubs -I../includes
LDFLAGS		= -L../stubs ${${TARGET_MACHINE}_LDFLAGS}

# List of objects to build gda daemon (gda_clerk uses default)

rdaclif_sstub.c_IDLFLAGS = -no_mepv
rdaclifv0_sstub.c_IDLFLAGS = -no_mepv

IDLFILES	= cds_clerkserver.idl cds_types.idl rdaclifv0.idl \
		rdaclif.idl	
cds_types_ACF =
cds_clerkserver_ACF =

gdad_OFILES     = gda_main.o gda_bind.o gda_list.o gda_process.o \
                  gda_rpcint.o gda_rwl.o gda_update_parent.o gda_svc.o \
                  gda_debug.o dcecdssvc.o dcecdsmsg.o x500process.o \
		  cds_clerkserver_sstub.o cds_types_saux.o 

gda_main.o_CFLAGS=${CFLAGS}
x500process.o_CFLAGS=${CFLAGS}

.if !defined(NO_GDS)
gdad_OFILES += x500_xdsint.o x500_utils.o x500convert.o
gda_main.o_CFLAGS += -DDO_X500
x500process.o_CFLAGS += -DDO_X500
.endif

x500_addcell_OFILES  = x500_addcell.o gda_cell.o x500_utils.o \
                       dcecdssvc.o dcecdsmsg.o 

SAMSFILES=cds.sams
SAMS_TARGETS=dcecdssvc.c dcecdsmsg.c

.include <${RULES_MK}>

.ifdef BLD_SEC
gdad_OFILES    += gda_dacl.o \
                  rdaclifv0_sstub.o rdaclif_sstub.o
.endif


gda_clerk_OFILES = gda_clerk.o \
		   cds_clerkserver_cstub.o cds_types_caux.o


GARBAGE = cds_clerkserver_sstub.c cds_types_saux.c \
	  cds_clerkserver_cstub.c cds_types_caux.c \
          rdaclifv0_sstub.c rdaclif_sstub.c
