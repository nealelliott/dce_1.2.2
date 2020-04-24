#!/bin/ksh
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rgy_add.sh,v $
# Revision 1.1.4.2  1996/03/11  02:34:23  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:46  marty]
#
# Revision 1.1.4.1  1995/12/11  15:24:34  root
# 	Submit
# 	[1995/12/11  14:35:43  root]
# 
# Revision 1.1.2.1  1994/07/06  15:10:24  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:21  marrek]
# 
# $EndLog$
#
# $RCSfile: rgy_add.sh,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:23 $
#
# check wether dce_login as cell_admin was performed

klist 1>klist.out  2>/dev/null
grep "Global Principal" klist.out | grep -q cell_admin 

if [ $? -eq 1 ]
	then
	echo "script requires dce_login as cell_admin"
	rm klist.out
	exit 1
fi   

rm klist.out

echo "dce_logged in as cell_admin"


rgy_edit << EOF
ktadd -p gds-dsa1  -pw gds-dsa1
ktadd -p gds-dsa2 -pw gds-dsa2
ktlist 
quit
EOF
