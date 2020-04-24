/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20hdlsrt.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:23  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:39  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:29  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:04:14  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:19:29  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:09:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:15:49  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:47  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:54:15  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:01:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:40:21  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:32  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20hdlsrt.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:23 $";
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
/* NAME         : d20hdlsrt.o 					      */
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

#define	ROOT			0	/* rule no. of root.		*/

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static signed32 d20_check_srule_integrity(Errcode *ret_code, Srt *srt_entry);
static signed32 d20_check_attrs_oclass(Errcode *ret_code, S_upd_info *upd_info);
static signed32 d20_check_too_many(Srt *srt_entry, signed32 no_srt, Srt *srt,
			signed32 np_max);
static signed32 d20_find_degree(signed32 rule_nr, signed32 no_srt, Srt *srt);
static signed32 d20_srule_cmp(Srt *srt_ent1, Srt *srt_ent2);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_030_check_srule()                                 */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_030_check_srule(ret_code,upd_info)       */
/*		Errcode		*ret_code;			      */
/*		S_upd_info	*upd_info;			      */	
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function does validation checks for structure    */
/*		rule table modification. Validation checks are de-    */
/*		pendent on the kind of modification (Add a new rule/  */
/*		modify an existing rule/delete a rule)	      	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		S_upd_info	*upd_info;			      */	
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode	*ret_code;	pointer to errocde	      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred				      */
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

 signed32 d20_030_check_srule(
   Errcode    *ret_code,
   S_upd_info *upd_info)

