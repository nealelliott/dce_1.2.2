/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: remutil.c,v $
 * Revision 1.1.12.2  1996/02/18  18:15:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:06  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:50:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:44  root]
 * 
 * Revision 1.1.10.5  1994/08/11  07:58:08  marrek
 * 	Fix alignment problem (ot11618).
 * 	[1994/08/10  14:54:46  marrek]
 * 
 * Revision 1.1.10.4  1994/06/21  14:46:18  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:37:12  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  15:53:14  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:28:10  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:13:54  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:25:46  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  17:37:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:24:57  marrek]
 * 
 * Revision 1.1.8.1  1993/10/29  15:11:32  keutel
 * 	CR 9261
 * 	[1993/10/29  14:06:01  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:59:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:27:17  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:58:58  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:34:38  marrek
 * 	avoid 1 internal serach
 * 	[1993/02/01  15:40:49  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:44:18  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:35  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:34:25  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  10:27:27  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:53:36  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:32:50  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char remutil_rcsid[] = "@(#)$RCSfile: remutil.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:15:34 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : remutil.c  < DS_REMOVE_OBJECT >                     */
/*                                                                    */
/* AUTHOR       : Volpers                                             */
/* DATE         : 24.03.88                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |24.03.88 |  Original                      | HV |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module-identification                          */
/*                                                                    */
/**********************************************************************/



/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

/*****  external Includes    *****/

/*****  internal Includes    *****/

#include <d26dsa.h>
#include <dce/d27util.h>
#include <d26schema.h>
#include <malgcfe.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_REM_INSRCH_LEN      8000

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

/******** EXPORT ********/

/******** LOCAL  ********/
/* STATIC    ...    */
static const char function[]      = "d26_u80_check_child";
static byte exact_name[D2_DNL_MAX+1]; /* exact name for error logging   */

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** EXPORT d26_u80_check_child() ********/

signed16 d26_u80_check_child (D23_ds_name *ds_name, byte *a_in, byte **a_out, 
	signed32 *length, byte **dap_mess, signed32 *dap_len, boolean attr_req,
	D26_attr_info *attr_inf, signed32 *oclass, D26_acl *acl, signed32 *mk,
	boolean *recurring, String *record, boolean *al_flag, signed16 *obj_idx,
	signed16 op_id, D26_acl *sup_acl_mrdn, signed32 sup_mk_mrdn);

/******** LOCAL  ********/

static void d26_u81_search_params (byte *, char *, signed16, D23_av_value *,
	byte *, signed32, boolean, boolean);
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_u80_check_child (...)                             */
/*                                                                    */
/* AUTHOR:      Volpers D AP 11                                       */
/* DATE:        24.03.88                                              */
/*                                                                    */
/* SYNTAX       : Short d26_u80_check_child (                         */
/*                                   D23_ds_name    *ds_name,         */
/*                                   char           *a_in,            */
/*                                   Char           **a_out,          */
/*                                   Int            *length,          */
/*                                   Char          **dap_mess,        */
/*                                   Int           *dap_len,          */
/*                                   Bool           attr_req,         */
/*                                   D26_attr_info  *attr_inf,        */
/*                                   Long           *oclass,          */
/*                                   D26_acl        *acl,             */
/*                                   Long           *mk,              */
/*                                   Bool           *recurring,       */
/*                                   String         *record,          */
/*                                   Bool           *al_flag,         */
/*                                   Short          *obj_idx,         */
/*                                   Short           op_id)           */
/*                                                                    */
/* DESCRIPTION: this function checks whether the given object has     */
/*              children. if not, the function checks the access      */
/*              rights and give the record back if the requestor      */
/*              have the rights to modify the naming attribute.       */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                  D23_ds_name   *ds_name;     pointer to            */
/*                                              distinguished name    */
/*                  Char          *a_in;     pointer to input message */
/*                  Int           in_len;    length of input message  */
/*                  Char          **a_out;   pointer to return message*/
/*                  Int           *length;   length of return message */
/*                  Char          **dap_mess; pointer to DAP-message  */
/*                  Int           *dap_len;  length of DAP-message    */
/*                  Bool          attr_req;  if attributes wanted     */
/*                  Short         op_id;     ROS-ID of operation      */
/*                                                                    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                  D26_attr_info *attr_inf;    pointer to attribute  */
/*                                              information           */
/*                  Long          *oclass;      index in OCT          */
/*                                                                    */
/*                  D26_acl       *acl;         array of "shorts"     */
/*                                              pointing to the DN-   */
/*                                              list (1st dimension:  */
/*                                              access class;         */
/*                                              2nd dimension: up to  */
/*                                                             4 DNs  */
/*                  Long          *mk;          index of master       */
/*                                              DSA in DN-list        */
/*                  Bool          *recurring;   a flag indicating     */
/*                                              whether recurring     */
/*                                              attributes are        */
/*                                              supplied              */
/*                  String        *record;      pointer to ISAM-record*/
/*                                              (containing entry     */
/*                                              information)          */
/*                  Bool          *al_flag;     indicates, whether    */
/*                                              base object is alias  */
/*                  Short         *obj_idx;     object number of base */
/*                                              object                */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:              no error occured          */
/*                  D2_ORPHAN:              the object that will be   */
/*                                          deleted have children     */
/*                  D2_NFOUND:              Entry not found           */
/*                  D2_ACCESS_RIGHTS_INSUFFICIENT:                    */
/*                                          it is not allowed to      */
/*                                          delete this entry         */
/*                  D26_ERROR:              any error occurred        */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  d26_u00_search()                                  */
/*                  d26_u81_search_params()                           */
/*                  d27_007_norm_name()                               */
/*                  d27_010_set_error_apdu()                          */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

 signed16 d26_u80_check_child (
				  D23_ds_name    *ds_name,
				  byte           *a_in,
				  byte          **a_out,
				  signed32       *length,
				  byte          **dap_mess,
				  signed32       *dap_len,
				  boolean         attr_req,
				  D26_attr_info  *attr_inf,
				  signed32       *oclass,
				  D26_acl        *acl,
				  signed32       *mk,
				  boolean        *recurring,
				  String         *record,
				  boolean        *al_flag,
				  signed16       *obj_idx,
				  signed16        op_id,
				  D26_acl        *sup_acl_mrdn,
				  signed32        sup_mk_mrdn
			      )

{                                   /*  d26_u80_check_child()         */


/**********************************************************************/
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/
 signed16         ret_value = D2_NO_ERR;
 signed16         search_ret_val = D2_NO_ERR;
 D26_dninfo       dn_info;
 byte            *p;
 char            *c;

				/* message blocks for DS_SEARCH   */
 byte           *sm_in;
 char            obj_name[D2_DNL_MAX+1];

                                /* attribute info for last RDN of */
				/* the object (for acl_check)     */
 D26_attr_info   attr_info[D2_NO_AVA+1]; 
 boolean          base_obj;     /* sign for base object           */
 D23_av_value    *bo_ep;        /* encoded part of the base object*/
 boolean          att_want;     /* attributes wanted              */
				/* sign whether acl check is      */
				/* necessary                      */
 signed16 	  i;
 D26_nam_at      *nam_atp;	/* pointer to array of naming     */
                                /* attribute descriptions         */
 signed32         serv_ctrls;   /* service controls from message  */
 signed32         rem_len;      /* remaining length               */
 signed32         meslen;       /* length of message for 010-fct. */
 signed16         rop_id;       /* reduced operation ID           */

 signed16         l_sup;                  /* length of superior   */
 char             sup_normed[D2_DNL_MAX]; /* normed superior name */
 byte             sup_exact_name[D2_DNL_MAX+1];
 D26_acl         *sup_acl;
 D26_acl          sup_acl_mem[D2_ACLIDX_MAX]; /* ACL of superior      */
 signed32         sup_mk;       /* MK  of superior                */
 boolean          sup_al_flag;  /* al_flag of superior            */
 signed16         sup_obj_idx;  /* obj_idx of superior            */
 signed32         sup_oclass;   /* object class of superior       */
 signed32         sup_rem_len;  /* remaining length               */
 char            *sup_out ;     /* output message for parent      */
 signed32         sup_length;   /* lenght of search-mess superior */
 D2_name_string   name_for_acl_check; /* needed for acl check     */
 signed16         acl_obj_idx;        /* needed for acl check     */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

rop_id  = op_id > D2_ADM_OPID ? op_id - D2_ADM_OPID : op_id;

base_obj    = FALSE;
att_want    = FALSE;

/* set d26_serv_contr */
p = rop_id == D23_REMOBJT ? a_in + D23_S_REMOBJ : a_in + D23_S_RPRDN;
d26_a02_analyse_chaining_args(&p);
d26_a03_analyse_common_arguments(&p);

if (op_id < D2_ADM_OPID) /* Standard. Op. -> use Service controls given */
{
	      /* global variable was set in d26_700_analyse_add_mess... */
  serv_ctrls = d26_serv_contr->d23_scopt & D2_LOCALSCOPE ?
	 d26_serv_contr->d23_scopt | (signed32) D2_DREFALIAS :
       /* else set D2_NOCACHE to get referrals in case of copy is found */
	 d26_serv_contr->d23_scopt | (signed32) D2_DREFALIAS | (signed32) D2_NOCACHE;
}
else
{
  serv_ctrls = (signed32) D2_DREFALIAS | (signed32) D2_LOCALSCOPE;
}

if
   ((sm_in = (byte *)D27_MALLOC(svc_c_sev_warning,D26_REM_INSRCH_LEN)) == NULL)
{
  d27_010_set_error_apdu((D23_rserror *)*a_out,D2_TEMP_ERR,D2_SYS_ERR,
     D2_UNWILLING,length);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 1));
  return(D26_ERROR);
}

bo_ep = (D23_av_value *)(ds_name->d23_dscomplete == FALSE ?
	(String)(ds_name + 1) + ds_name->d23_dsoff : NULL);
d26_u81_search_params(a_in,(char *)(ds_name + 1),ds_name->d23_dslen,bo_ep,
		      sm_in,serv_ctrls,base_obj,att_want);
				    /*  call search to check whether  */
				    /*  children exists               */
search_ret_val = (d26_u00_search(TRUE, rop_id, (byte *) sm_in, length,
			    (byte **) a_out,&rem_len,exact_name , attr_inf,
			    oclass,acl, mk, recurring, record, al_flag, 
			    obj_idx));

if                                  /*  search was not successful     */
   (search_ret_val != D2_NOERROR)
{                                   /*  unlock used files             */
  d26_i14_unlock();
  if                /*  no result present                         */
     (search_ret_val == D2_REF_ERROR)
  {                 /*  write the error referrals                 */
    if (search_ret_val == D2_REF_ERROR &&
	op_id < D2_ADM_OPID &&
	 (!(serv_ctrls & D2_NOCHAINING) && serv_ctrls & D2_PREF_CHAINING))
    {                              /* chain the operation            */
       ret_value = d26_d01_chain_request((byte *) *dap_mess, *dap_len, a_out,
			       length, &rem_len,op_id,(D23_rsrqpb *)NULL);
       if (ret_value != D2_REF_ERROR && ret_value != D26_ERROR)
       {
	 *length -= rem_len;
	 *obj_idx = D26_IDX_NOT_DEF;  /* to indicate the chaining */
  	 d26_u41_free_referrals();
	 free(sm_in);
   	 DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
	 return(ret_value);
       }                           /* else write error referral      */
    }
    if              /*  hard error from writing referrals         */
      (d26_u63_write_error_referral(a_out,length, &rem_len, d26_referral) != 
								D2_NO_ERR)
    {               /*  set  error apdu                           */
       d27_010_set_error_apdu((D23_rserror *)*a_out,D2_TEMP_ERR,
			      D2_REM_ERR,D2_UNWILLING,&meslen);
       rem_len -= meslen;
     }
   }
  d26_u41_free_referrals();
  *length -= rem_len;
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,search_ret_val));
  free(sm_in);
  return(search_ret_val);
}

				  /*  search was succesful:           */
				  /*  check result message whether    */
				  /*  children exists                 */
