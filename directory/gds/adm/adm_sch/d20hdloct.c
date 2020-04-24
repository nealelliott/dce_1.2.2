/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20hdloct.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:20  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:35  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:26  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:04:04  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:19:09  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:08:02  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:15:19  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:40  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:53:33  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:01:20  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:40:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:24  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20hdloct.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:21 $";
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
/* NAME         : d20hdloct.o 					      */
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

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

typedef struct
	{
	Abbrv	 obj_abbrv;
	signed32 no_sup_class;
	Abbrv	 sup_class[NO_SUP_CLASS];
	char	 obj_id[L_OBJ_ID];
	signed32 obj_class_kind;
	signed32 no_must_attr;
	Abbrv	 must_attr[NO_MAND_ATTR];
	} Def_oct;

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

static  Def_oct 	def_oct[NO_DEF_OCT] =
		{		
		{S_OC_ALI, 1, {S_OC_TOP}, S_OC_ALI_OBJ_ID, D2_ALIAS, 
						1, S_AT_AON},
		{S_OC_APE, 1, {S_OC_GTP}, S_OC_APE_OBJ_ID, D2_STRUCTURAL, 
						2, S_AT_CN, S_AT_PSA},
		{S_OC_APP, 1, {S_OC_GTP}, S_OC_APP_OBJ_ID, D2_STRUCTURAL, 
						1, S_AT_CN},
		{S_OC_C,   1, {S_OC_GTP}, S_OC_C_OBJ_ID,   D2_STRUCTURAL, 
						1, S_AT_C},
		{S_OC_DEV, 1, {S_OC_GTP}, S_OC_DEV_OBJ_ID, D2_STRUCTURAL, 
						1, S_AT_CN},
		{S_OC_DSA, 1, {S_OC_APE}, S_OC_DSA_OBJ_ID, D2_STRUCTURAL, 
						0},
		{S_OC_GON, 1, {S_OC_GTP}, S_OC_GON_OBJ_ID, D2_STRUCTURAL, 
						2, S_AT_CN, S_AT_MEM},
		{S_OC_GTP, 1, {S_OC_TOP}, S_OC_GTP_OBJ_ID, D2_ABSTRACT, 
						0},
		{S_OC_LOC, 1, {S_OC_GTP}, S_OC_LOC_OBJ_ID, D2_STRUCTURAL, 
						0},
		{S_OC_MDL, 1, {S_OC_GTP}, S_OC_MDL_OBJ_ID, D2_STRUCTURAL, 
						3, S_AT_CN, S_AT_MDS, S_AT_MOA},
		{S_OC_MMS, 1, {S_OC_APE}, S_OC_MMS_OBJ_ID, D2_STRUCTURAL, 
						0},
		{S_OC_MTA, 1, {S_OC_APE}, S_OC_MTA_OBJ_ID, D2_STRUCTURAL, 
						0},
		{S_OC_MUA, 1, {S_OC_APE}, S_OC_MUA_OBJ_ID, D2_STRUCTURAL, 
						0},
		{S_OC_MUS, 1, {S_OC_TOP}, S_OC_MUS_OBJ_ID, D2_AUXILIARY, 
						1, S_AT_MOA},
		{S_OC_ORG, 1, {S_OC_GTP}, S_OC_ORG_OBJ_ID, D2_STRUCTURAL, 
						1, S_AT_O},
		{S_OC_ORP, 1, {S_OC_PER}, S_OC_ORP_OBJ_ID, D2_STRUCTURAL, 
						0},
		{S_OC_ORR, 1, {S_OC_GTP}, S_OC_ORR_OBJ_ID, D2_STRUCTURAL, 
						1, S_AT_CN},
		{S_OC_OU,  1, {S_OC_GTP}, S_OC_OU_OBJ_ID,  D2_STRUCTURAL, 
						1, S_AT_OU},
		{S_OC_PER, 1, {S_OC_GTP}, S_OC_PER_OBJ_ID, D2_ABSTRACT, 
						2, S_AT_CN, S_AT_SN},
		{S_OC_REP, 1, {S_OC_PER}, S_OC_REP_OBJ_ID, D2_STRUCTURAL, 
						1, S_AT_L},
		{S_OC_SCH, 1, {S_OC_GTP}, S_OC_SCH_OBJ_ID, D2_STRUCTURAL, 
						1, S_AT_CN},
		{S_OC_TOP, 0, {""},    S_OC_TOP_OBJ_ID, D2_ABSTRACT, 
						1, S_AT_OCL}
		};

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static signed32 d20_check_obj_integrity(Errcode *ret_code, Oct *oct_entry);
static signed32 d20_check_oclasses_attrs(Oct *oct_entry, signed32 no_oct,
			Oct *oct, signed32 no_at, At *at, Errcode *ret_code);
static signed32 d20_check_sclasses(Oct *oct_entry, signed32 no_oct, Oct *oct,
			Errcode *ret_code);
static signed32 d20_check_sclass_loop(char *obj_abbrv, Oct *oct_entry,
			signed32 no_oct, Oct *oct);
static signed32 d20_check_attrs_with_sclass(Oct *orig_oct_entry, Oct *oct_entry,
			signed32 no_oct, Oct *oct, Errcode *ret_code);
static signed32 d20_check_gtp_for_str(Oct *oct_entry, signed32 no_oct, Oct *oct);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_020_check_obj()                                   */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_020_check_obj(ret_code,upd_info)         */
/*		Errcode		*ret_code;			      */
/*		S_upd_info	*upd_info;			      */	
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function does validation checks for obj. class   */
/*		table modification. Validation checks are dependent   */
/*		on the kind of modification (Add a new obj. class/    */
/*		modify an obj. class/delete an object class)	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		S_upd_info	*upd_info;			      */	
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

 signed32 d20_020_check_obj(
   Errcode    *ret_code,
   S_upd_info *upd_info)

