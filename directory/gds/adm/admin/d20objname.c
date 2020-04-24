/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20objname.c,v $
 * Revision 1.1.728.2  1996/02/18  19:41:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:04  marty]
 *
 * Revision 1.1.728.1  1995/12/08  15:17:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:10  root]
 * 
 * Revision 1.1.726.4  1994/06/21  14:43:42  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:24:38  marrek]
 * 
 * Revision 1.1.726.3  1994/05/10  15:49:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:12:53  marrek]
 * 
 * Revision 1.1.726.2  1994/03/23  15:05:36  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:10:51  keutel]
 * 
 * Revision 1.1.726.1  1994/02/22  15:58:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:31:55  marrek]
 * 
 * Revision 1.1.724.2  1993/10/14  16:46:35  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:45:00  keutel]
 * 
 * Revision 1.1.724.1  1993/10/13  17:29:49  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  07:36:05  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  10:21:51  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:33:13  marrek]
 * 
 * Revision 1.1.4.7  1993/01/29  08:37:59  marrek
 * 	bug in object class selection corrected.
 * 	[1993/01/28  10:40:27  marrek]
 * 
 * Revision 1.1.4.6  1993/01/25  10:12:46  marrek
 * 	Handle Object Class and Aux. Object Class fields
 * 	[1993/01/25  10:08:53  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  18:09:17  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:07  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/18  08:28:42  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:29:18  marrek]
 * 
 * Revision 1.1.4.3  1992/11/27  13:18:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:13:32  marrek]
 * 
 * Revision 1.1.4.2  1992/09/23  09:24:17  marrek
 * 	Bug fix for 5205. extra_sup_class += ......
 * 	[1992/09/23  09:23:08  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:54:24  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:33:17  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  22:00:10  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:17:02  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20objname.c,v $ $Revision: 1.1.728.2 $ $Date: 1996/02/18 19:41:22 $";
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
/* NAME         : objname.c   [Administrations-Schnittstelle]         */
/*								      */
/* AUTHOR       : Volpers, D AP 11                                    */
/* DATE         : 01.02.88                                            */
/*								      */
/* KOMPONENTE	: DS						      */
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
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 21.08.87| Erstellung                     | ws |         */
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
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define EXTRA_OBJ_CLASS	-1

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern char      *d20_1ifields[];       /* contains options given at  */
					/* logon time                 */
extern char	 *d20_6ifields[] ;	/* Input fields of the mask 6 */
extern D20_omask d20_6omask[] ;		/* Mask 6 */
extern D20_omask d20_qomask[] ;		/* Mask 'quit' */
extern char	 *d20_errtab[] ;	/* Array for error text     */
extern char      *d20_err_table[] ;     /* Array for error text     */
extern char      *d20_errmess[] ;     /* Array for error text     */

extern	D20_oct		*d2_p_d20oct;			     
extern	signed32	d2_nd20oct;			      

extern	D20_srt		*d2_p_d20srt;			     
extern	signed32	d2_nd20srt;			      

extern  Bool		d20_cache;

static 	D20_oct		*dummy_oct_ent = (D20_oct *)NULL;
/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/

 static	signed32 d20_handle_aux_class(D20_oct *oct_ent);


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME:        ds0_501_object_name()                                 */
/*								      */
/* AUTHOR:      Volpers (D AP 11)                                     */
/* DATE:        21.08.87                                              */
/*								      */
/* SYNTAX:      signed16 ds0_501_object_name(scheme_disp,type,	      */
/*                                        operation,rule_nr,tree,     */
/*                                        init_flds,count, 	      */
/*					  oct_ent) 		      */
/*              Bool            scheme_disp;                          */
/*              signed16        type;                                 */
/*              char            operation;                            */
/*		signed32 	rule_nr;            		      */
/*              char            tree[];                               */
/*              Bool            init_flds;                            */
/*              signed16        *count;                               */
/*		D20_oct	       **oct_ent;			      */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function displays the mask containing all object */
/*              names. The values for the object name parts can be    */
/*              inserted.                                             */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool            scheme_disp;    indicates whether     */
/*                                              scheme object should  */
/*                                              be displayed or not   */
/*              signed16        type;           OBJECT oder ALIAS     */
/*              char            operation;      selected  operation   */
/*		signed32 	rule_nr;       rule number of selected*/
/*						structure   	      */
/*              char            tree[];         appropriate tree      */
/*                                              structure             */
/*              Bool            init_flds;      a sign whether the    */
/*                                              input fields must be  */
/*                                              initialized           */
/*              signed16       *count;          number of attributes  */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed16       *count;          number of attributes  */
/*                                              (in case of ACL the   */
/*                                              number will be        */
/*                                              increased)            */
/*		D20_oct		*oct_ent;	pointer to valid obj. */
/*						class entry in case of*/
/*						relevant operation    */
/*						otherwise D2_NULL     */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       error occurred --> check error codes          */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*		D20_oct		*d2_p_d20oct;			      */
/*		signed32	d2_nd20oct;			      */
/*		D20_srt		*d2_p_d20srt;			      */
/*		signed32	d2_nd20srt;			      */
/*								      */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                      ds0_512_object_tree()                         */
/*                      ds4_b01_fill_entry()                          */
/*			d20_handlemask ()			      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

 signed16 ds0_501_object_name(
   Bool       scheme_disp,
   signed16   type,
   char       operation,
   signed32   rule_nr,
   char       tree[],
   Bool       init_flds,
   signed16  *count,
   D20_oct  **oct_ent)

{                            /*  ds0_501_object_name()            */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

   signed16 ret_value = D2_NOERROR;/* Return-Value                    */
   signed16 no_nparts;        /* for base object (search)         */
   signed16 i,k,idx;
   char message[D20_MAXMSG];
   Bool control = TRUE;
   Bool al_flag;            /* used for ADD-ALIAS                 */
   Bool rdn_flag;           /* used for MODIFY-RDN                */
   char *str, *max_str;
   signed32 al_rule_nr;     /* rule number of aliased object    */
   signed32 rdn_rule_nr;    /* rule number of new name in MODRDN */
   signed16 al_anz;         /* number of RDNs of aliased name     */
   signed16 rdn_anz;        /* number of RDNs of new name in MODRDN */
   signed16 ava_cnt;        /* number of AVAs in object name      */
   signed16 obj_anz;

   Ds_at_entry    attr_entry[D20_62CNT + NO_AUX_CLASS + 1];
   Ds_at_entry   *p_attr_entry;

   char           sav_val[D20_62CNT][DS_NP_VAL_LEN + 1];
static Ds_obj_entry   obj_entry[D20_62CNT];
   D20_srt	      *srt_ent;
   D20_srt	      *rdn_srt_ent;	/* srt entry for new name in MODRDN. */	
   D20_srt	      *run_srt_ent;
   signed32	       no_filter; 	/* no. of AVAs involved in filter */
					/* in case of search.		  */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

srt_ent = d20_search_adm_srule(d2_nd20srt, d2_p_d20srt, rule_nr);

/*  fields must be initialized  */
if (init_flds == TRUE)
	{
	/*  initialize fields                 */
  	for (i=0; i < D20_62CNT; i++)
		{
		d20_6omask[D20_62POS + 2*i].d20_fkt_id =
		d20_6omask[D20_62POS + 2*i+1].d20_fkt_id = D20_NO_OP ;
		d20_6ifields[i][0] = D2_EOS;
		}
	}

/* set mask fields and obj_entry pointers to names of naming attributes. */
ds0_512_object_tree(&d20_6omask[D20_62POS],tree,obj_entry,&ava_cnt,D20_MASK6);

obj_anz = ava_cnt;

/* fill the object class names valid for a rule and for relevant operations */
if ((operation == DS_DELETE_OBJECT) || 
    (operation == DS_MODRDN) || (operation == DS_ADD_ALIAS) ||
     (operation == DS_DUMP) || (operation == D20_DELETE_DEFAULT_DSA))
	{
	/* Object class and Auxiliary object class fields should be made */
	/* NO_OP.							 */
	d20_6omask[D20_64POS].d20_fkt_id 	= D20_NO_OP;
	d20_6omask[D20_64POS + 1].d20_fkt_id 	= D20_NO_OP;
	d20_6omask[D20_65POS].d20_fkt_id 	= D20_NO_OP;
	d20_6omask[D20_65POS + 1].d20_fkt_id 	= D20_NO_OP;

	/* initialize input for Object Class and Aux. Object Class fields. */
	*(signed32 *)d20_6omask[D20_64POS + 1].d20_ioinfo = 0;
	*(signed32 *)d20_6omask[D20_65POS + 1].d20_ioinfo = FALSE;
	}
else
	{	
	/* set fields to input object class name and Aux. object class (Y/N). */

	d20_6omask[D20_64POS].d20_fkt_id 	= D20_CSAY;
	d20_6omask[D20_64POS + 1].d20_fkt_id 	= D20_CGET;
	d20_6omask[D20_65POS].d20_fkt_id 	= D20_CSAY;
	d20_6omask[D20_65POS + 1].d20_fkt_id 	= D20_CGET;

	/* free the existing area for object class strings.	*/
	if (d20_6omask[D20_64POS + 1].d20_oformat != (char *)NULL)
		free(d20_6omask[D20_64POS + 1].d20_oformat);

	/* allocate space for object class strings.	*/
	if ((d20_6omask[D20_64POS + 1].d20_oformat = (char *)malloc(
	(srt_ent->no_obj_class + 1) * (L_MAX_NAME + 5) + 4)) == (char *)NULL)
		return(D2_ERROR);

	/*  input fields must be initialized  */
	if (init_flds == TRUE)
		{
		/*  initialize fields                 */
	       	*(signed32 *)d20_6omask[D20_64POS + 1].d20_ioinfo = 0;
	       	*(signed32 *)d20_6omask[D20_65POS + 1].d20_ioinfo = FALSE;
		}
	else
		if (operation != DS_DISPLAY_OBJECTS &&
	    	     *(signed32 *)d20_6omask[D20_64POS + 1].d20_ioinfo == 
		     EXTRA_OBJ_CLASS)
			/*  initialize fields                 */
	       		*(signed32 *)d20_6omask[D20_64POS + 1].d20_ioinfo = 0;

	/* generate object class names in the format string.	*/
	strcpy(d20_6omask[D20_64POS + 1].d20_oformat,"%");
	for (i=0; i<srt_ent->no_obj_class; i++)
		sprintf(d20_6omask[D20_64POS + 1].d20_oformat + 
			strlen(d20_6omask[D20_64POS + 1].d20_oformat),
			".%d=%-*.*s",
			i,L_MAX_NAME,strlen(srt_ent->obj_class[i]->obj_name),
				     srt_ent->obj_class[i]->obj_name);

	if (operation == DS_DISPLAY_OBJECTS)
		sprintf(d20_6omask[D20_64POS + 1].d20_oformat + 
			strlen(d20_6omask[D20_64POS + 1].d20_oformat),
			".%d=%-*.*s%s",
			EXTRA_OBJ_CLASS,L_MAX_NAME,strlen("*"),"*",".a");
	else
		if (operation == DS_MODIFY_ATTRIBUTE)
			{
			/* search for Aliased Object Class entry in D20_oct */
			for (i=0; i<d2_nd20oct; i++)
				{
				*oct_ent = d2_p_d20oct + i;
     				if (d20_cmp_objid(&((*oct_ent)->obj_id),
				   sizeof(D2_OALIAS),(Ob_id_string) D2_OALIAS))
					break;
				}
			sprintf(d20_6omask[D20_64POS + 1].d20_oformat + 
			strlen(d20_6omask[D20_64POS + 1].d20_oformat),
			".%d=%-*.*s%s",EXTRA_OBJ_CLASS,L_MAX_NAME,
			strlen((*oct_ent)->obj_name),(*oct_ent)->obj_name,".a");
			}
		else
			strcpy(d20_6omask[D20_64POS + 1].d20_oformat + 
       			strlen(d20_6omask[D20_64POS + 1].d20_oformat),".a");
	}

rdn_flag = al_flag = FALSE;

do
  	{
	/*  set fields of object name mask   */

	*d20_6ifields[DS_OB_EXEC] = *d20_6ifields[DS_OB_MORE] =
	*d20_6ifields[DS_OB_BREAK] = DS_DEFAULT_VALUE ;

	/* set Type of object  field of Object name mask.	*/
	switch((int)type)
		{
		case D20_OBJECT:
			d20_6omask[D20_610POS].d20_fkt_id = D20_CSAY;
			d20_6omask[D20_611POS].d20_fkt_id = D20_NO_OP;
			d20_6omask[D20_612POS].d20_fkt_id = D20_NO_OP;
			d20_6omask[D20_613POS].d20_fkt_id = D20_NO_OP;
			break;

		case D20_RDN:
			if (rdn_flag == FALSE)
				{
				d20_6omask[D20_610POS].d20_fkt_id = D20_CSAY;
				d20_6omask[D20_613POS].d20_fkt_id = D20_NO_OP;
				}
			else
				{
				/*  display tree structure of objects */
				/*  input tree not ok                 */
	   			if (ds0_500_objects(scheme_disp, D20_NEW_OBJ,
							&rdn_rule_nr, tree))
					{
					/*  break                             */
					ret_value = D2_ERROR;
					break;
					}

				/* check validity of new rule no.	    */
				/* new rule and old rule should have same   */
				/* parent and they should have at least one */
				/* object class common among them.	    */
				rdn_srt_ent = d20_search_adm_srule(d2_nd20srt, 
						d2_p_d20srt, rdn_rule_nr);

				if (rdn_srt_ent->par_rule_nr != 
							srt_ent->par_rule_nr)
					{
					d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errmess[D20_E56IDX];
					d20_handlemask (d20_qomask) ;
					control = FALSE;
					break;
					}

				for (i=0; i<srt_ent->no_obj_class; i++)
					{
					for (k=0; k<rdn_srt_ent->no_obj_class; 
								k++)
						if (strcmp(srt_ent->obj_class[i]
							->obj_abbrv, 
							rdn_srt_ent->obj_class[
							k]->obj_abbrv) == 0)
							break;

					if (k != rdn_srt_ent->no_obj_class)
						/* match found.	*/
						break;
					}

				if (i == srt_ent->no_obj_class)
					/* there is no common object class. */
					{
					d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errmess[D20_E56IDX];
					d20_handlemask (d20_qomask) ;
					control = FALSE;
					break;
					}
					
				/* prepare object name mask.	*/
				d20_6omask[D20_610POS].d20_fkt_id = D20_NO_OP;
				d20_6omask[D20_613POS].d20_fkt_id = D20_CSAY;
				
				/* set mask fields and obj_entry  */
				/* pointers to names of naming    */
				/* attributes. 			  */

	  			ds0_512_object_tree(&d20_6omask[D20_62POS],tree,
					obj_entry,&rdn_anz,D20_MASK6);

				/* make all fields except the ones for last  */
				/* RDN, D20_CSAY, so that only the last RDN  */
				/* can be entered.			     */
				
				for (i = D20_62POS + 1, 
				idx = i + 2 * (ava_cnt - srt_ent->no_nam_attr);
				i < idx; i += 2)
	  				d20_6omask[i].d20_fkt_id = D20_CSAY;

				if (rdn_anz < ava_cnt)
					{
					/* extra fields of original object */
					/* name mask, should be made NO_OP */
					for (i = rdn_anz; i < ava_cnt; i++)
						{
						d20_6omask[D20_62POS + 2*i].
						d20_fkt_id = 
						d20_6omask[D20_62POS + 2*i+1].
						d20_fkt_id = D20_NO_OP ;
						}

					}

	  			ava_cnt = rdn_anz;
				}

		  	d20_6omask[D20_611POS].d20_fkt_id = D20_NO_OP;
			d20_6omask[D20_612POS].d20_fkt_id = D20_NO_OP;
			break;

		case D20_ALIAS:
		 	d20_6omask[D20_610POS].d20_fkt_id = D20_NO_OP;
			d20_6omask[D20_613POS].d20_fkt_id = D20_NO_OP;

      			if (al_flag == FALSE)
				{
				d20_6omask[D20_611POS].d20_fkt_id = D20_CSAY;
				d20_6omask[D20_612POS].d20_fkt_id = D20_NO_OP;
				}
			else
				{
				d20_6omask[D20_611POS].d20_fkt_id = D20_NO_OP;
				d20_6omask[D20_612POS].d20_fkt_id = D20_CSAY;

				/* handle object name of Aliased object.    */

				/*  display tree structure of objects */
				/*  input tree not ok                 */
	   			if (ds0_500_objects(scheme_disp, D20__ALIASED,
							&al_rule_nr, tree))
					{
					/*  break                             */
					ret_value = D2_ERROR;
					break;
					}

				for (i = 0; i < D20_62CNT; i++)
					{
					d20_6omask[D20_62POS + 2*i].d20_fkt_id 
					= d20_6omask[D20_62POS + 2*i+1].
					d20_fkt_id = D20_NO_OP ;
					}

				/* set mask fields and obj_entry  */
				/* pointers to names of naming    */
				/* attributes. 			  */

	  			ds0_512_object_tree(&d20_6omask[D20_62POS],tree,
					obj_entry,&al_anz,D20_MASK6);

	  			ava_cnt = al_anz;

				}	/* end of al_flag is TRUE.	*/
			break;
		}	/* end of switch.	*/

	if (ret_value == D2_ERROR)
		break;

      	/*  error occured                     */
	if (control == FALSE)
      		{
		/*  set indicator for further input   */
		control = TRUE;
		continue;
		}


	/* take input from object name mask.	*/
	d20_handlemask (d20_6omask) ;

    	/*  "break" was selected              */
      	if (*d20_6ifields[DS_OB_BREAK] != DS_DEFAULT_VALUE)
    		{
		/*  return value = -1                 */
		/*  break                             */
		ret_value = D2_ERROR;
		break;
		}

	/* check correctness of attribute value assertions (AVAs).	*/
      	for (i = 0; i < ava_cnt; i++)
		{
		obj_entry[i].obj_real_len = d20_strlen(d20_6ifields[i]);

		/*  length == 0                       */
	  	if (obj_entry[i].obj_real_len == 0)
			{
			/*  display error message             */
			/*  break                             */
			d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errtab[D20_E60IDX];
			d20_handlemask (d20_qomask) ;
			control = FALSE;
			break;
			}

		/*  length > maximal name part length */
	  	if (obj_entry[i].obj_real_len > obj_entry[i].obj_length)
			{
			/*  display error message             */
			/*  break                             */
			sprintf(message, d20_errtab[D20_E04IDX],i+1);
			d20_qomask[D20_Q1POS].d20_ioinfo = message ;
			d20_handlemask (d20_qomask) ;
			control = FALSE;
			break;
			}
		}

      	/*  error occured                     */
	if (control == FALSE)
      		{
		/*  set indicator for further input   */
		control = TRUE;
		continue;
		}

	/* check correctness of usage of '*' in AVA.	*/
      	/*  all name parts                    */
	for (i=0, no_nparts = ava_cnt; i < ava_cnt; i++)
		{
		for (str = d20_6ifields[i],
	          max_str = str+obj_entry[i].obj_real_len; str < max_str; str++)
			{
	  		if (*str == '*')
				{
	  			/*  i == number of nameparts of the  */
			      	/*  base object                      */
	    			if (operation == DS_DISPLAY_OBJECTS)
					{
					/* WILDCARD is in the start || end */
					/* => no_nparts fixed, and filter  */
					/* should be generated.		   */
					if (str == d20_6ifields[i] ||
					    str == (max_str - 1))
						{
						no_nparts = i ;
						break;
						}
					}
				else
					{
					d20_qomask[D20_Q1POS].d20_ioinfo = 
						d20_err_table[D20_E16IDX];
					d20_handlemask (d20_qomask) ;
					control = FALSE;
					break;
					}
				}	/* char is WILDCARD.	*/
			}	/* end of analysis of one name part.	*/

		if (control == FALSE)
	  		break;

		/* first '*' found.	*/
		if (no_nparts != ava_cnt)
	  		break;

		}	/* end of analysis of all name parts.	*/

      	/*  error occured                     */
	if (control == FALSE)
		{
      		/*  set indicator for further input   */
		control = TRUE;
		continue;
		}

	if ((operation == DS_DELETE_OBJECT) || 
	    (operation == DS_MODRDN) || (operation == DS_ADD_ALIAS) || 
	    (operation == DS_DUMP) || (operation == D20_DELETE_DEFAULT_DSA))
		*oct_ent = (D20_oct *)NULL;
	else
		if (*(signed32 *)d20_6omask[D20_64POS + 1].d20_ioinfo ==
							EXTRA_OBJ_CLASS)
			{
			if (operation == DS_DISPLAY_OBJECTS)
				*oct_ent = (D20_oct *)NULL;

			/* in case it is modify attribute operation */
			/* *oct_ent is already set to Alias object  */
			/* class entry.	     			    */
			}
		else
			*oct_ent = srt_ent->obj_class[
			*(signed32 *)(d20_6omask[D20_64POS + 1].d20_ioinfo)];

	if (d20_6omask[D20_65POS + 1].d20_fkt_id == D20_CGET)
		{
		/* if aux. class field is operational,			     */
		/* check correctness of combination of Object class selected */
		/* and Auxiliary object class selection (y/n).		     */

		if ((*oct_ent == (D20_oct *)NULL) &&
		    (*(signed32 *) d20_6ifields[DS_OB_AUXOCLASS] == TRUE))
			{
			d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errmess[D20_E55IDX];
			d20_handlemask (d20_qomask) ;
			continue;
			}

		if (*oct_ent != (D20_oct *)NULL)
			if (((*oct_ent)->no_aux_class == 0) && 
			    (*(signed32 *) d20_6ifields[DS_OB_AUXOCLASS]
							== TRUE))
				{
				d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errmess[D20_E55IDX];
				d20_handlemask (d20_qomask) ;
				continue;
				}

		if (*(signed32 *) d20_6ifields[DS_OB_AUXOCLASS] == TRUE)
			if (d20_handle_aux_class(*oct_ent) == D2_NOERROR)
				*oct_ent = dummy_oct_ent;
			else
				{
				ret_value = D2_ERROR;
				break;
				}
		}

	switch ((int) operation)
		{
		case DS_ADD_OBJECT:
		case DS_DELETE_OBJECT:
		case DS_ADD_ATTRIBUTE:
		case DS_DELETE_ATTRIBUTE:
		case DS_MODIFY_ATTRIBUTE:
		case D20_DELETE_DEFAULT_DSA:
		case DS_DUMP:
			ds4_b01_fill_entry(D20_OBJECT, obj_entry, ava_cnt,tree);
			control = FALSE;
			break;

		case DS_DISPLAY_OBJECTS:
	    		/*  Cache was selected                */
	       		if (*((signed32 *) d20_1ifields[DS_LO_OPTIONS]) == 
								DS_L_DCACHEOPT)
				{
	      			if ((no_nparts != ava_cnt) || 
				    (*oct_ent !=(D20_oct *)NULL))
					/* filter is going to be generated, */
					/* search will be called. so set base */
					/* object as root.		     */
					no_nparts = 0;
				}

			control = FALSE;

			no_filter = 0;

			/*  enter filter pattern         */
    			if (no_nparts  < ava_cnt)  
				{
      				/*  Cache was selected           */
	 			if (*((signed32 *) d20_1ifields[DS_LO_OPTIONS])
						== DS_L_DCACHEOPT)
	      				/*  base object is root       */
					d20_set_subset(D2_WHOLE_SUBTREE);
				else
	      				if (no_nparts >= 
					    ava_cnt - srt_ent->no_nam_attr)
		 				d20_set_subset(D2_ONE_LEVEL);
					else
		 			       d20_set_subset(D2_WHOLE_SUBTREE);

				/* compute no. of filter items.	*/
				run_srt_ent = srt_ent;
				while (no_nparts < ava_cnt - no_filter)
					{
					no_filter = no_filter + 
						    run_srt_ent->no_nam_attr;
					run_srt_ent = run_srt_ent->par_rule;
					}

				/* set correct value of no_nparts in base */
				/* object.				  */
				no_nparts = ava_cnt - no_filter;

				/* fill p_attr_entry with AVAs which should */
				/* be part of filter.			    */
	      			for (p_attr_entry = attr_entry, 
				     i = ava_cnt - no_filter; i < ava_cnt; 
				     p_attr_entry++, i++)
					{
					p_attr_entry->attr_id.d2_type = 
					obj_entry[i].attr_id.d2_type;
					p_attr_entry->attr_id.d2_typ_len = 
					obj_entry[i].attr_id.d2_typ_len;
					p_attr_entry->a_val.d2_a_v_len =
					obj_entry[i].obj_real_len;
					if (d20_6ifields[i][0] == '*' &&
					    p_attr_entry->a_val.d2_a_v_len == 1)
						p_attr_entry->a_val.d2_a_v_len 
						= 0;
					else
						{
						p_attr_entry->a_val.
						d2_at_value.d2_oct_a_v =
						 (Octet_string)d20_6ifields[i];
						p_attr_entry->a_val.d2_a_rep = 
						obj_entry[i].rep;
						}

					p_attr_entry->hex_use = FALSE;
					}
				}
			else
	      			d20_set_subset (D2_BASE_OBJECT);

	    		if (*oct_ent != (D20_oct *)NULL)
				{
				/* object class was selected, 	*/
				/* generate the filter item from the object */
				/* class and the auxiliary object classes if*/
				/* any selected.    			    */

				p_attr_entry = &attr_entry[no_filter++];
				p_attr_entry->attr_id.d2_type = 
						(Ob_id_string)D2_OBJ_CLASS;
				p_attr_entry->attr_id.d2_typ_len = 
						sizeof(D2_OBJ_CLASS) - 1; 
				p_attr_entry->hex_use = FALSE;       					p_attr_entry->a_val.d2_a_rep =
				d27_401_syntax_to_rep((signed16)D2_OBJ_IDENT);
				p_attr_entry->a_val.d2_a_v_len = 
						(*oct_ent)->obj_id.d2_typ_len;
				p_attr_entry->a_val.d2_at_value.d2_oct_a_v = 
						(*oct_ent)->obj_id.d2_type;

				/* if some auxiliary object classes have been */
				/* selected.				      */
				if (*(signed32 *) d20_6ifields[DS_OB_AUXOCLASS]
								== TRUE)
					{
					for (i=0; i<(*oct_ent)->no_aux_class; 
						  i++)
						{
						if ((*oct_ent)->aux_class[i] !=
							(D20_oct *)NULL)
							{
							p_attr_entry = 
						       &attr_entry[no_filter++];
							p_attr_entry->attr_id.
							d2_type = 
						     (Ob_id_string)D2_OBJ_CLASS;
							p_attr_entry->attr_id.
							d2_typ_len = 
							sizeof(D2_OBJ_CLASS) -1;
							p_attr_entry->hex_use = 
							FALSE;
							p_attr_entry->a_val.
							d2_a_rep =
							d27_401_syntax_to_rep(
							(signed16)D2_OBJ_IDENT);
							p_attr_entry->a_val.
							d2_a_v_len = 
							(*oct_ent)->aux_class[i]
							->obj_id.d2_typ_len;
							p_attr_entry->a_val.
							d2_at_value.d2_oct_a_v =							(*oct_ent)->aux_class[i]
							->obj_id.d2_type;
							}
						}
					}
				}

			ds4_b01_fill_entry(D20_OBJECT,obj_entry,no_nparts,tree);
	    		d20_ffilter(D20_BEGIN, attr_entry, no_filter);

	  		break;

		case DS_ADD_ALIAS:
	    		if (al_flag == FALSE)
				{
	    			/* copy alias name                    */
	      			ds4_b01_fill_entry(D20_ALIAS, obj_entry,
								ava_cnt, tree);
	      			al_flag = TRUE;

				/* store input fields of alias name   */
	      			for (i = 0; i < ava_cnt; i++)
					strcpy(&sav_val[i][0], d20_6ifields[i]);
				}
			else
	    			{
				/* copy aliased name                  */
				ds4_b01_fill_entry(D20_OBJECT,obj_entry,
								al_anz, tree);
				control = FALSE;
				}

	    		break;

		case DS_MODRDN:
	    		if (rdn_flag == FALSE)
	    			{
				/* copy old name                      */
				ds4_b01_fill_entry(D20_OBJECT,obj_entry,
								ava_cnt, tree);
				rdn_flag = TRUE;

				/* store input fields of object name */
	      			for (i = 0; i < ava_cnt; i++)
					strcpy(&sav_val[i][0], d20_6ifields[i]);
				}
			else
				{
				rdn_srt_ent = d20_search_adm_srule(d2_nd20srt, 
						d2_p_d20srt, rdn_rule_nr);
	    			/* copy new name                 */
				ds4_b01_fill_entry(D20_RDN, 
				&obj_entry[rdn_anz - rdn_srt_ent->no_nam_attr],
				rdn_srt_ent->no_nam_attr, tree);
				control = FALSE;
				}
	    		break;

		default:
			break;
		}

	}
while (control == TRUE);


/*  operation is MODIFY-RDN           */
if ((rdn_flag == TRUE) || (al_flag == TRUE))
	{
	/* restore fields of orginal object name.	*/
  	for (i = 0; i < obj_anz; i++)
		{
    		strcpy(d20_6ifields[i], &sav_val[i][0]);

    		for (k = strlen(&sav_val[i][0]); k < DS_NP_VAL_LEN; k++)
      			d20_6ifields[i][k] = DS_DEFAULT_VALUE;
		}

  	for (i = obj_anz; i < ava_cnt; i++)
		{
		d20_6omask[D20_62POS + 2*i].d20_fkt_id =
		d20_6omask[D20_62POS + 2*i+1].d20_fkt_id = D20_NO_OP ;
		d20_6ifields[i][0] = D2_EOS;
		}
	}
				/*  return(ret_value)                 */
return(ret_value);

}                             /*  ds0_501_object_name               */

