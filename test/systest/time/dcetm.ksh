#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: dcetm.ksh,v $
# Revision 1.1.11.2  1996/03/11  02:49:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:09:50  marty]
#
# Revision 1.1.11.1  1995/12/11  22:50:14  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:23:24  root]
# 
# Revision 1.1.7.2  1993/07/28  19:38:38  cmckeen
# 	 HP's TET'ized version of the system tests
# 	[1993/07/27  14:56:29  cmckeen]
# 
# Revision 1.1.2.4  1993/06/03  18:16:05  eheller
# 	Fixed due to ode merge failure.
# 	[1993/06/03  18:15:25  eheller]
# 
# Revision 1.1.2.3  1993/05/27  18:59:30  eheller
# 	*** empty log message ***
# 
# Revision 1.1.1.2  1993/05/26  18:31:06  eheller
# 	*** empty log message ***
# 
# Revision 1.1.5.4  1993/04/06  15:13:32  truitt
# 	Modifications for new systest environment.
# 	[1993/04/06  15:13:09  truitt]
# 
# Revision 1.1.5.3  1993/03/29  17:43:46  truitt
# 	Added function support for dcetmchg system test.
# 	[1993/03/29  17:43:23  truitt]
# 
# Revision 1.1.5.2  1993/03/26  19:22:17  truitt
# 	First GAMERA submit.  Cleaned up existing functions and removed
# 	those functions that were not needed.
# 	[1993/03/26  19:19:26  truitt]
# 
# Revision 1.1.2.3  1993/01/18  14:47:26  bissen
# 	fixed change time functions
# 	[1993/01/18  14:46:15  bissen]
# 
# Revision 1.1.2.2  1992/11/03  18:08:14  mgm
# 	      Common functions for time TET tests.
# 	      Created by Bob Fritz.
# 	      [1992/11/03  18:07:13  mgm]
# 
# $EndLog$
#
# This file is dcetm.ksh
#
# 11/03/92      Bob Fritz       Original creator for TET testing
#               Mary MacGregor  Bci'd and bsubmit'd.
# 12/22/92      Bob Bissen      fixed dcetmchg functions
#
#
#==========================================================
# Function: check_RC_severe
# Purpose:  Input $1 is the return code of some command, 
#	    $2 is a string explaining the error.  tet_infoline
#	    is called with the error message and an return 1 occurs
#	    if the return code is not 0.
#==========================================================
check_RC_severe() {
#set -x
	RC="$1"		
	ERROR_MESSAGE="$2"
	if [ "$RC" != "0" ]; then
		print "ERROR: $ERROR_MESSAGE"
		tet_infoline "ERROR: $ERROR_MESSAGE"
		return 1
	fi
}

#==========================================================
# Function: check_RC_warning
# Purpose:  Input $1 is the return code of some command, 
#	    $2 is a string explaining the warning.  tet_infoline
#	    is called with the warning message
#	    if the return code is not 0.
#==========================================================
check_RC_warning() {
#set -x
	RC="$1"		
	WARNING_MESSAGE="$2"
	if [ "$RC" != "0" ]; then
		print "WARNING: $WARNING_MESSAGE"
		tet_infoline "WARNING: $WARNING_MESSAGE"
		return $RC
	fi
}

#==========================================================
# Function: check_format
# Purpose: 
#==========================================================
check_format() {
#set -x
	file="$1"		
	ERR="$2"
	grep ".*ast.*ynchroniz.*[0-9][0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9]-[0-2][0-9]:[0-5][0-9]:[0-5][0-9]" $file >/dev/null
	check_RC_severe "$?" "$ERR"
}

