/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20hdlat.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:18  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:32  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:52  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:18:46  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:06:08  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:14:50  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:08:09  marrek
 * 	Introduce D2_ANY_STX
 * 	[1993/02/01  17:41:32  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:55  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:35  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:52:47  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:00:49  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:53:29  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:32:05  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  21:39:47  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:16  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20hdlat.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:19 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20hdlat.o 					      */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 07.02.91                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.	: <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 23.03.88| Original                       | ws |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*		       Modulidentification			      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2shm.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define BLANK	' '

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/
typedef struct 
	{
	Abbrv    att_abbrv;
	char     att_obj_id[L_OBJ_ID];
	signed32 att_syntax;
	} Def_at;

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

static  Def_at	def_at[NO_DEF_AT] = 
			{
			S_AT_ACL, S_AT_ACL_OBJ_ID, D2_ACCL,
			S_AT_AON, S_AT_AON_OBJ_ID, D2_DISTNAME,
			S_AT_AT,  S_AT_AT_OBJ_ID,  D2_PRINTABLE,
			S_AT_BC,  S_AT_BC_OBJ_ID,  D2_CIGNORE,
			S_AT_C,   S_AT_C_OBJ_ID,   D2_COUNTRY_STX,
			S_AT_CDC, S_AT_CDC_OBJ_ID, D2_OCTET,
			S_AT_CDR, S_AT_CDR_OBJ_ID, D2_OCTET,
			S_AT_CN,  S_AT_CN_OBJ_ID,  D2_CIGNORE,
			S_AT_DI,  S_AT_DI_OBJ_ID,  D2_CIGNORE,
			S_AT_DSC, S_AT_DSC_OBJ_ID, D2_CIGNORE,
			S_AT_FTN, S_AT_FTN_OBJ_ID, D2_FAXNR_STX,
			S_AT_IIN, S_AT_IIN_OBJ_ID, D2_NUMERIC,
			S_AT_KNI, S_AT_KNI_OBJ_ID, D2_CIGNORE,
			S_AT_L,   S_AT_L_OBJ_ID,   D2_CIGNORE,
			S_AT_MDE, S_AT_MDE_OBJ_ID,   D2_OBJ_IDENT,
			S_AT_MDL, S_AT_MDL_OBJ_ID,   D2_INTEGER,
			S_AT_MDM, S_AT_MDM_OBJ_ID,   D2_MHS_ORNAME_STX,
			S_AT_MDS, S_AT_MDS_OBJ_ID,   D2_MHS_DLSP_STX,
			S_AT_MDT, S_AT_MDT_OBJ_ID,   D2_OBJ_IDENT,
			S_AT_MEM, S_AT_MEM_OBJ_ID, D2_DISTNAME,
			S_AT_MK,  S_AT_MK_OBJ_ID,  D2_DISTNAME,
			S_AT_MMS, S_AT_MMS_OBJ_ID,   D2_DISTNAME,
			S_AT_MOA, S_AT_MOA_OBJ_ID,   D2_MHS_ORADR_STX,
			S_AT_MPD, S_AT_MPD_OBJ_ID,   D2_MHS_PR_DM_STX,
			S_AT_MSA, S_AT_MSA_OBJ_ID,   D2_OBJ_IDENT,
			S_AT_MSC, S_AT_MSC_OBJ_ID,   D2_OBJ_IDENT,
			S_AT_MSO, S_AT_MSO_OBJ_ID,   D2_OBJ_IDENT,
			S_AT_O,   S_AT_O_OBJ_ID,   D2_CIGNORE,
			S_AT_OCL, S_AT_OCL_OBJ_ID, D2_OBJ_IDENT,
			S_AT_OCT, S_AT_OCT_OBJ_ID, D2_PRINTABLE,
			S_AT_OU,  S_AT_OU_OBJ_ID,  D2_CIGNORE,
			S_AT_OWN, S_AT_OWN_OBJ_ID, D2_DISTNAME,
			S_AT_PA,  S_AT_PA_OBJ_ID,  D2_POST_AD_STX,
			S_AT_PC,  S_AT_PC_OBJ_ID,  D2_CIGNORE,
			S_AT_PDM, S_AT_PDM_OBJ_ID, D2_PR_DM_STX,
			S_AT_PDO, S_AT_PDO_OBJ_ID, D2_CIGNORE,
			S_AT_POB, S_AT_POB_OBJ_ID, D2_CIGNORE,
			S_AT_PSA, S_AT_PSA_OBJ_ID, D2_PSAP_STX,
			S_AT_RA,  S_AT_RA_OBJ_ID,  D2_POST_AD_STX,
			S_AT_RO,  S_AT_RO_OBJ_ID,  D2_DISTNAME,
			S_AT_SAC, S_AT_SAC_OBJ_ID, D2_OBJ_IDENT,
			S_AT_SEA, S_AT_SEA_OBJ_ID, D2_DISTNAME,
			S_AT_SER, S_AT_SER_OBJ_ID, D2_PRINTABLE,
			S_AT_SG,  S_AT_SG_OBJ_ID,  D2_GUIDE,
			S_AT_SN,  S_AT_SN_OBJ_ID,  D2_CIGNORE,
			S_AT_SPN, S_AT_SPN_OBJ_ID, D2_CIGNORE,
			S_AT_SRT, S_AT_SRT_OBJ_ID, D2_PRINTABLE,
			S_AT_STA, S_AT_STA_OBJ_ID, D2_CIGNORE,
			S_AT_TIT, S_AT_TIT_OBJ_ID, D2_CIGNORE,
			S_AT_TN,  S_AT_TN_OBJ_ID,  D2_TELNR_STX,
			S_AT_TST, S_AT_TST_OBJ_ID, D2_TIME,
			S_AT_TTI, S_AT_TTI_OBJ_ID, D2_TTXID_STX,
			S_AT_TXN, S_AT_TXN_OBJ_ID, D2_TLXNR_STX,
			S_AT_UP,  S_AT_UP_OBJ_ID,  D2_PASSWD_STX,
			S_AT_X1A, S_AT_X1A_OBJ_ID, D2_NUMERIC
			};
			
/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static signed32 d20_check_at_integrity(Errcode *ret_code, At *at_entry);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_010_check_attr()                                  */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_010_check_attr(ret_code,upd_info)        */
/*		Errcode		*ret_code;			      */
/*		S_upd_info	*upd_info;			      */	
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function does validation checks for attribute    */
/*		table modification. Validation checks are dependent   */
/*		on the kind of modification (Add a new attribute/     */
/*		modify an attribute/delete an attribute)	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		S_upd_info	*upd_info;			      */	
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode	*ret_code;	pointer to errocde	      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred in memory reallocation         */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed32 d20_010_check_attr(
   Errcode    *ret_code,
   S_upd_info *upd_info)

{		/* begin d20_010_check_attr()	*/
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/
	signed32  ret_value = D2_NOERROR;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

switch ((int) upd_info->upd_mode)
	{
	case	ADD_AT_ENTRY:
		{
		At	 *at_entry = upd_info->upd_val.a_m_at->at_entry;
		signed32 no_at = upd_info->upd_val.a_m_at->no_at;
		At	 *at = upd_info->upd_val.a_m_at->at;
		signed32 j;

		if ((ret_value = d20_check_at_integrity(ret_code, at_entry)) 
								== D2_ERROR)
			break; /* break from case ADD_AT_ENTRY.		*/

		/* check consistency */
		/* check if some At entry with same ABBRV ,  same */
		/* Object identifier or same name already exists. */

		for (j=0; j<no_at; j++)
			{
			if (strcmp(at_entry->att_abbrv, (at + j)->att_abbrv) 
									== 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR,DS_EXISTS);
				ret_value = D2_ERROR;
				break; /* break from for loop.		*/
				}

			if (strcmp(at_entry->att_name,(at + j)->att_name) == 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_NAME_NUNIQUE);
				ret_value = D2_ERROR;
				break; /* break from for loop.		*/
				}

			if (strcmp(at_entry->att_obj_id, (at + j)->att_obj_id) 
									== 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_OBJID_NUNIQUE);
				ret_value = D2_ERROR;
				break; /* break from for loop.		*/
				}
			}

		break; /* break from case ADD_AT_ENTRY.		*/
		}

	case	MOD_AT_ENTRY:
		{
		At	 *at_entry = upd_info->upd_val.a_m_at->at_entry;
		signed32 no_at = upd_info->upd_val.a_m_at->no_at;
		At	 *at = upd_info->upd_val.a_m_at->at;
		signed32 no_srt = upd_info->upd_val.a_m_at->no_srt;
		Srt	 *srt = upd_info->upd_val.a_m_at->srt;
		signed32 i, j, index;

		/* entry should exist in At 			*/
		if ((index = d20_014_search_at_abbrv(at_entry->att_abbrv, 
					no_at, (char *) at, D2_TOTAL_SCHEMA)) 
									== -1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_ENT_NFOUND);
			ret_value = D2_ERROR;
			break; /* break from case MOD_AT_ENTRY.		*/
			}

		/* check integrity of entry.	*/
		if ((ret_value = d20_check_at_integrity(ret_code, at_entry)) 
								== D2_ERROR)
			break; /* break from case MOD_AT_ENTRY.		*/

		/* check consistency */

		/* check if attribute of default schema is being modified */
		/* and object identifier has been changed or attribute    */	
		/* syntax has been changed.				  */
		if ((j = d20_014_search_at_abbrv(at_entry->att_abbrv, NO_DEF_AT,
				(char *) def_at, D2_DEF_SCHEMA)) != -1)
			{			
			/* check if object identifier has been changed.	*/
			if (strcmp(at_entry->att_obj_id,
					(def_at + j)->att_obj_id) != 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_MOD_OID_DEF_SCH);
				ret_value = D2_ERROR;
				break; /* break from case MOD_AT_ENTRY.	*/
				}

			/* check if attr. syntax has been changed.	*/
			if (at_entry->att_syntax != (def_at + j)->att_syntax)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_MOD_SNTX_DEF_AT);
				ret_value = D2_ERROR;
				break; /* break from case MOD_AT_ENTRY.	*/
				}
			}

		/* check if some At entry (except the one  */
		/* which is being modified) with same Obj. */
		/* Id. or with same name already exists.   */
		for (j=0; j<no_at; j++)
			{
			if (j != index)
				{
				if (strcmp(at_entry->att_name,(at+j)->att_name) 
									== 0)
					{
					d20_errcode(ret_code, CALLERR, PARERR, 
							DS_NAME_NUNIQUE);
					ret_value = D2_ERROR;
					break; /* break from for loop.	*/
					}

				if (strcmp(at_entry->att_obj_id,
						      (at+j)->att_obj_id) == 0)
					{
					d20_errcode(ret_code, CALLERR, PARERR, 
							DS_OBJID_NUNIQUE);
					ret_value = D2_ERROR;
					break; /* break from for loop.	*/
					}
				}
			} 

		if (ret_value != D2_ERROR)
			{
			/* till now no error.	*/
			/* check if attribute is a naming attribute.	*/
			/* if it is a naming attr., syntax must be      */
			/* CASE IGNORE and no. of recurring values      */
			/* should be +ve.				*/

			/* check if attribute is naming attribute for some */
			/* structure rule.				   */
			for (j=0; j<no_srt; j++, srt++)
				{
				for (i=0; i<srt->no_nam_attr; i++)
					if (strcmp(at_entry->att_abbrv,
							srt->nam_attr[i]) == 0)
						break; /* break from for loop.*/

				if (i != srt->no_nam_attr)
					break;
				}

			if (j != no_srt)
				{
				/* attribute is naming attribute.	*/

				if (at_entry->att_no_recur == 0)
					{
					d20_errcode(ret_code, CALLERR, PARERR, 
							DS_WR_NAM_RECR);
					ret_value = D2_ERROR;
					break; /*break from case MOD_AT_ENTRY.*/
					}

				if (at_entry->att_syntax != D2_CIGNORE &&
				    at_entry->att_syntax != D2_IA5 &&
	    			    at_entry->att_syntax != D2_CEXACT &&
	    			    at_entry->att_syntax != D2_COUNTRY_STX &&
	    			    at_entry->att_syntax != D2_PRINTABLE)
					{
					d20_errcode(ret_code, CALLERR, PARERR, 
							DS_WR_NAM_SNTX);
					ret_value = D2_ERROR;
					break; /*break from case MOD_AT_ENTRY.*/
					}

				}

			}

		break;
		}

	case	DEL_AT_ENTRY:
		{
		char	 *abbrv = upd_info->upd_val.d_at->at_abbrv;
		signed32 no_at = upd_info->upd_val.d_at->no_at;
		At	 *at = upd_info->upd_val.d_at->at;
		signed32 no_oct = upd_info->upd_val.d_at->no_oct;
		Oct	 *oct = upd_info->upd_val.d_at->oct;
		signed32 no_srt = upd_info->upd_val.d_at->no_srt;
		Srt	 *srt = upd_info->upd_val.d_at->srt;
		signed32 i,j;

		/* entry should not belong to default schema.	*/
		if (d20_014_search_at_abbrv(abbrv, NO_DEF_AT, (char *) def_at, 
							D2_DEF_SCHEMA) != -1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_DEL_DEF_SCH);
			ret_value = D2_ERROR;
			break; /* break from case DEL_SRT_ENTRY	*/
			}

		/* entry should exist in At and should not be in use 	*/
		/* either in Oct or in Srt.				*/
		if (d20_014_search_at_abbrv(abbrv, no_at, (char *) at,
				D2_TOTAL_SCHEMA) == -1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_ENT_NFOUND);
			ret_value = D2_ERROR;
			break; /* break from case DEL_AT_ENTRY.		*/
			}

		/* check if attribute is naming attribute for some */
		/* structure rule.				   */
		for (i=0; i<no_srt; i++, srt++)
			{
			for (j=0; j<srt->no_nam_attr; j++)
				if (strcmp(abbrv,srt->nam_attr[j]) == 0)
					break; /* break from for loop.	*/

			if (j != srt->no_nam_attr)
				break;
			}
			
		if (i != no_srt)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_AT_IN_USE);
			ret_value = D2_ERROR;
			break; /* break from case DEL_AT_ENTRY.		*/
			}

		/* check if attribute is mandatory/optional */
		/* attr. for some object class.		    */
		for (i=0; i<no_oct; i++, oct++)
			{
			/* check if attribute is mandatory  */
			for (j=0; j<oct->no_must_attr; j++)
				if (strcmp(abbrv, oct->must_attr[j]) == 0)
					break; /* break from for loop.	*/

			if (j < oct->no_must_attr)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
								DS_AT_IN_USE);
				ret_value = D2_ERROR;
				break; /* break from for loop.	*/
				}

			/* check if attr. is optnl. */
			for (j=0;j<oct->no_may_attr;j++)
				if (strcmp(abbrv, oct->may_attr[j]) == 0)
					break; /* break from for loop.	*/

			if (j < oct->no_may_attr)
				{
				d20_errcode(ret_code, CALLERR, PARERR, 
								DS_AT_IN_USE);
				ret_value = D2_ERROR;
				break; /* break from for loop.	*/
				}
			}

		break; /* break from case DEL_AT_ENTRY.		*/
		}
	}

