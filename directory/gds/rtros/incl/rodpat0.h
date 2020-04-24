/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:33  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:04  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:30  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:06  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:05  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:48  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:10  keutel
 * 	creation
 * 	[1994/03/21  13:26:17  keutel]
 * 
 * $EndLog$
 */
#ifndef _RODPAT0_H
#define _RODPAT0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rodpat0_rcsid[] = { "@(#) $RCSfile: rodpat0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:33 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1991 BY SIEMENS NIXDORF INFORMATIONS AG         */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : rodpat0.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 91-10-31                                            */
/*                                                                    */
/* COMPONENT    : RTS dispatcher                                      */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : Interface between dispatcher and protocol machines  */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#)rodpat0.h  1.1 93/11/29"                         */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 2.00 K0  | 91-10-30|  original                      |jm  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Max numbers of automat on one ***************** */
/* ****************** protocol stack                ***************** */

#define RC0200_MaxAutomata     12

/* ****************** Automata target numbers *********************** */
/* ******************                         *********************** */

#define RC0201_Timer           0
#define RC0202_Lower           1
#define RC0203_AutNo2          2
#define RC0204_AutNo3          3
#define RC0205_AutNo4          4
#define RC0206_AutNo5          5
#define RC0207_AutNo6          6
#define RC0208_AutNo7          7
#define RC0209_AutNo8          8
#define RC0210_AutNo9          9
#define RC0211_AutN10         10
#define RC0212_AutN11         11

/* ****************** Automata target numbers for OSI *************** */
/* ****************** protocol stacks (general)       *************** */

#define RC0213_Transport      RC0203_AutNo2
#define RC0214_Session        RC0204_AutNo3
#define RC0215_Presentation   RC0205_AutNo4
#define RC0216_Acse           RC0206_AutNo5
#define RC0217_LcrtFct        RC0207_AutNo6
#define RC0218_Rtse           RC0208_AutNo7
#define RC0219_Rose           RC0209_AutNo8

/* ****************** Automata target numbers for OSI     *********** */
/* ****************** protocol stacks (variable ASE user) *********** */

#define RC0220_RtseUser       RC0209_AutNo8
#define RC0221_RoseUser       RC0210_AutNo9

/* ****************** Get entity invocation context  **************** */
/* ****************** layer number ** skh **         **************** */

#define RC0230_TransportEiCxt      0
#define RC0231_SessionEiCxt        1
#define RC0232_PresentationEiCxt   2
#define RC0233_ApplicationEiCxt    3

/* ****************** Protocol stack bassis configuration *********** */
/* ****************** numbers.(On Ecb basis )             *********** */

#define RC0240_CfRtseOnly       0
#define RC0241_CfRoseOnly       1
#define RC0242_CfRoseOverRtse   2


/* ****************** Protocol stack bassis configuration *********** */
/* ****************** numbers.(On Assoc Basis )           *********** */
#define RC0243_SwRtseOnly       0
#define RC0244_SwRoseOnly       1
#define RC0245_SwRoseOverRtse   2




/* ****************** Pop incomming event *************************** */
/* ******************                     *************************** */

signed long int PASCAL ro0210_popInEvent PROTO ((unsigned long int FAR *,
                                                 unsigned long int FAR *,
                                                 unsigned long int FAR *,
                                                 unsigned long int FAR *));
/* Return value: pm event  */
/* Parameter 1 : p1        */
/* Parameter 2 : p2        */
/* Parameter 3 : p3        */
/* Parameter 4 : p4        */


/* ****************** Push outgoing event *************************** */
/* ******************                     *************************** */

signed short int PASCAL ro0211_pushOutEvent PROTO ((signed   short int,
                                                    signed   long  int,
                                                    unsigned long  int,
                                                    unsigned long  int,
                                                    unsigned long  int,
                                                    unsigned long  int));
/* Return value == 0 : successful */
/*              != 0 : error      */
/* Parameter 1       : pm number  */
/* Parameter 2       : pm event   */
/* Parameter 3       : p1         */
/* Parameter 4       : p2         */
/* Parameter 5       : p3         */
/* Parameter 6       : p4         */

/* ****************** Register ppm invocation and ******************* */
/* ****************** ppm destruction             ******************* */

signed short int PASCAL ro0212_regPmInvoc PROTO ((unsigned long int));

