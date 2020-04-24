#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
#
# HISTORY
# $Log: testacl_drv.ksh,v $
# Revision 1.1.7.1  1996/10/02  20:58:31  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:47:05  damon]
#
# Revision 1.1.2.1  1994/10/11  17:46:01  mckeen
# 	Initial submit
# 	[1994/10/11  17:41:19  mckeen]
# 
# $EndLog$

# A simple test driver for test_acl.epi to test delegation in acls.

check_setup()
{

  error=0

  grep ${USER_A} /etc/passwd > /dev/null 2>&1
  if [ ! $? -eq 0 ]
  then
    echo Error: User accounts not detected in /etc/passwd
  fi

  grep ${GROUP_A1} /etc/group > /dev/null 2>&1
  if [ ! $? -eq 0 ]
  then
    echo Error: Groups not detected in /etc/group
    error=1
  fi

  dcecp -c account show ${USER_A} > /dev/null 2>&1
  if [ ! $? -eq 0 ]
  then
    echo Error: User accounts not detected in cell registry
    error=1
  fi

  if [ error -eq 1 ]
  then
    echo "Errors detected, unable to continue"

cat <<EOF

Please make sure the following users and groups are setup
in the passwd and group files and in the DCE registry.

${USER_A} in groups ${GROUP_A1} and ${GROUP_A2}
${USER_B} in groups ${GROUP_B1} and ${GROUP_B2}

EOF
    exit
  fi

  whence testacl.epi > /dev/null 2>&1
  if [ ! $? -eq 0 ]
  then
    echo Error: Unable to find driver program testacl.epi in path
    exit
  fi


}

print_hdr()
{
echo "---------------------"
echo "Starting Test $*"
}

