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
 * $Log: dcedimpl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:46  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:22 UTC  tatsu_s
 * 	Added dced_c_binding_syntax_local.
 * 	[1995/12/08  17:59:10  root]
 * 
 * Revision 1.1.2.12  1994/08/24  19:26:58  bowe
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 	[1994/08/24  19:10:28  bowe]
 * 
 * Revision 1.1.2.11  1994/08/03  22:06:32  pwang
 * 	Added local keytab support [OT 11280] and removed
 * 	unused dced__if_id_compare().
 * 	[1994/08/03  22:03:57  pwang]
 * 
 * Revision 1.1.2.10  1994/07/28  14:03:42  rsalz
 * 	Use dce_strdup (OT CR 11343).
 * 	[1994/07/28  13:41:59  rsalz]
 * 
 * Revision 1.1.2.9  1994/07/26  22:07:11  pwang
 * 	Some adjustment.
 * 	[1994/07/26  22:06:45  pwang]
 * 
 * Revision 1.1.2.8  1994/07/06  20:17:13  pwang
 * 	Reorg dced free routines [OT#11003]
 * 	[1994/07/06  20:07:57  pwang]
 * 
 * Revision 1.1.2.7  1994/06/09  16:02:56  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:32  devsrc]
 * 
 * Revision 1.1.2.6  1994/05/31  21:08:52  pwang
 * 	Added #ifndef _DCED_LIB_IMPL_H and fixed some macros.
 * 	[1994/05/31  20:56:17  pwang]
 * 
 * Revision 1.1.2.5  1994/05/05  14:41:14  pwang
 * 	Include dced_util.h
 * 	[1994/05/05  14:36:00  pwang]
 * 
 * Revision 1.1.2.4  1994/04/15  22:29:08  pwang
 * 	Added macros and fixed a number of problems.
 * 	[1994/04/15  21:38:56  pwang]
 * 
 * Revision 1.1.2.3  1994/03/11  00:07:23  pwang
 * 	Replaced the sec_attr_t cleanup routine with
 * 	sec_attr_util_inst_free_ptrs() and added the
 * 	prototype for rca_pwd_gen_transmit_rep().
 * 	[1994/03/10  23:56:04  pwang]
 * 
 * Revision 1.1.2.2  1994/03/09  20:47:03  pwang
 * 	Moved dced service header files from dced.h to here.
 * 	[1994/03/09  20:46:47  pwang]
 * 
 * Revision 1.1.2.1  1994/02/28  16:26:53  pwang
 * 	Initial Implementation.
 * 	[1994/02/28  16:03:37  pwang]
 * 
 * $EndLog$
 */

#ifndef _DCED_LIB_IMPL_H
#define	_DCED_LIB_IMPL_H

/*
**--------- DCED API IMPLEMENTATION DECLARATIONS ------------
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#include <dce/dce.h>
#include <dce/dce_utils.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>
#include <dce/keymgmt.h>
#include <dce/sec_authn.h>
#include <dce/sec_attr_util.h>
#include <dce/dce_cf.h>
#include <dce/dced.h>
#include <dce/dced_util.h>

#include <hostdata.h>
#include <rkeytab.h>
#include <srvrconf.h>
#include <srvrexec.h>
#include <secval.h>
#include <dce/rs_attr_schema.h>

/*
** dced_cache_state_t defines the cache status.
*/
typedef enum dced_cache_state_e_t {
	dced_e_cache_state_null,
	dced_e_cache_state_valid,
	dced_e_cache_state_invalid
} dced_cache_state_t;

typedef struct dced_entry_cache_s_t {
	dced_cache_state_t	state;	
	dced_entry_list_t	data;
} dced_entry_cache_t;
 
/*
** dced_binding_handle_s_t defines a binding structure.
*/
struct dced_binding_handle_s_t {
	dced_service_type_t	service;	/* dced service */
	unsigned32		binding_syntax;	/* dced binding flag */
	rpc_binding_handle_t	binding;	/* rpc binding handle */
	dced_entry_cache_t	cache;		/* cached entry list */
};

/*
** dced_cursor_s_t defines the cursor structure to refer
** to dced cache list.
*/
struct dced_cursor_s_t	{
	dced_binding_handle_t	h;	  /* pointer to dced handle */
	unsigned32		position; /* current cursor position */
};


/*
** dced_keytab_cursor_s_t defines the keytable cursor structure
*/
struct dced_keytab_cursor_s_t {
	dced_binding_handle_t	h;
	unsigned32		position;
	dced_key_list_t		*keys;
};


