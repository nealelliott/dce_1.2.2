/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20dpsrt.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:24  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:13  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:09:03  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:20  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:31  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:17:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:01:42  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:13:46  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:30:57  marrek
 * 	Changes for bug 6575.
 * 	[1993/01/28  10:17:54  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:15  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:51:23  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:59:36  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:38:56  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:52  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20dpsrt.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:13 $";
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
/* NAME         : d20dpsrt.o  [display SRT]                           */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 10.01.91					      */
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

extern D20_omask  d20_qomask[];         
extern char      *d20_err_table[];     /* and more Error Messages     */

extern D20_omask  d20_Comask[];         /* SRT mask                   */

extern char      *d20_Cifields[];       /* input fields of SRT-mask   */

extern char      *d20_errtab[];         /* error table                */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_021_display_srt()                                 */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        10.01.91					      */
/*								      */
/* SYNTAX:      signed32  d20_021_display_srt(nsrule, srt)            */
/*			signed16  nsrule;			      */
/*			Srt	 *srt;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will display the SRT.                   */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16  nsrule; 	no. of entries in SRT         */
/*		Srt	 *srt;		pointer to SRT		      */
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

 signed32 d20_021_display_srt(
   signed16  nsrule,
   Srt	    *srt)

{                               /*  d20_021_display_srt()             */

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
 signed32   *schema_index_space;
 signed32    no_selected_entries = 0;
 signed32    ret_value=D2_NOERROR;
 signed32    fkt;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* allocate space for selected indices.		*/
if ((schema_index_space = (signed32 *)malloc(nsrule * sizeof(signed32))) == 
							(signed32 *)NULL)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  	d20_handlemask(d20_qomask); /* memory error */
	return(D2_ERROR);
	}

/*	change the function id. of input fields to D20_CSAY		   */
for (idx = D20_S_RULE_POS + 1, max_idx = idx + 2 * D20_S_FIELD_CNT; 
     idx < max_idx; idx += 2)
	d20_Comask[idx].d20_fkt_id = D20_CSAY;

head.d2_version = DS_V03;

p6.d20_6pbhead  = &head;

/* enable F1 function key.	*/
d20_Comask[D20_S_F1_POS].d20_fkt_id = D20_FKTX;

/*  EVER                              */
for (fkt=FKTF1; ; )
	{
	if (fkt == FKTF1)
		{
		if ((ret_value = d20_handle_schema_list((signed32)DS__SRT_DISPLAY, 
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
    		d20_022_fill_srt_mask((srt + *p6.d20_6act));
		}
	else
		{
  		/*  display error message             */
    		switch ((int) (p6.d20_6pbhead)->d2_errvalue)
			{
      			case D2_E_SCH:    
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E69IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			case D2_B_SCH:    
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E66IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			case D2_NO_ELEM:  
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E65IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			default:
				break;
   			} 
		}

  	if ((p6.d20_6pbhead)->d2_errvalue == D2_NO_ELEM)
    		break;

	/*  display SRT entry                 */
	fkt = d20_handlemask (d20_Comask) ;

  	/*  break was selected                */
     	if (fkt == FKTDEL)
    		break;

	/*  SCROLL UP was selected            */
	if (fkt == FKTSCU)
		/*  take previous SRT-Entry   */
		p6.d20_6disp    = D2_SCH_PREVIOUS;
	else
		/*  take next SRT-entry       */
		p6.d20_6disp    = D2_SCH_NEXT;
	}


/*	reset the function id. of input fields to D20_CGET     		*/
for (idx = D20_S_RULE_POS + 1, max_idx = idx + 2 * D20_S_FIELD_CNT; 
     idx < max_idx; idx += 2)
	d20_Comask[idx].d20_fkt_id = D20_CGET;

/* disable F1 function key.	*/
d20_Comask[D20_S_F1_POS].d20_fkt_id = D20_NO_OP;

free(schema_index_space);
return(ret_value);
}                               /*  d20_021_display_srt()             */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_022_fill_srt_mask()                               */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        10.01.91					      */
/*								      */
/* SYNTAX:      void  d20_022_fill_srt_mask(srt_entry)                */
/*			Srt	*srt_entry;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will fill the input fields of the SRT-  */
/*              mask.                                                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Srt 		*srt_entry; SRT-entry to be displayed */
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

 void d20_022_fill_srt_mask(
   Srt  *srt_entry)

{                               /*  d20_022_fill_srt_mask()           */

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
signed32   rem_len,j;
char	  *run_ptr;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*d20_Cifields[DS_S_UP] = *d20_Cifields[DS_S_DOWN] = *d20_Cifields[DS_S_EXEC]
		       = *d20_Cifields[DS_S_BREAK] = DS_DEFAULT_VALUE;

*((signed32 *)d20_Cifields[DS_S_RULE_NO]) 	= srt_entry->rule_nr;
*((signed32 *)d20_Cifields[DS_S_PAR_RULE_NO])= srt_entry->par_rule_nr;

rem_len = D2_NAM_LEN - 1;
run_ptr = d20_Cifields[DS_S_NAM_ATTR_ABBRV];
for (j=0; j<srt_entry->no_nam_attr;j++)
	if (rem_len >= (int)strlen(srt_entry->nam_attr[j]))
		{
		strcpy(run_ptr,srt_entry->nam_attr[j]);
		rem_len -= strlen(run_ptr);
		run_ptr += strlen(run_ptr);
		if (rem_len > 0)
			{
			strcpy(run_ptr," ");
			run_ptr += 1;
			rem_len -= 1;
			}
		}
	else
		break;

strcpy(d20_Cifields[DS_S_OCLASS], srt_entry->obj_class);

}                               /*  d20_022_fill_srt_mask()           */

