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
# $Log: rgy_cleanup.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:54  marty]
#
# Revision 1.1.4.1  1995/12/11  21:49:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:46  root]
# 
# Revision 1.1.2.2  1992/12/31  17:31:58  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:15  htf]
# 
# Revision 1.1  1992/01/19  04:07:17  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 

echo "domain person" >rgy_input
echo "del rocky" >>rgy_input
echo "y" >>rgy_input
echo "quit" >>rgy_input
/etc/rgy_edit <rgy_input 
