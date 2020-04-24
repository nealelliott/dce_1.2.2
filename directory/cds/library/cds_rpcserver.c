/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_rpcserver.c,v $
 * Revision 1.1.4.2  1996/02/18  19:32:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:20:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:35  root]
 * 
 * Revision 1.1.2.10  1994/07/27  19:58:30  jd
 * 	Cast last argument of dce_cf_get_cell_name and dce_cf_get_host_name.
 * 	[1994/07/27  17:30:40  jd]
 * 
 * Revision 1.1.2.9  1994/07/27  13:39:05  jd
 * 	In cds_export_replace init current_binding_vector,check for null in frees on
 * 	failure, and dont checkfor rpc_s_entry_not_found in lookup_next.
 * 	[1994/07/27  12:53:49  jd]
 * 
 * Revision 1.1.2.8  1994/07/25  15:16:41  proulx
 * 	Provide routine for correcting namespace entry
 * 	[1994/07/25  13:50:06  proulx]
 * 
 * Revision 1.1.2.7  1994/06/30  19:16:21  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:19  mccann]
 * 
 * Revision 1.1.2.6  1994/06/09  18:40:29  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:01  devsrc]
 * 
 * Revision 1.1.2.5  1994/06/08  20:18:00  peckham
 * 	Missed a couple of includes...
 * 	[1994/06/08  13:05:15  peckham]
 * 
 * 	Add cds_server_set_progname() and use after forking.
 * 	[1994/06/06  20:26:15  peckham]
 * 
 * Revision 1.1.2.4  1994/05/12  21:12:12  peckham
 * 	Use _CDS_DEFAULT_STACK_
 * 	[1994/05/12  19:17:33  peckham]
 * 
 * Revision 1.1.2.3  1994/04/29  15:51:37  peckham
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	Eliminate unnecessary conditionals.
 * 	Clean up debugging text. Use common exit point.
 * 	[1994/04/29  13:59:16  peckham]
 * 
 * Revision 1.1.2.1  1994/03/12  22:05:36  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  20:03:32  peckham]
 * 
 * $EndLog$
 */
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
 *
 *  NAME
 *
 *      cds_rpcserver.c
 *
 *  FACILITY:
 *
 *      DCE RPC
 *
 *  ABSTRACT:
 *
 *      Useful RPC server utility routines.
 *
 */

#include <dce/dce.h>
#include <threads_lib.h>
#include <dce/assert.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_error.h>
#include <dce/dcecdsmsg.h>
#include <dce/cds_basic_types.h>
#include <dce/rpc.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <cds_old.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#if defined(SNI_SVR4)
# include <sys/termios.h>
#endif
#include <rpcserver.h>
#include <dce/dce_cf.h>

static int cds_parent_child_pipe[2];

void null_procedure () {}; /* Necessary for defining cds_old_v3... */

/*
**++
**  ROUTINE NAME:       cds_server_fork
**
**  SCOPE:              PRIVATE - declared in rpcserver.h
**
**  DESCRIPTION:
**
**  Fork a process to execute the server and create a pipe between
**  child and parent. Subsequent call to cds_server_detach will 
**  cause parent process to exit.
**  INPUTS:
**      program_name    The name of the server daemon.
**      mode            A bit mask containing the following fields:
**                      rpc_c_server_mode_verify - display informational
**                          messages during initialization
**
**  OUTPUTS:
**      status          The status of the operation. This will be rpc_s_ok
**                      if the operation succeeds. If the operation fails,
**                      it will either be rpc_s_ok, or some DCE error status
**                      providing specific information on the cause of the
**                      failure.
*/

