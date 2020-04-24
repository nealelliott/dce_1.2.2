/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20daemon.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:22  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:18:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:24  root]
 * 
 * Revision 1.1.8.5  1994/08/25  13:56:17  keutel
 * 	OT 11826: buffer for SVC calls was too small
 * 	[1994/08/25  13:25:04  keutel]
 * 
 * Revision 1.1.8.4  1994/08/16  08:15:57  marrek
 * 	Fix for delta update (ot 11617).
 * 	[1994/08/12  11:59:34  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:57  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:26:45  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:13:40  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:42  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:27  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:19  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:34:05  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:35:18  marrek
 * 	July 1993 code drop
 * 	[1993/07/30  08:50:09  marrek]
 * 
 * Revision 1.1.4.5  1993/01/29  08:38:20  marrek
 * 	error log operation id corrected
 * 	[1993/01/28  10:41:37  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:09:54  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:01  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:12:05  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:07:14  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  14:55:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:09:09  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:05:04  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:45:59  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20daemon.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:39 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   MODULE:   d20daemon.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The module contains the functionality to do all required delta      */
/*       update operations. This is done by performing the following steps:  */
/*                                                                           */
/*          1. Evaluate all calling parameter.                               */
/*                                                                           */
/*          2. Remove old log-files.                                         */
/*                                                                           */
/*          3. Synchronize all DSA-server processes, rename the SHDLOG-file  */
/*             to a temporary SHDLOG-file (SHDLOG.pid -> internally used to  */
/*             handle the contained log-informations) and then resynchronize */
/*             all DSA-server processes (the server processes can continue   */
/*             by writing the log-informations to the newly created SHDLOG-  */
/*             file).                                                        */
/*                                                                           */
/*          4. Read the job-table from the SHDJOBS-file into main memory     */
/*             and convert all existing invalid DN-indexes of DSA-names      */
/*             into valid DN-indexes. If such a conversion is required, then */
/*             the modified job-table is written back to the SHDJOBS-file.   */
/*             Synchronization of access to SHDJOBS-file with IAPL is done.  */
/*                                                                           */
/*          5. Read the log-informations contained within the temporary      */
/*             SHDLOG-file (entry by entry) and look for a corresponding     */
/*             entry within the job-table. If there is any (means this log-  */
/*             entry information is relevant for delta update operation),    */
/*             then this log-entry is written to a DSA-specific SHDLOG-file  */
/*             (SHDDSA.<DN-index>) and to the SHDSAVE-file. If there is no   */
/*             corresponding job-entry, then the log-entry is written only   */
/*             to the SHDSAVE-file. At the end of these operation the        */
/*             temporary SHDLOG-file is deleted.                             */
/*                                                                           */
/*          6. Search for job-table entries where the time stamp is equal    */
/*             to the actual time. If such an entry is found, then all log-  */
/*             entries of the corresponding DSA-specific SHDLOG-file are     */
/*             read into memory (entry by entry). For each log-entry, rele-  */
/*             vant for updating, an update operation is started and the     */
/*             update is performed. Entries, not relevant for updating resp. */
/*             entries where the update operation fails, are written to a    */
/*             temporary DSA-specific SHDLOG-file which is renamed to the    */
/*             original DSA-specific SHDLOG-file at the end of this DSA-     */
/*             specific SHDLOG-file handling.                                */
/*             If an update operation fails, then a new error log-entry is   */
/*             made within the SHDUPDERR-file resp. the log-entry is changed */
/*             if he already exists.                                         */
/*                                                                           */
/*          7. All entries of the SHDUPDERR-file which are set to a 'no      */
/*             action'-state are removed.                                    */
/*                                                                           */
/*   CALLING PARAMETER:                                                      */
/*             -d<directory-id>                                              */
/*             -m<operation-mode>                                            */
/*                1 = do distribution of SHDLOG-file only.                   */
/*                2 = do distribution of SHDLOG-FILE and also all required   */
/*                    delta update operations.                               */
/*             -n<job-entry-no>  (optional switch)                           */
/*                no. of the job-entry within the job-table which            */
/*                should be handled immediately even if the time stamp is    */
/*                invalid.                                                   */
/*             -l<size> (optional switch)                                    */
/*                max. size of the SHDSAVE-file in kB.                       */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                          DATE: 23.3.89   */
/*datoff *********************************************************************/

#include <gds.h>
#include <dce/dcesvcmsg.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <locale.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d26dsa.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d20proto.h>
#include <D00_gds_sec_protect.h>

/* --------------- definition of local constants -------------------------- */

#define D21_MAXJOBS	100		/* max. no. of job table entries */
#define D21_KBSZ	1024		/* kB-unit size */
#define D21_CBUFSZ	(2 * D21_KBSZ)	/* size of the command buffer */
#define D21_JBUFSZ	(8 * D21_KBSZ)	/* size of the job buffer */
#define D21_SMSGSZ	(4 * D21_KBSZ)	/* size of the log-message buffer */
#define D21_ALLVAL	-1		/* all possible time values */
#define D21_HANDLED	-1		/* 'handled entry'-flag */
#define D21_ACTION	1		/* job entry state values */
#define D21_MOBJ	2
#define D21_FOBJ	4
#define D21_LINE	__LINE__
#ifdef DEBUG
FILE    *debug_fd = (FILE *) NULL;
#define D20_LEN_LOGPATH	100		/* max. length of logfilepath */
#define DBGPRINTF(x)     if (debug_fd != (FILE *) NULL) { fprintf x ;\
				fflush(debug_fd); } 
#else
static char debug_fd[200 + D21_CBUFSZ];
#define DBGPRINTF(x)     sprintf x ; \
			 DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG, debug_fd));
#endif

/* ----------------- declaration of global data ---------------------------- */

extern char	d26_install[] ;
extern int      d27_pid;

Bool		asn1_log;
#ifdef DEBUG
char 		logfname[D27_LFILE_NAME];
char		logfpath[D20_LEN_LOGPATH];
#endif

/* ----------------- declaration of local data ----------------------------- */

static D2_jobentry	jobtab[D21_MAXJOBS] ;	/* job_table */
static char		dn_buffer[D2_DNL_MAX] ; /* temporary DN-buffer */
static D2_logentry	logentry = { dn_buffer } ;/* log-entry */
static char		cmdbuf[D21_CBUFSZ] ;	/* command buffer */
						/* name of the local DSA */
static char		loc_dsaname[D2_DNL_MAX] ;
static char		upd_dsaname[D2_DNL_MAX] ;
static signed32		loc_dsaindex ;
static char		jobhdinfo[D21_T_OFF] ;	/* SHDJOB-header informations */
						/* delta update operations */
static char             *upd_operation[] = { "-", "-", "-", "-", "-", "-",
					     D2_A_OBJ, D2_R_OBJ, D2_M_OBJ,
					     D2_M_RDN } ;
static D23_atclpb  	ipc_attach_pb = {{D23_V02, 0, 0, 0}, D23_SYNC_EM};

