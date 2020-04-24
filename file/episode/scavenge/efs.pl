# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: efs.pl,v $
# Revision 1.1.69.1  1996/10/02  17:28:20  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:34:24  damon]
#
# $EndLog$

# Copyright (C) 1995, 1991 Transarc Corporation - All rights reserved

package efs;

require 'util';

#
# <sys/stat.h> mode bit definitions:
#
$STAT_IFMT	= 0170000;
$STAT_IFDIR	= 0040000;
$STAT_IFCHR	= 0020000;
$STAT_IFBLK	= 0060000;
$STAT_IFREG	= 0100000;
$STAT_IFLNK	= 0120000;
$STAT_IFSOCK	= 0140000;
$STAT_IFIFO	= 0010000;

$STAT_ISUID	= 0004000;
$STAT_ISGID	= 0002000;
$STAT_ISVTX	= 0001000;
$STAT_IREAD	= 0000400;
$STAT_IWRITE	= 0000200;
$STAT_IEXEC	= 0000100;

$CFLAGS_AVL	= 1;
$CFLAGS_VOLUME	= 2;
$CFLAGS_BITMAP	= 3;
$CFLAGS_LOG	= 4;
$CFLAGS_BADBLOCKS = 5;
$CFLAGS_REGULAR = 0xf;

$OLD_MINOR_FIDINDEX = 1;

$ANODE_SIZE = 252;
$DIR_PIECE_SIZE = 32;
$FS_IDENT_NAME_SIZE = 112;
$FS_SPAREREC_SIZE = 76;
$FS_STATUS_MSG_SIZE = 128;
$INITIAL_BLOCK_SIZE = 1024;

#
# Anode block header (ABH) stuff:
#
$ABH_AVL_MAGIC = 0x3198a2e0;
$ABH_VOLUME_MAGIC = 0xb7afc1db;
@ABH_FORMAT = (
 'time', 'L', '%u',
 'magic', 'L', '%#x',
 'index', 'L', '%u'
);
$ABH_SIZE = &util'FormatSize(@ABH_FORMAT);
%ABH_MAP = &util'FormatToMap(@ABH_FORMAT);

#
# Aggregate stuff:
#
$AGGR_ANODE_OFFSET = $ANODE_SIZE;
@AGGR_LOCS = (
    1, 7, 46, 204, 836, 3955, 12836, 35592, 84148, 623118, 3682396,
    6017662, 39640819, 73675885, 563160323, 1269535423
);
$AGGR_MAGIC = 0x8946f098;
$AGGR_QUANTUM = 64 * 1024;
@AGGR_HEADER_FORMAT = (
 'magic', 'L', '%#x',
 'flags', 'L', '%#x',
 'blockSize', 'L', '%u',
 'fragmentSize', 'L', '%u',
 'firstBlock', 'L', '%u',
 'nBlocks', 'L', '%u',
 'nLogBlocks', 'L', '%u',
 'reservedIndexes', 'L', '%u',
 'bitmapIndex', 'L', '%u',
 'logIndex', 'L', '%u',
 'badBlocksIndex', 'L', '%u',
 'timeCreated', 'L', '%u',
 'creatingProgram', 'L', '%u',
 'diskFormatMinorVersion', 'L', '%u',
 'lastVerifyTime', 'L', '%u',
 'lastVerifyCode', 'L', '%#x'
);
$AGGR_HEADER_SIZE = &util'FormatSize(@AGGR_HEADER_FORMAT);
%AGGR_HEADER_MAP = &util'FormatToMap(@AGGR_HEADER_FORMAT);

#
# Various anode constants and names
#
$ANODE_BLOCKEMPTY = 0xffffffff;
$ANODE_BLOCKBACKING = 0x80000000;
$ANODE_DIRECTBLOCKS	= 8;
$ANODE_INDIRECT_TREES	= 4;

$AFLAGS_STATUSSIZE_MASK	= 0xff;

$AFLAGS_TYPE_SHIFT	= 8;
$AFLAGS_TYPE_MASK	= 0xf << $AFLAGS_TYPE_SHIFT;
$AFLAGS_TYPE_AVL	= $CFLAGS_AVL << $AFLAGS_TYPE_SHIFT;
$AFLAGS_TYPE_VOLUME	= $CFLAGS_VOLUME << $AFLAGS_TYPE_SHIFT;
$AFLAGS_TYPE_BITMAP	= $CFLAGS_BITMAP << $AFLAGS_TYPE_SHIFT;
$AFLAGS_TYPE_LOG	= $CFLAGS_LOG << $AFLAGS_TYPE_SHIFT;
$AFLAGS_TYPE_BADBLOCKS	= $CFLAGS_BADBLOCKS << $AFLAGS_TYPE_SHIFT;
$AFLAGS_TYPE_REGULAR	= $CFLAGS_REGULAR << $AFLAGS_TYPE_SHIFT;

$AFLAGS_BITS_SHIFT	= 12;

$AFLAGS_BITS_MASK	= 0x1f0 << $AFLAGS_BITS_SHIFT;
$AFLAGS_LOGGED		= 0x010 << $AFLAGS_BITS_SHIFT;
$AFLAGS_COPYONWRITE	= 0x020 << $AFLAGS_BITS_SHIFT;
$AFLAGS_PREALLOCATED	= 0x040 << $AFLAGS_BITS_SHIFT;
$AFLAGS_DATA_INLINE	= 0x080 << $AFLAGS_BITS_SHIFT;
$AFLAGS_DATA_FRAGMENTED = 0x100 << $AFLAGS_BITS_SHIFT;
$AFLAGS_DATA_NOTBLOCKED = ($AFLAGS_DATA_INLINE | $AFLAGS_DATA_FRAGMENTED);

$AFLAGS_UNSAFE		= 1 << 21;

$ANODE_MAGIC1 = 0xb4000000;
$ANODE_MAGIC1_MASK = 0xff000000;
$ANODE_MAGIC2 = 0x00002000;
$ANODE_MAGIC2_MASK = 0x0000f000;

#
# Auxiliary anode flags
#
$AUX_TYPE_MASK		= 0xf;
$AUX_IS_ACL		= 0x1;
$AUX_IS_INITIAL_DIR_ACL	= 0x2;
$AUX_IS_INITIAL_FILE_ACL = 0x3;
$AUX_IS_PLIST		= 0x5;
$AUX_IS_USER_PLIST	= 0x6;

