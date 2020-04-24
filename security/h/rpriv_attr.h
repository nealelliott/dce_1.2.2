/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpriv_attr.h,v $
 * Revision 1.1.6.2  1996/02/18  22:58:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:27:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:22  root]
 * 
 * Revision 1.1.2.6  1994/09/28  20:50:12  greg
 * 	Remove blank line from comments that was causing bci
 * 	to complain about improper leader line ...
 * 	[1994/09/28  18:10:47  greg]
 * 
 * Revision 1.1.3.2  94/09/21  9:43:46  greg
 * 	Add delegate info to priv_attr_trig callouts.
 * 
 * Revision 1.1.2.5  1994/08/25  16:19:38  mdf
 * 	final changes based on builds
 * 	[1994/08/25  16:19:17  mdf]
 * 
 * Revision 1.1.2.4  1994/08/10  19:21:44  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:02  annie]
 * 
 * Revision 1.1.2.3  1994/07/15  14:59:02  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:44  mdf]
 * 
 * 	/main/ODESSA_2/2  1994/06/17  12:19 UTC  mdf
 * 	 Hardwired Trigger support for attribute name authzp
 * 
 * 	/main/ODESSA_2/1  1994/03/10  16:07 UTC  ahop
 * 	 merge to ODESSA_2
 * 
 * 	/main/ahop_odss_era_bl18/2  1994/03/10  16:05 UTC  ahop
 * 	fix comment
 * 
 * 	/main/ahop_odss_era_bl18/1  1994/03/10  15:09 UTC  ahop
 * 	priv attr interface
 * 
 * $EndLog$
 */
/*
 *
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 * Priv Attr Interface - attribute support routines for
 * the Privilege Server.
 *
 */

#ifndef rpriv_attr_h__included
#define rpriv_attr_h__included

#include <dce/sec_attr_base.h>
#include <dce/id_epac.h>


/* r p r i v _ a t t r _ g e t _ e f f e c t i v e
 *
 * Allocate an array for and return all attributes 
 * of the requested types for the named principal object.
 * If a requested attribute doesn't exist for the
 * object, attempt to retrieve an appropriate default.
 * The calling identity, represented by client_epac, is checked for
 * access to the requested attributes.  If the caller does
 * not have authorization to view one or more of the
 * requested attributes, those attributes are not returned.
 * [Is this the right behavior OR should the entire call fail?]
 *
 * Attribute Sets:
 * Reads of an attr_set return all instances of members 
 * of that set.  If an instance doesn't exist for a certain
 * member, attempt to retrieve a default.
 * The attr_set instance itself will not be returned.
 *
 * Triggers:
 * If the requested attribute type is associated with a query
 * trigger AND the invoke_trigger input boolean is FALSE, 
 * the value returned for the attribute will be the
 * binding (as set in the schema entry) of the trigger server.
 * The caller must bind to the trigger server and pass the
 * original input query attribute to the sec_attr_trig_query call
 * in order to retrieve the attribute value.
 * If the requested attribute type is associated with a query
 * trigger AND the invoke_trigger input boolean is TRUE, 
 * this routine will attempt to bind to the trigger server to
 * retrieve the requested attributes.
 *
 *    In Parameters:
 *	principal_name - identifies the registry principal object on which
 *            to perform this operation.
 *	client_epac - identifies the caller; used for authorization
 *		check.
 *      num_upstream_delegates - if the principal is being added
 *            to an existing delegation chain, the number of delegates
 *            in that existing ("upstream") delegation chain.
 *      upstream_delegates - the upstream delegation chain, if any.
 *	num_attr_keys - specifies the number of elements in the
 *            input attr_keys array.
 *	attr_keys - in the attr_id field of each element, contains
 *            the attribute type id of the attribute instance(s)
 *            requested by this lookup. If the requested attribute
 *            type is associated with a query trigger, the attr_val
 *            field may be used to pass in optional information 
 *            required by the trigger query. If no information is
 *            to be passed in the attr_val field (whether the type
 *            indicates a trigger query or not), the attr_val
 *            encoding type should be set to
 *            sec_attr_enc_void.
 *	invoke_trigger - if TRUE, and an attr_key is associated with
 *		a query trigger, this routine will perform the
 *		remote sec_attr_trig_query call and return the
 *		attribute(s) from that call.
 *		if FALSE, this routine will not perform any 
 *		sec_attr_trig_query calls and will return
 *		the attribute's local representation (including
 *		trigger server binding info).
 *	get_auth_ident - function used by this routine, in
 *		cases where a query trigger must be made, to
 *		generate the auth_identity of the caller for
 *		use in setting the auth info on the binding
 *		handle used to contact the trigger server.
 *            
 *    Out Parameters:
 *	num_returned - specifies the number of attribute instances
 *            returned in the attrs array.
 *	attrs - rpc_ss_allocated array of every attribute
 *		satisfying the search criteria in attr_keys.
 *
 */
void rpriv_attr_get_effective
(
    sec_attr_component_name_t	principal_name,		/* [in] */
    sec_id_epac_data_t          *client_epac,		/* [in] */
    unsigned32                  num_upstream_delegates, /* [in] */
    sec_id_epac_data_t          upstream_delegates[],   /* [in] */
    unsigned32                  num_attr_keys,		/* [in]  */
    sec_attr_t                  attr_keys[],		/* [in]  */
    boolean32			invoke_trigger, 	/* [in]  */
    rpc_auth_identity_handle_t 	(*get_auth_ident)(),	/* [in]  */
    unsigned32                  *num_returned,  	/* [out] */
    sec_attr_t                  **attrs,        	/* [out] */
    error_status_t              *st_p			/* [out] */
);

#endif /* rpriv_attr_h__included */

