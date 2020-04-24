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
 * $Log: cs_s_conv.c,v $
 * Revision 1.1.4.2  1996/02/18  00:01:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:16:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:05  root]
 * 
 * Revision 1.1.2.4  1994/06/10  20:54:20  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:06  devsrc]
 * 
 * Revision 1.1.2.3  1994/04/12  16:51:57  mori_m
 * 	CR 9701:  Second submission for RPC runtime I18N support.
 * 	[1994/04/12  16:39:55  mori_m]
 * 
 * Revision 1.1.2.2  1994/02/09  21:12:17  mori_m
 * 	CR 9912:  Moved function definition to cs_s.h
 * 	[1994/02/09  21:11:57  mori_m]
 * 
 * Revision 1.1.2.1  1994/02/08  21:33:09  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:32:58  mori_m]
 * 
 * $EndLog$
 */
/*
**  NAME
**
**     cs_s_conv.c
**
**  FACILITY:
**
**     Remote Procedure Call (RPC)
**     I18N Character Set Conversion Call   (RPC)
**
**  ABSTRACT:
**
**
*/
#include <commonp.h>		/* include nbase.h lbase.h internally	*/
#include <com.h>		/* definition of rpc_binding_rep_p_t	*/
#include <dce/rpcsts.h>
#include <codesets.h>		/* Data definitions for I18N NSI 
							sub-component   */
#include <stdio.h>		/* definition of NULL			*/
#include <stdlib.h>		/* definition of MB_CUR_MAX		*/
#include <iconv.h>		/* definition of iconv routines		*/
#include <langinfo.h>		/* definition of nl_langinfo routine	*/
#include <string.h>		/* definition of strncpy routine	*/
#include <errno.h>		/* definition of error numbers 		*/

#include <codesets.h>
#include <cs_s.h>		/* Private defs for code set interoperability */


void stub_conversion
#ifdef _DCE_PROTOTYPE_
(
	rpc_binding_handle_t	h,
	boolean32		server_side,
	unsigned32		from_tag,
	unsigned32		to_tag,
	byte_t			*conv_ldata,
	unsigned32		conv_l_data_len,
	byte_t			*conv_wdata,
	unsigned32		*conv_p_w_data_len,
	error_status_t		*status
)
#else
(h, server_side, to_conversion, from_tag, to_tag, conv_ldata, conv_l_data_len,
                conv_wdata, conv_p_w_data_len, status)
	rpc_binding_handle_t	h;
	boolean32		server_side;
	unsigned32		from_tag;
	unsigned32		to_tag;
	byte_t			*conv_ldata;
	unsigned32		conv_l_data_len;
	byte_t			*conv_wdata;
	unsigned32		*conv_p_w_data_len;
	error_status_t		*status;
#endif
{
	iconv_t			cd;
	byte_t			*ldata = conv_ldata;
	byte_t			*wdata = conv_wdata;
	int			size;
	int			inbytesleft;
	int			outbytesleft;
	char			*iconv_from_cd;
	char			*iconv_to_cd;
	int			i_ret;
	int			init_len;


	dce_cs_rgy_to_loc (
		from_tag,
		(idl_char **)&iconv_from_cd,
		NULL,
		NULL,
		status );

	if (*status != dce_cs_c_ok)
		return;

	dce_cs_rgy_to_loc (
		to_tag,
		(idl_char **)&iconv_to_cd,
		NULL,
		NULL,
		status );

	if (*status != dce_cs_c_ok)
		return;

	if ((cd = iconv_open(iconv_to_cd, iconv_from_cd)) == (iconv_t)-1)
	{
		*status = rpc_s_ss_incompatible_codesets;
		return;
	}

	/* Set the number of bytes left in input buffer */
	init_len = strlen((char *)ldata);
	inbytesleft = init_len;
	outbytesleft = (int)conv_l_data_len * sizeof(unsigned_char_t);

	i_ret = iconv(cd, (char **)&ldata, &inbytesleft, (char **)&wdata, &outbytesleft);

	if (i_ret)	/* Iconv returns zero when it succeed */
	{
		if (errno == EILSEQ)
			*status = rpc_s_ss_invalid_char_input;
		else if (errno = E2BIG)
			*status = rpc_s_ss_short_conv_buffer;
		else if (errno = EINVAL)
			*status = rpc_s_ss_invalid_char_input;
		i_ret = iconv_close(cd);
		return;
	}
	*wdata = '\0';	/* Guard against a stale data */

	if ((i_ret = iconv_close(cd)) == -1)
	{
		*status = rpc_s_ss_iconv_error;
		return;
	}

	if (conv_p_w_data_len != NULL)
	{
		*conv_p_w_data_len = strlen((char *)conv_wdata);
	}

	*status = rpc_s_ok;
	return;
}
