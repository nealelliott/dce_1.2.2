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
 * $Log: sample_server.c,v $
 * Revision 1.1.7.3  1996/02/18  18:26:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:22  marty]
 *
 * Revision 1.1.7.2  1995/12/08  20:54:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:56  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:06:58  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:13  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/21  19:13:31  weir
 * 	Submit sample program sources (CR 12635)
 * 	[1994/10/21  19:12:45  weir]
 * 
 * $EndLog$
 */
/****************************************************************************/
/* [27.VI.94]                                                               */
/*                                                                          */
/* sample_server.c -- Main program for "sample" server: initialization      */
/*                    and cleanup.                                          */
/*                                                                          */
/*  Note that no remote calls are defined in this file; for those, refer    */
/*   to either sample_manager.c or sample_bind.c.                           */
/*                                                                          */
/*                            -77 cols-                                     */
/****************************************************************************/

#define DCE_DEBUG

#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>


#include <dce/dce.h>
/* #include <dce/stubbase.h>  */
#include <dce/dce_cf.h>
#include <dce/dce_error.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>
#include <dce/keymgmt.h>
#include <dce/uuid.h>
#include <dce/exc_handling.h>
#include <dce/dce_msg.h> 
#include <dce/dbif.h>
#include <dce/aclif.h>
#include <dce/dceacl.h>
#include <dce/pgo.h>
#include <dce/dced.h>


#include <dce/dcesvcmsg.h>
#include <dce/svcremote.h>

#include "dcesmpsvc.h"
#include "dcesmpmsg.h"
#include "dcesmpmac.h"

/* sample-specific includes:                                                */
#include "sample.h"
#include "sample_db.h"
#include "sample_bind.h"
#include "sample_server.h"


/****************************************************************************/
/* ANSI-C style prototypes for functions private to this module...          */

int do_command_line(int,
		char **,
		unsigned_char_t **,
		entryname_vector_t *);

void signal_handler(char *);

void server_register_get_bindings(rpc_if_handle_t,
		rpc_binding_vector_t **,
		unsigned32 *);

void server_export_objects(rpc_if_handle_t,
		rpc_binding_vector_t *,
		uuid_vector_t *,
		entryname_vector_t *,
		unsigned_char_t *,
		unsigned32 *);

void server_cleanup_objects(rpc_if_handle_t,
		rpc_binding_vector_t *,
		uuid_vector_t *,
		entryname_vector_t *,
		unsigned32 *);

void managekey(char *);

void server_get_identity(unsigned_char_p_t,
		sec_login_handle_t *,
		unsigned_char_p_t,
		unsigned32 *);

void server_renew_identity(unsigned_char_p_t,
		sec_login_handle_t,
		unsigned_char_p_t,
		unsigned32,
		unsigned32 *);

void server_create_dflt_acl(dce_db_handle_t,
		unsigned_char_t *,
		void(*)(),
		boolean32,
		sec_acl_t *,
		uuid_t *,
		unsigned32 *);

void server_get_local_principal_id(unsigned_char_t *,
		uuid_t *,
		unsigned32 *);

void server_create_acl(uuid_t,
		sec_acl_permset_t,
		unsigned_char_t *,
		sec_acl_t *,
		uuid_t *,
		unsigned32 *);

void server_store_acl(dce_db_handle_t,
		dce_db_handle_t,
		dce_db_handle_t,
		sec_acl_t *,
		uuid_t *,
		uuid_t *,
		unsigned_char_t *,
		void *,
		boolean32,
		unsigned32 *);

void server_acl_mgr_setup(unsigned_char_t *,
		dce_acl_resolve_func_t,
		uuid_t,
		uuid_t,
		unsigned_char_t *,
		sec_acl_permset_t,
		unsigned_char_t *,
		boolean32,
		dce_db_handle_t *,
		dce_db_handle_t *,
		dce_db_handle_t *,
		uuid_t *,
		uuid_t *,
		unsigned32 *);

void server_acl_mgr_close(dce_db_handle_t *,
		dce_db_handle_t *,
		dce_db_handle_t *,
		unsigned32 *);

void server_rdacl_export(rpc_binding_vector_t *,
		uuid_vector_t *,
		unsigned32 *);

void server_rdacl_cleanup(rpc_binding_vector_t *,
		uuid_vector_t *,
		unsigned32 *);

void server_bind_cleanup(rpc_binding_vector_t *,
		uuid_vector_t *,
		unsigned32 *);

boolean32 sample_mgmt_auth(rpc_binding_handle_t,
		unsigned32,
		unsigned32 *);

dce_acl_resolve_func_t sample_resolve_by_name(handle_t,
		sec_acl_component_name_t,
		sec_acl_type_t,
		uuid_t *,
		boolean32,
		void *,
		uuid_t *,
		error_status_t *);

void sample_bind_export(rpc_binding_vector_t *,
		uuid_vector_t *,
		unsigned32 *);

void create_server_uuid(uuid_t *,
		uuid_vector_t *);

void print_server_error(char *,
		error_status_t);

/****************************************************************************/


/* Default routing information:                                             */
#define SAMPLE_SVC_FATAL_DEFAULT_ROUTE 0
#define SAMPLE_SVC_ERROR_DEFAULT_ROUTE 1
#define SAMPLE_SVC_WARNING_DEFAULT_ROUTE 2
#define SAMPLE_SVC_NOTICE_DEFAULT_ROUTE 3
#define SAMPLE_SVC_VERBOSE_NOTICE_DEFAULT_ROUTE 4
#define MAX_DEFAULT_ROUTES 5
unsigned_char_t *default_routes[MAX_DEFAULT_ROUTES] =
	{"FATAL:STDERR:",  
	 "ERROR:STDERR:",
	 "WARNING:STDERR:",
	 "NOTICE:STDERR:",
	 "NOTICE_VERBOSE:STDERR:"};
/*
	{"FATAL:TEXTFILE:/tmp/smp_svc_%ld",
	 "ERROR:TEXTFILE:/tmp/smp_svc_%ld",
	 "WARNING:TEXTFILE:/tmp/smp_svc_%ld",
	 "NOTICE:TEXTFILE:/tmp/smp_svc_%ld",
	 "NOTICE_VERBOSE:/tmp/smp_svc_%ld"};
*/

/* Default debug level and route...                                         */
unsigned_char_t *default_debug_route = (unsigned_char_t *)"smp:*.9:STDERR:-";
unsigned_char_t *default_debug_level = (unsigned_char_t *)"*.9";

/*                                                                          */
/* The debug level scheme is at present roughly as follows:                 */
/*                                                                          */
/*     svc_c_debug1 -- not used.                                            */
/*     svc_c_debug2 -- not used.                                            */
/*     svc_c_debug3 -- not used.                                            */
/*     svc_c_debug4 -- messages announcing calls to system (DCE or OS)      */
/*                     routines.                                            */
/*     svc_c_debug5 -- messages announcing calls to local routines.         */
/*     svc_c_debug6 -- messages announcing entry/exit to/from server remote */
/*                     routines (e.g., remote bind, remote svc, sample in-  */
/*                     terface operations, etc.). This is also the level    */
/*                     for enabling announcements of library calls from     */
/*                     within these routines.                               */
/*     svc_c_debug7 -- messages announcing entry/exit to/from server local- */
/*                     ly called routines.                                  */
/*     svc_c_debug8 -- messages displaying various debugging information.   */
/*     svc_c_debug9 -- not used.                                            */
/*                                                                          */

int routes_G[MAX_ROUTES];

int debug_routes_G[MAX_LEVELS];

int debug_levels_G[MAX_LEVELS];


/* These two UUIDs could be treated as "well known": i.e. applications      */
/*   that use the same ACL manager for mgmt operations can use these...     */

uuid_t mgmt_acl_mgr_uuid = {/* 0060f928-bbf3-1d35-8d7d-0000c0d4de56         */
	0x0060f928, 0xbbf3, 0x1d35, 0x8d, 0x7d, 0x00, 0x00, 0xc0, 0xd4, 0xde, 0x56
};

uuid_t mgmt_object_uuid = {/* 00573b0e-bcc2-1d35-a73e-0000c0d4de56          */
	0x00573b0e, 0xbcc2, 0x1d35, 0xa7, 0xe3, 0x00, 0x00, 0xc0, 0xd4, 0xde, 0x56
};


/* These UUIDs are specific to this server...                               */
/* Some ACL UUIDs that will be globally used:                               */
uuid_t mgmt_acl_uuid;
uuid_t sample_acl_uuid;

/* The UUID of the sample ACL manager:                                      */
uuid_t sample_acl_mgr_uuid = { /* 001a15a9-3382-1d23-a16a-0000c0d4de56      */
	0x001a15a9, 0x3382, 0x1d23, 0xa1, 0x6a, 0x00, 0x00, 0xc0, 0xd4, 0xde, 0x56
};

/* A UUID for a sample object:                                              */
uuid_t sample_object_uuid = {/* 00415371-f29a-1d3d-b8c8-0000c0d4de56        */
	0x00415371, 0xf29a, 0x1d3d, 0xb8, 0xc8, 0x00, 0x00, 0xc0, 0xd4, 0xde, 0x56
};

/* The mgmt printstrings could be treated as standard for                   */
/*   a standard mgmt ACL manager...                                         */
sec_acl_printstring_t mgmt_info = {"mgmt", "Management Interface"};


/* Note that we don't need to use the unused bits here;                     */
/*  it's just less confusing this way...                                    */

sec_acl_printstring_t mgmt_printstr[] = {
	{ "i",	"m_inq_if",	mgmt_perm_inq_if       },
	{ "n",	"m_inq_pname",	mgmt_perm_inq_pname    },
	{ "s",	"m_inq_stats",	mgmt_perm_inq_stats    },
	{ "p",  "m_ping",       mgmt_perm_ping         },
	{ "k",  "m_kill",       mgmt_perm_kill         },
	{ "c",	"control",	sec_acl_perm_control   },
	{ "t",	"test",		sec_acl_perm_test      }
};

sec_acl_printstring_t sample_info = {"sample", "Sample RPC Program"};

sec_acl_printstring_t sample_printstr[] = {
	{ "r",	"read",		sec_acl_perm_read	},
	{ "w",	"write",	sec_acl_perm_write	},
	{ "d",	"delete",	sec_acl_perm_delete	},
	{ "c",	"control",	sec_acl_perm_control	},
	{ "t",	"test",		sec_acl_perm_test	},
	{ "x",	"execute",	sec_acl_perm_execute    }
};


/***************
 *
 *
 * main --
 *
 *
 *
 *
 **************/
