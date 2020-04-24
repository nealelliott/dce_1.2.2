#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: export_check-tc.sh,v $
# Revision 1.1.2.1  1996/10/03  20:36:13  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:44:13  arvind]
#
# 	OSF DCE 1.2.2 Drop 4
#
# Revision /main/DCE_1.2.2/2  1996/09/30  15:44 UTC  arvind
# 	Add OSF copyright
# 
# $EndLog$
#

tet_startup=
tet_cleanup=

iclist="ic1"
ic1="export_check_tp1"
tet_testlist="export_check_tp1"


mtet_infoline()
{
    tet_infoline $1
    print $1
}


export_check_tp1()
{
    mtet_infoline "PK login Exportability tp1: check that sec_pvtkey and sec_bsafe symbols don't exist."

    if [ $(nm /usr/lib/libdce.sl | grep sec_pvtkey | wc -l) -ne 0 ]
    then
        mtet_infoline "FAILED: PK login Exportability tp1: libdce.sl contains sec_pvtkey symbols."
        tet_result FAIL
    fi

    if [ $(nm /usr/lib/libdce.sl | grep sec_bsafe | wc -l) -ne 0 ]
    then
        mtet_infoline "FAILED: PK login Exportability tp1: libdce.sl contains sec_bsafe symbols. "
        tet_result FAIL
    fi

    # if tet FAIL, overrides PASS
    tet_result PASS

}

. $TET_ROOT/lib/ksh/tcm.ksh
