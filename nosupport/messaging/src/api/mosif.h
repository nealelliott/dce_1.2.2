/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  m o s i f . h
 *
 *  Common interfaces to support the Message Oriented Service Application 
 *  Programming Interface(MOS-API).
 *
 * ______________________________________________________________________
 *
 * $Log: mosif.h,v $
 * Revision 1.12  1996/12/02  16:16:24  salamone
 * Add mos_rsrc_name_get() API
 *
 * Revision 1.11  96/11/20  11:40:38  11:40:38  salamone (Julie Salamone)
 * Provide support for callers to explicitly decode msgs
 * 
 * Revision 1.10  96/11/13  18:20:14  18:20:14  salamone (Julie Salamone)
 * Allow apps to use mos_datatype_ack
 * 
 * Revision 1.9  96/11/12  15:19:49  15:19:49  salamone (Julie Salamone)
 * Remove resource group APIs
 * 
 * Revision 1.8  96/11/12  10:40:54  10:40:54  salamone (Julie Salamone)
 * Removed cursor parameter in mos_que_mgmt_msgid_list()
 * 
 * Revision 1.7  96/11/08  14:58:38  14:58:38  salamone (Julie Salamone)
 * Modified conv fn status
 * 
 * Revision 1.6  96/11/07  18:03:40  18:03:40  salamone (Julie Salamone)
 * Add msg move and que move support
 * 
 * Revision 1.5  96/10/25  16:50:30  16:50:30  salamone (Julie Salamone)
 * Clean-up I/O params on _set() calls
 * 
 * Revision 1.4  96/10/25  14:41:49  14:41:49  salamone (Julie Salamone)
 * Add rsrc protectlvl support
 * 
 * Revision 1.3  96/10/23  16:12:06  16:12:06  salamone (Julie Salamone)
 * Add selection filter support
 * 
 * Revision 1.2  96/10/15  01:33:50  01:33:50  bowe (John Bowe)
 * Use proper include for mostypes.h
 * 
 * Revision 1.1  1996/10/14  17:04:06  salamone
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */
#ifndef _MOSIF_H
#define _MOSIF_H

/*
 * INCLUDE FILES
 */
#include <dce/lbase.h>
#include <dce/utctypes.h>
#include <dce/id_epac.h>
#include <dce/rgynbase.h>
#include <dce/rpcbase.h>
#include <dce/idl_es.h>
#include <mostypes.h> 


/*
 * DECLARATIONS
 */

/* ______________________________________________________________________
 *  Types for Naming to support the Message Oriented Service 
 *  Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

/*
 * Protection level for Message Queuing Model
 */
#define mos_c_protect_level_default     rpc_c_protect_level_default
#define mos_c_protect_level_none        rpc_c_protect_level_none  
                                        /* no authentication performed*/
#define mos_c_protect_level_auth        rpc_c_protect_level_call  
                                        /* on first pkt of each call */
#define mos_c_protect_level_integ       rpc_c_protect_level_pkt_integ  
                                        /* strong integrity chk */
#define mos_c_protect_level_privacy     rpc_c_protect_level_pkt_privacy  
                                        /* encrypt arguments */


/*
 * Additional Protection levels for Message Passing Model
 */
#define mos_c_protect_level_connect     rpc_c_protect_level_connect
                                        /* only on "connect" */
#define mos_c_protect_level_pkt         rpc_c_protect_level_pkt
                                        /* on each packet */

/*
 * Authentication services for Message Passing Model
 *
 * The various authentication schemes/protocols available to apps.
 */
#define mos_c_authn_none                rpc_c_authn_none  
                                        /* no authentication */
#define mos_c_authn_dce_secret          rpc_c_authn_dce_secret 
                                        /* OSF DCE shared secret key auth */
#define mos_c_authn_dce_public          rpc_c_authn_dce_public 
                                        /* OSF DCE public key auth (reserved) */
#define mos_c_authn_dce_dummy           rpc_c_authn_dce_dummy  
                                        /* OSF DCE non-crypto auth */
#define mos_c_authn_dssa_public         rpc_c_authn_dssa_public 
                                        /* DSSA public key auth (reserved) */
#define mos_c_authn_default             rpc_c_authn_default  
                                        /* default for environment */

/*
 * Authorization services for Message Passing Model
 *
 * The various authorization schemes/protocol available to applications.
 * The application's choice of authentication service determines the
 * set of authorization services the application is allowed to use.
 */
#define mos_c_authz_none                rpc_c_authz_none 
#define mos_c_authz_name                rpc_c_authz_name 
#define mos_c_authz_dce                 rpc_c_authz_dce 


