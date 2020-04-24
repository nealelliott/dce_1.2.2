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
 * $Log: cs_s_stub.c,v $
 * Revision 1.1.4.3  1996/02/18  00:03:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:55:45  marty]
 *
 * Revision 1.1.4.2  1996/02/07  17:48:13  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:13:58  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.2.7  1994/09/27  18:47:22  mori_m
 * 	CR 11041:  Deleted unnecessary code for wchar_t varying array ops.
 * 	[1994/09/27  18:43:50  mori_m]
 * 
 * Revision 1.1.2.6  1994/09/23  19:47:40  mori_m
 * 	Merged with changes from 1.1.2.5
 * 	[1994/09/23  19:47:34  mori_m]
 * 
 * 	CR 11044/11041: Copy the null-terminator of a string.  Set the correct number
 * 			of character units for varying array operation.
 * 	[1994/09/23  18:42:39  mori_m]
 * 
 * Revision 1.1.2.5  1994/09/23  18:43:03  mori_m
 * 	CR 11044/11041: Copy the null-terminator of a string.  Set the correct number
 * 			of character units for varying array operation.
 * 	[1994/09/23  18:42:39  mori_m]
 * 
 * Revision 1.1.2.4  1994/06/10  20:54:47  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:12  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/04  21:52:57  mori_m
 * 	CR 9701: Copy the entire I-char array contents for no conversion, instead of
 * 		 using strncopy.   Fixed incorrect wchar size allocation.
 * 	[1994/05/04  21:04:08  mori_m]
 * 
 * Revision 1.1.2.2  1994/04/12  16:52:03  mori_m
 * 	CR 9701:  Second submission for RPC runtime I18N support.
 * 	[1994/04/12  16:45:46  mori_m]
 * 
 * Revision 1.1.2.1  1994/02/08  21:35:46  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:35:37  mori_m]
 * 
 * $EndLog$
 */
/*
**  NAME
**
**     cs_s_stub.c
**
**  FACILITY:
**
**     Remote Procedure Call (RPC)
**     I18N Character Call   (RPC)
**
**  ABSTRACT:
**
**
*/
#include <commonp.h>		/* include nbase.h lbase.h internally	*/
#include <ns.h>
#include <com.h>
#include <nsp.h>
#include <dce/idlbase.h>	/* definitions for idl_cs_* 		*/
#include <dce/rpcsts.h>
#include <codesets.h>		/* Data definitions for I18N NSI 
							sub-component   */
#include <dce/codesets_stub.h>	/* Stub support routines */

#include <stdio.h>		/* definition of NULL			*/
#include <stdlib.h>		/* definition of MB_CUR_MAX		*/
#include <langinfo.h>		/* definition of nl_langinfo routine	*/

#include <codesets.h>
#include <cs_s.h>		/* Private defs for code set interoperability */


/*
**++
**  ROUTINE NAME:       cs_byte_to_netcs
**
**  SCOPE:              PUBLIC - declared in dce/codesets_stub.h
**
**  DESCRIPTION:        
**
**  Convert an encoding of a local data to network encoding, based on
**  the information from a tag.
**
**  INPUTS:             
**
**      h		Binding handle
**
**      tag		Identifies the code set that will be used on the wire.
**
**	ldata		Address of the local data.
**
**      l_data_len	The number of "byte" data elements to be processed.
**
**  OUTPUTS:            
**
**	wdata		Address to which the converted data is to be written.
**
**	p_w_data_len	NULL if fixed array is being marshalled.
**			The address to which the routine writes the
**			on-the-wire data length.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void cs_byte_to_netcs
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	unsigned32		tag,
	idl_byte		*ldata,
	unsigned32		l_data_len, 
	idl_byte		*wdata, 
	unsigned32		*p_w_data_len, 
	error_status_t		*status
)
#else
(h, tag, ldata, l_data_len, wdata, p_w_data_len, status)
	rpc_binding_handle_t	h;
	unsigned32		tag;
	idl_byte		*ldata;
	unsigned32		l_data_len;
	idl_byte		*wdata;
	unsigned32		*p_w_data_len;
	error_status_t		*status;
#endif
{
	char			*current_codeset;
	unsigned32		current_rgy_codeset;
	int			i;
	idl_byte		*wdata_temp, *ldata_temp;
	
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	rpc_binding_rep_p_t 	bind_p;


	bind_p = (rpc_binding_rep_p_t)h;
	if (!RPC_BINDING_IS_SERVER (bind_p))
	{
		switch (bind_p->cs_eval.key)
		{
		case RPC_CS_EVAL_METHOD:
			method_p = &bind_p->cs_eval.tagged_union.method_key;

			switch (method_p->method)
			{
			case RPC_EVAL_NO_CONVERSION:
			case RPC_EVAL_RMIR_MODEL:
			case RPC_EVAL_SMIR_MODEL:
				wdata_temp = wdata;
				ldata_temp = ldata;
				for (i=0; i<= l_data_len; i++)
					*wdata_temp++ = *ldata_temp++;

				if (p_w_data_len != NULL)
				{
					*p_w_data_len = strlen((char *)wdata) + 1;
				}

				*status = rpc_s_ok;
				break;

			case RPC_EVAL_CMIR_MODEL:
			case RPC_EVAL_INTERMEDIATE_MODEL:
			case RPC_EVAL_UNIVERSAL_MODEL:

				stub_conversion (
					h,
					RPC_BINDING_IS_SERVER (bind_p),
					method_p->tags.client_tag,
					tag,
					ldata,
					l_data_len,
					wdata,
					p_w_data_len,
					status );

				if (p_w_data_len != NULL)
				{
					*p_w_data_len = strlen((char *)wdata) + 1;
				}

				break;
	
			default:
				*status = rpc_s_ss_incompatible_codesets;
				break;

			}
			break;

		case RPC_CS_EVAL_TAGS:

			tags_p = &bind_p->cs_eval.tagged_union.tags_key;

			/*
			 * Determine the conversion type.
			 */
			if (tag == tags_p->client_tag)	
					/* No conversion required */
			{
				wdata_temp = wdata;
				ldata_temp = ldata;
				for (i=0; i<= l_data_len; i++)
					*wdata_temp++ = *ldata_temp++;

				if (p_w_data_len != NULL)
				{
					*p_w_data_len = strlen((char *)wdata) + 1;
				}

				*status = rpc_s_ok;
			}
			else
			{
				stub_conversion (
					h,
					RPC_BINDING_IS_SERVER (bind_p),
					tags_p->client_tag,
					tag,
					ldata,
					l_data_len,
					wdata,
					p_w_data_len,
					status );

				if (p_w_data_len != NULL)
				{
					*p_w_data_len = strlen((char *)wdata) + 1;
				}
			}
			break;

		default:
			*status = rpc_s_ss_incompatible_codesets;
			break;
		}
	}
	else	/* server side */
	{
		/*
		 * Get the code set info from the current locale.
		 */
#if __hp9000s800
        	current_codeset = dce_langinfo(CODESET);
#else
           	current_codeset = nl_langinfo(CODESET);  /* broken on 9.0 UX */
#endif
				dce_cs_loc_to_rgy(
			(unsigned_char_p_t)current_codeset, 
			&current_rgy_codeset, 
			NULL, NULL,
			status);

		if (*status != dce_cs_c_ok)
		{
			/* codeset registry error */
			*status = rpc_s_ok;
			return;
		}
		/*
		 * Determine the conversion type.
		 */
		if (tag == current_rgy_codeset)	
				/* No conversion required */
		{
			wdata_temp = wdata;
			ldata_temp = ldata;
			for (i=0; i<= l_data_len; i++)
				*wdata_temp++ = *ldata_temp++;

			if (p_w_data_len != NULL)
			{
				*p_w_data_len = strlen((char *)wdata) + 1;
			}

			*status = rpc_s_ok;
		}
		else
		{
			stub_conversion (
				h,
				RPC_BINDING_IS_SERVER (bind_p),
				current_rgy_codeset,
				tag,
				ldata,
				l_data_len,
				wdata,
				p_w_data_len,
				status );

			if (p_w_data_len != NULL)
			{
				*p_w_data_len = strlen((char *)wdata) + 1;
			}
		}
	}
	return;
}


