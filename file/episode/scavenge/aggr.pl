# Copyright (C) 1993 Transarc Corporation - All rights reserved
#
# Aggregate tests for salvager test suite
#

##
## Superblock tests
##

#
# Test salvager exit status to see whether or not salvager claims
# to have completed successfully.
#
# XXX Assumption here is that exit status < 8 indicates 'normal'
# completion, >= 8 means that it gave up or dumped core; need to
# confirm that this is really true.
#
sub SalvageFailed {
    local($status) = @_;
    return ($status >= 8);
}

#
# Break a consistency rule in the superblock and run the salvager.
#
sub test_superblock {
    local($test_name, *break_rule, *check_repair) = @_;
    local(@result) = &FindObject(*GetAggr, 1);
    return 1 unless (@result);

    &UseTmpAggr(1);

    local($sblock, $offset) = @result[$#result - 1 .. $#result];
    &break_rule($test_name, $sblock, $offset);

    local($status) = &RunSalvager();

    @result = &FindObject(*GetAggr, 1);
    return 1 unless (@result);

    local($new_sblock) = $result[$#result - 1];
    &check_repair($sblock, $new_sblock, &SalvageFailed($status));
}

#
# Check whether the salvager gave up or not.
#
sub check_punted {
    local($old, $new, $index, $failed) = @_;
    return 0 if $failed;
    &salverr('superblock', $index, "salvager repairs unexpected\n");
}

#
# SB1. Magic number must be AVL_MAGIC.
#
# Repair action: give up.
#
sub test_sblk1 {
    local($this_test) = 'sblk1';
    &print_start_message($this_test, 'Try bad superblock magic number');
    local($errors) = &test_superblock($this_test, *break_sblk1, *check_punted);
    &print_end_message($this_test, $errors);
}

sub break_sblk1 {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting magic number to zero\n");
    &Set_Aggr($sblock, $soff, 'magic', 0);
}
#
# SB2. diskFormatMinorVersion <= 2
#
# Repair action: give up unless forced to continue.
#
sub test_sblk2 {
    local($this_test) = 'sblk2';
    &print_start_message($this_test, 'Try bad minor version number');
    local($errors) = &test_superblock($this_test . 'a',
	*break_sblk2a, *check_punted);
    $errors += &test_superblock($this_test . 'b',
	*break_sblk2b, *check_punted);
    &print_end_message($this_test, $errors);
}

sub break_sblk2a {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting version number to three\n");
    &Set_Aggr($sblock, $soff, 'diskFormatMinorVersion', 3);
}

sub break_sblk2b {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting version number to zero\n");
    &Set_Aggr($sblock, $soff, 'diskFormatMinorVersion', 0);
}

#
# SB3. Block and fragment relationships:
#	1) block and fragment sizes are powers of two
#	2) 1K <= fragment size <= blocksize <= 64K
#
# Repair action: give up.
#
sub test_sblk3 {
    local($this_test) = 'sblk3';
    &print_start_message($this_test, 'Try bad block and fragment sizes');
    local($errors) = &test_superblock($this_test . 'a',
	*break_sblk3a, *check_punted);
    $errors += &test_superblock($this_test . 'b', *break_sblk3b, *check_punted);
    $errors += &test_superblock($this_test . 'c', *break_sblk3c, *check_punted);
    $errors += &test_superblock($this_test . 'd', *break_sblk3d, *check_punted);
    $errors += &test_superblock($this_test . 'e', *break_sblk3e, *check_punted);
    $errors += &test_superblock($this_test . 'f', *break_sblk3f, *check_punted);
    &print_end_message($this_test, $errors);
}

sub break_sblk3a {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting block size to non-power of two\n");
    &Set_Aggr($sblock, $soff, 'blockSize', 8193);
}

sub break_sblk3b {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting frag size to non-power of two\n");
    &Set_Aggr($sblock, $soff, 'fragmentSize', 1023);
}

sub break_sblk3c {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting block size too small\n");
    &Set_Aggr($sblock, $soff, 'blockSize', 512);
}

sub break_sblk3d {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting frag size too small\n");
    &Set_Aggr($sblock, $soff, 'fragmentSize', 0);
}

sub break_sblk3e {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting block and frag sizes too big\n");
    &Set_Aggr($sblock, $soff, 'fragmentSize', 128 * 1024);
    &Set_Aggr($sblock, $soff, 'blockSize', 128 * 1024);
}

sub break_sblk3f {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting frag size larger than block\n");
    &Set_Aggr($sblock, $soff, 'fragmentSize', 2 * $efs'blockSize);
}

#
# SB4. reservedIndexes identifies AVL anode
#
# Repair action: give up.
#
sub test_sblk4 {
    local($this_test) = 'sblk4';
    &print_start_message($this_test, 'Try bad reservedIndexes');
    local($errors) = &test_superblock($this_test, *break_sblk4, *check_punted);
    &print_end_message($this_test, $errors);
}

sub break_sblk4 {
    local($test, $sblock, $soff) = @_;
    local($avl_anode, $avl_off) = &specinfo($efs'CFLAGS_AVL);
    local($flags) = &efs'GetAnodeMember($avl_anode, 'flags');
    local($newflags) = ($flags & ~$efs'AFLAGS_TYPE_MASK) |
	$efs'AFLAGS_TYPE_VOLUME;
    &test_info($test, 'superblock', "changing AVL anode type to volume\n");
    &Set_Anode($avl_anode, $avl_off, 'flags', $newflags);
}

#
# SB5. Bitmap index identifies valid bit map
#
# Repair action: create a new bit map.
#
sub test_sblk5 {
    local($this_test) = 'sblk5';
    &print_start_message($this_test, 'Try bad bitmap index');
    local($errors) = &test_superblock($this_test, *break_sblk5, *check_sblk5);
    &print_end_message($this_test, $errors);
}

sub break_sblk5 {
    local($test, $sblock, $soff) = @_;
    local($log_anode) = &specinfo($efs'CFLAGS_LOG);
    local($log_index) = &efs'GetAnodeMember($log_anode, 'index');
    &test_info($test, 'superblock', "setting bitmapIndex to $log_index\n");
    &Set_Aggr($sblock, $soff, 'bitmapIndex', $log_index);
}

sub check_sblk5 {
    local($old, $new, $index) = @_;
    local($bmap_index) = &efs'GetAggrMember($new, 'bitmapIndex');
    @result = &FindObject(*GetAggr, 1, *GetAVLAnodeFromIndex, $bmap_index);
    return &salverr('superblock', $index,
		"bad bitmap index $bmap_index\n") unless @result;
    local($bmap_anode) = $result[$#result - 1];
    local($flags) = &efs'GetAnodeMember($bmap_anode, 'flags');
    local($type) = ($flags & $efs'AFLAGS_TYPE_MASK);
    return &salverr('superblock', 1,
	"bad bitmap anode type %#x\n", $type >> $efs'AFLAGS_TYPE_SHIFT)
	unless ($type == $efs'AFLAGS_TYPE_BITMAP);
    return 0;
}

#
# SB6. Log index identifies a valid log.
#
# Repair action: create a new log.
#
sub test_sblk6 {
    local($this_test) = 'sblk6';
    &print_start_message($this_test, 'Try bad log index');
    local($errors) = &test_superblock($this_test,
	*break_sblk6, *check_sblk6);
    &print_end_message($this_test, $errors);
}

sub break_sblk6 {
    local($test, $sblock, $soff) = @_;
    local($bmap_anode) = &specinfo($efs'CFLAGS_BITMAP);
    local($bmap_index) = &efs'GetAnodeMember($bmap_anode, 'index');
    &test_info($test, 'superblock', "setting logIndex to $bmap_index\n");
    &Set_Aggr($sblock, $soff, 'logIndex', $bmap_index);
}

sub check_sblk6 {
    local($old, $new, $index) = @_;
    local($log_index) = &efs'GetAggrMember($new, 'logIndex');
    @result = &FindObject(*GetAggr, 1, *GetAVLAnodeFromIndex, $log_index);
    return &salverr('superblock', $index,
		"bad log index $log_index\n") unless @result;
    local($log_anode) = $result[$#result - 1];
    local($flags) = &efs'GetAnodeMember($log_anode, 'flags');
    local($type) = ($flags & $efs'AFLAGS_TYPE_MASK);
    return &salverr('superblock', 1,
	"bad log anode type %#x\n", $type >> $efs'AFLAGS_TYPE_SHIFT)
	unless ($type == $efs'AFLAGS_TYPE_LOG);
    return 0;
}

#
# SB7. Badblock index identifies a valid bad block container.
#
# Repair action: create a new log.
#
sub test_sblk7 {
    local($this_test) = 'sblk7';
    &print_start_message($this_test, 'Try invalid bblock index');
    local($errors) = &test_superblock($this_test, *break_sblk7, *check_sblk7);
    &print_end_message($this_test, $errors);
}

sub break_sblk7 {
    local($test, $sblock, $soff) = @_;
    local($bmap_anode) = &specinfo($efs'CFLAGS_BITMAP);
    local($bmap_index) = &efs'GetAnodeMember($bmap_anode, 'index');
    &test_info($test, 'superblock', "setting logIndex to $bmap_index\n");
    &Set_Aggr($sblock, $soff, 'badBlocksIndex', $bmap_index);
}

sub check_sblk7 {
    local($old, $new, $index) = @_;
    local($bb_index) = &efs'GetAggrMember($new, 'badBlocksIndex');
    @result = &FindObject(*GetAggr, 1, *GetAVLAnodeFromIndex, $bb_index);
    return &salverr('superblock', $index,
		"bad bblock index $bb_index\n") unless @result;
    local($bb_anode) = $result[$#result - 1];
    local($flags) = &efs'GetAnodeMember($bb_anode, 'flags');
    local($type) = ($flags & $efs'AFLAGS_TYPE_MASK);
    return &salverr('superblock', $index,
	"bad log anode type %#x\n", $type >> $efs'AFLAGS_TYPE_SHIFT)
	unless ($type == $efs'AFLAGS_TYPE_BADBLOCKS);
    return 0;
}

#
# SB8. 0 < nLogBlocks < nBlocks
#
# Repair action: set nLogBlocks to length of log container; give up
# if still invalid.
#
sub test_sblk8 {
    local($this_test) = 'sblk8';
    &print_start_message($this_test, 'Try invalid log size');
    local($errors) = &test_superblock($this_test . 'a',
	*break_sblk8a, *check_sblk8);
    $errors += &test_superblock($this_test . 'b', *break_sblk8b, *check_sblk8);
    &print_end_message($this_test, $errors);
}

sub break_sblk8a {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting nLogBlocks too small\n");
    &Set_Aggr($sblock, $soff, 'nLogBlocks', 0);
}

sub break_sblk8b {
    local($test, $sblock, $soff) = @_;
    local($nblocks) = &efs'GetSuperMember('nBlocks');
    &test_info($test, 'superblock', "setting nLogBlocks too large\n");
    &Set_Aggr($sblock, $soff, 'nLogBlocks', $nblocks);
}

sub check_sblk8 {
    local($old, $new, $index) = @_;
    local($log_anode) = &specinfo($efs'CFLAGS_LOG);
    local($log_len) = &efs'GetAnodeMember($log_anode, 'length2');
    local($log_blks) = $log_len / $efs'blockSize;
    local($nlogblks) = &efs'GetAggrMember($new, 'nLogBlocks');
    return 0 if ($log_blks == $nlogblks);
    &salverr('superblock', $index,
	"nLogBlocks is $nlogblks, expected $log_blks\n");
}

#
# SB9. firstBlock <= 64K / blockSize
#
# Repair action: set equal to 64K / blockSize
#
sub test_sblk9 {
    local($this_test) = 'sblk9';
    &print_start_message($this_test, 'Try invalid firstBlock');
    local($errors) = &test_superblock($this_test, *break_sblk9, *check_sblk9);
    &print_end_message($this_test, $errors);
}

sub break_sblk9 {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting firstBlock too large\n");
    &Set_Aggr($sblock, $soff, 'firstBlock', 1 + (65536 / $efs'blockSize));
}

sub check_sblk9 {
    local($old, $new, $index) = @_;
    local($firstblk) = &efs'GetAggrMember($new, 'firstBlock');
    local($expected) = &efs'GetAggrMember($old, 'firstBlock');
    return 0 if ($firstblk == $expected);
    &salverr('superblock', $index,
	"firstBlock is $firstblk, expected $expected\n");
}

#
# SB10.  firstBlock + nBlocks must not exceed partition size.
#
# Repair action: adjust nBlocks to maximum permitted value.
#
# XXX We just pick an arbitrary large value, then see if the salvager
# makes it smaller.
#
sub test_sblk10 {
    local($this_test) = 'sblk10';
    &print_start_message($this_test, 'Try invalid nBlocks');
    local($errors) = &test_superblock($this_test,
	*break_sblk10, *check_sblk10);
    &print_end_message($this_test, $errors);
}

sub break_sblk10 {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting nBlocks too large\n");
    &Set_Aggr($sblock, $soff, 'nBlocks', 0x7fffffff);
}

sub check_sblk10 {
    local($old, $new, $index) = @_;
    local($nblk) = &efs'GetAggrMember($new, 'nBlocks');
    local($onblk) = &efs'GetAggrMember($old, 'nBlocks');
    return 0 if ($onblk < $nblk && $nblk < 0x7fffffff);
    &salverr('superblock', $index,
	"unexpected nBlocks value $nblk\n");
}

#
# SB11. Various fixed values:
#		1) flags == 0
#		2) reservedIndexes == 1
#		3) spares contain 0
#
# Repair action: set to correct values
#
# XXX Not checking spares yet
#
sub test_sblk11 {
    local($this_test) = 'sblk11';
    &print_start_message($this_test, 'Try changing fixed header values');
    local($errors) = &test_superblock($this_test . 'a',
	*break_sblk11a, *check_sblk11);
    $errors += &test_superblock($this_test . 'b',
	*break_sblk11b, *check_sblk11);
    &print_end_message($this_test, $errors);
}

sub break_sblk11a {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting flags nonzero\n");
    &Set_Aggr($sblock, $soff, 'flags', 42);
}

sub break_sblk11b {
    local($test, $sblock, $soff) = @_;
    local($log_anode) = &specinfo($efs'CFLAGS_LOG);
    local($log_index) = &efs'GetAnodeMember($log_anode, 'index');
    &test_info($test, 'superblock', "setting reservedIndexes to bad value\n");
    &Set_Aggr($sblock, $soff, 'reservedIndexes', $log_index);
}

sub check_sblk11 {
    local($old, $new, $index) = @_;
    local($reserved) = &efs'GetAggrMember($new, 'reservedIndexes');
    local($flags) = &efs'GetAggrMember($new, 'flags');
    return 0 if ($reserved == 1 && $flags == 0);
    return &salverr('superblock', $index,
	"reservedIndexes is $reserved, expected 1\n") unless ($reserved == 1);
    &salverr('superblock', $efs'AVL_index, "flags is $flags, expected 0\n");
}

#
# Sblk12. creatingProgram should be in 1..3, corresponding to newaggr,
# testanode, and testvnops.
#
# Repair action: none, just issue warning.
#
sub test_sblk12 {
    local($this_test) = 'sblk12';
    &print_start_message($this_test, 'Try invalid creatingProgram');
    local($errors) = &test_superblock($this_test . 'a',
	*break_sblk12a, *check_sblk12);
    $errors += &test_superblock($this_test . 'b',
	*break_sblk12b, *check_sblk12);
    &print_end_message($this_test, $errors);
}

sub break_sblk12a {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock', "setting creatingProgram too large\n");
    &Set_Aggr($sblock, $soff, 'creatingProgram', 73);
}

sub break_sblk12b {
    local($test, $sblock, $soff) = @_;
    &test_info($test, 'superblock',
	"setting creatingProgram to testanode\n");
    &Set_Aggr($sblock, $soff, 'creatingProgram', 2);
}

sub check_sblk12 {
    return 0;
}

sub test_special_anode {
    local($testname, *break_rule, *check_repair, $type) = @_;
    local($spec_anode, $spec_off) = &specinfo($type);
    local($spec_index) = &efs'GetAnodeMember($spec_anode, 'index');

    &UseTmpAggr(1);
    &break_rule($testname, $spec_anode, $spec_off);
    local($status) = &RunSalvager();
    local(@anode) = &FindObject(
	*GetAggr, 1, *GetAVLAnodeFromIndex, $spec_index);
    return 1 unless (@anode);

    $new = $anode[$#anode - 1];
    &check_repair($spec_anode, $new, $spec_index, &SalvageFailed($status));
}

sub check_salvage_punt {
    local($old, $new, $index, $failed) = @_;
    return 0 if $failed;
    local($name) = &specname($old);
    &salverr($name, $index, "salvager repairs unexpected\n");
}

##
## AVL anode tests
##

sub test_AVL {
    &test_special_anode(@_, $efs'CFLAGS_AVL);
}

#
# AVL1. The AVL anode has the LOGGED and PREALLOCATED bits set.
#
# repair action: set these bits correctly.
#

sub test_AVL1 {
    local($this_test) = 'AVL1';
    &print_start_message($this_test,
	'Try clearing LOGGED and PREALLOCATED bits');
    local($errors) = &test_AVL($this_test . 'a', *break_AVL1a, *check_AVL1);
    $errors += &test_AVL($this_test . 'b', *break_AVL1b, *check_AVL1);
    &print_end_message($this_test, $errors);
}

sub break_AVL1a {
    local($test, $avl, $avloff) = @_;
    local($flags) = &efs'GetAnodeMember($avl, 'flags');
    &test_info($test, 'AVL anode', "clearing LOGGED bit\n");
    &Set_Anode($avl, $avloff, 'flags', $flags & ~$efs'AFLAGS_LOGGED);
}

sub break_AVL1b {
    local($test, $avl, $avloff) = @_;
    local($flags) = &efs'GetAnodeMember($avl, 'flags');
    &test_info($test, 'AVL anode', "clearing PREALLOCATED bit\n");
    &Set_Anode($avl, $avloff, 'flags', $flags & ~$efs'AFLAGS_PREALLOCATED);
}

sub check_AVL1 {
    &anode_flags_diff(@_, 'AVL anode');
}

#
# AVL2. Each logical block address must identify an anode block with
# magic number ANODE_BLOCK_AVL_MAGIC and volIndex == reservedIndexes.
#
# repair action: give up.
#
sub test_AVL2 {
    local($this_test) = 'AVL2';
    &print_start_message($this_test, 'Try bad AVL data');
    local($errors) = &test_AVL($this_test . 'a', *break_AVL2a, *check_salvage_punt);
    $errors += &test_AVL($this_test . 'b', *break_AVL2b, *check_salvage_punt);
    $errors += &test_AVL($this_test . 'c', *break_AVL2c, *check_salvage_punt);
    $errors += &test_AVL($this_test . 'd', *break_AVL2d, *check_salvage_punt);
    &print_end_message($this_test, $errors);
}

sub break_AVL2a {
    local($test, $avl, $avloff) = @_;
    &test_info($test, 'AVL anode', "setting empty block address\n");
    &Set_Anode($avl, $avloff, 'dBlock[0]', $efs'ANODE_BLOCKEMPTY);
}

sub break_AVL2b {
    local($test, $avl, $avloff) = @_;
    local($baddr) = &efs'GetAnodeMember($avl, 'dBlock[0]');
    &test_info($test, 'AVL anode', "setting COW bit in block address\n");
    &Set_Anode($avl, $avloff, 'dBlock[0]', $baddr | $efs'ANODE_BLOCKBACKING);
}

sub break_AVL2c {
    local($test, $avl, $avloff) = @_;
    local(@input) = ($avl, $avloff);
    local(@result) = &GetAnodeBlock(*input, 1);
    local($ablock, $aoff) = @result[$#result - 1 .. $#result];

    &test_info($test, 'AVL anode', "trash dBlock[0] magic number\n");
    &Set_AnodeBlock($ablock, $aoff, 'magic', $efs'ABH_VOLUME_MAGIC);
}

sub break_AVL2d {
    local($test, $avl, $avloff) = @_;
    local(@input) = ($avl, $avloff);
    local(@result) = &GetAnodeBlock(*input, 1);
    local($ablock, $aoff) = @result[$#result - 1 .. $#result];

    &test_info($test, 'AVL anode', "trash dBlock[0] reservedIndexes\n");
    &Set_AnodeBlock($ablock, $aoff, 'index', 3);
}

#
# AVL3. The length of the AVL must be a whole number of blocks.
#
# repair action: recompute length from block addresses
#
sub test_AVL3 {
    local($this_test) = 'AVL3';
    &print_start_message($this_test, 'Try invalid lengths');
    local($errors) = &test_AVL($this_test . 'a', *break_AVL3a, *check_AVL3);
    $errors += &test_AVL($this_test . 'b', *break_AVL3b, *check_AVL3);
    $errors += &test_AVL($this_test . 'c', *break_AVL3c, *check_AVL3);
    &print_end_message($this_test, $errors);
}

sub break_AVL3a {
    local($test, $avl, $avloff) = @_;
    &test_info($test, 'AVL anode', "setting length to 0\n");
    &Set_Anode($avl, $avloff, 'length2', 0);
}

sub break_AVL3b {
    local($test, $avl, $avloff) = @_;
    local($length) = &efs'GetAnodeMember($avl, 'length2');
    &test_info($test, 'AVL anode', "setting length too small\n");
    &Set_Anode($avl, $avloff, 'length2', $length - 1);
}

sub break_AVL3c {
    local($test, $avl, $avloff) = @_;
    local($length) = &efs'GetAnodeMember($avl, 'length2');
    &test_info($test, 'AVL anode', "setting length too large\n");
    &Set_Anode($avl, $avloff, 'length2', $length + $efs'blockSize);
}

sub check_AVL3 {
    local($old, $new, $index) = @_;
    &anode_diff($old, $new, 'AVL anode', $index, 'length2');
}

#
# AVL4. The first logical block address must be (64K / blockSize).
#
# repair action: set to correct value.
#

sub test_AVL4 {
    local($this_test) = 'AVL4';
    &print_start_message($this_test, 'Try bad dBlock[0]');
    local($errors) = &test_AVL($this_test . 'a', *break_AVL4a, *check_AVL4);
    $errors += &test_AVL($this_test . 'b', *break_AVL4b, *check_AVL4);
    &print_end_message($this_test, $errors);
}

sub break_AVL4a {
    local($test, $avl, $avloff) = @_;
    &test_info($test, 'AVL anode', "setting dBlock[0] to 0\n");
    &Set_Anode($avl, $avloff, 'dBlock[0]', 0);
}

sub break_AVL4b {
    local($test, $avl, $avloff) = @_;
    local($baddr) = &efs'GetAnodeMember($avl, 'dBlock[0]');
    &test_info($test, 'AVL anode', "setting dBlock[0] too large\n");
    &Set_Anode($avl, $avloff, 'dBlock[0]', $baddr + 1);
}

sub check_AVL4 {
    local($old, $new, $index) = @_;
    &anode_diff($old, $new, 'AVL anode', $index, 'dBlock[0]');
}

#
# AVL5. The AVL data should contain exactly one anode of each special
# type (with the exception of volumes).
#
# repair action: free any special non-volume anodes that are not
# identified in the superblock.
#
sub test_AVL5 {
    local($this_test) = 'AVL5';
    &print_start_message($this_test, 'Try duplicating special anodes');
    local($errors) = &test_AVL($this_test . 'a', *break_AVL5a, *check_AVL5);
    $errors += &test_AVL($this_test . 'b', *break_AVL5b, *check_AVL5);
    $errors += &test_AVL($this_test . 'c', *break_AVL5c, *check_AVL5);
    $errors += &test_AVL($this_test . 'd', *break_AVL5d, *check_AVL5);
    &print_end_message($this_test, $errors);
}

sub break_AVL5a {
    &break_AVL5_common(@_, $efs'CFLAGS_AVL);
}

sub break_AVL5b {
    &break_AVL5_common(@_, $efs'CFLAGS_BITMAP);
}

sub break_AVL5c {
    &break_AVL5_common(@_, $efs'CFLAGS_LOG);
}

sub break_AVL5d {
    &break_AVL5_common(@_, $efs'CFLAGS_BADBLOCKS);
}

sub break_AVL5_common {
    local($test, $avl, $avloff, $vtype) = @_;
    local($victim) = &specinfo($vtype);
    local($vname) = &specname($victim);
    &test_info($test, 'AVL anode', "duplicating $vname anode\n");
    &duplicate_anode($avl, $avloff, $victim);
}

sub check_AVL5 {
    local($old, $new, $index) = @_;
    local($anode) = &efs'AnodeGet(*AGGR, $saved_index, $new);
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    return 0 if ($flags == 0);
    local($name) = 'duplicate ' . &specname($old);
    &salverr($name, $index, "expected flags == 0, got %#x\n", $flags);
}

#
# Read an anode block page, find a free slot, and initialize it with
# the specified anode data.  Assumes that a free slot exists.
#
sub duplicate_anode {
    local($avl, $avloff, $anode) = @_;
    local(@input) = ($avl, $avloff);
    local(@result) = &GetAnodeBlock(*input, 1);
    local($ablock, $aboff) = @result[$#result - 1 .. $#result];
    local($i, $a, $aoff, $flags);

    for ($i = 0; $i != $efs'anodes_per_block; $i++) {
	($a, $aoff) = &efs'ExtractAnodeFromBlock($ablock, $i);
	$flags = &efs'GetAnodeMember($a, 'flags');
	next unless ($flags == 0);
	$aoff += $aboff;
	$saved_index = $i;
	&WriteData($anode, $aoff);
	&Set_Anode($anode, $aoff, 'index', $i);
        return ();
    }
    warn("duplicate_anode: anode block had no free slots");
}

##
## Bitmap tests
##

sub test_bitmap {
    &test_special_anode(@_, $efs'CFLAGS_BITMAP);
}

#
# XXX We always get the first page.  This is fine for now because
# the aggregate that we test has only a one-page bitmap anyway.
# For more rigor, we might want to test multi-page bitmaps later.
#
sub get_bitmap_page {
    local(@input) = @_;
    local(@result) = &GetBitmapPage(*input, 1);
    return @result[$#result - 1, $#result];
}

#
# BMap1.  The bitmap anode has the LOGGED and PREALLOCATED bits set.
#
# repair action: set these bits.
#
sub test_bmap1 {
    local($this_test) = 'bmap1';
    &print_start_message($this_test,
	'Try clearing LOGGED and PREALLOCATED bits');
    local($errors) = &test_bitmap($this_test . 'a',
	*break_bmap1a, *check_bmap1);
    $errors += &test_bitmap($this_test . 'b', *break_bmap1b, *check_bmap1);
    &print_end_message($this_test, $errors);
}

sub break_bmap1a {
    local($test, $bmap, $boff) = @_;
    local($flags) = &efs'GetAnodeMember($bmap, 'flags');
    &test_info($test, 'bitmap', "clearing LOGGED bit\n");
    &Set_Anode($bmap, $boff, 'flags', $flags & ~$efs'AFLAGS_LOGGED);
}

sub break_bmap1b {
    local($test, $bmap, $boff) = @_;
    local($flags) = &efs'GetAnodeMember($bmap, 'flags');
    &test_info($test, 'bitmap', "clearing PREALLOCATED bit\n");
    &Set_Anode($bmap, $boff, 'flags', $flags & ~$efs'AFLAGS_PREALLOCATED);
}

sub check_bmap1 {
    &anode_flags_diff(@_, 'bitmap');
}

#
# BMap2.  The bitmap's data consists of bitmap pages with the appropriate
# magic number.
#
# repair action: replace invalid block addresses with new blocks; initialize
# invalid blocks.
#
sub test_bmap2 {
    local($this_test) = 'bmap2';
    &print_start_message($this_test, 'Try invalid bitmap data');
    local($errors) = &test_bitmap($this_test . 'a',
	*break_bmap2a, *check_bmap2);
    $errors += &test_bitmap($this_test . 'b', *break_bmap2b, *check_bmap2);
    $errors += &test_bitmap($this_test . 'c', *break_bmap2c, *check_bmap2);
    &print_end_message($this_test, $errors);
}

sub break_bmap2a {
    local($test, $bmap, $boff) = @_;
    &test_info($test, 'bitmap', "replacing data with empty block address\n");
    &Set_Anode($bmap, $boff, 'dBlock[0]', $efs'ANODE_BLOCKEMPTY);
}

sub break_bmap2b {
    local($test, $bmap, $boff) = @_;
    local($baddr) = &efs'GetAnodeMember($bmap, 'dBlock[0]');
    &test_info($test, 'bitmap', "setting COW bit in block address\n");
    &Set_Anode($bmap, $boff, 'dBlock[0]', $baddr | $efs'ANODE_BLOCKBACKING);
}

sub break_bmap2c {
    local($test, $bmap, $boff) = @_;
    ($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    &test_info($test, 'bitmap', "setting magic number to bad value\n");
    &Set_BitmapPage($bpage, $bpoff, 'magic', 23);
}

sub check_bmap2 {
    local($old, $new, $index) = @_;
    local($baddr) = &efs'GetAnodeMember($new, 'dBlock[0]');
    return &salverr('bitmap', $index, "bad block address %#x\n", $baddr)
	unless &efs'IsValidBlockAddr($baddr);
    return 0 if (&get_bitmap_page($new, 0));
    &salverr('bitmap', $index, "invalid bitmap page at dBlock[0] (%#x)\n",
	$baddr);
}

#
# BMap3. Per-page conditions:
#	1) initialStamp == finalStamp
#	2) fragmentsPerBlock == (blockSize / fragmentSize) from superblock
#	3) blocksPerPage == 8 * ((blockSize - BITMAP_PAGE_OVERHEAD) / bfract)
#	    where bfract == (fragmentsPerBlock == 1) ?
#		1 : (1 + fragmentsPerBlock)
#	4) All spares zero
#
# repair action: set to correct values
#
sub test_bmap3 {
    local($this_test) = 'bmap3';
    &print_start_message($this_test, 'Try invalid page header data');
    local($errors) = &test_bitmap($this_test . 'a',
	*break_bmap3a, *check_bmap3);
    $errors += &test_bitmap($this_test . 'b', *break_bmap3b, *check_bmap3);
    $errors += &test_bitmap($this_test . 'c', *break_bmap3c, *check_bmap3);
    $errors += &test_bitmap($this_test . 'd', *break_bmap3d, *check_bmap3);
    $errors += &test_bitmap($this_test . 'e', *break_bmap3e, *check_bmap3);
    $errors += &test_bitmap($this_test . 'f', *break_bmap3f, *check_bmap3);
    &print_end_message($this_test, $errors);
}

sub break_bmap3a {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    $saved_bpage = $bpage;
    &test_info($test, 'bitmap', "setting initialStamp to 0\n");
    &Set_BitmapPage($bpage, $bpoff, 'initialStamp', 0);
}

sub break_bmap3b {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    local($fpb) = $efs'blockSize / $efs'fragSize;
    $saved_bpage = $bpage;
    &test_info($test, 'bitmap', "setting fragmentsPerBlock too high\n");
    &Set_BitmapPage($bpage, $bpoff, 'fragmentsPerBlock', $fpb + 1);
}

sub break_bmap3c {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    local($fpb) = $efs'blockSize / $efs'fragSize;
    $saved_bpage = $bpage;
    &test_info($test, 'bitmap', "setting fragmentsPerBlock too high\n");
    &Set_BitmapPage($bpage, $bpoff, 'fragmentsPerBlock', $fpb - 1);
}

sub break_bmap3d {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    $saved_bpage = $bpage;
    local($bpp) =  &efs'GetBitmapPageMember($bpage, 'blocksPerPage');
    &test_info($test, 'bitmap', "setting blocksPerPage too high\n");
    &Set_BitmapPage($bpage, $bpoff, 'blocksPerPage', $bpp + 1);
}

sub break_bmap3e {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    $saved_bpage = $bpage;
    local($bpp) =  &efs'GetBitmapPageMember($bpage, 'blocksPerPage');
    &test_info($test, 'bitmap', "setting blocksPerPage too low\n");
    &Set_BitmapPage($bpage, $bpoff, 'blocksPerPage', $bpp - 1);
}

sub break_bmap3f {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    $saved_bpage = $bpage;
    &test_info($test, 'bitmap', "setting spares nonzero\n");
    &Set_BitmapPage($bpage, $bpoff, 'spare16', 1234);
    &Set_BitmapPage($bpage, $bpoff, 'spare32', 9999);
}

sub check_bmap3 {
    local($old, $new, $index) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($new, 0);
    return 0 if &equal_objects($bpage, $saved_bpage, 4, -4);
    &salverr('bitmap', $index, "header values not correctly restored\n");
}

#
# BMap4. bitmapLength (in first and last pages)  == nBlocks in superblock
#
# repair action: make bitmapLength agree with nBlocks
#
sub test_bmap4 {
    local($this_test) = 'bmap4';
    &print_start_message($this_test, 'Try invalid bitmapLength');
    local($errors) = &test_bitmap($this_test . 'a',
	*break_bmap4a, *check_bmap4);
    $errors += &test_bitmap($this_test . 'b', *break_bmap4b, *check_bmap4);
    &print_end_message($this_test, $errors);
}

sub break_bmap4a {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    local($length) = &efs'GetBitmapPageMember($bmap, 'bitmapLength');
    $saved_length = $length;
    &test_info($test, 'bitmap', "decrementing bitmapLength\n");
    &Set_BitmapPage($bpage, $bpoff, 'bitmapLength', $length - 1);
}

sub break_bmap4b {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    local($length) = &efs'GetBitmapPageMember($bmap, 'bitmapLength');
    $saved_length = $length;
    &test_info($test, 'bitmap', "incrementing bitmapLength\n");
    &Set_BitmapPage($bpage, $bpoff, 'bitmapLength', $length + 1);
}

sub check_bmap4 {
    local($old, $new, $index) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($new, 0);
    local($length) = &efs'GetBitmapPageMember($new, 'bitmapLength');
    return ($length != $saved_length);
}

#
# BMap5. bitmap anode length == howmany(nBlocks, blocksPerPage);
#
# repair action: set to correct value
#
sub test_bmap5 {
    local($this_test) = 'bmap5';
    &print_start_message($this_test, 'Try invalid bitmap anode length');
    local($errors) = &test_bitmap($this_test . 'a',
	*break_bmap5a, *check_bmap5);
    $errors += &test_bitmap($this_test . 'b', *break_bmap5b, *check_bmap5);
    &print_end_message($this_test, $errors);
}

sub break_bmap5a {
    local($test, $bmap, $boff) = @_;
    local($length) = &efs'GetAnodeMember($bmap, 'length2');
    &test_info($test, 'bitmap', "decreasing bitmap anode length\n");
    &Set_Anode($bmap, $boff, 'length2', $length / 2);
}

sub break_bmap5b {
    local($test, $bmap, $boff) = @_;
    local($length) = &efs'GetAnodeMember($bmap, 'length2');
    &test_info($test, 'bitmap', "increasing bitmap anode length\n");
    &Set_Anode($bmap, $boff, 'length2', $length + 1);
}

sub check_bmap5 {
    local($old, $new, $index) = @_;
    &anode_diff($old, $new, 'bitmap', $index, 'length2');
}

#
# BMap6. baseOffset == firstBlock (from superblock) on first page; else
#	 baseOffset == baseOffset (from preceding page) + blocksPerPage
#
# repair action: set to correct value
#
sub test_bmap6 {
    local($this_test) = 'bmap6';
    &print_start_message($this_test, 'Try invalid baseOffset');
    local($errors) = &test_bitmap($this_test . 'a',
	*break_bmap6a, *check_bmap6);
    $errors += &test_bitmap($this_test . 'b', *break_bmap6b, *check_bmap6);
    &print_end_message($this_test, $errors);
}

sub break_bmap6a {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    local($base) = &efs'GetBitmapPageMember($bmap, 'baseOffset');
    $saved_base = $base;
    &test_info($test, 'bitmap', "decrementing baseOffset\n");
    &Set_BitmapPage($bpage, $bpoff, 'bitmapLength', $base - 1);
}

sub break_bmap6b {
    local($test, $bmap, $boff) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    local($base) = &efs'GetBitmapPageMember($bmap, 'baseOffset');
    $saved_base = $base;
    &test_info($test, 'bitmap', "incrementing baseOffset\n");
    &Set_BitmapPage($bpage, $bpoff, 'bitmapLength', $base + 1);
}

sub check_bmap6 {
    local($old, $new, $index) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($new, 0);
    local($base) = &efs'GetBitmapPageMember($new, 'baseOffset');
    return ($base != $saved_base);
}

#
# BMap7. A bit in the fragment map is set to one iff the corresponding
# fragment is in use.
#
# repair action: recalculate correct bit values from anodes.
#
sub test_bmap7 {
    local($this_test) = 'bmap7';
    &print_start_message($this_test, 'Try invalid bits bitmap');
    local($errors) = &test_bitmap($this_test . 'a',
	*break_bmap7a, *check_bmap7);
    $errors += &test_bitmap($this_test . 'b', *break_bmap7b, *check_bmap7);
    &print_end_message($this_test, $errors);
}

sub break_bmap7a {
    &break_bmap7_common(@_, '\x00');
}

sub break_bmap7b {
    &break_bmap7_common(@_, '\xff');
}

sub break_bmap7_common {
    local($test, $bmap, $boff, $fill_char) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    $saved_page = $bpage;
    local($mapoff, $mapsize) = &efs'FindFragmentMap($bpage);
    &test_info($test, 'bitmap', "filling bitmap with %#x\n", $fill_char);
    substr($bpage, $mapoff, $mapsize) = $fill_char x $mapsize;
    &WriteData($bpage, $bpoff);
}

sub check_bmap7 {
    local($old, $new, $index) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($new, 0);
    return 0 if &equal_objects($bpage, $saved_bpage, 4, -4);
    &salverr('bitmap', $index, "bitmap values not correctly restored\n");
}

#
# BMap8. The fragments in a block must belong either entirely to LOGGED
# anodes or entirely to non-LOGGED anodes.
#
# repair action: free all non-LOGGED anodes that use the block.
#
sub test_bmap8 {
    local($this_test) = 'bmap8';
    &print_start_message($this_test, 'Try mixing logged and unlogged frags');
    &UseTmpAggr(1);
    &break_bmap8($this_test);
    &RunSalvager();
    local($errors) = &check_bmap8;
    &print_end_message($this_test, $errors);
}

#
# Make a fragmented file share a block with the bad block container
#
sub break_bmap8 {
    local($test) = @_;
    local($bb_anode, $bb_off) = &specinfo($efs'CFLAGS_BADBLOCKS);
    local($bb_block) = &efs'GetAnodeMember($bb_anode, 'd.frags.block');
    local($bb_first) = &efs'GetAnodeMember($bb_anode, 'd.frags.first');
    local($f_anode, $f_off, $f_name) = @frag[0 .. 2];
    local($f_first) = &efs'GetAnodeMember($f_anode, 'd.frags.first');
    local($f_len) = &efs'GetAnodeMember($f_anode, 'd.frags.length');
    &test_info($test, $f_name,
	"sharing block %#x with bad block container\n", $bb_block);
    if ($f_first == $bb_first) {
	$f_first = ($bb_first + 1) % $efs'frags_per_block;
	&Set_Anode($f_anode, $f_off, 'd.frags.first', $f_first);
    }
    if ($f_first + $f_len > $efs'frags_per_block) {
	$f_len = $efs'frags_per_block - $f_first;
	&Set_Anode($f_anode, $f_off, 'd.frags.length', $new_len)
    }
    &Set_Anode($f_anode, $f_off, 'd.frags.block', $bb_block);
}

sub check_bmap8 {
    local($f_anode, $f_offset, $f_name) = @frag[0 ..  2];
    local($index) = &efs'GetAnodeMember($f_anode, 'index');
    local(@fs_info) = &get_fs_info($backing_FS);
    local(@anode) = &GetFSAnodeFromIndex(*fs_info, $index);
    return 1 unless (@anode);
    local($new) = $anode[$#anode - 1];
    &check_anode_free($f_anode, $new, $f_name, $index);
}

#
# BMap9. If a block contains both in-use and free fragments, then the
# corresponding bit of the logged fragment map is set to one iff the
# in-use fragment belonged to a LOGGED anode.
#
# repair action: set the bits to match the in-use fragments.
#
sub test_bmap9 {
    local($this_test) = 'bmap9';
    &print_start_message($this_test, 'Try invalid bits bitmap');
    local($errors) = &test_bitmap($this_test . 'a',
	*break_bmap9a, *check_bmap9);
    $errors += &test_bitmap($this_test . 'b', *break_bmap9b, *check_bmap9);
    &print_end_message($this_test, $errors);
}

sub break_bmap9a {
    &break_bmap9_common(@_, '\x00');
}

sub break_bmap9b {
    &break_bmap9_common(@_, '\xff');
}

sub break_bmap9_common {
    local($test, $bmap, $boff, $fill_char) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($bmap, $boff);
    $saved_page = $bpage;
    local($mapoff, $mapsize) = &efs'FindLoggedFragMap($bpage);
    &test_info($test, 'bitmap', "filling bitmap with %#x\n", $fill_char);
    substr($bpage, $mapoff, $mapsize) = $fill_char x $mapsize;
    &WriteData($bpage, $bpoff);
}

sub check_bmap9 {
    local($old, $new, $index) = @_;
    local($bpage, $bpoff) = &get_bitmap_page($new, 0);
    return 0 if &equal_objects($bpage, $saved_bpage, 4, -4);
    &salverr('bitmap', $index, "bitmap values not correctly restored\n");
}

##
## Bad block container tests
##

sub test_bb {
    &test_special_anode(@_, $efs'CFLAGS_BADBLOCKS);
}

sub get_bblock_frame {
    local(@input) = @_;
    local(@result) = &GetBadblockFrame(*input, 1);
    return @result[$#result - 1, $#result];
}

sub get_bblock_entry {
    local(@input) = @_;
    local(@result) = &GetBadblockEntry(*input, 1);
    return @result[$#result - 1, $#result];
}

#
# BB1. The bblock anode has the LOGGED and PREALLOCATED bits set.
#
# repair action: set these bits correctly.
#

sub test_bb1 {
    local($this_test) = 'bb1';
    &print_start_message($this_test,
	'Try clearing LOGGED and PREALLOCATED bits');
    local($errors) = &test_bb($this_test . 'a', *break_bb1a, *check_bb1);
    $errors += &test_bb($this_test . 'b', *break_bb1b, *check_bb1);
    &print_end_message($this_test, $errors);
}

sub break_bb1a {
    local($test, $bb, $bboff) = @_;
    local($flags) = &efs'GetAnodeMember($bb, 'flags');
    &test_info($test, 'bad block container', "clearing LOGGED bit\n");
    &Set_Anode($bb, $bboff, 'flags', $flags & ~$efs'AFLAGS_LOGGED);
}

sub break_bb1b {
    local($test, $bb, $bboff) = @_;
    local($flags) = &efs'GetAnodeMember($bb, 'flags');
    &test_info($test, 'bad block container', "clearing PREALLOCATED bit\n");
    &Set_Anode($bb, $bboff, 'flags', $flags & ~$efs'AFLAGS_PREALLOCATED);
}

sub check_bb1 {
    &anode_flags_diff(@_, 'Bad block container');
}

#
# BB2. The bblock anode has length 1K
#
# repair action: set it correctly.
#
sub test_bb2 {
    local($this_test) = 'bb2';
    &print_start_message($this_test, 'Try bad anode length');
    local($errors) = &test_bb($this_test . 'a', *break_bb2a, *check_bb2);
    $errors += &test_bb($this_test . 'b', *break_bb2b, *check_bb2);
    &print_end_message($this_test, $errors);
}

sub break_bb2a {
    &break_bb2_common(@_, 1);
}

sub break_bb2b {
    &break_bb2_common(@_, 2 * $efs'fragSize);
}

sub break_bb2_common {
    local($test, $bb, $bboff, $len) = @_;
    &test_info($test, 'bad block container', "setting length to $len\n");
    &Set_Anode($bb, $bboff, 'length2', $len);
}

sub check_bb2 {
    local($old, $new, $index) = @_;
    &anode_diff($old, $new, 'Bad block container', $index, 'length2');
}

#
# BB3.  Data consists of a bad block frame with magic number BBFRAMEMAGIC
#
# repair action: discard data and reinitialize.
#
# BB4. The bad block frame is followed by a bad block entry satisfying:
#	1) type == bbetSuperBlocks
#	2) thisFrag == 0
#	3) totFrags == 1
#	4) data contains the addresses of alternate superblocks
#
# repair action: discard data and reinitialize.
#
# (Do these two together)
#
sub test_bb3 {
    local($this_test) = 'bb3';
    &print_start_message($this_test, 'Try invalid bblock data');
    local($errors) = &test_bb($this_test . 'a', *break_bb3a, *check_bb3);
    $errors += &test_bb($this_test . 'b', *break_bb3b, *check_bb3);
    $errors += &test_bb($this_test . 'c', *break_bb3c, *check_bb3);
    $errors += &test_bb($this_test . 'd', *break_bb3c, *check_bb3);
    $errors += &test_bb($this_test . 'e', *break_bb3e, *check_bb3);
    $errors += &test_bb($this_test . 'f', *break_bb3f, *check_bb3);
    $errors += &test_bb($this_test . 'g', *break_bb3g, *check_bb3);
    $errors += &test_bb($this_test . 'h', *break_bb3h, *check_bb3);
    $errors += &test_bb($this_test . 'i', *break_bb3i, *check_bb3);
    &print_end_message($this_test, $errors);
}

sub break_bb3a {
    local($test, $bb, $bboff) = @_;
    ($saved_frame, $saved_offset) = &get_bblock_frame($bb, $bboff);
    &test_info($test, 'bad block container',
	"setting fragment block to EMPTY\n");
    &Set_Anode($bb, $bboff, 'd.frags.block', $efs'ANODE_BLOCKEMPTY);
}

sub break_bb3b {
    local($test, $bb, $bboff) = @_;
    local($frame, $froff) = &get_bblock_frame($bb, $bboff);
    $saved_frame = $frame;
    &test_info($test, 'bad block container', "setting magic number to 0\n");
    &Set_BadblockFrame($frame, $froff, 'bbFrameMagic', 0);
}

sub break_bb3c {
    local($test, $bb, $bboff) = @_;
    local($frame, $froff) = &get_bblock_frame($bb, $bboff);
    $saved_frame = $frame;
    local($entry, $eoff) = &get_bblock_entry($frame, $froff);
    &test_info($test, 'bad block container', "setting entry type to 2\n");
    &Set_BadblockEntry($entry, $eoff, 'type', 2);
}

sub break_bb3d {
    local($test, $bb, $bboff) = @_;
    local($frame, $froff) = &get_bblock_frame($bb, $bboff);
    $saved_frame = $frame;
    local($entry, $eoff) = &get_bblock_entry($frame, $froff);
    &test_info($test, 'bad block container', "setting entry type to 9\n");
    &Set_BadblockEntry($entry, $eoff, 'type', 9);
}

sub break_bb3e {
    local($test, $bb, $bboff) = @_;
    local($frame, $froff) = &get_bblock_frame($bb, $bboff);
    $saved_frame = $frame;
    local($entry, $eoff) = &get_bblock_entry($frame, $froff);
    &test_info($test, 'bad block container', "setting thisFrag too large\n");
    &Set_BadblockEntry($entry, $eoff, 'thisFrag', 123);
}

sub break_bb3f {
    local($test, $bb, $bboff) = @_;
    local($frame, $froff) = &get_bblock_frame($bb, $bboff);
    $saved_frame = $frame;
    local($entry, $eoff) = &get_bblock_entry($frame, $froff);
    &test_info($test, 'bad block container', "setting totFrags too small\n");
    &Set_BadblockEntry($entry, $eoff, 'totFrags', 0);
}

sub break_bb3g {
    local($test, $bb, $bboff) = @_;
    local($frame, $froff) = &get_bblock_frame($bb, $bboff);
    $saved_frame = $frame;
    local($entry, $eoff) = &get_bblock_entry($frame, $froff);
    &test_info($test, 'bad block container', "setting totFrags too large\n");
    &Set_BadblockEntry($entry, $eoff, 'totFrags', 65);
}

sub break_bb3h {
    local($test, $bb, $bboff) = @_;
    local($frame, $froff) = &get_bblock_frame($bb, $bboff);
    $saved_frame = $frame;
    local($entry, $eoff) = &get_bblock_entry($frame, $froff);
    local($length) = &efs'GetBadblockEntryMember($entry, 'length');
    local($naddr) = $length / 4;
    local(@addr);
    local($i) = 0;
    $addr[$i++] = $efs'ANODE_BLOCKEMPTY until ($i == $naddr);

    &test_info($test, 'bad block container',
	"seting invalid addresses in data\n");
    &Set_BadblockData(*addr, $eoff);
}

sub break_bb3i {
    local($test, $bb, $bboff) = @_;
    local($frame, $froff) = &get_bblock_frame($bb, $bboff);
    $saved_frame = $frame;
    local($entry, $eoff) = &get_bblock_entry($frame, $froff);
    local($length) = &efs'GetBadblockEntryMember($entry, 'length');
    local($naddr) = $length / 4;
    local($nBlocks) = &efs'GetSuperMember('nBlocks');
    local(@addr);
    local($i) = 0;
    $addr[$i++] = $nBlocks + 100 until ($i == $naddr);
    &test_info($test, 'bad block container',
	"seting invalid addresses in data\n");
    &Set_BadblockData(*addr, $eoff);
}

sub check_bb3 {
    local($old, $new, $index) = @_;
    local($frame, $froff) = &get_bblock_frame($new, 0);
    return &salverr('badblock container', $index, "badblock frame unreadable\n")
	unless ($frame);
    local($entry, $eoff) = &get_bblock_entry($frame, $froff);
    local($length) = &efs'GetBadblockEntryMember($entry, 'length');
    $length += $efs'BB_FRAME_SIZE + $efs'BB_ENTRY_SIZE;
    return 0 if &equal_objects($frame, $saved_frame, 4, $length);
    &salverr('badblock container', $index,
	"badblock frame not correctly restored\n");
}

##
## Log tests
##

sub test_log {
    &test_special_anode(@_, $efs'CFLAGS_LOG);
}

sub get_log_page {
    local(@input) = @_;
    local(@result) = &GetLogPage(*input, 1);
    return @result[$#result - 1, $#result];
}

#
# Log1. The log anode has the PREALLOCATED bit set and the LOGGED bit clear.
#
# repair action: set the bits appropriately.
#
sub test_log1 {
    local($this_test) = 'log1';
    &print_start_message($this_test,
	'Try inverting LOGGED and PREALLOCATED bits');
    local($errors) = &test_log($this_test . 'a', *break_log1a, *check_log1);
    $errors += &test_log($this_test . 'b', *break_log1b, *check_log1);
    &print_end_message($this_test, $errors);
}

sub break_log1a {
    local($test, $log, $loff) = @_;
    local($flags) = &efs'GetAnodeMember($log, 'flags');
    &test_info($test, 'log', "setting LOGGED bit\n");
    &Set_Anode($log, $loff, 'flags', $flags | $efs'AFLAGS_LOGGED);
}

sub break_log1b {
    local($test, $log, $loff) = @_;
    local($flags) = &efs'GetAnodeMember($log, 'flags');
    &test_info($test, 'log', "clearing PREALLOCATED bit\n");
    &Set_Anode($log, $loff, 'flags', $flags & ~$efs'AFLAGS_PREALLOCATED);
}

sub check_log1 {
    local($old, $new, $index) = @_;
    &anode_flags_diff($old, $new, 'log', $index);
}

#
# Log2. The anode length is nLogBlocks file system blocks
#
# repair action: recompute length from file system blocks; update
# nLogBlocks if necessary.
#
sub test_log2 {
    local($this_test) = 'log2';
    &print_start_message($this_test,
	'Try incorrect log length and nLogBlocks count');
    local($errors) = &test_log($this_test . 'a', *break_log2a, *check_log2);
    $errors += &test_log($this_test . 'b', *break_log2b, *check_log2);
    $errors += &test_log($this_test . 'c', *break_log2c, *check_log2);
    $errors += &test_log($this_test . 'd', *break_log2d, *check_log2);
    &print_end_message($this_test, $errors);
}

sub break_log2a {
    local($test, $log, $loff) = @_;
    local($length) = &efs'GetAnodeMember($log, 'length2');
    &test_info($test, 'log', "decreasing log length\n");
    &Set_Anode($log, $loff, 'length2', $length - $efs'blockSize);
}

sub break_log2b {
    local($test, $log, $loff) = @_;
    local($length) = &efs'GetAnodeMember($log, 'length2');
    &test_info($test, 'log', "increasing log length\n");
    &Set_Anode($log, $loff, 'length2', $length + $efs'blockSize);
}

sub break_log2c {
    local($test, $log, $loff) = @_;
    local($nLog) = &efs'GetSuperMember('nLogBlocks');
    &test_info($test, 'log', "decreasing nLogBlocks\n");
    &Set_Aggr($efs'SuperBlock, $efs'SBOffset, 'nLogBlocks', int($nLog / 2));
}

sub break_log2d {
    local($test, $log, $loff) = @_;
    local($nLog) = &efs'GetSuperMember('nLogBlocks');
    &test_info($test, 'log', "increasing nLogBlocks\n");
    &Set_Aggr($efs'SuperBlock, $efs'SBOffset, 'nLogBlocks', 2 * $nLog);
}

sub check_log2 {
    local($old, $new, $index) = @_;

    return 1 if &anode_flags_diff($old, $new, 'log', $index);

    local($nLog) = &efs'GetSuperMember('nLogBlocks');
    local($length) = &efs'GetAnodeMember($new, 'length2');
    return 0 if ($length == $nLog * $efs'blockSize);
    &salverr('log', $index,
	"length ($length) disagrees with nLogBlocks ($nLog)\n");
}

#
# Log3. Each data block begins with a logPageHdr containing salvageID ==
# LOG_SALVAGE_MAGIC_NUMBER.
#
#
# repair action: give up, since recovery phase was supposed to ensure
# that the log is valid.
#
sub test_log3 {
    local($this_test) = 'log3';
    &print_start_message($this_test, 'Try bad salvageID');
    local($errors) = &test_log($this_test, *break_log3, *check_salvage_punt);
    &print_end_message($this_test, $errors);
}

sub break_log3 {
    local($test, $log, $loff) = @_;
    local($log_page, $lpoff) = &get_log_page($log, $loff);
    &test_info($test, 'log', "setting salvageID to zero\n");
    &Set_LogPage($log_page, $lpoff, 'salvageID', 0);
}

1;
