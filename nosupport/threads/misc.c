/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: misc.c,v $
 * Revision 1.1.2.2  1996/12/05  08:53:57  rsalz
 * 	Final version
 * 	[1996/12/05  08:53:36  rsalz]
 *
 * Revision 1.1.2.1  1996/11/25  17:21:08  rsalz
 * 	Created.
 * 	[1996/11/25  17:00:39  rsalz]
 * 
 * $EndLog$
 */

#include <dcepthread.h>

int pthread_attr_init(pthread_attr_t *t)
    { return pthread_attr_create(t) < 0 ? errno : 0; }
int pthread_attr_destroy(pthread_attr_t *t)
    { return pthread_attr_delete(t) < 0 ? errno : 0; }

int xpthread_attr_setinheritsched(pthread_attr_t *t,int i)
    { return pthread_attr_setinheritsched(t, i) < 0 ? errno : 0; }
int xpthread_attr_getinheritsched(pthread_attr_t t, int*ip)
    { return (*ip = pthread_attr_getinheritsched(t)) < 0 ? errno : 0; }
int xpthread_attr_setstacksize(pthread_attr_t * t,long l)
    { return pthread_attr_getstacksize(t, l) < 0 ? errno : 0; }
int xpthread_attr_getstacksize(pthread_attr_t t, long* lp)
    { return (*lp = pthread_attr_getstacksize(t)) < 0L ? errno : 0; }

int xpthread_create(pthread_t *p,pthread_attr_t*t,void (*f)(void*),void*a)
    { return (pthread_create(p, *t, f, a) < 0 ? errno : 0; }
int xpthread_detach(pthread_t p)
    { return pthread_detach(&p) < 0 ? errno : 0; }
int xpthread_join(pthread_t t,void**ap)
    { return pthread_join(t, ap) < 0 ? errno : 0; }

int pthread_mutexattr_init(pthread_mutexattr_t *ma)
    { return pthread_mutexattr_create(ma) < 0 ? errno : 0; }
int pthread_mutexattr_destroy(pthread_mutexattr_t *ma)
    { return pthread_mutexattr_deleye(ma) < 0 ? errno : 0; }
int xpthread_mutex_init(pthread_mutex_t *m,pthread_mutexattr_t *t)
    { pthread_mutex_init(m, *t) < 0 ? errno : 0; }
int xpthread_mutex_destroy(pthread_mutex_t *m)
    { return pthread_mutex_destroy(m) < 0 ? errno : 0; }
int xpthread_mutex_lock(pthread_mutex_t *m)
    { return pthread_mutex_lock(m) < 0 ? errno : 0; }
int xpthread_mutex_trylock(pthread_mutex_t *m)
    { return pthread_mutex_trylock(m) < 0 ? errno : 0; }
int xpthread_mutex_unlock(pthread_mutex_t *m)
    { return pthread_mutex_unlock(m) < 0 ? errno : 0; }

int pthread_condattr_init(pthread_condattr_t *ca)
    { return pthread_condattr_create(ca) < 0 ? errno : 0; }
int pthread_condattr_destroy(pthread_condattr_t *ca)
    { return pthread_condattr_delete(ca) < 0 ? errno : 0; }

int xpthread_cond_init(pthread_cond_t *c,pthread_condattr_t ca)
    { return pthread_cond_init(c, ca) < 0 ? errno : 0; }
int xpthread_cond_destroy(pthread_cond_t *c)
    { return pthread_cond_destroy(c) < 0 ? errno : 0; }
int xpthread_cond_broadcast(pthread_cond_t *c)
    { return pthread_cond_broadcast(c) < 0 ? errno : 0; }
int xpthread_cond_signal(pthread_cond_t *c)
    { return pthread_cond_signal(c) < 0 ? errno : 0; }
int xpthread_cond_wait(pthread_cond_t *c,pthread_mutex_t *m)
    { return pthread_cond_wait(c, m) < 0 ? errno : 0; }
int xpthread_cond_timedwait(pthread_cond_t *c,pthread_mutex_t *m,struct timespec *ts)
    { return pthread_cond_timedwait(c, m, ts) < 0 ? errno : 0; }

int xpthread_once(pthread_once_t *o,void (*f)(void))
    { return pthread_once(o, f) < 0 ? errno : 0; }

int xpthread_keycreate(pthread_key_t *k,void (*f)(void))
    { return pthread_keycreate(k, f) < 0 ? errno : 0; }
int xpthread_setspecific(pthread_key_t k,void*v)
    { return pthread_setspecific(k, v) < 0 ? errno : 0; }
void *xpthread_getspecific(pthread_key_t k)
    { void *vp; return pthread_getspecific(k, &vp) < 0 ? NULL : vp; }

int xpthread_cancel(pthread_t t)
    { return pthread_cancel(t) < 0 ? errno : 0; }

int pthread_setcancelstate(int i, int*ip)
    { *ip = pthread_setcancel(i); return 0; }
int pthread_setcanceltype(int i, int*ip)
    { *ip = pthread_setasynccancel(i); return 0; }
