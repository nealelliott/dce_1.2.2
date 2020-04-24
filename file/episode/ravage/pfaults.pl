#!/usr/local/bin/perl
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: pfaults.pl,v $
# Revision 1.1.13.1  1996/10/02  17:26:02  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:04  damon]
#
# Revision 1.1.8.1  1994/06/09  14:01:51  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:04  annie]
# 
# Revision 1.1.4.3  1993/01/19  15:31:06  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:52:19  cjd]
# 
# Revision 1.1.4.2  1992/09/25  16:21:26  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:49:46  jaffe]
# 
# Revision 1.1.2.2  1992/05/22  21:11:33  jdp
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
# 	[1992/05/22  20:07:14  jdp]
# 
# Revision 1.1.1.2  1992/05/22  20:07:14  jdp
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

$DELIMITER='##########';
$FAULT_EXPR='(##!!)|(core)';

while (<>) {
    if (/^${DELIMITER}\n$/) {
        if (grep(/$FAULT_EXPR/, @test_output)) {
            while (@test_output) {
                print shift(@test_output);
            }
            print "$DELIMITER\n";
        } else {
            $#test_output = -1;
        }
    } else {
        push(@test_output,$_);
    }
}
