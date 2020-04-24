/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_vm.h,v $
 * Revision 1.1.65.1  1996/10/02  17:19:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:28:58  damon]
 *
 * Revision 1.1.59.2  1994/06/09  13:59:08  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:34:27  annie]
 * 
 * Revision 1.1.59.1  1994/02/04  20:12:12  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:42  devsrc]
 * 
 * Revision 1.1.57.1  1993/12/07  17:17:53  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:48:49  jaffe]
 * 
 * $EndLog$
 */
/* test_vm.h -- contains defintions for the test_anode VM code. */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/test_vm.h,v 1.1.65.1 1996/10/02 17:19:29 damon Exp $ */

/* Copyright (C) 1993, 1991 Transarc Corporation - All rights reserved. */

#ifndef TRANSARC_EPI_ANODE_TEST_VM_H
#define TRANSARC_EPI_ANODE_TEST_VM_H

/*
 * Disable VM-specific testing if OS doesn't support it.
 *
 * XXX This should be set in a machine-dependent header file.
 */
#if defined(AFS_AIX_ENV) || defined(AFS_SUNOS5_ENV)
#define	OS_SUPPORTS_VM	1
#else
#define	OS_SUPPORTS_VM	0
#endif /* AFS_AIX_ENV || AFS_SUNOS5_ENV */

/* SHARED */
extern int useVM;

/* This structure is the primary interface between test_anode and the simulated
 * VM system. */

struct vm_buf {
    u_long base;			/* beginning offset in file */
    u_long length;			/* length of this buffer */
    char *addr;				/* file contents */
    opaque data;			/* for VM simulator */
};

/* for vm GetBuf interface */
#define VM_FORREAD 1			/* read-only reference */
#define VM_FORWRITE 2			/* intend to modify */

/* for vm Write interface */
#define VM_WRITE	1		/* write dirty pages through */
#define VM_SYNC		2		/* await writes before returning */
#define VM_AWAIT	4		/* await busy pages before writing */
#define VM_INVALIDATE	8		/* invalidate pages */

/* SHARED */
void vm_Init _TAKES(( void ));

/* SHARED */
long vm_GetBuf _TAKES((
  IN epi_anode_t file,
  IN int flags,
  IN u_long offset,
  IN u_long length,
  IN struct vm_buf *b));

/* SHARED */
void vm_Release _TAKES((
  IN struct vm_buf *b));

/* SHARED */
long vm_Write _TAKES((
  IN long flags,
  IN epi_anode_t file));

/* SHARED */
long vm_SetSizes _TAKES((
  IN char *whereami,
  IN u_long blockSize,
  IN u_long pageSize,
  IN u_long extentSize));

#endif /* TRANSARC_EPI_ANODE_TEST_VM_H */
