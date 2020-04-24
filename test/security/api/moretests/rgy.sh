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
# $Log: rgy.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:50  marty]
#
# Revision 1.1.4.1  1995/12/11  21:49:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:43  root]
# 
# Revision 1.1.2.3  1993/03/18  16:44:48  cuti
# 	Add -up to rgy_edit command so all the operation will go to master.
# 	[1993/03/18  16:42:42  cuti]
# 
# Revision 1.1.2.2  1992/12/31  17:31:35  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:56:55  htf]
# 
# Revision 1.1  1992/01/19  04:08:06  devrcs
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
# src/test/security/api/moretests/rgy.sh, , dce_osf, dce.75d  
#   
#	Extracted	1/4/92 16:58:24
#	from delta on	11/21/91 21:40:12
#
#
# low level rgy_edit test script
#
# usage: rgy.sh [-d n] [-s] [-f] [-v string] <TestID> <Test Label> <command lines for rgy_edit>
#
# -d is debug flag
# -s is save in file
# -f is expect failure return from rgy_edit
# -v verify that the string is found in rgy_output
            
SFLAG="0"
SHOW_FAIL="1"
SHOW_SUCCESS="0"
TRACE="0"
EXPECT_FAIL="0"
VERIFY_FLAG="0"



if [ "$1" = "-d" ] 
then
    shift
    case $1 in 
      "1") SHOW_FAIL="1" ;;
      "2") SHOW_SUCCESS="1" ;;
      "3") SHOW_SUCCESS="1"; SHOW_FAIL="1" ;;
      "32") TRACE="1" ;;
      "33") TRACE="1"; SHOW_FAIL="1" ;;
      "34") TRACE="1"; SHOW_SUCCESS="1" ;;
      "63") TRACE="1"; SHOW_SUCCESS="1"; SHOW_FAIL="1" ;;
    esac
    shift
fi

if [ "$1" = "-s" ] 
then
    shift
    SFLAG="1"
fi

if [ "$1" = "-f" ] 
then
    shift
    EXPECT_FAIL="1"
fi

if [ "$1" = "-v" ]
then
    VERIFY_FLAG="1"
    shift
    VERIFY_STRING="$1"
    shift
fi

if [ "$1" = "-vn" ]
then
    VERIFY_FLAG="2"
    shift
    VERIFY_STRING="$1"
    shift
fi



CASEVAL="$1"
shift

LBLSTR="$1"
shift


EXIT=0
CLEANUP=0
STATUS=0

#
# Setup rgy_edit logfile for this run.
#

echo ""
echo "---------------------------------------------------------------"
echo "Routine Tested:    rgy_edit command line"
echo "Tested for:        $CASEVAL $LBLSTR"
echo""

#
# Send a copy of the output to stderr if -s is specified on command line.
#

if [ $SFLAG = "1" ]
then
  echo "" 1>&2
  echo "---------------------------------------------------------------" 1>&2
  echo "Routine Tested:    rgy_edit command line" 1>&2
  echo "Tested for:        $CASEVAL $LBLSTR" 1>&2
  echo"" 1>&2
fi


if [ -f rgy_input ]
    then
    rm rgy_input
fi

for LCV in "$@"
do
    echo "$LCV" >>rgy_input
    if [ $TRACE = "1" ]
    then 
        echo "Trace Message:     rgy_edit edit command: $LCV"
        if [ $SFLAG = "1" ]
        then
        echo "Trace Message:     rgy_edit edit command: $LCV" 1>&2
        fi
     fi
done
echo "quit" >>rgy_input
if [ $TRACE = "1" ]
then 
  echo "Trace Message:     rgy_edit edit command: quit"
  if [ $SFLAG = "1" ]
  then
    echo "Trace Message:     rgy_edit edit command: quit" 1>&2
  fi
fi

/opt/dcelocal/bin/rgy_edit -up <rgy_input >rgy_output; EXIT=$?

if [ $EXIT -ne "0" -a $EXPECT_FAIL = "0" ]
    then
    STATUS=2
fi


if [ $EXIT = "0" -a $EXPECT_FAIL = "1" ]
    then
    STATUS=2
fi

if [ $EXIT -ne "0"  -a $SHOW_FAIL = "1" ] 
    then
        echo -n "Failure Message (from rgy_edit):      "
        grep '?' <rgy_output
fi

if [ $SFLAG = "1" ]
        then
        echo -n "Failure Message (from rgy_edit):      :      " 1>&2
        grep '?' <rgy_output 1>&2
fi

if [ $VERIFY_FLAG -ne "0" ]
then
  grep -s "$VERIFY_STRING" <rgy_output
  EX1=$?
  if [ $VERIFY_FLAG = 1 -a $EX1 -ne "0" ]
  then
    EXIT=1
    STATUS=4
  fi
  if [ $VERIFY_FLAG = 2 -a $EX1 = "0" ]
  then
    EXIT=1
    STATUS=5
  fi
fi


if [ $SHOW_SUCCESS = "1" -a $STATUS -eq 0 ]
then
  echo "Test Status:       SUCCEEDED"
  if [ $SFLAG = "1" ]
  then
    echo "Test Status:       SUCCEEDED"  1>&2
  fi
fi

if [ $SHOW_FAIL = "1" -a $STATUS -ne 0 ]
then
  echo "Test Status:       Test FAILED"
  if [ $SFLAG = "1" ]
  then
    echo "Test Status:       Test FAILED" 1>&2
  fi
fi 

if [ $SHOW_FAIL = "1" ]
then
  case "$STATUS" in

    "1")

    echo "Failure Message:   Setup failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Setup failed with error $EXIT" 1>&2
    fi

    ;; # End case 1

    "2")

    echo "Failure Message:   rgy_edit test case failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   rgy_edit test case failed with error $EXIT" 1>&2
    fi
    ;; # End case 2

    "3")

    echo "Failure Message:   Cleanup failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Cleanup failed with error $EXIT" 1>&2
    fi

    ;; # End case 3

    "4")
    echo "Failure Message:   Verify failed \"$VERIFY_STRING\" not found in rgy_edit output"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Verify failed \"$VERIFY_STRING\" not found in rgy_edit output" 1>&2
    fi

    ;; # End case 4

    "5")
    echo "Failure Message:   Verify failed \"$VERIFY_STRING\" unexpectedly found in rgy_edit output"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Verify failed \"$VERIFY_STRING\" unexpectedly found in rgy_edit output" 1>&2
    fi

    ;; # End case 5

  esac # End case $STATUS in

fi # End if [ $SHOW_FAIL = "1" ] 

#
# Exit this shell and return error code.
#

exit $EXIT

#
# End script rgy.sh
#
