/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21chdnl.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:13  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:49  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:41  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:23:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:34:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:21:25  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:48:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:24  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:19:48  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:33:55  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:47:36  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:12:40  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21chdnl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:50 $";
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
/*                                                                    */
/* NAME         : d21chdnl.o                                          */
/*                                                                    */
/* AUTHOR       : Serra & Pulpillo, D525 (CDS)                        */
/* DATE         : 11.12.89                                            */
/*                                                                    */
/* KOMPONENTE   : DS                                                  */
/*                                                                    */
/* DOK.-NR.     : DS-Design-Spez.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 11.12.89| Erstellung                     | hv |         */
/*                                                                    */
/*datoff **************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                     Modulidentification                            */
/*                                                                    */
/**********************************************************************/
 
 
/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/
 
#include <gds.h>
#include <sys/types.h>
#include <malloc.h>
 
#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d21iapl.h>
#include <d21.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d2adm.h>
#include <d20proto.h>
 
/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/
 
#define  SEARCHSIZ  32768
#define CONVERR(value) (value % 16 + '0') /* Conversion short -> char */
 
/**********************************************************************/
/*                                                                    */
/*                 D E K L A R A T I O N E N      DATEN               */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                D E K L A R A T I O N E N      FUNCTIONEN           */
/*                                                                    */
/**********************************************************************/
 
 
/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21t065_change_dnlist()                                */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will modify the distinguished name list of      */
/*      all DSAs objects existent in the bind DSA, changing the       */
/*      matching entries of the changed objects                       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      char    *old_dn;     old string that have to be changed       */
/*      char    *new_dn;     new string value for D2_DNLIST attribute */
/*      signed16 bind_id;    bind identification                      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *return_code;       return code of operation      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/
 
 d2_ret_val d21t065_change_dnlist(
   char     *old_dn,
   char     *new_dn,
   signed16  bind_id,
   Errcode  *return_code)
 
{
   d2_ret_val  ret_val = D2_NOERROR;


   /* call "change_dnlist" function to change the entries in all DSA objects */
   /* presents in the bind DSA (search is done with LOCALSCOPE param. set)   */
ret_val = d21t066_chdnl(old_dn, new_dn, bind_id, return_code);

