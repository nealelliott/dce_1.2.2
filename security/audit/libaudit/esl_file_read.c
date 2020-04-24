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
 * $Log: esl_file_read.c,v $
 * Revision 1.1.8.1  1996/07/08  18:30:52  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:13  marty  1.1.6.2]
 *
 * Revision 1.1.6.2  1996/02/18  00:06:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:41  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:41:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/14  18:42 UTC  jrr
 * 	Change dce_printf() to dce_svc_printf().
 * 	[1995/07/14  18:31 UTC  jrr  /main/jrr_mothra_pure/1]
 * 
 * Revision 1.1.2.8  1994/04/10  19:57:52  weisz
 * 	CR 10320: used memcpy in aud_esl_input_file and aud_esl_input_update_file
 * 	          instead of cast *(unsigned32 *)data_p to avoid alignment problem
 * 	[1994/04/10  19:57:15  weisz]
 * 
 * Revision 1.1.2.7  1994/03/11  22:48:36  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:17:45  luan]
 * 
 * Revision 1.1.2.6  1994/02/22  16:47:28  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:40:14  luan]
 * 
 * Revision 1.1.2.5  1994/02/18  19:47:38  luan
 * 	CR 9845: Insert "/var/audit/" in front of
 * 	"esl-update-xxx" file path strings.
 * 	[1994/02/11  04:24:45  luan]
 * 
 * Revision 1.1.2.4  1994/02/02  23:03:12  ohara
 * 	add stdio need for SEEK_SET
 * 	[1994/02/02  22:43:22  ohara]
 * 
 * Revision 1.1.2.3  1994/01/25  04:59:39  luan
 * 	CR 9745 9746: File path names consolidated.
 * 	Hard-coded path names replaced with macros
 * 	which generate names dynamically.
 * 	[1994/01/25  04:12:54  luan]
 * 
 * Revision 1.1.2.2  1994/01/23  20:42:17  luan
 * 	OT# 9796: Changed to allow multiple-guide audfilter add
 * 	and audfilter remove operations.
 * 	[1994/01/23  20:41:22  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:23  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:36  zeliff]
 * 
 * $EndLog$
 */

/*
** esl_file_read.c
** Reads filters or filter updates from filter files.
*/

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>
#include <eslp.h>
#include <pathsp.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdio.h>
#include <dce/dce_error.h>

char *aud_g_esl_update_fname[aud_e_esl_max] = {
        "/var/audit/esl-update-princ",
        "/var/audit/esl-update-foreign-princ",
        "/var/audit/esl-update-group",
        "/var/audit/esl-update-foreign-group",
        "/var/audit/esl-update-cell",
        "/var/audit/esl-update-cell-overridable",
        "/var/audit/esl-update-world",
        "/var/audit/esl-update-world-overridable"
};

char *aud_g_esl_fname[aud_e_esl_max] = {
        "/var/audit/esl-princ",
        "/var/audit/esl-foreign-princ",
        "/var/audit/esl-group",
        "/var/audit/esl-foreign-group",
        "/var/audit/esl-cell",
        "/var/audit/esl-cell-overridable",
        "/var/audit/esl-world",
        "/var/audit/esl-world-overridable"
};

/* 
 * Each element in the following array is initialized when it
 * is referenced for the first time.
 */

char *aud_g_esl_update_full_fname[aud_e_esl_max] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};


char *aud_g_esl_full_fname[aud_e_esl_max] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

/* 
 * Allocate dir/file path variables.
 */

char * event_class_dir      = NULL;
char * client_binding_dir   = NULL;
char * daemon_identity_file = NULL;
char * daemon_binding_file  = NULL;

extern int      errno;

typedef struct {
    unsigned32      audit_action;
    unsigned32      audit_condition;
    short           num_evt_classes;
} aud_esl_file_guide_hdr_t;

#define CURRENT_FILE_VERSION 1

