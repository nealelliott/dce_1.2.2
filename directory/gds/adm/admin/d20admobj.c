/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20admobj.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:28  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:45  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:43:31  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:23:10  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:10:22  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:11  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:08:21  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:27:28  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:57:16  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:26:53  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:33:44  marrek
 * 	Changes for bug 6573.
 * 	[1993/01/28  10:23:11  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:49:11  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:47  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:08:52  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:07:01  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:52:29  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:15:04  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20admobj.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:31 $";
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
/* NAME         : admobj.c    [Administrations-Schnittstelle]         */
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
/*                                                                    */
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
#include <sys/types.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <cget.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

typedef struct
	{
	Ds_empty_pos	*page_beg;
	signed32	page_size;
	} Page_info;

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern char	 *d20_5ifields[] ;	/* Input fields for mask 5    */
extern D20_omask d20_5omask[] ;		/* Mask 5 */
extern D20_omask d20_qomask[] ;		/* Mask 'quit' */
extern char	 *d20_errtab[] ;	/* Array for error messages     */
extern	D20_srt		*d2_p_d20srt;
extern	signed32	d2_nd20srt;

 Bool   d20_obj_list = FALSE;  /* a flag indicating whether  */
					/* the ADMIN object structures*/
					/* are already built          */
 signed32  d20_init_rule_nr = 0;        /* default value of object-   */
					/* number for changing name-  */
					/* structure                  */

static 	char	shift_arr[NAM_ATTR_OFFSET + 1] = "   ";

static  struct 
	{
	signed32	no_of_pages;
	Page_info	*page_info;
	} srt_list_info;

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME:        ds0_500_objects()                                     */
/*								      */
/* AUTHOR:      Schmid  (D AP 11)                                     */
/* DATE:        19.08.87                                              */
/*								      */
/* SYNTAX:      signed16 ds0_500_objects(scheme_disp,type,rule_nr,    */
/*						tree)                 */
/*              Bool            scheme_disp;                          */
/*              signed16        type;                                 */
/*              signed32       *rule_nr;                              */
/*              char            tree[];                               */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function displays the SRT on the screen.         */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool            scheme_disp;    indicates whether     */
/*                                              scheme object should  */
/*                                              be displayed          */
/*              signed16        type;           D20_OBJECT / D20_ALIAS*/
/*					  	D20__ALIASED/ D20_RDN */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed32      *rule_nr;       selected rule no        */
/*              char            tree[];         appropriate tree      */
/*                                              structure             */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       error occurred --> check error codes          */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*		D20_srt		*d2_p_d20srt;			      */
/*		signed32	d2_nd20srt;			      */
/*								      */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*			d20_handlemask ()			      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

signed16 ds0_500_objects( Bool      scheme_disp,
			  signed16  type,
			  signed32 *rule_nr,
			  char      tree[] )
{                            /*  ds0_500_objects()                 */

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

   D20_srt *next;
   D20_srt *parent;
   signed16 i,j,k;             /* loop-variables                     */
   signed16 pos;
   Bool  hit = FALSE;
   Ds_empty_pos  *val;
   signed32 current_page = 0;
   signed32 min_no_rules_ppage;
   signed32 max_no_pages;
   signed32 curr_page_size;
   signed32 fkt_key;
   signed32 sav_root_fkt_id;
   signed16	ret_value = D2_NOERROR;

				/* position of scheme object          */
static Ds_empty_pos  *sch_pos = (Ds_empty_pos *) NULL;
static Ds_empty_pos  *sav_val = (Ds_empty_pos *) NULL;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (d20_obj_list == FALSE)
	{
  	if ((char *) sav_val != NULL)
  		free((char *) sav_val);

	/*  allocate memory and enter all     */
	/*  all Rules from SRT to table     */
  	if ((val = sav_val = (Ds_empty_pos *) calloc((size_t)d2_nd20srt, 
				     (size_t)sizeof(Ds_empty_pos))) == 0)
  		{
		/*  display error message             */
		/*  return(D2_ERROR)                  */
    		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E73IDX] ;
    		d20_handlemask (d20_qomask) ;
    		return(D2_ERROR);
		}

	/* handle SRT page info.	*/
	if ((char *)srt_list_info.page_info != NULL)
		free ((char *)srt_list_info.page_info);

	min_no_rules_ppage = D20_52CNT/D2_NO_AVA;
	max_no_pages = d2_nd20srt/min_no_rules_ppage + 1;

	if ((srt_list_info.page_info = (Page_info *) 
		calloc((size_t)max_no_pages, (size_t)sizeof(Page_info))) == 0)
		{
		/*  display error message             */
		/*  return(D2_ERROR)                  */
    		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errtab[D20_E73IDX] ;
    		d20_handlemask (d20_qomask) ;
    		return(D2_ERROR);
		}

	/* fill structure rule list.	*/
     	for (i = 0, next = d2_p_d20srt ; i < d2_nd20srt; i++, next++)
  		{
		/*  fill table                        */
    		if (next->no_obj_class != 0)
			/* at least one object class is there for this rule.*/
    			if (d20_cmp_objid(&(next->obj_class[0]->obj_id),
					sizeof(D2_OSCHEMA),
					(Ob_id_string) D2_OSCHEMA))
      				sch_pos = val;

    		if (d20_init_rule_nr == 0 && 
		    (scheme_disp == TRUE || sch_pos != val))
      			d20_init_rule_nr = *(signed32 *)d20_5ifields[DS_OC_CLASS] =
								next->rule_nr;

		sprintf(val->ds_numb,"%02d", next->rule_nr);

		val->ds_no_nam_attr = next->no_nam_attr;
		sprintf(val->ds_nam_attr[0],"%-*.*s", DS_NP_NM_LEN, 
			DS_NP_NM_LEN, next->nam_attr[0]->at.att_name);

		/* other naming attributes should be displayed with 	*/
		/* offset in the mask.					*/
		for (j=1; j<next->no_nam_attr; j++)
			{
			strcpy(val->ds_nam_attr[j], shift_arr);
			sprintf(&(val->ds_nam_attr[j][NAM_ATTR_OFFSET]),
					"%-*.*s", DS_NP_NM_LEN, DS_NP_NM_LEN, 
					next->nam_attr[j]->at.att_name);
			}

		sprintf(tree,"%02d",next->rule_nr);
		parent = next->par_rule;

    		while (parent != NULL)
    			{
			/*  search them                       */
			/*  build tree structure              */
      			sprintf(tree + strlen(tree),"-%02d",parent->rule_nr);
      			parent = parent->par_rule;
			}

		/*  reverse tree structure            */
    		for (k=0,j = strlen(tree);j > 0; j -= 3)
			{
			val->ds_tree[k++] = *(tree+j-2);
			val->ds_tree[k++] = *(tree+j-1);
			val->ds_tree[k++] = *(tree+j-3);
			}

		val->ds_tree[k - 1] = '\0';
		val++;
		}

	/* fil SRT page info.	*/

	srt_list_info.no_of_pages = 0;
	for (i=0, current_page = 0, val=sav_val; i<d2_nd20srt; current_page++)
		{
		(srt_list_info.page_info + current_page)->page_beg = val;
		(srt_list_info.page_info + current_page)->page_size = 0;

		for (j = 0; j < D20_52CNT; )
			{
			/* still rules left.	*/
			if (i < d2_nd20srt)
				{
				if ((D20_52CNT - j) >= val->ds_no_nam_attr)
					{
					j += val->ds_no_nam_attr;
					i++;
					val++;
					(srt_list_info.page_info + 
					current_page)->page_size++;
					}
				else
					break;
				}
			else
				break;
			}
		}

	srt_list_info.no_of_pages = current_page;

	d20_obj_list = TRUE;
	}

/* set type of object name.	*/
d20_5omask[D20_510POS].d20_fkt_id = d20_5omask[D20_511POS].d20_fkt_id = 
d20_5omask[D20_512POS].d20_fkt_id = d20_5omask[D20_513POS].d20_fkt_id = 
								D20_NO_OP;

switch ((int)type)
	{
	case D20_OBJECT:
	case D20_RDN:
		d20_5omask[D20_510POS].d20_fkt_id = D20_CSAY;
		break;
	case D20_ALIAS:
		d20_5omask[D20_511POS].d20_fkt_id = D20_CSAY;
		break;
	case D20__ALIASED:
		d20_5omask[D20_512POS].d20_fkt_id = D20_CSAY;
		break;
	case D20_NEW_OBJ:
		d20_5omask[D20_513POS].d20_fkt_id = D20_CSAY;
		break;
	}

/* loop to display pages of SRT.	*/
for (current_page=0, i=0, sav_root_fkt_id=d20_5omask[D20_5ROOT_POS].d20_fkt_id;
     ;i=0)
	{
	if (current_page == 0)
		d20_5omask[D20_5ROOT_POS].d20_fkt_id = sav_root_fkt_id;
	else
		d20_5omask[D20_5ROOT_POS].d20_fkt_id = D20_NO_OP;

	/*  fill screen                       */
	
	val = (srt_list_info.page_info + current_page)->page_beg;
	curr_page_size = (srt_list_info.page_info + current_page)->page_size;

	/* for all rows where SRT is displayed.	*/
	for (j = 0; j < D20_52CNT; j++)
  		{
		/*  fill output fields                */

		/* each row takes 3 entries in mask.	*/
    		pos = D20_52POS + 3*j;

		/* some rule is still left to be displayed.	*/
    		if (i < curr_page_size)
			{
      			if (val == sch_pos && scheme_disp == FALSE)
				{
				d20_5omask[pos + DS_OCNUMBER].d20_fkt_id =
				d20_5omask[pos + DS_OCNAME].d20_fkt_id =
				d20_5omask[pos + DS_OCTREE].d20_fkt_id = 
								D20_NO_OP ;
				
				val++;
				i++;
				continue;
				}

			for (k=0; k<val->ds_no_nam_attr; k++, j++)
				{
				pos = D20_52POS + 3*j;

				if (k == 0)
					{
					d20_5omask[pos + DS_OCNUMBER].
					d20_ioinfo = val->ds_numb ;
					d20_5omask[pos + DS_OCTREE].
					d20_ioinfo = val->ds_tree ;
					}
				else
					{
					d20_5omask[pos + DS_OCNUMBER].
					d20_ioinfo = ""; 
					d20_5omask[pos + DS_OCTREE].
					d20_ioinfo = "";
					}

				d20_5omask[pos + DS_OCNAME].d20_ioinfo =
							val->ds_nam_attr[k];

				d20_5omask[pos + DS_OCNUMBER].d20_fkt_id
				= d20_5omask[pos + DS_OCNAME].d20_fkt_id
				= d20_5omask[pos + DS_OCTREE].d20_fkt_id
				= D20_CSAY ;
				}

			j--;
			val++ ;
			i++;
			}
		else
			break;
		}

	/* make rest of the rows NO_OP.	*/
	for (; j < D20_52CNT; j++)
		{
		/* each row takes 3 entries in mask. */
		pos = D20_52POS + 3*j;
		d20_5omask[pos + DS_OCNUMBER].d20_fkt_id = 
		d20_5omask[pos + DS_OCNAME].d20_fkt_id =
		d20_5omask[pos + DS_OCTREE].d20_fkt_id = D20_NO_OP ;
		}

	/* loop to display one page of SRT.	*/
	do
		{
		fkt_key = d20_handlemask (d20_5omask);

		switch (fkt_key)
			{
			case FKTDEL:
				hit = TRUE; /* to leave do loop.	*/
				break;

			case FKTSCD:
				if (current_page == 
						srt_list_info.no_of_pages - 1)
					{
					d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errtab[D20_E38IDX] ;
					d20_handlemask (d20_qomask) ;
					}
				else
					{
					current_page++;
					hit = TRUE; /* to leave do loop. */
					}

				break;

			case FKTSCU:
				if (current_page == 0)
					{
					d20_qomask[D20_Q1POS].d20_ioinfo = 
							d20_errtab[D20_E42IDX] ;
					d20_handlemask (d20_qomask) ;
					}
				else
					{
					current_page--;
					hit = TRUE; /* to leave do loop. */
					}

				break;

			case FKTCR:
			case FKTMENU:
				/* look for selected structure rule */

				val = sav_val; 

				for (i=0, hit=FALSE; i < d2_nd20srt; 
							    i++, val++)
					{
					/*  compare object-classes            */
					if (scheme_disp == FALSE && 
								val == sch_pos)
						continue;

					if ((*rule_nr = *(signed32 *)d20_5ifields[
					    DS_OC_CLASS]) == atoi(val->ds_numb))
						{
						hit = TRUE;
						break;
						}
					}

				/*  No match in screen                */
				if (hit == FALSE)
					{
					/* root is selected and allowed       */
					if (*(signed32 *) d20_5ifields[DS_OC_CLASS] 
					    == 0 && 
					    d20_5omask[D20_5ROOT_POS].d20_fkt_id
				  	    == D20_CSAY)
						{
						*rule_nr = 0;
						hit = TRUE;
						}
					else
						{
						/*  put message on screen     */
						d20_qomask[D20_Q1POS].d20_ioinfo
						= d20_errtab[D20_E02IDX] ;
						d20_handlemask (d20_qomask) ;
						}
					}
				else
					/*  return object class and tree  */
					/* structure			  */
					strncpy(tree, val->ds_tree, 
								DS_TREE_LENGTH);

				break;
			} /* end of switch.	*/

		} while (hit == FALSE);
		 /*  selected object class not found   */

	if (fkt_key == FKTDEL)
		{
		ret_value = D2_ERROR;
		break;
		}

  	if (fkt_key == FKTSCU || fkt_key == FKTSCD)
		continue;
	else
		break;
	}

d20_5omask[D20_5ROOT_POS].d20_fkt_id = sav_root_fkt_id;

/*  return(DS_NOERROR)                */
return(ret_value);

}	/*  ds0_500_objects()                 */

