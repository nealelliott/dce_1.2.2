/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20dpat.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:08  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:20  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:18  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:22  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:51  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:17:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:59:36  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:13:17  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:30:37  marrek
 * 	Changes for bug 6575.
 * 	[1993/01/28  10:16:26  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:38  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:04  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:50:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:59:06  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:38:30  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:39  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20dpat.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:10 $";
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
/* NAME         : d20dpat.o  [display AT]                             */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 23.03.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
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
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 23.03.88| Original                       | ws |         */
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
#include <stdlib.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

extern D20_omask  d20_qomask[];         /* input fields of error mask */

extern D20_omask  d20_Bomask[];         /* ADT mask                   */
extern char      *d20_Bifields[];       /* input fields of ADT-mask   */

extern char      *d20_errtab[];         /* error table                */
extern char      *d20_err_table[];         /* error table                */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static  void d20_014_fill_at_mask(At *at_entry);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_013_display_at()                                  */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        23.03.88                                              */
/*								      */
/* SYNTAX:      signed32  d20_013_display_at(nattr, at)               */
/*			short	nattr;				      */
/*			At	*at;				      */	
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will display the AT.                    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		short		nattr;	no. of entries in AT	      */
/*		At		*at;    pointer to AT		      */
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

 signed32  d20_013_display_at(
   signed16  nattr,
   At	    *at)

{	/*  d20_013_display_at()             */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 signed16    idx;
 signed16    max_idx;
 D2_pbhead   head;
 D20_p6      p6;
 signed32	*schema_index_space;
 signed32	no_selected_entries = 0;
 signed32	ret_value = D2_NOERROR;
 signed32	fkt;


/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* allocate space for selected indices.		*/
if ((schema_index_space = (signed32 *)malloc(nattr * sizeof(signed32))) == 
							(signed32 *)NULL)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  	d20_handlemask(d20_qomask); /* memory error */
	return(D2_ERROR);
	}

/* change function id. of input fields to D20_CSAY	*/
for (idx = D20_B1POS + 1, max_idx = idx + 2 * D20_B3CNT; idx < max_idx;
     idx += 2)
  	d20_Bomask[idx].d20_fkt_id = D20_CSAY;

head.d2_version = DS_V03;
p6.d20_6pbhead  = &head;

/* enable F1 function key.	*/
d20_Bomask[D20_BF1POS].d20_fkt_id = D20_FKTX;

/*  EVER                              */
for (fkt=FKTF1; ; )
	{
	if (fkt == FKTF1)
		{
		if ((ret_value = d20_handle_schema_list((signed32)DS__AT_DISPLAY, 
			schema_index_space, &no_selected_entries)) == D2_ERROR)
			break;

		if (no_selected_entries == 0)
			break;

		p6.d20_6disp    = D2_SCH_FIRST;
		p6.d20_6nentries    = no_selected_entries;
		p6.d20_6first  = p6.d20_6act = schema_index_space;
		}

  	/*  scheme display operation is ok    */
     	if (!d20_6_display_scheme(&p6))
		{
  		/*  display received entry            */
		/*  fill output mask                  */
    		d20_014_fill_at_mask((at + *p6.d20_6act));
		}
	else
		{
  		/*  display error message             */
    		switch ((int) (p6.d20_6pbhead)->d2_errvalue)
			{
      			case D2_E_SCH:    
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E86IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			case D2_B_SCH:    
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E84IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			case D2_NO_ELEM:  
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E82IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			default:
				break;
			}
		}

  	if ((p6.d20_6pbhead)->d2_errvalue == D2_NO_ELEM)
    		break;

	/*  display AT entry                 */
	fkt = d20_handlemask (d20_Bomask) ;

  	/*  break was selected                */
     	if (fkt == FKTDEL)
		break;

  	/*  SCROLL UP was selected            */
	if (fkt == FKTSCU)
  		/*  take previous AT-Entry           */
    		p6.d20_6disp    = D2_SCH_PREVIOUS;
  	else
		/*  take next AT-entry               */
    		p6.d20_6disp    = D2_SCH_NEXT;
	}

/* reset function id. of input fields to D20_CGET	*/
for (idx = D20_B1POS + 1, max_idx = idx + 2 * D20_B3CNT; idx < max_idx;
     idx += 2)
  	d20_Bomask[idx].d20_fkt_id = D20_CGET;

/* disable F1 function key.	*/
d20_Bomask[D20_BF1POS].d20_fkt_id = D20_NO_OP;

free(schema_index_space);
return(ret_value);

}	/*  d20_013_display_at()             */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_014_fill_at_mask()                                */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        30.03.88                                              */
/*								      */
/* SYNTAX:      void  d20_014_fill_at_mask(at_entry)                  */
/*			At	*at_entry;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will fill the input fields of the AT-   */
/*              mask.                                                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		at_entry   pointer to At entry to be displayed        */
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
/*inoff ***************************************************************/

static  void d20_014_fill_at_mask(
   At	*at_entry)

{                               /*  d20_014_fill_at_mask()           */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/


/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*d20_Bifields[DS_A_UP] = *d20_Bifields[DS_A_DOWN] = 
*d20_Bifields[DS_A_EXEC] = *d20_Bifields[DS_A_BREAK] = DS_DEFAULT_VALUE;

strcpy(d20_Bifields[DS_A_ABBRV], at_entry->att_abbrv);
strcpy(d20_Bifields[DS_A_NAME], at_entry->att_name);
strcpy(d20_Bifields[DS_A_OBJ_ID], at_entry->att_obj_id);
*((signed32 *) d20_Bifields[DS_A_LOWER])  = at_entry->att_lower;
*((signed32 *) d20_Bifields[DS_A_UPPER])  = at_entry->att_upper;
*((signed32 *) d20_Bifields[DS_A_NREC])   = at_entry->att_no_recur;
*((signed32 *) d20_Bifields[DS_A_SYNTAX]) = at_entry->att_syntax;
*((signed32 *) d20_Bifields[DS_A_ACL])    = at_entry->att_acl;
*((signed32 *) d20_Bifields[DS_A_INDEX])  = at_entry->att_ind_lev;
*((signed32 *) d20_Bifields[DS_A_PHON])   = at_entry->att_phon;

}                               /*  d20_014_fill_at_mask()           */


