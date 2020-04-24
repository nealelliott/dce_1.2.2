/*
 * @OSF_COPYRIGHT@
 * 
 */
/*
 * HISTORY
 * $Log: dce_util_test_server.c,v $
 * Revision 1.1.6.3  1996/02/18  00:31:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:16  marty]
 *
 * Revision 1.1.6.2  1995/12/11  15:20:08  root
 * 	Submit
 * 	[1995/12/11  14:33:39  root]
 * 
 * Revision 1.1.2.2  1994/10/26  17:59:11  max
 * 	Initialize handle to NULL before passing to dce_server_register.
 * 	Declare registration data static as util API keeps reference to it.
 * 	[1994/10/26  17:57:30  max]
 * 
 * Revision 1.1.2.1  1994/10/18  20:55:47  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:47:57  max]
 * 
 * $EndLog$
 */
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
 * $Log: dce_util_test_server.c,v $
 * Revision 1.1.6.3  1996/02/18  00:31:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:16  marty]
 *
 * Revision 1.1.7.2  1996/02/17  23:17:16  marty
 * 	Update OSF copyright years
 *
 * Revision 1.1.6.2  1995/12/11  15:20:08  root
 * 	Submit
 * 	[1995/12/11  14:33:39  root]
 *
 * Revision 1.1.7.2  1995/12/11  14:33:39  root
 * 	Submit
 *
 * Revision 1.1.2.2  1994/10/26  17:59:11  max
 * 	Initialize handle to NULL before passing to dce_server_register.
 * 	Declare registration data static as util API keeps reference to it.
 * 	[1994/10/26  17:57:30  max]
 *
 * Revision 1.1.3.2  1994/10/26  17:57:30  max
 * 	Initialize handle to NULL before passing to dce_server_register.
 * Declare registration data static as util API keeps reference to it.
 *
 * Revision 1.1.2.1  1994/10/18  20:55:47  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:47:57  max]
 *
 * Revision 1.1.1.2  1994/10/18  20:47:57  max
 * 	Initial submission of FVTs for dced utility API
 *
 * $EndLog$
 */

/*
 * This module implements a server for the functional tests of the
 * dced server utility functions.  These functions can only be called
 * by a server launched by dced.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#include <dce/sec_login.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/dced.h>

#include <dce_util_test_if.h>

#define _dce_util_test_server_
#include <dce_util_test_server.h>


/* space for 3 lines */
#define SND_INFOLINE_SZ 240
char snd_infoline_buf[SND_INFOLINE_SZ];


#ifndef STDOUT
#define STDOUT 1
#endif


const char * tet_unresolved_c_str = "TET_UNRESOLVED";
const char * tet_pass_c_str       = "TET_PASS";
const char * tet_fail_c_str       = "TET_FAIL";


/* define out function table types */
typedef int(test_function_t )(void);
typedef struct test_functions_entry {
    char *fname;
    test_function_t *f;
} test_functions_entry_t;


int dce_server_inq_uuid_001(void);
int dce_server_inq_attr_001(void);
int dce_server_inq_server_001(void);
int dce_server_sec_begin_001(void);
int dce_server_sec_done_001(void);
int dce_server_register_001(void);
int dce_server_unregister_001(void);
int dce_server_disable_service_001(void);
int dce_server_enable_service_001(void);
int dce_server_use_protseq_001(void);


test_functions_entry_t tfun_table[] = {
    { "dce_server_inq_uuid_001" , &dce_server_inq_uuid_001 } ,
    { "dce_server_inq_attr_001" , &dce_server_inq_attr_001 } ,
    { "dce_server_inq_server_001" , &dce_server_inq_server_001 } ,
    { "dce_server_sec_begin_001" , &dce_server_sec_begin_001 } ,
    { "dce_server_sec_done_001" , &dce_server_sec_done_001 } ,
    { "dce_server_register_001" , &dce_server_register_001 } ,
    { "dce_server_unregister_001" , &dce_server_unregister_001 } ,
    { "dce_server_disable_service_001", &dce_server_disable_service_001 },
    { "dce_server_enable_service_001", &dce_server_enable_service_001 },
    { "dce_server_use_protseq_001", &dce_server_use_protseq_001 },
    { NULL, NULL }
};



char * exec_uuid_c_str = NULL;      /* determined by dced, process instance */

char *principal_g_str = "dce_util_server";    /* from command line '-p' arg */
char *attr_uuid_g_str = "c96d2ce0-def9-11cd-884f-000c00b4b407";    
char *attr_value_0_g_str = NULL; /* from command line '-v' arg */
char *conf_uuid_g_str = "be8cb6d8-def9-11cd-884f-000c00b4b407";
char *test_fn_g_str = NULL;      /* from command line '-f' arg */
char *output_file_g_str = NULL;  /* from command line '-o' arg */

