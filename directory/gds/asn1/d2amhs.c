/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2amhs.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:11  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:25:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:52  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:36  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:28:20  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:39  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:17:28  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:38:18  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:47:05  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:09:10  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:39:12  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:13:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:11:17  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:02:17  marrek
 * 	November 1992 code drop
 * 	remove include of cmx.h
 * 	[1992/12/04  10:42:00  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  16:03:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:54:38  marrek]
 * 
 * Revision 1.1.2.6  1992/08/11  20:58:39  mhickey
 * 	Drop of a fix for the mail handling system.
 * 
 * 	Defect 5008.
 * 	[1992/08/11  20:58:06  mhickey]
 * 
 * Revision 1.1.2.5  1992/07/17  19:58:38  weisman
 * 	Checked in at OSF for SNI
 * 	[1992/07/17  19:26:39  weisman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d2amhs.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:16 $";
#endif


/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*daton	***************************************************************/
/*								      */
/* TYP		: C-FILE					      */
/*								      */
/* NAME         : d2amhs.c                                            */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India                        */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source files contains subroutines to           */
/*                handle mhs attributes.                 	      */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL FUNCTIONS *********/

static signed32 d2a142_oradr_apdu_len ( ORAddress * );
static void d2a144_copy_str ( byte ** , char * , unsigned16 );
static void d2a146_copy_oradr_part (byte **,char *,unsigned16,unsigned16);
static signed32 d2a147_get_e_attr_idx ( ORAddress * , signed32 );
static void d2a148_copy_pds_parameter ( byte ** , PDSParameter * , unsigned16 );

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a140_oradr_to_ipc()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*                                                                    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*			D2_NOERROR : no error 			      */
/*			D2_ERROR   : error			      */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a140_oradr_to_ipc(
  ORAddress	*  or_address,
  byte         ** act_apdu,
  byte 	** s_apdu,
  signed32		*  apdu_len,
  byte 	*  err_apdu )
{
const char function_name[] = "d2a140_oradr_to_ipc";
  signed32 rv, sel_syntax, len;
  signed32 i , j , k;
  byte * scan;
  dua_psap * psap;
  D21_OR_address_part * p_oradr;
  string 	p_str;
  string 	* p_str1;
  D23_av_value  * av_value;
  P_Name 	* p_name = (P_Name *)NULL;
  P_Name 	* p1_name = (P_Name *)NULL;
  S_Attrs	* s_attr;
  O_U_Names 	* ou_names = (O_U_Names *)NULL;
  D_D_Attrs 	* dd_attr = (D_D_Attrs *)NULL;

	len = d2a142_oradr_apdu_len(or_address);
	/* check memory length */
	rv = d2a033_check_memsize(D2_ALIGN(len)+sizeof(D23_av_value), s_apdu,
	 	act_apdu,apdu_len,(D23_rserror *)err_apdu,apdu_len);
	if (rv != D2_NOERROR)
	{
	 	/* not enough memory for attribute value */
	D2_ASN1_ERR_RETURN_WITH_TRACE
	} /* if (rv != D2_NOERROR) */

	scan = *act_apdu + D23_S_ATV;
	av_value=(D23_av_value *)(*act_apdu);
	p_oradr = (D21_OR_address_part *) scan;
	s_attr = &or_address->s_attrs;
	if (s_attr->c_name != (str_NP_choice *)NULL)
	{
	  d2a146_copy_oradr_part(&scan, s_attr->c_name->str_val, 
					D2_PRINTABLE, D21_ORA_CTRY_N);
	}
	if (s_attr->a_d_name != (str_NP_choice *)NULL)
	{
	  d2a146_copy_oradr_part(&scan, s_attr->a_d_name->str_val, 
					D2_PRINTABLE, D21_ORA_ADMD_N);
	}
	
	if (s_attr->n_addr != (string)NULL)
	{
	   p_oradr = (D21_OR_address_part *)scan;
	   p_oradr->d21_ora_tag = D21_ORA_X121_A;
	   scan += sizeof(D21_OR_address_part); 
	   p_oradr->d21_ora_len = strlen(s_attr->n_addr)+1;
	   p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	   strcpy((char *)scan, s_attr->n_addr);
	   for (i=p_oradr->d21_ora_len-1; i < p_oradr->d21_ora_off; i++)
		*(scan + i) = '\0';
	   scan += p_oradr->d21_ora_off;
	 }

	if (s_attr->t_id != (string)NULL)
	 {
	   p_oradr = (D21_OR_address_part *)scan;
	   p_oradr->d21_ora_tag = D21_ORA_T_ID;
	   scan += sizeof(D21_OR_address_part); 
	   p_oradr->d21_ora_len = strlen(s_attr->t_id)+1;
	   p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	   strcpy((char *)scan, s_attr->t_id);
	   for (i=p_oradr->d21_ora_len-1; i < p_oradr->d21_ora_off; i++)
		*(scan + i) = '\0';
	   scan += p_oradr->d21_ora_off;
	   
	  }

	if (s_attr->p_d_name != (str_NP_choice *) NULL)
	  d2a146_copy_oradr_part(&scan, s_attr->p_d_name->str_val, D2_PRINTABLE,
							D21_ORA_PRMD_N);

	if (or_address->e_attrs != (E_Attrs *)NULL)
	{
	  p_oradr = (D21_OR_address_part *)scan;
	  scan += sizeof(D21_OR_address_part);  /* save pointer value */
	  *act_apdu = scan; 
	  if ((i = d2a147_get_e_attr_idx(or_address,common_name)) 
			!= UB_MAX_EXT_ATTRS)
	  {
	    d2a144_copy_str(&scan, or_address->e_attrs->v[i].e_value.P_str,
				 D2_PRINTABLE);
	  }  
	  if ((i = d2a147_get_e_attr_idx(or_address,
				teletex_common_name)) != UB_MAX_EXT_ATTRS)
	  {
	    d2a144_copy_str(&scan, or_address->e_attrs->v[i].e_value.T_str,
				 D2_T61);
	  }  
	  if ((scan - *act_apdu) != 0)
	  {
	    	p_oradr->d21_ora_tag = D21_ORA_COM_N;
	  	p_oradr->d21_ora_len = scan - *act_apdu;
	  	p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		p_oradr->d21_ora_fil = 0;
	   }
	  else
		scan -= sizeof(D21_OR_address_part); /* restore ptr value */
	 }

	i = d2a147_get_e_attr_idx(or_address, teletex_organization_name);

	if (s_attr->org_name != (string)NULL || i != UB_MAX_EXT_ATTRS)
	{
	   p_oradr = (D21_OR_address_part *)scan;
	   p_oradr->d21_ora_tag = D21_ORA_ORG_N;
	   scan += sizeof(D21_OR_address_part); 
	   *act_apdu = scan;  /* save pointer value */
	   if (s_attr->org_name != (string)NULL)
	    {
	  	d2a144_copy_str(&scan, s_attr->org_name, D2_PRINTABLE);
	     }
	   if (i != UB_MAX_EXT_ATTRS)
	     {
		d2a144_copy_str(&scan, or_address->e_attrs->v[i].e_value.T_str,
					D2_T61);
	      }
	    p_oradr->d21_ora_len = scan - *act_apdu;
	    p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	    p_oradr->d21_ora_fil = 0;
	  }
	if (s_attr->u_id != (string)NULL)
	 {
	   p_oradr = (D21_OR_address_part *)scan;
	   p_oradr->d21_ora_tag = D21_ORA_NUM_ID;
	   scan += sizeof(D21_OR_address_part); 
	   p_oradr->d21_ora_len = strlen(s_attr->u_id)+1;
	   p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	   p_oradr->d21_ora_fil = 0;
	   strcpy((char *)scan, s_attr->u_id);
	   for (i=p_oradr->d21_ora_len-1; i < p_oradr->d21_ora_off; i++)
		*(scan + i) = '\0';
	   scan += p_oradr->d21_ora_off;
	   
	  }
	i = d2a147_get_e_attr_idx(or_address, teletex_personal_name);
	if (s_attr->p_name != (P_Name *)NULL || i != UB_MAX_EXT_ATTRS)
	 {
	   p_oradr = (D21_OR_address_part *)scan;
	   p_oradr->d21_ora_tag = D21_ORA_SURNAME;
	   scan += sizeof(D21_OR_address_part); 
	   *act_apdu = scan;  /* save pointer value */
	   if (s_attr->p_name != (P_Name *)NULL)
	    {
		p_name = s_attr->p_name;
	  	d2a144_copy_str(&scan, s_attr->p_name->surname, D2_PRINTABLE);
	     }
	   if (i != UB_MAX_EXT_ATTRS)
	     {
		p1_name = &or_address->e_attrs->v[i].e_value.pn_val;
		d2a144_copy_str(&scan, p1_name->surname, D2_T61);
	      }
	   p_oradr->d21_ora_len = scan - *act_apdu;
	   p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	   p_oradr->d21_ora_fil = 0;
	   if ((p_name != (P_Name *)NULL && p_name->given_name != (string)NULL)
	   	|| (p1_name != (P_Name *)NULL && p1_name->given_name != (string)NULL))
	    {
	      p_oradr = (D21_OR_address_part *)scan;
	      p_oradr->d21_ora_tag = D21_ORA_GIV_N;
	      scan += sizeof(D21_OR_address_part); 
	      *act_apdu = scan;  /* save pointer value */
	      if (p_name != (P_Name *)NULL && p_name->given_name != (string)NULL)
	       {
	  	d2a144_copy_str(&scan, p_name->given_name, D2_PRINTABLE);
	        }
	      if (p1_name != (P_Name *)NULL && p1_name->given_name != (string)NULL)
	        {
		   d2a144_copy_str(&scan, p1_name->given_name, D2_T61);
	         }
	      p_oradr->d21_ora_len = scan - *act_apdu;
	      p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	      p_oradr->d21_ora_fil = 0;
	     }
	   if ((p_name != (P_Name *)NULL && p_name->initials != (string)NULL)
	   	|| (p1_name != (P_Name *)NULL && p1_name->initials != (string)NULL))
	    {
	      p_oradr = (D21_OR_address_part *)scan;
	      p_oradr->d21_ora_tag = D21_ORA_INIT;
	      scan += sizeof(D21_OR_address_part); 
	      *act_apdu = scan;  /* save pointer value */
	      if (p_name != (P_Name *)NULL && p_name->initials != (string)NULL)
	       {
	  	d2a144_copy_str(&scan, p_name->initials, D2_PRINTABLE);
	        }
	      if (p1_name != (P_Name *)NULL && p1_name->initials != (string)NULL)
	        {
		   d2a144_copy_str(&scan, p1_name->initials, D2_T61);
	         }
	      p_oradr->d21_ora_len = scan - *act_apdu;
	      p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	      p_oradr->d21_ora_fil = 0;
	     }
	   if ((p_name != (P_Name *)NULL && p_name->g_qualifier != (string)NULL)
	   	|| (p1_name != (P_Name *)NULL && p1_name->g_qualifier != (string)NULL))
	    {
	      p_oradr = (D21_OR_address_part *)scan;
	      p_oradr->d21_ora_tag = D21_ORA_GEN;
	      scan += sizeof(D21_OR_address_part); 
	      *act_apdu = scan;  /* save pointer value */
	      if (p_name != (P_Name *)NULL && p_name->g_qualifier != (string)NULL)
	       {
	  	d2a144_copy_str(&scan, p_name->g_qualifier, D2_PRINTABLE);
	        }
	      if (p1_name != (P_Name *)NULL && p1_name->g_qualifier != (string)NULL)
	        {
		   d2a144_copy_str(&scan, p1_name->g_qualifier, D2_T61);
	         }
	      p_oradr->d21_ora_len = scan - *act_apdu;
	      p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	      p_oradr->d21_ora_fil = 0;
	     }
	  }
	 i = d2a147_get_e_attr_idx(or_address, 
					teletex_organizational_unit_names);
	 if (s_attr->o_u_names != (O_U_Names *)NULL || i != UB_MAX_EXT_ATTRS)
	  {
	      j = (s_attr->o_u_names == (O_U_Names *)NULL) ? 
			or_address->e_attrs->v[i].e_value.ou_val.n : 
		  (i == UB_MAX_EXT_ATTRS) ? 
			s_attr->o_u_names->n :
		  (s_attr->o_u_names->n > or_address->e_attrs->v[i].e_value.ou_val.n) ?
		  	s_attr->o_u_names->n : or_address->e_attrs->v[i].e_value.ou_val.n;

	      if (i != UB_MAX_EXT_ATTRS)
		ou_names = &or_address->e_attrs->v[i].e_value.ou_val;

	      for (k=0; k < j; k++)
	      {
	      	p_oradr = (D21_OR_address_part *)scan;
	      	p_oradr->d21_ora_tag = D21_ORA_ORGU_N1+k;
	      	scan += sizeof(D21_OR_address_part); 
	      	*act_apdu = scan;  /* save pointer value */
	      	if (s_attr->o_u_names != (O_U_Names *)NULL && k < s_attr->o_u_names->n)
	       	{
	  	  d2a144_copy_str(&scan, s_attr->o_u_names->v[k], D2_PRINTABLE);
	        }
	      	if (ou_names != (O_U_Names *)NULL && k < ou_names->n)
	        {
		   d2a144_copy_str(&scan, ou_names->v[k], D2_T61);
	         }
	      	p_oradr->d21_ora_len = scan - *act_apdu;
	      	p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		p_oradr->d21_ora_fil = 0;
	     	}
	   }

	 i = d2a147_get_e_attr_idx(or_address, 
					teletex_domain_defined_attributes);
	 dd_attr = or_address->d_d_attrs;
	 if (dd_attr != (D_D_Attrs *)NULL || i != UB_MAX_EXT_ATTRS)
	  {
	      j = (dd_attr == (D_D_Attrs *)NULL) ? 
			or_address->e_attrs->v[i].e_value.dd_val.n : 
		  (i == UB_MAX_EXT_ATTRS) ? 
			dd_attr->n :
		  (dd_attr->n > or_address->e_attrs->v[i].e_value.dd_val.n) ?
		  	dd_attr->n : or_address->e_attrs->v[i].e_value.dd_val.n;

	      if (i != UB_MAX_EXT_ATTRS)
		dd_attr = &or_address->e_attrs->v[i].e_value.dd_val;
	      else
		dd_attr = (D_D_Attrs *)NULL;

	      for (k=0; k < j; k++)
	      {
	      	p_oradr = (D21_OR_address_part *)scan;
	      	p_oradr->d21_ora_tag = D21_ORA_DOM_T1+2*k;
	      	scan += sizeof(D21_OR_address_part); 
	      	*act_apdu = scan;  /* save pointer value */
	      	if (or_address->d_d_attrs != (D_D_Attrs *)NULL && k < or_address->d_d_attrs->n)
	       	{
	  	  d2a144_copy_str(&scan, or_address->d_d_attrs->v[k].type, D2_PRINTABLE);
	        }
	      	if (dd_attr != (D_D_Attrs *)NULL && k < dd_attr->n)
	        {
		   d2a144_copy_str(&scan, dd_attr->v[k].type, D2_T61);
	         }
	      	p_oradr->d21_ora_len = scan - *act_apdu;
	      	p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		p_oradr->d21_ora_fil = 0;
	      	p_oradr = (D21_OR_address_part *)scan;
	      	p_oradr->d21_ora_tag = D21_ORA_DOM_V1+2*k;
	      	scan += sizeof(D21_OR_address_part); 
	      	*act_apdu = scan;  /* save pointer value */
	      	if (or_address->d_d_attrs != (D_D_Attrs *)NULL && k < or_address->d_d_attrs->n)
	       	{
	  	   d2a144_copy_str(&scan, or_address->d_d_attrs->v[k].value, D2_PRINTABLE);
	        }
	      	if (dd_attr != (D_D_Attrs *)NULL && k < dd_attr->n)
	        {
		   d2a144_copy_str(&scan, dd_attr->v[k].value, D2_T61);
	         }
	      	p_oradr->d21_ora_len = scan - *act_apdu;
	      	p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		p_oradr->d21_ora_fil = 0;
	      }
	    }

	  i = d2a147_get_e_attr_idx(or_address, pds_name);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	      	p_oradr = (D21_OR_address_part *)scan;
	      	p_oradr->d21_ora_tag = D21_ORA_P_D_S_N;
	      	scan += sizeof(D21_OR_address_part); 
		p_str = or_address->e_attrs->v[i].e_value.P_str;
		p_oradr->d21_ora_len = strlen(p_str)+1;
		p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		p_oradr->d21_ora_fil = 0;
		strcpy((char *)scan, p_str);
	   	for (i=p_oradr->d21_ora_len-1;i<p_oradr->d21_ora_off; i++)
			*(scan + i) = '\0';
		scan += p_oradr->d21_ora_off;
	    }

	  i = d2a147_get_e_attr_idx(or_address, physical_delivery_country_name);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	    if (or_address->e_attrs->v[i].e_value.NP_val.mode == NUMERIC)
	    	d2a146_copy_oradr_part(&scan, or_address->e_attrs->v[i].e_value.NP_val.str_val,
	 		D2_NUMERIC, D21_ORA_P_CTRY);
	     else
	    	d2a146_copy_oradr_part(&scan, or_address->e_attrs->v[i].e_value.NP_val.str_val,
	 		D2_PRINTABLE, D21_ORA_P_CTRY);
	    }

	  i = d2a147_get_e_attr_idx(or_address, postal_code);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	    if (or_address->e_attrs->v[i].e_value.NP_val.mode == NUMERIC)
		d2a146_copy_oradr_part(&scan, or_address->e_attrs->v[i].e_value.NP_val.str_val,
	 		D2_NUMERIC, D21_ORA_P_CODE);
	     else
		d2a146_copy_oradr_part(&scan, or_address->e_attrs->v[i].e_value.NP_val.str_val,
	 		D2_PRINTABLE, D21_ORA_P_CODE);
	    }

	  i = d2a147_get_e_attr_idx(or_address, physical_delivery_office_name);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_O_N);
	    }

	  i = d2a147_get_e_attr_idx(or_address, physical_delivery_office_number);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_O_NUM);
	    }

	  i = d2a147_get_e_attr_idx(or_address, extension_OR_address_components);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_A_D);
	    }

	  i = d2a147_get_e_attr_idx(or_address, physical_delivery_personal_name);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_P_N);
	    }

	  i = d2a147_get_e_attr_idx(or_address, physical_delivery_organization_name);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_ORG_N);
	    }

	  i = d2a147_get_e_attr_idx(or_address, extension_physical_delivery_address_components);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_P_D);
	    }

	  i = d2a147_get_e_attr_idx(or_address, unformatted_postal_address);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	     if (or_address->e_attrs->v[i].e_value.up_addr.teletex_string != (string) NULL)
	      {
	      	p_oradr = (D21_OR_address_part *)scan;
	      	p_oradr->d21_ora_tag = D21_ORA_P_A_F;
	      	scan += sizeof(D21_OR_address_part); 
		p_str = or_address->e_attrs->v[i].e_value.up_addr.teletex_string;
		p_oradr->d21_ora_len = strlen(p_str)+1;
		p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		p_oradr->d21_ora_fil = 0;
		strcpy((char *)scan, p_str);
	   	for (j=p_oradr->d21_ora_len-1; j < p_oradr->d21_ora_off; j++)
			*(scan + j) = '\0';
		scan += p_oradr->d21_ora_off;
	       }
	     if (or_address->e_attrs->v[i].e_value.up_addr.p_address != (PostalAddress_Lines *) NULL)
	      {
	      	p_oradr = (D21_OR_address_part *)scan;
	      	p_oradr->d21_ora_tag = D21_ORA_P_A_L;
	      	scan += sizeof(D21_OR_address_part); 
		p_str1 = or_address->e_attrs->v[i].e_value.up_addr.p_address->v;
		for (k=0,p_oradr->d21_ora_len=0; 
	 	k < or_address->e_attrs->v[i].e_value.up_addr.p_address->n; k++)
		{
		  p_oradr->d21_ora_len += strlen(p_str1[k])+1;
		  strcpy((char *)scan, p_str1[k]);
		  scan += strlen(p_str1[k])+1;
	         }
		p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		p_oradr->d21_ora_fil = 0;
	   	for (i=p_oradr->d21_ora_len; i < p_oradr->d21_ora_off; i++)
			*(scan ++) = '\0';
	       }
	    }

	  i = d2a147_get_e_attr_idx(or_address, street_address);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_S_A);
	    }

	  i = d2a147_get_e_attr_idx(or_address, post_office_box_address);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_O_B_N);
	    }

	  i = d2a147_get_e_attr_idx(or_address, poste_restante_address);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_G_D_A);
	    }

	  i = d2a147_get_e_attr_idx(or_address, unique_postal_name);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_D_P_N);
	    }

	  i = d2a147_get_e_attr_idx(or_address, local_postal_attributes);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	  	d2a148_copy_pds_parameter(&scan, &or_address->e_attrs->v[i].e_value.pds_val, D21_ORA_P_LOC);
	    }

	  i = d2a147_get_e_attr_idx(or_address, extended_network_address);
	  if (i != UB_MAX_EXT_ATTRS)
	   {
	     if (or_address->e_attrs->v[i].e_value.en_addr.typ == E163_ADDRESS_TYPE)
	      {
	   	p_oradr = (D21_OR_address_part *)scan;
	   	p_oradr->d21_ora_tag = D21_ORA_ISDN_N;
	   	scan += sizeof(D21_OR_address_part); 
	   	p_oradr->d21_ora_len = strlen(or_address->e_attrs->v[i].e_value.en_addr.v.e163_4_address->number)+1;
	   	p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		p_oradr->d21_ora_fil = 0;
	   	strcpy((char *)scan, or_address->e_attrs->v[i].e_value.en_addr.v.e163_4_address->number);
	   	for (j=p_oradr->d21_ora_len-1;j<p_oradr->d21_ora_off; j++)
			*(scan + j) = '\0';
	   	scan += p_oradr->d21_ora_off;
	  	 
	     	if (or_address->e_attrs->v[i].e_value.en_addr.v.e163_4_address->sub_address != (string) NULL)
	      	 {
	   	   p_oradr = (D21_OR_address_part *)scan;
	   	   p_oradr->d21_ora_tag = D21_ORA_ISDN_S;
	   	   scan += sizeof(D21_OR_address_part); 
	   	   p_oradr->d21_ora_len = strlen(or_address->e_attrs->v[i].e_value.en_addr.v.e163_4_address->sub_address)+1;
	   	   p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
		   p_oradr->d21_ora_fil = 0;
	   	   strcpy((char *)scan, or_address->e_attrs->v[i].e_value.en_addr.v.e163_4_address->sub_address);
	   	   for(j=p_oradr->d21_ora_len-1;j<p_oradr->d21_ora_off;j++)
			   *(scan + j) = '\0';
	   	   scan += p_oradr->d21_ora_off;
	  	 
		  }
		}
	       else
	       {
	   	p_oradr = (D21_OR_address_part *)scan;
	   	p_oradr->d21_ora_tag = D21_ORA_PRES_A;
	   	scan += sizeof(D21_OR_address_part); 
		psap = or_address->e_attrs->v[i].e_value.en_addr.v.psap_address;
  		sel_syntax = SEL_SETSYNTAX(SEL_BINARY, SEL_BINARY, SEL_BINARY);
    		if ((len = d2p_encode_psap_addr(
		(psap->psel_defined == TRUE)?(Psap_info *)(&psap->psel):(Psap_info *) NULL,
		(psap->ssel_defined == TRUE)?(Psap_info *)(&psap->ssel):(Psap_info *) NULL,
		(psap->tsel_defined == TRUE)?(Psap_info *)(&psap->tsel):(Psap_info *) NULL,
	     		sel_syntax,psap->nb_nsap, (Psap_info *)(psap->nsap),
								scan)) < 0)
       		  {   
	   		D2_ASN1_ERR_RETURN_WITH_TRACE
	 	  }
		p_oradr->d21_ora_len = len;
		p_oradr->d21_ora_off = D2_ALIGN(len);
		p_oradr->d21_ora_fil = 0;
		scan += p_oradr->d21_ora_off;
		}
	    }

	  i = d2a147_get_e_attr_idx(or_address, terminal_type);
	  if (i != UB_MAX_EXT_ATTRS)
	  {
	  	p_oradr = (D21_OR_address_part *)scan;
	  	p_oradr->d21_ora_tag = D21_ORA_T_T;
	  	scan += sizeof(D21_OR_address_part); 
	  	p_oradr->d21_ora_len = sizeof(signed32);
		p_oradr->d21_ora_fil = 0;
	  	*(signed32 *)scan = or_address->e_attrs->v[i].e_value.ter_val;
	  	scan += p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	   }

	av_value->d23_avlen = scan - (byte *)(av_value +1);
	av_value->d23_avoff = D2_ALIGN(av_value->d23_avlen);

    *act_apdu = scan;
    return(D2_NOERROR);

 }

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a141_orname_to_ipc()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*                                                                    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*			D2_NOERROR : no error 			      */
/*			D2_ERROR   : error			      */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32  d2a141_orname_to_ipc(
  ORName 		*  or_name,
  byte 	        ** act_apdu,
  byte 		** s_apdu,
  signed32		*  apdu_len,
  byte 		*  err_apdu )
{
const char function_name[] = "d2a141_orname_to_ipc";
signed32 len = D23_S_ATV + sizeof(D21_OR_name);
signed32 rv , i;
signed16 h_len,h_off;
byte * scan ;
D23_av_value * av_value;
D23_av_value * av_value1;
D21_OR_name * ORname;
D23_ds_name * ds_name;

	len +=  d2a142_oradr_apdu_len(&or_name->address);
	if (or_name->d_name.name != (dua_dist_name *)NULL)
	 	len += sizeof(D21_OR_name) 
				 + D2_DNL_MAX;
	   
	 /* check memory length */
	rv = d2a033_check_memsize(len, s_apdu,
	 	   act_apdu,apdu_len,(D23_rserror *)err_apdu,apdu_len);
	if (rv != D2_NOERROR)
	 {
	 	/* not enough memory for attribute value */
	 	D2_ASN1_ERR_RETURN_WITH_TRACE
	  } /* if (rv != D2_NOERROR) */

	scan = *act_apdu;
	av_value = (D23_av_value *)scan;
	scan += D23_S_ATV;
	ds_name = (D23_ds_name *)scan;
	ORname = (D21_OR_name *)scan;
	if (or_name->d_name.name != (dua_dist_name *)NULL)
	 {
	   if (d2a017_dn_to_str(D23_NAMTAG, or_name->d_name.name, &scan,
		 s_apdu, apdu_len, err_apdu, apdu_len) == D2_ERROR)
		{
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		 }

	   h_len = ds_name->d23_dslen;
	   h_off = ds_name->d23_dsoff;
	   ORname->d21_orn_len = h_len;
	   ORname->d21_orn_off = h_off;
	   ORname->d21_orn_tag = D21_ORN_DN;
	   ORname->d21_orn_fil = 0;
	   for (i=1; i<=(ORname->d21_orn_off-ORname->d21_orn_len); i++)
		*(scan-i) = '\0';
	  }

	ORname = (D21_OR_name *)scan;
	av_value1 = (D23_av_value *)scan;

	if (d2a140_oradr_to_ipc(&or_name->address, &scan, s_apdu, apdu_len, 
				err_apdu) == D2_ERROR)
	  	{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}

	h_len = av_value1->d23_avlen;
	h_off = av_value1->d23_avoff;
	ORname->d21_orn_tag = D21_ORN_ORA;
	ORname->d21_orn_len = h_len;
	ORname->d21_orn_off = h_off;
	ORname->d21_orn_fil = 0;

	av_value->d23_avlen = (scan - *act_apdu) - D23_S_ATV;
	av_value->d23_avoff = D2_ALIGN(av_value->d23_avlen);

	*act_apdu = scan;
	return(D2_NOERROR);
 }

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a139_dlsp_to_ipc()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*                                                                    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*			D2_NOERROR : no error 			      */
/*			D2_ERROR   : error			      */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32  d2a139_dlsp_to_ipc(
  DLSubmitPermission  *  dls_perm,
  byte 	    ** act_apdu,
  byte 	    ** s_apdu,
  signed32		    *  apdu_len,
  byte 	    *  err_apdu )
{
const char function_name[] = "d2a139_dlsp_to_ipc";
  byte * scan;
  D21_DL_sub_perm * dlsub_perm;
  D23_av_value * av_val;
  ORName   * or_name = (ORName *)NULL;
  signed16 len;
  unsigned16 tag;
  signed32 rv, i;

	switch (dls_perm->typ) {
	   case MEMBER_OF_GROUP :
		tag = D21_DLSP_MEM_G;
		break;

	   case INDIVIDUAL :
		or_name = &dls_perm->v.individual;
		tag = D21_DLSP_INDIV;
		break;

	   case MEMBER_OF_DL :
		or_name = &dls_perm->v.member_of_dl;
		tag = D21_DLSP_MEM_DL;
		break;

	   case PATTERN_MATCH :
		or_name = &dls_perm->v.pattern_match;
		tag = D21_DLSP_PATT;
		break;

	} /* of switch */

	if (or_name != (ORName *)NULL)
	  {
		len =  D23_S_ATV + sizeof(D21_DL_sub_perm) + sizeof(D21_OR_name)
				 + d2a142_oradr_apdu_len(&or_name->address);
		if (or_name->d_name.name != (dua_dist_name *)NULL)
	 		len += sizeof(D21_OR_name) + D2_DNL_MAX;
	   }
	 else
	  {
		len = D23_S_ATV + sizeof(D21_DL_sub_perm) + D2_DNL_MAX;
	   }

	 /* check memory length */
	rv = d2a033_check_memsize(len, s_apdu,
		   	act_apdu,apdu_len,(D23_rserror *)err_apdu,apdu_len);
	if (rv != D2_NOERROR)
	{
		/* not enough memory for attribute value */
	 	D2_ASN1_ERR_RETURN_WITH_TRACE
	 } /* if (rv != D2_NOERROR) */

	scan = *act_apdu + D23_S_ATV;
	av_val = (D23_av_value *) *act_apdu;
	dlsub_perm = (D21_DL_sub_perm *)scan;

	if (or_name != (ORName *)NULL)
	  {
		if (d2a141_orname_to_ipc(or_name, &scan, 
					s_apdu, apdu_len, err_apdu) == D2_ERROR)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
	   dlsub_perm->d21_dlsp_len = (scan - *act_apdu) - D23_S_ATV 
						- sizeof(D21_DL_sub_perm);
	   dlsub_perm->d21_dlsp_off = D2_ALIGN(dlsub_perm->d21_dlsp_len);
	   dlsub_perm->d21_dlsp_fil = 0;
	   } /* of if */
	  else
	  {
	   if (d2a017_dn_to_str(D23_NAMTAG, dls_perm->v.member_of_group.name, &scan,
		 s_apdu, apdu_len, err_apdu, apdu_len) == D2_ERROR)
		{
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		 }

	   dlsub_perm->d21_dlsp_len = (scan - *act_apdu) - D23_S_ATV 
						- sizeof(D21_DL_sub_perm);
	   dlsub_perm->d21_dlsp_off = D2_ALIGN(dlsub_perm->d21_dlsp_len);
	   dlsub_perm->d21_dlsp_fil = 0;
	   for (i=1; i<=(dlsub_perm->d21_dlsp_off-dlsub_perm->d21_dlsp_len); i++)
		*(scan-i) = '\0';
	   }

	dlsub_perm->d21_dlsp_tag = tag;

	av_val->d23_avlen = (scan - *act_apdu) - D23_S_ATV;
	av_val->d23_avoff = D2_ALIGN(av_val->d23_avlen);
	*act_apdu = scan;
	return(D2_NOERROR);
 }

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a142_oradr_apdu_len()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*                                                                    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      ora         =  pointer to ORAddress Part                      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*	len	=	APDU length				      */
/*                      (without total offset and length)             */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Jochen Keutel                                             */
/**********************************************************************/