/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME:        d20_handle_aux_class()                                */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        21.08.87                                              */
/*								      */
/* SYNTAX:      signed16 d20_handle_aux_class(oct_ent)		      */
/*		D20_oct	*oct_ent;				      */
/*								      */
/* DESCRIPTION:                                                       */
/*		This function calls the function to display aux. obj. */
/*		classes corresponding to object class selected 	      */
/*		(oct_ent). This function after knowing the selected   */
/*		auxiliary object classes, builds a dummy_oct_ent in   */
/*		which selected aux. object classes occur as super     */
/*		classes and their mand. and opt. attributes are merged*/
/*		with original oct_ent.				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		D20_oct		*oct_ent;			      */
/*					original Oct entry.	      */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*		D2_ERROR	when d20_call_disp_aux function       */
/*				returns D2_ERROR or	      	      */
/*				in case of problem in malloc for      */
/*				dummy_oct_ent.			      */
/*		D2_NOERROR	otherwise.			      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*								      */
/* GLOBAL DATA (updated):                                             */
/*		dummy_oct_ent	contains pointer to newly created     */
/*				dummy Oct entry.		      */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*		d20_call_disp_aux().			      	      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

 static	signed32 d20_handle_aux_class(
   D20_oct *oct_ent)

{                            /*  d20_handle_aux_class()            */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

Bool	bool_array[NO_AUX_CLASS];
signed32 extra_sup_class, extra_attr;
signed32 i, j, k, l;
D20_oct	**oct_ent_dptr;
D20_at	**at_ent_dptr;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* release previously allocated memory.		*/
if (dummy_oct_ent != (D20_oct *)NULL)
	{
	free(dummy_oct_ent);
	dummy_oct_ent = (D20_oct *)NULL;
	}

if (d20_call_disp_aux(oct_ent, bool_array) == D2_ERROR)
	return(D2_ERROR);

/* compute how many extra super classes (selected aux. classes + their super */
/* classes) and how many extra attributes (attributes of selected aux. 	     */
/* classes) could be added to oct_ent to make dummy_oct_ent.		     */

extra_sup_class = extra_attr = 0;

for (i=0; i<oct_ent->no_aux_class; i++)
	if (bool_array[i] == TRUE)
		{
		extra_sup_class += 1 + oct_ent->aux_class[i]->no_sup_class;
		extra_attr += oct_ent->aux_class[i]->no_must_attr +
			      oct_ent->aux_class[i]->no_may_attr;
		}

/* allocate space for an Oct entry to which dummy_oct_ent points and space for*//* all super classes and all attributes that belong to dummy_oct_ent.	      */

if ((dummy_oct_ent = (D20_oct *)malloc(sizeof(D20_oct) + 
		     (sizeof(D20_oct *) * (oct_ent->no_sup_class + 
		     extra_sup_class)) + 
		     (sizeof(D20_at *) * (oct_ent->no_must_attr + 
		     oct_ent->no_may_attr + extra_attr)))) == 
		     (D20_oct *)NULL)
	return(D2_ERROR);

oct_ent_dptr = (D20_oct **)(dummy_oct_ent + 1);
at_ent_dptr = (D20_at **)(oct_ent_dptr + oct_ent->no_sup_class + 
			  extra_sup_class);

dummy_oct_ent->obj_id.d2_typ_len = oct_ent->obj_id.d2_typ_len;
dummy_oct_ent->obj_id.d2_type = oct_ent->obj_id.d2_type;

dummy_oct_ent->no_sup_class = oct_ent->no_sup_class;
dummy_oct_ent->sup_class = oct_ent_dptr;

/* copy sup. class pointers of oct_ent to dummy_oct_ent.	*/
for (i=0; i<oct_ent->no_sup_class; i++)
	*oct_ent_dptr++ = oct_ent->sup_class[i];	

dummy_oct_ent->no_aux_class = oct_ent->no_aux_class;

dummy_oct_ent->no_must_attr = oct_ent->no_must_attr;
dummy_oct_ent->must_attr = at_ent_dptr;

/* copy mand. attr. pointers of oct_ent to dummy_oct_ent.	*/
for (i=0; i<oct_ent->no_must_attr; i++)
	*at_ent_dptr++ = oct_ent->must_attr[i];	

/* collect mandatory attributes from selected aux. object classes.	*/

/* for all auxiliary object classes.	*/
for (i=0; i<oct_ent->no_aux_class; i++)	
	/* this auxiliary object class has been selected.	*/
	if (bool_array[i] == TRUE)
		{
		dummy_oct_ent->aux_class[i] = oct_ent->aux_class[i];

		/* check if this selected aux. object class is already in the */
		/* list of super classes.				      */
		for (j=0; j<dummy_oct_ent->no_sup_class; j++)
			if (oct_ent->aux_class[i] ==dummy_oct_ent->sup_class[j])
				break;

		/* aux. class is not in the list of super classes.	*/
		if (j == dummy_oct_ent->no_sup_class)
			{
			/* put aux. class in the list of super classes of */
			/* dummy_oct_ent.				  */
			*oct_ent_dptr++ = oct_ent->aux_class[i];

			/* put super classes of this aux. class in the list */
			/* of super classes of dummy_oct_ent.		    */
			for (k=0; k<oct_ent->aux_class[i]->no_sup_class; k++)
				{
				for (l=0; l<dummy_oct_ent->no_sup_class; l++)
					if (oct_ent->aux_class[i]->sup_class[k]
					    == dummy_oct_ent->sup_class[l])
						break;

				if (l == dummy_oct_ent->no_sup_class)
					*oct_ent_dptr++ = 
					oct_ent->aux_class[i]->sup_class[k];
				}

			/* put mandatory attributes of aux. class in the list */
			/* of mandatory attributes of dummy_oct_ent.	      */
			for (k=0; k<oct_ent->aux_class[i]->no_must_attr; k++)
				{
				for (l=0; l<dummy_oct_ent->no_must_attr; l++)
					if (oct_ent->aux_class[i]->must_attr[k]
					    == dummy_oct_ent->must_attr[l])
						break;

				if (l == dummy_oct_ent->no_must_attr)
					*at_ent_dptr++ = 
					oct_ent->aux_class[i]->must_attr[k];
				}

			dummy_oct_ent->no_must_attr = at_ent_dptr - 
						      dummy_oct_ent->must_attr;

			}
		else
			/* this aux. object class already exists as sup.  */
			/* object class.				  */
			/* make this aux. object class as good as not selected*/
			/* aux. object class.				      */			bool_array[i] = FALSE;

		}
	else
		dummy_oct_ent->aux_class[i] = (D20_oct *)NULL;

dummy_oct_ent->no_sup_class = oct_ent_dptr - dummy_oct_ent->sup_class;

dummy_oct_ent->no_may_attr = 0;
dummy_oct_ent->may_attr = at_ent_dptr;

/* copy those optional attributes of oct_ent to dummy_oct_ent which have not */
/* come as mandatory attributes from selected object classes.		     */
for (i=0; i<oct_ent->no_may_attr; i++)
	{
	/* check against those mand. attributes which have come from selected */
	/* aux. object classes.						      */

	for (j=oct_ent->no_must_attr; j<dummy_oct_ent->no_must_attr; j++)
		if (oct_ent->may_attr[i] == dummy_oct_ent->must_attr[j])
			break;

	if (j == dummy_oct_ent->no_must_attr)
		*at_ent_dptr++ = oct_ent->may_attr[i];
	}

dummy_oct_ent->no_may_attr = at_ent_dptr - dummy_oct_ent->may_attr;

/* for all auxiliary object classes.	*/
for (i=0; i<oct_ent->no_aux_class; i++)	
	/* this auxiliary object class has been selected.	*/
	if (bool_array[i] == TRUE)
		{
		/* check if optional attribute already exist as mandatory  */
		/* attribute in dummy_oct_ent.			 	   */
		for (j=0; j<oct_ent->aux_class[i]->no_may_attr; j++)
			{
			for (k=0; k<dummy_oct_ent->no_must_attr; k++)
				if (dummy_oct_ent->must_attr[k] == 
					oct_ent->aux_class[i]->may_attr[j])
					break;

			/* it does not occur as mand. attribute.	*/
			if (k == dummy_oct_ent->no_must_attr)
				{
				/* check if optional attribute already exist */
				/* as optional attribute in dummy_oct_ent    */

				for (l=0; l<dummy_oct_ent->no_may_attr; l++)
					if (dummy_oct_ent->may_attr[l] ==
					    oct_ent->aux_class[i]->may_attr[j])
						break;

				if (l == dummy_oct_ent->no_may_attr)
					*at_ent_dptr++ = 
					oct_ent->aux_class[i]->may_attr[j];
				}
			}

		dummy_oct_ent->no_may_attr = at_ent_dptr - 
						dummy_oct_ent->may_attr;
		}

return(D2_NOERROR);

}	/* end of d20_handle_aux_class().	*/
