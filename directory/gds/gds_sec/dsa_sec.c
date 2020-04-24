/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsa_sec.c,v $
 * Revision 1.1.5.2  1996/02/18  18:18:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:11  marty]
 *
 * Revision 1.1.5.1  1995/12/08  15:55:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:25  root]
 * 
 * Revision 1.1.2.10  1994/09/12  14:37:40  keutel
 * 	OT 11769
 * 	[1994/09/12  14:36:56  keutel]
 * 
 * Revision 1.1.2.9  1994/08/26  17:14:05  keutel
 * 	OT 11859
 * 	[1994/08/26  14:20:35  keutel]
 * 
 * Revision 1.1.2.8  1994/08/16  08:16:07  marrek
 * 	Fix for delta update (ot 11617).
 * 	[1994/08/12  12:00:17  marrek]
 * 
 * Revision 1.1.2.7  1994/07/06  15:07:19  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:15:31  marrek]
 * 
 * Revision 1.1.2.6  1994/06/21  14:46:50  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:42:46  marrek]
 * 
 * Revision 1.1.2.5  1994/06/09  18:45:09  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:56  devsrc]
 * 
 * Revision 1.1.2.4  1994/05/12  14:49:07  zeliff
 * 	ot10623: prefix include of gssapi.h with dce
 * 	[1994/05/12  14:43:37  zeliff]
 * 
 * Revision 1.1.2.3  1994/05/11  12:30:50  keutel
 * 	fix for two failing subtree tests: OT 10602
 * 	[1994/05/11  12:30:27  keutel]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:48  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:14:46  keutel
 * 	creation
 * 	[1994/03/21  15:55:27  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: dsa_sec.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/02/18 18:18:16 $";
#endif

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : dsa_sec.c                                           */
/*                                                                    */
/* Description  : The module contains the definition of functions     */
/*                that are called from the DSA                        */
/*                                                                    */
/* Date         : December 21, 1993                                   */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <gds.h>
#include <dce/d2dir.h>
#include <d26dsa.h>

#include <dce/gssapi.h>
/*#include <dce/gssapi-int.h>*/

#include <gds_sec.h>
#include <gds_sec_int.h>
#include <gds_sec_auth_ctx.h>
#include <gds_sec_util.h>



/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_acquire_cred                                  */
/*                                                                    */
/* Description	: This function is used to perform the network login  */
/*                of the DSA process. DSA has to call it once for     */
/*                Authentication mechanism used in directory binds    */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 dsa_acquire_cred( 
	signed16		mech,		/* IN security mechanism */
	D2_name_string 	dsa,		/* IN own name of the DSA  */
	D2_sec_status	*status)	/* OUT status and logging info */
	{

	gss_OID_set_desc oid_set ;
	gss_OID_set actual_oid_set;
	gss_OID_desc gss_mech;
	OM_uint32 minor_st;
	gss_name_t my_gss_name;
	OM_uint32 time_req , time_rec, ret;
	int cred_usage ;
	gss_cred_id_t cred_handle;
	signed32 d2_ret;
	D2_credent my_creds;


	GDS_SEC_TRACE0(2,"ENTRY dsa_acquirE_cred");
	if  (dsa != NULL )
    	{
		GDS_SEC_TRACE2(3,"mech:<%d> dsa:<%s>",mech,dsa);
		}
	else
		{
		GDS_SEC_TRACE1(3,"mech:<%d>",mech);
		}

	gds_sec__init_status(status);

	if ( mech == D2_ANONYMOUS)
		{
		GDS_SEC_TRACE0(2,"EXIT dsa_acquire_cred ANONYMOUS");
		return(D2_NOERROR);
		}

	if ( gds_sec__mech_2_oid( mech, &gss_mech, status) )
		{
		GDS_SEC_TRACE0(2,"dsa_acquire_cred: gds_sec__mech_2_oid failed");
		return (D2_ERROR);
		}

	if ((mech == D2_DCE_AUTH) || (mech == D2_STRONG))
		{
		oid_set.count = 1;
		oid_set.elements = &gss_mech;
	
		/* set client name for gssapi */

		if ( dsa_sec__map_client_name(	mech, &gss_mech,
										dsa, &my_gss_name,
										status))
			{                                                       
			GDS_SEC_TRACE0(2,
				"dsa_acquire_cred: dsa_sec__map_client_name failed");
			return(D2_ERROR);                                       
			}                                                       

		/* set up gss flags  */
		time_req = GSS_C_INDEFINITE;
		cred_usage = GSS_C_BOTH;

		GDS_SEC_TRACE_NAME(3,"dsa_acquire_cred", my_gss_name);          

		/* call on GSSAPI */
		ret = gss_acquire_cred(	&minor_st,my_gss_name, time_req, 
								&oid_set, cred_usage, &cred_handle,
								&actual_oid_set, &time_rec);
	
		/* in case of failure set logging info and status */
		if  (ret != GSS_S_COMPLETE )
			{
			/* map major returncode + fill minor code */
			gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);
			GDS_SEC_TRACE2(2,"gss_acquire-cred failed:<%d>:%s",ret,status->logstring);
			d2_ret = D2_ERROR;
			}
		else
			{                                                           
			d2_ret = D2_NOERROR;                                        
			}                                                           
		/* free name und oid buffer */

		ret = gss_release_name(&minor_st,&my_gss_name);                 
		if  (ret != GSS_S_COMPLETE )                                    
			{                                                           
			/* map major returncode & fill minor code */                
			gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);  
			GDS_SEC_TRACE2(2,"gss_release_name failed:<%d>:%s",ret,status->logstring );       
			}                                                           

		free(gss_mech.elements);

		/* store the cred_h associated with mechtype */
		if ( d2_ret == D2_NOERROR)
			{                                
			if(gds_sec__store_cred(mech,cred_handle, D2_DSA_CRED, status))
				d2_ret = D2_ERROR;          
			}                                                         
		}

		GDS_SEC_TRACE1(3,"stored cred_handle:<%d>", cred_handle);

		/* in case of DCE we have to acquire a second crredential  */
		/* containing ERA DN , therefore suitable for initiation of */
		/* security contexts (DSP binds)  */

		if (mech == D2_DCE_AUTH)
			{
			/* set ERA in the credentials */
			if ( dsa_sec__get_init_cred(&cred_handle ,status))
				return(D2_ERROR);
			if(gds_sec__store_cred(mech,cred_handle, D2_DSA_INIT_CRED, status))
				d2_ret = D2_ERROR;          

			}

	else /* simple authentication */
		{
		/* store name , password is not present in a simple DSP bind */
		d2_ret = D2_NOERROR;
		my_creds.d2_c_name = dsa;
		GDS_SEC_MALLOC(my_creds.d2_c_passwd,D2_password*,sizeof(D2_password),status);
		my_creds.d2_c_passwd->d2_p_len = 0;
		my_creds.d2_c_passwd->d2_p_pw = (Octet_string) 0;

		if (gds_sec__store_simple_cred(&my_creds,&cred_handle,status))
			return(D2_ERROR);

		if ( cred_handle != (gss_cred_id_t)1 )
			{
			/* DSA tried to acquire credentials for sec_mech == SIMPLE
			more often than once */
			GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR,D2_S_FAILURE); 
			sprintf(status->logstring,
				"more than one cred handle stored for authmech %d\n",
				mech);                                     
			GDS_SEC_TRACE1(3,"%s" ,status->logstring );
			return(D2_ERROR);
			}
		}


	GDS_SEC_TRACE1(3,"cred_handle:<%d>", cred_handle);
	GDS_SEC_TRACE0(2,"EXIT dsa_acquire_cred");
	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_del_cred                                      */
