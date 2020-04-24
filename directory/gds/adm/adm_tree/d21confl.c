/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21confl.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:58  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:15:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:20  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:54  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:07:12  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:24:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:40:08  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:22:59  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:48:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:02:43  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:21:58  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:35:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:48:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:13:10  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21confl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:56 $";
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
/* NAME         : d21confl.o                                          */
/*							              */
/* AUTHOR       : N. Serra (CDS)                                      */
/* DATE         : 24.01.90                                            */
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
/*  FUNCTION:  d21t068_conflicts()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function creates a list of objects that already exists   */
/*      under the given object of the target DSA.                     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed32       dn_no;       number of DNs handled so far      */
/*      D20_offset   **pdn;         pointer array refrencing DNs      */
/*                                  (DNs not leading to name          */
/*                                  collisions are marked with NULL   */
/*      Bool          *dn_name_col; name collision for every DN       */
/*      char          *old_dn;      old Distinguished name            */
/*      char          *new_dn;      new Distinguished name            */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Name_string  **conflicts; pointer of name collisions          */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR  = The function has been performed successfully.   */
/*      D2_ERROR    = function fails                                  */
/*                                                                    */
/*exoff ***************************************************************/

 d2_ret_val d21t068_conflicts(
   signed32      dn_no,
   D20_offset  **pdn,
   Name_string **conflicts,
   Bool         *dn_name_col,
   char         *old_dn,
   char         *new_dn)

{
signed32      i;
signed32      length = 0;       /* total length of all DN strings     */
signed32      no = 0;           /* total number of all DNs            */
signed32      mem_length;
signed32      diff_len;
char         *p_dn;
char         *dn_area;
Name_string  *ptr_area;
D20_offset  **poff;
Bool         *sav_dn_col = dn_name_col;
char          new_name[D2_DNL_MAX+1];

diff_len = strlen(new_dn) - strlen(old_dn);

for (i = 0, poff = pdn; i < dn_no; i++, poff++, sav_dn_col++)
{   if (*sav_dn_col == TRUE)
    {   no++;
	length += strlen(d20_beg1 + (*poff)->d20_mem_off) + diff_len + 1;
    }
}

if (no != 0)
{   mem_length = D2_ALIGN((no + 1) * sizeof(String)) + D2_ALIGN(length);
    if ((*conflicts = (Name_string *) malloc((size_t) mem_length))
		      == (Name_string *) NULL)
    {   d20_head->d2_errvalue = D20_ALLOC_ERR;
	return(D2_ERROR);
    }
    else
    {   ptr_area = *conflicts;
	dn_area = (char *) ptr_area + D2_ALIGN((no + 1) * sizeof(String));

	for (i = 0; i < dn_no; i++, pdn++, dn_name_col++)
	{   if (*dn_name_col == TRUE)
	    {   *ptr_area = (Name_string)dn_area;
                d21t045_build_dn(old_dn, new_dn, 
				 d20_beg1 + (*pdn)->d20_mem_off, new_name);
		p_dn = new_name;
		strcpy(dn_area, p_dn);
		dn_area += strlen(p_dn) + 1;
		ptr_area++;
	    }
	}

	*ptr_area = NULL;
    }
}

return(D2_NOERROR);
}
