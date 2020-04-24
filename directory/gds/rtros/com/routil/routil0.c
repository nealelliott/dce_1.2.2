/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:08  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:25  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:25  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:34:35  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:49  keutel
 * 	OT 11620
 * 	[1994/08/19  13:48:24  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:57  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:57  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:46  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:55:07  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:31  keutel
 * 	creation
 * 	[1994/03/21  13:24:52  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routil0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:08 $" } ;
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

/*daton ********************************************************************/
/*                                                                         */
/* TYPE         : MODULE                                                   */
/*                                                                         */
/* NAME         : routil0.c                                                */
/*                                                                         */
/* AUTHOR       : Roland Widmann, Michael Rippstain                        */
/* DATE         : 91-11-05                                                 */
/*                                                                         */
/* COMPONENT    : ROS and RTS utility functions                            */
/*                                                                         */
/* PRD#/VERS.   : ROS-V2.50, RTS-V2.0                                      */
/*                                                                         */
/* DESCRIPTION  : This modul provides functions which                      */
/*                                                                         */
/*   - copy and compare:                                                   */
/*             1. object identifier values ( ro0000_objcpy()               */
/*                                           ro0001_objcmp() )             */
/*                        (length as function argument)                    */
/*                                                                         */
/*             2. object identifier        ( ro0002_objcpy()               */
/*                                           ro0003_objcmp() )             */
/*                        (without length as function argument)            */
/*             3. Application Entity Invocation                            */
/*                                                                         */
/*             4. Application Entity Title                                 */
/*                                                                         */
/*             5. Two Applcation Context IDs                               */
/*                                                                         */
/*                                                                         */
/* SYSTEM DEPENDENCE :  none                                               */
/*                                                                         */
/* HISTORY      :                                                          */
/*                                                                         */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM#      */
/* 2.50 K0  | 91-04-01|  original                      |wdm |              */
/* 2.0  K0  | 91-10-25|  for the RTS design new fct    | mr |              */
/*          |         |   ro0002_objcpy() and          |    |              */
/*          |         |   ro0003_objcmp()              |    |              */
/*          | 91-12-04|  type for argument pointer from| mr |              */
/*          |         |   (unsigned short int) to      |    |              */
/*          |         |   (unsigned long int)          |    |              */
/*          | 92-04-21|  function for Apl. Entity added| mr |              */
/*          |         |                                |    |              */
/*          |         |                                |    |              */
/*          |         |                                |    |              */
/*          |         |                                |    |              */
/*                                                                         */
/*datoff *******************************************************************/

/***************************************************************************/
/*                                                                         */
/*              Module Identification                                      */
/*                                                                         */
/***************************************************************************/

static char *sccsid =    "@(#) routil0.c 1.1 93/11/29   RTS-V2.00";

/***************************************************************************/
/*                                                                         */
/*              I N C L U D E S                                            */
/*                                                                         */
/***************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>      /*                                               */
#include <roerval.h>      /*                                               */
#include <rouply0.h>      /* Include of ros  values                        */

#include <routil0.h>      /* Include of ROS common utilites (extern)       */
#include <routil1.h>      /* Include of ROS common utilites (extern)       */

#include <rodpat0.h>      /* For Rdb Data structures                       */
#include <rouctr4.h>      /* For the Data structures                       */
/* #include <roacpm1.h>       For the ROACPM Data structures                */

/***************************************************************************/
/*                                                                         */
/*              D E F I N E S                                              */
/*                                                                         */
/***************************************************************************/

RM0001_STRG (rs0000, "ro0000_objcpy           ")
RM0001_STRG (rs0001, "ro0001_objcmp           ")
RM0001_STRG (rs0002, "ro0002_objcpy           ")
RM0001_STRG (rs0003, "ro0003_objcmp           ")

RM0001_STRG (rs0004, "ro0004_cmpApplInvocation")
RM0001_STRG (rs0005, "ro0005_cpyApplInvocation")

RM0001_STRG (rs0006, "ro0006_cmpAETitle       ")
RM0001_STRG (rs0007, "ro0007_cpyAETitle       ")
RM0001_STRG (rs0009, "ro0009_deleteAETitle    ")
RM0001_STRG (rs000a, "ro000a_lengthAETitle    ")

RM0001_STRG (rs000b, "ro000b_cmpSetOfAVA      ")

RM0001_STRG (rs000c, "ro000c_AplCxtCmp        ")

RM0001_STRG (rs000d, "ro000d_MoveSend         ")
RM0001_STRG (rs000e, "ro000e_MoveReceive      ")
RM0001_STRG (rs000f, "ro000f_ComparePort      ")

signed short int PASCAL ro00fl_fgets (
    char    FAR *    pStr,
    signed int       Num,
    unsigned long int fp);

signed short int PASCAL ro000f_ComparePort ( signed short int sPort1);

/***************************************************************************/
/*                                                                         */
/*              L O C A L  F U N C T I O N  P R O T O T Y P E              */
/*                                                                         */
/***************************************************************************/

static signed short int PASCAL
ro000b_cmpSetOfAVA PROTO ((signed long  int   ,        /* lAVA1   */
			   rTro68_stAVA    [] ,        /* vAVA1[] */
			   signed long  int   ,        /* lAVA2   */
			   rTro68_stAVA    []  ));     /* vAVA2[] */

/* Return value = 0  : if ok                                               */
/*              < 0  : if an error occurs                                  */
/*              > 0  : if different AVAs, the number descibes what         */
/*                     difference:                                         */
/*                1  : different number of SET OFs (lAVA)                  */
/*                1x : the x indicates the AVA1, which isn't found         */
/* Parameter 1       : number of SET OF elements from AVA1                 */
/* Parameter 2       : vectores of AVA1                                    */
/* Parameter 3       : number of SET OF elements from AVA2                 */
/* Parameter 4       : vectores of AVA2                                    */

/***************************************************************************/
/*                                                                         */
/*                L O C A L   F U N C T I O N                              */
/*                                                                         */
/***************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro000b_cmpSetOfAVA                                       */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-06-22                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

static signed short int PASCAL ro000b_cmpSetOfAVA (
                     signed long  int lAVA1  ,
		     rTro68_stAVA     vAVA1[],
		     signed long  int lAVA2  ,
		     rTro68_stAVA     vAVA2[] )

/*                                                                         */
/* DESCRIPTION  :   This function compares a SET OF Attribute Value        */
/*                  Assertions (AVA).                                      */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : number of SET OF elements from AVA1                 */
/* Parameter 2       : vectores of AVA1                                    */
/* Parameter 3       : number of SET OF elements from AVA2                 */
/* Parameter 4       : vectores of AVA2                                    */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*              = 0  : if ok                                               */
/*              < 0  : if an error occurs                                  */
/*              > 0  : if different AVAs, the number descibes what         */
/*                     difference:                                         */
/*                1  : different number of SET OFs (lAVA)                  */
/*                1x : the x indicates the AVA1, which isn't found         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int k, l, sAVA1Found, rcc, rc;
signed short int vFoundAVA2[RCro7g_MaxRDN];  /* Found array for founded AVA1
						in SET OF AVA2 */
/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs000b)

rc = 0;
ro0043_memset((char*) vFoundAVA2, (int) -1,
	      (unsigned long int) sizeof (signed short int)*RCro7g_MaxRDN);

if ( lAVA1 != lAVA2) {
   rc = 1;       /* number of AVAs are different, error */

} else {

   /* set AVA1 */
   for ( k=0; k<lAVA1; k++) {

       l        = 0;    /* set first AVA2 */
       sAVA1Found = FALSE;

       /* cmp AVA1 with AVA2 */
       while (( sAVA1Found==FALSE) && ( l<lAVA2)) {

	 rcc = 0;

	 /* check if this AVA was still found */
	 if (vFoundAVA2[l] == -1) {

		 /* cmp object identifier */
		 if ( ro0003_objcmp (vAVA1[k].vAVA_Type,
				     vAVA2[l].vAVA_Type ))
		     rcc = 1;
		 /* cmp number of vAVA_Value[] elements */
	    else if ( vAVA1[k].lAVA_Value != vAVA2[l].lAVA_Value)
		     rcc = 2;
		 /* cmp vAVA_Value[] */
	    else if ( ro0042_memcmp (vAVA1[k].vAVA_Value,
				     vAVA2[l].vAVA_Value,
				     (unsigned long int) vAVA1[k].lAVA_Value ))
		     rcc = 3;


	    if ( rcc==0) {  /* AVAs are equal */

		vFoundAVA2[l] = k;     /* set the Found array  */
		sAVA1Found    = TRUE;  /* set the Found note, next AVA1  */
	    }
	 } /* (vFoundAVA2[l] == -1) { */

	 l++;    /* set new AVA2 */

       } /* end of while (( sAVA1Found==FALSE) && ( l<lAVA2)) { */

       /* check if a AVA isn't founded */
       if ( sAVA1Found==FALSE) {
	   rc = 10+k;                /* AVA1[k] isn't found       */
	   break;                    /* return, AVAs are different */
       }

   } /* end of for ( k=0; k<lAVA1; k++) { */
} /* end of else */

RM0003_RETURN (rs000b, signed short int, rc)
}/*end*/

