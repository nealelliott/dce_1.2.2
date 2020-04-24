/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20iapl.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:59  root]
 * 
 * Revision 1.1.8.3  1994/07/06  15:06:18  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:10:38  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:04  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:38  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:15:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:45:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:10:30  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:45:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:08  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:33:16  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:08:24  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20iapl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:54 $";
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
/* NAME         : d20iapl.c                                           */
/*								      */
/* AUTHOR       : Schmid , D AP 11                                    */
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
/*  FUNCTION:  d20_call_iapl()                                               */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function calls a specific directory system function (depending   */
/*      on operation code).                                                  */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      operation   = Operation code of the directory system function which  */
/*                    should be called.                                      */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    Depends on called directory system function.           */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D2_NOERROR = The function has been performed successfully.           */
/*      D2_ERROR   = An error is occurred.                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: W.Schmid                                      DATE: 2.5.88       */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <d21iapl.h>
#include <d2info.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d20proto.h>

D2_pbhead    *d20_head;

extern  Ds_v2_info      info ;   /* common IAPL-interface parameter block */

static d2_ret_val (*d20_fkttab[]) (D2_pbhead *) = { 
				         d20_a3_search, d20_a1_addobj,
				         d20_a2_delobj, d20_a6_modattr,
				         d20_a0_read, d20_a7_addalias,
				       } ;

/* --------------------------------------------------------------------- */

 d2_ret_val d20_call_iapl (
   signed16 operation)
{
d2_ret_val ret_value;
signed16   version;

	version = d20_head->d2_version;
	d20_head->d2_version = D21V022;

	ret_value = ((*d20_fkttab[operation]) (d20_head))
		    ? D2_ERROR : D2_NOERROR;
	d20_head->d2_version = version;

	return(ret_value);
}


 d2_ret_val d20_a0_read (
   D2_pbhead *pbhead)
{
	D21_p3	pb ;
	D2_c_res c_res;
	d2_ret_val result ;

	/* generate parameter block */
	pb.d21_3pbhead = pbhead ;

info.serv_cntrl.d2_serv_cntrls |= D2_PREF_CHAINING;

	pb.d21_3c_a = &info.serv_cntrl ;
	pb.d21_3bind_id = info.bind_id ;
	pb.d21_3entry = info.entry + 1 ;
	pb.d21_3r_info = &info.reqinfo ;

	pb.d21_3err = (D2_error *)NULL;
	pb.d21_3cres = &c_res;

	if (info.resentry != (D2_ei_info *)NULL)
		{
		free((char *)info.resentry);
		info.resentry = (D2_ei_info *)NULL;
		}

	if (info.error != (D2_error *)NULL)
		{
		free((char *)info.error);
		info.error = (D2_error *)NULL;
		}

	result = d21_3_read (&pb) ;

info.serv_cntrl.d2_serv_cntrls &= ~D2_PREF_CHAINING;

	info.reqinfo.d2_r_no_at = 0;
	info.resentry = pb.d21_3res ;

	if (pb.d21_3err != NULL)
		if (pb.d21_3err->d2_e_type == D2_AT_PROBLEM)
			{
			pb.d21_3pbhead->d2_errvalue = 
			pb.d21_3err->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob;
			}

	return (result) ;
}

/* ------------------------ DS_ADD_OBJECT ---------------------------------- */

 d2_ret_val d20_a1_addobj (
   D2_pbhead *pbhead)
{
	D21_p7		pb ;
	d2_ret_val	result;

	/* generate parameter block */
	pb.d21_7pbhead = pbhead ;
	pb.d21_7c_a = &info.serv_cntrl ;
	pb.d21_7bind_id = info.bind_id ;

	pb.d21_7entry_name = info.entry + 1;
	pb.d21_7_at_info = (info.attrinfo->d2_a_val == (D2_a_value *) NULL) ?
			 (D2_a_info *) NULL : info.attrinfo ;
	pb.d21_7err = (D2_error *)NULL;

	if (info.error != (D2_error *)NULL)
		{
		free((char *)info.error);
		info.error = (D2_error *)NULL;
		}

	result = d21_7_add_object (&pb) ;

	if (pb.d21_7err != NULL)
		if (pb.d21_7err->d2_e_type == D2_AT_PROBLEM)
			{
			pb.d21_7pbhead->d2_errvalue = 
			pb.d21_7err->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob;
			}

	return (result) ;
}

/* ----------------------- DS_REMOVE_OBJECT -------------------------------- */

 d2_ret_val d20_a2_delobj (
   D2_pbhead *pbhead)
{
	D21_p8	   pb ;
	d2_ret_val result;

	/* generate parameter block */
	pb.d21_8pbhead = pbhead ;
	pb.d21_8c_a = &info.serv_cntrl ;
	pb.d21_8bind_id = info.bind_id ;
	pb.d21_8entry = info.entry + 1 ;

	pb.d21_8err = (D2_error *)NULL;

	if (info.error != (D2_error *)NULL)
		{
		free((char *)info.error);
		info.error = (D2_error *)NULL;
		}

	result = d21_8_remove_object (&pb);

	if (pb.d21_8err != NULL)
		if (pb.d21_8err->d2_e_type == D2_AT_PROBLEM)
			{
			pb.d21_8pbhead->d2_errvalue = 
			pb.d21_8err->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob;
			}

	return ((int) result) ;
}

/* ----------------------------- DS_SEARCH --------------------------------- */

 d2_ret_val d20_a3_search (
   D2_pbhead *pbhead)
{
	D21_p5	   pb ;
	d2_ret_val result ;
	D2_c_res c_res;

	if (info.srchop == D2_BASE_OBJECT && info.filter_present == FALSE
	    && info.filename == NULL)
	{   
	return(d20_a0_read (pbhead));
	}
	else
	{
	/* generate parameter block */
	pb.d21_5pbhead = pbhead ;

info.serv_cntrl.d2_serv_cntrls |= D2_PREF_CHAINING;

	pb.d21_5c_a = &info.serv_cntrl ;
	pb.d21_5bind_id = info.bind_id ;
	pb.d21_5base_obj = (strlen((char *)info.entry) == 0) ?
				(D2_name_string) NULL :
				info.entry + 1 ;
	pb.d21_5subset = info.srchop ;
	pb.d21_5f_pattern = (info.filter_present)
			    ? &info.filter 
			    : (D2_Fi_filter *) NULL;
	pb.d21_5srch_al = FALSE;
	pb.d21_5r_info = &info.reqinfo ;
	pb.d21_5file = info.filename ;

	pb.d21_5err = (D2_error *)NULL;
	pb.d21_5cres = &c_res ; 

	if (info.resentry != (D2_ei_info *)NULL)
		{
		free((char *)info.resentry);
		info.resentry = (D2_ei_info *)NULL;
		}

	if (info.error != (D2_error *)NULL)
		{
		free((char *)info.error);
		info.error = (D2_error *)NULL;
		}

	result = d21_5_search (&pb) ;

info.serv_cntrl.d2_serv_cntrls &= ~D2_PREF_CHAINING;

	info.resentry = pb.d21_5res ;

	if (pb.d21_5err != NULL)
		if (pb.d21_5err->d2_e_type == D2_AT_PROBLEM)
			{
			pb.d21_5pbhead->d2_errvalue = 
			pb.d21_5err->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob;
			}

	return (result) ;
	}
}

/* ------------------------ DS_MODIFY_ATTRIBUTE ---------------------------- */

 d2_ret_val d20_a6_modattr (
   D2_pbhead *pbhead)
{
	D21_pC	   pb ;
	d2_ret_val result ;

	/* generate parameter block */
	pb.d21_Cpbhead = pbhead ;
	pb.d21_Cc_a = &info.serv_cntrl ;
	pb.d21_Cbind_id = info.bind_id ;
	pb.d21_Centry = info.entry + 1 ;
	pb.d21_Cmod = info.modinfo ;

	pb.d21_Cerr = (D2_error *)NULL;

	if (info.error != (D2_error *)NULL)
		{
		free((char *)info.error);
		info.error = (D2_error *)NULL;
		}

	result = d21_C_modify_attribute (&pb) ;

	if (pb.d21_Cerr != NULL)
		if (pb.d21_Cerr->d2_e_type == D2_AT_PROBLEM)
			{
			pb.d21_Cpbhead->d2_errvalue = 
			pb.d21_Cerr->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob;
			}

	return (result) ;
}

/*---------------------------- DS_ADD_ALIAS -------------------------------*/ 

 d2_ret_val d20_a7_addalias (
   D2_pbhead *pbhead)
{
d2_ret_val result;
signed16 type;
Ds_at_entry	at_entry[2];

at_entry[0].a_val.d2_a_v_len = strlen((char *)info.entry + 1) + 1;
at_entry[0].attr_id.d2_type = (Ob_id_string)D2_ALIASED_OBJECT;
at_entry[0].attr_id.d2_typ_len = sizeof(D2_ALIASED_OBJECT) - 1;
at_entry[0].hex_use = FALSE;
at_entry[0].a_val.d2_a_rep = (unsigned16) d27_401_syntax_to_rep(
				(signed16) D2_DISTNAME);
at_entry[0].a_val.d2_at_value.d2_oct_a_v = info.entry + 1;

at_entry[1].a_val.d2_a_v_len = sizeof(D2_OALIAS) - 1;
at_entry[1].attr_id.d2_type = (Ob_id_string)D2_OBJ_CLASS;
at_entry[1].attr_id.d2_typ_len = sizeof(D2_OBJ_CLASS) - 1;
at_entry[1].hex_use = FALSE;
at_entry[1].a_val.d2_a_rep = (unsigned16) d27_401_syntax_to_rep(
				(signed16) D2_OBJ_IDENT);
at_entry[1].a_val.d2_at_value.d2_oct_a_v = (Octet_string)D2_OALIAS;

type = D20_ASTART | D20_AINSERT | D20_AEND;

ds4_b02_fill_attr(type,&at_entry[0], 2);

strcpy((char *)info.entry, (char *)info.alias);

result = d20_a1_addobj(pbhead);
return (result) ;
}
