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
# $Log: acl_edit.sh,v $
# Revision 1.1.14.1  1996/10/03  18:50:19  arvind
# 	DCE 1.2.2 branch
# 	[1996/08/22  20:21 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
#
# 	Place quotes around 2nd argument to check_status().
# 	[1996/03/11  02:34:12  marty  1.1.12.2]
#
# Revision 1.1.12.2  1996/03/11  02:34:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:15  marty]
# 
# Revision 1.1.12.1  1995/12/11  20:20:24  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/04/05  23:02 UTC  burati
# 	Merge pe_site format change fix
# 
# 	HP revision /main/KK_FIX/1  1995/02/16  18:06 UTC  burati
# 	Apply pe_site BINDING retrieval fix from HOTSITE_GTE branch
# 
# 	HP revision /main/HOTSITE_GTE/1  1995/02/16  16:31 UTC  burati
# 	Fix binding extraction from pe_site file
# 	[1995/12/11  19:52:00  root]
# 
# Revision 1.1.9.1  1994/01/31  17:40:19  burati
# 	acl_server no longer needs (or accepts) useless ip option
# 	[1994/01/31  16:27:14  burati]
# 
# Revision 1.1.7.1  1993/10/07  19:06:01  desai
# 	CR 5939 fix. Trap an interrupt and clean-up by restoring original acl on the
# 	principal for the acl_edit test.
# 	[1993/10/07  19:05:08  desai]
# 
# Revision 1.1.4.3  1992/12/31  17:28:04  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:53:27  htf]
# 
# Revision 1.1.4.2  1992/11/05  19:33:34  sekhar
# 	[OT 4828]  Eliminate the spurious message caused by incorrect
# 	           grep for acl_server on the output of the ps cmd.
# 	[1992/11/05  16:39:19  sekhar]
# 
# Revision 1.1.2.4  1992/06/18  20:34:57  cuti
# 	Minor modification from [ "$CASEVAL" != 12 ]  to [ "$CASEVAL" = 12 ]
# 	[1992/06/18  20:33:38  cuti]
# 
# 	Fix OT4154: kill acl_server right before exiting test and remove some noisy t.
# 	[1992/06/16  18:39:50  cuti]
# 
# Revision 1.1.2.3  1992/06/02  21:06:22  cuti
# 	Add posix acl mask acl_edit commands (-n, -c, -p) test.
# 	[1992/06/02  18:48:17  cuti]
# 
# Revision 1.1.1.2  1992/05/05  16:13:26  cuti
# 	  Restore the object's original acls.
# 
# Revision 1.1  1992/01/19  04:07:58  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 
# 
# Copyright 1991 by Apollo Computer, Inc.
# 
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Apollo Computer, Inc not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Apollo Computer, Inc. makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
# 
#
# src/test/security/api/moretests/acl_edit.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 16:57:56
#	from delta on	11/19/91 19:12:56
#
#
# acl_edit command line test script
#
# usage: acl_edit.sh numbered_case
#

#
# Parse command line here.  Obtain 1) the value of the -d switch, if specified;
# 2) the -s switch, if specified; 3) the numbered test case; 4) the test case 
# description string.  Note that command line ordering is not significant to this
# parser.
#        
# Need to use setup to start acl_server.
# 10: posix_mask -n
# 11: posix_mask -c
# 12: posix_mask -p

#set -x
  
check_status()
{
      
	if [ $1 !=  $2 ] 
	then
		STATUS=$3 
		EXIT=1
		if [ $TRACE = "1" ]
		then
			echo "Trace Message:     $4:  expected: $2, received: $1"
			if [ $SFLAG = "1" ]
      		then
        		echo "Trace Message:      $4:  expected: $2, received: $1" 1>&2
			fi
		fi
	fi
}   