int
main(
int argc,
char *argv[]
)
{

	unsigned32 status;   /* For status returned from library calls.     */
	rpc_binding_vector_t *binding_vector; /* For bindings from RPC run- */
					/*  time.                           */
	unsigned_char_t *string_binding; /* For string binding conversions. */
	unsigned_char_t *server_principal_name; /* Our server principal     */
					/*  name, read from command line.   */ 
	unsigned_char_t *uuid_string;   /* For UUID string conversions.     */
	entryname_vector_t entryname_vector; /* List of server entry names, */
					/*  read from command line.         */
	sec_login_handle_t login_context; /* Our login context, for server- */
					/*  assumed identity.               */
	pthread_t sigcatcher;           /* Handle to signal catcher thread. */
	uuid_vector_t server_uuid_v;    /* Array of server instance UUIDs.  */
				/* At present there is only one of these,   */
				/*  and it is generated dynamically at the  */
				/*  beginning of the program. This ends up  */
				/*  being used as an object UUID for the    */
				/*  server's exported bindings and regis-   */
				/*  tered mappings.                         */
	int i;                  /* Utility index variable.                  */
	int route_error;        /* Condition variable for setting up de-    */
				/*  fault serviceability routings. Not used */
				/*  yet, however.                           */
	uuid_t server_uuid;  /* The UUID that identifies this server in-    */
				/*  stance; i.e., the object UUID that      */
				/*  identifies our server's bindings. It is */
				/*  created by us, in create_server_uuid(), */
				/*  and goes into server_entry_ptr->id in   */
				/*  make_server_entry(). It actually ends   */
				/*  up being identical to the configuration */
				/*  UUID, if one is created (by calling the */
				/*  dced_server_create() routine).          */	
	dce_error_string_t error_string;  /* Used to directly retrieve er-  */
				/*  ror message strings in cases where the  */
				/*  serviceability call can't be used, be-  */
				/*  cause the svc table hasn't been regis-  */
				/*  tered yet.                              */
	int print_status;       /* Used to return status from the           */
				/*  dce_error_inq_text() routine.           */


	databases_open = FALSE;

	/* Process the command line...                                      */
	do_command_line(argc,
			argv,
			&server_principal_name,
			&entryname_vector);


	/* The following calls set up default routing of serviceability     */
	/*  messages. Note that these must be called before                 */
	/*  dce_svc_register()...                                           */
	for (i = 0, route_error = FALSE; (i < MAX_DEFAULT_ROUTES) && (!route_error); i++)
	{
		fprintf(stdout, "          Setting default route %s ...\n",
			default_routes[i]);
		dce_svc_routing(default_routes[i], &status);
		if (status != svc_s_ok)
		{
			print_server_error("dce_svc_routing(default_routes[i])",
						status);
			status = svc_s_ok;
		}
	}

	/* Set the default serviceability debug level and route...          */
	dce_svc_debug_routing(default_debug_route, &status);

	if (status != error_status_ok)
	{
		dce_error_inq_text(status, error_string, &print_status);
		fprintf(stdout, "dce_svc_routing(): %s\n", error_string);
/*		exit(1);    */
	}

	/* Get serviceability handle...                                     */
	smp_svc_handle = dce_svc_register(smp_svc_table,
				(idl_char*)"smp",
				&status);
	if (status != error_status_ok)
	{
		print_server_error("dce_svc_register()", status);
		exit(1);
	}

	/* Set up in-memory serviceability message table...                 */
	dce_msg_define_msg_table(smp__table,
				sizeof smp__table / sizeof smp__table[0],
				&status);
	if (status != error_status_ok)
	{
		print_server_error("dce_msg_define_msg_table()", status);
		exit(1);
	}

	dce_svc_printf(SIGN_ON_MSG);


	/* Create an object UUID for this server instance...                */
	create_server_uuid(&server_uuid, &server_uuid_v);

	/* Register the interface and get bindings...                       */

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Calling server_register_get_bindings()"));
	server_register_get_bindings(sample_v1_0_s_ifspec,
		&binding_vector,
		&status);
	if (status != error_status_ok)
	{
		print_server_error("server_register_get_bindings()",
					status);
		exit(1);
	}

	/* Register server authentication information...                    */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_server_register_auth_info()"));
	rpc_server_register_auth_info(server_principal_name,
		rpc_c_authn_dce_secret,
		NULL,
		KEYTAB,
		&status);
	if (status != error_status_ok)
	{
		print_server_error("rpc_server_register_auth_info()",
					status);
		exit(1);
	}

	/* Assume new identity...                                           */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Calling server_get_identity()"));
	server_get_identity(server_principal_name,
		&login_context,
		(unsigned_char_p_t)KEYTAB,
		&status);
	if (status != error_status_ok)
	{
		print_server_error("server_get_identity()", status);
		exit(1);
	}

	/* Spin off a thread to wait for signals...                         */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Spawning signal handler thread"));
	if (pthread_create(&sigcatcher,
		pthread_attr_default,
		(pthread_startroutine_t)signal_handler,
		(void*)0))
	{
		dce_svc_printf(NO_SIGNAL_CATCHER_MSG);
		exit(1);
	}

	/* Export objects to namespace and register them in end-            */
	/*  point map...                                                    */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Calling server_export_objects()"));
	server_export_objects(sample_v1_0_s_ifspec,
		binding_vector,
		&server_uuid_v,
		&entryname_vector,
		(unsigned_char_t *)IF_ANNOTATION,
		&status);
	if (status != error_status_ok)
	{
		print_server_error("server_export_objects()", status);
		goto CLEANUP_EXIT;
	}

	/* Register the remote binding interface...                         */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Calling sample_bind_export()"));
	sample_bind_export(binding_vector, &server_uuid_v, &status);
	if (status != error_status_ok)
	{
		print_server_error("sample_bind_export()", status);
		exit(1);
	}

	/* Create a default ACL manager...                                  */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Calling server_acl_mgr_setup()"));
	server_acl_mgr_setup(
			(unsigned_char_t *)ACL_DB_PATH, /* Pathname for     */
					/*  database files.                 */
			(dce_acl_resolve_func_t)sample_resolve_by_name,
					/* Our name->ACL UUID resolution    */
					/*  function.                       */
			sample_acl_mgr_uuid, /* UUID of our ACL manager;    */
					/*  hard-coded at top of this file. */
			sample_object_uuid,  /* UUID of our sample object;  */
					/*  hard-coded at top of this file. */
			(unsigned_char_t *)SAMPLE_OBJECT_NAME, /* Name of   */
					/*  our sample object.              */
			OBJ_OWNER_PERMS, /* Owner's permissions on sample   */
					/*  object.                         */
			(unsigned_char_t *)SAMPLE_OWNER,   /* Principal     */
					/*  name of sample object owner.    */
			0,              /* TRUE => object is a container.   */
			&db_acl,    /* Will contain ACL UUID store handle.  */
			&db_object, /* Will contain obj UUID store handle.  */
			&db_name,   /* Will contain name store handle.      */
			&sample_acl_uuid, /* Will contain object ACL UUID.  */
			&mgmt_acl_uuid,   /* Will contain mgmt ACL UUID.    */
			&status);
	if (status != error_status_ok)
	{
		print_server_error("server_acl_mgr_setup()", status);
		goto CLEANUP_EXIT;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_to_string()"));
	uuid_to_string(&mgmt_acl_uuid, &uuid_string, &status);
	if (status != uuid_s_ok)
	{
		print_server_error("uuid_to_string()", status);
		return(0);
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"String form of mgmt_acl_uuid == %s", uuid_string));
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_string_free()"));
	rpc_string_free(&uuid_string, &status);
	if (status != rpc_s_ok)
	{
		print_server_error("rpc_string_free()", status);
		return(0);
	}


	/* Register the remote ACL interface (at the endpoint map,  */
	/*  but not in the namespace)...                            */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Calling server_rdacl_export()"));
	server_rdacl_export(binding_vector, &server_uuid_v, &status);
	if (status != error_status_ok)
	{
		print_server_error("server_rdacl_export()", status);
		goto CLEANUP_EXIT;
	}


	/* Start listening for calls...                                     */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_server_listen()"));
	rpc_server_listen(rpc_c_listen_max_calls_default, &status);
	if (status != error_status_ok)
	{
		print_server_error("rpc_server_listen()", status);
/*		exit(1);     */
	}

CLEANUP_EXIT:

	/********************************************************************/
	/*                                                                  */
	/* Cleanup code -- Reached either because the server listen         */
	/*                 returned, or because we never got to listen in   */
	/*                 the first place due to some runtime error.       */
	/*                                                                  */
	/*                                                                  */
	/********************************************************************/

	/* Close the ACL databases...                                       */
	if (databases_open)
	{
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling server_acl_mgr_close()"));
		server_acl_mgr_close(&db_acl, &db_object, &db_name, &status);
		if (status != error_status_ok)
		{
			print_server_error("server_acl_mgr_close()", status);
		}
	}

	/* Be sure credentials are still valid before we try to             */
	/*  cleanup...                                                      */
	dce_svc_printf(CLEANUP_MSG);
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
		"Calling server_renew_identity()"));
	server_renew_identity(server_principal_name,
			login_context,
			(unsigned_char_p_t)KEYTAB,
			CLEANUPTIME,
			&status);
	if (status != error_status_ok)
	{
		print_server_error("server_renew_identity()", status);
	}

	/* Unexport server objects from namespace and from endpoint         */
	/*  map...                                                          */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
		"Calling server_cleanup_objects()"));
	server_cleanup_objects(sample_v1_0_s_ifspec,
			binding_vector,
			&server_uuid_v,
			&entryname_vector,
			&status);
	if (status != error_status_ok)
	{
		print_server_error("server_cleanup_objects()", status);
		exit(1);
	}

	/* Unregister the remote ACL interface from the endpoint            */
	/*  map...                                                          */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling server_rdacl_cleanup()"));
	server_rdacl_cleanup(binding_vector, &server_uuid_v, &status);
	if (status != error_status_ok)
	{
		print_server_error("server_rdacl_cleanup()", status);
	}

	/* Unregister the remote bind interface from the endpoint           */
	/*  map...                                                          */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling server_bind_cleanup()"));
	server_bind_cleanup(binding_vector, &server_uuid_v, &status);
	if (status != error_status_ok)
	{
		print_server_error("server_bind_cleanup()", status);
	}

	/* Print server exit message...                                     */
	dce_svc_printf(SERVER_EXIT_MSG);

}




/******
 *
 * do_command_line -- Get and interpret arguments and options from the
 *                    command line, and do other setup related to the
 *                    command line's contents.
 *
 *                    Returns 0 if normal invocation, 1 if setup.
 *
 *    Called from main().
 *
 ******/

int do_command_line(
int argc,
char *argv[],
unsigned_char_t **server_principal_name,
entryname_vector_t *entryname_vector
)
{
	dce_error_string_t error_string;
	int print_status;
	unsigned32 status;

	/* Note that the code expects you to type as the second argument    */
	/*  a slash-terminated full CDS directory name, to which it will    */
	/*  then concatenate the entryname. It is this name that is then    */
	/*  passed to the server_export_objects() routine later on.         */

	/* Check the command line...                                        */
	if ( (argc == 2) && (( strcmp(argv[1], "setup") == 0) || ( strcmp(argv[1], "unsetup") == 0)) )
		return 1;

	else if (argc < 3)
	{
		fprintf(stdout, "\n Usage:\n");
		fprintf(stdout, "       %s <principal_name> <CDS_dir_name>/\n\n", argv[0]);
		exit(1);
	}

	/* Get the server's principal name from the command line...         */
	*server_principal_name = (unsigned_char_p_t)malloc(strlen(argv[1]));
	strcpy((char *)*server_principal_name, (char *)argv[1]);

	/* Get the list of server entry names from the command line...      */
	entryname_vector->count = 1;
	entryname_vector->name[0] = (unsigned_char_p_t)malloc(strlen(argv[2]) + NAMELEN);
	strcpy((char *)entryname_vector->name[0], argv[2]);
	strcat((char *)entryname_vector->name[0], DEFNAME);

	/* Set the program name for serviceability messages...              */
	dce_svc_set_progname(argv[0], &status);
	if (status != error_status_ok)
	{
		dce_error_inq_text(status, error_string, &print_status);
		fprintf(stdout, "dce_svc_set_progname(): %s\n", error_string);
		exit(1);
	}

	return 0;
}