return(ret_value);

}		/* end of d20_010_check_attr()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_at_integrity()                              */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_check_at_integrity(ret_code, at_entry)   */
/*		Errcode *ret_code;				      */
/*		At	*at_entry;				      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks for the correctness of various   */
/*		fields of an At entry.				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		At	*at_entry;				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode	*ret_code;	pointer to errocde	      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred 				      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 static signed32 d20_check_at_integrity(
   Errcode *ret_code,
   At      *at_entry)

{		/* begin d20_check_at_integrity()	*/
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* check attribute abbreviation.	*/
if (d20_015_check_string(at_entry->att_abbrv) == D2_ERROR)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_ABBRV);
	return(D2_ERROR);
	}

/* check attribute name			*/
if (d20_015_check_string(at_entry->att_name) == D2_ERROR)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_NAME);
	return(D2_ERROR);
	}

/* check attribute object identifier	*/
if (d20_016_check_obj_id(at_entry->att_obj_id) == D2_ERROR)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_OBJ_ID_FORMAT);
	return(D2_ERROR);
	}

if ((at_entry->att_lower < 0) || (at_entry->att_upper < 
				  at_entry->att_lower))
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_BOUND);
	return(D2_ERROR);
	}

/* check attribute no. of recurring vals.*/
if (at_entry->att_no_recur < 0)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_NO_RECUR);
	return(D2_ERROR);
	}

