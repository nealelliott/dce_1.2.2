/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20acl.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:24  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:43  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:43:29  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:23:00  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:10:11  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:08  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:08:11  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:13  marrek
 * 	Update for dce1.1.
 * 	[1994/02/09  14:47:34  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:27:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:54:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:26:15  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:49:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:37  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:08:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:06:25  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:51:59  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:14:48  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20acl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:26 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : acl.c       [Administrations-Schnittstelle]         */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 26.02.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 04.02.88| original                       | ws |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*		       Modulidentification			      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2hdlacl.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D20_ROOT    0

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask d20_6aomask[];        /* Input fields for mask 6a    */
extern char      *d20_6aifields[];     /* Mask 6a                     */

extern D20_omask d20_6bomask[];        /* Input fields for mask 6b    */
extern char      *d20_6bifields[];     /* Mask 6b                     */
extern char      *d20_opclass[];       /* operationclass              */

extern D20_omask d20_qomask[];         /* Input fields for error mask */
extern char      *d20_errtab[];        /* Error mask                  */

extern signed32   d2_nd20srt;
extern D20_srt   *d2_p_d20srt;

extern signed32   rbatch;
extern Bool	  d20_printable;

/**********************************************************************/
/*								      */
/*		  D E C	L A R A	T I O N	S  	 Functions 	      */
/*								      */
/**********************************************************************/

static D20_srt *ds0_509_rdn_search(signed32 no_nam_attr, char *nam_attr_type[],
				   signed32 par_rule_nr );

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_507_handle_acl        [handle_acl  ]              */
/*								      */
/* AUTHOR:      Schmid, D AP 11                                       */
/* DATE:        01.03.88                                              */
/*								      */
/* SYNTAX:      signed16 ds0_507_handle_acl(operation, init_flds,     */
/*					acl_len, acl_val, )           */
/*              char            operation                             */
/*              Bool            init_flds;                            */
/*              signed16        acl_len;                              */
/*              char           *acl_val;                              */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will display the ACL for each access    */
/*              right (if operation == MODIFY_ACL).                   */
/*              A new ACL can be entered.                             */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char            operation;      operation code        */
/*              Bool            init_flds;      a sign whether the    */
/*                                              mask fields must be   */
/*                                              initialized           */
/*              signed16        acl_len;        length of ACL-list    */
/*                                              (if operation is      */
/*                                              DS_DISPLAY_OBJECTS)   */
/*              char           *acl_val;        ACL_list              */
/*                                              (if operation is      */
/*                                              DS_DISPLAY_OBJECTS)   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR: if some error has occurred.		      */
/*		D2_NOERROR: otherwise.				      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

signed16 ds0_507_handle_acl(
char        operation,
Bool        init_flds,
signed16    acl_len,
char       *acl_val )
 
{                               /*  ds0_507_handle_acl                */
   Bool        dn_correct;

   signed16    ret_value;
   signed16    i, j;
   signed16    idx;
   signed16    acl_idx;
   signed16    anz;
   signed16    length;
   signed16    type = D20_ATYPE_ONLY | D20_ASTART | D20_AINSERT | D20_AEND;

   char        message[D20_MAXMSG];
   Ds_at_entry attr[2];

   char       *str;
   char       *max_str;
   char        old_acl[2 * D2_ACLIDX_MAX];

   char        new_acl[D2_ACLIDX_MAX * (D2_DNL_MAX + 2)];
					    /* DNs in ACL-list        */
   char        acl_names[D2_ACLIDX_MAX][D2_DNL_MAX + 1];
   char        old_names[D2_ACLIDX_MAX * ((D20_ATVAL_LEN * D20_T61_CONV_FACT) + 1)];
   char	   *old_nmptr = old_names;

   D20_p5      acl_par;
   D2_a_value  *attr_val;
   D20_aclinfo acl_info[D2_NOACCESS_RIGHTS];
   D20_aclinfo *p_acl_info;

   D20_obj     dns[D2_ACLIDX_MAX];      /* ACL-info (to be modified) */
   signed16    dnlen[D2_ACLIDX_MAX];    /* lengths of DNs            */
   signed16   *p_dnlen   = dnlen;
   D20_obj    *p_obj     = &dns[0];

   D20_obj    *p_max_obj = p_obj + D2_ACLIDX_MAX;

   D2_a_type        attr_type;

   static signed16    idx_typ[D2_NOIDX_ACL] =
				{ DS_DN3_TYP, DS_DN6_TYP, DS_DN9_TYP,
				  DS_DN12_TYP };



if                              /*  operation is ADD_OBJECT           */
   (operation == DS_ADD_OBJECT)
{                               /*  initialize ACL                    */
  for (i = 0, str = old_acl; i < D2_ACLIDX_MAX; i++)
  {
    *str++ = D2_IGNORE;
    *str++ = D2_EOS;
  }
  acl_par.d20_5c_acl = old_acl;
  acl_par.d20_5len   = 2 * D2_ACLIDX_MAX;
}
else
{                               
  if                            /*  operation is MODIFY-ACL           */
     (operation == DS_MODIFY_ATTRIBUTE)
  {                             /*  show ACL-mask (access classes)    */
    *d20_6aifields[DS_AC6_BREAK] = DS_DEFAULT_VALUE;
    if (init_flds == TRUE)
    {   
      for (i = DS_AC1_MOD_PUB; i <= DS_AC5_MOD_SEN; i++)
      { 
	*(signed32 *) d20_6aifields[i] = FALSE;
      } 
    } 

    d20_handlemask(d20_6aomask);

    if                          /*  "break" was selected              */
       (*d20_6aifields[DS_AC6_BREAK] != DS_DEFAULT_VALUE)
    {                           /*  return (D2_ERROR)                 */
      return(D2_ERROR);
    }                           

				/*  read ACL                          */
    attr[0].attr_id.d2_type      = (Ob_id_string)D2_ACL;
    attr[0].attr_id.d2_typ_len   = sizeof(D2_ACL) - 1;

    attr[0].a_val.d2_a_v_len = 0;
    ds4_b02_fill_attr(type, attr, 1);
    if                          /*  search for ACL failed             */
       (ds4_b11_call_directory(DS_READ_PROPERTY))
    {                           /*  return(D2_ERROR)                  */
      return(D2_ERROR);
    }
    else
    {                           /*  get result                        */
      if                        /*  attribute not found               */
	 (d20_get_attribute(D20_BEGIN, &attr_type, &attr_val))
      {                         /*  display error message             */
				/*  return(D2_ERROR)                  */
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E48IDX];
	d20_handlemask(d20_qomask);
	return(D2_ERROR);
      }
      else
      {
	acl_par.d20_5c_acl = (char *)(attr_val->d2_at_value.d2_oct_a_v);
	acl_par.d20_5len   = attr_val->d2_a_v_len;
      }                         
    }                           
  }
  else
  {                             /*  initialize ACL-length, ACL-Value  */
    acl_par.d20_5len = acl_len;
    acl_par.d20_5c_acl = acl_val;
  }                             

  str = acl_par.d20_5c_acl;
				/*  set pointer to ACL-names          */
  for                           /*  all ACL-names                     */
      (max_str = str + acl_par.d20_5len ;
       p_obj < p_max_obj && str < max_str; p_obj++, p_dnlen++)
  {                             
    if                          /*  ACL-interpretation is not correct */
       (*str != D2_SOBJ  && *str != D2_SUBTREE && *str != D2_GROUP &&
	*str != D2_IGNORE)
    {                           /*  display error message             */
				/*  return(D20_ACL_WRONG)             */
      sprintf(message, d20_errtab[D20_E43IDX], p_obj - &dns[0] + 1);
      d20_qomask[D20_Q1POS].d20_ioinfo = message;
      d20_handlemask(d20_qomask);
      return(D20_ACL_WRONG);
    }
    else
    {                           /*  store ACL-interpretation          */
      p_obj->dn_int = *str++;
      				/*  store ACL-name                    */
      strcpy(old_nmptr,str); 
      str += strlen(str) + 1;	
      p_obj->dn = (D2_name_string)old_nmptr;	
      if(d20_gen_dnintf((String) p_obj->dn) != D2_NOERROR)
		{
		d20_232_spec_exit();
		}
      old_nmptr += strlen(old_nmptr) + 1;
      *p_dnlen = strlen((char *)p_obj->dn) + 1;/* incl. EOS*/

    }                           
  }                             
  if                            /*  ACL-attribute is not correct      */
     (p_obj != p_max_obj || str != max_str)
  {                             /*  display error message             */
				/*  return(D20_ACL_WRONG)             */
    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E47IDX];
    d20_handlemask(d20_qomask);
    return(D20_ACL_WRONG);
  }                             
}                               

