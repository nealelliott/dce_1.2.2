/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: get_command.c,v $
 * Revision 1.1.12.1  1996/08/09  11:57:12  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:09 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Incorporate fix for OT 12914: Use readline from libdce to provide
 * 	command line editing for cdscp.
 * 	[1996/02/18  19:29:36  marty  1.1.10.2]
 *
 * Revision 1.1.10.2  1996/02/18  19:29:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:20  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  15:10:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:42  root]
 * 
 * Revision 1.1.8.3  1994/08/03  19:01:05  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:37  mccann]
 * 
 * Revision 1.1.8.2  1994/06/09  18:37:08  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:57  devsrc]
 * 
 * Revision 1.1.8.1  1994/03/12  21:59:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:02:41  peckham]
 * 
 * Revision 1.1.6.4  1992/12/30  13:07:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:38:30  zeliff]
 * 
 * Revision 1.1.6.3  1992/11/17  17:21:11  m_sawyer
 * 	Rewrite get_command() to use simple fgets() instead of [the now defunct]
 * 	non-standard / non-portable command line recall and command line editing.
 * 	[1992/11/17  15:12:04  m_sawyer]
 * 
 * Revision 1.1.6.2  1992/09/29  19:11:06  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:11:21  weisman]
 * 
 * Revision 1.1.2.3  1992/05/06  16:33:37  m_sawyer
 * 	added #include <pthreads.h> for "terminal handling" and "catching signals"
 * 	[1992/05/06  15:55:52  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:49:54  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:22:10  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:58  devrcs
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
 *
 *
 * DESCRIPTION OF MODULE:
 *
 *  This module gets one complete command into a single buffer.
 *  The command might be on one or more command lines.
 *
 */

#include <cdscp.h>
#include <cmdblk.h>

char * readline( const char * );
void add_history( const char * );


/*
**  FUNCTION: get_command_line
**
**  PARAMETERS:
**	cbp : pointer to command block (input/output)
**
**  RETURN STATUS:
**	non-zero : success
**	0        : EOF (indicates user to exit dnscp)
**
*/

char *
get_command_line (
    struct cmdblk	*cbp)		/* ptr to command block */
{
    char		*status;

    if (cbp->mode == INTERACTIVE)
    {
	status = readline((cbp->num_lines == 0) ? cbp->pprompt : cbp->sprompt);
	if (status) {
	    strncpy(cbp->cipbuf, status, MAX_CL_LEN);
	    add_history(cbp->cipbuf);
	}
    }
    else
    {
	/*
	 * get the command input
	 */
	cbp->ciplen = MAX_CL_LEN;
	status = fgets(cbp->cipbuf, cbp->ciplen, cbp->ipfile);
    }

    /*
     * echo all commands read in batch mode to standard output
     */
    if ((status != 0) && (cbp->ipfile == stdin) && (cbp->mode == BATCH))
    {
	fputs (cbp->cipbuf, stdout);
	fflush (stdout);
    }

    return (status);			/* 0 if eof, otherwise not zero */
}

int
get_command (
    struct cmdblk *cbp)			/* ptr to command block */
{
    char *next = cbp->cmdbuf;		/* where the next cmd fragment goes */
    int result;
    int return_code = NOT_EOF;		/* code to give to caller */

    cbp->num_lines = 0;			/* no command lines yet, this cmd */

    for (result=MORENEEDED; result==MORENEEDED; )
	{
	char *status = get_command_line(cbp);    

	if (status == 0)		/* end of file? */
	    {				/* yes */
	    result = NOMORENEEDED;	/* break out of this loop */
	    return_code = EOF;		/* tell caller of the EOF */
	    }				/* use none of the command */
	else
	    {				/* guaranteed to fit in cmd buf ?? */
	    if (next < (cbp->cmdbuf + cbp->cmdlen - cbp->ciplen -2))
		{			/* yes, it will fit */
		result =		/* attach to what is already there */
		    accum_cmd(cbp->cipbuf, &next);
		}
	    else			/* it might fit, but might not. */
		{			/* treat too long cmd just like eof */
		result = NOMORENEEDED;	/* break out of this loop */
		return_code = EOF;	/* tell caller of the EOF */
		}			/* use none of the command */
	    }
	cbp->num_lines++;		/* count lines for this command */
	}

    return(return_code);
}
