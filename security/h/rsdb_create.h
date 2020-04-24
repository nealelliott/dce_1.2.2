/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_create.h,v $
 * Revision 1.1.5.2  1996/02/18  23:00:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:37  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:28:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:47  root]
 * 
 * Revision 1.1.2.5  1992/12/29  13:25:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:20  zeliff]
 * 
 * Revision 1.1.2.4  1992/11/20  15:51:04  ahop
 * 	Move init_database to rsdb.c; move keyseed out of create_database params.
 * 	[1992/11/10  23:04:30  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:28:31  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:13:01  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:46:13  burati
 * 	 Second replication code drop: bl5
 * 	 Add keyseed to rsdb_create_database params.
 * 
 * Revision 1.1.2.2  1992/08/31  18:20:03  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:58:08  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - database creation operations
 */

#ifndef rsdb_create_h__included
#define rsdb_create_h__included


#include <dce/nbase.h>
#include <dce/id_base.h>


/* Default Policy */
#define RSDB_DEF_POL_PASSWD_MIN_LEN         0
#define RSDB_DEF_POL_PASSWD_LIFE            0
#define RSDB_DEF_POL_PASSWD_EXP_DATE        0
#define RSDB_DEF_POL_ACCT_LIFE              0
#define RSDB_DEF_POL_PASSWD_FLAGS           0

/* Default Auth Policy */
#define RSDB_DEF_AUTH_POL_MAX_TKT_LIFE      60*60*24    /* 24 hours */
#define RSDB_DEF_AUTH_POL_MAX_RENEW_LIFE    60*60*24*28 /* 28 days */

/* Default Properties */
#define RSDB_DEF_PROP_READ_VERSION          1
#define RSDB_DEF_PROP_WRITE_VERSION         1
#define RSDB_DEF_PROP_MIN_TKT_LIFE          60*5        /* 5 minutes */
#define RSDB_DEF_PROP_DEF_CERT_LIFE         60*60*10    /* 10 hours */
#define RSDB_DEF_PROP_LOW_UNIX_ID_PERSON    100
#define RSDB_DEF_PROP_LOW_UNIX_ID_GROUP     100
#define RSDB_DEF_PROP_LOW_UNIX_ID_ORG       100
#define RSDB_DEF_PROP_MAX_UNIX_ID           32767   /* "width" of unix id field */
#define RSDB_DEF_PROP_FLAGS                 sec_rgy_prop_auth_cert_unbound | \
                                            sec_rgy_prop_shadow_passwd     | \
                                            sec_rgy_prop_embedded_unix_id
#define RSDB_DEF_PROP_UNAUTH_QUOTA          0


void rsdb_create_set_default_properties (
#ifdef __STDC__
    sec_rgy_properties_t    *default_properties
#endif
);

void rsdb_create_database (
#ifdef __STDC__
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    unsigned_char_p_t       rgy_myname,
    sec_rgy_properties_t    *default_properties
#endif
);
 
#endif  /* rsdb_create_h__included */

