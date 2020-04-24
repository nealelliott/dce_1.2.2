/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: threads_lib.h,v $
 * Revision 1.1.9.2  1996/02/18  23:34:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:39  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:18:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:56  root]
 * 
 * Revision 1.1.7.6  1994/09/22  18:35:25  mccann
 * 	move defines to cds.mk
 * 	[1994/09/22  18:33:35  mccann]
 * 
 * Revision 1.1.7.5  1994/08/03  19:02:48  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:16  mccann]
 * 
 * Revision 1.1.7.4  1994/06/30  19:15:43  mccann
 * 	sams cleanup drop 2
 * 	[1994/06/30  18:01:54  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  18:39:39  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:20  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/14  14:39:50  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:57:59  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:04:36  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:20:32  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  13:51:06  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:55:12  zeliff]
 * 
 * Revision 1.1.5.2  1992/10/26  18:22:37  peckham
 * 	OT#4001: Remove cma_import_fd() reference.
 * 	[1992/10/26  18:21:59  peckham]
 * 
 * Revision 1.1.3.2  1992/05/13  18:52:12  peckham
 * 	CR3187: Fix cast in dthread_cleanup_push().
 * 	dthread_cleanup_push() and dthread_cleanup_pop() do not return values.
 * 	Prototypes for th_*{init|term}(); toss th_init() results.
 * 	[1992/05/13  18:03:54  peckham]
 * 
 * Revision 1.1  1992/01/19  15:16:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */
#ifndef _THREADS_LIB_H
#define _THREADS_LIB_H

#include <sys_time.h>		/* must be before pthread.h */
#include <sys/types.h>		/* required by socket.h on non-BSD ULTRIX */
#if !defined(SOCK_STREAM)	/* socket.h not idempotent in BSD ULTRIX */
# include <sys/socket.h>	/* required for prototypes */
#endif

#include <pthread.h>
#include <dce/dns_config.h>

#if defined(DTHREAD_WRAPPERS)

/*
 * Sample decisions for the environment types
 */
typedef	void			*dthread_address_t;

typedef float			dthread_interval_t;

typedef time_local_t		dthread_date_time_t;

/*
 * Sample decisions for what a handle should be
 */
typedef	dthread_address_t	dthread_handle;
#define	dthread_handle_size sizeof(dthread_handle)
#define	dthread_handle_init ((dthread_address_t)0)

typedef	dthread_handle		dthread_mutex_t;

typedef	dthread_handle		dthread_t;

typedef	dthread_address_t	(*dthread_start_routine) (dthread_address_t);

typedef void			(*dthread_destructor) (void);

typedef pthread_key_t		dthread_key_t;

typedef	dthread_handle		dthread_cond_t;

# if (defined(__VMS) && defined(__alpha))
#  define dthread_null cma_c_null
# else 
   extern dthread_handle		dthread_null;
# endif

#define	dthread_assign(handle1, handle2) *(handle2) = (handle1)

#define	dthread_equal(handle1, handle2) (handle1 == handle2)

typedef struct dthread_cleanup_handler {
    struct dthread_cleanup_handler *ch_lnk;
    void		(*ch_rtn)(dthread_address_t);
    dthread_address_t	ch_arg;
    int			ch_ex;
} dthread_cleanup_handler_t;

int
dthread_join (
    dthread_t		,
    dthread_address_t	*);

int
dthread_detach (
    dthread_t		*);

void
dthread_exit (
    dthread_address_t	);

dthread_t
dthread_self (
    void);

int
dthread_yield (
    void);

int
dthread_delay (
    dthread_interval_t	);


int
dthread_create_mutex (
    dthread_mutex_t	*);

int
dthread_delete_mutex (
    dthread_mutex_t	*);

int
dthread_lock (
    dthread_mutex_t	*);

int
dthread_trylock (
    dthread_mutex_t	*);

int
dthread_unlock (
    dthread_mutex_t	*);

int
dthread_create_condition (
    dthread_cond_t	*);

int
dthread_delete_condition (
    dthread_cond_t	*);

int
dthread_broadcast (
    dthread_cond_t	*);

int
dthread_signal (
    dthread_cond_t	*);

int
dthread_wait (
    dthread_cond_t	*,
    dthread_mutex_t	*);

int
dthread_timed_wait (
    dthread_cond_t	*,
    dthread_mutex_t	*,
    dthread_date_time_t	*);

