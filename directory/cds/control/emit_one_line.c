/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: emit_one_line.c,v $
 * Revision 1.1.7.2  1996/02/18  19:28:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:39  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:08:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:17  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:40  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:42:07  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:43  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:25  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:37  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:59:28  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:03:47  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:34:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:09  devrcs
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

/* each of the routines in this module emit one line to the outputs. */
/* the various routines provide for various numbers and kinds of parameters. */
/* there are provisions for message skeletons not found. */
/* there is no status checking after any write operation */
/* the operation is not portable, and there is n effective test. */
/* rather than trying to fake it, we just ignore it. */

/* the flag parameter is common to all the routines */
/* 0 = use the same message file for message missing messages */
/* 1 = use the in memory message missing message */

/* several routines have names like emit_line_nxxx_parm. */
/* here is what the nxxx part means. */
/* the number, n, is the number of parameters. */
/* there will be exactly n following letters. */
/* s means the corresponding parameter will be the address of a string */
/* d means the corresponding parameter will be a number. */

#define MAX_OP_LINE 9999		/* move this to op_list_defs.h */

#include <stdarg.h>

void
emit_line_fmt (
    struct op_blk *obp,
    unsigned32 num,
    char *txt,
    ...)
{
    va_list args;
    char the_line[MAX_OP_LINE];
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    va_start(args, txt);

    if (msg_txt)
    {
	vsprintf(the_line, msg_txt, args);
	free(msg_txt);
    }
    else
	vsprintf(the_line, txt, args);

    va_end(args);
    emit_line(obp, the_line);
}

void
emit_hdr (
    struct op_blk *obp,		/* where to write it */
    unsigned32 num,
    char *txt)
{
    char the_line[MAX_OP_LINE];
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    if (msg_txt)
    {
	sprintf(the_line, "%28.31s\n", msg_txt);
	free(msg_txt);
    }
    else
	sprintf(the_line, "%28.31s\n", txt);

    emit_line(obp, the_line);
}

void
emit_hdr_string (
    struct op_blk *obp,		/* where to write it */
    unsigned32 num,
    char *txt,
    char *string)
{
    char the_line[MAX_OP_LINE];
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    if (msg_txt)
    {
	sprintf(the_line, "%28.31s   %s\n", msg_txt, string);
	free(msg_txt);
    }
    else
	sprintf(the_line, "%28.31s   %s\n", txt, string);

    emit_line(obp, the_line);
}

void
emit_name_string (
    struct op_blk *obp,		/* where to write it */
    char *name,
    char *string)
{
    char the_line[MAX_OP_LINE];

    sprintf(the_line, "%28.31s = %s\n", name, string);

    emit_line(obp, the_line);
}

void
emit_attr_string (
    struct op_blk *obp,		/* where to write it */
    unsigned32 num,
    char *txt,
    char *string)
{
    char the_line[MAX_OP_LINE];
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    if (msg_txt)
    {
	sprintf(the_line, "%28.31s = %s\n", msg_txt, string);
	free(msg_txt);
    }
    else
	sprintf(the_line, "%28.31s = %s\n", txt, string);

    emit_line(obp, the_line);
}

void
emit_attr_number (
    struct op_blk *obp,		/* where to write it */
    unsigned32 num,
    char *txt,
    int number)
{
    char the_line[MAX_OP_LINE];
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    if (msg_txt)
    {
	sprintf(the_line, "%28.31s = %d\n", msg_txt, number);
	free(msg_txt);
    }
    else
	sprintf(the_line, "%28.31s = %d\n", txt, number);

    emit_line(obp, the_line);
}
