/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21oldobj.c,v $
 * Revision 1.1.10.2  1996/02/18  19:40:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:10  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:28  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:05:00  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:42  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:58:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:25:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:47:24  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:24:42  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:48:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:03:11  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:24:27  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:37:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:49:51  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:14:04  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21oldobj.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:40:05 $";
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
/* NAME         : d21oldobj.o                                         */
/*							              */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 29.08.89                                            */
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
/*								      */
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 28.07.89| Erstellung                     | hv |         */
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

#include <dce/d2dir.h>
#include <gdsext.h>
#include <dce/d27util.h>
#include <d2dump.h>
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

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D2_pbhead *d20_head;     /* Error information structure        */
extern char      *d20_beg1;     /* pointer to beginning of memory area*/
				/* of DNs of list 1 (saved DNs)       */

/**********************************************************************/
/*								      */
/*		  D E C L A R A T I O N S      FUNCTIONS	      */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21t002_old_objects()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function creates a list of objects that already exists   */
/*      under the given object of the target DSA.                     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16       dn_no;       number of DNs handled so far      */
/*      D20_offset   **pdn;         pointer array refrencing DNs      */
/*                                  (DNs not leading to name          */
/*                                  collisions are marked with NULL   */
/*      Bool          *dn_name_col; name collision for every DN       */
/*      Name_string    old_par;     old parent name                   */
/*      Name_string    new_par;     new parent name                   */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Name_string  **old_objects; pointer of name collisions        */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/

 d2_ret_val d21t002_old_objects(
   signed16       dn_no,
   D20_offset   **pdn,
   Name_string  **old_objects,
   Bool          *dn_name_col,
   Name_string    old_par,
   Name_string    new_par)

{
signed32      i;
signed32      length = 0;       /* total length of all DN strings     */
signed32      no = 0;           /* total number of all DNs            */
signed32      mem_length;
char         *dn_area;
Name_string  *ptr_area;
D20_offset  **poff;
Bool         *sav_dn_col = dn_name_col;
char          dist_name[D2_DNL_MAX];
signed32      diff_len;

poff = pdn;
d21t010_change_dn((Name_string) (d20_beg1 + (*poff)->d20_mem_off), new_par,
		  old_par, dist_name);

diff_len = strlen(dist_name) - strlen(d20_beg1 + (*poff)->d20_mem_off) + 1;
/*
   "diff_len" already contains memory for EOS of new DN
*/

for (i = 0, poff = pdn; i < dn_no; i++, poff++, sav_dn_col++)
{   if (*sav_dn_col == TRUE)
    {   no++;
	length += strlen(d20_beg1 + (*poff)->d20_mem_off) + diff_len;
    }
}

if (no != 0)
{   mem_length = D2_ALIGN((no + 1) * sizeof(String)) + D2_ALIGN(length);
    if ((*old_objects = (Name_string *) malloc((size_t) mem_length))
		      == (Name_string *) NULL)
    {   d20_head->d2_errvalue = D20_ALLOC_ERR;
	return(D2_ERROR);
    }
    else
    {   ptr_area = *old_objects;
	dn_area = (char *) ptr_area + D2_ALIGN((no + 1) * sizeof(String));

	for (i = 0; i < dn_no; i++, pdn++, dn_name_col++)
	{   if (*dn_name_col == TRUE)
	    {   *ptr_area = (Name_string)dn_area;
		d21t010_change_dn((Name_string)(d20_beg1 + (*pdn)->d20_mem_off),
				  new_par, old_par, dn_area);
		dn_area += strlen(dn_area) + 1;
		ptr_area++;
	    }
	}

	*ptr_area = NULL;
    }
}

return(D2_NOERROR);
}