/* ______________________________________________________________________
 *  Type-safe Opaque Pointers to support the Message Oriented Service 
 *  Application Programming Interface(MOS-API).
 *  At this time, idl [local] has a limitiation that you can not use 
 *  type-safe opaque pointers.
 * ______________________________________________________________________
 */

    /*
     *  Resource name service handle
     */
    typedef struct mos_rsrc_info_s_t *           mos_handle_t;

    /*
     *  Opaque pointer to message attribute structure
     *  obtained via mos_msg_attr_alloc() or mos_msg_attr_peek()
     */
    typedef struct mos_mattr_s_t *               mos_msg_attr_t;

    /*
     *  Opaque pointer to message buffer structure 
     *  obtained via mos_msg_dequeue() or mos_msg_peek().
     */
    typedef struct mos_datatype_bytearray_s_t *  mos_msg_buf_t;

    /*
     *  Opaque pointer to selection filter structure
     *  obtained via mos_msg_selfilter_alloc().
     */
    typedef struct mos_mask_list_s_t *           mos_msg_selfilter_t;

    /*
     *  Opaque pointer to a mask value
     */
    typedef void *                               mos_msg_selmask_value_t;

    /*
     *  Opaque pointer to queue attribute structure
     *  obtained via mos_que_attr_alloc() or mos_que_attr_peek()
     */
    typedef struct mos_qattr_s_t *               mos_que_attr_t;



/*
 * PROTOTYPES
 */

/* ______________________________________________________________________
 *  Client interfaces for API control to support 
 *  the Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     *  m o s _ d o n e
     *
     *  Releases all resources used by a MOS application.
     */
    void
    mos_done(
        /* [out] */        error_status_t *       status
    );

/* ______________________________________________________________________
 *  Client interfaces for Naming to support 
 *  the Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     *  m o s _ r s r c _ n a m e _ g e t
     *
     *  Returns the queue manager name, its security group name, its
     *  principal name, and if applicable the queue name itself for the
     *  specified resource handle.
     */
    void
    mos_rsrc_name_get(
        /* [in] */        mos_handle_t            resource_handle,
        /* [out] */       sec_rgy_name_t          security_name,
        /* [out] */       sec_rgy_name_t          quemgr_prin_name,
        /* [out] */       mos_rsrc_name_t         quemgr_name,
        /* [out] */       mos_rsrc_name_t         que_name,
        /* [out] */       error_status_t *        status
    );

    /*
     *  m o s _ r s r c _ d f l t _ q u e m g r _ g e t
     *
     *  Returns the default queue manager used by a MOS application.
     */
    void
    mos_rsrc_dflt_quemgr_get(
        /* [in] */        sec_rgy_name_t          security_name,
        /* [out] */       mos_rsrc_name_t         name,
        /* [out] */       mos_handle_t *          resource_handle,
        /* [out] */       error_status_t *        status
    );

    /*
     *  m o s _ r s r c _ d f l t _ q u e m g r _ s e t
     *
     *  Sets the default queue manager used by a MOS application.
     */
    void
    mos_rsrc_dflt_quemgr_set(
        /* [in] */        mos_rsrc_name_t         name,
        /* [in] */        sec_rgy_name_t          security_name,
        /* [out] */       mos_handle_t *          resource_handle,
        /* [out] */       error_status_t *        status
    );

    /*
     *  m o s _ r s r c _ a t t a c h
     *
     *  Finds one existing resource by name in the namespace.
     *  A resource is a name in the namespace that may be a queue, a
     *  Q-mgr, a destination peer application, a specific NSI entry, 
     *  group, or profile name.
     */
    void
    mos_rsrc_attach(
        /* [in] */        mos_rsrc_name_t         name,
        /* [in] */        sec_rgy_name_t          security_name,
        /* [out] */       mos_handle_t *          resource_handle,
        /* [out] */       error_status_t *        status
    );
    
    /*
     *  m o s _ r s r c _ d e t a c h
     *
     *  Releases one resource and any allocated memory within MOS 
     *  associated with the named resource.
     *  It is the responsibility of the caller to release any resources 
     *  obtained via mos_rsrc_attach(), mos_rsrc_dflt_quemgr_get(),
     *  mos_rsrc_dflt_quemgr_set(), mos_que_mgmt_create(), or
     *  mos_msg_attr_notice_set().
     */
    void
    mos_rsrc_detach(
        /* [in, out] */   mos_handle_t *          resource_handle,
        /* [out] */       error_status_t *        status
    );

    /*
     *  m o s _ r s r c _ p r o t e c t l v l _ g e t
     *
     *  Returns the protection level for communications made 
     *  to a resource. 
     *  This is not to be confused with end-to-end protection levels.
     */
    void
    mos_rsrc_protectlvl_get(
        /* [in] */        mos_handle_t            resource_handle,
        /* [out] */       unsigned32 *            protect_level,
        /* [out] */       error_status_t *        status
    );

    /*
     *  m o s _ r s r c _ p r o t e c t l v l _ s e t
     *
     *  Sets and registers the protection level for communications made 
     *  to a resource. 
     *  This is not to be confused with end-to-end protection levels.
     */
    void
    mos_rsrc_protectlvl_set(
        /* [in] */        unsigned32              protect_level,
        /* [in/out] */    mos_handle_t            resource_handle,
        /* [out] */       error_status_t *        status
    );