/******
 *
 * server_register_get_bindings --  Register an interface:
 * 
 *         Set up only one type manager
 *         Use all protocol sequences
 *         Return the bindings
 *
 *    Called from main().
 * 
 ******/

void server_register_get_bindings(
rpc_if_handle_t interface,              /* Interface to register.           */
rpc_binding_vector_t **binding_vector,  /* To return bindings.              */
unsigned32 *status)                     /* To return status.                */
{
	unsigned_char_t *string_binding;
	int i;

	*status = error_status_ok;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_register_get_bindings()"));

	/* Register the default interface, default epv, and nil type        */
	/*  UUID...                                                         */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_server_register_if()"));
	rpc_server_register_if(interface, NULL, NULL, status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_server_register_if()", *status);
		return;
	}

	/* Use all available protocol sequences...                          */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_server_use_all_protseqs()"));
	rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default,
			status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_server_use_all_protseqs()", *status);
		return;
	}

	/* Get the binding handles generated by the runtime...              */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_server_inq_bindings()"));
	rpc_server_inq_bindings(binding_vector, status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_server_inq_bindings()", *status);
		return;
	}

	/********************************************************************
	*
	* The following shows how to convert a vector of bindings into
	*    string bindings, and to print them out...
	*
	*
	*****/

	dce_svc_printf(BINDINGS_RECEIVED_MSG, "sample",
			(**binding_vector).count);

	for (i = 0; i < (**binding_vector).count; i++)
	{
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling rpc_binding_to_string_binding()"));
		rpc_binding_to_string_binding((**binding_vector).binding_h[i],
					&string_binding,
					status);
		if (*status != rpc_s_ok)
		{
			print_server_error("rpc_binding_to_string_binding()",
					*status);
			exit(1);
		}

		dce_svc_printf(FULL_BINDING_MSG, "sample", string_binding);

		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling rpc_string_free()"));
		rpc_string_free(&string_binding, status);
		if (*status != rpc_s_ok)
		{
			print_server_error("rpc_string_free()", *status);
			exit(1);
		}

	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_register_get_bindings()"));

}


/******
 *
 *
 * server_export_objects -- Set up a simple object-based binding scheme
 *                          for the server, i.e.:
 * 
 *          Register bindings and objects in the endpoint map,
 *          Then export each object to a separate name space entry.
 * 
 *    The function uses a vector of entry names that correspond one-to-one
 *     with the objects in the object uuid vector. The server must have
 *     export permission to CDS in order to successfully execute this
 *     function.
 * 
 *    Called from main().
 *
 ******/
void server_export_objects(
rpc_if_handle_t interface,             /* The interface specification.      */
rpc_binding_vector_t *binding_vector,  /* The server's binding handles.     */  
uuid_vector_t *object_uuid_vector,     /* Server instance UUID, created in  */
				       /*  main.                            */
entryname_vector_t *entryname_vector,  /* Server entry names, from command  */
				       /*  line.                            */
unsigned_char_t *annotation,           /* Annotation string for endpoint    */
				       /*  map entry.                       */
unsigned32 *status)                    /* To return status                  */
{


	uuid_vector_t object_uuid; /* Used to hold object UUIDs to be       */
				/* passed to rpc_ns_binding_export().       */
	int i;                     /* Index variable.                       */

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_export_objects()"));

	*status = error_status_ok;
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_ep_register()"));
	rpc_ep_register(interface, binding_vector, object_uuid_vector,
			annotation, status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_ep_register()", *status);
		return;
	}

	if (object_uuid_vector)
	{
		if (entryname_vector->count != object_uuid_vector->count)
		{
			dce_svc_printf(BAD_ENTRYNAME_COUNT_MSG);
			return;
		}

		object_uuid.count = 1;

		/* Export objects one at a time to CDS entries... */
		for (i = 0; i < entryname_vector->count; i++)
		{
			dce_svc_printf(EXPORTING_TO_MSG, entryname_vector->name[i]);
			object_uuid.uuid[0] = object_uuid_vector->uuid[i];
			DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
					"Calling rpc_ns_binding_export()"));
			rpc_ns_binding_export(rpc_c_ns_syntax_default,
					entryname_vector->name[i],
					interface,
					binding_vector,
					(uuid_vector_t*)&object_uuid,
					status);
			if (*status != error_status_ok)
			{
				print_server_error("rpc_ns_binding_export()",
						*status);
				return;
			}
		}
	}
	else
	{

		dce_svc_printf(EXPORTING_TO_MSG, entryname_vector->name[0]);
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling rpc_ns_binding_export()"));
		rpc_ns_binding_export(rpc_c_ns_syntax_default,
				entryname_vector->name[0],
				interface,
				binding_vector,
				NULL,
				status);
		if (*status != error_status_ok)
		{
			print_server_error("rpc_ns_binding_export()",
					*status);
			return;
		}
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_export_objects()"));

}



/******
 *
 * server_cleanup_objects -- Unexport and unregister all server objects.
 * 
 *      The server must have valid credentials for this routine to
 *       successfully execute.
 * 
 *    Called from main().
 *
 ******/

void server_cleanup_objects(
rpc_if_handle_t interface,             /* Interface to unregister.          */
rpc_binding_vector_t *binding_vector,  /* Server bindings to delete.        */
uuid_vector_t *object_uuid_vector,     /* Server instance UUID(s).          */
entryname_vector_t *entryname_vector,  /* Server entry names.               */
unsigned32 *status)                    /* To return status.                 */
{

	struct {
		unsigned32 count;
		uuid_t *uuid[1];
	} object_uuid;

	int i;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_cleanup_objects()"));

	*status = error_status_ok;

	/* Get rid of the endpoints...                                      */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_ep_unregister()"));
	rpc_ep_unregister(interface, binding_vector, object_uuid_vector,
			status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_ep_unregister()", *status);
		return;
	}

	/* Get rid of the server instance UUID(s). However, note that at    */
	/*  present there is only one of these, and it's hard-coded below.  */
	if (object_uuid_vector)
	{
		if (entryname_vector->count != object_uuid_vector->count)
		{
			dce_svc_printf(BAD_ENTRYNAME_COUNT_MSG);
			return;
		}
		object_uuid.count = 1;

		/* Unexport objects one at a time from CDS entries... */
		for (i = 0; i < entryname_vector->count; i++)
		{
			dce_svc_printf(UNEXPORTING_FROM_MSG,
				entryname_vector->name[i]);

			object_uuid.uuid[0] = object_uuid_vector->uuid[i];
			DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
					"Calling rpc_ns_binding_unexport()"));
			rpc_ns_binding_unexport(rpc_c_ns_syntax_default,
					entryname_vector->name[i],
					interface,
					(uuid_vector_t*)&object_uuid,
					status);
			if (*status != error_status_ok)
			{
				print_server_error("rpc_ns_binding_unexport()",
						*status);
				return;
			}
		}
	}
	else
	/* I.e., there is only one server instance to unexport...           */ 
	{
		dce_svc_printf(UNEXPORTING_FROM_MSG, entryname_vector->name[0]);
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling rpc_ns_binding_unexport()"));
		rpc_ns_binding_unexport(rpc_c_ns_syntax_default,
				entryname_vector->name[0],
				interface,
				NULL,
				status);
		if (*status != error_status_ok)
		{
			print_server_error("rpc_ns_binding_unexport()",
					*status);
			return;
		}
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_cleanup_objects()"));

}



/******
 *
 * managekey -- Make sure the server principal's key is changed before
 *              it expires.
 * 
 *      The key management thread which runs this function is created
 *       in server_get_identity(), below.
 *
 *
 ******/

void managekey(char *prin_name){          /* Server principal name          */
	unsigned32 status;

	status = error_status_ok;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering managekey()"));

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sec_key_mgmt_manage_key()"));
	sec_key_mgmt_manage_key(
		rpc_c_authn_dce_secret,   /* Authentication protocol.       */
		KEYTAB,                   /* Local key file.                */
		(idl_char *)prin_name,    /* Principal name.                */
		&status);
	if (status != error_status_ok)
		print_server_error("sec_key_mgmt_manage_key()", status);

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting managekey()"));

}




/******
 *
 * server_get_identity -- Establish a new server identity with valid
 *                        credentials. This includes setting up a key
 *                        management thread.
 * 
 * 
 *    Called from main().
 *
 ******/

void server_get_identity(
unsigned_char_p_t prin_name,            /* Server principal name.           */
sec_login_handle_t *login_context,      /* Returns server's login context.  */
unsigned_char_p_t keytab,               /* Local key file.                  */
unsigned32 *status)
{

	pthread_t keymgr;
	sec_passwd_rec_t *keydata;
	sec_login_auth_src_t auth_src;
	boolean32 reset_pwd;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_get_identity()"));

	*status = error_status_ok;

	/* Spin off thread to manage key for specified principal...         */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling pthread_create()"));
	if (pthread_create(&keymgr,            /* Thread handle.            */
			pthread_attr_default,  /* Specifies default thread  */
						/*  attributes.             */
			(pthread_startroutine_t)managekey, /* Start rou-    */
						/*  tine; see above.        */
			(void*)prin_name))     /* Argument to pass to start */
					       /*  routine: server princi-  */
					       /*  pal name.                */
	{
		dce_svc_printf(CANNOT_MANAGE_KEYS_MSG);
		return;
	}

	/* Create a context and get the login context...                    */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sec_login_setup_identity()"));
	sec_login_setup_identity(prin_name,
		sec_login_no_flags,
		login_context,
		status);
	if (*status != error_status_ok)
	{
		print_server_error("sec_login_setup_identity()", *status);
		return;
	}

	/* Get secret key from the keytab file...                           */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sec_key_mgmt_get_key()"));
	sec_key_mgmt_get_key(rpc_c_authn_dce_secret,
		keytab,
		prin_name,
		0,
		(void**)&keydata,
		status);
	if (*status != error_status_ok)
	{
		print_server_error("sec_key_mgmt_get_key()", *status);
		return;
	}

	/* Validate the login context...                                    */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sec_login_validate_identity()"));
	sec_login_validate_identity(*login_context,
		keydata,
		&reset_pwd,
		&auth_src,
		status);
	if (*status != error_status_ok)
	{
		print_server_error("sec_login_validate_identity()", *status);
		return;
	}

	/* Finally, set the context...                                      */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sec_login_set_context()"));
	sec_login_set_context(*login_context, status);
	if (*status != error_status_ok)
	{
		print_server_error("sec_login_set_context()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_get_identity()"));

}


/******
 *
 * server_renew_identity -- Make sure that credentials are still valid, and
 *                          enew them if they are not.
 * 
 * 
 *     This routine is called (with the current credentials) whenever a task
 *     is about to be attempted that requires valid credentials. For an ex-
 *     ample, see the cleanup code in "main()" above. A valid credential will
 *     nevertheless be considered invalid if it will expire within time_left
 *     seconds. This gives a margin of time between the validity check that
 *     occurs here and the actual use of the credential.
 *
 *    Called from main() (but can be called from elsewhere).
 *
 ******/

