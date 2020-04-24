/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20getsh.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:40  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:52  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:18  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:09:06  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:28:51  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:04:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:28:48  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:08:33  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:04:10  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:11:38  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:08:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:54:56  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:15:51  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20getsh.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:44 $";
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
/* NAME         : d20getsh.o [Load locally stored admin scheme file]  */
/*								      */
/* AUTHOR       : SANJAY JAIN (Siemens India) 			      */
/* DATE         : 26.11.90 					      */
/*								      */
/* COMPONENT    : 						      */
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
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2shm.h>
#include <d20proto.h>

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_lget_scheme()                              	      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/*								      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	signed32 d20_lget_scheme (no_attrs, atptr, no_oclass, */
/*				     octptr, no_rules, srtptr)        */	
/*		D20_at		**atptr;			      */
/*		D20_oct		**octptr;			      */
/*		D20_srt		**srtptr;			      */
/*		signed32     	*no_attrs, *no_oclass, *no_rules;     */
/*								      */
/* DESCRIPTION:                                                       */
/*		This function loads the locally stored admin. scheme  */
/*		file into AT, OCT and SRT.			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		*atptr		pointer to admin. AT		      */
/*		*no_attrs	no. of attributes in AT		      */
/*		*octptr		pointer to admin. OCT		      */
/*		*no_oclass	no. of object classes in OCT	      */
/*		*srtptr		pointer to admin. SRT		      */
/*		*no_rules	no. of rules in AT		      */
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
d2_ret_val d20_lget_scheme (
	signed32  *no_attrs,
	D20_at   **atptr,
	signed32  *no_oclass,
	D20_oct  **octptr,
	signed32  *no_rules,
	D20_srt  **srtptr)
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

FILE		*fdsch ;

D20_at		*at_ent;
D20_oct		*oct_ent;
D20_srt		*srt_ent;

At		*tmp_at;

char		shfname[D20_SHFNMAX] ;
register signed32 i, j ;
signed32	no_att_abbrv, no_sup_class, no_obj_class;

Abbrv		attr_abbrv;
Abbrv		nam_attr;
Abbrv		obj_class;
char		prt_obj_id[D2_OBJ_IDL_MAX + 1];

char		*next_oid_str;
D20_at		**at_ent_dptr;
D20_oct		**oct_ent_dptr;

char		*abbrv_space;
char		*abbrv_space_ptr;

char        env_dir[D27_LFILE_NAME];
String      p_env = env_dir;


/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

sprintf(p_env, "%s%s", dcelocal_path, D27_CLIENT_DIR);

/* generate admin. scheme file name */
sprintf (shfname, "%s%s", p_env, D20_ADMSCH_FILE) ;

/* open ADM scheme file */
if ((fdsch = fopen (shfname, "r")) == (FILE *)NULL) 
	{
	fclose (fdsch) ;
	return (D2_ERROR) ;			
	}

/* read AT HEADER */
if (fscanf(fdsch,ADMAT_HEAD,no_attrs) != 1)
	{
	/* wrong AT header */
	fclose(fdsch);
	return (D2_ERROR) ;	
	}

/* allocate space for attribute table and for object id. strings */
if ((*atptr = (D20_at *)malloc((sizeof(D20_at) + D2_OBJ_IDL_MAX) * *no_attrs)) 
	      ==  (D20_at *) NULL)
	{
	fclose(fdsch);
	return (D2_ERROR) ;	
	}

next_oid_str = (char *)(*atptr + *no_attrs);

/* for all attribute entries */
for (i = 0, at_ent = *atptr; i < *no_attrs; i++, at_ent++)
	{/*  read AT entry from file            */
	tmp_at = &(at_ent->at);
	if (fscanf(fdsch, ADMAT_FORMAT, tmp_at->att_abbrv, tmp_at->att_obj_id,
	  	               tmp_at->att_name, &tmp_at->att_lower, 
			       &tmp_at->att_upper, &tmp_at->att_no_recur, 
			       &tmp_at->att_syntax, &tmp_at->att_acl,
			       &tmp_at->att_ind_lev, &tmp_at->att_phon)
		 != 10)
		{
		fclose(fdsch);
		free((char *)*atptr);
		return (D2_ERROR) ;	
		}

	at_ent->obj_id.d2_type = (Ob_id_string)next_oid_str;
	next_oid_str += D2_OBJ_IDL_MAX;

	if (d27_302_str_oid(tmp_at->att_obj_id, &(at_ent->obj_id)) == D2_ERROR)
		{
		fclose(fdsch);
		free((char *)*atptr);
		return (D2_ERROR) ;	
		}
	}	/* end of for loop to read D20_at entries.	*/

