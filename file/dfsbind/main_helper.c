/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * HISTORY
 * $Log: main_helper.c,v $
 * $EndLog$
 */

/*
 * Copyright (C) 1996, 1991 Transarc Corporation All rights reserved.
 */


/*
 * main_helper.c
 * 
 * A user helper process is to service any requests from kernel; It helps CM,
 * the cache manager, to resolve pathnames with CDS. It gets authenticated
 * info of a particular user for the kernel rpc runtime.
 */


#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dce/ker/krpc_helper.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <dcedfs/dfsauth.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsdsbmac.h>
#include <dfsdsbsvc.h>
#include <dfsdsbmsg.h>
#include <pthread.h>
#include <dcedfs/icl.h>
#include <bind_trace.h>

#include <syslog.h>

struct icl_set         *bind_iclSetp = 0;

dce_svc_handle_t   dsb_svc_handle;

static void initialize_svc()
{
  error_status_t st;

  dsb_svc_handle = dce_svc_register(dsb_svc_table,  (idl_char *)"dsb",  &st);
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

  dfs_define_dsb_message_tables();

}

/* define argument indices */
#define DFSBIND_ARG_EXPRESS	0
#define DFSBIND_ARG_REGULAR	1
#define DFSBIND_ARG_JUNCTION	2
#define DFSBIND_ARG_PREFIX	3
#define DFSBIND_ARG_NOTFOUND	4
#define DFSBIND_ARG_DEBUG	5

/* The constant DFSBIND_BUFSIZE determines how large
 * BOTH user space and kernel space helper buffers will be.
 * This value gets passed down to the kernel at config time,
 * when dfsbind configures the helper subsystem.  This size
 * will also determine how much auth info the subsystem can
 * handle - this becomes important for delegation, since auth information
 * exceeding this limit will cause the rpc to fail - so choose
 * your value wisely.
 */
#define DFSBIND_BUFSIZE		4096
#define TTLHIT_DEFAULT          24*60*60   /* 1 day */
#define TTLMISS_DEFAULT         3600       /* 1 hour */
#define TTLDIR_DEFAULT          86400      

#define NTHREADS                4	/* default # of threads per pool */
#define DEFAULT_EXPRESS_PROCS	NTHREADS
#define DEFAULT_REGULAR_PROCS	NTHREADS

#define AUTH_MAX_OPCODE 11


/*
 * Dfsbind now has multiple threads of execution and it also provides
 * some simplistic prioritization of requests.  We basically have
 * two priorities: high priority and no priority.  High priority
 * requests consist of the sec_krb_build_msg and sec_krb_decode_msg
 * operations since these are upcalls from krpc's lthread.  All others
 * fall into the no priority category.  There is a dedicated thread
 * pool servicing each queue.
 *
 * The kernel-level krpc_helper code actually ensures that the nopriority
 * requests do not consume the entire request queue, therefore a shared
 * free pool in user space does not present any problems.
 *
 * The helper process will initialize the kernel queues to be the same
 * size as the combined sizes of the user-space high and no priority 
 * queues.  The kernel krpc initialization is a 3-step process:
 *
 *     1.  open the helper device to get
 *         a file descriptor for the krpc_helper subsystem.
 *         This will also perform any one-time initializations
 *         for the krpc_helper subsystem.
 *     
 *     2.  Configure the kernel request queue based and buffers based
 *         on the number of user-space threads; kernel buffer size is 
 *         specified at config time via dfsbind. This is done via an
 *         ioctl (KRPCH_CONFIGURE); (currently this a one-time
 *         configuration - we don't reconfigure across instantiations
 *         of dfsbind).
 *     
 *     3.  Enable the kernel krpc_helper subsystem.  This ioctl
 *         operation (KRPC_ENABLE) tells krpc_helper clients that
 *         the subsystem is now able to process requests.
 *
 */


#define NQUEUES         3     /* one high-priority, and one for the rest */
#define HIGHPRI         0     /* high priority request queue */
#define NOPRI           1     /* other queue */
#define LOWPRI          2     /* bind queue */

