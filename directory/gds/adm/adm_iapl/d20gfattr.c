/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20gfattr.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:50  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:58  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:03  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:35  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:15:16  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:44:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:10:19  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:45:45  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:40:04  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  09:02:17  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:33:05  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:08:19  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20gfattr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:53 $";
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
/* NAME         : d20gfattr.c                                         */
/*								      */
/* AUTHOR       : Schmid , D AP 11                                    */
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
/*  FUNCTION:  d20_attr_from_message ()                                      */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function reads an attribute information queue, given in a for-   */
/*      mat used at the IPC-interface of the directory system V2, and        */
/*      returns the attribute information (attribute type, attribute value,  */
/*      length of the attribute value) contained in this queue (one attri-   */
/*      bute per function call resp. one attribute value per function call   */
/*      on recurring attributes).                                            */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      mode        = Operation mode:                                        */
/*                       D20__BEGIN   = Start reading at the beginning of    */
/*                                      the attribute information queue.     */
/*                       D20__CONTINUE= Continue reading of the attribute    */
/*                                      information queue from the actual    */
/*                                      position.                            */
/*      check       = indicates whether it must be checked whether the       */
/*                    attribute was already handled or not                   */
/*                       D2_TRUE      = check                                */
/*                       D2_FALSE     = no check                             */
/*      area        = Pointer to memory area containing IPC attribute message*/
/*      max_len     = max. length of attribute list                          */
/*      type        = Pointer at which the attribute type should be stored.  */
/*      len         = Pointer at which the length of the attribute value     */
/*                    is stored in the message block                         */
/*      rep         = Pointer at which the representation of the             */
/*                    attribute value should be stored.                      */
/*      vptr        = Pointer at which the attribute value should be stored. */
/*      p_mess      = Pointer to actual position in message                  */
/*                    (only if mode == D20__CONTINUE)                        */
/*      p_max       = maximal address of message                             */
/*                    (only if mode == D20__CONTINUE)                        */
/*      at_type     = last attribute type that was handled                   */
/*                    (only if mode == D20__CONTINUE)                        */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      type        = Attribute type.                                        */
/*      len         = address of length field of the attribute value.        */
/*      rep         = representation syntax ot the attribute value.          */
/*      vptr        = Attribute value.                                       */
/*      p_mess      = Pointer to new position in message                     */
/*      p_max       = maximal address of message (only if mode == D20_BEGIN) */
/*      at_type     = actual attribute type                                  */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D2_NOERROR  = The function has been performed successfully.          */
/*      D2_ERROR    = An error is occurred -> attribute information queue    */
/*                    is empty resp. the end of the queue is reached.        */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: W.Schmid                                      DATE: 4.5.88       */
/*exoff **********************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d2dump.h>
#include <d20proto.h>

 d2_ret_val d20_attr_from_message(
   signed16     mode,
   Bool         check,
   char        *area,
   signed32     max_len,
   D2_a_type   *type,
   signed16   **len,
   unsigned16 **rep,
   char       **vptr,
   char       **p_mess,
   char       **p_max,
   D2_a_type   *at_type)
{

signed16             length = D20_HDLD;
unsigned16           tag;

if (mode == D20__BEGIN)
   {      /* start reading at the beginning of the attribute message */
     *p_mess = area;
     *p_max  = area + max_len;
   }

for (*len = &length; **len == D20_HDLD; )
  { if (*p_mess >= *p_max)
       return(D2_ERROR);

    if ((tag = D2_LDSHORT( ((D23_at_type *)*p_mess)->d23_atttag ))
	     == D23_ATTTAG)
       { type->d2_typ_len = at_type->d2_typ_len =
			    D2_LDSHORT( ((D23_at_type *)*p_mess)->d23_atlen);

	 type->d2_type = at_type->d2_type =
				      (Ob_id_string)(*p_mess + D23_S_ATT);
	 *p_mess += D23_S_ATT + ((D23_at_type*)*p_mess)->d23_atoff;
	 if ((tag = D2_LDSHORT( ((D23_av_info*)*p_mess)->d23_avtag))
	     == D23_AVTAG) {
	     *rep = (unsigned16 *) & D2_LDSHORT(((D23_av_info *)*p_mess)->d23_asyntx);
	     *p_mess += D23_S_AV;
	     tag = D2_LDSHORT( ((D23_av_value *)*p_mess)->d23_avlen);}
	  if (tag < D23_MINTAG)
	  { 
	    *len = (signed16 *) & D2_LDSHORT(((D23_av_value *)*p_mess)->d23_avlen);
	    *rep = (unsigned16 *) & D2_LDSHORT(((D23_av_value *)*p_mess)->d23_avrep);
	    *vptr = *p_mess + D23_S_ATV;
	    *p_mess = *vptr + D2_LDSHORT(((D23_av_value *)*p_mess)->d23_avoff);
	    if (check == FALSE){
	      break;
	     }
	  }
       }
    else
     if (tag < D23_MINTAG)
     {
      *len = (signed16 *) & D2_LDSHORT(((D23_av_value *)*p_mess)->d23_avlen);
      *rep = (unsigned16 *) & D2_LDSHORT(((D23_av_value *)*p_mess)->d23_avrep);
      *vptr = *p_mess + D23_S_ATV;
      *p_mess = *vptr + D2_LDSHORT(((D23_av_value *)*p_mess)->d23_avoff);
      if (check == FALSE){
	      break;
	    }
     }
     else
     return(D2_ERROR);
  }

return (D2_NOERROR) ;
}