rpc_if_handle_t if_handle_glob;
uuid_t binding_obj_uuid_glob;   /* created on the fly */

uuid_t attr_uuid_glob;          /* created from attr_uuid_g_str */
uuid_t conf_uuid_glob;          /* created from conf_uuid_g_str */

test_function_t *test_fn_glob = NULL;  /* found by searching the table tfun_table */



void main( int argc, char** argv);
int sprint_error(char* buf, error_status_t status);
void server_stop( rpc_binding_handle_t handle, unsigned32 quit_code, error_status_t* st);


/* these are manager functions */
error_status_t ping( handle_t handle, idl_ulong_int x, idl_ulong_int *y);
void stop_server( handle_t handle, error_status_t *st);



static unsigned32 exit_code;

/*
 * main() -- the main of the test server
 *
 * Due to the fact that the dce utility functions
 * can only be called by a server launched by dced
 * we cannot do the calls directly from the tet
 * executable.  So the test tet executable sets up
 * the server entry with dced before the test and
 * puts in the argv's the test name & the principal
 * 
 */


void
main(int argc, char *argv[])
{
    rpc_binding_vector_t	*bind_vector_p;
    uuid_t			obj_uuid;
    uuid_vector_t		obj_uuid_vec;
    error_status_t		status;
    char		        errbuf[1024];
    char                        infobuf[1024];
    unsigned32                  uuid_st;
    char                        **argp;
    int                         fd, fd1; /* redirection file descriptors */
    dce_server_handle_t         dce_server_handle;
    
    
    if (argc < 3) exit(1);

    /* init globals */
    if_handle_glob = dce_util_test_if_v1_0_s_ifspec;    
    
    /* init globals from argv's */
    
    for ( argp = argv + 1; argp < argv+argc; argp++) {
	if ( !strncmp( *argp, "-p" , 2 )) { /* principal */
	    argp++;
	    principal_g_str = *argp;
	}
	else if ( !strncmp( *argp, "-a" , 2 )) { /* attribute uuid */
	    argp++;
	    attr_uuid_g_str = *argp;
	}
	else if ( !strncmp( *argp, "-v" , 2 )) { /* attribute value */
	    argp++;
	    attr_value_0_g_str = *argp;
	}
	else if ( !strncmp( *argp, "-c" , 2 )) { /* conf uuid */
	    argp++;
	    conf_uuid_g_str = *argp;
	}
	else if ( !strncmp( *argp, "-f" , 2 )) { /* conf uuid */
	    argp++;
	    test_fn_g_str = *argp;
	}
	else if ( !strncmp( *argp, "-o" , 2 )) { /* conf uuid */
	    argp++;
	    output_file_g_str = *argp;
	}
    }
    
    
    /*****************************************************************
     * redirect stdout to the fifo
     *****************************************************************/


    if (!output_file_g_str) output_file_g_str =  "/tmp/error.out";

    fd = open( output_file_g_str, O_WRONLY, 0); /* open the fifo */
    if (fd < 0) {
	fd = open ( "/tmp/error.out", O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd < 0) {
	    send_infoline_nl ("Cannot open output file. Exiting ...");
	    exit(1);
	}
    }
    close(STDOUT);
    
    fd1 = dup2(fd ,STDOUT ); 
    if (fd1 != STDOUT ) exit(1); /* we cannot even send any info */
    
    
    if (test_fn_g_str == NULL) {
	send_infoline_nl ( "No test function specified in command line. Exiting...");
	exit(1);
    }
    else {
	test_functions_entry_t * p = tfun_table;
	
	while ( p->fname) {
	    if (!strcmp (p->fname,test_fn_g_str)) {
		test_fn_glob = p->f;
		break;
	    }
	    p++;
	}
    }
    
    /* check that we have a test function */
    if ( test_fn_glob == NULL ) {
	send_infoline_nl ( "Invalid test function specified in command line. Exiting...");
	exit(1);
    }


    if (principal_g_str) {
	send_infoline("Principal: ");
	send_infoline_nl(principal_g_str);
    }
    if (attr_uuid_g_str) {
	send_infoline("Attr uuid: ");
	send_infoline_nl(attr_uuid_g_str);
    }
    if (attr_value_0_g_str) {
	send_infoline("Attr value: ");
	send_infoline_nl(attr_value_0_g_str);
    }
    if (conf_uuid_g_str) {
	send_infoline("Conf uuid: ");
	send_infoline_nl(conf_uuid_g_str);
    }
    if (test_fn_g_str) {
	send_infoline("Test function: ");
	send_infoline_nl(test_fn_g_str);
    }


    uuid_create ( &binding_obj_uuid_glob, &uuid_st);
    uuid_from_string( attr_uuid_g_str, &attr_uuid_glob, &uuid_st);
    uuid_from_string( conf_uuid_g_str, &conf_uuid_glob, &uuid_st);
    if_handle_glob = dce_util_test_if_v1_0_s_ifspec;
    

    (*test_fn_glob)();

    
#ifdef UNUSED
    
    /*
   * Delete CDS entry if it already exists to avoid multiple object UUIDs.
   */
    rpc_ns_mgmt_entry_delete ( rpc_c_ns_syntax_dce, 
			      (unsigned_char_t *) argv[1], 
			      &status );
    
    /* 
     * Generate object uuid 
   */
    uuid_create ( &obj_uuid, &status );
    if ( status != uuid_s_ok ) 
	{
	    sprint_error("uuid_from_string()", status);
	    exit(1);
	}
    obj_uuid_vec.count = 1;
    obj_uuid_vec.uuid[0] = &obj_uuid;
    
    /* 
     * Register interface with rpc runtime
     */
    rpc_server_register_if ( dcdrel002_v1_0_s_ifspec, NULL, NULL, &status );
    if ( status != rpc_s_ok ) 
	{
	    sprint_error ( "rpc_server_register_if()", status );
	    exit(1);
	}
    
    /* 
     * Tell rpc runtime we want to use all supported protocol sequences 
     */
    rpc_server_use_all_protseqs ( rpc_c_protseq_max_reqs_default, &status );
    if ( status != rpc_s_ok ) 
	{
	    sprint_error ( "rpc_server_use_all_protseqs()", status );
	    exit(1);
	}
    
    /* 
     * Ask the runtime which binding handle(s) it's going to let us use
     */
    rpc_server_inq_bindings(&bind_vector_p, &status);
    if (status != rpc_s_ok) 
	{
	    sprint_error("rpc_server_inq_bindings()", status);
	    exit(1);
	}
    
    /* 
     * Register binding info with endpoint map
     */
    rpc_ep_register(dcdrel002_v1_0_s_ifspec, 
		  bind_vector_p,
		    (uuid_vector_t *) &obj_uuid_vec,
		    (unsigned_char_t *) "Test Server for DCED System Tests",
		    &status);
    if (status != rpc_s_ok) 
	{
	    sprint_error("rpc_ep_register()", status);
	    exit(1);
	}
    
    /* 
     * Export binding info to the namespace 
     */
    rpc_ns_binding_export(rpc_c_ns_syntax_dce, 
			  (unsigned_char_t *) argv[1],
			  dcdrel002_v1_0_s_ifspec, 
			  bind_vector_p,
			  (uuid_vector_t *) &obj_uuid_vec, 
			  &status);
    if (status != rpc_s_ok) 
	{
	    fprintf ( errbuf, "rpc_ns_binding_export(%s)", argv[1] );
	    sprint_error( errbuf, status);
	    exit(1);
	}
    
    /* 
     * Listen for service requests 
     */
    rpc_server_listen(rpc_c_listen_max_calls_default, &status);
    if (status != rpc_s_ok) 
	{
	    sprint_error("rpc_server_listen()", status);
	    exit(1);
	}
    
    /* 
     * Returned from listen loop, it should be hanppened after
     * mgr routine calls rpc_mgmt_stop_server_listening
     */
    rpc_ep_unregister(dcdrel002_v1_0_s_ifspec, 
		      bind_vector_p,
		      (uuid_vector_t *) &obj_uuid_vec,
		      &status);
    if (status != rpc_s_ok) 
	{
	    sprint_error("rpc_ep_unregister()", status);
	    exit(1);
	}
    
    rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_dce, 
			     (unsigned_char_t *) argv[1], 
			     &status);
    if (status != rpc_s_ok) 
	{
	    sprint_error("rpc_ns_mgmt_entry_delete()", status);
	    exit(1);
	}
    
    fprintf(stderr, "%s: stopping at exit code: %d\n", argv[0], 
	    exit_code);
    exit(exit_code);
    