/* definition for our very-low-priority type of request */
#define	LOW_PRIORITY(opc) ((opc) == 0)


struct krpch_config krpch_config_params;

extern do_auth_request(), do_GetBind(), do_InitAuth();


/*
 * Imported functions and variables
 */
extern unsigned long    ttlmiss,
                        ttlhit,
                        ttldir;
extern                  do_auth_request(), do_GetBind(), do_InitAuth();

#ifdef	_EPISODE_LIC_MGMT_
extern void             licenseEpisode();
extern int              checkEpisodeLicense();
extern int              episodeInUse();
extern int		checkPXLicense();
extern int		exportingAggregates();

#endif

/*
 * Global data
 */
int                     hpth = NTHREADS,
                        lpth = NTHREADS,
                        Debugging,
                        fd;

#ifndef DCE1_0_2A
char                    krpc_ports[KRPC_PORT_STRING_SIZE * 2];

#endif


struct upcall_request {
    struct upcall_request  *next;
    long                    opcode,
                            inSize,
                            outSize;
    long                    tid;
    char                    inbuf[DFSBIND_BUFSIZE];
    char                    outbuf[DFSBIND_BUFSIZE];
};

struct queue_head {
    struct upcall_request  *head,
                           *tail;
    pthread_mutex_t         qlock;
    pthread_cond_t          wcond;
    int			threadCount;
    int			queueCount;
    char		*queueLabel;
};

struct queue_head       work_queue[NQUEUES];

/*****************************************************************************/

#define EMPTY(q) ((q)->head == NULL)

/*****************************************************************************/

#define ENQUEUE(q, item) \
{ \
    if (EMPTY(q)) \
        (q)->head = (q)->tail = item; \
    else { \
        (q)->tail->next = item; \
        (q)->tail = item; \
    } \
    (q)->tail->next = NULL; \
}

/*****************************************************************************/

#define DEQUEUE(q, item)\
{\
    item = (q)->head; \
    if ((q)->head->next == NULL) \
        (q)->head = (q)->tail = NULL; \
    else \
        (q)->head = (q)->head->next; \
}

/*****************************************************************************/

static void
SetupLog()
{
    struct icl_log         *logp;
    long                    code;

    code = icl_CreateLog("bind", 60 * 1024, &logp);
    if (code == 0)
	code = icl_CreateSet("bind", logp, (struct icl_log *) 0,
			     &bind_iclSetp);
    if (code)
	dce_svc_printf(DSB_S_NO_INIT_ICL_MSG,code);
}

/*****************************************************************************/

static void
ProcessRequest(req)
    struct upcall_request  *req;
{
    char                    buf[DFSBIND_BUFSIZE];
    long                    code;

    /*
     * do_GetBind destroys the contents of inStr (passed in as the the first
     * arguement).  Therefore we make a local copy of req->inbuf and pass
     * that to do_GetBind so that we can use the intact value of req->inbuf
     * later
     */

    bcopy(req->inbuf, buf, req->inSize);
    /*
     * Get opcode and call appropriate handler leave a long word and the
     * start of the output buffer for the transaction id.
     */
    if (req->opcode == 0) {
	/*
	 * Get the cell and related info from CDS
	 */
	(void) do_GetBind(buf + sizeof(long), req->inSize - sizeof(long),
			  req->outbuf + sizeof(long), &(req->outSize));
	req->outSize += sizeof(long);
    } else if (req->opcode > 0 && req->opcode <= AUTH_MAX_OPCODE) {
	icl_Trace0(bind_iclSetp, BIND_TRACE_AUTH_REQUEST_ENTRY);
	(void) do_auth_request(buf + sizeof(long), req->inSize - sizeof(long),
			       req->outbuf + sizeof(long), &(req->outSize));
	req->outSize += sizeof(long);
	bcopy(req->outbuf + sizeof(long), (char *) &code, sizeof(long));
	icl_Trace1(bind_iclSetp, BIND_TRACE_AUTH_REQUEST_EXIT,
		   ICL_TYPE_LONG, code);
    } else {
	icl_Trace1(bind_iclSetp, BIND_TRACE_INVALID_OPCODE,
		   ICL_TYPE_LONG, req->opcode);
	code = EINVAL;
	bcopy((char *) &code, req->outbuf + sizeof(long), sizeof(long));
	req->outSize = 2 * sizeof(long);
    }
}

