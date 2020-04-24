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
 * $Log: gstep.h,v $
 * Revision 1.1.4.2  1996/03/11  13:23:56  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:45  root]
 * 
 * Revision 1.1.2.7  1994/09/07  13:33:57  bowe
 * 	Use #include <pthread.h>, not <dce/pthread.h> [CR 12068]
 * 	[1994/09/07  13:33:32  bowe]
 * 
 * Revision 1.1.2.6  1994/09/06  20:41:29  bowe
 * 	Rework args to gds_gst_print_data() [CR 12055]
 * 	[1994/09/06  20:40:51  bowe]
 * 
 * Revision 1.1.2.5  1994/09/06  16:30:19  keutel
 * 	OT 12055: dce_svc_log_get_arg will become obsolete
 * 	[1994/09/06  16:10:39  keutel]
 * 
 * Revision 1.1.2.4  1994/06/21  14:48:08  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:45:32  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:16:04  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:14  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:03:11  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:47:52  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:02  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:47:11  keutel
 * 	creation
 * 	[1994/03/21  15:55:21  keutel]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : gstep.h                                             */
/*                                                                    */
/* Description  : This Include-File contains function prototypes for  */
/*                the logfile evaluation tool gdssvcstep.             */
/*                                                                    */
/* Date         : 12/15/1993                                          */
/*                                                                    */
/* Author       : E. Kraemer, BU BA NM12                              */
/*                                                                    */
/**********************************************************************/

#ifndef _GSTEP_H
#define _GSTEP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gstep_h_rcsid[] = "@(#)$RCSfile: gstep.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:56 $";
#endif

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#ifndef _DCE_H
#include <dce/dce.h>
#endif
#include <dce/utc.h>

#include <dce/dcesvcmsg.h>
#include <dce/dcegdsmsg.h>
#include <pthread.h>
#include <dce/svclog.h>

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

			/* exported from gstapdu.c                    */

				    /* boolean values                 */
extern char *gst_true;
extern char *gst_false;

				    /* Filter tags                    */
extern char *gst_item;
extern char *gst_and;
extern char *gst_or;
extern char *gst_not;
extern char *gst_ill_filter;

				    /* Matching rules                 */
extern char *gst_equal;
extern char *gst_substrings;
extern char *gst_greater_or_equal;
extern char *gst_less_or_equal;
extern char *gst_present;
extern char *gst_approximate_match;

			/* exported from gstutil.c                    */

				    /* Basic representations          */
extern char *gst_dist_name_rep;
extern char *gst_obj_ident_rep;
extern char *gst_printable_rep;
extern char *gst_t61_rep;
extern char *gst_numeric_rep;
extern char *gst_ia5_rep;
extern char *gst_octet_rep;
extern char *gst_asn1_rep;
extern char *gst_boolean_rep;
extern char *gst_integer_rep;
extern char *gst_utctime_rep;
extern char *gst_s_guide_rep;
extern char *gst_telex_rep;
extern char *gst_ttxid_rep;
extern char *gst_fax_nr_rep;
extern char *gst_psap_rep;
extern char *gst_certificate;
extern char *gst_cert_pair;
extern char *gst_cert_list;
extern char *gst_mhs_ora_rep;
extern char *gst_mhs_orn_rep;
extern char *gst_mhs_dlsp_rep;
extern char *gst_acl_rep;
extern char *gst_ill_rep;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

			/* exported from gstep.c                      */

extern void gds_gst_print_binary_data(unsigned32 size, pointer_t data);

			/* exported from gstutil.c                    */

extern void gds_gst_print_op_id(const char *format, unsigned32 size,
	pointer_t data);
extern void gds_gst_print_entry(const char *format, unsigned32 size,
	pointer_t data);
extern void gds_gst_print_bo_filter(const char *format, signed16 call_nr,
	unsigned32 size, pointer_t data);
extern void gds_gst_print_filter_type(const char *format, signed16 call_nr,
	unsigned32 size, pointer_t data);
extern void gds_gst_print_name_filter(const char *format, unsigned32 size,
	pointer_t data);
extern void gds_gst_print_rv_int_search(const char *format, unsigned32 size,
	pointer_t data);
extern void gds_gst_print_rv_fa_check(const char *format, unsigned32 size,
	pointer_t data);
extern void gds_gst_print_rv_acl_check(const char *format, unsigned32 size,
	pointer_t data);
extern void gds_gst_print_type_and_rep(const char *format, signed16 call_nr,
	unsigned32 size, pointer_t data);
extern void gds_gst_print_type_and_pos(const char *format, signed16 call_nr,
	unsigned32 size, pointer_t data);
extern void gds_gst_print_int_and_name(const char *format, signed16 call_nr,
	unsigned32 size, pointer_t data);
extern void gds_gst_print_int_and_syntax(const char *format,
	signed16 call_nr, unsigned32 size, pointer_t data);
extern void gds_gst_print_rep_len_value(const char *format, signed16 call_nr,
	unsigned32 size, pointer_t data);
extern void gds_gst_print_dsa_configuration(const char *format,
	signed16 call_nr, unsigned32 size, pointer_t data);
extern void gds_dce_svc_log_get_arg( dce_svc_log_prolog_t prolog,
	unsigned32 *arg_index,
	pointer_t * data, unsigned32 * size, error_status_t * status);

			/* exported from gstapdu.c                    */

extern void gds_gst_print_apdu_request(const char *format, signed16 parcount,
	signed32 size, pointer_t data);
extern void gds_gst_print_apdu_result(const char *format, signed16 parcount,
	signed32 size, pointer_t data);
extern void gds_gst_print_apdu_error(signed32 size, pointer_t data);

extern void gds_gst_print_dn_struct(signed32 *len, byte **value);
extern void gds_gst_print_tm_info(signed32 *len, byte **value);
extern void gds_gst_print_bit_struct(signed32 *len, byte **value);
extern void gds_gst_print_obj_ident(signed16 len, byte *value);
extern void gds_gst_print_dist_name(signed16 len, byte *value);
extern void gds_gst_print_bit_string(signed32 len, byte *value);

extern void gds_gst_print_options_serv_control(signed32);
extern void gds_gst_print_time_limit(signed16);
extern void gds_gst_print_size_limit(signed16);

			/* exported from gstisam.c                    */

extern void gds_gst_print_keydesc(const char *format, signed16 call_nr,
	unsigned32 size, pointer_t data);

			/* exported from gstaval.c                    */

extern void gds_gst_print_attr_value(byte **message);
extern void gds_gst_print_filter_substr(byte **message);
extern void gds_gst_print_representation(unsigned16 rep);

			/* exported from gstasn1.c                    */

extern void gds_gst_print_asn1_encoded_msg( signed32 size, pointer_t data);

#endif

