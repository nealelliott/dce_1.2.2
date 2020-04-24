#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: add_org.sh,v $
# Revision 1.1.7.2  1996/03/11  02:34:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:18  marty]
#
# Revision 1.1.7.1  1995/12/11  21:48:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:06  root]
# 
# Revision 1.1.4.3  1993/03/24  21:45:18  weisman
# 	Resubmit broken Sue Cuti submission.
# 	[1993/03/24  21:44:11  weisman]
# 
# Revision 1.1.5.2  1993/03/22  15:40:32  cuti
# 	Add "-up" to rgy_edit, so update will go to master only.
# 
# Revision 1.1.4.2  1992/12/31  17:28:19  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:53:48  htf]
# 
# Revision 1.1.2.2  1992/05/14  21:23:51  cuti
# 	Initial file creation.
# 	[1992/05/12  17:50:16  cuti]
# 
# $EndLog$
     
#set -x

echo "do o" > /tmp/add_org.script
echo "a org" >> /tmp/add_org.script
echo "q" >> /tmp/add_org.script
           
/opt/dcelocal/bin/rgy_edit -up < /tmp/add_org.script > /tmp/add_org.out

EXIT=$? 

if [ $EXIT = 0 ]
then
	echo "\"org \"name added to rgy" 
fi

exit $EXIT
