/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fopen2.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:01  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:09:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:32  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:54  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:49:05  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:55  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:41  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:59:09  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:01:32  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:05:44  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:36:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:10  devrcs
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

#include <cdscp.h>

char *getenv(const char *);		/* to use getenv without a cast */

FILE *
fopen2 (				/* open a file or its substitute */
    char *spec,				/* the primary file to open */
    char *mode)				/* the access mode */
{
    return fopen(fopen2aux1(spec), mode);
}

char *
fopen2aux1(				/* make replacement name available */
    char *inspecp)			/* ptr to the original spec */
{					/* do no mallocs */
    char *const fnp = strrchr(inspecp, '/');
    char *const bnp = fnp ? &fnp[1] : inspecp;
    char *const evp = getenv(bnp);		/* find the replacement name */

    return(evp ? evp : inspecp);	/* use it if possible, else the input */
}					/* use it now, or save it */

static /* const */ char thisnull = '\0';/* shared by fopen2aux2 & fopen2aux3 */

char *
fopen2aux2 (				/* make replacement name available */
    char *inspecp)			/* ptr to the original spec */
{					/* ok to do a malloc */
    char *const evp = fopen2aux1(inspecp);
    int len = strlen(evp) + 1;		/* strlen of name */
    char *const p = malloc(len);	/* ptr to safe copy of it */

    if (p) {
	memcpy(p, evp, len);

	return(p);			/* give caller our best result */
    } else
	return(&thisnull);		/* use ptr to null as fallback */
}

void
fopen2aux3 (				/* give back mem for replacement name */
    char *p)				/* ptr to the name from fopen2aux2 */
{
    if (p != &thisnull)			/* avoid free if no malloc */
	free(p);
    return;				/* void routine returns no value */
}
