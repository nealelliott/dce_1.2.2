/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: moden.c,v $
 * Revision 1.1.733.2  1996/02/18  18:15:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:37:48  marty]
 *
 * Revision 1.1.733.1  1995/12/08  15:49:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:27  root]
 * 
 * Revision 1.1.731.4  1994/06/21  14:46:10  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:36:38  marrek]
 * 
 * Revision 1.1.731.3  1994/05/10  15:53:04  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:25:55  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:27:09  marrek]
 * 
 * Revision 1.1.731.2  1994/03/23  15:13:41  keutel
 * 	March 1994 code drop
 * 	[1994/03/22  14:15:12  keutel]
 * 
 * 	March 1994 code drop
 * 	[1994/03/21  16:24:54  keutel]
 * 
 * Revision 1.1.731.1  1994/02/22  18:41:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:23:09  marrek]
 * 
 * Revision 1.1.729.3  1993/10/29  15:11:26  keutel
 * 	CR 9261
 * 	[1993/10/29  14:04:48  keutel]
 * 
 * Revision 1.1.729.2  1993/10/14  17:16:49  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:13:50  keutel]
 * 
 * Revision 1.1.729.1  1993/10/13  17:31:24  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:28:15  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:46:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:24:06  marrek]
 * 
 * Revision 1.1.4.6  1993/02/02  14:57:06  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.5  1993/02/02  11:32:47  marrek
 * 	Error for OCL-value-modify corrected
 * 	introduce D2_IA5
 * 	[1993/02/01  15:34:53  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:43:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:00  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:05:44  marrek
 * 	November 1992 code drop
 * 	Prototyping of u10 was wrong
 * 	Inserted new parameter in d26_u10_syntax_check
 * 	[1992/12/04  10:52:49  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  20:28:43  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  10:26:31  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:54:39  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:59:48  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char moden_rcsid[] = "@(#)$RCSfile: moden.c,v $ $Revision: 1.1.733.2 $ $Date: 1996/02/18 18:15:15 $";
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
/* NAME         : moden.c  <DS_MODIFY_ENTRY>                          */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 01.02.88                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : This module contains the functions used for         */
/*                DS_MODIFY_ENTRY.                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*  0.1     |01.02.88 |  original                      | as |         */
/*          |         |                                |    |         */
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

/*****  internal Includes    *****/
#include <d26dsa.h>
#include <d26schema.h>

/*****  external Includes    *****/
#include <dce/d27util.h>
#include <malgcfe.h>

/*****  Operating-System-Includes  *****/
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_C01_SEARCH_MEM   20480  /* minimum length of DS_SEARCH invoke   */
				    /* message                              */
				    /* (minimum must be the maximum value   */
				    /* of the memory sizes which are        */
				    /* checked by the function              */
				    /* d26_C02_set_search_par using the     */
				    /* function d26_C03_check_search_memsize*/
				    /* = maximum attr. value length         */
				    /* (ACL ca. 18 k) + D2_ALIGN (length)   */
				    /* + sizeof (D23_ft_info) + 1)          */

				    /* length of DS_SEARCH result apdu      */
#define D26_C02_OUT_SEARCHMEM   8000

#define D26_C03_ERR_COMM_LEN    100 /* length of comment for exit or error  */

				    /* array size for update of dn list for */
				    /* old and new acl indeces (sorted and  */
				    /* unsorted) and old and new master-    */
				    /* knowledge index                      */
#define D26_C05_DNS_MAX   2 * D2_ACLIDX_MAX + 2
				    /* begin of old acl index values in     */
				    /* array                                */
#define D26_C06_OLD_ACL   D2_ACLIDX_MAX
				    /* begin of new acl index values in     */
				    /* array                                */
/* D26_C07_NEW_ACL   0                 defined in d26dsa.h                  */
				    /* old masterknowledge index value in   */
				    /* array                                */
#define D26_C08_OLD_MK    2 * D2_ACLIDX_MAX + 1
				    /* new masterknowledge index value in   */
				    /* array                                */
/* D26_C09_NEW_MK    2 * D2_ACLIDX_MAX  defined in d26dsa.h                 */

#define D26_C10_SCH_MOD_TERM 2      /* indicates a terminating schema */
				    /* modification                   */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/* structure for attribute check (d26_C05_find_attr) */
typedef struct D26_CT0_inf_attrs {
	struct D26_CT0_inf_attrs   *next_attr; /* pointer to next attribute*/
	D2_a_type     attr_type;         /* attribute type                 */
	signed16      val_length;        /* length of attribute value      */
	char          *attr_value;       /* attribute value                */
	} D26_CT0_inf_attrs;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/******** EXPORT ********/
				    /* exact name for error logging   */
				    /* used in modenfil.c too	      */
byte             exact_name[D2_DNL_MAX+1];

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/
static signed16 d26_C20_sch_mod(signed16, D26_md_info *);
static signed16 d26_C00_mod_attr_check (D26_md_info *, D26_attr_info *, char *,
		    boolean *, D23_av_value **, D23_av_value **, D26_dns **,
		    D26_dns *, D26_acl *, byte **, signed32 *, D2_name_string,
		    signed32 *, boolean);
static signed16 d26_C01_analyse_modentry_msg (D23_inmodentr *, D26_md_info *,
		    char **, signed16 *, signed16 *, byte **, byte **, 
		    signed32 *);
static signed16 d26_C02_set_search_par(D23_inmodentr *, D26_md_info *, signed16,
		    D23_insearch **, byte **, signed32 *, signed32);
static signed16 d26_C03_check_search_memsize(signed32, char **, char **, 
		    char **, char **, char **, signed16 *, byte **, signed32 *);
static signed16 d26_C05_find_attr(D26_CT0_inf_attrs *, D2_a_type *, signed16 *,
		    char *, D26_at *, signed32, boolean, D26_CT0_inf_attrs **,
		    byte **, signed32 *, D2_name_string);
static signed16 d26_C06_add_new_dns(char *, D26_acl *, D26_acl *, signed32 *, 
		    signed32 *, D23_av_value *, D23_av_value *, D26_dns *[], 
		    D26_dns *, boolean *, D26_dns *[], signed16 *);
static signed16 d26_C07_del_old_dns(D26_dns *[], signed16, signed32 *, 
		    signed16);
static signed16 d26_C15_mand_attr_check(D26_CT0_inf_attrs *,
		    D26_CT0_inf_attrs **, byte **, signed32 *, D2_name_string, 
		    signed32 *, boolean);
static signed16 d26_C16_name_attr_check(D2_name_string, D26_at *, 
		    D26_md_info *);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C_modify_entry      <DS_MODIFY_ENTRY>             */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        01.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function is the entry point of the ISERVICE-     */
/*              interface DS_MODIFY_ENTRY.                            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Short            op_id;        operation ID                   */
/*      D23_inmodentr   *mod_in        invoke message                 */
/*                                                                    */
/*      int             *length        INPUT OUTPUT parameter         */
/*                                     -> memory length of mod_out    */
/*                                     <- data length of mod_out      */
/*      Short           cont_id;        context ID                    */
/*      Int             usr_id;         user ID                       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      Char            **dap_in        pointer to dap-request        */
/*      Octet           **mod_out       result (or error) message     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short    D2_NO_ERR     no error occurred                       */
/*                                                                    */
/*              D26_ERROR     any error occurred                      */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*      d26_requestor       D26_requestor       requestor of the      */
/*                                              operation             */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        free                                                        */
/*        sprintf                                                     */
/*        d26_C00_mod_attr_check                                      */
/*        d26_C01_analyse_modentry_msg                                */
/*        d26_C02_set_search_par                                      */
/*        d26_C04_change_main_entry                                   */
/*        d26_C06_add_new_dns                                         */
/*        d26_C07_del_old_dns                                         */
/*        d26_C13_change_rec_attrs                                    */
/*        d26_C17_get_aliased_name                                    */
/*                                                                    */
/*        d26_mi6_save_schema_obj                                     */
/*        d26_u00_search                                              */
/*        d26_u05_acl_check                                           */
/*        d26_u09_init_dsa                                            */
/*                                                                    */
/*        d26_i14_unlock                                              */
/*                                                                    */
/*        d27_007_norm_name                                           */
/*        d27_010_set_error_apdu                                      */
/*        d27_012_remove_blanks                                       */
/*        d27_018_sort_name                                           */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_C_modify_entry (signed16       op_id,
			  D23_inmodentr *mod_in,
			  signed32       in_length,
			  byte         **dap_in,
			  byte         **mod_out,
			  signed32      *length,
			  signed16       cont_id,
			  signed32       usr_id
			 )


{                               /*  d26_C_modify_entry                   */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */
static const char  function[]      = "d26_C_modify_entry";

/* AUTO      ...    */
				     /* variables for erronous exits        */
signed16        return_val = D2_NO_ERR;
				     /* return value of called functions    */
signed16        ret_val = D2_NO_ERR;
signed16        search_ret_val;

signed32        dap_len;      /* length of DAP-message              */
signed32        rem_len;      /* remaining length of search mess.   */
signed32        meslen;       /* length for error message           */
signed32        serv_ctrls;   /* valid service controls             */

				     /* informations of attr. modifications */
D26_md_info       mod_attr[D2_AT_MAX+1];

				     /* normed entry name                   */
char              n_entryname [D2_DNL_MAX+1];
char              sup_normed[D2_DNL_MAX];     /* normed superior name */
char             *p;
D2_name_string    name_for_aclcheck;

D2_name_string    entryname;    /* ptr to entryname in message         */
signed16          l_entryname;  /* length of entryname (including EOS) */
				     /* no of attr. which should be changed */
signed16          no_of_attributes;

D23_insearch      *in_search;   /* input message of int. DS_SEARCH     */

				     /* output message block of DS_SEARCH   */
/*
AUTO char              out_search[D26_C02_OUT_SEARCHMEM];
*/
long              key;          /* C-ISAM key of record                */

				     /* attribute information of attr. which*/
				     /* should be modified                  */
D26_attr_info     attr_info[D2_AT_MAX + 1];

				     /* acl values (indices to DN list)     */
				     /* acl of entry (needed for acl_check) */
D26_acl           acl_old[D2_ACLIDX_MAX];
				     /* new acl when acl of entry is to be  */
				     /* changed                             */
D26_acl           acl_new[D2_ACLIDX_MAX];

signed32          old_mk_idx;   /* index of master DSA name in DN list */
				     /* (D26_OWNNAME indicates d26_myname)  */

signed32          new_mk_idx;   /* index of master DSA name in DN list */
				     /* (D26_OWNNAME indicates d26_myname)  */
				     /* when masterknowledge attribute is   */
				     /* to be changed                       */

boolean           rec_at_flag;  /* flag indicating whether DS_SEARCH   */
				     /* supplies a recurring attribute      */
				     /* that is really recurring attr. are  */
				     /* to be modified                      */

				     /* flag indicating whether dnlist in   */
				     /* isam file must be updated           */
boolean            upd_dnl_file = FALSE;

char              *ptr_out;     /* pointer to outgoing message         */

char              *record;      /* main entry record supplied by       */
				/* DS_SEARCH                           */

boolean           alias_flag;	/* flag indicating whether base object */
				/* is an alias (supplied by DS_SEARCH) */

				/* pointer to error apdu               */
D23_rserror       *err_ptr = (D23_rserror *) NULL;
signed16          rop_id;       /* reduced Op.-ID                      */
signed16          obj_idx;      /* index of object in SRT              */
				/* (par. obj_nr of d26_u00_search)     */

				     /* list of object classes              */
				     /* (par. ob_cl of d26_u00_search)      */
signed32	ob_cl[D26_OCL_VAL_COUNT + 1];

				     /* pointer to indices of dn list which */
				     /* should be "deleted" (replaced or    */
				     /* deleted values of attributes with   */
				     /* distinguished name syntax           */
				     /* the needed memory will be allocated */
				     /* by malloc or calloc and realloc)    */
signed32	*del_dns = (signed32 *) NULL;
				     /* no of indices in del_dns            */
signed16	no_del_dns = 0;
				     /* memory is available for             */
				     /* no_mem_del_dns in del_dns           */
signed16	no_mem_del_dns = 0;

				     /* array of pointers pointing to the */
				     /* internal structure "dns" contain- */
				     /* ing the ACL-names and MASTER-DSA  */
				     /* and deleted or replaced attributes*/
				     /* and values with dist. name syntax */
D26_dns         *dnl_ptr[D26_C05_DNS_MAX];
				     /* array of pointers pointing to the */
				     /* internal structure "dns" contain- */
				     /* ing the ACL-names (sorted pointer */
				     /* list) and MASTER-DSA              */
				     /* and deleted or replaced attributes*/
				     /* and values with dist. name syntax */
D26_dns         *dnl_sort[D26_C05_DNS_MAX];
				     /* internal structure containing ACL-*/
				     /* names and MASTER-DSA              */
D26_dns         dns[D26_C05_DNS_MAX];

				     /* number of entries in sorted DNL-list*/
signed16	no_dnl_entries = 0;

				     /* indicates whether the acl attribute */
				     /* should be modified                  */
D23_av_value    *acl_attr = (D23_av_value *) NULL;
				     /* indicates whether the master-       */
				     /* knowledge attribute should be       */
				     /* modified                            */
D23_av_value     *mk_attr = (D23_av_value *) NULL;

signed16	sch_mod_status; /* schema modification status     */

				/* requestor for first schema     */
				/* modification request           */
D26_requestor d26_sch_modificator;

D23_addcrdpb	auc_pb = {{D23_V02}};
D23_readcrdpb	ruc_pb = {{D23_V02}};

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

rop_id  = op_id > D2_ADM_OPID ? op_id - D2_ADM_OPID : op_id;

/*********/
/* entry */
/*********/

if                               /*  error during initialization of DSA    */
   (d26_u09_init_dsa (0))
{                                /*  set error apdu                        */
     d27_010_set_error_apdu ((D23_rserror *) *mod_out, D2_PERM_ERR,
			      D2_SYS_ERR, D2_NOT_SPEC, length);
     return (D26_ERROR);         /*  exit funtion                          */
}

				 /*  trace function entry                  */
DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));


/*******************/
/* analyse message */
/*******************/

if                               /*  d26_C01_analyse_modentry_msg fails   */
   (d26_C01_analyse_modentry_msg (mod_in, &mod_attr[0], (char **) &entryname,
				  &l_entryname, &no_of_attributes,
				  dap_in, mod_out, length)
     != D2_NO_ERR)
{                                
   /* GDS_S_FCT_FAILED (C01) */
   DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 11));
   return (D26_ERROR);
} 

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

				 /* length without chaining args          */
dap_len = in_length - ((char *)*dap_in - (char *) mod_in);

				 /* traces                                */
if                               /*  no attributes should be modified     */
   (no_of_attributes == 0)
{                               
     *length = 0;
     ptr_out = (char *) *mod_out;
     if                    /*  DSP was coming in                         */
       (d26_chain_arg != (D23_chain_arg *)NULL)
     {                     /*  write chaining result                     */
     ((D23_chain_res *) ptr_out)->d23_chrtag  = D23_CHRTAG;
     ptr_out += D23_S_CHRES;
     *length += D23_S_CHRES;
     }

				 /*  generate result message (OK)  */
     ((D23_eom *) ptr_out)->d23_eomtag = D23_EOMTAG;
     *length += sizeof (D23_eom);
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NO_ERR));
     return (D2_NO_ERR);
}

/*****************/
/* search object */
/*****************/

ret_val = d26_C02_set_search_par (mod_in, &mod_attr[0],no_of_attributes,
				  &in_search, mod_out, length, serv_ctrls);
if (ret_val != D2_NO_ERR)
{   
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 12));
     return (D26_ERROR);
} 

search_ret_val = d26_u00_search (TRUE, rop_id,  (byte *) in_search,
			  length, mod_out, &rem_len, exact_name,
			  attr_info, ob_cl, acl_old, &old_mk_idx,
			  &rec_at_flag, &record, &alias_flag, &obj_idx);

				 /*  check result of DS_SEARCH             */
if                               /*  DS_SEARCH fails                       */
   (((search_ret_val != D2_NO_ERR)  && (op_id > D2_ADM_OPID)) ||
    ((op_id < D2_ADM_OPID) && (search_ret_val != D2_NOERROR) &&
				 (search_ret_val != D2_REF_ERROR)))
{                                /*  unlock used files                     */
     d26_i14_unlock ();
     if                 /*  still no result present                   */
	 (search_ret_val == D2_REF_ERROR)
      {                 /*  write the error referrals                 */
	if              /*  hard error from writing referrals         */
	  (d26_u63_write_error_referral(mod_out,length, &rem_len, d26_referral)
								 != D2_NO_ERR)
	{               /*  set  error apdu                           */
	   d27_010_set_error_apdu((D23_rserror *)*mod_out,D2_TEMP_ERR,
				  D2_REM_ERR,D2_UNWILLING,&meslen);
	   rem_len -= meslen;
	}               /*                                            */
      d26_u41_free_referrals();
      }
     *length -= rem_len;
     free ((char *) in_search);
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 13));
     return (D26_ERROR);         /*  exit function                         */
}                                /*  DS_SEARCH fails */

if                                  /*  standard mod. op -> check     */
				    /*  wether to chain etc.          */
      ((op_id < D2_ADM_OPID) && ((search_ret_val == D2_NOERROR) ||
				 (search_ret_val == D2_REF_ERROR)))
{   
  ret_val = d26_st01_chain_ref(search_ret_val, serv_ctrls, old_mk_idx,
				  FALSE, op_id);

  if (ret_val != D2_NOERROR)     /* check the error and return        */
  {
    switch (ret_val)
    {
     case D2_CHAINING_REQUIRED:
	ret_val = d26_d01_chain_request(*dap_in, dap_len, mod_out,length,
	    &rem_len, D23_MODENTR,(D23_rsrqpb *)NULL);
	if (ret_val != D2_REF_ERROR && ret_val != D26_ERROR)
	{ 
          DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_val));
	  break; 
	}               /* else write error referral                  */
     case D2_REF_ERROR:
	if              /*  hard error from writing referrals         */
	  (d26_u63_write_error_referral(mod_out,length, &rem_len, d26_referral)
								 != D2_NO_ERR)
	{               /*  set  error apdu                           */
	   d27_010_set_error_apdu((D23_rserror *)*mod_out,D2_TEMP_ERR,
				  D2_REM_ERR,D2_UNWILLING,&meslen);
	   rem_len -= meslen;
	}               /*                                            */
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 14));
	ret_val = D26_ERROR;
	break;
     case D2_AFFECTS_MULTIPLE_DSAS:
	d27_010_set_error_apdu((D23_rserror *) *mod_out, D2_CALL_ERR,
			 D2_PAR_ERR, D2_AFFECTS_MULTIPLE_DSAS, &meslen);
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 15));
	ret_val = D26_ERROR;
	break;
     default: /* error */
	ret_val = D26_ERROR;
	break;
    }
    free ((char *) in_search);
    d26_u41_free_referrals();
    d26_i14_unlock ();
    *length -= rem_len;
    return(ret_val);
  }
} 

