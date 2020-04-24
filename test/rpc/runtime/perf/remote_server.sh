#!/bin/sh -
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: remote_server.sh,v $
# Revision 1.1.4.2  1996/03/11  02:29:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:49  marty]
#
# Revision 1.1.4.1  1995/12/11  20:15:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:48:10  root]
# 
# Revision 1.1.2.2  1993/01/13  19:57:29  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:25:25  bbelch]
# 
# Revision 1.1  1992/01/19  03:41:49  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#	$1 - START/STOP - start or stop the server
#	$2 - path name for the server
#	$3 - path name for the dcerpc.cat
#	$4 - communication family
#
#========================================================
#Arugment checking...
if test $# -ne 4
then
	echo "usage: remote_server START/STOP server_path dce_cat_path comm_family"
	exit 1
fi

#set NLSPATH
   NLSPATH=$3/%N
   export NLSPATH
#
   SERVER_PATH=$2
   COMM_FAMILY=$4

#if [ ! -r $SERVER_PATH/server ]
#then
#	echo "remote_server: ERROR - No server to start for perf testing"
#fi

#========================================================
#START...
if test $1 = 'START'
then
    # binding.dat output of server into common directory
    $SERVER_PATH/server 5 $COMM_FAMILY &
    SERVER_ID=$!    

# Set up shell script to kill server in $SERVER_PATH directory
    echo "#!/bin/sh -" > /tmp/stop_dtmperf_server
    chmod a+x /tmp/stop_dtmperf_server
    echo "echo '***Cleanup***: Aborting server'" >> /tmp/stop_dtmperf_server
    echo "kill -9 $SERVER_ID" >> /tmp/stop_dtmperf_server
fi
#========================================================
#STOP...
if test $1 = 'STOP'
then
     /tmp/stop_dtmperf_server
fi
exit 1
