/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1994, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/icl.h>

#include <ctype.h>
#include <string.h>

#include <com_err.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>
#include <dcedfs/episode/critical.h>
#include <anode.h>

#include <runtest.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/runtest.c,v 4.55 1995/03/10 21:25:41 andi Exp $")

#if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV)
#define sscanf_r(c,a) (cma_lock_global(), (c=sscanf a), cma_unlock_global(), c)
#define fscanf_r(c,a) (cma_lock_global(), (c=fscanf a), cma_unlock_global(), c)
#else
#define sscanf_r(c,a) (c=sscanf a)
#define fscanf_r(c,a) (c=fscanf a)
#endif

extern int afsdb_osi;
extern int afsdb_episode_recover;
extern int afsdb_episode_anode;
extern int afsdb_episode_anode_test;
extern int afsdb_episode_anode_verify;
/* EXPORT */
int afsdb_episode_runtest = 0;
#define TestPrintLines() (afsdb_episode_runtest & 1)	/* input lines */

extern char *ucstring ();
extern char *lcstring ();
#define WHITESPACE " \t"

static char *whoami;

static int processesCreated = 0;	/* this is the notify addr for both */
static int processesDeleted = 0;
static pthread_t *initialProcess = NULL;

static int testVariation;		/* which variation we're running */

static long logSize = 0;		/* explicit log size */

static long ConvertErrorCodeName (name)
  IN char *name;
{
    static struct {
	char *name;
	long code;
    } codes[] = {
	{"AGGREGATEFULL", EPI_E_AGGREGATEFULL},
	{"BADAGGREGATE", EPI_E_BADAGGREGATE},
	{"BADANODE", EPI_E_BADANODE},
	{"BADANODEINDEX", EPI_E_BADANODEINDEX},
	{"BADARGS", EPI_E_BADARGS},
	{"BADDEVICE", EPI_E_BADDEVICE},
	{"BADAUXSOURCE", EPI_E_BADAUXSOURCE},
	{"BADFLAGS", EPI_E_BADFLAGS},
	{"BADINDEX", EPI_E_BADINDEX},
	{"BADUNIQUE", EPI_E_BADUNIQUE},
	{"BADVOLID", EPI_E_BADVOLID},
	{"BADVOLUME", EPI_E_BADVOLUME},
	{"BADFILE", EPI_E_BADFILE},
	{"ENDOFAGGREGATE", EPI_E_ENDOFAGGREGATE},
	{"ENDOFVOLUME", EPI_E_ENDOFVOLUME},
	{"EOF", EPI_E_EOF},
	{"BLOCKTOOBIG", EPI_E_BLOCKTOOBIG},
	{"VOLIDEXISTS", EPI_E_VOLIDEXISTS},
	{"INDEXEXISTS", EPI_E_INDEXEXISTS},
	{"NOTRANS", EPI_E_NOTRANS},
	{"NOBLOCK", EPI_E_NOBLOCK},
	{"NOENT", EPI_E_NOENT},
	{"NOTAFILE", EPI_E_NOTAFILE},
	{"NOTAVOLUME", EPI_E_NOTAVOLUME},
	{"NOTABITMAP", EPI_E_NOTABITMAP},
	{"NOTANAGGREGATE", EPI_E_NOTANAGGREGATE},
	{"NOTBLOCKEDDATA", EPI_E_NOTBLOCKEDDATA},
	{"OPEN", EPI_E_OPEN},
	{"NOTEMPTY", EPI_E_NOTEMPTY},
	{"OUTSTANDINGCOPIES", EPI_E_OUTSTANDINGCOPIES},
	{"READSHORT", EPI_E_READSHORT},
	{"STATUSSIZE", EPI_E_STATUSSIZE},
	{"TOOBIG", EPI_E_TOOBIG},
	{"TOOMANYOPENANODES", EPI_E_TOOMANYOPENANODES},
	{"VOLUMEFULL", EPI_E_VOLUMEFULL},
	{"COPYONWRITE_ILLEGAL", EPI_E_COPYONWRITE_ILLEGAL},
	{"BADLOGBLOCKS", EPI_E_BADLOGBLOCKS},
	{"BADBITMAPBLOCKS", EPI_E_BADBITMAPBLOCKS},
	{"BADBACKING", EPI_E_BADBACKING},
	{"NYI_MULTIPLEQUOTAPROCS", EPI_E_NYI_MULTIPLEQUOTAPROCS},
	{"REINIT", EPI_E_REINIT},
	{"NOINIT", EPI_E_NOINIT},
	{"BLOCKEXISTS", EPI_E_BLOCKEXISTS},
	{"BADBLOCKLOCATION", EPI_E_BADBLOCKLOCATION},
	{"INSERTERROR", EPI_E_INSERTERROR},
	{"BADROCK", EPI_E_BADROCK},
	{"VOLOFFLINE", EPI_E_VOLOFFLINE},
	{"AGGOFFLINE", EPI_E_AGGOFFLINE},
	{"SPECIALTOOBIG", EPI_E_SPECIALTOOBIG},
	{"BADDISKVOLUME", EPI_E_BADDISKVOLUME},
	{"NYI", EPI_E_NYI},
	{"DIRONLYAUX", EPI_E_DIRONLYAUX},
	{"HASLINKS", EPI_E_HASLINKS},
    };
    int i;
    int namelen;

    ucstring (name, name, 32);
    if (strncmp (name, "EPI_E_", 6) == 0) name += 6;
    namelen = strlen(name);
    if (name[namelen-1] == ')') name[--namelen] = 0;
    for (i=0; i<sizeof(codes)/sizeof(codes[0]); i++)
	if (strcmp (name, codes[i].name) == 0) return codes[i].code;
    return 0;
}

