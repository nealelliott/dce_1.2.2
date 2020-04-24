/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26gendb.h,v $
 * Revision 1.1.8.2  1996/02/18  23:35:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:54  marty]
 *
 * Revision 1.1.8.1  1995/12/08  16:00:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:48  root]
 * 
 * Revision 1.1.6.2  1994/05/10  15:55:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:04  marrek]
 * 
 * Revision 1.1.6.1  1994/02/22  18:50:11  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:45:24  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  12:40:47  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:40:25  marrek]
 * 
 * Revision 1.1.2.3  1993/02/02  15:02:20  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.2.2  1993/02/02  11:37:58  marrek
 * 	Creation.
 * 	[1993/02/01  16:09:36  marrek]
 * 
 * $EndLog$
 */
#ifndef _D26GENDB_H
#define _D26GENDB_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d26gendb_rcsid[] = "@(#)$RCSfile: d26gendb.h,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 23:35:44 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : d26gendb.h                                          */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 12.01. 1993                                         */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOK.-NR.     : Directory-Design-Specification                      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                used for d26gendb                                   */
/*                                                                    */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 93-01-12| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                    D E F I N E S                                   */
/*                                                                    */
/**********************************************************************/

#define D26_DNLIST    "/dnlist"
#define D26_SCHEME    "/scheme"

/**********************************************************************/
/*                                                                    */
/*                    T Y P E D E F S                                 */
/*                                                                    */
/**********************************************************************/

typedef struct          /* Administrators structure rule table        */
	{
		signed16 prelink;
		signed16 link;
		int      ob_nr;
		int      par_ob_nr;
		signed16 nr_nattr;
		char  name_attr [D2_NO_AVA][D2_L_ACRONYM + 1];
		char  class_anym [D2_L_ACRONYM + 1];
	} Admin_srt;

typedef struct          /* Administrators object class table          */
	{
	    signed16 link;
	    boolean  used;
	    char     acronym [D2_L_ACRONYM + 1];
	    char     obj_id [D2_OBJ_IDL_MAX + 1];
	    char     name [D2_L_AT_NAME + 1];
	    int      kind;
	    int      file_nr;
	    signed16 nr_sup_cl;
	    char     super_anym[D2_NO_SUPCL][D2_L_ACRONYM + 1];
	    signed16 nr_aux_cl;
	    char     aux_anym[D2_NO_AUXCL][D2_L_ACRONYM + 1];
	    signed16 nr_mand;
	    char     mand_anym [D2_NO_MAND][D2_L_ACRONYM + 1];
	    signed16 nr_opt;
	    char     opt_anym [D2_NO_OPT][D2_L_ACRONYM + 1];
	} Admin_oct;

typedef struct          /* Administrators attribute table             */
	{
	    signed16 link;
	    boolean  used;
	    char     acronym [D2_L_ACRONYM + 1];
	    char     obj_id [D2_OBJ_IDL_MAX + 1];
	    char     name [D2_L_AT_NAME + 1];
	    int      lbound;
	    int      ubound;
	    int      nr_recur;
	    int      syntax;
	    int      phon;
	    int      acl;
	    int      index_level;
	} Admin_at;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

			/* exported by d26gendb.c                     */

extern char     dir_fname[];        /* Name of directory for database */
extern signed16 nr_adm_srt;         /* Number of admin_srt entries    */
extern signed16 nr_oct_indices;     /* No of indices contained in OCT */
extern signed16 nr_na_indices;      /* No of SRT naming attr.-indices */
extern signed16 nr_cl_indices;      /* No of object class arcs        */

extern Admin_srt *admin_srt;    /* Contents of SRT info in attr0 file */
extern Admin_oct *admin_oct;    /* Contents of OCT info in attr1 file */
extern Admin_at  *admin_at;     /* Contents of AT info in attr2 file  */

			/* array of shorts which keeps the position  */
			/* of every attribute in every level in the   */
			/* name in every object file                  */
extern signed16 *level_pos;

extern signed16 *d26_name_pos;      /* Array of positions of naming   */
				    /* attributes in each object file */
extern signed16 *d26_at_pos;        /* array of positions of other    */
				    /* attributes in each object file */

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

			/* exported by d26gutilis.c                   */

extern signed16 d26_gi1_read_configuration_data(FILE *fp,
    signed16 *max_attr_val_len);
extern signed16 d26_gi2_read_schema_object(long *recnum, char *schema_time);
extern signed16 d26_gi3_read_attribute_table(long recnum, signed16 tab_nr,
    signed16 nr_tab_entries, signed16 (*read_tab_entry)(char *, short));
extern signed16 d26_gi6_fildes_alloc(void);
extern void     d26_gi7_fildes_free(void);
extern signed16 d26_gi8_create_database(char *schema_time);

#endif /* _D26GENDB_H */