/***************************************************************************/
/*                                                                         */
/*              G L O B A L   V A R I A B L E S                            */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/*              G L O B A L   F U N C T I O N S                            */
/*                                                                         */
/***************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0000_objcpy                                            */
/*                                                                         */
/* AUTHOR       : Roland Widmann                                           */
/* DATE         : 90-10-29                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int   PASCAL ro0000_objcpy (
	unsigned long int FAR  *target,
	unsigned long int FAR  *source,
	unsigned long int       length)
/*                                                                         */
/* DESCRIPTION  :   This function copies an object identifier value        */
/*                  and checks whether it is longer than allowed.          */
/*                  It returns SUCES if no error ocuurs, a value           */
/*                  less than SUCES if an error occurs or a value          */
/*                  grather  than SUCES if the length of the               */
/*                  source object identifier value is equal to NULL.       */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/*      target  : object identifier target array.                          */
/*                                                                         */
/*      source  : object identifier source array.                          */
/*                                                                         */
/*      length  : length of the source object identifier value             */
/*                                                                         */
/* OUT-PARAMETERS             : none                                       */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/*      = 0  SUCES     successfully                                        */
/*      < 0 (SUCES)    error                                               */
/*      > 0 (SUCES)    not defined (length field equal to NULL)            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0000)

if (length ==  0) {
    *target     =  0;
    RM0003_RETURN (rs0000, signed short int, R_LENULL)
} else {
    if ((length > RCro00_MaxObjIdComp) || (length < 3 ))
	 RM0003_RETURN (rs0000, signed short int, -R_MAXOBJ)
    else {
	 *target++   = length;
	 while (length-- >  0)  *target++ = *source++;
    }
RM0003_RETURN (rs0000, signed short int, 0)
}
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0001_objcmp                                            */
/*                                                                         */
/* AUTHOR       : Roland Widmann                                           */
/* DATE         : 90-10-29                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int  PASCAL ro0001_objcmp (
    unsigned long int       len1,
    unsigned long int FAR  *value1,
    unsigned long int       len2,
    unsigned long int FAR  *value2)

/*                                                                         */
/* DESCRIPTION  :   This function compares to object identifier            */
/*                  values. If euqual it returns TRUE. In the other        */
/*                  case it returns FALSE.                                 */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/*      len1    : Length of the first object identifier                    */
/*                                                                         */
/*      value1  : Pointer to the first component of the first object       */
/*                identifier.                                              */
/*                                                                         */
/*      len2    : Length of the second object identifier                   */
/*                                                                         */
/*      value2  : Pointer to the first component of the second object      */
/*                identifier.                                              */
/*                                                                         */
/* OUT-PARAMETERS             : none                                       */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/*      =  0    if equal                                                   */
/*      <> 0    if unequal                                                 */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0001)

