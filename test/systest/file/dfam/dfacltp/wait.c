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
 * $Log: wait.c,v $
 * Revision 1.1.2.2  1996/03/11  14:11:16  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:28  marty]
 *
 * Revision 1.1.2.1  1996/02/29  04:31:36  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  04:30:24  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:51:27  takano
 * 	First Submit
 * 	[1996/02/22  10:51:11  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

/*****************************************************************************/
/*  ID     = main                                                            */
/*  name   = main routine                                                    */
/*  func   = (1)Checks the number of arguments.                              */
/*           (2)Checks the contents of arguments.                            */
/*           (3)Waits for the specified period.                              */
/*  io     = argc:int    :IN : argument count                                */
/*           argv:char **:IN : argument vector                               */
/*  return = none                                                            */
/*  common =                                                                 */
/*  note   =                                                                 */
/*  date   =                                                                 */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
void main(int argc, char *argv[])
{
	time_t	start, current, passage, wait_time;
	char	*pnumber;

	if(argc != 2) {
		printf("Syntax error\n");
		exit(0);
	}

	pnumber = argv[1];
	while(*pnumber != '\0') {
		if(!isdigit((int)*pnumber)) {
			printf("Syntax error\n");
			exit(0);
		}
		pnumber++;
	}
	
	wait_time = (time_t)atol(argv[1]);	

	start = time(NULL);
	
	while(1) {
		current = time(NULL);
		passage = current - start;
		if(passage > wait_time) exit(0);
	} 	
}
