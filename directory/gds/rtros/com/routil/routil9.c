/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil9.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:21  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:43  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:28  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:34:57  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:51  keutel
 * 	OT 11620
 * 	[1994/08/19  13:48:40  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:21  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:06  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:49  keutel
 * 	creation
 * 	[1994/03/21  13:25:41  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routil9.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:21 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1991 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE		: MODULE					      */
/*								      */
/* NAME         : routil9.c                                           */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : DEC-3rd-1991                                        */
/*								      */
/* COMPONENT    : RTS mapping functions                               */
/*								      */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*								      */
/* DESCRIPTION	: This modul provides functions	which		      */
/*								      */
/*   - are mapping timer and memory functions                         */
/*								      */
/* SYSTEM DEPENDENCE :  SINIX V5.4                                    */
/*								      */
/* HISTORY	:						      */
/*								      */
/* Vers.Nr. | Date    |	 Modification		       |mark| CR# EM# */
/* 1.00 K0  | 91-11-10|  original                      | gj |         */
/* 1.01 K0  | 92-02-18|  new function ro0045... by shk | shk|         */
/* 1.02 K0  | 92-05-07|  new: formEn/DecodedString...  | gj |         */
/* 03.00 K03| 94-06-16|  A0264044: Busy Flag: FILE.D   |skh |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*		Module Identification				      */
/**********************************************************************/

static char *sccsid =    "@(#) routil9.c 1.1 93/11/29   RTS-V2.00";

/*********************************************************************/
/*              S T D - I N C L U D E S                              */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <rogenrl.h>
#include <roerval.h>
#include <rouply0.h>
#include <routil0.h>


/*********************************************************************/
/*     D E F I N E S,  T Y P E D E F S  &   P R O T O T Y P E S      */
/*********************************************************************/


static char FAR* rts_asn1_skiptype PROTO((char FAR *z,char FAR *zm));
static char FAR* rts_asn1_length PROTO((char FAR *z,char FAR *zm,char FAR * FAR *z2));
static char FAR* rts_asn1_close PROTO((char FAR *z1,char FAR *z2,char FAR *limit));

unsigned long int FAR PASCAL ro0060_getCurrentTaskId (void);

/******************************************************************************/
/*                                                                            */
/*         P R O V I D E D   G L O B A L   F U N C T I O N S                  */
/*                                                                            */
/*  unsigned long int PASCAL ro0040_time (void);                              */
/*                                                                            */
/*  void PASCAL ro0041_memcpy (char FAR *destination,                         */
/*                             char FAR *source,                              */
/*                             unsigned long int length);                     */
/*                                                                            */
/*  void PASCAL ro0056_memmove (char FAR *destination,                        */
/*                              char FAR *source,                             */
/*                              unsigned long int length);                    */
/*                                                                            */
/*  unsigned long int PASCAL ro0042_memcmp (char FAR *string1,                */
/*                                          char FAR *string2,                */
/*                                          unsigned long int length);        */
/*                                                                            */
/*  void PASCAL ro0043_memset (char FAR *area, int byte,                      */
/*                             unsigned long int length);                     */
/*                                                                            */
/*  signed short int PASCAL ro0044_formString (signed short int stringtype,   */
/*                                             char FAR *asn1string,          */
/*                                             unsigned long int FAR *length, */
/*                                             unsigned long int value);      */
/*                                                                            */
/*  unsigned long int PASCAL  ro0045_memAndLenCmp (char FAR* pPointer1,       */
/*                                                 char FAR* pPointer2,       */
/*                                         unsigned long int ulLength1,       */
/*                                         unsigned long int ulLength2);      */
/*                                                                            */
/*  void PASCAL ro0050_strcpy (char FAR *destination,                         */
/*                             char FAR *source);                             */
/*                                                                            */
/*  void PASCAL ro0051_strcat (char FAR *destination,                         */
/*                             char FAR *source);                             */
/*                                                                            */
/*  signed short int PASCAL  ro0057_strcmp (char FAR *pString1,               */
/*                                          char FAR *pString2);              */
/*                                                                            */
/*  unsigned int PASCAL ro0053_strlen (char FAR *source);                     */
/*                                                                            */
/*  int  PASCAL ro0054_atoi (char * source);                                  */
/*                                                                            */
/*  int    PASCAL ro0055_itoa (int num, char *string, int radix);             */
/*                                                                            */
/*                       * ** skh: K03: A0264044: Busy Flag: FILE.D *         */
/*  signed long int FAR PASCAL ro0060_getCurrentTaskId (void);                */
/*                                                                            */
/******************************************************************************/

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0040_time                                         */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : NOV-11th-1991                                       */
/*								      */
/* SYNTAX	:						      */

  unsigned long int PASCAL ro0040_time(void)

/* DESCRIPTION  :   This function returns the current time in seconds */
/*                  that has elapsed since 00:00:00 GMT, January 1st  */
/*                  1970.                                             */
/*								      */
/* INPUT-PARAMETERS :   none                                          */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES :                                                    */
/*								      */
/*       >0   : number of seconds since January 1st 1970.             */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : none                                                     */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  return( (unsigned long int) time(NULL) );        /* get system time */

}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0041_memcpy                                       */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : NOV-11th-1991                                       */
/*								      */
/* SYNTAX	:						      */

void PASCAL ro0041_memcpy (
  char FAR *destination,
  char FAR *source,
  unsigned long int length)

/* DESCRIPTION  :   This function copies a block of <length> bytes    */
/*                  from <source> to <destination>.                   */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*    destination :  where to copy the bytes to.                      */
/*         source :  where to copy the bytes from.                    */
/*         length :  how many bytes to copy.                          */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES : none                                               */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : An overlapping of <area1> and <area2> is not allowed!!   */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  memcpy (destination, source, (size_t)length);
}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0042_memcmp                                       */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : NOV-11th-1991                                       */
/*								      */
/* SYNTAX	:						      */

