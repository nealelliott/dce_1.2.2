/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscp_args.c,v $
 * Revision 1.1.6.2  1996/02/18  19:27:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:13  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:07:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:49  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:25  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:38:44  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:10  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:55  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:57:35  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:02:16  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:32:09  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:50  devrcs
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

/* processing the command line arguments for dnscp */

static int debugarg;
static char *startuparg;

int
debug_requested(void)
{
    return(debugarg);
}

char *
startup_file_requested(void)
{
    return(startuparg);
}

int
process_args(
    int argc,
    char *argv[])
{
    int skip;				/* number of args to skip to cmd */
    int i, j;				/* loop control */

    debugarg = 0;			/* no debug unless requested */
    startuparg = 0;			/* use default startup file unless.. */

    skip = 1;				/* bypass the command name */
    for (i=1; i<argc; i++)
	{
	if (argv[i][0] == '-')
	    {
	    if (argv[i][1] == 'd')
		{
		debugarg = 1;
		skip++;
		}
	    else if (argv[i][1] == 'f')
		{
		for (j=i+1; (j<=argc) && (argv[j][0] == 0); j++)
		    ;			/* find the next non-null arg */
		if (j<argc)
		    {
		    startuparg = argv[j];
		    skip += j + 1 - i;
		    }
		}
	    else			/* some other unexpected option */
		break;
	    }
	else				/* not a dash */
	    break;			/* so treat it as part of the command */
	}
    /* insert here a check that the chosen interface is really available. */
    /* if not, change interfacearg to something that is available, like 'c' */
    return(skip);			/* the rest is a dnscp command */
}					/* end of process_args */