PRIVATE int 
cds_server_fork (char            *program_name,
                 unsigned int    mode,
                 error_status_t  *status)
{
    int			pid;
    int			result = 0;
    char		dummy_buffer;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >cds_server_fork(%s,%d,)",
	program_name, mode));
    
    /*
     * if the caller wants output status, initialize it
     */
    if (status)
    {
        *status = rpc_s_ok;
    }
  
    /*
     * create a pipe on which the parent will listen for the child
     * to tell it that's it's ok to exit - which happens later on
     * in cds_server_detach
     *
     * this is here to try to insure that the parent doesn't exit
     * before the server is actually listening, to avoid messing up
     * something else that might be started in sequence that depends
     * on this server - but it's not totally reliable, since the
     * child has to report *before* it actually calls rpc_server_listen
     */
    if ((pipe (cds_parent_child_pipe)) != 0)
    {
	dce_svc_printf(CDS_S_LIBRARY_PIPE_MSG, (long)errno);

	result = 1;
	goto leave_cds_server_fork;
    }
  
    /*
     * fork a daemon process to execute this server
     */
    pid = fork();
  
    /*
     * return an error if the fork failed
     */
    if (pid == -1)
    {
	dce_svc_printf(CDS_S_LIBRARY_FORK_MSG, (long)errno);
	
	result = 1;
	goto leave_cds_server_fork;
    }
  
    if (pid > 0)
    {
	int e_status;
	
	/*
	 * the parent waits for the child to report, then exits
	 */
	close (cds_parent_child_pipe[1]);
	
	read (cds_parent_child_pipe[0], &dummy_buffer, sizeof (dummy_buffer));
	
	if (waitpid (pid, &e_status, WNOHANG) != 0)
	{
	    int tmp_sts = WEXITSTATUS (e_status);
	    
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_library,
                svc_c_debug2,
	        " <cds_server_fork(%s,%d,): parent exiting - exit(%d)",
		program_name, mode, tmp_sts));

	    exit (tmp_sts);
	}
	else
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_library,
                svc_c_debug2,
	        " <cds_server_fork(%s,%d,): parent exiting - exit(0)",
		program_name, mode));
	
	    exit(0);
	}
    }
  
    /*
     * We now have a new name
     */
    cds_server_set_progname(program_name);

    /*
     * the child continues
     */
    close (cds_parent_child_pipe[0]);
  

leave_cds_server_fork:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" <cds_server_fork(%s,%d,) return(%d)",
	program_name, mode, result));

  return (result);
}