/* check for attribute syntax.		*/
switch((int) at_entry->att_syntax)
	{
	case D2_GUIDE :
	case D2_POST_AD_STX :
	case D2_TLXNR_STX : 
	case D2_TTXID_STX :
	case D2_FAXNR_STX : 
	case D2_PR_DM_STX :
	case D2_PSAP_STX :
	case D2_CERTIFICATE :
	case D2_CERT_PAIR :
	case D2_CERT_LIST :
	case D2_COUNTRY_STX :
	case D2_PASSWD_STX :
	case D2_ACCL :
	case D2_DISTNAME : 
	case D2_OBJ_IDENT :
	case D2_CEXACT : 
	case D2_CIGNORE :
	case D2_IA5:
	case D2_PRINTABLE :
	case D2_NUMERIC : 
	case D2_CIGN_LIST :
	case D2_BOOLEAN : 
	case D2_INTEGER :
	case D2_OCTET : 
	case D2_TIME : 
	case D2_TELNR_STX :
	case D2_MHS_DLSP_STX :
	case D2_MHS_ORADR_STX :
	case D2_MHS_ORNAME_STX :
	case D2_MHS_PR_DM_STX :
	case D2_ASN1:
	case D2_ANY_STX:
		break;

	default:
		d20_errcode(ret_code,CALLERR, PARERR, DS_WR_SYNTAX);
		return(D2_ERROR);
	}