int
dthread_create_key (
    dthread_destructor	,
    dthread_key_t	*);

int
dthread_set_context (
    dthread_key_t	,
    dthread_address_t	);

int
dthread_get_context (
    dthread_key_t	,
    dthread_address_t	*);

int
dthread_delete_context (
    dthread_key_t	);

int
dthread_cancel (
    dthread_t		);

void
dthread_handler_push (
    dthread_cleanup_handler_t *);

void
dthread_handler_pop (
    dthread_cleanup_handler_t *);

#define	dthread_cleanup_push(rtn,arg) { \
    dthread_cleanup_handler_t __cleanup_handler; \
    __cleanup_handler.ch_rtn = ((void (*)( dthread_address_t ))rtn); \
    __cleanup_handler.ch_arg = (dthread_address_t)(arg); \
    __cleanup_handler.ch_ex = -1; \
    dthread_handler_push(&__cleanup_handler); \
    pthread_cleanup_push(dthread_handler_pop,&__cleanup_handler);

#define	dthread_cleanup_pop(ex) __cleanup_handler.ch_ex = (ex); \
    if (!__cleanup_handler.ch_ex) dthread_handler_pop(&__cleanup_handler); \
    pthread_cleanup_pop(__cleanup_handler.ch_ex); }

int
dthread_setcancel (
    int			);

void
dthread_testcancel (
    void                );

int
dthread_get_errno (
    void                );

void
dthread_set_errno (
    int			);

/* blocking calls under thread control */

#if !defined(__VMS) && !defined(__alpha)

int
dthread_accept (
    int			,
    struct sockaddr	*,
    int			*);
int
dthread_connect (
    int			,
    struct sockaddr	*,
    int			);

int
dthread_recvfrom (
    int			,
    char		*,
    int			,
    int			,
    struct sockaddr	*,
    int			*);

int
dthread_sendto (
    int			,
    char		*,
    int			,
    int			,
    struct sockaddr	*,
    int			);

#endif /* !vms && !alpha */

#define	dthread_bind		bind

int
dthread_close (
    int			);

int
dthread_creat (
    char		*,
    int			);

#define	dthread_dup		dthread_notimplemented

#define	dthread_dup2		dthread_notimplemented

#define	dthread_listen		listen

int
dthread_open (
    char		*,
    int			,
    int			);

#define	dthread_pipe		dthread_notimplemented

int
dthread_read (
    int			,
    char		*,
    int			);

#define	dthread_readv		dthread_notimplemented

#define	dthread_recv		dthread_notimplemented

#define	dthread_recvmsg		dthread_notimplemented

#define	dthread_select		dthread_notimplemented

#define	dthread_send		dthread_notimplemented

#define	dthread_sendmsg		dthread_notimplemented

int
dthread_socket (
    int			,
    int			,
    int			);

#define	dthread_socketpair	dthread_notimplemented

int
dthread_write (
    int			,
    char		*,
    int			);

#define	dthread_writev		dthread_notimplemented

/* special cma-like call to make a fd non-blocking */

void
dthread_import_fd (
    int			);

/* old select() support */

int
dthread_read_wait (
    int			);

int
dthread_read_cancel (
    int			);

int
dthread_write_wait (
    int			);

int
dthread_both_wait (
    int			);

int
th_init (
    void);

int
th_term (
    void);

# define dthread_init() { (void)th_init(); }
# define dthread_term() { (void)th_term(); }

#else	/* !DTHREAD_WRAPPERS */

/*
 * Sample decisions for the environment types
 */
typedef	pthread_addr_t		dthread_address_t;

typedef float			dthread_interval_t;

typedef struct timespec		dthread_date_time_t;

typedef	pthread_mutex_t		dthread_mutex_t;

typedef	pthread_t		dthread_t;

typedef	pthread_startroutine_t	dthread_start_routine;

typedef pthread_destructor_t	dthread_destructor;

typedef pthread_key_t		dthread_key_t;

typedef	pthread_cond_t		dthread_cond_t;

#define	dthread_assign(handle1, handle2) *(handle2) = (handle1)

#define	dthread_equal		pthread_equal

#define	dthread_join \
		pthread_join
#define	dthread_detach(t) \
		pthread_detach(t)
#define	dthread_exit \
		pthread_exit
