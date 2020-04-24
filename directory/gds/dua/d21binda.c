/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21binda.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:18  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:53:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:46  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:46:38  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:38:44  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  15:53:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:25  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:31:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:07:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:52:46  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:46:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:37  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:09:49  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:35:59  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:19:50  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:48  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21binda.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:45 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*
* daton **************************************************************
*                                                                    *
* TYPE         :  MODUL                                              *
*                                                                    *
* NAME         :  d21binda.c                                         *
*                                                                    *
* AUTHOR       :  J.Heigert                                          *
* DATE         :  14.01.87                                           *
*                                                                    *
* COMPONENT    :  Directory Service-V2 / xxxx                        *
*                                                                    *
* PRD#/VERS.   :                                                     *
*                                                                    *
* DESCRIPTION  :  Auxilliary functions for BIND                      *
*                                                                    *
* HISTORY      :                                                     *
*                                                                    *
* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   *
* datoff *************************************************************
*/

/*
**********************************************************************
*                                                                    *
*       I N C L U D E S                                              *
*                                                                    *
**********************************************************************
*/

#include <gds.h>
#include	<stdio.h>
#include 	<stdlib.h>

#include        <d21dua.h>

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_b15_read_named_dsa()                           *
*                                                                    *
* AUTHOR       :  J.Heigert,     Softlab GmbH.                       *
* DATE         :  28.01.88                                           *
*                                                                    *
* SYNTAX       :  Short d21_b15_read_named_dsa(parblk)               *
*                   D21_p1 *parblk;                                  *
*                                                                    *
* DESCRIPTION  :  Read PSAP-address of given DSA from cache          *
*                                                                    *
* exoff **************************************************************
*/

d2_ret_val d21_b15_read_named_dsa(
  D21_p1 * parblk)
{
D2_a_info               *at_info;

d2_ret_val ret_value;

/* set addr_dsa to NULL: indicator if there will be allocated memory later */

parblk->d21_1addr_dsa = NULL;

/*
read from dua_cache; use "LOCAL-DSA" short form !
*/
ret_value =  d21_b_read_cache(parblk->d21_1pbhead,(D2_name_string)D2_N_LOC_DSA, 
                            parblk->d21_1dir_id, &at_info);

if (ret_value != D2_NOERROR && parblk->d21_1pbhead->d2_errvalue != D2_NFOUND)
  {
    return(D2_ERROR);
   }

if (ret_value == D2_NOERROR && 
      d21_c82_dsname_cmp(at_info->d2_a_val->d2_at_value.d2_obj_name_a_v,
			    parblk->d21_1name_dsa) == TRUE)
     {       /* dsa is a local dsa */
	     /* no further access to cache !" */
       free(at_info);
       return(D2_NOERROR);
      }

if (ret_value == D2_NOERROR)
	free(at_info);

/*
no entry for local-dsa found  or  local-dsa-name != name of dsa in parblk ->
read psap-address
*/

if (d21_b18_read_psap(parblk->d21_1name_dsa,parblk->d21_1dir_id,  
                    &(parblk->d21_1addr_dsa), parblk->d21_1pbhead) == D2_ERROR)
     {
       return(D2_ERROR);
      }

/*
set return_values
*/

parblk->d21_1pbhead->d2_retcode = D2_NO_ERR;

return(D2_NOERROR);
}	/* d21_b15_read_named_dsa() */


/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_b17_read_def_dsa()                             *
*                                                                    *
* AUTHOR       :  J.Forster,     Softlab GmbH.                       *
* DATE         :  09.08.88                                           *
*                                                                    *
* SYNTAX       :  Short d21_b17_read_def_dsa(def_dsa_names,parblk)   *
*                   D2_a_info   **def_dsa_names                      *
*                   D21_p1       *parblk                             *
*                                                                    *
* DESCRIPTION  :  Read Default DSAs from DUA-Cache                   *
*                  (may be recurring !)                              *
*                                                                    *
* INPUT-PARAMETER :                                                  *
*                                                                    *
* OUTPUT-PARAMETER :                                                 *
*                                                                    *
* RETURN-VALUE :  0: Ausfuehrung erfolgreich                         *
*                -1: Fehler: --> Fehlercode ueberpruefen             *
*                                                                    *
* STANDARDHEAD :                                                     *
*                                                                    *
* USED FUNCTIONS/PROGRAMS/MACROS :                                   *
*                                                                    *
* COMMENTS :                                                         *
*                                                                    *
* exoff **************************************************************
*/

d2_ret_val d21_b17_read_def_dsa(
  D2_a_info ** def_dsa_names,
  D21_p1 * parblk)
{

/*
no names of dsas read for given directory id
*/
/*
  make a Cache read for default DSAs
*/
if (d21_b_read_cache(parblk->d21_1pbhead,(D2_name_string)D2_N_DEF_DSA,
			parblk->d21_1dir_id,def_dsa_names) != D2_NOERROR)
  {
    return(D2_ERROR);
   }

return(D2_NOERROR);

} /* d21_b17_read_def_dsa */


