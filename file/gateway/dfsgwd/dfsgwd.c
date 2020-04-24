/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: dfsgwd.c,v $
 * Revision 1.1.6.1  1996/10/02  17:50:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:26  damon]
 *
 * Revision /main/HPDCE02/5  1995/06/22  20:14 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * Revision /main/HPDCE02/4  1995/03/30  20:04 UTC  maunsell_c
 * 	CHFts14091 - dfsgwd forks itself when new -background switch used
 * 
 * Revision /main/HPDCE02/4  1995/03/29  16:05 UTC  maunsell_c
 * 	use -background switch to fork vs startup with &
 * 	CHFts14091
 * 
 * Revision /main/HPDCE02/3  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/HPDCE02/maunsell_mothra1.45/3  1995/02/09  21:50 UTC  maunsell_c
 * 	@sys @host audit changes
 * 
 * Revision /main/HPDCE02/maunsell_mothra1.45/2  1995/02/01  17:00 UTC  maunsell_c
 * 	remove version file use
 * 
 * Revision /main/HPDCE02/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision /main/HPDCE02/2  1995/01/30  23:00 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:40 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * Revision /main/HPDCE02/1  1994/11/09  14:04 UTC  maunsell_c
 * 	use dfs_version file
 * 
 * Revision /main/maunsell_mothra1.42/1  1994/10/27  21:42 UTC  maunsell_c
 * 	use dfs version file
 * 
 * Revision 1.1.2.4  1994/09/28  21:22:01  sasala
 * 	bos output incorrect -missing output, OT 12280, S12Y
 * 	[1994/09/28  21:20:14  sasala]
 * 
 * Revision 1.1.2.3  1994/08/22  16:16:19  maunsell_c
 * 	include dcedfs/cmdAux.h
 * 	[1994/08/11  18:08:48  maunsell_c]
 * 
 * 	use defined offsets for parms in dfsgwd_argHandler and main()
 * 	use cmd_IndexedAddParm in dfsgwd main() cmd parsing
 * 	use cmd_IndexedAddParm with defined offsets in dfsgwd main() and dfsgwd_argHandler
 * 	[1994/08/10  16:38:51  maunsell_c]
 * 
 * Revision 1.1.2.2  1994/08/08  18:54:39  mckeen
 * 	Service name and server name are now different.
 * 	Pass both to as_init.
 * 	Added serviceability
 * 	[1994/08/08  17:47:09  mckeen]
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:22  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:57  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:05:39  mckeen]
 * 
 * $EndLog$
 */
/*
 * dfsgwd.c - DFS Remote Authentication Service Daemon.
 *
 * This daemon provides the server end of a mechanism to securley
 * authenticate remote, foreign users of DFS.	When foreign users make
 * requests into DFS, a mapping is performed, translating the user's  
 * network address and user ID into a PAG:
 *
 *		(Netaddr, UserID) => PAG
 * 
 * This mapping is set up by this server, in co-operation with the
 * corresponding client.  What actually happens is that the client
 * is running straight kerberos V5, and using the kdc gateway, obtains
 * a TGT.  The client then gets a service ticket for this server (again
 * using the kdc gateway), and after authenticating with the server, 
 * securely transfers the TGT, network ID, and User ID.	 The server
 * then installs the TGT into DCE security, yielding a PAG.  The last
 * step is to simply create the relationship using special (AFS) syscalls.
 *
 * This daemon is multithreaded to allow simultaneous processing of 
 * several requests.
 *
 * Usage: dfsgwd [ -file <logfile> ] [ -service <dfsgwd_service> ] [ -sysname <name> ]  [ -nodomains ] [ -verbose ]
 *
 * This module provides daemon startup and subsystem initialization.
 */
#include <dce/dce.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <errno.h>

#include <dcedfs/stds.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgwd/dfsgwd.c,v 1.1.6.1 1996/10/02 17:50:12 damon Exp $")

#ifdef AFS_SUNOS5_ENV
#undef MIN
#undef MAX
#endif

#include <dcedfs/osi.h>
#include <dcedfs/compat.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dfsgw.h>
#include <dfsgwd.h>
#include <as.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgwymac.h>
#include <dfsgwysvc.h>
#include <dfsgwymsg.h>

/*
 * Global data.
 */
char *progname;			/* Program name */
int debug=0;			/* Debugging flag */
int in_background = 0;              /* we need to fork */
char *logfile = NULL;
char *service = DFSGW_SERVICE;
char *server = DFSGW_SERVER;
char *sysname = "";           /* switch to set @sys value to unknown */
int basehostname = 0;               /* hostname w/o domain part */

/*
 * Local data.
 */