if (len1 != len2)    RM0003_RETURN (rs0001, signed short int, R_DIFOBJ)
while (len1-- > 0) if (*value1++ != *value2++)
		     RM0003_RETURN (rs0001, signed short int, R_DIFOBJ)
RM0003_RETURN (rs0001, signed short int, 0)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0002_objcpy                                            */
/*                                                                         */
/* AUTHOR       : Roland Widmann,  Michael Rippstain                       */
/* DATE         : 90-10-29                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int   PASCAL ro0002_objcpy (
	unsigned long int FAR  *target,
	unsigned long int FAR  *source)

/*                                                                         */
/* DESCRIPTION  :   This function copies an object identifier              */
/*                  and checks whether it is longer than allowed.          */
/*                  It returns SUCES if no error ocuurs, a value           */
/*                  less than SUCES if an error occurs or a value          */
/*                  grather  than SUCES if the length of the               */
/*                  source object identifier value is equal to NULL.       */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/*      target  : object identifier target array.                          */
/*                                                                         */
/*      source  : object identifier source array.                          */
/*                                                                         */
/* OUT-PARAMETERS             :  target array is filled if ok              */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/*      = 0  SUCES     successfully                                        */
/*      < 0            error                                               */
/*      > 0            not defined (length field equal to NULL)            */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

unsigned long int length;      /* length of the source                     */
			       /* object identifier value                  */

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0002)
/* kc on 31-8-93 */                  
length = (source)? *source++ : NULL ;


if (length == 0) { /* no obj. id. is defined  ->error */
    *target     =  0;    RM0003_RETURN (rs0002, signed short int, R_LENULL)
} else {
    if ((length > RCro00_MaxObjIdComp) || (length < 3 )) {
	/* too short or too long obj. id. */
		       RM0003_RETURN (rs0002, signed short int, -R_MAXOBJ)
    } else {
	*target++   = length;
	while (length-- >  0)  *target++ = *source++;
    }
    RM0003_RETURN (rs0002, signed short int, 0)
}
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0003_objcmp                                            */
/*                                                                         */
/* AUTHOR       : Roland Widmann, Michael Rippstain                        */
/* DATE         : 91-10-25                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int  PASCAL ro0003_objcmp ( 
    unsigned long int FAR  *value1,
    unsigned long int FAR  *value2)

/*                                                                         */
/* DESCRIPTION  :   This function compares to object identifier.           */
/*                  If equal it returns TRUE. In the other                 */
/*                  case it returns FALSE.                                 */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/*      value1  : Pointer to the first component of the first object       */
/*                identifier.                                              */
/*                                                                         */
/*      value2  : Pointer to the first component of the second object      */
/*                identifier.                                              */
/*                                                                         */
/* OUT-PARAMETERS             : none                                       */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/*      =  0    if equal                                                   */
/*      <> 0    if unequal                                                 */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

 unsigned long int    len1;          /* Length of the first obj. id.       */
 unsigned long int    len2;          /* Length of the second obj. id.      */

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0003)

