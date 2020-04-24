/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20gstruct.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:42  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:53  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:09:09  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:29:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:05:40  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:29:04  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:08:36  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:14  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:11:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:09:07  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:55:08  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:15:58  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20gstruct.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:47 $";
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
/* NAME         : d20gstruct.c                                        */
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
/*  FUNCTION:  d20_gen_name_structure ()                                     */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function initializes the variable partition of a mask with       */
/*      informations about a given (selected) structural rule. This initiali-*/
/*      zation consists of writing the names of all naming attributes to     */
/*      the mask output fields and generating corresponding mask input       */
/*      fields and mask input format information.The selected structural rule*/
/*      is given through rule number. Thus, the function can search the      */
/*      corresponding SRT entry and determine the structure of this selected */
/*      structural rule.                                                     */
/*      Note: The function initializes not always the complete variable      */
/*      partition of the mask, but only that part required to store the ob-  */
/*      ject class structure information. The caller of this function must   */
/*      declare unused the uninitialized part of the variable partition -->  */
/*      D20_NO_OP.                                                           */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      rule_nr     = Rule no. of the selected rule within the SRT.	     */
/*      maskptr     = Pointer to the variable part of the mask where the     */
/*                    information about the selected structural rule 	     */
/*                    should be stored.                                      */
/*      flen1       = Max. length of a mask output field.                    */
/*                    Note: If the name of a naming attribute exceeds this   */
/*                    limit, the supernumerary characters of the name are    */
/*                    lost.                                                  */
/*      flen2       = Max. length of a mask input field.                     */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      maskptr     = The initialized mask partition contains the output     */
/*                    informations (names of naming attributes) and the      */
/*                    corresponding input fields of the selected structural  */
/*                    rule.                                                  */
/*                    Note: The pointer 'maskptr' points to the first un-    */
/*                    initialized field (if there is any) of the variable    */
/*                    mask partition.                                        */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: xx.yy.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>

static 	char	shift_arr[NAM_ATTR_OFFSET + 1] = "   ";
extern	D20_srt		*d2_p_d20srt;
extern  signed32	d2_nd20srt;

/* ------------------------------------------------------------------------- */

void d20_gen_name_structure (	signed32 rule_nr, D20_omask **maskptr,
				signed32 flen1,   signed32 flen2)
{
	D20_srt		  *run_srt_ptr ;
	register signed32  i, len, par_nr ;

	/* search SRT entry.		*/
	for (run_srt_ptr = d2_p_d20srt, i = 0; i < d2_nd20srt; 
	     i++, run_srt_ptr++)
		if (run_srt_ptr->rule_nr == rule_nr)
			break ;
	
	if (run_srt_ptr->par_rule != (D20_srt *)NULL)
		{
		/* root position not arrived -> go to next higher level */
		par_nr = run_srt_ptr->par_rule->rule_nr;
		d20_gen_name_structure (par_nr, maskptr, flen1, flen2) ;
		}

	/* write naming attribute information into mask output field */
	for (i=0; i<run_srt_ptr->no_nam_attr; i++)
		{
		(*maskptr)->d20_fkt_id = D20_CSAY ;

		if (i == 0)
			/* first naming attribute is displayed without offset.*/
			sprintf (((*maskptr)++)->d20_ioinfo, "%.*s:", 
			flen1 - 1, run_srt_ptr->nam_attr[i]->at.att_name) ;
		else
			{
			strcpy((*maskptr)->d20_ioinfo, shift_arr);
			sprintf ((((*maskptr)++)->d20_ioinfo + NAM_ATTR_OFFSET),			"%.*s:",flen1-1, run_srt_ptr->nam_attr[i]->at.att_name);
			}

		len = (run_srt_ptr->nam_attr[i]->at.att_upper < flen2) ? 
		       run_srt_ptr->nam_attr[i]->at.att_upper : flen2 ; 

		/* initialize and enable mask input field */
		(*maskptr)->d20_fkt_id = D20_CGET ;
		*((*maskptr)->d20_ioinfo) = *((*maskptr)->d20_ioinfo + len) = 
									D2_EOS;

		/* write input field format into mask format field */
		sprintf (((*maskptr)++)->d20_oformat, "%%-%ds", len) ;
		}	

}