{		/* begin d20_020_check_obj()	*/
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
	signed32	ret_value = D2_NOERROR;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

switch ((int) upd_info->upd_mode)
	{
	case	ADD_OCT_ENTRY:
		{
		Oct	 *oct_entry = upd_info->upd_val.a_m_oct->oct_entry;
		signed32  no_at = upd_info->upd_val.a_m_oct->no_at;
		At	 *at = upd_info->upd_val.a_m_oct->at;
		signed32  no_oct = upd_info->upd_val.a_m_oct->no_oct;
		Oct	 *oct = upd_info->upd_val.a_m_oct->oct;
		signed32  j;

		if ((ret_value = d20_check_obj_integrity(ret_code, oct_entry)) 
								== D2_ERROR)
			break; /* break from case ADD_OCT_ENTRY.	*/

		/* check consistency */

		/* addition of ALIAS type of object class is not allowed. */
		if (oct_entry->obj_class_kind == D2_ALIAS)
			{
			d20_errcode(ret_code,CALLERR, PARERR,DS_NO_MORE_ALIAS);
			ret_value = D2_ERROR;
			break; /* break from ADD_OCT_ENTRY.	*/
			}

		/* check if some Oct entry with same ABBRV or same */
		/* Object identifier or same name already exists.  */

		for (j=0; j<no_oct; j++)
			{
			if (strcmp(oct_entry->obj_abbrv, (oct + j)->obj_abbrv) 
									== 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR,DS_EXISTS);
				ret_value = D2_ERROR;
				break; /* break from for loop.		*/
				}

			if (strcmp(oct_entry->obj_name,(oct+j)->obj_name) == 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_NAME_NUNIQUE);
				ret_value = D2_ERROR;
				break; /* break from for loop.		*/
				}

			/* check for uniqueness of object class   */
			/* object identifier.			  */ 
			if (strcmp(oct_entry->obj_id, (oct + j)->obj_id) == 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_OBJID_NUNIQUE);
				ret_value = D2_ERROR;
				break; /* break from for loop.		*/
				}

			}

		if (ret_value == D2_ERROR)
			break; /* break from case ADD_OCT_ENTRY.	*/

		/* check for super classes, aux. classes and attributes */
		/* consistency 						*/
		ret_value = d20_check_oclasses_attrs(oct_entry, no_oct, oct, 
						     no_at, at, ret_code);

		break; /* break from case ADD_OCT_ENTRY.	*/
		}

	case	MOD_OCT_ENTRY:
		{
		Oct	 *oct_entry = upd_info->upd_val.a_m_oct->oct_entry;
		signed32  no_at = upd_info->upd_val.a_m_oct->no_at;
		At	 *at = upd_info->upd_val.a_m_oct->at;
		signed32  no_oct = upd_info->upd_val.a_m_oct->no_oct;
		Oct	 *oct = upd_info->upd_val.a_m_oct->oct;
		signed32  no_srt = upd_info->upd_val.a_m_oct->no_srt;
		Srt	 *srt = upd_info->upd_val.a_m_oct->srt;
		signed32  i,j,k,index;
		Oct	  orig_oct_entry;

		/* entry should exist in Oct 			*/
		if ((index = d20_024_search_obj_abbrv(oct_entry->obj_abbrv, 
					no_oct, (char *) oct, D2_TOTAL_SCHEMA)) == -1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_ENT_NFOUND);
			ret_value = D2_ERROR;
			break;	/* break from case MOD_OCT_ENTRY.	*/	
			}

		/* schema object class is not allowed to be modified.	*/
		if (strcmp(oct_entry->obj_abbrv, S_OC_SCH) == 0)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_MOD_SCH);
			ret_value = D2_ERROR;
			break;	/* break from case MOD_OCT_ENTRY.	*/	
			}

		/* check integrity of entry.	*/
		if ((ret_value = d20_check_obj_integrity(ret_code, oct_entry)) 
								== D2_ERROR)
			break;	/* break from case MOD_OCT_ENTRY.	*/	


		/* check consistency */

		/* object class kind can not be changed to ALIAS.	*/
		if (oct_entry->obj_class_kind == D2_ALIAS && 
		    (oct + index)->obj_class_kind != D2_ALIAS)
			{
			d20_errcode(ret_code,CALLERR, PARERR,DS_NO_MORE_ALIAS);
			ret_value = D2_ERROR;
			break; /* break from case MOD_OCT_ENTRY.	*/
			}

		/* check if object class of default schema is being modified */
		/* and superior object classes have been changed or object   */
		/* identifier has been changed or some mandatory attribute is*/        		/* missing from the new list of mandatory attribute list     */	
		/* or object class kind is changed.			     */
		if ((j = d20_024_search_obj_abbrv(oct_entry->obj_abbrv, 
				NO_DEF_OCT, (char *) def_oct, D2_DEF_SCHEMA)) != -1)
			{
			/* sup. obj. classes have been changed.		*/
			if (oct_entry->no_sup_class == 
			    (def_oct + j)->no_sup_class)
				{
				for (i=0; i<oct_entry->no_sup_class; i++)
					{
					for (k=0; k<(def_oct + j)->no_sup_class;
							k++)
						if (strcmp(
						oct_entry->sup_class[i], 
						(def_oct + j)->sup_class[k])==0)
							break;

					/* match not found.	*/
					if (k == (def_oct + j)->no_sup_class)
						break;
					}

				/* for some super class match not found. */
				if (i != oct_entry->no_sup_class)
					{
					d20_errcode(ret_code,CALLERR, PARERR, 
							DS_MOD_SOBJ_DEF_OCT);
					ret_value = D2_ERROR;
					break; /* break from MOD_OCT_ENTRY. */
					}
				}
			else
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_MOD_SOBJ_DEF_OCT);
				ret_value = D2_ERROR;
				break; /* break from case MOD_OCT_ENTRY.     */
				}

			/* object identifier has been changed.		*/
			if (strcmp(oct_entry->obj_id,
					(def_oct + j)->obj_id) != 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_MOD_OID_DEF_SCH);
				ret_value = D2_ERROR;
				break; /* break from case MOD_OCT_ENTRY.     */
				}

			/* object class kind has been changed.		*/
			if (oct_entry->obj_class_kind != 
						(def_oct + j)->obj_class_kind)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_MOD_CK_DEF_SCH);
				ret_value = D2_ERROR;
				break; /* break from case MOD_OCT_ENTRY.     */
				}

			/* check if all manadatory attributes still exist    */
			/* in mandatory attribute list of modified entry.    */
			for (i=0; i<(def_oct + j)->no_must_attr; i++)
				{
				for (k=0; k<oct_entry->no_must_attr; k++)
					if (strcmp((def_oct + j)->must_attr[i],
						   oct_entry->must_attr[k])==0)
						break;
				
				/* attribute not found.		*/
				if (k == oct_entry->no_must_attr)
					break;
				}
		
			/* some mandatory attribute missing from the new    */
			/* mandatory attribute list.			    */
			if (i != (def_oct + j)->no_must_attr)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
							DS_MOD_MAND_DEF_OCT);
				ret_value = D2_ERROR;
				break; /* break from case MOD_OCT_ENTRY.     */
				}
			}	/* end of default entry being modified.	*/

		/* check if some Oct entry (except the one */
		/* which is being modified) with same Obj. */
		/* Id. or same name already exists.	   */ 

		for (j=0; j<no_oct; j++)
			{
			if (j != index)
				{
				/* check for uniqueness of object class name */
				if (strcmp(oct_entry->obj_name,
					   (oct+j)->obj_name) == 0)
					{
					d20_errcode(ret_code,CALLERR, PARERR, 
							DS_NAME_NUNIQUE);
					ret_value = D2_ERROR;
					break; /* break from for loop.	*/
					}

				/* check for uniqueness of object identifier. */
				if (strcmp(oct_entry->obj_id,(oct + j)->obj_id) 
									== 0)
					{
					d20_errcode(ret_code, CALLERR, PARERR, 
							DS_OBJID_NUNIQUE);
					ret_value = D2_ERROR;
					break; /* break from for loop.	*/
					}

				}
			}

		if (ret_value == D2_ERROR)
			break;	/* break from case MOD_OCT_ENTRY.	*/	

		/* check for super classes, aux. classes and attributes */
		/* consistency						*/
		if ((ret_value = d20_check_oclasses_attrs(oct_entry, no_oct, 
					oct, no_at, at, ret_code)) == D2_ERROR)
			break; /* break from case MOD_OCT_ENTRY.	*/

		/* check for loop in object class hierarchy.	*/
		if ((ret_value = d20_check_sclass_loop(oct_entry->obj_abbrv, 
					oct_entry, no_oct, oct)) == D2_ERROR)
			{
			d20_errcode(ret_code, CALLERR, PARERR, DS_SCLASS_LOOP);
			break; /* break from case MOD_OCT_ENTRY.	*/
			}

		/* check if this modification will lead to a situation where */
		/* for a structure rule, naming attribute does not belong to */
		/* one of the object classes referenced in that rule.	     */

		/* check if this modification causes violation of rules      */
		/* related to super class kinds and kind of auxiliary classes*/

		/* store the entry being modified in a temporary structure.  */
		memcpy((char *)&orig_oct_entry, (char *)(oct + index),
					       sizeof(Oct)); 

		/* store the new entry in OCT.				     */
		memcpy((char *)(oct + index),(char *)oct_entry, 
					       sizeof(Oct)); 

		/* check for all object classes that rules related to super  */
		/* classes and aux. classes kinds are still satisfied.	     */

		/* change in object class kind.		*/
		if (oct_entry->obj_class_kind != orig_oct_entry.obj_class_kind)
			{
			for (i=0; i<no_oct; i++)
				{
				if ((ret_value = d20_check_sclasses((oct + i), 
						no_oct, oct,ret_code)) == D2_ERROR)
					{
					d20_errcode(ret_code,CALLERR,
						PARERR, DS_SCLASS_CONS_VIOL);
					break;
					}
				
				if (orig_oct_entry.obj_class_kind == 
								D2_AUXILIARY)
					{
					for (j=0;j<(oct + i)->no_aux_class;j++)
						if (strcmp(
						(oct + i)->aux_class[j], 
						oct_entry->obj_abbrv) == 0)
							break;

					if (j != (oct + i)->no_aux_class)
						{
						d20_errcode(ret_code,CALLERR,
						PARERR, DS_AUX_CONS_VIOL);
						ret_value = D2_ERROR;
						break; 
						}
					}

				}
			}

		if (ret_value == D2_ERROR)
			{
			/* restore original entry in OCT.		*/
			memcpy((char *)(oct + index),(char *)&orig_oct_entry, 
						       sizeof(Oct)); 

			break;	/* break from case MOD_OCT_ENTRY.	*/	
			}

		/* check for all str. rules, whether all naming attributes   */
		/* belong to object class referenced in the rule.     	     */
		for (i=0; i<no_srt; i++)
			{
			for (j=0; j<(srt + i)->no_nam_attr; j++)
				if ((ret_value = d20_035_search_na_oclass(
							(srt + i)->nam_attr[j], 
							(srt + i)->obj_class,
							no_oct, oct)) == D2_ERROR)
					{
					d20_errcode(ret_code,CALLERR,PARERR,
							DS_OPT_MAND_NAME_ATTR);
					break; /* break from inner for loop. */	
					}
		
			if (j != (srt + i)->no_nam_attr)
				break;	/* break from main for loop.	*/
			}

		/* restore original entry in OCT.		*/
		memcpy((char *)(oct + index),(char *)&orig_oct_entry, 
					       sizeof(Oct)); 

		/* check if obj_class_kind has been changed from STRUCTURAL */
		/* and class is being used in a structure rule.		    */

		/* change in object class kind.		*/
		if (oct_entry->obj_class_kind != orig_oct_entry.obj_class_kind)
			if (orig_oct_entry.obj_class_kind == D2_STRUCTURAL)
				{
				for (i=0; i<no_srt; i++)
					if (strcmp(oct_entry->obj_abbrv,
						(srt + i)->obj_class) == 0)
						break;

				/* some srule uses this object class.	*/
				if ( i < no_srt)
					{
					d20_errcode(ret_code,CALLERR,PARERR,
							DS_SRULE_CONS_VIOL);
					ret_value = D2_ERROR;
					break;
					}
				}
				

		break;	/* break from case MOD_OCT_ENTRY.	*/	
		}

	case	DEL_OCT_ENTRY:
		{
		char	 *abbrv = upd_info->upd_val.d_oct->oct_abbrv;
		signed32  no_oct = upd_info->upd_val.d_oct->no_oct;
		Oct	 *oct = upd_info->upd_val.d_oct->oct;
		signed32  no_srt = upd_info->upd_val.d_oct->no_srt;
		Srt	 *srt = upd_info->upd_val.d_oct->srt;
		signed32  i,j;

		/* entry should not belong to default schema.	*/
		if (d20_024_search_obj_abbrv(abbrv, NO_DEF_OCT,(char *) def_oct,
							D2_DEF_SCHEMA) != -1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_DEL_DEF_SCH);
			ret_value = D2_ERROR;
			break; /* break from case DEL_OCT_ENTRY	*/
			}

		/* entry should exist in Oct and should not be in use 	*/
		/* in Srt or as superior object class of some object class */
		/* in Oct.						*/

		if (d20_024_search_obj_abbrv(abbrv, no_oct, (char *) oct, 
							D2_TOTAL_SCHEMA) == -1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_ENT_NFOUND);
			ret_value = D2_ERROR;
			break; /* break from case DEL_OCT_ENTRY.	*/
			}

		/* check if object class is sup. obj. class of some */
		/* object class.				    */
		for (i=0; i<no_oct; i++, oct++)
			{
			for (j=0; j<oct->no_sup_class; j++)
				if (strcmp(abbrv,oct->sup_class[j]) == 0)
					break; /* break from for loop.	*/

			if (j != oct->no_sup_class)
				break;
			}
		
		if (i != no_oct)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_ORPHAN);
			ret_value = D2_ERROR;
			break; /* break from case DEL_OCT_ENTRY.	*/
			}

		/* check if obj. class is used in Srt	*/
		for (i=0; i<no_srt; i++, srt++)
			if (strcmp(abbrv, srt->obj_class) == 0)
				break; /* break from for loop.	*/

		if (i != no_srt)
			{
			d20_errcode(ret_code, CALLERR, PARERR, DS_OCT_IN_USE);
			ret_value = D2_ERROR;
			break; /* break from for loop.		*/
			}

		break; /* break from case DEL_OCT_ENTRY.	*/
		}
	}

