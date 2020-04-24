/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dtetapi_stubs.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:21  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:16  root]
 * 
 * Revision 1.1.2.2  1994/06/17  18:46:08  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:36:55  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/26  22:44:25  fadden
 * 	Initial version
 * 	[1994/05/26  20:10:43  fadden]
 * 
 * $EndLog$
 */

#include <errno.h>
#include <time.h>
#include <tet_api.h>

#define	VERSION		"1.10"
#define ARGS_COUNT	0
#define ARGS_EXECUTE	1
#define	KILLWAIT	10

extern	void	(*tet_startup)();
extern	void	(*tet_cleanup)();
extern	struct tet_testlist
		tet_testlist[];


static int count_tps(int);
static int callfuncs(int);
static int procargs(char** , int);
void tet_tpstart(int);
void tet_tpend(int);
void tet_icstart(int, int);
void tet_icend(int, int);
void tet_tcmstart(char*, int);
void tet_error(int, char* );
static char * curtime();
char *tet_errname(int errno_val);

char *	tet_pname;
int	tet_thistest = 0;
int	tet_nosigreset = 0;

int
main(argc, argv)
int	argc;
char	**argv;
{
	tet_pname = argv[0]; /* used by tet_error() */

#	ifdef NEVER
	/* read in configuration variables */
	tet_config();
#	endif

	/* count number of IC's to be executed and output on TCM Start line */
	tet_tcmstart(VERSION, procargs(argv, ARGS_COUNT));

	/* call user-supplied startup function */
	if (tet_startup != NULL)
		(*tet_startup)();
	
	/* execute IC's specified on command line */
	(void) procargs(argv, ARGS_EXECUTE);

	/* call user-supplied cleanup function */
	if (tet_cleanup != NULL)
	{
		(*tet_cleanup)();
	}

	exit(0);

}

static int
procargs(argv, action)
char **argv;
int action;
{
	/*
	 * Process the IC list given on the command line.  If action is
	 * ARGS_COUNT simply return an IC count.  If action is ARGS_EXECUTE
	 * then execute each IC.
	 */

	char *cp;
	struct tet_testlist *tp;
	int ic_no, ic_first, ic_last, ic_count, testcnt;
	static char *default_args[] = { "arg0", "all", NULL };

	/*
	 * Each argument contains an invocable component list of the
	 * form: 1,3-5,7,11,... 
	 *
	 * If any argument is "all" then all components later than
	 * the last in the previous argument are executed.
	 *
	 * Default if no arguments are given is "all".
	 */

	if (argv[1] == NULL)
		argv = default_args;

	ic_last = -1;
	ic_count = 0;
	while (*++argv != NULL)
	{
		if (strncmp(*argv, "all", (size_t)3) == 0)
		{
			if (ic_last < 0)
			{
				/* no previous ICs executed - start at lowest */
				ic_first = tet_testlist[0].icref;
				for (tp = tet_testlist;
						tp->testfunc != NULL; tp++)
				{
					if (tp->icref < ic_first)
						ic_first = tp->icref;
				}
			}
			else
				/* carry on from last IC executed */
				ic_first = ic_last + 1;

			ic_last = ic_first;
			for (tp = tet_testlist; tp->testfunc != NULL; tp++)
			{
				if (tp->icref > ic_last)
					ic_last = tp->icref;
			}

			for (ic_no = ic_first; ic_no <= ic_last; ic_no++)
			{
				testcnt = count_tps(ic_no);

				/* skip ICs with zero TPs */
				if (testcnt == 0)
					continue;

				ic_count++;
				if (action == ARGS_EXECUTE)
				{
					/* output IC start message */
					tet_icstart(ic_no, testcnt);

					testcnt = callfuncs(ic_no);

					/* output IC end message */
					tet_icend(ic_no, testcnt);
				}
			}
		}
		else
		{
			for (cp = *argv; *cp != '\0'; cp++)
			{
				ic_first = atoi(cp);
				while(*cp != ',' && *cp != '-' && *cp != '\0')
					cp++;
				if (*cp == '-')
				{
					ic_last = atoi(++cp);
					while(*cp != ',' && *cp != '\0')
						cp++;
				}
				else
					ic_last = ic_first;

				for (ic_no = ic_first;
						ic_no <= ic_last; ic_no++)
				{
					testcnt = count_tps(ic_no);

					/* skip ICs with zero TPs */
					if (testcnt == 0)
					{
						if (action == ARGS_EXECUTE &&
						   (ic_no == ic_first ||
						    ic_no == ic_last))
						{
							printf(
			    "ERROR: IC %d is not defined for this test case\n",
								ic_no);
						}
						continue;
					}

					ic_count++;
					if (action == ARGS_EXECUTE)
					{
						/* output IC start message */
						tet_icstart(ic_no, testcnt);

						testcnt = callfuncs(ic_no);

						/* output IC end message */
						tet_icend(ic_no, testcnt);
					}
				}

				if (*cp == '\0')
					break;
			}
		}
	}

	return ic_count;
}