do_n()
{
         
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     acl_edit $MASK_TEST_OBJ -m user:uucp:rw -l"
    fi
	MASK_ACL=`acl_edit $MASK_TEST_OBJ -m user:uucp:rw -l | grep mask_obj`
	check_status "$MASK_ACL" "mask_obj:-rw----" 1 "acl_edit -m user:uucp:rw"
	UUCP_ACL=`acl_edit $MASK_TEST_OBJ -m user:uucp:rw -l | grep user:uucp`
	check_status "$UUCP_ACL" "user:uucp:-rw----" 1 "acl_edit -m user:uucp:rw"
         
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     acl_edit $MASK_TEST_OBJ -m user:daemon:rwx -l"
    fi
	MASK_ACL=`acl_edit $MASK_TEST_OBJ -m user:daemon:rwx -l | grep mask_obj`
	check_status "$MASK_ACL" "mask_obj:-rwx---" 1 "acl_edit -m user:daemon:rwx"
	UUCP_ACL=`acl_edit $MASK_TEST_OBJ -m user:daemon:rwx -l | grep user:uucp`
	check_status "$UUCP_ACL" "user:uucp:-rw----" 1 "acl_edit -m user:daemon:rwx"
	DAEMON_ACL=`acl_edit $MASK_TEST_OBJ -m user:daemon:rwx -l | grep user:daemon`
	check_status "$DAEMON_ACL" "user:daemon:-rwx---" 1 "acl_edit -m user:daemon:rwx"

    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     acl_edit $MASK_TEST_OBJ -m user_obj:crwxidt -n -l"
    fi
	MASK_ACL=`acl_edit $MASK_TEST_OBJ -m user_obj:crwxidt -n -l | grep mask_obj`
	check_status "$MASK_ACL" "mask_obj:-rwx---" 1 "acl_edit -m user_obj:crwxidt"
	USER_ACL=`acl_edit $MASK_TEST_OBJ -m user_obj:crwxidt -n -l | grep user_obj`
	check_status "$USER_ACL" "user_obj:crwxidt" 1 "acl_edit -m user_obj:crwxidt"
	UUCP_ACL=`acl_edit $MASK_TEST_OBJ -m user_obj:crwxidt -n -l | grep user:uucp`
	check_status "$UUCP_ACL" "user:uucp:-rw----" 1 "acl_edit -m user_obj:crwxidt"
	DAEMON_ACL=`acl_edit $MASK_TEST_OBJ -m user_obj:crwxidt -n -l | grep user:daemon`
	check_status "$DAEMON_ACL" "user:daemon:-rwx---" 1 "acl_edit -m user_obj:crwxidt"
                    
	if [ "$CASEVAL" = 10 ]
	then 
		STAGE=2 
	else
		STAGE=1
	fi

    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     acl_edit $MASK_TEST_OBJ -m user:bin:crwx -n -l"
    fi

	MASK_ACL=`acl_edit $MASK_TEST_OBJ -m user:bin:crwx -n -l | grep mask_obj`
	check_status "$MASK_ACL" "mask_obj:-rwx---" $STAGE "acl_edit -m user:bin:crwx"
	USER_ACL=`acl_edit $MASK_TEST_OBJ -m user:bin:crwx -n -l | grep user_obj`
	check_status "$USER_ACL" "user_obj:crwxidt" $STAGE "acl_edit -m user:bin:crwx"
	UUCP_ACL=`acl_edit $MASK_TEST_OBJ -m user:bin:crwx -n -l | grep user:uucp`
	check_status "$UUCP_ACL" "user:uucp:-rw----" $STAGE "acl_edit -m user:bin:crwx"
	DAEMON_ACL=`acl_edit $MASK_TEST_OBJ -m user:bin:crwx -n -l | grep user:daemon`
	check_status "$DAEMON_ACL" "user:daemon:-rwx---" $STAGE "acl_edit -m user:bin:crwx"
	BIN_ACL=`acl_edit $MASK_TEST_OBJ -m user:bin:crwx -n -l | grep user:bin | sed -e "s/\#.*//"`
	check_status "$BIN_ACL" "user:bin:crwx---	" $STAGE "acl_edit -m user:bin:crwx"

}
  

do_c()
{
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     acl_edit $MASK_TEST_OBJ -m user:root:crwx"
    fi

	acl_edit $MASK_TEST_OBJ -m user:root:crwx 2> /tmp/acl_edit.out
	REP=`cat /tmp/acl_edit.out | sed -e "s/ERROR.*/ERROR/"`
	check_status "$REP" "ERROR" 1 "acl_edit -m user:root:crwx"
    
	if [ "$CASEVAL" = 11 ]
	then 
		STAGE=2 
	else
		STAGE=1
	fi   

    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     acl_edit $MASK_TEST_OBJ -m user:root:crwx -c -l"
    fi

	MASK_ACL=`acl_edit $MASK_TEST_OBJ -m user:root:crwx -c -l | grep mask_obj` 
	check_status "$MASK_ACL" "mask_obj:crwx---" $STAGE "acl_edit -m user:root:crwx"
	USER_ACL=`acl_edit $MASK_TEST_OBJ -m user:root:crwx -c -l | grep user_obj` 
	check_status "$USER_ACL" "user_obj:crwxidt" $STAGE "acl_edit -m user:root:crwx"
	UUCP_ACL=`acl_edit $MASK_TEST_OBJ -m user:root:crwx -c -l | grep user:uucp` 
	check_status "$UUCP_ACL" "user:uucp:-rw----" $STAGE "acl_edit -m user:root:crwx"
	DAEMON_ACL=`acl_edit $MASK_TEST_OBJ -m user:root:crwx -c -l | grep user:daemon` 
	check_status "$DAEMON_ACL" "user:daemon:-rwx---" $STAGE "acl_edit -m user:root:crwx"
	BIN_ACL=`acl_edit $MASK_TEST_OBJ -m user:root:crwx -c -l | grep user:bin` 
	check_status "$BIN_ACL" "user:bin:crwx---" $STAGE "acl_edit -m user:root:crwx"
	ROOT_ACL=`acl_edit $MASK_TEST_OBJ -m user:root:crwx -c -l | grep user:root` 
	check_status "$ROOT_ACL" "user:root:crwx---" $STAGE "acl_edit -m user:root:crwx"

}

