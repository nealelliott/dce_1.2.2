/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spstep1.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:52  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:03  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:58  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:32  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:53:49  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:05  keutel
 * 	creation
 * 	[1994/03/21  13:23:32  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spstep1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:52 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * Copyright(C)                                                    */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
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


/*daton ************************************************************* */
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : spstep1.c                                           */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 02.03.92                                            */
/*                                                                    */
/* COMPONENT    : RTS Evaluation Control Program                      */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : This file provides a module which can be linked     */
/*                to the step program for logging ROS PDUs            */
/*                                                                    */
/*                                                                    */
/*datoff **************************************************************/

/* **** bu kc on 29/4/93 changed rs0001_* of recHead to the new names 
        as per in routil6.h                   *************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spstep1.c 1.1 93/11/29   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

/* common RTS-includes ************************************************/
#include <rogenrl.h>
#include <rolgval.h>

/* special RTS-includes for LOGGING ***********************************/
#include <roloif9.h>
#include <routil0.h>
#include <routil6.h>

/* special RTS-includes for STEP **************************************/
#include <spstep0.h>
#include <spstep1.h>



/**********************************************************************/
/*                                                                    */
/*            G L O B A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : usr_ros()                                           */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 24/01/92                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

int usr_ros (
    unsigned char FAR *rcrd,
    unsigned long	len)

/*                                                                    */
/* DESCRIPTION  : This is the function exported to the STEP program   */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*    rcdc    : pointer to the trace record                           */
/*    len     : length of record                                      */
/*                                                                    */
/* OUT-PARAMETERS:                                                    */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

signed   short int        rcc;
unsigned short int        i;
signed   short int        l;
signed   long  int        isPacked;
unsigned char    FAR     *ptr;
rT0060_LogRecHead	  hdr;
unsigned long int         p[16];

unsigned short int	  argn;
unsigned short int	  nerr;

static unsigned long int  argl[16];
static char FAR*	  argv[16];

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

if ((len < sizeof (rT0060_LogRecHead)) || (rcrd ==0)) {
    return (-1);
}

ro0041_memcpy ((char FAR *) &hdr, (char FAR *)rcrd, sizeof (rT0060_LogRecHead));
ptr  = rcrd + sizeof (rT0060_LogRecHead);
nerr = 0;

/* ******************  Compute number of record parts     *********** */
/* ******************  and move input data for aligmnent  *********** */

if (hdr.lLength > 0) {
       isPacked = (hdr.lRecordType) & RLro01_PackedRecord;

       if (isPacked == RLro01_PackedRecord) {
	      argn =	0;
	      i    = *ptr;  /* number of written parameters */
	      ptr++;
	      if (i > 16) i = 16;

	      while (i > 0) {
		   ro0041_memcpy ((char FAR *) &l,(char FAR *)ptr,sizeof(signed short int));
		   ptr	      += sizeof(signed short int);
		   argl[argn]  = l;

		   if (argl[argn] != RLro20_FIELDERR && argl[argn] != 0) {
			  p   [argn] = ro0020_globalalloc (argl[argn]);
			  argv[argn] = ro0021_globallock  (p[argn]);

			  ro0041_memcpy (argv[argn], (char FAR *)ptr, argl[argn]);
			  ptr	    += argl[argn];
		   } else {		   /* no alloc, if length=0 or 0xffff */
			   argv[argn] = 0;
		   }
		   i--;
		   argn++;
	      }

	      for (i = 0; i < argn; i++)	 {
		 if (argl[i] == (unsigned long int)RLro20_FIELDERR) {
			nerr++;
			argl[i] = 0;
		  }
	      }
       } else {
	      argn    = 1;
	      argl[0] = hdr.lLength;
	      p[0] = ro0020_globalalloc (argl[0]);
	      argv[0] = ro0021_globallock  (p[0]);
	      ro0041_memcpy (argv[0], (char FAR *)ptr, argl[0]);
       }
} else {
       argn    = 0;
       argv[0] = 0;
       argl[0] = 0L;
}

/* ******************		************************************* */
/* ******************           ************************************* */

if  (hdr.lRecordType < 0) {
      rcc = sp1220_writeLogRec(hdr.lRecordType               ,
			       hdr.lWriteTime                ,
			       argn		             ,
			       argv		             ,
			       (unsigned long int FAR *)argl ,
			       nerr		             );
} else {
      rcc = sp1225_writeUserRec(hdr.lRecordType              ,
				hdr.lWriteTime               ,
				argn		             ,
				argv		             ,
				(unsigned long int FAR *)argl,
				nerr		             );
}

/* ******************           ************************************* */
/* ******************           ************************************* */

i = 0;
while (argn > 0)  {
      if (argv[i] != 0 || argl[i] != 0) {
	  ro0022_globalunlock(p[i]);
	  ro0024_globalfree(p[i]);
      }
      i++; argn--;
}

return(rcc);
}/*end*/