/* if the pointers are NULL some m/c (HP) will cause core if U refer that */            
     
if(value1) len1 = *value1++;
else len1 = 0;
if(value2) len2 = *value2++;
else len2 = 0;

if (len1 != len2)        RM0003_RETURN (rs0003, signed short int, -R_DIFOBJ)

if ((len1 == 0 ) && (len2 == 0 ))
   RM0003_RETURN (rs0003, signed short int, 0)
    
while (len1-- > 0) if (*value1++ != *value2++)
			 RM0003_RETURN (rs0003, signed short int, -R_DIFOBJ)

RM0003_RETURN (rs0003, signed short int, 0)
}/*end*/

/* *************************************************************************/
/* ********** Application Entity Invocation processing facility ********** */
/* *************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0004_cmpApplInvocation                                 */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-04-21                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int PASCAL ro0004_cmpApplInvocation ( 
                           rTro74_stInvoc FAR * pstInvoc1,
			   rTro74_stInvoc FAR * pstInvoc2 )

/*                                                                         */
/* DESCRIPTION  :   This function compares Application Entity Invocation   */
/*                  If equal it returns TRUE. In the other                 */
/*                  case it returns FALSE.                                 */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : pointer to AE-Invoc-Id1 (see rouply0.h)             */
/* Parameter 2       : pointer to AE-Invoc-Id2 (see rouply0.h)             */
/*                                                                         */
/* OUT-PARAMETERS             : none                                       */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/*      =  0    if equal                                                   */
/*      =  1    if unequal because of ApInvocation                         */
/*      =  2    if unequal because of AeInvocation                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int rc;
signed short int bAp;            /* if TRUE the AP-Invoc-Ids are equal     */
signed short int bAe;            /* if TRUE the AE-Invoc-Ids are equal     */

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0004)
rc  = 0;
bAp = FALSE;
bAe = FALSE;
bAp = ( !pstInvoc1->bApInvocId  && !pstInvoc2->bApInvocId) ||
       ( pstInvoc1->bApInvocId && pstInvoc2->bApInvocId &&
	 pstInvoc1->lApInvocId && pstInvoc2->lApInvocId   );
bAe = ( !pstInvoc1->bAeInvocId  && !pstInvoc2->bAeInvocId) ||
       ( pstInvoc1->bAeInvocId && pstInvoc2->bAeInvocId &&
	 pstInvoc1->lAeInvocId && pstInvoc2->lAeInvocId   );

if (! bAp)   rc = 1;
if (! bAe)   rc = 2;
	   /* equal is ZERO for return code */
RM0003_RETURN (rs0004, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0005_cpyApplInvocation                                 */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-04-21                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

void PASCAL
ro0005_cpyApplInvocation ( rTro74_stInvoc FAR * pstTargetInvoc,
			   rTro74_stInvoc FAR * pstSourceInvoc )

/*                                                                         */
/* DESCRIPTION  :   This function copies an Application Entity Invocation  */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : pointer to AE-Invoc-Id target (see rouply0.h)       */
/* Parameter 2       : pointer to AE-Invoc-Id source (see rouply0.h)       */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                               none                                      */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0005)

pstTargetInvoc->bApInvocId = pstSourceInvoc->bApInvocId;
pstTargetInvoc->lApInvocId = pstSourceInvoc->lApInvocId;
pstTargetInvoc->bAeInvocId = pstSourceInvoc->bAeInvocId;
pstTargetInvoc->lAeInvocId = pstSourceInvoc->lAeInvocId;

RM0004_RETURN_VOID (rs0005)
}/*end*/

/* *************************************************************************/
/* ********** Application Entity Title processing facility *************** */
/* *************************************************************************/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0006_cmpAETitle                                        */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-04-21                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int PASCAL
ro0006_cmpAETitle ( unsigned long int hAeTitle1,
		    unsigned long int hAeTitle2 )
/*                                                                         */
/* DESCRIPTION  :   This function compares Application Entity Titles       */
/*                  If equal it returns TRUE. In the other                 */
/*                  case it returns FALSE.                                 */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : Handle of AE-Title1 (see rouply0.h)                 */
/* Parameter 2       : Handle of AE-Title2 (see rouply0.h)                 */
/*                                                                         */
/* OUT-PARAMETERS             : none                                       */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*                                                                         */
/*              = 0  : if equal                                            */
/*                1  : if not equal, not further referenced                */
/*                2  : if not equal because of ApTitle                     */
/*                3  : if not equal because of AeQualifier                 */
/*                                                                         */
/*              < 0  : if an error occurs                                  */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int i,rc;
rTro72_unAeTitle *pAeTitle1;
rTro72_unAeTitle *pAeTitle2;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0006)
rc = 0;