unsigned long int PASCAL ro0042_memcmp (
  char FAR *area1,
  char FAR *area2,
  unsigned long int length)

/* DESCRIPTION  :   This function compares two mem-areas <area1> and  */
/*                  <area2> for a length of exactly <length> bytes.   */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*      area1 :  first compare area.                                  */
/*      area2 :  second compare area.                                 */
/*     length :  how many bytes to compare.                           */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES :                                                    */
/*								      */
/*      !=0   : if <area1> differs from <area2>.                      */
/*       =0   : if <area1> is the same as <area2>.                    */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : none                                                     */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  return ( memcmp (area1, area2, length) );

}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0043_memset                                       */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : NOV-15th-1991                                       */
/*								      */
/* SYNTAX	:						      */

void PASCAL ro0043_memset (
  char FAR *area,
  int byte,
  unsigned long int length)

/* DESCRIPTION  :   This function overwrites an <area> of the memory  */
/*                  with <byte>. This is done sequentially <length>   */
/*                  times.                                            */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*      area1 :  Startaddress in memory.                              */
/*       byte :  content of the area after the operation              */
/*     length :  how many bytes to overwrite.                         */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES :      none                                          */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : none                                                     */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  memset (area,byte,length);
  return;                                              /* return void */
}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0050_strcpy                                       */
/*								      */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : may-10th-1993                                       */
/*								      */
/* SYNTAX	:						      */

void PASCAL ro0050_strcpy (
  char FAR *destination,
  char FAR *source)

/* DESCRIPTION  :   This function copies a string                     */
/*                  from <source> to <destination>.                   */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*    destination :  where to copy the string to.                     */
/*         source :  where to copy the string from.                   */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES : none                                               */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS :                                                          */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  strcpy (destination, source);
}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0051_strcat                                       */
/*								      */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : may-10th-1993                                       */
/*								      */
/* SYNTAX	:						      */

void PASCAL ro0051_strcat (
  char FAR *destination,
  char FAR *source)

/* DESCRIPTION  :   This function appends/concatenates the string     */
/*                  from <source> onto <destination>.                 */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*    destination :  where to copy the string to.                     */
/*         source :  where to copy the string from.                   */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES : none                                               */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS :                                                          */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  strcat (destination, source);
}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0053_strlen                                       */
/*								      */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : jul-10th-1993                                       */
/*								      */
/* SYNTAX	:						      */

unsigned int  PASCAL ro0053_strlen (
  char  FAR * source)

/* DESCRIPTION  :   This function finds the length of                 */
/*                  the string which is given in the source.          */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*         source :  the input string.                                */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES : The length of the string.                          */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS :                                                          */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  return (strlen (source));
}

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0054_atoi                                         */
/*								      */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : jul-10th-1993                                       */
/*								      */
/* SYNTAX	:						      */

int  PASCAL ro0054_atoi (
  char  * source)

/* DESCRIPTION  :   This function converts the given string           */
/*                  to an integer.                                    */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*         source :  the input string.                                */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES : The integer value of the string.                   */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS :                                                          */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  return(atoi (source));
}

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0055_itoa                                         */
/*								      */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : jul-10th-1993                                       */
/*								      */
/* SYNTAX	:						      */

int  PASCAL ro0055_itoa (
  int num,               
  char  *string,
  int radix)               

/* DESCRIPTION  :   This function converts a integer to               */
/*                  a string.                                         */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*            num :  The input integer.                               */
/*         string :  The resultant string.                            */
/*          radix :  The Base of the value.                           */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES : pointer to the resultant string.                   */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS :                                                          */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  return (sprintf(string, "%d", num));
}

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0056_memmove                                      */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : NOV-11th-1991                                       */
/*								      */
/* SYNTAX	:						      */

