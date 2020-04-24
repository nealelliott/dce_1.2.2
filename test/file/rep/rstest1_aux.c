/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: rstest1_aux.c,v $
 * Revision 1.1.6.1  1996/10/17  18:34:46  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:57:18  damon]
 *
 * $EndLog$
 */

/*
 * (C) Copyright 1994 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/* $Id: rstest1_aux.c,v 1.1.6.1 1996/10/17 18:34:46 damon Exp $ */

/*
 * The basic idea here is to fork off two processes for each replicated
 * fileset named as an argument.  One process, the writer, updates the test
 * file's mtime, according to "RS_UPDATE_FREQ".  The other process, the
 * reader, keeps track of how often the file's stat information shows that
 * the file is within the fileset's max age (passed to this program via
 * "RS_MAX_AGE").  The reader reports its findings back to the main program
 * via a pipe, and the main program tallies the results and prints them.
 *
 * Each fileset is expected to have two mount points under "RS_TEST_DIR",
 * one for the RW and one for the RO, so if "ft1" is the name of a fileset
 * we expect mount points named "ft1" and "ft1.readonly".
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>

/*
 * Types
 */
/* This enumerated type is used for the `ep_type' member of struct
 * env_param.
 */
typedef enum env_param_type {
    ENV_PARAM_TYPE_SENTINEL,		/* Marks the end of the list */
    ENV_PARAM_TYPE_NUMERIC,		/* A numeric parameter */
    ENV_PARAM_TYPE_STRING		/* A string parameter */
} env_param_type_t;


/* This structure is used along with ImportEnvParams() to initialize some
 * program parameters based on the values of environment variables.
 */
typedef struct env_param {
    char*		ep_name;	/* The environment variable's name */
    env_param_type_t	ep_type;	/* The type of the parameter */
    char*		ep_default;	/* A default value */
    void*		ep_value;	/* Where to store the real value */
} env_param_t;

/* This structure is passed back from the reader process and is used to
 * tally the results.
 */
typedef struct reader_result {
    unsigned long	rr_reads;	/* Total number of reads (stats) */
    unsigned long	rr_successes;	/* Number of times within deadline */
} reader_result_t;

/*
 * Constants
 */
const char*	REPLICA_SUFFIX = ".readonly";
const char*	TEST_FILE_NAME = "junk";

/*
 * Forward Declarations
 */
static void	DisplayParams(const env_param_t*); /* Prints parameter vals */
static void	EndTest(int);		       /* Print results and exit */
static void	ForkReader(const char*, int*); /* Forks a reader proc */
static void	ForkWriter(const char*);       /* Forks a writer proc */
static void	GatherResults(const fd_set*);  /* Gathers results from the */
					       /* readers */
static void	ImportEnvParams(env_param_t*); /* Initializes parmeters from */
					       /* environment */
static void	KillPG(void);		       /* Kills the process group */
static void	PrintResults(void);	       /* Report results */
static void	Reader(const char*);	       /* Does the reader processing */
static void	Writer(const char*);	       /* Does the writer processing */

/*
 * Globals/Parameters
 */
long		checkFreq;		/* How often the reader checks */
reader_result_t	cumResults;		/* Cumulative results */
long		maxAge;			/* Replica's max age */
long		reportInterval;		/* How often the reader reports back */
char*		testDir;		/* The directory that contains the */
					/* mount points */
long		timeLimit;		/* User specified time limit for the */
					/* test */
long		updateFreq;		/* How often the writer updates */

static env_param_t envParams[] = {
    {"RS_CHECK_FREQ", ENV_PARAM_TYPE_NUMERIC, "5", &checkFreq},
    {"RS_MAX_AGE", ENV_PARAM_TYPE_NUMERIC, "120", &maxAge},
    {"RS_REPORT_INTERVAL", ENV_PARAM_TYPE_NUMERIC, "5", &reportInterval},
    {"RS_TEST_DIR", ENV_PARAM_TYPE_STRING, ".", &testDir},
    {"RS_TIME_LIMIT", ENV_PARAM_TYPE_NUMERIC, "0", &timeLimit},
    {"RS_UPDATE_FREQ", ENV_PARAM_TYPE_NUMERIC, "30", &updateFreq},
    {ENV_PARAM_TYPE_SENTINEL}
};

