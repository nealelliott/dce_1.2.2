/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_common.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:16:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:16  root]
 * 
 * Revision 1.1.7.3  1994/08/03  19:02:26  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:14  mccann]
 * 
 * Revision 1.1.7.2  1994/06/09  18:39:13  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:51  devsrc]
 * 
 * Revision 1.1.7.1  1994/03/12  22:03:29  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:16:49  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  13:32:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:13  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  19:14:58  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:13:29  weisman]
 * 
 * Revision 1.1.2.2  1992/05/22  14:01:46  mfox
 * 	Delete obsolete wired-in tower lengths and add comments to debug levels.
 * 	[1992/05/21  23:45:43  mfox]
 * 
 * Revision 1.1  1992/01/19  15:16:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _GDA_COMMON_H
#define _GDA_COMMON_H
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
 */

/* gda_common.h: includes the definitions and includes common to all of the
	modules which make up the gda. */

/* These are architectural constants defined for CDS */
#define MASTER_REPLICA		1
#define READ_ONLY_REPLICA	3

/* Each function is associated with a number used as an index into func_array*/
#define BAD_FUNC			-2
#define RWL_INIT_FUNC			0
#define	RWL_FREE_FUNC			1
#define	READ_LOCK_FUNC			2
#define	WRITE_LOCK_FUNC			3
#define	READ_UNLOCK_FUNC		4
#define	WRITE_UNLOCK_FUNC		5
#define	READ_TRYLOCK_FUNC		6
#define	WRITE_TRYLOCK_FUNC		7

#define	L_INIT_FUNC			8
#define	L_FREE_FUNC			9
#define	L_EL_INIT_FUNC			10
#define	L_EL_FREE_FUNC			11
#define	L_INSERT_FUNC			12
#define	L_INSERT_HEAD_FUNC		13
#define	L_DELETE_FUNC			14
#define	L_NEXT_FUNC			15
#define	L_FIRST_FUNC			16
#define	L_RELEASE_LINK_FUNC		17
#define	L_RELEASE_EL_FUNC		18
#define	L_RELEASE_DATA_FUNC		19
#define	L_INIT_NL_FUNC			20
#define	L_FREE_NL_FUNC			21
#define	L_EL_INIT_NL_FUNC		22
#define	L_EL_FREE_NL_FUNC		23
#define	L_INSERT_NL_FUNC		24
#define	L_DELETE_NL_FUNC		25
#define	L_NEXT_NL_FUNC			26
#define	L_FIRST_NL_FUNC			27

#define	INIT_CONDUITS_FUNC		28
#define	CHOOSE_CONDUIT_FUNC		29
#define SHOULD_PRINT_FUNC_FUNC		30
#define	ERROR_EXIT_FUNC			31
#define	ERROR_EXIT_TH_FUNC		32
#define	READ_DEBUG_COMMANDS_FUNC	33
#define	INIT_DEBUG_FUNC			34
#define	SIGNAL_HANDLER_FUNC		35
#define	INIT_MAIN_FUNC			36
#define	MAIN_FUNC			37

#define	INIT_RPC_INT_FUNC		38
#define BUILD_PROGRESS_FUNC		39
#define GET_RPC_ANSWER_FUNC		40
#define	CDS_CREATESOFTLINK_FUNC		41
#define	CDS_CREATEOBJECT_FUNC		42
#define	CDS_DELETEOBJECT_FUNC		43
#define	CDS_DELETESOFTLINK_FUNC		44
#define	CDS_DELETEDIRECTORY_FUNC	45
#define	CDS_ENUMERATEATTRIBUTES_FUNC	46
#define	CDS_ENUMERATECHILDREN_FUNC	47
#define	CDS_ENUMERATEOBJECTS_FUNC	48
#define	CDS_ENUMERATESOFTLINKS_FUNC	49
#define	CDS_MODIFYATTRIBUTE_FUNC	50
#define	CDS_READATTRIBUTE_FUNC		51
#define	CDS_RESOLVENAME_FUNC		52
#define	CDS_SKULK_FUNC			53
#define	CDS_TESTATTRIBUTE_FUNC		54
#define	CDS_TESTGROUP_FUNC		55
#define	CDS_ALLOWCLEARINGHOUSES_FUNC	56
#define	CDS_DISALLOWCLEARINGHOUSES_FUNC 57

