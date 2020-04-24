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
 * $Log: gds_sec_rgy.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:37  root]
 * 
 * Revision 1.1.2.13  1994/09/30  13:59:17  keutel
 * 	again
 * 	[1994/09/30  13:59:04  keutel]
 * 
 * Revision 1.1.2.12  1994/09/30  12:02:49  keutel
 * 	OT 11880: again sec_rgy_site_open_update ---> sec_rgy_site_open_query
 * 	[1994/09/30  09:39:44  keutel]
 * 
 * Revision 1.1.2.11  1994/09/23  12:14:38  marrek
 * 	Enabled masterbind mechanism for DCE authentication.
 * 	[1994/09/23  12:14:06  marrek]
 * 
 * Revision 1.1.2.10  1994/09/12  14:37:43  keutel
 * 	OT 11769
 * 	[1994/09/12  14:37:05  keutel]
 * 
 * 	again
 * 	[1994/09/12  11:25:12  keutel]
 * 
 * 	prototyping for sec_rgy_attr_lookup_by_name added
 * 	[1994/09/12  11:23:28  keutel]
 * 
 * 	workaround for 11880: sec_rgy_site_open_query instead of sec_rgy_site_open_update
 * 	[1994/09/12  11:01:05  keutel]
 * 
 * Revision 1.1.2.9  1994/09/01  15:14:09  keutel
 * 	OT 11880: followed guidelines from Mike Burati
 * 	[1994/09/01  14:16:14  keutel]
 * 
 * Revision 1.1.2.8  1994/09/01  12:20:07  keutel
 * 	OT 11880: buffer for sec_login_set_extended_attrs
 * 	[1994/08/31  15:50:02  keutel]
 * 
 * Revision 1.1.2.7  1994/08/26  17:14:08  keutel
 * 	OT 11859
 * 	[1994/08/26  14:20:46  keutel]
 * 
 * Revision 1.1.2.6  1994/07/06  15:07:25  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:00  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:46:57  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:12  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:45:17  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:05  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/12  14:49:15  zeliff
 * 	ot10623: prefix include of gssapi.h with dce
 * 	[1994/05/12  14:43:49  zeliff]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:25  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:15:56  keutel
 * 	creation
 * 	[1994/03/21  15:55:53  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: gds_sec_rgy.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:27 $";
#endif

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : gds_sec_rgy.c                                       */
/*                                                                    */
/* Description  : The module contains the definition of function      */
/*                that set the extended attribute in the security     */
/*				  context and functions that retrieve data from the   */
/*				  DCE rgy and EPACs       							  */
/*                                                                    */
/* Date         : February 11, 1994                                   */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gds.h>
#include <d2dir.h>

#include <dce/gssapi.h>
/*#include <dce/gssapi-int.h>*/

#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/rgybase.h>
#include <dce/binding.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_cred.h>

#include <gds_sec.h>
#include <gds_sec_int.h>
#include <gds_sec_auth_ctx.h>
#include <gds_sec_util.h>

#define GLOBAL_DCE_NAME_PREFIX	"/.../"

/* The following 2 must correspond to the rgy schema extension  */
/* attribute Id of the extended rgy attribute "X500_DN" */

#define UUID_EXT_ATTR_DN "c5949eba-384a-11cd-8cba-080009353559"


/* attribute Id of the extended rgy attribute "X500_DSA_Admin" */

#define UUID_EXT_ATTR_MB "c6a51456-384a-11cd-b6ef-080009353559"


/* prototypes of internal functions */

static signed32 dsa_sec__fill_ext_attr(
	/*	IN	*/	sec_attr_t		*extended_attr,
	/*	IN	*/	D2_name_string	own_dsa_name,
	/*	OUT	*/	D2_name_string	*dua_name,
	/*	OUT	*/	Bool			*master_bind,
	/*	OUT	*/	signed32		*found,
	/*	OUT	*/	D2_sec_status	*status);

static signed32 gds_sec__compare_names(
	/*	IN	*/	D2_name_string	dsa_dn,
	/*	IN	*/	idl_char		*rgy_printable_name);

static void gds_sec__inq_err_text(
	/*	IN	*/	unsigned long	dce_status,
	/*	IN	*/	char			*text,
	/*	OUT	*/	unsigned char	*dce_err_text);

static signed32 gds_sec__set_dn_mb(
	/*	IN	*/	D2_name_string target_dsa,
	/*	OUT	*/	Bool *masterb_poss, 
	/* OUT */ 	D2_sec_status	*status );

static signed32 gds_sec__eval_mb (
	/*	IN	*/	sec_attr_t  	*attr ,
	/*	IN	*/	D2_name_string  dsa_name  ,
	/*	OUT	*/	Bool			*mb ,
	/* OUT */   D2_sec_status   *status );


static uuid_t dn_attr_id = {
	0xc5949eba,        
	0x384a, 
	0x11cd, 
	0x8c, 
	0xba, 
	{0x08, 0x00, 0x09, 0x35, 0x35, 0x59}  
	}; 

static uuid_t mb_attr_id = {
	0xc6a51456,        
	0x384a, 
	0x11cd, 
	0xb6, 
	0xef, 
	{0x08, 0x00, 0x09, 0x35, 0x35, 0x59}  
	}; 


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_sec__get_ext_attr                             */
/*                                                                    */
/* Description	: Retrieves extended attributes from the security     */
/*                context refered to by ctx_h                         */
/*                Called from the GDS target application              */
/*                                                                    */
/* Date         : February 16, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 dsa_sec__get_ext_attr(
	/*	IN	*/	gss_ctx_id_t	ctx_h,
	/*	IN	*/	D2_name_string	own_dsa_name,
	/*	OUT	*/	D2_name_string	*dua_name,
	/*	OUT	*/	Bool			*master_bind, 
	/*	OUT	*/	D2_sec_status	*status )
	{
	OM_uint32 ret , minor_st;
	signed32 d2_ret;

	unsigned char dce_e_string[dce_c_error_string_len];
	sec_cred_pa_handle_t  epac_handle;
	sec_attr_t extended_attr;
	sec_cred_attr_cursor_t epac_cursor;
	rpc_authz_cred_handle_t epac_list_handle;
	error_status_t dce_status;


	signed32 found = 0;
	signed32 tmp_found = 0;
	d2_ret = D2_NOERROR;

	GDS_SEC_TRACE1(3,"ENTRY dsa_sec__get_ext_attr ctx_h <%d>",ctx_h);

	/* get a handle to the chain of EPACs */
	ret = gssdce_extract_creds_from_sec_context ( &minor_st,
												ctx_h,
												&epac_list_handle);
	if ( ret != GSS_S_COMPLETE )
		{
		gds_sec__map_gss_error( GSSDCE_C_OID_DCE_KRBV5_DES, ret, minor_st, status);
		GDS_SEC_TRACE2(2,"gssdce_extract_creds_from_sec_context failed:<%d>:%s",ret,status->logstring );
		return(D2_ERROR);
		}
	
	GDS_SEC_TRACE0(5,"gssdce_extract_creds_from_sec_context returns ok");
	/* get a handle to the initiators EPAC in the chain */

	epac_handle = sec_cred_get_initiator ( epac_list_handle, &dce_status);
	if ( dce_status != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		gds_sec__inq_err_text(dce_status,"sec_cred_get_initiator",dce_e_string);
		strcpy((char*) status->logstring, (char*) dce_e_string);
		GDS_SEC_TRACE1(2,"sec_cred_get_initiator: %s",dce_e_string);
		return (D2_ERROR);
		}

	GDS_SEC_TRACE0(5,"sec_cred_get_initiator returns ok");

	/* initialize the sec_cred attr cursor */

	sec_cred_initialize_attr_cursor( &epac_cursor, &dce_status);
	if ( dce_status != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		gds_sec__inq_err_text(dce_status,"sec_cred_initialize_attr_cursor",dce_e_string);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		GDS_SEC_TRACE1(2,"sec_cred_initialize_attr_curso: %s",dce_e_string);
		return (D2_ERROR);
		}

	GDS_SEC_TRACE0(3,"sec_cred_initialize_attr_cursor returns ok");

	/* retrieve the EPACs attributes */
	while ( (found < 3) && (dce_status == error_status_ok))
		{
		sec_cred_get_extended_attrs	(	epac_handle,
									&epac_cursor,
									&extended_attr,
									&dce_status);
		switch (dce_status)
			{
			case error_status_ok:
				GDS_SEC_TRACE0(3,"sec_cred_get_extended_attrs OK");
				if(dsa_sec__fill_ext_attr(&extended_attr,
										own_dsa_name,
										dua_name,
										master_bind,
										&tmp_found,
										status))
					return(D2_ERROR);

				found += tmp_found;
				break;
			case sec_cred_s_no_more_entries:
				GDS_SEC_TRACE0(3,"sec_cred_get_extended_attrs NO MORE ENTRIES");
				switch (found)
					{
					case 3:
						/* both GDS relevant attr found */	
						GDS_SEC_TRACE0(3,"Extended Attributes DN and MB found");
						d2_ret= D2_NOERROR;
						break;
					case 2:
						/* only DN attr found */
						GDS_SEC_SET_STATUS(status,D2_WARNING,D2_MESSAGE,D2_S_DCE_NO_MB_ATTR);
						sprintf(status->logstring,                              
						"Master_bind attribute missing in security context\n");
						GDS_SEC_TRACE1(3,"%s",status->logstring);
						d2_ret = D2_NOERROR;
						break;
					case 1:
						/* only master_bind attr found */
						d2_ret = D2_ERROR;
						GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_NO_EXT_ATTR);
						sprintf(status->logstring,
						"DN attribute missing in security context\n");
						GDS_SEC_TRACE1(2,"%s",status->logstring);
						break;
					case 0:
						/* no attr found */
						d2_ret = D2_ERROR;
						break;
					default:
						/* strange things happened */
						GDS_SEC_SET_STATUS(status,D2_WARNING,D2_MESSAGE,D2_S_DCE_DLG);
						sprintf(status->logstring,                              
						"Multiple occurencies of GDS attributes in security context\n");
						GDS_SEC_TRACE1(2,"%s",status->logstring);
						d2_ret = D2_NOERROR;
						break;
					}
				break;
			default:
				GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
				gds_sec__inq_err_text(dce_status,
					"sec_cred_get_extended_attrs",dce_e_string);
				strcpy((char*)status->logstring, (char*)dce_e_string);
				d2_ret = D2_ERROR;
				GDS_SEC_TRACE1(2,"sec_cred_get_extended_attrs: %s",dce_e_string);
				break;
			}
		}


	if (d2_ret == D2_ERROR)
		{
		GDS_SEC_TRACE0(3,"EXIT dsa_sec__get_ext_attr ERROR");
		}
	else
		{
		GDS_SEC_TRACE2(3,"EXIT dsa_sec__get_ext_attr dua_name <%s> MB %d",*dua_name,*master_bind);
		}

	return(d2_ret);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__conf_target                              */
