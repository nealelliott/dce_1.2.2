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
 * $Log: dhd.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:20:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/10/17  20:29 UTC  nav
 * 	merge from the private branch.
 * 
 * 	HP revision /main/HPDCE02/nav_dcecp_ping/1  1995/10/17  19:19 UTC  nav
 * 	add timeout options added to server ping
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/29  20:47 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  16:49 UTC  tatsu_s
 * 	Added dhd__schema_ent_free_ptrs() kluge.
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/04  17:25 UTC  mdf
 * 	Merge for CHFts13120
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/14  16:05 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts13/1  1995/02/14  16:03 UTC  truitt
 * 	CHFts14285: Modify a couple macros to make sure the
 * 	_e convenience variable gets set upon error.
 * 	[1995/12/08  17:55:36  root]
 * 
 * Revision 1.1.2.14  1994/09/07  20:36:43  ajayb
 * 	Define  dhd_c_prot_level_none, dhd_c_authn_none,
 * 	and dhd_c_authz_none for the case when user is
 * 	not logged in.
 * 	[1994/09/07  17:53:25  ajayb]
 * 
 * Revision 1.1.2.13  1994/09/01  20:09:53  pwang
 * 	Added a param to dhd__parse_sec_attr[OT#11970]
 * 	[1994/09/01  20:02:45  pwang]
 * 
 * Revision 1.1.2.12  1994/08/24  21:01:47  pwang
 * 	Added entryname token for server [OT#11398]
 * 	[1994/08/24  20:55:25  pwang]
 * 
 * Revision 1.1.2.11  1994/08/24  16:56:18  baum
 * 	Added secval status and string bindings to secval cmds
 * 	[1994/08/24  16:54:05  baum]
 * 
 * Revision 1.1.2.10  1994/08/22  19:11:45  pwang
 * 	Added a param to dhd__parse_sec_attr() [OT#11424]
 * 	[1994/08/22  18:48:28  pwang]
 * 
 * Revision 1.1.2.9  1994/07/22  21:40:44  pwang
 * 	Replaced intermediate attr schema with dce_attr
 * 	schema interface [OT#10262]
 * 	[1994/07/22  19:42:39  pwang]
 * 
 * Revision 1.1.2.8  1994/07/08  21:17:59  pwang
 * 	Used level_pkt_privacy for keytab access.
 * 	[1994/07/08  19:41:28  pwang]
 * 
 * Revision 1.1.2.7  1994/07/05  16:30:04  pwang
 * 	Removed unused routine prototypes and some adjustments
 * 	[OT#11072, #11058, #11020]
 * 
 * Revision 1.1.2.6  1994/06/22  16:08:20  pwang
 * 	Added prototypes for new utility routines of attr_schema
 * 	and a few of minor fixes [OT #10464].
 * 	[1994/06/22  16:07:41  pwang]
 * 
 * Revision 1.1.2.5  1994/06/09  16:02:00  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:29  devsrc]
 * 
 * Revision 1.1.2.4  1994/05/18  21:57:19  pwang
 * 	Updated to latest RFC42.3 and added dhd__parse_bindata()
 * 	[1994/05/18  21:25:10  pwang]
 * 
 * Revision 1.1.2.3  1994/05/05  14:57:03  pwang
 * 	Don't nned to free string when calling Tcl_SetResult().
 * 	[1994/05/05  14:48:20  pwang]
 * 
 * Revision 1.1.2.2  1994/04/26  20:03:52  pwang
 * 	Added secval object (OT#10262)
 * 	[1994/04/26  19:37:25  pwang]
 * 
 * Revision 1.1.2.1  1994/04/22  19:59:24  pwang
 * 	Initial Implementation
 * 	[1994/04/22  19:50:06  pwang]
 * 
 * $EndLog$
 */

#ifndef	_DCP_HDH__H_INCLUDED
#define	_DCP_HDH__H_INCLUDED

/*
 * INCLUDE FILES
 */
#include <stdio.h>
#include <string.h>

#include <dce/dced.h>
#include <dce/dced_util.h>

#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_error.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/passwd.h>
#include <dce/sec_attr_util.h>
#include <dce/dce_cf.h>
#include <dce/dce_cf_const.h>
#include <dce/dce_attr_sch.h>

#include <dce/tcl.h>
#include <dcecp.h>
#include <util_general.h>
#include <util_var.h>
#include <util_help.h>
#include <util_signal.h>
#include <rgy.h>

/*
** DECLARATIONS 
*/


/*
 * General attribute tokens
 */
#define dhd_c_attr_id_token         "uuid"
#define dhd_c_attr_name_token       "name"
#define dhd_c_attr_annotation_token "annotation"
#define	dhd_c_def_attr_elements	    2

/*
 * Entry related attribute tokens
 */
#define dhd_c_entry_attr_storage_token    "storage"
#define dhd_c_entry_attr_data_token       "data"
#define dhd_c_max_entry_attr_size         4

/*
 * Server object related attribute tokens (includes fixedattr)
 */
#define	dhd_c_srvr_argument_token	"arguments"
#define	dhd_c_srvr_var_attr_token	"attributes"
#define	dhd_c_srvr_directory_token	"directory"
#define	dhd_c_srvr_entryname_token	"entryname"
#define	dhd_c_srvr_gid_token		"gid"
#define	dhd_c_srvr_keytab_token		"keytabs"
#define	dhd_c_srvr_prerequisite_token	"prerequisites"
#define	dhd_c_srvr_prin_name_token	"principals"
#define	dhd_c_srvr_program_token	"program"
#define	dhd_c_srvr_service_token	"services"
#define	dhd_c_srvr_starton_token	"starton"
#define	dhd_c_srvr_uid_token		"uid"
#define	dhd_c_max_srvr_attr_size	13

/*
 * Available server variable attribute type tokens
 */
#define	dhd_c_serv_attr_fileattr_token		"fileattr"
#define	dhd_c_serv_attr_binfileattr_token	"binfileattr"

/*
 * Available startup option tokens
 */
#define	dhd_c_starton_boot_token	"boot"
#define	dhd_c_starton_auto_token	"auto"
#define dhd_c_starton_explicit_token	"explicit"
#define	dhd_c_starton_failure_token	"failure"
#define	dhd_c_starton_schedule_token	"schedule"

/*
 * Service related attribute tokens
 */
#define	dhd_c_serv_bindings_token	"bindings"
#define	dhd_c_serv_entryname_token	"entryname"
#define	dhd_c_serv_flag_token		"flags"
#define	dhd_c_serv_ifname_token		"ifname"
#define	dhd_c_serv_ifspec_token		"interface"
#define	dhd_c_serv_object_token		"objects"
#define	dhd_c_serv_opnum_token		"operation"
#define	dhd_c_max_serv_attr_size	8

/*
 * Available service flag tokens  
 */
#define	dhd_c_serv_disabled_token	"disabled"

/*
 * Available service stop method tokens  
 */
#define	dhd_c_stop_m_rpc_token		"rpc"
#define	dhd_c_stop_m_soft_token		"soft"
#define	dhd_c_stop_m_hard_token		"hard"
#define dhd_c_stop_m_error_token	"error"

#define	dhd_c_stop_m_rpc_token_len	(sizeof dhd_c_stop_m_rpc_token - 1)
#define	dhd_c_stop_m_soft_token_len	(sizeof dhd_c_stop_m_soft_token - 1)
#define	dhd_c_stop_m_hard_token_len	(sizeof dhd_c_stop_m_hard_token - 1)
#define dhd_c_stop_m_error_token_len	(sizeof dhd_c_stop_m_error_token - 1)

/*
 * Available key type tokens and key data sizes
 */
#define	dhd_c_key_type_des_token    "des"
#define	dhd_c_key_type_plain_token  "plain"
#define dhd_c_min_key_data_size		2
#define dhd_c_max_key_data_size		4

/*
 * dced entry related tokens and their lengthes 
 */
#define	dhd_c_dced_entry_token		"/config/"
#define	dhd_c_server_token		"server"
#define	dhd_c_attr_schema_token		"xattrschema"

#define	dhd_c_dced_entry_token_len	(sizeof dhd_c_dced_entry_token - 1)
#define dhd_c_server_token_len		(sizeof dhd_c_server_token - 1)
#define	dhd_c_attr_schema_token_len	(sizeof dhd_c_attr_schema_token - 1)

/*
 * Available service ping method timeout tokens 
 */
#define	dhd_c_ping_m_timeout_min_token "min"
#define	dhd_c_ping_m_timeout_max_token "max"
#define	dhd_c_ping_m_timeout_default_token "default"

#define	dhd_c_ping_m_timeout_min_token_len       (sizeof dhd_c_ping_m_timeout_min_token -1)
#define	dhd_c_ping_m_timeout_max_token_len       (sizeof dhd_c_ping_m_timeout_max_token -1)
#define	dhd_c_ping_m_timeout_default_token_len   (sizeof dhd_c_ping_m_timeout_default_token -1)

/*
 * Server interface related 
 */
#define	dhd_c_def_if_elements	2

typedef enum dhd_show_flag_e_t {
    dhd_e_show_name,
    dhd_e_show_excl_key_value,
    dhd_e_show_all
} dhd_show_flag_t;

/*
** INTERNAL VARIABLES 
*/

/*
 * NSI entry search syntax
 */
#define dhd_c_entry_syntax	rpc_c_ns_syntax_dce

/*
 * The dce attr schema binding handle
 */
dce_attr_sch_handle_t		attr_schema_binding;

/*
 * Auth info related
 */
#define	dhd_c_prot_level	rpc_c_protect_level_default
#define	dhd_c_keytab_prot_level	rpc_c_protect_level_pkt_privacy
#define	dhd_c_authn_svc		rpc_c_authn_default
#define dhd_c_authz_svc		rpc_c_authz_dce
#define dhd_c_prot_level_none   rpc_c_protect_level_none
#define dhd_c_authn_none        rpc_c_authn_none
#define dhd_c_authz_none        rpc_c_authz_none

#define dhd_c_key_authn_service	rpc_c_authn_dce_secret

/*
 * We should use UUID_C_UUID_STRING_MAX instead of redefining one
 * But UUID_C_UUID_STRING_MAX is defined in uuidp.h and the header
 * file is not exported.
 */
#define	dhd_c_uuid_string_max	37

/*
 * The max decimal value of a byte is 255
 */
#define MAX_VALUE_OF_BYTE	255

/*
** Internal Macros
*/

#ifndef macros__included
#define macros__included

/*
 *  string and list related macros
 */
#define DCP_DHD_NULL_STRING(s)	     ((s) == NULL || *(s) == '\0')
#define DCP_DHD_EMPTY_LIST(listp)    ((listp)->count == 0 || !((listp)->list))
#define	DCP_DHD_STRING_FREE(s)	     if (!DCP_DHD_NULL_STRING(s)) free(s)
#define	DCP_DHD_STRING_FREE_RESET(s) if (!DCP_DHD_NULL_STRING(s)) {\
					free(s); s = NULL; }
#define	DCP_DHD_MALLOC_LIST_RESET(listp, type, num, st) { 	   \
		DCP_DHD_MALLOC_RESET((listp)->list, type, num, st);\
		if (DCP_DHD_GOOD_STATUS(st)) { 		   	   \
			(listp)->count = num;			   \
		}}

/*
 * Status code related macros
 */
#define DCP_DHD_GOOD_STATUS(st)            (st == error_status_ok)
#define DCP_DHD_BAD_STATUS(st)             (st != error_status_ok)
#define DCP_DHD_CLEAR_STATUS(st)           st = error_status_ok
#define DCP_DHD_CLEAR_STATUS_2(st1, st2)   st1 = st2 = error_status_ok

#define DCP_DHD_RESET_STATUS(st)     	   st = error_status_ok
#define DCP_DHD_STATUS_EQUAL(st, st_val)   (st == (st_val))
#define DCP_DHD_SET_STATUS(st, val)  	   st = val
#define DCP_DHD_CHECK_STATUS_RET(st)	   if (DCP_DHD_BAD_STATUS(st)) \
						DCP_SET_MISC_ERROR(st)
#define DCP_DHD_CHECK_STATUS_BRK(st)	   if (DCP_DHD_BAD_STATUS(st)) {\
						DCP_SET_RESULT_CODE(st);\
						break; }
#define DCP_DHD_CHECK_STATUS_OUT(st)	   if (DCP_DHD_BAD_STATUS(st)) goto out

/*
 * argc/argv related maccros
 */
#define	DCP_DHD_CLEAR_ARGV(margc, margv)   margc=0; margv=(char **)0
#define	DCP_DHD_CLEAR_ARGV_2(margc1, margv1, margc2, margv2)		    \
					   margc1=0; margv1=(char **)0;     \
					   margc2=0; margv2=(char **)0
#define	DCP_DHD_CLEAR_ARGV_3(margc1, margv1, margc2, margv2, margc3, margv3)\
					   margc1=0; margv1=(char **)0;	    \
					   margc2=0; margv2=(char **)0;     \
					   margc3=0; margv3=(char **)0
#define	DCP_DHD_ARGV_FREE(margc, margv)	   if (margv) free(margv);          \
					   DCP_DHD_CLEAR_ARGV(margc, margv)
#define	DCP_DHD_ARGV_FREE_2(margc1, margv1, margc2, margv2)	            \
	   if (margv1) free(margv1); if (margv2) free(margv2);              \
	   DCP_DHD_CLEAR_ARGV_2(margc1, margv1, margc2, margv2)
#define	DCP_DHD_ARGV_FREE_3(margc1, margv1, margc2, margv2, margc3, margv3) \
	   if (margv1) free(margv1); if (margv2) free(margv2); 	            \
	   if (margv3) free(margv3); 					    \
	   DCP_DHD_CLEAR_ARGV_3(margc1, margv1, margc2, margv2, margc3, margv3)
#define	DCP_DHD_ARGV_ALLOC(margc, margv)	{                           \
		if  (!((margv) = (char **)malloc(margc*sizeof(char *)))) {  \
			DCP_SET_MISC_ERROR(dcp_s_no_memory);                \
		} else {                                                    \
			memset((margv), 0, (margc*sizeof(char *)));         \
		}}
#define	DCP_DHD_ARGV_ALLOC_ST(margc, margv, st)	{                           \
		if  (!((margv) = (char **)malloc(margc*sizeof(char *)))) {  \
			DCP_SET_RESULT_CODE(dcp_s_no_memory);	 \
			DCP_DHD_SET_STATUS(st, dcp_s_no_memory); \
		} else {                                                    \
			memset((margv), 0, (margc*sizeof(char *)));         \
		}}

/*
 * Malloc macros
 */
#define DCP_DHD_MALLOC_RESET_RET(ptr, type, num)  {      \
	if (!(ptr = (type *)malloc(num*sizeof(type)))) { \
		DCP_SET_MISC_ERROR(dcp_s_no_memory);     \
	} else { 					 \
		memset((ptr), 0, (num*sizeof(type)));    \
	}}
