#!/usr/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: sec_cpi0.sh,v $
# Revision 1.1.2.2  1996/03/09  20:47:42  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:31  marty]
#
# Revision 1.1.2.1  1995/12/11  19:54:21  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:34  root]
# 
# $EndLog$
#
#	This functional test tests the configurable
#	checkpoint via dcecp.
#	It uses svc notice output to verify if a
#	rgy database checkpoint is done


# start secd and redirect stdout to a file ...
	integer total_checkpt=0
	integer test_fail=0
# LOGFILE=${TET_ROOT}/test/results/sec_checkpt_log.$(date +%h%d_%H:%M)
	checkpt_notice="secd.trace"


checkpt_notify_fail()
{
    test_fail=test_fail+1
    echo "FAILED, test : dcecp> $cmd (at `date`)"
}


checkpt_notice_verify()
{


    total=`grep "Successfully checkpointed registry database" $checkpt_notice | wc -l`
    if (( total != total_checkpt ))
    then
	checkpt_notify_fail
    else
  	echo "PASSED, test : $cmd (at `date`)"
    fi
}


total_checkpt=`grep "Successfully checkpointed registry database" $checkpt_notice | wc -l`

my_rep=`dcecp -c registry catalog |head -1`
cmd_checkpt_now="registry checkpoint $my_rep -now"
cmd=$cmd_checkpt_now
dcecp <<EOF
	$cmd
EOF
if (( $? == 0 ))
then
	total_checkpt=total_checkpt+1
	sleep 5
	checkpt_notice_verify
else
	checkpt_notify_fail
fi

cmd="registry checkpoint $my_rep -cpi 10"
dcecp <<EOF
    $cmd
    $cmd_checkpt_now
EOF
if (( $? == 0 ))
then
    sleep 5
    total_checkpt=`grep "Successfully checkpointed registry database" $checkpt_notice | wc -l`
    total_checkpt=total_checkpt+1 
    sleep 12
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

cmd="registry checkpoint $my_rep -cpi 1m"
dcecp <<EOF
    $cmd
    $cmd_checkpt_now
EOF
if (( $? == 0 ))
then
    sleep 5
    total_checkpt=`grep "Successfully checkpointed registry database" $checkpt_notice | wc -l`
    total_checkpt=total_checkpt+1
    sleep 62
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

cmd="registry checkpoint $my_rep -at *:*"
dcecp <<EOF
    $cmd
    $cmd_checkpt_now
EOF
if (( $? == 0 ))
then
    sleep 5
    total_checkpt=`grep "Successfully checkpointed registry database" $checkpt_notice | wc -l`
    total_checkpt=total_checkpt+1
    sleep 62
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

cmd="registry checkpoint $my_rep -cpi 2h"
dcecp <<EOF
    $cmd
EOF
if (( $? == 0 ))
then
    sleep 5
    total_checkpt=`grep "Successfully checkpointed registry database" $checkpt_notice | wc -l`
    checkpt_notice_verify
else
    checkpt_notify_fail
fi

time_str=`date | cut -c 12-19`
integer sec=`echo $time_str | cut -c 7-8`
integer min=`echo $time_str | cut -c 4-5`
integer hour=`echo $time_str | cut -c 1-2`
if (( sec > 55 ))
then 
    min=$min+2
else
    min=$min+1
fi
integer sleep=120
if (( min > 59 ))
then
    min=min-60
    hour=hour+1
    sleep=180
fi
t="$hour:$min"
cmd="registry checkpoint $my_rep -at $t"
dcecp <<EOF
    $cmd
EOF
if (( $? == 0 ))
then
    total_checkpt=`grep "Successfully checkpointed registry database" $checkpt_notice | wc -l`
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
	/bin/rm -f $checkpt_notice
fi

