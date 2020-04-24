/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsshmatt.c,v $
 * Revision 1.1.183.2  1996/02/18  18:20:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:20  marty]
 *
 * Revision 1.1.183.1  1995/12/08  16:06:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:47  root]
 * 
 * Revision 1.1.181.6  1994/09/06  12:26:05  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:22:06  keutel]
 * 
 * Revision 1.1.181.5  1994/06/21  14:47:45  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:43:10  marrek]
 * 
 * Revision 1.1.181.4  1994/06/09  18:47:13  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:25  devsrc]
 * 
 * Revision 1.1.181.3  1994/05/10  15:57:26  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:29:55  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:43:34  marrek]
 * 
 * Revision 1.1.181.2  1994/03/23  15:26:16  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:32:57  keutel]
 * 
 * Revision 1.1.181.1  1994/02/22  16:10:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:59:08  marrek]
 * 
 * Revision 1.1.179.2  1993/10/14  17:52:19  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:42:18  keutel]
 * 
 * Revision 1.1.179.1  1993/10/13  17:33:38  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:51:09  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsshmatt.c,v $ $Revision: 1.1.183.2 $ $Date: 1996/02/18 18:20:40 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    nds_shmattach ()                                               */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function attaches the caller to the NDS-configuration data      */
/*       base. Normally the data base informations should be available in    */
/*       shared memory (to allow comfortable on line modifications of the    */
/*       data base). However, in this version the data base is represented   */
/*       by a regular file only.                                             */
/*       Thus, modifications of the data base are possible by recompiling a  */
/*       configuration source file. But the modifications will become avail- */
/*       able to all instances using the data base with some delay, because  */
/*       the data base is checked for this only in intervals of NDS_CHCK_-   */
/*       INTV. The layout of the data base is already structured in a way    */
/*       allowing it to put the data in shared memory without any change.    */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*                     None                                                  */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       NDS_NOERR   = The function has been performed successfully.         */
/*       < 0         = A system error or an internal error is occurred       */
/*                     during execution of the function (in this case the    */
/*                     negative return value specifies the error).           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <d2dir.h>
#include <d27util.h>
#include <ndsif.h>
#include <ndscfg.h>

/* ******************* DEFINITION OF CONSTANTS ***************************** */

#define NDS_CHCK_INTV	60	/* data base modification checking interval */
				/* (in seconds) */

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

				/* enable serviceability prolog in NDS-library */
signed32 svcprolog = 0 ;
				/* reference to NDS-configuration data base */
Nds_shmgenarea *nds_shm_base = (Nds_shmgenarea *) NULL ;

				/* defined here because libdce is not */
				/* linked on RIOS for shared library libNDS */

/* ------------------------------------------------------------------------- */

signed32 nds_shmattach () {

    struct stat		flstat ;
    FILE		*fd ;
    int			cnt ;
    char		tmpbuf[D27_LFILE_NAME] ;
    static time_t	last_mod = (time_t) 0 ;
    static time_t	last_check = (time_t) 0 ;
    time_t		acttime ;

    if (time (&acttime) < last_check + NDS_CHCK_INTV)
	return (NDS_NOERR) ;

    /* get state information of NDS-configuration data base file */
    sprintf (tmpbuf, "%s%s%s/%s", dcelocal_path, D27_CLIENT_DIR,
				      D27_CONF_DIR, NDS_CONF_DIB) ;
    if (stat (tmpbuf, &flstat) < 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_GENERAL,
			      svc_c_sev_fatal, GDS_S_FOPEN_ERR, tmpbuf, errno) ;
	return (NDS_DESCSIZE_ERR) ;
    }

    /* check modification time of NDS-configuration data base file */
    if (flstat.st_mtime == last_mod) {
	/* no modification detected */
	last_check = acttime ;
	return (NDS_NOERR) ;
    }

    /* modification detected -> get new data base information */
    if (nds_shm_base != (Nds_shmgenarea *) NULL)
	/* release old data base information */
	free (nds_shm_base) ;

    if ((nds_shm_base = (Nds_shmgenarea *) malloc ((size_t) flstat.st_size)) ==
						      (Nds_shmgenarea *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			      svc_c_sev_fatal, GDS_S_NOMEMORY, flstat.st_size) ;
	return (NDS_DESCMEM_ERR) ;
    }

    if ((fd = fopen (tmpbuf, "r")) == (FILE *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%s%d"), GDS_S_GENERAL,
			 svc_c_sev_fatal, GDS_S_FOPEN_ERR, tmpbuf, "r", errno) ;
	return (NDS_DESCFOPEN_ERR) ;
    }
    if ((cnt = fread (nds_shm_base, (size_t) flstat.st_size, 1, fd)) != 1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				      svc_c_sev_fatal, GDS_S_FREAD_ERR, errno) ;
	fclose (fd) ;
	return (NDS_DESCFREAD_ERR) ;
    }
    fclose (fd) ;

    if (nds_shm_base->nds_versno != NDS_SHMVERS) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
					svc_c_sev_fatal, GDS_S_NDS_VERS_ERR,
					nds_shm_base->nds_versno, NDS_SHMVERS) ;
	return (NDS_DESCVERS_ERR) ;
    }

    last_mod = flstat.st_mtime ;
    last_check = acttime ;
    return (NDS_NOERR) ;
}
