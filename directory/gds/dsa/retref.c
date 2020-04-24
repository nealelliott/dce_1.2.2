/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: retref.c,v $
 * Revision 1.1.736.2  1996/02/18  18:15:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:17  marty]
 *
 * Revision 1.1.736.1  1995/12/08  15:51:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:53  root]
 * 
 * Revision 1.1.734.4  1994/06/21  14:46:24  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:37:54  marrek]
 * 
 * Revision 1.1.734.3  1994/05/10  15:53:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:29:01  marrek]
 * 
 * Revision 1.1.734.2  1994/03/23  15:14:00  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:26:30  keutel]
 * 
 * Revision 1.1.734.1  1994/02/22  17:37:42  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:26:14  marrek]
 * 
 * Revision 1.1.732.2  1993/10/14  17:16:55  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:14:59  keutel]
 * 
 * Revision 1.1.732.1  1993/10/13  17:31:36  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:30:53  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  16:08:13  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:29:33  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:44:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:54  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:37:05  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:26:29  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:57:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:34:33  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: retref.c,v $ $Revision: 1.1.736.2 $ $Date: 1996/02/18 18:15:45 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : retref.c        [retref]                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 10.05.88                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the utilities used by the     */
/*                  internal search function, that handle referrals.  */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 10.05.88| birth                          | ek |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <sys/types.h>
#include <stdlib.h>
#include <search.h>

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d2shm.h>
#include <d26schema.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static byte *d26_u36_write_referral_header(byte **output, byte *message,
    D26_referral *referral, signed32 *output_len, signed32 *remlen);
static signed16 d26_u42_make_base_obj_referral(D26_dninfo *dninfo,
    D2_name_string ntarget, signed16 al_rdns, D2_name_string target,
    D23_av_value *undecoded, D26_at *psap_at, signed16 acp_idx,
    signed32 master_dsa, boolean entry_only, boolean ignore);
static byte *d26_u60_write_access_point(signed16 acp_idx, byte **output,
    byte *message, signed32 *output_len, signed32 *remlen);
static signed16 d26_u61_find_break_access_point(D26_dninfo *dninfo,
    signed32 mk, signed16 *acp_idx, D26_at *psap_at);
static byte *d26_u62_write_referral(byte **output, byte *message,
    D26_referral *referral, signed32 *output_len, signed32 *remlen);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u35_find_access_point                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        29.11.90                                              */
/*                                                                    */
/* DESCRIPTION: This function finds the record containing the DSA-    */
/*              object pointed to by a DN-List-index.                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16                access-point found        */
/*      D2_NFOUND                           no access point found     */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u35_find_access_point(
    signed32 dnl_idx,   /* IN    -  Index to DN-List-entry            */
    char interpretation,/* IN    -  Interpretation of DN-list entry   */
    signed16 *acp_idx,  /* OUT   -  Index to access point in ACP-list */
    D26_at *psap_at,    /* IN    -  AT entry of PSAP address (unused) */
    boolean *db_access) /* OUT   -  Indicates database access         */

{                               /*  d26_u35_find_access_point         */

  static char function[] = "d26_u35_find_access_point";
				    /* invalid entry in PSAP-list     */
  static D2_str invalid_psap = {0,(Octet_string)NULL};

  D26_dninfo  dn_accinfo;           /* information for access point   */
  D26_dndesc  *dndesc;              /* pointer to DN description      */
  D26_rdninfo *l_rdn;               /* last RDN of master DSA         */
  D26_rdninfo *rdn1;                /* first RDN of access point      */
  signed16    rdn1_len;             /* length of first RDN            */
  signed16    no_ava1;              /* number of AVAs of first RDN    */

  if                    /*  DN-List index invalid                     */
    (0L > dnl_idx || dnl_idx > d26_maxidx_dnlist)
  {                     /*  return not found                          */
    return(D2_NFOUND);
  }
  else
  {                     /*  assign DN-description                     */
    dndesc = d26_dnlist + dnl_idx;
    dn_accinfo.d26_rd_anz = 0;
    if                  /*  name structure is ok                      */
      (d26_u06_get_name(dndesc->d26_dnname,&dn_accinfo) == D2_NO_ERR)
    {                   /*  check whether entry is valid              */
      if                /*  interpretation does not match or entry is */
			/*  invalid                                   */
	(interpretation != D2_IGNORE && dndesc->d26_dnint != interpretation ||
	 dndesc->d26_occurrency <= 0L)
      {                 /*  return "not found"                        */
	return(D2_NFOUND);
      }
      else if           /*  scope of referrals is restricted          */
	     (d26_serv_contr->d23_scref == D2_COUNTRY_SCOPE &&
	      (d26_serv_contr->d23_scopt & D2_NOCHAINING ||
	       !(d26_serv_contr->d23_scopt & D2_PREF_CHAINING)))
      {                 /*  set pointer to first RDN                  */
	rdn1 = dn_accinfo.d26_rdn;
	no_ava1 = rdn1->d26_r_nava;
	rdn1_len = rdn1->d26_r_ava[no_ava1 - 1].d26_a_val +
		   rdn1->d26_r_ava[no_ava1 - 1].d26_a_len - dndesc->d26_dnname;
	if              /*  first RDNs differ                         */
	  (strncmp((char *)dndesc->d26_dnname,(char *)d26_myname,rdn1_len))
	{               /*  return out of scope error                 */
	  return(D2_OUT_OF_SCOPE);
	}
      }
      for               /*  all written access points                 */
	 (*acp_idx = 0; *acp_idx < d26_accp_count; (*acp_idx)++)
      {                 /*  compare with new access point             */
	if              /*  new access point already in list          */
	  (d26_accp_list[*acp_idx].d26_dsaname == dndesc->d26_dnname)
	{               /*  return                                    */
	  return(d26_accp_list[*acp_idx].d26_psap.d2_size > 0 ? D2_NO_ERR :
								D2_NFOUND);
	}
      }

      d26_accp_list[*acp_idx].d26_dsaname = dndesc->d26_dnname;
      d26_accp_list[*acp_idx].d26_dsa_ename = dndesc->d26_ename;
      d26_accp_list[*acp_idx].d26_dsa_ep  = (D23_av_value *)NULL;
      d26_accp_list[*acp_idx].d26_psap    = invalid_psap;
      d26_accp_list[*acp_idx].d26_dnl_idx = dnl_idx;
      d26_accp_count++;
      if                /*  DSA-object could  be found                */
	(d26_i31_find_object(&dn_accinfo,&l_rdn,d26_ob_record) == D2_NO_ERR)
      {                 /*  read objects PSAP-Address                 */
	d26_i58_get_accp_info(dn_accinfo.d26_filenr,d26_ob_record,
	  &d26_accp_list[*acp_idx].d26_psap,
	  d26_accp_list[*acp_idx].d26_auth_m,
	  &d26_accp_list[*acp_idx].d26_pr_name);
      }
      D27_022_ENTER_TIME_TRACE(GDS_S_PSAP_ACCESSED,function,0,0);
      *db_access = TRUE;
      return(d26_accp_list[*acp_idx].d26_psap.d2_size > 0 ? D2_NO_ERR :
							    D2_NFOUND);
    }
    else
    {                   /*  return NOT FOUND                          */
      return(D2_NFOUND);
    }
  }

}                       /*  d26_u35_find_access_point                 */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u36_write_referral_header                         */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        29.11.90                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the referral structure into      */
/*              message.                                              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      output      String *        pointer to beginning of message   */
/*      message     String          actual message pointer            */
/*      referral    D26_referral *  referral information              */
/*      output_len  Int *           maximum length for message        */
/*      remlen      Int *           remaining length for message      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length for message        */
/*      remlen      Int *           remaining length for message      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String      message         pointer to byte after     */
/*                                          written message.          */
/*                          D2_NULL         error occurred.           */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_u36_write_referral_header(byte **output, byte *message,
    D26_referral *referral, signed32 *output_len, signed32 *remlen)

