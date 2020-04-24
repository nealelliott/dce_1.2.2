# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: efs_query.pl,v $
# Revision 1.1.8.1  1996/10/17  18:35:27  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:57:36  damon]
#
# Revision 1.1.3.1  1994/07/13  22:32:34  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:17:04  devsrc]
# 
# 	moving from file/episode to test/file
# 	[1994/03/22  21:02:44  delgado]
# 
# 	Transarc delta: jdp-ot3675-lfs-add-salvager-tests 1.1
# 	  Files modified:
# 	    Makefile
# 	    ravage: Makefile, and.c, bash.c, copy.c, copyall.sh
# 	    ravage: metamucil.c, nebconv.pl, pfaults.pl, ravage.sh
# 	    ravage: reravage.sh, rvg_setup.sh, scp.c, setall.sh, swap.c
# 	    ravage: swapall.sh, which.sh, zeroall.sh
# 	    scavenge: Makefile, aggrinfo.pl, efs.pl, efs_query.pl
# 	    scavenge: epidump.pl, fsdump.pl, simple_test.pl, upquota.pl
# 	    scavenge: util.pl
# 	  Selected comments:
# 	    This delta adds the ravager and scavenger tests to the Episode source tree.
# 	    These packages are for testing the Episode salvager.
# 	[1992/05/22  20:39:53  jdp]
# 
# Revision 1.1.4.4  1993/01/28  17:17:28  shl
# 	Transarc delta: blake-ot5950-new-salvager-tests 1.13
# 	  Selected comments:
# 	    The beginnings of a fairly comprehensive test suite for the salvager;
# 	    checkpointed here because of upcoming CML cataclysm.
# 	    Add new routines for finding anodes from pathnames and indices.
# 	    Another checkpoint.
# 	    Another checkpoint, since a large number of changes have accumulated.
# 	    Many bug fixes, performance improvements.
# 	    Another checkpoint; various new tests and bug fixes.
# 	    Another checkpoint to save recent changes
# 	    just another checkpoint...
# 	    another checkpoint...
# 	    Another checkpoint; more tests, reorganized into module.
# 	    salvager test suite, now ostensibly complete
# 	    Fix a couple of typos.
# 	    Fix a couple of bugs; take out perl -w option.
# 	    More silly bugs.
# 	[1993/01/27  18:25:03  shl]
# 
# Revision 1.1.4.3  1993/01/21  18:23:50  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  14:13:45  zeliff]
# 
# Revision 1.1.4.2  1992/09/25  16:28:08  jaffe
# 	Sync with Transarc by cleaned duplicate HISTORY and Log info
# 	[1992/09/24  17:50:58  jaffe]
# 
# Revision 1.1.1.2  1992/05/22  20:39:53  jdp
# 	Transarc delta: jdp-ot3675-lfs-add-salvager-tests 1.1
# 	  Files modified:
# 	    Makefile
# 	    ravage: Makefile, and.c, bash.c, copy.c, copyall.sh
# 	    ravage: metamucil.c, nebconv.pl, pfaults.pl, ravage.sh
# 	    ravage: reravage.sh, rvg_setup.sh, scp.c, setall.sh, swap.c
# 	    ravage: swapall.sh, which.sh, zeroall.sh
# 	    scavenge: Makefile, aggrinfo.pl, efs.pl, efs_query.pl
# 	    scavenge: epidump.pl, fsdump.pl, simple_test.pl, upquota.pl
# 	    scavenge: util.pl
# 	  Selected comments:
# 	    This delta adds the ravager and scavenger tests to the Episode source tree.
# 	    These packages are for testing the Episode salvager.
# 
# $EndLog$
#

# Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved

require 'efs';

#
# Module initialization
#

#
# These are used to divide sections of output
#
$LONG_DELIMITER = '#' x 20;
$SHORT_DELIMITER = '#' x 10;

#
# Put output on stderr
#
select(STDERR);

#
# Open the aggregate and read some frequently used values from it
#
$AGGR = $ARGV[0];
&Die("usage: $0 <aggregate>") unless $AGGR;
open(AGGR) || &Die("Can't open $AGGR: $!");
&efs'InitializeCachedInfo(*AGGR);

#
# This probably doesn't belong here...
#
srand(13);

#
# Aggregate I/O operations may be done either on the real aggregate
# (identified in $ARGV[0]) or on a temporary copy created below.
#

#
# Sparse copy program used to make temporary copies of aggregates
#
$SCP = 'scp';

#
# The salvager
#
$SALVAGE = 'salvage';

#
# PID of $SCP running in the background; if zero, no $SCP currently
# running; if -1, attempt to run failed.
#
$Background_scp = 0;

#
# PID of salvager; we use this to kill the salvager if it seems to
# have gone into an infinite loop.
#
$salvage_pid = 0;

#
# Max. no. of seconds that salvager can run before we kill it.
#
$max_salvage_run_time = 5 * 60;