/*
**++
**  ROUTINE NAME:       wchar_t_to_netcs
**
**  SCOPE:              PUBLIC - declared in dce/codesets_stub.h
**
**  DESCRIPTION:        
**
**  Convert an encoding of a local data to network encoding, based on
**  the information from a tag.
**
**  INPUTS:             
**
**      h		Binding handle
**
**      tag		Identifies the code set that will be used on the wire.
**
**	ldata		Address of the local data.
**
**      l_data_len	The number of "byte" data elements to be processed.
**
**  OUTPUTS:            
**
**	wdata		Address to which the converted data is to be written.
**
**	p_w_data_len	NULL if fixed array is being marshalled.
**			The address to which the routine writes the
**			on-the-wire data length.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void wchar_t_to_netcs
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	unsigned32		tag,
	wchar_t			*ldata,
	unsigned32		l_data_len, 
	idl_byte		*wdata, 
	unsigned32		*p_w_data_len, 
	error_status_t		*status
)
#else
(h, tag, ldata, l_data_len, wdata, p_w_data_len, status)
	rpc_binding_handle_t	h;
	unsigned32		tag;
	wchar_t			*ldata;
	unsigned32		l_data_len;
	idl_byte		*wdata;
	unsigned32		*p_w_data_len;
	error_status_t		*status;
#endif
{
	char			*current_codeset;
	unsigned32		current_rgy_codeset;
	idl_byte		*byte_ldata;
	idl_byte		*t_byte_ldata;
	size_t			ldata_length;
	size_t			conv_ret;
	int			i;
	idl_byte		*wdata_temp, *ldata_temp;
	
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	rpc_binding_rep_p_t 	bind_p;

	/* 
	 *  Allocate the largest buffer for the conversion.
	 */
	ldata_length = l_data_len * MB_CUR_MAX + MB_CUR_MAX;

	RPC_MEM_ALLOC (
		byte_ldata,
		unsigned_char_p_t,
		ldata_length,
		RPC_C_MEM_STRING,
		RPC_C_MEM_WAITOK);
	if (byte_ldata == NULL){
	    *status = rpc_s_no_memory;
	    return;
	}

	/* Initialize the buffer */
	t_byte_ldata = byte_ldata;
	i = ldata_length;
	while (i--)
		*t_byte_ldata++ = '\0';

	conv_ret = wcstombs((char *)byte_ldata, ldata, ldata_length);

	if (conv_ret == -1)		/* Conversion error took place */
	{
		*status = rpc_s_ss_invalid_char_input;
		RPC_MEM_FREE(byte_ldata, RPC_C_MEM_STRING);
		return;
	}

	bind_p = (rpc_binding_rep_p_t)h;
	if (!RPC_BINDING_IS_SERVER (bind_p))
	{
		switch (bind_p->cs_eval.key)
		{
		case RPC_CS_EVAL_METHOD:
			method_p = &bind_p->cs_eval.tagged_union.method_key;

			switch (method_p->method)
			{
			case RPC_EVAL_NO_CONVERSION:
			case RPC_EVAL_RMIR_MODEL:
			case RPC_EVAL_SMIR_MODEL:
				wdata_temp = wdata;
				ldata_temp = byte_ldata;
				for (i=0; i<= conv_ret; i++)
					*wdata_temp++ = *ldata_temp++;

				if (p_w_data_len != NULL)
					*p_w_data_len = conv_ret;

				*status = rpc_s_ok;
				break;

			case RPC_EVAL_CMIR_MODEL:
			case RPC_EVAL_INTERMEDIATE_MODEL:
			case RPC_EVAL_UNIVERSAL_MODEL:
				stub_conversion (
					h,
					RPC_BINDING_IS_SERVER (bind_p),
					method_p->tags.client_tag,
					tag,
					byte_ldata,
					conv_ret,
					wdata,
					p_w_data_len,
					status );
				break;
	
			default:
				*status = rpc_s_ss_incompatible_codesets;
				break;

			}
			break;

		case RPC_CS_EVAL_TAGS:
			tags_p = &bind_p->cs_eval.tagged_union.tags_key;

			/*
			 * Determine the conversion type.
			 */
			if (tag == tags_p->client_tag)	
					/* No conversion required */
			{
				wdata_temp = wdata;
				ldata_temp = byte_ldata;
				for (i=0; i<= conv_ret; i++)
					*wdata_temp++ = *ldata_temp++;

				if (p_w_data_len != NULL)
					*p_w_data_len = conv_ret;

				*status = rpc_s_ok;
			}
			else
			{
				stub_conversion (
					h,
					RPC_BINDING_IS_SERVER (bind_p),
					tags_p->client_tag,
					tag,
					byte_ldata,
					conv_ret,
					wdata,
					p_w_data_len,
					status );
			}
			break;

		default:
			*status = rpc_s_ss_incompatible_codesets;
			break;
		}
	}
	else	/* server side */
	{
		/*
		 * Get the code set info from the current locale.
		 */
#if __hp9000s800
        	current_codeset = dce_langinfo(CODESET);
#else
           	current_codeset = nl_langinfo(CODESET);  /* broken on 9.0 UX */
#endif
		dce_cs_loc_to_rgy(
			(unsigned_char_p_t)current_codeset, 
			&current_rgy_codeset, 
			NULL, NULL,
			status);

		if (*status != dce_cs_c_ok)
		{
			/* codeset registry error */
			*status = rpc_s_ok;
			RPC_MEM_FREE(byte_ldata, RPC_C_MEM_STRING);
			return;
		}
		/*
		 * Determine the conversion type.
		 */
		if (tag == current_rgy_codeset)	
				/* No conversion required */
		{
			wdata_temp = wdata;
			ldata_temp = byte_ldata;
			for (i=0; i<= conv_ret; i++)
				*wdata_temp++ = *ldata_temp++;

			if (p_w_data_len != NULL)
				*p_w_data_len = conv_ret;

			*status = rpc_s_ok;
		}
		else
		{
			stub_conversion (
				h,
				RPC_BINDING_IS_SERVER (bind_p),
				current_rgy_codeset,
				tag,
				byte_ldata,
				conv_ret,
				wdata,
				p_w_data_len,
				status );
		}
	}
	RPC_MEM_FREE(byte_ldata, RPC_C_MEM_STRING);
	return;
}