do_p()
{

    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     acl_edit $MASK_TEST_OBJ -m mask_obj:-rwx--- -l  "
    fi
      
	MASK_ACL=`acl_edit $MASK_TEST_OBJ -m mask_obj:-rwx--- -l | grep mask_obj`
	check_status "$MASK_ACL" "mask_obj:-rwx---" 1 "acl_edit -m mask_obj:-rwx"
	USER_ACL=`acl_edit $MASK_TEST_OBJ -m mask_obj:-rwx--- -l | grep user_obj`
	check_status "$USER_ACL" "user_obj:crwxidt" 1 "acl_edit -m mask_obj:-rwx"
	UUCP_ACL=`acl_edit $MASK_TEST_OBJ -m mask_obj:-rwx--- -l | grep user:uucp`
	check_status "$UUCP_ACL" "user:uucp:-rw----" 1 "acl_edit -m mask_obj:-rwx"
	DAEMON_ACL=`acl_edit $MASK_TEST_OBJ -m mask_obj:-rwx--- -l | grep user:daemon`
	check_status "$DAEMON_ACL" "user:daemon:-rwx---" 1 "acl_edit -m mask_obj:-rwx"
	BIN_ACL=`acl_edit $MASK_TEST_OBJ -m mask_obj:-rwx--- -l | grep user:bin | sed -e "s/\#.*//"` 
	check_status "$BIN_ACL" "user:bin:crwx---	" 1 "acl_edit -m mask_obj:-rwx"
	ROOT_ACL=`acl_edit $MASK_TEST_OBJ -m mask_obj:-rwx--- -l | grep user:root | sed -e "s/\#.*//"`
	check_status "$ROOT_ACL" "user:root:crwx---	" 1 "acl_edit -m mask_obj:-rwx"
           
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     acl_edit $MASK_TEST_OBJ -p -l "
    fi

	MASK_ACL=`acl_edit $MASK_TEST_OBJ -p -l | grep mask_obj`
	check_status "$MASK_ACL" "mask_obj:-rwx---" 2 "acl_edit -m mask_obj:-rwx"
	USER_ACL=`acl_edit $MASK_TEST_OBJ -p -l | grep user_obj`
	check_status "$USER_ACL" "user_obj:crwxidt" 2 "acl_edit -m mask_obj:-rwx"
	UUCP_ACL=`acl_edit $MASK_TEST_OBJ -p -l | grep user:uucp`
	check_status "$UUCP_ACL" "user:uucp:-rw----" 2 "acl_edit -m mask_obj:-rwx"
	DAEMON_ACL=`acl_edit $MASK_TEST_OBJ -p -l | grep user:daemon`
	check_status "$DAEMON_ACL" "user:daemon:-rwx---" 2 "acl_edit -m mask_obj:-rwx"
	BIN_ACL=`acl_edit $MASK_TEST_OBJ -p -l | grep user:bin`
	check_status "$BIN_ACL" "user:bin:-rwx---" 2 "acl_edit -m mask_obj:-rwx"
	ROOT_ACL=`acl_edit $MASK_TEST_OBJ -p -l | grep user:root`
	check_status "$ROOT_ACL" "user:root:-rwx---" 2 "acl_edit -m mask_obj:-rwx"

}

SFLAG="0"
FAIL="1"
SUCCESS="0"
TRACE="0"

for LCV in "$@"
do
if [ "$LCV" = "-d" ]
then
  DFLAG="-d"
  continue