#define	INIT_RECV_BIND_FUNC		58
#define	BINDQ_EXT_TO_INT_FUNC		59
#define	WHOLE_LIST_FREE_NL_FUNC		60
#define	RECV_BIND_FUNC			61
#define	GETTOKEN_FUNC			62
#define	INIT_SERVER_FUNC		63
#define	INIT_BIND_FUNC			64

#define	RR_HASH_FUNC			65
#define	LIST_INSERT_INORDER_FUNC	66
#define	RR_CMP_FUNC			67
#define	FIND_RR_FUNC			68
#define INIT_BIND_PROCESS_FUNC		69
#define SEND_RECV_VC_FUNC		70
#define BIND_PROCESS_FUNC		71

#define PROCESS_FUNC			72
#define FIND_STRING_TXT_FUNC		73
#define RPC_LISTENER_FUNC		74
#define	READ_TO_WRITE_LOCK_FUNC		75
#define	WRITE_TO_READ_LOCK_FUNC		76
#define	MAKE_BIND_QUERY_FUNC		77
#define	ALLOC_MACRO			78
#define	FREE_MACRO			79
#define	ALLOC_COMMA_MACRO		80
#define	FREE_COMMA_MACRO		81
#define	INIT_MEMSTAT_FUNC		82
#define	PRINT_MEMSTAT_FUNC		83
#define	MEMSTAT_FUNC			84
#define	INIT_COUNTER_MACRO		85
#define	PRINT_MEM_STAT_MACRO		86

#define X500_FUNC_BEGIN			87
#define X500_INIT_DEBUG_FUNC		87
#define X500_CELL_TO_STR_FUNC		88
#define X500_REPLICA_TO_STR_FUNC	89
#define X500_STR_TO_CELL_FUNC		90
#define X500_STR_TO_REPLICA_FUNC	91
#define X500_READ_DATA_FUNC		92
#define X500_WRITE_DATA_FUNC		93
#define X500_PROCESS_FUNC		94
#define X500_RES_NAME_PARENT_FUNC	95
#define X500_RES_NAME_RSP_FUNC		96
#define X500_VALID_NAME_RSP_FUNC	97
#define X500_CHILD_ERR_FUNC		98
#define X500_RN_CONVERT_FUNC		99
#define X500_RN_SPLIT_NAME_FUNC		100
#define X500_VN_CONVERT_FUNC		101
#define X500_MAIN_FUNC			102
#define X500_RES_NAME_REQ_FUNC		103
#define X500_VALID_NAME_REQ_FUNC	104
#define X500_BIND_FUNC			105
#define X500_UNBIND_FUNC		106
#define X500_READ_CELL_FUNC		107
#define X500_MODIFY_CELL_FUNC		108
#define X500_VALIDATE_NAME_FUNC		109
#define X500_ADMIN_PROGRAM_FUNC		110

#define CDS_DOUPDATE_FUNC		111
#define CDS_COMBINE_FUNC		112
#define CDS_LINKREPLICA_FUNC		113
#define CDS_MODIFYREPLICA_FUNC		114
#define CDS_ADDREPLICA_FUNC		115
#define CDS_REMOVEREPLICA_FUNC		116
#define CDS_NEWEPOCH_FUNC		117
#define CDS_DELETECHILD_FUNC		118
#define CDS_CREATECHILD_FUNC		119
#define CDS_CREATEDIRECTORY_FUNC	120

#define INIT_CLIENT_FUNC		121