if                                /*  not an alias                    */
  (*al_flag == FALSE)
{                                 /*  check whether children exist    */
  if                              /*  no message has been written     */
      (0 == d26_entry_count)
  {                               /*  no alias object and no children */
				  /*  norm name                       */
    ds_name->d23_dslen = d27_012_remove_blanks((String) (ds_name + 1),
				      ds_name->d23_dslen,
				      (String) (ds_name + 1));
    d27_007_norm_name((String) (ds_name + 1), ds_name->d23_dslen,
		       (String) obj_name);
  } else {                        /*  error D2_ORPHAN                 */
    d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			     D2_ORPHAN, &meslen);
				  /*    unlock used files             */
    rem_len -= meslen;
    d26_i14_unlock();

     *length -= rem_len;
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 10));
     free(sm_in);
     return(D26_ERROR);
  }
}

if (*al_flag || op_id  == D23_REMOBJT)
{
  strcpy(sup_normed, (char *) d26_objname); /* generate name of parent object */
  l_sup = 0;
  for (c = &sup_normed[strlen(sup_normed) - 1];c != sup_normed ; c--)
  {    if (*c == D2_RDN_SEP)
       {   *c = D2_EOS;
	   break;
       }
  }
  if (c == sup_normed)                /* parent is root -> take scheme    */
  {
     strcpy(sup_normed, D2_NAME_SCHEME);
  }
  l_sup = strlen(sup_normed) + 1;  /* length including EOS */
}