static void
                aud__esl_add_entry
                (
		                 aud_esl_type_t esl_type,
		                 uuid_t subject_uuid,
		                 uuid_t cell_uuid,
		                 aud_esl_guides_p_t guides_ptr,
				 void *** tables
) {
    unsigned16         probe;
    unsigned32	       st, string_st;
    unsigned_char_t   *uuid_string;
    dce_error_string_t error_string;
    int                print_status;

    probe = uuid_hash(&subject_uuid, &st) % HASH_TABLE_SIZE;
    if (st != uuid_s_ok) {
        dce_error_inq_text(st, error_string, &print_status);
        uuid_to_string(&subject_uuid, &uuid_string, & string_st );
        if ( string_st != uuid_s_ok ) {
           /* uuid bad from uuid_to string,
              so print unknown instead of uuid garbage */
           strcpy((char *)uuid_string, "?");
        }
        dce_svc_printf(AUD_S_UUID_HASH_FAILED_MSG, (char *)uuid_string, error_string );
        rpc_string_free(&uuid_string, & string_st );
	return;
    };

    if (FOREIGN_TYPE(esl_type)) {
	aud_esl_foreign_entry_p_t entry_ptr1, entry_ptr2, new_entry_ptr;
	aud_esl_foreign_entry_p_t *table =
	(aud_esl_foreign_entry_p_t *) tables[esl_type];

	new_entry_ptr = (aud_esl_foreign_entry_p_t)
	    malloc(sizeof(aud_esl_foreign_entry_t));
	new_entry_ptr->subject_uuid = subject_uuid;
	new_entry_ptr->cell_uuid = cell_uuid;
	new_entry_ptr->guides = guides_ptr;

	if (table[probe] == NULL) {
	    table[probe] = new_entry_ptr;
	    new_entry_ptr->next = NULL;
	    return;
	}
	entry_ptr1 = entry_ptr2 = table[probe];
	while (entry_ptr1 != NULL) {
	    entry_ptr2 = entry_ptr1;
	    entry_ptr1 = entry_ptr1->next;
	}
	entry_ptr2->next = new_entry_ptr;
	new_entry_ptr->next = NULL;
    } else {
	aud_esl_entry_p_t entry_ptr1, entry_ptr2, new_entry_ptr;
	aud_esl_entry_p_t *table = 
			(aud_esl_entry_p_t *) tables[esl_type];

	new_entry_ptr = (aud_esl_entry_p_t)
	    malloc(sizeof(aud_esl_entry_t));
	new_entry_ptr->subject_uuid = subject_uuid;
	new_entry_ptr->guides = guides_ptr;

	if (table[probe] == NULL) {
	    table[probe] = (aud_esl_entry_p_t) new_entry_ptr;
	    ((aud_esl_entry_p_t) new_entry_ptr)->next = NULL;
	    return;
	}
	entry_ptr1 = entry_ptr2 = table[probe];
	while (entry_ptr1 != NULL) {
	    entry_ptr2 = entry_ptr1;
	    entry_ptr1 = entry_ptr1->next;
	}
	entry_ptr2->next = new_entry_ptr;
	new_entry_ptr->next = NULL;
    }
    return;
}

