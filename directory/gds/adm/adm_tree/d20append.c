/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20append.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:58  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:38  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:05:53  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:21:42  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:22:51  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:18:50  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:36  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:33  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:16:28  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:30:57  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:43:54  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:36  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20append.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:37 $";
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
/* NAME         : d20append.c                                         */
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

/**********************************************************************/
/*								      */
/*		       Modulidentification			      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
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
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D20_omask   d20_11bomask[];
extern D20_omask   d20_6omask[];

extern D20_omask   d20_qomask[];
extern char       *d20_err_table[];
extern char       *d20_errtab[];
extern char       *d20_errmess[];

extern D2_credent *p_usercred;
extern signed16    d20_b_err;       /* error from bind                */
extern signed16    d20_bid;         /* global bind id                 */

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_E01_append()                                      */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        07.07.89                                              */
/*								      */
/* SYNTAX:      signed32 d20_E01_append(parblk)                       */
/*                                                                    */
/*              D20_pE      *parblk;                                  */
/*								      */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter input in order to    */
/*      append subtrees and calls the appropriate function to append  */
/*      subtrees.                                                     */
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
/*              ds0_530_display_errlist();                            */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed32 d20_E01_append(D20_pE *parblk)

{                           /*  d20_E01_append()                  */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

char          file_name[D27_LFILE_NAME];
char          dsa_name[D2_DNL_MAX];
Name_string   object;
char          obj_name[D2_DNL_MAX];
char         *p_obj;
signed32      subset;
Errcode       ret_code;
signed16      bind_id = -1;
Bool          overwrite;
signed32      inherit;
Name_string  *old_objects = NULL;
d2_ret_val    ret;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

				/* read object                        */
*(signed32 *) d20_6omask[D20_60POS+1].d20_ioinfo = OBJECT;
if (d20_C05_read_obj((D20_pE *) parblk, FALSE, &object, &subset)
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

				/* read file name and params. for append */
*d20_11bomask[DS_11B_1FILE].d20_ioinfo =
*d20_11bomask[DS_11B_2FILE].d20_ioinfo = D2_EOS;
* (signed32 *) d20_11bomask[DS_11B_OV_WR].d20_ioinfo = TRUE;
* (signed32 *) d20_11bomask[DS_11B_ACL].d20_ioinfo = NEW_ACL;
* (signed32 *) d20_11bomask[DS_11B_DSA].d20_ioinfo = D20_B_DSA;

for ( ; ; )
{   *d20_11bomask[DS_11B_BREAK].d20_ioinfo = DS_DEFAULT_VALUE;

    d20_handlemask(d20_11bomask);

    if                          /* break was selected                 */
       (*d20_11bomask[DS_11B_BREAK].d20_ioinfo != DS_DEFAULT_VALUE)
    {   return(D2_ERROR);
    }

    strcpy(file_name, d20_11bomask[DS_11B_1FILE].d20_ioinfo);
    strcat(file_name, d20_11bomask[DS_11B_2FILE].d20_ioinfo);

    if (strlen(file_name) == 0)
    {   d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E25IDX];
	d20_handlemask (d20_qomask) ;
    }
    else
    {   break;
    }
}

overwrite = (Bool) * (signed32 *) d20_11bomask[DS_11B_OV_WR].d20_ioinfo;
inherit = * (signed32 *) d20_11bomask[DS_11B_ACL].d20_ioinfo;

if (* (signed32 *) d20_11bomask[DS_11B_DSA].d20_ioinfo == D20_S_DSA)
{                               /* read specific DSA                  */
    for ( ; ; )
    {   if                      /* error on reading DSA               */
	   (d20_C06_read_dsa(parblk->d20_Eenv, dsa_name)
			     == D2_ERROR)
	{   return(D2_ERROR);
	}
				/* bind to specific DSA               */
	ret = d20__bind((D2_name_string) (dsa_name+1), &bind_id);

	if (ret == D2_ERROR)
	{   
            d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[map_errnum(
							   d20_b_err)];
	    d20_handlemask (d20_qomask) ;
	}
	else
		break;
    }
}
else
{   bind_id = parblk->d20_Ebid;
}

				/* append subtree                     */
if  (d21_12_appnd(bind_id, &ret_code, DS_V021, file_name, (Name_string) p_obj,
		inherit, overwrite, &old_objects) == D2_ERROR)
{   switch((int) ret_code.errvalue)
    {   case DS_SEARCH_ERR: d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E31IDX];
			    break;
	case DS_ADD_ERR:    d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E32IDX];
			    break;
	case DS_MOD_ERR:    d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E33IDX];
			    break;
	case DS_FILE_ERR:   d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E36IDX];
			    break;
	case DS_ALLOC_ERROR:d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E04IDX];
			    break;
	case DS_NO_FILE:     d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E35IDX];
			    break;
	case DS_NFOUND:     d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E34IDX];
			    break;
	default:            d20_qomask[D20_Q1POS].d20_ioinfo =
					   d20_err_table[D20_E37IDX];
			    break;
    }
    d20_handlemask(d20_qomask);
}

if ((bind_id >= 0) && (bind_id != d20_bid))
{   d20__unbind(bind_id);
}

if (old_objects != NULL)
{
  ds0_530_display_errlist(old_objects,overwrite == TRUE ?
		  D20_EH_OVWR : D20_EH_NOVWR);
  free(old_objects);
}

return(D2_NOERROR);

}                            /*  d20_E01_append()                  */
