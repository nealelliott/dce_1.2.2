#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: nocode.sh,v $
# Revision 1.1.4.2  1996/03/11  02:23:33  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:57:52  marty]
#
# Revision 1.1.4.1  1995/12/11  19:57:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:40:26  root]
# 
# Revision 1.1.2.3  1993/01/25  18:02:40  bbelch
# 	Fix duplicate log
# 	[1993/01/25  17:33:06  bbelch]
# 
# Revision 1.1.2.2  1993/01/11  21:41:39  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  17:38:30  bbelch]
# 
# Revision 1.1  1992/01/19  03:40:12  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#



#
# Parse command line here.  Obtain 1) the value of the -d switch, if specified;
# 2) the -s switch, if specified; 3) the test case description string.  Note
# that while there is a IDL command line parameter ordering convention, order
# is not significant to this parser.   
#
#	Need to set INC if include files are not located in current working directory.
#

SFLAG="0"
FAIL="1"
SUCCESS="0"
TRACE="0"

for LCV in "$@"
do
  if test "$LCV" = "-d"
  then
    DFLAG="-d"
    continue
  fi
  if test "$DFLAG" = "-d"
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
    *[" "]*) LBLSTR=$LCV ;;
  esac
done

#
# Setup list of all standard c source files including the header file.
#

CSRC="nocode.h nocode_cstub.c nocode_sstub.c"

#
# Setup the IDL test search path.
#

INCL=${INCL:-./}
DEFINES=-Dunix
INC="-I$INCL -I$INCL/dce  $DEF"
echo "INC="$INC
export INC 

#
# Setup the base IDL command line.
#

IDL="idl $INC nocode.idl"

#
# Initialize return value to 0 (zero) for normal return.
#

EXIT=0

#
# Initialize current working directory.
#

cwd=`pwd`

#
# Setup nocode logfile for this run.
#

echo ""
echo "---------------------------------------------------------------"
echo "Routine Tested:    nocode.sh"
echo "Tested for:        0 $LBLSTR"
echo""

#
# Send a copy of the output to stderr if -s is specified on command line.
#

if test $SFLAG = "1"
then
  echo "" 1>&2
  echo "---------------------------------------------------------------" 1>&2
  echo "Routine Tested:    nocode.sh" 1>&2
  echo "Tested for:        0 $LBLSTR" 1>&2
  echo"" 1>&2
fi

#
# Invoke the test for the code/nocode attributes.  The attributes are specified
# in nocode.acf.  Retain only source output.
#

$IDL -keep c_source
EXIT=$?

EFILES="$CSRC"

#
# Verify IDL output.  Check to see that all the files that were supposed to
# be generated were.
#

for LCV in $EFILES
do 
  if test -f "$LCV"
  then
    if test $TRACE = "1"
    then
      echo "Trace Message:     Expected file $LCV found"
      if test $SFLAG = "1"
      then
        echo "Trace Message:     Expected file $LCV found" 1>&2
      fi
    fi
  else
    EXIT=1
    if test $TRACE = "1" -a $FAIL = "0"
    then
      echo "Trace Message:     Expected file $LCV not found"
      if test $SFLAG = "1"
      then
        echo "Trace Message:     Expected file $LCV not found" 1>&2
      fi
    fi
    if test $FAIL != "0"
    then
      echo "Failure Message:   Expected file $LCV not found"
      if test $SFLAG = "1"
      then
        echo "Failure Message:   Expected file $LCV not found" 1>&2
      fi
    fi
  fi
done

#
# Now verify the contents of the output.  If nocode_cstub.c contains the string
# "op2", the test fails.
#

grep op2 nocode_cstub.c

if test $? -eq 0
then
  EXIT=1
  if test $FAIL != "0"
  then
    echo "Failure Message:   nocode_cstub.c contains invalid output"
    if test $SFLAG = "1"
    then
      echo "Failure Message:   nocode_cstub.c contains invalid output" 1>&2
    fi
  fi
fi

#
# Cleanup leftover test files.  
#

rm -f nocode.h nocode*.c

#
# Log test completion status, if required.
#

if test $SUCCESS = "1" -a $EXIT -eq 0
then
  echo "Test Status:       Test SUCCEEDED"
  if test $SFLAG = "1"
  then
    echo "Test Status:       Test SUCCEEDED" 1>&2
  fi
fi

if test $FAIL != "0" -a $EXIT -gt 0
then
  echo "Test Status:       Test FAILED"
  if test $SFLAG = "1"
  then
    echo "Test Status:       Test FAILED" 1>&2
  fi
fi

#
# Exit this shell and return error code.
#

exit $EXIT

#
# End script nocode.sh
#

