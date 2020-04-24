/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: modrdn.c,v $
 * Revision 1.1.10.2  1996/02/18  18:15:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:37:57  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:50:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:37  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:46:14  marrek
 * 	June 1994 code submission.
 * 	[1994/06/20  13:05:05  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:27:47  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:13:51  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:25:26  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:37:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:24:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  15:53:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:26:04  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:58:05  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:33:44  marrek
 * 	avoid 1 internal search
 * 	[1993/02/01  15:37:28  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:44:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:16  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:32:58  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:23:23  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:50:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:32:30  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char modrdn_rcsid[] = "@(#)$RCSfile: modrdn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:15:25 $";
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
/* NAME         : modrdn.c      <DS_REPLACE_RDN>                      */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 10.12.87                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : This module contains the functions used for         */
/*                DS_REPLACE_RDN.                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |10.12.87 |  Original                      | AS |         */
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

#include <d26dsa.h>

/*****  external Includes    *****/

#include <dce/d27util.h>
#include <malgcfe.h>

/*****  internal Includes    *****/

#include <d26schema.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/
#define D26_B00_ERR_COMM_LEN   100  /* length of comment for exit or error */
#define D26_RPLRDN_LEN        4000  /* length of RPLRDN for u00 func. */


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

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/
static signed16 d26_B00_check (D2_name_string, D2_name_string, D26_dninfo *,
	signed16 *, byte **, signed32 *);
static signed16 d26_B01_analyse_reprdn_msg (D23_in_replrdn *, D23_ds_name **,
	char **, signed16 *, char **, signed16 *, boolean *, byte **);
static signed16 d26_B02_not_exists_or_lock (D23_in_replrdn *, boolean,
	D2_name_string, signed16, D26_acl *, long *, byte **, signed32 *,
	D2_name_string);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_B_replace_rdn       [DS_REPLACE_RDN]              */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        10.12.87                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_B_replace_rdn (op_id,rep_in,dap_in,  */
/*                                         rep_out,length)            */
/*                                                                    */
/*              Short              op_id;        ->                   */
/*              D23_in_replrdn     *rep_in;      ->                   */
/*              char              **dap_in;      <-                   */
/*              Octet             **rep_out;     <-                   */
/*              Int                *length;      <>                   */
/*                                                                    */
/* DESCRIPTION: This function represents the main function of the     */
/*              DSA service element DS_REPLACE_RDN. The relative      */
/*              distinguished name (that is the last namepart of      */
/*              a distinguished name) of a leaf is replaced. The      */
/*              type must the new relative distinguished name must    */
/*              be equal to the old and the value must be different   */
/*              to the old.                                           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Short            op_id;        IPC operation ID               */
/*      D23_in_replrdn  *rep_in        invoke message                 */
/*                                                                    */
/*      Int             *length        INPUT OUTPUT parameter         */
/*                                     -> memory length of rep_out    */
/*                                     <- data length of rep_out      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      Char            **dap_in        pointer to dap-request        */
/*      Octet           **rep_out       result (or error) message     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short    D2_NO_ERR     no error occurred                       */
/*                                                                    */
/*              D26_ERROR     any error occurred                      */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        sprintf                                                     */
/*                                                                    */
/*        d26_B00_check                                               */
/*        d26_B01_analyse_reprdn_msg                                  */
/*        d26_B02_not_exists_or_lock                                  */
/*        d26_B03_change_main_entry                                   */
/*                                                                    */
/*        d26_u04_set_name_error_apdu                                 */
/*        d26_u08_calc_len                                            */
/*        d26_u09_init_dsa                                            */
/*        d27_010_set_error_apdu                                      */
/*        d26_u80_check_child                                         */
/*                                                                    */
/*        d26_i02_upd_file_dnlist                                     */
/*        d26_i14_unlock                                              */
/*                                                                    */
/*        d27_007_norm_name                                           */
/*        d27_012_remove_blanks                                       */
/*        d27_018_sort_name					      */
/*                                                                    */
/* NOTES: Note the following restriction for the naming attributes:   */
/*             The naming attribute can be a single valued attribute  */
/*             or a multiple valued attribute with a limited number   */
/*             of values.                                             */
/*             The naming attribute cannot be a multiple valued       */
/*             attribute with a unlimited number of values.           */
/*                                                                    */
/*        Attributes with more than one AVA are now supported.        */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_B_replace_rdn ( 
				signed16          op_id,
				D23_in_replrdn   *rep_in,
				signed32          in_length,
				byte            **dap_in,
				byte            **rep_out,
				signed32         *length
			   )

{                               /*  d26_B_replace_rdn             */

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
static String    function = "d26_B_replace_rdn";

/* AUTO      ...    */
 signed16          rop_id;      /* reduced operation ID           */

				/* pointer to error apdu               */
 D23_rserror       *err_ptr = (D23_rserror *) NULL;

				/* variables for return values         */
 signed16          return_value = D2_NO_ERR; /*return value to calling */
				/* function                */
 signed16          ret_val;     /* return value of called  */
				/* funtions                */
 signed32          rem_len;     /* rem length for name error       */

 D2_name_string    old_entryname;/* ptr to old entryname in message*/
 signed16          l_old_entryname;  /* length of old entryname    */
				/* (including EOS)                 */
 D23_ds_name       *ds_old;     /* ptr to dist. name information of*/
				/* old entryname in message        */

				/* normed value of old entryname   */
 char              n_old_entryname [D2_DNL_MAX+1];
				/* normed value of new entryname   */
				/* (that is: old entryname with    */
				/* last rdn replaced by new rdn)   */
 char              n_new_entryname [D2_DNL_MAX+1];
 char              e_new_entryname [D2_DNL_MAX+1];

 D2_name_string    new_rdn;     /* ptr to new rdn in message       */
 signed16          l_new_rdn;   /* length of new rdn               */
				/* (including EOS)                 */
 D26_dninfo        dninfo;      /* informations about entryname    */
 signed16          last;        /* index of new rdn in dninfo      */
 signed16          sup_len;     /* length of superior's name       */
 boolean           del_old_rdn; /* flag for deleting old rdn       */

				/* memory for DS_SEARCH output msg */
 D26_acl           acl[D2_ACLIDX_MAX];
 signed32          mk;          /* masterknowledge attribute       */
 D26_acl           sup_acl[D2_ACLIDX_MAX];
 signed32          sup_mk;      /* masterknowledge attribute       */
 boolean           rec_flag;    /* recurring attribute flag        */
 char              *record;     /* main entry record supplied by   */
				/* DS_SEARCH                       */
 char             *save_rec;   /* for saving record found in u80  */
 boolean          alias_flag;  /* flag whether the read entry is  */
				    /* an alias                        */
 signed16         obj_idx;     /* object index in SRT == obj_nr   */
				    /* list of indices in OCT          */
 signed32         ob_cl[D26_OCL_VAL_COUNT + 1];

 signed32         dnl_idx;     /* dummy var. for i02              */

 signed32         dap_len;      /* length of DAP-message           */
 D26_ocl_arc     *oa_ptr;       /* points to ocl-arc in SRT        */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

rop_id  = op_id > D2_ADM_OPID ? op_id - D2_ADM_OPID : op_id;

/*********/
/* entry */
/*********/

if                               /*  error during initialization of DSA    */
   (d26_u09_init_dsa (0))
{                                /*  set error apdu                        */
     d27_010_set_error_apdu ((D23_rserror *) *rep_out, D2_PERM_ERR,
			      D2_SYS_ERR, D2_NOT_SPEC, length);
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 1));
     return (D26_ERROR);         /*  exit funtion                          */
}

/*******************/
/* analyse message */
/*******************/

if                               /*  d26_B01_analyse_reprdn_msg fails   */
   (d26_B01_analyse_reprdn_msg (rep_in, &ds_old, (char **) &old_entryname,
				&l_old_entryname, (char **) &new_rdn,
				&l_new_rdn, &del_old_rdn, dap_in)
     != D2_NO_ERR)
{                                /*  MA_LOG and exit function           */
				 /*  set error apdu                     */
     d27_010_set_error_apdu ((D23_rserror *) *rep_out, D2_CALL_ERR,
				   D2_PAR_ERR, D2_NOT_SPEC, length);

     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 2));
     return (D26_ERROR);
}
dap_len = in_length - ((char *)*dap_in - (char *) rep_in);

