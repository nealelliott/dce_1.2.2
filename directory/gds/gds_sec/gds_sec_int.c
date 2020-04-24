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
 * $Log: gds_sec_int.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:36  root]
 * 
 * Revision 1.1.2.6  1994/07/06  15:07:24  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:15:53  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:46:56  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:05  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:45:16  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:03  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/12  14:49:14  zeliff
 * 	ot10623: prefix include of gssapi.h with dce
 * 	[1994/05/12  14:43:47  zeliff]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:18  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:15:54  keutel
 * 	creation
 * 	[1994/03/21  15:55:50  keutel]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: gds_sec_int.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:25 $";
#endif

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : gds_sec_intern.c                                    */
/*                                                                    */
/* Description  : The module contains the definition of internal      */
/*                functions of the security layer                     */
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
#include <d2dir.h>

#include <dce/gssapi.h>
/*#include <dce/gssapi-int.h>*/

#include <gds_sec.h>
#include <gds_sec_int.h>
#include <gds_sec_util.h>


#define VALUE_SCAN	(signed16) 1
#define OID_SCAN	(signed16) 2

/* this table is used to map oids that occur in iapl DNs to 
   abbreviations used in the printable form of DNs */  
   
#define	MAX_OIDS_IN_TABLE		16
static struct	{	char *oid ;
					char *abb ;
				} 	oid_table[MAX_OIDS_IN_TABLE]  =
			  	{
				"85.4.3" , "CN" ,
			   	"85.4.4" , "SN" ,
			   	"85.4.5" , "SER" ,
			   	"85.4.6" , "C" ,
			   	"85.4.7" , "L" ,
			   	"85.4.9" , "STA" ,
			   	"85.4.10" , "O" ,
			   	"85.4.11" , "OU" ,
			   	"85.4.12" , "TIT" ,
			   	"85.4.16" , "PA" ,
			   	"85.4.17" , "PC" ,
			   	"85.4.18" , "POB" ,
			   	"85.4.19" , "PDO"
			   	"85.4.29" , "PSA"
			   	"85.4.34" , "SEA"
			   	"85.4.35" , "UP"
				};


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_constr_log_string                             */
/*                                                                    */
/* Description	: This function is used to fill the logstring struct  */
/*                element of the D2_sec_status structure that is re-  */
/*                turned by each  function of the security layer.     */
/*                It uses the gss_display_status call           .     */
/*                                                                    */
/* Date         : December 21, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

void gds_constr_log_string( 
	/*  IN  */  gss_OID			mech_type,
	/* IN */  	int				status_value,
	/* OUT */ 	D2_sec_status	*status )
	{
	OM_uint32 ret, minor_st;
	int m_context = 0 ;
	gss_buffer_desc status_string;
	char *tmp;

	tmp = status->logstring;

	do
		{
		ret = gss_display_status( &minor_st, status_value, GSS_C_MECH_CODE,
				mech_type, &m_context, &status_string);
		if ( ret != GSS_S_COMPLETE )
			{
			sprintf(status->logstring, 
				"GSSAPI unable to display minor status hex %08X\n", 
				 status_value); 
			return;
			}
		/* copy buffer into logstring */
		strncpy(tmp, status_string.value,status_string.length);
		tmp+=status_string.length;

		} while (m_context != 0 );

	/* copy EOS to the logstring  */
	*tmp = 0x00;
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__mech_2_oid                               */
/*                                                                    */
/* Description	: This function is used to map the signed16 auth.mech */
/*                value used in the gds to the OID used in GSSAPI     */
/*                The function uses hardcoded OID values for the supp.*/
/*                mechanisms. See defines in gds_sec_int.h            */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : December 27, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

 signed32  gds_sec__mech_2_oid(
	/*  IN  */  signed16			desired_sec_mech,               
	/* OUT  */  gss_OID_desc		*gss_mech,                      
	/*  OUT */  D2_sec_status		*status )
	{

	signed32 ret = D2_NOERROR;

	switch (desired_sec_mech)
		{
		case D2_ANONYMOUS:
			break;
		case D2_SIMPLE:
			break;
		case D2_DCE_AUTH:
			gss_mech->length = GSSDCE_C_OID_DCE_KRBV5_DES->length;
			/* gss_mech->length = strlen(GSSDCE_C_OID_DCE_KRBV5_DES); */
			/*
			GDS_SEC_MALLOC(gss_mech->elements, void*,strlen(GSSDCE_C_OID_DCE_KRBV5_DES),status);
			memcpy (gss_mech->elements, (void*) GSSDCE_C_OID_DCE_KRBV5_DES, strlen(DCE_OID));
			*/
			GDS_SEC_MALLOC(gss_mech->elements, void*,gss_mech->length,status);
			memcpy (gss_mech->elements, 
					(void*) GSSDCE_C_OID_DCE_KRBV5_DES->elements,
					gss_mech->length);
			break;
		case D2_STRONG:
			gss_mech->length = strlen(STRONG_OID);
			GDS_SEC_MALLOC(gss_mech->elements, void*,strlen(STRONG_OID),status);
			memcpy (gss_mech->elements, (void*) STRONG_OID, strlen(STRONG_OID));
			break;
		default:
			ret = D2_ERROR;
			GDS_SEC_SET_STATUS (status,D2_CALL_ERR,D2_PAR_ERR,D2_S_INAPPR_AUTH);
			sprintf(status->logstring,"desired_sec_mech: %d not supported",
					desired_sec_mech);
		}
	return (ret);
	}                      


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__init_status                              */
/*                                                                    */
/* Description	: self explaining function name                       */
/*                                                                    */
/* Date         : December 27, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

void  gds_sec__init_status(
   /* IN/ OUT */  D2_sec_status       *status )
	{

	/* CHANGE : This is NOT threadsafe */

	static char st_logstring[GDS_SEC_MAX_LOGSTRING] = "";

	status->logstring = st_logstring; 

	status->d2_retcode = D2_NO_ERR ;
	status->d2_errclass = D2_NOT_SPEC ;
	status->d2_errvalue = D2_S_NOT_SPEC ;

	return;
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__map_gss_error                            */
/*                                                                    */
/* Description	: self explaining function name                       */
/*                                                                    */
/* Date         : December 27, 1993                                   */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

void  gds_sec__map_gss_error (
	/*  IN  */		gss_OID			mech_type,
	/*  IN  */		OM_uint32		major_st,
	/*  IN  */		OM_uint32		minor_st,
	/* IN/OUT */	D2_sec_status	*status )
	{

	switch (major_st)
		{
		case GSS_S_COMPLETE:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_ILL_STAT;
			status->d2_errvalue = D2_S_FAILURE;
			break;
		case GSS_S_CONTINUE_NEEDED:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_ILL_STAT;
			status->d2_errvalue = D2_S_FAILURE;
			break;
		case GSS_S_DEFECTIVE_TOKEN:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_SEC_ERR;
			status->d2_errvalue = D2_S_SEC_VIOLATION;
			break;
		case GSS_S_DEFECTIVE_CREDENTIAL:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_SEC_ERR;
			status->d2_errvalue = D2_S_SEC_VIOLATION;
			break;
		case GSS_S_NO_CRED:    
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_PAR_ERR;
			status->d2_errvalue = D2_S_FAILURE;
			break;
		case GSS_S_CREDENTIALS_EXPIRED:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_SEC_ERR;
			status->d2_errvalue = D2_S_EXPIRED_CRED;
			break;
		case GSS_S_BAD_BINDINGS:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_PAR_ERR;
			status->d2_errvalue = D2_S_FAILURE;
			break;
		case GSS_S_BAD_SIG:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_SEC_ERR;
			status->d2_errvalue = D2_S_SEC_VIOLATION;
			break;
		case GSS_S_OLD_TOKEN:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_ILL_STAT;
			status->d2_errvalue = D2_S_FAILURE;
			break;
		case GSS_S_DUPLICATE_TOKEN:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_ILL_STAT;
			status->d2_errvalue = D2_S_FAILURE;
			break;
		case GSS_S_CONTEXT_EXPIRED:
			status->d2_retcode = D2_PERM_ERR;
			status->d2_errclass = D2_SEC_ERR;
			status->d2_errvalue = D2_S_EXPIRED_CRED;
			break;
		case GSS_S_NO_CONTEXT:
			status->d2_retcode = D2_CALL_ERR;
			status->d2_errclass = D2_ILL_STAT;
			status->d2_errvalue = D2_S_FAILURE;
			break;
		case GSS_S_BAD_NAMETYPE:
			status->d2_retcode = D2_PERM_ERR;
			status->d2_errclass = D2_SEC_ERR;
			status->d2_errvalue = D2_S_BAD_NAME;
			break;
		case GSS_S_BAD_NAME:
			status->d2_retcode = D2_PERM_ERR;
			status->d2_errclass = D2_SEC_ERR;
			status->d2_errvalue = D2_S_BAD_NAME;
			break;
		case GSS_S_FAILURE:
			status->d2_retcode = D2_PERM_ERR;
			status->d2_errclass = D2_SEC_ERR;
			status->d2_errvalue = D2_S_FAILURE;
			break;
		default:
			status->d2_retcode = D2_TEMP_ERR;
			status->d2_errclass = D2_INT_ERR;
			status->d2_errvalue = D2_S_UNKNOWN;
			sprintf(status->logstring,"GSSAPI returns unknown \
								  major code %d\n", major_st);
			return;
		}
	gds_constr_log_string(mech_type, minor_st,status);
	return ;
	}

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__dce_map_name                             */
/*                                                                    */
/* Description	: maps a D2_name_string type pricipal name onto       */
/*                a gss_name_t Only for DCE Auth mechanism            */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : January  14,  1994                                  */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__dce_map_name(
	gss_OID			gss_mech ,		/* IN security mechanism */	
	D2_name_string 	princ_name ,	/* IN gds internal name format*/	
	gss_name_t 	    *sec_name,		/* OUT gssapi format name */
	D2_sec_status   *st)			/* OUT status and logging info */

	{
	OM_uint32 ret , minor_st;
	gss_buffer_desc x500_princ_name;
	/*
	gss_OID_desc oid_principal_name;
	*/
	gss_name_t tmp;

	GDS_SEC_TRACE1(4,"ENTRY gds_sec__dce_map_name gds_princ_name <%s>",princ_name);

	/* fill gss_buffer_t name struct */
	x500_princ_name.length = (size_t) strlen ((char*)princ_name);
	x500_princ_name.value = (void*) princ_name;

	GDS_SEC_TRACE_TOKEN(5,"gss_buffer_t X500_princ_name",&x500_princ_name);

	/* setup a gss_oid for the external name id */

	/* use the constant provided by GSSAPI

	oid_principal_name.length = strlen(GSSDCE_C_OID_DCENAME);
	GDS_SEC_MALLOC(oid_principal_name.elements, void*,strlen(X500NAME_OID),st);
	memcpy (oid_principal_name.elements, (void*) GSSDCE_C_OID_DCENAME,
									strlen(GSSDCE_C_OID_DCENAME));

	GDS_SEC_TRACE_TOKEN(5,"gss_oid_t oid_principal_name",&oid_principal_name);
	*/

	GDS_SEC_TRACE_TOKEN(5,"gss_oid_t oid_principal_name",GSSDCE_C_OID_DCENAME);

	/* ask for a gss_name_t struct */

	GDS_SEC_TRACE0(6,"calling gss_import_name "); 
	ret = gss_import_name(&minor_st, &x500_princ_name,
								GSSDCE_C_OID_DCENAME, &tmp);

	/* 							&oid_principal_name, &tmp); */
	/* free the x500name_oid */

	/* no more necessary

	free (oid_principal_name.elements);
	*/

	if ( ret != GSS_S_COMPLETE)
		{
		gds_sec__map_gss_error( gss_mech, ret, minor_st, st);
		return(D2_ERROR);
		}
	*sec_name = tmp;

	GDS_SEC_TRACE_NAME(4,"EXIT gds_sec__dce_map_name", *sec_name);
	return(D2_NOERROR);
	}
/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__strong_map_name                          */
/*                                                                    */
/* Description	: maps a D2_name_string type name onto gss_name_t     */
/*                Only for Strong Authentication mechanism            */
/*                                                                    */
/* Return Value	: D2_NOERROR or D2_ERROR                              */
/* Date         : January  14,  1994                                  */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed32 gds_sec__strong_map_name(
	gss_OID			gss_mech ,		/* IN security mechanism */	
	D2_name_string 	gds_name ,		/* IN gds internal name format*/	
	gss_name_t 	    *sec_name,		/* OUT gssapi format name */
	D2_sec_status   *st)			/* OUT status and logging info */

	{
	OM_uint32 ret , minor_st;
	gss_buffer_desc my_x500name;
	gss_OID_desc oid_x500name;
	gss_name_t tmp;

	GDS_SEC_TRACE1(4,"ENTRY gds_sec__strong_map_name gds_name <%s>",gds_name);

	/* fill gss_buffer_t name struct */
	my_x500name.length = (size_t) strlen ((char*)gds_name);
	my_x500name.value = (void*) gds_name;

	GDS_SEC_TRACE_TOKEN(5,"gss_buffer_t my_x500name",&my_x500name);

	/* setup a gss_oid for the external name id */
	oid_x500name.length = strlen(X500NAME_OID);
	GDS_SEC_MALLOC(oid_x500name.elements, void*,strlen(X500NAME_OID),st);
	memcpy (oid_x500name.elements, (void*) X500NAME_OID, strlen(X500NAME_OID));

	GDS_SEC_TRACE_TOKEN(5,"gss_oid_t oid_x500name",&oid_x500name);

	/* ask for a gss_name_t struct */
	ret = gss_import_name(&minor_st, &my_x500name, &oid_x500name,
						&tmp);
	/* free the x500name_oid */
	free (oid_x500name.elements);

	if ( ret != GSS_S_COMPLETE)
		{
		gds_sec__map_gss_error( gss_mech, ret, minor_st, st);
		return(D2_ERROR);
		}
	*sec_name = tmp;

	GDS_SEC_TRACE_NAME(4,"EXIT gds_sec__strong_map_name", *sec_name);
	return(D2_NOERROR);
	}

/*--------------------------------------------------------------------*/
/* Function Name:	gds_sec__buf_2_str                                */
/*                                                                    */
/* Description	: maps a gss_buffer_t onto D2_name_str                */
/* Date         : January  14,  1994                                  */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
void gds_sec__buf_2_str(buffer_name,name)
	/* IN	*/	gss_buffer_t buffer_name;
	/* OUT	*/	D2_name_string *name;
	{
	unsigned char *tmp;

	tmp = *name,

	memcpy(*name,buffer_name->value,buffer_name->length);
	tmp +=(buffer_name->length) ;
	tmp++;
	*tmp = 0x00;
	}

/*--------------------------------------------------------------------*/
/* Function Name:	gds_sec__iapl_2_print_name                        */
/*                                                                    */
/* Description	: converts a Name in aipl format to a more readable   */
/*                format "RDN=value/RDN=value,RDN=value/.... "        */
/* Date         : March    14,  1994                                  */
/* Author       : Sanjay Jain         SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed32	gds_sec__iapl_2_print_name(
	/*	IN	*/	D2_name_string iapl_name,
	/*	OUT	*/	unsigned char tdt_name[])

