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
 * $Log: client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:26  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:32  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:39:54  agd
 * 	expand copyright
 * 	[1994/10/06  14:31:13  agd]
 * 
 * Revision 1.1.2.1  1994/09/19  18:17:10  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:50:43  bhaim]
 * 
 * $EndLog$
 */
/*
 ***************************************************************************
 * This string conversion client takes as it's first argument a string
 * to be converted by the remote procedure to uppercase.  It's second
 * argument is the number of servers desired in the delegation chain.
 *
 * The client locates the server in the Cell Directory Service (CDS) by
 * looking under the name under which the server registered itself.  The
 * client uses the explicit binding method, so it locates the directory
 * entry and uses the binding information there to construct a binding
 * handle (rpc_binding_handle_t).  The client passes this binding handle to
 * the invocation of the remote procedure.
 ***************************************************************************
 */
/*
 * (c) Copyright 1992, 1993, 1994 Hewlett-Packard Co.
 */

#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <dce/rpcexc.h>
#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include "common.h"
#include "string_conv.h"

#define MIN_DEL_SERVERS 1
#define MAX_DEL_SERVERS 6

#define T_OK    	0
#define T_ERROR		1
#define T_UNKNOWN       2

void print_error
(
    error_status_t status,
    char *msg
);

void main(int argc, char *argv[])
{
    sec_login_handle_t    my_context;
    sec_login_handle_t    attr_context;
    sec_login_handle_t    new_context;
    sec_login_handle_t    deleg_context;
    rpc_binding_handle_t  bh;			/* points to the server */
    error_status_t        status, ignore;	/* returned by DCE calls */
    dce_error_string_t    dce_err_string;	/* text describing error code */
    unsigned_char_t       string_to_convert[max_conv_string_len];
    rpc_ns_handle_t       import_context;	/* used with the name server */
    ndr_boolean           done;			/* flag controlling NSI loop */
    char                  *dce_host_name;	/* holds name of this host */
    char                  cds_entry_name[256];	/* holds CDS name */
    char                  number_of_servers[2]; /* requested number of servers*/

    /* status returned from rpc */
    error_status_t eval_status = T_OK;
    unsigned t_status = T_OK;                   /* exit status */	
    unsigned32 num_attr_types = 5;
    sec_attr_t attrs[5];

    /*
     * When using the name service, no hostname argument is necessary.  The
     * server is found either by looking for the entry name used by the
     * server (using shared knowledge between client and server) or through
     * the entry named by the RPC_DEFAULT_ENTRY environment variable.
     */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s string-to-convert number-of-servers\n",
                argv[0]);
        exit(T_ERROR);
    } else {
        strcpy((char *)string_to_convert, argv[1]);
        strcpy((char *)number_of_servers, argv[2]);
        if ((strlen(number_of_servers) != 1) ||
            (atoi(number_of_servers) < MIN_DEL_SERVERS) ||
            (atoi(number_of_servers) > MAX_DEL_SERVERS)) {
            fprintf(stderr,
              "Error: number of servers must be between %d and %d inclusive\n",
              MIN_DEL_SERVERS, MAX_DEL_SERVERS);
            exit(T_ERROR);
        }
    }

    sec_login_get_current_context (&my_context, &status);
    
    if (status != rpc_s_ok) {
        print_error (status, "sec_login_get_current_context() failed with error:");
        exit(T_ERROR);
    }

    attr_setup(num_attr_types, attrs, &status);
    if (GOOD_STATUS(&status)) {
       attr_context = sec_login_set_extended_attrs(my_context, num_attr_types, attrs, &status);
       if (BAD_STATUS(&status)) {
          printf("sec_login_set_extended_attrs, failed status %x\n", status);
          exit(T_ERROR);
       }
    } else {
       printf("error setting up attribute array\n");
       exit(T_ERROR);
    }
    
    /* Done with tmp context with attributes so release it */
    sec_login_release_context(&my_context, &ignore);

    deleg_context = sec_login_become_initiator (
                        attr_context,
                        sec_id_deleg_type_traced,
                        NULL,  /* no delegate restrictions */
                        NULL,  /* no target restrictions */
                        NULL,  /* no optional restrictions */
                        NULL,  /* no required restrictions */
                        sec_id_compat_mode_caller,
                        &status
                    );

    if (status != error_status_ok) {
        print_error (status, "sec_login_become_initiator() failed with error:" );
        exit(T_ERROR);
    } else {
        new_context = deleg_context;
    }

    /* If sec_login_become_initiator didn't succeed, we can't proceed */
    if (status != rpc_s_ok) {
        print_error (status, "sec_login_become_intiator() failed with error:");
        exit(T_ERROR);
    }

    /* Done with tmp context with attributes so purge it */
    sec_login_purge_context(&attr_context, &ignore);

    /* Make the newly setup context the default for this process */
    sec_login_set_context (new_context,	&status);
    if (status != error_status_ok) {
        print_error (status, "sec_login_set_context() failed with error:");
        exit(T_ERROR);
    }

