## /* Copyright (C) 1993 Transarc Corporation - All rights reserved. */
## $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/scavenge/RCS/anode.pl,v 1.13 1993/07/12 20:02:42 ota Exp $
##
## Anode tests for salvager test suite
##

#
# Iterate over regular anodes, breaking one of the consistency
# rules; then salvage and iterate again, checking that the correct
# repair was done.
#
sub test_anodes {
    local($fs_index, $testname, *break_rule, *check_repair, *anode_list) = @_;
    local(@fs_info) = &get_fs_info($fs_index);
    local($i, $anode, $offset, $path, $index);
    local($old, $new);
    local(@anode);
    local($errors) = 0;

    &UseTmpAggr(1);
    for ($i = 0; $i <= $#anode_list; $i += 3) {
	$anode = $anode_list[$i];
	$offset = $anode_list[$i + 1];
	$path = $anode_list[$i + 2];
	&break_rule($testname, $anode, $offset, $path);
    }
    &RunSalvager();
    for ($i = 0; $i <= $#anode_list; $i += 3) {
	$old = $anode_list[$i];
	$path = $anode_list[$i + 2];
	$index = &efs'GetAnodeMember($old, 'index');
	@anode = &GetFSAnodeFromIndex(*fs_info, $index);
	if (@anode) {
	    $new = $anode[$#anode - 1];
	    $errors += &check_repair($old, $new, $path, $index);
	}
    }
    return $errors;
}

##
## Generic anode tests
##

#
# Anode1. The anode must have a valid type (1..5 or 0xf).
#
# Repair action: mark anode free.
#
sub test_anode1 {
    local($this_test) = 'anode1';
    &print_start_message($this_test, 'Try invalid anode type');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_anode1, *check_anode_free, *regular);
    &print_end_message($this_test, $errors);
}