static long ParseLine (o, olen, llen, wlen, op, line, whereami)
  INOUT struct epir_operationData *o;
  IN int olen;				/* length of opcode buffer */
  IN int llen;				/* length of line buffer */
  IN int wlen;				/* length of whereami string */
  OUT char *op;				/* opcode */
  OUT char *line;			/* rest of line */
  OUT char *whereami;			/* location string for com_err */
{
    int linelen;
    int loclen;
    int oplen;
    char *comment;			/* start of comment */
    int i, j;
    int count;
    long errorCode;
    char errorCodeName[32];		/* name of code */
    char printbuf[256];			/* to save for printing later */

    afsl_Assert(op && line && whereami);
    afsl_Assert(olen >= 32);

    do {
	char loc[10+EPIR_MAX_TESTNAME];
	int variation = 0;		/* this line's variation number */
	
	o->errorCode = 0;
	op[0] = 0;
	line[0] = 0;

	if (!fgets (line, llen, o->testStream)) {
	    if (feof(o->testStream)) return -1;
	    com_err (whoami, errno,
		     "encountered after line %d", o->lineNumber);
	    return -1;
	}
	o->lineNumber++;

	linelen = strlen (line);
	if ((linelen == 0) || (linelen >= llen) ||
	    (line[linelen-1] != '\n')) {
	    com_err (whoami, 0, "line %d too long ('%s')", o->lineNumber, line);
	    return -1;
	}
	line[--linelen] = 0;

	while ((linelen > 0) && isspace(line[linelen-1]))
	    line[--linelen] = 0;

	loc[sizeof(loc)-1] = 0;
	if (!pthread_equal(o->pid, *initialProcess))
	    sprintf (loc, "[%s.%d %3d]",
		     o->testName, o->procId, o->lineNumber);
	else sprintf (loc, "[%s %3d]", o->testName, o->lineNumber);
	afsl_Assert(loc[sizeof (loc) - 1] == 0);
	loclen = strlen (loc);

	if (TestPrintLines()) {
	    if (linelen+5 >= sizeof(printbuf)) {
		char *cp;
		strncpy (printbuf, line, sizeof(printbuf)-5);
		cp = printbuf + sizeof(printbuf)-5;
		*cp++ = '.'; *cp++ = '.'; *cp++ = '.';
		*cp++ = 0;
	    } else strcpy (printbuf, line);
	}

	/* flush comments */
	if (comment = strchr(line, '#')) *comment = 0;

	j=0;				/* opcode part of line */

	/* parse error code & variation syntax */
	while (line[strspn(line, WHITESPACE)] == '(') {
	    if (sscanf_r (count, (line, " ( code = %31s )", errorCodeName))) {
		errorCode = ConvertErrorCodeName (errorCodeName);
		if (errorCode == 0) {
		    fprintf (stderr, "Unknown error code name %s @ %d\n",
			     errorCodeName, o->lineNumber);
		    return -1;
		}
		o->errorCode = errorCode;
	    } else if (sscanf_r (count, (line, " ( var = %d )", &variation))) {
		; /* execute only this variation */
	    } else {
		fprintf (stderr,
			 "Unexpected special syntax @ %d\n", o->lineNumber);
		return -1;
	    }

	    /* squeeze out special syntax, by BLTing the line down */
	    i = 0;
	    while (line[i] != ')') i++;
	    for (i++, j=0; ; j++, i++)
		if ((line[j] = line[i]) == 0) break;
	}

	if (TestPrintLines()) {
	    if ((variation != 0) && (variation != testVariation))
		printf ("%s: #%d# %s\n", loc, variation, printbuf);
	    else printf ("%s:\t%s\n", loc, printbuf);
	}

	if ((variation != 0) && (variation != testVariation))
	    continue;			/* line doesn't match this variation */

	op[olen - 1] = 0;
	sscanf_r (count, (line, "%31s", op));
	afsl_Assert(op[olen - 1] == 0);
	if (count != 1) continue;	/* line is blank */
	oplen = strlen(op);

	/* now skip opcode and leading white space and blt line down */
	i = strspn(line, WHITESPACE);
	i += oplen;
	i += strspn(line + i, WHITESPACE);
	for (j = 0; ; j++, i++)
	    if ((line[j] = line[i]) == 0) break;

	whereami[wlen-1] = 0;
	if ((u_int) wlen < strlen(whoami) + oplen + loclen + 3) {
	    com_err (whoami, E2BIG, "whereami string too short");
	    return -1;
	}
	sprintf (whereami, "%s %s %s", whoami, op, loc);
	afsl_Assert(whereami[wlen-1] == 0);

	/* to simplify parsing in test_anode, flush trailing blanks caused by
         * comments, etc. */

	linelen = strlen(line);
	while ((linelen > 0) && isspace(line[linelen-1]))
	    line[--linelen] = 0;

	lcstring (op, op, oplen+1);
    } while (op[0] == 0);
    return 0;
}

