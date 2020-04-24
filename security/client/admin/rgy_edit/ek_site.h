/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ek_site.h,v $
 * Revision 1.1.4.2  1996/03/11  13:26:58  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:00:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:14  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:40:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ek_site.h V=9 06/12/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**       public rgy_edit kernel site binding operations
** 
*/
#ifndef EK_SITE_PUB_H
#define EK_SITE_PUB_H

#include <dce/lbase.h>

/* Needs to be changed to max char limit for NSI naming, once available */
typedef unsigned_char_t ek_site_name_t[1025];

typedef enum { ek_site_update, ek_site_query } ek_site_access_t;

error_status_t ek_site_ping (
#ifdef __STDC__
    ek_site_name_t    site,           /* [out] */
    ek_site_access_t  *access_type_p  /* [out] */
#endif
);

error_status_t ek_site_open_by_name (
#ifdef __STDC__
    ek_site_name_t    site,           /* [in, out] */
    ek_site_access_t  *access_type_p  /* [out] */
#endif
);

error_status_t ek_site_open_by_access (
#ifdef __STDC__
    ek_site_name_t    site,            /* [out] */
    ek_site_access_t  *access_type_p   /* [in] */
#endif
);

error_status_t ek_site_get_current ( 
#ifdef __STDC__
    ek_site_name_t    site,           /* [out] */
    ek_site_access_t  *access_type_p  /* [out] */
#endif
);


error_status_t ek_site_close(
#ifdef __STDC__
    void 
#endif
);

#endif
