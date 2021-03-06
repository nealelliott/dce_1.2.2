#! /afs/tr/fs/dev/perl
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: all_simple_tests.pl,v $
# Revision 1.1.24.1  1996/10/02  17:26:31  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:21  damon]
#
# Revision 1.1.17.2  1994/06/09  14:02:14  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:19  annie]
# 
# Revision 1.1.17.1  1994/02/04  20:14:28  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:57  devsrc]
# 
# Revision 1.1.15.2  1993/12/07  20:32:15  jaffe
# 	Fixed Bad Comment leaders
# 	[1993/12/07  20:31:07  jaffe]
# 
# Revision 1.1.15.1  1993/12/07  17:20:04  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.1.2  1993/12/02  20:32:52  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.2.2  1993/03/09  18:52:50  jaffe
# 	Transarc delta: rajesh-ot7277-rcx-and-symlinks 1.1
# 	  Selected comments:
# 	    The delta name is somewhat of a misnomer. It includes several fixes including
# 	    the fix in the rcx script for symlink target names.
# 	    This delta has undergone testing (all_simple_tests wrapper) and seem ok.
# 	    A new wrapper that runs all the rcx tests with the lightest of loads
# 	    by default so that you can get a zeroth order confidence in the tests.
# 	    Default NTHREADS=1 and default NCYCLES=1 with blocksize=8192 and
# 	    fragsize=1024.
# 	[1993/03/09  17:07:16  jaffe]
# 
# $EndLog$

# all_simple_tests -- Wrapper for runrcx
#
# $Header: /u0/rcs_trees/dce/rcs/file/episode/rcx/all_simple_tests.pl,v 1.1.24.1 1996/10/02 17:26:31 damon Exp $
# Copyright (C) 1992,1993 Transarc Corporation - All rights reserved
#

#
# Before running this, optionally set EPIBIN to the location of your EPISODE
# executables.

$ENV{'RCX_NCYCLES'} = 1 if !$ENV{'RCX_NCYCLES'};
$ENV{'RCX_NTHREADS'} =1 if !$ENV{'RCX_NTHREADS'};

@ALLTESTS = (<s*.rs>);

$ENV{'RCX_BLOCKSIZE'} = 8192;
$ENV{'RCX_FRAGSIZE'} = 1024;
#&runtest('basictest', "(blocksize $blocksize, fragsize $fragsize)");
for (@ALLTESTS) { 
	s/.rs$//;
	&runtest($_, '');
}

print("all_simple_tests complete\n");

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
