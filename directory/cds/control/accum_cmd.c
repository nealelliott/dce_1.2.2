/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: accum_cmd.c,v $
 * Revision 1.1.9.2  1996/02/18  19:27:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:50  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:06:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:10  root]
 * 
 * Revision 1.1.7.3  1994/08/03  19:00:05  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:35:52  mccann]
 * 
 * Revision 1.1.7.2  1994/06/09  16:08:15  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:02  devsrc]
 * 
 * Revision 1.1.7.1  1994/03/12  21:57:24  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:55:19  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  12:43:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:30:10  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/15  20:05:31  m_sawyer
 * 	change behavior of '-' and '!' to be normal characters
 * 	[1992/10/15  19:53:50  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:37:26  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:11:36  weisman]
 * 
 * Revision 1.1  1992/01/19  15:20:32  devrcs
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
#include <cmdblk.h>	/* moved common defs here, H. Tinkelenberg, 10-dec-91 */

/* a command is composed of one or more command lines. */
/* this routine moves characters from a command line in the from buffer to */
/* a command in the to buffer . */
/* the pointer to the result is updated. */

int
accum_cmd (
    char	*from,			/* pointer to the from buffer */
    char	**to)			/* where to find a ptr to result */
{
    char *p = *to;			/* ptr to current pos in to buffer */
    int last_was_space = FALSE;		/* last character was a space */
    int done = 0;			/* starts 0, 1 when ready to return */
    int result = ENDOFCMD;		/* result of scan operation */

    do	{
	switch (*from)
	    {
	    case '\"':
		*p++ = *from++;		/* copy quote char and advance */
		while (1)
		    {
		    char c = *from;
		    if ((*from == '\0') || (*from == '\n'))
			{
			from += LEN_mbc(from);
			done = 1;	/* force break out of loop */
			result = UNTERMQ;
			break;
			}
		    COPY_mbc_and_skip(from, p);
		    if (c == '\"')
			break;
		    }
		break;
	    case '\'':
		*p++ = *from++;		/* copy quote char and advance */
		while (1)
		    {
		    char c = *from;
		    if ((c == '\0') || (c == '\n'))
			{
			from += LEN_mbc(from);
			done = 1;	/* force break out of loop */
			result = UNTERMA;
			break;
			}
		    COPY_mbc_and_skip(from, p);
		    if (c == '\'')
			break;
		    }
		break;
	    case '-':
	    case '\\':
		/* dash or bs might be a continuation character and might not */
		/* if it is followed by optional white space and a comment */
		/* then it is a continuation. */
		/* otherwise it is part of a word. */
		{
		char *x = &from[1];	/* temp lookahead pointer */

		while (isspace(*x) || iscntrl(*x)) x += LEN_mbc(x);

		if ( (*x == '#') || (*x == '\0') || (*x == '\n') )
		    {
		    done = 1;		/* force break out of loop */
		    result = MORENEEDED;
		    }
		else
		    {			/* not continuation */
		    *p++ = *from++;	/* move it to output */
		    }
		}
		break;
	    case '!':
	    case '#':
	    case '\0':
	    case '\n':
		{			/* do not copy this to output */
		done = 1;		/* force break out of loop */
		result = ENDOFCMD;	/* no more lines to read */
		}
		break;
	    default:
		if (isspace(*from) || iscntrl(*from))
		    {
		    if (!last_was_space)
			{
			last_was_space = TRUE;
			*p++ = ' ';	/* one blank to output buffer */
			}
		    from++;		/* then advance to non white */
		    continue;
		    }
		else
		    {
		    COPY_mbc_and_skip(from, p);
		    break;
		    }
	    }
	    last_was_space = FALSE;
	} while (!done);		/* end do while for each character */

    *to = p;				/* record how far we got */	    
    *p  = '\0';				/* be sure to terminate the buffer */
    return(result);			/* tell the caller what happened */
}					/* end of accum_cmd */
