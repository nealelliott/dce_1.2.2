#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: grp_override.sh,v $
# Revision 1.1.2.2  1996/03/09  20:47:35  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:25  marty]
#
# Revision 1.1.2.1  1995/12/11  19:53:50  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:24  root]
# 
# $EndLog$
#
MYDIR=$(dirname $0)

GRP_OVERRIDE_DATAFILE="/opt/dcelocal/etc/group_override"
SAVE_GRP_OVERRIDE_DATAFILE="/opt/dcelocal/etc/grp_oride_saveit"
TEST_GRP_OVERRIDE_DATAFILE=${MYDIR}/group_override

PASSWD_OVERRIDE_DATAFILE="/opt/dcelocal/etc/passwd_override"
SAVE_PASSWD_OVERRIDE_DATAFILE="/opt/dcelocal/etc/pw_oride_saveit"
TEST_PASSWD_OVERRIDE_DATAFILE=${MYDIR}/passwd_override

function setup 
{
    for ITEM in GRP PASSWD
    do
	eval DATAFILE=\$${ITEM}_OVERRIDE_DATAFILE
	eval SAVE_FILE=\$SAVE_${ITEM}_OVERRIDE_DATAFILE
	eval TEST_FILE=\$TEST_${ITEM}_OVERRIDE_DATAFILE

	if [ -f ${DATAFILE} ]
	then
		mv ${DATAFILE} ${SAVE_FILE}
	fi
	cp ${TEST_FILE} ${DATAFILE}
    done
}

function cleanup 
{
    for ITEM in GRP PASSWD
    do
	eval DATAFILE=\$${ITEM}_OVERRIDE_DATAFILE
	eval SAVE_FILE=\$SAVE_${ITEM}_OVERRIDE_DATAFILE
	eval TEST_FILE=\$TEST_${ITEM}_OVERRIDE_DATAFILE

	/bin/rm -f ${DATAFILE} 
	if [ -f ${SAVE_FILE} ]
	then
		mv ${SAVE_FILE} ${DATAFILE}
	fi
    done
}

setup
${MYDIR}/grp_override
cleanup

