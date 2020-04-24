#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: dtm_rt.sh,v $
# Revision 1.1.4.2  1996/03/11  02:28:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:30  marty]
#
# Revision 1.1.4.1  1995/12/11  20:14:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:47:49  root]
# 
# Revision 1.1.2.2  1993/01/13  19:56:21  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:24:07  bbelch]
# 
# Revision 1.1  1992/01/19  03:41:47  devrcs
# 	Initial revision
# 
# $EndLog$
#
#   Params: 
#       $1 -- Path name for server invoking 
#	$2 -- Which client (i.e. client name)
#       $3 -- Node for running server 
#	$4 -- Node for running client
#	$5 -- Path name for client invoking
#	$6 -- Communication family 
#	$7 -- Path name of dcerpc.cat from client's node
#	$8 -- Path name of dcerpc.cat from server's node
#       $9... -- Client arguments
#
#
#========================================================
#Arugment checking...
if test $# -lt 8  
then
   echo "usage: dtm_rt server_path which_client server_target client_target client_path comm_family args..."
   exit 1
fi
# Make copies of needed parameters
   SERVER_PATH=$1
   WHICH_CLIENT=$2
   SERVER_TARGET=$3
   CLIENT_TARGET=$4
   CLIENT_PATH=$5
   COMM_FAMILY=$6
   CLIENT_DCECAT=$7
   SERVER_DCECAT=$8
#
NLSPATH=$CLIENT_DCECAT/%N
export NLSPATH

if test $SERVER_TARGET = $CLIENT_TARGET
then 
   LOCAL=1
else
   LOCAL=0
fi

shift;shift;shift;shift;shift;shift;shift;shift
#rm -f server_output binding.dat client_timeout_handler

#echo "$COMM_FAMILY" > /tmp/perf_comm_family
#SERVER_COMM=`awk -F: '{print $1}' /tmp/perf_comm_family`
#Construct communication address of server
cd $CLIENT_PATH
#nslookup $SERVER_TARGET > nodeadd &
#IP_ADD=`awk 'NR == 5 {print $2}' nodeadd`
#echo "$IP_ADD[2001]" > add.dat
#
if test $LOCAL -eq 1
then
    echo "Starting local server"
    # Start local server & Set up shell to kill client if a timeout occurs
    $SERVER_PATH/server 5 $COMM_FAMILY &
    export SERVER_ID; SERVER_ID=$!
    (sleep 40;sleep 160;sleep100;
echo '***TIMEOUT***';kill -9 $SERVER_ID;kill -9 $$)&
    TIMEOUT_ID=$!
else
echo "Starting remote server on $SERVER_TARGET"
# Start server and Set up shell to kill it if a timeout occurs
rsh $SERVER_TARGET $SERVER_PATH/remote_server.sh START $SERVER_PATH $SERVER_DCECAT $COMM_FAMILY &
sleep 10
rsh $SERVER_TARGET "ps -ax | grep server"
#    (sleep 40;sleep 160;sleep100;echo '***TIMEOUT***';rsh $SERVER_TARGET 
#$SERVER_PATH/remote_server.sh STOP $SERVER_PATH;kill -9 $$)&
#    TIMEOUT_ID=$!
fi

# Wait for binding.dat file to be written (constructed)
#    sleep 2
#    while [ ! -s binding.dat ]; do sleep 2; done

# Create a command using the binding.dat created by the server main routine.
# Generate a command string and execute it in a processes.
if test $WHICH_CLIENT = '3' -o $WHICH_CLIENT = '5'
then
    echo "$CLIENT_PATH/client $WHICH_CLIENT $COMM_FAMILY $*"
    echo '****TEST_START****'
    eval "$CLIENT_PATH/client $WHICH_CLIENT $COMM_FAMILY $*" 
else
    echo "$CLIENT_PATH/client $WHICH_CLIENT $COMM_FAMILY:$SERVER_TARGET $*"
    echo '****TEST_START****'
    eval "$CLIENT_PATH/client $WHICH_CLIENT $COMM_FAMILY:$SERVER_TARGET $*" 
fi

    # If test is for context handles, allow time for server rundown.
    # list all context handle tests here
    #if [ "$DTM_TESTNAME" = "context2" ]; then sleep 240; fi	

if test -s $SERVER_PATH/server_output
then 
   cat $SERVER_PATH/server_output
fi
#
if test $WHICH_CLIENT = '8'
then
rsh $SERVER_TARGET -l root /usr/users/rpcosf/kill_rpcd.sh
fi
echo '****TEST_END****'

if test $SERVER_TARGET = $CLIENT_TARGET
then 
   LOCAL=1
else
   LOCAL=0
fi

if test $LOCAL -eq 1
then    
    echo "Aborting local server $SERVER_ID"
    kill -9 $TIMEOUT_ID > /dev/null 2>&1
    kill -9 $SERVER_ID > /dev/null
else
    echo "Aborting remote server"
    rsh $SERVER_TARGET $SERVER_PATH/remote_server.sh STOP $SERVER_PATH $SERVER_DCECAT $COMM_FAMILY &
fi