if ( (! hAeTitle1) && (! hAeTitle2))  /* no hAeTitle for both, equal */
		   RM0003_RETURN (rs0006, signed short int, 0)

if (! ( hAeTitle1 && hAeTitle2)) {
    rc = 1; /* only one hAeTitle, different */

} else {    /* hAeTitle for both, the content decides */

    /* -------- lock the AeTitle ----------------------------------------- */
    pAeTitle1    = (rTro72_unAeTitle NEAR *) ro0031_locallock (hAeTitle1);
    if (! pAeTitle1)     RM0003_RETURN (rs0006, signed short int, -R_LLOCKE)

    if ((pAeTitle1->lAeTitleForm & 0xff00 )) { /* No checking */
     if (ro0032_localunlock (hAeTitle1)) rc = -R_LULOKE;
         RM0003_RETURN (rs0006, signed short int, rc)
    }    
    pAeTitle2    = (rTro72_unAeTitle NEAR *) ro0031_locallock (hAeTitle2);
    if (! pAeTitle2 ) {  ro0032_localunlock (hAeTitle1);
			 RM0003_RETURN (rs0006, signed short int, -R_LLOCKE)
    }
    /* -------- compare the AeTitle1 and AeTitle2 ------------------------ */
    if ( pAeTitle1->lAeTitleForm == RCro77_ObjId) {

	/* --------------- RCro77_ObjId ---------------------------------- */
	     if (pAeTitle1->stAeTitleForm2.lAeTitleForm !=
		 pAeTitle2->stAeTitleForm2.lAeTitleForm   )      rc = 1;
	/* cmp ApTitle */
	else if (ro0003_objcmp
		     (pAeTitle1->stAeTitleForm2.oApTitleObjId,
		      pAeTitle2->stAeTitleForm2.oApTitleObjId )) rc = 2;
	/* cmp AeQualifier */
	else if ( pAeTitle1->stAeTitleForm2.bAeQualInt !=
		  pAeTitle2->stAeTitleForm2.bAeQualInt   )       rc = 3;
	else if ( pAeTitle1->stAeTitleForm2.bAeQualInt &&
		   (pAeTitle1->stAeTitleForm2.lAeQualInt !=
		    pAeTitle2->stAeTitleForm2.lAeQualInt   ))    rc = 3;
    } else {
	/* --------------- RCro78_RelDisName ----------------------------- */

	     if (pAeTitle1->stAeTitleForm1.lAeTitleForm !=
		 pAeTitle2->stAeTitleForm1.lAeTitleForm   )      rc = 1;

	/* cmp ApTitle */
	else if (pAeTitle1->stAeTitleForm1.lApTitle  !=
		 pAeTitle2->stAeTitleForm1.lApTitle     )        rc = 2;

	if (! rc && ( pAeTitle1->stAeTitleForm1.lApTitle)) {
	  for ( i=0; i<pAeTitle1->stAeTitleForm1.lApTitle; i++) {
	     rc = ro000b_cmpSetOfAVA (
				pAeTitle1->stAeTitleForm1.vApTitle[i].lRDN,
				pAeTitle1->stAeTitleForm1.vApTitle[i].vRDN,
				pAeTitle2->stAeTitleForm1.vApTitle[i].lRDN,
				pAeTitle2->stAeTitleForm1.vApTitle[i].vRDN );

	     if ( rc) {  /* rc consists the localisation of difference
			       see routil0.c for further explanation */
								 rc = 2;
		 break;
	     }
	  }
	}

	/* cmp AeQualifier */
	if (! rc) {
	   rc = ro000b_cmpSetOfAVA ( pAeTitle1->stAeTitleForm1.lAeQuali,
				     pAeTitle1->stAeTitleForm1.vAeQuali,
				     pAeTitle2->stAeTitleForm1.lAeQuali,
				     pAeTitle2->stAeTitleForm1.vAeQuali );

	   if ( rc) {    /* rc consists the localisation of difference
			       see routil0.c for further explanation */
								 rc = 3;
	   }

	}
    } /* end of if ( pAeTitle1->lAeTitleForm == RCro77_ObjId) { */

    /* --------- unlock the AeTitle -------------------------------------- */
    if (ro0032_localunlock (hAeTitle1)) rc = -R_LULOKE;
    if (ro0032_localunlock (hAeTitle2)) rc = -R_LULOKE;

} /* end of if (! ( hAeTitle1 && hAeTitle2)) { */