fi
  if [ "$DFLAG" = "-d" ]
  then
    case $LCV in
      "1") FAIL="1" ;;
      "2") SUCCESS="1" ;;
      "3") SUCCESS="1"; FAIL="1" ;;
      "32") TRACE="1" ;;
      "33") TRACE="1"; FAIL="1" ;;
      "34") TRACE="1"; SUCCESS="1" ;;
      "63") TRACE="1"; SUCCESS="1"; FAIL="1" ;;
    esac
    DFLAG="0"
    continue
    fi
  case "$LCV" in
    -d*) DMASK=`echo $LCV | sed 's/^-d//'`
	 case $DMASK in
	   "1") FAIL="1" ;;
	   "2") SUCCESS="1" ;;
	   "3") SUCCESS="1"; FAIL="1" ;;
	   "32") TRACE="1" ;;
	   "33") TRACE="1"; FAIL="1" ;;
	   "34") TRACE="1"; SUCCESS="1" ;;
	   "63") TRACE="1"; SUCCESS="1"; FAIL="1" ;;
	 esac  ;;
    -s) SFLAG="1" ;;
    *[1-9]*) CASEVAL=$LCV ;;
    *[" "]*) LBLSTR=$LCV ;;
  esac
done


#
# Initialize program variables.
#
read tokena BINDINGS tokenb < /opt/dcelocal/etc/security/pe_site
BINDING=`echo $BINDINGS | sed -e "s/\[\].*//"`

ACL_EDIT="/opt/dcelocal/bin/acl_edit -addr $BINDING"
ACL_TEST_PERSON="flintstone"
ACL_TEST_OBJ="principal/$ACL_TEST_PERSON"
MASK_TEST_OBJ="/.:/myfilesystem/greg"
EXIT=0
CLEANUP=0
STATUS=0
SAVE_ACL=""
NEW_ACL=""
SAVE_USER_ACL=""
NEW_USER_ACL=""
SAVE_OTHER_ACL=""
NEW_OTHER_ACL=""
MASK_ACL=""
USER_ACL=""
UUCP_ACL=""
DAEMON_ACL=""
BIN_ACL=""
REP=""                 

#
# Save the existing acl so that it can be restored
# when the test finishes.
# 
  $ACL_EDIT $ACL_TEST_OBJ -l > ./acl_edit_save_entry
  $ACL_EDIT $ACL_TEST_OBJ -l > ./acl_edit_restore_entry
#
trap "$ACL_EDIT $ACL_TEST_OBJ -f ./acl_edit_restore_entry; rm ./acl_edit_save_entry; rm ./acl_edit_restore_entry; exit" 2
#
# Setup acl_edit logfile for this run.
#

echo ""
echo "---------------------------------------------------------------"
echo "Routine Tested:    acl_edit command line"
echo "Tested for:        $CASEVAL $LBLSTR" 
if [ "$CASEVAL" != 10 -a "$CASEVAL" != 11 -a "$CASEVAL" != 12 ]
then
	echo "Against Server:    $BINDING"
else
	echo "Against Server:    acl_server -e -f /tmp/aclmgr_keytab"
fi
echo""

#
# Send a copy of the output to stderr if -s is specified on command line.
#

if [ $SFLAG = "1" ]
then
  echo "" 1>&2
  echo "---------------------------------------------------------------" 1>&2
  echo "Routine Tested:    acl_edit command line" 1>&2
  echo "Tested for:        $CASEVAL $LBLSTR" 1>&2
  echo"" 1>&2
fi

#
# Select acl_edit test.
#

case "$CASEVAL" in

#
# Test the acl_edit -e command line option.
#

  "1")

  echo "The -e switch is not implemented yet."
  echo ""

  ;; # End case 1

#
# Test the acl_edit -ic command line option.
#

  "2")

#
# Obtain the initial acl for directory creation in the
# specified directory.
#
  SAVE_ACL=`$ACL_EDIT principal -ic -l | grep "Initial SEC_ACL for directories" | sed 's/^# I/i/' | sed 's/directories.*$/directory creation/`

#
# If the initial acl for directory creation could not be retrieved,
# set the status flag to a test failure and exit.
#
  if [ "$SAVE_ACL" = "" ]
  then
    STATUS=2
    EXIT=1

  else
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     Obtained $SAVE_ACL" 
      if [ $SFLAG = "1" ]
      then
        echo "Trace Message:      Obtained $SAVE_ACL" 1>&2
      fi
    fi
  fi

  ;; # End case 2


#
# Test the acl_edit -io command line option.
#

  "3")

