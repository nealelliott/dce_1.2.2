/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: process_commands.c,v $
 * Revision 1.1.9.2  1996/02/18  19:30:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:42  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:11:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:00  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:01:18  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:44:15  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:37:40  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:41  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:00:46  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:06:10  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  13:08:54  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:40:51  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/17  17:21:33  m_sawyer
 * 	Removed control-c checks.
 * 	[1992/11/17  15:14:24  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:54:07  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:23:24  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:04  devrcs
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

/* this is the main loop of a management program */

#include <cdscp.h>
#include <prsdef.h>
#include <cmdblk.h>
#include <logging.h>

/* process commands is the main loop in the main line. */
/* it is also the main loop when processing a nested file of commands. */
/* the exit command is never seen here.  it causes exit. */
/* end of file just causes return to caller. */
/* if already at top level, that routine might interpret the EOF as EXIT. */

int
process_commands (
    struct cmdblk *cbp,			/* pointer to command block */
    struct op_blk *obp,			/* ptr to head of output list */
    struct entblk *elp,			/* ptr to head of entity list */
    struct attrblk *alp,		/* ptr to head of attributes list */
    struct logblk *logp)		/* ptr to logging block */
{
    int done = FALSE;			/* false until a reason to break out */
    int cmd_status=DNS_SUCCESS;		/* out parm for exec_cmd */
    int rtn_status=DNS_SUCCESS;         /* what we return to our caller */

    while ( !done )
	{
	int status;

	cbp->func = NULLCMD;		/* forget about previous command */
        delete_all_err_list_elements(	/* start with empty error list */
	    get_err_blk_ptr());		/* needed if prev cmd had ctl c */
	delete_all_op_list_elements(obp);/* nowhere to write the results, yet */
	status = get_command(cbp);	/* command may be many lines */
	if (status == EOF)
	    {
	    done = TRUE;		/* no more commands */
	    }
	else
	    {
	    int parse_status;		/* returned by parser */

	    log_command(logp, cbp, obp);	/* capture the input */
	    delete_ent_list(elp);	/* do not reuse old entities */
	    delete_attr_list(alp);	/* do not reuse old attributes */
	    make_withlist_empty(
		withlist_pointer());	/* do not reuse old WITH filters */
	    parse_status = 
		parse_command(cbp);	/* figure out what it means */
	    form_with_list();		/* set up new filters. see with.c */
	    allow_default_entity(elp);	/* use default entity if none given */
	    if (parse_status == RET_SUCCESS)	/* valid command? */
		{				/* yes */
		allow_default_op_list(obp);	/* so we can write somewhere */
		if ( open_all_files(obp) == DNS_SUCCESS ) {
		    done = exec_cmd(cbp, obp, logp, &cmd_status);/* act on it */
		    close_all_files(obp);
		    }
		else done = TRUE;
		}
	    else if (parse_status == RET_EOF)	/* request to end file ? */
		{				/* yes */
		done = TRUE;			/* force the exit to happen */
		}
	    else
		{			/* something wrong with parse */
		use_default_op_list(obp);	/* so we can write somewhere */
		report_parse_error(cbp, parse_status);
		}
	    }
        if (cmd_status != DNS_SUCCESS)
            {
            rtn_status = cmd_status;
            }
	}				/* end of while not done */
    return (rtn_status);
}