#endif
    
    
    exit(0);

}


/*
 * sprint_error -- Prints text associated with bad status code.
 */
int
sprint_error 
(
 char *buf,
 error_status_t status
)
{
  char	*msg;
  int    ret;
  
  msg = (char *) dce_sprintf(status);
  ret = sprintf(buf, "%s", msg);
  free(msg);
  
  return ret;
}


#ifdef UNUSED
/*
 * stop_bad_op -- Instruct server to exit with -1.
 */
void
stop_bad_op
(
 rpc_binding_handle_t	handle,
 error_status_t		*st
)
{
  *st = error_status_ok;
  exit_code = -1;
  rpc_mgmt_stop_server_listening(NULL, st);
}
#endif





/*
 * part of the interface we export.
 */
void
stop_server
(
 handle_t	        handle,
 error_status_t		*st
)
{
  *st = error_status_ok;
  exit_code = 0;
  rpc_mgmt_stop_server_listening(NULL, st);
}


/*
 * part of the interface we export.
 */
error_status_t ping(
    handle_t handle,
    idl_ulong_int x,
    idl_ulong_int *y)
{
    error_status_t		st;
    st = error_status_ok;
    *y = x + 1;
    return st;
}






/**********************************************************************
 *              check_our_binding_ep_map 
 *
 * It tries to find one of our bindings in the endpoint map by
 * calling rpc_ep_resolve_binding.
 *
 * return 1 if find our binding in ep map
 * return 0 if not
 * return -1 if failed in the process
 **********************************************************************/

