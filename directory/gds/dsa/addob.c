/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: addob.c,v $
 * Revision 1.1.715.2  1996/02/18  19:44:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:34  marty]
 *
 * Revision 1.1.715.1  1995/12/08  15:41:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  18:04 UTC  dtruong
 * 	OT 12695: Can't add object via DSP
 * 	[1995/12/08  15:05:47  root]
 * 
 * Revision 1.1.713.6  1994/10/05  08:00:03  keutel
 * 	OT 12490: ACL name not initialized when inherited from superior
 * 	[1994/10/04  15:48:28  keutel]
 * 
 * Revision 1.1.713.5  1994/08/11  07:58:03  marrek
 * 	Fix alignment problem (ot11618).
 * 	[1994/08/10  14:54:24  marrek]
 * 
 * Revision 1.1.713.4  1994/06/21  14:44:58  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:29:50  marrek]
 * 
 * Revision 1.1.713.3  1994/05/10  15:51:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:18:14  marrek]
 * 
 * Revision 1.1.713.2  1994/03/23  15:08:56  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:18:56  keutel]
 * 
 * Revision 1.1.713.1  1994/02/22  16:04:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:06:12  marrek]
 * 
 * Revision 1.1.711.3  1993/10/29  15:11:00  keutel
 * 	CR 9261
 * 	[1993/10/29  14:05:52  keutel]
 * 
 * Revision 1.1.711.2  1993/10/14  17:16:10  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:07:00  keutel]
 * 
 * Revision 1.1.711.1  1993/10/13  17:30:12  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:32:26  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:18:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  12:56:52  marrek]
 * 
 * Revision 1.1.4.6  1993/02/02  14:42:44  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.5  1993/02/02  11:14:36  marrek
 * 	Bug fixes for OT 6551 (sorting of multivalued RDNs)
 * 	[1993/02/01  14:24:16  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:57:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:17:13  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:04:11  marrek
 * 	November 1992 code drop
 * 	Prototyping of u10 was wrong
 * 	[1992/12/04  10:49:21  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  19:30:08  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  10:24:59  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:43:18  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:51:41  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char addob_rcsid[] = "@(#)$RCSfile: addob.c,v $ $Revision: 1.1.715.2 $ $Date: 1996/02/18 19:44:30 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d26addob.c    <DS_ADD_OBJECT>                       */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 23.12.87                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  : This module contains the "add object" function of   */
/*                DSA.                                                */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |23.12.87 |  Original                      | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*                     Module Identification                          */
/*								      */
/**********************************************************************/


/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

/*****  internal Includes  *****/
#include <d26dsa.h>
#include <d26schema.h>

/*****  external Includes  *****/
#include <malgcfe.h>
#include <dce/d27util.h>

/*****  Operating System - Includes  *****/
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D26_ADD_INSRCH_LEN     8000

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

typedef struct
{           D23_at_type   *d26_acl_attr;    /* ACL attribute          */
	    D23_at_type   *d26_mk_attr;     /* master knowledge attr. */
	    D23_at_type   *d26_alias_attr;  /* ALIAS attribute        */
	    D23_at_type   *d26_dnlis_attr;  /* DN-list attribute      */
} D26_spec_attr;                /* special attributes                 */

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

				    /* exact name for error logging   */
static  byte              exact_name[D2_DNL_MAX+1];
static  byte              sup_exact_name[D2_DNL_MAX+1];

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*								      */
/**********************************************************************/

signed16 d26_7_add_object(signed16, D23_inaddobjt *, signed32, byte **, byte **,
			signed32 *, boolean *);

/******** LOCAL  ********/
static signed16 d26_700_analyse_add_message(D23_inaddobjt *, byte **, 
			char **, signed16 *, D23_at_type **, Bool *, 
			signed32 *, D26_dns *);
static void  d26_701_search_params(D23_inaddobjt *, char *, signed16, Bool,
			signed32, byte *);
static void     d26_702_handle_old_rights(D26_acl *, D26_dns **, D26_dns *);
static signed16 d26_703_mandatory_attr_check(signed16 *, signed32 *, 
			D26_nam_at *, signed16 *);
static signed16 d26_704_attr_info_check(signed16, char *, D23_at_type **, 
			signed16 [], signed32 *, signed32 *, D26_attr_info *, 
			boolean *, D26_spec_attr *, D26_dns **, D26_dns *, 
			D26_acl *, D26_rdninfo *, signed16 *, signed32 *, 
			D23_rserror *, signed32 *);
static   signed32  d26_705_get_master_dsa_idx(D23_at_type *, D26_dns *);
static   signed16 d26_706_update_dnl(signed16, D26_dns **);
static   signed16 d26_707_handle_alias(byte **,signed16, D26_dninfo *, 
			D23_at_type *, D26_attr_info *, signed16, signed32, 
			signed32 *);
static   signed16   d26_709_attr_val_check(D23_at_type **, signed16 *, 
			D26_at **, D26_rdninfo *, D2_a_type *, signed32 *, 
			byte  **, unsigned16 *);
static   boolean  d26_710_is_subclass(signed32, signed32);
static   signed32   d26_711_check_attr(signed16 *, signed32 *, signed16 *);
static   signed16 d26_712_get_correct_obj_class(signed32 *, signed32);
static   void  d26_713_rem_superclasses(signed32 *);
static   signed16 d26_714_check_obcl_consistency(signed32 *);

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_7_add_object (...)                              */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 23.12.87                                            */
/*								      */
/* SYNTAX       : Short d26_7_add_object(op_id,a_in,dap_in,a_out,     */
/*                                       length,add_mk)               */
/*								      */
/*                Short           op_id;                              */
/*                D23_inaddobjt   *a_in;                              */
/*                char            **dap_in;                           */
/*                char            **a_out;                            */
/*                int             *length;                            */
/*                Bool            *add_mk;                            */
/*								      */
/* DESCRIPTION  : This function is the entry point of the ISERVICE-   */
/*                interface DS_ADD_OBJECT.                            */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Short           op_id;    IPC operation ID          */
/*                D23_inaddobjt   *a_in;    invoke message            */
/*                Ushort          *length;  maximum length of result  */
/*                                          (or error) message        */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                Char            **dap_in; pointer to dap-request    */
/*                Char            **a_out;  result (or error) message */
/*                Int             *length;  actual length of message  */
/*                Bool            *add_mk;  indicator whether master- */
/*                                          knowledge attribute must  */
/*                                          be appended by d26_m12_   */
/*                                          logg_message              */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      any error occurred                */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_srt     *d26_srt;   pointer to SRT            */
/*                  D26_oct     *d26_oct;   pointer to OCT            */
/*                  D26_at      *d26_at;    pointer to AT             */
/*                                                                    */
/*     Bool        d26_ulogging;    indicates whether DSA has to logg */
/*                                  update operations                 */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*     Long        d26_sup_mk;         master knowledge of superior's */
/*                                     entry (is set when log of upd. */
/*                                     operations is required         */
/*                                                                    */
/*     Char        d26_objname[];      normed objname                 */
/*								      */
/*     Bool        d26_ma_info      indicates whether DSA has to logg */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d26_u00_search()                                  */
/*                  d26_u05_acl_check()                               */
/*                  d26_u04_set_name_name_error_apdu()                */
/*                  d26_u06_get_name()                                */
/*                  d26_u09_init_dsa()                                */
/*                  d26_u20_sort_dn()                                 */
/*                  d26_u24_sort_acl_idx()                            */
/*                  d26_700_analyse_add_message()                     */
/*                  d26_701_search_params()                           */
/*                  d26_702_handle_old_rights()                       */
/*                  d26_704_attr_info_check()                         */
/*                  d26_705_get_master_dsa_idx()                      */
/*                  d26_706_update_dnl()                              */
/*                  d26_707_handle_alias()                            */
/*                  d26_750_create_obj()                              */
/*                  d26_751_create_rec_attr()                         */
/*                  d26_753_get_aliased_info()                        */
/*                  d26_i14_unlock()                                  */
/*                  d27_007_norm_name()                               */
/*                  d27_010_set_error_apdu()                          */
/*                  d27_012_remove_blanks()                           */
/*                  d27_018_sort_name()                               */
/*                  sprintf()                                         */
/*                  strlen()                                          */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*exoff	***************************************************************/

signed16 d26_7_add_object (	signed16         op_id,
				D23_inaddobjt   *a_in,
				signed32         in_length,
				byte           **dap_in,
				byte           **a_out,
				signed32        *length,
				boolean         *add_mk
			  )

{                                   /*  d26_7_add_object()            */

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 const char function[] = "d26_7_add_object";
 byte             name[D2_DNL_MAX+1];
 signed16         ret_value;    /* return values                  */
 signed16         search_ret_val;
 signed32         dap_len;      /* length of DAP-message          */
 signed32         meslen;       /* length of message for 010-fct. */
 signed32         rem_len;      /*remaining length of search mess.*/
 signed32         serv_ctrls;   /* service controls from message  */

 signed16         rop_id;       /* reduced operation ID           */
 char            *obj;          /* pointer to object              */
 signed16         l_obj;        /* length of object               */

				/* message blocks for DS_SEARCH   */
 byte             *sm_in;

				/* parameters for DS_SEARCH       */
 D26_dninfo       dn_info;
 D26_attr_info    attr[D2_AT_MAX + 2];
 D26_acl          acl_sup[D2_ACLIDX_MAX];
 D26_acl          acl_new[D2_ACLIDX_MAX];
 signed32         mk_idx;       /* index of master-DSA-name in DN-*/
				/* list                           */
 signed32         mk_sup;
 boolean          rec;
 String           record;
 boolean          al_flag;

 signed32         key;          /* C-ISAM key                     */

				/* array of pointers pointing to the */
				/* internal structure "dns" contain- */
				/* ing the ACL-names and MASTER-DSA  */
 D26_dns         *dnl_ptr[D2_ACLIDX_MAX + 1];
				/* array of pointers pointing to the */
				/* internal structure "dns" contain- */
				/* ing the ACL-names (sorted pointer */
				/* list) and MASTER-DSA              */
 D26_dns         *dnl_sort[D2_ACLIDX_MAX + 1];
				/* internal structure containing ACL-*/
				/* names and MASTER-DSA              */
 D26_dns          dns[D2_ACLIDX_MAX + 1];
 signed16         no_dnl_entries;/* number of entries in DNL-list    */

				/* pointer to attribute informations */
				/* in ADD_OBJECT-Invoke-Message      */
 D23_at_type     *at_info[D2_AT_MAX + 2];
				/* number of occurrencies of attributes*/
 signed16         anz_rec[D2_AT_MAX + 1];

 char             obj_name[D2_DNL_MAX];   /* object name of entry */
					  /* (after removing      */
					  /* blanks)              */
 char             sup_normed[D2_DNL_MAX]; /* normed superior name */
					  /* (after removing      */
					  /* blanks)              */
 char            *sup;          /* pointer to superior object     */
 signed16         l_sup;                  /* length of superior   */

 D26_attr_info    attr_info[D2_NO_AVA+1];/* attribute info list for last */
                                /* RDN of superior object (for acl_check)*/
 D26_spec_attr    spec_attr;    /* indicates whether the ACL,     */
				/* MASTER-KNOWLEDGE attribute,    */
				/* ALIASED_OBJECT attribute,      */
				/* DN-List attribute              */
				/* exists in the message          */
 boolean          rec_attr;     /* indicates whether a recurring  */
				/* attribute is to inserted       */

 signed16         srt_idx;      /* index of new RDN in SRT        */
				/* object class of superior object*/
 signed32         sup_oct_idx[D26_OCL_VAL_COUNT + 1];  
				/* object classes of new object   */
 signed32         oct_idx[D26_OCL_VAL_COUNT + 1];
 signed32         alias_class;  /* index of object class ALIAS in */
				/* OCT                            */

 signed16         aliased_idx;  /* index of aliased object attribute*/
				/* in AT                          */

 signed16         i;
 D26_nam_at	 *nam_atp;      /* pointer to array of naming     */
                                /* attribute descriptions         */
 char            *p;
 byte            *b;
 char            *p_attr_val;   /* pointer to attribute value     */
 D2_a_type        oid_err;      /* attribute type and length (for */
				/* d26_u15_attr_error)            */
 signed32         l_err = 0;    /* length of error (attr_error)   */
 Bool             nm_compl;     /* complete flag for name         */
 D23_chain_arg    *ca_save;     /* save the chaining args         */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

rop_id  = op_id > D2_ADM_OPID ? op_id - D2_ADM_OPID : op_id;

if                                  /*  error during initialization of DSA */
   (d26_u09_init_dsa(0))
{                                   /*  set error message             */
				    /*  return(D26_ERROR)             */
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_PERM_ERR, D2_SYS_ERR,
			 D2_NOT_SPEC, length);
  return(D26_ERROR);
}                                   /*                                */
				    /*  trace function entry          */
DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

mk_idx = D26_OWNNAME;
dnl_ptr[D2_ACLIDX_MAX] = &dns[D2_ACLIDX_MAX];
dns[D2_ACLIDX_MAX].d26_didx = &mk_idx;

				    /*  analyse ADD_OBJECT-Invoke-Message */
if                                  /*  message is not correct        */
   (d26_700_analyse_add_message((D23_inaddobjt *) a_in, dap_in, &obj, &l_obj, at_info,
				&nm_compl, &mk_idx, dnl_ptr[D2_ACLIDX_MAX]))
{                                   /*  set error message             */
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			 D2_NOT_SPEC, length);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 11));
  return(D26_ERROR);
} 
				    /* length without chain. args     */
