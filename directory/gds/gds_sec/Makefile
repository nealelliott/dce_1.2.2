# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:13:46  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:05  marty]
#
# Revision 1.1.4.1  1995/12/08  15:55:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:11:24  root]
# 
# Revision 1.1.2.4  1994/06/21  14:46:49  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:39:34  marrek]
# 
# Revision 1.1.2.3  1994/06/09  18:45:08  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:17:55  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  15:54:33  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:34:41  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:14:45  keutel
# 	creation
# 	[1994/03/21  15:55:55  keutel]
# 
# $EndLog$
# 
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:46 $
# 

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#

#
# For libdce, we take only the files from libgds_sec_OFILES,
# until OSF delivers a complete libdce, that resolves security
# functions. Helmut Baumgaertner must check!
#
libdua_sec_OFILES	= ${libdua_sec.a_OFILES}
libdua_sec.a_OFILES	= dua_sec.o gds_sec_int.o gds_sec_rgy.o \
                          dua_sec_int.o gds_sec_auth_ctx.o \
						  D00_gds_sec_protect.o

#
# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
LIBRARIES = libdua_sec.a libdsa_sec.a

EXPDIR		= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
EXPSHLIB_TARGETS	= ${libdua_sec_OFILES}
INCFLAGS	= -I../h

CFLAGS		+= -DGSS_DEC -DGSS_AVAILABLE -DNEW_GSS_AVAILABLE

libdsa_sec.a_OFILES	= dsa_sec.o dsa_sec_int.o

.include <${RULES_MK}>

.endif