sub break_anode1 {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($bad_type) = 6;
    local($newflags) = ($flags & ~$efs'AFLAGS_TYPE_MASK) |
	($bad_type << $efs'AFLAGS_TYPE_SHIFT);
    local($type) = ($flags & $efs'AFLAGS_TYPE_MASK) >> $efs'AFLAGS_TYPE_SHIFT;
    &test_info($test, $name, "changing type from $type to $bad_type\n");
    &Set_Anode($anode, $aoff, 'flags', $newflags);
}

#
# Anode2. index must correspond to the anode's position in the volume
# or AVL that contains it.
#
# Repair action: assign correct value based on location.
#
sub test_anode2 {
    local($this_test) = 'anode2';
    &print_start_message($this_test, 'Try invalid anode index');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_anode2, *check_anode2, *regular);
    &print_end_message($this_test, $errors);
}

sub break_anode2 {
    local($test, $anode, $aoff, $name) = @_;
    local($index) = &efs'GetAnodeMember($anode, 'index');
    local($newindex) = $index + 1;
    &test_info($test, $name, "changing index from $index to $newindex\n");
    &Set_Anode($anode, $aoff, 'index', $newindex);
}

sub check_anode2 {
    &anode_flags_diff(@_) ||
    &anode_diff(@_, 'length2') ||
    &anode_diff(@_, 'index');
}

#
# Anode3. The anode may have at most one of the AFLAGS_DATA_INLINE and
# AFLAGS_DATA_FRAGMENTED bits set.
#
# Repair action: mark anode free.
#
sub test_anode3 {
    local($this_test) = 'anode3';
    &print_start_message($this_test, 'In-line and fragmented bits both set');
    local($errors) = &test_anodes($backing_FS, $this_test,
			*break_anode3, *check_anode_free, *regular);
    &print_end_message($this_test, $errors);
}

sub break_anode3 {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($newflags) = $flags | $efs'AFLAGS_DATA_NOTBLOCKED;
    local($type) = $bitname{$flags & $efs'AFLAGS_DATA_NOTBLOCKED};
    &test_info($test, $name, "is $type, setting inline and frag bits\n");
    &Set_Anode($anode, $aoff, 'flags', $newflags);
}

#
# Anode 4. Status information size may not exceed 156 bytes.
#
# Repair action: mark anode fee
#
sub test_anode4 {
    local($this_test) = 'anode4';
    &print_start_message($this_test, 'Try invalid status size');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_anode4, *check_anode_free, *regular);
    &print_end_message($this_test, $errors);
}

sub break_anode4 {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($bad_status_size) = 157;
    local($newflags) =
	($flags  & ~$efs'AFLAGS_STATUSSIZE_MASK) | $bad_status_size;
    &test_info($test, $name, "setting status size to $bad_status_size\n");
    &Set_Anode($anode, $aoff, 'flags', $newflags);
}

#
# Anode5. If length is zero, neither AFLAGS_DATA_INLINE nor
# AFLAGS_DATA_FRAGMENTED may be set.
#
# Repair action: clear these bits and zero d.blocks[].
#
sub test_anode5 {
    local($this_test) = 'anode5';
    &print_start_message($this_test, 'Zero-length files must be blocked');
    local($errors) =
	&test_anodes($backing_FS, $this_test, *break_anode5, *check_anode5, *zero);
    &print_end_message($this_test, $errors);
}

sub break_anode5 {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($i);
    &test_info($test, $name, "changing zero-length file to inline\n");
    for ($i = 0; $i != $efs'ANODE_DIRECTBLOCKS; $i++) {
	&Set_Anode($anode, $aoff, "dBlock[$i]", 100);
    }
    for ($i = 0; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	&Set_Anode($anode, $aoff, "iBlock[$i]", 99);
    }
    &Set_Anode($anode, $aoff, 'flags', $flags | $efs'AFLAGS_DATA_INLINE);
}

sub check_anode5 {
    local($old, $new, $name, $index) = @_;

    return 1 if &anode_flags_diff(@_) || &anode_diff(@_, 'length2');

    local($i, $block);
    for ($i = 0; $i != $efs'ANODE_DIRECTBLOCKS; $i++) {
	$block = &efs'GetAnodeMember($new, "dBlock[$i]");
	return &salverr($name, $index, "nonempty dBlock[$i] == %#x\n", $block)
	    unless ($block == $efs'ANODE_BLOCKEMPTY);
    }
    for ($i = 0; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	$block = &efs'GetAnodeMember($new, "iBlock[$i]");
	return &salverr($name, $index, "nonempty iBlock[$i] == %#x\n", $block)
	    unless ($block == $efs'ANODE_BLOCKEMPTY);
    }
    return 0;
}

#
# Anode6. The allocated member must agree with the number of allocated
# blocks or fragments.
#
# Repair action: set allocated to match anode storage.
#
sub test_anode6 {
    local($this_test) = 'anode6';
    &print_start_message($this_test, 'Try incorrect allocated frag count');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_anode6, *check_anode6, *regular);
    &print_end_message($this_test, $errors);
}

sub break_anode6 {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting allocated to -1\n");
    &Set_Anode($anode, $aoff, 'allocated', -1);
}

sub check_anode6 {
    &anode_flags_diff(@_) ||
    &anode_diff(@_, 'length2') ||
    &anode_diff(@_, 'allocated');
}

#
# Anode7. Spare fields must be zero.
#
# Repair action: zero them.
#
sub test_anode7 {
    local($this_test) = 'anode7';
    &print_start_message($this_test, 'Try setting spares to nonzero');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_anode7, *check_anode7, *regular);
    &print_end_message($this_test, $errors);
}

sub break_anode7 {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting spares to nonzero values\n");
    &Set_Anode($anode, $aoff, 'spareShort', -1);
    &Set_Anode($anode, $aoff, 'spare[0]', 1);
    &Set_Anode($anode, $aoff, 'spare[1]', 314159);
}

sub check_anode7 {
    &anode_flags_diff(@_) ||
    &anode_diff(@_, 'length2') ||
    &anode_diff(@_, 'spareShort') ||
    &anode_diff(@_, 'spare[0]') ||
    &anode_diff(@_, 'spare[1]');
}

#
# Anode8. Rules for valid block addresses.
#
# Repair action: discard invalid addresses.
#
# We try several varieties of bad address just for fun.
#
sub test_anode8 {
    local($this_test) = 'anode8';
    &print_start_message($this_test, 'Try invalid block addresses');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
	*break_anode8a, *check_blocked_anode8, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
	*break_anode8b, *check_blocked_anode8, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'c',
	*break_anode8c, *check_blocked_anode8, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'd',
	*break_anode8d, *check_blocked_anode8, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'e',
	*break_anode8e, *check_frag_anode8, *frag);
    $errors += &test_anodes($backing_FS, $this_test . 'f',
	*break_anode8f, *check_frag_anode8, *frag);
    $errors += &test_anodes($backing_FS, $this_test . 'g',
	*break_anode8g, *check_frag_anode8, *frag);
    $errors += &test_anodes($backing_FS, $this_test . 'h',
	*break_anode8h, *check_frag_anode8, *frag);
    &print_end_message($this_test, $errors);
}

#
# Reserved addresses
#
sub break_anode8a {
    &break_blocked_anode8_common(@_, 0xfffffffc);
}

sub break_anode8b {
    &break_blocked_anode8_common(@_, 0xfffffffd);
}

sub break_anode8c {
    &break_blocked_anode8_common(@_, 0);
}

sub break_anode8d {
    local($nBlocks) = &efs'GetSuperMember('nBlocks');
    local($firstBlock) = &efs'GetSuperMember('firstBlock');
    &break_blocked_anode8_common(@_, $nBlocks + $firstBlock);
}

sub break_blocked_anode8_common {
    local($test, $anode, $aoff, $name, $badaddr) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    return () if ($length == 0);
    local($end_block) = &util'HowMany($length, $efs'blockSize);
    local($i);

    &test_info($test, $name, "trashing block addresses\n");
    $end_block = &util'Min($end_block, $efs'ANODE_DIRECTBLOCKS);
    for ($i = 0; $i != $end_block; $i++) {
	&Set_Anode($anode, $aoff, "dBlock[$i]", $badaddr);
    }
    for ($i = 0; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	local($iblock) = &efs'GetAnodeMember($anode, "iBlock[$i]");
	next if ($iblock == $efs'ANODE_BLOCKEMPTY);
	&Set_Anode($anode, $aoff, "iBlock[$i]", $badaddr);
    }
}

sub break_anode8e {
    &break_frag_anode8_common(@_, 0xfffffffc);
}

sub break_anode8f {
    &break_frag_anode8_common(@_, 0xfffffffd);
}

sub break_anode8g {
    &break_frag_anode8_common(@_, 0);
}

sub break_anode8h {
    local($nBlocks) = &efs'GetSuperMember('nBlocks');
    local($firstBlock) = &efs'GetSuperMember('firstBlock');
    &break_frag_anode8_common(@_, $nBlocks + $firstBlock);
}

sub break_frag_anode8_common {
    local($test, $anode, $aoff, $name, $badaddr) = @_;
    &test_info($test, $name, "trashing fragment address\n");
    &Set_Anode($anode, $aoff, 'd.frags.block', $badaddr);
}

sub check_blocked_anode8 {
    local($old, $new, $name, $index) = @_;

    return 1 if &anode_flags_diff(@_) || &anode_diff(@_, 'length2');

    local($length) = &efs'GetAnodeMember($old, 'length2');
    return 0 if ($length == 0);
    local($end_block) = &util'HowMany($length, $efs'blockSize);
    local($block);
    local($i);

    $end_block = &util'Min($end_block, $efs'ANODE_DIRECTBLOCKS);
    for ($i = 0; $i != $end_block; $i++) {
	$block = &efs'GetAnodeMember($new, "dBlock[$i]");
	return &salverr($name, $index, "bad block address %#x not cleared\n",
			$block) unless ($block == $efs'ANODE_BLOCKEMPTY);
    }
    return 0;
}

sub check_frag_anode8 {
    local($old, $new, $name, $index) = @_;
    local($flags) = &efs'GetAnodeMember($new, 'flags');
    local($length) = &efs'GetAnodeMember($new, 'length2');

    return &salverr($name, $index, "anode freed unexpectedly\n")
	if ($flags == 0);
    return &salverr($name, $index, "bad block address in frag not cleared\n")
	if ($length != 0);
    return 0;
}

#
# Anode9. No duplicated block addresses.
#
# Repair action: Make new copies of duplicated blocks, discard
# duplicate addresses, or free conflicting anodes, depending on
# how many special anodes are involved in the conflict.
#
# Since this is an important and fairly complicated case, we do
# several subtests.  XXX Probably should do more.
#

$donor_reg = "$file_dir/iblocks";
$donor_spec = $efs'CFLAGS_BITMAP;

sub test_anode9 {
    local($this_test) = 'anode9';
    &print_start_message($this_test, 'Try duplicate block allocations');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_anode9a, *check_anode9a, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_anode9b, *check_anode9b, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'c',
		*break_anode9c, *check_anode9c, *frag);
    $errors += &test_anodes($backing_FS, $this_test . 'd',
		*break_anode9d, *check_anode9d, *frag);
    $errors += &test_anodes($backing_FS, $this_test . 'e',
		*break_anode9e, *check_anode9e, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'f',
		*break_anode9f, *check_anode9fg, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'g',
		*break_anode9g, *check_anode9fg, *block);
    $errors += &special_anode9($this_test . 'h',
			$efs'CFLAGS_LOG, $donor_spec);
    &print_end_message($this_test, $errors);
}

#
# 9a. Two regular anodes conflict; copy duplicated blocks.
#
# XXX Need to check case where copying fails.
#
sub break_anode9a {
    local($test, $anode, $aoff, $name) = @_;
    local($donor, $doffset, $dname) = &GetInfoByName($donor_reg, @block);
    &break_anode9_bcopy(@_, $donor, $dname) unless ($anode eq $donor);
}

sub check_anode9a {
    local($old, $new, $name, $index) = @_;
    local($donor, $doffset, $dname) = &GetInfoByName($donor_reg, @block);

    return 0 if $donor eq $old;
    return 1 if &anode_flags_diff(@_) || &anode_diff(@_, 'length2');

    local($dindex) = &efs'GetAnodeMember($donor, 'index');

    #
    # Get updated donor anode
    #
    local(@fs_info) = &get_fs_info($backing_FS);
    local(@anode) = &GetFSAnodeFromIndex(*fs_info, $dindex);

    return &salverr($name, $index, "can't find file $donor_reg?!")
	unless @anode;

    $donor = $anode[$#anode - 1];

    local($dlen) = &efs'GetAnodeMember($donor, 'length2');
    local($vlen) = &efs'GetAnodeMember($old, 'length2');
    local($len) = &util'Min($dlen, $vlen);
    return 0 if ($len == 0);

    local($end_block) = &util'HowMany($len, $efs'blockSize);
    $end_block = &util'Min($end_block, $efs'ANODE_DIRECTBLOCKS);

    local($i, $daddr, $naddr);
    for ($i = 0; $i != $end_block; $i++) {
	$daddr = &efs'GetAnodeMember($donor, "dBlock[$i]");
	$naddr = &efs'GetAnodeMember($new, "dBlock[$i]");
	#
	# XXX should check for conflict anywhere, not just with $daddr.
	#
	return &salverr($name, $index, "duplicate logical block %d (%#x)\n",
			$i, $naddr) if ($daddr == $naddr);
	return &salverr($name, $index, "invalid logical block %i (%#x)\n",
			$i, $naddr) unless &efs'IsValidBlockAddr($naddr);
    }
    return 0;
}

