/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27cbl.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:15:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:10  root]
 * 
 * Revision 1.1.8.1  1994/02/22  17:55:38  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:53:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:27:23  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:11:56  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:05:05  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:10:51  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:34:08  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27cbl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:32 $";
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
/*                                                                    */
/* NAME         : d27cbl.c  <create bitlist for country-syntax>       */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 27.08.91                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
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
/*   0.1    |17.12.87 |  Birth                         | ek |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

byte    d27_country_bitlist[D27_CNTRY_BITLIST_LEN];

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_609_compute_cntry_bitlist                       */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 23.08.91                                            */
/*                                                                    */
/* DESCRIPTION  : The function reads the list of countries from the   */
/*                input-file  and assigns a corresponding bit-mapping */
/*                to the global variable d27_country_bitlist:         */
/*                                                                    */
/*                The input-file contains  a list of all valid        */
/*                country-representations in two-letter alphabetical  */
/*                code ("alpha-2 code").                              */
/*                This function produces a bit-mapping, which         */
/*                corresponds with the list of all possible alpha-2   */
/*                combinations in the following manner:               */
/*                Whenever an alpha-2 code "XY" is a valid country-   */
/*                syntax, the bit in the place                        */
/*                " (26 * index of X) + index of Y "  is set to 1.    */
/*                                                                    */
/*                                                                    */
/* RETURNVALUE     :    D2_NOERROR  if processing successful          */
/*                      D2_ERROR  if processing failed                */
/*                                                                    */
/**********************************************************************/

signed32 d27_609_compute_cntry_bitlist(
    FILE *fp)           /* IN    -  file pointer to read countries    */

{
/*--------------------------------------------------------------------*/
/*                    D E C L A R A T I O N S                         */
/*--------------------------------------------------------------------*/

				    /* contains country and '\0'      */
  static char country[]={'\0','\0','\0'};
  static byte bitmask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

  signed16 i;
  unsigned32 bit_pos;
  byte *rel_byte_p;

/*--------------------------------------------------------------------*/
/*                    C O D E                                         */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------*/
/* set bitlist  to null                                         */
/*--------------------------------------------------------------*/
  for (i=0; i<D27_CNTRY_BITLIST_LEN; i++)
  {
    d27_country_bitlist[i] = '\0';
  }

/*--------------------------------------------------------------*/
/* scan to begin of ASCII country-names (marked by '#')         */
/*--------------------------------------------------------------*/
  while (country[0] != '#')
  {
    if (fscanf(fp, "%1s", country) != 1)
    {
      return (D2_ERROR);
    }
  }
/*--------------------------------------------------------------*/
/* scan all countries up to the end (marked by '#')             */
/*--------------------------------------------------------------*/
  country[0]='\0';

  while (country[0] != '#')
  {
    if ( fscanf(fp, "%2s", country) == 1)
    {
      if (country[0] != '#')
      {
	/*---------------------------------------------------------*/
	/* input not correct  =>  return (error)                   */
	/*---------------------------------------------------------*/
	if ( country[0] < 0x41  ||
	     country[0] > 0x7a  ||
	    (country[0]>0x5a && country[0]<0x61)  ||
	     country[1] < 0x41  ||
	     country[1] > 0x7a  ||
	    (country[1]>0x5a && country[1]<0x61)     )
	{
	  return (D2_ERROR);
	}
	/*---------------------------------------------------------*/
	/* convert small letters to capital letters                */
	/*---------------------------------------------------------*/
	if ( country[0] >= 0x61  &&  country[0] <= 0x7a )
	{
	   country[0] -= 32;
	}
	if ( country[1] >= 0x61  &&  country[1] <= 0x7a )
	{
	   country[1] -= 32;
	}
	/*---------------------------------------------------------*/
	/* compute position of relevant byte in country-bitlist    */
	/* and set relevant bit in relevant byte                   */
	/*---------------------------------------------------------*/
	bit_pos = 26 * (country[0]-65) + (country[1]-65);
	rel_byte_p = &d27_country_bitlist [bit_pos/8];

	*rel_byte_p = *rel_byte_p | bitmask[bit_pos%8];
      }
    }
    else
    {
      return (D2_ERROR);
    }
  }                                                  /* end while */

  return (D2_NOERROR);

}

