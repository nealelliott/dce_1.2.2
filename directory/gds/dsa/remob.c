/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: remob.c,v $
 * Revision 1.1.10.2  1996/02/18  18:15:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:50:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:42  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:46:17  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:37:05  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:13  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:28:02  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:13:53  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:25:38  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:37:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:24:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  15:58:11  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:26:55  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:58:41  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:34:21  marrek
 * 	avoid 1 internal search
 * 	[1993/02/01  15:39:46  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:44:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:31  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:33:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:24:08  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:52:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:32:44  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char remob_rcsid[] = "@(#)$RCSfile: remob.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:15:31 $";
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
/* NAME         : d26remob.c  < DS_REMOVE_OBJECT >                    */
/*                                                                    */
/* AUTHOR       : Volpers                                             */
/* DATE         : 17.03.88                                            */
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
/*   0.1    |17.03.88 |  Original                      | HV |         */
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

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_8_remove_object (...)                             */
/*                                                                    */
/* AUTHOR:      Volpers D AP 11                                       */
/* DATE:        17.03.88                                              */
/*                                                                    */
/* SYNTAX       : Short d26_8_remove_object (op_id, inv, in_length,   */
/*                                           dap_in,res,err)          */
/*                                                                    */
/* DESCRIPTION: <text>                                                */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                Short           op_id;    IPC operation ID          */
/*                String          *a_in;    invoke message            */
/*                Int            in_length; exact length of input m.  */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                Char            **a_out;   result (or error) message */
/*                Char            **dap_in; pointer to dap-request    */
/*                Int             *length;  length of message         */
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
/*                  d26_u04_set_name_error_apdu()                     */
/*                  d27_010_set_error_apdu                            */
/*                  d26_a02_analyse_chaining_args                     */
/*                  d26_a03_analyse_common_arguments                  */
/*                                                                    */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_8_remove_object (
				signed16        op_id,
				byte           *a_in,
				signed32        in_length,
				byte          **dap_in,
				byte          **a_out,
				signed32       *length
			     )
{                                   /*  d26_8_remove()                */

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

 static char function[] = "d26_8_remove";
 signed16         ret_value = D2_NO_ERR;
 signed16         rop_id;       /* reduced operation ID           */

				/* parameters for DS_SEARCH       */
 D26_acl          acl[D2_ACLIDX_MAX];
				/* list of indices in OCT        */
 signed32         ob_cl[D26_OCL_VAL_COUNT + 1];
 signed32         mk;
 boolean          recurring;
 String           record;
 boolean          al_flag;
 boolean          attr_req = TRUE;
 signed16         obj_idx;

 D23_ds_name     *ds_name;      /* pointer to distinguished name  */
				/* in message block               */
 D26_dninfo       dninfo;       /* informations about entryname   */
 signed32         rem_len;      /* rem length for name error      */
 byte            *scan = (byte *) a_in;
 signed16         i;
 D26_oct         *p_oct_entry; /* pointer to oct entry          */
 D26_at          *p_at_entry;  /* pointer to at entry of attr   */

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
 boolean          time_stamp = FALSE;
 signed32         dummy;

 char             normed_obj[D2_DNL_MAX];
 signed16         l_name;
 byte            *p_off;        /* pointer to messege               */
 signed32         dap_len;      /* length of DAP-message          */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

rop_id  = op_id > D2_ADM_OPID ? op_id - D2_ADM_OPID : op_id;

if                                  /*  error during initialization of DSA */
   (d26_u09_init_dsa(0))
{                                   /*  return(D26_ERROR)             */
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 1));
  d27_010_set_error_apdu((D23_rserror *) *a_out, D2_PERM_ERR, D2_SYS_ERR,
			  D2_NOT_SPEC, length);
  return(D26_ERROR);
}                                   /*                                */

scan += D23_S_REMOBJ;

p_off = scan;
				    /*  tag is Chaining arguments     */
d26_a02_analyse_chaining_args(&scan);
p_off = scan;
*dap_in = scan;
dap_len = in_length - ((char *)*dap_in - (char *) a_in);

				    /*  tag is common arguments       */
d26_a03_analyse_common_arguments(&scan);

ds_name = (D23_ds_name *) scan;
if (ds_name->d23_dscomplete)
{
  if                                /* root object should be removed  */
    (ds_name->d23_dslen == 0)
  {                                 /* write name error               */
    dninfo.d26_rd_anz = 0;
    rem_len           = *length;
    d26_u04_set_name_error_apdu((byte **)a_out,length,&rem_len,
       D2_NFOUND,(signed32)D26_IDX_NOT_DEF,&dninfo);
    *length -= rem_len;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 2));
    return(D26_ERROR);
  }
				   /*  check whether schema has to be */
				   /*  removed                        */
  l_name = d27_012_remove_blanks((String) (ds_name + 1), ds_name->d23_dslen,
				 normed_obj);
  d27_007_norm_name(normed_obj, l_name, normed_obj);
  strcpy((char *) d26_objname, normed_obj);
  if (strcmp((char *) normed_obj, (char *) d26_schema_name) == 0)
  {                                /*  set error message              */
    d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			   D2_UNWILLING, length);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 3));
    return(D26_ERROR);
  } 
}

				  /*  call function d26_u80_.. to   */
				  /*  whether children exist        */