RM0003_RETURN (rs0006, signed short int, rc)

}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0007_cpyAETitle                                        */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-04-21                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int PASCAL
ro0007_cpyAETitle ( unsigned long int FAR * phTargetAeTitle,
		    unsigned long int        hSourceAeTitle )

/*                                                                         */
/* DESCRIPTION  :   This function copies an Application Entity Title.      */
/*                  It creates a new AE-Title ( alloc, lock etc).          */
/*                                                                         */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : pointer to unsigned long int                        */
/*                     (see rouply0.h)                                     */
/* Parameter 2       : Handle of AE-Title source                           */
/*                     (see rouply0.h)                                     */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* Parameter 1       : pointer to Handle of AE-Title target                */
/*                     (see rouply0.h)                                     */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*              = 0  : if ok                                               */
/*             != 0  : if an error occurs                                  */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int   rc;
unsigned long int  ulLength;
unsigned long int  hTarget;
rTro72_unAeTitle  *pSource;
rTro72_unAeTitle  *pTarget;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0007)
rc       = 0;
ulLength = 0;

if (! hSourceAeTitle) {  /* no AeTitle */
		   RM0003_RETURN (rs0007, signed short int, 0)
} else {                 /* AeTitle */
    pSource = (rTro72_unAeTitle NEAR *) ro0031_locallock (hSourceAeTitle);
    if (! pSource) RM0003_RETURN (rs0007, signed short int, -R_LLOCKE )

    /* ------------------- RCro77_ObjId ---------------------------------- */
    if ( pSource->lAeTitleForm == RCro77_ObjId)
	  ulLength = sizeof ( rTro71_stAeTitleForm2);

    /* ------------------- RCro78_RelDisName ----------------------------- */
    else
	  ulLength = sizeof ( rTro70_stAeTitleForm1);

    /* ------- alloc and lock the target --------------------------------- */
    hTarget = ro0030_localalloc ( ulLength);
    pTarget = (hTarget) ? (rTro72_unAeTitle NEAR*) ro0031_locallock (hTarget)
			: 0;
    rc      = (pTarget) ? 0 : -R_LMEMRY;
    if (rc) { ro0034_localfree (hTarget);
	      ro0032_localunlock (hSourceAeTitle);
	      RM0003_RETURN (rs0007, signed short int, rc)
    }
    /* -------------------- copy ----------------------------------------- */
    ro0041_memcpy ( (char FAR*) pTarget, (char FAR*) pSource, ulLength);

    /* ------- unlock of source and target ------------------------------- */
    if ( ro0032_localunlock ( hSourceAeTitle) )
	      RM0003_RETURN (rs0007, signed short int, -R_LULOKE)
    if ( ro0032_localunlock ( hTarget)        )
	      RM0003_RETURN (rs0007, signed short int, -R_LULOKE)

    *phTargetAeTitle = hTarget;
}
RM0003_RETURN (rs0007, signed short int, 0)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0009_deleteAETitle                                     */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-04-21                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int PASCAL
ro0009_deleteAETitle ( unsigned long int FAR * phAeTitle )

/*                                                                         */
/* DESCRIPTION  :   This function deletes an Application Entity Title.     */
/*                  It frees the dynamic memory.                           */
/*                                                                         */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter         : pointer to Handle of AE-Title (see rouply0.h)       */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* Parameter         : pointer to ZERO-Handle of AE-Title (see rouply0.h)  */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*              = 0  : if ok                                               */
/*             != 0  : if an error occurs                                  */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int  rc;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs0009)
rc = 0;

if ( phAeTitle && *phAeTitle) {  /* AeTitle */
    if(ro0034_localfree ( *phAeTitle)) rc = -R_LFREEE;
     if (! rc)  *phAeTitle = 0;    /* set the handle to ZERO if ok */
}
RM0003_RETURN (rs0009, signed short int, rc)
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro000a_lengthAETitle                                     */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-04-21                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int PASCAL
ro000a_lengthAETitle ( unsigned long int       hAeTitle,
		       unsigned long int FAR * pulLength )

/*                                                                         */
/* DESCRIPTION  :   This function computes the Length of an                */
/*                  Application Entity Title or nothing if the handle      */
/*                  is ZERO.                                               */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : Handle of AE-Title (see rouply0.h)                  */
/*                                                                         */
/* OUT-PARAMETERS             :                                            */
/*                                                                         */
/* Parameter 2       : pointer to length of AE-Title (see rouply0.h)       */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*              = 0  : if ok                                               */
/*             != 0  : if an error occurs                                  */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

rTro72_unAeTitle  * pAeTitle;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs000a)

