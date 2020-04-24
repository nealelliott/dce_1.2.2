#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# @HP_COPYRIGHT@
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.2.2  1996/03/09  20:45:42  marty
# 	Add OSF copyright
# 	[1996/03/09  20:25:07  marty]
#
# Revision 1.1.2.1  1995/12/08  17:47:41  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/rps_moop/1  1995/06/29  03:46 UTC  rps
# 	optimization
# 	[1995/12/08  17:19:27  root]
# 
# $EndLog$
#

# archive is generated for programs, not shared libraries
NO_ZFLAG=

# safe to optimize
CC_OPT_LEVEL=${CC_OPT_HIGH}
