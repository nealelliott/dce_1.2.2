/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27log.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:00:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:56  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:50:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:45:43  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:42:15  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:40:43  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:41:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:42  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:06:03  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:29  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D27LOG_H
#define _D27LOG_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d27log_rcsid[] = "@(#)$RCSfile: d27log.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:52 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYP		: INCLUDE					      */
/*								      */
/* NAME         : d27log.h                                            */
/*								      */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 15. 06. 1991                                        */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-REF.    : DS-Design-Specification                             */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                for binary logging.                                 */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 87-12-15| Original                       | WS |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#define   D27_NB_PAR     10
#define   D27_LOG_TYPE   10
#define   D27_USER_LOG   0x10
#define   LEN_FNAME      14

					  /* logging message structures */
typedef struct
	{
	   int      logtype;
	   char     mname[LEN_FNAME];
	   int      line;
	   int      msgno;
	   int      par[D27_NB_PAR];
	} binlog_long;                  /* logging information for type  */
					/* D27_FAT and D27_ERR           */

typedef struct
	{
	   int      logtype;
	   int      msgno;
	   int      par[D27_NB_PAR];
	} binlog_short;                 /* logging information for type  */
					/* D27_FENT, D27_FEXI, D27_NLOG, */
					/* D27_SPEC (1 Argument) and     */
					/* D27_SPEC (2 Arguments)        */

typedef struct
	{
	   int      logtype;
	   int      msgno;
	   char     comment[D27_TTC_LEN + 1];
	   int      par[D27_NB_PAR];
	} ttr_log;                      /* logging information for type  */
					/* D27_PFLOG                     */

#endif /* _D27LOG_H */
