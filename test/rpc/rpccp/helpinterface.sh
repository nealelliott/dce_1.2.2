#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: helpinterface.sh,v $
# Revision 1.1.4.2  1996/03/11  02:20:33  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:56:33  marty]
#
# Revision 1.1.4.1  1995/12/11  19:36:26  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:19:10  root]
# 
# Revision 1.1.2.2  1993/01/11  21:14:58  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  14:55:38  bbelch]
# 
# Revision 1.1  1992/01/19  03:41:18  devrcs
# 	Initial revision
# 
# $EndLog$
#

rpccp help | grep "rpccp operation" || { echo "help option failed"; exit 1; }
rpccp invalid | grep "rpccp operation" || { echo "invalid option failed"; exit 1; }
rpccp show entry | grep "show entry" || { echo "show entry option failed"; exit 1; }
rpccp show group | grep "show group" || { echo "show group option failed" ;exit 1; }
rpccp show profile| grep "show profile" || { echo "show profile option failed" ;exit 1; }
rpccp show server | grep "show server" || { echo "show server option failed" ;exit 1; }
rpccp show mapping | grep "show mapping" || { echo "show mapping option failed" ;exit 1; }
rpccp add entry | grep "add entry" || { echo "add entry option failed" ;exit 1; }
rpccp add member | grep "add member" || { echo "add member option failed" ;exit 1; }
rpccp add element | grep "add element" || { echo "add element option failed" ;exit 1; }
rpccp remove entry | grep "remove entry" || { echo "remove mapping option failed" ;exit 1; }
rpccp remove group | grep "remove group" || { echo "remove mapping option failed" ;exit 1; }
rpccp remove member | grep "remove member" || { echo "remove mapping option failed" ;exit 1; }
rpccp remove profile | grep "remove profile" || { echo "remove mapping option failed" ;exit 1; }
rpccp remove element | grep "remove element" || { echo "remove mapping option failed" ;exit 1; }
rpccp remove mapping | grep "remove mapping" || { echo "remove mapping option failed" ;exit 1; }
rpccp show | grep "show <RPC object>" || { echo "show option failed" ;exit 1; }
rpccp add | grep "add <RPC object>" || { echo "add option failed" ;exit 1; }
rpccp remove | grep "remove <RPC object>" || { echo "remove option failed" ;exit 1; }
echo " "
echo "rpccp Help Interface test was successful!"
echo " "
echo " "