/* read OCT HEADER */
if (fscanf(fdsch,ADMOCT_HEAD, no_oclass, &no_att_abbrv, &no_sup_class) != 3)
	{
	/* wrong OCT header */
	fclose(fdsch);
	free((char *)*atptr);
	return (D2_ERROR) ;	
	}

/* allocate space for object class table , pointers to attr. entries, 	*/
/* pointers to superclasses and for object id. strings.			*/
if ((*octptr = (D20_oct *)malloc(((sizeof(D20_oct)+D2_OBJ_IDL_MAX) * *no_oclass)
				 + (sizeof(D20_at *) * no_att_abbrv)
				 + (sizeof(D20_oct *) * no_sup_class)))
	      ==  (D20_oct *) NULL)
	{
	fclose(fdsch);
	free((char *)*atptr);
	return (D2_ERROR) ;	
	}

/* allocate space for super class and auxiliary class abbreviations.	*/
if ((abbrv_space = (char *)malloc(sizeof(Abbrv) * 
				  (no_sup_class + (*no_oclass * NO_AUX_CLASS))))
		 == (char *)NULL)
	{
	fclose(fdsch);
	free((char *)*atptr);
	free((char *)*octptr);
	return (D2_ERROR) ;	
	}

at_ent_dptr = (D20_at **)(*octptr + *no_oclass);
oct_ent_dptr = (D20_oct **)(at_ent_dptr + no_att_abbrv);
next_oid_str = (char *)(oct_ent_dptr + no_sup_class);

abbrv_space_ptr = abbrv_space;

/* for all object classes */
for (i = 0, oct_ent = *octptr; i < *no_oclass; 
			       i++, oct_ent++, fscanf(fdsch, "\n"))
	{/*  read OCT entry from file            */
	if (fscanf(fdsch, ADMOCT_FORMAT, oct_ent->obj_abbrv,
					prt_obj_id,
					oct_ent->obj_name,
					&oct_ent->no_sup_class,
					&oct_ent->no_aux_class,
					&oct_ent->no_must_attr,
					&oct_ent->no_may_attr)
		 != 7)
		{
		fclose(fdsch);
		free((char *)*atptr);
		free((char *)*octptr);
		free((char *)abbrv_space);
		return (D2_ERROR) ;	
		}

	oct_ent->obj_id.d2_type = (Ob_id_string)next_oid_str;
	if (strcmp(prt_obj_id, D2_INVALID_PRT_OBJ_ID) == 0)
		oct_ent->obj_id.d2_typ_len = 0;
	else
		if (d27_302_str_oid(prt_obj_id, &(oct_ent->obj_id)) == D2_ERROR)
			{
			fclose(fdsch);
			free((char *)*atptr);
			free((char *)*octptr);
			free((char *)abbrv_space);
			return (D2_ERROR) ;	
			}
	
	next_oid_str += D2_OBJ_IDL_MAX;

	/* load super classes.		*/
	for (j=0, fscanf(fdsch, "%*s"); j < oct_ent->no_sup_class; j++)
		{
		if (fscanf(fdsch, "%s", abbrv_space_ptr) != 1)
			{
			fclose(fdsch);
			free((char *)*atptr);
			free((char *)*octptr);
			free((char *)abbrv_space);
			return (D2_ERROR) ;	
			}

		abbrv_space_ptr += strlen(abbrv_space_ptr) + 1;
		}

	/* load auxiliary classes.		*/
	for (j=0, fscanf(fdsch, "%*s"); j < oct_ent->no_aux_class; j++)
		{
		if (fscanf(fdsch, "%s", abbrv_space_ptr) != 1)
			{
			fclose(fdsch);
			free((char *)*atptr);
			free((char *)*octptr);
			free((char *)abbrv_space);
			return (D2_ERROR) ;	
			}

		abbrv_space_ptr += strlen(abbrv_space_ptr) + 1;
		}

	/* load mandatory attributes */
	oct_ent->must_attr = at_ent_dptr;
        for (j=0,fscanf(fdsch,"%*s"); j<oct_ent->no_must_attr; j++)
		{
		if (fscanf(fdsch, "%s", attr_abbrv) != 1)
			{
			fclose(fdsch);
			free((char *)*atptr);
			free((char *)*octptr);
			free((char *)abbrv_space);
			return (D2_ERROR) ;	
			}

		*at_ent_dptr++ = 
			d20_search_adm_attr(*no_attrs,*atptr,attr_abbrv);
		}

	/* load optional attributes */
	oct_ent->may_attr = at_ent_dptr;
        for (j=0,fscanf(fdsch,"%*s"); j<oct_ent->no_may_attr; j++)
		{
		if (fscanf(fdsch, "%s", attr_abbrv) != 1)
			{
			fclose(fdsch);
			free((char *)*atptr);
			free((char *)*octptr);
			free((char *)abbrv_space);
			return (D2_ERROR) ;	
			}

		*at_ent_dptr++ = 
			d20_search_adm_attr(*no_attrs,*atptr,attr_abbrv);

		}

	}	/* end of for loop to read D20_oct entries.	*/

