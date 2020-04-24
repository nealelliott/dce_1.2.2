#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: skeleton.st,v $
# Revision 1.1.8.2  1996/03/11  02:49:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:10  marty]
#
# Revision 1.1.8.1  1995/12/11  22:51:24  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:23:47  root]
# 
# Revision 1.1.3.3  1993/03/05  20:52:07  pellis
# 	Commented out tput clear command.
# 	[1993/03/05  20:39:54  pellis]
# 
# Revision 1.1.3.2  1993/02/05  19:34:46  zeliff
# 	Embedding copyrights
# 	[1993/02/05  19:30:37  zeliff]
# 
# $EndLog$
#
#
#
#   FILE_NAME: skeleton.st
#
#   COMPONENT_NAME: dce.systest
#
#   FUNCTIONS: check_RC
#              check_dependencies
#              cleanup_workstation
#              execute_testcases
#              execute_workstation
#              fail_workstation
#              initialize_command_line_variables
#              initialize_environment_variables
#              log_workstation_parameters
#              one_time_workstation_setup
#              parse_command_line
#              pass_workstation
#              setup_workstation
#              show_usage
#
#   ORIGINS: 27
#
#
#   (C) COPYRIGHT International Business Machines Corp. 1991
#   All Rights Reserved
#
#
#   FOR_TEST: testcase_name(s)
#
#   USAGE:
#      See show_usage function below.
#
#   DESCRIPTION:
#      This file is the DCE System Test testcase driver.  Search
#      for >>> to find the locations in this file you need to
#      alter.
#
#      Note:  "Workstation" is the System Test term for a collection
#      of one or more testcases.
#
#   HISTORY: 
#                 Claussen/Carpenter   Initial code
#      10/30/90   Ken Walker           Initial changes for DCE
#       1/02/91   Ken Walker           "Iteration 0" for workstation setup,
#                                      minor usability changes
#       4/29/91   Ken Walker           Added TCSUBDIR and STROOT variables
#                                      to fit into new directory structure
#

#>>> Update this function to add the options required by your testcases. <<<
#===============================================================================
# Function:  show_usage
# Purpose:   Display a detailed syntax of the workstation's command line
#            options.
#===============================================================================
show_usage() {
cat <<-EOF

Usage:  $WS [options] [case_nums]
Where [options] are:
 -c          Do not clean up the workstation log files (default: TRUE)
 -e          Exclude testcases that are found on the command line
 -h          Show detailed usage for the workstation
 -l<LOOPS>   Times to execute the workstation (default: 1)
 -t<HOURS>   Hours to execute the workstation 
and  case_nums  is a list of the testcases to execute or exclude (default: ALL)

EOF
}


#===============================================================================
# Function:  check_RC
# Purpose:   This function is sent a return code and a message.  If
#            the return code is not equal to zero, then it prints out the
#            message, shows the workstation usage, and exits the workstation.
#===============================================================================
check_RC() {
	RC="$1"
	ERROR_MESSAGE="$2"
	if [ "$RC" != "0" ]; then
		xx_severe "$ERROR_MESSAGE"
		show_usage
		WS_FINISH_TIME_DATE=`$TIME_DATE`
		fail_workstation
	fi
}


#>>> Update this function to double-check that the environment you need is <<<
#>>> already set up.  Leave the checks for the driver's tools in there.    <<<
#>>> If you perform any commands, redirect stdout and stderr to the        <<<
#>>> spoolfile (>>$SPOOLFILE 2>&1).                                        <<<
#===============================================================================
# Function:  check_dependencies
# Purpose:   Check to make sure the tools that are needed are on the system.
#===============================================================================
check_dependencies() {

# Examples
#	[ "$MACHINE" ]
#	check_RC "$?" "Machine environment variable is not set."
#
#	[ "$LOGNAME" = "root" ]
#	check_RC "$?" "User ID must be root."

	[ ! -z "$STDEXC" ]
	check_RC "$?" "The System Test Driver variable [STDEXC] must be set."
}


