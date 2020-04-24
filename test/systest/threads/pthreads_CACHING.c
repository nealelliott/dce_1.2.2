/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pthreads_CACHING.c,v $
 * Revision 1.1.8.2  1996/03/09  20:52:35  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:09  marty]
 *
 * Revision 1.1.8.1  1995/12/13  21:56:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:37  root]
 * 
 * Revision 1.1.8.2  1993/07/09  15:04:45  root
 * 	Initial King Kong branch
 * 	[1993/07/09  15:04:16  root]
 * 
 * Revision 1.1.6.3  1993/04/23  13:53:08  eheller
 * 	Cleaned up warnings
 * 	[1993/04/23  13:29:29  eheller]
 * 
 * Revision 1.1.6.2  1993/03/31  13:00:44  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  20:03:26  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:09:59  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:17:44  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:35:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *
 * Filename:  pthreads_CACHING.c
 *
 * Purpose:   To measure pthread_create() performance with and without caching.
 *
 * Author:    Frank Ginac
 *
 * Interface: pthreads_CACHING <number of threads> <yield count>
 *
 * Function:  The primary function of this program is to provide rough
 *            estimates of pthread_create() performance.  A secondary function
 *	      of this program is to exercise the threads scheduler.  To use
 *	      as a threads scheduler exerciser supply a large number
 *            such as 1000 for the first argument (number of threads to create)
 * 	      and a large number such as 1000 for the second argument (number
 *	      of times each thread should yield the processor).  In this
 *	      scenerio (1000x1000) each thread will yield and consequently
 * 	      force itself to be rescheduled 1000 times for a total yield
 *	      count of 1000000.
 *
 * Example:
 *
 * ./pthreads_CACHING 1000 10
 *
 * thd_cnt = 1000
 * yld_cnt = 10
 *
 * main: total time to create 1000 threads was 1.137575 seconds.
 * main: min seconds to create a thread: 0.000965
 * main: max seconds to create a thread: 0.034160
 * main: avg seconds to create a thread: 0.001138
 * main: avg active threads:             10.934000
 *
 * HISTORY:
 *	12/20/91	Added proper exit() statements.		Gerald Cantor
 */

#include <sys/time.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>

#define MAX_THREAD_COUNT	100000
#define MAX_SECONDS		999999
#define PERF			1

int			g_thread_active_count = 0;
int			g_thread_exit_count   = 0;
pthread_mutex_t        *g_mutex;

/**********
  thread()
 **********/
void *thread(arg)

	int *arg;

{

	int i;
	int err;

        /*
         * Lock the mutex used to prevent multiple threads from simultaneously
	 * incrementing the active thread count.
         */
        err = pthread_mutex_lock(g_mutex);
        if (err == -1)
        {
             switch (errno)
             {
                  case EINVAL: printf("thread: EINVAL from mutex_lock.\n");
                               break;

                  case EDEADLK: printf("thread: EDEADLK from mutex_lock.\n");
                                break;

                  default: perror("thread, unexpected err from mutex_lock");
                           break;
             }
        }

	g_thread_active_count++;

        /*
         * Unlock the mutex.
         */
        err = pthread_mutex_unlock(g_mutex);
        if (err == -1)
        {
             switch (errno)
             {
                  case EINVAL: printf("thread: EINVAL from mutex_unlock.\n");
                               break;

                  default: perror("thread, unexpected err from mutex_unlock");
                           break;
             }
        }

	/*
	 * Yield to push up active count. 
	 */
	for (i=0; i<=(*arg); i++) 
	{
		pthread_yield();
	}

        /*
         * Lock the mutex used to prevent multiple threads from simultaneously
         * incrementing the active thread count and the thread exit count.
         */
        err = pthread_mutex_lock(g_mutex);
        if (err == -1)
        {
             switch (errno)
             {
                  case EINVAL: printf("thread: EINVAL from mutex_lock.\n");
                               break;

                  case EDEADLK: printf("thread: EDEADLK from mutex_lock.\n");
                                break;

                  default: perror("thread, unexpected err from mutex_lock");
                           break;
             }
        }

	g_thread_active_count--;
	g_thread_exit_count++;

        /*
         * Unlock the mutex.
         */
        err = pthread_mutex_unlock(g_mutex);
        if (err == -1)
        {
             switch (errno)
             {
                  case EINVAL: printf("thread: EINVAL from mutex_unlock.\n");
                               break;

                  default: perror("thread, unexpected err from mutex_unlock");
                           break;
             }
        }

	/*
	 * Exit.
	 */	
	pthread_exit(NULL);
}

