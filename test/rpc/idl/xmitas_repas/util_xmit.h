/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_xmit.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:08  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:32:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:17:04  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:59:16  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:48:12  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*    
** Local type not necessarily defined in IDL (same as net rep)
*/
typedef struct {
    ndr_long_int last;
    ndr_long_int values[100];
    } local_t;