if (ds_old->d23_dscomplete)      /* name could be completely decoded      */
{
  if                                /* root object should be modified */
    (l_old_entryname == 0)
  {                                 /* write name error               */
    dninfo.d26_rd_anz = 0;
    rem_len           = *length;
    if                              /* name error cannot be written   */
      (d26_u04_set_name_error_apdu(rep_out,length,&rem_len,
       D2_NFOUND,(signed32)D26_IDX_NOT_DEF,&dninfo) == D26_ERROR)
    {                               /* log error                      */
    }
    *length -= rem_len;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 3));
    return(D26_ERROR);
  }
				 /*  sort multiple AVAs and trace changes */
  d27_018_sort_name(old_entryname);
  strcpy((char *) d26_objname, (char *) old_entryname);

  /* changing of schema object not allowed */
  if (strcmp((char *) old_entryname,(char *) d26_schema_name) == 0)
  {                                /*  set error message              */
    d27_010_set_error_apdu ((D23_rserror *) *rep_out, D2_CALL_ERR,
				   D2_PAR_ERR, D2_UNWILLING, length);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 11));
    return(D26_ERROR);
  } 

				 /*  sort multiple AVAs and trace changes */
  d27_018_sort_name(new_rdn);

  /**********************************************/
  /* removing of blanks in old name and new rdn */
  /**********************************************/
  l_old_entryname = d27_012_remove_blanks ((String) old_entryname,
					   l_old_entryname,
					   (String) old_entryname);
  l_new_rdn = d27_012_remove_blanks ((String) new_rdn, l_new_rdn,
				     (String) new_rdn);
  /************************************************/
  /* build name of superior object (normed value) */
  /************************************************/
			     /*  build name of superior object (normed val)*/
			     /* calculate length of superior's name        */
  sup_len = d26_u08_calc_len (old_entryname, 1);
			     /* copy superior's name to new entryname      */

  d27_007_norm_name ((String) old_entryname, sup_len, n_new_entryname);
  n_new_entryname[sup_len] = D2_EOS;
  if (sup_len > 0)
  {
       sup_len++;
  }

  /************************/
  /* lock superior object */
  /************************/

  ret_val = d26_B02_not_exists_or_lock (rep_in, TRUE,
					(D2_name_string) n_new_entryname,
					(signed16) sup_len, sup_acl, &sup_mk,
					rep_out, length,
					(D2_name_string) e_new_entryname);

  if                         /*  entry with new entryname already exists   */
     (ret_val != D2_NO_ERR)
  {                              /*  error                                 */
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 4));
     return (D26_ERROR);         /*  exit function                         */
  }                              /*  d26_B02_not_exists_or_lock fails */


  /*************************/
  /* check old and new rdn */
  /*************************/
  ret_val = d26_B00_check (old_entryname, new_rdn, &dninfo, &last,
			   rep_out, length);

  switch ((int) ret_val)  /*  check of old and new rdn                  */
  {
  case D2_NO_ERR:
			  /*  no error occurred: break                  */
       break;
  case D26_NO_CHANGE:
                        /*  old value == new value                    */
			/*  exit function successfull                 */
     d26_i14_unlock ();
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 5));
     return (D2_NO_ERR);
  case D26_ERROR:
  default:
                        /*  any error occurred                        */
			/*  exit function erroneous                   */
     d26_i14_unlock ();
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 6));
     return (D26_ERROR);
  }



  /**************************************/
  /* build new entryname (normed value) */
  /**************************************/
			     /*  build new entryname                       */
			     /* copy new rdn to new entryname              */
  if (sup_len > 0)
  {
       n_new_entryname[sup_len - 1] = D2_RDN_SEP;
       e_new_entryname[sup_len - 1] = D2_RDN_SEP;
  }
  strcat(e_new_entryname,(char *) new_rdn);
  d27_007_norm_name ((String) new_rdn, (signed16) l_new_rdn,
		      &n_new_entryname[sup_len]);
}