/* Return value == 0 : successful         */
/*              != 0 : error              */
/* Parameter 1       : pm context handle  */


void PASCAL ro0213_regPmDestr PROTO ((void));

/* ****************** Read association and application ************** */
/* ****************** handle (identifier)              ************** */


unsigned long int PASCAL ro0214_getAcbEcb PROTO ((unsigned long int FAR *));

/* Return value : association handle */
/* Parameter    : application handle */

/* ****************** Get entity invocation contexts **************** */
/* ******************                                **************** */

signed   short int PASCAL ro0215_getEiCxt PROTO ((signed   short int      ,
                                                  unsigned long  int FAR *));

/* return value == 0 : successful                                     */
/*               > 0 : No entity context defined for layer number     */
/*               < 0 : error                                          */
/* Parameter 1       : Layer number (one of the constants defined     */
/*                     above.                                         */
/* Parameter 3       : Returns handle of entity invocation context.   */

/* ****************** Get provided buffer *************************** */
/* ******************                     *************************** */

signed short int PASCAL ro0216_getPrBuffer PROTO ((unsigned long  int FAR *));

/* return value == 0 : successful               */
/*               > 0 : No buffer available.     */
/*               < 0 : error                    */
/* Parameter 1       : Returns provided buffer. */


/* ****************** Get address part ****************************** */
/* ******************                  ****************************** */

signed short int PASCAL ro0217_getAddress PROTO ((unsigned long  int  ,
                                                  signed   short int  ,
                                                  unsigned short int  ,
                                                  char           FAR *));

/* return value >= 0 : successful, real length of address part          */
/*              <  0 : error                                            */
/* Parameter 1       : If it is the NULL handle the local address is    */
/*                   : used.                                            */
/* Parameter 2       : Layer number (transport, session,                */
/*                                   presentation)                      */
/* Parameter 3       : Length of memory area (parameter 4)              */
/* Parameter 4       : Pointer to memory where the address part will be */
/*                     written                                          */

/* ****************** Concatenate address parts ********************* */
/* ******************                           ********************* */

signed short int PASCAL ro0218_conAddress PROTO ((unsigned long  int  FAR *,
                                                  signed   short int       ,
                                                  unsigned short int       ,
                                                  char                FAR *));

/* return value == 0 : successful,                                      */
/*              != 0 : error                                            */
/* Parameter 1       : Points to a memory handle. If it points to the   */
/*                   : NULL handle a new address is build.              */
/* Parameter 2       : Layer number (transport, session,                */
/*                                   presentation)                      */
/* Parameter 3       : Length of memory area (parameter 4)              */
/* Parameter 4       : Pointer to memory where the address part is      */
/*                     stored.                                          */

/* ****************** Push outgoging event; associatation *********** */
/* ****************** is explicitly specifed.             *********** */

signed short int PASCAL ro0219_pushOnAssoc PROTO ((unsigned long  int,
                                                   signed   short int,
                                                   signed   long  int,
                                                   unsigned long  int,
                                                   unsigned long  int,
                                                   unsigned long  int,
                                                   unsigned long  int));
/* return value == 0 : successful         */
/*              != 0 : error              */
/* Parameter 1       : association handle */
/* Parameter 2       : pm number          */
/* Parameter 3       : pm event           */
/* Parameter 4       : p1                 */
/* Parameter 5       : p2                 */
/* Parameter 6       : p3                 */
/* Parameter 6       : p4                 */

/* ****************** Push outgoging event and create a new ********* */
/* ****************** association.                          ********* */

signed short int PASCAL ro0220_pushNewAssoc PROTO ((unsigned long  int FAR *,
                                                    unsigned long  int,
                                                    signed   short int,
                                                    signed   long  int,
                                                    unsigned long  int,
                                                    unsigned long  int,
                                                    unsigned long  int,
                                                    unsigned long  int      ));
/* return value == 0 : successful         */
/*              != 0 : error              */
/* Parameter 1       : returns acb handle */
/* Parameter 2       : ecb handle         */
/* Parameter 3       : pm number          */
/* Parameter 4       : pm event           */
/* Parameter 5       : p1                 */
/* Parameter 6       : p2                 */
/* Parameter 7       : p3                 */
/* Parameter 8       : p4                 */

/* ****************** Push event to IRTS interface ****************** */
/* ******************                              ****************** */

