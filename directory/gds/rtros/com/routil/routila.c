/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routila.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:23  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:45  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:52  keutel
 * 	OT 11620
 * 	[1994/08/19  13:48:44  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:24  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:14  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:10  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:52  keutel
 * 	creation
 * 	[1994/03/21  13:25:44  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routila.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:23 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1990                     */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : routil2.c                                           */
/*                                                                    */
/* AUTHOR       : Roland Widmann                                      */
/* DATE         : 91-06-21                                            */
/*                                                                    */
/* COMPONENT    : ROS utility functions                               */
/*                                                                    */
/* PRD#/VERS.   : ROS-V2.50                                           */
/*                                                                    */
/* DESCRIPTION  : This modul provides functions which                 */
/*                                                                    */
/*   - SwapIntegal                                                    */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* 2.50 K0  | 91-04-01|  original                      |wdm |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#)routil2.c   2.50 91/06/21";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include "rogenrl.h"
#include <roendeco.h>


long SwapIntegrals PROTO ((
	int  size,
	long Val));
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : SwapIntegrals  (should be with unique prefix!!!)    */
/*                                                                    */
/* AUTHOR       : Christian Schoenherr                                */
/* DATE         : 91-06-21                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

long SwapIntegrals (
	int  size,
	long Val)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*   Changes the sequence of the bytes of an integer value.           */
/*   Called from ENCODE/DECODE macros (roendeco.h).                   */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

static char Dest[sizeof(long)];
char FAR *Src;
int j;


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

    if (size < 2) return Val;
    for (Src=(char FAR *)&Val,j=size; j--; ) Dest[j] = *Src++;
    return *(long *)Dest;
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : BytesToUnsignedshort                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K                                      */
/* DATE         : 93-09-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned short int  BytesToUnsignedshort (
	unsigned char  FAR * cpPointer,
	signed short  int    sLen)
/*                                                                    */
/* DESCRIPTION  : This DECODES the short value from the pointer &     */
/*                                                                    */
/*inoff ***************************************************************/
/*entr */
{
/* ****************** l o c a l    v a r i a b l e s **************** */
 signed short int  sVal= NULL;
 unsigned     char c   = NULL;
/* ****************************************************************** */
 while(sLen-- > 0 ) { 
   c = (unsigned char ) *cpPointer++;
   sVal = ( sVal * 256 ) + c ;
 }
 return sVal;                           

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : Pts_UnsignedIntToBytes                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K                                      */
/* DATE         : 93-09-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int  Pts_UnsignedIntToBytes   (
	unsigned      char FAR * cpPointer,
	signed short  int        sLen,
	unsigned short  int      sVal)     /* K02 */
/*                                                                    */
/* DESCRIPTION  : This RENCODE the short value from the pointer &     */
/*                                                                    */
/*inoff ***************************************************************/
/*entr */
{
/* ****************** l o c a l    v a r i a b l e s **************** */
 unsigned       char c   = NULL;
/* ****************************************************************** */
 while(sLen-- > 0 ) { 
   c = sVal % 256;
   sVal = sVal/256;
   *cpPointer ++ = (unsigned char) c;
 }
 return OK;                           

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : Pts_BytesToUnsignedint                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K                                      */
/* DATE         : 93-09-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned short int  Pts_BytesToUnsignedint(  /* K02 return sig to ub */
	unsigned      char FAR * cpPointer,
	signed short  int        sLen)

/*                                                                    */
/* DESCRIPTION  : This RENCODE the short value from the pointer &     */
/*                                                                    */
/*inoff ***************************************************************/
/*entr */
{
/* ****************** l o c a l    v a r i a b l e s **************** */
 unsigned       char c   = NULL;
 unsigned short  int    sVal =NULL; /* K02 */
/* ****************************************************************** */
 cpPointer = cpPointer + sLen -1 ;
 while(sLen-- > 0 ) { 
   c = (unsigned char) *cpPointer--;
   sVal = (sVal*256  ) + c ;
 }
 return sVal;                           

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : UnsignedIntToBytes                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K                                      */
/* DATE         : 93-09-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int  UnsignedIntToBytes   (
	unsigned     char  FAR * cpPointer,
	signed short  int        sLen,
	signed short  int        sVal)
/*                                                                    */
/* DESCRIPTION  : This RENCODE the short value from the pointer &     */
/*                                                                    */
/*inoff ***************************************************************/
/*entr */
{
/* ****************** l o c a l    v a r i a b l e s **************** */
 unsigned       char c   = NULL;
/* ****************************************************************** */
 cpPointer = cpPointer + sLen -1 ;
 while(sLen-- > 0 ) { 
   c = sVal % 256;
   sVal = sVal/256;
   *cpPointer-- = (unsigned char) c;
 }
 return OK;                           

}/*end*/


/*** EOF ***/