#
# Obtain the initial acl for object creation in the
# specified directory.
#
  SAVE_ACL=`$ACL_EDIT principal -io -l | grep "Initial SEC_ACL for objects" | sed 's/^# I/i/' | sed 's/objects.*$/object creation/`

#
# If the initial acl for directory creation could not be retrieved,
# set the status flag to a test failure and exit.
#
  if [ "$SAVE_ACL" = "" ]
  then
    STATUS=2
    EXIT=1

  else
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     Obtained $SAVE_ACL" 
      if [ $SFLAG = "1" ]
      then
        echo "Trace Message:      Obtained $SAVE_ACL" 1>&2
      fi
    fi
  fi


  ;; # End case 3


#
# Test the acl_edit -m command line option.
#

  "4")

#
# Save the existing acl so that it can be restored
# when the test finishes.
#
  SAVE_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`

#
# If the existing acl could not be retrieved, set the status
# flag to a setup failure and discontinue test processing; 
# otherwise, continue with the test.
#
  if [ "$SAVE_ACL" = "" ]
  then
    STATUS=1
    EXIT=1

  else
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     Saved acl_entry $SAVE_ACL" 
      if [ $SFLAG = "1" ]
      then
        echo "Trace Message:      Saved acl_entry $SAVE_ACL" 1>&2
      fi
    fi

  # 
  # Invoke the command line test here.
  #
    $ACL_EDIT $ACL_TEST_OBJ -m other_obj:rcDnfmaug
    EXIT=$?

  #
  # If the test failed, set the status flag to a test failure
  # and discontinue test processing; otherwise, obtain the test
  # object's acl to check whether the acl was really modified as 
  # specified by the test parameter.
  #
    if [ $EXIT -ne 0 ]
    then
      STATUS=2

    else
      NEW_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`

    #
    # If the new acl was not set correctly, set the status
    # flag to a test failure and cleanup from this test case.
    #
      if [ $NEW_ACL != "other_obj:rcDnfmaug" ]
      then
        STATUS=2
        EXIT=1

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Set new acl_entry $NEW_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Set new acl_entry $NEW_ACL" 1>&2
          fi
        fi

      fi # End of test whether acl_entry was really modified.
	 
    #
    # Restore the original acl to the test object.
    #
      $ACL_EDIT $ACL_TEST_OBJ -m $SAVE_ACL
      CLEANUP=$?

      if [ $CLEANUP -ne 0 ]
      then
        STATUS=3
	EXIT=$CLEANUP

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Restored acl_entry $SAVE_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Restored acl_entry $SAVE_ACL" 1>&2
          fi
        fi

      fi # End if [ $CLEANUP -ne 0 ]

    fi # End test of whether the acl could be modified.

  fi # End test of whether setup succeeded.

  ;; # End case 4


#
# Test the acl_edit -d command line option.
#

  "5")

#
# Save the existing acl so that it can be restored
# when the test finishes.
#
  SAVE_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`

#
# If the existing acl could not be retrieved, set the status
# flag to a setup failure and discontinue test processing; 
# otherwise, continue with the test.
#
  if [ "$SAVE_ACL" = "" ]
  then
    STATUS=1
    EXIT=1

  else
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     Saved acl_entry $SAVE_ACL" 
      if [ $SFLAG = "1" ]
      then
        echo "Trace Message:      Saved acl_entry $SAVE_ACL" 1>&2
      fi
    fi

  #
  # Invoke the command line test here.
  #
    $ACL_EDIT $ACL_TEST_OBJ -d other_obj
    EXIT=$?

  #
  # If the test failed, set the status flag to a test failure
  # and discontinue test processing; otherwise, attempt to obtain
  # the other_obj's acl to check whether the acl was really deleted as 
  # specified by the test parameter.
  #
    if [ $EXIT -ne 0 ]
    then
      STATUS=2

    else
      NEW_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`

    #
    # If an occurrence of the other_obj's acl could be found,
    # set the status flag to a test failure.
    #
      if [ "$NEW_ACL" != "" ]
      then
        STATUS=2
        EXIT=1

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     deleted acl_entry $NEW_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      deleted acl_entry $NEW_ACL" 1>&2
          fi
        fi

      fi # End of test whether acl_entry was really deleted.
       
    #
    # Restore the original acl to the test object.
    #
      $ACL_EDIT $ACL_TEST_OBJ -m $SAVE_ACL
      CLEANUP=$?

      if [ $CLEANUP -ne 0 ]
      then
        STATUS=3
	EXIT=$CLEANUP

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Restored acl_entry $SAVE_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Restored acl_entry $SAVE_ACL" 1>&2
          fi
        fi

      fi # End if [ $CLEANUP -ne 0 ]

    fi # End test of whether the acl could be deleted.

  fi # End test of whether setup succeeded.

  ;; # End case 5

