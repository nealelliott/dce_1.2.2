/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prrb_thd_001.c,v $
 * Revision 1.1.6.2  1996/02/17  23:31:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:08  marty]
 *
 * Revision 1.1.6.1  1995/12/11  23:03:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:24  root]
 * 
 * Revision 1.1.4.2  1992/12/30  16:53:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:01  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  13:48:40  bolinger
 * 	Test case corresponding to OT defect 4149 -- whale on cma_thread_detach(),
 * 	with some cma__terminate_thread() obbligato.
 * 	[1992/06/29  20:16:26  bolinger]
 * 
 * $EndLog$
 */
/*
 * Exercise thread terminate/detach code by starting n threads,
 * each of which continually creates and detaches threads that
 * immediately detach themselves, then terminate (or return).
 *
 * Test will run for ITERS iterations -- to be useful, really should be
 * built with debug version of CMA.
 */
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

#define MAXPARENTS	256
#define DEFPARENTS	16
#define MAXSIMCHILD	16
#define DEFSIMCHILD	1
#define ITERS	5000

int G_simchild = 1;

child(int method)
{
	pthread_t self;

	self = pthread_self();
	pthread_detach(&self);
	if (method)
		pthread_exit(1);
	else
		return (0);
}

pthread_mutex_t parent_mutex;
pthread_cond_t parent_cond;
int parents_done = 0;

parent(int number)
{
	int cnt, ccnt;
	pthread_t ct[MAXSIMCHILD];

	for (cnt = 0; cnt < ITERS; ++cnt) {
		for (ccnt = 0; ccnt < G_simchild; ++ccnt) {
			if (pthread_create(&ct[ccnt],
				pthread_attr_default, child, number & 1) == -1) {
				cts_failed("parent %d failed on %dth ", number, cnt);
				perror("child create");
				return (1);
			}
		}
		for (ccnt = 0; ccnt < G_simchild; ++ccnt) {
			if (cnt & 1)
				pthread_yield();
			if ((cnt & 7) == 3 || (cnt & 7) == 6)
				pthread_join(ct[ccnt], (void **)0);
			if (!(cnt & 1))
				pthread_detach(&ct[ccnt]);
		}
	}
	pthread_mutex_lock(&parent_mutex);
	++parents_done;
	cts_comment("parent %d finished", number);
	pthread_cond_signal(&parent_cond);
	pthread_mutex_unlock(&parent_mutex);
	return (0);
}

main(int ac, char **av)
{
	int status, p;
	pthread_t pt;
	int parents;

    cts_test("PRRB_THD_001", "Test multiple detach interaction");
	pthread_mutex_init(&parent_mutex, pthread_mutexattr_default);
	pthread_cond_init(&parent_cond, pthread_condattr_default);
	parents = DEFPARENTS;
	if (1 < ac)
		parents = atoi(av[1]);
	if (MAXPARENTS < parents)
		parents = MAXPARENTS;
	if (2 < ac)
		G_simchild = atoi(av[1]);
	if (MAXSIMCHILD < G_simchild)
		G_simchild = MAXSIMCHILD;
	for (p = 1; p <= parents; ++p) {
		if (pthread_create(&pt, pthread_attr_default, parent, p) == -1) {
			perror("can't create");
			exit(1);
		}
		cts_comment("parent %d created", p);
	}
	pthread_mutex_lock(&parent_mutex);
	while (parents_done < parents) {
		pthread_cond_wait(&parent_cond, &parent_mutex);
	}
	pthread_mutex_unlock(&parent_mutex);
    cts_result();
	exit(0);
}
