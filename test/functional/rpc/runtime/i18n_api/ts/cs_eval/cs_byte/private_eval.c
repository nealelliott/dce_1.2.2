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
 * $Log: private_eval.c,v $
 * Revision 1.1.4.2  1996/02/18  00:32:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:18:11  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:49:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:37  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:16:56  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:43:25  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:15:32  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:52  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:28:02  mori_m]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <dce/rpc.h>

#include "private_eval.h"     /* Private defs for code set interoperability */

void eval_cmir
#ifdef _DCE_PROTO_
(
	handle_t		binding_h,
	void			*args,
	void			**cntx
)
#else
(binding_h, args, cntx)
	handle_t		binding_h;
	void			*args;
	void			**cntx;
#endif
{
	rpc_cs_codeset_i14y_data_p	i14y_data_p;	
	rpc_cs_codeset_i14y_data_p	cntx_i14y_data_p;	
	rpc_cs_method_eval_p_t		method_p;
	rpc_ns_handle_t			inq_context;
	unsigned_char_p_t		client_codesets_file;
	unsigned_char_p_t		ns_name_p;
	int				i,j;
	int				model_found;
	int				smir_true;
	int				cmir_true;
	long				i_code;
	int				i_max_bytes;
	error_status_t			temp_status;



	i14y_data_p = (rpc_cs_codeset_i14y_data_p)args;

	if (i14y_data_p->cleanup)
	{
		cntx_i14y_data_p = (rpc_cs_codeset_i14y_data_p)*cntx;
		cntx_i14y_data_p->status = rpc_s_ok;
		return;
	}

	method_p = i14y_data_p->method_p;
	if (cntx == NULL)
	{
			return;
	}
	else
		cntx_i14y_data_p = (rpc_cs_codeset_i14y_data_p)*cntx;

	/* 
	 * Get the client's supported code sets.
	 */
	if (method_p->client == NULL)
	{
		rpc_rgy_get_codesets ( 
			&method_p->client,
			&cntx_i14y_data_p->status);
		
		if (cntx_i14y_data_p->status != rpc_s_ok)
			return;
	}

	/* 
	** Get the server's supported code sets from NSI.
	*/

	ns_name_p = i14y_data_p->ns_name;

	rpc_ns_mgmt_read_codesets (
		rpc_c_ns_syntax_default,
		ns_name_p,
		&method_p->server,
		&cntx_i14y_data_p->status);

	if (cntx_i14y_data_p->status != rpc_s_ok)
	{
		rpc_ns_mgmt_free_codesets( &method_p->client, &temp_status);
		return;
	}

	/* 
	 * Start evaluation
	 */
	if (method_p->client->codesets[0].c_set 
			== method_p->server->codesets[0].c_set)
	{
		/* 
		 * Both client and server are using the same code set 
		 */
		method_p->method = RPC_EVAL_NO_CONVERSION;
		method_p->tags.stag = method_p->client->codesets[0].c_set;
		method_p->tags.drtag = method_p->server->codesets[0].c_set;
	}
	else
	{
		/*
		 * We check character set compatibility first.
		 */
		rpc_cs_char_set_compat_check (
			method_p->client->codesets[0].c_set,
			method_p->server->codesets[0].c_set,
			&cntx_i14y_data_p->status);
		
		if (cntx_i14y_data_p->status != rpc_s_ok)
		{
			/* 
			 * Character set for client and server didn't match.
			 * Mass of data loss could result, so we quit the 
			 * evaluation here. 
			 */
			rpc_ns_mgmt_free_codesets( &method_p->server, &temp_status);
			return;
		}
		else
		{
			smir_true = cmir_true = model_found = 0;

			for (i = 1; i <= method_p->server->count; i++)
			{
			   if (model_found)
				break;
	
			   if (method_p->client->codesets[0].c_set 
				== method_p->server->codesets[i].c_set)
			   {
				smir_true = 1;
				model_found = 1;
			   }

			   if (method_p->server->codesets[0].c_set 
				== method_p->client->codesets[i].c_set)
			   {
				cmir_true = 1;
				model_found = 1;
			   }
			}
	
			if (model_found)
			{
			   if (smir_true && cmir_true)
			   {
				/* CMIR model */
				mtet_infoline("CMIR used\n");
				method_p->method = RPC_EVAL_CMIR_MODEL;
				method_p->tags.stag
					= method_p->server->codesets[0].c_set;
				method_p->tags.drtag
					= method_p->server->codesets[0].c_set;
				method_p->tags.stag_max_bytes 
				    = method_p->server->codesets[0].c_max_bytes;
				method_p->tags.client_tag 
				    = method_p->client->codesets[0].c_set;
				method_p->tags.client_max_bytes
				    = method_p->client->codesets[0].c_max_bytes;
			   }
			   else if (smir_true)
			   {
				/* SMIR model */
				method_p->method = RPC_EVAL_SMIR_MODEL;
				method_p->tags.stag
				    	= method_p->client->codesets[0].c_set;
				method_p->tags.drtag
				     	= method_p->client->codesets[0].c_set;
				method_p->tags.stag_max_bytes 
				    = method_p->client->codesets[0].c_max_bytes;
				method_p->tags.client_tag 
				    = method_p->client->codesets[0].c_set;
				method_p->tags.client_max_bytes
				    = method_p->client->codesets[0].c_max_bytes;
			   }
			   else
			   {
				/* CMIR model */
				method_p->method = RPC_EVAL_CMIR_MODEL;
				method_p->tags.stag
					= method_p->server->codesets[0].c_set;
				method_p->tags.drtag
					= method_p->server->codesets[0].c_set;
				method_p->tags.stag_max_bytes 
				    = method_p->server->codesets[0].c_max_bytes;
				method_p->tags.client_tag 
				    = method_p->client->codesets[0].c_set;
				method_p->tags.client_max_bytes
				    = method_p->client->codesets[0].c_max_bytes;
			   }
			}
			else
			{	
				/*
				 * We try to find the intermediate code set
				 */
				method_p->tags.client_tag 
				    = method_p->client->codesets[0].c_set;
				method_p->tags.client_max_bytes
				    = method_p->client->codesets[0].c_max_bytes;

				for (i = 1; i <= method_p->client->count; i++)
				{
				   if (model_found)
					break;
			  	   for (j = 1; j <= method_p->server->count; j++)
					{
					   if (method_p->client->codesets[i].c_set
						== method_p->server->codesets[j].c_set)
					   {
						i_code = method_p->client->codesets[i].c_set;
						i_max_bytes = method_p->client->codesets[i].c_max_bytes;
						method_p->tags.stag_max_bytes 
						   = method_p->client->codesets[i].c_max_bytes;
						model_found = 1;
						   break;
				    	    }
					}
				}
				if (model_found)
				{
					method_p->method = RPC_EVAL_INTERMEDIATE_MODEL;
					method_p->tags.stag = i_code;
					method_p->tags.drtag = i_code;
			   	}
				else
				{
					/*
					 * We use UNIVERSAL code set
					 */
					method_p->method = RPC_EVAL_UNIVERSAL_MODEL;
					method_p->tags.stag = UCS2_L2;
					method_p->tags.drtag = UCS2_L2;
					method_p->tags.client_tag 
				    	   = method_p->client->codesets[0].c_set;
					method_p->tags.client_max_bytes
					    = method_p->client->codesets[0].c_max_bytes;
			    		rpc_rgy_get_max_bytes (
						UCS2_L2,
						&method_p->tags.stag_max_bytes,
						&cntx_i14y_data_p->status
					);
					if (cntx_i14y_data_p->status != rpc_s_ok)
					{
						rpc_ns_mgmt_free_codesets( &method_p->server, &temp_status);
						return;
					}
			   	}
			}
		}
	}

	method_p->fixed = ndr_true;
	cntx_i14y_data_p->status = rpc_s_ok;
	rpc_ns_mgmt_free_codesets( &method_p->server, &temp_status);
	return;
}