/*
** dced_c_entry_syntax  defines the dced default entry syntax
*/
#define dced_c_entry_syntax	rpc_c_ns_syntax_dce

/*
** Local EPV
*/
#ifndef _DEF_LOCAL_EPV_
extern rkeytab_v1_0_epv_t       rkeytab_v1_0_l_epv;
#endif /* _DEF_LOCAL_EPV_ */

extern hostdata_v1_0_epv_t		*hostdata_v1_0_r_epv;
extern rkeytab_v1_0_epv_t		*rkeytab_v1_0_r_epv;
extern srvrconf_v1_0_epv_t		*srvrconf_v1_0_r_epv;
extern srvrexec_v1_0_epv_t		*srvrexec_v1_0_r_epv;

#ifndef macros__included
#define macros__included

#ifndef NULL
#   define NULL 0L
#endif

#ifndef FALSE
#   define FALSE 0
#endif
#ifndef TRUE
#   define TRUE !FALSE
#endif

/*
** string and list related macros
*/
#define NULL_STRING(s)		      ((s) == NULL || *(s) == '\0')
#define	EMPTY_LIST(listp)	      ((listp)->count == 0 || !((listp)->list))
#define	CHECK_EMPTY_LIST_RET(listp)   if (EMPTY_LIST(listp)) return 
#define	CHECK_EMPTY_LIST_BRK(listp)   if (EMPTY_LIST(listp)) break 

/*
** Status code related macros
*/
#define GOOD_STATUS(stp)	   ((*stp) == error_status_ok)
#define BAD_STATUS(stp)		   ((*stp) != error_status_ok)
#define CLEAR_STATUS(stp)	   (*stp) = error_status_ok
#define CLEAR_STATUS_2(stp1, stp2) (*stp1) = (*stp2) = error_status_ok

#define RESET_STATUS(stp)             (*stp) = error_status_ok
#define STATUS_EQUAL(stp, st_val)     ((*stp) == (st_val))
#define SET_STATUS(stp, val)          (*stp) = val
#define SET_STATUS_RET(stp, val)      (*stp) = val; return
#define CHECK_STATUS_RET(stp)	      if (BAD_STATUS(stp)) return
#define CHECK_STATUS_OUT(stp)	      if (BAD_STATUS(stp)) goto out 
#define CHECK_STATUS_BRK(stp)	      if (BAD_STATUS(stp)) break 

/*
** Dced binding related macros
*/
#define	RPC_BINDING(handle)	      	(handle)->binding
#define	DCED_SERVICE(handle)	      	(handle)->service
#define	DCED_SYNTAX(handle)	      	(handle)->binding_syntax
#define	CACHE_STATE(handle)	      	(handle)->cache.state
#define	CACHE_DATA(handle)		(handle)->cache.data
#define	CACHE_COUNT(handle)		(handle)->cache.data.count
#define CACHE_HEAD(handle)		(handle)->cache.data.list
#define CACHE_ENTRY(handle, i)		(handle)->cache.data.list[i]

#define	GOOD_BINDING(handle, serv)	((handle)->service == serv)
#define	SYNTAX_EQUAL(handle, syntax)	((handle)->binding_syntax == syntax)
#define CHECK_BINDING_SET_RET(handle, serv, stp)  			\
			if (!GOOD_BINDING(handle, serv)) { 		\
			    	SET_STATUS_RET(stp, dced_s_bad_binding);\
			}

#define VALID_CACHE(handle) ((handle)->cache.state == dced_e_cache_state_valid)
#define EMPTY_CACHE(handle) ((handle)->cache.state == dced_e_cache_state_null)
#define SET_VALID_CACHE(handle)						\
			(handle)->cache.state = dced_e_cache_state_valid
#define SET_INVALID_CACHE(handle) 					\
		if (VALID_CACHE(handle)) { 				\
		 (handle)->cache.state = dced_e_cache_state_invalid;	\
		}

/*
** Cursor macros
*/
#define CURSOR_PTR(cur)			(cur)->position
#define CURSOR_BINDING(cur)		(cur)->h
#define	SET_CURSOR(cur, handle, pos)	(cur)->h = handle; \
					(cur)->position = pos
#define	CURSOR_KEYS(cur)		(cur)->keys
#define	SET_KEYTAB_CURSOR(cur, handle, pos, key_list)	   \
	      (cur)->h = handle; (cur)->position = pos; (cur)->keys = key_list