/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME:        ds0_check_chgstruct()                                 */
/*								      */
/* AUTHOR:      Mueller V. (SNI AP 113)                               */
/* DATE:        10.06.91                                              */
/*								      */
/* SYNTAX:      Bool ds0_check_chgstruct(attr_type,len_attr,srt)      */
/*              Octet_string    attr_type;                            */
/*              signed32        len_attr;                             */
/*              D20_srt        *srt;                                  */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function checks wheter the attribute has a pass- */
/*              word or a psap-address                                */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Octet_string    attr_type;                            */
/*              signed32        len_attr;                             */
/*              D20_srt        *srt;                                  */
/*								      */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*    D2_TRUE   :       attribute present                             */
/*    D2_FALSE  :       attribute not present                         */
/*								      */
/* STANDARD-HEADER:                                                   */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*								      */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*inoff ***************************************************************/

 Bool ds0_check_chgstruct( Octet_string  attr_type,
			   signed32      len_attr,
			   D20_srt      *srt)
{                               /*  ds0_check_chgstruct()             */

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

   Bool  pwd_flag = FALSE;
   signed32  i, j;

for (j=0; j < srt->no_obj_class; j++)
     {
     if (pwd_flag != FALSE)
	  break;
     for (i=0; i < srt->obj_class[j]->no_must_attr; i++)
	  {
	  if (d20_cmp_objid(&(srt->obj_class[j]->must_attr[i]->obj_id),len_attr,attr_type))
		{
		pwd_flag = TRUE;
		break;
		}
	  }
	  if (pwd_flag == FALSE)
		{
		for (i=0;  i < srt->obj_class[j]->no_may_attr; i++)
		     {
		     if (d20_cmp_objid(&(srt->obj_class[j]->may_attr[i]->obj_id),len_attr,attr_type))
			   {
			   pwd_flag = TRUE;
			   break;
			   }
		     }
		}
     }

return(pwd_flag);

}                               /*  ds0_check_chgstruct()             */
