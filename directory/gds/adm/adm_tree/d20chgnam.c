/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20chgnam.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:01  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:40  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:03  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:33  marrek
 * 	Update for dce1.1.
 * 	[1994/02/09  14:47:27  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:22:04  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:25:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:19:18  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:42  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:17:05  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:31:34  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:44:20  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:47  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20chgnam.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:39 $";
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
/* NAME         : d20chgnam.c                                         */
/*                                                                    */
/* AUTHOR       : Josep Pulpillo D525 (CDS)                           */
/* DATE         : 21.09.89                                            */
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
/*      0.1 | 21.09.89| Erstellung                     | hv |         */
/*                                                                    */
/*datoff **************************************************************/
 
 
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
 
 
extern D20_omask   d20_11bomask[];
extern D20_omask   d20_6omask[];
 
extern D20_omask   d20_qomask[];
extern char       *d20_err_table[];
extern char       *d20_errtab[];
extern char       *d20_errmess[];
 
extern signed16    d20_bid;         /* global bind id                 */
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d20_E03_chgnam()                                      */
/*                                                                    */
/* AUTHOR:      Josep Pulpillo  D525 (CDS)                            */
/* DATE:        21.09.89                                              */
/*                                                                    */
/* SYNTAX:      signed32 d20_E03_chgnam(parblk)                       */
/*                                                                    */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter input for changing   */
/*      distinguished names of objects and calls the apropiate        */
/*      function to do it.                                            */
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
 
 signed32 d20_E03_chgnam(D20_pE *parblk)
{                               /*  d20_E03_chgnam                    */
Name_string   object;
char          obj_name_old[D2_DNL_MAX];
char          obj_name_new[D2_DNL_MAX];
char         *old_dn, *new_dn;
Errcode       return_code;
signed16      bind_id = -1;
Name_string  *conflicts = NULL;
signed32      subset;
Bool          overwrite;
 
 
                                /* read object to be moved            */
* (signed32 *)d20_6omask[D20_60POS+1].d20_ioinfo = OBJECT_SUBORDINATES;
if (d20_C05_read_obj((D20_pE *) parblk, FALSE, &object, &subset)
                     == D2_ERROR)
{   return(D2_NOERROR);
}
if (object == NULL)
{   old_dn = (char *)object;
}
else
{   old_dn = obj_name_old;
    strcpy(obj_name_old, (char *)object);
}
                                /* read object under wich the subtree */
                                /* will be appended                   */
* (signed32 *)d20_6omask[D20_60POS+1].d20_ioinfo = OBJECT_SUBORDINATES;
if (d20_C05_read_obj((D20_pE *) parblk, FALSE, &object, &subset)
    == D2_ERROR)
{   return(D2_NOERROR);
}
if (object == NULL)
{   new_dn = (char *)object;
}
else
{   new_dn = obj_name_new;
    strcpy(obj_name_new, (char *)object);
}
 
d20_11bomask[DS_11B_1FILE-1].d20_fkt_id =
d20_11bomask[DS_11B_1FILE].d20_fkt_id =
d20_11bomask[DS_11B_2FILE].d20_fkt_id =
d20_11bomask[DS_11B_ACL-1].d20_fkt_id =
d20_11bomask[DS_11B_ACL].d20_fkt_id =
d20_11bomask[DS_11B_DSA-1].d20_fkt_id =
d20_11bomask[DS_11B_DSA].d20_fkt_id = D20_NO_OP;
 
                                /* read overwrite parameter           */
* (signed32 *) d20_11bomask[DS_11B_OV_WR].d20_ioinfo = TRUE;
*d20_11bomask[DS_11B_BREAK].d20_ioinfo = DS_DEFAULT_VALUE;
d20_handlemask(d20_11bomask);

d20_11bomask[DS_11B_1FILE-1].d20_fkt_id =
d20_11bomask[DS_11B_ACL-1].d20_fkt_id =
d20_11bomask[DS_11B_DSA-1].d20_fkt_id = D20_CSAY;
d20_11bomask[DS_11B_1FILE].d20_fkt_id =
d20_11bomask[DS_11B_2FILE].d20_fkt_id =
d20_11bomask[DS_11B_ACL].d20_fkt_id =
d20_11bomask[DS_11B_DSA].d20_fkt_id = D20_CGET;

if                              /* break was selected                 */
   (*d20_11bomask[DS_11B_BREAK].d20_ioinfo != DS_DEFAULT_VALUE)
{   return(D2_ERROR);
}

overwrite = (Bool) * (signed32 *) d20_11bomask[DS_11B_OV_WR].d20_ioinfo;
bind_id = parblk->d20_Ebid;
                                /* change name                        */
if  (d21_15_chnam(bind_id, &return_code, DS_V021, (Name_string) old_dn,
		(Name_string) new_dn, overwrite, &conflicts) == D2_ERROR)
{   switch((int) return_code.errvalue)
    {   case DS_NO_RESPONSE:d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_errmess[DS_NO_RESPONSE*(-1)];
                            break;
        case DS_ACCESS_RIGHTS_INSUFFICIENT:
			    d20_qomask[D20_Q1POS].d20_ioinfo =
                            d20_errmess[DS_ACCESS_RIGHTS_INSUFFICIENT*(-1)];
                            break;
        case DS_SEARCH_ERR: d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E49IDX];
                            break;
        case DS_ALLOC_ERROR:d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E04IDX];
                            break;
	case DS_ADD_ERR:    d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E32IDX];
			    break;
	case DS_MOD_ERR:    d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E33IDX];
			    break;
	case DS_FILE_ERR:   d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E29IDX];
			    break;
	case DS_NO_FILE:    d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E35IDX];
			    break;
	case DS_NFOUND:     d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_errmess[DS_NFOUND*(-1)];
			    break;
	case DS_DEL_ERR:    d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E38IDX];
                            break;
        case DS_NO_BIND:    d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_errmess[DS_NO_BIND*(-1)];
                            break;
        case DS_PAR_NFOUND: d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E50IDX];
                            break;
        case DS_SERV_CTRLS_INVALID: d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E40IDX];
                            break;
        default:            d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E51IDX];
                            break;
    }
    d20_handlemask(d20_qomask);
}
 
if ((bind_id >= 0) && (bind_id != d20_bid))
{   d20__unbind(bind_id);
}
 
if (conflicts != NULL) {
  ds0_530_display_errlist(conflicts,overwrite == TRUE ?
		  D20_EH_OVWR : D20_EH_NOVWR);
  free(conflicts);      /* memory allocated by "d21_15_chnam()" function */
}

return(D2_NOERROR);
 
}                               /*  d20_E03_chgnam                    */
