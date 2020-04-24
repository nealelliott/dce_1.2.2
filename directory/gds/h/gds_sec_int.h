/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gds_sec_int.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:01:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:24  root]
 * 
 * Revision 1.1.2.9  1994/09/23  12:14:39  marrek
 * 	Enabled masterbind mechanism for DCE authentication.
 * 	[1994/09/23  12:14:09  marrek]
 * 
 * Revision 1.1.2.8  1994/09/12  14:37:44  keutel
 * 	OT 11769
 * 	[1994/09/12  14:37:09  keutel]
 * 
 * Revision 1.1.2.7  1994/07/15  13:29:30  marrek
 * 	Remove empty comment line.
 * 	[1994/07/15  10:02:50  marrek]
 * 
 * Revision 1.1.2.6  1994/07/07  07:02:41  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:58:52  marrek]
 * 
 * Revision 1.1.2.5  1994/07/06  15:07:35  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:35  marrek]
 * 
 * Revision 1.1.2.4  1994/06/21  14:47:22  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:42:01  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:24  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:56:09  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:46:25  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:06  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:18:39  keutel
 * 	creation
 * 	[1994/03/21  15:53:03  keutel]
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
/* File Name    : gds_sec_int.h                                       */
/*                                                                    */
/* Description  : The header contains defines typedefs and function   */
/*                prototypes for the Security layer internal function */
/*                                                                    */
/* Date         : December 21, 1993                                   */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

#ifndef _GDS_SEC_INT_H
#define _GDS_SEC_INT_H

/*  #define DUMMY_GSSAPI */

#if !defined lint && defined GDS_RCS_ID
static char gds_sec_int_rcsid[] = "@(#)$RCSfile: gds_sec_int.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:14 $";
#endif


#ifdef DUMMY_GSSAPI
typedef char *gss_name_t;
#endif

/* definitions of supported Security mechanism types */
	/* from  DEC GSSAPI gssapi-support.c */
#define DCE_OID 	"\x2b\x18\x09\x08" 
	/* from SAMSON GSSAPI scdirgss.h */
#define STRONG_OID 	"\x2B\xCE\x0F\x01\x01\x90\x08\x05\x02\x01"

#define X500NAME_OID 	"\x2B\xCE\x0F\x01\x01\x90\x08\x05\x02\x0A"

/* to be replaced by a GSSDCE_C_OID_DCENAME value from dec's gssapi.h */

/* done
#define GSSDCE_C_OID_DCENAME 	"\x2B\xCE\x0F\x01\x01\x90\x08\x05\x02\x0B"
*/


#define GDS_SEC_MAX_LOGSTRING 2048 

/* constants for tdt handling functions */

#define TDT_NAME "dsatdt"  /* name of the file containing the TDT */
#define TDT_FSEP ':'      /* field separator in the TDT */
#define TDT_RSEP '\n'     /* record separator in the TDT */

#define TDT_NOT_SPEC    -1

#define TDT_MAXBUF 1024

#define TDT_TYPE_VAL_SEP	'=' 
#define TDT_AVA_SEP			',' 
#define TDT_RDN_SEP			'/' 

#define CACHE_OK 0
#define CACHE_EMPTY 1
#define CACHE_NOMEM 2
#define CACHE_NOPERM 3
#define CACHE_NULL 4
#define CACHE_NOWRITE 5
#define CACHE_NOOPEN 6
#define CACHE_NOREAD 7
#define CACHE_NOFOUND 8
#define CACHE_NOTYPE 9
#define CACHE_NOFLUSH 10

/* constants to identify creds in the internal storage */
#define D2_DCE_USER_PRIV (D2_name_string) "DCE_PRIV"
#define D2_DCE_USER      (D2_name_string) "DCE_USER"
#define D2_DSA_INIT_CRED (D2_name_string) "DSA_INIT"
#define D2_DSA_CRED		 (D2_name_string) "DSA_ACCEPT"
#define D2_MB_UNUSED	 (D2_name_string) "MB_UNUSED"

