#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# (c) Copyright 1991, 1992 
#     Siemens-Nixdorf Information Systems, Burlington, MA, USA
#     All Rights Reserved
#
# HISTORY
# $Log: cmd_line.sh,v $
# Revision 1.1.15.2  1996/03/11  02:21:59  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:57:12  marty]
#
# Revision 1.1.15.1  1995/12/11  19:37:14  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/06/27  19:41 UTC  tatsu_s
# 	Submitted the Mothra10 fix.
# 
# 	HP revision /main/HPDCE02/tatsu_s.fix_ot12885.b1/1  1995/06/27  19:35 UTC  tatsu_s
# 	Fixed -cc/cpp_opt for HP-UX 10.x.
# 
# 	HP revision /main/HPDCE02/1  1994/07/29  18:38 UTC  tatsu_s
# 	Merged upto the post-bata DCE 1.1 baseline.
# 	[1995/12/11  19:19:41  root]
# 
# Revision 1.1.12.1  1994/06/29  21:00:04  ohara
# 	re-port to SINIX
# 	[1994/05/12  21:02:49  ohara]
# 
# Revision 1.1.10.1  1993/09/29  18:48:52  tom
# 	Bug 8873 - Remove references to 'aux' files, as the new idl doesn't
# 	produce them anymore.
# 	[1993/09/29  18:47:56  tom]
# 
# Revision 1.1.8.2  1993/06/16  19:15:14  sommerfeld
# 	Use ANSI-C and ANSI cpp on HP-UX
# 	Allow override of INCL
# 	[1993/06/16  17:31:04  sommerfeld]
# 
# Revision 1.1.3.5  1993/01/26  23:30:38  weisman
# 	Commented out SINIX-specific DEFINES.
# 	[1993/01/26  23:21:02  weisman]
# 
# Revision 1.1.3.4  1993/01/25  18:02:13  bbelch
# 	Fix duplicate log
# 	[1993/01/25  17:32:21  bbelch]
# 
# Revision 1.1.3.3  1993/01/11  21:38:27  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  14:57:30  bbelch]
# 
# Revision 1.1.3.2  1992/09/29  21:22:23  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:47:33  weisman]
# 
# Revision 1.1  1992/01/19  03:39:43  devrcs
# 	Initial revision
# 
# $EndLog$
#
# 	SNI NOTE: Compile stubs with SNI_SVR4 switches (not sure how to 
# 	actually handle	this in the merge).  Need a way to conditionally
#	dictate c switches passed to the idl compiler.
#
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
# Revision 1.1  91/09/11  16:09:08  marco
# Initial revision
# 



#
#
# IDL test script cmd_line.sh
#
# usage: cmd_line.sh name_of_idl_input_file numbered_case
#
# Tests 1-6 call the C preprocessor and compiler.  Expect these tests to take
# much longer to run than tests 7-26 which are simple verifications of source
# and error log output.
#
# Parse command line here.  Obtain 1) the value of the -d switch, if specified;
# 2) the -s switch, if specified; 3) the idl input file; 4) the numbered test
# case; 5) the test case description string.  Note that while there is a IDL
# command line parameter ordering convention, order is not significant to this
# parser.
#

# Set $TESTCASEDIR and $INCL to the current directory if they're not
# set in the environment


TESTCASEDIR=${TESTCASEDIR:-./}
INCL=${INCL:-/usr/include}
CPP=`ls /lib/cpp >/dev/null`
if [ $? -ne 0 ]
then
  CPP=`ls /usr/lib/cpp >/dev/null`
  if [ $? -ne 0 ]
  then
    echo "Ensure CPPDIR is set as cpp isn not in "
    echo "/lib nor in /usr/lib"
  fi
fi
CPPDIR=$CPP
CPPDIR=${CPPDIR:-/lib}

case `uname` in
HP-UX*)
  DEFINES="-D__hppa -Dhp9000s800 -Dhp9000s700 -D__hp9000s800 -D__hp9000s700 -DHPUX -D__hpux -Dunix -D_POSIX_REENTRANT_FUNCTIONS -D_HPUX_SOURCE -D_REENTRANT"
  ;;