#===============================================================================
# Function:  initialize_command_line_variables
# Purpose:   Do the basic initialization for the workstation.
#===============================================================================
initialize_command_line_variables() {
	CLEAN="TRUE"			#Clean up the JOURNALs
	EXCLUDE_FLAG="FALSE"		#Exclude command line testcase numbers
	EXECUTE_BY_TIME="FALSE"		#Execute by loops by default
	LOOPS_TO_EXECUTE="1"		#Execute one loop by default
	HOURS_TO_EXECUTE="0"		#Hours to execute by default is 0
	SECONDS_TO_EXECUTE="0"		#Seconds to execute by default is 0

	export CLEAN		#These may be used by underlying testcases
}


#===============================================================================
# Function:  initialize_environment_variables
# Purpose:   Initialize all the rest of the workstation variables.
#===============================================================================
initialize_environment_variables () {
	#>>> Set TCSUBDIR to the name of the subdirectory (see above) in which
	#>>> your testcase files will reside.
	#-----------------------------------------------------------------------
	# PID:    workstation's PID that we will use to distinguish files/dirs
	# HOST:   name of the host that the workstation was executed from
	# STLIB:  root directory of the entire test library
	# STROOT: root directory of the system test testcases
	# TCSUBDIR: subdirectory in which your test files will be located--
	#         choose from threads,rpc,time,security,directory,file,diskless
	# TCDIR:  location of this workstation's workstation and testcase files
	# STDEXC: directory where system test tools are stored
	#-----------------------------------------------------------------------
	PID=$$
	HOST=`hostname | cut -f1 -d.`	# Strip off TCP/IP domain
	STLIB=${STLIB:-/u/stlib}
	STROOT=${STROOT:-$STLIB/systest}
	TCSUBDIR=
	TCDIR=$STROOT/$TCSUBDIR
	STDEXC=${STDEXC:-$STROOT/tools}
	PATH=$TCDIR:$PATH
	
	#-----------------------------------------------------------------------
	# BASE_FAIL_DIR: directory contains directories of failed workstations
	# BASE_PASS_DIR: directory contains directories of passed workstations
	# PASS_FAIL_LOG: file containing workstation pass/fail information
	#-----------------------------------------------------------------------
	BASE_FAIL_DIR=${BASE_FAIL_DIR:-/tmp}
	BASE_PASS_DIR=${BASE_PASS_DIR:-/tmp}
	PASS_FAIL_LOG=${PASS_FAIL_LOG:-/tmp/pass_fail_log}
	
	#>>> Make sure each of the testcases controlled by this driver <<<
	#>>> are included in both lists below.                         <<<
	#-----------------------------------------------------------------------
	# TC_FILE_LIST: list of testcase file names
	# TC_NAME_LIST: list of testcase names
	#-----------------------------------------------------------------------
	TC_FILE_LIST="test.1 test.2"
	TC_NAME_LIST="ls grep"
	
	#-----------------------------------------------------------------------
	# TIMER: called like [T=`$TIMER`].  T equals seconds since beg. of year
	# TIME_DATE: called like [T=`$TIME_DATE`].  T = 11:44:50-03/16
	#-----------------------------------------------------------------------
	TIMER='eval echo `awk "BEGIN { print \`date +"%j"\`*86400 + \`date +"%H"\`*3600 + \`date +"%M"\`*60 + \`date +"%S"\`; exit}"`'
	TIME_DATE="date +%H:%M:%S-%m/%d"
	
	#-----------------------------------------------------------------------
	# Export all the variables needed by the testcase files
	#-----------------------------------------------------------------------
	export PID HOST PATH TIME_DATE

	#-----------------------------------------------------------------------
	# Special variables used by "iteration 0" (the initial setup) of the
	# workstation:
	# WPHI:       combination/substition - $WS.$PID.$HOST.$ITERATION
	# WSLOGDIR:   directory where our JOURNALs are kept during the run
	# JOURNAL:    file containing output from the standard library commands
	# SPOOLFILE:  file containing all information written to stdout
	#-----------------------------------------------------------------------
	WPHI="$WS.$PID.$HOST.0"
	ITERATION=0

	WSLOGDIR=${STLOGDIR:-/u/status}/$WPHI
	mkdir $WSLOGDIR
	
	JOURNAL=$WSLOGDIR/JOURNAL
	SPOOLFILE=$WSLOGDIR/spoolfile

	export JOURNAL
}