#==============================================================
# Function: setup_dcetmsyn
# Purpose:  save output of "show last sync"
#==============================================================
setup_dcetmsyn() {
#set -x
	MAXLOOPS=${MAXLOOPS:-50}

	print "Show Last Synchronization"
	tet_infoline "Show Last Synchronization"
	dtscp show last synchronization > $WSTMPDIR/lastsync
	cat $WSTMPDIR/lastsync

	print "Show synchronizations completed"
	tet_infoline "Show synchronizations completed"
	SYNCH_COMPLETE=`dtscp show synchronizations completed`
	echo $? > $WSTMPDIR/RC
	SYNCH_COMPLETE=`echo $SYNCH_COMPLETE | awk '{print $NF}'`
	print "Synchronizations completed is $SYNCH_COMPLETE"
	tet_infoline "Synchronizations completed is $SYNCH_COMPLETE"
	check_RC_severe `cat $WSTMPDIR/RC` "show synchronizations completed command returned error code"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	print "Make sure state is on"
	tet_infoline "Make sure state is on"
	STATE=`dtscp show state | awk '{print $NF}'`
	print "State of dts is $STATE"
	tet_infoline "State of dts is $STATE"
	if [ "$STATE" != "On" ]; then
		print "State must be on before attempting a synchronize"
		tet_infoline "State must be on before attempting a synchronize"
		return 1
	fi
}

#==============================================================
# Function: synch_time
# Purpose:  execute a synchronize command, wait for synch to finish
#	    and state to return to on from "Synchronizing".  If state 
#	    hasn't returned after MAXLOOPS seconds, fail because it's taking
#	    too long
#==============================================================
synch_time() {
#set -x
	print "Synchronize in synch_time()"
	tet_infoline "Synchronize in synch_time()"
	dtscp synchronize 
	check_RC_severe "$?" "synchronize command returned error"
	if [ "$?" -eq "1" ]; then
		return 1
	fi
	wait_for_on
	if [ "$?" -eq "1" ]; then
		return 1
	fi
}

#==============================================================
# Function: wait_for_on
# Purpose:  if state is synchronizing, wait for state to return to 'on'
#==============================================================
wait_for_on() {
#set -x
	print "Wait for state to return to 'on'"
	tet_infoline "Wait for state to return to 'on'"
	NumLoops=0
	state=`dtscp show state`
	state=`echo $state | awk '{print $NF}'`
	while [ "$state" != On ] && [ "$NumLoops" != "$MAXLOOPS" ]; do
		print "sleep 5"
		tet_infoline "sleep 5"
		sleep 5
		NumLoops=`expr $NumLoops + 1`
		state=`dtscp show state`
		state=`echo $state | awk '{print $NF}'`
	done
	if [ "$state" != On ]; then
		print "wait_for_on: Synchronization is taking too long"
		tet_infoline "wait_for_on: Synchronization is taking too long"
		return 1
	fi
	return 0
}
	
#==============================================================
# Function: verify_synch
# Purpose:  verify that a new synchronization occured
#==============================================================
verify_synch() {
#set -x
	print "Show last synchronization"
	tet_infoline "Show last synchronization"
	dtscp show last synchronization > $WSTMPDIR/newsync
        cat $WSTMPDIR/newsync
	check_RC_severe "$?" "show last synchronization command returned error code"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	print "Show synchronizations completed"
	tet_infoline "Show synchronizations completed"
	NEWSYNCH_COMPLETE=`dtscp show synch complete | awk '{print $NF}'`
	print "Synchronizations completed is $NEWSYNCH_COMPLETE"
	tet_infoline "Synchronizations completed is $NEWSYNCH_COMPLETE"

	print "Check that output of show last synch is right form"
	tet_infoline "Check that output of show last synch is right form"
	check_format "$WSTMPDIR/newsync" "output form of show last synch is wrong"
	check_format "$WSTMPDIR/lastsync" "output form of show last synch is wrong" 

	print "Compare values of show last synch"
	tet_infoline "Compare values of show last synch"
	diff $WSTMPDIR/newsync $WSTMPDIR/lastsync
	if [ "$?" -eq "0" ]; then
		check_RC_severe "1" "No new synchronization occured: show last synch"
		return 1
	fi

	print "Compare values of show synch complete"
	tet_infoline "Compare values of show synch complete"
	DIFF=`expr $NEWSYNCH_COMPLETE - \`expr $SYNCH_COMPLETE\``
	[ "$DIFF" -gt "0" ]
	check_RC_severe "$?" "No new synchronization occured: show synch complete"
	if [ "$?" -eq "1" ]; then
		return 1
	fi
}