void server_renew_identity(
unsigned_char_p_t prin_name,      /* Server's principal name.               */
sec_login_handle_t login_context, /* Server's login context.                */
unsigned_char_p_t keytab,         /* Local key file.                        */
unsigned32 time_left,             /* Amount of "margin" -- see above.       */
unsigned32 *status)               /* To return status.                      */
{
	signed32 expiration;
	time_t current_time;
	sec_passwd_rec_t *keydata;
	sec_login_auth_src_t auth_src;
	boolean32 reset_pwd;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_renew_identity()"));

	*status = error_status_ok;

	/* Get the lifetime for the server's Ticket-Granting-Ticket (TGT).  */
	/* Note that sec_login_get_expiration() returns a non-zero          */
	/*  status for an uncertified login context. This is not            */
	/*  an error. Hence the special error checking...                   */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sec_login_get_expiration()"));
	sec_login_get_expiration(login_context,
		&expiration,
		status);
	if (*status != sec_login_s_not_certified)
	{
		print_server_error("sec_login_validate_identity()", *status);
		return;
	}

	/* Get current time...                                              */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling time()"));
	time(&current_time);

	/* Now, if the expiration time is sooner than the desired "time     */
	/*  left"...                                                        */
	if (expiration < (current_time + time_left))
	{
		/* Refresh the server's authenticated identity...           */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling sec_login_refresh_identity()"));
		sec_login_refresh_identity(login_context,
		    status);
		if (*status != error_status_ok)
		{
			print_server_error("sec_login_refresh_identity()", *status);
			return;
		}

		/* Get key from local file...                               */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling sec_key_mgmt_get_key()"));
		sec_key_mgmt_get_key(rpc_c_authn_dce_secret,
			keytab,
			prin_name,
			0,
			(void**)&keydata,
			status);
		if (*status != error_status_ok)
		{
			print_server_error("sec_key_mgmt_get_key()", *status);
			return;
		}

		/* Validate the login context...                            */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling sec_login_validate_identity()"));
		sec_login_validate_identity(login_context,
			keydata,
			&reset_pwd,
			&auth_src,
			status);
		if (*status != error_status_ok)
		{
			print_server_error("sec_login_validate_identity()", *status);
			return;
		}
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Successfully exiting server_renew_identity()"));

}


/******
 * 
 * server_create_dflt_acl -- Create a default ACL; i.e., get the initial
 *                           container ACL and copy it (instead of con-
 *                           structing an ACL whole, as below), and create
 *                           a UUID for the ACL.
 * 
 *    Not called from anywhere. NOT YET TESTED.
 * 
 ******/

void server_create_dflt_acl(
		dce_db_handle_t db_acl,           /* Backing store handle.  */
		unsigned_char_t *container,  /* Object we want the ACL of.  */
		void (*resolver)(),  /* ACL name-to-UUID resolver function; */
					/*  i.e., sample_resolve_by_name(). */
		boolean32 is_container,      /* Is the object a container?  */
		sec_acl_t *acl,              /* ACL will be returned here.  */
		uuid_t *acl_uuid,     /* ACL's UUID will be returned here.  */
		unsigned32 *status)
{

	sec_acl_type_t sec_acl_type;     /* To contain ACL type specifier.  */
	uuid_t iacl_uuid;                /* To contain initial container    */
						/*  ACL's UUID.             */

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_create_dflt_acl()"));

	/* Create the UUID for the new ACL...                               */
	*status = error_status_ok;
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_create()"));
	uuid_create(acl_uuid, status);
	if (*status != error_status_ok)
	{
		print_server_error("uuid_create()", *status);
		return;
	}

	if (is_container)
		sec_acl_type = 2;
	else
		sec_acl_type = 1;

	/* Now get the initial container's ACL UUID.                        */
	/* This is a call to sample_resolve_by_name(); see below...         */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Calling (*resolver)()"));
	(*resolver)(NULL,  /* No client binding handle; this isn't a re-    */
				/* mote call.                               */
			container, /* The object whose ACL's UUID we want;  */
				/* here, the initial container.             */
			sec_acl_type, /* Type of ACL we want UUID for.      */
			NULL, /* No manager type specified.                 */
			0,    /* Dummy parameter for us.                    */
			NULL, /* No need to specify a special backing       */
				/*  store handle.                           */
			&iacl_uuid,  /* Initial container ACL's UUID is re- */
				/*  turned here.                            */
			status);
	if (*status != error_status_ok)
	{
		print_server_error("resolver function(*)", *status);
		return;
	}

	/* Now get the initial container ACL...                             */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_fetch_by_uuid()"));
	dce_db_fetch_by_uuid(db_acl,          /* ACL UUID-indexed database. */
			&iacl_uuid,      /* The initial container ACL UUID. */
			acl,                  /* The ACL is returned here.  */
			status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_fetch_by_uuid()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_create_dflt_acl()"));

}




/******
 *
 * server_get_local_principal_id -- Get (from the local cell registry) the
 *                                  UUID corresponding to a principal name.
 *
 * 
 *    Called from server_create_acl() and server_acl_mgr_setup().
 * 
 ******/

void server_get_local_principal_id(
		unsigned_char_t *p_name,    /* Simple principal name.       */
		uuid_t *p_id,               /* UUID returned here.          */
		unsigned32 *status)         /* Status returned here.        */
{

	char *cell_name;                    /* For local cell name.         */
	sec_rgy_handle_t rhandle;           /* For registry server handle.  */

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_get_local_principal_id()"));

	/* First, get the local cell name...                                */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_cf_get_cell_name()"));
	dce_cf_get_cell_name(&cell_name, status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_cf_get_cell_name()", *status);
		return;
	}

	/* Now bind to the cell's registry...                               */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sec_rgy_site_open()"));
	sec_rgy_site_open(cell_name, &rhandle, status);
	if (*status != error_status_ok)
	{
		print_server_error("sec_rgy_site_open()", *status);
		return;
	}

	/* Free the string space we got the cell name in...                 */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling free()"));
	free(cell_name);

	/* Now get from the registry the UUID associated with the principal */
	/*  name we got in the first place...                               */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sec_rgy_pgo_name_to_id()"));
	sec_rgy_pgo_name_to_id(rhandle,
			sec_rgy_domain_person,
			p_name,
			p_id,
			status);
	if (*status != error_status_ok)
	{
		print_server_error("sec_rgy_pgo_name_to_id()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_get_local_principal_id()"));

}



/******
 * 
 * server_create_acl -- Create an ACL with some specified set of permissions
 *                      assigned to some principal user.  
 *
 *
 *    Called from server_acl_mgr_setup().
 * 
 ******/

void server_create_acl(
		uuid_t mgr_type_uuid,     /* Manager type of ACL to create. */
		sec_acl_permset_t perms,  /* Permission set for ACL.        */
		unsigned_char_t *user,    /* Principal name for new entry.  */
		sec_acl_t *acl,           /* To return the ACL entry in.    */
		uuid_t *acl_uuid,         /* To return the ACL's UUID in.   */
		unsigned32 *status)       /* To return status in.           */
{

	static uuid_t u;  /* For the principal's UUID (from the registry).  */

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_create_acl()"));

	*status = error_status_ok;

	/* Create a UUID for the ACL...                                     */
	/* Note that the new UUID doesn't get associated with the entry in  */
	/*  this routine. It must happen in server_acl_mgr_setup()...       */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_create()"));
	uuid_create(acl_uuid, status);
	if (*status != error_status_ok)
	{
		print_server_error("uuid_create()", *status);
		return;
	}

	/* Create an initial ACL object with default permissions for the    */
	/*  designated user principal identity...                           */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_acl_obj_init()"));
	dce_acl_obj_init(&mgr_type_uuid, acl, status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_acl_obj_init()", *status);
		return;
	}

	/* Get the specified principal's UUID...                            */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
			"Calling server_get_local_principal_id()"));
	server_get_local_principal_id(user, &u, status);
	if (*status != error_status_ok)
	{
		print_server_error("server_get_local_principal_id()", *status);
		return;
	}

	/* Now add the user ACL entry to the ACL...                         */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_acl_obj_add_user_entry()"));
	dce_acl_obj_add_user_entry(acl, perms, &u, status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_acl_obj_add_user_entry()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_create_acl()"));

}


/******
 * 
 * server_store_acl -- Store ACL-related data.
 * 
 * 
 *      The data is stored in databases that support a
 *      name->object_uuid->acl_uuid style of ACL lookup.
 *
 * 
 *    Called from server_acl_mgr_setup().
 *
 ******/
/****************************************************************************/
/*                                                                          */
/* There are three databases (this can be seen also in the                  */
/*  server_acl_mgr_close() routine):                                        */
/*                                                                          */
/*   db_acl:ACL (UUID)-indexed:         (used to store the ACLs themselves) */
/*   db_object:Object (UUID)-indexed:   (used to store the object data      */
/*                                       itself)                            */
/*   db_name:Name ("Residual")-indexed: (used to store the simple names of  */
/*                                       the objects)                       */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

void server_store_acl(
		dce_db_handle_t db_acl,     /* ACL (UUID)-indexed store.    */
		dce_db_handle_t db_object,  /* Object (UUID)-indexed store. */
		dce_db_handle_t db_name,    /* Name-indexed store.          */
		sec_acl_t *acl,             /* The ACL itself.              */
		uuid_t *acl_uuid,           /* ACL UUID.                    */
		uuid_t *object_uuid,        /* Object UUID.                 */
		unsigned_char_t *object_name, /* The name of the object.    */
		void *object_contents,  /* The actual object data contents. */
					/* NOTE: NOT USED NOW.              */
		boolean32 is_container,  /* Are we storing a container ACL? */
		unsigned32 *status)         /* To return status.            */
{

	/* These two variables are used to hold UUIDs for the ACLs we will  */
	/*  need to create if we have a container ACL on our hands...       */
	static uuid_t def_object, def_container;
	static sample_data_t object_data;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_store_acl()"));

	*status = error_status_ok;

	/* Null the contents of the object_data variable...                 */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling bzero()"));
	bzero((char *)&object_data, sizeof object_data);

	/* If we have a container ACL, then we have to create and store the */
	/*  special stuff associated with it-- namely, the container ACL    */
	/*  itself, and a default object ACL to go with it...               */
	if (is_container)
	{
		/* Create a UUID for the default object ACL...              */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling uuid_create()"));
		uuid_create(&def_object, status);
		if (*status != error_status_ok)
		{
			print_server_error("uuid_create()", *status);
			return;
		}
		/* Create a UUID for the default container ACL...           */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling uuid_create()"));
		uuid_create(&def_container, status);
		if (*status != error_status_ok)
		{
			print_server_error("uuid_create()", *status);
			return;
		}

		/* Store the default object ACL into UUID-indexed store...  */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling dce_db_store_by_uuid()"));
		dce_db_store_by_uuid(db_acl, &def_object, acl, status);
		if (*status != error_status_ok)
		{
			print_server_error("dce_db_store_by_uuid()", *status);
			return;
		}

		/* Store the default container ACL into UUID-indexed        */
		/*  store...                                                */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling dce_db_store_by_uuid()"));
		dce_db_store_by_uuid(db_acl, &def_container, acl, status);
		if (*status != error_status_ok)
		{
			print_server_error("dce_db_store_by_uuid()", *status);
			return;
		}

        }

	/* Store the plain object ACL into ACL UUID-indexed store...        */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_store_by_uuid()"));
	dce_db_store_by_uuid(db_acl, acl_uuid, acl, status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_store_by_uuid()", *status);
		return;
	}

	/* Store the ACL UUID(s) into a standard object header...           */

	/* Observe how this all seems to work: the following call sets up   */
	/*  some general info in the object-indexed database that is asso-  */
	/*  ciated with the Object and ACL UUIDs passed. This is where the  */
	/*  ACL on an object actually gets connected with that object. Up   */
	/*  above the information connected with the ACL UUID was stored    */
	/*  in a sec_acl_t structure, but it's the following call that ac-  */
	/*  tually associates this structure with some object. Afterward    */
	/*  the object data itself (what is being "ACL'd") gets stored via  */
	/*  the dce_db_store_by_uuid() call, and finally the object UUID    */
	/*  itself is stored by name via the dce_db_store_by_name() call.   */
	/*  So the reverse process (beginning with the name) will be:       */
	/*                                                                  */
	/*    1. Look up the object UUID by name by calling                 */
	/*       dce_db_fetch_by_name().                                    */
	/*                                                                  */
	/*    2. Look up the data (i.e., object data) for the object        */
	/*       by calling dce_db_fetch_by_uuid().                         */
	/*                                                                  */
	/*    3. Extract the ACL UUID from the correct field in the object  */
	/*       data structure.                                            */
	/*                                                                  */
	/*  ...These steps can be seen in sample_resolve_by_name(), the     */
	/*  purpose of which is to return an ACL UUID when given an object  */
	/*  name; the permission lists in effect for the object can then be */
	/*  accessed and checked against some set of permissions presented  */
	/*  by a prospective accessor.                                      */
	/*                                                                  */
	/* Once the ACL library has gotten from us the UUID that identifies */
	/*  the ACL on the object it wants to investigate the permissions   */
	/*  on, it's up to it to go on to retrieve the ACL itself, using    */
	/*  the UUID to do so. It is able to do this because we have reg-   */
	/*  istered our ACL database via the dce_acl_register_object_type() */
	/*  call (this is also, by the way, where our *(resolver)() routine */
	/*  is registered). So the runtime can extract the ACL information, */
	/*  compare it with the permissions presented by the entity that's  */
	/*  trying to access the object in question, and allow, or not al-  */
	/*  low, the operation to proceed accordingly.                      */
	/*                                                                  */
	/* The way to test whether our sample ACLs have been set up cor-    */
	/*  rectly or not would be to try to do various things to them via  */
	/*  acl_edit.                                                       */
	/*                                                                  */
	/* Note that the registration procedures described here are only to */
	/*  set up an application's ACL manager so that it is accessible    */
	/*  via acl_edit (and, I suppose, dced and dcecp). In situations    */
	/*  where a client in contact with the application server itself is */
	/*  trying to perform some operation, it is the responsibility of   */
	/*  the application code itself to check the client's authorization */
	/*  and make the correct decision as to access. Note though that it */
	/*  does this through the dce_acl_is_client_authorized() call,      */
	/*  which again can work only if you have correctly registered the  */
	/*  application's manager. For an example of using this call see    */
	/*  the sample_call() code in sample_manager.c; it is also called   */
	/*  by sample_mgmt_auth(), below in this file.                      */
	/*                                                                  */
	/*                                                                  */
	/* To sum up, then, there are basically three avenues of access     */
	/*  that an application has to provide for when setting up an ACL   */
	/*  manager:                                                        */
	/*                                                                  */
	/*      1. Access by clients to the server via the remote mgmt      */
	/*         interface. This is handled by setting up a mgmt call-    */
	/*         back routine that will be automatically invoked by the   */
	/*         runtime whenever a remote mgmt access is attempted. Our  */
	/*         callback routine is sample_mgmt_auth(), below, and it is */
	/*         registered by a call to rpc_mgmt_set_authorization().    */
	/*                                                                  */
	/*      2. Access by entities of any kind via acl_edit. This is     */
	/*         handled by the mechanisms described above, which are     */
	/*         set up by the call to dce_acl_register_object_type().    */
	/*                                                                  */
	/*      3. Access by clients in contact with the server. This is    */
	/*         handled by the server code itself, as described above.   */
	/*                                                                  */
	/*                                                                  */
	/*                                                                  */
	/* Note that the use of the three databases given here is necessar- */
	/*  ily true only of the ACL databases we are setting up here. The  */
	/*  object data stored in databases is strictly up to the applica-  */
	/*  tion; that is why this parameter is defined as (void *).        */
	/*  In other words, the backing store library can be used for any-  */
	/*  thing.                                                          */
	/*                                                                  */
	/*  In the sample_db.idl file can be seen the object data type de-  */
	/*  fined for this sample application, which is stored in the ob-   */
	/*  ject UUID-indexed database...                                   */


	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_std_header_init()"));
	dce_db_std_header_init(
			db_object,      /* Object database.         */
			&(object_data.s_hdr), /* Object data hdr.   */
			object_uuid,    /* Object UUID.             */
			acl_uuid,       /* ACL UUID.                */
			&def_object,    /* Default object ACL.      */
			&def_container, /* Default container ACL.   */
			0,              /* Reference count.         */
			status);

	if (*status != error_status_ok)
	{
		print_server_error("dce_db_std_header_init()", *status);
		return;
	}

	/* Now store the object data keyed by object UUID...                */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_store_by_uuid()"));


	/* This is perhaps a not very nice way to do this, but it will have */
	/*  to serve for the time being...                                  */
	if (strcmp((char *)object_name, SAMPLE_OBJECT_NAME) == 0)
		strcpy((char *)object_data.s_data.message,
			"THIS IS AN OFFICIAL SAMPLE OBJECT TEXT!");
	else if (strcmp((char *)object_name, MGMT_OBJ_NAME) == 0)
		strcpy((char *)object_data.s_data.message,
			"THIS IS AN OFFICIAL MGMT OBJECT SAMPLE TEXT!");
	else
		strcpy((char *)object_data.s_data.message,
			"I DON'T KNOW WHAT THIS IS!");

	dce_db_store_by_uuid(db_object,
				object_uuid,
				(void *)&object_data,
				status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_store_by_uuid()", *status);
		return;
	}


	/* Finally, store the object UUID keyed by the object ("residual")  */
	/*  name...                                                         */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_store_by_name(%s)", object_name));
	dce_db_store_by_name(db_name, (char *)object_name, object_uuid, status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_store_by_name()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_store_acl()"));

}


/******
 *
 * server_acl_mgr_setup -- Open and, if necessary, create the ACL-related
 *                         databases, i.e.:
 * 
 *         1. Set up a default ACL manager for the management interface.
 * 
 *         2. Create an initial ACL. For servers that dynamically create
 *            objects, this ACL is intended to be used as the ACL on the
 *            "container" in which objects are created. If the server
 *            manages static objects, this ACL can be used for some other
 *            purpose.
 * 
 *            Note that all the container-related code is actually in the
 *            server_store_acl() routine above.
 *
 *    Called from main().
 *
 ******/

void server_acl_mgr_setup(
		unsigned_char_t *db_acl_path,    /* Pathname for databases. */
		dce_acl_resolve_func_t resolver, /* sample_resolve_by_name. */
		uuid_t acl_mgr_uuid,             /* ACL manager UUID.       */
		uuid_t object_uuid,              /* Object UUID.            */
		unsigned_char_t *object_name,    /* Object name.            */
		sec_acl_permset_t owner_perms,   /* Owner permission set.   */
		unsigned_char_t *owner,          /* Owner name.             */
		boolean32 is_container,      /* Is this a container object? */
						/* == TRUE from main().     */
	/* [out] parameters: */
		dce_db_handle_t *db_acl,    /* ACL-indexed store handle.    */
		dce_db_handle_t *db_object, /* Object-indexed store handle. */
		dce_db_handle_t *db_name,   /* Name-indexed store handle.   */
		uuid_t *object_acl_uuid,    /* Object ACL UUID.             */
		uuid_t *mgmt_acl_uuid,      /* Mgmt ACL UUID.               */
		unsigned32 *status)
{

	uuid_t machine_princ_id;
	unsigned_char_t machine_principal[MAXHOSTNAMELEN + 20];
	unsigned_char_t *uuid_string;
	sec_acl_t *new_obj_acl, *new_mgmt_acl;
	boolean32 need_init;        
	unsigned32 dbflags;
	static sample_data_t object_data;
	unsigned_char_t *acl_path_string;
	sec_acl_permset_t permset = (sec_acl_permset_t) 0;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_acl_mgr_setup()"));

	new_obj_acl = (sec_acl_t *)malloc(sizeof(sec_acl_t));
	new_mgmt_acl = (sec_acl_t *)malloc(sizeof(sec_acl_t));

	*status = error_status_ok;
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling bzero()"));
	bzero((char *)&object_data, sizeof object_data);

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_create_nil()"));
	uuid_create_nil(object_acl_uuid, status);
	if (*status != error_status_ok)
	{
		print_server_error("uuid_create_nil()", *status);
		return;
	}

	need_init = 0;

	/* Build the full pathname string for the db_acl database...        */
	acl_path_string = malloc(MAX_ACL_PATH_SIZE);
	strcpy((char *)acl_path_string, (char *)db_acl_path);
	strcat((char *)acl_path_string, (char *)"/");
	strncat((char *)acl_path_string, "db_acl", strlen("db_acl"));

	/* Find out if the database already exists...                       */
	if (access((char *)acl_path_string, R_OK) != 0)
		if (errno == ENOENT)
			need_init = 1;

	/********************************************************************/

	/* Create the indexed-by-UUID databases. There are two of these:    */
	/*        One for the ACL UUID-indexed store, and                   */
	/*        One for the Object UUID-indexed store...                  */

	dbflags = db_c_index_by_uuid;

	/* If the thing doesn't exist yet, then we need to do some init-    */
	/*  ialization...                                                   */
	if (need_init)
		dbflags |= db_c_create;

	/* Open (or create) the "db_acl" ACL UUID-indexed backing store.    */
	/*  Note that no header type is specified among the dbflags, so the */
	/*  database will be created with no header-- that's the default... */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_open()"));

	dce_db_open(
		(char *)acl_path_string,  /* Filename of backing store.     */
		NULL,      /* Backing store "backend type" default == hash. */
		dbflags,   /* We already specified index by UUID for this.  */
		(dce_db_convert_func_t)dce_rdacl_convert,  /* Serialization */
				/* function (generated by IDL).             */
		db_acl,    /* The returned backing store handle.            */
		status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_open()", *status);
		free(acl_path_string);
		return;
	}
	/* Set the global variable that records whether we actually have    */
	/*  opened the databases; this enables us to avoid calling the      */
	/*  dce_db_close() routine for unopened databases, which will cause */
	/*  a core dump...                                                  */
	databases_open = TRUE;

	/* For the object database, we need standard backing store headers  */
	/*  to hold UUIDs for all the various ACLs...                       */
	dbflags |= db_c_std_header;
	if (need_init)
		dbflags |= db_c_create;

	/* Now open (or create) the "db_object" store... */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_open()"));

	/* Build the full pathname string for the database...               */
	free(acl_path_string);
	acl_path_string = malloc(MAX_ACL_PATH_SIZE);
	strcpy((char *)acl_path_string, (char *)db_acl_path);
	strcat((char *)acl_path_string, (char *)"/");
	strncat((char *)acl_path_string, "db_object", strlen("db_object"));

	dce_db_open(
		(char *)acl_path_string, /* Filename of backing store.      */
		NULL,      /* Backing store "backend type" default == hash. */
		dbflags,   /* Specifies index by UUID, and include standard */
				/* headers.                                 */
		(dce_db_convert_func_t)sample_data_convert, /* Serializa-   */
				/* tion function for object data.           */
		db_object,    /* The returned backing store handle.         */
		status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_open()", *status);
		free(acl_path_string);
		return;
	}


	/* Create the indexed-by-name database...                           */

	dbflags = db_c_index_by_name;
	if (need_init)
		dbflags |= db_c_create;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_open()"));

	/* Build the full pathname string for the database...               */
	free(acl_path_string);
	acl_path_string = malloc(MAX_ACL_PATH_SIZE);
	strcpy((char *)acl_path_string, (char *)db_acl_path);
	strcat((char *)acl_path_string, (char *)"/");
	strncat((char *)acl_path_string, "db_name", strlen("db_name"));

	dce_db_open(
		(char *)acl_path_string,  /* Filename of backing store.     */
		NULL,      /* Backing store "backend type" default == hash. */
		dbflags,   /* Specifies index by name.                      */
		(dce_db_convert_func_t)uu_convert, /* Serialization func-   */
				/* tion for name data.                      */
		db_name,   /* The returned backing store handle.            */
		status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_open()", *status);
		free(acl_path_string);
		return;
	}
	free(acl_path_string);

	/********************************************************************/

	/* Now register our ACL manager's object types with the ACL         */
	/*  library...                                                      */

	/* Register for the mgmt ACL...                                     */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_acl_register_object_type()"));
	dce_acl_register_object_type(
		*db_acl,  /* Backing store where ACLs are to be stored.     */
		&mgmt_acl_mgr_uuid,     /* Type of ACL manager: this one is */
			/* for mgmt ACL operations; the UUID is defined     */
			/* globally at the top of this file.                */
			/* Why do we need this parameter? Well, the way     */
			/*  that the ACL library keeps track of the differ- */
			/*  ent "sets" of ACL databases is by manager UUID. */
			/*  The manager UUID is what the library will use   */
			/*  to figure out which ACL database to open and    */
			/*  retrieve a requested ACL's contents from.       */
			/*  Essentially what we are doing here is setting   */
			/*  up things so that calls to the library routine  */
			/*  dce_acl_is_client_authorized() can be made to   */
			/*  check our ACLs, giving only the ACL UUID and a  */
			/*  manager UUID to get the desired result.         */

		sizeof mgmt_printstr/sizeof mgmt_printstr[0], /* Number of  */
				/* items in mgmt_printstr array.            */
		mgmt_printstr,  /* An array of sec_acl_printstring_t struc- */
				/*  tures containing the printable repre-   */
				/*  sentation of each specified permission. */
		&mgmt_info,     /* A single sec_acl_printstring_t contain-  */
				/*  ing the name and short description for  */
				/*  the given ACL manager.                  */
		sec_acl_perm_control,  /* Permission set needed to change   */
				/* an ACL. Constants like these are defined */
				/* in <dce/aclbase.h>.                      */
		sec_acl_perm_test, /* Permission set needed to test an ACL. */

		resolver,  /* Server function to get ACL UUID for a given   */
				/*  object; for us it's the                 */
				/*  sample_resolve_by_name() call, below.   */
				/* This routine is for the use of acl_edit: */
				/*  it allows acl_edit to receive an object */
				/*  name and come up with the ACL UUID; at  */
				/*  least that's what I think it's for.     */
		NULL,           /* Argument to pass to resolver function.   */
		0,              /* Flags -- none here.                      */
		status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_acl_register_object_type()", *status);
		return;
	}

	/* Now register for the regular ACL...                              */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_acl_register_object_type()"));
	dce_acl_register_object_type(
		*db_acl, /* Backing store where ACLs are to be stored.      */
		&sample_acl_mgr_uuid,  /* Hard-coded at top of this file.   */
		sizeof sample_printstr/sizeof sample_printstr[0], /* Number */
				/* of items in our printstring array.       */
		sample_printstr, /* An array of sec_acl_printstring_t       */
				/* structures containing the printable rep- */
				/* resentation of each specified permis-    */
				/* sion set.                                */
		&sample_info,   /* A single sec_acl_printstring_t contain-  */
				/*  ing the name and short description for  */
				/*  the manager we're registering.          */
		sec_acl_perm_control, /* Permission set needed to change an */
				/*  ACL.                                    */
		sec_acl_perm_test,    /* The permission you need to test an */
					/* ACL maintained by this manager.  */

		resolver,       /* Application server function that gives   */
				/*  the ACL UUID for a given object, when   */
				/*  presented with that object's name; for  */
				/*  us it's the sample_resolve_by_name()    */
				/*  routine, below.                         */
		NULL,           /* Argument to pass to resolver routine;    */
				/*  identified as the "resolver_arg" in the */
				/*  code to that function below.            */
		0,              /* Flags -- none here.                      */
		status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_acl_register_object_type()", *status);
		return;
	}

	/* If we're initializing, then we have to create all this stuff...  */
	if (need_init)
	{

		dce_svc_printf(NO_ACL_DBS_MSG);
		/* Create the mgmt interface ACL...                         */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling server_create_acl()"));
		server_create_acl(
			mgmt_acl_mgr_uuid, /* Create mgmt manager type ACL. */
			ALL_MGMT_PERMS, /* Permission set for new ACL.      */
			owner,          /* Principal name for new entry.    */
			new_mgmt_acl,   /* This will contain the new ACL.   */
			mgmt_acl_uuid,  /* This will contain the ACL UUID.  */
			status);
		if (*status != error_status_ok)
		{
			print_server_error("server_create_acl()", *status);
			return;
		}

		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling uuid_to_string()"));
		uuid_to_string(mgmt_acl_uuid, &uuid_string, status);
		if (*status != uuid_s_ok)
		{
			print_server_error("uuid_to_string()", *status);
		}
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
				"String form of mgmt_acl_uuid == %s",
				uuid_string));
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling rpc_string_free()"));
		rpc_string_free(&uuid_string, status);
		if (*status != rpc_s_ok)
		{
			print_server_error("rpc_string_free()", *status);
		}


		/************************************************************/
		/* For the management ACL we must add a default entry for   */
   		/*  the machine principal so dced can manage the server.    */

		/* Construct the name entry string...                       */
		strcpy((char *)machine_principal, "hosts/");
		gethostname((char *)(machine_principal + 6), MAXHOSTNAMELEN + 1);
		strcat((char *)machine_principal, "/self");

		/* Get the machine principal's UUID...                      */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling server_get_local_principal_id()"));
		server_get_local_principal_id(machine_principal,
						&machine_princ_id,
						status);
		if (*status != error_status_ok)
		{
			print_server_error("server_get_local_principal_id()",
						*status);
			return;
		}

		/* Add a user entry for the machine principal to the new    */
		/*  ACL...                                                  */
		permset = ALL_MGMT_PERMS;
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling dce_acl_obj_add_user_entry()"));
		dce_acl_obj_add_user_entry(new_mgmt_acl,
						permset,
						&machine_princ_id,
						status);
		if (*status != error_status_ok)
		{
			print_server_error("dce_acl_obj_add_user_entry()",
						*status);
			return;
		}

		/* By default everybody must be able to get the principal   */
		/*  name. They should be able to ping too. So add an appro- */
		/*  priate unauthenticated permissions entry to the ACL...  */
		permset = mgmt_perm_inq_pname | mgmt_perm_ping;
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling dce_acl_obj_add_unauth_entry()"));
		dce_acl_obj_add_unauth_entry(
			new_mgmt_acl,
			permset,
			status);
		if (*status != error_status_ok)
		{
			print_server_error("dce_acl_obj_add_unauth_entry()",
						*status);
			return;
		}

		/* Add permissions for the any_other entry in the ACL...    */
		permset = mgmt_perm_inq_pname | mgmt_perm_ping;
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling dce_acl_obj_add_any_other_entry()"));
		dce_acl_obj_add_any_other_entry(
			new_mgmt_acl,
			permset,
			status);
		if (*status != error_status_ok)
		{
			print_server_error("dce_acl_obj_add_any_other_entry()",
						*status);
			return;
		}

		/* Store the mgmt ACL...                                    */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling server_store_acl()"));
		server_store_acl(
			*db_acl,     /* The ACL UUID-indexed store.         */
			*db_object,  /* The object UUID-indexed store.      */
			*db_name,  /* The name ("residual")-indexed store.  */
			new_mgmt_acl,       /* The ACL itself.              */
			mgmt_acl_uuid,      /* The mgmt ACL UUID.           */
			&mgmt_object_uuid,  /* The mgmt object UUID.        */
			(unsigned_char_t *)MGMT_OBJ_NAME,  /* The mgmt ob-  */
						/*  ject name.              */
			/* (void*) */ &object_data, /* The object contents. */
			0,                  /* Not a container ACL.         */
			status);

		if (*status != error_status_ok)
		{
			print_server_error("server_store_acl()", *status);
			return;
		}


	/********************************************************************/
	/* Object ACL creation code...                                      */

		/* Now create the object ACL...                             */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling server_create_acl()"));
		server_create_acl(
			sample_acl_mgr_uuid,  /* Create an ACL with this    */
					/*  manager type.                   */
			owner_perms,   /* Give it these permissions.        */
			owner,         /* Make this the principal name.     */
			new_obj_acl,   /* This will contain new ACL.        */
			object_acl_uuid, /* This will contain new ACL UUID. */
			status);
		if (*status != error_status_ok)
		{
			print_server_error("server_create_acl()", *status);
			return;
		}

		/* Null the data header...                                  */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling bzero()"));
		bzero((char *)&object_data, sizeof object_data);

		/* Store the object ACL...                                  */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling server_store_acl()"));
		server_store_acl(
			*db_acl,     /* The ACL UUID-indexed store.         */
			*db_object,  /* The object UUID-indexed store.      */
			*db_name,  /* The name ("residual")-indexed store.  */
			new_obj_acl,        /* The ACL itself.              */
			object_acl_uuid,    /* The object ACL UUID.         */
			&object_uuid,       /* The object UUID.             */
			object_name,        /* The object name.             */
			/* (void*) */ &object_data, /* The object contents. */
			/* is_container */  0,  /* Is this a container ACL? */
			status);
		if (*status != error_status_ok)
		{
			print_server_error("server_store_acl()", *status);
			return;
		}

		/* Finally, free the space we were using...                 */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
				"Calling dce_acl_obj_free_entries()"));
		dce_acl_obj_free_entries(new_obj_acl, status);
		if (*status != error_status_ok)
		{
			print_server_error("dce_acl_obj_free_entries()",
						*status);
			return;
		}

	/* ...end of object ACL creation code.                              */
	/********************************************************************/

	}
        else /* ACL databases already exist; get the two ACL UUIDs...       */
	{

		/* This is a call to sample_resolve_by_name() (see below);  */
		/*  it gives us the UUID of the ACL of the object whose     */
		/*  name we pass it...                                      */
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling (*resolver)()"));
		(*resolver)(
			NULL,         /* No client bind handle; local call. */
			object_name,  /* Object whose ACL UUID we want.     */
			0,            /* Type of ACL we want UUID of.       */
			&sample_acl_mgr_uuid, /* Object's manager type.     */
			0,            /* Ignored as far as we're concerned. */
			NULL,         /* "resolver_arg"; unused.            */
			object_acl_uuid, /* Will contain object ACL UUID.   */
			status);
		if (*status != error_status_ok)
		{
			print_server_error("resolver function (*)", *status);
			return;
		}


		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug5,
				"Calling (*resolver)()"));
		(*resolver)(
			NULL,         /* No client bind handle; local call. */
			(sec_acl_component_name_t)MGMT_OBJ_NAME, /* We want */
						/* mgmt object's ACL UUID.  */
			0,             /* Type of ACL we want UUID of.      */
			&mgmt_acl_mgr_uuid, /* Object's manager type=mgmt.  */
			0,            /* Ignored as far as we're concerned. */
			NULL,         /* "resolver_arg"; ignored.           */
			mgmt_acl_uuid, /* Will contain mgmt ACL UUID.       */
			status);
		if (*status != error_status_ok)
		{
			print_server_error("resolver function (*)", *status);
			return;
		}

  	}


	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_to_string()"));
	uuid_to_string(mgmt_acl_uuid, &uuid_string, status);
	if (*status != uuid_s_ok)
	{
		print_server_error("uuid_to_string()", *status);
	}
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"String form of mgmt_acl_uuid == %s", uuid_string));
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_string_free()"));
	rpc_string_free(&uuid_string, status);
	if (*status != rpc_s_ok)
	{
		print_server_error("rpc_string_free()", *status);
	}


	/* Set up remote management authorization to use the ACL manager.   */
	/*  Note that the first parameter to this call is the address of a  */
	/*  management authorization callback routine, which is defined     */
	/*  later in this file...                                           */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_mgmt_set_authorization_fn()"));
	rpc_mgmt_set_authorization_fn(sample_mgmt_auth, status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_mgmt_set_authorization_fn()", *status);
		return;
	}

	/* Finally, register the rdacl interface with the runtime...        */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_server_register_if()"));
	rpc_server_register_if(
			rdaclif_v1_0_s_ifspec, /* Interface to register.    */
			NULL,                  /* Manager type UUID.        */
			(rpc_mgr_epv_t) &dce_acl_v1_0_epv, /* Entry point   */
							/* vector.          */
			status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_server_register_if()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_acl_mgr_setup()"));

}


