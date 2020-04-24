/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1995 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * Copyright (c) 1996, 1995 Transarc Corp.  All rights reserved.
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/libgwauth/authtest.h,v 1.1.2.1 1996/10/02 17:50:34 damon Exp $ */
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>


/*
 * Fake out enough of the kernel stubs to be able to test in user-space
 */

extern osi_cred_t *u_cred;
#define osi_setucred(c)		(u_cred = c)
#define osi_getucred()		(u_cred)

extern char *cm_sysname;

#define TEST_PAG	0xfffffff1
#define TEST_PAG1	0xfffffff2
#define TEST_PAG2	0xfffffff3

