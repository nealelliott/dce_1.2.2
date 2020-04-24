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
# $Log: Makefile,v $
# Revision 1.1.79.2  1996/03/09  23:12:05  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:18  marty]
#
# Revision 1.1.79.1  1995/12/08  15:34:38  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:50:46  root]
# 
# Revision 1.1.77.9  1994/07/26  15:49:52  jd
# 	Added cds_old.h.
# 	[1994/07/26  15:43:58  jd]
# 
# 	Replaced the idl files that were removed from the INCLUDES line.
# 	[1994/07/26  15:23:49  jd]
# 
# Revision 1.1.77.8  1994/07/25  15:15:49  proulx
# 	Provide interface id to match old CDS exported interface id
# 	[1994/07/25  13:54:04  proulx]
# 
# Revision 1.1.77.7  1994/07/14  11:18:53  kevins
# 	CR 11219 export cds_clerkserver.idl for cross component depend.
# 	[1994/07/14  11:15:30  kevins]
# 
# Revision 1.1.77.6  1994/07/12  19:36:10  zee
# 	Add cprpc_hcell.* to INCLUDES.
# 	[1994/07/12  15:51:20  zee]
# 
# Revision 1.1.77.5  1994/05/20  22:03:20  sommerfeld
# 	remove last vestiges of rdaclif.idl references.
# 	[1994/05/20  21:32:02  sommerfeld]
# 
# 	flush duplicate cprpc.h entry.
# 	Don't export rdaclif.h, as security exports it.
# 	[1994/05/17  19:25:00  sommerfeld]
# 
# Revision 1.1.77.4  1994/04/07  10:22:27  kevins
# 	CR 10304 Export some header files for cross component dependency issues
# 	[1994/04/07  10:18:13  kevins]
# 
# Revision 1.1.77.3  1994/03/22  20:00:40  griffin
# 	HCell BL3 support: cprpc_hcell interface.
# 	[1994/03/22  18:20:58  griffin]
# 
# Revision 1.1.77.2  1994/02/02  21:31:09  rousseau
# 	Re-ordered INCLUDES line to fix dependancy problem caused by CR 9864.
# 	[1994/02/02  21:30:53  rousseau]
# 
# Revision 1.1.77.1  1994/02/02  20:31:48  rousseau
# 	Export cprpc.idl for CR 9864.
# 	[1994/02/02  20:30:25  rousseau]
# 
# Revision 1.1.75.1  1993/10/15  21:19:04  ohara
# 	expand the VPATH to use context, needed for ODE2.3 on SVR4
# 	[1993/10/15  16:13:00  ohara]
# 
# Revision 1.1.5.5  1993/01/20  20:52:51  jd
# 	Backed out the fix for #6673. It broke nightly build on RIOS.
# 	[1993/01/20  20:46:04  jd]
# 
# Revision 1.1.5.3  1992/12/30  14:25:49  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  23:34:00  zeliff]
# 
# Revision 1.1.5.2  1992/09/29  19:16:48  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:17:47  weisman]
# 
# Revision 1.1.2.2  1992/03/22  22:40:51  weisman
# 		Big merge with post-1.0 development at Digital.
# 	[1992/03/22  20:38:13  weisman]
# 
# Revision 1.1  1992/01/19  15:24:46  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991. ALL RIGHTS RESERVED.
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
# Makefile for libcdsrpc.a
#

VPATH           = ../../../../export/${target_machine}/usr/include/dce:../../../../export/${context}/usr/include/dce

#include files

IDLFILES	= cds_types.idl \
		  cds_clerkserver.idl \
		  cds_solicit.idl \
		  cprpc.idl \
		  cprpc_clerk.idl \
		  cprpc_server.idl \
		  cds_old.idl \
		  cprpc_hcell.idl


SAMIDL_INCLUDES = cds_solicit.idl cds_types.idl 
SAMS_TARGETS	= ${SAMIDL_INCLUDES}

INCLUDES       =  cds_types.h cds_types.acf  \
                  cds_solicit.h cds_solicit.acf \
                  cprpc_clerk.acf cprpc_server.acf cprpc_hcell.acf \
                  cprpc.h cprpc.idl cprpc_clerk.h cprpc_server.h \
                  cprpc_clerk.idl cprpc_server.idl \
                  cprpc_hcell.idl cprpc_hcell.h cds_clerkserver.acf \
                  cds_clerkserver.idl cds_clerkserver.h cds_old.h

EXPDIR		= /usr/include/dce/

INCFLAGS	= -I../stubs -I../includes

cds_clerkserver_ACF =
cds_types_ACF =
cds_solicit_ACF =
cprpc_clerk_ACF =
cprpc_server_ACF =
cprpc_hcell_ACF =



# compiler flags

CFLAGS		= ${${TARGET_MACHINE}_CFLAGS}

.include <${RULES_MK}>



GARBAGE = cds_clerkserver_cstub.c cds_clerkserver_sstub.c \
          cds_solicit_cstub.c cds_solicit_sstub.c \
	  cds_types_caux.c cds_types_saux.c \
	  cprpc_clerk_cstub.c cprpc_clerk_sstub.c \
	  cprpc_server_cstub.c cprpc_server_sstub.c \
	  cprpc_hcell_cstub.c cprpc_hcell_sstub.c 

	
