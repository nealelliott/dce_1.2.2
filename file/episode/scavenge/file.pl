# Copyright (C) 1993 Transarc Corporation - All rights reserved
#
# File-specific salvager tests
#

#
# File1. Each file has type AFLAGS_TYPE_REGULAR
#
# repair action: mark the anode free.
#
sub test_file1 {
    local($this_test) = 'file1';
    &print_start_message($this_test, 'Try invalid anode type');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_file1, *check_anode_free, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file1 {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    $flags = ($flags & ~$efs'AFLAGS_TYPE_MASK) | $efs'AFLAGS_TYPE_BITMAP;
    &test_info($test, $name, "changing type from regular to bitmap\n");
    &Set_Anode($anode, $aoff, 'flags', $flags);
}

#
# File2. Each file has 108 bytes of status information.
#
# repair action: mark the anode free.
#
sub test_file2 {
    local($this_test) = 'file2';
    &print_start_message($this_test, 'Try invalid status size');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_file2, *check_anode_free, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file2 {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    local($badsize) = $efs'ANODE_FILE_STATUS_SIZE + 1;
    $flags = ($flags & ~$efs'AFLAGS_STATUSSIZE_MASK) | $badsize;
    &test_info($test, $name, "setting incorrect status size $badsize\n");
    &Set_Anode($anode, $aoff, 'flags', $flags);
}

#
# File3. The IFMT bits in the file's mode must identify a valid
# file type.
#
# repair action: mark the anode free.
#
sub test_file3 {
    local($this_test) = 'file3';
    &print_start_message($this_test, 'Try invalid mode bits');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_file3, *check_anode_free, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file3 {
    local($test, $anode, $aoff, $name) = @_;
    local($mode) = &efs'GetAnodeMember($anode, 'mode');
    local($badfmt) = 0030000;
    $mode = ($mode & ~$efs'STAT_IFMT) | $badfmt;
    &test_info($test, $name, "setting invalid mode %#o\n", $badfmt);
    &Set_Anode($anode, $aoff, 'mode', $mode);
}

#
# File4. The LOGGED bit is set iff the file is a directory; the
# PREALLOCATED bit is not set.
#
# repair action: mark the anode free.
#
sub test_file4 {
    local($this_test) = 'file4';
    &print_start_message($this_test, 'Try LOGGED and PREALLOCATED bits');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_file4a, *check_anode_free, *regular);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_file4b, *check_anode_free, *regular);
    &print_end_message($this_test, $errors);
}

#
# Reverse the LOGGED bit
#
sub break_file4a {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    $flags ^= $efs'AFLAGS_LOGGED;
    &test_info($test, $name, "reversing LOGGED bit\n");
    &Set_Anode($anode, $aoff, 'flags', $flags);
}

#
# Set the PREALLOCATED bit
#
sub break_file4b {
    local($test, $anode, $aoff, $name) = @_;
    local($flags) = &efs'GetAnodeMember($anode, 'flags');
    $flags |= $efs'AFLAGS_PREALLOCATED;
    &test_info($test, $name, "setting PREALLOCATED bit\n");
    &Set_Anode($anode, $aoff, 'flags', $flags);
}

#
# File5. linkCount equals the number of directory entries that
# refer to the file.
#
# repair action: set linkCount correctly.
#
sub test_file5 {
    local($this_test) = 'file5';
    &print_start_message($this_test, 'Try invalid link count');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_file5a, *check_file5, *regular);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_file5b, *check_file5, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file5a {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting link count to zero\n");
    &Set_Anode($anode, $aoff, 'linkCount', 0);
}

sub break_file5b {
    local($test, $anode, $aoff, $name) = @_;
    local($count) = &efs'GetAnodeMember($anode, 'linkCount');
    &test_info($test, $name, "setting link count too high\n");
    &Set_Anode($anode, $aoff, 'linkCount', $count + 1);
}

sub check_file5 {
    &anode_diff(@_, 'linkCount');
}

#
# File6. linkCount is greater than zero unless the ZLC list identifies
# the file.
#
# repair action: place file in lost+found
#
# XXX The salvager doesn't do this yet, and we don't really check
# for it here.  We run the salvager anyway, just to see what happens.
#
sub test_file6 {
    local($this_test) = 'file6';
    local(@fs_info) = &get_fs_info($backing_FS);

    &print_start_message($this_test, 'Try zero link count file');
    &UseTmpAggr(1);
    &test_info($this_test, 'new file', "creating ZLC file\n");
    &make_zlc_anode(@fs_info);
    &RunSalvager();
    &test_info($this_test, 'new file', "checking not implemented\n");
    &print_end_message($this_test, 0);
}

#
# File7.  sflags and spares must be zero.
#
# repair action: set them to zero.
#
sub test_file7 {
    local($this_test) = 'file7';
    &print_start_message($this_test, 'Try setting spares to nonzero');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_file7, *check_file7, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file7 {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting spares to nonzero values\n");
    &Set_Anode($anode, $aoff, 'sflags', 1023);
    &Set_Anode($anode, $aoff, 'spares', 511);
    &Set_Anode($anode, $aoff, 'spare[0]', 255);
    &Set_Anode($anode, $aoff, 'spare[1]', 127);
    &Set_Anode($anode, $aoff, 'spare[2]', 63);
}

sub check_file7 {
    &anode_diff(@_, 'sflags') ||
    &anode_diff(@_, 'spares') ||
    &anode_diff(@_, 'spare[0]') ||
    &anode_diff(@_, 'spare[1]') ||
    &anode_diff(@_, 'spare[2]');
}

#
# File8.  acl must be zero or the index of a valid ACL anode.
#
# repair action: set acl to zero.
#
sub test_file8 {
    local($this_test) = 'file8';
    &print_start_message($this_test, 'Try bad ACL index');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_file8a, *check_file8, *regular);
    local($errors) = &test_anodes($backing_FS, $this_test . 'b',
		*break_file8b, *check_file8, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file8a {
    local($test, $anode, $aoff, $name) = @_;
    local($index) = &efs'GetAnodeMember($anode, 'index');
    &test_info($test, $name, "setting ACL to point to self\n");
    &Set_Anode($anode, $aoff, 'acl', $index);
}

sub break_file8b {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting ACL to invalid index\n");
    &Set_Anode($anode, $aoff, 'acl', -1);
}

sub check_file8 {
    local($old, $new, $path, $index) = @_;
    local($acl) = &efs'GetAnodeMember($new, 'acl');
    return 0 if ($acl == 0);
    &salverr($path, $index, "acl not cleared (%#x)\n", $acl);
}

#
# File9.  plist must be zero or the index of a valid plist anode.
#
# repair action: set to zero.
#
sub test_file9 {
    local($this_test) = 'file9';
    &print_start_message($this_test, 'Try bad plist index');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_file9a, *check_file9, *regular);
    local($errors) = &test_anodes($backing_FS, $this_test . 'b',
		*break_file9b, *check_file9, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file9a {
    local($test, $anode, $aoff, $name) = @_;
    local($index) = &efs'GetAnodeMember($anode, 'index');
    &test_info($test, $name, "setting plist to point to self\n");
    &Set_Anode($anode, $aoff, 'plist', $index);
}

sub break_file9b {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting plist to invalid index\n");
    &Set_Anode($anode, $aoff, 'plist', -1);
}

sub check_file9 {
    local($old, $new, $path, $index) = @_;
    local($plist) = &efs'GetAnodeMember($new, 'plist');
    return 0 if ($plist == 0);
    &salverr($path, $index, "plist not cleared (%#x)\n", $plist);
}

#
# File10.  reservedForUserPlist must be zero or the index of a valid
# plist anode.
#
# repair action: set to zero.
#
sub test_file10 {
    local($this_test) = 'file10';
    &print_start_message($this_test, 'Try bad user plist index');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_file10a, *check_file10, *regular);
    local($errors) = &test_anodes($backing_FS, $this_test . 'b',
		*break_file10b, *check_file10, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file10a {
    local($test, $anode, $aoff, $name) = @_;
    local($index) = &efs'GetAnodeMember($anode, 'index');
    &test_info($test, $name, "setting user plist to point to self\n");
    &Set_Anode($anode, $aoff, 'reservedForUserPlist', $index);
}

sub break_file10b {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting user plist to invalid index\n");
    &Set_Anode($anode, $aoff, 'reservedForUserPlist', -1);
}

sub check_file10 {
    local($old, $new, $path, $index) = @_;
    local($plist) = &efs'GetAnodeMember($new, 'reservedForUserPlist');
    return 0 if ($plist == 0);
    &salverr($path, $index, "user plist not cleared (%#x)\n", $plist);
}

#
# File11.  initialDirAcl and initialFileAcl must be zero on non-directories;
# when set, they must identify valid ACL anodes.
#
# repair action: set invalid values to zero.
#
sub test_file11 {
    local($this_test) = 'file11';
    &print_start_message($this_test, 'Try bad initial ACL indexes');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_file11a, *check_file11, *regular);
    local($errors) = &test_anodes($backing_FS, $this_test . 'b',
		*break_file11b, *check_file11, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file11a {
    local($test, $anode, $aoff, $name) = @_;
    local($index) = &efs'GetAnodeMember($anode, 'index');
    &test_info($test, $name, "setting ACL's to point to self\n");
    &Set_Anode($anode, $aoff, 'initialDirAcl', $index);
    &Set_Anode($anode, $aoff, 'initialFileAcl', $index);
}

sub break_file11b {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting ACL's to invalid index\n");
    &Set_Anode($anode, $aoff, 'initialDirAcl', -1);
    &Set_Anode($anode, $aoff, 'initialFileAcl', -1);
}

sub check_file11 {
    local($old, $new, $path, $index) = @_;
    local($dacl) = &efs'GetAnodeMember($new, 'initialDirAcl');
    local($facl) = &efs'GetAnodeMember($new, 'initialFileAcl');
    return 0 if ($dacl == 0 && $facl == 0);
    &salverr($path, $index, "acl's not cleared (%#x, %#x)\n", $dacl, $facl);
}

#
# File12.  volumeVersionNumber must not exceed the version number in
# the volume header.
#
# repair action: set equal to version number in the volume header.
#
sub test_file12 {
    local($this_test) = 'file12';
    &print_start_message($this_test, 'Try bad volume version number');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_file12, *check_file12, *regular);
    &print_end_message($this_test, $errors);
}

sub break_file12 {
    local($test, $anode, $aoff, $name) = @_;
    local(@fs_info) = &get_fs_info($backing_FS);
    local($fs) = $fs_info[2];
    local($version) = &efs'GetFSMember($fs, 'version2');
    &test_info($test, $name, "setting version number too high\n");
    &Set_Anode($anode, $aoff, 'volumeVersionNumber2', $version + 1);
}

sub check_file12 {
    local($old, $new, $path, $index) = @_;
    local($plist) = &efs'GetAnodeMember($new, 'plist');
    local(@fs_info) = &get_fs_info($backing_FS);
    local($fs) = $fs_info[2];
    local($version) = &efs'GetFSMember($fs, 'version2');
    local($vv) = &efs'GetAnodeMember($new, 'volumeVersionNumber2');
    return 0 if ($vv <= $version);
    &salverr($path, $index, "anode VV $vv exceeds volume version $version\n");
}

sub get_dev_info {
    &GetInfoByName(@_, @devices);
}

#
# Misc1.  Device special files have length sizeof (struct spec_file_info)
# (12 bytes).
#
# repair action: mark anode free
#
sub test_misc1 {
    local($this_test) = 'misc1';
    local(@dev_info) = &get_dev_info('devices/black_hole');
    &print_start_message($this_test, 'Try bad device file length');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_misc1a, *check_anode_free, *dev_info);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_misc1b, *check_anode_free, *dev_info);
    &print_end_message($this_test, $errors);
}

sub break_misc1a {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting length to zero\n");
    &Set_Anode($anode, $aoff, 'length2', 0);
}

sub break_misc1b {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting length too large\n");
    &Set_Anode($anode, $aoff, 'length2', 24);
}

#
# Misc2.  Sockets and fifos have length zero.
#
# repair action: set length to zero.
#
sub test_misc2 {
    local($this_test) = 'misc2';
    local(@fifo_info) = &get_dev_info('devices/pied_piper');
    &print_start_message($this_test, 'Try bad fifo length');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_misc2, *check_misc2, *fifo_info);
    &print_end_message($this_test, $errors);
}

sub break_misc2 {
    local($test, $anode, $aoff, $name) = @_;
    &test_info($test, $name, "setting length too large\n");
    &Set_Anode($anode, $aoff, 'length2', 64);
}

sub check_misc2 {
    &anode_diff(@_, 'length2');
}

#
# Misc3. Symbolic links have permission bits set to all ones.
#
# repair action: set them.
#
sub test_misc3 {
    local($this_test) = 'misc3';
    local(@link_info) = &get_dev_info('devices/piltdown');
    &print_start_message($this_test, 'Try bad symlink mode');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_misc3, *check_misc3, *link_info);
    &print_end_message($this_test, $errors);
}

sub break_misc3 {
    local($test, $anode, $aoff, $name) = @_;
    local($mode) = &efs'GetAnodeMember($anode, 'mode');
    &test_info($test, $name, "setting invalid symlink mode\n");
    &Set_Anode($anode, $aoff, 'mode', $mode & ~0777);
}

sub check_misc3 {
    &anode_diff(@_, 'mode');
}

sub test_aux {
    local($fs_index, $test_name, *break_rule, *check_repair) = @_;
    local(@fs_info) = &get_fs_info($fs_index);

    &UseTmpAggr(1);

    local($aux, $auxoff) = &make_aux_anode(@fs_info);
    local($znode, $zoff) = &make_zlc_anode(@fs_info);
    local($auxindex) = &efs'GetAnodeMember($aux, 'index');
    local($zindex) = &efs'GetAnodeMember($znode, 'index');
    &Set_Anode($znode, $zoff, 'acl', $auxindex);
    &Set_Anode($aux, $auxoff, 'linkCount', 1);

    &break_rule($test_name, $znode, $zoff, $aux, $auxoff);
    &RunSalvager();

    local(@result) = &GetFSAnodeFromIndex(*fs_info, $auxindex);
    return 1 unless (@result);
    ($aux, $auxoff) = @result[$#result -1 .. $#result];

    @result = &GetFSAnodeFromIndex(*fs_info, $zindex);
    return 1 unless (@result);
    ($znode, $zoff) = @result[$#result -1 .. $#result];

    &check_repair($znode, $zindex, $aux, $auxindex);
}

#
# Create an auxiliary anode for Aux tests.
#
sub make_aux_anode {
    local(@input) = @_;
    local(@result) = &GetFreeFSAnode(*input, $efs'ANODE_AUX_STATUS_SIZE);
    return @result[$#result - 1 .. $#result];
}

#
# Aux1. Each auxiliary anode has type AFLAGS_TYPE_REGULAR
#
# repair action: free the anode.
#
sub test_aux1 {
    local($this_test) = 'aux1';
    &print_start_message($this_test, 'Try bad auxiliary anode type');
    local($errors) = &test_aux($backing_FS, $this_test,
		*break_aux1, *check_aux_free);
    &print_end_message($this_test, $errors);
}

sub break_aux1 {
    local($test, $pnode, $poff, $aux, $auxoff) = @_;
    local($flags) = &efs'GetAnodeMember($aux, 'flags');
    $flags = ($flags & ~$efs'AFLAGS_TYPE_MASK) | $efs'AFLAGS_TYPE_LOG;
    &test_info($test, 'auxiliary anode', "changing type to log\n");
    &Set_Anode($aux, $auxoff, 'flags', $flags);
}

sub check_aux_free {
    local($pnode, $pindex, $aux, $auxindex) = @_;
    &check_anode_free($aux, $aux, 'auxiliary anode', $auxindex);
}

#
# Aux2. Each auxiliary anode has sixteen bytes of status info.
#
# repair action: free the anode.
#
sub test_aux2 {
    local($this_test) = 'aux2';
    &print_start_message($this_test, 'Try bad auxiliary status size');
    local($errors) = &test_aux($backing_FS, $this_test,
		*break_aux2, *check_aux_free);
    &print_end_message($this_test, $errors);
}

sub break_aux2 {
    local($test, $pnode, $poff, $aux, $auxoff) = @_;
    local($flags) = &efs'GetAnodeMember($aux, 'flags');
    $flags = ($flags & ~$efs'AFLAGS_STATUSSIZE_MASK) |
	$efs'ANODE_FILE_STATUS_SIZE;
    &test_info($test, 'auxiliary anode', "changing status size\n");
    &Set_Anode($aux, $auxoff, 'flags', $flags);
}

#
# Aux3. linkCount equals the number of references by files & dirs.
#
# repair action: adjust linkCount to match number of references.
#
sub test_aux3 {
    local($this_test) = 'aux3';
    &print_start_message($this_test, 'Try bad auxiliary link count');
    local($errors) = &test_aux($backing_FS, $this_test . 'a',
		*break_aux3a, *check_aux3);
    $errors += &test_aux($backing_FS, $this_test . 'b',
		*break_aux3b, *check_aux3);
    &print_end_message($this_test, $errors);
}

sub break_aux3a {
    local($test, $pnode, $poff, $aux, $auxoff) = @_;
    &test_info($test, 'auxiliary anode', "setting link count to zero\n");
    &Set_Anode($aux, $auxoff, 'linkCount', 0);
}

sub break_aux3b {
    local($test, $pnode, $poff, $aux, $auxoff) = @_;
    &test_info($test, 'auxiliary anode', "setting link count too high\n");
    &Set_Anode($aux, $auxoff, 'linkCount', 100);
}

sub check_aux3 {
    local($pnode, $pindex, $aux, $auxindex) = @_;
    local($count) = &efs'GetAnodeMember($aux, 'linkCount');
    return 0 if ($count == 1);
    &salverr('auxiliary anode', $auxindex,
	"expected linkCount == 1, got $count\n");
}

#
# Aux4. linkCount must be greater than zero (i.e., something must
# reference the auxiliary anode).
#
# repair action: free the anode.
#
sub test_aux4 {
    local($this_test) = 'aux4';
    &print_start_message($this_test, 'Try unreferenced auxiliary anode');
    local($errors) = &test_aux($backing_FS, $this_test,
		*break_aux4, *check_aux_free);
    &print_end_message($this_test, $errors);
}

sub break_aux4 {
    local($test, $pnode, $poff, $aux, $auxoff) = @_;
    &test_info($test, 'auxiliary anode', "removing reference to auxiliary\n");
    &Set_Anode($aux, $auxoff, 'linkCount', 0);
    &Set_Anode($pnode, $poff, 'acl', 0);
}

#
# Aux5. The auxiliary flags must define a valid type.
#
# repair action: free the anode.
#
sub test_aux5 {
    local($this_test) = 'aux5';
    &print_start_message($this_test, 'Try bad auxiliary type');
    local($errors) = &test_aux($backing_FS, $this_test,
		*break_aux5, *check_aux_free);
    &print_end_message($this_test, $errors);
}

sub break_aux5 {
    local($test, $pnode, $poff, $aux, $auxoff) = @_;
    local($badtype) = 4;
    &test_info($test, 'auxiliary anode', "setting type to $badtype\n");
    &Set_Anode($aux, $auxoff, 'aux_flags', $badtype);
}

#
# Aux6. spares must be zero.
#
# repair action: zero them.
#
sub test_aux6 {
    local($this_test) = 'aux6';
    &print_start_message($this_test, 'Try nonzero spares');
    local($errors) = &test_aux($backing_FS, $this_test,
		*break_aux6, *check_aux6);
    &print_end_message($this_test, $errors);
}

sub break_aux6 {
    local($test, $pnode, $poff, $aux, $auxoff) = @_;
    &test_info($test, 'auxiliary anode', "setting spares non-zero\n");
    &Set_Anode($aux, $auxoff, 'spare[0]', 31);
    &Set_Anode($aux, $auxoff, 'spare[1]', 17);
}

sub check_aux6 {
    local($pnode, $pindex, $aux, $auxindex) = @_;
    local($s0) = &efs'GetAnodeMember($aux, 'spare[0]');
    local($s1) = &efs'GetAnodeMember($aux, 'spare[1]');
    return 0 if ($s0 == 0 && $s1 == 0);
    &salverr('auxiliary anode', $auxindex,
	"spares not zero: (%#x, %#x)\n", $s0, $s1);
}

1;