/* if data base modification not allowed return error BUSY */
if (d26_dbop_flag == D26_DB_RO)
{
  d27_010_set_error_apdu((D23_rserror *) *mod_out, D2_TEMP_ERR,
		   D2_INT_ERR, D2_TOO_BUSY, &meslen);
  rem_len -= meslen;
  d26_i14_unlock();
  *length -= rem_len;
  free ((char *) in_search);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 16));
  return(D26_ERROR);
}

/********************/
/* check attributes */
/********************/
if (old_mk_idx == D26_OWNNAME)
{                                   /* master info is modified             */
   d26_ma_info = TRUE;
} else {
   d26_ma_info = FALSE;
} 

				 /*  remove blanks from entryname          */
l_entryname = d27_012_remove_blanks ((String) entryname, l_entryname,
				     (String) entryname);
				 /*  norm entryname                        */
d27_007_norm_name ((String) entryname, l_entryname, n_entryname);
d27_018_sort_name((D2_name_string)n_entryname);
				 /*  put object name to global field */
strcpy((char *) d26_objname, n_entryname);

				 /*  check attribute modifications         */
ret_val = d26_C00_mod_attr_check (&mod_attr[0], &attr_info[0],
				  (String) *mod_out, &upd_dnl_file,
				  &acl_attr, &mk_attr, dnl_ptr, &dns[0],
				  acl_new, mod_out, length,
				  (D2_name_string)n_entryname,ob_cl,
				  alias_flag);

if                               /*  at least one attribute is wrong       */
   (ret_val != D2_NO_ERR)
{                                /*  unlock used files                     */
   d26_i14_unlock ();
				 /* length is set in C00                   */
   free ((char *) in_search);
   DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 17));
   return (D26_ERROR);           /*  exit function                         */
}                                /*  at least one attribute is wrong */

/****************/
/* check rights */
/****************/
				 /*  set name for acl check                */
if (alias_flag == TRUE)
{    /* superior object name is used for acl check */
    strcpy(sup_normed,n_entryname);             /*  creating superior name */
    for (p = &sup_normed[strlen(sup_normed) - 1]; ; p--)
    {    if (*p == D2_RDN_SEP)
	 {   *p = D2_EOS;
	     break;
	 }
    }
    name_for_aclcheck = (D2_name_string) sup_normed;
} else { /* entryname itself is used for acl check    */
     name_for_aclcheck = (D2_name_string) n_entryname;
}  /* alias_flag == D2_TRUE */

/* GDS_S_NAME_LOGGING ("Check ACL","parent"|"object") */
				 /*  check rights                          */
ret_val = d26_u05_acl_check(name_for_aclcheck, D26_MODIFY, attr_info, acl_old);
if                               /*  error occurred during check rights    */
   (ret_val != D2_NO_ERR)
{                                /*  set error message                     */
     if (ret_val == D2_ACCESS_RIGHTS_INSUFFICIENT)
     {    /* access rights insufficient */
	  d27_010_set_error_apdu ((D23_rserror *) *mod_out, D2_CALL_ERR,
				   D2_PAR_ERR, D2_ACCESS_RIGHTS_INSUFFICIENT,
				   &meslen);
	rem_len -= meslen;
     } else { /* other error occurred */
	  d27_010_set_error_apdu ((D23_rserror *) *mod_out, D2_TEMP_ERR,
				   D2_REM_ERR, D2_UNWILLING,
				   &meslen);
	rem_len -= meslen;
     } 
				 /*  unlock used files                     */
     d26_i14_unlock ();
     *length -= rem_len;
     free ((char *) in_search);
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 18));
     return (D26_ERROR);         /*  exit function                         */
}                                /*  error during check rights             */


/**********************************/
/* update distinguished name list */
/**********************************/

if                               /*  dist. name list in isam file must be  */
				 /*  updated                               */
   (upd_dnl_file == TRUE)
{                                /*  add new dns to dn list in file        */
     ret_val = d26_C06_add_new_dns (n_entryname, acl_old, acl_new,
				    &old_mk_idx, &new_mk_idx, acl_attr,
				    mk_attr, dnl_ptr, &dns[0],
				    &upd_dnl_file,
				    dnl_sort, &no_dnl_entries);
     if                          /*  error occurred during add new dns     */
	(ret_val != D2_NO_ERR)
     {                           /*  set error message                     */
	  d27_010_set_error_apdu ((D23_rserror *) *mod_out, D2_TEMP_ERR,
				   D2_REM_ERR, D2_UNWILLING,
				   &meslen);
	  rem_len -= meslen;
	  *length -= rem_len;
				 /*  unlock used files                     */
	  d26_i14_unlock ();
	  free ((char *) in_search);
          DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 19));
	  return (D26_ERROR);    /*  exit function                         */
     }                           /*  error during add new dns              */
}                                /* */

if                              /*  schema will be modified           */
  ((sch_mod_status = d26_C20_sch_mod(no_of_attributes,mod_attr)) != FALSE)
{                               /*  check whether it is the first time*/
  if                            /*  DSP is being processed            */
    (d26_requestor.d26_sm_flag == D26_SM_UNKNOWN)
  {                             /*  read user credentials             */
    ruc_pb.d23_Ocontid = cont_id;
    ruc_pb.d23_Ousrid = usr_id;
    ruc_pb.d23_Ocredref = (byte *) &d26_sch_modificator;

    if                          /*  requestor cannot be read          */
      (d23_read_usr_cred(&ruc_pb) == D2_ERROR)
    {                           /*  reset schema modification flag    */
      d26_requestor.d26_sm_flag = D26_NO_SM_PENDING;
    }                           /*                                    */
  }                             /*                                    */
  if                            /*  modification flag not yet pending */
    (d26_requestor.d26_sm_flag == D26_NO_SM_PENDING)
  {                             /*  save schema object                */
    if                          /*  save schema object fails          */
      (d26_mi6_save_schema_obj() == D26_ERROR)
    {                           /*  write error message               */
      d27_010_set_error_apdu((D23_rserror *)*mod_out,D2_TEMP_ERR,
			     D2_REM_ERR,D2_UNWILLING,&meslen);
      rem_len -= meslen;
      *length -= rem_len;
      free ((char *) in_search);
      d26_i14_unlock ();
      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 20));
      return(D26_ERROR);
    } else {                        /*  reset schema modification flag    */
      d26_requestor.d26_sm_flag = D26_SM_PENDING;
      auc_pb.d23_Mcontid = cont_id;
      auc_pb.d23_Musrid = usr_id;
      auc_pb.d23_Mcredsize = D26_REQ_LEN;
      auc_pb.d23_Mcredref = (byte *) &d26_requestor;

      if                        /*  error from writing user credential*/
	(d23_add_usr_cred(&auc_pb) != D2_NOERROR)
      {                         /*  write error message               */
	d27_010_set_error_apdu((D23_rserror *)*mod_out,D2_TEMP_ERR,
			       D2_REM_ERR,D2_UNWILLING,&meslen);
	rem_len -= meslen;
	*length -= rem_len;
	free ((char *) in_search);
	d26_i14_unlock ();
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 21));
	return(D26_ERROR);
      }
    }
  }
}

/*********************/
/* change main entry */
/*********************/
				 /*  change main entry                     */
ret_val = d26_C04_change_main_entry ((D2_name_string) n_entryname, ob_cl[0],
				     obj_idx, alias_flag,
				     no_of_attributes, &mod_attr[0],
				     &attr_info[0], dnl_ptr, record,
				     &upd_dnl_file, &key, &del_dns,
				     &no_del_dns, &no_mem_del_dns,
				     (D23_rserror *) *mod_out, length);

if                               /*  error occurred during change main entry*/
   (ret_val != D2_NO_ERR)
{                                /*  free del_dns                          */
   if (del_dns != (signed32 *) NULL)
   {
	free ((char *) del_dns);
	del_dns = (signed32 *) NULL;
   }
			       /*  unlock used files                     */
   d26_i14_unlock ();
			       /* in error case length is set by C04     */
   free ((char *) in_search);
   DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 22));
   return (D26_ERROR);         /*  exit function                         */
}                              /*  error during change main entry        */


/************************************/
/* change real recurring attributes */
/************************************/

if                               /*  really recurring attr are to be changed*/
   (rec_at_flag == TRUE)
{                                /*  update recurring attributes            */
     ret_val = d26_C13_change_rec_attrs (ob_cl[0], key,
					 no_of_attributes,
					 &mod_attr[0], &attr_info[0]);
     if (ret_val != D2_NO_ERR)
     {   
     }  /* There is no error returned because the objectrecord is already */
}       /* updated. This a bad solution, this will be improved if we      */
	/* implement transaction routines                                 */

/***********************************************************/
/* check whether "scheme" object was successfully modified */
/***********************************************************/

if                              /*  schema modification successfully  */
				/*  terminated                        */
  (ret_val == D2_NO_ERR && sch_mod_status == D26_C10_SCH_MOD_TERM)
{                               /*  reset the global schema           */
				/*  modification flag                 */
  d26_requestor.d26_sm_flag = D26_SM_COMPLETE;
}

/**********************************/
/* update distinguished name list */
/**********************************/

if                               /*  dist. name list in isam file must be  */
				 /*  updated                               */
   (upd_dnl_file == TRUE)
{                                /*  delete old names from dnl             */
     ret_val = d26_C07_del_old_dns (&dnl_sort[0], no_dnl_entries,
				    del_dns, no_del_dns);
     if (ret_val != D2_NO_ERR)
     {   
     /* GDS_S_FCT_FAILED ("C07") */
     }      /* same problem as above ... */
}



/********/
/* exit */
/********/

if                              /*  operation successfull                  */
   (return_val == D2_NO_ERR)
{                               /*  generate result message (OK)           */

     *length = 0;
     ptr_out = (char *) *mod_out;
     if                    /*  DSP was coming in                         */
       (d26_chain_arg != (D23_chain_arg *)NULL)
     {                     /*  write chaining result                     */
     ((D23_chain_res *) ptr_out)->d23_chrtag  = D23_CHRTAG;
     ptr_out += D23_S_CHRES;
     *length += D23_S_CHRES;
     }                     /*                                            */

     ((D23_eom *) ptr_out)->d23_eomtag = D23_EOMTAG;
      *length += sizeof (D23_eom);
      err_ptr = (D23_rserror *) NULL;
     /* GDS_S_FCT_FAILED ("C") */
} else {                            /*  set error message                      */
     d27_010_set_error_apdu ((err_ptr = (D23_rserror *) *mod_out),
			      D2_TEMP_ERR, D2_REM_ERR, D2_UNWILLING,
			      &meslen);
     rem_len -= meslen;
     *length -= rem_len;
     err_ptr = (D23_rserror *) *mod_out;
}                               /*  operation successfull */

				/*  unlock used files                      */
d26_i14_unlock ();


				/*  trace function exit                    */
free ((char *) in_search);
DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_val));
return (return_val);

}                               /*  d26_C_modify_entry                */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_C00_mod_attr_check(...)                         */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 08.12.87                                            */
/*                                                                    */
/* SYNTAX       : LOCAL Short d26_C00_mod_attr_check(at_str, at_info, */
/*                                  searchresult, upd_dnlist_file,    */
/*                                  acl_attr, mk_attr, dnl_ptr, dns,  */
/*                                  acl_idx, error, l_err, entryname, */
/*                                  ob_cl, alias_flag)                */
/*                                                                    */
/*                D26_md_info    *at_str;               ->            */
/*                D26_attr_info  *at_info;              ->            */
/*                char           *searchresult;         ->            */
/*                Bool           *upd_dnlist_file;      <-            */
/*                D23_av_value   **acl_attr;            <-            */
/*                D23_av_value   **mk_attr;             <-            */
/*                D26_dns        **dnl_ptr;             <-            */
/*                D26_dns        *dns;                  <-            */
/*                D26_acl        *acl_idx;              <-            */
/*                Octet	         **error;               <>            */
/*                Int            *l_err;                <>            */
/*                D2_name_string entryname;             ->            */
/*                Long           *ob_cl;                ->            */
/*                Bool           alias_flag;            ->            */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : This function checks the whether the given          */
/*                informations for the attributes are correct.        */
/*                                                                    */
/*                The following informations are checked:             */
/*                    - syntax of acl attribute                       */
/*                    - minimum and maximum length of given new value */
/*                    - whether a new value is given when needed      */
/*                    - whether the modification type is allowed on   */
/*                      attribute                                     */
/*                    - whether an attribute should be added more     */
/*                      times                                         */
/*                    - whether an attribute should be deleted more   */
/*                      times                                         */
/*                    - whether an attribute value should be deleted  */
/*                      more times (via delete or replace value)      */
/*                    - whether the attribute exists or doesn't exist */
/*                      in the entry                                  */
/*                    - whether an attribute value should be added to */
/*                      a former deleted attribute                    */
/*                    - whether an attribute value should be replaced */
/*                      at a former deleted attribute or the value    */
/*                      is already deleted                            */
/*                    - whether a mandatory attribute is deleted      */
/*                    - whether aliased object attribute is modified  */
/*                      for an alias object only                      */
/*                      (is already checked by d26_u00_search)        */
/*                                                                    */
/*                The following is checked during the function        */
/*                "change main entry":                                */
/*                    - whether the last attribute value is deleted   */
/*                    - whether the maximum number of values is       */
/*                      exceeded.                                     */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*       D26_md_info     *at_str;           attribute structure       */
/*                                          containing the attributes */
/*                                          to be checked             */
/*                                                                    */
/*       D26_attr_info   *at_info;          attribute information     */
/*                                          (index in the AT)         */
/*                                          was set by DS_SEARCH with */
/*                                          attributes to be modified */
/*                                          == requested attributes   */
/*                                                                    */
/*       char            *searchresult;     result message of         */
/*                                          DS_SEARCH                 */
/*                                          (needed for check whether */
/*                                          a attribute exists or     */
/*                                          doesn't exist in the      */
/*                                          entry                     */
/*                                                                    */
/*       D2_name_string  entryname          name of entry to be       */
/*                                          modified                  */
/*                                                                    */
/*       Long            *ob_cl             object class list         */
/*                                                                    */
/*       Bool            alias_flag         flag indicating whether   */
/*                                          object is an alias        */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*       Bool            *upd_dnlist_file;  indicator whether the     */
/*                                          distinguished name list   */
/*                                          in C-isam file must be    */
/*                                          updated                   */
/*                                          (the file must be updated */
/*                                          when acl or master-       */
/*                                          knowledge attribute should*/
/*                                          be modified)              */
/*                                                                    */
/*       D23_av_value    **acl_attr;        indicates whether the acl */
/*                                          attribute should be       */
/*                                          modified                  */
/*                             D2_NULL      acl attr. shouldn't be    */
/*                                          modified                  */
/*                             != D2_NULL   acl attr. should be       */
/*                                          modified (== address of   */
/*                                          given acl value)          */
/*                                                                    */
/*       D23_av_value    **mk_attr;         indicates whether the     */
/*                                          masterknowledge attribute */
/*                                          should be modified        */
/*                             D2_NULL      mk. attr. shouldn't be    */
/*                                          modified                  */
/*                             != D2_NULL   mk. attr. should be       */
/*                                          modified (== address of   */
/*                                          given mk. value)          */
/*                                                                    */
/*   for new acl attribute value:                                     */
/*       D26_dns         **dnl_ptr;         pointer array to "dns"    */
/*                                          (internal structure for   */
/*                                          sorting and merging,      */
/*                                          memory must be allocated  */
/*                                          by the caller)            */
/*                                                                    */
/*       D26_dns         *dns;              filled structure of       */
/*                                          acl information           */
/*                                          (memory must be allocated */
/*                                          by the caller)            */
/*                                                                    */
/*       D26_acl         acl_idx;           is initialized to         */
/*                                          D26_IDX_NOT_DEF           */
/*                                          (memory must be allocated */
/*                                          by the caller)            */
/*                                                                    */
/*   In case of errors:                                               */
/*       Octet           **error;           error message (contains   */
/*                                          the wrong attribute and/or*/
/*                                          return codes)             */
/*                                                                    */
/*       Int             *l_err;            length of error message   */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      any error occurred                */
/*                                  (error and l_err are supplied)    */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  D26_at     *d26_at;         Attribute-Description-*/
/*                                              Table                 */
/*                  D23_chain_arg *d26_chain_arg chaining arguments   */
/*                                                                    */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d26_u10_syntax_check                              */
/*                  d26_u15_attr_error()                              */
/*                  d26_u23_get_acl_names()                           */
/*                  d26_u99_handle_blanks                             */
/*                                                                    */
/*                  d26_C05_find_attr()                               */
/*                  d26_C15_mand_attr_check()                         */
/*                                                                    */
/*                  d27_010_set_error_apdu()                          */
/*                  d27_301_oid_str                                   */
/*                                                                    */
/*                  sprintf                                           */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C00_mod_attr_check(D26_md_info     *at_str,
				    D26_attr_info   *at_info,
				    char            *searchresult,
				    boolean         *upd_dnlist_file,
				    D23_av_value   **acl_attr,
				    D23_av_value   **mk_attr,
				    D26_dns        **dnl_ptr,
				    D26_dns         *dns,
				    D26_acl         *acl_idx,
				    byte           **error,
				    signed32        *l_err,
				    D2_name_string   entryname,
				    signed32        *ob_cl,
				    boolean          alias_flag
				   )

