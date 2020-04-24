/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26schema.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:00:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:50  root]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:13  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:17  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:12  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:12:18  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:53:41  marrek]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : d26schema.h                                         */
/*                                                                    */
/* Description  : This Include-File contains extern declarations for  */
/*                variables describing the schema and the database.   */
/*                                                                    */
/* Date         : 12/13/1993                                          */
/*                                                                    */
/* Author       : E. Kraemer, BU BA NM12                              */
/*                                                                    */
/**********************************************************************/

#ifndef _D26SCHEMA_H
#define _D26SCHEMA_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d26schema_rcsid[] = "@(#)$RCSfile: d26schema.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:47 $";
#endif

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

			/* exported by d26util.c                      */

extern char         d26_install[];  /* Installation-Directory         */

extern D26_srt      *d26_srt;       /* Structure rule table           */
extern D26_nam_at   *d26_nam_at;    /* Naming attribute array         */
extern D26_ocl_arc  *d26_ocl_arc;   /* Object class arcs array        */
extern D26_oct      *d26_oct;       /* Object Class table             */
extern D26_at       *d26_at;        /* Attribute-Table                */
extern signed16     *d26_name_pos;  /* array of positions of naming   */
				    /* attrs in each object file.     */
extern signed16     *d26_at_pos;    /* array of positions of other    */
				    /* attrs in each object file.     */
extern byte         *d26_obid_str;  /* string which holds all octal   */
				    /* object ids for classes + attrs.*/
extern signed16     *d26_oct_idx;   /* String of indices to attrs in  */
				    /* the AT and classes in the OCT. */

extern signed32     d26_nosrt;      /* count of structure rules       */
extern signed32     d26_nooct;      /* count of object classes        */
extern signed32     d26_noat;       /* count of attributes in table   */
extern signed32     d26_nofile;     /* count of object files          */
extern signed32     d26_nafile;     /* count of attribute files       */

extern D26_dndesc   *d26_dnlist;    /* Distinguished-Name-list in core*/

			/* exported by d26utilis.c                    */

extern signed32   d26_oreclen;      /* maximal record length (objects)*/
extern signed32   d26_areclen;      /* maximal record length (attr.)  */
extern char       *d26_ob_record;   /* record containing objects      */
extern char       *d26_sn_record;   /* record containing superior node*/
extern char       *d26_a_record;    /* record containing attribute val*/

#endif
