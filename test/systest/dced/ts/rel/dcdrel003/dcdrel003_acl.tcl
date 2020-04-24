#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dcdrel003_acl.tcl,v $
# Revision 1.1.5.2  1996/03/11  02:38:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:26  marty]
#
# Revision 1.1.5.1  1995/12/11  21:55:50  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:46  root]
# 
# Revision 1.1.2.4  1994/10/05  15:26:11  fadden
# 	Modify to display commands as they are executed
# 	Add acl_setup
# 	[1994/10/05  15:20:37  fadden]
# 
# Revision 1.1.2.3  1994/07/26  21:41:50  fadden
# 	Obtain the location for the file to be sourced from an
# 	  environment variable
# 	Modify error reporting to work within standard system test setup
# 	[1994/07/26  19:51:05  fadden]
# 
# Revision 1.1.2.2  1994/06/27  18:26:47  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:20  baum]
# 
# Revision 1.1.2.1  1994/06/27  17:12:36  baum
# 	Changed names from 03 -> 003
# 	[1994/06/27  17:09:36  baum]
# 
# $EndLog$


#
# dcdrel003_acl.tcl - System Test Script to exercise dced ACL operations
#		     via dcecp commands.
#

# 
# Define the directory where the test case routines reside
#
set TCDIR $env(TCDIR)

source $TCDIR/dcd_st_utils.tcl

set TEST_NAME "dcdrel003_acl"

proc acl_setup {} {

  global T_CELL_ADMIN T_CELL_ADMIN_PWD
  global TGT_CELL_HOST WHO

  set KTB_CONTAINER $TGT_CELL_HOST/config/keytab
  set HD_CONTAINER $TGT_CELL_HOST/config/hostdata
  set SVR_CONTAINER $TGT_CELL_HOST/config/srvrconf

  ###
  ### 1. Setup the containers for all objects to allow manipulation
  ###

  doit_noexit_nofatal "acl modify $KTB_CONTAINER -io  -change any_other:acdepr"

  doit_noexit_nofatal "acl modify $HD_CONTAINER -io -change any_other:cdprw"

  doit_noexit_nofatal "acl modify $SVR_CONTAINER -io -change any_other:cdfrwx"

  ### 
  ### 2. Allow our test principal to manipulate the ACL's on the container
  ###

  doit_noexit_nofatal "acl modify $KTB_CONTAINER -add $WHO:cr-"

  doit_noexit_nofatal "acl modify $HD_CONTAINER -add $WHO:cr-"

  doit_noexit_nofatal "acl modify $SVR_CONTAINER -add $WHO:cr-"

  doit_noexit_nofatal "acl modify $KTB_CONTAINER -io -add $WHO:-c--pr"

  doit_noexit_nofatal "acl modify $HD_CONTAINER -io -add $WHO:c-pr-"

  doit_noexit_nofatal "acl modify $SVR_CONTAINER -io -add $WHO:c-fr-x"

  return 0 
}

proc systest_cleanup { } {

  global HD_NAME KTB_NAME SVR_NAME WHO HD_CONTAINER KTB_CONTAINER SVR_CONTAINER 

  tet_infoline "Running systest_cleanup"

  ###
  ### Cleanup: Delete object.
  ###

  doit_noexit_nofatal "acl modify $KTB_NAME -change $WHO:acdepr"

  doit_noexit_nofatal "acl modify $KTB_CONTAINER -io -change $WHO:acdepr"

  doit_noexit_nofatal "acl modify $HD_NAME -change $WHO:cdprw"

  doit_noexit_nofatal "acl modify $HD_CONTAINER -io -change $WHO:cdprw"

  doit_noexit_nofatal "acl modify $SVR_NAME -change $WHO:cdfrwx"

  doit_noexit_nofatal "acl modify $SVR_CONTAINER -io -change $WHO:cdfrwx"

  doit_noexit_nofatal "keytab delete $KTB_NAME"

  doit_noexit_nofatal "hostdata delete $HD_NAME"

  doit_noexit_nofatal "server delete $SVR_NAME"

  return
}   
  
