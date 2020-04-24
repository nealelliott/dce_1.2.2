/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdssw.c,v $
 * Revision 1.1.748.2  1996/02/18  18:18:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:09  marty]
 *
 * Revision 1.1.748.1  1995/12/08  15:55:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:17  root]
 * 
 * Revision 1.1.746.2  1994/05/10  15:54:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:30  marrek]
 * 
 * Revision 1.1.746.1  1994/02/22  18:06:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:39:24  marrek]
 * 
 * Revision 1.1.744.2  1993/10/14  17:27:36  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:05  keutel]
 * 
 * Revision 1.1.744.1  1993/10/13  17:32:16  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:34:54  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  06:57:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:04:14  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  10:18:29  marrek
 * 	Remove #ifndef lint and ANSIC
 * 	insert <stdlib.h>, <pthread.h>
 * 	[1993/02/02  09:35:01  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:49:49  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:27:20  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/16  08:59:30  marrek
 * 	Remove duplicated history line.
 * 	[1992/11/06  11:45:50  marrek]
 * 
 * Revision 1.1.4.2  1992/10/05  12:54:17  marrek
 * 	Changes for bug 5205.
 * 	[1992/10/05  12:53:30  marrek]
 * 
 * Revision 1.1.2.4  1992/06/30  20:59:36  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:40:30  melman]
 * 
 * Revision 1.1.2.3  1992/06/11  14:12:11  melman
 * 	Reverted the following:
 * 		if (*obj_name == '\2') {
 * 			obj_nam++;
 * 		}
 * 		if (*obj_name == '\1') {
 * 			obj_nam++;
 * 		}
 * 	with:
 * 		if (*obj_name != D2_EOS) {
 * 			obj_nam++;
 * 		}
 * 	The top version can get trouble in case of strings with "...\2\1....",
 * 	which should never happen, but who knows?
 * 	[1992/06/11  14:11:10  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:38:48  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:39:44  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_cdssw.c,v $ $Revision: 1.1.748.2 $ $Date: 1996/02/18 18:18:13 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*---------------------------------------------------------------------

	Filename:		d21_cdssw.c 

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Functions:	
		exported:	d21_cds_switch()

	Version:		1.0 (initial)

	Update History:
	Version:		Date:		Comments:			 	By whom:
	--------------------------------------------------------------------
	1.0				1.02.91		original				P. Dunne


 -----------------------------------------------------------------------*/

#include <gds.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>
#include <dce/dnsclerk.h>
#include <d21_cdssw.h>

/*-----------------------------------------------------------------------

	d21_cds_switch():

	Checks to see if an operation should be handled by CDS. 

 -----------------------------------------------------------------------*/

int		d21_cds_switch (
			u_char	*obj_name)
{
	u_char			type_str [D2_OBJ_IDL_MAX];
	int				r_code,		count, TL_OID_FOUND;
	const char function_name[] = "d21_cds_switch";


	r_code = D2_NOERROR;
	TL_OID_FOUND = FALSE;

	/* 
		Check to see if the object name is root (ie NULL).
															*/
	if (obj_name == (u_char *) NULL) {

		r_code = D2_NOERROR;

	} else {

		/* 
			Check for a DN with a typless OID.  
												*/
		while (*obj_name != D2_EOS && !TL_OID_FOUND) {

			count = 0;
		
			while ( (*obj_name != D2_T61_REP) &&
				(*obj_name != D2_PRTBL_REP) &&
				(*obj_name != D2_IA5_REP) &&
				(*obj_name != D2_NUM_REP) ) {

				type_str[count] = *obj_name;
				obj_name++;
				count++;
			}

			type_str[count] = D2_EOS;
			obj_name++;

			if (strcmp ((char *) type_str, DSW_TYPELESS_OID) == 0) {

				TL_OID_FOUND = TRUE;

			} else {

				/* scip past the RDN value of the DN */
				while (*obj_name != D2_AVA_SEP &&
				       *obj_name != D2_RDN_SEP &&
				       *obj_name != D2_EOS) {

					obj_name++;
				}

				if (*obj_name != D2_EOS) {
					obj_name++;
				}
			}
		}
	}

	if (TL_OID_FOUND) {
		r_code = DSW_NSPACE;

	} 

	if (r_code == DSW_NSPACE)
	  {
	  DUA_TRACE_CDS_NAME
	  }
	else
	  {
	  DUA_TRACE_X500_NAME
	  }
	return (r_code);
}
