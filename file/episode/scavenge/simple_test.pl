#!/usr/local/bin/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: simple_test.pl,v $
# Revision 1.1.13.1  1996/10/02  17:28:39  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:35  damon]
#
# Revision 1.1.8.1  1994/06/09  14:03:37  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:38:35  annie]
# 
# Revision 1.1.4.3  1993/01/21  18:23:58  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:13:58  zeliff]
# 
# Revision 1.1.4.2  1992/09/25  16:28:23  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:51:18  jaffe]
# 
# Revision 1.1.2.2  1992/05/22  21:18:05  jdp
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
# 	[1992/05/22  20:44:24  jdp]
# 
# Revision 1.1.1.2  1992/05/22  20:44:24  jdp
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

sub BashFields {
    local(*SetFunc, @format) = @_;
    local(@fields) = &util'FormatFields(@format);

    foreach $field (@fields) {
	foreach $val (0, -1) {
	    &StartTest();
	    &SetFunc($field, $val);
	    &EndTest();
	}
    }
}


if (&FindObject(*GetAggr, 1)) {
    &BashFields(*SetAggr, @efs'AGGR_HEADER_FORMAT);

    foreach $field ('blockSize', 'fragmentSize') {
	foreach $val (15, 16, 17, 511, 512, 513, 1023, 1024, 1025,
		      8191, 8192, 8193, 16383, 16384, 16385) {
	    &StartTest();
	    &SetAggr($field, $val);
	    &EndTest();
	}
    }
}

if (&FindObject(*GetAggr, 1, *GetAVLAnode, 1)) {
    &BashFields(*SetAnode, &efs'AnodeGetFormat($object));
}

if (&FindObject(*GetAggr, 1, *GetAVLAnode, 1, *GetFileSet, 1)) {
    &BashFields(*SetFileSet, @efs'FS_HEADER_FORMAT);
}

if (&FindObject(*GetAggr, 1, *GetAVLAnode, 1, *GetFileSet, 1, *GetFSAnode, 1)){
    &BashFields(*SetAnode, &efs'AnodeGetFormat($object));
}

if (&FindObject(*GetAggr, 1, *GetAVLAnode, 1, *GetFileSet, 1, *GetFSAnode, 1,
		*GetDirPage, 1)){
    &BashFields(*SetDirPage, @efs'DIR_HEADER_FORMAT);
}

if (&FindObject(*GetAggr, 1, *GetAVLAnode, 1, *GetFileSet, 1, *GetFSAnode, 1,
		*GetDirPage, 1, GetDirEntry, 1)){
    &BashFields(*SetDirEntry, &efs'DirEntryGetFormat($object));
}

# Use assignment to @junk to force an array context.  For some reason,
# the test fails otherwise.
if (@junk = &FindObject(*GetAggr, 1, *GetAVLAnode, 1, *GetBitmapPage, 1)) {
    &BashFields(*SetBitmapPage, @efs'BITMAP_PAGE_FORMAT);
}

if (&FindObject(*GetAggr, 1, *GetAVLAnode, 1, *GetIndirect, 1)) {
    &BashFields(*SetIndirect, @efs'INDIRECT_HEADER_FORMAT);
}


system "rm -f ${TMP_AGGR}";