#
# Are we doing I/O to the temporary copy or the real aggregate?
#
$using_tmp_aggr = 0;

#
# Temporary files to hold aggregate info
#
$TMP_AGGR_DIR = $ENV{'RVG_AGGR_TMP_DIR'};
$TMP_AGGR_DIR = '/tmp' unless $TMP_AGGR_DIR;

$TMP_AGGR	= "$TMP_AGGR_DIR/rvgaggr.$$";
$TMP_AGGR_2	= $TMP_AGGR . '_2';
$TMP_NZB	= "$TMP_AGGR_DIR/rvgnzb.$$";

#
# Install signal handlers
#
$SIG{'HUP'} = 'CleanupAndExit';
$SIG{'INT'} = 'CleanupAndExit';
$SIG{'TERM'} = 'CleanupAndExit';
$SIG{'ALRM'} = 'KillSalvager';


##############################################################################
## General purpose routines
##############################################################################

sub Cleanup {
    if ($Background_scp > 0) {
	waitpid($Background_scp, 0) if (kill(15, $Background_scp));
	$Background_scp = 0;
    }
    unlink $TMP_AGGR, $TMP_AGGR_2, $TMP_NZB;
}

sub CleanupAndExit {
    &Cleanup;
    exit 1;
}

sub Die {
    &Cleanup;
    die(@_);
}

sub RunSalvager {
    local($is_recursive) = @_;
    local($status);

    if ($nothing_set) {
	print "## No values were changed, hence salvager not run\n";
	return ();
    }

    $status = &Salvage();
    if ($status & 0xff) {
	&SalvageError("Salvager killed by signal %d\n", $status & 0x7f);
	return ($status);
    }
    $status >>= 8 if (($status & 0xff) == 0);
    if ($status == 0) {
	print "##! Salvager didn't report any damage\n";
    } elsif ($status == 1 || $status == 2) {
	&SalvageError("Salvage returned error status %#x", $status);
    } elsif ($status == 3) {
	print "## Verifying successful salvage...\n";
	&SalvageError("Verification failed") if (&Salvage('-verify') != 0);
    } elsif ($status & 0x80) {
	&SalvageError("Salvager dumped core (status=%#x)", $status);
    } elsif ($status & 0x4 && !$is_recursive) {
	printf "## Salvager returned %#x, requiring another salvage...\n",
		$status;
	&RunSalvager(1);
    } else {
	&SalvageError("Salvager failed (status=%#x)", $status);
    }
    return ($status);
}

#
# Kill off salvager if it has been running too long.
#
sub KillSalvager {
    if ($salvage_pid != 0) {
	kill(15, $salvage_pid);
	printf("## Killing salvager after $max_salvage_run_time seconds\n");
    }
}

#
# Make temporary copy of aggregate and switch I/O operations to it.
#
sub StartTest {
    &UseTmpAggr(0);
}

#
# Run the salvager and then close temporary aggregate; used by
# existing test programs in which salvaging is the final step.
#
sub EndTest {
    &RunSalvager;
    &SwitchToRealAggr;
}

sub FindObject {
    print "${LONG_DELIMITER}\n";
    if (@result = &Flush(@_)) {
	($object, $offset) = @result[$#result-1, $#result];
    } else {
	warn("\nCan't find a matching object");
    }
    return @result;
}

sub Flush {
    local(*func, $expr, @next) = @_;
    return &func($expr, @next);
}

#
# Store object information in an array; intended for iterating over
# objects to find all satisfying some property.
#
sub PushObject {
    local(*input, *info_array, @next) = @_;
    local($obj, $offset) = @input[$#input - 1, $#input];
    push(@info_array, $obj, $offset);
    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*input, $next_expr, @next_next);
    } else {
	return ();
    }
}

sub Salvage {
    local($arg) = @_;
    local(@salvage_args) = defined($arg) ?
	('-force', split(' ', $arg)) : ('-force');

    $salvage_pid = fork;
    exec $SALVAGE, @salvage_args, ${TMP_AGGR} if ($salvage_pid == 0);
    alarm($max_salvage_run_time);
    waitpid($salvage_pid, 0);
    local($status) = $?;
    alarm(0);
    $salvage_pid = 0;
    return $status;
}

sub SalvageError {
    local($format, @list) = @_;

    print '##!! ';
    printf $format, @list;
    print "\n";
}

sub ScribbleOnACL {
    local($val) = @_;
    local($acl_anode) = $object;
    local($buf) = pack("L", $val);
    local($length) = &efs'GetAnodeMember($acl_anode, 'length2');
    local($offset) = int($length * rand);

    &efs'AnodeWrite($buf, *AGGR, $offset, &util'FormatTypeToSize('L'),
		$acl_anode);
    $nothing_set = 0;
}