dap_len = in_length - ((char *)*dap_in - (char *) a_in);


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
  serv_ctrls = (signed32) D2_DREFALIAS;
}

				    /* allocate space for search input */
if((sm_in = (byte *)D27_MALLOC(svc_c_sev_warning,D26_ADD_INSRCH_LEN)) 
    == NULL)
{
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_TEMP_ERR,
		   D2_SYS_ERR, D2_UNWILLING, &meslen);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 111));
  return(D26_ERROR);
}
				    /*  look for new object:          */
if (nm_compl)                       /* name could be completely decoded   */
{
  dn_info.d26_rd_anz = 0;
  if                                /* root object should be added    */
    (l_obj == 0)
  {                                 /* write name error               */
    rem_len = *length;
    d26_u04_set_name_error_apdu(a_out,length,&rem_len,D2_NFOUND,
       (signed32)D26_IDX_NOT_DEF,&dn_info);
    *length -= rem_len;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 12));
    free(sm_in);
    return(D26_ERROR);
  }
				    /*  remove blanks in DN of new object */
  l_obj = d27_012_remove_blanks(obj, l_obj, obj);
				    /* sort multivalued RDNs          */
  if (d27_018_sort_name((D2_name_string) obj) == D2_ERROR)
  {                                   /*  set error message             */
    d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			   D2_NAMING_VIOLATION, length);
				      /*  return (D26_ERROR)            */
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 13));
    free(sm_in);
    return(D26_ERROR);
  }
				    /*  "norm" new object name		*/
  d27_007_norm_name(obj, l_obj, obj_name);
				    /*  put object name to global field */
  strcpy((char *) d26_objname, obj_name);

  if                                  /*  DN of new object is wrong     */
     (d26_u06_get_name((D2_name_string) obj, &dn_info))
  {                                   /*  set error message             */
    d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			   D2_NAMING_VIOLATION, length);
				      /*  return (D26_ERROR)            */
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 14));
    free(sm_in);
    return(D26_ERROR);
  } else {                            /*  determine srt_idx of object   */
    srt_idx = dn_info.d26_rdn[dn_info.d26_rd_anz - 1].d26_r_idx;
  }                                   /*                                */

  strcpy(sup_normed, obj_name);
  if (dn_info.d26_rd_anz == 1)        /* superior is root        */
  {   sup_normed[0] = D2_EOS;
      sup = NULL;
      l_sup = 0;
  } else {                            /*  creating superior name */
      for (p = &sup_normed[strlen(sup_normed) - 1]; ; p--)
      {    if (*p == D2_RDN_SEP)
	   {   *p = D2_EOS;
	       break;
	   }
      }
      sup = sup_normed;
      l_sup = strlen(sup_normed);
  }
				    /*  fill parameter block for DS_SEARCH */
  d26_701_search_params(a_in, sup_normed, l_sup, nm_compl,serv_ctrls, sm_in);

} else {                            /* search with the incomplete name     */
				    /* fill parameter block for DS_SEARCH  */
  d26_701_search_params(a_in, obj, l_obj, nm_compl, serv_ctrls, sm_in);
}
				    /*  look for superior object:     */
				    /* also in case of first level object */
				    /* to get ACL of scheme               */
search_ret_val = d26_u00_search(TRUE, rop_id, (byte *) sm_in, length, 
			       (byte **) a_out,
			       &rem_len,sup_exact_name, attr, sup_oct_idx,
			       acl_sup, &mk_sup, &rec,&record, &al_flag, &i);

if                                  /*  search was not successful     */
   ((search_ret_val != D2_NOERROR && op_id > D2_ADM_OPID) ||
    (op_id < D2_ADM_OPID && (search_ret_val != D2_NOERROR &&
				 search_ret_val != D2_REF_ERROR)))

{                                   /*  unlock used files             */
  if                /*  no result present                         */
     (search_ret_val == D2_REF_ERROR)
  {                 /*  write the error referrals                 */
    if (nm_compl)
    {
      d26_referral->d26_new_rdn = (D2_name_string) obj_name;
    }
    if              /*  hard error from writing referrals         */
      (d26_u63_write_error_referral(a_out,length, &rem_len,  d26_referral) !=
								D2_NO_ERR)
    {               /*  set  error apdu                           */
       d27_010_set_error_apdu((D23_rserror *)*a_out,D2_TEMP_ERR,
			      D2_REM_ERR,D2_UNWILLING,&meslen);
       rem_len = *length - meslen;
     }               /*                                            */
   }                 /*                                            */
  d26_u41_free_referrals();
  d26_i14_unlock();
  *length -= rem_len;
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 15));
  free(sm_in);
  return(D26_ERROR);
} else if                               /*  superior is alias entry       */
      (al_flag == TRUE)
{                                   /*  set error message             */
				    /*  return(D26_ERROR)             */
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			 D2_NAMING_VIOLATION, length);
				    /*  unlock used files             */
  d26_i14_unlock();
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 16));
  free(sm_in);
  return(D26_ERROR);
}
free(sm_in);

if (nm_compl)                       /* name was complete              */
{
  if                                /*  superior is root              */
    (l_sup == 0)
  {
    *sup_oct_idx = D26_ROOT;
    strcpy((char *) exact_name, (char *) obj);
  }
  else
  {                               /* build exact name for error logging   */
    for (p = &obj[strlen(obj) - 1]; ; p--)
    {    if (*p == D2_RDN_SEP)    /* last namepart is given in exact form */
	 {
	     break;
	 }
    }
    strcpy((char *) exact_name, (char *) sup_exact_name);
    strcat((char *) exact_name,p);
  }

}                                   /* name was not complete ->         */
else {                              /* st01 has only to check wether to */
  mk_sup = mk_idx = 0;              /* chain or to give referral        */
}

if                                  /*  standard mod. op -> check       */
				    /*  wether to chain etc.            */
      ((op_id < D2_ADM_OPID) && ((search_ret_val == D2_NOERROR) ||
				 (search_ret_val == D2_REF_ERROR)) &&
	l_sup > 0)                  /* not for first level objects      */
{   
  ret_value = d26_st01_chain_ref(search_ret_val, serv_ctrls, mk_sup,
				 mk_idx == D26_OWNNAME ? TRUE:FALSE,
				 op_id);

  if (ret_value != D2_NOERROR)
  {
    switch (ret_value)
    {
     case D2_CHAINING_REQUIRED:
	ret_value = d26_d01_chain_request(*dap_in, dap_len, a_out,
	    length,&rem_len,D23_ADDOBJT,(D23_rsrqpb *)NULL);
	if (ret_value != D2_REF_ERROR && ret_value != D26_ERROR)
	{ 
   	  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
	  break;
	}               /* else write error referral                  */
     case D2_REF_ERROR:
	if (nm_compl)
	{
	  d26_referral->d26_new_rdn = (D2_name_string) obj_name;
	}
	if              /*  hard error from writing referrals         */
	  (d26_u63_write_error_referral(a_out,length,&rem_len, d26_referral) != 
								D2_NO_ERR)
	{               /*  set  error apdu                           */
	   d27_010_set_error_apdu((D23_rserror *)*a_out,D2_TEMP_ERR,
				  D2_REM_ERR,D2_UNWILLING,&meslen);
	   rem_len = *length - meslen;
	}               /*                                            */
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 18));
	ret_value = D26_ERROR;
	break;
     case D2_AFFECTS_MULTIPLE_DSAS:
	d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR,
			 D2_PAR_ERR, D2_AFFECTS_MULTIPLE_DSAS, length);
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 19));
	ret_value = D26_ERROR;
	break;
     default: /* error */
	ret_value = D26_ERROR;
	break;
    }
    d26_u41_free_referrals();
    d26_i14_unlock();
    *length -= rem_len;
    return(ret_value);
  }
} 

/* if data base modification not allowed return error BUSY */
if (d26_dbop_flag == D26_DB_RO)
{
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_TEMP_ERR,
		   D2_INT_ERR, D2_TOO_BUSY, &meslen);
  rem_len -= meslen;
  d26_u41_free_referrals();
  d26_i14_unlock();
  *length -= rem_len;
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 20));
  return(D26_ERROR);
}

				    /*  build AT-index to last naming attr*/
for (i=0, nam_atp=(d26_srt + srt_idx)->d26s_nam_at; 
                         nam_atp->d26s_index != -1; nam_atp++, i++)
{ 
attr_info[i].d26_at_idx = nam_atp->d26s_index;
} 
attr_info[i].d26_at_idx = D26_EOL;  /*  last entry must have D26_EOL */

				    /*  check permission to create the obj.*/
if                                  /*  ACl-error                     */
   ((signed32)(ret_value = d26_u05_acl_check((D2_name_string) sup_normed,
					 D26_MODIFY, attr_info, acl_sup))
					  != D2_NOERROR)
{                                   /*  set error message             */
  ret_value = (ret_value != D2_ACCESS_RIGHTS_INSUFFICIENT)
						 ? D2_NOT_SPEC : ret_value;
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			 ret_value, length);
				    /*  unlock used files             */
  d26_i14_unlock();

  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 21));
  return (D26_ERROR);
}                                   /*                                */

if((sm_in = (byte *)D27_MALLOC(svc_c_sev_warning,D26_ADD_INSRCH_LEN)) 
    == NULL)
{
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_TEMP_ERR,
		   D2_SYS_ERR, D2_UNWILLING, &meslen);
  d26_i14_unlock();
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 211));
  return(D26_ERROR);
}
				    /*  look for new object:          */
				    /*  fill parameter block for DS_SEARCH */
d26_701_search_params(a_in, obj_name, l_obj, nm_compl,
		      (signed32) (D2_DREFALIAS | D2_NOREFERRAL | D2_LOCALSCOPE),
		      sm_in);

/* for this local search the chaining arguments are not relevant - set */
/* them to 0 */
ca_save = d26_chain_arg;
d26_chain_arg = (D23_chain_arg *) NULL;

if                                  /*  return value != NOT_FOUND     */
 ((ret_value = d26_u00_search(FALSE,rop_id, (byte *) sm_in, length, 
				   (byte **) a_out, &rem_len, name, attr, 
				   sup_oct_idx, acl_new, &mk_sup, &rec, 
				   &record, &al_flag, &i)) 
				   != D2_NFOUND)
{                                   /*  unlock used files             */
  free(sm_in);
  d26_i14_unlock();
  d26_chain_arg = ca_save;
  if                                /*  search was successful         */
     (!ret_value)
  {                                 /*  error = D2_ALREADY_EXISTS     */
    d26_u41_free_referrals();       /*  if referrals are returned */
    d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			   D2_ALREADY_EXISTS, length);
				    /*  return(D26_ERROR)             */
    return(D26_ERROR);
  } else {                          /*  return(ret_value of DS_SEARCH)*/
    *length -= rem_len;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 22));
    return(ret_value);
  }                                 /*                                */
}                                   /*                                */
free(sm_in);

d26_chain_arg = ca_save;

				    /*  check attributes of new object */