/* set super class and auxiliary class pointers.	*/
for (i = 0, oct_ent = *octptr, abbrv_space_ptr = abbrv_space; i < *no_oclass; 
							      i++, oct_ent++)
	{
	oct_ent->sup_class = oct_ent_dptr;

	for (j=0; j < oct_ent->no_sup_class; j++)
		{
		*oct_ent_dptr++ = (D20_oct *)d20_search_obj(
						           ADM_SCHEME,
					    	           *no_oclass, 
						           (caddr_t)*octptr, 
					    	       	   abbrv_space_ptr);

		abbrv_space_ptr += strlen(abbrv_space_ptr) + 1;
		}

	for (j=0; j < oct_ent->no_aux_class; j++)
		{
		oct_ent->aux_class[j] = (D20_oct *)d20_search_obj(
						           ADM_SCHEME,
					    	           *no_oclass, 
						           (caddr_t)*octptr, 
					    	       	   abbrv_space_ptr);

		abbrv_space_ptr += strlen(abbrv_space_ptr) + 1;
		}
	}


free((char *)abbrv_space);

/* read SRT HEADER */
if (fscanf(fdsch,ADMSRT_HEAD, no_rules, &no_obj_class) != 2)
	{
	/* wrong SRT header */
	fclose(fdsch);
	free((char *)*atptr);
	free((char *)*octptr);
	return (D2_ERROR) ;	
	}

/* allocate space for structure rule table and pointers to D20_oct */
if ((*srtptr = (D20_srt *)malloc((sizeof(D20_srt) * *no_rules) +
				 (sizeof(D20_oct *) * no_obj_class)))
	      ==  (D20_srt *) NULL)
	{
	fclose(fdsch);
	free((char *)*atptr);
	free((char *)*octptr);
	return (D2_ERROR) ;	
	}

oct_ent_dptr = (D20_oct **)(*srtptr + *no_rules);

/* for all structural rules */
for (i = 0, srt_ent = *srtptr; i < *no_rules; 
			       i++, srt_ent++, fscanf(fdsch, "\n"))
	{/*  read SRT entry from file            */
	if (fscanf(fdsch, ADMSRT_FORMAT, &srt_ent->rule_nr,
					&(srt_ent->par_rule_nr),
					&srt_ent->no_nam_attr,
					&srt_ent->no_obj_class)
		 != 4)
		{
		fclose(fdsch);
		free((char *)*atptr);
		free((char *)*octptr);
		free((char *)*srtptr);
		return (D2_ERROR) ;	
		}

	/* load naming attributes.		*/
	for (j=0, fscanf(fdsch, "%*s"); j < srt_ent->no_nam_attr; j++)
		{
		if (fscanf(fdsch, "%s", nam_attr) != 1)
			{
			fclose(fdsch);
			free((char *)*atptr);
			free((char *)*octptr);
			free((char *)*srtptr);
			return (D2_ERROR) ;	
			}

		srt_ent->nam_attr[j] = d20_search_adm_attr(*no_attrs, *atptr,
						    nam_attr);
		}

	srt_ent->obj_class = oct_ent_dptr;

	/* load valid object class abbrvs. for the current rule */
	for (j=0,fscanf(fdsch,"%*s");j<srt_ent->no_obj_class;j++)
		{
		if (fscanf(fdsch, "%s", obj_class) != 1)
			{
			fclose(fdsch);
			free((char *)*atptr);
			free((char *)*octptr);
			free((char *)*srtptr);
			return (D2_ERROR) ;	
			}

		*oct_ent_dptr++ = (D20_oct *) d20_search_obj(
						           ADM_SCHEME,
					    	           *no_oclass, 
						           (caddr_t)*octptr, 
					    	       	   obj_class);
		}
	}

/* fill parent rule pointers.		*/
for (i = 0, srt_ent = *srtptr; i < *no_rules; i++, srt_ent++)
	srt_ent->par_rule = d20_search_adm_srule(*no_rules, *srtptr,
							srt_ent->par_rule_nr);

