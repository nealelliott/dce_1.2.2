/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: lock.c,v $
 * Revision 1.1.2.2  1996/02/18  19:16:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:15  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/08  21:46 UTC  jss
 * 	Merge changes for CHFts14511, CHFts15038, CHFts14283.
 * 
 * 	HP revision /main/jss_mothra_fix3/1  1995/04/13  17:41 UTC  jss
 * 	Fix for CHFts14511. Fix the lock manager code.
 * 	[1995/12/08  20:49:01  root]
 * 
 * Revision 1.1.2.1  1995/04/06  17:46:57  karlz
 * 	Removed thread ID from debug messages; serviceability inserts the thread
 * 	ID anyways.
 * 	[1995/04/06  03:41:34  karlz]
 * 
 * 	Just adjusted debug levels.
 * 	Component = dhd, subcomponent = locks,
 * 		level 1	: assert failures
 * 		level 2 : lock creation/deletion
 * 		level 3 : read,riw,write,unlock
 * 	[1995/04/04  20:12:00  karlz]
 * 
 * 	Fixed lock management (see OT CR 12834).  Original code would grant a write
 * 	lock even if threads still held read locks.
 * 	[1995/04/04  19:20:55  karlz]
 * 
 * 	DCE for Digital UNIX: populate from OSF DCE R1.1
 * 	[1995/04/04  18:23:11  karlz]
 * 
 * Revision 1.1.2.8  1994/09/26  17:49:21  rsalz
 * 	Convert to true SVC functions (OT CR 11725).
 * 	[1994/09/26  17:12:17  rsalz]
 * 
 * Revision 1.1.2.7  1994/08/03  20:36:18  rsalz
 * 	Fix OT CR 11506.
 * 	[1994/08/03  20:32:24  rsalz]
 * 
 * Revision 1.1.2.6  1994/08/02  05:45:34  rsalz
 * 	Undo in-line version of dce_lock_justwrite.
 * 	[1994/08/02  05:36:20  rsalz]
 * 
 * Revision 1.1.2.5  1994/07/05  16:03:23  rsalz
 * 	Add dce_lock_justwrite (part of OT CR 11173).
 * 	[1994/07/05  15:57:18  rsalz]
 * 
 * Revision 1.1.2.4  1994/06/09  16:03:06  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:44  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  20:15:13  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:43  rsalz]
 * 
 * Revision 1.1.2.2  1994/04/28  18:38:45  rsalz
 * 	Clean up some compiler warnings; remove dead code.
 * 	[1994/04/28  18:37:53  rsalz]
 * 
 * Revision 1.1.2.1  1994/04/27  19:41:35  rsalz
 * 	Add dce_assert, set up headers right
 * 	[1994/04/27  19:38:06  rsalz]
 * 
 * 	Add lock manager.
 * 	[1994/04/27  19:00:21  rsalz]
 * 
 * $EndLog$
 */

/*
** Lock manager.  Based on code by Jonathan E. Chinitz <jec@isoft.com>.
*/
#include <libimpl.h>


/*
**  The lock datatype.
*/
typedef struct lock_where_s_t {
    char	*file;
    int		line;
} lock_where_t;

struct dce_lock_s_t {
    pthread_mutex_t	m;
    pthread_cond_t	cv;
    int			readers;
    enum {
	lock_state_nonexistent=0, lock_state_none, lock_state_read, lock_state_riw, lock_state_write
    }			state;
    pthread_t		writer;		/* who is the writer */
    lock_where_t	the_creator;
    lock_where_t	the_writer;
};

static char	*stateNames[] = {
	"nonexistent",
	"free",
	"read",
	"riw",
	"write"
};

/*
**  Concurrency convenience macros.
*/
#define LOCK_OR_RETURN(l, st)		\
    if (pthread_mutex_lock(&l->m) < 0) { \
	*st = dced_s_mutex_lock_failed; \
	return; \
    }
#define UNLOCK_OR_RETURN(l, st)		\
    if (pthread_mutex_unlock(&l->m) < 0) { \
	*st = dced_s_mutex_unlock_failed; \
	return; \
    }
#define BLOCK_OR_RETURN(l, st)		\
    if (pthread_cond_wait(&l->cv, &l->m) < 0) { \
	 *st = dced_s_cond_wait_failed; \
	 return; \
    }
#define WAKE_ONE_OR_RETURN(l, st)	\
    if (pthread_cond_signal(&l->cv) < 0) { \
	 *st = dced_s_cond_signal_failed; \
	 return; \
    }
#define WAKE_ALL_OR_RETURN(l, st)	\
    if (pthread_cond_broadcast(&l->cv) < 0) { \
	 *st = dced_s_cond_bcast_failed; \
	 return; \
    }


