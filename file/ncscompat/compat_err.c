/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: compat_err.c,v $
 * Revision 1.1.43.1  1996/10/02  17:54:25  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:09  damon]
 *
 * Revision 1.1.38.3  1994/07/13  22:28:37  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:41:33  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:52  mbs]
 * 
 * Revision 1.1.38.2  1994/06/09  14:13:19  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:26:29  annie]
 * 
 * Revision 1.1.38.1  1994/02/04  20:22:44  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:54  devsrc]
 * 
 * Revision 1.1.36.1  1993/12/07  17:28:05  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:49:28  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/19  16:08:19  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  15:11:49  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  17:55:11  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:10:05  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:47:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <compat.h>
#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */
#include <string.h>

static pthread_key_t errBufKey;


/* dfs_copyDceErrTxt
 *	put dce error text in user supplied buffer.
 * entry:
 *	st - status to decode
 *	txtbuf - for resulting error string
 *	bufsize - size of txtbuf
 * exit
 *	txtbuf - error text
 */

void dfs_copyDceErrTxt(st, txtbuf, bufsize)
unsigned long st;
char *txtbuf;
int bufsize;
{
    int st2;				/* dummy for dce_error_inq_text */
    dce_error_string_t Res;
    extern const int sys_nerr;

    if (st >= 0 && st < sys_nerr) {
	(void)strncpy(txtbuf, strerror(st), bufsize);
	txtbuf[bufsize - 1] = '\0';
	return;
    }

    dce_error_inq_text(st, Res, &st2);
    strncpy(txtbuf, (char *)Res, bufsize);
    txtbuf[bufsize - 1] = '\0';
}

static void destroyKey(void *buf)
{
    if (buf != NULL)
	osi_Free(buf, NCSCOMPAT_ERR_BUF_SIZE);
}

static void createKey(void)
{
    (void)pthread_keycreate(&errBufKey, destroyKey);
}

/* dfs_dceErrTxt
 *	decode dce error status
 * entry:
 *	st - status to decode
 * exit:
 *	dfs_dceErrTxt - ptr to static string holding error text
 */

char *dfs_dceErrTxt(st)
unsigned long st;
{
    char* errBuf;
    static pthread_once_t createKeyOnce = pthread_once_init;
    int failure;

    failure = pthread_once(&createKeyOnce, createKey);
    osi_assert(!failure);

    failure = pthread_getspecific(errBufKey, (pthread_addr_t*)&errBuf);
    if (failure)
	return "pthread_keycreate failed: out of memory";

    if (errBuf == NULL) {
	errBuf = osi_Alloc(NCSCOMPAT_ERR_BUF_SIZE);
	if (errBuf == NULL)
	    return "failed to allocated error buffer";
	failure = pthread_setspecific(errBufKey, errBuf);
	osi_assert(!failure);
    }


    dfs_copyDceErrTxt(st, errBuf, NCSCOMPAT_ERR_BUF_SIZE);
    return errBuf;
}

