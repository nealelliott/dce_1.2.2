/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif92.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:49  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:55  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:18  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:52  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:05  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:38  keutel
 * 	creation
 * 	[1994/03/21  13:33:55  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roloif92.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:49 $" } ;
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
 
/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : roloif9.c                                           */
/*                                                                    */
/* AUTHOR       : Monika Kraue                                        */
/* DATE         : 89-09-30                                            */
/*                                                                    */
/* COMPONENT    : RTS session                                         */
/*                                                                    */
/* PRD#/VERS.   : RTS   V2.0                                          */
/*                                                                    */
/* DESCRIPTION  : This modul contains functions which are calling     */
/*                all functions of the underlying session product     */
/*                                                                    */
/*   Only functions of this modul call functions of the session       */
/*   and the tpdu interface.                                          */
/*                                                                    */
/* SYSTEM DEPENDENCE :  MAKRO constant BS2000 defined    : for BS2000 */
/*                      MAKRO constant BS2000 not defined: for SINIX  */
/*                      This modul also depends on the session        */
/*                      interface of OSS-V2.0                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*datoff **************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/
 
static char *sccsid =    "@(#) roloif92.c 1.1 93/11/29   RTS-V2.00";
 
/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/
 
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <cmx.h>         /* Include of transport system              */
#include <oss.h>         /* Include of session system                */

#include <rogenrl.h>
#include <roerval.h>     /* Include of RTS error utilites            */
#include <routil0.h>     /* Prototyping and Trace macros             */
			 /* Constants e.g. INTERFACE , NewFile       */
#include <roloif9.h>     /* interface to session                     */


/*********************************************************************/
/*                                                                   */
/*           G L O B A L   F U N C T I O N S                         */
/*                                                                   */
/*********************************************************************/
 
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0340_openLog                                      */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 30-09-91                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
 
signed short int PASCAL ro0340_openLog (
char FAR              *fileName,
unsigned short int     fileMode,
unsigned short int      options)
 
/*                                                                    */
/* DESCRIPTION  :  This function calls the session trace "trace-on"   */
/*                 function.                                          */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*        filename  :  Name of the file where the logging information */
/*                     is written in.                                 */
/*                                                                    */
/*        filemode  :  Overwrite the logging file.                    */
/*                                                                    */
/*        options   :  logging level.                                 */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*								      */
/*      = 0          successfully                                     */
/*      < 0          error                                            */
/*      > 0          R_ICMXER    session interface error values       */
/*                   R_ISYSOS                                         */
/*                   R_IOSSER                                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
 
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 
struct s_tropt1      sopt;     /* Variable used to pass trace options */
                               /* to session trace system.            */
 
signed short int      rrc;     /* Used to compute the return code  of */
                               /* this function.                      */
 
int                   src;     /* Variable used to store return code  */
                               /* of the session function.            */
int            errorValue;
 
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 

sopt.s_trver   = S_TROPT1;
sopt.s_trmode  = (fileMode == RC0025_NewFile) ? S_TR_NEW : S_TR_EXT;
/**********************************************************************/
/* aus oss.h:                                                         */
/* S_TR_NEW: create a new trace file                    "0"           */
/* S_TR_EXT: extend old or create new file              "1"           */
/* S_TR_SERV: switch the service trace on                             */
/* S_TR_PROT: switch the protocoll trace on                           */
/* S_TR_NOEV: trace s_event with NOEVENT result                       */
/* S_TSERR: Fehler beim Transportsystem               "301"           */
/* S_SYSERR: error on system call                     "300"           */
/**********************************************************************/

if (options & RC0022_ACTIVITY ) {
    sopt.s_mludata = S_TR_UNLIM;
    sopt.s_mldt    = S_TR_UNLIM;
    sopt.s_mltd    = S_TR_UNLIM;
    }
else{
    sopt.s_mludata = 0;
    sopt.s_mldt    = 0;
    sopt.s_mltd    = 0;
    }
 
sopt.s_trsel   = S_TR_USER;
if (options & RC0020_INTERFACE)  sopt.s_trsel |= S_TR_SERV;
if (options & RC0022_ACTIVITY )  sopt.s_trsel |= S_TR_PROT;
 
sopt.s_traopt  = (options & (RC0020_INTERFACE | RC0023_CWAITTRACE))
               ? S_TR_NOEV : (char) 0;
 
/* ****************** Call session trace on function. *************** */
/* ******************                                 *************** */
 
src = s_tron (fileName, &sopt);
 
/* ****************** Compute function return code ****************** */
/* ******************                              ****************** */
 
if (src == S_OK){ rrc = OK;                                        }
else            { src = s_error (&errorValue);
 
                  switch (src)
                       {
                  case S_TSERR : rrc               = - R_ICMXER; break;
                  case S_SYSERR: rrc               = - R_ISYSOS; break;
		  default      : rrc               = - R_IOSSER; break;
                       }                                                }
return (rrc);
}/*end*/

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0341_closeLog                                     */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 01-10-91                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
 
 signed short int PASCAL ro0341_closeLog (void)
 
