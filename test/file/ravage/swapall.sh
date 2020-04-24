#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: swapall.sh,v $
# Revision 1.1.8.1  1996/10/17  18:33:40  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:42  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:20  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:14:53  devsrc]
# 
# 	moving files from file/episode to test/file
# 	[1994/03/22  20:29:00  delgado]
# 
# 	Transarc delta: ota-ot4438-make-ravage-use-path 1.1
# 	  Selected comments:
# 	    Need ravage scripts to be easier to use.  Changing them to use the path
# 	    to locate each other helps.  Also check the environment variable SALVAGE
# 	    to override the salvage program found via path.
# 	    Locate rvg_setup via path.
# 	[1992/08/30  02:40:59  jaffe]
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
# 	[1992/05/22  20:16:07  jdp]
# 
# Revision 1.1.4.4  1993/01/19  15:31:26  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:52:49  cjd]
# 
# Revision 1.1.4.3  1992/09/25  16:27:25  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:50:22  jaffe]
# 
# Revision 1.1.4.2  1992/08/31  20:09:56  jaffe
# 	Transarc delta: ota-ot4438-make-ravage-use-path 1.1
# 	  Selected comments:
# 	    Need ravage scripts to be easier to use.  Changing them to use the path
# 	    to locate each other helps.  Also check the environment variable SALVAGE
# 	    to override the salvage program found via path.
# 	    Locate rvg_setup via path.
# 	[1992/08/30  02:40:59  jaffe]
# 
# Revision 1.1.1.2  1992/05/22  20:16:07  jdp
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

cat $RVG_NZB | (read block1; while read block2; do
	RvgBeginTest

	RvgExecTest swap $RVG_AGGR $block1 $block2 $RVG_BUF_SIZE

	RvgEndTest $block1 $block2

	block1=${block2}
done)
