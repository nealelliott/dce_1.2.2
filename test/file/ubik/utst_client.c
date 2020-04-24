/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utst_client.c,v $
 * Revision 1.1.313.1  1996/10/17  18:37:11  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:58:14  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1995 Transarc Corporation
 * All rights reserved.
 */

/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <stdio.h>
#include "utst_proc.h"

/* main program */
main(argc, argv)     
    int argc;
    char **argv; {
    register long code;
    struct ubik_client *cstruct;
    struct sockaddr_in serverList[UBIK_MAXSERVERS];
    long serverCount;
    unsigned_char_t *nsGroupP;
    register long i;
    long temp;
    
    /* Force unbuffered output so that less output is lost */
    (void) setbuf(stdout, (char *)NULL);
    if (argc == 1) {
	printf("utst_client: usage is utst_client -nsgroup <CDS server group> [-try] [-get] [-inc] [-trunc] [-mget]\n");
	exit(0);
    }

    for (i=0;i<argc;i++)
      if (strncmp(argv[i], "-help", strlen(argv[i])) == 0 ||
	  strncmp(argv[i], "help", strlen(argv[i])) == 0) {
	printf("Usage: utst_client -nsgroup <CDS server group> [-try] [-get] [-inc] [-trunc] [-mget]\n");
	exit(0);
      }

    argv++;
    argc--;
    if ( (argc >=2) && (strcmp(*argv, "-nsgroup") == 0) )
    {
	argv++; argc--;
	nsGroupP = (unsigned char *)*argv;
	argv++; argc--;
    }
    else
    {
	printf("unrecognized argument, %s\n", *argv);
	exit(1);
    }

    code = ubik_ClientInit(SAMPLE_v4_0_c_ifspec, 
			   (unsigned_char_t *)nsGroupP, 
			   (unsigned_char_t *)"subsys/dce/dfs-utst-servers", 
			   0, 0, 1, 
			   (struct ubik_client **)&cstruct);
    /* check code from init */
    if (code) {
	printf("ubik client init failed with code %ld: %s\n",
	       code, dfs_dceErrTxt(code));
	exit(1);
    }

    /* parse command line for our own operations */
    for(i=0;i<argc;i++) {
	if (!strcmp(argv[i], "-inc")) {
	    /* use ubik_Call to do the work, finding an up server and handling
		the job of finding a sync site, if need be */
	    code = ubik_Call(SAMPLE_Inc, cstruct, 0);
	    if (code != 0)
		printf("inc: SAMPLE_Inc failed with code %ld: %s\n",
		       code, dfs_dceErrTxt(code));
	    else
		printf("inc: succeeded\n");
	}
	else if (!strcmp(argv[i], "-try")) {
	    code = ubik_Call(SAMPLE_Test, cstruct, 0);
	    if (code != 0)
		printf("try: SAMPLE_Test failed with code %ld: %s\n",
		       code, dfs_dceErrTxt(code));
	    else
		printf("try: succeeded\n");
	}
	else if (!strcmp(argv[i], "-qget")) {
	    code = ubik_Call(SAMPLE_QGet, cstruct, 0, &temp);
	    if (code != 0)
		printf("qget: SAMPLE_QGET failed with code %ld: %s\n",
		       code, dfs_dceErrTxt(code));
	    else
		printf("qget: got quick value %d)\n", temp);
	}
	else if (!strcmp(argv[i], "-get")) {
	    code = ubik_Call(SAMPLE_Get, cstruct, 0, &temp);
	    if (code != 0)
		printf("get: SAMPLE_Get failed with code %ld: %s\n",
		       code, dfs_dceErrTxt(code));
	    else
		printf("get: got value %d\n", temp);
	}
	else if (!strcmp(argv[i], "-trunc")) {
	    code = ubik_Call(SAMPLE_Trun, cstruct, 0);
	    if (code != 0)
		printf("trunc: SAMPLE_Trun failed with code %ld: %s\n",
		       code, dfs_dceErrTxt(code));
	    else
		printf("trunc: succeeded\n");
	}
	else if (!strcmp(argv[i], "-minc")) {
	    long temp;
	    struct timeval tv;

	    printf("ubik_client: Running minc...\n");
	    while (1) {
                temp=0;
		code = ubik_Call(SAMPLE_Get, cstruct, 0, &temp);
		if (code != 0)
		    printf("minc: SAMPLE_Get #1 failed with code %ld; %s\n",
			   code, dfs_dceErrTxt(code));
		else
		    printf("minc: SAMPLE_Get #1 succeeded, got value %d\n",
			   temp);

                temp=0;
		code = ubik_Call(SAMPLE_Inc, cstruct, 0);
		if (code != 0)
		    printf("minc: SAMPLE_Inc #1 failed with code %ld; %s\n",
			   code, dfs_dceErrTxt(code));
		else
		    printf("minc: SAMPLE_Inc #1 succeeded, incremented integer\n");

                temp=0;
		code = ubik_Call(SAMPLE_Get, cstruct, 0, &temp);
		if (code != 0)
		    printf("minc: SAMPLE_Get #2 failed with code %ld; %s\n",
			   code, dfs_dceErrTxt(code));
		else
		    printf("minc: SAMPLE_Get #2 succeeded value %d\n", temp);

                temp=0;
		code = ubik_Call(SAMPLE_Inc, cstruct, 0);
		if (code != 0)
		    printf("minc: SAMPLE_Inc #2 failed with code %ld; %s\n",
			   code, dfs_dceErrTxt(code));
		else
		    printf("minc: SAMPLE_Inc #2 succeeded, incremented integer\n");

		tv.tv_sec = 1;
		tv.tv_usec = 0;
		pthread_delay_np((struct timespec *)&tv);
		printf("Repeating the minc SAMPLE operations again...\n");
	    }
	}
	else if (!strcmp(argv[i], "-mget")) {
	    long temp;
	    struct timeval tv;

	    printf("ubik_client: Running mget...\n");
	    while (1) {
                temp=0;
		code = ubik_Call(SAMPLE_Get, cstruct, 0, &temp);
		if (code != 0)
		    printf("mget: SAMPLE_Get #1 failed with code %ld; %s\n",
			   code, dfs_dceErrTxt(code));
		else
		    printf("mget: SAMPLE_Get #1 succeeded value %d\n", temp);

                temp=0;
		code = ubik_Call(SAMPLE_Inc, cstruct, 0);
		if (code != 0)
		    printf("mget: SAMPLE_Inc failed with code %ld; %s\n",
			   code, dfs_dceErrTxt(code));
		else
		    printf("mget: SAMPLE_Inc succeeded, incremented integer\n");

                temp=0;
		code = ubik_Call(SAMPLE_Get, cstruct, 0, &temp);
		if (code != 0)
		    printf("mget: SAMPLE_Get #2 failed with code %ld; %s\n",
			   code, dfs_dceErrTxt(code));
		else
		    printf("mget: SAMPLE_Get #2 succeeded value %d\n", temp);

                temp=0;
		code = ubik_Call(SAMPLE_Get, cstruct, 0, &temp);
		if (code != 0)
		  printf("mget: SAMPLE_Get #3 failed with code %ld; %s\n",
			 code, dfs_dceErrTxt(code));
		else
		    printf("mget: SAMPLE_Get #3 succeeded value %d\n", temp);

		tv.tv_sec = 1;
		tv.tv_usec = 0;
		pthread_delay_np((struct timespec *)&tv);
		printf("Repeating the mget SAMPLE operations again...\n");
	    }
	}
	else if (!strcmp(argv[i], "-incf")) {
	    struct timeval tv;

	    i = 0;
	    printf("ubik_client: Running incf...\n");
	    while (1) {
		code = ubik_Call(SAMPLE_Inc, cstruct, 0);
		if (code != 0)
		    printf("incf: SAMPLE_Inc iter %ld failed with code %ld; %s\n",
			   i, code, dfs_dceErrTxt(code));
		else
		    printf("incf: SAMPLE_Inc iter %ld succeeded, incremented integer\n",
			   i);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		pthread_delay_np((struct timespec *)&tv);
		printf("Repeating the incf SAMPLE operations again...\n");
	    }
	}
	else
	{
	    printf("unrecognized argument, %s\n", *argv);
	    exit(1);
	}
    }

    exit(0);
}