fclose(fdsch);
return (D2_NOERROR) ;
}


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_search_obj() 				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	caddr_t	d20_search_obj(dsa_adm,no_oct,oct,	      */
/*					obj_abbrv)     	              */
/*		signed16 dsa_adm;				      */
/*		signed32 no_oct;				      */
/*		caddr_t	 oct;				              */
/*		Abbrv	 obj_abbrv;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function searches for an object class in OCT     */
/*		of DSA scheme/admin. scheme.			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		no_oct		no. of elements in OCT 		      */
/*		oct		pointer to OCT 			      */
/*		obj_abbrv	abbreviation of the object class to   */
/*				be searched			      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		pointer to the result OCT entry			      */
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

caddr_t	d20_search_obj(
	signed16 dsa_adm,
	signed32 no_oct,
	caddr_t	 oct,
	Abbrv	 obj_abbrv)
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

char 	*next_abbrv;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* This function uses the binary search algorithm to locate the object
   class entry for a given object class abbreviation. This can be used
   since OCT is sorted on object class abbreviation */

if (no_oct == 0)
	return((caddr_t)NULL);

if (dsa_adm == DSA_SCHEME)
	next_abbrv = oct + (sizeof(Oct) * (no_oct/2));
else
	next_abbrv = oct + (sizeof(D20_oct) * (no_oct/2));

if (strcmp(obj_abbrv,next_abbrv) != 0)
	if (strcmp(obj_abbrv,next_abbrv) < 0)
		return(d20_search_obj(dsa_adm,no_oct/2,oct,obj_abbrv));
	else
		{
		if (dsa_adm == DSA_SCHEME)
			oct = (caddr_t)((Oct *)oct + no_oct/2 + 1);
		else
			oct = (caddr_t)((D20_oct *)oct + no_oct/2 + 1);

		if (no_oct%2 == 0)
			no_oct = no_oct/2 - 1;
		else
			no_oct = no_oct/2;

		return(d20_search_obj(dsa_adm,no_oct,oct,obj_abbrv));
		}
else
	return(next_abbrv);
}


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_search_adm_attr() 				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	D20_at	*d20_search_adm_attr(no_at,at,attr_abbrv)     */
/*		signed32 no_at;					      */
/*		D20_at	*at;				              */
/*		Abbrv	 attr_abbrv;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function searches for an attribute in AT.        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		no_at		no. of elements in AT of admin scheme */
/*		at		pointer to AT of admin schema         */
/*		attr_abbrv	abbreviation of the attribute to be   */
/*				searched			      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		pointer to the result AT entry			      */
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

D20_at	*d20_search_adm_attr(
	signed32  no_at,
	D20_at	 *at,
	char	  attr_abbrv[])
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

/* This function uses the binary search algorithm to locate the attribute
   entry for a given attribute abbreviation. This can be used since AT is 
   sorted on attribute abbreviation */

if (no_at == 0)
	return((D20_at *)NULL);

if (strcmp(attr_abbrv,at[no_at/2].at.att_abbrv) != 0)
	if (strcmp(attr_abbrv,at[no_at/2].at.att_abbrv) < 0)
		return(d20_search_adm_attr(no_at/2,at,attr_abbrv));
	else
		{
		at = at + no_at/2 + 1;
		if (no_at%2 == 0)
			no_at = no_at/2 - 1;
		else
			no_at = no_at/2;

		return(d20_search_adm_attr(no_at,at,attr_abbrv));
		}
else
	return((D20_at *)(at + no_at/2));
}


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_search_adm_srule() 				      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:	D20_srt	*d20_search_adm_srule(no_srt,srt,rule_nr)     */
/*		signed32 no_srt;				      */
/*		D20_srt	 *srt;				              */
/*		signed32 rule_nr;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function searches for a structure rule in adm.   */
/*		SRT.						      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		no_srt		no. of elements in SRT of admin scheme*/
/*		srt		pointer to SRT of admin schema        */
/*		rule_nr		rule_nr to be searched.		      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		pointer to the result SRT entry			      */
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

D20_srt *d20_search_adm_srule(
	signed32  no_srt,
	D20_srt	 *srt,
	signed32  rule_nr)
{
if (no_srt == 0)
	return((D20_srt *)NULL);

if (rule_nr != srt[no_srt/2].rule_nr)
	if (rule_nr < srt[no_srt/2].rule_nr)
		return(d20_search_adm_srule(no_srt/2,srt,rule_nr));
	else
		{
		srt = srt + no_srt/2 + 1;
		if (no_srt%2 == 0)
			no_srt = no_srt/2 - 1;
		else
			no_srt = no_srt/2;

		return(d20_search_adm_srule(no_srt,srt,rule_nr));
		}
else
	return((D20_srt *)(srt + no_srt/2));
}