time_t          aud_esl_input_file
                (
		                 aud_esl_type_t esl_type,
				 unsigned32 num_of_relevant_ecs,
				 unsigned32 *relevant_evt_classes,
				 void *** tables,
				 aud_esl_guides_p_t * world_guides,
				 aud_esl_guides_p_t * world_overridable_guides
) {
    int             num_entries;
    struct stat     stat_buf;
    int             size, size_read;
    int             fd;
    int             file_hdr_size;
    char           *data_p, *mem_to_be_freed_p;
    short           file_version;
    time_t          timestamp;
    struct timespec timer;
    struct flock    lock;
    char           *fname = ESL_FILE_NAME(esl_type);

    fd = open(fname, O_RDONLY, 0);
    if (fd == -1) {
	return (0);
    }
    fstat(fd, &stat_buf);
    timestamp = stat_buf.st_mtime;

    timer.tv_nsec = 0;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = 0;
    lock.l_len    = 0;
    while (1) {
	if (fcntl(fd, F_SETLK, &lock) == 0) {
	    break;
	} 
	timer.tv_sec = 5; /* Retry locking 5 secs later. */
	dce_svc_printf(AUD_S_RETRY_LOCK_ESL_FILE_MSG, fname, timer.tv_sec);
	pthread_delay_np(&timer);
    }

    file_hdr_size = sizeof(num_entries) + sizeof(file_version);

    if (stat_buf.st_size <= file_hdr_size) {
	    if (esl_type == aud_e_esl_world) {
		world_guides = NULL;
	    } else if (esl_type == aud_e_esl_world_overridable) {
		world_overridable_guides = NULL;
	    } 
	    return(timestamp);
    }

    size = stat_buf.st_size - file_hdr_size;

    read(fd, (char *) &num_entries, sizeof(num_entries));
    read(fd, (char *) &file_version, sizeof(file_version));
    if (file_version != CURRENT_FILE_VERSION) {
	dce_svc_printf(AUD_S_WRONG_ESL_FILE_VERSION_MSG, fname);
	close(fd);
	return(0);
    }
    data_p = (char *) malloc(size);
    if (data_p == NULL) {
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, size);
	close(fd);
	return(0);
    }
    mem_to_be_freed_p = data_p;
    size_read = read(fd, data_p, size);
    close(fd);

    if (size_read != size) {
	dce_svc_printf(AUD_S_CANNOT_READ_FILE_MSG, fname);
	close(fd);
	return(0);
    } {
	int                     i, j, k, l;
	aud_esl_guides_p_t      guide_ptr, *guide_ptr_ptr;
	aud_esl_guides_p_t      first_guide_ptr;
	aud_esl_evt_classes_p_t ec_ptr, *ec_ptr_ptr;
	short int               num_guides, num_ecs;
	unsigned32              action, condition, evt_class;
	uuid_t                  subject_uuid, cell_uuid;
	boolean32               matching_ec_found = FALSE;
	
	for (i = num_entries; i > 0; i--) {
	    if (!(WORLD_TYPE(esl_type))) {
#ifdef DCE_AUD_DEBUG
		DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		    "aud_esl_input_file: Entry %li:\n", num_entries - i + 1));
#endif
		memcpy((char *) &subject_uuid, data_p, sizeof(subject_uuid));
		data_p += sizeof(subject_uuid);
	    }
	    if (FOREIGN_TYPE(esl_type)) {
		memcpy((char *) &cell_uuid, data_p, sizeof(cell_uuid));
		data_p += sizeof(cell_uuid);
	    }
	    memcpy(&num_guides, data_p, sizeof(num_guides));
	    data_p += sizeof(num_guides);
#ifdef DCE_AUD_DEBUG
	    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    	"Total of %hi guides:\n", num_guides));
#endif
	    first_guide_ptr = NULL;
	    guide_ptr_ptr = &first_guide_ptr;
	    for (j = num_guides; j > 0; j--) {
		guide_ptr = (aud_esl_guides_p_t)
		    malloc(sizeof(aud_esl_guides_t));
#ifdef DCE_AUD_DEBUG
	        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		    "Guide %li:\n", num_guides - j + 1));
#endif
		memcpy(&guide_ptr->audit_action, data_p,
		       sizeof(guide_ptr->audit_action));
		data_p += sizeof(guide_ptr->audit_action);
		memcpy(&guide_ptr->audit_condition, data_p,
		       sizeof(guide_ptr->audit_condition));
		data_p += sizeof(guide_ptr->audit_condition);
		memcpy(&num_ecs, data_p, sizeof(num_ecs));
		data_p += sizeof(num_ecs);

#ifdef DCE_AUD_DEBUG
	        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		    "Action %li, Condition %li, %hi Event Classes\n",
		       guide_ptr->audit_action,
		       guide_ptr->audit_condition,
		       num_ecs));
#endif
		guide_ptr->ec_list = NULL;
		ec_ptr_ptr = &guide_ptr->ec_list;
		for (k = num_ecs; k > 0; k--) {
		    if (num_of_relevant_ecs != 0) {
			matching_ec_found = FALSE;
		        memcpy(&evt_class, data_p, sizeof(evt_class));
		        for (l = num_of_relevant_ecs-1; l >= 0; l--) {
			    if (relevant_evt_classes[l] == evt_class) { 
				matching_ec_found = TRUE;
				break;
			    }
			}
		    }
		    if (num_of_relevant_ecs == 0 || 
			matching_ec_found == TRUE) 
		    {
			ec_ptr = (aud_esl_evt_classes_p_t)
			        malloc(sizeof(aud_esl_evt_classes_t));
		        *ec_ptr_ptr = ec_ptr;
		        ec_ptr->next = NULL;
		        ec_ptr_ptr = &ec_ptr->next;
		        memcpy(&ec_ptr->evt_class, data_p,
			           sizeof(ec_ptr->evt_class));
#ifdef DCE_AUD_DEBUG
			DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		            "%li ", ec_ptr->evt_class));