#
# 9b. Regular anode conflicts with special one; discard duplicate blocks
# in regular anode.
#
sub break_anode9b {
    local($donor, $doffset) = &specinfo($donor_spec);
    &break_anode9_bcopy(@_, $donor, &specname($donor));
}

sub check_anode9b {
    local($old, $new, $name, $index) = @_;

    return 1 if &anode_flags_diff(@_) || &anode_diff(@_, 'length2');

    #
    # Check that addresses were cleared in regular file
    #
    local($len) = &efs'GetAnodeMember($old, 'length2');
    return 0 if ($len == 0);

    local($end_block) = &util'Min(
	&util'HowMany($len, $efs'blockSize), $efs'ANODE_DIRECTBLOCKS);

    local($i, $addr);
    for ($i = 0; $i != $end_block; $i++) {
	$addr = &efs'GetAnodeMember($new, "dBlock[$i]");
	return &salverr($name, $index, "expected block %i free, is %#x\n",
			$i, $addr) unless ($addr == $efs'ANODE_BLOCKEMPTY);
    }

    #
    # Now check that donor is intact
    #
    &check_donor;
}

#
# 9c. Same as 9a, but with fragmented file.
#
sub break_anode9c {
    local($donor, $doffset, $dname) = &GetInfoByName($donor_reg, @block);
    &break_anode9_frag(@_, $donor, $dname);
}

