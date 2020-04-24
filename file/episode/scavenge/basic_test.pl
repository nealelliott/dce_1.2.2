#!/usr/local/bin/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: basic_test.pl,v $
# Revision 1.1.11.1  1996/10/02  17:28:14  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:22  damon]
#
# Revision 1.1.6.1  1994/06/09  14:03:24  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:38:27  annie]
# 
# Revision 1.1.2.3  1993/01/21  18:23:43  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:13:34  zeliff]
# 
# Revision 1.1.2.2  1992/09/25  16:27:54  jaffe
# 	Transarc delta: jdp-ot5076-lfs-add-basic_test.pl 1.1
# 	  Selected comments:
# 	    Add basic_test.pl--a place for putting general scavenger tests.
# 	    This file is a place for putting general scavenger tests.  Its first test
# 	    bashes the bOffset field of a COW anode.
# 	[1992/09/23  17:50:03  jaffe]
# 
# $EndLog$
#

# Copyright (C) 1992 Transarc Corporation - All rights reserved

require 'efs_query';


#
# Find a COW anode and bash its 'bOffset' field
#
if (&FindObject(*GetAggr, 1, *GetAVLAnode, 1, *GetFileSet, 1,
		*GetFSAnode,  q/$anode{'bOffset'} != 0/)) {
    &StartTest();
    &SetAnode('bOffset', 515);
    &EndTest();
}


&Cleanup();
