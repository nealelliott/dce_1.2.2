#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: rvg_setup.sh,v $
# Revision 1.1.13.1  1996/10/02  17:26:08  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:08  damon]
#
# Revision 1.1.8.1  1994/06/09  14:01:56  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:07  annie]
# 
# Revision 1.1.4.4  1993/01/19  15:31:14  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:52:31  cjd]
# 
# Revision 1.1.4.3  1992/09/25  16:21:38  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:50:12  jaffe]
# 
# Revision 1.1.4.2  1992/08/31  20:09:24  jaffe
# 	Transarc delta: jdp-ot4537-make-ravager-print-every-command 1.1
# 	  Selected comments:
# 	    Have the ravager print every command before running the salvager.
# 	    Change RvgExecTest(), SalvageError(), and SalvageWarning() to have all ravage
# 	    commands printed.  The command log is no longer necessary--wipe it out.
# 	Transarc delta: ota-ot4438-make-ravage-use-path 1.1
# 	  Selected comments:
# 	    Need ravage scripts to be easier to use.  Changing them to use the path
# 	    to locate each other helps.  Also check the environment variable SALVAGE
# 	    to override the salvage program found via path.
# 	    Don't modify path (invoker must do this).
# 	    Default SALVAGE environment variable to "salvage" if not set.
# 	[1992/08/30  02:40:24  jaffe]
# 
# Revision 1.1.3.2  1992/08/30  02:40:24  jaffe
# 	Transarc delta: jdp-ot4537-make-ravager-print-every-command 1.1
# 	  Selected comments:
# 	    Have the ravager print every command before running the salvager.
# 	    Change RvgExecTest(), SalvageError(), and SalvageWarning() to have all ravage
# 	    commands printed.  The command log is no longer necessary--wipe it out.
# 	Transarc delta: ota-ot4438-make-ravage-use-path 1.1
# 	  Selected comments:
# 	    Need ravage scripts to be easier to use.  Changing them to use the path
# 	    to locate each other helps.  Also check the environment variable SALVAGE
# 	    to override the salvage program found via path.
# 	    Don't modify path (invoker must do this).
# 	    Default SALVAGE environment variable to "salvage" if not set.
# 
# Revision 1.1.2.2  1992/05/22  21:12:20  jdp
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
# 	[1992/05/22  20:11:03  jdp]
# 
# Revision 1.1.1.2  1992/05/22  20:11:03  jdp
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

block_log=""
copy_blocks="/tmp/rvgcb.$$"
def_aggr_tmp_dir=/tmp
def_block_size=8192
def_buf_size=8192
def_frag_size=1024
prog_name=`basename $0`
usage="usage: ${prog_name} [-b <block size>] [-f <frag size>] [-B <buf size>] [-l <buf log>] [-t <aggr tmp dir>] [-e <no effect log>] <aggr>"

: ${SALVAGE:=salvage}			# allow caller to override

RVG_NEB=""
RVG_NZB="/tmp/rvgnzb.$$"

Cp()		{ cp "$@" || Fatal "cp failed"; }
Diff()		{ diff "$@"; }
Metamucil()	{ metamucil "$@" || Fatal "metamucil failed"; }
Mv()		{ mv "$@" || Fatal "mv failed"; }	
Rm()		{ rm "$@" || Fatal "rm failed"; }
Salvage()	{ $SALVAGE "$@"; }
Scp()		{ scp "$@" || Fatal "scp failed"; }
Sed()		{ sed "$@" || Fatal "sed failed"; }
Sort()		{ sort "$@" || Fatal "sort failed"; }
Sum()		{ sum "$@" || Fatal "sum failed"; }
Uniq()		{ uniq "$@" || Fatal "uniq failed"; }
Which()		{ which "$@" || Fatal "which failed"; }


Bin2Hex()
{
	echo "$1" | awk '{ printf("%#x\n", $1); }'
}

Fatal()
{
	echo "${prog_name}: $*" 1>&2
	exit 2
}

SalvageError()
{
	echo "##!!$*"
}

SalvageWarning()
{
	echo "##!$*"
}

RvgBeginTest()
{
	echo "##########"
	Scp -N "${copy_blocks}" "${orig_aggr}" "${RVG_AGGR}"
	echo "## Running the following commands:"
}

RvgExecTest()
{
	echo "## $*"
	$*
}

RvgEndTest()
{
	if [ "$1" != "recursive" ]; then
		block=$1
	fi

	Salvage "${RVG_AGGR}"
	status=$?
	case ${status} in
	0)	SalvageWarning "Salvager didn't report any damage" 
		while [ -n "$*" ]; do
			if [ "$1" != "recursive" ]; then
				echo -n "##"
				Metamucil -w $1 "${orig_aggr}"
			fi
			shift
		done
		if [ "$1" != "recursive" -a -n "${RVG_NEB}" ]; then
			echo "${block}" >>"${RVG_NEB}"
		fi
		;;
	1|2)	SalvageError "Salvager returned illegal status (0x${status})"
		;;
	3)	echo "##Verifying successful salvage..."
		Salvage -verify "${RVG_AGGR}"
		if [ ! $? -eq 0	]; then
			SalvageError "Verification failed"
		fi
		;;
	*)	if [ `and 128 ${status}` != "0" ]; then
			SalvageError \
			     "Salvage dumped core (status=`Bin2Hex ${status}`)"
		elif [ `and 4 ${status}` != "0" -a \
			  "$1" != "recursive" ]; then
			echo -n "##Salvage returned `Bin2Hex ${status}`, "
			echo "requiring another salvage..."
			RvgEndTest recursive "$@"
		else
			SalvageError \
				"Salvage failed (status=`Bin2Hex ${status}`)"
		fi
		;;
	esac

	if [ -n "${buf_log}" -a "$1" != "recursive" ]; then
		echo "${block}" >>"${buf_log}"
	fi

	Rm -f core
}