static signed32 d2a142_oradr_apdu_len(
  ORAddress *ora )
{
signed32 	i,j ;
signed32	len = 0 , helplen ;

signed32	m_org_n=0;
signed32	m_surname=0 , m_given_name=0 , m_initials=0 , m_g_qualifier=0 ;

signed32	m_orgu_n[4]	={0,0,0,0};
signed32	m_dom[4]	={0,0,0,0} ;

signed32	 m_e1=0 , m_e10=0 , m_e11=0 , m_e12=0 , m_e13=0 , m_e14 =0 ;
signed32	m_e15=0 , m_e17=0 , m_e18=0 , m_e19=0 , m_e20=0 , m_e21=0 ;


/*		Begin		*/


/*									*/
/*	Standard Attributes (1-9)					*/
/*									*/

/*	Country Name (P or N)		:	S1			*/

if ( ora->s_attrs.c_name != (str_NP_choice *) NULL )
	len	+=	sizeof (D21_OR_address_part)
		+	sizeof (D21_str_list)
		+	D2_ALIGN (strlen (ora->s_attrs.c_name->str_val)+1) ;

/*	ADMD Name (P or N)		:	S2			*/

if ( ora->s_attrs.a_d_name != (str_NP_choice *) NULL )
	len	+=	sizeof (D21_OR_address_part)
		+	sizeof (D21_str_list)
		+	D2_ALIGN (strlen (ora->s_attrs.a_d_name->str_val)+1) ;

/*	X121 Address (N)		:	S3			*/

if ( ora->s_attrs.n_addr != (string) NULL )
	len	+=	sizeof (D21_OR_address_part)
		+	D2_ALIGN (strlen (ora->s_attrs.n_addr)+1) ;

/*	Terminal Identifier (P)		:	S4			*/

if ( ora->s_attrs.t_id != (string) NULL )
	len	+=	sizeof (D21_OR_address_part)
		+	D2_ALIGN (strlen (ora->s_attrs.t_id)+1) ;

/*	PRMD Name (P or N)		:	S5			*/

if ( ora->s_attrs.p_d_name != (str_NP_choice *) NULL )
	len	+=	sizeof (D21_OR_address_part)
		+	sizeof (D21_str_list)
		+	D2_ALIGN (strlen (ora->s_attrs.p_d_name->str_val)+1) ;

/*	Organization Name (P)		:	S6			*/

if ( ora->s_attrs.org_name != (string) NULL ) 
  {
	len	+=	sizeof (D21_OR_address_part)
		+	sizeof (D21_str_list)
		+	D2_ALIGN (strlen (ora->s_attrs.org_name)+1) ;
	m_org_n = 1;
   }

/*	Numeric User Identifier (N)	:	S7			*/

if ( ora->s_attrs.u_id != (string) NULL )
	len	+=	sizeof (D21_OR_address_part)
		+	D2_ALIGN (strlen (ora->s_attrs.u_id)+1) ;

/*	Personal Name (P)		:	S8			*/

if ( ora->s_attrs.p_name != (P_Name *) NULL )
  {
						/* Surname 	: S8	*/

	if ( ora->s_attrs.p_name->surname != (string) NULL )
	  {
		len	+=	sizeof (D21_OR_address_part)
			+	sizeof (D21_str_list)
			+	D2_ALIGN (strlen (ora->s_attrs.p_name->surname)+1) ;
		m_surname = 1;
	   }

						/* Given Name	: S8	*/

	if ( ora->s_attrs.p_name->given_name != (string) NULL )
	  {	
		len	+=	sizeof (D21_OR_address_part)
			+	sizeof (D21_str_list)
			+	D2_ALIGN (strlen (ora->s_attrs.p_name->given_name)+1) ;
		m_given_name = 1;
	   }

						/* Initials	: S8	*/

	if ( ora->s_attrs.p_name->initials != (string) NULL )
	  {	
		len	+=	sizeof (D21_OR_address_part)
			+	sizeof (D21_str_list)
			+	D2_ALIGN (strlen (ora->s_attrs.p_name->initials)+1) ;
		m_initials = 1;
	   }

						/* Generation	: S8	*/

	if ( ora->s_attrs.p_name->g_qualifier != (string) NULL )
	  {
		len	+=	sizeof (D21_OR_address_part)
			+	sizeof (D21_str_list)
			+	D2_ALIGN (strlen (ora->s_attrs.p_name->g_qualifier)+1) ;
		m_g_qualifier = 1;
	   }
   }						/* End of Personal Name	*/

/*	Organisational Unit Name (1-4) (P)	:	S9		*/

if (ora->s_attrs.o_u_names != (O_U_Names *)NULL )

   for ( i = 1 ; i <= ora->s_attrs.o_u_names->n ; i++ )
     {
	   len	+=	sizeof (D21_OR_address_part)
		+	sizeof (D21_str_list)
		+	D2_ALIGN (strlen (ora->s_attrs.o_u_names->v[i-1])+1) ;
	   m_orgu_n[i-1] = 1;
      }

/*									*/
/*	Domain Defined Attributes					*/
/*									*/

/*	Domain Type / Value (1-4) (P)	:	DT / DV			*/

if (ora->d_d_attrs != (D_D_Attrs *)NULL )

   for ( i = 1 ; i <= ora->d_d_attrs->n ; i++ )
     {
	   len	+=  2 *	sizeof (D21_OR_address_part)
		+   2 *	sizeof (D21_str_list)
		+	D2_ALIGN (strlen (ora->d_d_attrs->v[i-1].type)+1)
		+	D2_ALIGN (strlen (ora->d_d_attrs->v[i-1].value)+1) ;
	   m_dom[i-1] = 1;
      }

/*									*/
/*	Extended Attributes (1-23)					*/
/*									*/

if (ora->e_attrs != (E_Attrs *)NULL )
{

for ( i = 1 ; i <= ora->e_attrs->n ; i++ )
  {
				/* Common Name (P)		: E1	*/

	if ( ora->e_attrs->v[i-1].e_type == common_name )
	  {
		len	+=	sizeof (D21_OR_address_part)
			+	sizeof (D21_str_list)
			+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.P_str)+1) ;
		m_e1 = 1;
	   }

				/* Common Name (T)		: E2	*/

	if ( ora->e_attrs->v[i-1].e_type == teletex_common_name )
	  {
		if (m_e1 == 0) len += sizeof (D21_OR_address_part);
		len	+=	sizeof (D21_str_list)
			+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.T_str)+1) ;
	   }

				/* Organization Name (T)	: E3	*/

	if ( ora->e_attrs->v[i-1].e_type == teletex_organization_name )
	  {
		if ( m_org_n == 0 ) len += sizeof (D21_OR_address_part) ;
		len	+=	sizeof (D21_str_list)
			+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.T_str)+1) ;
	   }

				/* Personal Name (T)		: E4	*/

	if ( ora->e_attrs->v[i-1].e_type == teletex_personal_name )
	  {
						/*	Surname	: E4	*/

		if ( ora->e_attrs->v[i-1].e_value.pn_val.surname != (string) NULL )
		  {
			if ( m_surname == 0 ) len += sizeof (D21_OR_address_part) ;
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pn_val.surname)+1);
		   }

						/* Given Name	: E4	*/

		if ( ora->e_attrs->v[i-1].e_value.pn_val.given_name != (string) NULL )
		  {
			if ( m_given_name == 0 ) len += sizeof (D21_OR_address_part) ;
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pn_val.given_name)+1);
		   }

						/* Initials	: E4	*/

		if ( ora->e_attrs->v[i-1].e_value.pn_val.initials != (string) NULL )
		  {
			if ( m_initials == 0 ) len += sizeof (D21_OR_address_part) ;
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pn_val.initials)+1);
		   }

						/* Generation	: E4	*/

		if ( ora->e_attrs->v[i-1].e_value.pn_val.g_qualifier != (string) NULL )
		  {
			if ( m_g_qualifier == 0 ) len += sizeof (D21_OR_address_part) ;

			len	+=	sizeof (D21_str_list)
				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pn_val.g_qualifier)+1);
		}
	   }				/* End of Teletex Personal Name	*/

			/* Organizational Unit Name (1-4) (T)	: E5	*/

	if ( ora->e_attrs->v[i-1].e_type == teletex_organizational_unit_names )
		for ( j = 1 ; j <= ora->e_attrs->v[i-1].e_value.ou_val.n ; j++ )
		  {
			if ( m_orgu_n[j-1] == 0 ) len += sizeof (D21_OR_address_part) ;
			len	+=	sizeof (D21_str_list)
				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.ou_val.v[j-1])+1) ;
		   }

			/* Domain Type/Value (1-4) (T)		: E6	*/

	if ( ora->e_attrs->v[i-1].e_type == teletex_domain_defined_attributes )
		for ( j = 1 ; j <= ora->e_attrs->v[i-1].e_value.dd_val.n ; j++ )
		  {
			if ( m_dom[j-1] == 0 ) len += 2 * sizeof (D21_OR_address_part) ;
			len	+=  2 *	sizeof (D21_str_list)
				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.dd_val.v[j-1].type)+1)
				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.dd_val.v[j-1].value)+1) ;
		   }

			/* Postal Delivery System Name (P) 	: E7	*/

	if ( ora->e_attrs->v[i-1].e_type ==  pds_name )
		len	+=	sizeof (D21_OR_address_part)
			+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.P_str)+1) ;

			/* Postal Country Name (P or N)		: E8	*/

	if ( ora->e_attrs->v[i-1].e_type ==  physical_delivery_country_name )
		len	+=	sizeof (D21_OR_address_part)
    			+	sizeof (D21_str_list)
    			+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.NP_val.str_val)+1) ;

			/* Postal Code (P or N)			: E9	*/

	if ( ora->e_attrs->v[i-1].e_type ==  postal_code )
		len	+=	sizeof (D21_OR_address_part)
    			+	sizeof (D21_str_list)
    			+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.NP_val.str_val)+1) ;



		/* Postal Office Name (P or T or both)		: E10	*/

	if ( ora->e_attrs->v[i-1].e_type == physical_delivery_office_name )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e10 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e10 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E10	*/

		/* Postal Office Number (P or T or both)	: E11	*/

	if ( ora->e_attrs->v[i-1].e_type == physical_delivery_office_number )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e11 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e11 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E11	*/

		/* Postal Office Details (P or T or both)	: E12	*/

	if ( ora->e_attrs->v[i-1].e_type == extension_OR_address_components )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e12 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e12 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E12	*/

		/* Postal Patron Name (P or T or both)		: E13	*/

	if ( ora->e_attrs->v[i-1].e_type == physical_delivery_personal_name )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e13 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e13 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E13	*/

		/* Postal Organization Name (P or T or both)	: E14	*/

	if ( ora->e_attrs->v[i-1].e_type == physical_delivery_organization_name )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e14 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e14 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E14	*/

		/* Postal Patron Details (P or T or both)	: E15	*/

	if ( ora->e_attrs->v[i-1].e_type == extension_physical_delivery_address_components )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e15 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e15 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E15	*/

		/* Postal Address in Full (T)			: E16	*/
		/* Postal Address in Lines (P)			: E16	*/

	if ( ora->e_attrs->v[i-1].e_type == unformatted_postal_address )
	  {
					/* Postal Address in Full	*/

		if ( ora->e_attrs->v[i-1].e_value.up_addr.teletex_string != (string) NULL )
			len	+=	sizeof (D21_OR_address_part)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.up_addr.teletex_string)+1) ;

					/* Postal Address in Lines	*/

		if ( ora->e_attrs->v[i-1].e_value.up_addr.p_address != (PostalAddress_Lines *) NULL)

		if ( ora->e_attrs->v[i-1].e_value.up_addr.p_address->n > 0 )
		  {
			len	+=	sizeof (D21_OR_address_part) ;
			helplen = 0;
			for ( j=1 ; j <= ora->e_attrs->v[i-1].e_value.up_addr.p_address->n ; j++)
				helplen += strlen (ora->e_attrs->v[i-1].e_value.up_addr.p_address->v[j-1]) + 1;
			len	+=	D2_ALIGN (helplen);
		   }
	   }						/* End of E16	*/

		/* Postal Street Address (P or T or both)	: E17	*/

	if ( ora->e_attrs->v[i-1].e_type == street_address )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e17 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e17 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E17	*/

		/* Postal Office Box Number (P or T or both)	: E18	*/

	if ( ora->e_attrs->v[i-1].e_type == post_office_box_address )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e18 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e18 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E18	*/

	/* Postal General Delivery Address (P or T or both)	: E19	*/

	if ( ora->e_attrs->v[i-1].e_type == poste_restante_address )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e19 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e19 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E19	*/

	/* Postal Delivery Point Name (P or T or both)		: E20	*/

	if ( ora->e_attrs->v[i-1].e_type == unique_postal_name )
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e20 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e20 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E20	*/

		/* Postal Locale (P or T or both)		: E21	*/

	if ( ora->e_attrs->v[i-1].e_type == local_postal_attributes)
	  {
		if ( ora->e_attrs->v[i-1].e_value.pds_val.printable != (string) NULL )
		  {
			m_e21 = 1;
			len	+=	sizeof (D21_OR_address_part)
    				+	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.printable)+1) ;
		   }

		if ( ora->e_attrs->v[i-1].e_value.pds_val.t61 != (string) NULL )
		  {
			if (m_e21 == 0) len += sizeof (D21_OR_address_part);
			len	+=	sizeof (D21_str_list)
    				+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.pds_val.t61)+1) ;
		   }
	   }						/* End of E21	*/

		/* Presentation Address (Psap)			: E22	*/
		/* ISDN Number (N)				: E22	*/
		/* ISDN Subaddress (N)				: E22	*/

	if ( ora->e_attrs->v[i-1].e_type == extended_network_address )
	  {
						/* Presentation Address	*/

		if ( ora->e_attrs->v[i-1].e_value.en_addr.typ == PSAP_ADDRESS_TYPE )
			len	+=	sizeof (D21_OR_address_part)
    				+	D2_ATL_MAX;

						/* E163 Address		*/

		if (ora->e_attrs->v[i-1].e_value.en_addr.typ == E163_ADDRESS_TYPE )
		  {
			if ( ora->e_attrs->v[i-1].e_value.en_addr.v.e163_4_address->number != (string) NULL )
				len	+=	sizeof (D21_OR_address_part)
    					+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.en_addr.v.e163_4_address->number)+1) ; 

			if ( ora->e_attrs->v[i-1].e_value.en_addr.v.e163_4_address->sub_address != (string) NULL )
				len	+=	sizeof (D21_OR_address_part)
    					+	D2_ALIGN (strlen (ora->e_attrs->v[i-1].e_value.en_addr.v.e163_4_address->sub_address)+1) ; 
		   }

	   }						/* End of E22	*/

		/* Terminal Type (Integer)			: E23	*/

	if (ora->e_attrs->v[i-1].e_type == terminal_type )
		len	+=	sizeof (D21_OR_address_part)
			+	D2_ALIGN (sizeof (TerminalType)) ;

   }					/* End of extension attributes	*/
}
return (len);
}			/* End of program : d2a142_oradr_apdu_len	*/


