#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: sec_cpi.sh,v $
# Revision 1.1.2.2  1996/03/09  20:47:41  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:31  marty]
#
# Revision 1.1.2.1  1995/12/11  19:54:19  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:33  root]
# 
# $EndLog$
#
#	This test tests the configurable checkpoint via dcecp.
#	This is a driver test.  It manages secd start/stop and
#	dce_login stuff for its unit tests.
#	It uses svc notice output to verify if a
#	rgy database checkpoint is done

#	set up secd so SVC NOTICE message is printed
export SVC_NOTICE=STDOUT:--

if (( $# != 2 ))
then
	echo "Usage: $0 DCE_LOGIN_NAME PASSWD"
	exit
fi

# start secd and redirect stdout to a file ...
logdir=${logdir:-/tet/test/results}
LOGFILE=${logdir}/sec_checkpt_log.$(date +%h%d_%H:%M)

stop_secd()
{
sec_admin << EOF > /dev/null
    stop
EOF
}


stop_secd
sleep 5
secd -d -v > $LOGFILE &
sleep 20

dce_login cell_admin -dce- -e sec_cpi_unit.sh $LOGFILE

stop_secd

unset SVC_NOTICE

secd