if                                  /*  at least one attribute is wrong*/
   (d26_704_attr_info_check(srt_idx, obj, at_info, anz_rec, oct_idx,
			    sup_oct_idx, &attr[0], &rec_attr, &spec_attr,
			    dnl_ptr, &dns[0], acl_new,
			    &dn_info.d26_rdn[dn_info.d26_rd_anz - 1],
			    &aliased_idx, &alias_class,
			    (D23_rserror *) *a_out, length))
				  /*  return(D26_ERROR)             */
{                                 /*  unlock used files             */
  d26_i14_unlock();
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 23));
  return(D26_ERROR);                /* 704 sets length to error length*/
}

				    /*  set indicator whether master- */
				    /*  knowledge attribute must be   */
				    /*  added, that is when no master-*/
				    /*  knowledge is given it must be */
				    /*  added by d26_m12_logg_message */
(*add_mk) = (spec_attr.d26_mk_attr == (D23_at_type *) NULL) ?
				      TRUE : FALSE;

				    /* create dn_info with exact name */
dn_info.d26_rd_anz = 0;
if                                  /*  DN of new object is wrong     */
   (d26_u06_get_name((D2_name_string) exact_name, &dn_info))
{                                   /*  set error message             */
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			 D2_NAMING_VIOLATION, length);
				    /*  return (D26_ERROR)            */
  d26_i14_unlock();
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 24));
  return(D26_ERROR);
}

if                                  /*  alias is to be created        */
   (spec_attr.d26_alias_attr != (D23_at_type *) NULL)
{                                   /*  handle alias                  */
				    /*  return                        */
  ret_value = d26_707_handle_alias(a_out, srt_idx, &dn_info,
				   spec_attr.d26_alias_attr, attr,
				   aliased_idx, alias_class, length);
  if ((ret_value == D2_NO_ERR) && (d26_ulogging == TRUE)
       && (mk_sup == D26_OWNNAME))
  {
       d26_ma_info = TRUE;
  } else {
       d26_ma_info = FALSE;
  } 
  *add_mk = FALSE;
}
else  /* real object to be created */
{
  if                                  /*  ACL-attribute is absent       */
     (spec_attr.d26_acl_attr == (D23_at_type *) NULL)
  {                                   /*  handle "old" rights           */
    d26_702_handle_old_rights(acl_sup, dnl_ptr, &dns[0]);
				      /*  sort ACL-Shorts               */
    d26_u24_sort_acl_idx(dnl_ptr,D2_ACLIDX_MAX, dnl_sort, &no_dnl_entries);
  } else {                                /*  sort DNs of ACL-list          */
    d26_u20_sort_dn((D2_name_string) obj_name, dnl_ptr, D2_ACLIDX_MAX,
		    dnl_sort, &no_dnl_entries);
  }

  if                                  /*  master DSA is any DSA         */
     (mk_idx != D26_OWNNAME)
  {                                   /*  put DSA-Name to list to be updated */
    dnl_sort[no_dnl_entries++] = &dns[D2_ACLIDX_MAX];
    d26_ma_info = FALSE;
  } else {                                /*  check whether DNLIST is given */
    if                                /*  DN-LIST is given as attribute */
       (spec_attr.d26_dnlis_attr != (D23_at_type *) NULL)
    {                                 /*  create error message          */
      oid_err.d2_type = (Ob_id_string) (spec_attr.d26_dnlis_attr + 1);
      oid_err.d2_typ_len = spec_attr.d26_dnlis_attr->d23_atlen;
      p_attr_val = (char *) spec_attr.d26_dnlis_attr;
      p_attr_val += D23_S_ATT + ((D23_at_type *) p_attr_val)->d23_atoff;
      p_attr_val += D23_S_AV;
      d26_u15_attr_error((byte **) a_out, (D2_name_string) obj, 1,
			 D2_CONSTRAINT_VIOLATION, &oid_err,
			 (d26_at + attr[0].d26_at_idx)->d26a_rep,
			 (d26_at + attr[0].d26_at_idx)->d26a_rep,
  /*                       D2_DISTNAME, D2_DISTNAME,               */
			 ((D23_av_value *) p_attr_val)->d23_avlen,
			 (byte *) (p_attr_val + D23_S_ATV), length, &l_err);
      *length = l_err;
      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 25));
      return(D26_ERROR);
    }
    d26_ma_info = TRUE;
  }

  if                                  /*  update of DNL is successful   */
     ((ret_value = d26_706_update_dnl(no_dnl_entries, dnl_sort)) == D2_NOERROR)
  {                                   /*  create new object             */
    if                                /*  creation was successful       */
       ((ret_value = d26_750_create_obj(oct_idx, &dn_info,
					spec_attr.d26_acl_attr,
					 spec_attr.d26_mk_attr, dnl_ptr,
					at_info, attr, anz_rec, &key))
		   == D2_NOERROR)
    {
      if                              /*  recurring attributes to be inserted*/
	 (rec_attr == TRUE)
      {                               /*  create recurring attributes   */
       ret_value = d26_751_create_rec_attr(oct_idx[0],
					   (d26_oct + oct_idx[0])->d26c_filenr,
					   key, attr, at_info, anz_rec);
      }
    }
  }
}
				    /*  unlock used files             */
d26_i14_unlock();
if                                  /*  operation is successful       */
   (!ret_value)
{                                   /*  generate result message (OK)  */
  *length = 0;
  b = *a_out;
  if                    /*  DSP was coming in                         */
    (d26_chain_arg != (D23_chain_arg *)NULL)
  {                     /*  write chaining result                     */
  ((D23_chain_res *) b)->d23_chrtag  = D23_CHRTAG;
  b += D23_S_CHRES;
  *length += D23_S_CHRES;
  }                     /*                                            */

  ((D23_eom *) b)->d23_eomtag = D23_EOMTAG;
  ((D23_eom *) b)->d23_eomfil = 0;
  ((D23_eom *) b)->d23_eom_oqoff = 0;
  *length += D23_S_EOM;

  if ((d26_ulogging == TRUE) && (d26_ma_info == TRUE))
  {    /* set d26_sup_mk */
       d26_sup_mk = mk_sup;
  } 
} else {
  if                                /* no alias                       */
   (spec_attr.d26_alias_attr == (D23_at_type *) NULL)
  {                                 /*  set error message             */
    d27_010_set_error_apdu((D23_rserror *)*a_out,D2_TEMP_ERR,D2_REM_ERR,
			   D2_UNWILLING,length);
  }                                 /* else err_msg is set in 707     */
}
				    /*  return (ret_value)            */
DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
return (ret_value);

}                                   /*  d26_7_add_object()            */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_700_analyse_add_message(...)                    */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 07.01.88                                            */
/*								      */
/* SYNTAX       : Short d26_700_analyse_add_message(add_m, dap_mess,  */
/*                                                 obj, l_obj, at_ptr,*/
/*                                                 nm_compl, mk_idx)  */
/*								      */
/*                D23_inaddobjt  *add_m;                              */
/*                String          dap_mess;                           */
/*                Char          **obj;                                */
/*                Ushort         *l_obj;                              */
/*                D23_at_type   **at_ptr;                             */
/*                Bool           *nm_compl;                           */
/*                signed32       *mk_idx;                             */
/*                D26_dns        *dns;                                */
/*								      */
/* DESCRIPTION  : This function will look for the DN of the new       */
/*                object. Furthermore a pointer to the first          */
/*                Attribute-Information-Block will be set.            */
/*                The lengths of the DN is returned, too.             */
/*                if the message contains wrong TAGs or other errors  */
/*                this function will return an error code.            */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                D23_inaddobjt  *add_m;    pointer to DS_ADD_OBJECT- */
/*                                          Invoke Message            */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                String         *dap_mess; pointer to DAP-part of    */
/*                                          ADD_message               */
/*                Char          **obj;      DN of new object          */
/*                Ushort         *l_obj;    length of DN              */
/*                D23_at_type  **at_ptr;    pointer to attribute      */
/*                                          information blocks        */
/*                Bool          *nm_compl;  name complete flag        */
/*                signed32      *mk_idx;    index of DSA in dnlist    */
/*                D26_dns        *dns;      entry in dns-list         */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      ADD_OBJECT-Invoke-Message not     */
/*                                  correct                           */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          d26_a02_analyse_chaining_args             */
/*                          d26_a03_analyse_common_arguments          */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  signed16 d26_700_analyse_add_message(
					D23_inaddobjt  *add_m,
					byte          **dap_mess,
					char          **obj,
					signed16       *l_obj,
					D23_at_type   **at_ptr,
					Bool           *nm_compl,
					signed32       *mk_idx,
					D26_dns        *dns
				       )

