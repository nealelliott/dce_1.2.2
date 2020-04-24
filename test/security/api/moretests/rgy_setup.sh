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
# $Log: rgy_setup.sh,v $
# Revision 1.1.6.2  1996/03/11  02:35:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:03  marty]
#
# Revision 1.1.6.1  1995/12/11  21:49:57  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:53  root]
# 
# Revision 1.1.4.3  1993/02/25  22:08:02  cuti
# 	Add "site -u" to rgy_edit commands to make sure working on master site.
# 	[1993/02/25  16:33:07  cuti]
# 
# Revision 1.1.4.2  1992/12/31  17:32:42  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:57:59  htf]
# 
# Revision 1.1.2.2  1992/05/05  21:35:13  cuti
# 	 Check the existence of flintstone accout. if it exits, remove it, then create it.
# 	[1992/05/05  16:18:28  cuti]
# 
# Revision 1.1  1992/01/19  04:08:26  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 
  
#set -x 

PASSWD=-dce-

if [ $1 ]
then
	PASSWD=$1
fi

echo "Checking rgy database. Please wait."

echo "site -u" > /tmp/rgy_edit.script
echo "do p" >>/tmp/rgy_edit.script
echo "v" >>/tmp/rgy_edit.script
echo "q" >>/tmp/rgy_edit.script
/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script >/tmp/rgy_output
EXIT=$?

if [ $EXIT -ne "0" ]
then
    echo "Something appears to be wrong with the rgy database. Please"
    echo "start with a fresh database and/or run setup_rgy.sh first."
    exit $EXIT
fi

grep -s flintstone /tmp/rgy_output
EXIT=$?
if [ $EXIT -eq "0" ]
then  
# Delete flintstone account and principal domains
echo "site -u" > /tmp/rgy_edit.script 
echo "del -p flintstone" >> /tmp/rgy_edit.script 
echo "y" >> /tmp/rgy_edit.script 
echo "do p" >> /tmp/rgy_edit.script
echo "del flintstone" >> /tmp/rgy_edit.script
echo "y"  >> /tmp/rgy_edit.script
echo "q"  >> /tmp/rgy_edit.script 
/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script >/tmp/rgy_output
fi

# Add flintstone and chowdah account
echo "site -u" > /tmp/rgy_edit.script
echo "do p" >>/tmp/rgy_edit.script
echo "add flintstone -f \"Fred Flintstone\"" >>/tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add flintstone -g none -o none -mp $PASSWD -pw yabadabado -m \"This space for rent\"" >>/tmp/rgy_edit.script
echo "q" >>/tmp/rgy_edit.script
/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script
EXIT=$?  
     
# check the existence of flintstone

if [ $EXIT -eq "0" ] 
then  
    echo "site -u" > /tmp/rgy_edit.script
    echo "do acc" >>/tmp/rgy_edit.script
    echo "v" >>/tmp/rgy_edit.script
    echo "q" >>/tmp/rgy_edit.script
    /opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script >/tmp/rgy_output
    grep -s flintstone /tmp/rgy_output
    EXIT=$?
    if [ $EXIT -ne "0" ]
        then
        echo "Something appears to be wrong with the rgy database. Please"
        echo "start with a fresh database and/or run setup_rgy.sh first."
        exit $EXIT
    fi
fi