/*************************************************/
/* check whether entry is a leaf and read record */
/*************************************************/
ret_val = d26_u80_check_child (ds_old, (byte *) rep_in, rep_out,
			       length, dap_in, &dap_len, FALSE,
			       d26_attr, ob_cl, acl, &mk, &rec_flag, &record,
			       &alias_flag, &obj_idx, op_id, sup_acl, sup_mk);

if                           /*  entry isn't leaf or entry isn't found or  */
			     /*  access not allowed                        */
   (ret_val != D2_NO_ERR)
{                                /*  unlock used files                     */
  d26_i14_unlock ();
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 61));
  return (D26_ERROR);         /*  exit function                         */
}                                /*  d26_u80_check_child fails */
  else if (obj_idx == D26_IDX_NOT_DEF)
{
  /* request was successfully chained -> return !!! */
  d26_i14_unlock ();
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_val));
  return(ret_val);
}

/* check wether the object_class of the found object is allowed under */
/* the new name structure                                             */
if (!alias_flag)
{
  for (oa_ptr = d26_srt[dninfo.d26_rdn[last].d26_r_idx].d26s_ocl_arc;
       oa_ptr->d26s_oclass > -1 && oa_ptr->d26s_oclass != ob_cl[0];
       oa_ptr++)
  {}
  if (oa_ptr->d26s_oclass == -1)  /* error - OCL not under allowed OCLs */
  {
    d27_010_set_error_apdu((D23_rserror *) *rep_out, D2_CALL_ERR,
			 D2_PAR_ERR, D2_NAMING_VIOLATION, length);
    d26_i14_unlock ();
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 7));
    return(D26_ERROR);
  }
}
/* save record found in u80 (search in B02 will overwrite it) */
if ((save_rec = (char *)D27_MALLOC(svc_c_sev_warning,d26_oreclen)) == NULL)
{
  d27_010_set_error_apdu((D23_rserror *)*rep_out,D2_TEMP_ERR,D2_SYS_ERR,
		   D2_UNWILLING,length);
   d26_i14_unlock ();
   d26_i14_unlock ();
   DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 8));
   return (D26_ERROR);         /*  exit function                    */
}
memcpy(save_rec,record,d26_oreclen);

