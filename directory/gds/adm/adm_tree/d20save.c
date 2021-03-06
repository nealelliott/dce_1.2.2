/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20save.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:07  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:44  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:21  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:22:49  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:29:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:20:11  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:01  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:18:11  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:32:35  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:46:31  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:12:10  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20save.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:44 $";
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
/*								      */
/* NAME         : d20save.c                                           */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 07.07.89                                            */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.	: <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 07.07.89| Erstellung                     | hv |         */
/*								      */
/*datoff **************************************************************/

/* Includes */

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2shm.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>


/* Variables */

extern D20_omask   d20_11aomask[];

extern D20_omask   d20_qomask[];
extern char       *d20_err_table[];
extern char       *d20_errtab[];
extern char       *d20_errmess[];

extern D2_credent *p_usercred;
extern signed16    d20_bid;         /* global bind id                 */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_E00_save()                                        */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        07.07.89                                              */
/*								      */
/* SYNTAX:      signed32 d20_E00_save(parblk)                         */
/*                                                                    */
/*              D20_pE      *parblk;                                  */
/*								      */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter input for saving of  */
/*      subtrees and calls the appropriate function to save subtrees. */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_pE      *parblk;                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

signed32 d20_E00_save(D20_pE *parblk)
{                               /*  d20_E00_save                      */
char        file_name[D27_LFILE_NAME];
char        dsa_name[D2_DNL_MAX];
Name_string object;
char        obj_name[D2_DNL_MAX];
char       *p_obj;
signed32    subset;
C_arg       c_arg;
Errcode     ret_code;
signed16    bind_id = -1;
d2_ret_val  ret;


				/* read object                        */
if (d20_C05_read_obj((D20_pE *) parblk, TRUE, &object, &subset)
		     == D2_ERROR)
{   return(D2_NOERROR);
}
if (object == NULL)
{   p_obj = (char *)object;
}
else
{   p_obj = obj_name;
    strcpy(obj_name, (char *)object);
}

				/* read DSA and file name             */
*d20_11aomask[DS_11A_1FILE].d20_ioinfo =
*d20_11aomask[DS_11A_2FILE].d20_ioinfo = D2_EOS;
* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo = D20_M_DSA;

for ( ; ; )
{   *d20_11aomask[DS_11A_BREAK].d20_ioinfo = DS_DEFAULT_VALUE;

    d20_handlemask(d20_11aomask);

    if                          /* break was selected                 */
       (*d20_11aomask[DS_11A_BREAK].d20_ioinfo != DS_DEFAULT_VALUE)
    {   return(D2_ERROR);
    }

    strcpy(file_name, d20_11aomask[DS_11A_1FILE].d20_ioinfo);
    strcat(file_name, d20_11aomask[DS_11A_2FILE].d20_ioinfo);

    if (strlen(file_name) == 0)
    {   d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E25IDX];
	d20_handlemask (d20_qomask) ;
    }
    else
    {   break;
    }
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
	ret = d20__bind((D2_name_string) dsa_name+1, &bind_id);

	if (ret == D2_ERROR)
	{
	    d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[D20_E24IDX];
	    d20_handlemask (d20_qomask) ;
	}
	else
	{   c_arg.serv_cntrls = D2_LOCALSCOPE;
	    break;
	}
    }
}
else
{   bind_id = parblk->d20_Ebid;
    if (* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo == D20_M_DSA)
    {   c_arg.serv_cntrls = D2_NOCACHE;
    }
    else
    {   if (* (signed32 *) d20_11aomask[DS_11A_DSA].d20_ioinfo == D20_B_DSA)
	{   c_arg.serv_cntrls = D2_LOCALSCOPE;
	}
    }
}

				/* save subtree                       */
if  (d21_14_save(bind_id, &ret_code, DS_V021, (Name_string) p_obj, subset,
		c_arg.serv_cntrls, file_name) == D2_ERROR)
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
        case DS_NFOUND:     d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_errmess[D20_E18IDX];
                            break;
	default:            d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E30IDX];
			    break;
    }
    d20_handlemask(d20_qomask);
}

if ((bind_id >= 0) && (bind_id != d20_bid))
{   d20__unbind(bind_id);
}

return(D2_NOERROR);

}                               /*  d20_E00_save                      */
