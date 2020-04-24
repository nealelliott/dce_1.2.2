/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:18:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:45  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:40:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:50  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <xmit.h>

static long_as_long_t _lal(h, a1, a2, a3, a4)
handle_t h;
long_as_long_t *a1, *a2, a3, *a4;
{
      *a2 = *a1;
      if (a2 != a4)
        printf("***FAILURE***: Alias between a2(0x%x) and a4(0x%x) not preserved\n");
      return a3;
}

static long_as_struct_t _las(h, a1, a2, a3)
handle_t h;
long_as_struct_t *a1, *a2, a3;
{
      *a2 = *a1;
      return a3;
}

static struct_as_long_t _sal(h, a1, a2, a3)
handle_t h;
struct_as_long_t *a1, *a2, a3;
{
      struct_as_long_t res;
      *a2->val = *a1->val;
      res.val = a3.val;
      res.next = (struct_t*)0;
      return res;
}

globaldef xmit_v0_0_epv_t xmit_v0_0_m_epv = {_lal,_las,_sal};
