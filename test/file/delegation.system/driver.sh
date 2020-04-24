#!/bin/sh

# run this script as root
# rsh as root to all of M1, M2 and M3

echo ""
echo "DELEGATION TESTS"
date
echo ""

. ./cellconfig.sh

echo "Source dir: $SRC_DIR"
echo "Top dir: $TOP_DIR"
echo "LFS dir: $LFS_DIR"
echo "Using $M1, $M2 and $M3"
echo ""

pid=$$

# The args should be -d::t::-c::n, but the DFS stuff does not work
# without a compatabilty mode other than none.
 
DELEG_ARGS=-d::t::-c::c

# track failed tests

passed=0
passed_dir_test=0
passed_file_test=0
passed_chain_test=0

cleanup()
{
	# Send the shutdown signal to the processes
	echo ""
	echo "Wait for remote process shutdown. Shouldn't take long."
	sh $SRC_DIR/stopem.sh
}

# trap handler

SIGINT=2
SIGQUIT=3
SIGTERM=15

trap_handler()
{
	echo "abnormal exit"
	cleanup
	date
	exit 1
}

if test $SETUP -eq 1
then
	echo "setting up for delegation tests"

	sh $TOP_DIR/run_simple_cmd.sh CMD=/bin/dcecp \
		ARG1=$SRC_DIR/setup.dcp SRC=$SRC_DIR
	if test $? -ne 0
	then
		echo "Fatal: setup failed"
		exit 1
	fi

	echo "setting up DFS for delegation tests"
	echo "assuming /:/delegated exists in LFS"
	
	dcecp -c acl modify $LFS_DIR/delegated \
		 -add {group testers rwxcid}
	dcecp -c acl modify $LFS_DIR/delegated -ic \
		 -add {group testers rwxcid}
	dcecp -c acl modify $LFS_DIR/delegated -io \
		 -add {group testers rwxcid}
	mkdir $LFS_DIR/delegated/dir0
	chmod 777 $LFS_DIR/delegated/dir0
	dcecp -c acl modify $LFS_DIR/delegated/dir0 -io \
		 -change {other_obj rwx}
	touch $LFS_DIR/delegated/file0
	chmod 777 $LFS_DIR/delegated/file0
	cp $SRC_DIR/dfs_exe_file $LFS_DIR/delegated/.to_copy
	chmod 777 $LFS_DIR/delegated/.to_copy
else
	echo ""
	echo "Skipping setup code"
fi

echo ""
echo "Starting eight servers."

rm -f /:/delegated.fail.log

rsh $M1 sh $SRC_DIR/run_server.sh SRC=$SRC_DIR \
	PRINC=delegation0 NS_ENTRY=/.:/delegation/servers/server0 \
	DCE_USER=delegation0 DCE_PASSWD=delegation0 \
	> $SRC_DIR/server.$M1.delegation0.$pid.log 2>&1 &

rsh $M2 $SRC_DIR/run_server.sh SRC=$SRC_DIR \
	PRINC=delegation1 NS_ENTRY=/.:/delegation/servers/server1 \
	DCE_USER=delegation1 DCE_PASSWD=delegation1 \
	> $SRC_DIR/server.$M2.delegation1.$pid.log 2>&1 &

rsh $M3 $SRC_DIR/run_server.sh SRC=$SRC_DIR \
	PRINC=delegation2 NS_ENTRY=/.:/delegation/servers/server2 \
	DCE_USER=delegation2 DCE_PASSWD=delegation2 \
	> $SRC_DIR/server.$M3.delegation2.$pid.log 2>&1 &

rsh $M1 $SRC_DIR/run_server.sh SRC=$SRC_DIR \
	PRINC=delegation3 NS_ENTRY=/.:/delegation/servers/server3 \
	DCE_USER=delegation3 DCE_PASSWD=delegation3 \
	> $SRC_DIR/server.$M1.delegation3.$pid.log 2>&1 &

rsh $M2 $SRC_DIR/run_server.sh SRC=$SRC_DIR \
	PRINC=delegation4 NS_ENTRY=/.:/delegation/servers/server4 \
	DCE_USER=delegation4 DCE_PASSWD=delegation4 \
	> $SRC_DIR/server.$M2.delegation4.$pid.log 2>&1 &

rsh $M3 $SRC_DIR/run_server.sh SRC=$SRC_DIR \
	PRINC=delegation5 NS_ENTRY=/.:/delegation/servers/server5 \
	DCE_USER=delegation5 DCE_PASSWD=delegation5 \
	> $SRC_DIR/server.$M3.delegation5.$pid.log 2>&1 &

rsh $M1 $SRC_DIR/run_server.sh SRC=$SRC_DIR \
	PRINC=delegation6 NS_ENTRY=/.:/delegation/servers/server6 \
	DCE_USER=delegation6 DCE_PASSWD=delegation6 \
	> $SRC_DIR/server.$M1.delegation6.$pid.log 2>&1 &

rsh $M2 $SRC_DIR/run_server.sh SRC=$SRC_DIR \
	PRINC=delegation7 NS_ENTRY=/.:/delegation/servers/server7 \
	DCE_USER=delegation7 DCE_PASSWD=delegation7 \
	> $SRC_DIR/server.$M2.delegation7.$pid.log 2>&1 &