/* LOCAL FUNCTIONS */

static signed32 read_log_entry ( FILE * , D2_logentry * );
static signed32 write_log_entry( FILE * , D2_logentry * , Bool );
static signed32 fatal_error_hdl( unsigned32 , signed32 , char * , signed32 );

/* ------------------------------------------------------------------------- */

main (
  int argc,
  char * argv[])
{
    D23_dwritepb  wdc_pb = {{D23_V02}};
    register signed32	i, j ;
    int		dir_id = -1;
    int		op_mode = -1; 
    signed32	last_dnindex = -1 ;
    signed32	job_entry_no = -1, no_of_jobs, result, srchstate ;
    signed32	nmsize;
    int		shdsavesize = 0 ;
    char	fname[D27_LFILE_NAME], fname1[D27_LFILE_NAME] ;
    char	fname2[D27_LFILE_NAME];
    char        flockjobs[D27_LFILE_NAME] ;
    char	*jend, *jhlp ;
    char	*jstart = NULL;
    FILE        *fd, *lockfd, *jobfd, *logfd, *errfd, *errinfofd, *fd_jobs ;
    FILE        *dsatmpfd = NULL,
    		*dsafd = NULL ;
    int         fno_jobs;
    struct stat	fileinfo;
    D2_jobentry	*jobptr, *jobptr1 ;
    signed32	dn_sindex ;
    Bool	conversion = FALSE ;
    time_t	seconds ;
    struct tm	*act_time ;
    D2_pbhead	pbhead ;
    Upd_time    *upd_t;
    char        *dn, *dn_max;
    signed32    del_count;
				    /* flag indicating whether temporary   */
				    /* SHDLOG file was generated           */
    Bool	tmp_file = FALSE;
    Bool        write_tmp_file;
    DIR		*dirptr;
    error_status_t st;		    /* error status from svc_init 	   */

#if defined(_AIX)
    struct direct	*deptr ;
#else
    struct dirent	*deptr ;
#endif
	signed32	dsaindex ;
    /* variables for fcntl */
    static struct flock    arg = { F_WRLCK, SEEK_SET, 0L, 0L, 0 };


    setlocale(LC_ALL, "");

    /* initialize serviceability  */
    d27_001_svc_init(argc,argv, &st);
    if (st != svc_s_ok)
    {
      fprintf(stderr,"Unable to initialize serviceability, status: 0x%lx\n",
	st);
      exit(9);
    }

#ifdef THREADSAFE
if (D00_gds_sec_protect_init() != 0)
{
  DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG, 
	"protection initialisation for gds_sec failed"));
}
#endif

    asn1_log = FALSE;
    for (i = 1; i < argc; i++) {
	if (*argv[i]++ != '-')
	    fatal_error_hdl (GDS_S_SUP_USAGE, 0, (char *) NULL, D21_LINE) ;
	switch (*argv[i]++) {
	case 'd':
	    dir_id = atoi (argv[i]) ;
	    break ;
	case 'm':
	    op_mode = atoi (argv[i]) ;
	    /* mode = 1 -> do distribution of SHDLOG-file only */
	    /* mode = 2 -> do distribution of SHDLOG-file and handle */
	    /*             distributed DSA-specific files */
	    break ;
	case 'n':
	    job_entry_no = atoi (argv[i]) ;
	    break ;
	case 'l':
	    shdsavesize = atoi (argv[i]) * D21_KBSZ ;
	    break ;
	case 'a':
            asn1_log = TRUE;
	    break;
	case 'v':
	case 'w':
	case 'P':
	    break;
	default:
	    fatal_error_hdl (GDS_S_SUP_USAGE, 0, (char *) NULL, D21_LINE) ;
	}
    }

    if (op_mode != 1 && op_mode != 2)
	fatal_error_hdl (GDS_S_SUP_WRONG_OPMODE, op_mode, (char *)NULL, 
	D21_LINE) ;
    if (dir_id < D2MINDIR_ID || dir_id > D2MAXDIR_ID)
	fatal_error_hdl (GDS_S_SUP_ILL_DIRID, dir_id, (char *)NULL, D21_LINE) ;

    dce_svc_printf(GDS_S_ENTRY_DAEMON_MSG,dir_id);
    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,svc_c_debug1,
	GDS_S_ENTRY_DAEMON,dir_id));

    /* initialize par-block for distributed command */
    wdc_pb.d23_Jsv_prid = D23_DS0SID + dir_id;
    wdc_pb.d23_Jtmout = D23_DMAXTOUT;
    wdc_pb.d23_Jparsize = 0;
    wdc_pb.d23_Jparref = NULL;

    /* get actual time stamp */
    seconds = time (0L) ;
    act_time = (struct tm*) localtime (&seconds) ;
    /* correct date and time (month, day of the week) to */
    /* internally used ranges */
    act_time->tm_mon++ ;
    if (act_time->tm_wday == 0)
    	act_time->tm_wday = 7 ;

    /* generate name of the data base directory */
    sprintf (d26_install, "%s%s%s%s%d", dcelocal_path, D27_SERVER_DIR,
					D27_DSA_DIR, D27_SUBDSA_DIR, dir_id);
#ifdef DEBUG
    sprintf (logfpath, "%s%s%s%s%s%d", dcelocal_path, 
			D27_SERVER_DIR, D27_ADM_DIR, D27_DSA_DIR, 
			D27_SUBDSA_DIR, dir_id );
    sprintf (logfname, "%s/DEBUG%d", logfpath, getpid() );
    if ((debug_fd = fopen(logfname, "w")) != (FILE *) 0)
	fprintf (debug_fd, "OP-MODE: %d DIR_ID: %d\n", op_mode, dir_id) ;