@ANODE_FORMAT0 = (
 'flags', 'L', '%#x',
 'volData1', 'L', '%u',
 'volData2', 'L', '%u',
 'index', 'L', '%u',
 'allocated', 'L', '%u',
 'length1', 'L', '%u',
 'length2', 'L', '%u'
);
%ANODE_MAP0 = &util'FormatToMap(@ANODE_FORMAT0);
@ANODE_FORMAT1_COW = (
 'bBlock', 'L', '%u',
 'bOffset', 'L', '%u'
);
@ANODE_FORMAT1_REG = (
 'c.volId1', 'L', '%u',
 'c.volId2', 'L', '%u'
);
@ANODE_FORMAT2 = (
 'copies', 'S', '%u',
 '!spareShort', 'S', '%u',
 '!spare[0]', 'L', '%u',
 'visible', 'L', '%u'
);
@ANODE_FORMAT3_FRAGMENTED = (
 'd.frags.block', 'L', '%u',
 'd.frags.first', 'S', '%u',
 'd.frags.length', 'S', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u',
 '!', 'L', '%u'
);
@ANODE_FORMAT3_BLOCKED = (
 'dBlock[0]', 'L', '%u',
 'dBlock[1]', 'L', '%u',
 'dBlock[2]', 'L', '%u',
 'dBlock[3]', 'L', '%u',
 'dBlock[4]', 'L', '%u',
 'dBlock[5]', 'L', '%u',
 'dBlock[6]', 'L', '%u',
 'dBlock[7]', 'L', '%u',
 'iBlock[0]', 'L', '%u',
 'iBlock[1]', 'L', '%u',
 'iBlock[2]', 'L', '%u',
 'iBlock[3]', 'L', '%u'
);
@ANODE_STATUS_FORMAT_AUX = (
 'linkCount', 'L', '%u',
 'aux_flags', 'l', '%#x',
 '!spare[0]', 'l', '%d',
 '!spare[1]', 'l', '%d'
);
$ANODE_AUX_STATUS_SIZE = &util'FormatSize(@ANODE_STATUS_FORMAT_AUX);
@ANODE_STATUS_FORMAT_FILE = (
 'sflags', 'S', '%#x',
 '!spares', 'S', '%u',
 'linkCount', 'S', '%u',
 'mode', 'S', '%#o',
 'unique', 'L', '%u',
 'volumeVersionNumber1', 'L', '%u',
 'volumeVersionNumber2', 'L', '%u',
 'dataVersionNumber', 'L', '%u',
 'mTime.tv_sec', 'l', '%d',
 'mTime.tv_usec', 'l', '%d',
 'cTime.tv_sec', 'l', '%d',
 'cTime.tv_usec', 'l', '%d',
 'aTime.tv_sec', 'l', '%d',
 'aTime.tv_usec', 'l', '%d',
# Uncomment if full UUIDs are used
# 'oid.longField1', 'L', '%u',
# 'oid.shortField1', 'S', '%u',
# 'oid.shortField2', 'S', '%u',
# 'oid.miscChars1', 'L', '%#x',
# 'oid.miscChars2', 'L', '%#x',
 'oid', 'L', '%u',
# 'gid.longField1', 'L', '%u',
# 'gid.shortField1', 'S', '%u',
# 'gid.shortField2', 'S', '%u',
# 'gid.miscChars1', 'L', '%#x',
# 'gid.miscChars2', 'L', '%#x',
 'gid', 'L', '%u',
 'acl', 'L', '%u',
 'plist', 'L', '%u',
 'reservedForUserPlist', 'L', '%u',
 'uuid.longField1', 'L', '%u',
 'uuid.shortField1', 'S', '%u',
 'uuid.shortField2', 'S', '%u',
 'uuid.miscChars[0..3]', 'L', '%#010x',
 'uuid.miscChars[4..8]', 'L', '%#010x',
 'res_timeUncertainty', 'L', '%u',
 '!spare[0]', 'L', '%u',
 '!spare[1]', 'L', '%u',
 '!spare[2]', 'L', '%u',
 'initialDirAcl', 'L', '%u',
 'initialFileAcl', 'L', '%u'
);
$ANODE_FILE_STATUS_SIZE = &util'FormatSize(@ANODE_STATUS_FORMAT_FILE);
@ANODE_STATUS_FORMAT_FS = (
 'ident.id1', 'L', '%u',
 'ident.id2', 'L', '%u',
 'ident.name', "A$FS_IDENT_NAME_SIZE", '"%s"',
);
$ANODE_FS_STATUS_SIZE = &util'FormatSize(@ANODE_STATUS_FORMAT_FS);

#
# Bitmap page (BITMAP_PAGE) stuff:
#
$BITMAP_PAGE_MAGIC = 0x5b7674d7;
@BITMAP_PAGE_FORMAT = (
 'initialStamp', 'l', '%d',
 'magic', 'l', '%#x',
 'baseOffset', 'l', '%d',
 'blocksPerPage', 'L', '%u',
 'bitmapLength', 'L', '%u',
 'fragmentsPerBlock', 'S', '%s',
 '!spare16', 'S', '%s',
 '!spare32', 'l', '%d'
);
%BITMAP_PAGE_MAP = &util'FormatToMap(@BITMAP_PAGE_FORMAT);
$BITMAP_PAGE_SIZE = &util'FormatSize(@BITMAP_PAGE_FORMAT);

#
# Bad block container definitions
#
$BBFrameSize = 1024;
$BBFRAMEMAGIC = 0x76618c6d;

@BB_FRAME_FORMAT = (
 'beginStamp', 'l', '%d',
 'bbFrameMagic', 'l', '%#x'
);
%BB_FRAME_MAP = &util'FormatToMap(@BB_FRAME_FORMAT);
$BB_FRAME_SIZE = &util'FormatSize(@BB_FRAME_FORMAT);

@BB_ENTRY_FORMAT = (
 'type', 'S', '%u',
 'length', 'S', '%u',
 'thisFrag', 'S', '%u',
 'totFrags', 'S', '%u'
);
%BB_ENTRY_MAP = &util'FormatToMap(@BB_ENTRY_FORMAT);
$BB_ENTRY_SIZE = &util'FormatSize(@BB_ENTRY_FORMAT);

#
# Log page definitions
#
$LOG_SALVAGE_MAGIC_NUMBER = 0x73697045;

@LOG_PAGE_HEADER_FORMAT = (
 'passnumber', 'S', '%u',
 'lastLogRecOff', 'S', '%u',
 'pageNumber', 'L', '%u',
 'salvageID', 'L', '%#x',
 'oldestDataPage', 'S', '%u',
 'oldestTranPage', 'S', '%u',
 '!reserved[0]', 'L', '%u',
 '!reserved[1]', 'L', '%u',
 '!reserved[2]', 'L', '%u',
 '!reserved[3]', 'L', '%u',
 'io_window', 'S', '%u',
 'cksum_ver', 'C', '%u',
 'cksum_len', 'C', '%u'
);
%LOG_PAGE_HEADER_MAP = &util'FormatToMap(@LOG_PAGE_HEADER_FORMAT);
%LOG_PAGE_HEADER_SIZE = &util'FormatSize(@LOG_PAGE_HEADER_FORMAT);

#
# Directory entry (DIR_ENTRY) stuff:
#
$DIR_ENTRY_MAGIC = 0x76e694c1;
$DIR_NONENTRY_MAGIC = 0xb937c268;
$DIR_ENTRY_FIRST_ENTRY = 5;

