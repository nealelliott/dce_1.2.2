/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: chesslib.c,v $
 * Revision 1.1.2.1  1996/08/09  21:13:43  arvind
 * 	DCE 1.2.2 Intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  14:11:00  arvind]
 *
 * $EndLog$
 */

/* testlib.c - test expectlib */

#include <stdio.h>
#include "expect.h"

timedout()
{
	fprintf(stderr,"timed out\n");
	exit(-1);
}

char move[100];

read_first_move(fd)
int fd;
{
	if (EXP_TIMEOUT == exp_expectl(fd,
			exp_glob,"first\r\n1.*\r\n",0,
			exp_end)) {
		timedout();
	}
	sscanf(exp_match,"%*s 1. %s",move);
}

/* moves and counter-moves are printed out in different formats, sigh... */

read_counter_move(fd)
int fd;
{
	switch (exp_expectl(fd,exp_glob,"*...*\r\n",0,exp_end)) {
	case EXP_TIMEOUT: timedout();
	case EXP_EOF: exit(-1);
	}

	sscanf(exp_match,"%*s %*s %*s %*s ... %s",move);
}

read_move(fd)
int fd;
{
	switch (exp_expectl(fd,exp_glob,"*...*\r\n*.*\r\n",0,exp_end)) {
	case EXP_TIMEOUT: timedout();
	case EXP_EOF: exit(-1);
	}

	sscanf(exp_match,"%*s %*s ... %*s %*s %s",move);
}

send_move(fd)
int fd;
{
	write(fd,move,strlen(move));
}

main(){
	int fd1, fd2;

	exp_loguser = 1;
	exp_timeout = 3600;

	fd1 = exp_spawnl("chess","chess",(char *)0);

	if (-1 == exp_expectl(fd1,exp_glob,"Chess\r\n",0,exp_end)) exit;

	if (-1 == write(fd1,"first\r",6)) exit;

	read_first_move(fd1);

	fd2 = exp_spawnl("chess","chess",(char *)0);

	if (-1 == exp_expectl(fd2,exp_glob,"Chess\r\n",0,exp_end)) exit;

	for (;;) {
		send_move(fd2);
		read_counter_move(fd2);

		send_move(fd1);
		read_move(fd1);
	}
}
