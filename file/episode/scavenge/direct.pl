#
# Directory tests for salvager test suite
#
# /* Copyright (C) 1993 Transarc Corporation - All rights reserved */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/scavenge/RCS/direct.pl,v 1.4 1993/09/08 18:45:18 ota Exp $

sub get_dir_page {
    &efs'DirPageGet(*AGGR, @_);
}

sub get_dir_entry {
    local($dir_anode, $name) = @_;
    local(@dh) = &get_dir_page(0, $dir_anode);
    return () unless @dh;
    local(@result) = &GetDirEntry(*dh, q/$de{'name'} eq '/ . $name . "'");
    return @result[$#result - 1 .. $#result];
}

sub get_root_dir {
    local($fs_index) = @_;
    local(@fs_info) = &get_fs_info($fs_index);
    local(@result) = &GetFSAnodeFromIndex(*fs_info, $efs'DIR_ROOT_INDEX);
    return @result[$#result - 1 .. $#result];
}

#
# Episode's hashing function for directory entries.
#
# XXX contains needless cruft.
#
sub dir_Hash {
    local($name) = @_;
    local($i, $c, $h, $result);
    $h = 0;
    for ($i = 0; $i != length($name); $i++) {
	$c = ord(substr($name, $i, 1));
	$h = ($h << 7) + ($h << 5) + ($h << 3) + ($h << 2) + $h + $c;
    }
    $result = $c & ($efs'DIR_HASH_SIZE - 1);
    $result = $efs'DIR_HASHSIZE - $result
	if ($result != 0 && ($h & 0x80000000));
    return $result;
}

#
# Get the hash value for a directory entry; return a special
# out-of-range value if the entry is not in use.
#
sub get_hash {
    local($de) = @_;
    local($inuse, $hash);
    $inuse = &efs'GetDirEntryMember($de, 'inuse');
    $hash = $inuse ? &dir_Hash(&get_name($de)) : $efs'DIR_HASH_SIZE;
}

sub get_name {
    local($de) = @_;
    local($name) = &util'MapGet($de, 'name', &efs'DirEntryGetMap($de));
    local($i) = index($name, "\0");
    return ($i == -1) ? $name : substr($name, 0, $i);
}

#
# Dir1.  Directory length must be a multiple of PIECESIZE (32)
#
# repair action: set length based on header information.
#
sub test_dir1 {
    local($this_test) = 'dir1';
    &print_start_message($this_test, 'Try invalid directory length');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_dir1, *check_dir1, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir1 {
    local($test, $anode, $aoff, $name) = @_;
    local($length) = &efs'GetAnodeMember($anode, 'length2');
    &test_info($test, $name, "incrementing length\n");
    &Set_Anode($anode, $aoff, 'length2', $length + 1);
}

sub check_dir1 {
    &anode_diff(@_, 'length2');
}

#
# Dir2.  Directory header properties:
#
# 1. dir_magic == htonl(DIR_MAGIC)
# 2. flist is zero or identifies a directory entry
# 3. if ntohs(pgcount) == 1, then pnhimark == anode length / PIECESIZE
# 4. ntohs(pgcount) == howmany(anode length, DIRPAGESIZE)
#
# repair action: free anode if dir_magic bad, else rebuild directory.
#
sub test_dir2 {
    local($this_test) = 'dir2';
    &print_start_message($this_test, 'Try invalid directory header');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir2a, *check_anode_free, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir2b, *check_dir2, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'c',
		*break_dir2c, *check_dir2, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'd',
		*break_dir2d, *check_dir2, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir2a {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $doff) = &get_dir_page(0, $anode);
    &test_info($test, $name, "clobbering magic number\n");
    &Set_DirPage($dh, $doff, 'dir_magic', 42);
}

sub break_dir2b {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $doff) = &get_dir_page(0, $anode);
    &test_info($test, $name, "clobbering free list\n");
    &Set_DirPage($dh, $doff, 'flist', $efs'DIR_ENTRY_FIRST_ENTRY);
}

sub break_dir2c {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $doff) = &get_dir_page(0, $anode);
    &test_info($test, $name, "clobbering high-water mark\n");
    &Set_DirPage($dh, $doff, 'pnhimark', 0);
}

sub break_dir2d {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $doff) = &get_dir_page(0, $anode);
    &test_info($test, $name, "clobbering page count\n");
    &Set_DirPage($dh, $doff, 'pgcount', -1);
}

sub check_dir2 {
    local($old, $new, $name, $index) = @_;
    local($dh, $doff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($magic) = &efs'GetDirPageMember($dh, 'dir_magic');
    return &salverr($name, $index, "bad magic number %#x\n", $magic)
	unless ($magic == $efs'DIR_HEADER_MAGIC);
    local($flist) = &efs'GetDirPageMember($dh, 'flist');
    if ($flist != 0) {
        local($entry) = $flist;
	local($de, $deoff) = &efs'DirEntryGet($entry, $dh, $doff);
	return &salverr($name, $index, "no directory piece at offset $flist\n")
	    unless $de;
	local($demagic) = &efs'GetDirEntryMember($de, 'dir_entry_magic');
	return &salverr($name, $index, "bad flist value $flist\n")
	    unless ($demagic == $efs'DIR_NONENTRY_MAGIC);
    }
    local($length) = &efs'GetAnodeMember($new, 'length2');
    local($himark) = &efs'GetDirPageMember($dh, 'pnhimark');
    return &salverr($name, $index, "bad high water mark $himark\n")
	unless ($himark * $efs'DIR_PIECE_SIZE == $length);
    local($pgcount) = &efs'GetDirPageMember($dh, 'pgcount');
    return &salverr($name, $index, "bad page count $pgcount\n")
	unless ($pgcount == &util'HowMany($length, $efs'DIR_PAGE_SIZE));
    return 0;
}

#
# Dir3.  Each directory page following the first contains a page
# header satisfying
#	1. dir_magic == htonl(DIR_PAGE_MAGIC);
#	2. flist is zero or identifies a free directory entry.
#
# repair action: mark directory page invalid; rebuild directory
#
sub test_dir3 {
    local($this_test) = 'dir3';
    local(@bigdir) = &GetInfoByName($big_dir, @dirs);
    &print_start_message($this_test, 'Try bad 2nd directory page header');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir3a, *check_dir3, *bigdir);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir3b, *check_dir3, *bigdir);
    &print_end_message($this_test, $errors);
}

sub break_dir3a {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(1, $anode);
    &test_info($test, $name, "clobbering magic number\n");
    &Set_DirPage($dh, $dhoff, 'dir_magic', 42);
}

sub break_dir3b {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(1, $anode);
    &test_info($test, $name, "clobbering free list\n");
    &Set_DirPage($dh, $dhoff, 'flist', 1);
}

sub check_dir3 {
    local($old, $name, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(1, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($magic) = &efs'GetDirPageMember($dh, 'dir_magic');
    return &salverr($name, $index, "bad magic number %#x\n", $magic)
	unless ($magic == $efs'DIR_PAGE_MAGIC);
    local($flist) = &efs'GetDirPageMember($dh, 'flist');
    return 0 unless $flist == 1;
    &salverr($name, $index, "bad flist value $flist\n")
}

#
# Dir4.  Each entry in the per-page hashTable is either zero or
# identifies a valid directory entry.
#
# repair action: recompute hash table and free list.
#
sub test_dir4 {
    local($this_test) = 'dir4';
    &print_start_message($this_test, 'Try invalid hash table');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir4a, *check_dir4, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir4b, *check_dir4, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'c',
		*break_dir4c, *check_dir4, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir4a {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local(@hash) = &efs'GetDirHash($dh);
    @saved_hash = @hash;
    local($i);
    &test_info($test, $name, "setting hashTable to all zeroes\n");
    $saved_dh = $dh;
    for ($i = 0; $i != 128; $i++) {
	$hash[$i] = 0;
    }
    &Set_DirHash(*hash, $dhoff);
}

sub break_dir4b {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local(@hash) = &efs'GetDirHash($dh);
    @saved_hash = @hash;
    local($i);
    &test_info($test, $name, "adding bad entries to hashTable\n");
    for ($i = 0; $i != 128; $i++) {
	$hash[$i] = 4 if ($hash[$i] == 0);
    }
    &Set_DirHash(*hash, $dhoff);
}

sub break_dir4c {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local(@hash) = &efs'GetDirHash($dh);
    @saved_hash = @hash;
    local($i, $h, $last);
    &test_info($test, $name, "rearranging hashTable entries\n");
    $last = 0;
    for ($i = 0; $i != 128; $i++) {
	$h = $hash[$i];
	$hash[$i] = $last;
	$last = $h;
    }
    &Set_DirHash(*hash, $dhoff);
}

sub check_dir4 {
    local($old, $new, $name, $index) = @_;
    local($dh, $doff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local(@new_hash) = &efs'GetDirHash($dh);
    local($i);
    for ($i = 0; $i != 128; $i++) {
	return &salverr($name, $index, "hash table not restored\n")
	    if ($saved_hash[$i] != $new_hash[$i]);
    }
    return 0;
}

#
# Dir5.  In each directory entry, dir_entry_magic is DIR_ENTRY_MAGIC
# or DIR_NONENTRY_MAGIC.
#
# repair action: mark entry invalid; rebuild directory
#
sub test_dir5 {
    local($this_test) = 'dir5';
    &print_start_message($this_test, 'Try bad dir entry magic no.');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_dir5, *check_dir5, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir5 {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "trashing directory entry magic nos.\n");
    while ($i <= 20) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	&Set_DirEntry($de, $deoff, 'dir_entry_magic', 1993);
    }
}

sub check_dir5 {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($de, $deoff, $magic);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    while ($i <= 20) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$magic = &efs'GetDirEntryMember($de, 'dir_entry_magic');
	return &salverr($name, $index, "bad magic number %#x\n",
	    $magic) unless ($magic == $efs'DIR_NONENTRY_MAGIC);
    }
}

#
# Dir6.  The inuse entry in each member must be zero or one.
#
# repair action: mark entry invalid; rebuild directory
#
sub test_dir6 {
    local($this_test) = 'dir6';
    &print_start_message($this_test, 'Try illegal inuse values');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir6a, *check_dir_entries, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir6b, *check_dir_entries, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir6a {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff, $inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "incrementing inuse on in-use entries\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next if ($inuse == 0);
	&Set_DirEntry($de, $deoff, 'inuse', $i);
    }
}

sub break_dir6b {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff, $inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "setting inuse on free entries\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next if ($inuse != 0);
	&Set_DirEntry($de, $deoff, 'inuse', -1);
    }
}

sub check_dir_entries {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($de, $deoff, $magic, $inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$magic = &efs'GetDirEntryMember($de, 'dir_entry_magic');
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	return &salverr($name, $index, "magic %#x, inuse $inuse\n",
	    $magic) unless
	    ($magic == $efs'DIR_NONENTRY_MAGIC && $inuse == 0 ||
	     $magic == $efs'DIR_ENTRY_MAGIC && $inuse == 1);
    }
    return 0;
}

#
# Dir7.  inuse == 1 iff dir_entry_magic == DIR_ENTRY_MAGIC.
#
# repair action: mark entry invalid; rebuild directory
#
sub test_dir7 {
    local($this_test) = 'dir7';
    &print_start_message($this_test, 'Try inconsistent inuse values');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir7a, *check_dir_entries, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir7b, *check_dir_entries, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir7a {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff, $inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "clearing inuse on in-use entries\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next if ($inuse == 0);
	&Set_DirEntry($de, $deoff, 'inuse', 0);
    }
}

sub break_dir7b {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff, $inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "setting inuse on free entries\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next if ($inuse != 0);
	&Set_DirEntry($de, $deoff, 'inuse', 1);
    }
}

#
# Dir8.  If an entry is in use, its next member identifies a distinct
# in-use entry with the same hash value (or is zero); if an entry is
# free, its next member identifies a distinct free entry (or is zero).
#
# repair action: rebuild directory
#
sub test_dir8 {
    local($this_test) = 'dir8';
    local(@bigdir) = &GetInfoByName($big_dir, @dirs);
    &print_start_message($this_test, 'Try illegal next links');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir8a, *check_entry_lists, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir8b, *check_entry_lists, *bigdir);
    $errors += &test_anodes($backing_FS, $this_test . 'c',
		*break_dir8c, *check_entry_lists, *bigdir);
    $errors += &test_anodes($backing_FS, $this_test . 'd',
		*break_dir8d, *check_entry_lists, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir8a {
    &make_cycles(@_, 1);
}

sub break_dir8b {
    &make_cycles(@_, 0);
}

sub make_cycles {
    local($test, $anode, $aoff, $name, $self_loop) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff, $this, $next, $p);
    local(@pred);
    &test_info($test, $name, "make%s linked lists circular\n",
	$self_loop ? "" : " long");
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	$this = $i;
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$next = &efs'GetDirEntryMember($de, 'next');
	$pred[$next] = $this;
    }
    $i = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	$this = $i;
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$p = $this;
	$p = $pred[$p] until ($pred[$p] == 0);
	next if (!$self_loop && $p == $this);
	&Set_DirEntry($de, $deoff, 'next', $p);
    }
}

sub break_dir8c {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff, $this, $next, $p);
    local(@pred, @head, @next);
    &test_info($test, $name, "make%s linked lists circular\n",
	$self_loop ? "" : " long");
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	$this = $i;
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$next = &efs'GetDirEntryMember($de, 'next');
	$pred[$next] = $this;
    }
    $i = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	$this = $i;
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$p = $this;
	$p = $pred[$p] until ($pred[$p] == 0);
	next if ($p == $this);
	next if ($p == $head[0]);
	push(@head, $p);
	last if ($#head == 1);
    }
    return () unless ($#head == 1);
    &test_info($test, $name, "crossing two lists\n");
    $i = $head[0];
    ($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
    $next[0] = &efs'GetDirEntryMember($de, 'next');
    ($de, $deoff) = &efs'DirEntryGet($head[1], $dh, $dhoff);
    $next[1] = &efs'GetDirEntryMember($de, 'next');
    &Set_DirEntry($de, $deoff, 'next', $next[0]);
    ($de, $deoff) = &efs'DirEntryGet($head[0], $dh, $dhoff);
    &Set_DirEntry($de, $deoff, 'next', $next[1]);
}

sub break_dir8d {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff, $next);
    &test_info($test, $name, "add invalid pointers\n");
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$next = &efs'GetDirEntryMember($de, 'next');
	next unless ($next == 0);
	&Set_DirEntry($de, $deoff, 'next', 1);
    }
}

#
# Check that all the flist pointers for a directory page are valid.
#
sub check_entry_lists {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($de, $deoff, $this, $next, $h, $p);
    local($next);
    local(@pred, @head);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;

    for (;;) {
	$this = $i;
	$pred[$this] = 0 unless defined($pred[$this]);
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$next = &efs'GetDirEntryMember($de, 'next');
	$pred[$next] = $this;
    }

    $i = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	$this = $i;
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$next = &efs'GetDirEntryMember($de, 'next');
	next unless ($next == 0);
	$h = &get_hash($de);
	if ($head[$h] != 0) {
	     local($what) = ($h == $efs'DIR_HASH_SIZE) ?
		"free entries" : "hash value $h";
	     return &salverr($name, $index,
		"two lists with $what, at $head[$h], $this\n");
	}
	$p = $pred[$this];
	while ($p != 0) {
	    return &salverr($name, $index,
		"cycle in flist at piece $p\n") if ($p == $this);
	    ($de, $deoff) = &efs'DirEntryGet($p, $dh, $dhoff);
	    $hash = &get_hash($de);
	    return &salverr($name, $index,
		"list at $this contains hash values $h, $hash\n")
		if ($hash != $h);
	    $p = $pred[$p];
	}
    }
    return 0;
}

#
# Dir9.  Each free entry is followed by an in-use entry or the
# end of the directory page.
#
# repair action: coalesce adjacent free entries.
#
sub test_dir9 {
    local($this_test) = 'dir9';
    &print_start_message($this_test, 'Try adjacent free entries');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_dir9, *check_dir9, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir9 {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff, $nextoff, $this);
    local($magic, $length);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	$this = $i;
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$magic = &efs'GetDirEntryMember($de, 'dir_entry_magic');
	next unless ($magic == $efs'DIR_NONENTRY_MAGIC);
	$length = &efs'GetDirEntryMember($de, 'length');
	last if ($length > 1);
    }
    if ($de) {
	&test_info($test, $name, "splitting free entry\n");
	&Set_DirEntry($de, $deoff, 'length', 1);
	$nextoff = $deoff + $efs'DIR_PIECE_SIZE;
	&WriteData($de, $nextoff);
	&Set_DirEntry($de, $deoff, 'next', $this + 1);
	&Set_DirEntry($de, $nextoff, 'length', $length - 1);
        local($oldlen) = &efs'GetAnodeMember($anode, 'length2');
	local($newlen) = ($this + 2) * $efs'DIR_PIECE_SIZE;
	&Set_Anode($anode, $aoff, 'length2', $newlen) if ($newlen > $oldlen);
    }
}

sub check_dir9 {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($de, $deoff);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    local($magic, $lastmagic) = 0;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$magic = &efs'GetDirEntryMember($de, 'dir_entry_magic');
	return &salverr($name, $index, "uncoalesced free entries\n")
	    if ($magic == $efs'DIR_NONENTRY_MAGIC && $magic == $lastmagic);
	$lastmagic = $magic;
    }
    return 0;
}

#
# Dir10.  Each name begins with a nonzero byte and is terminated by
# a zero byte; none of the bytes preceding the terminator contains '/'.
#
# repair action: free entry; rebuild directory.
#
sub test_dir10 {
    local($this_test) = 'dir10';
    &print_start_message($this_test, 'Try invalid file names');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir10a, *check_valid_names, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir10b, *check_valid_names, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'c',
		*break_dir10c, *check_valid_names, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir10a {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "making file names null\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	&Set_DirEntry($de, $deoff, 'name', "");
    }
}

sub break_dir10b {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($length, $inuse, $maxname, $longname);
    local($namechar) = 'a';
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "creating unterminated names\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	$length = &efs'GetDirEntryMember($de, 'length');
	$maxname =
	    $length * $efs'DIR_PIECE_SIZE - $efs'DIR_ENTRY_HEADER_SIZE;
	$longname = $namechar++ x $maxname;
	&Set_DirEntry($de, $deoff, 'name', $longname);
    }
}

sub break_dir10c {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($inuse, $fname);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "adding / to file names\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	$fname = &get_name($de) . '/';
	&Set_DirEntry($de, $deoff, 'name', $fname);
    }
}

#
# Iterate through directory page and validate all names.
#
sub check_valid_names {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($length, $fname, $maxname);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	$fname = &get_name($de);
        &salverr($name, $index, "empty name in dir entry\n")
	   if (length($fname) == 0);
	$length = &efs'GetDirEntryMember($de, 'length');
	$maxname =
	    $length * $efs'DIR_PIECE_SIZE - $efs'DIR_ENTRY_HEADER_SIZE;
	&salverr($name, $index, "unterminated name in dir entry\n")
	    if (length($fname) > $maxname);
	&salverr($name, $index, "name %s contains slash\n", $fname)
	    unless (index($fname, '/') == -1);
    }
    return 0;
}

