# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.4  1996/10/15  21:18:40  arvind
# 	OSF DCE 1.2.2 Drop 4.5
# 	[1996/10/15  21:13:04  arvind]
#
# Revision 1.1.2.3  1996/10/03  18:40:10  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  18:13:52  arvind]
# 
# Revision 1.1.2.2  1996/08/09  20:11:27  arvind
# 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
# 	[1996/08/09  19:57:09  arvind]
# 
# $EndLog$

.if ${TARGET_MACHINE} == "RIOS"
# If OBJECTDIR is a relative path
VPATH           = ${SOURCEBASE}/${OBJECTDIR}/admin/expect_dce
.else
# If OBJECTDIR is an absolute path
VPATH           = ${OBJECTDIR}/admin/expect_dce
.endif

TET_FILES       = test_setup.tcl default.exp rutil_setup.ksh gg.functions \
		  gg_setup.sh
IDIR            = /test/tet/functional/security/lib/
ILIST           = ${TET_FILES} expect
DATAFILES       = ${TET_FILES}
IMODE           = 755

.include <${RULES_MK}>