#define DCP_DHD_MALLOC_RESET(ptr, type, num, st)  {      \
	if (!(ptr = (type *)malloc(num*sizeof(type)))) { \
		DCP_SET_RESULT_CODE(dcp_s_no_memory);	 \
		DCP_DHD_SET_STATUS(st, dcp_s_no_memory); \
	} else { 					 \
		memset((ptr), 0, (num*sizeof(type)));    \
	}}
#define DCP_DHD_MALLOC_BLK_RESET(ptr, type, length, st) {\
	if (!(ptr = (type *)malloc(length))) { 		 \
		DCP_SET_RESULT_CODE(dcp_s_no_memory);    \
		DCP_DHD_SET_STATUS(st, dcp_s_no_memory); \
	} else { 					 \
		memset((ptr), 0, length); 		 \
	}}

/*
 * Macro for dce_sprintf
 */
#define	DCP_DHD_SET_RESULT(code, string) {                                     \
    if (string)                                                                \
        Tcl_SetResult(interp, (char *)dce_sprintf(code, string), TCL_DYNAMIC); \
    else                                                                       \
        Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);         \
    DCP_GENERAL_SET_E_VAR(code);                                               \
}

/*
 * Macro for return
 */
#define	DCP_DHD_RETURN(st)	if (DCP_DHD_GOOD_STATUS(st)) return TCL_OK;\
				else return TCL_ERROR