#
# Dir11.  In each in-use entry, length equals the minimum number
# of pieces required to hold the entry's name.
#
# repair action: if length is too small, discard the entry,
# else shrink it and free the excess pieces.
#
sub test_dir11 {
    local($this_test) = 'dir11';
    &print_start_message($this_test, 'Try invalid file names');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir11a, *check_dir11, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir11b, *check_dir11, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir11a {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($inuse, $length);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	$length = &efs'GetDirEntryMember($de, 'length');
	next if ($length == 1);
	&test_info($test, $name, "shrinking directory entry\n");
	&Set_DirEntry($de, $deoff, 'length', 1);
    }
}

sub break_dir11b {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($len0, $len1, $inuse, $this, $last);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    for (;;) {
	$this = $i;
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	if ($inuse) {
	    $last = $this;
	    $len0 = &efs'GetDirEntryMember($de, 'length');
	} else {
	    &test_info($test, $name, "expanding directory entry\n");
	    $len1 = &efs'GetDirEntryMember($de, 'length');
	    ($de, $deoff) = &efs'DirEntryGet($last, $dh, $dhoff);
	    &Set_DirEntry($de, $deoff, 'length', $len0 + $len1);
	}
    }
}

sub check_dir11 {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($length, $expect, $fname);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	$fname = &get_name($de);
	$expect = &util'HowMany(length($fname), $efs'DIR_PIECE_SIZE);
	$length = &efs'GetDirEntryMember($de, 'length');
	return &salverr($name, $index,
	    "expected dir entry length $expect, got $length\n")
	    unless ($expect == $length)
    }
    return 0;
}

