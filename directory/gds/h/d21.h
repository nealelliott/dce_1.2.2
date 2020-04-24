/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:59:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:24  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:47:05  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:41  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:55:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:18  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:18  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:25  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:47:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:43:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:17:00  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:37:53  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:39:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:56  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:08:43  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  09:31:30  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:56:15  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:41:13  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D21_H
#define _D21_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d21_rcsid[] = "@(#)$RCSfile: d21.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:28 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* File Name	: d21.h							*/
/*									*/
/* Description	: This module contains definitions and prototyping	*/
/*		  for the DUA.						*/
/*									*/
/* Date		: January 18, 1988					*/
/*									*/
/* Author	: J. Heigert						*/
/*	(newer stuff: Jochen Keutel, SNI BA NM 123, Munich, Germany)	*/
/*									*/
/*----------------------------------------------------------------------*/

#ifdef THREADSAFE
	#include <pthread.h>
#endif
#include <d23ipc.h>

/**********************************************************************/
/*                                                                    */
/*           D E F I N E S                                            */
/*                                                                    */
/**********************************************************************/

/*****   Common defines   *****/
					/* IPC states                   */
#define D21_NO_DIR_BIND          1
#define D21_DIR_BIND_EXISTS     10

#define D21_BIND_MAX	16		/* max nr of simultaneously  	*/
					/* open binds		     	*/
#define D21_REF_MAX     100              /* max nr of referrals that     */
					/* can be handled within one 	*/
					/* operation		     	*/

#define D21_ANONYM     " "              /* username of anonymous user   */

/****** For service controls ************/

#define D21_UNDEF        0
#define D21_CACHE_ONLY   1
#define D21_DSA_ONLY     2
#define D21_CACHE_FIRST  3
#define D21_DSA_FIRST    4

/****** internal states in "Modify_entry"/"Remove" ***************/
#define         D21_BOUND_TO_MASTER_DSA            20001
#define         D21_MASTER_DSA_OUT_OF_SCOPE        20002
#define         D21_BIND_TO_MASTER_DSA_FAILED      20003
#define         D21_READ_ERR                       20004

#define         D21_EPDU                           1

#define         D21_SHADOW_UPDATE                  10
#define         D21_ONE_UPDATE                     11
#define         D21_DOUBLE_UPDATE                  12
#define         D21_CHAINED_UPDATE                 13
#define         D21_ALIAS_UPDATE                   14
#define		D21_STD_UPDATE			   15


/**** append subtree function ****/
#define D21_OLD_PARENT         0   /* The new parent of the subtree   */
                                   /* is the same that the old one    */
#define D21_NEW_PARENT         1   /* The new parent of the subtree   */
                                   /* is different to the old one     */

/******** error values ***********/

#define D21_ALLOC_ERROR		-1001	/* memory allocation error	*/
#define D21_BINDS_TOO_MANY	-1002	/* bind table overflow		*/
#define D21_REFERRALS_TOO_MANY	-1003	/* referral table overflow	*/
#define D21_SERV_CTRLS_INVALID  -1004	/* invalid service controls	*/
#define D21_NO_ENTRYNAME        -1005	/* no entry name		*/
#define D21_RESULTAPDU_INVALID  -1006	/* wrong invoke id or wrong 	*/
					/* of incoming result APDU	*/
#define D21_LOOPING_REFERRALS   -1007	/* DSAs referred user around in */
					/* a circle			*/
#define D21_CREDENT_ERR		-1008	/* credentials not supplied	*/
#define D21_INTERRUPTED		-1009	/* operation interrupted	*/

/**** FOR *****/
#define D21_BAD_ARGUMENT     6
#define D21_BAD_NAME         9
#define D21_NO_DSA_BIND     60
#define D21_NO_SELECTION    61
#define D21_INV_R_NO_AT     62
#define D21_R_NO_AT_TOO_BIG 63
#define D21_INV_R_VAL_AT    64
#define D21_C_MAXDIR        65
#define D21_INV_SEARCH_SEL  66
#define D21_NO_SC           67
#define D21_ATL_EXCEEDED    68
#define D21_RDN_MISSING     69
#define D21_RDN_TOO_LONG    70
#define D21_AL_OBJ_DN_MISSING 71
#define D21_WRONG_DSA       72
#define D21_DN_MISSING      73

		       /* operation-modes */

#define D21_CACHE_ADD    10             /* Add to cache */
#define D21_SHADOW_ADD   11             /* Add shadow to bind-dsa */
#define D21_NO_MK        12
#define D21_LOCAL_MK     13
#define D21_REMOTE_MK    14
#define D21_ALIAS        15
#define D21_M_LOCAL      16
#define D21_M_REMOTE     17
#define D21_STD_ADD	 18

#define D21_ADD_SHADOW   1
#define D21_ADD_MASTER   2

/*
**********************************************************************
*                                                                    *
*       D A T A  -  D E C L A R A T I O N S                          *
*                                                                    *
**********************************************************************
*/

/*********** STRUCTs ***************/

					/*				*/
					/* info on ipc-association	*/
					/*				*/
typedef struct d21_ipc_info {
	Bool		d21_i_used;	/* TRUE if ass. is already used */
	signed16	d21_i_servid;	/* server-id			*/
	signed16	d21_i_assid;	/* association-id		*/
	byte	      * d21_i_refms;	/* ptr to memory buffer		*/
	signed16	d21_i_lenms;	/* length of memory buffer	*/
	Bool            d21_i_aborted;  /* indicates whether the        */
					/* corresponding IPC association*/
					/* has been aborted or not      */
	D23_evntpb	d21_ipcev_pblk ;	/* IPC-event parameter block */
#ifdef THREADSAFE
	Bool		d21_ipcev_flag ;	/* IPC-event flag */
	pthread_cond_t	d21_ipcev_condvar ;	/* IPC-evt cond. variable */
	pthread_mutex_t	d21_ipcev_mutex ;	/* IPC-event cond. var. mutex */
#endif
	} d21_ipc_info;
	
typedef struct d21_bind_info {
	unsigned16	d21_b_syntax;	/* transfer syntax		*/
	unsigned16	d21_b_used;	/* count of application binds	*/
					/* over dua-dsa-connection	*/
	signed16	d21_b_dir_id;	/* directory-id			*/
	D2_name_string	d21_b_name_dsa;	/* ptr to dsa-name		*/
	D2_credent    * d21_b_credent;	/* ptr to credential info	*/
	signed32	d21_b_usr_id;   /* ipc assoc. id. of the bind	*/
	d21_ipc_info  * d21_b_ipcinf;   /* ipc-assoc.*/
	Bool		d21_b_ipc_used; /* true if assoc. is already used 
						by an operation */
	signed16	d21_b_auth_mech;/* auth. mechanism used in the  */
					/*  bind operation.		*/
	} d21_bind_info;

typedef struct
  {
    signed16       d21_r_opcode;          /* --> Operation  */
    char         * d21_r_bufapdu;         /* --> Pointer to Req/Res-Data  */
    signed32     d21_r_lenapdu;         /* --> Length of the Req/Res-Data */
    d21_ipc_info * d21_r_ipcinfo;         /* --> Pointer to ipc-info block */
  } D21_req_info;

typedef struct {
	D2_name_string  lb_target;      /* Target Name                  */
	D2_op_progress  lb_op_prog;     /* Operation Progress           */
	D2_name_string  lb_dsa_name;    /* DSA-Name                     */
	} D21_lb_info;

typedef struct {
	signed16 op_code;
	signed16 serv_id;
	signed32 inv_id;
	signed32 usr_id;
	} D21_abandon_info;

#endif /* _D21_H */