/*
**++
**  ROUTINE NAME:       cs_byte_from_netcs
**
**  SCOPE:              PUBLIC - declared in dce/codesets_stub.h
**
**  DESCRIPTION:        
**
**  Convert an encoding of a network data to local encoding, based on
**  the information from a tag.
**
**  INPUTS:             
**
**      h		Binding handle
**
**      tag		Identifies the code set that will be used on the wire.
**
**	wdata		Address of the network data.
**
**      w_data_len	The number of "byte" data elements to be processed.
**
**  OUTPUTS:            
**
**	ldata		Address to which the converted data is to be written.
**
**	p_l_data_len	NULL if fixed array is being marshalled.
**			The address to which the routine writes the
**			local data length.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void cs_byte_from_netcs
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	unsigned32		tag,
	idl_byte		*wdata,
	unsigned32		w_data_len, 
	unsigned32		l_storage_len, 
	idl_byte		*ldata, 
	unsigned32		*p_l_data_len, 
	error_status_t		*status
)
#else
(h, tag, wdata, w_data_len, l_storage_len, ldata, p_l_data_len, status)
	rpc_binding_handle_t	h;
	unsigned32		tag;
	idl_byte		*wdata;
	unsigned32		w_data_len;
	unsigned32		l_storage_len;
	idl_byte		*ldata;
	unsigned32		*p_l_data_len;
	error_status_t		*status;
#endif
{
	char			*current_codeset;
	unsigned32		current_rgy_codeset;
	int			i;
	idl_byte		*wdata_temp, *ldata_temp;
	
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	rpc_binding_rep_p_t 	bind_p;

	bind_p = (rpc_binding_rep_p_t)h;
	if (!RPC_BINDING_IS_SERVER (bind_p))
	{
		switch (bind_p->cs_eval.key)
		{
		case RPC_CS_EVAL_METHOD:
			method_p = &bind_p->cs_eval.tagged_union.method_key;

			switch (method_p->method)
			{
			case RPC_EVAL_NO_CONVERSION:
			case RPC_EVAL_SMIR_MODEL:
				wdata_temp = wdata;
				ldata_temp = ldata;
				for (i=0; i<= w_data_len; i++)
					*wdata_temp++ = *ldata_temp++;

				if (p_l_data_len != NULL)
				{
					*p_l_data_len = strlen((char *)wdata) + 1;
				}

				*status = rpc_s_ok;
				break;

			case RPC_EVAL_CMIR_MODEL:
			case RPC_EVAL_RMIR_MODEL:
			case RPC_EVAL_INTERMEDIATE_MODEL:
			case RPC_EVAL_UNIVERSAL_MODEL:
				stub_conversion (
					h,
					RPC_BINDING_IS_SERVER (bind_p),
					tag,
					method_p->tags.client_tag,
					wdata,
					w_data_len,
					ldata,
					p_l_data_len,
					status );

				if (p_l_data_len != NULL)
				{
					*p_l_data_len = strlen((char *)wdata) + 1;
				}

				break;
	
			default:
				*status = rpc_s_ss_incompatible_codesets;
				break;

			}
			break;

		case RPC_CS_EVAL_TAGS:
			tags_p = &bind_p->cs_eval.tagged_union.tags_key;

			/*
			 * Determine the conversion type.
			 */
			if (tag == tags_p->client_tag)	
					/* No conversion required */
			{
				wdata_temp = wdata;
				ldata_temp = ldata;
				for (i=0; i<= w_data_len; i++)
					*wdata_temp++ = *ldata_temp++;

				if (p_l_data_len != NULL)
				{
					*p_l_data_len = strlen((char *)wdata) + 1;
				}

				*status = rpc_s_ok;
			}
			else
			{
				stub_conversion (
					h,
					RPC_BINDING_IS_SERVER (bind_p),
					tag,
					tags_p->client_tag,
					wdata,
					w_data_len,
					ldata,
					p_l_data_len,
					status );

				if (p_l_data_len != NULL)
				{
					*p_l_data_len = strlen((char *)wdata) + 1;
				}
			}
			break;

		default:
			*status = rpc_s_ss_incompatible_codesets;
			break;
		}
	}
	else	/* server side */
	{
		/*
		 * Get the code set info from the current locale.
		 */
#if __hp9000s800
        	current_codeset = dce_langinfo(CODESET);
#else
           	current_codeset = nl_langinfo(CODESET);  /* broken on 9.0 UX */
#endif
		dce_cs_loc_to_rgy(
			(unsigned_char_p_t)current_codeset, 
			&current_rgy_codeset, 
			NULL, NULL,
			status);

		if (*status != dce_cs_c_ok)
		{
			/* codeset registry error */
			*status = rpc_s_ok;
			return;
		}
		/*
		 * Determine the conversion type.
		 */
		if (tag == current_rgy_codeset)	
				/* No conversion required */
		{
			wdata_temp = wdata;
			ldata_temp = ldata;
			for (i=0; i<= w_data_len; i++)
				*wdata_temp++ = *ldata_temp++;

			if (p_l_data_len != NULL)
			{
				*p_l_data_len = strlen((char *)wdata) + 1;
			}

			*status = rpc_s_ok;
		}
		else
		{
			stub_conversion (
				h,
				RPC_BINDING_IS_SERVER (bind_p),
				tag,
				current_rgy_codeset,
				wdata,
				w_data_len,
				ldata,
				p_l_data_len,
				status );

			if (p_l_data_len != NULL)
			{
				*p_l_data_len = strlen((char *)wdata) + 1;
			}
		}
	}
	return;
}