{                               /*  d26_u36_write_referral_header     */

  signed16 dnlen;               /* length of DN to write              */
  signed16 new_rdnlen;          /* length of new RDN to write         */
  signed16 targ_len;            /* length of target object            */
  signed16 len;                 /* length of data to write            */

len = sizeof(D23_rf_info);
if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write referral structure                  */
  ((D23_rf_info *)message)->d23_rftag     = D23_REFTAG;
  ((D23_rf_info *)message)->d23_rf_alirdn = referral->d26_al_rdns;
  ((D23_rf_info *)message)->d23_rf_rdnres = referral->d26_rdn_res;
  if                    /*  no namepart resolved                      */
    (referral->d26_op_prog.d2_nr_phase == D2_NOT_STARTED ||
     referral->d26_op_prog.d2_nr_phase == D2_PROCEEDING &&
     referral->d26_op_prog.d2_next_rdn == 1)
  {                     /*  get reference type from master knowledge  */
    ((D23_rf_info *)message)->d23_rf_type = D2_SUPERIOR;
  }
  else if               /*  name resolution is proceeding             */
	 (referral->d26_op_prog.d2_nr_phase == D2_PROCEEDING)
  {                     /*  get reference type from difference of     */
			/*  nameparts                                 */
    ((D23_rf_info *)message)->d23_rf_type =
	referral->d26_op_prog.d2_next_rdn - referral->d26_rdn_res > 1 ?
	    D2_CROSS : D2_SUBORDINATE;
  }
  else
  {                     /*  set subordinate type                      */
    ((D23_rf_info *)message)->d23_rf_type = D2_SUBORDINATE;
  }                     
  ((D23_rf_info *)message)->d23_op_pr.d23_opnm_rs =
			    referral->d26_op_prog.d2_nr_phase;
  ((D23_rf_info *)message)->d23_op_pr.d23_op_nxtrdn =
			    referral->d26_op_prog.d2_next_rdn;
  ((D23_rf_info *)message)->d23_nm_acc    = referral->d26_acp_count;
  ((D23_rf_info *)message)->d23_rf_entry_only   = referral->d26_entry_only;
  message += len;
  *remlen  -= len;

  dnlen      = strlen((char *)referral->d26_etarget);
  new_rdnlen = 0;
  targ_len   = dnlen;
  if                    /*  target object must be enhanced by new RDN */
    (referral->d26_new_rdn != (D2_name_string)NULL)
  {                     /*  set length of new RDN                     */
    new_rdnlen = strlen((char *)referral->d26_new_rdn);
    targ_len += targ_len == 0 ? 0 : 1;
    targ_len += new_rdnlen;
  }                     
  targ_len += targ_len == 0 ? 0 : 1;
  len   = D2_ALIGN(targ_len);
  if                    /*  error from check buffer                   */
    ((message = d26_u40_check_buffer(sizeof(D23_ds_name) + len + 1,
     output_len,remlen,output,message)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write distinguished name                  */
    ((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
    ((D23_ds_name *)message)->d23_dscomplete =
      referral->d26_targ_ep == (D23_av_value *)NULL ? TRUE : FALSE;
    ((D23_ds_name *)message)->d23_dslen = targ_len;
    ((D23_ds_name *)message)->d23_dsoff = D2_ALIGN(targ_len);
    message += sizeof(D23_ds_name);
    *remlen -= sizeof(D23_ds_name);
    strcpy((char *)message,(char *)referral->d26_etarget);
    message += dnlen;
    *remlen -= dnlen;
    if                  /*  new RDN must be enhanced                  */
      (referral->d26_new_rdn != (D2_name_string)NULL)
    {                   /*  enhance new RDN                           */
      if                /*  DN has been written                       */
	(dnlen > 0)
      {                 /*  write RDN separator                       */
	*message = D2_RDN_SEP;
	message++;
	(*remlen)--;
      }                 
      strcpy((char *)message,(char *)referral->d26_new_rdn);
      message += new_rdnlen;
      *remlen -= new_rdnlen;
    }                   
    message   += targ_len == 0 ? 0 : 1;
    (*remlen) -= targ_len == 0 ? 0 : 1;
    message   += len - targ_len;
    (*remlen) -= len - targ_len;
    if                  /*  undecoded part of target object present   */
      (referral->d26_targ_ep != (D23_av_value *)NULL)
    {                   /*  check buffer for undecoded part           */
      len = referral->d26_targ_ep->d23_avoff;
      if                /*  error from check buffer                   */
	((message = d26_u40_check_buffer(sizeof(D23_av_value) + len,
	 output_len,remlen,output,message)) == NULL)
      {                 /*  return error                              */
	return(NULL);
      }
      else
      {                 /*  write undecoded part of target object     */
	*(D23_av_value *)message = *referral->d26_targ_ep;
	message += sizeof(D23_av_value);
	*remlen -= sizeof(D23_av_value);
	memcpy(message,(byte *)(referral->d26_targ_ep + 1),len);
	message += len;
	*remlen -= len;
      }                 
    }                   
  }                     
}                       

			/*  return message                            */
return(message);

}                       /*  d26_u36_write_referral_header             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u38_refcmp                                        */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        19.02.91                                              */
/*                                                                    */
/* DESCRIPTION: This function compares two referral structures.       */
/*              A referral is considered to be less than the other,   */
/*              if its target object is less than the others, or if   */
/*              the target objects are equal and the DNL-index of     */
/*              the master DSA of the referral is less than the one   */
/*              of the other referral. Two referrals are considered   */
/*              to be equal, if the target objects and master DSAs    */
/*              are equal.                                            */
/*              Two target objects are considered to be equal, if     */
/*              one of them is an extension of the other. The shorter */
/*              one will be used then. One target object is less than */
/*              the other, if it is less in alphabetical ordering.    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      referral1   Void *          first referral to compare         */
/*      referral2   Void *          first referral to compare         */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*          -1  Int     first referral is less than second            */
/*           0          referrals are equal                           */
/*           1          first referral is greater than second         */
/*                                                                    */
/*inoff ***************************************************************/

int d26_u38_refcmp(const void *referral1, const void *referral2)

{                               /*  d26_u38_refcmp                    */

  D26_referral *ref1;           /*  pointer to first referral         */
  D26_referral *ref2;           /*  pointer to second referral        */
  D26_referral *oldref;         /*  old referral existing in list     */
  D26_referral *newref;         /*  new referral to compare           */
  int          oldlen;          /*  length of old target object       */
  int          newlen;          /*  length of new target object       */
  int          len1;            /*  length of first target object     */
  int          len2;            /*  length of second target object    */
  int          cmp;             /*  result of comparison              */
				/*  old and new length                */
  int          minlen;          /*  minimum length of target objects  */
  boolean      extension1;      /*  indicates first referral being    */
				/*  extension of the second           */
  boolean      extension2;      /*  indicates second referral being   */
				/*  extension of the first            */

ref1 = (D26_referral *)referral1;
ref2 = (D26_referral *)referral2;

len1 = strlen((char *)ref1->d26_target);
len2 = strlen((char *)ref2->d26_target);
minlen = len1 < len2 ? len1 : len2;
cmp  = strncmp((char *)ref1->d26_target,(char *)ref2->d26_target,minlen);
extension1 = cmp == 0 && len2 < len1 && (len2 == 0 ||
  ref1->d26_target[len2] == D2_RDN_SEP) ? TRUE : FALSE;
extension2 = cmp == 0 && len1 < len2 && (len1 == 0 ||
  ref2->d26_target[len1] == D2_RDN_SEP) ? TRUE : FALSE;

if                      /*  first index is smaller                    */
  (ref1->d26_ref_index < ref2->d26_ref_index)
{                       /*  the first referral is the old one         */
  oldref = ref1;
  oldlen = len1;
  newref = ref2;
  newlen = len2;
}
else
{                       /*  the second referral is the old one        */
  oldref = ref2;
  oldlen = len2;
  newref = ref1;
  newlen = len1;
}                       

if                      /*  target objects are equal                  */
  (oldlen == newlen && cmp == 0)
{                       /*  set the new master dsa                    */
  newref->d26_master_dsa = oldref->d26_master_dsa;
}                       

if                      /*  new referral is extension of old one      */
  (oldlen == newlen ||
   oldlen < newlen && (extension1 == TRUE || extension2 == TRUE))
{                       /*  set the new predecessor                   */
  newref->d26_predecessor = oldref->d26_ref_index;
}                       

if                      /*  referral 1 smaller than referral 2        */
  (cmp < 0 || cmp == 0 && len1 < len2 && extension2 == FALSE ||
   cmp == 0 && len1 < len2 && extension2 == TRUE &&
   ref1->d26_acp[0] == D26_OWNNAME ||
   cmp == 0 && (len1 == len2 || len1 < len2 && extension2 == TRUE) &&
   ref1->d26_acp[0] < ref2->d26_acp[0])
{                       /*  return -1                                 */
  return(-1);
}
else if                 /*  referrals are equivalent                  */
     (cmp == 0 &&
      (len1 == len2 || len1 < len2 && extension2 == TRUE ||
		       len2 < len1 && extension1 == TRUE) &&
      ref1->d26_acp[0] == ref2->d26_acp[0])
{                       /*  replace new referral index and return 0   */
  newref->d26_ref_index = oldref->d26_ref_index;
  return(0);
}
else
{                       /*  return +1                                 */
  return(1);
}                       

}                       /*  d26_u38_refcmp                            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u41_free_referrals                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        19.01.91                                              */
/*                                                                    */
/* DESCRIPTION: This function deletes the referral structures from    */
/*              tree and frees all the target names and PSAP-Addresses*/
/*              of the Access-Point-list.                             */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u41_free_referrals(void)

{                               /*  d26_u41_free_referrals            */

  register signed32 i;              /* loop variable                  */
  register D26_acc_point *acp;      /* pointer to access-point-list   */

for                             /*  all referrals                     */
   (i = 0; i < d26_ref_count; i++)
{                               /*  delete the node from binary tree  */
  tdelete((void *)(d26_referral + i),&d26_ref_root,d26_u38_refcmp);
}                       

for                             /*  all entries of Access-point-list  */
   (i = 0, acp = d26_accp_list; i < d26_accp_count; i++, acp++)
{                               /*  check whether it contains PSAP-   */
				/*  address                           */
  if                            /*  access point contains PSAP-Address*/
    (acp->d26_psap.d2_value != NULL)
  {                             /*  free the memory                   */
    free(acp->d26_psap.d2_value);
  }                             
  if                            /*  access point contains principal n.*/
    (acp->d26_pr_name != NULL)
  {                             /*  free the memory                   */
    free(acp->d26_psap.d2_value);
  }                             
  if                            /*  memory for name was allocated     */
    (acp->d26_nam_alloc == TRUE)
  {                             /*  free the memory                   */
    free(acp->d26_dsaname);
    acp->d26_nam_alloc = FALSE;
  }                             
}                               

d26_accp_count = 0L;

for                             /*  all referrals                     */
   (i = 0; i < d26_ref_count; i++)
{                               /*  free the target object            */
  free(d26_referral[i].d26_target);
  free(d26_referral[i].d26_etarget);
}                               

d26_ref_present = FALSE;
d26_ref_count   = 0;

}                               /*  d26_u41_free_referrals            */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u42_make_base_obj_referral                        */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        14.03.91                                              */
/*                                                                    */
/* DESCRIPTION: This function creates the referral for a base object. */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dninfo      D26_dninfo *    information about object used for */
/*                                  reference                         */
/*      ntarget     D2_name_string  normed name of target object      */
/*      al_rdns     Short           number of aliased RDNs of base o. */
/*      target      D2_name_string  exact name of target object       */
/*      undecoded   D23_av_value *  undecoded part of base object     */
/*      psap_at     D26_at *        pointer to AT-entry of PSAP-addr. */
/*      shb_at      D26_at *        pointer to AT-entry of Shadowed-  */
/*                                  by-attribute                      */
/*      acp_idx     Short           Index of access-point list        */
/*      sh_by       Long *          collected shadowed by attribute   */
/*      master_dsa  Long            DN-List pointer of master DSA of  */
/*                                  target object                     */
/*      entry_only  Bool            indicates whether only the target */
/*                                  object should be searched         */
/*      ignore      Bool            indicates whether referral must be*/
/*                                  ignored                           */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      dninfo      D26_dninfo *    information about object used for */
/*                                  reference                         */
/*      sh_by       Long *          collected shadowed by attribute   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   Short           no error occurred                 */
/*      D26_ERROR                   malloc error occurred             */
/*                                                                    */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_u42_make_base_obj_referral(D26_dninfo *dninfo,
    D2_name_string ntarget, signed16 al_rdns, D2_name_string target,
    D23_av_value *undecoded, D26_at *psap_at, signed16 acp_idx,
    signed32 master_dsa, boolean entry_only, boolean ignore)

{                               /*  d26_u42_make_base_obj_referral    */

if                      /*  no memory for normed target available     */
  ((d26_referral[d26_ref_count].d26_target =
   (D2_name_string)D27_MALLOC(svc_c_sev_warning,strlen((char *)ntarget) + 1))
   == NULL)
{                       /* return error                               */
  return(D26_ERROR);
}
else if               /*  no memory for exact target available      */
       ((d26_referral[d26_ref_count].d26_etarget =
	(D2_name_string)D27_MALLOC(svc_c_sev_warning,
	strlen((char *)target) + 1)) == NULL)
{                       /* return error                               */
  free(d26_referral[d26_ref_count].d26_target);
  return(D26_ERROR);
}
else
{                       /*  fill referral                             */
  strcpy((char *)d26_referral[d26_ref_count].d26_target,(char *)ntarget);
  strcpy((char *)d26_referral[d26_ref_count].d26_etarget,(char *)target);
  DCE_SVC_LOG((GDS_S_TARGET_DSA_MSG,d26_referral[d26_ref_count].d26_target,
    acp_idx >= 0 ? d26_accp_list[acp_idx].d26_dsaname : d26_myname));
  d26_referral[d26_ref_count].d26_new_rdn     = (D2_name_string)NULL;
  d26_referral[d26_ref_count].d26_targ_ep     = undecoded;
  d26_referral[d26_ref_count].d26_al_rdns     = al_rdns;
  d26_referral[d26_ref_count].d26_rdn_res     = D2_NOT_DEFINED;
  d26_referral[d26_ref_count].d26_entry_only  = entry_only;
  d26_referral[d26_ref_count].d26_acp_count   = 1;
  d26_referral[d26_ref_count].d26_acp[0]      = acp_idx;
  d26_referral[d26_ref_count].d26_route       = FALSE;
  d26_referral[d26_ref_count].d26_master_dsa  = master_dsa;
  d26_referral[d26_ref_count].d26_ref_index   = d26_ref_count;
  d26_referral[d26_ref_count].d26_predecessor = D26_IDX_NOT_DEF;
  d26_referral[d26_ref_count].d26_own_sh_dsas = FALSE;
  d26_referral[d26_ref_count].d26_ignore      = ignore;
  if                    /*  still no namepart found                   */
    (dninfo->d26_rd_anz == 0)
  {                     /*  set name resolution not started           */
    d26_referral[d26_ref_count].d26_op_prog.d2_nr_phase = D2_NOT_STARTED;
  }
  else
  {                     /*  set operation progress proceeding         */
    d26_referral[d26_ref_count].d26_op_prog.d2_nr_phase = D2_PROCEEDING;
    if                  /*  request must be routed, no progress in    */
			/*  name resolution                           */
      (d26_routing == TRUE &&
       d26_op_progress->d23_opnm_rs == D2_PROCEEDING &&
       d26_op_progress->d23_op_nxtrdn == dninfo->d26_rd_anz)
    {                   /*  reset the routing flag                    */
      d26_referral[d26_ref_count].d26_route = TRUE;
    }
    if                  /*  master part could be read                 */
      (d26_u57_find_master_part(dninfo,(signed32)(acp_idx < 0 ?
       D26_OWNNAME : d26_accp_list[acp_idx].d26_dnl_idx),
       &d26_referral[d26_ref_count].d26_op_prog.d2_next_rdn) == D2_NO_ERR)
    {                   /*  set RDNs resolved                         */
      d26_referral[d26_ref_count].d26_rdn_res = dninfo->d26_rd_anz;
    }                   
  }                     
}                       

return(D2_NO_ERR);

}                       /*  d26_u42_make_base_obj_referral            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u48_write_referral_set                            */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.02.91                                              */
/*                                                                    */
/* DESCRIPTION: This function writes all of the referrals stored in   */
/*              the referral tree into message.                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      output      String *        pointer to beginning of message   */
/*      message     String          actual message pointer            */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String      message         pointer to byte after     */
/*                                          written message.          */
/*                          D2_NULL         error occurred.           */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u48_write_referral_set(byte **output, byte *message,
    signed32 *output_len, signed32 *remlen)

{                               /*  d26_u48_write_referral_set        */

  register signed32 i;          /* loop variable                      */

for                     /*  all generated referrals                   */
   (i = 0; i < d26_ref_count; i++)
{                       /*  write it                                  */
  if                    /*  error from writing referral               */
    ((message = d26_u62_write_referral(output,message,d26_referral + i,
     output_len,remlen)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }                     
}                       

			/*  return message                            */
return(message);

}                       /*  d26_u48_write_referral_set                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u58_make_subordinate_ref                          */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        15.03.91                                              */
/*                                                                    */
/* DESCRIPTION: This function creates a single referral, after the    */
/*              master access point was found during descending the   */
/*              subordinates of a given base object.                  */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dninfo      D26_dninfo *    information about base object     */
/*      dname       D26_name_string normed DN of base object          */
/*      ename       D26_name_string exact DN of base object           */
/*      db_access   Bool *          indicates intermediate database   */
/*                                  access                            */
/*      acp_idx     Short           index of access point             */
/*      psap_at     D26_at *        pointer to AT-entry of PSAP-addr. */
/*      shb_at      D26_at *        pointer to AT-entry of Shadowed-  */
/*                                  by-attribute                      */
/*      sh_by       Long *          collected shadowed by attribute   */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u58_make_subordinate_ref(
    D26_dninfo *dninfo,
    signed16   np_base,
    D2_name_string dname,
    D2_name_string ename,
    boolean *db_access,
    signed16 acp_idx,
    D26_at *psap_at)

{                               /*  d26_u58_make_subordinate_ref      */

  signed16 np_sup;          /* number of name parts of superior node  */
  signed32 mk_sup;          /* index into DN list for master knowledge*/
			    /* of superior node                       */
  signed32 ocl[D26_OCL_VAL_COUNT + 1];
			    /* indices into DN list for ACL           */
  D26_acl  acl[D2_ACLIDX_MAX];
  D26_rdninfo *rdn;         /* pointer to RDN of object               */
  D26_ava  *ava;            /* pointer to AVA of RDN                  */
  byte     *repl;           /* pointer to replaced character in DN    */
  byte     save;            /* saved character                        */

if                      /*  father is not root                        */
  (dninfo->d26_rd_anz > 1)
{                       /*  replace last character of father by EOS   */
  rdn   = dninfo->d26_rdn + dninfo->d26_rd_anz - 2;
  ava   = rdn->d26_r_ava + rdn->d26_r_nava - 1;
  repl  = ava->d26_a_val + ava->d26_a_len;
  save  = *repl;
  *repl = D2_EOS;
}
else
{                       /*  replace first character of DN             */
  repl  = dname;
  save  = *repl;
  *repl = D2_EOS;
}                       

d26_referral[d26_ref_count].d26_target     = dname;
d26_referral[d26_ref_count].d26_new_rdn    = (D2_name_string)NULL;
d26_referral[d26_ref_count].d26_targ_ep    = (D23_av_value *)NULL;
d26_referral[d26_ref_count].d26_entry_only = FALSE;
d26_referral[d26_ref_count].d26_acp_count  = 1;
d26_referral[d26_ref_count].d26_acp[0]     = acp_idx;
d26_referral[d26_ref_count].d26_route      = FALSE;
d26_referral[d26_ref_count].d26_master_dsa = D26_IDX_NOT_DEF;
d26_referral[d26_ref_count].d26_ref_index  = d26_ref_count;
d26_referral[d26_ref_count].d26_ignore     = FALSE;

if                      /*  equivalent referral is not yet in tree    */
  (tfind((void *)(d26_referral + d26_ref_count),&d26_ref_root,d26_u38_refcmp)
   == (void *)NULL)
{                       /*  check master of superior node             */
  np_sup = dninfo->d26_rd_anz - 1;
  if                    /*  master of superior not known              */
    ((mk_sup = d26_referral[d26_ref_count].d26_master_dsa)
     == D26_IDX_NOT_DEF)
  {                     /*  read parent entry                         */
    dninfo->d26_rd_anz--;
    switch(d26_i31_find_object(dninfo,&rdn,d26_ob_record))
    {
      case D2_NO_ERR:
	  d26_i36_read_ocl_acl_mk(dninfo,d26_ob_record,ocl,acl,&mk_sup);
	  break;
      case D2_NFOUND:
      case D26_ERROR:
	dce_svc_printf(GDS_S_DB_INCONSISTENT_MSG);
	break;
    }   
    *db_access = TRUE;
  }                     
  if                    /*  superior is not shadow                    */
    (mk_sup == D26_OWNNAME || d26_routing == TRUE && np_base == np_sup)
  {                     /*  enter referral into tree                  */
    if                  /* memory for normed target object available  */
      ((d26_referral[d26_ref_count].d26_target =
       (D2_name_string)D27_MALLOC(svc_c_sev_warning,
       strlen((char *)dname) + 1)) != NULL)
    {                   /* allocate memory for exact target object    */
      if                /* no memory for exact target object          */
	((d26_referral[d26_ref_count].d26_etarget =
	 (D2_name_string)D27_MALLOC(svc_c_sev_warning,
	 strlen((char *)ename) + 1)) == NULL)
      {                 /* free normed target object                  */
	free(d26_referral[d26_ref_count].d26_target);
      }
      else
      {                 /* fill referral                              */
	strcpy((char *)d26_referral[d26_ref_count].d26_target,(char *)dname);
	strcpy((char *)d26_referral[d26_ref_count].d26_etarget,(char *)ename);
	DCE_SVC_LOG((GDS_S_TARGET_DSA_MSG,
	  d26_referral[d26_ref_count].d26_target,
	  d26_accp_list[acp_idx].d26_dsaname));
	*repl = save;
	d26_referral[d26_ref_count].d26_new_rdn = (D2_name_string)NULL;
	d26_referral[d26_ref_count].d26_targ_ep = (D23_av_value *)NULL;
	d26_referral[d26_ref_count].d26_op_prog.d2_nr_phase = D2_COMPLETED;
	d26_referral[d26_ref_count].d26_rdn_res = D2_NOT_DEFINED;
	d26_referral[d26_ref_count].d26_al_rdns = 0;
	d26_referral[d26_ref_count].d26_master_dsa = D26_OWNNAME;
	d26_referral[d26_ref_count].d26_entry_only = FALSE;
	d26_referral[d26_ref_count].d26_route = mk_sup == D26_OWNNAME ?
	    FALSE : TRUE;
	if                    /* referral couldn't be inserted        */
	  (D27_TSEARCH(svc_c_sev_warning,
	   (void *)(d26_referral + d26_ref_count),&d26_ref_root,d26_u38_refcmp)
	   == (void *)NULL)
	{                     /* free target object names             */
	  free(d26_referral[d26_ref_count].d26_target);
	  free(d26_referral[d26_ref_count].d26_etarget);
	}
	else
	{                     /* increment referrals count            */
	  DCE_SVC_LOG((GDS_S_REF_GENERATED_MSG));
	  d26_ref_present = TRUE;
	  d26_ref_count++;
	}
      }
    }
  }
}

  *repl = save;

}                       /*  d26_u58_make_subordinate_ref              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u60_write_access_point                            */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        29.11.90                                              */
/*                                                                    */
/* DESCRIPTION: This function writes one single access point from the */
/*              list into the message in IPC format.                  */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      acp_idx     Short           index of access point in list     */
/*      output      String *        pointer to beginning of message   */
/*      message     String          actual message pointer            */
/*      output_len  Int *           maximum length of message         */
/*      remlen      Int *           remaining length of message       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of message         */
/*      remlen      Int *           remaining length of message       */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              D2_NO_ERR   Short           no error occurred         */
/*              D26_ERROR                   error occurred            */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_u60_write_access_point(signed16 acp_idx, byte **output,
    byte *message, signed32 *output_len, signed32 *remlen)

{                               /*  d26_u60_write_access_point        */

  D26_acc_point  *acp;              /* pointer to access point        */
				    /* indicates complete decoding of */
				    /* DNs                            */
  Bool           name_complete;
  signed16       len = 0;           /* length of data to write        */
				    /* comment for logging            */
  D2_name_string ds_name;           /* DN of access point             */

len = sizeof(D23_acc_info);

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(NULL);
}
else
{                       /*  write access-point-tag                    */
  ((D23_acc_info *)message)->d23_acctag = D23_ACCTAG;
  *remlen -= len;
  message += len;
  len = sizeof(D23_ds_name);
  if                    /*  error from check buffer                   */
    ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
     == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write DN-structure                        */
    acp     = d26_accp_list + acp_idx;
    ds_name = acp->d26_dsa_ename;
    len     = strlen((char *)ds_name) + 1;
    ((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
    ((D23_ds_name *)message)->d23_dscomplete = name_complete =
	acp->d26_dsa_ep == (D23_av_value *)NULL ? TRUE : FALSE;
    ((D23_ds_name *)message)->d23_dslen = len;
    ((D23_ds_name *)message)->d23_dsoff = D2_ALIGN(len);
    *remlen -= sizeof(D23_ds_name);
    message += sizeof(D23_ds_name);
    len      = D2_ALIGN(len);
    if                  /*  error from check buffer                   */
      ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
       == NULL)
    {                   /*  return error                              */
      return(NULL);
    }
    else
    {                   /*  write DN                                  */
      strcpy((char *)message,(char *)ds_name);
      *remlen -= len;
      message += len;
      if                /*  DSA name not completely decoded           */
	(name_complete == FALSE)
      {                 /*  check buffer for encoded part             */
	len = sizeof(D23_av_value);
	if              /*  error from check buffer                   */
	  ((message = d26_u40_check_buffer(len + acp->d26_dsa_ep->d23_avoff,
	   output_len,remlen,output,message)) == NULL)
	{               /*  return error                              */
	  return(NULL);
	}
	else
	{               /*  assign length and value of undecoded part */
	  *(D23_av_value *)message = *acp->d26_dsa_ep;
	  message += len;
	  *remlen -= len;
	  len      = acp->d26_dsa_ep->d23_avlen;
	  memcpy(message,(byte *)(acp->d26_dsa_ep + 1),len);
	  message += acp->d26_dsa_ep->d23_avoff;
	  *remlen -= acp->d26_dsa_ep->d23_avoff;
	}               
      }                 
      len      = sizeof(D23_av_value);
      if                /*  error from check buffer                   */
	((message = d26_u40_check_buffer(len,output_len,remlen,output,
	 message)) == NULL)
      {                 /*  return error                              */
	return(NULL);
      }
      else
      {                 /*  assign length and value of PSAP-Address   */
	len = acp->d26_psap.d2_size;
	((D23_av_value *)message)->d23_avlen = len;
	((D23_av_value *)message)->d23_avoff = D2_ALIGN(len);
	((D23_av_value *)message)->d23_avrep = D2_PSAP_STX;
	*remlen -= sizeof(D23_av_value);
	message += sizeof(D23_av_value);
	len = D2_ALIGN(len);
	if              /*  error from check buffer                   */
	  ((message = d26_u40_check_buffer(len,output_len,remlen,output,
	   message)) == NULL)
	{               /*  return error                              */
	  return(NULL);
	}
	else
	{               /*  write PSAP address into message           */
	  memcpy(message,acp->d26_psap.d2_value,acp->d26_psap.d2_size);
	  *remlen -= len;
	  message += len;
	}               
      }                 
    }                   
  }                     
}                       

			/*  return no error                           */
return(message);

}                       /*  d26_u60_write_access_point                */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u61_find_break_access_point                       */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        29.11.90                                              */
/*                                                                    */
/* DESCRIPTION: This function finds the record containing the DSA-    */
/*              object which belongs as a master knowledge to the     */
/*              longest part of dninfo.                               */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dninfo      D26_dninfo *    information about object found    */
/*      mk          Long            index for master knowledge in DN  */
/*                                  list                              */
/*      acp_idx     Short *         index for PSAP-Address in PSAP-   */
/*                                  list                              */
/*      psap_at     D26_at *        pointer to AT-entry of PSAP-addr. */
/*      shb_at      D26_at *        pointer to AT-entry of Shadowed-  */
/*                                  by-attribute                      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      mk          Long            index for master knowledge in DN- */
/*                                  list                              */
/*      sh_by       Long *          collected shadowed by attribute   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   Short           error referrals could be written  */
/*      D2_NFOUND                   error referrals could not be      */
/*                                  written                           */
/*      D2_OUT_OF_SCOPE             no referral within specified scope*/
/*      D26_ERROR                   hard error occurred               */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_u61_find_break_access_point(D26_dninfo *dninfo,
    signed32 mk, signed16 *acp_idx, D26_at *psap_at)

{                               /*  d26_u61_find_break_access_point   */

  D26_rdninfo *l_rdn;       /* last RDN of superior node              */
			    /* object classes of first level object   */
  signed32    ob_cl[D26_OCL_VAL_COUNT + 1];
			    /* indices into DN list for ACL           */
  D26_acl     acl[D2_ACLIDX_MAX];
  boolean     db_access;    /* indicates intermediate DB-access       */
  signed16    ret_value;    /* return value from find access point    */

for                     /*  all nameparts of DN                       */
   (D2_EVER)
{                       /*  try to find access point                  */
  if                    /*  access point found                        */
    ((ret_value = d26_u35_find_access_point(mk,D2_SOBJ,acp_idx,psap_at,
     &db_access)) == D2_NO_ERR)
  {                     /*  break loop                                */
    DCE_SVC_LOG((GDS_S_ACC_POINT_FOUND_MSG,
      d26_accp_list[*acp_idx].d26_dsaname));
    break;
  }
  else if               /*  access point is out of scope              */
       (ret_value == D2_OUT_OF_SCOPE)
  {                     /*  return out of scope                       */
    return(D2_OUT_OF_SCOPE);
  }
  else if               /*  parent of object is root                  */
       (--dninfo->d26_rd_anz == 0)
  {                     /*  set master to be DSA of scheme object     */
    mk = d26_u64_first_level_mk();
    D27_022_ENTER_TIME_TRACE(GDS_S_FIRST_LEVEL_MK_READ,NULL,0,0);
  }
  else if               /*  root was reached already                  */
	 (dninfo->d26_rd_anz < 0)
  {                     /*  break loop without referrals              */
    dninfo->d26_rd_anz = 0;
    return(D2_NFOUND);
  }
  else
  {                     /*  search parent object                      */
    switch(d26_i31_find_object(dninfo,&l_rdn,d26_ob_record))
    {
      case D2_NO_ERR:   /*  read master knowledge                     */
	d26_i36_read_ocl_acl_mk(dninfo,d26_ob_record,ob_cl,acl,&mk);
	break;
      case D2_NFOUND:   /*  return NOT FOUND                          */
	dce_svc_printf(GDS_S_DB_INCONSISTENT_MSG);
	return(D2_NFOUND);
      case D26_ERROR:   /*  return hard error                         */
	return(D26_ERROR);
    }   
  }                     
  if                    /*  DSA itself is master                      */
    (mk == D26_OWNNAME)
  {                     /*  break loop without referrals              */
    return(D2_NFOUND);
  }                     
}                       

			/*  return no error                           */
return(D2_NO_ERR);

}                       /*  d26_u61_find_break_access_point           */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u62_write_referral                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        29.11.90                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the referral structure and       */
/*              access points into message.                           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      output      String *        pointer to beginning of message   */
/*      message     String          actual message pointer            */
/*      referral    D26_referral *  referral information              */
/*      output_len  Int *           maximum length of message         */
/*      remlen      Int *           remaining length for message      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of message         */
/*      remlen      Int *           remaining length for message      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String      message         pointer to byte after     */
/*                                          written message.          */
/*                          D2_NULL         error occurred.           */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_u62_write_referral(byte **output, byte *message,
    D26_referral *referral, signed32 *output_len, signed32 *remlen)

{                               /*  d26_u62_write_referral            */

  register signed16 i;          /* loop variable                      */

if                      /*  referral is not a dummy                   */
  (referral->d26_ignore == FALSE && referral->d26_route == FALSE)
{                       /*  check whether DSP was coming in           */
  if                    /*  error from writing referral header        */
    ((message = d26_u36_write_referral_header(output,message,referral,
     output_len,remlen)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  write all the access points               */
    for                 /*  all DN-List-indices for access points     */
       (i = 0; i < referral->d26_acp_count; i++)
    {                   /*  write access point                        */
      if                /*  error from writing access point           */
	((message = d26_u60_write_access_point(referral->d26_acp[i],output,
	 message,output_len,remlen)) == NULL)
      {                 /*  return error                              */
	return(NULL);
      }                 
    }                   
  }                     
}                       

			/*  return message                            */
return(message);

}                       /*  d26_u62_write_referral                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u63_write_error_referral                          */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        13.05.91                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the error referrals if required  */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      output      String *        pointer to error message to write */
/*      output_len  Int *           maximum length of error message   */
/*	referral    D26_referral *  pointer to referral structure.    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of error     */
/*                                  message                           */
/*      output_len  Int *           maximum length of error message   */
/*      remlen      Int *           length of remaining message       */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   Short           error referrals could be written. */
/*      D26_ERROR                   hard error occurred.              */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*      d26_chain_arg   D23_chain_arg *     pointer to chaining       */
/*                                          arguments                 */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*      d26_u40_check_buffer()                                        */
/*      d26_u62_write_referral()                                      */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u63_write_error_referral(byte **output, signed32 *output_len,
    signed32 *remlen, D26_referral *referral)

{                               /*  d26_u63_write_error_referral      */

  signed16 len;                     /* length of data to write        */
  byte     *message;                /* message to write               */

message = *output;
*remlen = *output_len;
len = sizeof(D23_rserror);

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                       /*  return error                              */
  return(D26_ERROR);
}
else
{                       /*  write error values                        */
  ((D23_rserror *)message)->d23_Zretcod  = D2_CALL_ERR;
  ((D23_rserror *)message)->d23_Zerrcls  = D2_PAR_ERR;
  ((D23_rserror *)message)->d23_Zerrval  = D2_REF_ERROR;
  ((D23_rserror *)message)->d23_Zversion = D23_V1988;
  message += len;
  *remlen -= len;

  if                    /*  chaining arguments are present            */
    (d26_chain_arg != (D23_chain_arg *)NULL)
  {                     /*  write the DSA-Referral structure          */
    len = sizeof(D23_drf_info);
    if                  /*  error from check buffer                   */
      ((message = d26_u40_check_buffer(len,output_len,remlen,output,
       message)) == NULL)
    {                   /*  return error                              */
      return(D26_ERROR);
    }
    else
    {                   /*  write DSA-referral tag                    */
      ((D23_drf_info *)message)->d23_drftag = D23_DRFTAG;
      ((D23_drf_info *)message)->d23_drf_max_tr = 0;
      *remlen -= len;
      message += len;
    }                   
  }                     
  if                    /*  error from writing referral               */
    ((message = d26_u62_write_referral(output,message,referral,
     output_len,remlen)) == NULL)
  {                     /*  return error                              */
    return(D26_ERROR);
  }
  else
  {                     /*  check chaining arguments                  */
    len = sizeof(D23_eom);
    if                  /*  error from check buffer                   */
      ((message = d26_u40_check_buffer(len,output_len,remlen,output,
       message)) == NULL)
    {                   /*  return error                              */
      return(D26_ERROR);
    }
    else
    {                   /*  write EOM tag                             */
      ((D23_eom *)message)->d23_eomtag    = D23_EOMTAG;
      ((D23_eom *)message)->d23_eom_oqoff = 0L;
      *remlen -= len;
    }                   
  }                     
}                       

			/*  return no error                           */
return(D2_NO_ERR);

}                       /*  d26_u63_write_error_referral              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u64_first_level_mk                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        31.07.91                                              */
/*                                                                    */
/* DESCRIPTION: This function searches for a first level object which */
/*              is mastered by the current DSA. If there is no such   */
/*              object, the master knowledge of the first level object*/
/*              that is superior to the current DSA is returned.      */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= -1       Long    first level master knowledge              */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_u64_first_level_mk(void)

{                                /*  d26_u64_first_level_mk           */

  signed16    filenr;       /* actual file number                     */
  D26_dninfo  dninfo;       /* information about own DSA object       */
  D26_rdninfo *l_rdn;       /* last RDN of own DSA object             */
			    /* object classes of first level object   */
  signed32    ob_cl[D26_OCL_VAL_COUNT + 1];
			    /* indices into DN list for ACL           */
  D26_acl     acl[D2_ACLIDX_MAX];
  signed32    mk;           /* master knowledge                       */

  register signed16    ob_nr;   /* loop variable for SRT              */
  register D26_srt     *srt;    /* pointer to SRT entry               */
  register D26_ocl_arc *ocl_arc;/* loop variable for object class arcs*/

for                                 /*  all srt entries               */
   (ob_nr = 0, srt = d26_srt; ob_nr < d26_nosrt; ob_nr++, srt++)
{                                   /*  check whether first level     */
  if                                /*  entry found                   */
    (srt->d26s_par_nr == D26_ROOT)
  {                                 /*  scan assigned object classes  */
    for                             /*  all object files              */
       (filenr = 1; filenr < d26_nofile; filenr++)
    {                               /*  check whether object file     */
				    /*  contains objects whith this   */
				    /*  structure rule                */
      for                           /*  all assigned object class arcs*/
	 (ocl_arc = srt->d26s_ocl_arc; ocl_arc->d26s_oclass != -1; ocl_arc++)
      {                             /*  check whether file contains   */
				    /*  objects of this structure     */
	if                          /*  file contains objects of this */
				    /*  structure                     */
	  (filenr == d26_oct[ocl_arc->d26s_oclass].d26c_filenr)
	{                           /*  break loop, file must be      */
				    /*  searched                      */
	  break;
	}                           
      }                             
      if                            /*  file has to be searched       */
	(ocl_arc->d26s_oclass != -1)
      {                             /*  search file                   */
	if                          /*  file contains first level obj.*/
	  (d26_i59_m_obj_in_file(ob_nr,filenr) == TRUE)
	{                           /*  return "own name"             */
	  return((signed32)D26_OWNNAME);
	}                           
      }                             
    }                               
  }                                 
}                                   

dninfo.d26_rd_anz = 0;
d26_u06_get_name(d26_myname,&dninfo);
		/*  find first namepart of own DSA-object             */
dninfo.d26_rd_anz = 1;
if              /*  first namepart found                              */
  (d26_i31_find_object(&dninfo,&l_rdn,d26_ob_record) == D2_NO_ERR)
{                       /*  read master knowledge                     */
  d26_i36_read_ocl_acl_mk(&dninfo,d26_ob_record,ob_cl,acl,&mk);
  return(mk);
}
else
{                     /*  return own name                           */
  return((signed32)D26_OWNNAME);
}                       

}                       /*  d26_u64_first_level_mk                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u68_write_trace_info                              */
/*                                                                    */
/* AUTHOR:      Kraemer, AP 11                                        */
/* DATE:        14.04.92                                              */
/*                                                                    */
/* DESCRIPTION: This function copies the trace information from the   */
/*              chaining arguments into the message and enhances it   */
/*              by the trace item for the own DSA.                    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      output      String *        pointer to beginning of message to*/
/*                                  write                             */
/*      output_len  Int *           maximum length of message         */
/*      message     String          actual message pointer            */
/*      remlen      Int *           remaining message length          */
/*      referral    D26_referral *  referral to use                   */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*      output      String *        pointer to beginning of message to*/
/*                                  write                             */
/*      output_len  Int *           maximum length of message         */
/*      remlen      Int *           remaining message length          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      String  != D2_NULL          actual message pointer            */
/*                 D2_NULL          hard error occurred.              */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_u68_write_trace_info(byte **output, signed32 *output_len,
    byte *message, signed32 *remlen, D26_referral *referral)

{                       /*  d26_u68_write_trace_info                  */

  Bool     complete;        /* indicates whether name was completely  */
			    /* decoded                                */
  signed16 len;             /* length of data actually to write       */
  signed16 off;             /* offset to next structure               */
  byte     *chain_arg;      /* pointer to chaining arguments to copy  */

  signed16 bo_len;          /* length of base object                  */
  byte     norm_base_obj[D2_DNL_MAX + 1];   /* normed base object     */

if                      /*  chaining arguments are present            */
  (d26_chain_arg != (D23_chain_arg *)NULL)
{                       /*  copy old trace informations               */
  chain_arg = (byte *)(d26_chain_arg + 1);
  while                 /*  still trace information present           */
       (((D23_trace_info *)chain_arg)->d23_trtag == D23_TRTAG)
  {                     /*  copy trace information                    */
    len = sizeof(D23_trace_info);
    if                  /*  error from check buffer                   */
      ((message = d26_u40_check_buffer(len,output_len,remlen,output,
       message)) == NULL)
    {                   /*  set return value                          */
      return(NULL);
    }
    else
    {                   /*  write trace information                   */
      *(D23_trace_info *)message = *(D23_trace_info *)chain_arg;
      message   += sizeof(D23_trace_info);
      *remlen   -= sizeof(D23_trace_info);
      chain_arg += sizeof(D23_trace_info);
      while             /*  DN following                              */
	   (((D23_ds_name *)chain_arg)->d23_dstag == D23_NAMTAG)
      {                 /*  copy DN                                   */
	complete = ((D23_ds_name *)chain_arg)->d23_dscomplete;
	off = ((D23_ds_name *)chain_arg)->d23_dsoff;
	len = sizeof(D23_ds_name) + off;
	if              /*  error from check buffer                   */
	  ((message = d26_u40_check_buffer(len,output_len,remlen,output,
	   message)) == NULL)
	{               /*  set return value                          */
	  return(NULL);
	}
	else
	{               /*  copy DN structure                         */
	  *(D23_ds_name *)message = *(D23_ds_name *)chain_arg;
	  message   += sizeof(D23_ds_name);
	  *remlen   -= sizeof(D23_ds_name);
	  chain_arg += sizeof(D23_ds_name);
	  strcpy((char *)message,(char *)chain_arg);
	  message   += off;
	  *remlen   -= off;
	  chain_arg += off;
	  if            /*  undecoded part following                  */
	    (complete == FALSE)
	  {             /*  set offset                                */
	    off = ((D23_av_value *)chain_arg)->d23_avoff;
	    len = sizeof(D23_av_value) + off;
	    if          /*  error from check buffer                   */
	      ((message = d26_u40_check_buffer(len,output_len,remlen,output,
	       message)) == NULL)
	    {           /*  set return value                          */
	      return(NULL);
	    }
	    else
	    {           /*  copy attribute value                      */
	      *(D23_av_value *)message = *(D23_av_value *)chain_arg;
	      message   += sizeof(D23_av_value);
	      *remlen   -= sizeof(D23_av_value);
	      chain_arg += sizeof(D23_av_value);
	      memcpy(message,chain_arg,off);
	      message   += off;
	      *remlen   -= off;
	      chain_arg += off;
	    }           
	  }             
	}               
      }                 
    }                   
  }                     
}                       

len = sizeof(D23_trace_info) + (sizeof(D23_ds_name) + D2_DNL_MAX);
if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,output_len,remlen,output,message))
   == NULL)
{                     /*  set return value                          */
  return(NULL);
}
else
{                     /*  write own trace information               */
  ((D23_trace_info *)message)->d23_trtag   = D23_TRTAG;
  ((D23_trace_info *)message)->d23_op_prog = *d26_op_progress;
  message += sizeof(D23_trace_info);
  *remlen -= sizeof(D23_trace_info);

  ((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
  len = strlen((char *)d26_myexact_name) + 1;
  off = D2_ALIGN(len);
  ((D23_ds_name *)message)->d23_dscomplete = TRUE;
  ((D23_ds_name *)message)->d23_dslen = len;
  ((D23_ds_name *)message)->d23_dsoff = off;
  message += sizeof(D23_ds_name);
  *remlen -= sizeof(D23_ds_name);
  strcpy((char *)message,(char *)d26_myexact_name);
  message += off;
  *remlen -= off;
  if                    /*  incoming chaining arguments are present   */
    (d26_chain_arg != (D23_chain_arg *)NULL)
  {                     /*  check whether incoming target object is   */
			/*  different from the outgoing one           */
    if                  /*  base object is not root                   */
      (strcmp((char *)d26_base_object,""))
    {                   /*  norm it                                   */
      bo_len = strlen((char *)d26_base_object) + 1;
      d27_007_norm_name((char *)d26_base_object,bo_len,
	(char *)norm_base_obj);
      d27_012_remove_blanks((char *)norm_base_obj,bo_len,
	(char *)norm_base_obj);
      d27_018_sort_name(norm_base_obj);
    }
    else
    {                   /*  create an empty string for comparison     */
      strcpy((char *)norm_base_obj,"");
    }                   

    if                  /*  target objects are different              */
      (strcmp((char *)norm_base_obj,(char *)referral->d26_target) != 0 ||
       d26_bo_encoded_part != (D23_av_value *)NULL ||
       referral->d26_targ_ep != (D23_av_value *)NULL)
    {                   /*  write old target object into trace info.  */
      len = strlen((char *)d26_base_object) + 1;
      len = len > 1 ? len : 0;
      off = D2_ALIGN(len);
      complete = d26_bo_encoded_part == (D23_av_value *)NULL ?
					TRUE : FALSE;
      if                /*  error from check buffer                   */
	((message = d26_u40_check_buffer(sizeof(D23_ds_name) + off,
	 output_len,remlen,output,message)) == NULL)
      {                 /*  set return value                          */
	return(NULL);
      }
      else
      {                 /*  write DN structure                        */
	((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
	((D23_ds_name *)message)->d23_dscomplete = complete;
	((D23_ds_name *)message)->d23_dslen = len;
	((D23_ds_name *)message)->d23_dsoff = off;
	message += sizeof(D23_ds_name);
	*remlen -= sizeof(D23_ds_name);
	strcpy((char *)message,(char *)d26_base_object);
	message += off;
	*remlen -= off;
	if              /*  undecoded part following                  */
	  (complete == FALSE)
	{               /*  set offset                                */
	  off = d26_bo_encoded_part->d23_avoff;
	  len = sizeof(D23_av_value) + off;
	  if            /*  error from check buffer                   */
	    ((message = d26_u40_check_buffer(len,output_len,remlen,output,
	     message)) == NULL)
	  {             /*  set return value                          */
	    return(NULL);
	  }
	  else
	  {             /*  copy attribute value                      */
	    *(D23_av_value *)message = *d26_bo_encoded_part;
	    message   += sizeof(D23_av_value);
	    *remlen   -= sizeof(D23_av_value);
	    memcpy(message,(byte *)(d26_bo_encoded_part + 1),off);
	    message   += off;
	    *remlen   -= off;
	  }             
	}               
      }                 
    }                   
  }                     
}                       

return(message);

}                       /*  d26_u68_write_trace_info                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u70_make_error_referrals                          */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        14.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the error referrals if base      */
/*              object couldn't be found.                             */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16        error referral created            */
/*      D2_NFOUND                   error referral not found          */
/*      D2_OUT_OF_SCOPE             no referral within specified scope*/
/*      D2_ABANDONED                request was abandoned             */
/*      D26_ERROR                   hard error occurred               */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u70_make_error_referrals(
    D26_dninfo *dninfo, /* IN    -  information about object found    */
			/* IN    -  normed dereferenced object name   */
    D2_name_string dname,
    signed16 al_rdns,   /* IN    -  number of aliased RDNs of base o. */
			/* IN    -  exact dereferenced object name    */
    D2_name_string ename,
			/* IN    -  unresolved part of base object    */
    D2_name_string unresolved,
			/* IN    -  exact unresolved part             */
    D2_name_string eunres,
			/* IN    -  undecoded part of base object     */
    D23_av_value *undecoded,
			/* IN    -  index for master knowledge in     */
    signed32 mk,        /*          DN list                           */
			/* IN    -  indicates whether only the target */
    boolean entry_only) /*          object should be searched         */




{                               /*  d26_u70_make_error_referrals      */

  D26_at   *psap_at;            /* pointer to AT-entry for PSAP-      */
				/* address                            */
				/* space for exact target object      */
  byte     target[D2_DNL_MAX + 1];
				/* space for normed target object     */
  byte     ntarget[D2_DNL_MAX + 1];
  signed16 acp_idx;             /* index of Access-point in list      */
  void     **tsearch_result;    /* result returned by tsearch         */

  signed16 ret_value;           /* return value                       */

  psap_at = d26_at + d26_psap_idx;

  if                            /* operation was abandoned            */
    (d26_abandoned == TRUE)
  {                             /* return immediately                 */
    return(D2_ABANDONED);
  }
  else
  {                             /* try to find break access point     */
    if                          /* break access point not found       */
      ((ret_value = d26_u61_find_break_access_point(dninfo,mk,&acp_idx,
       psap_at)) != D2_NO_ERR)
    {                           /* return error                       */
      D27_022_ENTER_TIME_TRACE(GDS_S_NO_BREAK_ACP,NULL,0,0);
      return(ret_value);
    }
    else
    {                           /* check dereferenced object          */
      D27_022_ENTER_TIME_TRACE(GDS_S_BREAK_ACP_READ,NULL,0,0);
      if                        /* dereferenced object is empty       */
	(strlen((char *)ename) == 0)
      {                         /* check the unresolved part          */
	if                      /* unresolved part is empty           */
	  (strlen((char *)unresolved) == 0)
	{                       /* set target object empty            */
	  *target = *ntarget = D2_EOS;
	}
	else
	{                       /* write only unresolved part         */
	  sprintf((char *)target,"%s",eunres);
	  sprintf((char *)ntarget,"%s",unresolved);
	}
      }
      else
      {                         /* check unresolved part              */
	if                      /* unresolved part is empty           */
	  (strlen((char *)unresolved) == 0)
	{                       /* write dereferenced object only     */
	  sprintf((char *)target,"%s",ename);
	  sprintf((char *)ntarget,"%s",dname);
	}
	else
	{                       /* write both parts                   */
	  sprintf((char *)target,"%s%c%s",ename,D2_RDN_SEP,eunres);
	  sprintf((char *)ntarget,"%s%c%s",dname,D2_RDN_SEP,unresolved);
	}
      }
      if                        /* error from creating base object    */
				/* referral                           */
	(d26_u42_make_base_obj_referral(dninfo,ntarget,al_rdns,target,
	 undecoded,psap_at,acp_idx,
	 *unresolved == D2_EOS ? mk : D26_IDX_NOT_DEF,entry_only,FALSE)
	 != D2_NO_ERR)
      {                         /* return error                       */
	return(D26_ERROR);
      }
      else
      {                         /* increment referrals count          */
	tsearch_result = D27_TSEARCH(svc_c_sev_warning,
	  (void *)(d26_referral + d26_ref_count),&d26_ref_root,
	  d26_u38_refcmp);
	if                      /* referral could be inserted         */
	  (*tsearch_result == (void *)(d26_referral + d26_ref_count))
	{                       /* increment count of referrals       */
	  DCE_SVC_LOG((GDS_S_REF_GENERATED_MSG));
	  d26_ref_present = TRUE;
	  d26_ref_count++;
	}
	else
	{                       /* free the target memory             */
	  free(d26_referral[d26_ref_count].d26_target);
	  free(d26_referral[d26_ref_count].d26_etarget);
	}
      }
    }
  }

			/*  return no error                           */
  return(D2_NO_ERR);

}                       /*  d26_u70_make_error_referrals              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u72_make_p_res_referrals                          */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        14.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function creates  the partial result referrals   */
/*              for search and list.                                  */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dname       D26_name_string normed DN of base object          */
/*      ename       D26_name_string exact DN of base object           */
/*      dninfo      D26_dninfo *    information about object found    */
/*      al_rdns     Short           number of aliased RDNs of base o. */
/*      mk          Long            index for master knowledge in     */
/*                                  DN-list                           */
/*      sh_by       Long *          collected shadowed by attribute   */
/*      ob_nr       Short           object number for object          */
/*      subset      Short           indicates subset to search        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u72_make_p_res_referrals(
    D2_name_string dname,
    D2_name_string ename,
    D26_dninfo *dninfo,
    signed16 al_rdns,
    signed32 mk,
    signed16 ob_nr,
    signed16 subset)

{                               /*  d26_u72_make_p_res_referrals      */

  D26_at   *psap_at;                /* pointer to at-entry for PSAP-  */
				    /* address                        */
  signed16 acp_idx;                 /* index of Access-point in list  */
				    /* collected shadowed by attribute*/
  signed16 np_count;                /* count of nameparts of base     */
				    /* objects                        */
				    /* indicates that referral was    */
				    /* created                        */
  boolean  ref_flag = FALSE;
				    /* result returned by tsearch     */
  void     **tsearch_result;

if                      /*  referral limit not exceeded               */
  (d26_ref_count + 1 < D26_REF_BLOCKSIZE)
{                       /*  set AT-entry-pointer for PSAP-Address     */
  psap_at  = d26_at + d26_psap_idx;
  np_count = dninfo->d26_rd_anz;
  if                    /*  DSA is master of base object              */
    (mk == D26_OWNNAME)
  {                     /*  logg that                                 */
    DCE_SVC_LOG((GDS_S_DSA_MOF_BASE_OBJECT_MSG));
    if                  /*  Subtree or one level is searched          */
      (subset != D2_BASE_OBJECT)
    {                   /*  create dummy referral                     */
      if                /*  base object referral is created           */
	(d26_u42_make_base_obj_referral(dninfo,dname,al_rdns,ename,
	 (D23_av_value *)NULL,psap_at,D26_OWNNAME,mk,FALSE,TRUE)
	 == D2_NO_ERR)
      {                 /*  set referral flag                         */
	ref_flag = TRUE;
      }                 
    }                   
  }
  else
  {                     /*  find break access point                   */
    if                  /*  break access point is found               */
      ((d26_u61_find_break_access_point(dninfo,mk,&acp_idx,psap_at))
       == D2_NO_ERR)
    {                   /*  log success                               */
      if                /*  base object referral is created           */
	(d26_u42_make_base_obj_referral(dninfo,dname,al_rdns,ename,
	 (D23_av_value *)NULL,psap_at,
	 acp_idx,mk,subset == D2_BASE_OBJECT ? TRUE : FALSE,TRUE)
	 == D2_NO_ERR)
      {                 /*  set referral flag                         */
	ref_flag = TRUE;
      }                 
    }
    else
    {                   /*  log error                                 */
      if                /*  Subtree or one level is searched          */
	(subset != D2_BASE_OBJECT)
      {                 /*  create dummy referral                     */
	if              /*  base object referral is created           */
	  (d26_u42_make_base_obj_referral(dninfo,dname,al_rdns,ename,
	   (D23_av_value *)NULL,psap_at,D26_OWNNAME,mk,FALSE,TRUE)
	   == D2_NO_ERR)
	{               /*  set referral flag                         */
	  ref_flag = TRUE;
	}               
      }                 
    }                   
  }                     

  if                    /*  referral was created                      */
    (ref_flag == TRUE)
  {                     /*  insert referral into tree                 */
    tsearch_result = D27_TSEARCH(svc_c_sev_warning,
      (void *)(d26_referral + d26_ref_count),&d26_ref_root,d26_u38_refcmp);
    if                  /*  referral could be inserted                */
      (*tsearch_result == (void *)(d26_referral + d26_ref_count))
    {                   /*  increment count of referrals              */
      DCE_SVC_LOG((GDS_S_REF_GENERATED_MSG));
      if                /*  referral was not a dummy                  */
	(d26_referral[d26_ref_count].d26_ignore == FALSE)
      {                 /*  reset referral indicator                  */
	d26_ref_present = TRUE;
      }                 
      d26_ref_count++;
    }
    else
    {                   /*  free memory for target names              */
      free(d26_referral[d26_ref_count].d26_target);
      free(d26_referral[d26_ref_count].d26_etarget);
    }                   
  }                     

  dninfo->d26_rd_anz = np_count;
  if                    /*  subtree or one level must be searched     */
    (subset != D2_BASE_OBJECT)
  {                     /*  look for subtree referrals                */
    d26_i67_make_st_referrals(psap_at,subset,dname,dninfo,ob_nr,mk);
  }                     
}                       

}                       /*  d26_u72_make_p_res_referrals              */