check_results()
{

if [ $# -eq 2 ]
then
  RESULTS=$1
  EXPECTED=$2
else
  echo "ERROR: Improper number of arguments to check_results"
  return 1
fi

if [ $EXPECTED -eq $SHOULD_GRANT_ACCESS ] 
then
  grep "DACL_SUCCESS" $RESULTS > /dev/null
else
  grep "DACL_ERROR_ACCESS_DENIED" $RESULTS > /dev/null
fi

if [ $? -eq 0 ]
then
  echo "Test result PASS"
  return 0
else
  cat $RESULTS
  echo
  echo "Test result FAIL"
  return 1
fi

}


test_acl()
{

if [ $# -eq 9 ]
then
  USER_TO_SET=$1
  GROUP_TO_SET=$2
  USER_TO_TEST=$3
  GROUP_TO_TEST=$4
  DUSER_TO_TEST=$5
  DGROUPS_TO_TEST=$6
  PERM_TO_REQUEST=$7
  ACL_TO_TEST=$8
  EXPECTED_RESULT=$9
 else
  echo "ERROR: Wrong number of parameters to test_acl function (got $#)"
  return 1
fi

if [ -f $TEST_FILE ] 
then
  echo "$TEST_FILE aleady exists, using it"
else
  touch $TEST_FILE
fi

echo "Setting $TEST_FILE owner to $USER_TO_SET"
chown $USER_TO_SET $TEST_FILE
if [ ! $? -eq 0 ]
then
  echo "Unable to set owner of $TEST_FILE to $USER_TO_SET"
  return 1
fi

echo "Setting $TEST_FILE group to $GROUP_TO_SET"
chgrp $GROUP_TO_SET $TEST_FILE
if [ ! $? -eq 0 ]
then
  echo "Unable to set group of $TEST_FILE to $GROUP_TO_SET"
  return 1
fi

echo "Setting ACLs on $TEST_FILE"
echo dcecp -c "acl replace $TEST_FILE -acl $ACL_TO_TEST"
dcecp -c "acl replace $TEST_FILE -acl $ACL_TO_TEST"
echo dcecp -c "acl show $TEST_FILE"
dcecp -c "acl show $TEST_FILE"
if [ ! $? -eq 0  ] 
then
  echo "Unable to set ACLs on $TEST_FILE"
  return 1
fi

if [ ! -z "$DUSER_TO_TEST" ]
then
  DUSER_TMP="-duser $DUSER_TO_TEST"
fi

if [ ! -z "$DGROUPS_TO_TEST" ]
then
  DGROUPS_TMP="-dgroup $DGROUPS_TO_TEST"
fi

testacl.epi check -user $USER_TO_TEST -group $GROUP_TO_TEST \
	    $DUSER_TMP $DGROUPS_TMP -perm $PERM_TO_REQUEST \
	    -epifile $TEST_FILE > ${TEST_FILE}.$$

check_results ${TEST_FILE}.$$ $EXPECTED_RESULT
RESULT=$?

rm ${TEST_FILE}.$$

return $RESULT

}

TEST_FILE=${TEST_FILE:-/:/acl_test_file}
SHOULD_GRANT_ACCESS=0
SHOULD_DENY_ACCESS=1

USER_A=user_a
USER_B=user_b
GROUP_A1=group_a1
GROUP_A2=group_a2
GROUP_B1=group_b1
GROUP_B2=group_b2

check_setup

# Args to test_acl - 
#  (DUSER_TO_EST and DGROUPS_TO_TEST can be left as "" to not use)
# USER_TO_SET   GROUP_TO_SET   
# USER_TO_TEST  GROUP_TO_TEST   
# DUSER_TO_TEST DGROUPS_TO_TEST 
# PERM_TO_REQUEST   ACL_TO_TEST   EXPECTED_RESULT

NUMBER_FAIL=0

if [ -z "   " ]
then
print_hdr 1 Simple Positive Test
test_acl  ${USER_A} ${GROUP_A1} ${USER_A} ${GROUP_A1} "" "" rw \
          "{{user_obj rwc} {group_obj r} {other_obj rw}}" \
          $SHOULD_GRANT_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr 2 Simple Negative Test
test_acl  ${USER_B} ${GROUP_B1} ${USER_A} ${GROUP_A1} "" "" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr 3 Positive User Delegate Test IU - RW, DU - RW
test_acl  ${USER_B} ${GROUP_B1} ${USER_A} ${GROUP_A1} \
	  "${USER_B}" "${GROUP_B1}" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r} \
            {user ${USER_A} rw} {user_delegate ${USER_B} rw}}" \
          $SHOULD_GRANT_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr 4 Negative User Delegate Test IU - RW, DU - R
test_acl  ${USER_B} ${GROUP_B1} ${USER_A} ${GROUP_A1} \
	  "${USER_B}" "${GROUP_B1}" rw \
          "{{user_obj rc} {group_obj r} {other_obj r} \
            {user ${USER_A} rw} {user_delegate ${USER_B} r}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr 5 Negative User Delegate Test IU - R, DU - RW
test_acl  ${USER_B} ${GROUP_B1} ${USER_A} ${GROUP_A1} \
	  "${USER_B}" "${GROUP_B1}" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r} \
            {user ${USER_A} r} {user_delegate ${USER_B} rw}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr 6 Positive Group Delegate Test IU - RW, DG - RW
test_acl  ${USER_B} ${GROUP_B1} ${USER_A} ${GROUP_A1} \
          "${USER_B}" "${GROUP_B1}" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r} \
            {user ${USER_A} rw} {group_delegate ${GROUP_B1} rw}}" \
          $SHOULD_GRANT_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr 7 Negative Group Delegate Test IU - RW, DG - R
test_acl  ${USER_B} ${GROUP_B1} ${USER_A} ${GROUP_A1} \
          "${USER_B}" "${GROUP_B1}" rw \
          "{{user_obj rc} {group_obj r} {other_obj r} \
            {user ${USER_A} rw} {group_delegate ${GROUP_B1} r}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr 8 Negative User Delegate Test IU - R, DG - RW
