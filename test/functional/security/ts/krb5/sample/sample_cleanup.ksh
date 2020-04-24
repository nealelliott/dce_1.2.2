#!/bin/ksh

#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#

# HISTORY
# $Log: sample_cleanup.ksh,v $
# Revision 1.1.2.2  1996/07/09  15:02:14  arvind
# 	OSF DCE 1.2.2 Drop 3
# 	[1996/07/09  14:49:02  arvind]
#
# Revision 1.1.2.1  1996/06/04  22:10:05  arvind
# 	DCE1.2.2 Drop 2 to OSF
# 	[1996/06/04  21:29:30  arvind]
# 
# $EndLog$

CELL_ADMIN=cell_admin
CELL_ADMIN_PW=-dce-

if [ -z "${KRB5_SAMPLE_SERVER_HOST}" ]
then
      KRB5_SAMPLE_SERVER_HOST=`uname -n`
fi

fully_qual_host=`nslookup ${KRB5_SAMPLE_SERVER_HOST} | grep "Name: " | awk '{print $2}'`

if [ -z "$fully_qual_host" ]
then
      fully_qual_host=${KRB5_SAMPLE_SERVER_HOST}
fi

if [ -z "${KRB5_SAMPLE_SERVICE}" ]
then
      KRB5_SAMPLE_SERVICE=sample
fi

test_principal=${KRB5_SAMPLE_SERVICE}/${fully_qual_host}

cd /opt/dcelocal/bin
remove_noexec=false
if [[ ! -h dce_login_noexec ]]
then
   rm -f dce_login_noexec
   ln -s dce_login dce_login_noexec
   remove_noexec=true
fi

export PATH=$PATH:/opt/dcelocal/bin:.
export KRB5CCNAME=`dce_login_noexec ${CELL_ADMIN} ${CELL_ADMIN_PW}`

rgy_edit -update >/dev/null 2>&1 <<EOF
domain principal
del ${test_principal}
ktd -p ${test_principal} -v 1
quit
EOF

if [[ $remove_noexec = true ]]
then
   rm dce_login_noexec
fi

