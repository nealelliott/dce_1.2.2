#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: cdsserv.ksh,v $
# Revision 1.1.9.2  1996/03/11  02:39:50  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:57  marty]
#
# Revision 1.1.9.1  1995/12/11  21:56:55  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  18:32 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/04/12  15:23 UTC  truitt
# 
# 	HP revision /main/truitt_junk/1  1994/04/12  15:15 UTC  truitt
# 	clean up references to profile
# 	[1995/12/11  20:56:45  root]
# 
# Revision 1.1.6.6  1993/10/20  19:27:11  qbarry
# 	Added some TET_SETBLOCKs to sort multiprocess output better.
# 	[1993/10/19  21:33:35  qbarry]
# 
# Revision 1.1.6.5  1993/10/17  22:44:48  qbarry
# 	Put more error information into the journal file.
# 	[1993/10/17  22:44:35  qbarry]
# 
# Revision 1.1.6.4  1993/10/07  18:22:23  qbarry
# 	Worked around OSF1 KSH problem & fixed re-login code.
# 	[1993/10/07  18:20:26  qbarry]
# 
# 	This version now runs correctly under RUN.CDS.  Took out DATE_TO_SEC
# 	to use KSH $SECONDS instead.  Cleaned up a bunch of misc stuff.  Fixed
# 	some loop control bugs.  Made it accept decimal fraction hours.
# 	[1993/10/06  19:37:19  qbarry]
# 
# 	Added STARTUP and CLEANUP to do real init and clean.  Specifically
# 	they create a cleanup file, CLEANUPFILE, that gets whatever K-shell
# 	commands it needs to clean up.  Modified the test to use CLEANUPFILE
# 	to actually get rid of the login credentials it was creating, so now
# 	the test can run more than once without nasty messages from KDESTROY
# 	because the test forgot its creds.  Also did a little cleanup of the
# 	test code itself.
# 	[1993/10/05  16:02:31  qbarry]
# 
# Revision 1.1.6.3  1993/09/15  13:22:17  cmckeen
# 	Removed debug statements
# 	[1993/09/15  13:22:03  cmckeen]
# 
# Revision 1.1.6.2  1993/09/15  13:13:51  cmckeen
# 	Fixed the expression used to find the clearinghouse name to allow numbers in the name
# 	[1993/09/15  13:13:32  cmckeen]
# 
# Revision 1.1.6.1  1993/09/09  20:25:59  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  20:25:40  cmckeen]
# 
# Revision 1.1.4.3  1993/07/07  18:26:25  eheller
# 	Further TET upgrade
# 	[1993/07/07  18:16:23  eheller]
# 
# Revision 1.1.1.4  1993/04/21  20:24:06  root
# 	    Loading drop DCE1_0_2B23
# 
# Revision 1.1.2.2  1993/02/06  00:04:59  eperkins
# 	Initial version of 1.0.2 cds.server.4 system test
# 	[1993/02/05  14:29:49  eperkins]
# 
# $EndLog$

iclist="ic1"
ic1="tp1"
tet_startup="startup"
tet_cleanup="cleanup"

startup ()
{
        myecho "Starting test CDSSERV"
	cleanupfile=$STTMPDIR/cdsserv_CLEANUP_`hostname | cut -f1 -d.`_`date +%y%m%d%H%M%S`
        export cleanupfile
        echo "#!/bin/ksh" > $cleanupfile
	echo "export KRB5CCNAME" >> $cleanupfile
	if (( $? != 0 ))
	then
		myecho "ERROR: Cannot write cleanup file $cleanupfile."
		tet_result FAIL
		exit 1
	fi
	chmod 777  $cleanupfile
}

myecho()
{
	echo $1
	tet_infoline $1
}