void PASCAL ro0056_memmove (
  char FAR *destination,
  char FAR *source,
  unsigned long int length)

/* DESCRIPTION  :   This function copies a block of <length> bytes    */
/*                  from <source> to <destination> taking into        */
/*		    consideration  overlaps (copies where source and  */
/*		    target are same memory region  but different      */
/*		    offsets.                                          */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/*    destination :  where to copy the bytes to.                      */
/*         source :  where to copy the bytes from.                    */
/*         length :  how many bytes to copy.                          */
/*								      */
/* OUTPUT-PARAMETERS :  none                                          */
/*								      */
/* RETURN VALUES : none                                               */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : An overlapping of <area1> and <area2> is not allowed!!   */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
/* **************** S T A R T   O F   F U N C T I O N *************** */

  memmove (destination, source, (size_t)length);
}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0057_strcmp                                               */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL  ro0057_strcmp (
	char            FAR     *pString1,           /*  -> :               */
	char            FAR     *pString2)           /*  -> :               */
/*                                                                            */
/*     Checks whether the strings  provided in both the  pointers are same.  */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     pString1      = Pointer to first  string  to be  compared             */
/*     pString1      = Pointer to second string  to be  compared             */
/*                                                                            */
/* FORMAL OUT-PARAMETERS : None.                                              */
/*                                                                            */
/* PUSHED OUT EVENTS : None.                                                  */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*     OK     => values equal.                                                */
/*     ! OK   => values unequal                                               */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  None                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0057_strcmp*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro0057_strcmp")

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (pFnctName);

RM0003_RETURN (pFnctName,
               signed short int,
               strcmp (pString1, pString2))

}/*end-ro0057_strcmp*/
/* **************** E N D   O F   F U N C T I O N ******************* */


/*exon ************************************************************************/
/*		          						      */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0058_readPduLenEnv                                        */
/*                                                                            */
/* AUTHOR       : Chandrasekar.K                                              */
/* DATE         : 18-11-93                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed long int PASCAL  ro0058_readPduLenEnv (void )
/*                                                                            */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*                                                                            */
/* FORMAL OUT-PARAMETERS : None.                                              */
/*                                                                            */
/* PUSHED OUT EVENTS : None.                                                  */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*     10,000 if No env variable                                              */
/*     env variable is > 10,000 then the value                                */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  None                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0058_readPduLenEnv*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro0058_readPduLenEnv")
signed long int lLen = 0;
char            *pBuf;
static     char vEnvStr[10]="PDUSIZE";
#define PDULEN 10000
/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (pFnctName);
 pBuf = getenv(vEnvStr);
 if (pBuf == NULL) lLen = PDULEN; 
 else {
    lLen = ro0054_atoi( pBuf);	
    if (lLen < PDULEN ) lLen = PDULEN;
 }
RM0003_RETURN (pFnctName,
               signed long int,lLen)
               

}/*end-ro0058_readPduLenEnv*/
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0044_formString                                   */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : DEC-3rd-1991                                        */
/*								      */
/* SYNTAX	:						      */

signed short int PASCAL ro0044_formString (
  signed short int       stringtype,
  char FAR              *asn1string,
  unsigned long int FAR *length,
  unsigned long int      value)

