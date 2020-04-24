#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: kill_secd.sh,v $
# Revision 1.1.6.2  1996/03/11  02:34:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:29  marty]
#
# Revision 1.1.6.1  1995/12/11  21:48:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:20  root]
# 
# Revision 1.1.4.2  1992/12/31  17:29:08  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:54:41  htf]
# 
# Revision 1.1.2.2  1992/05/19  20:17:16  cuti
# 	 Created to robustically kill secd.
# 	[1992/05/19  18:51:32  cuti]
# 
# $EndLog$

#set -x 

echo "stop" > /tmp/kill_secd.script
echo "q" >> /tmp/kill_secd.script

sec_admin <   /tmp/kill_secd.script 

EXIT=$?

exit $EXIT

