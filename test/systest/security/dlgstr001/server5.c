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
 * $Log: server5.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:40  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:51  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:46:17  agd
 * 	expand copyright
 * 	[1994/10/06  14:32:49  agd]
 * 
 * Revision 1.1.2.1  1994/09/19  18:20:02  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:51:39  bhaim]
 * 
 * $EndLog$
 */
/*
 ***************************************************************************
 * This is the server program for the string conversion application.  It
 * registers the interface with the RPC runtime and the endpoint mapper.
 * It then listens for incoming requests and serves each in a separate thread.
 ***************************************************************************
 */
/*
 * (c) Copyright 1992, 1993 Hewlett-Packard Co.
 */

#include <pthread.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/param.h>
#include <dce/rpcexc.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <dce/dce_error.h>
#include "common5.h"
#include "identity.h"
#include "string_conv5.h"
#include "sigcatch.c"	

string_conv5_v1_0_epv_t string_conv5_v1_0_s_epv =
    { string_convert_uppercase5 };

#define IFSPEC                  string_conv5_v1_0_s_ifspec
#define MANAGER_EPV             (rpc_mgr_epv_t) &string_conv5_v1_0_s_epv

identity_info_t       *iden_info;
 
void print_error
(
    error_status_t status,
    char *msg
);

pthread_t	sigcatch_thread;		

