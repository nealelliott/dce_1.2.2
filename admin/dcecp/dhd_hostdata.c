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
 * $Log: dhd_hostdata.c,v $
 * Revision 1.1.6.1  1996/08/09  11:47:13  arvind
 * 	Merge changes.
 * 	[1996/06/11  14:14 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/04/26  19:24 UTC  truitt  /main/HPDCE02/9]
 *
 * 	Merge changes.
 * 	[1996/04/26  19:23 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf73/1]
 *
 * 	CHFts18165: Allow unauthenticated access in hostdata 'catalog' and 'show'
 * 	by introducing a new option.
 * 	[1996/03/18  17:40 UTC  truitt  /main/HPDCE02/8]
 *
 * 	Merge.
 * 	[1996/03/18  17:39 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1995/11/13  14:39 UTC  truitt  /main/HPDCE02/7]
 *
 * 	Merge local changes.
 * 	[1995/11/13  14:38 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts51/1]
 *
 * 	CHFts16779: Local mode problem when creating a hostdata object.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/11  13:50 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:13:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:58  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:20:46  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/10/19  17:07 UTC  truitt
 * 	Merge local work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts50/1  1995/10/19  17:06 UTC  truitt
 * 	CHFts16529: Correct order of hostdata options.  Fix malloc's and free's.
 * 
 * 	HP revision /main/HPDCE02/5  1995/09/29  20:47 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  16:49 UTC  tatsu_s
 * 	Added dhd__schema_ent_free_ptrs() kluge.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  00:51 UTC  tatsu_s
 * 	Added -local option.
 * 
 * 	HP revision /main/HPDCE02/4  1995/07/21  18:46 UTC  truitt
 * 	Merge hostdata show fixes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts41/1  1995/07/21  18:45 UTC  truitt
 * 	CHFts15790: Correct free of variables during some error cases in hostdata show.
 * 
 * 	HP revision /main/HPDCE02/3  1995/06/27  17:12 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts36/1  1995/06/27  17:11 UTC  truitt
 * 	CHFts15407: Allow string binding to be specified as a legal hostdata name.
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  17:44 UTC  truitt
 * 	Merge ordering work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:43 UTC  truitt
 * 	CHFts15237: Correct order of modify options.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  19:42 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chk_01/1  1994/12/15  19:38 UTC  truitt
 * 	The encoding type passed to get_attr_values()
 * 	has to come from the schema structure, not
 * 	from the attribute structure.
 * 	[1995/12/08  17:55:39  root]
 * 
 * Revision 1.1.2.14  1994/10/03  21:02:10  pwang
 * 	Fixed binary value parsed [OT#12468]
 * 	[1994/10/03  20:59:36  pwang]
 * 
 * Revision 1.1.2.13  1994/08/25  20:45:12  pwang
 * 	Eched the option(s) which is not entered [OT#11710]
 * 	Converted dced_s_no_more_entries into a better
 * 	understood msg [OT#11663]
 * 	[1994/08/25  20:42:47  pwang]
 * 
 * Revision 1.1.2.12  1994/08/08  18:51:43  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:12  melman]
 * 
 * Revision 1.1.2.11  1994/07/22  21:40:46  pwang
 * 	Replaced intermediate attr schema with dce_attr
 * 	schema interface [OT#10262]
 * 	[1994/07/22  19:42:56  pwang]
 * 
 * Revision 1.1.2.10  1994/07/08  21:18:01  pwang
 * 	Allowed unauthenticated access to dced [OT#11214]
 * 	[1994/07/08  21:14:47  pwang]
 * 
 * Revision 1.1.2.9  1994/07/07  20:53:38  coren
 * 	Changed call to get_attr_values for new signature.
 * 	[1994/07/05  21:43:18  coren]
 * 
 * Revision 1.1.2.8  1994/07/05  16:30:06  pwang
 * 	Properly released binding of attr_schema and modified mis-useed
 * 	attributes [OT#11058, #11020]
 * 	[1994/07/05  16:22:36  pwang]
 * 
 * Revision 1.1.2.7  1994/06/22  16:08:21  pwang
 * 	Used attr_schema of dced and Era routines of rgy_util.c
 * 	and checked the return of dced_binding_create() properly
 * 	[OT# 10464 10948]
 * 
 * Revision 1.1.2.6  1994/06/09  16:02:01  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:30  devsrc]
 * 
 * Revision 1.1.2.5  1994/05/31  21:09:55  pwang
 * 	Check -entry and -data to make sure they are mutual
 * 	excl. (OT#10762)
 * 	[1994/05/31  20:57:48  pwang]
 * 
 * Revision 1.1.2.4  1994/05/18  21:57:22  pwang
 * 	Merged with changes from 1.1.2.3
 * 	[1994/05/18  21:56:42  pwang]
 * 
 * 	Updated to latest RFC42.3 and added binary support and simplename
 * 	switch and others.
 * 	[1994/05/18  21:27:04  pwang]
 * 
 * Revision 1.1.2.3  1994/05/06  14:21:39  pwang
 * 	Changed the status code dced_s_no_more_entry to
 * 	dced_s_no_more_entries [OT#10554]
 * 	[1994/05/06  14:19:34  pwang]
 * 
 * Revision 1.1.2.2  1994/05/05  14:56:56  pwang
 * 	Used DCP_ARGV_NO_OPT_LEFTOVERS flag to detect illegal options.
 * 	Made corresponding name changes due to the changes in dced.
 * 	[1994/05/05  14:51:22  pwang]
 * 
 * Revision 1.1.2.1  1994/04/22  19:59:31  pwang
 * 	Initial Implementation
 * 	[1994/04/22  19:51:19  pwang]
 * 
 * $EndLog$
 */

/*
 * MODULE:
 * dhd_hostdata.c
 *
 * DESCRIPTION:
 * This module contains all the dced hostdata operations to access 
 * or manipulate data in a specified host thru dcecp.
 */

/*
 * INCLUDE FILES
 */
#include <dhd.h>
#include <util_binding.h>
#include <dce/rs_attr_schema.h>

/*
 * DECLARATIONS 
 */

extern rs_attr_schema_v0_0_epv_t	dced_rs_attr_schema_v0_0_epv;

/*
 * INTERNAL VARIABLES 
 */
static	dced_entry_t			s_null_entry;
static	sec_attr_t			s_null_sec_attr;
static	dce_attr_bind_auth_info_t	s_null_auth_info;


/*
 * FUNCTION:    dcp_hd_catalog 
 *
 * OVERVIEW:    Returns a list of the names of all hostdata objects 
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
dcp_hd_catalog(ClientData clientData, 
               Tcl_Interp *interp, 
               int        argc, 
               char       **argv)
{
    char                   *entryname;
    char                   *hostname;
    char                   *fullname = NULL;
    int                    argc1;
    char                   **argv1;
    boolean32              simplename = FALSE;
    int                    i, number_of_hosts;
    dced_binding_handle_t  hd_handle;
    sec_login_handle_t     login_context;
    boolean32              authn_user = FALSE;
    dced_cursor_t          hd_cursor;
    dced_entry_t           *entry_p;
    handle_t               rpc_binding = NULL;
    char                   *binding_string = NULL;
    error_status_t         status, status1;
    boolean32		   local_only = FALSE;
    boolean32              unauth_only = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_local_help},
        {"-simplename", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_simplename_help},
        {"-unauth", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_unauth_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&local_only;
    arg_table[1].dst = (char *)&simplename;
    arg_table[2].dst = (char *)&unauth_only;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

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

    if (!unauth_only) {
        /*
         * Check to see if this user is unauthenticated or authenticated 
         */
        sec_login_get_current_context(&login_context, &status);
        if (DCP_DHD_GOOD_STATUS(status)) {
            authn_user = TRUE;
        }
    }

    /*
     * Loop through the hostname list
     */
    for (i = 0; i < number_of_hosts; i++) {
        /*
         * If we are doing more than one host, leave some space.
         */
        if (i) {
            Tcl_AppendResult(interp, "\n", (char *) NULL);
            hostname = argv1[i];
        }

        /*
         * Parse the hostname.
         */
        if (dhd__parse_name(interp,
                            NULL,
                            dced_c_service_hostdata,
                            dced_c_service_hostdata_len,
                            hostname,
                            &entryname, 
                            NULL,
                            (simplename) ? NULL : &fullname) != TCL_OK) { 
            DCP_DHD_SET_STATUS(status, ~error_status_ok);
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

            dced_binding_from_rpc_binding(dced_e_service_type_hostdata,
                                          rpc_binding,
                                          &hd_handle,
                                          &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(binding_string);
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
             * Use dced_binding_create() to get dced binding handle.
             */
            dced_binding_create((dced_string_t)entryname,
                                ((local_only) ? dced_c_binding_syntax_local :
                                                dced_c_binding_syntax_default),
                                &hd_handle,
                                &status);
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
         * Call dced_binding_set_auth_info to set the dced binding if
         * operating as an authenticated user.
         */
        if (authn_user) {
            dced_binding_set_auth_info(hd_handle,
                                       dhd_c_prot_level,
                                       dhd_c_authn_svc, 
                                       (rpc_auth_identity_handle_t)login_context,
                                       dhd_c_authz_svc,
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Call dced_initialize_cursor() to get the cursor of 
         * the object list
         */
        dced_initialize_cursor(hd_handle, &hd_cursor, &status); 
        DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_entry_get_next() to traverse the object list
         */
        for ( ; ; ) {
            dced_entry_get_next(hd_cursor, &entry_p, &status);
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
                 * Break out of forever loop, no more to read.
                 */
                DCP_DHD_RESET_STATUS(status);
                break;
            } 
            else {
                break;
            }
        }

        /*
         * Release the dced object cursor
         */
        dced_release_cursor(&hd_cursor, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

out:
        /*
         * Free the dced binding handle
         */
        if (binding_string != NULL)
            free(binding_string);
        dced_binding_free(hd_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        if (!simplename) {
            free(fullname);	
        }

        /*
         * Break out of do loop, if a error has been seen.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free the memory and return right status
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_hd_create 
 *
 * OVERVIEW:    Create a hostdata entry and the data associated
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
 * OUTPUTS:
 *    UUID of the created object 
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_hd_create(ClientData clientData, 
              Tcl_Interp *interp, 
              int        argc, 
              char       **argv)
{
    char               			*annotation = (char *) NULL;
    char               			*attribute = (char *) NULL;
    char                    		*data = (char *) NULL;
    boolean32               		binary = FALSE;
    boolean32               		entry_only = FALSE;
    boolean32				local_only = FALSE;
    char               			*storage = (char *) NULL;
    char               			*uuid = (char *) NULL;
    char                    		*name, *entryname;
    dced_binding_handle_t		hd_handle;
    sec_login_handle_t			login_context;
    dced_entry_t            		entry;
    extern dce_attr_sch_handle_t	attr_schema_binding;
    dce_attr_component_name_t		schema_name;
    dce_attr_bind_auth_info_t		auth_info;
    dce_attr_schema_entry_t		schema_entry;
    uuid_t				attr_id;
    sec_attr_encoding_t			sec_attr_encoding_type;
    sec_attr_t              		sec_attr;
    dced_attr_list_t        		dced_attr_list;
    int                     		argc1, argc2, tmpac;
    char                    		**argv1, **argv2, **tmpav;
    int                     		i;
    error_status_t          		status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-annotation", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_hd_annotation_help},
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_hd_attribute_help},
        {"-binary", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_binary_help},
        {"-data", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_hd_data_help},
        {"-entry", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_dhd_entry_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_local_help},
        {"-storage", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_hd_storage_help},
        {"-uuid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_hd_uuid_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &annotation;
    arg_table[1].dst = (char *) &attribute;
    arg_table[2].dst = (char *) &binary;
    arg_table[3].dst = (char *) &data;
    arg_table[4].dst = (char *) &entry_only;
    arg_table[5].dst = (char *) &local_only;
    arg_table[6].dst = (char *) &storage;
    arg_table[7].dst = (char *) &uuid;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV_3(argc1, argv1, argc2, argv2, tmpac, tmpav);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

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
     * Split the name list 
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
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
    }
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
            } 
        }
	if (annotation) {
        	entry.description = (dced_string_t) dce_strdup(annotation);
	}
       	entry.storage_tag = (dced_string_t) dce_strdup(storage);
    }

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
     * data for the object
     */
    if (!entry_only) {
    	attr_id = (binary) ? dced_g_uuid_binfileattr : dced_g_uuid_fileattr;
    	/*
   	 * The reason we need to use tmpac/tmpav and copy it to
         * argc2/argv2 is the get_attr_values() in rgy_utils.c
         * always treats the attr values starting from argv[1].
         */
        if (binary) {
    	    argc2 = 2;
    	    DCP_DHD_ARGV_ALLOC_ST(argc2, argv2, status);
    	    if (!DCP_DHD_GOOD_STATUS(status)) {
		if (attribute) {
			free(data);
		}
		dced_entry_free_ptrs(&entry);
		DCP_DHD_ARGV_FREE(argc1, argv1);
		return TCL_ERROR;
    	    }
            argv2[0] = "bindata";
            argv2[1] = data;
        }
        else {
            if (Tcl_SplitList(interp, data, &tmpac, &tmpav) != TCL_OK) {
		if (attribute) {
			free(data);
		}
		dced_entry_free_ptrs(&entry);
        	DCP_DHD_ARGV_FREE(argc1, argv1);
       		return TCL_ERROR;
            }
    	    argc2 = tmpac + 1;
    	    DCP_DHD_ARGV_ALLOC_ST(argc2, argv2, status);
    	    if (!DCP_DHD_GOOD_STATUS(status)) {
		if (attribute) {
			free(data);
		}
		dced_entry_free_ptrs(&entry);
		DCP_DHD_ARGV_FREE_2(argc1, argv1, tmpac, tmpav);
		return TCL_ERROR;
    	    }
            argv2[0] = "data";
            argv2[1] = "";
            for (i = 1; i < argc2; i++) {
                argv2[i] = tmpav[i-1];
            }
    	}
	dced_attr_list.count = 1; 
	dced_attr_list.list = &sec_attr; 
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    auth_info = s_null_auth_info;
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
	auth_info.info_type = sec_attr_bind_auth_dce;
	auth_info.tagged_union.dce_info.protect_level = dhd_c_prot_level;
	auth_info.tagged_union.dce_info.authn_svc = dhd_c_authn_svc;
	auth_info.tagged_union.dce_info.authz_svc = dhd_c_authz_svc;
    }
	
    for (i=0; i<argc1; i++) {
        
	if (dhd__parse_name(interp, argv1[i], dced_c_service_hostdata,
			    dced_c_service_hostdata_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
        } 

        /*
         * data for the object
         */
        if (!entry_only) {
            /*
             * Create the dce_attr_sch binding
             */
            if (local_only) {
                dce_attr_sch_bind_local(0,
                                        &dced_rs_attr_schema_v0_0_epv,
                                        (dce_attr_component_name_t)dhd_c_attr_schema_token,
                                        &attr_schema_binding,
                                        &status);
            }
            else {
                if (dhd__schema_name_from_dced_name(interp, entryname, &schema_name) != TCL_OK) {
                    free(entryname);
                    free(name);
                    DCP_DHD_SET_STATUS(status, ~error_status_ok);
                    break;
                }
                dce_attr_sch_bind(schema_name,
                                  &auth_info, 
                                  &attr_schema_binding,
                                  &status);
                free((dce_attr_component_name_t)schema_name);
            }

            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(entryname);
                free(name);
                DCP_SET_RESULT_CODE(status);
                break;
            }

            /*
             * Lookup the entry by attr_id
             */
            dce_attr_sch_lookup_by_id(attr_schema_binding,
                                      &attr_id,
                                      &schema_entry,
                                      &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
               free(entryname);
               free(name);
               dce_attr_sch_bind_free(&attr_schema_binding, &status1);
               DCP_SET_RESULT_CODE(status);
               break;
            }

            /*
             * It should be sec_attr_enc_bytes or sec_attr_enc_printstring_array
             */
            sec_attr_encoding_type = schema_entry.attr_encoding;
            dhd__schema_ent_free_ptrs(&schema_entry);

            if ((i) && (sec_attr_encoding_type != sec_attr.attr_value.attr_encoding)) {
                sec_attr_util_inst_free_ptrs(&sec_attr);
            }

            if ((!i) || (sec_attr_encoding_type != sec_attr.attr_value.attr_encoding)) {
                sec_attr = s_null_sec_attr;
                status = get_attr_values(interp,
                                         sec_attr_encoding_type, 
                                         attr_id, 
                                         &sec_attr, 
                                         &argc2, 
                                         argv2);
                if (!DCP_DHD_GOOD_STATUS(status)) {
                    free(entryname);
                    free(name);
                    dce_attr_sch_bind_free(&attr_schema_binding, &status1);
                    DCP_SET_RESULT_CODE(status);
                    break;
                }
            }	
        }
        DCP_DHD_STRING_FREE(entry.name);
        entry.name = (dced_string_t) dce_strdup(name);

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t)entryname,
                            ((local_only) ? dced_c_binding_syntax_local :
                                            dced_c_binding_syntax_default),
                            &hd_handle,
                            &status);
        free(entryname);
        if (!DCP_DHD_GOOD_STATUS(status)) {
            if (!entry_only) {
                dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            }
            DCP_SET_RESULT_CODE(status);
            break;
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (auth_info.info_type == sec_attr_bind_auth_dce) {
                dced_binding_set_auth_info(
                                hd_handle, 
                                auth_info.tagged_union.dce_info.protect_level,
                                auth_info.tagged_union.dce_info.authn_svc, 
                                (rpc_auth_identity_handle_t) login_context,
                                auth_info.tagged_union.dce_info.authz_svc, 
                                &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
        }       

        /*
         * Call dced_entry_add() to create a host data configuration 
         * entry only or call dced_hostdata_create() to create a host 
         * data configuration item.
         */
        if (entry_only) {
            dced_entry_add(hd_handle, &entry, &status);
        }   
        else {
            dced_hostdata_create(hd_handle, &entry, &dced_attr_list, &status);
        }
        DCP_DHD_CHECK_STATUS_OUT(status);

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(hd_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        if (!entry_only) { 
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            if (DCP_DHD_GOOD_STATUS(status) && 
                !DCP_DHD_GOOD_STATUS(status1)) {
                DCP_DHD_SET_STATUS(status, status1);
            }
        }

        /*
         * If an error has been encountered, out of for loop
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    dced_entry_free_ptrs(&entry);
    if (!entry_only) {
        dced_attr_list_free_ptrs(&dced_attr_list);
    }

    /*
     * If the data attribute is specified by attribute option, 
     * the data is malloc-ed by Tcl_Merge() in dhd__parse_attr()
     * and it needs to be released by free.
     */
    if ((attribute) && (data)) {
        free(data);
    }
    DCP_DHD_ARGV_FREE_3(argc1, argv1, argc2, argv2, tmpac, tmpav);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_hd_delete 
 *
 * OVERVIEW:    Delete a hostdata entry and the data associated
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
dcp_hd_delete(ClientData clientData, 
              Tcl_Interp *interp, 
              int        argc, 
              char       **argv)
{
    char                    *name, *entryname;
    boolean32               entry_only = FALSE;
    int                     argc1;
    char                    **argv1;
    dced_binding_handle_t   hd_handle;
    sec_login_handle_t	    login_context;
    boolean32		    authn_user = FALSE;
    boolean32		    local_only = FALSE;
    uuid_t                  id;
    int                     i;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-entry", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_dhd_entry_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_local_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&entry_only;
    arg_table[1].dst = (char *)&local_only;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

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

    for (i = 0; i < argc1; i++) {
        if (dhd__parse_name(interp, argv1[i], dced_c_service_hostdata,
			    dced_c_service_hostdata_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
            DCP_DHD_SET_STATUS(status, ~error_status_ok);
            break;
        }

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t)entryname,
                            ((local_only) ? dced_c_binding_syntax_local :
                                            dced_c_binding_syntax_default),
                            &hd_handle,
                            &status);
        free(entryname);
        if (!DCP_DHD_GOOD_STATUS(status)) {
            free(name);
            DCP_SET_RESULT_CODE(status);
            break;
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed.
         */
        if (authn_user) {
            dced_binding_set_auth_info(hd_handle,
                                       dhd_c_prot_level,
                                       dhd_c_authn_svc, 
                                       (rpc_auth_identity_handle_t)login_context,
                                       dhd_c_authz_svc, 
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * need to call delete routine.
         */
        dced_inq_id(hd_handle, (dced_string_t) name, &id, &status);
        DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_entry_remove() to delete a host data configuration 
         * entry only or call dced_hostdata_delete() to delete a host 
         * data configuration items 
         */
        if (entry_only) {
            dced_entry_remove(hd_handle, &id, &status);
        }   
        else {
            dced_hostdata_delete(hd_handle, &id, &status);
        }

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(hd_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
	}
        free(name);

        /*
         * If a error has been met up to now, leave the for loop
         * and abort the rest of work.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_hd_modify 
 *
 * OVERVIEW:    modify the specified hostdata object (replace the 
 *              existing data with new one) on the specified host.
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
dcp_hd_modify(ClientData clientData, 
              Tcl_Interp *interp, 
              int        argc, 
              char       **argv)
{
    char				*attribute = (char *) NULL;
    boolean32		    		binary = FALSE;
    boolean32				local_only = FALSE;
    char                    		*data = (char *) NULL;
    char                    		*name, *entryname;
    dced_binding_handle_t   		hd_handle;
    extern dce_attr_sch_handle_t	attr_schema_binding;
    dce_attr_component_name_t		schema_name;
    dce_attr_bind_auth_info_t		auth_info;
    dce_attr_schema_entry_t		schema_entry;
    sec_login_handle_t			login_context;
    int                     		argc1, argc2, tmpac;
    char                    		**argv1, **argv2, **tmpav;
    int                     		i;
    dced_entry_t            		entry;
    uuid_t		    		attr_id;
    sec_attr_encoding_t	    		sec_attr_encoding_type;
    sec_attr_t              		sec_attr;
    dced_attr_list_t        		dced_attr_list;
    uuid_t                  		id;
    error_status_t          		status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-binary", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_binary_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_hd_change_help},
        {"-data", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_hd_data_help},
        {"-local", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
            dcp_t_hd_local_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&binary;
    arg_table[1].dst = (char *)&attribute;
    arg_table[2].dst = (char *)&data;
    arg_table[3].dst = (char *)&local_only;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV_3(argc1, argv1, argc2, argv2, tmpac, tmpav);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if ((!attribute) && (!data)) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "change or data");
        return TCL_ERROR;
    }

    if ((attribute) && (data)) {
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

        /*
         * The entry attributes can not be modified, if it's been
         * requested, return it as error.
         * The status code returned from uuid_is_nil is not needed to
         * be checked because it could only be uuid_s_ok.
         */
        if ((!uuid_is_nil(&entry.id, &status)) ||
            (entry.description) || (entry.storage_tag)) {
            if (data) {
                free(data);
            }
            DCP_DHD_ARGV_FREE(argc1, argv1);
            dced_entry_free_ptrs(&entry);
            DCP_SET_MISC_ERROR(dcp_s_dhd_mod_invalid_attr);
        }
        if (!data) {
            DCP_DHD_ARGV_FREE(argc1, argv1);
            DCP_SET_MISC_ERROR(dcp_s_dhd_data_attr_missing);
        }
    }

    attr_id = (binary) ? dced_g_uuid_binfileattr : dced_g_uuid_fileattr;

    /*
     * The reason we need to use tmpac/tmpav and copy it to
     * argc2/argv2 is the get_attr_values() in rgy_utils.c
     * always treats the attr values starting from argv[1].
     */
     if (binary) {
         argc2 = 2;
         DCP_DHD_ARGV_ALLOC_ST(argc2, argv2, status);
         if (!DCP_DHD_GOOD_STATUS(status)) {
             if (attribute) {
                 free(data);
             }
             dced_entry_free_ptrs(&entry);
             DCP_DHD_ARGV_FREE(argc1, argv1);
             return TCL_ERROR;
         }
         argv2[0] = "bindata";
         argv2[1] = data;
     }
     else {
         if (Tcl_SplitList(interp, data, &tmpac, &tmpav) != TCL_OK) {
             if (attribute) {
                 free(data);
             }
             dced_entry_free_ptrs(&entry);
             DCP_DHD_ARGV_FREE(argc1, argv1);
             return TCL_ERROR;
         }
         argc2 = tmpac + 1;
         DCP_DHD_ARGV_ALLOC_ST(argc2, argv2, status);
         if (!DCP_DHD_GOOD_STATUS(status)) {
             if (attribute) {
                 free(data);
             }
             dced_entry_free_ptrs(&entry);
             DCP_DHD_ARGV_FREE_2(argc1, argv1, tmpac, tmpav);
             return TCL_ERROR;
         }
         argv2[0] = "data";
         for (i=1; i<argc2; i++) {
             argv2[i] = tmpav[i-1];
         }
     } 
     dced_attr_list.count = 1; 
     dced_attr_list.list = &sec_attr; 

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    auth_info = s_null_auth_info;
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        auth_info.info_type = sec_attr_bind_auth_dce;
        auth_info.tagged_union.dce_info.protect_level = dhd_c_prot_level;
        auth_info.tagged_union.dce_info.authn_svc = dhd_c_authn_svc;
        auth_info.tagged_union.dce_info.authz_svc = dhd_c_authz_svc;
    }

    for (i = 0; i < argc1; i++) {

        if (dhd__parse_name(interp, argv1[i], dced_c_service_hostdata,
                            dced_c_service_hostdata_len, NULL,
                            &entryname, &name, NULL) != TCL_OK) {
            DCP_DHD_SET_STATUS(status,  ~error_status_ok);
            break;
        }

        /*
         * Create the dce_attr_sch binding
         */
        if (local_only) {
            dce_attr_sch_bind_local(0,
                                    &dced_rs_attr_schema_v0_0_epv,
                                    (dce_attr_component_name_t)dhd_c_attr_schema_token,
                                    &attr_schema_binding,
                                    &status);
        }
        else {
            if (dhd__schema_name_from_dced_name(interp, entryname, &schema_name) != TCL_OK) {
                free(entryname);
                free(name);
                DCP_DHD_SET_STATUS(status, ~error_status_ok);
                break;
            }
            dce_attr_sch_bind(schema_name, &auth_info, &attr_schema_binding, &status);
            free((dce_attr_component_name_t)schema_name);
        }
        if (!DCP_DHD_GOOD_STATUS(status)) {
            free(entryname);
            free(name);
            DCP_SET_RESULT_CODE(status);
            break;
        }

        /*
         * Lookup the entry by attr_id
         */
        dce_attr_sch_lookup_by_id(attr_schema_binding, &attr_id,
                                  &schema_entry, &status);
        if (!DCP_DHD_GOOD_STATUS(status)) {
            free(entryname);
            free(name);
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            DCP_SET_RESULT_CODE(status);
            break;
        }

        /*
         * It should be sec_attr_enc_bytes
         * or sec_attr_enc_printstring_array.
         */
        sec_attr_encoding_type = schema_entry.attr_encoding;
        dhd__schema_ent_free_ptrs(&schema_entry);

        if ((i) && (sec_attr_encoding_type != sec_attr.attr_value.attr_encoding)) {
            sec_attr_util_inst_free_ptrs(&sec_attr);
        }

        if ((!i) || (sec_attr_encoding_type != sec_attr.attr_value.attr_encoding)) {
            sec_attr = s_null_sec_attr;
            status = get_attr_values(interp, sec_attr_encoding_type,
                                     attr_id, &sec_attr, &argc2, argv2);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(entryname);
                free(name);
                dce_attr_sch_bind_free(&attr_schema_binding, &status1);
                DCP_SET_RESULT_CODE(status);
                break;
            }
        }

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((unsigned char *)entryname,
                            ((local_only) ? dced_c_binding_syntax_local :
                                            dced_c_binding_syntax_default),
                            &hd_handle,
                            &status);
        free(entryname);
        if (!DCP_DHD_GOOD_STATUS(status)) {
            free(name);
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            DCP_SET_RESULT_CODE(status);
            break;
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (auth_info.info_type == sec_attr_bind_auth_dce) {
            dced_binding_set_auth_info(hd_handle,
                                       auth_info.tagged_union.dce_info.protect_level,
                                       auth_info.tagged_union.dce_info.authn_svc, 
                                       (rpc_auth_identity_handle_t) login_context,
                                       auth_info.tagged_union.dce_info.authz_svc, 
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }       

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * needed to in the writing routine of dced.
         */
        dced_inq_id(hd_handle, (unsigned char *)name, &id, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_hostdata_write() to write the new host data out. 
         * Note: the new data is going to replace the existing data.
         */
        dced_hostdata_write(hd_handle, &id, &dced_attr_list, &status);

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(hd_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        dce_attr_sch_bind_free(&attr_schema_binding, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }
	free(name);

        /*
         * Get out the for loop if a error has been met up to now
         * and cancel the rest of work.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    dced_attr_list_free_ptrs(&dced_attr_list);

    /*
     * If the data attribute is specified by attribute option, 
     * the data is malloc-ed by Tcl_Merge() in dhd__parse_attr()
     * and it need to be released by free().
     */  
    if ((attribute) && (data)) {
        free(data);
    }
    DCP_DHD_CLEAR_ARGV_3(argc1, argv1, argc2, argv2, tmpac, tmpav);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_hd_show 
 *
 * OVERVIEW:    Show the contents of a hostdata object or just hostdata
 *              entry contents if -entry switch is present.
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
dcp_hd_show(ClientData clientData, 
            Tcl_Interp *interp, 
            int        argc, 
            char       **argv)
{
    char                          *name;
    char                          *entryname;
    boolean32                     binary = FALSE;
    boolean32                     entry_only = FALSE;
    boolean32			  local_only = FALSE;
    boolean32                     unauth_only = FALSE;
    char                          *ifname = NULL;
    extern dce_attr_sch_handle_t  attr_schema_binding;
    dce_attr_component_name_t     schema_name;
    dce_attr_bind_auth_info_t     auth_info;
    dced_binding_handle_t         hd_handle;
    sec_login_handle_t            login_context;
    dced_entry_t                  *entry_p;
    sec_attr_t                    *dced_attr_p;
    dced_cursor_t                 hd_cursor;
    int                           i;
    uuid_t                        id;
    int                           argc1, argc2;
    char                          **argv1, **argv2;
    char                          *objname = NULL;
    handle_t                      rpc_binding = NULL;
    char                          *binding_string = NULL;
    error_status_t                status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-binary", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_binary_help},
        {"-entry", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_dhd_entry_help},
        {"-ifname", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_hd_ifname_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_local_help},
        {"-unauth", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_hd_unauth_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&binary;
    arg_table[1].dst = (char *)&entry_only;
    arg_table[2].dst = (char *)&ifname;
    arg_table[3].dst = (char *)&local_only;
    arg_table[4].dst = (char *)&unauth_only;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

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
     * Check the options for correctness.
     */
    if ((binary == TRUE) && ((entry_only == TRUE) || (ifname != NULL))) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Split the argument name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    if ((ifname != NULL) && (argc1 > 1)) {
        /*
         * The user is specifying the ifname option, as well as multiple
         * string bindings tokens.  Assume that these multiple tokens
         * represent one string binding in tcl syntax.  If not, the next
         * function call will return an error, as expected.  If so, the
         * results of the next function call should be used during the
         * call to get the rpc binding handle.
         */
        binding_string = dcp_binding_tcl_to_string(argc1, argv1, &status);
        if (binding_string == NULL) {
            free((char *)argv1);
            return TCL_ERROR;
        }
        argc1 = 1;
    }

    if (ifname != NULL) {
        if (Tcl_SplitList(interp, ifname, &argc2, &argv2) != TCL_OK) {
            free((char *)argv1);
            return TCL_ERROR;
        }
        if (argc2 > 1) {
            free((char *)argv1);
            free((char *)argv2);
            DCP_SET_MISC_ERROR(dcp_s_dhd_too_many_ifnames);
        }
        free((char *)argv2);
    }

    auth_info = s_null_auth_info;
    if (!unauth_only) {
        /*
         * Check to see if this user is authenticated or unauthenticated 
         */
        sec_login_get_current_context(&login_context, &status);
        if (DCP_DHD_GOOD_STATUS(status)) {
            auth_info.info_type = sec_attr_bind_auth_dce;
            auth_info.tagged_union.dce_info.protect_level = dhd_c_prot_level;
            auth_info.tagged_union.dce_info.authn_svc = dhd_c_authn_svc;
            auth_info.tagged_union.dce_info.authz_svc = dhd_c_authz_svc;
        }
    }

    for (i = 0; i < argc1; i++) {
        /*
         * Parse the object name.
         */
        if (dhd__parse_name(interp,
                            argv1[i],
                            dced_c_service_hostdata,
                            dced_c_service_hostdata_len,
                            NULL,
                            &entryname,
                            &name,
                            NULL) != TCL_OK) {
            DCP_DHD_SET_STATUS(status, ~error_status_ok);
            break;
        }

        /*
         * If we are processing an interface name, then the name
         * argument should be a string binding.  Use the name to
         * get an rpc binding.  If we cannot get a legal rpc binding,
         * then return an error.
         */
        if (ifname != NULL) {
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

            /*
             * Turn the rpc binding into a dced binding.
             */
            dced_binding_from_rpc_binding(dced_e_service_type_hostdata,
                                          rpc_binding,
                                          &hd_handle,
                                          &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(name);
                free(entryname);
                free(binding_string);
                rpc_binding_free(&rpc_binding, &status1);
                DCP_SET_RESULT_CODE(status);
                break;
            }
        }
        else {
            /*
             * Get the dced binding directly, without using the rpc call.
             */
            dced_binding_create((dced_string_t)entryname,
                                ((local_only) ? dced_c_binding_syntax_local :
                                                dced_c_binding_syntax_default),
                                &hd_handle,
                                &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(entryname);
                free(name);
                DCP_SET_RESULT_CODE(status);
                break;
            }
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if authenticated.
         */
        if (auth_info.info_type == sec_attr_bind_auth_dce) {
            dced_binding_set_auth_info(hd_handle,
                                       auth_info.tagged_union.dce_info.protect_level,
                                       auth_info.tagged_union.dce_info.authn_svc, 
                                       (rpc_auth_identity_handle_t)login_context,
                                       auth_info.tagged_union.dce_info.authz_svc, 
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Create the binding for the dced attribute schema if the
         * ifname option is not being used.  Make sure the schema
         * attribute binding is NULL if the ifname option is used.
         */
        if (ifname == NULL) {
            if (local_only) {
                dce_attr_sch_bind_local(0,
                                        &dced_rs_attr_schema_v0_0_epv,
                                        (dce_attr_component_name_t)dhd_c_attr_schema_token,
                                        &attr_schema_binding,
                                        &status);
            }
            else {
                if (dhd__schema_name_from_dced_name(interp, entryname, &schema_name) != TCL_OK) {
                    free(entryname);
                    free(name);
                    dced_binding_free(hd_handle, &status1);
                    DCP_DHD_SET_STATUS(status, ~error_status_ok);
                    break;
                }
                dce_attr_sch_bind(schema_name, &auth_info, &attr_schema_binding, &status);
                free((dce_attr_component_name_t)schema_name);
            }
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(entryname);
                free(name);
                dced_binding_free(hd_handle, &status1);
                DCP_SET_RESULT_CODE(status);
                break;
            }
        }
        else {
            attr_schema_binding = NULL;
        }

        /*
         * Insert a newline between output of objects
         */
        if (i) {
            Tcl_AppendResult(interp, "\n", (char *) NULL);
        }

        /*
         * Call dced_initialize_cursor() to get the cursor of
         * the object list
         */
        dced_initialize_cursor(hd_handle, &hd_cursor, &status);
        DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_entry_get_next() to traverse the object list
         */
        for ( ; ; ) {
            dced_entry_get_next(hd_cursor, &entry_p, &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                break;
            }
            if (ifname == NULL)
                objname = dce_strdup(name);
            else
                objname = dce_strdup(ifname);
            if (strcasecmp(objname, (char *)entry_p->name) == 0) {
                /*
                 * Save the uuid which is needed to read the hostdata object,
                 * then display the object entry information.
                 */
                id = entry_p->id;
                dhd__show_entry(interp, entry_p, NULL, dhd_e_show_all);
                break;
            }
        }

        /*
         * Call dced_release_cursor() to release the cursor
         */
        dced_release_cursor(&hd_cursor, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }
        DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_hostdata_read() to read the hostdata object. 
         */
        if (!entry_only) {
            dced_hostdata_read(hd_handle,
                               &id, 
                               (binary) ? &dced_g_uuid_binfileattr : &dced_g_uuid_fileattr,
                               &dced_attr_p,
                               &status);
            DCP_DHD_CHECK_STATUS_OUT(status);

            /*
             * Show the hostdata object if read success
             */
            dhd__show_object(interp, 
                             (void *)dced_attr_p, 
                             dced_e_service_type_hostdata, 
                             dhd_e_show_all);

            /*
             * Release the hostdata object after showing
             */
            dced_objects_release(hd_handle, 1, (void *)dced_attr_p, &status);
        }

out:
        /*
         * Call dced_binding_free() to free the dced binding handle.
         */
        if (binding_string != NULL)
            free(binding_string);
        dced_binding_free(hd_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        if (attr_schema_binding != NULL) {
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
                DCP_DHD_SET_STATUS(status, status1);
            }
        }

        /*
         * If the error "no more entries" is returned, the requested
         * object doesn't exist.
         */
        if (DCP_DHD_STATUS_EQUAL(status, dced_s_no_more_entries)) {
            DCP_DHD_SET_RESULT(dcp_s_sr_obj_not_found, objname);
            free(name);
            free(entryname);
            if (objname != NULL) {
                free(objname);
            }
            break;
        }
        free(name);
        free(entryname);
        if (objname != NULL) {
            free(objname);
        }

        /*
         * If an error has been seen up to now, abort the
         * operation to show rest of names in the list.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free the memory and return right status 
     */
    free((char *)argv1);
    DCP_DHD_RETURN(status);
}
