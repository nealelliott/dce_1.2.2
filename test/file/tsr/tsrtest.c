/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsrtest.c,v $
 * Revision 1.1.108.1  1996/10/17  18:36:59  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:58:09  damon]
 *
 * Revision 1.1.103.1  1994/02/04  20:47:38  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:38  devsrc]
 * 
 * Revision 1.1.101.1  1993/12/07  17:46:38  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:42:03  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:37:24  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:22:16  htf]
 * 
 * Revision 1.1.2.2  1992/11/19  13:37:19  jaffe
 * 	New File
 * 	[1992/11/18  17:04:15  jaffe]
 * 
 * $EndLog$
 */
/* 
 * The program is used to test wether the CM is able to perform the token state
 * recovery (TSR) work correctly after 1) the server comes back or 2) the 
 * network partition gets repaired. 
 * 
 * USAGE 
 *       tsrtest 
 *                -dir     :  where the test dir is created, default is /:
 *                -files   :  Number of files to be created
 *                -verbose :  Print out usefull info.
 *                -nopause :  The program will not pause for the tsr test
 *                -flock   :  Use system V lock to perform file operations. 
 */

/*****************************************************************************/
/* Copyright (C) 1990, 1991 Transarc Corporation - All rights reserved */
/*****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/param.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/file.h>     /* flock() */
#include <unistd.h>       /* lockf() */

#define MAXFILES               50
#define BUFLEN                 20000
#define EQ(s,t)       (strcmp(s,t) == 0)
#define FIRST                  3

#if defined(SUNOS5) || defined(__hpux)
#define getwd(x) getcwd(x,sizeof(x))
#endif

/*
 *  Global variables.
 */

char *progName;

extern int errno;

usage()
{
    fprintf(stdout, "\nusage: %s [-dir testdir] [-files files] [-verbose] [-flock] [-nopause] \n", progName);
    fprintf(stdout, "    -dir testdir  where the test dir is created\n");    
    fprintf(stdout, "    -files files  Num of files to be crated\n");
    fprintf(stdout, "    -flock        Use system V type locks\n");
    fprintf(stdout, "    -verbose      print more info \n");
    fprintf(stdout, "    -nopause      the program will not pause and wait for the command to resume\n\n");

}

/* VARARGS */
printmsg(str, ar1, ar2, ar3, ar4, ar5, ar6, ar7, ar8, ar9)
      char *str;
{

    fprintf(stderr, str, ar1, ar2, ar3, ar4, ar5, ar6, ar7, ar8, ar9);
}


/*
 *  get parameter at parm, convert to int, and make sure that
 *  it is at least min.
 */
getparm(parm, min, label)
     char *parm, *label;
     int min;
{
    int val = atoi(parm);
    if (val < min) {
        printmsg("%s: Illegal %s parameter %d, must be at least %d",
		   progName, label, val, min);
	exit(1);
    }
    return val;
}

char writebuf[BUFLEN];
char readbuf[BUFLEN];
char testdir[MAXPATHLEN];

