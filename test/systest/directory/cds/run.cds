#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: run.cds,v $
# Revision 1.1.7.2  1996/03/11  02:40:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:06:12  marty]
#
# Revision 1.1.7.1  1995/12/11  21:57:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:57:07  root]
# 
# Revision 1.1.4.1  1994/03/02  20:28:17  qbarry
# 	Removed the code that causes DCECDSACL6 not to run on a machine other
# 	than the CDS server machine.
# 
# 	Unofficial revisons made from the 1.0.3 version while waiting for 1.1 tree.
# 	1-Dec-93 9:00 qbarry
# 	Removed the code that causes DCECDSACL6 not to run on a machine other
# 	than the CDS server machine.  This was done after a quicky experiment
# 	showing that restriction didn't always hold.  More testing is needed.
# 	[1994/03/02  20:16:16  qbarry]
# 
# Revision 1.1.2.2  1993/10/07  18:21:47  qbarry
# 	This version now runs both DCECDSACL6 and CDSSERV.  Completely
# 	revamped the help stuff.  Added the -L, -T, -u, and -p options for
# 	CDSSERV.  Added the -q option for both tests.  Made the whole thing
# 	much more user resistant.
# 	[1993/10/06  19:41:55  qbarry]
# 
# 	Added stuff required to handle cell name params following test name.
# 	[1993/10/05  18:02:16  qbarry]
# 
# Revision 1.1.2.1  1993/09/30  20:19:25  qbarry
# 	This is the first real version of RUN.CDS.  It runs DCECDSACL6 successfully.
# 	[1993/09/30  20:17:07  qbarry]
# 
# 	First version which has all the old RPC stuff out and the new stuff in
# 	place for DCECDSACL6.  All the -m/-M option stuff is commented out
# 	because there are no tests which use it right now.
# 	[1993/09/29  19:56:48  qbarry]
# 
# 	Original version.  Derived from RUN.RPC version 1.1.10.7.  Ripped out
# 	all the old comments but is still mostly RPC stuff as yet.
# 	[1993/09/28  21:36:41  qbarry]
# 
# $EndLog$
# Driver for tet CDS system tests
# Written by Rick O'Brien
# Modified EXTENSIVELY by Jeff Barry
LOOPS_TO_EXECUTE=""
HOURS_TO_EXECUTE=""
KEEP_ALL_JOURNALS=""
callwas="$*"
if [[ -z $TET_ROOT ]]
then
        echo TET_ROOT not defined.
        exit 1
fi

myname=${0##*/}

Usage() {
    cat <<BeQuiteNow

Usage: $myname {-l loops | -t hours} [other opts] test_name [cell name(s)]
Options:
  -l <number>	Number of loops or passes to run.
  -t <hours>	Time in hours.  Accepts decimal point, eg -t .5 = 30 minutes.
  -c		Keep the journal files from successful iterations.
  -e <number>	Max errors-in-a-row allowed before punting test (default=50).
  -E <number>	Max total errors allowed before punting test (default=500).
  -h		List test-specific options.
  -q <level>	SET CDSCP CONFIDENCE = low|medium|high (default=medium).
Args:
  Test name: dcecdsacl6|cdsserv|all
  Cell name(s): Zero or more DCE cells to run on.  Default is local cell.
  -l and -t are mutually exclusive, and one or the other is required.
  Quotes around a string arg may be omitted when arg contains no whitespace.

BeQuiteNow
# NOTE: AS OF THE MOMENT NO CDS TEST REQUIRES A MACHINE OPTION, SO ALL THE -M/-m STUFF IS COMMENTED OUT.
#	-m	Machine(s) to work with.  Multiple -m <name> options
#		and -m \"name1 ... nameN\" are ok.
#	-M	Get machine(s) to work with from <testname>.data file.
#    -l and -t are mutually exclusive." #, as are -m and -M
#	Either -m or -M or env. var. QWERTY_WHAT_VAR_NAME? are required for tests
#	\"all\" or QWERTY_DCECDS????.
	exit 2
}


havmopt=0
while getopts :l:L:t:T:e:E:chu:p:q: arguments	#QWERTY put Mm: back in if re-enabling m switches
do
	case $arguments in

		l)      LOOPS_TO_EXECUTE=$OPTARG;;
		L)	export LOOPNUM=$OPTARG;;	# CDSSERV internal loops/principal

		t)      HOURS_TO_EXECUTE=$OPTARG;;
		T)	export HOURS=$OPTARG;;		# CDSSERV internal loop time/principal

		e)	MaxErrsInARow=$OPTARG;;

		E)	MaxErrors=$OPTARG;;

		u)	export PRINCS=$OPTARG;;		# CDSSERV principals

		p)	export PWS=$OPTARG;;		# CDSSERV passwords for principals

		q)	export CONFIDENCE=$OPTARG
			[[ "$CONFIDENCE" != "high" && "$CONFIDENCE" != "medium" && "$CONFIDENCE" != "low" ]] && \
				echo "ERROR: Bad -q option value \"$CONFIDENCE\"" && \
				(Usage | grep -e "-q") && exit 1 ;;