if ((*al_flag && rop_id == D23_REMOBJT) /* only in the case of alias or    */
    || op_id  == D23_REMOBJT)           /* standard operation this search  */
{                                       /* of parent is needed             */
				    /* build search message for parent  */
  d26_u81_search_params(a_in,sup_normed,l_sup,(D23_av_value *)NULL,sm_in,
			D2_DREFALIAS, TRUE, FALSE);

  sup_length = *length;
  sup_rem_len = rem_len;
  sup_oclass = *oclass;
  sup_acl = sup_acl_mem;
  if
     ((sup_out = (String)D27_MALLOC(svc_c_sev_warning,D26_BLOCK_LEN)) == NULL)
  {
    d27_010_set_error_apdu((D23_rserror *)*a_out,D2_TEMP_ERR,D2_SYS_ERR,
		     D2_UNWILLING,length);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 11));
    free(sm_in);
    return(D26_ERROR);
  }

  if                                  /* search the parent object         */
    (ret_value = d26_u00_search(TRUE, rop_id, (byte *) sm_in, length,
			     (byte **) &sup_out, &rem_len, sup_exact_name,
			     attr_inf, oclass, sup_acl, &sup_mk,
			     recurring, record, &sup_al_flag,
			     &sup_obj_idx)
     != D2_NOERROR)
  {
    free(sup_out);
    *length -= rem_len;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
    free(sm_in);
    return(ret_value);
  }
  free(sup_out);
  *length = sup_length;            /* this is necessary because the search */
  rem_len = sup_rem_len;           /* doesn't accept the sup-variables     */
  *oclass = sup_oclass;            /* I don't see why...                   */

} else if (rop_id == D23_REPLRDN) {
  sup_mk = sup_mk_mrdn;
  sup_acl = sup_acl_mrdn;
  if (*al_flag)
  {
    if (d26_srt[*obj_idx].d26s_par_nr == -1) /* scheme is parent */
    {                                        /* (for acl_check)  */
      dn_info.d26_rd_anz = 0;
      d26_u06_get_name((D2_name_string) sup_normed, &dn_info);
      sup_obj_idx = dn_info.d26_rdn[dn_info.d26_rd_anz -1].d26_r_idx;
    } else
      sup_obj_idx = d26_srt[*obj_idx].d26s_par_nr;
  }
}

