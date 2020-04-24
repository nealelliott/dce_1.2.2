/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20copy.c,v $
 * Revision 1.1.717.2  1996/02/18  19:39:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:44  marty]
 *
 * Revision 1.1.717.1  1995/12/08  15:14:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:03  root]
 * 
 * Revision 1.1.715.2  1994/03/23  15:04:42  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:14  keutel]
 * 
 * Revision 1.1.715.1  1994/02/22  15:57:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:22:27  marrek]
 * 
 * Revision 1.1.713.2  1993/10/14  16:46:15  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:42:27  keutel]
 * 
 * Revision 1.1.713.1  1993/10/13  17:29:20  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:13:17  keutel]
 * 
 * 	October 1993 code drop
 * 
 * Revision 1.1.6.2  1993/08/10  09:27:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:19:45  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:48  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:51  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:17:41  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:32:06  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:46:01  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:59  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20copy.c,v $ $Revision: 1.1.717.2 $ $Date: 1996/02/18 19:39:42 $";
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
/* NAME         : d20copy.c                                           */
/*                                                                    */
/* AUTHOR       : Josep Pulpillo                                      */
/* DATE         : 18.09.89                                            */
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
/*      0.1 | 13.09.89| Erstellung                     | hv |         */
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

extern D20_omask   d20_11aomask[];
extern D20_omask   d20_11bomask[];
extern D20_omask   d20_6omask[];
 
extern D20_omask   d20_qomask[];
extern char       *d20_err_table[];
extern char       *d20_errtab[];
extern char       *d20_errmess[];
 
extern D2_credent *p_usercred;
extern signed16    d20_b_err;       /* error from bind                */

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME       : d20_E02_copy()                                        */
/*                                                                    */
/* AUTHOR:      Josep Pulpillo                                        */
/* DATE:        14.09.89                                              */
/*                                                                    */
/* SYNTAX:      signed32 d20_E02_copy(parblk)                         */
/*                                                                    */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter input for copying of */
/*      subtrees and calls the appropriate function to copy subtrees. */
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
/*              free();                                               */
/*              ds0_530_display_errlist();                            */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/
 
