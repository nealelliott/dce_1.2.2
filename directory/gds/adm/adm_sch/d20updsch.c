/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20updsch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:37  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:14:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:53  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:37  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:05:49  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:21:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:20:11  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:18:23  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:01:26  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:57:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:04:39  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:43:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:11:26  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20updsch.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:36 $";
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
/* NAME         : d20updsch.o [update scheme in memory]               */
/*								      */
/* AUTHOR       : Sanjay, D AP 11                                     */
/* DATE         : 08.02.91                                            */
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
/*      0.1 | 04.07.88| Original                       | ws |         */
/*      0.2 | 12.01.89| Fehlertext D20_E80IDX fuer     | WS |     56  */
/*          |         | Schema-Administration geaendert|    |         */
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
extern char      *d20_errtab[];      /* error table                */
extern char      *d20_err_table[];      /* error table                */
extern char      *d20_errmess[];        /* error table                */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_032_update()                                      */
/*								      */
/* AUTHOR:      Sanjay (D AP 11)                                      */
/* DATE:        08.02.91                                              */
/*								      */
/* SYNTAX:      d2_ret_val   d20_032_update(upd_info)     	      */
/*		S_upd_info	*upd_info;			      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*		This function calls another main update function.     */
/*		If any error occurs in update, suitable error messages*/
/*		are handled by this function.			      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		S_upd_info	*upd_info;			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
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

 d2_ret_val d20_032_update(
   S_upd_info *upd_info)
 
{			/*  d20_032_update()                  */

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

   d2_ret_val   ret_value = D2_NOERROR;
   char         message[D20_MAXMSG];
   Errcode      ret_code;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