{
signed32	index;
signed32	val_index, oid_index;
signed16 	what;

unsigned char iapl_oid[100];
unsigned char p_oid[10];

what	=	OID_SCAN;
oid_index = 0;
val_index = 1;

tdt_name[0] = TDT_RDN_SEP;

for (index=0; iapl_name[index] != D2_EOS; index++)
	{
	switch (iapl_name[index])
		{
		case D2_T61_REP:
		case D2_PRTBL_REP:
		case D2_NUM_REP:
		case D2_IA5_REP:
			what = VALUE_SCAN;
			iapl_oid[oid_index] = D2_EOS ;
			if (gds_sec__oid_2_prtoid(iapl_oid,p_oid))
				{
				return(D2_ERROR);
				}
			strcpy((char*)  &tdt_name[val_index],(char*) p_oid);
			val_index+=strlen((char*) p_oid);
			tdt_name[val_index] = TDT_TYPE_VAL_SEP;
			val_index++;
			break;
		case D2_AVA_SEP:
			what = OID_SCAN;
			oid_index = 0;
			tdt_name[val_index] = TDT_AVA_SEP;
			val_index++;
			break;
		case D2_RDN_SEP:
			what = OID_SCAN;
			oid_index = 0;
			tdt_name[val_index] = TDT_RDN_SEP;
			val_index++;
			break;
		default:
			if  (what == VALUE_SCAN)
				{
				tdt_name[val_index] = iapl_name[index];
				val_index++;
				}
			if  (what == OID_SCAN)
				{
				iapl_oid[oid_index] = iapl_name[index];
				oid_index++;
				}
			break;
		}
	}

tdt_name[val_index] = D2_EOS;
return (D2_NOERROR);
}