#endif

    /* generate name of the daemon lock file and lock the file */
    sprintf (fname, "%s%s", d26_install, D2_LOGDAE) ;
    if ((lockfd = (FILE *) fopen (fname, "a")) == (FILE *) NULL)
	fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, fname, D21_LINE) ;

    arg.l_type = F_WRLCK;
    if (fcntl (fileno (lockfd), F_SETLKW, &arg) == (-1))
	fatal_error_hdl (GDS_S_SUP_LOCK_ERR, errno, fname, D21_LINE) ;
	
    /* mac - attach client */
    if (d23_attclt (&ipc_attach_pb) == D23_ERROR)
	fatal_error_hdl (GDS_S_SUP_ATTACH_ERR, 0, (char *)NULL, D21_LINE) ;
    DBGPRINTF ((debug_fd, "attached client\n")) ;
	
    /* synchronize DSA-processes */
    wdc_pb.d23_Jcmd = D23_CLLFILE;
    if (d23_write_dist_cmd(&wdc_pb) == D23_ERROR)
	fatal_error_hdl (GDS_S_SUP_SYNC_DSA_ERR, 0, (char *)NULL, D21_LINE) ;
    DBGPRINTF ((debug_fd, "DSA-processes synchronized\n")) ;

    /* generate name of SHDLOG-file and temporary SHDLOG-file */
    sprintf (fname, "%s%s", d26_install, D2_SHDLOG) ;
    sprintf (fname1, "%s%s.%d", d26_install, D2_SHDLOG, getpid ()) ;
    if ((result = link (fname, fname1)) == D2_ERROR) {
	if (errno != ENOENT) {
	    /* resynchronize DSA-processes */
    	    wdc_pb.d23_Jcmd = D23_OPLFILE;
    	    if (d23_write_dist_cmd(&wdc_pb) == D23_ERROR)
		fatal_error_hdl (GDS_S_SUP_RESYNC_DSA_ERR, 0, (char *)NULL, 
			D21_LINE) ;
	    DBGPRINTF ((debug_fd, "DSA-processes resynchronized\n")) ;

	    fatal_error_hdl (GDS_S_LINK_ERR, errno, fname, D21_LINE) ;
	}
    } else {
	tmp_file = TRUE;
	if ((result = unlink (fname)) == D2_ERROR) {
	    /* resynchronize DSA-processes */
    	    wdc_pb.d23_Jcmd = D23_OPLFILE;
    	    if (d23_write_dist_cmd(&wdc_pb) == D23_ERROR)
		fatal_error_hdl (GDS_S_SUP_RESYNC_DSA_ERR, 0, (char *)NULL, 
			D21_LINE) ;

	  DBGPRINTF ((debug_fd, "DSA-processes resynchronized\nunlink failed\n"));

	    fatal_error_hdl (GDS_S_UNLINK_ERR, errno, fname, D21_LINE) ;
	}
    }
	
    /* resynchronize DSA-processes */
    wdc_pb.d23_Jcmd = D23_OPLFILE;
    if (d23_write_dist_cmd(&wdc_pb) == D23_ERROR)
	fatal_error_hdl (GDS_S_SUP_RESYNC_DSA_ERR, 0, (char *)NULL, D21_LINE) ;

    DBGPRINTF ((debug_fd, "DSA-processes resynchronized\n")) ;

    /* verify temporary SHDLOG-file for available log-informations */
    if (tmp_file == TRUE) {
	if ((logfd = (FILE *) fopen (fname1, "r")) == (FILE *) NULL)
	    fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, fname1, D21_LINE) ;

	/* get size of the temporary SHDLOG-file */
	if (fstat (fileno (logfd), &fileinfo) == -1)
	    fatal_error_hdl (GDS_S_SUP_NO_FILE_INFO, errno, fname1, D21_LINE) ;
    } else
	fileinfo.st_size = 0;

    /* allocate memory for temporary storage of delta update informations */
    if ((jstart = jhlp = (char *) malloc (D21_JBUFSZ)) == (char *) NULL)
	fatal_error_hdl (GDS_S_NOMEMORY, D21_JBUFSZ, (char *)NULL, D21_LINE) ;
    jend = jstart + D21_JBUFSZ ;

    if ((logentry.d2_lmsg = (char *) malloc (D21_SMSGSZ)) == (char *) NULL)
	fatal_error_hdl (GDS_S_NOMEMORY, D21_SMSGSZ, (char *)NULL, D21_LINE) ;

    /* generate name of the DSA-name file and read name of the local DSA */
    sprintf (fname, "%s%s", d26_install, D27_DSANAME) ;
    if ((fd = (FILE *) fopen (fname, "r")) == (FILE *) NULL)
	fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, fname, D21_LINE) ;
    if ((char *) fgets (loc_dsaname, D2_DNL_MAX, fd) == (char *) NULL)
	fatal_error_hdl (GDS_S_SUP_LOCDSA_ERR, errno, (char *)NULL, D21_LINE) ;
    fclose (fd) ;
    /* copy loc_dsaname to d26_myname for logging of update of dn-list */
    strcpy ((char *)d26_myexact_name, loc_dsaname);
    strcpy ((char *)d26_myname, loc_dsaname);
    d27_007_norm_name((char *)d26_myname, strlen((char *)d26_myname) + 1,
			(char *)d26_myname);
    /* get pid for logging of update of dn-list */
    d27_pid = getpid();
    DBGPRINTF ((debug_fd, ("LOCAL DSA-NAME: %s\n", loc_dsaname))) ;
    if (d26_i26_get_idx_from_file (FALSE, (D2_name_string) loc_dsaname,
			&loc_dsaindex) == D26_ERROR)
	fatal_error_hdl (GDS_S_SUP_LOCDSA_DNLIST_ERR, 0, (char *)loc_dsaname, 
		D21_LINE) ;
    DBGPRINTF ((debug_fd, "LOCAL DSA-DN-INDEX: %ld\n", loc_dsaindex)) ;

    /* synchronize access to SHDJOB file                  */
    /* generate name of D2_LOGJOBS file and lock the file */
    sprintf (flockjobs, "%s%s", d26_install, D2_LOGJOBS);
    if ((fd_jobs = (FILE *) fopen (flockjobs, "a")) == (FILE *) NULL)
	fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, flockjobs, D21_LINE) ;

    arg.l_type = F_WRLCK;
    fno_jobs = fileno (fd_jobs);
    if (fcntl (fno_jobs, F_SETLKW, &arg) == (-1))
	fatal_error_hdl (GDS_S_SUP_LOCK_ERR, errno, flockjobs, D21_LINE) ;

    /* log-informations available -> generate name */
    /* of the SHDJOB-file and read informations from file */
    sprintf (fname, "%s%s", d26_install, D21_SHD_JOBS) ;
    no_of_jobs = i = 0 ;
    jobptr = jobtab ;
    if ((jobfd = (FILE *) fopen (fname, "r")) != (FILE *) NULL) {
	jobptr->d2_jobjname = jhlp ;
	jobptr->d2_jdsaname = jobptr->d2_jobjname + D2_DNL_MAX ;

	/* read header information from SHDJOB-file */
	if (fread (jobhdinfo, 1, D21_T_OFF, jobfd) != D21_T_OFF)
	    fatal_error_hdl (GDS_S_SUP_HEADER_ERR, 0, (char *)fname, D21_LINE) ;

	DBGPRINTF ((debug_fd, "----------- Start of SHDJOBS-file handling --------------\n"));
	/* read job entries from SHDJOB-file */
	upd_t = &jobptr->d2_upd_t;
	while (fscanf (jobfd, D2_RJOB_FORM, &jobptr->d2_jstate,
		       &jobptr->d2_jnr, &upd_t->upd_t_mode,
		       &upd_t->upd_min, &upd_t->upd_hour,
		       &upd_t->upd_1d_of_week, &upd_t->upd_1hour,
		       &upd_t->upd_2d_of_week, &upd_t->upd_2hour,
		       jobptr->d2_jobjname, &jobptr->d2_jsubset,
		       jobptr->d2_jdsaname, &jobptr->d2_jdsa_index,
		       &jobptr->d2_jtimestamp) == D2_RJOBAR && i < D21_MAXJOBS) {
	    DBGPRINTF ((debug_fd, "(1) OBJ-NAME: %s %x\n(1) DSA-NAME: %s %x\n",
				  jobptr->d2_jobjname, jobptr->d2_jobjname,
				  jobptr->d2_jdsaname, jobptr->d2_jdsaname));
	    jhlp += strlen (jhlp) + 1 ;
	    strcpy (jhlp, jobptr->d2_jdsaname) ;
	    jobptr->d2_jdsaname = jhlp ;
	    DBGPRINTF ((debug_fd, "(2) OBJ-NAME: %s %x\n(2) DSA-NAME: %s %x\n",
				jobptr->d2_jobjname, jobptr->d2_jobjname,
				jobptr->d2_jdsaname, jobptr->d2_jdsaname));

	    DBGPRINTF ((debug_fd, "%d. JOB-ENTRY:\n  STATE: 0x%02x  NR: %d  MODE: %d\n",
		  i, jobptr->d2_jstate, jobptr->d2_jnr, upd_t->upd_t_mode)) ;
	    DBGPRINTF ((debug_fd, "  MIN: %d  HOUR: %d  DAY1: %d  HOUR1: %d  DAY2: %d  HOUR2: %d\n  OBJ-NAME: %s\n",
		       upd_t->upd_min, upd_t->upd_hour,
		       upd_t->upd_1d_of_week, upd_t->upd_1hour,
		       upd_t->upd_2d_of_week, upd_t->upd_2hour,
		       jobptr->d2_jobjname)) ;
	    DBGPRINTF ((debug_fd, "  SUBSET: %d  DSA-INDEX: %ld\n  DSA-NAME: %s\n  TIME-STAMP: %ld\n",
				jobptr->d2_jsubset, jobptr->d2_jdsa_index,
				jobptr->d2_jdsaname, jobptr->d2_jtimestamp)) ;

	    if (jobptr->d2_jdsa_index == D21_IDX_NOT_DEF) {
		/* do substitution of DSA-name (DN-index) */
		conversion = TRUE ;
		if (d26_i26_get_idx_from_file (FALSE,
			  (D2_name_string) jobptr->d2_jdsaname, &dn_sindex) 
			  == D26_ERROR)
		    fatal_error_hdl (GDS_S_SUP_REMDSA_DNLIST_ERR, 0, 
			jobptr->d2_jdsaname, D21_LINE) ;
		jobptr->d2_jdsa_index = dn_sindex ;
		DBGPRINTF ((debug_fd, "OBJ-NAME: %s DN-INDEX: %ld\n",
				 jobptr->d2_jdsaname, jobptr->d2_jdsa_index)) ;
	    }
	    if (jhlp + (3 * D2_DNL_MAX) > jend)
		fatal_error_hdl (GDS_S_SUP_INSUFF_MEM, 0, (char *)NULL, 
			D21_LINE) ;
	    i++; jobptr++ ;
	    jhlp += strlen (jhlp) + 1 ;
	    jobptr->d2_jobjname = jhlp;
	    jobptr->d2_jdsaname = jobptr->d2_jobjname + D2_DNL_MAX ;
	    upd_t = &jobptr->d2_upd_t;
	}

	fclose (jobfd) ;
	DBGPRINTF ((debug_fd, "----------- End of SHDJOBS-file handling (NO_OF_JOBS: %d) --------------\n", i)) ;

	no_of_jobs = i ;
	d26_i26_get_idx_from_file (TRUE, (D2_name_string)NULL, (signed32 *)NULL) ;

	if (conversion) {
	    DBGPRINTF ((debug_fd, "----------- Start of SHDJOBS-file conversion ------------\n")) ;
	    /* DN-index of a DSA converted -> generate new SHDJOB-file */
	    /* generate name of temporary SHDJOB-file and write new informations to the file */
	    sprintf (fname2, "%s%s.%d", d26_install, D21_SHD_JOBS, getpid ()) ;
	    if ((jobfd = (FILE *) fopen (fname2, "w")) == (FILE *) NULL)
		fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, fname1, D21_LINE) ;

	    /* write header information to temporary SHDJOB-file */
	    if (fwrite (jobhdinfo, 1, D21_T_OFF, jobfd) != D21_T_OFF)
	        fatal_error_hdl (GDS_S_SUP_WR_HEADER_INFO, errno, fname1, 
			D21_LINE) ;

	    /* write job entries to temporary SHDJOB-file */
	    for (i = 0, jobptr = jobtab; i < no_of_jobs; i++, jobptr++) {
		upd_t = &jobptr->d2_upd_t;
	        fprintf (jobfd, D2_WJOB_FORM, jobptr->d2_jstate, jobptr->d2_jnr,
			 upd_t->upd_t_mode, upd_t->upd_min, upd_t->upd_hour,
			 upd_t->upd_1d_of_week, upd_t->upd_1hour,
			 upd_t->upd_2d_of_week, upd_t->upd_2hour,
			 jobptr->d2_jobjname, jobptr->d2_jsubset, jobptr->d2_jdsaname,
			 jobptr->d2_jdsa_index, jobptr->d2_jtimestamp) ;
	    }
	    fclose (jobfd) ;

	    /* rename temporary SHDJOB-file to SHDJOB-file */
	    if (unlink (fname) == -1)
		fatal_error_hdl (GDS_S_UNLINK_ERR, errno, fname, D21_LINE) ;
	    DBGPRINTF ((debug_fd, "SHDJOBS-file unlinked: %s\n", fname)) ;
	    if (link (fname2, fname) == -1)
		fatal_error_hdl (GDS_S_LINK_ERR, errno, fname2, D21_LINE) ;

	    if (unlink (fname2) == -1)
		fatal_error_hdl (GDS_S_UNLINK_ERR, errno, fname2, D21_LINE);
	    DBGPRINTF ((debug_fd,"temp. SHDJOBS-file linked: %s -> %s\n", fname2, fname)) ;
	}
    } else {
	if (errno != ENOENT)
	    fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, fname, D21_LINE) ;
    }

    /* resynchronize access to SHDJOB file                */
    /* unlock file and close file                         */
    arg.l_type = F_UNLCK;
    fcntl (fno_jobs, F_SETLKW, &arg);
    fclose (fd_jobs);

    DBGPRINTF ((debug_fd, "------------- End of SHDJOBS-file conversion -------------------\n")) ;

    /* remove unused DSA-specific SHDLOG-files */
    sprintf (fname, "%s%s", d26_install, D2_UPDATE_DIR) ;
    if ((dirptr = opendir (fname)) == NULL)
        fatal_error_hdl (GDS_S_SUP_OPENDIR_ERR, errno, fname, D21_LINE) ;