#ifndef USE_CDS_HOSTS_DIRECTORY
    /*
     * This sample by default registers in /.:/subsys/HP/sample-apps/,
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
     * Get the current DCE host name.  This hostname will be relative to the
     * root of the cell, so it will be a string "hosts/<hostname>", where
     * hostname is your DCE host name.  The first parameter is a string that
     * is allocated to hold the return value.  The application must free
     * this string.  The second parameter is the DCE error status.
     *
     * This code requires that the user running the client and server have
     * read and write permissions in /.:/hosts/<hostname>/ respectively.  To
     * get around this restriction, the sample applications by default
     * register in /.:/subsys/HP/sample-apps/, a directory which should have
     * public read and write permissions.
     */
    dce_cf_get_host_name(&dce_host_name, &status);
    if (status != rpc_s_ok) {
        dce_error_inq_text(status, dce_err_string, (int *)&ignore);
        PRINT_FUNC(PRINT_HANDLE, "Cannot get DCE hostname: %s\n", dce_err_string);
        exit(T_ERROR);
    }
    sprintf(cds_entry_name, "/.:/%s/%s", dce_host_name, str_conv_cds_entry);
    free(dce_host_name);
#endif

    /*
     * Contact the directory service to determine the location information
     * for the server.  This call caches information from the directory
     * entry named into a local database.  We can then walk through the
     * cached information in the following loop.
     *
     * The first parameter is the syntax to use; in the first release of DCE
     * there is only one supported syntax (the default), so use it.  The
     * second parameter is the entry name to look under; it was created
     * above.  If NULL is passed in as the entry name, the environment
     * variable RPC_DEFAULT_ENTRY will be used as an entry name instead
     * (this is done in the code below).  The third parameter is the client
     * interface specification, with the UUID from the IDL file; this is
     * used to identify the server in the name service.  The fourth
     * parameter is used to specify an object UUID if the server exports
     * multiple objects; this server does not export multiple objects, so
     * NULL is used.  The fifth parameter is returned by the call; it points
     * to an opaque data structure holding context information used by the
     * rpc_ns_binding_import_next() routine as it walks through the
     * information retrieved.  The last parameter is the DCE error status.
     */
    rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
                                (unsigned_char_t *)cds_entry_name,
                                string_conv_v1_0_c_ifspec,
                                NULL,
                                &import_context,
                                &status);

    if (status != rpc_s_ok) {
        dce_error_inq_text(status, dce_err_string, (int *)&ignore);
        PRINT_FUNC(PRINT_HANDLE,
            "Warning: Could not import bindings from %s:\n\t%s\n",
            cds_entry_name, dce_err_string);

        /*
         * This search was unsuccessful, meaning either there is no CDS
         * directory entry here or there are no bindings in the entry.
         * Look instead in the default profile element for a matching
         * interface UUID.
         */
        rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
                                    NULL,
                                    string_conv_v1_0_c_ifspec,
                                    NULL,
                                    &import_context,
                                    &status);
        if (status != rpc_s_ok) {
            /*
             * Still no matching entry was found -- we must exit now since
             * we cannot locate a suitable server for this client.
             */
            dce_error_inq_text(status, dce_err_string, (int *)&ignore);
            PRINT_FUNC(PRINT_HANDLE, "Could not find a server: %s\n", dce_err_string);
            sec_login_purge_context(&new_context, &ignore);
            exit(T_ERROR);
        }
    }

    rpc_ns_mgmt_handle_set_exp_age(import_context, 0, &status);
    if (status != rpc_s_ok) {
        /*
         * Still no matching entry was found -- we must exit now since
         * we cannot locate a suitable server for this client.
         */
        dce_error_inq_text(status, dce_err_string, (int *)&ignore);
        PRINT_FUNC(PRINT_HANDLE, "could not set handle expiration: %s\n", dce_err_string);
        sec_login_purge_context(&new_context, &ignore);
        exit(T_ERROR);
    }

    /*
     * We have found a directory entry containing information about this
     * interface.  Now we find a suitable binding for this client.  The
     * following loop will continue until a successful RPC has been made
     * and the flag "done" is set to true (following the RPC) OR until one
     * of the rpc_ns_ routines detects that there are no more bindings and
     * decides to break out of the loop.
     */

    done = false;
    while (done != true) {
        eval_status = T_OK;	
        /*
         * Attempt to import a binding for the server.  The first parameter
         * is the context returned by the import_begin call above.  The
         * second parameter is a binding handle data structure that will be
         * allocated.  This application must free the binding handle after
         * we are done with it.  The last parameter is the DCE error status.
         *
         * This call will randomly return one of the bindings found in the
         * directory (if there are more than one).  It returns the status
         * rpc_s_no_more_bindings when the bindings have been exhausted.
         */
        rpc_ns_binding_import_next(import_context, /* context from begin()   */
                                   &bh,            /* single handle returned */
                                   &status);

        if (status == rpc_s_ok) {
	    printf("lookinig for binding %s\n",(unsigned char *)string_conv_SERVER_PRINCIPAL_NAME);
            rpc_binding_set_auth_info (
                bh,
                (unsigned char *)string_conv_SERVER_PRINCIPAL_NAME,  /* from common.h */
                rpc_c_protect_level_pkt,
                rpc_c_authn_dce_secret,
                new_context,
                rpc_c_authz_dce,
                &status
            );
        } else {
            if (status == rpc_s_no_more_bindings) {
                /*
                 * There are no more possible bindings to choose from.  Free
                 * the binding handle allocated by import_next() and break
                 * out of the while loop.
                 */
                PRINT_FUNC(PRINT_HANDLE,"There are no more server bindings\n");
                rpc_binding_free(&bh, &ignore);
                break;
            } else {
                /*
                 * We did not find a compatible binding handle; instead we
                 * encountered some other serious error.  Print an error
                 * message and exit this client.
                 */
                dce_error_inq_text(status, dce_err_string, (int *)&ignore);
                PRINT_FUNC(PRINT_HANDLE, "Could not import a binding: %s\n", dce_err_string);
                sec_login_purge_context(&new_context, &ignore);
                exit(T_ERROR);
            }
        }

        /*
         * At this point we have imported a binding handle.  It may be that
         * the server that exported this binding handle is no longer running,
         * so wrap the call to the server for this binding in a TRY/ENDTRY
         * block; this will catch any exceptions raised in the client stub
         * due to an invalid server binding handle (or any other error in
         * the client or server).  If an exception occurs the next binding
         * handle in the name service database will be tried.
         */
        TRY {
            PRINT_FUNC(PRINT_HANDLE,
                       "Calling string_convert_uppercase(%s), servers(%s)\n",
                       string_to_convert, number_of_servers);
            /*
             * Call the remote procedure, using the binding handle we just
             * got from the name service and giving it a string to convert.
             */
	    printf("eval_status = %d\n",eval_status);
            string_convert_uppercase(bh,
                                     string_to_convert,
                                     (unsigned_char_t *)number_of_servers,
                                     &eval_status);
	    printf("eval_status = %d\n",eval_status);

            PRINT_FUNC(PRINT_HANDLE,
                       "string_convert_uppercase returned \"%s\"\n",
                       string_to_convert);
            /* if (strlen((char *)string_to_convert) == 0) { */
            if (eval_status != error_status_ok) {
                printf("String Conversion Failed: ");
                t_status = T_ERROR;			
            } else if (eval_status == error_status_ok) { 
                printf("String Conversion Passed: ");
            } else {
                printf("Unknown Failure: ");
                t_status = T_UNKNOWN;
            }
            printf("conversion returned \"%s\"\n", string_to_convert);

            /*
             * Record successful completion of this remote procedure call so
             * we will break out of the while loop.  If you want to try to
             * call the remote procedure using all of the possible bindings,
             * don't set this to true.  If an exception occurs this code
             * will not be executed; instead the CATCH_ALL code is executed.
             */
            done = true;

        } CATCH_ALL {
            /*
             * We caught an exception in the client stub code.  Print out an
             * error message and keep trying.
             */
#if 1
            /*
             * Printing out the exception value without the building blocks
             * routine above requires use of non-standard interfaces (found
             * in exc_handling.h).  Instead, simply report that an exception
             * was raised and continue the loop.
             */
            PRINT_FUNC(PRINT_HANDLE, "Caught an exception\n");
#else
            /*
             * This code will be used by default.  It requires that you link
             * with the HP building blocks library (done by default in the
             * Makefile).
             */
            char  exc_buf[dce_c_error_string_len];

            /*
             * Return a text string corresponding to an exception.  The
             * first parameter is the exception of interest; the macro
             * THIS_CATCH evaluates to the exception that caused us to get
             * into this CATCH block.  The second parameter is a string
             * array where the message should be stored.  This function
             * returns its second argument, which will contain the string
             * corresponding do the exception in THIS_CATCH.
             */
            PRINT_FUNC(PRINT_HANDLE, "Caught an exception: %s\n",
                       err_get_exception_string(THIS_CATCH, exc_buf));
#endif
        }
        ENDTRY;

        /*
         * Free the binding handle allocated by rpc_ns_binding_import_next()
         * above.  The RPC has been made so it is no longer needed.
         */
        rpc_binding_free(&bh, &ignore);
    }

    /*
     * Close down the association with the name server and free the space
     * allocated by DCE for the import context.  The first parameter is the
     * import context; the second is the DCE error status.
     */
    rpc_ns_binding_import_done(&import_context, &ignore);
    sec_login_purge_context(&new_context, &ignore);

    exit(t_status);			
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
	fprintf (stderr, "SERVER: %s\n", msg);
    }
    fprintf (stderr, "SERVER: %s\n", (inq_st == 0) ?
	(char *)dce_err_string
	:
	"No dce error text available"
    );
    fflush (stderr);
} /* end print_error() */