/*****************************************************************************/

static void
send_response(req, tid)
    struct upcall_request  *req;
    long tid;

/* send the response to the kernel */
{
    long                    code;

    bcopy((char *) &tid, req->outbuf, sizeof(long));
    icl_Trace0(bind_iclSetp, BIND_TRACE_HELPER_WRITE_ENTRY);
    code = write(fd, req->outbuf, req->outSize);
    icl_Trace1(bind_iclSetp, BIND_TRACE_HELPER_WRITE_EXIT,
	       ICL_TYPE_LONG, code);
    if (code != req->outSize) {
	icl_Trace4(bind_iclSetp, BIND_TRACE_HELPER_WRITE_EXIT_2,
		   ICL_TYPE_LONG, errno,
		   ICL_TYPE_LONG, tid,
		   ICL_TYPE_LONG, req->opcode,
		   ICL_TYPE_LONG, req->outSize);
	if (Debugging)
		dce_svc_printf(DSB_S_NO_RESPONSE_SENT_MSG,code,errno);
    }
}

/*****************************************************************************/

static void
condense_work(q, req)
    struct queue_head      *q;
    struct upcall_request  *req;

{
    struct queue_head       save_queue,	/* to be placed back in q */
                            cond_queue;	/* to be answered now */
    struct upcall_request  *current;
    int saveCount;

    save_queue.head = save_queue.tail = NULL;
    cond_queue.head = cond_queue.tail = NULL;
    saveCount = 0;
    pthread_mutex_lock(&(q->qlock));
    while (!EMPTY(q)) {
	DEQUEUE(q, current);
	/* Compare requests to see if they are the same */
	if (req->inSize != current->inSize ||
		bcmp(req->inbuf + sizeof(long),
		     current->inbuf + sizeof(long),
		     req->inSize - sizeof(long))) {
	    ++saveCount;
	    ENQUEUE(&save_queue, current);
	} else {
	    ENQUEUE(&cond_queue, current);
	}
    }
    q->head = save_queue.head;
    q->tail = save_queue.tail;
    if (q->queueCount != saveCount) {
	icl_Trace4(bind_iclSetp, BIND_TRACE_HELPER_CONDENSEWORK,
		   ICL_TYPE_LONG, req->tid,
		   ICL_TYPE_STRING, q->queueLabel,
		   ICL_TYPE_LONG, q->queueCount,
		   ICL_TYPE_LONG, saveCount);
	q->queueCount = saveCount;
    }
    pthread_mutex_unlock(&(q->qlock));
    while (!EMPTY(&cond_queue)) {
	DEQUEUE(&cond_queue, current);
	icl_Trace1(bind_iclSetp, BIND_TRACE_CONDENSE,
		   ICL_TYPE_LONG, current->tid);
	send_response(req, current->tid);
	free(current);
    }
}

/*****************************************************************************/

static struct upcall_request  *
remove_work(q)
    struct queue_head      *q;
{
    struct upcall_request  *req;

    pthread_mutex_lock(&(q->qlock));
    while (EMPTY(q)) {
	pthread_cond_wait(&(q->wcond), &(q->qlock));
    }
    --(q->queueCount);
    DEQUEUE(q, req);
    icl_Trace3(bind_iclSetp, BIND_TRACE_HELPER_REMOVEWORK,
	       ICL_TYPE_LONG, req->tid,
	       ICL_TYPE_STRING, q->queueLabel,
	       ICL_TYPE_LONG, q->queueCount);
    pthread_mutex_unlock(&(q->qlock));

    return req;
}

/*****************************************************************************/

