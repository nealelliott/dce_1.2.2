/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20tree.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:08  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:46  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:24  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:42  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:22:58  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:30:11  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:20:24  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:18:25  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:32:47  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:46:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:12:15  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20tree.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:45 $";
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
/* NAME         : d20tree.c                                           */
/*                                                                    */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 07.07.89                                            */
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
/*      0.1 | 07.07.89| Erstellung                     | hv |         */
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
#include <stdio.h>
#include <string.h>
 
#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>
 
/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                 T Y P E  -  D E F I N I T I O N S                  */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                 D E K L A R A T I O N E N      DATEN               */
/*                                                                    */
/**********************************************************************/
 
extern D20_omask  d20_6omask[] ;         /* Maske 6 */
extern D20_omask  d20_10omask[];
 
extern D20_omask  d20_qomask[];
extern char      *d20_errtab[];
 
extern char       d20_actopmsg[];
extern char      *d20_tree_op[];
extern char      *d20_op[];
 
/**********************************************************************/
/*                                                                    */
/*                D E K L A R A T I O N E N      FUNCTIONEN           */
/*                                                                    */
/**********************************************************************/
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d20_E_tree_handling()                                 */
/*                                                                    */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        07.07.89                                              */
/*                                                                    */
/* SYNTAX:      signed16 d20_E_tree_handling(parblk)                  */
/*                                                                    */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter the tree handling    */
/*      functions.                                                    */
/*      The selected tree handling function will be performed.        */
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
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/
 
 signed16 d20_E_tree_handling(D20_pE *parblk)
 
{                               /*  d20_E_tree_handling               */
 
/**********************************************************************/
/*                                                                    */
/*                 T Y P E  -  D E F I N I T I O N S                  */
/*                                                                    */
/**********************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                  D E K L A R A T I O N E N                         */
/*                                                                    */
/**********************************************************************/
 
signed32 op;
d2_ret_val ret_value = D2_NOERROR;
 
/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/
 
			    /* disable OBJECT-CLASS field */
d20_6omask[D20_64POS].d20_fkt_id = d20_6omask[D20_64POS + 1].d20_fkt_id =
				   D20_NO_OP;

d20_init();
for (; ret_value == D2_NOERROR; )
{   strcpy(d20_actopmsg,d20_op[DS__HANDLE_TREE]);
    *d20_10omask[DS_10_BREAK].d20_ioinfo = DS_DEFAULT_VALUE;

    		/* show operations for delta updates  */
    d20_handlemask (d20_10omask);

    if
       (*d20_10omask[DS_10_BREAK].d20_ioinfo != DS_DEFAULT_VALUE)
    {                       /* enable OBJECT-CLASS field */
	    d20_6omask[D20_64POS].d20_fkt_id = D20_CSAY;
	    d20_6omask[D20_64POS + 1].d20_fkt_id = D20_CGET;
				/* return(D2_NOERROR)                 */
	    return(D2_NOERROR);
    }

    op = * ((signed32 *) d20_10omask[DS_10_OP].d20_ioinfo);
    switch ((int) op)
    {   case DS__END:           ret_value = D2_ERROR;
                                break;
        case DS__SAVE_TREE:     strcpy(d20_actopmsg, d20_tree_op[op]);
                                d20_E00_save(parblk);
                                break;
        case DS__APPND_TREE:    strcpy(d20_actopmsg, d20_tree_op[op]);
                                d20_E01_append(parblk);
                                break;
        case DS__COPY_TREE:     strcpy(d20_actopmsg, d20_tree_op[op]);
                                d20_E02_copy(parblk);
                                break;
        case DS__DEL_TREE:      strcpy(d20_actopmsg, d20_tree_op[op]);
				d20_E04_delete(parblk);
                                break;
	case DS__CHNG_ALL_ATTR: strcpy(d20_actopmsg, d20_tree_op[op]);
				d20_E06_modify(parblk);
                                break;
        case DS__MOVE_TREE:     strcpy(d20_actopmsg, d20_tree_op[op]);
                                d20_E03_chgnam(parblk);
                                break;
	case DS__CHNG_MASTER:   strcpy(d20_actopmsg, d20_tree_op[op]);
                                d20_E05_chgmas(parblk);
                                break;
        default:                d20_qomask[D20_Q1POS].d20_ioinfo =
                                                      d20_errtab[D20_E00IDX];
                                d20_handlemask(d20_qomask);
                                break;
    }
}
 
		       /* enable OBJECT-CLASS field */
d20_6omask[D20_64POS].d20_fkt_id = D20_CSAY;
d20_6omask[D20_64POS + 1].d20_fkt_id = D20_CGET;

return(D2_NOERROR);
 
}                               /*  d20_E_tree_handling               */