#
# Test the acl_edit -s command line option.
#

  "6")

#
# Save the existing acl so that it can be restored
# when the test finishes.
#   

#  $ACL_EDIT $ACL_TEST_OBJ -l > ./acl_edit_save_entry

#  SAVE_USER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep user_obj`
#  SAVE_OTHER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`
#  SAVE_ROOT_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep user:root`
#
#
# If the existing acl could not be retrieved, set the status
# flag to a setup failure and discontinue test processing; 
# otherwise, continue with the test.
#
#  if [ "$SAVE_USER_ACL" = "" -o "$SAVE_OTHER_ACL" = "" ]
#  then
#    STATUS=1
#    EXIT=1
#
#  else
#    if [ $TRACE = "1" ]
#    then
#      echo "Trace Message:     Saved acl_entry $SAVE_USER_ACL" 
#      echo "Trace Message:     Saved acl_entry $SAVE_OTHER_ACL" 
#      if [ $SFLAG = "1" ]
#      then
#        echo "Trace Message:      Saved acl_entry $SAVE_USER_ACL" 1>&2
#        echo "Trace Message:      Saved acl_entry $SAVE_OTHER_ACL" 1>&2
#      fi
#    fi
#
  #
  # Invoke the command line test here.
  #
    $ACL_EDIT $ACL_TEST_OBJ -s user_obj:rc
    EXIT=$?

  #
  # If the test failed, set the status flag to a test failure
  # and discontinue test processing; otherwise, obtain the test
  # object's acl to check whether the acl was really substituted
  # as specified by the test parameter.
  #
    if [ $EXIT -ne 0 ]
    then
      STATUS=2

    else
      NEW_USER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep user_obj`
      NEW_OTHER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`
        
	

    #
    # If the new acl was not set correctly, set the status
    # flag to a test failure and cleanup from this test case.
    #
      if [ "$NEW_OTHER_ACL" != "" -o $NEW_USER_ACL != "user_obj:rc-------" ]
      then
        STATUS=2
        EXIT=1

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Set new acl_entry $NEW_USER_ACL" 
          echo "Trace Message:     Set new acl_entry $NEW_OTHER_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Set new acl_entry $NEW_USER_ACL" 1>&2
            echo "Trace Message:      Set new acl_entry $NEW_OTHER_ACL" 1>&2
          fi
        fi

 #     fi # End of test whether acl_entry was really substituted
       
    #
    # Restore the original acl to the test object.
    # 

	   $ACL_EDIT $ACL_TEST_OBJ -f ./acl_edit_save_entry
#
#      $ACL_EDIT $ACL_TEST_OBJ -m $SAVE_USER_ACL
#      $ACL_EDIT $ACL_TEST_OBJ -m $SAVE_OTHER_ACL
#      $ACL_EDIT $ACL_TEST_OBJ -m $SAVE_ROOT_ACL
#      CLEANUP=$?

      if [ $CLEANUP -ne 0 ]
      then                                  
        STATUS=3
	EXIT=$CLEANUP

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Restored acl_entry $SAVE_USER_ACL" 
          echo "Trace Message:     Restored acl_entry $SAVE_OTHER_ACL" 
          echo "Trace Message:     Restored acl_entry $SAVE_ROOT_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Restored acl_entry $SAVE_USER_ACL" 1>&2
            echo "Trace Message:      Restored acl_entry $SAVE_OTHER_ACL" 1>&2
            echo "Trace Message:      Restored acl_entry $SAVE_ROOT_ACL" 1>&2
          fi
        fi

      fi # End if [ $CLEANUP -ne 0 ]

    fi # End test of whether the acl could be modified.

  fi # End test of whether setup succeeded.

  ;; # End case 6

#
# Test the acl_edit -f command line option.
#

  "7")

#
# Create the input file with the acl_edit test entry.
#
  echo "other_obj:rcDnfmaug" > ./acl_edit_test_entry

#
# Save the existing acl so that it can be restored
# when the test finishes.
# 
#  $ACL_EDIT $ACL_TEST_OBJ -l > ./acl_edit_save_entry
#
#  SAVE_USER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep user_obj`
#  SAVE_OTHER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`

