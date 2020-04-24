/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: romoni01.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:54  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:56  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:25  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:01  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:42  keutel
 * 	creation
 * 	[1994/03/21  13:19:33  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROMONI01_H
#define _ROMONI01_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _romoni01_rcsid[] = { "@(#) $RCSfile: romoni01.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:47 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c)                                                          *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1993                            *
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
/* NAME 	: romoni01.h		           		      */
/*                                                                    */
/* AUTHOR       : Milind m. Aphale                                    */
/* DATE 	: 93-05-14        				      */
/*                                                                    */
/* COMPONENT	: RTROS PDU MONITOR               		      */
/*                                                                    */
/* PRD#/VERS.	: RTROS-V03.00					      */
/*                                                                    */
/* DESCRIPTION	: defines required by PDU monitor module.             */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#) romoni01.h 1.1 93/11/29 RTROS-V3.00              */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*          |         |                                |    |         */
/* 03.00 K0 | 93-05-14|  original		       | ma |	      */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

/*********************************************************************/

#define  EINTR                    4       /* interrupted system call  */  
#define  R_MONI_PMERR             1 
#define  R_MONI_ERR               3 
#define  R_MONI_STORE_ERR         2
#define  R_MONI_SUCS              0 

#define PDUMONITOR_ON        0   /* SUCCESS             */
#define PDUMONITOR_OFF       1   /* ERROR but not panic */

/* static int   r_v090_envFlag   =  (R_int) 0; 
static char *r_v091_envString =   "ROSPDU"; 
static char *r_v092_envSwitch =       "ON";  */

/*****
static char *r_s09b = "RTROSV3.0-pduTM ->> Saveing  received  ppdu  part ? "; 
static char *r_s09c = "RTR0SV3.0-pduTM ->> Replaceing received ppdu part ? "; 
static char *r_s09d = "RTR0SV3.0-pduTM ->> Saveing   sending  ppdu  part ? "; 
static char *r_s09e = "RTR0SV3.0-pduTM ->> Replaceing sending  ppdu part ? "; 
******/

static char *r_s09b = "RTROSV3.0-pduTM ->> Saving    ppdu part ? "; 
static char *r_s09c = "RTR0SV3.0-pduTM ->> Replacing ppdu part ? "; 

static char *r_s09h = "\n      ->> Enter file name ";                      
static char *r_s09k = "\n      - ***  E R R O R  *** Open file failed\n\n";
                                                                           
static char  DefaultFile      [128] = { "/tmp/savePpdu"   };               
static char  DefaultSourceFile[128] = { "/tmp/sourcePpdu" };               
                                                                           
static char *r_s09y = "      -   Computed number = %d\n";                  
static char *r_s09z = "      ->> Number ok ? ";                            

/* extern char *getenv(); */
char          *buffer;  

#define  EINTR                     4      /* interrupted system call  */  
#define  R_MONI_PMERR              1 
#define  R_MONI_ERR                3 
#define  R_MONI_STORE_ERR          2
#define  R_MONI_SUCS               0 

#define PDUMONITOR_ON        0   /* SUCCESS             */
#define PDUMONITOR_OFF       1   /* ERROR but not panic */

#endif	/* _ROMONI01_H */
