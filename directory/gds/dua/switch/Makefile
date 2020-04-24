# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:13:43  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:02  marty]
#
# Revision 1.1.4.1  1995/12/08  15:55:12  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:10:56  root]
# 
# Revision 1.1.2.3  1994/06/21  14:46:46  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:39:24  marrek]
# 
# Revision 1.1.2.2  1994/06/09  18:45:06  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:17:53  devsrc]
# 
# Revision 1.1.2.1  1994/02/22  19:21:06  marrek
# 	Created for dce1.1.
# 	[1994/02/10  17:08:30  marrek]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:43 $
#

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#
libd21sw_OFILES		=  ${libd21sw.a_OFILES}

libd21sw.a_OFILES = \
		d21_cdsadd.o    d21_cdscom.o  d21_cdslis.o      \
		d21_cdsmis.o    d21_cdsmod.o  d21_cdsrd.o       \
		d21_cdsrem.o    d21_cdssw.o   D00_switch_protect.o

#
# The rest of the Makefile is private for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#

LIBRARIES	= libd21sw.a
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

EXPSHLIB_TARGETS	= ${libd21sw_OFILES}

EXPDIR		= /usr/lib/
INCFLAGS	= -I../../h

.include <${RULES_MK}>

.endif
