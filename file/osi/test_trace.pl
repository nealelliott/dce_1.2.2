#!/afs/tr/proj/tools/bin/perl
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: test_trace.pl,v $
# Revision 1.1.9.1  1996/10/02  18:12:07  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:45:21  damon]
#
# Revision 1.1.4.1  1994/06/09  14:17:29  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:29:38  annie]
# 
# Revision 1.1.2.3  1993/01/21  14:53:38  cjd
# 	embedded copyright notice
# 	[1993/01/20  14:57:32  cjd]
# 
# Revision 1.1.2.2  1992/12/09  20:34:36  jaffe
# 	Transarc delta: jaffe-add-missing-osf-stuff 1.1
# 	  Selected comments:
# 	    Add osf copyrights to files that don't have them
# 	[1992/12/09  18:40:51  jaffe]
# 
# 	Transarc delta: ota-ot6058-rcsid-parse-too-fragile 1.1
# 	  Selected comments:
# 	    The recent change in CML organization confused the heck out of the RCSID
# 	    parsing code.  I've changed it to be much more robust to future
# 	    confusion.  Also the built in package names were wrong so updated them.
# 	    I tried the run the test_trace program to see if I had broken anything
# 	    but that routine was badly written.  I augmented it with a perl script
# 	    which verifies that the test_trace program's output is acceptable.
# 	    This perl script runs test_trace and checks all the output returned
# 	    against the expected value.  The result is a zero or non-zero exit
# 	    status.
# 	[1992/12/04  17:55:52  jaffe]
# 
# $EndLog$
#
# Copyright (C) 1992 Transarc Corporation
# Licensed Materials - Property of Transarc
# All rights reserved.
#

$test_trace = $ENV{TEST_TRACE};
if (!$test_trace) {
    if ($ENV{SOURCEBASE} && $ENV{OBJECTDIR}) {
	$test_trace = "$ENV{SOURCEBASE}/$ENV{OBJECTDIR}/file/osi/test_trace";
    } else {
	$test_trace = "test_trace";
    }
}

$lastline = 0;				# expect higher line# next
$match = 0;				# expect this line# next

sub expect
{
    local ($string) = @_;
    local ($thisline);

    ($_ = <trace>) || die "Premature EOF";
    (($anyline, $thisline) = m"()$string") ||
	die "Match failed expecting '$string' but got '$_'";
    if ($thisline) {
	if ($thisline =~ m/^at ([0-9]+)$/) {
	    $match = $1;
	} else {
	    (($match == $thisline) ||
	     (!$match && ($thisline > $lastline))) ||
		 die "Line# not sequential; expected $thisline > $lastline";
	    $lastline = $thisline;
	    $match = 0;
	}
    }
}
    
open (trace, "$test_trace 2>&1 |") ||
    die "Can't run test program from '$test_trace'";

$revpat = "[0-9]+\\.[0-9]+";		# a revision number
$linepat = "([0-9]+)";			# a monotonically increasing line#
$atpat = "(at [0-9]+)";			# require specific line# next

&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat\\) trace_test:1: always : 0x4d2 1234 02322 \"string\"");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat sleep\\) trace_test:2: global enabledTypes is -1");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat sleep\\) trace_test:3: global enabledTypes is 4");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat\\) trace_test:4: unusual: one=1, str=string");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat sleep\\) trace_test:5: osi enabledTypes is 4 \\(10/3=3\\.33333>3\\.0\\)");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat sleep\\) trace_test:6: enabledFiles is 1 << 5-1");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat sleep\\) trace_test:7: enabledFiles is -1");
&expect("shouldn't assert at $linepat");
&expect("assert $atpat");
&expect("Assertion failed: i == 4, file  \\.\\./\\.\\./\\.\\./\\.\\./src/file/osi/test_trace\\.c, line $linepat");
&expect("sh: [0-9]+ Abort");
&expect("shouldn't assert at $linepat");
&expect("assert $atpat");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat assertion failed\\) assert failed");
&expect("sh: [0-9]+ Abort");
&expect("shouldn't assert at $linepat");
&expect("assert $atpat");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat assertion failed\\) i\\(5\\) not 4");
&expect("sh: [0-9]+ Abort");
&expect("assert $atpat");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat assertion failed\\) assert failed");
&expect("sh: [0-9]+ Abort");
&expect("assert $atpat");
&expect("\\(file/osi/test_trace\\.c rev\\. $revpat #$linepat assertion failed\\) i\\(5\\) not 4");
&expect("sh: [0-9]+ Abort");
&expect("shouldn't assert at $linepat");
&expect("shouldn't assert at $linepat");

!($_ = <trace>) || die "Expecting EOF got $_";

exit (0);