if
   (op_id < D2_ADM_OPID &&  search_ret_val == D2_NOERROR &&
    d26_srt[*obj_idx].d26s_par_nr > -1) /* not for superior == root */
{
  ret_value = d26_st01_chain_ref(search_ret_val, serv_ctrls, *mk,
				  sup_mk == D26_OWNNAME ? TRUE:FALSE,
				  op_id);
  if (ret_value != D2_NOERROR)
  {			/*  check error and return		      */
    switch (ret_value)
    {
     case D2_CHAINING_REQUIRED:
	ret_value = d26_d01_chain_request(*dap_mess, *dap_len, a_out,
				length, &rem_len,op_id,(D23_rsrqpb *)NULL);
	if (ret_value != D2_REF_ERROR && ret_value != D26_ERROR)
	{
	  *obj_idx = D26_IDX_NOT_DEF;  /* to indicate the chaining    */
          DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
	  break; 
	}               /*  else write error referral 		      */
     case D2_REF_ERROR:
	if              /*  hard error from writing referrals         */
	  (d26_u63_write_error_referral(a_out,length,&rem_len, d26_referral) 
								!= D2_NO_ERR)
	{               /*  set  error apdu                           */
	   d27_010_set_error_apdu((D23_rserror *)*a_out,D2_TEMP_ERR,
				  D2_REM_ERR,D2_UNWILLING,&meslen);
	   rem_len -= meslen;
	}               /*                                            */
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 12));
	ret_value = D26_ERROR;
	break;
     case D2_AFFECTS_MULTIPLE_DSAS:
	d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR,
			 D2_PAR_ERR, D2_AFFECTS_MULTIPLE_DSAS, &meslen);
	rem_len -= meslen;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 13));
	ret_value = D26_ERROR;
	break;
     default: /* error should never occur */
	ret_value = D26_ERROR;
	break;
    }
    d26_i14_unlock();
    d26_u41_free_referrals();
    *length -= rem_len;
    free(sm_in);
    return(ret_value);
  }
} else {
  d26_u41_free_referrals();
}