/* DESCRIPTION  :   This function encodes the <value> to a valid      */
/*                  ASN1-String. The tag is determined by <stringtype>*/
/*                  i.e. if <stringtype>=RC0041_FormOCTET then the    */
/*                  ASN1-String is encoded as an octet-string etc...  */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/* stringtype :  determines the ASN1-type to be encoded.              */
/* asn1string :  target where the encoded ASN1-string can be found    */
/*     length :  limit for the string-field.                          */
/*      value :  this value shall be encoded.                         */
/*								      */
/* OUTPUT-PARAMETERS :                                                */
/*								      */
/*     length :  length of the encoded ASN1-string.                   */
/*								      */
/* RETURN VALUES :    error status (0= no error)                      */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : There are 4 possible encoding routines provided          */
/*           that are choosen by the <stringtype>-parameter:          */
/*            - RC0040_FormT61 encodes <value> as a Teletext-String   */
/*            - RC0041_FormOCTET encodes <value> as an Octet-String   */
/*            - RC0042_FormUTC encodes <value> as UTC-Time            */
/*            - RC0043_FormINTEGER encodes <value> as an integer      */
/*           when the UTC-Time is to encode, <value> shall be the     */
/*           number of seconds since 00:00:00, January 1st, 1970.     */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
  char st[100];
  char *z;
  int l;
  struct tm *tt;
  long r;
  /*static short qm[] =
  {31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  long yy, mm, dd, hh, mn, ss, dh, dm, quadrat,ds; */
  long dh, dm, ds; 
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (stringtype != RC0040_FormT61     &&  /* only 4 possible options */
      stringtype != RC0041_FormOCTET   &&
      stringtype != RC0042_FormUTC     &&
      stringtype != RC0043_FormINTEGER)
      return (R_PARMER);

  if (stringtype == RC0040_FormT61)     /* encode the Teletext String */
     {
      /* sprintf(st,"%ld\0",value); for GCC */                  /* creat the string */
      sprintf(st,"%ld%c",value,'\0');
      l=strlen(st);
      if ((l+2) > *length || (l+2) > 127 )       /* length is limited */
	  return(R_LENERR);
      asn1string[0]=0x14;                              /* the T-field */
      asn1string[1]=l;                                 /* the L-field */
      ro0050_strcpy(asn1string+2,st);
      *length = l+2;                        /* report the ASN1-length */
     }

  if (stringtype == RC0041_FormOCTET)       /* encode as Octet-String */
     {
      /* sprintf(st,"%ld\0",value); */                 /* creat the string */
      sprintf(st,"%ld%s",value,"\0");
      l=strlen(st);
      if ((l+2) > *length || (l+2) > 127 )       /* length is limited */
	  return(R_LENERR);
      asn1string[0]=0x04;                              /* the T-field */
      asn1string[1]=l;                                 /* the L-field */
      ro0050_strcpy(asn1string+2,st);
      *length = l+2;                        /* report the ASN1-length */
     }

  if (stringtype == RC0042_FormUTC)             /* encode as UTC-Time */
     {
      tzset();                              /* set the right timezone */
      value-=timezone;                  
      tt=gmtime((const time_t*) &value);
      sprintf(asn1string+2,"%.2d%.2d%.2d%.2d%.2d%.2d",
      tt->tm_year,tt->tm_mon+1,tt->tm_mday,tt->tm_hour,tt->tm_min,tt->tm_sec);
      if (timezone == 0)
	 {
	  asn1string[14] = 'Z';                      /* Zoulou format */
	  asn1string[15] = 0;
	 }
      else
	 {
	  if ((ds=timezone) < 0)                 /* difference to GMT */
	     {asn1string[14]='-'; ds=ds*-1; } /* only positive values */
	  else
	     {asn1string[14]='+'; }  /* '+' means westwards Greenwich */
	  dm = ds/60;
	  dh = dm/60;                     /* calc the time-difference */
	  dm -= 60*dh;
	  /* sprintf(asn1string+15,"%.2d%.2d\0",dh,dm);*/ /* add to string */
	   sprintf(asn1string+15,"%.2d%.2d%c",dh,dm,'\0'); /* add to string */
	 }
      asn1string[0]=0x17;                              /* the T-field */
      l=strlen(asn1string+2);
      if ((l+2) > *length || (l+2) > 127 )       /* length is limited */
	  return(R_LENERR);                           /* the L-field */
      asn1string[1]=l;
      *length=l+2;
     }

  if (stringtype == RC0043_FormINTEGER)          /* encode as Integer */
     {
      *asn1string=0x02;                          /* primitive integer */
      r=value;                     /* use the MAVROS-encoding routine */
      z=asn1string+1;
      if (r < 128)                                  /* left unchanged */
	 {
	  if (r >= -128)
	     {*z++ =1; *z++ =r;}
	  else if (r >= -0x8000)
		  {*z++ = 2; *z++ = (r>>8); *z++= r;}
	       else if (r >= -0x800000)
		       {
			*z++ = 3; *z++ = r>>16;
			*z++ = (r>>8)&0xFF; *z++ = r&0xFF;
		       }
		    else
		       {
			*z++ = 4; *z++ = r>>24; *z++ = (r>>16)&0xFF;
			*z++ = (r>>8)&0xFF; *z++ = r&0xFF;
		       }
	 }else if (r < 0x8000)
		  {*z++ = 2; *z++ = r>>8; *z++ = r&0xFF;}
	       else  if (r < 0x800000)
			{
			 *z++ = 3; *z++ = r>>16;
			 *z++ = (r>>8)&0xFF; *z++ = r&0xFF;
			}
		     else
			{
			 *z++ = 4; *z++ = r>>24; *z++ = (r>>16)&0xFF;
			 *z++ = (r>>8)&0xFF; *z++ = r&0xFF;
			}
      *length=(*(asn1string+1))+2;
     }
 return(OK);
}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0045_memAndLenCmp                                         */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

unsigned long int PASCAL  ro0045_memAndLenCmp (
char            FAR     *pPointer1,           /*  -> :               */
char            FAR     *pPointer2,           /*  -> :               */
unsigned long   int      ulLength1,           /*  -> :               */
unsigned long   int      ulLength2)           /*  -> :               */