{                                   /*  d26_700_analyse_add_message() */

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

     char       *p_mess;        /* pointer to message             */
     D2_a_type   at_type;       /* attribute type                 */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

p_mess = (char *) (add_m + 1);

				    /*  tag is Chaining arguments     */
d26_a02_analyse_chaining_args((byte **) &p_mess);
*dap_mess = (byte *) p_mess;


				    /*  tag is common arguments       */
d26_a03_analyse_common_arguments((byte **) &p_mess); /* the function updates */
					          /* the global varaiable    */
					          /* d26_serv_contr          */

				    /*  copy DN of the new object     */
*obj = p_mess + D23_S_NM;
*nm_compl = ((D23_ds_name *)p_mess)->d23_dscomplete;
*l_obj = ((D23_ds_name *)p_mess)->d23_dslen;
				  /* determine address of attribute tag */

if (*nm_compl == FALSE) return(D2_NO_ERR);

/* makes sense only if nm_compl */
p_mess =  *obj + ((D23_ds_name *)p_mess)->d23_dsoff;

				    /*  store first attribute tag     */
*at_ptr = (D23_at_type *) p_mess;

				    /* search MK attribute */
while (((D23_eom *) p_mess)->d23_eomtag != D23_EOMTAG)
{
  at_type.d2_typ_len = ((D23_at_type *) p_mess)->d23_atlen;
  at_type.d2_type = (Ob_id_string) (p_mess + D23_S_ATT);
  if (!D2_ATTCMP(&at_type,D2_MASTER_KNOWLEDGE))
  {
    dns->d26_didx = mk_idx;
    *mk_idx = d26_705_get_master_dsa_idx((D23_at_type *) p_mess, dns);
    break;
  }
  else {  /* go to the next ATTTAG */
    p_mess += ((D23_at_type *) p_mess)->d23_atoff + D23_S_ATT;
    p_mess += D23_S_AV;
    while (((D23_at_type *)p_mess)->d23_atttag != D23_ATTTAG &&
	   ((D23_eom *) p_mess)->d23_eomtag != D23_EOMTAG)
    {
      p_mess += ((D23_av_value *) p_mess)->d23_avoff + D23_S_ATV;
    }
  }
}
				    /*  return (D2_NO_ERR)            */
return(D2_NO_ERR);

}                                   /*  d26_700_analyse_add_message() */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_701_search_params (...)                         */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 07.01.88                                            */
/*								      */
/* SYNTAX       : Void  d26_701_search_params(add_m, name, l_name,    */
/*                                            nm_compl,               */
/*                                            serv_cntr, search_m)    */
/*                D23_inaddobjt  *add_m;                              */
/*                char           *name;                               */
/*                Ushort          l_name;                             */
/*                Bool            nm_compl;                           */
/*                signed32        serv_cntr;                          */
/*                char           *search_m;                           */
/*                                                                    */
/* DESCRIPTION  : This function will build the DS_SEARCH-Invoke-      */
/*                for the internal DS_SEARCH function.                */
/*                It takes the header information of DS_ADD_OBJECT-   */
/*                Invoke-Message and copies the delivered             */
/*                distinguished name in the message block.            */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                D23_inaddobjt *add_m;     ADD_OBJECT-Invoke-Message */
/*                Char          *name;      name of object to be      */
/*                                          searched                  */
/*                Ushort         l_name;    length of object name     */
/*                Bool           nm_compl;  name complete flag        */
/*                signed32       serv_cntr; service controls for      */
/*                                          search                    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                Char          *search_m;  SEARCH-Invoke-Message     */
/*								      */
/* RETURN VALUE :   -                                                 */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_oct         *d26_oct;   pointer to OCT        */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d27_005_octncpy()                                 */
/*                  sprintf()                                         */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  void d26_701_search_params (
				  D23_inaddobjt  *add_m,
				  char           *name,
				  signed16        l_name,
				  Bool            nm_compl,
				  signed32        serv_cntr,
				  byte           *search_m
				 )
{                                   /*  d26_701_search_params()       */

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 D23_ds_name    *ds_name;       /* distinguished name of object   */
				    /* to be searched                 */
 D23_ds_name    *ds_in_name;    /* distinguished name in msg.     */
 D23_rq_info    *rq_info;       /* pointer to requested info      */
 byte           *srch_ptr = search_m;
 char           *str;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

				    /*  fill invoke message for DS_SEARCH */
*((D23_header *)srch_ptr) = *((D23_header *)add_m);
srch_ptr += D23_S_INSRCH;

((D23_com_arg *) srch_ptr )->d23_cmtag = D23_CMTAG;
((D23_com_arg *) srch_ptr )->d23_cmali = 0;
((D23_com_arg *) srch_ptr )->d23_cmctr.d23_scopt = serv_cntr;
((D23_com_arg *) srch_ptr )->d23_cmctr.d23_scpri = 0;        /* not used   */
((D23_com_arg *) srch_ptr )->d23_cmctr.d23_sctlm = D2_T_UNLIMITED;
((D23_com_arg *) srch_ptr )->d23_cmctr.d23_scslm = 1;
((D23_com_arg *) srch_ptr )->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED;
((D23_com_arg *) srch_ptr )->d23_cmopr.d23_op_nxtrdn = 0;
srch_ptr += D23_S_CARG;

((D23_srcharg *) srch_ptr )->d23_Itag = D23_ITAG;
((D23_srcharg *) srch_ptr )->d23_Iobjsub = D2_BASE_OBJECT;

ds_name = (D23_ds_name *)(srch_ptr + D23_S_SCHARG);
ds_name->d23_dstag = D23_NAMTAG;
ds_name->d23_dscomplete = nm_compl;
ds_name->d23_dslen = l_name == 0 ? 0 : l_name + 1;
ds_name->d23_dsoff = D2_ALIGN(ds_name->d23_dslen);
if (nm_compl)
{
  str = (char *) (ds_name + 1);
  if                                /*  superior is root              */
     (l_name == 0)
  {                                 /*  superior object = "SCHEME"    */
    strcpy(str, (char *) D2_NAME_SCHEME);
    ds_name->d23_dslen = strlen(str) + 1;
    ds_name->d23_dsoff = D2_ALIGN(ds_name->d23_dslen);
  } else {                          /*  copy name in SEARCH-message   */
    strcpy(str, name);
  }
  rq_info = (D23_rq_info *)(str + ds_name->d23_dsoff);
} else {
  str = (char *) ds_name;
			       /* in case of incomplete names 'name' points*/
			       /* still to the name in the message         */
  ds_in_name = (D23_ds_name *) (name - D23_S_NM);
  d27_502_copy_distinguished_name((byte **)&ds_in_name,(byte **)&str);
  rq_info = (D23_rq_info *) str;
}
			       /* put requested info (NONE) to message*/
rq_info->d23_rqtag = D23_REQTAG;
rq_info->d23_rqreq = D2_R_TYP;
rq_info->d23_rqnbr = 0;

				    /*  put EOM to message            */
((D23_eom *)(rq_info + 1))->d23_eomtag = D23_EOMTAG;
((D23_eom *)(rq_info + 1))->d23_eom_oqoff = 0;

}                                   /*  d26_701_search_params()       */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_702_handle_old_rigths(...)                      */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 14.01.88                                            */
/*								      */
/* SYNTAX       : Void  d26_702_handle_old_rights(p_acl, dnl_ptr, dns)*/
/*                D26_acl        *p_acl;                              */
/*                D26_dns       **dnl_ptr;                            */
/*                D26_dns        *dns;                                */
/*                                                                    */
/* DESCRIPTION  : This function updates the ACL-rigths if the new ACL */
/*                is missing. The references of negative values is    */
/*                updated.                                            */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                D26_acl       *p_acl;     pointer to acl-shorts of  */
/*                                          superior object           */
/*                D26_dns       **dnl_ptr;  address of pointer list   */
/*                                          pointing to DNL info      */
/*                D26_dns        *dns;      address of DNL information*/
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                D26_dns       **dnl_ptr;  pointer list pointing to  */
/*                                          DNL information           */
/*                D26_dns        *dns;      DNL information           */
/*								      */
/* RETURN VALUE :  -                                                  */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  void d26_702_handle_old_rights(D26_acl *p_acl, D26_dns **dnl_ptr,
				     D26_dns *dns)

{                                   /*  d26_702_handle_old_rights()   */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

     D26_dns    *p_max_dns;
     D26_dns   **pp_dnl;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for                                 /*  all ACL-indices               */
    (p_max_dns = dns + D2_ACLIDX_MAX,
     pp_dnl = dnl_ptr; dns < p_max_dns; pp_dnl++)
{                                   /*  set pointer to ACL-idx        */
  *pp_dnl = dns++;
  (*pp_dnl)->d26_dno = 1;
  if                                /*  ACL entry is not in DNL       */
     (*p_acl < D26_OWNNAME && *p_acl != D26_EOL)
  {                                 /*  update ACl reference          */
				    /*  (ACL-Short -= 1)              */
    (*p_acl)--;
  }                                 /*                                */
  else
  {                                 /* enter empty name               */
    (*pp_dnl)->d26_edname[0] = D2_EOS;
  }
  (*pp_dnl)->d26_didx = p_acl++;
}                                   /*                                */

}                                   /*  d26_702_handle_old_rigths()   */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_703_mandatory_attr_check(...)                   */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 14.01.88                                            */
/*								      */
/* SYNTAX       : Short d26_703_mandatory_attr_check(rec_no, oct_idx, */
/*                                                   nam_at, idx)    */
/*                Short *rec_no;                                      */
/*                Long  *oct_idx;                                     */
/*                D26_nam_at *nam_at;                                 */
/*                Short *idx;                                         */
/*                                                                    */
/* DESCRIPTION  : This function will check whether all mandatory      */
/*                attributes are given in the ADD-OBJECT attribute    */
/*                list. (The ACL and the Master-Knowledge attribute   */
/*                are specially handled --> they will be inserted     */
/*                implicitly if missing)                              */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Short *rec_no;        indicates wether an attribute */
/*                                      is given in the ADD-OBJECT    */
/*                                      invoke message                */
/*                                      (this array is "parallel" to  */
/*                                      the AT)                       */
/*                                      == 0: attribute missing       */
/*                                      >  0: number of occurrencies  */
/*                Long  *oct_idx;       object class array            */
/*                D26_nam_at  *nam_at;  array of naming attribute     */
/*                                      descriptions of last component*/
/* OUTPUT PARAMETERS:                                                 */
/*                Short *idx;           index of attribute in AT      */
/*                                      that caused error             */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:          all mandatory attributes are  */
/*                                      given                         */
/*                  D26_ERROR:          at least one mandatory        */
/*                                      attribute is missing          */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_at     *d26_at;         Attribute-Table (AT)  */
/*                  Int         d26_noat;       number of entries in  */
/*                                              AT                    */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  signed16 d26_703_mandatory_attr_check
(
  signed16   *rec_no,
  signed32    *oct_idx,
  D26_nam_at *nam_at,
  signed16   *idx
)

{                                   /*  d26_703_mandatory_attr_check()*/

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 signed16           acl_idx;       /* index of ACL in AT             */
 signed16           mk_idx;        /* index of MASTER_KNOWLEDGE in AT*/

 D2_a_type       attr;
 signed16          *mand_attr;     /* array of shorts with mandatory */
				    /* attributes (indices in AT)     */
				    /* [These indices must be valid in*/
				    /* the "rec_no" array too]        */
 D26_nam_at     *nam_atp;       /* pointer to array of naming     */
                                    /* attribute descriptions         */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

attr.d2_type = (Ob_id_string) D2_ACL;
attr.d2_typ_len = sizeof(D2_ACL) - 1;
acl_idx = d26_u03_attr_search(&attr);

attr.d2_type = (Ob_id_string) D2_MASTER_KNOWLEDGE ;
attr.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE) - 1;
mk_idx = d26_u03_attr_search(&attr);

