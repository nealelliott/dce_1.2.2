#!/afs/transarc.com/fs/dev/perl
#/* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/kill-test_anode.pl,v 1.2 1994/03/21 19:18:11 ota Exp $

# kill-test_anode -- runs in the background and kills test_anode by sending
#     it a SIGHUP.  It takes an argument which is the test_anode script prefix
#     to check for.  It contains several built-in constants controlling how
#     long to let the script run before killing it and how long to sleep before
#     checking again.

$me = $ENV{USER};
$scriptName = "nbs";
$scriptName = shift
    if (@ARGV);

# number of consecutive times we didn't see a test_anode of the right type.
$misses = 0;

local ($time, $min, $sec);
local ($user, $pid);

$|=1;
while ($misses++ < 10) {
    $sleep = 5;
    open (PS,"ps auxw|");
    while (<PS>) {
	if (/kill-test_anode/) {
	    ($user, $pid) = split;
	    if (($pid < $$) && ($user eq $me)) {
		print "Another kill-test_anode is running; Exiting.\n";
		exit 0;
	    }
	    next;
	}
	next unless (($min, $sec)
		     = m=(\d+):(\d\d) (\./)?test_anode $scriptName=);
	$misses = 0;
	($user, $pid) = split;
	$time = $min * 60 + $sec;
	next unless (($user eq $me) && ($time > 5));
	chop;
	print "Killing $pid with line '$_'\n";
	kill ("SIGHUP",$pid);
	$sleep = int(rand(30));
	last;
    }
    close (PS);
    sleep $sleep;
}
print "Exiting kill-test_anode.\n";

