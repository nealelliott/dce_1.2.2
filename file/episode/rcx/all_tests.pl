#! /afs/tr/fs/dev/perl
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: all_tests.pl,v $
# Revision 1.1.26.1  1996/10/02  17:26:33  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:22  damon]
#
# $EndLog$

# all_tests -- Wrapper for runrcx
#
# $Header: /u0/rcs_trees/dce/rcs/file/episode/rcx/all_tests.pl,v 1.1.26.1 1996/10/02 17:26:33 damon Exp $
# Copyright (C) 1992 Transarc Corporation - All rights reserved
#
# Before running this, optionally set EPIBIN to the location of your EPISODE
# executables.

# Determine system type
$SYSTYPE=`uname`;
chop $SYSTYPE;

if ( $SYSTYPE =~ /OSF/i ) {
	$ENV{'RCX_NCYCLES'} = 5;
	$ENV{'RCX_NTHREADS'} = 2;
} else {
	$ENV{'RCX_NCYCLES'} = 10 if !$ENV{'RCX_NCYCLES'};
	$ENV{'RCX_NTHREADS'} = 10 if !$ENV{'RCX_NTHREADS'};
}

@ALLTESTS = (<*.rs>);

if ($SYSTYPE =~ /SunOS/i) {
	@sizes = (8192, 1024);
	print("Only 8K/1K supported on Sun");
} elsif (!($SYSTYPE =~ /OSF/i)) {
	@sizes = (
			  8192, 1024,
			  4096, 4096,
#			  1024, 1024,
#			  2048, 1024,
#			  2048, 2048,
			  4096, 1024,
			  4096, 2048,
			  8192, 2048,
	     	  8192, 4096,
			  8192, 8192,
			 16384, 1024,
			 16384, 2048,
	         16384, 4096,
	         16384, 8192,
	         16384,16384);
}

while(@sizes) {
	local($blocksize) = $ENV{'RCX_BLOCKSIZE'} = shift(@sizes);
	local($fragsize) = $ENV{'RCX_FRAGSIZE'} = shift(@sizes); 
	&runtest('basictest', "(blocksize $blocksize, fragsize $fragsize)");
	&runtest('bigsimple', "(blocksize $blocksize, fragsize $fragsize)");
}

print("all_tests complete\n");

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
