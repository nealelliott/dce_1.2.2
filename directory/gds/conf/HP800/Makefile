# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:13:20  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:33  marty]
#
# Revision 1.1.4.1  1995/12/08  15:30:56  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/08/30  23:12 UTC  sommerfeld
# 	Use ${RM} ${_RMFLAGS_} to flush target; avoid DATAFILES problems.
# 	[1994/08/29  15:55 UTC  sommerfeld  /main/dce1.1_clearmakefix/1]
# 
# Revision 1.1.2.2  1994/06/09  18:44:44  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:17:27  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  16:14:29  marrek
# 	Created for April Submission.
# 	[1994/04/26  12:49:30  marrek]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:13:20 $
#

LOCALFILES	= nsapform.dat dirparam.dat
OTHERS		= ${LOCALFILES:S/.dat//g} 
ILIST		= ${LOCALFILES:S/.dat//g} 
nsapform_IDIR	= /share/var/adm/directory/gds/adm/
dirparam_IDIR	= /share/var/adm/directory/gds/conf/

.include <${RULES_MK}>

nsapform:	nsapform.dat
	${RM} ${_RMFLAGS_} nsapform
	${CP} ${nsapform.dat:P} nsapform

dirparam:	dirparam.dat
	${RM} ${_RMFLAGS_} dirparam
	${CP} ${dirparam.dat:P} dirparam