@DIR_ENTRY_HEADER_FORMAT = (
 'dir_entry_magic', 'N', '%#x',
 'fid.index', 'L', '%u',
 'fid.unique', 'L', '%u',
 'next', 'C', '%u',
 'length', 'C', '%u',
 'inuse', 'C', '%u',
);
%DIR_ENTRY_HEADER_MAP = &util'FormatToMap(@DIR_ENTRY_HEADER_FORMAT);
$DIR_ENTRY_HEADER_SIZE = &util'FormatSize(@DIR_ENTRY_HEADER_FORMAT);

#
# Directory header (DIR_HEAD) stuff:
#
$DIR_HEADER_MAGIC = 0x2c70bf7f;
$DIR_PAGE_MAGIC = 0xb63185f6;

@DIR_HEADER_FORMAT = (
 'dir_magic', 'N', '%#x',
 'flist', 'C', '%u',
 'pnhimark', 'C', '%u',
 'pgcount', 'n', '%u',
 '!DP_pad', 'a24', '%s',
 'hashTable[0..3]', 'L', '%#010x',
 'hashTable[4..7]', 'L', '%#010x',
 'hashTable[8..11]', 'L', '%#010x',
 'hashTable[12..15]', 'L', '%#010x',
 'hashTable[16..19]', 'L', '%#010x',
 'hashTable[20..23]', 'L', '%#010x',
 'hashTable[24..27]', 'L', '%#010x',
 'hashTable[28..31]', 'L', '%#010x',
 'hashTable[32..35]', 'L', '%#010x',
 'hashTable[36..39]', 'L', '%#010x',
 'hashTable[40..43]', 'L', '%#010x',
 'hashTable[44..47]', 'L', '%#010x',
 'hashTable[48..51]', 'L', '%#010x',
 'hashTable[52..55]', 'L', '%#010x',
 'hashTable[56..59]', 'L', '%#010x',
 'hashTable[60..63]', 'L', '%#010x',
 'hashTable[64..67]', 'L', '%#010x',
 'hashTable[68..71]', 'L', '%#010x',
 'hashTable[72..75]', 'L', '%#010x',
 'hashTable[76..79]', 'L', '%#010x',
 'hashTable[80..83]', 'L', '%#010x',
 'hashTable[84..87]', 'L', '%#010x',
 'hashTable[88..91]', 'L', '%#010x',
 'hashTable[92..95]', 'L', '%#010x',
 'hashTable[96..99]', 'L', '%#010x',
 'hashTable[100..103]', 'L', '%#010x',
 'hashTable[104..107]', 'L', '%#010x',
 'hashTable[108..111]', 'L', '%#010x',
 'hashTable[112..115]', 'L', '%#010x',
 'hashTable[116..119]', 'L', '%#010x',
 'hashTable[120..123]', 'L', '%#010x',
 'hashTable[124..127]', 'L', '%#010x',
);
$DIR_HASH_SIZE = 128;
$DIR_HEADER_SIZE = &util'FormatSize(@DIR_HEADER_FORMAT);
%DIR_HEADER_MAP = &util'FormatToMap(@DIR_HEADER_FORMAT);

#
# Directory page (DIR_PAGE) stuff:
#
$DIR_PAGE_MAGIC = 0xb63185f6;
$DIR_PAGE_SIZE = 8192;
$DIR_ROOT_INDEX = 2;

#
# File set (FS) stuff:
#
$VOLUME_MAGIC = 0xa197840a;

#
# Fileset header flags definitions:
#
$FS_FLAGS_REPFIELD	= 0xf;
$FS_FLAGS_REP_NONE	= 0x0;
$FS_FLAGS_REP_RELEASE	= 0x1;
$FS_FLAGS_REP_LAZY	= 0x2;

$FS_FLAGS_READONLY	= 0x10;
$FS_FLAGS_INCONSISTENT	= 0x20;
$FS_FLAGS_COMPLETE	= 0x40;

$FS_FLAGS_TYPEFIELD	= 0x780;
$FS_FLAGS_TYPE_RW	= 0x080;
$FS_FLAGS_TYPE_RO	= 0x100;
$FS_FLAGS_TYPE_BK	= 0x180;
$FS_FLAGS_TYPE_TEMP	= Ox200;

$FS_FLAGS_REPSERVER_MGD	= 0x800;

$FS_FLAGS_LEGALFLAGS	= 0xfff;

@FS_HEADER_FORMAT = (
 'magic', 'L', '%#x',
 'ident.id1', 'L', '%u',
 'ident.id2', 'L', '%u',
 'ident.name', "A$FS_IDENT_NAME_SIZE", '"%s"',
 'flags', 'L', '%#x',
 'index', 'L', '%u',
 'version1', 'L', '%u',
 'version2', 'L', '%u',
 'unique', 'L', '%u',
 'createTime', 'L', '%u',
 'modTime', 'L', '%u',
 'accessTime', 'L', '%u',
 'backingVolIndex', 'L', '%u',
 'backingVolId1 (llBackId1)', 'L', '%u',
 'backingVolId2 (llBackId2)', 'L', '%u',
 'upLevelIds[0]1 (llFwdId1)', 'L', '%u',
 'upLevelIds[0]2 (llFwdId2)', 'L', '%u',
 'upLevelIds[1]1 (parentId1)', 'L', '%u',
 'upLevelIds[1]2 (parentId2)', 'L', '%u',
 'upLevelIds[2]1 (backupId1)', 'L', '%u',
 'upLevelIds[2]2 (backupId2)', 'L', '%u',
 'upLevelIds[3]1 (cloneId1)', 'L', '%u',
 'upLevelIds[3]2 (cloneId2)', 'L', '%u',
 '!allocated.spare1', 'L', '%u',
 'allocated.limit', 'L', '%u',
 '!allocated.spare2', 'L', '%u',
 'allocated.usage', 'L', '%u',
 '!sparel[0]', 'l', '%d',
 '!sparel[1]', 'l', '%d',
 '!sparel[2]', 'l', '%d',
 '!sparel[3]', 'l', '%d',
 '!sparel[4]', 'l', '%d',
 '!visible.spare1', 'L', '%u',
 'visible.limit', 'L', '%u',
 '!visible.spare2', 'L', '%u',
 'visible.usage', 'L', '%u',
 'statusMsg', "A$FS_STATUS_MSG_SIZE", '"%s"',
 'fileStatus[0]', 'l', '%d',
 'fileStatus[1]', 'l', '%d',
 'fileStatus[2]', 'l', '%d',
 'fileStatus[3]', 'l', '%d',
 'fileStatus[4]', 'l', '%d',
 'fileStatus[5]', 'l', '%d',
 'fileStatus[6]', 'l', '%d',
 'fileStatus[7]', 'l', '%d',
 'fileStatus[8]', 'l', '%d',
 'fileStatus[9]', 'l', '%d',
 'fileStatus[10]', 'l', '%d',
 'fileStatus[11]', 'l', '%d',
 '!sparerec', "A$FS_SPAREREC_SIZE", ''
);

$FS_HEADER_SIZE = &util'FormatSize(@FS_HEADER_FORMAT);
%FS_HEADER_MAP = &util'FormatToMap(@FS_HEADER_FORMAT);

