/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: esl_sync.c,v $
 * Revision 1.1.13.1  1996/07/08  18:31:14  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:19  marty  1.1.11.3]
 *
 * Revision 1.1.11.3  1996/02/18  00:06:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:44  marty]
 * 
 * Revision 1.1.11.2  1995/12/08  17:41:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/14  18:42 UTC  jrr
 * 	Change dce_printf() to dce_svc_printf().
 * 	[1995/07/14  18:36 UTC  jrr  /main/jrr_mothra_pure/1]
 * 
 * Revision 1.1.7.1  1994/10/14  17:05:25  blazin
 * 	Use correct number of args in call to aud_esl_clean_table.
 * 	[1994/10/14  17:04:41  blazin]
 * 
 * Revision 1.1.5.2  94/10/14  11:05:11  weisz
 * 	CR 12625: aud_esl_clean_table called with only one parameter.
 * 
 * Revision 1.1.2.7  1994/06/23  19:09:09  weisz
 * 	CR 10995: use rmdir and remove instead of unlink,
 * 	          in unregister_update_interface
 * 	[1994/06/23  19:07:31  weisz]
 * 
 * Revision 1.1.2.6  1994/06/06  17:43:52  weisz
 * 	CR 10829: fix some memory leaks.
 * 	[1994/06/06  15:37:43  weisz]
 * 
 * Revision 1.1.2.5  1994/04/11  19:23:33  weisz
 * 	CR 10322: add unregister update interface
 * 	[1994/04/11  19:21:26  weisz]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:39  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	Changed to use correct RPC bindings from RPC runtime.
 * 	[1994/03/11  22:24:07  luan]
 * 
 * Revision 1.1.2.3  1994/02/22  16:47:31  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:41:26  luan]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:40  luan
 * 	CR 9745 9746: File path names consolidated.
 * 	Hard-coded path names replaced with macros
 * 	which generate names dynamically.
 * 	[1994/01/25  04:13:27  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:27  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:39  zeliff]
 * 
 * $EndLog$
 */

/*
** esl_sync.c
** Synchronizes in-core filter tables and event table when filter files
** and event class configuration files are upated.
*/

#include <sys/stat.h>
#include <fcntl.h>

#include <dce/dce.h>
#include <dce/audit_update.h>
#include <dce/audit.h>
#include <dce/audit_control.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>
#include <pthread.h>
#include <eslp.h>
#include <pathsp.h>
#include <error_statusp.h>

extern time_t ESL_File_Timestamp[];
extern time_t ESL_UPDATE_File_Timestamp[];

extern time_t aud_esl_input_file(aud_esl_type_t, unsigned32, 
		                 unsigned32 *, void ***, aud_esl_guides_p_t *,
		                 aud_esl_guides_p_t *);
extern time_t 
aud_esl_input_update_file(aud_esl_type_t, unsigned32, 
		          unsigned32 *, void ***);

rpc_binding_vector_p_t bindings;
extern unsigned32                aud_g_cli_num_of_ecs;
extern unsigned32               *aud_g_cli_evt_classes;

extern void                    **aud_g_cli_esl[];
extern aud_esl_guides_p_t        aud_g_cli_world_guides;
extern aud_esl_guides_p_t        aud_g_cli_world_overridable_guides;
/*
extern aud_esl_entry_p_t         cli_esl_princ[];
extern aud_esl_foreign_entry_p_t cli_esl_foreign_princ[];
extern aud_esl_entry_p_t         cli_esl_group[];
extern aud_esl_foreign_entry_p_t cli_esl_foreign_group[];
extern aud_esl_entry_p_t         cli_esl_cell[];
extern aud_esl_entry_p_t         cli_esl_cell_overridable[];
 */

extern pthread_mutex_t esl_mutex;

void update_esl_check
(
  handle_t handle, 
  aud_esl_type_t esl_type,
  error_status_t *stp
)
{
    struct stat stat_buf;
    int         fd;
    time_t      timestamp;

    *stp = aud_s_ok;

    pthread_mutex_lock(&esl_mutex);
    fd = open(ESL_FILE_NAME(esl_type), O_RDONLY, 0);
    if (fd == -1) {
	dce_svc_printf(AUD_S_OPEN_ESL_FILE_MSG, ESL_FILE_NAME(esl_type));
	ESL_File_Timestamp[esl_type] = 0;
	/* It is OK if esl file does not exist. Don't return here. */
	/* Also don't forget that we have locked "esl_mutex". */
    } else {
        if (fstat(fd, &stat_buf) < 0) {
	    dce_svc_printf(AUD_S_STAT_ESL_FILE_MSG, ESL_FILE_NAME(esl_type));
	    *stp = aud_s_stat_esl_file;
    	    pthread_mutex_unlock(&esl_mutex);
	    return;
	}
	timestamp = stat_buf.st_mtime;
	close(fd);
	if (timestamp > ESL_File_Timestamp[esl_type]) {
	    if (!(WORLD_TYPE(esl_type))) {
		aud_esl_clean_table(aud_g_cli_esl, esl_type);
	    }
	    timestamp = aud_esl_input_file(esl_type, 
		aud_g_cli_num_of_ecs, aud_g_cli_evt_classes, 
		aud_g_cli_esl, &aud_g_cli_world_guides,
		&aud_g_cli_world_overridable_guides);
	    ESL_File_Timestamp[esl_type] = timestamp;
	}
    } 

    /* There is no update file for world and world_overriable filters */
    if (esl_type >= aud_e_esl_world) {
	pthread_mutex_unlock(&esl_mutex);
	return;
    }

    fd = open(ESL_UPDATE_FILE_NAME(esl_type), O_RDONLY, 0);
    if (fd == -1) {
	dce_svc_printf(AUD_S_OPEN_ESL_UPDATE_FILE_MSG, ESL_UPDATE_FILE_NAME(esl_type));
	ESL_UPDATE_File_Timestamp[esl_type] = 0;
	/* It is OK if esl-update file does not exist.  Don't return here. */
	/* Also don't forget that we have locked "esl_mutex". */
    } else {
        if (fstat(fd, &stat_buf) < 0) {
	    dce_svc_printf(AUD_S_STAT_ESL_UPDATE_FILE_MSG, 
		ESL_UPDATE_FILE_NAME(esl_type));
	    *stp = aud_s_stat_esl_update_file;
    	    pthread_mutex_unlock(&esl_mutex);
	    return;
	}
	timestamp = stat_buf.st_mtime;
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "update_esl_check: esl update file modify time = %d\n",
	     timestamp));
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "update_esl_check: my timestamp = %d\n", 
	    ESL_UPDATE_File_Timestamp[esl_type]));