/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Checks whether the length as well as the memory contents of the two    */
/*     pointer provided match. Returns "unequal" when this predicate is untrue*/
/*                                                                            */
/*     Note: This function is not something like a lexicographic compare, i.e.*/
/*           it does NOT "compare the data in memory with the smaller length  */
/*           with the same length of data in memory with the larger length    */
/*           and return a success if they match".                             */
/*                                                                            */
/*     Returns "unequal" if one of the following is untrue in that order :    */
/*     - Lengths are unequal.                                                 */
/*     - Lengths greater than zero and memory data unequal.                   */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     pPointer1      = Pointer to first  memory location.                    */
/*     pPointer2      = Pointer to second memory location.                    */
/*     ulLength1      = Length of data from pPointer1 to be compared.         */
/*     ulLength2      = Length of data from pPointer2 to be compared.         */
/*                                                                            */
/* FORMAL OUT-PARAMETERS : None.                                              */
/*                                                                            */
/* PUSHED OUT EVENTS : None.                                                  */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*     OK     => values equal.                                                */
/*     ! OK   => value (pPointer1) != value (pPointer2)                       */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  None                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0045_memAndLenCmp*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro0045_memAndLenCmp")
unsigned long   int      ulRetCode;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (pFnctName);

/*----------------------------------------------------------------------------*/
/* If lengths are unequal, data unequal.                                      */
/* If lengths are equal and null, data equal (nothing to be compared).        */
/* If lengths are equal and not null, the return value of ro0042_memcmp used. */
/*----------------------------------------------------------------------------*/
if      (ulLength1 != ulLength2)  ulRetCode = !OK;
else if (ulLength1 == LNULL)      ulRetCode = OK;
else     ulRetCode = ro0042_memcmp (pPointer1, pPointer2, ulLength1);

RM0003_RETURN (pFnctName, unsigned long int PASCAL, ulRetCode)

}/*end-ro0045_memAndLenCmp*/

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0046_formString                                   */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAY-6th-1992                                        */
/*								      */
/* SYNTAX	:						      */

signed short int PASCAL ro0046_formString (
  signed short int stringtype,
  char FAR * asn1string,
  unsigned long int FAR *length,
  unsigned long int value)

/* DESCRIPTION  :   This function encodes the <value> to a valid      */
/*                  ASN1-String but without Tag and Length -Field.    */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/* stringtype :  determines the ASN1-type to be encoded.              */
/* asn1string :  target where the encoded string can be found.        */
/*     length :  maximum size of the provided buffer (->asn1string).  */
/*      value :  this value shall be encoded.                         */
/*								      */
/* OUTPUT-PARAMETERS :                                                */
/*								      */
/*     length :  length of the encoded string.                        */
/*								      */
/* RETURN VALUES :    error status (0 = no error)                     */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : There are 4 possible encoding routines provided          */
/*           that are choosen by the <stringtype>-parameter:          */
/*            - RC0040_FormT61 encodes <value> as a Teletext-String   */
/*            - RC0041_FormOCTET encodes <value> as an Octet-String   */
/*            - RC0042_FormUTC encodes <value> as UTC-Time            */
/*            - RC0043_FormINTEGER encodes <value> as an integer      */
/*           when the UTC-Time is to encode, <value> shall be the     */
/*           number of seconds since 00:00:00, January 1st, 1970.     */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
  char st[RCro5m_MaxRef];
  char FAR *z;
  int l;
  struct tm *tt;
  long r;
  /*long yy, mm, dd, hh, mn, ss, dh, dm, quadrat,ds;*/
  long dh, dm, ds; 
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (stringtype != RC0040_FormT61     &&  /* only 4 possible options */
      stringtype != RC0041_FormOCTET   &&
      stringtype != RC0042_FormUTC     &&
      stringtype != RC0043_FormINTEGER)
      return (R_PARMER);

  if (stringtype == RC0040_FormT61)     /* encode the Teletext String */
     {
      /* sprintf(st,"%ld\0",value);*/                  /* creat the string */
      sprintf(st,"%ld%c",value,'\0');                  /* creat the string */
      l=strlen(st);
      if ( l > *length )                         /* length is limited */
	  return(R_LENERR);
      ro0050_strcpy(asn1string,st);
      *length = l;                          /* report the ASN1-length */
     }
  if (stringtype == RC0041_FormOCTET)       /* encode as Octet-String */
     {
      /* sprintf(st,"%ld\0",value); */                 /* creat the string */
      sprintf(st,"%ld%c",value,'\0');                  /* creat the string */
      l=strlen(st);
      if ( l > *length )                         /* length is limited */
	  return(R_LENERR);
      ro0050_strcpy(asn1string,st);
      *length = l;                          /* report the ASN1-length */
     }

  if (stringtype == RC0042_FormUTC)             /* encode as UTC-Time */
     {
      tzset();                              /* set the right timezone */
      value-=timezone;                  
      tt=gmtime((const time_t*) &value);
      sprintf(asn1string,"%.2d%.2d%.2d%.2d%.2d%.2d",
      tt->tm_year,tt->tm_mon+1,tt->tm_mday,tt->tm_hour,tt->tm_min,tt->tm_sec);
      if (timezone == 0)
	 {
	  asn1string[12] = 'Z';                      /* Zoulou format */
	  asn1string[13] = 0;
	 }
      else
	 {
	  if ((ds=timezone) < 0)                 /* difference to GMT */
	     {asn1string[12]='-'; ds=ds*-1; } /* only positive values */
	  else
	     {asn1string[12]='+'; }  /* '+' means westwards Greenwich */
	  dm = ds/60;
	  dh = dm/60;                     /* calc the time-difference */
	  dm -= 60*dh;
	  /* sprintf(asn1string+13,"%.2d%.2d\0",dh,dm);*/ /* add to string */
	  sprintf(asn1string+13,"%.2d%.2d%c",dh,dm,'\0'); /* add to string */
	 }
      l=strlen(asn1string);
      if ( l > *length )                         /* length is limited */
	  return(R_LENERR);
      *length=l;
     }

  if (stringtype == RC0043_FormINTEGER)          /* encode as Integer */
     {
      r=value;                     /* use the MAVROS-encoding routine */
      z=asn1string;
      if (r < 128)                                  /* left unchanged */
	 {
	  if (r >= -128)
	     {*length =1; *z++ =r;}
	  else if (r >= -0x8000)
		  {*length = 2; *z++ = (r>>8); *z++= r;}
	       else if (r >= -0x800000)
		       {
			*length = 3; *z++ = r>>16;
			*z++ = (r>>8)&0xFF; *z++ = r&0xFF;
		       }
		    else
		       {
			*length = 4; *z++ = r>>24; *z++ = (r>>16)&0xFF;
			*z++ = (r>>8)&0xFF; *z++ = r&0xFF;
		       }
	 }else if (r < 0x8000)
		  {*length = 2; *z++ = r>>8; *z++ = r&0xFF;}
	       else  if (r < 0x800000)
			{
			 *length = 3; *z++ = r>>16;
			 *z++ = (r>>8)&0xFF; *z++ = r&0xFF;
			}
		     else
			{
			 *length = 4; *z++ = r>>24; *z++ = (r>>16)&0xFF;
			 *z++ = (r>>8)&0xFF; *z++ = r&0xFF;
			}
     }
 return(OK);
}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0047_formEncodedString                            */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAY-6th-1992                                        */
/*								      */
/* SYNTAX	:						      */