test_acl  ${USER_B} ${GROUP_B1} ${USER_A} ${GROUP_A1} \
          "${USER_B}" "${GROUP_B1}" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r} \
            {user ${USER_A} r} {group_delegate ${GROUP_B1} rw}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

fi

if [ -n "${FOREIGN_CELL}" ]
then

FC=${FOREIGN_CELL}/

print_hdr I1 Intercell Simple Positive Test
test_acl  ${USER_A} ${GROUP_A1} ${FC}${USER_A} ${FC}${GROUP_A1} "" "" rw \
          "{{user_obj rwc} {group_obj r} {other_obj rw} \
	    {foreign_user ${FC}${USER_A} rw}}" \
          $SHOULD_GRANT_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr I2 Intercell Simple Negative Test
test_acl  ${USER_B} ${GROUP_B1} ${USER_A} ${GROUP_A1} "" "" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr I3 Intercell Positive User Delegate Test IU - RW, DU - RW
test_acl  ${USER_B} ${GROUP_B1} ${FC}${USER_A} ${FC}${GROUP_A1} \
	  "${FC}${USER_B}" "${FC}${GROUP_B1}" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r} \
            {foreign_user ${FC}${USER_A} rw} \
	    {foreign_user_delegate ${FC}${USER_B} rw}}" \
          $SHOULD_GRANT_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr I4 Intercell Negative User Delegate Test IU - RW, DU - R
test_acl  ${USER_B} ${GROUP_B1} ${FC}${USER_A} ${FC}${GROUP_A1} \
	  "${FC}${USER_B}" "${FC}${GROUP_B1}" rw \
          "{{user_obj rc} {group_obj r} {other_obj r} \
            {foreign_user ${FC}${USER_A} rw} \
            {user_delegate ${FC}${USER_B} r}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr I5 Intercell Negative User Delegate Test IU - R, DU - RW
test_acl  ${USER_B} ${GROUP_B1} ${FC}${USER_A} ${FC}${GROUP_A1} \
	  "${FC}${USER_B}" "${FC}${GROUP_B1}" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r} \
            {foreign_user ${FC}${USER_A} r} \
            {foreign_user_delegate ${FC}${USER_B} rw}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr I6 Intercell Positive Group Delegate Test IU - RW, DG - RW
test_acl  ${USER_B} ${GROUP_B1} ${FC}${USER_A} ${FC}${GROUP_A1} \
          "${FC}${USER_B}" "${FC}${GROUP_B1}" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r} \
            {foreign_user ${FC}${USER_A} rw} \
            {foreign_group_delegate ${FC}${GROUP_B1} rw}}" \
          $SHOULD_GRANT_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr I7 Intercell Negative Group Delegate Test IU - RW, DG - R
test_acl  ${USER_B} ${GROUP_B1} ${FC}${USER_A} ${FC}${GROUP_A1} \
          "${FC}${USER_B}" "${FC}${GROUP_B1}" rw \
          "{{user_obj rc} {group_obj r} {other_obj r} \
            {foreign_user ${FC}${USER_A} rw} \
            {foreign_group_delegate ${FC}${GROUP_B1} r}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

print_hdr I8 Intercell Negative User Delegate Test IU - R, DG - RW
test_acl  ${USER_B} ${GROUP_B1} ${FC}${USER_A} ${FC}${GROUP_A1} \
          "${FC}${USER_B}" "${FC}${GROUP_B1}" rw \
          "{{user_obj rwc} {group_obj rw} {other_obj r} \
            {foreign_user ${FC}${USER_A} r} \
            {foreign_group_delegate ${FC}${GROUP_B1} rw}}" \
          $SHOULD_DENY_ACCESS
let NUMBER_FAIL=NUMBER_FAIL+$?

else
  echo
  echo "Environment variable FOREIGN_CELL not set."
  echo "No intercell delegation testing done"
  echo
fi

echo "====================================="
echo "Number of tests that failed = $NUMBER_FAIL"