/*
**  Create a lock.
*/
void
dce_lock__create(
    dce_lock_t		*lock,
    error_status_t	*st,
    char		*file,
    int			line
)
{
    dce_lock_t		new;

    if ((new = (dce_lock_t)malloc(sizeof *new)) == NULL) {
	*st = dced_s_no_memory;
	return;
    }

    if (pthread_mutex_init(&new->m, pthread_mutexattr_default) < 0) {
	free(new);
	*st = dced_s_mutex_init_fail;
	return;
    }
    if (pthread_cond_init(&new->cv, pthread_condattr_default) < 0) {
	pthread_mutex_destroy(&new->m);
	free(new);
	*st = dced_s_cond_init_fail;
	return;
    }

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug2,
	"%s:%d LOCK: 0x%x (%s): create",
	file, line, new, stateNames[ lock_state_nonexistent ]));

    new->readers = 0;
    new->state = lock_state_none;
    new->the_creator.file = file;
    new->the_creator.line = line;
    *lock = new;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug2,
	"LOCK: 0x%x (--> %s)",
	new, stateNames[ new->state ]));

    *st = error_status_ok;
}


/*
**  Free a lock.  Ignores failures.
*/
void
dce_lock__delete(
    dce_lock_t		lock,
    error_status_t	*st,
    char		*file,
    int			line
)
{
    /* The lock must be free in order to remove it! */
    if (lock->state != lock_state_none) {
        DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug1,
        "%s:%d LOCK: 0x%x (%s, nreaders = %d): ERRONEOUS delete WHEN HELD",
        file, line, lock, stateNames[ lock->state ], lock->readers));

    }
    dce_assert(dhd_svc_handle, lock->state == lock_state_none);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug2,
        "%s:%d LOCK: 0x%x (%s, nreaders = %d): delete",
        file, line, lock, stateNames[ lock->state ], lock->readers));

    pthread_mutex_destroy(&lock->m);
    pthread_cond_destroy(&lock->cv);
    free((char *)lock);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug2,
        "LOCK: 0x%x (--> %s)",
        lock, stateNames[ lock_state_nonexistent ]));

    *st = error_status_ok;
}


/*
**  Get a read-lock.
*/
void
dce_lock__read(
    dce_lock_t		lock,
    error_status_t	*st,
    char		*file,
    int			line
)
{
    LOCK_OR_RETURN(lock, st);

    /* Readers have to wait for the writer to finish */
    while (lock->state == lock_state_write) {
	BLOCK_OR_RETURN(lock, st);
    }

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug3,
        "%s:%d LOCK: 0x%x (%s, nreaders = %d): read",
        file, line, lock, stateNames[ lock->state ], lock->readers));

    /* Increment the number of readers. If first reader, set state. */
    lock->readers++;
    if (lock->state == lock_state_none)
	lock->state = lock_state_read;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug3,
        "LOCK: 0x%x (--> %s, nreaders = %d)",
        lock, stateNames[ lock->state ], lock->readers));

    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}


/*
** Get a read-with-intent-to-write lock.
*/
void
dce_lock__riw(
    dce_lock_t		lock,
    error_status_t	*st,
    char		*file,
    int			line
)
{
    LOCK_OR_RETURN(lock, st);

    /* Readers have to wait for the writer to finish. */
    while (lock->state == lock_state_write || lock->state == lock_state_riw) {
	BLOCK_OR_RETURN(lock, st);
    }

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug3,
        "%s:%d LOCK: 0x%x (%s, nreaders = %d): riw",
        file, line, lock, stateNames[ lock->state ], lock->readers));

    /* Update state. */
    lock->the_writer.file = file;
    lock->the_writer.line = line;
    lock->readers++;
    lock->state = lock_state_riw;
    lock->writer = pthread_self();

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug3,
        "LOCK: 0x%x (--> %s, nreaders = %d)",
        lock, stateNames[ lock->state ], lock->readers));

    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}