/* check for PR_DM_STX and MHS_PR_DM_STX syntaxes.	*/
if ((at_entry->att_syntax == D2_PR_DM_STX ||
     at_entry->att_syntax == D2_MHS_PR_DM_STX) && (at_entry->att_no_recur != 1))
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_PR_DM_STX_RECUR);
	return(D2_ERROR);
	}

/* check for attribute ACL		*/
if ((at_entry->att_acl != PUBLIC) && (at_entry->att_acl != STANDARD) &&
				     (at_entry->att_acl != SENSITIVE))
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_ACL);
	return(D2_ERROR);
	}

/* check for attribute index level	*/
if (at_entry->att_ind_lev < 0)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_IND_LEV);
	return(D2_ERROR);
	}

/* check for attribute phonetical matching.	*/
if ((at_entry->att_phon != TRUE) && (at_entry->att_phon != FALSE))
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_PHON);
	return(D2_ERROR);
	}

return(D2_NOERROR);

}		/* end of d20_check_at_integrity()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_014_search_at_abbrv()                             */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_014_search_at_abbrv(abbrv, no_at, at,    */
/*					schema)                       */
/*		char	*abbrv;		attr. abbreviation.	      */
/*		signed32 no_at;		no. entries in attr. table.   */
/*		char	*at;		pointer to Attr. table.       */
/*		signed32 schema;	type of schema in which       */
/*					abbrv. is to be searched.     */
/*				(D2_TOTAL_SCHEMA/D2_DEF_SCHEMA)	      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function searches for attr. abbreviation in attr.*/
/*		table. If found, it returns the index in attr. table  */
/* 		otherwise it returns -1.			      */
/*		Which attribute table is to be searched depends on    */
/*		schema flag.				    	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	*abbrv;		attr. abbreviation.	      */
/*		signed32 no_at;		no. entries in attr. table.   */
/*		char	*at;		pointer to Attr. table.       */
/*		signed32 schema;	schema type (Total schema or  */
/*					default schema)		      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		returns index in attr. table if abbrv. is found else  */
/*		returns -1 .					      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed32 d20_014_search_at_abbrv(
   char     *abbrv,
   signed32  no_at,
   char     *at,
   signed32  schema)

{		/* begin d20_014_search_at_abbrv()	*/
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/
	signed16 found = FALSE;
	signed32 offset;
	signed32 no_entries;
	signed32 index;
	char	*next_abbrv;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

no_entries = no_at;
offset = 0;

while (found == FALSE)
	{
	if (no_entries == 0)
		break;

	index = offset + (no_entries/2);

	if (schema == D2_TOTAL_SCHEMA)
		next_abbrv = ((At *)((At *)at + index))->att_abbrv;
	else
		next_abbrv = ((Def_at *)((Def_at *)at + index))->att_abbrv;

	if (strcmp(abbrv,next_abbrv) == 0)
		{
		found = TRUE;
	 	break;
		}	
	else
		if (strcmp(abbrv,next_abbrv) < 0)
			no_entries = no_entries/2;
		else
			{
			offset = index + 1; 
			if (no_entries%2 == 0)
				no_entries = (no_entries/2) - 1;
			else
				no_entries = no_entries/2;
			} 
	}

if (found == TRUE)
	return(index);
else
	return(-1);

}		/* end d20_014_search_at_abbrv()	*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_015_check_string()                                */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_015_check_string(str)    	      */
/*		char	*str;					      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function returns ERROR if str is of length 0 or  */
/*		contains non-printable characters or contains blank   */
/*		characters otherwise it returns NOERROR.	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	*str;					      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		D2_NOERROR					      */
/*		D2_ERROR					      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 d2_ret_val d20_015_check_string(
   char	*str)

{		/* begin d20_015_check_string()	*/
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* check if str is of length 0 */
if (strlen(str) == 0)
	return(D2_ERROR);

