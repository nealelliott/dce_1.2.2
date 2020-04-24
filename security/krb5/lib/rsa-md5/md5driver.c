/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: md5driver.c,v $
 * Revision 1.1.5.2  1996/02/18  00:16:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:26  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:45:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:01  root]
 * 
 * Revision 1.1.3.3  1992/12/29  16:33:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:20:53  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:19:53  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:35:41  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:52:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  md5driver.c V=1 07/08/91 //littl/prgy/krb5/lib/rsa-md5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 ***********************************************************************
 ** md5driver.c -- sample test routines                               **
 ** RSA Data Security, Inc. MD5 Message-Digest Algorithm              **
 ** Created: 2/16/90 RLR                                              **
 ** Updated: 1/91 SRD                                                 **
 ***********************************************************************
 */

/*
 ***********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved.  **
 **                                                                   **
 ** RSA Data Security, Inc. makes no representations concerning       **
 ** either the merchantability of this software or the suitability    **
 ** of this software for any particular purpose.  It is provided "as  **
 ** is" without express or implied warranty of any kind.              **
 **                                                                   **
 ** These notices must be retained in any copies of any part of this  **
 ** documentation and/or software.                                    **
 ***********************************************************************
 */

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "md5.h"

/* Prints message digest buffer in mdContext as 32 hexadecimal digits.
   Order is from low-order byte to high-order byte of digest.
   Each byte is printed with high-order hexadecimal digit first.
 */
static void MDPrint (mdContext)
MD5_CTX *mdContext;
{
  int i;

  for (i = 0; i < 16; i++)
    printf ("%02x", mdContext->digest[i]);
}

/* size of test block */
#define TEST_BLOCK_SIZE 1000

/* number of blocks to process */
#define TEST_BLOCKS 10000

/* number of test bytes = TEST_BLOCK_SIZE * TEST_BLOCKS */
static long TEST_BYTES = (long)TEST_BLOCK_SIZE * (long)TEST_BLOCKS;

/* A time trial routine, to measure the speed of MD5.
   Measures wall time required to digest TEST_BLOCKS * TEST_BLOCK_SIZE
   characters.
 */
static void MDTimeTrial ()
{
  MD5_CTX mdContext;
  unsigned char data[TEST_BLOCK_SIZE];
  unsigned int i;
  double t;
  struct timeval stv, etv, dtv;
  
  /* initialize test data */
  for (i = 0; i < TEST_BLOCK_SIZE; i++)
    data[i] = (unsigned char)(i & 0xFF);

  /* start timer */
  printf ("MD5 time trial. Processing %ld characters...\n", TEST_BYTES);
#ifdef SNI_SVR4_POSIX
  gettimeofday(&stv);
#else
  gettimeofday(&stv, 0);
#endif /* SNI_SVR4_POSIX */

  /* digest data in TEST_BLOCK_SIZE byte blocks */
  MD5Init (&mdContext);
  for (i = TEST_BLOCKS; i > 0; i--)
    MD5Update (&mdContext, data, TEST_BLOCK_SIZE);
  MD5Final (&mdContext);

  /* stop timer, get time difference */
#ifdef SNI_SVR4_POSIX
  gettimeofday(&etv);
#else
  gettimeofday(&etv, 0);
#endif /* SNI_SVR4_POSIX */
  MDPrint (&mdContext);
  printf (" is digest of test input.\n");
  
  dtv.tv_usec = etv.tv_usec - stv.tv_usec;
  dtv.tv_sec = etv.tv_sec - stv.tv_sec;
  while (dtv.tv_usec < 0) {
      dtv.tv_usec += 1000000;
      dtv.tv_sec -= 1;
  }
  t = dtv.tv_sec + ((double)dtv.tv_usec)/1000000.0;
  
  printf
    ("Seconds to process test input: %ld.%03ld\n", dtv.tv_sec, dtv.tv_usec/1000);
  printf
    ("Characters processed per second: %ld\n",
     (long)(TEST_BYTES/t));
}

/* Computes the message digest for string inString.
   Prints out message digest, a space, the string (in quotes) and a
   carriage return.
 */
static void MDString (inString)
char *inString;
{
  MD5_CTX mdContext;
  unsigned int len = strlen (inString);

  MD5Init (&mdContext);
  MD5Update (&mdContext, inString, len);
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  printf (" \"%s\"\n", inString);
}

/* Computes the message digest for a specified file.
   Prints out message digest, a space, the file name, and a carriage
   return.
 */
static void MDFile (filename)
char *filename;
{
  FILE *inFile = fopen (filename, "rb");
  MD5_CTX mdContext;
  int bytes;
  unsigned char data[1024];

  if (inFile == NULL) {
    printf ("%s can't be opened.\n", filename);
    return;
  }

  MD5Init (&mdContext);
  while ((bytes = fread (data, 1, 1024, inFile)) != 0)
    MD5Update (&mdContext, data, bytes);
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  printf (" %s\n", filename);
  fclose (inFile);
}

/* Writes the message digest of the data from stdin onto stdout,
   followed by a carriage return.
 */
static void MDFilter ()
{
  MD5_CTX mdContext;
  int bytes;
  unsigned char data[16];

  MD5Init (&mdContext);
  while ((bytes = fread (data, 1, 16, stdin)) > 0)
    MD5Update (&mdContext, data, bytes);
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  printf ("\n");
}

/* Runs a standard suite of test data.
 */
static void MDTestSuite ()
{
  printf ("MD5 test suite results:\n");
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
  ** -t                -- prints time trial statistics for 10M
                          characters
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
 ***********************************************************************
 ** End of md5driver.c                                                **
 ******************************** (cut) ********************************
 */
