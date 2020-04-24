/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epistat.c,v $
 * Revision 1.1.64.1  1996/10/02  17:27:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:11  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1995, 1991 Transarc Corporation
 * All rights reserved.
 */

/*			An experimental interface for
			monitoring and tuning Episode

 */

#include <dcedfs/param.h>
#include <dcedfs/syscall.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/episode/efs_opcode.h>
#include <dcedfs/episode/instrument.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/sautils/epistat.c,v 1.1.64.1 1996/10/02 17:27:50 damon Exp $")

static int DoStats (long op, long proc, long var, long value,
		    long *resultP, long *instanceP)
{
    struct ins_varDesc vars;
    struct ins_opDesc opcode;
    struct ins_basicObject resultValues;
    struct ins_basicObjectId argObjIds, resultObjIds;
    struct ins_sysOp sysop;

    bzero((char *)&resultValues,sizeof(resultValues));
    bzero((char *)&argObjIds,sizeof(argObjIds));
    bzero((char *)&resultObjIds,sizeof(resultObjIds));
    opcode.opcode = op;
    opcode.count = 1;
    vars.procIdx = proc;
    vars.varIdx = var;

    if (opcode.opcode == INS_SET) {
	resultValues.length = sizeof(long);
	bcopy((char *)&value,resultValues.value,sizeof(long));
	resultValues.type = INS_DT_LONG;
    } else /* get or getnext */ {
	argObjIds.objId.type = INS_DT_LONG;
	argObjIds.objId.length = sizeof(long);
	bcopy((char *)&value,argObjIds.objId.value,sizeof(long));
    }
    bcopy((char *)&opcode,(char *)&(sysop.op),sizeof(struct ins_opDesc));
    bcopy((char *)&vars,(char *)&(sysop.var),sizeof(struct ins_varDesc));

    if (afs_syscall(AFSCALL_EPISODE, EPIOP_STATS, (int) &sysop,\
	(int) &argObjIds, (int) &resultObjIds, (int) &resultValues)) {
	if (errno == EPERM) {
	    perror("epistat");
	    exit (1);
	}
	return errno;
    }
    if (resultP)
	bcopy(resultValues.value, (char *)resultP, sizeof(long));
    if (instanceP)
	bcopy(resultObjIds.objId.value, (char *)instanceP, sizeof(long));
    return 0;
}

#define PROC_BUFFER 9
#define BUFALLOCS 7
#define BUFMAXTRANS 14

static int DumpAllStats (void)
{
    int proc, var;
    int logBsize;			/* ln2 of buffer pool block size */
    long result;
    int code;

    for (proc=8; proc<17; proc++) {
	if (proc == PROC_BUFFER)
	    continue;			/* do these later */
	for (var=0; var<20; var++) {
	    code = DoStats (INS_GET, proc, var, 0, &result, NULL);
	    if (!code) {
		printf("%d %d = %u\n", proc, var, result);
	    }
	}
    }

    proc = PROC_BUFFER;			/* do per buffer pools stats */
    /* buffer.maxtrans isn't per pool, should be in ins_tran. */
    var = BUFMAXTRANS;
    code = DoStats (INS_GET, proc, var, 0, &result, NULL);
    if (!code) {
	printf("%d %d = %u\n", proc, var, result);
    }
    
    for (logBsize=0; logBsize<=6; logBsize++) {
	long bsize = 1<<logBsize;	/* in Kbytes */

	var = BUFALLOCS;
	code = DoStats (INS_GET, proc, var, bsize, &result, NULL);
	if (code) continue;
	if (result == 0) {
	    /* This buffer pool size was never used, so skip it. */
	    continue;
	}
	printf("%d %d [%d] = %u\n", proc, var, bsize, result);
	for (var=1; var<20; var++) {
	    if ((var == BUFALLOCS) || (var == BUFMAXTRANS))
		continue;		/* did this above */
	    code = DoStats (INS_GET, proc, var, bsize, &result, NULL);
	    if (!code) {
		printf("%d %d [%d] = %u\n", proc, var, bsize, result);
	    }
	}
    }
}

main(int argc, char *argv[])
{
    long opcode, proc, var;
    long value;
    long myresult, myinstance;
    int code;

    osi_setlocale(LC_ALL, "");

    if (strcmp (argv[1], "-all") == 0) {
	code = DumpAllStats();
	exit (code != 0);
    }

    if (argc < 4) {
usage:
	fprintf(stderr, "usage: epistat {get|%d|set|%d|getnext|%d}",
		INS_GET, INS_SET, INS_GETNEXT);
	fprintf(stderr, "<procIdx> <varIdx> [<value/instance>]\n");
	fprintf(stderr, "  or   epistat -all\n");
	exit(1);
    }

    if (strcmp (argv[1], "get") == 0)
	opcode = INS_GET;
    else if (strcmp (argv[1], "set") == 0)
	opcode = INS_SET;
    else if (strcmp (argv[1], "getnext") == 0)
	opcode = INS_GETNEXT;
    else {
	opcode = atoi(argv[1]);
	if ((opcode != INS_GET) && (opcode != INS_SET) &&
	    (opcode != INS_GETNEXT)) {
	    fprintf (stderr, "unknown opcode: '%s'\n", argv[1]);
	    goto usage;
	}
    }

    proc = atol(argv[2]);
    var = atol(argv[3]);

    if (argc > 4) {
	value = atol(argv[4]);   
    } else
	value = 0;

    code = DoStats (opcode, proc, var, value, &myresult, &myinstance);
    if (code) {
	perror("calling epiop_stats");
	exit(1);
    }
    printf("result %u nextInstance %u \n",myresult,myinstance);
    exit(0);
}