/*
** Malloc macros
*/
#define MALLOC_RESET_RET(ptr, type, num, stp)  { \
	if (!(ptr = (type *)malloc(num*sizeof(type)))) { \
		SET_STATUS_RET(stp, dced_s_no_memory); \
	} else { \
		memset((ptr), 0, (num*sizeof(type))); \
	}}
#define MALLOC_RESET(ptr, type, num, stp)  { \
	if (!(ptr = (type *)malloc(num*sizeof(type)))) { \
		SET_STATUS(stp, dced_s_no_memory); \
	} else { \
		memset((ptr), 0, (num*sizeof(type))); \
	}}

/*
** rkeytab operations 
*/
#define Rkeytab_create(handle, ktentry, ifimport, ktkeys, st_p)	     	\
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) {   	\
	   (*rkeytab_v1_0_r_epv->rkeytab_create)			\
		(NULL, ktentry, ifimport, ktkeys, st_p);	     	\
	} else if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {    \
	   (*rkeytab_v1_0_l_epv.rkeytab_create)			     	\
		(NULL, ktentry, ifimport, ktkeys, st_p);	     	\
	} else {						     	\
	   (*rkeytab_v1_0_c_epv.rkeytab_create)			     	\
		(RPC_BINDING(handle), ktentry, ifimport, ktkeys, st_p); \
	}
#define Rkeytab_delete(handle, ktid, ifdelobj, st_p)		     \
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) {    \
	   (*rkeytab_v1_0_r_epv->rkeytab_delete)		     \
		(NULL, ktid, ifdelobj, st_p);			     \
	} else if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {    \
	   (*rkeytab_v1_0_l_epv.rkeytab_delete)			     \
		(NULL, ktid, ifdelobj, st_p);			     \
	} else {						     \
	   (*rkeytab_v1_0_c_epv.rkeytab_delete)			     \
		(RPC_BINDING(handle), ktid, ifdelobj, st_p);	     \
	}
#define Rkeytab_add_key(handle, ktid, ktkey, st_p)		     \
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) {    \
	   (*rkeytab_v1_0_r_epv->rkeytab_add_key)		     \
		(NULL, ktid, ktkey, st_p);			     \
	} else if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {    \
	   (*rkeytab_v1_0_l_epv.rkeytab_add_key)		     \
		(NULL, ktid, ktkey, st_p);			     \
	} else {						     \
	   (*rkeytab_v1_0_c_epv.rkeytab_add_key)		     \
		(RPC_BINDING(handle), ktid, ktkey, st_p);	     \
	}
#define Rkeytab_delete_key(handle, ktid, ktkey, st_p)		     \
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) {    \
	   (*rkeytab_v1_0_r_epv->rkeytab_delete_key)		     \
		(NULL, ktid, ktkey, st_p);			     \
	} else if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {    \
	   (*rkeytab_v1_0_l_epv.rkeytab_delete_key)		     \
		(NULL, ktid, ktkey, st_p);			     \
	} else {						     \
	   (*rkeytab_v1_0_c_epv.rkeytab_delete_key)		     \
		(RPC_BINDING(handle), ktid, ktkey, st_p);	     \
	}
#define Rkeytab_change_key(handle, ktid, ktkey, st_p)		     \
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) {    \
	   (*rkeytab_v1_0_r_epv->rkeytab_change_key)		     \
		(NULL, ktid, ktkey, st_p);			     \
	} else if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {    \
	   (*rkeytab_v1_0_l_epv.rkeytab_change_key)		     \
		(NULL, ktid, ktkey, st_p);			     \
	} else {						     \
	   (*rkeytab_v1_0_c_epv.rkeytab_change_key)		     \
		(RPC_BINDING(handle), ktid, ktkey, st_p);	     \
	}
#define Rkeytab_get_keys(handle, ktid, ktkeys, st_p)		     \
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) {    \
	   (*rkeytab_v1_0_r_epv->rkeytab_get_keys)		     \
		(NULL, ktid, ktkeys, st_p);			     \
	} else if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {    \
	   (*rkeytab_v1_0_l_epv.rkeytab_get_keys)		     \
		(NULL, ktid, ktkeys, st_p);			     \
	} else {						     \
	   (*rkeytab_v1_0_c_epv.rkeytab_get_keys)		     \
		(RPC_BINDING(handle), ktid, ktkeys, st_p);	     \
	}
