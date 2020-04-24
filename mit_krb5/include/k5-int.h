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
 * $Log: k5-int.h,v $
 * Revision 1.1.2.1  1996/06/05  20:25:11  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:25:29  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: k5-int.h,v $
 * Revision 1.1.2.1  1996/06/05  20:25:11  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:25:29  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  19:25:29  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:18 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:43 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:07 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * This prototype for k5-int.h (Krb5 internals include file)
 * includes the user-visible definitions from krb5.h and then
 * includes other definitions that are not user-visible but are
 * required for compiling Kerberos internal routines.
 *
 * John Gilmore, Cygnus Support, Sat Jan 21 22:45:52 PST 1995
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#ifndef _KRB5_INT_H
#define _KRB5_INT_H

#include "k5-config.h"

#include "krb5.h"

/* krb5/krb5.h includes many other .h files in the krb5 subdirectory.
   The ones that it doesn't include, we include below.  */

#include "k5-errors.h"

#include "asn1.h"
#include "copyright.h"
#ifdef OSF_DCE
#include "krb5/dbm.h"
#else
#include "dbm.h"
#endif
#include "ext-proto.h"
/* Needed to define time_t for kdb.h prototypes.  */
#include "sysincl.h"
#include "los-proto.h"
#include "kdb.h"
#include "kdb_dbm.h"
#include "libos.h"
#include "mit-des.h"
#include "preauth.h"
#include "rsa-md5.h"
/* #include "krb5/wordsize.h" -- comes in through base-defs.h. */
#include "profile.h"

struct _krb5_context {
	krb5_magic	magic;
	krb5_enctype  FAR *etypes;
	int		etype_count;
	void	      FAR *os_context;
	char	      FAR *default_realm;
	profile_t     profile;
};
#endif /* _KRB5_INT_H */