/* modes to read data from the cache */
#define CACHE_FIRST 1
#define CACHE_NEXT 2
#define CACHE_KEY 3

/* typedefs for tdt handling functions */

typedef struct {
	int File;
	int seek;
	int Type;
	int FieldSep;
	int RecordSep;
	} chandle_t_s;

typedef chandle_t_s *chandle_t;

signed32 dsa_sec__get_init_cred  (
		/* I/O */  gss_cred_id_t    *cred_h ,
		/* OUT */   D2_sec_status   *status);                           

signed32 dua_sec__map_client_name(
		/*	IN	*/	signed16		auth_method,
		/*  IN */  	gss_OID			gss_mech,
		/*	IN	*/	D2_name_string	client_DN,
		/*	I/O	*/	Bool			*master_bind,
		/*	OUT	*/	gss_name_t 		*client_sec_name,
		/* 	OUT	*/	D2_sec_status	*status );

signed32 dua_sec__map_target_name( 
		/* IN */  	signed16 		desired_sec_mech,
		/* IN */  	gss_OID			gss_mech,
		/* IN */  	signed16		directory_id,
		/* IN */  	D2_name_string	target_dsa_DN,
		/* IN */  	unsigned16		target_dsa_psap_len,
		/* IN */  	Psap_string		target_dsa_psap,
		/* OUT */ 	gss_name_t 		*target_dsa_sec_name,
		/* OUT */ 	D2_sec_status 	*status );

signed32 dsa_sec__map_client_name(
		/*	IN	*/	signed16		auth_method,
		/*  IN */  	gss_OID			gss_mech,
		/*	IN	*/	D2_name_string	DSA_DN,
		/*	OUT	*/	gss_name_t 		*DSA_sec_name,
		/* 	OUT	*/	D2_sec_status	*status );

signed32 dsa_sec__map_target_name( 
		/* IN */  	signed16 		desired_sec_mech,
		/* IN */  	gss_OID			gss_mech,
		/* IN */  	D2_name_string	target_dsa_DN,
		/* IN */  	Psap_string		target_dsa_psap,
		/* OUT */ 	gss_name_t 		*target_dsa_sec_name,
		/* OUT */ 	D2_sec_status 	*status );


signed32 dsa_sec__get_client_name( 
		/*	IN	*/	signed16		auth_method,
		/*  IN */  	gss_OID			gss_mech,
		/*	IN	*/	gss_ctx_id_t  	ctx_h,
		/*	IN	*/	gss_name_t 		dua_gss_name,
		/*	OUT	*/	D2_name_string 	*requester_name,
		/*	OUT	*/	Bool			*master_bind,
		/*	OUT	*/	D2_sec_status 	*status );

void gds_sec__buf_2_str(
		/* IN   */  gss_buffer_t buffer_name,
		/* OUT  */  D2_name_string *name );

signed32 dua_sec__psap_2_name( 
		/*	IN/OUT*/D2_name_string 	*target_name,
		/*  IN  */ 	Psap_string		target_psap,
		/*	OUT	*/	D2_sec_status 	*status );

void gds_constr_log_string( 
		/*  IN  */  gss_OID             mech_type,
		/* IN */  	int			status_value,
		/* OUT */ 	D2_sec_status	*status );

signed32  gds_sec__strong_map_name( 
		/*  IN  */  gss_OID			gss_mech,
		/* IN */  	D2_name_string  dir_name,   
		/* OUT */	gss_name_t      *sec_name,
		/*	OUT	*/	D2_sec_status 	*status );

signed32  gds_sec__dce_map_name( 
		/*  IN  */  gss_OID			gss_mech,
		/* IN */  	D2_name_string  dce_princ_name,
		/* OUT */	gss_name_t      *sec_name,
		/*	OUT	*/	D2_sec_status 	*status );

