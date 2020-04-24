# Copyright (C) 1993 Transarc Corporation - All rights reserved
##
## Volume tests for salvager test suite
##
sub test_volume_anode {
    local($testname, $fs_name, $fs_index, *break_rule, *check_repair) = @_;
    local(@fs_info) = &get_fs_info($fs_index);

    &UseTmpAggr(1);
    &break_rule($testname, @fs_info[0 .. 1], $fs_name);
    &RunSalvager();
    local(@new_info) = &FindObject(
	*GetAggr, 1, *GetAVLAnodeFromIndex, $fs_index);
    return 1 unless (@new_info);

    local($new_anode) = $new_info[$#new_info - 1];
    &check_repair($fs_info[0], $new_anode, $fs_name, $fs_index);
}

sub get_fileset {
    local(@input) = @_;
    local(@result) = &GetFileSet(*input, 1);
    return @result[$#result - 1 .. $#result];
}

sub test_volume {
    local($testname, $fs_name, $fs_index, *break_rule, *check_repair) = @_;
    local(@fs_info) = &get_fs_info($fs_index);

    &UseTmpAggr(1);
    &break_rule($testname, $fs_name, @fs_info);
    &RunSalvager();
    local(@new) = &FindObject(
	*GetAggr, 1,
	*GetAVLAnodeFromIndex, $fs_index);
    local($new_anode) = $new[$#new - 1];
    local($flags) = &efs'GetAnodeMember($new_anode, 'flags');
    if ($flags == 0) {
	local($index) = &efs'GetAnodeMember($fs_info[0], 'index');
	return &salverr($fs_name, $index, "volume anode was freed\n");
    }
    local($new_fs, $new_fsoff) = &get_fileset(@new[$#new - 1 .. $#new]);
    &check_repair($fs_name, $fs_index,
	$fs_info[0], $new_anode, $fs_info[2], $new_fs);
}

#
# Test whether old and new FS headers agree.
#
sub fs_diff {
    local($name, $index, $old, $new, $member) = @_;
    local($ovalue) = &efs'GetFSMember($old, $member);
    local($nvalue) = &efs'GetFSMember($new, $member);
    return 0 if ($ovalue == $nvalue);
    &salverr($name, $index, "$member was %#x, now %#x\n", $ovalue, $nvalue);
}

#
# Check for inconsistent file system.
#
sub check_inconsistent {
    local($name, $index, $fs) = @_;
    local($flags) = &efs'GetFSMember($fs, 'flags');
    return 0 if ($flags & $efs'FS_FLAGS_INCONSISTENT);
    &salverr($name, $index, "unexpectedly not marked inconsistent\n");
}

sub check_incons {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    &check_inconsistent($name, $index, $fsnew);
}

#
# Vol1. Each volume anode has 120 bytes of status information.
#
# repair action: reconstruct status from volume header; free anode
# if not successful.
#
sub test_vol1 {
    local($this_test) = 'vol1';
    &print_start_message($this_test, 'Try bad volume anode status');
    local($errors) = &test_volume_anode(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol1, *check_vol1);
    &print_end_message($this_test, $errors);
}

sub break_vol1 {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($bad_status_size) = 119;
    local($newflags) =
	($flags  & ~$efs'AFLAGS_STATUSSIZE_MASK) | $bad_status_size;
    &test_info($test, $name, "setting status size to $bad_status_size\n");
    &Set_Anode($anode, $aoff, 'flags', $newflags);
}

sub check_vol1 {
    &anode_flags_diff(@_);
}

#
# Vol2. The volume data consists only of anode blocks with magic number
# ANODE_BLOCK_VOLUME_MAGIC
#
# repair action: free the volume anode.
#
sub test_vol2 {
    local($this_test) = 'vol2';
    &print_start_message($this_test, 'Try bad volume anode data');
    local($errors) = &test_volume_anode(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol2a, *check_anode_free);
    $errors += &test_volume_anode(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol2b, *check_anode_free);
    $errors += &test_volume_anode(
	$this_test . 'c', $backing_FS_name, $backing_FS,
	*break_vol2c, *check_anode_free);
    &print_end_message($this_test, $errors);
}

sub break_vol2a {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting dBlock[0] to empty\n");
    &Set_Anode($anode, $aoff, 'dBlock[0]', $efs'ANODE_BLOCKEMPTY);
}

sub break_vol2b {
    local($test, $anode, $aoff, $name) = @_;
    local($anode_block) = int($aoff / $efs'blockSize);
    &test_info($test, $name, "setting dBlock[0] to %#x\n", $anode_block);
    &Set_Anode($anode, $aoff, 'dBlock[0]', $anode_block);
}

sub break_vol2c {
    local($test, $anode, $aoff, $name) = @_;
    local(@input) = ($anode, $aoff);
    local(@result) = &GetAnodeBlock(*input, 1);
    local($ablock, $aboff) = @result[$#result - 1 .. $#result];
    &test_info($test, $name, "changing magic number\n");
    &Set_AnodeBlock($ablock, $aboff, 'magic', $efs'ABH_AVL_MAGIC);
}

#
# Vol3. The length of the volume anode is a whole number of blocks.
#
# repair action: recalculate length from disk addresses.
#
sub test_vol3 {
    local($this_test) = 'vol3';
    &print_start_message($this_test, 'Try bad volume anode data');
    local($errors) = &test_volume_anode(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol3a, *check_vol3);
    $errors += &test_volume_anode(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol3b, *check_vol3);
    &print_end_message($this_test, $errors);
}

sub break_vol3a {
    local($test, $anode, $aoff, $name) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    &test_info($test, $name, "decreasing length\n");
    &Set_Anode($anode, $aoff, 'length2', $length - 1);
}

sub break_vol3b {
    local($test, $anode, $aoff, $name) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    &test_info($test, $name, "increasing length\n");
    &Set_Anode($anode, $aoff, 'length2', $length + 1);
}

sub check_vol3 {
    &anode_diff(@_, 'length2');
}

#
# Vol4. The volume header begins with magic number VOLUME_MAGIC.
#
# repair action: free the volume anode.
#
sub test_vol4 {
    local($this_test) = 'vol4';
    &print_start_message($this_test, 'Try bad volume header magic number');
    local($errors) = &test_volume_anode(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol4, *check_anode_free);
    &print_end_message($this_test, $errors);
}

sub break_vol4 {
    local($test, $anode, $aoff, $name) = @_;
    local($fs, $fsoff) = &get_fileset($anode, $aoff);
    &test_info($test, $name, "changing magic number\n");
    &Set_FileSet($fs, $fsoff, 'magic', $efs'AGGR_MAGIC);
}

#
# Vol5. index correctly identifies the volume anode.
#
# repair action: correct the index by scanning the AVL.
#
sub test_vol5 {
    local($this_test) = 'vol5';
    &print_start_message($this_test, 'Try bad volume header index');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol5, *check_vol5);
    &print_end_message($this_test, $errors);
}

sub break_vol5 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "changing index\n");
    &Set_FileSet($fs, $fsoff, 'index', $cow_FS);
}

sub check_vol5 {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    &fs_diff($name, $index, $fsold, $fsnew, 'index');
}

#
# Vol6. The volume ID and name in the anode status area must match
# the ID and name in the volume header.
#
# repair action: reconstruct the status area from the volume header.
#
sub test_vol6 {
    local($this_test) = 'vol6';
    &print_start_message($this_test, 'Try bad anode status info');
    local($errors) = &test_volume_anode(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol6a, *check_vol6);
    $errors += &test_volume_anode(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol6b, *check_vol6);
    &print_end_message($this_test, $errors);
}

sub break_vol6a {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "changing volume name\n");
    &Set_Anode($anode, $aoff, 'ident.name', 'Goofy');
}

sub break_vol6b {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "changing volume ID\n");
    &Set_Anode($anode, $aoff, 'ident.id1', 0);
    &Set_Anode($anode, $aoff, 'ident.id2', 0);
}

sub check_vol6 {
    &anode_flags_diff(@_) ||
    &anode_diff(@_, 'ident.name') ||
    &anode_diff(@_, 'ident.id1') ||
    &anode_diff(@_, 'ident.id2');
}

#
# Vol7. If the volume name contains a zero byte, then all
# following bytes must also be zero.
#
# repair action: zero all bytes following the first zero.
#
sub test_vol7 {
    local($this_test) = 'vol7';
    &print_start_message($this_test, 'Try extra chars after vol name');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol7, *check_vol7);
    &print_end_message($this_test, $errors);
}

sub break_vol7 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "inserting chars after end of name\n");
    local($name) = &efs'GetAnodeMember($anode, 'ident.name');
    $name .= "\0the Spanish Inquisition";
    &Set_Anode($anode, $aoff, 'ident.name', $name);
    &Set_FileSet($fs, $fsoff, 'ident.name', $name);
}

sub check_vol7 {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    &anode_diff($aold, $anew, $name, $index, 'ident.name') ||
    &fs_diff($name, $index, $fsold, $fsnew, 'ident.name');
}

#
# Vol8. The volume type must be in 1..4.
#
# repair action: mark the volume inconsistent.
#
sub test_vol8 {
    local($this_test) = 'vol8';
    &print_start_message($this_test, 'Try bad volume type');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol8, *check_incons);
    &print_end_message($this_test, $errors);
}

sub break_vol8 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local($flags) = &efs'GetFSMember($fs, 'flags');
    local($badtype) = 0x300;
    &test_info($test, $name, "changing to bad volume type %#x\n", $badtype);
    $flags = ($flags & ~$efs'FS_FLAGS_TYPEFIELD) | $badtype;
    &Set_FileSet($fs, $fsoff, 'flags', $flags);
}

#
# Vol9.  If the volume type is RW, the READONLY flags bit should not
# be set.
#
# repair action: mark the volume inconsistent.
#
sub test_vol9 {
    local($this_test) = 'vol9';
    &print_start_message($this_test, 'Try inconsistent use of RO bit');
    local($errors) = &test_volume(
	$this_test, $cow_FS_name, $cow_FS,
	*break_vol9, *check_incons);
    &print_end_message($this_test, $errors);
}

sub break_vol9 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "setting RO bit on RW volume\n");
    local($flags) = &efs'GetFSMember($fs, 'flags');
    $flags |= $efs'FS_FLAGS_READONLY;
    &Set_FileSet($fs, $fsoff, 'flags', $flags);
}

#
# Vol10.  The replication type must be in 0..2.
#
# repair action: mark the volume inconsistent.
#
sub test_vol10 {
    local($this_test) = 'vol10';
    &print_start_message($this_test, 'Try bad replication type');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol10, *check_incons);
    &print_end_message($this_test, $errors);
}

