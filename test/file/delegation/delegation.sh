#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: delegation.sh,v $
# Revision 1.1.9.1  1996/10/17  18:18:45  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:50:13  damon]
#
# Revision 1.1.4.2  1994/08/10  19:22:58  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:17:16  annie]
# 
# Revision 1.1.4.1  1994/07/13  22:33:00  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:17:41  devsrc]
# 
# 	Delegation Functional Tests - Initial Submit
# 	[1994/06/08  18:30:20  delgado]
# 
# $EndLog$
#
#
# This is the DFS delegation test for user space.  It exercises admin list
# access for delegated entities.
# 
# There are 3 components to this test:
#     client dlg program - logs in as the specified entity and initiaties
#          an access check to the intemediary server
#     intemediary dlg server - becomes a delegate of the client program
#          and issues an access check request to the target server
#          This server runs as "dfsdlg-int" principal
#     target dlg server - uses the dfsauth_CheckAdminListAccess family
#          of functions to perform access list checking and returns
#          a boolean value indicating whether the client entity has
#          access or not.  This server runs as "dfsdlg-target" principal
#
# Setup:  we rely on bosserver to create the target server and its
#         admin lists.  We use the bos interface to modify the adminlist,
#         run the client dlg program to tests access and check our results.
#
# Warning: Currently, running this script more than one successive time
#          give inconsistent results.  This is because the bosserver uses
#          sec_id_parse_name to retrieve the uuid when adding entries to
#          the adminlist, and sec_id_parse_name sometimes will return the
#          extinct uuid of for the dfsdlg_client.  (Problem with deleting
#          a principal and re-creating a principal of the same name, we
#          get the wrong uuid returned from sec_id_parse_name - a cacheing
#          problem perhaps?)  Workaround is to restart the bosserver
#
# To Do:   add filesystem acl access checking
#          smater cleanup
#


TARGET_HOST=/.:/hosts/alcatraz
INT_HOST=/.:/hosts/alcatraz
TESTBIN=/fs3/sb/dfs11/obj/rios/test/file/delegation
TARGET_SERVER=dfsdlg_target
INTERMEDIARY_SERVER=dfsdlg_int
USER=dfsdlg_client
USER_GROUP=dfsdlg_test_admin_user
DLG_GROUP=dfs_int_delegate
TGT_GROUP=dfs_target_group
export TGT_GROUP DLG_GROUP USER_GROUP USER
CLIENT_FLAGS="-d -p $USER -pw $USER"
num_errors=0
#
#
ERROR=0
SUCCESS=1
DEBUG=1
DOCLEANUP=0
DOCLEANUP_FIRST=1
NO_DELEGATION=1

#
#
# This function could be a little
# better about checking before attempting
# to delete things - so you can ignore
# any errors which it generates regarding non-existent
# entitities 
#
do_cleanup()
{
echo "Cleaning up Registry...\n"
rgy_edit << EOF
domain p
del $USER 
del $INTERMEDIARY_SERVER 
del $TARGET_SERVER 
domain g
del $USER_GROUP 
del $DLG_GROUP 
del $TGT_GROUP 
ktd -p $INTERMEDIARY_SERVER -v 1
ktd -p $TARGET_SERVER -v 1
exit
EOF
}

shutdown_and_delete()
{

    echo "Stopping process $1 ...\n"
    bos stop -proc $1 -server $2 
    bos delete -proc $1 -server $2 
    bos lsadmin -server $2 -admin admin.$1
    if [ $? -eq 0 ]
    then
       bos rmadmin -server $2 -admin admin.$1 -removelist 
    fi
}
#
# $1 = exit code from dfsdlg_client
# $2 = which test case
# $3 = no cleanup on exit
#
check_error()
{
   if [ $1 -ne 0 ]
   then
       echo "Test case $2 FAILED"
       if [ $3 -eq 1 ]
       then
           shutdown_and_delete dfsdlg_int $INT_HOST
           shutdown_and_delete dfsdlg_target $TARGET_HOST
           do_cleanup
           echo "Delegation Tests Failed"
       fi
       num_errors=`expr $num_errors + 1`
   else
       echo "Test case $2 PASSED "
   fi
}

