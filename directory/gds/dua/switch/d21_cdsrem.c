/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdsrem.c,v $
 * Revision 1.1.11.2  1996/02/18  18:18:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:08  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:55:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:16  root]
 * 
 * Revision 1.1.9.2  1994/05/10  15:54:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:27  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  18:06:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:39:16  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  06:55:57  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:03:55  marrek]
 * 
 * Revision 1.1.5.5  1993/02/02  10:18:24  marrek
 * 	Insert <pthread.h>
 * 	Fixes for OT 6554
 * 	[1993/02/02  09:33:12  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  19:49:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:27:14  bbelch]
 * 
 * Revision 1.1.5.3  1992/11/16  08:59:21  marrek
 * 	Remove duplicated history line, empty #ifndef lint and #ifdef ANSIC,
 * 	replace SNI_SVR4 by SNI_DLOPEN.
 * 	<
 * 	[1992/11/06  11:44:12  marrek]
 * 
 * Revision 1.1.5.2  1992/09/29  21:11:05  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:25:21  treff]
 * 
 * Revision 1.1.2.3  1992/06/30  20:59:34  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:40:24  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:38:39  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:39:39  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_cdsrem.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:18:12 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*---------------------------------------------------------------------

	Filename:		d21_cdsrem.c 

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Functions:	
		exported:		d21_8_switch().

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

#include <d21dua.h>
#include <dce/dnsclerk.h>
#include <d21_cdssw.h>

/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------

	d21_8_switch():

	Removes an object from the local DNS cell. The remove entry  
	operation is received at the IAPL* interface and has to be mapped 
	to a DNSPI interface calls. The result from the DNSPI remove
	entry operation is then mapped to the IAPL* remove entry operation 
	result.

 -----------------------------------------------------------------------*/

int		d21_8_switch (
			D21_p8	*rem_par_blk)
{
	dns_opq_fname		part_result;
	dns_full_name_t		*dns_fname;
	u_char				cds_name [D2_DNL_MAX];
	struct dnsFlagStat	flags;
	D2_pbhead			*rem_error;
	int			r_code;
	const char function_name[] = "d21_8_switch";

	DUA_TRACE_IAPL_ENTRY
	r_code = D2_NOERROR;

	/*
		the pbhead structure is malloced by the IAPL* interface
															*/
	rem_error = rem_par_blk->d21_8pbhead;
	/*
		IAPL* common arguments are mapped to DNS flags structure.
																	*/
	(void) d21_cds_set_flags (&flags, rem_par_blk->d21_8c_a);
	flags.fsPartialRes = &part_result;
	flags.fsResLength = sizeof (part_result);
	flags.fsMaybeMore = TRUE;

	if (d21_x500name2cdsname (rem_par_blk->d21_8entry, cds_name)
		== D2_ERROR) {
		DUA_TRACE_X500_TO_CDS_NAME_FAILED
		rem_error->d2_retcode = D2_CALL_ERR;
        rem_error->d2_errclass = D2_PAR_ERR;
        rem_error->d2_errvalue = D2_NOT_SPEC;
        return (D2_ERROR);
	}

	dns_fname = (dns_full_name_t *) cds_name;

	/*
		Remove an entry from the local DNS cell. The object to 
		be removed may be a CDS alias. CDS groups may be removed
		using dnsDelObj(). 

		It is important to call dnsDelSLink() first as CDS always
		dereferences aliases for dnsDelObj().
																*/
	if (cdsDelSLink (dns_fname, &flags) == DNS_ERROR) {
		DUA_TRACE_CDS_FUNCTION_FAILED("cdsDelSLink")
		if (flags.fsLocStat == DNS_UNKNOWNENTRY) {

			if (cdsDelObj (dns_fname, &flags) == DNS_ERROR) {
				DUA_TRACE_CDS_FUNCTION_FAILED("cdsDelObj")
				d21_cds_map_error (flags.fsLocStat, rem_error);
				d21_cds_error_info (rem_error, &part_result, 
							&rem_par_blk->d21_8err);
				r_code = D2_ERROR;
			}

		} else {

			d21_cds_map_error (flags.fsLocStat, rem_error);
			d21_cds_error_info (rem_error, dns_fname, &rem_par_blk->d21_8err);
			r_code = D2_ERROR;
		}
	}

	DUA_TRACE_IAPL_EXIT(rem_par_blk->d21_8pbhead->d2_errvalue)
	return (r_code);
}