static struct epir_operation *FindOp (opname, ops)
  IN char *opname;			/* name of operation */
  IN struct epir_operationTable *ops;	/* array of defined operations */
{
    int i;
    int oplen = strlen(opname);
    int found;

    found = -1;
    /* TODO: if sorted do binary search... */
    for (i=0; i<ops->nOperations; i++)
	if (strncmp (ops->table[i].name, opname, oplen) == 0) {
	    if (found == -1) found = i;
	    else {
		if (found >= 0)
		    fprintf (stderr,
			     "Operation name '%s' is ambiguous: could be %s\n",
			     opname, ops->table[found].name);
		fprintf (stderr,
			 "Operation name '%s' is ambiguous: could be %s\n",
			 opname, ops->table[i].name);
		found = -2;
	    }
	}
    if (found >= 0) return &ops->table[found];
    return 0;
}

static int RunTestProcess(struct epir_operationData *o)
{
    long code;
    struct epir_operationTable *ops = o->ops;
    opaque rock = o->rock;

#ifdef OSI_TRACE_SYNC
    {   struct icl_set *setp = icl_FindSet("disk/lock");
	if (setp) {
	    icl_InitThreadData (setp, 0, logSize);
	    icl_SetRele(setp);
	}
    }
#endif

    o->pid = pthread_self();
    o->testName[0] = 0;
    o->version = 0;
    code = ParseLine (o, sizeof(o->testName), sizeof(o->line),
		      sizeof(o->whereami), o->testName, o->line, o->whereami);
    if (code) return code;

    sscanf_r (code, (o->line, "%d", &o->version));
    if (o->version != 1) {
	com_err (whoami, 0,
		 "Unknown tester version %d in test %s from file %s",
		 o->version, o->testName, o->testFile);
	return EPI_E_BADTESTVERSION;
    }
    fprintf (stdout, "Running test %s from file %s\n", o->testName, o->testFile);

    BEGIN_CRIT (if (ops->testProlog) code = (*ops->testProlog) (rock, o),
		if (ops->testEpilog) code = (*ops->testEpilog) (rock, o)) ;
    while (!o->endProcessing) {
	struct epir_operation *thisOp;

	code = ParseLine (o,
			  sizeof(o->op), sizeof(o->line), sizeof(o->whereami),
			  o->op, o->line, o->whereami);
	if ((code == -1) && feof(o->testStream)) {
	    code = 0;
	    break;
	} else if (code)
	    break;

	thisOp = FindOp (o->op, ops);
	if (thisOp == 0) {
	    code = ENOENT;
	    com_err (o->whereami, code, "no such operation: %s", o->op);
	    break;
	}

	if (ops->opProlog) {
	    code = (*ops->opProlog) (rock, o);
	    if (code) break;
	}
	code = (*thisOp->proc) (rock, o);
	o->lastCode = code;
	if (ops->opEpilog) {
	    long localCode;
	    localCode = (*ops->opEpilog) (rock, o);
	    if (localCode) break;
	}
	if (code != o->errorCode) {
	    if (code == 0) {
		com_err (o->whereami, o->errorCode,
			 "was expected, instead no error encountered");
		code = EPI_E_BADCODE;
	    }
	    break;
	}
    }
    END_CRIT;
    if (o->testStream != stdin) fclose (o->testStream);

    if (o->flags & EPIR_NEW_PROCESS) {
	pthread_t myPid = o->pid;
	afsl_Assert(!pthread_equal(myPid, *initialProcess));
	if (code) {
	    printf ("Test %s in pid=%#x FAILED\n", o->testName, o->pid);
	    exit (1);
	}
	osi_Free ((opaque)o, sizeof(*o));
	processesDeleted++;
	osi_Wakeup ((caddr_t)&processesCreated);

	pthread_exit(0);
    }

    return code;
}

