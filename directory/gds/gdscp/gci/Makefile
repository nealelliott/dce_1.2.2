# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:13:49  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:07  marty]
#
# Revision 1.1.4.1  1995/12/08  15:56:12  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:11:42  root]
# 
# Revision 1.1.2.3  1994/06/09  18:45:20  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:18:10  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  15:54:42  marrek
# 	Bug fixes.
# 	[1994/04/29  09:31:59  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  13:23:47  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:16:03  keutel
# 	creation
# 	[1994/03/21  12:55:32  keutel]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:49 $
#

#
###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#

PROGRAMS	= gdscp

ILIST		= ${PROGRAMS}
IDIR		= /bin/

# Indicate the dependency to the translator library
gdscp:	../gct/libgct.a

gdscp_OFILES	= gcicmdframe.o \
		  gcimisc.o	\
		  gciprint.o	\
		  gciutil.o	\
		  gcivar.o	\
		  gcix500abbr.o	\
		  gcix500obj.o	\
		  gcix500svc.o


gdscp_LIBS	= -lgct -ltcl -lm ${LIBS}

INCFLAGS	= -Ih -I../gct/h -I../../h

.include <${RULES_MK}>