{		/* begin d20_030_check_srule()	*/
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
	signed32        ret_value = D2_NOERROR;
	signed32 	parent_flag;
/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

switch ((int) upd_info->upd_mode)
	{
	case	ADD_SRT_ENTRY:
		{
		Srt	 *srt_entry = upd_info->upd_val.a_m_srt->srt_entry;
		signed32  no_srt = upd_info->upd_val.a_m_srt->no_srt;
		Srt	 *srt = upd_info->upd_val.a_m_srt->srt;
		signed32  j;

		if ((ret_value = d20_check_srule_integrity(ret_code, 
							srt_entry)) == D2_ERROR)
			break; /* break from case ADD_SRT_ENTRY	*/

		/* check consistency */

		/* check if some Srt entry with same rule no. exists */
		/* check if parent rule exists or not.		     */
		/* check if two rules are same except their rule no. */

		parent_flag = FALSE;

		/* if rule is added under root, then the existence of parent */
		/* is implied.						     */
		if (srt_entry->par_rule_nr == ROOT)
			parent_flag = TRUE;

		for (j=0; j<no_srt; j++)
			{
			/* check for uniqueness of rule number    */
			if (srt_entry->rule_nr == (srt + j)->rule_nr)
				{
				d20_errcode(ret_code,CALLERR,PARERR,DS_EXISTS);
				ret_value = D2_ERROR;
				break; /* break from for loop	*/
				}

			/* check if two rules are same 	*/
			if (d20_srule_cmp(srt_entry, (srt + j)) == 0)
				{
				/* two rules are same.		*/
				d20_errcode(ret_code,CALLERR,PARERR,
							DS_SIMILAR_RULE);
				ret_value = D2_ERROR;
				break; /* break from for loop	*/
				}

			/* check if parent rule number exists.    */
			if (parent_flag == FALSE)
				if (srt_entry->par_rule_nr == 
						(srt + j)->rule_nr)
					parent_flag = TRUE;
			}

		if (ret_value == D2_ERROR)
			break; /* break from case ADD_SRT_ENTRY	*/
			
		if (parent_flag == FALSE)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_SUP_NFOUND);
			ret_value = D2_ERROR;
			break; /* break from case ADD_SRT_ENTRY	*/
			}	

		/* check naming attribute and object classes. */
		if ((ret_value = d20_check_attrs_oclass(ret_code, upd_info)) 
								== D2_ERROR)
			break; /* break from case ADD_SRT_ENTRY	*/

		/* check if new structure rule will lead to a situation */
		/* when too many RDNs generate a DN.			*/
		if ((ret_value = d20_check_too_many(srt_entry, no_srt, srt, 
							D2_NP_MAX)) == D2_ERROR)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_TOO_MANY);
			break; /* break from case ADD_SRT_ENTRY	*/
			}

		break; /* break from case ADD_SRT_ENTRY	*/
		}

	case	MOD_SRT_ENTRY:
		{
		Srt	 *srt_entry = upd_info->upd_val.a_m_srt->srt_entry;
		signed32  no_srt = upd_info->upd_val.a_m_srt->no_srt;
		Srt	 *srt = upd_info->upd_val.a_m_srt->srt;
		signed32  j, index, degree;

		/* entry should exist in Srt 			*/
		if ((index = d20_034_search_srule(srt_entry->rule_nr, 
							no_srt, srt)) == -1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_ENT_NFOUND);
			ret_value = D2_ERROR;
			break; /* break from case MOD_SRT_ENTRY	*/
			}

		/* check integrity of entry.	*/

		if ((ret_value = d20_check_srule_integrity(ret_code,srt_entry)) 
								== D2_ERROR)
			break; /* break from case MOD_SRT_ENTRY	*/

		/* check consistency */

		/* entry should not belong to default schema.	*/
		if ((srt_entry->rule_nr >= DS_MIN_DEF_RULE_NR) &&
				(srt_entry->rule_nr <= DS_MAX_DEF_RULE_NR))
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_MOD_DEF_SRT);
			ret_value = D2_ERROR;
			break; /* break from case MOD_SRT_ENTRY	*/
			}

		/* check if parent rule exists.  */
		/* check if too many rules generate same structure.	*/
		/* both these checks are done for all rules except the  */
		/* one being modified.					*/

		parent_flag = FALSE;

		/* if rule is added under root, then the existence of parent */
		/* is implied.						     */
		if (srt_entry->par_rule_nr == ROOT)
			parent_flag = TRUE;

		for (j=0; j<no_srt; j++)
			{
			if (j != index)
				{
				/* check if two rules are same 	*/
				if (d20_srule_cmp(srt_entry, (srt + j)) == 0)
					{
					/* two rules are same.		*/
					d20_errcode(ret_code,CALLERR,PARERR,
							DS_SIMILAR_RULE);
					ret_value = D2_ERROR;
					break; /* break from for loop	*/
					}

				if (parent_flag == FALSE)
					if (srt_entry->par_rule_nr == 
							(srt + j)->rule_nr)
						parent_flag = TRUE;
				}
			}

		if (ret_value == D2_ERROR)
			break; /* break from case ADD_SRT_ENTRY	*/
			
		if (parent_flag == FALSE)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_SUP_NFOUND);
			ret_value = D2_ERROR;
			break; /* break from case ADD_SRT_ENTRY	*/
			}	

		/* check naming attribute and object classes. 	      */
		if ((ret_value = d20_check_attrs_oclass(ret_code, upd_info)) 
								== D2_ERROR)
			break; /* break from case MOD_SRT_ENTRY	*/

		/* check if new structure rule will lead to a situation */
		/* when too many RDNs generate a DN.			*/

		/* first find the size of the longest branch of the SRT */
		/* tree with root=srt_entry->rule_nr.			*/
		degree = d20_find_degree(srt_entry->rule_nr, no_srt, srt);

		if ((ret_value = d20_check_too_many(srt_entry, no_srt, srt, 
						D2_NP_MAX - degree)) == D2_ERROR)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_TOO_MANY);
			break; /* break from case MOD_SRT_ENTRY	*/
			}

		break; /* break from case MOD_SRT_ENTRY	*/
		}

	case	DEL_SRT_ENTRY:
		{
		signed32  rule_nr = upd_info->upd_val.d_srt->rule_nr;
		signed32  no_srt = upd_info->upd_val.d_srt->no_srt;
		Srt	 *srt = upd_info->upd_val.d_srt->srt;
		signed32  i;

		/* entry should not belong to default schema.	*/
		if ((rule_nr >= DS_MIN_DEF_RULE_NR) &&
						(rule_nr <= DS_MAX_DEF_RULE_NR))
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_DEL_DEF_SCH);
			ret_value = D2_ERROR;
			break; /* break from case DEL_SRT_ENTRY	*/
			}

		/* entry should exist in Srt 			*/
		if (d20_034_search_srule(rule_nr, no_srt, srt) == -1)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_ENT_NFOUND);
			ret_value = D2_ERROR;
			break; /* break from case DEL_SRT_ENTRY	*/
			}

		/* check if rule is parent of another rule.	*/
		for (i=0; i<no_srt; i++)
			if (rule_nr == (srt + i)->par_rule_nr)
				{
				d20_errcode(ret_code,CALLERR,PARERR,DS_ORPHAN);
				ret_value = D2_ERROR;
				break; /* break from for loop.	*/
				}

		break; /* break from case DEL_SRT_ENTRY	*/
		}
	}