#>>> If you added options that will be used by the driver (not passed <<<
#>>> directly to the testcases), update this function.                <<<
#===============================================================================
# Function:  parse_command_line
# Purpose:   Parse the command line parameters and set the proper flags
#	     and variables accordingly.
#===============================================================================
parse_command_line() {
	#-----------------------------------------------------------------------
	# Use getopt to set the command line parameters that were passed.  If
	# an invalid parameter is passed then give a help screen about the
	# workstation by calling show_usage and then exit.
	#-----------------------------------------------------------------------
	set -- `getopt cehl:t: $COMMAND_LINE_PARAMETERS` \
		>>$SPOOLFILE 2>&1
	check_RC "$?" "Invalid command line parameters"
	
	#----------------------------------------------------------------------
	# Set variables depending on the flag encountered and then shift that
	# variable and/or flag out.
	#----------------------------------------------------------------------
	while :; do
		case $1 in
			-c)	CLEAN="FALSE"
				shift;;
			-e)	EXCLUDE_FLAG="TRUE"
				shift;;
			-h)	show_usage
				exit;;
			-l)	LOOPS_TO_EXECUTE="$2"
				shift
				shift;;
			-t)	HOURS_TO_EXECUTE="$2"
				EXECUTE_BY_TIME="TRUE"
				SECONDS_TO_EXECUTE=`awk "BEGIN { print 3600 * $HOURS_TO_EXECUTE; exit; }"`
				shift
				shift;;
			--)	shift
				break;;
		esac
	done

	#-----------------------------------------------------------------------
	# If there are any parameters left on the command line, they should
	# be a list of testcases that are either to be run or excluded for
	# the workstation run if the -e flag is specified.  The function
	# setup_workstation uses the COMMAND_LINE_TESTCASES variable.
	#-----------------------------------------------------------------------
	COMMAND_LINE_TESTCASES="$@"
}


#>>> If there are things that fit into this category, put them  <<<
#>>> in this function.  Redirect stdout and stderr of any       <<<
#>>> commands you perform to the spoolfile (>>$SPOOLFILE 2>&1). <<<
#===============================================================================
# Function:  one_time_workstation_setup
# Purpose:   Performs any setup procedures that need to be executed only
#	     once no matter how many times the testcases are run.
#===============================================================================
one_time_workstation_setup() {
	xx_log "Nothing to set up one time only"
}


