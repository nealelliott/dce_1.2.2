/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  m o s _ a p i . h
 *
 *  Structures and definitions to support the
 *  Message Oriented Service Application Programming Interface(MOS-API).
 *
 * ______________________________________________________________________
 *
 * $Log: mos_api.h,v $
 * Revision 1.5  1996/11/07  18:03:02  salamone
 * Add msg move and que move support
 *
 * Revision 1.4  96/10/25  14:42:25  14:42:25  salamone (Julie Salamone)
 * Add rsrc protectlvl support
 * 
 * Revision 1.3  96/10/23  16:13:33  16:13:33  salamone (Julie Salamone)
 * Add selection filter support
 * 
 * Revision 1.2  96/10/15  16:15:36  16:15:36  salamone (Julie Salamone)
 * Added mos_msg_peek()/mos_msg_delete() support
 * 
 * Revision 1.1  96/10/14  17:03:48  17:03:48  salamone (Julie Salamone)
 * Initial revision
 * 
 * $EndLog$
 * ______________________________________________________________________
 */
#ifndef _MOS_API_H
#define _MOS_API_H

#include <mos_proto.h>

/* 
 * DECLARATIONS 
 */
#define mos_c_dflt_name_syntax          rpc_c_ns_syntax_default

/*
 * This structure contains all the necessary information to
 * perform message-oriented operations to resources. 
 * Note that a resource may be a Q-mgr or a queue.
 * Note that a binding handle may be shared amongst several resources.
 * The protection level registered via mos_rsrc_protectlvl_set() is
 * used for all communications to a resource.
 */
typedef struct mos_rsrc_info_s_t {
    struct mos_rsrc_info_s_t *    next;
    boolean                       dflt_que_mgr;
    rpc_binding_handle_t          server_handle;    /* server binding handle */
    sec_rgy_name_t                server_prin_name; /* server principal name */
    unsigned32                    protectlvl;       /* protectlvl on binding */
    sec_rgy_name_t                security_name;    /* server security group */
    mos_rsrc_name_t               server_name;      /* server NSI name */
    mos_rsrc_name_t               resource_name;    /* queue name */
    uuid_t                        que_id;           /* queue ID */
} mos_rsrc_info_t;

extern mos_rsrc_info_t *rsrc_list_head;     /* head item in resource list */
extern mos_rsrc_info_t *rsrc_list_tail;     /* tail item in resource list */
extern mos_rsrc_info_t  null_rsrc_item;     /* NULL resource item */

/* 
 * This list contains all the registered application-specific
 * datatypes and their respective conversion functions.
 */
typedef struct mos_datatype_info_s_t {
    struct mos_datatype_info_s_t *    next;
    uuid_t                            datatype_id;
    mos_msg_attr_convfn_t             conv_fn;
} mos_datatype_info_t;

extern mos_datatype_info_t *datatype_list_head; /* head item in datatype list */
extern mos_datatype_info_t *datatype_list_tail; /* tail item in datatype list */
extern mos_datatype_info_t  null_datatype_item; /* NULL datatype item */

/*
 * INTERNAL PROTOTYPES
 */
void   mos__bind_to_server(sec_rgy_name_t, mos_rsrc_info_t *, error_status_t *);
void    mos__auth_info_set(mos_rsrc_info_t *, unsigned32, error_status_t *);
boolean mos__is_dflt_quemgr_found(mos_rsrc_info_t **);
boolean mos__is_rsrc_found(mos_rsrc_name_t,mos_rsrc_name_t,mos_rsrc_info_t **);
boolean mos__is_server_found(mos_rsrc_name_t, mos_rsrc_info_t **);
void    mos__rsrc_info_create(mos_rsrc_info_t **, error_status_t *);
boolean mos__is_rsrc_handle_valid(boolean, mos_rsrc_info_t *, error_status_t *);
void    mos__rsrc_info_append(mos_rsrc_info_t *);
void    mos__rsrc_info_remove(mos_rsrc_info_t *, error_status_t *);
void    mos__server_refcnt_get(mos_rsrc_name_t, unsigned32 *);
boolean mos__is_datatype_found(uuid_t *, mos_datatype_info_t **);
boolean mos__is_datatype_valid(boolean, uuid_t *, error_status_t *);
boolean mos__is_uuid_valid(uuid_t *, error_status_t *);
boolean mos__is_enqmode_valid(unsigned32, error_status_t *);
boolean mos__is_deqmode_valid(unsigned32, error_status_t *);
boolean mos__is_msgtype_valid(mos_msg_attr_msgtype_t, error_status_t *);
boolean mos__is_msgpersistence_valid(mos_msg_attr_persistence_t, error_status_t
*);
boolean mos__is_protectlvl_valid(unsigned32, error_status_t *);
void mos__msg_pep(mos_handle_t, mos_handle_t, mos_msg_selfilter_t, error_status_t *);

#endif   /* _MOS_API_H */