static void *
RunTestThread(void *arg)
{
    (void) RunTestProcess((struct epir_operationData *)arg);
    return 0;
}

/* EXPORT */
long epir_RunTest (testFile, ops, rock, flags)
  IN char *testFile;			/* file name of test script */
  IN struct epir_operationTable *ops;	/* array of defined operations */
  IN opaque rock;
  IN long flags;
{
    long code;
/*  int newTest = (o->flags & EPIR_NEW_TEST) != 0; */
    int newProcess = (flags & EPIR_NEW_PROCESS) != 0;
    struct epir_operationData opData;

    if (flags & ~3) return EINVAL;	/* only two bits defined */
    bzero ((char *)&opData, sizeof(opData));

    if (strcmp(testFile, "-") == 0) opData.testStream = stdin;
    else {
	opData.testStream = fopen (testFile, "r");
	if (!opData.testStream) {
	    com_err (whoami, errno, "Can't open test file %s", testFile);
	    exit (9);
	}
    }
    opData.testFile = strcpy (osi_Alloc(strlen(testFile)+1), testFile);
    opData.ops = ops;
    opData.rock = rock;
    opData.flags = flags;
    opData.lineNumber = 0;
    opData.endProcessing = 0;

    if (newProcess) {
	/* we can't pass a ptr to our state to sub-process */
	struct epir_operationData *o;
	pthread_t newPid;
	o = (struct epir_operationData *) osi_Alloc (sizeof(*o));
	*o = opData;
	o->procId = ++processesCreated;
	osi_Wakeup ((caddr_t)&processesCreated);
	code = epi_ThreadCreate(RunTestThread, 16000, EPI_NORMAL_PRIORITY,
				 (void *)o, &newPid);
	if (code) com_err (o->whereami, code, "can't create new process");
    } else {

	/* Make sure the initial thread is at same priority and are using the
         * same scheduler as new threads are. */

	code = pthread_setscheduler (pthread_self(),
				     epi_threadScheduler,
				     epi_ActualPriority(EPI_NORMAL_PRIORITY));
	if ((code == -1) && (errno != ENOSYS)) {
	    com_err ("epir_RunTest", errno,
		     "can't set scheduler to %d or priority to %d",
		     epi_threadScheduler,
		     epi_ActualPriority(EPI_NORMAL_PRIORITY));
	    return code;
	}
 
	opData.procId = 0;		/* initialProcess */
	code = RunTestProcess (&opData);
    }
    return code;
}

/* EXPORT */
long epir_WaitAll ()
{
    while (processesCreated != processesDeleted) {
	osi_Sleep ((caddr_t)&processesCreated);
    }
    return 0;
}

/* epir_WaitSome -- sleeps until at least one process exits. */

/* EXPORT */
long epir_WaitSome ()
{
    int del = processesDeleted;
    while (del == processesDeleted) {
	osi_Sleep ((caddr_t)&processesCreated);
    }
    return 0;
}

