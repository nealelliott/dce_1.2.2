/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25amod.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:40  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:24  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:17  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:26  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:09  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:42:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:56:11  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:35:52  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:33  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:58:40  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  13:03:35  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:06:42  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:49  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25amod.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:01 $";
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
/* NAME         : d25amod.c                                           */
/*								      */
/* AUTHOR       :  Praveen Gupta (Siemens India)                      */
/* DATE         :  01.03.90                                           */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*                ( for modify entry )                                */
/*								      */
/* SYSTEM DEPENDENCIES:  SSTUB                                        */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-03-01| Original                       | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static signed32 d2a109_mk_mod_apdu (ids_att *,signed32,byte **,byte **,signed32 *,
							byte *,signed32 *);

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a105_ids_modify_arg_apdu()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms modify_arg's from ids format to */
/*           the modify arguments and common arguments of the APDU    */
/*           interface.                                               */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      modify_arg  = modify arguments in IDS_interface.            */
/*      com_arg   = Common arguments of IDS_interface.              */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = APDU for the modify arguments.                  */
/*      apdu_len    = APDU length for the modify arguments.           */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len    = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a105_ids_modify_arg_apdu(
  byte            **apdu,
  byte            **beg_apdu,
  signed32              *apdu_len,
  ids_modify_arg   *modify_arg,
  ids_common_arg   *com_arg,
  byte            *err_apdu )
{
byte            *act_apdu = *apdu;
ids_entry_modif  *mod;
ids_entry_modif  *last_mod;
ids_att          ids_attr;
char             err_comm[100];
const char function_name[] = "d2a105_ids_modify_arg_apdu";

if (d2a123_mk_apdu_cmarg(&act_apdu,com_arg, beg_apdu, apdu_len,
	    err_apdu,apdu_len) == D2_ERROR )
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
/*
   generate DN from DUA dist. name
*/
if (d2a122_dn_apdu(D23_NAMTAG, &act_apdu, &modify_arg->object,
	     beg_apdu, apdu_len, err_apdu,apdu_len) == D2_ERROR)
{
    D2_ASN1_ERR_RETURN_WITH_TRACE
}

/*
   generate APDU from DUA attributes to be modified
*/
for (mod = modify_arg->changes, last_mod = mod + modify_arg->nb_change;
     mod < last_mod; mod++)
{   switch (mod->modif_kind)
    {   case ENTRYMOD_ADD_ATT:
		if (d2a109_mk_mod_apdu (&mod->modif.add_att,
					mod->modif_kind, &act_apdu, beg_apdu,
					apdu_len, err_apdu, apdu_len)
					!= D2_NOERROR)
		   {
		   D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		break;

	case ENTRYMOD_ADD_VAL:
		if (d2a109_mk_mod_apdu (&mod->modif.add_val,
					mod->modif_kind, &act_apdu, beg_apdu,
					apdu_len, err_apdu, apdu_len)
					!= D2_NOERROR)
		    {
		   D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		break;

	case ENTRYMOD_REMOV_ATT:
		ids_attr.type.l = mod->modif.rm_att.l;
		ids_attr.type.v = mod->modif.rm_att.v;
		ids_attr.nb_values = 0;
		if (d2a109_mk_mod_apdu (&ids_attr, mod->modif_kind,
					&act_apdu, beg_apdu, apdu_len, err_apdu,
					apdu_len) != D2_NOERROR)
		    {
		   D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		break;

	case ENTRYMOD_REMOV_VAL:
		if (d2a109_mk_mod_apdu (&mod->modif.rm_val,
					mod->modif_kind, &act_apdu, beg_apdu,
					apdu_len, err_apdu, apdu_len)
					!= D2_NOERROR)
		    {
		   D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		break;

	default:
		sprintf(err_comm, "wrong modification: %d", mod->modif_kind);
		D2_ASN1_ERR_RETURN_WITH_TRACE
    }
}

((D23_eom *) act_apdu)->d23_eomtag = D23_EOMTAG;
*apdu = (act_apdu + sizeof(D23_eom));

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a109_mk_mod_apdu()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function transforms one attribute being modifed from     */
/*      IDS format into APDU format.                                  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      ids_attr      = attribute information  in THORN-IDS format    */
/*                      (ids_att structure)                           */
/*      modification  = modification type                             */
/*      act_apdu      = pointer to the APDU where to write the        */
/*                      attribute info (D23_md_info)                  */
/*      beg_apdu      = start address of APDU                         */
/*      max_apdu_len  = max. length of allocated APDU memory          */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      act_apdu    = pointer to next tag in the APDU where to write  */
/*                    the next block                                  */
/*      beg_apdu    = (new) start address of APDU 		      */
/*      max_apdu_len= (new) max. length of allocated APDU memory      */
/*      err_apdu    = error APDU (memory error)                       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static signed32 d2a109_mk_mod_apdu (
  ids_att     *ids_attr,
  signed32          modification,
  byte      **act_apdu,
  byte      **beg_apdu,
  signed32         *max_apdu_len,
  byte       *err_apdu,
  signed32         *err_len )
{
signed32              len = sizeof(D23_md_info)+D23_S_ATT;
ids_att_value   *value;
ids_att_value   *last_value;
D23_md_info     *md_info;
D23_av_info     *av_info;
D23_av_value    *av_value;
signed32              syntax;
const char function_name[] = "d2a109_mk_mod_apdu";

if (d2a033_check_memsize(len,beg_apdu,act_apdu,max_apdu_len,
			(D23_rserror *) err_apdu,err_len) != D2_NOERROR) 
  {
  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
                          D2_SYS_ERR, D21_ALLOC_ERROR,
                          err_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
  
/*
   set D23_md_info and D23_ma_info structure
*/
md_info = (D23_md_info *) *act_apdu;
md_info->d23_mdtag = D23_MODTAG;
*act_apdu += sizeof(D23_md_info);
if (d2a124_copy_attyp_apdu(act_apdu, &ids_attr->type, beg_apdu,
	      max_apdu_len, err_apdu, err_len) == D2_ERROR)
    { 
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

md_info->d23_mdmod = modification;
if (md_info->d23_mdmod == D2_DELATT)
  {
   return(D2_NOERROR);
  }

len = D23_S_AV + ids_attr->nb_values * D23_S_ATV;

if (d2a033_check_memsize(len,beg_apdu,act_apdu,max_apdu_len,
			(D23_rserror *) err_apdu,err_len) != D2_NOERROR) 
  {
  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
                          D2_SYS_ERR, D21_ALLOC_ERROR,
                          err_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
  


av_info = (D23_av_info *) *act_apdu;
*act_apdu += sizeof(D23_av_info);
av_info->d23_avtag = D23_AVTAG;
av_info->d23_asyntx = syntax = recover_att_syntax(&ids_attr->type);
if (syntax == D2_ERROR) av_info->d23_asyntx = D2_ASN1;
av_info->d23_avnum = ids_attr->nb_values;
av_value = (D23_av_value *) *act_apdu;
*act_apdu += sizeof(D23_av_value);

for (value = ids_attr->values, last_value = value + ids_attr->nb_values;
     value < last_value; 
     value++, av_value = (D23_av_value *) *act_apdu, *act_apdu += sizeof(D23_av_value))
{
    /*
       copy attribute value in APDU
    */
    if (d2a106_ids_atval_to_atval( value, syntax, &av_value->d23_avlen,
				  &av_value->d23_avoff,
				  &av_value->d23_avrep,act_apdu, beg_apdu,
				  max_apdu_len, err_apdu, err_len)
				  == D2_ERROR)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    av_value->d23_avfil = 0;
}
*act_apdu -= sizeof(D23_av_value);  /* to undo last increment in for loop */

return(D2_NOERROR);
}
