# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:24:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:35:08  marty]
#
# Revision 1.1.4.1  1995/12/11  15:13:45  root
# 	Submit
# 	[1995/12/11  14:31:21  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:16:02  root]
# 
# Revision 1.1.2.1  1994/09/28  21:44:42  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:13:05  melman]
# 
# Revision 1.1.2.6  1994/06/20  19:52:04  coren
# 	Merged with changes from 1.1.2.5
# 	[1994/06/20  19:51:55  coren]
# 
# 	Added endpoint tests.
# 	[1994/06/16  17:08:59  coren]
# 
# Revision 1.1.2.5  1994/06/17  18:43:35  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:27:16  devsrc]
# 
# Revision 1.1.2.4  1994/05/31  21:39:59  rousseau
# 	Added rpcgroup tests.
# 	[1994/05/31  21:39:06  rousseau]
# 
# Revision 1.1.2.3  1994/05/26  20:20:59  coren
# 	Added rpcentry tests.
# 	[1994/05/26  20:20:36  coren]
# 
# Revision 1.1.2.2  1994/05/26  18:42:07  rousseau
# 	Initial submission.
# 	[1994/05/26  18:39:21  rousseau]
# 
# $EndLog$

ENTRY_FILES	= rpcentry_create_N.tcl rpcentry_create_P.tcl \
                  rpcentry_delete_N.tcl rpcentry_delete_P.tcl \
                  rpcentry_export_N.tcl rpcentry_export_P.tcl \
                  rpcentry_import_N.tcl rpcentry_import_P.tcl \
                  rpcentry_show_N.tcl rpcentry_show_P.tcl \
                  rpcentry_unexport_N.tcl rpcentry_unexport_P.tcl
GROUP_FILES	= rpcgroup_add_N.tcl rpcgroup_add_P.tcl \
		  rpcgroup_create_N.tcl rpcgroup_create_P.tcl \
		  rpcgroup_delete_N.tcl rpcgroup_delete_P.tcl \
		  rpcgroup_import_N.tcl rpcgroup_import_P.tcl \
		  rpcgroup_list_N.tcl rpcgroup_list_P.tcl \
		  rpcgroup_remove_N.tcl rpcgroup_remove_P.tcl
PROFILE_FILES	= rpcprofile_add_N.tcl rpcprofile_add_P.tcl \
		  rpcprofile_create_N.tcl rpcprofile_create_P.tcl \
		  rpcprofile_delete_N.tcl rpcprofile_delete_P.tcl \
		  rpcprofile_import_N.tcl rpcprofile_import_P.tcl \
		  rpcprofile_list_N.tcl rpcprofile_list_P.tcl \
		  rpcprofile_remove_N.tcl rpcprofile_remove_P.tcl \
		  rpcprofile_show_N.tcl rpcprofile_show_P.tcl
ENDPOINT_FILES  = endpoint_create_N.tcl endpoint_create_P.tcl \
                  endpoint_delete_N.tcl endpoint_delete_P.tcl \
                  endpoint_show_N.tcl endpoint_show_P.tcl
IDIR		= /test/tet/functional/admin/dcecp/ts/rpc/
ILIST		= ${ENTRY_FILES} ${GROUP_FILES} ${PROFILE_FILES} \
                  ${ENDPOINT_FILES}
DATAFILES	= ${ENTRY_FILES} ${GROUP_FILES} ${PROFILE_FILES} \
                  ${ENDPOINT_FILES}
IMODE		= 755

.include <${RULES_MK}>
