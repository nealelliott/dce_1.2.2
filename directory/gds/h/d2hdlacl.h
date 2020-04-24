/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2hdlacl.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:24  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:01:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:13  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:52:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:47:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:51:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:42:56  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:41:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:21  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:11:03  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:14  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2HDLACL_H
#define _D2HDLACL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2hdlacl_rcsid[] = "@(#)$RCSfile: d2hdlacl.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:04 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: INCLUDE					      */
/*								      */
/* NAME         : d2hdlacl.h                                          */
/*								      */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 25. 2. 1988                                         */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-REF.    : DS-Design-Specification                             */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                used for DS_HANDLE_ACL                              */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 88-25-02| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

					/* RETURN-VALUES of "HANDLE-  */
					/* ACL"                       */
#define D20_ACC_ILLEGAL        -1       /* illegal access right       */
#define D20_MOD_ILLEGAL        -2       /* illegal modification type  */
#define D20_ACL_WRONG          -3       /* ACL syntax wrong           */
#define D20_TOO_MANY           -4       /* too many DNs               */


			    /* type of ACL modification               */
			    /* --> "D20_mod"                          */
#define D2_MOD_DN       0               /* modify Distinguished Names */
					/* in ACL                     */
#define D2_READ_DN      1               /* read ACL                   */


typedef signed16   D20_mod;                /* type of ACL modification   */


typedef struct
	{   D2_name_string  dn;         /* Distinguished Name in ACL  */
	    D2_dnint        dn_int;     /* Distinguished Name signed32er-  */
					/* pretation                  */
	} D20_obj;                      /* ACL entry                  */


typedef struct
	{   D2_aright    d20_5aright;   /* -> access right to be      */
					/*    handled                 */
	    D20_mod      d20_5mod;      /* -> type of ACL modification*/
	    signed16        d20_5dn_count; /* <> number of DNs           */
	    D20_obj      d20_5dns[D2_NOIDX_ACL];
					/* <> array of Dns to be      */
					/*    handled                 */
	} D20_aclinfo;                  /* ACL information            */


typedef struct
	{   char        *d20_5c_acl;    /* -> current ACL             */
	    signed16        d20_5len;      /* -> length of current ACL   */
	    signed16        d20_5no_rights;/* -> number of access rights */
					/*    to be handled           */
	    D20_aclinfo *d20_5acl;      /* -> ACL info block for each */
					/*    access right            */
	    signed16        d20_5max_len;  /* -> maximum length of new   */
					/*    ACL                     */
	    char        *d20_5n_acl;    /* <- new ACL                 */
	    signed16        d20_5act_len;  /* <- actual length of new ACL*/
	} D20_p5;

#endif /* _D2HDLACL_H */
