/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22cffkt.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:28  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:26:32  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/mothra_work/1  1995/07/07  23:02 UTC  dtruong
 * 	Correct uninitialize variable
 * 	[1995/12/08  14:54:15  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:43  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:44  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:50:58  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:34  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:02:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:51:34  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:21:56  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:11:55  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:12:49  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:13:25  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:05:42  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22cffkt.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:32 $";
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
/* NAME         : d22cffkt.c                                          */
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
/*  FUNCTIONS: d22_cfopen (), d22_cfclose (), d22_cfread (), d22_cfwrite ()  */
/*             d22_cflseek (), d22_cflink (), d22_cfunlink ()                */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      This Module represents the interface to the DUA-Cache files. It      */
/*      contains all functions through which the DUA-Cache files and         */
/*      directory are generated and administrated. The interface is          */
/*      introduced for giving the alternative to realize the DUA-cache       */
/*      not only on the file system, but also in main memory. To achieve     */
/*      this, only the functions contained within this module must be        */
/*      modified in an appropiate manner.                                    */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*  OUTPUT-PARAMETER:                                                        */
/*  RETURN-VALUES:                                                           */
/*      d22_cfopen ()   = same as system call "open ()".                     */
/*      d22_cfclose ()  = same as system call "close ()".                    */
/*      d22_cfread ()   = same as system call "read ()" (additional input    */
/*                        parameter 'offset' -> start of reading).           */
/*      d22_cfwrite ()  = same as system call "write ()" (additional input   */
/*                        parameter 'offset' -> start of writing - offset =  */
/*                        D22_EOF means writing at end of file).             */
/*      d22_cflseek ()  = same as system call "lseek ()".                    */
/*      d22_cflink ()   = same as system call "link ()".                     */
/*      d22_cfunlink () = same as system call "unlink ()".                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE:  9.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

int d22_cfopen (char *name, int mode, int access) {

    int result ;

    if ((result = open (name, mode, access)) == -1) {
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
	    GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_OPEN_ERR, name, mode, errno) ;
    }
    return (result) ;
}

/* ------------------------------------------------------------------------- */

int d22_cfclose (int fd) {

    int result ;

    if ((result = close (fd)) == -1) {
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CLOSE_ERR, errno) ;
    }
    return (result) ;
}

/* ------------------------------------------------------------------------- */

int d22_cfread (int fd, void *buf, size_t count, off_t offset) {

    int	result ;

    if ((result = lseek (fd, offset, D22_SEEK)) == -1) {
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
		        GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_SEEK_ERR, errno) ;
	return (result) ;
    }
    if ((result = read (fd, buf, count)) == -1) {
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
		        GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_READ_ERR, errno) ;
    }
    return (result) ;
}

/* ------------------------------------------------------------------------- */

int d22_cfwrite (int fd, void *buf, size_t count, off_t offset) {

    int	result ;
#ifndef __hpux_13317CHFts
    if ((result = lseek (fd, (offset == D22_EOF) ? 0L : offset,
		  (offset == D22_EOF) ? D22_ESEEK : D22_SEEK)) == -1) {
#else
    if (lseek (fd, (offset == D22_EOF) ? 0L : offset,
		  (offset == D22_EOF) ? D22_ESEEK : D22_SEEK) == -1) {
#endif
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
		        GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_SEEK_ERR, errno) ;
	return (result) ;
    }
    if ((result = write (fd, buf, count)) == -1) {
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_WRITE_ERR, errno) ;
    }
    return (result) ;
}

/* ------------------------------------------------------------------------- */

off_t d22_cflseek (int fd, off_t offset, int mode) {

    off_t	result ;

    if ((result = lseek (fd, offset, mode)) == -1) {
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
		        GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_SEEK_ERR, errno) ;
    }
    return (result) ;
}

/* ------------------------------------------------------------------------- */

int d22_cflink (char *name1, char *name2) {

    int	result ;

    if ((result = link (name1, name2)) == -1) {
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"),
		 GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_LINK_ERR, name1, errno) ;
    }
    return (result) ;
}

/* ------------------------------------------------------------------------- */

int d22_cfunlink (char *name) {

    int	result ;

    if ((result = unlink (name)) == -1) {
	d22_errno = errno ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"),
		GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_UNLINK_ERR, name, errno) ;
    }
    return (result) ;
}
