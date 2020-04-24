/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: codesets_stub.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:23:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:56  root]
 * 
 * Revision 1.1.2.5  1994/07/28  18:30:31  mori_m
 * 	CR: 11132  Added _CODESETS_STUB_H defines.
 * 	[1994/07/28  18:18:21  mori_m]
 * 
 * Revision 1.1.2.4  1994/06/10  20:55:04  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:29  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/04  21:52:59  mori_m
 * 	CR 9701: Added cs_byte typedef for use by applications.
 * 	[1994/05/04  21:52:27  mori_m]
 * 
 * Revision 1.1.2.2  1994/04/12  16:52:08  mori_m
 * 	CR 9701:  Second submission for RPC runtime I18N support.
 * 	[1994/04/12  16:35:57  mori_m]
 * 
 * Revision 1.1.2.1  1994/02/08  21:26:36  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:23:13  mori_m]
 * 
 * $EndLog$
 */
/*
**
**  NAME:
**
**      codesets_stub.h
**
**	This file cannot be IDL file, since 
**		idl_cs_convert_t 		-> (idlbase.h)
**		wchar_t 			-> (stdlib.h)
**	are defined in header file.  IDL file cannot import *.h files.
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  This file defines stub support routines, which support marshalling and
**  unmarshalling of i18n data.  Code set conversion is done automatically
**  when code set interoperability functionality is enabled.
**  
**
*/

#ifndef _CODESETS_STUB_H
#define _CODESETS_STUB_H

#include <dce/idlbase.h>
#include <stdlib.h>

/*
 * cs_byte is used as I18N byte type from I18N applications.
 */
typedef ndr_byte cs_byte;

/*
 * rpc_ns_import_ctx_add_eval routine requires the identification for
 * the evaluation routines.  If more evaluation routines are supported
 * in the future, the new identifications need to be added here.
 */
#define rpc_c_eval_type_codesets		0x0001
#define rpc_c_custom_eval_type_codesets		0x0002


extern void cs_byte_net_size (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	unsigned32		tag,
	/* [in] */	unsigned32		l_storage_len,
	/* [out] */	idl_cs_convert_t	*p_convert_type,
	/* [out] */	unsigned32		*p_w_storage_len,
	/* [out] */	error_status_t		*status
);

extern void wchar_t_net_size (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	unsigned32		tag,
	/* [in] */	unsigned32		l_storage_len,
	/* [out] */	idl_cs_convert_t	*p_convert_type,
	/* [out] */	unsigned32		*p_w_storage_len,
	/* [out] */	error_status_t		*status
);

extern void cs_byte_local_size (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	unsigned32		tag,
	/* [in] */	unsigned32		w_storage_len,
	/* [out] */	idl_cs_convert_t	*p_convert_type,
	/* [out] */	unsigned32		*p_l_storage_len,
	/* [out] */	error_status_t		*status
);

extern void wchar_t_local_size (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	unsigned32		tag,
	/* [in] */	unsigned32		w_storage_len,
	/* [out] */	idl_cs_convert_t	*p_convert_type,
	/* [out] */	unsigned32		*p_l_storage_len,
	/* [out] */	error_status_t		*status
);

extern void cs_byte_to_netcs (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	unsigned32		tag,
	/* [in] */	idl_byte		*ldata,
	/* [in] */	unsigned32		l_data_len,
	/* [out] */	idl_byte		*wdata,
	/* [out] */	unsigned32		*p_w_data_len,
	/* [out] */	error_status_t		*status
);

extern void wchar_t_to_netcs (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	unsigned32		tag,
	/* [in] */	wchar_t			*ldata,
	/* [in] */	unsigned32		l_data_len,
	/* [out] */	idl_byte		*wdata,
	/* [out] */	unsigned32		*p_w_data_len,
	/* [out] */	error_status_t		*status
);

extern void cs_byte_from_netcs (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	unsigned32		tag,
	/* [in] */	idl_byte		*wdata,
	/* [in] */	unsigned32		w_data_len,
	/* [in] */	unsigned32		l_storage_len,
	/* [out] */	idl_byte		*ldata,
	/* [out] */	unsigned32		*p_l_data_len,
	/* [out] */	error_status_t		*status
);

extern void wchar_t_from_netcs (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	unsigned32		tag,
	/* [in] */	idl_byte		*wdata,
	/* [in] */	unsigned32		w_data_len,
	/* [in] */	unsigned32		l_storage_len,
	/* [out] */	wchar_t			*ldata,
	/* [out] */	unsigned32		*p_l_data_len,
	/* [out] */	error_status_t		*status
);

/*
 * R P C _ C S _ G E T _ T A G S
 *
 * Take a binding handle, and figure out the necessary tags value.
 * This routine is called from stubs.
 */
extern void rpc_cs_get_tags (
	/* [in] */	rpc_binding_handle_t	h,
	/* [in] */	idl_boolean		server_side,
	/* [out] */	unsigned32		*p_stag,
	/* [in, out] */	unsigned32		*p_drtag,
	/* [out] */	unsigned32		*p_rtag,
	/* [out] */	error_status_t		*status
);

#endif
