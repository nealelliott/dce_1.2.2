/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: grouptest.h,v $
 * Revision 1.1.335.2  1996/02/18  23:06:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:30  marty]
 *
 * Revision 1.1.335.1  1995/12/11  20:04:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:15  root]
 * 
 * Revision 1.1.333.1  1994/01/21  22:40:20  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:16:15  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/25  18:04:25  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:36:02  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  22:14:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:06:05  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:35:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#ifndef GROUP_TEST_H
#define GROUP_TEST_H

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/idlbase.h>
#include <dce/lbase.h>
#include <dce/rpctypes.h>
#include <dce/dnsclerk.h>

#define rpc_c_annotation_max   18

/* typedef void *pointer_t; */


typedef struct
{
   uuid_t		uuid;
} rpc_nsuuid_t, *rpc_nsuuid_p_t;

typedef enum 
{ 
    rpc_e_group_member, 
    rpc_e_profile_element, 
    rpc_e_tower_member,
    rpc_e_uuid_member
} rpc_inqcntx_usage_t;


typedef struct {
    unsigned32              name_length;
    dns_opq_fname           *name;
} rpc_nsentry_t, *rpc_nsentry_p_t;

typedef dns_opq_sname rpc_nsattr_t;
typedef rpc_nsattr_t *rpc_nsattr_p_t;

#define rpc_c_entry_name_max (DNS_STR_FNAME_MAX + 1)

typedef struct
{
    rpc_nsentry_t           nsmember_name;
    unsigned_char_t         member_name[rpc_c_entry_name_max]; 
} rpc_inqcntx_group_t, *rpc_inqcntx_group_p_t;

typedef struct
{
    pointer_t               next;
    pointer_t               last;
} rpc_list_t, *rpc_list_p_t;

typedef struct {
    rpc_nsuuid_t	    if_uuid;
    unsigned16              if_vers_major;
    unsigned16              if_vers_minor;
    unsigned8               priority;
    unsigned8               count;
    unsigned_char_t         annotation[1];
} rpc_profile_elt_octet_str_t, *rpc_profile_elt_octet_str_p_t;

typedef struct {
    unsigned32              octet_str_length;
    rpc_profile_elt_octet_str_p_t  octet_str;
}  rpc_nsprofile_elt_t, *rpc_nsprofile_elt_p_t;

typedef struct
{
    rpc_nsuuid_t            nsobj_uuid;
    uuid_t                  obj_uuid;
} rpc_inqcntx_nsuuid_t, *rpc_inqcntx_nsuuid_p_t;

typedef struct
{
    rpc_if_id_t             if_id;
    unsigned8               priority;
    unsigned_char_t         annotation[rpc_c_annotation_max];
    unsigned_char_t         name[rpc_c_entry_name_max];
} rpc_profile_elt_t, *rpc_profile_elt_p_t;

typedef struct
{
    unsigned32              inquiry_type;
    rpc_if_id_t             if_id;
    unsigned32              if_vers_option;
    unsigned_char_t         member_name[rpc_c_entry_name_max];
    rpc_nsprofile_elt_t     nsprofile_elt;
    rpc_profile_elt_t       profile_elt;
} rpc_inqcntx_profile_t, *rpc_inqcntx_profile_p_t;

typedef struct
{
    twr_t           tower;
} rpc_inqcntx_tower_t, *rpc_inqcntx_tower_p_t;


typedef struct rpc_ns_inq_rep_s_t
{
    rpc_inqcntx_usage_t     usage;          /* inquiry usage */
    rpc_nsentry_t           *nsentry;       /* entry name in ns format */
    rpc_nsattr_t            *nsattr;        /* attribute name in ns format */
    unsigned32              member_name_syntax; /* ns syntax of member name */
    dns_handle              dns_context;    /* dns context handle */
    union
    {
        rpc_inqcntx_group_t     inqcntx_group;
        rpc_inqcntx_profile_t   inqcntx_profile;
        rpc_inqcntx_tower_t     inqcntx_tower;
        rpc_inqcntx_nsuuid_t    inqcntx_nsuuid;
    } inq_specific;
} rpc_ns_inq_rep_t, *rpc_ns_inq_rep_p_t;

#endif /* GROUP_TEST_H */
