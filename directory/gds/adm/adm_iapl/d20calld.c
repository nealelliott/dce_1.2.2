/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20calld.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:47  root]
 * 
 * Revision 1.1.8.4  1994/07/06  15:06:17  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:10:35  marrek]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:13  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:43:52  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:55  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:08  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:16  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:14:02  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:38:14  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:08:52  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:45:06  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:35  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  13:39:18  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:35:16  marrek]
 * 
 * Revision 1.1.4.2  1992/09/28  11:45:11  marrek
 * 	Move d20_a7_addalias() into #ifndef CACHE_ADMIN.
 * 	[1992/09/28  11:44:25  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:53:21  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:31:50  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  21:27:56  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:07:37  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20calld.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:43 $";
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
/* NAME         : d20calld.c                                          */
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
/*  FUNCTION:  ds4_b11_call_directory ()                                     */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function calls a specific directory system function (depending   */
/*      on operation code) and handles the returned error (if there is any)  */
/*      by displaying an appropriate error message on the screen.            */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      operation   = Operation code of the directory system function which  */
/*                    should be called.                                      */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    Depends on called directory system function.           */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D2_NOERROR = The function has been performed successfully.           */
/*      D2_ERROR   = An error is occurred (specific error information        */
/*                    is displayed on the screen).                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 4.3.88       */
/*exoff **********************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <d21iapl.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2info.h>
#include <d21.h>
#include <d20proto.h>

/* -------------------- declaration of local data -------------------------- */

extern char *d20_errmess[];
extern signed32 rbatch;
extern D20_omask d20_qomask[] ;

#ifndef CACHE_ADMIN
static d2_ret_val (*d20_fkttab[]) (D2_pbhead *) = { 
				         d20_a0_read,     d20_a1_addobj,
				         d20_a2_delobj,   d20_a3_search,
				         d20_a3_search,
				         d20_a4_addattr,  d20_a5_delattr,
				         d20_a6_modattr,
				         d20_a7_addalias,
				         d20_a11_modrdn
				       } ;
#else
static d2_ret_val (*d20_fkttab[]) (D2_pbhead *) = { 
				         d20_a0_read,     d20_a1_addobj,
				         d20_a2_delobj,   d20_a3_search,
				         d20_a3_search,
				         NULL,  NULL,
				         NULL,
				         d20_a7_addalias,
				         NULL
				       } ;
#endif

static D2_pbhead pbhead = { D21V022 } ;

/* ------------------------------------------------------------------------- */

 signed16 ds4_b11_call_directory (
   signed16 operation )
{
	signed16   err_val ;

	if ((err_val = ((signed16) (*d20_fkttab[operation]) (&pbhead))) == D2_ERROR) {

		/* handle error message */
		/* Note: the handling of errors should be done by using */
		/* appropriate error messages -> not possible now, because */
		/* the error values returned from the directory system are */
		/* inconsistent */

		d20_qomask[D20_Q1POS].d20_ioinfo = d20_errmess[map_errnum(pbhead.d2_errvalue)];
		d20_handlemask (d20_qomask) ;
	}
	return (err_val) ;
}

 signed16 map_errnum(
   signed16 errnum)
{
signed16 indx;

		switch ((int) errnum)
			{
			case D2_NO_BIND:
				indx = D20_E01IDX;
				break;	
			case D2_DIR_UNKNOWN:
				indx = D20_E02IDX;
				break;	
			case D2_SHADOW_UPD:
				indx = D20_E03IDX;
				break;	
			case D2_NO_SHADOW:
				indx = D20_E04IDX;
				break;	
			case D2_MEMORY_INSUFFICIENT:
				indx = D20_E05IDX;
				break;	
			case D2_CREDENTIALS_INVALID:
				indx = D20_E06IDX;
				break;	
			case D2_DSA_UNKNOWN:
				indx = D20_E07IDX;
				break;	
			case D2_INAPPR_AUTH:
				indx = D20_E08IDX;
				break;	
			case D2_INVALID_SIGNATURE:
				indx = D20_E09IDX;
				break;	
			case D2_PROTECTION_REQUIRED:
				indx = D20_E10IDX;
				break;	
			case D2_ACCESS_RIGHTS_INSUFFICIENT:
				indx = D20_E11IDX;
				break;	
			case D2_ATTRIBUTE_MISSING:
				indx = D20_E12IDX;
				break;	
			case D2_CONSTRAINT_VIOLATION:
				indx = D20_E13IDX;
				break;	
			case D2_INAPPR_MATCH:
				indx = D20_E14IDX;
				break;	
			case D2_UNDEFINED_ATTR_TYPE:
				indx = D20_E15IDX;
				break;	
			case D2_ATSYNTAX_ERROR:
				indx = D20_E16IDX;
				break;	
			case D2_AT_VALUE_EXISTS:
				indx = D20_E17IDX;
				break;	
			case D2_NFOUND:
				indx = D20_E18IDX;
				break;	
			case D2_ON_ALIAS_NOT_ALLOWED:
				indx = D20_E19IDX;
				break;	
			case D2_NMSYNTAX_ERROR:
				indx = D20_E20IDX;
				break;	
			case D2_ALIAS_ERROR:
				indx = D20_E21IDX;
				break;	
			case D2_TIME_LIMIT_EXCEEDED:
				indx = D20_E22IDX;
				break;	
			case D2_TOO_BUSY:
				indx = D20_E23IDX;
				break;	
			case D2_NO_RESPONSE:
				indx = D20_E24IDX;
				break;	
			case D2_CHAINING_REQUIRED:
				indx = D20_E25IDX;
				break;	
			case D2_UNABLE_TO_PROCEED:
				indx = D20_E26IDX;
				break;	
			case D2_INVALID_REFERENCE:
				indx = D20_E27IDX;
				break;	
			case D2_ADM_LIM_EXCEEDED:
				indx = D20_E28IDX;
				break;	
			case D2_LOOP_DETECTED:
				indx = D20_E29IDX;
				break;	
			case D2_UNAV_CRIT_EXTENSION:
				indx = D20_E30IDX;
				break;	
			case D2_OUT_OF_SCOPE:
				indx = D20_E31IDX;
				break;	
			case D2_DIT_INCONSISTENT:
				indx = D20_E32IDX;
				break;	
			case D2_ORPHAN:
				indx = D20_E33IDX;
				break;	
			case D2_ALREADY_EXISTS:
				indx = D20_E34IDX;
				break;	
			case D2_NOT_ALLOWED_ON_RDN:
				indx = D20_E35IDX;
				break;	
			case D2_NAMING_VIOLATION:
				indx = D20_E36IDX;
				break;	
			case D2_OBJ_CLS_VIOLATION:
				indx = D20_E37IDX;
				break;	
			case D2_AFFECTS_MULTIPLE_DSAS:
				indx = D20_E38IDX;
				break;	
			case D2_OBJCLS_MOD_PROHIBITED:
				indx = D20_E39IDX;
				break;	
			case D2_COMMUNICATION_ERROR:
				indx = D20_E40IDX;
				break;	
			case D2_NO_SUCH_OPERATION:
				indx = D20_E42IDX;
				break;	
			case D2_TOO_LATE:
				indx = D20_E43IDX;
				break;	
			case D2_CANNOT_ABANDON:
				indx = D20_E44IDX;
				break;	
			case D2_ABANDONED:
				indx = D20_E45IDX;
				break;	
			case D2_PAR_NFOUND:
				indx = D20_E46IDX;
				break;	
			case D2_NO_NEW_MASTER:
				indx = D20_E47IDX;
				break;	
			case D21_BAD_ARGUMENT:
				indx = D20_E77IDX;
				break;
			default:
				indx = D20_E00IDX;
				break;
			}

return(indx);
}
