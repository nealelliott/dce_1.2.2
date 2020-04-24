/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_misc.h,v $
 * Revision 1.1.5.2  1996/02/18  23:00:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:50  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:28:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:53  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:26:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:32  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:28:52  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:13:09  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:46:46  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:42:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - miscellaneous database operations
 */


#ifndef rsdb_misc_h__included
#define rsdb_misc_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>


 /* r s d b _ u u i d _ c r e a t e
  *
  * Create a new uuid with the version number set to show that the low 32 bits of
  * time represents a 32 bit Unix id.  If embedded_unix_id is false, just
  * create a uuid without embedding the unix_id.
  *
  * This function assumes that an external mechanism will guarantee uniqueness.
  */

PUBLIC void rsdb_uuid_create (
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    boolean32           embedded_unix_id,
    int                 unix_id,
    uuid_t              *uuid,
    error_status_t      *st
#endif
);


/* r s d b _ u n i x _ u u i d _ g e n _ c h e c k
 *
 * Generate a unix id and/or uuid OR check that the
 * input unix id and uuid are consistent (uuid contains
 * unix id) if embedded_unix_id is true.
 * For release DCE 1.0, the embedded_unix_id flag
 * should be false ONLY when creating ids for a cell principal.
 * Future releases of DCE may not require embedded unix ids,
 * in which case, this flag my be false.
 *
 * Handle four cases:  Note that the input domain is checked only
 * in the first case.
 * 1. Neither unix id nor uuid given: get the next free unix id
 *    for the given domain; use unix id to create uuid.
 * 2. Unix id given, uuid not given: use unix id to create uuid.
 * 3. Unix id not given, uuid given: extract unix id from uuid.
 * 4. Both unix id and uuid given: extract unix id from uuid and
 *    compare with input unix id -- error if they don't match.
 *
 * An input unix id of -1 indicates "not given."
 * An input uuid of uuid_nil indicates "not given."
 *
 *  Return true if no error, false otherwise.
 *
 *  Status Codes
 *  sec_rgy_unix_id_changed - (case 4) unix id doesn't match
 *      unix id extracted from uuid.
 *  sec_rgy_uuid_bad_version - uuid version bits didn't correspond to
 *      input boolean embedded_unix_id.
 *
 */
PUBLIC boolean32 rsdb_unix_uuid_gen_check (
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    boolean32           embedded_unix_id,
    int                 *unix_id,
    uuid_t              *uuid,
    error_status_t      *st
#endif
);


/* r s d b _ u n i x _ u u i d _ c h e c k
 *
 * Check that the input unix id and uuid are consistent
 * (uuid contains unix id).
 *
 * Both unix id and uuid must be given: extract unix id from uuid and
 * compare with input unix id -- error if they don't match.
 *
 * Return true if ids are consistent and no error, false otherwise.
 *
 *  Status Codes
 *  sec_rgy_unix_id_changed - (case 4) unix id doesn't match
 *      unix id extracted from uuid.
 *  sec_rgy_uuid_bad_version - uuid version bits incorrect.
 *
 */
PUBLIC boolean32 rsdb_unix_uuid_check (
#ifdef __STDC__
    int                 *unix_id,
    uuid_t              *uuid,
    error_status_t      *st
#endif
);



/* r s d b _ u u i d _ e m b e d s _ u n i x _ i d
 *
 * Return true if the SEC_UUID_VERSION_BITS are set on the
 * input uuid.  Return false otherwise.
 *
 */
PUBLIC boolean32 rsdb_uuid_embeds_unix_id (
#ifdef __STDC__
    uuid_t              *uuid
#endif
);

#endif  /* rsdb_misc_h__included */

