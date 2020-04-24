/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Lock manager.  Taken from DCE-Web distribution.
 * ______________________________________________________________________
 *
 * $Log: lock.c,v $
 * Revision 1.1  1996/09/27  18:04:46  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

/*
**  Copyright (C) 1995, 1996 Open Software Foundation, Inc.
**  All Rights Reserved (DCE-Web). Please see COPYRIGHT.DCEWEB
**  from the source distribution.
*/

#include "mosdefs.h"

#include <dce/assert.h>

/* This package can be built with the ability to take recursive riw and
 * write locks.  Some folks like 'em, some don't, and some folks simply
 * need 'em because of preexisting software.
 */
/*#define RECURSIVE_LOCKS*/

/* ______________________________________________________________________ */

/* Concurrency convenience macros. */
#define LOCK_OR_RETURN(l, st)		\
    if (pthread_mutex_lock(&l->m) < 0) { \
	*st = mos_s_mutex_lock_failed; \
	return; \
    }
#define UNLOCK_OR_RETURN(l, st)		\
    if (pthread_mutex_unlock(&l->m) < 0) { \
	*st = mos_s_mutex_unlock_failed; \
	return; \
    }
#define BLOCK_OR_RETURN(l, st)		\
    if (pthread_cond_wait(&l->cv, &l->m) < 0) { \
	 *st = mos_s_cond_wait_failed; \
	 return; \
    }
#define WAKE_ONE_OR_RETURN(l, st)	\
    if (pthread_cond_signal(&l->cv) < 0) { \
	 *st = mos_s_cond_signal_failed; \
	 return; \
    }
#define WAKE_ALL_OR_RETURN(l, st)	\
    if (pthread_cond_broadcast(&l->cv) < 0) { \
	 *st = mos_s_cond_bcast_failed; \
	 return; \
    }

/* ______________________________________________________________________ */

/* The lock datatype. */
typedef struct lock_where_s_t {
    char	*file;
    int		line;
} lock_where_t;

struct lock_s_t {
    pthread_mutex_t	m;
    pthread_cond_t	cv;
    int			readers;
    enum {
	lock_state_dummy = 0, lock_state_free, lock_state_read,
	lock_state_riw, lock_state_write
    }			state;
    pthread_t		writer;
    lock_where_t	the_creator;
    lock_where_t	the_writer;

#ifdef RECURSIVE_LOCKS
    /* recursive_lock_depth is a current count of how many times this
     * thread has (RIW|WRITE) locked.  Note that pure readers aren't
     * managed with this counter.  */
    int                 recursive_lock_depth;
#endif				/* RECURSIVE_LOCKS */
};

/* ______________________________________________________________________ */

/* Create a lock. */
void
lock__create(mos_lock_t *lock, error_status_t *st, char *file, int line)
{
    mos_lock_t	new;

    if ((new = NEW(struct lock_s_t, 1)) == NULL) {
	*st = mos_s_no_memory;
	return;
    }

    if (pthread_mutex_init(&new->m, pthread_mutexattr_default) < 0) {
	free(new);
	*st = mos_s_mutex_init_fail;
	return;
    }
    if (pthread_cond_init(&new->cv, pthread_condattr_default) < 0) {
	pthread_mutex_destroy(&new->m);
	free(new);
	*st = mos_s_cond_init_fail;
	return;
    }

    new->readers = 0;
    new->state = lock_state_free;
    new->the_creator.file = file;
    new->the_creator.line = line;
#ifdef RECURSIVE_LOCKS
    new->recursive_lock_depth = 0;
#endif
    *lock = new;
    *st = error_status_ok;
}


/* Free a lock.  Ignores thread-related failures. */
void
lock__delete(mos_lock_t lock, error_status_t *st, char *file, int line)
{
    /* The lock must be free in order to remove it! */
    dce_assert(NULL, lock->state == lock_state_free);

    pthread_mutex_destroy(&lock->m);
    pthread_cond_destroy(&lock->cv);
    free((char *)lock);

    *st = error_status_ok;
}


/* Get a read-lock. */
void
lock__read(mos_lock_t lock, error_status_t *st, char *file, int line)
{
    LOCK_OR_RETURN(lock, st);

    /* Readers have to wait for the writer to finish */
    while (lock->state == lock_state_write) {
	BLOCK_OR_RETURN(lock, st);
    }

    /* Increment the number of readers. If first reader, set state. */
    lock->readers++;
    if (lock->state == lock_state_free)
	lock->state = lock_state_read;

    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}