#define	dthread_self \
		pthread_self
#define	dthread_yield \
		pthread_yield
#define	dthread_create_mutex(ma) \
		pthread_mutex_init(ma,pthread_mutexattr_default)
#define	dthread_delete_mutex \
		pthread_mutex_destroy
#define	dthread_lock \
		pthread_mutex_lock
#define	dthread_trylock \
		pthread_mutex_trylock
#define	dthread_unlock \
		pthread_mutex_unlock
#define	dthread_create_condition(ca) \
		pthread_cond_init(ca,pthread_condattr_default)
#define	dthread_delete_condition \
		pthread_cond_destroy
#define	dthread_broadcast \
		pthread_cond_broadcast
#define	dthread_signal \
		pthread_cond_signal
#define	dthread_wait \
		pthread_cond_wait
#define	dthread_timed_wait \
		pthread_cond_timedwait
#define	dthread_create_key(a,b) \
		pthread_keycreate(b,a)
#define	dthread_set_context \
		pthread_setspecific
#define	dthread_get_context \
		pthread_getspecific
#define	dthread_delete_context(key)	0
#define	dthread_cancel(t) \
		pthread_cancel(t)
#define	dthread_cleanup_push(ra,a) \
		pthread_cleanup_push(((void (*)(dthread_address_t))ra),((pthread_addr_t)a))
#define	dthread_cleanup_pop \
		pthread_cleanup_pop
#define	dthread_setcancel \
		pthread_setcancel
#define	dthread_testcancel \
		pthread_testcancel
#define	dthread_delay \
		dthread_delay_np
#define	dthread_main() \
		(pthread_exit((pthread_addr_t)20), 0)
#define	dthread_get_errno() \
		errno
#define	dthread_set_errno

int
dthread_delay_np (
    dthread_interval_t	);


/*
 * Unix system calls
 */
#define	dthread_accept			accept
#define	dthread_bind			bind
#define	dthread_close			close
#define	dthread_connect			connect
#define	dthread_creat			creat
#define	dthread_dup			dup
#define	dthread_dup2			dup2
#define	dthread_listen			listen
#define	dthread_open			open
#define	dthread_pipe			pipe
#define	dthread_read			read
#define	dthread_readv			readv
#define	dthread_recv			recv
#define	dthread_recvfrom		recvfrom
#define	dthread_recvmsg			recvmsg
#define	dthread_select			select
#define	dthread_send			send
#define	dthread_sendmsg			sendmsg
#define	dthread_sendto			sendto
#define	dthread_socket			socket
#define	dthread_socketpair		socketpair
#define	dthread_write			write
#define	dthread_writev			writev

#define dthread_import_fd(fd)		0

/*
 * Incompatable data
 */
#define dthread_null cma_c_null

# define dthread_init() 0
# define dthread_term() 0

#endif	/* DTHREAD_WRAPPERS */

/*
 * Unconditionally wrap thread creation calls for stack specification.
 */

typedef struct dthread_attr {
    char		*name;
    long		stacksize;
} dthread_attr_t;
#define dthread_attr_default ((dthread_attr_t *)0)

int
dthread_create (
    dthread_t		  *,
    dthread_attr_t	  *,
    dthread_start_routine ,
    dthread_address_t	  );

int
dthread_create_with_stack (
    dthread_t		  *,
    dthread_start_routine ,
    dthread_address_t	  ,
    unsigned int	  );

int
dthread_get_expiration (
    time_local_t	*,
    dthread_date_time_t	*);

/*
 * The once-only mechanism
 */
#define	dthread_once_t		pthread_once_t
#define	dthread_once_init	pthread_once_init
#define	dthread_initroutine_t	pthread_initroutine_t
#define	dthread_once		pthread_once


/*
 * diagnostics support
 */
int
dthread_diag_threads (
    int              ,
    int              ,
    dthread_t        *,
    dthread_mutex_t  *,
    dthread_cond_t   *,
    int              (*)(char *,void *),
    void	     *);


void
dthread_diag_log_thread (
    dthread_t  ,
    char       *);


/*
 * Prototype from sec_init.c
 */
int 
server_sec_init (
    unsigned char   *,
    char            *,
    int              ,
    unsigned char   *,
    uuid_p_t         ,
    unsigned char   *,
    uuid_p_t         ); 


#endif  /* #ifndef _THREADS_LIB_H */