{                                   /*  d26_C00_mod_attr_check()          */

/**********************************************************************/
/*                                                                    */
/*                         D E F I N E S                              */
/*                                                                    */
/**********************************************************************/

#define C00_VAL_CL  28      /* value length in err comment            */
#define C00_TYP_CL  27      /* type length in err comment             */

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

static const char function[] = "d26_C00_mod_attr_check";

signed16 ret_value = D2_NO_ERR, rv;
signed16 i;
				    /* attribute types to be added         */
D26_CT0_inf_attrs  add_a_types [D2_AT_MAX + 1];  /* memory            */
D26_CT0_inf_attrs  *p_add_types;                 /* start of list     */
				    /* attribute values to be added        */
D26_CT0_inf_attrs  add_a_values [D2_AT_MAX + 1]; /* memory            */
D26_CT0_inf_attrs  *p_add_values;                /* start of list     */
				    /* attribute types to be deleted       */
D26_CT0_inf_attrs  del_a_types [D2_AT_MAX + 1];  /* memory            */
D26_CT0_inf_attrs  *p_del_types;                 /* start of list     */
				    /* attribute values to be deleted      */
D26_CT0_inf_attrs  del_a_values [D2_AT_MAX + 1]; /* memory            */
D26_CT0_inf_attrs  *p_del_values;                /* start of list     */

char         *val;            /* pointer to attribute value     */
D2_a_type    type, cmp_type;  /* attribute type                 */
D2_attr_mod  modification;    /* type of modification           */

signed16        offset;
signed32        apdu_len = 0;

unsigned16      rep_ai, rep_av; /* representations for error massage */
boolean         attr_found;	/* indicates whether attribute type  */
				/* exists in the entry               */
signed32	app;		/* indicates whether an type or value  */
				/* should be appended to existing list */
				/* by d26_C05_find_attr                */

signed16	add_type;       /* return value from d26_C05_find_attr */
signed16	add_value;      /* return value from d26_C05_find_attr */
signed16	del_type;       /* return value from d26_C05_find_attr */
signed16	del_value;      /* return value from d26_C05_find_attr */

register signed16 length;	/* attribute length               */
register D26_at      *at;	/* pointer to AT                  */
register D26_attr_info *at_entry;   /* entry in AT                    */
register char       *msg_ptr;       /* pointer to DS_SEARCH result    */

register D23_av_value    *ma;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/************************/
/* initialize variables */
/************************/

for (i=0; i<D2_AT_MAX; i++)
{ 
   p_add_types  = &add_a_types[i];
   p_add_values = &add_a_values[i];
   p_del_types  = &del_a_types[i];
   p_del_values = &del_a_values[i];
   p_add_types->next_attr =
     p_add_values->next_attr =
     p_del_types->next_attr =
     p_del_values->next_attr = (D26_CT0_inf_attrs *) NULL;
   p_add_types->attr_type.d2_type =
     p_add_values->attr_type.d2_type =
     p_del_types->attr_type.d2_type =
     p_del_values->attr_type.d2_type = (Ob_id_string) NULL;
   p_add_types->attr_type.d2_typ_len =
     p_add_values->attr_type.d2_typ_len =
     p_del_types->attr_type.d2_typ_len =
     p_del_values->attr_type.d2_typ_len = 0;
   p_add_types->val_length =
     p_add_values->val_length =
     p_del_types->val_length =
     p_del_values->val_length = 0;
   p_add_types->attr_value =
     p_add_values->attr_value =
     p_del_types->attr_value =
     p_del_values->attr_value = (char *) NULL;
} 
/* initialize start of list */
p_add_types  = (D26_CT0_inf_attrs *) NULL;
p_add_values = (D26_CT0_inf_attrs *) NULL;
p_del_types  = (D26_CT0_inf_attrs *) NULL;
p_del_values = (D26_CT0_inf_attrs *) NULL;

                   /*  set DS_SEARCH result message pointer to first */
		   /*  attribute type                                */
/* The internal DS_SEARCH result message is expected in the following way: */
/*     D23_chain_res [optional: are skipped if necessary]                  */
/*     D23_en_info                                                         */
/*         with first: D23_ds_name                                         */
/*         and then:   D23_at_type with found attribute types sorted as    */
/*                                 requested                               */
/*     D23_eom                                                             */
/* The syntax of the result message isn't checked.                         */

msg_ptr = searchresult;
/* skip chaining results if necessary */
if                    /*  DSP was coming in                         */
  (d26_chain_arg != (D23_chain_arg *)NULL)
{   
     msg_ptr += sizeof (D23_chain_res);
} 
msg_ptr += sizeof (D23_en_info);
offset = ((D23_ds_name *) msg_ptr)->d23_dsoff;
msg_ptr = msg_ptr + sizeof (D23_ds_name) + offset;

/************************/
/* check all attributes */
/************************/

for                     /*  all attributes in queue       */
    (at_entry = &at_info[0]; at_entry->d26_at_idx != D26_EOL;
	      at_str++, at_entry++)
{                       /*                                */
  at = &d26_at[at_entry->d26_at_idx];
  type = at_str->d26_mtype;
  rep_ai = at_str->d26_mrep;
  if (at_str->d26_mold != (D23_av_value *) NULL)
  {
    rep_av = (at_str->d26_mold)->d23_avrep;
  }
  else if (at_str->d26_mnew != (D23_av_value *) NULL)
  {
    rep_av = (at_str->d26_mnew)->d23_avrep;
  }
  else
  {
    rep_av = rep_ai;
  }
  modification = at_str->d26_m;

  /*******************************/
  /* importance (mod. allowed ?) */
  /*******************************/
  if                    /*  importance is wrong         */
     ((at->d26a_importance == D26_NOUPDATE)
      || ((at->d26a_importance == D26_NODELETE) &&
		   (modification != D2_REP_AV))
      || ((at->d26a_importance == D26_NOCHANGE) &&
		   (modification != D2_ADDATT) &&
		   (modification != D2_ADD_AV)))
  {                     /*  return (D26_ERROR)            */
       if (D2_ATTCMP(&type,D2_OBJ_CLASS))
       {
	 d26_u15_attr_error ((byte **) error, exact_name, 1,
			     D2_CONSTRAINT_VIOLATION, &type,
			     rep_ai, rep_av,
			     (signed16) 0, (byte *) NULL, l_err,
			     &apdu_len);
  /* GDS_ATT_ERR ("Modification of object class not allowed","85.4.0") */
	 *l_err = apdu_len;
         DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 11));
	 return(D26_ERROR);
       }
  }                     /*                                */

  /****************************************************************/
  /* new value syntax and mod. not allowed on non recurring attr. */
  /****************************************************************/
  switch ((signed32) modification)   /*  modification type                 */
  {
    case  D2_ADD_AV:            /*  ADD value                              */
		if              /*  not recurring attribute                */
		   (at->d26a_nr_recur == 1)
		{               /*  return (D26_ERROR)                     */
				/* ERROR: not allowed on non recurring attr*/
		  d26_u15_attr_error ((byte **) error,
				      exact_name, 1, D2_CONSTRAINT_VIOLATION,
				      &type,
				      rep_ai, rep_av,
				      (signed16) 0, (byte *) NULL,
				      l_err, &apdu_len);
		  *l_err = apdu_len;

   		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 12));
		  return(D26_ERROR);
		}
		/* FALLTHROUGH */

    case  D2_REP_AV:            /*  REPLACE value                          */
		if              /*  check whether old value is given       */
		   ((modification == D2_REP_AV) &&
		    ((ma=at_str->d26_mold) == (D23_av_value *) NULL))
		{               /*  ERROR: no old value given              */
		  d26_u15_attr_error ((byte **) error,
				      exact_name, 1, D2_CONSTRAINT_VIOLATION,
				      &type,
				      rep_ai, rep_av,
				      (signed16) 0, (byte *) NULL,
				      l_err, &apdu_len);
		  *l_err = apdu_len;

   		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 13));
		  return(D26_ERROR);
		}               /*  check whether old value is given       */
		/* FALLTHROUGH */

    case  D2_ADDATT:            /*  ADD type                               */
				/*  ADD type, ADD value, REPLACE value     */
		if              /*  new value exists                       */
		   ((ma=at_str->d26_mnew) !=
					 (D23_av_value *) NULL)
		{               /*  check length of new attribute          */
		  val = ((char *) ma) + sizeof (D23_av_value);
				/*  remove blanks from new value when      */
				/*  necessary                              */
		  ma->d23_avlen =
			  d26_u99_handle_blanks (at, val, ma->d23_avlen);

		  length = ma->d23_avlen;
		  if            /*  length is wrong                        */
		     (d26_u96_attr_len_check(at, (signed16) length)
					    == D26_ERROR)
		  {             /*  return (D26_ERROR)                     */
				/* ERROR: wrong new value length           */
		    d26_u15_attr_error ((byte **) error,
				      exact_name, 1, D2_CONSTRAINT_VIOLATION,
				      &type,
				      rep_ai, ma->d23_avrep,
				      length, (byte *) val,
				      l_err, &apdu_len);
		    *l_err = apdu_len;

   		    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				D26_ERROR - 14));
		    return(D26_ERROR);

		  } else {      /*  check value syntax                     */
		    if          /*  syntax not ok                          */
	 ((at->d26a_rep == D2_T61_PR && ma->d23_avrep != D2_T61 &&
					ma->d23_avrep != D2_PRINTABLE) ||
	   at->d26a_rep != D2_T61_PR && ma->d23_avrep != at->d26a_rep  ||
	   d26_u10_syntax_check (at,ma->d23_avrep, (byte *) val, length) 
		   != D2_NO_ERR)
		    {           /*  syntax error                           */
		       d26_u15_attr_error ((byte **) error,
				      exact_name, 1, D2_ATSYNTAX_ERROR,
				      &type, rep_ai, ma->d23_avrep,
				      length,(byte *)  val, l_err, &apdu_len);
		       *l_err = apdu_len;

   		       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				D26_ERROR - 15));
		       return(D26_ERROR);
		    }
		  }
		} else {        /*  error: no new value given              */
		    d26_u15_attr_error ((byte **) error,
				      exact_name, 1, D2_CONSTRAINT_VIOLATION,
				      &type,
				      rep_ai, rep_ai,
				      (signed16) 0, (byte *) NULL,
				      l_err, &apdu_len);
		    *l_err = apdu_len;

   		    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				D26_ERROR - 16));
		    return(D26_ERROR);
		}               /*  new value exists                       */
		break;

    case  D2_DEL_AV:            /*  DELETE value                           */
		if              /*  not recurring attribute                */
		   ((at->d26a_nr_recur == 1)  ||
		    ((ma=at_str->d26_mold) == (D23_av_value *) NULL))
		{               /*  return (D26_ERROR)                     */
		  d26_u15_attr_error ((byte **) error,
				      exact_name, 1, D2_CONSTRAINT_VIOLATION,
				      &type,
				      rep_ai, rep_av,
				      (signed16) 0, (byte *) NULL,
				      l_err, &apdu_len);
		  *l_err = apdu_len;

		  if (at->d26a_nr_recur == 1)
		  {    /* ERROR: not allowed on non rec. attribute */
		  } else { /* ERROR: no old value given */
		  } 
   		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 17));
		  return(D26_ERROR);
		}
		/* FALLTHROUGH */

    case  D2_DELATT:            /*  DELETE type                            */
				/*  DELETE type + DELETE value             */
		           /*  no action (break;) */

		break;
    default:            /* otherwise:                    */
			/* return (D26_ERROR)            */
		  d26_u15_attr_error ((byte **) error,
				      exact_name, 1, D2_CONSTRAINT_VIOLATION,
				      &type,
				      rep_ai, rep_av,
				      (signed16) 0, (byte *) NULL,
				      l_err, &apdu_len);
		  *l_err = apdu_len;

   		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 18));
		  return(D26_ERROR);
  }

  /***********************************/
  /* naming attr. (mod. not allowed) */
  /***********************************/
  if                    /*  attribute is naming attr.     */
    (at_entry->d26_srt_idx != D26_IDX_NOT_DEF)
  {                     /*  return (D26_ERROR)            */
       rv = d26_C16_name_attr_check (entryname, at, at_str);
       if (rv == D26_ERROR)
       {   
	    d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR, 
				    D2_PAR_ERR, D2_NOT_ALLOWED_ON_RDN, l_err);

	    /* error log in funktion */
   	    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 19));
	    return(D26_ERROR);
       } 
  }                     /*                                */

  /**********************/
  /* special attributes */
  /**********************/

     /*****************/
     /* ACL attribute */
     /*****************/
     if (D2_ATTCMP (&type, D2_ACL) == 0)
     {    /* ACL attribute found */
	  *acl_attr = at_str->d26_mnew; /* store attribute pointer         */
	  *upd_dnlist_file = TRUE;   /* set flag for update dnlist file */
	  /* check acl syntax and get acl names     */
	  if                       /* ACL-names are not correct */
	     (d26_u23_get_acl_names((D2_name_string) (*acl_attr + 1),
				    (*acl_attr)->d23_avlen,
				    dnl_ptr+D26_C07_NEW_ACL,
				    &dns[D26_C07_NEW_ACL], acl_idx))
	  {                           /* return value = D26_ERROR      */
	       d26_u15_attr_error ((byte **) error,
			     exact_name, 1, D2_ATSYNTAX_ERROR,
			     &type,
			     rep_ai, (*acl_attr)->d23_avrep,
			     (*acl_attr)->d23_avlen, (byte *) (*acl_attr + 1),
			     l_err, &apdu_len);
	       *l_err = apdu_len;

   	       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 20));
	       return(D26_ERROR);
	  }   /* ACL names are not correct */
     }  /* ACL attribute */

     /******************************/
     /* Master-Knowledge attribute */
     /******************************/
     if (D2_ATTCMP (&type, D2_MASTER_KNOWLEDGE) == 0)
     {    /* Master-Knowledge attribute found */
	  *mk_attr = at_str->d26_mnew; /* store attribute pointer         */
	  *upd_dnlist_file = TRUE;  /* set flag for update dnlist file */
	  break;
     }  /* Master-Knowledge attribute */

     /****************************/
     /* Object Class attribute   */
     /****************************/
     /* already checked while building search message */

     /********************/
     /* Dnlist attribute */
     /********************/
     if (D2_ATTCMP (&type, D2_DNLIST) == 0)
     {    /* dnlist attribute found */
	  /* if the modification will be done to    */
	  /* master info the modification type must */
	  /* be D2_REP_AV                           */
	  if (d26_ma_info == TRUE)
	  {    /* check the modification type             */
	       if (modification != D2_REP_AV)
		  {    /* error message */
		       d26_u15_attr_error ((byte **) error,
			      exact_name, 1, D2_CONSTRAINT_VIOLATION,
			      &type,
			      rep_ai, rep_av,
			      (signed16) NULL, (byte *) NULL,
			      l_err, &apdu_len);
		       *l_err = apdu_len;

   		       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				D26_ERROR - 21));
		       return(D26_ERROR);
		  } 
	  } 
	  break;
     }  /* dnlist attribute */

  /********************************/
  /* attribute existence in entry */
  /********************************/
				/*  check whether attribute exists in the  */
				/*  entry that is is returned in DS_SEARCH */
				/*  result message                         */
  attr_found = FALSE;
  if                            /*  msg_ptr points to attr. information    */
     (((D23_at_type *) msg_ptr)->d23_atttag == D23_ATTTAG)
  {                             /*                                         */
       cmp_type.d2_typ_len = (int) (((D23_at_type *)msg_ptr)->d23_atlen);
       cmp_type.d2_type = (Ob_id_string) (msg_ptr + D23_S_ATT);
       if                       /*  attribute type in DS_SEARCH result ==  */
				/*  attribute type of attr. to be changed  */
	  (D27_ATTCMP (&cmp_type, &type) == 0)
       {                        /*  set indicator for attr. exists in entry*/
	    attr_found = TRUE;
				/*  set msg_ptr to next information block  */
	    offset = ((D23_at_type *) msg_ptr)->d23_atoff;
	    msg_ptr = msg_ptr + sizeof (D23_at_type) + offset;
       }                        /*  attr. type in DS_SEARCH ... */
  }                             /*  msg_ptr points ...                     */

  /****************************************/
  /* attribute existence for modification */
  /****************************************/
  switch ((signed32) modification){  /*  modification type                 */
    case  D2_ADD_AV:            /*  ADD value                              */
    case  D2_REP_AV:            /*  REPLACE value                          */
	  if ((add_type =
	       d26_C05_find_attr (&add_a_types[0], &type, (signed16 *) NULL,
		    (char *) NULL, at, (signed32) D26_NOTHING,
		    TRUE, &p_add_types, error, l_err, entryname))
		== D26_ERROR)
	  {   
   	       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 22));
	       return (D26_ERROR);
	  } 

	  if ((del_type =
	       d26_C05_find_attr (&del_a_types[0], &type, (signed16 *) NULL,
		    (char *) NULL, at, (signed32) D26_NOTHING,
		    TRUE, &p_del_types, error, l_err, entryname))
		 == D26_ERROR)
	  {   
   	       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 23));
	       return (D26_ERROR);
	  } 

	  if                    /*  attribute doesn't exist in entry and   */
				/*  isn't added former or attribute is     */
				/*  deleted former                         */
	     (((attr_found == FALSE) && (add_type == FALSE))
	     || (del_type == TRUE))
	  {                     /*  error: attr. doesn't exist             */
	       d26_u15_attr_error ((byte **) error,
				   exact_name, 1, D2_ATTRIBUTE_MISSING,
				   &type,
				   rep_ai, rep_av,
				   (signed16) 0, (byte *) NULL,
				   l_err, &apdu_len);
	       *l_err = apdu_len;

   		DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 24));
	       return(D26_ERROR);
	  }                     /*  attr. doesn't exist ... */

	  if (modification == D2_ADD_AV)
	  {   
		break;
	  } 

	  /* here continues case D2_REP_AV */
	  if                    /*  old value is given                     */
	     ((ma = at_str->d26_mold) != NULL)
	  {                     /*                                         */
	       if ((del_value =
		    d26_C05_find_attr (&del_a_values[0], &type,
			 &(ma->d23_avlen), (val=(char *)(ma+1)),
			 at, D2_ADDATT, TRUE, &p_del_values,
			 error, l_err, entryname)) == D26_ERROR)
	       {   
   	            DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 25));
		    return (D26_ERROR);
	       } 

	       length = ma->d23_avlen;
	       if               /*  old value is deleted former            */
		  (del_value == TRUE)
	       {                /*  error: attr. doesn't exist             */
		    d26_u15_attr_error ((byte **) error,
				   exact_name, 1, D2_ATTRIBUTE_MISSING,
				   &type,
				   rep_ai, ma->d23_avrep,
				   length, (byte *) val,
				   l_err, &apdu_len);
		    *l_err = apdu_len;

   		    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 26));
		    return(D26_ERROR);
	       }                /*  old value is deleted former */
	  }                     /*  old value is given                     */
	  break;

    case  D2_ADDATT:            /*  ADD type                               */
	  if ((add_type =
	       d26_C05_find_attr (&add_a_types[0], &type, (signed16 *) NULL,
		    (char *) NULL, at, D2_ADDATT, TRUE,
		    &p_add_types, error, l_err, entryname)) == D26_ERROR)
	  {   
   	       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 27));
	       return (D26_ERROR);
	  } 

	  if                    /*  attribute exists in entry or attribute */
				/*  is added former                        */
	     ((attr_found == TRUE) || (add_type == TRUE))
	  {                     /*  check whether type was deleted prior   */
	       if ((del_type =
		       d26_C05_find_attr (&del_a_types[0], &type, (signed16 *)
		       NULL, (char *) NULL, at, D2_DELATT,
		       TRUE, &p_del_types, error, l_err, entryname))
		     == D26_ERROR)
	       {   
   		    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 28));
		    return (D26_ERROR);
	       } 

	       if (del_type == FALSE)
	       {                     /*  error: attr. already exists       */
		    d26_u15_attr_error ((byte **) error,
				   exact_name, 1, D2_AT_VALUE_EXISTS,
				   &type,
				   rep_ai, rep_av,
				   (signed16) NULL, (byte *) NULL,
				   l_err, &apdu_len);
		    *l_err = apdu_len;

   		    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 29));
		    return(D26_ERROR);
	       }                     /*  attr. already exists ... */
	  }                     /*  check whether attribute was deleted prior */
	  break;

    case  D2_DEL_AV:            /*  DELETE value                           */
	  if                    /*  old value is given                     */
	     ((ma = at_str->d26_mold) != NULL)
	  {                     /*                                         */
	       if ((del_value =
		    d26_C05_find_attr (&del_a_values[0], &type,
			 &(ma->d23_avlen), (val=(char *)(ma+1)),
			 at, D2_ADDATT, TRUE, &p_del_values,
			 error, l_err, entryname)) == D26_ERROR)
	       {   
   		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 30));
		    return (D26_ERROR);
	       } 

	       length = ma->d23_avlen;
	       if               /*  old value is deleted former            */
		  (del_value == TRUE)
	       {                /*  error: was deleted former              */
		    d26_u15_attr_error ((byte **) error,
				   exact_name, 1, D2_ATTRIBUTE_MISSING,
				   &type,
				   rep_ai, ma->d23_avrep,
				   length, (byte *) val,
				   l_err, &apdu_len);
		    *l_err = apdu_len;

   		    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 31));
		    return(D26_ERROR);
	       }                /*  old value is deleted former */
	  }                     /*  old value is given                     */
	/* FALLTHROUGH */

    case  D2_DELATT:            /*  DELETE type                            */
				/*  DELETE type + DELETE value             */
	  if (modification == D2_DELATT)
	  {   
	       app = D2_ADDATT;
	  } else {
	       app = D26_NOTHING;
	  } 

	  if ((del_type =
	       d26_C05_find_attr (&del_a_types[0], &type, (signed16 *) NULL,
		    (char *) NULL, at, app, TRUE,
		    &p_del_types, error, l_err, entryname)) == D26_ERROR)
	  {   
   	       DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 32));
	       return (D26_ERROR);
	  } 

	  if                    /*  attribute doesn't exist in entry or    */
				/*  attribute is deleted former            */
	     ((attr_found == FALSE) || (del_type == TRUE))
	  {                     /*  check whether attr. was added prior    */
	       if (modification == D2_DELATT)
	       {    /* check whether attribute was added prior */
		    if ((add_type =
			   d26_C05_find_attr (&add_a_types[0], &type,
			   (signed16 *) NULL, (char *) NULL,
			   at, D2_DELATT, TRUE, &p_add_types,
			   error, l_err, entryname)) == D26_ERROR)
		    {   
   			DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				D26_ERROR - 33));
			 return (D26_ERROR);
		    } 
	       } else { /* error: attr. doesn't exist */
		    add_type = FALSE;
	       }  /* check whether attribute was added prior */

	       if (add_type == FALSE)
	       {    /* error: attr. doesn't exist */
		    d26_u15_attr_error ((byte **) error,
				   exact_name, 1, D2_ATTRIBUTE_MISSING,
				   &type,
				   rep_ai, rep_av,
				   (signed16) 0, (byte *) NULL,
				   l_err, &apdu_len);
		    *l_err = apdu_len;

   		    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 34));
		    return(D26_ERROR);
	       }    /* attr. doesn't exist ... */
	  }                     /*  check whether attr. was added prior */
	  break;

    default:            /* otherwise:                    */
			/* return (D26_ERROR)            */
		  d26_u15_attr_error ((byte **) error,
				      exact_name, 1, D2_CONSTRAINT_VIOLATION,
				      &type,
				      rep_ai, rep_av,
				      (signed16) NULL, (byte *) NULL,
				      l_err, &apdu_len);
		  *l_err = apdu_len;

   		  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			D26_ERROR - 35));
		  return(D26_ERROR);
  }                             /*                                */

  /********************************************/
  /* check whether new value is already added */
  /********************************************/
  /* Note that existing values in record are checked by internal search */
  if ((modification == D2_REP_AV) || (modification == D2_ADD_AV)
       || (modification == D2_ADDATT))
  {    /* check for new value */
       if                    /*  new value is given                     */
	  ((ma = at_str->d26_mnew) != NULL)
       {                     /*                                         */
	    if ((add_value =
		 d26_C05_find_attr (&add_a_values[0], &type,
		      &(ma->d23_avlen), (val=(char *)(ma+1)),
		      at, D2_ADDATT, FALSE, &p_add_values,
		      error, l_err, entryname)) == D26_ERROR)
	    {   
   		DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 36));
		 return (D26_ERROR);
	    } 

	    length = ma->d23_avlen;
	    if               /*  new value is added former              */
	       (add_value == TRUE)
	    {                /*  error: attr. value already exists      */
		 d26_u15_attr_error ((byte **) error,
				exact_name, 1, D2_AT_VALUE_EXISTS,
				&type,
				rep_ai, ma->d23_avrep,
				length, (byte *) val,
				l_err, &apdu_len);
		 *l_err = apdu_len;

   		 DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 37));
		 return(D26_ERROR);
	    }                /*  new value is added former */
       }                     /*  new value is given                     */
  }  /* check for new value */

} 

