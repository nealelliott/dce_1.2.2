/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krbstrdup.c,v $
 * Revision 1.1.305.2  1996/02/18  00:04:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:56:51  marty]
 *
 * Revision 1.1.305.1  1995/12/08  00:20:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:43  root]
 * 
 * Revision 1.1.303.1  1994/01/21  22:38:04  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:21  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:26:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:08:32  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:52:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:40:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:07:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*              
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      krbstrdup.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This function has been inserted to resolve the reference to 
**  strdup defined in the Domain runtime library but not in Ultrix.
**
**
*/

#include <commonp.h>
#include <krbp.h>

/*
**
** Prototype for strdup
**
*/
#ifdef ultrix
PRIVATE char * strdup PROTOTYPE (( char *  s ));
#endif



/*
** Function: strdup           
** Note:     This function has been inserted to resolve the reference to 
**	     strdup defined in the Domain runtime library but not in Ultrix.
*/

#ifdef ultrix
PRIVATE char * strdup( register char * s)
{
   register char * ret;
   if (ret = (char *) malloc(strlen( s )+1))
   {
       strcpy(ret, s);
   }
   return (ret);
}
#endif