/******
 *
 * server_acl_mgr_close --  Called at cleanup time to close 
 *                          the three ACL databases.
 *
 * 
 *    Called from main().
 *
 ******/

void server_acl_mgr_close(
dce_db_handle_t *db_acl,          /* ACL UUID-indexed database.             */
dce_db_handle_t *db_object,       /* Object UUID-indexed database.          */
dce_db_handle_t *db_name,         /* Name-indexed database.                 */
unsigned32 *status)
{

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_acl_mgr_close()"));

	*status = error_status_ok;

	/* Close the ACL UUID-indexed database...                           */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_close()"));
	dce_db_close(db_acl, status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_close()", *status);
		return;
	}

	/* Close the Object UUID-indexed database...                        */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_close()"));
	dce_db_close(db_object, status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_close()", *status);
		return;
	}

	/* Close the name-indexed database...                               */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_close()"));
	dce_db_close(db_name, status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_db_close()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_acl_mgr_close()"));

}


/******
 *
 * server_rdacl_export -- Make the rdacl interface available
 *                        for ACL editors.
 * 
 * 
 *     Note that we don't export to the namespace. Instead, the ACL editor
 *      will typically bind to the server via some other entry that holds
 *      the application-specific interface bindings. For our application,
 *      that entry is:
 *
 *             /.:/sample_server_entry
 *
 *      ...This entry (the "junction" to the object "entries") must hold at
 *      least one object UUID, and the same UUID must be put into the end-
 *      point map too. If not, ACL editors will have no way to distinguish
 *      the endpoints of this server from those of other servers on the same
 *      host that also export the rdacl interface.
 *
 *    Called from main().
 *
 ******/

void server_rdacl_export(
rpc_binding_vector_t *binding_vector, /* Binding handles from RPC runtime.  */
uuid_vector_t *object_uuid_vector,    /* Server instance UUID(s).           */
unsigned32 *status)
{

	uuid_vector_t my_uuids;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_rdacl_export()"));

	*status = error_status_ok;

	/* Register the server's endpoints with the rdacl interface at the  */
	/*  local endpoint map...                                           */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_ep_register()"));
	rpc_ep_register(rdaclif_v1_0_s_ifspec,
		binding_vector,    /* Our binding handles from RPC runtime. */
		object_uuid_vector,  /* Server instance UUID (only one).    */
		(unsigned_char_p_t) "rdacl interface", /* Annotation.       */
		status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_ep_register()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_rdacl_export()"));

}


/******
 *
 * server_rdacl_cleanup -- Called at cleanup time to
 *                         unregister the rdacl interface.
 *
 * 
 *    Called from main().
 *
 ******/

void server_rdacl_cleanup(
rpc_binding_vector_t *binding_vector,  /* Binding handles from RPC runtime. */
uuid_vector_t *object_uuid_vector,     /* Server instance UUID(s).          */
unsigned32 *status)
{

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_rdacl_cleanup()"));

	*status = error_status_ok;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_ep_unregister()"));
	rpc_ep_unregister(rdaclif_v1_0_s_ifspec,
		binding_vector,
		object_uuid_vector,
		status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_ep_unregister()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_rdacl_cleanup()"));

}



/******
 *
 * server_bind_cleanup -- Called at cleanup time to
 *                        unregister the remote bind interface.
 *
 * 
 *    Called from main().
 *
 ******/

void server_bind_cleanup(
rpc_binding_vector_t *binding_vector,  /* Binding handles from RPC runtime. */
uuid_vector_t *object_uuid_vector,     /* Server instance UUID(s).          */
unsigned32 *status)
{

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering server_bind_cleanup()"));

	*status = error_status_ok;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_ep_unregister()"));
	rpc_ep_unregister(sample_bind_v1_0_s_ifspec,
		binding_vector,
		object_uuid_vector,
		status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_ep_unregister()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting server_bind_cleanup()"));

}





/****************************************************************************/
/* End of server init and cleanup functions                                 */
/****************************************************************************/



/******
 * 
 * signal_handler -- Thread to handle asynchronous interrupts. 
 * 
 *  Catch and handle SIGINT and SIGTERM. Note that we
 *  don't use sigaction() here because it won't work with 
 *  asynchronous signals. Also note that signals must be 
 *  blocked prior to being waited for.
 *
 * 
 *    The thread that runs this function is started in main().
 *
 ******/

void signal_handler(char *arg)
{
	sigset_t signals;  /* Set of signals available to the application.  */
	int sig;
	unsigned32 status;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering signal_handler()"));

	status = error_status_ok;

	/* Initialize the signal set...                                     */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sigemptyset()"));
	sigemptyset(&signals);

	/* Add SIGINT to signal set...                                      */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sigaddset()"));
	sigaddset(&signals, SIGINT);

	/* Add SIGTERM to signal set...                                     */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sigaddset()"));
	sigaddset(&signals, SIGTERM);

	/* Set the current signal mask...                                   */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sigprocmask()"));
	sigprocmask(SIG_BLOCK, &signals, NULL);

	/* And now wait for the signals...                                  */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sigwait()..."));
	while (1)
	{
		sig = sigwait(&signals);
		switch (sig)
		{
			case SIGINT:
			case SIGTERM:
				/* SIGNAL-SPECIFIC ACTIONS GO HERE...       */
				break;
			default:
				continue;
		}
		break;
	}

	/* Unset the signal mask...                                         */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling sigprocmask()"));
	sigprocmask(SIG_UNBLOCK, &signals, NULL);

	/* Terminate server: cause the main thread listen loop to return    */
	/*  and go to cleanup. Obviously, if we're not listening yet, this  */
	/*  will fail...                                                    */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_mgmt_stop_server_listening()"));
	rpc_mgmt_stop_server_listening(NULL, &status);
	if (status != error_status_ok)
	{
		print_server_error("rpc_mgmt_stop_server_listening()", status);
		exit(1);
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting signal_handler()"));

}



/******
 *
 * sample_mgmt_auth -- Management authorization callback function.
 * 
 *    This is the routine that is implicitly called to test authorization
 *     whenever someone tries to use the mgmt interface to tinker with us
 *     or our ACLs.
 * 
 *
 * 
 *    The callback is set up by a call to rpc_mgmt_set_authorization() in 
 *    server_acl_mgr_setup().
 *
 ******/

boolean32 sample_mgmt_auth(
rpc_binding_handle_t client_binding, /* Client's binding, whoever he is.    */
unsigned32 requested_mgmt_operation, /* What client is attempting to do.    */
unsigned32 *status)
{
	boolean32 authorized = 0;
	sec_acl_permset_t perm_required;
	unsigned_char_t *uuid_string;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering sample_mgmt_auth()"));

	*status = error_status_ok;

	/* Discover what permission is required in order to do what the     */
	/*  client is trying to do...                                       */
	switch (requested_mgmt_operation)
	{
		case rpc_c_mgmt_inq_if_ids:
			perm_required = mgmt_perm_inq_if;
			break;
		case rpc_c_mgmt_inq_princ_name:
			perm_required = mgmt_perm_inq_pname;
			break;
		case rpc_c_mgmt_inq_stats:
			perm_required = mgmt_perm_inq_stats;
			break;
		case rpc_c_mgmt_is_server_listen:
			perm_required = mgmt_perm_ping;
			break;
		case rpc_c_mgmt_stop_server_listen:
			perm_required = mgmt_perm_kill;
			break;
		default:
		/* This should never happen, but just in case...            */
			return(0);
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_to_string()"));
	uuid_to_string(&mgmt_acl_uuid, &uuid_string, status);
	if (*status != uuid_s_ok)
	{
		print_server_error("uuid_to_string()", *status);
		return(0);
	}
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"String form of mgmt_acl_uuid == %s", uuid_string));
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_string_free()"));
	rpc_string_free(&uuid_string, status);
	if (*status != rpc_s_ok)
	{
		print_server_error("rpc_string_free()", *status);
		return(0);
	}

	/* Okay, now check whether the client is authorized or not...       */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_acl_is_client_authorized()"));
	dce_acl_is_client_authorized(
		client_binding,          /* Client's binding handle.        */
		&mgmt_acl_mgr_uuid,      /* ACL manager type UUID.          */
		&mgmt_acl_uuid,          /* The ACL UUID.                   */
		NULL,                   /*         Pointer to owner's UUID. */
		NULL,                   /* Pointer to owner's group's UUID. */
		perm_required,          /* The desired privileges.          */
		&authorized,            /* Will be TRUE or FALSE on return. */
		status);
	if (*status != error_status_ok)
	{
		print_server_error("dce_acl_is_client_authorized()", *status);
		return(0);
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting sample_mgmt_auth()"));

	if (authorized)
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
				"AUTHORIZED!"));
	else
		DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
				"NOT AUTHORIZED!"));

	/* Return the result to the caller...                               */
	return(authorized);
}



/******
 *
 * sample_resolve_by_name -- take the name of an object, and return the
 *                           UUID of the object's ACL.
 * 
 * The address of this function is passed (via the call to
 * server_acl_mgr_setup()) to the dce_acl_register_object_type() call. So
 * it gets implicitly called anytime someone tries to retrieve the ACL of
 * an object managed by the ACL manager we've set up. 
 * 
 * Basically, the most a server needs is one resolve-by-name routine and
 * one resolve-by-UUID routine; the former gets you the desired object's
 * UUID; and the latter then will get you the object data itself (the way
 * this works can be seen in the body of this routine below). In most
 * cases, these routines will share the same name and UUID databases; if
 * they don't, the resolver_arg can be used to point to the correct other
 * database. Typically, the only difference between the managers is that
 * they use different print strings.
 * 
 * For the official statement of the signature of a dce_acl_resolve_func_t,
 * see the dce_acl_resolve_by_uuid() manpage; that routine has the same
 * type.
 *
 * NOTE that all this routine really has to do is look up the object
 *      UUID, get the ACL UUID from the object header, then extract the
 *      ACL and check its manager type with the manager_type passed, and,
 *      if the manager types match, return the ACL UUID; otherwise, return
 *      an error. Everything else is superfluous, though (perhaps) inter-
 *      esting.
 *
 *******/

dce_acl_resolve_func_t sample_resolve_by_name(
handle_t h,                     /* Client binding handle passed into the    */
				/*  server stub. sec_acl_bind() is used to  */
				/*  create this handle.                     */
sec_acl_component_name_t name,  /* The object whose ACL's UUID we want.     */
sec_acl_type_t sec_acl_type,    /* The type of ACL whose UUID we want.      */
uuid_t *manager_type,           /* The object's manager type.               */
				/* NOTE that this parameter isn't used be-  */
				/*  low.                                    */
boolean32 writing,		/* "This parameter is ignored in OSF's im-  */
				/*  plementation" (from the manpage for     */
				/*  dce_acl_resolve_by_uuid()).             */
void *resolver_arg,             /* This is the app-defined argument passed  */
				/*  to dce_acl_register_object_type(); it   */
				/*  should be a handle for a backing store  */
				/*  indexed by UUID. Note that it isn't     */
				/*  used here though.                       */
uuid_t *acl_uuid,               /* To return ACL's UUID in.                 */
error_status_t *st              /* To return status in.                     */
)
{
	uuid_t u, *up; /* To hold the retrieved object UUID, and to         */
		       /*  take a pointer to it.                            */
	unsigned_char_t *uuid_string;
	sec_acl_t retrieved_acl;
	uuid_t owner_uuid, group_uuid;

	/* The definition of the following is in the sample.idl file.       */
	/*                                                                  */
	/* See the "Examples" section in the dce_db_open() manpage,         */
	/*  where the skeleton IDL interface for a server's backing         */
	/*  store is given. The data type definition (which is what         */
	/*  sample_data_t is) is there prescribed as consisting of a        */
	/*  dce_db_header_t, plus whatever server-specific data is          */
	/*  quired, all in a single structure.                              */
	/*                                                                  */
	/*  Essentially it's a dce_db_header_t structure (with an           */
	/*  application-defined message string tacked on); this is          */
	/*  the object header data structure that is returned, e.g.,        */
	/*  by dce_db_header_fetch(); in other words, this is the           */
	/*  thingie that actually contains the data "in" an object          */
	/*  held in an object store. At least that's what I think it        */
	/*  is...                                                           */
	sample_data_t object_data;


	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering sample_resolve_by_name()"));

	*st = error_status_ok;

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"Object name == %s", name));

	/* Check for non-existence of object name...                        */
	if (!name || !*name)
	{
		dce_svc_printf(CANNOT_RESOLVE_NAME_MSG);
		return;
	}

	/* Get the object's UUID, which will be the key that we will use to */
	/*  fetch this particular object's data in the call following this  */
	/*  one...                                                          */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_fetch_by_name()"));
	dce_db_fetch_by_name(db_name, (char *)name, /* (void *) */ &u, st);
	if (*st != error_status_ok)
	{
		print_server_error("dce_db_fetch_by_name()", *st);
		return;
	}

	up = &u;     /* ...take the pointer to the key.                     */

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_to_string()"));
	uuid_to_string(up, &uuid_string, st);
	if (*st != uuid_s_ok)
	{
		print_server_error("uuid_to_string()", *st);
		return(0);
	}
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"String form of retrieved key UUID == %s",
			uuid_string));
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_string_free()"));
	rpc_string_free(&uuid_string, st);
	if (*st != rpc_s_ok)
	{
		print_server_error("rpc_string_free()", *st);
		return(0);
	}


	/* Using the UUID "key" that we just retrieved, get the object_data */
	/*  for the desired object (note that the data that one retrieves   */
	/*  with this routine can be anything; it depends on what we are    */
	/*  using the backing store for)...                                 */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_fetch_by_uuid()"));
	dce_db_fetch_by_uuid(db_object, up, /* (void *) */ &object_data, st);
	if (*st != error_status_ok)
	{
		print_server_error("dce_db_fetch_by_uuid()", *st);
		return;
	}

	/* Now, depending on the kind of ACL we're hunting for (i.e. ob-    */
	/*  ject, container, etc.), extract its UUID from the object's      */
	/*  header structure...                                             */
	switch (sec_acl_type)
	{
		case 1:
			DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
					"Case == 1"));
			*acl_uuid = object_data.s_hdr.tagged_union.h.def_object_acl;
			break;
		case 2:
			DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
					"Case == 2"));
			*acl_uuid = object_data.s_hdr.tagged_union.h.def_container_acl;
			break;
		default:
			DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
					"Case == default"));
			*acl_uuid = object_data.s_hdr.tagged_union.h.acl_uuid;
	}

	/* Find out some other interesting stuff...                         */

	owner_uuid = object_data.s_hdr.tagged_union.h.owner_id;
	group_uuid = object_data.s_hdr.tagged_union.h.group_id;

	uuid_to_string(&owner_uuid, &uuid_string, st);
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"Owner UUID == %s", uuid_string));
	rpc_string_free(&uuid_string, st);

	uuid_to_string(&group_uuid, &uuid_string, st);
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"Group UUID == %s", uuid_string));
	rpc_string_free(&uuid_string, st);

	/* Here it might be interesting to try retrieving the ACL itself,   */
	/*  and e.g seeing what its manager type is...                      */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling dce_db_fetch_by_uuid()"));
	dce_db_fetch_by_uuid(db_acl,
			acl_uuid,
			&retrieved_acl,
			st);
	if (*st != error_status_ok)
	{
		print_server_error("dce_db_fetch_by_uuid()", *st);
		return(0);
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_to_string()"));
	uuid_to_string(&(retrieved_acl.sec_acl_manager_type), &uuid_string, st);
	if (*st != uuid_s_ok)
	{
		print_server_error("uuid_to_string()", *st);
		return(0);
	}
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"sec_acl_manager_type == %s", uuid_string));
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_string_free()"));
	rpc_string_free(&uuid_string, st);
	if (*st != rpc_s_ok)
	{
		print_server_error("rpc_string_free()", *st);
	}

	/* We are handling two ACL managers through this function, so we    */
	/*  have to make sure that we've extracted from the single ACL      */
	/*  database the correct ACL: i.e., one whose manager type UUID is  */
	/*  identical to the manager_type parameter we were passed: this is */
	/*  the manager whose ACL the runtime is trying to bind to. The     */
	/*  point is that the ACL library is going to call all its regis-   */
	/*  tered resolvers successively with the SAME ACL UUID, until it   */
	/*  finds one that works. If we just return the ACL UUID without    */
	/*  checking whether the right manager_type is being asked for,     */
	/*  we'll only cause an error in the ACL library when it discovers  */
	/*  that the types don't match up. This will prevent acl_edit from  */
	/*  working. So do the checking here...                             */
	if ((manager_type != NULL) && (!uuid_equal(manager_type,
						&(retrieved_acl.sec_acl_manager_type),
						st)))
	{
		/* Return a bad status...                                   */
		*st = acl_s_bad_manager_type;
		/* And no ACL UUID...                                       */
		acl_uuid = NULL;
		return(0);
	}


	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling uuid_to_string()"));
	uuid_to_string(acl_uuid, &uuid_string, st);
	if (*st != uuid_s_ok)
	{
		print_server_error("uuid_to_string()", *st);
		return(0);
	}
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"sec_acl_type == %d", (int)sec_acl_type));
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug8,
			"String form of retrieved ACL UUID == %s",
			uuid_string));
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_string_free()"));
	rpc_string_free(&uuid_string, st);
	if (*st != rpc_s_ok)
	{
		print_server_error("rpc_string_free()", *st);
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting sample_resolve_by_name()"));

}