for                                 /*  all object classes in list      */
   (; *oct_idx != D26_EOL; oct_idx++)
{                                   /*  scan the mandatory attributes   */
  for                               /*  all mandatory attributes of OCT */
				    /*  entry                           */
     (mand_attr = d26_oct[*oct_idx].d26c_mandatory; *mand_attr >= 0;
      mand_attr++)
  {                                 /*  check presence of attribute     */
    if                              /*  attribute is missing            */
      (*(rec_no + *mand_attr) == 0)
    {                               /*  check whether it is naming      */
      for                           /*  all naming attributes           */
	 (nam_atp = nam_at; nam_atp->d26s_index != *mand_attr; nam_atp++)
      {                             /*  check whether eond of list      */
				    /*  reached                         */
	if ( nam_atp->d26s_index == -1 )
	{                           /*  return(D26_ERROR)               */
	  *idx = *mand_attr;
	  return(D26_ERROR);
	} 
      } 
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */

				    /*  return(D2_NO_ERR)             */
return(D2_NO_ERR);

}                                   /*  d26_703_mandatory_attr_check()*/

/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_704_attr_info_check(...)                        */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 08.12.87                                            */
/*								      */
/* SYNTAX       : Short d26_704_attr_info_check (srt_idx, obj_name,   */
/*                                               at_str,              */
/*                                               anz_rec, oct_idx,    */
/*                                               sup_oct_idx,         */
/*                                               at_info, recurring,  */
/*                                               spec_attr, dnl_ptr,  */
/*                                               dns, acl_idx,        */
/*                                               rdn_info,            */
/*                                               aliased_idx,         */
/*                                               alias_class,         */
/*                                               error, length)       */
/*                Short           srt_idx;                            */
/*                Char           *obj_name;                           */
/*                D23_at_type   **at_str;                             */
/*                Short           anz_rec[];                          */
/*                Long           *oct_idx;                            */
/*                Long           *sup_oct_idx;                        */
/*                D26_attr_info  *at_info;                            */
/*                Bool           *recurring;                          */
/*                D26_spec_attr  *spec_attr;                          */
/*                D26_dns       **dnl_ptr;                            */
/*                D26_dns        *dns;                                */
/*                D26_acl        *acl_idx;                            */
/*                D26_rdninfo    *rdn_info;                           */
/*                Short          *aliased_idx;                        */
/*                Long           *alias_class;                        */
/*                D23_rserror    *error;                              */
/*                Int            *length;                             */
/*								      */
/* DESCRIPTION  : This function will look whether the supplied        */
/*                attributes are known by the DSA (in its DSA).       */
/*                As result three pointers are returned:              */
/*                One pointer indicates whether the ACL attribute is  */
/*                contained in the message block, one pointer shows   */
/*                whether the MASTER-KNOWLEDGE is supplied and the    */
/*                last one shows whether the ALIASED_OBJECT_ATTRIBUTE */
/*                is supplied.                                        */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                  Short            srt_idx;   index of last RDN in  */
/*                                              SRT                   */
/*                  Long            *sup_oct_idx; internal index of   */
/*                                              superior object class */
/*                                              in the OCT            */
/*                  Char            *obj_name;  objname               */
/*                  D23_at_type    **at_str;    attribute structure   */
/*                                              a pointer to the first*/
/*                                              attribute tag         */
/*      for ACl-attribute:                                            */
/*                  D26_dns        **dnl_ptr;   address of pointer    */
/*                                              array where the       */
/*                                              pointers pointing to  */
/*                                              internal structure are*/
/*                                              stored                */
/*                  D26_dns         *dns;       address of internal   */
/*                                              structure where the   */
/*                                              ACL information can be*/
/*                                              stored                */
/*                  D26_acl         *acl_idx;   address of array of   */
/*                                              "shorts" where the    */
/*                                              index in the DNL can  */
/*                                              be stored             */
/*                  D26_rdninfo     *rdn_info;  last naming attribute */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                  Long            *oct_idx;   internal index of the */
/*                                              object class in the   */
/*                                              OCT                   */
/*                  D23_at_type    **at_str;    attribute structure   */
/*                                              pointers to attribute */
/*                                              tag                   */
/*                  D26_attr_info   *at_info;   attribute information */
/*                                              (index in the AT)     */
/*                                              in case of "at_str->  */
/*                  Bool            *recurring; a flag indicating     */
/*                                              whether (real)        */
/*                                              recurring attributes  */
/*                                              have to be inserted   */
/*                  D26_spec_attr  *spec_attr;  indicates whether     */
/*                                              the special attributes*/
/*                                              are found             */
/*                                              ( != D2_NULL)         */
/*      for ACl-attribute:                                            */
/*                  D26_dns        **dnl_ptr;   pointers that are set */
/*                                              (pointing to "dns")   */
/*                  D26_dns         *dns;       filled structure of   */
/*                                              ACL information       */
/**********************************************************************/
/*                  Short          *aliased_idx;index of aliased      */
/*                                              objetc attribute in   */
/*                                              AT                    */
/*                  Long           *alias_class;index of object class */
/*                                              ALIAS in OCT          */
/*                  D23_rserror     *error;     error message         */
/*                                              (contains the wrong   */
/*                                              attribute)            */
/*                  Int             *length;    length of error       */
/*                                              message               */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      any error occurred                */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_at     *d26_at;         Attribute-Table       */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*      d26_u03_attr_search()                                         */
/*      d26_u10_syntax_check()                                        */
/*      d26_u15_attr_error()                                          */
/*      d26_u23_get_acl_names()                                       */
/*      d26_703_mandatory_attr_check()                                */
/*      d26_u99_handle_blanks()                                       */
/*      d27_010_set_error_apdu()                                      */
/*      d27_012_remove_blanks()                                       */
/*      calloc()                                                      */
/*      free()                                                        */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static signed16 d26_704_attr_info_check(
				    signed16        srt_idx,
				    char           *obj_name,
				    D23_at_type   **at_str,
				    signed16        anz_rec[],
				    signed32       *oct_idx,
				    signed32       *sup_oct_idx,
				    D26_attr_info  *at_info,
				    boolean        *recurring,
				    D26_spec_attr  *spec_attr,
				    D26_dns       **dnl_ptr,
				    D26_dns        *dns,
				    D26_acl        *acl_idx,
				    D26_rdninfo    *rdn_info,
				    signed16       *aliased_idx,
				    signed32       *alias_class,
				    D23_rserror    *error,
				    signed32       *length
				   )

{                                   /*  d26_704_attr_info_check()     */

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 const char function[] = "d26_704_attr_info_check";
 signed16           i,j;

 D2_obj_id       obj_class;
 signed16           ocl_idx;       /* index of object class in OCT   */
 D26_ocl_arc    *ocl_arc;	    /* pointer to entry of OC arc     */
 boolean            obj_attr;      /* flag indicating whether actual */
				    /* attribute is OBJECT_CLASS      */
 signed32            act_oct_idx;
				    /* pointer to AT for each attribute */
				    /* in attribute queue             */
 D26_at         *attr_at[D2_AT_MAX + 1];

 signed16          *sav_anz_rec = &anz_rec[0];
 D23_at_type   **sav_at_str = at_str;
 D2_a_type       at_type;
 D23_av_info    *av_info;
 D23_av_value   *av_val;
 signed16        at_len;        /* attribute length               */
 unsigned16      rep;           /* attribute representation       */
 char           *val;           /* pointer to attribute value     */

 signed16       *rec_no;        /* pointer to list of shorts      */
				    /* indicating the total number of */
				    /* occurrencies of an attribute   */
				    /* (this array is parallel to AT) */
 signed16       *p_rec_no;

 signed32        l_err = 0;     /* length of error (attr_error)   */

 signed16        idx;           /* index of attribute in AT       */
 signed16        nm;            /* flag for naming attr.          */
 boolean         ocl_in_srt;    /* flag for ocl-arc-check         */

register D26_at     *at;            /* pointer to AT                  */
register char       *p_mess;        /* pointer to ADD-OBJECT message  */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

*recurring = FALSE;
spec_attr->d26_acl_attr = spec_attr->d26_mk_attr = spec_attr->d26_alias_attr
		       = spec_attr->d26_dnlis_attr = (D23_at_type *) NULL;
for                                 /*  all object classes            */
   (i = 0; i <= D26_OCL_VAL_COUNT; i++)
{                                   /*  initialize the object class   */
				    /*  array                         */
  oct_idx[i] = (signed32)D26_EOL;
}                                   /*                                */

*aliased_idx = -1;
*alias_class = -1L;

				    /*  get index of alias class in OCT */
obj_class.d2_typ_len = sizeof(D2_OALIAS) - 1;
obj_class.d2_type    = (Ob_id_string)D2_OALIAS;
*alias_class = (signed32)d26_u14_get_obj_class_idx(&obj_class);

if                                  /*  memory allocation fails       */
   ((rec_no = (signed16 *)D27_CALLOC(svc_c_sev_warning,(unsigned)d26_noat, 
     sizeof(signed16))) == 0)
{                                   /*  return(D26_ERROR)             */
  d27_010_set_error_apdu(error,D2_TEMP_ERR,D2_REM_ERR,D2_UNWILLING,length);
  return(D26_ERROR);
}                                   /*                                */

for                                 /*  all tags in message           */
    (i=0, p_mess = (char *) *at_str;
     i <= D2_AT_MAX && ((D23_at_type *)p_mess)->d23_atttag != D23_EOMTAG;
     i++, at_str++, anz_rec++)
{                                   /*                                */
  if                                /*  tag != attribute info tag     */
     (((D23_at_type *)p_mess)->d23_atttag != D23_ATTTAG)
  {                                 /*  return (D26_ERROR)            */
    d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR, D2_NOT_SPEC,
			   length);
    free((char *) rec_no);
    return(D26_ERROR);
  } else {                              /*  store pointer to attr-info    */
    *at_str = (D23_at_type *) p_mess;
    val = (char *) *at_str;
    av_info = (D23_av_info *) (val + D23_S_ATT +
			       ((D23_at_type *) val)->d23_atoff);
    av_val = (D23_av_value *) ((char *) av_info + D23_S_AV);
    val = (char *) av_val + D23_S_ATV;
    if                              /* no value following               */
       ((unsigned16) (av_val->d23_avlen) >= D23_MINTAG)
    {                               /* return constraint violation error*/
      d26_u15_attr_error((byte **)&error,exact_name,1,
	    D2_CONSTRAINT_VIOLATION,&at_type,av_info->d23_asyntx,
	    av_val->d23_avrep,av_val->d23_avlen,(byte *) val,length,&l_err);
      *length = l_err;
      free((char *) rec_no);
      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 11));
      return(D26_ERROR);
    }

    at_type.d2_typ_len = (*at_str)->d23_atlen;
    at_type.d2_type = (Ob_id_string)(*at_str + 1);
				    /*  look for attribute index in AT  */
    if                              /*  attribute was not found         */
       ((idx = at_info->d26_at_idx = d26_u03_attr_search(&at_type)) < 0)
    {                               /*  return value = D26_ERROR      */
      d26_u15_attr_error((byte **) &error, exact_name, 1,
			 D2_UNDEFINED_ATTR_TYPE, &at_type,
			 av_info->d23_asyntx, av_val->d23_avrep,
			 av_val->d23_avlen, (byte *) val, length, &l_err);
      *length = l_err;
      free((char *) rec_no);
      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 12));
      return(D26_ERROR);
    }
    attr_at[i] = at = d26_at + idx;

    if                              /* value couldn't be decoded        */
      (at->d26a_rep == D2_T61_PR &&
       av_val->d23_avrep != D2_T61 && av_val->d23_avrep != D2_PRINTABLE ||
       at->d26a_rep != D2_T61_PR &&
       at->d26a_rep != av_val->d23_avrep)
    {                               /* return syntax error              */
      d26_u15_attr_error((byte **) &error, exact_name, 1,D2_ATSYNTAX_ERROR,
			 &at_type, at->d26a_rep, av_val->d23_avrep,
			 av_val->d23_avlen, (byte *) val, length, &l_err);
      *length = l_err;
      free((char *) rec_no);
      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 13));
      return(D26_ERROR);
    } else {
      p_rec_no = rec_no + idx;
      (*p_rec_no)++;
				    /*  eliminate blanks              */
      at_len = av_val->d23_avlen = d26_u99_handle_blanks(at, val,
							 av_val->d23_avlen);

      obj_attr = FALSE;
      if                            /*  attribute = ACL               */
	 (!D2_ATTCMP(&at_type, D2_ACL))
      {                             /*  store attribute pointer       */
	spec_attr->d26_acl_attr = *at_str;
	if                          /*  ACL-names are not correct     */
	   (d26_u23_get_acl_names((D2_name_string) val,
				  av_val->d23_avlen, dnl_ptr, dns, acl_idx))
	{                           /*  return value = D26_ERROR      */
	  d26_u15_attr_error((byte **) &error, exact_name, 1,
			     D2_ATSYNTAX_ERROR, &at_type, at->d26a_rep,
			     av_val->d23_avrep,av_val->d23_avlen, (byte *) val,
			     length, &l_err);
	  *length = l_err;
	  free((char *) rec_no);
   	  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 14));
	  return(D26_ERROR);
	}                           /*                                */
      } else if                         /*  attribute = MASTER_KNOWLEDGE  */
	    (!D2_ATTCMP(&at_type, D2_MASTER_KNOWLEDGE))
      {                             /*  store attribute pointer       */
	spec_attr->d26_mk_attr = *at_str;
      } else if                         /*  attribute = ALIASED_OBJECT    */
	    (!D2_ATTCMP(&at_type, D2_ALIASED_OBJECT))
      {                             /*  store attribute pointer       */
	spec_attr->d26_alias_attr = *at_str;
	*aliased_idx = idx;
      } else if                         /*  attribute = DN-LIST           */
	    (!D2_ATTCMP(&at_type, D2_DNLIST))
      {                             /*  store attribute pointer       */
	spec_attr->d26_dnlis_attr = *at_str;
      } else if                         /*  attribute is OBJECT-CLASS     */
	    (!D2_ATTCMP(&at_type, D2_OBJ_CLASS))
      {                             /*  mark OBJECT_CLASS attribute   */
	obj_attr = TRUE;
      }                             /*                                */

				    /*  enter attr. index in internal array*/

      for                           /*  all attribute occurrencies    */
	  (*anz_rec = 1; ; )
      {                             /*  eliminate blanks              */
	if                          /*  length not correct            */
	   (d26_u96_attr_len_check(at, at_len) == D26_ERROR)
	{                           /*  return value = D26_ERROR      */
	  d26_u15_attr_error((byte **) &error, exact_name, 1,
			     D2_CONSTRAINT_VIOLATION, &at_type,
			     at->d26a_rep, av_val->d23_avrep, at_len, 
			     (byte *) val, length, &l_err);
	  *length = l_err;
	  free((char *) rec_no);
   	  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 15));
	  return(D26_ERROR);
	} else if                   /*  attribute syntax is not correct */
	      (d26_u10_syntax_check(at, av_val->d23_avrep, (byte *) val, 
	      at_len))
	{                           /*  return value = D26_ERROR        */
	  d26_u15_attr_error((byte **) &error, exact_name, 1,
			     D2_ATSYNTAX_ERROR, &at_type, at->d26a_rep,
			     av_val->d23_avrep, at_len, (byte *) val, length, 
			     &l_err);
	  *length = l_err;
	  free((char *) rec_no);
   	  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 16));
	  return(D26_ERROR);
	} else if                   /*  attribute is OBJECT_CLASS     */
	      (obj_attr == TRUE)
	{                           /*  look for OBJECT_CLASS in OCT  */
	  obj_class.d2_typ_len = av_val->d23_avlen;
	  obj_class.d2_type    = (Ob_id_string) val;
	  if                        /*  OBJECT_CLASS not in schema    */
	     ((act_oct_idx = d26_u14_get_obj_class_idx(&obj_class))
			   == D26_ERROR)
	  {                         /*  generate update error "object */
				    /* class violation                */
	    d27_010_set_error_apdu(error,D2_CALL_ERR,D2_PAR_ERR,
				   D2_OBJ_CLS_VIOLATION,length);
	    free((char *) rec_no);
   	    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 17));
	    return(D26_ERROR);
	  } else {                      /*  check the object class for    */
				    /*  consistency with the other    */
				    /*  object class values           */
	    if (d26_712_get_correct_obj_class(oct_idx,act_oct_idx)
		== D26_ERROR)
	    {                       /*  generate attribute error      */
	      d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR,
				     D2_OBJ_CLS_VIOLATION, length);
   	      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 18));
	      return(D26_ERROR);
	    }                       /*                                */
	  }                         /*                                */
	}                           /*                                */

	av_val = (D23_av_value *) (val + av_val->d23_avoff);
				    /*  look for next occurrency      */
	if                          /*  attribute occurrency found    */
	   ((unsigned16) (at_len = av_val->d23_avlen) < D23_MINTAG )
	{                           /*  take next occurrency          */
	  if (obj_attr == FALSE)
	  {   
	    (*anz_rec)++;
	    (*p_rec_no)++;
	  } 
	  val = (char *) av_val + D23_S_ATV;
	  at_len = av_val->d23_avlen =
		   d26_u99_handle_blanks(at, val, at_len);
	} else {                        /*  take new attribute            */
				    /*  break                         */
	  p_mess = (char *) av_val;
	  break;
	}                           /*                                */
      }                             /*                                */
      if			    /*  object class attribute was    */
				    /*  processed		      */
	(obj_attr == TRUE)
      {                             /*  check consistency of object   */
				    /*  classes                       */
	if                          /*  object classes are incons.    */
	  ((*p_rec_no = *anz_rec = d26_714_check_obcl_consistency(oct_idx))
	   == D26_ERROR)
	{                           /*  generate an update error      */
	  d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR,
				 D2_OBJ_CLS_VIOLATION, length);
   	  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 19));
	  return(D26_ERROR);
	}

	if			    /* scheme object should be added  */
	  (!D2_ATTCMP(&(d26_oct[oct_idx[0]].d26c_ob_id), D2_OSCHEMA))
	{
	  d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR,
				 D2_UNWILLING, length);
   	  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 191));
	  return(D26_ERROR);
	}
      }

      if                            /*  AT-attr. is a (real) recurring one*/
	 (at->d26a_filenr >= 0)
      {                             /* recurring-flag = D2_TRUE        */
	*recurring = TRUE;
      } else {                      /* if attr. is naming increment    */
				    /* the count of recurring values   */
	for (j=0,nm=0; d26_srt[srt_idx].d26s_nam_at[j].d26s_index > -1; j++)
	{
	  if (d26_srt[srt_idx].d26s_nam_at[j].d26s_index == idx) nm=1;
	}
	if                          /* number of occurencies > maximum */
	   (obj_attr == FALSE && (*p_rec_no + nm) > at->d26a_nr_recur)
	{                             /*  return(D26_ERROR)             */
	   d26_u15_attr_error((byte **) &error, exact_name, 1,
			      D2_CONSTRAINT_VIOLATION, &at_type,
			      at->d26a_rep, av_val->d23_avrep, at_len, 
			      (byte *) val, length, &l_err);
	   *length = l_err;
	   free((char *) rec_no);
   	   DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 20));
	   return(D26_ERROR);
	}
      }
    }
    at_info++;
  }
} 

