/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20dpoct.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:10  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:22  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:11  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:59  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:27  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:17:46  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:00:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:13:32  marrek]
 * 
 * Revision 1.1.4.4  1993/01/29  08:30:48  marrek
 * 	Changes for bug 6575.
 * 	[1993/01/28  10:17:12  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:41  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:10  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:51:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:59:20  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:38:41  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:46  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20dpoct.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:12 $";
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
/* NAME         : d20dpoct.o  [display OCT]                           */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 10.01.91					      */
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

extern D20_omask  d20_qomask[];         /* input fields of error mask */
extern D20_omask  d20_Aomask[];         /* ODT mask                   */
extern char      *d20_Aifields[];       /* input fields of ODT-mask   */

extern char      *d20_errtab[];         /* error table                */
extern char      *d20_err_table[];         /* error table                */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static void d20_005_fill_multi_fields(Oct *oct_entry, signed32 field_type);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_003_display_oct()                                 */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        10.01.91					      */
/*								      */
/* SYNTAX:      signed32  d20_003_display_oct(nobj, oct)              */
/*			signed16 nobj;				      */
/*			Oct	*oct;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will display the OCT.                   */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16 nobj;		no. of entries in OCT	      */
/*		Oct	*oct;		pointer to OCT		      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 signed32 d20_003_display_oct(
   signed16  nobj,
   Oct	    *oct)

{                               /*  d20_003_display_oct()             */

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

 signed16    idx;
 D2_pbhead   head;
 D20_p6      p6;
 signed32   *schema_index_space;
 signed32    no_selected_entries = 0;
 signed32    ret_value = D2_NOERROR;
 signed32    fkt;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/* allocate space for selected indices.		*/
if ((schema_index_space = (signed32 *)malloc(nobj * sizeof(signed32))) == 
							(signed32 *)NULL)
	{
	d20_qomask[D20_Q1POS].d20_ioinfo = d20_err_table[D20_E04IDX];
  	d20_handlemask(d20_qomask); /* memory error */
	return(D2_ERROR);
	}

/* change the function id. of input fields to D20_CSAY		*/
d20_Aomask[D20_O_ABBRV_POS+1].d20_fkt_id = 
d20_Aomask[D20_O_NAME_POS+1].d20_fkt_id = 
d20_Aomask[D20_O_OBJID_POS+1].d20_fkt_id = 
d20_Aomask[D20_O_CKIND_POS+1].d20_fkt_id = 
d20_Aomask[D20_O_FNO_POS+1].d20_fkt_id = D20_CSAY;

/* change the function. id. of sup. class fields to D20_SAY */
for (idx = D20_O_SUP_POS + 1; idx <= D20_O_SUP_POS + D2_SUP_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CSAY;

/* change the function. id. of aux. class fields to D20_SAY */
for (idx = D20_O_AUX_POS + 1; idx <= D20_O_AUX_POS + D2_AUX_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CSAY;

/* change the function. id. of mandatory attribute fields to D20_SAY */
for (idx = D20_O_MAND_POS + 1; idx <= D20_O_MAND_POS + D2_MAND_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CSAY;

/* change the function. id. of optional attribute fields to D20_SAY */
for (idx = D20_O_OPT_POS + 1; idx <= D20_O_OPT_POS + D2_OPT_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CSAY;


head.d2_version = DS_V03;
p6.d20_6pbhead  = &head;

/* enable F1 function key.	*/
d20_Aomask[D20_O_F1_POS].d20_fkt_id = D20_FKTX;

/*  EVER                              */
for (fkt=FKTF1 ; ; )
	{
	if (fkt == FKTF1)
		{
		/* display summary list.	*/
		if ((ret_value = d20_handle_schema_list((signed32)DS__OCT_DISPLAY, 
			schema_index_space, &no_selected_entries)) == D2_ERROR)
			break;

		if (no_selected_entries == 0)
			break;

		p6.d20_6disp    = D2_SCH_FIRST;
		p6.d20_6nentries    = no_selected_entries;
		p6.d20_6first  = p6.d20_6act = schema_index_space;
		}


  	/*  scheme display operation is ok    */
     	if (!d20_6_display_scheme(&p6))
  		{
		/*  display received entry            */
		/*  fill output mask                  */
    		d20_004_fill_oct_mask((oct + *p6.d20_6act));
		}
	else
		{
  		/*  display error message             */
    		switch ((int) (p6.d20_6pbhead)->d2_errvalue)
			{
      			case D2_E_SCH:    
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E85IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			case D2_B_SCH:    
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E83IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			case D2_NO_ELEM:  
				d20_qomask[D20_Q1POS].d20_ioinfo =
						      d20_errtab[D20_E81IDX];
				d20_handlemask (d20_qomask) ;
				break;

      			default:
				break;
			}
		}

  	if ((p6.d20_6pbhead)->d2_errvalue == D2_NO_ELEM)
    		break;

	/*  display OCT entry                 */
	fkt = d20_handlemask (d20_Aomask) ;

  	/*  break was selected                */
     	if (fkt == FKTDEL)
    		break;

	/*  SCROLL UP was selected            */
	if (fkt == FKTSCU)
		/* take previous entry.	      */
		p6.d20_6disp    = D2_SCH_PREVIOUS;
	else
		/*  take next OCT-entry       */
		p6.d20_6disp    = D2_SCH_NEXT;

	}

/* change the function id. of input fields to D20_CGET		*/
d20_Aomask[D20_O_ABBRV_POS+1].d20_fkt_id = 
d20_Aomask[D20_O_NAME_POS+1].d20_fkt_id = 
d20_Aomask[D20_O_OBJID_POS+1].d20_fkt_id = 
d20_Aomask[D20_O_CKIND_POS+1].d20_fkt_id = 
d20_Aomask[D20_O_FNO_POS+1].d20_fkt_id = D20_CGET;

/* change the function. id. of sup. class fields to D20_CGET */
for (idx = D20_O_SUP_POS + 1; idx <= D20_O_SUP_POS + D2_SUP_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CGET;

/* change the function. id. of aux. class fields to D20_CGET */
for (idx = D20_O_AUX_POS + 1; idx <= D20_O_AUX_POS + D2_AUX_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CGET;

/* change the function. id. of mandatory attribute fields to D20_CGET */
for (idx = D20_O_MAND_POS + 1; idx <= D20_O_MAND_POS + D2_MAND_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CGET;

/* change the function. id. of optional attribute fields to D20_CGET */
for (idx = D20_O_OPT_POS + 1; idx <= D20_O_OPT_POS + D2_OPT_CNT; idx++)
  	d20_Aomask[idx].d20_fkt_id = D20_CGET;

/* disable F1 function key.	*/
d20_Aomask[D20_O_F1_POS].d20_fkt_id = D20_NO_OP;

free(schema_index_space);
return(ret_value);

}                               /*  d20_003_display_oct()             */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_004_fill_oct_mask()                               */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        10.01.91					      */
/*								      */
/* SYNTAX:      void  d20_004_fill_oct_mask(oct_entry)                */
/*			Oct	*oct_entry;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will fill the input fields of the OCT-  */
/*              mask.                                                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Oct 		*oct_entry; OCT-entry to be displayed */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 void d20_004_fill_oct_mask(
   Oct  *oct_entry)

{                               /*  d20_004_fill_oct_mask()           */

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

	signed32	idx;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*d20_Aifields[DS_O_UP] = *d20_Aifields[DS_O_DOWN] = *d20_Aifields[DS_O_EXEC]
		       = *d20_Aifields[DS_O_BREAK] = DS_DEFAULT_VALUE;

strcpy(d20_Aifields[DS_O_ABBRV],	oct_entry->obj_abbrv);
strcpy(d20_Aifields[DS_O_NAME],		oct_entry->obj_name);

/* initialize all super class fields */
for (idx=DS_O_SUP; idx<DS_O_SUP + D2_SUP_CNT; idx++)
	*d20_Aifields[idx] = D2_EOS;

/* initialize all aux. class fields */
for (idx=DS_O_AUX; idx<DS_O_AUX + D2_AUX_CNT; idx++)
	*d20_Aifields[idx] = D2_EOS;

/* initialize all mandatory attribute fields */
for (idx=DS_O_MAND; idx<DS_O_MAND + D2_MAND_CNT; idx++)
	*d20_Aifields[idx] = D2_EOS;

/* initialize all optional attribute fields */
for (idx=DS_O_OPT; idx<DS_O_OPT + D2_OPT_CNT; idx++)
	*d20_Aifields[idx] = D2_EOS;

/* fill fields of superior object classes */
d20_005_fill_multi_fields(oct_entry, DS_O_SUP);

strcpy(d20_Aifields[DS_O_OBJID],	oct_entry->obj_id);
*((signed32 *)d20_Aifields[DS_O_CKIND])     	= oct_entry->obj_class_kind;
*((signed32 *)d20_Aifields[DS_O_FNO])     	= oct_entry->obj_file_nr;

/* fill fields of auxiliary object classes */
d20_005_fill_multi_fields(oct_entry, DS_O_AUX);

/* fill fields of mandatory attributes */
d20_005_fill_multi_fields(oct_entry, DS_O_MAND);

/* fill fields of optional attributes */
d20_005_fill_multi_fields(oct_entry, DS_O_OPT);

}                               /*  d20_004_fill_oct_mask()           */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_005_fill_multi_fields()                           */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        10.01.91					      */
/*								      */
/* SYNTAX:      void  d20_005_fill_multi_fields(oct_entry, field_type)*/
/*			Oct	 *oct_entry;			      */
/*			signed32  field_type;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function fill fill fields of OCT mask which have */
/*		more than one line in the mask and these fields       */
/*		involve abbreviations.				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Oct 		*oct_entry; OCT-entry to be displayed */
/*		signed32	field_type; possible values are	      */
/*				DS_O_SUP, DS_O_AUX, DS_O_MAND or      */
/*				DS_O_OPT			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 static void d20_005_fill_multi_fields(
   Oct	     *oct_entry,
   signed32   field_type)

{		/*  d20_005_fill_multi_fields()           */

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
signed32 rem_len,i,j;
char	*run_ptr;
signed32 field_cnt, first_field_len, last_field_len, no_abbrvs;
Abbrv	*abbrvs;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

switch ((int) field_type)
	{
	case DS_O_SUP:
		field_cnt = D2_SUP_CNT;
		first_field_len = D2_SUPF_LEN;
		last_field_len = D2_SUPL_LEN;
		no_abbrvs = oct_entry->no_sup_class;
		abbrvs = oct_entry->sup_class;
		break;

	case DS_O_AUX:
		field_cnt = D2_AUX_CNT;
		first_field_len = D2_AUXF_LEN;
		last_field_len = D2_AUXL_LEN;
		no_abbrvs = oct_entry->no_aux_class;
		abbrvs = oct_entry->aux_class;
		break;

	case DS_O_MAND:
		field_cnt = D2_MAND_CNT;
		first_field_len = D2_MANDF_LEN;
		last_field_len = D2_MANDL_LEN;
		no_abbrvs = oct_entry->no_must_attr;
		abbrvs = oct_entry->must_attr;
		break;

	case DS_O_OPT:
		field_cnt = D2_OPT_CNT;
		first_field_len = D2_OPTF_LEN;
		last_field_len = D2_OPTL_LEN;
		no_abbrvs = oct_entry->no_may_attr;
		abbrvs = oct_entry->may_attr;
		break;
	}

j=0;
for (i=0;i<field_cnt-1;i++)
	{
	rem_len = first_field_len - 1;
	run_ptr = d20_Aifields[field_type + i];
	for (;j<no_abbrvs;j++)
		if (rem_len >= (int)strlen(abbrvs[j]))
			{
			strcpy(run_ptr,abbrvs[j]);
			rem_len -= strlen(run_ptr);
			run_ptr += strlen(run_ptr);
			if (rem_len > 0)
				{
				strcpy(run_ptr," ");
				run_ptr += 1;
				rem_len -= 1;
				}
			}
		else
			break;
	}

/* fill last field */
if (j<no_abbrvs)
	{/* still some abbrvs. are left */
	rem_len = last_field_len - 1;
	run_ptr = d20_Aifields[field_type + field_cnt - 1];
	for (;j<no_abbrvs;j++)
		if (rem_len >= (int)strlen(abbrvs[j]))
			{
			strcpy(run_ptr,abbrvs[j]);
			rem_len -= strlen(run_ptr);
			run_ptr += strlen(run_ptr);
			if (rem_len > 0)
				{
				strcpy(run_ptr," ");
				run_ptr += 1;
				rem_len -= 1;
				}
			}
		else
			break;
	}

}		/* end of d20_005_fill_multi_fields()	*/
