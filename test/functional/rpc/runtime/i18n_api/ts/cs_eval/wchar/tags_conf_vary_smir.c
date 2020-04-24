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
 * $Log: tags_conf_vary_smir.c,v $
 * Revision 1.1.4.2  1996/02/18  00:33:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:18:54  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:51:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:16  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:17:37  root]
 * 
 * Revision 1.1.2.1  1994/09/27  18:47:37  mori_m
 * 	CR 11041:  New test for conformant array ops.
 * 	[1994/09/27  18:38:51  mori_m]
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

extern void	client();
extern void     start_server();
extern char	**environ;

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

void 
client(void)
{
	rpc_binding_handle_t	bind_handle;
	rpc_ns_handle_t		lookup_context;
	rpc_binding_vector_p_t	bind_vec_p;
	unsigned_char_t		*entry_name;
	unsigned32		binding_count;
	wchar_t			net_string[SIZE];
	wchar_t			loc_string[SIZE];
	unsigned char		in_net_string[SIZE];
	unsigned char		out_loc_string[SIZE];
	int			i, k, rpc_num, length;
	int			model_found, smir_true, cmir_true;
	rpc_codeset_mgmt_p_t	client, server;
	unsigned32		stag;
	unsigned32		drtag;
	unsigned16		stag_max_bytes;
	error_status_t		status;
	error_status_t		temp_status;
	unsigned char		err_buf[256];
	char			*nsi_entry_name;
	char			*client_locale_name;
	FILE			*fp_in, *fp_out;

	/*
	 * Sleep for a while for a server to start listening.
	 */
	sleep(30);

	mtet_infoline("test case (wchar_t client_tags_conf_vary_array_smir)\n");

	GET_SERVER_ENTRY(nsi_entry_name);

	GET_CLIENT_LOCALE(client_locale_name);

	setlocale(LC_ALL, client_locale_name);

	rpc_ns_binding_lookup_begin (
                rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		cs_test_v1_0_c_ifspec,
                NULL,
		rpc_c_binding_max_count_default,
                &lookup_context,
                &status );

	CHECK_STATUS(TRUE, "rpc_ns_binding_lookup_begin", status, rpc_s_ok);

	rpc_ns_binding_lookup_next (
		lookup_context,
		&bind_vec_p,
		&status );

	CHECK_STATUS(TRUE, "rpc_ns_binidng_lookup_next", status, rpc_s_ok);

        rpc_ns_binding_lookup_done (
                &lookup_context,
                &status );

	CHECK_STATUS(TRUE, "rpc_ns_binding_lookup_done", status, rpc_s_ok);

	/* 
	 *  Get the client's supported code sets 
	 */
	rpc_rgy_get_codesets (
		&client,
		&status );

	CHECK_STATUS(TRUE, "rpc_rgy_get_codesets", status, rpc_s_ok);

	binding_count = (bind_vec_p)->count;
	for (i=0; i < binding_count; i++)
	{
		if ((bind_vec_p)->binding_h[i] == NULL)
			continue;

		rpc_ns_binding_select (
			bind_vec_p,
			&bind_handle,
			&status );

		CHECK_STATUS(FALSE, "rpc_ns_binding_select", status, rpc_s_ok);

		if (status != rpc_s_ok)
		{
			rpc_ns_mgmt_free_codesets(&client, &status);
			CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);
		}

		rpc_ns_binding_inq_entry_name (
			bind_handle,
			rpc_c_ns_syntax_default,
			&entry_name,
			&status );

		CHECK_STATUS(TRUE, "rpc_ns_binding_inq_entry_name", status, rpc_s_ok);
		if (status != rpc_s_ok)
		{
			rpc_ns_mgmt_free_codesets(&client, &status);
			CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);
		}

		/* 
		 *  Get the server's supported code sets from NSI 
		 */
		rpc_ns_mgmt_read_codesets (
			rpc_c_ns_syntax_default,
			entry_name,
			&server,
			&status );

		CHECK_STATUS(FALSE, "rpc_ns_mgmt_read_codesets", status, rpc_s_ok);

		if (status != rpc_s_ok)
		{
			rpc_ns_mgmt_free_codesets(&client, &status);
			CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);
		}

		/*  
		 *  Start evaluation 
		 */
		if (client->codesets[0].c_set == server->codesets[0].c_set)
		{
			/* 
			 *  client and server are using the same code set 
			 */
			stag = client->codesets[0].c_set;
			drtag = server->codesets[0].c_set;
			break;
		}

		/* 
		 *  check character set compatibility first 
		 */
		rpc_cs_char_set_compat_check (
			client->codesets[0].c_set,
			server->codesets[0].c_set,
			&status );

		CHECK_STATUS(FALSE, "rpc_cs_char_set_compat_check", status, rpc_s_ok);

		if (status != rpc_s_ok)
		{
			rpc_ns_mgmt_free_codesets(&server, &status);
			CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);
		}

		smir_true = cmir_true = model_found = 0;

		for (k = 1; k <= server->count; k++)
		{
			if (model_found)
				break;

			if (client->codesets[0].c_set 
				== server->codesets[k].c_set)
			{
				smir_true = 1;
				model_found = 1;
			}

			if (server->codesets[0].c_set
				== client->codesets[k].c_set)
			{
				cmir_true = 1;
				model_found = 1;
			}
		}

		if (model_found)
		{
			if (smir_true && cmir_true)
			{
				/* SMIR model */
				mtet_infoline("SMIR used\n");
				stag = client->codesets[0].c_set;
				drtag = client->codesets[0].c_set;
				stag_max_bytes
				     = client->codesets[0].c_max_bytes;
			}
			else if (smir_true)
			{
				/* SMIR model */
				stag = client->codesets[0].c_set;
				drtag = client->codesets[0].c_set;
				stag_max_bytes
				     = client->codesets[0].c_max_bytes;
			}
			else
			{
				/* CMIR model */
				stag = server->codesets[0].c_set;
				drtag = server->codesets[0].c_set;
				stag_max_bytes
				     = server->codesets[0].c_max_bytes;
			}

			/* 
			 *  set tags value to the binding 
			 */
			rpc_cs_binding_set_tags (
				&bind_handle,
				stag,
				drtag,
				stag_max_bytes,
				&status );

			CHECK_STATUS(FALSE, "rpc_cs_binding_set_tags", status, rpc_s_ok);
			if (status != rpc_s_ok)
			{
				rpc_ns_mgmt_free_codesets(&server, &status);
				CHECK_STATUS(FALSE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);
				rpc_ns_mgmt_free_codesets(&client, &status);
				CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);
			}
		}
		else
		{
			/* 
			 *  try another binding 
			 */
			rpc_binding_free (
				&bind_handle,
				&status );

			CHECK_STATUS(FALSE, "rpc_binding_free", status, rpc_s_ok);
			if (status != rpc_s_ok)
			{
				rpc_ns_mgmt_free_codesets(&server, &status);
				CHECK_STATUS(FALSE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);
				rpc_ns_mgmt_free_codesets(&client, &status);
				CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);
			}
		}
	}

	rpc_ns_mgmt_free_codesets(&server, &status);
	CHECK_STATUS(FALSE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);

	rpc_ns_mgmt_free_codesets(&client, &status);
	CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_codesets", status, rpc_s_ok);

	if (!model_found)
	{
		mtet_infoline("FAILED No compabile server found\n");
		tet_result(TET_DCE_FAIL);
	}

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

	fp_out = fopen("./i18n_tags_conf_vary_smir_result_file", "w");

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
		int w_len;
		long in_str_len;

		(void)fgets((char *)in_net_string, SIZE, fp_in);

		length = strlen((char *)in_net_string);

		w_len = mbstowcs(net_string, (char *)in_net_string, (size_t)(length + 1));

		if (w_len == -1)
		{
			mtet_infoline("FAIL Invalid char is detected from mbstowcs\n");
			tet_result(TET_FAIL);
		}
		in_str_len = ((long)w_len + 1) * sizeof(wchar_t);

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

		for (i = 0; i < SIZE; i++)
		{
			in_net_string[i] = '\0';
			out_loc_string[i] = '\0';
			net_string[i] = L'\0';
			loc_string[i] = L'\0';
		}
		rpc_num++;
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
