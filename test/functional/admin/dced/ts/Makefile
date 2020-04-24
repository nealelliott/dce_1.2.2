# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.10.1  1996/10/03  18:37:57  arvind
# 	Submit fix for CHFts19700
# 	[1996/08/14  19:45 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
#
# 	merge makefile IDIR fix to mainline
# 	[1996/07/24  17:33 UTC  nfd  /main/HPDCE02/nfd_fix_tests/1]
#
# 	fix IDIR value
# 	[1996/03/20  01:22 UTC  jss  /main/HPDCE02/5]
#
# 	merge ILIST additon to mainline
# 	[1996/02/28  19:52 UTC  nfd  /main/HPDCE02/3]
#
# 	Do Ralph's job for him, again!
# 	[1996/02/29  18:26 UTC  rmm  /main/HPDCE02/nfd_davis_rev/2]
#
# 	Fix IDIR to so that datafiles appear in the ts directory.
# 	[1996/02/28  19:52 UTC  nfd  /main/HPDCE02/3]
#
# 	merge ILIST additon to mainline
# 	[1996/02/28  19:37 UTC  nfd  /main/HPDCE02/nfd_davis_rev/1]
#
# 	add ILIST value to Makefile
# 	[1996/01/15  23:07 UTC  jss  /main/HPDCE02/2]
#
# 	Merge in all changes to dced tests for new ACLs.
# 	[1996/01/15  22:53 UTC  jss  /main/HPDCE02/jss_dced_acl/1]
#
# 	Added common datafiles for dced tests.
# 	[1995/08/22  18:28 UTC  mgm  /main/HPDCE02/1]
#
# 	change order of subdirs
# 	[1994/10/18  21:13:26  max  1.1.4.1]
#
# Revision 1.1.8.3  1996/03/11  02:29:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:17  marty]
# 
# Revision 1.1.8.2  1995/12/11  15:17:50  root
# 	Submit
# 	[1995/12/11  14:32:57  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:18:08  root]
# 
# Revision 1.1.4.1  1994/10/18  21:13:26  max
# 	Extend build to util_api subdir
# 	[1994/10/18  21:04:04  max]
# 
# Revision 1.1.2.1  1994/09/28  21:47:30  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:13  melman]
# 
# Revision 1.1.2.2  1994/06/17  18:44:40  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:32:02  devsrc]
# 
# Revision 1.1.2.1  1994/05/18  21:21:03  ganni
# 	Initial Version
# 	[1994/05/18  20:54:25  ganni]
# 
# $EndLog$

DATAFILES	= dced.data dced.functions
IDIR		= /test/tet/functional/admin/dced/ts/
ILIST		= dced.data dced.functions
SUBDIRS		= util_api api
EXPINC_SUBDIRS	= api
DATA_SUBDIRS	= ${SUBDIRS}

.include <${RULES_MK}>
