/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20chngall.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:42  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:02  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:19  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:22:24  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:41  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:09  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:22:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:26:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:19:30  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:47  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:17:20  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:31:49  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:44:32  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:53  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20chngall.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:41 $";
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
/* NAME         : chngall.o   [Administrations-Schnittstelle]         */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 10.03.88                                            */
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
/*      0.1 | 10.03.88| Erstellung                     | ws |         */
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

/*****  Operating-System-Includes  *****/

#include <gds.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2info.h>
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

extern D20_omask  d20_6omask[];
extern D20_omask d20_qomask[];         /* Input fields for error mask */
extern D20_omask d20_msgomask[];       /* Input fields for message    */
				       /* mask                        */
extern char      *d20_errtab[];        /* Error mask                  */

extern Ds_v2_info       info ;          /* common IAPL-interface      */
					/* parameter block            */

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : ds0_505_change_all_attr()                             */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        10.03.88                                              */
/*								      */
/* SYNTAX:      void  ds0_505_change_all_attr(subset)                 */
/*              signed32         subset;                              */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will look for all entries with the      */
/*              purported filter attribute.                           */
/*              All entries returned via DS_SEARCH will be changed.   */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed32         subset;        subset information    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:     -                                                 */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          ds4_b11_call_directory()                  */
/*                          ds4_b02_fill_attr()                       */
/*                          d20_handlemask()                          */
/*                          d20_set_subset()                          */
/*                          d20_dnget()                               */
/*                          sprintf()                                 */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 void ds0_505_change_all_attr(
   signed32 subset)

{                               /*  ds0_505_change_all_attr()         */

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

 char           message[D20_MAXMSG];
 signed16       mode;
 signed16       anz = 0;
 Bool           error = FALSE;
 D2_name_string obj_name;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_set_subset(subset);
				/*  set filter attribute              */
info.filter_present = TRUE;
info.filter.d2_fi_opmod = D2_ITEM;
info.filter.d2_fi_v.d2_fi_item.d2_fi_match = D2_EQUAL;
info.filter.d2_fi_v.d2_fi_item.d2_fi_type =
		    info.modinfo->d2_m_attr.d2_a_type;
info.filter.d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_att_val.d2_a_v_len =
		    info.modinfo->d2_m_attr.d2_a_val->d2_a_v_len;
info.filter.d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_att_val.d2_a_rep =
		    info.modinfo->d2_m_attr.d2_a_val->d2_a_rep;
info.filter.d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_att_val.d2_at_value =
		    info.modinfo->d2_m_attr.d2_a_val->d2_at_value;

if                              /*  search for objects not successful */
   (ds4_b11_call_directory(DS_DISPLAY_OBJECTS))
{                               /*  return(D2_ERROR)                  */
				/*  reset filter attribute            */
  info.filter_present = FALSE;
  return;
}
else
{                               
  mode = D20_BEGIN;
  while                         /*  object name found                 */
	(!d20_dnget(mode, &obj_name))
  {                             
    if                          /*  change of attribute failed        */
       (ds4_b11_call_directory(DS_MODIFY_ATTRIBUTE))
    {                           /*  set error flag                    */
      error = TRUE;
    }
else
{                           /*  increase number of change entries */
      anz++;
    }                           
    mode = D20_CONTINUE;
  }                             
  if                            /*  error flag set                    */
     (error == TRUE)
  {                             /*  display error message             */
    d20_qomask[D20_Q1POS]. d20_ioinfo = d20_errtab[D20_E49IDX];
    d20_handlemask(d20_qomask);
  }                             
				/*  display number of changed entries */
  sprintf(message,d20_errtab[D20_E23IDX], anz);
  d20_msgomask[D20_M1POS].d20_ioinfo = message;
  d20_handlemask(d20_msgomask);
  info.filter_present = FALSE;
  return;
}                               

}                               /*  ds0_505_change_all_attr()         */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_E06_modify()                                      */
/*								      */
/* AUTHOR:      Mueller V., (SNI AP 11)                               */
/* DATE:        11.06.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_E06_modify(parplk)                       */
/*                                                                    */
/*              D20_pE      *parblk;                                  */
/*								      */
/* DESCRIPTION:                                                       */
/*      This function displays the mask to enter input for modifying  */
/*      subtrees and calls the appropriate function to modify subtrees*/
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

 signed32 d20_E06_modify(D20_pE *parblk)

{                               /*  d20_E06_modify                    */

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

   Ds_at_entry  attr_entry[2];
   char        *p_obj;
   Name_string  object;
   signed32     subset;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*(signed32 *) d20_6omask[D20_60POS+1].d20_ioinfo = OBJECT_SUBORDINATES;

				/* read object                        */
if (d20_C05_read_obj((D20_pE *) parblk, FALSE, &object, &subset)
		     == D2_ERROR)
{   return(D2_NOERROR);
}

if (object == NULL)
{   p_obj = (char *)object;
}
else
{   strcpy((char *)(info.entry+1), (char *)object);
}

if (!ds0_503_mod_attr((D20_srt *) D20_UNKNOWN,D20_UNKNOWN, attr_entry))
	{                       /*  change all attributes             */
	    if (subset == OBJECT_SUBORDINATES)
		  subset = D2_WHOLE_SUBTREE;
	    ds0_505_change_all_attr(subset);
	}

return(D2_NOERROR);

}                               /*  d20_E06_modify                    */
