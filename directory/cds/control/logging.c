/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: logging.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:24  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:10:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:44  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:01:07  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:32  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:29:47  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:23:29  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:26  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:22  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:11  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:04:28  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:07:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:39:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:39  devrcs
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


/* logging is tricky.  */
/* we want the log file to contain info about the command that turns it off. */
/* the commands do set up work */
/* later, the real change takes place */

#include <cdscp.h>
#include <cmdblk.h>
#include <logging.h>

int
init_logblk (
	struct logblk	*lp)		/* ptr to a log block */
{
    lp->kind_of_structure_code	= LOGGING_BLOCK;
    lp->version			= 1;
    lp->mode			= LOGGING_DISABLED;
    lp->numcmd			= 0;
    lp->numcl			= 0;
    lp->numopl			= 0;
    lp->numnot			= 0;
    lp->defspec			= 0;	/* caller must fill this in later */
    lp->alloc_code		= 0;
    lp->envptr			= 0;	/* caller must fill this in later */
    lp->pres			= 0;	/* caller must fill this in later */
    lp->next			= 0;	/* caller must fill this in later */
    return(1);				/* always succeeds */
}

int
log_command_now (
	struct logblk	*lp,		/* ptr to logging control block */
	struct cmdblk	*cbp)		/* ptr to command block */
{
    int ignore;

    ignore =
	fprintf(
	    lp->pres->file_ptr,		/* write to the log file */
	    "%s\n",			/* the cmd might have % in it */
	    cbp->cmdbuf);		/* the entire command */
    return(1);				/* claim success */
}

/* No logging for CDS */
int
log_command (
	struct logblk	*lp,		/* ptr to logging control block */
	struct cmdblk	*cbp,		/* ptr to command block */
	struct op_blk	*obp)		/* ptr to output list */
{
    lp->numnot++;			/* count commands not logged */
    return(DNS_SUCCESS);		/* claim success */
}


int
log_output_now (
	struct logparmblk	*lpp,	/* ptr to logging parameters */
	struct op_blk		*cop,	/* ptr to current output block */
	unsigned char		*dp)	/* ptr to data to be written */
{
    /* here is where to add options for formatting the line to be logged */
    /* it is ok to assume that the log file is the last file to be written */
    /* thus it is ok to change the data */
    /* use the format field of the logblk structure for format options */
    char temp[10000];

    (void)strcpy(&temp[1], (char *)dp);
    temp[0] = '!';			/* insert comment character */
    emit_one_copy_of_line(cop, temp);	/* write to log file */
    return(DNS_ERROR);			/* tell output routine don't write */
}

int					/* 1=write, 0=I did it for you */
log_output (
    void *data_p,			/* ptr to logging parameters */
    struct op_blk *cop,			/* ptr to current output block */
    unsigned char *dp)			/* ptr to data to be written */
{
    struct logparmblk *lpp = data_p;	/* ptr to logging parameters */
    struct logblk *lp;
    int return_code;			/* what we tell the caller */

    return_code = 0;			/* caller will probably not write */
    lp = lpp->lp;			/* point to log control block */
    if (lp->mode == LOGGING_ENABLED)
	return_code = log_output_now(lpp, cop, dp);
    return(return_code);		/* tell caller to write or not */
}
