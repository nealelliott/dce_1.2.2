/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsxtimap.c,v $
 * Revision 1.1.4.2  1996/02/18  18:20:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:05:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:00  root]
 * 
 * Revision 1.1.2.4  1994/09/06  12:25:45  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:01  keutel]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:36  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:43  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:01  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:30:06  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:42:01  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:19:17  keutel
 * 	creation
 * 	[1994/03/21  15:53:25  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsxtimap.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:20:07 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    ndsxtimap.c                                                    */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all the functions used to map global OSI TSAP- */
/*       address information into the local address formats defined by the   */
/*       supported XTI-transport provider. This module must be modified      */
/*       resp. rewritten if there are changes in existing local address      */
/*       formats or if new local address formats are introduced.             */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_map_ositoxti   - mapping entry call function               */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <sys/types.h>
#include <xti.h>
#include <ndsif.h>
#include <ndscfg.h>

/* -------------------------------------------------------------------------- */

signed32 nds_map_ositoxti (signed32	 profid,     /* >  profile-ID */
			   struct netbuf *from,	     /* >< from TSAP-address */
			   struct netbuf *to,	     /* >< to TSAP-address */
			   Nds_options   *optref,    /* >  specific options info */
			   Nds_options   *coptref) { /* >  common options info */

    /* XTI-interface is not supported */
    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_XTI_ERR) ;
    return (NDS_NOSELERR) ;
}
