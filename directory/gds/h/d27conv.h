/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: d27conv.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:00:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:55  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:47:12  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:09:55  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:18  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:21  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:54  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:02:16  marrek]
 * 
 * 	Created for April 1994 submission.
 * 	[1994/04/29  10:29:17  marrek]
 * 
 * $EndLog$
 */
/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1994 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Component   : DCE 1.1 GDS Conversion Routines	               */
/*                                                                 */
/* File Name   : d27conv.c                                         */
/* Description : This file contains modules for                    */
/*      	     T.61 to LATIN conversions and back.               */
/* Date        : 15. April 1994                                    */
/* Author      : Wolfgang Schmid, SNI BU BA NM 123                 */
/*                                                                 */
/*-----------------------------------------------------------------*/
#ifndef _D27CONV_H
#define _D27CONV_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d27conv_rcsid[] = "@(#)$RCSfile: d27conv.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:51 $";
#endif


/*-- Includes -----------------------------------------------------*/
/*
 * The following piece of code should be used when the "private" conversion
 * routines are replaced by "ICONV":
 *
 * #include <iconv.h>
 */
#include <gds.h>
#include <d2dir.h>


/*--- defines -----------------------------------------------------*/
#define D27_T61_CONV_FACT 2 /* conversion factor:                  */
/* the factor by which the size of a buffer holding a T61          */
/* string must be multiplied to give a buffer capable of           */
/* holding the longest possible converted local string             */

/*-- Exported Function(s) -----------------------------------------*/


extern d2_ret_val d27_init_conv_tables( void );

extern d2_ret_val d27_close_conv_tables( void );

extern d2_ret_val d27_t61_to_local(const char *inp_str, const unsigned32 inp_len, 
			  char **out_str, unsigned32 *out_len); 

extern d2_ret_val d27_t61_to_local_code(const char *inp_str, unsigned32 inp_len, 
			       char *out_str, unsigned32 *out_len);

extern d2_ret_val d27_local_to_t61(const char *inp_str, const unsigned32 inp_len, 
			  char **out_str, unsigned32 *out_len);

#endif /*D27CONV_H  */
