/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21checkp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:14  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:50  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:06:45  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:23:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:35:14  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:21:36  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:48:12  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:27  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:20:03  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:34:09  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:47:47  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:12:44  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21checkp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:51 $";
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
/* NAME         : d21checkp.o                                         */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 16.01.89                                            */
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
/*      0.1 | 16.01.89| Erstellung                     | hv |         */
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
#include <gdsext.h>
#include <dce/d27util.h>
#include <d21.h>
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

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21t009_param_check()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function checks version and whether a bind was done.     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*        signed16  version;                                          */
/*        signed16  bind_id;                                          */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*        Errcode  *return_code;                                      */
/*        signed16 *dir_id;                                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/

 d2_ret_val d21t009_param_check(
  signed16  bind_id,
  Errcode  *return_code,
  signed16  version,
  signed16 *dir_id)
{
struct d21_bind_info b_tab_ent;          /* bind table entry     */
D2_pbhead	pbhead;
Bool		dummy;

return_code->retcode  = return_code->errclass = NOERR;
return_code->errvalue = NOERROR;

if (version != DS_V021)
{   return_code->retcode  = CALLERR;
    return_code->errclass = ILLVERS;
    return_code->errvalue = DS_NOT_SPEC;
    return(D2_ERROR);
}

if (d21_bh8_get_bind_info(bind_id, &b_tab_ent,&dummy,&pbhead,FALSE) == D2_ERROR)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_NO_BIND;
    return(D2_ERROR);
}

if (dir_id != 0)
*dir_id = b_tab_ent.d21_b_dir_id;

return(D2_NOERROR);
}