signed short int PASCAL ro0047_formEncodedString (
  signed short int  stringtype,
  char FAR *        asn1string,
  unsigned long int FAR *length,
  char FAR *        inpstring,
  unsigned long int inplength)

/* DESCRIPTION  :   This function encodes the <inpstring> to a valid  */
/*                  ASN1-String with Tag-,Length- and Value-Field.    */
/*                  This means that <inpstring> is expected to be     */
/*                  the correct encoded V-Field of the corresponding  */
/*                  <stringtype>.                                     */
/*                  i.e. if <stringtype>=RC0041_FormOCTET then the    */
/*                  <inpstring> is encoded as an octet-string etc...  */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/* stringtype :  determines the ASN1-type to be encoded.              */
/* asn1string :  target where the encoded string can be found         */
/*     length :  limit for the provided buffer (->asn1string)         */
/*  inpstring :  input string address                                 */
/*  inplength :  length of input string                               */
/*								      */
/* OUTPUT-PARAMETERS :                                                */
/*								      */
/*     length :  length of the encoded string.                        */
/*								      */
/* RETURN VALUES :    error status (0= no error)                      */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : There are 4 possible encoding routines provided          */
/*           that are choosen by the <stringtype>-parameter:          */
/*            - RC0040_FormT61 encodes <inpstring> as Teletext-String */
/*            - RC0041_FormOCTET encodes <inpstring> as Octet-String  */
/*            - RC0042_FormUTC encodes <inpstring> as UTC-Time        */
/*            - RC0043_FormINTEGER encodes <inpstring> as an integer  */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
  char st[RCro5m_MaxRef];
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (stringtype != RC0040_FormT61     &&  /* only 4 possible options */
      stringtype != RC0041_FormOCTET   &&
      stringtype != RC0042_FormUTC     &&
      stringtype != RC0043_FormINTEGER)
      return (R_PARMER);

  if (*length > RCro5m_MaxRef)     /* protect buffer from overwriting */
      return(R_LENERR);

  if (stringtype == RC0040_FormT61)     /* encode the Teletext String */
     {
      st[0]=0x14;                                      /* the T-Field */
      st[1]=inplength;                                 /* the L-Field */
     }
  if (stringtype == RC0041_FormOCTET)       /* encode as Octet-String */
     {
      st[0]=0x04;                                      /* the T-Field */
      st[1]=inplength;                                 /* the L-Field */
     }
  if (stringtype == RC0042_FormUTC)             /* encode as UTC-Time */
     {
      st[0]=0x17;                                      /* the T-Field */
      st[1]=inplength;                                 /* the L-Field */
     }
  if (stringtype == RC0043_FormINTEGER)          /* encode as Integer */
     {
      st[0]=0x02;                                      /* the T-Field */
      st[1]=inplength;                                 /* the L-Field */
     }
 ro0041_memcpy (st+2, inpstring, inplength);           /* make a copy */
 ro0041_memcpy (asn1string,st, inplength+2);     /* make ASN-1 string */
 *length = inplength + 2;                   /* report the ASN1-length */
 return(OK);
}
/* **************** E N D   O F   F U N C T I O N ******************* */

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0048_formDecodedString                            */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAY-6th-1992                                        */
/*								      */
/* SYNTAX	:						      */

