/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdsref.c,v $
 * Revision 1.1.11.2  1996/02/18  18:18:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:07  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:55:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:14  root]
 * 
 * Revision 1.1.9.2  1994/05/10  15:54:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:23  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  18:06:52  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:39:07  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  06:55:05  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:03:46  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  19:49:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:27:10  bbelch]
 * 
 * Revision 1.1.5.3  1992/11/16  08:59:16  marrek
 * 	Remove empty #ifndef lint and #ifdef ANSIC,
 * 	replace SNI_SVR4 by SNI_DLOPEN.
 * 	[1992/11/06  11:43:08  marrek]
 * 
 * Revision 1.1.5.2  1992/09/29  21:11:00  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:25:06  treff]
 * 
 * Revision 1.1.2.2  1992/06/30  13:52:41  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  13:51:39  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_cdsref.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:18:10 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*---------------------------------------------------------------------

	Filename:		d21_cdsref.c 

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Functions:	
		exported:	d21_cds_referral().

		static:		name2untyped().

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
#include <dce/gda_x500.h>
#include <dce/dnsclerk.h>
#include <d21_cdssw.h>

/*----------------  local static function prototypes  ------------------*/


static	void	name2untyped (
					u_char	*name,
					u_char	*untyped);

/*--------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------

	d21_cds_referral():

	Checks to see if an operation should be referred to the local
	dns cell. If a referral is required the local cell is passed 
	referral addressing information. This addressing information 
	will be used to direct the next naming operation to a remote
	cell.

 -----------------------------------------------------------------------*/

int		d21_cds_referral (
			D2_name_string	gds_name,
			D2_error		*naming_error)
{
	dns_full_name_t		*dns_fname;
    dns_opq_fname       part_result;
	dnsFlagStat_t		flags;
	D21_p3				read_par_blk;
	D2_n_error			*name_error;
	x500_dns_cell_t		cell_info;
	x500_replica_t		**cell_replicas;
	u_char				*resolved,			cds_name [D2_DNL_MAX];
	char				*getenv();
	char				*replicas_space,		*cds_root,			
						*cell_attr,	 		**replicas_attr,	**freeme;
	short				r_code,	num_replicas,	cell_len,	*replicas_len,
				 		dummy_id,	size, 		count;
	const char function_name[] = "d21_cds_referral";


	DUA_TRACE_IAPL_ENTRY

	r_code = D2_NOERROR;

	/*
		check to see if a name error occurred
												*/
	if (naming_error == (D2_error *) NULL) {
		
		return (D2_NOERROR);
	}

	if (naming_error->d2_e_type == D2_NM_PROBLEM) {

		name_error = naming_error->d2_e_problem.d2_e_nmproblem;

		if ((name_error->d2_ne_prob == D2_NFOUND) ||
			(name_error->d2_ne_prob == D2_NMSYNTAX_ERROR)) {

			/*
				do a read of the resolved part of the name.
																*/
			resolved =  name_error->d2_ne_match;

			if (x500_bind(&dummy_id) != X500_NOERROR) {

				return(D2_ERROR);
			}

			if (x500_read_cell(dummy_id, resolved, &cell_attr, &cell_len,
				&num_replicas, &replicas_attr, &replicas_len, 
				&freeme) != X500_NOERROR) {

				return(D2_ERROR);
			}
			
			if (x500_unbind(dummy_id) != X500_NOERROR) {

				return (D2_ERROR);
			}

			/*	
				Convert the global name to a CDS relative name.
																*/
			size = strlen ((char*)resolved);
			gds_name += size + 1;
			strcpy ((char *)cds_name, (char *)gds_name);
			name2untyped (gds_name, cds_name);
			dns_fname = (dns_full_name_t *) cds_name;
				
			if (x500_str_to_cell ((u_char *)cell_attr, cell_len, 
				&cell_info) != X500_NOERROR) {

				return(D2_ERROR);
			}


			count = cell_info.num_replicas;
			if ((replicas_space = D27_MALLOC(svc_c_sev_warning,count * sizeof (x500_replica_t)))
				== NULL) {

				return (D2_ERROR);
			}

			count = 0;
			cell_replicas = (x500_replica_t **) replicas_space;

			if (x500_str_to_replica ((u_char *) replicas_attr[count],
				replicas_len[count],cell_replicas[count]) != X500_NOERROR) {

				return (D2_ERROR);
			}

			memset ((char *) &flags, 0, sizeof (struct dnsFlagStat));
			flags.fsPartialRes = &part_result;
			flags.fsResLength = sizeof (part_result);

#ifdef NOTDEF
#  ifdef NEW_PROTOTYPE

			if (dnsCacheDir (dns_fname, cell_info, cell_replicas, &flags) 
				== DNS_ERROR) {
				r_code = D2_ERROR;
			} else {
					r_code = DSW_REFERRED;
			}
#  else

			if (dnsCacheDir (dns_fname, (u_char *) NULL, (u_char **) NULL, 
				&flags) == DNS_ERROR) {
				r_code = D2_ERROR;
			} else {
					r_code = DSW_REFERRED;
			}
#  endif /* NEW_PROTOTYPE */
#endif /* NOTDEF */

			/*
				Free all space involved in the referral.
															*/
			free ((char *) cell_replicas);

			if (replicas_attr !=  (char **) NULL) {

				for (count = 0; count != cell_info.num_replicas; count ++) {

					free ((char *) replicas_attr[count]);
				}
			}

			if (replicas_len !=  (short *) NULL) {

				for (count = 0; count != cell_info.num_replicas; count ++) {

					free ((char *) replicas_len[count]);
				}
			}

			if (freeme != (char **) NULL) {

				free ((char *) freeme);
			}
		}
	}

	DUA_TRACE_IAPL_EXIT(r_code)
	return (r_code);
}



/*------------------------------------------------------------------------

	name2untyped:

	Strips the typeless OID from a distinguished name.

 ------------------------------------------------------------------------*/

static	void	name2untyped (
					u_char	*name,
					u_char	*untyped)
{
	while (*name != '\0') {
		/*	strip out the typed part of the current RDN	*/
		while (*name != '\1') {
			name ++;
		}
		name ++;

		/*	copy the rdn value to the untyped name */
		while ((*name  != '\1') && (*name != '\0')) {
			*untyped = *name;
			name ++;
			untyped ++;
		}
		if (*name == '\0') {
			*untyped = '\0';
		} else  {
			*untyped = '/';
			untyped ++;
			name ++;
		}
	}

}
