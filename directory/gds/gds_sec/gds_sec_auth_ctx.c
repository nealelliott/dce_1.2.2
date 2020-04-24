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
 * $Log: gds_sec_auth_ctx.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:55:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:34  root]
 * 
 * Revision 1.1.2.6  1994/07/06  15:07:22  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:15:46  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:46:55  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:00  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:45:15  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:02  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/12  14:49:12  zeliff
 * 	ot10623: prefix include of gssapi.h with dce
 * 	[1994/05/12  14:43:45  zeliff]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:38  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  15:06:07  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:12  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:15:52  keutel
 * 	creation
 * 	[1994/03/21  15:55:45  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: gds_sec_auth_ctx.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:23 $";
#endif

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : gds_sec_auth_sec.c                                  */
/*                                                                    */
/* Description  : The module contains the definition of internal      */
/*                functions of the security layer dealing with the    */
/*                storage of cred handles and context handles         */
/*                                                                    */
/* Date         : January 05, 1994                                    */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gds.h>
#include <dce/d2dir.h>

#include <dce/gssapi.h>
/*#include <dce/gssapi-int.h>*/

#include <gds_sec.h>
#include <gds_sec_int.h>
#include <gds_sec_util.h>
#include <gds_sec_auth_ctx.h>
#include <D00_gds_sec_protect.h>

/* D2_STRONG is the max number of mechanism supported by one 
implementation .  */

static  D2_sec_auth_ctx glob_auth_ctx[D2_STRONG +1 ] = 
{ 
(gss_cred_id_t) 0, 0, (D2_sec_bind_ctx*) 0, (D2_credent *) 0 ,(D2_sec_auth_ctx*) 0,
(gss_cred_id_t) 0, 0, (D2_sec_bind_ctx*) 0, (D2_credent *) 0 ,(D2_sec_auth_ctx*) 0,
(gss_cred_id_t) 0, 0, (D2_sec_bind_ctx*) 0, (D2_credent *) 0 ,(D2_sec_auth_ctx*) 0,
(gss_cred_id_t) 0, 0, (D2_sec_bind_ctx*) 0, (D2_credent *) 0 ,(D2_sec_auth_ctx*) 0,
(gss_cred_id_t) 0, 0, (D2_sec_bind_ctx*) 0, (D2_credent *) 0 ,(D2_sec_auth_ctx*) 0,
(gss_cred_id_t) 0, 0, (D2_sec_bind_ctx*) 0, (D2_credent *) 0 ,(D2_sec_auth_ctx*) 0,
(gss_cred_id_t) 0, 0, (D2_sec_bind_ctx*) 0, (D2_credent *) 0 ,(D2_sec_auth_ctx*) 0,
};
/* Dont forget to update this initialization if Auth Mech Enumeration changes ! */

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__store_simple_cred                        */
/*                                                                    */
/* Description	: This function is used to store a credential handle  */
/*                in case of simple Aauthentication. A D2_credent     */
/*                structure is stored in D2_sec_auth_ctx              */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__store_simple_cred (
  /* IN */    D2_credent *simple_cred,
  /* OUT */   gss_cred_id_t *cred_h,
  /* OUT */   D2_sec_status   *status )
	{
	D2_sec_auth_ctx *auth_ctx , *tmp;
	gss_cred_id_t  tmp_h ;
	D2_credent *s_cred;
	D2_password *s_passwd;
	signed32 len;
	signed32 d2_ret = D2_NOERROR;

	auth_ctx = &(glob_auth_ctx[D2_SIMPLE]) ;
	tmp_h = (gss_cred_id_t)1;

	/* alloc space for D2_credential struct */

	GDS_SEC_MALLOC( s_passwd, D2_password* , sizeof(D2_password), status);
	s_passwd->d2_p_len = simple_cred->d2_c_passwd->d2_p_len ;

	if (s_passwd->d2_p_len > 0 )
		{
		GDS_SEC_MALLOC( s_passwd->d2_p_pw, Octet_string , s_passwd->d2_p_len , status);
		memcpy (s_passwd->d2_p_pw, 
			simple_cred->d2_c_passwd->d2_p_pw,
			simple_cred->d2_c_passwd->d2_p_len );
		}
	else
		s_passwd->d2_p_pw = (Octet_string) 0;

	GDS_SEC_MALLOC( s_cred, D2_credent* , sizeof(D2_credent) , status);

	len = 1 + strlen ((char*) (simple_cred->d2_c_name));

	GDS_SEC_MALLOC( s_cred->d2_c_name , D2_name_string , len , status);

	memcpy (s_cred->d2_c_name, simple_cred->d2_c_name,len);
	s_cred->d2_c_passwd = s_passwd;

	/* CHANGE there should always exist only one cred_h per sec_mech. */
	/* Only exception is when gdsditadm call on gds_set_master_bind.  */
	/* The way it is implemented now may cause the process to grow.   */

	BEGIN_gds_sec_cred_table_LOCK

	if ( auth_ctx->cred_h == 0 ) /* first struct in list is free */
		{
		auth_ctx->cred_h = tmp_h;
		auth_ctx->simple_cred = s_cred;
		*cred_h = auth_ctx->cred_h ;
		auth_ctx->next = (D2_sec_auth_ctx*) 0;
		}
	else  /* more than one auth for this mech type ! */     
		{
		tmp_h ++;

		tmp = (D2_sec_auth_ctx*)D27_MALLOC(svc_c_sev_warning,sizeof(D2_sec_auth_ctx));
		if ( tmp == NULL)
			{
			GDS_SEC_SET_STATUS(status,D2_S_ALLOC_ERROR,D2_TEMP_ERR,D2_SYS_ERR);
			d2_ret = D2_ERROR;
			goto auth_return;
			}

		while ( auth_ctx->next != (D2_sec_auth_ctx*) 0 )
			{
			auth_ctx=auth_ctx->next;
			tmp_h ++;
			}
		auth_ctx->next = tmp;
		tmp->cred_h = tmp_h;
		tmp->num_binds = 0 ;
		tmp->simple_cred = s_cred;
		tmp->bind_ctx = (D2_sec_bind_ctx*) 0 ;
		tmp->next = (D2_sec_auth_ctx *) 0;
		*cred_h = tmp->cred_h ;
		}

	auth_return: ;

	END_gds_sec_cred_table_LOCK

	return(d2_ret);
	}
/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__store_cred                               */
/*                                                                    */
/* Description	: This function is used to store a credential handle  */
/*                in the static array glob_auth_ctx                   */
/*                The D2_name_strings contains the Name for which     */
/*                credentials are acquired in case of strong Auth OR  */
/*                a constant indicating whether the credentials       */
/*                include the master_bind privilege in case of DCE    */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__store_cred (
  /* IN */    signed16    desired_sec_mech, 
  /* IN */    gss_cred_id_t cred_h,
  /* IN */    D2_name_string  owner,
  /* OUT */   D2_sec_status   *status )
	{
	D2_sec_auth_ctx *auth_ctx , *tmp;
	D2_credent *g_cred;
	signed32 len;
	signed32 d2_ret = D2_NOERROR;

	auth_ctx = &(glob_auth_ctx[desired_sec_mech]) ;

	/* alloc space for D2_credential struct */

	GDS_SEC_MALLOC( g_cred, D2_credent* , sizeof(D2_credent) , status);

	len = 1 + strlen ((char*) (owner));

	GDS_SEC_MALLOC( g_cred->d2_c_name , D2_name_string , len , status);

	memcpy (g_cred->d2_c_name, owner,len);
	g_cred->d2_c_passwd = (D2_password*) 0 ;

	/* there should always exist only one cred_h per sec_mech/name */

BEGIN_gds_sec_cred_table_LOCK

	if ( auth_ctx->cred_h == 0 ) /* first struct in list is free */
		{
		auth_ctx->cred_h = cred_h;
		auth_ctx->simple_cred = g_cred;
		auth_ctx->next = (D2_sec_auth_ctx *) 0;
		}
	else  /* more than one auth for this mech type ! */     
		{
		tmp = (D2_sec_auth_ctx*)D27_MALLOC(svc_c_sev_warning,sizeof(D2_sec_auth_ctx));
		if ( tmp == NULL)
			{
			GDS_SEC_SET_STATUS(status,D2_S_ALLOC_ERROR,D2_TEMP_ERR,D2_SYS_ERR);
			d2_ret = D2_ERROR;
			goto auth_return;
			}

		while ( auth_ctx->next != (D2_sec_auth_ctx*) 0 )
			auth_ctx=auth_ctx->next;
		auth_ctx->next = tmp;
		tmp->cred_h = cred_h;
		tmp->num_binds = 0 ;
		tmp->simple_cred = g_cred ;
		tmp->bind_ctx = (D2_sec_bind_ctx*) 0 ;
		tmp->next = (D2_sec_auth_ctx *) 0;
		}

	auth_return: ;

	END_gds_sec_cred_table_LOCK

	return(d2_ret);
	}


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__get_simple_cred                          */
/*                                                                    */
/* Description	: retrieves D2_credent struct stored with a cred_h    */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__get_simple_cred(          
	/* IN  */   gss_cred_id_t cred_h,                              
	/* OUT */   D2_credent      **simple_cred ,
	/* OUT */   D2_sec_status   *status )
	{
	D2_sec_auth_ctx *auth_ctx ;
	D2_sec_auth_ctx  *tmp = (D2_sec_auth_ctx*) 0 ;
	Bool found = FALSE;
	signed32 d2_ret = D2_NOERROR;

	auth_ctx = &(glob_auth_ctx[D2_SIMPLE]) ;

	BEGIN_gds_sec_cred_table_LOCK

	if ( auth_ctx->cred_h == cred_h )
		found = TRUE;

	while ((auth_ctx->next != (D2_sec_auth_ctx*)0 ) && (found == FALSE))
		{
		tmp = auth_ctx ;
		auth_ctx = auth_ctx->next ;
		if ( auth_ctx->cred_h == cred_h )
			{
			found = TRUE;
			}
		}
	if ( found == FALSE)
		{
		GDS_SEC_TRACE1(4,"no cred <%d> for authmech SIMPLE",cred_h);
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR, D2_S_FAILURE);
		sprintf(status->logstring
				,"no cred handle %d stored for authmech SIMPLE\n"
				,cred_h);
		d2_ret = D2_ERROR;
		goto auth_return;
		}
	/* auth_ctx points to the struct we are looking for */

	/* return D2_credntial struct pointer */ 
	*simple_cred = auth_ctx->simple_cred;

	auth_return: ;

	END_gds_sec_cred_table_LOCK

	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__test_cred                                */
/*                                                                    */
/* Description	: retrieves cred_h stored with the respective mech-   */
/*                type and the name supplied                          */
/*                returnes error if no entry is found.                */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__test_cred(                                            
	/* IN */    signed16    desired_sec_mech,
	/* IN */    D2_name_string  owner,
	/* OUT */   gss_cred_id_t *cred_h,                              
	/* OUT */   D2_sec_status   *status )
	{
	D2_sec_auth_ctx *auth_ctx ;
	D2_sec_auth_ctx  *tmp = (D2_sec_auth_ctx*) 0 ;
	signed32 ret;
	Bool found = FALSE;

	GDS_SEC_TRACE2(5,"gds_sec__test_cred: testing for <%s> for authmech %d",owner, desired_sec_mech);

	*cred_h = GSS_C_NO_CREDENTIAL ;

	auth_ctx = &(glob_auth_ctx[desired_sec_mech]) ;

	BEGIN_gds_sec_cred_table_LOCK

	do
		{
		tmp = auth_ctx ;
		auth_ctx = auth_ctx->next ;
		if ( tmp->cred_h != 0 )
			{
			ret = strcmp (	(void*) owner,
							(void*) tmp->simple_cred->d2_c_name);
			if ( ret == 0 )
				found = TRUE;
			}
		}while ((auth_ctx != (D2_sec_auth_ctx*)0 ) && (found == FALSE));

	END_gds_sec_cred_table_LOCK

	if ( found == FALSE)
		{
		GDS_SEC_TRACE2(4,"no cred stored for <%s> for authmech %d",owner, desired_sec_mech);
		return(D2_ERROR);
		}
	/* tmp points to the struct we are looking for */

	/*	return cred_h */
	*cred_h = tmp->cred_h;

	return(D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__get_cred                                 */
/*                                                                    */
/* Description	: retrieves cred_h stored with the respective mech-   */
/*                type. Used in the DSA where the cred_h is not       */
/*                returned by the sec function and exactly one        */
/*                credential per sec_mech is used in one process.     */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__get_cred(                                             
	/* IN */    signed16    desired_sec_mech,
	/* OUT */   gss_cred_id_t *cred_h,                              
	/* OUT */   D2_sec_status   *status )
	{
	D2_sec_auth_ctx *auth_ctx ;
	signed32 d2_ret = D2_NOERROR;

	auth_ctx = &(glob_auth_ctx[desired_sec_mech]) ;
	*cred_h = GSS_C_NO_CREDENTIAL ;

	BEGIN_gds_sec_cred_table_LOCK

	if ( auth_ctx->cred_h == 0 ) /* no cred stored for this sec_mech */
		{
		GDS_SEC_TRACE1(2,"no cred for authmech %d",desired_sec_mech);         
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR,D2_S_FAILURE);
		sprintf(status->logstring ,"no cred handle stored for authmech %d\n"
				,desired_sec_mech);
		d2_ret = D2_ERROR;
		goto auth_return;
		}
	else if (auth_ctx->next != (D2_sec_auth_ctx*) 0) 
						/* more than one auth for this mech type ! */     
		{
		GDS_SEC_TRACE1(2,"more than one cred handle stored for authmech %d"
							,desired_sec_mech);         
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR,D2_S_FAILURE);
		sprintf(status->logstring
				,"more than one cred handle stored for authmech %d\n"
				,desired_sec_mech);
		d2_ret = D2_ERROR;
		goto auth_return;
		}
	else /* exactly what is expected when this func is called */
		{
		*cred_h = auth_ctx->cred_h;
		}

	auth_return: ;

	END_gds_sec_cred_table_LOCK
	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__free_cred                                */
/*                                                                    */
/* Description	: removes a stored cred_h from the list and frees     */
/*                memory allocated for the D2_sec_auth_ctx struct     */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__free_cred(                                            
/* IN */    signed16    desired_sec_mech,                       
/* IN */    gss_cred_id_t cred_h,                               
/* OUT */   D2_sec_status   *status )                          
	{
	D2_sec_auth_ctx *auth_ctx ;
	D2_sec_auth_ctx  *tmp = (D2_sec_auth_ctx*) 0 ;
	Bool found = FALSE;
	signed32 d2_ret = D2_NOERROR;

	auth_ctx = &(glob_auth_ctx[desired_sec_mech]) ;

	BEGIN_gds_sec_cred_table_LOCK

	if ( auth_ctx->cred_h == cred_h )
		found = TRUE;

	while ((auth_ctx->next != (D2_sec_auth_ctx*)0 ) && (found == FALSE))
		{
		tmp = auth_ctx ;
		auth_ctx = auth_ctx->next ;
		if ( auth_ctx->cred_h == cred_h )
			{
			found = TRUE;
			}
		}
	if ( found == FALSE)
		{
		GDS_SEC_TRACE2(4,"no cred <%d> for authmech %d",cred_h, desired_sec_mech);
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR, D2_S_FAILURE);
		sprintf(status->logstring
				,"no cred handle %d stored for authmech %d\n"
				,cred_h, desired_sec_mech);
		d2_ret = D2_ERROR;
		goto auth_return;
		}
	/* auth_ctx points to the struct we are looking for */

	/* free attached bind_ctx's */
	gds_sec__free_all_ctx(desired_sec_mech , auth_ctx->bind_ctx);

	/* tmp is equal 0 if auth_ctx is the first in list */
	if ( tmp == (D2_sec_auth_ctx*) 0 )
		{
		if (desired_sec_mech == D2_SIMPLE)
			{
			/* free the simple_cred struct */
			if (auth_ctx->simple_cred != (D2_credent*)0)
				{
				gds_sec__free_s_cred (auth_ctx->simple_cred);
				auth_ctx->simple_cred =  (D2_credent*)0;
				}
			}
		auth_ctx->cred_h = GSS_C_NO_CREDENTIAL;
		}
	else /* is was not the first in the list */
		{
		tmp->next = auth_ctx->next;
		if (desired_sec_mech == D2_SIMPLE)
			{
			/* free the simple_cred struct */
			if (auth_ctx->simple_cred != (D2_credent*)0)
				{
				gds_sec__free_s_cred (auth_ctx->simple_cred);
				auth_ctx->simple_cred =  (D2_credent*)0;
				}
			}
		free (auth_ctx);
		}

	auth_return: ;

	END_gds_sec_cred_table_LOCK
	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__store_ctx                                */
/*                                                                    */
/* Description	: stores ctx handle, target name and bind_d in the    */
/*                static array glob_auth_ctx                          */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__store_ctx  (
  /* IN */    signed16    desired_sec_mech, 
  /* IN */    gss_cred_id_t cred_h,
  /* IN */    gss_ctx_id_t ctx_h,
  /* IN */    gss_name_t target,
  /* OUT */   unsigned32 *bind_id,
  /* OUT */   D2_sec_status   *status )
	{
	D2_sec_auth_ctx *auth_ctx ;
	D2_sec_bind_ctx *bind_ctx, *tmp ;
	Bool found = FALSE;
	signed32 d2_ret = D2_NOERROR;


	auth_ctx = &(glob_auth_ctx[desired_sec_mech]) ;

	BEGIN_gds_sec_cred_table_LOCK

	/* find the concerned auth_ctx */ 
	while ((found != TRUE ) && (auth_ctx != (D2_sec_auth_ctx*) 0 ))
		{
		if ( auth_ctx->cred_h != cred_h)
			auth_ctx = auth_ctx->next;
		else
			found = TRUE;
		}
	if (found == FALSE)		 /* cred not stored ! */
		{
		GDS_SEC_TRACE2(2,"no cred handle %d stored for authmech %d"
						,cred_h,desired_sec_mech);         
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR,D2_S_FAILURE);
		sprintf(status->logstring
				,"no cred handle %d stored for authmech %d\n"
				,cred_h, desired_sec_mech);
		d2_ret = D2_ERROR;
		goto auth_return;
		}

	tmp = (D2_sec_bind_ctx*)D27_MALLOC(svc_c_sev_warning,sizeof(D2_sec_bind_ctx));
	if ( tmp == NULL)
		{
		GDS_SEC_SET_STATUS(status,D2_S_ALLOC_ERROR,D2_TEMP_ERR,D2_SYS_ERR);
		d2_ret = D2_ERROR;
		goto auth_return;
		}

	tmp->ctx_h = ctx_h;
	tmp->target = target;
	tmp->bind_id = (unsigned32) ctx_h;
	tmp->next  = (D2_sec_bind_ctx *) 0 ;

	bind_ctx = auth_ctx->bind_ctx;

	/* Its the first bind cxt for the concerned auth ctx */
	if ( bind_ctx == (D2_sec_bind_ctx * ) 0 )
		{
		auth_ctx->bind_ctx = tmp ;
		}
	else 
		{
		/* Skip the used bind cxt's */
		while ( bind_ctx->next != (D2_sec_bind_ctx*) 0 )
			{
			bind_ctx = bind_ctx->next ;
			}
		bind_ctx->next = tmp;
		}

	auth_ctx->num_binds ++;

	/* This implies that gssapi generates ctx_handles unique for the
	   whole process, not only unique for all binds based on a parti-
	   cular credential */
	*bind_id = (unsigned32) ctx_h;

	auth_return: ;

	END_gds_sec_cred_table_LOCK
	return (d2_ret);
	}
/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__get_ctx                                  */
/*                                                                    */
/* Description	: retrieves ctx_h  stored with the respective cred_h  */
/*                and mech_type.                                      */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__get_ctx(
	/* IN */    signed16    desired_sec_mech,
	/* IN */    unsigned32 bind_id,
	/* OUT */   gss_ctx_id_t *ctx_h,
	/* OUT */   gss_cred_id_t *cred_h,
	/* OUT */   gss_name_t *target,
	/* OUT */   D2_sec_status   *status )
	{

	D2_sec_auth_ctx *auth_ctx ;
	D2_sec_bind_ctx *bind_ctx ;
	Bool found = FALSE;
	signed32 d2_ret = D2_NOERROR;


	auth_ctx = &(glob_auth_ctx[desired_sec_mech]) ;

	BEGIN_gds_sec_cred_table_LOCK

	while ((found != TRUE ) && (auth_ctx != (D2_sec_auth_ctx*) 0 ))
		{
		bind_ctx = auth_ctx->bind_ctx;
		*cred_h  = auth_ctx->cred_h ;
		while ((found != TRUE) && (bind_ctx != (D2_sec_bind_ctx*) 0 ))
			{
			if ( bind_ctx->bind_id == bind_id)
				{
				*ctx_h = bind_ctx->ctx_h;
				*target = bind_ctx->target;
				found = TRUE;
				}
			else
				{
				bind_ctx = bind_ctx->next;
				}
			}
		auth_ctx = auth_ctx->next;
		}
	if (found == FALSE)		 /* context not stored ! */
		{
		*ctx_h = GSS_C_NO_CONTEXT;
		*cred_h = GSS_C_NO_CREDENTIAL;
		*target = GSS_C_NO_NAME;
		GDS_SEC_TRACE2(2,"no bind_id %d for authmech %d",bind_id,desired_sec_mech);         
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR,D2_S_FAILURE);
		sprintf(status->logstring
				,"no bind_id %d stored for authmech %d\n"
				,bind_id ,desired_sec_mech);
		d2_ret = D2_ERROR;
		}

	END_gds_sec_cred_table_LOCK

	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__free_ctx                                 */
/*                                                                    */
/* Description	: frees the memory allocates for a D2_sec_bind_ctx    */
/*                struct                                              */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__free_ctx( 
	/* IN */    signed16    desired_sec_mech,                       
	/* IN */    unsigned32 bind_id,                                   
	/* OUT */   D2_sec_status   *status )                          
	{
	D2_sec_auth_ctx *auth_ctx ;
	D2_sec_bind_ctx *bind_ctx ;
	D2_sec_bind_ctx *tmp = (D2_sec_bind_ctx *) 0 ;
	Bool found = FALSE;
	signed32 d2_ret = D2_NOERROR;


	auth_ctx = &(glob_auth_ctx[desired_sec_mech]) ;

	BEGIN_gds_sec_cred_table_LOCK

	while ((found != TRUE ) && (auth_ctx != (D2_sec_auth_ctx*) 0 ))
		{
		bind_ctx = auth_ctx->bind_ctx;
		while ((found != TRUE) && (bind_ctx != (D2_sec_bind_ctx*) 0 ))
			{
			if ( bind_ctx->bind_id == bind_id)
				{
				found = TRUE;
				auth_ctx->num_binds --;
				}
			else
				{
				tmp = bind_ctx;
				bind_ctx = bind_ctx->next;
				}
			}
		if (found != TRUE)
			auth_ctx = auth_ctx->next;
		}
	if (found == FALSE)		 /* bind_id not stored ! */
		{
		GDS_SEC_TRACE2(2,"no bind_id %d for authmech %d",bind_id,desired_sec_mech);         
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR,D2_S_FAILURE);
		sprintf(status->logstring
				,"no bind_id %d stored for authmech %d\n"
				,bind_id ,desired_sec_mech);
		d2_ret = D2_ERROR;
		goto auth_return;
		}
	if ( tmp == (D2_sec_bind_ctx *) 0 )
		{
		auth_ctx->bind_ctx = bind_ctx->next;
		}
	else
		{
		tmp->next = bind_ctx->next; 
		}
	/* free the found bind_ctx struct */
	if ( desired_sec_mech == D2_SIMPLE)
		{
		if ( bind_ctx->ctx_h != (gss_ctx_id_t) 0 )
			{
			if (((D2_str*) (bind_ctx->ctx_h))->d2_value != NULL)
				{
				free ( ((D2_str*) (bind_ctx->ctx_h))->d2_value);
				}

 			free((void *)(bind_ctx->ctx_h));
			}
		}
	free(bind_ctx);

	auth_return: ;

	END_gds_sec_cred_table_LOCK
	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__free_s_cred                              */
/*                                                                    */
/* Description	: frees the memory allocated for name + pasword       */
/*                                                                    */
/* Date         : February  15, 1994                                  */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
void gds_sec__free_s_cred (
	/*	IN */	D2_credent *s_cred)
	{
	D2_password *pw;

	if (s_cred->d2_c_name != (D2_name_string) 0 )
		{
		memset((void*) (s_cred->d2_c_name), 0, strlen((char*) s_cred->d2_c_name));
		free (s_cred->d2_c_name);
		}

	pw = s_cred->d2_c_passwd;

	if (pw != (D2_password*) 0 )
		{
		memset((void*) pw->d2_p_pw, 0, pw->d2_p_len);
		free (pw->d2_p_pw);
		free (pw);
		}
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__free_all_ctx                             */
/*                                                                    */
/* Description	: frees the memory allocates for all bind_ctx structs */
/*                belonging to a auth_ctx struct                      */
/*                                                                    */
/* Date         : January  05, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
void gds_sec__free_all_ctx(      
	/* IN */ 	signed16  desired_sec_mech,
	/* IN */ 	D2_sec_bind_ctx *base_ctx )
	{

	if (base_ctx == (D2_sec_bind_ctx *) 0 )
		return;
	if ( base_ctx->next != (D2_sec_bind_ctx *) 0 )
		gds_sec__free_all_ctx(desired_sec_mech,base_ctx->next);

	/*
	if ( desired_sec_mech == D2_SIMPLE)
		{
		if (base_ctx->ctx_h != (gss_ctx_id_t) 0 )
			free ((D2_str*) base_ctx->ctx_h);
		}
	*/
	free ( base_ctx );
	base_ctx = (D2_sec_bind_ctx * ) 0 ;
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	Test function gds_sec__print_auth_ctx             */
/*                                                                    */
/* Description	: This function is used to print the  contents        */
/*                of the static array glob_auth_ctx                   */
/*--------------------------------------------------------------------*/

void   gds_sec__print_auth_ctx (
			/* in */ signed16 sec_mech  )
	{
	D2_sec_auth_ctx *auth_ctx ,*tmp;
	D2_sec_bind_ctx *tmp2;
	int i =1;

	auth_ctx = &(glob_auth_ctx[sec_mech]) ;

	fprintf(stderr,"\n");
	do 
		{
		fprintf(stderr,"Authentication Context No %d  - Mechanism %d \n",i, sec_mech);
		fprintf(stderr,"\tauth_ctx->cred_h = <%d> \n",auth_ctx->cred_h);
		fprintf(stderr,"\tauth_ctx->num_binds = <%d> \n",auth_ctx->num_binds);
		fprintf(stderr,"\tauth_ctx->simple_cred->name <%s>\n",
							auth_ctx->simple_cred->d2_c_name);
		fflush(stderr);
		tmp2 = auth_ctx->bind_ctx;
		while ( tmp2 != (D2_sec_bind_ctx *)0)
			{
			fprintf(stderr,"\t\tctx_h <%d> bind_id <%d> target <%s>\n",
					tmp2->ctx_h, tmp2->bind_id,tmp2->target);
			fflush(stderr);
			tmp2 = tmp2->next;
			}
		fflush(stderr);
		tmp = auth_ctx->next;
		auth_ctx = auth_ctx->next;
		i++;
		} 
	while ( tmp != (D2_sec_auth_ctx *)0 );
	}