void main(int argc, char *argv[])
{
   rpc_binding_vector_t  *bvec;			/* used to register w/runtime */
   error_status_t	 status, ignore;	/* returned by DCE calls */
   dce_error_string_t	 dce_err_string;	/* text describing error code */
   sec_login_handle_t    my_context;
   sec_login_handle_t    new_context;
   sec_login_handle_t    deleg_context;
   ndr_char		 *string_binding;	/* printable rep of binding */
   int			 i;			/* index into bvec */
   char			 cds_entry_name[256];	/* holds CDS name */
   char			 cds_profile_name[256]; /* holds profile name */
   rpc_if_id_t		 if_id;			/* taken from the ifspec */
#ifdef	USE_CDS_HOSTS_DIRECTORY
   char			 *dce_host_name;	/* DCE name of this host */
#else
   char			 hostname[MAXHOSTNAMELEN];
#endif

setup_sigcatch(&sigcatch_thread);

   /*
    * have the server establish it's identity and register it's name with
    * dce (using a keytab file as well).  This prevents having to dce_login
    * the conv_server program.
    */
   establish_identity(
      string_conv5_AUTHN_SVC,
      (unsigned char *)string_conv5_SERVER_PRINCIPAL_NAME,
      (unsigned char *)"/krb5/v5srvtab",
      &iden_info);

   /*
    * Specify that the RPC runtime should use all protocol sequences for
    * this application (UDP/IP and TCP/IP are currently supported).  This
    * allows the client the flexibility of choosing whichever protocol
    * sequence it prefers.
    *
    * The first parameter specifies the maximum number of concurrent remote
    * procedure call requests that the server can accept.  This server
    * wishes to allow as many as possible.  The second parameter is the DCE
    * error status.
    */
#ifdef ALL_PROTSEQS
   rpc_server_use_all_protseqs(rpc_c_protseq_max_calls_default, &status);
#else
   rpc_server_use_protseq((unsigned_char_p_t) "ncacn_ip_tcp",
		rpc_c_protseq_max_calls_default, &status);
#endif
   if (status != rpc_s_ok) {
      dce_error_inq_text(status, dce_err_string, (int *)&ignore);
      PRINT_FUNC(PRINT_HANDLE, "Cannot use all prot sequences: %s\n", dce_err_string);
      exit(1);
   }

   /*
    * Register the interface definition, and manager entry point vector
    * application does not use type UUIDs, so specify a null type UUID.
    */
   rpc_server_register_if(IFSPEC,		/* IDL-generated ifspec */
                           NULL,		/* No type UUID */
                           MANAGER_EPV,		/* manager epv */
                            &status);
   if (status != rpc_s_ok) {
      dce_error_inq_text(status, dce_err_string, (int *)&ignore);
      PRINT_FUNC(PRINT_HANDLE,"Cannot register with runtime: %s\n", dce_err_string);
      exit(1);
   }

   /*
    * Get and display the bindings for debugging purposes.  The binding
    * information (bvec) is also required for registration with the
    * endpoint mapper.
    */
   rpc_server_inq_bindings(&bvec, &status);
   if (status != rpc_s_ok) {
      dce_error_inq_text(status, dce_err_string, (int *)&ignore);
      PRINT_FUNC(PRINT_HANDLE, "Cannot get bindings: %s\n", dce_err_string);
      exit(1);
   }

   /*
    * Print out the bindings obtained from the RPC runtime.  These show
    * what protocol sequence(s) and port(s) have been grabbed by the
    * runtime for this server.
    */
   PRINT_FUNC(PRINT_HANDLE, "Bindings:\n");

   for (i = 0; i < bvec->count; i++) {
      rpc_binding_to_string_binding(bvec->binding_h[i], &string_binding, &status);
      if (status != rpc_s_ok) {
         dce_error_inq_text(status, dce_err_string, (int *)&ignore);
         PRINT_FUNC(PRINT_HANDLE, "Cannot get string bindings: %s\n", dce_err_string);
      } else
         PRINT_FUNC(PRINT_HANDLE, "   %s\n", string_binding);

      rpc_string_free(&string_binding, &ignore);
   }

   /*
    * Register the interface with the local endpoint mapper.  This allows
    * connections by applications using this interface without specifying
    * a port (i.e., using a partially-bound binding handle).
    *
    * NOTE: If multiple servers on the same system register the same
    * interface with the endpoint mapper, only the last one registering
    * will be visible (this call overwrites binding information).
    */
   rpc_ep_register(IFSPEC,
                   bvec,					/* binding vector */
                   NULL,					/* No object UUID */
                   (unsigned_char_t *)str_conv_description,	/* name */
                   &status);
   if (status != rpc_s_ok) {
      dce_error_inq_text(status, dce_err_string, (int *)&ignore);
      PRINT_FUNC(PRINT_HANDLE, "Cannot register with endpoint map: %s\n", dce_err_string);
      exit(1);
   }

#ifndef USE_CDS_HOSTS_DIRECTORY
   /*
    * This sample by default register in /.:/subsys/HP/sample-apps/,
    * a directory which should have been created when your cell admin
    * first configured the cell.  It should have been created with
    * public read and write permissions, allowing any authenticated
    * user to register its server bindings (create a new server entry
    * or write bindings to an existing entry); any authenticated user
    * should likewise be able to run the client and read the bindings.
    */
   sprintf(cds_entry_name, "/.:/subsys/HP/sample-apps/%s", str_conv_cds_entry);
#else
   /*
    * The server will regster its bindings at the current cell-relative
    * DCE host name and construct a CDS entry name from it.  This requires
    * that the user running the client and server have read and write
    * permissions in /.:/hosts/<hostname>/ respectively.  To get around
    * this restriction, the sample applications by default register in
    * /.:/subsys/HP/sample-apps/, a directory which should have public
    * read and write permissions.
    */
   dce_cf_get_host_name(&dce_host_name, &status);
   if (status != rpc_s_ok) {
      dce_error_inq_text(status, dce_err_string, (int *)&ignore);
      PRINT_FUNC(PRINT_HANDLE, "Cannot get DCE hostname: %s\n", dce_err_string);
      exit(1);
   }
   sprintf(cds_entry_name, "/.:/%s/%s", dce_host_name, str_conv_cds_entry);
   free(dce_host_name);
#endif

   /*
    * Export the binding vector and interface specification to the name
    * server.  Register in the name service under the host-specific entry
    * name just computed above.  The first parameter is the syntax to use;
    * in the first release of DCE there is only one supported syntax.  The
    * second parameter is the entry name to look under; it was created
    * above.  The third parameter is the server interface specification,
    * with the UUID from the IDL file.  The fourth parameter is used to
    * specify an object UUID if the server exports multiple objects; this
    * server does not export multiple objects, so NULL is used.
    *
    * NOTE: If multiple servers export bindings to the same entry name,
    * only the last one will be heeded (the first server will become
    * unreachable).  However, if the first server were to terminate and
    * unregister itself, it would unregister the endpoint and CDS
    * information for the second server as well, making it now unreachable.
    * To avoid this in your applications see the code in the lookup sample.
    */
   rpc_ns_binding_export(rpc_c_ns_syntax_default,	/* default name syntax */
                         (unsigned char *)cds_entry_name,
                         IFSPEC,			/* IDL-generated ifspec */
                         bvec,				/* this server's bindings */
                         NULL,				/* No object UUID */
                         &status);
   if (status != rpc_s_ok) {
      dce_error_inq_text(status, dce_err_string, (int *)&ignore);
      PRINT_FUNC(PRINT_HANDLE,"Cannot export bindings in NSI under %s: %s\n",
                 cds_entry_name, dce_err_string);
      exit(1);
   }

   /*
    * Get the interface identifier for an interface specification.  This
    * identifier is required by the call to add the server entry above to
    * the profile element done below.  The first parameter is the server's
    * interface specification.  The second is a return parameter, the
    * interface identifier.  The final parameter is the DCE error status.
    */
   rpc_if_inq_id(IFSPEC,		/* IDL-generated if spec */
                 &if_id,		/* Fills in the interface ID */
                 &status);		/* Error status for this call */
   if (status != rpc_s_ok) {
      dce_error_inq_text(status, dce_err_string, (int *)&ignore);
      PRINT_FUNC(PRINT_HANDLE,"Cannot determine interface id: %s\n", dce_err_string);
      exit(1);
   }

   /*
    * For the purposes of this test, we are going to set what used to be
    * the RPC_DEFAULT_ENTRY environment variable to a hard-coded default
    * value of "/.:/cell-profile".  You can look at the regular string-conv
    * sample program for the better way of handling the profile name.
    */
   strcpy(cds_profile_name,"/.:/cell-profile");

   /*
    * Add the entry just exported to the name service as an element of a
    * profile.  This enables clients to look up this server using either
    * the entry name unique to this server or just the profile name.  The
    * profile can contain bindings for multiple instances of this service
    * registered under different entries in the name space.
    *
    * The first parameter is the name syntax to use for the profile element
    * (default since there is only one valid syntax).  The second parameter
    * is the profile name in which to register this server's entry.  The
    * third parameter is the interface identifier retrieved earlier.  The
    * fourth parameter is the name syntax to use for the server entry name
    * (default again).  The fifth parameter is the actual server entry name
    * registered in CDS above.  The sixth parameter is a priority value, an
    * integer between 0-7 that determines in which order bindings will be
    * returned in a lookup operation.  The seventh parameter is a string
    * annotation useful when browsing in the name space (though DCE RPC
    * does not use this string during lookup or import operations, or for
    * enumerating profile elements).  The last parameter is the DCE status.
    */
   rpc_ns_profile_elt_add(rpc_c_ns_syntax_default,
                          (unsigned char *)cds_profile_name,
                          &if_id,
                          rpc_c_ns_syntax_default,
                          (unsigned char *)cds_entry_name,
                          my_profile_element_priority,
                          (unsigned char *)str_conv_description,
                          &status);
   if (status != rpc_s_ok) {
      dce_error_inq_text(status, dce_err_string, (int *)&ignore);
      PRINT_FUNC(PRINT_HANDLE,"Cannot add entry %s to CDS profile %s: %s\n",
                 cds_entry_name, cds_profile_name, dce_err_string);
      exit(1);
   }

   /*
    * Wrap the call to listen() in a TRY/ENDTRY block to properly handle
    * any exceptions raised in the manager or server stub code.  Exceptions
    * are raised as a result of communication failure, data segmentation
    * faults or other disastrous conditions in the server.  The reason for
    * this TRY is so that if an exception occurs, the FINALLY block can
    * properly unregister the server before exiting.
    */
   TRY {
      PRINT_FUNC(PRINT_HANDLE, "Listening...\n");

      /*
       * Listen and handle incoming RPC requests.  The manager function
       * will be called from the server stub to handle each incoming RPC.
       * The manager must be reentrant since up to max_calls_default
       * threads can be executing that code simultaneously.  Normally the
       * listen call will not return unless something unusual happens.
       */
      rpc_server_listen(rpc_c_listen_max_calls_default, &status);
      if (status != rpc_s_ok) {
         dce_error_inq_text(status, dce_err_string, (int *)&ignore);
         PRINT_FUNC(PRINT_HANDLE, "Listen returned with error: %s\n", dce_err_string);
      } else
         PRINT_FUNC(PRINT_HANDLE, "Stopped listening...\n");

   } FINALLY {
      /*
       * An exception was raised in the manager or server stub.
       * Clean up the entries and exit the application.
       *
       * NOTE WELL: This code is NOT executed when you interrupt the
       * server from the keyboard!  A keyboard interrupt is received by
       * the process as an asychronous signal causing the process to
       * terminate without cleaning up.
       */

      PRINT_FUNC(PRINT_HANDLE, "Unregistering endpoints and interface...\n");
      /*
       * Unregister the interface and endpoints with the RPC runtime.
       */
      rpc_ep_unregister(IFSPEC,		/* IDL-generated ifspec */
                        bvec,		/* this server's bindings */
                        NULL,		/* no object UUIDs */
                        &ignore);

      rpc_server_unregister_if(IFSPEC,
                                NULL,		/* No object UUID. */
                                &ignore);

      /*
       * Free the binding vector now that it is no longer used.
       */
      rpc_binding_vector_free(&bvec, &ignore);
   }
   ENDTRY;

   exit(0);
}



/*
**********************************************************************
**
**  P R I N T _ E R R O R
**
**********************************************************************
*/
void print_error
(
    error_status_t status,
    char *msg
)
{
    int       inq_st;                    /* status for dce_error_inq_text */
    dce_error_string_t	dce_err_string;  /* text describing error code */

    dce_error_inq_text (status, dce_err_string, &inq_st);

    if (msg != NULL) {
	fprintf (stderr,
	    "SERVER: %s\n",
	    msg
	);
    }
    fprintf (stderr,
	"SERVER: %s\n",
	(inq_st == 0) ?
	(char *)dce_err_string
	:
	"No dce error text available"
    );
    fflush (stderr);
} /* end print_error() */
