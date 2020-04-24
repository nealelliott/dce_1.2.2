/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27sntx.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:52  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:16:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:39  root]
 * 
 * Revision 1.1.8.2  1994/06/21  14:48:34  marrek
 * 	June 1994 code submission.
 * 	[1994/06/20  13:03:53  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  19:21:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:55:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:38:18  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:14:21  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:20:25  marrek
 * 	Insert <pthread.h>
 * 	Introduce d27_606_is_IA5String
 * 	[1993/02/02  09:48:16  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:06:01  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:49:17  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:55:20  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:45:19  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:12:18  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:35:14  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27sntx.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:56 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d27sntx.c <Utility-Functions for DSA-Syntax-check>  */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 08.04.91                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |08.04.91 |  Original                      | GR |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*--------------------------------------------------------------------*/
/*                        I N C L U D E S                             */
/*--------------------------------------------------------------------*/

#include <gds.h>

/*****  Operating System - Includes  *****/

#include <time.h>

/*****  external Includes  *****/

#include <dce/d27util.h>
#include <malgcfe.h>

/*--------------------------------------------------------------------*/
/*                 D E C L A R A T I O N S        DATA                */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                 D E C L A R A T I O N S        FUNCTIONS           */
/*--------------------------------------------------------------------*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d27_605_is_PrtString                                */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 09.04.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of Printable characters only, that is Capital letters,*/
/*      small letters, digits, space, Apostrophe, '(', ')', '+', ',', */
/*      '-', '.', '/', ':', '=' and '?'.                              */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d27_605_is_PrtString(
    byte *val,          /* IN    -  string to check                   */
    signed16 len)       /* IN    -  length of string to check         */

{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
byte           *curr_char, *max_addr;
signed16        char_val;

/*--------------------------------------------------------*/
/* ranges for graphic characters                          */
/*--------------------------------------------------------*/
#define D27_DIGIT_0      0x30
#define D27_DIGIT_9      0x39
#define D27_SPACE        0x20
#define D27_L_BOUND_1    0x27
#define D27_U_BOUND_1    0x29
#define D27_L_BOUND_2    0x2b
#define D27_U_BOUND_2    0x2f
#define D27_COLON        0x3a
#define D27_EQUALSIGN    0x3d
#define D27_QUESTMARK    0x3f

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/
  curr_char = val;
  max_addr = val + len;
  while (curr_char < max_addr)
  {
    char_val = (signed16)*curr_char;
    if ( (char_val >= D27_SMALL_A   && char_val <= D27_SMALL_Z  )   ||
	(char_val >= D27_CAPITAL_A && char_val <= D27_CAPITAL_Z)   ||
	(char_val >= D27_DIGIT_0   && char_val <= D27_DIGIT_9  )   ||
	 char_val == D27_SPACE                                       ||
	(char_val >= D27_L_BOUND_1 && char_val <= D27_U_BOUND_1)   ||
	(char_val >= D27_L_BOUND_2 && char_val <= D27_U_BOUND_2)   ||
	 char_val == D27_COLON                                       ||
	 char_val == D27_EQUALSIGN                                   ||
	 char_val == D27_QUESTMARK                                      )
    {
      curr_char ++ ;
    }
    else
    {
      return (D2_ERROR);
    }
  }                                                    /* end while  */

  return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d27_606_is_IA5String                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 28.01.93                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of IA5 characters only, that is Capital letters, that */
/*      are all 7 Bit characters.                                     */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d27_606_is_IA5String(
    byte *val,          /* IN    -  string to check                   */
    signed16 len)       /* IN    -  length of string to check         */

{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
byte *curr_char, *max_addr;

/*--------------------------------------------------------*/
/* range for IA5 characters                               */
/*--------------------------------------------------------*/

#define D27_IA5_UBOUND   0x7f

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/
  curr_char = val;
  max_addr = val + len;
  while (curr_char < max_addr)
  {
    if ((signed16)*curr_char <= D27_IA5_UBOUND)
    {
      curr_char ++ ;
    }
    else
    {
      return (D2_ERROR);
    }
  }                                                     /* end while  */

  return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d27_608_is_country                                  */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 22.04.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string. It must be one of the valid     */
/*      country names.                                                */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d27_608_is_country(
    byte *val)          /* IN    -  string to check                   */

{

/*--------------------------------------------------------------------*/
/*                 T Y P E -  D E F I N I T I O N S                   */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
  byte       inp[2];
  signed16   char1, char2;
  signed32   bit_idx;
  unsigned32 relevant_byte;

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/
  memcpy ((char *)inp, (char *)val, 2);

/*------------------------------------------------------------------*/
/* Convert input to Capital letters                                 */
/*------------------------------------------------------------------*/
  char1 = (signed16)*inp;
  char2 = (signed16)*(inp + 1);
  if ( char1 < D27_CAPITAL_A ||  char1 > D27_SMALL_Z ||
       (char1 > D27_CAPITAL_Z && char1 < D27_SMALL_A) ||
       char2 < D27_CAPITAL_A ||  char2 > D27_SMALL_Z ||
       (char2 > D27_CAPITAL_Z && char2 < D27_SMALL_A))
  {
    return (D2_ERROR);
  }
  else
  {
    if (char1 >= D27_SMALL_A)
    {
       *inp -= (D27_SMALL_A - D27_CAPITAL_A);
    }
    if (char2 >= D27_SMALL_A)
    {
       *(inp+1) -= (D27_SMALL_A - D27_CAPITAL_A);;
    }
  }
/*------------------------------------------------------------------*/
/* index of relevant bit: 26*index of first char + idx of 2nd char  */
/*------------------------------------------------------------------*/
  bit_idx = 26 * (*inp - 65) + (*(inp+1) - 65);

/*------------------------------------------------------------------*/
/* relevant byte in array has index bit_idx/8                       */
/* relevant byte must be shifted right by 7-(bit_pos%8)   =>        */
/* relevant bit is rightmost in relevant byte                       */
/*------------------------------------------------------------------*/
  relevant_byte = (unsigned32)d27_country_bitlist[bit_idx/8];

  if ((relevant_byte >> (7-bit_idx%8)) % 2 == 0)
  {
    return (D2_ERROR);
  }

  return (D2_NOERROR);

}