acl_par.d20_5no_rights = 0;
acl_par.d20_5acl = p_acl_info = &acl_info[0];
acl_par.d20_5max_len = sizeof(new_acl);
acl_par.d20_5n_acl = new_acl;
for                             /*  all possible access rights        */
    (i = DS_AC1_MOD_PUB; i <= DS_AC5_MOD_SEN; )
{ 
  if                            /*  access right selected             */
     (*(signed32 *) d20_6aifields[i] == TRUE || 
		operation != DS_MODIFY_ATTRIBUTE)
  {                             /*  initialize DN-mask                */
    d20_6bomask[D20_6A2POS].d20_ioinfo = d20_opclass[i];
    *d20_6bifields[DS_DN13_BREAK] = *d20_6bifields[DS_DN14_MORE] =
    *d20_6bifields[DS_DN16_SCU] = *d20_6bifields[DS_DN17_F1] =
							   DS_DEFAULT_VALUE;
    if (init_flds == TRUE || operation != DS_ADD_OBJECT || rbatch)
    {   
      *(signed32 *) d20_6bifields[DS_DN3_TYP] = *(signed32 *) d20_6bifields[DS_DN6_TYP] =
      *(signed32 *) d20_6bifields[DS_DN9_TYP] = *(signed32 *) d20_6bifields[DS_DN12_TYP] = (signed32) (D2_IGNORE - '0');
      for (j = 0; j < D2_NOIDX_ACL; j++)
      { 
	*d20_6bifields[3*j] = *d20_6bifields[3*j+1] = D2_EOS;
      } 
    } 

    if                          /*  operation is not ADD_OBJECT       */
       (operation != DS_ADD_OBJECT)
    {                           /*  put DNs in Distinguished Name mask*/
      p_acl_info->d20_5mod = D2_MOD_DN;
      p_obj = &dns[idx = i * D2_NOIDX_ACL];
      p_dnlen = &dnlen[idx];
      for (p_max_obj = p_obj + D2_NOIDX_ACL, idx = 0; p_obj < p_max_obj;
	   p_obj++, p_dnlen++, idx++)
      {                         
	switch ((int) p_obj->dn_int)
	{
	  case D2_SOBJ:     *(signed32 *) d20_6bifields[j = idx_typ[idx]] =
					      (signed32) (D2_SOBJ - '0');
			    break;
	  case D2_IGNORE:   *(signed32 *) d20_6bifields[j = idx_typ[idx]] =
					      (signed32) (D2_IGNORE - '0');
			    break;
	  case D2_SUBTREE:  *(signed32 *) d20_6bifields[j = idx_typ[idx]] =
					      (signed32) (D2_SUBTREE - '0');
			    break;
	}   

	if ((length = *p_dnlen) > ((D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT) - 1))
	{   
	  strncpy(d20_6bifields[j - 2],
	      (char *)p_obj->dn,
	      anz = (D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT));
	  length = ((length -= anz) > anz - 1) ? (anz - 1) : length;
	  strncpy(d20_6bifields[j - 1], (char *)(p_obj->dn + anz), length);
	  d20_6bifields[j - 1][length] = D2_EOS;
	}
	else
	{
	  strncpy(d20_6bifields[j - 2], (char *)p_obj->dn, length);
	  d20_6bifields[j - 2][length] = *d20_6bifields[j - 1] = D2_EOS;
	} 
      }                         
    }
    else
    {                           /*  modification type = MOD_DN        */
      p_acl_info->d20_5mod = D2_MOD_DN;
      init_flds = FALSE;
    }                           

    p_acl_info->d20_5aright = i;
    for                         /*  input is not correct              */
	(dn_correct = FALSE; dn_correct == FALSE; )
    {                           /*  show Distinguished Name mask      */
      d20_handlemask(d20_6bomask);

      if                        /*  "break" or F1 or SCROLL_DOWN or   */
				/*  SCROLL_UP was selected            */
	 (*d20_6bifields[DS_DN13_BREAK] != DS_DEFAULT_VALUE ||
	  *d20_6bifields[DS_DN14_MORE] != DS_DEFAULT_VALUE ||
	  *d20_6bifields[DS_DN16_SCU] != DS_DEFAULT_VALUE ||
	  *d20_6bifields[DS_DN17_F1] != DS_DEFAULT_VALUE)
      {                         /*  return(D2_ERROR)                  */
	return(D2_ERROR);
      }
      else if                 /*  operation is DISPLAY_OBJECTS      */
	    (operation == DS_DISPLAY_OBJECTS)
      {                         /*  break                             */
	i++;
	break;
      }
      else
      {                         /*  check input                       */
	p_acl_info->d20_5dn_count = 0;
	p_obj = &(p_acl_info->d20_5dns[0]);
	for                     /*  all DNs                           */
	    (p_max_obj = p_obj + D2_NOIDX_ACL, acl_idx = i * D2_NOIDX_ACL,
	     idx = 0, dn_correct = TRUE, anz = 0;
	     p_obj < p_max_obj && dn_correct == TRUE;
	     p_obj++, acl_idx++, idx++)
	{                       /*  determine DN-length               */
	  strcpy((char *)(p_obj->dn = (D2_name_string)(&acl_names[acl_idx][0])),
		 d20_6bifields[j = idx_typ[idx] - 2]);
	  strcpy((char *)(p_obj->dn + (D20_ATVAL_FLD_LEN * D20_T61_CONV_FACT)), 
		 d20_6bifields[j + 1]);
	  if                    /*  length not correct                */
	     ((length = d20_strlen((char *) p_obj->dn)) > D2_DNL_MAX)
	  {                     /*  show error message                */
	    sprintf(message, d20_errtab[D20_E46IDX], idx + 1);
	    d20_qomask[D20_Q1POS].d20_ioinfo = message;
	    d20_handlemask(d20_qomask);
	    dn_correct = FALSE;
	  }
          else if             /*  length == 0                       */
		(!length)
	  {                     
	    if                  /*  interpretation is '_'             */
	       (*(signed32 *) d20_6bifields[idx_typ[idx]] ==
				 (signed32) (D2_IGNORE - '0'))
	    {                   /*  ACL entry == IGNORE               */
	      p_obj->dn_int = D2_IGNORE;
	      *(p_obj->dn) = D2_EOS;
	    }
	    else if           /*  interpretation is single object   */
		  (*(signed32 *) d20_6bifields[idx_typ[idx]] == 
					(signed32) (D2_SOBJ - '0'))
	    {                   /*  ACL entry == SOBJ */
	      sprintf(message, d20_errtab[D20_E43IDX],idx+1);
	      d20_qomask[D20_Q1POS].d20_ioinfo = message;
	      d20_handlemask(d20_qomask);
	      dn_correct = FALSE;
	    }
	    else
	    {
	      p_obj->dn_int = D2_SUBTREE;
	      *(p_obj->dn) = D2_EOS;
	      anz++;
	    }                   
	  }
	    else
	    {                     
	    acl_names[acl_idx][length] = D2_EOS;

	    if                  /*  syntax not correct                */
	       (ds0_508_check_name(&acl_names[acl_idx][0]))
	    {                   /*  show error message                */
	      sprintf(message, d20_errtab[D20_E44IDX], idx + 1);
	      d20_qomask[D20_Q1POS].d20_ioinfo = message;
	      d20_handlemask(d20_qomask);
	      dn_correct = FALSE;
	    }
	    else
	    {                   /*  determine DN-interpretation       */
	      switch (*(signed32 *) d20_6bifields[idx_typ[idx]])
	      {
		case (signed32) (D2_SUBTREE - '0'):
			      p_obj->dn_int = D2_SUBTREE;
			      anz++;
			      break;
		case (signed32) (D2_SOBJ - '0'):
			      p_obj->dn_int = D2_SOBJ;
			      anz++;
			      break;
		default :     sprintf(message, d20_errtab[D20_E43IDX],idx+1);
			      d20_qomask[D20_Q1POS].d20_ioinfo = message;
			      d20_handlemask(d20_qomask);
			      dn_correct = FALSE;
			      break;
	      }   
	    }                   
	  }                     
	  if (dn_correct == TRUE)
	  {   
	    p_acl_info->d20_5dn_count++;
	  } 
	}                       
      }                         
    }                           
    if (dn_correct == TRUE)
    {   
      if (!anz)
      {   
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E45IDX];
	d20_handlemask(d20_qomask);
      }
      else
      {
	acl_par.d20_5no_rights++;
	i++;
	p_acl_info++;
      } 
    } 
  }
  else
  {                             /*  take next right                   */
    i++;
  }                             
}                               