static void d2a144_copy_str(
  byte ** apdu,
  string str,
  unsigned16 rep )
{
  byte * scan = *apdu;
  D21_str_list * p_str;
  signed32 i;

	p_str = (D21_str_list *)scan;
	p_str->d21_strl_rep = rep;
	p_str->d21_strl_len = strlen(str)+1;
	p_str->d21_strl_off = D2_ALIGN(p_str->d21_strl_len);
	p_str->d21_strl_fil = 0;
	scan += sizeof(D21_str_list);
	strcpy((char *)scan, str);
	for (i=p_str->d21_strl_len-1; i < p_str->d21_strl_off; i++)
		*(scan + i) = '\0';
	scan += p_str->d21_strl_off;
	*apdu = scan;
}

static void d2a146_copy_oradr_part(
  byte ** apdu,
  string str,
  unsigned16 rep,
  unsigned16 tag )
{
  byte * scan = *apdu;
  D21_OR_address_part * p_oradr;

	  p_oradr = (D21_OR_address_part *)scan;
	  p_oradr->d21_ora_tag = tag;
	  scan += sizeof(D21_OR_address_part);
	  d2a144_copy_str(&scan, str, rep);
	  p_oradr->d21_ora_len = scan - *apdu - sizeof(D21_OR_address_part);
	  p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	  p_oradr->d21_ora_fil = 0;
	  *apdu = scan;
}