/*--------------------------------------------------------------------*/
/* Function Name:	gds_sec__print_2_iapl_name                        */
/*                                                                    */
/* Description	: converts a Name                                     */
/*                format "RDN=value/RDN=value,RDN=value/.... "        */
/*                to an iapl formatted name                           */
/* Date         : March    14,  1994                                  */
/* Author       : Sanjay Jain         SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed32	gds_sec__print_2_iapl_name(
	/*	IN	*/	unsigned char print_name[],
	/*	OUT	*/	unsigned char iapl_name[])	

{
signed16 what;
signed32	index;
signed32	oid_index , val_index;
unsigned char iapl_oid[100];
unsigned char p_oid[10];

what    =   OID_SCAN;
oid_index = 0;
val_index = 0;


/* start with 1 due to printable name always begin with /  */

for (index=1; print_name[index] != D2_EOS; index++)
	{
	switch (print_name[index])
		{
		case TDT_TYPE_VAL_SEP:
			what = VALUE_SCAN;
			p_oid[oid_index] = D2_EOS ;
			if (gds_sec__prtoid_2_oid(p_oid,iapl_oid))
				{
				return(D2_ERROR);
				}
			strcpy((char*) &iapl_name[val_index],(char*) iapl_oid);
			val_index+=strlen((char*) iapl_oid);
			iapl_name[val_index] = D2_PRTBL_REP;
			val_index++;
			break;
		case TDT_AVA_SEP:
			what = OID_SCAN;
			oid_index = 0 ;
			iapl_name[val_index] = D2_AVA_SEP;
			val_index++;
			break;
		case TDT_RDN_SEP:
			what = OID_SCAN;
			oid_index = 0 ;
			iapl_name[val_index] = D2_RDN_SEP;
			val_index++;
			break;
		default:
			if ( what == VALUE_SCAN )
				{
				iapl_name[val_index] = print_name[index];
				val_index++;
				}
			if  (what == OID_SCAN)
				{
				p_oid[oid_index] =  print_name[index];
				oid_index++;
				}
			break;
		}
	}

iapl_name[val_index] = D2_EOS;
return (D2_NOERROR);
}

