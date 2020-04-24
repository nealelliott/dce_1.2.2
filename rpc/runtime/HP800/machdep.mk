#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# @HP_COPYRIGHT@
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.2.2  1996/03/09  20:45:24  marty
# 	Add OSF copyright
# 	[1996/03/09  20:24:54  marty]
#
# Revision 1.1.2.1  1995/12/08  00:16:13  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/07/14  19:07 UTC  tatsu_s
# 	Submitted the fix for CHFts15685.
# 
# 	HP revision /main/HPDCE02/tatsu_s.local_rpc.b4/1  1995/07/14  18:32 UTC  tatsu_s
# 	Removed some exceptions.
# 
# 	HP revision /main/HPDCE02/1  1995/06/29  22:01 UTC  rps
# 	optimization
# 
# 	HP revision /main/rps_moop/1  1995/06/29  03:45 UTC  rps
# 	optimization
# 	[1995/12/07  23:56:55  root]
# 
# $EndLog$
#

# safe to optimize
CC_OPT_LEVEL=${CC_OPT_HIGH}

# except these until fixed...
cnassoc.o_CC_OPT_LEVEL=${CC_OPT_LOW}
cncall.o_CC_OPT_LEVEL=${CC_OPT_LOW}
cnnet.o_CC_OPT_LEVEL=${CC_OPT_LOW}
cnrcvr.o_CC_OPT_LEVEL=${CC_OPT_LOW}
cnxfer.o_CC_OPT_LEVEL=${CC_OPT_LOW}
comnlsn.o_CC_OPT_LEVEL=${CC_OPT_LOW}
krbclt.o_CC_OPT_LEVEL=${CC_OPT_LOW}
rpclist.o_CC_OPT_LEVEL=${CC_OPT_LOW}
rpctimer.o_CC_OPT_LEVEL=${CC_OPT_LOW}