sub break_vol10 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local($flags) = &efs'GetFSMember($fs, 'flags');
    local($badtype) = 3;
    &test_info($test, $name, "setting bad replication type %#x\n", $badtype);
    $flags = ($flags & ~$efs'FS_FLAGS_REPFIELD) | $badtype;
    &Set_FileSet($fs, $fsoff, 'flags', $flags);
}

#
# Vol11.  The first anode slot (index == 2) in the volume table contains
# a valid directory anode.
#
# repair action: mark the volume inconsistent.
#
sub test_vol11 {
    local($this_test) = 'vol11';
    &print_start_message($this_test, 'Try bad root directory');
    local($errors) = &test_volume(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol11a, *check_incons);
    $errors += &test_volume(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol11b, *check_incons);
    &print_end_message($this_test, $errors);
}

sub break_vol11a {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local(@input) = ($anode, $aoff, $fs, $fsoff);
    local(@result) = &GetFSAnodeFromIndex(*input, $efs'DIR_ROOT_INDEX);
    local($root, $roffset) = @result[$#result -1 .. $#result];
    local($mode) = &efs'GetAnodeMember($root, 'mode');
    &test_info($test, $name, "changing root directory to plain file\n");
    $mode = ($mode & ~$efs'STAT_IFMT) | $efs'STAT_IFREG;
    &Set_Anode($root, $roffset, 'mode', $mode);
}

sub break_vol11b {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local(@input) = ($anode, $aoff, $fs, $fsoff);
    local(@result) = &GetFSAnodeFromIndex(*input, $efs'DIR_ROOT_INDEX);
    local($root, $roffset) = @result[$#result -1 .. $#result];
    &test_info($test, $name, "freeing root directory anode\n");
    &Set_Anode($root, $roffset, 'flags', 0);
}

#
# Vol12.  createTime <= modTime <= accessTime
#
# repair action: satisfy inequalities by copyine the smaller value
# from right to left.
#
sub test_vol12 {
    local($this_test) = 'vol12';
    &print_start_message($this_test, 'Try bad create & mod times');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol12, *check_vol12);
    &print_end_message($this_test, $errors);
}

sub break_vol12 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local($atime) = &efs'GetFSMember($fs, 'accessTime');
    local($mtime) = $atime + 1000;
    local($ctime) = $mtime + 1000;
    &test_info($test, $name, "setting incorrect create, mod times\n");
    &Set_FileSet($fs, $fsoff, 'modTime', $mtime);
    &Set_FileSet($fs, $fsoff, 'createTime', $ctime);
}

sub check_vol12 {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    local($atime) = &efs'GetFSMember($fsnew, 'accessTime');
    local($mtime) = &efs'GetFSMember($fsnew, 'modTime');
    local($ctime) = &efs'GetFSMember($fsnew, 'createTime');
    return 0 if ($ctime <= $mtime && $mtime <= $atime);
    &salverr($name, $index, "times disordered: ($ctime, $mtime, $atime)\n");
}

#
# Vol13.  All spare fields contain zero.
#
# repair action: zero them.
#
sub test_vol13 {
    local($this_test) = 'vol13';
    &print_start_message($this_test, 'Try non-zero spare fields.');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol13, *check_vol13);
    &print_end_message($this_test, $errors);
}

sub break_vol13 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local($i);
    &test_info($test, $name, "setting spares to non-zero\n");
    for ($i = 0; $i != 5; $i++) {
	&Set_FileSet($fs, $fsoff, "sparel[$i]", $i)
    }
    &Set_FileSet($fs, $fsoff, 'allocated.spare1', 123);
    &Set_FileSet($fs, $fsoff, 'allocated.spare2', 456);
    &Set_FileSet($fs, $fsoff, 'visible.spare1', 789);
    &Set_FileSet($fs, $fsoff, 'visible.spare2', 531);
    &Set_FileSet($fs, $fsoff, 'sparerec', 'I am the walrus');
}

sub check_vol13 {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    local($i);
    for ($i = 0; $i != 5; $i++) {
	return 1 if &fs_diff($name, $index, $fsold, $fsnew, "sparel[$i]");
    }
    &fs_diff($name, $index, $fsold, $fsnew, 'allocated.spare1') ||
    &fs_diff($name, $index, $fsold, $fsnew, 'allocated.spare2') ||
    &fs_diff($name, $index, $fsold, $fsnew, 'visible.spare1') ||
    &fs_diff($name, $index, $fsold, $fsnew, 'visible.spare2') ||
    &fs_diff($name, $index, $fsold, $fsnew, 'sparerec');
}

#
# Various tests of the zero-link-count (ZLC) list follow.  We assume
# that the volume initially has a completely empty ZLC list.  The test
# data must (and does) satisfy this assumption.
#
sub get_zlc_count {
    &efs'GetFSMember(@_, 'fileStatus[0]');
}

sub set_zlc_count {
    local($fs, $fsoff, $count) = @_;
    &Set_FileSet($fs, $fsoff, 'fileStatus[0]', $count);
}

sub get_zlc_file {
    local($fs, $fileno) = @_;
    local($fsindex) = $fileno + 1;
    &efs'GetFSMember($fs, "fileStatus[$fsindex]");
}

sub set_zlc_file {
    local($fs, $fsoff, $fileno, $index) = @_;
    local($fsindex) = $fileno + 1;
    &Set_FileSet($fs, $fsoff, "fileStatus[$fsindex]", $index);
}

#
# Find a free anode and initialize it as a regular file.
#
sub make_zlc_anode {
    local(@input) = @_;
    local(@result) = &GetFreeFSAnode(*input, $efs'ANODE_FILE_STATUS_SIZE);
    return @result[$#result - 1 .. $#result];
}

sub fill_zlc_list {
    local($anode, $aoff, $fs, $fsoff) = @_;
    local($zanode, $zoffset, $zindex, $i);
    for ($i = 0; $i != 10; $i++) {
	($zanode, $zoffset) = &make_zlc_anode($anode, $aoff, $fs, $fsoff);
	$zindex = &efs'GetAnodeMember($zanode, 'index');
	&set_zlc_file($fs, $fsoff, $i, $zindex);
    }
}

#
# An anode index too large to be valid:
#
$big_index = &efs'GetSuperMember('nBlocks') * $efs'anodes_per_block;

sub check_zlc_count {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    local($zcount) = &get_zlc_count($fsnew);
    local($zindex) = &get_zlc_file($fsnew, 0);
    local($expected) = ($zindex == 0) ? 0 : (-1);
    return 0 if ($zcount == $expected);
    &salverr($name, $index, "expected ZLC count == $expected, got $zcount\n");
}

#
# Vol14.  The ZLC number is either zero, greater than N_ZERO_LINKS and
# less than the number of anodes in the volume, or is the special
# value NO_COUNT (-1).
#
# repair action: set ZLC number equal to zero if the ZLC list contains
# all zeroes, and to NO_COUNT otherwise.
#
sub test_vol14 {
    local($this_test) = 'vol14';
    &print_start_message($this_test, 'Try bad ZLC count');
    local($errors) = &test_volume(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol14a, *check_zlc_count);
    $errors +=&test_volume(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol14b, *check_zlc_count);
    $errors +=&test_volume(
	$this_test . 'c', $backing_FS_name, $backing_FS,
	*break_vol14c, *check_zlc_count);
    $errors +=&test_volume(
	$this_test . 'd', $backing_FS_name, $backing_FS,
	*break_vol14d, *check_zlc_count);
    &print_end_message($this_test, $errors);
}

sub break_vol14a {
    &break_vol14_common(@_, -2);
}

sub break_vol14b {
    &break_vol14_common(@_, 5);
}

sub break_vol14c {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &fill_zlc_list($anode, $aoff, $fs, $fsoff);
    &break_vol14_common(@_, 5);
}

sub break_vol14d {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &fill_zlc_list($anode, $aoff, $fs, $fsoff);
    &break_vol14_common(@_, $big_index);
}

sub break_vol14_common {
    local($test, $name, $anode, $aoff, $fs, $fsoff, $count) = @_;
    &test_info($test, $name, "setting ZLC count to $count\n");
    &set_zlc_count($fs, $fsoff, $count);
}

#
# Vol15.  Each nonzero word in the ZLC list identifies a valid anode
# with linkCount equal to zero.
#
# repair action: remove invalid entries from the list.
#
sub test_vol15 {
    local($this_test) = 'vol15';
    &print_start_message($this_test, 'Try bad ZLC entries');
    local($errors) = &test_volume(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol15a, *check_vol15);
    $errors +=&test_volume(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol15b, *check_vol15);
    $errors +=&test_volume(
	$this_test . 'c', $backing_FS_name, $backing_FS,
	*break_vol15c, *check_vol15);
    &print_end_message($this_test, $errors);
}

sub break_vol15a {
    &break_vol15_common(@_, $zero[1]);
}

sub break_vol15b {
    &break_vol15_common(@_, $big_index);
}

sub break_vol15c {
    &break_vol15_common(@_, -1);
}

sub break_vol15_common {
    local($test, $name, $anode, $aoff, $fs, $fsoff, $zindex) = @_;
    &test_info($test, $name, "putting bad index $zindex on ZLC list\n");
    &set_zlc_file($fs, $fsoff, 0, $zindex);
    &set_zlc_count($fs, $fsoff, -1);
}

sub check_vol15 {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    local($zindex) = &get_zlc_file($fsnew, 0);
    &salverr($name, $index, "ZLC entry 0 not freed ($zindex)\n")
	unless ($zindex == 0);
    &check_zlc_count(@_);
}

#
# Vol16.  Each nonzero word in the ZLC list identifies a valid anode
# with linkCount equal to zero.
#
# repair action: remove invalid entries from the list.
#
sub test_vol16 {
    local($this_test) = 'vol16';
    &print_start_message($this_test, 'Try bad ZLC count');
    local($errors) = &test_volume(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol16a, *check_vol16);
    $errors +=&test_volume(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol16b, *check_vol16);
    &print_end_message($this_test, $errors);
}

#
# Try a real ZLC anode
#
sub break_vol16a {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local($znode, $zoff) = &make_zlc_anode($anode, $aoff, $fs, $fsoff);
    local($zindex) = &efs'GetAnodeMember($znode, 'index');
    &test_info($test, $name, "adding ZLC entry twice\n");
    &set_zlc_file($fs, $fsoff, 0, $zindex);
    &set_zlc_file($fs, $fsoff, 1, $zindex);
    &set_zlc_count($fs, $fsoff, -1);
}

#
# Fake a ZLC anode by clobbering a file's linkCount.
#
sub break_vol16b {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local($znode, $zoff) = @zero[0 .. 1];
    local($zindex) = &efs'GetAnodeMember($znode, 'index');
    &Set_Anode($znode, $zoff, 'linkCount', 0);
    &test_info($test, $name, "adding fake ZLC entry twice\n");
    &set_zlc_file($fs, $fsoff, 0, $zindex);
    &set_zlc_file($fs, $fsoff, 1, $zindex);
    &set_zlc_count($fs, $fsoff, -1);
}

sub check_vol16 {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    local($zindex) = &get_zlc_file($fsnew, 1);
    &salverr($name, $index, "duplicate ZLC entry 1 not freed ($zindex)\n")
	unless ($zindex == 0);
    &check_zlc_count(@_);
}

#
# Vol17.  If any word in the ZLC is zero, all following words must
# also be zero.
#
# repair action: rearrange the entries so that the zeroes are at
# the end.
#
sub test_vol17 {
    local($this_test) = 'vol17';
    &print_start_message($this_test, 'Try bad ZLC list order');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol17, *check_vol17);
    &print_end_message($this_test, $errors);
}

sub break_vol17 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local($znode, $zoff) = &make_zlc_anode($anode, $aoff, $fs, $fsoff);
    local($zindex) = &efs'GetAnodeMember($znode, 'index');
    &test_info($test, $name, "adding ZLC entry in second position\n");
    &set_zlc_file($fs, $fsoff, 1, $zindex);
    &set_zlc_count($fs, $fsoff, -1);
}

sub check_vol17 {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    local($zindex0) = &get_zlc_file($fsnew, 0);
    local($zindex1) = &get_zlc_file($fsnew, 1);
    &salverr($name, $index, "entries not rearranged\n")
	unless ($zindex0 != 0 && $zindex1 == 0);
    &check_zlc_count(@_);
}

#
# Vol18.  If the ZLC number is nonzero and not equal to NO_COUNT, then
# all the entries is the ZLC list must be nonzero.
#
# repair action: set ZLC number to zero or NO_COUNT, depending on the
# contents of the ZLC list.
#
sub test_vol18 {
    local($this_test) = 'vol18';
    &print_start_message($this_test, 'Try inconsistent ZLC count > 0');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol18, *check_zlc_count);
    &print_end_message($this_test, $errors);
}

