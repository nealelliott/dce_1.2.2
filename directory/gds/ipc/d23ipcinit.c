/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23ipcinit.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:28  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:07  root]
 * 
 * Revision 1.1.8.6  1994/09/06  12:25:26  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:32  keutel]
 * 
 * Revision 1.1.8.5  1994/07/06  15:07:39  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:52  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:47:26  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  12:59:33  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:56:30  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  06:41:48  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:34  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:17  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:54  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:47  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:40:38  marrek
 * 	Replace DCESHARED_PATH by DCELOCAL_PATH
 * 	[1994/02/10  10:53:10  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:52:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:11:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:26:24  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  20:45:16  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:17  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  23:13:30  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:08:50  tom]
 * 
 * Revision 1.1.4.3  1992/11/27  17:32:07  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  14:06:30  marrek]
 * 
 * Revision 1.1.4.2  1992/10/12  21:06:52  sommerfeld
 * 	Change _HPUX_SOURCE to __hpux in #ifdef
 * 	[1992/10/05  16:13:33  sommerfeld]
 * 
 * 	Flush ftok prototype on hpux, as it conflicts with system header file.
 * 	[1992/09/30  21:33:07  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/06/01  20:54:37  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:54  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23ipcinit.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:41 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    program                                                        */
/*                                                                           */
/*   NAME:    d23ipcinit                                                     */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The program provides the IPC-resources which are required within a  */
/*       directory system installation to allow communication between the    */
/*       different directory system components (i.e. DUA, C-Stub, S-Stub,    */
/*       DSA, CACHE, etc.). There are the IPC-mechanisms shared memory,      */
/*       semaphores and message queue which are used for this purpose.       */
/*       The size of the shared memory area can be configured by using       */
/*       several calling parameters (switches) which describe the require-   */
/*       ments of the directory system installation (e.g. max. no. of server */
/*       processes or max. size of the communication buffer).                */
/*       Note: Default values are used if the program is started without     */
/*       any switch (all switches are optional).                             */
/*                                                                           */
/*       The program can also be used to remove the provided IPC-            */
/*       resources from the system. In this case the switch '-d' must be     */
/*       given on startup.                                                   */
/*                                                                           */
/*   CALLING PARAMETER:                                                      */
/*       -d<nnn>   = Max. no. of distributed command entries -->             */
/*                   <nnn> must be within the range ( 1 <= <nnn> <=          */
/*                   D23_MXDISTCMD)                                          */
/*       -r<nnn>   = Max. no. of available registration entries -->          */
/*                   <nnn> must be within the range ( 1 <= <nnn> <=          */
/*                   D23_MXREGINFO)                                          */
/*       -l<nnn>   = Size of the communication buffer (in kB) --> <nn>       */
/*                   must be within the range (1 <= <nn> <= D23_MXSIZECBUF)  */
/*       -s<nnn>   = Max. no. of available server process entries -->        */
/*                   <nnn> must be within the range ( 1 <= <nnn> <=          */
/*                   D23_MXSRVPROC)                                          */
/*       -c<nnn>   = Max. no. of available client process entries -->        */
/*                   <nnn> must be within the range ( 1 <= <nnn> <=          */
/*                   D23_MXCLTPROC)                                          */
/*       -u<nnn>   = Max. no. of available user credential entries -->       */
/*                   <nnn> must be within the range ( 0 <= <nnn> <=          */
/*                   D23_MXCRED)                                             */
/*       -h<nnn>   = Max. no. of buffers within the shared memory heap -->   */
/*                   <nnn> must be within the range (0 <= <nnn> <=           */
/*                   D23_MXHPBUF)                                            */
/*       -e<einfo> = Description of an extension area in shared memory.      */
/*                   <einfo> has the format <id>,<size>[,<entries>][,<val>]  */
/*                   <id>      = ext. area identifier (must be unambiguous   */
/*                               if several ext. areas are specified)        */
/*                   <size>    = size of extension area (if <entries> is     */
/*                               not specified or if <entries> is equal to   */
/*                               zero) or size of one entry in ext. area     */
/*                   <entries> = # of entries in extension area              */
/*                   <val>     = ext. area intialization value               */
/*                               (if not specified the ext. area is keeps    */
/*                               uninitialized)                              */
/*                   e.g.        -e 1,32,128,0                               */
/*                                                                           */
/*	 -P,-v, -w = serviceability switches                                 */
/*                or                                                         */
/*       -R        = Removes IPC-resources from system                       */
/*	 -P,-v, -w = serviceability switches                                 */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*    Author: R.Horn                                      Date: 17.5.91      */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <locale.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <dce/d2dir.h>
#include <dce/dcesvcmsg.h>
#include <dce/d27util.h>
#include <d23ipc.h>

/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern char	*optarg ;	/* reference to actual option argument */
extern int	opterr ;

/* ------------------- DECLARATION OF LOCAL DATA --------------------------- */

				/* semaphore set initialization information */
static unsigned16	seminit[D23_MXSEMA] = { 1 } ;

/* ------------------- PROTOTYPING OF LOCAL FUNCTIONS ---------------------- */

					/* remove IPC-resources */
signed32 rem_ipc (key_t) ;
int main (signed32 argc, char *argv[]);

/* ------------------- PROTOTYPING OF EXTERNAL FUNCTIONS ------------------- */
#if defined(__OSF__) || defined(_AIX)
extern key_t ftok (char *path_name, char project_id);
#endif /* __OSF__ || _AIX */

/* ------------------------------------------------------------------------- */

main (signed32 argc, char *argv[]) {

    key_t		ipc_key ;
    boolean		remmode = FALSE ;
    signed32		i, opt, shmsize, shmid, semid, msgid, instno ;
    signed32		szcbuf = D23_MXSIZECBUF/D23_MDEFREL ;
    signed32		mxdst = D23_MXDISTCMD/D23_MDEFREL ;
    signed32		mxreg = D23_MXREGINFO/D23_MDEFREL ;
    signed32		mxsrv = D23_MXSRVPROC/D23_MDEFREL ;
    signed32		mxclt = D23_MXCLTPROC/D23_MDEFREL ;
    signed32		mxusr = D23_MXCRED/D23_MDEFREL ;
    signed32		mxhpb = D23_MXHPBUF/D23_MDEFREL ;
    signed32		ext_asizes = 0, no_ext = 0 ;
    signed32		res, ext_size, ext_id, ext_mxentries, ext_ival ;
    char		ipc_path[D27_LFILE_NAME] ;
    D23_ipcinfo		*ipcinfo ;
    D23_distcmd		*distinfo ;
    D23_reginfo		*procinfo ;
    D23_ipcentry	*cltptr ;
    D23_ipcentry	*svrptr ;
    D23_usrcred		*usrcred ;
    D23_extinfo		*extinfo = (D23_extinfo *) NULL, *extptr ;
    error_status_t	svc_result ;
    union semun {
	int val;
	struct semid_ds *buf;
	unsigned16 *array;
    } semctl_arg;	/* union needed on SUN machines	*/

    setlocale(LC_ALL, "");

    /* initialize serviceability */
    d27_001_svc_init (argc, argv, &svc_result) ;
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to initialize serviceability, status: 0x%lx\n",    
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* evaluate all calling parameters */
    opterr = 0 ;
    while ((opt = getopt (argc, argv, "c:d:e:h:l:P:Rr:s:u:v:w:")) != -1) {
	switch (opt) {
	case 'R':
	    remmode = TRUE ;
	    break ;
	case 'd':
	    if ((mxdst = atoi (optarg)) > D23_MXDISTCMD || mxdst <= 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		   svc_c_sev_fatal, GDS_S_IPCMXDIST_ERR, mxdst, D23_MXDISTCMD) ;
		exit (1) ;
	    }
	    break ;
	case 'r':
	    if ((mxreg = atoi (optarg)) > D23_MXREGINFO || mxreg <= 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		   svc_c_sev_fatal, GDS_S_IPCMXREG_ERR, mxreg, D23_MXREGINFO) ;
		exit (1) ;
	    }
	    break ;
	case 'l':
	    if ((szcbuf = atoi (optarg)) > D23_MXSIZECBUF || szcbuf <= 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		 svc_c_sev_fatal, GDS_S_IPCMXBUFS_ERR, szcbuf, D23_MXSIZECBUF) ;
		exit (1) ;
	    }
	    break ;
	case 's':
	    if ((mxsrv = atoi (optarg)) > D23_MXSRVPROC || mxsrv <= 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		    svc_c_sev_fatal, GDS_S_IPCMXSRV_ERR, mxsrv, D23_MXSRVPROC) ;
		exit (1) ;
	    }
	    break ;
	case 'c':
	    if ((mxclt = atoi (optarg)) > D23_MXCLTPROC || mxclt <= 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		    svc_c_sev_fatal, GDS_S_IPCMXCLT_ERR, mxclt, D23_MXCLTPROC) ;
		exit (1) ;
	    }
	    break ;
	case 'u':
	    if ((mxusr = atoi (optarg)) > D23_MXCRED || mxusr < 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		       svc_c_sev_fatal, GDS_S_IPCMXCRD_ERR, mxusr, D23_MXCRED) ;
		exit (1) ;
	    }
	    break ;
	case 'h':
	    if ((mxhpb = atoi (optarg)) > D23_MXHPBUF || mxhpb < 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
		       svc_c_sev_fatal, GDS_S_IPCMXHP_ERR, mxhpb, D23_MXHPBUF) ;
		exit (1) ;
	    }
	    break ;
	case 'e':
	    /* evaluate information describing a extension area */
	    ext_mxentries = ext_ival = 0 ;
	    if ((res = sscanf (optarg, "%d,%d,%d,%d", &ext_id, &ext_size,
					      &ext_mxentries, &ext_ival)) < 2) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
		   svc_c_sev_fatal, GDS_S_IPCEXTSPEC_ERR) ;
		exit (1) ;
	    }
	    /* verify consistence of information */
	    for (i = 0, extptr = extinfo; i < no_ext; i++, extptr++) {
		if (extptr->d23_extid == ext_id) {
		    /* extension area identifier already exists */
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					svc_c_sev_fatal, GDS_S_IPCEXTSPEC_ERR) ;
		    exit (1) ;
		}
	    }
	    if (ext_size <= 0 || ( res > 2 && ext_mxentries < 0) ||
				(res > 3 && (ext_ival < 0 || ext_ival > 255))) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
		   svc_c_sev_fatal, GDS_S_IPCEXTSPEC_ERR) ;
		exit (1) ;
	    }

	    /* save information */
	    extinfo = (extinfo == (D23_extinfo *) NULL) ?
		(D23_extinfo *) malloc (sizeof (D23_extinfo)) :
		(D23_extinfo *) realloc ((void *) extinfo, (no_ext + 1) *
							 sizeof (D23_extinfo)) ;
	    if (extinfo == (D23_extinfo *) NULL) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				svc_c_sev_fatal, GDS_S_NOMEMORY,
				(no_ext + 1) * sizeof (D23_extinfo)) ;
		exit (1) ;
	    }
	    extptr = extinfo + no_ext;
	    extptr->d23_extstate = (res > 3) ? D23_INITIALIZED : 0 ;
	    extptr->d23_extid    = ext_id ;
	    extptr->d23_mextno   = ext_mxentries ;
	    extptr->d23_aextno   = 0 ;
	    extptr->d23_extsize  = (ext_mxentries > 0) ?
					   ext_size * ext_mxentries : ext_size ;
	    extptr->d23_extoff   = ext_asizes ;
	    extptr->d23_private  = ext_ival ;

	    no_ext++ ;
	    ext_asizes += D2_ALIGN(extptr->d23_extsize) ;
	    break ;
	case 'P':
	case 'v':
	case 'w':
	    break ;
	default:
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%c"), GDS_S_GENERAL,
				   svc_c_sev_fatal, GDS_S_IPCUNKNOWN_ERR, opt) ;
	    exit (1) ;
	}
    }

    /* get # of installation */
    sprintf (ipc_path, D23_IPC_KFILE, dcelocal_path, D27_BIN_DIR);
    instno = 1;
    if ((ipc_key = ftok (ipc_path, D23_IPC_KEY)) == (key_t) -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%c%d"), GDS_S_GENERAL,
                svc_c_sev_fatal, GDS_S_FTOK_ERR, ipc_path, D23_IPC_KEY, errno) ;
	exit (1) ;
    }

    if (argc == 2 && remmode)
	/* command option was -R -> remove old IPC-resources */
	exit ((rem_ipc (ipc_key) == D23_SUCES) ? 0 : 1) ;

    /* command: initialize new IPC-resources */
    /* examine the size of the shared memory area */
    shmsize = sizeof (D23_ipcinfo) + mxdst * sizeof (D23_distcmd) +
	      mxreg * sizeof (D23_reginfo) + mxsrv * sizeof (D23_ipcentry) +
	      mxclt * sizeof (D23_ipcentry) + mxclt * szcbuf * 1024 +
	      mxusr * sizeof (D23_usrcred) + mxhpb * sizeof (D23_hpbufinfo) +
	      no_ext * sizeof (D23_extinfo) + ext_asizes ;

    /* test availability of valid IPC-resources */
    if ((shmid = shmget (ipc_key, 0, D23_ACCMODE)) == 0) {
	/* shared memory already exists -> check state */
	if ((ipcinfo = (D23_ipcinfo *) shmat (shmid, 0, 0)) ==
							   (D23_ipcinfo *) -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				      svc_c_sev_fatal, GDS_S_SHMAT_ERR, errno) ;
	     exit (1) ;
	}
	/* verify existence of the IPC-monitoring process */
	procinfo = (D23_reginfo *) ((char *) ipcinfo + ipcinfo->d23_rarea) ;
	for (i = 0; i < ipcinfo->d23_mrno; i++, procinfo++) {
	    if (procinfo->d23_pregtype == D23_MONPROC &&
					  kill (procinfo->d23_procid, 0) == 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					  svc_c_sev_fatal, GDS_S_IPCINUSE_ERR) ;
		exit (1) ;
	    }
	}
    }	

    /* remove old IPC-resources (if there are any) */
    if (rem_ipc (ipc_key) == D23_ERROR)
	 exit (1) ;

    /* get required shared memory area */
    if ((shmid = shmget (ipc_key, shmsize, IPC_CREAT | IPC_EXCL |
							  D23_ACCMODE)) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
			svc_c_sev_fatal, GDS_S_SHMGET_ERR, ipc_key, IPC_CREAT |
			IPC_EXCL | D23_ACCMODE, errno) ;
	exit (1) ;
    }

    /* attach to shared memory */
    if ((ipcinfo = (D23_ipcinfo *) shmat (shmid, 0, 0)) ==
							   (D23_ipcinfo *) -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
                                      svc_c_sev_fatal, GDS_S_SHMAT_ERR, errno) ;
	exit (1) ;
    }

    /* initialize shared memory - common information area */
    ipcinfo->d23_versno = D23_SHMVERS ;
    ipcinfo->d23_creatime = time (0L) ;
    ipcinfo->d23_lvpid = 0 ;
    ipcinfo->d23_linvid = 1 ;

    ipcinfo->d23_mdno = mxdst ;
    ipcinfo->d23_dno = 0 ;
    ipcinfo->d23_darea = sizeof (D23_ipcinfo) ;

    ipcinfo->d23_mrno = mxreg ;
    ipcinfo->d23_rno = 0 ;
    ipcinfo->d23_rarea = ipcinfo->d23_darea + mxdst * sizeof (D23_distcmd) ;

    ipcinfo->d23_msno = mxsrv ;
    ipcinfo->d23_sno = 0 ;
    ipcinfo->d23_sarea = ipcinfo->d23_rarea + mxreg * sizeof (D23_reginfo) ;

    ipcinfo->d23_mcno = mxclt ;
    ipcinfo->d23_cno = 0 ;
    ipcinfo->d23_carea = ipcinfo->d23_sarea + mxsrv * sizeof (D23_ipcentry) ;

    ipcinfo->d23_bno = mxclt ;
    ipcinfo->d23_bsize = szcbuf * 1024 ;
    ipcinfo->d23_barea = ipcinfo->d23_carea + mxclt * sizeof (D23_ipcentry) ;

    ipcinfo->d23_mcred = mxusr ;
    ipcinfo->d23_cred = 0 ;
    ipcinfo->d23_crarea = ipcinfo->d23_barea + mxclt * ipcinfo->d23_bsize ;

    ipcinfo->d23_mhpno = mxhpb ;
    for (i = 0 ; i < D23_MXHPBUF / D23_FLDSIZE; i++)
	ipcinfo->d23_hpstate[i] = 0L ;
    ipcinfo->d23_hparea = ipcinfo->d23_crarea + mxusr * sizeof (D23_usrcred) ;

    ipcinfo->d23_mext = no_ext ;
    ipcinfo->d23_exarea = ipcinfo->d23_hparea + mxhpb * sizeof (D23_hpbufinfo) ;

    /* initialize distributed command area */
    ipcinfo->d23_gltstamp = 0L ;
    distinfo = (D23_distcmd *) ((char *) ipcinfo + ipcinfo->d23_darea) ;
    for (i = 0; i < ipcinfo->d23_mdno; i++, distinfo++)
	distinfo->d23_dcmd = D23_UNUSED ;

    /* initialize IPC-registration area */
    procinfo = (D23_reginfo *) ((char *) ipcinfo + ipcinfo->d23_rarea) ;
    for (i = 0; i < ipcinfo->d23_mrno; i++, procinfo++)
	procinfo->d23_pregtype = D23_UNUSED ;

    /* initialize shared memory - server process area */
    svrptr = (D23_ipcentry *) ((char *) ipcinfo + ipcinfo->d23_sarea) ;
    for (i = 0; i < mxsrv; i++, svrptr++)
	svrptr->d23_estate = D23_SES(D23_NOSVR) ;

    /* initialize shared memory - client process area */
    cltptr = (D23_ipcentry *) ((char *) ipcinfo + ipcinfo->d23_carea) ;
    for (i = 0; i < mxclt; i++, cltptr++)
	cltptr->d23_estate = D23_SES(D23_NOCLT) ;

    /* initialize shared memory - user credential area */
    usrcred = (D23_usrcred *) ((char *) ipcinfo + ipcinfo->d23_crarea) ;
    for (i = 0; i < mxusr; i++, usrcred++)
	usrcred->d23_credusrid = D23_UNUSED ;

    /* copy ext. area(s) description area information into shared memory */
    extptr = (D23_extinfo *) ((char *) ipcinfo + ipcinfo->d23_exarea) ;
    memcpy ((void *) extptr, (void *) extinfo,
				     (size_t) (no_ext * sizeof (D23_extinfo))) ;
    free (extinfo) ;

    /* initialize shared memory - ext. areas */
    /* update ext. area(s) description area */
    for (i = 0; i < no_ext; i++, extptr++) {
	extptr->d23_extoff += ipcinfo->d23_exarea + no_ext *
							  sizeof (D23_extinfo) ;
	if (extptr->d23_extstate == D23_INITIALIZED) {
	    memset ((void *) ((char *) ipcinfo + extptr->d23_extoff),
		      (int) extptr->d23_private, (size_t) extptr->d23_extsize) ;
	    extptr->d23_private = 0 ;
	}
    }

    /* get semaphores */
    if ((semid = semget (ipc_key, D23_MXSEMA, IPC_CREAT | IPC_EXCL |
							  D23_ACCMODE)) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
			svc_c_sev_fatal, GDS_S_SEMGET_ERR, ipc_key, IPC_CREAT |
			IPC_EXCL | D23_ACCMODE, errno) ;
	/* release shared memory area */
	shmctl (shmid, IPC_RMID, (struct shmid_ds *) NULL) ;
	exit (1) ;
    }

    /* get message queue */
    if ((msgid = msgget (ipc_key, IPC_CREAT | IPC_EXCL | D23_ACCMODE)) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
			svc_c_sev_fatal, GDS_S_MSGGET_ERR, ipc_key, IPC_CREAT |
			IPC_EXCL | D23_ACCMODE, errno) ;
	/* release shared memory area */
	shmctl (shmid, IPC_RMID, (struct shmid_ds *) NULL) ;

	/* release semaphores */
	semctl_arg.buf = (struct semid_ds *) NULL;
	semctl (semid, 0, IPC_RMID, semctl_arg) ;
	exit (1) ;
    }

    /* initialize IPC-shared memory semaphore */
    semctl_arg.array = seminit;
    if (semctl (semid, D23_MXSEMA, SETALL, semctl_arg) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			     svc_c_sev_fatal, GDS_S_SEMCTL_ERR, SETALL, errno) ;
	/* release shared memory area */
	shmctl (shmid, IPC_RMID, (struct shmid_ds *) NULL) ;
	/* release semaphores */
	semctl_arg.buf = (struct semid_ds *) NULL;
	semctl (semid, 0, IPC_RMID, semctl_arg) ;
	/* release message queue */
	msgctl (msgid, IPC_RMID, (struct msqid_ds *) NULL) ;
	exit (1) ;
    }
    exit (0) ;
}

