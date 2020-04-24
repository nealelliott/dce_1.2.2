#!/usr/local/bin/perl
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: pfaults.pl,v $
# Revision 1.1.8.1  1996/10/17  18:33:30  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:36  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:11  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:14:29  devsrc]
# 
# 	moving files from file/episode to test/file
# 	[1994/03/22  20:28:52  delgado]
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
# 	[1992/05/22  20:07:14  jdp]
# 
# Revision 1.1.4.3  1993/01/19  15:31:06  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:52:19  cjd]
# 
# Revision 1.1.4.2  1992/09/25  16:21:26  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:49:46  jaffe]
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