#		m)	if ((havmopt<0))
#			then
#				echo "-m <machine> and -M are mutually exclusive."
#				Usage
#				exit 1
#			elif ((havmopt==0))
#			then
#				QWERTY_WHAT_VAR_NAME?="$OPTARG"
#			else
#				QWERTY_WHAT_VAR_NAME?="$QWERTY_WHAT_VAR_NAME? $OPTARG"
#			fi
#			((havmopt+=1))
#			export QWERTY_WHAT_VAR_NAME?;;
#
#		M)	if ((havmopt>0))
#			then
#				echo "-m <machine> and -M are mutually exclusive."
#				Usage
#				exit 1
#			fi
#			QWERTY_WHAT_VAR_NAME?=""
#			havmopt=-1
#			export QWERTY_WHAT_VAR_NAME?;;

		c)	KEEP_ALL_JOURNALS="YES";;

		h)	cat <<SayNoMore

TEST: cdsserv
  -L <number>	Number of internal loops to do per principal (default=1, > 999
		means forever).  -L & -T are mutually exclusive.
  -T <hours>	Hours to run internal loop per principal.  Accepts decimal
		point, eg -T .5 = 30 minutes.
  -u "<names>"	Principals to use: -u "name1 ... nameN" (default=cell_admin).
  -p "<pswds>"	Passwords for -u principals in same order (default=-dce-).

TEST: dcecdsacl6
  Will fail if cell has more than one CDS server.

TEST: all
  Run all of the above.

SayNoMore
			exit 1;;

		:)	echo "	-$OPTARG requires an argument"
			Usage
			exit 1;;

		\?)	echo "	-$OPTARG is not a valid switch"			
			Usage
			exit 1;;

         esac
done
shift OPTIND-1


if [ -n "$1" ]
then
	test_name=$1	# Assign test name if there is one on command line.
	shift
	CELLS="$*"	# Assign rest of args if any to remainder string
else
	echo "ERROR: Test name missing.  Use \"all\" to run all tests."
	Usage
	exit 1
fi
export CELLS

[[ $test_name = "dcecdsacl6" ]] && [[ -n "$LOOPNUM" || -n "$HOURS" || -n "$PRINCS" || -n "$PWS" ]] && \
	echo "ERROR: Illegal option(s) given for running $test_name" && exit 1

[[ -n "$LOOPNUM" && -n "$HOURS" ]] && \
	echo "ERROR: You cannot set both -T and -L options." && exit 1

# There are no special case tests as of the moment.  Just recognize the test names.
if [[ $test_name = "all" || $test_name = "cdsserv" || $test_name = "dcecdsacl6" ]]
then
	:

#elif [[ $test_name = "all" || $test_name = "dcecds????" ]]
#then
#        if [[ $havmopt -eq 0 && -z "$QWERTY_WHAT_VAR_NAME?" ]]
#        then
#		echo "Test \"$test_name\" requires the -m or -M switch or the"
#		echo "env. var. QWERTY_WHAT_VAR_NAME? to specify remote machines."
#		Usage
#		exit 1
#        fi

else
	echo "Test name \"$test_name\" is unknown."
	exit 1
fi

#
# Now run the main loops.
#
. $TET_ROOT/system/tools/run_loops.ksh directory/cds "run.cds $callwas"
