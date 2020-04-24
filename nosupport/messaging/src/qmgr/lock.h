/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Definitions for lock manager.  Taken from DCE-Web distribution.
 * ______________________________________________________________________
 *
 * $Log: lock.h,v $
 * Revision 1.1  1996/09/27  18:03:14  bowe
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

/*
** Lock manager.
*/

typedef struct lock_s_t *mos_lock_t;

#if	!defined(LOCK__FILE__)
#define LOCK__FILE__		__FILE__
#endif	/* !defined(LOCK__FILE__) */

#define lock_create(l,s)	\
	lock__create(l, s, LOCK__FILE__, __LINE__)
#define lock_delete(l,s)	\
	lock__delete(l, s, LOCK__FILE__, __LINE__)
#define lock_read(l,s)	\
	lock__read(l, s, LOCK__FILE__, __LINE__)
#define lock_riw(l,s)	\
	lock__riw(l, s, LOCK__FILE__, __LINE__)
#define lock_write(l,s)	\
	lock__write(l, s, LOCK__FILE__, __LINE__)
#define lock_justwrite(l,s)	\
	lock__justwrite(l, s, LOCK__FILE__, __LINE__)
#define lock_unlock(l,s)	\
	lock__unlock(l, s, LOCK__FILE__, __LINE__)

#if     defined(CAN_RELOAD_CONFIG)
#define LOCK_JUSTWRITE(l, st)   lock_justwrite((l), (st))
#define LOCK_READ(l, st)        lock_read((l), (st))
#define LOCK_UNLOCK(l, st)      lock_unlock((l), (st))
#else
#define LOCK_JUSTWRITE(l, st)   /* NULL */
#define LOCK_READ(l, st)        /* NULL */
#define LOCK_UNLOCK(l, st)      /* NULL */
#endif  /* defined(CAN_RELOAD_CONFIG) */

extern void lock__create(
    mos_lock_t	*		/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void lock__delete(
    mos_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void lock__read(
    mos_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void lock__riw(
    mos_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void lock__write(
    mos_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void lock__justwrite(
    mos_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void lock__unlock(
    mos_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);