#endif
		    }
		    data_p += sizeof(unsigned32);
		}
		if (guide_ptr->ec_list == NULL) {
		    free(guide_ptr);
		} else {
		    *guide_ptr_ptr = guide_ptr;
		    guide_ptr->next = NULL;
		    guide_ptr_ptr = &guide_ptr->next;
		}
	    }

	    if (first_guide_ptr != NULL) {
	        if (WORLD_TYPE(esl_type)) {
                    if (esl_type == aud_e_esl_world) {
	                *world_guides = first_guide_ptr;
                    } else {
		        if (esl_type == aud_e_esl_world_overridable) {
	                    *world_overridable_guides = first_guide_ptr;
		        }
                    }
		    return(timestamp);
		}

	        aud__esl_add_entry(esl_type, subject_uuid, 
				      cell_uuid, first_guide_ptr, 
 				      tables);
	    } 
	}
    }
    free(mem_to_be_freed_p); 
    return (timestamp);
}

time_t          aud_esl_input_update_file
                (
		                 aud_esl_type_t esl_type,
				 unsigned32 num_of_relevant_ecs,
				 unsigned32 *relevant_evt_classes,
				 void *** tables
) {
    int		    num_entries;
    char           *fname;
    struct stat     stat_buf;
    int             size;
    int             fd;
    int             file_hdr_size;
    char           *data_p, *mem_to_be_freed_p;
    short           file_version;
    error_status_t  st;
    time_t          timestamp;
    struct timespec timer;
    struct flock    lock;

    fname = ESL_UPDATE_FILE_NAME(esl_type);
    fd = open(fname, O_RDONLY, 0);
    if (fd == -1) {
	return (0);
    }
    fstat(fd, &stat_buf);
    timestamp = stat_buf.st_mtime;
    file_hdr_size = sizeof(num_entries) + sizeof(file_version);

    if (stat_buf.st_size <= file_hdr_size) {
	close(fd);
	return(0);
    }
    size = stat_buf.st_size - file_hdr_size;

    timer.tv_nsec = 0;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = 0;
    lock.l_len    = 0;
    while (1) {
	if (fcntl(fd, F_SETLK, &lock) == 0) {
	    break;
	} 
	timer.tv_sec = 5; /* Retry locking 5 secs later. */
	dce_svc_printf(AUD_S_RETRY_LOCK_ESL_UPDT_FILE_MSG, fname, timer.tv_sec);
	pthread_delay_np(&timer);
    }

    read(fd, (char *) &num_entries, sizeof(num_entries));
    read(fd, (char *) &file_version, sizeof(file_version));
    if (file_version != CURRENT_FILE_VERSION) {
	dce_svc_printf(AUD_S_WRONG_ESL_UPDT_FILE_VERSION_MSG, fname);
	close(fd);
	return(0);
    }
    data_p = (char *) malloc(size);
    if (data_p == NULL) {
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, size);
	close(fd);
	return(0);
    }
    mem_to_be_freed_p = data_p;
    read(fd, data_p, size);
    close(fd);

    {
	int                     i, j, k, l;
	uuid_t                  subject_uuid, cell_uuid;
	aud_esl_guides_p_t      first_guide_ptr, guide_ptr, *guide_ptr_ptr;
	aud_esl_evt_classes_p_t ec_ptr, *ec_ptr_ptr;
	short int               num_guides, num_ecs, operation;
	long                    action, level;
        unsigned32              evt_class;
	boolean32               matching_ec_found;

	for (i = num_entries; i > 0; i--) {
	    memcpy(&operation, data_p, sizeof(operation));
	    data_p += sizeof(operation);
#ifdef DCE_AUD_DEBUG
	    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		"aud_esl_input_update_file: Entry %li:\n", num_entries - i + 1));