/**********************************************/
/* check for deletion of mandatory attributes */
/**********************************************/
ret_value = d26_C15_mand_attr_check (&del_a_types[0], &p_del_types,
				     error, l_err, entryname,
				     ob_cl, alias_flag);

			/*  return (return value)         */
DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
return (ret_value);

}                                   /*  d26_C00_mod_attr_check()          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C01_analyse_modentry_msg                          */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        02.02.88                                              */
/*                                                                    */
/* SYNTAX:      LOCAL Short d26_C01_analyse_modentry_msg (mod_in,     */
/*                                                   mod_attr,        */
/*                                                   entryname,       */
/*                                                   l_entryname,     */
/*                                                   no_of_attributes,*/
/*                                                   dap_mess,        */
/*                                                   error, l_err)    */
/*                                                                    */
/*              D23_inmodentr   *mod_in;             ->               */
/*              D26_md_info     *mod_attr;           <-               */
/*              char            **entryname;         <-               */
/*              Ushort          *l_entryname;        <-               */
/*              Short           *no_of_attributes;   <-               */
/*              String          *dap_mess;           <-               */
/*              Octet           **error;             <>               */
/*              Int             *l_err;              <>               */
/*                                                                    */
/* DESCRIPTION: This function analyses the IPC message received by    */
/*              the function DS_MODIFY_ENTRY. A pointer to the given  */
/*              entry name, the entry name's length and a pointer     */
/*              array to the attribute modification informations are  */
/*              returned.                                             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D23_inmodentr  *mod_in        pointer to DS_MODIFY_ENTRY      */
/*                                    invoke message                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      D26_md_info    *mod_attr      pointer to attribute            */
/*                                    modification informations       */
/*                                    contained in message            */
/*      char           **entryname    pointer to entryname contained  */
/*                                    in message                      */
/*      Ushort         *l_entryname   length of entryname             */
/*      Short      *no_of_attributes  no of attributes which should   */
/*                                    be modified                     */
/*      Octet          **error;       error message                   */
/*                                    (contains return_codes)         */
/*      Int             *l_err;       length of error message         */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Short   D2_NO_ERR      no error occurred                      */
/*              D26_ERROR      DS_MODIFY_ENTRY invoke message not     */
/*                             correct                                */
/*                             (error apdu and length of error apdu   */
/*                             are set)                               */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*             d26_u15_attr_error()                                   */
/*             d27_010_set_error_apdu()                               */
/*                                                                    */
/*             sprintf                                                */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/


static signed16 d26_C01_analyse_modentry_msg (D23_inmodentr   *mod_in,
					  D26_md_info     *mod_attr,
					  char           **entryname,
					  signed16        *l_entryname,
					  signed16        *no_of_attributes,
					  byte           **dap_mess,
					  byte           **error,
					  signed32        *l_err
					 )

{                               /*  d26_C01_analyse_modentry_msg       */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

static const char    function[]  = "d26_C01_analyse_modentry_msg";

/* AUTO      ...    */
D2_a_type  type, err_type, cmp_old_type, cmp_new_type;
signed32   apdu_len = 0;
byte        *ptr_msg;     /* pointer to message */

/* FASTAUTO  ...    */
register signed16    i, j;           /* counter */
register unsigned16  tag;         /* for check of tag   */
register D26_md_info *ptr_mod_attr, *old_ptr_mod_attr;
register signed32    novals;
register signed32    mod;
register unsigned16  rep;
register signed32    error_flag = FALSE;
register signed32    max_no_exceeded = FALSE;
register boolean     wrong_msg = FALSE;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/************************/
/* initialize variables */
/************************/

                               /*  analyse message                    */
*no_of_attributes = 0;        /*  initialize no of attr. to change   */
ptr_msg = (byte *) (mod_in + 1);    /*  set pointer to first tag           */


/******************/
/* find entryname */
/******************/

/* skip D23_chain_arg */
d26_a02_analyse_chaining_args (&ptr_msg);
*dap_mess = ptr_msg;

/* skip D23_com_arg */
d26_a03_analyse_common_arguments (&ptr_msg); /* the function updates    */
					     /* the global varaiable    */
					     /* d26_serv_contr          */

				    /*  set ptr to entryname in message    */
				    /*  (OUT_PAR: entryname)               */
*entryname = (char *) (ptr_msg + sizeof (D23_ds_name));
				    /*  set length of entryname            */
				    /*  (OUT_PAR: l_entryname)             */
*l_entryname = ((D23_ds_name *)ptr_msg)->d23_dslen;
				    /*  scan message (set ptr_msg)         */

ptr_msg = (byte *) (*entryname + ((D23_ds_name *)ptr_msg)->d23_dsoff);

/***********************************/
/* find all attributemodifications */
/***********************************/

for                                 /*  all attribute modifications        */
    (i = 0, tag = ((D23_md_info*)ptr_msg)->d23_mdtag,
     ptr_mod_attr = mod_attr;
     ((i < D2_AT_MAX) && (tag != D23_EOMTAG) && (error_flag == FALSE));
     i++, ptr_mod_attr++)
{                                   /*                                     */
   if                               /*  D23_MODTAG not found               */
      (tag != D23_MODTAG)
   {                                /*  MA-LOG + MA-ERROR                  */
	wrong_msg = TRUE;
	break;
   }                                /*  D23_MODTAG not found */


				    /*  set attr. modification informations*/
				    /*  (OUT_PAR: mod_attr)                */
   ptr_mod_attr->d26_m    = mod = (int) (((D23_md_info *) ptr_msg)->d23_mdmod);
   ptr_mod_attr->d26_mnew = (D23_av_value *) NULL;
   ptr_mod_attr->d26_mold = (D23_av_value *) NULL;

   ptr_msg = ptr_msg + sizeof (D23_md_info);
				    /*  set next tag                       */
   tag = ((D23_at_type *)ptr_msg)->d23_atttag;
   if                               /*  D23_ATTTAG not found               */
      (tag != D23_ATTTAG)
   {                                /*  MA-LOG + MA-ERROR                  */
	wrong_msg = TRUE;
	break;
   }                                /*  D23_MODTAG not found */
				    /* get attribute type                  */
   ptr_mod_attr->d26_mtype.d2_typ_len = type.d2_typ_len
			       = (int) (((D23_at_type *)ptr_msg)->d23_atlen);
   ptr_mod_attr->d26_mtype.d2_type    = type.d2_type
			       = (Ob_id_string) (ptr_msg + D23_S_ATT);

   /* check wether modification is allowed */

   /* if type = objectclass */
   if (!D2_ATTCMP(&(ptr_mod_attr->d26_mtype),D2_OBJ_CLASS))
   {
     d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR,
				D2_PAR_ERR, D2_OBJCLS_MOD_PROHIBITED,l_err);
/* GDS_S_ATTR_ERR ("Modification of object class not allowed","85.4.0") */
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 11));
     return(D26_ERROR);
   }

				   /*  set ptr_msg to attribute values    */
   ptr_msg = ptr_msg + ((D23_at_type *)ptr_msg)->d23_atoff + D23_S_ATT;
   tag = ((D23_av_info *)ptr_msg)->d23_avtag;

   /* when the modification is D2_DELATT there will be no values */
   /* ---> continue loop                                         */
   if (mod == D2_DELATT)
   {    /* there will be no values */
	continue;
   } 

   /******************/
   /* get all values */
   /******************/
   if                               /*  D23_AVTAG not found               */
      (tag != D23_AVTAG)
   {                                /*  MA-LOG + MA-ERROR                  */
	wrong_msg = TRUE;
	break;
   }                                /*  D23_AVTAG not found */

   rep = ((D23_av_info *) ptr_msg)->d23_asyntx; /* representation */
   novals = (int) (((D23_av_info *) ptr_msg)->d23_avnum);
   if ((novals <= 0) || ((mod == D2_REP_AV) && ((novals % 2) != 0)))
   {                                /*  MA-LOG + MA-ERROR                  */
				    /* attribute error no value given */
	error_flag = TRUE;
	err_type = type;
	d26_u15_attr_error ((byte **) error, (D2_name_string) *entryname, 1,
			    D2_CONSTRAINT_VIOLATION, &err_type,
			    ((D23_av_info *) ptr_msg)->d23_asyntx,
			    ((D23_av_info *) ptr_msg)->d23_asyntx,
			    (signed16) NULL, (byte *) NULL, l_err,
			    &apdu_len);
	*l_err = apdu_len;
	break;
   }                                /*  novals <= 0 || ... */

   ptr_msg = ptr_msg + D23_S_AV;
   i--; /* in other case one value is encountered twice ! */
   /* give 1 value modified only for 1 attribute type */
   for /* all values */
       (j = 0;
	(((i+j) < D2_AT_MAX) && (j < novals) && (error_flag == FALSE));
	j++, ptr_mod_attr++)
   {  /* set 1 value for 1 attribute */
      if ((j > 0) && (mod == D2_ADDATT))
      {    /* next modification should be D2_ADD_AV */
	   mod = D2_ADD_AV;
      } 
      ptr_mod_attr->d26_m                = mod;
      ptr_mod_attr->d26_mtype.d2_typ_len = type.d2_typ_len;
      ptr_mod_attr->d26_mtype.d2_type    = type.d2_type;
      ptr_mod_attr->d26_mrep             = rep;
      ptr_mod_attr->d26_mnew = (D23_av_value *) NULL;
      ptr_mod_attr->d26_mold = (D23_av_value *) NULL;

      /* get last modification (old_ptr_mod_attr) */
      if ((i+j) >= 0)
      {    /* last modification is prior modification */
	   old_ptr_mod_attr = ptr_mod_attr - 1;
      } else { /* last modification is actual modification (first modification) */
	   old_ptr_mod_attr = ptr_mod_attr;
      } 

      /* check for replace value */
      cmp_old_type = old_ptr_mod_attr->d26_mtype;
      cmp_new_type = ptr_mod_attr->d26_mtype;
      if ( (D27_ATTCMP (&cmp_old_type, &cmp_new_type) == 0)
	   && (   ((old_ptr_mod_attr->d26_m == D2_DEL_AV) &&
		   (ptr_mod_attr->d26_m == D2_ADD_AV))
	       || ( (old_ptr_mod_attr->d26_m == D2_ADD_AV) &&
		    (ptr_mod_attr->d26_m == D2_DEL_AV)    &&
		    ( (old_ptr_mod_attr->d26_mnew->d23_avlen !=
		       ((D23_av_value *) ptr_msg)->d23_avlen) ||
		      (memcmp (
			     (Octet_string) (old_ptr_mod_attr->d26_mnew + 1),
			       (Octet_string) (ptr_msg + D23_S_ATV),
			       old_ptr_mod_attr->d26_mnew->d23_avlen) != 0)
		    ))))
      {    /* set replace value */
	   ptr_mod_attr--;
	   ptr_mod_attr->d26_m = D2_REP_AV;
	   i--;
      }  /* check for replace value */

      /* set value */
      switch (mod) {
      case D2_DEL_AV:
	   ptr_mod_attr->d26_mold = (D23_av_value *) ptr_msg;
	   break;

      case D2_ADDATT:
      case D2_ADD_AV:
	   ptr_mod_attr->d26_mnew = (D23_av_value *) ptr_msg;
	   break;

      case D2_REP_AV:
	   /* first value is old value */
	   ptr_mod_attr->d26_mold = (D23_av_value *) ptr_msg;
	   /* next value is new value */
	   ptr_msg = ptr_msg + D23_S_ATV + ((D23_av_value *)ptr_msg)->d23_avoff;
	   ptr_mod_attr->d26_mnew = (D23_av_value *) ptr_msg;
	   /* two values handled */
	   j++;
	   i--;
	   break;

      default:
	   error_flag = TRUE;
	   d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR, 
				   D2_PAR_ERR, D2_NOT_SPEC, l_err);
	   break;
      }    /* switch (mod) */

      /* next value */
      ptr_msg = ptr_msg + D23_S_ATV + ((D23_av_value *)ptr_msg)->d23_avoff;

   }  /* for all values */
   i = i + j;      /* set total no. of modified attributes */
   if (i == D2_AT_MAX)
   {    /* max. number of modifications excceeded */
	max_no_exceeded = TRUE;
   } 
   ptr_mod_attr--; /* set pointer back to last modification        */
		   /* otherwhise this pointer is incremented twice */
   /* set next tag */
   tag = ((D23_av_info *)ptr_msg)->d23_avtag;

}                                   /*  all attribute modifications */

*no_of_attributes = i;              /*  (OUT-PAR: no. of modifications)    */

				    /*  set last values of mod_attr        */
				    /*  indicating the end of list         */
ptr_mod_attr->d26_m    = (int) D26_EOL;
ptr_mod_attr->d26_mold = (D23_av_value *) NULL;
ptr_mod_attr->d26_mnew = (D23_av_value *) NULL;

/***************************/
/* look for end of message */
/***************************/