static int
count_tps(ic_no)
int ic_no;
{
	/* count number of tests in specified invocable component */
	/* or in all ICs if ic_no < 0 */

	struct tet_testlist *tp;
	int testcnt = 0;

	for (tp = tet_testlist; tp->testfunc != NULL; tp++)
	{
		if (ic_no < 0 || tp->icref == ic_no)
			testcnt++;
	}

	return testcnt;
}

static int
callfuncs(ic_no)
int ic_no;
{
	/* call each test function in specified invocable component */
	/* return count of test functions called */

	struct tet_testlist *tp;
	int test_no, testcnt;
	int signum;

	testcnt = 0;
	for (test_no = 1, tp = tet_testlist;
			tp->testfunc != NULL; test_no++, tp++)
	{
		if (tp->icref != ic_no)
			continue;
		/* output test start message */
		tet_tpstart(test_no);

		/* set global current test purpose indicator */
		tet_thistest = test_no;

		(*tp->testfunc)();

		/* output test result */
		tet_tpend(test_no);

		testcnt++;
	}

	return testcnt;
}

void tet_infoline(char *msg)
{
  printf("TET_INFOLINE: %s\n",msg);
}

void tet_result(int res)
{

  char *codes[]={"PASS", "FAIL", "UNRESOLVED", "NOTINUSE", "UNSUPPORTED",
		   "UNTESTED", "UNINITIATED", "NORESULT"};

  if (res < 0 || res > 7)
    printf("TET_RESULT: Result set to %d\n", res );
  else
    printf("TET_RESULT: Result set to %s\n", codes[res]);

}

int tet_remgetsys(void)
{
  
  static int sysid=-1;
  char idstr[10];
  
  if (sysid==-1) {
    
    printf("TET_REMGETSYS: called for the first time.\n");
    printf("\tEnter system id: ");
    gets(idstr);
    sysid=atoi(idstr); 
  } else
    printf("TET_REMGETSYS: called again.  Returning %d as sysid.\n", sysid);
  
  return(sysid);

}


int tet_sync(int spt, int *bar, int timeout)
{
  char str[10];
  
  printf("TET_SYNC: Reached sync-point %d with timeout of %d at %s\n", 
	 spt, timeout, curtime());
  printf("\t- Press return when synced - ");
  gets(str);
  return(0);

}

int tet_remgetlist(int **foo)
{
  
  static dummy[]={1,2,3,4,5,6,7,8,9,10};
  static int numslaves=-1;
  char idstr[10];
  
  if (numslaves==-1) {
    
    printf("TET_REMGETLIST: called for the first time.\n");
    printf("\tEnter number of slaves: ");
    gets(idstr);
    numslaves=atoi(idstr); 
  } else
    printf("TET_REMGETLIST: called again.  Returning %d as number of slaves.\n"
	   , numslaves);

  *foo=dummy;
  return(numslaves);

}