#if defined(_AIX)
    while ((deptr = (struct direct *)readdir (dirptr)) != NULL) {
#else
    while ((deptr = (struct dirent *)readdir (dirptr)) != NULL) {
#endif
	if (strncmp (D2_DSA_FONLY, deptr->d_name, sizeof (D2_DSA_FONLY)-1) == 0) {
	    dsaindex = atoi (deptr->d_name+sizeof (D2_DSA_FONLY)) ;
	    for (i = 0, jobptr = jobtab; i < no_of_jobs; i++, jobptr++)
		if (jobptr->d2_jdsa_index == dsaindex) break ;
				
	    if (i == no_of_jobs) {
		/* unlink unreferenced DSA-specific log-file */
		sprintf (fname, "%s%s.%ld", d26_install, D2_DSA_IDX, dsaindex) ;
		if (unlink (fname) == -1)
		    fatal_error_hdl (GDS_S_UNLINK_ERR, errno, fname, 
			D21_LINE) ;
		DBGPRINTF ((debug_fd, "unreferenced DSA-file unlinked: %s\n", fname)) ;

		/* remove DSA-specific entries from SHDERR-file */
		if (d26_i27_get_dn_from_file (FALSE, dsaindex,
				(D2_name_string) upd_dsaname) == D26_ERROR)
		    fatal_error_hdl (GDS_S_SUP_DNLIST_REMDSA_ERR, dsaindex,
						(char *)NULL, D21_LINE) ;

		sprintf (cmdbuf, DELDSACMD, upd_dsaname,
						d26_install, D21_UPD_ERR,
						d26_install, D21_UPD_ERR,
						d26_install, D21_UPD_ERR,
						d26_install, D21_UPD_ERR) ;
		DBGPRINTF ((debug_fd, "DELDSACMD: %s\n", cmdbuf)) ;
		if (system (cmdbuf) != D2_NOERROR)
		    fatal_error_hdl (GDS_S_SUP_REM_UNUSED_ERR, 0, D21_UPD_ERR, 
			D21_LINE) ;
	    }
	}
    }
    closedir (dirptr) ;

    DBGPRINTF ((debug_fd, "------------- Start of SHDLOG-file distribution -----------------\n")) ;

    if (fileinfo.st_size > 0) {
	/* distribute and save SHDLOG-file informations */
	while (read_log_entry (logfd, &logentry) == D2_RLOGPAR) {
	    /* search corresponding job entry */
	    for (i = 0, jobptr = jobtab; i < no_of_jobs; i++, jobptr++) {
		nmsize = strlen (jobptr->d2_jobjname) ;
		if ((jobptr->d2_jsubset == OBJECT_SUBORDINATES &&
		   (strcmp (jobptr->d2_jobjname, D21_ROOT) == 0 ||
		   (strncmp (logentry.d2_lobjname, jobptr->d2_jobjname, nmsize) == 0 &&
		   (logentry.d2_lobjname[nmsize] == D2_RDN_SEP ||
		    logentry.d2_lobjname[nmsize] == D2_EOS)))) ||
		   (jobptr->d2_jsubset == OBJECT &&
		    strcmp (logentry.d2_lobjname, jobptr->d2_jobjname) == 0))
		{   /* object name in SHDLOG file matches job entry
		     * --> check whether object has a mandatory shadow on
		     *     the target DSA;
		     *     if object is a mandatory shadow in the first level
		     *     then it will be updated  ("del_count = 1")
		     */
		    for (dn = logentry.d2_lobjname, dn_max = dn + nmsize,
			 del_count = 0; dn < dn_max; dn++)
		    {   if (*dn == D2_RDN_SEP)
			{   del_count++;
			    if (del_count > 1)
			    {   break;
			    }
			}
		    }

		    if ((logentry.d2_lsupindex != jobptr->d2_jdsa_index ||
			del_count == 1) &&
			jobptr->d2_jdsa_index != loc_dsaindex &&
			logentry.d2_ltimestamp >= jobptr->d2_jtimestamp) {
			/* 1. master knowledge index of superior is not equal */
			/*    to the DSA-index or object is an entry of the   */
			/*    first level                                     */
			/* 2. DSA-index is not equal to the DN-index of the   */
			/*    local DSA                                       */
			/* 3. master update time is younger than job creation */
			/*    time                                            */
			DBGPRINTF ((debug_fd,"CORRESPONDING JOB ENTRY FOUND (%d)\n", i)) ;
			if (jobptr->d2_jdsa_index != last_dnindex) {
			    if (last_dnindex != -1)
				fclose (dsafd) ;
			    last_dnindex = jobptr->d2_jdsa_index ;

			    /* generate file name of DSA-specific file */
			    /* and open file */
			    sprintf (fname, "%s%s.%ld", d26_install, D2_DSA_IDX,
				last_dnindex) ;
			    if ((dsafd = (FILE *) fopen (fname, "a")) 
				 == (FILE *) NULL)
				fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, 
					fname, D21_LINE) ;
			    DBGPRINTF ((debug_fd, "DSA-SPECIFIC LOG-FILE: %s\n",
				fname)) ;
			}
			/* write log-entry information to specific DSA-file */
			write_log_entry (dsafd, &logentry, FALSE) ;
		    }
		}
	    }
	}

	/* close and remove temporary SHDLOG-file */
	if (tmp_file == TRUE) {
	    fclose (logfd) ;
	    unlink (fname1) ;
	}

	/* close specific DSA file */
	if (dsafd != (FILE *) NULL)
	    fclose (dsafd) ;

	DBGPRINTF ((debug_fd, "------------ End of SHDLOG-file distribution --------------------\n")) ;
    }

    if (op_mode != 2) {
	/* operation mode is 'distribute SHDLOG-file only' */
	DBGPRINTF ((debug_fd, "distribute SHDLOG-file only\n"));
	exit (0) ;
    }

    /* ------------------ DO DELTA UPDATE OPERATIONS ------------------- */

    DBGPRINTF ((debug_fd, "-------------- Start of delta update operations -----------------\n")) ;

    /* handle distributed DSA-specific SHDLOG-files */
    for (i = 0, jobptr = jobtab; i < no_of_jobs; i++, jobptr++) {
	if (jobptr->d2_jdsa_index == D21_HANDLED) {
	    DBGPRINTF ((debug_fd, "JOB-Nr. %d not equal\n", i)) ;
	    continue ;
	}
	/* generate name of DSA-specific file */
	sprintf (fname, "%s%s.%ld", d26_install, D2_DSA_IDX, jobptr->d2_jdsa_index) ;
	DBGPRINTF ((debug_fd, "DSA-SPECIFIC FILE NAME: %s\n", fname)) ;
	if ((dsafd = (FILE *) fopen (fname, "r")) == (FILE *) NULL) {
	    if (errno != ENOENT)
		fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, fname, D21_LINE) ;

	    /* no DSA-specific file available - set corresponding */
	    /* job-table entries to a 'handled'-state */
	    for (j = i + 1, jobptr1 = jobptr + 1; j < no_of_jobs; j++, jobptr1++) {
		if (jobptr1->d2_jdsa_index == jobptr->d2_jdsa_index)
		    jobptr1->d2_jdsa_index = D21_HANDLED ;
	    }
	    jobptr->d2_jdsa_index = D21_HANDLED ;
	    continue ;
	}

	DBGPRINTF ((debug_fd, "DSA-SPECIFIC LOG-FILE HANDLING\n")) ;

	/* handle all entries of the DSA-specific SHDLOG-file */
	dsatmpfd = (FILE *) NULL ;
	dsaindex = jobptr->d2_jdsa_index ;
	result = D2_NOERROR ;
	while (read_log_entry (dsafd, &logentry) == D2_RLOGPAR) {
	    /* search corresponding job-entry */
	    write_tmp_file = FALSE;
	    srchstate = 0 ;
	    for (j = i, jobptr1 = jobptr; j < no_of_jobs; j++, jobptr1++) {
		DBGPRINTF ((debug_fd, "LOG-NAME: %s JOB-Name: %s---\n",
				logentry.d2_lobjname, jobptr1->d2_jobjname));
		DBGPRINTF ((debug_fd, "act-DSA-IDX: %ld ref-IDX: %ld\n",
				jobptr1->d2_jdsa_index, jobptr->d2_jdsa_index));
		DBGPRINTF ((debug_fd, "act-Job-Nr: %d call-Job-Nr: %d\n",
				jobptr1->d2_jnr, job_entry_no));
		nmsize = strlen (jobptr1->d2_jobjname);
		if (jobptr1->d2_jdsa_index == jobptr->d2_jdsa_index &&
		   ((jobptr1->d2_jsubset == OBJECT_SUBORDINATES &&
		   (strcmp (jobptr1->d2_jobjname, D21_ROOT) == 0 ||
		   (strncmp (logentry.d2_lobjname, jobptr1->d2_jobjname, nmsize) == 0 &&
		   (logentry.d2_lobjname[nmsize] == D2_RDN_SEP ||
		   logentry.d2_lobjname[nmsize] == D2_EOS)))) ||
		   (jobptr1->d2_jsubset == OBJECT &&
		   strcmp (logentry.d2_lobjname, jobptr1->d2_jobjname) == 0))) {
		    DBGPRINTF ((debug_fd, "CORRESPONDING JOB ENTRY FOUND\n")) ;
		    if (jobptr1->d2_jtimestamp > logentry.d2_ltimestamp) {
			/* invalid time stamp -> ignore log-entry */
			srchstate = D21_MOBJ | D21_FOBJ ;
			break;
		    }

		    /* generate search command */
		    sprintf (cmdbuf, SEARCHCMD, d26_install, D21_UPD_ERR,
				logentry.d2_ltimestamp, logentry.d2_llpid,
				logentry.d2_lobjname, jobptr1->d2_jdsaname,
				d26_install, D21_UPD_ERR) ;
		    DBGPRINTF ((debug_fd, "SEARCHCMD: %s\n", cmdbuf)) ;
		    if ((errfd = (FILE *) popen (cmdbuf, "r")) == (FILE *) NULL)
			fatal_error_hdl (GDS_S_SUP_SEARCH_ERR, 0, D21_UPD_ERR, 
				D21_LINE) ;

		   /* read search state from pipe */
		   if (fscanf (errfd, "%d", &srchstate) != 1)
			fatal_error_hdl (GDS_S_SUP_SEARCH_STATE_ERR, 0, 
				(char *)NULL, D21_LINE) ;
		    pclose (errfd) ;
		    DBGPRINTF ((debug_fd,"SEARCH STATE: 0x%02x\n", srchstate));
		    if ((srchstate & D21_ACTION) != D21_ACTION)
			/* ignore inactive log-entry */
			break ;

		    upd_t = &jobptr1->d2_upd_t;
		    if (!(jobptr1->d2_jnr == job_entry_no ||
			 ((jobptr1->d2_jstate & ACT_JOB) == ACT_JOB &&
			  (upd_t->upd_t_mode == UPD_HIGH &&
			   act_time->tm_min % upd_t->upd_min == 0) ||
			  (upd_t->upd_t_mode == UPD_MEDIUM &&
			   act_time->tm_hour % upd_t->upd_hour == 0) ||
			  (upd_t->upd_t_mode == UPD_LOW_1 &&
			   act_time->tm_hour == upd_t->upd_1hour &&
			   (act_time->tm_wday == upd_t->upd_1d_of_week ||
			   upd_t->upd_1d_of_week == ALL_POSSIBLE_TIMES)) ||
			  (upd_t->upd_t_mode == UPD_LOW_2 &&
			   ((act_time->tm_hour == upd_t->upd_1hour &&
			     act_time->tm_wday == upd_t->upd_1d_of_week) ||
			    (act_time->tm_hour == upd_t->upd_2hour &&
			     act_time->tm_wday == upd_t->upd_2d_of_week)))))) {
			/* no update required for this log-entry at the moment */
			write_tmp_file = TRUE ;
			break ;
		    }

		    DBGPRINTF ((debug_fd, "UPDATE OPERATION:\n")) ;
		    DBGPRINTF ((debug_fd, "DIR-ID: %d  DSA-NAME: %s\n",
					dir_id, jobptr1->d2_jdsaname)) ;
		    DBGPRINTF ((debug_fd, "OPERATION: %d  MESSAGESIZE: %d\n",
					logentry.d2_lopid, logentry.d2_lmsgsize)) ;
		    if (result == D2_NOERROR) {
			/* do update operation */
			if (d26_i27_get_dn_from_file (FALSE,
				    jobptr1->d2_jdsa_index,
				    (D2_name_string) upd_dsaname) == D26_ERROR)
			    fatal_error_hdl (GDS_S_SUP_DNLIST_REMDSA_ERR, 
				jobptr1->d2_jdsa_index, (char *)NULL, D21_LINE);
			DBGPRINTF ((debug_fd, "UPDATE OPERATION: INDEX: %ld DSA-NAME: %s\n",
						jobptr1->d2_jdsa_index, upd_dsaname)) ;
			result = d20d050_update_dsa (&pbhead, FALSE,
				    dir_id,(D2_name_string)upd_dsaname,
				    logentry.d2_lopid,
				    logentry.d2_lmsg, logentry.d2_lmsgsize) ;

		    }
		    switch ((int)result) {
		    case D2_FATAL_ERR:
			write_tmp_file = TRUE;
			fatal_error_hdl (GDS_S_SUP_UPD_OP_ERR, 0, (char *)NULL,
				D21_LINE) ;
			break ;
		    case D2_NOERROR:
			break ;
		    default: /* e.g. D2_NEXT_DSA */
			write_tmp_file = TRUE;
			if ((srchstate & (D21_MOBJ | D21_FOBJ)) == (D21_MOBJ | D21_FOBJ)) {
			    DBGPRINTF ((debug_fd, "CHANGING OF A UPD_ERR-ENTRY: (timestamp = %ld:%d)\n",
						logentry.d2_ltimestamp, logentry.d2_llpid)) ;

			    /* change available error log-entry */
			    sprintf (cmdbuf, CHANGECMD, logentry.d2_ltimestamp,
						logentry.d2_llpid, act_time->tm_year,
						act_time->tm_mon, act_time->tm_mday,
						act_time->tm_hour, act_time->tm_min,
						pbhead.d2_retcode, pbhead.d2_errclass,
						pbhead.d2_errvalue, d26_install,
						D21_UPD_ERR, d26_install, D21_UPD_ERR,
						d26_install, D21_UPD_ERR, d26_install,
						D21_UPD_ERR) ;
			    DBGPRINTF ((debug_fd, "CHANGE COMMAND: %s\n", cmdbuf)) ;
			    if (system (cmdbuf) != D2_NOERROR)
				fatal_error_hdl (GDS_S_SUP_CHANGE_ERRLOG_ERR, 
					0, (char *)D21_UPD_ERR, D21_LINE) ;
			} else {
			    DBGPRINTF ((debug_fd, "ADDING A NEW SHDERR-ENTRY: (timestamp = %ld:%d)\n",
						logentry.d2_ltimestamp, logentry.d2_llpid)) ;
			    /* write a new error log-entry */
			    sprintf (fname2, "%s%s", d26_install, D21_UPD_ERR) ;
			    if ((errinfofd = (FILE *) fopen (fname2, "a")) ==
								(FILE *) NULL)
				fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, 
					fname2, D21_LINE) ;

			    fprintf (errinfofd, APPENDCMD, logentry.d2_ltimestamp,
						logentry.d2_llpid, D21_UPDMSG, dir_id,
						upd_dsaname, logentry.d2_lobjname,
upd_operation [logentry.d2_lopid > D2_ADM_OPID ? logentry.d2_lopid - D2_ADM_OPID : logentry.d2_lopid],
						act_time->tm_year, act_time->tm_mon,
						act_time->tm_mday, act_time->tm_hour,
						act_time->tm_min,
						pbhead.d2_retcode, pbhead.d2_errclass,
						pbhead.d2_errvalue, 1, 1) ;
			    fclose (errinfofd) ;
			}
			if (result != D2_NEXT_DSA)
	    		    result = D2_NOERROR ;
			break ;
		     }
		    break ;
		}
	    }
	    if (j == no_of_jobs)  {
		/* no corresponding job entry found -> */
		/* remove error log-entry from SHDERR-file */
		srchstate = D21_MOBJ | D21_FOBJ ;
	        DBGPRINTF ((debug_fd, "NO CORRESPONDING JOB ENTRY FOUND\n")) ;
	    }

	    if (write_tmp_file) {
	        /* no corresponding job-entry found for the actual date */
	        /* or update operation failed --> write log-entry to */
	        /* temporary file */
		if (dsatmpfd == (FILE *) NULL) {
		    sprintf (fname1, "%s%s.X%d", d26_install, D2_DSA_IDX, getpid ()) ;
		    if ((dsatmpfd = fopen (fname1, "a")) == (FILE *) NULL)
			fatal_error_hdl (GDS_S_SUP_FOPEN_ERR, errno, fname1, 
				D21_LINE) ;
		}
		/* write log-entry to specific DSA-file */
		write_log_entry (dsatmpfd, &logentry, TRUE) ;
	    } else {
		if ((srchstate & (D21_MOBJ | D21_FOBJ)) == (D21_MOBJ | D21_FOBJ)) {
		    /* remove error log-entry from SHDERR-file */
		    if (d26_i27_get_dn_from_file (FALSE, dsaindex,
				(D2_name_string) upd_dsaname) == D26_ERROR)
			fatal_error_hdl (GDS_S_SUP_DNLIST_REMDSA_ERR, dsaindex,
						(char *)NULL, D21_LINE) ;
		    sprintf (cmdbuf, DELCMD, logentry.d2_ltimestamp,
					logentry.d2_llpid, upd_dsaname,
					d26_install, D21_UPD_ERR,
					d26_install, D21_UPD_ERR, d26_install, D21_UPD_ERR,
					d26_install, D21_UPD_ERR) ;
		    DBGPRINTF ((debug_fd, "DELCMD: %s\n", cmdbuf)) ;
		    if (system (cmdbuf) != D2_NOERROR)
			fatal_error_hdl (GDS_S_SUP_DELENTR_ERRLOG_ERR, 
				0, (char *)D21_FUPD_ERR, D21_LINE) ;
		}
	    }
	}

	/* initiate unbind to the actual update DSA */
	(void)d20d050_update_dsa (&pbhead, TRUE, dir_id, (D2_name_string)NULL,
			    0, (char *)NULL, 0) ;

	/* set all DSA-specific entries of the job-table to a 'handled'-state */
	for (j = i + 1, jobptr1 = jobptr + 1; j < no_of_jobs; j++, jobptr1++) {
	    if (jobptr1->d2_jdsa_index == jobptr->d2_jdsa_index)
		jobptr1->d2_jdsa_index = D21_HANDLED ;
	}
	jobptr->d2_jdsa_index = D21_HANDLED ;

	/* remove DSA-specific SHDLOG-file */
	if (unlink (fname) == D2_ERROR)
	    fatal_error_hdl (GDS_S_UNLINK_ERR, errno, fname, D21_LINE) ;

	/* rename temporary DSA-specific SHDLOG-file if available */
	if (dsatmpfd != (FILE *) NULL) {
	    fclose (dsatmpfd) ;
	    if (link (fname1, fname) == D2_ERROR)
		fatal_error_hdl (GDS_S_LINK_ERR, errno, fname1, D21_LINE) ;

	    if (unlink (fname1) == D2_ERROR)
		fatal_error_hdl (GDS_S_UNLINK_ERR, errno, fname1, D21_LINE);
	    DBGPRINTF ((debug_fd, "relink of the temporary DSA-specific SHDLOG-file (%s -> %s)\n",
								fname1, fname)) ;
	}
    }

    /* initiate unbind to the actual update DSA */
    (void)d20d050_update_dsa (&pbhead, TRUE, dir_id, (D2_name_string)NULL,
			0, (char *)NULL, 0) ;

    /* initiated closing of DN-list file */
    d26_i27_get_dn_from_file (TRUE, 0, (D2_name_string) NULL) ;

    /* release tempory SHDJOBS-buffer */
    if (jstart != (char *) 0)
	free (jstart) ;

    DBGPRINTF ((debug_fd, "-------------- End of delta update distributions ----------------\n")) ;
    dce_svc_printf(GDS_S_EXIT_DAEMON_MSG,dir_id);
    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,svc_c_debug1,
	GDS_S_EXIT_DAEMON,dir_id));

    exit (0) ;
}

