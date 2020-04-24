#!/bin/sh
#/* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/nbs-crash.sh,v 1.1 1994/03/16 20:15:58 ota Exp $

# nbs-crash -- runs the nbs-recovery.test script with large or small block
#     size a number of times.  If forks kill-test_anode.pl to kill the
#     test_anode process periodically.  After recovery the script
#     verifies the aggregate and checks that the file's contents are
#     legal.  With a large block size most writes are to a fragmented
#     file, with small block size most are not.  This test both code
#     paths which are substantially different.  It forks threads to run
#     nbs-meta.int.test and nbs-recovery.int.test which provide the
#     loads for this test.

if [ $# -ne 2 ]
  then
    echo Usage is "'$0 large|small #iterations'"
    exit 1
fi
if [ $1 = large ]; then var=2
elif [ $1 = small ]; then var=1
else echo "$1 is not large|small"; exit 1
fi
iterations=$2
: ${TEST_ANODE_TMPDEV:=/tmp/dev-$USER}

test_anode nbs-recovery.test -var $var

echo Pid of $0 is $$.
perl kill-test_anode.pl &		# periodically send kills

cnt=0
while [ $cnt -lt $iterations ]
do
    cnt=`expr $cnt + 1`
    echo Starting iteration $cnt
    test_anode nbs-recovery.test -var 3
    rc=$?
    if [ $rc -eq 129 ]
      then
	echo Saving unrecovered aggregate.
	scp ${TEST_ANODE_TMPDEV}-two ${TEST_ANODE_TMPDEV}-save
    elif [ $rc -ne 0 ]
        then echo Unexpected exit status is $rc.;break
    fi
done
