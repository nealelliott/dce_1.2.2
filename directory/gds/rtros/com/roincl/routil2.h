/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil2.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:22  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:14  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:45  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:00  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:09  keutel
 * 	creation
 * 	[1994/03/21  13:19:40  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUTIL2_H
#define _ROUTIL2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _routil2_rcsid[] = { "@(#) $RCSfile: routil2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:05 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1992 by                                                  *
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
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : routil2.h                                           */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 11-JUN-1992                                         */
/*                                                                    */
/* COMPONENT    : RTS common utilities                                */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : RTS common utilities                                */
/*                                                                    */
/*                - Logging system facility                           */
/*                                                                    */
/* SYSTEM DEPENDENCE :  no                                            */
/*                                                                    */
/* HISTORY      : sccsid= "@(#) routil2.h 1.2 93/12/20"                           */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*   1.00   | 11-06-92|  original                      | gj |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#define RC0027_ON            1    /* used to switch logging on or off */
#define RC0028_OFF           0    /* used to switch logging on or off */
#define RC0029_MAXCONF     512    /* read conf-file after 512 records */

#define CONSTlow             1    /* minimum of parameters with pack10Log */
#define CONSThigh           16    /* maximum of parameters with pack10log */

#define CONSTmax          5120    /* max. length for the va-list-store    */

#define RC1209_FIELDERR   0xffff    /* signal for a non present value */

/* ****************************************************************** */
/* ************************  Logging facility             *********** */

#define RC0040_MaxStrLen               16          /*   - max length  */
                                                   /*     modul/func- */
                                                   /*     name        */

/* ****************  RTS-V2.0 Logging administration  *************** */

typedef struct {                             /* r_logpar                      */
    unsigned short int        r_logLevel;    /* logging level                 */
    char                      r_fname1[120]; /* file name for logging, .l1 Ext*/
    char                      r_fname2[120]; /* file name for logging, .l2 Ext*/
    unsigned short int        r_fselect;     /* which file is used (.l1/.l2)  */
    unsigned short int        r_recnum;      /* max. no. of records in file   */
    unsigned short int        r_actnum;      /* actual no. of written records */
    unsigned short int        r_confreadnum; /* when this counter expires the */
				             /* cfg-file will be read again   */
    signed   short int        r_logging;     /* shows whether logging on/off  */
    unsigned long  int        hFile;         /* File handle                   */
    void                FAR * r_fileinfo;    /* File Internal Info            */
} rs0002_logpar;

/* **********************  Prototypes  ********************************* */

signed short int PASCAL ro004c_GetLogParams PROTO((
					    unsigned short int FAR *loglevel,
					    unsigned short int FAR *recnum,
					    unsigned short int FAR *confnum,
					    char FAR * FAR     *fname));

       /*******************************************************/
       /* DESCRIP:  Reads the logging configuration           */
       /* RETURN:      0  : if ok                             */
       /*             !0  : if no cfg-file found              */
       /* INPUT:  loglevel: dummy                             */
       /*          recnum : dummy                             */
       /*         confnum : dummy                             */
       /*           fname : dummy                             */
       /* OUTPUT: loglevel: logging level selector            */
       /*          recnum : max no. of records in log-file    */
       /*         confnum : no. of records after which the    */
       /*                   cfg-file will be read again.      */
       /*           fname : filename for logging output       */
       /*******************************************************/

#endif	/* _ROUTIL2_H */
