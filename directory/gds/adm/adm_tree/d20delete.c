/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20delete.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:45  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:05  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:43  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:18  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:38  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:22:40  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:28:30  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:19:59  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:31:05  marrek
 * 	Add D2_PREF_CHAINING.
 * 	[1993/01/28  10:18:43  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:57  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:17:57  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:32:23  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:46:18  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:12:05  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20delete.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:43 $";
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
/* NAME         : d20delete.c                                         */
/*                                                                    */
/* AUTHOR       : N. Serra, D 525 (CDS)                               */
/* DATE         : 12.09.89                                            */
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
/*      0.1 | 12.09.89| Erstellung                     | hv |         */
/*                                                                    */
/*datoff **************************************************************/

/* Includes */
 
#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>
 

/* Variables */

extern D20_omask   d20_6omask[];
extern D20_omask   d20_11aomask[];
 
extern D20_omask   d20_qomask[];
extern char       *d20_err_table[];
extern char       *d20_errtab[];
extern char       *d20_errmess[];
 
extern D2_credent *p_usercred;
extern signed16    d20_b_err;       /* error from bind                */
extern signed16    d20_bid;         /* global bind id                 */
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d20_E04_delete()                                      */
/*                                                                    */
/* AUTHOR:      N. Serra, D 525 (CDS)                                 */
/* DATE:        12.09.89                                              */
/*                                                                    */
/* SYNTAX:      signed32 d20_E04_delete(parblk)                       */
/*                                                                    */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter input for deleting   */
/*      of subtrees and calls the appropriate function to delete      */
/*      subtrees.                                                     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read-only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*              ds0_530_display_errlist();                            */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/
 
signed32 d20_E04_delete(D20_pE *parblk)
{                               /*  d20_E04_delete                    */
char         dsa_name[D2_DNL_MAX];
Name_string  object;
char         obj_name[D2_DNL_MAX];
char        *subtree_root;
signed32     subset;
signed32     serv_cntrls;
Errcode      return_code;
signed16     bind_id = -1;
Name_string *not_deleted;
d2_ret_val   ret;
 
                                /* read object                        */
* (signed32 *)d20_6omask[D20_60POS+1].d20_ioinfo = OBJECT_SUBORDINATES;
if (d20_C05_read_obj((D20_pE *) parblk, FALSE, &object, &subset)
                     == D2_ERROR)
{   return(D2_NOERROR);
}
if (object == NULL)
{   subtree_root = (char *)object;
}
else
{   subtree_root = obj_name;
    strcpy(obj_name, (char *)object);
}
 
                                /* read DSA                           */
* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo = D20_M_DSA;
d20_11aomask[DS_11A_1FILE-1].d20_fkt_id =
d20_11aomask[DS_11A_1FILE].d20_fkt_id =
d20_11aomask[DS_11A_2FILE].d20_fkt_id = D20_NO_OP;
 
    *d20_11aomask[DS_11A_BREAK].d20_ioinfo = DS_DEFAULT_VALUE;

    d20_handlemask(d20_11aomask);

    if                          /* break was selected                 */
       (*d20_11aomask[DS_11A_BREAK].d20_ioinfo != DS_DEFAULT_VALUE)
    {   d20_11aomask[DS_11A_1FILE-1].d20_fkt_id = D20_CSAY;
	d20_11aomask[DS_11A_1FILE].d20_fkt_id =
	d20_11aomask[DS_11A_2FILE].d20_fkt_id = D20_CGET;
	return(D2_ERROR);
    }

d20_11aomask[DS_11A_1FILE-1].d20_fkt_id = D20_CSAY;
d20_11aomask[DS_11A_1FILE].d20_fkt_id =
d20_11aomask[DS_11A_2FILE].d20_fkt_id = D20_CGET;
 
serv_cntrls = D2_NOREFERRAL | D2_USEDSA | D2_DONT_STORE | D2_DREFALIAS
	      | D2_PREF_ADM_FUN | D2_PREF_CHAINING;

if (* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo == D20_S_DSA)
{                               /* read specific DSA                  */
    for ( ; ; )
    {   if                      /* error on reading DSA               */
	   (d20_C06_read_dsa(parblk->d20_Eenv, dsa_name)
                             == D2_ERROR)
        {   return(D2_ERROR);
        }
                                /* bind to specific DSA               */
	ret = d20__bind((D2_name_string) dsa_name+1, &bind_id);
 
	if (ret == D2_ERROR)
        {   
            d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[map_errnum(
							   d20_b_err)];
            d20_handlemask (d20_qomask) ;
        }
        else
	{   serv_cntrls |= D2_LOCALSCOPE;
            break;
        }
    }
}
else
{   bind_id = parblk->d20_Ebid;
    if (* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo == D20_M_DSA)
    {   serv_cntrls |= D2_NOCACHE;
    }
    else
    {   if (* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo == D20_B_DSA)
	{   serv_cntrls |= D2_LOCALSCOPE;
        }
    }
}
                                /* delete subtree                     */
if  (d21_10_delst(bind_id, &return_code, DS_V021, (Name_string) subtree_root,
		serv_cntrls, &not_deleted) == D2_ERROR)
{   switch((int) return_code.errvalue)
    {   case DS_NO_RESPONSE:d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E27IDX];
                            break;
        case DS_DEL_ERR:    d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E38IDX];
                            break;
        case DS_SEARCH_ERR: d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E39IDX];
                            break;
        case DS_ALLOC_ERROR:d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E04IDX];
                            break;
        case DS_NO_BIND:    d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_errmess[D20_E01IDX];
                            break;
        case DS_NFOUND:     d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_errmess[D20_E18IDX];
                            break;
        case DS_SERV_CTRLS_INVALID: d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E40IDX];
                            break;
        default:            d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E30IDX];
                            break;
    }
    d20_handlemask(d20_qomask);
}

if (not_deleted != NULL) { 
  ds0_530_display_errlist(not_deleted,D20_EH_NDEL);
  free(not_deleted);    /* memory allocated by "d21_10_delst()" function */
}

if ((bind_id >= 0) && (bind_id != d20_bid))
{   d20__unbind(bind_id);
}
 
return(D2_NOERROR);
 
}                               /*  d20_E04_delete                    */