if (! hAeTitle) {  /* no AeTitle */
		       RM0003_RETURN (rs000a, signed short int, 0)
} else {                 /* AeTitle */
    pAeTitle = (rTro72_unAeTitle NEAR *) ro0031_locallock (hAeTitle);
    if (! pAeTitle)    RM0003_RETURN (rs000a, signed short int, -R_LLOCKE )

    /* ------------------- RCro77_ObjId ---------------------------------- */
    if ( pAeTitle->lAeTitleForm == RCro77_ObjId) {
	  *pulLength = sizeof ( rTro71_stAeTitleForm2);

    /* ------------------- RCro78_RelDisName ----------------------------- */
    } else {
	  *pulLength = sizeof ( rTro70_stAeTitleForm1);
    }
    if ( ro0032_localunlock ( hAeTitle) ) {
			RM0003_RETURN (rs000a, signed short int, -R_LULOKE)
    }
}
RM0003_RETURN (rs000a, signed short int, 0)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro000c_AplCxtCmp				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int FAR PASCAL ro000c_AplCxtCmp(
           signed    short int                sIndiId,
           signed    short int                sRspId,
           unsigned  long  int                hHd,
           signed    short int      FAR     * pbRes)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
signed   short  int   rc=OK;
signed   short  int   i,j;

rT0501_AEInvocation  NEAR *pApl;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs000c)

pApl = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hHd);               
if (! pApl)                                              rc = -R_LLOCKE;    
else {
       if ( (sIndiId < 0                      )||                            
            (sIndiId > RC0570_MaxApplCtxtInfos)||                            
            (sIndiId > pApl->sNumbOfApplCtxts )  )        rc = -R_AAPLID;    
 
  else if ( (sRspId < 0                      )||                            
            (sRspId > RC0570_MaxApplCtxtInfos)||                            
            (sRspId > pApl->sNumbOfApplCtxts )  )        rc = -R_AAPLID;    
       if (rc != OK )
               RM0003_RETURN (rs000c, signed short int, rc)                                   
       *pbRes = TRUE; 
      /* ----- Check AseCif ------------- */
      if ( pApl->vApplCtxtInfos[sRspId].usAseConfig !=
           pApl->vApplCtxtInfos[sIndiId].usAseConfig  )  *pbRes = FALSE;

 else if ( pApl->vApplCtxtInfos[sRspId].lModeSelector !=                     
           pApl->vApplCtxtInfos[sIndiId].lModeSelector  )  *pbRes = FALSE;    

 else if ( pApl->vApplCtxtInfos[sRspId].vAbstrSynId[0]  >
           pApl->vApplCtxtInfos[sIndiId].vAbstrSynId[0]  )  *pbRes = FALSE;        
 else 
      for ( i = 1; 
            i <= pApl->vApplCtxtInfos[sRspId].vAbstrSynId[0]; 
            i++)  
      {
          for ( j = 1;
                j <= pApl->vApplCtxtInfos[sIndiId].vAbstrSynId[0];
                j++ )
          {
              if ( pApl->vApplCtxtInfos[sRspId].vAbstrSynId[i] ==
                   pApl->vApplCtxtInfos[sIndiId].vAbstrSynId[j]  ) break;
          }/*end of 2nd for loop */

           if ( j > pApl->vApplCtxtInfos[sIndiId].vAbstrSynId[0] ){
              *pbRes = FALSE;
               break;
           }
       } /*end of 1st for loop */
       if ( ro0032_localunlock ( hHd) )  rc = -R_LULOKE;
 }/* end of else */      
            
RM0003_RETURN (rs000c, signed short int, rc)                                    
}/*end*/                                                                        


/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* AUTHOR       : Vinita                                                   */
/*                                                                         */
/* DATE         : 93-04-06                                                 */
/*                                                                         */
/* SYNTAX       : ro000d_MoveSend                                          */

signed short int PASCAL ro000d_MoveSend (
	unsigned long int   FAR *vUserInfo,
	unsigned long int   FAR *pulUserInfo)

/*                                                                         */
/* DESCRIPTION  :   This function would copy handles for sequence  of      */
/*                  externals to pci of ACPM from p1 received from ext     */
/*                  interface. Support for sequence of external has been   */
/*                  added in RTROS V3.00. this  function would be called   */
/*                  called from  ACPM transaction functions .              */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : UserInfo vector                                     */
/* Parameter 2       : pointer to UserInfo containing no.  of externals    */
/*                                                                         */
/* OUT-PARAMETERS             :   NONE                                     */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*              = 0  : if ok                                               */
/*             != 0  : if an error occurs                                  */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

rTro04_stUserData     FAR   *pP1;
signed  short  int           rc=0;
signed  long   int           count=0;
signed  long   int           i=0;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs000d)


   pP1 = (rTro04_stUserData FAR  *) ro0021_globallock (vUserInfo[0]);
   if (!pP1)                                                      
   RM0003_RETURN (rs000d, signed short int, -R_GMEMRY)

   count = *pulUserInfo = pP1->stPdvHeader.lFraCount ;
   for (i=1; i<count; i++) /* x-x-x 10-11 <= count was previous -1 in*/
   {
     vUserInfo[i] = pP1->stPdvHeader.vPdvHandle[i-1] ;
   }

   if (ro0022_globalunlock (vUserInfo[0])) rc = -R_GULOKE;

