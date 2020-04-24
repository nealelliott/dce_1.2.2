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
 * $Log: attrimpl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:50  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  20:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:12 UTC  tatsu_s
 * 	Added dce_attr_sch_bind_local().
 * 	[1995/12/08  18:07:40  root]
 * 
 * Revision 1.1.2.2  1994/09/21  21:49:36  pwang
 * 	Added dce_attr_c_schema_name[_len] [OT#12146]
 * 	[1994/09/21  21:43:23  pwang]
 * 
 * Revision 1.1.2.1  1994/07/22  21:39:40  pwang
 * 	Initial implementation
 * 	[1994/07/22  20:33:14  pwang]
 * 
 * $EndLog$
 */

#ifndef _DCE_ATTR_IMPL_H_INCLUDED_
#define	_DCE_ATTR_IMPL_H_INCLUDED_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>


#include <dce/dce.h>
#include <dce/rpc.h>
#include <dce/dce_utils.h>
#include <dce/sec_login.h>
#include <dce/dcelibmsg.h>

#include <dce/dce_attr_base.h>
#include <dce/rs_attr_schema.h>
#include <dce/dce_attr_sch.h>


/*
** d c e _ a t t r _ i f _ t y p e _ t
**
** The internal type for representing generic dce attr interfaces 
*/
typedef enum dce_attr_if_type_e_t {
	dce_attr_e_if_null,
        dce_attr_e_if_attr,
        dce_attr_e_if_attr_sch,
        dce_attr_e_if_attr_srch
} dce_attr_if_type_t;


/*
** d c e _ a t t r _ b i n d i n g _ h a n d l e _ r e p _ t 
**
** The internal data structure for representing binding handles 
**
** Note: This internal data structure is intended for generial use
**       in generic attr interfaces (i.e attr, attr_scheme, attr_srch,
**	 etc.). It can be changed subject to the future need. 
*/
typedef struct  dce_attr_binding_handle_rep_s_t {
	rpc_binding_handle_t		rpc_handle;
	dce_attr_component_name_t	component_name;
	dce_attr_if_type_t		dce_attr_if_type;
	boolean32			rpc_handle_valid;
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	unsigned32			if_version;
	union {
	    void			*epv;
	    rs_attr_schema_v0_0_epv_t	*v0_0_epv;
	}	lepv;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
} dce_attr_binding_handle_rep_t, *dce_attr_binding_handle_rep_p_t;


/*
** d c e _ a t t r _ c _ s c h e m a _ n a m e
**
** The default dce_attr [leading] schmea name: xattrschema 
** the legal schema name is xattrscheam[/...]   
*/
#define	dce_attr_c_schema_name		"xattrschema"
#define dce_attr_c_schema_name_len	(sizeof dce_attr_c_schema_name - 1)

/*
** m a c r o s
*/
#include <dce/macros.h>

/*
** Additional status related
*/
#define RESET_STATUS(stp)             CLEAR_STATUS(stp)
#define CHECK_STATUS_RET(stp)         if (BAD_STATUS(stp)) return
#define CHECK_STATUS_OUT(stp)         if (BAD_STATUS(stp)) goto out 
#define CHECK_STATUS_BRK(stp)         if (BAD_STATUS(stp)) break 

/*
** Dce_attr binding related
*/
#define RPC_BINDING(handle)             (handle)->rpc_handle
#define COMPONENT_NAME(handle)          (handle)->component_name
#define IF_TYPE(handle)             	(handle)->dce_attr_if_type
#define HANDLE_VALID(handle)            (handle)->rpc_handle_valid
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
#define	IS_LOCAL_HANDLE(handle)		((handle)->lepv.epv != NULL)
#define IF_VERSION(handle)		(handle)->if_version
#define	V0_0_EPV(handle)		(*(handle)->lepv.v0_0_epv)
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

#define GOOD_BINDING(handle, if_type)     				\
			(IF_TYPE(handle) == if_type && HANDLE_VALID(handle))
#define DCE_ATTR_BIND_VALIDATE(handle, if_type, stp)                    \
                        if (!GOOD_BINDING(handle, if_type)) {    	\
                           SET_STATUS(stp, dce_attr_s_bad_binding);	\
			   return;					\
			}
#define DCE_ATTR_BIND_VALIDATE_SCH(handle, stp)				\
		DCE_ATTR_BIND_VALIDATE(handle, dce_attr_e_if_attr_sch, stp)

/*
** Malloc related 
*/
#define MALLOC_AND_RESET(ptr, type, stp)  {				\
        		if (!(ptr = NEW(type))) {			\
                		SET_STATUS(stp, dce_attr_s_no_memory);	\
        		} else { 					\
                		memset((ptr), 0, sizeof(type)); 	\
        		}}


#endif /* _DCE_ATTR_IMPL_H_INCLUDED_ */