/* ______________________________________________________________________
 *  Client interfaces for Conversation Management to support the Message 
 *  Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     *  m o s _ m s g _ e n q u e u e
     *
     *  Enqueue one message to a queue.
     */
    void
    mos_msg_enqueue(
        /* [in] */        mos_handle_t             resource_handle,
        /* [in] */        unsigned32               flags,
        /* [in] */        mos_msg_attr_t           msg_attributes,
        /* [in] */        void *                   msg_buffer,
        /* [out] */       uuid_t *                 msg_id,
        /* [out] */       error_status_t *         status
    );

    /*
     *  m o s _ m s g _ d e q u e u e
     *
     *  Dequeue one message from a queue.
     */
    void
    mos_msg_dequeue(
        /* [in] */     mos_handle_t                resource_handle,
        /* [in] */     unsigned32                  flags,
        /* [in] */     mos_msg_selfilter_t         msg_selection_filter,
        /* [out] */    mos_msg_attr_t *            msg_attributes,
        /* [out] */    mos_msg_buf_t *             msg_buffer,
        /* [out] */    error_status_t *            status
    );

    /*
     *  m o s _ m s g _ p e e k
     *
     *  Returns a copy of one message from a queue.
     */
    void
    mos_msg_peek(
        /* [in] */     mos_handle_t                resource_handle,
        /* [in] */     unsigned32                  flags,
        /* [in] */     mos_msg_selfilter_t         msg_selection_filter,
        /* [out] */    mos_msg_attr_t *            msg_attributes,
        /* [out] */    mos_msg_buf_t *             msg_buffer,
        /* [out] */    error_status_t *            status
    );

    /*
     *  m o s _ m s g _ d e l e t e
     *
     *  Deletes one message by message ID from a queue.
     */
    void
    mos_msg_delete(
        /* [in] */        mos_handle_t             resource_handle,
        /* [in] */        uuid_t *                 msg_id,
        /* [out] */       error_status_t *         status
    );

    /*
     *  m o s _ m s g _ m o v e
     *
     *  Move one message by message ID from one queue to another.
     */
    void
    mos_msg_move(
        /* [in] */        mos_handle_t             src_resource_handle,
        /* [in] */        mos_handle_t             dest_resource_handle,
        /* [in] */        uuid_t *                 msg_id,
        /* [out] */       error_status_t *         status
    ); 