/* --------------- read one log-entry from a file ------------------------- */

static signed32 read_log_entry(
  FILE * logfd,
  D2_logentry * logentry)
{
	size_t	rcnt ;
	int	rpar ;

	/* read common log-entry information from file */
	if ((rpar = fscanf (logfd, D2_RSHDLOG_FORM, logentry->d2_lobjname,
	    &logentry->d2_lsupindex, &logentry->d2_ltimestamp, &logentry->d2_llpid,
	    &logentry->d2_lopid, &logentry->d2_lmsgsize)) != D2_RLOGPAR)
		return ((signed32) rpar) ;

	DBGPRINTF ((debug_fd, "LOG-ENTRY:\n")) ;
	DBGPRINTF ((debug_fd, "OBJ-NAME: %s  SUPERIOR-INDEX: %ld\n",
				logentry->d2_lobjname, logentry->d2_lsupindex)) ;
	DBGPRINTF ((debug_fd, "TIME-STAMP: %ld  PID: %d\n",
				logentry->d2_ltimestamp, logentry->d2_llpid)) ;
	DBGPRINTF ((debug_fd, "OP-ID: %d  MSG-LENGTH: %ld\n",
				logentry->d2_lopid, logentry->d2_lmsgsize)) ;

	/* read log-entry message from file */
	if (logentry->d2_lmsgsize > D21_SMSGSZ) {
		/* insufficient memory -> realloc */
		if ((logentry->d2_lmsg = (char *) realloc (logentry->d2_lmsg,
		     logentry->d2_lmsgsize)) == (char *) NULL)
			fatal_error_hdl (GDS_S_NOMEMORY, logentry->d2_lmsgsize, 
			(char *)NULL, D21_LINE) ;

		DBGPRINTF ((debug_fd, "ADDITIONAL MEMORY ALLOCATED\n")) ;
	}

	if ((rcnt = fread (logentry->d2_lmsg, 1, (size_t) logentry->d2_lmsgsize, logfd)) == D2_ERROR)
		fatal_error_hdl (GDS_S_SUP_READ_LOG_ERR, errno, (char *)NULL, 
			D21_LINE) ;

	if (rcnt != (size_t) logentry->d2_lmsgsize)
		fatal_error_hdl (GDS_S_SUP_READ_COMPL_LOG_ERR, 0, (char *)NULL, 
			D21_LINE) ;

	DBGPRINTF ((debug_fd, "LOG-MESSAGE READ FROM FILE (%d Bytes)\n",rcnt));

	return ((signed32) rpar) ;
}

