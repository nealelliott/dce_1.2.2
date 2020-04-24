/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: string_lib.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:14  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:27:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:34  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:55  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:04:38  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:31  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:44  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:55:38  peckham
 * 	Eliminate use of dnstables.
 * 	[1994/04/29  14:35:09  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:08:51  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:35:05  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:17:21  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:17:14  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module string_lib.c
 *
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
 *
 */


#include <dce/dce.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <string_lib.h>
#include <assert.h>

int 
dns_bcmp (char  *s1,
          char  *s2,
          int   n)
{
    if (n)
	do {
	    if (*s1++ != *s2++)
		break;
	} while (--n);

    return(n);
/*	return (memcmp(s1, s2, n));	*/
}

int 
dns_bcmp_lexical (char  *s1,
                  char  *s2,
                  int   n)
{
    int ret = memcmp(s1, s2, n);

    if (ret < 0)
	return(-1);
    if (0 < ret)
	return(1);
    return(0);
}

void 
dns_bcopy (char  *from,
           char  *to,
           int   size)
{
    if ((from <= to) && (to < from + size)) {
	char *out = to + size;
	char *in = from + size;

	while (to < out)
	    *--out = *--in;
    } else if ((to <= from) && (from < to + size)) {
	if (size)
	    do
		*to++ = *from++;
	    while (--size);
    } else
	(void)memcpy(to, from, size);	/* note difference in call seq.  */
}

void 
dns_bzero (char  *base,
           int   length)
{
	(void)memset(base, 0, length);
	return;
}

void 
dns_lower (unsigned char  *from,
           unsigned char  *to,
           int            size)
{
    if (size)
	do
	    *to++ = islower(*from) ? *from : tolower(*from);
	while (from++, --size);
}

void 
dns_upper (unsigned char  *from,
           unsigned char  *to,
           int            size)
{
    if (size)
	do
	    *to++ = isupper(*from) ? *from : toupper(*from);
	while (from++, --size);
}

char *
dns_rindex (char  *string_p,
            int   single_char)
{
    if (string_p) {
	register char *end_p = strchr(string_p, '\0');
	while (string_p <= --end_p  ) {
	    if (*end_p == single_char)
		return (end_p);
	}
    }

    return (0);
}

int 
dns_strcasecmp (char  str1_p[],
                char  str2_p[])
{
    for (;; str1_p++, str2_p++) {
	char c1 = islower(*str1_p) ? *str1_p : tolower(*str1_p);
	char c2 = islower(*str2_p) ? *str2_p : tolower(*str2_p);

	if ((c1 != c2) || !c1 || !c2)
	    return (c1 - c2);
    }
}






