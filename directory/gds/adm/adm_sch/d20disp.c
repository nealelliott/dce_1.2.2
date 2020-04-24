/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20disp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:07  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:19  root]
 * 
 * Revision 1.1.8.4  1994/07/06  15:06:19  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:10:41  marrek]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:14  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:07:50  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:16  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:19  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:17:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:58:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:13:06  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:59  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:50:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:58:54  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:38:17  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:33  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20disp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:09 $";
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
/* NAME         : d20disp.o   [scheme display interface]              */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 04.07.88                                            */
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
/*      0.1 | 04.07.88| Original                       | ws |         */
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

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
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

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_6_display_scheme()                                */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        04.07.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_6_display_scheme(parblk)               */
/*								      */
/*              D20_p6  *parblk;                                      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will return the new SRT/OCT/AT entry to */
/*              be handled.                                           */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              D20_p6      *parblk;    			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              D20_p6      *parblk;   				      */ 
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 d2_ret_val d20_6_display_scheme(
   D20_p6 *parblk)
 
{                               /*  d20_6_display_scheme()            */

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

d2_ret_val   ret_value = D2_NOERROR;

/*
static char *f_name = "d20_6_display_scheme";
*/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_error(parblk->d20_6pbhead, D2_NOERROR, D2_NOERROR, D2_NOERROR);
/* d21_l03_trace_err(0, f_name, "ENTRY", parblk->d20_6pbhead); */
if                              /*  version is wrong                  */
    (parblk->d20_6pbhead->d2_version != DS_V03)
{                               /*  set error values                  */
  d20_error(parblk->d20_6pbhead, D2_CALL_ERR, D2_ILL_VERS, D2_NOT_SPEC);
  /* d21_l03_trace_err(0, f_name, "EXIT", parblk->d20_6pbhead); */
  return(D2_ERROR);
}                               

switch ((int) parblk->d20_6disp)/*  operation is                      */
  {
  case D2_SCH_FIRST:                /*  get first element         */
		    if          /*  no elements in table                */
		       (parblk->d20_6nentries == 0)
		    {           /*  set error values                  */
		      d20_error(parblk->d20_6pbhead, D2_CALL_ERR, D2_PAR_ERR,
				D2_NO_ELEM);
		      ret_value = D2_ERROR;
		    }
		    else
		    {           /*  return first element          */
		      parblk->d20_6act = parblk->d20_6first;
		    }           
		    break;
  case D2_SCH_NEXT:                /*  get next element          */
		    if          /*  no element in table */
		       (parblk->d20_6nentries == 0)
		    {           /*  set error values                  */
		      d20_error(parblk->d20_6pbhead, D2_CALL_ERR, D2_PAR_ERR,
				D2_NO_ELEM);
		      ret_value = D2_ERROR;
		    }
else if   /*  end of table reached */
		       (parblk->d20_6act == (parblk->d20_6first + 
						parblk->d20_6nentries - 1))
		    {           /*  set error values                  */
		      d20_error(parblk->d20_6pbhead, D2_CALL_ERR, D2_PAR_ERR,
				D2_E_SCH);
		      ret_value = D2_ERROR;
		    }
else
{           /*  return next SRT element           */
		      parblk->d20_6act += 1;
		    }           
		    break;
  case D2_SCH_PREVIOUS:                /*  get previous element      */
		    if          /*  no element in table */
		       (parblk->d20_6nentries == 0)
		    {           /*  set error values                 */
		      d20_error(parblk->d20_6pbhead, D2_CALL_ERR, D2_PAR_ERR,
				D2_NO_ELEM);
		      ret_value = D2_ERROR;
		    }
		    else if   /*  no previous element                */
			  (parblk->d20_6act == parblk->d20_6first)
		    {           /*  set error values                 */
		      d20_error(parblk->d20_6pbhead, D2_CALL_ERR, D2_PAR_ERR,
				D2_B_SCH);
		      ret_value = D2_ERROR;
		    }
		    else
		    {           /*  return PREVIOUS element          */
		      parblk->d20_6act -= 1; 
		    }           
		    break;
  default:                      /*  otherwise                        */
		    d20_error(parblk->d20_6pbhead, D2_CALL_ERR, D2_PAR_ERR,
			      D2_ILL_OP);
		    ret_value = D2_ERROR;
		    break;
  }                               

/* d21_l03_trace_err(0, f_name, "EXIT", parblk->d20_6pbhead); */
				/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_6_display_scheme()            */