#endif
	close(fd);
	if (timestamp > ESL_UPDATE_File_Timestamp[esl_type]
			&&
	    timestamp >= ESL_File_Timestamp[esl_type]) {
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1, 
	    "update_esl_check: Input esl-update file type %ld\n", esl_type));
#endif
	    timestamp = aud_esl_input_update_file(esl_type,
			    aud_g_cli_num_of_ecs, aud_g_cli_evt_classes,
			    aud_g_cli_esl);
	    ESL_File_Timestamp[esl_type] = timestamp;
	}
    } 
    pthread_mutex_unlock(&esl_mutex);
    return;
}
	    	
void update_eccf_check
(
  handle_t handle, 
  error_status_t *stp
)
{
    pthread_mutex_lock(&esl_mutex);
    input_evt_table();
    pthread_mutex_unlock(&esl_mutex);
    return;
}

void compose_client_binding_file_name
  (
    char *fname,
    char *dname
  )        
{
    char      pid_string[20];
    int       string_len;


    strcpy(fname, CLIENT_BINDING_DIR);
    sprintf(pid_string, "%i", getpid());
    strcat(fname, pid_string);
    strcat(fname, "/");
           
    strcpy(dname, fname);

    strcat(fname, "update_binding_file");
    return;

}


/*
 * Register update interface with runtime 
 */

void register_update_interface
  (
    error_status_t  *status
  )
{
    int               i;
    int               fd;
    unsigned_char_p_t string_binding;
    char              dname[128];
    char              fname[128];
    int               string_len;

    CLEAR_STATUS(status);

    rpc_server_inq_bindings(&bindings, status);
    if (BAD_STATUS(status)) {
        return;
    }

    compose_client_binding_file_name(fname, dname);

    if (mkdir(dname, 0700) == -1) {
	*status = aud_s_cannot_mkdir;
	dce_svc_printf(AUD_S_CANNOT_MKDIR_MSG, dname);
	return;
    }
    
    fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0600);

    if (fd > -1) {
        for (i = 0; i < bindings->count; i++) {
            rpc_binding_to_string_binding(bindings->binding_h[i],
                                          &string_binding, status);
            if (GOOD_STATUS(status) && (char *) string_binding != 0) {
                write(fd, (char *) string_binding, 
			strlen((char *) string_binding));
                write(fd, "\n", 1);
                rpc_string_free(&string_binding, status);
            } 
        }
        close(fd);
    } else {
	dce_svc_printf(AUD_S_CANT_OPEN_CLIENT_BINDING_FILE_MSG, fname);
        *status = aud_s_cant_open_client_binding_file;
    }

    rpc_server_register_if(update_v1_0_s_ifspec, 
                          (uuid_t *) NULL, (rpc_mgr_epv_t) NULL, status);
    return;

}


/*
 * Unregister update interface with runtime 
 */

void unregister_update_interface
  (
    error_status_t  *status
  )
{

    int     st;
    extern  int errno;
    char    dname[128];
    char    fname[128];

    CLEAR_STATUS(status);

    compose_client_binding_file_name(fname, dname);

    st = remove(fname);

    if (( st != 0 ) && ( errno != ENOENT )) {
      dce_svc_printf(AUD_S_REMOVE_FILE_CLIENT_UPDATE_BINDING_FAILED_MSG, fname, st, errno);
     *status = aud_s_remove_file_client_update_binding_failed;
    }
    else
      st = rmdir(dname);

    if (( st != 0 ) && ( errno != ENOENT )) {
        dce_svc_printf(AUD_S_REMOVE_DIR_CLIENT_UPDATE_BINDING_FAILED_MSG, dname, st, errno);
       *status = aud_s_remove_dir_client_update_binding_failed;
    }

    rpc_server_unregister_if(update_v1_0_s_ifspec, (uuid_t *) NULL, status);
    return;

}
