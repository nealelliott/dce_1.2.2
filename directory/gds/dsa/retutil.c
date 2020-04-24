/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: retutil.c,v $
 * Revision 1.1.743.3  1996/02/18  18:15:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:28  marty]
 *
 * Revision 1.1.743.2  1995/12/08  15:51:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:05  root]
 * 
 * Revision 1.1.739.1  1994/10/13  13:08:00  keutel
 * 	OT 12584: DSA core dump for search
 * 	[1994/10/13  12:19:33  keutel]
 * 
 * Revision 1.1.736.4  1994/06/21  14:46:27  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:55:05  marrek]
 * 
 * Revision 1.1.736.3  1994/05/10  15:53:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:29:44  marrek]
 * 
 * Revision 1.1.736.2  1994/03/23  15:14:05  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:26:55  keutel]
 * 
 * Revision 1.1.736.1  1994/02/22  17:37:51  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:27:22  marrek]
 * 
 * Revision 1.1.734.2  1993/10/14  17:17:00  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:15:37  keutel]
 * 
 * Revision 1.1.734.1  1993/10/13  17:31:44  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:31:33  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  16:16:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:31:50  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  15:00:55  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  11:36:29  marrek
 * 	Fixes for OT 6587
 * 	[1993/02/01  15:46:48  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  19:45:10  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:15  bbelch]
 * 
 * Revision 1.1.4.4  1992/11/27  20:39:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:28:45  marrek]
 * 
 * Revision 1.1.4.3  1992/10/06  18:36:38  sommerfeld
 * 	Comment out #ident.
 * 	[1992/09/30  21:30:26  sommerfeld]
 * 
 * Revision 1.1.4.2  1992/09/28  12:43:31  marrek
 * 	Changes for correct u31 error output and add missing output_len
 * 	parameter in d26_u04_set_name_error_apdu() .
 * 	[1992/09/28  12:41:21  marrek]
 * 
 * Revision 1.1.2.4  1992/07/17  19:59:06  weisman
 * 	Checked in at OSF for SNI
 * 	[1992/07/17  19:27:31  weisman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: retutil.c,v $ $Revision: 1.1.743.3 $ $Date: 1996/02/18 18:15:58 $";
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
/* NAME         : retutil.c       [retutil]                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 04.05.88                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the internal search function  */
/*                  and utilities used by internal search.            */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 04.05.88| birth                          | ek |         */
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

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

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
/*                D E C L A R A T I O N S        DATA                 */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static signed16 d26_u07_resolve_name(boolean lock_mode, signed16 fct_tag,
    D26_dninfo *dn_info, boolean *al_flag, signed16 *ob_nr,
    D2_name_string norm_base_obj, D2_name_string e_target_obj,
    D2_name_string deref_obj, D2_name_string *unresolved,
    D2_name_string *eunres);
static signed16 d26_u32_search_base_object(D2_name_string dname,
    D2_name_string exname, D26_dninfo *dninfo, signed16 ob_nr,
    signed32 *ob_cl, D23_rq_info *r_info, D23_ft1_info *f_info,
    signed32 *output_len, signed32 *remlen, signed16 fct_tag, byte **output,
    D26_attr_info *attr, D26_acl *acl, signed32 mk, boolean *recur);
static signed16 d26_u33_handle_name_res_result(signed16 ret_value,
    D2_name_string dname, D26_dninfo *dninfo, signed32 mk,
    D2_name_string ename, D2_name_string unresolved, D2_name_string eunres,
    byte **output, signed32 *output_len, signed32 *remlen);
static signed16 d26_u47_search_set(D2_subset subset, D2_name_string dname,
    D2_name_string ename, D26_dninfo *dninfo, signed16 bo_nr,
    signed32 *ob_cl, D23_rq_info *r_info, D23_ft1_info *f_info,
    signed32 *output_len, signed32 *remlen, signed16 fct_tag, byte **output,
    D26_acl *acl, signed32 mk);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u04_set_name_error_apdu                         */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 28.9. 90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will write a name error message in    */
/*                the error message block.                            */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      output          String *      error message to write to       */
/*      output_len      Int *         maximum length of error message */
/*      remlen          Int *         remaining length of message     */
/*      err_value       Short         name problem                    */
/*      mk              Long          index of master DSA in DN-list  */
/*      D26_dninfo *    dn_info       information about matched object*/
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      output          String *      error messgage to write         */
/*      output_len      Int *         length of error message         */
/*      remlen          Int *         remaining length of message     */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR       Short         everything ok                   */
/*      D26_ERROR       Short         error occurred                  */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u04_set_name_error_apdu(
    byte       **output,
    signed32   *output_len,
    signed32   *remlen,
    signed16   err_value,
    signed32   mk,
    D26_dninfo *dn_info)

{                                   /*  d26_u04_set_name_error_apdu   */

  byte     *message;            /* pointer to IPC-message             */
  signed32 meslen;              /* length of written message          */
				/* memory for exact name              */
  byte     exact_dn[D2_DNL_MAX];
  signed16 change;              /* indicates RDN where MK changes     */
  signed16 dnlen;               /* length of exact name               */

  signed16 ret_value = D2_NO_ERR;   /* return value                   */

message = *output;

if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(sizeof(D23_rserror),output_len,remlen,
   output,message)) == NULL)
{                       /*  return error                              */
  ret_value = D26_ERROR;
}
else
{                       /*  write ENT tag                             */
				    /*  set error codes               */
  ((D23_rserror *)message)->d23_Zretcod  = D2_CALL_ERR;
  ((D23_rserror *)message)->d23_Zerrcls  = D2_PAR_ERR;
  ((D23_rserror *)message)->d23_Zerrval  = err_value;
  ((D23_rserror *)message)->d23_Zversion = D23_V1988;
  message += sizeof(D23_rserror);
  *remlen -= sizeof(D23_rserror);

  if                    /*  master information is required            */
    (d26_serv_contr->d23_scopt & D2_NOCACHE)
  {                     /*  find master part of the object            */
    if                  /*  master part cannot be read                */
      (d26_u57_find_master_part(dn_info,mk,&change) != D2_NO_ERR)
    {                   /*  return error                              */
      ret_value = D26_ERROR;
    }
  }
  dnlen = d26_i48_get_exact_name(d26_ob_record,dn_info,0,exact_dn);
  if                    /*  error from writing distinguished name     */
    ((message = d26_u49_write_dist_name(message,output,exact_dn,dnlen,
     remlen,output_len,D23_NAMTAG)) == NULL)
  {                     /*  return error                              */
    ret_value = D26_ERROR;
  }
  else if               /*  error from check buffer                   */
	 ((message = d26_u40_check_buffer(sizeof(D23_eom),output_len,remlen,
	  output,message)) == NULL)
  {                     /*  return error                              */
    ret_value = D26_ERROR;
  }
  else
  {                     /*  put "end of message"-flag to message      */
    ((D23_eom *)message)->d23_eomtag    = D23_EOMTAG;
    ((D23_eom *)message)->d23_eom_oqoff = 0L;
    *remlen     -= sizeof(D23_eom);
  }
}

if                      /*  error occurred                            */
  (ret_value == D26_ERROR)
{                       /*  write simple error APDU                   */
  d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
			 D2_UNWILLING,&meslen);
  *remlen -= meslen;
}

