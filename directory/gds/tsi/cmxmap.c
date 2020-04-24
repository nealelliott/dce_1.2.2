/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxmap.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:25  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:48  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:28  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:35  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:18  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:30  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:44  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:29  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:22  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxmap.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:25 $";
#endif

/*
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1992
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/*****************************************************************************/
/*                                                                           */
/*        ICMX(L) MAPPING OF LOCAL NAME AND TRANSPORT ADDRESS                */
/*        ===================================================                */
/*                                                                           */
/*     The functions herein can be replaced by any CMX application program   */
/*  as long as the syntactical requirements are met. These functions are     */
/*  called with the LOCAL NAME and/or TRANSPORT ADDRESS arguments passed to  */
/*  the CMX calls t_attach(), t_detach and t_conrq(). The call occurs prior  */
/*  to ANY operation on the arguments LOCAL NAME and/or TRANSPORT ADDRESS    */
/*  (not even storing it into some trace facility [what length?]).           */
/*  These functions return a pointer to a properly formatted LOCAL NAME as   */
/*  defined in the Transport Name Service. Any check of syntax or semantic   */
/*  operates on the information pointed to by the pointer returned.          */
/*  The information is never again accessed, once the calling t_attach(),    */
/*  t_detach(), t_conrq() function has returned.			     */
/*  The information, where the arguments points to upon entry must not be    */
/*  modified.								     */
/*									     */
/* Author: rit                                           Date:    21.05.91   */
/* Corrections:  kam                                              31.03.92   */
/*****************************************************************************/

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include	"cmx.h"
/*
static char	sccsid[] = "@(#)cmxmap.c	311.3 93/08/24 CMX (NC1)";
*/
/*
 * Prototyping
 */
char *t_mapname(char *name);
union t_address *t_mapaddr(union t_address *toaddr, union t_address *fromaddr);

/*------------------------------------------------------------------------*
 * T-MAPNAME
 *------------------------------------------------------------------------*/

/*
 * t_mapname - map to LOCAL NAME
 * The function is called with a pointer to a byte string
 * that is to be mapped.
 * This function is expected to return a pointer to a LOCAL NAME as
 * defined in CMX by the Transport Name Service or the NULL pointer in case
 * no such mapping is possible.
 * This function call may be replaced by a user function providing this
 * interface.
 * The code here serves as default, if no such user function is provided,
 * to satisfy the external reference from within the CMX library.
 */
char *t_mapname(char *name)
{
	return (name);
}

/*------------------------------------------------------------------------*
 * T-MAPADDR
 *------------------------------------------------------------------------*/

/*
 * t_mapaddr - map to TRANSPORT ADDRESS
 * The function is called with two pointers to byte strings
 * to be mapped.
 * This function is expected to return a pointer to a TRANSPORT ADDRESS as
 * defined in CMX by the Transport Name Service or the NULL pointer in case
 * no such mapping is possible.
 * This function call may be replaced by a user function providing this
 * interface.
 * The code here serves as default, if no such user function is provided,
 * to satisfy the external reference from t_attach() and t_detach().
 */
union t_address *t_mapaddr(union t_address *toaddr, union t_address *fromaddr)
{
	return (toaddr);
}
