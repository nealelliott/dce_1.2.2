/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24alist.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:27  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:11  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:24  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:17  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:41  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:40:28  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:41:00  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:32:37  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:37  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:54:57  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:48:26  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:54:43  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:33:44  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:04:51  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:30  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24alist.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:43 $";
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
/* NAME         : d24alist.c                                          */
/*								      */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  26.02.90                                           */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*                ( for list )                                        */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a100_apdu_list_arg_ids()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms list_arg's from APDU format to  */
/*  the remove arguments and common arguments of the ids interface.   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the list arguments.                    */
/*      apdu_len    = APDU length for the list arguments.             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      list_arg  = list arguments in IDS_interface.                */
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
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a100_apdu_list_arg_ids(
  byte * apdu,
  signed32 * apdu_len,
  ids_list_arg * list_arg,
  ids_common_arg * com_arg,
  byte * err_apdu )
{
byte * scan = apdu;
#ifndef DSA
signed32 version = ((D23_header *)apdu)->d23_versno;
#endif
const char function_name[] = "d2a100_apdu_list_arg_ids";

#ifndef DSA

    if                              /*  IPC version is not ok         */
      (!D2_CHECK_MINMAX(D23_APDUV22,D23_APDUV22,version))
    {                               /*  reset to 0 and return error   */
      d27_010_set_error_apdu((D23_rserror *)apdu,D2_CALL_ERR,D2_ILL_VERS,
			     D2_NOT_SPEC,apdu_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }                               /*                                */
    scan += sizeof(D23_inlist);
#endif

    d2a080_mk_ids_comarg(&scan,com_arg);

    if (d2a121_dn_ids(&scan, &list_arg->object, err_apdu,apdu_len)
				 == D2_ERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a103_ids_list_res_apdu()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms list_results from ids format to */
/*  the list results and common results of the APDU interface.        */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the list result .                      */
/*      apdu_len    = APDU length for the list result .               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      list_res  = list result in IDS_interface.                   */
/*      com_res   = Common result of IDS_interface.                 */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len     = length of error APDU                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a103_ids_list_res_apdu(
  byte ** cur_apdu,
  byte ** apdu,
  signed32 * apdu_len,
  ids_list_res * list_res,
  ids_common_res * com_res,
  byte * err_apdu )
{
signed32 i,max_length,len;
byte * scan;
D23_subord_info * sord_info;
ids_list_info * l_info;
ids_dist_name dn;
const char function_name[] = "d2a103_ids_list_res_apdu";

    if (list_res->uncorrelated == TRUE)
	{
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_PERM_ERR,
		D2_INT_ERR,D2_NOT_SPEC, apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
#ifndef DSA
    if ((*cur_apdu = *apdu = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM))==NULL)
	{
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
		D2_SYS_ERR,D21_ALLOC_ERROR, apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
#endif
    scan = *cur_apdu;
    max_length = (*apdu_len > D2A_APDU_MEM) ? *apdu_len : D2A_APDU_MEM;
    l_info = &(list_res->info.list_info);
    if (l_info->dname_defined == TRUE)
    {

      if (d2a122_dn_apdu( D23_BONTAG, &scan, &l_info->dname,
		apdu, &max_length, err_apdu,apdu_len) == D2_ERROR)
	{
	  free(*apdu);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      }
    if (l_info->part_oq_defined == TRUE)
      { len = D23_S_SUBORD+D23_S_POQ+D23_S_EOM+D23_S_CRES;}
     else
      { len = D23_S_SUBORD+D23_S_EOM+D23_S_CRES;}

    for (i=0; i < l_info->nb_subordinate; i++)
     {
      if (d2a033_check_memsize(len, apdu, &scan, &max_length,
			(D23_rserror *)err_apdu, apdu_len) != D2_NOERROR)
	{
            free(*apdu);
            d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
                        D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
            D2_ASN1_ERR_RETURN_WITH_TRACE
                }


       sord_info = (D23_subord_info *) scan;
       scan += sizeof(D23_subord_info);
       sord_info->d23_somas = (l_info->subordinates + i)->from_entry;
       sord_info->d23_soali = (l_info->subordinates + i)->alias_entry;
       sord_info->d23_sotag = D23_SOITAG;
       dn.nb_rdnames = 1;
       dn.rdnames = &(l_info->subordinates + i)->rdname;
       if (d2a122_dn_apdu( D23_NAMTAG, &scan, &dn, apdu, &max_length,
			 err_apdu, apdu_len) == D2_ERROR)
	{
	  free(*apdu);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
       }
     if ((l_info->part_oq_defined == TRUE)
			&&(l_info->part_oq.unexplored_defined == TRUE))
     {
      len = scan - *apdu;
        if(d2a033_check_memsize(sizeof(D23_part_oq),apdu,&scan,
              &max_length,(D23_rserror *)err_apdu,apdu_len) != D2_NOERROR)
	    {
            free(*apdu);
            d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
                        D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
            D2_ASN1_ERR_RETURN_WITH_TRACE
        	}

      ((D23_part_oq *)scan)->d23_oqtag = D23_OQTAG;
      ((D23_part_oq *)scan)->d23_oq_uncrit = l_info->part_oq.un_crit_ext;
      ((D23_part_oq *)scan)->d23_oq_limit =
		  ( l_info->part_oq.limit_pb_defined == TRUE) ?
		   l_info->part_oq.limit_pb: D2_NO_LIMIT_EX;
      scan += D23_S_POQ;
      for (i=0; i < l_info->part_oq.nb_unexplored; i++)
      d2a053_put_ref((l_info->part_oq.unexplored+i),
			 &scan, apdu, &max_length, err_apdu, apdu_len);
      }

      /* check memory for common result and EOM 			*/
      if(d2a033_check_memsize(sizeof(D23_com_res) + sizeof(D23_eom),apdu,&scan,
         &max_length,(D23_rserror *)err_apdu,apdu_len) != D2_NOERROR)
      {
	  free(*apdu);
	  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
		      D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
      }

     d2a130_mk_apdu_comres(com_res, &scan);
     ((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
     ((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);

     if (l_info->part_oq_defined == TRUE)
      ((D23_eom *) scan)->d23_eom_oqoff  = *apdu_len - len;
     else
       ((D23_eom *) scan)->d23_eom_oqoff = 0;

     *cur_apdu = scan + sizeof(D23_eom);
     *apdu_len = *cur_apdu - *apdu;

return(D2_NOERROR);
}