/**********************************************************/
/* check whether an entry with the new rdn already exists */
/**********************************************************/
ret_val = d26_B02_not_exists_or_lock (rep_in, FALSE,
				    (D2_name_string) n_new_entryname,
				    ((signed16)(strlen(n_new_entryname) + 1)),
				    NULL, NULL, rep_out, length,
				    (D2_name_string) n_old_entryname);
					    /* last parameter not needed */

if                         /*  entry with new entryname already exists   */
 (ret_val != D2_NO_ERR)
{                          /*  error                                 */
   d26_i14_unlock ();
   DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 9));
   return (D26_ERROR);         /*  exit function                    */
}                              /*  d26_B02_not_exists_or_lock fails */

/*********************/
/* change main entry */
/*********************/
				 /*  change main entry                     */
ret_val = d26_B03_change_main_entry (&dninfo, last, del_old_rdn,
				     ob_cl[0], obj_idx, alias_flag, save_rec,
				     rep_out, length);
if                               /*  error occurred during change main entry*/
   (ret_val != D2_NO_ERR)
{                                /*  unlock used files                     */
     d26_i14_unlock ();
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 10));
     return (D26_ERROR);         /*  exit function                         */
}                                /*  error during change main entry        */
free(save_rec);

/******************/
/* update dn list */
/******************/
			     /*  build old entryname                       */
d27_007_norm_name ((String) old_entryname, (signed16) l_old_entryname,
		   n_old_entryname);
			     /*  update dn list                            */
ret_val = d26_i02_upd_file_dnlist((D2_attr_mod) D2_REP_AV,
				  (D2_name_string) n_old_entryname,
				  D2_IGNORE,(D2_name_string) e_new_entryname,
				  (signed32) NULL, TRUE, &dnl_idx);

/********/
/* exit */
/********/

if                              /*  operation successfull                  */
   (return_value == D2_NO_ERR)
{                               /*  generate result message (OK)           */
     record = (char *)*rep_out;
     *length = 0;

     if                    /*  DSP was coming in                         */
	(d26_chain_arg != (D23_chain_arg *)NULL)
     {                     /*  write chaining result                     */
       ((D23_chain_res *) record)->d23_chrtag  = D23_CHRTAG;
       record += D23_S_CHRES;
       *length += D23_S_CHRES;
     }                     /*                                            */
			    /*  generate result message (OK)  */
     ((D23_eom *) record)->d23_eomtag = D23_EOMTAG;
     *length += sizeof (D23_eom);
     err_ptr = (D23_rserror *) NULL;
} else {                            /*  set error message                */
     d27_010_set_error_apdu ((err_ptr = (D23_rserror *) *rep_out),
			      D2_TEMP_ERR, D2_REM_ERR, D2_NO_RESPONSE,
			      length);
     err_ptr = (D23_rserror *) *rep_out;
}                               /*  operation successfull */

				/*  unlock used files                      */
d26_i14_unlock ();

				/*  trace function exit                    */
DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_value));
return (return_value);

}                               /*  d26_B_replace_rdn */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_B00_check                                         */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        06.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function checks whether the type of the last     */
/*              name part (new rdn) isn't changed.                    */
/*                                                                    */
/*              When the value of the new rdn is equal to the old     */
/*              D26_NO_CHANGE and the result message is returned      */
/*              so that the d26_B_replace_rdn can be left immediately.*/
/*                                                                    */
/*              In error cases D26_ERROR and the error apdu is        */
/*              returned.                                             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*       D2_name_string   old_name      name of the entry which should*/
/*                                      be changed                    */
/*                                                                    */
/*                                                                    */
/*       D2_name_string   new_rdn       new rdn                       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*       D26_dninfo       *new_dninfo   distinguished name information*/
/*                                      of new entry name             */
/*                                                                    */
/*       Short            *last_rdn     index of new rdn in           */
/*                                      'new_dninfo'                  */
/*                                                                    */
/*       Octet            **rep_out     result message of DS_REPLACE_ */
/*                                      RDN in error cases and in the */
/*                                      case of no change             */
/*                                                                    */
/*       Int              *len          length of result message      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*       Short   D2_NO_ERR        no error occured                    */
/*                                                                    */
/*               D26_NO_CHANGE    new rdn is equal to old rdn         */
/*                                (result msg and len is supplied)    */
/*                                                                    */
/*               D26_ERROR        error occured                       */
/*                                (error apdu and len is supplied)    */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                  D26_at      *d26_at;    pointer to AT             */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_u06_get_name                                            */
/*        d27_010_set_error_apdu                                      */
/*        strcmp                                                      */
/*                                                                    */
/* NOTES: old_name and new_rdn are already sorted for multiple AVAs.  */
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/