#define Rkeytab_get_list(handle, ktentry_list, st_p)		     \
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) {    \
	   (*rkeytab_v1_0_r_epv->rkeytab_get_list)		     \
		(NULL, ktentry_list, st_p);			     \
	} else if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {    \
	   (*rkeytab_v1_0_l_epv.rkeytab_get_list)		     \
		(NULL, ktentry_list, st_p);			     \
	} else {						     \
	   (*rkeytab_v1_0_c_epv.rkeytab_get_list)		     \
		(RPC_BINDING(handle), ktentry_list, st_p);	     \
	}

/*
** hostdata operations 
*/
#define	Rhostdata_get_list(handle, hdentry_list, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*hostdata_v1_0_r_epv->hostdata_get_list) \
	(RPC_BINDING(handle), hdentry_list, st_p); \
} else { \
    (*hostdata_v1_0_c_epv.hostdata_get_list) \
	(RPC_BINDING(handle), hdentry_list, st_p); \
}

#define	Rhostdata_create(handle, hdentry, ifimport, hdattr, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*hostdata_v1_0_r_epv->hostdata_create) \
	(RPC_BINDING(handle), hdentry, ifimport, hdattr, st_p); \
} else { \
    (*hostdata_v1_0_c_epv.hostdata_create) \
	(RPC_BINDING(handle), hdentry, ifimport, hdattr, st_p); \
}

#define	Rhostdata_delete(handle, hdid, ifdelobj, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*hostdata_v1_0_r_epv->hostdata_delete) \
	(RPC_BINDING(handle), hdid, ifdelobj, st_p); \
} else { \
    (*hostdata_v1_0_c_epv.hostdata_delete) \
	(RPC_BINDING(handle), hdid, ifdelobj, st_p); \
}

#define	Rhostdata_get(handle, hdid, hdattrid, hdattr, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*hostdata_v1_0_r_epv->hostdata_get) \
	(RPC_BINDING(handle), hdid, hdattrid, hdattr, st_p); \
} else { \
    (*hostdata_v1_0_c_epv.hostdata_get) \
	(RPC_BINDING(handle), hdid, hdattrid, hdattr, st_p); \
}

#define	Rhostdata_set(handle, hdid, hdattr, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*hostdata_v1_0_r_epv->hostdata_set) \
	(RPC_BINDING(handle), hdid, hdattr, st_p); \
} else { \
    (*hostdata_v1_0_c_epv.hostdata_set) \
	(RPC_BINDING(handle), hdid, hdattr, st_p); \
}

/*
** srvrconf operations 
*/
#define	Rsrvrconf_get_list(handle, scentry_list, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrconf_v1_0_r_epv->srvrconf_get_list) \
	(RPC_BINDING(handle), scentry_list, st_p); \
} else { \
    (*srvrconf_v1_0_c_epv.srvrconf_get_list) \
	(RPC_BINDING(handle), scentry_list, st_p); \
}

#define	Rsrvrconf_get(handle, scid, scentry, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrconf_v1_0_r_epv->srvrconf_get) \
	(RPC_BINDING(handle), scid, scentry, st_p); \
} else { \
    (*srvrconf_v1_0_c_epv.srvrconf_get) \
	(RPC_BINDING(handle), scid, scentry, st_p); \
}

#define	Rsrvrconf_create(handle, scentry, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrconf_v1_0_r_epv->srvrconf_create) \
	(RPC_BINDING(handle), scentry, st_p); \
} else { \
    (*srvrconf_v1_0_c_epv.srvrconf_create) \
	(RPC_BINDING(handle), scentry, st_p); \
}

#define	Rsrvrconf_delete(handle, scid, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrconf_v1_0_r_epv->srvrconf_delete) \
	(RPC_BINDING(handle), scid, st_p); \
} else { \
    (*srvrconf_v1_0_c_epv.srvrconf_delete) \
	(RPC_BINDING(handle), scid, st_p); \
}

#define	Rsrvrconf_attr_replace(handle, scid, scattr, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrconf_v1_0_r_epv->srvrconf_attr_replace) \
	(RPC_BINDING(handle), scid, scattr, st_p); \
} else { \
    (*srvrconf_v1_0_c_epv.srvrconf_attr_replace) \
	(RPC_BINDING(handle), scid, scattr, st_p); \
}

#define	Rsrvrconf_fixattr_replace(handle, scid, scfix, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrconf_v1_0_r_epv->srvrconf_fixattr_replace) \
	(RPC_BINDING(handle), scid, scfix, st_p); \
} else { \
    (*srvrconf_v1_0_c_epv.srvrconf_fixattr_replace) \
	(RPC_BINDING(handle), scid, scfix, st_p); \
}