return(ret_value);

}                                   /*  d26_u04_set_name_error_apdu   */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u07_resolve_name                                */
/*                                                                    */
/* AUTHOR       : Kraemer,D AP 11                                     */
/* DATE         : 01.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will look up the local database for   */
/*                the base object. It returns the longest matching    */
/*                object, described by dn_info, d26_ob_record,        */
/*                al_flag, ob_nr and deref_obj. It sets the unresolved*/
/*                pointer to the unresolved part of the base object.  */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      Bool            lock_mode   a flag indicating whether the     */
/*                                  records should be locked (for     */
/*                                  update) or not                    */
/*      Short           fct_tag     a flag indicating the calling     */
/*                                  function                          */
/*      D2_name_string  norm_base_obj   normed DN of base object      */
/*      D2_name_string  e_target_obj    exact DN of target object     */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      D26_dninfo      *dn_info    information about base object     */
/*      Bool            *al_flag    indicates, whether base object is */
/*                                  alias                             */
/*      Short           *ob_nr      object number of base object      */
/*      D2_name_string  deref_obj   DN of found object                */
/*      D2_name_string  *unresolved unresolved part of the base object*/
/*      D2_name_string  *eunres     exact name of unresolved part of  */
/*                                  the target object                 */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR           Short   no error occured                  */
/*      D2_NFOUND                   no base object found              */
/*      D2_NMSYNTAX_ERROR           syntax error in DN of base object */
/*      D2_ALIAS_ERROR              orphan alias detected             */
/*      D2_ON_ALIAS_NOT_ALLOWED     service controls prohibit alias   */
/*                                  dereferencing                     */
/*      D2_INVALID_REFERENCE        invalid reference in chaining     */
/*                                  arguments                         */
/*      D2_UNABLE_TO_PROCEED        DSA is unable to proceed in name  */
/*                                  resolution                        */
/*      D2_DIT_INCONSISTENT         inconsistent database             */
/*      D26_ERROR                   system error occurred             */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_u07_resolve_name(
    boolean        lock_mode,
    signed16       fct_tag,
    D26_dninfo     *dn_info,
    boolean        *al_flag,
    signed16       *ob_nr,
    D2_name_string norm_base_obj,
    D2_name_string e_target_obj,
    D2_name_string deref_obj,
    D2_name_string *unresolved,
    D2_name_string *eunres)

