/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isdcepcs.c,v $
 * Revision 1.1.4.2  1996/02/18  19:22:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:12  root]
 * 
 * Revision 1.1.2.3  1994/09/16  17:33:28  bowe
 * 	Check for negative values in isdcepcs(). [CR 12123]
 * 	[1994/09/16  17:32:44  bowe]
 * 
 * Revision 1.1.2.2  1994/07/27  17:12:53  rsalz
 * 	Use dce_strdup (OT CR 11341) and do code-cleanup.
 * 	[1994/07/27  17:03:33  rsalz]
 * 
 * Revision 1.1.2.1  1994/06/29  13:15:06  melman
 * 	made threadsafe, removed memset of static array
 * 	[1994/06/29  13:14:21  melman]
 * 
 * 	style suggestions from John Bowe
 * 	[1994/06/29  12:14:44  melman]
 * 
 * 	Implemented isdcepcs()
 * 	[1994/06/28  22:39:12  melman]
 * 
 * $EndLog$
 */

#include <dce/dce.h>
#include <dce/dce_utils.h>
#define NCHARS 256

/* replacement for isascii */
int isdcepcs(int c)
{
    static char array[NCHARS];          /* init'ed to zeros */
    static boolean initialized=FALSE;
    char *p;
    
    /* easy check if out of bounds */
    if (c > (NCHARS-1) || c < 0) return FALSE;

    /* PCS as found in Appendix G of RPC AES */
    p=PCS_CHARS;
    
    if (!initialized) {
	for ( ; *p; p++)
	    array[(int)*p]=TRUE;
        initialized=TRUE;        
    }
    return array[c];
}
