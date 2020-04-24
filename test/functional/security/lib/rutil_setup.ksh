#!/bin/ksh

#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#

# HISTORY
# $Log: rutil_setup.ksh,v $
# Revision 1.1.2.2  1996/10/03  18:40:22  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  18:14:01  arvind]
#
# Revision 1.1.2.1  1996/08/09  20:11:38  arvind
# 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
# 	[1996/08/09  19:57:17  arvind]
# 
# $EndLog$

if [ -z "${OPTDCELOCAL}" ]
then
      echo "OPTDCELOCAL not set in environment"
      exit -1
fi

if [ -z "${KRB5_TESTER}" ]
then
      echo "KRB5_TESTER not set in environment"
      exit -1
fi

if [ -z "${KRB5_TESTER_PW}" ]
then
      echo "KRB5_TESTER_PW not set in environment"
      exit -1
fi

if [ -z "${KRB5_CELL_ADMIN}" ]
then
      echo "KRB5_CELL_ADMIN not set in environment"
      exit -1
fi

if [ -z "${KRB5_CELL_ADMIN_PW}" ]
then
      echo "KRB5_CELL_ADMIN_PW not set in environment"
      exit -1
fi

cd $OPTDCELOCAL/bin
remove_noexec=false
if [[ ! -h dce_login_noexec ]]
then
   rm -f dce_login_noexec
   ln -s dce_login dce_login_noexec
   remove_noexec=true
fi

export PATH=$PATH:$OPTDCELOCAL/bin:.
export KRB5CCNAME=`dce_login_noexec ${KRB5_CELL_ADMIN} ${KRB5_CELL_ADMIN_PW}`

dcecp -c principal delete ${KRB5_TESTER} >/dev/null 2>&1

dcecp >/tmp/dcecp_out 2>&1 <<EOF
principal create ${KRB5_TESTER}
group add none -member ${KRB5_TESTER}
organization add none -member ${KRB5_TESTER}
account create ${KRB5_TESTER} -group none -organization none -pass ${KRB5_TESTER_PW} -myp ${KRB5_CELL_ADMIN_PW}
quit
EOF

if [ $? -ne 0 ]
then
    echo "Failed to create principal ${KRB5_TESTER} and its account"
    cat /tmp/dcecp_out
    exit -1
fi

if [[ $remove_noexec = true ]]
then
   rm dce_login_noexec
fi

# Add an entry in /etc/passwd file for ${KRB5_TESTER}
# Get required information from registry
dcecp -c principal show ${KRB5_TESTER} > /tmp/dcecp_out 2>&1
if [ $? -ne 0 ]
then
    echo "Failed to query principal ${KRB5_TESTER}"
    cat /tmp/dcecp_out
    exit -1
fi
sed -e 's/^{//g' -e 's/}$//g' /tmp/dcecp_out > /tmp/princ_out
KRB_UID=`grep "^uid " /tmp/princ_out | cut -d' ' -f2`
rm -f /tmp/princ_out

dcecp -c account show ${KRB5_TESTER} > /tmp/dcecp_out 2>&1
if [ $? -ne 0 ]
then
    echo "Failed to query account for ${KRB5_TESTER}"
    cat /tmp/dcecp_out
    exit -1
fi
sed -e 's/^{//g' -e 's/}$//g' /tmp/dcecp_out > /tmp/acc_out
KRB_GROUP=`grep "group " /tmp/acc_out | cut -d' ' -f2`
KRB_HOME=`grep "home " /tmp/acc_out | cut -d' ' -f2`
KRB_SHELL=`grep "shell " /tmp/acc_out | cut -d' ' -f2`
rm -f /tmp/acc_out
if [[ $KRB_SHELL = {} ]]
then
    KRB_SHELL=/bin/sh
fi

dcecp -c group show ${KRB_GROUP} > /tmp/dcecp_out 2>&1
if [ $? -ne 0 ]
then
    echo "Failed to query group ${KRB_GROUP}"
    cat /tmp/dcecp_out
    exit -1
fi
sed -e 's/^{//g' -e 's/}$//g' /tmp/dcecp_out > /tmp/grp_out
KRB_GID=`grep "^gid " /tmp/grp_out | cut -d' ' -f2`
rm -f /tmp/princ_out

cp /etc/passwd /etc/passwd.rutil
sed -e `eval 'echo /$KRB5_TESTER/d'` /etc/passwd > /tmp/passwd.$$
echo "$KRB5_TESTER:*:$KRB_UID:$KRB_GID::$KRB_HOME:$KRB_SHELL" >> /tmp/passwd.$$
cp /tmp/passwd.$$ /etc/passwd
rm -f /tmp/passwd.$$

cp /etc/group /etc/group.rutil
sed -e `eval 'echo /^none:/d'` /etc/group > /tmp/group.$$
echo "none::$KRB_GID:$KRB5_TESTER" >> /tmp/group.$$
cp /tmp/group.$$ /etc/group
rm -f /tmp/group.$$

exit 0