{                                   /*  d26_u07_resolve_name          */

  signed32       al_idx;            /* DN-List-Index of aliased object*/
  boolean        go_on;             /* indicates that name resolution */
				    /* has to be continued            */

  D26_rdninfo    *l_rdn;            /* last converted RDN             */
  D26_ava        *l_ava;            /* last converted AVA             */
				    /* points to part of name, that   */
				    /* cannot be converted            */
  D2_name_string problem_name;
				    /* temporary copy of unresolved   */
				    /* part                           */
  byte           unres_tmp[D2_DNL_MAX];
				    /* temporary copy of exact name   */
				    /* of unresolved part             */
  byte           eunres_tmp[D2_DNL_MAX];
				    /* return value                   */
  signed16       ret_value = D2_NO_ERR;

go_on       = TRUE;

while                               /*  chance to proceed exists      */
     (go_on == TRUE)
{                                   /*  initialize name structure     */
  problem_name = norm_base_obj;
  dn_info->d26_rd_anz = 0;
  d26_u06_get_name(norm_base_obj,dn_info);
  if                                /*  nameparts are converted       */
    (dn_info->d26_rd_anz > 0)
  {                                 /*  set pointer to last AVA       */
    l_rdn = dn_info->d26_rdn + dn_info->d26_rd_anz - 1;
    l_ava = l_rdn->d26_r_ava + l_rdn->d26_r_nava - 1;
    problem_name = l_ava->d26_a_val + l_ava->d26_a_len;
  }                                 /*                                */
				    /*  read break object             */
  if                                /*  System error from reading     */
				    /*  break object                  */
    ((*unresolved = d26_u31_find_break_object(dn_info,norm_base_obj,
     deref_obj,d26_ob_record,lock_mode,fct_tag)) == NULL)
  {                                 /*  return error                  */
    return(D26_ERROR);
  }
  else
  {                                 /*  check name problem            */
    *eunres   = e_target_obj + (*unresolved - norm_base_obj);
    go_on     = FALSE;
    if                          /*  yet no alias detected             */
      (*al_flag == FALSE)
    {                           /*  check operation progress          */
      if                        /*  not enough nameparts resolved     */
	(d26_op_progress->d23_opnm_rs == D2_COMPLETED &&
	 (**unresolved != D2_EOS ||
	  d26_bo_encoded_part != (D23_av_value *)NULL) ||
	 d26_op_progress->d23_opnm_rs == D2_PROCEEDING &&
	 (signed16)d26_op_progress->d23_op_nxtrdn > dn_info->d26_rd_anz + 1)
      {                         /*  set the return value              */
	return(D2_INVALID_REFERENCE);
      }
      else if                   /*  just the last namepart not res.   */
	     (d26_op_progress->d23_opnm_rs == D2_PROCEEDING &&
	      (signed16)d26_op_progress->d23_op_nxtrdn
	      == dn_info->d26_rd_anz + 1)
      {                         /*  check whether DSP is processed    */
	if                      /*  DSP is processed                  */
	  (d26_chain_arg != (D23_chain_arg *)NULL)
	{                       /*  take reference type from chaining */
				/*  arguments                         */
	  return(d26_chain_arg->d23_ref_type == D2_NON_SPEC_SUBORDINATE ?
		 D2_UNABLE_TO_PROCEED : D2_INVALID_REFERENCE);
	}
	else
	{                       /*  take reference type as undefined  */
	  return(D2_UNABLE_TO_PROCEED);
	}
      }
    }
    if                              /*  break object is found         */
      (*unresolved > norm_base_obj)
    {                               /*  check whether alias found     */
      if                            /*  break object is alias         */
	((al_idx = d26_i47_get_al_references(dn_info->d26_filenr,
	 d26_ob_record)) >= 0)
      {                             /*  set alias flag                */
	*al_flag = TRUE;
	if                          /*  alias contains not more       */
				    /*  RDNs than already are aliased */
	  (dn_info->d26_rd_anz <= d26_al_rdns)
	{                           /*  set return value              */
	  ret_value = D2_ON_ALIAS_NOT_ALLOWED;
	}
	else if                     /*  aliases have to be derefd.    */
	       ((d26_serv_contr->d23_scopt & D2_DREFALIAS) == 0L)
	{                           /*  save unresolved part          */
	  strcpy((char *)unres_tmp,(char *)*unresolved);
	  strcpy((char *)eunres_tmp,(char *)*eunres);
	  if                        /*  error from dereferencing alias*/
	    ((d26_al_rdns = d26_i33_deref_alias(al_idx,norm_base_obj,
	     e_target_obj,dn_info)) < 0)
	  {                         /*  set return value              */
	    ret_value = D2_DIT_INCONSISTENT;
	  }
	  else
	  {                         /*  copy unresolved part behind   */
				    /*  the aliased object            */
	    strcat((char *)norm_base_obj,(char *)unres_tmp);
	    strcat((char *)e_target_obj,(char *)eunres_tmp);
	    go_on               = TRUE;
	    al_idx              = -1L;
	  }                         /*                                */
	}
	else if                     /*  Object not yet resolved       */
	       (**unresolved != D2_EOS ||
		d26_bo_encoded_part != (D23_av_value *)NULL)
	{                           /*  set return value              */
	  ret_value = D2_ON_ALIAS_NOT_ALLOWED;
	}                           /*                                */
      }
      else if                       /*  object not yet resolved       */
	     (**unresolved != D2_EOS)
      {                             /*  check problem name            */
	if                          /*  all converted nameparts found */
	  (*unresolved == problem_name)
	{                           /*  set Name syntax error         */
	  ret_value = D2_NMSYNTAX_ERROR;
	}
	else
	{                           /*  set Not found                 */
	  ret_value = D2_NFOUND;
	}                           /*                                */
	(*unresolved)++;
	(*eunres)++;
      }
      else if                       /*  undecoded nameparts present   */
	     (d26_bo_encoded_part != (D23_av_value *)NULL)
      {                             /*  set name syntax error         */
	ret_value = D2_NMSYNTAX_ERROR;
      }                             /*                                */
    }
    else if                       /*  object not yet resolved       */
	   (**unresolved != D2_EOS)
    {                               /*  check problem name            */
      if                            /*  all converted nameparts found */
	(*unresolved == problem_name)
      {                             /*  set Name syntax error         */
	ret_value = D2_NMSYNTAX_ERROR;
      }
      else
      {                             /*  set Not found                 */
	ret_value = D2_NFOUND;
      }                             /*                                */
    }
    else if                         /*  undecoded nameparts present   */
	   (d26_bo_encoded_part != (D23_av_value *)NULL)
    {                               /*  set name syntax error         */
      ret_value = D2_NMSYNTAX_ERROR;
    }
    else
    {                               /*  object not found              */
      ret_value = D2_NFOUND;
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */

*ob_nr = dn_info->d26_rd_anz > 0 ?
	 dn_info->d26_rdn[dn_info->d26_rd_anz - 1].d26_r_idx : D26_ROOT;
				    /*  return (return value)         */
return(ret_value);

}                                   /*  d26_u07_resolve_name          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u31_find_break_object                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function finds the "break object" belonging to   */
/*              dninfo; that means, it finds the longest part of      */
/*              dninfo that exists. If an alias was found, the alias- */
/*              references are supplied.                              */
/*              All nameparts found are written into deref.           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dninfo      D26_dninfo *    information about object to be    */
/*                                  found                             */
/*      assert      D2_name_string  tail of DN to work on             */
/*      lock_mode   Bool            indicates whether table must be   */
/*                                  locked.                           */
/*      fct_tag     Short           a flag indicating the calling     */
/*                                  function                          */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*      dninfo      D26_dninfo *    information about object found    */
/*      deref       D2_name_string  pointer, where nameparts have to  */
/*                                  be written to                     */
/*      record      String          pointer to DB - record            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     D2_name_string           pointer to the end of DN, that could  */
/*                              not be found still now.               */
/*     D2_NULL                  C-ISAM-error occurred                 */
/*                                                                    */
/*exoff ***************************************************************/

D2_name_string d26_u31_find_break_object(
    D26_dninfo     *dninfo,
    D2_name_string assert,
    D2_name_string deref,
    char           *record,
    boolean        lock_mode,
    signed16       fct_tag)

{                               /*  d26_u31_find_break_object         */

  D26_rdninfo    *rdn;              /* pointer to RDN                 */
  D26_rdninfo    *l_rdn;            /* last RDN                       */
  D26_ava        *l_ava;            /* last AVA in last RDN           */
  D2_name_string new_tail;          /* pointer to first nampart which */
				    /* was not found                  */

  register signed16 i,j;    /* loop variables                         */
  register D26_ava  *ava;   /* loop pointer to AVAs                   */

while                   /*  still RDN's to check                      */
     (dninfo->d26_rd_anz > 0)
{                       /*  find object                               */
  DCE_SVC_LOG((GDS_S_OBJ_LOOKUP_MSG,dninfo->d26_rd_anz));
  l_rdn = dninfo->d26_rdn + dninfo->d26_rd_anz - 1;
  l_ava = l_rdn->d26_r_ava + l_rdn->d26_r_nava - 1;
  new_tail = l_ava->d26_a_val + l_ava->d26_a_len;
  switch(d26_i31_find_object(dninfo,&l_rdn,record))
  {
    case D2_NO_ERR:
      for               /*  all RDNs found                            */
	 (i = 0, rdn = dninfo->d26_rdn; i < dninfo->d26_rd_anz; i++, rdn++)
      {                 /*  write RDN into distinguished name         */
	for             /*  all AVAs of RDN                           */
	   (j = 0, ava = rdn->d26_r_ava; j < rdn->d26_r_nava; j++, ava++)
	{               /*  convert attribute type to printable form  */
	  d27_301_oid_str(&d26_at[ava->d26_a_idx].d26a_type,(char *)deref);
	  deref += strlen((char *)deref);
	  sprintf((char *)deref,"%c%*.*s%c",D2_T61_REP,ava->d26_a_len,
		  ava->d26_a_len,ava->d26_a_val,D2_AVA_SEP);
	  rdn->d26_r_ava[j].d26_a_val = deref + 1;
	  deref += strlen((char *)deref);
	}               /*                                            */
	*(deref - 1) = D2_RDN_SEP;
      }                 /*                                            */
      *(deref - 1) = D2_EOS;
      return(new_tail);
    case D2_NFOUND:     /*  check function tag and lock_mode          */
      dninfo->d26_rd_anz =
	      (fct_tag != D23_ADDOBJT && fct_tag != D23_REMOBJT &&
	       fct_tag != D23_MODENTR && fct_tag != D23_REPLRDN)
	      || lock_mode == TRUE ? dninfo->d26_rd_anz - 1 : 0;
      break;
    case D26_ERROR:     /*  return NULL pointer                       */
      dninfo->d26_rd_anz = 0;
      return((D2_name_string)NULL);
  }   
}

			/*  return old DN                             */
return(assert);

}                       /*  d26_u31_find_break_object                 */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u32_search_base_object                            */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        03.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function searches the base object described by   */
/*              dninfo, checks the filters and writes the result      */
/*              message consisting of the required attributes to be   */
/*              read.                                                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      exname      D2_name_string  exact DN of base object           */
/*      dname       D2_name_string  normed DN of base object          */
/*      dninfo      D26_dninfo *    information about object found    */
/*      ob_nr       Short           object number                     */
/*      ob_cl       Long *          object classes of base object     */
/*      r_info      D23_rq_info *   information about required        */
/*                                  attributes                        */
/*      f_info      D23_ft1_info *  information about filter          */
/*                                  attributes                        */
/*      output_len  Int *           maximum length of result message  */
/*      remlen      Int *           remaining length for result data  */
/*      fct_tag     Short           indicates calling function        */
/*      output      String *        pointer to beginning of message   */
/*      acl         D26_acl *       acl of base object                */
/*      mk          Long            index of master DSA in DN-list    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output_len  Int *           length of result message          */
/*      remlen      Int *           remaining length for result data  */
/*      output      String *        pointer to beginning of message   */
/*      attr        D26_attr_info * array of indices (and a flag for  */
/*                                  read permission for each requested*/
/*                                  attribute in the AT (the last one */
/*                                  will be "-1")                     */
/*      recur       Bool *          indicates, whether recurring      */
/*                                  attributes are supplied           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   Short           no error occurred                 */
/*      D2_ATT_ERROR                No requested attribute is found   */
/*      D2_UNDEFINED_ATTR_TYPE      Filter undefined for object       */
/*      D2_ATTRIBUTE_MISSING        Filter attribute not present      */
/*                                  (Only for Compare)                */
/*      D2_OBJ_CLS_VIOLATION        Filter attribute does not belong  */
/*                                  to object class (Only for Modify  */
/*                                  Entry)                            */
/*      D2_ACCESS_RIGHTS_INSUFFICIENT access is not given on all      */
/*                                  filter attributes                 */
/*      D2_ADM_LIM_EXCEEDED         Too much requested attributes     */
/*      D26_NOFILTERMATCH           filters don't match object        */
/*      D26_ERROR                   System error occurred             */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_u32_search_base_object(
    D2_name_string dname,
    D2_name_string exname,
    D26_dninfo     *dninfo,
    signed16       ob_nr,
    signed32       *ob_cl,
    D23_rq_info    *r_info,
    D23_ft1_info   *f_info,
    signed32       *output_len,
    signed32       *remlen,
    signed16       fct_tag,
    byte          **output,
    D26_attr_info  *attr,
    D26_acl        *acl,
    signed32       mk,
    boolean        *recur)

{                               /*  d26_u32_search_base_object        */

				    /* return value                   */
  signed16         ret_value = D2_NO_ERR;
				    /* indicates update error for     */
				    /* modify                         */
  signed16         upd_err   = D2_NO_ERR;

  byte             *message;        /* pointer for message to write   */
  signed32         meslen;          /* length of written message      */


  unsigned16       tag;             /* indicates kind of DN           */

  D23_ft1_info     *f1_info;        /* pointer to Filter structure    */
				    /* array for filter attribute     */
				    /* information                    */
  D26_attr_info    ft_info[D2_AT_MAX + 1];
  D26_f_err        f_err;           /* information about erroneous    */
				    /* filter item                    */

  byte   exact_dn[D2_DNL_MAX];      /* memory for exact name          */
  signed16 dnlen;                   /* length of exact name           */

f_err.d26_error  = D2_NO_ERR;
f_err.d26_filter = NULL;
if                                  /*  no filter pattern given       */
  (f_info->d23_fp1tag == D23_PATTAG)
{                                   /*  analyse filter for object nr  */
  f1_info = f_info;
  f_err.d26_error = d26_f09_analyse_filter_object(fct_tag == D23_MODENTR ?
			D26_ROOT : ob_nr,ob_cl,&f1_info,&f_err.d26_filter);
}

if                                  /*  not all filter attributes     */
				    /*  belong to object class        */
				    /*  while update function calls   */
  (f_err.d26_error != D2_NO_ERR && fct_tag == D23_MODENTR)
{                                   /*  return error                  */
  d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,D2_PAR_ERR,
			 D2_OBJ_CLS_VIOLATION,&meslen);
  *remlen -= meslen;
  return(D2_OBJ_CLS_VIOLATION);
}
else
{                       /*  calculate filter attribute information    */
  d26_f20_calc_fat_info(ob_nr,ob_cl,f_info,ft_info);
  ret_value = d26_f19_filter_attr_check(d26_ob_record,dninfo->d26_filenr,
		  ob_nr,dname,ft_info,acl,f_info,&f_err.d26_filter,fct_tag,
		  FALSE);
  if                    /*  access rights for filter attributes insuf.*/
    (ret_value == D2_ACCESS_RIGHTS_INSUFFICIENT && fct_tag != D23_SEARCH &&
     fct_tag != D2_ADM_OPID + D23_SEARCH)
  {                                 /*  return error                  */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,D2_PAR_ERR,
	    D2_ACCESS_RIGHTS_INSUFFICIENT,&meslen);
    *remlen -= meslen;
    return(D2_ACCESS_RIGHTS_INSUFFICIENT);
  }
  else if               /*  just no Filter match for non-retrieval    */
			/*  function                                  */
	 ((ret_value == D2_UNDEFINED_ATTR_TYPE ||
	   ret_value == D2_ATTRIBUTE_MISSING ||
	   ret_value == D2_AT_VALUE_EXISTS ||
	   ret_value == D26_NOFILTERMATCH) &&
	  (fct_tag == D23_MODENTR || fct_tag == D23_BIND))
  {                     /*  reset error indicator                     */
    f_err.d26_error = ret_value;
    d26_a06_write_filter_error(exname,&f_err,output,output_len,remlen);
    return(D26_NOFILTERMATCH);
  }
  else if               /*  attribute missing in compare function     */
	 ((ret_value == D2_ATTRIBUTE_MISSING ||
	   ret_value == D2_UNDEFINED_ATTR_TYPE) && fct_tag == D23_COMPARE)
  {                     /*  reset error indicator                     */
    f_err.d26_error = D2_ATTRIBUTE_MISSING;
    d26_a06_write_filter_error(exname,&f_err,output,output_len,remlen);
    return(D2_ATTRIBUTE_MISSING);
  }
  else if               /*  too many requested attributes             */
	 (r_info->d23_rqnbr > D2_AT_MAX)
  {                     /*  write service error message               */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,D2_PAR_ERR,
      D2_ADM_LIM_EXCEEDED,&meslen);
    *remlen -= meslen;
    return(D2_ADM_LIM_EXCEEDED);
  }                     /*                                            */

  upd_err = d26_u71_make_attr_list(r_info,attr,dname,acl,ob_nr,ob_cl,
				   fct_tag);
  fct_tag = fct_tag > D2_ADM_OPID ? fct_tag - D2_ADM_OPID : fct_tag;
  if                    /*  modify entry is getting error             */
    (fct_tag == D23_MODENTR && upd_err == D2_OBJ_CLS_VIOLATION)
  {                     /*  return error                              */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,D2_PAR_ERR,
			   D2_OBJ_CLS_VIOLATION,&meslen);
    *remlen -= meslen;
    return(D2_OBJ_CLS_VIOLATION);
  }
  else if               /*  read is calling                           */
	 (fct_tag == D23_READ && r_info->d23_rqnbr != 0)
  {                     /*  check the requested attributes            */
    switch(d26_i39_check_req_attr(d26_ob_record,dninfo->d26_filenr,attr))
    {
      case D2_NO_ERR: break;
      case D2_ATTRIBUTE_MISSING:
	if              /*  list of attributes was requested          */
	  (r_info->d23_rqnbr > 0)
	{               /*  write attribute error message             */
	  d26_u43_no_req_attr_error(output,exname,r_info,output_len,remlen);
	  return(D2_ATT_ERROR);
	}               /*                                            */
	break;
      case D2_ACCESS_RIGHTS_INSUFFICIENT:
	d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,
	  D2_PAR_ERR,D2_ACCESS_RIGHTS_INSUFFICIENT,&meslen);
	*remlen -= meslen;
	return(D2_ACCESS_RIGHTS_INSUFFICIENT);
    }   
  }                     /*                                            */

  message = *output;
  d26_entry_count = 1;

  if                    /*  DSP was coming in                         */
    (d26_chain_arg != (D23_chain_arg *)NULL)
  {                     /*  write chaining result                     */
    if                  /*  error from chaining results               */
      ((message = d26_u54_write_chaining_results(message,output,remlen,
       output_len)) == NULL)
    {                   /*  return error                              */
      return(D26_ERROR);
    }
  }

  if                    /*  function was called by compare            */
    (fct_tag == D23_COMPARE)
  {                     /*  write match flag                          */
    if                  /*  error from writing match flag             */
      ((message = d26_u45_write_match_flag(message,output,remlen,output_len,
       ret_value,mk)) == NULL)
    {                   /*  return error                              */
      return(D26_ERROR);
    }
  }

  tag = fct_tag == D23_SEARCH ? D23_BONTAG : D23_NAMTAG;
  if                    /*  base object is alias and search has called*/
			/*  internal search                           */
    (d26_al_rdns > 0 && (d26_serv_contr->d23_scopt & D2_DREFALIAS) == 0L
     && (fct_tag == D23_SEARCH || fct_tag == D23_COMPARE))
  {                     /*  write distinguished name of base object   */
    dnlen = d26_i48_get_exact_name(d26_ob_record,dninfo,0,exact_dn);
    if                  /*  error from writing distinguished name     */
      ((message = d26_u49_write_dist_name(message,output,exact_dn,dnlen,
       remlen,output_len,tag)) == NULL)
    {                   /*  return error                              */
      return(D26_ERROR);
    }
  }

  if                    /*  filter did match and compare did not call */
    (ret_value == D2_NO_ERR && fct_tag != D23_COMPARE)
  {                     /*  check limits                              */
    if                  /*  limits not reached                        */
      ((d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	d26_entry_count <= d26_serv_contr->d23_scslm) &&
       d26_time_ex == FALSE)
    {                   /*  write the entry information               */
      if                /*  error from write entry                    */
	((message = d26_u37_write_entry(d26_ob_record,message,output,dname,
	 dninfo,r_info,recur,attr,mk,ob_nr,remlen,output_len)) == NULL)
      {                 /*  return error                              */
	return(D26_ERROR);
      }
    }
    else if             /*  error from writing partial outcome qualif.*/
	 ((message = d26_u30_write_part_out_qual(message,output,remlen,
	  output_len)) == NULL)
    {                   /*  return error                              */
      return(D26_ERROR);
    }
  }

  if                    /*  error from write EOM                      */
    ((message = d26_u39_write_eom(message,remlen,output,output_len,fct_tag))
     == NULL)
  {                     /*  return error                              */
    return(D26_ERROR);
  }
  else
  {                     /*  return no error                           */
    return(ret_value);
  }
}

}                       /*  d26_u32_search_base_object                */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u33_handle_name_res_result()                    */
/*                                                                    */
/* AUTHOR       : Kraemer,D AP 11                                     */
/* DATE         : 14.02.91                                            */
/*                                                                    */
/* DESCRIPTION  : This function handles the return value of local name*/
/*                resolution. It creates the various error messages.  */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR       signed16    no error occured                  */
/*      D2_NFOUND:                  no base object found              */
/*      D2_NMSYNTAX_ERROR           syntax error in DN of base object */
/*      D2_ALIAS_ERROR              orphan alias detected             */
/*      D2_ON_ALIAS_NOT_ALLOWED     service controls prohibit alias   */
/*                                  dereferencing                     */
/*      D2_REF_ERROR                referral is returned              */
/*      D2_OUT_OF_SCOPE             no referral within specified scope*/
/*      D26_ERROR                   system error occurred             */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_u33_handle_name_res_result(
			/* IN    -  return value from name resolution */
    signed16       ret_value,
			/* IN    -  normed dereferenced object name   */
    D2_name_string dname,
			/* IN    -  information about object found    */
    D26_dninfo     *dninfo,
    signed32       mk,  /* IN    -  index of master DSA in DN-list    */
			/* IN    -  exact dereferenced object name    */
    D2_name_string ename,
			/* IN    -  unresolved part of base object    */
    D2_name_string unresolved,
			/* IN    -  exact unresolved part of base     */
    D2_name_string eunres,
			/* OUT   -  pointer tor error data            */
    byte           **output,
			/* INOUT -  maximum length of error data      */
    signed32       *output_len,
			/* INOUT -  remaining length of message       */
    signed32       *remlen)