#>>> If there are things that fit into this category, add them <<<
#>>> to this function.                                         <<<
#===============================================================================
# Function:  setup_workstation
# Purpose:   Performs any setup procedures that need to be executed before
#	     each run of the testcases.
#===============================================================================
setup_workstation() {

	trap   'echo
		echo "========================================"
		echo "Trap signal received..."
		echo "========================================"
		echo
		xx_severe "Signal received, exiting workstation..."
		cleanup_workstation
		exit 1' 1 2 3 15
	
	#>>> TC_NUM_LIST should have a number for each testcase that can <<<
	#>>> be run by this driver.                                      <<<
	#----------------------------------------------------------------------
	# Set the TC_NUM_LIST variable correctly.  This variable can vary
	# depending on whether or not an exclude flag was passed to the 
	# workstation, and command line testcase numbers that were passed to 
	# the workstation.
	#----------------------------------------------------------------------
	TC_NUM_LIST="1 2"
	
	if [ "$EXCLUDE_FLAG" = "TRUE" ]; then
		tmp="$TC_NUM_LIST $COMMAND_LINE_TESTCASES"	
		TC_NUM_LIST=`{ for i in $tmp; do echo $i; done; } | sort -n | uniq -u`
	elif [ -n "$COMMAND_LINE_TESTCASES" ]; then
		TC_NUM_LIST=$COMMAND_LINE_TESTCASES
	fi

	#-----------------------------------------------------------------------
	# WPHI:       combination/substition - $WS.$PID.$HOST.$ITERATION
	# WSTMPDIR:   the workstation temporary directory
	# WSLOGDIR:   directory where our JOURNALs are kept during the run
	# JOURNAL:    file containing output from the standard library commands
	# CMDFILE:    file containing all output from workstation commands
	# STATUSFILE: file containing other log information from the workstation
	# SPOOLFILE:  file containing all information written to stdout
	#-----------------------------------------------------------------------
	WPHI="$WS.$PID.$HOST.$ITERATION"

	WSTMPDIR=${STTMPDIR:-/tmp}/$WPHI
	mkdir $WSTMPDIR

	WSLOGDIR=${STLOGDIR:-/u/status}/$WPHI
	mkdir $WSLOGDIR
	
	JOURNAL=$WSLOGDIR/JOURNAL
	CMDFILE=$WSLOGDIR/cmdfile
	STATUSFILE=$WSLOGDIR/statusfile
	SPOOLFILE=$WSLOGDIR/spoolfile

	xx_log
	xx_log "--------------------------------------------------------------------------------"
	xx_log "SETUP for workstation"
	xx_log "--------------------------------------------------------------------------------"
	xx_log "     - Setting workstation trap"
	xx_log "     - Setting the WPHI environmental variable"
	xx_log "          * $WS.$PID.$HOST.$ITERATION"
	xx_log "     - Making the workstation temporary directory"
	xx_log "          * \$STTMPDIR/`basename $WSTMPDIR`"
	xx_log "     - Making the workstation log directory"
	xx_log "          * \$STLOGDIR/`basename $WSLOGDIR`"
	xx_log "     - Setting the log file environmental variables"
	xx_log "          * JOURNAL=\$WSLOGDIR/`basename $JOURNAL`"
	xx_log "          * CMDFILE=\$WSLOGDIR/`basename $CMDFILE`"
	xx_log "          * STATUSFILE=\$WSLOGDIR/`basename $STATUSFILE`"
	xx_log "          * SPOOLFILE=\$WSLOGDIR/`basename $SPOOLFILE`"
	
	export WSTMPDIR JOURNAL CMDFILE STATUSFILE SPOOLFILE
}
	
	
#>>> If you have added any options to this driver (whether they are <<<
#>>> being used by the driver or the testcases), make sure they get <<<
#>>> logged in this function.                                       <<<
#===============================================================================
# Function:  log_workstation_parameters
# Purpose:   Display the parameters that the workstation to execute will run
#            with.
#===============================================================================
log_workstation_parameters() {
	#-----------------------------------------------------------------------
	# This function logs the parameters that this workstation will be run
	# with and the testcases that will be run with these
	# parameters.  
	#-----------------------------------------------------------------------
	xx_log
	xx_log "--------------------------------------------------------------------------------"
	xx_log "PARAMETERS for workstation"
	xx_log "--------------------------------------------------------------------------------"
	if [ "$EXECUTE_BY_TIME" = "TRUE" ]; then
		xx_log "     - Executing by TIME"
		xx_log "          * $HOURS_TO_EXECUTE total hours to execute for"
		xx_log "          * $SECONDS_TO_EXECUTE total seconds to execute for"
	else
		xx_log "     - Executing by LOOPS"
		xx_log "          * $LOOPS_TO_EXECUTE total loops to execute for"
	fi
	xx_log "     - Clean:         $CLEAN"
	xx_log "     - Exclude flag:  $EXCLUDE_FLAG"
	xx_log "     - Testcase list:"
	for i in $TC_NUM_LIST; do 
		xx_log "          * `echo $TC_NAME_LIST | cut -f$i -d\" \"` - `echo $TC_FILE_LIST | cut -f$i -d\" \"`"
	done
}


#>>> Add cleanup procedures specific to your testcases here. <<<
#===============================================================================
# Function:  cleanup_workstation
# Purpose:   Leave the camp site better than when we found it
#===============================================================================
cleanup_workstation() {
	if [ "$CLEAN" = "TRUE" ]; then
		rm -rf $WSLOGDIR
		rm -rf $WSTMPDIR
	fi	
}