/*
**++
**  ROUTINE NAME:       wchar_t_from_netcs
**
**  SCOPE:              PUBLIC - declared in dce/codesets_stub.h
**
**  DESCRIPTION:        
**
**  Convert an encoding of a network data to local encoding, based on
**  the information from a tag.
**
**  INPUTS:             
**
**      h		Binding handle
**
**      tag		Identifies the code set that will be used on the wire.
**
**	wdata		Address of the network data.
**
**      w_data_len	The number of "byte" data elements to be processed.
**
**  OUTPUTS:            
**
**	ldata		Address to which the converted data is to be written.
**
**	p_l_data_len	NULL if fixed array is being marshalled.
**			The address to which the routine writes the
**			local data length.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void wchar_t_from_netcs
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	unsigned32		tag,
	idl_byte		*wdata,
	unsigned32		w_data_len, 
	unsigned32		l_storage_len, 
	wchar_t			*ldata, 
	unsigned32		*p_l_data_len, 
	error_status_t		*status
)
#else
(h, tag, wdata, w_data_len, l_storage_len, ldata, p_l_data_len, status)
	rpc_binding_handle_t	h;
	unsigned32		tag;
	idl_byte		*wdata;
	unsigned32		w_data_len;
	unsigned32		l_storage_len;
	wchar_t			*ldata;
	unsigned32		*p_l_data_len;
	error_status_t		*status;
#endif
{
	char			*current_codeset;
	unsigned32		current_rgy_codeset;
	idl_byte		*byte_wdata;
	idl_byte		*t_byte_wdata;
	size_t			wdata_length;
	size_t			conv_length;
	int			i;
	idl_byte		*ldata_temp;
	
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	rpc_binding_rep_p_t 	bind_p;

	bind_p = (rpc_binding_rep_p_t)h;
	if (!RPC_BINDING_IS_SERVER (bind_p))
	{
		switch (bind_p->cs_eval.key)
		{
		case RPC_CS_EVAL_METHOD:
			method_p = &bind_p->cs_eval.tagged_union.method_key;

			switch (method_p->method)
			{
			case RPC_EVAL_NO_CONVERSION:
			case RPC_EVAL_SMIR_MODEL:

        			wdata_length = mbstowcs(ldata, (char *)wdata, (size_t)w_data_len);
				if (wdata_length == -1)         
					/* Conversion error took place */
					*status = rpc_s_ss_invalid_char_input;
				else
					*status = rpc_s_ok;
						
				if (p_l_data_len != NULL)
					*p_l_data_len = wdata_length;
				break;

			case RPC_EVAL_CMIR_MODEL:
			case RPC_EVAL_RMIR_MODEL:
			case RPC_EVAL_INTERMEDIATE_MODEL:
			case RPC_EVAL_UNIVERSAL_MODEL:

				RPC_MEM_ALLOC (
					byte_wdata,
					unsigned_char_p_t,
					w_data_len,
					RPC_C_MEM_STRING,
					RPC_C_MEM_WAITOK);
				if (byte_wdata == NULL){
				    *status = rpc_s_no_memory;
				    return;
				}

				/* initialize the buffer */
				t_byte_wdata = byte_wdata;
				i = l_storage_len;
				while (i--)
					*t_byte_wdata++ = L'\0';

				stub_conversion (
					h,
					RPC_BINDING_IS_SERVER (bind_p),
					tag,
					method_p->tags.client_tag,
					wdata,
					w_data_len,
					byte_wdata,	/* ldata, */
					(unsigned32 *)&wdata_length, /* p_l_data_len,*/
					status );

				wdata_length = mbstowcs(ldata, (char *)byte_wdata, (size_t)w_data_len);
				if (wdata_length == -1)         
					/* Conversion error took place */
					*status = rpc_s_ss_invalid_char_input;
				else
				{
					if (p_l_data_len != NULL)
						*p_l_data_len = wdata_length;

					*status = rpc_s_ok;
				}

				RPC_MEM_FREE(byte_wdata, RPC_C_MEM_STRING);

				break;
	
			default:
				*status = rpc_s_ss_incompatible_codesets;
				break;

			}
			break;

		case RPC_CS_EVAL_TAGS:
			tags_p = &bind_p->cs_eval.tagged_union.tags_key;

			if (tag == tags_p->client_tag)	
					/* No conversion required */
			{
        			wdata_length = mbstowcs(ldata, (char *)wdata, (size_t)w_data_len);
				if (wdata_length == -1)         
					/* Conversion error took place */
					*status = rpc_s_ss_invalid_char_input;
				else
					*status = rpc_s_ok;

				if (p_l_data_len != NULL)
					*p_l_data_len = wdata_length;
			}
			else
			{
				RPC_MEM_ALLOC (
					byte_wdata,
					unsigned_char_p_t,
					w_data_len,
					RPC_C_MEM_STRING,
					RPC_C_MEM_WAITOK);
				if (byte_wdata == NULL){
				    *status = rpc_s_no_memory;
				    return;
				}

				/* initialize the buffer */
				t_byte_wdata = byte_wdata;
				i = w_data_len;
				while (i--)
					*t_byte_wdata++ = L'\0';

				stub_conversion (
					h,
					RPC_BINDING_IS_SERVER (bind_p),
					tag,
					tags_p->client_tag,
					wdata,
					w_data_len,
					byte_wdata,	/* ldata, */
					(unsigned32 *)&wdata_length, /* p_l_data_len,*/
					status );

				wdata_length = mbstowcs(ldata, (char *)byte_wdata, (size_t)w_data_len);
				if (wdata_length == -1)         
					/* Conversion error took place */
					*status = rpc_s_ss_invalid_char_input;
				else
				{
					if (p_l_data_len != NULL)
						*p_l_data_len = wdata_length;

					*status = rpc_s_ok;
				}

				RPC_MEM_FREE(byte_wdata, RPC_C_MEM_STRING);

			}
			break;

		default:
			*status = rpc_s_ss_incompatible_codesets;
			break;
		}
	}
	else	/* server side */
	{
		/*
		 * Get the code set info from the current locale.
		 */
#if __hp9000s800
        	current_codeset = dce_langinfo(CODESET);
#else
           	current_codeset = nl_langinfo(CODESET);  /* broken on 9.0 UX */
#endif
		dce_cs_loc_to_rgy(
			(unsigned_char_p_t)current_codeset, 
			&current_rgy_codeset, 
			NULL, NULL,
			status);

		if (*status != dce_cs_c_ok)
		{
			/* codeset registry error */
			*status = rpc_s_ok;
			return;
		}
		/*
		 * Determine the conversion type.
		 */
		if (tag == current_rgy_codeset)	
				/* No conversion required */
		{
        		wdata_length = mbstowcs(ldata, (char *)wdata, (size_t)w_data_len);
			if (wdata_length == -1)         
				/* Conversion error took place */
				*status = rpc_s_ss_invalid_char_input;
			else
			{
				if (p_l_data_len != NULL)
					*p_l_data_len = wdata_length;

				*status = rpc_s_ok;
			}
		}
		else
		{
			RPC_MEM_ALLOC (
				byte_wdata,
				unsigned_char_p_t,
				w_data_len,
				RPC_C_MEM_STRING,
				RPC_C_MEM_WAITOK);
			if (byte_wdata == NULL){
			    *status = rpc_s_no_memory;
			    return;
			}

			/* initialize the buffer */
			t_byte_wdata = byte_wdata;
			i = w_data_len;
			while (i--)
				*t_byte_wdata++ = L'\0';

			stub_conversion (
				h,
				RPC_BINDING_IS_SERVER (bind_p),
				tag,
				current_rgy_codeset,
				wdata,
				w_data_len,
				byte_wdata,	/* ldata, */
				(unsigned32 *)&wdata_length, /* p_l_data_len,*/
				status );

			wdata_length = mbstowcs(ldata, (char *)byte_wdata, (size_t)w_data_len);
			if (wdata_length == -1)         
				/* Conversion error took place */
				*status = rpc_s_ss_invalid_char_input;
			else
			{
				if (p_l_data_len != NULL)
					*p_l_data_len = wdata_length;

				*status = rpc_s_ok;
			}

			RPC_MEM_FREE(byte_wdata, RPC_C_MEM_STRING);
		}
	}
	return;
}


/*
**++
**  ROUTINE NAME:       cs_byte_net_size
**
**  SCOPE:              PUBLIC - declared in dce/codesets_stub.h
**
**  DESCRIPTION:        
**
**  Calculate the necessary buffer size for code set converesion, based on
**  the information from a binding handle, tag, and local storage size.
**
**  INPUTS:             
**
**      h		Binding handle
**
**      tag		Identifies the code set that will be used on the wire.
**			When the caller is a client stub, this is cs_stag value.
**			When the caller is a server stub, this is cs_rtag value.
**
**      l_storage_len	The size, in units of byte, of the local storage
**			allocated for the I-char data.  This is the local value
**			of the size_is variable for the array.
**
**  OUTPUTS:            
**
**	p_convert_type	Indicate whether data conversion is necessary.
**			In case of  idl_cs_in_place_convert, l_storage_len 
**			is assumed to be sufficient for the buffer size.
**
**	p_w_storage_len	NULL if fixed or varying array is being marshalled.
**			When conformant or conformant varying array is being
**			marshalled, and p_convert_type is idl_cs_new_buffer
**			convert, this is the storage size in units of idl_byte.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void cs_byte_net_size
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	unsigned32		tag,		/* wire encoding */
	unsigned32		l_storage_len, 
	idl_cs_convert_t	*p_convert_type, 
	unsigned32		*p_w_storage_len, 
	error_status_t		*status
)
#else
(h, tag, l_storage_len, p_convert_type, p_w_storage_len, status)
	rpc_binding_handle_t	h;
	unsigned32		tag;		/* wire encoding */
	unsigned32		l_storage_len;
	idl_cs_convert_t	*p_convert_type;
	unsigned32		*p_w_storage_len;
	error_status_t		*status;
