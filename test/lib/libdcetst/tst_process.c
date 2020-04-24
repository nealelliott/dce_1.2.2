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
 * $Log: tst_process.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:28  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:54:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:42  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:44:49  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:29  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/10  22:38:22  bhaim
 * 	Common TET functions
 * 	[1994/03/10  22:36:30  bhaim]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <sys/types.h>

/* the initialization of these depends on your system */
static char pspart[] = "ps -eo pid,comm";
static char fmt[]    = "%d  %s";
#define LINESZ 80


/*
 *  ROUTINE NAME:       tst_chk_command
 *
 *  SCOPE:              PUBLIC (intended for use by DME system tests)
 *
 *  DESCRIPTION:
 *  tst_chk_command checks whether a given command is in the process table
 *  0 is returned if the command is found in the process table, 1 if the command
 *  is not found in the process table.
 *  If the command is found, the value pid is returned also.
 */


int
tst_chk_command(char *command, pid_t *pid)
{

char comline[LINESZ], inbuf[LINESZ], header[LINESZ];
char name[LINESZ];
FILE *f, *popen();


	strcpy(comline, pspart);

	/* start ps command */
	if ((f = popen(comline, "r")) == NULL){
		fprintf(stderr, "could not run ps program\n");
		return(2);
	}

	/* get and ignore first line from ps */
	if ( fgets(header, LINESZ, f) == NULL){
		fprintf(stderr, "no output from ps?\n");
		return(3);
	}

	/* look for programs to kill */
	while(fgets(inbuf, LINESZ, f) != NULL) {

		if( sscanf(inbuf, fmt, pid, name) < 2)
			break;


		if (!strcmp(name,command))
		{
			return(0);
		}
	}
	
	
	*pid = (pid_t) 0;
	return(1);
}


/*
 *  ROUTINE NAME:       tst_chk_process
 *
 *  SCOPE:              PUBLIC (intended for use by DME system tests)
 *
 *  DESCRIPTION:
 *  tst_chk_command checks whether a given pid is in the process table
 *  0 is returned if the pid is found in the process table, 1 if the pid
 *  is not found in the process table.
 */


int tst_chk_process(pid_t *pid)
{

char comline[LINESZ], inbuf[LINESZ], header[LINESZ];
char name[LINESZ];
FILE *f, *popen();
pid_t  tmppid;


	strcpy(comline, pspart);

	/* start ps command */
	if ((f = popen(comline, "r")) == NULL){
		fprintf(stderr, "could not run ps program\n");
		return(2);
	}

	/* get and ignore first line from ps */
	if ( fgets(header, LINESZ, f) == NULL){
		fprintf(stderr, "no output from ps?\n");
		return(3);
	}

	/* look for programs to kill */
	while(fgets(inbuf, LINESZ, f) != NULL) {

		if( sscanf(inbuf, fmt, &tmppid, name) < 2)
			break;


		if (tmppid == *pid)
		{
			return(0);
		}
	}
	
	return(1);
}