return(ret_val);
}


 d2_ret_val d21t066_chdnl(
   char     *old_dn,
   char     *new_dn,
   signed16  bind_id,
   Errcode  *return_code)
{
   D21_p5       pb;                      /* parameter block for SEARCH*/
   D21_pC       pbmod;                   /* parameter block for MODIFY*/
   d2_ret_val   ret_value;               /* return value of function  */
   signed32     cc;
   Bool         change;
   char         n_name[D2_DNL_MAX + 1];  /* New name of the object    */
   D2_ei_info  *dsa_obj;                 /* Ptr to the object block   */
   D2_a_info   *ptr_attr;                /* Ptr to the attribute block*/
   D2_a_value  *ptr_val;                 /* Ptr to the values block   */
   signed16     num_attr;                /* Number of attribute values*/
   T61_string   object;                  /* Attribute value           */
   D2_a_modinf  modinf;                  /* info for MODIFY function  */
   D2_a_value   oldnewval[2];
 
   static   D2_c_arg     d2_c_arg; 
 
   static   D2_a_type      rinf={sizeof(D2_DNLIST)-1,
				 (Ob_id_string) D2_DNLIST};
   static   D2_Fi_filter   filter;    /* filter information area   */
   static   D2_req_info    reqinfo = {D2_R_VAL_TYP, 1, &rinf};
   static   D2_pbhead      head = {D21V022, D2_NO_ERR, D2_NO_ERR, D2_NO_ERR};
   D2_c_res c_res;
   char         norm_old_dn[D2_DNL_MAX + 1]; /* normalized old string */
   char         norm_object[D2_DNL_MAX + 1]; /* normalized object name */
 
   ret_value = D2_NOERROR;
 
 
/**********************************************************************/
/* initialize param. block for search DSA objects that have to change */
/**********************************************************************/
 
   strcpy(norm_old_dn, old_dn);
   d20_norm(norm_old_dn, strlen(norm_old_dn));

   pb.d21_5pbhead = &head;
   pb.d21_5c_a = (D2_c_arg *)&d2_c_arg;
   d2_c_arg.d2_serv_cntrls = D2_LOCALSCOPE | D2_NOREFERRAL | D2_USEDSA | 
			     D2_DONT_STORE | D2_PREF_ADM_FUN;
   d2_c_arg.d2_priority    = D2_PRIO_LOW;
   d2_c_arg.d2_time_limit  = D2_T_UNLIMITED;
   d2_c_arg.d2_size_limit  = D2_S_UNLIMITED;
   pb.d21_5bind_id = bind_id; 
   pb.d21_5base_obj = (D2_name_string)NULL; /* all objects under root */
   pb.d21_5subset = D2_WHOLE_SUBTREE;
   pb.d21_5srch_al = FALSE;

     /* filter for the objects that matches whith "old_dn"   */
     /********************************************************/
     filter.d2_fi_opmod = D2_ITEM;
     filter.d2_fi_v.d2_fi_item.d2_fi_type.d2_typ_len = sizeof(D2_DNLIST)-1;
     filter.d2_fi_v.d2_fi_item.d2_fi_type.d2_type
      = (Ob_id_string) (D2_DNLIST);
     filter.d2_fi_v.d2_fi_item.d2_fi_match = D2_SUBSTRINGS;
     filter.d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_pos
      = D2_INITIAL;
     filter.d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_str_val.
     d2_a_v_len = strlen(old_dn) + 1;
     filter.d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_str_val.
     d2_a_rep = (unsigned16) d27_401_syntax_to_rep((signed16)D2_DISTNAME );
     filter.d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_str_val.
     d2_at_value.d2_obj_name_a_v = (D2_name_string) old_dn;
     filter.d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_next
      = NULL;

   pb.d21_5f_pattern = &filter;

   pb.d21_5r_info = &reqinfo;
   pb.d21_5file = NULL;
   pb.d21_5err = (D2_error *)NULL;
   pb.d21_5cres = &c_res ; 
 
/**********************************************************************/
/* Parameter block for modify the DNLIST of the DSA objects required  */
/**********************************************************************/
 
   pbmod.d21_Cpbhead = &head;
   pbmod.d21_Cc_a = (D2_c_arg *)&d2_c_arg;

   d2_c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
   d2_c_arg.d2_al_rdn = 0;

   pbmod.d21_Cmod = &modinf;
      modinf.d2_m_next = (D2_a_modinf *)NULL;
      modinf.d2_m_mod = D2_REP_AV;
      modinf.d2_m_attr.d2_a_next = (D2_a_info *)NULL;
      modinf.d2_m_attr.d2_a_type.d2_typ_len = sizeof(D2_DNLIST)-1;
      modinf.d2_m_attr.d2_a_type.d2_type    = (Ob_id_string) D2_DNLIST;
      modinf.d2_m_attr.d2_a_val = &oldnewval[0];
      modinf.d2_m_attr.d2_a_no_val = 2;
   pbmod.d21_Cbind_id = bind_id;
 
   /************************************************/
   /*            Search of the DSA objects         */
   /************************************************/
 
   ret_value=d21_5_search(&pb);
 
   if (ret_value == D2_NOERROR) {
      d2_c_arg.d2_serv_cntrls |= D2_NOCACHE; /* MODIFY ATTR. in NOCACHE mode*/
      d2_c_arg.d2_serv_cntrls &= ~D2_LOCALSCOPE;
      dsa_obj=pb.d21_5res;             /* Ptr to the object structure */
 
      /****************************************************************/
      /*        Handling of the linked list of the found objects      */
      /****************************************************************/
 
      while (dsa_obj!=NULL && ret_value==D2_NOERROR) {
         pbmod.d21_Centry = dsa_obj->d2_ei_entry;
         ptr_attr=dsa_obj->d2_ei_ai;
                                   /* Ptr to the Attributes structure */
         while(ptr_attr!=NULL && ret_value==D2_NOERROR) {
	    if (D2_ATTCMP(&(ptr_attr->d2_a_type),D2_DNLIST)==0) {
               num_attr=ptr_attr->d2_a_no_val;
               ptr_val=ptr_attr->d2_a_val;
                                   /* Array of attribute values       */
               for(cc=0; cc < num_attr; cc++,ptr_val++) {
                  object=ptr_val->d2_at_value.d2_t61_a_v;

		   strcpy(norm_object, (char *)object);
		   d20_norm(norm_object, strlen(norm_object));

                        /* Looking if object name has been changed    */
      		  if (d21t042_filter_obj(norm_object, norm_old_dn) == 
								D2_REAL_OBJ) 
                  	change=TRUE;
		  else
                        change=FALSE;

                        /* The object name has been changed           */
                  if (change) {
                     d21t045_build_dn(old_dn,new_dn,(char *)object,n_name);
		     oldnewval[0].d2_a_v_len = strlen((char *)object) + 1;
		     oldnewval[0].d2_a_rep = (unsigned16) d27_401_syntax_to_rep(
							(signed16)D2_DISTNAME);
		     oldnewval[0].d2_at_value.d2_obj_name_a_v = object;
		     oldnewval[1].d2_a_v_len = strlen(n_name) + 1;
		     oldnewval[1].d2_a_rep = (unsigned16) d27_401_syntax_to_rep(
							(signed16)D2_DISTNAME);
		     oldnewval[1].d2_at_value.d2_obj_name_a_v = 
							(D2_name_string)n_name;
                     if (d21_C_modify_attribute(&pbmod) == D2_ERROR) {
                        return_code->retcode =CONVERR(head.d2_retcode);
                        return_code->errclass=CONVERR(head.d2_errclass);
                        return_code->errvalue = head.d2_errvalue;
                        ret_value = D2_ERROR;
                        break;
                     }
                  }
               }
            }
            ptr_attr=ptr_attr->d2_a_next; /* Next attribute structure */
         }
         dsa_obj=dsa_obj->d2_ei_next;     /* Next object   structure  */
      }
   }
   else {
      return_code->retcode  = CONVERR(head.d2_retcode);
      return_code->errclass = CONVERR(head.d2_errclass);
      return_code->errvalue = head.d2_errvalue;
      ret_value = D2_ERROR;
   }

   if (pb.d21_5res != (D2_ei_info *) NULL)
   	free((char *) pb.d21_5res);

   return(ret_value);
}
/**********************************************************************/
