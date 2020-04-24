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
 * $Log: dce_lock.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:18  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:48:51  root]
 * 
 * Revision 1.1.2.6  1994/09/26  17:49:19  rsalz
 * 	Convert to true SVC functions (OT CR 11725).
 * 	[1994/09/26  17:11:58  rsalz]
 * 
 * Revision 1.1.2.5  1994/08/03  20:36:14  rsalz
 * 	Fix OT CR 11506.
 * 	[1994/08/03  20:31:55  rsalz]
 * 
 * Revision 1.1.2.4  1994/07/05  16:03:22  rsalz
 * 	Add dce_lock_justwrite (part of OT CR 11173).
 * 	[1994/07/05  15:57:21  rsalz]
 * 
 * Revision 1.1.2.3  1994/06/09  16:03:04  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  20:15:08  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:37  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/05  20:07:27  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:00:54  rsalz]
 * 
 * 	Rename dced_lock.h to dce_lock.h
 * 	[1994/04/29  15:20:53  rsalz]
 * 
 * $EndLog$
 */

/*
** Lock manager.  Based on code by Jonathan E. Chinitz <jec@isoft.com>.
** 
** Definitions for Lock manager code
** The lock manager allows simultaneous readers to co-exist. Only 1 RIW with
** many readers is allowed, others block. Writers always block all other
** readers and riw's.
** The interesting transitions are as follows:
**    a Writer can only promote a lock that it owns, otherwise it blocks
**    a RIW conflicts with another so it blocks too
**    a Reader unlock will signal if N=0, the only sleeper can be a Writer
**    a RIW unlock will broadcast if N=0, can be another RIW or Writer
**    a Writer unlock will broadcast if N=0, can be any other kind
**
** The state machine looks like this:
**
**	  STATE	| NONE	    READ	RIW		WRITE
** OPERATION	|---------------------------------------------------
**  create	| set state to NONE
** -------------|---------------------------------------------------
**  destroy	| OK	    ERROR	ERROR		ERROR
** -------------|---------------------------------------------------
**  read	| READ	    READ	RIW		WRITE
**		| N=1	    N++		N++		block
** -------------|---------------------------------------------------
**  riw		| RIW	    RIW		RIW		WRITE
**		| N=1	    N++		block		block
** -------------|---------------------------------------------------
**  write	| ERROR	    READ	WRITE if	WRITE
**		|	    block	owner and	block
**		|			N=1 else RIW
**		|			and block
** -------------|---------------------------------------------------
**  unlock	| ERROR	    NONE (N=0)	NONE (N=0)	NONE
**		|	    READ (N>0)	READ (N>0	N=0
**		|	    N--		  && owner)	broadcast
**		|	    signal(N=0)	RIW (N>0
**		|			  && !owner)
**		|			N--
**		|			broadcast(N=0)    
** -------------|---------------------------------------------------
*/
#if	!defined(_DCE_LOCK_H)
#define _DCE_LOCK_H

typedef struct dce_lock_s_t *dce_lock_t;

#if	!defined(LOCK__FILE__)
#define LOCK__FILE__		__FILE__
#endif	/* !defined(LOCK__FILE__) */

#define dce_lock_create(l,s)	\
	dce_lock__create(l, s, LOCK__FILE__, __LINE__)
#define dce_lock_delete(l,s)	\
	dce_lock__delete(l, s, LOCK__FILE__, __LINE__)
#define dce_lock_read(l,s)	\
	dce_lock__read(l, s, LOCK__FILE__, __LINE__)
#define dce_lock_riw(l,s)	\
	dce_lock__riw(l, s, LOCK__FILE__, __LINE__)
#define dce_lock_write(l,s)	\
	dce_lock__write(l, s, LOCK__FILE__, __LINE__)
#define dce_lock_justwrite(l,s)	\
	dce_lock__justwrite(l, s, LOCK__FILE__, __LINE__)
#define dce_lock_unlock(l,s)	\
	dce_lock__unlock(l, s, LOCK__FILE__, __LINE__)
#define dce_lock_hold_lock(l,b,s)	\
	dce_lock__hold_lock(l, b, s, LOCK__FILE__, __LINE__)

extern void dce_lock__create(
    dce_lock_t*			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void dce_lock__delete(
    dce_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void dce_lock__read(
    dce_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void dce_lock__riw(
    dce_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void dce_lock__write(
    dce_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void dce_lock__justwrite(
    dce_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void dce_lock__unlock(
    dce_lock_t			/* lock */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);

extern void dce_lock__hold_lock(
    dce_lock_t			/* lock */,
    boolean32*			/* b */,
    error_status_t*		/* st */,
    char*			/* file */,
    int				/* line */
);
#endif	/* !defined(_DCE_LOCK_H) */