return(ret_value);

}		/* end of d20_020_check_obj()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_obj_integrity()                             */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_check_obj_integrity(ret_code, oct_entry) */
/*		Errcode *ret_code;				      */
/*		Oct	*oct_entry;				      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks for the correctness of various   */
/*		fields of an Oct entry.				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Oct	*oct_entry;				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode	*ret_code;	pointer to errocde	      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Some error in Oct entry			      */
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

 static	signed32 d20_check_obj_integrity(
   Errcode *ret_code,
   Oct     *oct_entry)

{		/* begin d20_check_obj_integrity()	*/
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

/* check object class abbreviation.		*/
if (d20_015_check_string(oct_entry->obj_abbrv) == D2_ERROR)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_ABBRV);
	return(D2_ERROR);
	}

/* check object class name			*/
if (d20_015_check_string(oct_entry->obj_name) == D2_ERROR)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_NAME);
	return(D2_ERROR);
	}

/* check object class object identifier	*/
if (strcmp(oct_entry->obj_abbrv, S_OC_GTP) != 0 || strcmp(oct_entry->obj_id, 
							  S_OC_GTP_OBJ_ID) != 0)
	if (d20_016_check_obj_id(oct_entry->obj_id) == D2_ERROR)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_WR_OBJ_ID_FORMAT);
		return(D2_ERROR);
		}