if (error_flag == TRUE)
{   
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 12));
     return (D26_ERROR);
} 

if                                  /*  end of message not reached         */
   ((tag != D23_EOMTAG) || (wrong_msg == TRUE))
{                                   /*  message wrong                      */
				    /*  MA-LOG + MA-ERROR                  */
				    /*  set error apdu                     */
     if (((*no_of_attributes) >= D2_AT_MAX) &&
	 ((tag == D23_MODTAG) || (max_no_exceeded == TRUE)))
     {   
				    /* set error apdu                     */
	  d27_010_set_error_apdu ((D23_rserror *)*error,D2_CALL_ERR,
				   D2_PAR_ERR,D2_ADM_LIM_EXCEEDED,l_err);
     } else {
				    /* set error apdu                     */
	  d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR, 
				   D2_PAR_ERR, D2_NOT_SPEC, l_err);
     } 

     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 13));
     return (D26_ERROR);
}                                   /*  message wrong */


DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NO_ERR));
return (D2_NO_ERR);
}                               /*  d26_C01_analyse_modentry_msg      */




/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C02_set_search_par                                */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        08.02.88                                              */
/*                                                                    */
/* SYNTAX:      LOCAL Short d26_C02_set_search_par (mod_in,           */
/*                                             mod_attrs,no_of_attrs, */
/*                                             in_s, error,l_err,     */
/*                                             serv_ctrls)            */
/*                                                                    */
/*              D23_inmodentr    *mod_in;          ->                 */
/*              D26_md_info      *mod_attrs;       ->                 */
/*              Short            no_of_attrs;      ->                 */
/*              D23_insearch     **in_s;           <-                 */
/*              Octet            **error;          <>                 */
/*              Int              *l_err;           <>                 */
/*              Long              serv_ctrls;      ->                 */
/*                                                                    */
/* DESCRIPTION: This function builds the DS_SEARCH invoke message     */
/*              for the internal DS_SEARCH function.                  */
/*              The attribute types which should be modified are      */
/*              requested and the old value of values which should    */
/*              be replaced are put in message as filterattributes.   */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*     D23_inmodentr *mod_in           DS_MODIFY_ENTRY invoke message */
/*                                                                    */
/*     D26_md_info   *mod_attrs        informations of attributes     */
/*                                     which should be modified       */
/*                                                                    */
/*     Short         no_of_attrs       no. of attributes which should */
/*                                     be modified                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*     D23_insearch  **in_s            invoke message for internal    */
/*                                     DS_SEARCH                      */
/*                                     the memory is allocated by this*/
/*                                     function                       */
/*                                     the caller must free this      */
/*                                     memory                         */
/*                                                                    */
/*     Octet         **error            error message (contains return */
/*                                     codes in case of errors)       */
/*                                                                    */
/*     Ushort        *l_err            length of error message        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short    D2_NO_ERR    no error occurred                         */
/*                                                                    */
/*             D26_ERROR    any error occurred                        */
/*                          (e.g. not enough memory for DS_SEARCH     */
/*                          invoke message, error apdu and length     */
/*                          of error apdu are set)                    */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                  D26_at     *d26_at;         Attribute-Description-*/
/*                                              Table                 */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        malloc                                                      */
/*                                                                    */
/*        d26_C03_check_search_memsize                                */
/*        d26_C05_find_attr                                           */
/*                                                                    */
/*        d26_u03_attr_search                                         */
/*                                                                    */
/*        d27_007_norm_name                                           */
/*        d27_010_set_error_apdu                                      */
/*        d27_012_remove_blanks                                       */
/*        d27_018_sort_name                                           */
/*                                                                    */
/*        MA2_002_ERROR                                               */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C02_set_search_par (D23_inmodentr   *mod_in,
				    D26_md_info     *mod_attrs,
				    signed16         no_of_attrs,
				    D23_insearch   **in_s,
				    byte           **error,
				    signed32        *l_err,
				    signed32         serv_ctrls
				   )

{                               /*  d26_C02_set_search_par            */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
char       *begin_mem,        /* begin of allocated memory for        */
				   /* DS_SEARCH invoke message             */
		*end_mem,          /* end of allocated memory for DS_SEARCH*/
				   /* invoke message                       */
		*act_pos,          /* actual position of DS_SEARCH invoke  */
				   /* message                              */
		*no_items = (char *) NULL;
				   /* ptr to memory of no of items */

signed16	no_of_memblocks;   /* number of allocated memory block     */
				   /* size of allocated memory =           */
				   /*  no_of_memblocks * D26_C01_SEARCH_MEM*/

signed32        use_mem;           /* size of memory which is needed for   */
				   /* data in DS_SEARCH message            */

signed16        ret_val;           /* return value of called functions     */
D26_md_info *md_info_ptr;          /* ptr to modification information      */

				    /* attribute types to be deleted       */
D26_CT0_inf_attrs  del_a_types [D2_AT_MAX + 1];  /* memory            */
D26_CT0_inf_attrs  *p_del_types;                 /* start of list     */
				    /* attribute values to be deleted      */
D26_CT0_inf_attrs  del_a_values [D2_AT_MAX + 1]; /* memory            */
D26_CT0_inf_attrs  *p_del_values;                /* start of list     */
signed16        del_type;       /* return value from d26_C05_find_attr */
signed16        del_value;      /* return value from d26_C05_find_attr */
signed16        dummy_val;
signed16        at_idx;         /* index in AT                    */
D26_at         *at_entry;       /* entry in AT                    */

D23_av_value  *old_val;
D23_av_value  *new_val;
D23_av_value  *val;
char          *mi_ptr;         /* pointer to mod_in msg               */
D23_ds_name   *ds_in_name;     /* pointer to distinguished name       */
char          name[D2_DNL_MAX + 1];/* normed and sorted entry name    */
signed16      *add_or_del_at;  /* table parallel to AT to set flags   */
				    /* for DEL_ATT und ADD_ATT             */
/* FASTAUTO  ...    */
register signed16      i;              /* counter                          */
register char          *val_ptr;       /* pointer to attr value            */
register D23_at_type   *attr_type;
register D23_av_value  *attr_val;
register D23_com_arg   *com_arg_ptr;   /* pointer to common arguments      */
register D23_srcharg   *srcharg_ptr;   /* pointer to search information    */
register D23_av_value  *nm_enc;        /* still encoded name part        */

register D23_rq_info   *rq_info_ptr;   /* pointer to requested information  */
register D23_ft1_info  *ft_info_ptr;   /* pointer to filter information     */
register D23_fi1_info  *fi_info_ptr;   /* pointer to filter item information*/
register signed16      no_ands = 0;    /* number of AND filter items        */
register boolean       first_val = TRUE; /* indicates first filter value */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/*******************/
/* allocate memory */
/*******************/
                        /*  allocate memory for DS_SEARCH invoke msg  */
if                           /*  no memory available                       */
   ((begin_mem = (char *) D27_MALLOC(svc_c_sev_warning, D26_C01_SEARCH_MEM))
     == NULL)
{                           
			     /*  set error apdu and length                 */
     d27_010_set_error_apdu ((D23_rserror *) *error, D2_TEMP_ERR, D2_REM_ERR,
			      D2_UNWILLING, l_err);
     return (D26_ERROR);     /*  exit function                             */
}                            /*  malloc fails */

if                           /*  no memory available                       */
   ((add_or_del_at = (signed16 *) D27_CALLOC(svc_c_sev_warning, d26_noat, 
					   sizeof(signed16))) == NULL)
{                           
			     /*  set error apdu and length                 */
     d27_010_set_error_apdu ((D23_rserror *) *error, D2_TEMP_ERR, D2_REM_ERR,
			      D2_UNWILLING, l_err);
     return (D26_ERROR);     /*  exit function                             */
}                            /*  calloc fails */

			     /*  set variables                             */
*in_s = (D23_insearch *) begin_mem;
no_of_memblocks = 1;
end_mem = begin_mem + D26_C01_SEARCH_MEM;

/* initialize lists which are used to build the logical query for search */
for (i=0; i<D2_AT_MAX; i++)
{ 
   p_del_types  = &del_a_types[i];
   p_del_values = &del_a_values[i];
   p_del_types->next_attr =
     p_del_values->next_attr = (D26_CT0_inf_attrs *) NULL;
   p_del_types->attr_type.d2_type =
     p_del_values->attr_type.d2_type = (Ob_id_string) NULL;
   p_del_types->attr_type.d2_typ_len = p_del_values->attr_type.d2_typ_len = 0;
   p_del_types->val_length = p_del_values->val_length = 0;
   p_del_types->attr_value = p_del_values->attr_value = (char *) NULL;
} 
/* initialize start of list */
p_del_types  = (D26_CT0_inf_attrs *) NULL;
p_del_values = (D26_CT0_inf_attrs *) NULL;


/********************************************/
/* fill message header and common arguments */
/********************************************/
                        /*  fill message header                       */
(*in_s)->d23_Ihdr.d23_versno = mod_in->d23_Nhdr.d23_versno;
(*in_s)->d23_Ihdr.d23_dirid  = mod_in->d23_Nhdr.d23_dirid;
(*in_s)->d23_Ihdr.d23_bindid = mod_in->d23_Nhdr.d23_bindid;

act_pos = (char *) ((*in_s) + 1);
mi_ptr = (char *) (mod_in + 1);
				    /*  tag is Chaining arguments     */
d26_a02_analyse_chaining_args((byte **)&mi_ptr);
d26_a03_analyse_common_arguments((byte **)&mi_ptr);
ds_in_name = (D23_ds_name *) mi_ptr;
d27_007_norm_name((String)(ds_in_name + 1),ds_in_name->d23_dslen,
		  name);
d27_012_remove_blanks(name,ds_in_name->d23_dslen,name);
d27_018_sort_name((D2_name_string)name);

			     /*  set common argumemts                      */
com_arg_ptr = (D23_com_arg *) act_pos;
com_arg_ptr->d23_cmtag = D23_CMTAG;
com_arg_ptr->d23_cmali = D2_NOT_DEFINED;
com_arg_ptr->d23_cmctr.d23_scopt = serv_ctrls;
com_arg_ptr->d23_cmctr.d23_scpri = 0;            /* priority (not used)     */
com_arg_ptr->d23_cmctr.d23_sctlm = D2_T_UNLIMITED; /* no time limit         */
com_arg_ptr->d23_cmctr.d23_scslm = 1;            /* size limit: return 1 obj*/
com_arg_ptr->d23_cmctr.d23_scref = D2_WORLD_SCOPE;
com_arg_ptr->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED;

			     /*  set search information                    */
srcharg_ptr = (D23_srcharg *) (com_arg_ptr + 1);
srcharg_ptr->d23_Itag = D23_ITAG;
srcharg_ptr->d23_Iali = FALSE;
srcharg_ptr->d23_Iobjsub = D2_BASE_OBJECT;
                         /* */


/******************/
/* set entry name */
/******************/
				/*  set actual message pointer        */
act_pos = (String)(srcharg_ptr + 1);
				/*  calculate needed memory size:     */
use_mem = sizeof(D23_ds_name) + ds_in_name->d23_dsoff;
if                              /* name not completely decoded        */
  (ds_in_name->d23_dscomplete == FALSE)
{                               /* set attribute value pointer        */
  nm_enc = (D23_av_value *)((String)ds_in_name + use_mem);
  use_mem += sizeof(D23_av_value) + nm_enc->d23_avoff;
}
use_mem += sizeof (D23_rq_info) + 1;
				  /*  check memory size                    */
ret_val = d26_C03_check_search_memsize (use_mem, &begin_mem, (char **) in_s,
					&act_pos, &end_mem, &no_items,
					&no_of_memblocks, error, l_err);
if (ret_val != D2_NO_ERR)       /*  not enough memory                    */
{                               /*  exit function                        */
   return (D26_ERROR);
}                               /*  not enough memory */
				/* copy complete distinguished name   */
d27_502_copy_distinguished_name((byte **)&ds_in_name,(byte **)&act_pos);

/*****************************/
/* set requested information */
/*****************************/
			       /*  set ptr to begin of requested inf.   */
rq_info_ptr = (D23_rq_info *) act_pos;
			       /*  copy requested inf: D23_rq_info      */
rq_info_ptr->d23_rqtag = D23_REQTAG;  /* tag                            */
rq_info_ptr->d23_rqreq = D2_R_TYP;    /* attribute type requested       */
rq_info_ptr->d23_rqnbr = no_of_attrs; /* number of requested attributes */

				  /*  set ptr to begin requested attributes*/
act_pos = (char *) rq_info_ptr + sizeof (D23_rq_info);
				  /*  calculate needed memory size         */
use_mem = ((no_of_attrs + 1) * D23_S_ATT) +
	  ((no_of_attrs + 1) * (D2_ALIGN(D2_OBJ_IDL_MAX))) +
	  D23_S_FT1 + D23_S_FT1 + D23_S_FT1 + D23_S_FI1 + D23_S_ATV + 1;
				  /*  check memory size                    */
ret_val = d26_C03_check_search_memsize (use_mem, &begin_mem, (char **) in_s,
					&act_pos, &end_mem, &no_items,
					&no_of_memblocks, error, l_err);
if (ret_val != D2_NO_ERR)         /*  not enough memory                    */
{                                 /*  exit function                        */
     return (D26_ERROR);
}                                 /*  not enough memory */

  /* copy requested attr types to message  */

		      /* if there is more than one modification to one   */
		      /* attribute type this type will be copied several */
		      /* times to the search message. This is not an     */
		      /* error because in the following functions (C04)  */
		      /* it is expected that the attributes in the search*/
		      /* result (structure attr_info) have the same      */
		      /* sequence as in the modify request (mod_attr).   */

for                            /*  all attributes which should be changed*/
    (i = 0, md_info_ptr = mod_attrs;
     i < no_of_attrs;
     i++, md_info_ptr++)
{                                 /*  copy attr type to requ attr in msg   */
   attr_type = (D23_at_type *) act_pos;
   attr_type->d23_atttag = D23_ATTTAG;
   attr_type->d23_atoff  = D2_ALIGN (md_info_ptr->d26_mtype.d2_typ_len);
   attr_type->d23_atlen  = md_info_ptr->d26_mtype.d2_typ_len;
   act_pos = (char *) (attr_type + 1);
   memcpy ((Octet_string) act_pos,
		    (Octet_string) (md_info_ptr->d26_mtype.d2_type),
		    attr_type->d23_atlen);
   act_pos = act_pos + attr_type->d23_atoff;
				  /* check for DEL_ATT or ADD_ATT          */
   if
      (md_info_ptr->d26_m == D2_DELATT || md_info_ptr->d26_m == D2_ADDATT)
   {
     at_idx = d26_u03_attr_search(&(md_info_ptr->d26_mtype));
     if
	(at_idx != D26_ERROR) /* in other case search generates an error */
     {
       add_or_del_at[at_idx] = TRUE;
       if(md_info_ptr->d26_m == D2_DELATT &&
	  (d26_C16_name_attr_check((D2_name_string)name,d26_at + at_idx,
	   md_info_ptr) == D26_ERROR))
       { /* no check wether type is added later with same value */
	 d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR,
				 D2_PAR_ERR, D2_NOT_ALLOWED_ON_RDN, l_err);
	 return(D26_ERROR);
       }
     }
   }
}                                 /*  all attr. ... */

/**************************/
/* set filter information */
/**************************/
			     /*  filter information                        */