at_info->d26_at_idx = D26_EOL;      /* end of list                    */

if                                  /*  object class not in attr. list */
   (*oct_idx == -1L)
{                                   /*  return(D26_ERROR)             */
  d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR,
			 D2_OBJ_CLS_VIOLATION, length);
  free((char *) rec_no);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 21));
  return(D26_ERROR);
} else if                               /*  object class is not ALIAS     */
      (*oct_idx != *alias_class)
{                                   /*  check whether object class    */
				    /*  belongs to SRT entry          */
  for (ocl_arc = (d26_srt + srt_idx)->d26s_ocl_arc, ocl_in_srt = FALSE;
                 (ocl_idx = ocl_arc->d26s_oclass) >= 0; ocl_arc++)
  { 
    if ((signed32)ocl_idx == *oct_idx)
    {
      ocl_in_srt = TRUE;         /*  check validity of sup. OCL    */
      if ((signed32)ocl_arc->d26s_supocl == *sup_oct_idx)
      {
	break;
      }
    }
  } 
  if (!ocl_in_srt)
  {                                 /*  generate update error         */
    d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR,
			   D2_NAMING_VIOLATION, length);
    free((char *) rec_no);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 22));
    return(D26_ERROR);
  } else if
    (ocl_in_srt &&
     (((signed32)ocl_idx == *oct_idx &&
      (signed32)ocl_arc->d26s_supocl != *sup_oct_idx && ocl_in_srt) ||
     (signed32)ocl_idx != *oct_idx))
  {                                 /*  generate update error         */
    d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR,
			   D2_OBJ_CLS_VIOLATION, length);
    free((char *) rec_no);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 23));
    return(D26_ERROR);
  }                                 /*                                */
}                                   /*                                */

if                                  /*  number of attr-infos > maximum*/
   ((i == D2_AT_MAX+1) && ((D23_at_type *)p_mess)->d23_atttag != D23_EOMTAG)
{                                   /*  return(D26_ERROR)             */
  d27_010_set_error_apdu(error,D2_CALL_ERR,D2_PAR_ERR,D2_ADM_LIM_EXCEEDED,
			 length);
  free((char *)rec_no);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 24));
  return(D26_ERROR);
} else if                           /*  attributes doesn't belong to object*/
     (d26_711_check_attr(rec_no,oct_idx,&idx) == D26_ERROR)
{                                   /*  return value = D26_ERROR      */
  at_type = (d26_at + idx)->d26a_type;
  d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR,
			 D2_OBJ_CLS_VIOLATION, length);
  free((char *) rec_no);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 25));
  return(D26_ERROR);
} else if                           /*  mandatory attributes missing  */
      ((signed32) d26_703_mandatory_attr_check(rec_no,oct_idx,
				    (d26_srt + srt_idx)->d26s_nam_at, &idx))
{                                   /*  return value = D26_ERROR      */
  at_type = (d26_at + idx)->d26a_type;
  d27_010_set_error_apdu(error, D2_CALL_ERR, D2_PAR_ERR,
			 D2_OBJ_CLS_VIOLATION, length);
  free((char *) rec_no);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 26));
  return(D26_ERROR);
} else {
  *at_str = (D23_at_type *) NULL;
  if                                /*  attribute values are not distinct */
     (d26_709_attr_val_check(sav_at_str, sav_anz_rec, &attr_at[0], rdn_info,
	     &at_type, (signed32 *) &at_len, (byte  **) &val, &rep) == D26_ERROR)
  {                                 /*  return value = D26_ERROR      */
    d26_u15_attr_error((byte **) &error, exact_name, 1, D2_AT_VALUE_EXISTS,
		       &at_type, attr_at[0]->d26a_rep, rep,
		       at_len, (byte *) val, length, &l_err);
    *length = l_err;
    free((char *) rec_no);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 27));
    return(D26_ERROR);
  }
}

				    /*  return(D2_NO_ERR);            */
free((char *) rec_no);
DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NO_ERR));
return(D2_NO_ERR);

}                                   /*  d26_704_attr_info_check()     */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_705_get_master_dsa_idx(...)                     */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 14.01.88                                            */
/*								      */
/* SYNTAX       : Long  d26_705_get_master_dsa_idx(mk_attr, dns)      */
/*                D23_at_type    *mk_attr;                            */
/*                D26_dns        *dns;                                */
/*                                                                    */
/* DESCRIPTION  : if the MASTER-KNOWLEDGE attribute is missing, the   */
/*                DSA is master of this object.                       */
/*                Otherwise D26_IDX_NOT_DEF will be returned.         */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                D23_at_type   *mk_attr;   pointer to master know-   */
/*                                          ledge attribute           */
/*                D26_dns       *dns;       pointer to memory for     */
/*                                          Master DSA information    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                D26_dns       *dns;       Master DSA information    */
/*								      */
/* RETURN VALUE :  =  D26_OWNNAME           if master DSA is own DSA  */
/*                 =  D26_IDX_NOT_DEF       if master DSA is any DSA  */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  char    d26_myname[];   own DSA-name              */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d27_007_norm_name()                               */
/*                  d27_012_remove_blanks()                           */
/*                  strcmp()                                          */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  signed32  d26_705_get_master_dsa_idx
(
  D23_at_type    *mk_attr,
  D26_dns        *dns
)

{                                   /*  d26_705_get_master_dsa_idx()  */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 signed16        len;
 char         *mk;
 D23_av_value *av_val;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

dns->d26_dno = 1;
if                                  /*  master knowledge attr. is absent */
   (mk_attr == (D23_at_type *) NULL)
{                                   /*  return(D26_OWNNAME)           */
  return( *(dns->d26_didx) = D26_OWNNAME);
} else {                                /*  norm master knowledge attribute */
  av_val = (D23_av_value *) ((char *) mk_attr + D23_S_ATT +
			     mk_attr->d23_atoff + D23_S_AV);
  mk = (char *) av_val + D23_S_ATV;
  len = d27_012_remove_blanks(mk, av_val->d23_avlen - 1, mk);
  strcpy(dns->d26_edname, mk);      /*  exact name for DN-List        */
  d27_007_norm_name(mk, len, mk);
  *(mk + len) = D2_EOS;

  if                                /*  master DSA = own DSA          */
     (!strcmp(mk, (char *) d26_myname))
  {                                 /*  return(D26_OWNNAME)           */
    return( *(dns->d26_didx) = D26_OWNNAME );
  } else {                          /* return(master = D26_IDX_NOT_DEF)*/
    dns->d26_dint = D2_SOBJ;
    dns->d26_dname  = (byte  *) mk;
    return( *(dns->d26_didx) = D26_IDX_NOT_DEF );
  }
}

}                                   /*  d26_705_get_master_dsa_idx()  */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_706_update_dnl()                                */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 26.01.88                                            */
/*								      */
/* SYNTAX       : Short d26_706_update_dnl(no_dnl_entries,dnl_ptr)    */
/*                Short     no_dnl_entries;                           */
/*                D26_dns **dnl_ptr;                                  */
/*								      */
/* DESCRIPTION  : This function updates the DNL.                      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                Short     no_dnl_entries; number of entries to be   */
/*                                          updated                   */
/*                D26_dns **dnl_ptr;        pointer list to entries   */
/*                                          that contain information  */
/*                                          for DNL update            */
/*                                                                    */
/* OUTPUT-PARAMETERS: -                                               */
/*								      */
/* RETURNVALUE     :    D2_NO_ERR       update was ok                 */
/*                      D26_ERROR       update fails                  */
/*								      */
/* STANDARD-HEADER: -                                                 */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  -                                                 */
/*								      */
/* GLOBAL DATA (updated) :                                            */
/*                  -                                                 */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          d26_i02_upd_file_dnlist()                 */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  signed16 d26_706_update_dnl (signed16      no_dnl_entries,
				D26_dns **dnl_ptr
			       )


