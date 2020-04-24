#!/usr/local/bin/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: upquota.pl,v $
# Revision 1.1.13.1  1996/10/02  17:28:40  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:36  damon]
#
# Revision 1.1.8.1  1994/06/09  14:03:39  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:38:36  annie]
# 
# Revision 1.1.4.3  1993/01/21  18:24:00  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:14:02  zeliff]
# 
# Revision 1.1.4.2  1992/09/25  16:28:28  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:51:23  jaffe]
# 
# Revision 1.1.2.2  1992/05/22  21:18:19  jdp
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
# 	[1992/05/22  20:45:45  jdp]
# 
# Revision 1.1.1.2  1992/05/22  20:45:45  jdp
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

require 'efs_query';


if (&FindObject(*GetAggr, 1, *GetAVLAnode, 1, 
		*GetFileSet, q/$fs{'ident.name'} eq $main'ARGV[1]/)) {
    open(TMP_AGGR, "+<$AGGR") || &Die("${AGGR}: open(2) failed");
    &SetFileSet('quota.limit', $main'ARGV[2]);
}


