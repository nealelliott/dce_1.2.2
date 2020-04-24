#!/afs/tr/fs/dev/perl
#
# Copyright (C) 1993 Transarc Corporation - All rights reserved
#
# Salvager test suite
#

##
## Basic utility and aggregate-munching routines:
##

#
# XXX The module initialization code currenty does more than it should,
# for instance, pulling the aggregate name from @ARGV and opening it.
# Because of this, we have to be careful about the order in which we
# do things here.  It would be nicer to move the initialization code
# into subroutines to be called from the main program, but this would
# entail rewriting existing users of efs_query et al.
#
unshift(@INC, '.');
require 'util';
&util'UnpackAllFields(1);

##
## Configuration parameters
##

#
# These describe the structure of the aggregate that we will test.
# We could probably devise a nicer way to specify this information.
#
$backing_FS_name = 'abelard';
$cow_FS_name = 'heloise';
$file_dir = 'files';
$dev_dir = 'devices';
$big_dir = 'bigdir';
@test_file = (
    'zero', 'inline', 'frags', 'frags.2',
    'dblocks', 'iblocks', 'sparse'
);

@test_dev = ('black_hole', 'pied_piper', 'piltdown');

#
# The types of tests and the number of tests of each type.  The -run argument
# allows the user to select a subset of these.
#
# Note that all test function names must have the form "test_$type$num" for
# this scheme to work correctly.
#
@test_categories = (
    'anode', 10,
    'blk', 3,
    'frag', 3,
    'cow', 7,
    'sblk', 12,
    'AVL', 5,
    'bmap', 9,
    'bb', 3,
    'log', 3,
    'ablk', 5,
    'vol', 22,
    'file', 12,
    'misc', 3,
    'aux', 6,
    'dir', 17
);

&ProcessArguments;
require 'efs_query';

#
# Log file in which to place summary of results
#
$LOG_FILE = "salt_log.$$";

#
# Names for anode types and storage methods.
#
%typename = (
	$efs'AFLAGS_TYPE_AVL,		'AVL',
	$efs'AFLAGS_TYPE_VOLUME,	'fileset',
	$efs'AFLAGS_TYPE_BITMAP,	'bit map',
	$efs'AFLAGS_TYPE_LOG,		'log',
	$efs'AFLAGS_TYPE_BADBLOCKS,	'bad block container',
	$efs'AFLAGS_TYPE_REGULAR,	'regular anode'
);

%bitname = (
	$efs'AFLAGS_DATA_INLINE,	'inline',
	$efs'AFLAGS_DATA_FRAGMENTED,	'fragmented',
	0,				'blocked',
	$efs'AFLAGS_DATA_NOTBLOCKED,	'illegal bit combination'
);

##
## The various tests
##
require 'anode';
require 'aggr';
require 'ablk';
require 'volume';
require 'file';
require 'direct';

##
## Setup and utility functions
##

#
# Process leading arguments beginning with '-'; we recognized the following:
#
# -run <category>{<regular_expression>}{,<category>{regular_expression>}}
# -aggregate <name>
#
# The argument to -run must be the name of a category of tests, optionally
# followed by a regular expression specifying which of the tests to run
# (e.g. 'anode[7-9]').
#
# -aggregate is not strictly necessary, but allows clearer identification of
# the aggregate name.
#
sub ProcessArguments {
    local($opt);
    local($aggr_name);

    @test_patterns = ();
    while ($ARGV[0] =~ /^-/) {
	$opt = shift(@ARGV);
	if ($opt eq '-run') {
	    $opt = &get_arg;
	    $opt =~ s/\s//g;
	    push(@test_patterns, split(/,/, $opt));
	    next;
	}
	if ($opt eq '-aggregate') {
	    $aggr_name = &get_arg;
	    next;
	}
	&Usage;
    }
    unshift(@ARGV, $aggr_name) if $aggr_name;
}

sub get_arg {
    &Usage unless (@ARGV);
    return shift(@ARGV);
}

