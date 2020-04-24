/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20transf.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:34  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:50  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:33  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:05:33  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:23  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:20:55  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:17:53  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:17:50  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:16  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:56:56  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:04:07  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:43:10  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:15  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20transf.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:32 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20_transform[transform DSA scheme to admin. scheme]*/
/*								      */
/* AUTHOR       : SANJAY JAIN (Siemens India) 			      */
/* DATE         : 26.11.90 					      */
/*								      */
/* COMPONENT    : 						      */
/*								      */
/* DOK.-NR.	: 						      */
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
#include <sys/types.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2shm.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define MAY	0
#define	MUST	1
#define ROOT	0	/* rule no. of root of SRT tree.	*/

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

static void d20_build_supclass(signed32 *no_sup_class, D20_oct **sup_class,
			Oct *oct_ent, signed32 no_oct, Oct *oct,
			signed32 no_admoct, D20_oct *admoct);
static void d20_build_attr(signed16 must_may, signed32 *no_dst,
			D20_at **attrlist_dst, signed32 no_bar_attr,
			D20_at **bar_attr_list, Oct *oct_ent, signed32 no_oct,
			Oct *oct, signed32 no_admat, D20_at *admat);
static signed32 d20_merge_attr(D20_at **attrlist_dst, signed32 no_dst,
			Abbrv *attrlist_src, signed32 no_src,
			D20_at **bar_attr_list, signed32 no_bar_attr,
			signed32 no_admat, D20_at *admat);
static  void d20_compute_max_attr_supclass(signed32 no_oct, Oct *oct,
			Oct *oct_ent, signed32 *max_no_attr,
			signed32 *max_no_supclass);
static signed32 d20_check_rule_merge(Srt *srt_ent, Srt *run_srt_ent);
static signed32 d21_adm_srt_ent_cmp(D20_srt *adm_srt_ent1,
			D20_srt *adm_srt_ent2);
