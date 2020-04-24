# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: test_setup.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:08:39  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:32  marty]
#
# Revision 1.1.4.2  1995/12/13  15:23:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:50:34  root]
# 
# Revision 1.1.2.1  1994/09/28  21:36:52  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:05:53  melman]
# 
# Revision 1.1.2.4  1994/06/10  20:01:29  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:51:59  devsrc]
# 
# Revision 1.1.2.3  1994/05/06  15:38:27  rousseau
# 	Removed unneeded definitions.
# 	[1994/05/06  15:37:45  rousseau]
# 
# Revision 1.1.2.2  1994/03/07  23:01:24  rousseau
# 	Ignore signal 26 (SIGVTALARM).
# 	[1994/03/07  23:01:10  rousseau]
# 
# Revision 1.1.2.1  1994/02/10  23:15:15  rousseau
# 	Initial test drop.
# 	[1994/02/10  23:14:53  rousseau]
# 
# $EndLog$

# Boiler plate setup for DCECP functional tests.

# Largest system supported signal + 1
set TET_NSIG 33