return(ret_value);

}		/* end of d20_030_check_srule()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_srule_integrity()                           */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_check_srule_integrity(ret_code,srt_entry)*/
/*		Errcode *ret_code;				      */
/*		Srt	*srt_entry;				      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks for the correctness of various   */
/*		fields of an Srt entry.				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Srt	*srt_entry;				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode	*ret_code;	pointer to errocde	      */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       some error in Srt entry			      */
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

 static signed32 d20_check_srule_integrity(
   Errcode *ret_code,
   Srt     *srt_entry)

{		/* begin d20_check_srule_integrity()	*/
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

if (srt_entry->rule_nr <= ROOT)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_WR_RULE_NR);
	return(D2_ERROR);
	}

return(D2_NOERROR);

}		/* end of d20_check_srule_integrity()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_attrs_oclass()                              */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_check_attrs_oclass(ret_code,upd_info)    */
/*		Errcode 	*ret_code;			      */
/*		S_upd_info	*upd_info;			      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function does various validation checks on       */
/*		naming attributes and object class refered in SRT     */
/*		entry.						      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Errcode 	*ret_code;			      */
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

 static signed32 d20_check_attrs_oclass(
   Errcode    *ret_code,
   S_upd_info *upd_info)

{		/* begin d20_check_attrs_oclass()	*/
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
	signed32  i,j,index;
	Srt	 *srt_entry = upd_info->upd_val.a_m_srt->srt_entry;
	signed32  no_at = upd_info->upd_val.a_m_srt->no_at;
	At	 *at = upd_info->upd_val.a_m_srt->at;
	signed32  no_oct = upd_info->upd_val.a_m_srt->no_oct;
	Oct	 *oct = upd_info->upd_val.a_m_srt->oct;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* check if there is at least one naming attribute.		*/
if (srt_entry->no_nam_attr == 0)
	{
	d20_errcode(ret_code,CALLERR, PARERR, DS_NO_VALUE);
	return(D2_ERROR);
	}

/* check if naming attribute abbreviations repeate 		*/
for (i=0; i<srt_entry->no_nam_attr; i++)
	for (j=i+1; j<srt_entry->no_nam_attr; j++)
		if (strcmp(srt_entry->nam_attr[i],srt_entry->nam_attr[j])==0)
			{
			d20_errcode(ret_code,CALLERR, PARERR, DS_REPETITION);
			return(D2_ERROR);
			}

/* for all naming attributes.	*/
for (i=0; i<srt_entry->no_nam_attr; i++)
	{
	/* check if naming attributes exists in AT */
	if ((index = d20_014_search_at_abbrv(srt_entry->nam_attr[i], no_at,
			(char *) at, D2_TOTAL_SCHEMA)) == -1)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_NAME_ATTR_NFOUND);
		return(D2_ERROR);
		}

	/* check if naming attribute has +ve value for no. of recurr. values. */
	if (((At *)(at + index))->att_no_recur == 0)
		{
		d20_errcode(ret_code,CALLERR, PARERR, DS_WR_NAM_RECR);
		return(D2_ERROR);
		}

	/* check representaion of naming attribute.	*/
	switch ((int) d27_401_syntax_to_rep((signed16) 
					((At *)(at + index))->att_syntax))
		{
		case D2_T61_PR:
		case D2_PRINTABLE:
		case D2_NUMERIC:
		case D2_IA5:
			break;
		default:
			d20_errcode(ret_code,CALLERR, PARERR, DS_WR_NAM_SNTX);
			return(D2_ERROR);
		}
	}

