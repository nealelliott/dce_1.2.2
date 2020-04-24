#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: cell_alias-tc.sh,v $
# Revision 1.1.4.3  1996/03/09  20:47:31  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:22  marty]
#
# Revision 1.1.4.2  1995/12/11  19:52:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:48  root]
# 
# Revision 1.1.4.1  1995/07/11  23:25:29  marty
# 	      HP warrenty patch
# 	      [1995/07/11  23:19:45  marty]
# 
# $EndLog$
#

tet_startup="startup"
tet_cleanup="cleanup"

iclist="ic1"
ic1="cell_alias_tp1"
tet_testlist="cell_alias_tp1"

#
# NAME : this is the name of the test user
#
NAME=cellaliasprinc
# CELL_ADMIN : this is the name of the principal who will be able
# to run rgy_edit
#
CELL_ADMIN=cell_admin
#
# CELL_PW : this is the password of the principal who will be able
# to run rgy_edit
#
CELL_PW=-dce-
#
# CELL_ALIAS : this is the suffix of the cell alias the script creates
#
CELL_ALIAS=_alias_$(date +%y%m%d_%H%M)
export NAME CELL_ADMIN CELL_PW CELL_ALIAS


myecho()
{
    tet_infoline $1
    print $1
}

startup()
{
    # Add principal & account
    # Enable cell aliases & create one for our cell

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp -c   "principal create $NAME ;\
		group add none -member $NAME ;\
		organization add none -member $NAME ;\
		directory modify /.: -add {CDS_UpgradeTo 4.0} ;\
		directory synchronize /.: ;\
		clearinghouse verify [clearinghouse catalog] ;\
		directory synchronize /.: ;\
		directory show /.: ;\
		set aliasname [name expand /.:] ;\
		append aliasname $CELL_ALIAS ;\
		cellalias create \$aliasname"
	> ./startup_errors 2>&1

    if [ $? -ne 0 ]
    then
        myecho "FAILED: startup. See startup_errors file"
	for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup has failed."
        done
        return
    fi

#
# !!! NOTE !!!
#
# dcecp does not allow the user to input passwords on the command
# line via the -c switch so we have to go back to using rgy_edit
# until we figure out a way around this.
#
    dce_login $CELL_ADMIN  $CELL_PW -e \
    rgy_edit -update << EOF >> ./startup_errors 2>&1
do account
add $NAME -g none -o none -pw test -mp $CELL_PW
exit
EOF

    if [ $? -ne 0 ]
    then
        myecho "FAILED: startup. See startup_errors file"
        for test_purp in $tet_testlist
        do
            tet_delete $test_purp "startup has failed."
        done
        return
    fi
}

cell_alias_tp1()
{
    myecho "DCE Cell Alias test purpose 1: successful login"

    dce_login $(getcellname)$CELL_ALIAS/$NAME test -e kdestroy >> ./tp1_errors 2>&1
    if [ $? -ne 0 ]
    then
        myecho "FAILED: cell_alias_tp1: see tp1_errors file"
        tet_result FAIL
        return
    fi

    myecho "PASSED: cell_alias_tp1"
    tet_result PASS
}

cleanup()
{
    #
    # Note that there's no "cellalias delete" operation
    #

    dce_login $CELL_ADMIN  $CELL_PW -e \
    dcecp -c "account delete $NAME; \
              principal delete $NAME" > ./cleanup_errors 2>&1

    if [ $? -ne 0 ]
    then
        myecho "FAILED: cleanup. See cleanup_errors file"
    fi
}

. $TET_ROOT/lib/ksh/tcm.ksh