int check_our_binding_ep_map (void) 
{
    rpc_binding_vector_t * binding_vector;
    rpc_binding_handle_t binding;
    unsigned32 rpc_st;

    /* now check that the endpoint data is correct */
    rpc_server_inq_bindings( &binding_vector, &rpc_st );
    if ( rpc_st != rpc_s_ok) {
	send_infoline_nl( "In check_our_binding_ep_map." );
	send_infoline( "Failed rpc_server_inq_bindings: " );
	send_infoerror_nl( rpc_st );
	return -1;
    }

    if (binding_vector->count < 1) {
	/* have no bindings */
	send_infoline_nl( "In check_our_binding_ep_map." );
	send_infoline( "Failed: rpc_server_inq_bindings returned " );
	send_infoline_nl( "a binding vector with no bindings." );
	return -1;
    }

    binding = binding_vector->binding_h[0];
    
    rpc_binding_reset(binding, &rpc_st);
    if ( rpc_st != rpc_s_ok) {
	send_infoline_nl( "In check_our_binding_ep_map." );
	send_infoline( "Failed rpc_binding_reset: " );
	send_infoerror_nl( rpc_st );
	return -1;
    }

    rpc_binding_set_object( binding, &binding_obj_uuid_glob, &rpc_st );
    if ( rpc_st != rpc_s_ok) {
	send_infoline_nl( "In check_our_binding_ep_map." );
	send_infoline( "Failed rpc_binding_set_object: " );
	send_infoerror_nl( rpc_st );
	return -1;
    }

    rpc_ep_resolve_binding( binding, if_handle_glob, &rpc_st);
    if ( rpc_st == rpc_s_ok) 
	return 1;
    else if (rpc_st == ept_s_not_registered) 
	return 0;
    else {
	send_infoline_nl( "In check_our_binding_ep_map." );
	send_infoline( "Failed rpc_ep_resolve_binding: " );
	send_infoerror_nl( rpc_st );
	return -1;
    }
}


/* return 1 if find our interface registered
 * return 0 if not
 * return -1 if failed in the process
 */
int check_our_if_registered( void)
{
    unsigned32 rpc_st;

    rpc_server_register_if( if_handle_glob, NULL, NULL, &rpc_st);
    if ( rpc_st == rpc_s_ok ) {
	/* return to the previous unregistered state */
	rpc_server_unregister_if( if_handle_glob, NULL, &rpc_st);
	return 0;
    }
    else if ( rpc_st == rpc_s_type_already_registered )
	return 1;
    else {
	send_infoline_nl( "In check_our_if_registered." );
	send_infoline( "Failed rpc_server_register_if: " );
	send_infoerror_nl( rpc_st );
	return -1;
    }
}



/*
 *
 * return 0 on success
 * return -1 on failure
*/