#endif
{
	char			*current_codeset;
	unsigned32		current_rgy_codeset;
	unsigned16		stag_bytes;
	unsigned16		client_bytes;
	
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	rpc_binding_rep_p_t 	bind_p;


	bind_p = (rpc_binding_rep_p_t)h;
	if (!RPC_BINDING_IS_SERVER (bind_p))
	{
		switch (bind_p->cs_eval.key)
		{
		case RPC_CS_EVAL_METHOD:
			method_p = &bind_p->cs_eval.tagged_union.method_key;

			if (method_p->tags.type_handle != NULL)
			{
				if ((idl_cs_convert_t)method_p->tags.type_handle 
						==  idl_cs_no_convert ||
				    (idl_cs_convert_t)method_p->tags.type_handle 
						==  idl_cs_in_place_convert)
				{

					*p_convert_type = (idl_cs_convert_t)
							method_p->tags.type_handle;
					if (p_w_storage_len != NULL)
				   		*p_w_storage_len = l_storage_len;
					*status = rpc_s_ok;

				}
				else if ((idl_cs_convert_t)
					method_p->tags.type_handle 
						==  idl_cs_new_buffer_convert)
				{
					*p_convert_type = (idl_cs_convert_t)
							method_p->tags.type_handle;

					if (p_w_storage_len != NULL)
					   *p_w_storage_len 
						= l_storage_len * 
						     method_p->tags.stag_max_bytes;
					*status = rpc_s_ok;

				}
			}
			else
			{
				switch (method_p->method)
				{
				case RPC_EVAL_NO_CONVERSION:
				case RPC_EVAL_RMIR_MODEL:
				case RPC_EVAL_SMIR_MODEL:
					*p_convert_type = idl_cs_no_convert;
					if (p_w_storage_len != NULL)
					   *p_w_storage_len = l_storage_len;

					if (method_p->fixed) 
					    method_p->tags.type_handle 
						= (rpc_ns_handle_t)*p_convert_type;

					*status = rpc_s_ok;
					break;

				case RPC_EVAL_CMIR_MODEL:
				case RPC_EVAL_INTERMEDIATE_MODEL:
				case RPC_EVAL_UNIVERSAL_MODEL:

					if ((method_p->fixed) &&
					    (method_p->tags.stag_max_bytes ==
					       method_p->tags.client_max_bytes))
					{
						*p_convert_type = 
							idl_cs_in_place_convert;
					        if (p_w_storage_len != NULL)
						     *p_w_storage_len = l_storage_len;
					}
					else if ( method_p->tags.stag_max_bytes ==
					       method_p->client->codesets[0].c_max_bytes)
					{
						*p_convert_type = 
							idl_cs_in_place_convert;
					        if (p_w_storage_len != NULL)
						     *p_w_storage_len = l_storage_len;
					}
					else
					{
					    *p_convert_type 
					        = idl_cs_new_buffer_convert;
					    if (p_w_storage_len != NULL)
					    	*p_w_storage_len = l_storage_len 
						      * method_p->tags.stag_max_bytes;
					}
					*status = rpc_s_ok;

					if (method_p->fixed)
					    method_p->tags.type_handle 
					     = (rpc_ns_handle_t)*p_convert_type;
					break;
	
				default:
					*status = rpc_s_ss_incompatible_codesets;
					break;

				}
			}
			break;

		case RPC_CS_EVAL_TAGS:
			tags_p = &bind_p->cs_eval.tagged_union.tags_key;

			if (tags_p->type_handle != NULL)
			{
				if ((idl_cs_convert_t)tags_p->type_handle 
							== idl_cs_no_convert ||
				(idl_cs_convert_t)tags_p->type_handle 
							== idl_cs_in_place_convert)
				{
					*p_convert_type = (idl_cs_convert_t)
							    tags_p->type_handle;

					if (p_w_storage_len != NULL)
				   		*p_w_storage_len = l_storage_len;
					*status = rpc_s_ok;

				}
				else if ((idl_cs_convert_t)tags_p->type_handle 
						== idl_cs_new_buffer_convert)
				{
					*p_convert_type = (idl_cs_convert_t)
							tags_p->type_handle;

					if (p_w_storage_len != NULL)
					   *p_w_storage_len = 
					       l_storage_len * tags_p->stag_max_bytes;

					*status = rpc_s_ok;
				}
			}
			else
			{
				/*
				 * Determine the conversion type.
				 */
				if (tag == tags_p->client_tag)	
						/* No conversion required */
				{
					*p_convert_type = idl_cs_no_convert;
					if (p_w_storage_len != NULL)
						*p_w_storage_len = l_storage_len;
				}
				else
				{
					if (tags_p->stag_max_bytes ==
						tags_p->client_max_bytes)

					{
						*p_convert_type = 
							idl_cs_in_place_convert;

					     if (p_w_storage_len != NULL)
						     *p_w_storage_len = l_storage_len;
					}
					else
					{
						*p_convert_type =
							idl_cs_new_buffer_convert;
						if (p_w_storage_len != NULL)
					    	    *p_w_storage_len = 
							l_storage_len * tags_p->stag_max_bytes;
					}

				}
				*status = rpc_s_ok;

				tags_p->type_handle = (rpc_ns_handle_t)
							*p_convert_type;
			}
			break;

		default:
			*status = rpc_s_ss_incompatible_codesets;
			break;
		}
	}
	else	/* Server side */
	{
		/*
		 * Get the code set info from the current locale.
		 */
#if __hp9000s800
        	current_codeset = dce_langinfo(CODESET);
#else
           	current_codeset = nl_langinfo(CODESET);  /* broken on 9.0 UX */
#endif
		dce_cs_loc_to_rgy(
			(unsigned_char_p_t)current_codeset, 
			&current_rgy_codeset, 
			NULL, NULL,
			status);

		if (*status != dce_cs_c_ok)
		{
			/* codeset registry error */
			*status = rpc_s_ok;
			return;
		}

		/*
		 * Determine the conversion type.
		 */
		if (tag == current_rgy_codeset)	
				/* No conversion required */
		{
			*p_convert_type = idl_cs_no_convert;
			if (p_w_storage_len != NULL)
				*p_w_storage_len = l_storage_len;

			*status = rpc_s_ok;
		}
		else
		{
			rpc_rgy_get_max_bytes (
				tag,
				&stag_bytes,
				status );

			if (*status != rpc_s_ok)
				return;

			rpc_rgy_get_max_bytes (
				current_rgy_codeset,
				&client_bytes,
				status );

			if (*status != rpc_s_ok)
				return;

			if (stag_bytes == client_bytes)
			{
				*p_convert_type = idl_cs_in_place_convert;

				if (p_w_storage_len != NULL)
				     *p_w_storage_len = l_storage_len;
			}
			else
			{
				*p_convert_type = idl_cs_new_buffer_convert;

				if (p_w_storage_len != NULL)
				    *p_w_storage_len = l_storage_len * stag_bytes;
			}

			*status = rpc_s_ok;
		}
	}
	return;
}


/*
**++
**  ROUTINE NAME:       wchar_t_net_size
**
**  SCOPE:              PUBLIC - declared in dce/codesets_stub.h
**
**  DESCRIPTION:        
**
**  Calculate the necessary buffer size for code set converesion, based on
**  the information from a binding handle, tag, and local storage size.
**
**  INPUTS:             
**
**      h		Binding handle
**
**      tag		Identifies the code set that will be used on the wire.
**			When the caller is a client stub, this is cs_stag value.
**			When the caller is a server stub, this is cs_rtag value.
**
**      l_storage_len	The size, in units of byte, of the local storage
**			allocated for the I-char data.  This is the local value
**			of the size_is variable for the array.
**
**  OUTPUTS:            
**
**	p_convert_type	Indicate whether data conversion is necessary.
**			In case of  idl_cs_in_place_convert, l_storage_len 
**			is assumed to be sufficient for the buffer size.
**
**	p_w_storage_len	NULL if fixed or varying array is being marshalled.
**			When conformant or conformant varying array is being
**			marshalled, and p_convert_type is idl_cs_new_buffer
**			convert, this is the storage size in units of idl_byte.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

/*
**  We calculate the buffer size by multiplying the number of wchar_t by
**  the maximum bytes of wire encoding.
*/

PUBLIC void wchar_t_net_size
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	unsigned32		tag,			/* wire encoding */
	unsigned32		l_storage_len,		/* wchar_t length */ 
	idl_cs_convert_t	*p_convert_type, 
	unsigned32		*p_w_storage_len, 
	error_status_t		*status
)
#else
(h, tag, l_storage_len, p_convert_type, p_w_storage_len, status)
	rpc_binding_handle_t	h;
	unsigned32		tag;			/* wire encoding */
	unsigned32		l_storage_len;		/* wchar_t length */ 
	idl_cs_convert_t	*p_convert_type;
	unsigned32		*p_w_storage_len;
	error_status_t		*status;
