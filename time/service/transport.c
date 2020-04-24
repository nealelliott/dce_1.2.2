/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: transport.c,v $
 * Revision 1.1.7.2  1996/02/17  23:35:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:12  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:14:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:26:22  root]
 * 
 * Revision 1.1.5.3  1994/05/16  18:52:07  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:42  rhw]
 * 
 * Revision 1.1.5.2  1994/04/21  22:06:21  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:13  cbrooks]
 * 
 * Revision 1.1.5.1  1994/01/25  17:16:57  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:55  rsalz]
 * 
 * Revision 1.1.3.3  1992/12/30  16:40:25  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:35:42  htf]
 * 
 * Revision 1.1.3.2  1992/09/29  20:44:26  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:07:47  sekhar]
 * 
 * Revision 1.1  1992/01/19  15:34:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	Module: TRANSPORT.C
 */
/*
 * Copyright (c) 1989, 1990, 1991 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:	General transport routines. These routines switch on
 *              the transport type and then call os-specific routines
 *              for the individual transports.
 *
 * ENVIRONMENT:	Portable
 *
 * AUTHORS:
 *		Wick Nichols
 *		Distributed Systems Services (DSS)
 *
 * DATE:	November 1989
 */

/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */

#include <string.h>

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>

/*
 *	Global variables
 */

Que portQhd;	/* Port Queue header, declared external in other modules */

int transports_initialized = 0;

/*
 * Externals
 */

extern ShmBuf *shmPtr;		/* pointer to shared memory */

extern PortDsc  *decnetPortPtr;	/* init'd in port init*/

/*
 * Local procedure prototypes
 */
/*
 *
 * InitTransport() -
 *
 * Description:
 *
 *	Initialize the generic transport module.
 *
 * Inputs:
 *
 *	none
 *
 * Implicit Inputs:
 *
 *	none
 *
 * Outputs:
 *
 *	none
 *
 * Implicit Outputs:
 *
 *	inits the multicast address global variables.
 *
 *	transports_initialized gets set to 1.
 *
 * Return Value in StatusBlk.status:
 *
 *	DTSS_SUCCESS - always
 *
 */

StatusBlk InitTransport (void)
{
    StatusBlk statusBlk;

    /*
     * If we've initialized the transport, then just return.
     */
    if (transports_initialized) {
	statusBlk.status = DTSS_SUCCESS;
	return (statusBlk);
    }

    /*
     * Initialize the port queue
     */
    Qinit (&portQhd);

    /*
     * Initialize the RPC port
     */
    statusBlk = InitRPCport ();
    if (IsError (statusBlk)) {
	StatusBlk	s;
	char	msgBuf [K_MAX_OSMSG+1];

	s = GetOsMsg (statusBlk.sysStatus, msgBuf, sizeof msgBuf);
	if (IsError (s))
	    (void)sprintf (msgBuf, "%ld", statusBlk.sysStatus);
	fprintf (stderr, "InitRPC error (%s)\n", msgBuf);
    }

    transports_initialized = 1;

    statusBlk.status = DTSS_SUCCESS;
    return (statusBlk);
}



/*
 *++
 *  DisconnectLink (portPtr, linkIdPtr)
 *	Disconnect the WAN link used on this port.
 *      If called for a connectionless socket, just return.
 *  Inputs:
 *	portPtr - pointer to port dsc, indicates protocol type and port
 *      linkIdPtr - pointer to channel number for link to be closed
 *  Outputs:
 *     zero's caller's linkId
 *  Side Effects:
 *	disconnects connectionful links
 *	zeros caller's link id
 *--
 */
void DisconnectLink (
    PortDsc  *portPtr,
    int      *linkIdPtr
)
{

    if (linkIdPtr && *linkIdPtr >= 0) {
	/*
	 * Nothing to do for connectionless transports
	 */
	*linkIdPtr = -1;
    }
}