/* EXPORT */
long epir_StartTest (argc, argv, ops, rock)
  IN int   argc;
  IN char *argv[];
  IN struct epir_operationTable *ops;	/* array of defined operations */
  IN opaque rock;
{
    long code;

    int a;
    char *testFile = 0;
    char *sp;
    int setSleep;
    struct timeval tv;
    int preempt = 0;
    int useA;
    FILE *logfile = NULL;			/* icl log file */

    whoami = argv[0];

    initialize_asy_error_table();
    initialize_lgb_error_table();
    initialize_epi_error_table();

    setSleep = 0;
    for (a = 1; a < argc; a += useA) {
	int count = 0;
	int arglen = strlen(argv[a]);
	char arg[256];

/* test argument for match and bump count on each */
#define IsArg(a) \
	(arglen > 1 && strncmp (arg, a, arglen) == 0 && ++count == 1)

	useA = 1;			/* consumed arguments */
	if (arglen == 0)
	    continue;			/* already processed  */

	if (arglen >= sizeof (arg)) {
	    fprintf(stderr, "argument too long (%s)\n", argv[a]);
	    goto usage;
	}
	lcstring (arg, argv[a], sizeof (arg));
	if (IsArg("-runtest")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    testFile = argv[a + 1];
	}
	if (IsArg("-setsleep")) {
	    setSleep = 1;
	}
	if (IsArg("-setdebug")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    afsdb_episode_anode = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	}
	if (IsArg("-settestdebug")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    afsdb_episode_anode_test = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	    if (afsdb_episode_anode_test & 1) afsdb_episode_runtest |= 1;
	}
	if (IsArg("-setverifydebug")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    afsdb_episode_anode_verify = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	}
	if (IsArg("-setasyncdebug")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    afsdb_episode_async = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	}
	if (IsArg("-setlogbufdebug")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    afsdb_episode_logbuf = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	}
	if (IsArg("-setrecoverdebug")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    afsdb_episode_recover = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	}
	if (IsArg("-setosidebug")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    afsdb_osi = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	}
	if (IsArg("-setpreempt")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    preempt = 1;
	    tv.tv_sec = 0;
	    tv.tv_usec = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;

	}
	if (IsArg("-setscheduler")) {
	    char *type;
	    int typeValue;
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    type = argv[a + 1];
	    typeValue = strtoul(type, &sp, 0);
	    if (sp != type && *sp == '\0') {
		/* typeValue is a number */;
	    } else if (sp == type) { /* non-numeric */
		int i; int j = (int) strlen(type);
		for (i = 0; i < j; i++)
		    if (islower(type[i])) type[i] = toupper(type[i]);
		if (strcmp (type, "FIFO") == 0)
		    typeValue = SCHED_FIFO;
		else if (strcmp (type, "RR") == 0)
		    typeValue = SCHED_RR;
		else {
		    fprintf (stderr, "unknown sched type '%s'\n", type);
		    goto usage;
		}
	    } else {
		fprintf (stderr, "unknown scheduler type '%s'\n", type);
		goto usage;
	    }
	    epi_threadScheduler = typeValue;
	}
	if (IsArg("-setquantum")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    epi_threadQuantum = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	}
	if (IsArg("-settrace")) {
	    /* only global trace settable for now */
	    char *type;
	    int types;
	    if (a + 1 == argc || argv[a + 1][0] == '-') {
		types = -1;
	    } else {
		type = argv[a + 1];
		useA = 2;
		types = strtoul(type, &sp, 0);
		if (sp != type && *sp != '\0') {
		     fprintf (stderr, "unknown trace type '%s'\n", type);
		     goto usage;
		} else if (sp == type) { /* non-numeric */
		    int i; int j = (int) strlen(type);
		    for (i = 0; i < j; i++)
			if (islower(type[i])) type[i] = toupper(type[i]);
		    if (strcmp (type, "ENTRY") == 0)
			types = AFSL_TR_ENTRY;
		    else if (strcmp (type, "EXIT") == 0)
			types = AFSL_TR_EXIT;
		    else if (strcmp (type, "SLEEP") == 0)
			types = AFSL_TR_SLEEP;
		    else if (strcmp (type, "UNUSUAL") == 0)
			types = AFSL_TR_UNUSUAL;
		    else if (strcmp (type, "ERRMAP") == 0)
			types = AFSL_TR_ERRMAP;
		    else if (strcmp (type, "ALWAYS") == 0)
			types = AFSL_TR_ALWAYS;
		    else if (strcmp (type, "ASSERT") == 0)
			types = AFSL_TR_ASSERT;
		    else {
		        fprintf (stderr, "unknown trace type '%s'\n", type);
		        goto usage;
		    }
		}
		if (types >= 0) types = 1 << types;
	    }
	    afsl_tr_global.enabledTypes |= types;
	}
	if (IsArg("-errorcode")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    code = atoi(argv[a + 1]);
	    printf ("Message for %d is '%s'\n", code, dfs_dceErrTxt (code));
	    exit (0);
	}
	if (IsArg("-traceanode")) {
#ifndef AFS_DEBUG
	    printf("-traceanode disabled (AFS_DEBUG was turned off)\n");
#else
	    long d = 0;			/* minor device number */
	    while (a + useA < argc && argv[a + useA][0] != '-') {
		/* these come from fixed_anode.c (when AFS_DEBUG is defined */
		extern int epix_tr_downCounter;
		extern struct anodeList {
			long mindev;
			u_long volIndex;
			u_long anodeIndex;
		    } *epix_tr_anodeList;
		int size;		/* new size of array */
		u_long vi, ai;

		useA++;
		if (argv[a + useA - 1][0] == '#') {
		    epix_tr_downCounter = atoi(argv[a+useA]+1);
		    continue;
		}
		if (argv[a + useA - 1][0] == 'd') {
		    d = atoi(argv[a + useA] + 1);
		    continue;
		}
		vi = ai = 0;
		sscanf_r (code, (argv[a + useA - 1], "%lu,%lu", &vi, &ai));
		if (!vi || !ai) goto usage; size = 2;
		if (epix_tr_anodeList) {
		    struct anodeList *new;
		    for (size = 0; epix_tr_anodeList[size].volIndex; size++);
		    size += 2;
		    new = (struct anodeList *)
			osi_Alloc ((size + 1) * sizeof (new[0]));
		    bcopy ((char *)epix_tr_anodeList,
			   (char *)new,
			   size * sizeof (new[0]));
		    osi_Free ((opaque)epix_tr_anodeList,
			size * sizeof (new[0]));
		    epix_tr_anodeList = new;
		} else {
		    epix_tr_anodeList = (struct anodeList *)
			osi_Alloc ((size) * sizeof (epix_tr_anodeList[0]));
		}
		epix_tr_anodeList[size - 2].mindev = d;
		epix_tr_anodeList[size - 2].volIndex = vi;
		epix_tr_anodeList[size - 2].anodeIndex = ai;
		epix_tr_anodeList[size - 1].volIndex = 0;
	    }
	    if (useA == 1) goto missing;
#endif /* AFS_DEBUG */
	}
	if (IsArg("-tracebuffer")) {
#if 1
	    printf("tracebuffer is disabled\n");
#else
	    long d = 0;			/* minor device number */
	    while (a + useA < argc && argv[a + useA][0] != '-') {
		/* these come from buffer.c */
		extern int elbb_tr_downCounter;
		extern struct blockList {
			long mindev;
			daddr_t block;
		    } *elbb_tr_blockList;
		int size;		/* new size of array */
		daddr_t b;

		useA++;
		if (argv[a + useA - 1][0] == '#') {
		    elbb_tr_downCounter = atoi(argv[a + useA - 1] + 1);
		    continue;
		}
		if (argv[a + useA - 1][0] == 'd') {
		    d = atoi(argv[a + useA - 1] + 1);
		    continue;
		}
		b = 0;
		sscanf_r (code, (argv[a + useA - 1], "%d", &b));
		if (!b) goto usage;
		size = 2;
		if (elbb_tr_blockList) {
		    struct blockList *new;
		    for (size = 0; elbb_tr_blockList[size].block; size++);
		    size += 2;
		    new = (struct blockList *)
			osi_Alloc ((size + 1) * sizeof (new[0]));
		    bcopy (elbb_tr_blockList, new, size * sizeof (new[0]));
		    osi_Free ((opaque)elbb_tr_blockList,
			      size * sizeof (new[0]));
		    elbb_tr_blockList = new;
		} else {
		    elbb_tr_blockList = (struct blockList *)
			osi_Alloc ((size) * sizeof (elbb_tr_blockList[0]));
		}
		elbb_tr_blockList[size - 2].mindev = d;
		elbb_tr_blockList[size - 2].block = b;
		elbb_tr_blockList[size - 1].block = 0;
	    }
	    if (useA == 1) goto missing;
#endif /* if 1 */
	}
	if (IsArg("-variation")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    testVariation = atoi(argv[a + 1]);
	    if (testVariation)
		printf ("Running variation %d of test\n", testVariation);
	}
	if (IsArg("-icl")) {
	    char *logfileName;
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    logfileName = argv[a + 1];
	    logfile = fopen(logfileName, "w");
	    if (logfile == NULL) {
		perror ("Opening icl log file");
		goto usage;
	    }
	    printf ("Will write icl log to %s\n", logfileName);
	}
	if (IsArg("-enable")) {
	    char *setname;
	    struct icl_set *setp;

	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    setname = argv[a + 1];
	    setp = icl_FindSet(setname);
	    if (setp) {
		code = icl_SetSetStat(setp, ICL_OP_SS_ACTIVATE);
		afsl_MBZ (code);
		icl_SetRele(setp);
	    } else {
		fprintf (stderr, "can't find set %s.\n", setname);
	    }
	}
	if (IsArg("-logsize")) {
	    if (a + 1 == argc)
		goto missing;
	    useA = 2;
	    logSize = atoi(argv[a + 1]);
	}
	if (count > 1)
	    goto ambiguous;
	if (arglen > 1 && arg[0] == '-') {
	    if (count == 0) {
		fprintf (stderr, "unexpected arg '%s'\n", arg);
		goto usage;
	    }
	} else {	/* otherwise assume it's a test filename */
	    if (testFile) {
		fprintf (stderr, "test (%s) already specified; can't run %s\n",
			 testFile, argv[a]);
		goto usage;
	    }
	    testFile = argv[a];
	}
    }
    if (setSleep) {
	u_long mask = (u_long) 1 << (EPIA_DEBUG_SLEEP - 1);
	afsdb_episode_anode_test |= mask;
	afsdb_episode_anode_verify |= mask;
	afsdb_episode_async |= mask;
	afsdb_episode_logbuf |= mask;
	afsdb_episode_anode |= mask;
	afsdb_osi |= mask;
	afsl_tr_global.enabledTypes |= (1<<AFSL_TR_SLEEP);
    }
    if (afsdb_episode_anode_test & 4) {
	/* enable coverage counting */
	afsdb_episode_anode |= (1<<(EPIA_DEBUG_CODEBLOCKS-1));
    }
    if (afsdb_episode_anode_test ||
	afsdb_episode_anode_verify ||
	afsdb_episode_async ||
	afsdb_episode_logbuf ||
	afsdb_episode_anode ||
	afsdb_osi ||
	afsl_tr_global.enabledTypes) {
	printf ("Running with debugging on %#x, %#x (test), %#x (verify), %#x (async), %#x (logbuf) %#x (osi); %#x (tr_global).\n",
		afsdb_episode_anode, afsdb_episode_anode_test,
		afsdb_episode_anode_verify, afsdb_episode_async,
		afsdb_episode_logbuf, afsdb_osi, afsl_tr_global.enabledTypes);
    }
    if (logSize && logfile) {
	struct icl_log *logp = icl_FindLog("disk");
	if (logp) {
	    /* setting the log clears the old contents... */
	    icl_DumpUserLog(logfile, "disk");
	    icl_LogSetSize(logp, logSize);
	    icl_LogRele(logp);
	}
    }

    if (initialProcess == NULL) {
	initialProcess = osi_Alloc(sizeof (*initialProcess));
	*initialProcess = pthread_self();
    }

    if (testFile) { 
	code = epir_RunTest (testFile, ops, rock, EPIR_NEW_TEST);
	if (code) return code;
    }
    if (logfile) {
	printf("Writing ICL log.\n");
	icl_DumpUserToFile(logfile);
	fclose(logfile);
    }
    return 0;

ambiguous:
    fprintf(stderr, "ambiguous argument '%s'\n", argv[a]);
    goto usage;

missing:
    fprintf(stderr, "missing argument following '%s'\n", argv[a]);
usage:
    fprintf (stderr,
"Usage is: '%s [[-runtest] <testfile>] [-set{,test,verify,async,logbuf,recover,osi}debug <n>] [-setpreempt <ms> ] [-settrace [<mask> | <typename>]]+ [-setsleep] [-traceanode [+<skip count>] {d<mindev> | <volIx>,<anodeIx>}+] [-tracebuffer [+<skip count>] {d<mindev> | <blk>}+] [-variation <n>] [-setscheduler {<number>| RR | FIFO}] [-setquantum <usec>]\n",
	whoami);
    exit (1);
}
