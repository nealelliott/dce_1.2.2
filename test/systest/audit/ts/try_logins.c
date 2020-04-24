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
 * $Log: try_logins.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:54:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:00  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:47:19  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:18:14  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/18  14:32:32  bhaim
 * 	Initial drop
 * 	[1994/03/18  14:29:48  bhaim]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define SHMKEY1 ((key_t)7890) /* shared mem1 key */
#define SHMKEY2 ((key_t)7891) /* shared mem2 key */
#define IFLAGS  (IPC_CREAT|IPC_EXCL)
int *shmcount, *shmretval;
int count, retval;

main(argc,argv)
int argc;
char *argv[];
{
	
int i;
char prefix[80];
char tempstr[80];
char countstr[80];
int wt_st;
void do_something();
int shmid1, shmid2;

	if (argc < 3 ){
		fprintf(stderr, "Usage: try_logins prefix number\n");
		exit(1);
	}
	
	if (strcpy(prefix,argv[1]) == NULL){
		fprintf(stderr, "Could not strcpy prefix in try_logins\n");
		exit(1);
	}


	if (sscanf(argv[2],"%d",&count) != 1) {
		fprintf(stderr, "Could not obtain count in try_logins\n");
		exit(1);
	}

	if ((shmid1 = shmget(SHMKEY1, sizeof(int), 0666|IFLAGS)) < 0 ) {
		perror("1st shmget");
		fprintf(stderr, "Could not get first shared memory segment id\n");
		exit(1);
	}


	if ((shmid2 = shmget(SHMKEY2, sizeof(int), 0666|IFLAGS)) < 0 ) {
		perror("2nd shmget");
		fprintf(stderr, "Could not get second shared memory segment id\n");
		exit(1);
	}

	if ((shmcount = (int *) shmat(shmid1, 0, 0)) == (int *) -1 ) {
		fprintf(stderr, "Could not attach first shared memory segment\n");
		exit(1);
	}

	if ((shmretval = (int *) shmat(shmid2, 0, 0)) == (int *) -1 ) {
		fprintf(stderr, "Could not attach second shared memory segment\n");
		exit(1);
	}

	*shmcount = 0;
        *shmretval = 0;

	for (i =1 ; i <= count; i++ )
	{
		memset(tempstr,'\0',sizeof(tempstr));
		strcpy(tempstr,prefix);
		sprintf(countstr,"%d",i);
		strcat(tempstr,countstr);
      		switch(fork())
		{         
      			case -1 :                         /* bad news.  scream and die */
				fprintf(stderr, "Could not fork in try_logins\n");
        			exit(1);
      			case 0 :                          /* we're the child, do something */
        			do_something(tempstr);
        			exit(0);
      			default :                         /* we're the parent so look for */
        			continue;
      		}
	}
	while ((wait(&wt_st) > 0) || (errno != ECHILD)) ;

	retval = *shmretval;

	if (shmdt(shmcount) < 0 ) {
		fprintf(stderr, "Could not detach first shared memory segment\n");
		exit(1);
	}

	if (shmdt(shmretval) < 0 ) {
		fprintf(stderr, "Could not detach second shared memory segment\n");
		exit(1);
	}

	if (shmctl(shmid1, IPC_RMID, (struct shmid_ds *)0) < 0 ) {
		fprintf(stderr, "Could not remove first shared memory segment id\n");
		exit(1);
	}

	if (shmctl(shmid2, IPC_RMID, (struct shmid_ds *)0) < 0 ) {
		fprintf(stderr, "Could not remove second shared memory segment id\n");
		exit(1);
	}

	exit(retval);
}

void do_something(tempstr)
char *tempstr;
{

error_status_t st;

		*shmcount = *shmcount + 1;
		while (*shmcount < count ) ;
		tst_dce_login(tempstr,"-dce-",&st);
		if (st != error_status_ok)
		{
			fprintf(stderr,"Try login failed: %s\n",tempstr);
			*shmretval = 1;
		}
}