/******
 *
 * sample_bind_export -- Register the interface specification
 *                       and endpoints for the remote binding inter-
 *                       face.
 * 
 *    Called from main().
 *
 ******/

void sample_bind_export(
rpc_binding_vector_t *binding_vector,
uuid_vector_t *uuid_vec,
unsigned32 *status)
{

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Entering sample_bind_export()"));

	*status = error_status_ok;

	/* Register sample_bind interface...                                */
	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_server_register_if()"));
	rpc_server_register_if(sample_bind_v1_0_s_ifspec,
		NULL,
		(rpc_mgr_epv_t) &sample_bind_epv,
		status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_server_register_if()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug4,
			"Calling rpc_ep_register()"));
	rpc_ep_register(sample_bind_v1_0_s_ifspec,
		binding_vector,
		uuid_vec,
		(unsigned_char_p_t) "sample_bind interface",
		status);
	if (*status != error_status_ok)
	{
		print_server_error("rpc_ep_register()", *status);
		return;
	}

	DCE_SVC_DEBUG((smp_svc_handle, smp_s_server, svc_c_debug7,
			"Exiting sample_bind_export()"));

}



/*****
*
* create_server_uuid -- Create server instance UUID.
* 
*
*  Called from main(), make_server_entry().
*
*****/

