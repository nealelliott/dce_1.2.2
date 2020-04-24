/* 
 * Copyright (C) 1996, 1993  Transarc Corporation - All rights reserved 
 */
/*
 * aux.h
 * Some common definitions to help micro tests.
 */

#include <sys/time.h>
#define TDNAME	"tdir"
#define TDMODE	0777
#define TFNAME	"file"
#define TFMODE	0666

extern struct timeval tv_start, tv_end;
extern void timer_value(/*struct timeval * */);
extern int make_testdir(/*int files, char *buf, int bsize */);
extern int remove_testdir(/*int files, int ignore_errors */);
extern int aux_usage_start();
extern int aux_usage_end();
extern void aux_net_usage(/*double *utp, double *stp*/);

/*
 * Start timing an event
 */
#if	defined(_SVID_GETTOD)
#define timer_start()	gettimeofday(&tv_start)
#else /* _SVID_GETTOD */
#define timer_start()	gettimeofday(&tv_start, (struct timezone *)0)
#endif /* _SVID_GETTOD */
/*
 * Stop timing an event
 */
#if	defined(_SVID_GETTOD)
#define timer_stop()	gettimeofday(&tv_end)
#else /* _SVID_GETTOD */
#define timer_stop()	gettimeofday(&tv_end, (struct timezone *)0)
#endif /* _SVID_GETTOD */

#if	defined(__hpux)
#define aux_usage_start() 0
#define aux_usage_end() 0
#define aux_net_usage(utp, systp) *(utp) = 0; *(systp) = 0;
#endif	/* hpux */