signed short int PASCAL ro0048_formDecodedString (
  signed short int  stringtype,
  char FAR *        asn1string,
  unsigned long int FAR *length,
  char FAR *        inpstring,
  unsigned long int inplength)

/* DESCRIPTION  :   This function decodes the ASN1-<inpstring> to a   */
/*                  String which contains only the packed V-Field.    */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/* stringtype :  determines the ASN1-type to be encoded.              */
/* asn1string :  target where the encoded string can be found         */
/*     length :  limit for the provided buffer (->asn1string)         */
/*  inpstring :  input string address                                 */
/*  inplength :  length of input string                               */
/*								      */
/* OUTPUT-PARAMETERS :                                                */
/*								      */
/*     length :  length of the encoded string.                        */
/*								      */
/* RETURN VALUES :                                                    */
/*           0  : if no error.                                        */
/*          <0  : serious error.                                      */
/*          >0  : decoding error.                                     */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : There are 3 possible decoding routines provided          */
/*           that are choosen by the <stringtype>-parameter:          */
/*            - RC0040_FormT61 decodes <inpstring> as Teletext-String */
/*            - RC0041_FormOCTET decodes <inpstring> as Octet-String  */
/*            - RC0042_FormUTC decodes <inpstring> as UTC-Time        */
/*            - RC0043_FormINTEGER decodes <inpstring> as an integer  */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
  char st[RCro5m_MaxRef];
  unsigned long int len;
  char FAR * rc;
/* **************** S T A R T   O F   F U N C T I O N *************** */

  if (stringtype != RC0040_FormT61     &&  /* only 4 possible options */
      stringtype != RC0041_FormOCTET   &&
      stringtype != RC0042_FormUTC     &&
      stringtype != RC0043_FormINTEGER)
      return (-R_PARMER);

  if (*length > RCro5m_MaxRef)     /* protect buffer from overwriting */
      return (-R_LENERR);

  if (stringtype == RC0040_FormT61)     /* encode the Teletext String */
     {
      if (inpstring[0]!= 20 && inpstring[0] != 52)
	  return (R_WRGTAG);
     }
  if (stringtype == RC0041_FormOCTET)       /* encode as Octet-String */
     {
      if (inpstring[0] != 4 && inpstring [0] != 36)   /* check for valid Tag */
	  return (R_WRGTAG);
     }
  if (stringtype == RC0042_FormUTC)             /* encode as UTC-Time */
     {
      if (inpstring[0] != 23 && inpstring [0] != 55)   /* check for valid Tag */
	  return (R_WRGTAG);
     }

  if (stringtype == RC0043_FormINTEGER)          /* encode as Integer */
     {
      if (inpstring[0] != 2)                   /* check for valid Tag */
	  return (R_WRGTAG);
     }
			 /* as the inpstring might be packed call the */
				 /* unpack routine (like MAVROS does) */
  rc=ro0049_OctMove (inpstring, inpstring+inplength, st, &len);
  if (rc == NULL)
      return (R_OCTPAK);                     /* no valid Octet-string */
  ro0041_memcpy(asn1string , st ,len);
 *length = len;
 return(OK);
}
/* **************** E N D   O F   F U N C T I O N ******************* */


/* ********** local functions for the following Octmove-fct ********* */

