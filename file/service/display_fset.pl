#!/afs/tr/proj/tools/bin/perl

# /* Copyright (C) 1995 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/service/RCS/display_fset.pl,v 1.2 1995/05/17 21:00:23 rajesh Exp $

# Dump fileset dump

# Originally written by Bob Sidebotham.
# Cleaned up. -ota 950317

# Parameters for hex dump.
$NWORDS = 4;	# Number of words per line to dump
$NNWORDS = 4;	# Number to actually show.

require 'ctime.pl';

%codes = (
	  0x80,		VOLS_DUMP_HEADER_START,		
	  0x100,	VOLS_DUMP_HEADER_UNIQUEGEN,		
	  0x101,	OBS_VOLS_DUMP_HEADER_MAXQUOTA,	
	  0x102,	VOLS_DUMP_HEADER_MINQUOTA,		
	  0x103,	OBS_VOLS_DUMP_HEADER_DISKUSED,	
	  0x104,	OBS_VOLS_DUMP_HEADER_FILECOUNT,	
	  0x105,	VOLS_DUMP_HEADER_OWNER,		
	  0x106,	VOLS_DUMP_HEADER_CREATION_DATE,	
	  0x107,	VOLS_DUMP_HEADER_ACCESS_DATE,		
	  0x108,	VOLS_DUMP_HEADER_UPDATE_DATE,		
	  0x109,	VOLS_DUMP_HEADER_MOTD,
	  0x10a,	VOLS_DUMP_HEADER_VNODE,	  
	  0x10b,	VOLS_DUMP_HEADER_NOVNODE,		
	  0x10c,	VOLS_DUMP_HEADER_VOLVERSION,		
	  0x10d,	VOLS_DUMP_HEADER_BASETYPE,		
	  0x10e,	VOLS_DUMP_HEADER_STYLE,
	  0x10f,	VOLS_DUMP_HEADER_VISQUOTALIMIT,	
	  0x110,	VOLS_DUMP_HEADER_ALLOCLIMIT,		
	  0x111,	VOLS_DUMP_HEADER_MOTD_CODESET,	
	  0x112,	VOLS_DUMP_HEADER_INDEX_MAX,		
	  0x113,	VOLS_DUMP_HEADER_ENDVNODES,		
	  0x114,	VOLS_DUMP_HEADER_DUMP_DATE,		
	  0x115,	VOLS_DUMP_HEADER_N_UNIQUEGEN,
	  0x200,	VOLS_DUMP_VNODE_TYPE,			
	  0x201,	VOLS_DUMP_VNODE_LINKCOUNT,		
	  0x202,	VOLS_DUMP_VNODE_DATAVERSION,		
	  0x203,	VOLS_DUMP_VNODE_VOLVERSION,		
	  0x204,	VOLS_DUMP_VNODE_ACCESSTIME,		
	  0x205,	VOLS_DUMP_VNODE_MODTIME,		
	  0x206,	VOLS_DUMP_VNODE_CHANGETIME,		
	  0x207,	VOLS_DUMP_VNODE_SERVERMODTIME,	
	  0x208,	VOLS_DUMP_VNODE_GROUP,		
	  0x209,	VOLS_DUMP_VNODE_AUTHOR,		
	  0x20a,	VOLS_DUMP_VNODE_OWNER,		
	  0x20b,	VOLS_DUMP_VNODE_UNIXMODE,		
	  0x20c,	VOLS_DUMP_VNODE_ACL,			
	  0x20d,	VOLS_DUMP_VNODE_SIZE,			
	  0x20e,	VOLS_DUMP_VNODE_ACL_INLINE,		
	  0x20f,	VOLS_DUMP_VNODE_ACL_COPY,		
	  0x212,	VOLS_DUMP_VNODE_CLIENTATTRS,		
	  0x213,	VOLS_DUMP_VNODE_RDEV,			
	  0x214,	VOLS_DUMP_HEADER_INCONSISTENT,	
	  0xf001,	VOLS_DUMP_VNODE_DATA,			
	  0xf002,	VOLS_DUMP_VNODE_DIR,			
	  0x210,	VOLS_DUMP_VNODE_DIR_ENTRY,		
	  0x211,	VOLS_DUMP_VNODE_DIR_END,		
	  0x2ff,	VOLS_DUMP_VNODE_EOF,			
	  0x81,		VOLS_DUMP_HEADER_END			
);