static void
service_thread(addr)
    pthread_addr_t          addr;
{
    struct queue_head      *q;
    struct upcall_request  *req;
    unsigned32              clocktime;
    long                    code;

    q = (struct queue_head *) addr;

    for (;;) {
	req = remove_work(q);
	icl_Trace2(bind_iclSetp, BIND_TRACE_PROCESSREQUEST_ENTRY_3,
		   ICL_TYPE_LONG, req->opcode, ICL_TYPE_LONG, req->tid);
	clocktime = (unsigned32) osi_Time();
	ProcessRequest(req);
	clocktime -= (unsigned32) osi_Time();
	bcopy(req->outbuf + sizeof(long), (char *) &code, sizeof(long));
	icl_Trace2(bind_iclSetp, BIND_TRACE_PROCESSREQUEST_EXIT_2,
		   ICL_TYPE_LONG, ((long) -((signed32) clocktime)),
		   ICL_TYPE_LONG, code);
	send_response(req, req->tid);
	condense_work(q, req);
	free(req);
    }
}

/*****************************************************************************/

static void
add_work(q, req)
    struct queue_head      *q;
    struct upcall_request  *req;
{
    pthread_mutex_lock(&(q->qlock));
    ENQUEUE(q, req);
    ++(q->queueCount);
    icl_Trace3(bind_iclSetp, BIND_TRACE_HELPER_ADDWORK,
	       ICL_TYPE_LONG, req->tid,
	       ICL_TYPE_STRING, q->queueLabel,
	       ICL_TYPE_LONG, q->queueCount);
    pthread_cond_signal(&(q->wcond));
    pthread_mutex_unlock(&(q->qlock));
}

/*****************************************************************************/

static void traceEntry(struct upcall_request *req)
{
    char *bitp, *ptr;
    signed32 a, b, c, d, len;

    icl_Trace3(bind_iclSetp, BIND_TRACE_DISPATCH_1,
	       ICL_TYPE_LONG, req->tid,
	       ICL_TYPE_LONG, req->opcode,
	       ICL_TYPE_LONG, req->inSize);
    bitp = (char *)&req->inbuf[2*sizeof(signed32)];
    switch (req->opcode) {
	case 0:
	    memcpy(&a, bitp, sizeof(a)); bitp += sizeof(a);
	    icl_Trace2(bind_iclSetp, BIND_TRACE_DISPATCH_2,
		       ICL_TYPE_LONG, a,
		       ICL_TYPE_STRING, bitp);
	    break;
	case 3:
	    memcpy(&a, bitp, sizeof(a)); bitp += sizeof(a);
	    memcpy(&b, bitp, sizeof(b)); bitp += sizeof(b);
	    icl_Trace2(bind_iclSetp, BIND_TRACE_DISPATCH_3,
		       ICL_TYPE_LONG, a,
		       ICL_TYPE_LONG, b);
	    memcpy(&len, bitp, sizeof(len)); bitp += sizeof(len);
	    ptr = bitp; ptr[len-1] = 0; bitp += len;
	    memcpy(&a, bitp, sizeof(a)); bitp += sizeof(a);
	    memcpy(&b, bitp, sizeof(b)); bitp += sizeof(b);
	    icl_Trace3(bind_iclSetp, BIND_TRACE_DISPATCH_4,
		       ICL_TYPE_STRING, ptr,
		       ICL_TYPE_LONG, a,
		       ICL_TYPE_LONG, b);
	    break;
	case 7:
	    memcpy(&len, bitp, sizeof(len)); bitp += sizeof(len);
	    ptr = bitp; ptr[len-1] = 0; bitp += len;
	    icl_Trace1(bind_iclSetp, BIND_TRACE_DISPATCH_11,
		       ICL_TYPE_STRING, ptr);
	    break;
	case 8:
	    icl_Trace0(bind_iclSetp, BIND_TRACE_DISPATCH_10);
	    break;
	case 9:
	    icl_Trace0(bind_iclSetp, BIND_TRACE_DISPATCH_9);
	    break;
	case 10:
	    memcpy(&a, bitp, sizeof(a)); bitp += sizeof(a);
	    memcpy(&b, bitp, sizeof(b)); bitp += sizeof(b);
	    memcpy(&c, bitp, sizeof(c)); bitp += sizeof(c);
	    memcpy(&d, bitp, sizeof(d)); bitp += sizeof(d);
	    icl_Trace4(bind_iclSetp, BIND_TRACE_DISPATCH_5,
		       ICL_TYPE_LONG, a,
		       ICL_TYPE_LONG, b,
		       ICL_TYPE_LONG, c,
		       ICL_TYPE_LONG, d);
	    memcpy(&a, bitp, sizeof(a)); bitp += sizeof(a);
	    memcpy(&b, bitp, sizeof(b)); bitp += sizeof(b);
	    memcpy(&c, bitp, sizeof(c)); bitp += sizeof(c);
	    memcpy(&len, bitp, sizeof(len)); bitp += sizeof(len);
	    ptr = bitp; ptr[len-1] = 0; bitp += len;
	    icl_Trace4(bind_iclSetp, BIND_TRACE_DISPATCH_6,
		       ICL_TYPE_LONG, a,
		       ICL_TYPE_LONG, b,
		       ICL_TYPE_LONG, c,
		       ICL_TYPE_STRING, ptr);
	    break;
	case 11:
	    icl_Trace0(bind_iclSetp, BIND_TRACE_DISPATCH_7);
	    break;
	default:
	    icl_Trace0(bind_iclSetp, BIND_TRACE_DISPATCH_8);
	    break;
    }
}

