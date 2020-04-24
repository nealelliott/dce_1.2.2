/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21refh.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:19  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:29  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:54:07  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:45  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:42:48  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:35:20  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:33:55  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:58:58  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:48:37  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:00  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:36:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:41:51  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:32:12  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:10  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21refh.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:29 $";
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
* NAME         :  d21refh.c                                          *
*                                                                    *
* AUTHOR       :  J.Heigert                                          *
* DATE         :   3.12.87                                           *
*                                                                    *
* DESCRIPTION  :  auxiliary routines for referral handling           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <d21dua.h>

/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_cr1_add_lb()                                   */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  08.12.87                                           */
/*                                                                    */
/* DESCRIPTION  :  Adds one LoopBack in ipc-block-format              */
/*                 to lb_tab (LoopBack table);                        */
/*                                                                    */
/* INPUT-PARAMETER :  lb: LoopBack entry                              */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                 lb_tab                                             */
/*                                                                    */
/*exoff **************************************************************/

d2_ret_val d21_cr1_add_lb(
  D21_lb_info * lb_tab,
  signed16 * lb_tab_next,
  D2_name_string tr_obj,
  D2_op_progress * op_prog,
  D2_name_string dsa_name)
{
Octet_string            os_ptr;
register signed16 i;
size_t j;
byte null_str[] = "";

				/*					*/
				/* assumes that no similar entry exists */
				/*  in lb_ref table                     */
if (tr_obj == NULL)
  {
  tr_obj = null_str;
  }

i = *lb_tab_next;
j = strlen((char *)dsa_name)+strlen((char *)tr_obj)+4;
				/*                                      */
				/* fill in new entry			*/
				/*					*/
				/* allocate memory for entry            */
if ((lb_tab[i].lb_target = os_ptr = D27_MALLOC(svc_c_sev_warning,j)) == NULL)
	{
	return(D21_ALLOC_ERROR);
	}
				/* copy targetObject to allocated area  */
d27_007_norm_name((char *)tr_obj,strlen((char *)tr_obj) + 1,
				(char *)(lb_tab[i].lb_target));
os_ptr += strlen((char *)tr_obj )+2;

				/* copy dsa-name to allocated area	*/
lb_tab[i].lb_dsa_name = os_ptr;
d27_007_norm_name((char *)dsa_name,strlen((char *)dsa_name) + 1,
				(char *)(lb_tab[i].lb_dsa_name));

			      /* copy OperationProgress to LoopBack Table */
lb_tab[i].lb_op_prog.d2_nr_phase = op_prog->d2_nr_phase;
if (op_prog->d2_nr_phase == D2_NOT_STARTED)
 {
  lb_tab[i].lb_op_prog.d2_next_rdn = 0 ;
  }
else
 {
  lb_tab[i].lb_op_prog.d2_next_rdn = op_prog->d2_next_rdn;
 }
*lb_tab_next +=1;

return(D2_NOERROR);
}       /* d21_cr1_add_lb */


/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_cr3_check_lb()                                 */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  08.12.87                                           */
/*                                                                    */
/* DESCRIPTION  :  check for the LoopBack conditions                  */
/*                                                                    */
/* INPUT-PARAMETER :  ptr to dsa-name: dsa-name of referred dsa       */
/*                    TargetName     : TargetName of referral         */
/*                    op_prog        : OperationProg. of referral     */
/*                                                                    */
/* RETURN-VALUE :  TRUE: referral info returned                    */
/*                 FALSE: no unprocessed referral found in ref_tab */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                 lb_tab                                            */
/*                                                                    */
/*exoff **************************************************************/

Bool d21_cr3_check_lb(
  D21_lb_info * lb_tab,
  signed16 * lb_tab_next,
  D2_name_string tr_obj,
  D2_op_progress * op_prog,
  D2_name_string  dsa_name)
{
char  norm_target[D2_DNL_MAX + 1];
char  norm_dsa[D2_DNL_MAX + 1];
signed32 ot_len;
byte null_str[] = "";

register signed16 i ;
			      /*                                          */
			      /* Scan entries for the LoopBack conditions */
			      /*                                          */
  if (*lb_tab_next == 0) { return(TRUE); }


  if (tr_obj == NULL)
    {
    tr_obj = null_str;
    }

  d27_007_norm_name((char *)tr_obj,strlen((char *)tr_obj) + 1,norm_target);
  d27_007_norm_name((char *)dsa_name,strlen((char *)dsa_name) + 1,norm_dsa);
  for (i = 0; i < *lb_tab_next ; i++)
  {
      ot_len = strlen((char *)lb_tab[i].lb_target);
      if(!strcmp((char *)lb_tab[i].lb_dsa_name,norm_dsa) &&
	 !strncmp((char *)lb_tab[i].lb_target,norm_target,ot_len) &&
	 (norm_target[ot_len] == D2_EOS ||
	  norm_target[ot_len] == D2_RDN_SEP))
      {
       return(TRUE);
      }
  }       /* for */
				/* no not_proc entry found		*/
  return(FALSE);
}       /* d21_cr3_check_lb() */

/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_cr4_purge_lb_tab()                            */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  08.12.87                                           */
/*                                                                    */
/* DESCRIPTION  :  clears all entries in lb_tab; frees allocated     */
/*                 heap space                                         */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                 lb_tab                                            */
/*                                                                    */
/*exoff **************************************************************/

void d21_cr4_purge_lb_tab(
  D21_lb_info * lb_tab,
  signed16 * lb_tab_next)
{
register signed16 i;
				/*					*/
				/* walk through lb_tab                 */
				/*					*/
for (i=0; i<*lb_tab_next; i++)
	{
	    free (lb_tab[i].lb_target);
	}	/* for */
lb_tab_next = 0;
}       /* d21_cr4_purge_lb_tab() */
