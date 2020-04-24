/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: spoke_mgr.c,v $
 * Revision 1.1.322.1  1996/10/17  18:28:43  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:26  damon]
 *
 * Revision 1.1.317.1  1994/02/04  20:46:25  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:01  devsrc]
 * 
 * Revision 1.1.315.1  1993/12/07  17:45:30  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:24:27  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: spoke_mgr.c,v 1.1.322.1 1996/10/17 18:28:43 damon Exp $
 *
 * COMPONENT_NAME: Distributed ITL Development Infrastructure
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */

#include <pthread.h>
#include <utils/cht_hash.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/defs.h>

int initialized;
int debug;

void (*spoke_connect_callback)();

static pthread_cond_t		globalCond;
static pthread_mutex_t		globalMutex;

typedef struct {
    int			references;	/* how many outstanding refs */
    char		name[32];	/* the name of the syncpoint */
    cht_link_t		link;		/* hash table link */
    int			released;	/* whether 'joiners' can proceed */
    int			timeout;	/* whether anyone has timed out */
    unsigned int	waiting;	/* how many waiters */
    unsigned int	expected; 	/* how many are expected */
} syncpoint_t;

#define	SYNC_HASH_KEY(x)	((x)->name)
#define	SYNC_HASH_FUNC(x)	((x)[0])
#define	SYNC_HASH_EQUAL(x,y)	(strcmp(x,y) == 0)

CHT_GENERATE_ROUTINES(PRIVATE, sync_, 32, syncpoint_t, link,
		      char *, SYNC_HASH_KEY, SYNC_HASH_FUNC, SYNC_HASH_EQUAL)

static unsigned32 
spoke_mgr_setup(
		handle_t h,
		idl_char * binding
		)
{
    char 		* copy;

    ITL_ALLOC(copy, char *, strlen((char *)binding)+1);
    strcpy(copy, (char *) binding);
    
    DEBUG((ITL_TRACE_SPOKE,"server received binding:\n%s\n", copy));

    if (spoke_connect_callback) 
	    (*spoke_connect_callback)(copy);

    return 0;
}

unsigned32 
spoke_mgr_execute_string(
			 handle_t h,
			 unsigned char * str,
			 unsigned32 * status
			 )
{
    boolean_t result;
    int realStatus;

    DEBUG((ITL_TRACE_SPOKE,"about to execute string:\n%s\n",str));
    itl_SetIntVariable("ITL_SpokeResult", 0, var_local);

    /*
     *  If itl_RunFromString() fails, and the ITL_SpokeResult is zero
     *  then the script had a command failure, i.e, the command interpreter
     *  exited abnormally.  So we force an un-successful status to be
     *  returned.
     */
    result = itl_RunFromString((char *)str);
    realStatus = itl_GetIntVariable("ITL_SpokeResult");
    if ((result == TRUE) && (realStatus == 0))
	realStatus = -1;
    itl_SetErrorStatus(FALSE);
    *status = (unsigned32) realStatus;
    
    DEBUG((ITL_TRACE_SPOKE,"done\n"));
    return 0;
}

syncpoint_t * sync_create(char * name)
{
    syncpoint_t		* sync;
    
    ITL_NEW(sync, syncpoint_t);
    strcpy(sync->name, name);
    sync->released = sync->waiting = 0;
    sync->expected = 0;         /* means not initialized by hub */
    sync->references = 0;	/* one for the table */
    sync->timeout = 0;
    return sync;
}

syncpoint_t * sync_reference(char * name)
{
    syncpoint_t		* sync;

    sync = sync_Lookup(name, CHT_LATCH_NONE);
    if (!sync) {
	sync = sync_create(name);
	sync_Insert(sync, CHT_LATCH_NONE);
    }
    sync->references++;
    return sync;
}

/*
 * Entries remain in the table until either released or timedout
 */

void sync_dereference(syncpoint_t * sync)
{
    sync->references--;
    if (sync->references == 0 && (sync->timeout || sync->released)) {
	ITL_FREE(sync);
    }
}

void 
syncpoint_wait(
	       syncpoint_t * sync,
	       unsigned32 timeout,
	       int * timed_out
	       )
{
    struct timespec 	delta, abstime;    

    sync->waiting++;
    if (sync->expected && (sync->waiting >= sync->expected))
	BROADCAST(&globalCond);
   
    *timed_out = sync->timeout;
    while (!sync->released && !sync->timeout) {
	if (timeout != -1) {
	    delta.tv_sec = timeout; delta.tv_nsec = 0;
	    pthread_get_expiration_np(&delta, &abstime);
	    TIMED_WAIT(&globalCond, &globalMutex, &abstime, timed_out);
	    if (*timed_out) {
		sync->timeout++;
		break;
	    }
	} else { 
	    WAIT(&globalCond, &globalMutex);
	}	
    }
    sync->waiting--;
}