{                                   /*  d26_706_update_dnl()          */

/**********************************************************************/
/*								      */
/*		   T Y P  -  D E F I N I T I O N E N		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

register signed16       i;
register D26_dns    *p_dns;
     boolean        time_stamp = FALSE;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for                                 /*  all entries to be updated     */
    (i = 0; i < no_dnl_entries; dnl_ptr++)
{                                   /*                                */
  p_dns = *dnl_ptr;
  if (++i == no_dnl_entries)
  {   
    time_stamp = TRUE;
  } 
  if                                /*  update not successful         */
    (d26_i02_upd_file_dnlist(D2_ADD_AV, (D2_name_string) NULL,
			     p_dns->d26_dint,
			     (D2_name_string) p_dns->d26_edname,
			     p_dns->d26_dno, time_stamp, p_dns->d26_didx))
  {                                 /*  return(D26_ERROR)             */
    return(D26_ERROR);
  }
}

				    /*  return(D2_NO_ERR)             */
return(D2_NO_ERR);

}                                   /*  d26_706_update_dnl()          */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_707_handle_alias(...)                           */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 08.02.88                                            */
/*								      */
/* SYNTAX       : Short d26_707_handle_alias(a_out, srt_idx, dn_info, */
/*                                           alias_attr, attr,        */
/*                                           aliased_idx,             */
/*                                           alias_class, length)     */
/*                char            **a_out;                             */
/*                Short            srt_idx;                           */
/*                D26_dninfo      *dn_info;                           */
/*                D23_at_type     *alias_attr;                        */
/*                D26_attr_info   *attr;                              */
/*                Short            aliased_idx;                       */
/*                Long             alias_class;                       */
/*                Int             *length;                            */
/*								      */
/* DESCRIPTION  : This function will create the alias object.         */
/*                At first it will generate the index of the aliased  */
/*                object in the DNLIST.                               */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Char            **a_out;   pointer to ADD-OBJECT     */
/*                                          result message            */
/*                Short            srt_idx; index of SRT entry in SRT */
/*                D26_dninfo      *dn_info; Dn-info of alias object   */
/*                D23_at_type     *alias_attr;  pointer to aliased    */
/*                                          object name in ADD-OBJECT */
/*                                          invoke message            */
/*                D26_attr_info   *attr;    attributes of alias       */
/*                Short            aliased_idx; index of aliased      */
/*                                          object attribute in AT    */
/*                Long             alias_class; index of object class */
/*                                          ALIAS in OCT              */
/*                Int             *length;  maximum length of result  */
/*                                          (or error) message        */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                Int             *length;  actual length of message  */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      any error occurred                */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_oct     *d26_oct;   pointer to OCT            */
/*                  D26_at      *d26_at;    pointer to AT             */
/* GLOBAL DATA (updated):                                             */
/*                  d26_ma_info                                       */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d26_u06_get_name()                                */
/*                  d26_701_search_params()                           */
/*                  d26_754_create_alias()                            */
/*                  d26_i14_unlock()                                  */
/*                  d27_010_set_error_apdu()                          */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  signed16 d26_707_handle_alias(byte            **a_out,
				 signed16            srt_idx,
				 D26_dninfo      *dn_info,
				 D23_at_type     *alias_attr,
				 D26_attr_info   *attr,
				 signed16            aliased_idx,
				 signed32             alias_class,
				 signed32             *length
				)

{                                   /*  d26_707_handle_alias()        */

/**********************************************************************/
/*								      */
/*		   T Y P  -  D E F I N I T I O N E N		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

 static char function[] = "d26_707_handle_alias";
 signed16         ret_value;
 signed32         ald_idx = D26_IDX_NOT_DEF;
				/* index of aliased object in     */
				/* DNLIST                         */
 char            *p_aliatt;     /* pointer to attribute value     */
 signed16         l_ali;        /* length of aliased object name  */
 byte            *p;            /* pointer to APDU     */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

p_aliatt = (char *) alias_attr;
p_aliatt += D23_S_ATT + alias_attr->d23_atoff + D23_S_AV;
l_ali = ((D23_av_value *) p_aliatt)->d23_avlen;
p_aliatt += D23_S_ATV;

				    /*  remove blanks in aliased object */
l_ali = d27_012_remove_blanks(p_aliatt, l_ali, p_aliatt);
				    /*  get index of aliased object in */
				    /*  DNLIST                         */
if                                  /*  update not successful         */
  (d26_i02_upd_file_dnlist(D2_ADD_AV, (D2_name_string) NULL, D2_SOBJ,
			   (D2_name_string) p_aliatt, 1L, TRUE, &ald_idx))
{                                   /*  ret_value = D26_ERROR         */
  ret_value = D26_ERROR;
} else {                                /*  create alias object           */
  if                                /*  creation was not successful   */
     ((ret_value = d26_754_create_alias(srt_idx, dn_info, attr, aliased_idx,
					alias_class, ald_idx)) != D2_NOERROR)
  {                                 /*  set error message             */
    d27_010_set_error_apdu((D23_rserror *) *a_out, D2_TEMP_ERR, D2_REM_ERR,
			   D2_UNWILLING, length);
  } else {                              /*  generate result message (OK)  */
    *length = 0;
    p = *a_out;
    if                    /*  DSP was coming in                         */
       (d26_chain_arg != (D23_chain_arg *)NULL)
    {                     /*  write chaining result                     */
     ((D23_chain_res *) p)->d23_chrtag  = D23_CHRTAG;
     p += D23_S_CHRES;
     *length += D23_S_CHRES;
    }                     /*                                            */

    ((D23_eom *) p)->d23_eomtag = D23_EOMTAG;
    *length = D23_S_EOM;
  }                                 /*                                */
}                                   /*                                */
				    /*  unlock used files             */
d26_i14_unlock();

				    /*  return(ret_value)             */
DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR));
return(ret_value);

}                                   /*  d26_707_handle_alias()        */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_709_attr_val_check()                            */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 10.04.91                                            */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : This function will look whether all attribute values*/
/*                for an attribute are disjunct or not.               */
/*                The naming attribute in the last RDN has to be      */
/*                different too.                                      */
/*                if at least two values are the same, it will return */
/*                the errorneous attribute.                           */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                D23_at_type **at_types;   attribute queue in IPC    */
/*                                          message format            */
/*                Short        *rec_no;     number of recurring       */
/*                                          values for each attribute */
/*                D26_at      **at;         pointers to At for each   */
/*                                          attribute                 */
/*                D26_rdninfo  *rdn;        information about last    */
/*                                          RDN                       */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                D26_at        at[0];      AT information for        */
/*                                          errorneous attribute      */
/*                D2_a_type    *err_type;   errorneous attribute type */
/*                Int          *err_len;    attribute length          */
/*                byte        **err_val;    attribute length          */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR       all attributes are disjunct       */
/*                  D26_ERROR       at least two attribute values     */
/*                                  are the same                      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static  signed16 d26_709_attr_val_check
(
  D23_at_type **at_types,
  signed16     *rec_no,
  D26_at      **at,    
  D26_rdninfo  *rdn,    
  D2_a_type    *err_type,
  signed32     *err_len,
  byte        **err_val,
  unsigned16   *rep
)

{
D23_at_type  **attr1, **attr2;
Octet_string   val1, val2;
signed16       len1,         len2;
signed32       no1,          no2;
D23_av_value  *av1_val,     *av2_val;
signed16      *rec1_no,     *rec2_no;
D26_at       **tmp1_at,    **tmp2_at;
D26_at        *nam_at;
D26_ava       *p_ava;          /* pointer to ava in rdn          */
signed16       i_ava;          /* index of ava in d26_r_ava[]    */
D23_av_info   *av_info;
D23_av_value  *sav_val;

			    /* look for multiple values for same attribute */
for (attr1 = at_types, rec1_no = rec_no, tmp1_at = at;
     *attr1 != (D23_at_type *) 0; attr1++, rec1_no++, tmp1_at++)
{   av_info = (D23_av_info *) ((Octet_string) *attr1 + D23_S_ATT +
			       (*attr1)->d23_atoff);
    av1_val = av2_val = (D23_av_value *) ((char *) av_info + D23_S_AV);

    for (no1 = 1; ; )
    {   val1 = val2 = (Octet_string) av1_val + D23_S_ATV;
	len1 = av1_val->d23_avlen;

        for (i_ava = 0, p_ava = rdn->d26_r_ava; i_ava < rdn->d26_r_nava; 
                                           i_ava++, p_ava++)
        {
	    if ((nam_at = d26_at + p_ava->d26_a_idx) == *tmp1_at)
	    {   
                 if (d26_u98_attr_val_cmp(val1, len1, p_ava->d26_a_val,
				         p_ava->d26_a_len,
				         nam_at->d26a_syntax) == TRUE)
	        {   err_type->d2_typ_len = (*attr1)->d23_atlen;
		    err_type->d2_type = (Ob_id_string)(*attr1 + 1);
		    *at = *tmp1_at;
		    *err_len = (signed32) len1;
		    *err_val = val1;
		    *rep = av1_val->d23_avrep;
		    return(D26_ERROR);
	        }
                
	    }
	}

	for (no2 = ++no1; no2 <= *rec1_no; no2++)
	{                   /* take next recurring value of same attribute */
	    av2_val = (D23_av_value *) (val2 + av2_val->d23_avoff);
	    val2 = (Octet_string) av2_val + D23_S_ATV;
	    len2 = av2_val->d23_avlen;
	    if (d26_u98_attr_val_cmp(val1, len1, val2, len2,
				     (*tmp1_at)->d26a_syntax) == TRUE)
	    {   err_type->d2_typ_len = (*attr1)->d23_atlen;
		err_type->d2_type = (Ob_id_string)(*attr1 + 1);
		*at = *tmp1_at;
		*err_len = (signed32) len2;
		*err_val = val2;
		*rep = av2_val->d23_avrep;
		return(D26_ERROR);
	    }
	}

	if (no1 > *rec1_no)
	{   break;
	}
	else
	{                   /* multiple values available */
	    av1_val = av2_val = (D23_av_value *) (val1 + av1_val->d23_avoff);
	}
    }
}

			    /* look for multiple values for attribute,
			     * which are at different positions in the
			     * attribute queue
			     */
for (attr1 = at_types, rec1_no = rec_no, tmp1_at = at;
     *attr1 != (D23_at_type *) 0; attr1++, rec1_no++, tmp1_at++)
{                           /* take first value of actual attribute */
    av_info = (D23_av_info *) ((Octet_string) *attr1 + D23_S_ATT +
			       (*attr1)->d23_atoff);
    sav_val = av1_val = (D23_av_value *) ((char *) av_info + D23_S_AV);

			    /* take all attributes in the remaining queue */
    for (attr2 = attr1 + 1, rec2_no = rec1_no + 1, tmp2_at = tmp1_at + 1;
	 *attr2 != (D23_at_type *) 0; attr2++, rec2_no++, tmp2_at++)
    {   if (*tmp1_at == *tmp2_at)
	{                   /* attribute types are the same;
			     * take first attribute value to be compared
			     * with
			     */
	    val1 = (Octet_string) sav_val + D23_S_ATV;
	    len1 = sav_val->d23_avlen;
	    av_info = (D23_av_info *) ((Octet_string) *attr2 + D23_S_ATT +
				       (*attr2)->d23_atoff);

	    for (no1 = 0; ; )
	    {   av2_val = (D23_av_value *) ((char *) av_info + D23_S_AV);
		val2 = (Octet_string) av2_val + D23_S_ATV;
		len2 = av2_val->d23_avlen;
		for (no2 = 0; ; )
		{   if (d26_u98_attr_val_cmp(val1, len1, val2, len2,
					     (*tmp1_at)->d26a_syntax)
					     == TRUE)
		    {   err_type->d2_typ_len = (*attr1)->d23_atlen;
			err_type->d2_type = (Ob_id_string)(*attr1 + 1);
			*at = *tmp1_at;
			*err_len = (signed32) len2;
			*err_val = val2;
			*rep = av2_val->d23_avrep;
			return(D26_ERROR);
		    }
		    if (++no2 == *rec2_no)
		    {       /* last recurring value reached */
			break;
		    }
		    else
		    {       /* take next recurring value */
			av2_val = (D23_av_value *)
					  (val2 + av2_val->d23_avoff);
			val2 = (Octet_string) av2_val + D23_S_ATV;
			len2 = av2_val->d23_avlen;
		    }
		}
		if (++no1 == *rec1_no)
		{           /* last recurring value reached */
		    break;
		}
		else
		{           /* take next recurring value from remaining
			     * queue for this attribute type
			     */
		    av1_val = (D23_av_value *) (val1 + av1_val->d23_avoff);
		    val1 = (Octet_string) av1_val + D23_S_ATV;
		    len1 = av1_val->d23_avlen;
		}
	    }
	}
    }
}

return(D2_NO_ERR);
}

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_710_is_subclass()                               */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 08.04.92                                            */
/*                                                                    */
/* DESCRIPTION  : This function checks whether ob_cl1 is a subclass   */
/*                of ob_cl2. Ob_cl1 and Ob_cl2 are the indices of OCT */
/*                entries.                                            */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      ob_cl1      Long        OCT index of first object class       */
/*      ob_cl2      Long        OCT index of second object class      */
/*                                                                    */
/* OUTPUT PARAMETERS: -                                               */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_TRUE     Bool        ob_cl1 is subclass of ob_cl2          */
/*      D2_FALSE                ob_cl1 is not a subclass of ob_cl2    */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*      d26_oct     D26_oct *   Object Class Table                    */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static  boolean d26_710_is_subclass
(
  signed32 ob_cl1,
  signed32 ob_cl2
)