/*****************************************************************************/

static void
dispatch_thread()
{
    struct upcall_request  *req;
    struct timespec         interval;
    int	queueId;

    for (;;) {
	while (!(req = (struct upcall_request *) malloc(sizeof(struct upcall_request)))) {
	    icl_Trace0(bind_iclSetp, BIND_TRACE_MEMORY_WAIT);
	    if (Debugging)
		dce_svc_printf(DSB_S_DISPATCH_WAIT_MSG);
	    interval.tv_sec = 15;
	    interval.tv_nsec = 0;
	    (void) pthread_delay_np(&interval);
	}
	bzero((char *) req, sizeof(struct upcall_request));
	icl_Trace0(bind_iclSetp, BIND_TRACE_HELPER_READ_ENTRY);
	req->inSize = read(fd, req->inbuf, DFSBIND_BUFSIZE);
	icl_Trace1(bind_iclSetp, BIND_TRACE_HELPER_READ_EXIT,
		   ICL_TYPE_LONG, req->inSize);
	if (req->inSize < 2 * sizeof(long)) {
	    interval.tv_sec = 5;
	    interval.tv_nsec = 0;
	    icl_Trace2(bind_iclSetp, BIND_TRACE_HELPER_READ_EXIT_2,
		       ICL_TYPE_LONG, errno, ICL_TYPE_LONG, interval.tv_sec);
	    if (Debugging)
		dce_svc_printf(DSB_S_NO_HELPER_REQ_MSG,req->inSize,
			errno);
	    (void) pthread_delay_np(&interval);
	    continue;
	}
	/* extract transaction id */
	bcopy(req->inbuf, (char *) &(req->tid), sizeof(long));
	bcopy(req->inbuf + sizeof(long),
	      (char *) &(req->opcode), sizeof(long));
	if (LOW_PRIORITY(req->opcode)) {
	    /* A name-lookup request */
	    queueId = LOWPRI;
	} else if (HIGH_PRIORITY(req->opcode)) {
	    queueId = HIGHPRI;
	} else {
	    queueId = NOPRI;
	}
	traceEntry(req);
	add_work(&work_queue[queueId], req);
    }
}

/*****************************************************************************/

static int
mainproc(as, arock)
    register struct cmd_syndesc *as;
    char                   *arock;