/*********
   main()
 *********/
main(argc, argv)

	int	argc;
	char 	*argv[];

{

	pthread_t	 	*th[MAX_THREAD_COUNT];
	int			i;
	int 			err;
	int 			thd_cnt;
	int 			yld_cnt;

#ifdef PERF
	int			l_thread_active_count		= 0;
	float                   seconds				= 0.0;
	float			min_seconds			= MAX_SECONDS;
	float			max_seconds			= 0.0;
	float			avg_seconds			= 0.0;
	float			totsecs				= 0.0;
	float			totactive			= 0.0;
	float			avg_active 			= 0.0;
	struct timeval          timeptr1;
	struct timeval		timeptr2;
#endif

	thd_cnt = atoi(argv[1]);
	yld_cnt = atoi(argv[2]);

	printf("\n\nthd_cnt = %d\nyld_cnt = %d\n\n", thd_cnt, yld_cnt);

        /*
         * Create mutex.
         */
        g_mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
        err = pthread_mutex_init(g_mutex, pthread_mutexattr_default);
        if (err == -1)
        {
                switch(errno)
                {
                        case EAGAIN: printf("main: EAGAIN from mutex_init.\n");
                                     exit(1);

                        case ENOMEM: printf("main: ENOMEM from mutex_init.\n");
                                     exit(1);

                        case EBUSY: printf("main: EBUSY from mutex_init.\n");
                                    exit(1);

                        case EINVAL: printf("main: EINVAL from mutex_init.\n");
                                     exit(1);

                        default: perror("main, unexpected err from mutex_init.");
                                 exit(1);
                }
        }

	/*
	 * Create argv[1] threads.
	 */
	for (i=1; i<=thd_cnt; i++)
	{
		th[i] = (pthread_t *) malloc(sizeof(pthread_t));

#ifdef VERBOSE
		printf("main: creating thread, iter = %d\n", i);
#endif

                err = gettimeofday(&timeptr1, 0);
                if (err != 0)
                {
                        perror("main, from gettimeofday");
                }

		err = pthread_create(th[i],pthread_attr_default,
				     (pthread_startroutine_t)thread,&yld_cnt);
		if (err == -1)
		{
			switch (errno)
			{
			   case EAGAIN: printf("main: EAGAIN from create.\n");
			   		exit(1);

			   case ENOMEM: printf("main: ENOMEM from create.\n");
					exit(1);

			   default: perror("main, unexpected err from create");
				    exit(1);
			}
		}

                err = gettimeofday(&timeptr2, 0);
                if (err != 0)
                {
                        perror("main, from gettimeofday");
                }


#ifdef PERF
		l_thread_active_count = g_thread_active_count;
#endif

                /*
                 * Free memory used by thread when thread exits.
                 */
                err = pthread_detach(th[i]);
                if (err == -1)
                {
                        switch (errno)
                        {
                                case EINVAL: printf("main: EINVAL from detach.\n");
                                             break;

                                case ENOMEM: printf("main: ESRCH from detach.\n");
                                             break;

                                default: perror("main, unexpected err from detach");
                                         break;
                        }
                }

#ifdef VERBOSE
		printf("main: created thread with tid = %d%d\n", *th[i]);
#endif

		pthread_yield();

#ifdef PERF
                if (timeptr1.tv_sec != timeptr2.tv_sec)
                {
                        seconds = (1e6 - timeptr1.tv_usec + timeptr2.tv_usec)*1e-6;
                }
                else
                {
                        seconds = (timeptr2.tv_usec - timeptr1.tv_usec)*1e-06;
                }

		totsecs += seconds;

		if (seconds > max_seconds)
		{
			max_seconds = seconds;
		}

		if (seconds < min_seconds)
		{
			min_seconds = seconds;
		}

		totactive += l_thread_active_count;
#endif

	}

#ifdef PERF
	printf("main: total time to create %d threads was %f seconds.\n", \
	       thd_cnt, \
	       totsecs);
	printf("main: min seconds to create a thread: %f\n", min_seconds);
	printf("main: max seconds to create a thread: %f\n", max_seconds);

	avg_seconds = totsecs/thd_cnt;
	printf("main: avg seconds to create a thread: %f\n", avg_seconds);

	avg_active = totactive/thd_cnt;
	printf("main: avg active threads:             %f\n", avg_active);
#endif

	/*
	 * Wait for all threads to exit. 
	 */
	while (g_thread_exit_count < thd_cnt)
	{
		pthread_yield();
	}
	exit(0);
}