first_val = TRUE;
for                               /* all attributes which should be changed*/
    (i = 0, md_info_ptr = mod_attrs, no_ands = 0;
     i < no_of_attrs;
     i++, md_info_ptr++)
{                                 /* */
   /*************************************************************/
   /* check whether filter must be set for old and or new value */
   /*************************************************************/
   old_val = new_val = (D23_av_value *) 0;
   del_type = del_value = FALSE;
   /* check whether a value should be deleted */
   if (((md_info_ptr->d26_m == D2_DEL_AV)
	|| (md_info_ptr->d26_m == D2_REP_AV))
       && (md_info_ptr->d26_mold != (D23_av_value *) 0))
   {    /* append old value to list of values to be deleted */
	/* get index in AT for current attribute */
	at_idx = d26_u03_attr_search (&(md_info_ptr->d26_mtype));
	if (at_idx != D26_ERROR)
	{    /* append old value to list of values to be deleted */
	     at_entry = &d26_at[at_idx];
	     del_value = d26_C05_find_attr (&del_a_values[0],
			     &(md_info_ptr->d26_mtype),
			     &(md_info_ptr->d26_mold->d23_avlen),
			     (char *)((md_info_ptr->d26_mold)+1),
			     at_entry, D2_ADDATT, TRUE, &p_del_values,
			     (byte **) 0, (signed32 *) 0, (D2_name_string) 0);
	}  /* at_idx != D26_ERROR */
	/* set old value only, if it was not found in list */
	/* it must be checked in search whether  */
	/* the old value exists                  */
	if (del_value == FALSE)
	{   
		old_val = md_info_ptr->d26_mold;
	} 
   }  /* d26_m == D2_DEL_AV or D2_REP_AV */

   /* check whether the type should be deleted */
   if (md_info_ptr->d26_m == D2_DELATT)
   {    /* append type to list of types to be deleted */
	/* get index in AT for current attribute */
	at_idx = d26_u03_attr_search (&(md_info_ptr->d26_mtype));
	if (at_idx != D26_ERROR)
	{    /* append old type to list of types to be deleted */
	     at_entry = &d26_at[at_idx];
	     dummy_val = d26_C05_find_attr (&del_a_types[0],
			     &(md_info_ptr->d26_mtype),
			     (signed16 *) NULL, (char *) NULL,
			     at_entry, D2_ADDATT, TRUE, &p_del_types,
			     (byte **) 0, (signed32 *) 0, (D2_name_string) 0);
	}  /* at_idx != D26_ERROR */
   }  /* d26_m == D2_DELATT */

   /* check whether a value should be added */
   if (((md_info_ptr->d26_m == D2_ADDATT)
	|| (md_info_ptr->d26_m == D2_ADD_AV)
	|| (md_info_ptr->d26_m == D2_REP_AV))
       && (md_info_ptr->d26_mnew != (D23_av_value *) 0))
   {    /* check whether new value or attribute type should be deleted before */
	if (md_info_ptr->d26_m != D2_REP_AV)
	{    /* get index in AT for current attribute */
	     at_idx = d26_u03_attr_search (&(md_info_ptr->d26_mtype));
	} 
	if (at_idx != D26_ERROR)
	{    /* check whether type should be deleted before */
	     at_entry = &d26_at[at_idx];
	     del_type = d26_C05_find_attr (&del_a_types[0],
			     &(md_info_ptr->d26_mtype),
			     (signed16 *) NULL, (char *) NULL,
			     at_entry, (signed32) D26_NOTHING, FALSE,
			     &p_del_types, (byte **) 0, (signed32 *) NULL,
			     (D2_name_string) 0);
	     if (del_type == FALSE)
	     {    /* check whether value should be deleted before. */
		  /* add value to list, such that the filter will not */
		  /* contain double or contradicting constructions like */
		  /* ... AND (VALUE1) AND ... AND NOT(VALUE1) ... */
		  del_value = d26_C05_find_attr (&del_a_values[0],
				 &(md_info_ptr->d26_mtype),
				 &(md_info_ptr->d26_mnew->d23_avlen),
				 (char *)((md_info_ptr->d26_mnew)+1),
				 at_entry, D2_ADDATT, FALSE,
				 &p_del_values, (byte **) 0, (signed32 *) 0,
				 (D2_name_string) 0);
		  if (del_value == FALSE)
		  {    /* set new value */
		       /* neither type nor value is deleted before   */
		       /* ---> it must be checked in search whether  */
		       /*      the new value already exists          */
		       new_val = md_info_ptr->d26_mnew;
		  }  /* new_val == D2_FALSE */
	     }  /* del_type == D2_FALSE */
	} else {
	     /* it must be checked in search whether  */
	     /* the new value already exists          */
	     new_val = md_info_ptr->d26_mnew;
	}  /* at_idx != D26_ERROR */
   }  /* d26_m == D2_ADDATT or D2_ADD_AV or D2_REP_AV */

   /**************************/
   /* set filter information */
   /**************************/
   if                             /*  attr. value should be deleted or     */
				  /*  replaced and old value is given      */
				  /*  or value should be added             */
      ((new_val != (D23_av_value *) 0) || (old_val != (D23_av_value *) 0))
   {                              /*  copy value into filter information*/
	no_ands++;
	if ((old_val != (D23_av_value *) 0)
	    && (new_val != (D23_av_value *) 0))
	{    /* 2 filter items will be supplied */
	     no_ands++;
	} 

	if (first_val == TRUE)
	{    /* copy D23_ft1_info */
	     first_val = FALSE;
				  /*  set ft_info_ptr                       */
	     ft_info_ptr = (D23_ft1_info *) act_pos;
				  /*  copy filter inf: D23_ft_info         */
	     ft_info_ptr->d23_fp1tag  = D23_PATTAG;  /* tag              */
	     ft_info_ptr->d23_fp1mod  = D2_AND;      /* operation mode   */
	     no_items = (char *) &(ft_info_ptr->d23_fp1count);
	     act_pos = (char *) (ft_info_ptr + 1);
	}  /* i == 0 */
				  /*  set ft_info_ptr (NOT for add val  */
				  /*  and add attribute only)           */
	if (new_val != (D23_av_value *) 0)
	{    /* new value given: set filter mode NOT */
	     ft_info_ptr = (D23_ft1_info *) act_pos;
				  /*  copy filter inf: D23_ft_info         */
	     ft_info_ptr->d23_fp1tag  = D23_PATTAG;  /* tag              */
	     ft_info_ptr->d23_fp1mod  = D2_NOT;      /* operation mode   */
	     ft_info_ptr->d23_fp1count = 1;
	     act_pos = (char *) (ft_info_ptr + 1);
	     val = new_val; /* set value for filter item */
	} else { /* no new value given */
	     val = old_val; /* set value for filter item */
	}  /* new value given */

	/* supply filter item */
	while (val != (D23_av_value *) 0)
	{ 
				  /*  set ft_info_ptr                       */
	   ft_info_ptr = (D23_ft1_info *) act_pos;
				  /*  copy filter inf: D23_ft_info         */
	   ft_info_ptr->d23_fp1tag  = D23_PATTAG;  /* tag              */
	   ft_info_ptr->d23_fp1mod  = D2_ITEM;     /* operation mode   */
	   ft_info_ptr->d23_fp1count = 1;
	   act_pos = (char *) (ft_info_ptr + 1);
				  /*  set filter items                     */
	   fi_info_ptr = (D23_fi1_info *) act_pos;
	   fi_info_ptr->d23_fi1tag = D23_FIITAG;
	   if
	      (add_or_del_at[at_idx] != TRUE)
				  /* set match to D2_EQUAL and set filter  */
	   {
	     fi_info_ptr->d23_fi1match = D2_EQUAL; /* complete matching    */
	   } else                 /* need not match for equality because   */
	   {                      /* TYPE will be added or deleted !       */
	     fi_info_ptr->d23_fi1match = D2_PRESENT; /* attribute must be  */
	   }                                         /* present only       */
	   fi_info_ptr->d23_fi1syntx = md_info_ptr->d26_mrep;

				  /*  set attribute type                  */
	   attr_type = (D23_at_type *) (fi_info_ptr + 1);
	   attr_type->d23_atttag = D23_ATTTAG;
	   attr_type->d23_atoff  = D2_ALIGN (md_info_ptr->d26_mtype.d2_typ_len);
	   attr_type->d23_atlen  = md_info_ptr->d26_mtype.d2_typ_len;
	   act_pos = (char *) (attr_type + 1);
	   memcpy ((Octet_string) act_pos,
			    (Octet_string) (md_info_ptr->d26_mtype.d2_type),
			    attr_type->d23_atlen);
	   act_pos = act_pos + attr_type->d23_atoff;

	   if
	      (add_or_del_at[at_idx] != TRUE)
				  /* only in case of D2_EQUAL the values  */
				  /* must be supplied                     */
	   {
				  /*  set value                           */
	     attr_val = (D23_av_value *) act_pos;
	     attr_val->d23_avoff = val->d23_avoff;  /* offset to next data   */
	     attr_val->d23_avlen = val->d23_avlen;  /* length of attr value  */
	     attr_val->d23_avrep = val->d23_avrep;  /* repr.  of attr value  */

				    /*  set act_pos to begin of attr. value  */
	     act_pos = (char *) (attr_val + 1);
				    /*  calculate needed memory size         */
	     use_mem = attr_val->d23_avoff + D23_S_FT1 + D23_S_FT1 + D23_S_FI1 +
		       D23_S_ATT + D2_ALIGN(D2_OBJ_IDL_MAX) + D23_S_ATV + 1;
				    /*  check memory size                    */
	     ret_val = d26_C03_check_search_memsize (use_mem, &begin_mem,
			       (char **) in_s, &act_pos, &end_mem, &no_items,
			       &no_of_memblocks, error, l_err);
	     if (ret_val != D2_NO_ERR)    /*  not enough memory            */
	     {                            /*  exit function                */
		  return (D26_ERROR);
	     }                              /*  not enough memory */

				  /*  copy attr. value to message          */
	     val_ptr = ((char *) val) + D23_S_ATV;
	     memcpy ((Octet_string) act_pos, (Octet_string) val_ptr,
			      attr_val->d23_avlen);
				  /*  set act_pos to begin of next data    */
	     act_pos = act_pos + attr_val->d23_avoff;
	   }
	   /* set next filter item value */
	   val = (val == new_val) ? old_val : (D23_av_value *) 0;
	}  /* supply filter item (val != 0) */
   }                              /*  attr. value ... */
}                                 /*  all attr ...  */

/* set number of filter items */
if (no_ands > 0)
{   
     (*((signed16 *) no_items)) = (signed16) no_ands;
} 


/**********************/
/* set end of message */
/**********************/
                        /*  fill end of message                       */
((D23_eom *) act_pos)->d23_eomtag = D23_EOMTAG;

