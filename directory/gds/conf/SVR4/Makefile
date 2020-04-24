# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:13:30  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:47  marty]
#
# Revision 1.1.4.1  1995/12/08  15:31:37  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:55:23  root]
# 
# Revision 1.1.2.2  1994/06/09  18:44:51  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:17:36  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  16:14:45  marrek
# 	Created for April Submission.
# 	[1994/04/26  12:49:44  marrek]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:30 $
#

LOCALFILES	= nsapform.dat dirparam.dat
DATAFILES	= ${LOCALFILES:S/.dat//g} 
ILIST		= ${LOCALFILES:S/.dat//g} 
nsapform_IDIR	= /share/var/adm/directory/gds/adm/
dirparam_IDIR	= /share/var/adm/directory/gds/conf/

.include <${RULES_MK}>

nsapform:	nsapform.dat
	-${CP} ${nsapform.dat:P} nsapform

dirparam:	dirparam.dat
	-${CP} ${dirparam.dat:P} dirparam