#define	NUM_FUNC			122

/* Each error string is associated with a number used as an index into
	error_array */
#define	BAD_ERR_ST				-2
#define	GETTIMEOFDAY_ERR_ST			0
#define	MALLOC_ERR_ST				1
#define FOPEN_ERR_ST				2
#define SOCKET_ERR_ST				3
#define SENDTO_ERR_ST				4
#define CONNECT_ERR_ST				5
#define WRITEV_ERR_ST				6
#define READ_ERR_ST				7
#define	SIGFILLSET_ERR_ST			8
#define	RECVFROM_ERR_ST				9

#define	PTHREAD_CREATE_ERR_ST			10
#define	PTHREAD_EXIT_ERR_ST			11
#define	PTHREAD_MUTEX_INIT_ERR_ST		12
#define	PTHREAD_MUTEX_DESTROY_ERR_ST		13
#define	PTHREAD_MUTEX_LOCK_ERR_ST		14
#define	PTHREAD_MUTEX_TRYLOCK_ERR_ST		15
#define	PTHREAD_MUTEX_UNLOCK_ERR_ST		16
#define	PTHREAD_COND_INIT_ERR_ST		17
#define	PTHREAD_COND_DESTROY_ERR_ST		18
#define	PTHREAD_COND_WAIT_ERR_ST		19
#define	PTHREAD_COND_TIMEDWAIT_ERR_ST		20
#define	PTHREAD_COND_SIGNAL_ERR_ST		21
#define	PTHREAD_COND_BROADCAST_ERR_ST		22
#define	PTHREAD_KEYCREATE_ERR_ST		23
#define	PTHREAD_SETSPECIFIC_ERR_ST		24
#define	PTHREAD_GETSPECIFIC_ERR_ST		25

#define	RPC_SERVER_USE_PROTOSEQ_IF_ERR_ST	26
#define	RPC_SERVER_REGISTER_IF_ERR_ST		27
#define	RPC_SERVER_LISTEN_ERR_ST		28

#define	RWL_INIT_ERR_ST				29
#define	RWL_FREE_ERR_ST				30
#define	READ_LOCK_ERR_ST			31
#define	WRITE_LOCK_ERR_ST			32
#define	READ_UNLOCK_ERR_ST			33
#define	WRITE_UNLOCK_ERR_ST			34
#define	READ_TRYLOCK_ERR_ST			35
#define	WRITE_TRYLOCK_ERR_ST			36

#define	L_INIT_ERR_ST				37
#define	L_FREE_ERR_ST				38
#define	L_EL_INIT_ERR_ST			39
#define	L_EL_FREE_ERR_ST			40
#define	L_INSERT_ERR_ST				41
#define	L_DELETE_ERR_ST				42
#define	L_NEXT_ERR_ST				43
#define	L_FIRST_ERR_ST				44
#define	L_RELEASE_LINK_ERR_ST			45
#define	L_RELEASE_EL_ERR_ST			46
#define	L_RELEASE_DATA_ERR_ST			47
#define	L_EL_INIT_NL_ERR_ST			48
#define	L_INSERT_NL_ERR_ST			49
#define	L_FREE_NL_ERR_ST			50

#define	HEAD_NONULL_ERR_ST			51
#define	GETWORD_ERR_ST				52
#define	BINDQ_EXT_TO_INT_ERR_ST			53
#define MAKE_BIND_QUERY_ERR_ST			54
#define LIST_INSERT_INORDER_ERR_ST		55
#define SEND_RECV_VC_ERR_ST			56
#define	NO_ERR_ST				57
#define	SIGWAIT_ERR_ST				58
#define	BIND_ERR_ST				59
#define	RPC_SERVER_INQ_BINDINGS_ERR_ST		60
#define	RPC_BINDING_TO_STRING_BINDING_ERR_ST	61
#define	RPC_SS_ALLOCATE_ERR_ST			62
#define WRITE_ERR_ST				63
#define	READ_TO_WRITE_LOCK_ERR_ST		64
#define	CHOOSE_CONDUIT_ERR_ST			65
#define	RPC_SERVER_USE_PROTOSEQ_ERR_ST		66
#define	RPC_EP_REGISTER_ERR_ST			67