$FTU_DUMP_TYPE_FULL = 0;
$FTU_DUMP_TYPE_DATE_WITH_DIRS = 1;
$FTU_DUMP_TYPE_VERS_WITH_DIRS = 2;
$FTU_DUMP_TYPE_DATE = 3;
$FTU_DUMP_TYPE_VERS = 4;

if ($STDIN = shift) {
    open(STDIN) || die("couldn't open $STDIN");
}

while (read(STDIN, $buf, 4) == 4) {
    ($op, $len) = unpack('nn', $buf);
    # Check for data opcode; if so, length is 48 bits.
    if (($op & 0xf000) == 0xf000) {
	die("missing data length") if read(STDIN, $buf, 4) != 4;
	$len_ext = unpack('N', $buf);
	$len = ($len << 32) + $len_ext;	# may overflow.
    }
    printf("op=%s (%x), len=%d\n", $codes{$op}, $op, $len);
    if ($op == 0xf001) {
	&VnodeData($len);
    } elsif (!$codes{$op} || ($len > 1000)) {
	printf("   BAD OPCODE\n");
    } else {
	die("missing data") if read(STDIN, $buf, $len) != $len;
	eval("&$codes{$op}(\$buf, \$len)");
    }
}

sub VnodeData {
    local($len) = @_;
    local($buf);

    die("missing data") if read(STDIN, $buf, 8) != 8;
    $len -= 8;

    local($off0, $off1) = unpack('NN', $buf);
    print("    offset=$off0,,$off1\n");
    if ($last_vnode_type == 5) {
	# Symbolic link.
	die("missing data") if read(STDIN, $buf, $len) != $len;
	local($symlink) = unpack('x8a*', $buf);
	print("symbolic link -> $symlink\n");
    } else {
	# discard data
	local ($l);
	while ($len > 0) {
	    $l = ($len > 8192) ? 8192 : $len;	    
	    die("missing data") if read(STDIN, $buf, $l) != $l;
	    $len -= $l;
	}
    }
}

sub VOLS_DUMP_HEADER_VNODE {
    local($buf, $len) = @_;
    local($vnode, $unique) = unpack('NN', $buf);
    print("    vnode: $vnode, unique: $unique\n");
}

sub VOLS_DUMP_VNODE_DIR_ENTRY {
    local($buf, $len) = @_;
    local($off1, $off2, $vnode, $codeset, $name) = unpack('NNNNa*', $buf);
    if (($off1 | $off2)) {
	print("dir entry offset: $off1,$off2.\n");
    }
    if ($codeset) {
	print("dir entry non null codeset: $codeset\n");
    }
    print("    name=$name, vnode=$vnode\n");
}

sub VOLS_DUMP_VNODE_SIZE {
    local($buf, $len) = @_;
    local($size0, $size1) = unpack('NN', $buf);
    print("    length=$size0,,$size1\n");
}

sub VOLS_DUMP_VNODE_TYPE {
    local($buf, $len) = @_;
    local($type) = unpack('N', $buf);
    $last_vnode_type = $type;
    print("    type=$type\n");
}

sub VOLS_DUMP_VNODE_ACL_INLINE {
    &dump(@_);
}

sub VOLS_DUMP_VNODE_ACL_COPY {
    &dump(@_);
}

sub VOLS_DUMP_VNODE_UNIXMODE {
    local($buf, $len) = @_;
    local($mode) = unpack('N', $buf);
    printf("  mode = %#o\n", $mode);
}

sub VOLS_DUMP_HEADER_N_UNIQUEGEN {
    local($buf, $len) = @_;
    local($unique) = unpack('N', $buf);
    print("	unique=$unique\n");
}

sub VOLS_DUMP_HEADER_VOLVERSION {
    local($buf, $len) = @_;
    local($vsn0, $vsn1) = unpack('NN', $buf);
    print("	vol version=$vsn0,,$vsn1\n");
}    

sub VOLS_DUMP_HEADER_DUMP_DATE {
    &ShowDate(@_);			# pass args through
}

sub VOLS_DUMP_HEADER_STYLE {
    local($buf, $len) = @_;
    local($type, $sec, $usec, $v1, $v2, @spares) = unpack('N11', $buf);
    local ($style);

    if ($type == $FTU_DUMP_TYPE_FULL) {
	$style = "full";
    } elsif (($type == $FTU_DUMP_TYPE_DATE) ||
	     ($type == $FTU_DUMP_TYPE_DATE_WITH_DIRS)) {
	local ($date) = &Date($sec, $usec);
	$style = (($type == $FTU_DUMP_TYPE_DATE) ? "date" : "date with dirs");
	$style = "$style from $date";
    } else {
	$style = (($type == $FTU_DUMP_TYPE_VERS) ? "VV" : "VV with dirs");
	$style = "$style from VV=$v1,,$v2";
    }
    print("     style=$type, $style\n");
}

