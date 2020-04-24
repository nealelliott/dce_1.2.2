# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  13:23:42  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:12:14  marty]
#
# Revision 1.1.4.1  1995/12/08  17:45:34  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:24:21  root]
# 
# Revision 1.1.2.3  1994/06/21  14:47:55  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:43:39  marrek]
# 
# Revision 1.1.2.2  1994/06/10  21:14:41  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:53:46  devsrc]
# 
# Revision 1.1.2.1  1994/03/23  15:46:33  keutel
# 	creation
# 	[1994/03/21  13:38:33  keutel]
# 
# $EndLog$
#
#
#
# @SNI_COPYRIGHT@
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:42 $
#

LIBRARIES		= libtpdu.a

libtpdu.a_EXPDIR	= /usr/lib/

EXPLIB_TARGETS  	= ${LIBRARIES:S/^/export_/g}

INCFLAGS		= -I../incl -I../../h
CFLAGS			+= -DRP0003_HP ${${TARGET_MACHINE}_CFLAGS}

libtpdu.a_OFILES = tpdu.o  tpdu0.o  tpdu1.o

.include <${RULES_MK}>
