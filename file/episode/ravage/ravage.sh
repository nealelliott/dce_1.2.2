#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: ravage.sh,v $
# Revision 1.1.13.1  1996/10/02  17:26:04  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:06  damon]
#
# Revision 1.1.8.1  1994/06/09  14:01:53  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:05  annie]
# 
# Revision 1.1.4.3  1993/01/19  15:31:09  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:52:23  cjd]
# 
# Revision 1.1.4.2  1992/09/25  16:21:31  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:49:50  jaffe]
# 
# Revision 1.1.2.2  1992/05/22  21:11:50  jdp
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
# 	[1992/05/22  20:08:27  jdp]
# 
# Revision 1.1.1.2  1992/05/22  20:08:27  jdp
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

PATH=".:${PATH}"
out_dir=${RVG_OUTPUT:-.}

if [ $# -lt 1 ]; then
	echo "usage: ravage <aggr> [<dir>]" 1>&2
	exit 2
fi

aggr=$1
dir=${2:-.}

for i in 8192 512; do
	zeroall -B ${i} ${dir}/${aggr} >${out_dir}/${aggr}.zeroall.${i}.O 2>&1
	setall -B ${i} ${dir}/${aggr} >${out_dir}/${aggr}.setall.${i}.O 2>&1
	swapall -B ${i} ${dir}/${aggr} >${out_dir}/${aggr}.swapall.${i}.O 2>&1
	copyall -B ${i} ${dir}/${aggr} >${out_dir}/${aggr}.copyall.${i}.O 2>&1
done