/*
**++
**  ROUTINE NAME:       cds_server_init
**
**  SCOPE:              PRIVATE - declared in rpcserver.h
**
**  DESCRIPTION:
**
**  (optionally) Fork a process to execute the server, and initialize the
**  RPC runtime in preparation for the server to listen for RPC calls.
**
**  INPUTS:
**
**      program_name    The name of the server daemon.
**      server_name     The name of the directory service entry into which
**                      server binding information is to be exported.
**                      If NULL, no binding information will be exported.
**      if_count        A count of the number of RPC server interfaces.
**      if_descriptors  An array of size if_count of interface descriptors
**                      to be registered with the RPC runtime and the
**                      endpoint mapper, and exported to the directory
**                      service. For each if descriptor:
**
**                      if_handle = interface handle generated by idl
**                      if_epv = interface entry point vector 
**                      object_ids = vector of object uuid's to be registered
**                          with ep mapper and exported to directory service
**                          along with this interface; NULL if no object uuid's
**                      annotation = annotation field to be registered with
**                          ep mapper along with this interface;
**                          NULL to use default based on program name
**
**                      Note that the first interface descriptor in the array
**                      will be used to check if there is already a server
**                      of this type running, so it should contain the
**                      descriptor of some interface that will uniquely
**                      identify the server. Furthermore, the first object
**                      id in the vector for the first interface will be used
**                      in the test, so in the event there is no unique
**                      interface for the server, an object id can be supplied
**                      that will uniquely identify a server of a specific
**                      type.
**
**      mode            A bit mask containing the following fields:
**
**                      rpc_c_server_mode_debug - don't create a child process
**                      rpc_c_server_mode_verify - display informational
**                          messages during initialization
**                      rpc_c_server_mode_no_if_register - don't register
**                          interfaces with the RPC runtime
**                      rpc_c_server_mode_no_ep_register - don't register
**                          interfaces with the ep mapper
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**     binding_vector   A vector of generated server bindings, comprising the
**                      protocol sequences actually supported by the server.
**                      NULL on input if not required.
**      status          The status of the operation. This will be rpc_s_ok
**                      if the operation succeeds. If the operation fails,
**                      it will either be rpc_s_ok, or some DCE error status
**                      providing specific information on the cause of the
**                      failure.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     0 for success, 1 for failure
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE int 
cds_server_init (char                    *program_name,
                 char                    *server_name,
                 unsigned int            if_count,
                 rpc_if_descriptor_p_t   if_descriptors,
                 unsigned int            mode,
                 rpc_binding_vector_p_t  *binding_vector,
                 error_status_t          *status)
{
    dce_error_string_t 	    error_text;
    uuid_vector_p_t         object_ids;
    rpc_binding_vector_p_t  server_binding_vector = NULL;
    char                    *ep_name = NULL;
    char                    *annotation;
    error_status_t          local_status = rpc_s_ok;
    error_status_t          temp_status;
    int                     inq_status;
    int                     i;
    int			    result = 0;



    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
	" >cds_server_init(%s,%s,%d,,%d,,)",
	program_name, server_name, if_count, mode));

    /*
     * Set the stack size for rpc servers
     */
    rpc_mgmt_set_server_stack_size((unsigned32)_CDS_DEFAULT_STACK_,
				   &local_status);

    /*
     * Setup for the use of dynamic endpoints
     */
    rpc_server_use_all_protseqs (rpc_c_protseq_max_reqs_default, &local_status);

    if (local_status != rpc_s_ok)
    {
	dce_svc_printf(CDS_S_RPCUSEALLPROTSEQS_MSG, (long)local_status);

        goto EXIT_HANDLER;
    }

    /*
     * get back what is actually supported
     */
    rpc_server_inq_bindings (&server_binding_vector, &local_status);

    if (local_status != rpc_s_ok)
    {
	dce_svc_printf(CDS_S_RPCINQBINDINGS_MSG, (long)local_status);

        goto EXIT_HANDLER;
    }

    /*
     * for each binding in the vector, try to contact a running server
     */
    for (i = 0; i < server_binding_vector->count; i++)
    {
        /*
         * clear any existing endpoint in the binding
         */
        rpc_binding_reset (server_binding_vector->binding_h[i], &local_status);

        if (local_status != rpc_s_ok)
        {
	    dce_svc_printf(CDS_S_RPCBINDINGRESET_MSG, (long)local_status);

            goto EXIT_HANDLER;
        }

        /*
         * if there are any object id's for the first interface, use them
         * to disambiguate servers of the same interface
         */
        if (if_descriptors[0].object_ids != NULL)
        {
            rpc_binding_set_object (server_binding_vector->binding_h[i],
                if_descriptors[0].object_ids->uuid[0], &local_status);

            if ((local_status != rpc_s_ok) &&
                (local_status != ept_s_not_registered))
            {
		dce_svc_printf(CDS_S_RPCSETOBJECT_MSG,(long)local_status);
                goto EXIT_HANDLER;
            }
        }

        /*
         * resolve this binding to see if an endpoint is registered
         */
        rpc_ep_resolve_binding (server_binding_vector->binding_h[i],
            if_descriptors[0].if_handle, &local_status);

        if ((local_status != rpc_s_ok) &&
            (local_status != ept_s_not_registered))
        {
	    dce_svc_printf(CDS_S_RPCRESOLVEBINDING_MSG, (long)local_status);
            goto EXIT_HANDLER;
        }

        /*
         * if there is a server registered for this binding, try to
         * contact it to see if it's active
         */
        if (local_status == rpc_s_ok)
        {
            /*
             * set the comm timeout to a relatively short wait
             */
            rpc_mgmt_set_com_timeout
                (server_binding_vector->binding_h[i], 1, &local_status);

            if (rpc_mgmt_is_server_listening
                (server_binding_vector->binding_h[i], &temp_status))
            {
		dce_svc_printf(CDS_S_LIBRARY_ALREADYRUNNING_MSG, program_name);

                rpc_binding_vector_free (&server_binding_vector, &temp_status);
                errno = 0;	/* this return (1) is NOT a system failure */

		DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_library,
                    svc_c_debug2,
                    "E cds_server_init(%s,%s,%d,,%d,,): daemon already running",
		    program_name, server_name, if_count, mode));

		result = 1;
		goto leave_cds_server_init;
            }
        }
    }

    /*
     * free the binding vector
     */
    rpc_binding_vector_free (&server_binding_vector, &local_status);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug7,
	"  cds_server_init(%s,%s,%d,,%d,,): Server protocols registered",
	program_name, server_name, if_count, mode));

    /*
     * get back what is actually supported (again)
     */
    rpc_server_inq_bindings (&server_binding_vector, &local_status);

    if (local_status != rpc_s_ok)
    {
	dce_svc_printf(CDS_S_RPCINQBINDINGS_MSG, (long)local_status);
        goto EXIT_HANDLER;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug7,
	"  cds_server_init(%s,%s,%d,,%d,,): Server binding information inquired",
	program_name, server_name, if_count, mode));

    /*
     * construct a name to annotate the ep map entry - make it big
     * enough to accommodate a reasonable pid
     */
    ep_name = (char *) (malloc ((strlen (program_name)) + 12));

    if (ep_name == NULL)
    {
	dce_svc_printf(CDS_S_ALLOC_MSG);

        local_status = rpc_s_no_memory;
        goto EXIT_HANDLER;
    }

    sprintf (ep_name, "%s [%d]", program_name, getpid());

    for (i = 0; i < if_count; i++)
    {
        if (! (mode & rpc_c_server_mode_no_if_register))
        {
            /*
             * register the interface with the runtime
             */
            rpc_server_register_if (if_descriptors[i].if_handle, NULL, 
                                    (rpc_mgr_epv_t)if_descriptors[i].if_epv, 
                                    &local_status);

            if (local_status != rpc_s_ok)
            {
		dce_svc_printf(CDS_S_RPCREGISTERIF_MSG, (long)local_status);
                goto EXIT_HANDLER;
            }

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_library,
                svc_c_debug7,
	        "  cds_server_init(%s,%s,%d,,%d,,): Server interfaces registered",
		program_name, server_name, if_count, mode));
        }

        /*
         * check the optional fields in the if descriptor
         */
        if (if_descriptors[i].object_ids)
        {
            object_ids = if_descriptors[i].object_ids;
        }
        else
        {
            object_ids = NULL;
        }

        if (! (mode & rpc_c_server_mode_no_ep_register))
        {
            if (if_descriptors[i].annotation)
            {
                annotation = if_descriptors[i].annotation;
            }
            else
            {
                annotation = ep_name;
            }

            /*
             * register the dynamic endpoints that were created
             */
            rpc_ep_register (if_descriptors[i].if_handle, server_binding_vector,
                object_ids, (unsigned_char_t *) annotation, &local_status);

            if (local_status != rpc_s_ok)
            {
		dce_svc_printf(CDS_S_RPCREGISTEREP_MSG, (long)local_status);
                goto EXIT_HANDLER;
            }

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_library,
                svc_c_debug7,
	        "  cds_server_init(%s,%s,%d,,%d,,): Server endpoints registered",
		program_name, server_name, if_count, mode));
        }

    }


