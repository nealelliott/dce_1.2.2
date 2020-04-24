/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: argv.c,v $
 * Revision 1.1.6.2  1996/02/18  19:27:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:52  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:06:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:12  root]
 * 
 * Revision 1.1.4.2  1994/06/09  16:08:16  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:03  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:25  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:55:27  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:00:50  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:30:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:54  devrcs
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

int
argv_size (
	int	argc,			/* normal meaning, number of parms */
	char	*argv[],		/* pointer to vector of strings */
	int	argskip)		/* how many leading parms to skip */
{
    int i;				/* loop control */
    int size;				/* accumulated size of parms and */
					/* separating blanks, + \0 */

    size = 0;
    for (i=argskip; i<argc; i++)
	size += 1 + strlen(argv[i]);	/* string + trailing blank or \0 */
    return (size ? size : 1);		/* null string has trailing \0 */
}

char *
reassemble_command (
	int	argc,			/* normal meaning, number of parms */
	char	*argv[],		/* pointer to vector of strings */
	int	argskip,		/* how many leading parms to skip */
	char	*out)			/* where to put the result */
{
    char	*p = out;		/* where next char goes */
    int		i;			/* loop control */

    for (i=argskip; i<argc; i++)	/* for each of 0 or more args */
	{
	char *s = argv[i];		/* where next char comes from */

	if (p != out)
	    *p++ = ' ';			/* a blank between each arg */

	while (*s)			/* copy all except the \0 */
	    COPY_mbc_and_skip(s, p);

	}

    *p = '\0';				/* terminate the concatenated string */
    return(p);				/* return pointer to \0 */
}
