# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.7.1  1996/08/09  12:14:38  arvind
# 	Add DATA_SUBDIRS.
# 	[1996/08/05  21:00 UTC  psn  /main/DCE_1.2.2/psn_bugfix/1]
#
# 	Add lib.
# 	[1996/07/12  18:52 UTC  psn  /main/psn_krb5/1]
#
# Revision 1.1.5.2  1996/03/11  02:06:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:26  marty]
# 
# Revision 1.1.5.1  1995/12/11  19:51:59  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:37  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:18:00  root]
# 
# Revision 1.1.2.4  1994/08/04  16:15:05  mdf
# 	new security tests.
# 	[1994/08/04  15:45:00  mdf]
# 
# Revision 1.1.2.3  1994/07/19  19:33:56  wray
# 	Make tet-compliant
# 	[1994/07/19  19:24:50  wray]
# 
# Revision 1.1.2.2  1994/06/10  20:44:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:16:22  devsrc]
# 
# Revision 1.1.2.1  1994/05/06  13:50:35  hasbrouc
# 	add grp_override TET directory for group override tests
# 	[1994/05/06  13:40:27  hasbrouc]
# 
# $EndLog$


CFG_TOKEN = 's;%TET_SIG_LEAVE_TOKEN%;12,26;'
HP800_CFG_TOKEN	= 's;%TET_SIG_LEAVE_TOKEN%;12,20;'

SUBDIRS = tools common ts lib

DATA_SUBDIRS  = lib ts
EXPLIB_SUBDIRS  = common


DATAFILES	= tet_code tet_scen.dat
SCRIPTS		= tetexec.cfg
INST_DATAFILES	= tet_code tet_scen tetexec.cfg
TET_ROOT	= /test/tet/functional


ILIST		= ${INST_DATAFILES} 
IDIR		= ${TET_ROOT}/security/


.include <${RULES_MK}>
.SUFFIXES: .cfg

.cfg:
	${SED} -e ${${TARGET_MACHINE}_CFG_TOKEN:U${CFG_TOKEN}}\
	${.IMPSRC} > ${.TARGET}.X
	${MV} -f ${.TARGET}.X ${.TARGET}.cfg