{                                   /*  d26_u33_handle_name_res_result*/

  signed32 meslen;              /* length of written message          */

  if                            /* Master knowledge is still not known*/
    (mk == D26_IDX_NOT_DEF)
  {                             /* return error                       */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
      D2_UNWILLING,&meslen);
    *remlen -= meslen;
    return(D26_ERROR);
  }
  else if                       /* alias cannot be dereferenced       */
	 (ret_value == D2_NFOUND && dninfo->d26_rd_anz < d26_al_rdns &&
	  mk == D26_OWNNAME)
  {                             /* reset return value                 */
    ret_value = D2_ALIAS_ERROR;
  }

  switch(d26_serv_contr->d23_scopt & (D2_LOCALSCOPE | D2_NOCACHE))
  {
    case D2_LOCALSCOPE | D2_NOCACHE:
      ret_value = ret_value == D2_NO_ERR && mk != D26_OWNNAME &&
	d26_subset == D2_BASE_OBJECT ? D2_NFOUND : ret_value;
				/* fall into next case                */
    case D2_LOCALSCOPE:
      if                        /* object not found                   */
	(ret_value != D2_NO_ERR)
      {                         /* return a name error                */
	if                      /* error from writing name error      */
	  ((d26_u04_set_name_error_apdu(output,output_len,remlen,ret_value,
	   mk,dninfo)) == D26_ERROR)
	{                       /* return error                       */
	  return(D26_ERROR);
	}
	return(ret_value);
      }
      break;
    case D2_NOCACHE:
    case 0:
      if                        /* object not found                   */
	(ret_value != D2_NO_ERR)
      {                         /* check operation progress           */
	if                      /* DSA is master of break object      */
	  (mk == D26_OWNNAME)
	{                       /* return a name error                */
	  if                    /* error from writing name error      */
	    ((d26_u04_set_name_error_apdu(output,output_len,remlen,ret_value,
	     mk,dninfo)) == D26_ERROR)
	  {                     /* return error                       */
	    return(D26_ERROR);
	  }
	  return(ret_value);
	}
	else
	{                       /* create error referral              */
	  if                    /* error referral created             */
	    ((ret_value = d26_u70_make_error_referrals(dninfo,dname,
	     d26_al_rdns,ename,unresolved,eunres,d26_bo_encoded_part,mk,
	     FALSE)) == D2_NO_ERR)
	  {                     /* just return referral error         */
	    return(D2_REF_ERROR);
	  }
	  else
	  {                     /* free referrals                     */
	    d26_u41_free_referrals();
	    switch(ret_value)
	    {
	      case D26_ERROR:   /* hard error occurred                */
		d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,
		  D2_REM_ERR,D2_UNWILLING,&meslen);
		*remlen -= meslen;
		return(D26_ERROR);
	      case D2_NFOUND:   /* master DSA not found               */
		if              /* error from writing name error      */
		  ((d26_u04_set_name_error_apdu(output,output_len,remlen,
		   D2_NFOUND,D26_OWNNAME,dninfo)) == D26_ERROR)
		{               /* return hard error                  */
		  return(D26_ERROR);
		}
		else
		{               /* return NOT FOUND                   */
		  return(D2_NFOUND);
		}
	      case D2_OUT_OF_SCOPE: /* referral is out of scope       */
		d27_010_set_error_apdu((D23_rserror *)*output,D2_PERM_ERR,
		  D2_REM_ERR,D2_OUT_OF_SCOPE,&meslen);
		*remlen -= meslen;
		return(D2_OUT_OF_SCOPE);
	    }
	  }
	}
      }
      else if                   /* only copy is found for single      */
				/* object operations                  */
	   (mk != D26_OWNNAME &&
	    (d26_serv_contr->d23_scopt & D2_NOCACHE != 0 &&
	     d26_subset == D2_BASE_OBJECT ||
	     d26_op_progress->d23_opnm_rs != D2_COMPLETED &&
	     d26_subset != D2_BASE_OBJECT))
      {                         /* write error referral               */
	if                      /* error referral created             */
	  ((ret_value = d26_u70_make_error_referrals(dninfo,dname,
	   d26_al_rdns,ename,unresolved,eunres,d26_bo_encoded_part,mk,FALSE))
	   == D2_NO_ERR)
	{                       /* just return referral error         */
	  return(D2_REF_ERROR);
	}
	else
	{                       /* free referrals                     */
	  d26_u41_free_referrals();
	  switch(ret_value)
	  {
	    case D26_ERROR:     /* hard error occurred                */
	      d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,
		D2_REM_ERR,D2_UNWILLING,&meslen);
	      *remlen -= meslen;
	      return(D26_ERROR);
	    case D2_NFOUND:     /* master DSA not found               */
	      if                /* error from writing name error      */
		((d26_u04_set_name_error_apdu(output,output_len,remlen,
		 D2_NFOUND,D26_OWNNAME,dninfo)) == D26_ERROR)
	      {                 /* return hard error                  */
		return(D26_ERROR);
	      }
	      else
	      {                 /* return NOT FOUND                   */
		return(D2_NFOUND);
	      }
	    case D2_OUT_OF_SCOPE:   /* referral is out of scope       */
	      d27_010_set_error_apdu((D23_rserror *)*output,D2_PERM_ERR,
		D2_REM_ERR,D2_OUT_OF_SCOPE,&meslen);
	      *remlen -= meslen;
	      return(D2_OUT_OF_SCOPE);
	  }
	}
      }
      break;
  }
				/* return no error                    */
  return(D2_NO_ERR);

}                                   /* d26_u33_handle_name_res_result */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u46_descendant                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        18.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function decides, whether desc is a descendant   */
/*              of ob_nr.                                             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              ob_nr       Short           object number of purposed */
/*                                          ancestor                  */
/*              desc        Short           object number of purposed */
/*                                          descendant                */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              D2_TRUE                     desc is descendant of     */
/*                                          ob_nr.                    */
/*              D2_FALSE                    desc is not descendant of */
/*                                          ob_nr.                    */
/*                                                                    */
/*exoff ***************************************************************/

