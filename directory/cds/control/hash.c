/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: hash.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:21  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:10:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:43  root]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:24  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:20  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:06  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:04:14  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:07:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:38:35  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */

#include <limits.h>

/* this might look machine independent, but it is not. */

#define valsperchar (UCHAR_MAX + 1)

int
hash (
    unsigned char *cp,			/* ptr to first char of key */
    int size,				/* number of bytes in key */
    int div)				/* answer is in range 0 - div-1 */
{
    const int small = size % sizeof(int); /* how many bytes of key not in int */
    const int limit = size/sizeof(int);	/* loop control */
    int *p = (int *)(cp + small);	/* ptr that scans the key */
    unsigned int tot = 0;		/* the hash, as it is made */
    int i;				/* loop control */

    for (i=0; i<small; i++)		/* use each of 0,1,2, or 3 bytes */
	{
	tot = tot*valsperchar + cp[i];	/* load that many bytes */
	}

    for (i=0; i<limit; i++)		/* for each full int in key */
	{
	struct				/* to force alignment */
	    {
	    int x;			/* so this is aligned */
	    } aligned_int;

	memcpy(				/* move to natural alignment */
	    &aligned_int,		/* to the aligned int */
	    &p[i],			/* from the key to be hashed */
	    sizeof(int));		/* one int worth */
	tot = 
	    (tot<<1) ^ aligned_int.x;	/* shift and xor */
	}
    return(div ? tot%div : tot);	/* divide or let caller do it */
}