/*--------------------------------------------------------------------*/
/* Function Name:	gds_sec__oid_2_prtoid                             */
/*                                                                    */
/* Description	: converts a OID to its printable Abbreviation        */
/*                THIS FUNCTION USES A HARDCODED STATIC TABLE. It is  */
/*                highly recommended to introduce a general purpose   */
/*                Schema description file that can be used by the DUA */
/* Date         : March    14,  1994                                  */
/* Author       : Sanjay Jain         SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed32  gds_sec__oid_2_prtoid(
/*  IN  */  unsigned char oid[],
/*  OUT */  unsigned char prt_oid[])
	{
	signed32 i;

	strcpy ((char*) prt_oid, "");

	for ( i=0; i<MAX_OIDS_IN_TABLE; i++)
		{
		if (strcmp ((char*) oid, oid_table[i].oid ) == 0 )
			{
			strcpy((char*) prt_oid,  oid_table[i].abb);
			return(D2_NOERROR);
			}
		}
	return (D2_ERROR);
	}

/*--------------------------------------------------------------------*/
/* Function Name:	gds_sec__prtoid_2_oid                             */
/*                                                                    */
/* Description	: converts a printable Abbreviation of a DN name part */
/*                to the OID string value                             */
/*                THIS FUNCTION USES A HARDCODED STATIC TABLE. It is  */
/*                highly recommended to introduce a general purpose   */
/*                Schema description file that can be used by the DUA */
/* Date         : March    14,  1994                                  */
/* Author       : Sanjay Jain         SNI BA NM 123, Munich, Germany  */
/*--------------------------------------------------------------------*/
signed32  gds_sec__prtoid_2_oid(
/*  IN  */  unsigned char prt_oid[],
/*  OUT */  unsigned char oid[])
	{
	signed32 i;

	strcpy ((char*) oid, "");

	for ( i=0; i<MAX_OIDS_IN_TABLE; i++)
		{
		if (strcmp ((char*) prt_oid, oid_table[i].abb ) == 0 )
			{
			strcpy((char*) oid,  oid_table[i].oid);
			return(D2_NOERROR);
			}
		}
	return (D2_ERROR);
	}


/*--------------------------------------------------------------------*/
/*                                                                    */
/* Function Name:	gds_sec__printtoken                               */
/* Description	:   test function                                     */
/*--------------------------------------------------------------------*/

void  gds_sec__printtoken(
    /* IN */ D2_str  *t)
	{
	int i;
	unsigned char *p;

	p = (t->d2_value);
	
	for ( i = 1 ; i<= t->d2_size; i++)
		{
		if ( ((i-1) % 20 ) == 0 ) fprintf(stderr, "   ");
		fprintf(stderr,"%02X ", *p);
		p++;
		if ( (i % 4) == 0 ) fprintf(stderr, " ");
		if (((i % 20) == 0) && (i != t->d2_size)) fprintf(stderr,"\n");
		}
	fprintf(stderr, "\n");
	}