# if a server bails out, we should know soon

sleep 10
if test -f /:/delegated.fail.log
then
	echo "servers failed to start"
	rm -f /:/delegated.fail.log
	cleanup
	exit 1
fi

echo ""
echo "Wait for servers to start up. Shouldn't take long."

sh $SRC_DIR/wait_for_server.sh SRC=$SRC_DIR SLEEPTIME=5 \
	SERVER=/.:/delegation/servers/server0
echo "first server OK"
sh $SRC_DIR/wait_for_server.sh SRC=$SRC_DIR SLEEPTIME=5 \
	SERVER=/.:/delegation/servers/server1
echo "second server OK"
sh $SRC_DIR/wait_for_server.sh SRC=$SRC_DIR SLEEPTIME=5 \
	SERVER=/.:/delegation/servers/server2
echo "third server OK"
sh $SRC_DIR/wait_for_server.sh SRC=$SRC_DIR SLEEPTIME=5 \
	SERVER=/.:/delegation/servers/server3
echo "fourth server OK"
sh $SRC_DIR/wait_for_server.sh SRC=$SRC_DIR SLEEPTIME=5 \
	SERVER=/.:/delegation/servers/server4
echo "fifth server OK"
sh $SRC_DIR/wait_for_server.sh SRC=$SRC_DIR SLEEPTIME=5 \
	SERVER=/.:/delegation/servers/server5
echo "sixth server OK"
sh $SRC_DIR/wait_for_server.sh SRC=$SRC_DIR SLEEPTIME=5 \
	SERVER=/.:/delegation/servers/server6
echo "seventh server OK"
sh $SRC_DIR/wait_for_server.sh SRC=$SRC_DIR SLEEPTIME=5 \
	SERVER=/.:/delegation/servers/server7
echo "eigth server OK"

echo ""
echo "All servers are now running.  Tests starting."

rm -f /tmp/delegate.fail.log
rm -f $LFS_DIR/delegated/dir0/*

echo ""
echo "Run the DFS file tests."
date
if test $RUN_FILE_TEST -eq 1
then
	sh $TOP_DIR/runas.sh SUBCMD=/bin/dcecp \
		DCEUSER=dtest0 DCEPASSWD=dtest0 VERBOSE=y SRC=$SRC_DIR \
		DARGS=$DELEG_ARGS $SRC_DIR/do_dfs_file.dcp
	if test -f /tmp/delegate.fail.log
	then
	        echo "delegation: DFS file tests failed"
		passed=1
		passed_file_test=1
		rm -f /tmp/delegate.fail.log
	fi
else
	echo "Skipping file tests."
fi

echo ""
echo "Run the DFS directory tests."
date
if test $RUN_DIR_TEST -eq 1
then
	echo "$TOP_DIR/runas.sh SUBCMD=/bin/dcecp"
	echo "DCEUSER=dtest0 DCEPASSWD=dtest0 VERBOSE=y SRC=$SRC_DIR"
	echo "DARGS=$DELEG_ARGS $SRC_DIR/do_dfs_dir.dcp"
	sh $TOP_DIR/runas.sh SUBCMD=/bin/dcecp \
	       DCEUSER=dtest0 DCEPASSWD=dtest0 VERBOSE=y SRC=$SRC_DIR \
	       DARGS=$DELEG_ARGS $SRC_DIR/do_dfs_dir.dcp
	if test -f /tmp/delegate.fail.log
	then
	       echo "delegation: DFS directory tests failed"
	       passed=1
	       passed_dir_test=1
	       rm -f /tmp/delegate.fail.log
	fi
else
	echo "Skipping dir tests."
fi

echo ""
echo "Run the delegation chain tests."
date
if test $RUN_CHAIN_TEST -eq 1
then
	sh $TOP_DIR/runas.sh SUBCMD=/bin/dcecp \
		DCEUSER=dtest0 DCEPASSWD=dtest0 VERBOSE=y SRC=$SRC_DIR \
		DARGS=$DELEG_ARGS $SRC_DIR/do_deleg_chain.dcp
	if test -f /tmp/delegate.fail.log
	then
	        echo "delegation: chain tests failed"
		passed=1
		passed_chain_test=1
		rm -f /tmp/delegate.fail.log
	fi
else
	echo "Skipping chain tests."
fi

cleanup

# verify servers stopped

# report results

echo ""
if test $passed -eq 0
then
	echo "************************";
	echo " Delegation test PASSED ";
	echo "************************";
else
	echo "************************";
	echo " Delegation test FAILED ";
	echo "************************";
	echo ""
	echo "Sumary"
	if test $RUN_DIR_TEST -eq 1
	then
		echo "dir test returned $passed_dir_test"
	else
		echo "skipped dir test"
	fi
	if test $RUN_FILE_TEST -eq 1
	then
		echo "file test returned $passed_file_test"
	else
		echo "skipped file test"
	fi
	if test $RUN_CHAIN_TEST -eq 1
	then
		echo "chain test returned $passed_chain_test"
	else
		echo "skipped chain test"
	fi
fi

echo ""
echo "delegation tests exiting normally"
date
exit $passed
