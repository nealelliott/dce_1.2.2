/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20gobj.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:15  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:28  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:13  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:09:08  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:22  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:40  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:59  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:18:20  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:03:41  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:14:15  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:49  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:25  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:52:00  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:00:08  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:39:23  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:04  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20gobj.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:16 $";
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
/* NAME         : d20gobj.o  [get object description from mask]       */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 14.02.91                                            */
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
/*      0.1 | 07.07.88| Original                       | ws |         */
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
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define	BLANK	' '

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

extern D20_omask  d20_Aomask[];         /* ODT mask                   */
extern char      *d20_Aifields[];       /* input fields of ODT-mask   */

extern D20_omask  d20_qomask[];         /* input fields of error mask */
extern char      *d20_err_table[];      /* error table                */
extern char      *d20_errmess[];      	/* error table                */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_005_get_obj()                                     */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        23.03.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_005_get_obj(init, abbrv)        	      */
/*              Bool     init;                                        */
/*		char	*abbrv;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will read the object class abbrv.	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool     init;        a flag inidcating whether the   */
/*                                    mask must be initialized        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		char	*abbrv;					      */
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR:   break was selected                        */
/*              D2_NOERROR: input ok                                  */
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

 d2_ret_val d20_005_get_obj(
   Bool  init,
   char	*abbrv)