static signed32 d21_at_cmp_obj_id(D20_at **at1, D20_at **at2);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_sch_trans() 				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	d2_ret_val d20_sch_trans(no_at, at, no_oct, oct,      */
/*				no_srt,	srt, no_admat, admat,         */
/*				no_admoct, admoct, no_admsrt, admsrt) */
/*		signed32 no_at;					      */
/*		At	 *at;					      */
/*		signed32  no_oct;				      */
/*		Oct	 *oct;					      */
/*		signed32  no_srt;				      */
/*		Srt      *srt;					      */
/*								      */
/*		signed32 *no_admat;				      */
/*		D20_at	**admat;				      */
/*		signed32 *no_admoct;				      */
/*		D20_oct	**admoct;				      */
/*		signed32 *no_admsrt;				      */
/*		D20_srt **admsrt;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function transforms the DSA scheme to admin. sch.*/
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		no_at;		no. of attributes.		      */
/*		at;		pointer to AT.			      */
/*		no_oct;		no. of object classes.		      */
/*		oct;		pointer to OCT.			      */
/*		no_srt;		no. of structural rules.	      */
/*		srt;		pointer to SRT.			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		*no_admat;	no. of attributes in admin. scheme.   */
/*		*admat;		pointer to admin. AT.		      */
/*		*no_admoct;	no of object classes in admin scheme. */
/*		*admoct;	pointer to admin. OCT.		      */
/*		*no_admsrt;	no of structural rules in admin scheme*/
/*		*admsrt;	pointer to admin. SRT.		      */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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

 d2_ret_val d20_sch_trans(
   signed32  no_at,
   At       *at,
   signed32  no_oct,
   Oct      *oct,
   signed32  no_srt,
   Srt      *srt,
   signed32 *no_admat,
   D20_at  **admat,
   signed32 *no_admoct,
   D20_oct **admoct,
   signed32 *no_admsrt,
   D20_srt **admsrt)

{

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

At		*at_ent;
At		*tmp_at;
D20_at		*adm_at_ent;

Oct		*oct_ent;
D20_oct 	*adm_oct_ent;

Srt		*srt_ent, *run_srt_ent;

D20_srt		*next_lev_ent;
signed32	no_next_lev;
D20_srt		*curr_lev_ent;
signed32	no_curr_lev;
D20_srt		*basis_srt_ent;

D20_srt		root_ent[1];

D20_srt		*adm_srt_ent;

signed32	i,j,k,l;
signed32	max_no_attr;
signed32	max_no_supclass;
signed32	max_no_class;
char		*next_oid_str;
D20_at		**at_ent_dptr;
D20_oct		**oct_ent_dptr;

signed32	*sav_par_rule;
signed32	old_rule_nr;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/************ convert AT of DSA scheme to At of admin. scheme **********/

*no_admat = no_at;

/* allocate space for attribute table and for object id. strings */
if ((*admat = (D20_at *)malloc((sizeof(D20_at) + D2_OBJ_IDL_MAX) * *no_admat))
	      ==  (D20_at *)NULL)
	return (D2_ERROR) ;	

next_oid_str = (char *)(*admat + *no_admat);

/* for all attribute entries */
for (i = 0, adm_at_ent = *admat, at_ent = at; i < no_at; 
     i++, adm_at_ent++, at_ent++)
	{
	tmp_at = &(adm_at_ent->at);
	strcpy(tmp_at->att_abbrv,at_ent->att_abbrv);
	strcpy(tmp_at->att_obj_id,at_ent->att_obj_id);

/* generate obj_id from printable obj_id string */
	adm_at_ent->obj_id.d2_type = (Ob_id_string)next_oid_str;
	next_oid_str += D2_OBJ_IDL_MAX;

	if (d27_302_str_oid(tmp_at->att_obj_id,
			    &(adm_at_ent->obj_id)) == D2_ERROR)
		{
		free((char *)*admat);
		return (D2_ERROR) ;	
		}

	strcpy(tmp_at->att_name,at_ent->att_name);
	tmp_at->att_lower 		= at_ent->att_lower;
	tmp_at->att_upper 		= at_ent->att_upper;
	tmp_at->att_no_recur 		= at_ent->att_no_recur;
	tmp_at->att_syntax 		= at_ent->att_syntax;
	tmp_at->att_acl 		= at_ent->att_acl;
	tmp_at->att_ind_lev 		= at_ent->att_ind_lev;
	tmp_at->att_phon 		= at_ent->att_phon;
	}

/************ convert OCT of DSA scheme to OCT of admin. scheme **********/

*no_admoct = no_oct;

/* for all object class entries compute the maximum no. of  attributes 	*/
/* and maximum no. of super classes involved and take the total.	*/

max_no_attr = max_no_supclass = 0;

for (i = 0, oct_ent = oct; i < no_oct; i++, oct_ent++)
	d20_compute_max_attr_supclass(no_oct, oct, oct_ent,
				       &max_no_attr, &max_no_supclass);

/* allocate space for object class table , pointers to attr. entries, */
/* pointers to Supclasses and for object id. strings.		      */
if ((*admoct = (D20_oct *)malloc(((sizeof(D20_oct)+D2_OBJ_IDL_MAX) * *no_admoct)
				 + (sizeof(D20_at *) * max_no_attr) 
				 + (sizeof(D20_oct *) * max_no_supclass)))
	      ==  (D20_oct *)NULL)
	{
	free((char *)*admat);
	return (D2_ERROR) ;	
	}

at_ent_dptr = (D20_at **)(*admoct + *no_admoct);
oct_ent_dptr = (D20_oct **)(at_ent_dptr + max_no_attr);
next_oid_str = (char *)(oct_ent_dptr + max_no_supclass);

/* for all object class entries */
for (i = 0, adm_oct_ent = *admoct, oct_ent = oct; i < no_oct; 
     i++, adm_oct_ent++, oct_ent++)
	{
	strcpy(adm_oct_ent->obj_abbrv,oct_ent->obj_abbrv);

	adm_oct_ent->obj_id.d2_type = (Ob_id_string)next_oid_str;
	next_oid_str += D2_OBJ_IDL_MAX;

	if (strcmp(oct_ent->obj_id, D2_INVALID_PRT_OBJ_ID) == 0)
		adm_oct_ent->obj_id.d2_typ_len = 0;
	else
		if (d27_302_str_oid(oct_ent->obj_id,
			    	&(adm_oct_ent->obj_id)) == D2_ERROR)
			{
			free((char *)*admat);
			free((char *)*admoct);
			return (D2_ERROR) ;	
			}

	strcpy(adm_oct_ent->obj_name,oct_ent->obj_name);

	/* build mandatory attribute list for the current object class */

	adm_oct_ent->must_attr = at_ent_dptr;
	adm_oct_ent->no_must_attr = 0;
	d20_build_attr(MUST,
		       &adm_oct_ent->no_must_attr,
		       adm_oct_ent->must_attr,
		       0, (D20_at **)NULL,
		       oct_ent, no_oct, oct, *no_admat, *admat);	

	at_ent_dptr += adm_oct_ent->no_must_attr;

	/* build optional attribute list for the current object
	   class */

	adm_oct_ent->may_attr = at_ent_dptr; 
	adm_oct_ent->no_may_attr = 0;
	d20_build_attr(MAY,
		       &adm_oct_ent->no_may_attr,
		       adm_oct_ent->may_attr,
		       adm_oct_ent->no_must_attr,
		       adm_oct_ent->must_attr,
		       oct_ent, no_oct, oct, *no_admat, *admat);	

	at_ent_dptr += adm_oct_ent->no_may_attr;
	}

/* for all object class entries of administration schema */
/* fill auxiliary object classes component and  superior classes component */
for (i = 0, adm_oct_ent = *admoct, oct_ent = oct; i < no_oct; 
     i++, adm_oct_ent++, oct_ent++)
	{
	adm_oct_ent->sup_class = oct_ent_dptr;
	adm_oct_ent->no_sup_class = 0;
	d20_build_supclass(&adm_oct_ent->no_sup_class,
		       adm_oct_ent->sup_class,
		       oct_ent, no_oct, oct, *no_admoct, *admoct);	

	oct_ent_dptr += adm_oct_ent->no_sup_class;

	/* fill auxiliary classes component.	*/

	adm_oct_ent->no_aux_class = oct_ent->no_aux_class;
	for (j=0; j<oct_ent->no_aux_class; j++)
		adm_oct_ent->aux_class[j] = (D20_oct *)d20_search_obj(
						       ADM_SCHEME,
					    	       *no_admoct, 
						       (caddr_t)*admoct, 
					    	       oct_ent->aux_class[j]);

	}

/************ convert SRT of DSA scheme to SRT of admin. scheme **********/

/* no. of pointers to D20_oct. 					*/
/* Each SRT rule has just 1 object class.			*/
max_no_class = no_srt;

/* allocate space for structural rule table and pointers to D20_oct.	*/
if ((*admsrt = (D20_srt *)malloc((sizeof(D20_srt) * no_srt) +
				 (sizeof(D20_oct *) * max_no_class)))
	      ==  (D20_srt *)NULL)
	{
	free((char *)*admat);
	free((char *)*admoct);
	return (D2_ERROR) ;	
	}

/* allocate space to save parent rule nos.	*/
if ((sav_par_rule = (signed32 *)malloc(sizeof(signed32) * no_srt)) ==  (signed32 *)NULL)
	{
	free((char *)*admat);
	free((char *)*admoct);
	free((char *)*admsrt);
	return (D2_ERROR) ;	
	}

/* pointer to space where pointers to D20_oct are stored.	*/
oct_ent_dptr = (D20_oct **)(*admsrt + no_srt);

/* save parent rule numbers.	*/
for (i = 0, srt_ent = srt; i < no_srt; i++, srt_ent++)
	*(sav_par_rule + i) = srt_ent->par_rule_nr;

curr_lev_ent = root_ent;
curr_lev_ent->rule_nr = ROOT;
no_curr_lev = 1;

next_lev_ent = *admsrt;
no_next_lev = 0;
*no_admsrt = 0;

/* process all levels of SRT tree.				*/
/* process till no. of SRT rules in current level is 0.		*/

while (no_curr_lev != 0)
	{
	/* process all rules numbers at the same level.	*/	
	for (i = 0; i < no_curr_lev; i++)
		{
		/* try to merge children of this rule.	*/
		for (j = 0, srt_ent = srt; j < no_srt; j++, srt_ent++)
			{
			/* look for the first child in which other children */
			/* can be merged.				    */
			if (srt_ent->par_rule_nr != (curr_lev_ent + i)->rule_nr)
				continue;

			/* first child found.	*/

			basis_srt_ent = next_lev_ent + no_next_lev;
			(*no_admsrt)++;
			no_next_lev++;	

			basis_srt_ent->rule_nr 	= srt_ent->rule_nr;
			basis_srt_ent->par_rule_nr = srt_ent->par_rule_nr;
			basis_srt_ent->no_nam_attr = srt_ent->no_nam_attr;

			for (k = 0; k < srt_ent->no_nam_attr; k++)
				basis_srt_ent->nam_attr[k] = 
						d20_search_adm_attr(*no_admat,
						*admat, srt_ent->nam_attr[k]);

			basis_srt_ent->no_obj_class = 1;
			basis_srt_ent->obj_class = oct_ent_dptr;
			*oct_ent_dptr++ = (D20_oct *)d20_search_obj(
						       ADM_SCHEME,
					    	       *no_admoct, 
						       (caddr_t)*admoct, 
					    	       srt_ent->obj_class);

			/* in all subsequent SRT entries check for entries */
			/* which can be merged to basis_srt_ent.	   */
			for (k = j + 1, run_srt_ent = srt_ent + 1; k < no_srt; 
						     k++, run_srt_ent++)
				{
				if (d20_check_rule_merge(run_srt_ent, srt_ent))
					{
					/* merge this rule.		    */
					basis_srt_ent->no_obj_class++;
					*oct_ent_dptr++ = 
						(D20_oct *)d20_search_obj(
						       ADM_SCHEME,
					    	       *no_admoct, 
						       (caddr_t)*admoct, 
					    	       run_srt_ent->obj_class);
					
					/* all structure rules which have  */
					/* their parent rule same as the   */
					/* rule just merged to basis_rule, */
					/* change the parent of such rules */
					/* to the basis rule.		   */

					for (l=0; l<no_srt; l++)
						if ((srt+l)->par_rule_nr ==
							run_srt_ent->rule_nr)
							(srt+l)->par_rule_nr =
							srt_ent->rule_nr;
						
					/* mark this entry as merged.	*/
					run_srt_ent->par_rule_nr = -1;
					}
				else
					/* look for another rule which can */
					/* be merged. 			   */
					continue;
				}	/* end of for loop to merge SRT rules.*/

			srt_ent->par_rule_nr = -1;

			}	/* end of loop to look for children.	*/ 	
		}	/* end of for loop to process all entries of one level*/

	curr_lev_ent = next_lev_ent;
	no_curr_lev = no_next_lev;

	next_lev_ent = curr_lev_ent + no_curr_lev;
	no_next_lev = 0;
	}	/* end of while loop to process all levels.	*/

/* restore parent rule numbers.	*/
for (i = 0, srt_ent = srt; i < no_srt; i++, srt_ent++)
	 srt_ent->par_rule_nr = *(sav_par_rule + i);

/* sort D20_srt on integer value of rule number.	*/
qsort((char *)*admsrt, (size_t)*no_admsrt, (size_t)(sizeof(D20_srt)),
		(int (*)(const void *, const void *)) d21_adm_srt_ent_cmp);

/* change rule numbers.		*/
for (i = 1, adm_srt_ent = *admsrt; i <= *no_admsrt; i++, adm_srt_ent++)
	{
	old_rule_nr = adm_srt_ent->rule_nr;
	adm_srt_ent->rule_nr = i;

	for (j = 0; j < *no_admsrt; j++)
		if ((*admsrt + j)->par_rule_nr == old_rule_nr)
			(*admsrt + j)->par_rule_nr = i;
	}

/* fill parent rule pointers.		*/
for (i = 0, adm_srt_ent = *admsrt; i < *no_admsrt; i++, adm_srt_ent++)
	adm_srt_ent->par_rule = d20_search_adm_srule(*no_admsrt, *admsrt,
						     adm_srt_ent->par_rule_nr);
		
/* sort naming attributes of each admsrt entry in ascending order of */
/* obj_id.							     */
for (i=0; i<*no_admsrt; i++)
	qsort((char *)(*admsrt)[i].nam_attr, (size_t)(*admsrt)[i].no_nam_attr,
	      (size_t)(sizeof(D20_at *)),
	      (int (*)(const void *, const void *)) d21_at_cmp_obj_id);

free((char *)sav_par_rule);
return(D2_NOERROR);

}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_build_attr() 				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	d20_build_attr(must_may, no_dst, attrlist_dst,	      */
/*			       no_bar_attr, bar_attr_list, oct_ent,   */
/*			       no_oct, oct, no_admat, admat)          */
/*		signed16	must_may;			      */
/*		signed32	*no_dst;			      */
/*		D20_at		**attrlist_dst;			      */
/*		signed32	no_bar_attr;			      */
/*		D20_at		**bar_attr_list;		      */
/*		Oct		*oct_ent;			      */
/*		signed32	no_oct;				      */
/*		Oct		*oct;				      */	
/*		signed32	no_admat;			      */
/*		D20_at		*admat;				      */	
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function builds the optional/mandatory attribute */
/*		list for an object class. While generating optional   */
/*		attribute list it should be checked that these 	      */
/*		optional attributes don't occur in mandatory attribute*/
/*		list (bar_attr_list).			      	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		must_may	flag indicating optional/mandatory    */
/*				attribute list to be generated	      */
/*		no_bar_attr	no. of attributes in bar_list.	      */
/*		bar_attr_list	list of attributes against which it   */
/*				should be checked that attribute      */
/*				being merged does not exist in this   */
/*				list.		      		      */
/*		*oct_ent	current object class entry in DSA sch.*/
/*		no_oct		no. of elements in OCT of DSA scheme  */
/*		oct		pointer to OCT of DSA scheme	      */
/*		no_admat	no. of attributes in admin. AT	      */
/*		admat		pointer to admin. AT		      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		attrlist_dst	destination attribute list	      */
/*		*no_dst		no. of attributes in attrlist_dst     */
/*								      */
/* RETURNVALUE:							      */
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

 static void d20_build_attr(
   signed16   must_may,
   signed32  *no_dst,
   D20_at   **attrlist_dst,
   signed32   no_bar_attr,
   D20_at   **bar_attr_list,
   Oct       *oct_ent,
   signed32   no_oct,
   Oct       *oct,
   signed32   no_admat,
   D20_at    *admat)

{

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

Oct		*sup_oct_ent;
signed32	i;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (must_may == MUST)
	/* merge the mandatory attribute list of source object class
	   to the destination attribute list */
	*no_dst = *no_dst + d20_merge_attr(attrlist_dst,*no_dst,
					   oct_ent->must_attr,
					   oct_ent->no_must_attr,
					   bar_attr_list, no_bar_attr, 
					   no_admat,admat);
else
	/* merge the optional attribute list of source object class
	   to the destination attribute list */
	*no_dst = *no_dst + d20_merge_attr(attrlist_dst,*no_dst,
					   oct_ent->may_attr,
					   oct_ent->no_may_attr,
					   bar_attr_list, no_bar_attr, 
					   no_admat,admat);

for (i=0; i<oct_ent->no_sup_class; i++)
	{
	sup_oct_ent = (Oct *)d20_search_obj(DSA_SCHEME,no_oct,(caddr_t)oct,
				  oct_ent->sup_class[i]);
	d20_build_attr(must_may, no_dst, attrlist_dst, 
		no_bar_attr, bar_attr_list, sup_oct_ent,
		       	       no_oct,oct,no_admat,admat);
	}
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_merge_attr() 				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	signed32 d20_merge_attr(attrlist_dst,no_dst,	      */
/*				   attrlist_src,no_src,		      */
/*				   bar_attr_list,no_bar_attr,	      */
/*				   no_admat,admat)		      */
/*		D20_at  **attrlist_dst;				      */
/*		signed32 no_dst;	      		              */
/*		Abbrv	 *attrlist_src;		      		      */
/*		signed32 no_src;	      		              */
/*		D20_at  **bar_attr_list;			      */
/*		signed32 no_bar_attr;		      		      */
/*		signed32 no_admat;				      */
/*		D20_at	 *admat;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function merges attrlist_src into attrlist_dst.  */
/*		Only those Elements of source list are merged into    */
/*		destination list which don't occur in bar_attr_list.  */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		attrlist_dst	pointer to destination attr. list     */
/*		no_dst		no. of attributes in attrlist_dst     */	
/*		attrlist_src	pointer to source attr. list	      */	
/*		no_src		no. of attributes in attrlist_src     */
/*		bar_attr_list	list of attributes to be avoided.     */
/*		no_bar_attr	no. of attributes to be avoided.      */
/*		no_admat	no. of attributes in admin. AT	      */
/*		admat		pointer to admin. AT		      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		new attribute list attrlist_dst			      */
/*								      */
/* RETURNVALUE:							      */
/*		no. of attributes inserted by this routine into       */	
/*		attrlist_dst.					      */
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

 static signed32 d20_merge_attr(
   D20_at   **attrlist_dst,
   signed32   no_dst,
   Abbrv     *attrlist_src,
   signed32   no_src,
   D20_at   **bar_attr_list,
   signed32   no_bar_attr,
   signed32   no_admat,
   D20_at    *admat)

{

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

signed32 i,j;
signed32 no_att_inserted=0;
signed16 found;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for (i=0;i<no_src;i++)
	{
	found = FALSE;
	j = 0;
	while ((found == FALSE) && (j<no_dst)) 
		{
		if (!strcmp((*(attrlist_dst + j))->at.att_abbrv,
			    attrlist_src[i]))
			found = TRUE;
		else
			j++;
		}

	/* attribute does not exist in destination list.	*/
	if (found == FALSE)
		{
		/* check if attribute exists in list of attributes to be   */
		/* avoided.						   */
		/* e.g while making list of optional attributes, attribute */
		/* should not occur in list of mandatory attributes.	   */
		j = 0;
		while ((found == FALSE) && (j < no_bar_attr))
			{
			if (!strcmp((*(bar_attr_list + j))->at.att_abbrv,
				    attrlist_src[i]))
				found = TRUE;
			else
				j++;
			}

		if (found == FALSE)
			{
			*(attrlist_dst + no_dst) = d20_search_adm_attr(no_admat,
						   admat, attrlist_src[i]);
			no_att_inserted++;
			no_dst++;
			}
		}
	}

return(no_att_inserted);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_compute_max_attr_supclass() 		      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	void d20_compute_max_attr_supclass(no_oct, oct,       */
/*				oct_ent, max_no_attr, max_no_supclass)*/
/*		signed32  no_oct;				      */
/*		Oct	 *oct, *oct_ent;			      */
/*		signed32 *max_no_attr, *max_no_supclass;	      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function computes the maximum no. of attributes  */
/*		and superclasses involved in admin. schema for an     */
/*		object class table entry.	      		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32  no_oct;				      */
/*		Oct	 *oct, *oct_ent;			      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *max_no_attr, *max_no_supclass;	      */
/*								      */
/* RETURNVALUE:							      */
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

 static  void d20_compute_max_attr_supclass(
   signed32  no_oct,
   Oct      *oct,
   Oct      *oct_ent,
   signed32 *max_no_attr,
   signed32 *max_no_supclass)

{

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

signed32  i;
Oct	 *sup_oct_ent;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*max_no_attr += oct_ent->no_must_attr + oct_ent->no_may_attr;
*max_no_supclass += oct_ent->no_sup_class;

for (i=0; i<oct_ent->no_sup_class; i++)
	{
	/* serach for the superior object class entry of source object class */
	sup_oct_ent = (Oct *)d20_search_obj(DSA_SCHEME, no_oct, 
			    (caddr_t)oct, oct_ent->sup_class[i]);
	d20_compute_max_attr_supclass(no_oct, oct, sup_oct_ent,
					       max_no_attr, max_no_supclass);
	}
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_build_supclass() 				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	void d20_build_supclass(no_sup_class, sup_class,      */
/*      	       oct_ent, no_oct, oct, no_admoct, admoct)	      */
/*		signed32 *no_sup_class;				      */
/*		D20_oct	**sup_class;				      */
/*		Oct	 *oct_ent;				      */
/*		signed32  no_oct;			      	      */
/*		Oct	 *oct;				      	      */
/*		signed32  no_admoct;				      */
/*		D20_oct	 *admoct;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function builds the complete list of super       */
/*		classes for an OCT entry of admin. schema.	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32 *no_sup_class;				      */
/*		D20_oct	**sup_class;				      */
/*		Oct	 *oct_ent;				      */
/*		signed32  no_oct;			      	      */
/*		Oct	 *oct;				      	      */
/*		signed32  no_admoct;				      */
/*		D20_oct	 *admoct;				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *no_sup_class;				      */
/*		D20_oct	**sup_class;				      */
/*								      */
/* RETURNVALUE:							      */
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

 static void d20_build_supclass(
   signed32  *no_sup_class,
   D20_oct  **sup_class,
   Oct       *oct_ent,
   signed32   no_oct,
   Oct       *oct,
   signed32   no_admoct,
   D20_oct   *admoct)

{

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

signed16 found;
signed32 i, j;
Oct	*sup_oct_ent;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* merge super classes.	*/
for (i=0;i<oct_ent->no_sup_class; i++)
	{
	found = FALSE;
	j = 0;
	while ((found == FALSE) && (j<*no_sup_class)) 
		{
		if (!strcmp((*(sup_class + j))->obj_abbrv, 
			    oct_ent->sup_class[i]))
			found = TRUE;
		else
			j++;
		}

	if (found == FALSE)
		{
		/* superior object class does not exist in the list of 	*/
		/* object classes.					*/
		*(sup_class + j) = (D20_oct *)d20_search_obj(
						       ADM_SCHEME,
			  	       		       no_admoct, 
						       (caddr_t)admoct, 
					    	       oct_ent->sup_class[i]);
		(*no_sup_class)++;
		}
	}

for (i=0; i<oct_ent->no_sup_class; i++)
	{
	sup_oct_ent = (Oct *)d20_search_obj(DSA_SCHEME, no_oct, 
		(caddr_t)oct, oct_ent->sup_class[i]);
	d20_build_supclass(no_sup_class, sup_class,
		       	       sup_oct_ent, no_oct, oct, no_admoct, admoct);	
	}

}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_check_rule_merge()				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	signed32 d20_check_rule_merge(srt_ent, run_srt_ent)   */
/*		Srt	*srt_ent, *run_srt_ent;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function checks if two SRT rules in DSA schema   */
/*		have same set of naming attributes and same parent    */
/*		rule number for the purpose of merge. 		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		Srt	*srt_ent, *run_srt_ent;			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		D2_TRUE		If two rules can be merged.	      */
/*		D2_FALSE	If not.				      */
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

 static signed32 d20_check_rule_merge(
   Srt *srt_ent,
   Srt *run_srt_ent)

{

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

signed32	k, l;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if ((srt_ent->par_rule_nr == run_srt_ent->par_rule_nr) &&
    (srt_ent->no_nam_attr == run_srt_ent->no_nam_attr))
	{
	/* no. of naming attributes is same and parent rule no. is same.   */

	/* check if naming attributes are same*/
	for (k=0; k<run_srt_ent->no_nam_attr; k++)
		{
		for (l=0; l<srt_ent->no_nam_attr; l++)
			if (strcmp(run_srt_ent->nam_attr[k], 
				   srt_ent->nam_attr[l]) != 0)
				break;

		if (l != srt_ent->no_nam_attr)
			break;
		}

	if (k != run_srt_ent->no_nam_attr)
		return(FALSE);
	else
		return(TRUE);

	}
else
	return(FALSE);

}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_adm_srt_ent_cmp()				      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_adm_srt_ent_cmp(adm_srt_ent1,	      */
/*					     adm_srt_ent2)            */
/*		D20_srt	*adm_srt_ent1;				      */
/*		D20_srt	*adm_srt_ent2;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function defines the criteria for sorting the    */
/*		D20_srt entries.				      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*		D20_srt	*adm_srt_ent1;				      */
/*		D20_srt	*adm_srt_ent2;				      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		-1	adm_srt_ent1 < adm_srt_ent2		      */
/*		 0	adm_srt_ent1 = adm_srt_ent2		      */
/*		 1	adm_srt_ent1 > adm_srt_ent2		      */
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

 static signed32 d21_adm_srt_ent_cmp(
   D20_srt *adm_srt_ent1,
   D20_srt *adm_srt_ent2)

{

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

if (adm_srt_ent1->rule_nr < adm_srt_ent2->rule_nr)
	return(-1);

if (adm_srt_ent1->rule_nr == adm_srt_ent2->rule_nr)
	return(0);

if (adm_srt_ent1->rule_nr > adm_srt_ent2->rule_nr)
	return(1);

}		/* end of d21_adm_srt_ent_cmp()		*/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d21_at_cmp_obj_id()				      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.01.91					      */
/*								      */
/* SYNTAX:      signed32 d21_at_cmp_obj_id(at1, at2)	              */
/*		String	**at1;					      */
/*		String	**at2;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function defines the criteria for sorting the    */
/*		D20_at entries.					      */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*		String	**at1;					      */
/*		String	**at2;					      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		-1	at1 < at2				      */
/*		 0	at1 = at2				      */
/*		 1	at1 > at2				      */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*		memcmp()					      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 static signed32 d21_at_cmp_obj_id(
   D20_at **at1,
   D20_at **at2)

{

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

   signed32	cmp_len, cmp_res;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/


if ((*at1)->obj_id.d2_typ_len < (*at2)->obj_id.d2_typ_len)
	cmp_len = (*at1)->obj_id.d2_typ_len;
else
	cmp_len = (*at2)->obj_id.d2_typ_len;

cmp_res = memcmp((*at1)->obj_id.d2_type, (*at2)->obj_id.d2_type, cmp_len);

if (cmp_res < 0)
	return(-1);

if (cmp_res > 0)
	return(1);

if (cmp_res == 0)
	if ((*at1)->obj_id.d2_typ_len < (*at2)->obj_id.d2_typ_len)
		return(-1);
	else
		if ((*at1)->obj_id.d2_typ_len > (*at2)->obj_id.d2_typ_len)
			return(1);
		else
			return(0);
	
}		/* end of d21_at_cmp_obj_id()		*/
