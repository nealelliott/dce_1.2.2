/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27util.c,v $
 * Revision 1.1.10.2  1996/02/18  18:24:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:01  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:17:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:52  root]
 * 
 * Revision 1.1.8.5  1994/10/05  15:39:26  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:38:01  keutel]
 * 
 * Revision 1.1.8.4  1994/07/06  15:08:53  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:24  marrek]
 * 
 * Revision 1.1.8.3  1994/06/21  14:48:38  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:10:10  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  16:03:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:35  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  19:22:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:54:59  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:42:33  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:15:23  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:06:19  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:49:30  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:56:09  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:46:00  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:12:39  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:36:44  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27util.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:24:10 $";
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
/* NAME         : d27util.c <Utility-Functions for DIR-Interfaces>    */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 17.12.87                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
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
/*   0.1    |17.12.87 |  Birth                         | ek |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <gds.h>

/*****  Operating System - Includes  *****/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/times.h>

/*****  external Includes  *****/

#include <dce/d2dir.h>
#include <d23apdu.h>
#include <dce/d27util.h>

/*****  internal Includes  *****/

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

			/* format string for UTC-time strings         */
#define D27_T_STRING    "%02d%02d%02d%02d%02d%02dZ"

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

int     d27_pid;
char    d27_proc_name[D27_LINST_NAME];

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_000_log_signal                                  */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 08.06.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function logs a signal arrived which isn't     */
/*                handled.                                            */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d27_000_log_signal(
    int signal)         /* IN    -  signal to be logged               */

{                       /* d27_000_log_signal                         */

  dce_svc_printf(GDS_S_SIGNAL_ERR_MSG,signal);

  exit(1);              /* program exit                               */

}                       /* d27_000_log_signal                         */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d27_010_set_error_apdu                              */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 7. 1. 88                                            */
/*								      */
/* DESCRIPTION  : This function will set the supplied error codes in  */
/*                the error message block.                            */
/*								      */
/* RETURN VALUE :                                                     */
/*      None                                                          */
/*								      */
/*exoff ***************************************************************/

void d27_010_set_error_apdu(
    D23_rserror *error, /* OUT   -  pointer to error message          */
    signed16 ret_code,  /* IN    -  return code for error message     */
    signed16 err_class, /* IN    -  error class for error message     */
    signed16 err_value, /* IN    -  error value for error message     */
    signed32 *l_err)    /* OUT   -  length of error message           */

{                       /* d27_010_set_error_apdu                     */
			/* set error codes                            */
  error->d23_Zretcod  = ret_code;
  error->d23_Zerrcls  = err_class;
  error->d23_Zerrval  = err_value;
  error->d23_Zversion = D23_V1988;

			/* put "end of message"-flag to message       */
  ((D23_eom *)(error + 1))->d23_eomtag = D23_EOMTAG;
			/* determine length of error message          */
  *l_err = sizeof(D23_rserror) + sizeof(D23_eom);

}                       /* d27_010_set_error_apdu                     */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d27_013_get_utc_time()                                */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        14.04.88                                              */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will write the UTC-time to string.      */
/*								      */
/* RETURNVALUE: None                                                  */
/*								      */
/*exoff ***************************************************************/

void d27_013_get_utc_time(
    char *string)       /* OUT   -  pointer to write UTC time to      */

{                               /*  d27_013_get_utc_time()            */

  time_t t_stamp;
  struct tm  *t;

  t_stamp = time(0L);
  t = gmtime(&t_stamp);
  sprintf(string, D27_T_STRING, t->tm_year, t->tm_mon + 1, t->tm_mday,
		  (t->tm_hour == 24) ? 0 : t->tm_hour,
		  t->tm_min, t->tm_sec);

}                                /*  d27_013_get_utc_time()            */

#ifdef THREADSAFE
/* return the thread-id (integer) of the calling thread */

unsigned32 d27_thread_id(void)
{
pthread_t me;

me = pthread_self();
return(pthread_getunique_np(&me));
}
#endif /* THREADSAFE */
