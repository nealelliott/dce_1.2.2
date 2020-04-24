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
 * $Log: dhd_keytab.c,v $
 * Revision 1.1.6.1  1996/08/09  11:47:28  arvind
 * 	Merge changes.
 * 	[1996/06/11  14:25 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/21  22:10 UTC  truitt  /main/HPDCE02/8]
 *
 * 	Merge quick change.
 * 	[1996/03/21  22:04 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 *
 * 	Put options in the correct order.
 * 	[1996/03/19  21:02 UTC  dnguyen  /main/HPDCE02/7]
 *
 * 	Add string binding support
 * 	[1996/03/19  15:28 UTC  dnguyen  /main/HPDCE02/dnguyen_CHFts15878/1]
 *
 * 	Merge to main line
 * 	[1996/03/18  18:36 UTC  truitt  /main/HPDCE02/5]
 *
 * 	Merge changes.
 * 	[1996/03/18  18:34 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/11  13:51 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:13:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:02  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:20:50  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/10/19  17:05 UTC  truitt
 * 	Merge local work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts50/1  1995/10/19  17:04 UTC  truitt
 * 	CHFts16529: Correct order of hostdata options.  Fix malloc's and free's.
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/29  20:47 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  00:51 UTC  tatsu_s
 * 	Added -local option.
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  17:45 UTC  truitt
 * 	Merge ordering work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:44 UTC  truitt
 * 	CHFts15237: Correct order of help and show options.
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/21  16:27 UTC  truitt
 * 	Merge help message work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts14/1  1995/02/21  16:25 UTC  truitt
 * 	CHFts14293: Have the keytab commands call the
 * 	correct help messages.
 * 	[1995/12/08  17:55:42  root]
 * 
 * Revision 1.1.2.13  1994/09/06  18:30:07  pwang
 * 	Added -noprivacy option to allow non-dce-pkt-privacy access of
 * 	dced keytab service [OT#10442]
 * 	[1994/09/06  18:29:52  pwang]
 * 
 * Revision 1.1.2.12  1994/08/25  20:45:14  pwang
 * 	Eched the option(s) which is not entered [OT#11710]
 * 	[1994/08/25  20:44:19  pwang]
 * 
 * Revision 1.1.2.11  1994/08/08  18:51:45  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:14  melman]
 * 
 * Revision 1.1.2.10  1994/08/03  22:09:48  pwang
 * 	Added local keytab support [OT 11280]
 * 	[1994/08/03  22:08:01  pwang]
 * 
 * Revision 1.1.2.9  1994/07/08  21:18:04  pwang
 * 	Allowed unauthenticated access to dced [OT#11214] and
 * 	used level_pkt_privacy to access dced keytab service
 * 	if authenticated.
 * 	[1994/07/08  21:17:20  pwang]
 * 
 * Revision 1.1.2.8  1994/07/05  16:30:07  pwang
 * 	Corrected mis-useed attributes [OT#11020]
 * 	[1994/07/05  16:23:24  pwang]
 * 
 * Revision 1.1.2.7  1994/06/22  16:08:23  pwang
 * 	Checked the return of dced_binding_create() properly
 * 	[OT#10948]
 * 
 * Revision 1.1.2.6  1994/06/09  16:02:02  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:32  devsrc]
 * 
 * Revision 1.1.2.5  1994/05/31  21:09:58  pwang
 * 	Check -entry and -data to make sure they are mutual
 * 	excl. (OT#10762)
 * 	[1994/05/31  20:58:19  pwang]
 * 
 * Revision 1.1.2.4  1994/05/18  21:57:25  pwang
 * 	Updated to changes in RFC and added swithes and adjusted
 * 	the outputs and fixed bugs.
 * 	[1994/05/18  21:30:14  pwang]
 * 
 * Revision 1.1.2.3  1994/05/06  14:21:41  pwang
 * 	Changed the status code dced_s_no_more_entry to
 * 	dced_s_no_more_entries [OT#10554]
 * 	[1994/05/06  14:20:13  pwang]
 * 
 * Revision 1.1.2.2  1994/05/05  14:56:59  pwang
 * 	Used DCP_ARGV_NO_OPT_LEFTOVERS flag to detect illegal options.
 * 	Made corresponding name changes due to the changes in dced.
 * 	[1994/05/05  14:52:28  pwang]
 * 
 * Revision 1.1.2.1  1994/04/22  19:59:32  pwang
 * 	Initial Implementation
 * 	[1994/04/22  19:51:42  pwang]
 * 
 * $EndLog$
 */

/*
 * MODULE:
 * dhd_keytab.c
 *
 * DESCRIPTION:
 * This module contains all the dced keytab operations to access 
 * or manipulate keys thr dcecp on a specified host.
 */

/*
 * INCLUDE FILES
 */
#include <dhd.h>

/*
 * DECLARATIONS 
 */

/*
 * INTERNAL VARIABLES 
 */

static dced_entry_t	s_null_entry;
static dced_key_t	s_null_key;
static sec_passwd_rec_t	s_null_passwd;


/*
 * FUNCTION:    dcp_kt_catalog 
 *
 * OVERVIEW:    Returns a list of the names of all keytab objects 
 *              on the specified host.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_kt_catalog(ClientData   clientData, 
               Tcl_Interp   *interp, 
               int          argc, 
               char         **argv)
{
    char                    *entryname, *hostname;
    int                     argc1, argc2;
    char                    **argv1, **argv2;
    boolean32		    simplename = FALSE, local_only= FALSE,
                            authn_user = FALSE, noprivacy = FALSE;
    char		    *fullname = NULL;
    int                     i, number_of_hosts;
    dced_binding_handle_t   kt_handle;
    sec_login_handle_t	    login_context;
    dced_cursor_t           kt_cursor;
    dced_entry_t            *entry_p;
    handle_t		    rpc_binding = NULL;
    char		    *binding_string = NULL;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-local", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
        dcp_t_kt_local_help},
	{"-noprivacy", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
	dcp_t_kt_noprivacy_help},
	{"-simplename", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
	dcp_t_kt_simplename_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &local_only;
    arg_table[1].dst = (char *) &noprivacy;
    arg_table[2].dst = (char *) &simplename;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV_2(argc1, argv1, argc2, argv2);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
  
    /*
     * Check for syntax errors
     */
    if (argc > 2) {
	DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (argc < 2) {
        hostname = NULL;
        number_of_hosts = 1;
    }
    else {
        /*
         * Set convenience variables
         */
        DCP_GENERAL_SET_NAMELIST(argv[1]);

	/*
	 * Split the hostname list 
	 */
	if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
	    return TCL_ERROR;
	}
	hostname = argv1[0];
	number_of_hosts = argc1;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    /*
     * Loop thr the hostname list
     */
    for (i=0; i<number_of_hosts; i++) {

        if (i) {
                Tcl_AppendResult(interp, "\n", (char *) NULL);
                hostname = argv1[i];
        }

	/*
	 * The user might have entered multiple string bindings. Assume the
	 * string bindings are in tcl syntax and convert them to string
	 * bindings. If not, the result of the conversion is NULL and
	 * we can go on as usual. If so, the result binding string should
	 * be used during the call to dhd__parse_name.
	 */
 	if (hostname != NULL) {
 	    if (Tcl_SplitList(interp, hostname, &argc2, &argv2) != TCL_OK) {
		DCP_DHD_ARGV_FREE(argc1, argv1);
 		return TCL_ERROR;
 	    }
 	    binding_string = (char *) dcp_binding_tcl_to_string(argc2, argv2, &status);
  	    DCP_DHD_ARGV_FREE(argc2, argv2);
 	    if (DCP_DHD_GOOD_STATUS(status) && binding_string != NULL) {
 		hostname = binding_string;
 	    }
 	}

	/*
	 * Parse the hostnames
	 */
	if (dhd__parse_name(interp, NULL, dced_c_service_keytab,
			     dced_c_service_keytab_len, hostname, &entryname, 
			     NULL, (simplename) ? NULL : &fullname) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

        /*
         * Compare the hostname entered with the entryname returned
         * by the parsing function.  If they are equal, then the user
         * passed in a string binding, not a hostname object.
         */
        if (strcmp(hostname, entryname) == 0) {
            /*
             * Use rpc calls to get the dced binding handle.
             */
            free(entryname);
            if (binding_string == NULL)
		binding_string = dce_strdup(hostname);
            rpc_binding_from_string_binding((unsigned_char_t *)binding_string,
                                            &rpc_binding,
                                            &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(binding_string);
                if (!simplename) {
                    free(fullname);
                }
                DCP_SET_RESULT_CODE(status);
                break;
            }
	    free(binding_string);
	    binding_string = NULL;

            /*
             * Turn the rpc binding into a dced binding.
             */
            dced_binding_from_rpc_binding(dced_e_service_type_keytab,
                                          rpc_binding,
                                          &kt_handle,
                                          &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                if (!simplename) {
                    free(fullname);
                }
                rpc_binding_free(&rpc_binding, &status1);
                DCP_SET_RESULT_CODE(status);
                break;
            }
        }
	else {
	    /*
	     * Call dced_binding_create() to get binding
	     */
	    if (dcp_general_is_bound_locally(interp)) {
		dced_binding_create((dced_string_t)entryname, 
				    ((local_only) ? dced_c_binding_syntax_local :
				     dced_c_binding_syntax_default),
				    &kt_handle,
				    &status);
	    }
	    else {
		dced_binding_create((dced_string_t)entryname, 
				    ((local_only) ? dced_c_binding_syntax_bypass :
				     dced_c_binding_syntax_default), 
				    &kt_handle,
				    &status);
	    }
	    free(entryname);
	    if (!DCP_DHD_GOOD_STATUS(status)) {
		if (!simplename) {
			free(fullname);
		}
	    	DCP_SET_RESULT_CODE(status);
		break;
	    }
	}

	/*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
                dced_binding_set_auth_info(
                       kt_handle, 
                       (noprivacy) ? dhd_c_prot_level : dhd_c_keytab_prot_level,
                       dhd_c_authn_svc, 
                       (rpc_auth_identity_handle_t) login_context,
                       dhd_c_authz_svc, 
                       &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
        }       

        /*
         * Call dced_initialize_cursor() to get the cursor of 
         * the object list
         */
        dced_initialize_cursor(kt_handle, &kt_cursor, &status); 
	DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_entry_get_next() to traverse the object list
         */
        for ( ; ; ) {
            dced_entry_get_next(kt_cursor, &entry_p, &status);
	    if (DCP_DHD_GOOD_STATUS(status)) {
                /*
                 * Display the object list
                 */
                dhd__show_entry(interp, 
				entry_p, 
				(simplename) ? NULL : fullname,
				dhd_e_show_name); 
            } 
            else if (DCP_DHD_STATUS_EQUAL(status, dced_s_no_more_entries)) {
                /*
                 * Out of for ( ; ; ) loop, no more to read.
                 */
		DCP_DHD_RESET_STATUS(status);
                break;
            }
            else {
		/*
		 * A error has been seen
		 */
                break;
            }

        } /* end of for ( ; ; ) */

        /*
         * Call dced_release_cursor() to release the cursor
         */
        dced_release_cursor(&kt_cursor, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

out:
        /*
         * Call dced_binding_free to free the dced binding handle
         */
        dced_binding_free(kt_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

	if (!simplename) {
		free(fullname);
	}
        /*
         * Out of do loop, a error has been met.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free the memory and return right status
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
} /* End of dcp_kt_catalog() */


/*
 * FUNCTION: dcp_kt_create 
 *
 * OVERVIEW: Create a keytab entry and the keys associated
 *           with it on the specified host.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter.
 *
 * OUTPUTS:
 *    UUID of the created object 
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_kt_create(ClientData    clientData, 
              Tcl_Interp    *interp, 
              int           argc, 
              char          **argv)
{
    char                    *attribute = (char *) NULL;
    char                    *uuid = (char *) NULL;
    char                    *annotation = (char *) NULL;
    char                    *storage = (char *) NULL;
    char                    *data = (char *) NULL;
    char                    *name, *entryname;
    char                    *ktname = NULL;
    boolean32               entry_only = FALSE, random_key = FALSE,
			    local_only = FALSE, noprivacy = FALSE,
                            authn_user = FALSE;
    dced_entry_t	    entry;
    dced_binding_handle_t   kt_handle;
    sec_login_handle_t	    login_context;
    dced_key_list_t         dced_key_list;
    handle_t                rpc_binding = NULL;
    char                    *binding_string = NULL;
    int                     argc1, argc2;
    char                    **argv1, **argv2;
    int                     i;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-annotation", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_annotation_help},
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_attribute_help},
        {"-data", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_data_help},
        {"-entry", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_dhd_entry_help},
        {"-ktname", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_ktname_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_local_help},
        {"-noprivacy", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
        dcp_t_kt_noprivacy_help},
        {"-storage", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_storage_help},
        {"-uuid", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_uuid_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &annotation;
    arg_table[1].dst = (char *) &attribute;
    arg_table[2].dst = (char *) &data;
    arg_table[3].dst = (char *) &entry_only;
    arg_table[4].dst = (char *) &ktname;
    arg_table[5].dst = (char *) &local_only;
    arg_table[6].dst = (char *) &noprivacy;
    arg_table[7].dst = (char *) &storage;
    arg_table[8].dst = (char *) &uuid;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV_2(argc1, argv1, argc2, argv2);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
  
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if ((attribute) && 
        ((uuid) || (annotation) || (storage) || (data))) {
            DCP_SET_MISC_ERROR(dcp_s_dhd_too_many_opts);
    }

    if ((!attribute) && (!storage)) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "attribute or storage");
        return TCL_ERROR;
    }

    if ((storage) && (!entry_only) && (!data)) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "data");
        return TCL_ERROR;
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list or the string bindings list if the user
     * entered string bindings.
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }
    
    if (ktname != NULL) {
        if (Tcl_SplitList(interp, ktname, &argc2, &argv2) != TCL_OK) {
            DCP_DHD_ARGV_FREE(argc1, argv1);
            return TCL_ERROR;
        }
        /* allow only 1 ktname per command */
        if (argc2 > 1) {
            DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
            DCP_SET_MISC_ERROR(dcp_s_too_many_args);
        }
	DCP_DHD_ARGV_FREE(argc2, argv2);
    }

    /*
     * Prepare the data
     */
    if (attribute) {
        if (dhd__parse_attr(interp, attribute, &entry, &data) != TCL_OK) {
		if (data) {
			free(data);
		}
                DCP_DHD_ARGV_FREE(argc1, argv1);
                return TCL_ERROR;
        }
    } /* End of if (attribute) */
    else {
        /*
         * data for entry
         */
	entry = s_null_entry;
        if (uuid) {
            uuid_from_string((unsigned char *) uuid, &entry.id, &status);   
	    if (!DCP_DHD_GOOD_STATUS(status)) {
		DCP_DHD_ARGV_FREE(argc1, argv1);
                DCP_SET_MISC_ERROR(status);
            } /* End of if (!DCP_DHD_GOOD_STATUS(status)) */
        }
	if (annotation) {
		entry.description = (dced_string_t) dce_strdup(annotation);
	}
	entry.storage_tag = (dced_string_t) dce_strdup(storage);
    } /* End of else of if (attribute) */
	
    /*
     * Check the entry_only and data availability
     */
    if ((entry_only) && (data)) {
       if (attribute) {
		free(data);
       }
       dced_entry_free_ptrs(&entry);
       DCP_DHD_ARGV_FREE(argc1, argv1);
       DCP_SET_MISC_ERROR(dcp_s_dhd_data_on_entry_ops);
    }
    if ((!entry_only) && (!data)) {
	dced_entry_free_ptrs(&entry);
	DCP_DHD_ARGV_FREE(argc1, argv1);
	DCP_SET_MISC_ERROR(dcp_s_dhd_data_attr_missing);
    }


    /*
     * Prepare key list
     */
    dced_key_list.count = 0;
    dced_key_list.list = NULL;
    if (!entry_only) {
	if (dhd__parse_keys(interp, data, random_key, &dced_key_list) 
							!= TCL_OK) {
    		if (attribute) {
			free(data);
    		}
		dced_entry_free_ptrs(&entry);
		DCP_DHD_ARGV_FREE(argc1, argv1);
		return TCL_ERROR;
	}
    }

    /*
     * If the data attribute is specified by attribute option, 
     * the data is malloc-ed by Tcl_Merge() in dhd__parse_attr()
     * and it need to be released by free().
     */  
    if ((attribute) && (data)) {
	free(data);
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i=0; i<argc1; i++) {
        
	if (dhd__parse_name(interp, argv1[i], dced_c_service_keytab,
			    dced_c_service_keytab_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

        /*
         * If we are processing a key table name, then the name
         * argument should be a string binding.  Use the name to
         * get an rpc binding.  If we cannot get a legal rpc binding,
         * then return an error.
         */
        if (ktname != NULL) {
	    /*
	     * The user may have entered the string bindings in tcl syntax.
	     * Assume that is true and convert them to string bindings.
	     * If not, the result will be NULL.  If so, the result
	     * is used during the call to get the rpc binding handle.
	     */
	    if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) != TCL_OK) {
                free(name);
                free(entryname);
		DCP_DHD_ARGV_FREE(argc1, argv1);
		return TCL_ERROR;
	    }
	    binding_string = (char *) dcp_binding_tcl_to_string(argc2, argv2, &status);
	    DCP_DHD_ARGV_FREE(argc2, argv2);
            if (binding_string == NULL) {
                binding_string = dce_strdup(name);
            }
            rpc_binding_from_string_binding((unsigned_char_t *)binding_string,
                                            &rpc_binding,
                                            &status);
            if (status != rpc_s_ok) {
                free(name);
                free(entryname);
                free(binding_string);
                DCP_SET_RESULT_CODE(status);
                break;
            }
	    free(binding_string);
	    binding_string = NULL;

            /*
             * Turn the rpc binding into a dced binding.
             */
            dced_binding_from_rpc_binding(dced_e_service_type_keytab,
                                          rpc_binding,
                                          &kt_handle,
                                          &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(name);
                free(entryname);
                rpc_binding_free(&rpc_binding, &status1);
                DCP_SET_RESULT_CODE(status);
                break;
            }
	    free(entryname);
        }
       	else {
	    /*
	     * Call dced_binding_create() to get binding
	     */
	    if (dcp_general_is_bound_locally(interp)) {
		dced_binding_create((dced_string_t)entryname, 
				    ((local_only) ? dced_c_binding_syntax_local :
				     dced_c_binding_syntax_default),
				    &kt_handle,
				    &status);
	    }
	    else {
		dced_binding_create((dced_string_t)entryname, 
				    ((local_only) ? dced_c_binding_syntax_bypass :
				     dced_c_binding_syntax_default), 
				    &kt_handle,
				    &status);
	    }
	    free(entryname);
	    DCP_DHD_CHECK_STATUS_BRK(status);
	}

	/*
         * If string binding is used, name holds a copy of the string binding,
	 * not the key table name. Free it and make another copy of the key
	 * table name to be assigned to entry.name below.
         */
	if (ktname != NULL) {
	    free((char *) name);
	    name = dce_strdup(ktname);
	}

	DCP_DHD_STRING_FREE(entry.name);
        entry.name = (dced_string_t) name;

	/*
       	 * Call dced_binding_set_auth_info to set the dced binding
       	 * if dce_login-ed
       	 */
       	if (authn_user) {
		dced_binding_set_auth_info(
                       kt_handle, 
                       (noprivacy) ? dhd_c_prot_level : dhd_c_keytab_prot_level,
                       dhd_c_authn_svc, 
                       (rpc_auth_identity_handle_t) login_context,
                       dhd_c_authz_svc, 
                       &status);
               	DCP_DHD_CHECK_STATUS_OUT(status);
       	}       

        /*
         * Call dced_entry_add() to create a  keytab configuration 
         * entry only or call dced_keytab_create() to create a keytab 
         * object 
         */
        if (entry_only) {
            dced_entry_add(kt_handle, &entry, &status);
        }   
        else {
            dced_keytab_create(kt_handle, &entry, &dced_key_list, &status);
        } 
	DCP_DHD_CHECK_STATUS_OUT(status);

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
       	dced_binding_free(kt_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

        /*
         * If a error has been encountered, out of for loop
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    } /* End of for (i=0; i<argc1; i++) */

    /*
     * Free memory and return right status
     */
    dced_entry_free_ptrs(&entry);
    if (!entry_only) {
    	dced_key_list_free_ptrs(&dced_key_list);
    }
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
} /* End of dcp_kt_create() */



/*
 * FUNCTION:    dcp_kt_delete 
 *
 * OVERVIEW:    Delete a keytab entry and the data associated
 *              with it on the specified host.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_kt_delete(ClientData    clientData, 
              Tcl_Interp    *interp, 
              int           argc, 
              char          **argv)
{
    char                    *name, *entryname;
    boolean32               entry_only = FALSE, local_only = FALSE,
                            noprivacy = FALSE, authn_user = FALSE;
    int                     argc1;
    char                    **argv1;
    dced_binding_handle_t   kt_handle;
    sec_login_handle_t	    login_context;
    uuid_t                  id;
    int                     i;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-entry", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_dhd_entry_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_local_help},
        {"-noprivacy", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
        dcp_t_kt_noprivacy_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &entry_only;
    arg_table[1].dst = (char *) &local_only;
    arg_table[2].dst = (char *) &noprivacy;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
  
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i=0; i<argc1; i++) {

	if (dhd__parse_name(interp, argv1[i], dced_c_service_keytab,
			    dced_c_service_keytab_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

       	/*
       	 * Call dced_binding_create() to get binding
       	 */
        if (dcp_general_is_bound_locally(interp)) {
            dced_binding_create((dced_string_t)entryname, 
                                ((local_only) ? dced_c_binding_syntax_local :
                                                dced_c_binding_syntax_default),
                                &kt_handle,
                                &status);
        }
        else {
            dced_binding_create((dced_string_t)entryname, 
                                ((local_only) ? dced_c_binding_syntax_bypass :
                                                dced_c_binding_syntax_default), 
                                &kt_handle,
                                &status);
        }
      	free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
    		free(name);
    		DCP_SET_RESULT_CODE(status);
       		break;
      	}

	/*
       	 * Call dced_binding_set_auth_info to set the dced binding
       	 * if dce_login-ed
       	 */
       	if (authn_user) {
		dced_binding_set_auth_info(
                       kt_handle, 
                       (noprivacy) ? dhd_c_prot_level : dhd_c_keytab_prot_level,
                       dhd_c_authn_svc, 
                       (rpc_auth_identity_handle_t) login_context,
                       dhd_c_authz_svc, 
                       &status);
               	DCP_DHD_CHECK_STATUS_OUT(status);
	}

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * need to call delete routine.
         */
         dced_inq_id(kt_handle, (dced_string_t) name, &id, &status);
	 DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_entry_remove() to delete a  keytab configuration 
         * entry only or call dced_keytab_delete() to delete a keytab 
         * configuration entry and keys 
         */
        if (entry_only) {
            dced_entry_remove(kt_handle, &id, &status);
        }   
        else {
            dced_keytab_delete(kt_handle, &id, &status);
        }
out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
       	dced_binding_free(kt_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

	free(name);
        /*
         * If a error has been met up to now, leave the for loop
         * and abort the rest of work.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    } /* End of for (i=0; i<argc1; i++) */

    /*
     * Free memory and return right status
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
} /* End of dcp_kt_delete() */


/*
 * FUNCTION:    dcp_kt_add 
 *
 * OVERVIEW:    add key(s) in a keytab on the specified host.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_kt_add(ClientData    clientData, 
           Tcl_Interp    *interp, 
           int           argc, 
           char          **argv)
{
    char                    *plain_key = (char *) NULL;
    char                    *principals = (char *) NULL;
    char                    *version = (char *) NULL;
    char                    *name, *entryname;
    boolean32               random_key = FALSE, update_registry = FALSE,
			    local_only = FALSE, noprivacy = FALSE,
                            authn_user = FALSE;
    char                    *ktname = NULL;
    dced_binding_handle_t   kt_handle;
    sec_login_handle_t	    login_context;
    handle_t                rpc_binding = NULL;
    char                    *binding_string = NULL;
    int                     argc1, argc2, argc3;
    char                    **argv1, **argv2, **argv3;
    int                     i, j;
    dced_key_t              dced_key;
    uuid_t                  id;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-key", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_key_help},
        {"-ktname", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_ktname_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_local_help},
        {"-member", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_member_help},
        {"-noprivacy", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
        dcp_t_kt_noprivacy_help},
        {"-random", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_random_help},
        {"-registry", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_registry_help},
        {"-version", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_version_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV_3(argc1, argv1, argc2, argv2, argc3, argv3);

    arg_table[0].dst = (char *) &plain_key;
    arg_table[1].dst = (char *) &ktname;
    arg_table[2].dst = (char *) &local_only;
    arg_table[3].dst = (char *) &principals;
    arg_table[4].dst = (char *) &noprivacy;
    arg_table[5].dst = (char *) &random_key;
    arg_table[6].dst = (char *) &update_registry;
    arg_table[7].dst = (char *) &version;

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
  
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (!principals) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "member");
        return TCL_ERROR;
    }

    if ((!plain_key) && (!random_key)) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "key or random");
        return TCL_ERROR;
    }
    if ((random_key) && (!update_registry)) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "registry");
        return TCL_ERROR;
    }

    if ((plain_key) && (!version)) {
        DCP_SET_MISC_ERROR(dcp_s_kt_missing_key_version);
    }

    if ((plain_key) && (random_key)) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }
                                                             
    if (ktname != NULL) {
        if (Tcl_SplitList(interp, ktname, &argc3, &argv3) != TCL_OK) {
            free((char *)argv1);
            return TCL_ERROR;
        }
        /* allow only 1 ktname per string binding */
        if (argc3 > 1) {
            DCP_DHD_ARGV_FREE_2(argc1, argv1, argc3, argv3);
            DCP_SET_MISC_ERROR(dcp_s_too_many_args);
        }
	DCP_DHD_ARGV_FREE(argc3, argv3);
    }

    /*
     * Split the principal names
     */
    if (Tcl_SplitList(interp, principals, &argc2, &argv2) != TCL_OK) {
	    DCP_DHD_ARGV_FREE(argc1, argv1);
            return TCL_ERROR;
    }

    /*
     * Prepare the key 
     */
    dced_key = s_null_key;
    dced_key.authn_service = dhd_c_key_authn_service;
    if (plain_key) {
    	DCP_DHD_MALLOC_RESET(dced_key.passwd, sec_passwd_rec_t, 1, status);
    	if (!DCP_DHD_GOOD_STATUS(status)) {
		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		return TCL_ERROR;
    	}
        *dced_key.passwd = s_null_passwd;
	dced_key.version = dced_key.passwd->version_number 
			 = atoi(version);
    	dced_key.passwd->key.key_type = sec_passwd_plain;
	dced_key.passwd->key.tagged_union.plain 
					= (unsigned char *) dce_strdup(plain_key);
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i=0; i<argc1; i++) {

	if (dhd__parse_name(interp, argv1[i], dced_c_service_keytab,
			    dced_c_service_keytab_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}


        /*
         * If we are processing a key table name, then the name
         * argument should be a string binding.  Use the name to
         * get an rpc binding.  If we cannot get a legal rpc binding,
         * then return an error.
         */
        if (ktname != NULL) {
	    /*
	     * The user may have entered the string bindings in tcl syntax.
	     * Assume that is true and convert them to string bindings.
	     * If not, the result will be NULL.  If so, the result
	     * is used during the call to get the rpc binding handle.
	     */
	    if (Tcl_SplitList(interp, argv1[i], &argc3, &argv3) != TCL_OK) {
                free(name);
                free(entryname);
		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		return TCL_ERROR;
	    }
	    binding_string = (char *) dcp_binding_tcl_to_string(argc3, argv3, &status);
	    DCP_DHD_ARGV_FREE(argc3, argv3);
	    if (binding_string == NULL) {
                binding_string = dce_strdup(name);
            }
            rpc_binding_from_string_binding((unsigned_char_t *)binding_string,
                                            &rpc_binding,
                                            &status);
            if (status != rpc_s_ok) {
                free(name);
                free(entryname);
                free(binding_string);
                DCP_SET_RESULT_CODE(status);
                break;
            }
	    free(binding_string);
	    binding_string = NULL;

            /*
             * Turn the rpc binding into a dced binding.
             */
            dced_binding_from_rpc_binding(dced_e_service_type_keytab,
                                          rpc_binding,
                                          &kt_handle,
                                          &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(name);
                free(entryname);
                rpc_binding_free(&rpc_binding, &status1);
                DCP_SET_RESULT_CODE(status);
                break;
            }
        }
       	else {
	    /*
	     * Call dced_binding_create() to get binding
	     */
	    if (dcp_general_is_bound_locally(interp)) {
		dced_binding_create((dced_string_t)entryname, 
				    ((local_only) ? dced_c_binding_syntax_local :
				     dced_c_binding_syntax_default),
				    &kt_handle,
				    &status);
	    }
	    else {
		dced_binding_create((dced_string_t)entryname, 
				    ((local_only) ? dced_c_binding_syntax_bypass :
				     dced_c_binding_syntax_default), 
				    &kt_handle,
				    &status);
	    }
	    free(entryname);
	    if (!DCP_DHD_GOOD_STATUS(status)) {
		free(name);
		DCP_SET_RESULT_CODE(status);
       		break;
	    }
	}

	/*
       	 * Call dced_binding_set_auth_info to set the dced binding
       	 * if dce_login-ed
       	 */
       	if (authn_user) {
       		 dced_binding_set_auth_info(
                       kt_handle, 
                       (noprivacy) ? dhd_c_prot_level : dhd_c_keytab_prot_level,
                       dhd_c_authn_svc, 
                       (rpc_auth_identity_handle_t) login_context,
                       dhd_c_authz_svc, 
                       &status);
               	DCP_DHD_CHECK_STATUS_OUT(status);
	}

 	/*
         * If string binding was used, name holds a copy of the string binding.
         * Free it and make another copy of the key table name for use in the
         * next calls to dced_inq_id() and strcasecmp()
         */
	if (ktname != NULL) {
	    free((char *) name);
	    name = dce_strdup(ktname);
	}

	/*
         * Call dced_inq_id() to convert the name into uuid which is
         * needed to call the key add routine.
         */
        dced_inq_id(kt_handle, (dced_string_t) name, &id, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_keytab_add() or dced_keytab_change(), if -registry is
	 * presented, to add the key. 
         */
	for (j=0; j<argc2; j++) {
		DCP_DHD_STRING_FREE(dced_key.principal);
		dced_key.principal = (dced_string_t) dce_strdup(argv2[j]);	
		if (update_registry) {
			dced_keytab_change_key(kt_handle, 
					       &id, 
					       &dced_key,
					       &status);
		}
		else {
			dced_keytab_add_key(kt_handle,
					    &id,
					    &dced_key,
					    &status);
		}
		DCP_DHD_CHECK_STATUS_OUT(status);
	} /* End of for (j=0; j<argc2; j++) */
out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
       	dced_binding_free(kt_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

	free(name);
        /*
         * Get out the for loop if a error has been met up to now
         * and abort the rest of work.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    } /* End of for (i=0; i<argc1; i++) */

    /*
     * Free memory and return right status
     */
    dced_key_free_ptrs(&dced_key);
    DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
    DCP_DHD_RETURN(status);
} /* End of dcp_kt_add() */


/*
 * FUNCTION:    dcp_kt_remove 
 *
 * OVERVIEW:    Remove a key from a keytab on the specified host.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_kt_remove(ClientData    clientData, 
              Tcl_Interp    *interp, 
              int           argc, 
              char          **argv)
{
    char                    *principals = (char *) NULL;
    char                    *key_type = (char *) NULL;
    char                    *version = (char *) NULL;
    boolean32		    local_only = FALSE, noprivacy = FALSE,
                            authn_user = FALSE;
    char                    *name, *entryname;
    dced_binding_handle_t   kt_handle;
    sec_login_handle_t	    login_context;
    int                     argc1, argc2, argc3;
    char                    **argv1, **argv2, **argv3;
    int                     i, j, k;
    dced_key_t              dced_key;
    uuid_t                  id;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
	{"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_local_help},
        {"-member", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_member_help},
        {"-noprivacy", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
        dcp_t_kt_noprivacy_help},
        {"-type", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_key_help},
        {"-version", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_version_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &local_only;
    arg_table[1].dst = (char *) &principals;
    arg_table[2].dst = (char *) &noprivacy;
    arg_table[3].dst = (char *) &key_type;
    arg_table[4].dst = (char *) &version;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV_3(argc1, argv1, argc2, argv2, argc3, argv3);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
  
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (!principals) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "member");
        return TCL_ERROR;
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }
                                                             
    /*
     * Parse the principal names
     */
    if (Tcl_SplitList(interp, principals, &argc2, &argv2) != TCL_OK) {
            DCP_DHD_ARGV_FREE(argc1, argv1);
            return TCL_ERROR;
    }

    /*
     * Parse the version numbers
     */
    if (version) {
    	if (Tcl_SplitList(interp, version, &argc3, &argv3) != TCL_OK) {
	     DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
             return TCL_ERROR;
    	}
    }	

    /*
     * Prepare the key 
     */
    dced_key = s_null_key;
    dced_key.authn_service = dhd_c_key_authn_service;
    if (key_type) {
    	DCP_DHD_MALLOC_RESET(dced_key.passwd, sec_passwd_rec_t, 1, status);
    	if (!DCP_DHD_GOOD_STATUS(status)) {
		DCP_DHD_ARGV_FREE_3(argc1, argv1, argc2, argv2, argc3, argv3);
		return TCL_ERROR;
    	}
    	*dced_key.passwd = s_null_passwd;
    	if (!strcasecmp(key_type, dhd_c_key_type_plain_token)) {
		dced_key.passwd->key.key_type = sec_passwd_plain;
    	} /* End of if (!strcasecmp(key_type, dhd_c_key_type_plain_token)) */
    	else if (!strcasecmp(key_type, dhd_c_key_type_des_token)) {
		dced_key.passwd->key.key_type = sec_passwd_des;
    	} /* End of if (!strcasecmp(key_type, dhd_c_key_type_des_token)) */
    } /* End of if (key_type) */


    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i=0; i<argc1; i++) {

	if (dhd__parse_name(interp, argv1[i], dced_c_service_keytab,
			    dced_c_service_keytab_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

       	/*
       	 * Call dced_binding_create() to get binding
       	 */
        if (dcp_general_is_bound_locally(interp)) {
            dced_binding_create((dced_string_t)entryname, 
                                ((local_only) ? dced_c_binding_syntax_local :
                                                dced_c_binding_syntax_default),
                                &kt_handle,
                                &status);
        }
	else {
            dced_binding_create((dced_string_t)entryname, 
                                ((local_only) ? dced_c_binding_syntax_bypass :
                                                dced_c_binding_syntax_default), 
                                &kt_handle,
                                &status);
        }
       	free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
		free(name);
		DCP_SET_RESULT_CODE(status);
		break;
       	}

	/*
       	 * Call dced_binding_set_auth_info to set the dced binding
       	 * if dce_login-ed
       	 */
       	if (authn_user) {
       		dced_binding_set_auth_info(
                       kt_handle, 
                       (noprivacy) ? dhd_c_prot_level : dhd_c_keytab_prot_level,
                       dhd_c_authn_svc, 
                       (rpc_auth_identity_handle_t) login_context,
                       dhd_c_authz_svc, 
                       &status);
               	DCP_DHD_CHECK_STATUS_OUT(status);
	}

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * needed to call the key remove routine.
         */
        dced_inq_id(kt_handle, (unsigned char *) name, &id, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_keytab_remove() to delete the key for keytab. 
         */
	for (j=0; j<argc2; j++) {
		DCP_DHD_STRING_FREE(dced_key.principal);
		dced_key.principal = (dced_string_t) dce_strdup(argv2[j]);	
		if (!version) {
			dced_keytab_remove_key(kt_handle, 
					       &id, 
					       &dced_key,
					       &status);
			DCP_DHD_CHECK_STATUS_OUT(status);
		} /* End of if (!version) */
		else {
			for (k=0; k<argc3; k++) {
				dced_key.version = atoi(argv3[k]);
				if (dced_key.passwd) {
					dced_key.passwd->key.key_type 
							= dced_key.version;
				}
				dced_keytab_remove_key(kt_handle, 
						       &id, 
						       &dced_key,
						       &status);
				DCP_DHD_CHECK_STATUS_OUT(status);
        		} /* End of for (k=0; k<argv3; k++) */
		} /* End of else of if (!version) */
	} /* End of for (j=0; j<argc2; j++) */
out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
       	dced_binding_free(kt_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

	free(name);
        /*
         * Get out the for loop if a error has been met up to now
         * and abort the rest of work.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    } /* End of for (i=0; i<argc1; i++) */

    /*
     * Free memory and return right status
     */
    dced_key_free_ptrs(&dced_key);
    DCP_DHD_ARGV_FREE_3(argc1, argv1, argc2, argv2, argc3, argv3);
    DCP_DHD_RETURN(status);
} /* End of dcp_kt_remove() */



/*
 * FUNCTION:    dcp_kt_show 
 *
 * OVERVIEW:    Show the contents of a keytab object if -members 
 *		switch is present or just keytab entry if -entry switch 
 *		is present or both if neither of them is present.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_kt_show(ClientData  clientData, 
            Tcl_Interp  *interp, 
            int         argc, 
            char        **argv)
{
    char                    *name, *entryname;
    boolean32               entry_only = FALSE, members_only = FALSE,
			    key_value = FALSE, local_only = FALSE,
                            noprivacy = FALSE, authn_user = FALSE;
    char                    *ktname = NULL;
    dced_binding_handle_t   kt_handle;
    sec_login_handle_t	    login_context;
    dced_entry_t            *entry_p;
    dced_key_list_t         *dced_keytab_p;
    dced_cursor_t           kt_cursor;
    int                     i;
    uuid_t                  id;
    handle_t                rpc_binding = NULL;
    char                    *binding_string = NULL;
    int                     argc1, argc2;
    char                    **argv1, **argv2;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-entry", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_dhd_entry_help},
        {"-keys", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_keys_help},
        {"-ktname", DCP_ARGV_STRING, NULL, NULL,
        dcp_t_kt_ktname_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_local_help},
        {"-members", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_members_help},
        {"-noprivacy", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
        dcp_t_kt_noprivacy_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &entry_only;
    arg_table[1].dst = (char *) &key_value;
    arg_table[2].dst = (char *) &ktname;
    arg_table[3].dst = (char *) &local_only;
    arg_table[4].dst = (char *) &members_only;
    arg_table[5].dst = (char *) &noprivacy;
    
    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV_2(argc1, argv1, argc2, argv2);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if ((entry_only) && (members_only)) {
	DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    if (ktname != NULL) {
        if (Tcl_SplitList(interp, ktname, &argc2, &argv2) != TCL_OK) {
            free((char *)argv1);
            return TCL_ERROR;
        }
	/* allow only 1 ktname per command */
        if (argc2 > 1) {
            DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
            DCP_SET_MISC_ERROR(dcp_s_too_many_args);
        }
        DCP_DHD_ARGV_FREE(argc2, argv2);
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i=0; i<argc1; i++) {

	if (dhd__parse_name(interp, argv1[i], dced_c_service_keytab,
			    dced_c_service_keytab_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

        /*
         * If we are processing a key table name, then the name
         * argument should be a string binding.  Use the name to
         * get an rpc binding.  If we cannot get a legal rpc binding,
         * then return an error.
         */
        if (ktname != NULL) {
	    /*
	     * The user may have entered the string bindings in tcl syntax.
	     * Assume that is true and convert them to string bindings.
	     * If not, the result will be NULL.  If so, the result
	     * is used during the call to get the rpc binding handle.
	     */
	    if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) != TCL_OK) {
                free(name);
                free(entryname);
		DCP_DHD_ARGV_FREE(argc1, argv1);
		return TCL_ERROR;
	    }
	    binding_string = (char *) dcp_binding_tcl_to_string(argc2, argv2, &status);
	    DCP_DHD_ARGV_FREE(argc2, argv2);
	    if (binding_string == NULL) {
                binding_string = dce_strdup(name);
            }
            rpc_binding_from_string_binding((unsigned_char_t *)binding_string,
                                            &rpc_binding,
                                            &status);
            if (status != rpc_s_ok) {
                free(name);
                free(entryname);
                free(binding_string);
                DCP_SET_RESULT_CODE(status);
                break;
            }
	    free(binding_string);
	    binding_string = NULL;

            /*
             * Turn the rpc binding into a dced binding.
             */
            dced_binding_from_rpc_binding(dced_e_service_type_keytab,
                                          rpc_binding,
                                          &kt_handle,
                                          &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(name);
                free(entryname);
                rpc_binding_free(&rpc_binding, &status1);
                DCP_SET_RESULT_CODE(status);
                break;
            }
        }
       	else {
	    /*
	     * Call dced_binding_create() to get binding
	     */
	    if (dcp_general_is_bound_locally(interp)) {
		dced_binding_create((dced_string_t)entryname, 
				    ((local_only) ? dced_c_binding_syntax_local :
				     dced_c_binding_syntax_default),
				    &kt_handle,
				    &status);
	    }
	    else {
		dced_binding_create((dced_string_t)entryname, 
				    ((local_only) ? dced_c_binding_syntax_bypass :
				     dced_c_binding_syntax_default), 
				    &kt_handle,
				    &status);
	    }
	    free(entryname);
	    if (!DCP_DHD_GOOD_STATUS(status)) {
		free(name);
		DCP_SET_RESULT_CODE(status);
		break;
	    }
	}

	/*
       	 * Call dced_binding_set_auth_info to set the dced binding
       	 * if dce_login-ed
       	 */
       	if (authn_user) {
               	dced_binding_set_auth_info(
                       kt_handle, 
                       (noprivacy) ? dhd_c_prot_level : dhd_c_keytab_prot_level,
                       dhd_c_authn_svc, 
                       (rpc_auth_identity_handle_t) login_context,
                       dhd_c_authz_svc, 
                       &status);
               	DCP_DHD_CHECK_STATUS_OUT(status);
        }

	/*
         * If string binding was used, name holds a copy of the string binding.
         * Free it and make another copy of the key table name for use in the
         * next calls to dced_inq_id() and strcasecmp()
         */
	if (ktname != NULL) {
	    free((char *) name);
	    name = dce_strdup(ktname);
	}

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * needed to call the key read routine.
         */
        dced_inq_id(kt_handle, (dced_string_t) name, &id, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Insert a newline between output of objects
         */
        if (i) {
                Tcl_AppendResult(interp, "\n", (char *) NULL);
        }

	/*
	 * Show the keytab entry attribute
	 */
	if (!members_only) {
      	 	 /*
       		  * Call dced_initialize_cursor() to get the cursor of
       		  * the object list
       		  */
       		 dced_initialize_cursor(kt_handle, &kt_cursor, &status);
		 DCP_DHD_CHECK_STATUS_OUT(status);

       		 /*
      	 	  * Call dced_entry_get_next() to traverse the object list
       		  */
       		 for ( ; ; ) {
       		     dced_entry_get_next(kt_cursor, &entry_p, &status);
       		     if (DCP_DHD_GOOD_STATUS(status) && 
       		         (!strcasecmp(name, (char *) entry_p->name))) {
       		         /*
       		          * Display the object list and then
       		          * Out of for ( ; ; ) loop.
       		          */
       		         dhd__show_entry(interp, entry_p, NULL, dhd_e_show_all);
       			 break;
       		     }
       		     else if (!DCP_DHD_GOOD_STATUS(status)) {
      	       	  	 break;
       	     	     }
       	 	}

       		/*  
       		 * Call dced_release_cursor() to release the cursor
       	  	 */
       		dced_release_cursor(&kt_cursor, &status1);
		if (DCP_DHD_GOOD_STATUS(status) && 
		    !DCP_DHD_GOOD_STATUS(status1)) {
			DCP_DHD_SET_STATUS(status, status1);
		}
		DCP_DHD_CHECK_STATUS_OUT(status);

	} /* End of if (!members_only) */	

        /*
         * Call dced_object_read() to read the keytab object. 
         */
        if (!entry_only) {
		dced_object_read(kt_handle, &id,
                                 (void **) &dced_keytab_p, &status);
		DCP_DHD_CHECK_STATUS_OUT(status);

		/*
		 * Show the keytab if read success
		 */
		dhd__show_object(
		     interp, 
		     (void *) dced_keytab_p,
		     dced_e_service_type_keytab, 
		     (key_value) ? dhd_e_show_all : dhd_e_show_excl_key_value);

		/*
		 * Relase the keytab after showing
		 */
		dced_objects_release(kt_handle,
				     1,
				     (void *) dced_keytab_p,
				     &status);
        } /* End of if (!entry_only) */
out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
       	dced_binding_free(kt_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

	free(name);
        /*
         * If a error has been met up to now, abort the
         * operation to show rest of names in the list
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    } /* End of for (i=0; i<argc1; i++) */

    /*
     * Free the memory and return right status 
     */
    DCP_DHD_CLEAR_ARGV(argc1, argv1);
    DCP_DHD_RETURN(status);
} /* End of dcp_kt_show() */



/*
 * FUNCTION:    dcp_kt_list 
 *
 * OVERVIEW:    Show all the principals of a keytab object on a
 *		specified host.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_kt_list(ClientData  clientData, 
            Tcl_Interp  *interp, 
            int         argc, 
            char        **argv)
{
    char                    *name, *entryname;
    dced_binding_handle_t   kt_handle;
    sec_login_handle_t	    login_context;
    boolean32		    authn_user = FALSE, local_only = FALSE,
                            noprivacy = FALSE;
    dced_key_list_t         *dced_keytab_p;
    int                     i;
    uuid_t                  id;
    int                     argc1;
    char                    **argv1;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
        dcp_t_kt_local_help},
        {"-noprivacy", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
        dcp_t_kt_noprivacy_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &local_only;
    arg_table[1].dst = (char *) &noprivacy;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i=0; i<argc1; i++) {

	if (dhd__parse_name(interp, argv1[i], dced_c_service_keytab,
			    dced_c_service_keytab_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

       	/*
       	 * Call dced_binding_create() to get binding
       	 */
        if (dcp_general_is_bound_locally(interp)) {
            dced_binding_create((dced_string_t)entryname, 
                                ((local_only) ? dced_c_binding_syntax_local :
                                                dced_c_binding_syntax_default),
                                &kt_handle,
                                &status);
        }
	else {
            dced_binding_create((dced_string_t)entryname, 
                                ((local_only) ? dced_c_binding_syntax_bypass :
                                                dced_c_binding_syntax_default), 
                                &kt_handle,
                                &status);
        }
       	free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
		free(name);
		DCP_SET_RESULT_CODE(status);
		break;
       	}

	/*
       	 * Call dced_binding_set_auth_info to set the dced binding
       	 * if dce_login-ed
       	 */
       	if (authn_user) {
               	dced_binding_set_auth_info(
                       kt_handle, 
                       (noprivacy) ? dhd_c_prot_level : dhd_c_keytab_prot_level,
                       dhd_c_authn_svc, 
                       (rpc_auth_identity_handle_t) login_context,
                       dhd_c_authz_svc, 
                       &status);
               	DCP_DHD_CHECK_STATUS_OUT(status);
        }       

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * needed call the key read routine.
         */
        dced_inq_id(kt_handle, (dced_string_t) name, &id, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Insert a newline between output of objects
         */
        if (i) {
                Tcl_AppendResult(interp, "\n", (char *) NULL);
        }

        /*
         * Call dced_object_read() to read the keytab object. 
         */
	dced_object_read(kt_handle, &id, (void **) &dced_keytab_p, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

	/*
	 * Show the keytab name if read success
	 */
        dhd__show_object(interp, 
			 (void *) dced_keytab_p,
                         dced_e_service_type_keytab, 
                         dhd_e_show_name);   
	/*
	 * Relase the keytab after use
	 */
	dced_objects_release(kt_handle,
			     1,
			     (void *) dced_keytab_p,
			     &status);
out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
       	dced_binding_free(kt_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

	free(name);
        /*
         * If a error has been met up to now, abort the
         * operation to show rest of names in the list
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    } /* End of for (i=0; i<argc1; i++) */

    /*
     * Free the memory and return right status 
     */
    DCP_DHD_CLEAR_ARGV(argc1, argv1);
    DCP_DHD_RETURN(status);
} /* End of dcp_kt_list() */


