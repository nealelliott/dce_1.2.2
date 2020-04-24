/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_util.h,v $
 * Revision 1.1.4.2  1996/03/11  13:26:47  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:59:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:58  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:39:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:11  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  eif_util.h V=7 11/22/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  rgy_edit generic interface utilities
** 
*/

#ifndef eif_util_h__included
#define eif_util_h__included
 
#include <dce/nbase.h>

#include <ek_types.h>
#include <tty_io.h>

#ifdef UNDEF
#define EIF_FATAL(preamble, stp) { \
            tty_print_status((preamble), *(stp)); \
            pfm_$signal(*(stp)); \
	}
#endif
#define EIF_FATAL(preamble, stp) { \
            tty_print_status((preamble), *(stp)); \
            exit(*stp); \
	}
#endif