/*                                                                    */
/* Description	: Verifies the correctness of the dsa's principal name*/
/*                by query of the concerned rgy server.               */
/*                                                                    */
/* Date         : February 16, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__conf_target(
		/*  IN  */	D2_name_string		distinguished_name,
		/*  IN  */	D2_name_string		principal_name,
		/*	OUT	*/	D2_sec_status		*status)

	{

	Bool status_lock;
	unsigned char dce_e_string[dce_c_error_string_len];
	sec_rgy_handle_t rgy_ctx;
	unsigned_char_t cell_name[1024] , local_pname[1024];
	sec_attr_t attr_returned[2];
	sec_rgy_name_t	rgy_attr_princ_name;
	error_status_t dce_status;
	int ret;
	signed32 d2_ret;
	sec_attr_t dn_attr_returned;


	GDS_SEC_TRACE0(3,"ENTRY gds_sec__conf_target");
	GDS_SEC_TRACE1(4,"retrieved from dir DN <%s>", distinguished_name);
	GDS_SEC_TRACE1(4,"retrieved from dir PN <%s>", principal_name);

	status_lock = FALSE;

	/* check the principal name for global name format */
	
	if (principal_name == (D2_name_string) 0 )
		{
		GDS_SEC_SET_STATUS(status,D2_CALL_ERR,D2_PAR_ERR,D2_S_BAD_NAME);
		sprintf(status->logstring,
			"Empty Principal name was supplied for target");
		GDS_SEC_TRACE1(2,"%s",status->logstring);
		return(D2_ERROR);
		}
	ret = strncmp (	(char*) principal_name,
					GLOBAL_DCE_NAME_PREFIX,
					strlen(GLOBAL_DCE_NAME_PREFIX));
		
	if ( ret != 0 )
		{
		GDS_SEC_SET_STATUS(status,D2_CALL_ERR,D2_PAR_ERR,D2_S_BAD_NAME);
		sprintf(status->logstring,
			"No GLOBAL principal name was supplied for target");
		GDS_SEC_TRACE1(2,"%s",status->logstring);
		return (D2_ERROR);
		}


	/* separate the cellname from the principals local name */

	/* the following name semantics is assumed for principal names of */
	/* GDS users and GDS servers : /.../CELLNAME/whatever   */
	/* where CELLNAME is either a typed name or a string without '=' */

	if ( gds_sec__separate_pn ( (unsigned_char_t *) principal_name ,
								cell_name , local_pname ))
		{
		GDS_SEC_SET_STATUS(status,D2_CALL_ERR,D2_PAR_ERR,D2_S_BAD_NAME);
		sprintf(status->logstring,
			"Could not separate cell name from principal name");
		GDS_SEC_TRACE1(2,"%s",status->logstring);
		return (D2_ERROR);
		}
	GDS_SEC_TRACE2(2,"cell name <%s> principal name <%s>",cell_name,local_pname);

	
	/* bind to the concerned rgy server                              */

	/* By default sec_rgy_site_open* uses package integrity which   */
	/* is sufficient for this purpose */

	sec_rgy_site_open_query (	(unsigned_char_t*) cell_name, 
								&rgy_ctx, &dce_status );	

	if ( dce_status != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_ERA);
		gds_sec__inq_err_text(dce_status,"sec_rgy_site_open_query",dce_e_string);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		GDS_SEC_TRACE1(2,"sec_rgy_site_open_query: %s",dce_e_string);
		return (D2_ERROR);
		}

	GDS_SEC_TRACE0(4,"sec_rgy_site_open_query OK");

	strcpy ( (char*) rgy_attr_princ_name, (char*) local_pname);

	sec_rgy_attr_lookup_by_name (	rgy_ctx,
								sec_rgy_domain_person ,
								rgy_attr_princ_name,
								(unsigned char *) "X500_DN" ,
								&dn_attr_returned,
								&dce_status);
	if ( dce_status != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_ERA);
		gds_sec__inq_err_text(dce_status,"sec_rgy_attr_lookup_by_name",dce_e_string);
		strcpy((char*)status->logstring,(char*) dce_e_string);
		GDS_SEC_TRACE1(2,"sec_rgy_attr_lookup_by_id: %s",dce_e_string);
		status_lock = TRUE;
		d2_ret = D2_ERROR;
		goto close;
		}

	else
		{
		GDS_SEC_TRACE0(4,"sec_rgy_attr_lookup_by_name OK");
		
		if ( dn_attr_returned.attr_value.attr_encoding  != sec_attr_enc_printstring )
			{
			GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_ERA);
			sprintf(status->logstring,
			"sec_rgy_attr_lookup_by_name returned wrong attr encoding <%d> ",
			dn_attr_returned.attr_value.attr_encoding);
			GDS_SEC_TRACE1(2,"%s",status->logstring);
			status_lock = TRUE;
			d2_ret = D2_ERROR;
			}

	/* here we have got the DN of the specified principal */

	GDS_SEC_TRACE1(4,"sec_rgy_attr_lookup_by_name returned DN <%s>", dn_attr_returned.attr_value.tagged_union.printstring);
		
		}

	close:

	/* terminate the binding to the rgy server */
	sec_rgy_site_close (	rgy_ctx,
							&dce_status);

	if ( dce_status != error_status_ok)
		{
		gds_sec__inq_err_text(dce_status,"sec_rgy_site_close",dce_e_string);
		if ( status_lock != TRUE )
			{
			GDS_SEC_SET_STATUS(status,D2_WARNING,D2_MESSAGE,D2_S_DCE_ERA);
			strcpy((char*)status->logstring,(char*) dce_e_string);
			}
		GDS_SEC_TRACE1(2,"sec_rgy_site_close : %s",dce_e_string);
		/*  do not change the status in d2_ret */
		}
	else
		{
		GDS_SEC_TRACE0(2,"sec_rgy_site_close OK");
		}
	

	/* here we have got the DN of the target DSA */

	
	if (gds_sec__compare_names(
				distinguished_name,
				dn_attr_returned.attr_value.tagged_union.printstring))
		{
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_SEC_VIOLATION);

		sprintf(status->logstring,
			"Target DSA name mismatch directory : <%s> rgy:<%s>",
			distinguished_name,
			attr_returned[0].attr_value.tagged_union.printstring);
		GDS_SEC_TRACE1(1,"%s",status->logstring);
		return (D2_ERROR);
		}


	GDS_SEC_TRACE1(3,"target DN <%s> confirmed with rgy",distinguished_name);
	GDS_SEC_TRACE0(3,"EXIT gds_sec__conf_target");
	return(D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_sec__fill_ext_attr                            */
/*                                                                    */
/* Description	: scans the extended attribute and copies to DN or    */
/*                master_bind if one of them is found .               */
/*                sets out param found to 0 if attr isn't gds related */
/*                sets out param found to 1 if attr masterb. is found */
/*                and sets masterbind to TRUE if dsaname is contained */
/*                sets out param found to 2 if attr DN       is found */
/*                and copies clients DN to dua_name                   */
/*                                                                    */
/* Date         : February 16, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

static signed32 dsa_sec__fill_ext_attr(
	/*	IN	*/	sec_attr_t		*extended_attr,
	/*	IN	*/	D2_name_string 	own_dsa_name,
	/*	OUT	*/	D2_name_string	*dua_name,
	/*	OUT	*/	Bool			*master_bind,
	/*	OUT	*/	signed32		*found,
	/*	OUT	*/	D2_sec_status	*status) 
	{

	unsigned32 dce_status;
	unsigned char dce_e_string[dce_c_error_string_len];
	signed32 uret,d2_ret;
	Bool mb_poss;

	GDS_SEC_TRACE0(3,"ENTRY  dsa_sec__fill_ext_attr");

	*found = 0 ;

	uret = uuid_compare(&(extended_attr->attr_id), &dn_attr_id, &dce_status);
	if ( dce_status != error_status_ok )
		{
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		gds_sec__inq_err_text(dce_status,"uuid_compare",dce_e_string);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		GDS_SEC_TRACE1(2,"uuid_compare: %s",dce_e_string);
		return( D2_ERROR);
		}
	if ( uret == 0 )
		{
		GDS_SEC_TRACE0(4,"Found X500_DN");
		if (extended_attr->attr_value.attr_encoding == sec_attr_enc_printstring)
			{
			d2_ret = gds_sec__print_2_iapl_name(
					extended_attr->attr_value.tagged_union.printstring, 
					*dua_name);
			if (d2_ret == D2_ERROR)
				{
				GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_BAD_NAME);
				sprintf(status->logstring,
					"conversion of EPAC DN <%s> to iapl format failed",  
					extended_attr->attr_value.tagged_union.printstring);
				GDS_SEC_TRACE1(2,"%s",status->logstring);
				return( D2_ERROR);
				}
			*found = 2;
			GDS_SEC_TRACE1(4," Found X500_DN <%s>",*dua_name);
			GDS_SEC_TRACE0(3,"EXIT  dsa_sec__fill_ext_attr");
			return (D2_NOERROR);
			}     
		else 
			{
			GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
			sprintf(status->logstring,
			"wrong encoding for ERA X500_DN <%d>, should be printstring",
			extended_attr->attr_value.attr_encoding);
			GDS_SEC_TRACE1(2,"%s" ,status->logstring);
			return (D2_ERROR);
			}
		}

	/* in case anything goes wrong with X500_DSA_Admin dont return an
	error. The calling function is content if only X500_DN could be
	extracted sucessfiully */

	uret = uuid_compare(&(extended_attr->attr_id), &mb_attr_id, &dce_status);
	if ( dce_status != error_status_ok )
		{
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		gds_sec__inq_err_text(dce_status,"uuid_compare",dce_e_string);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		GDS_SEC_TRACE1(2,"uuid_compare: %s",dce_e_string);
		return( D2_NOERROR);
		}
	if ( uret == 0 )
		{
		GDS_SEC_TRACE0(4,"Found X500_DSA_Admin");
		if ( gds_sec__eval_mb (extended_attr, own_dsa_name, &mb_poss, status))
			{
			return(D2_NOERROR);
			}
		*master_bind = mb_poss ;
		*found = 1;
		GDS_SEC_TRACE0(3,"EXIT  dsa_sec__fill_ext_attr");
		return (D2_NOERROR);
		}     
	return(D2_NOERROR);
	}


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_sec__get_dn_from_rgy                          */
/*                                                                    */
/* Description	: gets the ERAs for the specified principal directly  */
/*                from the rgy                                        */
/*                makes use of the fact that the gss_name is          */
/*                semantically equal to the DCE principal name        */
/* Date         : March    23, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed32 dsa_sec__get_dn_from_rgy(
	/*  IN  */	gss_name_t			dua_gss_name,
	/*  IN  */	D2_name_string		dsa_name,
	/*  OUT */	D2_name_string		*dua_name,
	/*  OUT */	Bool				*master_bind,
	/*	OUT	*/	D2_sec_status		*status)

	{

	unsigned char dce_e_string[dce_c_error_string_len];
	sec_rgy_handle_t rgy_ctx;
	sec_attr_t dn_attr_returned;
	sec_rgy_name_t	rgy_attr_princ_name;
	error_status_t dce_status;
	signed32 d2_ret = D2_NOERROR;
	unsigned_char_t rgy_name[100];
	unsigned_char_t cell_name[1024] , local_pname[1024];
	Bool mb_poss, status_lock;


	GDS_SEC_TRACE0(3,"ENTRY dsa_sec__get_dn_from_rgy");

	if (gds_sec__separate_pn ( (unsigned_char_t *)dua_gss_name ,
								cell_name , local_pname ))
		{
		GDS_SEC_SET_STATUS(status,D2_CALL_ERR,D2_PAR_ERR,D2_S_BAD_NAME);
		sprintf(status->logstring,
			"Could not separate cell name from principal name");
		GDS_SEC_TRACE1(2,"%s",status->logstring);
		return (D2_ERROR);
		}
	GDS_SEC_TRACE2(5,"cell name <%s> principal name <%s>",cell_name,local_pname);
	strcpy ( (char *) rgy_attr_princ_name, (char *) local_pname);
	strcpy ( (char *) rgy_name, (char *) cell_name);

	sec_rgy_site_open_query (	rgy_name ,
								&rgy_ctx, &dce_status );	

	if ( dce_status != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_ERA);
		gds_sec__inq_err_text(dce_status,"sec_rgy_site_open_query",dce_e_string);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		GDS_SEC_TRACE1(2,"sec_rgy_site_open_query: %s",dce_e_string);
		return (D2_ERROR);
		}

	GDS_SEC_TRACE0(4,"sec_rgy_site_open_query OK");

	/* read the  X500_DN attribute. If anything fails, return error */ 
	/* before continuing with the next attribute .  */

	sec_rgy_attr_lookup_by_name (	rgy_ctx,
								sec_rgy_domain_person ,
								rgy_attr_princ_name,
								(unsigned char*) "X500_DN" ,
								&dn_attr_returned,
								&dce_status);
	if ( dce_status != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_ERA);
		gds_sec__inq_err_text(dce_status,"sec_rgy_attr_lookup_by_name",dce_e_string);
		strcpy((char*)status->logstring,(char*) dce_e_string);
		GDS_SEC_TRACE1(2,"sec_rgy_attr_lookup_by_name: %s",dce_e_string);
		status_lock = TRUE;
		d2_ret = D2_ERROR;
		goto close;
		}
	else
		{
		GDS_SEC_TRACE0(2,"sec_rgy_attr_lookup_by_name OK");
		
		if ( dn_attr_returned.attr_value.attr_encoding  != sec_attr_enc_printstring )
			{
			status_lock = TRUE;
			GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_ERA);
			sprintf(status->logstring,
			"sec_rgy_attr_lookup_by_name returned wrong attr encoding <%d> ",
			dn_attr_returned.attr_value.attr_encoding);
			GDS_SEC_TRACE1(2,"%s",status->logstring);
			d2_ret = D2_ERROR;
			goto close;
			}

		/* here we have got the DN of the specified principal */

		else
			{
			GDS_SEC_TRACE1(2,"sec_rgy_attr_lookup_by_name returned DN <%s>", dn_attr_returned.attr_value.tagged_union.printstring);
		
			if ( gds_sec__print_2_iapl_name(
				dn_attr_returned.attr_value.tagged_union.printstring, 
				*dua_name))
				{ 
				status_lock = TRUE ;
				GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_BAD_NAME);
				GDS_SEC_TRACE0(2,"gds_sec__print_2_iapl_name failed");
				d2_ret = D2_ERROR ;
				goto close;
				}
			}

		}

	/* get X500_DSA_Admin */
	/* Dont return an error if somethings fails wrt this attr, just
	   set masterbind to FALSE */

	sec_rgy_attr_lookup_by_name (	rgy_ctx,
								sec_rgy_domain_person ,
								rgy_attr_princ_name,
								(unsigned char *) "X500_DSA_Admin" ,
								&dn_attr_returned,
								&dce_status);
	if ( dce_status != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_ERA);
		gds_sec__inq_err_text(dce_status,"sec_rgy_attr_lookup_by_name",dce_e_string);
		strcpy((char*)status->logstring,(char*) dce_e_string);
		GDS_SEC_TRACE1(2,"sec_rgy_attr_lookup_by_name X500_DSA_Admin: %s",dce_e_string);
		status_lock = TRUE;
		d2_ret = D2_NOERROR;
		*master_bind = FALSE;
		goto close;
		}
	else
		{
		GDS_SEC_TRACE0(2,"sec_rgy_attr_lookup_by_name  X500_DSA_Admin OK");
		
		if ( gds_sec__eval_mb (&dn_attr_returned, dsa_name, &mb_poss,status))
			{
			d2_ret = D2_NOERROR;
			}
		*master_bind  = mb_poss;

		}
	close:

	/* terminate the binding to the rgy server */
	sec_rgy_site_close (	rgy_ctx,
							&dce_status);

	if ( dce_status != error_status_ok)
		{
		gds_sec__inq_err_text(dce_status,"sec_rgy_site_close",dce_e_string);
		if ( status_lock != TRUE )
			{
			GDS_SEC_SET_STATUS(status,D2_WARNING,D2_MESSAGE,D2_S_DCE_ERA);
			strcpy((char*)status->logstring,(char*) dce_e_string);
			}
		GDS_SEC_TRACE1(2,"sec_rgy_site_close : %s",dce_e_string);
		/*  do not change the status in d2_ret */
		}
	else
		{
		GDS_SEC_TRACE0(2,"sec_rgy_site_close OK");
		}
	

	GDS_SEC_TRACE2(2,"EXIT dsa_sec__get_dn_from_rgy: dua_name <%s> master_bind <%d>",*dua_name, *master_bind);
	return (d2_ret);
	}


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__separate_pn                              */
/*                                                                    */
/* Description	: separates the cell name and the local pricipal name */
/*                from a global DCE name                              */
/*                given in the format  as it is stored in the rgy     */
/* Date         : March    23, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed32 gds_sec__separate_pn ( 
	/*	IN	*/ unsigned_char_t * global_name ,
	/*	OUT	*/ unsigned_char_t * c_name ,
	/*	OUT	*/ unsigned_char_t * p_name )
	{
	unsigned char *tmp, *pslash, *pequal;
	Bool typed_name = TRUE ;
	unsigned char g_name[1024];

	strcpy ((char*) g_name, (char*) global_name);
	
	pslash  =  g_name + strlen (GLOBAL_DCE_NAME_PREFIX);
	pequal  =  g_name + strlen (GLOBAL_DCE_NAME_PREFIX);


	/* skip nameparts containing '='s */
	while ( typed_name == TRUE )
		{
		tmp = (unsigned char *) strchr ( (char*)pequal , '=');
		if ( tmp == NULL)
			typed_name = FALSE ;
		else
			pequal += (1+(tmp - pequal)) ;
		}
	pslash = (unsigned char *) strchr ( (char*)pequal , '/'); 
	if (pslash == NULL )
		return (D2_ERROR);
	
	*pslash++ = 0x00 ;
	strcpy ((char*)  c_name ,(char*)  g_name);
	strcpy ((char*)  p_name ,(char*)  pslash);
	return (D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__compare_names                            */
/*                                                                    */
/* Description	: checks dirname and rgyname for equality             */
/*                dirname is given in the iapl semantics, rgyname is  */
/*                given in the format  as it is stored in the rgy     */
/* Date         : March    23, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/

static signed32 gds_sec__compare_names(
	/*	IN	*/	D2_name_string	dirname,
	/*	IN	*/	idl_char		*rgyname)
	{
	unsigned char dirname_p[D2_DNL_MAX + 1];

	if (gds_sec__iapl_2_print_name(	dirname, dirname_p))
		{
		GDS_SEC_TRACE1(2,"cannot map dirname <%s> to printable ",dirname);
		return (D2_ERROR);
		}

	if (strcmp((char*)dirname_p, (char*) rgyname ) != 0)
		{
		GDS_SEC_TRACE2(2,"Name mismatch dir:<%s> rgy:<%s>",dirname_p,rgyname);
		return (D2_ERROR);
		}
	else
		return (D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__inq_err_text                             */
/*                                                                    */
/* Description	: gets the dce error text for a given dce error_status*/
/* Date         : March    23, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
static void gds_sec__inq_err_text(
	/*	IN	*/	unsigned long	dce_status,
	/*	IN	*/	char			*text,
	/*	OUT	*/	unsigned char	*dce_err_text)
	{

	int error_st;

	dce_error_inq_text ( (unsigned long) dce_status, 
						dce_err_text, &error_st);
		if ( error_st == -1 )
			{
			sprintf((char*) dce_err_text ," %s returns error <%08x>",
									text, dce_status);
			}
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__set_ext_attr                             */
/*                                                                    */
/* Description	: sets the extended attribute X500_DN in the default  */
/*				  login context.									  */
/* Date         : June    13, 1994                                    */
/* Author       : Sanjay Jain	SNI BA NM 123, Munich, Germany  	  */
/*--------------------------------------------------------------------*/

signed32	gds_sec__set_ext_attr(
	/*	OUT	*/	D2_sec_status		*status)
	{
	error_status_t 		dce_st;
	sec_login_handle_t 	login_ctx, new_login_ctx;
	sec_attr_t 			x500_dn_attr[2];
	unsigned char 		buffer[2024];
	unsigned char 		*dce_e_string;
	unsigned32          num_attr = 1 ;

	GDS_SEC_TRACE0(2,"ENTRY gds_sec__set_ext_attr()");

	dce_e_string = buffer;

	/* get the current login context */

	sec_login_get_current_context(  &login_ctx, &dce_st);

	if ( dce_st != error_status_ok)
		{
		gds_sec__inq_err_text(dce_st,"sec_login_get_current_context",dce_e_string);
		GDS_SEC_TRACE0(2, dce_e_string);
		GDS_SEC_TRACE0(2,"EXIT gds_sec__set_ext_attr()");
		return(D2_ERROR);
		}

	GDS_SEC_TRACE1(3, "got current login context handle <%d>", login_ctx);

	/* set extended attribute required */

	x500_dn_attr[0].attr_id = dn_attr_id;
	x500_dn_attr[0].attr_value.attr_encoding = sec_attr_enc_printstring;
	x500_dn_attr[0].attr_value.tagged_union.printstring  = NULL ;

	GDS_SEC_TRACE0(3, "call on  sec_login_set_extended_attrs ");

	/* set X500_DN */
	new_login_ctx = sec_login_set_extended_attrs(
				login_ctx,
				num_attr,
				x500_dn_attr,
				&dce_st);

	if ( dce_st != error_status_ok)
		{
		gds_sec__inq_err_text(dce_st,"sec_login_set_extended_attrs",dce_e_string);
		GDS_SEC_TRACE0(2, dce_e_string);
		GDS_SEC_TRACE0(2,"EXIT gds_sec__set_ext_attr()");
		return(D2_ERROR);
		}

	GDS_SEC_TRACE0(3, "sec_login_set_extended_attrs returned ok");
	GDS_SEC_TRACE1(3, "got new  context handle<%d>",new_login_ctx);

	/* set the extended login context to be the default one */
	sec_login_set_context(  new_login_ctx, &dce_st); 

	if ( dce_st != error_status_ok)
		{
		gds_sec__inq_err_text(dce_st,"sec_login_set_context",dce_e_string);
		GDS_SEC_TRACE0(2, dce_e_string);
		GDS_SEC_TRACE0(2,"EXIT gds_sec__set_ext_attr()");
		return(D2_ERROR);
		}

	GDS_SEC_TRACE0(3, "New login context set to the default context");
	GDS_SEC_TRACE0(2,"EXIT gds_sec__set_ext_attr()");
	return(D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec_set_master_bind                           */
/*                                                                    */
/* Description	: This function is called from the admin programs     */
/*                before a the d21bind is called. The process may     */
/*                have acquired credentials before or not.            */
/*                                                                    */
/*                The function gds_sec_set_master_bind  release exis- */
/*                ting credentials from gds_sec credential list and   */
/*                from gssapi, if they already are acquired.          */
/*                gds_sec_set_mb_dn ed_attrs is called and if master_ */
/*                bind_possible is returned with FALSE the function   */
/*                returns.                                            */
/*                Then dua_acquire_cred is called with the master_bind*/
/*                flag set to TRUE.                                   */
/*                                                                    */
/* Date         : February 16, 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_set_master_bind(
	/*	IN	*/	D2_name_string target_dsa,
	/*	OUT	*/	Bool *masterb_poss, 
	/*  OUT */ 	D2_sec_status	*status )
	{
	gss_cred_id_t cred_h;
	OM_uint32 gssret;
	signed32 ret;
	D2_cred_handle_t dum_cred_h;
	gss_OID_desc gss_mech ;
	OM_uint32  minor_st;
	Bool mb ;



	GDS_SEC_TRACE0(4,"ENTRY gds_set_master_bind");
	*masterb_poss = FALSE;

	/* ask for a new login context containing both extended attribs */
	/* X500_DN and X_500_DSA_Admin */

	if ( gds_sec__set_dn_mb(target_dsa, masterb_poss, status))
		return(D2_ERROR);

	if (*masterb_poss == FALSE)
		{
		GDS_SEC_TRACE1(2,"masterbind to DSA <%s> not possible",target_dsa);
		GDS_SEC_TRACE0(2,"check values of ERA  <X500_DSA_Admin> for the calling user");
		return (D2_NOERROR);
		}

	mb = TRUE;

	/* check whether old MB creds exist, if so delete them */

	ret = gds_sec__test_cred (D2_DCE_AUTH,(unsigned char *) D2_DCE_USER_PRIV, &cred_h, status);
	if (ret == D2_NOERROR ) 
		{
		GDS_SEC_TRACE0(1," Old MB  creds are present");
		/* delete creds with GSSAPI and  free memory in the  */
		/* private auth-ctx list */
		if (gds_sec__free_cred(D2_DCE_AUTH,cred_h,status))
			{
			GDS_SEC_TRACE1(2,"gds_sec__free_cred failed for cred_h <%d>",cred_h);
			/* ignore failure of this routine but produce logging str */
			}

		/* release creds with GSSAPI, so that a new call on */
		/*	gss_acquire_cred will pick up the new login context */ 
		gssret = gss_release_cred(&minor_st, &cred_h);
		if (gssret != GSS_S_COMPLETE)
			{
			gds_sec__map_gss_error( &gss_mech, gssret,minor_st, status);
			GDS_SEC_TRACE2(2,
			"gss_release_cred  failed:<%d>:%s",gssret,status->logstring );
			return(D2_ERROR);
			}
		GDS_SEC_TRACE0(1,"Released Old MB creds");
		}

	if (dua_acquire_cred(D2_DCE_AUTH, (D2_credent *)0 , &mb, &dum_cred_h ,status))
		{
		GDS_SEC_TRACE0(2,"dua_acquire_cred failed for masterbind");
		*masterb_poss = FALSE;
		return(D2_ERROR);
		}
	
	/* store a D@_MB_UNUSED cred in the internal storage, so that the */
	/* subsequent call oin dua _acquire_cred recognizes that MB creds */
	/* are to return */
	
	if(gds_sec__store_cred(	D2_DCE_AUTH, (gss_cred_id_t) 99,
							D2_MB_UNUSED, status)) 
		{
		GDS_SEC_TRACE0(2,"storing D2_MB_UNUSED creds failed");
		return(D2_ERROR);
		}

	GDS_SEC_TRACE0(4,"EXIT gds_set_master_bind");
	return(D2_NOERROR);
	}


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__set_dn_mb                                */
/*                                                                    */
/* Description	: Asks for a new login context containing the ext.    */
/*                attributes X500_DN and X500_DSA_Admin               */
/*                                                                    */
/*                The value of the ERA X500_DSA_Admin is checked for  */
/*                a match with the supplied target DSA name.          */
/*                                                                    */
/*                If no match is found the login context is cleared   */
/*                from the X500_DSA_Admin attribute and an error is   */
/*                returned.                                           */
/* Date         : June 16, 1994                                       */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

static signed32 gds_sec__set_dn_mb(
	/*	IN	*/	D2_name_string target_dsa,
	/*	OUT	*/	Bool *masterb_poss, 
	/* OUT */ 	D2_sec_status	*status )
	{
	error_status_t dce_st,dst;
	sec_login_handle_t login_ctx, new_login_ctx;
	sec_attr_t x500_attrs[4], attr;
    sec_cred_attr_cursor_t  acursor;
    sec_cred_pa_handle_t    pa_h;
	unsigned char buffer[2024];
	unsigned char *dce_e_string;
	unsigned32 num_attrs ;
	signed32 d2_ret;
	sec_attr_enc_str_array_t era_array_buf;
	Bool mb_poss;

	dce_e_string = buffer;
	era_array_buf.num_strings = 0;
	era_array_buf.strings[0]  =  NULL;

	d2_ret = D2_NOERROR;

	GDS_SEC_TRACE0(4,"ENTRY gds_sec__set_dn_mb");

	x500_attrs[0].attr_id = dn_attr_id;
	x500_attrs[0].attr_value.attr_encoding = sec_attr_enc_printstring;
	x500_attrs[0].attr_value.tagged_union.printstring  = NULL ;

	x500_attrs[1].attr_id = mb_attr_id;
	x500_attrs[1].attr_value.attr_encoding = sec_attr_enc_printstring_array;
	x500_attrs[1].attr_value.tagged_union.string_array  = &era_array_buf;
	num_attrs = 2;
	*masterb_poss = FALSE;

	/* get the current login context */

	sec_login_get_current_context(  &login_ctx, &dce_st);
	if ( dce_st != error_status_ok)
		{
		gds_sec__inq_err_text(dce_st,"sec_login_get_current_context",dce_e_string);
		GDS_SEC_TRACE1(2,"%s",dce_e_string);
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		return (D2_ERROR);
		}
	GDS_SEC_TRACE0(4,"sec_login_get_current_context OK");


	/* set X500_DN and X500_DSA_Admin ERAs in the login context */
	new_login_ctx = sec_login_set_extended_attrs(
				login_ctx,
				num_attrs,
				x500_attrs,
				&dce_st);
	if ( dce_st != error_status_ok)
		{
		gds_sec__inq_err_text(dce_st,"sec_login_set_extended_attrs",dce_e_string);
		GDS_SEC_TRACE1(2,"%s",dce_e_string);
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		return (D2_ERROR);
		}

	GDS_SEC_TRACE0(5, "sec_login_set_extended_attrs returned ok \n");

	/* set the extended login context to be the default one */
	sec_login_set_context(  new_login_ctx, &dce_st); 
	if ( dce_st != error_status_ok)
		{
		gds_sec__inq_err_text(dce_st,"sec_login_set_context",dce_e_string);
		GDS_SEC_TRACE1(2,"%s",dce_e_string);
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		return (D2_ERROR);
		}

	GDS_SEC_TRACE0(5, "New login context (master_bind) available\n");

	sec_cred_initialize_attr_cursor( &acursor, &dce_st);
	if ( dce_st != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		gds_sec__inq_err_text(dce_st,"sec_cred_initialize_attr_cursor",dce_e_string);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		GDS_SEC_TRACE1(2,"sec_cred_initialize_attr_curso: %s",dce_e_string);
		return (D2_ERROR);
		}

	GDS_SEC_TRACE0(3,"sec_cred_initialize_attr_cursor returns ok");

	/* get a handle to the callers priv attrs */
	pa_h = sec_login_cred_get_initiator ( new_login_ctx, &dce_st);
	if ( dce_st != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		gds_sec__inq_err_text(dce_st,"sec_login_cred_get_initiator",dce_e_string);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		GDS_SEC_TRACE1(2,"sec_login_cred_get_initiator: %s",dce_e_string);
		return (D2_ERROR);
		}
	GDS_SEC_TRACE0(3,"sec_login_cred_get_initiator returns ok III");


	/* get the callers extended attributes */

	while (dce_st == error_status_ok)
		{
		sec_cred_get_extended_attrs(pa_h, &acursor, &attr, &dce_st);
		if ( (dce_st != error_status_ok) && (dce_st != sec_cred_s_no_more_entries))
			{
			gds_sec__inq_err_text(dce_st,"sec_cred_get_extended_attrs",dce_e_string);
			GDS_SEC_TRACE1(2,"%s",dce_e_string);
			GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
			strcpy((char*)status->logstring, (char*)dce_e_string);
			d2_ret = D2_ERROR;
			}
		if (dce_st == error_status_ok) 
			{
			if (uuid_compare(&(attr.attr_id), &mb_attr_id, &dst) == 0 )
			/* got X500_DSA_Admin */
			{

			if ( gds_sec__eval_mb (&attr, target_dsa, &mb_poss, status))
				{
				d2_ret = D2_ERROR;
				}
			*masterb_poss =  mb_poss ;

			/* purge the new and install the old login context in
			case master_bind is not possible for the calling process */
			if ( *masterb_poss == FALSE) 
				{
				sec_login_purge_context(  &new_login_ctx, &dce_st);
				if ( dce_st != error_status_ok)
					{
					gds_sec__inq_err_text(dce_st,"sec_login_purge_context",dce_e_string);
					GDS_SEC_TRACE1(2,"%s",dce_e_string);
					GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
					strcpy((char*)status->logstring, (char*)dce_e_string);
					d2_ret = D2_ERROR;
					}

				/* set the old login context to be the default one */
				sec_login_set_context( login_ctx, &dce_st); 
				if ( dce_st != error_status_ok)
					{
					gds_sec__inq_err_text(dce_st,"sec_login_set_context",dce_e_string);
					GDS_SEC_TRACE1(2,"%s",dce_e_string);
					GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
					strcpy((char*)status->logstring, (char*)dce_e_string);
					return (D2_ERROR);
					}
				GDS_SEC_TRACE0(5, "The old login context again\n");
				} /* end reinstall old context */

			} /* end if attrtype = X500_DSA_Admin */

		} /* end if sec_cred_get_exteneded_attr returned OK */

	} /* end while attributes */

	return (d2_ret);
}


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__eval_mb                                  */
/*                                                                    */
/* Description	: Checks the extended attribute X55_DSA_Admin for the */
/*                coprrect encoding, i.e printstring_array.           */
/*                                                                    */
/*                Checks whether the own ddsa name is contained in    */
/*                list of names contained in the ERA.                 */
/*                                                                    */
/* Date         : June 16, 1994                                       */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

static signed32 gds_sec__eval_mb (
	/*	IN	*/	sec_attr_t		*attr ,
	/*	IN	*/	D2_name_string	dsa_name  ,
	/*	OUT	*/	Bool 			*mb ,
	/* OUT */   D2_sec_status   *status ) 

	{
	int i;
	unsigned char dsa_p_name[D2_DNL_MAX + 1];

	*mb = FALSE;

	GDS_SEC_TRACE1(2,"ENTRY gds_sec__eval_mb: target <%s>", dsa_name);
	if (gds_sec__iapl_2_print_name(	dsa_name , dsa_p_name))
		{
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_BAD_NAME);
		sprintf(status->logstring,
		"conversion of iapl dsa name <%s> to printable failed",  
		dsa_name);
		GDS_SEC_TRACE1(2,"%s",status->logstring);
		return (D2_ERROR);
		}

	GDS_SEC_TRACE1(2,"Printable  target <%s>", dsa_p_name);

	if (attr->attr_value.attr_encoding != sec_attr_enc_printstring_array)
		{
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_SEC_ERR,D2_S_DCE_DLG);
		sprintf(status->logstring,
		"wrong encoding for ERA X500_DSA_Admin <%d>, should be printstring_array",
		attr->attr_value.attr_encoding);
		return( D2_ERROR);
		}

	for (i = 0 ; 
		i<attr->attr_value.tagged_union.string_array->num_strings; 
		i++)
			{
			if (strcmp ( 
					(char *) dsa_p_name,
					(char *) attr->attr_value.tagged_union.string_array->strings[i]) == 0 )
					{
					*mb = TRUE ;
					return (D2_NOERROR);	
					}

			}
	GDS_SEC_TRACE0(2,"EXIT gds_sec__eval_mb  - target NOT FOUND");
	return (D2_NOERROR);	
	}




/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_sec__get_init_cred                            */
/*                                                                    */
/* Description	: performs steps necessary to login a DSA process     */
/*                takes care that ERA are contained in the login ctx  */
/*                                                                    */
/* Date         : July 16, 1994                                       */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 dsa_sec__get_init_cred  (
		/* I/O */  gss_cred_id_t 	*cred_h ,
		/* OUT */	D2_sec_status   *status)
{

	error_status_t dce_st;
	sec_login_handle_t  login_ctx, new_login_ctx;
	sec_attr_t x500_attrs[1];

	OM_uint32 minor_st;
	OM_uint32 ret;
	gss_cred_id_t cred_handle;

	unsigned char buffer[2024];
	unsigned char *dce_e_string;
	unsigned32 num_attrs = 1;


	GDS_SEC_TRACE0(4,"ENTRY dsa_sec__set_ext_attr");

	dce_e_string = buffer;

	cred_handle = *cred_h;

	x500_attrs[0].attr_id = dn_attr_id;
	x500_attrs[0].attr_value.attr_encoding = sec_attr_enc_printstring;
	x500_attrs[0].attr_value.tagged_union.printstring = NULL;

	num_attrs = 1;

	/* set X500_DN */
	GDS_SEC_TRACE0(2, "calling gssdce_cred_to_login_context ...");

	ret = gssdce_cred_to_login_context(	&minor_st, cred_handle,&login_ctx);
	if  (ret != GSS_S_COMPLETE )
		{
		gds_sec__map_gss_error( GSSDCE_C_OID_DCE_KRBV5_DES, ret, minor_st, status);
		GDS_SEC_TRACE2(2,"gssdce_cred_to_login_context failed:<%d>:%s",ret,status->logstring );
		return(D2_ERROR);
		}


	/* set X500_DN */
	GDS_SEC_TRACE0(2, "calling sec_login_set_extended_attrs ...");

	new_login_ctx = sec_login_set_extended_attrs(
				login_ctx,
				num_attrs,
				x500_attrs,
				&dce_st);
	if ( dce_st != error_status_ok)
		{
		GDS_SEC_SET_STATUS(status,D2_TEMP_ERR,D2_SEC_ERR,D2_S_DCE_ERA);
		gds_sec__inq_err_text(dce_st,"sec_login_set_extended_attrs",dce_e_string);
		strcpy((char*)status->logstring, (char*)dce_e_string);
		GDS_SEC_TRACE1(2,"sec_login_set_extended_attrs: %s",dce_e_string);
		return (D2_ERROR);
		}

	/* acquire gss credentials */

	GDS_SEC_TRACE0(2,"calling gssdce_login_context_to_cred ... ");

	ret = gssdce_login_context_to_cred(	&minor_st, new_login_ctx,
						GSS_C_INDEFINITE, GSS_C_NULL_OID_SET,
						&cred_handle, NULL, NULL );


	/* in case of failure set logging info and status */
	if  (ret != GSS_S_COMPLETE )
		{
		gds_sec__map_gss_error( GSSDCE_C_OID_DCE_KRBV5_DES, ret, minor_st, status);
		GDS_SEC_TRACE2(2,"gssdce_login_context_to_cred failed:<%d>:%s",ret,status->logstring );
		return(D2_ERROR);
		}


	*cred_h = cred_handle;

	GDS_SEC_TRACE0(4,"EXIT dsa_sec__set_ext_attr");
	return(D2_NOERROR);
}
