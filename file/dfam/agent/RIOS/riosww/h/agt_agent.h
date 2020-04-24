/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
# ifndef RCS_agt_agent_h_Included
# define RCS_agt_agent_h_Included
 static char *RCSID_agt_agent_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_agent.h,v 1.1.2.2 1996/03/11 14:05:57 marty Exp $";
# endif /* Not RCS_agt_agent_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_agent.h,v $
 * Revision 1.1.2.2  1996/03/11  14:05:57  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:45  marty]
 *
 * Revision 1.1.2.1  1996/02/19  10:08:08  dce-sdc
 * 	First Submit
 * 	[1996/02/19  10:07:45  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  05:22:47  05:22:47  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:22:35  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  01:40:59  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:40:29  kishi]
 * 
 * Revision 1.1.2.2  1995/10/25  01:47:40  takano
 * 	Add D_EXIT_PERMIT.
 * 	[1995/10/25  01:47:08  takano]
 * 
 * Revision 1.1.2.1  1995/09/09  07:43:36  kishi
 * 	Initial revision
 * 	[1995/09/09  07:14:35  kishi]
 * 
 * $EndLog$
 */

#ifndef _AGT_AGENT_H
#define _AGT_AGENT_H

#define  ERRMSG(x)     fprintf(stderr, x);

#include <unistd.h>
#include <time.h>
#include <nl_types.h>
#include <pthread.h>

#define D_UNIT_MB           1048576
#define D_UNIT_KB           1024

#define D_TRUE              1
#define D_FALSE             0

#define D_MAX_HOSTNAME_LEN  128

#define D_WORK_DIR          "/"

#define D_THREAD_MAX        3
#define D_THREAD_ID_MASK    0x000000ff
#define D_THREAD_MAIN       0
#define D_THREAD_USR1       1
#define D_THREAD_SIGNALS    2

#define D_EXIT_OK           0
#define D_EXIT_NOAGENT      1
#define D_EXIT_INIT         2
#define D_EXIT_ALREADY      3
#define D_EXIT_OUTOFRSC     4
#define D_EXIT_NETWORK      5
#define D_EXIT_INVPARM      6
#define D_EXIT_INPROG       7
#define D_EXIT_NODFSD       8
#define D_EXIT_PERMIT       9
#define D_EXIT_UNKNOWN      99


#define D_DFS_CHK_INTERVAL 15
#define D_DFS_CHK_PS        "/bin/ps"
#define D_DFS_CHK_PS_OPT    "-ef"
#define D_DFS_CHK_STRING    "dfsd"

#define D_IPC_ACCESS_MODE   0660

#define D_PTYP_MAIN         "MAIN"
#define D_PTYP_TRAC         "TRAC"
#define D_PTYP_WCHD         "WCHD"
#define D_PTYP_SYSC         "SYSC"
#define D_PTYP_USRM         "USRM"
#define D_PTYP_USRC         "USRC"
#define D_PTYP_1ST_CONN     D_PTYP_USRM

#define D_FUNC_STARTAGT     1
#define D_FUNC_STOPAGT      2
#define D_FUNC_ABORTAGT     3
#define D_FUNC_SNDMESG      4
#define D_FUNC_PRMEMTR      5

#define D_MAN_ST_INIT       0x00010000
#define D_MAN_ST_RUN        0x00040000
#define D_MAN_ST_STOP       0x00080000
#define D_MAN_ST_ABRT       0x00088000
#define D_MAN_ST_LGP        0x00041000
#define D_MAN_ST_EXIT	    0x00042000
#define D_MAN_ST_LRK        0x00044000
#define D_MAN_ST_LGOUT      0x00046000
#define D_MAN_ST_FA         0x00048000
#define D_MAN_ST_USEX       0x00081000

#define TRACEERR	1
#define TRACECOM	3
#define TRACEDEB	5

#define AgentTrace(a,b,c,d,e)	{if ((a) <= agt.traceflag)\
					Agenttrace((b),(c),(d),(e));}

struct d_thread_info {
    pthread_t handle;
    int thread_id;
};

struct d_sysc {
    struct d_sysc *next;
    char tableid[4];
    long status;
    long ipaddr;
    unsigned short portno;
    char ptype[4];
    long conn_no;
    pid_t pid;
    int  exitstatus;
    struct timeval time_signaled;
    long gw_conn_no;
};

struct d_maintbl {
    char tableid[4];
    long agt_status;
    long num_proc;
    pid_t mainpid;
    pid_t tracpid;
    pid_t syscpid;

    struct d_sysc *iusysctop;
    struct d_sysc *iusysclst;
    struct d_sysc *frsysctop;
    struct d_sysc *frsysclst;
    long num_sysc_segments;
    long num_sysc;
    long num_dead_sysc;

    long evtr_size;
    long evtr_files;
    long evtr_curfil;
    long evtr_p;
    long memtr_size;
    char *memtr_buf;
    long memtr_p;
};

struct d_abort_info {
    char modname[16];

    long location;
    long info1;
};

struct d_agt {
    char tableid[4];
    char proctype[4];

    pid_t mainprocpid;

    pid_t ownpid;
    long status;
    long event;

    struct d_maintbl *main;

    struct d_abort_info abinfo;

    int semid;
    int shmid_m;
    int shmid_m2;
    int shmid_t;

    char *ipcpath;

    char *agenthostname;

    char *agent_id;

    char *statfilename;
    int statfilefd;

    nl_catd catd;
    int traceflag;
};

extern struct d_agt agt;
extern struct d_thread_info thread_info[D_THREAD_MAX];
extern pthread_mutex_t sig_mutex;
extern pthread_key_t thr_id;

extern int datasize;

extern void agt_bare_exit(int status);
extern void agt_exit(int status);
extern void agt_abort(void);

#ifdef _DEBUG

#define DEBUG_SCRIPT_FILE_M "/usr/bin/DFAM.SIG.NOTIFY_M"
#define DEBUG_SCRIPT_FILE_S "/usr/bin/DFAM.SIG.NOTIFY_S"

extern int check_debug_loop(void);
extern void debug_forever(void);
extern void debug_wait_signal(void);
#endif


#endif
