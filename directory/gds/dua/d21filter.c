/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21filter.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:01  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:53:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:06  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:05:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:32:42  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:16:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:55:10  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:47:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:06  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:31:22  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:38:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:24:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:29  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21filter.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:58 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/* daton **************************************************************/
/*                                                                    */
/* NAME         :  d21filter.c                                        */
/*                                                                    */
/* AUTHOR       :  J.Heigert                                          */
/* DATE         :  03.02.88                                           */
/*                                                                    */
/* DESCRIPTION  :  Auxiliary routines for filter handling             */
/*                                                                    */
/* datoff *************************************************************/

#include <gds.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <d21dua.h>

/********** LOCAL **********/

static d2_ret_val d21_ca_calc_f_item_len( D2_Fi_filter * , signed32 * );
static d2_ret_val d21_ca_calc_f_f_len( D2_Fi_filter * , signed32 * );
static d2_ret_val d21_fiitem_iput( D2_Fi_filter * , char * , char ** );
static d2_ret_val d21_filter_iput( D2_Fi_filter * , char * , char ** );

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_f_item_len()                           */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 a filter-item in DUA.                              */
/*                                                                    */
/* INPUT-PARAMETER :  f_f : ptr to D2_Fi_filter                       */
/*                    f_len : start value of length                   */
/*                                                                    */
/* OUTPUT-PARAMETER :  calculated length                              */
/*                                                                    */
/* RETURN-VALUE :  Error Code                                         */
/*                                                                    */
/* exoff **************************************************************/

static d2_ret_val d21_ca_calc_f_item_len(
  D2_Fi_filter * f_f,
  signed32 * f_len)
{
	D2_Fi_substr       *sub_ptr;

	if ( (f_f->d2_fi_v.d2_fi_item.d2_fi_match > D2_APPROX_MATCH) ||
	   (f_f->d2_fi_v.d2_fi_item.d2_fi_match < D2_EQUAL)       ) {
		/* log error */
		return(D2_ERROR);
	}

	*f_len += D23_S_FT1 + D23_S_FI1 + D23_S_ATT
	  + D2_ALIGN(f_f->d2_fi_v.d2_fi_item.d2_fi_type.d2_typ_len);

	if (f_f->d2_fi_v.d2_fi_item.d2_fi_match == D2_PRESENT) {
		return(D2_NOERROR);
	}

	if (f_f->d2_fi_v.d2_fi_item.d2_fi_match == D2_SUBSTRINGS)
		/* space for first substring block */
	{
		sub_ptr = &(f_f->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings);

		while (sub_ptr != 0) {
			*f_len += D23_S_FS1 +
				d21_ca_calc_av_info(&sub_ptr->d2_fi_str_val,1);
			sub_ptr = sub_ptr->d2_fi_next;
		}

		return(D2_NOERROR);
	}

	/* endif ........->d2_fi_match == D2_SUBSTRINGS  */
	/*
	else : get length of attribute value
	*/

	*f_len += D23_S_ATV +
	d21_ca_calc_av_info(&f_f->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_att_val,1);
	return(D2_NOERROR);
}				 /* end of d21_ca_calc_f_item_len */

/**********************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_f_f_len()                              */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 a filter in DUA.                                   */
/*                                                                    */
/* INPUT-PARAMETER :  f_f : ptr to D2_Fi_filter                       */
/*                    f_len : start value of length                   */
/*                                                                    */
/* OUTPUT-PARAMETER :  calculated length                              */
/*                                                                    */
/* RETURN-VALUE :  Error Code                                         */
/*                                                                    */
/* exoff **************************************************************/

