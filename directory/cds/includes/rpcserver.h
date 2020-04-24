#ifndef _RPCSERVER_H
#define _RPCSERVER_H
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpcserver.h,v $
 * Revision 1.1.8.2  1996/02/18  23:34:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:18  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:17:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:39  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:02:37  mccann
 * 	includes cleanup
 * 	[1994/08/01  19:53:03  mccann]
 * 
 * Revision 1.1.6.4  1994/07/25  15:14:08  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  14:00:13  proulx]
 * 
 * Revision 1.1.6.3  1994/06/09  18:39:28  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:09  devsrc]
 * 
 * Revision 1.1.6.2  1994/06/08  20:17:59  peckham
 * 	Add cds_server_set_progname().
 * 	[1994/06/06  20:26:07  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:04:05  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:18:55  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:33:50  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:53:29  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:49:46  weisman
 * 		New file from Digital (post 1.0)
 * 	[1992/03/22  20:56:52  weisman]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *  NAME:
 *
 *      rpcserver.h
 *
 *  FACILITY:
 *
 *      DCE RPC
 *
 *  ABSTRACT:
 *
 *      Useful RPC server utility functions.
 *
 *
 */


#ifndef EXTERNAL
#define EXTERNAL extern
#endif

#ifndef GLOBAL
#define GLOBAL
#endif

#ifndef PUBLIC
#define PUBLIC
#endif

#ifndef PRIVATE
#define PRIVATE
#endif

#ifndef INTERNAL
#define INTERNAL static
#endif

#ifdef GLOBALDEFS
#define IMPEXP
#else
#define IMPEXP extern
#endif

#define rpc_c_server_mode_debug             1
#define rpc_c_server_mode_verify            2
#define rpc_c_server_mode_no_if_register    4
#define rpc_c_server_mode_no_ep_register    8

typedef struct
{
    rpc_if_handle_t     if_handle;
    rpc_mgr_epv_t       *if_epv;
    uuid_vector_p_t     object_ids;
    char                *annotation;
} rpc_if_descriptor_t, *rpc_if_descriptor_p_t;


typedef struct  {
   unsigned_char_p_t       program_name;
   unsigned_char_p_t       entry_name;
   rpc_if_handle_t         if_spec;
   rpc_binding_vector_p_t  new_binding_vector;
   uuid_p_t                object_uuid;
} cds_export_ns_info_t;

typedef cds_export_ns_info_t *cds_export_ns_info_p_t;

PRIVATE void 
export_ns_new (char *,
	       char *,
	       rpc_if_handle_t,
	       rpc_binding_vector_p_t,
	       uuid_p_t,
	       unsigned int *);

PRIVATE int 
cds_server_init (
        char                    *,
        char                    *,
        unsigned int            ,
        rpc_if_descriptor_p_t   ,
        unsigned int            ,
	rpc_binding_vector_p_t  *,
        error_status_t          *);



PRIVATE void
cds_export_replace (cds_export_ns_info_p_t);

PRIVATE int 
cds_server_detach (
        char                    *,
        char                    *,
        error_status_t          *);


PRIVATE int 
cds_server_fork (	
		 char           *,
		 unsigned int    ,
		 error_status_t *);

PRIVATE void
cds_server_set_progname(
	char			*);

#endif  /* #ifndef _RPCSERVER_H */
