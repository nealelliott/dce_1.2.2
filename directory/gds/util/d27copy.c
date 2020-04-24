/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27copy.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:42  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:16:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:23  root]
 * 
 * Revision 1.1.8.1  1994/02/22  17:55:42  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:53:42  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:29:17  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:12:19  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:05:11  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:46  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:54:10  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:44:29  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:11:11  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:34:25  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27copy.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:44 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d27copy.c                                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 21.09.90                                            */
/*                                                                    */
/* COMPONENT    : DIR-X, UTIL                                         */
/*                                                                    */
/* DESCRIPTION  : The module contains functions, which copy or skip   */
/*                APDUs.                                              */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

/*****  local    functions   *****/

static void d27_504_copy_extensions(byte **from, byte **to);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d27_501_copy_common_arguments                       */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will copy all the parts of the        */
/*                common   arguments from "from" to "to". "to" must   */
/*                point to an area of memory of sufficient size.      */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d27_501_copy_common_arguments(
    byte **from,        /* IN    -  message to copy from              */
    byte **to)          /* OUT   -  message to copy to                */
{
			/* copy common argument structure             */
  *(D23_com_arg *)*to = *(D23_com_arg *)*from;
  *from += sizeof(D23_com_arg);
  *to   += sizeof(D23_com_arg);

			/* copy security-parameters                   */
  d27_503_copy_security_parameters(from,to);

  if(((D23_ds_name *)*from)->d23_dstag == D23_RQTAG)
  {                     /* requestor present, copy requestor          */
    d27_502_copy_distinguished_name(from,to);
  }

  d27_504_copy_extensions(from,to);

  return;

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d27_502_copy_distinguished_name                     */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will copy all the parts of the        */
/*                distinguished name (APDU + string) from "from" to   */
/*                "to". "to" must point to an area of memory of       */
/*                sufficient size.                                    */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d27_502_copy_distinguished_name(
    byte **from,        /* IN    -  message to copy from              */
    byte **to)          /* OUT   -  message to copy to                */
{
  signed16 offset, length;
  Bool     complete;

  complete = ((D23_ds_name *)*from)->d23_dscomplete;
  offset   = ((D23_ds_name *)*from)->d23_dsoff;
  length   = ((D23_ds_name *)*from)->d23_dslen;

			/* copy distinguished-name structure          */
  *(D23_ds_name *)*to = *(D23_ds_name *)*from;
  *from += sizeof(D23_ds_name);
  *to   += sizeof(D23_ds_name);

			/* copy distinguished name string             */
  memcpy(*to,*from,length);
  *from += offset;
  *to   += offset;

			/* copy undecoded part if necessary           */
  if(complete == FALSE)
  {
    offset   = ((D23_av_value *)*from)->d23_avoff;
    length   = ((D23_av_value *)*from)->d23_avlen;

			/* copy attribute value structure             */
    *(D23_av_value *)*to = *(D23_av_value *)*from;
    *from += sizeof(D23_av_value);
    *to   += sizeof(D23_av_value);

			/* copy attribute value string                */
    memcpy(*to,*from,length);
    *from += offset;
    *to   += offset;
  }

  return;

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d27_503_copy_security-parameters                    */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will copy all the parts of the        */
/*                security-parameters from "from" to "to". "to" must  */
/*                point to an area of memory of sufficient size.      */
/*                If from does not security-parameters, nothing is    */
/*                copied.                                             */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d27_503_copy_security_parameters(
    byte **from,        /* IN    -  message to copy from              */
    byte **to)          /* OUT   -  message to copy to                */
{
  if(((D23_se_parm *)*from)->d23_setag == D23_SECTAG)
  {                     /* security-parameters present                */
			/* copy security-parameters                   */
    *(D23_se_parm *)(*to) = *((D23_se_parm *)(*from));
    *from += sizeof(D23_se_parm);
    *to   += sizeof(D23_se_parm);
  }

  return;

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d27_504_copy_extensions                             */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will copy all the parts of the        */
/*                extensions          from "from" to "to". "to" must  */
/*                point to an area of memory of sufficient size.      */
/*                If from does not contain extensions,  nothing is    */
/*                copied.                                             */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*exoff ***************************************************************/

static void d27_504_copy_extensions(
    byte **from,        /* IN    -  message to copy from              */
    byte **to)          /* OUT   -  message to copy to                */
{
  signed16 offset;

  if(((D23_bit_info *)*from)->d23_btag == D23_BITTAG)
  {                     /* Bitstring is present                       */
    offset = ((D23_bit_info *)*from)->d23_boff;
			/* copy bitstring                             */
    *(D23_bit_info *)*to = *(D23_bit_info *)*from;
    *from += sizeof(D23_bit_info);
    *to   += sizeof(D23_bit_info);
    memcpy(*to,*from,offset);
    *from += offset;
    *to   += offset;
  }

  return;

}