#define X500_ERR_BEGIN				68
#define X500_FATAL_ERR_ST			68
#define X500_SYSTEM_ERR_ST			69
#define X500_DIRECTORY_ERR_ST			70
#define X500_RECOVERABLE_ERR_ST			71

#define X500_RN_FAILURE_ERR_ST			72
#define X500_VN_FAILURE_ERR_ST			73
#define X500_UNKNOWN_MSG_ERR_ST			74
#define UPDATE_PARENT_ERR_ST			75
#define PRINT_PROGRESS_ERR_ST			76
#define PRINT_RP_ERR_ST				77
#define PRINT_OBJUID_ERR_ST			78
#define INIT_CLIENT_ERR_ST			79

#define	NUM_ERROR_STRINGS			80

/* These are the severity levels used to determine how bad an error has
	occurred.  The first six levels are associated with a syslog error
	code. */
#define BAD_SEV			-2
#define SEV_DEATH		0
#define	NO_DEBUG		1
#define SEV_TH_DEATH		2
#define SEV_RECOVERABLE		3
#define SEV_PROBLEM		4
#define SEV_POSSIBLE_PROBLEM	5
#define SEV_DEBUG_1		6			/* -d6 for errors */
#define SEV_DEBUG_2		7			/* -d7 for ins & outs */
#define SEV_DEBUG_3		8			/* -d8 for heap memory usage */
#define SEV_DEBUG_4		9			/* -d9 for everything */
#define SEV_DEBUG_5		10
#define SEV_DEBUG_6		11
#define SEV_DEBUG_LAST		SEV_DEBUG_6

#define	NUM_SEVERITY_LEVELS	12
#define	NO_ERRNUM		-1

/* thread type constants */
#define	BAD_THREAD				0
#define	CDS_CREATESOFTLINK_TH_NUM		1
#define	CDS_CREATEOBJECT_TH_NUM			2
#define	CDS_DELETE_OBJECT_TH_NUM		3
#define	CDS_DELETESOFTLINK_TH_NUM		4
#define	CDS_DELETEDIRECTORY_TH_NUM		5
#define	CDS_ENUMERATEATTRIBUTES_TH_NUM		6
#define	CDS_ENUMERATECHILDREN_TH_NUM		7
#define	CDS_ENUMERATEOBJECTS_TH_NUM		8
#define	CDS_ENUMERATESOFTLINKS_TH_NUM		9
#define	CDS_MODIFYATTRIBUTE_TH_NUM		10
#define	CDS_READ_ATTRIBUTE_TH_NUM		11
#define	CDS_RESOLVENAME_TH_NUM			12
#define	CDS_SKULK_TH_NUM			13
#define	CDS_TESTATTRIBUTE_TH_NUM		14
#define	CDS_TESTGROUP_TH_NUM			15
#define	CDS_ALLOWCLEARINGHOUSES_TH_NUM		16
#define	CDS_DISALLOWCLEARINGHOUSES_TH_NUM	17
#define	MAIN_TH_NUM				18
#define	RECV_BIND_TH_NUM			19
#define	RECV_500_TH_NUM				20
#define	RPC_LISTENER_TH_NUM			21
#define	MEMSTAT_TH_NUM				22
#define CDS_DOUPDATE_TH_NUM			23
#define CDS_COMBINE_TH_NUM			24
#define CDS_LINKREPLICA_TH_NUM			25
#define CDS_MODIFYREPLICA_TH_NUM		26
#define CDS_ADDREPLICA_TH_NUM			27
#define CDS_REMOVEREPLICA_TH_NUM		28
#define CDS_NEWEPOCH_TH_NUM			29
#define CDS_DELETECHILD_TH_NUM			30
#define CDS_CREATECHILD_TH_NUM			31
#define CDS_CREATEDIRECTORY_TH_NUM		32

