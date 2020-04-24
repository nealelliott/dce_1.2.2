/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20gattr.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:14  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:26  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:21  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:35  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:57  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:18:06  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:02:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:13:58  marrek]
 * 
 * Revision 1.1.4.4  1993/02/04  08:39:24  marrek
 * 	CIGNORE is the first syntax and not CEXACT.
 * 	[1993/02/03  08:38:35  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:46  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:20  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:51:38  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:59:49  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:39:09  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:58  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20gattr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:14 $";
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
/* NAME         : d20gattr.o  [get attribute description from mask]   */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 11.02.91                                            */
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

extern D20_omask  d20_Bomask[];         /* ADT mask                   */

extern char      *d20_Bifields[];       /* input fields of ADT-mask   */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_015_get_attr()                                    */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        11.02.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_015_get_attr(init, abbrv)     	      */
/*              Bool     init;                                        */
/*		char	*abbrv;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function will get the abbreviation of the attr.  */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool     init;          a flag inidcating whether the */
/*                                      mask must be initialized      */
/*		char	*abbrv;		abbreviation of the attribute */
/*					(in case init is false)	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		char	*abbrv;		abbreviation of the attribute */
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR:      break was selected                     */
/*              D2_NOERROR:    input ok                               */
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

 d2_ret_val d20_015_get_attr(
   Bool   init,
   char  *abbrv)

{                               /*  d20_015_get_attr()                */

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
 signed16  max_idx;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*d20_Bifields[DS_A_EXEC] = *d20_Bifields[DS_A_BREAK] = DS_DEFAULT_VALUE;

/* deactivate scroll up/down	*/
d20_Bomask[D20_B11POS].d20_fkt_id = d20_Bomask[D20_B11POS + 1].d20_fkt_id =
				   D20_NO_OP;

for (idx = D20_B2POS, max_idx = idx + 2 * D20_B1CNT; idx < max_idx; idx++)
  	d20_Bomask[idx].d20_fkt_id = D20_NO_OP;

if (init == TRUE)
	*d20_Bifields[DS_A_ABBRV] = D2_EOS;
else
	strcpy(d20_Bifields[DS_A_ABBRV], abbrv);

/*  display ADT mask                  */
d20_handlemask (d20_Bomask) ;

/*  'break' was  selected             */
if   (*d20_Bifields[DS_A_BREAK] != DS_DEFAULT_VALUE)
	{/*  ret_value = D2_ERROR              */
  	ret_value = D2_ERROR;
	}
else
	{/*  get attribute abbreviation		*/
	strcpy(abbrv,d20_Bifields[DS_A_ABBRV]);
  	ret_value = D2_NOERROR;
	}

d20_Bomask[D20_B11POS].d20_fkt_id = d20_Bomask[D20_B11POS + 1].d20_fkt_id =
				   D20_FKTX;

for (idx = D20_B2POS; idx < max_idx; )
	{
  	d20_Bomask[idx++].d20_fkt_id = D20_CSAY;
  	d20_Bomask[idx++].d20_fkt_id = D20_CGET;
	}
				/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_015_get_attr()                */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_016_get_new_attr()                                */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        08.02.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_016_get_new_attr(init, op_mode, attr)  */
/*              Bool		init;                                 */
/*		signed32	op_mode;			      */
/*              At 		*attr;                                */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will read the new attribute description */
/*		from the mask. If operation is 'modify an existing    */
/*		attribute' then Attribute Abbrv. field is a SAY field */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool     init;        a flag indicating whether the   */
/*                                    mask must be initialized        */
/*		signed32 op_mode;     specifies whether operation is  */
/*				      Add a new attribute or modify   */
/*				      an existing attribute. 	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              At	*attr; new attribute description     	      */
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

 d2_ret_val d20_016_get_new_attr(
   Bool      init,
   signed32  op_mode,
   At	    *attr)

{                               /*  d20_016_get_new_attr()            */

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

    d2_ret_val            ret_value = D2_NOERROR;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/*  initialize AT mask               */
*d20_Bifields[DS_A_EXEC] = *d20_Bifields[DS_A_BREAK] = DS_DEFAULT_VALUE;

/* deactivate scroll up/down keys.	*/
d20_Bomask[D20_B11POS].d20_fkt_id = d20_Bomask[D20_B11POS + 1].d20_fkt_id =
				   D20_NO_OP;

/* if operation is modify attr. set abbrv. field to CSAY	*/
if (op_mode == DS__M_ATTR)
	d20_Bomask[D20_B1POS + 1].d20_fkt_id = D20_CSAY;

if (init == TRUE)
	{
    	*d20_Bifields[DS_A_ABBRV] 		= D2_EOS;
    	*d20_Bifields[DS_A_NAME] 		= D2_EOS;
    	*d20_Bifields[DS_A_OBJ_ID] 		= D2_EOS;
    	*((signed32 *)d20_Bifields[DS_A_LOWER]) = 1;
    	*((signed32 *)d20_Bifields[DS_A_UPPER]) = 1;
    	*((signed32 *)d20_Bifields[DS_A_NREC]) 	= 1;
    	*((signed32 *)d20_Bifields[DS_A_SYNTAX])= D2_CIGNORE;
    	*((signed32 *)d20_Bifields[DS_A_ACL])  	= 1;
    	*((signed32 *)d20_Bifields[DS_A_INDEX])	= 0;
    	*((signed32 *)d20_Bifields[DS_A_PHON]) 	= FALSE;
	}
else
	{
    	strcpy(d20_Bifields[DS_A_ABBRV], attr->att_abbrv);
    	strcpy(d20_Bifields[DS_A_NAME],  attr->att_name);
    	strcpy(d20_Bifields[DS_A_OBJ_ID],attr->att_obj_id);
    	*((signed32 *)d20_Bifields[DS_A_LOWER])	 = attr->att_lower;
    	*((signed32 *)d20_Bifields[DS_A_UPPER])	 = attr->att_upper;
    	*((signed32 *)d20_Bifields[DS_A_NREC]) 	 = attr->att_no_recur;
    	*((signed32 *)d20_Bifields[DS_A_SYNTAX]) = attr->att_syntax;
    	*((signed32 *)d20_Bifields[DS_A_ACL])    = attr->att_acl;
    	*((signed32 *)d20_Bifields[DS_A_INDEX])  = attr->att_ind_lev;
    	*((signed32 *)d20_Bifields[DS_A_PHON])   = attr->att_phon;
	}

/*  display ADT mask                  */
d20_handlemask (d20_Bomask) ;

/*  'break' was  selected             */
if (*d20_Bifields[DS_A_BREAK] != DS_DEFAULT_VALUE)
	{
	/*  ret_value = D2_ERROR              */
  	ret_value = D2_ERROR;
	}
else
	{
	/*  get attribute                     */
    	strcpy(attr->att_abbrv,d20_Bifields[DS_A_ABBRV]);
    	strcpy(attr->att_name,d20_Bifields[DS_A_NAME]);
    	strcpy(attr->att_obj_id,d20_Bifields[DS_A_OBJ_ID]);
    	attr->att_lower		= *((signed32 *)d20_Bifields[DS_A_LOWER]);
    	attr->att_upper		= *((signed32 *)d20_Bifields[DS_A_UPPER]);
    	attr->att_no_recur	= *((signed32 *)d20_Bifields[DS_A_NREC]);
    	attr->att_syntax	= *((signed32 *)d20_Bifields[DS_A_SYNTAX]);
    	attr->att_acl		= *((signed32 *)d20_Bifields[DS_A_ACL]);
    	attr->att_ind_lev	= *((signed32 *)d20_Bifields[DS_A_INDEX]);
    	attr->att_phon		= *((signed32 *)d20_Bifields[DS_A_PHON]);

  	ret_value = D2_NOERROR;
	}

/* activate scroll up/down keys.	*/
d20_Bomask[D20_B11POS].d20_fkt_id = d20_Bomask[D20_B11POS + 1].d20_fkt_id =
				   D20_FKTX;

/* if operation is modify attr. reset abbrv. field to CGET	*/
if (op_mode == DS__M_ATTR)
	d20_Bomask[D20_B1POS + 1].d20_fkt_id = D20_CGET;

/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_016_get_new_attr()            */