boolean d26_u46_descendant(
    signed16 ob_nr,
    signed16 desc)

{                               /*  d26_u46_descendant                */

  register signed16 par_nr;     /* parent object number               */
  register D26_srt  *srt;       /* pointer to SRT                     */

for                         /*  all ancestors of descendant           */
   (srt = d26_srt + desc, par_nr = srt->d26s_par_nr; ;
    srt = d26_srt + par_nr, par_nr = srt->d26s_par_nr)
{                           /*  check parenthood                      */
  if                        /*  parent number is object number        */
    (par_nr == ob_nr)
  {                         /*  return match                          */
    return(TRUE);
  }
  else if                   /*  root is reached                       */
	 (par_nr == D26_ROOT)
  {                         /*  return no match                       */
    return(FALSE);
  }
}

}                       /*  d26_u46_descendant                        */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u47_search_set                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        16.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the DN of the base object into   */
/*              message and searches for all its children and         */
/*              eventually for referrals. At last it writes the       */
/*              message.                                              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      subset      D2_subset       indicates subset to be searched   */
/*      dname       D26_name_string normed DN of base object          */
/*      ename       D26_name_string exact DN of base object           */
/*      dninfo      D26_dninfo *    information about object found    */
/*      bo_nr       Short           base object number                */
/*      ob_cl       Long *          object class array                */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      f_info      D23_ft1_info *  information about filter attr.    */
/*      output_len  Int *           maximum length of result message  */
/*      remlen      Int *           remaining length for result data  */
/*      output      String *        pointer to beginning of message   */
/*      fct_tag     Short           indicates the calling function    */
/*      acl         D26_acl *       acl of base object                */
/*      mk          Long            index of master knowledge in      */
/*                                  DN-list.                          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output_len  Int *           length of result message          */
/*      remlen      Int *           remaining length for result data  */
/*      output      String *        pointer to beginning of message   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   Short           no error occurred                 */
/*      D2_ABANDONED                request was abandoned             */
/*      D26_ERROR                   error occurred                    */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_u47_search_set(
    D2_subset      subset,
    D2_name_string dname,
    D2_name_string ename,
    D26_dninfo     *dninfo,
    signed16       bo_nr,
    signed32       *ob_cl,
    D23_rq_info    *r_info,
    D23_ft1_info   *f_info,
    signed32       *output_len,
    signed32       *remlen,
    signed16       fct_tag,
    byte          **output,
    D26_acl        *acl,
    signed32       mk)

