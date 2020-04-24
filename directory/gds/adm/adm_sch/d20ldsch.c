/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20ldsch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:41  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:31  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:04:17  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:11  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:19:39  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:11:15  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:16:08  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:07  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:52  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:54:35  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:02:15  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:40:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:39  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20ldsch.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:25 $";
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
/* NAME         : d20ldsch.c [load locally stored scheme file]        */
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
#include <stdio.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
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
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

extern D20_omask  d20_qomask[];         /* input fields of error mask */
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
/* NAME       : d20_27_load()                                         */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        23.03.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_027_load(file, nrsrt, psrt, nroct,     */
/*					poct, nrat, pat)	      */
/*              char      *file;                                      */
/*		signed32  *nrsrt;				      */
/*		Srt      **psrt;				      */
/*              signed32  *nroct;                                     */
/*              Oct      **poct;                                      */
/*              signed32  *nrat;                                      */
/*              At       **pat;                                       */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will load a locally created scheme file.*/
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              char      *file;        file name of scheme file      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32  *nsrt;	number of SRT entries         */	
/*		char     **psrt;	pointer to SRT		      */
/*              signed32  *nroct;       number of OCT entries 	      */
/*              char     **poct;        pointer to OCT		      */
/*              signed32  *nrat;        number of AT entries 	      */
/*              char     **pattr;       pointer to AT		      */
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

 d2_ret_val d20_027_load(
   char      *file,
   signed32  *nrsrt,
   Srt      **psrt,
   signed32  *nroct,
   Oct      **poct,
   signed32  *nrat,
   At       **pat)

{                               /*  d20_027_load()                    */

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
   Errcode      return_code;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if /*  loading of local scheme file is not ok	*/
   (gds_ldsh(&return_code, DS_V03, file, nrsrt, psrt, nroct, poct,
		 nrat, pat) == D2_ERROR)
{                               
				/*  display error message             */
  switch ((int) return_code.errvalue)
  {
    case DS_ALLOC_ERROR:    d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E65IDX];
			break;
    case DS_FILE_ERR:   d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E91IDX];
			break;
    case DS_NO_FILE:     d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E71IDX];
			break;
    default:            break;
  }   
  d20_handlemask (d20_qomask) ;

  ret_value = D2_ERROR;
}                               

				/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_027_load()                    */
