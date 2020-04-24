#!/bin/sh

#
# Basic functions
#

#
# log
# 
# arguments
#   $1 = comment
#
log()
{
	msg=$1
	echo "[ `date` | $msg ]"
}

#
# logstarttest
#  
# arguments
#	none
#
# assumptions
# 	Env. Var FSETTESTNAME is correctly set

logstarttest()
{
	test=$FSETTESTNAME
	log "*****************************************************************"
	log "**** Starting fileset test $test at `date` "
	log "*****************************************************************"
	log ""
}

#
# logcasestart
#
# arguments
#  	$1 = case number
#

logcasestart()
{
	testcase=$1
	log ""
	log "*****************************************************************"
	log "Starting testcase $testcase at `date` "
}

# run
# 	run a command.
#	
#	arguments
#	$1 = command ( with all args )
#	$2 = expected result ( -1 is don't care flag )
#   $3 = error message
#	$4 = fatal failure flag. If this is 1, then on error the script exits.
# 	
#	If $VERBOSE != 0, this script prints lots of messages
#

run()
{
	if [ "$VERBOSE" -ne 0 ]
	then
		set -x
	fi

	cmd=$1
	expectedResult=$2
	errorMsg=$3
	fatalFlag=$4

	# Parameter validation
	if [ "$expectedResult" -eq -1 -a "$fatalFlag" -eq $FATAL ]
	then
		log "run: Cannot specify \"don't care result\" and \"fatal error\" "
		exit 2
	fi

	if [ "$fatalFlag" -eq "$FATAL" ]
	then
		failMsg="FATAL FAILURE"
	else
		failMsg="FAILURE"
	fi

	log "Executing $cmd"
	
	eval $cmd
	result=$?

	if [ "$expectedResult" -ne -1 ] 
	then
		if [ "$result" -ne "$expectedResult" ] 
		then
			FAILCNT=`expr $FAILCNT + 1`
			log "$failMsg: $errorMsg"
			log "   $cmd, expected $expectedResult, got $result"

			if [ "$fatalFlag" -eq "$FATAL" ]
			then
				log "exiting"
				exit 1
			fi
		else 
			if [ "$VERBOSE" -ne 0 ]
			then
				log "	success"
			fi
		fi
	else
		if [ "$VERBOSE" -ne 0 ]
		then
		    log "DONTCARE about result"
		fi
	fi
}


# fatal - 
# 	prints error message and exits.
#
#   arguments
#	$1 = actual result 
# 	$2 = expected result
# 	$3 = error message

fatal()
{
	if [ "$VERBOSE" -ne 0 ]
	then
		set -x
	fi

	if [ "$1" -ne "$2" ] 
	then
		log "$3"
		exit 1
	fi
}	

# error
#  	prints error message 
#
#   arguments
#	$1 = actual result 
# 	$2 = expected result
# 	$3 = error message

error()
{
	if [ "$VERBOSE" -ne 0 ]
	then
		set -x
	fi

	if [ "$1" -ne "$2" ] 
	then
		echo "$3"
	fi
}

# finishup
#   complete end-of-test processing
#
#   arguments
#   $1 = testname
finishup()
{
	if [ "$FAILCNT" -ne 0 ] 
	then
		log "Test *$1* FAILED - $FAILCNT failures"
	else
		log "Test *$1* passed"
	fi
}


#
# calcFsetID
#  $1 = base id
#  $2 = offset id
#
calcFsetID()
{
	fsetId=`expr $1 + $2`
	echo $fsetId
}

#
# detFsetName
#  $1 = name prefix
#  $2 = offset id
#
detFsetName()
{
	fsetName="${1}${2}"
	echo $fsetName
}

#
# runcthon -
#
# Arguments - 
#   $1 - test dir
# 	$2 - test number
# 
runcthon()
{
	testDir=$1
	testNum=$2
	log "Running CTHON test $test"
	( cd $testDir; 
	  log "  in dir `pwd`"; 
	  run "$CTHONDIR/test${test}" 0 "Cthon test $testNum" "$FATAL"
    )
}

##############################################################################

# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