#===============================================================================
# Function:  execute_testcases
# Purpose:   Execute all the testcases requested.
#===============================================================================
execute_testcases() {
	#-----------------------------------------------------------------------
	# For loop through the entire set of testcase numbers in the current
	# testcase number list (TC_NUM_LIST).  
	#-----------------------------------------------------------------------
	for TN in $TC_NUM_LIST; do
	
		#---------------------------------------------------------------
		# Get the testcase filename and normal name our of the 
		# TC_FILE_LIST and the TC_NAME_LIST by cutting the out the
		# proper field.  Export the testcase name and number because
		# the testcase is reliant upon these two environmental variables
		#---------------------------------------------------------------
		TC_FILE=`echo $TC_FILE_LIST | cut -f$TN -d" "`
		TC_NAME=`echo $TC_NAME_LIST | cut -f$TN -d" "`
		export TC_NAME TN
		
		#---------------------------------------------------------------
		#  Log the beginning of the testcase and the starting time/date
		#---------------------------------------------------------------
		echo 
		xx_begin "--------------------------------------------------------------------------------"
		xx_begin "Beginning testcase [$TC_NAME($TN)-$TC_FILE] at `$TIME_DATE`"
		xx_begin "--------------------------------------------------------------------------------"
		echo 
			
		#---------------------------------------------------------------			
		# Check to make sure the testcase file is there
		#---------------------------------------------------------------			
		if [ ! -f $TCDIR/$TC_FILE ]; then
			echo 
			xx_fail "--------------------------------------------------------------------------------"
			xx_fail "Finished testcase [$TC_NAME($TN)-$TC_FILE] at `$TIME_DATE`.  File $TCDIR/$TC_FILE not found."
			xx_fail "--------------------------------------------------------------------------------"
			echo 
		else
			#-------------------------------------------------------
			# Run the testcase
			#-------------------------------------------------------
			$TCDIR/$TC_FILE
			if [ "$?" = "0" ]; then
				echo 
				xx_pass "--------------------------------------------------------------------------------"
				xx_pass "Finished testcase [$TC_NAME($TN)-$TC_FILE].  It PASSED at `$TIME_DATE`."
				xx_pass "--------------------------------------------------------------------------------"
				echo 
			else
				echo 
				xx_fail "--------------------------------------------------------------------------------"
				xx_fail "Finished testcase [$TC_NAME($TN)-$TC_FILE].  It FAILED at `$TIME_DATE`."
				xx_fail "--------------------------------------------------------------------------------"
				echo 
			fi
		fi
	done
}


