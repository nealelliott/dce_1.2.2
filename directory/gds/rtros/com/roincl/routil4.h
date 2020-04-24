/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil4.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:25  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:40  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:51  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:06  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:12  keutel
 * 	creation
 * 	[1994/03/21  13:19:45  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUTIL4_H
#define _ROUTIL4_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _routil4_rcsid[] = { "@(#) $RCSfile: routil4.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:07 $" } ;
#endif

/*************************************************************************\
*                                                                         *
*  * Copyright (c) 1992 by                                                *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                               *
*  * All rights reserved                                                  *
*  *                                                                      *
*  * This software is furnished under licence and may be used only in     *
*  * accordance with the terms of that licence and with the inclusion of  *
*  * the above copyright notice. This software may not be provided or     *
*  * otherwise made available to, or used by, any other person. No title  *
*  * to or ownership of the software is hereby transferred.               *
*                                                                         *
\*************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : routil4.h                                           */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : MAR-8th-1992                                        */
/*                                                                    */
/* COMPONENT    : Malloc Controlling Unit which                       */
/*                checks allocated area for:                          */
/*                  - Overwriting                                     */
/*                  - Global/Local mismatch                           */
/*                  - freeing only unlocked areas                     */
/*                  - non matching sizes during realloc               */
/*                  - Handle mismatch (by providing real handles)     */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)routil4.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*  1.00    | 92/03/01|  original                      | gj |         */
/*  1.01    | 92/03/15|  new: M_FATAL + M_NEGLIG       | gj |         */
/*datoff **************************************************************/

/* ********************************************************************/
/* **********             Malloc check utility             ********** */
/* **********        internal definitions for routil4.c    ********** */
/* ********************************************************************/

#define MAXMALL         512              /* number of checked mallocs */
#define CHECKSIZE        16              /* check-area size (ovwrt)   */
                                         /* changed by kc on 18-08-93 */
                                         /* for RM portation          */
#define CHECKBYTE      0xaa              /* ok-byte in check area     */
#define MAXMALLHDL     0xffffffL         /* limit for handle number   */
#define HDLOFFSET       100L             /* Offset for HdlCtr to      */
					 /* avoid 0-Handles           */


#define MODULNAMLEN      16              /* max. modul name length    */
#define MESSLEN          80              /* max. length of abort-msg  */
#define HEADLEN          40              /* max. length of header-msg */


#define MALLFILEBIN  "mallfile.bin"      /* Current malloc table      */
#define MALLPTKLTXT  "mallfile.pcl"      /* malloc-protocol file      */
#define MALLFILETXT  "mallfile.txt"      /* converted table file      */


#define NOTYPE            0              /* undefined type            */
#define GLOBALTYPE        1              /* Source is a global fct    */
#define LOCALTYPE         2              /* Source is a local fct     */

#define M_YES             1              /* means logical yes or no   */
#define M_NO		  0	         /* for the malloc-controller */

#define M_NEGLIG          5              /* negligibal error type     */
#define M_FATAL           6              /* fatal error type          */

typedef struct {
    unsigned long  int    MallHdl;        /* Handle to allocation     */
    unsigned short int    LockCounter;    /* Counts number of Locks   */
    unsigned short int    Type;           /* Whether global or local  */
    char FAR *            address;        /* malloc address           */
    unsigned long int     size;           /* allocated size           */
    int                   line;           /* Module line number       */
    char            modul [MODULNAMLEN];  /* Module name              */
    }  MallInfo;

typedef struct {
    char             header [HEADLEN];    /* Std.-Header of Err-Report */
    char             message[MESSLEN];    /* Abort message             */ 
    int                   line;           /* Module line number        */
    char            modul [MODULNAMLEN];  /* Module name               */
    unsigned long int     hdl;            /* Active handle while abort */
    }  AbortInfo;

#endif	/* _ROUTIL4_H */
