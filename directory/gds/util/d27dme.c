/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27dme.c,v $
 * Revision 1.1.6.2  1996/02/18  18:23:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:44  marty]
 *
 * Revision 1.1.6.1  1995/12/08  16:16:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:25  root]
 * 
 * Revision 1.1.4.3  1994/06/10  21:18:50  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:18  devsrc]
 * 
 * Revision 1.1.4.2  1994/03/23  15:47:28  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:33:51  keutel]
 * 
 * Revision 1.1.4.1  1994/02/22  17:55:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:53:51  marrek]
 * 
 * Revision 1.1.2.1  1993/10/04  17:44:27  keutel
 * 	Creation
 * 	[1993/10/04  17:43:29  keutel]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27dme.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:23:45 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1993          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/************************************************************************/
/*									*/
/* TYPE         : MODULE                                                */
/*                                                                      */
/* NAME         : d27dme.c                                              */
/*                                                                      */
/* AUTHOR       : Jochen Keutel					        */
/* DATE         : 13.09.93					        */
/*                                                                      */
/* DESCRIPTION  : The module contains functions for conversion of       */
/*		  dme attributes from structured form to octet string   */
/*		  and vice versa				        */
/*                                                                      */
/************************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>

#define STR_SHORTLEN 5 /* no. of ASCII chars needed to hold a numeric short */

/******** LOCAL FUNCTIONS ********/

static signed32 d27str_to_signed32 (char *);
static void d27signed32_to_str (signed32 , char *);

/************************************************************************/
/*									*/
/* NAME:	gds_decode_alt_addr					*/
/*									*/
/* DESCRIPTION:	This function is used to convert an alternate address	*/
/*		attribute from the encoded octet string format to the	*/
/*		structure D2_alt_addr. Memory for the structured form	*/
/*		will be allocated.					*/
/*									*/
/* RETURN VALUE: D2_NOERROR if successful				*/
/*		 D2_ERROR if not					*/
/*									*/
/* AUTHOR:	Jochen Keutel						*/
/* DATE:	13.09.93						*/
/*									*/
/************************************************************************/

d2_ret_val gds_decode_alt_addr (
  const D2_str * oct_str,	/* IN: encoded alternate address */
  D2_alt_addr ** alt_addr)	/* OUT: structured alternate address */
{
size_t alloc_len=0;
signed32 len;
char * scan = (char *)oct_str->d2_value;
Ob_id_string type_ptr;
d2_ret_val ret_value;
signed32 num = 0;
D2_obj_id * obj_id;

len = d27str_to_signed32(scan);
alloc_len += D2_ALIGN(len);
scan += STR_SHORTLEN + len;

while (scan < (char *)(oct_str->d2_value + oct_str->d2_size) )
  {
  num ++;
  len = d27str_to_signed32(scan);
  alloc_len += D2_ALIGN(len);
  scan += STR_SHORTLEN + len;
  }
  
alloc_len += sizeof(D2_alt_addr) + num * sizeof(D2_obj_id);

if ((*alt_addr = (D2_alt_addr *) malloc(alloc_len)) == NULL)
  {
  ret_value = D2_ERROR;
  }
else
  {
  ret_value = D2_NOERROR;
  scan = (char *)oct_str->d2_value;
  (*alt_addr)->d2_addr.d2_size = d27str_to_signed32(scan);
  (*alt_addr)->d2_addr.d2_value =
			(unsigned char *)(*alt_addr) + sizeof(D2_alt_addr);
  (*alt_addr)->d2_ob_id_set.d2_num = num;
  (*alt_addr)->d2_ob_id_set.d2_obj_id =
			(D2_obj_id *)((*alt_addr)->d2_addr.d2_value + 
					D2_ALIGN((*alt_addr)->d2_addr.d2_size));

  scan += STR_SHORTLEN;
  memcpy((*alt_addr)->d2_addr.d2_value,scan,(*alt_addr)->d2_addr.d2_size);

  scan += (*alt_addr)->d2_addr.d2_size;
  obj_id = (*alt_addr)->d2_ob_id_set.d2_obj_id;
  type_ptr = (Ob_id_string)obj_id + num * sizeof(D2_obj_id);
  while (scan < (char *)((oct_str->d2_value + oct_str->d2_size)) )
    {
    obj_id->d2_typ_len = d27str_to_signed32(scan);
    scan += STR_SHORTLEN;
    obj_id->d2_type = type_ptr;
    memcpy(obj_id->d2_type,scan,obj_id->d2_typ_len);
    scan += obj_id->d2_typ_len;
    type_ptr += D2_ALIGN(obj_id->d2_typ_len);
    obj_id++;
    }
  }
return (ret_value);
}
  
/************************************************************************/
/*									*/
/* NAME:	gds_encode_alt_addr					*/
/*									*/
/* DESCRIPTION:	This function is used to convert an alternate address	*/
/*		attribute from structure D2_alt_addr to the encoded	*/
/*		octet string format. Memory for the octet string 	*/
/*		will be allocated.					*/
/*									*/
/* RETURN VALUE: D2_NOERROR if successful				*/
/*		 D2_ERROR if not (malloc problem)			*/
/*									*/
/* AUTHOR:	Jochen Keutel						*/
/* DATE:	13.09.93						*/
/*									*/
/************************************************************************/

d2_ret_val gds_encode_alt_addr (
  const D2_alt_addr * alt_addr,	/* IN: structured alternate address */
  D2_str ** oct_str)		/* OUT: encoded alternate address */
{
d2_ret_val ret_value;
signed32 i;
size_t alloc_len = 0;
char * scan;

alloc_len += sizeof (D2_str);
alloc_len += STR_SHORTLEN + alt_addr->d2_addr.d2_size;
for (i=0; i<alt_addr->d2_ob_id_set.d2_num; i++)
  {
  alloc_len += STR_SHORTLEN +
		(alt_addr->d2_ob_id_set.d2_obj_id + i)->d2_typ_len;
  }
if ((*oct_str = (D2_str *)malloc(alloc_len)) == NULL)
  {
  ret_value = D2_ERROR;
  }
else
  {
  ret_value = D2_NOERROR;
  (*oct_str)->d2_size = alloc_len - sizeof (D2_str);;
  (*oct_str)->d2_value = (unsigned char *) (*oct_str + 1);
  scan = (char *)((*oct_str)->d2_value);
  d27signed32_to_str(alt_addr->d2_addr.d2_size,scan);
  scan += STR_SHORTLEN;
  memcpy(scan,alt_addr->d2_addr.d2_value,alt_addr->d2_addr.d2_size);
  scan += alt_addr->d2_addr.d2_size;
  for (i=0; i<alt_addr->d2_ob_id_set.d2_num; i++)
    {
    d27signed32_to_str((alt_addr->d2_ob_id_set.d2_obj_id + i)->d2_typ_len,scan);
    scan += STR_SHORTLEN;
    memcpy(scan,(alt_addr->d2_ob_id_set.d2_obj_id + i)->d2_type,
			(alt_addr->d2_ob_id_set.d2_obj_id + i)->d2_typ_len);
    scan += (alt_addr->d2_ob_id_set.d2_obj_id + i)->d2_typ_len;
    }
  }
return (ret_value);
}

static signed32 d27str_to_signed32(
  char * str)
{
char str_help [STR_SHORTLEN + 1];

memcpy (str_help,str,STR_SHORTLEN);
str_help [STR_SHORTLEN] = '\0';
return(atoi(str_help));
}

static void d27signed32_to_str (
  signed32 i,
  char * str)
{
sprintf((char *)str,"%*d",STR_SHORTLEN,i);
}