if (d27_605_is_PrtString ((byte *)str, (signed16)strlen(str)) == D2_ERROR)
	return(D2_ERROR);                                         

/* look for BLANK in str	*/
while (*str != D2_EOS)
	if (*str == BLANK)
		break;
	else
		str++;

if (*str != D2_EOS)
	return(D2_ERROR);

return(D2_NOERROR);

}		/* end of d20_015_check_string()	*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_016_check_obj_id()                                */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_016_check_obj_id(str)    	      */
/*		char	*str;					      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks if 'str' is of the format(obj_id)*/
/*		'integer.integer.integer. ...'			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	*str;					      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		D2_NOERROR					      */
/*		D2_ERROR					      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 d2_ret_val d20_016_check_obj_id(
   char	*str)

{		/* begin d20_016_check_obj_id()	*/
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

	unsigned char	obj_id_str[D2_OBJ_IDL_MAX + 1];
	unsigned char	next_int_str[D2_OBJ_IDL_MAX + 1];
	signed32	next_int;
	signed32	k;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

strcpy((char *)obj_id_str, str);

/* check characters of obj_id_str.		*/
if (d20_015_check_string((char *) obj_id_str) == D2_ERROR)
	return(D2_ERROR);

/* check format of obj_id_str.			*/
while ((k = sscanf((char *)obj_id_str, "%ld.%s", &next_int, obj_id_str)) >= 1)
	if (k == 1)
		{
		sprintf((char *)next_int_str, "%d", next_int);
		if (strcmp((char *)next_int_str, (char *)obj_id_str) == 0)
			return(D2_NOERROR);
		else
			return(D2_ERROR);
		}
	else
		continue;