free(add_or_del_at);
return (D2_NO_ERR);

}                               /*  d26_C02_set_search_par            */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C03_check_search_memsize                          */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        08.02.88                                              */
/*                                                                    */
/* SYNTAX:      LOCAL Short d26_C03_check_search_memsize (usedmem,   */
/*                                           begin, par, act, end,    */
/*                                           no_items,                */
/*                                           no_mems, error, l_err)   */
/*                                                                    */
/*              Long        usedmem;          ->                      */
/*              char        **begin,          <>                      */
/*                          **par,            <>                      */
/*                          **act,            <>                      */
/*                          **end,            <>                      */
/*                          **no_items;       <>                      */
/*              Short       *no_mems;         <>                      */
/*              Octet       **error;          <>                      */
/*              Int         *l_err;           <>                      */
/*                                                                    */
/* DESCRIPTION: This function checks whether there is enough memory   */
/*              for the data  which should be copied into the         */
/*              invoke message of the internal DS_SEARCH. if there    */
/*              isn't enough memory the function allocates new using  */
/*              the function realloc so that the data  which were     */
/*              already copied are further available. The new pointers*/
/*              to the data were returned. In case of errors a error  */
/*              apdu is returned.                                     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*     Long     usedmem                size of memory which is        */
/*                                     is required for the data which */
/*                                     should be copied into the      */
/*                                     DS_SEARCH invoke message       */
/*                                                                    */
/*     char     **begin                INPUT OUTPUT parameter         */
/*                                     begin of memory                */
/*                                                                    */
/*     char     **par                  INPUT OUTPUT parameter         */
/*                                     pointer to DS_SEARCH invoke    */
/*                                     message                        */
/*                                                                    */
/*     char     **act                  INPUT OUTPUT parameter         */
/*                                     pointer to the begin of the    */
/*                                     memory for the new data        */
/*                                                                    */
/*     char     **end                  INPUT OUTPUT parameter         */
/*                                     pointer to the end of allocated*/
/*                                     memory                         */
/*                                                                    */
/*     char     **no_items             INPUT OUTPUT parameter         */
/*                                     pointer to the number of filter*/
/*                                     items                          */
/*                                                                    */
/*     Short    *no_mems               INPUT OUTPUT parameter         */
/*                                     number of allocated memory     */
/*                                     blocks                         */
/*                                     (size of allocated memory =    */
/*                                      *no_mems * D26_C01_SEARCH_MEM)*/
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*     Octet         **error           error message (contains return */
/*                                     codes in case of errors)       */
/*                                                                    */
/*     Int           *l_err            length of error message        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short    D2_NO_ERR    no error occurred                         */
/*                                                                    */
/*             D26_ERROR    any error occurred                        */
/*                          (e.g. not enough memory for DS_SEARCH     */
/*                          invoke message, error apdu and length     */
/*                          of error apdu are set)                    */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        realloc                                                     */
/*        free                                                        */
/*                                                                    */
/*        d27_010_set_error_apdu                                      */
/*                                                                    */
/* NOTES: The memory for the DS_SEARCH invoke message is former       */
/*        (in the function d26_C02_set_search_par) allocated via      */
/*        malloc.                                                     */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C03_check_search_memsize(
					  signed32    usedmem,
					  char      **begin,
					  char      **par,
					  char      **act,
					  char      **end,
					  char      **no_items,
					  signed16   *no_mems,
					  byte      **error,
					  signed32   *l_err
					 )

{                               /*  d26_C03_check_search_memsize      */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
signed32      offset;         /* offset to begin of old memory        */
signed32      off_items;      /* offset of number of items            */
unsigned32    memlength;      /* new memory length                    */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/


if                            /*  not enough memory for the new data       */
   ((*act + usedmem) >= *end)
{                             /*  reallocate new memory:                   */
			      /*  calculate offset of new datas to begin   */
			      /*  of memory                                */
     offset = *act - *begin;
     if (*no_items != (char *) NULL)
     {   
	  off_items = *no_items - *begin;
     } 

			      /*  calculate new memory size                */
     (*no_mems)++;
     memlength = *no_mems * D26_C01_SEARCH_MEM;

			      /*  realloc new memory                       */
     if                       /*  no memory available                      */
	((*begin = (char *) D27_REALLOC(svc_c_sev_warning, *par, memlength))
	  == NULL)
     {                        /*  free old memory                          */
	  free (*par);
			      /*  set error apdu and length of apdu        */
	  d27_010_set_error_apdu ((D23_rserror *) *error, D2_TEMP_ERR, 
				   D2_REM_ERR, D2_UNWILLING, l_err);
	  return (D26_ERROR); /*  exit function                            */
     }                        /*  realloc fails */

			      /*  set new pointer to invoke message        */
     *par = *begin;
			      /*  set new pointer to end of invoke message */
     *end = *begin + memlength;
			      /*  set new pointer to begin of memory for   */
			      /*  new data                                 */
     *act = *begin + offset;
     if (*no_items != (char *) NULL)
     {   
	  *no_items = *begin + off_items;
     } 
}                             /*  not enough memory for the new data */


return (D2_NO_ERR);
}                               /*  d26_C03_check_search_memsize      */





/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C05_find_attr                                     */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        18.02.88                                              */
/*                                                                    */
/* SYNTAX:      LOCAL Short d26_C05_find_attr (attr_list, type,      */
/*                                             length, value, p_at,   */
/*                                             mod_lst, norm_val,     */
/*                                             start_lst, error,      */
/*                                             l_err, entryname)      */
/*                                                                    */
/*              D26_CT0_inf_attrs   *attr_list;      <>               */
/*              D2_a_type           *type;           ->               */
/*              Ushort              *length;         <>               */
/*              char                *value;          <>               */
/*              D26_at              *p_at;           ->               */
/*              Int                 mod_lst;         ->               */
/*              Bool                norm_val;        ->               */
/*              D26_CT0_inf_attrs   **start_lst;     <>               */
/*              Octet               **error;         <>               */
/*              Int                 *l_err;          <>               */
/*              D2_name_string      entryname;       ->               */
/*                                                                    */
/* DESCRIPTION: This function looks whether the given attribute type  */
/*              'type' exists in the attribute list 'attr_list'.      */
/*              When a value is given (value != D2_NULL) the value    */
/*              is also compared.                                     */
/*                                                                    */
/*              When the indicator 'append' is set to D2_TRUE the     */
/*              attribute is added to the list when it isn't found.   */
/*                                                                    */
/*              When the indicator 'delete' is set to D2_TRUE the     */
/*              attribute is deleted from the list when it was found. */
/*                                                                    */
/*              The function returns a bool value whether the         */
/*              attribute is found.                                   */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D26_CT0_inf_attrs  *attr_list   INPUT OUTPUT parameter        */
/*                                      memory for                    */
/*                                      list with attribute types and */
/*                                      values                        */
/*                                      (the list must be initialized */
/*                                      with D26_EOL for all values   */
/*                                      before the first call of this */
/*                                      function)                     */
/*                                      -> existing attribute list    */
/*                                      <- new attribute list         */
/*                                         (searched attribute is     */
/*                                         appended if it wasn't found*/
/*                                         and 'append' was set)      */
/*                                         (searched attribute is     */
/*                                         deleted if it was found    */
/*                                         and 'delete' was set)      */
/*                                                                    */
/*      D2_a_type          *type        searched attribute type       */
/*                                                                    */
/*      Ushort             *length      INPUT OUTPUT parameter        */
/*                                      length of searched attribute  */
/*                                      value                         */
/*                                      (when syntax is D2_CEXACT,    */
/*                                      D2_DISTNAME, D2_CIGNORE,      */
/*                                      D2_ACCL blanks will be        */
/*                                      removed and the new length    */
/*                                      is returned)                  */
/*                                                                    */
/*      char               *value       INPUT OUTPUT Parameter        */
/*                                      searched attribute value      */
/*                           != D2_NULL attribute value is relevant   */
/*                                      and must be compared          */
/*                                      (that is attr. type and value */
/*                                      must match, the value is      */
/*                                      normed after function call,   */
/*                                      when 'norm_val' == D2_TRUE and*/
/*                                      syntax != D2_TIME and value   */
/*                                      must be normed for comparison.*/
/*                                      Blanks will be removed always)*/
/*                           == D2_NULL attribute value is irrelevant */
/*                                      (that is only attribute type  */
/*                                      must match)                   */
/*                                                                    */
/*      D26_at             *p_at        at entry of attribute         */
/*                                                                    */
/*      Int                mod_lst      indicates whether the         */
/*                                      attribute is to be appended   */
/*                                      when attribute wasn't found   */
/*                                      or attribute is to be deleted */
/*                                      when attribute was found      */
/*                            D2_ADDATT append attribute to list      */
/*                            D2_DELATT delete attribute from list    */
/*                            D26_NOTHING do nothing of above actions */
/*                                                                    */
/*      Bool               norm_val     indicates whether the         */
/*                                      value should be normed or not */
/*                            D2_TRUE   norm value allowed            */
/*                            D2_FALSE  norm value not allowed        */
/*                                                                    */
/*      D26_CT0_inf_attrs  **start_lst  INPUT OUTPUT Parameter        */
/*                                      start of list                 */
/*                                      must be initialized with      */
/*                                      NULL-Pointer for the first    */
/*                                      call (indicates empty list)   */
/*                                      All following calls gets the  */
/*                                      output value of the preceding */
/*                                      call.                         */
/*                                                                    */
/*       D2_name_string  entryname          name of entry to be       */
/*                                          modified                  */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*     Octet         **error           error message (contains return */
/*                                     codes in case of errors)       */
/*                                                                    */
/*     Int           *l_err            length of error message        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short  D2_TRUE      attribute was found                         */
/*                        when 'mod_lst' == D2_DELATT the attribute   */
/*                        was deleted from the attribute list         */
/*                                                                    */
/*           D2_FALSE     attribute wasn't found                      */
/*                        when 'mod_lst' == D2_ADDATT the attribute   */
/*                        was added to the attribute list             */
/*                                                                    */
/*           D26_ERROR    wrong syntax (error apdu and length of      */
/*                        are supplied if par. error != 0)            */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_u03_attr_search                                         */
/*        d26_u15_attr_error                                          */
/*        d26_u28_norm_numbers                                        */
/*        d26_u98_attr_val_cmp                                        */
/*        d26_u99_handle_blanks                                       */
/*      d26_un1_norm_or_address()                                     */
/*      d26_un2_norm_or_name()                                        */
/*      d26_un3_norm_submit_perm()                                    */
/*                                                                    */
/*        d27_007_norm_name                                           */
/*        d27_301_oid_str                                             */
/*                                                                    */
/*        sprintf                                                     */
/*                                                                    */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C05_find_attr (
			       D26_CT0_inf_attrs   *attr_list,
			       D2_a_type           *type,
			       signed16            *length,
			       char                *value,
			       D26_at              *p_at,
			       signed32             mod_lst,
			       boolean              norm_val,
			       D26_CT0_inf_attrs  **start_lst,
			       byte               **error,
			       signed32            *l_err,
			       D2_name_string       entryname
			      )
{                               /*  d26_C05_find_attr                 */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
signed32    found;     /* return value */
D26_CT0_inf_attrs   *ptr_type;
signed32      apdu_len = 0;
signed16   rv;

/* FASTAUTO  ...    */
register  D26_CT0_inf_attrs  *ptr_attr_list; /* pointer to attribute list */
register  D26_CT0_inf_attrs  *next_free;     /* next free attr. in list   */
register  D26_CT0_inf_attrs  *last_attr;     /* last handled attribute    */
register  char               *val_list;      /* value of attr. in list    */
register  signed16           match;          /* return value of compare-  */
					     /* function                  */
register  boolean            rv_norm_cmp;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/**************/
/* norm value */
/**************/
if                              /*  blanks must be removed and value is given*/
  (value != (char *) NULL)
{                               /*  remove blanks                          */
     *length = d26_u99_handle_blanks (p_at, value, *length);
     if                         /*  value must be normed                   */
	(norm_val == TRUE)
     {                          /*  norm value                             */
	  switch ((signed32) p_at->d26a_syntax)
	  {
	  case D2_NUMERIC:
	  case D2_TELNR_STX:
	       rv = d26_u28_norm_numbers (value, *length, value);
	       *length = rv;
	       break;
	  case D2_ACCL:
	       d26_uso_norm_acl((byte *)value,(byte *)value);
	       break;
	  case D2_DISTNAME:
	       d27_007_norm_name (value, *length, value);
	       break;
	  case D2_COUNTRY_STX:
	  case D2_IA5:
	       d27_031_norm_pr_string((byte *)value,(byte *)value,*length);
	       break;
	  case D2_CIGNORE:
	       d27_651_norm_T61String((byte *)value,(byte *)value,*length);
	       break;
	  case D2_POST_AD_STX:
	  case D2_CIGN_LIST:
	       d26_usp_norm_ci_list((byte *)value,(byte *)value,*length);
	       break;
	  case D2_TIME:
	       /* cannot be normed on value because normed value */
	       /* may be longer                                  */
	       break;
	  case D2_CEXACT:
	  case D2_PRINTABLE:
	  case D2_INTEGER:
	  case D2_BOOLEAN:
	  case D2_TLXNR_STX:
	  case D2_OCTET:
	  case D2_ANY_STX:
	  case D2_PASSWD_STX:
	  case D2_PSAP_STX:
	  case D2_TTXID_STX:
	  case D2_FAXNR_STX:
	  case D2_OBJ_IDENT:
	  case D2_GUIDE:
	  case D2_PR_DM_STX:
	  case D2_MHS_PR_DM_STX:
	  case D2_CERTIFICATE:
	  case D2_CERT_PAIR:
	  case D2_CERT_LIST:
	       break;
	  case D2_MHS_DLSP_STX:
	       *length = d26_un3_norm_submit_perm(value,*length,value);
	       break;
	  case D2_MHS_ORADR_STX:
	       *length = d26_un1_norm_or_address(value,*length,value);
	       break;
	  case D2_MHS_ORNAME_STX:
	       *length = d26_un2_norm_or_name(value,*length,value);
	       break;
	  default:
	       /* syntax error */
	      if (error != (byte **) NULL)
	      {   
		   d26_u15_attr_error ((byte **) error,
			      exact_name, 1, D2_ATSYNTAX_ERROR,
			      type,
			      p_at->d26a_rep, p_at->d26a_rep,
			      *length, (byte *) value,
			      l_err, &apdu_len);
		   *l_err = apdu_len;
	      } 

	      return(D26_ERROR);
	  }    /* syntax */
     }
}

/********************/
/* search attribute */
/********************/
for                             /*  all attributes in list and attribute   */
				/*  not found                              */
    (found = FALSE,
       ptr_attr_list = last_attr = *start_lst, next_free = attr_list;
     ((ptr_attr_list != (D26_CT0_inf_attrs *) 0) && (found == FALSE));
     last_attr = ptr_attr_list, next_free = ptr_attr_list + 1,
       ptr_attr_list = ptr_attr_list->next_attr)
{                               /*  search attribute                       */
   ptr_type = ptr_attr_list;
   if                           /*  attribute type is found                */
      (D27_ATTCMP(&(ptr_type->attr_type), type) == 0)
   {                            /*  check whether value must match also    */
	if                      /*  value is given                         */
	   (value != (char *) NULL)
	{                       /*  compare value                          */
	     if                 /*  length matches                         */
		((ptr_attr_list->val_length == *length)
		 || (p_at->d26a_syntax == D2_TIME)
		 || (norm_val == FALSE))
	     {                  /*  compare value                          */
		  val_list  = ptr_attr_list->attr_value;
		  if ((norm_val == FALSE) || (p_at->d26a_syntax == D2_TIME))
		  {   
		       rv_norm_cmp = d26_u98_attr_val_cmp((byte *) val_list,
					 ptr_attr_list->val_length,
					 (byte *) value, *length,
					 p_at->d26a_syntax);
		       match = (rv_norm_cmp == TRUE) ? 0 : -1;
		  } else {
		       match = memcmp ((Octet_string) val_list,
					      (Octet_string) value, *length);
		  } 
		  if (match == 0)
		  {   
		       found = TRUE;
		       break;
		  } 
	     }                  /*  length matches */
	} else {                    /*  set found to D2_TRUE                   */
	     found = TRUE;
	     break;
	}                       /*  value is given */
   }                            /*  attr. type is found */
}                               /*  all attr. ... */

/******************************/
/* append or delete attribute */
/******************************/
switch (mod_lst)
{
case D2_ADDATT: /* append attribute to list */
     if (found == FALSE) /* attribute wasn't found */
     {    /* append attribute to list */
	  if (*start_lst == (D26_CT0_inf_attrs *) 0)
	  {    /* set start of list */
	       *start_lst = next_free;
	  } else { /* append next free element to list */
	       last_attr->next_attr = next_free;
	  }  /* start_lst == 0 */
	  /* set list values */
	  next_free->next_attr = (D26_CT0_inf_attrs *) 0;
	  next_free->attr_type = (*type);
	  if (value != (char *) NULL)
	  {   
	       next_free->val_length = *length;
	       next_free->attr_value = value;
	  } 
     }  /* attribute wasn't found ... */
     break;

case D2_DELATT:  /* delete attribute from list */
     if (found == TRUE) /* attribute was found */
     {    /* delete attribute from list */
	  if (*start_lst == ptr_attr_list)
	  {    /* set start of list (first attribute is to be deleted) */
	       *start_lst = ptr_attr_list->next_attr;
	  } else { /* delete attribute from list */
	       last_attr->next_attr = ptr_attr_list->next_attr;
	  }  /* start_lst == attr. to be deleted */
     }  /* attribute was found ... */
     break;

case D26_NOTHING:
default:
     break;
}     /* mod_lst */


return (found);
}                               /*  d26_C05_find_attr                 */




/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C06_add_new_dns                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        22.02.88                                              */
/*                                                                    */
/* SYNTAX:      LOCAL Short d26_C06_add_new_dns (n_entry, acl_old,   */
/*                                        acl_new, old_mk_idx,        */
/*                                        new_mk_idx, acl_attr,       */
/*                                        mk_attr, dnl_ptr,           */
/*                                        dns, dnl_sort,              */
/*                                        no_dnl_entries)             */
/*                                                                    */
/*              char          *n_entry;        ->                     */
/*              D26_acl       *acl_old;        ->                     */
/*              D26_acl       *acl_new;        ->                     */
/*              Long          *old_mk_idx;     ->                     */
/*              Long          *new_mk_idx;     ->                     */
/*              D23_av_value  *acl_attr;       ->                     */
/*              D23_av_value  *mk_attr;        ->                     */
/*              D26_dns       *dnl_ptr[];      <>                     */
/*              D26_dns       *dns;            <>                     */
/*              D26_dns       *dnl_sort[];     <-                     */
/*              Short         *no_dnl_entries; <-                     */
/*                                                                    */
/* DESCRIPTION: This function adds the new distinguished name list    */
/*              entries to distinguished name list file to get the    */
/*              index values for the new acl attribute value and/or   */
/*              the new master knowledge attribute value.             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      char         *n_entry        normed entryname (is needed      */
/*                                   for d26_u20_sort_dn)             */
/*                                                                    */
/*      D26_acl      *acl_old        old acl value (indices to dnl in */
/*                                   memory)                          */
/*                                                                    */
/*      D26_acl      *acl_new        memory for new acl value (index  */
/*                                   values)                          */
/*                                                                    */
/*      Long         *old_mk_idx     old masterknowledge value (index */
/*                                   to dnl in memory)                */
/*                                                                    */
/*      Long         *new_mk_idx     memory for new masterknowledge   */
/*                                   value (index value)              */
/*                                                                    */
/*      D23_av_value *acl_attr       pointer to new acl attribute     */
/*                       D2_NULL     acl attr. shouldn't be modified  */
/*                       != D2_NULL  acl attribute should be modified */
/*                                                                    */
/*      D23_av_value *mk_attr        pointer to new mk attribute      */
/*                       D2_NULL     mk attr. shouldn't be modified   */
/*                       != D2_NULL  mk attribute should be modified  */
/*                                                                    */
/*      D26_dns      *dnl_ptr[]      INPUT OUTPUT parameter           */
/*                                   pointer array to "dns"           */
/*                                   (internal structure for sorting  */
/*                                   and merging, memory must be      */
/*                                   allocated by the caller)         */
/*                                   -> when acl attribute should be  */
/*                                      modified the pointers to the  */
/*                                      new acl information are set   */
/*                                      in this structure             */
/*                                   <- when acl attribute should be  */
/*                                      modified the pointers to the  */
/*                                      old acl information           */
/*                                      when masterknowledge should   */
/*                                      be modified the pointers to   */
/*                                      the old and the new           */
/*                                      information are set           */
/*                                                                    */
/*      D26_dns      *dns            INPUT OUTPUT parameter           */
/*                                   filled structure of acl and/or   */
/*                                   masterknowledge information      */
/*                                   (internal structure for sorting  */
/*                                   and merging, memory must be      */
/*                                   allocated by the caller)         */
/*                                   -> when acl attribute should be  */
/*                                      modified the the new acl      */
/*                                      informations are set in this  */
/*                                      structure                     */
/*                                   <- when acl attribute should be  */
/*                                      modified the old acl          */
/*                                      informations are set          */
/*                                      when masterknowledge should   */
/*                                      be modified the old and the   */
/*                                      new information are set       */
/*                                                                    */
/*      Bool         *upd_file       INPUT OUTPUT parameter           */
/*                                   flag which indicates whether dnl */
/*                                   in file must be updated          */
/*                                   input information isn't used     */
/*                                   (always D2_TRUE)                 */
/*                                   <- is set to D2_FALSE when       */
/*                                      *no_dnl_entries == D2_NULL    */
/*                                      (that is no record must be    */
/*                                      updated in dnl in file though */
/*                                      acl attribute and/or mk attr. */
/*                                      must be updated else the flag */
/*                                      isn't changed)                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      D26_dns      *dnl_sort       sorted and merged pointer array  */
/*                                   to "dns" which is used to add    */
/*                                   and delete dns in dn list in file*/
/*                                   (internal structure for sorting  */
/*                                   and merging, memory must be      */
/*                                   allocated by the caller)         */
/*                                                                    */
/*      Short        *no_dnl_entries number of sorted pointers        */
/*                                   == number of records which must  */
/*                                      be updated in dnl file        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short   D2_NO_ERR   no error occurred                           */
/*                                                                    */
/*            D26_ERROR   any error occurred                          */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*      D26_dndesc     *d26_dnlist    distinguished name list in core */
/*      char           d26_myname[]   own DSA name (normed)           */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_i02_upd_file_dnlist                                     */
/*        d26_u06_get_name                                            */
/*        d26_u11_check_dn_index                                      */
/*        d26_u83_update_modify_rights                                */
/*        d26_u20_sort_dn                                             */
/*        d27_007_norm_name                                           */
/*                                                                    */
/* NOTES: s. also DESCRIPTION of function d26_u20_sort_dn             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C06_add_new_dns (char          *n_entry,
				 D26_acl       *acl_old,
				 D26_acl       *acl_new,
				 signed32      *old_mk_idx,
				 signed32      *new_mk_idx,
				 D23_av_value  *acl_attr,
				 D23_av_value  *mk_attr,
				 D26_dns       *dnl_ptr[],
				 D26_dns       *dns,
				 boolean       *upd_file,
				 D26_dns       *dnl_sort[],
				 signed16      *no_dnl_entries
				)

{                               /*  d26_C06_add_new_dns               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */

/* AUTO      ...    */
D26_dndesc      *ptr_dnlist;
char            *val;
signed16         ret_val;         /* return value of called function   */

/* FASTAUTO  ...    */
register signed16       i;        /* counter */
register signed32       index_val;
register signed16       idx;
register D26_dns     *p_dns;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/*****************/
/* acl attribute */
/*****************/

if                           /*  acl attribute should be modified          */
   (acl_attr != (D23_av_value *) NULL)
{                            /*                                            */

     /**************************************************/
     /* put old acl information to dn list for sorting */
     /**************************************************/
			     /*  set old acl information for sorting       */
     for                     /* all old acl indices */
	 (i = 0; i < D2_ACLIDX_MAX; i++)
     { 
	index_val = acl_old[i];
	idx = i + D26_C06_OLD_ACL;      /* index of dns and dns_ptr        */
	p_dns = &dns[idx];              /* pointer to acl information      */
	dnl_ptr[idx] = p_dns;           /* pointer to list of acl inf      */

					/* store acl information           */
	p_dns->d26_didx = &acl_old[i];  /* old index value                 */
	p_dns->d26_dno  = -1;           /* number of referencies -1        */

	if (index_val >= 0)       /* index to dn list in core        */
	{   
	     d26_u11_check_dn_index (index_val, &ptr_dnlist);
	     if                         /* valid index given               */
		(ptr_dnlist != (D26_dndesc *) NULL)
	     {   
					/* store old dn interpretation     */
		  p_dns->d26_dint = ptr_dnlist->d26_dnint;
					/* store old normed and exact dn   */
	  strcpy((char *) p_dns->d26_edname, (char *) ptr_dnlist->d26_ename);
		  p_dns->d26_dname = ptr_dnlist->d26_dnname;
	     } else {                   /* error: no valid index to dnlist */
					/*        in core                  */
		  /* GDS_S_DNL_IDX_ERR */
		  return (D26_ERROR);   /* exit function                   */
	     }                          /* valid index given */
	} else {                        /* no index to dnlist in core      */
	     p_dns->d26_dint  = D2_IGNORE;
	     p_dns->d26_dname = (D2_name_string) NULL;
	     p_dns->d26_edname[0] = D2_EOS;
	}                     /* index to dn list in core */
     }                       /* i=D26_C06_OLD_ACL; i < upper; i++ */

			     /*  sort and merge DNs of old and new acl     */
     d26_u20_sort_dn ((D2_name_string) n_entry, dnl_ptr, 2 * D2_ACLIDX_MAX,
		      dnl_sort, no_dnl_entries);
}                            /*  acl attr. ... modified */

/*****************************/
/* masterknowledge attribute */
/*****************************/

if                           /*  mk attribute should be modified           */
   (mk_attr != (D23_av_value *) NULL)
{                            /*  set old mk information                    */
     p_dns = &dns[D26_C08_OLD_MK];    /* pointer to mk information         */
     dnl_ptr[D26_C08_OLD_MK] = p_dns;
     p_dns->d26_didx = old_mk_idx;    /* old mk index value                */

     if (*old_mk_idx != D26_OWNNAME)  /* old index not own DSA             */
     {                                /* append information to sorted list */
	  if (*old_mk_idx >= 0) /* old index record no in dnl in file*/
	  {   
	       p_dns->d26_dno = -1;   /* ==> 'delete' old value from dnl   */
	       p_dns->d26_dint = D2_IGNORE;
	       p_dns->d26_dname = (D2_name_string) NULL;
	       p_dns->d26_edname[0] = D2_EOS;
	       dnl_sort[*no_dnl_entries] = p_dns;
	       *no_dnl_entries = *no_dnl_entries + 1;
	  }                           /* old index >= 0 */
     }                                /* old index not own DSA */

			     /*  set new mk information                    */
				 /* norm new mk value                      */
     val = ((char *) mk_attr) + sizeof (D23_av_value);

     p_dns = &dns[D26_C09_NEW_MK];
     dnl_ptr[D26_C09_NEW_MK] = p_dns;
     strcpy(p_dns->d26_edname, (char *) val); /* copy exact name for DN-list */
				    /* build new mk index value            */
     d27_007_norm_name (val, mk_attr->d23_avlen, val);
     if (strcmp(val, (char *) d26_myname))
     {   
	  *new_mk_idx = D26_IDX_NOT_DEF;
     } else {
	  *new_mk_idx = D26_OWNNAME;
     } 
     p_dns->d26_didx = new_mk_idx;

     if (*new_mk_idx != D26_OWNNAME)  /* new index not own DSA             */
     {                                /* append information to sorted list */
	  p_dns->d26_dint = D2_SOBJ;
	  p_dns->d26_dname = (D2_name_string) val;
	  p_dns->d26_dno = 1;         /* ==> 'add' new value to dnl        */
	  dnl_sort[*no_dnl_entries] = p_dns;
	  *no_dnl_entries = *no_dnl_entries + 1;
     }                                /* new index not own DSA             */
}                            /*  mk attr. ... */

/**********************************/
/* add new dns to dn list in file */
/**********************************/

for                          /*  all new dns                               */
    (i = 0; i < *no_dnl_entries; i++)
{                            /*  add new dn to dnl in C-isam file          */
    p_dns = dnl_sort[i];
    if                       /* new dn                                     */
       (p_dns->d26_dno > 0)
    {                        /* add dn                                     */
	 ret_val = d26_i02_upd_file_dnlist (D2_ADD_AV,
		       (D2_name_string) NULL, p_dns->d26_dint,
		       (D2_name_string) p_dns->d26_edname, p_dns->d26_dno,
		       FALSE, p_dns->d26_didx);
	 if                  /* update not successfull                     */
	    (ret_val != D2_NO_ERR)
	 {                   /* error during update of dnl in file         */
			     /* exit function                              */
	      /* GDS_S_FCT_FAILED ("i02") */
	      return (D26_ERROR);
	 }                   /* upd not successfull */
    }                        /* new dn                                     */
}                            /*  all new dns  */

if                           /*  no record in dnl in file must be updated  */
   (*no_dnl_entries == 0)
{                            /*  switch off indicator for dnl file update  */
     *upd_file = FALSE;
}                            /*  no record ... */

return (D2_NO_ERR);
}                               /*  d26_C06_add_new_dns               */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_C07_del_old_dns                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        02.03.88                                              */
/*                                                                    */
/* SYNTAX:      LOCAL Short d26_C07_del_old_dns (dnl_sort,           */
/*                                        no_dnl_sort, del_dns,       */
/*                                        no_del_dns)                 */
/*                                                                    */
/*              D26_dns       *dnl_sort[];      ->                    */
/*              Short         no_dnl_sort;      ->                    */
/*              Long          *del_dns;         ->                    */
/*              Short         no_del_dns;       ->                    */
/*                                                                    */
/* DESCRIPTION: This function deletes old distinguished name list     */
/*              entries from distinguished name list in file and      */
/*              updates the timestamp in dn list in file.             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D26_dns      *dnl_sort[]     sorted and merged pointer array  */
/*                                   to "dns" which is used to add    */
/*                                   and delete dns in dn list in file*/
/*                                   (internal structure for sorting  */
/*                                   and merging, memory must be      */
/*                                   allocated by the caller)         */
/*                                                                    */
/*      Short        no_dnl_entries  number of sorted pointers        */
/*                                   == number of records which must  */
/*                                      be updated in dnl file        */
/*                                                                    */
/*      Long         *del_dns        array with index values which    */
/*                                   should 'deleted' from dn list    */
/*                                   in file                          */
/*                                   the memory used from this shorts */
/*                                   is 'free'd by this function      */
/*                                                                    */
/*      Short        no_del_dns      number of index values in        */
/*                                   'del_dns'                        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short   D2_NO_ERR   no error occurred                           */
/*                                                                    */
/*            D26_ERROR   any error occurred                          */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_i02_upd_file_dnlist                                     */
/*        free                                                        */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C07_del_old_dns (D26_dns      *dnl_sort[],
				 signed16       no_dnl_sort,
				 signed32      *del_dns,
				 signed16       no_del_dns
				)

{                               /*  d26_C07_del_old_dns               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */

/* AUTO      ...    */
signed32         dummy;
			      /* return value of this function     */
signed16        return_value = D2_NO_ERR;
signed16        ret_val;      /* return value of a called function */

/* FASTAUTO  ...    */
register signed16     i;               /* counter */
register D26_dns     *p_dns;
register signed32    *p_del_dns;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/


/***************************************/
/* delete old dns from dn list in file */
/* (caused by modification of acl and/ */
/*  or mk attribute)                   */
/***************************************/

for                          /*  all old dns                               */
    (i = 0; i < no_dnl_sort; i++)
{                            /*  delete old dn to dnl in C-isam file       */
    p_dns = dnl_sort[i];
    if                       /* old dn                                     */
       (p_dns->d26_dno < 0)
    {                        /* add dn                                     */
	 ret_val = d26_i02_upd_file_dnlist (D2_DEL_AV,
		       (D2_name_string) NULL, p_dns->d26_dint,
		       (D2_name_string) p_dns->d26_edname, p_dns->d26_dno,
		       FALSE, p_dns->d26_didx);
	 if                  /* update not successfull                     */
	    (ret_val != D2_NO_ERR)
	 {                   /* error during update of dnl in file         */
			     /* exit function                              */
	      /* GDS_S_FCT_FAILS ("C07:i02 dnl_sort") */
	      return_value = D26_ERROR;
	 }                   /* upd not successfull */
    }                        /* old dn                                     */
}                            /*  all old dns  */

/***************************************/
/* delete old dns from dn list in file */
/* (caused by modification of other    */
/*  attributes stored at main entry)   */
/***************************************/

if                           /*  other dns must be deleted                 */
   (del_dns != (signed32 *) NULL)
{                            /*  delete all dns given in list of del_dns   */
     p_del_dns = del_dns;

     for                     /* all dns in del_dns                         */
	 (i = 0; i < no_del_dns; i++, p_del_dns++)
     {                       /* delete dn from dnl file                    */
	 ret_val = d26_i02_upd_file_dnlist (D2_DEL_AV,
		       (D2_name_string) NULL, D2_IGNORE,
		       (D2_name_string) NULL, -1L, FALSE,
		       p_del_dns);
	 if                  /* update not successfull                     */
	    (ret_val != D2_NO_ERR)
	 {                   /* error during update of dnl in file         */
			     /* exit function                              */
	      /* GDS_S_FCT_FAILS ("C07:i02 del_dns") */
	      return_value = D26_ERROR;
	 }                   /* upd not successfull */
     }                       /* all dns in del dns                         */
			     /* free memory of del_dns                     */
     free ((char *) del_dns);
}                            /*  other dns must be deleted */

/************************/
/* update of time stamp */
/************************/

                        /*  update of time stamp                      */
                         /*  */
ret_val = d26_i02_upd_file_dnlist (D26_NOTHING,
	      (D2_name_string) NULL, D2_IGNORE,
	      (D2_name_string) NULL, (signed32) NULL, TRUE,
	      &dummy);
if                  /* update not successfull                     */
   (ret_val != D2_NO_ERR)
{                   /* error during update of dnl in file         */
		    /* exit function                              */
	      /* GDS_S_FCT_FAILS ("C07:i02 time_stamp") */
     return_value = D26_ERROR;
}                   /* upd not successfull */

return (return_value);          /* */
}                               /*  d26_C07_del_old_dns               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_C15_mand_attr_check(...)                        */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 08.12.87                                            */
/*                                                                    */
/* SYNTAX       : LOCAL Short d26_C15_mand_attr_check (mem_deltype,  */
/*                                  start_deltype,                    */
/*                                  error, l_err, entryname,          */
/*                                  ob_cl, alias_flag)                */
/*                                                                    */
/*                D26_CT0_inf_attrs  *mem_deltype;      ->            */
/*                D26_CT0_inf_attrs  **start_deltype;   ->            */
/*                Octet              **error;           <>            */
/*                Int                *l_err;            <>            */
/*                D2_name_string     entryname;         ->            */
/*                Long               *ob_cl;            ->            */
/*                Bool               alias_flag;        ->            */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : This function checks the whether a mandatory        */
/*                attribute should be deleted.                        */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                                                                    */
/*       D26_CT0_inf_attrs  *mem_deltype    memory for list of deleted*/
/*                                          attributes                */
/*                                                                    */
/*       D26_CT0_inf_attrs  **start_deltype list of attributes which  */
/*                                          should be deleted         */
/*                                                                    */
/*       D2_name_string  entryname          name of entry to be       */
/*                                          modified                  */
/*                                                                    */
/*       Long            *ob_cl             object classes            */
/*                                                                    */
/*       Bool            alias_flag         flag indicating whether   */
/*                                          object is an alias        */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/*   In case of errors:                                               */
/*       Octet           **error;           error message (contains   */
/*                                          the wrong attribute and/or*/
/*                                          return codes)             */
/*                                                                    */
/*       Ushort          *l_err;            length of error message   */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      any error occurred                */
/*                                  (error and l_err are supplied)    */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  D26_at     *d26_at;         Attribute-Description-*/
/*                                              Table                 */
/*                                                                    */
/*                  D26_oct    *d26_oct;        Object-Class-Table    */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d26_C05_find_attr()                               */
/*                                                                    */
/*                  d27_010_set_error_apdu()                          */
/*                  d27_301_oid_str                                   */
/*                                                                    */
/*                  sprintf                                           */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C15_mand_attr_check (D26_CT0_inf_attrs   *mem_deltype,
				     D26_CT0_inf_attrs  **start_deltype,
				     byte               **error,
				     signed32            *l_err,
				     D2_name_string       entryname,
				     signed32            *ob_cl,
				     boolean              alias_flag
				    )

{                                   /*  d26_C15_mand_attr_check()          */

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

signed16 ret_value = D2_NO_ERR;
D2_a_type      type;          /* attribute type                 */
signed16       at_idx;        /* index in AT                    */
D26_at         *at_entry;     /* entry in AT                    */
signed16       *mand_attr;
				   /* return value from d26_C05_find_attr */
signed16       del_type = FALSE;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/**********************************************/
/* check for deletion of mandatory attributes */
/**********************************************/
if (alias_flag == FALSE)
{    /* check deletion of masterknowledge attribte */
     at_idx = d26_mk_idx;
     at_entry = &d26_at[at_idx];
     /* look whether mandatory attr. is in list of deleted attrs. */
     if ((del_type =
	     d26_C05_find_attr (mem_deltype, &type, (signed16 *)
	     NULL, (char *) NULL, at_entry, D26_NOTHING,
	     TRUE, start_deltype, error, l_err, entryname))
	   == D26_ERROR)
     {   
	  return (D26_ERROR);
     } 

     /* check deletion of ACL attribute */
     if (del_type == FALSE)
     {   
	  at_idx = d26_acl_idx;
	  at_entry = &d26_at[at_idx];
	  /* look whether mandatory attr. is in list of deleted attrs. */
	  if ((del_type =
		  d26_C05_find_attr (mem_deltype, &type, (signed16 *)
		  NULL, (char *) NULL, at_entry, D26_NOTHING,
		  TRUE, start_deltype, error, l_err, entryname))
		== D26_ERROR)
	  {   
	       return (D26_ERROR);
	  } 
     }  /* del_type == D2_FALSE */
}  /* alias_flag == D2_FALSE */

/* check all mandatory attributes of object classes */
for /* all object classes of the object */
   (; *ob_cl != D26_EOL; ob_cl++)
{ 
   for /* all mandatory attributes of object class */
       (mand_attr = (d26_oct + *ob_cl)->d26c_mandatory;
	*mand_attr >= 0 && del_type == FALSE; mand_attr++)
   { 
      at_entry = d26_at + (*mand_attr);
      type = at_entry->d26a_type;
      /* look whether mandatory attr. is in list of deleted attrs. */
      if ((del_type =
	      d26_C05_find_attr (mem_deltype, &type, (signed16 *)
	      NULL, (char *) NULL, at_entry, D26_NOTHING,
	      TRUE, start_deltype, error, l_err, entryname))
	    == D26_ERROR)
      {   
	   return (D26_ERROR);
      } 
   }  /* all mandatory attributes of object class */
}  /* all object classes of the object */

/********************************************************************/
/* check whether at least one mandatory attribute should be deleted */
/********************************************************************/
if (del_type == TRUE)
{                     /* error: mand attr. deleted      */
     d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR,
				D2_PAR_ERR, D2_OBJ_CLS_VIOLATION,
			      l_err);
     return(D26_ERROR);
}                     /* mandatory attr. deleted */

return (ret_value);
}                                   /*  d26_C15_mand_attr_check()          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_C16_name_attr_check(...)                        */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 01.05.91                                            */
/*                                                                    */
/* SYNTAX       : LOCAL Short d26_C16_name_attr_check (entryname,    */
/*                                                      at, mod_info) */
/*                                                                    */
/*                D2_name_string     entryname;         ->            */
/*                D26_at             *at;               ->            */
/*                D26_md_info        *mod_info;         ->            */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : This function checks the whether the namepart value */
/*                should be modified.                                 */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                                                                    */
/*       D2_name_string  entryname          name of entry to be       */
/*                                          modified                  */
/*                                                                    */
/*       D26_at          *at                at_entry of mod_info      */
/*                                                                    */
/*       D26_md_info     *mod_info          attribute information of  */
/*                                          modification              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      modification not allowed for      */
/*                                  naming attribute (replace RDN     */
/*                                  must be used)                     */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                  D26_at     *d26_at;         Attribute-Description-*/
/*                                              Table                 */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                  d26_u06_get_name                                  */
/*                                                                    */
/*                  d27_012_remove_blanks                             */
/*                  d27_301_oid_str                                   */
/*                                                                    */
/*                  sprintf                                           */
/*                  memcmp                                            */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C16_name_attr_check (D2_name_string  entryname,
				     D26_at         *at,
				     D26_md_info    *mod_info
				    )