EXIT_HANDLER:

    /*
     * free the name used for the ep database annotation
     */
    if (ep_name != NULL)
    {
        free (ep_name);
    }

    /*
     * free the binding vector, unless the caller wants it
     */
    if ((local_status == rpc_s_ok) && (binding_vector != NULL))
    {
    	*binding_vector = server_binding_vector;
        server_binding_vector = NULL;
    }
    else
    {
        if (server_binding_vector != NULL)
        {
            rpc_binding_vector_free (&server_binding_vector, &temp_status);
        }
    }

    if (local_status != rpc_s_ok)
    {
        /*
         * pass back the failure status if the caller wants it
         */
        if (status)
        {
            *status = local_status;
        }

        result = 1;
    }

leave_cds_server_init:

    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <cds_server_init(%s,%s,%d,,%d,,%p->%d) return(%d)",
	program_name, server_name, if_count, mode, status, local_status, result));

    return(result);
}

/*
 * export_ns_new
 *
 * Functional Description:
 *
 */
void export_ns_new (char *myname_p,
		    char *servername,
		    rpc_if_handle_t        if_spec,
		    rpc_binding_vector_p_t new_binding_vector,
		    uuid_p_t               object_uuid,
		    unsigned int           *status)
{
   dthread_t thread_p;
   dthread_attr_t attr = {"export_thread", /* t name */
			    0}; /* default stack */
   char *handle;
   unsigned int st;
   char *cellname, *hostname, *entry_name;
   cds_export_ns_info_t *ex;

   /*
    * Put data in temporary storage to pass to new thread.  New
    * thread is responsible for freeing our allocated memory.
    */
   {
      /* 
       * Allocate data area for parameter passed to export thread
       */
      ex = malloc (sizeof(cds_export_ns_info_t));
      if (ex == NULL) {
	 *status = -1;
	 dce_svc_printf(CDS_S_ALLOC_A_MSG, (long)*status); 
	 return;
      }
      /* 
       * Generate: <cellname>/hosts/<hostname>/server-name
       */
      entry_name = malloc (FULLNAMEMAX);
      if (entry_name == NULL) {
	 *status = -1;
	 dce_svc_printf(CDS_S_ALLOC_A_MSG, (long)*status); 
	 return;
      }
      handle = (char *)dns_cfgopen(NULL, NULL);
      dce_cf_get_cell_name(&cellname, (error_status_t *)status);
      if (*status != dce_cf_st_ok) {
	 DCE_SVC_DEBUG((cds__svc_handle,
			cds_svc_general,
			svc_c_debug1,
			"InitializeRPC() %lu = dce_cf_get_cell_name()", 
			*status));
	 return;
      }
      dce_cf_get_host_name(&hostname, (error_status_t *)status);
      if (*status != dce_cf_st_ok) {
	 DCE_SVC_DEBUG((cds__svc_handle,
			cds_svc_general,
			svc_c_debug1,
			"InitializeRPC() %lu = dce_cf_get_host_name()",
			*status));
	 free(cellname);
	 return;
      }
      (void)sprintf(entry_name,"%s/%s/%s", cellname, hostname, servername);
      free(cellname);
      free(hostname); 
      dns_cfgclose(handle);
   }

   ex->program_name = myname_p;
   ex->entry_name = entry_name;
   ex->if_spec = if_spec;
   ex->new_binding_vector = new_binding_vector;
   ex->object_uuid = object_uuid;
   
   /*
    * Start up thread to fix namespace on our behalf
    */
   if ((*status = dthread_create (&thread_p,
				 &attr, 
				 (dthread_start_routine)cds_export_replace,
				 (dthread_address_t)ex) < 0)) {
      dce_svc_printf(CDS_S_EXPORT_THREAD_MSG, (long)*status); 
      free(entry_name);
   }
}

