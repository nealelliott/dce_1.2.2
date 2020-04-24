#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/09  23:13:59  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:20  marty]
#
# Revision 1.1.12.1  1995/12/08  16:04:13  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:15:33  root]
# 
# Revision 1.1.10.2  1994/08/08  13:21:01  marrek
# 	Fix for OT11045.
# 	[1994/08/08  13:19:03  marrek]
# 
# Revision 1.1.10.1  1994/02/22  17:41:12  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  10:55:48  marrek]
# 
# Revision 1.1.8.2  1993/08/11  15:01:40  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:00:29  marrek]
# 
# Revision 1.1.6.5  1992/12/31  20:46:55  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:33:17  bbelch]
# 
# Revision 1.1.6.4  1992/11/27  17:42:40  marrek
# 	November 1992 code drop
# 	[1992/11/23  12:40:46  marrek]
# 
# Revision 1.1.6.3  1992/09/29  21:11:49  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:26:53  treff]
# 
# Revision 1.1.2.2  1992/06/01  20:58:26  melman
# 	New GDS merged drop
# 	[1992/05/28  21:42:23  melman]
# 
# Revision 1.1  1992/01/19  15:14:03  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.12.2 $ $Date: 1996/03/09 23:13:59 $
#

OBJECTS	= gdssysadm.cat gdsproc.cat gdsditadm.cat gdsdithelp.cat gdssyshelp.cat gdsclhelp.cat
IDIR	= /nls/msg/en_US.ASCII/
ILIST	= ${OBJECTS}

.include <${RULES_MK}>

gdsditadm.cat: d20adm.msg
	       ${GENCAT} gdsditadm.cat $>

gdssysadm.cat: d20admin.msg
	       ${GENCAT} gdssysadm.cat $>

gdsproc.cat: shproc.msg
	     ${GENCAT} gdsproc.cat $>

gdsdithelp.cat: d20admhelp.msg
	       ${GENCAT} gdsdithelp.cat $>

gdssyshelp.cat: d20adminhelp.msg
	       ${GENCAT} gdssyshelp.cat $>

gdsclhelp.cat: d20clhelp.msg
		${GENCAT} gdsclhelp.cat $>
