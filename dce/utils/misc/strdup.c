/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: strdup.c,v $
 * Revision 1.1.4.2  1996/02/18  19:22:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:17  root]
 * 
 * Revision 1.1.2.3  1994/08/10  19:21:28  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:15:45  annie]
 * 
 * 	expand copyright with OSF copyright text
 * 
 * 	expand copyright with OSF copyright text
 * 
 * Revision 1.1.2.2  1994/07/27  17:12:54  rsalz
 * 	Use dce_strdup (OT CR 11341) and do code-cleanup.
 * 	[1994/07/27  17:03:43  rsalz]
 * 
 * Revision 1.1.2.1  1994/07/18  14:50:31  rsalz
 * 	Add dce_strdup (OT CR 11341).
 * 	[1994/07/18  14:49:08  rsalz]
 * 
 * $EndLog$
 */
#include <stdlib.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dce/dce_utils.h>


char *
dce_strdup(
    char	*p
)
{
    char	*new;

    if ((new = malloc(strlen(p) + 1)) != NULL)
	strcpy(new, p);
    return new;
}
