/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20ffilt.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  00:29 UTC  dtruong
 * 	Merge to DCE 1.1
 * 
 * 	HP revision /main/HPDCE01/1  1994/04/08  00:02 UTC  dtruong
 * 
 * 	HP revision /main/dtruong_mothra/1  1994/04/07  22:08 UTC  dtruong
 * 	Fix object class bug in gdsditadm
 * 	[1995/12/08  14:49:53  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:00  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:25  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:14:49  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:42:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:09:48  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:45:30  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:53  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:39:52  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  09:02:06  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:31:23  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:08:03  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20ffilt.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:49 $";
static char rcsid[] = "@(#)$Id: d20ffilt.c,v 1.1.10.2 1996/02/18 19:38:49 marty Exp $";
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
/* NAME         : d20ffilt.c                                          */
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
/*  FUNCTION:  d20_ffilter ()                                                */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function converts one or more filter attributes, given in a      */
/*      format used by the administration, into the 'list'-format used at    */
/*      the IAPL-interface of the directory system V2.                       */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      optype      = Operation mode:                                        */
/*                    The operation mode is controlled by a 'bit field'-     */
/*                    mask, where a specific mask can be generated by ORing  */
/*                    the following 'bit field'-values:                      */
/*                       D20_BEGIN    = initialize filter attribute list.    */
/*                       D20_CONTINUE = insert given attribute informations  */
/*                                      into filter attribute list.          */
/*      attr_ent    = Pointer to an array containing the informations about  */
/*                    the convertible filter attributes (attribute type,     */
/*                    attribute value, length of the attribute value).       */
/*                                                                           */
/*      no_attr     = No. of convertible filter attributes.                  */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      info        = Converted filter attribute(s).                         */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
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
extern D2_Fi_filter    d20_finfo[D2_NP_MAX+1] ;  /* filter information area */
extern Ds_v2_info      info ;  /* common IAPL-interface parameter block */
			       /* present or not			*/

/* ------------------------------------------------------------------------- */

#define WILDCARD    '*'


 void d20_ffilter (
   signed16     optype,
   Ds_at_entry *attr_ent,
   signed16     no_attr)
{
static D2_Fi_filter     *d20_filtptr ;
static Octet_string	d20_valptr ;
D2_Fi_item		*d2_fi_item;
signed16                len;
Octet_string            p_max;
register signed16	i, j ;
register Octet_string	p, p1 ;

if ((optype & D20_BEGIN) == D20_BEGIN)
	{ 
	if (no_attr == 0)
		{
		info.filter_present = FALSE;
		return;
		}
	else
		{
		/*initialize filter attribute list */

		d20_valptr = (Octet_string) info.avalues ;

		info.filter_present = TRUE;

		if (no_attr == 1)
			{
			info.filter.d2_fi_opmod = D2_ITEM;
			d2_fi_item = &(info.filter.d2_fi_v.d2_fi_item);
			}
		else
			{
			info.filter.d2_fi_opmod = D2_AND;
			info.filter.d2_fi_v.fi_and_or.d2_fi_no_fi = 0 ;
			info.filter.d2_fi_v.fi_and_or.d2_fi_filter = d20_finfo;
			d20_filtptr = 
			info.filter.d2_fi_v.fi_and_or.d2_fi_filter; 
			d20_filtptr->d2_fi_opmod = D2_ITEM;
			d2_fi_item = &(d20_filtptr->d2_fi_v.d2_fi_item);
			}
		}

	}

/* insert filter information into filter attribute list */
for (i = 0; i < no_attr; i++, attr_ent++) 
	{ 
	d2_fi_item->d2_fi_type.d2_type = attr_ent->attr_id.d2_type;
	d2_fi_item->d2_fi_type.d2_typ_len = attr_ent->attr_id.d2_typ_len;

	if (attr_ent->a_val.d2_a_v_len == 0)
		d2_fi_item->d2_fi_match = D2_PRESENT;
	else
		{ 
		len = attr_ent->a_val.d2_a_v_len;
		p1 = (Octet_string) attr_ent->a_val.d2_at_value.d2_oct_a_v;
		p_max = p1 + len; 

		/* check position of wild card (start or end) in the input */	
                /* only if attribute type is not object class.             */
                if  ((d20_cmp_objid(&(attr_ent->attr_id), sizeof(D2_OBJ_CLASS),
                      (Ob_id_string) D2_OBJ_CLASS) == 0) &&
                      (*p1 == WILDCARD || *(p_max - 1) == WILDCARD))
			{
			d2_fi_item->d2_fi_match = D2_SUBSTRINGS ;
			d2_fi_item->d2_fi_val.d2_fi_substrings.
			d2_fi_str_val.d2_a_rep = attr_ent->a_val.d2_a_rep;
			d2_fi_item->d2_fi_val.d2_fi_substrings.
			d2_fi_str_val.d2_at_value.d2_oct_a_v = d20_valptr;
			d2_fi_item->d2_fi_val.d2_fi_substrings.
			d2_fi_next = (D2_Fi_substr *)NULL;	

			if (*p1 == WILDCARD)
				{
				p1++;

				if (*(p_max - 1) == WILDCARD)
					{ 
					d2_fi_item->d2_fi_val.
					d2_fi_substrings.d2_fi_pos = D2_ANY;

					len = d2_fi_item->d2_fi_val.
					d2_fi_substrings.  d2_fi_str_val.
					d2_a_v_len = len - 2;

					if (len == 0)
						d2_fi_item->d2_fi_match
							 = D2_PRESENT ;
					}
				else
					{
					d2_fi_item->d2_fi_val.d2_fi_substrings.
							d2_fi_pos = D2_FINAL;

					len = d2_fi_item->d2_fi_val.
				        d2_fi_substrings.d2_fi_str_val.
				        d2_a_v_len = len - 1;
					}
				}
			else
				{
				/* last character is WILDCARD	*/
				d2_fi_item->d2_fi_val.d2_fi_substrings.
						d2_fi_pos = D2_INITIAL;
				len = d2_fi_item->d2_fi_val.
				      d2_fi_substrings.d2_fi_str_val.
				      d2_a_v_len = len - 1;
				}
			}
		else
			{ 
			d2_fi_item->d2_fi_match = D2_EQUAL;
			d2_fi_item->d2_fi_val.d2_fi_att_val.d2_a_v_len = len;
			d2_fi_item->d2_fi_val.d2_fi_att_val.d2_a_rep = 
						attr_ent->a_val.d2_a_rep;
			d2_fi_item->d2_fi_val.d2_fi_att_val.d2_at_value.
						d2_oct_a_v = d20_valptr;
			}

		for (p = d20_valptr, j = 0; j < len; j++)
			*p++ = *p1++;

		d20_valptr += len;

	       	}

	if (no_attr > 1)
		{
		info.filter.d2_fi_v.fi_and_or.d2_fi_no_fi++;
		d20_filtptr++;
		d20_filtptr->d2_fi_opmod = D2_ITEM;
	 	d2_fi_item = &(d20_filtptr->d2_fi_v.d2_fi_item);
		}
	}
}
