/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtsscp_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:34:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:51  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:08:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:14  root]
 * 
 * Revision 1.1.2.3  1994/08/23  20:32:08  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:42  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:29:06  cbrooks]
 * 
 * Revision 1.1.2.2  1994/05/16  18:51:41  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:12  rhw]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:22  cbrooks
 * 	CR 8353, 3231
 * 	[1994/04/21  19:37:50  cbrooks]
 * 
 * Revision 1.1.2.2  1992/12/30  16:35:14  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:12:10  htf]
 * 
 * Revision 1.1  1992/01/19  15:35:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	Module:	DTSSCP_AUX.C
 */
/*
 ****************************************************************************
 *									    *
 *  COPYRIGHT (c) 1988,1989,1990,1991 BY				    *
 *  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASSACHUSETTS.		    *
 *  ALL RIGHTS RESERVED.						    *
 * 									    *
 *  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND COPIED   *
 *  ONLY IN  ACCORDANCE WITH  THE  TERMS  OF  SUCH  LICENSE  AND WITH THE   *
 *  INCLUSION OF THE ABOVE COPYRIGHT NOTICE. THIS SOFTWARE OR  ANY  OTHER   *
 *  COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY   *
 *  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE IS  HEREBY   *
 *  TRANSFERRED.							    *
 * 									    *
 *  THE INFORMATION IN THIS SOFTWARE IS  SUBJECT TO CHANGE WITHOUT NOTICE   *
 *  AND  SHOULD  NOT  BE  CONSTRUED AS  A COMMITMENT BY DIGITAL EQUIPMENT   *
 *  CORPORATION.							    *
 * 									    *
 *  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE  OR  RELIABILITY OF ITS   *
 *  SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.		    *
 * 									    *
 *									    *
 ****************************************************************************
 */
/*
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *
 *	This module contains the POSIX-specific pieces of the control
 *	program for DTSS.
 *
 * ENVIRONMENT: 	POSIX (ANSI-C, POSIX)	
 *
 * AUTHOR:	        K.P. Baryiames
 *
 * CREATION DATE: 	July, 1990
 *
 * Modified: 2/18/94 CBrooks for DCE 1.1 
 * moved from dtsscp_ultrix.c to dtsscp_aux.c in the POSIX subdirectory
 *
 */


#include <unistd.h>
#include <string.h>
#include <dts.h>
#include <dtss_includes.h>

void FindPrsBinFile( char *) ;


/*
 * FindPrsBinFile()
 * 
 * Find parse binary file and return its complete name in the given 
 * buffer.
 *
 * Assumes buffer is size K_MAX_FILENAME+1
 *
 */
void FindPrsBinFile (char	*bufPtr)
{

    /*
     * See if we can find the parse file.  If this is a DEBUG version, look for
     * it in "/tmp/_dtsscp.bpt". If we can't find one, or this is a production
     * version, then just return the name of the installed bin file, which
     * resides in "/usr/etc/dtsscp.bpt". If the installed bin file doesn't
     * exist we'll catch the error when pr_read() tries to open the file.
     *
     * access (file_spec, F_OK) returns 0 if the file exists, else EOF.
     */

#ifdef DCE_DEBUG
    char *TestPrsBinFile = "/tmp/_dtsscp.bpt";

    if (access (TestPrsBinFile, F_OK) == 0)
	(void) strcpy (bufPtr, TestPrsBinFile);
    else
    {
	(void) strncpy(bufPtr, dcelocal_path, K_MAX_FILENAME);
        (void) strncat(bufPtr, K_PRSFILE_NAME, K_MAX_FILENAME);
    }
#else
    (void) strncpy(bufPtr, dcelocal_path, K_MAX_FILENAME);
    (void) strncat(bufPtr, K_PRSFILE_NAME, K_MAX_FILENAME);
#endif

}