#==========================================================
# Function: check_RC_severe_dcetmchg
# Purpose:  Input $1 is the return code of some command, 
#	    $2 is a string explaining the error.  tet_infoline
#	    is called with the error message and an return 1 occurs
#	    if the return code is not 0.
#==========================================================
check_RC_severe_dcetmchg() {
#set -x
	RC="$1"		
	ERROR_MESSAGE="$2"
	if [ "$RC" != "0" ]; then
		print "ERROR: $ERROR_MESSAGE"
		tet_infoline "ERROR: $ERROR_MESSAGE"
		cleanup_dcetmchg
		return 1
	fi
}

#==========================================================
# Function: finalcheck_severe
# Purpose:  Input $1 is the return code of some command, 
#	    $2 is a string explaining the error.  tet_infoline
#	    is called with the error message and an return 1 occurs
#	    if the return code is not 0.
#	    Use when in cleanup_dcetmchg
#==========================================================
finalcheck_severe() {
#set -x
	RC="$1"		
	ERROR_MESSAGE="$2"
	if [ "$RC" != "0" ]; then
		print "ERROR: $ERROR_MESSAGE"
		tet_infoline "ERROR: $ERROR_MESSAGE"
		return 1
	fi
}

#==========================================================
# Function: setup_dcetmchg
# Purpose:  Check that machine is a server, save original 
#	    epoch number, and determine an epoch number
#	    that is not being used
#==========================================================
setup_dcetmchg() { 
#set -x
	MAXLOOPS=${MAXLOOPS:-12}
	trap   'echo
	echo "========================================"
	echo "Trap signal received... dcetmchg"
	echo "========================================"
	echo
	print "setup: Exiting testcase..."
	tet_infoline "setup: Exiting testcase..."
	if [ "$CHANGEDTIME" != "" ]; then
		cleanup_dcetmchg
	fi
		return 1
	' 1 2 3 15

     	# Check to make sure machine is a server
	print "Check that `hostname` is a time server" 
	tet_infoline "Check that `hostname` is a time server" 
	dtscp show type | grep Server >/dev/null
	finalcheck_severe $? "Must be run on a time server"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	# Find a unique epoch number:
	# rsh to each server and find it's epoch number
	dtscp show local servers | grep Local | cut -f5 -d"/" | tee $WSTMPDIR/locals >/dev/null
	for i in `cat $WSTMPDIR/locals`; do
	    EpochNum=`rshsp $i "KRB5CCNAME=\`dce_login_noexec cell_admin -dce- >/dev/null 2>&1\`;export KRB5CCNAME;"dtscp show epoch number""`
	    echo $? > $WSTMPDIR/RC
	    echo "$i = \c" >> $WSTMPDIR/epochfile
	    echo $EpochNum | awk '{print $NF}' >> $WSTMPDIR/epochfile
	    check_RC_warning `cat $WSTMPDIR/RC` "setup: error on rsh to $i"
	done

	OLDEPOCH=`dtscp show epoch number | awk '{print $NF}'`
	echo "`hostname` = \c" >> $WSTMPDIR/epochfile
	echo $OLDEPOCH >> $WSTMPDIR/epochfile
	print "Show epoch number of local servers"
	tet_infoline "Show epoch number of local servers"
	cat $WSTMPDIR/epochfile >/dev/null

	print "Original epoch number is $OLDEPOCH"
	tet_infoline "Original epoch number is $OLDEPOCH"
	NEW_EPOCH=255
	EPOCH_FOUND=1
	print "Validate new epoch number"
	tet_infoline "Validate new epoch number"
	until [ "$EPOCH_FOUND" = "0" ]; do
		grep $NEW_EPOCH $WSTMPDIR/epochfile >/dev/null
		if [ "$?" = "0" ]; then
			NEW_EPOCH=`expr $NEW_EPOCH - 1`
		else
			EPOCH_FOUND=0
		fi
	done
	print "New epoch number is $NEW_EPOCH"
	tet_infoline "New epoch number is $NEW_EPOCH"
}