# Indirect Block (INDIRECT) stuff:
$INDIRECT_MAGIC = 0x5a308d31;
@INDIRECT_HEADER_FORMAT = (
 'initialStamp', 'l', '%d',
 'magic', 'l', '%#x',
 'whoPoints.block', 'l', '%ld',
 'whoPoints.offset', 'L', '%u',
 'baseOffset', 'L', '%lu',
 'span', 'L', '%u',
 'ibsize', 'L', '%u'
);

$INDIRECT_HEADER_SIZE = &util'FormatSize(@INDIRECT_HEADER_FORMAT);
%INDIRECT_HEADER_MAP = &util'FormatToMap(@INDIRECT_HEADER_FORMAT);

sub AggrGet {
    local(*DEVICE) = @_;
    local($i);

    foreach $i ($[ .. $#AGGR_LOCS) {
	local($offset) = $AGGR_LOCS[$i] * $AGGR_QUANTUM;
	local($block) = $offset / $INITIAL_BLOCK_SIZE;
	local($abh) = &BlockRead(*DEVICE, $block, $INITIAL_BLOCK_SIZE);

	local($aggr) = substr($abh, $ABH_SIZE, $ANODE_SIZE);
	local($aggr_anode) =
	    substr($abh, $ABH_SIZE + $AGGR_ANODE_OFFSET, $ANODE_SIZE);

	local($abh_magic) = &GetAnodeBlockMember($abh, 'magic');
	local($aggr_magic) = &GetAggrMember($aggr, 'magic');

	if ($abh_magic != $ABH_AVL_MAGIC || $aggr_magic != $AGGR_MAGIC) {
	    warn("Bad aggregate header at block $block; trying next one");
	    next;
	}
	return ($aggr, $aggr_anode, $offset + $ABH_SIZE);
    }

    return ();			# We didn't find one;
}

#
# Cache some information for efficiency
#
sub InitializeCachedInfo {
    local(*AGGR) = @_;
    ($SuperBlock, $AVL_anode, $SBOffset) = &AggrGet(*AGGR);
    $AVL_index = &GetSuperMember('reservedIndexes');
    $AVL_offset = $SBOffset + $AGGR_ANODE_OFFSET;
    $blockSize = &GetSuperMember('blockSize');
    $fragSize = &GetSuperMember('fragmentSize');
    $frags_per_block = $blockSize / $fragSize unless ($fragSize == 0);
    $anodes_per_block = &AnodesPerBlock($blockSize);
    $ibp_per_block = &IndirectsPerBlock($blockSize);
}

#
# Extract a value from the superblock.
#
sub GetSuperMember {
   local($member) = @_;
   return &GetAggrMember($SuperBlock, $member);
}

sub AggrGetAnode {
    return $AVL_anode;
}

sub AggrPrint {
    &util'FormatPrint(@_, @AGGR_HEADER_FORMAT);
}

sub AnodeFlagsToType {
    local($flags) = @_;
    return +($flags & $AFLAGS_TYPE_MASK) >> $AFLAGS_TYPE_SHIFT;
}

sub AnodeGet {
    local(*DEVICE, $index, $main_anode) = @_;
    local($block) = int($index / $anodes_per_block);
    local($offset_in_block) =
	$index % $anodes_per_block * $ANODE_SIZE + $ABH_SIZE;
    local($anode, $offset, $length) =
	&AnodeRead(*DEVICE,
		   $block * $blockSize + $offset_in_block, $ANODE_SIZE,
		   $main_anode);

    if ($length != $ANODE_SIZE) {
	warn("AnodeRead failed");
	return ();
    }

    return ($anode, $offset);
}

sub AnodeGetBlockMap {
    local(*AGGR, $anode) = @_;
    local(%anode) = &util'UnpackAssoc($anode, &AnodeGetFormat($anode));
    local($block, $first_index, $i);
    local(@map);

    $first_index = $ANODE_DIRECTBLOCKS;
    for ($i = 1; $i < $ANODE_INDIRECT_TREES; $i++) {
	$first_index += $ibp_per_block ** $i;
    }

    for ($i = $ANODE_INDIRECT_TREES - 1; $i >= 0; $i--) {
	$block = $anode{"iBlock[$i]"};
	&AnodeGetBlockMap2(*AGGR, $block, $i, $first_index, *map);
	$first_index -= $ibp_per_block ** $i;
    }

    for ($i = $ANODE_DIRECTBLOCKS - 1; $i >= 0; $i--) {
	$block = $anode{"dBlock[$i]"};
	$map[$i] = $block unless ($block == $ANODE_BLOCKEMPTY);
    }

    return @map;
}

sub AnodeGetBlockMap2 {
    local(*AGGR, $block, $level, $first_index, *map) = @_;
    local(@block);
    local($i);
    local($indirect_block);

    return () if ($block == $ANODE_BLOCKEMPTY);

    ($indirect_block) = &IndirectGet(*AGGR, $block);
    @block = &ExtractBlockAddr($indirect_block);

    for ($i = $ibp_per_block - 1; $i >= 0; $i--) {
	if ($level == 0) {
	    if ($block[$i] != $ANODE_BLOCKEMPTY) {
		local($block_num) = $first_index + $i;
		$map[$block_num] = $block[$i];
	    }
	} else {
	    &AnodeGetBlockMap2(*AGGR, $block[$i], $level - 1,
				$first_index + $ibp_per_block ** $level * $i,
				*map);
	}
    }
}

#
# Get the anode format; this depends on the anode's type, storage method,
# and COW status.  We call this a lot, so it is worth doing a little
# extra work to make it go fast.  $fixed_anode_size is the size of the
# part of the anode preceding the disk addresses (or in-line data), and
# is invariable.  @base_array_size gives the anode size including the
# disk address area; for inline anodes, we just use $fixed_anode_size
# and add in the file length later.
#
# From the various parameters that determine the anode format we construct
# a unique signature and use this to cache previously computed formats.
# (Caching is perhaps a misnomer here, since we are really just saving
# the first N distinct formats, but that is fine for practical purposes.)
#
$fixed_anode_size =
    &util'FormatSize(@ANODE_FORMAT0) +
    &util'FormatSize(@ANODE_FORMAT1_REG) +
    &util'FormatSize(@ANODE_FORMAT2);
@base_anode_size = (
    $fixed_anode_size + &util'FormatSize(@ANODE_FORMAT3_BLOCKED),
    $fixed_anode_size,
    $fixed_anode_size + &util'FormatSize(@ANODE_FORMAT3_FRAGMENTED)
);

$max_anode_format_cache_size = 100;

sub AnodeGetFormat {
    local($anode) = @_;
    local($flags) = &GetAnodeMember($anode, 'flags');
    local($cow) = (($flags & $AFLAGS_COPYONWRITE) != 0);
    local($data) = ($flags & $AFLAGS_DATA_NOTBLOCKED);
    local($dindex) = $data >> ($AFLAGS_BITS_SHIFT + 7);
    local($dlen) = 0;
    $dlen = &GetAnodeMember($anode, 'length2')
	if ($data == $AFLAGS_DATA_INLINE);

    local($status_size) = $flags & $AFLAGS_STATUSSIZE_MASK;
    local($filler_size) =
	$ANODE_SIZE - $base_anode_size[$dindex] - $dlen - $status_size;
    local($type) = &AnodeFlagsToType($flags);

    local($signature) = "t${type}s${status_size}f${filler_size}d$dindex$cow";
    local($cached_value) = $anode_format_cache{$signature};

    return split(';', $cached_value) if $cached_value;

    local(@format) = @ANODE_FORMAT0;
    push(@format, $cow ? @ANODE_FORMAT1_COW : @ANODE_FORMAT1_REG);
    push(@format, @ANODE_FORMAT2);

    if ($data == $AFLAGS_DATA_INLINE) {
	push(@format, ('inlineData', "A$dlen", '"%s"'));
    } elsif ($data == $AFLAGS_DATA_FRAGMENTED) {
	push(@format, @ANODE_FORMAT3_FRAGMENTED);
    } else {
	push(@format, @ANODE_FORMAT3_BLOCKED);
    }
    push(@format, ('!', "A$filler_size", '"%s"'));

    if ($type == $CFLAGS_VOLUME) {
	if ($status_size != $ANODE_FS_STATUS_SIZE) {
	    warn("Status size ($status_size) does not equal FS format size " .
		 "($ANODE_FS_STATUS_SIZE)");
	} else {
	    push(@format, @ANODE_STATUS_FORMAT_FS);
	}
    } elsif ($type == $CFLAGS_REGULAR) {
	if ($status_size == $ANODE_FILE_STATUS_SIZE) {
	    push(@format, @ANODE_STATUS_FORMAT_FILE);
	} elsif ($status_size == $ANODE_AUX_STATUS_SIZE) {
	    push(@format, @ANODE_STATUS_FORMAT_AUX);
	} else {
	    warn("Status size ($status_size) looks bogus");
	}
    }

    if ($#anode_format_cache < $max_anode_format_cache_size) {
	$anode_format_cache{$signature} = join(';', @format);
    }
    return @format;
}

$_last_anode = 0;

sub AnodeGetMap {
    local($anode) = @_;

    return %_last_anode_map if ($anode eq $_last_anode);
    $_last_anode = $anode;
    %_last_anode_map = &util'FormatToMap(&AnodeGetFormat($anode));
    return (%_last_anode_map);
}

sub AnodeIndexToBlockIndex {
    local($index) = @_;

    return int($index / $anodes_per_block);
}

sub AnodeGetIndirectBlockMap {
    local(*AGGR, $anode) = @_;
    local(%anode) = &util'UnpackAssoc($anode, &AnodeGetFormat($anode));
    local($block_name, $first_index, $i);
    local(@map);

    $first_index = 1;
    for ($i = 1; $i < $ANODE_INDIRECT_TREES - 1; $i++) {
	$first_index += $ibp_per_block ** $i;
    }

    for ($i = $ANODE_INDIRECT_TREES - 1; $i >= 1; $i--) {
	$block_name = "iBlock[$i]";
	&AnodeGetIndirectBlockMap2(*AGGR, $anode{$block_name}, $i,
				   $first_index, *map);
	$first_index -= $ibp_per_block ** ($i - 1);
    }

    $map[0] = $anode{'iBlock[0]'}
	unless ($anode{'iBlock[0]'} == $ANODE_BLOCKEMPTY);

    return @map;
}

sub AnodeGetIndirectBlockMap2 {
    local(*AGGR, $block, $level, $first_index, *map) = @_;
    local(@block);
    local($i);
    local($indirect_block);

    return if ($block == $ANODE_BLOCKEMPTY);

    ($indirect_block) = &IndirectGet(*AGGR, $block);
    @block = &ExtractBlockAddr($indirect_block);

    for ($i = $ibp_per_block - 1; $i >= 0; $i--) {
	if ($level == 1) {
	    if ($block[$i] != $ANODE_BLOCKEMPTY) {
		local($block_num) = $first_index + $i;
		$map[$block_num] = $block[$i];
	    }
	} else {
	    &AnodeGetIndirectBlockMap2(*AGGR, $block, $level - 1,
			$first_indexi + $ibp_per_block ** $level* $i, *map);
	}
    }
}

sub AnodeIsValid {
    local($anode) = @_;
    local($flags) = &GetAnodeMember($anode, 'flags');

    return ($flags & $ANODE_MAGIC1_MASK) == $ANODE_MAGIC1 &&
	   ($flags & $ANODE_MAGIC2_MASK) == $ANODE_MAGIC2;
}

sub AnodePrint {
    local($anode) = @_;
    local(@format) = &AnodeGetFormat($anode);

    &util'FormatPrint($anode, @format);
}

sub AnodeRead {
    return &AnodeRW(undef, @_);
}

sub AnodeRW {
    local($write_buf, *DEVICE, $offset, $length, $anode) = @_;
    local($block, $buf, $device_offset);
    local($flags) = &GetAnodeMember($anode, 'flags');
    local($length2) = &GetAnodeMember($anode, 'length2');
    local($offset_in_block);

    return () unless ($offset < $length2);

    $length = $length2 - $offset if ($length > $length2 - $offset);

    if ($flags & $AFLAGS_DATA_INLINE) {
	#
	# XXX This requires an interface change, viz. supplying the
	# anode offset.
	#
	warn('Can not read or write inline data (yet)');
	return ();
    } elsif ($flags & $AFLAGS_DATA_FRAGMENTED) {
	local($first_frag) = &GetAnodeMember($anode, 'd.frags.first');

	$block = &GetAnodeMember($anode, 'd.frags.block');
	return () unless &IsValidBlockAddr($block, 1);
	$block &= ~$ANODE_BLOCKBACKING;	# Turn off high bit
	$offset_in_block = $first_frag * $fragSize + $offset;

	if ($write_buf) {
	    &Write(*DEVICE, $write_buf, $block + $offset_in_block, $num_bytes);
	} else {
	    $buf = substr(&BlockRead(*DEVICE, $block, $blockSize),
			  $offset_in_block, $length);
	    $device_offset = $block * $blockSize + $offset_in_block;
	}
    } else {
	local($block_index) = int($offset / $blockSize);
	local(@block_map) = &AnodeGetBlockMap(*DEVICE, $anode);
	local($num_bytes);
	local($resid) = $length;

	$block = $block_map[$block_index++];
	return () unless &IsValidBlockAddr($block, 1);
	$block &= ~$ANODE_BLOCKBACKING;	# Turn off high bit
	$offset_in_block = $offset % $blockSize;

	if ($write_buf) {
	    $num_bytes = &util'Min($blockSize - $offset_in_block, $resid);
	    &Write(*DEVICE, $write_buf, $block + $offset_in_block, $num_bytes);
	    $write_buf = substr($buf, $num_bytes);
	    $resid -= $num_bytes;
	} else {
	    $buf = substr(&BlockRead(*DEVICE, $block, $blockSize),
			  $offset_in_block);
	    # If there are multiple reads, there will be more than one
	    # device offset, but most episode objects don't cross
	    # block boundaries.  The exception to this is when the
	    # block size is less than the directory page size (8192).
	    # For now we just return the offset of the first chunk.
	    $device_offset = $block * $blockSize + $offset_in_block;
	    $resid -= $blockSize - $offset_in_block;
	}

	while ($resid > 0) {
	    $block = $block_map[$block_index++];
	    last unless &IsValidBlockAddr($block, 1);
	    $block &= ~$ANODE_BLOCKBACKING;	# Turn off high bit

	    if ($write_buf) {
		$num_bytes = &util'Min($blockSize, $resid);
		&Write(*DEVICE, $write_buf, $block, $num_bytes);
		$write_buf = substr($write_buf, $num_bytes);
		$resid -= $num_bytes;
	    } else {
		$buf .= &BlockRead(*DEVICE, $block, $blockSize);
		$resid -= $blockSize;
	    }
	}
	$length -= $resid if ($resid > 0);
	$buf = substr($buf, 0, $length) unless $write_buf;
    }

    if ($write_buf) {
	return $length;
    } else {
	return ($buf, $device_offset, $length);
    }
}

sub AnodeWrite {
    return &AnodeRW(@_);
}

sub AnodesPerBlock {
    local($block_size) = @_;
    return int(($block_size - $ABH_SIZE - 4) / $ANODE_SIZE);
}

$_last_block = 0;
$_last_block_size = 0;
$_last_buf = 0;

#
# Update cached block if we write to it.
#
sub UpdateReadBuffer {
    local($buf, $offset, $length) = @_;
    local($block_start) = $_last_block * $_last_block_size;
    local($block_end) = $block_start + $_last_block_size;

    if ($block_start <= $offset + $length && $offset < $block_end) {
	local($left_off) = &util'Max($offset, $block_start);
	local($right_off) = &util'Min($offset + $length, $block_end);
	local($len) = $right_off - $left_off;
	local($lblk) = $left_off - $block_start;
	local($lbuf) = $left_off - $offset;
	substr($_last_buf, $lblk, $len) = substr($buf, $lbuf, $len);
    }
}

#
# Discard cached block when switching to new aggregate.
#
sub InvalidateReadBuffer {
    $_last_block = $last_block_size = 0;
}

sub BlockRead {
    local(*DEVICE, $block, $block_size) = @_;

    if (!($block == $_last_block && $block_size == $_last_block_size &&
	  $_last_buf)) {
	local($offset) = $block * $block_size;

	seek(DEVICE, $offset, 0) || die("seek failed: $!");
	sysread(DEVICE, $_last_buf, $block_size) || die("read failed: $!");
	$_last_block = $block;
	$_last_block_size = $block_size;
    }
    return $_last_buf;
}

sub AnodeBlockGet {
    local(*DEVICE, $page_number, $vol_anode) = @_;
    local($ablock, $ablock_magic);
    local($length, $offset);

    ($ablock, $offset, $length) =
	&AnodeRead(*DEVICE, $page_number * $blockSize, $blockSize, $vol_anode);
    if ($length < $blockSize) {
	warn("AnodeRead failed");
	return ();
    }

    $ablock_magic = &GetAnodeBlockMember($ablock, 'magic');
    if ($ablock_magic != $ABH_AVL_MAGIC && $ablock_magic != $ABH_VOLUME_MAGIC) {
	warn("Bad anode block at offset $offset");
	return ();
    }

    return ($ablock, $offset);
}

sub ExtractAnodeFromBlock {
    local($ablock, $index) = @_;
    if ($index >= $anodes_per_block) {
	warn("bad index $index passed to ExtractAnodeFromBlock");
	return ();
    }
    local($ioff) = $ABH_SIZE + $index * $ANODE_SIZE;
    return (substr($ablock, $ioff, $ANODE_SIZE), $ioff);
}

sub BitmapPageGet {
    local(*DEVICE, $page_number, $bitmap_anode) = @_;
    local($bp, $bp_magic);
    local($length, $offset);

    ($bp, $offset, $length) =
	&AnodeRead(*DEVICE, $page_number * $blockSize, $blockSize,
		   $bitmap_anode);
    if ($length < $blockSize) {
	warn("AnodeRead failed");
	return ();
    }

    $bp_magic = &GetBitmapPageMember($bp, 'magic');
    if ($bp_magic != $BITMAP_PAGE_MAGIC) {
	warn("Bad bitmap page at offset $offset");
	return ();
    }

    return ($bp, $offset);
}

sub ZeroBits
{
    @zeroBits = ();			# index byte to get num zero bits
    for ($i=0; $i<256; $i++) {
	$zeroBits[$i] = 0;
	for ($j=1; $j<256; $j<<=1) {
	    $zeroBits[$i]++ if (!($i & $j));
	}
    }
}

sub BitmapPagePrint
{
    local($bp) = @_;
    local(%bp) = &util'UnpackAssoc($bp, @BITMAP_PAGE_FORMAT);
    local(@block_map, @fragment_map);
    local($fragment_map_offset);
    local($i);

    local($fragsPerBlock) = $bp{'fragmentsPerBlock'};
    local($nbytes) = $bp{'blocksPerPage'} / 8;

    $fragment_map_offset = $BITMAP_PAGE_SIZE;
    if ($fragsPerBlock > 1) {
        @block_map = unpack("x$BITMAP_PAGE_SIZE, C$nbytes", $bp);
	$fragment_map_offset += $nbytes;
	$nbytes *= $fragsPerBlock;
    }
    @fragment_map = unpack("x$fragment_map_offset, C$nbytes", $bp);

    &util'FormatPrint($bp, @BITMAP_PAGE_FORMAT);

    if ($fragsPerBlock > 1) {
	print "\tBlock Map:\n";
	for ($i = 0; $i <= $#block_map; $i++) {
	    if ($i % 16 == 0) {
		printf("\t\t%5u: ", $i * 8 + $bp{'baseOffset'});
	    }
	    printf("%02x ", $block_map[$i]);
	    print "\n" if (($i + 1) % 16 == 0);
	}
	print "\n" if ($i % 16 != 0);
    }

    local($freeFrags, $freeBlocks);
    if ($fragsPerBlock == 8) {
	local ($byte);
	&ZeroBits;
	$freeFrags = $freeBlocks = 0;
	for ($i = 0; $i < @fragment_map; $i++) {
	    $byte = $fragment_map[$i];
	    next if ($byte == 0xff);
	    if ($byte == 0) {
		$freeBlocks++;
	    } else {
		$freeFrags += $zeroBits[$byte];
	    }
	}
    } else {
	$freeFrags = $freeBlocks = "?";
    }
    print "\tFragment Map with $freeBlocks blocks and $freeFrags fragments free:\n";
    for ($i = 0; $i <= $#fragment_map; $i++) {
	if ($i % 16 == 0) {
	    printf("\t\t%5u: ",
		   $i * 8 / $bp{'fragmentsPerBlock'} + $bp{'baseOffset'});
	}
	printf("%02x ", $fragment_map[$i]);
	print "\n" if (($i + 1) % 16 == 0);
    }
    print "\n" if ($i % 16 != 0);
}

#
# Search bitmap page for a free block and return its block number;
# mark block free if $mark is true.
#
sub FreeBlockGet {
    local(*AGGR, $bp, $offset, $mark) = @_;
    local(%bp) = &util'UnpackAssoc($bp, @BITMAP_PAGE_FORMAT);
    local($fpb) = $bp{'fragmentsPerBlock'};
    local($nbytes) = $bp{'blocksPerPage'} / 8;

    local(@fragment_map);
    local($fragment_map_offset);

    $fragment_map_offset = $BITMAP_PAGE_SIZE + $nbytes;
    $nbytes *= $fpb;
    @fragment_map = unpack("x$fragment_map_offset, C$nbytes", $bp);

    local($bytes_per_block) = ($fpb <= 8) ? 1 : ($fpb / 8);
    local($blocks_per_byte) = ($fpb >= 8) ? 1 : (8 / $fpb);
    local($bincr) = ($fpb <= 8) ? $blocks_per_byte : $bytes_per_block;
    local($i, $j, $mask);
outer:
    for ($i = 0; $i <= $#fragment_map; $i += $bincr) {
	$mask = (1 << $fpb) - 1;
	for ($j = 0; $j != $bincr; $j++) {
	    if ($bytes_per_block >= 1) {
		next outer if ($fragment_map[$i + $j] != 0);
	    } else {
		last if (($fragment_map[$i] & $mask) == 0);
		$mask <<= $fpb;
	    }
	}
	local($block_number) = $bp{'baseOffset'};
	if ($bytes_per_block >= 1) {
	    for ($j = 0; $j != $bincr; $j++) {
		$fragment_map[$i + $j] = 0xff;
	    }
	    $block_number += ($i / $bincr);
	}  else {
	    $fragment_map[$i] |= $mask;
	    $block_number += ($i * $bincr) + $j;
	}
	if ($mark) {
	    substr($bp, $fragment_map_offset, $nbytes) =
		pack("C$nbytes", @fragment_map);
	    &Write(*AGGR, $bp, $offset, $blockSize);
	}
	return $block_number;
    }
    return 0;
}

#
# Check whether specified block number is in this bitmap page and free.
#
sub IsBlockFree {
    local($bp, $block_number) = @_;
    local(%bp) = &util'UnpackAssoc($bp, @BITMAP_PAGE_FORMAT);
    local($fpb) = $bp{'fragmentsPerBlock'};
    local($nbytes) = $bp{'blocksPerPage'} / 8;
    local($relno) = $block_number - $bp{'baseOffset'};

    local(@fragment_map);
    local($fragment_map_offset);

    $fragment_map_offset = $BITMAP_PAGE_SIZE + $nbytes;
    $nbytes *= $fpb;
    @fragment_map = unpack("x$fragment_map_offset, C$nbytes", $bp);

    if ($fpb >= 8) {
	local($bytes_per_block) = $fpb / 8;
	local($bindex) = $relno * $bytes_per_block;
	for ($i = 0; $i != $bytes_per_block; $i++) {
	    return 0 if ($fragment_map[$bindex + $i] != 0);
	}
	return 1;
    } else {
	local($blocks_per_byte) = 8 / $fpb;
	local($bitno) = $relno % $blocks_per_byte;
	local($bindex) = ($relno - $bitno)/ $blocks_per_byte;
	local($mask) = ((1 << $fpb) - 1) << $bitno;
	return (($fragment_map[$bindex] & $mask) == 0);
    }
}

sub BadblockFrameGet {
    local(*DEVICE, $frame_number, $bb_anode) = @_;
    local($bb, $bb_magic);
    local($length, $offset);

    ($bb, $offset, $length) =
	&AnodeRead(*DEVICE, $frame_number * $BBFrameSize, $BBFrameSize,
		   $bb_anode);
    if ($length < $BBFrameSize) {
	warn("AnodeRead failed");
	return ();
    }

    $bb_magic = &GetBadblockFrameMember($bb, 'bbFrameMagic');
    if ($bb_magic != $BBFRAMEMAGIC) {
	warn("Bad badblock frame at offset $offset");
	return ();
    }

    return ($bb, $offset);
}

sub BadblockEntryGet {
    local($entry_offset, $bb, $bb_offset) = @_;

    return () if ($entry_offset >= $BBFrameSize);

    local($entry) = substr($bb, $entry_offset, $BB_ENTRY_SIZE);
    local($length) = &GetBadblockEntryMember($entry, 'length');
    $entry .= substr($bb, $entry_offset + $BB_ENTRY_SIZE, $length);
    $_[0] += $BB_ENTRY_SIZE + $length;
    return ($entry, $bb_offset + $entry_offset);
}

sub LogPageGet {
    local(*DEVICE, $page_number, $log_anode) = @_;
    local($logp, $length, $offset) =
	&AnodeRead(*DEVICE, $page_number * $blockSize, $blockSize,
		   $log_anode);
    if ($length < $blockSize) {
	warn("AnodeRead failed");
	return ();
    } else {
	return ($logp, $offset);
    }
}

sub DirEntryGet {
    local($entry, $dh, $dh_offset) = @_;
    local($de);
    local($length);
    local($offset) = $entry * $DIR_PIECE_SIZE;

    return () if ($offset >= $DIR_PAGE_SIZE);

    $de = substr($dh, $offset, $DIR_ENTRY_HEADER_SIZE);
    $length = &GetDirEntryMember($de, 'length');
    if ($length == 0) { $length = 1; }
    $de = substr($dh, $offset, $length * $DIR_PIECE_SIZE);
    $_[0] = $entry + $length;
    return ($de, $dh_offset + $offset);
}

sub DirEntryGetFormat {
    local($de, $shrink_name) = @_;
    local(@format);
    local($length) = &GetDirEntryMember($de, 'length');
    local($max_name_length) =
	$length * $DIR_PIECE_SIZE - $DIR_ENTRY_HEADER_SIZE;
    local($name, $name_length);

    @format = (@DIR_ENTRY_HEADER_FORMAT, 'name', "a$max_name_length", '"%s"');

    if ($shrink_name) {
	$name = &util'MapGet($de, 'name', &util'FormatToMap(@format));
	$name_length = index($name, "\0");
	$format[$#format-1] = "a$name_length";
    }

    return @format;
}

sub DirEntryGetMap {
    local($de) = @_;
    return &util'FormatToMap(&DirEntryGetFormat($de));
}

sub DirEntryPrint {
    local($de) = @_;
    &util'FormatPrint($de, &DirEntryGetFormat($de, 1));
}

sub DirPageGet {
    local(*DEVICE, $page_number, $dir_anode) = @_;
    local($dh, $dh_magic);
    local($length, $offset);

    ($dh, $offset, $length) =
	&AnodeRead(*DEVICE, $page_number * $DIR_PAGE_SIZE, $DIR_PAGE_SIZE,
		   $dir_anode);
    if ($length < $DIR_HEADER_SIZE) {
	warn("AnodeRead failed");
	return ();
    }

    $dh_magic = &GetDirPageMember($dh, 'dir_magic');
    if ($page_number == 0 && $dh_magic != $DIR_HEADER_MAGIC ||
	$page_number != 0 && $dh_magic != $DIR_PAGE_MAGIC) {
	warn("Bad directory page at offset $offset");
	return ();
    }

    return ($dh, $offset);
}

sub DirPagePrint {
    local($dh) = @_;
    &util'FormatPrint($dh, @DIR_HEADER_FORMAT);
}

sub FSFidToIndex {
    local($fid) = @_;
    local($index, $version);

    $version = &GetSuperMember('diskFormatMinorVersion');
    if ($version <= $OLD_MINOR_FIDINDEX) {
	$index = $fid + 1;
    } else {
	local($abp) = $anodes_per_block;
	$index = int(($fid + 1) / $abp) * 2 * $abp + ($fid + 1) % $abp;
    }

    return $index;
}

sub FSIndexToFid {
    local($index) = @_;
    local($fid, $version);

    $version = &GetSuperMember('diskFormatMinorVersion');
    if ($version <= $OLD_MINOR_FIDINDEX) {
	$fid = $index + 1;
    } else {
	local($abp) = $anodes_per_block;
	$fid = int($index / (2 * $abp)) * $abp + $index % $abp - 1;
    }
    return $fid;
}

sub FSGet {
    local(*DEVICE, $fs_anode) = @_;
    local($abh, $abh_magic);
    local($fs, $fs_magic);
    local($length, $offset);

    ($abh, $offset, $length) =
	&AnodeRead(*DEVICE, 0, $ABH_SIZE + $FS_HEADER_SIZE, $fs_anode);
    if ($length < $ABH_SIZE + $FS_HEADER_SIZE) {
	warn("AnodeRead failed");
	return ();
    }

    $fs = substr($abh, $ABH_SIZE, $FS_HEADER_SIZE);
    $offset += $ABH_SIZE;

    $abh_magic = &GetAnodeBlockMember($abh, 'magic');
    $fs_magic = &GetFSMember($fs, 'magic');

    if ($abh_magic != $ABH_VOLUME_MAGIC || $fs_magic != $VOLUME_MAGIC) {
	warn("Bad file set header at block $block");
	return ();
    }

    return ($fs, $offset);
}

sub FSPrint {
    local($fs) = @_;
    &util'FormatPrint($fs, @FS_HEADER_FORMAT);
}

sub IndirectGet {
    local(*AGGR, $block) = @_;
    $block &= ~ANODE_BLOCKBACKING;
    local($offset) = $block * $blockSize;
    local($ib, $ib_magic);

    $ib = &BlockRead(*AGGR, $block, $blockSize);

    $ib_magic = &util'MapGet($ib, 'magic', %INDIRECT_HEADER_MAP);
    if ($ib_magic != $INDIRECT_MAGIC) {
	warn("Bad indirect block at offset $offset");
	return ();
    }
    return ($ib, $offset);
}

sub IndirectPrint {
    local($ib) = @_;
    &util'FormatPrint($ib, @INDIRECT_HEADER_FORMAT);
}

sub IndirectsPerBlock {
    local($block_size) = @_;
    return int(($block_size - $INDIRECT_HEADER_SIZE - 4) / 4);
}

sub ExtractBlockAddr {
    local($indirect_block) = @_;
    unpack("x$INDIRECT_HEADER_SIZE L$ibp_per_block", $indirect_block);
}

#
# Test for valid block address.
#
sub IsValidBlockAddr {
    local($blkno, $backing_OK) = @_;
    local($first) = &GetSuperMember('firstBlock');
    local($nblocks) = &GetSuperMember('nBlocks');

    $blkno &= ~$ANODE_BLOCKBACKING if (defined($backing_OK) && $backing_OK);
    return $first <= $blkno && $blkno < $first + $nblocks;
}

#
# Extract a value from various structures.
#
sub GetAnodeMember {
    &util'MapGet(@_, $ANODE_MAP0{$_[1]} ? %ANODE_MAP0 : &AnodeGetMap($_[0]));
}

sub GetAnodeBlockMember {
    &util'MapGet(@_, %ABH_MAP);
}

sub GetAggrMember {
    &util'MapGet(@_, %AGGR_HEADER_MAP);
}

sub GetBitmapPageMember {
    &util'MapGet(@_, %BITMAP_PAGE_MAP);
}

sub GetBadblockFrameMember {
    &util'MapGet(@_, %BB_FRAME_MAP);
}

sub GetBadblockEntryMember {
    &util'MapGet(@_, %BB_ENTRY_MAP);
}

sub GetFSMember {
    &util'MapGet(@_, %FS_HEADER_MAP);
}

sub GetDirPageMember {
    &util'MapGet(@_, %DIR_HEADER_MAP);
}

sub GetDirEntryMember {
    &util'MapGet(@_, %DIR_ENTRY_HEADER_MAP);
}

sub GetDirHash {
    local($dh) = @_;
    local($hoffset) = &util'MapOffset('hashTable[0..3]', %DIR_HEADER_MAP);
    return unpack("x$hoffset, C$DIR_HASH_SIZE", $dh);
}

#
# Return the offset & length of the fragment bitmap within a bitmap page
#
sub FindFragmentMap {
    local($bpage) = @_;
    local($nblks) = &GetBitmapPageMember($bpage, 'blocksPerPage');
    local($fpb) = &GetBitmapPageMember($bpage, 'fragmentsPerBlock');
    return ($BITMAP_PAGE_SIZE + $nblks / 8, int(($nblks * $fpb + 7) / 8));
}

#
# Ditto for the other bitmap
#
sub FindLoggedFragMap {
    local($bpage) = @_;
    local($nblks) = &GetBitmapPageMember($bpage, 'blocksPerPage');
    local($fpb) = &GetBitmapPageMember($bpage, 'fragmentsPerBlock');
    return ($BITMAP_PAGE_SIZE, int(($nblks + 7) / 8));
}

#
# Return the backing anode for this anode.
#
sub GetBackingAnode {
    local(*DEVICE, $anode) = @_;
    local($bBlock) = &GetAnodeMember($anode, 'bBlock');
    local($bOffset) = &GetAnodeMember($anode, 'bOffset');
    local($bData) = &BlockRead(*DEVICE, $bBlock, $blockSize);
    substr($bData, $bOffset, ANODE_SIZE);
}

sub Write {
    local(*DEVICE, $buf, $offset, $length) = @_;

    seek(DEVICE, $offset, 0) || die("seek failed: $!");
    syswrite(DEVICE, $buf, $length) || die("write failed: $!");
    &UpdateReadBuffer($buf, $offset, $length);
}

1;
