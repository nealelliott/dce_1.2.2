/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26common.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:00  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:43:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:19  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:45:12  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:52:10  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:19:44  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:16  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:09:06  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:40:44  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:02:25  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:59:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:25  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:45:50  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:53:25  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26common.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:51 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d26common.c <Common Utilities for DSA-Processes>    */
/*                                                                    */
/* AUTHOR       : Kraemer D AP 11                                     */
/* DATE         : 05.12.90                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |05.12.90 |  Original                      | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating System - Includes  *****/

#include <string.h>

/*****  external Includes  *****/

/*****  internal Includes  *****/

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:          C FUNCTION                                          */
/*                                                                    */
/* NAME:        : d26_u02_fill_field                                  */
/*                                                                    */
/* AUTHOR:        Schreck, D AP 11                                    */
/* DATE:          26.02.88                                            */
/*                                                                    */
/* DESCRIPTION:  This function fills "field" with the value given in  */
/*               the parameter "value". Is "value" shorter than       */
/*               the value given in "ln_field" the rest of "field"    */
/*               is filled with the character given in                */
/*               "fill_character". Is "value" longer than             */
/*               "ln_field" the value is shortened to "ln_field"s     */
/*               characters.                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   signed16        no error occurred                 */
/*      D26_CHANGED                 the value was shortened           */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u02_fill_field(
			/* IN    -  character to fill up the field    */
    char     fill_character,
    signed16 ln_value,  /* IN    -  length of value to copy to field  */
    char     *value,    /* IN    -  value to copy to field            */
    signed16 ln_field,  /* IN    -  length of field to fill           */
    char     *field)    /* OUT   -  field to fill                     */

{                                   /* d26_u02_fill_field             */

  signed16 return_value;

  if                    /* contents is longer than field              */
    (ln_value > ln_field)
  {                     /* value is shortened                         */
    memcpy(field,value,ln_field);
    return_value = D26_CHANGED;
  }
  else
  {                     /* field is filled with fill_characters       */
    memcpy(field,value,ln_value);
    memset(field + ln_value,fill_character,ln_field - ln_value);
    return_value = D2_NO_ERR;
  }

  return(return_value);
}