#
# If the existing acl could not be retrieved, set the status
# flag to a setup failure and discontinue test processing; 
# otherwise, continue with the test.
#
#  if [ "$SAVE_USER_ACL" = "" -o "SAVE_OTHER_ACL" = "" ]
#  then
#    STATUS=1
#    EXIT=1
#
#  else
#    if [ $TRACE = "1" ]
#    then
#      echo "Trace Message:     Saved acl_entry $SAVE_USER_ACL" 
#      echo "Trace Message:     Saved acl_entry $SAVE_OTHER_ACL" 
#      if [ $SFLAG = "1" ]
#      then
#        echo "Trace Message:      Saved acl_entry $SAVE_USER_ACL" 1>&2
#        echo "Trace Message:      Saved acl_entry $SAVE_OTHER_ACL" 1>&2
#      fi
#    fi
#
  #
  # Invoke the command line test here.
  #
    $ACL_EDIT $ACL_TEST_OBJ -f ./acl_edit_test_entry
    EXIT=$?

  #
  # If the test failed, set the status flag to a test failure
  # and discontinue test processing; otherwise, obtain the test
  # object's acl to check whether the acl was really modified as 
  # specified by the test parameter.
  #
    if [ $EXIT -ne 0 ]
    then
      STATUS=2

    else
      NEW_OTHER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`

    #
    # If the new acl was not set correctly, set the status
    # flag to a test failure and cleanup from this test case.
    #
      if [ $NEW_OTHER_ACL != "other_obj:rcDnfmaug" ]
      then
        STATUS=2
        EXIT=1

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Set new acl_entry $NEW_OTHER_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Set new acl_entry $NEW_OTHER_ACL" 1>&2
          fi
        fi

      fi # End of test whether acl_entry was really set from a file.
	 
    #
    # Restore the original acl to the test object.
    #  
	   $ACL_EDIT $ACL_TEST_OBJ -f ./acl_edit_save_entry


#      $ACL_EDIT $ACL_TEST_OBJ -m $SAVE_USER_ACL
#      $ACL_EDIT $ACL_TEST_OBJ -m $SAVE_OTHER_ACL
      CLEANUP=$?

      if [ $CLEANUP -ne 0 ]
      then
        STATUS=3
	EXIT=$CLEANUP

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Restored acl_entry $SAVE_USER_ACL" 
          echo "Trace Message:     Restored acl_entry $SAVE_OTHER_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Restored acl_entry $SAVE_USER_ACL" 1>&2
            echo "Trace Message:      Restored acl_entry $SAVE_OTHER_ACL" 1>&2
          fi
        fi

      fi # End if [ $CLEANUP -ne 0 ]

    fi # End test of whether the acl could be modified.

#  fi # End test of whether setup succeeded.

#
# Remove the acl_edit test entry input file.
#
  rm ./acl_edit_test_entry

  ;; # End case 7


#
# Test the acl_edit -k command line option.
#

  "8")

#
# Save the existing acl so that it can be restored
# when the test finishes.
#
#  SAVE_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`

#
# If the existing acl could not be retrieved, set the status
# flag to a setup failure and discontinue test processing; 
# otherwise, continue with the test.
#
#  if [ "$SAVE_ACL" = "" ]
#  then
#    STATUS=1
#    EXIT=1
#
#  else
#    if [ $TRACE = "1" ]
#    then
#      echo "Trace Message:     Saved acl_entry $SAVE_ACL" 
#      if [ $SFLAG = "1" ]
#      then
#        echo "Trace Message:      Saved acl_entry $SAVE_ACL" 1>&2
#      fi
#    fi

  #
  # Invoke the command line test here.
  #
    $ACL_EDIT $ACL_TEST_OBJ -k
    EXIT=$?                                      

  #
  # If the test failed, set the status flag to a test failure
  # and discontinue test processing; otherwise, obtain the test
  # object's acl to check whether the acl was really killed as 
  # specified by the test parameter.
  #
    if [ $EXIT -ne 0 ]
    then
      STATUS=2

    else
      NEW_USER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep user_obj`
      NEW_OTHER_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l | grep other_obj`

    #
    # If one of the old acl_entries (that should have been killed)
    # is retrieved, set the status flag to a test failure and cleanup
    # from this test case;
    #
      if [ "$NEW_OTHER_ACL" != "" -o "$NEW_USER_ACL" = "" ]
      then
        STATUS=2
        EXIT=1

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Killed acl_entry $NEW_OTHER_ACL" 
          echo "Trace Message:     acl_entry $NEW_USER_ACL unaffected" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Killed acl_entry $NEW_OTHER_ACL" 1>&2
            echo "Trace Message:      acl_entry $NEW_USER_ACL unaffected" 
          fi
        fi

      fi # End test of whether the acl_entry was really killed.
       
    #
    # Restore the original acl to the test object.
    #