/* check object class kind.	*/
if (oct_entry->obj_class_kind != D2_ABSTRACT && 
	oct_entry->obj_class_kind != D2_ALIAS && 
	oct_entry->obj_class_kind != D2_AUXILIARY && 
	oct_entry->obj_class_kind != D2_STRUCTURAL )
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_INVALID_OC_KIND);
	return(D2_ERROR);
	}

/* check object class file no. 		*/
if ((oct_entry->obj_file_nr == 0) || 
    (oct_entry->obj_class_kind == D2_STRUCTURAL && oct_entry->obj_file_nr < 0))
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_FILENO);
	return(D2_ERROR);
	}

return(NOERROR);

}		/* end of d20_check_obj_integrity()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_oclasses_attrs()                            */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_check_oclasses_attrs(oct_entry,          */
/*					no_oct,oct,no_at,at,ret_code) */
/*		Oct	 *oct_entry;				      */
/*		signed32  no_oct;				      */
/*		Oct	 *oct;					      */
/*		signed32  no_at;				      */
/*		At	 *at;					      */
/*		Errcode  *ret_code;				      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function does various checks on object classes   */
/*		involved (super and auxiliary) and on attributes      */
/*		involved (mandatory and optional).		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Oct	  *oct_entry;				      */
/*		signed32   no_oct;				      */
/*		Oct	  *oct;					      */
/*		signed32   no_at;				      */
/*		At	  *at;					      */
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

 static	signed32 d20_check_oclasses_attrs(
   Oct	    *oct_entry,
   signed32  no_oct,
   Oct	    *oct,
   signed32  no_at,
   At	    *at,
   Errcode  *ret_code)

{		/* begin d20_check_oclasses_attrs()	*/
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
	signed32	i,j,index;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/* check for super classes.	*/

/* every object class (except TOP) should have at least one super class.*/
if (oct_entry->no_sup_class == 0 && 
    strcmp(oct_entry->obj_abbrv, S_OC_TOP) != 0)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_NO_VALUE);
	return(D2_ERROR);
	}