SINIX*)
  DEFINES="-Dunix -DSNI_SINIX -DSNI_MX300I -DM_I86 -DSNI_SVR4 -DSNI_SVR4_POSIX"
  ;;
*)
  DEFINES=-Dunix
  ;;
esac

echo
echo "TESTCASEDIR="$TESTCASEDIR
echo "INCL="$INCL
echo "CPPDIR="$CPPDIR

export TESTCASEDIR INCL CPPDIR DEFINES

SFLAG="0"
FAIL="1"
SUCCESS="0"
TRACE="0"

# The idl compiler should already be accessible as a build tool.
#case "$IDL_LOCATION" in
#"")	echo 1>&2 "path not specified in global shell variable IDL_LOCATION"
#	exit 1
#	;;
#esac

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
    *.idl) IDLF=`echo $LCV | sed 's/\.idl$//'` ;;
    *[1-9]*) CASEVAL=$LCV ;;
    *[" "]*) LBLSTR=$LCV ;;
  esac
done

#
# Setup list of all standard c source files including the header file.
#

CSRC="cmd_line_cstub.c cmd_line_sstub.c"

#
# Setup list of all standard c object files.
#

COBJ="cmd_line_cstub.o cmd_line_sstub.o"

#
# Setup the IDL test search path.


INC="-I$INCL -I$INCL/dce -I$TESTCASEDIR"
echo "INC="$INC

#
# Setup the base IDL command line.
#

#IDL="$IDL_LOCATION/idl $IDLF.idl $INC"
IDL="idl $DEFINES $IDLF.idl $INC"
case `uname` in
HP-UX*)
  CC_CMD="/bin/c89 -D_HPUX_SOURCE -c"
  CC_OPT="$DEFINES $INC"
  CPP_OPT="-A -P -C $DEFINES $INC"
  case `uname -r` in
  [A-Z]\.09*)
    CPP_CMD="$CPPDIR/cpp.ansi"
    ;;
  *)
    CPP_CMD="$CPPDIR/cpp"
    ;;
  esac
  ;;
*)
  CC_CMD="/bin/cc -c"
  CC_OPT="$DEFINES $INC"
  CPP_CMD="$CPPDIR/cpp"
  CPP_OPT="$DEFINES $INC"
  ;;
esac

#
# Initialize return value to 0 (zero) for normal return.
#

EXIT=0

#
# Initialize current working directory.
#

cwd=`pwd`


#
# Setup cmd_line logfile for this run.
#

echo ""
echo "---------------------------------------------------------------"
echo "Routine Tested:    compiler command line"
echo "Tested for:        $CASEVAL $LBLSTR"
echo""

#
# Send a copy of the output to stderr if -s is specified on command line.
#

if test $SFLAG = "1"
then
  echo "" 1>&2
  echo "---------------------------------------------------------------" 1>&2
  echo "Routine Tested:    compiler command line" 1>&2
  echo "Tested for:        $CASEVAL $LBLSTR" 1>&2
  echo"" 1>&2
fi

#
# Select idl compiler test.
#

case "$CASEVAL" in

#
# Test IDL specifying the C preprocessor and compiler.  Retain both source
# and object output.
#

     "1")

     $IDL -cc_cmd "$CC_CMD" -cc_opt "$CC_OPT" -cpp_cmd "$CPP_CMD" -cpp_opt "$CPP_OPT" -keep both
     EXIT=$?

     EFILES="cmd_line.h $CSRC $COBJ"
     UFILES=""
     ;;

#
# Verify the -error all command line option.
#
#
#     "2")
#
#     $IDL -cc_opt "$CC_OPT" -error all 
#     EXIT=$?
#
#     EFILES="cmd_line.h $COBJ"
#     UFILES="$CSRC"
#     ;;
#
#
# Verify the -error trees command line option.
#
#     "3")
#
#     $IDL -cc_opt "$CC_OPT" -error trees 
#     EXIT=$?
#
#     EFILES="cmd_line.h $COBJ"
#     UFILES="$CSRC"
#     ;;
#
#
# Verify the -port all command line option.
#
#
#     "4")
#
#     $IDL -cc_opt "$CC_OPT" -port all 
#     EXIT=$?
#
#     EFILES="cmd_line.h $COBJ"
#     UFILES="$CSRC"
#     ;;
#
#
# Verify the -port case command line option.
#
#
#     "5")
#
#     $IDL -cc_opt "$CC_OPT" -port case 
#     EXIT=$?
#
#     EFILES="cmd_line.h $COBJ"
#     UFILES="$CSRC"
#     ;;

