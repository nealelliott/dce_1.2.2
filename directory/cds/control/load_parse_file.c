/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: load_parse_file.c,v $
 * Revision 1.1.8.2  1996/02/18  19:29:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:23  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:10:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:44  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:01:07  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:34  mccann]
 * 
 * Revision 1.1.6.4  1994/06/23  18:29:46  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:17:42  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:37:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:21  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/04  20:06:11  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:07:52  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:00:10  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:04:21  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:07:23  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:38:50  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:50:36  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:22:20  weisman]
 * 
 * Revision 1.1  1992/01/19  15:21:47  devrcs
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
#ifndef vms
#include <unistd.h>
#endif
#include <prsdef.h>

/* These are all fatal errors */
static void
load_failure (
	struct prs	*p,		/* ptr to parse block */
	char		*f,		/* ptr to file name of parse table */
	int		result)		/* status from attempted load */
{
    struct op_blk *const obp = get_op_blk_ptr();

    /* rather than pass in extra parameters or insisting on the */
    /* expected set of pointers between the various control blocks, */
    /* we find the output list and the message block only if we need them. */
    /* the routines to find them are in a separate module. */
    /* you can write your own routines if your data structures are different. */

    switch (result)
	{
	case RET_SIZE:
	    {
	    emit_line_fmt(obp, CDSCP_NO_FIT,
		"%d byte file %s will not fit in %d byte buffer\n",
		p->tbu, f, p->tbs);
	    break;
	    }
	case RET_VERSION:
	    {
	    dce_svc_printf(CDSCP_PARSE_VERSION_MSG, f);
	    break;
	    }
	case RET_OPENERROR:
	    {
	    dce_svc_printf(CDSCP_CANT_OPEN_PARSE_TABLE_MSG, f);
	    break;
	    }
	case  RET_IOERROR:
	    {
	    dce_svc_printf(CDSCP_READ_ERROR_MSG, f);
	    break;
	    }
	default:
	    {
	    dce_svc_printf(CDSCP_LOAD_FAILURE_MSG, result);
	    break;
	    }
	}
}

/* try to load the parse table file into memory */
/* return true if it works, and false otherwise */

void
load_parse_file (
	struct prs	*p,		/* ptr to parse control block */
	char		*fspec,		/* ptr to file spec */
	int		code)		/* 1 to ... */
{
    int  status;			/* DNS_SUCCESS if ok, else DNS_ERROR */
    int	 result;			/* status from the msg routine */
    char tiny_buffer[1];		/* let read fail, discover size needed*/
    char *f;				/* ptr to filespec to use */
    struct op_blk *const obp = get_op_blk_ptr();

    status = DNS_SUCCESS;		/* assume it will work. */
    f = fopen2aux2(fspec);		/* substitute if need be */

    if (p->tbl == NULL)			/* no buffer speced, we pick size&loc*/
	{				/* caller did not select the buffer */
	p->tbl = &tiny_buffer[0];	/* use one we expect to fail */
	p->tbs = sizeof(tiny_buffer);	/* because it is too small */
	result = pr_read(p, f);		/* try to read it */
	if (result == RET_SIZE)		/* means too small */
	    {				/* size needed is in blk.tbu */
	    p->tbl = (char *)malloc(p->tbu);	/* get a big enough buffer */
	    if (p->tbl == NULL)		/* did the malloc work ok? */
		{			/* no. should never happen */
		  emit_line_fmt(obp, CDSCP_NO_FILE_MEM,
		  "no memory for %d byte file %s\n", p->tbu, f);
		status = DNS_ERROR;		/* failure */
		}
	    else
		{			/* yes, the malloc worked. */
		p->tbs = p->tbu;	/* remember size & location */
		}			/* all set to try again */
	    }	
	else
	    {				/* tiny buffer insures some failure */
	    load_failure(p, f, result);	/* report the problem */
	    status = DNS_ERROR;			/* failure */
	    }
	}

    /* to here in all cases. */
    /* if no problems or only expected "problems" status is still DNS_SUCCESS */
    /* if unexpected problems, status is DNS_ERROR */
    if (status == DNS_SUCCESS)
	{				/* ok to continue */
	/* next is first read of the parse table file, */
	/* if user supplied the buffer. */
	/* it is second read if we used first read to get buffersize needed */
	result = pr_read(p, f);
	if (result != RET_SUCCESS)
	    {
	    /* failures are logically impossible if we computed buffer size */
	    /* anything can happen if this is the first read. */
	    load_failure(p, f, result);		/* report the problem */
	    status = DNS_ERROR;			/* failure */
	    }
	/* else it worked. table safely in memory */
	}				/* end of if (status==DNS_SUCCESS) */

    /* all paths through this routine end here */
    fopen2aux3(f);		/* return mem for spec of used file */
}
