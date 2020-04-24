/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: audit.h,v $
 * Revision 1.1.10.1  1996/07/08  18:26:36  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:31  marty  1.1.8.3]
 *
 * Revision 1.1.8.3  1996/02/18  22:57:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:24  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  17:39:40  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/11/01  15:34 UTC  psn
 * 	Initial Mothra branch
 * 	[1995/12/08  17:17:48  root]
 * 
 * Revision 1.1.4.1  1994/10/13  19:36:32  blazin
 * 	For CR 12493, added dce_aud_set_local_cell_uuid, a new api definition which
 * 	will be called when secd starts.
 * 	[1994/10/13  19:36:13  blazin]
 * 
 * Revision 1.1.2.6  1994/09/22  21:09:00  weisz
 * 	CR 10703: handling of trail full situation.
 * 	[1994/09/22  21:07:24  weisz]
 * 
 * Revision 1.1.2.5  1994/08/30  17:21:33  weisz
 * 	CR 11884: added new API - dce_aud_start_with_uuid
 * 	[1994/08/30  17:17:43  weisz]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:30  luan
 * 	CR 9775 9776: Changed DCE_AUD_DEBUG definition.
 * 	[1994/03/11  21:59:18  luan]
 * 
 * Revision 1.1.2.3  1994/03/02  16:06:15  luan
 * 	CR 10038: Added delegation information in audit record header.
 * 	[1994/03/02  16:03:55  luan]
 * 
 * Revision 1.1.2.2  1994/02/22  16:47:26  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:39:15  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:02  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:16  zeliff]
 * 
 * $EndLog$
 */

#ifndef _AUDIT_H
#define _AUDIT_H

#ifdef DCE_DEBUG 
#define DCE_AUD_DEBUG
#endif /* DCE_DEBUG */

#include <dce/rpc.h>
#include <dce/utctypes.h>
#include <dce/audit_control.h>
#include <dce/audit_log.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _dce_aud_rec_handle   * dce_aud_rec_t;
typedef struct _dce_aud_trail_handle * dce_aud_trail_t;

typedef struct {
    unsigned16 format;
    union {
	idl_small_int small_int;
	idl_short_int short_int;
	idl_long_int long_int;
	idl_hyper_int hyper_int;
	idl_usmall_int usmall_int;
	idl_ushort_int ushort_int;
	idl_ulong_int ulong_int;
	idl_uhyper_int uhyper_int;
	idl_short_float short_float;
	idl_long_float long_float;
	idl_boolean boolean;
	uuid_t uuid;
	utc_t utc;
	sec_acl_t * acl;
	idl_byte * byte_string;
	idl_char * char_string;
    } data;
} dce_aud_ev_info_t;

#define aud_s_ok                       0x0

/* dce_aud_start, dce_aud_print options */
#define	aud_c_evt_all_info	       0x0000000F
#define	aud_c_evt_groups_info	       0x00000001
#define	aud_c_evt_address_info	       0x00000002
#define	aud_c_evt_delegates_info       0x00000004

/* dce_aud_print options */
#define	aud_c_evt_specific_info        0x00000008
#define aud_c_evt_raw_info             0x00000100

/* dce_aud_commit options */
#define	aud_c_evt_always_log           0x00010000
#define aud_c_evt_always_alarm         0x00020000
#define aud_c_evt_commit_sync          0x00000002
#define aud_c_evt_commit_sync_no_wait  0x00000004

/* flags of dce_aud_open() call */
#define aud_c_trl_open_read            0x00000001
#define aud_c_trl_open_write           0x00000002
#define aud_c_trl_open_write_no_filter 0x00000004

/* bits defining state strategies */
#define aud_c_trl_ss_wrap              0x00000100 
#define aud_c_trl_ss_save              0x00000000 

/* bits defining storage strategies */
#define aud_c_trl_ss_bits              0x00000100 


extern void
dce_aud_start(
	      unsigned32,
	      rpc_binding_handle_t,
	      unsigned32,
	      unsigned32,
	      dce_aud_rec_t *,
	      error_status_t *
);

extern void
dce_aud_start_with_server_binding(
	      unsigned32,
	      rpc_binding_handle_t,
	      unsigned32,
	      unsigned32,
	      dce_aud_rec_t *,
	      error_status_t *
);

extern void
dce_aud_start_with_pac(
		   unsigned32,
		   sec_id_pac_t *,
		   unsigned_char_t *,
		   unsigned32,
		   unsigned32,
		   dce_aud_rec_t *,
		   unsigned32 *
);

extern void
dce_aud_start_with_name(
		   unsigned32,
		   unsigned_char_t *,
		   unsigned_char_t *,
		   unsigned32,
		   unsigned32,
		   dce_aud_rec_t *,
		   unsigned32 *
);

extern void
dce_aud_start_with_uuid(
                   unsigned32 ,
                   uuid_t     ,
                   uuid_t     ,
                   uuid_t     ,
                   unsigned_char_t * ,
                   unsigned32 ,
                   unsigned32 ,
                   dce_aud_rec_t * ,
                   unsigned32 * 

);

extern void
dce_aud_put_ev_info(
		   dce_aud_rec_t,
		   dce_aud_ev_info_t,
		   error_status_t *
);

extern void
dce_aud_next(
	     dce_aud_trail_t,
	     char *,
	     unsigned16,
	     dce_aud_rec_t *,
	     unsigned32 *
);

extern void
dce_aud_prev(
	     dce_aud_trail_t,
	     char *,
	     unsigned16,
	     dce_aud_rec_t *,
	     unsigned32 *
);

extern void
dce_aud_get_header(
		   dce_aud_rec_t,
		   dce_aud_hdr_t **,
		   unsigned32 *
);

extern void
dce_aud_free_header(
		   dce_aud_hdr_t *,
		   unsigned32 *
);

extern void
dce_aud_get_ev_info(
		   dce_aud_rec_t,
		   dce_aud_ev_info_t **,
		   unsigned32 *
);

extern void
dce_aud_free_ev_info(
		   dce_aud_ev_info_t *,
		   unsigned32 *
);

extern unsigned32
dce_aud_length(
	       dce_aud_rec_t,
	       unsigned32 *
);

extern void
dce_aud_commit(
	       dce_aud_trail_t,
	       dce_aud_rec_t,
	       unsigned32,
	       unsigned16,
	       unsigned32,
	       unsigned32 *
);

extern void
dce_aud_discard(
	       dce_aud_rec_t,
               unsigned32 *
);

extern void
dce_aud_print(
	      dce_aud_rec_t,
	      unsigned32,
	      unsigned_char_t **,
	      unsigned32 *
);

extern void
dce_aud_open(
	     unsigned32,
	     char *,
	     unsigned32,
	     unsigned32,
	     dce_aud_trail_t *,
	     unsigned32 *
);

extern void
dce_aud_set_trail_size_limit(
	      dce_aud_trail_t,
	      unsigned32,
	      unsigned32 *
);

extern void
dce_aud_close(
	      dce_aud_trail_t,
	      unsigned32 *
);

extern void
dce_aud_reset(
	      dce_aud_trail_t *,
	      unsigned32 *
);

extern void
dce_aud_rewind(
	      dce_aud_trail_t,
	      unsigned32 *
);

extern void
dce_aud_modify_sstrategy(
              dce_aud_trail_t,
              unsigned32,
              error_status_t *
);

extern void
dce_aud_set_local_cell_uuid(
              uuid_t ,
              error_status_t *
);

#ifdef __cplusplus
}
#endif


#endif /* _AUDIT_H */
