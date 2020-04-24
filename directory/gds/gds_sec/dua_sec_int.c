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
 * $Log: dua_sec_int.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:55:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:31  root]
 * 
 * Revision 1.1.2.7  1994/09/23  12:14:37  marrek
 * 	Enabled masterbind mechanism for DCE authentication.
 * 	[1994/09/23  12:13:54  marrek]
 * 
 * Revision 1.1.2.6  1994/07/06  15:07:21  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:15:40  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:46:54  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:54  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:45:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:01  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/12  14:49:11  zeliff
 * 	ot10623: prefix include of gssapi.h with dce
 * 	[1994/05/12  14:43:43  zeliff]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:03  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:14:51  keutel
 * 	creation
 * 	[1994/03/21  15:55:39  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: dua_sec_int.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:21 $";
#endif

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : dua_sec_int.c                                       */
/*                                                                    */
/* Description  : The module contains the definition of functions     */
/*                that are called from the security functions layer   */
/*                from the DUA library                                */
/*                                                                    */
/* Date         : December 21, 1993                                   */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gds.h>
#include <dce/d2dir.h>
#include <d21iapl.h>

#include <dce/gssapi.h>
/*#include <dce/gssapi-int.h>*/

#include <gds_sec.h>
#include <gds_sec_int.h>
#include <gds_sec_util.h>

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dua_sec__map_client_name                          */
/*                                                                    */
/* Description	: This function is used to map the DN of DUA onto     */
/*                the name meaningful for the GSSAPI implementation   */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 dua_sec__map_client_name(
	signed16	    sec_mech,	   /* IN security mechanism */	
	gss_OID 		gss_mech,      /* IN mech oid for gss_import_name */
	D2_name_string	client_dn,     /* IN DN of a the dir client  */	
	Bool     		*master_bind,  /* IN/OUT Flag indicating master_b */
	gss_name_t 	    *client_sec_name, /* OUT DSA name for GSS */
	D2_sec_status   *st)           /* OUT status and logging info */
	{

	GDS_SEC_TRACE0(3,"ENTRY dua_sec__map_client_name");
	/* in case of strong master_bind is not supported */
	if (sec_mech == D2_STRONG)
		{
		*master_bind = FALSE ;
		if (gds_sec__strong_map_name(gss_mech, client_dn,
					client_sec_name, st))
			{
			GDS_SEC_TRACE0(2,"gds_sec__strong_map_name failed");
			return(D2_ERROR);
			}
		}

	if (sec_mech == D2_DCE_AUTH)
		{
		/* use always the default context */
		*client_sec_name = GSS_C_NO_NAME;

		}
	GDS_SEC_TRACE_NAME(4,"dua_sec__map_client_name",*client_sec_name);
	GDS_SEC_TRACE0(3,"EXIT dua_sec__map_client_name");
	return(D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dua_sec__map_target_name                          */
/*                                                                    */
/* Description	: This function is used to map the DN or the address  */
/*                of a target DSA onto the name meaningful for the    */
/*                GSSAPI implementation                               */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 dua_sec__map_target_name(
	signed16	    sec_mech,	   /* IN security mechanism */	
	gss_OID 		gss_mech,      /* IN mech oid for gss_import */
	signed16		dir_id,        /* IN directory id. to which bind */	
								   /* is made. Relevant only if DN   */
								   /* of DSA is given to make bind to*/
	D2_name_string	dsa_dn,        /* IN DN of a DSA  */	
	unsigned16		dsa_psap_len,  /* Length of PSAP address of DSA */
	Psap_string     dsa_psap,      /* IN address of a DSA  */	
	gss_name_t 	    *dsa_sec_name, /* OUT DSA name for GSS */
	D2_sec_status   *st)           /* OUT status and logging info */
	{
	unsigned char  	dsa_pn[D2_DNL_MAX + 1];
	unsigned char *ppn;

	GDS_SEC_TRACE0(3,"ENTRY dua_sec__map_target_name");
	GDS_SEC_TRACE2(4,"dsa_dn:<%s> dsa_psap <%s>",dsa_dn, dsa_psap);

	if (sec_mech == D2_STRONG) 
		{
		if (dua_sec__psap_2_name( &dsa_dn, dsa_psap, st))
			return(D2_ERROR);

		/* simply convert the target name format  */ 
		if (gds_sec__strong_map_name(	gss_mech, dsa_dn,  
										dsa_sec_name, st))
			{
			GDS_SEC_TRACE0(2,"dua_sec__map_target_name: gds_sec__strong_map_name failed");
			return(D2_ERROR);
			}
		}

	if (sec_mech == D2_DCE_AUTH)
		{
		/* For the DCE mech this function has to perform the following 
		necessary name mappings 
		1) (D2_namestring) target_dsa_name || PSAP 				-->
					(D2_printable_str (RGY!)) target_dsa_name 
		2) (D2_printable_str (RGY!)) target_dsa_name	-->
					(principal_name_t) dsa_principal.
		3) (principal_name_t) dsa_principal  			-->
					(gss_buffer_t) dsa_principal
		4) (gss_buffer_t) dsa_principal  				-->
					(gss_name_t) principal		*/
		
		ppn =  dsa_pn ;
		memset ( (void *) ppn, 0x00 , (size_t) D2_DNL_MAX );

		/* get the principal name (+ DN if only the PSAP was supplied */
		/* from the directory */
		if ( dua_sec__get_dir_princ(dir_id, dsa_dn, dsa_psap_len, 
									dsa_psap, dsa_pn, st))
			{
			GDS_SEC_TRACE0(2,"gds_sec__get_dir_princ failed");
			return(D2_ERROR);
			}

		/* confirm the target name with the rgy */
		if ( gds_sec__conf_target(dsa_dn, dsa_pn, st))
			{
			GDS_SEC_TRACE0(2,"gds_sec__conf_target failed");
			return(D2_ERROR);
			}

		/* map and gss import principal name */
		if (gds_sec__dce_map_name(gss_mech, dsa_pn, dsa_sec_name, st))
			{
			GDS_SEC_TRACE0(2,"gds_sec__dce_map_name failed");
			return(D2_ERROR);
			}
		}

	GDS_SEC_TRACE_NAME(4,"dua_sec__map_target_name", *dsa_sec_name);
	GDS_SEC_TRACE0(3,"EXIT dua_sec__map_target_name");
	return(D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dua_sec__psap_2_name                              */
/*                                                                    */
/* Description	: checks whether one of PSAP or NAME is supplied.     */
/*                If it is the PSAP it retrieves the associated name  */
/*                and returns it .                                    */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : January , 19. 1994                                  */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
 signed32 dua_sec__psap_2_name(
	/*  IN/OUT*/D2_name_string  *target_name,
	/*  IN  */  Psap_string     target_psap,
	/*  OUT */  D2_sec_status   *status )
	{
	if ( *target_name != (D2_name_string) 0 )
		{
		/* Use that name */
		return (D2_NOERROR);
		}
	if ( target_psap == (Psap_string) 0 )
		{
		/* target is not specified */
		GDS_SEC_SET_STATUS(status,D2_CALL_ERR,D2_PAR_ERR,D2_S_BAD_NAME);
		return(D2_ERROR);
		}
	else /* only PSAP was supplied */
		{
		/* CHANGE: use ds_read/ds_search to retrieve the name */
		GDS_SEC_SET_STATUS(status,D2_CALL_ERR,D2_PAR_ERR,D2_S_NOT_SUPPORTED);
		return(D2_ERROR);
		}
	return (D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dua_sec__get_dir_princ                            */
/*                                                                    */
/* Description	: retrieves the DCE principal name - which is an      */
/*                attribute  of the DSA object - from the directory   */
/*                and returns it .                                    */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : January , 19. 1994                                  */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
 signed32 dua_sec__get_dir_princ(
	/*	IN	*/	signed16		dir_id,
	/*  IN	*/	D2_name_string  dsa_dn,
	/*	IN	*/	unsigned16		target_psap_len,
	/*  IN  */  Psap_string     target_psap,
	/*  OUT */  unsigned char   dsa_pn[],
	/*  OUT */  D2_sec_status   *status )
	{
	D2_pbhead   	head = { D21V022, 0, 0, 0 };

	D21_p1			bind_blk;
	D21_p2			unbind_blk;
	D21_p3			read_blk;
	D21_p5			search_blk;

	D2_c_res		c_res;
	D2_c_arg		c_arg;

	static	D2_a_type      	attr_type = { sizeof(D2_PR_NM)-1,
								(Ob_id_string) D2_PR_NM };     
	static 	D2_req_info    	req_info = {D2_R_VAL_TYP, 1, &attr_type};

	D2_a_value		psap_value;
	D2_a_value		dsa_oc_value;

	D2_Fi_filter	filter[3];


	GDS_SEC_TRACE0(3,"ENTRY dua_sec__get_dir_princ");

	/* set common arg. structure for read or serach.	*/
	c_arg.d2_extension            = (D2_extension *)NULL; 
	c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED; 
	c_arg.d2_op_progress.d2_next_rdn = 0; 
	c_arg.d2_al_rdn               = 0; 
	c_arg.d2_serv_cntrls          = D2_NOCACHE | D2_NOREFERRAL | 
	D2_USEDSA | D2_DREFALIAS | D2_DONT_STORE;     
	c_arg.d2_priority             = D2_PRIO_LOW; 
	c_arg.d2_time_limit           = D2_T_UNLIMITED; 
	c_arg.d2_size_limit           = D2_S_UNLIMITED; 
	c_arg.d2_scope_ref            = D2_WORLD_SCOPE;

	/* set bind block.	*/
	bind_blk.d21_1pbhead = &head;
	bind_blk.d21_1dir_id = dir_id;
	bind_blk.d21_1u_credent = (D2_credent *)NULL;
	bind_blk.d21_1name_dsa = (D2_name_string)NULL;
	bind_blk.d21_1addr_dsa = (D2_a_value *)NULL;
	bind_blk.d21_1auth_mech = D2_ANONYMOUS;

	/* DN of DSA specified, use it to map to the principal name.	*/
	if (dsa_dn != (D2_name_string)NULL)
		{    
		/* bind to the DSA.	*/
		bind_blk.d21_1name_dsa = dsa_dn;

		if (d21_1_bind(&bind_blk) == D2_ERROR)
			{
			/* set status structure.	*/

			GDS_SEC_TRACE1(2,"bind to <%s> failed",dsa_dn);
			GDS_SEC_TRACE0(2,"EXIT dua_sec__get_dir_princ");
			GDS_SEC_SET_STATUS(status,bind_blk.d21_1pbhead->d2_retcode,
							bind_blk.d21_1pbhead->d2_errclass,
							bind_blk.d21_1pbhead->d2_errvalue);

			return(D2_ERROR);
			}

		/* read the DSA object with principal name as the required	*/
		/* attribute.												*/
		head.d2_version = D21V022;
		head.d2_retcode = 0;
		head.d2_errclass = 0;
		head.d2_errvalue = 0;

		read_blk.d21_3pbhead = &head;

		read_blk.d21_3c_a = &c_arg;

		read_blk.d21_3bind_id = bind_blk.d21_1bind_id;
		read_blk.d21_3entry = dsa_dn;
		read_blk.d21_3r_info = &req_info;
		read_blk.d21_3err = (D2_error *)NULL;
		read_blk.d21_3cres = &c_res;

		if (d21_3_read(&read_blk) == D2_ERROR)
			{
			/* set status structure.	*/

			GDS_SEC_TRACE1(2,"read from <%s> failed",dsa_dn);
			GDS_SEC_TRACE0(2,"EXIT dua_sec__get_dir_princ");
			GDS_SEC_SET_STATUS(status,read_blk.d21_3pbhead->d2_retcode,
							read_blk.d21_3pbhead->d2_errclass,
							read_blk.d21_3pbhead->d2_errvalue);

			free((char *)(read_blk.d21_3err));

			return(D2_ERROR);
			}

		/* set unbind block.	*/
		head.d2_version = D21V022;
		head.d2_retcode = 0;
		head.d2_errclass = 0;
		head.d2_errvalue = 0;

		unbind_blk.d21_2pbhead = &head;
		unbind_blk.d21_2bind_id = bind_blk.d21_1bind_id;

		/* extract principal name from the result.	*/
		if (read_blk.d21_3res->d2_ei_ai->d2_a_val->d2_a_v_len > 
													D2_DNL_MAX + 1)
			{
			/* set status structure.	*/

			GDS_SEC_TRACE1(2,"PN too long <%d>",read_blk.d21_3res->d2_ei_ai->d2_a_val->d2_a_v_len);
			GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR,D2_S_INSUF_BUF);

			free((char *)(read_blk.d21_3res));

			/* unbind from the DSA.	*/
			d21_2_unbind(&unbind_blk);

			GDS_SEC_TRACE0(3,"EXIT dua_sec__get_dir_princ");

			return(D2_ERROR);
			}

		strncpy((char*) dsa_pn, 
		read_blk.d21_3res->d2_ei_ai->d2_a_val->d2_at_value.d2_prt_a_v,
		(size_t)read_blk.d21_3res->d2_ei_ai->d2_a_val->d2_a_v_len);

		GDS_SEC_TRACE2(2,"read from <%s> PN <%s>",dsa_dn,dsa_pn);
		GDS_SEC_TRACE1(2,"len read <%d>",read_blk.d21_3res->d2_ei_ai->d2_a_val->d2_a_v_len);

		free((char *)(read_blk.d21_3res));

		/* unbind from the DSA.	*/
		d21_2_unbind(&unbind_blk);

		GDS_SEC_TRACE0(3,"EXIT dua_sec__get_dir_princ");

		return(D2_NOERROR);
		}

	/* use PSAP to map to the principal name.	*/

	/* bind to the DSA.	*/
	psap_value.d2_a_v_len = target_psap_len;
	psap_value.d2_a_rep = D2_PSAP_STX;
	psap_value.d2_at_value.d2_psap_a_v = target_psap;

	bind_blk.d21_1addr_dsa = &psap_value;

	if (d21_1_bind(&bind_blk) == D2_ERROR)
		{
		/* set status structure.	*/

		GDS_SEC_TRACE0(2,"bind to PSAP failed");
		GDS_SEC_TRACE0(3,"EXIT dua_sec__get_dir_princ");
		GDS_SEC_SET_STATUS(status,bind_blk.d21_1pbhead->d2_retcode,
						bind_blk.d21_1pbhead->d2_errclass,
						bind_blk.d21_1pbhead->d2_errvalue);


		return(D2_ERROR);
		}

	/* search the DSA object with principal name as the 	*/
	/* required	attribute and PSAP as the filter.			*/

	head.d2_version = D21V022;
	head.d2_retcode = 0;
	head.d2_errclass = 0;
	head.d2_errvalue = 0;

	search_blk.d21_5pbhead = &head;
	search_blk.d21_5c_a = &c_arg;
	search_blk.d21_5bind_id = bind_blk.d21_1bind_id;

	/* root of DIT */
	search_blk.d21_5base_obj = (D2_name_string)NULL; 

	search_blk.d21_5subset = D2_WHOLE_SUBTREE;
	search_blk.d21_5srch_al = FALSE;

	filter[0].d2_fi_opmod = D2_AND;
	filter[0].d2_fi_v.fi_and_or.d2_fi_no_fi = 2;
	filter[0].d2_fi_v.fi_and_or.d2_fi_filter = &filter[1];

	filter[1].d2_fi_opmod = D2_ITEM;
	filter[1].d2_fi_v.d2_fi_item.d2_fi_match = D2_EQUAL;             
	filter[1].d2_fi_v.d2_fi_item.d2_fi_type.d2_typ_len = 
									sizeof(D2_PSAP_ADDRESS)-1; 
	filter[1].d2_fi_v.d2_fi_item.d2_fi_type.d2_type  = 
									(Ob_id_string) (D2_PSAP_ADDRESS); 
	filter[1].d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_att_val = psap_value;


	filter[2].d2_fi_opmod = D2_ITEM;
	filter[2].d2_fi_v.d2_fi_item.d2_fi_match = D2_EQUAL;             
	filter[2].d2_fi_v.d2_fi_item.d2_fi_type.d2_typ_len = 
									sizeof(D2_OBJ_CLASS)-1; 
	filter[2].d2_fi_v.d2_fi_item.d2_fi_type.d2_type  = 
									(Ob_id_string) (D2_OBJ_CLASS); 

	dsa_oc_value.d2_a_v_len = sizeof (D2_ODSA) - 1;
	dsa_oc_value.d2_a_rep = D2_OBJ_IDENT;
	dsa_oc_value.d2_at_value.d2_obid_a_v = (Ob_id_string)D2_ODSA;

	filter[2].d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_att_val = dsa_oc_value;

	search_blk.d21_5f_pattern = &filter[0];

	search_blk.d21_5r_info = &req_info;
	search_blk.d21_5file = (String)NULL;
	search_blk.d21_5err = (D2_error *)NULL;
	search_blk.d21_5cres = &c_res;

	if (d21_5_search(&search_blk) == D2_ERROR)
		{
		/* set status structure.	*/

		GDS_SEC_TRACE0(2,"search from PSAP failed");
		GDS_SEC_TRACE0(3,"EXIT dua_sec__get_dir_princ");
		GDS_SEC_SET_STATUS(status,search_blk.d21_5pbhead->d2_retcode,
						search_blk.d21_5pbhead->d2_errclass,
						search_blk.d21_5pbhead->d2_errvalue);

		free((char *)(search_blk.d21_5err));

		return(D2_ERROR);
		}

	/* set unbind block.	*/
	head.d2_version = D21V022;
	head.d2_retcode = 0;
	head.d2_errclass = 0;
	head.d2_errvalue = 0;

	unbind_blk.d21_2pbhead = &head;
	unbind_blk.d21_2bind_id = bind_blk.d21_1bind_id;

	/* extract principal name from the result.	*/
	if (search_blk.d21_5res->d2_ei_ai->d2_a_val->d2_a_v_len > 
												D2_DNL_MAX + 1)
		{
		/* set status structure.	*/
		GDS_SEC_TRACE1(2,"search result too long <%d>",search_blk.d21_5res->d2_ei_ai->d2_a_val->d2_a_v_len);
		GDS_SEC_SET_STATUS(status,D2_PERM_ERR,D2_INT_ERR,D2_S_INSUF_BUF);

		free((char *)(search_blk.d21_5res));

		/* unbind from the DSA.	*/
		d21_2_unbind(&unbind_blk);

		GDS_SEC_TRACE0(3,"EXIT dua_sec__get_dir_princ");

		return(D2_ERROR);
		}

	strncpy((char*) dsa_pn, 
	search_blk.d21_5res->d2_ei_ai->d2_a_val->d2_at_value.d2_prt_a_v,
	(size_t)search_blk.d21_5res->d2_ei_ai->d2_a_val->d2_a_v_len);

	GDS_SEC_TRACE1(2,"searched from PSAP PN <%s>",dsa_pn);

	free((char *)(search_blk.d21_5res));

	/* unbind from the DSA.	*/
	d21_2_unbind(&unbind_blk);

	GDS_SEC_TRACE0(3,"EXIT dua_sec__get_dir_princ");

	return(D2_NOERROR);
	}
