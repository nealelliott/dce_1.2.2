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
 * Revision 1.1.4.2  1996/02/18  00:32:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:50  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:48:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:24  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:16:41  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:43:03  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:15:21  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:44  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:25:57  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>

#include "cs_test.h"
#include "check_status2.h"


error_status_t cs_fixed_trans (
	handle_t	h,
	net_byte	in_string[SIZE],
	net_byte	out_string[SIZE]
)
{
	int i, length, k;
	wchar_t wcs[SIZE];
	wchar_t wcs_rev[SIZE];


	length = (int)mbstowcs(wcs, (char *)in_string, SIZE);
	if (length == -1) {
		fprintf(stderr, "Error occurred in mbstowcs!!\n");
		fprintf(stderr, "TET_FAIL\n");
	}

	k = length - 1;
	for (i = 0; i<length; i++)
		wcs_rev[i] = wcs[k--]; 
	wcs_rev[i] = L'\0';

	length = (int)wcstombs((char *)out_string, wcs_rev, SIZE);
	if (length == -1) {
		fprintf(stderr, "Error occurred in wcstombs!!\n");
		fprintf(stderr, "TET_FAIL\n");
		return;
	}

	return(rpc_s_ok);
}

error_status_t cs_conf_trans (
	handle_t	h,
	idl_long_int	in_size,
	net_byte	*in_string,
	net_byte	*out_string
)
{
	int i, length, k, s_size;
	wchar_t *wcs;
	wchar_t *wcs_rev;

	s_size = (int)in_size;

	wcs     = (wchar_t *)malloc((s_size + 1) * MB_CUR_MAX);
	wcs_rev = (wchar_t *)malloc((s_size + 1) * MB_CUR_MAX);


	length = (int)mbstowcs(wcs, (char *)in_string, s_size);
	if (length == -1) {
		fprintf(stderr, "Error occurred in mbstowcs!!\n");
		fprintf(stderr, "TET_FAIL\n");
	}

	k = length - 1;
	for (i = 0; i<length; i++)
		wcs_rev[i] = wcs[k--]; 
	wcs_rev[i] = L'\0';

	k = (int)wcstombs((char *)out_string, wcs_rev, s_size);
	if (k == -1) {
		fprintf(stderr, "Error occurred in wcstombs!!\n");
		fprintf(stderr, "TET_FAIL\n");
		return;
	}

	free(wcs);
	free(wcs_rev);

	return(rpc_s_ok);
}

error_status_t cs_conf_vary_trans (
	handle_t	h,
	idl_long_int	in_size,
	idl_long_int	in_length,
	net_byte	*in_string,
	net_byte	*out_string
)
{
	int i, length, k, s_size;
	wchar_t *wcs;
	wchar_t *wcs_rev;

	s_size = (int)in_size;

	wcs     = (wchar_t *)malloc((s_size + 1) * MB_CUR_MAX);
	wcs_rev = (wchar_t *)malloc((s_size + 1) * MB_CUR_MAX);


	length = (int)mbstowcs(wcs, (char *)in_string, s_size);
	if (length == -1) {
		fprintf(stderr, "Error occurred in mbstowcs!!\n");
		fprintf(stderr, "TET_FAIL\n");
	}

	k = length - 1;
	for (i = 0; i<length; i++)
		wcs_rev[i] = wcs[k--]; 
	wcs_rev[i] = L'\0';

	k = (int)wcstombs((char *)out_string, wcs_rev, s_size);
	if (k == -1) {
		fprintf(stderr, "Error occurred in wcstombs!!\n");
		fprintf(stderr, "TET_FAIL\n");
		return;
	}

	free(wcs);
	free(wcs_rev);

	return(rpc_s_ok);
}

error_status_t cs_vary_trans (
	handle_t	h,
	idl_long_int	in_length,
	net_byte	*in_string,
	net_byte	*out_string
)
{
	int i, length, k, s_len;
	wchar_t wcs[SIZE];
	wchar_t wcs_rev[SIZE];

	for (i=0; i < SIZE; i++)
	{
		wcs[i] = L'\0';
		wcs_rev[i] = L'\0';
	}

	s_len = (int)in_length;


	length = (int)mbstowcs(wcs, (char *)in_string, s_len);
	if (length == -1) {
		fprintf(stderr, "Error occurred in mbstowcs!!\n");
		fprintf(stderr, "TET_FAIL\n");
	}

	k = length - 1;
	for (i = 0; i<length; i++)
		wcs_rev[i] = wcs[k--]; 

	length = (int)wcstombs((char *)out_string, wcs_rev, s_len);
	if (length == -1) {
		fprintf(stderr, "Error occurred in wcstombs!!\n");
		fprintf(stderr, "TET_FAIL\n");
		return;
	}

	return(rpc_s_ok);
}
