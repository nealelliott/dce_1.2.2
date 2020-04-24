#! /afs/tr/fs/dev/perl
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# Copyright (C) 1993 Transarc Corporation - All rights reserved
#
# HISTORY
# $Log: 8k_1ktests.pl,v $
# Revision 1.1.20.1  1996/10/02  17:26:24  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:18  damon]
#
# Revision 1.1.13.3  1994/10/06  20:48:59  agd
# 	expand copyright
# 
# 	8k_1k -- Wrapper for runrcx
# 	[1994/10/06  17:34:58  agd]
# 
# $EndLog$

$ENV{'RCX_NCYCLES'} = 100 if !$ENV{'RCX_NCYCLES'};
$ENV{'RCX_NTHREADS'} = 10 if !$ENV{'RCX_NTHREADS'};

local($blocksize) = $ENV{'RCX_BLOCKSIZE'} = 8192;
local($fragsize) = $ENV{'RCX_FRAGSIZE'} = 1024;
&runtest('basictest', "(blocksize $blocksize, fragsize $fragsize)");
&runtest('bigsimple', "(blocksize $blocksize, fragsize $fragsize)");

print("8k_1ktests complete\n");

sub runtest {
	local($test, $comment) = @_;
	local($code);
	print("all_tests: Running test $test $comment\n");
	$code = system("runrcx $test");
	die("runrcx $test failed, with code $code") if $code;
}


# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