static void d2a148_copy_pds_parameter(
  byte 		**apdu,
  PDSParameter	* pds_val,
  unsigned16		  tag )
{
  byte * scan = *apdu;
  D21_OR_address_part * p_oradr;

      	p_oradr = (D21_OR_address_part *)scan;
      	p_oradr->d21_ora_tag = tag;
      	scan += sizeof(D21_OR_address_part); 
      	*apdu = scan;  /* save pointer value */
    	if (pds_val->printable != (string)NULL)
     	{
  	  d2a144_copy_str(&scan, pds_val->printable, D2_PRINTABLE);
        }
    	if (pds_val->t61 != (string)NULL)
     	{
  	  d2a144_copy_str(&scan, pds_val->t61, D2_T61);
        }
      	p_oradr->d21_ora_len = scan - *apdu;
      	p_oradr->d21_ora_off = D2_ALIGN(p_oradr->d21_ora_len);
	p_oradr->d21_ora_fil = 0;
	*apdu = scan;
 }

static signed32 d2a147_get_e_attr_idx(
  ORAddress * or_address,
  signed32 	type )
{
signed32 i;

	if (or_address->e_attrs == (E_Attrs *)NULL)
		return(UB_MAX_EXT_ATTRS);

	for (i=0; i < or_address->e_attrs->n; i++)
	{
	  if (or_address->e_attrs->v[i].e_type == type)
		return(i);
	 }
	return(UB_MAX_EXT_ATTRS);
 }
