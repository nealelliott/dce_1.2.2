#!/bin/sh

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.

# HISTORY
# $Log: gg_setup.sh,v $
# Revision 1.1.2.1  1996/10/16  14:04:51  arvind
# 	OSF DCE 1.2.2 Drop 4.5
# 	[1996/10/16  13:57:05  arvind]
#
# $EndLog$

# This file is not used in the automated (TETized) tests for GG.
# A copy of it is provided to make a complete submission.

# This script needs variables settings done in tetexec.cfg 
# . ${TET_ROOT}/functional/security/tetexec.cfg

RGY_EDIT="/usr/bin/rgy_edit -update"
RGY_SCRIPT="/tmp/rgy_script.$$"
DCE_LOGIN="/usr/bin/dce_login"
DCE_LOGIN_NOEXEC="/usr/bin/dce_login_noexec"
KDESTROY=/"usr/bin/kdestroy"
AWK="/usr/bin/awk"
ECHO="echo"
#ECHO=":"

usage() {
	cat << E_O_F
usage: $0 {local | remote | clean} [<cell_admin> [<password>]]
E_O_F
}

cleanup() {
	$KDESTROY
	rm -f $RGY_SCRIPT
	rm -f $DCE_LOGIN_NOEXEC
}

#
# main procedure
#
if [ $# -lt 1 ]; then
	usage
	exit 1
fi

# analyze the arguments
case "$1" in
local)	WHICH_CELL=local;;
remote)	WHICH_CELL=remote;;
clean)	WHICH_CELL=clean;;
*)	usage()
	exit 1
	;;
esac

if [ $# -eq 2 ]; then
	CELL_ADMIN=$2
fi
if [ $# -eq 3 ]; then
	CELL_ADMIN=$2
	PASSWORD=$3
fi

# login to dce
if [ ! -f $DCE_LOGIN_NOEXEC ]; then
	ln -s $DCE_LOGIN $DCE_LOGIN_NOEXEC
fi
CMD=`basename $DCE_LOGIN_NOEXEC`
KRB5CCNAME=`$CMD $CELL_ADMIN $PASSWORD 2> /dev/null`
export KRB5CCNAME

#
$ECHO "** cleanup the registry entry if the specific groups already exist **"
#
case $WHICH_CELL in
local|clean)
	> $RGY_SCRIPT
	cat >> $RGY_SCRIPT << E_O_F
domain g
delete $gG
delete $gGa
delete $gGnp
exit
E_O_F

	$RGY_EDIT < $RGY_SCRIPT > /dev/null 2>&1
	;;

remote|clean)
	> $RGY_SCRIPT
	cat >> $RGY_SCRIPT << E_O_F
domain g
delete $gR
exit
E_O_F

	$RGY_EDIT < $RGY_SCRIPT > /dev/null 2>&1
	;;

*)	;;
esac

#
$ECHO "** cleanup the registry entry if the specific principals already exist **"
#
> $RGY_SCRIPT

case $WHICH_CELL in
local|clean)
	cat >> $RGY_SCRIPT << E_O_F
domain p
delete $pL
delete $pLo
exit
E_O_F
	;;

remote|clean)
	cat >> $RGY_SCRIPT << E_O_F
domain p
delete $pR
delete $pRs
delete $pRl
delete $pRa
exit
E_O_F
	;;

*)	;;
esac


$RGY_EDIT < $RGY_SCRIPT > /dev/null 2>&1

if [ $WHICH_CELL = "clean" ]; then
	cleanup
	exit 0
fi

#
$ECHO "** add the groups/principals/accounts for GlobalGroup testing **"
#
case $WHICH_CELL in
local)
	> $RGY_SCRIPT
	cat >> $RGY_SCRIPT << E_O_F
domain g
add $gG
add $gGnp -nl
domain p
add $pL
add $pLo
domain a
add $pL -g none -o none -mp $PASSWORD -pw $pL
add $pLo -g none -o none -mp $PASSWORD -pw $pLo
exit
E_O_F

	$RGY_EDIT < $RGY_SCRIPT
	EXIT=$?
	if [ $EXIT -ne 0 ]; then
		cleanup
		exit $EXIT
	fi

	# add an alias group
	unix_num=`$RGY_EDIT -g -v | grep -e "^$gG[ 	]" | $AWK '{print $2}'`
	> $RGY_SCRIPT
	cat >> $RGY_SCRIPT << E_O_F
domain g
add $gGa $unix_num -al
exit
E_O_F

	$RGY_EDIT < $RGY_SCRIPT
	EXIT=$?
	if [ $EXIT -ne 0 ]; then
		cleanup
		exit $EXIT
	fi

	;;

remote)
	> $RGY_SCRIPT
	cat >> $RGY_SCRIPT << E_O_F
domain g
add $gR
domain p
add $pR
add $pRs
add $pRl
domain a
add $pR -g none -o none -mp $PASSWORD -pw $pR
add $pRs -g none -o none -mp $PASSWORD -pw $pRs
add $pRl -g none -o none -mp $PASSWORD -pw $pRl
exit
E_O_F

	$RGY_EDIT < $RGY_SCRIPT
	EXIT=$?
	if [ $EXIT -ne 0 ]; then
		cleanup
		exit $EXIT
	fi

	# add an alias account
	unix_num=` $RGY_EDIT -p -v | grep -e "^$pR[ 	]" | $AWK '{print $2}'`
	> $RGY_SCRIPT
	cat >> $RGY_SCRIPT << E_O_F
domain p
add $pRa $unix_num -al
domain a
add $pRa -g none -o none -mp $PASSWORD -pw $pRa
exit
E_O_F

	$RGY_EDIT < $RGY_SCRIPT
	EXIT=$?
	if [ $EXIT -ne 0 ]; then
		cleanup
		exit $EXIT
	fi

	;;

*)	;;
esac

# cleanup the work files
cleanup
exit 0
