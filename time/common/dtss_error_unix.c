/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_error_unix.c,v $
 * Revision 1.1.6.2  1996/02/17  23:34:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:40  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:06:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:46  root]
 * 
 * Revision 1.1.4.3  1994/06/24  19:33:59  cbrooks
 * 	CR9340 use of sys_nerr is not portable
 * 	[1994/06/22  14:31:00  cbrooks]
 * 
 * 	CR 9340 - sys_nerr is not portable
 * 	[1994/06/14  14:40:20  cbrooks]
 * 
 * Revision 1.1.4.2  1994/05/16  18:51:38  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:09  rhw]
 * 
 * Revision 1.1.4.1  1994/05/12  15:17:04  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:23  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:34:10  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:10:40  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *
 *	Module: DTSS_ERROR_UNIX.C
 */
/*
 ****************************************************************************
 *									    *
 *  COPYRIGHT (c) 1991 BY						    *
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
 *  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE  OR  RELIABILITY OF IT    *
 *  SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.		    *
 * 									    *
 *									    *
 ****************************************************************************
 */

/*
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *	This module contains the system and DCE error message handling routines
 *
 * ENVIRONMENT: 	OSF DCE
 *
 * AUTHOR:		Brian Bailey
 *
 * CREATION DATE: 	June 1991
 *
 */

#include <dts.h>
#include <dtss_includes.h>		/* DTSS top-level header file */
#include <string.h>			/* String routines */

extern int sys_nerr;                    /* Number of entries in error list */
extern char *sys_errlist[];            


/*
 *++
 *  GetOsMsg()
 *
 *  Functional Description:
 *  
 *    Get a Unix system or DCE error message.
 *  
 *  Inputs:
 *    error    - system or DCE error code.
 *    buffer_p - pointer to a buffer.
 *    blen     - length of said buffer which must be >= 1.
 *
 *  Implicit Inputs:
 *    None.
 *
 *  Outputs:
 *    buffer_p - contains error message.
 * 
 *  Implicit Outputs:
 *    None.
 *      
 *  Value Returned:
 *    StatusBlk 
 *
 *  Side Effects:
 *    None
 *--
 */
StatusBlk GetOsMsg ( 
   unsigned long	error,
   char		*buffer_p,
   int  		blen
)
{
    StatusBlk		statusBlk;
    dce_error_string_t 	dceMsg;
    int inqstatus = rpc_s_ok ;

    /* begin */

    /*
     * 6-14-94 CLBrooks  CR 9340
     * prior code used sys_nerr and sys_errlist[], which is not 
     * POSIX compatible. Hence, we change the code to check for 
     * a DCE error first; if the call to dce_error_inq_text() fails,
     * we then use strerror() to retrieve the (locale specific) error 
     * message, else null
     */

    dce_error_inq_text((error), dceMsg, &inqstatus);
    if (inqstatus == 0)		/* a valid DCE error message */
    {
	(void) strncpy (buffer_p, (char *)dceMsg, (size_t) blen);
	buffer_p[blen] = '\0';
	statusBlk.sysStatus = inqstatus;
    }
    else			/* treat as a system error  */
    {
	char * p = strerror(error) ;
	if (p == NULL)		/* not a system error either */
	{
	    statusBlk.status = DTSS_ERROR;
	    sprintf(buffer_p, "(%lu)\n", error) ;
	}
	else 
	{
	    strncpy( buffer_p, p, (size_t) blen);
	}
    }
    return(statusBlk);

} /* end of routine GetOSmsg */

/*
 *
 * PrintMsgToOperator () -
 *
 * Description:
 *
 *             Print the translated message to the network operator's console
 *
 * Inputs:
 *
 *	message : address of a character string
 *
 * Outputs:
 *
 *	none
 *
 * Return Value in statusBlk.status:  DTSS_ERROR or DTSS_SUCCESS
 *
 */

StatusBlk PrintMsgToOperator (
    char *message
)
{
    StatusBlk    statusBlk;
    int          ConsoleFD;
    int          AmntWritten;
    int          length;

    /* begin */

    if ((ConsoleFD = open ((char *)"/dev/console", O_WRONLY, 
			   (int)NULL )) == (-1))
    {
	statusBlk.status  = DTSS_ERROR;
        return(statusBlk);
    }
    else
    {
	length = strlen (message);
	AmntWritten = write (ConsoleFD, message, length);
	if (AmntWritten == -1)
	    statusBlk.status  = DTSS_ERROR;
	else
	    statusBlk.status  = DTSS_SUCCESS;
    }

    (void) close (ConsoleFD);
    return(statusBlk);
}

