/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil5.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:17  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:37  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:12  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:06  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:51  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:55:44  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:42  keutel
 * 	creation
 * 	[1994/03/21  13:25:23  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routil5.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:17 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1992 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : routil5.c                                           */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : FEB-6th-1992                                        */
/*                                                                    */
/* COMPONENT    : ROS utility functions                               */
/*                                                                    */
/* PRD#/VERS.   : ROS-V2.50                                           */
/*                                                                    */
/* DESCRIPTION  : This modul provides functions which                 */
/*                                                                    */
/*   - handle memory management                                       */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* 2.50 K0  | 91-04-01|  original                      |wdm |         */
/* 2.51 K0  | 91-10-24|  modified for SINIX            | gj |         */
/* 2.52 K0  | 92-01-14|  Windows-parts cleared         | gj |         */
/* 2.52 K0  | 92-02-06|  New In-para's for reallocs    | gj |         */
/* 2.53 K0  | 92-03-09|  Switch for Malloc-Checking    | gj |         */
/*datoff **************************************************************/

/**********************************************************************/
/*              Module Identification                                 */
/**********************************************************************/

static char *sccsid =    "@(#) routil5.c 1.1 93/11/29   RTS-V2.00";

#ifndef RC0007_CHECKMALLOC
			  /*This module is the default-malloc module */

/*********************************************************************/
/*              I N C L U D E S                                      */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdlib.h>                 /* std-memory defines+prototypes */

#include <rogenrl.h>                       /* common ROS definitions */
#include <routil0.h>     /* Include of RTS common utilites (extern)  */

/*********************************************************************/
/*              D E F I N E S                                        */
/*********************************************************************/


/*********************************************************************/
/*              G L O B A L   V A R I A B L E S                      */
/*********************************************************************/

/****************************************************************************\
* 									     *
* 	      P R O V I D E D   G L O B A L   F U N C T I O N S              *
* 									     *
*   unsigned long int PASCAL ro0020_globalalloc (unsigned long int size);    *
*   char FAR *        PASCAL ro0021_globallock  (unsigned long int hMem);    *
*   unsigned long int PASCAL ro0022_globalunlock(unsigned long int hMem);    *
*   unsigned long int PASCAL ro0023_globalrealloc (unsigned long int hMem,   *
*                                                unsigned long int oldsize,  *
*                                                unsigned long int newsize); *
*   unsigned long int PASCAL ro0024_globalfree  (unsigned long int hMem);    *
*   unsigned long int PASCAL ro0030_localalloc  (unsigned long int size);    *
*   char NEAR *       PASCAL ro0031_locallock   (unsigned long int hMem);    *
*   unsigned long int PASCAL ro0032_localunlock (unsigned long int hMem);    *
*   unsigned long int PASCAL ro0033_localrealloc  (unsigned long int hMem,   *
*                                                unsigned long int oldsize,  *
*                                                unsigned long int newsize); *
*   unsigned long int PASCAL ro0034_localfree   (unsigned long int hMem);    *
* 									     *
\****************************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0020_globalalloc                                  */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : 91-10-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0020_globalalloc (
  unsigned long int size)

/*                                                                    */
/* DESCRIPTION    :  Allocates memory area of size <size>.            */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      size      :  Size of memory to be allocated (>0).             */
/*                                                                    */
/* OUT-PARAMETERS :              none                                 */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0  :  Handle of the memory area.                           */
/*         0  :  if an error occurs.                                  */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS:   none                                                    */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
  char FAR *ptr;
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (size == 0) size = 1; /**skh: Required in some exceptional circumstances */

  if (size < 1)                    /* a size of this is not supported */
      return (0);
  ptr = (char FAR *) malloc (size);                  /* allocate area */
  if (ptr == NULL)
      return ( (unsigned long int) 0 );
  ro0043_memset ( ptr, 0, size );           /* fill area with 0-bytes */
			  /* 'calloc' produced some terrible errors!! */
  return ( (unsigned long int) ptr );

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0021_globallock                                   */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

char FAR * PASCAL ro0021_globallock ( 
  unsigned long int hMem)

/*                                                                    */
/* DESCRIPTION  :  Lock of a memory area                              */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      hMem      :  Handle of the memory area                        */
/*                                                                    */
/* OUT-PARAMETERS :    none                                           */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0   :  Pointer to the allocated memory area.               */
/*        NULL :  if an error occurs.                                 */
/*                                                                    */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS :  none                                                    */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (!hMem)
      return ( (char FAR *) NULL );
  else
      return ( (char FAR *) hMem);

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0022_globalunlock                                 */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0022_globalunlock (
  unsigned long int  hMem)

/*                                                                    */
/* DESCRIPTION  :  Unlocks the memory block                           */
/*                                                                    */
/* IN-PARAMETERS  :  Handle to the memory block                       */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*         0  :  if successful                                        */
/*       <>0  :  error  (invalid memory-handle)                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS :   none                                                   */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (!hMem)
      return ( (unsigned long int) 1 );             /* invalid handle */
  else
      return ( (unsigned long int) 0 );     /* not supported in SINIX */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0023_globalrealloc                                */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0023_globalrealloc ( 
  unsigned long int hMem,
  unsigned long int oldsize,
  unsigned long int newsize)

/*                                                                    */
/* DESCRIPTION  :  Reallocates the memory block by increasing         */
/*                 or decreasing its size. If the <newsize> is larger */
/*                 than the <oldsize> was, the extended area will be  */
/*                 filled with zero-bytes.                            */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*   oldsize      :  Old size of the memory block (>0).               */
/*   newsize      :  New size of the memory block (>0).               */
/*      hMem      :  Handle of already allocated memory block.        */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        >0  :  Handle of the memory block.                          */
/*         0  :  if an error occurs.                                  */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :                                       */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
  char FAR* nptr;
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (!hMem || oldsize == 0L || newsize ==0L)/* check input-parameters */
      return( (unsigned long int) 0 );               /* faulty para's */
  else
     {
      nptr = realloc( (char FAR*) hMem, newsize);
      if (newsize > oldsize)
	 {
	  ro0043_memset( nptr+oldsize, 0, newsize-oldsize );
	 }
     }
  return ( (unsigned long int) nptr);

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0024_globalfree                                   */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0024_globalfree (
  unsigned long int hMem)

/*                                                                    */
/* DESCRIPTION    :  Frees the allocated memory block                 */
/*                   connected to <hMem>.                             */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      hMem      :  Handle to the memory block to be free            */
/*                                                                    */
/* OUT-PARAMETERS :              none                                 */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*         0  : sucessful                                             */
/*       <>0  : if error  (invalid memory-handle)                     */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS : none                                                     */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (!hMem)                              /* check for invalid handle */
      return ( (unsigned long int) 1 );
  else
     {
      free ((char FAR *) hMem);
      return ( (unsigned long int) 0 );
     }

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0030_localalloc                                   */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0030_localalloc (
  unsigned long int size)

/*                                                                    */
/* DESCRIPTION    :  Allocates memory of size <size> (>0).            */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      size      :  Size of memory to be allocated.                  */
/*                                                                    */
/* OUT-PARAMETERS :              none                                 */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*     >0  :  Handle of the memory area                               */
/*      0  :  if an error occurs.                                     */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS :  internally the pointers are NEAR.                       */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
  char NEAR * ptr;
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (size < 1)                                    /* not a useful size */
      return ( (unsigned long int) 0 );
  else
     {
      ptr = (char NEAR *) malloc (size);  /* calloc caused some error!! */
      if (ptr == NULL)
	  return ( (unsigned long int) 0 );
      ro0043_memset ( ptr, 0, size );         /* fill area with 0-bytes */
      return ( (unsigned long int) ptr );
     }

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0031_locallock                                    */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

char NEAR * PASCAL ro0031_locallock (
  unsigned long int hMem)

/*                                                                    */
/* DESCRIPTION  :  Lock of a memory area                              */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      hMem      :  Handle of the memory area                        */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*         >0  :  Pointer to the allocated memory area.               */
/*        NULL :  if an error occurred.                               */
/*                                                                    */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS :  none                                                    */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (!hMem)
      return ( (char NEAR *) NULL);
  else
      return ( (char NEAR *) hMem);

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0032_localunlock                                  */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0032_localunlock ( 
  unsigned long int hMem)

/*                                                                    */
/* DESCRIPTION  :  Unlocks the memory block                           */
/*                                                                    */
/* IN-PARAMETERS  : Handle to the memory                              */
/*                                                                    */
/* OUT-PARAMETERS : none                                              */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*          0   = successful                                          */
/*        <>0   = if error  (invalid memory handle)                   */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :                                       */
/*                                                                    */
/*inoff  ***************************************************************/

{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (!hMem)
      return ((unsigned long int) 1);               /* invalid handle */
  else
      return ((unsigned long int) 0);           /* no action in SINIX */

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0033_localrealloc                                 */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0033_localrealloc ( 
  unsigned long int hMem,
  unsigned long int oldsize,
  unsigned long int newsize)

/*                                                                    */
/* DESCRIPTION  :  Reallocates the memory block by increasing         */
/*                 or decreasing its size and fills extra area with 0.*/
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*   oldsize      :  New size of the memory block (>0).               */
/*   newsize      :  New size of the memory block (>0).               */
/*      hMem      :  Handle of the memory block .                     */
/*                                                                    */
/* OUT-PARAMETERS :              none                                 */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*       >0  :  Handle of the memory block                            */
/*        0  :  if an error occurs.                                   */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :                                       */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
  char NEAR* nptr;
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (!hMem || oldsize == 0L || newsize == 0L)  /* check the para's */
      return( (unsigned long int) 0);
  else
     {
      nptr = realloc( (char NEAR*) hMem, newsize);
      if (newsize > oldsize)
	 {
	  ro0043_memset( nptr+oldsize, 0, newsize-oldsize );
	 }
     }
  return ( (unsigned long int) nptr);

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0034_localfree                                    */
/*                                                                    */
/* AUTHOR       : Roland Widmann, Gerhard Jahn                        */
/* DATE         : OCT-24th-1991                                       */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0034_localfree ( 
  unsigned long int hMem)

/*                                                                    */
/* DESCRIPTION    :                                                   */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      hMem      :  Handle to the memory block to be free            */
/*                                                                    */
/* OUT-PARAMETERS :              none                                 */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*          0  :  if successful.                                      */
/*        <>0  :  if error occurred.                                  */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :                                       */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (!hMem)
      return ( (unsigned long int) 1 );             /* invalid handle */
  else
     {
      free ((char NEAR *) hMem);
      return ( (unsigned long int) 0 );
     }

/* **************** E N D   O F   F U N C T I O N ******************* */
}/*end*/

#endif /*RC0007_CHECKMALLOC*/

/* ****************************************************************** */
/* ****************  E N D   O F   M O D U L E    ******************* */
/* ****************************************************************** */


