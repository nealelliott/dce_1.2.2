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
# $Log: run_tests.sh,v $
# Revision 1.1.8.2  1996/03/11  02:18:15  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:55:24  marty]
#
# Revision 1.1.8.1  1995/12/11  19:29:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:15:40  root]
# 
# Revision 1.1.6.3  1994/05/10  18:03:36  tom
# 	Merged with changes from 1.1.6.2
# 	[1994/05/10  18:03:14  tom]
# 
# 	Add es_attr test for ecoding services get/set attr API.
# 	[1994/05/10  17:35:29  tom]
# 
# Revision 1.1.6.2  1994/05/03  18:50:55  tom
# 	Add strint test.
# 	[1994/05/03  18:46:55  tom]
# 
# Revision 1.1.6.1  1994/05/02  22:43:48  tom
# 	Add bindcall test.
# 	[1994/05/02  21:06:42  tom]
# 
# Revision 1.1.4.3  1993/08/11  16:28:58  ganni
# 	tests for new IDL functionality
# 	[1993/08/11  16:16:30  ganni]
# 
# Revision 1.1.4.2  1993/06/15  21:07:31  sommerfeld
# 	Don't assume that '.' is at the head of path; invoke "client" and "server"
# 	by relative pathname rather than just by entry name..
# 	[1993/06/15  19:45:27  sommerfeld]
# 
# Revision 1.1.2.2  1993/01/07  23:35:37  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  13:43:05  bbelch]
# 
# Revision 1.1  1992/01/19  03:34:25  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# First argument is number of times to run tests.
# Remaining arguments are testcases to run.  If no additional arguments,
# then run all testcases that exist under the current directory.

# Environment variables can be used to control testing.
#	RESULTS_FILE:	Full path name where results (test outputs) are saved.
#			(Default is /tmp/results.)
#	FAILURES_FILE:	Full path name where list of testcase failures is kept.
#			(Default is /tmp/failures.)
#	RUN_ARGS_DIR:	Full path name to alternate directory containing
#			testcase subdirectories, which may contain RUN_ARGS
#			files. (Default is to look in current directory.)
#
#	PROTOCOL:	Protocol to use (Default is "ncadg_ip_udp".)
#
#	The RUN_ARGS file can override the following default definitions:
#	SERVER_ARGS="-f $BINDING_FILE $PROTOCOL"
#				Note: $BINDING_FILE is defined by this shell
#				script and should not be modified.
#	SERVER_DELAY=30		# Max time (in seconds) to wait for
#				# server to start and write binding file
#	CLIENT_ARGS="\`cat $BINDING_FILE\` $1"
#		#Arguments to client.  Note: This string is evaluated (with
#		#the "eval" command), so appropriate escapes must be used.
#	TESTCASE_DELAY=100		# Max time (in seconds) to wait for
#					# testcase to complete
#	unset NO_BINDING_FILE	# Set this variable to any non-null string if
#				# a binding file is not written.  In this case,
#				# the CLIENT_ARGS variable should be
#				# redefined to provide the proper arguments.
#
#	Return codes:
#		0: All testcases (that were run) succeeded.
#		1: One or more testcases failed.
#		2: repeat_count argument was omitted
#		3: Script was aborted (with ^C).

