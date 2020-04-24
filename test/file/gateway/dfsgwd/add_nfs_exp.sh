#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
#
# HISTORY
# $Log: add_nfs_exp.sh,v $
# Revision 1.1.6.1  1996/10/17  18:23:58  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:12  damon]
#
# Revision 1.1.2.1  1994/07/28  17:38:41  mckeen
# 	Initial Revision
# 	[1994/07/28  14:10:36  mckeen]
# 
# $EndLog$

cellpw="-dce-"
DCELOCAL="/opt/dcelocal"

BOSCONFIG=$DCELOCAL/var/dfs/BosConfig
SUBSYSDIR="subsys/dce"
DFS_ADMIN_GROUP="$SUBSYSDIR/dfs-admin"

org="none"

HOSTNAME=`hostname`

startup_bosserver()
{
        ps -ef | grep -q bosserver | grep -v grep
        if [ $? -eq 0 ]; then
                echo "bosserver is already running."
		return
        fi

	rm -f $DCELOCAL/var/dfs/admin.*
	rm -f $BOSCONFIG
        $DCELOCAL/bin/bosserver -noauth &
        echo "Waiting for bosserver to start.\c"
        sleep 20
        timeout=0
        ps -ef | grep -q bosserver | grep -v grep > /dev/null 2>&1
        until [ $? -ne 0 ]
        do
           echo ".\c"
           sleep 1
           timeout=`expr $timeout + 1`
           if [ $timeout -gt 60 ]; then
                echo "bosserver failed to start.  Continuing will cause subsequent failures."
		return
           fi
           ps -ef | grep -q bosserver | grep -v grep > /dev/null 2>&1
        done
        echo "ready."

        # now add the minimal set of information to the bos server's
        # admin list

        $DCELOCAL/bin/bos addadmin -server /.:/hosts/$HOSTNAME -adminlist\
		admin.bos -group $DFS_ADMIN_GROUP

        # now turn on authorization checks.
	$DCELOCAL/bin/bos setauth -server /.:/hosts/$HOSTNAME -authchecking on
}

dfs_rgyinit()
{
	rgy_edit -update -a hosts/$HOSTNAME/dfs-server -v | grep "^hosts/$HOSTNAME/dfs-server" >/dev/null
	if [ $? -ne 0 ]
	then
             rgy_edit -update <<EOF
domain principal
add hosts/$HOSTNAME/dfs-server
domain account
add hosts/$HOSTNAME/dfs-server -g $SUBSYSDIR/dfs-admin -o none -pw $cellpw -mp $cellpw
ktadd -p hosts/$HOSTNAME/dfs-server -pw $cellpw
ktadd -p hosts/$HOSTNAME/dfs-server -a -r
quit
EOF


        acl_edit /.:/sec/principal/hosts/$HOSTNAME/dfs-server \
                    -m group:$SUBSYSDIR/dfs-admin:rcDnfmag
        fi
}


dlog_rgyinit()
{
	rgy_edit -update -g $SUBSYSDIR/dlog-admin -v | grep "^$SUBSYSDIR/dlog-admin" >/dev/null
	if [ $? -ne 0 ]
	then
             rgy_edit -update <<EOF
domain group
add $SUBSYSDIR/dlog-admin
quit
EOF
	fi

	rgy_edit -update -a hosts/$HOSTNAME/dlog -v | grep "^hosts/$HOSTNAME/dlog" >/dev/null
	if [ $? -ne 0 ]
	then
             rgy_edit -update <<EOF
domain principal
add hosts/$HOSTNAME/dlog
domain account
add hosts/$HOSTNAME/dlog -g $SUBSYSDIR/dlog-admin -o $org -pw $cellpw -mp $cellpw
ktadd -p hosts/$HOSTNAME/dlog -pw $cellpw
ktadd -p hosts/$HOSTNAME/dlog -a -r
quit
EOF
	fi
}

#
# Be sure bosserver is up and running

dfs_rgyinit
startup_bosserver

#
# Add rgy stuff for dfsgw
dlog_rgyinit