ret_value = (d26_u80_check_child(ds_name, (byte *) a_in, a_out, length,
			    dap_in, &dap_len,
			    attr_req, d26_attr, ob_cl, acl, &mk,
			    &recurring, &record, &al_flag, &obj_idx,
			    op_id, NULL, 0L));
if                                /*  search was not successful     */
  (ret_value)
{
				  /*  unlock used files             */
  d26_i14_unlock();
				  /*  return (D26_ERROR)            */
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 4));
    return(D26_ERROR);
} else if (obj_idx == D26_IDX_NOT_DEF)
{
  /* request was successfully chained -> return ! */
  d26_i14_unlock ();
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 5));
  return(ret_value);
} else {
  if                          /*  delete record                 */
  ((ret_value = d26_850_delete_obj( record, ob_cl[0], al_flag,obj_idx,
       *a_out, length, &p_oct_entry)) != D2_NO_ERR)
  {                                 /*  delete failed                 */
    d27_010_set_error_apdu((D23_rserror *) *a_out, D2_CALL_ERR, D2_PAR_ERR,
			     ret_value, length);
				    /*  unlock used files             */
    d26_i14_unlock();
				    /*  return (D26_ERROR)            */
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR - 6));
    return(D26_ERROR);
  }
}
				    /*  check result (d26_attr) whether  */
				    /*  attributes have DN syntax or are */
				    /*  recurring attributes             */
for (i=0; d26_attr[i].d26_at_idx != D26_EOL ; i++)
{ 
  p_at_entry = &d26_at[d26_attr[i].d26_at_idx];
  if                              /*  attribute is real recurring      */
    (p_at_entry->d26a_filenr >= 0)
  {                               /*  delete real recurring attribute  */
     d26_851_del_rec(record, p_at_entry, ob_cl[0]);
  } else if                       /*  attribute has DN Syntax          */
    (p_at_entry->d26a_syntax == D2_DISTNAME)
  {                               /*  update DN list                   */
     d26_852_upd_dnlist(record, p_at_entry, ob_cl[0], p_oct_entry, &time_stamp);
  } 
} 


if                                  /*  no alias object                */
  (al_flag == FALSE)
{   
				    /*  update of acl list               */
				    /*  index in DN List                 */
  for (i = 0; i < D2_ACLIDX_MAX; i++)
  { 
    dnl_ptr[i] = &dns[i];
    dns[i].d26_dno = -1;
    dns[i].d26_didx = &acl[i];
  } 
				    /*  sort acl shorts               */
  d26_u24_sort_acl_idx(dnl_ptr, D2_ACLIDX_MAX, dnl_sort, &no_dnl_entries);
  for                               /*  all entries to be updated     */
     (i = 0; i < no_dnl_entries; i++)
  { 
    if (i+1 == no_dnl_entries)
    {   
      time_stamp = TRUE;
    } 
    if                              /* update of dnl is successful    */
      (d26_i02_upd_file_dnlist (D2_DEL_AV,(D2_name_string)NULL, D2_IGNORE, (D2_name_string)NULL,
		    dnl_sort[i]->d26_dno,FALSE,dnl_sort[i]->d26_didx))
    {   
    } 
  } 
} 
				    /* update time stamp of dnlist file */
if (time_stamp == TRUE)
{    /* update time stamp */
     if
	(d26_i02_upd_file_dnlist (D26_NOTHING, (D2_name_string) NULL,
	 D2_IGNORE , (D2_name_string) NULL, (signed32) NULL, TRUE, &dummy))
     {   
     } 
} 
				    /*  unlock used files             */
d26_i14_unlock();

p_off = *a_out;
*length = 0;

if                    /*  DSP was coming in                         */
    (d26_chain_arg != (D23_chain_arg *)NULL)
{                     /*  write chaining result                     */
  ((D23_chain_res *) p_off)->d23_chrtag  = D23_CHRTAG;
p_off += D23_S_CHRES;
*length += D23_S_CHRES;
}                     /*                                            */
			    /*  generate result message (OK)  */
((D23_eom *) p_off)->d23_eomtag = D23_EOMTAG;
*length += D23_S_EOM;

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
return(ret_value);

}                                   /*  d26_8_remove_object()         */

