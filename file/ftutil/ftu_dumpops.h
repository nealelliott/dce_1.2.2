/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_dumpops.h,v $
 * Revision 1.1.22.1  1996/10/02  17:48:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:31  damon]
 *
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_dumpops.h,v 1.1.22.1 1996/10/02 17:48:33 damon Exp $ */

#ifndef _TRANSARC_DFS_FTU_DUMPOPS_H_
#define _TRANSARC_DFS_FTU_DUMPOPS_H_

/* 
 * Definition of new dump format.
 * Volume dump consists of a volume header dump followed by a set of individual
 * vnode dumps, followed by an end-of-volume opcode. Individual vnode dumps
 * consist of a vnode start opcode, a bunch of vnode descriptor opcodes, and
 * an EOV opcode.
 *
 * Fields are generally represented as a two byte opcode followed by a 2 byte
 * field length, with the length not including the opcode and length fields.
 * A data opcode field is recognized as having an opcode such that
 * (opcode & 0xf000) == 0xf000.
 * These opcodes are followed by a 48 bit length that follow the 16 bit opcode.
 * The 48 bit length is represented byte by byte from MSB to LSB for 6 bytes.
 *
 * Some fields contain shorts, which are represented as two bytes, MSB first.
 * Longs are represented as 4 bytes, MSB first. Hypers are represented as 8
 * bytes, MSB first.  Strings are represented as null-terminated arrays of
 * bytes. In many cases, the null terminator is redundant, as the overall
 * field length implies where the null-terminator must appear in the string.
 */

struct vols_opcode {
    u_short opcode;
    union {
	struct vols_shortField {
	    u_short length;
	} s;
	struct vols_longField {
	    u_short high;
	    long length;
	} l;
    } f;
};


/* 
 * Start of a dump; followed by long magic number 
 */
#define VOLS_DUMP_HEADER_START		0x80	/* start of a volume dump */
#define VOLS_DUMP_HEADER_STARTMAGIC	0x343833af

/* 
 * Max uniquifier gives the unique counter state we're up to.  Followed by 8
 * longs, in case we're generating entire vnode+unique pairs via some obscure
 * encryption.
 */
#define VOLS_DUMP_HEADER_UNIQUEGEN	0x100
#define VOLS_DUMP_NUIDS			8

/*
 * Max and min quotas give the maximum number of 1024 byte units that this
 * volume can use, as well as the minimum guaranteed as reserved for the volume.
 * Either can be 0, meaning unlimited / no space reserved.
 */
/* #define VOLS_DUMP_HEADER_MAXQUOTA	0x101 */
#define VOLS_DUMP_HEADER_MINQUOTA	0x102

/* 
 * The disk used field keeps track of the amount of 1024 byte blocks actually
 * used, so that after a restore, we can adjust the running count of space used.
 * The opcode is followed by a single long integer, representing the number of
 * blocks.
 */
/* OBSOLETE as of DCE 1.0.2 */
#define VOLS_DUMP_HEADER_DISKUSED	0x103

/* 
 * The file count is also a running count maintained over dumps. The opcode is 
 * followed by a single long, representing the count.
 */
/* OBSOLETE as of DCE 1.0.2 */
#define VOLS_DUMP_HEADER_FILECOUNT	0x104

/* 	
 * The owner of a volume is a user, just like the other user-naming fields. The 
 * owner is represented as a long integer, and directly follows the opcode
 * field.
 */
#define VOLS_DUMP_HEADER_OWNER		0x105

/*
 * The creation date of a volume represents the date that the volume ID was
 * first used by this volume. It is followed by two long integers representing
 * seconds and msecs since 1/1/1970; this will be referred to as an Unix time
 * stamp.
 */
#define VOLS_DUMP_HEADER_CREATION_DATE	0x106

/* 
 * The access date of a volume records the last time that the volume was
 * accessed.  The date is represented as a Unix time stamp.
 */
#define VOLS_DUMP_HEADER_ACCESS_DATE	0x107