#
# Dir12.  In each in-use entry, fid.index describes a regular anode.
#
# repair action: discard the entry.
#
sub test_dir12 {
    local($this_test) = 'dir12';
    &print_start_message($this_test, 'Try invalid fid.index');
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir12a, *check_dir12, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir12b, *check_dir12, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir12a {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "setting fid.index to zero\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	&Set_DirEntry($de, $deoff, 'fid.index', 0);
    }
}

sub break_dir12b {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local(@fs_info) = &get_fs_info($backing_FS);
    local($aux, $auxoff) = &make_aux_anode(@fs_info);
    local($auxindex) = &efs'GetAnodeMember($aux, 'index');
    local($auxfid) = &efs'FSIndexToFid($auxindex);
    local($inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    &test_info($test, $name, "setting fid.index to aux anode\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	&Set_DirEntry($de, $deoff, 'fid.index', $auxfid);
    }
}

sub check_dir12 {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local(@fs_info) = &get_fs_info($backing_FS);
    local(@result);
    local($fid, $index, $anode, $flags);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	$fid = &efs'GetDirEntryMember($de, 'fid.index');
	$index = &efs'FSFidToIndex($fid);
	@result = &GetFSAnodeFromIndex(*fs_info, $index);
	return &salverr($name, $index, "no anode at fid.index $fid\n")
	    unless (@result);
	$anode = $result[$#result - 1];
	$flags = &efs'GetAnodeMember($anode, 'flags');
	return &salverr($name, $index,
	    "anode at fid.index $fid not a file\n")
	    unless (($flags & $efs'AFLAGS_STATUSSIZE_MASK) ==
	    $efs'ANODE_FILE_STATUS_SIZE);

    }
    return 0;
}

#
# Dir13.  The same name does not appear twice in in-use entries
#
# repair action: discard all but one entry.
#
sub test_dir13 {
    local($this_test) = 'dir13';
    &print_start_message($this_test, 'Try duplicated names');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_dir13, *check_dir13, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir13 {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($inuse, $dupname);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY + 2;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	if ($dupname) {
	    &test_info($test, $name, "duplicating name $dupname\n");
	    &Set_DirEntry($de, $deoff, 'name', $dupname);
	    last;
	} else {
	    $dupname = &get_name($de);
	}
    }
}

sub check_dir13 {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($de, $deoff);
    local($fname, $name2);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	$name2 = &get_name($de);
	if ($fname) {
	    return 0 unless ($name2 eq $fname);
	    return &salverr($name, $index, "name %s appears twice\n", $fname);
	} else {
	    $fname = $name2;
	}
    }
    return 0;
}

#
# Dir14.  At least two entries must be in use.
#
# repair action: mark the anode invalid.
#
sub test_dir14 {
    local($this_test) = 'dir14';
    &print_start_message($this_test, 'Try marking all entries unused');
    local($errors) = &test_anodes($backing_FS, $this_test,
		*break_dir14, *check_anode_free, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir14 {
    local($test, $anode, $aoff, $name) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    &test_info($test, $name, "freeing all entries\n");
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	&Set_DirEntry($de, $deoff, 'inuse', 0);
	&Set_DirEntry($de, $deoff,
	    'dir_entry_magic', $efs'DIR_NONENTRY_MAGIC);
    }
}

sub delete_entry {
    local($test, $dname, $anode, $ename) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $anode);
    local($de, $deoff);
    local($inuse);
    local($i) = $efs'DIR_ENTRY_FIRST_ENTRY;
    for (;;) {
	($de, $deoff) = &efs'DirEntryGet($i, $dh, $dhoff);
	last unless $de;
	$inuse = &efs'GetDirEntryMember($de, 'inuse');
	next unless $inuse;
	last if (&get_name($de) eq $ename);
    }
    if ($de) {
	&test_info($test, $dname, "freeing entry for $ename\n");
	$saved_de = $de;
	&Set_DirEntry($de, $deoff, 'inuse', 0);
	&Set_DirEntry($de, $deoff,
	    'dir_entry_magic', $efs'DIR_NONENTRY_MAGIC);
    }
}

#
# Dir15.  There is an entry named '.' that identifies the directory anode.
#
# repair action: create a correct entry.
#
sub test_dir15 {
    local($this_test) = 'dir15';
    &print_start_message($this_test, "Try clobbering '.'");
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir15a, *check_dir15, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir15b, *check_dir15, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir15a {
    local($test, $anode, $aoff, $name) = @_;
    &delete_entry($test, $name, $anode, '.');
}

sub break_dir15b {
    local($test, $anode, $aoff, $name) = @_;
    local($de, $deoff) = &get_dir_entry($anode, '.');
    local($root, $roff) = &get_root_dir($backing_FS);
    local($rde, $rdeoff) = &get_dir_entry($root, '.');
    local($fid) = &efs'GetDirEntryMember($rde, 'fid.index');
    local($uniq) = &efs'GetDirEntryMember($rde, 'fid.unique');
    &test_info($test, $name, "linking '.' to root\n");
    $saved_de = $de;
    &Set_DirEntry($de, $deoff, 'fid.index', $fid);
    &Set_DirEntry($de, $deoff, 'fid.unique', $uniq);
}

sub entries_match {
    local($de1, $de2) = @_;
    &efs'GetDirEntryMember($de1, 'dir_entry_magic') ==
    &efs'GetDirEntryMember($de2, 'dir_entry_magic') &&
    &efs'GetDirEntryMember($de1, 'inuse') ==
    &efs'GetDirEntryMember($de2, 'inuse') &&
    &efs'GetDirEntryMember($de1, 'fid.index') ==
    &efs'GetDirEntryMember($de2, 'fid.index');
}

sub check_dir15 {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($de, $deoff) = &get_dir_entry($new, '.');
    return &salverr($name, $index, "no entry for '.'\n") unless $de;
    return 0 if &entries_match($saved_de, $de);
    &salverr($name, $index, "entry for '.' incorrect\n")
}

#
# Dir16.  Except for the root directory, each directory is referred
# to by (at least) one parent directory.
#
# repair action: move the directory to lost+found.
#
# XXX No checking for now, since it is difficult and the salvager
# doesn't do this repair anyway.
#
sub test_dir16 {
    local($this_test) = 'dir16';
    &print_start_message($this_test, "Try unlinking subtree");
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir16, *check_dir16, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir16 {
    local($test, $anode, $aoff, $name) = @_;
    local(@fs_info) = &get_fs_info($backing_FS);
    local(@result) = &GetFSAnodeFromIndex(*fs_info, $efs'DIR_ROOT_INDEX);
    local($root, $rootoff) = @result[$#result - 1, $#result];
    &delete_entry($test, 'root directory', $root, $name);
    &self_parent(@_);
}

sub check_dir16 {
    &logpr("## No checking implemented for test dir16\n");
    return 0;
}

#
# Dir17.  There is an entry named '..' that identifies the parent
# directory, or in the case of root, the directory itself.
#
# repair action: create a correct entry.
#
sub test_dir17 {
    local($this_test) = 'dir17';
    &print_start_message($this_test, "Try clobbering '..'");
    local($errors) = &test_anodes($backing_FS, $this_test . 'a',
		*break_dir17a, *check_dir17, *dirs);
    $errors += &test_anodes($backing_FS, $this_test . 'b',
		*break_dir17b, *check_dir17, *dirs);
    &print_end_message($this_test, $errors);
}

sub break_dir17a {
    local($test, $anode, $aoff, $name) = @_;
    &delete_entry($test, $name, $anode, '..');
}

sub break_dir17b {
     &self_parent(@_);
}

sub self_parent {
    local($test, $anode, $aoff, $name) = @_;
    local($dotde, $dotoff) = &get_dir_entry($anode, '.');
    local($de, $deoff) = &get_dir_entry($anode, '..');
    local($fid) = &efs'GetDirEntryMember($dotde, 'fid.index');
    local($uniq) = &efs'GetDirEntryMember($dotde, 'fid.unique');
    &test_info($test, $name, "linking '..' to '.'\n");
    $saved_de = $de;
    &Set_DirEntry($de, $deoff, 'fid.index', $fid);
    &Set_DirEntry($de, $deoff, 'fid.unique', $uniq);
}

sub check_dir17 {
    local($old, $new, $name, $index) = @_;
    local($dh, $dhoff) = &get_dir_page(0, $new);
    return &salverr($name, $index, "can't read directory header\n")
	unless $dh;
    local($de, $deoff) = &get_dir_entry($new, '..');
    return &salverr($name, $index, "no entry for '..'\n") unless $de;
    return 0 if &entries_match($saved_de, $de);
    &salverr($name, $index, "entry for '..' incorrect\n")
}

1;