/* check if there is any repetition of super classes.	*/
for (i=0; i<oct_entry->no_sup_class; i++)
	for (j=i+1; j<oct_entry->no_sup_class; j++)
		if (strcmp(oct_entry->sup_class[i],oct_entry->sup_class[j])==0)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_REPETITION);
			return(D2_ERROR);
			}

/* check super classes (there existence and validity of their kind.)	*/
if (d20_check_sclasses(oct_entry, no_oct, oct, ret_code) == D2_ERROR)
	return(D2_ERROR);

/**********************************************************************/

/**********************************************************************/
/* check for auxiliary object classes.		*/

/* Only a structural object class can have aux. object classes.		 */
if (oct_entry->obj_class_kind != D2_STRUCTURAL && oct_entry->no_aux_class != 0)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_ONLY_STR_AUX);
	return(D2_ERROR);
	}

/* check for repetition in list of auxiliary object classes.		 */
for (i=0; i<oct_entry->no_aux_class; i++)
	for (j=i+1; j<oct_entry->no_aux_class; j++)
		if (strcmp(oct_entry->aux_class[i],oct_entry->aux_class[j])==0)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_REPETITION);
			return(D2_ERROR);
			}


/* check if all aux. object classes exists as aux. object classes in OCT */

for (i=0; i<oct_entry->no_aux_class; i++)
	{
	if ((index = d20_024_search_obj_abbrv(oct_entry->aux_class[i], no_oct, 
					 (char *) oct, D2_TOTAL_SCHEMA)) == -1)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_OBJCLS_NFOUND);
		return(D2_ERROR);
		}

	/* check for kind of auxiliary classes.		*/

	if ((oct + index)->obj_class_kind != D2_AUXILIARY)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_AUX_AUX);
		return(D2_ERROR);
		}
	}

/**********************************************************************/

/* check if at least one attribute is there.		*/
if ((oct_entry->no_must_attr == 0) && (oct_entry->no_may_attr == 0))
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_NO_VALUE);
	return(D2_ERROR);
	}

/* check if there is any repetition of attributes in mandatory attribute list */
for (i=0; i<oct_entry->no_must_attr; i++)
	for (j=i+1; j<oct_entry->no_must_attr; j++)
		if (strcmp(oct_entry->must_attr[i],oct_entry->must_attr[j])==0)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_REPETITION);
			return(D2_ERROR);
			}

/* check if there is any repetition of attributes in optional  attribute list */
for (i=0; i<oct_entry->no_may_attr; i++)
	for (j=i+1; j<oct_entry->no_may_attr; j++)
		if (strcmp(oct_entry->may_attr[i],oct_entry->may_attr[j])==0)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_REPETITION);
			return(D2_ERROR);
			}

/* check if all mandatory attributes are defined in attribute table.	*/
for (i=0; i<oct_entry->no_must_attr; i++)
	if (d20_014_search_at_abbrv(oct_entry->must_attr[i], no_at, (char *) at,
							D2_TOTAL_SCHEMA) == -1)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_MAND_ATTR_NFOUND);
		return(D2_ERROR);
		}

/* check if all optional attributes are defined in attribute table.	*/
for (i=0; i<oct_entry->no_may_attr; i++)
	if (d20_014_search_at_abbrv(oct_entry->may_attr[i], no_at, (char *) at,
							D2_TOTAL_SCHEMA) == -1)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_OPT_ATTR_NFOUND);
		return(D2_ERROR);
		}

/* check if some attribute is not both optional and mandatory.		*/
for (i=0; i<oct_entry->no_must_attr; i++)
	for (j=0; j<oct_entry->no_may_attr; j++)
		if (strcmp(oct_entry->may_attr[j], oct_entry->must_attr[i])
								== 0)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_OPT_AND_MAND);
			return(D2_ERROR);
			}

/* check for mandatory attributes, any mandatory attribute should not   */
/* appear as mandatory attribute in any of the superior object classes. */
/* It can occur as optional attribute in any of the sup. obj. classes.  */

/* check for optional attributes, any optional attribute should not     */
/* appear as mandatory or optional attribute in any of the superior     */
/* object classes. 						        */

if (d20_check_attrs_with_sclass(oct_entry, oct_entry, no_oct, oct, ret_code)
						== D2_ERROR)
	return(D2_ERROR);

