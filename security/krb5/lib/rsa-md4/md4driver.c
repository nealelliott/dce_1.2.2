/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: md4driver.c,v $
 * Revision 1.1.4.2  1996/02/18  00:16:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:56  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:32:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:20:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  md4driver.c V=2 10/24/91 //littl/prgy/krb5/lib/rsa-md4
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *	$Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/rsa-md4/md4driver.c,v $
 *	$Author: marty $
 *	$Id: md4driver.c,v 1.1.4.2 1996/02/18 00:16:10 marty Exp $
 */

#ifndef lint
static char rcsid_md4driver_c[] = "$Id: md4driver.c,v 1.1.4.2 1996/02/18 00:16:10 marty Exp $";
#endif /* lint */

/*
 **********************************************************************
 ** md4driver.c -- sample routines to test                           **
 ** RSA Data Security, Inc. MD4 message digest algorithm.            **
 ** Created: 2/16/90 RLR                                             **
 ** Updated: 1/91 SRD                                                **
 **********************************************************************
 */

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <krb5/krb5.h>
#include <krb5/rsa-md4.h>

/* Prints message digest buffer in mdContext as 32 hexadecimal digits.
   Order is from low-order byte to high-order byte of digest.
   Each byte is printed with high-order hexadecimal digit first.
 */
static void MDPrint (mdContext)
MD4_CTX *mdContext;
{
  int i;

  for (i = 0; i < 16; i++)
    printf ("%02x", mdContext->digest[i]);
}

/* size of test block */
#define TEST_BLOCK_SIZE 1000

/* number of blocks to process */
#define TEST_BLOCKS 2000

/* number of test bytes = TEST_BLOCK_SIZE * TEST_BLOCKS */
static long TEST_BYTES = (long)TEST_BLOCK_SIZE * (long)TEST_BLOCKS;

/* A time trial routine, to measure the speed of MD4.
   Measures wall time required to digest TEST_BLOCKS * TEST_BLOCK_SIZE
   characters.
 */
static void MDTimeTrial ()
{
  MD4_CTX mdContext;
  time_t endTime, startTime;
  unsigned char data[TEST_BLOCK_SIZE];
  unsigned int i;

  /* initialize test data */
  for (i = 0; i < TEST_BLOCK_SIZE; i++)
    data[i] = (unsigned char)(i & 0xFF);

  /* start timer */
  printf ("MD4 time trial. Processing %ld characters...\n", TEST_BYTES);
  time (&startTime);

  /* digest data in TEST_BLOCK_SIZE byte blocks */
  MD4Init (&mdContext);
  for (i = TEST_BLOCKS; i > 0; i--)
    MD4Update (&mdContext, data, TEST_BLOCK_SIZE);
  MD4Final (&mdContext);
  /* stop timer, get time difference */
  time (&endTime);
  MDPrint (&mdContext);
  printf (" is digest of test input.\n");
  printf
    ("Seconds to process test input: %ld\n", (long)(endTime-startTime));
  printf
    ("Characters processed per second: %ld\n",
     TEST_BYTES/(endTime-startTime));
}

/* Computes the message digest for string inString.
   Prints out message digest, a space, the string (in quotes) and a
   carriage return.
 */
static void MDString (inString)
char *inString;
{
  MD4_CTX mdContext;
  unsigned int len = strlen (inString);

  MD4Init (&mdContext);
  MD4Update (&mdContext, inString, len);
  MD4Final (&mdContext);
  MDPrint (&mdContext);
  printf (" \"%s\"\n\n", inString);
}

/* Computes the message digest for a specified file.
   Prints out message digest, a space, the file name, and a carriage
   return.
 */
static void MDFile (filename)
char *filename;
{
#ifdef __STDC__
  FILE *inFile = fopen (filename, "rb");
#else
  FILE *inFile = fopen (filename, "r");
#endif
  MD4_CTX mdContext;
  int bytes;
  unsigned char data[1024];

  if (inFile == NULL) {
    printf ("%s can't be opened.\n", filename);
    return;
  }

  MD4Init (&mdContext);
  while ((bytes = fread (data, 1, 1024, inFile)) != 0)
    MD4Update (&mdContext, data, bytes);
  MD4Final (&mdContext);
  MDPrint (&mdContext);
  printf (" %s\n", filename);
  fclose (inFile);
}


/* Writes the message digest of the data from stdin onto stdout,
   followed by a carriage return.
 */
static void MDFilter ()
{
  MD4_CTX mdContext;
  int bytes;
  unsigned char data[16];

  MD4Init (&mdContext);
  while ((bytes = fread (data, 1, 16, stdin)) != 0)
    MD4Update (&mdContext, data, bytes);
  MD4Final (&mdContext);
  MDPrint (&mdContext);
  printf ("\n");
}

/* Runs a standard suite of test data.
 */
static void MDTestSuite ()
{
  printf ("MD4 test suite results:\n\n");
  MDString ("");
  MDString ("a");
  MDString ("abc");
  MDString ("message digest");
  MDString ("abcdefghijklmnopqrstuvwxyz");
  MDString
    ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  MDString
    ("1234567890123456789012345678901234567890\
1234567890123456789012345678901234567890");
  /* Contents of file foo are "abc" */
  MDFile ("foo");
}

void main (argc, argv)
int argc;
char *argv[];
{
  int i;

  /* For each command line argument in turn:
  ** filename          -- prints message digest and name of file
  ** -sstring          -- prints message digest and contents of string
  ** -t                -- prints time trial statistics for 1M characters
  ** -x                -- execute a standard suite of test data
  ** (no args)         -- writes messages digest of stdin onto stdout
  */
  if (argc == 1)
    MDFilter ();
  else
    for (i = 1; i < argc; i++)
      if (argv[i][0] == '-' && argv[i][1] == 's')
        MDString (argv[i] + 2);
      else if (strcmp (argv[i], "-t") == 0)
        MDTimeTrial ();
      else if (strcmp (argv[i], "-x") == 0)
        MDTestSuite ();
      else MDFile (argv[i]);
}

/*
 **********************************************************************
 ** End of md4driver.c                                               **
 ******************************* (cut) ********************************
 */
