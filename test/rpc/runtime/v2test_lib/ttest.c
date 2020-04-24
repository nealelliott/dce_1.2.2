/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: ttest.c,v $
 * Revision 1.1.8.2  1996/02/17  23:24:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:25  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:15:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:26  root]
 * 
 * Revision 1.1.5.3  1993/01/12  00:03:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:26:15  bbelch]
 * 
 * Revision 1.1.5.2  1992/09/29  21:24:18  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:48:58  weisman]
 * 
 * Revision 1.1.2.2  1992/05/01  15:42:25  rsalz
 * 	Part of rpc6 code drop.
 * 	[1992/05/01  01:31:42  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:42:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifdef TEST_RPC
#include "commonp.h" 
#include "com.h" 
#include "dg.h" 
#else
#include <sys/time.h>
#include <stdio.h>
#ifdef EXC
#include <pthread_exc.h>
#else
#include <pthread.h>
#endif
#endif

#ifdef apollo
#ifndef TEST_RPC
#include <apollo/base.h>
#endif
#include <apollo/mutex.h>
#endif

void c_time(
#ifdef __STDC__
    struct timeval *
#endif
);

unsigned long e_time(
#ifdef __STDC__
    struct timeval *
#endif
);

struct test_t {
    char *name;
    void (*proc)(void);
};

extern struct test_t tests[];

#define TEST(name) {"name", name}

#ifdef TEST_RPC
#define RPCTEST(name) TEST(name)
#else
#define RPCTEST(name) {"name", NULL}
#endif

#ifdef apollo
#define APOLLOTEST(name) TEST(name)
#else
#define APOLLOTEST(name) {"name", NULL}
#endif

#ifdef apollo
mutex_$lock_rec_t mlock;
#endif

pthread_cond_t cond;
pthread_mutex_t mutex;

main(argc, argv)
int argc;
char *argv[];
{
    unsigned long delta;
    int count, i, test;
    struct timeval timer;

    test = atoi(argv[1]);
    count = atoi(argv[2]);

    if (tests[test].proc == NULL)
    {
        fprintf(stderr, "Test %s not implemented\n", tests[test].name);
        exit(1);
    }

    printf("Test = %s (%d), Count = %d\n", tests[test].name, test, count);

#ifdef TEST_RPC
    RPC_INIT(0);
    RPC_DG_INIT();
#endif

#ifdef apollo
    mutex_$init(&mlock);
#endif

    pthread_cond_init(&cond, pthread_condattr_default); 
    pthread_mutex_init(&mutex, pthread_mutexattr_default); 

    c_time(&timer);

    for (i = 0; i < count; i++)
        (*tests[test].proc)();

    delta = e_time(&timer);

    printf("    usec/pass = %lu\n", delta / count);
}

#ifdef TEST_RPC

static void xq_alloc_free()
{
    rpc_dg_xmitq_elt_p_t p;

    p = rpc__dg_xmitq_elt_alloc();
    rpc__dg_xmitq_elt_free(p);
}


static void xq_alloc_free_pre_opt()
{
    rpc_dg_xmitq_elt_p_t p;

    p = rpc__dg_xmitq_elt_alloc();
    RPC_DG_XMITQ_ELT_FREE(p);
}


static void rpc_lock_unlock()
{
    RPC_LOCK();
    RPC_UNLOCK();
}

#endif

static void nothing()
{
}


static void foo()
{
}

static pthread_once_t onceblock = pthread_once_init;

static void once()
{
    pthread_once(&onceblock, foo);
}

#ifdef apollo

static void cps_mutex()
{
    mutex_$lock(&mlock, mutex_$wait_forever);
    mutex_$unlock(&mlock);
}

#endif


static void cond_signal()
{
    pthread_cond_signal(&cond);
}


static void data_move()
{
    static char buff1[1024], buff2[1024];

    bcopy(buff1, buff2, sizeof buff2);
}


static void lock_unlock()
{
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
}


static void setcancel_pair()
{
    pthread_setcancel(CANCEL_OFF);
    pthread_setcancel(CANCEL_ON);
}


static void setasynccancel_pair()
{
    pthread_setasynccancel(CANCEL_OFF);
    pthread_setasynccancel(CANCEL_ON);
}


static void testcancel()
{
    pthread_testcancel();
}


static void self()
{
    pthread_self();
}


static void *dummy(void *arg)
{
	pthread_t	self;

	self = pthread_self();
	pthread_detach(&self);
	pthread_exit(NULL);
}

static void thread_create()
{
	pthread_t	thread;

	pthread_create(&thread, pthread_attr_default, dummy, NULL);
}

static void create_yield()
{
	pthread_t	thread;

	pthread_create(&thread, pthread_attr_default, dummy, NULL);
	pthread_yield();
}

struct test_t tests[] = {
    RPCTEST(xq_alloc_free),         /* 0 */
    RPCTEST(rpc_lock_unlock),       /* 1 */
    TEST(nothing),                  /* 2 */
    TEST(once),                     /* 3 */
    RPCTEST(xq_alloc_free_pre_opt), /* 4 */
    APOLLOTEST(cps_mutex),          /* 5 */
    TEST(cond_signal),              /* 6 */
    TEST(data_move),                /* 7 */
    TEST(lock_unlock),              /* 8 */
    TEST(setcancel_pair),           /* 9 */
    TEST(setasynccancel_pair),      /* 10 */
    TEST(testcancel),               /* 11 */
    TEST(self),                     /* 12 */
    TEST(thread_create),	    /* 13 */
    TEST(create_yield),		    /* 14 */
};

void c_time(timer)
struct timeval *timer;
{
    struct timezone tz;

#ifdef SNI_SVR4
    gettimeofday(timer);
#else
    gettimeofday(timer, &tz);
#endif
}

unsigned long e_time(timer)
struct timeval *timer;
{  
    struct timeval t;
    struct timezone tz;

#ifdef SNI_SVR4
    gettimeofday(timer);
#else
    gettimeofday(&t, &tz);
#endif
    if (t.tv_usec < timer->tv_usec)
    {
        t.tv_sec--;
        t.tv_usec += 1000000;
    }
    t.tv_usec -= timer->tv_usec;
    t.tv_sec -= timer->tv_sec;
                       
    return ((t.tv_sec * 1000000) + t.tv_usec);
}