if [ "${OPTIND}" = 1 ]; then
	while getopts b:f:B:l:t:e: i; do
		case ${i} in
		b)	RVG_BLOCK_SIZE="${OPTARG}"
			;;
		f)	RVG_FRAG_SIZE="${OPTARG}"
			;;
		B)	RVG_BUF_SIZE="${OPTARG}"
			;;
		l)	buf_log="${OPTARG}"
			;;
		t)	RVG_AGGR_TMP_DIR="${OPTARG}"
			;;
		e)	RVG_NEB="${OPTARG}"
			;;
		\?)	Fatal "${usage}"
       			;;
		esac
	done
	shift `expr ${OPTIND} - 1`
else
	set -- `getopt b:f:B:l:t:e: $*`
	if [ $? != 0 ]; then
		Fatal "${usage}"
	fi
	for i in $*; do
		case $i in
		-b)	RVG_BLOCK_SIZE="$2"
			shift 2
			;;
		-f)	RVG_FRAG_SIZE="$2"
			shift 2
			;;
		-B)	RVG_BUF_SIZE="$2"
			shift 2
			;;
		-l)	buf_log="$2"
			shift 2
			;;
		-t)	RVG_AGGR_TMP_DIR="$2"
			shift 2
			;;
		-e)	RVG_NEB="$2"
			shift 2
			;;
		--)	shift
			break
			;;
		esac
	done
fi

if [ $# -lt 1 ]; then
	Fatal "${usage}"
fi

case "$1" in
/*)	orig_aggr="$1"
	;;
*)	orig_aggr="`pwd`/$1"
	;;
esac

: ${RVG_AGGR_TMP_DIR:=${def_aggr_tmp_dir}};  export RVG_AGGR_TMP_DIR
: ${RVG_BLOCK_SIZE:=${def_block_size}};  export RVG_BLOCK_SIZE
: ${RVG_BUF_SIZE:=${def_buf_size}};  export RVG_BUF_SIZE
: ${RVG_FRAG_SIZE:=${def_frag_size}};  export RVG_FRAG_SIZE

RVG_AGGR="${RVG_AGGR_TMP_DIR}/rvgaggr.$$"

if [ ! -r "${orig_aggr}" ]; then
	Fatal "${orig_aggr}: Can't access"
fi

if [ ! -d "${RVG_AGGR_TMP_DIR}" -o ! -x "${RVG_AGGR_TMP_DIR}" ]; then
	Fatal "${RVG_AGGR_TMP_DIR}: Not a directory or can't access"
fi

if [ -n "${buf_log}"  ]; then
	salvager=`Which salvage`
	x=`Sum -r ${salvager}`
	set - ${x}
	checksum=$1
	size=$2

	if [ -f "${buf_log}" ]; then
		if [ ! -r "${buf_log}" ]; then
			Fatal "${buf_log}: Can't read"
		fi

		read xsalvager xchecksum xsize xbufsize <"${buf_log}"

		if [ "${RVG_BUF_SIZE}" != "${xbufsize}" ]; then
			Fatal "${buf_log}: different buf size (${xbufsize})"
		fi

		if [ "${salvager}" != "${xsalvager}" -o \
		     "${checksum}" != "${xchecksum}" -o \
		     "${size}" != "${xsize}" ]; then
			echo "Salvager changed; starting from beginning..."
			Cp "${buf_log}" "${buf_log}.bak"
			echo "${salvager} ${checksum} ${size} ${RVG_BUF_SIZE}"\
				>"${buf_log}"
		fi
	else
		echo "${salvager} ${checksum} ${size} ${RVG_BUF_SIZE}" \
			>"${buf_log}"
	fi
fi

trap "rm -f ${RVG_AGGR} ${RVG_NZB} ${copy_blocks}; exit 0" 0 1 2 3 15

echo "Finding non-zero bufs..."
Scp -n "${orig_aggr}" >"${copy_blocks}"

if [ -n "${buf_log}" ]; then
	Diff "${buf_log}" "${copy_blocks}" | Sed -n 's/^> \(.*\)/\1/p' \
	>"${RVG_NZB}"
else
	Cp "${copy_blocks}" "${RVG_NZB}"
fi

if [ -n "${RVG_NEB}" ]; then
	if [ -f "${RVG_NEB}" ]; then
		if [ ! -r "${RVG_NEB}" -o \
		     ! -w "${RVG_NEB}" ]; then
			Fatal "${RVG_NEB}: Can't access"
		fi
	fi
fi

echo ""
echo "Verifying aggregate..."
Salvage -verify "${orig_aggr}" || Fatal "Verify of ${orig_aggr} failed"

