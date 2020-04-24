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
# Revision 1.1.2.2  1996/03/09  20:45:43  marty
# 	Add OSF copyright
# 	[1996/03/09  20:25:08  marty]
#
# Revision 1.1.2.1  1995/12/08  17:49:49  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/08/21  15:24 UTC  rps
# 	merge
# 
# 	HP revision /main/HPDCE02/rps_moth3/1  1995/08/17  23:10 UTC  rps
# 	OPT_LEVEL => CC_OPT_LEVEL
# 
# 	HP revision /main/rps_moop/1  1995/06/29  03:46 UTC  rps
# 	optimization
# 	[1995/12/08  17:19:54  root]
# 
# $EndLog$
#

# archive is generated for programs, not shared libraries
NO_ZFLAG=

# safe to optimize
CC_OPT_LEVEL=${CC_OPT_HIGH}

# except these until fixed...
rpriv_v1_1_mgrs.o_CC_OPT_LEVEL=${CC_OPT_LOW}
rs_prop_tasks.o_CC_OPT_LEVEL=${CC_OPT_LOW}
rs_rsdb.o_CC_OPT_LEVEL=${CC_OPT_LOW}
rwl.o_CC_OPT_LEVEL=${CC_OPT_LOW}
