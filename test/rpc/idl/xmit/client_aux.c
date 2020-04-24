/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.8.2  1996/02/17  23:18:23  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:18  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:31:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:45  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:09:06  sommerfeld
 * 	Add #include of test_common.h
 * 	[1993/06/14  21:08:17  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  23:40:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:46  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <xmit.h>
#include <stdio.h>
#include <dce/nbase.h>
#include <test_common.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    int failures = 0;
    long val1 = random();
    long val2 = random();
    long val3 = random();
    struct_t s1, s2, s3, sret;
    long ret;

    ret = lal(h, &val1, &val2, val3, &val2);
    if (ret != val3) {   
       failures++;
       printf("***FAILURE***: lal returned %d, expected %d\n",ret,val3);
       }
    if (val1 != val2) {   
       failures++;
       printf("***FAILURE***: lal val1(%d) != val2(%d)\n",val1,val2);
       }

#if 0 /* This test will not be supported in V2 */
    val1 = 10;
    val2 = 20;
    val3 = 30;
    ret = las(h, &val1, &val2, val3);
    if (ret != val3) {   
       failures++;
       printf("***FAILURE***: las returned %d, expected %d\n",ret,val3);
       }
    if (val1 != val2) {   
       failures++;
       printf("***FAILURE***: las val1(%d) != val2(%d)\n",val1,val2);
       }
#endif

    val1 = 10;
    val2 = 20;
    val3 = 30;
    s1.val = &val1;
    s2.val = &val2;
    s3.val = &val3;
    s1.next = s2.next = s3.next = NULL;
    sret = sal(h, &s1, &s2, s3);
    if (*sret.val != *s3.val) {   
       failures++;
       printf("***FAILURE***: las returned %d, expected %d\n",*sret.val,*s3.val);
       }
    if (*s1.val != *s2.val) {   
       failures++;
       printf("***FAILURE***: las val1(%d) != val2(%d)\n",*s1.val,*s2.val);
       }
      

    return failures;
}