if [ $# -lt 1 ]
then	echo "Syntax:  $0 <repeat_count> [ testcase directories ... ]"
	exit 2
fi

if [ "$1" -eq '' -o `echo $1 | sed -e 's/^[0-9]*$/0/'` -ne 0 ]
then	echo "Repeat count must be numeric"
	exit 2
fi

REPEAT_COUNT=$1
if [ $REPEAT_COUNT -gt 100 ]
then	echo Repeat count greater than 100--1 will be used.
	REPEAT_COUNT=1
fi

shift

DIRLIST='
	alias		ao_arrays	array		array2
	array_v1_string	array_v2_string	arrayp		arrayptr
	arraystr	arraystr2	arrayswith	arrayv1
	bindcall
	bigtest1	binop_alloc	binopwk		brown
	bug_3		comprefs	comprefs_ool	conftree
	context		context2	cover1		cover2
	cover_array	cover_array_ool	cover_contain_ref cover_ool
	cover_struct	cover_top_pointer cover_union	epv
	es_array	es_arrayb	es_arrayc	es_attr
	factorial	faultstat	fpe		gregbug1
	h_ta		ichar1		ichar2		icharexp		
	icharpkl	ignore		mtsaconf	mtsvary
	multiref	n_e_union	nidltest
	no_op		nodethreads	ool		oolsmall
	oolthings	op_results	orecord		out_refs
	outrefs		packed_byte_array parray	pipe_of_array
	pipefault	pipes		pipetest	pipetest2
	pipetest2_cancel pipetest2_ool	pointarray	pointed
	primes		ptrarray	refdels		refptrs
	repas1		repas2		represent	represent_ool	
	sp_union	sparse		string0		string1
	string_tree	stringptr	stringts	stringy
	strint
	struct_ct_union threads		tree		ue1
	ue2		unique1		unique2
	xmit		xmitas		xmitas_conf	xmitas_nested	
	xmitas_repas	xmitas_repas_ool'

DIRS=${*-"$DIRLIST"}	# Get list of directories to test, if entered on
			# command line.

SERVER_OUTPUT=/tmp/srv$$
CLIENT_OUTPUT=/tmp/cli$$
TIMEOUT_OUTPUT=/tmp/too$$
TIMEOUT_HANDLER=/tmp/toh$$
BINDING_FILE=/tmp/bnd$$
TEMP_FILES="$SERVER_OUTPUT $CLIENT_OUTPUT $TIMEOUT_OUTPUT $TIMEOUT_HANDLER \
	$BINDING_FILE"

PROTOCOL=${PROTOCOL-ncadg_ip_udp}
RESULTS_FILE=${RESULTS_FILE-/tmp/results}
FAILURES_FILE=${FAILURES_FILE-/tmp/failures}

> $FAILURES_FILE
> $RESULTS_FILE

successes=0
failures=0
not_run=0

# Define signal handlers
# For HANGUP, INT and QUIT signals, signal the timeout process; kill any client
# and server; delete the temp files; echo a message; and exit
trap '
rm -f $TEMP_FILES
echo "Successes $successes; Failures $failures; Not run $not_run"
kill -9 $SERVER_ID $CLIENT_ID  > /dev/null 2>&1
kill -2 $TIMEOUT_ID > /dev/null 2>&1
echo Testing aborted
exit 3' 1 2 3	

# Set up script to kill client if a timeout occurs
# Arguments to the script are:
#	delay(in seconds), client PID, server PID, and testcase name
cat > $TIMEOUT_HANDLER << \END
#!/bin/sh -
trap 'exit 0' 1
trap 'kill -9 $2 $3 > /dev/null 2>&1;
	exit 0' 2	# In case ^C out of run.sh, kill server too

count=`expr $1 / 2`	# Each loop sleeps 2 seconds, so divide delay by 2.
while [ $count -gt 0 ]
do	sleep 2
	count=`expr $count - 1`
done
kill -9 $2	# The client
echo "***TIMEOUT***: Aborting client and server for testcase $4"
END

chmod a+x $TIMEOUT_HANDLER

for f in $DIRS
do if [ -d $f ]
   then if [ -s $f/server -a $f/client ]
    then
	cd $f
	echo Running $f test
	echo "****TEST $f START****"		>> $RESULTS_FILE

	# Set up default arguments for client and server
	SERVER_ARGS="-f $BINDING_FILE $PROTOCOL"
	SERVER_DELAY=30
	CLIENT_ARGS="\`cat $BINDING_FILE\` $REPEAT_COUNT"
	TESTCASE_DELAY=100
	unset NO_BINDING_FILE
	
	# For some testcases, special arguments may be needed, or
	# a longer timeout should be specified, or additional
	# environment variables should be specified.  In this case,
	# the file "RUN_ARGS" should exist in the same directory
	# as the client or server, or in an alternate directory, as
	# specified by RUN_ARGS_DIR environment variable.
	if [ -s $RUN_ARGS_DIR/$f/RUN_ARGS ]
	then . $RUN_ARGS_DIR/$f/RUN_ARGS
	elif [ -s ./RUN_ARGS ]
	then . ./RUN_ARGS
	fi

	# Start server.  Ordinarily, (if not overridden by RUN_ARGS),
	# this will write binding information to $BINDING_FILE.
	./server $SERVER_ARGS > $SERVER_OUTPUT 2>&1 &
	SERVER_ID=$!

	if [ $NO_BINDING_FILE ]
	then	sleep $SERVER_DELAY	# Wait max time
	else
		# Wait for server to start and write binding information
		count=$SERVER_DELAY
		while [ $count -gt 0 ]
		do	if [ -s $BINDING_FILE ]
			then	count=0
			else	count=`expr $count - 1`
			fi
			sleep 1
		done
		if [ \! -s $BINDING_FILE ]
		then	echo "Server for testcase $f did not write binding file." \
				>> $RESULTS_FILE

			kill -9 $SERVER_ID		> /dev/null 2>&1
			echo "****server $f output****"	>> $RESULTS_FILE

			# Delete usual message from server being killed.
			sed -e '${;/unexpected CMA exception cma_e_alerted/d;}' $SERVER_OUTPUT \
						>> $RESULTS_FILE
			echo "****TEST $f END****"	>> $RESULTS_FILE
			failures=`expr $failures + 1 `
			echo $f >> $FAILURES_FILE
			cd ..
			continue
		fi
	fi

	# Evaluate the CLIENT_ARGS string, because ordinarily, we must
	# read the $BINDING_FILE file.
	eval CLIENT_ARGS='"'"$CLIENT_ARGS"'"'

	# Run the client command.
	./client $CLIENT_ARGS > $CLIENT_OUTPUT 2>&1 &
	CLIENT_ID=$!

	# Start the timeout job	
	$TIMEOUT_HANDLER $TESTCASE_DELAY $CLIENT_ID $SERVER_ID $f \
		> $TIMEOUT_OUTPUT  &
	TIMEOUT_ID=$!

	wait $CLIENT_ID		# Wait for client to terminate
	cr=$?

	# Signal the timeout process so it exits
	kill -1 $TIMEOUT_ID	> /dev/null 2>&1

	# Try to kill the server gracefully, so files will be closed.
	kill -1 $SERVER_ID	> /dev/null 2>&1	

	# Wait, then try to kill the server again, in case the first attempt
	# was caught or ignored.
	sleep 1
	kill -9 $SERVER_ID	> /dev/null 2>&1

	# Collect the output
	cat $CLIENT_OUTPUT			>> $RESULTS_FILE
	echo "****server $f output****"		>> $RESULTS_FILE

	# Delete usual message from server being killed.
	sed -e '${;/unexpected CMA exception cma_e_alerted/d;}' $SERVER_OUTPUT \
						>> $RESULTS_FILE

	# If timeout occurred, $TIMEOUT_OUTPUT file should be non-empty.
	if [ -s $TIMEOUT_OUTPUT ]
	then
		echo "****TEST $f timeout"	>> $RESULTS_FILE
		cat $TIMEOUT_OUTPUT		>> $RESULTS_FILE
	fi
	echo "****TEST $f END****"		>> $RESULTS_FILE
	echo "Client return code for $f: $cr"	>> $RESULTS_FILE

	# Check for results line, to keep track of successes and failures.
	# (This could also be done by checking the return code, as long as all
	# testcase generate proper return codes.)
	if grep -s Summary $CLIENT_OUTPUT &&
	  [ `sed -n -e '/Summary:/{s/^.*, //;s/ failures//;p;}' $CLIENT_OUTPUT` -eq 0 ]
	then	successes=`expr $successes + 1`
	else	failures=`expr $failures + 1`
		echo $f >> $FAILURES_FILE
	fi
	rm -f $BINDING_FILE	# Delete binding file, so next test
				# won't be misled by its presence.
	cd ..
    else not_run=`expr $not_run + 1`
	echo "Not run: $f" >> $FAILURES_FILE
    fi
   fi
done

echo "Successes $successes; Failures $failures; Not run $not_run"
rm -f $TEMP_FILES

if [ $failures -gt 0 ]
then	exit 1
else 	exit 0
fi
