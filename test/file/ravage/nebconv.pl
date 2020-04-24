#!/usr/local/bin/perl
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: nebconv.pl,v $
# Revision 1.1.8.1  1996/10/17  18:33:29  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:36  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:10  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:14:28  devsrc]
# 
# 	moving files from file/episode to test/file
# 	[1994/03/22  20:28:51  delgado]
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
# 	[1992/05/22  20:06:03  jdp]
# 
# Revision 1.1.4.3  1993/01/19  15:31:03  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:52:15  cjd]
# 
# Revision 1.1.4.2  1992/09/25  16:21:21  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:49:41  jaffe]
# 
# Revision 1.1.1.2  1992/05/22  20:06:03  jdp
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

$from = $ARGV[0];
$to = $ARGV[1];

$from % $to == 0 || die "First arg must be a multiple of the second";

$n = $from / $to;

while ($x = <STDIN>) {
    chop($x);
    for ($i = 0; $i < $n; $i++) {
        print $x + $i * $to, "\n";
    }
}

    
