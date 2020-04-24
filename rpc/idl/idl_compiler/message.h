/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: message.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:18  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:19:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:04 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:12:23  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:25  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:07  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:58:06  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:53  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  20:01:12  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:33:22  ganni]
 * 
 * Revision 1.1.2.3  1993/01/03  21:40:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  14:36:22  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  18:48:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:03:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:02:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      message.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Constant definitions for international message support.
**
**  VERSION: DCE 1.0
**
*/

void message_print(
#if defined(__STDC__) && !defined(VMS)
    long msgid, ...
#endif
);

void message_open(
#ifdef PROTO
    char *image_name
#endif
);

void message_close(
#ifdef PROTO
    void
#endif
);