{                               /*  d20_005_get_obj()            */

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

 d2_ret_val  ret_value = D2_NOERROR;
 signed16  idx;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*d20_Aifields[DS_O_EXEC] = *d20_Aifields[DS_O_BREAK] = DS_DEFAULT_VALUE;

/* deactivate scroll up/down keys	*/
d20_Aomask[D20_O_SCU_POS].d20_fkt_id = 
d20_Aomask[D20_O_SCU_POS + 1].d20_fkt_id = D20_NO_OP;

for (idx = D20_O_NAME_POS; idx <= D20_O_OPT_POS; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_NO_OP;

for (idx = D20_O_OPT_POS + 1; idx <= D20_O_OPT_POS +  D2_OPT_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_NO_OP;

if (init == TRUE)
	*d20_Aifields[DS_O_ABBRV] = D2_EOS;
else
	strcpy(d20_Aifields[DS_O_ABBRV], abbrv);

/*  display ODT mask                  */
d20_handlemask (d20_Aomask) ;

/*  'break' was  selected             */
if   (*d20_Aifields[DS_O_BREAK] != DS_DEFAULT_VALUE)
	{/*  ret_value = D2_ERROR              */
  	ret_value = D2_ERROR;
	}
else
	{/*  get object class abbreviation     */
	strcpy(abbrv,d20_Aifields[DS_O_ABBRV]);
  	ret_value = D2_NOERROR;
	}

/* activate scroll up/down keys	*/
d20_Aomask[D20_O_SCU_POS].d20_fkt_id = 
d20_Aomask[D20_O_SCU_POS + 1].d20_fkt_id = D20_FKTX;

/* reset function id. of all the fields */
d20_Aomask[D20_O_ABBRV_POS].d20_fkt_id =
d20_Aomask[D20_O_NAME_POS].d20_fkt_id =
d20_Aomask[D20_O_SUP_POS].d20_fkt_id =
d20_Aomask[D20_O_OBJID_POS].d20_fkt_id =
d20_Aomask[D20_O_CKIND_POS].d20_fkt_id =
d20_Aomask[D20_O_FNO_POS].d20_fkt_id =
d20_Aomask[D20_O_AUX_POS].d20_fkt_id =
d20_Aomask[D20_O_MAND_POS].d20_fkt_id =
d20_Aomask[D20_O_OPT_POS].d20_fkt_id = D20_CSAY;

d20_Aomask[D20_O_ABBRV_POS + 1].d20_fkt_id =
d20_Aomask[D20_O_NAME_POS + 1].d20_fkt_id =
d20_Aomask[D20_O_OBJID_POS + 1].d20_fkt_id =
d20_Aomask[D20_O_CKIND_POS + 1].d20_fkt_id =
d20_Aomask[D20_O_FNO_POS + 1].d20_fkt_id = D20_CGET;


for (idx = D20_O_SUP_POS + 1; idx <= D20_O_SUP_POS + D2_SUP_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CGET;

for (idx = D20_O_AUX_POS + 1; idx <= D20_O_AUX_POS + D2_AUX_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CGET;

for (idx = D20_O_MAND_POS + 1; idx <= D20_O_MAND_POS + D2_MAND_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CGET;

for (idx = D20_O_OPT_POS + 1; idx <= D20_O_OPT_POS + D2_OPT_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CGET;

				/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_005_get_object_type()         */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_006_get_new_obj()                                 */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.02.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_006_get_new_obj(init, op_mode, obj)    */
/*              Bool            init;                                 */
/*		signed32	op_mode;			      */
/*              Oct		*obj;                                 */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will get the new object class from the  */
/*		mask.						      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool     init;        a flag inidcating whether the   */
/*                                    mask must be initialized        */
/*		signed32 op_mode;     specifies whether operation is  */
/*				      Add a new object class or modify*/
/*				      an existing object class.       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              Oct	*obj;         new object class description    */
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR:   break was selected                        */
/*              D2_NOERROR: input ok                                  */
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

 d2_ret_val d20_006_get_new_obj(
   Bool      init,
   signed32  op_mode,
   Oct	    *obj)

{                               /*  d20_006_get_new_obj()             */

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

 d2_ret_val ret_value = D2_NOERROR;
 signed32 j;
 char	max_sup_fld[D2_SUPF_LEN];
 char	max_aux_fld[D2_AUXF_LEN];
 char	max_mand_fld[D2_MANDF_LEN];
 char	max_may_fld[D2_OPTF_LEN];
 signed32 ret_code;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/*  initialize OCT mask               */
*d20_Aifields[DS_O_EXEC] = *d20_Aifields[DS_O_BREAK] = DS_DEFAULT_VALUE;

/* deactivate scroll up/down keys	*/
d20_Aomask[D20_O_SCU_POS].d20_fkt_id = 
d20_Aomask[D20_O_SCU_POS + 1].d20_fkt_id = D20_NO_OP;

/* if operation is modify object class set abbrv. field to CSAY	*/
if (op_mode == DS__M_OBJ)
	d20_Aomask[D20_O_ABBRV_POS + 1].d20_fkt_id = D20_CSAY;

/* initialize fields of superior and aux. classes and for mandatory and      */
/* optional attributes (whether init is D2_TRUE or it is D2_FALSE).In case   */
/* when init is D2_FALSE,  these fields are filled by appropriate functions. */

if (init == TRUE)
	{
	*d20_Aifields[DS_O_ABBRV] = D2_EOS;
	*d20_Aifields[DS_O_NAME] = D2_EOS;
	*d20_Aifields[DS_O_OBJID] = D2_EOS;
	*((signed32 *)d20_Aifields[DS_O_CKIND]) = D2_ABSTRACT;
	*((signed32 *)d20_Aifields[DS_O_FNO]) = 1;

	for (j=DS_O_SUP; j<DS_O_SUP + D2_SUP_CNT; j++)
		*d20_Aifields[j] = D2_EOS;

	for (j=DS_O_AUX; j<DS_O_AUX + D2_AUX_CNT; j++)
		*d20_Aifields[j] = D2_EOS;

	for (j=DS_O_MAND; j<DS_O_MAND + D2_MAND_CNT; j++)
		*d20_Aifields[j] = D2_EOS;

	for (j=DS_O_OPT; j<DS_O_OPT + D2_OPT_CNT; j++)
		*d20_Aifields[j] = D2_EOS;
	}
else
	d20_004_fill_oct_mask(obj);

/* loop until valid object class mask is entered or break is entered.	*/
for (;;)   
	{
	/*  display OCT mask     */
	d20_handlemask (d20_Aomask) ;

	/*  'break' was  selected             */
	if   (*d20_Aifields[DS_O_BREAK] != DS_DEFAULT_VALUE)
		{/*  ret_value = D2_ERROR              */
  		ret_value = D2_ERROR;
		break;
		}
	else
		{
		/* get new object class                   */
		strcpy(obj->obj_abbrv, d20_Aifields[DS_O_ABBRV]);
		strcpy(obj->obj_name, d20_Aifields[DS_O_NAME]);
		strcpy(obj->obj_id, d20_Aifields[DS_O_OBJID]);
		obj->obj_class_kind = *((signed32 *)d20_Aifields[DS_O_CKIND]);
		obj->obj_file_nr = *((signed32 *)d20_Aifields[DS_O_FNO]);

		/* extract super classes from the mask.	*/
		if (d20_extract_acronyms(D2_SUP_CNT, &d20_Aifields[DS_O_SUP], 
				NO_SUP_CLASS, &(obj->no_sup_class), 
				obj->sup_class, max_sup_fld, &ret_code) 
								== D2_ERROR)
			{
			if (ret_code == D20_TOO_MANY_ACR)
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E57IDX];
			else	/* ret_code == D20_TOO_BIG_ACR	*/
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E58IDX];

  			d20_handlemask (d20_qomask) ;
			continue; /* go back to get object class mask loop    */
			}

		/* extract aux. classes from the mask.	*/
		if (d20_extract_acronyms(D2_AUX_CNT, &d20_Aifields[DS_O_AUX], 
				NO_AUX_CLASS, &(obj->no_aux_class), 
				obj->aux_class, max_aux_fld, &ret_code) 
								== D2_ERROR)
			{
			if (ret_code == D20_TOO_MANY_ACR)
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E59IDX];
			else	/* ret_code == D20_TOO_BIG_ACR	*/
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E60IDX];

  			d20_handlemask (d20_qomask) ;
			continue; /* go back to get object class mask loop    */
			}

		/* extract mandatory attributes from the mask.	*/
		if (d20_extract_acronyms(D2_MAND_CNT, &d20_Aifields[DS_O_MAND], 
				NO_MAND_ATTR, &(obj->no_must_attr), 
				obj->must_attr, max_mand_fld, &ret_code) 
								== D2_ERROR)
			{
			if (ret_code == D20_TOO_MANY_ACR)
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E80IDX];
			else	/* ret_code == D20_TOO_BIG_ACR	*/
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E82IDX];

  			d20_handlemask (d20_qomask) ;
			continue; /* go back to get object class mask loop    */
			}

		/* extract optional attributes from the mask.	*/
		if (d20_extract_acronyms(D2_OPT_CNT, &d20_Aifields[DS_O_OPT], 
				NO_OPT_ATTR, &(obj->no_may_attr), 
				obj->may_attr, max_may_fld, &ret_code) 
								== D2_ERROR)
			{
			if (ret_code == D20_TOO_MANY_ACR)
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E81IDX];
			else	/* ret_code == D20_TOO_BIG_ACR	*/
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E82IDX];

  			d20_handlemask (d20_qomask) ;
			continue; /* go back to get object class mask loop    */
			}

  		ret_value = D2_NOERROR;
		break;
		}
	} /* end of loop to get valid object class mask		*/