#define	NUM_TH_TYPES		33

#if defined (X500_CHILD_PROCESS)

/* function names array */
static char *func_array[] = {
	"rwl_init",		"rwl_free",		"read_lock",
	"write_lock",		"read_unlock",		"write_unlock",
	"read_trylock",		"write_trylock",

	"l_init",		"l_free",		"l_el_init",
	"l_el_free",		"l_insert",		"l_insert_head",
	"l_delete",		"l_next",		"l_first",
	"l_release_link",	"l_release_el",		"l_release_data",
	"l_init_nl",		"l_free_nl",		"l_el_init_nl",
	"l_el_free_nl",		"l_insert_nl",		"l_delete_nl",
	"l_next_nl",		"l_first_nl",

	"init_conduits",	"choose_conduits",	"should_print_func",
	"error_exit",		"error_exit_th",	"read_debug_commands",
	"init_debug",		"signal_handler",	"init_main",
	"main",

	"init_rpc_int",			"build_progress",
	"get_rpc_answer",		"cds_createsoftlink",
	"cds_createobject",		"cds_deleteobject",
	"cds_deletesoftlink",		"cds_deletedirectory",
	"cds_enumerateattributes",	"cds_enumeratechildren",
	"cds_enumerateobjects",		"cds_enumeratesoftlinks",
	"cds_modifyattrubute",		"cds_readattribute",
	"cds_resolvename",		"cds_skulk",
	"cds_testattrubute",		"cds_testgroup",
	"cds_allowclearinghouses",	"cds_disallowclearinghouses",

	"init_recv_bind",	"bindq_ext_to_int",	"whole_list_free_nl",
	"recv_bind",		"gettoken",		"init_server",
	"init_bind",

	"rr_hash",		"list_insert_inorder",	"rr_cmp",
	"find_rr",		"init_bind_process",	"send_recv_vc",
	"bind_process",

	"process",

	"find_string_txt",

	"rpc_listener",

	"read_to_write_lock",

	"write_to_read_lock",

	"make_bind_query",

	"alloc_macro",

	"free_macro",

	"alloc_comma_macro",

	"free_comma_macro",

	"init_memstat",

	"print_memstat",

	"memstat",

	"init_counter_macro",

	"print_mem_stat_macro",
	/*
	 * The following functions are exist within the X.500 GDA module.
	 */
        "x500_init_debug",     "x500_cell_to_str",     "x500_replica_to_str",
        "x500_str_to_cell",    "x500_str_to_replica",  "x500_read_data",
        "x500_write_data",

        "x500_process",        "x500_res_name_parent", "x500_res_name_rsp",
        "x500_valid_name_rsp", "x500_child_err",

        "x500_rn_convert",     "x500_rn_split_name",   "x500_vn_convert",

        "x500_main",           "x500_res_name_req",    "x500_valid_name_req",

        "x500_bind",           "x500_unbind",          "x500_read_cell",
        "x500_modify_cell",    "x500_validate_name",

        "x500_admin_program",

	"cds_doupdate",			"cds_combine",
	"cds_linkreplica",		"cds_modifyreplica",
	"cds_addreplica",		"cds_removereplica",
	"cds_newepoch",			"cds_deletechild",
	"cds_createchild",		"cds_createdirectory",

	"init_client"
};