void
dce_lock__write(
    dce_lock_t		lock,
    error_status_t	*st,
    char		*file,
    int			line
)
{
    pthread_t		me = pthread_self();

    LOCK_OR_RETURN(lock, st);

    /* One is only allowed to request write lock if one has previously
     * obtained a riw lock!  Note that this requires two tests;
     * 1) Verify that lock state is riw, and
     * 2) Verify that this is in fact the thread that owns the riw.
     */
    if ((lock->state != lock_state_riw) || !pthread_equal(me, lock->writer)) {
        DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug1,
	"%s:%d LOCK: 0x%x (%s, nreaders = %d): ERRONEOUS write WHEN NOT OWNER",
	file, line, lock, stateNames[ lock->state ], lock->readers));
    }
    dce_assert(dhd_svc_handle, ((lock->state == lock_state_riw) && pthread_equal(me, lock->writer)));

    /* Readers and Writers block. The only question here is if the state
     * was RIW and this is the owner of the lock and there are no other 
     * readers, then we can proceed to write. Otherwise we have to wait. */
    while (lock->readers > 1) {
	BLOCK_OR_RETURN(lock, st);
    }

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug3,
        "%s:%d LOCK: 0x%x (%s, nreaders = %d): write",
        file, line, lock, stateNames[ lock->state ], lock->readers));

    /* The last riwer becomes a writer now. */
    lock->readers = 0;
    lock->state = lock_state_write;
    lock->writer = me;
    lock->the_writer.file = file;
    lock->the_writer.line = line;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug3,
        "LOCK: 0x%x (--> %s, nreaders = %d)",
        lock, stateNames[ lock->state ], lock->readers));

    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}


void
dce_lock__justwrite(
    dce_lock_t		lock,
    error_status_t	*st,
    char		*file,
    int			line
)
{
    error_status_t	st2;

    dce_lock__riw(lock, st, file, line);
    if (*st == error_status_ok) {
	dce_lock__write(lock, st, file, line);
	if (*st != error_status_ok)
	    dce_lock__unlock(lock, &st2, file, line);
    }
}


void
dce_lock__unlock(
    dce_lock_t		lock,
    error_status_t	*st,
    char		*file,
    int			line
)
{
    pthread_t		me = pthread_self();

    LOCK_OR_RETURN(lock, st);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug3,
        "%s:%d LOCK: 0x%x (%s, nreaders = %d): unlock",
        file, line, lock, stateNames[ lock->state ], lock->readers));

    switch (lock->state) {
    case lock_state_none:
	/* The lock must be held in order to release it! */
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug1,
	"%s:%d LOCK: 0x%x (%s, nreaders = %d): ERRONEOUS unlock WHEN NOT HELD",
	file, line, lock, stateNames[ lock->state ], lock->readers));
	dce_assert(dhd_svc_handle, lock->state != lock_state_none);
	/* NOTREACHED */
    case lock_state_read:
	/* Decrement the readers. If zero, then only a writer might be
	 * sleeping on the cv, so wake it up. */
	lock->readers--;
	if (lock->readers == 0) {
	    WAKE_ONE_OR_RETURN(lock, st);
	    lock->state = lock_state_none;
	}
	break;
    case lock_state_riw:
	/* Decrement the readers. If zero, might be another RIW or a W so 
	 * must broadcast. If I was the owner of the lock, then unmark
	 * me and make the RIW lock a R lock. */
	lock->readers--;
	if (lock->readers == 0) {
	    /* If we're not the writer, this is an error! */
	    if (!pthread_equal( me, lock->writer )) {
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug1,
		"%s:%d LOCK: 0x%x (%s, nreaders = %d): ERRONEOUS unlock WHEN NOT RIWER",
		file, line, lock, stateNames[ lock->state ], lock->readers));
	    }
	    dce_assert(dhd_svc_handle, pthread_equal( me, lock->writer ));
	    WAKE_ALL_OR_RETURN(lock, st);
	    lock->state = lock_state_none;
	}
	else if (pthread_equal(me, lock->writer)) {
	    lock->state = lock_state_read;
	}
	break;
    case lock_state_write:
	/* Writer resets the lock and wakes up any RIW or R that might be
	 * sleeping. */
	/* If we're not the writer, this is an error! */
	if (!pthread_equal( me, lock->writer )) {
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug1,
	    "%s:%d LOCK: 0x%x (%s, nreaders = %d): ERRONEOUS unlock WHEN NOT WRITER",
	    file, line, lock, stateNames[ lock->state ], lock->readers));
	}
	dce_assert(dhd_svc_handle, pthread_equal( me, lock->writer ));
	lock->readers = 0;
	lock->state = lock_state_none;
	WAKE_ALL_OR_RETURN(lock, st);
	break;
    }

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_lock, svc_c_debug3,
        "LOCK: 0x%x (--> %s, nreaders = %d)",
        lock, stateNames[ lock->state ], lock->readers));

    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}

void
dce_lock__hold_lock(
    dce_lock_t		lock,
    boolean32		*b,
    error_status_t	*st,
    char		*file,
    int			line
)
{
    *b = FALSE;
    LOCK_OR_RETURN(lock, st);

    switch (lock->state) {
    case lock_state_none:
    case lock_state_read:
	break;
    case lock_state_riw:
    case lock_state_write:
	*b = pthread_equal(pthread_self(), lock->writer);
	break;
    }

    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}
