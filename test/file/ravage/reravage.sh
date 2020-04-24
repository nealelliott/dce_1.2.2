#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: reravage.sh,v $
# Revision 1.1.8.1  1996/10/17  18:33:33  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:38  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:13  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:14:49  devsrc]
# 
# 	moving files from file/episode to test/file
# 	[1994/03/22  20:28:54  delgado]
# 
# 	Transarc delta: jdp-ot3675-lfs-add-salvager-tests 1.1
# 	  Files modified:
# 	    Makefile
# 	    ravage: Makefile, and.c, bash.c, copy.c, copyall.sh
# 	    ravage: metamucil.c, nebconv.pl, pfaults.pl, ravage.sh
# 	    ravage: reravage.sh, rvg_setup.sh, scp.c, setall.sh, swap.c
# 	    ravage: swapall.sh, which.sh, zeroall.sh
# 	    scavenge: Makefile, aggrinfo.pl, efs.pl, efs_query.pl
# 	    scavenge: epidump.pl, fsdump.pl, simple_test.pl, upquota.pl
# 	    scavenge: util.pl
# 	  Selected comments:
# 	    This delta adds the ravager and scavenger tests to the Episode source tree.
# 	    These packages are for testing the Episode salvager.
# 	[1992/05/22  20:09:26  jdp]
# 
# Revision 1.1.4.3  1993/01/19  15:31:11  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:52:27  cjd]
# 
# Revision 1.1.4.2  1992/09/25  16:21:36  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:49:59  jaffe]
# 
# Revision 1.1.1.2  1992/05/22  20:09:26  jdp
# 	Transarc delta: jdp-ot3675-lfs-add-salvager-tests 1.1
# 	  Files modified:
# 	    Makefile
# 	    ravage: Makefile, and.c, bash.c, copy.c, copyall.sh
# 	    ravage: metamucil.c, nebconv.pl, pfaults.pl, ravage.sh
# 	    ravage: reravage.sh, rvg_setup.sh, scp.c, setall.sh, swap.c
# 	    ravage: swapall.sh, which.sh, zeroall.sh
# 	    scavenge: Makefile, aggrinfo.pl, efs.pl, efs_query.pl
# 	    scavenge: epidump.pl, fsdump.pl, simple_test.pl, upquota.pl
# 	    scavenge: util.pl
# 	  Selected comments:
# 	    This delta adds the ravager and scavenger tests to the Episode source tree.
# 	    These packages are for testing the Episode salvager.
# 
# $EndLog$
#

# Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved

cmd=""
prog_name=`basename $0`
tmp_aggr=/tmp/rvgaggr.$$
usage="usage: ${prog_name} [<ravage command>]"

Salvage()	{ salvage "$@"; }
Scp()		{ scp "$@" || Fatal "scp failed"; }

Bin2Hex()
{
	echo "$1" | awk '{ printf("%#x\n", $1); }'
}

Fatal()
{
	echo "${prog_name}: $*" 1>&2
	exit 2
}


if [ $# -eq 0 ]; then
	read first_line
else
	if [ $# -lt 2 ]; then
		Fatal "${usage}"
	fi

	cmd="$*"
	first_line="$*"
fi

set - ${first_line}
orig_aggr="$2"

if [ ! -r "${orig_aggr}" ]; then
	echo "${prog_name}: ${orig_aggr}: Can't access" 1>&2
	exit 2
fi

echo "Verifying..."
Salvage -verify "${orig_aggr}" || Fatal "Verify of ${orig_aggr} failed"

trap "rm -f ${tmp_aggr}; exit 0" 0 1 2 3 15

echo ""
echo "Copying..."
Scp "${orig_aggr}" "${tmp_aggr}"
echo ""

(echo "${first_line}"; test -z "${cmd}" && cat) | while read line; do
	set - ${line}

	case "$1" in
	bash|swap)
		cmd=$1
		shift
		;;
	*)	echo "${prog_name}: $1: Skipping illegal command" 1>&2
		continue
		;;
	esac

	if [ "$1" != "${orig_aggr}" ]; then
		echo "${prog_name}: $2: Different aggregate" 1>&2
		continue
	fi
	shift

	echo "Executing: ${cmd}" "${tmp_aggr}" "$@"
	"${cmd}" "${tmp_aggr}" "$@"
done

echo ""
echo "Salvaging..."
Salvage "${tmp_aggr}"
echo "Salvager returned status of `Bin2Hex $?`"
