/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27shid.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:16:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:34  root]
 * 
 * Revision 1.1.8.1  1994/02/22  19:21:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:55:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:36:52  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:13:59  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:05:57  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:49:12  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:54:49  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:44:59  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:12:06  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:35:04  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27shid.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:52 $";
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
/* NAME         : d27shid.c                                           */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 16.05.88                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : This module contains functions to store and load    */
/*                dn in ipc messages.                                 */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |16.05.88 |  Birth                         | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

/*****  Operating System - Includes  *****/

/*****  external Includes  *****/

/*****  internal Includes  *****/

#include <dce/d27util.h>

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
/*								      */
/* TYPE         : C-FUNKTION                                          */
/*								      */
/* NAME         : d27_002_set_error_code()                            */
/*								      */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 21.12.87                                            */
/*								      */
/* DESCRIPTION  : The Function assigns error codes to the standard    */
/*                header.                                             */
/*								      */
/* RETURNVALUE	   : Void					      */
/*								      */
/*exoff	***************************************************************/

void d27_002_set_error_code(
    D2_pbhead *head,    /* OUT   -  function header structure to fill */
    signed16  ret_code, /* IN    -  return code to set                */
    signed16  err_class,/* IN    -  error class to set                */
    signed16  err_value)/* IN    -  error value to set                */
{                       /* d27_002_set_error_code                     */
			/* Assign Error codes to standard header      */

  head->d2_retcode  = ret_code;
  head->d2_errclass = err_class;
  head->d2_errvalue = err_value;

}                       /* d27_002_set_error_code                     */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNKTION                                          */
/*								      */
/* NAME         : d27_201_fill_msg_dn ()                              */
/*								      */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 16.05.88                                            */
/*								      */
/* DESCRIPTION  : This function stores a distinguished name at the    */
/*                given pointer in ipc message format.                */
/*								      */
/* RETURNVALUE     : char *   pointer to next place in message        */
/*                            (after stored dn)                       */
/*								      */
/*exoff	***************************************************************/

char *d27_201_fill_msg_dn(
    D2_name_string name,/* IN    -  distinguished name to write       */
			/* OUT   -  distinguished name in APDU format */
    D23_ds_name *dsname_ptr)
{                       /* d27_201_fill_msg_dn                        */
/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

  register char     *return_value;
  register char     *name_ptr;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

  dsname_ptr->d23_dstag = D23_NAMTAG;
  dsname_ptr->d23_dscomplete = TRUE;
  dsname_ptr->d23_dslen = strlen((char *)name) + 1;
  dsname_ptr->d23_dsoff = D2_ALIGN(dsname_ptr->d23_dslen);
  strcpy((name_ptr = (char *)(dsname_ptr + 1)),(char *)name);
  return_value = name_ptr + dsname_ptr->d23_dsoff;

  return (return_value);
}                       /* d27_201_fill_msg_dn                        */
