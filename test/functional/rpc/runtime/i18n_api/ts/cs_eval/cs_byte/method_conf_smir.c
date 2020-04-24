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
 * $Log: method_conf_smir.c,v $
 * Revision 1.1.4.2  1996/02/18  00:32:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:56  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:48:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:27  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:16:45  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:43:05  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:15:24  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:47  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:26:51  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <locale.h>
#include <dce/rpc.h>
#include <tet_api.h>
#include <pthread.h>

#include "cs_test.h"		/* IDL generated include file */
#include "check_status.h"       /* error checking */
#include "get_env.h"		/* GET.. macros */

extern void	eval_smir();
extern void	client();
extern void	start_server();
extern char	**environ;

/*
 * Initialize TCM data structures
 * Do not try to stop server from tet_cleanup routine.
 * Because if called from tet_cleanup routine, tet_result() 
 * will cause write() failed on temporary result file, which
 * makes an entire test case fail.
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
	static char     	*args[] = {"./server", NULL };
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
client()
{
	rpc_binding_handle_t    bind_handle;
	rpc_ns_handle_t         import_context;
	error_status_t		status;
	error_status_t		temp_status;
	cs_byte			net_string[SIZE];
	cs_byte			loc_string[SIZE];
	unsigned char		err_buf[256];
	char			*nsi_entry_name;
	char			*client_locale_name;
	int			i, rpc_num;
	FILE			*fp_in, *fp_out;
	long			in_str_len;


	/*
	 * Sleep for a while for a server to start listening.
	 */
	sleep(30);

	mtet_infoline("test case (cs_byte client_method_conf_smir_array)\n");

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
                eval_smir,
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

	fp_out = fopen("./i18n_method_conf_smir_result_file", "w");

	if (fp_out == NULL)
	{
                mtet_infoline("i18n_result_file open failed\n");
		tet_result(TET_DCE_FAIL);
	}

	for (i = 0; i < SIZE; i++)
	{
		net_string[i] = '\0';
		loc_string[i] = '\0';
	}

	rpc_num = 1;
	while (!feof(fp_in))
	{
		(void)fgets((char *)net_string, SIZE, fp_in);

		in_str_len = (long)strlen(net_string) + 1;

		temp_status = cs_conf_trans(bind_handle, in_str_len, net_string, loc_string);

		if (temp_status != rpc_s_ok)
		{
			dce_error_inq_text(temp_status, err_buf, (int *)&status);

			mtet_infoline("FAILED %ld  MSG: %s\n", (unsigned long)temp_status, err_buf);
		}
		else
		{
			mtet_infoline("PASSED rpc #%d\n", rpc_num++);
			(void)fputs((char *)loc_string, fp_out);
			(void)fputs("\n", fp_out);
		}

		for (i = 0; i < SIZE; i++)
		{
			net_string[i] = '\0';
			loc_string[i] = '\0';
		}
	}

	fclose(fp_in);
	fclose(fp_out);

	/* 
	 * Stop server...
	 */
	rpc_mgmt_stop_server_listening(bind_handle, &status); 

	CHECK_STATUS(TRUE, "rpc_mgmt_stop_server_listening", status, rpc_s_ok);

	tet_result(TET_PASS);
}