signed32 d20_E02_copy(D20_pE *parblk)
{                               /*  d20_E02_copy                      */
char          dsa_name[D2_DNL_MAX];
Name_string   object;
char          obj_name_sav[D2_DNL_MAX];
char          obj_name_app[D2_DNL_MAX];
char         *p_obj_sav,*p_obj_app;
signed32      subset_sav,subset_app;
C_arg         c_arg;
signed16      source_bind_id = -1;
signed16      sink_bind_id = -1;
Errcode       ret_code;
Bool          overwrite;
signed32      inherit;
Name_string  *old_objects = NULL;
d2_ret_val    ret;
 
 
                                /* read object to be saved            */
if (d20_C05_read_obj((D20_pE *) parblk, TRUE, &object, &subset_sav)
                     == D2_ERROR)
{   return(D2_NOERROR);
}
if (object == NULL)
{   p_obj_sav = (char *)object;
}
else
{   p_obj_sav = obj_name_sav;
    strcpy(obj_name_sav, (char *)object);
}
                                /* read source DSA                    */
* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo = D20_M_DSA;
d20_11aomask[DS_11A_1FILE-1].d20_fkt_id =
d20_11aomask[DS_11A_1FILE].d20_fkt_id =
d20_11aomask[DS_11A_2FILE].d20_fkt_id = D20_NO_OP;

*d20_11aomask[ DS_11A_BREAK ].d20_ioinfo = DS_DEFAULT_VALUE;
d20_handlemask(d20_11aomask);

d20_11aomask[DS_11A_1FILE-1].d20_fkt_id = D20_CSAY;
d20_11aomask[DS_11A_1FILE].d20_fkt_id =
d20_11aomask[DS_11A_2FILE].d20_fkt_id = D20_CGET;

if                          /* break was selected                 */
   (*d20_11aomask[ DS_11A_BREAK ].d20_ioinfo != DS_DEFAULT_VALUE)
{   return(D2_ERROR);
}

if (* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo == D20_S_DSA)
{                               /* read specific DSA                  */
    for ( ; ; )
    {   if                      /* error on reading DSA               */
	   (d20_C06_read_dsa(parblk->d20_Eenv, dsa_name)
                             == D2_ERROR)
        {   return(D2_ERROR);
        }
                                /* bind to specific DSA               */
	ret = d20__bind((D2_name_string) (dsa_name+1), &source_bind_id);
 
	if (ret == D2_ERROR)
        {   
            d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[map_errnum(
							   d20_b_err)];
            d20_handlemask (d20_qomask) ;
        }
        else
	{   c_arg.serv_cntrls = D2_LOCALSCOPE;
            break;
        }
    }
}
else
{   source_bind_id = parblk->d20_Ebid;
    if (* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo == D20_M_DSA)
    {   c_arg.serv_cntrls = D2_NOCACHE;
    }
    else
    {   if (* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo == D20_B_DSA)
	{   c_arg.serv_cntrls = D2_LOCALSCOPE;
        }
    }
}
                                /* read object under wich the subtree */
				/* will be appended                   */
*(signed32 *) d20_6omask[D20_60POS+1].d20_ioinfo = OBJECT;
if (d20_C05_read_obj((D20_pE *) parblk, FALSE, &object, &subset_app)
    == D2_ERROR)
{   if ((source_bind_id >= 0) && (source_bind_id != parblk->d20_Ebid))
    {   d20__unbind(source_bind_id);
    }
    return(D2_NOERROR);
}
if (object == NULL)
{   p_obj_app = (char *)object;
}
else
{   p_obj_app = obj_name_app;
    strcpy(obj_name_app, (char *)object);
}

				/* read target DSA, overwrite and     */
                                /* ACL parameters                     */
d20_11bomask[DS_11B_1FILE-1].d20_fkt_id =
d20_11bomask[DS_11B_1FILE].d20_fkt_id =
d20_11bomask[DS_11B_2FILE].d20_fkt_id = D20_NO_OP;
* (signed32 *) d20_11bomask[DS_11B_OV_WR].d20_ioinfo = TRUE;
* (signed32 *) d20_11bomask[DS_11B_ACL].d20_ioinfo = NEW_ACL;
* (signed32 *) d20_11bomask[DS_11B_DSA].d20_ioinfo = D20_B_DSA;
*d20_11bomask[DS_11B_BREAK].d20_ioinfo = DS_DEFAULT_VALUE;

d20_handlemask(d20_11bomask);

d20_11bomask[DS_11B_1FILE-1].d20_fkt_id = D20_CSAY;
d20_11bomask[DS_11B_1FILE].d20_fkt_id =
d20_11bomask[DS_11B_2FILE].d20_fkt_id = D20_CGET;

if                          /* break was selected                 */
   (*d20_11bomask[DS_11B_BREAK].d20_ioinfo != DS_DEFAULT_VALUE)
{   if ((source_bind_id >= 0) && (source_bind_id != parblk->d20_Ebid))
    {   d20__unbind(source_bind_id);
    }
    return(D2_ERROR);
}

overwrite = (Bool) * (signed32 *) d20_11bomask[DS_11B_OV_WR].d20_ioinfo;
inherit = * (signed32 *) d20_11bomask[DS_11B_ACL].d20_ioinfo;
if (* (signed32 *) d20_11bomask[DS_11B_DSA].d20_ioinfo == D20_S_DSA)
{                               /* read specific DSA                  */
    for ( ; ; )
    {   if                      /* error on reading DSA               */
	   (d20_C06_read_dsa(parblk->d20_Eenv, dsa_name)
                             == D2_ERROR)
	{   if (source_bind_id >= 0 && source_bind_id != parblk->d20_Ebid)
	    {   d20__unbind(source_bind_id);
	    }
	    return(D2_ERROR);
        }
                                /* bind to specific DSA               */
	ret = d20__bind((D2_name_string) (dsa_name+1), &sink_bind_id);
 
	if (ret == D2_ERROR)
        {   
            d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[map_errnum(
							   d20_b_err)];
            d20_handlemask (d20_qomask) ;
        }
        else
	{
            break;
        }
    }
}
else
{   sink_bind_id = parblk->d20_Ebid;
}
                                /* copy subtree                       */
if  (d21_11_copy(source_bind_id, &ret_code, DS_V021, (Name_string) p_obj_sav,
	subset_sav, &c_arg, sink_bind_id, (Name_string) p_obj_app, inherit,
	overwrite, &old_objects)
      == D2_ERROR)
{   switch((int) ret_code.errvalue)
    {   case DS_NO_RESPONSE:d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E27IDX];
                            break;
        case DS_SEARCH_ERR: d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E28IDX];
                            break;
        case DS_FILE_ERR:   d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E29IDX];
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
	case DS_NO_FILE:    d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E35IDX];
                            break;
        case DS_NFOUND:     d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_errmess[D20_E18IDX];
                            break;
	case DS_PAR_NFOUND: d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E50IDX];
                            break;
        default:            d20_qomask[D20_Q1POS].d20_ioinfo =
                                           d20_err_table[D20_E30IDX];
                            break;
    }
    d20_handlemask(d20_qomask);
}
 
if ((source_bind_id >= 0) && (source_bind_id != parblk->d20_Ebid))
{   d20__unbind(source_bind_id);
}
if ((sink_bind_id >= 0) && (sink_bind_id != parblk->d20_Ebid))
{   d20__unbind(sink_bind_id);
}

if (old_objects != NULL)
{
  ds0_530_display_errlist(old_objects,overwrite == TRUE ?
		  D20_EH_OVWR : D20_EH_NOVWR);
  free(old_objects);
}

return(D2_NOERROR);
}                               /*  d20_E02_copy                      */
