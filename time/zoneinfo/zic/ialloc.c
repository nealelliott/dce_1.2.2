/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ialloc.c,v $
 * Revision 1.1.7.2  1996/02/17  23:36:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:15  marty]
 *
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/17  16:43:35  pare]
 *
 * Revision 1.1.7.1  1995/12/09  20:58:03  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  20:24 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:26:45  root]
 * 
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/18  15:51:06  truitt]
 * 
 * Revision 1.1.4.1  1994/08/23  20:32:47  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:53:20  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:27:51  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  13:56 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  19:49  pare
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.9.3  1993/09/15  17:49:32  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.8.2  1993/07/08  20:54:07  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:53:40  root]
 * 
 * Revision 1.1.4.2  1993/03/11  17:29:30  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.2.3  1992/12/30  16:40:33  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:35:54  htf]
 * 
 * Revision 1.1.3.2  1992/11/11  15:25:20  smythe
 * 	adding new pure OSF DCE1.0.2 version to HP source base
 * 	[1992/11/11  15:21:12  smythe]
 * 
 * Revision 1.1.1.2  1992/11/10  21:58:32  osfrcs
 * 	    Loading P Smythe new 1.0.2 files to src/time
 * 
 * Revision 1.1.2.2  1992/10/20  19:09:24  comuzzi
 * 	Initial version
 * 	[1992/10/20  19:03:37  comuzzi]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Arthur David Olson of the National Cancer Institute.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
static char sccsid[] = "@(#)ialloc.c	5.3 (Berkeley) 4/20/91";
#endif /* not lint */

#ifdef notdef
static char	elsieid[] = "@(#)ialloc.c	8.18";
#endif

/*LINTLIBRARY*/

#include <string.h>
#include <stdlib.h>
#include <private.h>

#ifdef MAL
#define NULLMAL(x)	((x) == NULL || (x) == MAL)
#else /* !defined MAL */
#define NULLMAL(x)	((x) == NULL)
#endif /* !defined MAL */

#define nonzero(n)	(((n) == 0) ? 1 : (n))

char *	icalloc (int , int );
char *	icatalloc (char * const , const char * const );
char *	icpyalloc (const char * const );
char *	imalloc (const int );
char *	irealloc (char * const , const int );
void	ifree (char * const );
void    icfree( char * const );


char *
imalloc(const int n)
{
#ifdef MAL
	register char *	result;

	result = malloc((size_t) nonzero(n));
	return NULLMAL(result) ? NULL : result;
#else /* !defined MAL */
	return malloc((size_t) nonzero(n));
#endif /* !defined MAL */
}

char *
icalloc(int nelem, int elsize)
{
	if (nelem == 0 || elsize == 0)
		nelem = elsize = 1;
	return calloc((size_t) nelem, (size_t) elsize);
}

char *
irealloc(char * const pointer, const int size)
{
	if (NULLMAL(pointer))
		return imalloc(size);
	return realloc((void *) pointer, (size_t) nonzero(size));
}

char *
icatalloc(char * const old, const char * const new)
{
	register char *	result;
	register	oldsize, newsize;

	newsize = NULLMAL(new) ? 0 : strlen(new);
	if (NULLMAL(old))
		oldsize = 0;
	else if (newsize == 0)
		return old;
	else	oldsize = strlen(old);
	if ((result = irealloc(old, oldsize + newsize + 1)) != NULL)
		if (!NULLMAL(new))
			(void) strcpy(result + oldsize, new);
	return result;
}

char *
icpyalloc(const char * const string)
{
	return icatalloc((char *) NULL, string);
}

void
ifree(char * const p)
{
	if (!NULLMAL(p))
		(void) free(p);
}

void
icfree(char * const p)
{
	if (!NULLMAL(p))
		(void) free(p);
}