#==========================================================
# Function: adjust_time 
# Purpose:  keep the time within the 0-23 range
#==========================================================
adjust_time() {
#set -x
	print "adjust_time - Making sure the time is within the 0-23 range"
	tet_infoline "adjust_time - Making sure the time is within the 0-23 range"
	TIME1=$1
	if [ "$TIME1" -ge "24" ]; then
		REMAINDER=`expr $TIME1 - 24`
		TIME1="0"$REMAINDER
		return 0
	fi
	if [ "$TIME1" -le "9" ]; then
		TIME1="0"$TIME1
		return 0
	fi	
	if [ "$TIME1" -le "0" ]; then
		REMAINDER=`0 - expr $TIME1`
		TIME1="0"$REMAINDER
	fi	
}

#==========================================================
# Function: change_time
# Purpose:  save the current time, set the time to one hour
#	    ahead
#==========================================================
change_time() {
#set -x
	# show & save current time.
	print "Getting the current time"
	tet_infoline "Getting the current time"
	TIMEDIFF=`dtscp show current time | awk '{print $NF}' | cut -c24-26`
	STARTTIME=`dtscp show current time | awk '{print $NF}'`
	print "Original time is $STARTTIME"
	tet_infoline "Original time is $STARTTIME"

	# Get as much time as possible to complete time change within one minute
	MIN=`echo $STARTTIME | cut -c15-16`
	SEC=`echo $STARTTIME | cut -c18-19`
	if [ "$SEC" -gt "10" ]; then
		SLP=`expr 60 - $SEC`
		print "Minutes (${MIN}:${SEC}) too close to changing, sleeping for $SLP second(s)"
		tet_infoline "Minutes (${MIN}:${SEC}) too close to changing, sleeping for $SLP second(s)"
		sleep $SLP
		STARTTIME=`dtscp show current time | awk '{print $NF}'`
	fi

	# get the hour field
	print "Real start time is $STARTTIME"
	tet_infoline "Real start time is $STARTTIME"
	print "Converting the current time to one hour ahead"
	tet_infoline "Converting the current time to one hour ahead"
	CURRHOUR=`echo $STARTTIME | cut -c12-13`
	NEWLOCALHOUR=`expr $CURRHOUR + 1`;
	adjust_time "$NEWLOCALHOUR"
	NEWLOCALHOUR="$TIME1"
	print "New local hour is $NEWLOCALHOUR"
	tet_infoline "New local hour is $NEWLOCALHOUR"

	print "Do an adjust for TDF (timediff $TIMEDIFF)"
	tet_infoline "Do an adjust for TDF (timediff $TIMEDIFF)"
	if [ "$TIMEDIFF" != "+00" ] && [ "$TIMEDIFF" != "-00" ]; then 
		SIGN=`echo $TIMEDIFF | cut -c1`
		VALUE=`echo $TIMEDIFF | cut -c2-3`
		print "Time Differential Factor is $SIGN$VALUE"
		tet_infoline "Time Differential Factor is $SIGN$VALUE"
		case "$SIGN" in
			+)  NEWHOUR=`expr $CURRHOUR - \`expr \$VALUE\` + 1` ;;
			-)  NEWHOUR=`expr $CURRHOUR + \`expr \$VALUE\` + 1` ;;
			*)  print "change_time: dcetmchg invalid sign"
			    tet_infoline "change_time: dcetmchg invalid sign" 
			    return 1
			    ;;
		esac
	else
		print "TDF is 0"
		tet_infoline "TDF is 0"
		NEWHOUR=`expr $CURRHOUR + 1`
	fi

	adjust_time "$NEWHOUR"
	NEWHOUR="$TIME1"
	NEWTIME=`echo $STARTTIME | cut -c1-11`$NEWHOUR`echo $STARTTIME | cut -c14-23`
	NEWLOCALTIME=`echo $STARTTIME | cut -c1-11`$NEWLOCALHOUR`echo $STARTTIME | cut -c14-23`

	print "NEWTIME is $NEWTIME"
	tet_infoline "NEWTIME is $NEWTIME"
	print "NEWLOCALTIME is $NEWLOCALTIME"
	tet_infoline "NEWLOCALTIME is $NEWLOCALTIME"

	print "Make sure the state is on"
	tet_infoline "Make sure the state is on"
	wait_for_on

	print "Change the time to one hour ahead (utc), $NEWTIME"
	tet_infoline "Change the time to one hour ahead (utc), $NEWTIME"
	print "Do dtscp change epoch $NEW_EPOCH time (utc) $NEWTIME"
	tet_infoline "Do dtscp change epoch $NEW_EPOCH time (utc) $NEWTIME"

	dtscp change epoch $NEW_EPOCH time $NEWTIME > $WSTMPDIR/change_return
	finalcheck_severe "$?" "change_time: change epoch command returned bad error code"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	CHANGEDTIME=`dtscp show current time`
	CHANGEDTIME=`echo $CHANGEDTIME | awk '{print $NF}'`
	echo $CHANGEDTIME  | cut -c1-16 | tee $WSTMPDIR/changetime >/dev/nul
	echo $NEWLOCALTIME  | cut -c1-16 | tee $WSTMPDIR/newtime >/dev/null
	print "changetime (the changed time) and newtime (local time in utc) files are..."
        cat $WSTMPDIR/changetime
	cat $WSTMPDIR/newtime
}