#endif
	    memcpy(&subject_uuid, data_p, sizeof(subject_uuid));
	    data_p += sizeof(subject_uuid);
	    if (FOREIGN_TYPE(esl_type)) {
		memcpy(&cell_uuid, data_p, sizeof(cell_uuid));
		data_p += sizeof(cell_uuid);
	    }
	    memcpy(&num_guides, data_p, sizeof(num_guides));
	    data_p += sizeof(num_guides);
#ifdef DCE_AUD_DEBUG
	    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	        "\n Total of %hi guides:\n", num_guides));
#endif
	    first_guide_ptr = NULL;
	    guide_ptr_ptr = &first_guide_ptr;
	    for (j = num_guides; j > 0; j--) {
		guide_ptr = (aud_esl_guides_p_t)
		    malloc(sizeof(aud_esl_guides_t));
#ifdef DCE_AUD_DEBUG
	    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		"Filter %li:\n", num_guides - j + 1));
#endif
		memcpy(&guide_ptr->audit_action, data_p,
		       sizeof(guide_ptr->audit_action));
		data_p += sizeof(guide_ptr->audit_action);
		memcpy(&guide_ptr->audit_condition, data_p,
		       sizeof(guide_ptr->audit_condition));
		data_p += sizeof(guide_ptr->audit_condition);
		memcpy(&num_ecs, data_p, sizeof(num_ecs));
		data_p += sizeof(num_ecs);
#ifdef DCE_AUD_DEBUG
	        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		    "Action %li, Condition %li, %hi Event Classes\n",
		    guide_ptr->audit_action,
		    guide_ptr->audit_condition,
		    num_ecs));
#endif

		guide_ptr->ec_list = NULL;
		ec_ptr_ptr = &guide_ptr->ec_list;
		for (k = num_ecs; k > 0; k--) {
		    if (num_of_relevant_ecs != 0) {
			matching_ec_found = FALSE;
		        memcpy(&evt_class, data_p, sizeof(evt_class));
		        for (l = num_of_relevant_ecs-1; l >= 0; l--) {
			    if (relevant_evt_classes[l] == evt_class) { 
				matching_ec_found = TRUE;
				break;
			    }
			}
		    }
		    if (num_of_relevant_ecs == 0 || 
			matching_ec_found == TRUE) 
		    {
			ec_ptr = (aud_esl_evt_classes_p_t)
			        malloc(sizeof(aud_esl_evt_classes_t));
		        *ec_ptr_ptr = ec_ptr;
		        ec_ptr->next = NULL;
		        ec_ptr_ptr = &ec_ptr->next;
		        memcpy(&ec_ptr->evt_class, data_p,
			           sizeof(ec_ptr->evt_class));
#ifdef DCE_AUD_DEBUG
	        	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		            "%li ", ec_ptr->evt_class));
#endif
		    }
		    data_p += sizeof(unsigned32);
		}
		if (guide_ptr->ec_list == NULL) {
		    free(guide_ptr);
		} else {
		    *guide_ptr_ptr = guide_ptr;
		    guide_ptr->next = NULL;
		    guide_ptr_ptr = &guide_ptr->next;
		}
	    }

	    if (first_guide_ptr != NULL) {
	        if (operation == ESL_UPDATE_ADD_FILTER) {
		    aud_esl_entry_add_guides(tables,
					    esl_type,
					    subject_uuid,
					    cell_uuid,
					    &first_guide_ptr,
					    &st);
	        }
	        if (operation == ESL_UPDATE_REMOVE_FILTER) {
		    aud_esl_entry_remove_guides(tables,
					       esl_type,
					       subject_uuid,
					       cell_uuid,
					       first_guide_ptr);
		    aud_esl_free_guide(first_guide_ptr);
		}
	    } else {
		if (operation == ESL_UPDATE_DELETE_FILTER) {
		    aud_esl_entry_delete_filter(esl_type,
					       tables,
					       subject_uuid,
					       cell_uuid);
		}
	    }
	}
    }
    free(mem_to_be_freed_p);
    return (timestamp);
}
