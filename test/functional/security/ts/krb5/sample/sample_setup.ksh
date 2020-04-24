#!/bin/ksh

#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#

# HISTORY
# $Log: sample_setup.ksh,v $
# Revision 1.1.2.2  1996/07/09  15:02:25  arvind
# 	OSF DCE 1.2.2 Drop 3
# 	[1996/07/09  14:49:08  arvind]
#
# Revision 1.1.2.1  1996/06/04  22:10:17  arvind
# 	DCE1.2.2 Drop 2 to OSF
# 	[1996/06/04  21:29:45  arvind]
# 
# $EndLog$


CELL_ADMIN=cell_admin
CELL_ADMIN_PW=-dce-

if [ -z "${KRB5_SAMPLE_SERVER_HOST}" ]
then
      echo "KRB5_SAMPLE_SERVER_HOST not set in environment"
      exit -1
fi

fully_qual_host=`nslookup ${KRB5_SAMPLE_SERVER_HOST} | grep "Name: " | awk '{print $2}'`

if [ -z "$fully_qual_host" ]
then
      fully_qual_host=${KRB5_SAMPLE_SERVER_HOST}
fi

if [ -z "${KRB5_SAMPLE_SERVICE}" ]
then
      echo "KRB5_SAMPLE_SERVICE not set in environment"
      exit -1
fi

if [ -z "${KRB5_SAMPLE_PW}" ]
then
      echo "KRB5_SAMPLE_PW not set in environment"
      exit -1
fi

test_principal=${KRB5_SAMPLE_SERVICE}/${fully_qual_host}
test_passwd=${KRB5_SAMPLE_PW}

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

rgy_edit -update >/tmp/rgy_edit_out 2>&1 <<EOF
domain principal
add ${test_principal}
domain account
add ${test_principal} -g none -o none -pw ${test_passwd} -mp ${CELL_ADMIN_PW}
ktadd -p ${test_principal} -pw ${test_passwd}
quit
EOF

if [ $? -ne 0 ]
then
    echo "Failed to create account for principal ${test_principal}"
    cat /tmp/rgy_edit_out
    exit -1
fi

KRB5CCNAME=`dce_login_noexec ${test_principal} ${test_passwd}`
rm -f /tmp/sample.env
echo "KRB5CCNAME=${KRB5CCNAME}" > /tmp/sample.env

if [[ $remove_noexec = true ]]
then
   rm dce_login_noexec
fi

