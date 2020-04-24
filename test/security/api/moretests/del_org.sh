#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: del_org.sh,v $
# Revision 1.1.7.2  1996/03/11  02:34:21  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:20  marty]
#
# Revision 1.1.7.1  1995/12/11  21:48:09  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:10  root]
# 
# Revision 1.1.4.3  1993/03/24  21:45:21  weisman
# 	Resubmit broken Sue Cuti submission.
# 	[1993/03/24  21:44:29  weisman]
# 
# Revision 1.1.5.3  1993/03/22  15:48:49  cuti
# 	Add "-up" to rgy_edit, so update will go to master only.
# 
# Revision 1.1.4.2  1992/12/31  17:28:37  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:54:06  htf]
# 
# Revision 1.1.2.2  1992/05/14  21:20:16  cuti
# 	*** empty log message ***
# 	[1992/05/14  20:47:08  cuti]
# 
# 	Initial file creation
# 	[1992/05/12  17:51:02  cuti]
# 
# $EndLog$

    
#set -x

echo "do o" > /tmp/add_org.script
echo "del org" >> /tmp/add_org.script
echo "q" >> /tmp/add_org.script
           
/opt/dcelocal/bin/rgy_edit -up < /tmp/add_org.script > /tmp/add_org.out

EXIT=$? 

if [ $EXIT = 0 ]
then
	echo "\"org \"name removed from rgy" 
fi

exit $EXIT
