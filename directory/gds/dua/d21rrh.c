/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21rrh.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:24  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:33  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:54:09  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:51  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:43:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:35:38  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:35:42  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:59:17  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:48:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:08  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:36:38  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:42:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:33:42  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:20  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21rrh.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:34 $";
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
* NAME         :  d21rrh.c                                           *
*                                                                    *
* AUTHOR       :  J.Forster                                          *
* DATE         :   18.3.88                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include	<stdio.h>
#include	<string.h>

#include <d21dua.h>

/***********************************************/

d2_ret_val d21_rr1_check_params(
  D21_pB * parblk)
{
signed16           errval = D2_NO_ERR;
const char function_name[] = "d21_rr1";

do
{   /*
    check version
    */
    if (!D2_CHECK_MINMAX(D21MIN1,D21MAX1,parblk->d21_Bpbhead->d2_version))
    {                       /* illegal version supplied             */
	errval = D2_ILL_VERS;
	break;
    }

    /*
    check service controls
    */
    d21_check_comarg(parblk->d21_Bc_a, &errval, function_name,
		     parblk->d21_Bbind_id);
    if (errval != D2_NO_ERR)
    {   /* error in service controls */
	break;
    }
    else
    {   /*
	Check if Service-Controls correct
	*/
	errval = d21_ca4_check_sc(parblk->d21_Bc_a);


	if (errval != D2_NO_ERR)
	{   /* error in service controls */
	    break;
	}
    }

    if ((parblk->d21_Bc_a->d2_serv_cntrls & D2_DUACACHE) ||
    	 (parblk->d21_Bc_a->d2_serv_cntrls & D2_DUAFIRST) )
    {   errval = D21_SERV_CTRLS_INVALID;
	break;
    }

    /*
       check entry name
    */
    if (parblk->d21_Bentry != NULL &&
	strlen((char *)parblk->d21_Bentry) > D2_DNL_MAX)
    {   errval = D21_BAD_NAME;
	break;
    }
    if (parblk->d21_Bentry == NULL)
    {   errval = D21_DN_MISSING;
	break;
    }
    if (parblk->d21_Bnew_rdn == NULL)
    {   errval = D21_RDN_MISSING;
	break;
    }
}
while (FALSE); /* end of do-while loop */

if (errval != D2_NO_ERR)
{   if (errval != D2_ILL_VERS)
    {   parblk->d21_Bpbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_Bpbhead->d2_errclass = D2_PAR_ERR;
	parblk->d21_Bpbhead->d2_errvalue = errval;
    }
    else
    {   parblk->d21_Bpbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_Bpbhead->d2_errclass = D2_ILL_VERS;
	parblk->d21_Bpbhead->d2_errvalue = D2_NOT_SPEC;
    }
    return (D2_ERROR);
}
else
{   return(D2_NO_ERR);
}

}


void d21_rr3_build_req(
  D21_pB * parblk,
  char * req,
  d21_bind_info * bt_ptr)
{
D23_in_replrdn  *mp_head;
D23_repinfo     *rep_info;

/*
build first part of request
*/
mp_head = (D23_in_replrdn *) req;
d21_cm02_build_hdr( &mp_head->d23_Ohdr, bt_ptr);
req += D23_S_INMOD;
d21_cm03_build_comarg( parblk->d21_Bc_a, &req);

rep_info = (D23_repinfo *) req;
rep_info->d23_reptag = D23_REPTAG;
rep_info->d23_delordn = parblk->d21_Bdel_old_rdn;

req += D23_S_REP;
/*
move Distinguished name of object
*/
d21_cab_dsname_iput(parblk->d21_Bentry, req, &req);

/*
move new RDN
*/
d21_cab_dsname_iput(parblk->d21_Bnew_rdn, req, &req);

/*
move eom-tag
*/
d21_ca9_eom_iput(req, &req);

}


/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_inreplrdn()                             */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_inreplrdn(parblk)                   */
/*                    D21_pB *parblk;                                 */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 REPLACE RDN REQUEST in DUA.                        */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/
/*                                                                    */

signed32 d21_ca_len_inreplrdn(parblk)
D21_pB *parblk;
{
signed32 d23_len;

d23_len = D23_S_RPRDN + D23_S_EOM + D23_S_REP +
	  d21_ca_calc_carglen(parblk->d21_Bc_a);

if (parblk->d21_Bentry != NULL) {
    d23_len += D23_S_NM + D2_ALIGN(strlen ((char *)parblk->d21_Bentry) + 1);
}

if (parblk->d21_Bnew_rdn != NULL) {
    d23_len += D23_S_NM + D2_ALIGN(strlen ((char *)parblk->d21_Bnew_rdn) + 1);
}

return(D2_ALIGN(d23_len));
}

