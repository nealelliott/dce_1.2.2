/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fifo.h,v $
 * Revision 1.1.67.1  1996/10/02  17:42:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:52  damon]
 *
 * $EndLog$
 */

/* fifo.h -- Definitions for Episode fifo package.  This is a macros only
 *     facility.  An simple usage example is given below. */

/* Copyright (C) 1994, 1992 Transarc Corporation - All rights reserved. */

/*
 * $Header: /u0/rcs_trees/dce/rcs/file/episode/tools/fifo.h,v 1.1.67.1 1996/10/02 17:42:05 damon Exp $
 */

#if !defined(TRANSARC_EPI_FIFO_H)
#define TRANSARC_EPI_FIFO_H

#include <dcedfs/stds.h>

/* Here is an example of how to use the fifo package excerpted from usage in
 * anode/block_alloc.c file.  Also see logbuf/buffer.c for more examples.
 *
 * Consider this sample structure and fifo head:
 *
 * struct freeTrans {
 *     ... misc fields ... 
 *     struct fifo_Thread active;
 *     ... more misc fields ... 
 * };
 * PRIVATE struct fifo activeTrans;
 *
 * We need to pass both the fifo head and the offset between the objects
 * being added to the fifo and the thread structure used to link them
 * together.  This allows objects to be on multiple lists and for the
 * threads to be placed as convenient.  It does, however, complicate the
 * initialization call with funny looking pointer arithmetic.
 *
 * void Init ()
 * {   struct freeTrans *tmp;
 *     FIFO_INIT (&fbl->activeTrans, (char *)&tmp->active - (char *)tmp);
 * }
 *
 * To add an object to a fifo you just call add.  As mentioned below the init
 * thread macro is not necessary and could probably be eliminiated.
 *
 *     struct freeTrans *ft = osi_Alloc (struct freeTrans);
 *     FIFO_ADD (&activeTrans, &ft->active);
 *
 * Here is an example of how to walk though a fifo looking for something, then
 * remove it from the fifo.  Note that it is not kosher to remove an element
 * while traversing fifo it is in.
 *
 * The (opaque) casts in these macros should avoid the need for explicit type
 * casts, but the older pmax compiler doesn't do this very well.
 *
 * {   struct freeTrans *ft, *best_ft;
 *
 *     for (ft = FIFO_LookAtOldest(&activeTrans,
 *				   offsetof(struct freeTrans, active));
 *          ft;
 *          ft = FIFO_NextOldest(&activeTrans, ft, &ft->active)) {
 *         if (ft->flags & FB_TRANS_ENDED) best_ft = ft;
 *     }
 *     FIFO_REMOVE (&activeTrans, &best_ft->active);
 * }
 */

struct fifo_Thread {
    struct fifo_Thread *next;
    struct fifo_Thread *prev;
};

struct fifo {
    u_int32 threadOffset;		/* byte offset of fifo thread */
    u_int32 length;			/* length of fifo */
    struct fifo_Thread thread;		/* circular */
};

#define IsFifoEmpty(f) ((f)->length == 0)
#define fifo_ThreadOf(fifo, b) \
    ((struct fifo_Thread *)((char *)(b) + (fifo)->threadOffset))

#define FIFO_INIT(fifo, off) \
((fifo)->thread.next=(fifo)->thread.prev= &(fifo)->thread,(fifo)->threadOffset=(off),(fifo)->length=0)

/* FIFO_INITTHREAD -- Clears the thread fields of an object.  This isn't
 *     necessary if followed by a FIFO_ADD, which is the common case. */

#define FIFO_INITTHREAD(t) \
((t)->prev = (t)->next = 0)

/* FIFO_ADD -- Insert an object at the newest end of a fifo. */

#define FIFO_ADD(f, e) \
((f)->length++, (e)->prev = &(f)->thread, (e)->next = (f)->thread.next, \
 (f)->thread.next->prev = (e), (f)->thread.next = (e))

/* FIFO_ADD_OLDEST -- Insert an object at the oldest end of a fifo. */

#define FIFO_ADD_OLDEST(f, e) \
((f)->length++, (e)->next = &(f)->thread, (e)->prev = (f)->thread.prev, \
 (f)->thread.prev->next = (e), (f)->thread.prev = (e))