#endif /* macros__included */

/*
 * PROTOTYPES 
 */

/*
 * server
 */
extern int dcp_sr_catalog(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_create(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_delete(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_disable(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_enable(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_modify(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_ping(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_show(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_start(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sr_stop(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

/*
 * hostdata
 */
extern int dcp_hd_catalog(
        ClientData      /* clientData */, 
	Tcl_Interp*     /* interp */, 
        int             /* argc */, 
        char**          /* argv */
);
extern int dcp_hd_create(
        ClientData      /* clientData */, 
	Tcl_Interp*     /* interp */, 
        int             /* argc */, 
        char**          /* argv */
);
extern int dcp_hd_delete(
        ClientData      /* clientData */, 
	Tcl_Interp*     /* interp */, 
        int             /* argc */, 
        char**          /* argv */
);
extern int dcp_hd_modify(
        ClientData      /* clientData */, 
	Tcl_Interp*     /* interp */, 
        int             /* argc */, 
        char**          /* argv */
);
extern int dcp_hd_show(
        ClientData      /* clientData */, 
	Tcl_Interp*     /* interp */, 
        int             /* argc */, 
        char**          /* argv */
);

/*
 * keytab
 */
extern int dcp_kt_catalog(
	ClientData	/* clientData */,
	Tcl_Interp*	/* interp */,
	int		/* argc */,
	char**		/* argv */
);

extern int dcp_kt_create(
	ClientData	/* clientData */,
	Tcl_Interp*	/* interp */,
	int		/* argc */,
	char**		/* argv */
);

extern int dcp_kt_delete(
	ClientData	/* clientData */,
	Tcl_Interp*	/* interp */,
	int		/* argc */,
	char**		/* argv */
);

extern int dcp_kt_add(
	ClientData	/* clientData */,
	Tcl_Interp*	/* interp */,
	int		/* argc */,
	char**		/* argv */
);

extern int dcp_kt_remove(
	ClientData	/* clientData */,
	Tcl_Interp*	/* interp */,
	int		/* argc */,
	char**		/* argv */
);

extern int dcp_kt_show(
	ClientData	/* clientData */,
	Tcl_Interp*	/* interp */,
	int		/* argc */,
	char**		/* argv */
);

extern int dcp_kt_list(
	ClientData	/* clientData */,
	Tcl_Interp*	/* interp */,
	int		/* argc */,
	char**		/* argv */
);

/*
 * secval
 */
extern int dcp_sv_ping(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sv_status(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sv_activate(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sv_deactivate(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);

extern int dcp_sv_update(
	ClientData	/* clientData */, 
	Tcl_Interp*	/* interp */, 
	int		/* argc */, 
	char**		/* argv */
);


/*
 * INTERNAL PROTOTYPES
 */
extern int dhd__parse_attr(
	Tcl_Interp*     /* interp */, 
        char*           /* list */, 
        dced_entry_t*   /* entry */, 
        char**          /* dataptr */
);

extern int dhd__parse_keys(
	Tcl_Interp*		/* interp */,
	char*			/* list */,
	boolean32		/* random_gen */,
	dced_key_list_t*	/* keys */
);

extern int dhd__show_entry(
	Tcl_Interp*     /* interp */, 
        dced_entry_t*   /* entry */, 
	char*		/* ns_entry */,
        dhd_show_flag_t /* flag */
);

extern int dhd__show_object(
	Tcl_Interp*           /* interp */, 
	void*                 /* object */,
        dced_service_type_t   /* service */, 
        dhd_show_flag_t       /* flag */
);

extern int isllist(
	char*	/* list */
);

extern int dhd__parse_interface(
	Tcl_Interp*	/* interp */,
	char*		/* interface_str */,
	rpc_if_id_t*	/* rpc_if */
);
extern int dhd__parse_interface_list(
	Tcl_Interp*	/* interp */,
	char*		/* interface_list */,
	int*		/* count */,
	rpc_if_id_t**	/* rpc_if_list */
);

extern int dhd__parse_name(
	Tcl_Interp*	/* interp */,
	char*		/* name */,
	char*		/* service */,
	int		/* service_len */,
	char*		/* hostname */,
	char**		/* entryname */,
	char**		/* objname */,
	char**		/* fullname */
);

extern int dhd__parse_towers(
	Tcl_Interp*			/* interp */,
	rpc_if_handle_t			/* if_handle */,
	int				/* inargc */,
	char**				/* inargv */,
	dced_tower_list_t*		/* tower_vector */
);

extern int dhd__parse_sec_attr(
	Tcl_Interp*		/* interp */,
	char*			/* list */,
	dced_attr_list_t*	/* attribute */,
        era_info_t**            /* era_info */,
        boolean32               /* tyoe */
);

extern int dhd__parse_services(
	Tcl_Interp*	/* interp */,
	char*		/* list */,
	service_list_t*	/* service_list */
);

extern int dhd__parse_server(
	Tcl_Interp*	/* interp */,
	char*		/* list */,
	server_t*	/* server */,
	char**		/* var_attr */
);

extern int dhd__schema_name_from_dced_name(
        Tcl_Interp*			/* interp */,
        char*				/* dced_name */,
        dce_attr_component_name_t*	/* schema_name */
);

#ifndef	HPDCE_DCED_LOCAL
extern void dhd__schema_ent_free_ptrs(
	sec_attr_schema_entry_t*	/* sec_sch_entry_p */
);
#endif	/* HPDCE_DCED_LOCAL */

#endif /* _DCP_DHD_H_INCLUDED */