sub catlist {
    local($i);
    local($cats) = '';
    for ($i = 0; $i < $#test_categories; $i += 2) {
	$cats .= ',' unless ($i == 0);
	$cats .= $test_categories[$i];
    }
    return $cats;
}

sub Usage {
    local($cats) = &catlist;
    print "usage: $0 {-run $cats} [-aggregate] aggr_name\n";
    die("exiting");
}

#
# Print to log file and standard output.
#
sub logpr {
    printf @_;
    printf LOG @_;
}

#
# Log a salvager error.
#
sub salverr {
    local($name) = shift;
    local($index) = shift;

    &logpr("## SALVAGE ERROR: anode $index ($name): ");
    &logpr(@_);
    return 1;
}

#
# Find the fileset in which tests will be done and remember its
# AVL index to speed subsequent lookups.
#
sub find_FS {
    local($fs_name) = @_;
    local(@result) = &FindObject(
	*GetAggr, 1,
	*GetAVLAnode,
	    q/$type == / . $efs'CFLAGS_VOLUME .
	    q/ && $fs_anode{'ident.name'} eq / . "'$fs_name'",
	*GetFileSet, 1);
    local($anode) = $result[$#result - 3];
    if (!$anode) {
	&logpr("## CONFIG error: can't find fileset $fs_name in aggregate.\n");
	&Die('Test aborted');
    }
    local($index) = &efs'GetAnodeMember($anode, 'index');
    $fs_info_map[$index] = 1 + $#fs_info;
    push(@fs_info, @result[$#result - 3 .. $#result]);
    return $index;
}

#
# Given fileset index, return (anode, anode offset, FS header, header offset)
#
sub get_fs_info {
    local($index) = @_;
    local($start) = $fs_info_map[$index];
    return @fs_info[$start .. $start + 3];
}

#
# To save time, collect all the anode information that we will need
# before starting the tests.  Each test runs on a fresh copy of the
# aggregate (which we assume doesn't change between tests), so this
# is O.K.  We store the information as a list of tuples of the form
# (anode_data, offset [, path_name]); path_name is omitted for special
# anodes.
#

#
# Find the special anodes for the aggregate and store in @special.
#
sub find_special_anodes {
    print $LONG_DELIMITER, "\n";
    print "## Finding special anodes for test use...\n";
    @special = ();
    &Flush(
	*GetAggr, 1,
	*GetAVLAnode, q/$type != / . $efs'CFLAGS_REGULAR,
	*PushObject, *special);
}

#
# Find the files that we will use for testing.
#
sub find_files {
    local($fs_name, $fs_index, *filelist) = @_;
    local(@fs_info) = &get_fs_info($fs_index);
    local($file, $path);
    local(@anode);
    local($errors) = 0;

    print $LONG_DELIMITER, "\n";
    print "## Finding files in fileset $fs_name for test use...\n";
    @filelist = ();
    foreach $file (@test_file) {
	$path = "$file_dir/$file";
	@anode = &GetFSAnodeFromPath(*fs_info, $path);
	if (@anode) {
	    local($anode, $offset) = @anode[$#anode - 1, $#anode];
	    push(@filelist, $anode, $offset, $path);
	} else {
	    &logpr(
"## CONFIG ERROR, find_files: file $path not found\n");
	    $errors++;
	}
    }
    &logpr("## WARNING: file setup failed.\n") if $errors;
}

#
# Find the directories.
#
sub find_directories {
    local($fs_name, $fs_index, *dirlist) = @_;
    local(@fs_info) = &get_fs_info($fs_index);
    local($dir);
    local(@anode);
    local($errors) = 0;

    print $LONG_DELIMITER, "\n";
    print "## Finding files in fileset $fs_name for test use...\n";
    @dirlist = ();
    foreach $dir ($file_dir, $dev_dir, $big_dir) {
	@anode = &GetFSAnodeFromPath(*fs_info, $dir);
	if (@anode) {
	    local($anode, $offset) = @anode[$#anode - 1, $#anode];
	    push(@dirlist, $anode, $offset, $dir);
	} else {
	    &logpr(
"## CONFIG ERROR, find_directories: directory $path not found\n");
	    $errors++;
	}
    }
    &logpr("## WARNING: directory setup failed.\n") if $errors;
}

#
# Find the device special files
#
sub find_devices {
    local($fs_name, $fs_index, *devlist) = @_;
    local(@fs_info) = &get_fs_info($fs_index);
    local($dev, $path);
    local(@anode);
    local($errors) = 0;

    print $LONG_DELIMITER, "\n";
    print "## Finding device files in fileset $fs_name for test use...\n";
    @dirlist = ();
    foreach $dev (@test_dev) {
	$path = "$dev_dir/$dev";
	@anode = &GetFSAnodeFromPath(*fs_info, $path);
	if (@anode) {
	    local($anode, $offset) = @anode[$#anode - 1, $#anode];
	    push(@devlist, $anode, $offset, $path);
	} else {
	    &logpr(
"## CONFIG ERROR, find_devices: device $path not found\n");
	    $errors++;
	}
    }
    &logpr("## WARNING: device file setup failed.\n") if $errors;
}


#
# Divide a list of regular anodes into lists of blocked, fragmented,
# inline, and zero-length anodes.
#
sub split_regular_anodes {
    local(*regular, *blocked, *fragmented, *inline, *zero) = @_;
    local($i, $anode, $flags, $kind);
    for ($i = 0; $i <= $#regular; $i += 3) {
	$anode = $regular[$i];
	$flags = &efs'GetAnodeMember($anode, 'flags');
	$kind = ($flags & $efs'AFLAGS_DATA_NOTBLOCKED);
	if ($kind == $efs'AFLAGS_DATA_INLINE) {
	    push(@inline, @regular[$i .. $i + 2]);
	} elsif ($kind == $efs'AFLAGS_DATA_FRAGMENTED) {
	    push(@fragmented, @regular[$i .. $i + 2]);
	} elsif (&efs'GetAnodeMember($anode, 'length2') == 0) {
	    push(@zero, @regular[$i .. $i + 2]);
	} else {
	    push(@blocked, @regular[$i .. $i + 2]);
	}
    }
}

#
# Return short description of special anode
#
sub specname {
    local($anode) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($type) = $flags & $efs'AFLAGS_TYPE_MASK;
    return $typename{$type};
}

#
# Find an anode of given type in @special.
#
sub specinfo {
    local($type) = @_;
    local($atype) = $type << $efs'AFLAGS_TYPE_SHIFT;
    local($anode, $flags, $offset, $i);
    for ($i = 0; $i <= $#special; $i += 2) {
	($anode, $offset) = @special[$i .. $i + 1];
	$flags = &efs'GetAnodeMember($anode, 'flags');
	return ($anode, $offset)
	    if (($flags & $efs'AFLAGS_TYPE_MASK) == $atype);
    }
    &logpr("## WARNING: specinfo: no anode of type $type\n");
    return ();
}

#
# Find anode info with specified name.
#
sub GetInfoByName {
    local($name) = shift;
    local($i);

    for ($i = 0; $i <= $#_; $i += 3) {
	return @_[$i .. $i + 2] if ($_[$i + 2] eq $name);
    }
    return ();
}

#
# Print messages at the beginning and end of each test.
#
sub print_start_message {
    local($testname, $description) = @_;
    print "\n";
    print $LONG_DELIMITER, "\n";
    &logpr("## Starting test $testname: $description\n");
    print $LONG_DELIMITER, "\n";
}

sub print_end_message {
    local($testname, $errors) = @_;
    print $LONG_DELIMITER, "\n";
    &logpr("## Test $testname %s\n", $errors ? 'FAILED' : 'PASSED');
    print "$LONG_DELIMITER\n\n";
}

#
# Print informational message during test.
#
sub test_info {
    local($testname) = shift;
    local($filename) = shift;
    &logpr("## test $testname, file $filename: ");
    &logpr(@_);
}

#
# Check that anode at specified index has been freed.  Common repair
# action for several properties.
#
sub check_anode_free {
    local($old, $new, $name, $index) = @_;
    local($flags) = &efs'GetAnodeMember($new, 'flags');
    local($type) = &efs'AnodeFlagsToType($flags);
    return 0 if ($type == 0);
    &salverr($name, $index, "expected flags == 0, got %#x\n", $flags);
}

#
# Test whether old and new anode members agree.
#
sub anode_diff {
    local($old, $new, $name, $index, $member) = @_;
    local($ovalue) = &efs'GetAnodeMember($old, $member);
    local($nvalue) = &efs'GetAnodeMember($new, $member);
    return 0 if ($ovalue == $nvalue);
    &salverr($name, $index, "$member was %#x, now %#x\n", $ovalue, $nvalue);
}

#
# Test whether old and new flags agree, but allowing UNSAFE bit
# in new flags.
#
sub anode_flags_diff {
    local($old, $new, $name, $index) = @_;
    local($oflags) = &efs'GetAnodeMember($old, 'flags');
    local($nflags) = &efs'GetAnodeMember($new, 'flags');
    return 0 if ($oflags == ($nflags & ~$efs'AFLAGS_UNSAFE));
    &salverr($name, $index, "expected flags %#x, got %#x\n", $oflags, $nflags);
}

#
# Compare a block within two objects for equality.
#
sub equal_objects {
    local($obj1, $obj2, $start, $end) = @_;
    return 0 if (length($obj1) != length($obj2));
    $end += length($obj1) if ($end < 0);
    local($len) = $end - $start;
    return substr($obj1, $start, $len) eq substr($obj2, $start, $len);
}

##
## Initialization, etc.
##

#
# Find and return FS index and initialize per-FS anode lists
#
sub Get_FS_Info {
    local($fs_name, *files, *dirs, *devs) = @_;
    local($fs_index) = &find_FS($fs_name);
    &find_files($fs_name, $fs_index, *files);
    &find_directories($fs_name, $fs_index, *dirs);
    &find_devices($fs_name, $fs_index, *devs);
    return $fs_index;
}

# Do some setup, then run all the tests.
#
sub PrepareForTests {
    open(LOG, ">$LOG_FILE") || die("Can't open $LOG_FILE: $!");
    &UseTmpAggr(1);
    &find_special_anodes;
    $backing_FS = &Get_FS_Info($backing_FS_name, *files, *dirs, *devices);
    @regular = (@files, @dirs, @devices);
    &split_regular_anodes(*regular, *block, *frag, *inline, *zero);
    $cow_FS = &Get_FS_Info($cow_FS_name, *cow_files, *cow_dirs, *cow_devs);
    @cow_reg = (@cow_files, @cow_dirs, @cow_devs);
    &split_regular_anodes(
	*cow_reg, *cow_block, *cow_frag, *cow_inline, *cow_zero);
}

#
# Run the tests whose names match @test_patterns, or all the tests
# if @test_patterns is empty.
#
sub RunTests {
    if (@test_patterns) {
	&RunMatchingTests;
    } else {
	&RunAllTests;
    }
}

sub RunAllTests {
    local($i, $j);
    local($test_name, $ntest);
    for ($i = 0; $i < $#test_categories; $i += 2) {
	($test_name, $ntest) = @test_categories[$i .. $i + 1];
	for ($j = 1; $j <= $ntest; $j++) {
	    eval "&test_" . $test_name . $j || die($@);
	}
    }
}

sub RunMatchingTests {
    local($i, $j);
    local($test_name, $ntest, $suffix, $pat);
    for ($i = 0; $i < $#test_categories; $i += 2) {
	($test_name, $ntest) = @test_categories[$i .. $i + 1];
	foreach $pat (@test_patterns) {
	    next unless $pat =~ /^$test_name/;
	    for ($j = 1; $j <= $ntest; $j++) {
		$suffix = $test_name . $j;
		eval "&test_" . $suffix || die($@)
		    if ($pat eq $test_name || $suffix =~ /^$pat$/);
	    }
	}
    }
}

##
##  Main program
##

&PrepareForTests;
&RunTests;
&Cleanup;