/* error strings array */
static char *error_array[] = {
	"gettimeofday",		"malloc",	"fopen",
	"socket",		"sendto",	"connect",
	"writev",		"read",		"sigfillset",
	"recvfrom",

	"pthread_create",		"pthread_exit",
	"pthread_mutex_init",		"pthread_mutex_destroy",
	"pthread_mutex_lock",		"pthread_mutex_trylock",
	"pthread_mutex_unlock",		"pthread_cond_init",
	"pthread_cond_destroy",		"pthread_cond_wait",
	"pthread_cond_timedwait",	"pthread_cond_signal",
	"pthread_cond_broadcast",	"pthread_keycreate",
	"pthread_setspecific",		"pthread_getspecific",

	"rpc_server_use_protoseq_if",	"rpc_server_register_if",
	"rpc_server_listen",

	"rwl_init",		"rwl_free",	"read_lock",
	"write_lock",		"read_unlock",	"write_unlock",
	"read_trylock",		"write_trylock",

	"l_init",		"l_free",	"l_el_init",
	"l_el_free",		"l_insert",	"l_delete",
	"l_next",		"l_first",	"l_release_link",
	"l_release_el",		"l_release_data","l_el_init_nl",
	"l_insert_nl",		"l_free_nl",

	"HEAD_NONNULL",		"getword",		"bindq_ext_to_int",
	"make_bind_query",	"list_insert_inorder",	"send_recv_vc",

	"",
	"sigwait",
	"bind",
	"rpc_server_inq_bindings",
	"rpc_binding_to_string_binding",
	"rpc_ss_allocate",
	"write",
	"read_to_write_lock",
	"choose_conduit",
	"rpc_server_use_protoseq",
	"rpc_ep_register",
	/*
	 * The following error codes may be generated within the X.500
	 * GDA module.
	 */
        "x500_fatal_program_error", "x500_system_error",
        "x500_directory_error",     "x500_recoverable_error",
	/*
	 * The following codes may be return inside an X500_CHILD_ERR
	 *  message from the X.500 GDA child process to its parent.
	 */
        "x500_res_name_failure",    "x500_valid_name_failure",
        "x500_unknown_socket_msg",
        "update_parent",

        "print_progress",
        "print_rp",
        "print_objuid",

        "init_client"
};


#if !defined(SYSV)
/* array to map severity levels to syslog levels */
static int syslog_array[] = {
	LOG_ERR,	LOG_ERR,	0,		LOG_WARNING,
	LOG_NOTICE,	LOG_INFO,	LOG_DEBUG,	LOG_DEBUG,
	LOG_DEBUG,	LOG_DEBUG,	LOG_DEBUG,	LOG_DEBUG
};

/* array of severity strings */
char *sev_array[] = {
	"death",	"thread_death",	"",
	"recoverable",	"problem",	"possible_problem",
	"debug_1",	"debug_2",	"debug_3",
	"debug_4",	"debug_5",	"debug_6"
};
#endif /* !SYSV */


#ifndef USE_LOCAL_PRINT_MACROS

/* PRINT_ALLWAYS is used by a function that allways wants a message printed */
#define PRINT_ALLWAYS(A,B,C,D,E)	error_print(A,B,C,D,E,1)

/* ERROR_PRINT is used by a function that wants a message printed only when
	debugging is enabled. */
#define ERROR_PRINT(A,B,C,D,E)	if(debug != 0) error_print(A,B,C,D,E,0)

#ifdef DEBUG

/* if the gda is compiled with debug, enable the printing of messages. */
#define	PRINT(A,B,C,D,E)	if(debug != 0) error_print(A,B,C,D,E,0)

#define	SHOULD_PRINT(A,B,C,D,E) (debug != 0) && should_print(A,B,C,D,E,0)

#else

/* if the gda is not compiled with debug, don't print any messages. */
#define	PRINT(A,B,C,D,E)

#define	SHOULD_PRINT(A,B,C,D,E)	0

#endif /* DEBUG */
#endif /* USE_LOCAL_PRINT_MACROS */

#ifndef TRUE
#define TRUE			1
#define FALSE			0
#endif

#endif /* X500_CHILD_PROCESS */

#define FULLNAME_MAXLEN		1023
#endif  /* #ifndef _GDA_COMMON_H */
