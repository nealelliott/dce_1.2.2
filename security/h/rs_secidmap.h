/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_secidmap.h,v $
 * Revision 1.1.6.1  1996/10/15  20:50:02  arvind
 * 	CHFts20208: prototype check.
 * 	[1996/10/09  18:31 UTC  sommerfeld  /main/sommerfeld_pk_kdc_2/1]
 *
 * Revision 1.1.4.2  1996/02/18  22:59:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:14  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:27:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:34  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:25:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_secidmap.h V=3 11/11/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *  internally available rs_secidmap functions
 */

#ifndef rs_secidmap_h__included
#define rs_secidmap_h__included

#include <rsecidmap.h>


/*
 * r s _ s e c _ i d _ p a r s e _ n a m e            AKA redbox-internals
 *
 * Parse a global principal name into a cell name and a cell relative
 * principal name.  Return the IDs associated with each component.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped (On the wire this is controlled by the 
 * selector parameter).
 */
void rs_sec_id_parse_name (
#ifdef __STDC__
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              global_namep,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              cell_namep,
    uuid_t                      *cell_idp,
    sec_rgy_name_t              princ_namep,
    uuid_t                      *princ_idp,
    error_status_t              *status
#endif
);

/*
 * r s _ s e c _ i d _ g e n _ n a m e              AKA bluebox-internals
 *
 * Generate a global name from the cell and principal uuids.  Also returns
 * the name of the cell uuid and the principal uuid as independent units.
 */
void rs_sec_id_gen_name (
#ifdef __STDC__
    sec_rgy_domain_t            domain,
    uuid_t                      *cell_idp,
    uuid_t                      *princ_idp,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              global_namep,
    sec_rgy_name_t              cell_namep,
    sec_rgy_name_t              princ_namep,
    error_status_t              *status
#endif
);

void rs_sec_id_install_bypass(
    error_status_t *status
);

#endif  /* rs_secidmap_h__included */
