#!/usr/local/bin/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: epidump.pl,v $
# Revision 1.1.8.1  1996/10/17  18:35:30  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:37  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:37  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:17:04  devsrc]
# 
# 	moving from file/episode to test/file
# 	[1994/03/22  21:02:46  delgado]
# 
# 	Transarc delta: jdp-ot5165-add-bitmap-flag-to-epidump 1.1
# 	  Selected comments:
# 	    This delta gives epidump the ability to dump bitmaps.
# 	    Add code for processing the "-bitmap" flag.
# 	[1992/09/14  21:07:57  jaffe]
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
# 	[1992/05/22  20:41:56  jdp]
# 
# Revision 1.1.4.4  1993/01/21  18:23:53  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:13:50  zeliff]
# 
# Revision 1.1.4.3  1992/09/25  16:28:12  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:51:09  jaffe]
# 
# Revision 1.1.4.2  1992/09/15  17:39:54  jaffe
# 	Transarc delta: jdp-ot5165-add-bitmap-flag-to-epidump 1.1
# 	  Selected comments:
# 	    This delta gives epidump the ability to dump bitmaps.
# 	    Add code for processing the "-bitmap" flag.
# 	[1992/09/14  21:07:57  jaffe]
# 
# Revision 1.1.1.2  1992/05/22  20:41:56  jdp
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

if ($ARGV[0] eq "-bitmap") {
    $show_bitmap = 1;
    shift(@ARGV);
}

require 'efs_query';

select(STDOUT);

if ($show_bitmap) {
    &Flush(*GetAggr, 1,		*GetAVLAnode, 1,
	   *GetBitmapPage, 1,	*PrintBitmapPage, 0);
}

&Flush(*GetAggr, 1,	*PrintAggr, 1,
       *GetAVLAnode, 1,	*PrintAnode, 1,
       *GetFileSet, 1,	*PrintFileSet, 1,
       *GetFSAnode, 1,	*PrintAnode, 1,
       *GetDirPage, 1,	*PrintDirPage, 1,
       *GetDirEntry, 1,	*PrintDirEntry, 0);