#endif
{
	char			*current_codeset;
	unsigned32		current_rgy_codeset;
	unsigned16		stag_bytes;
	unsigned16		client_bytes;
	
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	rpc_binding_rep_p_t 	bind_p;


	bind_p = (rpc_binding_rep_p_t)h;
	if (!RPC_BINDING_IS_SERVER (bind_p))
	{
		switch (bind_p->cs_eval.key)
		{
		case RPC_CS_EVAL_METHOD:
			method_p = &bind_p->cs_eval.tagged_union.method_key;

			if (method_p->tags.type_handle != NULL)
			{
				*p_convert_type = (idl_cs_convert_t)
						method_p->tags.type_handle;

				if (p_w_storage_len != NULL)
				   *p_w_storage_len = l_storage_len
					     		* sizeof(wchar_t);
				*status = rpc_s_ok;

			}
			else
			{
				switch (method_p->method)
				{
				case RPC_EVAL_NO_CONVERSION:
				case RPC_EVAL_RMIR_MODEL:
				case RPC_EVAL_SMIR_MODEL:
				case RPC_EVAL_CMIR_MODEL:
				case RPC_EVAL_INTERMEDIATE_MODEL:
				case RPC_EVAL_UNIVERSAL_MODEL:

					*p_convert_type = idl_cs_new_buffer_convert;

					if (p_w_storage_len != NULL)
					    	*p_w_storage_len = l_storage_len 
						      * sizeof(wchar_t);

					*status = rpc_s_ok;

					if (method_p->fixed)
					   method_p->tags.type_handle 
						= (rpc_ns_handle_t)*p_convert_type;
					break;
	
				default:
					*status = rpc_s_ss_incompatible_codesets;
					break;

				}
			}
			break;

		case RPC_CS_EVAL_TAGS:
			tags_p = &bind_p->cs_eval.tagged_union.tags_key;

			if (tags_p->type_handle != NULL)
			{
				*p_convert_type = (idl_cs_convert_t)tags_p->type_handle;

				if (p_w_storage_len != NULL)
				   *p_w_storage_len = l_storage_len 
							* sizeof(wchar_t);

				*status = rpc_s_ok;
			}
			else
			{
				/*
				 * Determine the conversion type.
				 */
				*p_convert_type = idl_cs_new_buffer_convert;
				if (p_w_storage_len != NULL)
			    		*p_w_storage_len = l_storage_len * sizeof(wchar_t);
				*status = rpc_s_ok;

				tags_p->type_handle = 
					(rpc_ns_handle_t)*p_convert_type;
			}
			break;

		default:
			*status = rpc_s_ss_incompatible_codesets;
			break;
		}
	}
	else	/* Server side */
	{
		*p_convert_type = idl_cs_new_buffer_convert;

		if (p_w_storage_len != NULL)
		    *p_w_storage_len = l_storage_len * sizeof(wchar_t);

		*status = rpc_s_ok;
	}
	return;
}


/*
**++
**  ROUTINE NAME:       cs_byte_local_size
**
**  SCOPE:              PUBLIC - declared in dce/codesets_stub.h
**
**  DESCRIPTION:        
**
**  Calculate the necessary buffer size for code set converesion, based on
**  the information from a binding handle, tag, and on-the_wire storage size.
**
**  INPUTS:             
**
**      h		Binding handle
**
**      tag		Identifies the code set that will be used on the wire.
**			When the caller is a client stub, this is cs_stag value.
**			When the caller is a server stub, this is cs_rtag value.
**
**      w_storage_len	The size, in units of byte, of the on-the_wire storage
**			allocated for the I-char data.  
**
**  OUTPUTS:            
**
**	p_convert_type	Indicate whether data conversion is necessary.
**			In case of  idl_cs_in_place_convert, w_storage_len 
**			is assumed to be sufficient for the buffer size.
**
**	p_l_storage_len	NULL if fixed or varying array is being marshalled.
**			When conformant or conformant varying array is being
**			marshalled, and p_convert_type is idl_cs_new_buffer
**			convert, this is the storage size in units of idl_byte.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void cs_byte_local_size
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	unsigned32		tag,		/* wire encoding */
	unsigned32		w_storage_len, 
	idl_cs_convert_t	*p_convert_type, 
	unsigned32		*p_l_storage_len, 
	error_status_t		*status
)
#else
(h, tag, w_storage_len, p_convert_type, p_l_storage_len, status)
	rpc_binding_handle_t	h;
	unsigned32		tag;		/* wire encoding */
	unsigned32		w_storage_len;
	idl_cs_convert_t	*p_convert_type;
	unsigned32		*p_l_storage_len;
	error_status_t		*status;
#endif
{
	char			*current_codeset;
	unsigned32		current_rgy_codeset;
	unsigned16		stag_bytes;
	unsigned16		client_bytes;
	
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	rpc_binding_rep_p_t 	bind_p;


	bind_p = (rpc_binding_rep_p_t)h;
	if (!RPC_BINDING_IS_SERVER (bind_p))
	{
		switch (bind_p->cs_eval.key)
		{
		case RPC_CS_EVAL_METHOD:
			method_p = &bind_p->cs_eval.tagged_union.method_key;

			if (method_p->tags.type_handle != NULL)
			{
				if ((idl_cs_convert_t)method_p->tags.type_handle 
						==  idl_cs_no_convert ||
				    (idl_cs_convert_t)method_p->tags.type_handle 
						==  idl_cs_in_place_convert)
				{

					*p_convert_type = (idl_cs_convert_t)
						method_p->tags.type_handle;

					if (p_l_storage_len != NULL)
				   		*p_l_storage_len = w_storage_len;
					*status = rpc_s_ok;

				}
				else if ((idl_cs_convert_t)method_p->tags.type_handle 
						==  idl_cs_new_buffer_convert)
				{
					*p_convert_type = (idl_cs_convert_t)
						method_p->tags.type_handle;

					if (p_l_storage_len != NULL)
					   *p_l_storage_len = w_storage_len 
						* method_p->tags.stag_max_bytes;

					*status = rpc_s_ok;

				}
			}
			else
			{
				switch (method_p->method)
				{
				case RPC_EVAL_NO_CONVERSION:
				case RPC_EVAL_SMIR_MODEL:
				case RPC_EVAL_CMIR_MODEL:
				case RPC_EVAL_RMIR_MODEL:
				case RPC_EVAL_INTERMEDIATE_MODEL:
				case RPC_EVAL_UNIVERSAL_MODEL:

					if ((method_p->fixed) &&
					    (method_p->tags.stag_max_bytes ==
					    method_p->tags.client_max_bytes))
					{
					     *p_convert_type = idl_cs_in_place_convert;

					     if (p_l_storage_len != NULL)
						  *p_l_storage_len = w_storage_len;
					}
					else if (method_p->tags.stag_max_bytes ==
						method_p->client->codesets[0].c_max_bytes)
					{
					     *p_convert_type = idl_cs_in_place_convert;

					     if (p_l_storage_len != NULL)
						  *p_l_storage_len = w_storage_len;
					}
					else
					{
					    *p_convert_type = idl_cs_new_buffer_convert;
					    if (p_l_storage_len != NULL)
					    	*p_l_storage_len = w_storage_len 
						     * method_p->tags.stag_max_bytes;
					}
					*status = rpc_s_ok;

					if (method_p->fixed)
						method_p->tags.type_handle = 
						(rpc_ns_handle_t)*p_convert_type;
					break;
	
				default:
					*status = rpc_s_ss_incompatible_codesets;
					break;

				}
			}
			break;

		case RPC_CS_EVAL_TAGS:
			tags_p = &bind_p->cs_eval.tagged_union.tags_key;

			if (tags_p->type_handle != NULL)
			{
				if ((idl_cs_convert_t)tags_p->type_handle 
						== idl_cs_no_convert ||
				(idl_cs_convert_t)tags_p->type_handle 
						== idl_cs_in_place_convert)
				{
					*p_convert_type = 
					     (idl_cs_convert_t)tags_p->type_handle;
					if (p_l_storage_len != NULL)
				   		*p_l_storage_len = w_storage_len;
					*status = rpc_s_ok;

				}
				else if ((idl_cs_convert_t)tags_p->type_handle 
						== idl_cs_new_buffer_convert)
				{
					*p_convert_type = 
					     (idl_cs_convert_t)tags_p->type_handle;
					if (p_l_storage_len != NULL)
					   *p_l_storage_len  = 
						w_storage_len * tags_p->stag_max_bytes;
					*status = rpc_s_ok;
				}
			}
			else
			{
				/*
				 * Determine the conversion type.
				 */
				if (tag == tags_p->client_tag)	
						/* No conversion required */
				{
					*p_convert_type = idl_cs_no_convert;
					if (p_l_storage_len != NULL)
						*p_l_storage_len = w_storage_len;
				}
				else
				{
					if (tags_p->stag_max_bytes == 
						tags_p->client_max_bytes)
					{
						*p_convert_type 
						     = idl_cs_in_place_convert;

					     if (p_l_storage_len != NULL)
						     *p_l_storage_len = w_storage_len;
					}
					else
					{
						*p_convert_type 
						    = idl_cs_new_buffer_convert;

						if (p_l_storage_len != NULL)
					    	    *p_l_storage_len 
							= w_storage_len * tags_p->stag_max_bytes;
					}

				}
				*status = rpc_s_ok;

				tags_p->type_handle
					= (rpc_ns_handle_t) *p_convert_type;
			}
			break;

		default:
			*status = rpc_s_ss_incompatible_codesets;
			break;
		}
	}
	else	/* Server side */
	{
		/*
		 * Get the code set info from the current locale.
		 */
#if __hp9000s800
        	current_codeset = dce_langinfo(CODESET);
#else
           	current_codeset = nl_langinfo(CODESET);  /* broken on 9.0 UX */
#endif
		dce_cs_loc_to_rgy(
			(unsigned_char_p_t)current_codeset, 
			&current_rgy_codeset, 
			NULL, NULL,
			status);

		if (*status != dce_cs_c_ok)
		{
			/* codeset registry error */
			*status = rpc_s_ok;
			return;
		}

		/*
		 * Determine the conversion type.
		 */
		if (tag == current_rgy_codeset)	
				/* No conversion required */
		{
			*p_convert_type = idl_cs_no_convert;

			if (p_l_storage_len != NULL)
				*p_l_storage_len = w_storage_len;

			*status = rpc_s_ok;
		}
		else
		{
			rpc_rgy_get_max_bytes (
				tag,
				&stag_bytes,
				status );

			if (*status != rpc_s_ok)
				return;

			rpc_rgy_get_max_bytes (
				current_rgy_codeset,
				&client_bytes,
				status );

			if (*status != rpc_s_ok)
				return;

			if (stag_bytes == client_bytes)
			{
			     *p_convert_type = idl_cs_in_place_convert;

			     if (p_l_storage_len != NULL)
				     *p_l_storage_len = w_storage_len;
			}
			else
			{
			     *p_convert_type = idl_cs_new_buffer_convert;

			     if (p_l_storage_len != NULL)
				    *p_l_storage_len = w_storage_len * stag_bytes;
			}

			*status = rpc_s_ok;
		}
	}
	return;
}


