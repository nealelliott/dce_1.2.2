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
# $Log: runtsh.sh,v $
# Revision 1.1.4.2  1996/03/11  02:25:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:58:50  marty]
#
# Revision 1.1.4.1  1995/12/11  20:00:48  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:41:55  root]
# 
# Revision 1.1.2.2  1993/01/11  22:07:55  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:01:14  bbelch]
# 
# Revision 1.1  1992/01/19  03:39:27  devrcs
# 	Initial revision
# 
# $EndLog$
#
../tsh/testsh interface.tsh network.tsh binding.tsh server.tsh
sleep 5
runsrvrif.sh