void
create_server_uuid(
uuid_t *server_uuid,
uuid_vector_t *server_uuid_v
)
{
	unsigned32 status;
	dce_error_string_t error_string;
	int print_status;

	/********************************************************************/
	/* Create a UUID to identify this server instance; this will go in- */
	/*  to the namespace and endpoint map so that clients of such gen-  */
	/*  eric interfaces as rdacl can find this server's endpoints.      */
	/*  Without such a UUID, these clients can't distinguish among      */
	/*  servers on the same host that also export the generic inter-    */
	/*  faces. This could be a well-known UUID, but here we will        */
	/*  generate one on the fly. Clients binding to us by name will get */
	/*  this UUID without having to know what it is.                    */
	/*                                                                  */
	/********************************************************************/

	fprintf(stdout, "Entering create_server_uuid()...\n");

	/* Create and save server instance UUID...                          */
	fprintf(stdout, "Calling uuid_create()...\n");
	uuid_create(server_uuid, &status);
	if (status != error_status_ok)
	{
		dce_error_inq_text(status, error_string, &print_status);
		fprintf(stdout, "uuid_create(): %s\n", error_string);
		exit(1);
	}

	server_uuid_v->uuid[0] = server_uuid;
	server_uuid_v->count = 1;

	fprintf(stdout, "...Exiting create_server_uuid()\n");
}



/*****
*
* print_server_error-- Server version. Prints text associated with
*                      bad status code.
*
*
*****/

void
print_server_error(
char *caller,           /* Routine that received the error.                 */
error_status_t status)  /* Status we want to print the message for.         */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);
	dce_svc_printf(SERVER_ERROR_MSG, caller, error_string);

}

