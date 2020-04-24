#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: sec_cpi_unit.sh,v $
# Revision 1.1.4.1  1996/10/03  18:49:52  arvind
# 	Submit fix for CHFts19838
# 	[1996/08/19  21:36 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
#
# 	Change /usr/bin/ksh to /bin/ksh.
# 	[1996/03/09  20:47:43  marty  1.1.2.2]
#
# Revision 1.1.2.2  1996/03/09  20:47:43  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:32  marty]
# 
# Revision 1.1.2.1  1995/12/11  19:54:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:35  root]
# 
# $EndLog$
#
#	This functional unit test tests the configurable
#	checkpoint via dcecp.
#	It uses svc notice output to verify if a
#	rgy database checkpoint is done


# start secd and redirect stdout to a file ...
	integer total_checkpt=0
	integer test_fail=0
	LOGFILE=$1


checkpt_notify_fail()
{
    test_fail=test_fail+1
    echo "FAILED, test : dcecp> $cmd (at `date`)"
}


checkpt_notice_verify()
{


    total=`grep "Successfully checkpointed registry database" $LOGFILE | wc -l`
    if (( total != total_checkpt ))
    then
	checkpt_notify_fail
    else
  	echo "PASSED, test : $cmd (at `date`)"
    fi
}


set_total_checkpt()
{
    total_checkpt=`grep "Successfully checkpointed registry database" $LOGFILE| wc -l`
}

my_rep=`dcecp -c registry catalog |head -1`

# test checkpoint -now
cmd_checkpt_now="registry checkpoint $my_rep -now"
cmd=$cmd_checkpt_now
dcecp <<EOF
	$cmd
EOF
if (( $? == 0 ))
then
	total_checkpt=total_checkpt+1
	sleep 10
	checkpt_notice_verify
else
	checkpt_notify_fail
fi

# test checkpoint with an interval of seconds
cmd="registry checkpoint $my_rep -cpi 10"
dcecp <<EOF
    $cmd
    $cmd_checkpt_now
EOF
if (( $? == 0 ))
then
    sleep 5
    set_total_checkpt
    total_checkpt=total_checkpt+1 
    sleep 15
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

# test checkpoint with interval of minutes
cmd="registry checkpoint $my_rep -cpi 1m"
dcecp <<EOF
    $cmd
    $cmd_checkpt_now
EOF
if (( $? == 0 ))
then
    sleep 10
    set_total_checkpt
    total_checkpt=total_checkpt+1
    sleep 62
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

# test checkpoint with interval of wild cards
cmd="registry checkpoint $my_rep -at *:*"
dcecp <<EOF
    $cmd
    $cmd_checkpt_now
EOF
if (( $? == 0 ))
then
    sleep 10
    set_total_checkpt
    total_checkpt=total_checkpt+1
    sleep 62
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

# test checkpoint with hours
cmd="registry checkpoint $my_rep -cpi 2h"
dcecp <<EOF
    $cmd
EOF
if (( $? == 0 ))
then
    sleep 10
    set_total_checkpt
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

# test checkpoint at a specific time : current time + 1 minute
time_str=`date | cut -c 12-19`
integer sec=`echo $time_str | cut -c 7-8`
integer min=`echo $time_str | cut -c 4-5`
integer hour=`echo $time_str | cut -c 1-2`
# close to end of a minute, do it save, add 2
if (( sec > 50 ))
then 
    min=$min+2
    integer sleep=185	# wait three minutes
else
    min=$min+1
    integer sleep=125	# wait two minutes
fi
# at hour boundary ...
if (( min > 59 ))
then
    min=min-60
    hour=hour+1
fi
# at day boundary
if (( hour > 23 ))
then
    hour=0
fi

t="$hour:$min"
cmd="registry checkpoint $my_rep -at $t"
dcecp <<EOF
    $cmd
EOF
if (( $? == 0 ))
then
    set_total_checkpt
    total_checkpt=total_checkpt+1
    sleep $sleep
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

if (( test_fail != 0 ))
then
	echo "FAILED : configurable checkpoint test ($test_fail FAILED)"
else
	echo "PASSED : ALL configurable checkpoint tests PASSED"
fi