if (gds_upsh(&ret_code, DS_V03, upd_info) == D2_ERROR)
	{
  	switch ((int) ret_code.errvalue)
		{
		/* either AT, OCT or SRT entry doesn't    */
		case DS_ENT_NFOUND : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E57IDX];
			break;

		/* either superior OCT entry or SRT entry does not exist */
		case DS_SUP_NFOUND : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E58IDX];
			break;

		/* either AT, OCT or SRT entry already    */
		case DS_EXISTS :    
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E59IDX];
			break;

		/* object class (referenced in SRT entry) doesn't exist */
		case DS_OBJCLS_NFOUND : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E60IDX];
			break;

		/* optional attribute (referenced in OCT entry) doesn't exist */
		case DS_OPT_ATTR_NFOUND : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E61IDX];
			break;

		/* mandatory attribute (referenced in OCT entry) doesn't exist*/
		case DS_MAND_ATTR_NFOUND :
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E62IDX];
			break;

		/* attribute is both an optional and a mandatory one */
		case DS_OPT_AND_MAND :   
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E63IDX];
			break;

		/* the object identifier is not unique */
		case DS_OBJID_NUNIQUE : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E64IDX];
			break;

		/* too many rules which generate same structure */
		case DS_SIMILAR_RULE :
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E65IDX];
			break;

		/* some entries will be left orphans because of this operation*/
		case DS_ORPHAN :   
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E66IDX];
			break;

		/* OCT entry still in use in a SRT entry	*/
		case DS_OCT_IN_USE : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E67IDX];
			break;

		/* AT entry still in use (either in OCT ar SRT entry	*/
		case DS_AT_IN_USE : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E68IDX];
			break;

		/* wrong input for file number            */
		case DS_WR_FILENO : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E69IDX];
			break;

		/* wrong input for an abbreviation (contains ' ' or is empty */
		case DS_WR_ABBRV : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E70IDX];
			break;

		/* wrong format of object identifier      */
		case DS_WR_OBJ_ID_FORMAT : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E71IDX];
			break;

		/* wrong name either for attribute or for object class */
		case DS_WR_NAME :   
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E72IDX];
			break;

		/* wrong input for either upper or lower  bound */
		case DS_WR_BOUND : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E73IDX];
			break;

		/* wrong input for number of recurring attribute values */
		case DS_WR_NO_RECUR : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E74IDX];
			break;

		/* wrong input for syntax                 */
		case DS_WR_SYNTAX :  
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E75IDX];
			break;

		/* wrong input for access class           */
		case DS_WR_ACL :    
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E76IDX];
			break;

		/* wrong input for index level            */
		case DS_WR_IND_LEV : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E77IDX];
			break;

		/* wrong input for phonetical matching    */
		case DS_WR_PHON :   
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E78IDX];
			break;

		/* the attribute/object class name is not unique */
		case DS_NAME_NUNIQUE : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E79IDX];
			break;

		/* the naming attribute does not exist.		*/
		case DS_NAME_ATTR_NFOUND : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E87IDX];
			break;

		/* repetition in attribute/object class list	*/
		case DS_REPETITION : 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E83IDX];
			break;

		/* no value for mandatory-attribute/object-class	*/
		case DS_NO_VALUE: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E84IDX];
			break;

		/* Naming attribute does not belong to object classes   */
		/* referenced in SRT entry.			        */
		case DS_NAME_ATTR_INVALID: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E88IDX];
			break;

		/* Modification in an OCT entry leads to a situation when */
		/* naming attribute in an SRT entry does not belong to    */
		/* some object class referenced in that SRT entry.	  */
		case DS_OPT_MAND_NAME_ATTR: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E90IDX];
			break;

		/* Entry of default schema can not be deleted.		*/
		case DS_DEL_DEF_SCH: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E91IDX];
			break;

		/* SRT entry of default schema can not be modified.	*/
		case DS_MOD_DEF_SRT: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E92IDX];
			break;

		/* Obj. id. in default schema can not be changed.	*/
		case DS_MOD_OID_DEF_SCH: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E93IDX];
			break;

		/* Sup. obj. class of object class of default schema 	*/
		/* can not be changed.					*/
		case DS_MOD_SOBJ_DEF_OCT: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E94IDX];
			break;

		/* Mand. attribute of object class of default schema 	*/
		/* can not be changed.					*/
		case DS_MOD_MAND_DEF_OCT: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E95IDX];
			break;

		/* wrong rule no. to create a new structure rule.	*/
		case DS_WR_RULE_NR: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E96IDX];
			break;

		/* too many rules involved to generate a DN	*/
		case DS_TOO_MANY :
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E97IDX];
			break;

		/* Syntax of DEFAULT schema entry can not be changed. 	*/
		case DS_MOD_SNTX_DEF_AT: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_err_table[D20_E105IDX];
			break;

		/* Constraint violation in super class usage.	*/
		case DS_WR_KIND_SCLASS: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E61IDX];
			break;

		/* naming attr. must have case ignore syntax.		*/
		case DS_WR_NAM_SNTX: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E48IDX];
			break;

		/* naming attribute must have +ve no. for recurring val.*/
		case DS_WR_NAM_RECR: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E49IDX];
			break;

		/* Mand. attr. appears in one of the sup. obj. classes. */
		/* as mand. attr.					*/
		case DS_MAND_SUP: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errtab[D20_E40IDX];
			break;

		/* Opt. attr. appears in one of the sup. obj. classes. */
		/* as mand. or opt. attr.			       */
		case DS_OPT_SUP: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errtab[D20_E75IDX];
			break;

		/* Object class kind of def. schema entry can not be changed. */
		case DS_MOD_CK_DEF_SCH: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E62IDX];
			break;

		/* Schema can not have more than one obj. class of kind ALIAS */
		case DS_NO_MORE_ALIAS: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E63IDX];
			break;

		/* Invalid object class kind.	*/
		case DS_INVALID_OC_KIND: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E64IDX];
			break;

		/* too many structural object classes as super classes.  */
		case DS_TOO_MANY_STR_SC: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E65IDX];
			break;

		/* Modification of schema object class is not possible.	*/
		case DS_MOD_SCH: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E66IDX];
			break;

		/* Modification of an object class generates loop in object */
		/* class hierarchy.					    */
		case DS_SCLASS_LOOP: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E67IDX];
			break;

		/* Constraint violation: Class is being used as an AUX. class */
		case DS_AUX_CONS_VIOL: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E68IDX];
			break;

		/* Only structural kind of obj. class can have AUX. classes */
		case DS_ONLY_STR_AUX: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E69IDX];
			break;

		/* Auxiliary class should be of kind AUXILIARY */
		case DS_AUX_AUX: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E70IDX];
			break;

		/* Constraint violation: Class is being used as super class */
		case DS_SCLASS_CONS_VIOL: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E71IDX];
			break;

		/* Constraint violation: Class is being used in a str. rule */
		case DS_SRULE_CONS_VIOL: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E72IDX];
			break;

		/* structural object class does not have GTP as one of its */
		/* super classes.					   */
		case DS_GTP_MISSING: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E73IDX];
			break;

		/* Object class in a structure rule should be of kind STRUCTURAL
*/
		case DS_SRULE_STR: 
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E74IDX];
			break;

		/* attributes with PR_DM_STX and MHS_PR_DM_STX should not be */
		/* multi-valued.					     */
		case DS_PR_DM_STX_RECUR:
			d20_qomask[D20_Q1POS].d20_ioinfo=
						d20_errmess[D20_E76IDX];
			break;

    		default: 
			sprintf(message, "Undefined message ! Err-Value = %d !",
							ret_code.errvalue);
			d20_qomask[D20_Q1POS].d20_ioinfo = message;
			break;
		}
  	d20_handlemask (d20_qomask) ;
  	ret_value = D2_ERROR;
	}

return(ret_value);

}		/* end of d20_032_update()		*/
