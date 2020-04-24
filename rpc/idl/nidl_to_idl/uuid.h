/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuid.h,v $
 * Revision 1.1.4.2  1996/02/18  23:46:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:00  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:38:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:54  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:34:16  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:49:06  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:34:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:14:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:59:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef uuid__v0_included
#define uuid__v0_included
#include <idl_base.h>
#include <nbase.h>
typedef ndr_$char uuid_$string_t[37];
extern  void uuid_$gen
#ifdef __STDC__
 (
  /* [out] */uuid_$t *uuid);
#else
 ( );
#endif
extern  void uuid_$encode
#ifdef __STDC__
 (
  /* [in] */uuid_$t *uuid,
  /* [out] */uuid_$string_t s);
#else
 ( );
#endif
extern  void uuid_$decode
#ifdef __STDC__
 (
  /* [in] */uuid_$string_t s,
  /* [out] */uuid_$t *uuid,
  /* [out] */status_$t *st);
#else
 ( );
#endif
extern  ndr_$boolean uuid_$equal
#ifdef __STDC__
 (
  /* [in] */uuid_$t *u1,
  /* [in] */uuid_$t *u2);
#else
 ( );
#endif
extern  ndr_$long_int uuid_$cmp
#ifdef __STDC__
 (
  /* [in] */uuid_$t *u1,
  /* [in] */uuid_$t *u2);
#else
 ( );
#endif
extern  ndr_$ulong_int uuid_$hash
#ifdef __STDC__
 (
  /* [in] */uuid_$t *u,
  /* [in] */ndr_$ulong_int modulus);
#else
 ( );
#endif
#endif
