#!/usr/local/bin/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: aggrinfo.pl,v $
# Revision 1.1.13.1  1996/10/02  17:28:08  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:21  damon]
#
# Revision 1.1.8.1  1994/06/09  14:03:23  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:38:25  annie]
# 
# Revision 1.1.4.4  1993/01/28  17:16:08  shl
# 	Transarc delta: ota-ot5926-AFS-quota 1.7
# 	  Selected comments:
# 	    Motivation:
# 	    This delta augments Episode to maintain two types of size
# 	    information about filesets and their constituent anodes.  Further,
# 	    independent limits are omposed for each size measures.  The two
# 	    measures are called allocated and visible.  The first matches the
# 	    old semantics and counts fragments actually allocated to the anode
# 	    or fileset.  The second (new) measure counts fragments visible in an
# 	    anode or fileset.  The principle difference appears when COW is
# 	    being used.  A freshly cloned anode will have zero allocated size
# 	    but visible size which is the same as that for its backing anode.
# 	    Similarly deleting a large file which was just clones will not
# 	    reduce the allocated usage of the containing fileset but will return
# 	    the expected amount of space to the visible usage, even if not
# 	    blocks were actually freed in the underlying aggregate.
# 	    Details of the implementation strategy are in:
# 	    ~ota/doc/episode/FilesetQuota.text.
# 	    Status:
# 	    This delta is restricted to Episode only changes.  The corresponding
# 	    changes in the fileset layers (including ftutil, ftserver, and the
# 	    fts command, not to mention the doc changes) are in other OTs and
# 	    deltas.
# 	    Display visible and allocated limit & usage.
# 	    Introduced a bug in recloning an already recloned fragmented fileset.
# 	    This was exposed by the fts tests.
# 	    Fix several problems that turned up when running the fts tests.
# 	    All_tests was failing on multi-block-frag.int.test.  The cause was a
# 	    confusion of the calling conventions of MoveUnblockedAnodes; it can be
# 	    called with the backing container BLOCKED, with the visible field not
# 	    yet updated.
# 	    A typo augmented by liberal cut-and-paste caused several volume quota
# 	    updates to be done with a transaction id of zero (and therefore not
# 	    logged).  This lead to errors after recovery.
# 	    Found several salvage problems while tracking down another problem.  But
# 	    since the afflicted code had been modified in this delta, I thought it
# 	    would be reduce the risk of merge conflicts to make the change as part
# 	    of this delta.
# 	    Changing the IsAnodeAllEmpty predicate to use the visible field is a
# 	    great efficiency win but the salvager gets really unhappy when it tries
# 	    to read in directories with a zero visible field (i.e. running on
# 	    -verify on an old aggregate).
# 	    For now, for upward compatibility, go back to the old scheme.
# 	[1993/01/27  18:11:21  shl]
# 
# Revision 1.1.4.3  1993/01/21  18:23:40  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:13:30  zeliff]
# 
# Revision 1.1.4.2  1992/09/25  16:27:48  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:50:45  jaffe]
# 
# Revision 1.1.2.2  1992/05/22  21:16:19  jdp
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
# 	[1992/05/22  20:26:52  jdp]
# 
# Revision 1.1.1.2  1992/05/22  20:26:52  jdp
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

@NEW_FS_HEADER_FORMAT = 
(
 '!magic', 'L', '%#x',
 'ident.id1', 'L', '%u',
 'ident.id2', 'L', '%u',
 'ident.name', "A$efs'FS_IDENT_NAME_SIZE", '"%s"',
 '!flags', 'L', '%#x',
 '!index', 'L', '%u',
 '!version1', 'L', '%u',
 '!version2', 'L', '%u',
 '!unique', 'L', '%u',
 '!createTime', 'L', '%u',
 '!modTime', 'L', '%u',
 '!accessTime', 'L', '%u',
 '!backingVolIndex', 'L', '%u',
 '!backingVolId1', 'L', '%u',
 '!backingVolId2', 'L', '%u',
 '!upLevelIds[0]1', 'L', '%u',
 '!upLevelIds[0]2', 'L', '%u',
 '!upLevelIds[1]1', 'L', '%u',
 '!upLevelIds[1]2', 'L', '%u',
 '!upLevelIds[2]1', 'L', '%u',
 '!upLevelIds[2]2', 'L', '%u',
 '!upLevelIds[3]1', 'L', '%u',
 '!upLevelIds[3]2', 'L', '%u',
 '!allocated.spare1', 'L', '%u',
 'allocated.limit', 'L', '%u',
 '!allocated.spare2', 'L', '%u',
 'allocated.usage', 'L', '%u',
 '!sparel[0]', 'L', '%u',
 '!sparel[1]', 'L', '%u',
 '!sparel[2]', 'L', '%u',
 '!sparel[3]', 'L', '%u',
 '!sparel[4]', 'L', '%u',
 '!visible.spare1', 'L', '%u',
 'visible.limit', 'L', '%u',
 '!visible.spare2', 'L', '%u',
 'visible.usage', 'L', '%u'
 );


select(STDOUT);

&Flush(*GetAggr, 1,	*PrintAggr, 1,
       *GetAVLAnode, 1,
       *GetFileSet, 1,	*NewPrintFileSet, 0);

sub NewPrintFileSet {
    local(*input, $expr, @next) = @_;
    local($fs, $offset) = @input[$#input-1, $#input];
	
    print "\nFile set at offset $offset\n";
    &util'FormatPrint($fs, @NEW_FS_HEADER_FORMAT);

    if (@next) {
        local(*next_func, $next_expr, @next_next) = @next;

        return &next_func(*input, $next_expr, @next_next);
    } else {
        if (eval $expr) {
            return @input;
        } else {
            return ();
        }
    }
}

