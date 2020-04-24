#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: run.sh,v $
# Revision 1.1.2.1  1996/09/09  16:30:20  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  16:21:37  bfc]
#
# $EndLog$
#
# This differs from the execution script for the other subtests for idlcxx
# as two instances of serverx get kicked off from server, then server
# immediately dies.  The two serverx process should then be brought down
# by the client.  Also, the client needs more time than in other tests.
#
# DECLARATION SECTION
#
server_pid_file=server_pid.out
client_pid_file=client_pid.out
cur_dir=`pwd`
hostname=`hostname -s`
testname=`echo $cur_dir | sed 's/^.*\///'`
echo $testname
RPC_DEFAULT_ENTRY="/.:/idltest/$testname"_"$hostname"
export RPC_DEFAULT_ENTRY
# ps should show output long enough for the entire path name plus
# executable names.  On OSF, 'ax' will do.  We use the entire path
# of the command to keep from killing any old 'server' and 'client
# Perhaps the use of ! could simplify some of this.
psopts=ax
#
# CLEANUP SECTION
#
# Get rid of any previous object..
echo "Deleting any previous CDS object with the same name.."
cdscp del object $RPC_DEFAULT_ENTRY
#
# EXECUTION SECTION
#
# Kick off the server
#
echo Running server..
$cur_dir/server &
#
# server_pid=$!		# optimization oppurtunity
# echo "$server_pid"	# optimization oppurtunity
#
# Wait a bit, then kick off the client
#
sleep 20
echo Running client
$cur_dir/client &
#
# Wait a bit longer, make sure client has completed, then kill off the server..
#
sleep 60
echo Checking to make sure client completed..
ps $psopts | grep "$cur_dir/client" | grep -v grep > $client_pid_file
if [ -s $client_pid_file ]
then
  echo Warning: client did not complete, killing..
  kill `ps $psopts | grep "$cur_dir/client" | grep -v grep | awk '{print $1}'`
  sleep 2
  ps $psopts | grep "$cur_dir/client" | grep -v grep > $client_pid_file
  if [ -s $client_pid_file ]
  then
    echo Client did not die gracefully, trying again more forcefully..
    kill -9 `ps $psopts | grep "$cur_dir/client" | grep -v grep | awk '{print $1}'`
    ps $psopts | grep "$cur_dir/client" | grep -v grep > $client_pid_file
    if [ -s $client_pid_file ]
    then
      echo Client would not die, giving up.
    fi
  fi
fi
#
# ..now that client is gone, take care of the server
#
ps $psopts | grep serverx | grep -v grep > $server_pid_file
if [ ! -s $server_pid_file ]
then
  echo No server to kill, situation normal.
else
  echo Killing serverx..
  kill `ps $psopts | grep serverx | grep -v grep | awk '{print $1}'`
  sleep 2
  # Make sure there's not another copy to kill
  kill `ps $psopts | grep serverx | grep -v grep | awk '{print $1}'`
  echo Rechecking for serverx..
  ps $psopts | grep serverx | grep -v grep > $server_pid_file
  if [ -s $server_pid_file ]
  then
    echo Server did not die gracefully, trying again more forcefully..
    kill -9 `ps $psopts | grep serverx | grep -v grep | awk '{print $1}'`
    sleep 2
    # Make sure there's not another copy to kill
    kill -9 `ps $psopts | grep serverx | grep -v grep | awk '{print $1}'`
    sleep 1
    ps $psopts | grep serverx | grep -v grep > $server_pid_file
    if [ -s $server_pid_file ]
    then
      echo Server would not die, giving up
    fi
  fi
fi
#
# CLEANUP SECTION
#
rm $server_pid_file $client_pid_file
cdscp del object $RPC_DEFAULT_ENTRY