void 
spoke_create_syncpoint(
		       char * sync_name,
		       int num_expected,
		       unsigned32 timeout,
		       int * timed_out
		       )
{
    syncpoint_t		* sync;
    struct timespec 	delta, abstime;

    DEBUG((ITL_TRACE_SPOKE,"> spoke_create_syncpoint %s %d %d\n",
	   sync_name, num_expected, timeout));
    
    LOCK(&globalMutex);
    sync = sync_reference(sync_name);

    sync->expected = num_expected;
    *timed_out = sync->timeout;
    
    delta.tv_sec = timeout; delta.tv_nsec = 0;
    pthread_get_expiration_np(&delta, &abstime);

    while ((sync->waiting < sync->expected) && !sync->timeout) 
    {
	TIMED_WAIT(&globalCond, &globalMutex, &abstime, timed_out);
	if (*timed_out)
	    sync->timeout++;
    }
    
    sync_dereference(sync);
    UNLOCK(&globalMutex);

    DEBUG((ITL_TRACE_SPOKE,"< spoke_create_syncpoint %s time out %d\n",
	   sync_name, *timed_out));
}

void 
spoke_release_syncpoint(
			char * sync_name,
			int * timed_out
			)
{
    syncpoint_t		* sync;

    DEBUG((ITL_TRACE_SPOKE,"> spoke_release_syncpoint %s\n",
	   sync_name));
    
    LOCK(&globalMutex);

    /*
     * don't use sync_reference here, because it will create one if
     * it doesn't exist, which might be the case if the spokes
     * timedout waiting to be released.
     */
    
    sync = sync_Lookup(sync_name, CHT_LATCH_NONE);
    if (!sync) {
	*timed_out = TRUE;
	UNLOCK(&globalMutex);
	return;
    }
    *timed_out = sync->timeout;    
    sync->references++;
    sync->released++;
    /*
     *  The sync point is deleted here to avoid a deadlock that might occur
     *  if a sync point is re-used in a tight loop. (This is bad programming
     *  but it's a free country.)  It is free'd in the dereference code.
     *  This way, threads waiting for it will still have a valid memory
     *  pointer when they wake up.
     */
    sync_Del(sync, CHT_LATCH_NONE);
    BROADCAST(&globalCond);
    sync_dereference(sync);	
    UNLOCK(&globalMutex);

    /*
     *  Now give those threads waiting for the sync point to release a
     *  chance to complete and return to the clients.
     */
    pthread_yield();

    DEBUG((ITL_TRACE_SPOKE,"< spoke_release_syncpoint %s time out %d\n",
	   sync_name, *timed_out));    
}
    
unsigned32 
spoke_mgr_syncpoint(
		    handle_t h,
		    unsigned char * sync_name,
		    unsigned32 timeout,
		    unsigned32 * timed_out
		    )
{
    syncpoint_t		* sync;

    DEBUG((ITL_TRACE_SPOKE,"> spoke_mgr_syncpoint %s %d\n",
	   sync_name, timeout));
   
    *timed_out = 0;
    
    LOCK(&globalMutex);
    sync = sync_reference((char *)sync_name);

    syncpoint_wait(sync, timeout, (int *)timed_out);
    
    sync_dereference(sync);
    UNLOCK(&globalMutex);

    DEBUG((ITL_TRACE_SPOKE,"< spoke_mgr_syncpoint %s time out %d\n",
	   sync_name, *timed_out));
    
    return 0;
}

unsigned32 spoke_mgr_null(handle_t h)
{
    return 0;
}

void spoke_mgr_exit(handle_t h)
{
    itl_AdviseUser("Exiting at the request of the hub.\n");
    exit(0);
}

spoke_v1_0_epv_t spoke_v1_0_epv = {
    spoke_mgr_null,
    spoke_mgr_setup,
    spoke_mgr_syncpoint,
    spoke_mgr_execute_string,
    spoke_mgr_exit
    };

void spoke_mgr_init( void (*spoke_connect_cb)() )
{
    unsigned32		st;

    initialized = TRUE;
    
    pthread_cond_init(&globalCond, pthread_condattr_default);
    pthread_mutex_init(&globalMutex, pthread_mutexattr_default);
    
    sync_Init();
    
    spoke_connect_callback = spoke_connect_cb;
    
    rpc_server_register_if(spoke_v1_0_s_ifspec, NULL,
			   (rpc_mgr_epv_t) &spoke_v1_0_epv, &st);

    if (st)
	DCE_ERROR(st, "register_if FAILED");
}