if                              /*  operation is not DISPLAY_OBJECTS  */
   (operation != DS_DISPLAY_OBJECTS)
{                               
  if                            /*  new ACL is not correct            */
     ((ret_value = d20_5_handle_acl(&acl_par)) != D2_NOERROR)
  {                             /*  display error message             */
    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E47IDX];
    d20_handlemask(d20_qomask);
				/*  return (ret_value)                */
    return(ret_value);
  }
  else
  {                             /*  put new ACL in info-block         */

    attr[1].attr_id.d2_type = (Ob_id_string)D2_ACL;
    attr[1].attr_id.d2_typ_len = sizeof(D2_ACL) - 1;

    attr[0].a_val.d2_a_rep = attr[1].a_val.d2_a_rep = 
		(unsigned16) d27_401_syntax_to_rep((signed16)D2_ACCL);
    attr[1].a_val.d2_a_v_len = acl_par.d20_5act_len;
    attr[1].a_val.d2_at_value.d2_oct_a_v = (Octet_string)acl_par.d20_5n_acl;
    attr[1].hex_use = FALSE;

#ifndef CACHE_ADMIN
    if                          /*  operation is MODIFY_ACL           */
       (operation == DS_MODIFY_ATTRIBUTE)
    {                           /*  put old ACL in info-block         */
      attr[0].attr_id.d2_type      = (Ob_id_string)D2_ACL;
      attr[0].attr_id.d2_typ_len   = sizeof(D2_ACL) - 1;

      attr[0].a_val.d2_a_v_len = acl_par.d20_5len;
      attr[0].a_val.d2_at_value.d2_oct_a_v   = (Octet_string)acl_par.d20_5c_acl;
      attr[0].hex_use = FALSE;
      attr[0].mod_type = attr[1].mod_type = D2_REP_AV;
				/*  fill modify info block            */
      d20_fill_modify(D20_ASTART | D20_AINSERT | D20_AEND, attr, 1);
    }
    else
    {                           /*  fill attr info block              */
      ds4_b02_fill_attr(D20_AINSERT, &attr[1], 1);
    }                           
#endif
  }                             
}                               

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  ds0_507_handle_acl                */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ds0_508_check_name(...)                             */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 29.02.88                                            */
/*								      */
/* SYNTAX       : signed16 ds0_508_check_name (dn)                    */
/*                char           *dn;                                 */
/*								      */
/* DESCRIPTION  :                                                     */
/*      The function verifies a given distinguished name for validity.*/
/*      This means, all distinguished name parts must be IA5-strings, */
/*      all naming attribute abbrvs. must satisfy the structure rule  */
/*	for the distinguish name.				      */
/*	And the syntax of the distinguish name should be as follows:  */
/*                                                                    */
/*	<rdn1>\1<rdn2>\1...\1<rdnn>				      */
/*								      */
/*	The syntax of <rdn> is:					      */
/*								      */
/*	<AVA1>\2<AVA2>\2...\2<AVAn>				      */
/*								      */
/*	The syntax of <AVA> is:					      */
/*								      */
/*	<Type>\3<value>						      */
/*                                                                    */
/*	If the distinguish name is correct, then the distinguish name */
/*	containing the naming attribute types is returned in the same */
/*	string.							      */	
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                  char           *dn;     distinguished name        */
/*				 	    as entered in the admin.  */
/*					    masks.		      */ 
/*								      */
/* OUTPUT PARAMETERS:                                                 */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:        no error occurred               */
/*                  D2_NMSYNTAX_ERROR:name syntax error               */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*								      */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  atol()                                            */
/*								      */
/*inoff ***************************************************************/

