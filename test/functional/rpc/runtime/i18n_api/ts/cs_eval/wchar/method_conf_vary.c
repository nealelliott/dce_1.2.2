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
 * $Log: method_conf_vary.c,v $
 * Revision 1.1.4.2  1996/02/18  00:33:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:18:38  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:50:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:03  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:17:23  root]
 * 
 * Revision 1.1.2.3  1994/09/27  18:47:29  mori_m
 * 	CR 11041:  'size_is' value should be (wcslen+1) * sizeof(wchar_t).
 * 	[1994/09/27  18:35:55  mori_m]
 * 
 * Revision 1.1.2.2  1994/09/23  19:30:51  mori_m
 * 	CR 11041/11044: New test for varying array operation.
 * 	[1994/09/23  18:33:59  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <locale.h>
#include <dce/rpc.h>
#include <tet_api.h>
#include <pthread.h>

#include "cs_test.h"		/* IDL generated include file */
#include "check_status.h"	/* error checking */
#include "get_env.h"		/* GET.. macros */

extern void     client();
extern void     start_server();
extern char     **environ;

/*
 * Initialize TCM data structures
 */
void (*tet_startup)() = start_server;
void (*tet_cleanup)() = NULL;
struct tet_testlist tet_testlist[] = {
	{client, 1},
	{NULL, 0}
};

void
start_server()
{
	static char		*args[] = {"./server", NULL };
	pid_t			server_pid;

	/*
	 *   fork a server
	 */
	server_pid = fork();

	switch (server_pid)
	{
	case -1:
		pthread_lock_global_np();
			tet_infoline("Can't create server process");
			tet_result(TET_FAIL);
		pthread_unlock_global_np();
		break;

	case 0:
		execv("./server", args);
		pthread_lock_global_np();
			tet_infoline("Can't execute server process");
			tet_result(TET_FAIL);
		pthread_unlock_global_np();
		break;

	default:
		break;
	}
}

void client()
{
	rpc_binding_handle_t	bind_handle;
	rpc_ns_handle_t		import_context;
	error_status_t		status;
	error_status_t		temp_status;
	unsigned char		in_net_string[SIZE];
	unsigned char		out_loc_string[SIZE];
	wchar_t			net_string[SIZE];
	wchar_t			loc_string[SIZE];
	unsigned char		err_buf[256];
	char			*nsi_entry_name;
	char			*client_locale_name;
	int			i, rpc_num, w_len, length;
	FILE			*fp_in, *fp_out;
	long			in_str_len;

	/*
	 * Sleep for a while for a server to start listening.
	 */
	sleep(30);

	mtet_infoline("test case (wchar_t client_method_conf_vary_array)\n");

	GET_SERVER_ENTRY(nsi_entry_name);

	GET_CLIENT_LOCALE(client_locale_name);

	setlocale(LC_ALL, client_locale_name);

	rpc_ns_binding_import_begin (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		cs_test_v1_0_c_ifspec,
                NULL,
                &import_context,
                &status );

	CHECK_STATUS(TRUE, "rpc_ns_binding_import_begin", status, rpc_s_ok);

	rpc_ns_import_ctx_add_eval (
		&import_context,
		rpc_c_eval_type_codesets,
		(void *)nsi_entry_name,
		rpc_cs_eval_without_universal,
		NULL,
		&status );

	CHECK_STATUS(TRUE, "rpc_ns_import_ctx_add_eval", status, rpc_s_ok);

	while (1) {
        	rpc_ns_binding_import_next (
               		import_context,
               		&bind_handle,
               		&status );

		CHECK_STATUS(TRUE, "rpc_ns_binding_import_next", status, rpc_s_ok);
		if (status == rpc_s_ok)
			break;
                else
                {
                        tet_result(TET_FAIL);
                        return;
                }
	}

	rpc_ns_binding_import_done (
		&import_context,
		&status );

	CHECK_STATUS(TRUE, "rpc_ns_binding_import_done", status, rpc_s_ok);

	rpc_ep_resolve_binding (bind_handle,
		cs_test_v1_0_c_ifspec,
		&temp_status);

	CHECK_STATUS(TRUE, "rpc_ep_resolve_binding", temp_status, rpc_s_ok);

	if(rpc_mgmt_is_server_listening(bind_handle, &status)
		&& temp_status == rpc_s_ok)
	{
		mtet_infoline("PASSED rpc_mgmt_is_server_listening()\n");
	}
	else
	{
		dce_error_inq_text ((unsigned long)status, err_buf, (int *)&temp_status);
		mtet_infoline("is_server_listening error -> %s\n", err_buf);
	}

	fp_in = fopen("./i18n_input_data", "r");

	if (fp_in == NULL)
	{
		mtet_infoline("i18n_input_data open failed\n");
		tet_result(TET_DCE_FAIL);
	}

	fp_out = fopen("./i18n_method_conf_vary_result_file", "w");

	if (fp_out == NULL)
	{
		mtet_infoline("i18n_result_file open failed\n");
		tet_result(TET_DCE_FAIL);
	}

	for (i = 0; i < SIZE; i++)
	{
		net_string[i] = L'\0';
		loc_string[i] = L'\0';
		in_net_string[i] = '\0';
		out_loc_string[i] = '\0';
	}

	rpc_num = 1;
	while (rpc_num <= 4)
	{
		(void)fgets((char *)in_net_string, SIZE, fp_in);

		length = strlen((char *)in_net_string);

		w_len = mbstowcs(net_string, (char *)in_net_string, (size_t)(length + 1));

		if (w_len == -1)
		{
			mtet_infoline("FAIL Invalid char is detected from mbstowcs\n");
			tet_result(TET_FAIL);
		}

		in_str_len  = ((long)w_len + 1) * sizeof(wchar_t);

		temp_status = cs_conf_vary_trans(bind_handle, in_str_len, in_str_len, net_string, loc_string);

		if (temp_status != rpc_s_ok)
		{
			dce_error_inq_text(temp_status, err_buf, (int *)&status);

			mtet_infoline("FAILED %ld  MSG: %s\n", (unsigned long)temp_status, err_buf);
		}
		else
		{
			mtet_infoline("PASSED rpc #%d\n", rpc_num);
			if (wcstombs((char *)out_loc_string, loc_string, (length + 1)) == -1)
			{
				mtet_infoline("FAIL Invalid char is detected from wcstombs\n");
				tet_result(TET_FAIL);
			}
			else
			{
				(void)fputs((char *)out_loc_string, fp_out);
				(void)fputs("\n", fp_out);
			}
		}
		rpc_num++;

		for (i = 0; i < SIZE; i++)
		{
			net_string[i] = L'\0';
			loc_string[i] = L'\0';
			in_net_string[i] = '\0';
			out_loc_string[i] = '\0';
		}
	}
	(void)fputs("\n", fp_out);

	fclose(fp_in);
	fclose(fp_out);

	/*
	 * Stop server...
	 */
	rpc_mgmt_stop_server_listening(bind_handle, &status);

	CHECK_STATUS(TRUE, "rpc_mgmt_stop_server_listening", status, rpc_s_ok);

	tet_result(TET_PASS);
}
