/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20interf.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:54  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:04  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:05  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:43  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:15:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:47:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:10:51  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:45:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:16  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:34:52  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:08:33  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20interf.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:56 $";
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
/* NAME         : d20interf.c                                         */
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
/*  FUNCTIONS: d20_a0_read (),    d20_a1_sddobj (),   d20_a2_delobj (),      */
/*             d20_a3_search (),  d20_a4_addattr (),  d20_a5_delattr ()      */
/*             d20_a6_modattr (), d20_a7_addalias (),                        */
/*             d20_a11_modrdn ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      Each function contained in this module generates a function specific */
/*      parameter block by using the informations from the common IAPL-      */
/*      interface parameter block, calls the corresponding IAPL-function     */
/*      of the directory system V2 and transfers the result parameter (if    */
/*      any) back to the common IAPL-interface parameter block.              */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*                   Specific to each function (see document 'Leistungs-     */
/*                   beschreibung Directory System V2').                     */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                   Specific to each function (see document 'Leistungs-     */
/*                   beschreibung Directory System V2').                     */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D2_NOERROR = The function has been performed successfully.           */
/*      D2_ERROR   = An error is occurred (the used standard header con-     */
/*                   tains detailed error information).                      */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 7.3.88       */
/*exoff **********************************************************************/

#include <gds.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d21iapl.h>
#include <d2info.h>
#include <d2spec_msk.h>
#include <d20proto.h>

/* -------------------- declaration of global data ------------------------- */

extern Ds_v2_info	info ;	/* common IAPL-interface parameter block */

/*----------------------- ADD ATTRIBUTE ----------------------------------*/

 d2_ret_val d20_a4_addattr (
   D2_pbhead *pbhead)
{
Ds_at_entry     attr_ent[2 * D2_AT_MAX],*tmp_attr_ent;
D2_a_info	*tmp_attrinfo;
D2_a_value 	*tmp_a_val;
d2_ret_val	result;
signed16	counter;
signed32	no_attr_ent = 0;
signed16	mod_type;

tmp_attr_ent = attr_ent;
tmp_attrinfo = info.attrinfo;

while (tmp_attrinfo != (D2_a_info *)NULL)
	{
	tmp_a_val = tmp_attrinfo->d2_a_val;
	for (counter = 0,mod_type = D2_ADDATT; counter < tmp_attrinfo->d2_a_no_val ; counter++,tmp_attr_ent++)
		{
		tmp_attr_ent->a_val.d2_a_v_len = tmp_a_val->d2_a_v_len;
		tmp_attr_ent->attr_id.d2_type = tmp_attrinfo->d2_a_type.d2_type;
		tmp_attr_ent->attr_id.d2_typ_len = 
		tmp_attrinfo->d2_a_type.d2_typ_len;
		tmp_attr_ent->hex_use = FALSE;
		tmp_attr_ent->mod_type = mod_type;
		tmp_attr_ent->a_val.d2_a_rep = tmp_a_val->d2_a_rep;
		switch((int)tmp_attr_ent->a_val.d2_a_rep)
			{
			case D2_T61:
				tmp_attr_ent->a_val.d2_at_value.d2_t61_a_v = 
				tmp_a_val->d2_at_value.d2_t61_a_v;
				break;
			case D2_ACCL:
			case D2_OCTET:
				tmp_attr_ent->a_val.d2_at_value.d2_oct_a_v = 
				tmp_a_val->d2_at_value.d2_oct_a_v;
				break;
			case D2_PRINTABLE:
				tmp_attr_ent->a_val.d2_at_value.d2_prt_a_v = 
				tmp_a_val->d2_at_value.d2_prt_a_v;
				break;
			case D2_DISTNAME:
				tmp_attr_ent->a_val.d2_at_value.d2_obj_name_a_v
				= tmp_a_val->d2_at_value.d2_obj_name_a_v;
				break;
			case D2_OBJ_IDENT:
				tmp_attr_ent->a_val.d2_at_value.d2_obid_a_v = 
				tmp_a_val->d2_at_value.d2_obid_a_v;
				break;
			case D2_NUMERIC:
				tmp_attr_ent->a_val.d2_at_value.d2_num_a_v = 
				tmp_a_val->d2_at_value.d2_num_a_v;
				break;
			case D2_TIME:
				tmp_attr_ent->a_val.d2_at_value.d2_utc_a_v = 
				tmp_a_val->d2_at_value.d2_utc_a_v;
				break;
			case D2_PSAP_STX:
				tmp_attr_ent->a_val.d2_at_value.d2_psap_a_v = 
				tmp_a_val->d2_at_value.d2_psap_a_v;
				break;
			case D2_ASN1:
				tmp_attr_ent->a_val.d2_at_value.d2_asn1_a_v = 
				tmp_a_val->d2_at_value.d2_asn1_a_v;
				break;
			case D2_IA5:
				tmp_attr_ent->a_val.d2_at_value.d2_ia5_a_v = 
				tmp_a_val->d2_at_value.d2_ia5_a_v;
				break;
			case D2_T61_LIST:
				tmp_attr_ent->a_val.d2_at_value.d2_t61lst_a_v = 
				tmp_a_val->d2_at_value.d2_t61lst_a_v;
				break;
			case D2_T61_PR_LIST:
				tmp_attr_ent->a_val.d2_at_value.d2_post_a_v = 
				tmp_a_val->d2_at_value.d2_post_a_v;
				break;
			case D2_TTXID_STX:
				tmp_attr_ent->a_val.d2_at_value.d2_ttxid_a_v = 
				tmp_a_val->d2_at_value.d2_ttxid_a_v;
				break;
			case D2_TLXNR_STX:
				tmp_attr_ent->a_val.d2_at_value.d2_tlxnr_a_v = 
				tmp_a_val->d2_at_value.d2_tlxnr_a_v;
				break;
			case D2_FAXNR_STX:
				tmp_attr_ent->a_val.d2_at_value.d2_faxnr_a_v = 
				tmp_a_val->d2_at_value.d2_faxnr_a_v;
				break;
			case D2_BOOLEAN:
				tmp_attr_ent->a_val.d2_at_value.d2_bool_a_v = 
				tmp_a_val->d2_at_value.d2_bool_a_v;
				break;
			case D2_INT_LIST:
				tmp_attr_ent->a_val.d2_at_value.d2_intlist_a_v =
				tmp_a_val->d2_at_value.d2_intlist_a_v;
				break;
				break;
			case D2_INTEGER:
				tmp_attr_ent->a_val.d2_at_value.d2_int_a_v = 
				tmp_a_val->d2_at_value.d2_int_a_v;
				break;
			case D2_MHS_ORADR_STX:
				tmp_attr_ent->a_val.d2_at_value.d2_or_addr_av =
				tmp_a_val->d2_at_value.d2_or_addr_av;
				break;
			case D2_MHS_ORNAME_STX:
				tmp_attr_ent->a_val.d2_at_value.d2_or_name_av =
				tmp_a_val->d2_at_value.d2_or_name_av;
				break;
			case D2_MHS_DLSP_STX:
				tmp_attr_ent->a_val.d2_at_value.d2_dl_s_p_av =
				tmp_a_val->d2_at_value.d2_dl_s_p_av;
				break;
			case D2_CERTIFICATE:
				tmp_attr_ent->a_val.d2_at_value.d2_cert_a_v =
				tmp_a_val->d2_at_value.d2_cert_a_v;
				break;
			case D2_CERT_PAIR:
				tmp_attr_ent->a_val.d2_at_value.d2_ctpair_a_v =
				tmp_a_val->d2_at_value.d2_ctpair_a_v;
				break;
			case D2_CERT_LIST:
				tmp_attr_ent->a_val.d2_at_value.d2_ctlist_a_v =
				tmp_a_val->d2_at_value.d2_ctlist_a_v;
				break;
			}
		tmp_attr_ent ++;
		tmp_attr_ent->mod_type = mod_type = D2_ADD_AV;
		no_attr_ent++;
		tmp_a_val++;
		}
	tmp_attrinfo = tmp_attrinfo->d2_a_next;
	tmp_attr_ent--;
	}

d20_fill_modify(D20_ASTART | D20_AINSERT | D20_AEND,attr_ent,no_attr_ent);

result = d20_a6_modattr(pbhead);

return (result) ;
}

/*------------------------ DS_REMOVE_ATTRIBUTE ----------------------------*/ 

 d2_ret_val d20_a5_delattr (
   D2_pbhead *pbhead)
{
Ds_at_entry     attr_ent[D2_AT_MAX],*tmp_attr_ent;
D2_a_type 	*tmp_type;
d2_ret_val	result;
signed16	counter;

tmp_attr_ent = attr_ent;
tmp_type = info.reqinfo.d2_r_type_at;

for (counter = 0; counter < info.reqinfo.d2_r_no_at; counter++)
	{
	tmp_attr_ent->attr_id.d2_type = tmp_type->d2_type;
	tmp_attr_ent->attr_id.d2_typ_len = tmp_type->d2_typ_len;
	tmp_attr_ent->mod_type = D2_DELATT;
	tmp_attr_ent++;
	tmp_type++;
	}

d20_fill_modify(D20_ASTART | D20_AINSERT | D20_AEND,attr_ent,info.reqinfo.d2_r_no_at);
info.reqinfo.d2_r_no_at = 0;

result = d20_a6_modattr(pbhead);

return (result) ;
}

/* ---------------------------- DS_MODIFY_RDN ------------------------------ */

 d2_ret_val d20_a11_modrdn (
   D2_pbhead *pbhead)
{
	D21_pB	pb ;
	d2_ret_val result;

	/* generate parameter block */
	pb.d21_Bpbhead = pbhead ;
	pb.d21_Bc_a = &info.serv_cntrl ;
	pb.d21_Bbind_id = info.bind_id ;
	pb.d21_Bentry = info.entry + 1 ;
	pb.d21_Bnew_rdn = info.rdn +1 ;

	pb.d21_Bdel_old_rdn = TRUE;
	pb.d21_Berr = (D2_error *)NULL;

	if (info.error != (D2_error *)NULL)
		{
		free((char *)info.error);
		info.error = (D2_error *)NULL;
		}

	result = d21_B_replace_rdn (&pb);

/* ignore the value returned in common result structure and set the error value
   in pbhead as the error value for the first attribute in the list of errors
   for attributes, if the problem is there with an attribute.
*/
	if (pb.d21_Berr != NULL)
		if (pb.d21_Berr->d2_e_type == D2_AT_PROBLEM)
			{
			pb.d21_Bpbhead->d2_errvalue = 
			pb.d21_Berr->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob;
			}

	return ((int) result) ;
}
