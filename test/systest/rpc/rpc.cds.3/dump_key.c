/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump_key.c,v $
 * Revision 1.1.6.2  1996/02/17  23:27:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:49  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:42:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:19:38  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:49:25  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:14  devsrc]
 * 
 * Revision 1.1.2.2  1993/03/12  04:25:47  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:32:24  mhickey]
 * 
 * $EndLog$
 */


/*---*
 *---* History from before the name change
 *---*
 * Revision 1.1.3.3  1993/01/19  18:30:57  mhickey
 * 	Changed all of the Server strings to dump_key(pid).
 *
 * Revision 1.1.3.2  1993/01/14  16:03:44  mhickey
 * 	Mainline submit for dump_key routine
 * 
 * Revision 1.1.1.2  1993/01/07  15:42:29  mhickey
 * 	Initial checkin of routine to dump sec_passwd structs.
 * 
 *---*
 *---*/

/*---*
 *---* dump_key.c - dumps a security key for debugging
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the DCE 1.0.2 System Tests
 *---*
 *---*    The DCE 1.0.2 system tests corresponds to the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the DCE 1.0.2 system tests, see the test 
 *---*    plan.
 *---* 
 *---* FUNCTION
 *---*    Prints the contents of the key pointed to by key_p
 *---*
 *---* CALLED BY
 *---*    whoever
 *---* 
 *---* CALLS INTO 
 *---*    'C' library
 *---*
 *---* PREREQUISITES
 *---*/

/*---*
 *---* Includes
 *---*/
#include <stdio.h>
#include <errno.h>
#include <dce/sec_login.h>


void
dump_key(sec_passwd_rec_t *key_p)
{
  char buff[10];
  int pid = getpid();

  if (!key_p)
  {
     printf("dump_key(%d): key at 0%x is null.\n", pid, key_p);
     return;
  }

  
  printf("dump key(%d): dumping key at 0%x\n", key_p);
  printf("dump_key(%d):   struct sec_passwd_rec_t {\n", pid);
  printf("dump_key(%d):            version_number = %u\n", pid, 
                                                        key_p->version_number);
#if defined(DUMP_KEY_PRINT_PEP)
  if (!key_p->pepper) 
       printf("Null pointer key_p->pepper.\n"); 
  else
       printf("dump_key(%d):            pepper = %s\n", pid, key_p->pepper);
#endif /*---* if defined(DUMP_KEY_PRINT_PEP) *---*/
  printf("dump_key(%d):            struct key {\n", pid);
  printf("dump_key(%d):                          key_type = ", pid);
  switch(key_p->key.key_type)
  {
      case sec_passwd_none: printf("none\n"); strcpy(buff, ""); break;
      case sec_passwd_plain: printf("plain\n"); 
                             if (!key_p->key.tagged_union.plain)
                               printf(
                              "Null pointer key_p->key.tagged_union.plain.\n");
                             else
                               strcpy(buff, 
                                   (char *)(key_p->key.tagged_union.plain)); 
                             break;
      case sec_passwd_des: printf("encrypted\n"); 
                             memcpy(buff, 
                                  (char *)(key_p->key.tagged_union.des_key),8);
                             buff[8] = '\0';
                             break;
  }
  printf("dump_key(%d):                          tagged_union = %s\n", pid, 
                                                                         buff);
  printf("dump_key(%d):            };\n",pid);
  printf("dump_key(%d):   };   \n",pid);
}