/* if data base modification not allowed return error BUSY */
if (d26_dbop_flag == D26_DB_RO)
{
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_TEMP_ERR,
		   D2_INT_ERR, D2_TOO_BUSY, &meslen);
  rem_len -= meslen;
  d26_i14_unlock();
  *length -= rem_len;
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 14));
  free(sm_in);
  return(D26_ERROR);
}

				    /*  if the right for modify the     */
				    /*  naming attribute exist the entry*/
				    /*  could be deleted                */

				    /*  put object name to global field */
  strcpy((char *) d26_objname, obj_name);
				  /*  check master-knowledge          */
  if (*mk == D26_OWNNAME || *al_flag == TRUE)
  {                               /*  master info is removed          */
     d26_ma_info = TRUE;
  } else {
     d26_ma_info = FALSE;
  } 

  if (*al_flag)                    /*  for alias                     */
  {                                /*  check acl from parent object  */
    acl_obj_idx = sup_obj_idx;
    name_for_acl_check = (D2_name_string) sup_normed;
  } else {
    acl_obj_idx = *obj_idx;
    name_for_acl_check = (D2_name_string) obj_name;
  }
				  /*  build at-index to last naming attr*/
  for (i=0, nam_atp=(d26_srt + acl_obj_idx)->d26s_nam_at;
                           nam_atp->d26s_index != -1; nam_atp++, i++)
  { 
     attr_info[i].d26_at_idx = nam_atp->d26s_index;
  } 
  attr_info[i].d26_at_idx = D26_EOL;  /*  last entry must have D26_EOL */


  if                                /*  ACl-error                     */
    ((ret_value = d26_u05_acl_check((D2_name_string) name_for_acl_check,
      D26_MODIFY, attr_info, *al_flag ? sup_acl : acl)) != D2_NOERROR)
  {                                 /*  set error message             */
    ret_value = (ret_value != D2_ACCESS_RIGHTS_INSUFFICIENT)
					       ? D2_NOT_SPEC : ret_value;
    d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			     ret_value, &meslen);
				    /*  unlock used files             */
    rem_len -= meslen;
    d26_i14_unlock();
				    /*  return (D26_ERROR)            */
    *length -= rem_len;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
    free(sm_in);
    return(ret_value);
  }                                 /*                                */

				    /*  read baseobject or alias      */
				    /*  and give it back              */
base_obj    = TRUE;

if (attr_req == TRUE)
{                                   /*  call search with requested attr*/
  att_want    = TRUE;
} else {
  att_want    = FALSE;
} 

serv_ctrls = (signed32) D2_DREFALIAS;   /* at this point no sc. are required */
d26_u81_search_params(a_in, (char *)(ds_name+1),ds_name->d23_dslen,bo_ep,
		      sm_in,serv_ctrls, base_obj,
		      att_want);
				    /*  call search to get the        */
				    /*  record and attributes         */
ret_value = (d26_u00_search(FALSE, rop_id, (byte *) sm_in, length,
			    (byte **) a_out,&rem_len,exact_name, attr_inf, 
			    oclass, acl,mk, recurring,record, al_flag, 
			    obj_idx));
free(sm_in);

if                                  /*  search was not successful     */
  (ret_value)
{   
/********* build ERROR_MESSAGE (copy from DS_SEARCH) ******************/

  if (ret_value == D2_REF_ERROR )
  {   
   if              /*  hard error from writing referrals         */
     (d26_u63_write_error_referral(a_out,length,&rem_len, d26_referral) != 
								D2_NO_ERR)
    {               /*  set  error apdu                           */
       d27_010_set_error_apdu((D23_rserror *)*a_out,D2_TEMP_ERR,
			      D2_REM_ERR,D2_UNWILLING,&meslen);
       rem_len -= meslen;
   }               /*                                            */
    d26_u41_free_referrals();
  } 
  *length -= rem_len;
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
  return(ret_value);
} 

