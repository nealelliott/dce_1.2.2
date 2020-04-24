#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dacl_setup.sh,v $
# Revision 1.1.10.2  1996/03/11  01:44:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:44  marty]
#
# Revision 1.1.10.1  1995/12/08  22:09:40  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:12  root]
# 
# Revision 1.1.7.1  1994/06/10  20:18:52  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:58:13  devsrc]
# 
# Revision 1.1.5.1  1993/10/28  22:01:40  pwang
# 	Back out the work around of  OT#7759 and replace -or with -o.
# 	[OT#9288]
# 
# Revision 1.1.2.2  1993/08/26  21:53:53  pwang
# 	Renamed from dacl_start.sh to match with documentation [OT#8083]
# 	[1993/08/26  21:33:54  pwang]
# 
# $EndLog$

# MODIFICATION HISTORY:
# 1993/06/17  sommerfeld comment out create and acl_edit of 
#			 third clearinghouse since it is 
#			 frequently unusable (OT7759),
# 	   	  	 use default clearinghouse for now
# 			 redirect error output from acl_edit -io and -ic
# 	   		 for multiple passes of tests
# 1993/02/24 jd # 	Script changes.

cellpw="-dce-"

for i in $*
do
    case $i in
        -ch1)  ch1=$2; shift;shift;;
        -ch2)  ch2=$2; shift;shift;;
        -chd)  chd=$2; shift;shift;;
    esac
done

if [ x${ch1} = x"" -o x${ch2} = x"" -o x${chd} = x"" ]
then
    echo "$0 : -ch1 clearinghouse1 -ch2 clearinghouse2 -chd default_clearinghouse"
    exit
fi



rgy_edit <<RGY_EDIT_EOF
do p
add notroot
do acc
add  notroot -g none -o none -pw $cellpw -mp $cellpw
RGY_EDIT_EOF



cdscp create clearinghouse /.:/${ch1} 
cdscp set dir /.: to skulk
cdscp create clearinghouse /.:/${ch2}
cdscp set dir /.: to skulk


acl_edit /.:/${ch1} -m user:notroot:wc
acl_edit /.:/${ch2} -m user:notroot:wc
acl_edit /.:/${chd} -m user:notroot:wc
acl_edit /.: -m user:notroot:ri


acl_edit /.: -io -k 2>/dev/null
acl_edit /.: -ic -k 2>/dev/null

