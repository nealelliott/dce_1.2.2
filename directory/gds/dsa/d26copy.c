/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26copy.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:03  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:43:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:23  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:19  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:09:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:42:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:02:57  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:59:10  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:35  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:35:50  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:04:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:46:13  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:53:44  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26copy.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:54 $";
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
/* NAME         : d26copy.c                                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 21.09.90                                            */
/*                                                                    */
/* COMPONENT    : DIR-X, DSA, retrieval functions                     */
/*                                                                    */
/* DESCRIPTION  : The module contains functions, which copy APDUs.    */
/*                                                                    */
/*datoff **************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

/*****  internal Includes    *****/

/*****  local    functions   *****/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_c07_copy_req_attributes                         */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will copy all the requested attributes*/
/*                from "from" to "to".                     "to" must  */
/*                point to an area of memory of sufficient size.      */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                  String        *from;        points to requested   */
/*                                              attributes            */
/*                  String        *to;          points to memory to   */
/*                                              copy to.              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                  String        *from;        points to structure   */
/*                                              following requested   */
/*                                              attributes            */
/*                  String        *to;          points to memory after*/
/*                                              copy.                 */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d26_c07_copy_req_attributes(byte **from, byte **to)

{
signed16 count, i;

count  = ((D23_rq_info *)(*from))->d23_rqnbr;

			    /* copy requested info structure          */
*(D23_rq_info *)(*to) = *((D23_rq_info *)(*from));
*from += sizeof(D23_rq_info);
*to   += sizeof(D23_rq_info);

			    /* copy attribute types                   */
for(i = 0; i < count; i++)
{
  d26_c08_copy_attribute_type(from,to);
}

return;

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_c08_copy_attribute_type                         */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will copy an attribute type (structure*/
/*                and object-identifier-string) from "from" to "to".  */
/*                "to" must point to an area of memory of sufficient  */
/*                size.                                               */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                  String        *from;        points to attribute   */
/*                                              type.                 */
/*                  String        *to;          points to memory to   */
/*                                              copy to.              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                  String        *from;        points to structure   */
/*                                              following attribute   */
/*                                              type                  */
/*                  String        *to;          points to memory after*/
/*                                              copy.                 */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d26_c08_copy_attribute_type(byte **from, byte **to)

{
signed16 offset, length;

offset = ((D23_at_type *)(*from))->d23_atoff;
length = ((D23_at_type *)(*from))->d23_atlen;

			    /* copy attribute type     structure      */
*(D23_at_type *)(*to) = *((D23_at_type *)(*from));
*from += sizeof(D23_at_type);
*to   += sizeof(D23_at_type);

			    /* copy object identifier string          */
memcpy(*to,*from,length);
*from += offset;
*to   += offset;

return;

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_c09_copy_attribute_value                        */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 30.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will copy one attribute value         */
/*                (av_value structure and string) from "from" to "to".*/
/*                "to" must point to an area of memory of sufficient  */
/*                size.                                               */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                  String        *from;        points to attribute   */
/*                                              value structure.      */
/*                  String        *to;          points to memory to   */
/*                                              copy to.              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                  String        *from;        points to structure   */
/*                                              following attribute   */
/*                                              type                  */
/*                  String        *to;          points to memory after*/
/*                                              copy.                 */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d26_c09_copy_attribute_value(byte **from, byte **to)

{
signed16 offset, length;

offset = ((D23_av_value *)(*from))->d23_avoff;
length = ((D23_av_value *)(*from))->d23_avlen;

			    /* copy attribute value structure         */
*(D23_av_value *)(*to) = *((D23_av_value *)(*from));
*from += sizeof(D23_av_value);
*to   += sizeof(D23_av_value);


			    /* copy attribute value string            */
memcpy(*to,*from,length);
*from += offset;
*to   += offset;

return;

}