static signed16 d26_B00_check (D2_name_string    old_name,
			D2_name_string    new_rdn,
			D26_dninfo       *new_dninfo,
			signed16         *last_rdn,
			byte            **rep_out,
			signed32         *len
		       )

{                               /*  d26_B00_check                     */

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
 D26_dninfo     old_dninfo;    /* distinguished name information of old */
				   /* entryname                             */

				   /* normed value of last rdn of old name  */
				   /* (which should be replaced by new_rdn).*/
				   /* It contains for multiple AVAs only    */
				   /* one value at a time.                  */
 char           n_old_rdn[D2_NP_LEN+1];
				   /* one normed value of the new rdn       */
 char           n_new_rdn[D2_NP_LEN+1];
 D26_rdninfo   *p_old_rdninfo,
		*p_new_rdninfo;
				   /* pointer to AVA components             */
 D26_ava       *old_ava,
               *new_ava;

 boolean	same_values = TRUE; /* indicates same multiple AVAs   */

/* FASTAUTO  ...    */
register signed16         ret_val;
register signed16	       i;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/*********************************/
/* get dist. name inf. of old dn */
/*********************************/
old_dninfo.d26_rd_anz = 0;
ret_val = d26_u06_get_name (old_name, &old_dninfo);
if                           /*  old name not correct                      */
   (ret_val != D2_NO_ERR)
{                            /*  error: old rdn not correct                */
			     /*  set error codes                           */
     d27_010_set_error_apdu ((D23_rserror *) *rep_out, D2_CALL_ERR,
			      D2_PAR_ERR, D2_NAMING_VIOLATION, len);
     return (D26_ERROR);     /*  exit function                             */
}                            /*  old name not correct                      */

/**********************************/
/* get dist. name inf. of new rdn */
/**********************************/
*new_dninfo = old_dninfo;
new_dninfo->d26_rd_anz = old_dninfo.d26_rd_anz - 1;
ret_val = d26_u06_get_name (new_rdn, new_dninfo);
if                           /*  old name not correct                      */
   (ret_val != D2_NO_ERR)
{                            /*  error: new rdn not correct                */
			     /*  set error codes                           */
     d27_010_set_error_apdu ((D23_rserror *) *rep_out, D2_CALL_ERR,
			      D2_PAR_ERR, D2_NAMING_VIOLATION, len);
     return (D26_ERROR);     /*  exit function                             */
}                            /*  old name not correct                      */

/*********************************************/
/* compare type and value of old and new rdn */
/*********************************************/
if                           /*  number of nameparts not equal             */
   (old_dninfo.d26_rd_anz != new_dninfo->d26_rd_anz)
{                            /*  error: number of nameparts not equal      */
			     /*  set error codes                           */
     d27_010_set_error_apdu ((D23_rserror *) *rep_out, D2_CALL_ERR,
			      D2_PAR_ERR, D2_CONSTRAINT_VIOLATION, len);
     return (D26_ERROR);     /*  exit function                             */
} else {
			     /*  get index of new rdn in new_dninfo        */
     *last_rdn = new_dninfo->d26_rd_anz - 1;
     p_old_rdninfo = &old_dninfo.d26_rdn[*last_rdn];
     p_new_rdninfo = &new_dninfo->d26_rdn[*last_rdn];
			     /*  check types				   */
     if                      /*  compare indices into SRT                  */
	(p_old_rdninfo->d26_r_idx != p_new_rdninfo->d26_r_idx)
     {   
	  return (D2_NO_ERR); /*  exit function                            */
     } 

     for (i = 0; i < p_old_rdninfo->d26_r_nava; i++)
     { 
	old_ava = &p_old_rdninfo->d26_r_ava[i];
        new_ava = &p_new_rdninfo->d26_r_ava[i];
			     /* norm old rdn                               */
	d27_651_norm_T61String(old_ava->d26_a_val,(byte *)n_old_rdn,
	  old_ava->d26_a_len);
	n_old_rdn[old_ava->d26_a_len] = D2_EOS;
			     /* norm new rdn                               */
	d27_651_norm_T61String(new_ava->d26_a_val,(byte *)n_new_rdn,
	  new_ava->d26_a_len);
	n_new_rdn[new_ava->d26_a_len] = D2_EOS;

	if                 /*  old value != new value                      */
	   (same_values == TRUE && strcmp (n_old_rdn, n_new_rdn) != 0)
	{                  /*  indicate different values                   */
	   same_values = FALSE;
	} 
     } 

     if                 /*  old value == new value                         */
	( same_values == TRUE )
     {                  /*  set result message (OK)                        */

	((D23_eom *) *rep_out)->d23_eomtag = D23_EOMTAG;
	*len = sizeof (D23_eom);

	return (D26_NO_CHANGE);
     }                  /*  old value == new value */
}                            /*  old name not correct                      */

return (D2_NO_ERR);
}                               /*  d26_B00_check                     */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_B01_analyse_reprdn_msg                            */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        30.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function analyses the IPC message received by    */
/*              the function DS_REPLACE_RDN.                          */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D23_in_replrdn      *rep_in      pointer to DS_REPLACE_RDN    */
/*                                       invoke message               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      D23_ds_name         **ds_old     pointer to distinguished name*/
/*                                       information in message       */
/*                                       (needed for fct. u80)        */
/*      Char                **old_name   pointer to old entryname     */
/*                                       contained in message         */
/*      Ushort              *l_old_name  length of old entryname      */
/*                                       (including EOS)              */
/*      Char                **new_rdn    pointer to new rdn           */
/*                                       contained in message         */
/*      Ushort              *l_new_rdn   length of new rdn            */
/*                                       (including EOS)              */
/*      Bool                *del_old_rdn flag indicating whether old  */
/*                                       rdn should be deleted        */
/*      String              *dap_mess    pointer to begin of dap_mess */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Short  D2_NO_ERR    no error occured                          */
/*                                                                    */
/*             D26_ERROR    DS_REPLACE_RDN invoke message not correct */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*             d26_c01_copy_chaining_arguments                        */
/*             d26_c02_copy_common_arguments                          */
/*             sprintf                                                */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/