signed short int PASCAL ro0221_pushOnInterface  PROTO ((signed   short int,
                                                        signed   short int,
                                                        signed   long  int,
                                                        unsigned long  int,
                                                        unsigned long  int,
                                                        unsigned long  int,
                                                        unsigned long  int));

/* Return value == 0 : successful                                        */
/*              != 0 : error                                             */
/*                                                                       */
/* Parameter 1       : mode                                              */
/*                   :      == 0: return provided buffer                 */
/*                          <  0: pm event/association no more available */
/*                   :      >  0: normal pm event                       */
/*                                                                       */
/*                     mode != 0               |  mode == 0              */
/* Parameter 2       : pm number               |  not defined            */
/* Parameter 3       : pm event                |  not defined            */
/* Parameter 4       : p1                      |  not defined            */
/* Parameter 5       : p2                      |  not defined            */
/* Parameter 6       : p3                      |  returned buffer        */
/* Parameter 7       : p4 = specFlag condition |  not defined            */

/* ****************** Get address length **************************** */
/* ******************			 **************************** */

signed long int PASCAL ro0222_getAddrLength PROTO ((unsigned long int FAR *,
						    unsigned long int	   ));

/* return value == 0 : successful,                                      */
/*              != 0 : error                                            */
/* Parameter 1	     : Returns the length of address in bytes. (Returns */
/*		       null if parameter 2 is equal to NULL handle)	*/
/* Parameter 2	     : Memory handle where the address is stored or	*/
/*		     : the NULL handle. 				*/

/* ****************** Get scb handle ******************************** */
/* ******************                ******************************** */

unsigned long int PASCAL ro0223_getScb PROTO ((void));

/* Return value != 0 : successful         */
/*              == 0 : error              */

/* ****************** Name association ****************************** */
/* ******************		       ****************************** */

signed short int PASCAL ro0224_nameAcb PROTO ((unsigned long  int  ,
					       char	      FAR *,
					       unsigned long  int  ,
					       char	      FAR *,
					       unsigned long  int  ,
					       char	      FAR *));

/* Return value == 0 : successful				  */
/*		 > 0 : Such a named acb is allready active.	  */
/*		 < 0 : error					  */

/* Parameter 1	     : Length of name part 1. Max. is 64 bytes.   */
/* Parameter 2	     : Pointer to where name part 1 is read from. */
/* Parameter 3	     : Length of name part 2. Max. is 64 bytes.   */
/* Parameter 4	     : Pointer to where name part 2 is read from. */
/* Parameter 5	     : Length of name part 3. Max. is 4 bytes.	  */
/* Parameter 6	     : Pointer to where name part 3 is read from. */

/* ****************** Conncatenate acb's **************************** */
/* ******************			 **************************** */

signed short int PASCAL ro0225_conAcbs PROTO ((unsigned long  int FAR *,
					       signed	short int      ,
					       unsigned long  int      ,
					       char		  FAR *,
					       unsigned long  int      ,
					       char		  FAR *,
					       unsigned long  int      ,
					       char		  FAR *));

/* Return value == 0 : Succesful.				      */
/*		 > 0 : No old such named acb found		      */
/*		 < 0 : error					      */
/*								      */
/* Parameter 1	     : Pm context handle returned.		      */
/*								      */
/* Parameter 2	     : Pm number indicating the PM cxt handle which   */
/*		       will be returned where parameter 1 points to in*/
/*		       the case of success.			      */
/*								      */
/*		       Name value used for search:		      */
/*								      */
/* Parameter 3	     : Length of name part 1. Max. is 64 bytes.       */
/* Parameter 4	     : Pointer to where name part 1 is read from      */
/* Parameter 5	     : Length of name part 2. Max. is 64 bytes.       */
/* Parameter 6	     : Pointer to where name part 2 is read from.     */
/* Parameter 7	     : Length of nmae part 3. Max. is 4 bytes.	      */
/* Parameter 8	     : Pointer to where name part 3 is read from.     */

/* ****************** Change state of PM **************************** */
/* ******************			 **************************** */

void PASCAL ro0226_changeState PROTO ((unsigned long int));

/* Parameter 1 : Pm context handle			     */
/*		 Idle state must be NULL. It may be used if  */
/*		 PM invocation only needs a state vector.    */