/*
 * main()
 *	Initialize the parameters, fork the processes, and gather the
 *	results. 
 */
int
main(int argc, char* argv[])
{
    fd_set		fdSet;
    struct sigaction	sigAct;

    FD_ZERO(&fdSet);

    /* Get parameters from the environment */
    ImportEnvParams(envParams);

    /* Display the paremeters for this run */
    DisplayParams(envParams);

    if (argc < 2) {
	exit(2);			/* Nothing to do */
    }

    /* Each argument is the name of a fileset/mountpoint */
    while (--argc) {
	int	fd;
	int	fdFlags;
	char*	filesetName;

	filesetName = *++argv;

	ForkWriter(filesetName);

	ForkReader(filesetName, &fd);

	/*
	 * Set non-blocking mode on input end of reader pipe
	 */
	/* Get current flags */
	if ((fdFlags = fcntl(fd, F_GETFL)) == -1) {
	    perror("Can't get file descriptor flags");
	    KillPG();
	}

	/* Add O_NONBLOCK */
	fdFlags |= O_NONBLOCK;

	/* Set new flags */
	if (fcntl(fd, F_SETFL, fdFlags) == -1) {
	    perror("Can't set file descriptor flags");
	    KillPG();
	}

	/* Add fd to the set of file descriptors to select on */
	FD_SET(fd, &fdSet);
    }

    cumResults.rr_reads = 0;
    cumResults.rr_successes = 0;

    /*
     * Set handlers to print results and end the test on a user interrupt or
     * when a user-specified time limit expires.
     */
    sigAct.sa_handler = EndTest;
    sigAct.sa_flags = 0;
    sigemptyset(&sigAct.sa_mask);
    sigaddset(&sigAct.sa_mask, SIGALRM); /* No alarms while handling */

    if (sigaction(SIGINT, &sigAct, NULL) == -1) {
	perror("Can't set handler for interrupt signals");
	KillPG();
    }

    if (timeLimit != 0) {
	/* Set an alarm handler if the user requested a time limit */
	sigdelset(&sigAct.sa_mask, SIGALRM);
	sigaddset(&sigAct.sa_mask, SIGINT); /* No interrupts while handling */

	if (sigaction(SIGALRM, &sigAct, NULL) == -1) {
	    perror("Can't set handler for alarm signals");
	    KillPG();
	}

	(void)alarm(timeLimit);
    }

    (void)printf("\nStarting the test...\n");

    /* We're done forking the children, now we'll gather the results */
    GatherResults(&fdSet);
    assert(0);			/* We should never get here */
}	/* main() */

/*
 * DisplayParams()
 *	Initialize global variables based on environment.
 */
static void
DisplayParams(const env_param_t* envParamP)
{
    const env_param_t*	ep;

    (void)printf("Parameters:\n");

    for (ep = envParamP; ep->ep_type != ENV_PARAM_TYPE_SENTINEL; ep++) {
	(void)printf("\t%s=", ep->ep_name);

	switch (ep->ep_type) {
	  case ENV_PARAM_TYPE_NUMERIC:
	    (void)printf("%ld\n", *(long*)ep->ep_value);
	    break;
	  case ENV_PARAM_TYPE_STRING:
	    (void)printf("\"%s\"\n", *(char**)ep->ep_value);
	    break;
	  default:
	    assert(0);		/* Unknown type */
	    break;
	}
    }
}	/* DisplayParams() */

/*
 * EndTest()
 *	Print the results to this point and terminate the test.
 */