void eval_smir
#ifdef _DCE_PROTO_
(
	handle_t		binding_h,
	void			*args,
	void			**cntx
)
#else
(binding_h, args, cntx)
	handle_t		binding_h;
	void			*args;
	void			**cntx;
#endif
{
	rpc_cs_codeset_i14y_data_p	i14y_data_p;	
	rpc_cs_codeset_i14y_data_p	cntx_i14y_data_p;	
	rpc_cs_method_eval_p_t		method_p;
	rpc_ns_handle_t			inq_context;
	unsigned_char_p_t		client_codesets_file;
	unsigned_char_p_t		ns_name_p;
	int				i,j;
	int				model_found;
	int				smir_true;
	int				cmir_true;
	long				i_code;
	int				i_max_bytes;
	error_status_t			temp_status;



	i14y_data_p = (rpc_cs_codeset_i14y_data_p)args;

	if (i14y_data_p->cleanup)
	{
		cntx_i14y_data_p = (rpc_cs_codeset_i14y_data_p)*cntx;
		cntx_i14y_data_p->status = rpc_s_ok;
		return;
	}

	method_p = i14y_data_p->method_p;
	if (cntx == NULL)
	{
			return;
	}
	else
		cntx_i14y_data_p = (rpc_cs_codeset_i14y_data_p)*cntx;

	/* 
	 * Get the client's supported code sets.
	 */
	if (method_p->client == NULL)
	{
		rpc_rgy_get_codesets ( 
			&method_p->client,
			&cntx_i14y_data_p->status);
		
		if (cntx_i14y_data_p->status != rpc_s_ok)
			return;
	}

	/* 
	** Get the server's supported code sets from NSI.
	*/

	ns_name_p = i14y_data_p->ns_name;

	rpc_ns_mgmt_read_codesets (
		rpc_c_ns_syntax_default,
		ns_name_p,
		&method_p->server,
		&cntx_i14y_data_p->status);

	if (cntx_i14y_data_p->status != rpc_s_ok)
	{
		rpc_ns_mgmt_free_codesets( &method_p->client, &temp_status);
		return;
	}

	/* 
	 * Start evaluation
	 */
	if (method_p->client->codesets[0].c_set 
			== method_p->server->codesets[0].c_set)
	{
		/* 
		 * Both client and server are using the same code set 
		 */
		method_p->method = RPC_EVAL_NO_CONVERSION;
		method_p->tags.stag = method_p->client->codesets[0].c_set;
		method_p->tags.drtag = method_p->server->codesets[0].c_set;
	}
	else
	{
		/*
		 * We check character set compatibility first.
		 */
		rpc_cs_char_set_compat_check (
			method_p->client->codesets[0].c_set,
			method_p->server->codesets[0].c_set,
			&cntx_i14y_data_p->status);
		
		if (cntx_i14y_data_p->status != rpc_s_ok)
		{
			/* 
			 * Character set for client and server didn't match.
			 * Mass of data loss could result, so we quit the 
			 * evaluation here. 
			 */
			rpc_ns_mgmt_free_codesets( &method_p->server, &temp_status);
			return;
		}
		else
		{
			smir_true = cmir_true = model_found = 0;

			for (i = 1; i <= method_p->server->count; i++)
			{
			   if (model_found)
				break;
	
			   if (method_p->client->codesets[0].c_set 
				== method_p->server->codesets[i].c_set)
			   {
				smir_true = 1;
				model_found = 1;
			   }

			   if (method_p->server->codesets[0].c_set 
				== method_p->client->codesets[i].c_set)
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
				method_p->method = RPC_EVAL_SMIR_MODEL;
				method_p->tags.stag
				    	= method_p->client->codesets[0].c_set;
				method_p->tags.drtag
				     	= method_p->client->codesets[0].c_set;
				method_p->tags.stag_max_bytes 
				    = method_p->client->codesets[0].c_max_bytes;
				method_p->tags.client_tag 
				    = method_p->client->codesets[0].c_set;
				method_p->tags.client_max_bytes
				    = method_p->client->codesets[0].c_max_bytes;
			   }
			   else if (smir_true)
			   {
				/* SMIR model */
				method_p->method = RPC_EVAL_SMIR_MODEL;
				method_p->tags.stag
				    	= method_p->client->codesets[0].c_set;
				method_p->tags.drtag
				     	= method_p->client->codesets[0].c_set;
				method_p->tags.stag_max_bytes 
				    = method_p->client->codesets[0].c_max_bytes;
				method_p->tags.client_tag 
				    = method_p->client->codesets[0].c_set;
				method_p->tags.client_max_bytes
				    = method_p->client->codesets[0].c_max_bytes;
			   }
			   else
			   {
				/* CMIR model */
				method_p->method = RPC_EVAL_CMIR_MODEL;
				method_p->tags.stag
					= method_p->server->codesets[0].c_set;
				method_p->tags.drtag
					= method_p->server->codesets[0].c_set;
				method_p->tags.stag_max_bytes 
				    = method_p->server->codesets[0].c_max_bytes;
				method_p->tags.client_tag 
				    = method_p->client->codesets[0].c_set;
				method_p->tags.client_max_bytes
				    = method_p->client->codesets[0].c_max_bytes;
			   }
			}
			else
			{	
				/*
				 * We try to find the intermediate code set
				 */
				method_p->tags.client_tag 
				    = method_p->client->codesets[0].c_set;
				method_p->tags.client_max_bytes
				    = method_p->client->codesets[0].c_max_bytes;

				for (i = 1; i <= method_p->client->count; i++)
				{
				   if (model_found)
					break;
			  	   for (j = 1; j <= method_p->server->count; j++)
					{
					   if (method_p->client->codesets[i].c_set
						== method_p->server->codesets[j].c_set)
					   {
						i_code = method_p->client->codesets[i].c_set;
						i_max_bytes = method_p->client->codesets[i].c_max_bytes;
						method_p->tags.stag_max_bytes 
						   = method_p->client->codesets[i].c_max_bytes;
						model_found = 1;
						   break;
				    	    }
					}
				}
				if (model_found)
				{
					method_p->method = RPC_EVAL_INTERMEDIATE_MODEL;
					method_p->tags.stag = i_code;
					method_p->tags.drtag = i_code;
			   	}
				else
				{
					/*
					 * We use UNIVERSAL code set
					 */
					method_p->method = RPC_EVAL_UNIVERSAL_MODEL;
					method_p->tags.stag = UCS2_L2;
					method_p->tags.drtag = UCS2_L2;
					method_p->tags.client_tag 
				    	   = method_p->client->codesets[0].c_set;
					method_p->tags.client_max_bytes
					    = method_p->client->codesets[0].c_max_bytes;
			    		rpc_rgy_get_max_bytes (
						UCS2_L2,
						&method_p->tags.stag_max_bytes,
						&cntx_i14y_data_p->status
					);
					if (cntx_i14y_data_p->status != rpc_s_ok)
					{
						rpc_ns_mgmt_free_codesets( &method_p->server, &temp_status);
						return;
					}
			   	}
			}
		}
	}

	method_p->fixed = ndr_true;
	cntx_i14y_data_p->status = rpc_s_ok;
	rpc_ns_mgmt_free_codesets( &method_p->server, &temp_status);
	return;
}