/*
 * cds_export_replace
 *
 * Function Description:
 *
 *  Here we check that the specified binding is correctly in the 
 *  namespace.  In the case where the host network address has
 *  changed since the system was dce_config'd, the namespace info
 *  will be wrong and will be unexported, and the correct partial
 *  binding exported.
 *
 * Returns:
 *  error_status_ok = success
 *  other = error returned from library call
 */
void cds_export_replace ( cds_export_ns_info_p_t ex )
{
   unsigned32       name_syntax = rpc_c_ns_syntax_default;
   unsigned_char_t  *new_string_binding;
   unsigned_char_t  *current_string_binding;
   unsigned32       j, i;
   unsigned32       new_binding_count;
   unsigned32       current_binding_count;
   unsigned32       status, temp_status;
   rpc_binding_vector_t *current_binding_vector = NULL;
   rpc_binding_vector_t export_binding_vector;
   uuid_vector_t    uuids;
   boolean valid_entry;
   boolean no_valid_entries;
   rpc_ns_handle_t lookup_context;
   DCE_SVC_DEBUG((cds__svc_handle,
		  cds_svc_library,
		  svc_c_debug2,
		  " >cds_export_replace(%s,%s,,%p,,%p)",
		  ex->program_name, ex->entry_name,  
		  ex->new_binding_vector, status));

      
   /* 
    * Return rpc_s_nothing_to_export if either the if_spec or the binding_vec
    * are NULL AND the object_uuid_vec is NULL as well.
    */
   if ((ex->if_spec == (rpc_if_handle_t)NULL) || 
       (ex->new_binding_vector == (rpc_binding_vector_p_t)NULL) || 
       (ex->object_uuid == (uuid_t *)NULL)) {
      status = rpc_s_nothing_to_export;
      goto MIN_CLEANUP;
   }
  
   /*
    * Lookup our namespace binding info to see if it needs updating.
    */
   rpc_ns_binding_lookup_begin (name_syntax, ex->entry_name, ex->if_spec, 
				ex->object_uuid, rpc_c_binding_max_count_default, 
				&lookup_context, &status);
   if (status != rpc_s_ok) {
      goto MIN_CLEANUP;
   }
   
   /*
    * Keep looking until we either find the bindings or we find there's none
    */
   do {
      rpc_ns_binding_lookup_next (lookup_context, &current_binding_vector, &status );
   } while ((status != rpc_s_ok && status != rpc_s_no_more_bindings) &&
      dthread_delay(15.0) >= 0);
   

   if (status != rpc_s_ok) {
      current_binding_count = 0;
   }
   else {
      /*
       * Binding lookup succeeded.  We should only have had one binding
       * in the ns to lookup, but we'll manage if there's more.  If we find any that
       * don't match the "new" binding vector, then we unexport them all and
       * export the good stuff.
       */
      current_binding_count = current_binding_vector->count;
   }

   no_valid_entries = TRUE;

   /*
    * Loop through the current bindings in the namespace
    */
   for ( i = 0; i < current_binding_count ; i++ ) {
      
      valid_entry = FALSE;
      
      rpc_binding_reset( current_binding_vector->binding_h[i], &status );
      if (status != rpc_s_ok) {
	 rpc_string_free( &current_string_binding, &status );
	 dce_svc_printf(CDS_S_RPCBINDINGRESET_MSG, (long)status);
	 goto CLEANUP;
      }
      rpc_binding_to_string_binding (current_binding_vector->binding_h[i], 
				     &current_string_binding,
				     &status);
      if (status != rpc_s_ok) {
	 goto CLEANUP;
      }
      
      /*
       * Loop through the new bindings to see if this current namespace entry
       * is valid or invalid
       */
      for ( j = 0; j < (ex->new_binding_vector)->count; j++ ) {
	 rpc_binding_reset (ex->new_binding_vector->binding_h[j], &status);
	 if (status != rpc_s_ok) {
	    dce_svc_printf(CDS_S_RPCBINDINGRESET_MSG, (long)status);
	    goto CLEANUP;
	 }
	 
	 rpc_binding_to_string_binding (ex->new_binding_vector->binding_h[j],
					&new_string_binding,
					&status);
	 if (status != rpc_s_ok) {
	    rpc_string_free( &current_string_binding, &status );
	    goto CLEANUP;
	 }
	    
	 if ((strcmp ((char *)current_string_binding, (char *)new_string_binding)) == 0) {
	    valid_entry = TRUE;
	    no_valid_entries = FALSE;
	    rpc_string_free( &new_string_binding, &status );
	    break;
	 }
	 rpc_string_free( &new_string_binding, &status );
      }
      
      rpc_string_free( &current_string_binding, &status );
      
      /*
       * Nuke the bad binding
       */
      if ( !valid_entry ) {
	 rpc_ns_binding_unexport (name_syntax, ex->entry_name, ex->if_spec, 
				     NULL, &status);
	 if (status != rpc_s_ok) {
	    goto CLEANUP;
	 }
      } /* new bindings */
   } /* current bindings */
   
   /*
    * Export a binding if there are no valid bindings already exported
    */
   if ( no_valid_entries ) {
      /*
       * Export 
       */
      rpc_binding_reset (ex->new_binding_vector->binding_h[0], &status);
      if (status != rpc_s_ok) {
	 goto CLEANUP;
      }
      rpc_binding_to_string_binding (ex->new_binding_vector->binding_h[0],
				     &new_string_binding,
				     &status);
      if (status != rpc_s_ok) {
	 rpc_string_free( &current_string_binding, &status );
	 goto CLEANUP;
      }
	 
      uuids.count = 1;
      uuids.uuid[0] = ex->object_uuid;

      new_binding_count = ex->new_binding_vector->count;
      ex->new_binding_vector->count = 1;

      rpc_ns_binding_export (name_syntax, ex->entry_name, ex->if_spec, 
				ex->new_binding_vector, 
				&uuids, &status);

      ex->new_binding_vector->count = new_binding_count;

      if (status != rpc_s_ok) {
	 goto CLEANUP;
      }
   }
      
   /*
    * Unexport old interface that used to be exported via dce_config before v1.1.
    * We leave this until last in case something fails above.  Better to have 
    * something in the namespace than nothing.
    * If it isn't there, that's fine.
    */
   rpc_ns_binding_unexport (name_syntax, ex->entry_name, cds_old_v3_0_s_ifspec,
			    (uuid_vector_t *)NULL, &status);
   DCE_SVC_DEBUG((cds__svc_handle, cds_svc_general, svc_c_debug2,
		  " status returned from unexporting old interface = %d",
		  status));

   status = error_status_ok;

 CLEANUP:
   /*
    * Destroy lookup context
    */
   rpc_ns_binding_lookup_done (&lookup_context, &status);
   if (status != rpc_s_ok) {
      goto CLEANUP;
   }
  
   if(current_binding_vector) 
   	rpc_binding_vector_free (&current_binding_vector, &temp_status);
      
 MIN_CLEANUP:
   /*
    * Not much to cleanup.  We can't free the current binding
    * vector because it may be in use by another thread. 
    */
    
   if(ex->entry_name)
   	free (ex->entry_name);
   if(ex)
   	free (ex);

   DCE_SVC_DEBUG((cds__svc_handle,
		  cds_svc_library,
		  svc_c_debug2,
		  " <cds_export_replace(%s,%s,,%p,,%d)",
		  ex->program_name, ex->entry_name,  
		  ex->new_binding_vector, status));
   return;
}