{                               /*  d26_u47_search_set                */

  void       *target_root;      /* pointer to root of targets         */
  D26_target *act_target;       /* actual target object               */
  D26_target cmp_target;        /* target object to compare           */

  byte       *message;          /* pointer for message to write       */

  register signed32 i;          /* loop variable                      */

message = *output;

if                      /*  DSP was coming in and list or search have */
			/*  called                                    */
  (d26_chain_arg != (D23_chain_arg *)NULL &&
   (fct_tag == D23_LIST || fct_tag == D2_ADM_OPID + D23_LIST ||
    fct_tag == D23_SEARCH || fct_tag == D2_ADM_OPID + D23_SEARCH))
{                       /*  write chaining result                     */
  if                    /*  error from chaining results               */
    ((message = d26_u54_write_chaining_results(message,output,remlen,
     output_len)) == NULL)
  {                     /*  return error                              */
    return(D26_ERROR);
  }
}

if                      /*  base object is alias and search or list   */
			/*  have called internal search               */
  (d26_al_rdns > 0)
{                       /*  write distinguished name of base object   */
  if                    /*  error from writing distinguished name     */
    ((message = d26_u49_write_dist_name(message,output,ename,
     strlen((char *)ename) + 1,remlen,output_len,D23_BONTAG)) == NULL)
  {                     /* return error                               */
    return(D26_ERROR);
  }
}

target_root = (void *)NULL;
act_target  = (D26_target *)NULL;
if                      /*  base object cannot be put into target     */
			/*  object array                              */
  ((act_target = d26_u69_fill_target(dname,ename,bo_nr,subset,&target_root,
   act_target)) == NULL)
{                       /*  return error                              */
  return(D26_ERROR);
}
else
{                       /*  enhance actual target object information  */
  act_target->d26_tbase = TRUE;
  act_target->d26_tunresolved = act_target->d26_tdname +
				strlen((char *)act_target->d26_tdname );
  act_target->d26_tal_rdns    = d26_al_rdns;
  for                   /*  still object class following              */
     (i = 0; ob_cl[i] != D26_EOL; i++)
  {                     /*  copy object class                         */
    act_target->d26_tob_cl[i] = ob_cl[i];
  }                     /*                                            */
  act_target->d26_tob_cl[i] = ob_cl[i];
  for                   /*  still ACL index following                 */
     (i = 0; i < D2_ACLIDX_MAX; i++)
  {                     /*  copy ACL index                            */
    act_target->d26_tacl[i] = acl[i];
  }                     /*                                            */
  act_target->d26_tmk         = mk;
  act_target->d26_tmessage    = &message;
  act_target->d26_toutput     = output;
  act_target->d26_toutput_len = output_len;
  act_target->d26_tremlen     = remlen;
  act_target->d26_tr_info     = r_info;
  act_target->d26_tf_info     = f_info;
  act_target->d26_top_id      = fct_tag;
  act_target->d26_tfilenr     = dninfo->d26_filenr;
  if                    /*  memory for record can't be allocated      */
    ((act_target->d26_trecord = (char *)D27_MALLOC(svc_c_sev_warning,
     (unsigned)d26_oreclen)) == NULL)
  {                     /* free targets and return error              */
    d26_u75_free_targets(&target_root);
    return(D26_ERROR);
  }
  else
  {                     /*  copy read record                          */
    memcpy(act_target->d26_trecord,d26_ob_record,d26_oreclen);
    cmp_target.d26_tbo_nr  = act_target->d26_tbo_nr;
    cmp_target.d26_tsubset = act_target->d26_tsubset;
    cmp_target.d26_tdname  = dname;
    act_target = (D26_target *)NULL;
    if                  /*  aliases must be searched                  */
      (d26_search_arg->d23_Iali == TRUE)
    {                   /*  search aliases                            */
      if                /*  error from searching aliases              */
	(d26_i50_search_aliases(&cmp_target,subset,dname,dninfo,bo_nr,
	 &target_root,&act_target,&message,output,output_len,remlen,r_info,
	 f_info,fct_tag) == D26_ERROR)
      {                 /*  return error                              */
	d26_u75_free_targets(&target_root);
	return(D26_ERROR);
      }
    }
    twalk(target_root,d26_u76_target_action);
    d26_u75_free_targets(&target_root);
    if                  /*  allocation error occurred                 */
      (message == NULL)
    {                   /*  return error                              */
      return(D26_ERROR);
    }
  }
}

if                      /* request is not abandoned                   */
  (d26_abandoned == FALSE)
{                       /* write partial outcome qualifier            */
  fct_tag = fct_tag > D2_ADM_OPID ? fct_tag - D2_ADM_OPID : fct_tag;
  if                    /* error from writing partial outcome qualif. */
    ((message = d26_u30_write_part_out_qual(message,output,remlen,
     output_len)) == NULL)
  {                     /*  return error                              */
    return(D26_ERROR);
  }
}

if                      /*  error from write EOM                      */
  ((message = d26_u39_write_eom(message,remlen,output,output_len,fct_tag))
   == NULL)
{                       /*  return error                              */
  return(D26_ERROR);
}

return(d26_abandoned == TRUE ? D2_ABANDONED : D2_NO_ERR);

}                       /*  d26_u47_search_set                        */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u57_find_master_part                              */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function finds the longest superior node of an   */
/*              object, whose master is the current DSA. During this  */
/*              loop it reads the shadowed by attribute, if required. */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR       signed16        everything ok                 */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_u57_find_master_part(
    D26_dninfo *dninfo, /* INOUT -  information about object found    */
    signed32   mk,      /* IN    -  DN-List index of its master DSA   */
    signed16   *change) /* OUT   -  RDN-index where master knowledge  */
			/*          changes                           */