static d2_ret_val d21_ca_calc_f_f_len(
  D2_Fi_filter * f_f,
  signed32 * f_len)
{
	D2_Fi_filter * f_patt;
	signed16 i;
	d2_ret_val ret_value;

	if ( (f_f->d2_fi_opmod == D2_AND) || (f_f->d2_fi_opmod == D2_OR) ) {
		if (f_f->d2_fi_v.fi_and_or.d2_fi_no_fi < 2) {
			/* error */
			return(D2_ERROR);
		}
	}

	if ( (f_f->d2_fi_opmod == D2_AND) || (f_f->d2_fi_opmod == D2_OR) ) {
		*f_len += D23_S_FT1;
		f_patt = (D2_Fi_filter *) f_f->d2_fi_v.fi_and_or.d2_fi_filter;

		for ( i = 0; i < f_f->d2_fi_v.fi_and_or.d2_fi_no_fi; i++) {
			ret_value = d21_ca_calc_f_f_len(f_patt, f_len);

			if (ret_value == D2_NOERROR) {
				f_patt++;
			}
			else {
				return(D2_ERROR);
			}
		}

		return(D2_NOERROR);
	}

	/* endif AND or OR */

	if (f_f->d2_fi_opmod == D2_NOT) {
		*f_len += D23_S_FT1;
		f_patt = (D2_Fi_filter *) f_f->d2_fi_v.d2_fi_not;
		ret_value = d21_ca_calc_f_f_len(f_patt, f_len);
		return(ret_value);
	}

	/* endif NOT */

	if ( f_f->d2_fi_opmod == D2_ITEM) {
		ret_value =  d21_ca_calc_f_item_len(f_f, f_len);
		return(ret_value);
	}
	else {
		/* undefined opmode     */
		return(D2_ERROR);
	}
}				 /* end of d21_ca_calc_f_f_len  */

/**********************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_fi_len()                               */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 a filter in DUA.                                   */
/*                                                                    */
/* INPUT-PARAMETER :  f_f : ptr to D2_Fi_filter                       */
/*                    f_len : start value of length                   */
/*                                                                    */
/* OUTPUT-PARAMETER :  calculated length                              */
/*                                                                    */
/* RETURN-VALUE :  Error Code                                         */
/*                                                                    */
/* exoff **************************************************************/

d2_ret_val d21_ca_calc_fi_len(
  D2_Fi_filter * f_f,
  signed32 * f_len)
{
	D2_a_opmod         opmode;
	d2_ret_val           ret_value;

	if (f_f == NULL)
	{   return(D2_NOERROR);
	}

	opmode = f_f->d2_fi_opmod;

	if ( (opmode == D2_AND) || (opmode == D2_OR) || (opmode == D2_NOT) ) {
		/* process filter_pattern */
		ret_value = d21_ca_calc_f_f_len(f_f, f_len);
		return(ret_value);
	}

	if ( opmode == D2_ITEM ) {
		/* process  filter_item */
		ret_value = d21_ca_calc_f_item_len(f_f, f_len);
		return(ret_value);
	}
	else {
		/* undefined opmode     */
		return(D2_ERROR);
	}
}					 /* end of d21_ca_calc_fi_len */

/* *********************************************
the following routines are for internal use for
. calculating the length of filters,
. moving filters to ipc-apdu
for old and new filter-structures
************************************************ */

/*
Put filter to ipc-apdu
*/

d2_ret_val d21_fi_iput(
  D2_Fi_filter * from,
  char * to,
  char ** next)
{
d2_ret_val  ret_value;

if ( from->d2_fi_opmod == D2_ITEM )
	ret_value = d21_fiitem_iput(from, to, next);
else
	ret_value = d21_filter_iput(from, to, next);
return(ret_value);

} /* end of d21_fi_iput */

/*
Put filter-item
*/