#
# Note: if any of the following output functions is used to alter the
# superblock, efs'InitializeCachedInfo should be called afterwards to
# avoid stale data.
#

#
# Update a member within an object
#
# XXX too intimate with util.pl details
#
sub Set {
    local($obj, $field, $value, $offset, %map) = @_;
    local($bin_value);
    local($disk_offset);

    defined($map{$field}) || &Die("${field}: bogus field name");

    local($field_offset, $format_string, $size, $print_format) =
	split(/:/, $map{$field});

    $bin_value = pack($format_string, $value);
    if ($bin_value eq &util'MapGetRaw($obj, $field, %map)) {
	printf "## %s is already set to $print_format\n", $field, $value;
    } else {
	$disk_offset = $offset + $field_offset;
	printf "## Setting %s (%d byte%s at offset %d) to $print_format\n",
		$field, $size, ($size == 1) ? "" : "s", $disk_offset, $value;
	&efs'Write(*AGGR, $bin_value, $disk_offset, $size);
	$nothing_set = 0;
    }
}

#
# Write an array at the specified offset
#
sub Set_Array {
    local(*adata, $offset, $atype, $alen) = @_;
    local($bin_value) = pack("$atype$alen", @adata);
    local($size) = &util'FormatTypeToSize($atype) * $alen;

    print "## Setting $size bytes at offset $offset\n";
    &efs'Write(*AGGR, $bin_value, $offset, $size);
    $nothing_set = 0;
}

#
# Write already packed data at the specified offset
#
sub WriteData {
    local($bdata, $offset) = @_;
    local($size) = length($bdata);
    print "## Writing $size bytes at offset $offset\n";
    &efs'Write(*AGGR, $bdata, $offset, $size);
    $nothing_set = 0;
}

#
# Make a temporary copy of the aggregate and switch I/O operations to it.
# This can be a bit time-consuming, so when we are going to run several
# tests, it may help to do a "copy-ahead" in parallel with testing.
# $Background_scp contains the PID of the background copy process;
# if a copy fails, we set $Background_scp to -1 and fall back to making
# one copy at a time.
#
sub UseTmpAggr {
    local($copy_ahead) = @_;
    return () if ($using_tmp_aggr && $nothing_set);
    print "${SHORT_DELIMITER}\n";
    print "## Copying aggregate...\n";
    if ($Background_scp <= 0) {
	if (-e $TMP_NZB) {
	    system "$SCP -N $TMP_NZB $AGGR $TMP_AGGR";
	} else {
	    system "$SCP -n $AGGR $TMP_AGGR > $TMP_NZB";
	}
    } else {
	waitpid($Background_scp, 0);
	if ($? == 0 && rename($TMP_AGGR_2, $TMP_AGGR)) {
	    &SwitchToRealAggr if ($using_tmp_aggr);
	    $Background_scp = 0;
	} else {
	    $Background_scp = -1;
	    unlink $TMP_AGGR_2;
	    system "$SCP -N $TMP_NZB $AGGR $TMP_AGGR";
	}
    }
    if ($copy_ahead && $Background_scp == 0) {
	$Background_scp = fork;
	exec $SCP, '-N', $TMP_NZB, $AGGR, $TMP_AGGR_2
	    if ($Background_scp == 0);
    }
    &SwitchToTmpAggr;
    $nothing_set = 1;
}

#
# Switch between temporary copy of aggregate and the real one.
#
sub SwitchToTmpAggr {
    if (!$using_tmp_aggr) {
	close(AGGR) || &Die("$AGGR: close failed: $!");
	open(AGGR, "+<$TMP_AGGR") || &Die("$TMP_AGGR: open failed: $!");
	$using_tmp_aggr = 1;
	&efs'InitializeCachedInfo(*AGGR);
	&efs'InvalidateReadBuffer;
    }
}

sub SwitchToRealAggr {
    if ($using_tmp_aggr) {
	close(AGGR) || &Die("$TMP_AGGR: close failed: $!");
	open(AGGR) || &Die("Can't open $AGGR: $!");
	$using_tmp_aggr = 0;
	&efs'InitializeCachedInfo(*AGGR);
	&efs'InvalidateReadBuffer;
    }
}

##############################################################################
## Routines for each object type
##############################################################################

#######################################
## Aggregate routines
#######################################

sub GetAggr {
    local($expr, @next) = @_;
    local(@aggr) = ($efs'SuperBlock, $efs'SBOffset);

    # Ignore $expr
    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	@result = &next_func(*aggr, $next_expr, @next_next);
	return @result;
    } else {
	print "## Found an aggregate (${expr}) at offset $aggr[1]\n";
	return @aggr;
    }
}

sub PrintAggr {
    local(*input, $expr, @next) = @_;
    local($aggr, $offset) = @input[$#input - 1, $#input];

    print "\nAggregate header at offset $offset\n";
    &efs'AggrPrint($aggr);

    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*input, $next_expr, @next_next);
    } else {
	return (eval $expr) ? @input : ();
    }
}