sub break_vol18 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "setting ZLC count to positive value\n");
    &set_zlc_count($fs, $fsoff, 12);
}

#
# Vol19.  If the ZLC number is NO_COUNT, then at least one entry in the
# ZLC list must be nonzero.

# repair action: set ZLC number to zero.
#
sub test_vol19 {
    local($this_test) = 'vol19';
    &print_start_message($this_test, 'Try inconsistent ZLC count == -1');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol19, *check_zlc_count);
    &print_end_message($this_test, $errors);
}

sub break_vol19 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "setting ZLC count to NO_COUNT (-1)\n");
    &set_zlc_count($fs, $fsoff, -1);
}

#
# Vol20.  If the ZLC number is zero, then all entriess in the ZLC list
# must be zero.
#
# repair action: set ZLC number to NO_COUNT.
#
sub test_vol20 {
    local($this_test) = 'vol20';
    &print_start_message($this_test, 'Try inconsistent ZLC count == -0');
    local($errors) = &test_volume(
	$this_test, $backing_FS_name, $backing_FS,
	*break_vol20, *check_zlc_count);
    &print_end_message($this_test, $errors);
}

sub break_vol20 {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    local($znode, $zoff) = &make_zlc_anode($anode, $aoff, $fs, $fsoff);
    local($zindex) = &efs'GetAnodeMember($znode, 'index');
    &test_info($test, $name, "adding ZLC entry to empty list\n");
    &set_zlc_file($fs, $fsoff, 0, $zindex);
}