return(D2_ERROR);

}		/* end of d20_016_check_obj_id()	*/
/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_011_add_attr()                                    */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_011_add_attr(ret_code,at_entry,no_at,at) */
/*		Errcode	 *ret_code;	pointer to errocde	      */
/*		At	 *at_entry; 	pointer to at entry to be     */
/*					added			      */
/*		signed32 *no_at;	pointer to no. entries in     */
/*					attribute table.	      */
/*		At	**at;		pointer to pointer to Attr.   */
/*					table.			      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function adds a new attribute in attribute table.*/
/*		New attribute is added, keeping the attributes sorted */
/*		on attribute abbreviation. If the new attribute can   */
/*		not be inserted in the current Attr. table because of */
/*		Attribute table being full, space is reallocated for  */
/*		attribute table.				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		At	 *at_entry; 	pointer to at entry to be     */
/*					added			      */
/*		signed32 *no_at;	pointer to no. entries in     */
/*					attribute table.	      */
/*		At	**at;		pointer to pointer to Attr.   */
/*					table.			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode	 *ret_code;	pointer to errocde	      */
/*		signed32 *no_at;	pointer to no. entries in     */
/*					attribute table.	      */
/*		At	**at;		pointer to pointer to Attr.   */
/*					table.			      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:  No error occurred                                */
/*      D2_ERROR:    Error occurred in memory reallocation            */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed32 d20_011_add_attr(
   Errcode   *ret_code,
   At        *at_entry,
   signed32  *no_at,
   At       **at)