#define	Rsrvrconf_start(handle, scid, scfix, scattr, scinst, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrconf_v1_0_r_epv->srvrconf_start) \
	(RPC_BINDING(handle), scid, scfix, scattr, scinst, st_p); \
} else { \
    (*srvrconf_v1_0_c_epv.srvrconf_start) \
	(RPC_BINDING(handle), scid, scfix, scattr, scinst, st_p); \
}

/*
** srvrexec operations 
*/
#define	Rsrvrexec_get_list(handle, sxentry_list, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_get_list) \
	(RPC_BINDING(handle), sxentry_list, st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_get_list) \
	(RPC_BINDING(handle), sxentry_list, st_p); \
}

#define	Rsrvrexec_get(handle, sxid, sxentry, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_get) \
	(RPC_BINDING(handle), sxid, sxentry, st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_get) \
	(RPC_BINDING(handle), sxid, sxentry, st_p); \
}

#define	Rsrvrexec_started(handle, sxid, sxeps, sxinst, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_started) \
	(RPC_BINDING(handle), sxid, sxeps, sxinst, st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_started) \
	(RPC_BINDING(handle), sxid, sxeps, sxinst, st_p); \
}

#define	Rsrvrexec_create(handle, sxentry, sxinst, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_create) \
	(RPC_BINDING(handle), sxentry, sxinst, st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_create) \
	(RPC_BINDING(handle), sxentry, sxinst, st_p); \
}

#define	Rsrvrexec_exited(handle, sxinst, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_exited) \
	(RPC_BINDING(handle), sxinst, st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_exited) \
	(RPC_BINDING(handle), sxinst, st_p); \
}

#define	Rsrvrexec_stop(handle, sxid, sxmethod, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_stop) \
	(RPC_BINDING(handle), sxid, sxmethod, st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_stop) \
	(RPC_BINDING(handle), sxid, sxmethod, st_p); \
}

#define	Rsrvrexec_shutdown(handle, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_shutdown) \
	(RPC_BINDING(handle), st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_shutdown) \
	(RPC_BINDING(handle), st_p); \
}

#define	Rsrvrexec_enable_service(handle, sxid, sxifid, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_enable_service) \
	(RPC_BINDING(handle), sxid, sxifid, st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_enable_service) \
	(RPC_BINDING(handle), sxid, sxifid, st_p); \
}

#define	Rsrvrexec_disable_service(handle, sxid, sxifid, st_p) \
if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_local)) { \
    (*srvrexec_v1_0_r_epv->srvrexec_disable_service) \
	(RPC_BINDING(handle), sxid, sxifid, st_p); \
} else { \
    (*srvrexec_v1_0_c_epv.srvrexec_disable_service) \
	(RPC_BINDING(handle), sxid, sxifid, st_p); \
}

/*
** Free routines
*/
#define	Dced_entry_list_free_ptrs(handle, list_p)			\
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {	\
		dced_entry_list_free_ptrs(list_p);			\
	} else {							\
		entry_list_free_ptrs(list_p, dced_sm_client_free);	\
	}
#define	Dced_key_list_free_ptrs(handle, dced_key_list_p)		 \
	if (SYNTAX_EQUAL(handle, dced_c_binding_syntax_bypass)) {	 \
		dced_key_list_free_ptrs(dced_key_list_p);		 \
	} else {							 \
		key_list_free_ptrs(dced_key_list_p, dced_sm_client_free);\
	}

#endif /* macros__included */

/*
** dced api internal funcction prototypes
*/ 

/*
** dced__service_from_name converts a service name strings to
** a dced service type.
*/
extern void dced__service_from_name(
        /* [in]  */     dced_string_t		/* name */,
        /* [out] */     dced_service_type_t*	/* service */,
        /* [out] */     error_status_t*		/* st */
);

/*
** dced__get_entry_list reads dced service's entry list from a 
** specified host.
*  If the list is NULL, the list will be loaded into the cache.
*/
extern void dced__get_entry_list(
        /* [in]  */     dced_binding_handle_t	/* h */,
        /* [out] */     dced_entry_list_t*	/* list */,	
        /* [out] */     error_status_t*		/* st */
);

/*
** dced__cache_delete erases/zeroes a entry in the
** cache.
*/
extern void dced__cache_delete(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced__load_cache loads local cache
*/
extern void dced__load_cache(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced__unload_cache unloads the local cache(s)
*/
extern void dced__unload_cache(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [out] */	error_status_t*		/* st */
);

#endif
