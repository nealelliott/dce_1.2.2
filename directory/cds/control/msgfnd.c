/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgfnd.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:29  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:10:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:47  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:11  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:22  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:30  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:27  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:23  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:05:06  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:08:02  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:39:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Library MSG, Module MSGFND - Operate on in-memory message text.
 *
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
 * MODULE DESCRIPTION:
 *
 * Library MSG,  Module MSGFND
 *
 * Get an entire MSG file into memory.
 * Get a message from an in-memory message file.
 *
 *
 */
static char ver[] = "<< MOD=[MSGFND] VER=[1.06] COM=[Find a message] >>";

/*
 * General declarations.
 */

#include <cdscp.h>
#include <stdarg.h>

void
cds_printf (unsigned32 num, char *txt, ...)
{
    va_list args;
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    va_start(args, txt);

    if (msg_txt)
    {
	vprintf(msg_txt, args);
	free(msg_txt);
    }
    else
	vprintf(txt, args);

    va_end(args);
}

void
cds_fprintf (FILE *stream, unsigned32 num, char *txt, ...)
{
    va_list args;
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    va_start(args, txt);

    if (msg_txt)
    {
	vfprintf(stream, msg_txt, args);
	free(msg_txt);
    }
    else
	vfprintf(stream, txt, args);

    va_end(args);
}

void
cds_sprintf (char *buf, unsigned32 num, char *txt, ...)
{
    va_list args;
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    va_start(args, txt);

    if (msg_txt)
    {
	vsprintf(buf, msg_txt, args);
	free(msg_txt);
    }
    else
	vsprintf(buf, txt, args);

    va_end(args);
}

void
cds_fputs (
    unsigned32 num,
    char *txt,
    FILE *stream)
{
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    if (msg_txt)
    {
	fputs(msg_txt, stream);
	free(msg_txt);
    }
    else
	fputs(txt, stream);
}

void
cds_strcpy (
    char *buf,
    unsigned32 num,
    char *txt)
{
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    if (msg_txt)
    {
	strcpy(buf, msg_txt);
	free(msg_txt);
    }
    else
	strcpy(buf, txt);
}

void
cds_strcat (
    char *buf,
    unsigned32 num,
    char *txt)
{
    unsigned32 msg_status;
    char *msg_txt = (char *)dce_msg_get_msg(num, &msg_status);

    if (msg_txt)
    {
	strcat(buf, msg_txt);
	free(msg_txt);
    }
    else
	strcat(buf, txt);
}