static signed16 d26_B01_analyse_reprdn_msg (
					D23_in_replrdn      *rep_in,
					D23_ds_name       **ds_old,
					char              **old_name,
					signed16            *l_old_name,
					char               **new_rdn,
					signed16            *l_new_rdn,
					boolean             *del_old_rdn,
					byte               **dap_mess
				    )

{                               /*  d26_B01_analyse_reprdn_msg        */

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
 byte        *ptr_msg;    /* pointer to message      */

/* FASTAUTO  ...    */
register unsigned16      tag;         /* for check of tag        */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

                               /*  set ptr_msg to begin of message    */
ptr_msg = (byte *) (rep_in + 1);
                                /* */

/* skip D23_chain_arg */
d26_a02_analyse_chaining_args(&ptr_msg);
*dap_mess = ptr_msg;

/* skip D23_com_arg */
d26_a03_analyse_common_arguments(&ptr_msg);

/******************/
/* find repinfo   */
/******************/
if                                  /*  tag is not distinguished name      */
   ((tag = ((D23_repinfo *) ptr_msg)->d23_reptag) != D23_REPTAG)
{                                   /*  error: message incorrect           */
				    /*  MA-LOG + MA-ERROR                  */
     return (D26_ERROR);            /* */
}                                   /*  tag is not dn */

				    /*  flag whether deleting old rdn      */
				    /*  (OUT_PAR: del_old_rdn)             */
*del_old_rdn = ((D23_repinfo *) ptr_msg)->d23_delordn;
ptr_msg = ptr_msg + D23_S_REP;


/******************/
/* find entryname */
/******************/
if                                  /*  tag is not distinguished name      */
   ((tag = ((D23_ds_name *) ptr_msg)->d23_dstag) != D23_NAMTAG)
{                                   /*  error: message incorrect           */
				    /*  MA-LOG + MA-ERROR                  */
     return (D26_ERROR);            /* */
}                                   /*  tag is not dn */

				    /*  dn information of old name in msg  */
				    /*  (OUT_PAR: ds_old)                  */
*ds_old = (D23_ds_name *) ptr_msg;
				    /*  set ptr to entryname in message    */
				    /*  (OUT_PAR: old_name)                */
*old_name = (char *)(ptr_msg + sizeof (D23_ds_name));
				    /*  set length of entryname            */
				    /*  (OUT_PAR: l_old_name)              */
*l_old_name = ((D23_ds_name *)ptr_msg)->d23_dslen;

				    /*  scan message (set ptr_msg)         */
ptr_msg = (byte *) (*old_name + ((D23_ds_name *)ptr_msg)->d23_dsoff);

/****************/
/* find new rdn */
/****************/
if                                  /*  tag is not distinguished name      */
   ((tag = ((D23_ds_name *) ptr_msg)->d23_dstag) != D23_NAMTAG)
{                                   /*  error: message incorrect           */
				    /*  MA-LOG + MA-ERROR                  */
     return (D26_ERROR);            /* */
}                                   /*  tag is not dn */

				    /*  set ptr to new rdn in message      */
				    /*  (OUT_PAR: new_rdn)                 */
*new_rdn = (char *)(ptr_msg + sizeof (D23_ds_name));
				    /*  set length of new rdn              */
				    /*  (OUT_PAR: l_new_rdn)               */
*l_new_rdn = ((D23_ds_name *)ptr_msg)->d23_dslen;

				    /*  scan message (set ptr_msg)         */
ptr_msg = (byte *)(*new_rdn + ((D23_ds_name *)ptr_msg)->d23_dsoff);

/***************************/
/* look for end of message */
/***************************/

if                                  /*  end of message not reached         */
   ((tag = ((D23_eom *)ptr_msg)->d23_eomtag) != D23_EOMTAG)
{                                   /*  message wrong                      */
				    /*  MA-LOG + MA-ERROR                  */
     return (D26_ERROR);
}                                   /*  message wrong */

return (D2_NO_ERR);
}                               /*  d26_B01_analyse_reprdn_msg        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_B02_not_exists_or_lock                            */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        06.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function checks whether an entry with a given    */
/*              doesn't exist (when lock == D2_FALSE) or locks the    */
/*              the record found (when lock == D2_TRUE, in this case  */
/*              the function is used to lock the superior object).    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*       D23_in_replrdn       *rep_in    invoke message of DS_REPLACE_*/
/*                                       RDN (needed for DS_SEARCH    */
/*                                       invoke msg parameters)       */
/*                                                                    */
/*                                                                    */
/*        Bool                lock        flag indicating whether     */
/*                                        the record read should be   */
/*                                        locked.                     */
/*                              D2_FALSE  record should not be locked */
/*                                        check whether object exists */
/*                                        is done and corresponding   */
/*                                        error apdu is supplied      */
/*                                        (see also return value)     */
/*                              D2_TRUE   record will be locked       */
/*                                        (function is used to lock   */
/*                                        superior)                   */
/*                                                                    */
/*       D2_name_string       name       name of searched entry       */
/*                                                                    */
/*       Ushort               name_len   length of name               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*       Octet		      **error    error message (contains      */
/*                                       ret. codes in case of errors)*/
/*                                                                    */
/*       Int                  *l_err     length of error message      */
/*       D2_name_string       sup_ename  exact name of searched entry */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*       Short  D2_NO_ERR           in case of 'lock == D2_FALSE':    */
/*                                     object with entryname 'name'   */
/*                                     doesn't exist                  */
/*                                  in case of 'lock == D2_TRUE':     */
/*                                     object with entryname 'name'   */
/*                                     found and locked               */
/*                                                                    */
/*              D2_ALREADY_EXISTS   object with 'name' exists         */
/*                                  (error apdu is supplied with      */
/*                                   error code D2_ALREADY_EXISTS)    */
/*                                  this return value is supplied only*/
/*                                  in case of 'lock == D2_FALSE'     */
/*                                                                    */
/*              D26_ERROR           any error occurred (error apdu is */
/*                                  supplied)                         */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        D2_ALIGN                                                    */
/*                                                                    */
/*        d26_u00_search                                              */
/*        d27_010_set_error_apdu                                      */
/*        strcpy                                                      */
/*        strlen                                                      */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_B02_not_exists_or_lock (
					D23_in_replrdn   *rep_in,
					boolean           lock,
					D2_name_string    name,
					signed16          name_len,
					D26_acl          *sup_acl,
					long             *sup_mk,
					byte           **error,
					signed32        *l_err,
					D2_name_string   sup_ename
				    )

