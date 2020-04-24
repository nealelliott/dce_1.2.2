/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21trace.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:20:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:47  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:43:53  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:09:54  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:12  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:29  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:59  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:44  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:37:37  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:56:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:40:33  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:48  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:22  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:01:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:14:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:01:12  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:45:24  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21trace.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:10 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21trace.o                                          */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 30.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <magenex.h>
#include <malgcfe.h>
#include <d2delupd.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d010_trace_job()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will trace the given parameters.                */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      Name_string dsa;        destination dsa name                  */
/*      Name_string object;     object/subtree name                   */
/*      signed32    subset;     object interpretation                 */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*exoff ***************************************************************/

void d21d010_trace_job(
  Name_string dsa,
  Name_string object,
  signed32 subset)
{
  DCE_SVC_LOG((GDS_S_SUP_SHDJOB_INFO_MSG, (char *)dsa,
    (object == NULL) ? (String) "ROOT" : (String) object,
    (subset == OBJECT) ? "OBJECT" : "OBJECT AND SUBORDINATES"));
} /* end of d21d010_trace_job */


/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d011_trace_upd_mode_and_times()                     */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will trace the given parameters.                */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed32  upd_mode;                                           */
/*      Upd_time *update_times;                                       */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*exoff ***************************************************************/

void d21d011_trace_upd_mode_and_times(
  signed32 upd_mode,
  Upd_time * update_times)
{
char    log_data[D21_MAX_LOG_DATA];

if (update_times != (Upd_time *) NULL)
{   
  switch ((int) update_times->upd_t_mode)
  { case UPD_HIGH:
	    sprintf(log_data, "min: %d", update_times->upd_min);
	    break;
    case UPD_MEDIUM:
	    sprintf(log_data, "hour: %d", update_times->upd_hour);
	    break;
    case UPD_LOW_1:
	    sprintf(log_data, "day of week (1): %d  hours (1): %d",
		    update_times->upd_1d_of_week,
		    update_times->upd_1hour);
	    break;
    case UPD_LOW_2:
	    sprintf(log_data, 
       "day of week (1): %d  hours (1): %d  day of week (2): %d  hours (2): %d",
		    update_times->upd_1d_of_week,
		    update_times->upd_1hour,
		    update_times->upd_2d_of_week,
		    update_times->upd_2hour);
	    break;
  }
}
else
{
  strcpy(log_data,"not given");
}

DCE_SVC_LOG((GDS_S_SUP_SHDUPD_INFO_MSG, upd_mode, log_data));
} /* end of d21d011_trace_upd_mode_and_times */
