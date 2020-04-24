/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: confidence.c,v $
 * Revision 1.1.7.2  1996/02/18  19:27:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:59  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:06:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:28  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:16  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:36:25  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:08:26  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:13  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:40  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:56:30  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:01:31  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:11  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:46  devrcs
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
/* routines to use the confidence value */

#include <cdscp.h>
#include <prsdef.h>

static int confidence;

int *
get_conf_ptr(void)
    {
    return(&confidence);
    }

int
get_conf(void)
    {
    return(confidence);
    }

int
saveconf(				/* parser action routine to save conf */
    struct prs *pcbp)			/* ptr to parser context block */

    {
    confidence = pcbp->av1;		/* remember the confidence value */
    return(ACT_SUCCESS);
    }

dns_status_t
set_conf(				/* execute the set confidence cmd */
    struct envblk *ebp,			/* ptr to environment block */
    struct cmdblk *cbp,			/* ptr to command block */
    struct op_blk *obp)			/* ptr to head of output list */

    {
    ebp->curconf = confidence;		/* keep the value until changed again */
    return(DNS_SUCCESS);		/* report success */
    }					/* no output, so valid preps ignored */

dns_status_t
show_conf(				/* execute the show confidence cmd */
    struct envblk *ebp,			/* ptr to environment block */
    struct cmdblk *cbp,			/* ptr to command block */
    struct op_blk *obp)			/* ptr to head of output list */

    {
    char valuebuf[100];			/* big enough to hold one word */

    format_confidence_for_print(	/* format the right side of the line */
	valuebuf,			/* ptr to output buffer */
	ebp->curconf);			/* the confidence code */

    emit_line_fmt(obp, CDSCP_SHO_CONF, "Confidence used is %s\n", valuebuf);
    return(DNS_SUCCESS);		/* report success */
    }