{                               /*  d26_B02_not_exists_or_lock        */

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
 signed16      return_value = D2_NO_ERR; /* return value to calling */
					 /* function                */
 signed16      ret_val;                  /* return value of called  */
					 /* funtions                */

					 /* memory for DS_SEARCH input msg  */
 char             in_search[D26_RPLRDN_LEN];
 D23_insearch     *s_in = (D23_insearch *) in_search;

 D26_acl          *acl_ptr;          /* pointer to acl for search       */
 signed32         *mk_ptr;           /* pointer to mk for search        */
 D26_acl          acl[D2_ACLIDX_MAX];
 signed32         mk;                /* masterknowledge attribute       */
 boolean          rec_flag;          /* recurring attribute flag        */
 char             *record;           /* main entry record supplied by   */
			 	     /* DS_SEARCH                       */
 boolean          alias_flag;        /* flag whether the read entry is  */
				     /* an alias                        */
 signed16         obj_idx;           /* object index in SRT == obj_nr   */
                                     /* list of indices in OCT          */
 signed32         ob_cl[D26_OCL_VAL_COUNT + 1];
 signed32         rem_len;           /* rem length for search          */
 D23_chain_arg    *ca_save;          /* save the chaining args         */

/* FASTAUTO  ...    */
register char          *msg_ptr;         /* pointer to DS_SEARCH invoke msg */
register D23_ds_name   *dn_name_ptr;     /* pointer to distinguished name   */
register D23_rq_info   *rq_info_ptr;     /* pointer to requested information*/
register D23_com_arg   *com_arg_ptr;     /* pointer to common arguments     */
register D23_srcharg   *srcharg_ptr;     /* pointer to search information   */


/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/*************************/
/* set search parameters */
/*************************/
                        /*  fill message header                       */
s_in->d23_Ihdr.d23_versno = rep_in->d23_Ohdr.d23_versno;
s_in->d23_Ihdr.d23_dirid  = rep_in->d23_Ohdr.d23_dirid;
s_in->d23_Ihdr.d23_bindid = rep_in->d23_Ohdr.d23_bindid;

			     /*  set common argumemts                      */
com_arg_ptr = (D23_com_arg *) (s_in + 1);
com_arg_ptr->d23_cmtag = D23_CMTAG;
com_arg_ptr->d23_cmali = D2_NOT_DEFINED;
					     /* don't dereference alias */
					     /* and no referrals        */
					     /* and local scope         */
com_arg_ptr->d23_cmctr.d23_scopt = lock ? D2_DREFALIAS :
				D2_DREFALIAS | D2_LOCALSCOPE | D2_NOREFERRAL;
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

			     /*  set entry name                           */
				  /* set msg ptr to begin of D23_ds_name */
dn_name_ptr = (D23_ds_name *) (srcharg_ptr + 1);
				  /* copy ds_name information: D23_ds_name*/
dn_name_ptr->d23_dstag = D23_NAMTAG;          /* tag                 */
dn_name_ptr->d23_dslen = name_len;            /* name length         */
dn_name_ptr->d23_dscomplete = TRUE;        /* complete flag       */

				  /* set ptr to begin of name string      */
msg_ptr = (char *) dn_name_ptr + sizeof (D23_ds_name);
				  /* copy entry name to message           */
if (name_len == 0)
{    /* searched object is root: read schema object */
     strcpy (msg_ptr, D2_NAME_SCHEME);
     dn_name_ptr->d23_dslen = strlen (msg_ptr) + 1; /* name length         */
} else {
     memcpy ((Octet_string) msg_ptr, name, name_len);
} 
				  /* offset to next data */
dn_name_ptr->d23_dsoff = D2_ALIGN (dn_name_ptr->d23_dslen);

			     /*  requested information (NONE)              */
				  /* set ptr to begin of requested inf.   */
rq_info_ptr = (D23_rq_info *) (msg_ptr + dn_name_ptr->d23_dsoff);
				  /* copy requested inf: D23_rq_info      */
rq_info_ptr->d23_rqtag = D23_REQTAG;  /* tag                            */
rq_info_ptr->d23_rqreq = D2_R_TYP;    /* attribute type requested       */
rq_info_ptr->d23_rqnbr = 0;           /* number of requested attributes */

			     /*  fill end of message                       */
msg_ptr = (char *) rq_info_ptr + sizeof (D23_rq_info);
((D23_eom *) msg_ptr)->d23_eomtag = D23_EOMTAG;
                         /* */

/*********************/
/* search the object */
/*********************/
                        /*  search the object                         */
if (lock) /* mk and acl of superior are needed */
{
  mk_ptr = sup_mk;
  acl_ptr = sup_acl;
} else {
  mk_ptr = &mk;
  acl_ptr = acl;
}

/* for this local search the chaining arguments are not relevant - set */
/* them to 0 */
ca_save = d26_chain_arg;
d26_chain_arg = (D23_chain_arg *) NULL;

ret_val = d26_u00_search (lock, D23_REPLRDN, (byte *) s_in, l_err,
			  error, &rem_len, sup_ename, d26_attr,
			  ob_cl, acl_ptr, mk_ptr, &rec_flag, &record,
			  &alias_flag, &obj_idx);

/* reset the chaining arguments */
d26_chain_arg = ca_save;

/*******************************/
/* analyse search return value */
/*******************************/
if (lock == TRUE)
{    /* object should be locked */
     if (ret_val != D2_REF_ERROR && ret_val != D2_NO_ERR)
     {    /* d26_u00_search fails */
	  *l_err -= rem_len;
	  return_value = D26_ERROR;
     }  /* d26_u00_search fails */
} else { /* check should be done whether object doesn't exist */
     switch ((int) ret_val)  /*  return value of d26_u00_search          */
     {
     case D2_NFOUND:
	                     /*  object wasn't found (wished result)     */
	  return_value = D2_NO_ERR;
	  break;

     case D2_NO_ERR:         /*  object was found                        */
     case D2_ACCESS_RIGHTS_INSUFFICIENT: /* */
	                     /*  set error apdu: object already exists   */
	  d27_010_set_error_apdu ((D23_rserror *) *error, D2_CALL_ERR, 
				   D2_PAR_ERR, D2_ALREADY_EXISTS, l_err);
	  return_value = D2_ALREADY_EXISTS; 
	                     
	  break;

     case D26_ERROR:         /*  otherwhise                              */
     default:
	                     /*  search fails                            */
	  *l_err -= rem_len;
	  return_value = D26_ERROR;
	   
	  break;
     }
}  /* (lock == D2_TRUE) */

d26_u41_free_referrals();
return (return_value);
}                               /*  d26_B02_not_exists_or_lock          */
