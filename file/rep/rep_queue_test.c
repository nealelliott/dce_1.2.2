/*
 * Copyright (C) 1996 Transarc Corporation -- All Rights Reserved
 *
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/rep/RCS/rep_queue_test.c,v 1.3 1996/03/22 14:01:36 cfe Exp $
 */
#include <stdio.h>
#include <pthread.h>

#include <dcedfs/osi.h>

#include "rep_queue.h"

static rep_queue_cookie_t	cookie1;
static rep_queue_cookie_t	cookie2;
static rep_queue_t		q;

static int MyThread(pthread_addr_t);

int
main(int argc, char* argv[])
{
    time_t	d;
    pthread_t  	t;
    extern int	pthread_patch_ctw;

    repq_Init(&q);

    d = osi_Time() + (rand() & 0xff);
    fprintf(stderr, "Entering item with deadline: %ld\n", d);
    cookie1 = repq_Enter(&q, "hello", d);

/*
    d = osi_Time() + (rand() & 0xff);
    fprintf(stderr, "Entering item with deadline: %ld\n\n", d);
    cookie2 = repq_Enter(&q, "world", d);
*/

    pthread_create(&t, pthread_attr_default, (pthread_startroutine_t)MyThread,
		   (pthread_addr_t)1);
/*    pthread_create(&t, pthread_attr_default, MyThread, 2); */

    for (;;)
	pthread_yield();
}

int
MyThread(pthread_addr_t arg)
{
    rep_queue_cookie_t	c;
    time_t		d;
    int			i = (int)arg;
    char*		s;

    while (1) {
	fprintf(stderr, "%d: Getting an item at %ld\n", i, osi_Time());
	s = repq_Get(&q, &c);
	fprintf(stderr, "%d: Got one at %ld: %s\n", i, osi_Time(), s);

	pthread_yield();

	d = osi_Time() + (rand() & 0xff);
	fprintf(stderr, "%d: Enqueuing with new deadline: %ld\n", i, d);
	repq_Put(&q, c, d);
    }
}
