/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20getattr.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:57  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:02  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:32  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:15:07  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:44:11  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:10:08  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:45:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:32:57  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:08:13  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20getattr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:51 $";
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
/* NAME         : d20getattr.c                                        */
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
/*  FUNCTION:  d20_get_attribute ()                                          */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The fucntion reads an attribute information queue, given in a for-   */
/*      mat used at the IAPL-interface of the directory system V2, and       */
/*      returns the attribute information (attribute type, attribute value,  */
/*      length of the attribute value) contained in this queue (one attri-   */
/*      bute per function call resp. one attribute value per function call   */
/*      on recurring attributes).                                            */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      mode        = Operation mode:                                        */
/*                       D20_BEGIN    = Start reading at the beginning of    */
/*                                      the attribute information queue.     */
/*                       D20_CONTINUE = Continue reading of the attribute    */
/*                                      information queue from the actual    */
/*                                      position.                            */
/*                       D20_NEXT     = Attributes for the next DN in the    */
/*                                      queue should be handled              */
/*      type        = Pointer at which the attribute type should be stored.  */
/*      a_val       = Pointer at which the attribute should be stored        */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      type        = Attribute type.                                        */
/*      a_val       = Attribute                                              */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D2_NOERROR  = The function has been performed successfully.          */
/*      D2_ERROR    = An error is occurred -> attribute information queue    */
/*                    is empty resp. the end of the queue is reached.        */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 4.3.88       */
/*exoff **********************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2info.h>
#include <d20proto.h>

/* -------------------- declaration of global data ------------------------- */

extern Ds_v2_info	info ;	/* common IAPL-interface parameter block */

D2_ei_info     *d20_entptr ;    /* pointer to actual entry information   */

/* ------------------------------------------------------------------------- */

 d2_ret_val d20_get_attribute (
   signed16     mode,
   D2_a_type   *type,
   D2_a_value **a_val)
{
	static D2_a_info	*attptr ;
	static D2_a_value	*valptr ;
	static signed16		recno ;

	if (mode == D20_BEGIN)
		/* start reading at the beginning of the attribute information queue */
	   {    if (info.resentry == (D2_ei_info *) NULL)
			/* error -> attribute information queue is empty */
			return (D2_ERROR) ;
		attptr = info.resentry->d2_ei_ai ;
		recno = 0 ;
	   }
	else
	   if (mode == D20_NEXT)
	      { attptr = d20_entptr->d2_ei_ai ;
		recno = 0 ;
	      }

	/* check for no. of values for an attribute, if 0 go to next attr. */
	while (attptr != (D2_a_info *) NULL)
		{
	  	if (attptr->d2_a_no_val != 0)
	       		break;
	  	else
	       		attptr = attptr->d2_a_next;
		}

	if (attptr == (D2_a_info *) NULL)
		/* error -> end of attribute information queue is reached */
		return (D2_ERROR) ;

	if (recno == 0)
		valptr = attptr->d2_a_val ;

	/* attribute value(s) available, return attribute information */
	type->d2_type= attptr->d2_a_type.d2_type ;
	type->d2_typ_len = attptr->d2_a_type.d2_typ_len ;
	*a_val = valptr;

	if (++recno == attptr->d2_a_no_val) {
		/* go to next attribute queue entry */
		attptr = attptr->d2_a_next ;
		recno = 0 ;
	} else
		/* recurring attribute -> go to next attribute value */
		valptr++ ;

	return (D2_NOERROR) ;
}
