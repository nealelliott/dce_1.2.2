/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20fentryr.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:52  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:49  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:59  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:22  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:14:40  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:41:45  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:09:39  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:45:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:50  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:31:10  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:07:58  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20fentryr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:48 $";
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
/* NAME         : d20fentryr.c                                        */
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
/*  FUNCTION:  ds4_b03_fill_obj_fields ()                                    */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function converts a distinguished name (DN), given in the        */
/*      'string'-format used at the IAPL-interface of the directory system   */
/*      V2, into a format used by the administration.                        */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      name        = Pointer to the given DN.                               */
/*      ifields     = Reference of the fields, at which the name part values */
/*                    (RDN-values) of the converted DN should be stored.     */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      ifields     =  RDN-values of the given DN.                           */
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
#include <d20proto.h>

/* ------------------------------------------------------------------------- */

 void ds4_b03_fill_obj_fields (
   D2_name_string  name,
   char	          *ifields[])
{
	register signed16 i, no_ava, max ;
	register char	 *p = (char *)name ;
	char		 *ava_ptr[D2_NP_MAX * NO_NAM_ATTR] ;

	/* scan distinguished name string for AVAs */
	for (i = no_ava = 0, max = strlen (p); i < max; i++, p++) 
		{
		if (*p == D2_T61_REP || *p == D2_PRTBL_REP ||
		    *p == D2_NUM_REP || *p == D2_IA5_REP)
			{
			ava_ptr[no_ava++] = p + 1;
			continue;
			}

		if (*p == D2_AVA_SEP || *p == D2_RDN_SEP)
			{
			*p = D2_EOS;
			continue;
			}
		}

	/* transfer AVAs into output fields */
	for (i = 0; i < no_ava; i++)
		sprintf (ifields[i], "%-*.*s", DS_NP_VAL_LEN, 
			 strlen (ava_ptr[i]), ava_ptr[i]) ;
}