/* 
 * The update date of a volume records the last time that the volume was
 * updated.  The date is represented as a Unix time stamp.
 */
#define VOLS_DUMP_HEADER_UPDATE_DATE	0x108

/* 
 * The volume message-of-the-day is a string that gives human readable
 * information about the particular state of this volume (why the volume is
 * unavailable, when it'll be back, etc.) The opcode is followed by a string
 * representing the message.
 */
#define VOLS_DUMP_HEADER_MOTD		0x109

/* 
 * The following opcode introduces a vnode.  The vnode reader reads the
 * remainder of the dump, until an VOLS_DUMP_VNODE_EOF opcode is hit. This
 * opcode is followed by a afs_hyper_t that tells the vnode/unique for this file.
 */
#define VOLS_DUMP_HEADER_VNODE		0x10a

/* 
 * The following opcode introduces a vnode that has been deleted.
 * This opcode is followed by a afs_hyper_t that tells the vnode/unique for this file.
 */
#define VOLS_DUMP_HEADER_NOVNODE	0x10b

/*
 * The volume version number is an 8-byte monotonically-increasing
 * number that is used to stamp each change made to the entire volume.
 */
#define	VOLS_DUMP_HEADER_VOLVERSION 0x10c

/*
 * The dump type gives the type of aggregate from which this dump was
 * made.  It thus provides a clue as to how to interpret directory formats,
 * ACL connections, and so forth.  It is a afs_hyper_t (two unsigned 32-bit integers).
*/
#define	VOLS_DUMP_HEADER_BASETYPE   0x10d

/*
 * The dump source style records how the information in the source was selected.
 * It is represented as eleven longs: a flag word, a time value in two words,
 * a version number in two words, and three spare hypers.
 * If the source was a full dump, the flag word is zero.
 * If the source was incremental-by-date but the dump includes all directories,
 * the flag word is 1 and the time value gives the cutoff date.
 * If the source was incremental by version but the dump includes all
 * directories, the flag word is 2 and the version is the cutoff volume version
 * number, but the dump includes all directories.
 * If the source was incremental-by-date, the flag word is 3 and
 * the time value gives the cutoff date.
 * If the source was incremental by version, the flag word is 4 and the
 * version is the cutoff volume version number.
 */
#define	VOLS_DUMP_HEADER_DUMP_STYLE	0x10e

/*
 * VISQUOTALIMIT and ALLOCLIMIT are hypers giving the two quota limits in bytes.
 * They are represented as hypers.
 */
#define	VOLS_DUMP_HEADER_VISQUOTALIMIT	0x10f
#define	VOLS_DUMP_HEADER_ALLOCLIMIT	0x110

/*
 * The codeset used for the MOTD.
 */
#define	VOLS_DUMP_HEADER_MOTD_CODESET	0x111

/*
 * The maximum index represented in this dump.
 */
#define	VOLS_DUMP_HEADER_INDEX_MAX		0x112

/* 
 * The following opcode introduces a vnode that is past all existing vnodes.
 * That is, all vnodes from this index to the end of the volume should be deleted.
 * This opcode is followed by a afs_hyper_t that tells the vnode/unique for this file.
 */
#define VOLS_DUMP_HEADER_ENDVNODES	0x113

/* 
 * The following opcode records when the dump was made on the source machine.
 * The date is represented as a Unix time stamp.
 */
#define VOLS_DUMP_HEADER_DUMP_DATE	0x114

/* 
 * This new uniquifier-generator provides the data on which a filesystem can
 * base the uniquifier portion of file IDs, in case it's generating entire
 * vnode+unique pairs via some obscure encryption.  The data must be a
 * multiple of 4 bytes in length.
 * The older VOLS_DUMP_HEADER_UNIQUEGEN has been co-opted for other uses.
 */
#define VOLS_DUMP_HEADER_N_UNIQUEGEN	0x115


