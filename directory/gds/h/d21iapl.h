/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21iapl.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:59:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:29  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:55:44  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:27  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:18:20  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:28  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:48:18  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:43:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:18:05  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:38:04  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:39:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  03:56:26  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:41:19  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D21IAPL_H
#define _D21IAPL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d21iapl_rcsid[] = "@(#)$RCSfile: d21iapl.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:32 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* File Name	: d21iapl.h						*/
/*									*/
/* Description	: This module contains definitions and prototyping	*/
/*		  for the IAPL interface of the DUA			*/
/*									*/
/* Date		: January 21, 1987					*/
/*									*/
/* Author	: W. Schmid						*/
/*	(newer stuff: Jochen Keutel, SNI BA NM 123, Munich, Germany)	*/
/*									*/
/*----------------------------------------------------------------------*/

#include <dce/dce.h>
#include <dce/d2dir.h>

#define D21V02          10      /* version number of IAPL-SS         */
#define D21V021         11      /* version number of 2.01A          */
#define D21V022         12      /* version number of 2.2            */
#define D21MIN1     D21V021
#define D21MAX1     D21V022


#define D21DIR_ID        1      /* Directory-Identification           */

/************************** d21_1_bind() ******************************/
typedef struct
      {   D2_pbhead        *d21_1pbhead;    /* -> standard header */
	  signed16             d21_1dir_id;    /* -> directory_id        */
	  D2_credent       *d21_1u_credent; /* -> user credentials */
	  D2_name_string    d21_1name_dsa;  /* -> BIND-DSA   */
	  D2_a_value       *d21_1addr_dsa;  /* -> adress of BIND-DSA */
	  D2_name_string    d21_1ret_dsa;   /* <- BIND-DSA   */
					    /*    (given free by user)*/
	  signed16             d21_1bind_id;   /* <- bind_id             */
	  signed16	    d21_1auth_mech; /* -> auth. mechanism to  */
					    /* be used in the bind    */
					    /* operation.             */
	  D2_str	    *d21_1auth_info; /* -> auth. information      */
					    /* (not yet used)	      */
      } D21_p1;


/************************** d21_2_unbind() ****************************/
typedef struct
      {   D2_pbhead        *d21_2pbhead;    /* -> standard header */
	  signed16             d21_2bind_id;   /* -> Bind_Id             */
      } D21_p2;


/************************** d21_3_read() ******************************/
typedef struct
      {   D2_pbhead        *d21_3pbhead;    /* -> standard header */
	  D2_c_arg         *d21_3c_a;       /* -> common arguments    */
	  signed16             d21_3bind_id;   /* -> bind_id             */
					    /*    (if read to cache   */
					    /*    then dirid)         */
	  D2_name_string    d21_3entry;     /* -> name */
	  D2_req_info      *d21_3r_info;    /* -> requested info */
	  D2_ei_info       *d21_3res;       /* <- result */
	  D2_error         *d21_3err;       /* <- Pointer to add.     */
					    /*    error information   */
	  D2_c_res         *d21_3cres;      /* <- Pointer to common   */
					    /*    results             */
      } D21_p3;


/************************** d21_4_list() ******************************/
typedef struct
      {   D2_pbhead        *d21_4pbhead;    /* -> standard header */
	  D2_c_arg         *d21_4c_a;       /* -> common arguments    */
	  signed16             d21_4bind_id;   /* -> Bind-Id             */
					    /*    (if read to cache   */
					    /*    then dirid)         */
	  D2_name_string    d21_4parent;    /* -> parent */
	  char            * d21_4file;      /* -> file name (or NULL pointer) */
	  D2_l_info        *d21_4res;       /* <- result */
	  D2_ref_inf       *d21_4rf;        /* <- result referrals    */
	  D2_name_string    d21_4dist_name; /* <- Distinguished name  */
	  signed16             d21_4lim_prob;  /* <- set for limit problem */
	  Bool              d21_4uav_crit_ext;  /* <- indicates       */
					    /* unavailable critical   */
					    /* extension              */
	  D2_error         *d21_4err;       /* <- Pointer to add.     */
					    /*    error information   */
	  D2_c_res         *d21_4cres;      /* <- Pointer to common   */
					    /*    results             */
      } D21_p4;


/************************** d21_5_search() ****************************/
typedef struct
      {   D2_pbhead        *d21_5pbhead;    /* -> standard head */
	  D2_c_arg         *d21_5c_a;       /* -> common arguments    */
	  signed16             d21_5bind_id;   /* -> bind_id             */
					    /*    (if read to cache   */
					    /*    then dirid)         */
	  D2_name_string    d21_5base_obj;  /* -> base object */
	  D2_subset         d21_5subset;    /* -> depth */
	  Bool              d21_5srch_al;   /* -> indicates whether   */
					    /*    aliases below the   */
					    /*    base object must be */
					    /*    dereferenced        */
	  D2_Fi_filter     *d21_5f_pattern; /* -> filter */
	  D2_req_info      *d21_5r_info;    /* -> requested info */
	  char            * d21_5file;      /* -> file name (or NULL pointer) */
	  D2_ei_info       *d21_5res;       /* <- result */
	  D2_ref_inf       *d21_5rf;        /* <- Pointer to result  */
					    /*    referrals           */
	  D2_name_string    d21_5dist_name; /* <- Distinguished Name  */
	  signed16             d21_5lim_prob;  /* <- set for limit problem */
	  Bool              d21_5uav_crit_ext;  /* <- indicates       */
					    /* unavailable critical   */
					    /* extension              */
	  D2_error         *d21_5err;       /* <- Pointer to add.     */
					    /*    error information   */
	  D2_c_res         *d21_5cres;      /* <- Pointer to common   */
					    /*    results             */
      } D21_p5;