static char FAR* rts_asn1_skiptype(
	char FAR *z,
	char FAR *zm)
{
	if (z == 0) return((char FAR*)0);
	if (((*z++)&0x1F) == 31)
		while ((z < zm) && ((*z++)&0x80));
	if (z < zm)
		return(z);
	else{ /*  ASN1_CKERR(ASN1_ERR_TAG,z);  */
		return(0);
	}
}
/*--------------------------------------------------------------------*/
static char FAR* rts_asn1_length(
	char FAR *z,
	char FAR *zm,
	char FAR * FAR *z2)
{	register k,l;

	if (z == 0)
		return(0);
 	if (z >= zm){
	     /*   ASN1_CKERR(ASN1_ERR_TAG,z); */
		return(0);
	}

	if ((l = *z++) & 0x80){
		if (k = (l&0x7F)){
			l = 0;
			if (z+k > zm){
			   /*     ASN1_CKERR(ASN1_ERR_LLEN,z); */
				return(0);
			}
			while (k--) l = (l<<8)+ *z++;
		}else{	*z2 = 0; return(z);}
	}
	if ((*z2 = z + l) > zm){
	      /*  ASN1_CKERR(ASN1_ERR_LEN,z); */
		return(0);
	}
	return(z);
}
/*--------------------------------------------------------------------*/
static char FAR * rts_asn1_close(
	char FAR *z1,
	char FAR *z2, 
	char FAR *limit)
{
	if (z1 == 0) return(z1);
	if (limit){
		if (z1 != z2){
		      /*  ASN1_CKERR(ASN1_ERR_SHORT,z1); */
			return(0);
		}
		return(z1);
	}else{
		if ((z2 - z1) < 2){
		     /*   ASN1_CKERR(ASN1_ERR_BOUND,z1); */
			return(0);
		}
		if (z1[0] || z1[1]){
		    /*    ASN1_CKERR(ASN1_ERR_EOC,z1);  */
			return(0);
		}
		return(z1+2);
	}
}

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0049_OctMove                                      */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAY-6th-1992                                        */
/*								      */
/* SYNTAX	:						      */

char FAR * PASCAL ro0049_OctMove (
  char FAR *        z,
  char FAR *        zm,
  char FAR *        s,
  unsigned long int *l)

/* DESCRIPTION  :   This function decodes a packed ASN1-string to an  */
/*                  unpacked ASN1-String (in memory)                  */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*        z : points to the TYPE field of an ASN-1 octet string.      */
/*       zm : points to the end of the buffer.                        */
/*        s : points to the target character string.                  */
/*        l : dummy.                                                  */
/*								      */
/* OUTPUT-PARAMETERS :                                                */
/*        l : will contain the number of bytes in the string <s>.     */
/*								      */
/* RETURN VALUES :    0 : if error                                    */
/*                   !0 : pointer to the next byte in <z>             */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : This function uses the MAVROS decoding routine           */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
  char FAR *z2;
  unsigned long int lz = 0, m;
/* **************** S T A R T   O F   F U N C T I O N *************** */
	*l = 0;
	m = (z < zm)?(*z&32):0;
	if ((z = rts_asn1_skiptype(z,zm)) == 0)
		return(0);
	if ((z = rts_asn1_length(z,zm,&z2))==0)
		return(0);
	if (m){
		if (z2)	zm = z2;
		while (z < zm && *z){
			if (*z != 4 && *z != 36)
			    return(0);
			if ((z = ro0049_OctMove(z,zm,s + *l,&lz)) == 0)
				return(0);
			*l += lz;
		}
		return(rts_asn1_close(z,zm,z2));
	}else{
		if (!z2)
		    return(0);
		ro0041_memcpy(s,z,*l = z2-z);
		return(z2);
	}
}
/* **************** E N D   O F   F U N C T I O N ******************* */


/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : ro0060_getCurrentTaskId                             */
/*								      */
/* AUTHOR       : Shivkumar, Haran                                    */
/* DATE         : 8/11/93                                             */
/*								      */
/* SYNTAX	:						      */

   unsigned long int FAR PASCAL ro0060_getCurrentTaskId (void)

/* DESCRIPTION  :                                                     */
/*                  ** skh: K03: A0264044: Busy Flag: FILE.D          */
/*                                                                    */
/*                  This function gets the ID of the task that has    */
/*                  currently invoked the ROS DLL.                    */
/*                  Change done for FILE.D to enable multi-tasking at */
/*                  the FILE.D level. In this way, FILE.D can have a  */
/*                  DLL above the ROS DLL and can support many tasks  */
/*                  calling this single DLL.                          */
/*								      */
/* INPUT-PARAMETERS :                                                 */
/*								      */
/* OUTPUT-PARAMETERS :                                                */
/*								      */
/* RETURN VALUES :  != 0    : The current task ID.                    */
/*                  == 0    : Error.                                  */
/*								      */
/* GLOBAL DATA	 ( read	only ):	 none				      */
/*								      */
/* GLOBAL DATA	 ( modified ) :	 none				      */
/*								      */
/* REMARKS : This function uses the MAVROS decoding routine           */
/*								      */
/*inoff	***************************************************************/

/* ****************** l	o c a l	   v a r i a b l e s **************** */
{/*entr*/
return (getpid());
}/*end*/

/* **************** E N D   O F   F U N C T I O N ******************* */

/* ****************************************************************** */
/* ********************   End of routil9.c  ************************* */
/* ****************************************************************** */