/* activate scroll up/down keys	*/
d20_Aomask[D20_O_SCU_POS].d20_fkt_id = 
d20_Aomask[D20_O_SCU_POS + 1].d20_fkt_id = D20_FKTX;

/* if operation is modify object class reset abbrv. field to CGET	*/
if (op_mode == DS__M_OBJ)
	d20_Aomask[D20_O_ABBRV_POS + 1].d20_fkt_id = D20_CGET;

/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_006_get_new_obj()             */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_extract_acronyms()                                */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        14.02.91                                              */
/*								      */
/* SYNTAX:	d2_ret_val	d20_extract_acronyms(field_cnt,       */
/*			fields,	acr_limit, no_acr, acr_arr, max_fld,  */
/*			ret_code)				      */
/*		signed32  field_cnt;				      */
/*		char	 *fields[];				      */
/*		signed32  acr_limit;				      */
/*		signed32 *no_acr;				      */
/*		Abbrv	 *acr_arr;				      */
/*		char	 *max_fld;				      */
/*		signed32 *ret_code;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function extracts the acronyms of attrs. or of   */
/*		object classes from masks of schema admin.	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed32  field_cnt; no of fields to be used.	      */
/*		char	 *fields[]; first field pointer		      */
/*		signed32  acr_limit; max. how many acronyms are       */
/*				     expected			      */
/*		char	 *max_fld; pointer to space to store largest  */
/*				   field.			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *no_acr; no. of acronyms actually present in */
/*				  the input.			      */
/*		Abbrv	 *acr_arr; array where acronyms should be     */
/*				   returned			      */
/*		signed32 *ret_code; error code is returned in this var*/
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR:   break was selected                        */
/*              D2_NOERROR: input ok                                  */
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

 d2_ret_val d20_extract_acronyms(
   signed32  field_cnt,
   char	    *fields[],
   signed32  acr_limit,
   signed32 *no_acr,
   Abbrv    *acr_arr,
   char     *max_fld,
   signed32 *ret_code)

{                               /*  d20_extract_acronyms()             */

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

signed32	i,j;
char	*run_ptr;


/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* initialize acronym count.	*/
j=0;

/* for all the fields 	*/
for (i=0;i<field_cnt;i++)
	{
	run_ptr = fields[i];

	/* skip initial blanks from the input	*/
	while (*run_ptr == BLANK) 
		run_ptr++;

	while (*run_ptr != D2_EOS)
		{
		if (j == acr_limit)
			{
			/* too many acronyms.   */	
			*ret_code = D20_TOO_MANY_ACR;
			return(D2_ERROR);
			}

		sscanf(run_ptr, "%s", max_fld);

		if ((int)strlen(max_fld) > (L_ABBRV - 1))
			{
			/* acronym too big.	     */
			*ret_code = D20_TOO_BIG_ACR;
			return(D2_ERROR);
			}

		strcpy(acr_arr[j++], max_fld);

		run_ptr = run_ptr + strlen(max_fld);

		/* skip blanks between or at the end of      */
		/* acronyms 		     		     */
		while (*run_ptr == BLANK) 
			run_ptr++;

		} /* end of analysis of one field.*/

	} /* end of analysis of all fields.	*/

*no_acr = j;

return(D2_NOERROR);

}
