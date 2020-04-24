/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22distcmd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:25  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:54  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:31  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:13  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  07:29:27  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:08  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:18  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:53:16  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:32:46  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:13:49  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:31  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:29:27  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:08  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22distcmd.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:46 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d22distcmd.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 17.07.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d22_hdldistcmd ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function handles all incoming distributed commands.              */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      signo = No. of occurred signal                                       */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 17.7.89      */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <errno.h>
#include <dce/dcesvcmsg.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>


/* -------------------- declaration of global data ------------------------- */

				/* DUA-cache operation flag */
extern boolean 		d22_dboperation ;

				/* DUA-cache administration information table */
extern D22_admin_tab	d22_admtab ;

/* ------------------------------------------------------------------------- */

void d22_hdldistcmd (int signo) {

    D23_dreadpb		drdpb ;
    D23_dconfpb		dcfpb ;
    boolean		confirm = TRUE ;
    error_status_t	result ;

    /* verify distributed command state */
    drdpb.d23_Khdinfo.d2_version = D23_V02 ;
    if (d23_read_dist_cmd (&drdpb) == D23_ERROR)
	/* no distributed command available */
	return ;

    /* handle distributed command */
    switch (drdpb.d23_Kcmd) {
    case D23_CLDBFILES:
    case D23_DISDBOP:
	/* command: disable cache data base access operations */
	d22_dboperation = FALSE ;
	break ;
    case D23_ENADBOP:
	/* command: enable cache data base access operations */
	d22_dboperation = TRUE ;
	break ;
    case D23_RESTART:
	/* command: restart -> data base has changed */
	d22_dboperation = TRUE ;
	d22_admtab.d22_act_cache_id = D22_RESTART ;
	break ;
    case D23_ENALOG:
    case D23_DISLOG:
	/* command: enable/disable logging resp. exception handling */
	d27_003_reset_routing ((char *) drdpb.d23_Kparref, &result) ;
	break ;
    case D23_SVCLOGLEVEL:
	/* command: change logging level */
	dce_svc_debug_set_levels (drdpb.d23_Kparref, &result) ;
	if (result != svc_s_ok) {
	    fprintf (stderr,
	     "DUA-cache: can't change logging level (status = %#x)\n", result) ;
	}
	break ;
    default:
	confirm = FALSE ;
	break ;
    }

    if (confirm) {
	/* confirm distributed command */
	dcfpb.d23_Lhdinfo.d2_version = D23_V02 ;
	dcfpb.d23_Lkey = drdpb.d23_Kkey ;
	d23_conf_dist_cmd (&dcfpb) ;
    }
    return ;
}
