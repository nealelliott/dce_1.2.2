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
 * $Log: shell.c,v $
 * Revision 1.1.2.2  1996/03/11  14:11:14  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:27  marty]
 *
 * Revision 1.1.2.1  1996/02/29  04:28:33  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  04:28:21  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:50:52  takano
 * 	First Submit
 * 	[1996/02/22  10:50:42  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <io.h>

/*****************************************************************************/
/*  ID     = main                                                            */
/*  name   = main routine                                                    */
/*  func   = (1)Checks the number of arguments.                              */
/*           (2)Binds stdout file handle with stderr file handle.            */
/*           (3)Frees stdout and stderr buffers.                             */
/*           (4)Executes the character string as an operating-system command.*/
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
	int		rc_dup2;
	int		rc_setvbuf;
	int		rc_system;
	int		errno_save;

	if (argc != 2) {
		printf("Syntax error\n");
		exit(0);
	}

	rc_dup2 = _dup2(_fileno(stdout), _fileno(stderr));
	if (rc_dup2 != 0) {
		printf("I/O error\n");
		exit(0);
	}

	rc_setvbuf = setvbuf(stdout, NULL, _IONBF, 0);
	if (rc_setvbuf != 0) {
		printf("I/O error\n");
		exit(0);
	}

	rc_setvbuf = setvbuf(stderr, NULL, _IONBF, 0);
	if (rc_setvbuf != 0) {
		printf("I/O error\n");
		exit(0);
	}

	rc_system = system(argv[1]);
	errno_save = errno;

	if (rc_system != 0) {
		printf("Invalid command\n");
	}
}