/* ****************** Set special flag condition ******************** */
/* ******************				 ******************** */

void PASCAL ro0227_setSpecFlg PROTO ((void));

/* ****************** Get tpdu function pointer ********************* */
/* ******************				********************* */

FARPROC PASCAL ro0228_getTpduFct PROTO ((void));

/* Return value == 0: error.  No tpdu call back function defined.     */
/*		!= 0: Pointer to tpdu call back function.	      */
/*								      */

/* ****************** Function to save lower interface's  *********** */
/* ****************** association (connection) identifier *********** */
/* ****************** within the running acb.		  *********** */

void	PASCAL ro0229_setLoifId PROTO ((signed long int));

/* Parameter 1 : Identifier which must be stored.	     */
/*							     */

/* ****************** Search acb handle if lower interface's ******** */
/* ****************** association (connection) identifier    ******** */
/* ****************** is only known.			     ******** */

unsigned long int PASCAL ro0230_seaLoifId PROTO ((signed long  int,
						  signed short int));

/* Return value == 0 : Not found.				      */
/*		 > 0 : Acb handle of acb where the lower interface's  */
/*		       assoc. (connection) identierfer is stored and  */
/*		       pm is invoked.				      */
/*								      */
/* Parameter 1	     : Lower interfaces assoc. (connection) identifier*/
/* Parameter 2	     : Pm number.				      */
/*								      */

/* ******** Inform functions Used by the Pms.  ********************** */

   signed   short int  PASCAL ro0231_getInform PROTO (( 
   unsigned long  int  ,
   signed   short int  ,
   signed   short int  ,
   void     FAR       *));

/* Parameter 1       : Offset of the acb offset for the Acb array     */
/* Parameter 2       : The Pm number of the Inform function           */
/* Parameter 3       : Information type                               */
/* Parameter 4       : Pointer to the Information structure           */

/* ************* get address for the ecb ***************************  */
signed short int PASCAL ro0232_getAddress2 PROTO ((
   unsigned long  int       ,
   signed   short int       ,
   unsigned short int       ,
   char               FAR   *));

/* Parameter 1       : Ecb offset for the Ecb array                   */
/* Parameter 2       : layer for which the Address is required        */
/* Parameter 3       : Length of the address                          */
/* Parameter 4       : Char * for the return Address                  */

/* ****************************************************************** */
/* ****************** Structures and functions used for ************* */
/* ****************** recovery data bass operatios	************* */
/* ****************************************************************** */
/* ****************************************************************** */

#define RC0249_maxRdbHandle  8

typedef struct rS0200_rdbheader 				{
	       unsigned long  int uBasLength			;
	       unsigned long  int uLenOfSub[RC0249_maxRdbHandle];
	       unsigned long  int hHanOfSub[RC0249_maxRdbHandle];
} rT0200_rdbHeader;

/* ****************** Register ppm invocation and  ****************** */
/* ****************** ppm destruction and Recovery ****************** */
/* ****************** data bass access. 	   ****************** */

signed short int PASCAL ro0235_regPmRdbInv PROTO ((unsigned long int));

/* Return value == 0 : successful         */
/*              != 0 : error              */
/* Parameter 1       : pm context handle  */


signed short int PASCAL ro0236_regPmRdbDes  PROTO ((void));

/* Return value == 0 : successful         */
/*              != 0 : error              */

/* ****************** Write pm context to RDB file ****************** */
/* ******************				   ****************** */

signed short int PASCAL ro0237_pmRdbWrite  PROTO ((void));

/* Return value == 0 : successful         */
/*              != 0 : error              */


/* ****************** Reset ACB ExActive Flag *********************** */
/* ******************			      *********************** */

void		 PASCAL ro0238_resetExActiveFlag  PROTO ((void));

/* ****************************************************************** */
/* ******************			   ************************** */
/* ****************** Pdv-Tree operations. ************************** */
/* ****************************************************************** */
/* ****************************************************************** */

/* ****************** Clean up pdv tree ***************************** */
/* ******************			***************************** */

signed short int PASCAL ro0246_clearPdvTree PROTO ((unsigned long int FAR *));

/* Return value == 0: successfully.				      */
/*		!= 0: error					      */
/*								      */
/* Parameter1	    : --> Memory handle to PDV node (pointer to)      */

#endif	/* _RODPAT0_H */
