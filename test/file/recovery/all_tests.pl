#! /afs/tr/fs/dev/perl
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: all_tests.pl,v $
# Revision 1.1.8.1  1996/10/17  18:33:54  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:50  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:39  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:15:00  devsrc]
# 
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:16  delgado]
# 
# 	delta bob-add-rcx 1.1
# 	[1992/05/04  19:33:01  jdp]
# 
# Revision 1.1.19.1  1994/02/04  20:14:30  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:58  devsrc]
# 
# Revision 1.1.17.2  1993/12/07  20:32:16  jaffe
# 	Fixed Bad Comment leaders
# 	[1993/12/07  20:31:08  jaffe]
# 
# Revision 1.1.17.1  1993/12/07  17:20:08  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.1.2  1993/12/02  20:33:00  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.4.4  1993/01/19  15:31:38  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:53:08  cjd]
# 
# Revision 1.1.4.3  1992/10/28  21:43:07  jaffe
# 	Fixed RCS id
# 	[1992/10/28  21:13:05  jaffe]
# 
# Revision 1.1.4.2  1992/10/27  20:52:34  jaffe
# 	Transarc delta: rajesh-ot5391-errors-in-runrcx 1.6
# 	  Selected comments:
# 	    Fix bugs in runrcx script.
# 	    Further updates
# 	    A checkpoint.
# 	    Running version
# 	    Make a portion of code more readable.
# 	    Minor cleanup to all_tests and stress_tests
# 	    See above
# 	[1992/10/27  13:00:48  jaffe]
# 
# $EndLog$

# all_tests -- Wrapper for runrcx
#
# $Header: /u0/rcs_trees/dce/rcs/test/file/recovery/all_tests.pl,v 1.1.8.1 1996/10/17 18:33:54 damon Exp $
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

if ( ! ($SYSTYPE =~ /OSF/i) ) {

	@sizes = (
#			  1024, 1024,
#			  2048, 1024,
#			  2048, 2048,
			  4096, 1024,
			  4096, 2048,
			  4096, 4096,
			  8192, 1024,
			  8192, 2048,
	     	  8192, 4096,
			  8192, 8192,
			 16384, 1024,
			 16384, 2048,
	         16384, 4096,
	         16384, 8192,
	         16384,16384);
	while(@sizes) {
	 	local($blocksize) = $ENV{'RCX_BLOCKSIZE'} = shift(@sizes);
		local($fragsize) = $ENV{'RCX_FRAGSIZE'} = shift(@sizes); 
		&runtest('basictest', "(blocksize $blocksize, fragsize $fragsize)");
		&runtest('bigsimple', "(blocksize $blocksize, fragsize $fragsize)");
	}
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
