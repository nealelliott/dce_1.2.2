/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26isam.h,v $
 * Revision 1.1.12.2  1996/02/18  23:35:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:56  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:00:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:49  root]
 * 
 * Revision 1.1.10.2  1994/06/21  14:47:11  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:41:10  marrek]
 * 
 * Revision 1.1.10.1  1994/02/22  18:50:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:45:34  marrek]
 * 
 * Revision 1.1.8.1  1993/10/29  15:11:37  keutel
 * 	CR 9261
 * 	[1993/10/29  14:08:41  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:41:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:40:35  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:40:49  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:39  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:10:01  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  09:32:30  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  04:05:54  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:24  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D26ISAM_H
#define _D26ISAM_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d26isam_rcsid[] = "@(#)$RCSfile: d26isam.h,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 23:35:46 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : INCLUDE                                             */
/*								      */
/* NAME         : d26isam.h                                           */
/*								      */
/* AUTHOR       : E. Kraemer, K SYS AP 11                             */
/* DATE         : 27.12.1988                                          */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.     : Directory-Design-Specification                      */
/*								      */
/* PRD#/VERS.	:						      */
/*								      */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                used for the ISAM-utilities of ISERVICE interfaces. */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 87-12-27| Original                       | ek |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <time.h>
#include <isam.h>
#include <dce/dce.h>

			    /* return values for function d26_i55_... */
#define D26_CMP           1 /* value can be longer than key length    */
			    /* (---> value must be compared after read*/
			    /* record)                                */
#define D26_NCMP          2 /* value isn't longer than key length     */


#define D26_MOPENFILES   5  /* maximum number of open ISAM files      */
#define D26_LIFILE      10  /* length of ISAM-filename                */

/************** filenames used by DSA                           *******/


#define D26_OBJECTS     "/objects"      /* name of ISAM objects file  */
#define D26_ATTR        "/attr"         /* name of ISAM attribute file*/

#define D26_CHDB_DIR    "/chdb"         /* name chdb directory        */

typedef struct
	{   int             d26_isfd;   /* C-ISAM file descriptor     */
	    signed16        d26_rlen;   /* C-ISAM record length       */
	    boolean         d26_in_use; /* flag whether C-ISAM file   */
					/* is in use                  */
					/* time of last use of file   */
	    time_t          d26_last_use;
	} D26_fildes;                   /* C-ISAM FILE DESCRIPTION    */

typedef struct keydesc D26_keydesc; /* ISAM key description           */

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

			/* exported by d26utilis.c                    */

extern D26_fildes *d26_ofildes;     /* Description for object files   */
extern boolean    *d26_ofile_searched;/* Indicates which object files */
				    /* are searched already           */
extern D26_fildes *d26_afildes;     /* Description for attribute files*/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

			/* exported by C-ISAM modules                 */
void stlong(long l, char *p);

			/* exported by d26rschis.c                    */

extern signed16 d26_mi9_read_scheme(char *sch_dir, signed32 *nosrt,
	D26_srt **srt, D26_nam_at **nam_at, signed16 **nam_pos,
	D26_ocl_arc **ocl_arc, signed32 *nooct, D26_oct **oct,
	signed16 **oct_idx, signed32 *noat, D26_at **at, signed16 **at_pos,
	byte **obid_str, signed32 *nofile, D26_fildes **ofildes,
	signed32 *oreclen, char **orec, signed32 *nafile,
	D26_fildes **afildes, signed32 *areclen, char **arec, byte *cntr_bl,
	D2_obj_id *soundex_obid);

			/* exported by d261utilis.c                   */

extern char *d26_i15_get_record_pos(char *record, signed16 val_no,
	D26_at *p_at, D26_oct *p_oct, D26_nam_at *p_nam_at, boolean pos_no);
extern signed16 d26_i16_insert_val(D26_at *at, unsigned16 rep, char *val,
	signed16 length, D26_dns *acl_val[], D26_dns *mk_val[], char **pos);
extern signed16 d26_i17_del_all_rec_vals(D26_at *p_at_entry, signed32  key,
	signed16 obj_filenr, int isfd, char *record);
extern signed16 d26_i18_read_rec_val(signed16 len, char *val, signed32 key,
	signed16 obj_filenr, D26_at *p_at_entry, boolean no_old_val,
	boolean lock, int mode, int isfd, signed16 rule, char *record);
extern signed16 d26_i19_del_rec_val(signed16 rec_found, D26_at *p_at_entry,
	int isfd, char *val_pos);
extern signed16 d26_i20_append_dnlidx(D26_at *p_at_entry, char *pos,
	boolean *upd_dnl_file, signed32 **del_dns, signed16 *no_del_dns,
	signed16 *no_mem_del_dns, signed32 *dnl_idx, D23_rserror *error,
	signed32 *l_err);
extern int      d26_i23_open_file(signed16 file_nr, signed16 oa_flag);
extern signed16 d26_i38_cmp_equal(signed16 len, char *value,
	signed16 ofilenr, char *record, D2_name_string dname, signed16 *pos,
	D26_at *at);
extern signed32 d26_i59_get_valmemsize(D26_at *p_at);

			/* exported by d26srk.c                       */

extern signed16 d26_i55_sel_rec_key(D26_keydesc *key, D26_at *at,
	D2_a_match rule, signed16 op_id);

			/* exported by retutilis.c                    */

extern void d26_i32_select_key(D26_keydesc *key, signed16 pos, D26_at *at,
	signed16 av_nr, D2_a_match rule, signed16 filenr);
extern signed16 d26_i42_cmp_phon(signed16 len, byte *value, char *record,
	signed16 *pos, D26_at *at);
extern signed16 d26_i63_check_name(D26_dninfo *dninfo, signed16 np_count,
	char *record);

			/* exported by d26spatis.c                    */

extern signed16 d26_is11_ld_search_guide(char *search_guide, char *record);
extern signed16 d26_is12_ld_ttx_term_id(char *ttx_id, char *record,
	signed16 len);
extern signed16 d26_is13_ld_fax_number(char *fax_number, char *record,
	signed16 len);
extern signed16 d26_is14_ld_psap_addr(char *psap_addr, char *record,
	signed16 len);

extern signed16 d26_is21_st_search_guide(char *search_guide, char *record);
extern signed16 d26_is22_st_ttx_term_id(char *ttx_id, char *record,
	signed16 len);
extern signed16 d26_is23_st_fax_number(char *fax_number, char *record,
	signed16 len);
extern signed16 d26_is24_st_psap_addr(char *psap_addr, char *record,
	signed16 len);

			/* exported by d26mhsis.c                     */

extern signed16 d26_im11_ld_or_address(char *or_address, char *record,
	signed16 len);
extern signed16 d26_im12_ld_or_name(char *or_name, char *record,
	signed16 len);
extern signed16 d26_im13_ld_dl_sub_perm(char *dl_sub_perm, char *record,
	signed16 len);
extern signed16 d26_im21_st_or_address(char *or_address, char *record,
	signed16 len);
extern signed16 d26_im22_st_or_name(char *or_name, char *record,
	signed16 len);
extern signed16 d26_im23_st_dl_sub_perm(char *dl_sub_perm, char *record,
	signed16 len);

			/* exported by d26secis.c                     */

extern void d26_is31_ld_certificate(char **certificate, char **record);
extern void d26_is32_ld_cert_pair(char **cert_pair, char **record);
extern void d26_is33_ld_cert_list(char **cert_list, char **record);
extern void d26_is51_st_certificate(char **certificate, char **record);
extern void d26_is52_st_cert_pair(char **cert_pair, char **record);
extern void d26_is53_st_cert_list(char **cert_list, char **record);

#endif /* _D26ISAM_H */
