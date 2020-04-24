/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24amod.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:34  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:28  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:12  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:28  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:18  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:44  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:40:37  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:41:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:32:50  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:41  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:55:11  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:48:38  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:04:58  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:37  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24amod.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:44 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: C-FILE					      */
/*								      */
/* NAME         : d24amod.c                                            */
/*								      */
/* AUTHOR       :  Helmut Volpers                                     */
/* DATE         :  01.03.90                                           */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.           */
/*                ( for modify entry )                                */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-03-01| Original                       | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a104_apdu_modify_arg_ids()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms modify_arg's from APDU format to*/
/*           the modify arguments and common arguments of the THORN   */
/*           ids interface.                                           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the modify arguments.                  */
/*      apdu_len    = APDU length for the modify arguments.           */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      modify_arg  = modify arguments in IDS_interface.            */
/*      com_arg   = Common arguments of IDS_interface.              */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len     = length of error APDU                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Helmut Volpers                                            */
/**********************************************************************/

signed32 d2a104_apdu_modify_arg_ids(
  byte * apdu,
  signed32 * apdu_len,
  ids_modify_arg * modify_arg,
  ids_common_arg * com_arg,
  byte * err_apdu )
{
signed32      i,j;                           /* counter */
byte             *ptr_msg = apdu;       /* pointer to message */
byte    *sav_ptr;              /* pointer to message */
ids_entry_modif *act_modif;
signed32      modification;
signed32               rv;
ids_att           *ids_att1;
ids_att           *ids_att2;
D23_av_info     * av_info;
ids_att_value   * ids_valptr;
ids_att_value   * ids_valptr1;
ids_att_type    * ids_type;
dua_att_value     valptr;
D23_av_value    * av_value;
signed32               syntax;
#ifndef DSA
signed32 version = ((D23_header *)apdu)->d23_versno;
#endif
const char function_name[] = "d2a104_apdu_modify_arg_ids";

#ifndef DSA
    if                              /*  IPC version is not ok         */
      (!D2_CHECK_MINMAX(D23_APDUV22,D23_APDUV22,version))
    {                               /*  reset to 0 and return error   */
      d27_010_set_error_apdu((D23_rserror *)apdu,D2_CALL_ERR,D2_ILL_VERS,
			     D2_NOT_SPEC,apdu_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }                               /*                                */
    ptr_msg += D23_S_INMOD; /* pointer to message */
#endif

   /************************/
   /* initialize variables */
   /************************/
   modify_arg->nb_change = 0;

   d2a080_mk_ids_comarg(&ptr_msg, com_arg);

   /******************/
   /* find entryname */
   /******************/

   /* generate DUA-DN from APDU */
   if (d2a121_dn_ids(&ptr_msg,
	       &modify_arg->object, err_apdu, apdu_len) == D2_ERROR)
   {
      D2_ASN1_ERR_RETURN_WITH_TRACE
   }

   sav_ptr = ptr_msg;
   /*******************************************/
   /* count number of attribute modifications */
   /*******************************************/
   for (i = 0;
	((D23_md_info *)ptr_msg)->d23_mdtag == D23_MODTAG; i++) {
	if (((D23_md_info *)ptr_msg)->d23_mdmod == D2_REP_AV)
	    i++;
	/*
	   D2_REP_AV will be built on D2_DEL_AV and D2_ADD_AV
	*/

        modification = ((D23_md_info *)ptr_msg)->d23_mdmod;

	/* next tag */
	ptr_msg += D23_S_MOD ;
	/* scan att-type */
	ptr_msg += ((D23_at_type *)ptr_msg)->d23_atoff  + D23_S_ATT;
        if (modification != D2_DELATT)
	 for (j= ((D23_av_info *)ptr_msg)->d23_avnum, ptr_msg += D23_S_AV;
	      j > 0; j--)
	     ptr_msg += ((D23_av_value *)ptr_msg)->d23_avoff + D23_S_ATV;
   } /* for (all attribute modifications) */

   /* set number of attribute modifications */
   modify_arg->nb_change = i;

   /***************************/
   /* allocate memory         */
   /***************************/
      modify_arg->changes = act_modif = D27_CALLOC (svc_c_sev_warning,
				i+1, sizeof (ids_entry_modif));
      if (modify_arg->changes == (ids_entry_modif *) NULL) {
	 d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
				 D2_SYS_ERR, D21_ALLOC_ERROR,apdu_len);
	 D2_ASN1_ERR_RETURN_WITH_TRACE
      }

   /***********************************/
   /* convert attribute modifications */
   /***********************************/
   for (ptr_msg = sav_ptr, i=0;
	i < modify_arg->nb_change; i++, act_modif++) {
	modification = (((D23_md_info *)ptr_msg)->d23_mdmod);
	/* next tag */
	ptr_msg += D23_S_MOD;

	switch (modification) {
			   /***************************/
	   case D2_REP_AV: /* replace attribute value */
			   /***************************/
	    i += 1;
	    act_modif->modif_kind = ENTRYMOD_REMOV_VAL;
	    ids_att2 = &act_modif->modif.rm_val;
	    act_modif++;
	    act_modif->modif_kind = ENTRYMOD_ADD_VAL;
	    ids_att1 = &act_modif->modif.add_val;
	    sav_ptr = ptr_msg;
	    if (d2a120_copy_att_type(&ptr_msg, &ids_att2->type,
		  err_apdu, apdu_len) == D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	    ptr_msg = sav_ptr ;
	    if (d2a120_copy_att_type(&ptr_msg, &ids_att1->type,
		  err_apdu, apdu_len) == D2_ERROR) 
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}

	    av_info = (D23_av_info *)ptr_msg;
	    ptr_msg += D23_S_AV;
	    ids_att2->nb_values = ids_att1->nb_values = j
			       = av_info->d23_avnum/2 ;

	    if ((ids_valptr = ids_att2->values = D27_MALLOC
	      (svc_c_sev_warning,av_info->d23_avnum/2 * sizeof(ids_att_value)))
		 == NULL)
	    {   
		d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			   D2_SYS_ERR, D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
	      }
	    if ((ids_valptr1 = ids_att1->values = D27_MALLOC
	       (svc_c_sev_warning,av_info->d23_avnum/2 * sizeof(ids_att_value)))
		 == NULL)
	    {   
		d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			   D2_SYS_ERR, D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
	      }

	    /*
	       get attribute syntax
	    */
	    syntax = av_info->d23_asyntx;

	    for (av_value = (D23_av_value *)ptr_msg;
		 j >0; av_value = (D23_av_value *)ptr_msg,j--,
		 ids_valptr++, ids_valptr1++ )
	     {   /*
		  copy attribute value from APDU  to DEL_VAL
		 */
		 ptr_msg += D23_S_ATV;
		 if (d2a031_cp_attr(av_value->d23_avrep, av_value->d23_avlen,
			ptr_msg, &valptr, err_apdu, apdu_len) != D2_NOERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		    }
		 if (dua_att_value_ids(&valptr, ids_valptr) == D2_ERROR)
		  {
		    D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		 if (syntax != D2_DISTNAME)
		 	dua_att_value_free(&valptr);
		 ptr_msg += av_value->d23_avoff;
		 /*
		  copy attribute value from APDU to ADD_VAL
		 */
		 av_value = (D23_av_value *)ptr_msg;
		 ptr_msg += D23_S_ATV;
		 if (d2a031_cp_attr(av_value->d23_avrep, av_value->d23_avlen,
			ptr_msg, &valptr, err_apdu, apdu_len) != D2_NOERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		    }
		 if (dua_att_value_ids(&valptr, ids_valptr1) == D2_ERROR)
		  {
		    D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		 if (syntax != D2_DISTNAME)
		 	dua_att_value_free(&valptr);
		 ptr_msg += av_value->d23_avoff;
	       }
	       break;

			   /**************************/
	   case D2_ADDATT: /* add attribute          */
	   case D2_ADD_AV: /* add attribute value    */
	   case D2_DEL_AV: /* delete attribute value */
			   /**************************/
		act_modif->modif_kind = modification;
		ids_att2 = &(act_modif->modif.add_att);
/*
		ids_att2 = (modification == D2_ADD_AV)?
                    &(act_modif->modif.add_val):&(act_modif->modif.add_att);
		ids_att2 = (modification == D2_DEL_AV)?
                    &(act_modif->modif.rm_val):&(act_modif->modif.add_att);
*/
		 rv = d2a062_mk_apdu_att ( &ptr_msg,
			      ids_att2, &syntax, err_apdu, apdu_len);
		 if (rv == D2_ERROR) 
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		 break;

			   /********************/
	   case D2_DELATT: /* delete attribute */
			   /********************/
		act_modif->modif_kind = ENTRYMOD_REMOV_ATT;
		ids_type = &act_modif->modif.rm_att;

		/* allocate memory for attribute type */
		if (d2a120_copy_att_type(&ptr_msg, ids_type, err_apdu,
		      apdu_len) == D2_ERROR)
		  {
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }

		break;

		    /*********/
	   default: /* error */
		    /*********/
		d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
					D2_PAR_ERR, D2_CONSTRAINT_VIOLATION ,
					apdu_len);
		D2_ASN1_ERR_RETURN_WITH_TRACE
	} /* switch (modification) */
   } /* for (all attribute modifications) */

return (D2_NOERROR);
} /* End: d2a104_apdu_modify_arg_ids */


