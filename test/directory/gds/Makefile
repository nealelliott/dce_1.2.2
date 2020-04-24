#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.2  1996/03/11  01:46:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:16:25  marty]
#
# Revision 1.1.9.1  1995/12/08  22:11:16  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/25  15:14 UTC  mothra
# 	build non-tet functional tests
# 	[1995/12/08  21:44:46  root]
# 
# Revision 1.1.7.4  1994/07/06  15:09:31  marrek
# 	July 1994 code drop.
# 	[1994/07/06  12:02:58  marrek]
# 
# Revision 1.1.7.3  1994/05/11  13:39:09  marrek
# 	No tst and gdscp here any more. Moved to test/functional.
# 	[1994/05/11  13:38:06  marrek]
# 
# Revision 1.1.7.2  1994/03/23  15:50:42  keutel
# 	gdscp added
# 	[1994/03/23  13:20:13  keutel]
# 
# 	new code drop
# 	[1994/03/21  14:55:53  keutel]
# 
# Revision 1.1.7.1  1994/02/22  19:01:49  marrek
# 	Code drop for dce1.1.
# 	[1994/02/07  11:54:49  marrek]
# 
# Revision 1.1.5.3  1993/02/22  20:44:48  treff
# 	DCT @ OSF
# 	Added "DATA_SUBDIRS = " to move datafiles
# 	into obj tree, where they belong
# 	[1993/02/22  20:44:29  treff]
# 
# Revision 1.1.5.2  1993/01/04  17:03:04  htf
# 	Embedded copyright notice
# 	[1993/01/04  16:15:19  htf]
# 
# Revision 1.1.2.2  1992/06/11  21:51:13  melman
# 	New GDS test code drop.
# 	[1992/06/11  21:44:44  melman]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.9.2 $ $Date: 1996/03/11 01:46:03 $
#

.ifdef BUILD_FUNC_TESTS
SUBDIRS		= gdscp
.else
SUBDIRS		= mavrostest
DATA_SUBDIRS	= mavrostest
.endif

.include <${RULES_MK}>
