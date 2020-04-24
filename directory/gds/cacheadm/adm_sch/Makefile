#
#  @OSF_COPYRIGHT@
#  COPYRIGHT NOTICE
#  Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
#  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
#  src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.2  1996/03/09  23:13:11  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:23  marty]
#
# Revision 1.1.10.1  1995/12/08  15:28:28  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:40  root]
# 
# Revision 1.1.8.1  1994/02/22  16:03:45  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:56:08  marrek]
# 
# Revision 1.1.6.2  1993/08/11  07:54:33  marrek
# 	July 1993 code drop.
# 	[1993/08/02  12:16:58  marrek]
# 
# Revision 1.1.4.2  1992/12/31  18:49:06  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:14:45  bbelch]
# 
# Revision 1.1.2.2  1992/06/01  23:33:43  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  20:39:04  zeliff]
# 
# Revision 1.1  1992/01/19  15:14:22  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:13:11 $
#

LIBRARIES	= libadmschc.a
EXPDIR	= /usr/lib/
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
INCFLAGS	= -I../../h
# for picking up sources
VPATH		= ../../adm/adm_sch
libadmschc.a_OFILES = \
            d20transf.o d20read.o \
	    d20sadmsch.o 

.include <${RULES_MK}>

