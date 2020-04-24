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
 * $Log: exp_test.c,v $
 * Revision 1.1.2.1  1996/08/09  19:17:30  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  19:01:04  arvind]
 *
 * $EndLog$
 */

/*
 * exp-test -- this is a simple C program to test the interactive functions of expect.
 */

#include <stdio.h>
#include <string.h>

#define ARRAYSIZE 128

main (argc, argv)
int argc;
char *argv[];
{
  char line[ARRAYSIZE];

  do {
    memset (line, 0, ARRAYSIZE);
    fgets (line, ARRAYSIZE, stdin);
    *(line + strlen(line)-1) = '\0'; /* get rid of the newline */

    /* look for a few simple commands */
    if (strncmp (line,"prompt ", 6) == 0) {
      printf ("%s (y or n) ?", line + 6);
      if (getchar() == 'y')
	puts ("YES");
      else
	puts ("NO");
    }
    if (strncmp (line, "print ", 6) == 0) {
      puts (line + 6);
    }
  } while (strncmp (line, "quit", 4));
}
