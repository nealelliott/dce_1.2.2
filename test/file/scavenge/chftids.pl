#!/usr/local/bin/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: chftids.pl,v $
# Revision 1.1.8.1  1996/10/17  18:35:22  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:34  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:31  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:17:01  devsrc]
# 
# 	moving from file/episode to test/file
# 	[1994/03/22  21:02:41  delgado]
# 
# 	Transarc delta: rajesh-db3161-fix-chftids-script 1.1
# 	  Selected comments:
# 	    Even though I am exporting this delta, the work was done by jdp and blake.
# 	    (Thanks).
# 	    Changes in the scavenge perl scripts efs, efs_query broke chftids.pl.
# 	[1993/03/09  17:17:05  jaffe]
# 
# 	Transarc delta: jdp-ot5924-add-chftids.pl-to-scavenge-dir 1.1
# 	  Selected comments:
# 	    This delta adds a scavenge tool that changes the fileset ID for each fileset
# 	    in an aggregate.  Both the fileset anode and the fileset header are modified.
# 	    This tool will be used with RCX to allow fileset IDs to be changed before
# 	    mounting a copy of a fileset while the original is still mounted.
# 	    This script is a hack that changes each fileset's ID by setting the second
# 	    highest bit (don't ask) to 1.
# 	[1993/01/12  20:24:32  shl]
# 
# Revision 1.1.6.1  1994/02/04  20:15:22  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:23  devsrc]
# 
# Revision 1.1.4.1  1994/01/28  20:43:41  annie
# 	expand OSF copyright
# 	[1994/01/28  20:42:16  annie]
# 
# 	expand OSF copyright
# 
# $EndLog$
#

# Copyright (C) 1992 Transarc Corporation - All rights reserved

require 'efs_query';

close(AGGR) || &Die("$AGGR: close failed: $!");
open(AGGR, "+<$AGGR") || &Die("${AGGR}: open(2) failed");

&FindObject(*GetAggr, 1,
	    *GetAVLAnode,
	    q/$type == $efs'CFLAGS_VOLUME &&
              &ChangeId(@fs_anode, $block_size, $frag_size) &&
              0/);

sub ChangeId {
    local($fs_anode, $fs_anode_offset, $block_size, $frag_size) = @_;
    local(%fs_anode_map) = &efs'AnodeGetMap($fs_anode);
    local($low_id) = &util'MapGet($fs_anode, 'ident.id2', %fs_anode_map);
    local($new_low_id) = $low_id | (1 << 30);
    local($fs, $fs_offset) = &efs'FSGet(*AGGR, $fs_anode);
    local($fs_low_id) = &util'MapGet($fs, 'ident.id2', %efs'FS_HEADER_MAP);

    print "Found a fileset anode (id $low_id) at offset $fs_anode_offset:\n";
    &efs'AnodePrint($fs_anode);
    print "\nFound a fileset (id $fs_low_id) at offset $fs_offset:\n";
    &efs'FSPrint($fs);

    if ($low_id != $fs_low_id) {
	warn("AVL anode and fileset header have different IDs");
    }

    # Set id in the AVL anode
    &Set($fs_anode, 'ident.id2', $new_low_id, $fs_anode_offset, %fs_anode_map);

    # Set id in the fileset header
    &Set($fs, 'ident.id2', $new_low_id, $fs_offset, %efs'FS_HEADER_MAP);
}