#===============================================================================
# Function:  pass_workstation
# Purpose:   Mark the workstation as having passed and move its output files
#            to the save directory for passed workstations.
#===============================================================================
pass_workstation() {
	echo 
	xx_pass "================================================================================"
	xx_pass "Finished workstation [$WS($ITERATION)($PID)] at $WS_FINISH_TIME_DATE.  It PASSED."
	xx_pass "================================================================================"
	echo 
	banner "$WS" "iter. $ITERATION" "PASSED"
	mkdir $BASE_PASS_DIR/$WPHI >>$CMDFILE 2>>$CMDFILE
	cp -r $WSLOGDIR/* $BASE_PASS_DIR/$WPHI > /dev/null 2>&1
	touch $BASE_PASS_DIR/$WPHI/`date +%H:%M`
	echo "$WPHI $WS_START_TIME_DATE $WS_FINISH_TIME_DATE PASS $BASE_PASS_DIR/$WPHI/JOURNAL" >> $PASS_FAIL_LOG
}


#===============================================================================
# Function:  fail_workstation
# Purpose:   Mark the workstation as having failed and move its output files
#            to the save directory for failed workstations.
#===============================================================================
fail_workstation() {
	echo
	xx_fail "================================================================================"
	xx_fail "Finished workstation [$WS($ITERATION)($PID)] at $WS_FINISH_TIME_DATE.  It FAILED."
	xx_fail "================================================================================"
	echo    
	banner "$WS" "iter. $ITERATION" "FAILED"
	mkdir $BASE_FAIL_DIR/$WPHI >>$CMDFILE 2>>$CMDFILE
	cp -r $WSLOGDIR/* $BASE_FAIL_DIR/$WPHI > /dev/null 2>&1
	touch $BASE_FAIL_DIR/$WPHI/`date +%H:%M`
	echo "$WPHI $WS_START_TIME_DATE $WS_FINISH_TIME_DATE FAIL $BASE_FAIL_DIR/$WPHI/JOURNAL" >> $PASS_FAIL_LOG
}


#===============================================================================
# Function:  execute_workstation
# Purpose:   Perform the typical steps to execute the workstation.
#            These include workstation setup, execution, pass/fail
#            status reporting, and cleanup.
#===============================================================================
execute_workstation() {
	#tput clear

	#-----------------------------------------------------------------------
	#  Put the banner of workstation beginning out
	#-----------------------------------------------------------------------
	banner "$WS" "iter. $ITERATION" "beginning"
	
	#-----------------------------------------------------------------------
	#  Perform the workstation setup steps.
	#-----------------------------------------------------------------------
	setup_workstation
	
	#-----------------------------------------------------------------------
	#  Log the parameters used to start the workstation.
	#-----------------------------------------------------------------------
	log_workstation_parameters
	
	#-----------------------------------------------------------------------
	#  Set the start time and date of the workstation.
	#-----------------------------------------------------------------------
	WS_START_TIME_DATE=`$TIME_DATE`
	
	#-----------------------------------------------------------------------
	#  Log the beginning of the workstation, and iteration of the
	#  workstation and start time and date of the workstation.
	#-----------------------------------------------------------------------
	echo 
	xx_begin "================================================================================"
	xx_begin "Beginning workstation [$WS($ITERATION)($PID)] at $WS_START_TIME_DATE."
	xx_begin "================================================================================"
	echo 
	
	#-----------------------------------------------------------------------
	#  Call the function that executes all the testcases of the
	#  workstation.
	#-----------------------------------------------------------------------
	execute_testcases 2>&1 | tee -a $SPOOLFILE
	
	#-----------------------------------------------------------------------
	#  Set the finish time and date of the workstation.
	#-----------------------------------------------------------------------
	WS_FINISH_TIME_DATE=`$TIME_DATE`
	
	#-----------------------------------------------------------------------
	#  Check to see if the workstation passed or failed.
	#-----------------------------------------------------------------------
	#>>> Change the method of determining whether the testcase(s) <<<
	#>>> passed, if necessary.  If there are things you want to   <<<
	#>>> save besides the log/journal files, do those also.       <<<
	xx_examine
	if [ "$?" = "0" ] ; then
		#---------------------------------------------------------------
		# The workstation passed
		#---------------------------------------------------------------
		pass_workstation
	else            
		#---------------------------------------------------------------
		# The workstation failed
		#---------------------------------------------------------------
		fail_workstation
	fi

	#-----------------------------------------------------------------------
	#  Do all necessary workstation cleanup
	#-----------------------------------------------------------------------
	cleanup_workstation
}


#==============================================================================
# MAIN PROGRAM
#==============================================================================
#tput clear

WS=`basename $0` ; export WS
COMMAND_LINE_PARAMETERS="$@"

initialize_command_line_variables
initialize_environment_variables

#------------------------------------------------------------------------------
# Perform the overall workstation setup (iteration 0).
#------------------------------------------------------------------------------
WS_START_TIME_DATE=`$TIME_DATE`

parse_command_line
one_time_workstation_setup
check_dependencies

WS_FINISH_TIME_DATE=`$TIME_DATE`
pass_workstation	# If we made it this far, everything worked sufficiently
if [ "$CLEAN" = "TRUE" ]; then
	rm -rf $WSLOGDIR
fi

#-------------------------------------------------------------------------------
# Run the tests (iterations 1 - n)
#-------------------------------------------------------------------------------
ITERATION=1
START_TIME=`$TIMER`
DONE="FALSE"
while [ "$DONE" = "FALSE" ]; do
	#-----------------------------------------------------------------------
	# Execute the entire workstation 
	#-----------------------------------------------------------------------
	execute_workstation

	#-----------------------------------------------------------------------
	#  Increment the workstation iteration count
	#-----------------------------------------------------------------------
	ITERATION=`expr $ITERATION + 1`              

	#-----------------------------------------------------------------------
	#  Check to see if we should run the workstation again
	#-----------------------------------------------------------------------
	if [ "$EXECUTE_BY_TIME" = "TRUE" ]; then
		CURRENT_TIME=`$TIMER`
		SECONDS_EXECUTED=`expr $CURRENT_TIME - $START_TIME`
		if [ "$SECONDS_EXECUTED" -gt "$SECONDS_TO_EXECUTE" ]; then
			DONE="TRUE"
		fi
	else
		if [ "$ITERATION" -gt "$LOOPS_TO_EXECUTE" ]; then
			DONE="TRUE"
		fi
	fi
done