proc acl_systest {} {

  #  1. Prove that we can't create (insert) objects (hostdata/keytab/srvrconf)
  #     by removing 'i' from each of the ACL's and then attempting
  #     a create.  The create should fail with a permissions error.
  #
  #  2. Reset the ACL's to allow creates and create one of each object, set the
  #     -io on each to disallow add (modify) and (delete) delete operation. Show
  #	that modify and delete will FAIL.
  #
  #  3. Change the ACL to allow adds (modifys) and show this works.
  #
  #  4. Perform some object specific ACL checks
  #     - Show keytab remove of a key fails until ACL properly set.
  #
  #  5. Change the ACL to include dlete and show this works      
  #
  #  5. CLEANUP: Delete objects (may be redundant if steps 1-4 work OK.  Reset 
  #     ACL's back to orgrinal state

  global HD_NAME KTB_NAME SVR_NAME T_CELL_ADMIN T_CELL_ADMIN_PWD
  global TGT_CELL_HOST WHO KTB_CONTAINER HD_CONTAINER SVR_CONTAINER

  ###
  ### 1. Prove that we can stop creates of all dced objects.
  ###
  set MY_PID [pid]
  set VERSION1 53
  set VERSION2 54

  set KTB_SHORT_NAME systest_acl_ktb.$MY_PID
  set KTB_CONTAINER $TGT_CELL_HOST/config/keytab
  set KTB_NAME $KTB_CONTAINER/$KTB_SHORT_NAME
  set KTB_STORAGE /tmp/$KTB_SHORT_NAME

  set HD_SHORT_NAME systest_acl_hd.$MY_PID
  set HD_CONTAINER $TGT_CELL_HOST/config/hostdata
  set HD_NAME $HD_CONTAINER/$HD_SHORT_NAME
  set HD_STORAGE /tmp/$HD_SHORT_NAME
  set HD_INITIAL_DATA "Initial_data"
  set HD_MODIFIED_DATA "Modified_data"

  set SVR_SHORT_NAME systest_acl_svr.$MY_PID
  set SVR_CONTAINER $TGT_CELL_HOST/config/srvrconf
  set SVR_NAME $SVR_CONTAINER/$SVR_SHORT_NAME
  set SVR_STORAGE /tmp/$SVR_SHORT_NAME
  set SVR_CDS_PATH $TGT_CELL_HOST/systest/$SVR_SHORT_NAME
  set SVR_INTERFACE_UUID 008bebed-c7c1-1ddc-9cb3-0000c0ba4944,1.0
  set SVR_OBJECT_UUID 0073f23a-2e1a-1ddd-b73a-0000c0ba4944
  set SVR_PATH /tmp/

  #
  # This must run as 'self':
  #
  # Make sure ACL entry of type WHO exists by doing an 'add' so that
  # subsequent 'change' operations will succeed.  The perms of the ACL
  # entry at this point are irrevelant accept for the 'c' bit.
  #
  #  set cmd "acl modify $KTB_CONTAINER -add $WHO:cr-"
  #  set cmd "acl modify $HD_CONTAINER -add $WHO:cr-"
  #  set cmd "acl modify $SVR_CONTAINER -add $WHO:cr-"
  #  set cmd "acl modify $KTB_CONTAINER -io -add $WHO:-c--pr"
  #  set cmd "acl modify $HD_CONTAINER -io -add $WHO:c-pr-"
  #  set cmd "acl modify $SVR_CONTAINER -io -add $WHO:c-fr-x"

  #
  # Set ACL's for all objects to disallow  creates (turn off 'i')
  #
  doit "acl modify $KTB_CONTAINER -change $WHO:cr-"

  doit "acl modify $HD_CONTAINER -change $WHO:cr-"

  doit "acl modify $SVR_CONTAINER -change $WHO:cr-"



  #
  # Attempt to create keytab.  Verify that it FAILS with a permissions error
  #
  set cmd "keytab create $KTB_NAME -storage $KTB_STORAGE -data {$T_CELL_ADMIN plain $VERSION1 $T_CELL_ADMIN_PWD}"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  #
  # Attempt to create hostdata.  Verify that it FAILS with a permissions error
  #
  set cmd "hostdata create $HD_NAME -storage $HD_STORAGE -data $HD_INITIAL_DATA"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  #
  # Attempt to create server conf. Verify that it FAILS with a permissions error
  #
  set cmd "server create $SVR_NAME -attribute { {program $SVR_PATH}  {arguments $SVR_CDS_PATH} { services { {entryname $SVR_CDS_PATH} {interface $SVR_INTERFACE_UUID} {objects $SVR_OBJECT_UUID}} } }"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }


  ###
  ###  2. Reset the ACL's to allow creates and the -io ACL to disallow modify
  ###     and delete.  Show that we can create but can;t modify or delete
  ###

  #
  # Set ACL's for all objects to allow creates (turn on 'i')
  #
  doit "acl modify $KTB_CONTAINER -change $WHO:cri"

  doit "acl modify $HD_CONTAINER -change $WHO:cri"

  # Priviledged insert is required for the default of 
  # starting server as root
  doit "acl modify $SVR_CONTAINER -change $WHO:criI"

  #
  # Set -io ACL to disallow add(modify) and delete
  #
  doit "acl modify $KTB_CONTAINER -io -change $WHO:-c---r"

  doit "acl modify $HD_CONTAINER -io -change $WHO:c--r-"

  doit "acl modify $SVR_CONTAINER -io -change $WHO:c-fr-x"

  #
  # Attempt to create keytab.  It should work
  #
  doit "keytab create $KTB_NAME -storage $KTB_STORAGE -data {$T_CELL_ADMIN plain $VERSION1 $T_CELL_ADMIN_PWD}"

  #
  # Attempt to add a key, it should not work due to a perms error
  #
  set cmd "keytab add $KTB_NAME -member $T_CELL_ADMIN -key $T_CELL_ADMIN_PWD -version $VERSION2"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  #
  # Attempt to delete keytab, it should not work due to a perms error
  #
  set cmd "keytab delete $KTB_NAME"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }
     
  #
  # Attempt to create hostdata.  It should work
  #
  doit "hostdata create $HD_NAME -storage $HD_STORAGE -data $HD_INITIAL_DATA"

  #
  # Attempt to modify hostdata, it should not work due to a perms error
  #
  set cmd "hostdata modify $HD_NAME -data $HD_MODIFIED_DATA"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  #
  # Attempt to delete hostdata, it should not work due to a perms error
  #
  set cmd "hostdata delete $HD_NAME"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  #
  # Attempt to create server conf. It should work.
  #
  doit "server create $SVR_NAME -attribute { {program $SVR_PATH}  {arguments $SVR_CDS_PATH} { services { {entryname $SVR_CDS_PATH} {interface $SVR_INTERFACE_UUID} {objects $SVR_OBJECT_UUID}} } }"

  #
  # Attempt to delete server conf.  It should fail
  #
  set cmd "server delete $SVR_NAME"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  ###
  ### 3. Change the ACL on each object to allow add (modify) and prove this 
  ###    works
  ###

  #
  # Set ACL on objects to allow add (modify)
  #
  doit "acl modify $KTB_NAME -change $WHO:ac---r"

  doit "acl modify $HD_NAME -change $WHO:c--rw"

  #
  # Attempt to add a key to keytab, this should work.
  #
  doit "keytab add $KTB_NAME -member $T_CELL_ADMIN -key $T_CELL_ADMIN_PWD -version $VERSION2"

  #
  # Attempt to modify hostdata, this should work.
  #
  doit "hostdata modify $HD_NAME -data $HD_MODIFIED_DATA"

  #
  # Make sure that delete's still fail even with add(modify enabled)
  #

  #
  # Attempt to delete keytab, it should not work due to a perms error
  #
  set cmd "keytab delete $KTB_NAME"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  #
  # Attempt to delete hostdata, it should not work due to a perms error
  #
  set cmd "hostdata delete $HD_NAME"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  ###
  ### 4. Object specific operations
  ###       - verify keytab remove doesn't work, but will after ACL set
  ###	    - verify that server can't be started if 'x' bit off.

  #
  # Attempt to remove key, it should fail.
  #
  set cmd "keytab remove $KTB_NAME -member $T_CELL_ADMIN -version $VERSION2"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }
  
  # 
  # Change ACL to allow keytab remove and verify that we do a remove
  #
  doit "acl modify $KTB_NAME -change $WHO:ac-epr"

  doit "keytab remove $KTB_NAME -member $T_CELL_ADMIN -version $VERSION2"
  
  set msg [doit_ret_msg "keytab show -members $KTB_NAME"]
  
  set ret [string match *$VERSION2* $msg]
  if { $ret != 0 } {
    tet_infoline "FATAL: Key with version #$VERSION2 is still in keytab $KTB_NAME after remove"
    return -1
  }
  
  #
  # Change ACL to disallow server start, verify.
  #
  doit "acl modify $SVR_NAME -change $WHO:c-fr--"
  
  set cmd "server start $SVR_NAME"
  tet_infoline "Executing $cmd"
  set ret [catch "$cmd" msg]
  if { $ret == 0 } { unwanted_success_proc; return -1 }

  set ret [string match *permission* $msg]
  if { $ret != 1 } { not_bad_perms_proc; return -1 }

  ###
  ### 5. Change the ACL to include delete and show that this works
  ###
  doit "acl modify $KTB_NAME -change $WHO:acdepr"

  doit "acl modify $HD_NAME -change $WHO:cdprw"

  doit "acl modify $SVR_NAME -change $WHO:cdfrwx"

  doit "keytab delete $KTB_NAME"

  doit "hostdata delete $HD_NAME"

  doit "server delete $SVR_NAME"

  return 0 
}

parse_args 

tet_infoline "Starting ACL Test"

set WHO user:$TEST_PRINC

set ret [catch "login $TEST_PRINC -password $TEST_PRINC_PWD" msg]
if { $ret != 0 } {
  tet_infoline "FATAL: Could not dce_login $TEST_PRINC $TEST_PRINC_PWD ($msg)"
  exit -1
}

tet_infoline "Starting test Setup"

set ret [catch acl_setup msg]
if { $ret != 0 } {
  tet_infoline "FATAL: Error Occurred ($msg)"
} else {
  set ret [catch acl_systest msg]
  if { $ret != 0 } {
    tet_infoline "FATAL: Error Occurred ($msg)"
  }
}

systest_cleanup

if { $ret != 0 } {
  exit -1
} else {
exit 0
}
