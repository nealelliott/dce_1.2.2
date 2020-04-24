#!/bin/sh

#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
#
# HISTORY
# $Log: rem_nfs_exp.sh,v $
# Revision 1.1.6.1  1996/10/17  18:24:02  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:14  damon]
#
# Revision 1.1.2.1  1994/07/28  17:38:42  mckeen
# 	Initial Revision
# 	[1994/07/28  14:10:37  mckeen]
# 
# $EndLog$

cellpw="-dce-"
DCELOCAL="/opt/dcelocal"

BOSCONFIG=$DCELOCAL/var/dfs/BosConfig
SUBSYSDIR="subsys/dce"
DFS_ADMIN_GROUP="$SUBSYSDIR/dfs-admin"

org="none"

HOSTNAME=`hostname`

terminate_bosserver()
{

HEADING=`ps -e | head -1`        # generating region info from
where=`expr "$HEADING" : "\(.* PID \)"` # ps output to find where to
PIDEND=`expr length "$where"`           # get process id from
PIDSTART=`expr $PIDEND - 5`             # PIDSTART = start of field
CUT_LINE="cut -c${PIDSTART}-${PIDEND}"  # PIDEND = end of field
        
        ps -ef | grep -q bosserver | grep -v grep
        if [ $? -eq 0 ]; then
                echo "bosserver not running."
        else
                 PID=`ps -e | grep bosserver | ${CUT_LINE}`
                 kill $PID        
        fi

        rm -f $DCELOCAL/var/dfs/admin.*
        rm -f $BOSCONFIG

}


dfs_rgyremove()
{
	rgy_edit -update -a hosts/$HOSTNAME/dfs-server -v | grep "^hosts/$HOSTNAME/dfs-server" >/dev/null
	if [ $? -eq 0 ]
	then
             rgy_edit -update <<EOF
domain principal
del hosts/$HOSTNAME/dfs-server
y
domain account
del hosts/$HOSTNAME/dfs-server
y
ktd -p hosts/$HOSTNAME/dfs-server -v 1
ktd -p hosts/$HOSTNAME/dfs-server -v 2
quit
EOF
        fi
}


dlog_rgyremove()
{
	rgy_edit -update -g $SUBSYSDIR/dlog-admin -v | grep "^$SUBSYSDIR/dlog-admin" >/dev/null
	if [ $? -eq 0 ]
	then
             rgy_edit -update <<EOF
domain group
del $SUBSYSDIR/dlog-admin
y
quit
EOF
	fi
	rgy_edit -update -a hosts/$HOSTNAME/dlog -v | grep "^hosts/$HOSTNAME/dlog" >/dev/null
	if [ $? -eq 0 ]
	then
             rgy_edit -update <<EOF
domain principal
del hosts/$HOSTNAME/dlog
y
domain account
del hosts/$HOSTNAME/dlog 
y
ktd -p hosts/$HOSTNAME/dlog -v 1
ktd -p hosts/$HOSTNAME/dlog -v 2 
quit
EOF
	fi
}

#
#  get rid of  bosserver

terminate_bosserver
dfs_rgyremove

#
# remove rgy stuff for dfsgw
dlog_rgyremove