sub SetAggr {
    &Set_Aggr($object, $offset, @_);
}

sub Set_Aggr {
    local($aggr, $aggroff, $field, $value) = @_;
    &Set($aggr, $field, $value, $aggroff, %efs'AGGR_HEADER_MAP);
    &efs'InitializeCachedInfo(*AGGR);
}

#######################################
## Anode routines
#######################################

sub GetAVLAnode {
    local(*input, $expr, @next) = @_;
    local(*next_func, $next_expr, @next_next) = @next;
    local(@block_map) = &efs'AnodeGetBlockMap(*AGGR, $efs'AVL_anode);
    local($i, $a);
    local($last_index) = $efs'anodes_per_block * @block_map - 1;
    local(@fs_anode, @output);

    for ($i = 1; $i <= $last_index; $i++) {
	@fs_anode = &efs'AnodeGet(*AGGR, $i, $efs'AVL_anode);
	$a = $fs_anode[0];
	next unless (@fs_anode && &efs'AnodeIsValid($a));

	local(%fs_anode);
	local($type);

	if ($expr != 1) {
	    %fs_anode = &util'UnpackAssoc($a, &efs'AnodeGetFormat($a));
	    $type = &efs'AnodeFlagsToType($fs_anode{'flags'});
	}

	next unless ($expr == 1 || eval $expr);
	@output = (@input, @fs_anode);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found an AVL anode (${expr}) at offset $fs_anode[1]\n";
	    return @output;
	}
    }
    return ();
}

#
# Get the anode with the specified index.  We use this routine to examine
# anodes following salvaging to verify that they were correctly repaired.
# The code doesn't check for a valid anode here, since the expected salvager
# action is often to free the anode.
#
sub GetAVLAnodeFromIndex {
    local(*input, $index, @next) = @_;
    local(@fs_anode) = &efs'AnodeGet(*AGGR, $index, $efs'AVL_anode);

    return () unless (@fs_anode);

    local(@output) = (@input, @fs_anode);
    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	@result = &next_func(*output, $next_expr, @next_next);
	return @result;
    } else {
	print "## Found an AVL anode (${expr}) at offset $fs_anode[1]\n";
	return @output;
    }
}


