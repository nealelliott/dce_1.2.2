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
 * $Log: gds_sec_auth_ctx.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:01:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:23  root]
 * 
 * Revision 1.1.2.4  1994/06/21  14:47:21  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:41:58  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:28  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:56:08  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:46:22  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:02  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:18:37  keutel
 * 	creation
 * 	[1994/03/21  15:52:59  keutel]
 * 
 * $EndLog$
 */
/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : gds_sec_auth_ctx.h                                  */
/*                                                                    */
/* Description  : The header contains defines typedefs and function   */
/*                prototypes for the cred and context handle storage  */
/*                functions of the security function layer            */
/*                                                                    */
/* Date         : January 05, 1994                                    */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

#ifndef _GDS_SEC_AUTH_CTX_H
#define _GDS_SEC_AUTH_CTX_H

#if !defined lint && defined GDS_RCS_ID
static char gds_sec_auth_rcsid[] = "@(#)$RCSfile: gds_sec_auth_ctx.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:13 $";
#endif

/* includes */

#include <D00_gds_sec_protect.h>  /* THREADSAFE MACROS */

/* constants for cred and context storage */


/* typedefs for cred and context storage */

typedef struct _D2_sec_bind_ctx
	{
	gss_ctx_id_t ctx_h;
	unsigned32 bind_id;
	gss_name_t target;
	struct _D2_sec_bind_ctx *next;
	} D2_sec_bind_ctx ;

typedef struct _D2_sec_auth_ctx
	{
	gss_cred_id_t cred_h;
	signed32 num_binds;
	D2_sec_bind_ctx *bind_ctx;
	D2_credent    *simple_cred;
	struct _D2_sec_auth_ctx *next;
	} D2_sec_auth_ctx ;


/* prototypes for cred and context storage */

signed32 gds_sec__store_simple_cred( 
		/* IN */  	D2_credent * simple_cred,
		/* OUT */  	gss_cred_id_t *cred_h,
		/* OUT */ 	D2_sec_status 	*status );

signed32 gds_sec__store_cred( 
		/* IN */  	signed16		desired_sec_mech,
		/* IN */  	gss_cred_id_t	cred_h,
		/* IN */  	D2_name_string	owner,
		/* OUT */ 	D2_sec_status 	*status );

signed32 gds_sec__test_cred( 
		/* IN */  	signed16		desired_sec_mech,
		/* IN */  	D2_name_string	owner,
		/* OUT */  	gss_cred_id_t	*cred_h,
		/* OUT */ 	D2_sec_status 	*status );

signed32 gds_sec__get_simple_cred( 
		/* IN  */  	gss_cred_id_t cred_h,
		/* OUT */  	D2_credent **simple_cred,
		/* OUT */ 	D2_sec_status 	*status );

signed32 gds_sec__get_cred( 
		/* IN */  	signed16 	desired_sec_mech,
		/* OUT */  	gss_cred_id_t *cred_h,
		/* OUT */ 	D2_sec_status 	*status );

signed32 gds_sec__free_cred( 
		/* IN */  	signed16 	desired_sec_mech,
		/* IN */  	gss_cred_id_t cred_h,
		/* OUT */ 	D2_sec_status 	*status );

signed32 gds_sec__store_ctx( 
		/* IN */  	signed16 	desired_sec_mech,
		/* IN */  	gss_cred_id_t cred_h,
		/* IN */	gss_ctx_id_t ctx_h,
		/* IN */	gss_name_t target,
		/* OUT */	unsigned32 *bind_id,
		/* OUT */ 	D2_sec_status 	*status );

signed32 gds_sec__get_ctx( 
		/* IN */  	signed16 	desired_sec_mech,
		/* IN */  	unsigned32 bind_id,
		/* OUT */	gss_ctx_id_t *ctx_h,
		/* OUT */	gss_cred_id_t *cred_h,
		/* OUT */	gss_name_t *target,
		/* OUT */ 	D2_sec_status 	*status );

signed32 gds_sec__free_ctx( 
		/* IN */  	signed16 	desired_sec_mech,
		/* IN */	unsigned32 bind_id,
		/* OUT */ 	D2_sec_status 	*status );

void gds_sec__free_all_ctx( 
		/* IN */  	signed16 	desired_sec_mech,
		/* IN */  	D2_sec_bind_ctx *base_ctx);

void gds_sec__free_s_cred ( 
		/* IN */  	D2_credent *creds );

void   gds_sec__print_auth_ctx ( 
        /* in */ signed16 sec_mech  );

#endif /* ifndef _GDS_SEC_AUTH_CTX_H */
