#!/usr/local/bin/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: basic_test.pl,v $
# Revision 1.1.8.1  1996/10/17  18:35:20  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:32  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:30  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:15:54  devsrc]
# 
# 	moving from file/episode to test/file
# 	[1994/03/22  21:02:40  delgado]
# 
# 	Embedding copyright notices
# 	[1993/01/19  14:13:34  zeliff]
# 
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
