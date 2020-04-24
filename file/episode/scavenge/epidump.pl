
# The above line intentionally left blank.

# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: epidump.pl,v $
# Revision 1.1.13.1  1996/10/02  17:28:28  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:28  damon]
#
# $EndLog$
#

# Copyright (C) 1994, 1991 Transarc Corporation - All rights reserved

# $Header: /u0/rcs_trees/dce/rcs/file/episode/scavenge/epidump.pl,v 1.1.13.1 1996/10/02 17:28:28 damon Exp $

eval "exec perl -S $0 $*"
	if ($running_under_some_shell);

($base,$prog) = ($0 =~ m=(/.*)/([^/]+)$=);
unshift (@INC, $base) if ($base);	# prepend referencing dir to path

sub Usage
{
    print STDERR "Usage is: epidump [-aggrinfo] [{-volume|-fileset} <name or id>] <aggr>\n";
    print STDERR "   or     epidump -bitmap [-dump] <aggr>\n";
    print STDERR "   or     epidump -anode <vol index> <anode index> <aggr>\n";
    print STDERR "   or     aggrinfo [{-volume|-fileset} <name or id>] [-dump] <aggr>\n";
    die ("Incorrect usage");
}

if ($prog eq "aggrinfo") {
    $show_aggrinfo = 1;
} else {
    $show_all = 1;
}
for ($i=0; $i<@ARGV; ) {
    $arg = $ARGV[$i];
    if (&ArgOK($arg,"-bitmap")) {
	$show_bitmap = 1;
	$show_all = 0;
    } elsif (&ArgOK($arg,"-aggrinfo")) {
	$show_aggrinfo = 1;
	$show_all = 0;
    } elsif (&ArgOK($arg,"-anode")) {
	$voli = $ARGV[$i+1];
	$filei = $ARGV[$i+2];
	splice(@ARGV, $i+1, 2);
	$show_all = 0;
    } elsif (&ArgOK($arg,"-volume") || &ArgOK($arg,"-fileset")) {
	$volume = $ARGV[$i+1];
	splice(@ARGV, $i+1, 1);
    } elsif (&ArgOK($arg,"-dump")) {
	&Usage if ($filei);
	$show_all = 1;
	$show_aggrinfo = 0;
    } else {
	(-e $arg) || &Usage;		# aggr name
	(-r $arg) || die ("Must be able to read device: $arg");
	$i++;
	next;
    }
    splice(@ARGV, $i, 1);
}
(@ARGV == 1) || &Usage;

sub ArgOK
{
    local ($arg, $val) = @_;
    (substr($val, 0, length($arg)) eq $arg);
}

require 'efs_query';

select(STDOUT);

if ($show_bitmap) {
    &Flush(*GetAggr, 1,		*GetAVLAnode, 1,
	   *GetBitmapPage, 1,	*PrintBitmapPage, 0);
}
if ($voli && $filei) {
    ($f, $foff) = &efs'AnodeGet(*AGGR, $voli, $efs'AVL_anode);#'
    ($a, $aoff) = &efs'AnodeGet(*AGGR, $filei, $f);
    %anode = &util'UnpackAssoc($a, &efs'AnodeGetFormat($a));
    print "\nAnode at offset $aoff\n";
    &efs'AnodePrint($a);
}
if ($show_aggrinfo) {
    &NoPrint(*efs'AGGR_HEADER_FORMAT,'magic');
    &NoPrint(*efs'AGGR_HEADER_FORMAT,'flags');
    &NoPrint(*efs'AGGR_HEADER_FORMAT,'reservedIndexes');
    &NoPrint(*efs'AGGR_HEADER_FORMAT,'bitmapIndex');
    &NoPrint(*efs'AGGR_HEADER_FORMAT,'logIndex');
    &NoPrint(*efs'AGGR_HEADER_FORMAT,'badBlocksIndex');
    &NoPrint(*efs'AGGR_HEADER_FORMAT,'creatingProgram');
    &NoPrint(*efs'AGGR_HEADER_FORMAT,'diskFormatMinorVersion');

    &NoPrint(*efs'FS_HEADER_FORMAT,'magic');
    &NoPrint(*efs'FS_HEADER_FORMAT,'accessTime');
    &NoPrint(*efs'FS_HEADER_FORMAT,'backingVolIndex');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[1]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[2]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[3]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[4]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[5]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[6]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[7]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[8]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[9]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[10]');
    &NoPrint(*efs'FS_HEADER_FORMAT,'fileStatus[11]');
    &Flush(*GetAggr, 1,	*PrintAggr, 1,
	   *GetAVLAnode, 1,
	   *GetFileSet, q/!$main'volume || ($fs{'ident.name'} eq $main'volume) || ($fs{'ident.id2'} eq $main'volume)/,
	   *PrintFileSet, 0);
}
if ($show_all) {
    &Flush(*GetAggr, 1,	*PrintAggr, 1,
	   *GetAVLAnode, 1,	*PrintAnode, 1,
	   *GetFileSet, q/!$main'volume || ($fs{'ident.name'} eq $main'volume) || ($fs{'ident.id2'} eq $main'volume)/,
	       *PrintFileSet, 1,
	   *GetFSAnode, 1,	*PrintAnode, 1,
	   *GetDirPage, 1,	*PrintDirPage, 1,
	   *GetDirEntry, 1,	*PrintDirEntry, 0);
}

sub NoPrint
{
    local (*array, $field) = @_;
    local ($i, $found);
    for ($i=0; $i<@array; $i+=3) {
	if ($array[$i] eq $field) {
	    $array[$i] =~ s/^/!/;
	    $found++;
	    last;
	}
    }
    $found || die("Unknown field $field specified");
}