/*
**++
**  ROUTINE NAME:       cds_server_detach
**
**  SCOPE:              PRIVATE - declared in rpcserver.h
**
**  DESCRIPTION:
**
**  Detach the server from the user terminal and (optionally) redirect
**  stdout and stderr to a log file.
**
**  INPUTS:
**
**      program_name    The name of the server daemon.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      status          The status of the operation. This will be rpc_s_ok
**                      if the operation succeeds. If the operation fails,
**                      it will either be rpc_s_ok, or some DCE error status
**                      providing specific information on the cause of the
**                      failure.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     0 for success, 1 for failure
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE int 
cds_server_detach (char            *program_name,
                   char            *logfile_name,
                   error_status_t  *status)
{
    time_t              systime;
    int                 tty;
    int			result = 0;
    
    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >cds_server_detach(%s,%s,)",
	program_name, logfile_name));

    /*
     * if the caller wants output status, initialize it
     */
    if (status)
    {
        *status = rpc_s_ok;
    }

    time (&systime);


    if (setsid() < 0) 
    {
	dce_svc_printf(CDS_S_LIBRARY_SETSID_MSG, strerror(errno)); 

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug2,
	    "E cds_server_detach(%s,%s,): unable to set process group id",
	    program_name, logfile_name));

	result = 1;
	goto leave_cds_server_detach;
    }
    /*
     * Enable privileged user mode for the daemon
     */
    if (dce_priv_proc_enable() < 0) 
    {	    
	dce_svc_printf(CDS_S_NOPRIV_EB_MSG);
    }

    /*
     * inform the parent that we're ready to process requests (by closing
     * the pipe, and causing the parent to wake up)
     */
    close (cds_parent_child_pipe[1]);

leave_cds_server_detach:

    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <cds_server_detach(%s,%s,) return(%d)",
	program_name, logfile_name, result));

    return(result);
}

/*
 * Set the servicability name
 */
PRIVATE void
cds_server_set_progname(char *program_name)
{
    char svcname[128];
    error_status_t st;

    (void)sprintf(svcname, "%s(%ld)", program_name, (long)getpid());
    dce_svc_set_progname(svcname, &st);
    dce_assert(cds__svc_handle, (st == svc_s_ok));
}