/*
**++
**  ROUTINE NAME:       wchar_t_local_size
**
**  SCOPE:              PUBLIC - declared in dce/codesets_stub.h
**
**  DESCRIPTION:        
**
**  Calculate the necessary buffer size for code set converesion, based on
**  the information from a binding handle, tag, and on-the_wire storage size.
**
**  INPUTS:             
**
**      h		Binding handle
**
**      tag		Identifies the code set that will be used on the wire.
**			When the caller is a client stub, this is cs_stag value.
**			When the caller is a server stub, this is cs_rtag value.
**
**      w_storage_len	The size, in units of byte, of the on-the_wire storage
**			allocated for the I-char data.  
**
**  OUTPUTS:            
**
**	p_convert_type	Indicate whether data conversion is necessary.
**			In case of  idl_cs_in_place_convert, w_storage_len 
**			is assumed to be sufficient for the buffer size.
**
**	p_l_storage_len	NULL if fixed or varying array is being marshalled.
**			When conformant or conformant varying array is being
**			marshalled, and p_convert_type is idl_cs_new_buffer
**			convert, this is the storage size in units of idl_byte.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void wchar_t_local_size
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	unsigned32		tag,			/* wire encoding */
	unsigned32		w_storage_len, 
	idl_cs_convert_t	*p_convert_type, 
	unsigned32		*p_l_storage_len,	/* wchar_t size */ 
	error_status_t		*status
)
#else
(h, tag, w_storage_len, p_convert_type, p_l_storage_len, status)
	rpc_binding_handle_t	h;
	unsigned32		tag;			/* wire encoding */
	unsigned32		w_storage_len;
	idl_cs_convert_t	*p_convert_type;
	unsigned32		*p_l_storage_len;	/* wchar_t size */ 
	error_status_t		*status;
#endif
{
	char			*current_codeset;
	unsigned32		current_rgy_codeset;
	unsigned16		stag_bytes;
	unsigned16		client_bytes;
	
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	rpc_binding_rep_p_t 	bind_p;


	bind_p = (rpc_binding_rep_p_t)h;
	if (!RPC_BINDING_IS_SERVER (bind_p))
	{
		switch (bind_p->cs_eval.key)
		{
		case RPC_CS_EVAL_METHOD:
			method_p = &bind_p->cs_eval.tagged_union.method_key;

			if (method_p->tags.type_handle != NULL)
			{
				*p_convert_type = (idl_cs_convert_t)
						method_p->tags.type_handle;

				if (p_l_storage_len != NULL)
				  	*p_l_storage_len = w_storage_len
							/ sizeof(wchar_t);

					*status = rpc_s_ok;
			}
			else
			{
				switch (method_p->method)
				{
				case RPC_EVAL_NO_CONVERSION:
				case RPC_EVAL_SMIR_MODEL:
				case RPC_EVAL_CMIR_MODEL:
				case RPC_EVAL_RMIR_MODEL:
				case RPC_EVAL_INTERMEDIATE_MODEL:
				case RPC_EVAL_UNIVERSAL_MODEL:

					*p_convert_type = idl_cs_new_buffer_convert;
					if (p_l_storage_len != NULL)
				  		*p_l_storage_len = 
						w_storage_len / sizeof(wchar_t);

					*status = rpc_s_ok;

					if (method_p->fixed)
					    method_p->tags.type_handle = 
						(rpc_ns_handle_t)*p_convert_type;
					break;
	
				default:
					*status = rpc_s_ss_incompatible_codesets;
					break;

				}
			}
			break;

		case RPC_CS_EVAL_TAGS:
			tags_p = &bind_p->cs_eval.tagged_union.tags_key;

			if (tags_p->type_handle != NULL)
			{
				*p_convert_type = 
				     (idl_cs_convert_t)tags_p->type_handle;

				if (p_l_storage_len != NULL)
			   		*p_l_storage_len = w_storage_len
							/ sizeof(wchar_t);

				*status = rpc_s_ok;
			}
			else
			{
				*p_convert_type = idl_cs_new_buffer_convert;

				if (p_l_storage_len != NULL)
					*p_l_storage_len = w_storage_len
							/ sizeof(wchar_t);

				*status = rpc_s_ok;

				tags_p->type_handle
					= (rpc_ns_handle_t) *p_convert_type;
			}
			break;

		default:
			*status = rpc_s_ss_incompatible_codesets;
			break;
		}
	}
	else	/* Server side */
	{
		*p_convert_type = idl_cs_new_buffer_convert;

		if (p_l_storage_len != NULL)
			*p_l_storage_len = w_storage_len / sizeof(wchar_t);

		*status = rpc_s_ok;
	}
	return;
}