/* ______________________________________________________________________
 *  Client interfaces for Message Attribute Utilities to support the 
 *  Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     *  m o s _ m s g _ a t t r _ a l l o c
     *
     *  Allocates memory within MOS and returns an opaque pointer to 
     *  a message attributes structure with defaults set.
     */
    void
    mos_msg_attr_alloc(
        /* [out] */        mos_msg_attr_t *         msg_attributes,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ p e e k
     *
     *  Returns an opaque pointer to the message attributes on an 
     *  existing message in the specified queue.
     *  Applies to Message Queuing model only.
     */
    void
    mos_msg_attr_peek(
        /* [in] */         mos_handle_t             resource_handle,
        /* [in] */         unsigned32               flags,
        /* [in] */         mos_msg_selfilter_t      msg_selection_filter,
        /* [out] */        mos_msg_attr_t *         msg_attributes,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ f r e e
     *
     *  Releases memory used for message attributes.
     *  It is the responsibility of the caller to release the memory 
     *  if allocated by mos_msg_attr_alloc() or mos_msg_attr_peek().
     */
    void
    mos_msg_attr_free(
        /* [in,out] */     mos_msg_attr_t *         msg_attributes,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ m s g i d _ g e t
     *
     *  Return message ID from a message attributes structure.
     */
    void
    mos_msg_attr_msgid_get(
        /* [in] */         mos_msg_attr_t            msg_attributes,
        /* [out] */        uuid_t *                  msg_id,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ m s g _ a t t r _ e n q t i m e _ g e t
     *
     *  Return message enqueue time stamp in absolute time from 
     *  a message attributes structure.
     */
    void
    mos_msg_attr_enqtime_get(
        /* [in] */         mos_msg_attr_t           msg_attributes,
        /* [out] */        utc_t *                  msg_enq_time,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ m s g t y p e _ g e t
     *
     *  Return message type from a message attributes structure.
     */
    void
    mos_msg_attr_msgtype_get(
        /* [in] */         mos_msg_attr_t           msg_attributes,
        /* [out] */        mos_msg_attr_msgtype_t * msg_type,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ m s g t y p e _ s e t
     *
     *  Set message type in a message attributes structure.
     */
    void
    mos_msg_attr_msgtype_set(
        /* [in] */         mos_msg_attr_msgtype_t  msg_type,
        /* [in,out] */     mos_msg_attr_t          msg_attributes,
        /* [out] */        error_status_t *        status
    );


    /*
     *  m o s _ m s g _ a t t r _ p r i o r i t y _ g e t
     *
     *  Return priority from a message attributes structure.
     */
    void
    mos_msg_attr_priority_get(
        /* [in] */         mos_msg_attr_t          msg_attributes,
        /* [out] */        unsigned32 *            priority,
        /* [out] */        error_status_t *        status
    );

    /*
     *  m o s _ m s g _ a t t r _ p r i o r i t y _ s e t
     *
     *  Set priority in a message attributes structure.
     */
    void
    mos_msg_attr_priority_set(
        /* [in] */         unsigned32              priority,
        /* [in,out] */     mos_msg_attr_t          msg_attributes,
        /* [out] */        error_status_t *        status
    );

    /*
     *  m o s _ m s g _ a t t r _ t t l _ g e t
     *
     *  Return message expiration(time-to-live) in absolute time from 
     *  a message attributes structure.
     */
    void
    mos_msg_attr_ttl_get(
        /* [in] */         mos_msg_attr_t           msg_attributes,
        /* [out] */        utc_t *                  ttl,
        /* [out] */        error_status_t *        status
    );

    /*
     *  m o s _ m s g _ a t t r _ t t l _ s e t
     *
     *  Set message expiration(time-to-live) in absolute time in a 
     *  message attributes structure.
     */
    void
    mos_msg_attr_ttl_set(
        /* [in] */         utc_t *                  ttl,
        /* [in,out] */     mos_msg_attr_t           msg_attributes,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ t t r _ g e t
     *
     *  Return time-to-receive in absolute time from a message 
     *  attributes structure.
     */
    void
    mos_msg_attr_ttr_get(
        /* [in] */         mos_msg_attr_t           msg_attributes,
        /* [out] */        utc_t *                  ttr,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ t t r _ s e t
     *
     *  Set time-to-receive in absolute time in a message attributes 
     *  structure.
     */
    void
    mos_msg_attr_ttr_set(
        /* [in] */         utc_t *                  ttr,
        /* [in,out] */     mos_msg_attr_t           msg_attributes,
        /* [out] */        error_status_t *         status
    );


    /*
     *  m o s _ m s g _ a t t r _ p e r s i s t e n c e _ g e t
     *
     *  Return persistence from a message attributes structure.
     */
    void
    mos_msg_attr_persistence_get(
        /* [in] */         mos_msg_attr_t                  msg_attributes,
        /* [out] */        mos_msg_attr_persistence_t *    persistence,
        /* [out] */        error_status_t *                status
    );

    /*
     *  m o s _ m s g _ a t t r _ p e r s i s t e n c e _ s e t
     *
     *  Set persistence in a message attributes structure.
     */
    void
    mos_msg_attr_persistence_set(
        /* [in] */         mos_msg_attr_persistence_t      persistence,
        /* [in,out] */     mos_msg_attr_t                  msg_attributes,
        /* [out] */        error_status_t *                status
    );


    /*
     *  m o s _ m s g _ a t t r _ n o t i c e _ g e t
     *
     *  Return notice options from a message attributes structure.
     */
    void
    mos_msg_attr_notice_get(
        /* [in] */         mos_msg_attr_t           msg_attributes,
        /* [out] */        unsigned32 *             notice_flags,
        /* [out] */        mos_rsrc_name_t          notice_destination,
        /* [out] */        sec_rgy_name_t           notice_security_name,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ n o t i c e _ s e t
     *
     *  Set notice options in a message attributes structure
     *  and return a handle to the notice destination.
     */
    void
    mos_msg_attr_notice_set(
        /* [in] */         unsigned32               notice_flags,
        /* [in] */         mos_rsrc_name_t          notice_destination,
        /* [in] */         sec_rgy_name_t           notice_security_name,
        /* [in,out] */     mos_msg_attr_t           msg_attributes,
        /* [out] */        mos_handle_t *           resource_handle,
        /* [out] */        error_status_t *         status
    );

/* ______________________________________________________________________
 *  Client interfaces for Security to support 
 *  the Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     *  m o s _ m s g _ a t t r _ p r o t e c t l v l _ g e t 
     *
     *  Return the end-to-end protection level of initial sender 
     *  from the message attribute structure.
     *  Applies to Message Queuing model only.
     */ 
    void
    mos_msg_attr_protectlvl_get(
        /* [in] */         mos_msg_attr_t           msg_attributes,
        /* [out] */        unsigned32 *             protect_level,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ p r o t e c t l v l _ s e t 
     *
     *  Sets the end-to-end protection level of initial sender 
     *  from the message attribute structure.
     *  Applies to Message Queuing model only.
     */ 
    void
    mos_msg_attr_protectlvl_set(
        /* [in] */         unsigned32               protect_level,
        /* [in,out] */     mos_msg_attr_t           msg_attributes,
        /* [out] */        error_status_t *         status
    );
 
    /*
     *  m o s _ m s g _ a t t r _ s e c i d _ g e t
     *
     *  Return the security identity of initial sender of a message 
     *  from the message attribute structure.
     *  Applies to Message Queuing model only.
     */
    void
    mos_msg_attr_secid_get(
        /* [in] */         mos_msg_attr_t           msg_attributes,
        /* [out] */        sec_id_pa_t *            sec_id,
        /* [out] */        error_status_t *         status
    );


/* ______________________________________________________________________
 *  Client interfaces for Marshalling to support 
 *  the Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     * conv_fn:  Conversion function for data types
     */
    typedef void (*mos_msg_attr_convfn_t)(
        /* [in] */         idl_es_handle_t        handle,
        /* [in,out] */     void *                 data,
        /* [in,out] */     error_status_t *       status
    );

    /*
     *  Well-known ACK datatype UUID for mos_ack_convert().  
     *    e7daf4c6-bd70-11cf-87df-0800092474cc 
     *  Agrees with ../idl/mos_ack.idl.
     */
    extern uuid_t            mos_datatype_ack;  

    /*
     *  m o s _ m s g _ a t t r _ d a t a t y p e _ g e t 
     *
     *  Return application-specific data type from message 
     *  attributes structure.
     */
    void
    mos_msg_attr_datatype_get(
        /* [in] */         mos_msg_attr_t           msg_attributes,
        /* [out] */        uuid_t *                 datatype_id,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ m s g _ a t t r _ d a t a t y p e _ s e t 
     *
     *  Set application-specific data type from message 
     *  attributes structure.
     */
    void
    mos_msg_attr_datatype_set(
        /* [in] */         uuid_t *                 datatype_id,
        /* [in,out] */     mos_msg_attr_t           msg_attributes,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ a p i _ d a t a t y p e _ r e g i s t e r
     *
     *  Register one application-specific data type and its associated 
     *  conversion function with MOS to encode/decode the data.
     */
    void
    mos_api_datatype_register(
        /* [in] */         uuid_t *                      datatype_id,
        /* [in] */         mos_msg_attr_convfn_t         conv_fn,
        /* [out] */        error_status_t *              status
    );


    /*
     *  m o s _ a p i _ d a t a t y p e _ u n r e g i s t e r
     *
     *  Unregister one application-specific data type and its associated 
     *  conversion function with MOS.
     */
    void    
    mos_api_datatype_unregister(
        /* [in] */         uuid_t *                      datatype_id,
        /* [out] */        error_status_t *              status
    );

    /*
     *  m o s _ m s g _ d a t a t y p e _ d e c o d e
     *
     *  Decodes the message buffer returned by mos_msg_dequeue() 
     *  or mos_msg_peek().
     */
    void
    mos_msg_datatype_decode(
        /* [in] */         uuid_t *                      datatype_id,
        /* [in] */         mos_msg_buf_t                 msg_buffer,  
        /* [out] */        void *                        decoded_msg_buffer,  
        /* [out] */        error_status_t *              status
    );

    /*
     *  m o s _ m s g _ b u f _ f r e e
     *
     *  Releases memory used for the message buffer.
     *  It is the responsibility of the caller to release the memory 
     *  if allocated by mos_msg_dequeue() or mos_msg_peek().
     */
    void
    mos_msg_buf_free(
        /* [in,out] */     mos_msg_buf_t *               msg_buffer,  
        /* [out] */        error_status_t *              status
    );


/* ______________________________________________________________________
 *  Client interfaces for Message Selection Filter Utilities to support 
 *  the Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     *  m o s _ m s g _ s e l f i l t e r _ a l l o c
     *
     *  Allocates memory within MOS and returns an opaque pointer to a 
     *  selection filter structure.
     *  The selection filter is comprised of one or more masks that follow 
     *  the rules of the logical AND operator among all specified criteria 
     *  when retrieving messages.
     */
    void
    mos_msg_selfilter_alloc(
        /* [out] */         mos_msg_selfilter_t *   selection_filter,
        /* [out] */         error_status_t *        status
    );

    /*
     *  m o s _ m s g _ s e l f i l t e r _ f r e e
     *
     *  Releases memory used for the selection filter when retrieving messages.
     *  It is the responsibility of the caller to release the memory if 
     *  allocated by mos_msg_selfilter_alloc().
     */
    void
    mos_msg_selfilter_free(
        /* [in,out] */      mos_msg_selfilter_t *   selection_filter,
        /* [out] */         error_status_t *        status
    );

    /*
     *  m o s _ m s g _ s e l m a s k _ a d d
     *
     *  Adds one mask to the list of masks in the selection filter structure. 
     *  A mask is comprised of a selection criteria type, a selection
     *  criteria value, and the relational operator to be performed.
     */
    void
    mos_msg_selmask_add(
        /* [in] */         mos_msg_selmask_type_t   mask_type,
        /* [in] */         mos_msg_selmask_value_t  mask_value,
        /* [in] */         mos_msg_selmask_op_t     mask_op,
        /* [in,out] */     mos_msg_selfilter_t      selection_filter,
        /* [out] */        error_status_t *         status
    );


/* ______________________________________________________________________
 *  Client interfaces for Queue Attribute Utilities to support the Message 
 *  Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     *  m o s _ q u e _ a t t r _ a l l o c
     *
     *  Allocates memory within MOS and returns an opaque pointer to a 
     *  queue attributes structure with defaults set.
     */
    void
    mos_que_attr_alloc(
        /* [out] */        mos_que_attr_t *         que_attributes,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ q u e _ a t t r _ p e e k
     *
     *  Returns an opaque pointer to the queue attributes on an 
     *  existing queue.
     */
    void
    mos_que_attr_peek(
        /* [in] */         mos_handle_t             resource_handle,
        /* [out] */        mos_que_attr_t *         que_attributes,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ q u e _ a t t r _ f r e e
     *
     *  Releases memory used for queue attributes.
     *  It is the responsibility of the caller to release the memory 
     *  if allocated by mos_que_attr_alloc() or mos_que_attr_peek().
     */
    void
    mos_que_attr_free(
        /* [in,out] */     mos_que_attr_t *         que_attributes,
        /* [out] */        error_status_t *         status
    );


    /*
     *  m o s _ q u e _ a t t r _ c o m m i t
     *
     *  Commit queue attribute modifications on an existing queue.
     */
    void
    mos_que_attr_commit(
        /* [in] */         mos_handle_t             resource_handle,
        /* [in] */         mos_que_attr_t           queue_attributes,
        /* [out] */        error_status_t *         status
    );


    /*
     *  m o s _ q u e _ a t t r _ q u e i d _ g e t
     *
     *  Return queue ID from queue attributes structure.
     */
    void
    mos_que_attr_queid_get(
        /* [in] */         mos_que_attr_t           que_attributes,
        /* [out] */        uuid_t *                 que_id,
        /* [out] */        error_status_t *         status
    );


    /*
     *  m o s _ q u e _ a t t r _ q u e c u r s i z e  _ g e t
     *
     *  Return current queue size from queue attributes structure.
     */
    void
    mos_que_attr_quecursize_get(
        /* [in] */         mos_que_attr_t           que_attributes,
        /* [out] */        unsigned32 *             que_cur_size,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ q u e _ a t t r _ c r e a t e t i m e _ g e t
     *
     *  Return queue creation time stamp in absolute time from queue 
     *  attributes structure.
     */
    void
    mos_que_attr_createtime_get(
        /* [in] */         mos_que_attr_t           que_attributes,
        /* [out] */        utc_t *                  que_create_time,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ q u e _ a t t r _ a c t i v e t i m e _ g e t
     *
     *  Return queue last activity time stamp in absolute time from 
     *  queue attributes structure.
     */
    void
    mos_que_attr_activetime_get(
        /* [in] */         mos_que_attr_t           que_attributes,
        /* [out] */        utc_t *                  que_active_time,
        /* [out] */        error_status_t *         status
    );

    /*
     *  m o s _ q u e _ a t t r _ a l i a s _ l i s t 
     *
     *  Return the list of queue aliases by name from queue attributes 
     *  structure. The first entry in the list is the queue name itself.
     */
    void
    mos_que_attr_alias_list(
        /* [in] */         mos_que_attr_t            que_attributes,
        /* [in] */         unsigned32                space_avail,
        /* [out] */        unsigned32 *              num_returned,
        /* [out, size_is(space_avail), length_is(*num_returned)] */
                           mos_rsrc_name_t           que_alias_list[],
        /* [out] */        unsigned32 *              num_left,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ a l i a s _ a d d
     *
     *  Adds one queue alias to the list of queue aliases in the queue 
     *  attributes structure.
     */
    void
    mos_que_attr_alias_add(
        /* [in] */         mos_rsrc_name_t           link_name,
        /* [in] */         mos_rsrc_name_t           que_alias_name,
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ a l i a s _ r e m o v e
     *
     *  Removes one queue alias from the list of queue aliases in the 
     *  queue attributes structure.
     */
    void
    mos_que_attr_alias_remove(
        /* [in] */         mos_rsrc_name_t           link_name,
        /* [in] */         mos_rsrc_name_t           que_alias_name,
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ a l i a s _ r e s e t
     *
     *  Sets the queue alias list to NULL in the queue attributes structure.
     */
    void
    mos_que_attr_alias_reset(
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );


    /*
     *  m o s _ q u e _ a t t r _ a n n o t a t i o n _ g e t
     *
     *  Return queue annotation from queue attributes structure.
     */
    void
    mos_que_attr_annotation_get(
        /* [in] */        mos_que_attr_t             que_attributes,
        /* [out] */       mos_que_attr_annotation_t  annotation,
        /* [out] */       error_status_t *           status
    );

    /*
     *  m o s _ q u e _ a t t r _ a n n o t a t i o n _ s e t
     *
     *  Set queue annotation in queue attributes structure.
     */
    void
    mos_que_attr_annotation_set(
        /* [in] */         mos_que_attr_annotation_t annotation,
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ q u e m a x s i z e _ g e t
     *
     *  Return maximum queue size from queue attributes structure.
     */
    void
    mos_que_attr_quemaxsize_get(
        /* [in] */         mos_que_attr_t            que_attributes,
        /* [out] */        unsigned32 *              que_maxsize,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ q u e m a x s i z e _ s e t
     *
     *  Set maximum queue size in queue attributes structure.
     */
    void
    mos_que_attr_quemaxsize_set(
        /* [in] */         unsigned32                que_maxsize,
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ m s g m a x s i z e _ g e t
     *
     *  Return maximum message size on queue from queue attributes structure.
     */
    void
    mos_que_attr_msgmaxsize_get(
        /* [in] */         mos_que_attr_t            que_attributes,
        /* [out] */        unsigned32 *              msg_maxsize,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ m s g m a x s i z e _ s e t
     *
     *  Set maximum message size on queue in queue attributes structure.
     */
    void
    mos_que_attr_msgmaxsize_set(
        /* [in] */         unsigned32                msg_maxsize,
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ p e r s i s t e n c e _ g e t
     *
     *  Return queue persistence from queue attributes structure.
     */
    void
    mos_que_attr_persistence_get(
        /* [in] */         mos_que_attr_t                que_attributes,
        /* [out] */        mos_que_attr_persistence_t *  persistence,
        /* [out] */        error_status_t *              status
    );

    /*
     *  m o s _ q u e _ a t t r _ p e r s i s t e n c e _ s e t
     *
     *  Set queue persistence in queue attributes structure.
     */
    void
    mos_que_attr_persistence_set(
        /* [in] */         mos_que_attr_persistence_t    persistence,
        /* [in,out] */     mos_que_attr_t                que_attributes,
        /* [out] */        error_status_t *              status
    );

    /*
     *  m o s _ q u e _ a t t r _ i d l e t i m e o u t _ g e t
     *
     *  Return queue idle timeout in relative time from queue attributes 
     *  structure.
     */
    void
    mos_que_attr_idletimeout_get(
        /* [in] */         mos_que_attr_t            que_attributes,
        /* [out] */        utc_t *                   que_idle_timeout,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ i d l e t i m e o u t _ s e t
     *
     *  Set queue idle timeout in relative time in queue attributes 
     *  structure.
     */
    void
    mos_que_attr_idletimeout_set(
        /* [in] */         utc_t *                   que_idle_timeout,
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );


    /*
     *  m o s _ q u e _ a t t r _ a l l o w e n q _ g e t
     *
     *  Return boolean that describes whether enqueuing is allowed or 
     *  not from queue attributes structure.
     */
    void
    mos_que_attr_allowenq_get(
        /* [in] */         mos_que_attr_t            que_attributes,
        /* [out] */        boolean *                 allow_enq,
        /* [out] */        error_status_t *          status
    );


    /*
     *  m o s _ q u e _ a t t r _ a l l o w e n q _ s e t
     *
     *  Turn enqueuing on or off in queue attributes structure.
     */
    void
    mos_que_attr_allowenq_set(
        /* [in] */         boolean                   allow_enq,
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );


    /*
     *  m o s _ q u e _ a t t r _ a l l o w d e q _ g e t
     *
     *  Return boolean that describes whether dequeuing is allowed or 
     *  not from queue attributes structure.
     */
    void
    mos_que_attr_allowdeq_get(
        /* [in] */         mos_que_attr_t            que_attributes,
        /* [out] */        boolean *                 allow_deq,
        /* [out] */        error_status_t *          status
    );

    /*
     *  m o s _ q u e _ a t t r _ a l l o w d e q _ s e t
     *
     *  Turn dequeuing on or off in queue attributes structure.
     */
    void
    mos_que_attr_allowdeq_set(
        /* [in] */         boolean                   allow_deq,
        /* [in,out] */     mos_que_attr_t            que_attributes,
        /* [out] */        error_status_t *          status
    );



/* ______________________________________________________________________
 *  Client interfaces for Queue Management Utilities to support the 
 *  Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 */

    /*
     *  m o s _ q u e _ m g m t _ c r e a t e
     *
     *  Create one queue and commit its queue attributes to be managed 
     *  by one queue manager.
     */
    void
    mos_que_mgmt_create(
        /* [in] */        mos_handle_t          qmgr_handle,
        /* [in] */        mos_rsrc_name_t       name,
        /* [in] */        mos_que_attr_t        que_attributes,
        /* [out] */       mos_handle_t *        que_handle,
        /* [out] */       error_status_t *      status
    );

    /*
     *  m o s _ q u e _ m g m t _ d e l e t e
     *
     *  Delete one queue managed by the specified queue manager.
     */
    void
    mos_que_mgmt_delete(
        /* [in] */        mos_que_delete_op_t   flags,
        /* [in,out] */    mos_handle_t *        resource_handle,
        /* [out] */       error_status_t *      status
    );

    /*
     *  m o s _ q u e _ m g m t _ m o v e
     *
     *  Move or rename one queue within a Q-mgr or from one Q-mgr 
     *  to another.
     */
    void
    mos_que_mgmt_move(
        /* [in] */        mos_handle_t          dest_qmgr_handle,
        /* [in] */        mos_rsrc_name_t       new_que_name,
        /* [in] */        unsigned32            flags,
        /* [in/out] */    mos_handle_t *        move_que_handle,
        /* [out] */       error_status_t *      status
    );

    /*
     *  m o s _ q u e _ m g m t _ s a v e
     *
     *  Save all messages in the specified queue to persistent storage.
     */
    void
    mos_que_mgmt_save(
        /* [in] */        mos_handle_t          resource_handle,
        /* [out] */       error_status_t *      status
    );

    /*
     *  m o s _ q u e _ m g m t _ q u e c o u n t _ g e t
     *
     *  Return number of queues managed by specified Q-mgr.
     */
    void
    mos_que_mgmt_quecount_get(
        /* [in] */        mos_handle_t          resource_handle,
        /* [out] */       unsigned32 *          que_nbr,
        /* [out] */       error_status_t *      status
    );

    /*
     *  m o s _ q u e _ m g m t _ q u e n a m e _ l i s t 
     *
     *  Return list of queues by name managed by specified Q-mgr.
     */
    void
    mos_que_mgmt_quename_list(
        /* [in] */        mos_handle_t          resource_handle,
        /* [in] */        unsigned32            space_avail,
        /* [out] */       unsigned32 *          num_returned,
        /* [out, size_is(space_avail), length_is(*num_returned)] */
                          mos_rsrc_name_t       que_name_list[],
        /* [out] */       unsigned32 *          num_left,
        /* [out] */       error_status_t *      status
    );


    /*
     *  m o s _ q u e _ m g m t _ m s g i d _ l i s t 
     *
     *  Return list of messages by UUID from the specified queue that 
     *  satisfy the selection criteria.
     */
    void
    mos_que_mgmt_msgid_list(
        /* [in] */        mos_handle_t          resource_handle,
        /* [in] */        mos_msg_selfilter_t   msg_selection_filter,
        /* [in] */        unsigned32            space_avail,
        /* [out] */       unsigned32 *          num_returned,
        /* [out, size_is(space_avail), length_is(*num_returned)] */
                          uuid_t                msg_id_list[], 
        /* [out] */       unsigned32 *          num_left,
        /* [out] */       error_status_t *      status
    );

#endif   /* _MOSIF_H */
