#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.2  1996/03/09  23:13:58  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:19  marty]
#
# Revision 1.1.11.1  1995/12/08  16:04:10  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:15:32  root]
# 
# Revision 1.1.9.1  1994/02/22  17:41:11  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  10:55:39  marrek]
# 
# Revision 1.1.7.2  1993/08/11  15:00:53  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:00:20  marrek]
# 
# Revision 1.1.5.3  1992/12/31  20:46:53  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:33:14  bbelch]
# 
# Revision 1.1.5.2  1992/09/29  21:11:47  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:26:44  treff]
# 
# Revision 1.1.2.2  1992/06/01  20:58:13  melman
# 	New GDS merged drop
# 	[1992/05/28  21:42:18  melman]
# 
# Revision 1.1  1992/01/19  15:14:00  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/09 23:13:58 $
#

ILIST		= d20admin.cat shproc.cat d20adm.cat
IDIR		= /nls/msg/De/
OBJECTS		= d20adm.cat d20admin.cat shproc.cat

.include <${RULES_MK}>

d20adm.cat: d20adm.msg
	    ${GENCAT} d20adm.cat d20adm.msg

d20admin.cat: d20admin.msg
	    ${GENCAT} d20admin.cat d20admin.msg

shproc.cat: shproc.msg
	    ${GENCAT} shproc.cat shproc.msg