/*
**++
**  ROUTINE NAME:       rpc_cs_get_tags
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Select codeset conversion tags based on the binding handle
**
**  INPUTS:             
**
**      h		Binding handle
**
**      server_side	boolean input to indicate if the caller is a server stub
**
**  OUTPUTS:            
**
**	p_stag		tag to indicate the sending codeset by a client
**			When the caller is a server stub, this is unused.
**
**	p_drtag		tag to indicate the desired received codeset.
**			When the caller is a server stub, this is an input
**			When the caller is a client stub, this is an output
**
**	p_rtag		tag to indicate the sending codeset by a server
**			When the caller is a client stub, this is unused.
**
**      status          The status of the operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_ss_incompatible_codesets
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_cs_get_tags
#ifdef _DCE_PROTO_
(
	rpc_binding_handle_t	h,
	idl_boolean		server_side,
	unsigned32		*p_stag, 
	unsigned32		*p_drtag, 
	unsigned32		*p_rtag, 
	error_status_t		*status
)
#else
(h, server_side, p_stag, p_drtag, p_rtag, status)
	rpc_binding_handle_t	h;
	idl_boolean		server_side;
	unsigned32		*p_stag;
	unsigned32		*p_drtag;
	unsigned32		*p_rtag;
	error_status_t		*status;
#endif
{
	rpc_binding_rep_p_t	bind_p;
	rpc_cs_method_eval_p_t	method_p;
	rpc_cs_tags_eval_p_t	tags_p;
	unsigned_char_t		*entry_name;
	int			i, j;
	rpc_codeset_mgmt_p_t	client, server;
	int			model_found, smir_true, cmir_true;
	long			i_code;
	int			i_max_bytes;
	error_status_t		temp_status;

	if (!server_side)
	{
		bind_p = (rpc_binding_rep_p_t)h;

		if (bind_p->extended_bind_flag == RPC_C_BH_EXTENDED_CODESETS)
		{
			/*
	 		 * Determine the data structure
	 		 */
			switch (bind_p->cs_eval.key)
			{
			case RPC_CS_EVAL_METHOD:
				method_p = &bind_p->cs_eval.tagged_union.method_key;

				if (method_p->fixed)
				{
					*p_stag = method_p->tags.stag;
					*p_drtag = method_p->tags.drtag;
				}
				else
				{
					/* 
					 * We will evaluate code set I14Y here
					 */
					 (*(method_p->cs_stub_eval_func))(p_stag, p_drtag, status);
					if (*status != rpc_s_ok)
						return;

				        method_p->tags.stag = *p_stag;
				        method_p->tags.drtag = *p_drtag;

					rpc_rgy_get_max_bytes (
						*p_stag,
						&method_p->tags.stag_max_bytes,
						status
                                        );

					if (*status != rpc_s_ok)
						return;

					/* Get client's supported code sets */
					rpc_rgy_get_codesets (
						&client,
						status );

					if (*status != rpc_s_ok)
						return;

				        method_p->tags.client_tag = client->codesets[0].c_set;
				        method_p->tags.client_max_bytes = client->codesets[0].c_max_bytes;
				        method_p->tags.type_handle = NULL;
				}

				break;

			case RPC_CS_EVAL_TAGS:
				tags_p = &bind_p->cs_eval.tagged_union.tags_key;

				*p_stag = tags_p->stag;
				*p_drtag = tags_p->drtag;

				break;

			default:
				*status = rpc_s_ss_invalid_codeset_tag;
			}

			*status = rpc_s_ok;
		}
		else
		{
			/* No evaluation is done so far, so we try to figure
			 * out if server and client are compatible
			 */

			/* Get the client's supported code sets */
			rpc_rgy_get_codesets (
				&client,
				status );

			if (*status != rpc_s_ok)
				return;

			/* Get the name of server entry in NSI */
			rpc_ns_binding_inq_entry_name (
				h,
				rpc_c_ns_syntax_default,
				&entry_name,
				status );

			if (*status != rpc_s_ok)
			{
				rpc_ns_mgmt_free_codesets(&client, &temp_status);
				return;
			}

			/* Get the server's supported code sets from NSI */
			rpc_ns_mgmt_read_codesets (
				rpc_c_ns_syntax_default,
				entry_name,
				&server,
				status );

			if (*status != rpc_s_ok)
			{
			     rpc_ns_mgmt_free_codesets(&client, &temp_status);
			     return;
			}

			/*  Start evaluation */
			if (client->codesets[0].c_set == server->codesets[0].c_set)
			{
			    /* client and server are using the same code set */
			    *p_stag = client->codesets[0].c_set;
			    *p_drtag = server->codesets[0].c_set;

			    tags_p->stag = *p_stag;
			    tags_p->drtag = *p_drtag;
			    tags_p->stag_max_bytes = client->codesets[0].c_max_bytes;
			    tags_p->client_tag = client->codesets[0].c_set;
			    tags_p->client_max_bytes = client->codesets[0].c_max_bytes;
			    tags_p->type_handle = NULL;
			}
			else
			{
			     /* Check character set compatibility first */
			     rpc_cs_char_set_compat_check (
				client->codesets[0].c_set,
				server->codesets[0].c_set,
				status );

			     if (*status != rpc_s_ok)
			     {
				rpc_ns_mgmt_free_codesets(&server, &temp_status);
				rpc_ns_mgmt_free_codesets(&client, &temp_status);
				return;
			     }

			     smir_true = cmir_true = model_found = 0;

			     for (i = 1; i <= server->count; i++)
			     {
				if (model_found)
					break;

				if (client->codesets[0].c_set
					== server->codesets[i].c_set)
				{
					smir_true = 1;
					model_found = 1;
				}

				if (server->codesets[0].c_set
					== client->codesets[i].c_set)
				{
					cmir_true = 1;
					model_found = 1;
				}
			     }

			     if (model_found)
			     {
				tags_p = &bind_p->cs_eval.tagged_union.tags_key;

				if (smir_true && cmir_true)
				{
					/* RMIR model works */
					*p_stag = client->codesets[0].c_set;
					*p_drtag = server->codesets[0].c_set;

					tags_p->stag = *p_stag;
					tags_p->drtag = *p_drtag;
					tags_p->stag_max_bytes = client->codesets[0].c_max_bytes;
					tags_p->client_tag = client->codesets[0].c_set;
					tags_p->client_max_bytes = client->codesets[0].c_max_bytes;
					tags_p->type_handle = NULL;
				}
				else if (smir_true)
				{
					/* SMIR model */
					*p_stag = client->codesets[0].c_set;
					*p_drtag = client->codesets[0].c_set;

					tags_p->stag = *p_stag;
					tags_p->drtag = *p_drtag;
					tags_p->stag_max_bytes = client->codesets[0].c_max_bytes;
					tags_p->client_tag = client->codesets[0].c_set;
					tags_p->client_max_bytes = client->codesets[0].c_max_bytes;
					tags_p->type_handle = NULL;
				}
				else
				{
					/* CMIR model */
					*p_stag = server->codesets[0].c_set;
					*p_drtag = server->codesets[0].c_set;

					tags_p->stag = *p_stag;
					tags_p->drtag = *p_drtag;
					tags_p->stag_max_bytes = server->codesets[0].c_max_bytes;
					tags_p->client_tag = client->codesets[0].c_set;
					tags_p->client_max_bytes = client->codesets[0].c_max_bytes;
					tags_p->type_handle = NULL;
				}
				*status = rpc_s_ok;
			     }
			     else
			     {
				/* Try to find the intermediate code set */
				tags_p->client_tag = client->codesets[0].c_set;
				tags_p->client_max_bytes = client->codesets[0].c_max_bytes;
				tags_p->type_handle = NULL;
				
				for (i = 1; i <= client->count; i++)
				{
				    if (model_found)
					break;

				    for (j = 1; j <= server->count; j++)
				    {
					if (client->codesets[i].c_set
                                                == server->codesets[j].c_set)
					{
						*p_stag = client->codesets[i].c_set;
						*p_drtag = client->codesets[i].c_set;
						tags_p->stag = *p_stag;
						tags_p->drtag = *p_drtag;
                                                tags_p->stag_max_bytes = client->codesets[i].c_max_bytes;
                                                model_found = 1;
                                                   break;
                                        }
                                    }
				}

				if (!model_found)
				{
				    /* We use UNIVERSAL code set */
				    *p_stag = UCS2_L2;
				    *p_drtag = UCS2_L2;
				    tags_p->stag = *p_stag;
				    tags_p->drtag = *p_drtag;
				    tags_p->stag_max_bytes = client->codesets[i].c_max_bytes;

				    rpc_rgy_get_max_bytes (
					UCS2_L2,
					&tags_p->stag_max_bytes,
					status
				    );
				}
			     }
			}

			rpc_ns_mgmt_free_codesets(&server, &temp_status);
			rpc_ns_mgmt_free_codesets(&client, &temp_status);
			bind_p->extended_bind_flag = RPC_C_BH_EXTENDED_CODESETS;
		}
	}
	else	/* server side */
	{
		if (p_rtag != p_drtag)
			*p_rtag = *p_drtag;

		*status = rpc_s_ok;
	}
	return;
}
