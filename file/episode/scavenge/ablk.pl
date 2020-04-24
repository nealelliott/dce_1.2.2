# Copyright (C) 1993 Transarc Corporation - All rights reserved
#
# Anode block tests for the salvager test suite
#

#
# Get an anode block from this file system and test by breaking some
# consistency condition, running the salvager, and checking the repair.
#
# We just use the first anode block in the fileset.  In the small filesets
# that typically used for testing, there is only one block anyway.
#
sub test_ablock {
    local($fs_index, $fs_name, $test_name, *break_rule, *check_repair) = @_;
    local(@fs_info) = &FindObject(
	*GetAggr, 1, *GetAVLAnodeFromIndex, $fs_index);
    local(@result) = &GetAnodeBlock(*fs_info, 1);
    return 1 unless (@result);

    &UseTmpAggr(1);

    local($ablock, $offset) = @result[$#result - 1, $#result];
    &break_rule($test_name, $ablock, $offset, $fs_name);

    &RunSalvager();

    @result = &GetAnodeBlock(*fs_info, '$i == 0');
    return &salverr($fs_name, $fs_index, "Anode block unreadable\n")
	unless (@result);

    local($new_ablock) = $result[$#result - 1];
    &check_repair($ablock, $new_ablock, $fs_name, $fs_index);
}

#
# ABlk1. Magic number must be one of ANODE_BLOCK_{AVL, VOLUME}_MAGIC
#
# Repair action: reinitialize blocks; free all anodes within them.
#
sub test_ablk1 {
    local($this_test) = 'ablk1';
    &print_start_message($this_test, 'Try bad anode block magic number');
    local($errors) = &test_ablock($backing_FS, $backing_FS_name,
	$this_test . 'a', *break_ablk1, *check_ablk_cleared);
    $errors += &test_ablock($efs'AVL_index, 'AVL',
	$this_test . 'b', *break_ablk1, *check_ablk_cleared);
    &print_end_message($this_test, $errors);
}

sub break_ablk1 {
    local($test, $ablock, $aoff, $name) = @_;
    &test_info($test, $name, "setting magic number to zero\n");
    &Set_AnodeBlock($ablock, $aoff, 'magic', 0);
}

#
# ABlk2. Magic number is ANODE_BLOCK_AVL_MAGIC iff block in AVL
#
# Repair action: reinitialize block
#
sub test_ablk2 {
    local($this_test) = 'ablk2';
    &print_start_message($this_test, 'Try using wrong magic number');
    local($errors) = &test_ablock($backing_FS, $backing_FS_name,
	$this_test . 'a', *break_ablk2a, *check_ablk_cleared);
    $errors += &test_ablock($efs'AVL_index, 'AVL',
	$this_test . 'b', *break_ablk2b, *check_ablk_cleared);
    &print_end_message($this_test, $errors);
}

sub break_ablk2a {
    local($test, $ablock, $aoff, $name) = @_;
    &test_info($test, $name, "setting magic number to AVL_MAGIC\n");
    &Set_AnodeBlock($ablock, $aoff, 'magic', $efs'ABH_AVL_MAGIC);
}

sub break_ablk2b {
    local($test, $ablock, $aoff, $name) = @_;
    &test_info($test, $name, "setting magic number to VOLUME_MAGIC\n");
    &Set_AnodeBlock($ablock, $aoff, 'magic', $efs'ABH_VOLUME_MAGIC);
}

#
# ABlk3. Index must identify an anode of the appropriate type.
#
# Repair action: reinitialize block
#
sub test_ablk3 {
    local($this_test) = 'ablk3';
    &print_start_message($this_test, 'Try indexing anode of wrong type');
    local($errors) = &test_ablock($backing_FS, $backing_FS_name,
	$this_test . 'a', *break_ablk3, *check_ablk_cleared);
    $errors += &test_ablock($efs'AVL_index, 'AVL',
	$this_test . 'b', *break_ablk3, *check_ablk_cleared);
    &print_end_message($this_test, $errors);
}

sub break_ablk3 {
    local($test, $ablock, $aoff, $name) = @_;
    local($log_anode) = &specinfo($efs'CFLAGS_LOG);
    local($log_index) = &efs'GetAnodeMember($log_anode, 'index');
    &test_info($test, $name, "setting index to point to log\n");
    &Set_AnodeBlock($ablock, $aoff, 'index', $log_index);
}

#
# ABlk4. Index must identify the anode that contains this block
#
# Repair action: reinitialize block
#
sub test_ablk4 {
    local($this_test) = 'ablk4';
    &print_start_message($this_test, 'Try using index of wrong anode');
    local($errors) = &test_ablock($backing_FS, $backing_FS_name,
	$this_test . 'a', *break_ablk4, *check_ablk_cleared);
    $errors += &test_ablock($efs'AVL_index, 'AVL',
	$this_test . 'b', *break_ablk4, *check_ablk_cleared);
    &print_end_message($this_test, $errors);
}

sub break_ablk4 {
    local($test, $ablock, $aoff, $name) = @_;
    &test_info($test, $name, "setting index to point to different volume\n");
    &Set_AnodeBlock($ablock, $aoff, 'index', $cow_FS);
}

#
# ABlk5. Initial and final time stamps must be equal.
#
# Repair action: make them so.
#
sub test_ablk5 {
    local($this_test) = 'ablk5';
    &print_start_message($this_test, 'Try mismatched time stamps');
    local($errors) = &test_ablock($backing_FS, $backing_FS_name,
	$this_test . 'a', *break_ablk5, *check_ablk5);
    $errors += &test_ablock($efs'AVL_index, 'AVL',
	$this_test . 'b', *break_ablk5, *check_ablk5);
    &print_end_message($this_test, $errors);
}

sub break_ablk5 {
    local($test, $ablock, $aoff, $name) = @_;
    &test_info($test, $name, "setting initial time stamp to zero\n");
    &Set_AnodeBlock($ablock, $aoff, 'time', 0);
}

sub check_ablk5 {
    local($old, $new, $fs_name, $fs_index) = @_;
    &check_ablk_timestamps($new, $fs_name, $fs_index);
}

#
# Check that the anode block is reinitialized.
#
sub check_ablk_cleared {
    local($old, $new, $fs_name, $fs_index) = @_;

    local($omagic) = &efs'GetAnodeBlockMember($old, 'magic');
    local($nmagic) = &efs'GetAnodeBlockMember($new, 'magic');
    return &salverr($fs_name, $fs_index, "bad magic number %#x\n", $nmagic)
	unless ($omagic == $nmagic);

    local($nindex) = &efs'GetAnodeBlockMember($new, 'index');
    return &salverr($fs_name, $fs_index, "bad index %#x\n", $nindex)
	unless ($fs_index == $nindex);

    return 1 if &check_ablk_timestamps($new, $fs_name, $fs_index);

    local($i);
    local($first) = ($fs_index == $efs'AVL_index) ? 1 : 2;
    local($anode, $aoff, $flags);
    for ($i = $first; $i != $efs'anodes_per_block; $i++) {
	($anode, $aoff) = &efs'ExtractAnodeFromBlock($new, $i);
	$flags = &efs'GetAnodeMember($anode, 'flags');
	next if ($flags == 0);
	return &salverr($fs_name, $fs_index,
		"anode %d: flags %#x, expected 0x0\n", $i, $flags);
    }
    return 0;
}

sub check_ablk_timestamps {
    local($ablock, $fs_name, $fs_index) = @_;
    local($istamp) = &efs'GetAnodeBlockMember($ablock, 'time');
    local($fstamp) = unpack('L', substr($ablock, $efs'blockSize - 4));
    return 0 if ($istamp == $fstamp);
    &salverr($fs_name, $fs_index,
	"initial and final timestamps disagree: %#x, %#x\n", $istamp, $fstamp)
}

1;
