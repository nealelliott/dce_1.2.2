/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20gdnname.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:35  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:50  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:15  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:08:49  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:28:16  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:01:40  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:28:07  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:35:06  marrek
 * 	Return Int for d20_gen_distname().
 * 	[1993/01/28  10:27:05  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:49:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:59  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:10:35  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:08:11  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:54:18  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:15:30  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20gdnname.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:38 $";
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
/* NAME         : d20gdnname.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
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
/*  FUNCTION:  d20_gen_distname ()                                           */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function generates a distinguished name (DN) string, where       */
/*      the RDN values are given through mask input fields, and the structural*/
/*      rule, to which this DN belongs, is given by an rule number. The      */
/*      function takes this rule number and searches in the structural       */
/*      rule table (SRT) for the corresponding entry. After this      	     */
/*      entry is found, the function is able to determine the structure of   */
/*      this rule and to handle the values of the mask input fields  	     */
/*      accordingly.                                                         */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      rule_nr     = rule no. of the structure entry within the SRT.        */
/*      ifields     = Reference to the mask input fields containing the      */
/*                    RDN values of the DN.                                  */
/*      ofield      = Pointer the field where the generated DN should be     */
/*                    stored.                                                */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      ofield      = Generated distinguished name.                          */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*	D2_ERROR	error in name fields.				     */
/*	D2_NOERROR	no error in input fields.			     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                         DATE: 14.3.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>

extern 	D20_srt		*d2_p_d20srt;
extern	signed32	d2_nd20srt;
extern	Bool		d20_printable;

/* ------------------------------------------------------------------------- */

d2_ret_val d20_gen_distname(signed32 rule_nr, char **ifields[], char *ofield)
{
	D20_srt		*run_srt_ptr ;
	register char	*p ;
	register signed16 i, len ;
	signed32 par_nr;
	signed32 sav_len;
	Bool	nam_attr_found = FALSE;
	char	type_val_sep;

	/* search structural rule entry within SRT */
	for (run_srt_ptr = d2_p_d20srt, i = 0; i < d2_nd20srt; 
	     i++, run_srt_ptr++)
		if (run_srt_ptr->rule_nr == rule_nr)
			break ;
	
	/* determine determine structure of the selected rule */
	if (run_srt_ptr->par_rule != (D20_srt *)NULL)
		{
		/* root position not arrived -> go to next higher level */
		par_nr = run_srt_ptr->par_rule->rule_nr;
		if (d20_gen_distname (par_nr, ifields, ofield) == D2_ERROR)
			return(D2_ERROR);
		}

	sav_len = strlen(ofield);

	/* fill AVAs belonging to one RDN.	*/
	for (i=0; i<run_srt_ptr->no_nam_attr; i++, (*ifields)++)
		{
		/* determine real size of RDN value */
		len = strlen (**ifields) ;
		p = **ifields + len - 1 ;
		while (*p-- == DS_DEFAULT_VALUE && len > 0)
			len-- ;

		if (len > 0) 
			{
			nam_attr_found = TRUE;
			/* write AVA information into ofield.	*/

			switch ((int) d27_401_syntax_to_rep((signed16)
				run_srt_ptr->nam_attr[i]->at.att_syntax))
				{
				case D2_T61_PR:
					if (d20_printable == TRUE)
						type_val_sep = D2_PRTBL_REP;
					else
						type_val_sep = D2_T61_REP;
					break;
				case D2_PRINTABLE:
					type_val_sep = D2_PRTBL_REP;
					break;
				case D2_IA5:                        
					type_val_sep = D2_IA5_REP;
					break;
				case D2_NUMERIC:
					type_val_sep = D2_NUM_REP;
					break;
				default:
					type_val_sep = D2_T61_REP;
					break;
				}

			sprintf (ofield + strlen (ofield), 
			D2_RDN_FORMAT, D2_AVA_SEP, 
			strlen(run_srt_ptr->nam_attr[i]->at.att_obj_id),
			run_srt_ptr->nam_attr[i]->at.att_obj_id, 
			type_val_sep,
			len, 
			**ifields) ;
			}
		else
			return(D2_ERROR);
		}

	/* put RDN seperator between two RDNs.	*/
	if (nam_attr_found)
		*(ofield + sav_len) = D2_RDN_SEP;

return(D2_NOERROR);
}