/* ------------------------------------------------------------------------- */

signed32 rem_ipc (key_t ipc_key) {

    signed32	shmid, semid, msgid ;
    union semun {
	int val;
	struct semid_ds *buf;
	unsigned16 *array;
    } semctl_arg;	/* union needed on SUN machines	*/

    /* remove all available IPC-resources */

    if ((shmid = shmget (ipc_key, 0, D23_ACCMODE)) == -1) {
	if (errno != ENOENT) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
	       svc_c_sev_fatal, GDS_S_SHMGET_ERR, ipc_key, D23_ACCMODE, errno) ;
	    return (D23_ERROR) ;
	}
    } else {
	if (shmctl (shmid, IPC_RMID, (struct shmid_ds *) NULL) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			   svc_c_sev_fatal, GDS_S_SHMCTL_ERR, IPC_RMID, errno) ;
	}
    }

    if ((semid = semget (ipc_key, D23_MXSEMA, D23_ACCMODE)) == -1) {
	if (errno != ENOENT) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
	       svc_c_sev_fatal, GDS_S_SEMGET_ERR, ipc_key, D23_ACCMODE, errno) ;
	    return (D23_ERROR) ;
	}
    } else {
	semctl_arg.buf = (struct semid_ds *) NULL;
	if (semctl (semid, 0, IPC_RMID, semctl_arg) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			   svc_c_sev_fatal, GDS_S_SEMCTL_ERR, IPC_RMID, errno) ;
	    return (D23_ERROR) ;
	}
    }
    if ((msgid = msgget (ipc_key, D23_ACCMODE)) == -1) {
	if (errno != ENOENT) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_GENERAL,
	       svc_c_sev_fatal, GDS_S_MSGGET_ERR, ipc_key, D23_ACCMODE, errno) ;
	    return (D23_ERROR) ;
	}
    } else {
	if (msgctl (msgid, IPC_RMID, (struct msqid_ds *) NULL) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			   svc_c_sev_fatal, GDS_S_MSGCTL_ERR, IPC_RMID, errno) ;
	    return (D23_ERROR) ;
	}
    }
    return (D23_SUCES) ;
}
