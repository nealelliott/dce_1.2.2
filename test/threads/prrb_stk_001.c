/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prrb_stk_001.c,v $
 * Revision 1.1.100.2  1996/02/17  23:31:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:07  marty]
 *
 * Revision 1.1.100.1  1995/12/11  23:02:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:23  root]
 * 
 * Revision 1.1.98.1  1993/10/12  19:37:27  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:57  rhw]
 * 
 * Revision 1.1.8.2  1993/06/14  20:18:00  sommerfeld
 * 	Workaround HPUX compiler bug with
 * 	passing attr's from an array.
 * 	[1993/06/04  20:18:27  hopkins]
 * 
 * Revision 1.1.4.2  1992/12/30  16:53:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:57  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  16:57:07  bolinger
 * 	Fix OT defect 2922:  add testcase from defect 2100, which tests
 * 	stack allocation by creating many threads with a variety of stack
 * 	sizes.
 * 	[1992/05/15  13:57:55  bolinger]
 * 
 * $EndLog$
 */
/*
 * prrb_stk_001.c: Stress-test thread stack creation by creating
 * REPEAT_COUNT threads (more-or-less serially), with the stack
 * sizes specified in stackSizeList.
 */
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <dce/cts_test.h>

#define STACK_SIZE	 40*1024
#define REPEAT_COUNT    600

#define NATTRS	    (8)
pthread_attr_t attrList[NATTRS];

unsigned long stackSizeList[] = {
	   40*1024,
	   40*1024,
	   40*1024,
	   1024*1024,
	(1024*1024+512),
	(1024*1024+16),
	   16*100*1024,
	   40*1024,
	   40*1024
};

/*
unsigned long stackSizeList[] = {
  40960,
  40960,
  40960,
  40960,
  40960,
  40960,
  40960,
  40960,
  40960
};
*/

#define STACK_LIST_SIZE (sizeof(stackSizeList)/sizeof(long))

#define TRUE    1
#define FALSE   0

pthread_attr_t myAttr;
int createdFlag;

#define CHECK_ERROR(call, msg) \
	   if ((call) == -1) {\
	       cts_failed("pthread_%s: Error Code (%d)", msg, errno);\
	       exit (1);\
	   }

void DoNothing(i)
{
    i = i * i;
}

void Delay(count)
int count;
{
	int i;

	for (i=0; i < count; i++)
	    DoNothing(i);
}

#define LINEMAX 79

int G_quietf = 0;
int G_verbosef = 0;
int G_debugf = 0;

void output(smsg, lmsg, arg1, arg2)
	char *smsg, *lmsg;
	int arg1, arg2;
{
	int len;
	char buf[LINEMAX+1];
	static int col = 0;

	if (G_quietf)
		;
	else if (G_verbosef)
		printf(lmsg, arg1, arg2);
	else {
		sprintf(buf, smsg, arg1, arg2);
		len = strlen(buf);
		if (LINEMAX <= col + len) {
			printf("\n");
			col = 0;
		}
		printf("%s", buf);
		col += len;
	}
}

void SetFlag(dummy)
	int dummy;
{
	pthread_t threadId;

	threadId = pthread_self();
	createdFlag = TRUE;
	Delay(10000);
	CHECK_ERROR(pthread_detach(&threadId), "detach");
	if (G_debugf)
		output("SP %x ", "\tgot stack %x\n", &threadId, 0);
	output("D ", "\t-- thread died\n", 0, 0);
	pthread_exit(0);
}

ThreadCreate(attr, stackSize)
pthread_attr_t attr;
int stackSize;
{
    pthread_t id;

    CHECK_ERROR(pthread_attr_setprio(&attr,
		(PRI_OTHER_MIN+PRI_OTHER_MAX+1)/2), "attr_setprio");

    CHECK_ERROR(pthread_attr_setstacksize(&attr, stackSize),
		"attr_setstacksize");

    if (pthread_attr_setstacksize(&attr, stackSize) == -1) {
		cts_failed("pthread_attr_setstacksize: error code %d, size %d",
			errno, stackSize);
		exit(0);
    }
    createdFlag = FALSE;
    CHECK_ERROR(pthread_create(&id,
		attr,
		(pthread_startroutine_t) SetFlag,
		0), "create");
    while (createdFlag == FALSE)
		pthread_yield();
    return;
}

main(argc,argv)
    int argc;
    char **argv;
{
    int i;
#ifdef __hpux
    /* work around compiler bug, see below */
    pthread_attr_t a;
#endif

    cts_test("PRRB_STK_001", "Test repeated large/small thread stack creation");
	if (argc <= 1)
		;
	else if (!strcmp(argv[1], "-q"))
		G_quietf = 1;
	else if (!strcmp(argv[1], "-d"))
		G_debugf = 1;
	else if (!strcmp(argv[1], "-v"))
		G_verbosef = 1;
	if (!G_quietf)
		cts_comment("Start of test...");
    for (i = 0; i < NATTRS; i++) {
		CHECK_ERROR(pthread_attr_create(&attrList[i]), "attr_create");
		CHECK_ERROR(pthread_attr_setinheritsched(&attrList[i],
	                 PTHREAD_DEFAULT_SCHED),
	                 "attr_setinheritsched");
		CHECK_ERROR(pthread_attr_setsched(&attrList[i], SCHED_OTHER),
	                 "attr_setsched");
		CHECK_ERROR(pthread_attr_setprio(&attrList[i], PRI_OTHER_MIN),
	                 "attr_setprio");
    }
    for (i = 0; i < REPEAT_COUNT; i++) {
			output("Th %3d S %7d ", "Starting thread %d with stack size %d\n",
				i, stackSizeList[i%STACK_LIST_SIZE]);
#ifdef __hpux
		/*
		 * work around compiler bug ... compiler doesn't seem to pass
		 * attrList[x] properly: if attrList[x] is passed then ThreadCreate
		 * gets "invalid argument" errors when it tries to use the attr.
		 * But if we stuff the attr into another var and pass it, it
		 * works just fine.
		 */
		a = attrList[i%NATTRS];
		ThreadCreate(a, stackSizeList[i%STACK_LIST_SIZE]);
#else
		ThreadCreate(attrList[i%NATTRS], stackSizeList[i%STACK_LIST_SIZE]);
#endif
    }
	if (!G_quietf) {
		printf("\n");
		cts_comment("End of test...");
	}
	cts_result();
    exit(0);
}