/* check if obj. class exists in OCT.	     */
if ((index = d20_024_search_obj_abbrv(srt_entry->obj_class, no_oct,(char *) oct,
						D2_TOTAL_SCHEMA)) == -1)
	{
	d20_errcode( ret_code,CALLERR,PARERR,DS_OBJCLS_NFOUND);
	return(D2_ERROR);
	}

/* check if obj. class exists in OCT.	     */
if ((index = d20_024_search_obj_abbrv(srt_entry->obj_class, no_oct,(char *) oct,
						D2_TOTAL_SCHEMA)) == -1)
	{
	d20_errcode( ret_code,CALLERR,PARERR,DS_OBJCLS_NFOUND);
	return(D2_ERROR);
	}

/* object class should be structural object class.	*/
if (((Oct *)(oct + index))->obj_class_kind != D2_STRUCTURAL)
	{
	d20_errcode( ret_code,CALLERR,PARERR,DS_SRULE_STR);
	return(D2_ERROR);
	}

/* all naming attributes should belong to structural object class.	*/
for (i=0; i<srt_entry->no_nam_attr; i++)
	if (d20_035_search_na_oclass(srt_entry->nam_attr[i], 
				srt_entry->obj_class, no_oct, oct) == D2_ERROR)
		{
		/* naming attribute does not belong to this object class. */	
		d20_errcode( ret_code,CALLERR,PARERR,DS_NAME_ATTR_INVALID);
		return(D2_ERROR);
		}

return(D2_NOERROR);

}		/* end of d20_check_attrs_oclass()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_find_degree()                                     */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_find_degree(rule_nr, no_srt, srt)	      */
/*		signed32  rule_nr;				      */
/*		signed32  no_srt;				      */
/*		Srt	 *srt;					      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function finds the size of the longest branch    */
/*		of the SRT tree with root=rule_nr.		      */
/*								      */
/* INPUT-PARAMETERS :						      */
/*		signed32  rule_nr;				      */
/*		signed32  no_srt;				      */
/*		Srt	 *srt;					      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		size of the longest branch of the SRT tree with       */
/*		root = rule_nr.					      */
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

 static signed32 d20_find_degree(
   signed32  rule_nr,
   signed32  no_srt,
   Srt      *srt)