#
# Verify the -space_opt command line option.
#

     "6")

     $IDL -cc_opt "$CC_OPT" -space_opt 
     EXIT=$?

     EFILES="cmd_line.h $COBJ"
     UFILES="$CSRC"
     ;;

#
# Verify the -confirm command line option.
#

     "7")

     $IDL -cc_opt "$CC_OPT" -confirm 
     EXIT=$?

     EFILES=""
     UFILES="cmd_line.h $CSRC $COBJ"
     ;;

#
# Generate client stub module, but not client auxiliary.
#

     "8")

     $IDL -cc_opt "$CC_OPT" -client stub -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line_cstub.c cmd_line_sstub.c"
     UFILES="$COBJ"
     ;;

#
# Generate server stub module, but not auxiliary.
#

     "9")

     $IDL -cc_opt "$CC_OPT" -server stub -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line_sstub.c cmd_line_cstub.c"
     UFILES="$COBJ"
     ;;

#
# Generate no client files.
#

     "10")

     $IDL -cc_opt "$CC_OPT" -client none -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line_sstub.c"
     UFILES="$COBJ cmd_line_cstub.c"
     ;;

#
# Generate client auxiliary file, but not client stub.
#

     "11")

     $IDL -cc_opt "$CC_OPT" -client aux -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line_sstub.c"
     UFILES="$COBJ cmd_line_cstub.c"
     ;;

#
# Generate server auxiliary file, but not server stub.
#

     "12")

     $IDL -cc_opt "$CC_OPT" -server aux -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line_cstub.c"
     UFILES="$COBJ cmd_line_sstub.c"
     ;;

#
# Generate no server files.
#

     "13")

     $IDL -cc_opt "$CC_OPT" -server none -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line_cstub.c"
     UFILES="$COBJ cmd_line_sstub.c"
     ;;

#
# Specify the client stub filename.
#

     "14")

     $IDL -cc_opt "$CC_OPT" -cstub cmd_line.clt.stub.c -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line.clt.stub.c cmd_line_sstub.c"
     UFILES="$COBJ cmd_line_cstub.c"
     ;;

#
# Specify the server stub filename.
#

     "15")

     $IDL -cc_opt "$CC_OPT" -sstub cmd_line.svr.stub.c -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line_cstub.c cmd_line.svr.stub.c"
     UFILES="$COBJ cmd_line_sstub.c"
     ;;

#
# Specify the header filename.
#

     "16")

     $IDL -cc_opt "$CC_OPT" -header cmd_line.hdr.h -keep c_source 
     EXIT=$?

     EFILES="cmd_line.hdr.h cmd_line_cstub.c cmd_line_sstub.c"
     UFILES="$COBJ cmd_line.h"
     ;;

#
# Specify the client auxiliary filename.
#

     "17")

     $IDL -cc_opt "$CC_OPT" -caux cmd_line.clt.aux.c -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h cmd_line_cstub.c cmd_line_sstub.c"
     UFILES="$COBJ"
     ;;

#
# Specify the server auxiliary filename.
#

     "18") 

     $IDL -cc_opt "$CC_OPT" -saux cmd_line.svr.aux.c -keep c_source
     EXIT=$?

     EFILES="cmd_line.h cmd_line_cstub.c cmd_line_sstub.c"
     UFILES="$COBJ"
     ;;

#
# Suppress generation of enumeration literals.
#

     "19")

     $IDL -cc_opt "$CC_OPT" -no_enum_lit -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h $CSRC"
     UFILES=$COBJ
     ;;

#
# Generate no ouput; check syntax only.
#

     "20")

     $IDL -cc_opt "$CC_OPT" -syntax_only
     EXIT=$?

     EFILES=""
     UFILES="cmd_line.h $CSRC $COBJ"
     ;;