/*                                                                    */
/* Description	: This function releases the credentials credentials  */
/*                of the DSA process that have been acquired via      */
/*                a dsa_acquire_cred call using the same mechanism    */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dsa_del_cred(
	signed16		mech, 			/* IN security mechanism  */	
	D2_sec_status	*status)    	/* OUT status and logging info */	

	{
	OM_uint32 minor_st;
	OM_uint32 ret;
	gss_cred_id_t cred_h;
	gss_OID_desc gss_mech ; 
	gss_cred_id_t tmp_cred_h;
	unsigned32 creds_left = 1 ;
	D2_name_string cred_id[2] = { D2_DSA_CRED, D2_DSA_INIT_CRED } ;

	GDS_SEC_TRACE0(2,"ENTRY dsa_del_cred");
    GDS_SEC_TRACE1(3,"mech:<%d>",mech);

	gds_sec__init_status(status);

	if ( mech == D2_ANONYMOUS)
		{
		return(D2_NOERROR);
		}

	/* retrieve cred_h associated with the mech_type */ 

	if ( mech == D2_DCE_AUTH )
		creds_left = 2;


	if ( gds_sec__mech_2_oid( mech, &gss_mech, status) )
		{
		GDS_SEC_TRACE0(2,"dsa_del_cred: gds_sec__mech_2_oid failed");
		return (D2_ERROR);
		}


	while ( creds_left > 0)
		{
		creds_left -- ;
		if ( mech != D2_DCE_AUTH )
			{
			if ( gds_sec__get_cred(mech,&cred_h,status))
				return(D2_ERROR);
			}
		else
			{
			if (gds_sec__test_cred (mech,cred_id[creds_left], &cred_h, status))
				return(D2_ERROR);
			}

		GDS_SEC_TRACE1(5,"use cred_h %d", cred_h); 

		tmp_cred_h = cred_h;

		if ((mech == D2_DCE_AUTH) || (mech == D2_STRONG))
			{
			/* call on GSS API */
			ret = gss_release_cred(&minor_st,&cred_h);
			if  (ret != GSS_S_COMPLETE )                                    
				{                                                           
				/* map major returncode & fill minor code */                
				gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);  
				GDS_SEC_TRACE2(2,"gss_release_cred failed:<%d>:%s",status->logstring ,ret);       
				}                                                           
			}
		else /* simple auth */
			{
			/* no op */
			}
		/* free the gds internal cred_h store anyway */
		if ( gds_sec__free_cred (mech, tmp_cred_h, status))
			{
			/* dont report an error in this case */
			return(D2_NOERROR);
			}
		}
	GDS_SEC_TRACE0(2,"EXIT dsa_del_cred");
	return (D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_accept_sec_ctx                                */
/*                                                                    */
/* Description	: This function is used to verify the security context*/
/*                transported in the bind arguments of the DAP or DSP.*/
/*                The functions uses the credentials acquired by      */
/*                dsa_acquire_cred. Only if the desired_sec_mech==DCE */
/*                the master_bind flag set to TRUE may indicate a  */
/*                master bind                                         */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dsa_accept_sec_ctx(
	signed16		sec_mech,	/* IN  security mechanism */	
	D2_str			*client_token,		/* IN  clients sec ctx token */	
	D2_str			*return_token,		/* OUT token to be sent back */	
	D2_name_string	*client_name,		/* OUT clients X.500 DN */	
	Bool			*master_bind,		/* OUT TRUE or FALSE  */   
	D2_sec_status	*status)		/* OUT status and logging info */

	{
	gss_ctx_id_t ctx_h;
	gss_cred_id_t cred_h, deleg_cred_h;
	gss_OID actual_oid;
	OM_uint32 minor_st;
	gss_name_t gss_source;
	OM_uint32  time_rec, ret;
	int ret_flags;
	gss_channel_bindings_t chan_bindings;
	gss_buffer_desc input_token, output_token;
	signed32	return_value = D2_NOERROR;
	boolean tmp_mb;


	GDS_SEC_TRACE0(2,"ENTRY dsa_accept_sec_ctx");
	GDS_SEC_TRACE1(3,"sec_mech:<%d>", sec_mech);
	GDS_SEC_TRACE_TOKEN(4,"client_token",client_token);

	*master_bind = FALSE;
	strcpy((char*) *client_name,D26_ANONYMOUS);

	gds_sec__init_status(status);

	if ( sec_mech == D2_ANONYMOUS)
		{
		return_token->d2_size = 0;
		return_token->d2_value = (unsigned char *) 0;
		GDS_SEC_TRACE0(2,"EXIT anonymous dsa_accept_sec_ctx");
		return(D2_NOERROR);
		}

	/* retrieve the cred_handle belonging to the specified mech */
	
	if ( sec_mech != D2_DCE_AUTH )
		{
		if ( gds_sec__get_cred(sec_mech,&cred_h,status))
			return(D2_ERROR);
		}
	else
		{
		if (gds_sec__test_cred (sec_mech, D2_DSA_CRED, &cred_h, status))
			return(D2_ERROR);

		}
	GDS_SEC_TRACE1(5,"use cred_h %d",cred_h); 


	if ((sec_mech == D2_DCE_AUTH) || (sec_mech == D2_STRONG))
		{
		ctx_h = GSS_C_NO_CONTEXT;
		chan_bindings = GSS_C_NO_CHANNEL_BINDINGS;

		/* convert the client token buffer */

        input_token.length = client_token->d2_size ;
		input_token.value = (void *) client_token->d2_value;

		if (sec_mech == D2_DCE_AUTH)
			gss_source = NULL ;

		/* call on GSSAPI */
		ret = gss_accept_sec_context(&minor_st, &ctx_h, cred_h, 
						&input_token, chan_bindings, &gss_source, 
						&actual_oid,  &output_token, &ret_flags,
						&time_rec , &deleg_cred_h);
		
		/* in case of failure set logging info and status */
		if  (ret != GSS_S_COMPLETE )
			{
			/* map major returncode */
			/* fill minor code */
			gds_sec__map_gss_error( actual_oid, ret, minor_st, status);  
			GDS_SEC_TRACE2(2,"gss_accept_sec_context failed:<%d>:%s",ret,status->logstring);
			return(D2_ERROR);
			}
		GDS_SEC_TRACE1(3,"context_handle:<%d>",ctx_h);
		GDS_SEC_TRACE_TOKEN(4,"output_token",&output_token);

		/* return the return_token */
		/* if the supplied token buffers size is sufficient */
        if (return_token->d2_size < output_token.length)
			{
        	return_token->d2_size = output_token.length;
			GDS_SEC_SET_STATUS(status,D2_CALL_ERR,D2_SYS_ERR,D2_S_INSUF_BUF);
			sprintf(status->logstring,"return buffer size too small: %d of %d\n",
					return_token->d2_size, output_token.length);
			GDS_SEC_TRACE1(2,"%s",status->logstring );
			return (D2_ERROR);
			}
		return_token->d2_size = output_token.length;
		memcpy(return_token->d2_value, (unsigned char *)output_token.value,
							return_token->d2_size);

		/* retrieve the client name and master_bind flag */

        if ( dsa_sec__get_client_name ( sec_mech, 
								actual_oid,
								ctx_h, 
								gss_source,  
								client_name,  
								master_bind, 
								status))
			return(D2_ERROR);

		/* free the gss output token */

		ret = gss_release_buffer(&minor_st, &output_token);
		if (ret != GSS_S_COMPLETE)
			{
			/* ignore failure of this routine but produce logging str */
			gds_sec__map_gss_error( actual_oid, ret, minor_st, status); 
			GDS_SEC_TRACE2(2,"gss_release_buffer failed:<%d>:%s",ret,status->logstring );
			GDS_SEC_SET_STATUS(status,D2_WARNING,D2_MESSAGE,D2_S_FAILURE);
			}

		/* In case of strong free the gss source name */
		if (sec_mech == D2_STRONG)
				{
			ret = gss_release_name(&minor_st,&gss_source);  
			if  (ret != GSS_S_COMPLETE )
				{
				/* map major returncode & fill minor code */
				gds_sec__map_gss_error( actual_oid, ret, minor_st, status);
				GDS_SEC_TRACE2(2,"gss_release_name failed:<%d>:%s",ret,status->logstring );
				GDS_SEC_SET_STATUS(status,D2_WARNING,D2_MESSAGE,D2_S_FAILURE);
				}
			}

		/* delete the security context as it is no more necessary */
		ret = gss_delete_sec_context(&minor_st, &ctx_h, &output_token);
		if  (ret != GSS_S_COMPLETE )
			{
			/* map major returncode */
			/* fill minor code */
			gds_sec__map_gss_error( actual_oid, ret, minor_st, status); 
			GDS_SEC_TRACE2(2,"gss_delete_sec_context failed:<%d>:%s",ret,status->logstring );
			GDS_SEC_SET_STATUS(status,D2_WARNING,D2_MESSAGE,D2_S_FAILURE);
			return(D2_NOERROR);
			}
		}	
	else /* simple authentication */
		{
		/* call internal function , return DSA name in return_token*/

		if ( dsa_accept_simp_anon_sec_ctx(	sec_mech,
										client_token,
										return_token,
										client_name,
										&tmp_mb,
										status))
			{
			GDS_SEC_TRACE0(2,"dsa_accept_simp_anon_sec_ctx failed");
			return (D2_ERROR);
			}

		if (tmp_mb == TRUE)
			*master_bind = TRUE;
		}

	GDS_SEC_TRACE2(3,"client_name:<%s> master_bind <%d>",*client_name,*master_bind);
	GDS_SEC_TRACE_TOKEN(4,"return_token",return_token);

	GDS_SEC_TRACE0(2,"EXIT dsa_accept_sec_ctx");
	return(return_value);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_init_sec_ctx                                  */
/*                                                                    */
/* Description	: This function is used to create the security context*/
/*                transported in the bind arguments of the DSP to the */
/*                target DSA. Contexts are refenreced internally      */
/*                using the bind id. One of target_dsa_name or _psap  */
/*                must be supplied by the caller.                     */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dsa_init_sec_ctx(
	signed16		sec_mech, /* IN security mechanism */	
	D2_name_string	target_dsa_name,  /* IN DSAs X.500 DN (opt) */ 	
	Psap_string		target_dsa_psap,  /* IN DSAs address (opt) */ 	
	unsigned32		*bind_id,          /* OUT to reference handles */
	D2_str 			*sec_token,		  /* OUT token for target DSA */
	D2_sec_status	*status)       /* OUT status and logging info */
	{

	gss_ctx_id_t ctx_h;
	gss_OID actual_oid;
	gss_OID_desc gss_mech;
	OM_uint32 minor_st;
	gss_name_t gss_target;
	gss_cred_id_t cred_h ;
	OM_uint32 time_req, time_rec, ret;
	int req_flags , ret_flags;
	gss_channel_bindings_t chan_bindings;
	gss_buffer_desc output_token;
	D2_credent *my_creds;
	signed32 d2_ret = D2_NOERROR;

	GDS_SEC_TRACE0(2,"ENTRY dsa_init_sec_ctx");
	GDS_SEC_TRACE1(3,"sec_mech:<%d>", sec_mech);

	*bind_id = (unsigned32)0;
	sec_token->d2_size = 0;
	sec_token->d2_value = (unsigned char *) 0;
	gds_sec__init_status(status);


	if ( sec_mech == D2_ANONYMOUS)
		{
		GDS_SEC_TRACE1(2,
				"EXIT dsa_init_sec_ctx: bind_id <%d>",*bind_id);
		return(D2_NOERROR);
		}

	if ( gds_sec__mech_2_oid( sec_mech, &gss_mech, status) )
		{
		GDS_SEC_TRACE0(2,"dsa_init_sec_ctx: gds_sec__mech_2_oid failed");
		return (D2_ERROR);
		}

	/* retrieve the cred_handle belonging to the specified mech */
	
	if ( sec_mech != D2_DCE_AUTH )
		{
		if ( gds_sec__get_cred(sec_mech,&cred_h,status))
			return(D2_ERROR);
		}
	else
		{
		if (gds_sec__test_cred (sec_mech, D2_DSA_INIT_CRED, &cred_h, status))
			return(D2_ERROR);

		}

	GDS_SEC_TRACE1(5,"use cred_h %d",cred_h); 


	if ((sec_mech == D2_DCE_AUTH) || (sec_mech == D2_STRONG))
		{
		/* check whether one of target name or psap is supplied */
		/* map contents of target name to something known to GSS */
		/* and convert format to gss_name_t */

		if (dsa_sec__map_target_name(	sec_mech, &gss_mech,
									target_dsa_name,
									target_dsa_psap, &gss_target,
									status))
			return(D2_ERROR);


		/* set gssflags and handles */
		time_req = GSS_C_INDEFINITE;
		req_flags = GSS_C_MUTUAL_FLAG | GSS_C_REPLAY_FLAG ;
		ctx_h = GSS_C_NO_CONTEXT;
		chan_bindings = GSS_C_NO_CHANNEL_BINDINGS;

		GDS_SEC_TRACE_NAME(3,"dsa_init_sec_ctx", gss_target);

		/* call on GSSAPI */
		ret = gss_init_sec_context(	&minor_st, (gss_cred_id_t) cred_h,
									&ctx_h, gss_target, &gss_mech, 
									req_flags, time_req, 
									chan_bindings, GSS_C_NO_BUFFER,
									&actual_oid, &output_token, 
									&ret_flags, &time_rec);
		
		/* in case of failure set logging info and status */
		if  (ret != GSS_S_CONTINUE_NEEDED )
			{
			/* map major returncode */
			/* fill minor code */
			gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);  
			GDS_SEC_TRACE2(2,"gss_init_sec_context failed:<%d>:%s",ret,status->logstring );
			d2_ret = D2_ERROR;
			}

		else
			{
			d2_ret = D2_NOERROR;

			/* return the sec_token */
			sec_token->d2_size = output_token.length;
			sec_token->d2_value = (unsigned char*) output_token.value ;
			}

		/* free the oid  */
		free(gss_mech.elements);

		/* gss_name and token buffer are freed in dsa_conf_sec_ctx */

		} /* end dce or strong authentication */

	else /* simple authentication */
		{
		/* get user credential structure from internal store */
		if ( gds_sec__get_simple_cred( cred_h, &my_creds, status))
			{
			return(D2_ERROR);
			}

		GDS_SEC_TRACE1(5,"Simple cred_h %d",cred_h); 
		GDS_SEC_TRACE1(5,"dsa_init_sec_ctx cred->name <%s>", my_creds->d2_c_name);

		/* return name + password structure in sec_token */

		if ( dsa_gen_simp_anon_sec_ctx( sec_mech, sec_token,status))
			{
			GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_FAILURE);
			sprintf(status->logstring,
			"dsa_gen_simp_anon_sec_ctx failed for <%s>\n",
			my_creds->d2_c_name);
			GDS_SEC_TRACE1(2,"%s",status->logstring );
			return(D2_ERROR);
			}
		/* generate ctx handle  */

		GDS_SEC_MALLOC(ctx_h, gss_ctx_id_t, sizeof(D2_str), status);
		((D2_str *)ctx_h)->d2_size = sec_token->d2_size;
		((D2_str *)ctx_h)->d2_value = sec_token->d2_value;
		}

	/* generate bind_id */

	/* store ctx_h and assign the bind_id */
	if (gds_sec__store_ctx(sec_mech,(gss_cred_id_t) cred_h,
									ctx_h,gss_target, bind_id,status))
			return(D2_ERROR);

	GDS_SEC_TRACE2(3,
		"EXIT dsa_init_sec_ctx: context_handle:<%d> bind_id <%d>"
		,ctx_h,*bind_id);
	GDS_SEC_TRACE_TOKEN(4,
		"EXIT dsa_init_sec_ctx: output_token"
		,sec_token);

	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_conf_sec_ctx                                  */
/*                                                                    */
/* Description	: This function is used to accomplish the establish-  */
/*                ment of the security context between initiating DSA */
/*                and target DSA. The security context is deleted     */
/*                immediately after completion.                       */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dsa_conf_sec_ctx(
	signed16		sec_mech,	/* IN security mechanism */
	unsigned32		bind_id,			/* IN referenc to ctx handle */
	D2_str			*return_sec_token,	/* IN token from target */
	D2_sec_status	*status)		/* OUT status and logging info */

	{

	gss_ctx_id_t ctx_h;
	gss_cred_id_t cred_h;
	gss_OID actual_oid;
	gss_OID_desc gss_mech;
	OM_uint32 minor_st;
	gss_name_t gss_target;
	OM_uint32 time_req, time_rec, ret;
	int req_flags , ret_flags;
	gss_channel_bindings_t chan_bindings;
	gss_buffer_desc input_token, output_token;
	signed32 d2_ret = D2_NOERROR;

	GDS_SEC_TRACE0(2,"ENTRY dsa_conf_sec_ctx");
	GDS_SEC_TRACE2(3,"sec_mech:<%d> bind_id:<%d>", sec_mech,bind_id);
	if ( return_sec_token != NULL)
		{
		GDS_SEC_TRACE_TOKEN(3,"return_token",return_sec_token);
		}

	gds_sec__init_status(status);

	if ( sec_mech == D2_ANONYMOUS)
		{
		GDS_SEC_TRACE0(2,
			"EXIT dsa_conf_sec_ctx: anonymous");
		return(D2_NOERROR);
		}

	if ( gds_sec__mech_2_oid( sec_mech, &gss_mech, status) )
		{
		GDS_SEC_TRACE0(2,"dsa_conf_sec_ctx: gds_sec__mech_2_oid failed");
		return (D2_ERROR);
		}

	/* retrieve ctx_h, cred_h and target_name from store */
	if (gds_sec__get_ctx (sec_mech, bind_id, &ctx_h, &cred_h,
							&gss_target,status)) 
		return(D2_ERROR);

	if ((sec_mech == D2_DCE_AUTH) || (sec_mech == D2_STRONG))
		{

		/* set gssflags and handles */

		time_req = GSS_C_INDEFINITE;
		req_flags = GSS_C_MUTUAL_FLAG | GSS_C_REPLAY_FLAG ;
		chan_bindings = GSS_C_NO_CHANNEL_BINDINGS;

		/* map input_token */
		if ( return_sec_token != NULL)
			{
			input_token.length = return_sec_token->d2_size;
			input_token.value  = (void*) return_sec_token->d2_value;
			}
		else
			{
			input_token.length =  (size_t) 0;
			}


		GDS_SEC_TRACE_NAME(3,"dsa_conf_sec_ctx", gss_target);
		GDS_SEC_TRACE2(3,"dsa_conf_sec_ctx : cred_h %d ctx_h %d",
						cred_h,ctx_h);

		if ( input_token.length >0 )
			{
			/* call on GSSAPI */
			ret = gss_init_sec_context(	&minor_st, cred_h, &ctx_h,
										gss_target, &gss_mech, 
										req_flags, time_req,
										chan_bindings, &input_token,
										&actual_oid, &output_token, 
										&ret_flags, &time_rec);
			
			/* in case of failure set logging info and status */
			if  (ret != GSS_S_COMPLETE )
				{
				/* map major returncode */
				/* fill minor code */
				gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);  
				GDS_SEC_TRACE2(2,"2. gss_init_sec_context failed:<%d>:%s",ret,status->logstring );
				d2_ret = D2_ERROR;
				}

			else
				d2_ret = D2_NOERROR;
			}
		else /* there was no input token, hence no successful bind */
			{
			output_token.length = 0;
			/* this is handled in DSA itself, so dont return error */
			d2_ret= D2_NOERROR ; 
			}

		/* free the oid */
		free(gss_mech.elements);

		/* release the target name used by gss_init_sec_context */
		ret = gss_release_name(&minor_st,&gss_target);
		if  (ret != GSS_S_COMPLETE )
			{
			/* map major returncode & fill minor code */
			gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);  
			GDS_SEC_TRACE2(2,"gss_release_name failed:<%d>:%s",ret,status->logstring );
			}

		/* delete the security context */
		ret = gss_delete_sec_context(&minor_st, &ctx_h ,&output_token);

		if (ret != GSS_S_COMPLETE)
			{
			gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);  
			GDS_SEC_TRACE2(2,"gss_delete_sec_context failed:<%d>:%s",ret,status->logstring );
			/* ignore failure of this routine but produce logging str */
			}
		/* if gss_delete_sec_ctx returned an output_token free it */
		if (output_token.length != 0 )
			{
			ret = gss_release_buffer(&minor_st, &output_token);
			if (ret != GSS_S_COMPLETE)
				{
				gds_sec__map_gss_error( &gss_mech, ret,
										minor_st, status);  
				GDS_SEC_TRACE2(2,"gss_release_buffer failed:<%d>:%s",ret,status->logstring );
				/* ignore failure of this routine but 
												produce logging str */
				}
			}
		} /* end dce or strong authentication */

	else /* simple authentication */
		{
		/* get DSA name from the return token */

		dsa_conf_simp_anon_sec_ctx(sec_mech,return_sec_token,status);
		}

	/* free ctx_h and assign the bind_id */
	if (gds_sec__free_ctx(sec_mech,bind_id,status))
		{
		/* ignore failure of this routine but produce logging str */
		}

	GDS_SEC_TRACE1(2,"EXIT dsa_conf_sec_ctx: d2_ret <%d>",d2_ret);
	return(d2_ret);
	}
