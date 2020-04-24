#!/bin/csh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: rpccp_profile.sh,v $
# Revision 1.1.4.2  1996/03/09  23:26:24  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:00  marty]
#
# Revision 1.1.4.1  1995/12/08  00:15:56  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:56:45  root]
# 
# Revision 1.1.2.3  1993/01/03  22:37:27  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:54:28  bbelch]
# 
# Revision 1.1.2.2  1992/12/23  19:41:42  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:55:19  zeliff]
# 
# Revision 1.1  1992/01/19  03:14:09  devrcs
# 	Initial revision
# 
# $EndLog$
#
# Set up some environment variables
#
setenv RPC_DEFAULT_ENTRY_SYNTAX $1
setenv NLSPATH $2/%N
setenv if1 d5c89800-6dae-11c9-a1c1-08002b102989,0000.0000
setenv if2 e9eb0340-6dae-11c9-823d-08002b102989,0000.0000
setenv if21 e9eb0340-6dae-11c9-823d-08002b102989,0001.0000
setenv if10 d5c89800-6dae-11c9-a1c1-08002b102989,0001.0000
setenv if11 d5c89800-6dae-11c9-a1c1-08002b102989,0001.0001
setenv if20 d5c89800-6dae-11c9-a1c1-08002b102989,0002.0000
setenv if30 d5c89800-6dae-11c9-a1c1-08002b102989,0003.0000
setenv obj1 fbe696e0-6dae-11c9-b093-08002b102989
setenv obj2 02d52fc0-6daf-11c9-b958-08002b102989
setenv b1 ncadg_ip_udp:127.0.0.1'[1234]'
setenv b2 ncacn_dnet_nsp:4.133'[22]'
setenv b3 ncadg_ip_udp:16.20.16.54'[1250]'
setenv b4 ncadg_ip_udp:16.20.16.54'[1251]'
setenv b5 ncadg_ip_udp:16.20.16.54'[1252]'
setenv b6 ncadg_ip_udp:16.20.16.54'[1253]'
#
rpccp add_element nstest/profile1 -i if1 -m nstest/entry1 -a IF1 -p 0
rpccp add_element nstest/profile2 -i if1 -m nstest/entry2 -a IF1 -p 0
rpccp add_element nstest/profile -i if1 -m nstest/profile1 -a IF1 -p 0
rpccp add_element nstest/profile -i if1 -m nstest/profile2 -a IF1 -p 1
