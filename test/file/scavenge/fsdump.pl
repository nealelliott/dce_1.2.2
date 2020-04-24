#!/usr/local/bin/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: fsdump.pl,v $
# Revision 1.1.8.1  1996/10/17  18:35:32  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:38  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:39  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:17:05  devsrc]
# 
# 	moving from file/episode to test/file
# 	[1994/03/22  21:02:47  delgado]
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
# 	[1992/05/22  20:43:00  jdp]
# 
# Revision 1.1.4.3  1993/01/21  18:23:56  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:13:55  zeliff]
# 
# Revision 1.1.4.2  1992/09/25  16:28:18  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:51:13  jaffe]
# 
# Revision 1.1.1.2  1992/05/22  20:43:00  jdp
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

select(STDOUT);

&Flush(*GetAggr, 1, *GetAVLAnode, 1,
       *GetFileSet, q/$fs{'ident.name'} eq $main'ARGV[1]/,
       *PrintFileSet, 1,
       *GetFSAnode, 1,	*PrintAnode, 1,
       *GetDirPage, 1,	*PrintDirPage, 1,
       *GetDirEntry, 1,	*PrintDirEntry, 0);