#
# Verify the -out command line option.
#

     "21")

     mkdir soupy

     $IDL -cc_opt "$CC_OPT" -out soupy -keep c_source 
     EXIT=$?

     EFILES="soupy/cmd_line.h soupy/cmd_line_cstub.c soupy/cmd_line_sstub.c"
     UFILES="soupy/cmd_line_cstub.o soupy/cmd_line_sstub.o"
     ;;

#
# Verify the -no_warn command line option.
#

     "22")

     $IDL -cc_opt "$CC_OPT" -no_warn -keep none 
     EXIT=$?

     EFILES="cmd_line.h"
     UFILES="$CSRC $COBJ"
     ;;

#
# Verify the -env command line option.
#

     "23")

     $IDL -cc_opt "$CC_OPT" -env -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h $CSRC"
     UFILES=$COBJ
     ;;

#
# Verify the -bug command line option.
#

     "24")

     $IDL -cc_opt "$CC_OPT" -bug 1 -keep none 
     EXIT=$?

     EFILES="cmd_line.h"
     UFILES="$CSRC $COBJ"
     ;;

#
# Verify the -no_bug command line option.
#

     "25")

     $IDL -cc_opt "$CC_OPT" -no_bug 1 -keep c_source 
     EXIT=$?

     EFILES="cmd_line.h $CSRC"
     UFILES=$COBJ
     ;;

#
# Verify the -list command line option.
#
#
#     "26")
#
#     $IDL -cc_opt "$CC_OPT" -list cmd_line.list -keep c_source
#     EXIT=$?
#
#     EFILES="cmd_line.h $CSRC cmd_line.list"
#     UFILES=$COBJ
#     ;;

#
# Verify the -no_def_idir command line option.
#

     "27")

     $IDL -cc_opt "$CC_OPT" -no_def_idir -keep c_source
     EXIT=$?

     EFILES="cmd_line.h $CSRC"
     UFILES=$COBJ

#
# End case $2 in
#

esac 

#
# Verify IDL output.  First check to see that all the files that were 
# supposed to be generated were; then make sure that files that were not
# supposed to be generated were not.
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
    if test $TRACE = "1" -a $FAIL != "1"
    then
      echo "Trace Message:     Expected file $LCV not found"
    if test $SFLAG = "1"
      then
        echo "Trace Message:     Expected file $LCV not found" 1>&2
      fi
    fi
    if test $FAIL = "1"
    then
      echo "Failure Message:   Expected file $LCV not found"
    if test $SFLAG = "1"
      then
        echo "Failure Message:   Expected file $LCV not found" 1>&2
      fi
    fi
  fi
done

for LCV in $UFILES
do 
  if test -f "$LCV"
  then
    EXIT=1
    if test $TRACE = "1" -a $FAIL != "1"
    then
      echo "Trace Message:     Unexpected file $LCV found"
    if test $SFLAG = "1"
      then
        echo "Trace Message:     Unexpected file $LCV found" 1>&2
      fi
    fi
    if test $FAIL = "1"
    then
      echo "Failure Message:   Unexpected file $LCV found"
    if test $SFLAG = "1"
      then
        echo "Failure Message:   Unexpected file $LCV found" 1>&2
      fi
    fi
  fi
done

#
# Cleanup leftover test files.  
#

\rm -f cmd_line*.h cmd_line*.c cmd_line*.o cmd_line.list

if test $CASEVAL = "21"
then
  \rm -r soupy
fi

#
# Exit this shell and return error code.
#

if test $SUCCESS = "1" -a $EXIT -eq 0
then
  echo "Test Status:       Test SUCCEEDED"
  if test $SFLAG = "1"
  then
    echo "Test Status:       Test SUCCEEDED" 1>&2
  fi
fi

if test $FAIL = "1" -a $EXIT -gt 0
then
  echo "Test Status:       Test FAILED"
  if test $SFLAG = "1"
  then
    echo "Test Status:       Test FAILED" 1>&2
  fi
fi

exit $EXIT

#
# End script cmd_line.sh
#