return(D2_NOERROR);

}		/* end of d20_check_oclasses_attrs()		*/
/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_024_search_obj_abbrv()                            */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_024_search_obj_abbrv(abbrv,no_oct,oct,   */
/*				schema)		 		      */
/*		char	 *abbrv;	object class abbreviation.    */
/*		signed32  no_oct;	no. entries in OCT.   	      */
/*		char	 *oct;		pointer to OCT.       	      */
/*		signed32  schema;	type of schema in which       */
/*					abbrv. is to be searched.     */
/*				(D2_TOTAL_SCHEMA/D2_DEF_SCHEMA)	      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function searches for object class abbrv. in OCT.*/
/*		If found, it returns the index in OCT otherwise it    */
/*		returns -1.					      */
/*		Which object class table is to be searched depends on */
/*		schema flag.				    	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	 *abbrv;	object class abbreviation.    */
/*		signed32  no_oct;	no. entries in OCT.   	      */
/*		char	 *oct;		pointer to OCT.       	      */
/*		signed32  schema;	type of schema in which       */
/*					abbrv. is to be searched.     */
/*				(D2_TOTAL_SCHEMA/D2_DEF_SCHEMA)	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		returns index in Oct if abbrv. is found in Oct, else  */
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

 signed32 d20_024_search_obj_abbrv(
   char	    *abbrv,
   signed32  no_oct,
   char	    *oct,
   signed32  schema)

{		/* begin d20_024_search_obj_abbrv()	*/
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
	signed16	found = FALSE;
	signed32	offset;
	signed32	no_entries;
	signed32	index;
 	char	*next_abbrv;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

no_entries = no_oct;
offset = 0;

while (found == FALSE)
	{
	if (no_entries == 0)
		break;

	index = offset + (no_entries/2);

	if (schema == D2_TOTAL_SCHEMA)
		next_abbrv = ((Oct *)((Oct *)oct + index))->obj_abbrv;
	else
		next_abbrv = ((Def_oct *)((Def_oct *)oct + index))->obj_abbrv;

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

}		/* end d20_024_search_obj_abbrv()	*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_021_add_obj()                                     */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_021_add_obj(ret_code, oct_entry, no_oct, */
/*					oct)                          */
/*		Errcode	 *ret_code;	pointer to errocde	      */
/*		Oct	 *oct_entry; 	pointer to oct entry to be    */
/*					added			      */
/*		signed32 *no_oct;	pointer to no. entries in     */
/*					object class table.	      */
/*		Oct	**oct;		pointer to pointer to obj.    */
/*					class table.		      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function adds a new object class in OCT.	      */
/*		New object class is added, keeping the obj. clssses   */
/*		sorted on obj. class abbreviation. If the new object  */
/*		class can not be inserted in the current OCT, because */
/*		of OCT being full, space is reallocated for OCT.      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Oct	 *oct_entry; 	pointer to Oct entry to be    */
/*					added			      */
/*		signed32 *no_oct;	pointer to no. entries in     */
/*					object class table.	      */
/*		Oct	**oct;		pointer to pointer to OCT.    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode	 *ret_code;	pointer to errocde	      */
/*		signed32 *no_oct;	pointer to no. entries in     */
/*					object class table.	      */
/*		Oct	**oct;		pointer to pointer to OCT .   */
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

 signed32 d20_021_add_obj(
   Errcode   *ret_code,
   Oct       *oct_entry,
   signed32  *no_oct,
   Oct      **oct)

{		/* begin d20_021_add_obj()	*/
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
	signed32	i,j;
	Oct	*tmp_ent;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* check if new object class can be inserted in current OCT */
if (*no_oct == *(signed32 *)(*oct + *no_oct)) 
	{/* there is no space in current OCT */
	/* reallocate space for OCT */
  	if ((mem = realloc(*oct, ((*no_oct + D2_O_MORE) * sizeof(Oct)) + 
				  sizeof(signed32))) == (char *)NULL) 
		{
  		d20_errcode(ret_code, TEMPERR, SYSERR, DS_ALLOC_ERROR);
		return(D2_ERROR);
		}

	*oct = (Oct *)mem;

	/* store new object class no. limit */
	*(signed32 *)(*oct + *no_oct) += D2_O_MORE;
	}

/* at this point enough space is there in OCT to store a new object class. */

/* find the position where new object class is to be inserted.	*/
for (j = 0; j < *no_oct; j++)
	if (strcmp(oct_entry->obj_abbrv,(*oct + j)->obj_abbrv) < 0)
		break;

/* store the object class no. limit at new position (one Oct structure ahead).*/
*(signed32 *)(*oct + *no_oct + 1) = *(signed32 *)(*oct + *no_oct);

/* shift the OCT from the point where new object class is to be   */
/* inserted, by sizeof(Oct) towards higher memory address.	  */

/* tmp_ent points to last free entry.		*/
tmp_ent = *oct + *no_oct;

for (i = 0; i < *no_oct - j; i++, tmp_ent--)
	memcpy((char *)tmp_ent, (char *)(tmp_ent - 1), sizeof(Oct));

/* new object class is inserted in the proper slot in the OCT */
memcpy((char *)(*oct + j), (char *)oct_entry, sizeof(Oct));

(*no_oct)++;

return(D2_NOERROR);

}		/* end of d20_021_add_obj()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_022_del_obj()                                     */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_022_del_obj(abbrv,no_oct,oct)     	      */
/*		char	 *abbrv;	abbreviation of the obj. class*/
/*					to be deleted.		      */
/*		signed32 *no_oct;	pointer to no. entries in     */
/*					object class.	      	      */
/*		Oct	 *oct;		pointer to object class table.*/
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function deletes an object class from the object */
/*		class table. Entries ahead of the object class are    */
/*		shifted towards lower address by one Oct entry.New no.*/
/*	        of entries in obj. class table is returned as a result*/
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	 *abbrv;	abbreviation of the obj. class*/
/*					to be deleted.		      */
/*		signed32 *no_oct;	pointer to no. entries in     */
/*					object class table.	      */
/*		Oct	 *oct;		pointer to object class table.*/
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *no_oct;	pointer to no. entries in     */
/*					object class table.	      */
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

 signed32 d20_022_del_obj(
   char     *abbrv,
   signed32 *no_oct,
   Oct      *oct)

{		/* begin d20_022_del_obj()	*/
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

/* search object class in OCT.	*/
j = d20_024_search_obj_abbrv(abbrv, *no_oct, (char *) oct, D2_TOTAL_SCHEMA);

/* shift the entries,ahead of the entry to be deleted, by sizeof(Oct) towards */
/* lower memory address.		   				      */
memcpy((char *)(oct + j),(char *)(oct + j + 1),sizeof(Oct) * (*no_oct - j - 1));

/* store the capacity of object class table at the proper place.	*/
*(signed32 *)(oct + *no_oct - 1) = *(signed32 *)(oct + *no_oct);

/* reduce the no. of entries in object class table.	*/
(*no_oct)--;

return(D2_NOERROR);

}		/* end of d20_022_del_obj()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_023_mod_obj()                                     */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_023_mod_obj(oct_entry,no_oct,oct)        */
/*		Oct	 *oct_entry;	pointer to modified Oct entry.*/
/*		signed32  no_oct;	no. entries in obj. class tab.*/
/*		Oct	 *oct;		pointer to obj. class table.  */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function stores an object class entry in object  */
/*		class table after it has been modified.		      */ 
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Oct	 *oct_entry;	pointer to modified OCT entry.*/
/*		signed32  no_oct;	no. entries in obj. class. tab*/
/*		Oct	 *oct;		pointer to OCT.	      	      */
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

 signed32 d20_023_mod_obj(
   Oct	    *oct_entry,
   signed32  no_oct,
   Oct	    *oct)

{		/* begin d20_023_mod_obj()	*/
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

/* search object class in OCT.	*/
j = d20_024_search_obj_abbrv(oct_entry->obj_abbrv,no_oct,(char *) oct,D2_TOTAL_SCHEMA);

/* copy oct_entry into OCT.	*/
memcpy((char *)(oct + j),(char *)oct_entry, sizeof(Oct));

return(D2_NOERROR);

}		/* end of d20_023_mod_obj()		*/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_sclass_loop()                               */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:	signed32 d20_check_sclass_loop(obj_abbrv, oct_entry,  */
/*					  no_oct, oct)		      */
/*		char	 *obj_abbrv;				      */
/*		Oct	 *oct_entry;				      */
/*		signed32  no_oct;			      	      */
/*		Oct	 *oct;				      	      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks if there is a loop in super class*/
/*		hierarchy.					      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		char	 *obj_abbrv;    obj. class abbrv. to be       */
/*					checked	if exists in super    */
/*					classes of oct_entry.	      */
/*		Oct	 *oct_entry;				      */
/*		signed32  no_oct;	no. entries in obj. class. tab*/
/*		Oct	 *oct;		pointer to OCT.	      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		D2_ERROR:	there is a loop.		      */
/*		D2_NOERROR:	All is O.K.			      */
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

 static signed32 d20_check_sclass_loop(
   char     *obj_abbrv,
   Oct      *oct_entry,
   signed32  no_oct,
   Oct      *oct)


{		/* begin d20_check_sclass_loop()	*/
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

signed32	i, index;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* check if obj_abbrv occurs in super classes of oct_entry.	*/
for (i=0; i<oct_entry->no_sup_class; i++)
	if (strcmp(oct_entry->sup_class[i], obj_abbrv) == 0)
		/* loop found.	*/
		return(D2_ERROR);

/* obj_abbrv does not occur in super classes of oct_entry.	*/
/* check in super classes of super classes.			*/
for (i=0; i<oct_entry->no_sup_class; i++)
	{
	/* assuming all super classes exist in OCT.	*/
	index = d20_024_search_obj_abbrv(oct_entry->sup_class[i], no_oct,
			(char *) oct, D2_TOTAL_SCHEMA);

	if (d20_check_sclass_loop(obj_abbrv, (oct + index), no_oct, oct) == 
								D2_ERROR)
		return(D2_ERROR);
	}

return(D2_NOERROR);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_attrs_with_sclass()                         */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:	signed32 d20_check_attrs_with_sclass(orig_oct_entry,  */
/*			oct_entry, no_oct, oct, ret_code)	      */
/*		Oct	 *orig_oct_entry, *oct_entry, *oct;	      */
/*		signed32  no_oct;      				      */
/*		Errcode  *ret_code;	      		              */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks attributes of orig_oct_entry     */
/*		against attributes of its super class.		      */
/*		Mand. attrs. of orig_oct_entry may occur as optional  */
/*		attributes in super classes but optional attrs. of    */
/*		orig_oct_entry should not appear in super classes.    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Oct	 *orig_oct_entry;			      */
/*		Oct	 *oct_entry;				      */
/*		signed32  no_oct;	no. entries in obj. class. tab*/
/*		Oct	 *oct;		pointer to OCT.	      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode		*ret_code;	      		      */
/*								      */
/* RETURNVALUE:							      */
/*		D2_ERROR:	check against attrs. of super classes */
/*				fails.				      */
/*		D2_NOERROR:	All is O.K.			      */
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

static  signed32 d20_check_attrs_with_sclass(
   Oct      *orig_oct_entry,
   Oct      *oct_entry,
   signed32  no_oct,
   Oct      *oct,
   Errcode  *ret_code)

{		/* begin d20_check_attrs_with_sclass()	*/
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

 signed32	i,j,k,index;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/
 
for (k=0; k<oct_entry->no_sup_class; k++)
	{
	if ((index = d20_024_search_obj_abbrv(oct_entry->sup_class[k], 
				no_oct, (char *) oct, D2_TOTAL_SCHEMA)) == -1)
		break;

	/* check for mandatory attributes.	*/
	for (i=0; i<orig_oct_entry->no_must_attr; i++)
		{
		/* compare with mandatory attributes of superior object */
		/* class.						*/
		for(j=0; j<(oct + index)->no_must_attr; j++)
			if (strcmp(orig_oct_entry->must_attr[i], 
				   (oct + index)->must_attr[j]) == 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
								DS_MAND_SUP);
				return(D2_ERROR);
				}
		}
		
	/* check for optional attributes.	*/
	for (i=0; i<orig_oct_entry->no_may_attr; i++)
		{
		/* compare with mandatory attributes of superior object */
		/* class.						*/
		for(j=0; j<(oct + index)->no_must_attr; j++)
			if (strcmp(orig_oct_entry->may_attr[i], 
				   (oct + index)->must_attr[j]) == 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
								DS_OPT_SUP);
				return(D2_ERROR);
				}

		/* compare with optional attributes of superior object  */
		/* class.						*/
		for(j=0; j<(oct + index)->no_may_attr; j++)
			if (strcmp(orig_oct_entry->may_attr[i], 
				   (oct + index)->may_attr[j]) == 0)
				{
				d20_errcode(ret_code,CALLERR, PARERR, 
								DS_OPT_SUP);
				return(D2_ERROR);
				}
		}

	if (d20_check_attrs_with_sclass(orig_oct_entry, (oct + index), no_oct, 
					oct, ret_code) == D2_ERROR)
		return(D2_ERROR);
	}


return(D2_NOERROR);

}


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_sclasses()                         	      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:	signed32 d20_check_sclasses(oct_entry, no_oct, oct,   */
/*					   ret_code)		      */
/*		Oct	 *oct_entry, *oct;			      */
/*		signed32  no_oct;		      		      */
/*		Errcode	 *ret_code;			      	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks the validity of kinds of super   */
/*		classes for an oct_entry.			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Oct	 *oct_entry;				      */
/*		signed32  no_oct;	no. entries in obj. class. tab*/
/*		Oct	 *oct;		pointer to OCT.	      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode		*ret_code;	      		      */
/*								      */
/* RETURNVALUE:							      */
/*		D2_ERROR:	invalid kind of super classes.	      */
/*		D2_NOERROR:	All is O.K.			      */
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

 static signed32 d20_check_sclasses(
   Oct	    *oct_entry, 
   signed32  no_oct,
   Oct      *oct,
   Errcode  *ret_code)


{		/* begin d20_check_sclasses()	*/
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

	signed32 	i, index, no_str_sclass;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/
 
no_str_sclass = 0;
for (i=0; i<oct_entry->no_sup_class; i++)
	{
	/* look for super class in OCT.		*/
	if ((index = d20_024_search_obj_abbrv(oct_entry->sup_class[i], no_oct, 
					(char *) oct, D2_TOTAL_SCHEMA)) == -1)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_OBJCLS_NFOUND);
		return(D2_ERROR);
		}

	/* check for kinds of super classes.		*/

	/* ALIAS kind can not be super class.	*/
	if ((oct + index)->obj_class_kind == D2_ALIAS)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_WR_KIND_SCLASS);
		return(D2_ERROR);
		}

	/* ABSTRACT object class can have only ABSTRACT super classes.	*/
	if (oct_entry->obj_class_kind == D2_ABSTRACT && 
	    (oct + index)->obj_class_kind != D2_ABSTRACT)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_WR_KIND_SCLASS);
		return(D2_ERROR);
		}

	/* AUXILIARY object class can have AUXILIARY or ABSTRACT sup. classes */
	if (oct_entry->obj_class_kind == D2_AUXILIARY && 
	    (oct + index)->obj_class_kind == D2_STRUCTURAL)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_WR_KIND_SCLASS);
		return(D2_ERROR);
		}

	/* STRUCTURAL object class can have at most 1 STR. object class and */
	/* any no. of ABSTRACT. object classes as imediate super classes.   */
	if (oct_entry->obj_class_kind == D2_STRUCTURAL) 
		{
	    	if ((oct + index)->obj_class_kind == D2_AUXILIARY)
			{
			d20_errcode(ret_code,CALLERR, PARERR, 
							DS_WR_KIND_SCLASS);
			return(D2_ERROR);
			}

	    	if ((oct + index)->obj_class_kind == D2_STRUCTURAL)
			no_str_sclass++;

		if (no_str_sclass > 1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, 
							DS_TOO_MANY_STR_SC);
			return(D2_ERROR);
			}
		}
	}

