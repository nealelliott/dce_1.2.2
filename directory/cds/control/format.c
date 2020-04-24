/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: format.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:02  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:09:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:33  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:55  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:49:03  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:56  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:45  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:59:10  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:01:38  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:05:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:36:45  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:11  devrcs
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
#include <dce/dce_utils.h>
void
format (
	unsigned char	*bufptr,	/* ptr to input data */
	int		size,		/* size of input data */
	char		*x)		/* ptr to output buffer */
{
    char *in_p = (char *)bufptr;
    char *pre = in_p;			/* prescan pointer */
    char *end_p = &in_p[size];		/* end of string to scan */
    char *out_p = x;
    int printables = 0;			/* number of printable chars in input*/
    int nonprints = 0;			/* number that will not print */
    int numquotes = 0;			/* number of quote chars in input */

    while (pre < end_p)			/* look at each character */
	{
	const int len_mb = LEN_mbc(pre);
	int len = len_mb ? len_mb : 1;

	if (isdcepcs(*pre) && isprint(*pre))
	    printables += len;		/* count the printable chars */
	else
	    nonprints += len;		/* count the non printable chars */
	if (*pre == '"')
	    numquotes++;		/* count the quotes */

	pre += len;
	}				/* looked at each character */

    if ((nonprints == 0) && (numquotes == 0))
	{
	*out_p++ = '"';			/* opening quote */
	memcpy(out_p, in_p, size);	/* the data, problem-free */
	out_p += size;
	*out_p++ = '"';			/* terminal quote */
	*out_p = '\0';			/* terminal null  */
	}
    else if (nonprints == 0)		/* all printable but embedded quotes */
	{
	*out_p++ = '"';			/* opening quote */
	while (in_p < end_p)
	    {
	    if (*in_p == '"')		/* is this a quote char */
		{			/* yes, so copy two of them */
		*out_p++ = '"';		/* the first one */
		*out_p++ = '"';		/* the second one */
		in_p++;
		}
	    else			/* it is a normal character */
		{
		COPY_mbc_and_skip(in_p, out_p); /* copy character and bump ptr */
		}
	    }				/* end of for, each character moved */
	*out_p++ = '"';			/* add the terminal quote mark */
	*out_p   = '\0';		/* end of string marker */
	}
    else if (9*(nonprints+numquotes) <= printables)
	{
	*out_p++ = '"';			/* opening quote of asciz string */
	while (in_p < end_p)		/* look at each char in turn */
	    {
	    unsigned char c = *in_p;	/* pick up the character */
	    const int len_mb = LEN_mbc(in_p);
	    int len = len_mb ? len_mb : 1;

	    if (isdcepcs(c) && isprint(c)) /* printable character ? */
		{			/* yes */
		COPY_mbc_and_skip(in_p, out_p);
		}
	    else do
		{			/* nonprintable so use \nnn form */
		sprintf(out_p, "\\%03.03o", *in_p++);
		out_p += 4;		/* point at the null */
		}
	    while (0 < --len);
	    }
	*out_p++ = '"';			/* ending quote for string */
	*out_p = '\0';			/* null terminate the string */
	}
    else				/* nothing else works, so ... */
	{				/* messy, so use hex */
	*out_p++ = '%';			/* 1st char of hex introducer */
	*out_p++ = 'x';			/* 2nd char of hex introducer */
	while (in_p < end_p)		/* look at each character */
	    {
	    sprintf(out_p, "%02.2x", *in_p++);
	    out_p += 2;
	    }				/* end of for loop for hex value */
	}				/* end of hex option */
}					/* end of format routine */