#==========================================================
# Function: verify_time
# Purpose:  verify that time was changed correctly
#==========================================================
verify_time() {
#set -x
	print "Verify that the new time is almost the same as the time we set it to"
	tet_infoline "Verify that the new time is almost the same as the time we set it to"

	# Here we compare the times only to the minute level, cutting off seconds
	# because it may take a second or two between time set and display.
	print "Verify that a time change occured"
	tet_infoline "Verify that a time change occured"
	print "Time has been changed to $CHANGEDTIME"
	tet_infoline "Time has been changed to $CHANGEDTIME"
	print "Time should be changed to $NEWLOCALTIME"
	tet_infoline "Time should be changed to $NEWLOCALTIME"

	diff $WSTMPDIR/changetime $WSTMPDIR/newtime
	check_RC_severe_dcetmchg "$?"  "verify_time: Time not changed to value specified"
	if [ "$?" -eq "1" ]; then
		return 1
	fi
	CHANGEDEPOCH=`dtscp show epoch number`
	CHANGEDEPOCH=`echo $CHANGEDEPOCH | awk '{print $NF}'`

	print "Changed epoch is $NEW_EPOCH"
	tet_infoline "Changed epoch is $NEW_EPOCH"
	[ "$NEW_EPOCH" -eq "$CHANGEDEPOCH" ]
	check_RC_severe "$?" "verify_time: changing of epoch to new value failed"
	if [ "$?" -eq "1" ]; then
		return 1
	fi
}

#==========================================================
# Function: cleanup_dcetmchg
# Purpose:  reset epoch and time to original values  
#==========================================================
cleanup_dcetmchg() {
#set -x
	# reset trap
	trap '' 1 2 3 15
	print "Returning machine to its original numbers"
	tet_infoline "Returning machine to its original numbers"
	print "This will result in faulting the local machine"
	tet_infoline "This will result in faulting the local machine"
	OldErrTolerance=`dtscp show error tolerance`
	OldErrTolerance=`echo $OldErrTolerance | awk '{print $NF}' | cut -c2-15`
	SMERRTOL="00-00:00:03.000"
	print "temporarily set small error tolerance to $SMERRTOL"
	tet_infoline "temporarily set small error tolerance to $SMERRTOL"
	dtscp set error tolerance $SMERRTOL
	check_RC_warning "$?" "cleanup: Set error tolerance command failed"

	wait_for_on

	print "Do dtscp change epoch $OLDEPOCH"
	tet_infoline "Do dtscp change epoch $OLDEPOCH"
	dtscp change epoch $OLDEPOCH
	check_RC_warning "$?" "cleanup: change epoch command failed"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	# verify that parameters have been restored
	NEWEPOCH=`dtscp show epoch number | awk '{print $NF}'`
	if test ! $NEWEPOCH -eq $OLDEPOCH; then
		print "cleanup: Changing of epoch to original value failed"
		tet_infoline "cleanup: Changing of epoch to original value failed"
	fi
	print "Final epoch is $NEWEPOCH"
	tet_infoline "Final epoch is $NEWEPOCH"

	wait_for_on

	print "Run sync set clock true"
	tet_infoline "Run sync set clock true"
	dtscp sync set clock true
	check_RC_warning "$?" "cleanup: Sync set clock true failed"

	print "Restoring old error tolerance to $OldErrTolerance"
	tet_infoline "Restoring old error tolerance to $OldErrTolerance"
	dtscp set error tolerance $OldErrTolerance
	check_RC_warning "$?" "cleanup: Restore of error tolerance failed"
	ErrTolerance=`dtscp show error tolerance`
	ErrTolerance=`echo $ErrTolerance | awk '{print $NF}' | cut -c2-15`
	if test ! ${OldErrTolerance} = ${ErrTolerance}; then
		print "cleanup: Restoration of error tolerance to $OldErrTolerance failed"
		tet_infoline "cleanup: Restoration of error tolerance to $OldErrTolerance failed"
		return 1
	else
		return 0
	fi
}

