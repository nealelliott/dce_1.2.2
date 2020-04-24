/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: casestrcpy.c,v $
 * Revision 1.1.10.1  1996/10/02  21:13:10  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:59  damon]
 *
 * Revision 1.1.5.1  1994/06/09  14:24:44  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:36:50  annie]
 * 
 * Revision 1.1.3.3  1993/01/21  16:31:09  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:24:47  cjd]
 * 
 * Revision 1.1.3.2  1992/09/25  19:47:20  jaffe
 * 	Transarc delta: jaffe-ot5416-cleanup-RCS-log-entries 1.1
 * 	  Selected comments:
 * 	    Cleanup extra RCS log entries.  There should only be one per file, and
 * 	    it should be closed with an EndLog comment.
 * 	[1992/09/23  20:06:26  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:45:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
****************************************************************************
*        Copyright IBM Corporation 1988, 1989 - All Rights Reserved        *
*                                                                          *
* Permission to use, copy, modify, and distribute this software and its    *
* documentation for any purpose and without fee is hereby granted,         *
* provided that the above copyright notice appear in all copies and        *
* that both that copyright notice and this permission notice appear in     *
* supporting documentation, and that the name of IBM not be used in        *
* advertising or publicity pertaining to distribution of the software      *
* without specific, written prior permission.                              *
*                                                                          *
* IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL IBM *
* BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY      *
* DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER  *
* IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING   *
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.    *
****************************************************************************
*/
#ifndef lint
static char rcsid[] = "$Header: /u0/rcs_trees/dce/rcs/file/util/casestrcpy.c,v 1.1.10.1 1996/10/02 21:13:10 damon Exp $";
#endif

#include <ctype.h>

/* Just like strncpy but shift-case in transit and forces null termination */

char *lcstring (d, s, n)
  char *d;				/* dest string */
  char *s;				/* source string */
  int   n;				/* max transfer size */
{   char *original_d = d;
    char  c;

    if ((s == 0) || (d == 0)) return 0;	/* just to be safe */
    while (n) {
	c = *s++;
	if (isupper(c)) c = tolower(c);
	*d++ = c;
	if (c == 0) break;		/* quit after transferring null */
	if (--n == 0) *(d-1) = 0;	/* make sure null terminated */
    }
    return original_d;
}

char *ucstring (d, s, n)
  char *d;
  char *s;
  int   n;
{   char *original_d = d;
    char  c;

    if ((s == 0) || (d == 0)) return 0;
    while (n) {
	c = *s++;
	if (islower(c)) c = toupper(c);
	*d++ = c;
	if (c == 0) break;
	if (--n == 0) *(d-1) = 0;	/* make sure null terminated */
    }
    return original_d;
}