if [ $DOCLEANUP_FIRST -ne 0 ]
then
    do_cleanup
    shutdown_and_delete dfsdlg_target $TARGET_HOST
    shutdown_and_delete dfsdlg_int $INT_HOST
fi

#
#  Setup the registry
#
rgy_edit << EOF
domain p
add  dfsdlg_target
add  dfsdlg_int
add  $USER
domain g
add $USER_GROUP
add $DLG_GROUP 
add $TGT_GROUP
domain a 
add dfsdlg_target -g $TGT_GROUP -o none -mp -dce- -pw dfsdlg_target
add dfsdlg_int -g $DLG_GROUP -o none -mp -dce- -pw dfsdlg_int
add $USER -g $USER_GROUP -o none -mp -dce- -pw $USER
kta -p dfsdlg_target -pw dfsdlg_target
kta -p dfsdlg_int -pw dfsdlg_int
quit
EOF


if [ $? -ne 0 ]
then
    echo "E1 Registry Setup Failed"
    do_cleanup
    exit 1
fi
echo "Registry Setup Complete\n"
#
# Now create our servers via bosserver
#
echo "Creating $TESTBIN/dfsdlg_target"
bos create -proc dfsdlg_target -type simple   -cmd "$TESTBIN/dfsdlg_target -d"\
           -server $TARGET_HOST
if [ $? -ne 0 ]
then
    echo "E2 Cannot create the dfsdlg_target server on host $TARGET_HOST"
    shutdown_and_delete dfsdlg_target $TARGET_HOST
    do_cleanup
    exit 1
fi
echo "dfsdlg_target process created...\n"
echo "Creating $TESTBIN/dfsdlg_int"
bos create -proc dfsdlg_int -type simple -cmd "$TESTBIN/dfsdlg_int -d" \
           -server $INT_HOST


if [ $? -ne 0 ]
then
    echo "E3 Cannot create the dfsdlg_int server on host $INT_HOST"
    shutdown_and_delete dfsdlg_target $TARGET_HOST
    shutdown_and_delete dfsdlg_int $INT_HOST
    do_cleanup
    exit 1
fi
echo "dfsdlg_int process created \n"

#
#  Check that both servers are running normally before we begin
#

bos status -proc dfsdlg_target -server $TARGET_HOST | \
    fgrep "currently running normally"
if [ $? -ne 0 ]
then
    "Error checking state of dfsdlg_target server"
    shutdown_and_delete dfsdlg_target $TARGET_HOST
    shutdown_and_delete dfsdlg_int $INT_HOST
    do_cleanup
    exit 1
fi

bos status -proc dfsdlg_int -server $INT_HOST |\
    fgrep "currently running normally"
if [ $? -ne 0 ]
then
    "Error checking state of dfsdlg_int server"
    shutdown_and_delete dfsdlg_target $TARGET_HOST
    shutdown_and_delete dfsdlg_int $INT_HOST
    do_cleanup
    exit 1
fi
#
#
if [ $DEBUG -ne 0 ]
then
    CLIENT_UUID=`rgy_edit -p -v -f $USER | fgrep Uuid  `
    echo "The client principal's uuid is $CLIENT_UUID"
fi
#
# Checks with dfsdlg_client identity, no delegation
#
# Check user access
bos addadmin -admin admin.dfsdlg_target -createlist -princ $USER -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "\n\nAdmin list for case A1\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $SUCCESS $CLIENT_FLAGS -ndlg
check_error $? A1 $DOCLEANUP

#
# Check Group Access, no delegation
#
bos rmadmin -admin admin.dfsdlg_target -princ $USER -server $TARGET_HOST
bos addadmin -admin admin.dfsdlg_target -group $USER_GROUP -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "\n\nAdmin list for case A2\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $SUCCESS $CLIENT_FLAGS -ndlg
check_error $? A2 $DOCLEANUP