{
    long                    code;
    int                     i,
			    temp,
                            pipefds[2];
    int                     syntaxError = 0;
    pthread_t               thread;
    char                    pb = 0;

#ifndef DCE1_0_2A
    char                   *ports;

#endif

    Debugging = 0;

    if (as->parms[DFSBIND_ARG_DEBUG].items) {
	/* -debug */
	Debugging = 1;
    }
    /* time-to-live value for sucessfull lookups */
    if (as->parms[DFSBIND_ARG_JUNCTION].items) {
	if ((ttlhit = strtoul(as->parms[DFSBIND_ARG_JUNCTION].items->data,
			      (char **) NULL, 10)) < 30) {
	    dce_svc_printf(DSB_S_LIFE_TOO_SHORT_MSG, TTLHIT_DEFAULT);
	    syntaxError++;
	}
    } else
	ttlhit = TTLHIT_DEFAULT;

    /* time-to-live for prefix lookups */
    if (as->parms[DFSBIND_ARG_PREFIX].items) {
	if ((ttldir = strtoul(as->parms[DFSBIND_ARG_PREFIX].items->data,
			      (char **) NULL, 10)) < 30) {
	    dce_svc_printf(DSB_S_TOO_SHORT_PREFIXLIFE_MSG, TTLDIR_DEFAULT);
	    syntaxError++;
	}
    } else
	ttldir = TTLDIR_DEFAULT;

    /* time-to-live value for unsucessfull lookups */
    if (as->parms[DFSBIND_ARG_NOTFOUND].items) {
	if ((ttlmiss = strtoul(as->parms[DFSBIND_ARG_NOTFOUND].items->data,
			       (char **) NULL, 10)) < 30) {
	    dce_svc_printf(DSB_S_TOO_SHORT_NOTFOUNDLIFE_MSG, TTLMISS_DEFAULT);
	    syntaxError++;
	}
    } else
	ttlmiss = TTLMISS_DEFAULT;

    if (as->parms[DFSBIND_ARG_EXPRESS].items) {
	if ((hpth = strtoul(as->parms[DFSBIND_ARG_EXPRESS].items->data,
			    (char **) NULL, 10)) <= 0) {
	    dce_svc_printf(DSB_S_NO_EXPR_DAEMONS_MSG, DEFAULT_EXPRESS_PROCS);
	    syntaxError++;
	}
    }
    if (as->parms[DFSBIND_ARG_REGULAR].items) {
	if ((lpth = strtoul(as->parms[DFSBIND_ARG_REGULAR].items->data,
			    (char **) NULL, 10)) <= 0) {
	    dce_svc_printf(DSB_S_NO_REGULAR_DAEMON_MSG, DEFAULT_REGULAR_PROCS);
	    syntaxError++;
	}
    }
    if (syntaxError)
	exit(1);

retry:
    if ((fd = KRPC_OPEN_HELPER()) < 0) {
	code = errno;
	if (code == EALREADY) {
	    dce_svc_printf(DSB_S_ALREADY_RUNNING_HELPER_MSG);
	    exit(code);
	}
	if (code == EINPROGRESS) {
	    sleep(30);
	    goto retry;
	}
	dce_svc_printf(DSB_S_NO_START_MSG, strerror(code));
	exit(code);
    }
    /*
     * qsize = (hpth+lpth)*2;
     */

    /*
     * Set qsize to NTHREADS. Guarantees that there will be a thread
     * available for each outstanding request from krpch.
     * 
     * qsize=NTHREADS;
     * 
     * On review found too restrictive. Instead use hpth+lpth - this value does
     * not guarantee that a thread will always be available.
     */

   krpch_config_params.qsize = (hpth + lpth + 1) * 2;
   krpch_config_params.bufsize = DFSBIND_BUFSIZE;
   /*
    * Restrict krpc ports, if specified by the environment
    */
   ports = getenv("RPC_RESTRICTED_PORTS");
   if (ports != (char *) NULL) {
       strncpy((char *)&krpc_ports[0], ports, KRPC_PORT_STRING_SIZE);
       if (ioctl(fd, KRPCH_RESTRICT_PORTS, krpc_ports) != 0) {
	   dce_svc_printf(DSB_S_KRPC_PORTS_RESTR_MSG, errno);
	   exit(errno);
       }
       dce_svc_printf(DSB_S_KRPC_PORTS_MSG, ports);
   }
   /*
    * configure the kernel's helper queue size, buffer size and
    * enable the the helper subsystem 
    */

   if (ioctl(fd, KRPCH_CONFIGURE, (char *)&krpch_config_params.qsize) < 0){
       dce_svc_printf(DSB_S_BAD_HELPER_CONFIG_MSG, errno);
       exit(errno);
   }
   if (ioctl(fd, KRPCH_ENABLE, (char *)NULL) < 0){
       dce_svc_printf(DSB_S_BAD_HELPER_ENABLE_MSG, errno);
       exit(errno);
   }

    /*
     * Open syslog so we can log any problems
     */
    openlog("dfsbind", LOG_CONS | LOG_NDELAY, LOG_DAEMON);

    /*
     * Use the technique of fork() and exit(read(pipe)) to convey child
     * status to parent which was started by a script. Fork if not debugging.
     * It is important to fork early before any activity occurrs which will
     * start threads.
     */
    /* The CMA fork() wrapper doesn't work, but other wrappers do. */
#if defined(CMA_UX) && defined(fork)
#undef fork			/* Turn off CMA wrapper for fork */
#endif				/* defined(CMA_UX) && defined(fork) */
    if (pipe(pipefds) == -1) {
	dce_svc_printf(DSB_S_BAD_PIPE_MSG,strerror(errno));
	exit(1);
    }
    if (Debugging == 0) {
	switch (fork()) {
	  case -1:
	    dce_svc_printf(DSB_S_FORK_FAILED_MSG, -1, errno);
	    exit(1);
	    break;
	  default:
	    /* we are the parent */
	    (void) close(pipefds[1]);
	    exit((read(pipefds[0], &pb, 1) == 1) ? 0 : 1);
	    break;
	  case 0:
	    /* we are in the child here */
	    (void) close(pipefds[0]);
	    /*
	     * get rid of our controlling terminal and get into a new
	     * process group
	     */
	    if (setsid() == -1) {
		dce_svc_printf(DSB_S_BAD_SETSID_MSG, strerror(errno)); 
	    }
	    break;
	}
    }
#ifdef	_EPISODE_LIC_MGMT_
    /*
     * Look to see if LFS is in use.  If so, require a license before
     * continuing and setting up kernel helper threads.  If unable to get
     * valid license status we exit now.
     */
    as_init();			/* initialize names of aggrs */
    if (exportingAggregates() && checkPXLicense()) {
	syslog(LOG_ERR + LOG_DAEMON,"Could not obtain valid fileserver license.\n");
	dce_svc_printf(DSB_S_FILESERVER_LIC_MSG);
	exit(3);
    }
    if (episodeInUse() && checkEpisodeLicense()) {
	syslog(LOG_ERR + LOG_DAEMON,"Could not obtain valid Episode license.\n");
	dce_svc_printf(DSB_S_LFS_LIC_MSG);
	exit(3);
    }
#endif

    /* Start the ICL log, even before we ask for the signal catcher */
    SetupLog();

    /* start the auth helper; note that this will start up threads */
    do_InitAuth(Debugging);

    /* set up ICL signal catcher */
    code = icl_StartCatcher(0);
    if (code < 0)
	(void) dce_svc_printf(DSB_S_NO_SIGNAL_WATCHER_MSG, code, errno);

    /* Initialize queues and create worker threads */
    for (i = 0; i < NQUEUES; i++) {
	work_queue[i].head = work_queue[i].tail = NULL;
	pthread_mutex_init(&work_queue[i].qlock, pthread_mutexattr_default);
	pthread_cond_init(&work_queue[i].wcond, pthread_condattr_default);
	work_queue[i].threadCount = 0;
	work_queue[i].queueCount = 0;
	work_queue[i].queueLabel = 0;
    }
    work_queue[HIGHPRI].queueLabel = "high";
    work_queue[NOPRI].queueLabel = "no";
    work_queue[LOWPRI].queueLabel = "low";

    for (i = 0; i < hpth; i++) {
	pthread_create(&thread, pthread_attr_default,
		       (pthread_startroutine_t) service_thread,
		       (pthread_addr_t) & work_queue[HIGHPRI]);
	++(work_queue[HIGHPRI].threadCount);
    }
    for (i = 0; i < lpth; i++) {
	pthread_create(&thread, pthread_attr_default,
		       (pthread_startroutine_t) service_thread,
		       (pthread_addr_t) & work_queue[NOPRI]);
	++(work_queue[NOPRI].threadCount);
    }

    /*
     * Since the low priority queue is handling bind requests and those
     * requests are forced to be serialized because of a mutex, we just
     * create one thread.  This also allows the queue compressor to compress
     * out "same" requests since they will still be in the queue rather than
     * being blocked in some other thread.
     */
    pthread_create(&thread, pthread_attr_default,
		   (pthread_startroutine_t) service_thread,
		   (pthread_addr_t) & work_queue[LOWPRI]);
    ++(work_queue[LOWPRI].threadCount);

#ifdef	_EPISODE_LIC_MGMT_
    pthread_create(&thread, pthread_attr_default,
		   (pthread_startroutine_t) licenseEpisode,
		   (pthread_addr_t) 0);
#endif

    /* Let parent exit normally */
    (void) write(pipefds[1], &pb, 1);
    (void) close(pipefds[1]);

    dispatch_thread();

    return 0;
}

