/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: ccdefops.c,v $
 * Revision 1.1.2.1  1996/06/05  20:37:17  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:40:09  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/ccache/ccdefops.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * Default credentials cache determination.  This is a separate file
 * so that the user can more easily override it.
 */

#include "k5-int.h"

#ifdef HAVE_SYS_TYPES_H
/* Systems that have <sys/types.h> probably have Unix-like files (off_t,
   for example, which is needed by fcc.h).  */

#include "fcc.h"		/* From file subdir */
krb5_cc_ops *krb5_cc_dfl_ops = &krb5_cc_file_ops;

#else
/* Systems that don't have <sys/types.h> probably have stdio anyway.  */

#include "scc.h"		/* From stdio subdir */
krb5_cc_ops *krb5_cc_dfl_ops = &krb5_scc_ops;

#endif
