/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2amhs1.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:14  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:25:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:54  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:38  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:28:30  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:40  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:17:38  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:38:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:47:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:10:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:39:40  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  18:13:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:11:22  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/04  18:02:46  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	Replace T_NO_NSAP by T_MAXNSAP
 * 	[1992/12/04  10:43:54  marrek]
 * 
 * Revision 1.1.4.3  1992/11/27  16:03:38  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:55:06  marrek]
 * 
 * Revision 1.1.4.2  1992/09/23  11:16:09  marrek
 * 	Replace some blanks.
 * 	[1992/09/23  11:14:59  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:38:45  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:48:29  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:34:18  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d2amhs1.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:18 $";
#endif


/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*daton	***************************************************************/
/*								      */
/* TYP		: C-FILE					      */
/*								      */
/* NAME         : d2amhs1.c                                           */
/*								      */
/* AUTHOR       : Jochen Keutel , SNI                                 */
/* DATE         :   6. 2. 1991                                        */
/*								      */
/* DESCRIPTION  : This file includes functions for transforming APDU  */
/*                messages to IDS interface for MHS attributes.       */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL **********/

static signed32 d2a_151_st_oun_fr_ipc (O_U_Names ** ,string ,signed32 ,byte * ,signed32 *);
static void d2a_152_ex_oun_fr_ipc ( E_Attrs * , string , signed32 );
static signed32 d2a_153_dt_fr_ipc (D_D_Attrs ** ,string ,signed32 ,byte * ,signed32 * );
static void d2a_154_ex_dom_fr_ipc ( E_Attrs * , string , signed32 , signed32 );
static signed32 d2a_155_pds_fr_ipc (E_Attrs * ,D21_OR_address_part * ,
							signed16 ,signed32 );

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a137_oradr_fr_ipc()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION: Transformation of an APDU message to the OR-   */
/*                     Address-Part of the IDS interface              */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      scan		= pointer to the APDU message                 */
/*	or_address	= pointer to the ORAddress Part               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      or_address	= pointer to the filled ORAddress Part        */ 
/*	err_apdu	= pointer 		 		      */
/*	err_len		= pointer to error length 		      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                                                                    */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Jochen Keutel                                             */
/**********************************************************************/


signed32 d2a137_oradr_fr_ipc (
  signed16 offset,
  byte * scan,
  ORAddress * or_address,
  byte * err_apdu,
  signed32 * err_len )
{
const char function_name[] = "d2a137_oradr_fr_ipc";
#define mode(x) (((x) == D2_NUMERIC) ? NUMERIC : PRINTABLE) 
#define	DT	1			/* Domain Type in extensions	*/
#define	DV	2			/* Domain Value in extensions	*/

byte		* tag_ptr;		/* pointer to tag		*/
D21_OR_address_part * addr_part;	/* pointer to addr_part	*/
D21_str_list	* str_list;		/* pointer to  str_list	*/
string		block;		/* pointer to the text */
string		pp;			/*  pointer used in P_A_L	*/

unsigned16		addr_tag;
signed16	addr_off,addr_len,le,str_off;
unsigned16		str_rep ;  
signed32		nn,i;
signed32		sel_syntax,ii;		/* for Psap address	*/
signed32		s_surname=0 , e_surname=0;      /* for P_Name   */
signed32		e_on=0;				/* for Org Name	*/
signed32		s_o1=0 , s_o2=0 , s_o3=0;	/* for Org Unit Names */
signed32		e_o1=0 , e_o2=0 , e_o3=0;
signed32		d_t1=0 , d_t2=0 , d_t3=0 , d_t4=0;  /* for Domain Types	*/
signed32		e_t1=0 , e_t2=0 , e_t3=0 , e_t4=0;
signed32		d_v1=0 , d_v2=0 , d_v3=0 , d_v4=0; /* for Domain Values	*/
signed32		e_v1=0 , e_v2=0 , e_v3=0 , e_v4=0;
signed32		p_a_f=0 ;				/* for P_A_F	*/
signed32		e_pres = 0;		/* for Presentation Address	*/
signed32		e_num = 0;			/* for ISDN Number	*/
E_Attrs		ee_attrs;	/* local for extended attributes	*/

/*	Initialisizing	*/

ee_attrs.n = 0;

or_address->s_attrs.c_name = or_address->s_attrs.a_d_name = 
or_address->s_attrs.p_d_name = (str_NP_choice *)NULL;

or_address->s_attrs.n_addr = or_address->s_attrs.t_id = 
or_address->s_attrs.org_name = or_address->s_attrs.u_id = (string)NULL;

or_address->s_attrs.p_name = (P_Name *)NULL;

or_address->s_attrs.o_u_names = (O_U_Names *)NULL;

or_address->d_d_attrs = (D_D_Attrs *)NULL;

or_address->e_attrs = (E_Attrs *)NULL;

/*	Begin			*/

tag_ptr = scan;

while (offset > 0) {				/* there is another	*/
						/* ORAddress Part	*/

addr_part =(D21_OR_address_part *) tag_ptr;
addr_tag = addr_part->d21_ora_tag;
addr_off = addr_part->d21_ora_off;
addr_len = addr_part->d21_ora_len;

block = (string) (addr_part + 1);

switch (addr_tag) {

case D21_ORA_CTRY_N:	 	/*	Country Name 			*/
				/*	S1	(N or P)		*/

str_list = (D21_str_list *) (addr_part + 1);
str_rep = str_list->d21_strl_rep;
str_off = str_list->d21_strl_off;

block = (string) (str_list + 1);

if ( sizeof(D21_str_list) + str_off < addr_off )  
   {			/*      there is a second part	*/
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }

if ( str_rep != D2_NUMERIC && str_rep != D2_PRINTABLE )
   {			/* neither N nor P	*/
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }

if ((or_address->s_attrs.c_name = D27_MALLOC(svc_c_sev_warning,sizeof(str_NP_choice))) == NULL)
   {
   d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }
or_address->s_attrs.c_name->mode = mode(str_rep);
or_address->s_attrs.c_name->str_val = block;
break;

case D21_ORA_ADMD_N:		/*	Administration Name 		*/
				/*	S2	(N or P)		*/

str_list = (D21_str_list *) (addr_part + 1);
str_rep = str_list->d21_strl_rep;
str_off = str_list->d21_strl_off;

block = (string) (str_list + 1);

if ( sizeof(D21_str_list) + str_off < addr_off )  
   {
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }

if ( str_rep != D2_NUMERIC && str_rep != D2_PRINTABLE )
   {
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }

if ((or_address->s_attrs.a_d_name = D27_MALLOC(svc_c_sev_warning,sizeof(str_NP_choice))) == NULL)
   {
   d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }
or_address->s_attrs.a_d_name->mode = mode(str_rep);
or_address->s_attrs.a_d_name->str_val = block;
break;

case D21_ORA_X121_A:		/*	X121 Address			*/
				/*	S3	(N)			*/

or_address->s_attrs.n_addr = block;
break;

case D21_ORA_T_ID:		/*	Terminal Identifier		*/
				/*	S4	(P)			*/

or_address->s_attrs.t_id = block;
break;

case D21_ORA_PRMD_N:		/*	PRMD Name 			*/
				/*	S5	(N or P)		*/

str_list = (D21_str_list *) (addr_part + 1);
str_rep = str_list->d21_strl_rep;
str_off = str_list->d21_strl_off;

block = (string) (str_list + 1);

if ( sizeof(D21_str_list) + str_off < addr_off )  
   {
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }

if ( str_rep != D2_NUMERIC && str_rep != D2_PRINTABLE )
   {
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }

if ((or_address->s_attrs.p_d_name = D27_MALLOC(svc_c_sev_warning,sizeof(str_NP_choice))) == NULL)
   {
   d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }
or_address->s_attrs.p_d_name->mode = mode(str_rep);
or_address->s_attrs.p_d_name->str_val = block;
break;

case D21_ORA_COM_N:		/*	Common Name 			*/
				/*	E1 and E2   (P or T or both)	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if ( str_rep == D2_PRINTABLE )
      {
      ee_attrs.v[ee_attrs.n].e_type = common_name;
      ee_attrs.v[ee_attrs.n].e_value.P_str = block;
      ee_attrs.n++;
      }

   else if ( str_rep == D2_T61 )
      {
      ee_attrs.v[ee_attrs.n].e_type = teletex_common_name;
      ee_attrs.v[ee_attrs.n].e_value.T_str = block;
      ee_attrs.n++;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_ORG_N:		/*  Organization Name 			*/
				/*	S6 and E3    (P or T or both)	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S6	(P)	*/
      or_address->s_attrs.org_name = block;

   else if (str_rep == D2_T61) 				/* E3	(T)	*/
      {
      ee_attrs.v[ee_attrs.n].e_type = teletex_organization_name;
      ee_attrs.v[ee_attrs.n].e_value.T_str = block;
      e_on = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }

if ( e_on == 1 ) ee_attrs.n++;
break;

case D21_ORA_NUM_ID:		/*	Numeric User Identifier		*/
				/*	S7	(Numeric)		*/

or_address->s_attrs.u_id = block;
break;

case D21_ORA_SURNAME:		/*	Surname 			*/
				/*	S8 and E4   (P or T or both)	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S8	(P)	*/
      {
      if ((or_address->s_attrs.p_name = D27_MALLOC(svc_c_sev_warning,sizeof(P_Name))) == NULL)
         {
         d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			           D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
         D2_ASN1_ERR_RETURN_WITH_TRACE
         }

      or_address->s_attrs.p_name->surname = block;
      or_address->s_attrs.p_name->given_name = NULL;
      or_address->s_attrs.p_name->initials = NULL;
      or_address->s_attrs.p_name->g_qualifier = NULL;
      s_surname = 1;
      }
   else if (str_rep == D2_T61) 				/* E4	(T)	*/
      {
      ee_attrs.v[ee_attrs.n].e_type = teletex_personal_name;
      ee_attrs.v[ee_attrs.n].e_value.pn_val.surname = block;
      ee_attrs.v[ee_attrs.n].e_value.pn_val.given_name =  (string)NULL;
      ee_attrs.v[ee_attrs.n].e_value.pn_val.initials = (string)NULL;
      ee_attrs.v[ee_attrs.n].e_value.pn_val.g_qualifier = (string)NULL;
      ee_attrs.n++;
      e_surname = 1;
      }
   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break; 

case D21_ORA_GIV_N:		/*	Given Name 			*/
				/*	S8 and E4   (P or T or both)	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S8	(P)	*/
      {
      if ( s_surname == 0 ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      or_address->s_attrs.p_name->given_name = block;
      }

   else if (str_rep == D2_T61 ) 			/* E4	(T)	*/
      {
      if ( e_surname == 0 ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      ee_attrs.v[ee_attrs.n-1].e_value.pn_val.given_name = block;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_INIT:		/*	Initials 			*/
				/*	S8 and E4   (P or T or both)	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S8	(P)	*/
      {
      if ( s_surname == 0 ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      or_address->s_attrs.p_name->initials = block;
      }

   else if (str_rep == D2_T61 ) 			/* E4	(T)	*/
      {
      if ( e_surname == 0 ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      ee_attrs.v[ee_attrs.n-1].e_value.pn_val.initials = block;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_GEN:		/*	Generation			*/
				/*	S8 and E4   (P or T or both)	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S8	(P)	*/
      {
      if ( s_surname == 0 ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      or_address->s_attrs.p_name->g_qualifier = block;
      }

   else if (str_rep == D2_T61 ) 			/* E4	(T)	*/
      {
      if ( e_surname == 0 ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      ee_attrs.v[ee_attrs.n-1].e_value.pn_val.g_qualifier = block;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

				/* 	Organizational Unit Name (1-4)	*/
				/*		(P or T or both)	*/

case D21_ORA_ORGU_N1:		/*	1:	S9 and E5		*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S9	(P)	*/
      {
      if (d2a_151_st_oun_fr_ipc(&or_address->s_attrs.o_u_names,block,1,err_apdu,err_len) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      s_o1 = 1;
      }

   else if (str_rep == D2_T61) 				/* E5	(T)	*/
      {
      d2a_152_ex_oun_fr_ipc(&ee_attrs,block,1);
      e_o1 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_ORGU_N2:		/*	2:	S9 and E5		*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S9	(P)	*/
      {
      if (s_o1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d2a_151_st_oun_fr_ipc(&or_address->s_attrs.o_u_names,block,2,err_apdu,err_len) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      s_o2 = 1;
      }

   else if (str_rep == D2_T61) 				/* E5	(T)	*/
      {
      if (e_o1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_152_ex_oun_fr_ipc(&ee_attrs,block,2);
      e_o2 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_ORGU_N3:		/*	3:	S9 and E5		*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S9	(P)	*/
      {
      if (s_o1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (s_o2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d2a_151_st_oun_fr_ipc(&or_address->s_attrs.o_u_names,block,3,err_apdu,err_len) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      s_o3 = 1;
      }

   else if (str_rep == D2_T61) 				/* E5	(T)	*/
      {
      if (e_o1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_o2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_152_ex_oun_fr_ipc(&ee_attrs,block,3);
      e_o3 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_ORGU_N4:		/*	4:	S9 and E5		*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* S9	(P)	*/
      {
      if (s_o1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (s_o2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (s_o3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d2a_151_st_oun_fr_ipc(&or_address->s_attrs.o_u_names,block,4,err_apdu,err_len) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      }

   else if (str_rep == D2_T61) 				/* E5	(T)	*/
      {
      if (e_o1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_o2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_o3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_152_ex_oun_fr_ipc(&ee_attrs,block,4);
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

				/*	Domain Type / Value		*/
				/*		(1-4)			*/
				/*	(P or T or both)		*/

case D21_ORA_DOM_T1:		/*	Type	1:	DT and E6	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE) 			/* DT1	(P)	*/
      {
      if ( d2a_153_dt_fr_ipc(&or_address->d_d_attrs,block,1,err_apdu,err_len ) != D2_NOERROR ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d_t1 = 1;
      }

   else if (str_rep == D2_T61) 				/* E6 (T1) (T)	*/
      {
      d2a_154_ex_dom_fr_ipc(&ee_attrs,block,1,DT);
      e_t1 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_DOM_V1:		/*	Value	1:	DV and E6	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE) 			/* DV1	(P)	*/
      {
      if (d_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      or_address->d_d_attrs->v[0].value = block;
      d_v1 = 1;
      }

   else if (str_rep == D2_T61) 				/* E6 (V1) (T)	*/
      {
      if (e_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_154_ex_dom_fr_ipc(&ee_attrs,block,1,DV);
      e_v1 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_DOM_T2:		/*	Type	2:	DT and E6	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE) 			/* DT2	(P)	*/
      {
      if (d_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if ( d2a_153_dt_fr_ipc(&or_address->d_d_attrs,block,2,err_apdu,err_len ) != D2_NOERROR ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d_t2 = 1;
      }

   else if (str_rep == D2_T61) 				/* E6 (T2) (T)	*/
      {
      if (e_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_154_ex_dom_fr_ipc(&ee_attrs,block,2,DT);
      e_t2 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_DOM_V2:		/*	Value	2:	DV and E6	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE) 			/* DV2	(P)	*/
      {
      if (d_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      or_address->d_d_attrs->v[1].value = block;
      d_v2 = 1;
      }

   else if (str_rep == D2_T61) 				/* E6 (V2) (T)	*/
      {
      if (e_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_154_ex_dom_fr_ipc(&ee_attrs,block,2,DV);
      e_v2 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_DOM_T3:		/*	Type	3:	DT and E6	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE) 			/* DT3	(P)	*/
      {
      if (d_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if ( d2a_153_dt_fr_ipc(&or_address->d_d_attrs,block,3,err_apdu,err_len ) != D2_NOERROR ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d_t3 = 1;
      }

   else if (str_rep == D2_T61) 				/* E6 (T3) (T)	*/
      {
      if (e_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_154_ex_dom_fr_ipc(&ee_attrs,block,3,DT);
      e_t3 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_DOM_V3:		/*	Value	3:	DV and E6	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE) 			/* DV3	(P)	*/
      {
      if (d_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      or_address->d_d_attrs->v[2].value = block;
      d_v3 = 1;
      }

   else if (str_rep == D2_T61) 				/* E6 (V3) (T)	*/
      {
      if (e_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_154_ex_dom_fr_ipc(&ee_attrs,block,3,DV);
      e_v3 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_DOM_T4:		/*	Type	4:	DT and E6	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE) 			/* DT4	(P)	*/
      {
      if (d_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if ( d2a_153_dt_fr_ipc(&or_address->d_d_attrs,block,4,err_apdu,err_len ) != D2_NOERROR ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d_t4 = 1;
      }

   else if (str_rep == D2_T61) 				/* E6 (T4) (T)	*/
      {
      if (e_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_154_ex_dom_fr_ipc(&ee_attrs,block,4,DT);
      e_t4 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_DOM_V4:		/*	Value	4:	DV and E6	*/

str_list = (D21_str_list *) (addr_part + 1);
str_off = str_list->d21_strl_off;

while ( str_off + sizeof(D21_str_list) <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE) 			/* DV4	(P)	*/
      {
      if (d_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_v3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (d_t4 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      or_address->d_d_attrs->v[3].value = block;
      d_v4 = 1;
      }

   else if (str_rep == D2_T61) 				/* E6 (V4) (T)	*/
      {
      if (e_t1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v1 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v2 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_v3 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      if (e_t4 == 0) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      d2a_154_ex_dom_fr_ipc(&ee_attrs,block,4,DV);
      e_v4 = 1;
      }

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
break;

case D21_ORA_P_D_S_N:		/*	Postal Delivery System Name	*/
				/*	E7	(P)			*/

ee_attrs.v[ee_attrs.n].e_type = pds_name;
ee_attrs.v[ee_attrs.n].e_value.P_str = block;
ee_attrs.n++;
break;

case D21_ORA_P_CTRY:		/*	Postal Country Name		*/
				/*	E8	(N or P)		*/

str_list = (D21_str_list *) (addr_part + 1);
str_rep = str_list->d21_strl_rep;
str_off = str_list->d21_strl_off;

block = (string) (str_list + 1);

if ( sizeof(D21_str_list) + str_off < addr_off )  
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

if ( str_rep != D2_NUMERIC && str_rep != D2_PRINTABLE )
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

ee_attrs.v[ee_attrs.n].e_type = physical_delivery_country_name;
ee_attrs.v[ee_attrs.n].e_value.NP_val.mode = mode(str_rep);
ee_attrs.v[ee_attrs.n].e_value.NP_val.str_val = block;
ee_attrs.n++;
break;

case D21_ORA_P_CODE:		/*	Postal Code			*/
				/*	E9	(N or P)		*/

str_list = (D21_str_list *) (addr_part + 1);
str_rep = str_list->d21_strl_rep;
str_off = str_list->d21_strl_off;

block = (string) (str_list + 1);

if ( sizeof(D21_str_list) + str_off < addr_off )  
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

if ( str_rep != D2_NUMERIC && str_rep != D2_PRINTABLE )
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

ee_attrs.v[ee_attrs.n].e_type = postal_code;
ee_attrs.v[ee_attrs.n].e_value.NP_val.mode = mode(str_rep);
ee_attrs.v[ee_attrs.n].e_value.NP_val.str_val = block;
ee_attrs.n++;
break;

case D21_ORA_P_O_N:		/*	Postal Office Name		*/
				/*	E10	(P or T or both)	*/

i = physical_delivery_office_name;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_O_NUM:		/*	Postal Office Number		*/
				/*	E11	(P or T or both)	*/

i = physical_delivery_office_number;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_A_D:		/*	Postal Address Details		*/
				/*	E12	(P or T or both)	*/

i = extension_OR_address_components;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_P_N:		/*	Postal Patron Name		*/
				/*	E13	(P or T or both)	*/

i = physical_delivery_personal_name;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_ORG_N:		/*	Postal Organization Name	*/
				/*	E14	(P or T or both)	*/

i = physical_delivery_organization_name;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_P_D:		/*	Postal Patron Details		*/
				/*	E15	(P or T or both)	*/

i = extension_physical_delivery_address_components;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_A_F:		/*	Postal Address in Full		*/
				/*	E16 (T)				*/

ee_attrs.v[ee_attrs.n].e_type = unformatted_postal_address;
ee_attrs.v[ee_attrs.n].e_value.up_addr.teletex_string = block;
ee_attrs.v[ee_attrs.n].e_value.up_addr.p_address = (PostalAddress_Lines *)NULL;

p_a_f = 1;
ee_attrs.n++;
break;

case D21_ORA_P_A_L:		/*	Postal Address in Full (Lines)	*/
				/*	E16 (P)				*/

if (p_a_f == 0)
   {
   ee_attrs.n++;
   ee_attrs.v[ee_attrs.n-1].e_value.up_addr.teletex_string = (string)NULL;
   ee_attrs.v[ee_attrs.n-1].e_type = unformatted_postal_address;
   }

if ((ee_attrs.v[ee_attrs.n-1].e_value.up_addr.p_address =
			D27_MALLOC(svc_c_sev_warning,sizeof(PostalAddress_Lines))) == NULL)
   {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
   }

for ( nn=0,pp=block,le=0 ; le < addr_len ; nn++ )
  {
  if (nn >= UB_PDS_ADDR_LINES) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
  ee_attrs.v[ee_attrs.n-1].e_value.up_addr.p_address->v[nn] = pp;
  le += strlen(pp) + 1;
  pp += strlen(pp) + 1;
  }

ee_attrs.v[ee_attrs.n-1].e_value.up_addr.p_address->n = nn;
break;

case D21_ORA_P_S_A:		/*	Postal Street Address		*/
				/*	E17 (P or T or both)		*/

i = street_address;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_O_B_N:		/*	Postal Office Box Number	*/
				/*	E18 (P or T or both)		*/

i = post_office_box_address;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_G_D_A:		/*	Postal General Delivery Address	*/
				/*	E19 (P or T or both)		*/

i = poste_restante_address;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_D_P_N:		/*	Postal Delivery Point Name	*/
				/*	E20 (P or T or both)		*/

i = unique_postal_name;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_P_LOC:		/*	Postal Locale			*/
				/*	E21 (P or T or both)		*/

i = local_postal_attributes;
if (d2a_155_pds_fr_ipc(&ee_attrs,addr_part,addr_off,i) != D2_NOERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
break;

case D21_ORA_PRES_A:		/*	Presentation Address		*/
				/*	E22 (Object)			*/

e_pres=1;
ee_attrs.v[ee_attrs.n].e_type = extended_network_address;
ee_attrs.v[ee_attrs.n].e_value.en_addr.typ = PSAP_ADDRESS_TYPE;

if ((ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address =
				D27_MALLOC(svc_c_sev_warning,sizeof(dua_psap))) == NULL)
   {
   d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }
sel_syntax = SEL_SETSYNTAX(SEL_BINARY, SEL_BINARY, SEL_BINARY);
if ((ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->nsap =
		       D27_MALLOC (svc_c_sev_warning,T_MAXNSAP * sizeof(octet_string))) == NULL)
   {
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }
ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->psel.contents 
	= ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->ssel.contents 
	= ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->tsel.contents 
	= (unsigned char *) NULL ;
for (ii=0; ii< T_MAXNSAP; ii++)
   ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->nsap[ii].contents = (unsigned char *)  NULL;

if(d2p_decode_psap_addr(
   (Psap_info *)(&ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->psel),
   (Psap_info *)(&ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->ssel),
   (Psap_info *)(&ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->tsel),
   &sel_syntax,
  (signed32 *)(&ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->nb_nsap),
   (Psap_info *)(ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->nsap),
   (byte *)block)
	 < 0)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->psel_defined =
(ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->psel.contents == NULL)?
FALSE:TRUE;
ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->ssel_defined =
(ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->ssel.contents == NULL)?
FALSE:TRUE;
ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->tsel_defined =
(ee_attrs.v[ee_attrs.n].e_value.en_addr.v.psap_address->tsel.contents == NULL)?
FALSE:TRUE;

ee_attrs.n++;
break;

case D21_ORA_ISDN_N:		/*	ISDN Number			*/
				/*	E22 (N)				*/

if (e_pres == 1) /* both is not allowed		*/
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
e_num = 1;
ee_attrs.v[ee_attrs.n].e_type = extended_network_address;

if ((ee_attrs.v[ee_attrs.n].e_value.en_addr.v.e163_4_address =
				D27_MALLOC(svc_c_sev_warning,sizeof(E163_4_address))) == NULL)
   {
   d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }
ee_attrs.v[ee_attrs.n].e_value.en_addr.typ = E163_ADDRESS_TYPE;
ee_attrs.v[ee_attrs.n].e_value.en_addr.v.e163_4_address->number = block;
ee_attrs.v[ee_attrs.n].e_value.en_addr.v.e163_4_address->sub_address = (string)NULL;
ee_attrs.n++;
break;

case D21_ORA_ISDN_S:		/*	ISDN Subaddress			*/
				/*	E22 (N)				*/

if (e_pres == 1) 	/* both is not allowed		*/
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
if (e_num == 0) 		/* no ISDN Number	*/
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

ee_attrs.v[ee_attrs.n-1].e_value.en_addr.v.e163_4_address->sub_address = block;
break;

case D21_ORA_T_T:		/*	Terminal Type			*/
				/*	E23 (Enum)			*/

ee_attrs.v[ee_attrs.n].e_type = terminal_type;
ee_attrs.v[ee_attrs.n].e_value.ter_val = *( (signed32 *)block);
ee_attrs.n++;
break;

default:

D2_ASN1_ERR_RETURN_WITH_TRACE

}						/* End of switch	*/

tag_ptr	+= (addr_off + sizeof(D21_OR_address_part));
offset	-= (addr_off + sizeof(D21_OR_address_part));

}						/* End of while		*/

/* Assigning of extended attributes	*/

if (ee_attrs.n > 0)
   {
   if ((or_address->e_attrs = D27_MALLOC(svc_c_sev_warning,sizeof(E_Attrs))) == NULL)
      {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

   * or_address->e_attrs = ee_attrs;
   }


if ((d_t1 == 1) && (d_v1 == 0)) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
if ((d_t2 == 1) && (d_v2 == 0)) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
if ((d_t3 == 1) && (d_v3 == 0)) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
if ((d_t4 == 1) && (d_v4 == 0)) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

return(D2_NOERROR);
}				/* End of  d2_a_137_oradr_fr_ipc	*/

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a138_orname_fr_ipc()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION: Transformation of an APDU message to the OR-   */
/*                     Name-Part of the IDS interface                 */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      scan	= pointer to the APDU message                         */
/*   or_name	= pointer to  ORName Part                             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*   or_name	= pointer to filled ORName Part                       */
/*	err_apdu	= error pointer 			      */
/*	err_len		= pointer to error length 		      */
/*                                                                    */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Jochen Keutel                                             */
/**********************************************************************/

signed32 d2a138_orname_fr_ipc (
  signed16 offset,
  byte	* scan,
  ORName * or_name,
  byte * err_apdu,
  signed32 * err_len )
{
const char function_name[] = "d2a138_orname_fr_ipc";
byte	     *	block ;
D21_OR_name  *	name_part ;

signed16		off , name_off ;
signed32		n;
signed32		n_dir , n_adr ;

/*	Initialization	*/

or_name->d_name.name = (dua_dist_name *)NULL;

/*	Begin	*/

name_part = (D21_OR_name *) scan;
name_off = name_part->d21_orn_off;
off = name_part->d21_orn_off;

while (off + sizeof(D21_OR_name) <= offset)
   {
   block = (byte *) ( name_part + 1);

   switch (name_part->d21_orn_tag)
      {

      case D21_ORN_DN :				/* Directory Name	*/

      if ((or_name->d_name.name = D27_MALLOC(svc_c_sev_warning,sizeof(dua_dist_name))) == NULL)
      {
         d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			        D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
         D2_ASN1_ERR_RETURN_WITH_TRACE
      }
      if ( d2a_016_str_to_dn ((char *)block,or_name->d_name.name,err_apdu,
			err_len) != D2_NOERROR )
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

      n_dir = 1;
      break;

      case D21_ORN_ORA :			/* ORAddress		*/

      if ((n = d2a137_oradr_fr_ipc (name_off,block, &or_name->address,err_apdu,err_len)) != D2_NOERROR)
         {
	 D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
      n_adr = 1;
      break;

      default:
      D2_ASN1_ERR_RETURN_WITH_TRACE

      }
      name_part = (D21_OR_name *) ( block + name_off);
      name_off = name_part->d21_orn_off;
      off += name_part->d21_orn_off + sizeof(D21_OR_name);

   }

/*	Error:	Directory Name without Address	*/

if ((n_adr == 0) && (n_dir == 1))
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}


return (D2_NOERROR);	/*	end of d2a_138_orname_fr_ipc		*/
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a136_dlsp_fr_ipc                                      */
/*                                                                    */
/*  SHORT DESCRIPTION: Transformation of an APDU message to the DL-   */
/*                     Submit-Permission-Part of the IDS interface    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      scan	= pointer to the APDU message                         */
/*   dls_perm	= pointer to DLSubmitPermission Part                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*   dls_perm	= pointer to filled DLSubmitPermission Part           */
/*	err_apdu	= error pointer				      */
/*	err_len		= pointer to error length		      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Jochen Keutel                                             */
/**********************************************************************/

signed32 d2a136_dlsp_fr_ipc (
  signed16 offset,
  byte	* scan,
  DLSubmitPermission * dls_perm,
  byte * err_apdu,
  signed32 * err_len )
{
const char function_name[] = "d2a136_dlsp_fr_ipc";
byte		     *	block ;
D21_DL_sub_perm      *	name_part ;
signed16 name_off;

signed32			n ;

/*		Begin			*/

name_part = (D21_DL_sub_perm *) scan;
dls_perm->typ = name_part->d21_dlsp_tag;
name_off = name_part->d21_dlsp_off;

block = (byte *) ( name_part + 1);

switch (dls_perm->typ) {

case D21_DLSP_INDIV:				/*	individual	*/

dls_perm->typ = INDIVIDUAL;
if ((n = d2a138_orname_fr_ipc(name_off,block,&dls_perm->v.individual,err_apdu,err_len)) != D2_NOERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

break;

case D21_DLSP_MEM_DL:				/*	member-of-dl	*/

dls_perm->typ = MEMBER_OF_DL;
if ((n = d2a138_orname_fr_ipc(name_off,block,&dls_perm->v.member_of_dl,err_apdu,err_len )) != D2_NOERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

break;

case D21_DLSP_PATT:				/*	pattern-match	*/

dls_perm->typ = PATTERN_MATCH;
if ((n = d2a138_orname_fr_ipc(name_off,block,&dls_perm->v.pattern_match,err_apdu,err_len )) != D2_NOERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

break;

case D21_DLSP_MEM_G:				/*	member-of-group	*/

dls_perm->typ = MEMBER_OF_GROUP;
   
if ((dls_perm->v.member_of_group.name =
				D27_MALLOC(svc_c_sev_warning,sizeof(dua_dist_name))) == NULL)
   {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
   }

if ( d2a_016_str_to_dn ((char *)block,dls_perm->v.member_of_group.name,
			err_apdu,err_len) != D2_NOERROR )
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

break;

default:

D2_ASN1_ERR_RETURN_WITH_TRACE

}

return(D2_NOERROR);		/*	End of d2a_136_dlsp_fr_ipc	*/
}

static signed32 d2a_151_st_oun_fr_ipc(
  O_U_Names ** o_u_names,
  string bl,
  signed32 i,
  byte * err_apdu,
  signed32 * err_len )
{
const char function_name[] = "d2a_151_st_oun_fr_ipc";
if ( i==1 )
   if ((*o_u_names = D27_MALLOC(svc_c_sev_warning,sizeof(O_U_Names))) == NULL)
      {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

(*o_u_names)->v[i-1] = bl;
(*o_u_names)->n = i;

return (D2_NOERROR);
}


static void d2a_152_ex_oun_fr_ipc(
  E_Attrs * p_ext,
  string bl,
  signed32 i )
{
if ( i==1 ) p_ext->n++;
p_ext->v[p_ext->n-1].e_type = teletex_organizational_unit_names;
p_ext->v[p_ext->n-1].e_value.ou_val.n = i;
p_ext->v[p_ext->n-1].e_value.ou_val.v[i-1] = bl;
}


static signed32 d2a_153_dt_fr_ipc(
  D_D_Attrs **d_d_attrs,
  string bl,
  signed32 i,
  byte * err_apdu,
  signed32 * err_len )
{
const char function_name[] = "d2a_153_dt_fr_ipc";
if ( i == 1 )
   if ((*d_d_attrs = D27_MALLOC(svc_c_sev_warning,sizeof(D_D_Attrs))) == NULL)
      {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

(*d_d_attrs)->n = i;
(*d_d_attrs)->v[i-1].type = bl;
return (D2_NOERROR);
}


static void d2a_154_ex_dom_fr_ipc(
  E_Attrs * p_ext,
  string bl,
  signed32 i,
  signed32 type )
{
if ( type == DT )
   {
   if ( i == 1 ) p_ext->n++;
   p_ext->v[p_ext->n-1].e_type = teletex_domain_defined_attributes;
   p_ext->v[p_ext->n-1].e_value.dd_val.n = i;
   p_ext->v[p_ext->n-1].e_value.dd_val.v[i-1].type = bl;
   }
else
   {
   p_ext->v[p_ext->n-1].e_value.dd_val.v[i-1].value = bl;
   }
}

static signed32 d2a_155_pds_fr_ipc(
  E_Attrs * p_ext,
  D21_OR_address_part * addr_part,
  signed16 addr_off,
  signed32 e_type )
{
const char function_name[] = "d2a_155_pds_fr_ipc";
D21_str_list	* str_list;
string		block;
signed16		str_off ;
unsigned16	 str_rep;

str_list = (D21_str_list *) (addr_part + 1);
str_off = sizeof(D21_str_list) + str_list->d21_strl_off;

p_ext->v[p_ext->n].e_type = e_type;
p_ext->v[p_ext->n].e_value.pds_val.printable = NULL;
p_ext->v[p_ext->n].e_value.pds_val.t61 = NULL;

while ( str_off <= addr_off )
   {
   str_rep = str_list->d21_strl_rep;
   block = (string) (str_list + 1);

   if (str_rep == D2_PRINTABLE)				/* E(type) (P)	*/
      p_ext->v[p_ext->n].e_value.pds_val.printable = block;

   else if (str_rep == D2_T61)				/* E(type) (T)	*/
      p_ext->v[p_ext->n].e_value.pds_val.t61 = block;

   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   str_list = (D21_str_list *) ( (byte *) block + str_off ); 
   str_off += sizeof(D21_str_list) + str_list->d21_strl_off;
   }
p_ext->n++;
return (D2_NOERROR);
}