{		/* begin d20_011_add_attr()	*/
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

	char	*mem;
	signed32 i,j;
	At	*tmp_ent;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* check if new attribute can be inserted in current attribute table  */
if (*no_at == *(signed32 *)(*at + *no_at)) 
	{/* there is no space in current attribute table */
	/* reallocate space for attribute table	*/
  	if ((mem = realloc(*at, ((*no_at + D2_A_MORE) * sizeof(At)) + 
				  sizeof(signed32))) == (char *)NULL) 
		{
  		d20_errcode(ret_code, TEMPERR, SYSERR, DS_ALLOC_ERROR);
		return(D2_ERROR);
		}

	*at = (At *)mem;

	/* store new attribute no. limit */
	*(signed32 *)(*at + *no_at) += D2_A_MORE;
	}

/* at this point enough space is there in attribute table to store a 	*/
/* new attribute.							*/

/* find the position where new attribute is to be inserted.	*/
for (j = 0; j < *no_at; j++)
	if (strcmp(at_entry->att_abbrv,(*at + j)->att_abbrv) < 0)
		break;

/* store the attribute no. limit at new position (one At structure ahead). */
*(signed32 *)(*at + *no_at + 1) = *(signed32 *)(*at + *no_at);

/* shift the attribute table from the point where new attribute is to be   */
/* inserted, by sizeof(At) towards higher memory address.		   */

/* tmp_ent points to last free entry.		*/
tmp_ent = *at + *no_at;

for (i = 0; i < *no_at - j; i++, tmp_ent--)
	memcpy((char *)tmp_ent, (char *)(tmp_ent - 1), sizeof(At));

/* new attribute is inserted in the proper slot in the attribute table */
memcpy((char *)(*at + j), (char *)at_entry, sizeof(At));

(*no_at)++;

return(D2_NOERROR);
}		/* end of d20_011_add_attr()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_012_del_attr()                                    */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_012_del_attr(abbrv,no_at,at)      	      */
/*		char	 *abbrv;	abbreviation of the attr. to  */
/*					be deleted.		      */
/*		signed32 *no_at;	pointer to no. entries in     */
/*					attribute table.	      */
/*		At	*at;		pointer to Attr. table.	      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function deletes an attribute from the attribute */
/*		table. Entries ahead of the attribute are shifted     */
/*		towards lower address by one At entry. New no. of     */
/*		entries in attribute table is returned as a result.   */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	 *abbrv;	abbreviation of the attr. to  */
/*					be deleted.		      */
/*		signed32 *no_at;	pointer to no. entries in     */
/*					attribute table.	      */
/*		At	 *at;		pointer to Attr. table.	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *no_at;	pointer to no. entries in     */
/*					attribute table.	      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed32 d20_012_del_attr(
   char     *abbrv,
   signed32 *no_at,
   At       *at)

{		/* begin d20_012_del_attr()	*/
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

	signed32	j;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* search attribute in attribute table .	*/
j = d20_014_search_at_abbrv(abbrv, *no_at, (char *) at, D2_TOTAL_SCHEMA);

/* shift the entries, ahead of the entry to be deleted, by sizeof(At) towards */
/* lower memory address.		   				      */
memcpy((char *)(at + j), (char *)(at + j + 1), sizeof(At) * (*no_at - j - 1));

/* store the capacity of attribute table at the proper place.	*/
*(signed32 *)(at + *no_at - 1) = *(signed32 *)(at + *no_at);

(*no_at)--;

return(D2_NOERROR);

}		/* end of d20_012_del_attr()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_013_mod_attr()                                    */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_013_mod_attr(at_entry,no_at,at)          */
/*		At	 *at_entry;	pointer to modified At entry. */
/*		signed32  no_at;	no. entries in attr. table.   */
/*		At	 *at;		pointer to Attr. table.	      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function stores an At entry in attribute table   */
/*		after it has been modified.			      */ 
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		At	 *at_entry;	pointer to modified At entry. */
/*		signed32  no_at;	no. entries in attr. table.   */
/*		At	 *at;		pointer to Attr. table.	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed32 d20_013_mod_attr(
   At       *at_entry,
   signed32  no_at,
   At       *at)

{		/* begin d20_013_mod_attr()	*/
/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

	signed32	j;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* search attribute in attribute table .	*/
j = d20_014_search_at_abbrv(at_entry->att_abbrv, no_at, (char *) at, D2_TOTAL_SCHEMA);

/* copy at_entry into attribute table.	*/
memcpy((char *)(at + j),(char *)at_entry, sizeof(At));

return(D2_NOERROR);
}	/* end of d20_013_mod_attr()		*/