bos rmadmin -admin admin.dfsdlg_target -group $USER_GROUP -server $TARGET_HOST

if [ $NO_DELEGATION -eq 1 ]
then
    shutdown_and_delete dfsdlg_target $TARGET_HOST
    shutdown_and_delete dfsdlg_int $INT_HOST
    do_cleanup
    if [ $num_errors -eq 0 ]
    then
        echo "Delegation Test A Passed"
    fi 
    exit
fi 

#
# Checks with dfsdlg_Client identity, delegation enabled 
#  
#  Case B1:  initiator in user, delegate = no access
#
bos addadmin -admin admin.dfsdlg_target -createlist -princ $USER -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "\n\nAdmin list for case B1\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $ERROR $CLIENT_FLAGS
check_error $? B1 $DOCLEANUP
#
#  Now modify admin lists and check access
#  Case B2:  inititator in user, delegate in user#

if [ $DEBUG -ne 0 ]
then
    echo "Admin list for case B2\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
bos addadmin -admin admin.dfsdlg_target -princ $INTERMEDIARY_SERVER -server $TARGET_HOST
$TESTBIN/dfsdlg_client -expect $SUCCESS $CLIENT_FLAGS
check_error $? B2 $DOCLEANUP

#  Case b3:  inititator in user, delegate in group

bos rmadmin -admin admin.dfsdlg_target -princ  $INTERMEDIARY_SERVER -server $TARGET_HOST
bos addadmin -admin admin.dfsdlg_target -group $DLG_GROUP -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "Admin list for case B3\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $SUCCESS  $CLIENT_FLAGS
check_error $? B3 $DOCLEANUP

# case B4: initiator = no access, delegate in group

bos rmadmin -admin admin.dfsdlg_target -princ $USER -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "Admin list for case B4\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $ERROR $CLIENT_FLAGS
check_error $? B4 $DOCLEANUP

#  Case B5:  inititator in group, delegate in group
bos addadmin -admin admin.dfsdlg_target -group $USER_GROUP -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "Admin list for case B5\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $SUCCESS $CLIENT_FLAGS
check_error $? B5 $DOCLEANUP

#  Case B6: initiator in group, delegate = no access

bos rmadmin -admin admin.dfsdlg_target -group $DLG_GROUP -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "Admin list for case B6\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $ERROR $CLIENT_FLAGS
check_error $? B6 $DOCLEANUP

#  Case B7:  inititator in group, delegate in user

bos addadmin -admin admin.dfsdlg_target -princ $INTERMEDIARY_SERVER -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "Admin list for case B7\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $SUCCESS $CLIENT_FLAGS
check_error $? B7 $DOCLEANUP

#  Case B8:  initiator no access, delegate in user

bos rmadmin -admin admin.dfsdlg_target -group $USER_GROUP -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "Admin list for case B8\n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $ERROR $CLIENT_FLAGS
check_error $? B8 $DOCLEANUP

#  Case b9:  initiator no access, delegate in group
bos addadmin -admin admin.dfsdlg_target -group $DLG_GROUP -server $TARGET_HOST
bos rmadmin -admin admin.dfsdlg_target -princ  $INTERMEDIARY_SERVER -server $TARGET_HOST
if [ $DEBUG -ne 0 ]
then
    echo "\n\nAdmin list for case B9 \n"
    bos lsadmin -admin admin.dfsdlg_target -server $TARGET_HOST
fi
$TESTBIN/dfsdlg_client -expect $ERROR $CLIENT_FLAGS
check_error $? B9 $DOCLEANUP

bos rmadmin -admin admin.dfsdlg_target -server $TARGET_HOST -group $DLG_GROUP

#
#  File system Access checking
#  Not Yet Implemented
#
 if [ $num_errors -eq 0 ]
then
    echo "Delegation Tests Passed"
else
    echo "Delgation Tests Failed $num_errors/$num_cases Cases Executed"
echo "Cleaning Up...."
do_cleanup
shutdown_and_delete dfsdlg_target $TARGET_HOST
shutdown_and_delete dfsdlg_int $INT_HOST