void
tet_tcmstart(versn, no_ics)
char *versn;
int no_ics;
{

	printf("TC START: %s %d\n", versn, no_ics);
}

void
tet_icstart(ic_num, no_tests)
int ic_num;
int no_tests;
{
	printf("IC START: %d %d %s\n", ic_num, no_tests, curtime());
}

void
tet_icend(ic_num, no_tests)
int ic_num;
int no_tests;
{
	printf("IC END: %d %d %s\n", ic_num, no_tests, curtime());

}

void
tet_tpstart(tp_num)
int tp_num;
{

  printf("TP START: %d %s\n", tp_num, curtime());

}

void
tet_tpend(tp_num)
int tp_num;
{

	printf("TP END: %d %s\n", tp_num, curtime());
}

void
tet_error(errno_val, msg)
int errno_val;
char *msg;
{
	/* print error message on stderr and in results file */

	size_t len;
	extern char *tet_pname;

	printf("ERROR: %s: %s\n", tet_pname, msg);
	if (errno_val > 0)
		printf("ERROR: %s: errno = %d (%s)\n", tet_pname,
			errno_val, tet_errname(errno_val));

	/* don't use output() to write results file - it calls tet_error() */
}

static char *
curtime()
{
	/* return string containing current time */

	time_t t;
	struct tm *tp;
	static char s[10];

	(void) time(&t);
	tp = localtime(&t);
	(void) sprintf(s, "%02d:%02d:%02d", tp->tm_hour,
		tp->tm_min, tp->tm_sec);

	return s;
}

char *
tet_errname(err)
int err;
{
	/* look up name for given errno value */

	int	i;
	static struct {
		int num;
		char *name;
	} err_table[] = {
		E2BIG,		"E2BIG",
		EACCES,		"EACCES",
		EAGAIN,		"EAGAIN",
		EBADF,		"EBADF",
		EBUSY,		"EBUSY",
		ECHILD,		"ECHILD",
		EDEADLK,	"EDEADLK",
		EDOM,		"EDOM",
		EEXIST,		"EEXIST",
		EFAULT,		"EFAULT",
		EFBIG,		"EFBIG",
		EINTR,		"EINTR",
		EINVAL,		"EINVAL",
		EIO,		"EIO",
		EISDIR,		"EISDIR",
		EMFILE,		"EMFILE",
		EMLINK,		"EMLINK",
		ENAMETOOLONG,	"ENAMETOOLONG",
		ENFILE,		"ENFILE",
		ENODEV,		"ENODEV",
		ENOENT,		"ENOENT",
		ENOEXEC,	"ENOEXEC",
		ENOLCK,		"ENOLCK",
		ENOMEM,		"ENOMEM",
		ENOSPC,		"ENOSPC",
		ENOSYS,		"ENOSYS",
		ENOTDIR,	"ENOTDIR",
		ENOTEMPTY,	"ENOTEMPTY",
		ENOTTY,		"ENOTTY",
		ENXIO,		"ENXIO",
		EPERM,		"EPERM",
		EPIPE,		"EPIPE",
		ERANGE,		"ERANGE",
		EROFS,		"EROFS",
		ESPIPE,		"ESPIPE",
		ESRCH,		"ESRCH",
		EXDEV,		"EXDEV",
#ifdef EIDRM
		EIDRM,		"EIDRM",
#endif
#ifdef ENOMSG
		ENOMSG,		"ENOMSG",
#endif
#ifdef ENOTBLK
		ENOTBLK,	"ENOTBLK",
#endif
#ifdef ETXTBSY
		ETXTBSY,	"ETXTBSY",
#endif
		0,		NULL
	};

	for (i = 0; err_table[i].name != NULL; i++)
	{
		if (err_table[i].num == err)
			return err_table[i].name;
	}

	return "unknown errno";
}