{                          /* d26_u57_find_master_part                */

				    /* object class of superior node  */
  signed32    ocl[D26_OCL_VAL_COUNT + 1];
				    /* indices into DN list for ACL   */
  D26_acl     acl[D2_ACLIDX_MAX];
  D26_rdninfo *l_rdn;               /* last RDN                       */
  signed32    mk_change;            /* changing master knowledge      */

  *change   = dninfo->d26_rd_anz;
  mk_change = mk;
  while                 /* master part not found                      */
       (dninfo->d26_rd_anz > 0 && mk_change != D26_OWNNAME)
  {                     /* reduce object length                       */
    dninfo->d26_rd_anz--;
    if                  /* still object not empty                     */
      (dninfo->d26_rd_anz > 0)
    {                   /* find the new part                          */
      if                /* object exists                              */
	(d26_i31_find_object(dninfo,&l_rdn,d26_ob_record) == D2_NO_ERR)
      {                             /*  read master knowledge          */
	d26_i36_read_ocl_acl_mk(dninfo,d26_ob_record,ocl,acl,&mk_change);
	if              /* master knowledge still not changed         */
	  (mk_change == mk)
	{               /* reset change index                         */
	  *change = dninfo->d26_rd_anz;
	}
      }
      else
      {                 /* return error                               */
	dce_svc_printf(GDS_S_DB_INCONSISTENT_MSG);
	return(D26_ERROR);
      }
    }
  }

			/* return no error                            */
  return(D2_NO_ERR);

}                       /* d26_u57_find_master_part                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u67_trace_name_struct                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.03.92                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the object identifiers that      */
/*              belong to the structure rule and all their superior   */
/*              structure rules into the logfile.                     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      srt         D26_srt *       structure rule to be logged       */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u67_trace_name_struct(
    D26_srt *srt)

{                               /*  d26_u67_trace_name_struct         */

  char log_comment[1000];       /* comment for logging                */

  register D26_nam_at  *nam_at; /* pointer to naming attributes       */
  register char        *logp;   /* pointer to logging string          */

for                                 /*  all rules of name structure   */
   (logp = log_comment; srt >= d26_srt; srt = d26_srt + srt->d26s_par_nr)
{                                   /*  scan the naming attributes    */
  for                               /*  all naming attributes         */
     (nam_at = srt->d26s_nam_at; nam_at->d26s_index != -1; logp++, nam_at++)
  {                                 /*  write naming attribute into   */
				    /*  logging string                */
    d27_301_oid_str(&d26_at[nam_at->d26s_index].d26a_type,logp);
    logp += strlen(logp);
    *logp = ',';
  }
  *(logp - 1) = '/';
}
*(logp - 1) = D2_EOS;

DCE_SVC_LOG((GDS_S_NAME_STRUCTURE_MSG,log_comment));

}                       /*  d26_u67_trace_name_struct                 */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u00_search                                      */
/*                                                                    */
/* AUTHOR       : Kraemer,D AP 11                                     */
/* DATE         : 22.01.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will perform the search as specified  */
/*                in input. Furthermore it will return information    */
/*                that can be used by callers within the same DSA.    */
/*                (See OUTPUT PARAMETERS).                            */
/*                                                                    */
/* RETURN VALUE :   signed16                                          */
/*      D2_NO_ERR:                      no error occured              */
/*      D2_NFOUND:                      no base object found          */
/*      D2_NMSYNTAX_ERROR      :        syntax error in DN of base    */
/*                                      object                        */
/*      D2_ALIAS_ERROR         :        orphan alias detected         */
/*      D2_ON_ALIAS_NOT_ALLOWED:        service controls prohibit     */
/*                                      alias dereeferencing          */
/*      D2_ATT_ERROR                    No requested attribute is     */
/*                                      found (only for READ)         */
/*      D2_UNDEFINED_ATTR_TYPE          Filter undefined for object   */
/*      D2_ATTRIBUTE_MISSING            Filter attribute not present  */
/*                                      (Only for Compare)            */
/*      D2_OBJ_CLS_VIOLATION            Filter attribute does not     */
/*                                      belong  to object class (Only */
/*                                      for Modify Entry)             */
/*      D2_ACCESS_RIGHTS_INSUFFICIENT   access is not given on all    */
/*                                      filter attributes (Not for    */
/*                                      SEARCH)                       */
/*      D26_NOFILTERMATCH               object was found, but filter  */
/*                                      doesn't match (Not for SEARCH)*/
/*      D2_REF_ERROR                    referral is returned          */
/*      D2_DIT_INCONSISTENT             inconsistent database         */
/*      D2_UNABLE_TO_PROCEED            DSA is unable to proceed in   */
/*                                      name resolution               */
/*      D2_INVALID_REFERENCE            invalid reference in chaining */
/*                                      arguments                     */
/*      D2_TIME_LIMIT_EXCEEDED          time limit exceeded           */
/*      D2_ADM_LIM_EXCEEDED             too much filter items or      */
/*                                      requested attributes          */
/*      D2_LOOP_DETECTED                Loop detected                 */
/*      D2_UNAV_CRIT_EXTENSION          Unavailable critical extension*/
/*                                      required                      */
/*      D2_OUT_OF_SCOPE                 no referral within specified  */
/*                                      scope                         */
/*      D26_ERROR                       system error occurred         */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u00_search(
			/* IN    -  indicates whether record should be*/
    boolean        lock_mode,       /* locked                         */
			/* IN    -  indicates the calling function    */
    signed16       fct_tag,
			/* IN    -  internal search message           */
    byte           *input,
			/* INOUT -  length of memory for result data  */
    signed32       *output_len,
			/* OUT   -  result data or error data         */
    byte           **output,
			/* OUT   -  remaining length for result data  */
    signed32       *remlen,
			/* OUT   -  exact DN of the found object      */
    D2_name_string exact_obj,
			/* OUT   -  array of indices and flags for    */
			/*          read permission for the requested */
    D26_attr_info  *attr,           /* attributes                     */
			/* OUT   -  object classes of the object      */
    signed32       *ob_cl,
			/* OUT   -  array of DN-list indices          */
    D26_acl        *acl,/*          representing the ACL              */
    signed32       *mk, /*          DN-list index of master knowledge */
			/* OUT   -  indicates whether recurring       */
    boolean        *recurring,      /* attributes are supplied        */
			/* OUT   -  C-ISAM-record containing object   */
    char           **record,
			/* OUT   -  indicates whether base object is  */
    boolean        *al_flag,        /* alias                          */
			/* OUT   -  object number of base object      */
    signed16       *ob_nr)                            /**/