#==========================================================
# Function: check_RC_severe_dcetmudt
# Purpose:  Input $1 is the return code of some command, 
#	    $2 is a string explaining the error.  tet_infoline
#	    is called with the error message and an return 1 occurs
#	    if the return code is not 0.
#==========================================================
check_RC_severe_dcetmudt() {
#set -x
	RC="$1"		
	ERROR_MESSAGE="$2"
	if [ "$RC" != "0" ]; then
		tet_infoline "ERROR: $ERROR_MESSAGE"
		cleanup_dcetmudt
		return 1
	fi
}

#=================================================================
# Function: setup_dcetmudt
# Purpose:  Verify that REMHOST is defined and is pingable.
#	    Determine an epoch that is not
#	    being used, and verify that inaccuracy is greater than
#	    60 seconds.  If not add 61 seconds to the inaccuracy to 
#	    allow the update to occur. Change the epoch.
#=================================================================
setup_dcetmudt() {
#set -x
	MAXLOOPS=${MAXLOOPS:-12}

	# Check to make sure machine is a server
	print "Check that machine is a time server" 
	tet_infoline "Check that machine is a time server" 
	dtscp show type | grep Server >/dev/null
	if [ "$?" != "0" ]; then
		print "Must be run on a time server"
		tet_infoline "Must be run on a time server"
		return 1
	fi

	# ENSURE THAT THE STATE OF DTS IS ON OR SYNCHRONIZING
	#	case `dtscp show state | awk '{print $NF}'` in
	#	On|Synchronizing) ;;
	#	*)  echo "$WS: SEVERE - State of DTS must be On or Synchronizing"
	#	    return 1;;
	#	esac

	# REMHOST must be defined when program is called 
	print "Make sure remote host is defined"
	tet_infoline "Make sure remote host is defined"
	if [ "$REMHOST" = "" ]; then
		print "Remote host not defined";
		tet_infoline "Remote host not defined";
		return 1;
	fi

	# Must be able to ping remote host
	print "Attempt to ping remote host $REMHOST"
	tet_infoline "Attempt to ping remote host $REMHOST"
	ping $REMHOST 56 1 >/dev/null
	check_RC_severe_dcetmudt "$?" "Cannot ping remote host $REMHOST"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	# Find a unique epoch number, rsh to each server and find it's epoch number
	dtscp show local servers | grep Local | cut -f5 -d"/" | tee $WSTMPDIR/locals

	for i in `cat $WSTMPDIR/locals`; do
		EpochNum=`rshsp $i "KRB5CCNAME=\`dce_login_noexec cell_admin -dce-\`;export KRB5CCNAME;"dtscp show epoch number""`
		check_RC_warning "$?" "error on rsh to $i"
		echo $EpochNum | awk '{print $NF}' >> $WSTMPDIR/epochfile
	done	
	OLD_EPOCH=`dtscp show epoch number | awk '{print $NF}'`
	echo $OLD_EPOCH >> $WSTMPDIR/epochfile
	print "Show epoch number of all local servers"
	tet_infoline "Show epoch number of all local servers"
	cat $WSTMPDIR/epochfile
	print "Original epoch number is $OLD_EPOCH"
	tet_infoline "Original epoch number is $OLD_EPOCH"
	NEW_EPOCH=255
	EPOCH_CHANGED=0
	EPOCH_FOUND=1
	until [ "$EPOCH_FOUND" = "0" ]; do
		grep $NEW_EPOCH $WSTMPDIR/epochfile
		if [ "$?" = "0" ]; then
			NEW_EPOCH=`expr $NEW_EPOCH - 1`
		else
			EPOCH_FOUND=0
		fi
	done

	# The update command will only allow a command to update to a time
	# that falls within the computed inaccuracy of the server.
	# check inaccuracy, ensure that inaccuracy is greater than 1 minute 
	# (may have to change the inaccuracy)
	# Cut the I (inaccuracy) field from STARTTIME
        # NOTE: this code expects there to actually *be* an inaccuracy value,
	# but since we are operating in an environment without an external
	# time provider, our inaccuracy will be infinite, so causing no problem
	# to the update.  This code is also wrong for our configuration.
	#print "Check the inaccuracy"
	#tet_infoline "Check the inaccuracy"
	#while ( true ); do
	#	INAC=`dtscp show current time | cut -c59-| cut -f1 -d"."`
	#	if [ "$INAC" = "-----" ]; then
	#		break
	#	fi
	#done

	# We will do our inaccuracy calculation this way...
	print "Check the inaccuracy"
	tet_infoline "Check the inaccuracy"

	INAC=`dtscp show current time | cut -c58- | cut -f1 -d"."`
	print "Inaccuracy is $INAC"
	tet_infoline "Inaccuracy is $INAC"
	
	TDF=`dtscp show current time | awk '{print $NF}' | cut -c24-29`
	print "Time differential factor is $TDF"
	tet_infoline "Time differential factor is $TDF"

	if [ "$INAC" = "I-----" ]; then
		print "Changing epoch to $NEW_EPOCH"
		tet_infoline "Changing epoch to $NEW_EPOCH"
		EPOCH_CHANGED=1;
		dtscp change epoch $NEW_EPOCH
		check_RC_severe_dcetmudt "$?" "change command error"
		if [ "$?" -eq "1" ]; then
			return 1
		fi
	else
		if [ "$INAC" -lt "61" ]; then
			TM=`dtscp show current time | cut -c29-47`
			INAC=`expr $INAC + 61`
			TM=$TM".000"$TDF"I"$INAC
			EPOCH_CHANGED=1;
			print "dtscp change epoch $NEW_EPOCH time $TM"
			tet_infoline "dtscp change epoch $NEW_EPOCH time $TM"
			dtscp change epoch $NEW_EPOCH time $TM
			check_RC_severe_dcetmudt "$?" "change command error"
			if [ "$?" -eq "1" ]; then
				return 1
			fi
		fi
	fi
	trap   'echo
	echo "======================================"
	echo "       Trap signal received..."
	echo "======================================"
	echo
	cleanup_dcetmudt
	return 1
	' 1 2 3 15
}

