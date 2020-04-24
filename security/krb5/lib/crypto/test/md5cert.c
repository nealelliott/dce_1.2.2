/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: md5cert.c,v $
 * Revision 1.1.2.2  1996/03/09  20:45:40  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:25:06  marty]
 *
 * Revision 1.1.2.1  1995/12/08  17:50:43  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/08/21  17:36 UTC  sommerfeld
 * 	MD5 regression test.
 * 	[1995/08/17  21:50 UTC  sommerfeld  /main/sommerfeld_mothpatch/1]
 * 
 * $EndLog$
 */

/*
 * MD5 regression test, extracted from RFCxxxx and rewritten to be
 * stand-alone
 */

#include <stdio.h>
#include <krb5/base-defs.h>
#include <krb5/encryption.h>
#include <krb5/rsa-md5.h>
/* Prints a message digest in hexadecimal.
 */
int errors;

static void MDSPrint (unsigned char *digest, char *buf)
{
  unsigned int i;

  for (i = 0; i < 16; i++)
      sprintf (&buf[i*2],"%02x", digest[i]);
}


/* Digests a string and prints the result.
 */
static void MDString (char *string, char *correct)
{
  MD5_CTX context;
  char buf[40];
  unsigned int len = strlen (string);

  MD5Init (&context);
  MD5Update (&context, (unsigned char *)string, len);
  MD5Final (&context);

  printf ("MD5 (\"%s\") = ", string);
  MDSPrint (context.digest, buf);
  printf ("%s", buf);
  if (strcmp(correct, buf)) 
  {
      errors++;
      printf(" FAILED %s", correct);
  } else {
      printf(" OK");
  }
  printf ("\n");
}
/* Digests a reference suite of strings and prints the results.
 */
static void MDTestSuite ()
{
  printf ("MD5 test suite:\n");

  MDString ("",
	    "d41d8cd98f00b204e9800998ecf8427e");
  MDString ("a",
	    "0cc175b9c0f1b6a831c399e269772661");
  MDString ("abc",
	    "900150983cd24fb0d6963f7d28e17f72");
  MDString ("message digest",
	    "f96b697d7cb7938d525a2f31aaf161d0");
  MDString ("abcdefghijklmnopqrstuvwxyz",
	    "c3fcd3d76192e4007dfb496cca67e13b");
  MDString
      ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
       "d174ab98d277d9f5a5611c2c9f419d9f");
  MDString
 ("1234567890123456789012345678901234567890\
1234567890123456789012345678901234567890",
  "57edf4a22be3c955ac49da2e2107b67a");
}

main()
{
    errors = 0;
    
    MDTestSuite();

    if (errors != 0) 
    {
	printf("md5cert FAILED: %d errors\n", errors);
	return 1;
    } else {
	printf("md5cert PASSED: no errors\n");
	return 0;
    }
}
