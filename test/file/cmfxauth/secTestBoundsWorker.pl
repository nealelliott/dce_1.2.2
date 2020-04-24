#!/afs/tr/proj/tools/bin/perl

# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Protection-bounds Test Worker
#
#   This is a simple worker that the protection-bounds tests can fork off
#   with the desired local/DCE credentials.  It just reads the specified file
#   and reports the results.
#
#   Exit value: 0 - read successful; > 0 - read failed with returned value
#

# main(filename)
$| = 1;

$0 =~ m#([^/]+)$#;
$myExec = $1;

if ($ENV{DFSXSECTEST_SRCDIR}) {
    unshift(@INC, "$ENV{DFSXSECTEST_SRCDIR}");
} else {
    unshift(@INC, ".");
}

require("secconfig.pl");

# check arguments

if (@ARGV != 1) {
    print "$myExec: Incorrect argument count; error in test script?\n";
    exit(1);
}

$filename = $ARGV[0];

# attempt to read file

system("$CAT $filename > /dev/null");

exit($? / 256);
