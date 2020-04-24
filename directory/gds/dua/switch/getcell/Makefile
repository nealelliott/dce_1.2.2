#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.2  1996/03/09  23:13:44  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:03  marty]
#
# Revision 1.1.10.1  1995/12/08  15:55:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:11:19  root]
# 
# Revision 1.1.8.2  1994/07/12  16:57:41  marty
# 	CR # 10017 - Set permissions on header files to 444 during install pass
# 	[1994/07/12  15:53:22  marty]
# 
# Revision 1.1.8.1  1994/02/22  18:06:55  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  10:39:33  marrek]
# 
# Revision 1.1.6.2  1993/08/11  06:58:18  marrek
# 	July 1993 code drop.
# 	[1993/07/30  11:04:22  marrek]
# 
# Revision 1.1.4.2  1992/12/31  19:49:57  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:27:24  bbelch]
# 
# Revision 1.1.2.2  1992/06/01  19:40:48  zeliff
# 	New files from GDS drop
# 	[1992/05/27  19:37:24  zeliff]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:13:44 $
#

#
# Makefile for the configuration library 
#

LIBRARIES	= libdce_conf.a
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
IDIR		= /usr/include/dce/
EXPDIR		= /usr/include/dce/

INCLUDES	= dce_cf_sts.h dce_cf.h

IMODE		= 444
ILIST		= ${INCLUDES} 

libdce_conf.a_IDIR	= /usr/lib/
libdce_conf.a_EXPDIR	= /usr/lib/

OFILES 		= dce_cf_db.o


.include <${RULES_MK}>

