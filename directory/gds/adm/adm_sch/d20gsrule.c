/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20gsrule.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:17  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:30  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:24  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:44  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:18:30  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:04:45  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:14:28  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:46:52  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:29  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:52:17  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:00:28  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:39:33  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:10  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20gsrule.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:17 $";
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
/* NAME         : d20gsrule.o  [get structure rule from mask]         */
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

extern D20_omask  d20_Comask[];         /* SRT mask                   */
extern char      *d20_Cifields[];       /* input fields of SRT-mask   */

extern D20_omask  d20_qomask[];         /* input fields of error mask */
extern char      *d20_err_table[];      /* error table                */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_025_get_srule()                                   */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        11.02.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_025_get_srule(init, rule_nr)  	      */
/*              Bool      init;                                       */
/*		signed32 *rule_nr;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function will get the structure rule no. from the*/
/*		mask.						      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool      init;         a flag inidcating whether the */
/*                                      mask must be initialized      */
/*		signed32 *rule_nr;	pointer to rule no. to be     */
/*				        returned.		      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		signed32 *rule_nr;	pointer to rule no. to be     */
/*				        returned.		      */
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

 d2_ret_val d20_025_get_srule(
   Bool      init,
   signed32 *rule_nr)

{                               /*  d20_025_get_srule()                */

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

*d20_Cifields[DS_S_EXEC] = *d20_Cifields[DS_S_BREAK] = DS_DEFAULT_VALUE;

/* deactivate scroll up/down	*/
d20_Comask[D20_S_SCU_POS].d20_fkt_id = 
d20_Comask[D20_S_SCU_POS + 1].d20_fkt_id = D20_NO_OP;

for (idx = D20_S_SUP_POS; idx <= D20_S_OCLASS_POS + 1; idx++)
  	d20_Comask[idx].d20_fkt_id = D20_NO_OP;

if (init == TRUE)
	*((signed32 *)d20_Cifields[DS_S_RULE_NO]) = 1;
else
	*((signed32 *)d20_Cifields[DS_S_RULE_NO]) = *rule_nr;

/*  display SRT mask                  */
d20_handlemask (d20_Comask) ;

/*  'break' was  selected             */
if   (*d20_Cifields[DS_S_BREAK] != DS_DEFAULT_VALUE)
	{/*  ret_value = D2_ERROR              */
  	ret_value = D2_ERROR;
	}
else
	{/*  get structure rule no.	*/
	*rule_nr = *((signed32 *)d20_Cifields[DS_S_RULE_NO]) ;
  	ret_value = D2_NOERROR;
	}

/* activate scroll up/down keys.	*/
d20_Comask[D20_S_SCU_POS].d20_fkt_id = 
d20_Comask[D20_S_SCU_POS + 1].d20_fkt_id = D20_FKTX;

/* reset function id. of all the fields 	*/
for (idx = D20_S_SUP_POS; idx <= D20_S_OCLASS_POS; )
	{
  	d20_Comask[idx++].d20_fkt_id = D20_CSAY;
  	d20_Comask[idx++].d20_fkt_id = D20_CGET;
	}

				/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_025_get_srule()                */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_026_get_new_srule()                               */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        08.02.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_026_get_new_srule(init, op_mode, srule)*/
/*              Bool		init;                                 */
/*		signed32	op_mode;			      */
/*		Srt		*srule;				      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will read the new structure rule descr. */
/*		from the mask. If operation is 'modify an existing    */
/*		structure rule' then rule no. field is a SAY field    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool     init;        a flag indicating whether the   */
/*                                    mask must be initialized        */
/*		signed32 op_mode;     specifies whether operation is  */
/*				      Add a new structure rule or     */
/*				      modify an existing str. rule.   */
/*		Srt	*srule;	      pointer to Srt entry.	      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Srt	*srule;	      pointer to Srt entry.	      */
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

 d2_ret_val d20_026_get_new_srule(
   Bool      init,
   signed32  op_mode,
   Srt      *srule)

{                               /*  d20_026_get_new_srule()            */

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
	char	 max_nam_fld[D2_NAM_LEN];
	signed32 ret_code;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

/*  initialize SRT mask               */
*d20_Cifields[DS_S_EXEC] = *d20_Cifields[DS_S_BREAK] = DS_DEFAULT_VALUE;

/* deactivate scroll up/down keys.	*/
d20_Comask[D20_S_SCU_POS].d20_fkt_id = 
d20_Comask[D20_S_SCU_POS + 1].d20_fkt_id = D20_NO_OP;

/* if operation is modify structure rule set rule no. field to CSAY	*/
if (op_mode == DS__M_SRULE)
	d20_Comask[D20_S_RULE_POS + 1].d20_fkt_id = D20_CSAY;

if (init == TRUE)
	{
    	*((signed32 *)d20_Cifields[DS_S_RULE_NO]) 	= 1;
    	*((signed32 *)d20_Cifields[DS_S_PAR_RULE_NO]) 	= 0;
    	*d20_Cifields[DS_S_NAM_ATTR_ABBRV] 		= D2_EOS;
    	*d20_Cifields[DS_S_OCLASS] 			= D2_EOS;
	}
else
	d20_022_fill_srt_mask(srule);

/* loop until valid SRT mask is entered or break is entered.	*/
for (;;)
	{

	/*  display SRT mask                  */
	d20_handlemask (d20_Comask) ;

	/*  'break' was  selected             */
	if (*d20_Cifields[DS_S_BREAK] != DS_DEFAULT_VALUE)
		{
		/*  ret_value = D2_ERROR      */
  		ret_value = D2_ERROR;
		break;
		}
	else
		{
		/*  get structure rule	      */
    		srule->rule_nr = *((signed32 *)d20_Cifields[DS_S_RULE_NO]);
    		srule->par_rule_nr = *((signed32 *)d20_Cifields[DS_S_PAR_RULE_NO]);

		/* extract naming attributes.		*/
		if (d20_extract_acronyms(1, &d20_Cifields[DS_S_NAM_ATTR_ABBRV], 
				NO_NAM_ATTR, &(srule->no_nam_attr), 
				srule->nam_attr, max_nam_fld, &ret_code) 
								== D2_ERROR)
			{
			if (ret_code == D20_TOO_MANY_ACR)
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E85IDX];
			else	/* ret_code == D20_TOO_BIG_ACR	*/
				d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E86IDX];

  			d20_handlemask (d20_qomask) ;
			continue; /* go back to get object class mask loop    */
			}

		/* extract object class.	*/
    		if (sscanf(d20_Cifields[DS_S_OCLASS], "%s",
			   srule->obj_class) != 1)
			strcpy(srule->obj_class, "");


  		ret_value = D2_NOERROR;
		break;
		}
	}

/* activate scroll up/down keys.	*/
d20_Comask[D20_S_SCU_POS].d20_fkt_id = 
d20_Comask[D20_S_SCU_POS + 1].d20_fkt_id = D20_FKTX;

/* if operation is modify structure rule reset rule no. field to CGET	*/
if (op_mode == DS__M_SRULE)
	d20_Comask[D20_S_RULE_POS + 1].d20_fkt_id = D20_CGET;

/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_026_get_new_srule()            */
