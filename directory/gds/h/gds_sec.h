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
 * $Log: gds_sec.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:01:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:22  root]
 * 
 * Revision 1.1.2.6  1994/08/16  08:16:08  marrek
 * 	Fix for delta update (ot 11617).
 * 	[1994/08/12  12:00:20  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:47:20  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:41:56  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:46:22  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:26  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  15:56:07  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:00  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:18:36  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:24  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:21  marrek
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:54:04  marrek]
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
/* File Name    : gds_sec.h                                           */
/*                                                                    */
/* Description  : The header contains defines typedefs and function   */
/*                prototypes for the function visible from GDS code   */
/*                                                                    */
/* Date         : December 21, 1993                                   */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

#ifndef _GDS_SEC_H
#define _GDS_SEC_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gds_sec_rcsid[] = "@(#)$RCSfile: gds_sec.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:12 $";
#endif

/*  #define DUMMY_GSSAPI */

/* defines */

#define D2_S_NOT_SPEC   0
#define D2_S_FAILURE 2000
#define D2_S_SEC_VIOLATION 2001
#define D2_S_EXPIRED_CRED 2002 
#define D2_S_BAD_NAME 2003 
#define D2_S_INAPPR_AUTH 2004 
#define D2_S_INSUF_BUF 2005 
#define D2_S_TDT_OPEN 2006 
#define D2_S_TDT_READ 2007 
#define D2_S_NOT_SUPPORTED 2008 
#define D2_S_UNKNOWN 2010 
#define D2_S_GSSDCE	2011
#define D2_S_DCE_ERA	2012
#define D2_S_DCE_DLG	2013
#define D2_S_DCE_NO_MB_ATTR	2014
#define D2_S_DCE_NO_EXT_ATTR	2015
#define D2_S_ALLOC_ERROR 2100

/* independent bits for trust level */
#define TDT_NOT_TRUSTED		0
#define TDT_TRUSTED_FOR_READ	1
#define TDT_TRUSTED_FOR_MOD	2
#define TDT_TRUSTED_FOR_ALL	TDT_TRUSTED_FOR_READ | TDT_TRUSTED_FOR_MOD

/* Typedefs for types used at the sec_layer interface */

typedef signed32 D2_cred_handle_t;
typedef signed32 gds_sec_handle_t;

typedef struct
        {   char *          logstring;      /* logging information    */
            signed16        d2_retcode;     /* return code            */
            signed16        d2_errclass;    /* error class            */
            signed16        d2_errvalue;    /* error value            */
        } D2_sec_status;                        


/* Prototypes of functions visible on the sec_layer interface */

signed32 dua_acquire_cred(
		/*	IN	*/	signed16				desired_sec_mech, 
		/*	IN	*/	D2_credent			*my_creds,
		/*IN/OUT*/	Bool				*master_bind,
		/*	OUT	*/	D2_cred_handle_t	*cred_h,
		/*	OUT	*/	D2_sec_status		*status); 

signed32 dua_init_sec_ctx( 
		/*	IN	*/	signed16			desired_sec_mech, 
		/*	IN	*/ 	D2_name_string		target_dsa_name,
		/*	IN	*/ 	signed16			target_dsa_dirid,
		/*	IN	*/ 	Psap_string			target_dsa_psap,	
		/*	IN	*/ 	signed16			target_dsa_psap_len,	
		/*	IN	*/ 	D2_cred_handle_t	cred_h,
		/*	OUT	*/ 	unsigned32			*bind_id,
		/*	OUT	*/	D2_str				*sec_token,		
		/*	OUT	*/	D2_sec_status		*status );

signed32 dua_conf_sec_ctx(
		/*	IN	*/	signed16		desired_sec_mech, 
		/*	IN	*/ 	unsigned32		bind_id,
		/*	IN	*/	D2_str			*return_sec_token,
		/*	OUT	*/	D2_name_string	*target_dsa,
		/*	OUT	*/	D2_sec_status	*status );

signed32 dsa_acquire_cred(
		/*	IN	*/	signed16			desired_sec_mech, 
		/*	IN	*/	D2_name_string	my_name,
		/*	OUT	*/	D2_sec_status	*status); 

signed32 dsa_del_cred(
		/*	IN	*/	signed16		desired_sec_mech, 
		/*	OUT	*/	D2_sec_status	*status); 

signed32 dsa_accept_sec_ctx(
		/*	IN	*/	signed16		desired_sec_mech, 
		/*	IN	*/	D2_str		*client_token,
		/*	OUT	*/	D2_str		*return_token,
		/*	OUT	*/	D2_name_string	*client_name,
		/*	OUT	*/	Bool			*master_bind,
		/*	OUT	*/	D2_sec_status	*status); 

signed32 dsa_init_sec_ctx(
		/*	IN	*/	signed16		desired_sec_mech, 
		/*	IN	*/ 	D2_name_string	target_dsa_name,
		/*	IN	*/ 	Psap_string		target_dsa_psap,
		/*	OUT	*/ 	unsigned32		*bind_id,
		/*	OUT	*/	D2_str 			*sec_token,		
		/*	OUT	*/	D2_sec_status	*status );

signed32 dsa_conf_sec_ctx(
		/*	IN	*/	signed16		desired_sec_mech, 
		/*	IN	*/ 	unsigned32		bind_id,
		/*	IN	*/	D2_str			*return_sec_token,
		/*	OUT	*/	D2_sec_status	*status );

signed32 dsa_get_tdt_entry(
		/*	IN	*/	D2_name_string	dsa_name, 
		/* 	OUT	*/	signed32		*trust_level,
		/*  OUT	*/	D2_sec_status  	*status );

signed32 gds_set_master_bind(                                           
		/*  IN  */  D2_name_string	target_dsa, 
		/*  OUT */  Bool			*masterb_poss,
		/* OUT */   D2_sec_status	*status );
                                                                  
/*** Prototypes for functions called by the dsa_... functions in case ***/
/*** anonymous or simple credentials								  ***/

signed16 dsa_accept_simp_anon_sec_ctx(
	signed16     desired_sec_mech,	/* IN  - desired mechanism	*/
	D2_str 		*client_token, 		/* IN  - client token		*/
	D2_str 		*return_token,  	/* OUT - security token -	*/
									/*       contains D23_simcr_info*/
	D2_name_string 	*client_name,	/* OUT - requestor name			*/
	boolean         *master_bind,	/* OUT - TRUE if master bind 	*/
									/*	     FALSE otherwise		*/
	D2_sec_status 	*sec_status);	/* OUT - status					*/

signed16 dsa_conf_simp_anon_sec_ctx(
	signed16 desired_sec_mech,	/* IN  - security mechanism	*/
	D2_str   *sec_token, 		/* IN  - bind result token	*/
	D2_sec_status *status);		/* OUT - error status		*/

signed16 dsa_gen_simp_anon_sec_ctx(
	signed16 desired_sec_mech,	/* IN  - security mechanism	*/
	D2_str   *sec_token, 		/* OUT - bind request token	*/
	D2_sec_status *sec_status);	/* OUT - error status		*/

#endif /* ifndef _GDS_SEC_H */