sub GetFSAnode {
    local(*input, $expr, @next) = @_;
    local(*next_func, $next_expr, @next_next) = @next;
    local($fs) = $input[$#input - 1];
    local($fs_anode) = $input[$#input - 3];
    local(@block_map) = &efs'AnodeGetBlockMap(*AGGR, $fs_anode);
    local($last_index) = $efs'anodes_per_block * @block_map - 1;
    local($last_fid) = &efs'FSIndexToFid($last_index);
    local($fid, $i, $a, $aoff, $flags, $type, $status_size);
    local(@anode, @output);
    local(%anode);

    for ($fid = 1; $fid <= $last_fid; $fid++) {
	$i = &efs'FSFidToIndex($fid);
	@anode = &efs'AnodeGet(*AGGR, $i, $fs_anode);
	next unless (@anode || &efs'AnodeIsValid($anode[0]));

	($a, $aoff) = @anode;
	if ($expr != 1) {
	    %anode = &util'UnpackAssoc($a, &efs'AnodeGetFormat($a));
	    $flags = $anode{'flags'};
	    $type = &efs'AnodeFlagsToType($flags);
	    $status_size = $flags & $efs'ANODE_STATUSSIZE_MASK;
	}

	next unless ($expr == 1 || eval $expr);

	@output = (@input, @anode);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found a file set anode ($expr) at offset $aoff\n";
	    return @output;
	}
    }
    return ();
}

#
# Find an unused anode and partially initialize it.
#
sub GetFreeFSAnode {
    local(*input, $status_size, @next) = @_;
    local(*next_func, $next_expr, @next_next) = @next;
    local($fs) = $input[$#input - 1];
    local($fs_anode) = $input[$#input - 3];
    local(@block_map) = &efs'AnodeGetBlockMap(*AGGR, $fs_anode);
    local($last_index) = $efs'anodes_per_block * @block_map - 1;
    local($last_fid) = &efs'FSIndexToFid($last_index);
    local($fid, $i, $j, $a, $aoff, $flags, $type);
    local(@anode, @output);

    for ($fid = $last_fid; $fid != 0; --$fid) {
	$i = &efs'FSFidToIndex($fid);
	@anode = &efs'AnodeGet(*AGGR, $i, $fs_anode);
	next unless @anode;

	($a, $aoff) = @anode;
	$flags = &efs'GetAnodeMember($a, 'flags');
	$type = &efs'AnodeFlagsToType($flags);
	next unless ($type == 0);

	if (!&efs'AnodeIsValid($a)) {
	    $flags = $efs'ANODE_MAGIC1 | $efs'ANODE_MAGIC2 |
		$efs'AFLAGS_TYPE_REGULAR | $status_size;
	    &Set_Anode($a, $aoff, 'flags', $flags);
	    &Set_Anode($a, $aoff, 'index', $i);
	    @anode = &efs'AnodeGet(*AGGR, $i, $fs_anode);
	}

	@output = (@input, @anode);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found a free file set anode at offset $aoff\n";
	    return @output;
	}
    }
    return ();
}

#
# Get the anode with the specified index.  We use this routine to examine
# anodes following salvaging to verify that they were correctly repaired.
# The code doesn't check for a valid anode here, since the expected salvager
# action is often to free the anode.
#
sub GetFSAnodeFromIndex {
    local(*input, $index, @next) = @_;
    local($fs) = $input[$#input - 1];
    local($fs_anode) = $input[$#input - 3];
    local(@anode) = &efs'AnodeGet(*AGGR, $index, $fs_anode);

    return () unless (@anode);

    local(@output) = (@input, @anode);
    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	@result = &next_func(*output, $next_expr, @next_next);
	return @result;
    } else {
	print "## Found a file set anode (index == $index) at offset " .
	    "$anode[1]\n";
	return @output;
    }
}

sub GetFSAnodeFromDirEntry {
    local(*input, $expr, @next) = @_;
    local($de) = $input[$#input - 1];
    local($fid) = &efs'GetDirEntryMember($de, 'fid.index');
    local($fs_anode) = $input[2];

    local(@anode) = &efs'AnodeGet(*AGGR, &efs'FSFidToIndex($fid), $fs_anode);
    return () unless @anode;

    local($a) = $anode[0];
    local(%anode) = &util'UnpackAssoc($a, &efs'AnodeGetFormat($a))
	unless ($expr == 1);

    return () unless ($expr == 1 || eval $expr);

    local(@output) = (@input, @anode);
    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	@result = &next_func(*output, $next_expr, @next_next);
	return @result;
    } else {
	print "## Found anode (${expr}) at offset $anode[1]\n";
	return @output;
    }
}

#
# Find the anode corresponding to the given path, which is interpreted
# relative to the fileset's root directory.  The path should not begin
# with a slash; an empty path denotes the root.
#
sub GetFSAnodeFromPath {
    local(*input, $expr, @next) = @_;
    local(@path) = split(/\//, $expr);

    local($fs) = $input[$#input - 1];
    local($fs_anode) = $input[$#input - 3];
    local(@anode) = &efs'AnodeGet(*AGGR, $efs'DIR_ROOT_INDEX, $fs_anode);
    local($compoment, $length, $mode);
    local(@dh, @de, %de, $dent, $fid, $i);

search_path:
    while (@path && @anode && &efs'AnodeIsValid($anode[0])) {
	$component = shift(@path);
	$length = &efs'GetAnodeMember($anode[0], 'length2');
	$mode = &efs'GetAnodeMember($anode[0], 'mode');

	return () if (($mode & $efs'STAT_IFDIR) == 0);

	for ($i = 0; $length > 0; $i++, $length -= $efs'DIR_PAGE_SIZE) {
	    @dh = &efs'DirPageGet(*AGGR, $i, $anode[0]);
	    $dent = $efs'DIR_ENTRY_FIRST_ENTRY;
	    while (@de = &efs'DirEntryGet($dent, @dh)) {
		%de = &util'UnpackAssoc($de[0],
			&efs'DirEntryGetFormat($de[0], 1));

		if ($de{'inuse'} == 1 && $de{'name'} eq $component) {
		    $fid = &efs'GetDirEntryMember($de[0], 'fid.index');

		    @anode = &efs'AnodeGet(*AGGR, &efs'FSFidToIndex($fid),
				  $fs_anode);

		    next search_path;
		}
	    }
	    return ();
	}
    }

    return () unless (@anode);

    local(@output) = (@input, @anode);
    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	@result = &next_func(*output, $next_expr, @next_next);
	return @result;
    } else {
	local($pathname) = $expr ? $expr : "root directory";
	print "## Found anode for $pathname at offset $anode[1]\n";
	return @output;
    }
}

sub GetBlockMap {
    local($anode) = @_;
    return &efs'AnodeGetBlockMap(*AGGR, $anode);
}

sub PrintAnode {
    local(*input, $expr, @next) = @_;
    local($anode, $offset) = @input[$#input - 1, $#input];

    print "\nAnode at offset $offset\n";
    &efs'AnodePrint($anode);

    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*input, $next_expr, @next_next);
    } else {
	return (eval $expr) ? @input : ();
    }
}

sub SetAnode {
    &Set_Anode($object, $offset, @_);
}

sub Set_Anode {
    local($anode, $aoff, $field, $value) = @_;
    local(%map) = &efs'AnodeGetMap($anode);
    &Set($anode, $field, $value, $aoff, %map);
}

#
# Change the flags of the in-core anode in order to change its format;
# a bit of a hack, particularly since knowledge of the size and location
# of the flags shouldn't be embedded here.
#
sub Set_IncoreFlags {
    local(*anode, $newflags) = @_;
    substr($anode, 0, &util'FormatTypeToSize('L')) = pack('L', $newflags);
}

#######################################
## Anode Block routines
#######################################

sub GetAnodeBlock {
    local(*input, $expr, @next) = @_;
    local($anode) = $input[$#input - 1];
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($type) = &efs'AnodeFlagsToType($flags);

    return () unless ($type == $efs'CFLAGS_VOLUME || $type == $efs'CFLAGS_AVL);

    local($pdelta) = ($type == $efs'CFLAGS_AVL) ? 2 : 1;
    local($bdelta) = $pdelta * $efs'blockSize;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    local(*next_func, $next_expr, @next_next) = @next;
    local(@ablock, @output);
    local(%ablock);
    local($i);

    for ($i = 0; $length > 0; $i += $pdelta, $length -= $bdelta) {
	@ablock = &efs'AnodeBlockGet(*AGGR, $i, $anode);
	next unless @ablock;

	%ablock = &util'UnpackAssoc($ablock[0], @efs'ABH_FORMAT)
	    unless ($expr == 1);
	next unless ($expr == 1 || eval $expr);

	@output = (@input, @ablock);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found an anode block (${expr}) at offset $ablock[1]\n";
	    return @output;
	}
    }
    return ();
}

sub SetAnodeBlock {
    &Set_AnodeBlock($object, $offset, @_);
}

sub Set_AnodeBlock {
    local($abpage, $aboff, $field, $value) = @_;
    &Set($abpage, $field, $value, $aboff, %efs'ABH_MAP);
}

#######################################
## Bitmap Page routines
#######################################

sub GetBitmapPage {
    local(*input, $expr, @next) = @_;
    local($anode) = $input[$#input - 1];
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($type) = &efs'AnodeFlagsToType($flags);

    return () unless ($type == $efs'CFLAGS_BITMAP);

    local($length) = &efs'GetAnodeMember($anode, 'length2');
    local(*next_func, $next_expr, @next_next) = @next;
    local(@bp, @output);
    local(%bp);
    local($i);

    for ($i = 0; $length > 0; $i++, $length -= $efs'blockSize) {
	@bp = &efs'BitmapPageGet(*AGGR, $i, $anode);
	next unless @bp;

	%bp = &util'UnpackAssoc($bp[0], @efs'BITMAP_PAGE_FORMAT)
	    unless ($expr == 1);
	next unless ($expr == 1 || eval $expr);

	@output = (@input, @bp);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found a bitmap page (${expr}) at offset $bp[1]\n";
	    return @output;
	}
    }
    return ();
}

sub PrintBitmapPage {
    local(*input, $expr, @next) = @_;
    local($bp, $offset) = @input[$#input - 1, $#input];

    print "\nBitmap page at offset $offset\n";
    &efs'BitmapPagePrint($bp);

    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*input, $next_expr, @next_next);
    } else {
	return (eval $expr) ? @input : ();
    }
}

sub SetBitmapPage {
    &Set_BitmapPage($object, $offset, @_);
}

sub Set_BitmapPage {
    local($bpage, $bpoff, $field, $value) = @_;
    &Set($bpage, $field, $value, $bpoff, %efs'BITMAP_PAGE_MAP);
}

#
# Find a free block in a bitmap page; mark it in use if $expr == 1
#
sub GetFreeBlock {
    local(*input, $expr, @next) = @_;
    local($bp, $offset) = @input[$#input - 1, $#input];

    local($addr) = &efs'FreeBlockGet(*AGGR, $bp, $offset, $expr);
    local(@output) = (@input, $addr);
    if ($addr != 0 && @next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*output, $next_expr, @next_next);
    } else {
	return @output;
    }
}

#######################################
## Log Page routines
#######################################

sub GetLogPage {
    local(*input, $expr, @next) = @_;
    local($anode) = $input[$#input - 1];
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($type) = &efs'AnodeFlagsToType($flags);

    return () unless ($type == $efs'CFLAGS_LOG);

    local($length) = &efs'GetAnodeMember($anode, 'length2');
    local(*next_func, $next_expr, @next_next) = @next;
    local(@logp, @output);
    local(%logp);
    local($i);

    for ($i = 0; $length > 0; $i++, $length -= $efs'blockSize) {
	@logp = &efs'LogPageGet(*AGGR, $i, $anode);
	next unless @logp;

	%logp = &util'UnpackAssoc($logp[0], @efs'LOG_PAGE_HEADER_FORMAT)
	    unless ($expr == 1);
	next unless ($expr == 1 || eval $expr);

	@output = (@input, @logp);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found a log page (${expr}) at offset $logp[1]\n";
	    return @output;
	}
    }
    return ();
}

sub Set_LogPage {
    local($lpage, $lpoff, $field, $value) = @_;
    &Set($lpage, $field, $value, $lpoff, %efs'LOG_PAGE_HEADER_MAP);
}

#######################################
## Badblock Container routines
#######################################

sub GetBadblockFrame {
    local(*input, $expr, @next) = @_;
    local(*next_func, $next_expr, @next_next) = @next;
    local($anode) = $input[$#input - 1];
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($type) = &efs'AnodeFlagsToType($flags);

    return () unless ($type == $efs'CFLAGS_BADBLOCKS);

    local($length) = &efs'GetAnodeMember($anode, 'length2');
    local(@bp, @output);
    local(%bp);
    local($i);

    for ($i = 0; $length > 0; $i++, $length -= $efs'BBFrameSize) {
	@bp = &efs'BadblockFrameGet(*AGGR, $i, $anode);
	next unless @bp;

	%bp = &util'UnpackAssoc($bp[0], @efs'BB_FRAME_FORMAT)
	    unless ($expr == 1);
	next unless ($expr == 1 || eval $expr);

	@output = (@input, @bp);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found a badblock frame (${expr}) at offset $bp[1]\n";
	    return @output;
	}
    }
    return ();
}

sub Set_BadblockFrame {
    local($bb, $bboff, $field, $value) = @_;
    &Set($bb, $field, $value, $bboff, %efs'BB_FRAME_MAP);
}

sub GetBadblockEntry {
    local(*input, $expr, @next) = @_;
    local(*next_func, $next_expr, @next_next) = @next;
    local(@entry, @output);
    local(@bb) = @input[$#input - 1, $#input];
    local($bb_offset) = $bb[1];
    local($i) = $efs'BB_FRAME_SIZE;
    local(%entry);

    while (@entry = &efs'BadblockEntryGet($i, @bb)) {

	%entry = &util'UnpackAssoc($entry[0], @efs'BB_ENTRY_FORMAT)
	    unless ($expr == 1);
	next unless ($expr == 1 || eval $expr);

	@output = (@input, @entry);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found a badblock entry (${expr}) at offset $entry[1]\n";
	    return @output;
	}
    }
    return ();
}

sub Set_BadblockEntry {
    local($be, $beoff, $field, $value) = @_;
    &Set($be, $field, $value, $beoff, %efs'BB_ENTRY_MAP);
}

sub Set_BadblockData {
    local(*bbdata, $entry_off) = @_;
    local($nitem) = 1 + $#bbdata;
    &Set_Array(*bbdata, $entry_off + $efs'BB_ENTRY_SIZE, 'L', $nitem);
}

#######################################
## Directory Entry routines
#######################################

sub GetDirEntry {
    local(*input, $expr, @next) = @_;
    local(*next_func, $next_expr, @next_next) = @next;
    local(@de, @output);
    local(@dh) = @input[$#input - 1, $#input];
    local($dh_offset) = $input[$#input];
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    local(%de);

    while (@de = &efs'DirEntryGet($i, @dh)) {

	%de = &util'UnpackAssoc($de[0], &efs'DirEntryGetFormat($de[0], 1))
	    unless ($expr == 1);
	next unless ($expr == 1 || eval $expr);

	@output = (@input, @de);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found a directory entry (${expr}) at offset " .
		"$de[1]\n";
	    return @output;
	}
    }
    return ();
}

sub PrintDirEntry {
    local(*input, $expr, @next) = @_;
    local($de, $offset) = @input[$#input - 1, $#input];

    print "\nDirectory entry at offset $offset\n";
    &efs'DirEntryPrint($de);

    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*input, $next_expr, @next_next);
    } else {
	return (eval $expr) ? @input : ();
    }
}

sub SetDirEntry {
    &Set_DirEntry($object, $offset, @_);
}

sub Set_DirEntry {
    local($de, $deoff, $field, $value) = @_;
    local(%map) = &efs'DirEntryGetMap($de);
    &Set($de, $field, $value, $deoff, %map);
}

#######################################
## Directory Page routines
#######################################

sub GetDirPage {
    local(*input, $expr, @next) = @_;
    local(*next_func, $next_expr, @next_next) = @next;
    local($anode) = $input[$#input - 1];
    local($mode) = &efs'GetAnodeMember($anode, 'mode');

    return () unless ($mode & $efs'STAT_IFDIR);

    local($length) = &efs'GetAnodeMember($anode, 'length2');
    local(@output, @dh);
    local(%dh);
    local($i);

    for ($i = 0; $length > 0; $i++, $length -= $efs'DIR_PAGE_SIZE) {
	@dh = &efs'DirPageGet(*AGGR, $i, $anode);
	next unless @dh;

	%dh = &util'UnpackAssoc($dh[0], @DIR_HEADER_FORMAT)
	    unless ($expr == 1);

	next unless ($expr == 1 || eval $expr);
	@output = (@input, @dh);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	 } else {
	    print "## Found a directory page (${expr}) at offset $dh[1]\n";
	    return @output;
	}
    }
    return ();
}

sub PrintDirPage {
    local(*input, $expr, @next) = @_;
    local($dp, $offset) = @input[$#input - 1, $#input];

    print "\nDirectory page at offset $offset\n";
    &efs'DirPagePrint($dp);

    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*input, $next_expr, @next_next);
    } else {
	return (eval $expr) ? @input : ();
    }
}

sub SetDirPage {
    &Set_DirPage($object, $offset, @_);
}

sub Set_DirPage {
    local($dpage, $dpoff, $field, $value) = @_;
    &Set($dpage, $field, $value, $dpoff, %efs'DIR_HEADER_MAP);
}

sub Set_DirHash {
    local(*hdata, $dhoff) = @_;
    local($nitem) = 1 + $#hdata;
    local($hashoff) = &util'MapOffset('hashTable[0..3]', %efs'DIR_HEADER_MAP);
    &Set_Array(*hdata, $dhoff + $hashoff, 'C', $nitem);
}

#######################################
## File Set routines
#######################################

sub GetFileSet {
    local(*input, $expr, @next) = @_;

    local($anode) = $input[$#input - 1];
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($type) = &efs'AnodeFlagsToType($flags);

    return () unless ($type == $efs'CFLAGS_VOLUME);

    local(@fs) = &efs'FSGet(*AGGR, $anode);
    return () unless @fs;

    local(%fs) = &util'UnpackAssoc($fs[0], @efs'FS_HEADER_FORMAT)
	unless ($expr == 1);

    return () unless ($expr == 1 || eval $expr);

    local(@output) = (@input, @fs);

    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*output, $next_expr, @next_next);
    } else {
	print "## Found file set (${expr}) at offset $fs[1]\n";
	return @output;
    }
}

sub SetFileSet {
    &Set_FileSet($object, $offset, @_);
}

sub Set_FileSet {
    local($fs, $fsoff, $field, $value) = @_;
    &Set($fs, $field, $value, $fsoff, %efs'FS_HEADER_MAP);
}

sub PrintFileSet {
    local(*input, $expr, @next) = @_;
    local($fs, $offset) = @input[$#input - 1, $#input];

    print "\nFile set at offset $offset\n";
    &efs'FSPrint($fs);

    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*input, $next_expr, @next_next);
    } else {
	return (eval $expr) ? @input : ();
    }
}

#######################################
## Indirect Block routines
#######################################

sub GetIndirect {
    local(*input, $expr, @next) = @_;
    local(*next_func, $next_expr, @next_next) = @next;
    local($anode) = $input[$#input - 1];
    local($flags) = &efs'GetAnodeMember($anode, 'flags');

    return ()
	if ($flags & ($efs'AFLAGS_DATA_INLINE | $efs'AFLAGS_DATA_FRAGMENTED));

    local(@indirect_map) = &efs'AnodeGetIndirectBlockMap(*AGGR, $anode);
    local(@ib, @output);
    local(%ib);
    local($i);

    for ($i = 0; $i <= $#indirect_map; $i++) {
	@ib = &efs'IndirectGet(*AGGR, $indirect_map[$i]);
	next unless @ib;

	%ib = &util'UnpackAssoc($ib[0], @efs'INDIRECT_HEADER_FORMAT)
	    unless ($expr == 1);

	next unless ($expr == 1 || eval $expr);
	@output = (@input, @ib);
	if (@next) {
	    @result = &next_func(*output, $next_expr, @next_next);
	    return @result if @result;
	} else {
	    print "## Found indirect block (${expr}) at offset $ib[1]\n";
	    return @output;
	}
    }
    return ();
}

sub SetIndirect {
    &Set_Indirect($object, $offset, @_);
}

sub Set_Indirect {
    local($ib, $iboff, $field, $value) = @_;
    &Set($ib, $field, $value, $iboff, %efs'INDIRECT_HEADER_MAP);
}

sub Set_IndirectBlockAddrs {
    local(*ibpdata, $iboff) = @_;
    &Set_Array(*ibpdata, $iboff + $efs'INDIRECT_HEADER_SIZE,
	'L', $efs'ibp_per_block);
}

sub PrintIndirect {
    local(*input, $expr, @next) = @_;
    local($ib, $offset) = @input[$#input - 1, $#input];

    print "\nIndirect block at offset $offset\n";
    &efs'IndirectPrint($ib);

    if (@next) {
	local(*next_func, $next_expr, @next_next) = @next;
	return &next_func(*input, $next_expr, @next_next);
    } else {
	return (eval $expr) ? @input : ();
    }
}

1;
