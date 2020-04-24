#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: setall.sh,v $
# Revision 1.1.13.1  1996/10/02  17:26:12  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:14  damon]
#
# Revision 1.1.8.1  1994/06/09  14:02:00  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:09  annie]
# 
# Revision 1.1.4.4  1993/01/19  15:31:20  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:52:40  cjd]
# 
# Revision 1.1.4.3  1992/09/25  16:27:23  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:50:17  jaffe]
# 
# Revision 1.1.4.2  1992/08/31  20:09:42  jaffe
# 	Transarc delta: ota-ot4438-make-ravage-use-path 1.1
# 	  Selected comments:
# 	    Need ravage scripts to be easier to use.  Changing them to use the path
# 	    to locate each other helps.  Also check the environment variable SALVAGE
# 	    to override the salvage program found via path.
# 	    Locate rvg_setup via path.
# 	[1992/08/30  02:40:44  jaffe]
# 
# Revision 1.1.3.2  1992/08/30  02:40:44  jaffe
# 	Transarc delta: ota-ot4438-make-ravage-use-path 1.1
# 	  Selected comments:
# 	    Need ravage scripts to be easier to use.  Changing them to use the path
# 	    to locate each other helps.  Also check the environment variable SALVAGE
# 	    to override the salvage program found via path.
# 	    Locate rvg_setup via path.
# 
# Revision 1.1.2.2  1992/05/22  21:13:12  jdp
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
# 	[1992/05/22  20:13:38  jdp]
# 
# Revision 1.1.1.2  1992/05/22  20:13:38  jdp
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

. rvg_setup

cat $RVG_NZB | while read block; do
	if [ -n "$RVG_NEB" ]; then
		if grep -s "^${block}\$" $RVG_NEB; then
			echo '##########'
			echo "Skipping block $block..."
			echo '##########'
			continue
		fi
	fi

	RvgBeginTest

	RvgExecTest bash $RVG_AGGR $block $RVG_BUF_SIZE -1

	RvgEndTest $block
done