/*                                                                    */
/* DESCRIPTION  :  This function calls the session trace "close       */
/*                 logging file" function.                            */
/*                                                                    */
/* IN-PARAMETERS  :  none                                             */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        OK                                                          */
/*        R_ICMXER                                                    */
/*        R_ISYSOS                                                    */
/*        R_IOSSER                                                    */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
 
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 
signed short int      rrc;     /* Used to compute the return code  of */
                               /* this function.                      */
 
int                   src;     /* Variable used to store return code  */
                               /* of the session function.            */

int            errorValue;
 
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 
/* ****************** Call session function to close **************** */
/* ****************** logging file.                  **************** */
 
src = s_troff((char *) 0);
 
/* ****************** Compute function return code ****************** */
/* ******************                              ****************** */
 
if (src == S_OK){ rrc = OK ;                                        }
else            { src = s_error (&errorValue);
 
                  switch (src)
                       {
                  case S_TSERR : rrc               = - R_ICMXER; break;
                  case S_SYSERR: rrc               = - R_ISYSOS; break;
		  default      : rrc               = - R_IOSSER; break;
                       }                                                }
return (rrc);
}/*end*/

 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0342_writeLog                                     */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 01-10-91                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
 
signed short int PASCAL ro0342_writeLog (
RS0001_logRecHead FAR *header,
char              FAR *record)

/*                                                                    */
/* DESCRIPTION  :  This function calls the session trace "write       */
/*                 logging record function".                          */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*        header  : Logging record header.                            */
/*                                                                    */
/*        record  : Logging record.                                   */
/*                                                                    */
/* OUT-PARAMETERS :  none                                             */
/*                                                                    */
/* RETURN VALUES  :                                                   */
/*                                                                    */
/*        OK                                                          */
/*        R_ICMXER                                                    */
/*        R_ISYSOS                                                    */
/*        R_IOSSER                                                    */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

 
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 
signed short int      rrc;     /* Used to compute the return code  of */
                               /* this function.                      */
 
int                   src;     /* Variable used to store return code  */
                               /* of the session function.            */

int            errorValue;
 
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 
/* ****************** Call session function for write *************** */
/* ****************** a logging file record.          *************** */
src = s_wutr ((int  *) 0                     ,
	      RC0029_RosLogType              ,
              (char *) header                ,
	      sizeof (RS0001_logRecHead)     ,
              record                         ,
	      (int) header->rs0001_rlength   );
 
/* ****************** Compute function return code ****************** */
/* ******************                              ****************** */
 
if (src == S_OK){ rrc = OK;                                        }
else            { src = s_error (&errorValue);
 
                  switch (src)
                       {
                  case S_TSERR : rrc               = - R_ICMXER; break;
                  case S_SYSERR: rrc               = - R_ISYSOS; break;
		  default      : rrc               = - R_IOSSER; break;
		       }
		}
return (rrc);
}/*end*/