static d2_ret_val d21_fiitem_iput(
  D2_Fi_filter * from,
  char * to,
  char ** next)
{
D23_ft1_info    *ft1_ptr;
D23_fi1_info    *fi1_ptr;
D23_fs1_info    *fs1_ptr;
D2_Fi_substr    *sub_ptr;

/*
move D23_ft1_info
*/
ft1_ptr  = (D23_ft1_info *) to;
ft1_ptr->d23_fp1tag   = D23_PATTAG;
ft1_ptr->d23_fp1mod   = D2_ITEM;
ft1_ptr->d23_fp1count = 0;
to = (char *) ++ft1_ptr;
/*
move D23_fi1_info
*/
fi1_ptr = (D23_fi1_info *) to;
fi1_ptr->d23_fi1tag   = D23_FIITAG;
fi1_ptr->d23_fi1match = from->d2_fi_v.d2_fi_item.d2_fi_match;
fi1_ptr->d23_fi1syntx = 0;
to = (char *) (fi1_ptr + 1);
d21_cm_cpyobj(&from->d2_fi_v.d2_fi_item.d2_fi_type, &to);
*next = to;

if (from->d2_fi_v.d2_fi_item.d2_fi_match == D2_PRESENT)
	{       /* all done */
	return(D2_NOERROR);
	}
if (from->d2_fi_v.d2_fi_item.d2_fi_match != D2_SUBSTRINGS)
	{       /* move attribute value */
        fi1_ptr->d23_fi1syntx  =
	    from->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_att_val.d2_a_rep;
	if (d21_ca5_attr_val_iput
	    (&from->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_att_val, to,
					next, fi1_ptr->d23_fi1syntx)
	     != D2_NOERROR)
	 {
	  return(D2_ERROR);
	  }

	return(D2_NOERROR);
	}
/*
filteritem with substrings :
   move D23_fs1_info block
*/
fs1_ptr = (D23_fs1_info *) *next;
fs1_ptr->d23_fs1tag        = D23_FISTAG;
fs1_ptr->d23_fs1position   =
	from->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_pos;
fi1_ptr->d23_fi1syntx  =
    from->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_str_val.d2_a_rep;

to = (char *) (fs1_ptr + 1) - D23_S_ATV;
*next = to;
if (d21_ca5_attr_val_iput
    (&from->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_str_val,
     to, next,
    from->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_str_val.d2_a_rep)
     != D2_NOERROR)
 {
  return(D2_ERROR);
  }

fs1_ptr->d23_fs1len        =
  from->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_str_val.d2_a_v_len;
fs1_ptr->d23_fs1off        = D2_ALIGN(fs1_ptr->d23_fs1len);

sub_ptr = (D2_Fi_substr *)
	  from->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_next;

while (sub_ptr != 0)
	{       /* move all substrings */
	fs1_ptr = (D23_fs1_info *) *next;
	fs1_ptr->d23_fs1tag        = D23_FISTAG;
	fs1_ptr->d23_fs1position   = sub_ptr->d2_fi_pos;

	to = (char *) (fs1_ptr + 1) - D23_S_ATV;
	*next = to;
	if (d21_ca5_attr_val_iput (&sub_ptr->d2_fi_str_val,to,
		next, sub_ptr->d2_fi_str_val.d2_a_rep) != D2_NOERROR)
	 {
	   return(D2_ERROR);
	  }
	fs1_ptr->d23_fs1len    = sub_ptr->d2_fi_str_val.d2_a_v_len;
	fs1_ptr->d23_fs1off        = D2_ALIGN(fs1_ptr->d23_fs1len);

	sub_ptr = sub_ptr->d2_fi_next;
	}

return(D2_NOERROR);
} /* end of d21_fiitem_iput */

/*
move filter
*/

static d2_ret_val d21_filter_iput(
  D2_Fi_filter * from,
  char * to,
  char ** next)
{
D23_ft1_info    *ft1_ptr;
signed16           i;
D2_Fi_filter    *f_patt;
signed16           f_number;
d2_ret_val             ret_value = D2_NOERROR;

if (from->d2_fi_opmod == D2_ITEM)
	{       /* move filter item */
	ret_value = d21_fiitem_iput(from, to, next);
	}
else
	{       /* move filters */
	/*
	move D23_ft1_info
	*/
	ft1_ptr = (D23_ft1_info *) to;
	ft1_ptr->d23_fp1tag   = D23_PATTAG;
	ft1_ptr->d23_fp1mod   = from->d2_fi_opmod;
	to += sizeof(D23_ft1_info);
	*next = to;
	if (ft1_ptr->d23_fp1mod != D2_NOT)
	    {
	     ft1_ptr->d23_fp1count = f_number =
				      from->d2_fi_v.fi_and_or.d2_fi_no_fi;
	     f_patt = (D2_Fi_filter *) from->d2_fi_v.fi_and_or.d2_fi_filter;
	     for ( i = 0; i < f_number; i++, to = *next)
			{
			ret_value = d21_filter_iput(f_patt, to, next);
			if (ret_value != D2_NOERROR) break;
			f_patt++;
			}
	     }
	else
	   {
	    ft1_ptr->d23_fp1count = 0;
	    f_patt = (D2_Fi_filter *) from->d2_fi_v.d2_fi_not;
	    ret_value = d21_filter_iput(f_patt, to, next);
	    }
	}
return(ret_value);
} /* end of d21_filter_iput */