#
# Vol21.  allocated.usage contains the total number of fragments that belong
# to the volume, excluding the volume anode.
#
# repair action: recalculate by inspection of the volume data structures.
#
sub test_vol21 {
    local($this_test) = 'vol21';
    &print_start_message($this_test, 'Try incorrect allocated.usage');
    local($errors) = &test_volume(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol21a, *check_ausage);
    local($errors) = &test_volume(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol21b, *check_ausage);
    &print_end_message($this_test, $errors);
}

sub break_vol21a {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "setting allocated.usage too low\n");
    local($ausage) = &efs'GetFSMember($fs, 'allocated.usage');
    &Set_FileSet($fs, $fsoff, 'allocated.usage', $ausage - 1);
}

sub break_vol21b {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "setting allocated.usage too high\n");
    local($ausage) = &efs'GetFSMember($fs, 'allocated.usage');
    &Set_FileSet($fs, $fsoff, 'allocated.usage', $ausage + 1);
}

sub check_ausage {
    local($name, $index, $aold, $anew, $fsold, $fsnew) = @_;
    &fs_diff($name, $index, $fsold, $fsnew, 'allocated.usage');
}

#
# Vol22.  allocated.usage <= allocated.limit
#
# repair action: recalculate allocated.usage; mark volume inconsistent
# if the condition is still violated.
#
sub test_vol22 {
    local($this_test) = 'vol22';
    &print_start_message($this_test, 'Try violating quota limit');
    local($errors) = &test_volume(
	$this_test . 'a', $backing_FS_name, $backing_FS,
	*break_vol22a, *check_ausage);
    local($errors) = &test_volume(
	$this_test . 'b', $backing_FS_name, $backing_FS,
	*break_vol22b, *check_incons);
    &print_end_message($this_test, $errors);
}

#
# Raise usage above limit (repairable)
#
sub break_vol22a {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "raising allocated.usage over limit\n");
    local($alimit) = &efs'GetFSMember($fs, 'allocated.limit');
    &Set_FileSet($fs, $fsoff, 'allocated.usage', $alimit + 1);
}

#
# Lower limit below usage (not repairable)
#
sub break_vol22b {
    local($test, $name, $anode, $aoff, $fs, $fsoff) = @_;
    &test_info($test, $name, "shrinking allocated.limit below usage\n");
    local($ausage) = &efs'GetFSMember($fs, 'allocated.usage');
    &Set_FileSet($fs, $fsoff, 'allocated.limit', $ausage - 1);
}

#
# TODO -- We still need the equivalent tests for visible quota.
#

1;
