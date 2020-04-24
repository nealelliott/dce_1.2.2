/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27close.c,v $
 * Revision 1.1.15.2  1996/02/18  18:23:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:38  marty]
 *
 * Revision 1.1.15.1  1995/12/08  16:16:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:19  root]
 * 
 * Revision 1.1.13.2  1994/07/06  15:08:50  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:19  marrek]
 * 
 * Revision 1.1.13.1  1994/02/22  17:55:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:53:33  marrek]
 * 
 * Revision 1.1.11.2  1993/08/11  14:28:16  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:12:07  marrek]
 * 
 * Revision 1.1.7.3  1992/12/31  22:05:07  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:41  bbelch]
 * 
 * Revision 1.1.7.2  1992/10/13  17:23:29  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:06:37  marty]
 * 
 * Revision 1.1.5.2  1992/09/29  22:45:45  marty
 * 	OSC1.1 upgrade
 * 	[1992/09/29  22:38:08  marty]
 * 
 * Revision 1.1.3.2  1992/09/29  20:03:07  garyf
 * 	use sysconf() for max open files
 * 
 * Revision 1.1.2.2  1992/06/01  23:11:00  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:34:16  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27close.c,v $ $Revision: 1.1.15.2 $ $Date: 1996/02/18 18:23:39 $";
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
/* NAME         : d27close.c <Utility-Functions for DIR-Interfaces>   */
/*                                                                    */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 24.6.88                                             */
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
/*   0.1    |24.6.88  |  Birth                         | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#ifndef __hpux
#include <gds.h>
#endif

/*****  Operating System - Includes  *****/

#include <unistd.h>

/*****  external Includes  *****/

#include <dce/d27util.h>

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
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d27_011_close_unused_files()                        */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 24. 6. 88                                           */
/*								      */
/* DESCRIPTION  : This function will close all files that were        */
/*                herited when processes were forked.                 */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                close_all             indicates whether stdin,      */
/*                                      stdout can be close or not    */
/*								      */
/* OUTPUT PARAMETERS:                                                 */
/*								      */
/* RETURN VALUE :   -                                                 */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*exoff ***************************************************************/

void d27_011_close_unused_files(
    boolean close_all)  /* IN    -  indicates whether stdin and stdout*/
			/*          should be closed too              */

{                                   /*  d27_011_close_unused_files()  */
/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

  int    fd;
  int    open_max;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

#ifdef NONPOSIX
  open_max = NOFILE;
#else
  open_max = sysconf(_SC_OPEN_MAX);
#endif /* NONPOSIX */

  for (fd = 3; fd < open_max; fd++)
  {
    close(fd);
  }

  if (close_all == TRUE)
  {
    close(0);
    close(1);
  }

}                                   /*  d27_011_close_unused_files()  */