{                                   /*  d26_710_is_subclass           */

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

 signed16 *sup_oct;        /* pointer to superclass index            */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if                                  /*  object class 2 not available  */
   (ob_cl2 < 0)
{                                   /*  return TRUE                   */
  return(TRUE);
} else {                                /*  check all direct superclasses */
				    /*  of first object class         */

  for 				    /*  all direct superclasses       */
				    /*  of first object class         */
     (sup_oct = d26_oct[ob_cl1].d26c_sup_cl; *sup_oct >= 0L; sup_oct++)
  { 				    /*  compare with object class 2   */
    if 				    /*  object class2 is direct       */
				    /*  superclass		      */
      ((signed32)*sup_oct == ob_cl2)
    {   			    /*  return TRUE		      */
      return(TRUE);
    } else if			    /*  direct superclass is subclass */
				    /*  of second object class	      */
	 (d26_710_is_subclass((signed32)*sup_oct,ob_cl2) == TRUE)
    {   			    /*  return TRUE		      */
      return(TRUE);
    } 				    /* 				      */
  } 
				    /*  second object class is not a  */
				    /*  superclass		      */
  return(FALSE);

} 				    /* 				      */

}                                   /*  d26_710_is_subclass           */

/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_711_check_attr(...)                             */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 15.01.90                                            */
/*								      */
/* SYNTAX       : Int d26_711_check_attr(rec_no, oct_idx, err_idx)    */
/*                Short *rec_no;                                      */
/*                Long  *oct_idx;                                     */
/*                Short *err_idx;                                     */
/*                                                                    */
/* DESCRIPTION  : This function will check whether the attributes     */
/*                in the attribute list belong to the new object.     */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Short *rec_no;        indicates wether an attribute */
/*                                      is given in the ADD-OBJECT    */
/*                                      invoke message                */
/*                                      (this array is "parallel" to  */
/*                                      the AT)                       */
/*                                      == 0: attribute missing       */
/*                                      >  0: number of occurrencies  */
/*                Long  *oct_idx;       object class array            */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                Short *err_idx;       index of attribute in AT      */
/*                                      that caused error             */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:          all attributes are ok         */
/*                  D26_ERROR:          at least one attribute doesn't*/
/*                                      belong to the object          */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_at     *d26_at;         Attribute-Table (AT)  */
/*                  Int         d26_noat;       number of entries in  */
/*                                              AT                    */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  signed32 d26_711_check_attr
(
  signed16   *rec_no,
  signed32    *oct_idx,
  signed16   *err_idx
)

{                                   /*  d26_711_check_attr()          */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 signed16          *act_rec_no;
 signed16          *max_rec_no;

 signed16           idx;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for                                 /*  all attribute in list         */
    (act_rec_no = rec_no, max_rec_no = rec_no + d26_noat, idx = 0;
     act_rec_no < max_rec_no; act_rec_no++, idx++)
{                                   /*                                */
  if                                /*  attribute is valid            */
     (*act_rec_no != 0)
  {                                 /*  check whether it is allowed   */
    if                              /*  attribute is not in object    */
				    /*  classes                       */
      (d26_u59_attr_in_obj_class(oct_idx,idx) == FALSE)
    {                               /*  return errorneous attribute   */
      *err_idx = idx;
      return(D26_ERROR);
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */

				    /*  return(D2_NO_ERR)             */
return(D2_NO_ERR);

}                                   /*  d26_711_check_attr()          */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_712_get_correct_obj_class()                     */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 17.12.87                                            */
/*                                                                    */
/* SYNTAX       : d26_712_get_correct_obj_class(oct_idx,act_oct_idx)  */
/*                                                                    */
/*                Long *oct_idx;                                      */
/*                Long act_oct_idx;                                   */
/*                                                                    */
/* DESCRIPTION  : This check whether the act_oct_idx is consistent    */
/*                with the other ones, which are in the oct_idx array */
/*                already. if so it eventually enters act_oct_idx into*/
/*                this array, if it is not a superclass of one of the */
/*                others. The array is terminated by D26_EOL.         */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                Long  *oct_idx;                                     */
/*                Long  act_oct_idx;                                  */
/*                                                                    */
/* OUTPUT PARAMETERS: -                                               */
/*                Long  act_oct_idx;                                  */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   Short           actual object class is OK.        */
/*      D26_ERROR                   inconsistency detected.           */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  D26_oct     *d26_oct;       Object Class Table    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*      d26_710_is_subclass()                                         */
/*      d26_713_rem_superclasses()                                    */
/*      d27_301_oid_str()                                             */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static  signed16 d26_712_get_correct_obj_class
(
  signed32  *oct_idx,
  signed32   act_oct_idx
)

{                                   /*  d26_712_get_correct_obj_class */

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

 signed32 *ob_cl;	    /* loop pointer to object classes	      */
			    /* memory for printable object identifiers**/
 char ob_id1[D2_OBJ_IDL_MAX], ob_id2[D2_OBJ_IDL_MAX];

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

switch                              /*  check kind of actual object   */
				    /*  class                         */
      (d26_oct[act_oct_idx].d26c_kind)
{
  case  D2_ALIAS:                   /*  actual object class is a      */
  case  D2_STRUCTURAL:              /*  structural or alias one       */
    if				    /*  actual object class is a      */
				    /*  subclass of the one stored    */
				    /*  in first position	      */
      (d26_710_is_subclass(act_oct_idx,*oct_idx) == TRUE)
    {   			    /*  put it into first position    */
      *oct_idx = act_oct_idx;
      d26_713_rem_superclasses(oct_idx);
    } else if                           /*  actual object class is not a  */
				    /*  superclass                    */
	 (d26_710_is_subclass(*oct_idx,act_oct_idx) == FALSE)
    {                               /*  log and return error          */
      d27_301_oid_str(&d26_oct[*oct_idx].d26c_ob_id,ob_id1);
      d27_301_oid_str(&d26_oct[act_oct_idx].d26c_ob_id,ob_id2);
      return(D26_ERROR);
    } 				    /* 				      */
    break;
  case  D2_ABSTRACT:
    if                              /*  *oct_idx is not empty         */
       (*oct_idx != D26_EOL)
    {   
      if                            /*  actual object class is a      */
				    /*  superclass of structural      */
	(d26_710_is_subclass(*oct_idx,act_oct_idx) == TRUE)
      {                             /*  break                         */
	break;
      }                             /*                                */
    } 
				    /*  go into next case otherwise   */
  case  D2_AUXILIARY:		    /*  actual object class is        */
				    /*  abstract or auxiliary         */
    for				    /*  all auxiliary or abstract     */
				    /*  object classes		      */
       (ob_cl = oct_idx + 1; *ob_cl != D26_EOL; ob_cl++)
    { 				    /*  check whether the actual      */
				    /*  object class is a subclass    */
      if			    /*  actual object class is a      */
				    /*  subclass		      */
	(d26_710_is_subclass(act_oct_idx,*ob_cl) == TRUE)
      {   			    /*  put the actual object class   */
				    /*  into the array		      */
        *ob_cl = act_oct_idx;
        d26_713_rem_superclasses(ob_cl);
	break;
      } else if			    /*  actual object class is a      */
				    /*  superclass		      */
	   (d26_710_is_subclass(*ob_cl,act_oct_idx) == TRUE)
      {   			    /*  break loop		      */
	break;
      } 			    /* 				      */
    } 				    /* 				      */
    if                              /*  neither superclass nor        */
				    /*  subclass was found            */
      (*ob_cl == D26_EOL)
    {   			    /*  check space in array	      */
      if			    /*  last element is reached       */
	(ob_cl - oct_idx == D26_OCL_VAL_COUNT)
      {   			    /*  return error		      */
	return(D26_ERROR);
      } else {			    /*  append actual object class    */
				    /*  index			      */
        *ob_cl = act_oct_idx;
      } 			    /*  			      */ 
    } 				    /* 				      */
    break;
}   

				    /*  return no error		      */
return(D2_NO_ERR);

}                                   /*  d26_712_get_correct_obj_class */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_713_rem_superclasses                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 08.04.92                                            */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : This function checks for all abstract object classes*/
/*                which follow ob_cl, whether they are superclasses   */
/*                of *ob_cl. if so, they are removed from the array.  */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      ob_cl       Long *      pointer to an array of object classes */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      ob_cl       Long *      pointer to an array of object classes */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      None                                                          */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*      d26_710_is_subclass()                                         */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static  void d26_713_rem_superclasses
(
  signed32 *ob_cl
)

{                                   /*  d26_713_rem_superclasses      */

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

 signed32 *ob_clp1,*ob_clp2;        /* pointer to object class array  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

for                                 /*  all following object classes  */
   (ob_clp1 = ob_cl + 1; *ob_clp1 != D26_EOL; ob_clp1++)
{ 				    /*  check whether it is superclass*/
  if				    /*  object class is superclass of */
				    /*  the one under check	      */
    (d26_710_is_subclass(*ob_cl,*ob_clp1) == TRUE)
  {   				    /*  remove the abstract object    */
				    /*  class from the array	      */
    for				    /*  all following object classes  */
       (ob_clp2 = ob_clp1; *ob_clp2 != D26_EOL; ob_clp2++)
    { 				    /*  overwrite the object class    */
				    /*  with the following	      */
      *ob_clp2 = *(ob_clp2 + 1);
    } 				    /* 				      */
  } 				    /* 				      */
} 				    /* 				      */

}                                   /*  d26_713_rem_superclasses      */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_714_check_obcl_consistency                      */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 08.04.92                                            */
/*                                                                    */
/* DESCRIPTION  : This function checks whether the first object class */
/*                in the array is structural or alias, whether no     */
/*                more abstract object classes occur in the list and  */
/*                whether all other object classes in the list are    */
/*                auxiliary for the structural one.                   */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      ob_cl       Long *      pointer to an array of object classes */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      >0          Short       count of object classes               */
/*      D26_ERROR               inconsistency detected                */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*      d26_oct     D26_oct *   object class table                    */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*      d27_301_oid_str()                                             */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static  signed16 d26_714_check_obcl_consistency
(
  signed32 *ob_cl
)

{                                   /*  d26_714_check_obcl_consistency*/

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

signed16  *aux_idx;                 /* pointer to auxiliary object    */
				    /* class array                    */
			    /* memory for printable object identifiers*/
char ob_id1[D2_OBJ_IDL_MAX],ob_id2[D2_OBJ_IDL_MAX];

register signed32 *ob_clp;          /* pointer to object class array  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if                          /*  first element of array is not set     */
  (*ob_cl == D26_EOL)
{                           /*  generate update error                 */
  return(D26_ERROR);
} else {                        /*  scan the object classes               */
  for                       /*  all object classes in array           */
     (ob_clp = ob_cl + 1; *ob_clp != D26_EOL; ob_clp++)
  {                         /*  check whether it is abstract          */
    if                      /*  object class is abstract              */
      (d26_oct[*ob_clp].d26c_kind == D2_ABSTRACT)
    {                       /*  generate update error                 */
      d27_301_oid_str(&d26_oct[*ob_clp].d26c_ob_id,ob_id1);
      return(D26_ERROR);
    } else {                    /*  check whether it is among the         */
			    /*  permitted auxiliary object classes    */
      for                   /*  all auxiliary object classes          */
	 (aux_idx = d26_oct[*ob_cl].d26c_aux_cl; *aux_idx != -1; aux_idx++)
      {                     /*  compare with the actual index         */
	if                  /*  OCT-indices match                     */
	  ((signed32)*aux_idx == *ob_clp)
	{                   /*  object class is OK, break loop        */
	  break;
	}                   /*                                        */
      }                     /*                                        */
      if                    /*  actual index is not among the         */
			    /*  auxiliary object classes              */
	(*aux_idx == -1)
      {                     /*  log the error                         */
	d27_301_oid_str(&d26_oct[*ob_clp].d26c_ob_id,ob_id1);
	d27_301_oid_str(&d26_oct[*ob_cl].d26c_ob_id,ob_id2);
	return(D26_ERROR);
      }                     /*                                        */
    }                       /*                                        */
  }                         /*                                        */
}                           /*                                        */

			    /*  return count of object class values   */
return((signed16)(ob_clp - ob_cl));

}                                   /*  d26_714_check_obcl_consistency*/
