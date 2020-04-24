/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil31.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:13  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:31  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:05  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:02  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:55:20  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:37  keutel
 * 	creation
 * 	[1994/03/21  13:25:03  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routil31.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:13 $" } ;
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

/*daton	***************************************************************/
/*								      */
/* TYPE		: MODULE					      */
/*								      */
/* NAME         : routil31.c                                          */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 14-JUN-1992                                         */
/*								      */
/* COMPONENT    : RTS utility functions                               */
/*								      */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*								      */
/* DESCRIPTION  : This modul provides just a dummy for the function   */
/*                  ro004d_GetLogParams() to support the interface    */
/*								      */
/* EVALUATED CONDITIONS :  RC0008_LOG_V1,RC0008_LOG_V2,RC0008_LOG_V3  */
/*								      */
/* SYSTEM DEPENDENCE :	none					      */
/*								      */
/* HISTORY      :       sccsid = %@(#)routil31.c	1.1% %94/01/26%                          */
/*								      */
/* Vers.Nr. | Date    |	 Modification		       |mark| CR# EM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*		Module Identification				      */
/**********************************************************************/

static char *sccsid =    "@(#) routil31.c 1.1 93/11/29   RTS-V2.00";

/**********************************************************************/
/*		     Module Dependencies			      */
/**********************************************************************/
/*
#if  defined RC0008_LOG_V1  &&  \
    !defined RC0008_LOG_V2  &&  \
    !defined RC0008_LOG_V3   */
		      /***** use the Interface controlled logging *****/
/**********************************************************************/
/*		      I N C L U D E S				      */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <roerval.h>      /* Include of RTS error values.             */
#include <routil0.h>      /* Include of RTS common utilites (extern)  */
#include <routil2.h>    /* Common util function defs                */

/**********************************************************************/
/*		G L O B	A L   F	U N C T	I O N S			      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME         : ro004c_GetLogParams                                 */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 14-JUN-1992                                         */
/*								      */
/* SYNTAX	:						      */

signed short int PASCAL ro004c_GetLogParams (
  unsigned short int       FAR *loglevel,
  unsigned short int       FAR *recnum,
  unsigned short int       FAR *confnum,
  char               FAR * FAR *fname)

/*								      */
/* DESCRIPTION  : This function is not supported in this module!      */
/*								      */
/* IN-PARAMETERS  :                                                   */
/*      loglevel  : dummy                                             */
/*        recnum  : dummy                                             */
/*       confnum  : dummy                                             */
/*         fname  : dummy                                             */
/*								      */
/* OUT-PARAMETERS :                                                   */
/*      loglevel  : dummy                                             */
/*        recnum  : dummy                                             */
/*       confnum  : dummy                                             */
/*         fname  : dummy                                             */
/*								      */
/* RETURN VALUES  :    R_NOTSUP                                       */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*								      */
/* REMARKS : May be used only with Logging Version 1 (RC0008_LOG_V1)  */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro004d_GetLogParams*/
/* ******************  Start of Function  *************************** */

  return (R_NOTSUP);                /* not supported in this module ! */
				  /* because logging is controlled by */
	    /* calls of openLog() + closeLog() from the RTS-Interface */

/* *******************  End of Function  **************************** */
}/*end-ro004d_GetLogParams*/

/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0040_openLog				      */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE		: 13-JUN-1992					      */
/*								      */
/* SYNTAX	:						      */

signed short int PASCAL ro0040_openLog(
  char FAR              *fname,
  unsigned short int     fmode,
  unsigned short int    recnum,
  unsigned short int  loglevel)

/*								      */
/* DESCRIPTION	:  This	function opens the logging system.            */
/*		   First it tries to read the cfg-file. If there is   */
/*		   one, the settings from the file are used to        */
/*		   update the logging system because parameters from  */
/*		   the cfg-file have a higher priority than the input */
/*		   values have. If there's no cfg-file the input      */
/*                 parameters are used to refresh logging.            */
/*								      */
/* IN-PARAMETERS  :						      */
/*								      */
/*         fname  :  Name of file to which the logging information    */
/*                   will be written.                                 */
/*	   fmode  :  Open mode of logging file.			      */
/*        recnum  :  Number of records written to one logging file.   */
/*                   (before a new logging file will be created)      */
/*	loglevel  :  Logging level.				      */
/*								      */
/* OUT-PARAMETERS :  none			    	              */
/*								      */
/* RETURN VALUES  :                                                   */
/*								      */
/*	   = 0 :  successfully					      */
/*	  <> 0 :  error						      */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro0040_openLog*/
/* ******************  Start of Function  *************************** */
	    /* This function is only available with logging version 1 */

  /* No configuration file present, so simply refresh internal logging info */

  return (ro004d_RefreshLog (loglevel, recnum, fname, fmode));

/* ******************  End of Function  ***************************** */
}/*end-ro0040_openLog*/


/*exon ****************************************************************/
/*								      */
/* TYPE		: C-FUNCTION					      */
/*								      */
/* NAME		: ro0041_closeLog				      */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 13-JUN-1992                                         */
/*								      */
/* SYNTAX	:						      */

  signed short int  PASCAL  ro0041_closeLog (void)

/*								      */
/* DESCRIPTION  :  This functions closes the logging system.          */
/*								      */
/* IN-PARAMETERS  :  none					      */
/*								      */
/* OUT-PARAMETERS :  none					      */
/*								      */
/* RETURN VALUES  :						      */
/*								      */
/*        =  0   if successfully                                      */
/*        <> 0   if ERROR                                             */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS		      :					      */
/*								      */
/*inoff	***************************************************************/

{/*entr-ro0041_closeLog*/
/* ******************  Start of Function  *************************** */
	    /* This function is only available with logging version 1 */

  /* No configuration file present, so simply refresh internal logging info */

  return (ro004d_RefreshLog (0,0,"nolog",0));        /* switch logging off */

/* *******************  End of Function  **************************** */
}/*end-ro0041_closeLog*/


/* ****************** END of MODULE ********************************* */
/* #endif */     /* RC0008_LOG_V1 */