/* Get a read-with-intent-to-write lock. */
void
lock__riw(mos_lock_t lock, error_status_t *st, char *file, int line)
{
    pthread_t		me = pthread_self();

    LOCK_OR_RETURN(lock, st);

#ifdef RECURSIVE_LOCKS
    /* If we're already the locker, then just maintain refcount */

    if ((lock->state == lock_state_write || lock->state == lock_state_riw) &&
	pthread_equal(me, lock->writer)) {
	dce_assert(NULL, lock->recursive_lock_depth > 0);
	lock->recursive_lock_depth++;
	UNLOCK_OR_RETURN(lock, st);
	*st = error_status_ok;
	return;
    }
#endif				/* RECURSIVE_LOCKS */

    /* Readers have to wait for the writer to finish. */
    while (lock->state == lock_state_write || lock->state == lock_state_riw) {
	BLOCK_OR_RETURN(lock, st);
    }

    /* Update state. */
    lock->the_writer.file = file;
    lock->the_writer.line = line;
    lock->readers++;
    lock->state = lock_state_riw;
    lock->writer = me;
#ifdef RECURSIVE_LOCKS
    lock->recursive_lock_depth = 1;
#endif
    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}


void
lock__write(mos_lock_t lock, error_status_t *st, char *file, int line)
{
    pthread_t		me = pthread_self();
    int                 is_me;

    LOCK_OR_RETURN(lock, st);

    is_me = pthread_equal(me, lock->writer);

#ifdef RECURSIVE_LOCKS

    /* If we're going into another level of recursive write (already setup
     * by a previous call to lock__write), then just return.  Note that
     * recursive_lock_depth was already incremented by RIW above.
     */

    if (lock->state == lock_state_write && is_me) {
	UNLOCK_OR_RETURN(lock, st);
	*st = error_status_ok;
	return;
    }	

#endif				/* RECURSIVE_LOCKS */

    /* Must have RIW to get the write lock.  So we must verify:
     *  that lock state is RIW; and
     *  current threads owns the RIW. */
    dce_assert(NULL, lock->state == lock_state_riw && is_me);

    /* Readers and Writers block. The only question here is if the state
     * was RIW and this is the owner of the lock and there are no other
     * readers, then we can proceed to write. Otherwise we have to wait. */
    while (lock->readers > 1) {
	BLOCK_OR_RETURN(lock, st);
    }

    /* The last riwer becomes a writer now. */
    lock->readers = 0;
    lock->state = lock_state_write;
    lock->writer = me;
    lock->the_writer.file = file;
    lock->the_writer.line = line;

    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}


void
lock__justwrite(mos_lock_t lock, error_status_t *st, char *file, int line)
{
    error_status_t	st2;

    lock__riw(lock, st, file, line);
    if (*st == error_status_ok) {
	lock__write(lock, st, file, line);
	if (*st != error_status_ok)
	    lock__unlock(lock, &st2, file, line);
    }
}


void
lock__unlock(mos_lock_t lock, error_status_t *st, char *file, int line)
{
    pthread_t		me;

    LOCK_OR_RETURN(lock, st);

    me = pthread_self();
    switch (lock->state) {
    case lock_state_free:
	/* The lock must be held in order to release it! */
	dce_assert(NULL, lock->state != lock_state_free);
	break;
    case lock_state_read:
	/* Decrement the readers. If zero, then only a writer might be
	 * sleeping on the cv, so wake it up. */
	lock->readers--;
	if (lock->readers == 0) {
	    WAKE_ONE_OR_RETURN(lock, st);
	    lock->state = lock_state_free;
	}
	break;
    case lock_state_riw:
	dce_assert(NULL, pthread_equal(me, lock->writer));

	/* If this is the last layer in a recursive unlock then do all the
	 * normal processing, otherwise done */

#ifdef RECURSIVE_LOCKS
	if (--lock->recursive_lock_depth == 0)
#endif				/* RECURSIVE_LOCKS */
	{

	    /* Decrement the readers. If zero, might be another RIW or a W so
	     * must broadcast. If I was the owner of the lock, then unmark
	     * me and make the RIW lock a R lock. */

	    lock->readers--;
	    if (lock->readers == 0) {
		WAKE_ALL_OR_RETURN(lock, st);
		lock->state = lock_state_free;
	    }
	    else if (pthread_equal(me, lock->writer))
		lock->state = lock_state_read;
	}

	break;
    case lock_state_write:
	dce_assert(NULL, pthread_equal(me, lock->writer));

	/* Writer resets the lock and wakes up any RIW or R that might be
	 * sleeping. If we're not the writer, this is an error! */
#ifdef RECURSIVE_LOCKS
	if (--lock->recursive_lock_depth == 0)
#endif				/* RECURSIVE_LOCKS */
	{
	    lock->readers = 0;
	    lock->state = lock_state_free;
	    WAKE_ALL_OR_RETURN(lock, st);
	}
	break;
    }

    UNLOCK_OR_RETURN(lock, st);

    *st = error_status_ok;
}