static void
EndTest(int whichSignal)
{
    (void)printf("\nTest complete: ");

    switch (whichSignal) {
      case SIGALRM:
	(void)printf("time limit expired\n");
	break;
      case SIGINT:
	(void)printf("user interrupt\n");
	break;
      default:
	assert(0);			/* Unexpected signal */
	break;
    }

    PrintResults();
    KillPG();
}	/* EndTest() */

/*
 * ForkReader()
 *	Construct a pipe, fork the reader process, and return the file
 *	descriptor for the input end of the pipe.
 */
static void
ForkReader(const char* filesetName, int* fdP)
{
    pid_t	pid;
    int		pipeFd[2];

    /*
     * Create a pipe for communication with the reader process
     */
    if (pipe(pipeFd) == -1) {
	perror("Can't create pipe");
	KillPG();
    }
    
    /*
     * Fork off the reader process
     */
    if ((pid = fork()) == -1) {
	perror("Can't fork reader process");
	kill(0, SIGTERM);
    } else if (pid == 0) {
	/* We're in the child process now */
	
	/* Close standard output */
	if (close(1) == -1) {
	    perror("Can't close standard output");
	    KillPG();
	}
	
	/* Dup the output end of the pipe onto our standard output */
	if (dup2(pipeFd[1], 1) == -1) {
	    perror("Can't dup pipe file descriptor");
	    KillPG();
	}
	
	/* Close unneeded file descriptors */
	if (close(pipeFd[0]) == -1) {
	    perror("Can't close input end of pipe in reader");
	    KillPG();
	}
	if (close(pipeFd[1]) == -1) {
	    perror("Can't close output end of pipe in reader");
	    KillPG();
	}
	
	/* Do the real work of the reader */
	Reader(filesetName);
	assert(0);		/* We should never get here */
    }
    
    /* Close the output end of the pipe in the parent */
    if (close(pipeFd[1]) == -1) {
	perror("Can't close output end of pipe in parent");
	KillPG();
    }
    
    *fdP = pipeFd[0];
}	/* ForkReader() */

/*
 * ForkWriter()
 *	Fork the reader process
 */
static void
ForkWriter(const char* filesetName)
{
    pid_t	pid;

    if ((pid = fork()) == -1) {
	perror("Can't fork writer process");
	KillPG();
    } else if (pid == 0) {
	/* We're in the child process now */
	Writer(filesetName);
	assert(0);		/* We should never get here */
    }
}

/*
 * GatherResults()
 *	Loop forever gathering results from the reader processes.
 */
static void
GatherResults(const fd_set* fdSet)
{
    int			i;
    int			numReadyFds;
    fd_set		readyFds;
    reader_result_t	result;
    long		maxFds;

    if ((maxFds = sysconf(_SC_OPEN_MAX)) == -1) {
	perror("Can't determine maximum number of file descriptors\n");
	KillPG();
    }

    for (;;) {
	/* Wait until at least one reader has a result */
	readyFds = *fdSet;
	if ((numReadyFds = select(maxFds, &readyFds, NULL, NULL, NULL)) == -1){
	    perror("Select failed");
	    KillPG();
	}

	/* Find out which readers are ready to report and read their results */
	i = 0;
	while(numReadyFds > 0) {
	    if (FD_ISSET(i, &readyFds)) {
		int	nbytes;

		--numReadyFds;

		/* Read as many result structures as are available */
		while ((nbytes = read(i, &result, sizeof result)) > 0) {
		    if (nbytes != sizeof result) {
			(void)fprintf(stderr, "Too few bytes in result\n");
			KillPG();
		    }

		    cumResults.rr_reads += result.rr_reads;
		    cumResults.rr_successes += result.rr_successes;

		    PrintResults();
		}

		if (nbytes == 0) {
		    /* The other end of the pipe was closed prematurely */
		    (void)fprintf(stderr, "Reader closed pipe prematurely\n");
		    KillPG();
		} else if (errno != EAGAIN) {
		    perror("Can't read pipe");
		    KillPG();
		}
	    }

	    i++;
	}
    }
}	/* GatherResults() */