/* ---------------- write a log-entry to a file --------------------------- */

static signed32 write_log_entry(
  FILE * logfd,
  D2_logentry * logentry,
  Bool memmode)
{
	/* write common log-entry information to file */
	fprintf (logfd, D2_WSHDLOG_FORM, logentry->d2_lobjname, 
	         logentry->d2_lsupindex, logentry->d2_ltimestamp,
		 logentry->d2_llpid, logentry->d2_lopid, logentry->d2_lmsgsize) ;
	/* write log-entry message to file */
	if (fwrite (logentry->d2_lmsg, 1, (size_t) logentry->d2_lmsgsize, logfd)
	    != (size_t) logentry->d2_lmsgsize)
		fatal_error_hdl (GDS_S_SUP_WRITE_LOG_ERR, errno, (char *)NULL, 
			D21_LINE) ;

	DBGPRINTF ((debug_fd, "LOG-ENTRY WRITTEN TO FILE (%d Bytes)\n",
							logentry->d2_lmsgsize)) ;

	if (memmode && logentry->d2_lmsgsize > D21_SMSGSZ) {
		/* release additional memory */
		if ((logentry->d2_lmsg = (char *) realloc (logentry->d2_lmsg,
		     D21_SMSGSZ)) == (char *) NULL)
			fatal_error_hdl (GDS_S_NOMEMORY, D21_SMSGSZ, 
				(char *)NULL, D21_LINE) ;
		DBGPRINTF ((debug_fd, "ADDITIONAL MEMORY RELEASED\n")) ;
	}
	return (D2_NOERROR) ;
}

