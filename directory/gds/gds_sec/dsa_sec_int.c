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
 * $Log: dsa_sec_int.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:55:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:27  root]
 * 
 * Revision 1.1.2.6  1994/09/23  12:14:34  marrek
 * 	Enabled masterbind mechanism for DCE authentication.
 * 	[1994/09/23  12:13:42  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:46:51  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:43  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:45:11  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:57  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/12  14:49:09  zeliff
 * 	ot10623: prefix include of gssapi.h with dce
 * 	[1994/05/12  14:43:39  zeliff]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:35  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:34:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:14:48  keutel
 * 	creation
 * 	[1994/03/21  15:55:31  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: dsa_sec_int.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:18 $";
#endif

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : dsa_sec_int.c                                       */
/*                                                                    */
/* Description  : The module contains the definition of functions     */
/*                that are called from the security functions layer   */
/*                fro the DSA                                         */
/*                                                                    */
/* Date         : December 21, 1993                                   */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#include <gds.h>
#include <dce/d2dir.h>
#include <d26dsa.h>

#include <dce/gssapi.h>
/*#include <dce/gssapi-int.h>*/

#include <gds_sec.h>
#include <gds_sec_int.h>
#include <gds_sec_util.h>

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_sec__map_client_name                          */
/*                                                                    */
/* Description	: This function is used to map the own DN of a DSA    */
/*                onto the name meaningful for the GSSAPI implemen-   */
/*                tation                                              */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 dsa_sec__map_client_name(
	signed16	    sec_mech,	   /* IN security mechanism */	
	gss_OID 		gss_mech,      /* IN mech oid for gss_import_name */
	D2_name_string	dsa_dn,     	/* IN DN of a the dir client  */	
	gss_name_t 	    *dsa_sec_name, /* OUT DSA name for GSS */
	D2_sec_status   *st)           /* OUT status and logging info */
	{

	GDS_SEC_TRACE1(3,"ENTRY dsa_sec__map_client_name dsa_dn <%s>",dsa_dn);
	if (sec_mech == D2_STRONG)
		{
		if (gds_sec__strong_map_name(gss_mech, dsa_dn,
					dsa_sec_name, st))
			{
			GDS_SEC_TRACE0(2,"gds_sec__strong_map_name failed");
			return(D2_ERROR);
			}
		}

	if (sec_mech == D2_DCE_AUTH)
		{
		/* provide a dce principal name useful for gss_acquire_cred */
		/* use the global variable of the DSA d26_my_pr_name */
		if (gds_sec__dce_map_name(gss_mech, (D2_name_string) d26_mypr_name,
					dsa_sec_name, st))
			{
			GDS_SEC_TRACE0(2,"gds_sec__dce_map_name failed");
			return(D2_ERROR);
			}
		}
	GDS_SEC_TRACE_NAME(4,"dsa_sec__map_client_name",*dsa_sec_name);
	GDS_SEC_TRACE0(3,"EXIT dsa_sec__map_client_name");
	return(D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_sec__map_target_name                          */
/*                                                                    */
/* Description	: This function is used to map the DN of a target DSA */
/*                onto the name meaningful for the GSSAPI implemen-   */
/*                tation                                              */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dsa_sec__map_target_name(
	signed16	    sec_mech,	   /* IN security mechanism */	
	gss_OID 		gss_mech,      /* IN mech oid for gss_import_name */
	D2_name_string	dsa_dn,        /* IN DN of a DSA  */	
	Psap_string     dsa_psap,      /* IN address of a DSA  */	
	gss_name_t 	    *dsa_sec_name, /* OUT DSA name for GSS */
	D2_sec_status   *st)           /* OUT status and logging info */
	{
	Bool bad_name = FALSE;

	GDS_SEC_TRACE0(3,"ENTRY dsa_sec__map_target_name");
	GDS_SEC_TRACE2(4,"dsa_dn:<%s> dsa_psap <%s>",dsa_dn, dsa_psap);

	if (sec_mech == D2_STRONG) 
		{
		if (dua_sec__psap_2_name( &dsa_dn, dsa_psap, st))
			return(D2_ERROR);

		/* simply convert the target name format  */ 
		if (gds_sec__strong_map_name(	gss_mech, dsa_dn,  
										dsa_sec_name, st))
			{
			GDS_SEC_TRACE0(2,"dsa_sec__map_target_name: gds_sec__strong_map_name failed");
			return(D2_ERROR);
			}
		}

	if (sec_mech == D2_DCE_AUTH)
		{
		/* The DSA will supply the DCE pricipal name of the target DSA 
		in the dsa_dn input parameter */
		/* Check whether name is given , forget PSAP */

		if (dsa_dn == (D2_name_string) 0 )
			bad_name = TRUE;	
		else
			{
			if (strlen ((char *) dsa_dn) == 0 )
				bad_name = TRUE;	
			}
		if (bad_name == TRUE)
			{
			GDS_SEC_SET_STATUS(st,D2_CALL_ERR,D2_PAR_ERR,D2_S_BAD_NAME);
			return(D2_ERROR);
			}

		/* Following mapping steps remain to be performed here */
		/*
		1) (D2_name_string) target_dsa_principal_name	-->
					(gss_buffer_t) dsa_principal
		2) (gss_buffer_t) dsa_principal  				-->
					(gss_name_t) principal		*/
		
		if (gds_sec__dce_map_name(gss_mech, dsa_dn,               
                                dsa_sec_name, st))
			{
			GDS_SEC_TRACE0(2,"gds_sec__dce_map_name failed");
			return(D2_ERROR);
			}
		*dsa_sec_name = (gss_name_t) dsa_dn ;
		}

	GDS_SEC_TRACE0(3,"EXIT dsa_sec__map_target_name");
	return(D2_NOERROR);
	}
	
/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	dsa_sec__get_client_name                          */
/*                                                                    */
/* Description	: This function is used to read the atributes X.500DN */
/*                and X.500_DSA_Admin from the sec ctx. In case of    */
/*                STRONG it is a call to gss_display_name. For DCE    */
/*                gssdce and sec_cred calls are used.                 */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/
signed32 dsa_sec__get_client_name(
	signed16			auth_method,	/* IN security mechanism */
	gss_OID 			gss_mech,		/* IN  gss mech type     */
	gss_ctx_id_t		ctx_h,			/* IN handle to sec ctx */
	gss_name_t			dua_gss_name, 	/* IN clients gss_name */	
	D2_name_string		*dua_name,	 	/* OUT clients DN */	
	Bool				*master_bind,	/* OUT flag */  
	D2_sec_status		*st)			/* OUT status and loginfo */

	{
	OM_uint32 ret, minor_st;
	gss_buffer_desc dua_buffer_name;
	gss_OID  name_oid;
	signed32 d2_ret ;

	extern byte d26_myname[];

	GDS_SEC_TRACE0(3,"ENTRY dsa_sec__get_client_name");
	GDS_SEC_TRACE_NAME(3,"dsa_sec__get_client_name", dua_gss_name)

	*master_bind = FALSE;

    if ( auth_method == D2_DCE_AUTH )
		{
		/* retrieve extended attributes from the security context */
		d2_ret = dsa_sec__get_ext_attr( ctx_h, 
									(D2_name_string) d26_myname,
									dua_name, 
									master_bind, 
									st);
		if ( d2_ret == D2_ERROR )
			{
			GDS_SEC_TRACE0(2,"dsa_sec__get_ext_attr failed");
			/* Fallback: Try to get the dn_name from the rgy directly */
			if ( dsa_sec__get_dn_from_rgy( dua_gss_name ,
											(D2_name_string) d26_myname,
											dua_name,
											master_bind,
											st))
				{ 
				GDS_SEC_TRACE0(2,"dsa_sec__get_dn_from_rgy failed");
				return (D2_ERROR );
				}
			}
		}

	if ( auth_method == D2_STRONG )
		{
		ret = gss_display_name(	&minor_st, 
								dua_gss_name, 
								&dua_buffer_name, 
								&name_oid);
		if ( ret != GSS_S_COMPLETE)
			{
			gds_sec__map_gss_error( gss_mech, ret, minor_st, st);
			return(D2_ERROR);
			}
		GDS_SEC_TRACE_TOKEN(4,"buffer_name from gss_display_name",&dua_buffer_name);

		/* CHANGE 
		
		if (gds_sec__oid_compare( name_oid, X500NAME_OID )
			{  
			displayed name is useless 
			GDS_SET_STATUS(st,XXX);
			}
		*/
		gds_sec__buf_2_str(&dua_buffer_name,dua_name);

		} /* end strong authentication */

	GDS_SEC_TRACE0(3,"EXIT dsa_sec__get_client_name");
	return(D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/* Function Name:	dsa__tdt_read_perm                                */
/* Description	: The function tests whether permission chars (r/m)   */
/*                are present in the record supplied by the caller    */
/*                                                                    */
/* Return Value	: trust_level                                         */
/* Date         : january  4 , 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed16 dsa__tdt_read_perm (
	char *record,
	int keypos,
	int FieldSep )
	{
	int i;
	int tl = TDT_NOT_TRUSTED;

	for (i=0;i<keypos;i++) 
		{
		record=(char*)strchr(record,FieldSep);
		if (!record) return(tl);
		record++;
		}
	while ((*record!='\0')&&(*record!=FieldSep))
		{ 
		if ( tolower( (int) *record) == 'r' )
				tl |= TDT_TRUSTED_FOR_READ;
		if ( tolower( (int) *record) == 'm' )
				tl |= TDT_TRUSTED_FOR_MOD;
		record++;
		}
	return(tl);
	}


/*--------------------------------------------------------------------*/
/* Function Name:	dsa__try_tdt_mask                                 */
/* Description	: The function tests whether the string keyfield is   */
/*                contained as keypos'th field in a record            */
/*                                                                    */
/* Return Value	: short 0 or 1                                        */
/* Date         : january  4 , 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed16 dsa__try_tdt_mask(
	char *record,
	int keypos,
	int FieldSep,
	char *keyfield)
	{
	int i;

	for (i=0;i<keypos;i++) 
		{
		record=(char*)strchr(record,FieldSep);
		if (!record) break;
		record++;
		}
	while ((*record!='\0')&&(*record!=FieldSep)&&
			(*keyfield!='\0')&&(*record==*keyfield)) 
		{
		record++;
		keyfield++;
		}
	/* return 0 if not present , 1 if matched */
	return((*keyfield=='\0')&&((*record=='\0')||(*record==FieldSep)));
	}

/*--------------------------------------------------------------------*/
/* Function Name:	dsa__close_tdt                                    */
/* Description	: closes the file refered to by handle                */
/* Date         : january  4 , 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
void dsa__close_tdt(
		/* in  */	chandle_t	handle)
{
	if (handle!=NULL) close(handle->File);
	free(handle);
}

/*--------------------------------------------------------------------*/
/* Function Name:	dsa__open_tdt                                     */
/* Description	: The function opnes a file. structure of records and */
/*                fields are specified via fieldsep and recordsep     */
/*                                                                    */
/* Return Value	: CACHE_OK or errorvalue CACHE_????                   */
/* Date         : january  4 , 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed32 dsa__open_tdt(
		/* in  */	char *name,
		/* in  */	int fieldsep,
		/* in  */	int recordsep,
		/* out */	chandle_t *handle)
{
	*handle=(chandle_t)malloc(sizeof(chandle_t_s));
	if (!*handle) return(CACHE_NOMEM);
	(*handle)->File=open(name,O_RDONLY);
	if ((*handle)->File==-1) {
		free(*handle);
		return(CACHE_NOPERM);
	}
	(*handle)->FieldSep=fieldsep;
	(*handle)->RecordSep=recordsep;
	(*handle)->seek=0;
	return(CACHE_OK);
}

/*--------------------------------------------------------------------*/
/* Function Name:	dsa__read_tdt_record                              */
/* Description	: The function eads one record  from the file spec.   */
/*                by handle. If readmode is set to CACHE_KEY keypos   */
/*                has to contain the number of the keyfield ( 0 for   */
/*                the first), keyfield the string that is looked for, */
/*                                                                    */
/* Return Value	: CACHE_OK or errorvalue CACHE_????                   */
/* Date         : january  4 , 1994                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed32 dsa__read_tdt_record (
		/* in  */	chandle_t handle,
		/* in  */	int readmode,
		/* in  */	int keypos,
		/* in  */	char *keyfield,
		/* out */	char *record)
{
	int	help , temp;
	int	n;
	char *nh;
	if (!handle) return(CACHE_NULL);
	switch (readmode) {
		case CACHE_FIRST:
				if (-1==lseek(handle->File,0,SEEK_SET))
					return(CACHE_NOOPEN);
				if (1>(n=read(handle->File,record,TDT_MAXBUF-1)))
					return(CACHE_NOREAD);
				record[n+1]='\0';
				nh=(char*)strchr(record,(char)handle->RecordSep);
				if (nh) *nh='\0';
				handle->seek=strlen(record)+1;
			break;
		case CACHE_NEXT:
				if (-1==handle->seek)
					return(CACHE_NOOPEN);
				if (-1==lseek(handle->File,handle->seek,SEEK_SET))
					return(CACHE_NOOPEN);
				if (1>(n=read(handle->File,record,TDT_MAXBUF-1))) {
					return(CACHE_NOREAD);
				}
				record[n+1]='\0';
				nh=(char*)strchr(record,(char)handle->RecordSep);
				if (nh) *nh='\0';
				handle->seek+=strlen(record)+1;
			break;
		case CACHE_KEY:
				help=CACHE_FIRST;
				while (dsa__read_tdt_record(handle,help,0,"",record)== CACHE_OK) {
					temp = (int) dsa__try_tdt_mask(record,keypos,
								handle->FieldSep,keyfield) ;
					if ( temp != 0 )
						return(CACHE_OK);
					help=CACHE_NEXT;
				}
				return(CACHE_NOFOUND);
		default:
			return(CACHE_NOREAD);
	}
	return(CACHE_OK);
}

/* testfunction */

void print_cache_handle(
	/* in */ chandle_t handle)
	{
	fprintf(stderr, "\thandle %08X\n", handle);
	fprintf(stderr, "\t\t FILE %d\n", (handle)->File);
	fprintf(stderr, "\t\t seek %d\n", (handle)->seek);
	fprintf(stderr, "\t\t Fieldsep <%c>\n", (handle)->FieldSep);
	fprintf(stderr, "\t\t Recordsep <%c>\n", (handle)->RecordSep);
	fflush(stderr);
	}
/*EOF*/