signed32  gds_sec__mech_2_oid( 
		/*  IN  */  signed16			desired_sec_mech,
		/* OUT  */  gss_OID_desc		*gss_mech,
		/*	OUT	*/	D2_sec_status 		*status );

void gds_sec__init_status(
		/* IN/OUT */ D2_sec_status		*status);

void gds_sec__map_gss_error(
		/*  IN  */  gss_OID  			mech_type, 
		/*  IN  */  OM_uint32			major_st, 
		/*  IN  */  OM_uint32			minor_st, 
		/*	I/O	*/	D2_sec_status		*status);

signed32 gds_sec__get_mb_lctx(
		/*	OUT	*/	D2_sec_status		*status);

signed32 dsa_sec__get_ext_attr(
		/*  IN  */  gss_ctx_id_t		ctx_h,
		/*  IN  */  byte				*own_dsa_name,
		/*  OUT  */	D2_name_string		*dua_name,
		/*  OUT  */	Bool				*master_bind,
		/*	OUT	*/	D2_sec_status		*status);

signed32 gds_sec__conf_target(
		/*  IN  */	D2_name_string		distinguished_name,
		/*  IN  */	D2_name_string		principal_name,
		/*	OUT	*/	D2_sec_status		*status);

signed32 dua_sec__get_dir_princ(
	/*  IN	*/	signed16  		directory_id,
	/*  IN	*/	D2_name_string  dsa_dn,
	/*  IN  */  unsigned16     	target_psap_len,
	/*  IN  */  Psap_string     target_psap,
	/*  OUT */  D2_name_string  dsa_pn,
	/*  OUT */  D2_sec_status   *status );

signed32 dsa_sec__get_dn_from_rgy(
	/*  IN  */	gss_name_t			dua_gss_name,
	/*  IN  */  D2_name_string      dsa_name,
	/*  OUT */	D2_name_string		*dua_name,
	/*  OUT */	Bool				*master_bind,
	/*	OUT	*/	D2_sec_status		*status);

signed32 gds_sec__separate_pn ( 
	/*	IN	*/ unsigned_char_t * global_name ,
	/*	OUT	*/ unsigned_char_t * c_name ,
	/*	OUT	*/ unsigned_char_t * p_name );

signed32    gds_sec__iapl_2_print_name(
	/*  IN  */  D2_name_string iapl_name,                               
	/*  OUT */  unsigned char tdt_name[]);

signed32    gds_sec__print_2_iapl_name(
	/*  IN  */  unsigned char print_name[],
	/*  OUT */  unsigned char iapl_name[]);

signed32  gds_sec__oid_2_prtoid(
	/*	IN	*/	unsigned char oid[],
	/*	OUT	*/	unsigned char prt_oid[]);

signed32  gds_sec__prtoid_2_oid(
	/*  IN  */  unsigned char prt_oid[],
	/*  OUT */  unsigned char oid[]);

void dsa__close_tdt(
		/* in  */	chandle_t	handle);

signed32 dsa__open_tdt(
		/* in  */	char *name,
		/* in  */	int fieldsep,
		/* in  */	int recordsep,
		/* out */	chandle_t *handle);

signed32 dsa__read_tdt_record (
		/* in  */	chandle_t handle,
		/* in  */	int readmode,
		/* in  */	int keypos,
		/* in  */	char *keyfield,
		/* out */	char *record);

signed16 dsa__try_tdt_mask(
				char *record,
				int keypos,                     
				int FieldSep,
				char *keyfield);

signed16 dsa__tdt_read_perm (
				char *record,
				int keypos,
				int FieldSep );

void print_cache_handle(                                            
		/* in */ chandle_t handle) ; 

signed32	gds_sec__set_ext_attr(
		/*  OUT */  D2_sec_status       *status);


#endif /* ifndef _GDS_SEC_INT_H */