{		/* begin d20_find_degree()	*/
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
	signed32	i, curr_deg, next_deg;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

curr_deg = 0;

for (i=0; i<no_srt; i++)
	if ((srt + i)->par_rule_nr == rule_nr)	
		{
		next_deg = d20_find_degree((srt + i)->rule_nr, no_srt, srt) + 1;
		if (next_deg > curr_deg)
			curr_deg = next_deg;
		}

return(curr_deg);

}		/* end of d20_find_degree()	*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_too_many()                                  */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_check_too_many(srt_entry, no_srt, srt,   */
/*				 np_max)			      */
/*		Srt	 *srt_entry;				      */
/*		signed32  no_srt;				      */
/*		Srt	 *srt;					      */
/*		signed32  np_max;	maximum no. of naming parts   */
/*					allowed for the new rule      */
/*					(srt_entry)		      */ 
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks if new srt_entry will lead to a  */
/* 		situation when too many RDNs generate a distinguish   */
/*		name (DN).					      */
/*								      */
/* INPUT-PARAMETERS :						      */
/*		Srt      *srt_entry;				      */
/*		signed32  no_srt;				      */
/*		Srt	 *srt;					      */
/*		signed32  np_max;	maximum no. of naming parts   */
/*					allowed for the new rule      */
/*					(srt_entry)		      */ 
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*      D2_NOERROR:     valid srt_entry				      */
/*      D2_ERROR:       invalid srt_entry			      */
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

 static signed32 d20_check_too_many(
   Srt      *srt_entry,
   signed32  no_srt,
   Srt      *srt,
   signed32  np_max)

{		/* begin d20_check_too_many()	*/
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

	signed32	rdn_cnt;
	signed32	rule_nr;
	signed32	index;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

rdn_cnt = 1;
rule_nr = srt_entry->par_rule_nr;

while ((index = d20_034_search_srule(rule_nr,no_srt,srt)) != -1)
	{
	rdn_cnt++;

	if (rdn_cnt > np_max)
		break;

	rule_nr = (srt + index)->par_rule_nr;
	}

if (rdn_cnt > np_max)
	return(D2_ERROR);
else
	return(D2_NOERROR);

}		/* end of d20_check_too_many()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_035_search_na_oclass()                            */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_035_search_na_oclass(nam_abbrv,oclass,   */
/*				no_oct,oct)  			      */
/*	 	Abbrv		nam_abbrv;			      */
/*		Abbrv  		oclass;				      */
/*		signed32	no_oct;				      */
/*		Oct		*oct;				      */	
/*								      */
/* DESCRIPTION:                                                       */
/*		This function checks if the attribute 'nam_abbrv'     */
/*		belongs to object class 'oclass' or its superior      */
/*		object classes. 			      	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*	 	Abbrv		nam_abbrv;			      */
/*		Abbrv  		oclass;				      */
/*		signed32	no_oct;				      */
/*		Oct		*oct;				      */	
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		D2_NOERROR:	if attribute belongs.		      */
/*		D2_ERROR:	it attribute does not belong.	      */
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

 signed32 d20_035_search_na_oclass(
   Abbrv     nam_abbrv,
   Abbrv     oclass,
   signed32  no_oct,			
   Oct      *oct)

{		/* begin d20_035_search_na_oclass()	*/
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
	Oct	*oct_entry;
	signed32	index, j;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

index = d20_024_search_obj_abbrv(oclass, no_oct, (char *) oct, D2_TOTAL_SCHEMA);

oct_entry = oct + index;

/* check if naming attribute is mandatory attribute of the object */
/* class.							  */
for (j=0; j<oct_entry->no_must_attr; j++)
	if (strcmp(oct_entry->must_attr[j],nam_abbrv) == 0)
		break;

if (j<oct_entry->no_must_attr)
	{
	/* naming attribute is mandatory attribute.	*/
	/* attribute belongs to object class.		*/
	return(D2_NOERROR);
	}

/* check if naming attribute is optional attribute of the object  */
/* class.							  */
for (j=0; j<oct_entry->no_may_attr; j++)
	if (strcmp(oct_entry->may_attr[j],nam_abbrv) == 0)
		break;

if (j<oct_entry->no_may_attr)
	{
	/* naming attribute is optional attribute.	*/
	/* attribute belongs to object class.		*/
	return(D2_NOERROR);
	}

/* attribute does not belong to object class, try in superior 	*/
/* object classes.			     			*/
for (j=0; j<oct_entry->no_sup_class; j++)
	/* attribute found in one of the super classes.		*/
	if (d20_035_search_na_oclass(nam_abbrv, oct_entry->sup_class[j], 
				     no_oct, oct) == D2_NOERROR)
		break; 

if (j == oct_entry->no_sup_class)
	return(D2_ERROR);
else
	return(D2_NOERROR);

}		/* end of d20_035_search_na_oclass()	*/
/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_034_search_srule()                                */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_034_search_srule(rule_nr,no_srt,srt)     */
/*		signed32 rule_nr;	structure rule number.        */
/*		signed32 no_srt;	no. entries in SRT.   	      */
/*		Srt	*srt;		pointer to SRT.       	      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function searches for rule_nr. in SRT.	      */
/*		If found, it returns the index in SRT otherwise it    */
/*		returns -1.					      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32 rule_nr;	structure rule number.        */
/*		signed32 no_srt;	no. entries in SRT.   	      */
/*		Srt	*srt;		pointer to SRT.       	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		returns index in Srt if rule_nr. is found in Srt, else*/
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

 signed32 d20_034_search_srule(
   signed32  rule_nr,
   signed32  no_srt,
   Srt      *srt)

{		/* begin d20_034_search_srule()	*/
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
	signed32	offset = 0;
	signed32	no_entries = no_srt;
	signed32	index;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

while (found == FALSE)
	{
	if (no_entries == 0)
		break;

	index = offset + (no_entries/2);

	if (rule_nr == (srt + index)->rule_nr)
		{
		found = TRUE;
	 	break;
		}	
	else
		if (rule_nr < (srt + index)->rule_nr)
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

}		/* end d20_034_search_srule()	*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_031_add_srule()                                   */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_031_add_srule(ret_code,srt_entry,no_srt, */
/*					   srt)			      */
/*		Errcode	 *ret_code;	pointer to errocde	      */
/*		Srt	 *srt_entry; 	pointer to srt entry to be    */
/*					added			      */
/*		signed32 *no_srt;	pointer to no. entries in     */
/*					structure rule table.	      */
/*		Srt	**srt;		pointer to pointer to SRT.    */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function adds a new structure rule in SRT.	      */
/*		New structure rule is added, keeping the rules	      */
/*		sorted on rule number.	If the new structure rule     */
/*		can not be inserted in the current SRT, because       */
/*		of SRT being full, space is reallocated for SRT.      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Srt	 *srt_entry; 	pointer to Srt entry to be    */
/*					added			      */
/*		signed32 *no_srt;	pointer to no. entries in     */
/*					structure rule table.	      */
/*		Srt	**srt;		pointer to pointer to SRT.    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode	 *ret_code;	pointer to errocde	      */
/*		signed32 *no_srt;	pointer to no. entries in     */
/*					structure rule table.	      */
/*		Srt	**srt;		pointer to pointer to SRT.    */
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

 signed32 d20_031_add_srule(
   Errcode   *ret_code,
   Srt       *srt_entry,
   signed32  *no_srt,
   Srt      **srt)

{		/* begin d20_031_add_srule()	*/
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
	Srt	*tmp_ent;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* check if new rule can be inserted in current SRT */
if (*no_srt == *(signed32 *)(*srt + *no_srt)) 
	{/* there is no space in current SRT */
	/* reallocate space for SRT */
  	if ((mem = realloc(*srt, ((*no_srt + D2_S_MORE) * sizeof(Srt)) + 
				  sizeof(signed32))) == (char *)NULL) 
		{
  		d20_errcode(ret_code, TEMPERR, SYSERR, DS_ALLOC_ERROR);
		return(D2_ERROR);
		}

	*srt = (Srt *)mem;

	/* store new tructure rule no. limit */
	*(signed32 *)(*srt + *no_srt) += D2_S_MORE;
	}

/* at this point enough space is there in SRT to store a new structure rule. */

/* find the position where new structure rule is to be inserted.	*/
for (j = 0; j < *no_srt; j++)
	if (srt_entry->rule_nr < (*srt + j)->rule_nr)
		break;

/* store the str. rule no. limit at new position (one Srt structure ahead).*/
*(signed32 *)(*srt + *no_srt + 1) = *(signed32 *)(*srt + *no_srt);

/* shift the SRT from the point where new rule is to be   */
/* inserted, by sizeof(Srt) towards higher memory address.	  */

/* tmp_ent points to last free entry.		*/
tmp_ent = *srt + *no_srt;

for (i = 0; i < *no_srt - j; i++, tmp_ent--)
	memcpy((char *)tmp_ent, (char *)(tmp_ent - 1), sizeof(Srt));

/* new structure rule is inserted in the proper slot in SRT */
memcpy((char *)(*srt + j), (char *)srt_entry, sizeof(Srt));

(*no_srt)++;

return(D2_NOERROR);

}		/* end of d20_031_add_srule()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_032_del_srule()                                   */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_032_del_srule(rule_nr,no_srt,srt)        */
/*		signed32  rule_nr;	rule no. of structure rule to */
/*					be deleted.		      */
/*		signed32 *no_srt;	pointer to no. entries in     */
/*					SRT.	      	      	      */
/*		Srt	 *srt;		pointer to SRT.		      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function deletes an structure rule from SRT.     */
/*		Entries ahead of the structure rule are shifted       */
/*		towards lower address by one Srt entry. New number    */
/*	        of entries in SRT is returned as a result	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32  rule_nr;	rule no. of structure rule to */
/*					be deleted.		      */
/*		signed32 *no_srt;	pointer to no. entries in     */
/*					SRT.	      	      	      */
/*		Srt	 *srt;		pointer to SRT.		      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *no_srt;	pointer to no. entries in     */
/*					SRT.	      	      	      */
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

 signed32 d20_032_del_srule(
   signed32  rule_nr,
   signed32 *no_srt,
   Srt      *srt)

{		/* begin d20_032_del_srule()	*/
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

/* search structure rule in SRT.	*/
j = d20_034_search_srule(rule_nr, *no_srt, srt);

/* shift the entries,ahead of the entry to be deleted, by sizeof(Srt) towards */
/* lower memory address.		   				      */
memcpy((char *)(srt + j),(char *)(srt + j + 1),sizeof(Srt) * (*no_srt - j - 1));

/* store the capacity of structure rule table at the proper place.	*/
*(signed32 *)(srt + *no_srt - 1) = *(signed32 *)(srt + *no_srt);

/* reduce the no. of entries in structure rule table.	*/
(*no_srt)--;

return(D2_NOERROR);

}		/* end of d20_032_del_srule()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_033_mod_srule()                                   */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:      signed32 d20_033_mod_srule(srt_entry,no_srt,srt)      */
/*		Srt	 *srt_entry;	pointer to modified Srt entry.*/
/*		signed32  no_srt;	no. entries in SRT.	      */ 
/*		Srt	 *srt;		pointer to SRT		      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function stores an structure rule entry in       */
/*		structure rule table after it has been modified.      */ 
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Srt	 *srt_entry;	pointer to modified SRT entry.*/
/*		signed32  no_srt;	no. entries in SRT.	      */ 
/*		Srt	 *srt;		pointer to SRT		      */
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

 signed32 d20_033_mod_srule(
   Srt      *srt_entry,
   signed32  no_srt,
   Srt      *srt)

{		/* begin d20_033_mod_srule()	*/
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

/* search structure rule in SRT.	*/
j = d20_034_search_srule(srt_entry->rule_nr, no_srt, srt);

/* copy srt_entry into SRT.	*/
memcpy((char *)(srt + j),(char *)srt_entry, sizeof(Srt));

return(D2_NOERROR);

}		/* end of d20_033_mod_srule()		*/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_srule_cmp()	                                      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        07.02.91                                              */
/*								      */
/* SYNTAX:	signed32 d20_srule_cmp(srt_ent1, srt_ent2)	      */
/*		Srt	*srt_ent1, *srt_ent2;	      		      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks if two str. rules have same parnt*/
/*		, same object class and same set of naming attributes.*/
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Srt	*srt_ent1, *srt_ent2;	      		      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		0	if two str. rules are same.		      */
/*		1	otherwise.				      */
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

 static signed32 d20_srule_cmp(
  Srt *srt_ent1,
  Srt *srt_ent2)

{		/* begin d20_srule_cmp()	*/
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

	signed32	i, j;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if ((srt_ent1->par_rule_nr == srt_ent2->par_rule_nr) && 
    (strcmp(srt_ent1->obj_class, srt_ent2->obj_class) == 0) && 
    (srt_ent1->no_nam_attr == srt_ent2->no_nam_attr))
	{
	/* check if set of naming attributes is same. */
	for (i=0; i<srt_ent1->no_nam_attr; i++)
		{
		for (j=0; j<srt_ent2->no_nam_attr; j++)
			if (strcmp(srt_ent1->nam_attr[i], 
				   srt_ent2->nam_attr[j]) == 0)
				break;

		/* this naming attribute 'i' does not match.	*/
		if (j == srt_ent2->no_nam_attr)
			break;
		}

	/* all naming attributes match.	*/
	if (i == srt_ent1->no_nam_attr)
		return(0);
	
	return(1);
	}

return(1);

}	/* end of d20_srule_cmp().	*/
