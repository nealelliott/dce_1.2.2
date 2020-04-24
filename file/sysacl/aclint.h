/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: aclint.h,v $
 * Revision 1.1.10.1  1996/10/02  21:01:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:57  damon]
 *
 * Revision 1.1.5.1  1994/06/09  14:20:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:51  annie]
 * 
 * Revision 1.1.3.2  1993/01/21  15:20:44  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:59:49  cjd]
 * 
 * Revision 1.1  1992/01/19  02:55:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * (C) Copyright 1994, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /u0/rcs_trees/dce/rcs/file/sysacl/aclint.h,v 1.1.10.1 1996/10/02 21:01:01 damon Exp $
 *
 * aclint.h
 *	Definitions used in passing ACL's across various interfaces.
 */

#ifndef _TRANSARC_ACLINT_H_
#define _TRANSARC_ACLINT_H_ 1

/* values for which-acl */

#define VNX_ACL_REGULAR_ACL	0	/* regular */
#define VNX_ACL_DEFAULT_ACL	1	/* default for (sub)directories */
#define VNX_ACL_INITIAL_ACL	2	/* default for (sub)files */

#define VNX_ACL_DELEGATE_ENTRY_OK 4  

#define VNX_ACL_TYPE_MASK  (VNX_ACL_REGULAR_ACL | VNX_ACL_DEFAULT_ACL | \
			    VNX_ACL_INITIAL_ACL)

/* structure identical to afsACL */

/* define this as (8k - sizeof(long)) so that a ``struct dfs_acl'' fits in an 8k free-buffer. */
#define MAXDFSACL	8188
struct dfs_acl {			/* identical to afsACL in afs4int.h */
    long dfs_acl_len;			/* length */
    char dfs_acl_val[MAXDFSACL];	/* data */
};

/* Opcodes for syscall */

#define VNX_OPCODE_GETACL	0
#define VNX_OPCODE_SETACL	1
#define VNX_OPCODE_COPYACL	2
#define VNX_OPCODE_QUOTA	3

#endif /* _TRANSARC_ACLINT_H_ */