{                                   /*  d26_u00_search                */

  signed32       meslen;           /* length of written message      */
				   /* information for DN of base     */
  D26_dninfo     dn_info;          /* object                         */
				   /* normed target object           */
  byte           norm_base_obj[D2_DNL_MAX];
				   /* exact target object            */
  byte           e_target_obj[D2_DNL_MAX];
				   /* normed dereferenced object     */
  byte           deref_obj[D2_DNL_MAX + 1];
  D2_name_string unresolved;       /* unresolved part of base object */
  D2_name_string eunres;           /* exact unresolved part of b. o. */
  D23_rq_info    *r_info;          /* requested informations         */
  D23_ft1_info   *f_info;          /* filter attributes              */

				   /* return value                   */
  signed16       ret_value = D2_NO_ERR;

				    /*  initialize remaining memory   */
				    /*  length and record area        */
  *remlen = *output_len;
  *record = d26_ob_record;
  if                                /*  modify operation has called   */
    (fct_tag == D23_BIND || fct_tag == D23_ADDOBJT ||
     fct_tag == D23_REMOBJT || fct_tag == D23_MODENTR ||
     fct_tag == D23_REPLRDN)
  {                                 /*  reinitialize the base object  */
    d26_base_object = (D2_name_string)NULL;
  }
				    /*  analyse Search Invoke Message */
  if                                /*  message is not correct        */
    ((ret_value = d26_a01_analyse_search_message(input,output,&meslen,
     norm_base_obj,&r_info,&f_info,fct_tag)) != D2_NO_ERR)
  {                                 /*  return error                  */
    *remlen -= meslen;
    return(ret_value);
  }
  else
  {                                 /*  initialize alias flag         */
    *al_flag = FALSE;
    strcpy((char *)deref_obj,"");
    strcpy((char *)exact_obj,"");
    unresolved = norm_base_obj;
    eunres     = e_target_obj;
    d27_012_remove_blanks((char *)d26_base_object,
      strlen((char *)d26_base_object) + 1,(char *)e_target_obj);
    if                              /*  base object is root           */
      (!strcmp((char *)d26_base_object,"") &&
       d26_bo_encoded_part == (D23_av_value *)NULL)
    {                               /*  set number of nameparts       */
      dn_info.d26_rd_anz = 0;
      if                            /*  single object is searched     */
	(d26_subset == D2_BASE_OBJECT)
      {                             /*  write error codes             */
	if                          /*  no error from writing name err*/
	  ((d26_u04_set_name_error_apdu(output,output_len,remlen,D2_NFOUND,
	   D26_IDX_NOT_DEF,&dn_info)) == D26_ERROR)
	{                           /*  return error                  */
	  return(D26_ERROR);
	}
	return(D2_NFOUND);
      }
      else
      {                             /*  set DN of root                */
	*ob_nr = D26_ROOT;
	*ob_cl = (signed32)D26_EOL;
	*mk    = d26_u64_first_level_mk();
	D27_022_ENTER_TIME_TRACE(GDS_S_FIRST_LEVEL_MK_READ,NULL,0,0);
      }
    }
    else
    {                               /*  resolve name                  */
      if                            /*  base object found             */
	((ret_value = d26_u07_resolve_name(lock_mode,fct_tag,&dn_info,
	 al_flag,ob_nr,norm_base_obj,e_target_obj,deref_obj,&unresolved,
	 &eunres)) == D2_NO_ERR)
      {                             /*  check lock mode               */
	if                          /*  lock mode is set              */
	  (lock_mode == TRUE)
	{                           /*  lock the base object          */
	  ret_value = d26_i35_reread_object(dn_info.d26_filenr,lock_mode);
	}
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug1,
	  GDS_S_ENTRY_FOUND,deref_obj));
      }
      else
      {                             /*  check lock mode               */
	if                          /*  lock mode is set              */
	  (lock_mode == TRUE)
	{                           /*  unlock all tables             */
	  d26_i14_unlock();
	}
	DCE_SVC_LOG((GDS_S_BO_NOT_FOUND_MSG));
				    /*  set error apdu for service    */
				    /*  errors                        */
	switch(ret_value)
	{
	  case D2_INVALID_REFERENCE:
	  case D2_UNABLE_TO_PROCEED:
	    d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,
				   D2_PAR_ERR,ret_value,&meslen);
	    *remlen -= meslen;
	    return(ret_value);
	  case D2_DIT_INCONSISTENT:
	    d27_010_set_error_apdu((D23_rserror *)*output,D2_PERM_ERR,
				   D2_ILL_STAT,D2_DIT_INCONSISTENT,&meslen);
	    *remlen -= meslen;
	    return(D26_ERROR);
	  case D26_ERROR:
	    d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,
				   D2_REM_ERR,D2_UNWILLING,&meslen);
	    *remlen -= meslen;
	    return(D26_ERROR);
	  default:  /*  continue for other errors                     */
	    break;
	}
      }

      if                            /*  break object found            */
	(dn_info.d26_rd_anz > 0)
      {                             /*  read master knowledge only    */
	d26_i36_read_ocl_acl_mk(&dn_info,d26_ob_record,ob_cl,acl,mk);
	d26_i48_get_exact_name(d26_ob_record,&dn_info,0,exact_obj);
      }
      else
      {                             /*  take first level master       */
				    /*  knowledge                     */
	*mk = d26_u64_first_level_mk();
      }
    }

    if                              /*  referrals are required        */
      (!(d26_serv_contr->d23_scopt & D2_LOCALSCOPE))
    {                               /*  reset count and root of       */
				    /*  referral tree                 */
      d26_ref_present = FALSE;
      d26_ref_count   = 0;
      d26_ref_root    = (void *)NULL;
    }

    if                              /*  error from handling the name  */
				    /*  resolution result             */
      ((ret_value = d26_u33_handle_name_res_result(ret_value,deref_obj,
       &dn_info,*mk,exact_obj,unresolved,eunres,output,output_len,remlen))
       != D2_NO_ERR)
    {                               /*  return error                  */
      DCE_SVC_LOG((GDS_S_LOC_NRS_BROKEN_MSG,ret_value));
      return(ret_value);
    }
    else if                         /*  filters are not correct       */
	   ((ret_value = d26_a05_analyse_filter(exact_obj,f_info,output,
	    d26_subset,output_len,remlen,fct_tag)) != D2_NO_ERR)
    {                               /*  return with error             */
      return(ret_value);
    }
    else if                         /*  retrieval function has called */
	   (fct_tag == D23_READ || fct_tag == D2_ADM_OPID + D23_READ ||
	    fct_tag == D23_SEARCH || fct_tag == D2_ADM_OPID + D23_SEARCH ||
	    fct_tag == D23_LIST || fct_tag == D23_COMPARE)
    {                               /*  check time                    */
      if                            /*  time has run out              */
	(d26_time_ex == TRUE)
      {                             /*  reset return value            */
	d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
		    D2_TIME_LIMIT_EXCEEDED,&meslen);
	*remlen -= meslen;
	return(D2_TIME_LIMIT_EXCEEDED);
      }
    }
    d26_start_p_o_q = NULL;
    d26_entry_count = 0;
    switch(d26_subset)
    {
      case D2_BASE_OBJECT  :
	ret_value = d26_u32_search_base_object(deref_obj,exact_obj,&dn_info,
	  *ob_nr,ob_cl,r_info,f_info,output_len,remlen,fct_tag,output,attr,
	  acl,*mk,recurring);
	break;
      case D2_ONE_LEVEL    :
      case D2_WHOLE_SUBTREE:
	ret_value = d26_u47_search_set(d26_subset,deref_obj,exact_obj,
	  &dn_info,*ob_nr,ob_cl,r_info,f_info,output_len,remlen,fct_tag,
	  output,acl,*mk);
	break;
    }
    if                              /*  hard error occurred           */
      (ret_value == D26_ERROR)
    {                               /*  set error apdu                */
      d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
			     D2_UNWILLING,&meslen);
      *remlen -= meslen;
    }
    DCE_SVC_LOG((GDS_S_INT_SEARCH_COMPLETE_MSG,ret_value));
    D27_022_ENTER_TIME_TRACE(GDS_S_INT_SEARCH_COMPL,NULL,0,0);
    return(ret_value);
  }

}                                   /*  d26_u00_search                */
