#!/afs/tr/fs/dev/perl

# /* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/icl/RCS/unadb.pl,v 1.1 1994/08/15 13:23:36 ota Exp $

# unadb -- creates a binary file from adb output.  It also takes some options
#     for unwinding a circular ICL log.
#
#     It always writes its output to /tmp/unadb.data.  This should be fixed.

if ((@ARGV > 0) && ($ARGV[0] =~ /^-h/)) {
    print "Usage: $0 <log base> <words in log> <1st word in log>\n";
    exit (1);
}

$base = eval($ARGV[0]) || -1;
$size = eval($ARGV[1]) || 2**20;	# 1M word default
$first = eval($ARGV[2]);

while (<STDIN>) {
    ($addr,@words) = /^0x([\da-f]+):\s+([\da-f]+)\s+([\da-f]+)\s+([\da-f]+)\s+([\da-f]+)/i;
    if ($addr) {
	$point = eval("0x$addr");
	if ($base < 0) {
	    $base = $point;
	    printf "Using %#x as base address\n", $base;
	}
    } else {
	(@words) = /^\s+([\da-f]+)\s+([\da-f]+)\s+([\da-f]+)\s+([\da-f]+)/i;
    }
    next if (@words != 4);
    for ($i=0; $i<@words; $i++) {
	$words[$i] = eval("0x$words[$i]");
    }
    $line = pack ("I4",@words);
    $off = $point - $base;
    print "Bogus base? $base and $point differ by $off\n"
	if ($off < 0 || $off > 32*1024*1024);
    substr($buffer, $off, 16) = $line;
    $point += 4*4;
}
open (BIN, ">/tmp/unadb.data");
if ($first) {
    # unwrap log
    print BIN substr($buffer,$first*4);
    print BIN substr($buffer,0,$first*4);
} else {
    print BIN $buffer;
}
close BIN;