/************************** d21_6_compare() ***************************/
typedef struct
      {   D2_pbhead        *d21_6pbhead;    /* -> standard header */
	  D2_c_arg         *d21_6c_a;       /* -> common arguments    */
	  signed16             d21_6bind_id;   /* -> bind_id             */
	  D2_name_string    d21_6entry;     /* -> Distinguished name       */
	  D2_ava           *d21_6comp;      /* -> AVA to compare    */
	  D2_name_string    d21_6dist_name; /* <- Distinguished name       */
	  Bool              d21_6match;     /* <- result   */
	  Bool              d21_6from_entry;/* <- indicates if it is */
					    /*    a master entry or a copy   */
	  D2_error         *d21_6err;       /* <- Pointer to add.     */
					    /*    error information   */
	  D2_c_res         *d21_6cres;      /* <- Pointer to common   */
					    /*    results             */
      } D21_p6;


/************************** d21_7_add_object() ************************/
typedef struct
      {   D2_pbhead        *d21_7pbhead;    /* -> standard header */
	  D2_c_arg         *d21_7c_a;       /* -> common arguments    */
	  signed16             d21_7bind_id;   /* -> Bind-Id             */
					    /*    (if read to cache   */
					    /*    then dirid)         */
	  D2_name_string    d21_7entry_name;/* -> Distinguished Name  */
	  D2_a_info        *d21_7_at_info;  /* attribute informations */
					    /* (queue of attribute    */
					    /* information)           */
	  D2_error         *d21_7err;       /* <- Pointer to add.     */
					    /*    error information   */
      } D21_p7;


/************************** d21_8_remove_object() *********************/
typedef struct
      {   D2_pbhead        *d21_8pbhead;    /* -> standard header */
	  D2_c_arg         *d21_8c_a;       /* -> common arguments    */
	  signed16             d21_8bind_id;   /* -> bind_id             */
					    /*    (if read to cache   */
					    /*    then dirid)         */
	  D2_name_string    d21_8entry;     /* -> Distinguished Name  */
	  D2_error         *d21_8err;       /* <- Pointer to add.     */
					    /*    error information   */
      } D21_p8;

/************************** d21_B_replace_rdn() ***********************/
typedef struct
      {   D2_pbhead        *d21_Bpbhead;    /* -> standard header */
	  D2_c_arg         *d21_Bc_a;       /* -> common arguments    */
	  signed16             d21_Bbind_id;   /* -> Bind_Id             */
	  D2_name_string    d21_Bentry;     /* -> Distinguished name       */
	  D2_name_string    d21_Bnew_rdn;   /* -> new RDN           */
	  Bool              d21_Bdel_old_rdn;   /* -> indicates, if   */
					    /*    old RDN must be     */
					    /*    removed             */
	  D2_error         *d21_Berr;       /* <- Pointer to add.     */
					    /*    error information   */
      } D21_pB;


/************************** d21_C_modify_attribute() ******************/
typedef struct
      {   D2_pbhead        *d21_Cpbhead;    /* -> standard header */
	  D2_c_arg         *d21_Cc_a;       /* -> common arguments    */
	  signed16             d21_Cbind_id;   /* -> Bind_Id             */
	  D2_name_string    d21_Centry;     /* -> Distinguished Name  */
	  D2_a_modinf      *d21_Cmod;       /* -> description of the */
					    /*    modification      */
	  D2_error         *d21_Cerr;       /* <- Pointer to add.     */
					    /*    error information   */
      } D21_pC;


/* prototypes */

d2_ret_val d21_1_bind ( D21_p1 * );
d2_ret_val d21_2_unbind ( D21_p2 * );
d2_ret_val d21_3_read ( D21_p3 * );
d2_ret_val d21_4_list ( D21_p4 * );
d2_ret_val d21_5_search ( D21_p5 * );
d2_ret_val d21_6_compare ( D21_p6 * );
d2_ret_val d21_7_add_object ( D21_p7 * );
d2_ret_val d21_8_remove_object ( D21_p8 * );
d2_ret_val d21_B_replace_rdn ( D21_pB * );
d2_ret_val d21_C_modify_attribute ( D21_pC * );

/* more prototypes - used by XDS for Abandon */

#ifdef THREADSAFE
int d21_old_cancel_state ( void );
void d21_store_old_cancel_state ( int );
#else
boolean d21_abandon_is_indicated ( void );
void d21_reset_abandon_indication ( void );
void d21_set_abandon_indication ( void );
#endif /* THREADSAFE */

#endif /* _D21IAPL_H */