#=================================================================
# Function: get_starttime
# Purpose:  save the initial time
#=================================================================
get_starttime() {
#set -x
	print "Make sure state is on"
	tet_infoline "Make sure state is on"
	wait_for_on

	print "Get the start time"
	tet_infoline "Get the start time"
	STARTTIME=`dtscp show current time | awk '{print $NF}'`
	S_YEAR=`echo $STARTTIME | cut -c2-4`
	S_MONTH=`echo $STARTTIME | cut -c6-7`
	S_MONTH=`expr $S_MONTH - 1`
	S_DAY=`echo $STARTTIME | cut -c9-10`
	S_HOUR=`echo $STARTTIME | cut -c12-13`
	S_MIN=`echo $STARTTIME | cut -c15-16`
	S_SEC=`echo $STARTTIME | cut -c18-19`

	print "Current time is $STARTTIME"
	tet_infoline "Current time is $STARTTIME"
}

#=================================================================
# Function: update_time
# Purpose:  update the time to the current time + 60 seconds.
#	    rsh to REMHOST machine (non-DTS machine) , and do sleep 6000
#	    (this will cause a sleep for 40 minutes). 
#=================================================================
update_time() {
#set -x
	print "Calculate current time + 60 seconds"
	tet_infoline "Calculate current time + 60 seconds"
	UPDATETIME=`$WSTMPDIR/addtime`
	check_RC_severe_dcetmudt "$?" "addtime returned error"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	print "Update time is $UPDATETIME".000"$TDF"I1.000""
	tet_infoline "Update time is $UPDATETIME".000"$TDF"I1.000""
	dtscp update time $UPDATETIME".000"$TDF"I1.000"
	check_RC_severe_dcetmudt "$?" "dtscp update time command error"
	if [ "$?" -eq "1" ]; then
		return 1
	fi
	STATE=`dtscp show state | awk '{print $NF}'`
	print "State is $STATE"
	tet_infoline "State is $STATE"
	[ "Updating" = "$STATE" ]
	check_RC_warning $? "State of DTS is not Updating."

	#rsh to a remote trusted time machine and execute "sleep 6000"
	print "Running... (rsh to remote host) Please wait 100 min."
	tet_infoline "Running... (rsh to remote host) and sleep 6000"
	rshsp $REMHOST "sleep 6000"
	check_RC_severe_dcetmudt "$?" "error on rsh to $REMHOST" 
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	STATE=`dtscp show state | awk '{print $NF}'`
	print "State is $STATE"
	tet_infoline "State is $STATE"
	[ "Updating" != "$STATE" ]
	check_RC_severe_dcetmudt $? "State of DTS is still updating."
	return 0
}