main(argc, argv) 
  int argc;
  char *argv[];
{
    char wd[MAXPATHLEN], dirname[MAXPATHLEN], filename[MAXFILES+FIRST];
    off_t offset;

    char *buf;
    int verbose = 0, flock = 0, pause = 1, timeout = 0;
    int i, num, numFiles = 5, fd[MAXFILES+FIRST], t1, t2;
    
    buf = &writebuf[0];
    for (i=0; i < BUFLEN; i++)
        *buf++ = 'A';

    strcpy(testdir,"/:");       /* default test dir */
    setbuf(stdout,(char *)NULL);

    if (getwd(wd) == 0) {
	printmsg("%s: getwd fails: %s\n", progName, wd);
	exit(1);
    }

    progName = *argv++;
    argc--;

    while (argc) {
        if (EQ(*argv, "-files")) {
	        numFiles = getparm(*++argv, 1, "files");
		if (numFiles >= MAXFILES) {
		    printmsg("%s: The number of files, %d, to create exceed the the hard limit %d\n", progName, numFiles, MAXFILES);
		    usage();
		    exit(1);
		 }
		argc--;
	}
	else 
	    if (EQ(*argv, "-nopause")) {
	        pause = 0;
	    }
	else 
	    if (EQ(*argv, "-verbose")) {
	        verbose = 1;
	    }
	else 
	    if (EQ(*argv, "-flock")) {
	        flock = 1;
	     }
	else 
	    if (EQ(*argv, "-dir")) {
	        strcpy(testdir, *++argv);
		if (strlen(testdir) == 0 || testdir[0] == '-') {
		    printmsg("%s: specify the test dir \n", progName);
                    usage();
                    exit(1);
		}
		argc--;
	     }
	else {
	    usage();
	    exit(1);
	}
	argc--;
	argv++;
	    
    } /* while loop */
		
    printmsg("%s: TSRTEST starting from %s ....\n\n", progName, wd);
 
    errno = 0;

    if (chdir(testdir) < 0) {
        printmsg("%s: can't chdir to directory %s, Error=%d", 
		   progName, testdir, errno);
	exit(1);
    }
	
    if (getwd(wd) == 0) {
	printmsg("%s: getwd fails: %s\n", progName, wd);
	exit(1);
     }
    if (verbose)
        printf("%s: cd to directory %s \n\n", progName, wd);
    
    /*
     * Start creating dirs and files under each dir just created.
     */
    if (access("./tsrDir", F_OK|X_OK) < 0) {
        if (errno == ENOENT) {
	    if (verbose)
	        printf("%s: directory %s does not exits, create it\n", 
		       progName, "./tsrDir");
	    if (mkdir("./tsrDir", 0755) < 0) {
	        printmsg("%s: can't create test directory %s, Error=%d\n", 
			   progName, "tsrDir", errno);
		exit(1);
	    }
	}
    }

    if (chdir("./tsrDir") < 0) {
        printmsg("%s: can't chdir to test directory %s, Error=%d", 
		   progName, "tsrDir", errno);
	exit(1);
    }
    if (verbose)
        printf("%s: cd to directory %s \n", progName, "./tsrDir");


    for (i = FIRST; i < numFiles+FIRST; i++) {

        sprintf(filename, "tsrfile%d", i);
        if ((fd[i] = open(filename, O_RDWR|O_CREAT, 0755)) <= 0) {
	   printmsg("%s: can't create test files %s, error=%d \n", 
		      progName, filename, errno);
	   exit(1);
	}
	if (verbose)
	   printf("\n%s: open file (fd=%d) %s OK...\n", 
		  progName, fd[i], filename);

	/* 
	 * Start WRITE works 
	 */
        offset = i*BUFLEN;

        if (lseek(fd[i], offset, SEEK_SET) < 0) {
	    printmsg("%s: lseek FAILS, error=%d\n",  
		       progName, errno);
	    exit(1);
	}
        if (verbose)
            printf("%s: lseek offset=%d on file %s OK...\n",
                   progName, offset, filename);
	
	if (flock) {
	    struct flock tlock;
	    /* 
	     * Request for flock lock 
	     */
	    tlock.l_type =  F_WRLCK;
	    tlock.l_whence = 0;
	    tlock.l_start = offset;
	    tlock.l_len = BUFLEN;
	    tlock.l_pid = 1234; /* try to confusing things */

	    if (fcntl(fd[i], F_SETLK, &tlock) < 0) {
	        printmsg("%s: fcntl f_SETLK FAILS, error = %d\n", 
			   progName, errno);
		exit(1);
	    }
	    if (verbose)
	        printf("%s: lockf file %s OK...\n", progName, filename);

	}
	if (num = write(fd[i], writebuf, BUFLEN) != BUFLEN) {
	    printmsg("%s: write FAIL on file %s \n", 
		       progName, filename);
	    exit(1);
	}	  
	if (verbose)
	    printf("%s: write to file %s at OK...\n", progName, filename);

    } /* for */

    t1 = i - 1;   /* last file written */
    t2 = i - 2;

retry:


    timeout = 0;
    
    offset = t1*BUFLEN;
    if (lseek(fd[t1], offset, SEEK_SET) < 0) {
        printmsg("%s: lseek FAILS, error=%d\n",  
		 progName, errno);
    } 

    sprintf(filename, "tsrfile%d", t1);
    if (num = write(fd[t1], writebuf, BUFLEN) != BUFLEN) {
       printmsg("%s: write FAIL on file %s, errno = %d \n", 
		progName, filename, errno);
       if ((errno == ETIMEDOUT)|| (errno == EAGAIN)) 
	    printmsg("%s: communication failure \n", progName);
   }	  
    offset = t2*BUFLEN;
    if (lseek(fd[t2], offset, SEEK_SET) < 0) {
        printmsg("%s: lseek FAILS, error=%d\n",  
		 progName, errno);
    }

    sprintf(filename, "tsrfile%d", t2);
    if (num = read(fd[t2], readbuf, BUFLEN) != BUFLEN) {
       printmsg("%s: read FAIL on file %s, errno = %d \n", 
		progName, filename, errno);
       if ((errno == ETIMEDOUT)|| (errno == EAGAIN)) 
	    printmsg("%s: communication failure \n", progName);
    }	  

    if (pause) {
        printf("\n\n\n\n%s:  ....... PAUSE .......  \n", progName);
	printf("%s: Type any char to resume!", progName);
	num = getchar();
	printf("\n\n\n\n");
    }
    /* 
     * Day After pause ...
     */

    /* 
     * Determine whether the network is down ...
     */
    errno = 0;
    sprintf(filename, "tsrfile%d", i);
    if ((fd[i] = open(filename, O_RDWR|O_CREAT, 0755)) <= 0) {
        printmsg("%s: can't create test files %s, error=%d\n",
		  progName, filename, errno);
        if ((errno == ETIMEDOUT)|| (errno == EAGAIN)) {
	    printmsg("%s: communication failure \n", progName);
	    timeout = 1;
	    goto  retry;
	}
	else 
	   exit(1);
    } else 
        if (verbose)
	    printf("\n%s: open file (fd=%d) %s and network is OK...\n\n\n\n",
		   progName, fd[i], filename);

    

    for (i = FIRST; i < numFiles+FIRST; i++) {

	/* 
	 * Start READ works 
	 */
        offset = i*BUFLEN;
	errno = 0;
        sprintf(filename, "tsrfile%d", i);

        if (lseek(fd[i], offset, SEEK_SET) < 0) {
	    printmsg("%s: lseek FAILS on file %s, error=%d\n",  
		       progName, filename, errno);
	    if ((errno == ETIMEDOUT)|| (errno == EAGAIN)) {
		printmsg("%s: communication failure \n", progName);
	    }
	} else {
	    if (verbose)
	        printf("%s: lseek offset=%d on file %s OK...\n", 
		       progName, offset, filename);
	}

	if (flock) {
	    struct flock tlock;
	    /* 
	     * Request for flock lock 
	     */
	    tlock.l_type =  F_RDLCK;
	    tlock.l_whence = 0;
	    tlock.l_start = offset;
	    tlock.l_len = BUFLEN;
	    tlock.l_pid = 1234; /* try to confusing things */

	    if (fcntl(fd[i], F_SETLK, &tlock) < 0) {
	        printmsg("%s: fcntl f_SETLK FAILS on file %s, error = %d\n", 
			   progName, filename, errno);
	    } else 
	       if (verbose)
		   printf("%s: RDLCK file %s OK...\n", progName, filename);

	}
	errno = 0;
	if (num = read(fd[i], writebuf, BUFLEN) != BUFLEN) {
	    printmsg("%s: read FAIL on file (fd=%d) %s, bytes Read = %d \n", 
		       progName, fd[i], filename, num);
	    if ((errno == ETIMEDOUT)|| (errno == EAGAIN)) {
		printmsg("%s: communication failure errno = %d\n", 
			 progName, errno);
	    } else 
	       if (errno == ESTALE) {
		   printmsg("%s: Error: file %s is ESTALE\n", 
			    progName, filename);
	       } else 
		   printmsg("%s: Error = %d \n", progName, filename);
	} else 
	   if (verbose)
	       printf("%s: read file (fd=%d) %s OK...\n", 
		      progName, fd[i], filename);
	
	if (flock) {
	    struct flock tlock;
	    /* 
	     * Request for flock lock 
	     */
	    tlock.l_type =  F_UNLCK;
	    tlock.l_whence = 0;
	    tlock.l_start = offset;
	    tlock.l_len = BUFLEN;
	    tlock.l_pid = 1234; /* try to confusing things */

	    if (fcntl(fd[i], F_SETLK, &tlock) < 0) {
	        printmsg("%s: fcntl F_UNLCK FAILS on file %s, error = %d\n", 
			   progName, filename, errno);
	    } else 
	       if (verbose)
		   printf("%s: F_UNLCK file %s OK...\n", progName, filename);

	}
	errno = 0;
	if (close(fd[i]) < 0) {
	    printmsg("%s: close FAIL on file (fd=%d) %s, error=%d\n", 
		       progName, fd[i], filename, errno);
	} else 
	  if (verbose)
	      printf("%s: close file (fd=%d) %s OK...\n\n", 
		     progName, fd[i], filename);

    } /* for */

    strcpy(dirname,wd) ;
    strcat(dirname, "/tsrDir");
    if (chdir(dirname) < 0) {
        printmsg("%s: can't chdir to test directory %s, Error=%d",
		   progName, dirname, errno);
        exit(1);
    }
    if (chdir(wd) < 0) {
        printmsg("%s: can't chdir to directory %s, Error=%d",
                   progName, wd, errno);
        exit(1);
    } 
    else 
        if (verbose)
	  printf("\n%s: cd to directory %s OK...\n\n", progName, wd);

    if (rename("tsrDir", "tsrDir1") < 0) {
        /* Ignore it */
    } else 
    if (verbose)
        printf("\n%s: rename directory from %s to %s OK...\n\n", 
	       progName, "tsrDir", "tsrDir1");
    
    printf("\n\n%s: Test Complete \n\n", progName);
	
    exit(0);
}
