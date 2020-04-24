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
 * $Log: dua_sec.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:55:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:29  root]
 * 
 * Revision 1.1.2.8  1994/09/23  12:14:36  marrek
 * 	Enabled masterbind mechanism for DCE authentication.
 * 	[1994/09/23  12:13:47  marrek]
 * 
 * Revision 1.1.2.7  1994/08/26  17:14:06  keutel
 * 	OT 11859
 * 	[1994/08/26  14:20:39  keutel]
 * 
 * Revision 1.1.2.6  1994/07/06  15:07:20  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:15:36  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:46:53  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:48  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:45:12  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:59  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/12  14:49:10  zeliff
 * 	ot10623: prefix include of gssapi.h with dce
 * 	[1994/05/12  14:43:41  zeliff]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:59  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:14:50  keutel
 * 	creation
 * 	[1994/03/21  15:55:35  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: dua_sec.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:19 $";
#endif

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : dua_sec.c                                           */
/*                                                                    */
/* Description  : The module contains the definition of functions     */
/*                that are called from the DUA library                */
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

#include <dce/gssapi.h>
/*#include <dce/gssapi-int.h>*/

#include <d21dua.h>

#include <gds_sec.h>
#include <gds_sec_int.h>
#include <gds_sec_auth_ctx.h>
#include <gds_sec_util.h>

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dua_acquire_cred                                  */
/*                                                                    */
/* Description	: This function is used to trigger the import of      */
/*                network credentials. Whether a login procedur is    */
/*                executed below the gssapi call is mech dependent.   */
/*                In DCE the DUA application process has to call on   */
/*                dce_login before !                                  */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 dua_acquire_cred(
	signed16			sec_mech,  /* IN security mechanism   */
	D2_credent			*my_creds,   /* IN clients credentials (cond) */
	Bool				*master_bind, /* IN/OUT flag - only for DCE */
	D2_cred_handle_t	*cred_h,         /* OUT handle for dua_init.. */
	D2_sec_status		*status)       /* OUT status and logging info */
	{
     
	gss_OID_set_desc oid_set ;
	gss_OID_set actual_oid_set;
	gss_OID_desc gss_mech ;
	OM_uint32 minor_st;
	gss_name_t my_gss_name;
	gss_cred_id_t tmp_cred_handle;
	OM_uint32 time_req , time_rec, ret;
	int cred_usage ;
	D2_name_string test_name;
	signed32 d2_ret = D2_NOERROR;
	signed32 testlen;

	GDS_SEC_TRACE0(2,"ENTRY dua_acquire_cred");

	gds_sec__init_status(status);

	if (( my_creds != NULL) && (my_creds->d2_c_name != NULL))
		{
		GDS_SEC_TRACE2(3,"sec_mech:<%d> name <%s>",
								sec_mech,my_creds->d2_c_name);
		testlen = 1 + strlen((char*) my_creds->d2_c_name);
		}
	else
		{
		GDS_SEC_TRACE1(3,"sec_mech:<%d>", sec_mech);
		testlen = 1 + strlen((char*)D2_DCE_USER_PRIV) ;
		}


	if ( sec_mech == D2_ANONYMOUS)
		{
		*master_bind = FALSE;
		*cred_h = (D2_cred_handle_t)0;
		GDS_SEC_TRACE1(2,"EXIT dua_acquire_cred: cred_handle:<%d>",*cred_h);
		return(D2_NOERROR);
		}

	if (( sec_mech != D2_DCE_AUTH) && (  *master_bind == TRUE ))
		{
		GDS_SEC_SET_STATUS(status,D2_CALL_ERR,D2_PAR_ERR,D2_S_FAILURE);
		GDS_SEC_TRACE0(1,"Masterbind is only valid for sec_mech = D2_DCE_AUTH");
		return (D2_ERROR);

		}
	/* check whether credentials are already acquired */
	GDS_SEC_MALLOC(test_name,D2_name_string,testlen,status);

	if (sec_mech == D2_DCE_AUTH)
		{
		if ( *master_bind == FALSE)
			{
			strcpy((char*) test_name,(char*) D2_DCE_USER);
			} 
		if ( *master_bind == TRUE)
			{
			strcpy((char*) test_name,(char*) D2_DCE_USER_PRIV);
			}
		} 

	else /*  simple or strong */
		{
		strcpy((char*) test_name,(char*) my_creds->d2_c_name);
		*master_bind = FALSE;
		}


	/* in case DCE && Master_bind == FALSE we have to look whether
	MB credentials have been installed by gds_sec_set_master_bind.
	The cred storage has still to contain creds for name D2_MB_UNUSED.
	If so , we return the MB creds anfd remove the D2_MB_UNUSED .
	Sounds rather weired, but it is the truth */

	if ( (sec_mech == D2_DCE_AUTH) && ( *master_bind == FALSE) )
		{
		ret = gds_sec__test_cred (  sec_mech, D2_MB_UNUSED ,
								&tmp_cred_handle, status);
		if ( ret == D2_NOERROR )
			{
			/* there are MB creds just installed by gds_sec_set_master_bind */
			/* remove the indicator creds D2_MB_UNUSED */
			ret = gds_sec__free_cred (  sec_mech, tmp_cred_handle,status);
			if ( ret != D2_NOERROR )
				{
				/* just log a warning */
				GDS_SEC_TRACE1(2,"gds_sec__free_cred failed for <%s>",  D2_MB_UNUSED);
				}

			/* get the MB cred */
			ret = gds_sec__test_cred (  sec_mech, D2_DCE_USER_PRIV ,
									(gss_cred_id_t *) cred_h, status);
			if ( ret == D2_NOERROR )
				{
				/* Masterbind creds are available */
				free ( test_name);
				GDS_SEC_TRACE1(2,"EXIT dua_acquire_cred: MB cred_handle:<%d>",*cred_h);
				return(D2_NOERROR);
				}
			GDS_SEC_TRACE0(2,"UNEXPECTED: NO MB CREDS FOUND");
			}
		}

	if ( *master_bind == FALSE)
		{
		ret = gds_sec__test_cred (	sec_mech, test_name ,
							(gss_cred_id_t *) cred_h, status);
		if ( ret == D2_NOERROR )
			{
			/* creds are already available */
			/* return as we can reuse the credentials */
			free ( test_name);
			GDS_SEC_TRACE1(2,"EXIT dua_acquire_cred: cred_handle:<%d>",*cred_h);
			return(D2_NOERROR);
			}
		}

	GDS_SEC_TRACE0(2,"dua_acquire_cred:  NO cred - continue");

	if ( gds_sec__mech_2_oid( sec_mech, &gss_mech, status) )
		{
		GDS_SEC_TRACE0(2,"dua_acquire_cred: gds_sec__mech_2_oid failed");
		return (D2_ERROR);
		}

	if ((sec_mech == D2_DCE_AUTH) || (sec_mech == D2_STRONG))
		{
		/* set up the oid set with the required mechanism */
		oid_set.count = 1;
		oid_set.elements = &gss_mech;
    
		/* set client name for gssapi */

		if ( dua_sec__map_client_name(	sec_mech, &gss_mech,
										test_name, master_bind,
										&my_gss_name, status))
			{
			GDS_SEC_TRACE0(2,"dua_acquire_cred: dua_sec__map_client_name failed");
			return(D2_ERROR);
			}

		/* set X500_DN extended attribute if its a DCE bind and
		if its not a master_bind . In case of masterbind == TRUE 
		extended attrs are set from gds_sec_set_masterbind() */

		if ( (sec_mech == D2_DCE_AUTH) && (*master_bind != TRUE))
			{
			if (gds_sec__set_ext_attr(status) == D2_ERROR)
				{
				GDS_SEC_TRACE0(2,"dua_acquire_cred: gds_sec__set_ext_attr() failed");
				return(D2_ERROR);
				}
			GDS_SEC_TRACE0(3,"dua_acquire_cred: gds_sec__set_ext_attr() succeeded");
			}

		/* set up flags */
		if (*master_bind != TRUE)
			time_req = GSS_C_INDEFINITE;
		if (*master_bind != TRUE)
			time_req = 5 * 60 ; /* mb credentials only for 5 minutes */;
		cred_usage = GSS_C_INITIATE;
		GDS_SEC_TRACE_NAME(3,"dua_acquire_cred", my_gss_name);

		/* call on GSSAPI */
		ret = gss_acquire_cred(	&minor_st,my_gss_name, time_req,
								&oid_set, cred_usage,
								(gss_cred_id_t *)cred_h,
								&actual_oid_set, &time_rec);
	
		/* in case of failure set loggin info and status */
		if  (ret != GSS_S_COMPLETE )
			{
			/* map major returncode & fill minor code */
			gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);  
			GDS_SEC_TRACE2(2,"gss_acquire-cred failed:<%d>:%s",ret,status->logstring );
			d2_ret = D2_ERROR;
			}
		/* gss was ok , but we dont check actual_mech  + time_rec */
		else
			{
			d2_ret = D2_NOERROR;
			}

		/* free name and oid buffers */

		ret = gss_release_name(&minor_st,&my_gss_name);
		if  (ret != GSS_S_COMPLETE )
			{
			/* map major returncode & fill minor code */
			gds_sec__map_gss_error( &gss_mech, ret, minor_st, status);  
			GDS_SEC_TRACE2(2,"gss_release_name failed:<%d>:%s",ret,status->logstring );
			}

		free(gss_mech.elements);

		/* store the credential handle */
		if ( d2_ret == D2_NOERROR)
			{
			if(gds_sec__store_cred(	sec_mech,(gss_cred_id_t) *cred_h,
									test_name, status)) 
				{
				d2_ret = D2_ERROR;
				}
			} 
		}

	else /* simple authentication */
		{
		d2_ret= D2_NOERROR;
		/* store name+password  and get a cred_h */
		if (gds_sec__store_simple_cred(my_creds, (gss_cred_id_t*) cred_h, status))
			{
			d2_ret = D2_ERROR;
			}
		}
	free ( test_name);

	GDS_SEC_TRACE1(2,"EXIT dua_acquire_cred: cred_handle:<%d>",*cred_h);
	
	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dua_init_sec_ctx                                  */
/*                                                                    */
/* Description	: This function is used to create the security context*/
/*                transported in the bind arguments of the DAP to the */
/*                DSA. The cred_h is used to request the security ctx.*/
/*                bind_id is generated and stored to enable dua_conf  */
/*                _sec_ctx to retrieve the correct context.           */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dua_init_sec_ctx(
	signed16		sec_mech, /* IN security mechanism */	
	D2_name_string	target_dsa_name,  /* IN DSAs X.500 DN (opt) */ 	
	signed16		target_dsa_dirid, /* IN dirid  need if name given */
	Psap_string		target_dsa_psap,  /* IN DSAs address (opt) */
	signed16		target_dsa_psap_len, /* IN length of PSAP */
	D2_cred_handle_t	cred_h,       /* IN	from dua_acquire_cred */
	unsigned32		*bind_id,         /* OUT to reference handles */
	D2_str 			*sec_token,		  /* OUT token for DSA */
	D2_sec_status	*status)          /* OUT status and logging info */
	{

	gss_ctx_id_t ctx_h,tmp_h;
	gss_OID actual_oid;
	gss_OID_desc gss_mech;
	OM_uint32 minor_st;
	gss_name_t gss_target;
	OM_uint32 time_req, time_rec, ret;
	int req_flags , ret_flags;
	gss_channel_bindings_t chan_bindings;
	gss_buffer_desc output_token;
	D2_credent *my_creds;
	signed32 d2_ret = D2_NOERROR;

	GDS_SEC_TRACE0(2,"ENTRY dua_init_sec_ctx");
	GDS_SEC_TRACE2(3,"sec_mech:<%d> cred_handle:<%d>",
		sec_mech,cred_h);

	*bind_id = (unsigned32)0;
	sec_token->d2_size = 0;
	sec_token->d2_value = (unsigned char *) 0;
	gds_sec__init_status(status);


	if ( sec_mech == D2_ANONYMOUS)
		{
		GDS_SEC_TRACE1(2,
				"EXIT dua_init_sec_ctx: bind_id <%d>",*bind_id);
		return(D2_NOERROR);
		}

	if ( gds_sec__mech_2_oid( sec_mech, &gss_mech, status) )
		{
		GDS_SEC_TRACE0(2,"dua_init_sec_ctx: gds_sec__mech_2_oid failed");
		return (D2_ERROR);
		}


	if ((sec_mech == D2_DCE_AUTH) || (sec_mech == D2_STRONG))
		{
		/* check whether one of target name or psap is supplied */
		/* map contents of target name to something known to GSS */
		/* and convert format to gss_name_t */

		if (dua_sec__map_target_name(	sec_mech, &gss_mech,
									target_dsa_dirid, target_dsa_name,
									(unsigned16) target_dsa_psap_len,
									target_dsa_psap,
									&gss_target, status))
			return(D2_ERROR);


		/* set gssflags and handles */
		time_req = GSS_C_INDEFINITE;
		req_flags = GSS_C_MUTUAL_FLAG | GSS_C_REPLAY_FLAG ;
		ctx_h = GSS_C_NO_CONTEXT;
		chan_bindings = GSS_C_NO_CHANNEL_BINDINGS;

		GDS_SEC_TRACE_NAME(3,"dua_init_sec_ctx", gss_target);

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

		/* gss_name and token buffer are freed in dua_conf_sec_ctx */

		/* generate bind_id */

		/* store ctx_h and assign the bind_id */
		if (gds_sec__store_ctx(sec_mech,(gss_cred_id_t) cred_h,
										ctx_h,gss_target, bind_id,status))
				return(D2_ERROR);

		} /* end dce or strong authentication */

	else /* simple authentication */
		{
		/* get user credential structure from internal store */
		if ( gds_sec__get_simple_cred( (gss_cred_id_t) cred_h, 
										&my_creds, status))
			return(D2_ERROR);

		GDS_SEC_TRACE1(3,"dua_init_sec_ctx cred->name <%s>", my_creds->d2_c_name);

		/* return name + password structure in sec_token */
		if ( dua_gen_simp_anon_sec_ctx( sec_mech, my_creds, sec_token))
			{
			GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_FAILURE);
			sprintf(status->logstring,
			"Construction of simple credentials failed for <%s>\n",
			my_creds->d2_c_name);
			GDS_SEC_TRACE1(3,"%s",status->logstring );
			return(D2_ERROR);
			}
		/* generate ctx handle  */
		tmp_h = (gss_ctx_id_t) sec_token;
		ctx_h =  tmp_h;
		gss_target=GSS_C_NO_NAME;


		/* free simple cred element from internal storage */

		if ( gds_sec__free_cred(sec_mech, (gss_cred_id_t) cred_h, status))
			{
			GDS_SEC_TRACE0(3,"could not free simple creds");
			}
		else
			{
			GDS_SEC_TRACE0(3,"free simple creds Ok");
			}
		}

	GDS_SEC_TRACE2(3,
		"EXIT dua_init_sec_ctx: context_handle:<%d> bind_id <%d>"
		,ctx_h,*bind_id);
	GDS_SEC_TRACE_TOKEN(4,
		"EXIT dua_init_sec_ctx: output_token"
		,sec_token);

	return(d2_ret);
	}


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dua_conf_sec_ctx                                  */
/*                                                                    */
/* Description	: This function is used to accomplish the establish-  */
/*                ment of the security context between the DUA process*/
/*                and target DSA. The security context is deleted     */
/*                immediately after completion.                       */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dua_conf_sec_ctx(
	signed16		sec_mech,			/* IN security mechanism */	
	unsigned32		bind_id,            /* IN reference to ctx handle */
	D2_str			*return_sec_token,  /* IN token from the target */
	D2_name_string  *target_dsa,	    /* OUT name of the peer DSA */
	D2_sec_status	*status)           /* OUT status and logging info */
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

	GDS_SEC_TRACE0(2,"ENTRY dua_conf_sec_ctx");
	GDS_SEC_TRACE2(3,"sec_mech:<%d> bind_id:<%d>",
		sec_mech,bind_id);
	if ( return_sec_token != NULL)
		{
		GDS_SEC_TRACE_TOKEN(3,"return_token",return_sec_token);
		}

	strcpy((char*) *target_dsa, ""); 
	gds_sec__init_status(status);

	if ( sec_mech == D2_ANONYMOUS)
		{
		GDS_SEC_TRACE1(2,
			"EXIT dua_conf_sec_ctx: target_dsa <%s>",*target_dsa);
		return(D2_NOERROR);
		}

	if ( gds_sec__mech_2_oid( sec_mech, &gss_mech, status) )
		{
		GDS_SEC_TRACE0(2,"dua_conf_sec_ctx: gds_sec__mech_2_oid failed");
		return (D2_ERROR);
		}


	if ((sec_mech == D2_DCE_AUTH) || (sec_mech == D2_STRONG))
		{

		GDS_SEC_TRACE0(3,"calling gds_sec__get_ctx");
		/* retrieve ctx_h, cred_h and target_name from store */
		if (gds_sec__get_ctx (sec_mech, bind_id, &ctx_h, &cred_h,
								&gss_target,status)) 
			return(D2_ERROR);

		GDS_SEC_TRACE1(3,"got ctx_h <%d>", ctx_h);
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
			input_token.value =  (void*) 0;
			}

		GDS_SEC_TRACE_NAME(3,"dua_conf_sec_ctx", gss_target);
		GDS_SEC_TRACE2(3,"dua_conf_sec_ctx : cred_h %d ctx_h %d",
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
			/* this case is handled in DUA, so dont return error */
			d2_ret= D2_NOERROR ;
			}

		/* free the oid */
		free(gss_mech.elements);

		/* release the buffer issued by gss_init_sec_context if any */
		if (output_token.length != 0 )
			{
			ret = gss_release_buffer(&minor_st, &output_token);
			if (ret != GSS_S_COMPLETE)
				{
				gds_sec__map_gss_error( &gss_mech, ret,
										minor_st, status);  
				GDS_SEC_TRACE2(2,
				"gss_release_buffer (from gss_init_sec_context) failed:<%d>:%s",ret,status->logstring );
				/* ignore failure of this routine but produce logging str */
				}
			}

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
				GDS_SEC_TRACE2(2,
				"gss_release_buffer (from gss_delete_sec_context) failed:<%d>:%s",ret,status->logstring );
				/* ignore failure of this routine but produce logging str */
				}
			}
		/* free ctx_h and assign the bind_id */
		if (gds_sec__free_ctx(sec_mech,bind_id,status))
			{
			/* ignore failure of this routine but produce logging str */
			}
		} /* end dce or strong authentication */

	else /* simple authentication */
		{

		GDS_SEC_TRACE0(5,"calling dua_conf_simp_anon_sec_ctx");
		/* get DSA name from the return token */

		dua_conf_simp_anon_sec_ctx(sec_mech,return_sec_token,target_dsa);
		}


	GDS_SEC_TRACE1(2,"EXIT dua_conf_sec_ctx: target_dsa <%s>",*target_dsa);
	return(d2_ret);
	}