#=================================================================
# Function: length_time
# Purpose:  check to be sure that the time difference between when
#	    we started the update on this machine, and when 100 real
#	    minutes later is ~41 minutes. (The machine speeded up it's
#	    clock to update it's time.)
#=================================================================
length_time() {
#set -x
	print "Calculate time difference, expect to see minutes changed by ~41 min"
	tet_infoline "Calculate time difference, expect to see minutes changed by ~41 min"

	# We expect the system to have advanced 101 min during a real 100 min period
	# subtime subtracts the STARTTIME from the current time and returns the 
	# difference of hours, minutes and seconds.  Input to subtime is the 
	# start time separated into year, month, day, hour, minute and second.

	DIFFTIME=`$WSTMPDIR/subtime $S_YEAR, $S_MONTH, $S_DAY, $S_HOUR, $S_MIN, $S_SEC`
	check_RC_severe_dcetmudt "$?" "subtime returned error"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	print "Time difference is $DIFFTIME"
	tet_infoline "Time difference is $DIFFTIME"
	DIF_HR=`echo $DIFFTIME | cut -f1 -d:`
	DIF_MIN=`echo $DIFFTIME | cut -f2 -d:`
	DIF_SEC=`echo $DIFFTIME | cut -f3 -d:`
	[ "$DIF_HR" = "01" ]
	check_RC_severe_dcetmudt "$?" "hour time difference is not 01"
	if [ "$?" -eq "1" ]; then
		return 1
	fi

	# if DIFFTIME is ~41 min, then pass 
	if [ "$DIF_MIN" -lt 39 ] || [ "$DIF_MIN" -gt 42 ]; then
		check_RC_severe_dcetmudt "1" "minute time difference is not ~41"	
		if [ "$?" -eq "1" ]; then
			return 1
		fi
	else
		print "hour and minute time differences are correct"
		tet_infoline "hour and minute time differences are correct"
		return 0
	fi
}

#=================================================================
# Function: cleanup_dcetmudt
# Purpose:  if epoch has been changed, reset it to original value.
#=================================================================
cleanup_dcetmudt() {
#set -x
	# Do not reset the time on this machine, 
	# Assume that the time service will bring this machine back to a
	# correct time.
	# Reset the epoch number
	if [ "$EPOCH_CHANGED" = "1" ]; then
		print "Checking state of DTS"
		tet_infoline "Checking state of DTS"
		STATE=`dtscp show state | awk '{print $NF}'`
		print "State is $STATE"
		tet_infoline "State is $STATE"

		print "Restoring epoch number to $OLD_EPOCH"
		tet_infoline "Restoring epoch number to $OLD_EPOCH"
		dtscp change epoch $OLD_EPOCH
		return $?
	fi
	return 0
}
