/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: astab.h,v $
 * Revision 1.1.720.1  1996/10/02  21:11:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:26  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1990 Transarc Corporation -- All Rights Reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/xaggr/astab.h,v 1.1.720.1 1996/10/02 21:11:21 damon Exp $ */

/*
 *		Aggregate table entry format
 */

#ifndef	_ASTABH_
#define	_ASTABH_

#define	ASTAB_INFIX	"/var/dfs/"
#define	ASTAB_SFX		"dfstab"

extern char *AstabFileName;

/* Sizes of strings */

#define	ASTABSIZE_SPEC	512
#define	ASTABSIZE_DIR	512
#define	ASTABSIZE_TYPE	64

/* Entry structure */

struct astab {
    char    as_spec[ASTABSIZE_SPEC];	/* block special device name */
    char    as_aggrName[ASTABSIZE_DIR];	/* Aggregate name */
    char    as_type[ASTABSIZE_TYPE];   	/* type of physical file system */
    u_long  as_aggrId;   		/* Aggregate Id */
};

/* File system extra data:  UFS version */

struct ufs_astab {
    afs_hyper_t uas_volId;		/* 1st & only volume */
    char uas_mountedon[ASTABSIZE_DIR];	/* name of mounted-on dir */
};

/* Values for as_type */

#define	ASTABTYPE_UFS	"ufs"
#define	ASTABTYPE_EPI	"lfs"
#define	ASTABTYPE_AIX3	"aix3"
#define	ASTABTYPE_VXFS	"vxfs"
#define	ASTABTYPE_DMEPI	"dmepi"

/*
 * Macros to make addition of new file system types in the future
 * easier.
 */
#define ASTABTYPE_SUPPORTS_EFS(atype) ((strncmp(atype, ASTABTYPE_EPI,  ASTABSIZE_TYPE) == 0) || \
				       (strncmp(atype, ASTABTYPE_VXFS, ASTABSIZE_TYPE) == 0))

#define ASTABTYPE_TO_AGTYPE(astype, agtype)  \
    if (!strncmp(astype, ASTABTYPE_UFS, ASTABSIZE_TYPE)) \
         agtype = AG_TYPE_UFS; \
    else if (!strncmp(astype, ASTABTYPE_EPI, ASTABSIZE_TYPE)) \
         agtype = AG_TYPE_EPI; \
    else if (!strncmp(astype, ASTABTYPE_VXFS, ASTABSIZE_TYPE)) \
         agtype = AG_TYPE_VXFS; \
    else if (!strncmp(astype, ASTABTYPE_DMEPI, ASTABSIZE_TYPE)) \
         agtype = AG_TYPE_DMEPI; \
    else \
         agtype = AG_TYPE_UNKNOWN

#endif	/* _ASTABH_ */
