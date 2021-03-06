/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ra.h,v $
 * Revision 1.1.4.2  1996/02/18  23:05:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:02  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:29:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:30  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:34:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:42:27  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/idlbase.h>

    typedef struct {
        ndr_byte high_byte;
        ndr_byte low_byte;
    } JIS_KANJI;

    typedef struct {
        ndr_byte high_byte;
        ndr_byte low_byte;
    } IBM_KANJI;
