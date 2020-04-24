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
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/18  00:33:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:18:36  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:50:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:01  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:17:21  root]
 * 
 * Revision 1.1.2.5  1994/09/27  18:47:27  mori_m
 * 	CR 11041:  Modified conformant array operation.  In case of wchar_t array,
 * 		   size_is value cannot be trusted due to the architectual limitation.
 * 		   Used wcslen to figure out the exact length of wchar_t array.
 * 	[1994/09/27  18:34:32  mori_m]
 * 
 * Revision 1.1.2.4  1994/09/23  19:23:52  mori_m
 * 	Merged with changes from 1.1.2.3
 * 	[1994/09/23  19:23:47  mori_m]
 * 
 * 	CR 11041/11044: Deleted un-used code within the comments.
 * 	[1994/09/23  18:33:30  mori_m]
 * 
 * Revision 1.1.2.3  1994/09/23  18:43:41  mori_m
 * 	CR 11041/11044: Deleted un-used code within the comments.
 * 	[1994/09/23  18:33:30  mori_m]
 * 
 * Revision 1.1.2.2  1994/06/10  20:43:58  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:15:57  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:54:29  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:35:13  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>

#include "cs_test.h"
#include "check_status2.h"


error_status_t cs_fixed_trans (
	handle_t	h,
	wchar_t		in_string[SIZE],
	wchar_t		out_string[SIZE]
)
{
	int	i, w_len, k;

	w_len =  wcslen(in_string);

	k = w_len - 1;
	for (i = 0; i < w_len; i++)
		out_string[i] = in_string[k--]; 

	out_string[i] = L'\0';

	return(rpc_s_ok);
}

error_status_t cs_conf_trans (
	handle_t	h,
	idl_long_int	in_size,
	wchar_t		*in_string,
	wchar_t		*out_string
)
{
	int	i, k, s_size;
	wchar_t	*in_wcs, *out_wcs;

	s_size  = wcslen(in_string);
	in_wcs  = in_string;
	out_wcs = out_string;

	k = s_size - 1;
	in_wcs += k;
	for (i = 0; i < s_size; i++)
		*out_wcs++ = *in_wcs--; 

	*out_wcs = L'\0';

	return(rpc_s_ok);
}

error_status_t cs_conf_vary_trans (
	handle_t	h,
	idl_long_int	in_size,
	idl_long_int	in_length,
	wchar_t		*in_string,
	wchar_t		*out_string
)
{
	int	i, w_len, k;

	w_len =  wcslen(in_string);

	k = w_len - 1;
	for (i = 0; i < w_len; i++)
		out_string[i] = in_string[k--]; 

	out_string[i] = L'\0';

	return(rpc_s_ok);
}

error_status_t cs_vary_trans (
	handle_t	h,
	idl_long_int	in_length,
	wchar_t		in_string[SIZE],
	wchar_t		out_string[SIZE]
)
{
	int	i, w_len, k;

	w_len =  (int)in_length;

	k = w_len - 1;
	for (i = 0; i < w_len; i++)
		out_string[i] = in_string[k--]; 

	out_string[i] = L'\0';

	return(rpc_s_ok);
}
