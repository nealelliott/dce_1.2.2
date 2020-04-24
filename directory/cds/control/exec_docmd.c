/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: exec_docmd.c,v $
 * Revision 1.1.8.2  1996/02/18  19:29:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:55  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:09:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:29  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:00:50  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:49:59  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:32  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:15:24  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:36:51  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:37  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:59:00  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:01:07  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:05:09  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:35:59  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/01  15:57:49  m_sawyer
 * 	Remove code that appends "." to filenames without an extension.
 * 	[1992/06/01  15:55:27  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:48:11  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:21:37  weisman]
 * 
 * Revision 1.1  1992/01/19  15:21:21  devrcs
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
 */

#include <cdscp.h>
#include <cmdblk.h>

#if !defined(__alpha)
FILE *fopen(const char *,const char *);	/* to use fopen without a cast */
#endif

int					/* 1 if it works, 0 otherwise */
exec_docmd (				/* execute an indirect command file */
    char *spec,				/* ptr to name of file */
    struct entblk *elp,			/* ptr to entity list */
    struct attrblk *alp,		/* ptr to attribute list */
    struct envblk *ebp,			/* ptr to environment block */
    struct cmdblk *cbp,			/* ptr to command block */
    struct op_blk *obp)			/* ptr to output block */
{
    int status = DNS_ERROR;             /* init stat as haven't succeeded yet */
    FILE *newfile;			/* ptr to FILE structure */
    struct cmdblk *newcbp;		/* ptr to new command block */
    static char r_access[] = "r";	/* to read the file */
#define max_file 	256
#ifdef VMS
#define	separator  	']'
#else /* not VMS */
#define separator  	'/'
#endif /* not VMS */

    if (*spec)				/* check for most likely problem 1st */
	newfile = fopen((char *)spec, r_access);/* open the new file */
    else
	newfile = stdin;		/* or use the terminal */
    if (newfile)			/* can we use the file ? */
	{				/* yes, the open was successful */
	newcbp =			/* get space for new command block */
	    (struct cmdblk *)
		malloc(sizeof(struct cmdblk));
	if (newcbp)			/* did malloc work ? */
	    {				/* yes, malloc got enough space */
	    cmdblk_init(		/* initialize the command block */
		newcbp,			/* ptr to block to init */
		cbp);			/* ptr to block to copy from */
	    cbp->nxtlvl = newcbp;	/* link old cmdblk to new */
	    newcbp->ipfile = newfile;	/* use new file as source of cmds */
	    if (*spec)
		newcbp->mode = BATCH;	/* from file, so not interactive */
	    else
		newcbp->mode = INTERACTIVE;	/* from terminal, stdin */
	    ebp->curcmd = newcbp;	/* tell env blk where to find new */
	    process_commands(		/* execute commands from file */
		newcbp,			/* new command block */
		obp,			/* same output list */
		elp,			/* same entity list */
		alp,			/* same attributes list */
		ebp->log);		/* same logging info */
	    ebp->curcmd = cbp;		/* restore environment block */
	    cbp->nxtlvl = cbp;		/* unlink new cmd block from prev */
	    free(newcbp);		/* give back the new space */
	    status = DNS_SUCCESS;	/* all is well */
	    }
	else
	    {				/* malloc failed */
	    emit_line_fmt(obp, CDSCP_NOMEM_DOFILE,
		"No memory for processing command file %s\n", spec);
	    }
	if (*spec)
	    fclose(newfile);		/* close the file */
	}
    else				/* the open failed */
	{				/* report the problem */
	emit_line_fmt(obp,
	    CDSCP_CANT_OPEN, "can't open file %s \n", spec);
	}
    return ( status );
}




