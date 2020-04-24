/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: preferred_clh.c,v $
 * Revision 1.1.7.2  1996/02/18  19:30:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:40  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:11:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:59  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:17  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:44:17  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:38  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:39  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:42  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:05:59  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:08:48  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:40:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
#include <stdio.h>
#include <prsdef.h>

static dns_opq_fname	pfd_clh;	/* the full name of the preferred clh */
static struct entblk    cur_pfd_clh;	/* the current preferred clearinghouse*/

static dns_opq_fname    temp_name;	/* full name from the parser */
static dns_opq_fname *  temp_ptr;	/* 0 or ptr to previous */

dns_opq_fname *
preferred_clh_pointer (void)		/* find the preferred clearinghouse */
{
    return((dns_opq_fname *)cur_pfd_clh.name_p); /* pointer to name, or 0 */
}

int
show_preferred_clh (
    struct envblk   *envbp,/*ARGSUSED*/	/* ptr to environment block */
    struct cmdblk   *cmdbp,/*ARGSUSED*/	/* ptr to current command block */
    struct op_blk   *outbp)		/* ptr to head of output list */
{
    char namestring[DNS_STR_FNAME_MAX];	/* string form of default name */

    if (cur_pfd_clh.name_p)		/* is there a preferred clh ? */
	{				/* yes */
	format_fullname_for_print(
	    (FullName_t *)&pfd_clh,
	    namestring);		/* resulting string form */

	emit_line_fmt(outbp, CDSCP_MSG_PFD_CLH,
		"read attribute values from clearinghouse %s\n", namestring);
	}
    else
	{				/* no preferred clearinghouse now */
	emit_line_fmt(outbp, CDSCP_MSG_NOPFD_CLH,
		"no preferred clearinghouse\n");
	}
    return(DNS_SUCCESS);		/* report success status to caller */
}

int
set_preferred_clh (			/* remember what the parser saved */
    struct envblk   *envbp,/*ARGSUSED*/	/* ptr to environment block */
    struct cmdblk   *cmdbp,/*ARGSUSED*/	/* ptr to current command block */
    struct op_blk   *outbp)/*ARGSUSED*/	/* ptr to head of output list */
{
    if (temp_ptr)			/* clearinghouse name supplied */
	{				/* yes. so save it */
	memcpy(&pfd_clh, temp_ptr, DNS_OPQ_FNAME_MAX);
	cur_pfd_clh.name_p = 
	    (unsigned char *)&pfd_clh;	/* point to the saved name */
	}
    else
	{				/* use any clearinghouse */
	cur_pfd_clh.name_p = 
	    (unsigned char *)0;		/* remember by null pointer */
	}
    return(DNS_SUCCESS);		/* always report success */
}

int
save_pfd_clh (				/* parser action routine for set */
    struct prs *pcbp)			/* ptr to parser context block */
{
    if (pcbp->av1)			/* clearinghouse name supplied */
	{				/* yes. so save it */
	memcpy(&temp_name, name_pointer(), DNS_OPQ_FNAME_MAX);
	temp_ptr = &temp_name;		/* point to the saved name */
	}				/* in case parse fails later */
    else
	{				/* use any clearinghouse */
	temp_ptr = (dns_opq_fname *)0;	/* remember by null pointer */
	}
    return(ACT_SUCCESS);		/* always report success */
}
