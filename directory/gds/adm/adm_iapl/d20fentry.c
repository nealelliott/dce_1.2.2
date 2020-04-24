/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20fentry.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:51  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:58  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:19  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:22  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:14:32  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:40:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:09:29  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:45:23  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:46  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:31:02  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:07:53  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20fentry.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:46 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20fentry.c                                         */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  ds4_b01_fill_entry ()                                         */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function converts a distinguished name (DN), an alias name or a  */
/*      relative distinguished name (RDN) given in a format used by the      */
/*      administration into a 'string'-format used at the IAPL-interface     */
/*      of the directory system V2.                                          */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      optype      = Operation mode:                                        */
/*                       D20_OBJECT = convert a DN.                          */
/*                       D20_ALIAS  = convert an alias name.                 */
/*                       D20_RDN    = convert a RDN.                         */
/*      obj_entry   = Pointer to an array which contains the informations    */
/*                    about the name parts of the given name (name part      */
/*                    attribute type, name part value size, name part value).*/
/*      no_np       = No. of name parts of the given name.                   */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      info        = Converted (DN-, alias name- or RDN-) string.           */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 4.3.88       */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2info.h>
#include <d20proto.h>

/* -------------------- declaration of global data ------------------------- */

extern Ds_v2_info	info ;	/* common IAPL-interface parameter block */
extern  D20_srt    *d2_p_d20srt;          /* pointer to SRT              */
extern  signed32    d2_nd20srt ;          /* number of SRT elements      */
extern  Bool	   d20_printable;

/* ------------------------------------------------------------------------- */

 void ds4_b01_fill_entry (
   signed16      optype,
   Ds_obj_entry	*obj_entry,
   signed16      no_np,
   char	         tree[])
{
register signed16 i, k;
	signed32 nr, sav_len;
	D20_srt	*ptr;
D2_name_string 		str;
char	type_val_sep;

switch ((int) optype) 
	{
	case D20_OBJECT:
		sprintf ((char *)info.entry, "") ;
		str = info.entry;
		break ;
	case D20_ALIAS:
		sprintf ((char *)info.alias, "") ;
		str = info.alias;
		break ;
	case D20_RDN:
		sprintf ((char *)info.rdn, "") ;
		str = info.rdn;
	}

if (optype == D20_RDN)
	/* take the last component of tree structure.	*/
	k = (int)strlen(tree)/3;
else
	/* start from root of the tree.	*/
	k = 0;

/*  create object name from tree      */
for (; no_np != 0; k++)
	{
	/* rule number cannot be read from tree string.	*/
    	if (sscanf(tree + 3 * k,"%2d",&nr) <= 0)
  		/*  break loop                        */
    		break;

	/* look for the rule no. in SRT and use naming attributes.	*/
	ptr = d20_search_adm_srule(d2_nd20srt, d2_p_d20srt, nr);

	/* save length of distinguish name generated so far.	*/
	sav_len = strlen((char *)str);

	/* fill AVAs belonging to one RDN.	*/
	for (i=0; i<ptr->no_nam_attr; i++, obj_entry++)
		{
		switch ((int) d27_401_syntax_to_rep((signed16)
					ptr->nam_attr[i]->at.att_syntax))
			{
			case D2_T61_PR:
				if (d20_printable == TRUE)
					type_val_sep = D2_PRTBL_REP;
				else
					type_val_sep = D2_T61_REP;
				break;
			case D2_PRINTABLE:
				type_val_sep = D2_PRTBL_REP;
				break;
			case D2_IA5:                        
				type_val_sep = D2_IA5_REP;
				break;
			case D2_NUMERIC:
				type_val_sep = D2_NUM_REP;
				break;
			default:
				type_val_sep = D2_T61_REP;
				break;
			}

		sprintf ((char *)(str + strlen((char *)str)), D2_RDN_FORMAT,
			 D2_AVA_SEP,
			 strlen((char *)obj_entry->attr_type),
			 obj_entry->attr_type, 
			 type_val_sep,
			 obj_entry->obj_real_len, 
			 obj_entry->obj_name) ;

		no_np--;
		}

	/* put RDN delimiter between two RDNs.	*/
	*(str + sav_len) = D2_RDN_SEP;
	}
}