#      $ACL_EDIT $ACL_TEST_OBJ -m $SAVE_ACL
	   $ACL_EDIT $ACL_TEST_OBJ -f ./acl_edit_save_entry

      CLEANUP=$?

      if [ $CLEANUP -ne 0 ]
      then
        STATUS=3
	EXIT=$CLEANUP

      else
        if [ $TRACE = "1" ]
        then
          echo "Trace Message:     Restored acl_entry $SAVE_ACL" 
          if [ $SFLAG = "1" ]
          then
            echo "Trace Message:      Restored acl_entry $SAVE_ACL" 1>&2
          fi
        fi

      fi # End of [ $CLEANUP -ne 0 ]

    fi # End test of whether the acl could be killed.

#  fi # End test of whether setup succeeded.

  ;; # End case 8


#
# Test the acl_edit -l command line option.
#

  "9")

#
# Obtain a listing of the test object's acl.
#
  SAVE_ACL=`$ACL_EDIT $ACL_TEST_OBJ -l`

#
# If the test object's acl could not be retrieved, set the status
# flag to a setup failure and exit this test.
#
  if [ "$SAVE_ACL" = "" ]
  then
    STATUS=2
    EXIT=1

  else
    if [ $TRACE = "1" ]
    then
      echo "Trace Message:     Obtained acl_entry $SAVE_ACL"
      if [ $SFLAG = "1" ]
      then
        echo "Trace Message:      Obtained acl_entry $SAVE_ACL" 1>&2
      fi
    fi
  fi

  ;; # End case 9 


  "10" )
    
    do_n 

  ;; # case 10
  
  "11")
	
	do_n
	do_c            
   ;;  #csse 11


  "12")

	do_n
	do_c
    do_p 


  ;; # case 12 ( -p)

esac

#
# Remove the acl_edit save entry input file.
#   

	if [ "$CASEVAL" = 10 -o "$CASEVAL" = 11 -o "$CASEVAL" = 12 ]   
	then
		#  remove all the acls. 
		if [ "$CASEVAL" != 10 ]
		then
        	acl_edit $MASK_TEST_OBJ -d user:root:-rwx--- -l  >> /tmp/acl_edit.out
		fi
        acl_edit $MASK_TEST_OBJ -d user:daemon:-rwx--- -l >> /tmp/acl_edit.out
        acl_edit $MASK_TEST_OBJ -d user:bin:-rwx--- -l   >> /tmp/acl_edit.out
        acl_edit $MASK_TEST_OBJ -d user:uucp:-rw---- -l  >> /tmp/acl_edit.out
        acl_edit $MASK_TEST_OBJ -d user_obj:crwxidt -l  >> /tmp/acl_edit.out
        acl_edit $MASK_TEST_OBJ -d mask_obj:-rwx--- -l   >> /tmp/acl_edit.out 
		# Remove acl_server
		if  [ "$CASEVAL" = 12 ]
		then  
			echo "Stoppping acl_server ...\c"
			ps -ef | grep acl_server | grep -v "grep acl_server" | awk '{ print $2 }' | xargs kill -9
        fi
	else
		rm ./acl_edit_save_entry
    fi
	
#
# Report acl_edit test results.
#

if [ $FAIL = "1" ]
then

  case $STATUS in

    "1")

    echo "Failure Message:   Setup failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Setup failed with error $EXIT" 1>&2
    fi

    ;; # End case 1

    "2")

    echo "Failure Message:   acl_edit test case failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   acl_edit test case failed with error $EXIT" 1>&2
    fi

    ;; # End case 2

    "3")

    echo "Failure Message:   Cleanup failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Cleanup failed with error $EXIT" 1>&2
    fi

    ;; # End case 3

  esac # End case $STATUS in

fi # End if [ $FAIL = "1" ]

if [ $SUCCESS = "1" -a $EXIT -eq 0 ]
then
  echo "Test Status:       Test SUCCEEDED"
  if [ $SFLAG = "1" ]
  then
    echo "Test Status:       Test SUCCEEDED" 1>&2
  fi
fi

if [ $FAIL = "1" -a $EXIT -gt 0 ]
then
  echo "Test Status:       Test FAILED"
  if [ $SFLAG = "1" ]
  then
    echo "Test Status:       Test FAILED" 1>&2
  fi
fi

#
# Exit this shell and return error code.
#
exit $EXIT

#
# End script acl_edit.sh
#