static FILE *lfs;			/* Log file stream */
static pthread_mutex_t loglock;

DCE_SVC_DEFINE_HANDLE(gwy_svc_handle, gwy_svc_table, "gwy")

void initialize_svc()
{
  error_status_t st;


  gwy_svc_handle = dce_svc_register(gwy_svc_table,  (idl_char *)"gwy",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dce_msg_define_msg_table(gwy__table,
        sizeof gwy__table / sizeof gwy__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register gwy incore message table 0x%lx\
n", st);
}


/*
 * dfsgwd_argHandler
 *	dfsgwd arg handler. Sets global variables based on the
 *	user supplied arguments
 */

int
dfsgwd_argHandler(as, arock)
    struct cmd_syndesc *as;
    char *arock;
{
    long code = 0;

    /*
     * Collect arguments.
     */
    if (as->parms[SERVICE_OFFSET].items) {
        service = strdup(as->parms[SERVICE_OFFSET].items->data);
    }

    if (as->parms[LOGFILE_OFFSET].items) {
        logfile = strdup(as->parms[LOGFILE_OFFSET].items->data);
    }

    if (as->parms[SYSNAME_OFFSET].items) {
        sysname = strdup(as->parms[SYSNAME_OFFSET].items->data);
    }

    if (as->parms[DOMAINS_OFFSET].items) {
        basehostname++;
    }

    if (as->parms[VERBOSE_OFFSET].items) {
        debug++;
    }

    if (as->parms[BGROUND_OFFSET].items) {
        in_background++;
    }

    if (!logfile) {
        if (logfile = malloc(strlen(dcelocal_path)+strlen(LOGPATH)+strlen(LOGFILE)+1)) {
            sprintf(logfile, "%s%s%s", dcelocal_path , LOGPATH, LOGFILE);
        }
    }
    if (!logfile || log_init(logfile) < 0) {
        dce_svc_printf(GWY_S_LOGINITERR_MSG, progname);
    }
    log("Log file initialized as %s", logfile);

    return(code);
}

/*
 * log_init - Initialize log operations.
 *
 * If a logfile exists already, it is moved away. 
 *
 * This routine actually calls internal_log_init to do the real work.
 * pthread_once is there to make it thread-safe.
 *
 * Inputs:
 *	logfile		: Name of the log.
 * Return value:
 *	-1 on failure, 0 otherwise.
 */	
int
log_init
#ifdef __STDC__
	(char *logfile)
#else
	(logfile)
	char *logfile;
#endif
{
	char *newname = 0;
	char *suffix = ".old";
	int status = 0;

	if (pthread_mutex_init(&loglock, pthread_mutexattr_default) == -1) { 
		status = -1;
		goto fail;	  
	}
	if (access(logfile, F_OK) == 0) {
		/*
		 * File exists.  Move it away.
		 */
		if ((newname = malloc(strlen(logfile) + strlen(suffix))) != NULL) {
			strcpy(newname, logfile);
			strcat(newname, suffix);
			if (rename(logfile, newname) < 0) {
				status = -1;
				goto fail;
			}
		} else {
			status = -1;
			goto fail;
		}
	}
	if ((lfs = fopen(logfile, "wb")) == NULL) {
		status = -1;
	}
fail:
	if (newname) {
		free(newname);
	}
	return(status); 
}

/*
 * log_close - Stop logging.
 *
 * This routine closes the log.
 *
 * Return value:
 *	void
 */
void log_close()
{
	fclose(lfs);
}

/*
 * log - Add message to log.
 *
 * This routine adds an entry to the log, using a printf(3)
 * style syntax.
 *
 * Inputs:
 *	fmt		: printf-like format string.
 *      va_alist	: variable length argument list.
 * Return value:
 *	-1 on failure, 0 otherwise
 */
int log
#ifdef __STDC__
	(char *fmt, ...)
#else
	(fmt, va_alist)
	char *fmt;
	va_dcl
#endif 
{
	int status = 0;
	time_t tloc;
	char *x;
	va_list ap;

	if (lfs) {
		/*
		 * Single thread logging so things don't look weird.
		 */
		pthread_mutex_lock(&loglock);
		time(&tloc);
		x = ctime(&tloc);
		x[24] = '\0';
		fprintf(lfs, "%s: ", x);
		va_start(ap, fmt);
		vfprintf(lfs, fmt, ap);
		fprintf(lfs, "\n");
		fflush(lfs);
		va_end(ap);
		pthread_mutex_unlock(&loglock);
	} else {
		status = -1;
	}	
	return(status);
}

/*
 * fatal - Shut down daemon.
 *
 * This routine writes a final message to the log before
 * terminating the daemon.
 *
 * Return value:
 *	DOES NOT RETURN.
 */
fatal()
{
	log("fatal error - %s exiting", progname);
	log_close();
	exit(1);
}	

/*
 * main - Entry point for dfsgwd.
 *
 * Here we simply parse out arguments and start subsystems before 
 * starting the daemon.
 */
main
#ifdef __STDC__
	(int argc, char **argv)
#else
	(argc, argv)
	int argc;
	char **argv;
#endif
{
	int fd, i;
        long code;
	extern char *optarg;
	extern int optind, optopt;
        struct cmd_syndesc *ts;

	initialize_svc();

	if (progname = strrchr(argv[0], '/'))
	    progname++;
	else
	    progname = argv[0];

        ts = cmd_CreateSyntax((char *) 0, dfsgwd_argHandler, (char *) 0, "dfsgwserver");
        cmd_IndexedAddParm(ts, "-service", CMD_SINGLE, CMD_OPTIONAL, "service number", SERVICE_OFFSET);
        cmd_IndexedAddParm(ts, "-file", CMD_SINGLE, CMD_OPTIONAL, "log_file", LOGFILE_OFFSET);
        cmd_IndexedAddParm(ts, "-sysname", CMD_SINGLE, CMD_OPTIONAL, "sysname", SYSNAME_OFFSET);
        cmd_IndexedAddParm(ts, "-nodomains", CMD_FLAG, CMD_OPTIONAL, "no domain hostname", DOMAINS_OFFSET);
        cmd_IndexedAddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL, "verbose", VERBOSE_OFFSET);
        cmd_IndexedAddParm(ts, "-background", CMD_FLAG, CMD_OPTIONAL, "background", BGROUND_OFFSET);

        code = cmd_Dispatch(argc, argv);
        if (code) {
            dce_svc_printf(GWY_S_DISPATCHERR_MSG, progname, dfs_dceErrTxt(code));
            exit(1);
        }

        /*
         * Check if help was requested, and if so, quit cleanly
         */
        for (i=0; i < argc; i++) {
            if (strncmp(argv[i], "-help", strlen(argv[i])) == 0)
                exit(0);
        }
        if (in_background) {
           /* fork daemon here, no more running dfsgwd & 
            * makes dce_config start/stop unhappy
            * CHFts14091
            *   Starting dfsgwd by using the daemonrunning call in /etc/rc.dfs AND placing
            *   that call in background is the culprit for this defect.  Daemons typically
            *   should not need to be started in background.  Doing so with the daemonrunning
            *   call not only leaves /etc/rc.dfs running around in background, but causes
            *   the err_exit as described in this defect.
            */
            if (debug) {
               dce_svc_printf(GWY_S_FORKINGDAEMON_MSG);
            }
            code = fork();
            if (code > 0) {
               _exit(0);
            } else if (code < 0) {
               dce_svc_printf(GWY_S_FORK_FAILED_MSG, code, errno);
               exit(1);
            }
            /* Child */

#ifndef HPDCE_FIX_OT6076
            /*
             * This is dfsgwd's instance of this fix, lifted from rpcd.c,
             * version 1.1.12.8.  Note: HPDCE_FIX_OT6076 is a tag for
             * this fix.
             */
            if (setsid() == -1) {
               dce_svc_printf(GWY_S_BAD_SETSID_MSG, strerror(errno));
            }
            if ((fd = open("/dev/null", O_RDWR,
                           S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) != -1)
               {
                   (void)dup2(fd,0);
                   (void)dup2(fd,1);
                   (void)dup2(fd,2);
                   (void)close(fd);
            }

            /* 
             * also stdout and stderr need to goto the logfile  
             *
             */
            if ((fd = open(logfile, O_WRONLY | O_CREAT | O_APPEND, 0644)) >= 0) {
	        (void)dup2(fd, 1);
	        (void)dup2(fd, 2);
	        (void)close(fd);
            }
#endif  /* HPDCE_FIX_OT6076 */
         }
        /* end of CHFts14091 fix */
	/*
	 * Initialize networking.  Upon error just bail out - there may
	 * be another server running so inform the user of this.
	 */
	if (as_init(service,server) < 0) {
                dce_svc_printf(GWY_S_NETINITERR_MSG, progname); 
		exit(1);
	}
        log("DFS Gateway (dfsgwd) server Protocol Version %d starting", DFSGW_PROTOCOL_VERSION);
        
	/*
	 * Now just run the daemon.
	 */
	if (as() < 0) {
		log("Error while running authentication service");
	} else {
		log("Normal shutdown");
	}
	log_close();
	exit(0);
}