tp1()
{

# assume a PASS until a failure is generated
tet_result PASS

if [ -n "$HOURS" ]
then
	RT_DURATION=`awk "BEGIN { printf (\"%d\", 3600 * $HOURS ); exit; }"`
	myecho "The test will execute for: $RT_DURATION sec."
else
	RT_DURATION="0"
	HOURS="0"
fi

if [ -z "$LOOPNUM" ]
then
	LOOPNUM="1"
fi

if [ "$LOOPNUM" -gt 999 ]
then
	INFINITE="true"
else
	INFINITE="false"
fi 

LOSTYPE=`uname -s`
PID=$$
FAIL_STAT=0
ATTEMPTS=1
LOOPS=0
CELLS=${CELLS:-`getcellname`}
PRINCS=${PRINCS:-"cell_admin"}
PWS=${PWS:-"-dce-"}
CONFIDENCE=${CONFIDENCE:-"medium"}
#Half of the default credential expiration interval 5 hours in seconds
DEFAULT_CRED_EXP=18000
export KRB5CCNAME RT_DURATION CELLS FAIL_STAT ATTEMPTS CONFIDENCE LOOPS PID

myecho "\tExecuting in cell: $CELLS"

PRINC_NUM=0
for PRINC in `echo "$PRINCS"`
do
	PRINC_NUM=`expr $PRINC_NUM + 1`
	PRINC[$PRINC_NUM]="$PRINC"
done
PWN=0
for PW in `echo " $PWS"` 
do
	PWN=`expr $PWN + 1`
	PW[$PWN]="$PW"
done

export PRINC_NUM PWN
if [ "$PRINC_NUM" -eq "$PWN" ]
then 
   PWN=0
   for PW in `echo " $PWS"`
   do
        PWN=`expr $PWN + 1`
	PRINC[$PWN]="${PRINC[$PWN]} ${PW[$PWN]}"
        myecho "Using principal PRINC[$PWN]: ${PRINC[$PWN]}"
   done
else 
   PWN=0
   for PRINC in `echo " $PRINCS"`
   do
        PWN=`expr $PWN + 1`
     	PRINC[$PWN]="${PRINC[$PWN]} -dce-"
        myecho "Using principal PRINC[$PWN]: ${PRINC[$PWN]}"
   done
fi
myecho "Executing $LOOPNUM cdscp concurrent loops for each principal"
export PS=0 LOOPNUM lOOPS INFINITE CALLS
export MATCH_STR="Clearinghouse Name = "

if [ -x /opt/dcelocal/bin/dce_login -a ! -x /usr/bin/dce_login_noexec ]
then
	ln -s /opt/dcelocal/bin/dce_login /usr/bin/dce_login_noexec
fi
date 
R_ACC_TEMP=0
R_ACC=0
# Local  cdscp client calls
for CELL in `echo "$CELLS"`
do 
  (while [ "$PRINC_NUM" -gt 0 ] 
   do
     tet_setblock
     myecho "Accessing: $CELL using ${PRINC[$PRINC_NUM]} principal"
     KRB5CCNAME=`dce_login_noexec $CELL/${PRINC[$PRINC_NUM]}`
     if [ "$?" != "0" ]
     then
	myecho "Failure: Unable to authenticate $CELL/${PRINC[$PRINC_NUM]}"
	myecho "KRB5CCNAME=$KRB5CCNAME"
        PRINC_NUM=`expr $PRINC_NUM - 1`
	tet_result FAIL
	continue
     else
	echo "KRB5CCNAME=$KRB5CCNAME" >> $cleanupfile
	echo "kdestroy && echo \"OK: KDESTROYed \$KRB5CCNAME\" || echo \"ERROR: Did not KDESTROY \$KRB5CCNAME\"" >> $cleanupfile
	echo "Login creds: $KRB5CCNAME"
	CRED_TIME=$SECONDS
     fi

# The following WHILE loop runs in a seperate process.  We create a seperate process
# for each possible cell/principal pair to stress CDS.
     tet_setblock
     (while [ "$LOOPNUM" -ge 1 -o "$INFINITE" = "true" -o "$RT_DURATION" -gt 0 ] 
      do
	CDSCP_OUT="/tmp/cdscp_out_$PID$PRINC_NUM"
	touch $CDSCP_OUT
	NOW_SEC=$SECONDS
	if (( $NOW_SEC - $CRED_TIME > $DEFAULT_CRED_EXP ))
	then
		myecho "Refreshing login via dce_login"
		KRB5CCNAME=`dce_login_noexec $CELL/${PRINC[$PRINC_NUM]}`
		if [ "$?" != "0" ]
		then
			myecho "Failure: Unable to authenticate $CELL/${PRINC[$PRINC_NUM]}"
			myecho "KRB5CCNAME=$KRB5CCNAME"
        		PRINC_NUM=`expr $PRINC_NUM - 1`
			tet_result FAIL
			continue
		else 
			echo "KRB5CCNAME=$KRB5CCNAME" >> $cleanupfile
			echo "kdestroy && echo \"OK: KDESTROYed \$KRB5CCNAME\" || echo \"ERROR: Did not KDESTROY \$KRB5CCNAME\"" >> $cleanupfile
			echo "Login creds: $KRB5CCNAME"
			CRED_TIME=$SECONDS
		fi
	fi
	if [ "$LOOPS" -eq 0 ]
	then
	   TEMP_SEC="$NOW_SEC"
	   cdscp <<-END > $CDSCP_OUT 2>&1
	   set cdscp confidence = $CONFIDENCE 
	   show dir $CELL 
		END
  	   if [ -s "$CDSCP_OUT" ]
	   then
	     WHEN=`cat $CDSCP_OUT | egrep "AT"` 
	     CH=`cat $CDSCP_OUT | egrep "${MATCH_STR}".*[a-z,A-Z,0-9]_ch | sed 's/^          Clearinghouse Name = //'`
	     if [ -n "$CH" ]
	     then
	       myecho "${PRINC[$PRINC_NUM]} L${LOOPS}: $WHEN"
	       myecho "${PRINC[$PRINC_NUM]} $CH"  
	     else
	       myecho "${PRINC[$PRINC_NUM]} L${LOOPS}:" 
	       myecho "${PRINC[$PRINC_NUM]} L${LOOPS},$CELL $WHEN: Failed cdscp show dir!"
	       file2journal "CDSCP ERROR" $CDSCP_OUT
	       tet_result FAIL
	       continue
	     fi
	  fi
	else
		T_DELTA=`expr $NOW_SEC - $TEMP_SEC`
		if [[ "$HOURS" != "0" ]]
		then
			(( RT_DURATION -= $T_DELTA ))
			myecho "Runtime seconds left: $RT_DURATION"
		fi
		TEMP_SEC="$NOW_SEC"
	fi
	if [ "$LOOPS" -eq 1 ]
	then
		MAX=$T_DELTA
	fi
	(( LOOPS += 1 ))
	(( LOOPNUM -= 1 ))

	cdscp <<-END > $CDSCP_OUT 2>&1
	set cdscp confidence = $CONFIDENCE 
	show dir $CELL 
	show clear $CH
	END
	if [ -s "$CDSCP_OUT" ]
	then
	  WHEN=`cat $CDSCP_OUT | egrep "AT"` 
	  CH=`cat $CDSCP_OUT | egrep "${MATCH_STR}".*[a-z,A-Z,0-9]_ch | sed 's/^          Clearinghouse Name = //'`
	R_ACC=`cat $CDSCP_OUT | grep "Read Accesses =" | sed 's/^               Read Accesses = //'`
	  if [ -n "$CH" ]
	  then
	     myecho "${PRINC[$PRINC_NUM]} L${LOOPS}: $WHEN"
	     myecho "${PRINC[$PRINC_NUM]} $CH"  
	  else
	     myecho "${PRINC[$PRINC_NUM]} L${LOOPS}:" 
	     myecho "${PRINC[$PRINC_NUM]} L${LOOPS},$CELL $WHEN: Failed cdscp show dir!" 
	     file2journal "CDSCP ERROR" $CDSCP_OUT
	     tet_result FAIL
	     continue
	  fi
	fi 
	if [ -n "$R_ACC" ]
	then
	   DELTA=`expr $R_ACC - $R_ACC_TEMP`  
		if [ "$DELTA" -eq "$R_ACC" ]
		then
			R_ACC_TEMP=`expr $R_ACC_TEMP + $R_ACC`
		else 
			R_ACC_TEMP="$R_ACC"
		fi
	fi
	if [ $R_ACC_TEMP -eq 0 -o "$LOOPS" -eq 1 -a -n "$DELTA" ]
	then 
	   myecho "${PRINC[$PRINC_NUM]} L${LOOPS}: Start read access:$DELTA reads" 
	else
	   date
	   myecho "${PRINC[$PRINC_NUM]} L${LOOPS}: Time delta:$T_DELTA sec"
	   myecho "${PRINC[$PRINC_NUM]} L${LOOPS}: maximum reading:$MAX sec"
	   myecho "${PRINC[$PRINC_NUM]} L${LOOPS}: Read access delta:$DELTA reads" 
		if [ "$T_DELTA" -gt "$MAX" ]
		then
			MAX=$T_DELTA
		fi
	fi

# End of WHILE loop that is spun off to a seperate process.
      done) &

# End of principal id loop.  Note the pipe to the WHILE READ LINE loop.
      PRINC_NUM=`expr $PRINC_NUM - 1`
    done) | 
    while read line
    do
	echo $line
	if echo $line | grep "Time delta" >/dev/null 
	then
		ATTEMPTS=`expr $ATTEMPTS + 1`
		LOOP_CHK="$LOOPNUM"
	fi
	if echo $line | grep Fail
	then
		if [ "$RT_DURATION" -gt 0 -o "$LOOPNUM" -ge 1 ]
		then 
			date
			FAIL_STAT=`expr $FAIL_STAT + 1`
			if [ "$FAIL_STAT" -gt "$ATTEMPTS" -a "$LOOP_CHK" -eq "$LOOPNUM" ]
			then
				ATTEMPTS="$FAIL_STAT"
			fi
			myecho "Failed  $FAIL_STAT/$ATTEMPTS attempted loops"
			tet_result FAIL
			LOOP_CHK="$LOOPNUM"
		fi
	fi
    done
# end CELL loop
done  

if [ -f /tmp/cdscp* -a -z "$DEBUG" ]
then
	rm /tmp/cdscp*
fi 
}	#End TP1

cleanup ()
{
        myecho "Cleaning up"
	( . $cleanupfile )
	if (( $? != 0 ))
	then
		myecho "ERROR: Could not clean up after CDSSERV.  See $cleanupfile."
	else
		rm -f $cleanupfile 1>/dev/null 2>&1
	fi
}       # end cleanup

# Little routine to take a file and echo it to the TET journal with a
# header on each line so it stands out.  P1 is header, P2 is filespec.
file2journal() {
    tet_infoline "$1	BEGIN LINES FROM $2 AT `date +%x-%X`"
    cat $2 |
    while read line
    do
	tet_infoline "$1	$line"
    done
}

. $TET_ROOT/profile.dcest
. ./cdsserv.cfg
. $TET_ROOT/lib/ksh/tcm.ksh