/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_get_tdt_entry                                 */
/*                                                                    */
/* Description	: This function is used to determine the level of     */
/*                trust wrt the originator name in a chained operation*/
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dsa_get_tdt_entry( 
	D2_name_string	dsa_name,		/* IN DN of chaining DSA  */ 
	signed32		*trust_level,	/* OUT qualifies originator name */
	D2_sec_status	*st)			/* OUT status and logging info */

	{

	signed32 ret;
	chandle_t tdt_h;
	char record[TDT_MAXBUF];
	unsigned char tdt_dsa_name[D2_DNL_MAX + 1];
	unsigned char tdt_file[D2_DNL_MAX + 1];

	extern char d26_install[];

	GDS_SEC_TRACE0(2,"ENTRY dsa_get_tdt_entry");


	*trust_level = TDT_NOT_SPEC ;
	/* check the dsa name to look for */

	/* dsa name check  */
	if ((dsa_name == NULL) || (strlen ((char*) dsa_name) == 0))
		{
		GDS_SEC_SET_STATUS(st, D2_CALL_ERR, D2_PAR_ERR, D2_S_BAD_NAME);
		sprintf(st->logstring,"dsa_get_tdt_entry called with empty name");
		GDS_SEC_TRACE1(3,"%s",st->logstring );
		return (D2_ERROR);
		}

    GDS_SEC_TRACE1(3,"dsa_name:<%s>",dsa_name);
	/* convert iapl formatted DSA name to TDT formatted DSA name. */
	if (gds_sec__iapl_2_print_name(dsa_name,tdt_dsa_name)) 
		{
		GDS_SEC_SET_STATUS(st, D2_CALL_ERR, D2_PAR_ERR, D2_S_BAD_NAME);
		sprintf(st->logstring,
			"dsa_get_tdt_entry: cannot map dsa name <%s> to printable form",
			dsa_name);
		GDS_SEC_TRACE1(1,"%s",st->logstring );
		return (D2_ERROR);
		}

	/* construct the tdt file name */
	sprintf((char*) tdt_file,"%s/%s",d26_install,TDT_NAME);

    GDS_SEC_TRACE2(3,"looking for <%s> in <%s>",tdt_dsa_name,tdt_file);


	ret = dsa__open_tdt((char*) tdt_file, TDT_FSEP, TDT_RSEP, &tdt_h);
	if ( ret != CACHE_OK)
		{
		GDS_SEC_SET_STATUS(st, D2_PERM_ERR, D2_SYS_ERR, D2_S_TDT_OPEN);
		sprintf(st->logstring,"dsa__open_tdt returns error %d\n", ret );
		GDS_SEC_TRACE1(3,"%s",st->logstring );
		return (D2_ERROR);
		}

	ret = dsa__read_tdt_record(tdt_h,CACHE_KEY, 0, (char*) tdt_dsa_name, record);
	if ((ret != CACHE_OK) && (ret != CACHE_NOFOUND))
		{
		GDS_SEC_SET_STATUS(st, D2_PERM_ERR, D2_SYS_ERR, D2_S_TDT_READ);
		sprintf(st->logstring,"dsa__read_tdt_record returns error %d\n", ret );
		GDS_SEC_TRACE1(3,"%s",st->logstring );
		dsa__close_tdt(tdt_h);
		return (D2_ERROR);
		}

	/* if there is NO entry for a dsa in the TDT, it is trusted by 
	   DEFAULT */

	if (ret == CACHE_NOFOUND)
		*trust_level = TDT_TRUSTED_FOR_ALL;

	else
		*trust_level = dsa__tdt_read_perm( record, 1, TDT_FSEP);

	dsa__close_tdt(tdt_h);

	GDS_SEC_TRACE2(3,"dsa_get_tdt_entry found trustlevel <%d> for <%s>",
				*trust_level, dsa_name);
	GDS_SEC_TRACE0(2,"EXIT dsa_get_tdt_entry");

	return(D2_NOERROR);
	}

