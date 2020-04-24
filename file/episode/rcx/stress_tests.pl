#! /afs/tr/fs/dev/perl

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: stress_tests.pl,v $
# Revision 1.1.26.1  1996/10/02  17:27:13  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:33:52  damon]
#
# Revision 1.1.19.2  1994/06/09  14:02:55  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:37:57  annie]
# 
# Revision 1.1.19.1  1994/02/04  20:14:46  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:06  devsrc]
# 
# Revision 1.1.17.2  1993/12/07  20:32:20  jaffe
# 	Fixed Bad Comment leaders
# 	[1993/12/07  20:31:16  jaffe]
# 
# Revision 1.1.17.1  1993/12/07  17:20:26  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.1.2  1993/12/02  20:35:12  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.2.3  1993/01/19  15:32:43  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:54:40  cjd]
# 
# Revision 1.1.2.2  1992/10/27  20:53:01  jaffe
# 	New file
# 	[1992/10/27  13:05:24  jaffe]
# 
# $EndLog$

# stress_tests -- Wrapper for runrcx
#
# $Header: /u0/rcs_trees/dce/rcs/file/episode/rcx/stress_tests.pl,v 1.1.26.1 1996/10/02 17:27:13 damon Exp $
# Copyright (C) 1992 Transarc Corporation - All rights reserved
#
# Before running this, optionally set EPIBIN to the location of your EPISODE
# executables.

$ENV{'RCX_NCYCLES'} = 10 if !$ENV{'RCX_NCYCLES'};

@ALLTESTS = (<*.rs>);

@sizes = (1024, 1024,
		  2048, 1024,
		  2048, 2048,
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
         16384,16384,
	 	 32768, 1024,
	 	 32768, 2048,
         32768, 4096,
         32768, 8192,
         32768,16384,
	 	 32768,32768,
	 	 65536, 1024,
	 	 65536, 2048,
         65536, 4096,
         65536, 8192,
         65536,16384,
	 	 65536,32768,
	 	 65536,65536
);

while(@sizes) {
	local($blocksize) = $ENV{'RCX_BLOCKSIZE'} = shift(@sizes);
	local($fragsize) = $ENV{'RCX_FRAGSIZE'} = shift(@sizes); 
	&runtest('basictest', "(blocksize $blocksize, fragsize $fragsize)");
	&runtest('bigsimple', "(blocksize $blocksize, fragsize $fragsize)");
}

print("stress_tests complete\n");

sub runtest {
	local($test, $comment) = @_;
	local($code);
	print("stress_tests: Running test $test $comment\n");
	$code = system("runrcx $test");
	die("runrcx $test failed, with code $code") if $code;
}


# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
