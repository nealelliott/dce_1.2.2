/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_id_pac_util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:01:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:29:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:16  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:27:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:16:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sec_id_pac_util.h V=1 09/26/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
** 
** sec_id_pac_t utility routines
*/

#ifndef sec_id_pac_util_included
#define sec_id_pac_util_included

#ifdef __cplusplus
    extern "C" {
#endif

#include <dce/id_base.h>

/*
 * Functions
 */


/* s e c _ i d _ p a c _ u t i l _ f r e e
 * 
 * Release dynamic storage associated with a PAC.
 */
void sec_id_pac_util_free ( sec_id_pac_t *p );


/* s e c _ i d _ p a c _ u t i l _ c o p y
 *
 * Utility function to copy a pac.
 */
error_status_t sec_id_pac_util_copy (
#ifdef __STDC__
   /* [in]  */   sec_id_pac_t *to,
   /* [out] */   sec_id_pac_t *from
#endif
);


#ifdef __cplusplus
    }
#endif

#endif