/* 
 * The next set of opcodes are found in the vnode area of a dump itself. The
 * vnode area ends when VOLS_DUMP_VNODE_EOF opcode is encountered.
 */

/*
 * The type of the vnode is represented by a long integer, following the opcode.
 */
#define VOLS_DUMP_VNODE_TYPE		0x200

/* 
 * The link count of the vnode is represented as a long integer in the dump.
 * The long integer's representation immediately follows the opcode.
 */
#define VOLS_DUMP_VNODE_LINKCOUNT	0x201

/* 
 * The dataversion of a file is represented as a afs_hyper_t following the opcode.
 * This value may be represented as the same as the mtime, or some other field
 * in the vnode, but we provide a separate place for it just in case a
 * particular type of file system implements it separately.  For instance, for
 * UFS we might use mtime, 'cause that's all there is, but st_mtime has the
 * problem that it is user settable, and thus could seriously mislead the
 * system.
 */
#define VOLS_DUMP_VNODE_DATAVERSION	0x202

/* 
 * The volume version of a file is a version number that increases monotonically
 * on every update to the file's data *or status*.  It is also represented as a
 * afs_hyper_t, immediately following the opcode.
 */
#define VOLS_DUMP_VNODE_VOLVERSION	0x203

/* 
 * The next field type is the file's access time.  It is represented as a Unix
 * time immediately following the operation code.  We also define MODTIME and
 * CHANGETIME for the time that a file's data is last changed, and the time
 * that the inode status information was last changed.  The server modtime is
 * a true date for when the data was last updated, rather than the date that
 * is stoerd in MODTIME, which might be changed by an application call.
 */
#define VOLS_DUMP_VNODE_ACCESSTIME	0x204
#define VOLS_DUMP_VNODE_MODTIME		0x205
#define VOLS_DUMP_VNODE_CHANGETIME	0x206
#define VOLS_DUMP_VNODE_SERVERMODTIME	0x207

/* 
 * The next three fields label various interesting users for a file. The owner
 * of a file is the unix owner. The group of a file is the unix group owner of
 * the file.  The author of a file is the person to last change the inode data
 * or status (except for implicit changes of the inode's access time). All
 * fields are represented as 32 bit integers immediately following the
 * appropriate operation code.
 */
#define VOLS_DUMP_VNODE_GROUP		0x208
#define VOLS_DUMP_VNODE_AUTHOR		0x209
#define VOLS_DUMP_VNODE_OWNER		0x20a

/* 
 * This field stores the Unix mode bits for a file. These bits are represented
 * as a long, although no system I'm aware of uses more than 16 of these
 * at present.
 */
#define VOLS_DUMP_VNODE_UNIXMODE	0x20b

/* 
 * The next field represents an access control list. The acl is represented as 
 * a null-terminated string, containing the external representation of the ACL.
 */
#define VOLS_DUMP_VNODE_ACL		0x20c
/* ((this entry is obsoleted by the VOLS_DUMP_VNODE_ACL_xxx entries)) */

/*
 * The size of the file.  This is followed by a afs_hyper_t giving the number of
 * bytes in the file.
 */
#define VOLS_DUMP_VNODE_SIZE		0x20d

/* 
 * The following two possibilities represent access control lists (ACLs).
 * A file may have a regular ACL.
 * A directory may have a regular ACL, an initial ACL for newly-created
 *    subdirectories, and an initial ACL for newly-created files in the directory.
 * The ACL may be represented in-line in the dump, or it may be represented
 * by reference to an ACL that has already been shipped in-line.
 * The _INLINE operator is followed by a four-byte integer giving the
 * code for which ACL is being represented (one of the VNX_ACL_xxx_ACL
 * values), followed by the data stream of the flattened ACL itself.
 * The _COPY operator is followed by two four-byte integers and a hyper:
 * 	a four-byte integer giving the code for which ACL is being set
 * 	a four-byte integer giving the code for which ACL of another vnode
 * 	    is being used.
 * 	a afs_hyper_t giving the vnode index and uniquifier value for the vnode
 * 	    one of whose ACLs is to be copied.
 * The semantics of the _COPY operator is that the identified ACL of the current
 * vnode is set to be a copy of the (other) identified ACL of the other vnode.
 * That ACL (of the other vnode) may or may not have been shipped in the
 * dump.
 */
