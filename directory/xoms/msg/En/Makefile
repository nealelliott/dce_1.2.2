# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  13:25:06  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:20  marty]
#
# Revision 1.1.4.1  1995/12/08  17:50:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:26:32  root]
# 
# Revision 1.1.2.1  1994/06/21  14:39:45  marrek
# 	Created in June 94 code submission.
# 	[1994/06/10  16:04:47  marrek]
# 
# $EndLog$
# 
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:06 $
# 

OBJECTS	= xoserr.cat
IDIR	= /nls/msg/en_US.ASCII/
ILIST	= ${OBJECTS}

.include <${RULES_MK}>

xoserr.cat: xoserr.msg
	       ${GENCAT} xoserr.cat $>