sub VOLS_DUMP_HEADER_CREATION_DATE {
    &ShowDate(@_);
}

sub VOLS_DUMP_HEADER_ACCESS_DATE {
    &ShowDate(@_);
}

sub VOLS_DUMP_HEADER_UPDATE_DATE {
    &ShowDate(@_);
}

sub Date
{
    local($sec, $usec) = @_;
    local($date) = &ctime($sec);
    chop($date);
    $usec += 1000000;
    $usec = substr($usec, -6);
    "$sec.$usec ($date)";
}

sub ShowDate
{
    local($buf, $len) = @_;
    local($date) = &Date(unpack('NN', $buf));
    print("	time=$date\n");
}    

sub VOLS_DUMP_HEADER_NOVNODE {
    local($buf, $len) = @_;
    local($vnode, $unique) = unpack('NN', $buf);
    print("    vnode: $vnode, unique: $unique\n");
}

sub VOLS_DUMP_HEADER_ENDVNODES {
    local($buf, $len) = @_;
    local($vnode, $unique) = unpack('NN', $buf);
    print("    vnode: $vnode, unique: $unique\n");
}

sub VOLS_DUMP_HEADER_INDEX_MAX {
    local($buf, $len) = @_; 
    local($max) = unpack('N', $buf);
    print("    max vnode index: $max\n");
}

sub VOLS_DUMP_VNODE_VOLVERSION {
    local($buf, $len) = @_;
    local($vsn0, $vsn1) = unpack('NN', $buf);
    print("	vol version=$vsn0,,$vsn1\n");
}

sub VOLS_DUMP_VNODE_DATAVERSION {
    local($buf, $len) = @_;
    local($vsn0, $vsn1) = unpack('NN', $buf);
    print("	data version=$vsn0,,$vsn1\n");
}

sub VOLS_DUMP_VNODE_LINKCOUNT {
    local($buf, $len) = @_;
    local($linkcount) = unpack('N', $buf);
    print("	link count=$linkcount\n");
}

sub VOLS_DUMP_VNODE_GROUP {
    local($buf, $len) = @_;
    local($group) = unpack('N', $buf);
    print("	group=$group\n");
}

sub VOLS_DUMP_VNODE_OWNER {
    local($buf, $len) = @_;
    local($owner) = unpack('N', $buf);
    print("	owner=$owner\n");
}

sub VOLS_DUMP_VNODE_ACCESSTIME {
    &ShowDate(@_);
}

sub VOLS_DUMP_VNODE_MODTIME {
    &ShowDate(@_);
}

sub VOLS_DUMP_VNODE_CHANGETIME {
    &ShowDate(@_);
}

sub VOLS_DUMP_VNODE_SERVERMODTIME {
    &ShowDate(@_);
}

sub VOLS_DUMP_VNODE_ACCESSTIME {
    &ShowDate(@_);
}

sub VOLS_DUMP_HEADER_VISQUOTALIMIT {
    local($buf, $len) = @_;
    local($lim0, $lim1) = unpack('NN', $buf);
    print("     visible quota = $lim0,,$lim1\n");
}

sub VOLS_DUMP_HEADER_ALLOCLIMIT {
    local($buf, $len) = @_;
    local($lim0, $lim1) = unpack('NN', $buf);
    print("     allocation quota = $lim0,,$lim1\n");
}

sub dump {
    local($buf, $len) = @_;
    $buf .= "\0\0\0";
    local($offset);
    while ($offset < $len) {
	$buf1 = substr($buf, $offset, $NNWORDS * 4);
	@data = unpack("x$offset L$NNWORDS", $buf);
	printf("%8d %8X  ", $offset, $offset);
	for $word(@data) {
		printf("%08X ", $word);
	}
	print(" " x (9 * $rem)) if $rem = ($NNWORDS - @data);
	$buf1 =~ s/[^ \w\~\!\@\#\$\%\^\&\*\(\)\_\+\`\{\}\[\]\\\|\;\'\:\"\,\.\/\<\>\?]/-/g;
	print("|$buf1|\n");
	$offset += $NWORDS * 4;
    }
}
