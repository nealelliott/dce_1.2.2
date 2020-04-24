#! /afs/tr/fs/dev/perl
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# HISTORY
# $Log: 8k_1ktests.pl,v $
# Revision 1.1.8.1  1996/10/17  18:33:44  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:45  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:27  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:14:56  devsrc]
# 
# 	moving from file/episode to test/file
# 	[1994/03/22  20:51:34  delgado]
# 
# $EndLog$
#
# Copyright (C) 1993 Transarc Corporation - All rights reserved

# 8k_1k -- Wrapper for runrcx
#

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