/* FIFO_REMOVE -- extract an object from a fifo.  This also calls
 *     FIFO_INITTHREAD, but as mentioned above this is unnecessary except as a
 *     debugging aid. */

#define FIFO_REMOVE(f, e) \
((f)->length--, (e)->next->prev = (e)->prev, (e)->prev->next = (e)->next, \
 FIFO_INITTHREAD(e))

#define FIFO_LOOKATOLDEST(f) ((f)->thread.prev)

#define FIFO_LOOKATNEWEST(f) ((f)->thread.next)

#define FIFO_Off2Obj(t, o) ((char *)(t) - (unsigned)(o))

/* Cope with weird HP compiler bug; also see FIFO_NextOldest */
#ifdef	AFS_HPUX_ENV
#define FIFO_Off2ObjHP(t, o) ((char *)(t) + (unsigned)(o))
#endif	/* AFS_HPUX_ENV */

/*
 * These calls are the "civilized" versions which perform the appropriate
 * thread arithmetic.  Everything is still an expression!  See above for usage
 * of these macros to enumerate a fifo.
 */

/* To avoid spurious casts, which should be unnecessary on a ANSI C compiler we
 * will cast the result to (void *).  However, old pmax compiers don't handle
 * this correctly and the standard AIX3.1 compiler doesn't assert __STDC__ even
 * though it does handle (void *) correctly.  Therefore the following kludge.
 * */

#if defined(__STDC__) || defined(_IBMR2)
#define FIFO_OPAQUE void *
#else
#define FIFO_OPAQUE char *
#endif

/* Cope with weird HP compiler bug */
#ifdef	AFS_HPUX_ENV
#define FIFO_NextOldest(f, e, t) \
    ((FIFO_OPAQUE)((t)->prev == (&(f)->thread) ? 0 : \
		   FIFO_Off2ObjHP(((t)->prev), ((char *)(e)-(char *)(t)))))
#else
#define FIFO_NextOldest(f, e, t) \
    ((FIFO_OPAQUE)((t)->prev == (&(f)->thread) ? 0 : \
		   FIFO_Off2Obj(((t)->prev), ((char *)(t)-(char *)(e)))))
#endif

#define FIFO_LookAtOldest(f, off) \
    ((FIFO_OPAQUE)(IsFifoEmpty(f) ? 0 : \
		   FIFO_Off2Obj(FIFO_LOOKATOLDEST(f), off)))

#define FIFO_LookAtNewest(f, off) \
    ((FIFO_OPAQUE)(IsFifoEmpty(f) ? 0 : \
		   FIFO_Off2Obj(FIFO_LOOKATNEWEST(f), off)))

/* Cope with weird HP compiler bug */
#ifdef	AFS_HPUX_ENV
#define FIFO_NextNewest(f, e, t) \
    ((FIFO_OPAQUE)((t)->next == (&(f)->thread) ? 0: \
		   FIFO_Off2ObjHP(((t)->next), ((char *)(e)-(char *)(t)))))
#else
#define FIFO_NextNewest(f, e, t) \
    ((FIFO_OPAQUE)((t)->next == (&(f)->thread) ? 0: \
		   FIFO_Off2Obj(((t)->next), ((char *)(t)-(char *)(e)))))
#endif

/* FIFO_FOR -- is a handy macro for iterating over the contents of a fifo from
 *     oldest to newest.  Unless you know what you're doing, it is best to use
 *     this only when the fifo isn't being changed by the body of the loop.
 *
 * PARAMETERS -- vbl is a local variable declared by the caller to be used as
 *     the loop index.  The thread argument is the fifo_Thread field relative
 *     to vbl.
 *
 * CAUTIONS -- the initialization of vbl is a NON-CONFORMING construct to
 *     supress warnings about possible use of uninitialized data in the address
 *     arithmetic used to calculate the thread offset.  The idea is to use the
 *     type of the local variable instead of requiring an explicit type
 *     argument. */

#define FIFO_FOR(vbl, fifo, thread) \
    for ((vbl = NULL, \
	  vbl = FIFO_LookAtOldest(fifo, (char *)&vbl->thread - (char *)vbl)); \
	 vbl; \
	 vbl = FIFO_NextOldest(fifo, vbl, &vbl->thread))

#endif /* TRANSARC_EPI_FIFO_H */

