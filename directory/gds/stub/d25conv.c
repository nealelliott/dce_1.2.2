/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25conv.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:07  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:15:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:40  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:26:24  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:11  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  16:03:22  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:07  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:55:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:50:48  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  08:21:06  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:18:01  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:03:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:47:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:08:30  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:30:03  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25conv.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:10 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

#include <gds.h>
#include <ros.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d25stub.h>

			/* function name to index conversion table */
			/* (used for logging purposes only) */
signed32 (*func[])(D25_actionpb *) = {
	d25_rosattach, d25_rosdetach, d25_rosbinrq,  d25_robinrs,
	d25_rosubnrq,  d25_roubnrs,   d25_rosinvrq,  d25_rosresrq,
	d25_roerrrq,   d25_rorjurq,   d25_rosaborq,  d25_rosprbf,
	d25_rostrrq,   d25_ipcsvratt, d25_ipsvrdet,  d25_ipccltatt,
	d25_ipcltdet,  d25_ipcbind,   d25_ipcunbind, d25_ipcinvoke,
	d25_ipcwaitin, d25_ipcresult, d25_ipwaitrs,  d25_ipretbuf,
	d25_ipcabort,  d25_getpsap,   d25_getres,    d25_relres,
	d25_relbuf,    d25_checkres,  d25_regproc,   d25_unregproc
#ifdef CSTUB
      , d25_initasn1,  d25_apdutoasn1, d25_asn1toapdu,
	d25_101_save_invoke_data,      d25_111_remove_cup_job,
	d25_121_save_result_data,      d25_141_restore_env,
	d25_151_continue_update
#endif
	} ;


/* ------------- convert function address into an index -------------------- */

signed32 d25_cvfname (signed32 (*funcptr)(D25_actionpb *)) {

    signed32	i ;
    signed32	(**fptr)(D25_actionpb *) ;

    for (i = 0, fptr = func;
	   i < sizeof (func)/sizeof (signed32 (*)(D25_actionpb *)); i++, fptr++)
	if (*fptr == funcptr)
	    break ;
    return ((i == sizeof (func)/sizeof (signed32 (*)(D25_actionpb *))) ?
							       D25_UNUSED : i) ;
}