/* for an object class of STRUCTURAL kind, check if GTP is one of the super */
/* class.								    */
if (oct_entry->obj_class_kind == D2_STRUCTURAL) 
	if (d20_check_gtp_for_str(oct_entry, no_oct, oct) == D2_ERROR)
		{
		d20_errcode(ret_code, CALLERR, PARERR, DS_GTP_MISSING);
		return(D2_ERROR);
		}

return(D2_NOERROR);

}


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_gtp_for_str()                        	      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:	signed32 d20_check_gtp_for_str(oct_entry, no_oct, oct)*/
/*		Oct	 *oct_entry, *oct;			      */
/*		signed32  no_oct;			      	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks if object class oct_entry has    */
/*		S_OC_GTP as its one of the superior object classes    */
/*		or not.						      */
/*		This function when called first time, oct_entry       */
/*		should have object class kind STRUCTURAL.	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Oct	 *oct_entry;				      */
/*		signed32  no_oct;	no. entries in obj. class. tab*/
/*		Oct	 *oct;		pointer to OCT.	      	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		D2_ERROR:	oct_entry does not have S_OC_GTP      */
/*				as super class.			      */
/*		D2_NOERROR:	All is O.K.			      */
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

 static signed32 d20_check_gtp_for_str(
   Oct      *oct_entry,
   signed32  no_oct,
   Oct      *oct)

{		/* begin d20_check_gtp_for_str()	*/
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

	signed32 	i, index;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (oct_entry->no_sup_class == 0)
	return(D2_ERROR);

for (i=0; i<oct_entry->no_sup_class; i++)
	if (strcmp(oct_entry->sup_class[i], S_OC_GTP) != 0)
		{
		index = d20_024_search_obj_abbrv(oct_entry->sup_class[i], 
					no_oct, (char *) oct, D2_TOTAL_SCHEMA);

		if ((oct + index)->obj_class_kind == D2_ABSTRACT)
			{
			if (d20_check_gtp_for_str((oct + index),no_oct,oct) ==
								D2_NOERROR)
				return(D2_NOERROR);
			}
		else
			/* object class kind is structural.	*/
			if (oct_entry->no_sup_class == 1)
				/* the only super class is of kind STRUCTURAL.*/
				return(D2_NOERROR);
		}
	else
		return(D2_NOERROR);

return(D2_ERROR);

}