sub check_anode9c {
    local($old, $new, $name, $index) = @_;

    return 1 if &anode_flags_diff(@_) || &anode_diff(@_, 'length2');

    local($donor, $doffset, $dname) = &GetInfoByName($donor_reg, @block);
    local($dindex) = &efs'GetAnodeMember($donor, 'index');

    local(@fs_info) = &get_fs_info($backing_FS);
    local(@anode) = &GetFSAnodeFromIndex(*fs_info, $dindex);

    return &salverr($name, $index, "can't find file $donor_reg?!")
	unless @anode;

    $donor = $anode[$#anode - 1];

    local($daddr) = &efs'GetAnodeMember($donor, 'dBlock[0]');
    local($faddr) = &efs'GetAnodeMember($new, 'd.frags.block');
    return &salverr($name, $index, "duplicate fragment address %#x\n", $faddr)
	if ($faddr == $daddr);
    return &salverr($name, $index, "invalid fragment address %#x\n", $faddr)
	unless &efs'IsValidBlockAddr($faddr);
    return 0;
}

#
# 9d. Same as 9b, but with fragmented file.
#
sub break_anode9d {
    local($donor, $doffset) = &specinfo($donor_spec);
    &break_anode9_frag(@_, $donor, &specname($donor));
}

sub check_anode9d {
    local($old, $new, $name, $index) = @_;
    local($flags) = &efs'GetAnodeMember($new, 'flags');
    local($length) = &efs'GetAnodeMember($new, 'length2');
    local($type) = ($flags & $efs'AFLAGS_DATA_NOTBLOCKED);

    #
    # Check that donor is intact
    #
    return 1 if &check_donor;

    #
    # File should be truncated to zero
    #
    return &salverr($name, $index, "length is $length, expected zero\n")
	unless ($length == 0);
    return &salverr($name, $index,
	"file is $bitname{$type}, expected blocked\n") unless ($type == 0);

    return 0;
}

#
# 9e. Duplicated direct blocks within one anode.
#
sub break_anode9e {
    local($test, $anode, $aoff, $name) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    return () if ($length < 2 * $efs'blockSize);
    local($addr) = &efs'GetAnodeMember($anode, 'dBlock[0]');
    return () if ($addr == $efs'ANODE_BLOCKEMPTY);
    &test_info($test, $name, "copying dBlock[0] to dBlock[1]\n");
    &Set_Anode($anode, $aoff, 'dBlock[1]', $addr);
}

sub check_anode9e {
    local($old, $new, $name, $index) = @_;
    local($index) = &efs'GetAnodeMember($old, 'index');

    return 1 if &anode_flags_diff(@_) || &anode_diff(@_, 'length2');

    local($length) = &efs'GetAnodeMember($old, 'length2');
    return 0 if ($length < 2 * $efs'blockSize);

    local($addr0) = &efs'GetAnodeMember($old, 'dBlock[0]');
    return 0 if ($addr0 == $efs'ANODE_BLOCKEMPTY);
    local($naddr0) = &efs'GetAnodeMember($new, 'dBlock[0]');
    local($naddr1) = &efs'GetAnodeMember($new, 'dBlock[1]');
    return 0 if &efs'IsValidBlockAddr($naddr0) &&
		&efs'IsValidBlockAddr($naddr1) &&
		($naddr0 != $naddr1) &&
		($naddr0 == $addr0 || $naddr1 == $addr0);
    &salverr($name, $index, "duplicate blocks (%#x, %#x) became (%#x, %#x)\n",
	$addr0, $addr0, $naddr0, $naddr1);
}

#
# 9f. Data block same as indirect block
#
sub break_anode9f {
    local($test, $anode, $aoff, $name) = @_;
    local($i, $iblock);

    for ($i = 0; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	$iblock = &efs'GetAnodeMember($anode, "iBlock[$i]");
	next unless &efs'IsValidBlockAddr($iblock);
	&test_info($test, $name, "setting data block to iBlock[$i]\n");
	&trash_child_block($iblock, $i);
    }
}

#
# 9g. Two identical indirect blocks
#
sub break_anode9g {
    local($test, $anode, $aoff, $name) = @_;
    local($i, $iblock);

    for ($i = 1; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	$iblock = &efs'GetAnodeMember($anode, "iBlock[$i]");
	next unless &efs'IsValidBlockAddr($iblock);
	&test_info($test, $name, "setting indirect block to iBlock[$i]\n");
	&trash_child_block($iblock, 0);
    }
}

#
# Alter an indirect block at depth $dist to point back to this block
#
sub trash_child_block {
    local($iblock, $dist) = @_;
    local($this_block) = $iblock;
    local($block_index);
    local($ib, $iboff);
    local(@block);
    local($i);

    while ($dist-- >= 0) {
	($ib, $iboff) = &efs'IndirectGet(*AGGR, $this_block);
	@block = &efs'ExtractBlockAddr($ib);
	for ($i = 0; $i != $efs'ibp_per_block; $i++) {
	    if (&efs'IsValidBlockAddr($block[$i])) {
		($saved_block, $saved_index, $saved_value) =
		    ($this_block, $i, $iblock);
		$this_block = $block[$i];
		$block_index = $i;
		last;
	    }
	}
    }
    $block[$block_index] = $iblock;
    &Set_IndirectBlockAddrs(*block, $iboff);
}

sub check_anode9fg {
    local($old, $new, $name) = @_;
    local($index) = &efs'GetAnodeMember($old, 'index');
    local($ib, $iboff) = &efs'IndirectGet(*AGGR, $saved_block);
    local(@block) = &efs'ExtractBlockAddr($ib);
    local($baddr) = $block[$saved_index];
    return 0 if ($baddr != $saved_value);
    &salverr($name, $index,
	"duplicate address in indirect block $saved_block[$saved_index]");
}

#
# 9h. Two special anodes conflict.  Free them both.
# (XXX and create new ones??)
#
sub special_anode9 {
    local($test, $vtype, $dtype) = @_;
    local($victim, $voffset) = &specinfo($vtype);
    local($donor, $doffset) = &specinfo($dtype);
    local($vname) = &specname($victim);
    local($dname) = &specname($donor);
    local($vindex) = &efs'GetAnodeMember($victim, 'index');
    local($dindex) = &efs'GetAnodeMember($donor, 'index');
    local(@anode, $new);

    &UseTmpAggr(1);
    &break_anode9_bcopy($test, $victim, $voffset, $vname, $donor, $dname);
    &RunSalvager();
    @anode = &FindObject(*GetAggr, 1, *GetAVLAnodeFromIndex, $vindex);
    return &salverr($vname, $vindex, "anode lookup failed!\n") unless @anode;
    $new = $anode[$#anode - 1];
    return 1 if &check_anode_free($victim, $new, $vname, $vindex);

    @anode = &FindObject(*GetAggr, 1, *GetAVLAnodeFromIndex, $dindex);
    return &salverr($dname, $dindex, "anode lookup failed!\n") unless @anode;
    $new = $anode[$#anode - 1];
    &check_anode_free($donor, $new, $dname);
}

#
# Copy direct block addresses from donor to victim.
# XXX Should do indirects too.
#
sub break_anode9_bcopy {
    local($test, $victim, $aoff, $name, $donor, $dname) = @_;
    local($dlen) = &efs'GetAnodeMember($donor, 'length2');
    local($vlen) = &efs'GetAnodeMember($victim, 'length2');
    local($len) = &util'Min($dlen, $vlen);
    return () if ($len == 0);
    local($end_block) = &util'Min(
	&util'HowMany($len, $efs'blockSize), $efs'ANODE_DIRECTBLOCKS);

    local($i, $addr);
    &test_info($test, $name, "copying block addresses from $dname\n");
    for ($i = 0; $i != $end_block; $i++) {
	$addr = &efs'GetAnodeMember($donor, "dBlock[$i]");
	&Set_Anode($victim, $aoff, "dBlock[$i]", $addr);
    }
}

#
# Copy fragment address from donor's first direct block.
#
sub break_anode9_frag {
    local($test, $victim, $aoff, $name, $donor, $dname) = @_;
    local($addr) = &efs'GetAnodeMember($donor, 'dBlock[0]');
    &test_info($test, $name, "copying fragment address from $dname\n");
    &Set_Anode($victim, $aoff, 'd.frags.block', $addr);
}

#
# Make sure that special donor anode hasn't changed.
#
sub check_donor {
    local($old, $offset) = &specinfo($donor_spec);
    local($name) = &specname($old);
    local($index) = &efs'GetAnodeMember($old, 'index');
    local(@fs_info) = &get_fs_info($backing_FS);
    local(@anode) = &GetFSAnodeFromIndex(*fs_info, $index);

    return &salverr($name, $index, "can't find $name?!\n") unless @anode;

    local($new) = $anode[$#anode - 1];

    return 1 if &anode_flags_diff($old, $new, $name, $index);
    return 1 if &anode_diff($old, $new, $name, $index, 'length2');

    local($len) = &efs'GetAnodeMember($old, 'length2');
    return 0 if ($len == 0);

    local($end_block) = &util'Min(
	&util'HowMany($len, $efs'blockSize), $efs'ANODE_DIRECTBLOCKS);

    local($i);
    for ($i = 0; $i != $end_block; $i++) {
	return 1 if &anode_diff($old, $new, $name, $index, "dBlock[$i]");
    }
    return 0;
}

#
#
# Anode10. If any block address has the BLOCKBACKING bit set, then
# the anode's COPY_ON_WRITE bit must also be set.
#
# Repair action: set the COPY_ON_WRITE bit (unless the anode has
# a valid BackingAnode, this will ultimately cause both the COPY_ON_WRITE
# and BLOCKBACKING bits to be cleared, by COW6; that is what we look
# for in this test.)
#
sub test_anode10 {
    local($this_test) = 'anode10';
    &print_start_message($this_test, 'Set BLOCKBACKING bits in non-COW anode');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
	*break_blocked_anode10, *check_blocked_anode10, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
	*break_frag_anode10, *check_frag_anode10, *frag);
    &print_end_message($this_test, $errors);
}

sub break_blocked_anode10 {
    local($test, $anode, $aoff, $name) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    local($lastblock) = &util'HowMany($length, $efs'blockSize);
    local($i, $addr);

    &test_info($test, $name, "setting BLOCKBACKING bit in block addresses\n");
    $lastblock = &util'Min($lastblock, $efs'ANODE_DIRECTBLOCKS - 1);
    for ($i = 0; $i <= $lastblock; $i++) {
	$addr = &efs'GetAnodeMember($anode, "dBlock[$i]");
	next if ($addr == $efs'ANODE_BLOCKEMPTY);
	&Set_Anode($anode, $aoff,
		"dBlock[$i]", $addr | $efs'ANODE_BLOCKBACKING);
    }
}

sub break_frag_anode10 {
    local($test, $anode, $aoff, $name) = @_;
    local($nBlocks) = &efs'GetSuperMember('nBlocks');
    &test_info($test, $name, "setting BLOCKBACKING bit in fragment address\n");
    local($addr) = &efs'GetAnodeMember($anode, 'd.frags.block');
    &Set_Anode($anode, $aoff,
		'd.frags.block', $addr | $efs'ANODE_BLOCKBACKING);
}

sub check_blocked_anode10 {
    local($old, $new, $name, $index) = @_;

    return 1 if &anode_flags_diff(@_) || &anode_diff(@_, 'length2');

    local($length) = &efs'GetAnodeMember($old, 'length2');
    return 0 if ($length == 0);

    local($end_block) = &util'HowMany($length, $efs'blockSize);
    $end_block = &util'Min($end_block, $efs'ANODE_DIRECTBLOCKS);

    local($i);

    for ($i = 0; $i != $end_block; $i++) {
	return 1 if &anode_diff(@_, "dBlock[$i]");
    }
    return 0;
}

sub check_frag_anode10 {
    &anode_flags_diff(@_) || &anode_diff(@_, 'd.frags.block');
}

##
## Blocked anode tests
##

#
# Find a free block in the bitmap and mark it in use.
#
sub AllocateFreeBlock {
    local($bmap, $boff) = &specinfo($efs'CFLAGS_BITMAP);
    local($bindex) = &efs'GetAnodeMember($bmap, 'index');
    local(@result) = &FindObject(
	*GetAggr, 1,
	*GetAVLAnodeFromIndex, $bindex,
	*GetBitmapPage, 1,
	*GetFreeBlock, 1);
    return $result[$#result];
}

#
# Check whether specified block is free
#
sub CheckBlockFree {
    local($block_number) = @_;

    local($bmap, $boff) = &specinfo($efs'CFLAGS_BITMAP);
    local($bindex) = &efs'GetAnodeMember($bmap, 'index');
    local(@result) = &FindObject(
	*GetAggr, 1,
	*GetAVLAnodeFromIndex, $bindex,
	*GetBitmapPage,
	q/$bp{'baseOffset'} <= / . $block_number . ' && ' .
	$block_number . q/ < $bp{'baseOffset'} + $bp{'blocksPerPage'}/);
    local($bp) = $result[$#result - 1];
    return &efs'IsBlockFree($bp, $block_number);
}

#
# Blk1. No allocated blocks beyond EOF.
#
# Repair action: free extra blocks.
#
sub test_blk1 {
    local($this_test) = 'blk1';
    &print_start_message($this_test, 'Try allocating blocks beyond EOF');
    local($errors) = &test_anodes($backing_FS, $this_test,
	*break_blk1, *check_blk1, *block);
    &print_end_message($this_test, $errors);
}

sub break_blk1 {
    local($test, $anode, $aoff, $name) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    local($nextblock) = &util'HowMany($length, $efs'blockSize);
    return () unless ($nextblock < $efs'ANODE_DIRECTBLOCKS);
    local($newaddr) = &AllocateFreeBlock;
    $blk1_map{$name} = $newaddr;
    &test_info($test, $name, "setting dBlock[$nextblock] to %#x\n", $newaddr);
    &Set_Anode($anode, $aoff, "dBlock[$nextblock]", $newaddr);
}

sub check_blk1 {
    local($old, $new, $name, $index) = @_;

    return 1 if &anode_flags_diff(@_) || &anode_diff(@_, 'length2');

    local($length) = &efs'GetAnodeMember($old, 'length2');
    local($nextblock) = &util'HowMany($length, $efs'blockSize);
    return 0 unless ($nextblock < $efs'ANODE_DIRECTBLOCKS);
    local($nb) = &efs'GetAnodeMember($new, "dBlock[$nextblock]");
    return &salverr($name, $index, "dBlock[$nextblock] == %#x\n", $nb)
	unless ($nb == $efs'ANODE_BLOCKEMPTY);
    local($ob) = $blk1_map{$name};
    return &salverr($name, $index, "block %#x not freed\n", $ob)
	unless (&CheckBlockFree($ob));

    return 0;
}

#
# Blk2. Indirect blocks must have valid headers.
#
# Repair action: delete block if bad magic number, else fix
# incorrect values.
#

sub test_blk2 {
    local($this_test) = 'blk2';
    &print_start_message($this_test, 'Try bad indirect block headers');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
	*break_blk2_magic, *check_iblocks_freed, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
	*break_blk2_ibsize, *check_blk2_nomagic, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'c',
	*break_blk2_baseoffset, *check_blk2_nomagic, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'd',
	*break_blk2_span, *check_blk2_nomagic, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'e',
	*break_blk2_who_block, *check_blk2_nomagic, *block);
    $errors += &test_anodes($backing_FS, $this_test . 'f',
	*break_blk2_who_offset, *check_blk2_nomagic, *block);

    &print_end_message($this_test, $errors);
}

sub break_blk2_magic {
    &break_blk2_common(@_, 'magic');
}

sub break_blk2_ibsize {
    &break_blk2_common(@_, 'ibsize');
}

sub break_blk2_baseoffset {
    &break_blk2_common(@_, 'baseOffset');
}

sub break_blk2_span {
    &break_blk2_common(@_, 'span');
}

sub break_blk2_who_block {
    &break_blk2_common(@_, 'whoPoints.block');
}

sub break_blk2_who_offset {
    &break_blk2_common(@_, 'whoPoints.offset');
}

sub break_blk2_common {
    local($test, $anode, $aoff, $name, $member) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    return () unless ($length > $efs'blockSize * $efs'ANODE_DIRECTBLOCKS);

    &test_info($test, $name, "trashing $member in indirect block headers\n");
    local($i, $iblock);
    for ($i = 0; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	$iblock = &efs'GetAnodeMember($anode, "iBlock[$i]");
	next unless &efs'IsValidBlockAddr($iblock);
	&trash_itree($iblock, $i, $member);
    }
}

#
# The following is a workaround for a bug in perl.  A statement of
# the form "local($x, $y) = &foo(...);" causes a core dump when used
# in a recursive function.  The code for saving local variables before
# making a recursive call appears to choke on a list. So we get the
# list here and squirrel away one of the results in a global variable
# to work around the problem.
#
sub perl_bug_workaround {
    local(*DEVICE, $iblock) = @_;
    local($ib, $iboff) = &efs'IndirectGet(*DEVICE, $iblock);
    $saved_iboff = $iboff;
    return $ib;
}

#
# Recursively trash specified member of indirect block header in all
# indirect blocks in the subtree rooted here.
#
sub trash_itree {
    local($iblock, $height, $member) = @_;
#   local($ib, $iboff) = &efs'IndirectGet(*AGGR, $iblock);
    local($ib) = &perl_bug_workaround(*AGGR, $iblock);
    local($iboff) = $saved_iboff;
    local(@block) = &efs'ExtractBlockAddr($ib);
    if ($height > 0) {
	local($i);
	for ($i = 0; $i != $efs'ibp_per_block; $i++) {
	    next unless &efs'IsValidBlockAddr($block[$i]);
	    &trash_itree($block[$i], $height - 1, $member);
	}
    }
    &Set_Indirect($ib, $iboff, $member, -1);
}

#
# Check that the indirect block addresses in an anode have been cleared
# and that the corresponding blocks have been freed.
#
# XXX Should check whole trees, not just roots.
#
sub check_iblocks_freed {
    local($old, $new, $name, $index) = @_;

    local($length) = &efs'GetAnodeMember($old, 'length2');
    return 0 unless ($length > $efs'blockSize * $efs'ANODE_DIRECTBLOCKS);

    return 1 if &anode_flags_diff(@_);

    local($i, $iblock, $nblock);
    for ($i = 0; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	$iblock = &efs'GetAnodeMember($old, "iBlock[$i]");
	next unless &efs'IsValidBlockAddr($iblock);
	$nblock = &efs'GetAnodeMember($new, "iBlock[$i]");
	return &salverr($name, $index,
			"indirect block $i (%#x) not cleared\n", $nblock)
	    unless ($nblock == $efs'ANODE_BLOCKEMPTY);
	return &salverr($name, $index, "block %#x not freed\n", $nblock)
		unless &CheckBlockFree($iblock);
    }
    return 0;
}

#
# Check when something other than magic number is trashed; in this case,
# the trashed quantity should be recalculated and corrected.
#
# XXX We are just checking that nothing is set to -1, not verifying
# that the values are actually right.
#
sub check_blk2_nomagic {
    local($old, $new, $name, $index) = @_;

    local($length) = &efs'GetAnodeMember($old, 'length2');
    return 0 unless ($length > $efs'blockSize * $efs'ANODE_DIRECTBLOCKS);

    return 1 if &anode_flags_diff(@_);

    local($i, $iblock);
    for ($i = 0; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	$iblock = &efs'GetAnodeMember($new, "iBlock[$i]");
	next unless &efs'IsValidBlockAddr($iblock);
	&check_itree($iblock, $i, $name, $index);
    }
    return 0;
}

#
# Recursively check indirect block tree; any header memmber equal to -1
# is considered bogus.
#
sub check_itree {
    local($iblock, $height, $name, $index) = @_;
#   local($ib, $iboff) = &efs'IndirectGet(*AGGR, $iblock);
    local($ib) = &perl_bug_workaround(*AGGR, $iblock);
    local($iboff) = $saved_iboff;
    local(@block) = &efs'ExtractBlockAddr($ib);
     if ($height > 0) {
	local($i);
	for ($i = 0; $i != $efs'ibp_per_block; $i++) {
	    next unless &efs'IsValidBlockAddr($block[$i]);
	    &check_itree($block[$i], $height - 1, $name, $index);
	}
    }
    local(%ibh) = &util'UnpackAssoc($ib, @efs'INDIRECT_HEADER_FORMAT);
    foreach $i (keys %ibh) {
	return &salverr($name, $index, "member $i is trashed\n")
	   if ($ibh{$i} == -1 || $ibh{$i} == ~0);
    }
}

#
# Blk3. Indirect blocks should contain at least one nonempty block address
#
# Repair action: free indirect block.
#
sub test_blk3 {
    local($this_test) = 'blk3';
    &print_start_message($this_test, 'Try empty indirect block');
    local($errors) = &test_anodes($backing_FS, $this_test,
	*break_blk3, *check_iblocks_freed, *block);
    &print_end_message($this_test, $errors);
}

sub break_blk3 {
    local($test, $anode, $aoff, $name) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    return () unless ($length > $efs'blockSize * $efs'ANODE_DIRECTBLOCKS);
    local($i, $j, $iblock, $ib, $iboff, @block);

    &test_info($test, $name, "making indirect blocks empty\n");
    for ($i = 0; $i != $efs'ANODE_INDIRECT_TREES; $i++) {
	$iblock = &efs'GetAnodeMember($anode, "iBlock[$i]");
	next unless &efs'IsValidBlockAddr($iblock);
	($ib, $iboff) = &efs'IndirectGet(*AGGR, $iblock);
	@block = &efs'ExtractBlockAddr($ib);
	for ($j = 0; $j != $efs'ibp_per_block; $j++) {
	    $block[$j] = $efs'ANODE_BLOCKEMPTY;
	}
	&Set_IndirectBlockAddrs(*block, $iboff);
    }
}

##
## Fragmented anode tests
##

#
# Frag1. Valid fragment description (block, first, and length).
#
# Repair action: truncate file to length zero.
#
# Note: we don't try trashing block here, since that happens in Anode8.
#
sub test_frag1 {
    local($this_test) = 'frag1';
    &print_start_message($this_test, 'Try allocating blocks beyond EOF');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
	*break_frag1_first_a, *check_frag1, *frag);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
	*break_frag1_first_b, *check_frag1, *frag);
    $errors += &test_anodes($backing_FS, $this_test . 'c',
	*break_frag1_length_a, *check_frag1, *frag);
    $errors += &test_anodes($backing_FS, $this_test . 'd',
	*break_frag1_length_b, *check_frag1, *frag);
    &print_end_message($this_test, $errors);
}

#
# Try illegal block offsets
#
sub break_frag1_first_a {
    &break_frag1_common(@_, 'd.frags.first', -1);
}

sub break_frag1_first_b {
    &break_frag1_common(@_, 'd.frags.first', $efs'frags_per_block);
}

#
# Try illegal lengths
#
sub break_frag1_length_a {
    &break_frag1_common(@_, 'd.frags.length', -1);
}

sub break_frag1_length_b {
    &break_frag1_common(@_, 'd.frags.length', $efs'frags_per_block + 1);
}

sub break_frag1_common {
    local($test, $anode, $aoff, $name, $member, $badvalue) = @_;
    &test_info($test, $name, "setting $member to %#x\n", $badvalue);
    &Set_Anode($anode, $aoff, $member, $badvalue);
}

sub check_frag1 {
    local($old, $new, $name, $index) = @_;
    local($length) = &efs'GetAnodeMember($new, 'length2');
    return &salverr($name, $index, "file length is $length, expected 0\n")
	unless ($length == 0);

    return 0;
}

#
# Frag2. No allocated fragments beyond EOF (unless fragments are COW).
#
# Repair action: discard allocated fragments
#
sub test_frag2 {
    local($this_test) = 'frag2';
    &print_start_message($this_test, 'Try allocating blocks beyond EOF');
    local($errors) = &test_anodes($backing_FS, $this_test,
	*break_frag2, *check_frag2, *frag);
    &print_end_message($this_test, $errors);
}

#
# To avoid muddying the waters, we should make sure that the extra
# fragments that we allocate are free, but for now, we just optimistically
# extend the file.
#
sub break_frag2 {
    local($test, $anode, $aoff, $name) = @_;
    local($blk) = &efs'GetAnodeMember($anode, 'd.frags.block');
    return () if ($blk & $efs'ANODE_BLOCKBACKING);
    local($fpb) = $efs'blockSize / $efs'fragSize;
    local($flen) = &efs'GetAnodeMember($anode, 'd.frags.length');
    return () unless ($flen < $fpb);
    $flen += 1;
    &test_info($test, $name, "extending fragment length to $flen\n");
    &Set_Anode($anode, $aoff, 'd.frags.length', $flen);
}

sub check_frag2 {
    &anode_diff(@_, 'd.frags.length');
}

#
# Frag3. Copy-on-write anode must have fragment description identical
# to backing anode's.
#
# Repair action: correct description to match backing anode, provided that
# the backing anode is fragmented; else delete the COW anode (cf. COW2)
#
sub test_frag3 {
    local($this_test) = 'frag3';
    &print_start_message($this_test, 'Try bad COW fragment description');
    local($errors) = &test_anodes($cow_FS, $this_test . 'a',
	*break_frag3a, *check_frag3, *cow_frag);
    $errors += &test_anodes($cow_FS, $this_test . 'b',
	*break_frag3b, *check_frag3, *cow_frag);
    $errors += &test_anodes($cow_FS, $this_test . 'c',
	*break_frag3c, *check_frag3, *cow_frag);
    &print_end_message($this_test, $errors);
}

#
# Try altering each member of the fragment description in turn; stay
# within legal range of values to avoid introducing other inconsistencies.
# XXX could be more rigorous here
#
sub break_frag3a {
    &break_frag3_common(@_, 'd.frags.first', 0);
}

sub break_frag3b {
    &break_frag3_common(@_, 'd.frags.length', 0);
}

sub break_frag3c {
    local($first) = &efs'GetSuperMember('firstBlock');
    &break_frag3_common(@_, 'd.frags.block', $first);
}

sub break_frag3_common {
    local($test, $anode, $aoff, $name, $member, $minval) = @_;
    local($oldval) = &efs'GetAnodeMember($anode, $member);
    local($newval) = $oldval + ($oldval > $minval) ? (-1) : 1;
    &test_info($test, $name, "setting $member to %#x\n", $newval);
    &Set_Anode($anode, $aoff, $member, $newval);
}

sub check_frag3 {
    &anode_diff(@_, 'd.frags.first') ||
    &anode_diff(@_, 'd.frags.length') ||
    &anode_diff(@_, 'd.frags.block');
}

##
## Copy-on-write anode tests
##

#
# COW1. Both the backing and COW anodes must be of regular type.
#
# Repair action: free the COW anode
#
sub test_cow1 {
    local($this_test) = 'cow1';
    &print_start_message($this_test, 'Try bad COW anode type');
    local($errors) = &test_anodes($cow_FS, $this_test . 'a',
	*break_cow1a, *check_anode_free, *cow_reg);
    $errors += &test_anodes($cow_FS, $this_test . 'b',
	*break_cow1b, *check_not_cow, *cow_reg);
    &print_end_message($this_test, $errors);
}

#
# Give COW anode illegal type
#
sub break_cow1a {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($badtype) = $efs'AFLAGS_TYPE_LOG;
    local($newflags) = ($flags & ~$efs'AFLAGS_TYPE_MASK) | $badtype;
    &test_info($test, $name, "setting type to $typename{$badtype}\n");
    &Set_Anode($anode, $aoff, 'flags', $newflags);
}

#
# Point to an illegal backing anode
#
sub break_cow1b {
    local($test, $anode, $aoff, $name) = @_;
    local($badtype) = $efs'CFLAGS_VOLUME;
    local($spec, $spec_off) = &specinfo($badtype);
    local($spec_block) = int($spec_off / $efs'blockSize);
    local($spec_boff) = $spec_off - $spec_block * $efs'blockSize;
    &test_info($test, $name,
	"setting backing anode to $typename{$badtype} (%#x, %#x)\n",
	$spec_block, $spec_boff);
    &Set_Anode($anode, $aoff, 'bBlock', $spec_block);
    &Set_Anode($anode, $aoff, 'bOffset', $spec_boff);
}

#
# COW2. If a COW anode is blocked, the backing anode must also be
# blocked, and similarly for fragmented anodes.
#
# Repair action: free the COW anode
#
sub test_cow2 {
    local($this_test) = 'cow2';
    &print_start_message($this_test, 'Try bad COW storage type');
    local($errors) = &test_anodes($cow_FS, $this_test . 'a',
	*break_cow2a, *check_anode_free, *cow_block);
    $errors += &test_anodes($cow_FS, $this_test . 'b',
	*break_cow2b, *check_anode_free, *cow_frag);
    &print_end_message($this_test, $errors);
}

#
# Change blocked file to fragmented.
#
sub break_cow2a {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($block) = &efs'GetAnodeMember($anode, 'dBlock[0]');
    &test_info($test, $name, "changing from blocked to fragmented\n");
    $flags |= $efs'AFLAGS_DATA_FRAGMENTED;
    &Set_Anode($anode, $aoff, 'flags', $flags);
    &Set_IncoreFlags(*anode, $flags);
    &Set_Anode($anode, $aoff, 'd.frags.first', 0);
    &Set_Anode($anode, $aoff, 'd.frags.length', 1);
    &Set_Anode($anode, $aoff, 'd.frags.block', $block);
}

#
# Change fragmented file to blocked.
#
sub break_cow2b {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($block) = &efs'GetAnodeMember($anode, 'd.frags.block');
    &test_info($test, $name, "changing from fragmented to blocked\n");
    $flags &= ~$efs'AFLAGS_DATA_NOTBLOCKED;
    &Set_Anode($anode, $aoff, 'flags', $flags);
    &Set_IncoreFlags(*anode, $flags);
    &Set_Anode($anode, $aoff, 'dBlock[0]', $block);
}

#
# COW3. Block addresses must agree in the COW and backing anodes.
#
# Repair action: discard bad addresses in the COW anode
#
sub test_cow3 {
    local($this_test) = 'cow3';
    &print_start_message($this_test, 'Try bad COW block addresses');
    local($errors) = &test_anodes($cow_FS, $this_test . 'a',
	*break_cow3a, *check_cow3a, *cow_block);
    $errors += &test_anodes($cow_FS, $this_test . 'b',
	*break_cow3b, *check_cow3b, *cow_frag);
    &print_end_message($this_test, $errors);
}

sub break_cow3a {
    local($test, $anode, $aoff, $name) = @_;
    local($i, $block);
    for ($i = 0; $i != $efs'ANODE_DIRECTBLOCKS; $i++) {
	$block = &efs'GetAnodeMember($anode, "dBlock[$i]");
	next if ($block == $efs'ANODE_BLOCKEMPTY);
	&test_info($test, $name, "incrementing dBlock[$i]\n");
	$block++;
	&Set_Anode($anode, $aoff, "dBlock[$i]", $block);
    }
}

sub check_cow3a {
    local($old, $new, $name, $index) = @_;
    local($i, $block);
    for ($i = 0; $i != $efs'ANODE_DIRECTBLOCKS; $i++) {
	$block = &efs'GetAnodeMember($anode, "dBlock[$i]");
	return &salverr($name, $index, "dBlock[$i] == %#x, expected empty\n",
	    $block) unless ($block == $efs'ANODE_BLOCKEMPTY);
    }
    return 0;
}

sub break_cow3b {
    local($test, $anode, $aoff, $name) = @_;
    local($block) = &efs'GetAnodeMember($anode, 'd.frags.block');
    &test_info($test, $name, "incrementing d.frags.block\n");
    $block++;
    &Set_Anode($anode, $aoff, 'd.frags.block', $block);
}

sub check_cow3b {
    local($old, $new, $name, $index) = @_;
    local($block) = &efs'GetAnodeMember($anode, 'd.frags.block');
    return &salverr($name, $index, "d.frags.block == %#x, expected empty\n",
	    $block) unless ($block == $efs'ANODE_BLOCKEMPTY);
    return 0;
}

#
# COW4. Status length must be the same in COW and backing anodes.
#
# Repair action: free the COW anode.
#
sub test_cow4 {
    local($this_test) = 'cow4';
    &print_start_message($this_test, 'Try bad COW status size');
    local($errors) = &test_anodes($cow_FS, $this_test,
	*break_cow4, *check_anode_free, *cow_reg);
    &print_end_message($this_test, $errors);
}

sub break_cow4 {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($ssize) = $flags & $efs'AFLAGS_STATUSSIZE_MASK;
    $flags = ($flags & ~$efs'AFLAGS_STATUSSIZE_MASK) | ($ssize - 1);
    &test_info($test, $name, "decrementing status size\n");
    &Set_Anode($anode, $aoff, 'flags', $flags);
}

#
# COW5. Each anode must back no more than 65535 COW anodes.
#
# Repair action: free enough COW anodes to reduce the total
# below 65535.
#
# XXX This one is a little esoteric and hard to implement, so
# we will skip it for now.
#
sub test_cow5 {
    &print_start_message('cow5', 'Try too many COW anodes (unimplemented)');
    &print_end_message('cow5', 0);
}

#
# COW6. The backingAnode member of each COW anode must identify
# a valid anode.
#
# Repair action: convert the anode into a non-COW anode by making
# copies of all COW blocks.
#
sub test_cow6 {
    local($this_test) = 'cow6';
    &print_start_message($this_test, 'Try bad COW backing anode');
    local($errors) = &test_anodes($cow_FS, $this_test . 'a',
	*break_cow6a, *check_not_cow, *cow_reg);
    $errors += &test_anodes($cow_FS, $this_test . 'b',
	*break_cow6b, *check_not_cow, *cow_reg);
    &print_end_message($this_test, $errors);
}

$cow6_ticker = 0;

#
# Trash the block number
#
sub break_cow6a {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "trashing bBlock\n");
    local($case) = ($cow6_ticker++ % 9);
    if ($case == 0) {
	&Set_Anode($anode, $aoff, 'bBlock', 0);
    } elsif ($case == 1) {
	&Set_Anode($anode, $aoff, 'bBlock', $efs'ANODE_BLOCKEMPTY);
    } elsif ($case == 2) {
	&Set_Anode($anode, $aoff, 'bBlock', $efs'ANODE_BLOCKBACKING);
    } elsif ($case == 3) {
	&Set_Anode($anode, $aoff, 'bBlock', 0xfffffffc);
    } elsif ($case == 4) {
	&Set_Anode($anode, $aoff, 'bBlock', 0xfffffffd);
    } elsif ($case == 5) {
	local($bb) = &efs'GetAnodeMember($anode, 'bBlock');
	&Set_Anode($anode, $aoff, 'bBlock', $bb - 1);
    } elsif ($case == 6) {
	local($bb) = &efs'GetAnodeMember($anode, 'bBlock');
	&Set_Anode($anode, $aoff, 'bBlock', $bb | $efs'ANODE_BLOCKBACKING);
    } elsif ($case == 7) {
	local($nb) = &efs'GetSuperMember('nBlocks');
	&Set_Anode($anode, $aoff, 'bBlock', $nb);
    } elsif ($case == 8) {
	&Set_Anode($anode, $aoff, 'bBlock', -1);
    }
}

#
# Trash the block offset
#
sub break_cow6b {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "trashing bOffset\n");
    local($case) = ($cow6_ticker++ % 7);
    if ($case == 0) {
	&Set_Anode($anode, $aoff, 'bOffset', 0);
    } elsif ($case == 1) {
	local($off) = &efs'GetAnodeMember($anode, 'bOffset');
	&Set_Anode($anode, $aoff, 'bOffset', -$off);
    } elsif ($case == 2) {
	local($off) = &efs'GetAnodeMember($anode, 'bOffset');
	&Set_Anode($anode, $aoff, 'bOffset', $off + 1);
    } elsif ($case == 3) {
	&Set_Anode($anode, $aoff, 'bOffset', $efs'blockSize);
    } elsif ($case == 4) {
	local($nb) = &efs'GetSuperMember('nBlocks');
	&Set_Anode($anode, $aoff, 'bOffset', $nb * $efs'blockSize);
    } elsif ($case == 5) {
	&Set_Anode($anode, $aoff, 'bOffset', -1);
    } elsif ($case == 6) {
	&Set_Anode($anode, $aoff, 'bOffset', 1);
    }
}

#
# Check that the COPY_ON_WRITE bit and BLOCK_BACKING bits
# are cleared.
#
sub check_not_cow {
    local($old, $new, $name, $index) = @_;
    local($oflags) = &efs'GetAnodeMember($old, 'flags');
    local($nflags) = &efs'GetAnodeMember($new, 'flags');
    local($expected) = ($oflags & ~$efs'AFLAGS_COPYONWRITE);
    $nflags &= ~$efs'AFLAGS_UNSAFE;
    return &salverr($name, $index, "expected flags %#x, got %#x\n",
	$expected, $nflags) unless ($nflags == $expected);
    return 0 if ($nflags & $efs'AFLAGS_DATA_NOTBLOCKED);
    local($i, $b);
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    local($lastblock) = &util'HowMany($length, $efs'blockSize);
    $lastblock = &util'Min($lastblock, $efs'ANODE_DIRECTBLOCKS - 1);
    for ($i = 0; $i <= $lastblock; $i++) {
	$b = &efs'GetAnodeMember($new, "dBlock[$i]");
	next if ($b == $efs'ANODE_BLOCKEMPTY);
	return &salverr($name, $index, "bad block address %#x\n", $b)
	    unless &efs'IsValidBlockAddr($b, 0);
    }
    return 0;
}

#
# COW7. The copies member of each anode must equal the number of COW
# anodes whose backingAnode members identify it.
#
# Repair action: set copies to correct value
#
sub test_cow7 {
    local($this_test) = 'cow7';
    &print_start_message($this_test, 'Try incorrect copies count');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
	*break_cow7a, *check_cow7, *regular);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
	*break_cow7b, *check_cow7, *regular);
    &print_end_message($this_test, $errors);
}

#
# Set copies to zero.
#
sub break_cow7a {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting copies to zero\n");
    &Set_Anode($anode, $aoff, 'copies', 0);
}

#
# Set copies to largest possible value.
#
sub break_cow7b {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting copies to maximum value\n");
    &Set_Anode($anode, $aoff, 'copies', 65535);
}

sub check_cow7 {
    &anode_diff(@_, 'copies');
}

1;