*length -= rem_len;                 /* should be 0                    */
DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
return(ret_value);

}                                   /*  d26_u80_check_child()         */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u81_search_params (...)                         */
/*                                                                    */
/* AUTHOR       : Volpers, D AP 11                                    */
/* DATE         : 17.03.88                                            */
/*                                                                    */
/* SYNTAX       : Void  d26_u81_search_params(a_in, name, l_name,     */
/*                          bo_ep,s_in,serv_ctrls,base_obj,att_want)  */
/*                                                                    */
/*                Char          *a_in;                                */
/*                Char          *name;                                */
/*                Ushort        l_name;                               */
/*                D23_av_value  *bo_ep;                               */
/*                Char          *s_in;                                */
/*                Long          serv_ctrls;                           */
/*                Bool          base_obj;                             */
/*                Bool          att_want;                             */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                Char          *a_in;     REM_OBJECT-Invoke-Message  */
/*                Char          *name;     name of object             */
/*                Ushort         l_name;   length of name             */
/*                D23_av_value  *bo_ep;    encoded part of base object*/
/*                Long        serv_ctrls;  serv_ctrls from message    */
/*                Bool          base_obj;  base-object or one-level   */
/*                Bool          att_want;  switch whether req attr    */
/*                                                          wanted    */
/* OUTPUT PARAMETERS:                                                 */
/*                Char          *s_in;      SEARCH-Invoke-Message     */
/*                                                                    */
/* RETURN VALUE :   -                                                 */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d27_005_octncpy()                                 */
/*                  sprintf()                                         */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_u81_search_params (
				  byte           *a_in,
				  char           *name,
				  signed16        l_name,
				  D23_av_value   *bo_ep,
				  byte           *s_in,
				  signed32        serv_ctrls,
				  boolean         base_obj,
				  boolean         att_want
				  )
{                                   /*  d26_u81_search_params()       */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

 D23_ds_name    *ds_name;       /* distinguished name of object   */
				/* to be searched                 */
 D23_rq_info    *rq_info;       /* pointer to requested info      */
 byte           *scan ;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

				    /*  fill invoke message for DS_SEARCH */
((D23_insearch *) s_in)->d23_Ihdr.d23_versno
			   = ((D23_inremobjt *) (a_in))->d23_Phdr.d23_versno;
((D23_insearch *) s_in)->d23_Ihdr.d23_dirid
			    = ((D23_inremobjt *) (a_in))->d23_Phdr.d23_dirid;
s_in += D23_S_INSRCH;
a_in += D23_S_REMOBJ;

((D23_com_arg *) s_in)->d23_cmtag = D23_CMTAG;
((D23_com_arg *)s_in)->d23_cmali = D2_NOT_DEFINED;
((D23_com_arg *)s_in)->d23_cmctr.d23_scopt = serv_ctrls;

((D23_com_arg *) s_in)->d23_cmctr.d23_scpri = 0;            /* not used   */
((D23_com_arg *) s_in)->d23_cmctr.d23_sctlm = D2_T_UNLIMITED;
((D23_com_arg *) s_in)->d23_cmctr.d23_scslm = 1;
((D23_com_arg *) s_in)->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED;
((D23_com_arg *) s_in)->d23_cmopr.d23_op_nxtrdn = 0;
s_in += D23_S_CARG;

((D23_srcharg *) s_in)->d23_Itag = D23_ITAG;
if (base_obj == TRUE)
{   
  ((D23_srcharg *) s_in)->d23_Iobjsub = D2_BASE_OBJECT;
} else {
  ((D23_srcharg *) s_in)->d23_Iobjsub = D2_ONE_LEVEL;
} 
((D23_srcharg *) s_in)->d23_Iali = FALSE;
scan = (byte *) (s_in + D23_S_SCHARG);

ds_name = (D23_ds_name *) scan;
ds_name->d23_dstag = D23_NAMTAG;
ds_name->d23_dslen = l_name;
ds_name->d23_dscomplete = TRUE;
ds_name->d23_dsoff = D2_ALIGN(l_name);
scan += D23_S_NM;

strcpy((char *) scan, name);
scan += ds_name->d23_dsoff;

if (bo_ep != (D23_av_value *)NULL)
{
  *(D23_av_value *)scan = *bo_ep;
  scan += sizeof(D23_av_value);
  memcpy(scan,(String)(bo_ep + 1),bo_ep->d23_avlen);
  scan += bo_ep->d23_avoff;
}

rq_info = (D23_rq_info *) scan ;
rq_info->d23_rqtag = D23_REQTAG;
rq_info->d23_rqreq = D2_R_TYP;

if (att_want == TRUE)            /* requested attributes wanted    */
{   
				    /* put requested info to message  */
  rq_info->d23_rqnbr = D2_ALL_ATT_REQ;
} else {
  rq_info->d23_rqnbr = 0;
} 

scan += D23_S_REQ;
((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;

}                                   /*  d26_u81_search_params         */