#define VOLS_DUMP_VNODE_ACL_INLINE 	0x20e
#define VOLS_DUMP_VNODE_ACL_COPY		0x20f

/*
 * The next field gives a 32-bit value for the client-only attributes.
 */
#define VOLS_DUMP_VNODE_CLIENTATTRS	0x212
/*
 * The next field gives a long for the device number
 */
#define VOLS_DUMP_VNODE_RDEV	0x213

/*
 * The next field is only present if the fileset was marked inconsistent
 * (has VOL_DELONSALVAGE set in its states field) at the time it was
 * dumped.  This allows us to preserve the inconsistent state at restore
 * time.
 */
#define VOLS_DUMP_HEADER_INCONSISTENT	0x214

/*
 * The next field is used by the dump routine to pad the data stream so
 * the next buffer allocated from the async queue will begin on a double
 * word boundary. 
 */
#define VOLS_DUMP_PAD			0x215

/*
 * The next field represents the vnode data, if present; the length follows the
 * opcode. This is a data opcode, which is followed with the data.  The first 8
 * bytes are a afs_hyper_t giving the file offset at which this data should be
 * written.  The remainder of the parameter is simply the data to be written.
 */
#define VOLS_DUMP_VNODE_DATA		0xf001	/* must be 0xwwwwfyyy */

/*
 * The next field introduces the directory contents; a version number follows
 * the opcode.  Since each directory entry is introduced with its own opcode,
 * the only reason this is a data opcode at all is because it will come
 * in the same part of the dump stream as the VOLS_DUMP_VNODE_DATA, so the
 * restore code needs to be able to fetch either opcode.
 */
#define VOLS_DUMP_VNODE_DIR		0xf002

/*
 * This opcode introduces each directory entry.  It is followed by a 
 * recommended offset (a afs_hyper_t), a vnode number, a codesetTag, and a filename.
 */
#define VOLS_DUMP_VNODE_DIR_ENTRY	0x210

/*
 * This opcode indicates the end of a directory.
 */
#define VOLS_DUMP_VNODE_DIR_END		0x211

/* 
 * The end of a vnode's dump. This opcode has no parameters. 
 */
#define VOLS_DUMP_VNODE_EOF		0x2ff

/* 
 * End of a dump; followed by a long magic # 
 */
#define VOLS_DUMP_HEADER_END		0x81	/* end of a volume dump */
#define VOLS_DUMP_HEADER_ENDMAGIC	0x4383faf7

/*
 * Version numbers for directory dump format
 */
#define VOLS_INITIAL_DIRDUMP_VERSION	1


/*
 * The following values define a system independent number space for vnode
 * types.
 */
typedef enum vols_vtype {
    VOLS_VTYPE_NON = 0,
    VOLS_VTYPE_REG = 1,
    VOLS_VTYPE_DIR = 2,
    VOLS_VTYPE_BLK = 3,
    VOLS_VTYPE_CHR = 4,
    VOLS_VTYPE_LNK = 5,
    VOLS_VTYPE_SOCK = 6,
    VOLS_VTYPE_BAD = 7,
    VOLS_VTYPE_FIFO = 8,
    VOLS_VTYPE_MPC = 9,
    VOLS_VTYPE_SPARE1 = 10,
    VOLS_VTYPE_SPARE2 = 11,
    VOLS_VTYPE_SPARE3 = 12,
    VOLS_VTYPE_SPARE4 = 13,
    VOLS_VTYPE_SPARE5 = 14,
    VOLS_VTYPE_SPARE6 = 15
} vols_vtype_t;

#endif	/* _TRANSARC_DFS_FTU_DUMPOPS_H_ */
