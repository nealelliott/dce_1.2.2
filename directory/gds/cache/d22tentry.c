/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22tentry.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:02  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:28:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:35  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:07  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:13  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:35  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:39  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:55:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:48:00  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:15:55  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:42:02  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:14:19  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:31:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:38:23  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22tentry.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:00 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d22tentry.c                                         */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d22_trace_entry ()                                            */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function writes information about a service request (this means  */
/*      request resp. result parameter) into the DUA-cache specific trace    */
/*      file. The range of the trace information depends on the actual trace */
/*      level (short trace level or full trace level). In the first case     */
/*      only the type of the request and the time of its occurence resp. the */
/*      type of result (or error) are traced. In the second case all request */
/*      and result parameter are traced.                                     */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      admptr      = Pointer to the actual administration table.            */
/*      trlevel     = Actual valid trace level.                              */
/*      pbrequest   = Pointer to a IPC-request parameter block containing    */
/*                    references to request parameter which should be traced.*/
/*      pbresult    = Pointer to a IPC-result parameter block containing     */
/*                    references to result parameter or to error parameter   */
/*                    which should be traced.                                */
/*                    Note: Only one of the two pointers can be set at a     */
/*                    time, the other must be a NIL-Pointer.                 */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 23.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>


/* -------------------- declaration of global data ------------------------- */

extern char	d22_locmsg[] ;	/* locality message */

/* -------------------- declaration of local data -------------------------- */

static FILE	*fd = (FILE *) NULL ;	/* File descriptor of trace file */
static signed16	d22_rqmsgno = 0 ;	/* actual no. of service request message */
static signed16	d22_rsmsgno = 0 ;	/* actual no. of the service result message */

/* ------------------------------------------------------------------------- */

void d22_trace_entry (D22_admin_tab *admptr, signed16 trlevel,
				  D23_wtinpb *pbrequest, D23_rsrqpb *pbresult) {

    char	fname[D27_LFILE_NAME], lname[D27_LFILE_NAME] ;
    off_t	tfilesize ;
    time_t	acttime ;

    if (fd == (FILE *) NULL) {
	/* open trace file */
	sprintf (fname, "%s%s", admptr->d22_conftab->d22_base_path,
								D22_TRACEFILE) ;
	if ((fd = fopen (fname, "a")) == NULL)
	    d22_errorhdl (D22_FATAL, 0, D22_1FATAL_EMSG) ;
    }

    /* verify size of trace file */
    if ((tfilesize = ftell (fd)) >= (long) (admptr->d22_conftab->d22_trace_max *
							       D22_TBASESIZE)) {
	/* max. trace file size exceeded - move content into a temporary file */
	fclose (fd) ;
	sprintf (fname, "%s%s", admptr->d22_conftab->d22_base_path,
								D22_TRACEFILE) ;
	sprintf (lname, "%s%s.SAV", admptr->d22_conftab->d22_base_path,
								D22_TRACEFILE) ;
	unlink (lname) ;
	link (fname, lname) ;
	unlink (fname) ;
	if ((fd = fopen (fname, "w")) == NULL)
	    d22_errorhdl (D22_FATAL, 0, D22_1FATAL_EMSG) ;
    }

    /* do trace entry */
    if (pbrequest != (D23_wtinpb *) NULL) {
	if (d22_rqmsgno == 0) {
	    /* write service request header into trace file if the */
	    /* received message is the first message of a service request */
	    acttime = time ((time_t) 0) ;
	    fprintf (fd, D22_1TRMSG) ;
	    fprintf (fd, D22_2TRMSG, pbrequest->d23_2oprid,
			  pbrequest->d23_2invinfo.d23_usrid, ctime (&acttime)) ;
	}
	if (pbrequest->d23_2imdat == D23_MRDATA)
	    fprintf (fd, D22_3TRMSG, ++d22_rqmsgno) ;
	else
	    d22_rqmsgno = 0 ;
	if (trlevel == D22_FUTRACE) {
	    /* full trace level -> write content of the request message */
	    /* into trace file */
	    fprintf (fd, D22_4TRMSG) ;
	    d27_printmsg (fd, pbrequest->d23_2rfidt, pbrequest->d23_2lnidt) ;
	}
    }
    if (pbresult != (D23_rsrqpb *) NULL) {
	if (pbresult->d23_3rmdat == D23_MRDATA || d22_rsmsgno > 0)
	    fprintf (fd, D22_5TRMSG, ++d22_rsmsgno) ;
	if (pbresult->d23_3rmdat == D23_LASTFG) {
	    d22_rsmsgno = 0 ;
	    if (pbresult->d23_3rltid == D23_RERROR)
		fprintf (fd, D22_6TRMSG,
			((D23_rserror *) pbresult->d23_3rfrdt)->d23_Zretcod,
			((D23_rserror *) pbresult->d23_3rfrdt)->d23_Zerrcls,
			((D23_rserror *) pbresult->d23_3rfrdt)->d23_Zerrval,
				 				   d22_locmsg) ;
	    else
		fprintf (fd, D22_7TRMSG) ;
	}
	if (trlevel == D22_FUTRACE) {
	    /* full trace level -> write content of the result message */
	    /* into trace file */
	    fprintf (fd, D22_8TRMSG) ;
	    d27_printmsg (fd, pbresult->d23_3rfrdt, pbresult->d23_3lnrdt) ;
	}
    }
    fflush (fd) ;
}