/*
 * ImportEnvParams()
 *	Initialize global variables based on environment.
 */
static void
ImportEnvParams(env_param_t* envParamP)
{
    env_param_t*	ep;

    for (ep = envParamP; ep->ep_type != ENV_PARAM_TYPE_SENTINEL; ep++) {
	char*	valueStr = getenv(ep->ep_name);

	if (valueStr == NULL) {
	    valueStr = ep->ep_default;
	}

	switch (ep->ep_type) {
	  case ENV_PARAM_TYPE_NUMERIC:
	    *(long*)ep->ep_value = strtol(valueStr, NULL, 0);
	    break;
	  case ENV_PARAM_TYPE_STRING:
	    *(char**)ep->ep_value = valueStr;
	    break;
	  default:
	    assert(0);		/* Unknown type */
	    break;
	}
    }
}	/* ImportEnvParams() */

/*
 * KillPG()
 *	Send SIGTERM to our process group.
 */
static void
KillPG(void)
{
    if (kill(0, SIGTERM) == -1) {
	/* This should really never happen */
	perror("Failed to terminate process group");
	exit(2);			/* We'll bail out anyway */
    }
    assert(0);			/* We really should never get here */
}	/* KillPG() */

/*
 * PrintResults()
 *	Print results to standard output.
 */
static void
PrintResults(void)
{
    double	successRate = 0.0;

    if (cumResults.rr_reads != 0) {
	successRate = (double)cumResults.rr_successes / 
	    (double)cumResults.rr_reads * 100.0;
    }
    (void)printf("reads = %4lu, successes = %4lu, rate = %6.2f%%\n",
		 cumResults.rr_reads, cumResults.rr_successes, successRate);
}	/* PrintResults() */

/*
 * Reader()
 *	Loop stat'ing the test file and sending results back to our parent.
 *
 *	We don't need to call KillPG() here, since exiting will close the
 *	pipe.  Our parent will notice and terminate the rest of the process
 *	group.
 */
static void
Reader(const char* filesetName)
{
    time_t		curTime;
    unsigned		i = 0;
    reader_result_t	result;
    struct stat		statBuf;
    char		testFile[MAXPATHLEN];

    /* Give the writer a chance to do the first update */
    sleep(maxAge + updateFreq);

    (void)sprintf(testFile, "%s/%s%s/%s",
		  testDir, filesetName, REPLICA_SUFFIX, TEST_FILE_NAME);

    result.rr_reads = 0;
    result.rr_successes = 0;

    for (;;) {
	/* Stat the file to determine its mtime */
	if (stat(testFile, &statBuf) == -1) {
	    perror("stat");
	    exit(2);
	}

	/* Get the current time */
	if (time(&curTime) == -1) {
	    perror("time");
	    exit(2);
	}

	result.rr_reads++;
	if (curTime <= statBuf.st_mtime + maxAge + updateFreq) {
	    /* If file is no older than the replica's max age plus our */
	    /* update frequency, the repserver has succeeded in meeting its */
	    /* commitments */
	    result.rr_successes++;
	}

	if (++i >= reportInterval) {
	    /* It's time to report the last batch of results */
	    if (write(1, &result, sizeof result) != sizeof result) {
		perror("Can't write result back to parent");
		exit(2);
	    }

	    /* Prepare for another batch of results */
	    i = 0;
	    result.rr_reads = 0;
	    result.rr_successes = 0;
	}

	(void)sleep(checkFreq);
    }
}

/*
 * Writer()
 *	Loop updating the test file.
 */
static void
Writer(const char* filesetName)
{
    char	testFile[MAXPATHLEN];
	
    (void)sprintf(testFile, "%s/%s/%s",
		  testDir, filesetName, TEST_FILE_NAME);

    for (;;) {
	/* "Touch" the test file */
	if (utime(testFile, NULL) == -1) {
	    perror("Can't update test file's mtime");
	    KillPG();
	}

	(void)sleep(updateFreq);
    }
}	/* Writer() */