signed16 ds0_508_check_name(char *dn)
{                                /*  ds0_508_check_name()          */
    signed16     no_rdn;
    signed16     no_nam_attr_type;
    signed16     no_nam_attr_val;
    signed32     par_rule_nr;
    D20_srt     *srt_ent;
    char		*nam_attr_type[NO_NAM_ATTR];
    char		*nam_attr_val[NO_NAM_ATTR];

    signed32	i, j, size, max_size, sav_len;	
    char        new_dn[D2_DNL_MAX + 1];
    char        tmp_dn[D2_DNL_MAX + 1];
    char	    *tmp_dn_ptr = tmp_dn;
    char	type_val_sep;


/* copy dn to work area.	*/
strcpy(tmp_dn, dn);

*new_dn = D2_EOS;

if (*tmp_dn_ptr != D2_ADM_RDN_SEP)
	/*  error -> wrong inputformat vor DN */
	return (D2_NMSYNTAX_ERROR);

/*  verify distinguished name for syntax and IA5 legality */
no_rdn = 0;
max_size = strlen(dn);
size = 0;
par_rule_nr = D20_ROOT;

/* process one RDN at a time	*/
/* till complete dn is scanned.		*/
while (size != max_size)
	{
	tmp_dn_ptr++;
	size++;

	/* one more RDN.	*/
	no_rdn++;

	/* too many RDNs.	*/
	if (no_rdn > D2_NP_MAX)
		return (D2_NMSYNTAX_ERROR);

	no_nam_attr_type = 0;
	no_nam_attr_val = 0;
	nam_attr_type[no_nam_attr_type++] = tmp_dn_ptr;

	/* analyze next RDN.	*/
	while (*tmp_dn_ptr != D2_ADM_RDN_SEP && *tmp_dn_ptr != D2_EOS)
		{
		if (*tmp_dn_ptr == D2_ADM_TYPE_VAL_SEP)
			{
			/* terminate last naming attribute type.	*/
			*tmp_dn_ptr = D2_EOS;

			/* set value pointer for the the current naming attr. */
			/* type.					      */
			nam_attr_val[no_nam_attr_val++] = tmp_dn_ptr + 1;
			}

		if (*tmp_dn_ptr == D2_ADM_AVA_SEP)
			{
			/* no_nam_attr_type has aleady reached the limit    */
			/* more naming attribute is comming.		    */
			if (no_nam_attr_type == NO_NAM_ATTR)
				return(D2_NMSYNTAX_ERROR);

			/* terminate last naming attribute value.	*/
			*tmp_dn_ptr = D2_EOS;

			nam_attr_type[no_nam_attr_type++] = tmp_dn_ptr + 1;
			}

		tmp_dn_ptr++;
		size++;
		}

	if (no_nam_attr_type != no_nam_attr_val)
		return(D2_NMSYNTAX_ERROR);

	*tmp_dn_ptr = D2_EOS;

	/* try to match some rule with the naming attributes.	*/
	if ((srt_ent = ds0_509_rdn_search(no_nam_attr_type, nam_attr_type,
					  par_rule_nr)) == (D20_srt *)NULL)
		return(D2_NMSYNTAX_ERROR);

	sav_len = strlen(new_dn);

	/* copy one RDN to tmp. target array.	*/
	for (i = 0; i < no_nam_attr_type; i++)
		{
		for (j = 0; j < srt_ent->no_nam_attr; j++)
			if (strcmp(srt_ent->nam_attr[j]->at.att_abbrv,
				   nam_attr_type[i]) == 0)
				break;

		if (((int)strlen(nam_attr_val[i]) > 
					srt_ent->nam_attr[j]->at.att_upper) ||
		    ((int)strlen(nam_attr_val[i]) < 
					srt_ent->nam_attr[j]->at.att_lower))
			return(D2_NMSYNTAX_ERROR);

		switch ((int) d27_401_syntax_to_rep((signed16)
					srt_ent->nam_attr[i]->at.att_syntax))
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

		sprintf (new_dn + strlen (new_dn), D2_RDN_FORMAT,
			 D2_AVA_SEP,
			 strlen(srt_ent->nam_attr[j]->at.att_obj_id),
			 srt_ent->nam_attr[i]->at.att_obj_id, 
			 type_val_sep,
			 strlen(nam_attr_val[i]), 
			 nam_attr_val[i]) ;
		}

	*(new_dn + sav_len) = D2_RDN_SEP;

	par_rule_nr = srt_ent->rule_nr;
	}


strcpy(dn, new_dn + 1);
return(D2_NO_ERR);

}                                 /*  ds0_508_check_name()          */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ds0_509_rdn_search(...)                             */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 04.01.88                                            */
/*								      */
/* SYNTAX       : D20_srt *ds0_509_rdn_search (no_nam_attr, 	      */
/*					   nam_attr_type, par_rule_nr)*/
/*		  signed32  no_nam_attr;			      */
/*                char	   *nam_attr_type[];                          */
/*                signed32  par_rule_nr;                              */
/*                                                                    */
/* DESCRIPTION  : This function checks if there exists some rule whose*/
/*		  parent is par_rule_nr and which has same set of     */
/*		  naming attributes as nam_attr_type array. If there  */
/*		  exists such a rule then it has to be unique.	      */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*		  signed32  no_nam_attr;			      */
/*                char	   *nam_attr_type[];                          */
/*                signed32  par_rule_nr;                              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*								      */
/* RETURN VALUE :   pointer to rule satisfying the required condition */
/*                  D2_NULL:      no. rule found		      */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*		D20_srt		*d2_p_d20srt;			      */
/*		signed32	d2_nd20srt;			      */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static D20_srt *ds0_509_rdn_search(
		    signed32  no_nam_attr,
		    char     *nam_attr_type[],
		    signed32  par_rule_nr )

{                                   /*  d26_509_rdn_search()          */
  register D20_srt 	*srt_ent;
  register D20_srt     	*max_srt;
  register signed32	i, j;


/*  all entries in SRT            */
for (srt_ent = d2_p_d20srt, max_srt = srt_ent + d2_nd20srt; 
     srt_ent < max_srt; srt_ent++)
	{
	if (srt_ent->par_rule_nr == par_rule_nr && srt_ent->no_nam_attr == 
								no_nam_attr)
		{
		for (i=0; i<srt_ent->no_nam_attr; i++)
			{
			for (j=0; j<no_nam_attr; j++)
				if (strcmp(srt_ent->nam_attr[i]->at.att_abbrv,
				    nam_attr_type[j]) == 0)
					break;

			/* match not found.	*/
			if (j == no_nam_attr)
				break;
			}

		/* match found for all attributes.	*/
		if (i == srt_ent->no_nam_attr)
    			return (srt_ent);
		}
	}

return((D20_srt *)NULL);

}                                   /*  ds0_509_rdn_search()          */
