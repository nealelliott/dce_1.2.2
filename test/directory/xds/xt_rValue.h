/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_rValue.h,v $
 * Revision 1.1.4.2  1996/02/18  23:04:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:14:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:19  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:09:48  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:28:11  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:29:01  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:49:14  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  20:19:41  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:43  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:16:16  marrek
 * 	Beautification.
 * 	[1994/02/09  15:45:26  marrek]
 * 
 * 	Created for dce1.1 code drop.
 * 	[1994/02/09  11:30:40  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_RVALUE_H
#define _XT_RVALUE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_rvalue_rcsid[] = "@(#)$RCSfile: xt_rValue.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:04:18 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


char *xt_status_to_buffer(DS_status status, char *buffer);
OM_sint xt_cmp_status (
	OM_private_object  received_status,
	OM_descriptor      *expected_status,
	/*OM_descriptor      **expected_status,*/
	enum f_type        func_called);
OM_sint xt_cmp_system_error ( OM_public_object  received_system_error,
OM_public_object  expected_system_error );
OM_sint xt_cmp_update_error ( OM_public_object  received_update_error,
OM_public_object  expected_update_error );
OM_sint xt_cmp_library_error ( OM_public_object  received_library_error,
OM_public_object  expected_library_error );
OM_sint xt_cmp_communications_error ( OM_public_object  received_communications_error,
OM_public_object  expected_communications_error );
OM_sint xt_cmp_attribute_error ( OM_public_object  received_attribute_error,
OM_public_object  expected_attribute_error );
OM_sint xt_cmp_name_error ( OM_public_object  received_name_error,
OM_public_object  expected_name_error );
OM_sint xt_cmp_referral ( OM_public_object  received_referral,
OM_public_object  expected_referral );
OM_sint xt_cmp_security_error ( OM_public_object  received_security_error,
OM_public_object  expected_security_error );
OM_sint xt_cmp_service_error ( OM_public_object  received_service_error,
OM_public_object  expected_service_error );
OM_sint xt_cmp_abandon_failed ( OM_public_object  received_abandon_failed,
OM_public_object  expected_abandon_failed );
OM_sint xt_cmp_session ( OM_private_object received_session,
OM_descriptor    *expected_session );
OM_sint xt_cmp_compare_result ( OM_private_object received_result,
OM_descriptor    *expected_result );
OM_sint xt_cmp_list_result ( OM_private_object received_result,
OM_descriptor    *expected_result );
OM_sint xt_cmp_read_result ( OM_private_object received_result,
OM_descriptor    *expected_result );
OM_sint xt_cmp_search_result ( OM_private_object received_result,
OM_descriptor    *expected_result );
OM_sint xt_cmp_integer ( OM_sint    received_integer,
OM_sint    expected_integer );
OM_sint xt_cmp_enum ( OM_enumeration  received_enum,
OM_enumeration  expected_enum );
OM_sint xt_cmp_boolean ( OM_boolean received_boolean,
OM_boolean expected_boolean );
OM_sint xt_cmp_class ( OM_object_identifier    received_class,
OM_object_identifier    expected_class );
OM_sint xt_cmp_string ( OM_string    received_string,
OM_string    expected_string );
OM_sint xt_cmp_return_code ( OM_uint    received_return_code,
OM_uint    expected_return_code );
OM_sint xt_cmp_object ( OM_descriptor    *received_object,
OM_descriptor    *expected_object );
OM_sint xt_check_atts (OM_descriptor        *p_rv,
OM_descriptor        *p_ev,
attribute            *p_atts_tbl);
OM_sint xt_compare_atts (
	attribute       *atts_tbl,
	OM_descriptor   *received_list,
	OM_descriptor   *expected_list,
	OM_sint no_of_atts);
OM_sint xt_free_index (attribute *atts_tbl);
#endif /* ifndef _XT_RVALUE_H */