{                                   /*  d26_C16_name_attr_check()          */
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

signed16 ret_value = D2_NO_ERR;
signed32	modification;
char		n_entryname[D2_DNL_MAX+1];
D26_dninfo	dninfo;
D26_rdninfo	*last_rdn;
D26_ava		*p_ava;
signed16	rv;
char		*old_val;
signed32        match;
signed16	i_ava;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/* get dn information for comparison with last namepart */
dninfo.d26_rd_anz = 0;
rv = d26_u06_get_name (entryname, &dninfo);

/* check wether given type is naming       */
/* (for checking before search was called) */
last_rdn = &dninfo.d26_rdn[dninfo.d26_rd_anz - 1];
for (i_ava = 0, p_ava = last_rdn->d26_r_ava; i_ava<last_rdn->d26_r_nava;
     i_ava++, p_ava++)
{
  if (!D27_ATTCMP(&(d26_at[p_ava->d26_a_idx].d26a_type),
		  &(mod_info->d26_mtype)))
  break;
}
if (i_ava == last_rdn->d26_r_nava) /* not a naming attribute */
   return(D2_NOERROR);             /* check not necessary    */

if (at->d26a_nr_recur == 1) {
   /* Error: naming attribute is non recurring */
   ret_value = D26_ERROR;
} else {
   modification = mod_info->d26_m;
   switch (modification) {
      case D2_DEL_AV:
      case D2_REP_AV:
	   /* check whether not the namepart should be modified */
	   if /* check whether old value is given       */
	      (mod_info->d26_mold == (D23_av_value *) NULL) {
	      /* ERROR: no old value given */
	      ret_value = D26_ERROR;
	   } else {
	      /* check whether namepart value should be modified */
	      /* get dn information of last namepart */

	      /* get old value */
	      old_val = (char *) (mod_info->d26_mold + 1);
	      /* norm value */
	      mod_info->d26_mold->d23_avlen =
		  d27_012_remove_blanks (old_val, 
                                   mod_info->d26_mold->d23_avlen, old_val);
	      d27_651_norm_T61String((byte *)old_val,(byte *)old_val,
		mod_info->d26_mold->d23_avlen);
              /* check against each ava component */
	      for ( i_ava = 0, p_ava = last_rdn->d26_r_ava; 
                    i_ava < last_rdn->d26_r_nava; i_ava++, p_ava++ )
              {
	          /* norm last rdn values */
	          p_ava->d26_a_len = 
			 d27_012_remove_blanks ((String) (p_ava->d26_a_val),
					      p_ava->d26_a_len, n_entryname);
		  d27_651_norm_T61String((byte *)n_entryname,
		    (byte *)n_entryname,p_ava->d26_a_len);
	          /* compare values */
	          if (mod_info->d26_mold->d23_avlen == p_ava->d26_a_len) {
		     match = memcmp (old_val, n_entryname, p_ava->d26_a_len);
		     if (match == 0) {
		        /* Error: namepart value should be changed */
		        ret_value = D26_ERROR;
		     }
	          }
              }   /* check each ava */
	   } /* check whether old value is given */
	   break;

      case D2_ADD_AV:
	   break;

      case D2_ADDATT:
      case D2_DELATT:
      default:
	   /* Error: naming attribute should be added or deleted */
	   ret_value = D26_ERROR;
	   break;
   } /* switch (modification) */
} /* if (at->d26a_nr_recur == 1) */

return (ret_value);
}                                   /*  d26_C16_name_attr_check()          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_C20_sch_mod                                     */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 06.11.91                                            */
/*                                                                    */
/* SYNTAX       : LOCAL Short d26_C20_sch_mod(no_of_attributes,      */
/*                                                   mod_attr)        */
/*                                                                    */
/*                Short              no_of_attributes;                */
/*                D26_md_info        *mod_attr;                       */
/*                                                                    */
/* DESCRIPTION  : This function checks whether the operation is going */
/*                to change the schema attributes of the schema       */
/*                object                                              */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                                                                    */
/*      no_of_attributes    Short           number of attributes to   */
/*                                          be modified               */
/*      mod_attr            D26_md_info *   array of modifications    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :   D2_TRUE         Schema attributes will be modified*/
/*                  D2_FALSE        Schema attributes will not be     */
/*                                  modified                          */
/*                  D26_C10_SCH_MOD_TERM Schema modification will     */
/*                                  terminate                         */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*      d26_objname     D2_name_string      object to be modified     */
/*      d26_schema_name char []             name of schema object     */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_C20_sch_mod(signed16       no_of_attributes,
			        D26_md_info   *mod_attr
			       )

{                                   /*  d26_C20_sch_mod               */

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

	    /* object identifier for time stamp attribute for compare */
static D2_a_type time_stamp = {sizeof(D2_TIME_STAMP) - 1,
			       (Ob_id_string)D2_TIME_STAMP};
	    /* object identifier for SRT attribute for compare        */
static D2_a_type srt = {sizeof(D2_SRT) - 1,(Ob_id_string)D2_SRT};
	    /* object identifier for OCT attribute for compare        */
static D2_a_type oct = {sizeof(D2_OCT) - 1,(Ob_id_string)D2_OCT};
	    /* object identifier for AT attribute for compare         */
static D2_a_type at = {sizeof(D2_AT) - 1,(Ob_id_string)D2_AT};

			    /* return value                           */
signed16   ret_value = FALSE;

register signed16  i;       /* loop variable                          */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if                          /*  schema object is affected by modify   */
  (strcmp((char *) d26_objname,(char *) d26_schema_name) == 0)
{                           /*  look for schema attributes in the     */
			    /*  attribute list                        */
  for                       /*  all attributes of list                */
     (i = 0; i < no_of_attributes; i++)
  {                         /*  check whether it belongs to schema    */
    if                      /*  attribute belongs to schema           */
      (D27_ATTCMP(&mod_attr[i].d26_mtype,&srt) == 0 ||
       D27_ATTCMP(&mod_attr[i].d26_mtype,&oct) == 0 ||
       D27_ATTCMP(&mod_attr[i].d26_mtype,&at) == 0  ||
       D27_ATTCMP(&mod_attr[i].d26_mtype,&time_stamp) == 0 &&
       mod_attr[i].d26_m == D2_DELATT)
    {                       /*  set return value                      */
      ret_value = TRUE;
    } else if                   /*  time stamp is added or replaced       */
	 (D27_ATTCMP(&mod_attr[i].d26_mtype,&time_stamp) == 0 &&
	  (mod_attr[i].d26_m == D2_ADDATT || mod_attr[i].d26_m == D2_REP_AV))
		/* D2_ADDATT generated by "dscstsh()";           */
		/* D2_REP_AV generated by "dscmdatt()" called by */
		/* "save & append"                               */
    {                       /*  set return value                      */
      ret_value = D26_C10_SCH_MOD_TERM;
      break;
    } 
  } 
} 

			    /* return ret_value                       */
return(ret_value);

}                                   /*  d26_C20_sch_mod               */