/* -------------- fatal error handling ------------------------------------ */

static signed32 fatal_error_hdl(
  unsigned32 message_id,
  signed32 call_errno,
  char * text,
  signed32 line)
{

#define D20_SVC(args)   gds_svc_handle, __FILE__, line, args, GDS_S_GENERAL

	static	FILE	*ferrfd = (FILE *) NULL ;
	struct	stat	fstate ;
	char	lname[D2_DNL_MAX] ;
	time_t	seconds ;

	/* for usage error directory ID not known and SVC not initialized */
	if (message_id == GDS_S_SUP_USAGE)
	{
	  ferrfd = stderr;
	}
	else	/* open file for update errors in the database directory */
	{
	  if (ferrfd == (FILE *) NULL) {
		/* open the fatal error log-file */
		sprintf (lname, "%s%s", d26_install, D21_FUPD_ERR) ;
		if ((ferrfd = (FILE *) fopen (lname, "a")) == (FILE *) NULL) {
			fprintf (stderr, "FATAL ERROR: %d - can't open file %s\n", errno, lname) ;
			exit(5);
		}
	  }

	  /* check size of log-file */
	  if (fstat (fileno (ferrfd), &fstate) == D2_ERROR) {
		fprintf (stderr, "FATAL ERROR: %d - can't get state of file %s%s\n", errno, d26_install, D21_FUPD_ERR) ;
		exit (6) ;
	  }
	  if (fstate.st_size > D21_MAX_LOG_SIZE) {
		sprintf (lname, "%s%s", d26_install, D21_FUPD_ERR) ;
		if (freopen (lname, "w", ferrfd) == (FILE *) NULL) {
			fprintf (stderr, "FATAL ERROR: %d - can't open file %s\n", errno, lname) ;
			exit (7) ;
		}
	  }
	}

	/* write fatal error message to the log-file */
	seconds = time (0L) ;
	fprintf (ferrfd, D21_0FATFORM, asctime (localtime (&seconds)), line) ;
	if (call_errno != 0) {
		if (text != (char *) NULL) {
			dce_fprintf(ferrfd, message_id, call_errno, text);
			DCE_SVC_LOG((D20_SVC("%s%d"), svc_c_debug1,
				    message_id, text, call_errno));
			dce_svc_printf(D20_SVC("%s%d"), svc_c_sev_fatal,
				       message_id, text, call_errno);
		} else {
			dce_fprintf (ferrfd, message_id, call_errno) ;
			DCE_SVC_LOG((D20_SVC("%d"), svc_c_debug1,
				    message_id, text, call_errno));
			dce_svc_printf(D20_SVC("%d"), svc_c_sev_fatal,
				       message_id, call_errno);
		}
	} else {
		if (text != (char *) NULL) {
			dce_fprintf (ferrfd, message_id, text) ;
			DCE_SVC_LOG((D20_SVC("%s"), svc_c_debug1,
				    message_id, text, call_errno));
			dce_svc_printf(D20_SVC("%s"), svc_c_sev_fatal,
				       message_id, text);
		} else {
			dce_fprintf (ferrfd, message_id) ;
			DCE_SVC_LOG((D20_SVC(""), svc_c_debug1,
				    message_id, text, call_errno));
			dce_svc_printf(D20_SVC(""), svc_c_sev_fatal,
				       message_id);
		}
	}
	fflush (ferrfd) ;
	if (ferrfd != stderr)
	{
	  fclose (ferrfd) ;
	}
	exit (8) ;
}