/*****************************************************************************/

int
main(argc, argv)
    int                     argc;
    char                  **argv;

{
    register struct cmd_syndesc *ts;
    sigset_t                signalBlockSet;

    osi_setlocale(LC_ALL, "");

    initialize_svc();

    /*
     * block SIGUSR1 for all threads. The ICL thread that will be waiting for
     * it will clear its mask by using sigwait()
     */

    (void) sigemptyset(&signalBlockSet);
    (void) sigaddset(&signalBlockSet, SIGUSR1);
    (void) sigprocmask(SIG_BLOCK, &signalBlockSet, (sigset_t *) NULL);

    /* make sure dfsbind is running as the 'self' identity */
    dfsauth_server_RunAsSelf();

    ts = cmd_CreateSyntax((char *) 0, mainproc, (char *) 0, "start dfsbind");

    cmd_IndexedAddParm(ts, "-expressprocs",
		       CMD_SINGLE, CMD_OPTIONAL | CMD_PARM_NUMERIC,
		       "number_of_express_daemons", DFSBIND_ARG_EXPRESS);
    cmd_IndexedAddParm(ts, "-regularprocs",
		       CMD_SINGLE, CMD_OPTIONAL | CMD_PARM_NUMERIC,
		       "number_of_regular_daemons", DFSBIND_ARG_REGULAR);
    cmd_IndexedAddParm(ts, "-junctionlife",
		       CMD_SINGLE, CMD_OPTIONAL | CMD_PARM_NUMERIC,
		       "seconds_to_live", DFSBIND_ARG_JUNCTION);
    cmd_IndexedAddParm(ts, "-prefixlife",
		       CMD_SINGLE, CMD_OPTIONAL | CMD_PARM_NUMERIC,
		       "seconds_to_live", DFSBIND_ARG_PREFIX);
    cmd_IndexedAddParm(ts, "-notfoundlife",
		       CMD_SINGLE, CMD_OPTIONAL | CMD_PARM_NUMERIC,
		       "seconds_to_live", DFSBIND_ARG_NOTFOUND);
    cmd_IndexedAddParm(ts, "-debug", CMD_FLAG,
		       CMD_OPTIONAL, "", DFSBIND_ARG_DEBUG);

    return (cmd_Dispatch(argc, argv));
}

/*****************************************************************************/
