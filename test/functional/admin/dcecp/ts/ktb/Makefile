# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/11  02:16:30  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:04  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:22  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/11/10  14:26 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/10  14:25 UTC  truitt
# 	Add a keytab cleanup file.
# 	[1995/12/11  15:53:25  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:16  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:30  melman]
# 
# Revision 1.1.2.5  1994/06/22  20:38:23  baum
# 	Added ktb_vars.tcl
# 	[1994/06/22  20:38:13  baum]
# 
# Revision 1.1.2.4  1994/06/10  20:13:11  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:54:21  devsrc]
# 
# Revision 1.1.2.3  1994/05/19  23:06:20  baum
# 	Resync with changes made to commands and command output
# 	[1994/05/19  23:05:48  baum]
# 
# Revision 1.1.2.2  1994/05/11  14:46:52  rousseau
# 	Updated to new install path and removed bad use of SCRIPTS (CR 10601).
# 	[1994/05/11  14:46:18  rousseau]
# 
# Revision 1.1.2.1  1994/05/10  22:21:50  baum
# 	Initial checkin
# 	[1994/05/10  22:21:37  baum]
# 
# $EndLog$

KEYTAB_FILES 	= keytab_catalog_N.tcl keytab_catalog_P.tcl \
		  keytab_create_N.tcl keytab_create_P.tcl \
		  keytab_delete_N.tcl keytab_delete_P.tcl \
		  keytab_list_N.tcl keytab_list_P.tcl \
		  keytab_remove_N.tcl keytab_remove_P.tcl \
		  keytab_show_N.tcl keytab_show_P.tcl \
                  keytab_add_N.tcl keytab_add_P.tcl \
		  ktb_misc.tcl ktb_setup.tcl ktb_cleanup.tcl ktb_vars.tcl

IDIR		= /test/tet/functional/admin/dcecp/ts/ktb/
ILIST		= ${KEYTAB_FILES}
DATAFILES	= ${KEYTAB_FILES}
IMODE		= 755

.include <${RULES_MK}>