int do_our_registration ( dce_server_handle_t * server_handle_p)
{

    server_t *svr_info;
    static dce_server_register_data_t reg_data; /* need to make static as util api will keep a reference to it */
    error_status_t		st;
    unsigned32                  rpc_st;

    *server_handle_p = NULL;


    dce_server_inq_server(&svr_info, &st);
    if ( st != rpc_s_ok) {
	send_infoline_nl( "In do_our_registration." );
	send_infoline( "Failed dce_server_inq_server: " );
	send_infoerror_nl( st );
	return -1;
    }

    reg_data.ifhandle = if_handle_glob;
    reg_data.epv = NULL;
    reg_data.num_types = 0;
    reg_data.types = NULL;


    dce_server_register( 
			0,
			svr_info,
			&reg_data,
			server_handle_p,
			&st);

    if ( st != rpc_s_ok) {
	send_infoline_nl( "In do_our_registration." );
	send_infoline( "Failed dce_server_register: " );
	send_infoerror_nl( st );
	return -1;
    }


    if ( check_our_if_registered() != 1) {
	send_infoline_nl( "In do_our_registration." );
	send_infoline_nl( "Error. Our interface is not registered");
	return -1;
    }


    if ( check_our_binding_ep_map() != 1 ) {
	send_infoline_nl( "In do_our_registration." );
	send_infoline_nl( "Error. Our endpoint not in ep map");
	return -1;
    }


    return 0;
}





int send_infoline( const char * a) {                                
   if ( a != snd_infoline_buf) /* only copy if passed different pointer */
       strncpy(snd_infoline_buf, a, SND_INFOLINE_SZ);                  
   write( STDOUT, snd_infoline_buf, SND_INFOLINE_SZ);    
   return 0;
}	 



int send_infoline_nl( const char * a) {                               
   int len = strlen(a);
   if ( a != snd_infoline_buf) /* only copy if passed different pointer */
       strncpy(snd_infoline_buf, a, SND_INFOLINE_SZ -1 );                
   if (len > (SND_INFOLINE_SZ -2)) len = SND_INFOLINE_SZ -2;          
   snd_infoline_buf[len++] = '\n';
   snd_infoline_buf[len] = '\0';
   write( STDOUT, snd_infoline_buf, SND_INFOLINE_SZ);    
   return 0;
}	 



int send_infoerror(unsigned32 a) {    
   sprint_error(snd_infoline_buf, a);		  
   write( STDOUT, snd_infoline_buf, SND_INFOLINE_SZ);
   return 0;
}

int send_infoerror_nl(unsigned32 a) {  
   int len;
   len = sprint_error(snd_infoline_buf, a);   
   if (len > (SND_INFOLINE_SZ -2)) len = SND_INFOLINE_SZ -2;          
   snd_infoline_buf[len++] = '\n';
   snd_infoline_buf[len] = '\0';
   write( STDOUT, snd_infoline_buf, SND_INFOLINE_SZ);   				 
   return 0;
}




int net_info_to_principal ( sec_login_net_info_t *net_info, sec_rgy_name_t *prin )
{
/*     return prin_uuid_to_name( &(net_info->pac.principal.uuid), prin ); */
    sec_id_pac_t    *p = &(net_info->pac);
    unsigned char   *cell;
    unsigned char   *princ;
    sec_rgy_name_t  cn;
    sec_rgy_name_t  pn;
    sec_rgy_name_t  gn;
    error_status_t  st;
    unsigned int    i;

    cell = p->realm.name;
    princ = p->principal.name;

    if (cell && *cell && princ && *princ) {
	send_infoline("Global Principal: ");
        send_infoline (cell);
	send_infoline_nl(princ);
    } else {
        sec_id_gen_name(sec_rgy_default_handle, &p->realm.uuid, &p->principal.uuid,
                            gn, cn, pn, &st);
        if (st) {
            send_infoline("Failed sec_id_gen_name: ");
	    send_infoerror_nl( st);
	    return -1;
        } else {
	    send_infoline("Global Principal: ");
            send_infoline_nl( gn);
            cell = cn;
            princ = pn;
        }
    }

    strcpy(prin, princ);	    
    return 0;
}



int prin_uuid_to_name( uuid_t * uuid, sec_rgy_name_t *name )
{

    int errval=0;
    error_status_t st;
    sec_rgy_handle_t rgy_context;


    sec_rgy_site_open ( NULL, &rgy_context, &st);
    if ( st != error_status_ok) {
	send_infoline( "Failed sec_rgy_site_open_query: ");
	send_infoerror_nl( st );
	errval = 5;
    }

    if (!errval) {
	sec_rgy_pgo_id_to_name ( sec_rgy_default_handle,
				sec_rgy_domain_person,
				uuid,
				name,
				&st);
	if ( st != error_status_ok) {
	    send_infoline( "Failed sec_rgy_pgo_id_to_name: ");
	    send_infoerror_nl( st );
	    errval = 4;
	}
    }


#ifdef UNUSED
    if (errval < 5)
	sec_rgy_site_close( rgy_context, &st);
#endif

    return ( errval != 0 ? -1: 0);
} 