RM0003_RETURN (rs000d, signed short int, rc)
}/*end*/


/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* AUTHOR       : Vinita                                                   */
/*                                                                         */
/* DATE         : 93-04-06                                                 */
/*                                                                         */
/* SYNTAX       : ro000e_MoveReceive                                       */

signed short int PASCAL ro000e_MoveReceive ( 
	unsigned long int   FAR *vUserInfo,
	unsigned long int   FAR *pulUserInfo)

/*                                                                         */
/* DESCRIPTION  :   This function would copy handles for sequence  of      */
/*                  externals to P1 from pci of ACPM .                     */
/*                  Support for sequence of external has been              */
/*                  added in RTROS V3.00. this  function would be called   */
/*                  called from  ACPM transaction functions .              */
/*                                                                         */
/* IN-PARAMETERS              :                                            */
/*                                                                         */
/* Parameter 1       : UserInfo vector                                     */
/* Parameter 2       : pointer to UserInfo containing no.  of externals    */
/*                                                                         */
/* OUT-PARAMETERS             :   NONE                                     */
/*                                                                         */
/* RETURN VALUES :                                                         */
/*              = 0  : if ok                                               */
/*             != 0  : if an error occurs                                  */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

rTro04_stUserData     FAR   *pP1;
signed  short  int           rc=0;
signed  long   int           count=0;
signed  long   int           i=0;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY (rs000d)


   pP1 = (rTro04_stUserData FAR  *) ro0021_globallock (vUserInfo[0]);
   if (!pP1)                                                      
   RM0003_RETURN (rs000d, signed short int, -R_GMEMRY)

   count =  pP1->stPdvHeader.lFraCount = *pulUserInfo ;
/*
   pP1->stPdvHeader.lFraCount = (count-1) ; */

   /* PdvHeader Fragment count does not include the first handle in */
   /* the total count */

   for (i=1; i<count; i++)/* x-x-x kc 10-11 <= count previous */
   {
     pP1->stPdvHeader.vPdvHandle[i-1] = vUserInfo[i] ;
   }

   if (ro0022_globalunlock (vUserInfo[0])) rc = -R_GULOKE;

RM0003_RETURN (rs000e, signed short int, rc)
}/*end*/
/*exon *********************************************************************/  
/*                                                                         */  
/* TYPE         : C-FUNKTION                                               */  
/*                                                                         */  
/* AUTHOR       : Chandrasekar.K                                           */  
/*                                                                         */  
/* DATE         : 19-01-94                                                 */  
/*                                                                         */  
/* SYNTAX       : ro000f_ComparePort                                       */  
                                                                               
signed short int PASCAL ro000f_ComparePort ( signed short int sPort1)
/*                                                                         */  
/* DESCRIPTION  :   This function compares the Port number from the file   */  
/*                                                                         */  
/* IN-PARAMETERS              :                                            */  
/*                                                                         */  
/*                                                                         */  
/* OUT-PARAMETERS             :   NONE                                     */  
/*                                                                         */  
/* RETURN VALUES :                                                         */  
/*             == FALSE Not found                                          */  
/*             == TRUE  Found                                              */  
/*                                                                         */  
/* GLOBAL DATA   ( read only ):  none                                      */  
/*                                                                         */  
/* GLOBAL DATA   ( modified ) :  none                                      */  
/*                                                                         */  
/* REMARKS                    :  none                                      */  
/*                                                                         */  
/*inoff ********************************************************************/  
                                                                               
/* ****************** l o c a l    v a r i a b l e s ********************* */  
/* ******************                                ********************* */  
{/*entr*/                                                                      
                                                                               
signed short int sLocPort;                                                     
unsigned long int ulFile=NULL;                                                 
static        char Str[80];                                                    
/* ****************** S T A R T ****************************************** */  
/* ******************           ****************************************** */  
RM0002_ENTRY (rs000f)                                                          
                                                                               
ulFile = ro00f1_fopen("c:/etc/tpmflow","r");                             
if (ulFile == NULL) RM0003_RETURN (rs000f, signed short int, TRUE)       
while ( !ro00fl_fgets((char FAR *)Str,78,ulFile) ){                      
     sLocPort = (signed short int ) ro0054_atoi(Str);                    
     if (sLocPort == sPort1 ) {                                          
        ro00f2_fclose(ulFile);                                           
        RM0003_RETURN (rs000f, signed short int, FALSE)                  
     }                                                                   
}                                                                        
ro00f2_fclose(ulFile);                                                   
RM0003_RETURN (rs000f, signed short int, TRUE)                           
}/*end*/                                                                                                                                               

/* EOF */
