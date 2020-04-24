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
 * $Log: dmn_esl_file_write.c,v $
 * Revision 1.1.6.1  1996/07/08  18:25:25  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:00  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  00:06:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:35  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:38:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:33  root]
 * 
 * Revision 1.1.2.5  1994/03/11  22:48:26  luan
 * 	CR 9774: Changed to use memcpy instead of bcopy().
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:39:52  luan]
 * 
 * Revision 1.1.2.4  1994/02/18  21:42:31  luan
 * 	CR 9777: Use <dce/dceaudmsg.h> instead of <dceaudmsg.h> since
 * 	it is exported.
 * 	[1994/02/18  21:42:08  luan]
 * 
 * Revision 1.1.2.3  1994/02/02  23:03:14  ohara
 * 	add include unistd.h, neede on SVR4
 * 	[1994/02/02  23:00:37  ohara]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:34  luan
 * 	CR 9745 9746: File name paths consolidated.
 * 	Hard-coded file names replaced with macros
 * 	which generate names dynamically.
 * 	[1994/01/25  04:01:13  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:38:50  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:08  zeliff]
 * 
 * $EndLog$
 */

/* 
** dmn_esl_file_write.c
** Outputs filters and filter updates to files.
*/

#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <dce/dce.h>
#include <dce/dce_svc.h>
#include <dce/dce_msg.h>
#include <pthread.h>

#include <eslp.h>
#include <pathsp.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudsvc.h>
#include <dce/dceaudmac.h>
#include <unistd.h>


extern boolean32 debug_mode;
extern int      errno;

static int      total_size;
static char     buf[1024];
static char    *buf_p;
static int      size;
static int      room;
static int      num_entries;

extern void               **aud_g_dmn_esl[];
extern aud_esl_guides_p_t   aud_g_dmn_world_guides; 
extern aud_esl_guides_p_t   aud_g_dmn_world_overridable_guides;

typedef struct {
    unsigned32      audit_action;
    unsigned32      audit_condition;
    short           num_evt_classes;
}               aud_esl_file_guide_hdr_t;

#define CURRENT_FILE_VERSION 1
#define FILE_LOCK_RETRY_INTERVAL 5 /* 5 seconds */

#define append_data(fd, data_ptr, size) \
        if ((size > room) && (room < sizeof(buf))) { \
            write(fd, buf, sizeof(buf) - room); \
            room = sizeof(buf); \
            buf_p = buf; \
        } \
 \
        if (size > room) { \
            write(fd, (char *) data_ptr, size); \
        } else { \
	    memcpy((char *) buf_p, data_ptr, size); \
            buf_p += size; \
            room -= size; \
        } \
 \
        total_size += size; \

static          aud__esl_output_guide
                (
				 int fd,
		                 aud_esl_guides_p_t guide_ptr
) {

    aud_esl_file_guide_hdr_t guide_hdr;
    aud_esl_evt_classes_p_t  ec_ptr;
    short                    num_evt_classes = 0;
    unsigned32              *ec_list_ptr;

    ec_ptr = guide_ptr->ec_list;

    if (ec_ptr == NULL) {
#ifdef DCE_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
			"Empty event class list in a filter!\n"));
#endif 
	return;
    }
    while (ec_ptr != NULL) {
	num_evt_classes++;
	ec_ptr = ec_ptr->next;
    }

    guide_hdr.num_evt_classes = num_evt_classes;
    guide_hdr.audit_action = guide_ptr->audit_action;
    guide_hdr.audit_condition = guide_ptr->audit_condition;
    append_data(fd, &guide_hdr, sizeof(guide_hdr.num_evt_classes)
		+ sizeof(guide_hdr.audit_action)
		+ sizeof(guide_hdr.audit_condition)
    );

    /*
     * Cannot use "sizeof(guide_hdr)" because the fullword boundary problem 
     */

    ec_ptr = guide_ptr->ec_list;
    ec_list_ptr = (unsigned32 *) malloc(4 * num_evt_classes);

    while (ec_ptr != NULL) {
	*ec_list_ptr = ec_ptr->evt_class;
	ec_list_ptr++;
	ec_ptr = ec_ptr->next;
    }
    ec_list_ptr -= num_evt_classes;
    append_data(fd, ec_list_ptr, 4 * num_evt_classes);

    free(ec_list_ptr); /* Watch out for memory leaks. */
    return;
}

static void     aud__esl_output_guides
                (
				 int fd,
		                 aud_esl_guides_p_t first_guide_ptr
) {
    aud_esl_guides_p_t guide_ptr = first_guide_ptr;
    short              num_guides = 0;

    while (guide_ptr != NULL) {
	num_guides++;
	guide_ptr = guide_ptr->next;
    }

    append_data(fd, &num_guides, sizeof(num_guides));
    guide_ptr = first_guide_ptr;

    while (guide_ptr != NULL) {
	aud__esl_output_guide(fd, guide_ptr);
	guide_ptr = guide_ptr->next;
    }
    return;
}

static void     aud__esl_output_entry
                (
				 int fd, 
		                 aud_esl_type_t esl_type,
		                 uuid_t subject_uuid,
		                 uuid_t cell_uuid,
		                 aud_esl_guides_p_t first_guide_ptr
) { 
/* delete operation has null first_guide_ptr */
/* if              (first_guide_ptr == NULL)
	                return; */

    append_data(fd, &subject_uuid, sizeof(subject_uuid));
    if (FOREIGN_TYPE(esl_type)) {
	append_data(fd, &cell_uuid, sizeof(cell_uuid));
    }
    aud__esl_output_guides(fd, first_guide_ptr);
    num_entries++;
    return;
}

static void     aud__esl_output_table
                ( 
				 int fd,
		                 aud_esl_type_t esl_type
) {
    int             i;

    if (FOREIGN_TYPE(esl_type)) {
	aud_esl_foreign_entry_p_t *table =
	(aud_esl_foreign_entry_p_t *) aud_g_dmn_esl[esl_type];
	aud_esl_foreign_entry_p_t entry_ptr;

	for (i = 0; i < HASH_TABLE_SIZE; i++) {
	    uuid_t          cell_uuid;
	    entry_ptr = table[i];
	    while (entry_ptr != NULL) {
		aud__esl_output_entry(fd, esl_type, entry_ptr->subject_uuid,
					 cell_uuid, entry_ptr->guides);
		entry_ptr = entry_ptr->next;
	    }
	}
    } else {
	uuid_t          cell_uuid;	/* needed for parameter passing below */
	aud_esl_entry_p_t *table =
	(aud_esl_entry_p_t *) aud_g_dmn_esl[esl_type];
	aud_esl_entry_p_t entry_ptr;

	for (i = 0; i < HASH_TABLE_SIZE; i++) {
	    entry_ptr = table[i];
	    while (entry_ptr != NULL) {
		aud__esl_output_entry(fd, esl_type, entry_ptr->subject_uuid,
					 cell_uuid, entry_ptr->guides);
		entry_ptr = entry_ptr->next;
	    }
	}
    }
    return;
}

int             aud_esl_save_state
                (
		                 aud_esl_type_t esl_type
) {
    short           file_version = CURRENT_FILE_VERSION;
    char           *fname;
    int		    fd;
    struct          timespec timer;
    char	    name_buffer[FILE_NAME_SIZE_LIMIT];
    struct flock    lock;

    num_entries = 0;

    /* Save the old esl file and esl update file in a different  */
    /* name.  For the system may crash during this checkpoint.   */
    /* The audit daemon and the audit clients should read the    */
    /* ".old" file if the esl file is incomplete or corrupted.   */ 

    if (!WORLD_TYPE(esl_type)) {
    	fname = ESL_UPDATE_FILE_NAME(esl_type);
        if (access(fname, F_OK) == 0) {
    	    strcpy(name_buffer, fname);
    	    strcat(name_buffer, ".old");
    	    if (rename(fname, name_buffer) == -1 ) {
	        dce_svc_printf(AUD_S_RENAME_ESL_UPDATE_FILE_MSG, fname, name_buffer);
            }
        }
    }

    fname = ESL_FILE_NAME(esl_type);
    if (access(fname, F_OK) == 0) {
        strcpy(name_buffer, fname);
        strcat(name_buffer, ".old");
	if (rename(fname, name_buffer) == -1) {
	    dce_svc_printf(AUD_S_RENAME_ESL_FILE_MSG, fname, name_buffer);
	}
    }

    fd = open(fname, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
	dce_svc_printf(AUD_S_OPEN_ESL_FILE_MSG, fname);
    }

    timer.tv_nsec = 0;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = 0;
    lock.l_len    = 0;
    while (1) {
	if (fcntl(fd, F_SETLK, &lock) == 0) {
	    break;
	} 
	dce_svc_printf(AUD_S_LOCK_ESL_FILE_MSG, fname);
	timer.tv_sec = FILE_LOCK_RETRY_INTERVAL; 
#ifdef DCE_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
			"Retry locking file %s %d secs later\n", 
			fname, FILE_LOCK_RETRY_INTERVAL));
#endif 
	pthread_delay_np(&timer);
    }

    total_size = 0;

    write(fd, (char *) &num_entries, sizeof(num_entries));
    write(fd, (char *) &file_version, sizeof(file_version));

    buf_p = buf;
    room = sizeof(buf);

    if (!(WORLD_TYPE(esl_type))) {
        aud__esl_output_table(fd, esl_type);
    } else {
        if (esl_type == aud_e_esl_world) {
	    if (aud_g_dmn_world_guides != NULL) {
	        num_entries++;
	        aud__esl_output_guides(fd, aud_g_dmn_world_guides);
	    }
        }
        if (esl_type == aud_e_esl_world_overridable) {
	    if (aud_g_dmn_world_overridable_guides != NULL) {
	        num_entries++;
	        aud__esl_output_guides(fd, aud_g_dmn_world_overridable_guides);
	    }
        }
    }

    if (room < sizeof(buf)) {
	write(fd, buf, sizeof(buf) - room);
    }
    lseek(fd, 0, SEEK_SET);
    write(fd, (char *) &num_entries, sizeof(num_entries));

    fsync(fd);
    close(fd);
    return (total_size);
}


void            aud_esl_output_update_entry
                (
		                 short int update_operation,
		                 aud_esl_type_t esl_type,
		                 uuid_t subject_uuid,
		                 uuid_t cell_uuid,
		                 aud_esl_guides_p_t guides_ptr
) {
    char           *fname;
    struct stat     stat_buf;
    int             size;
    int             fd;
    int             file_hdr_size;
    short           file_version;
    struct          timespec timer;
    struct flock    lock;

    if (update_operation == ESL_UPDATE_DELETE_FILTER &&  
	guides_ptr != NULL)
    {
#ifdef DCE_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		"operation is delete filter, guides_ptr should be NULL"));
#endif 
	return;
    }

    fname = ESL_UPDATE_FILE_NAME(esl_type);

    fd = open(fname, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
	dce_svc_printf(AUD_S_OPEN_ESL_UPDATE_FILE_MSG, fname);
    }
    if (fstat(fd, &stat_buf) < 0) {
	dce_svc_printf(AUD_S_STAT_ESL_UPDATE_FILE_MSG, fname);
    }
    if (stat_buf.st_size > ESL_UPDATE_FILE_SIZE_LIMIT) {
#ifdef DCE_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		"update file %s size limit exceeded", fname));
#endif
	aud_esl_save_state(esl_type);
	ftruncate(fd, 0);
	fsync(fd);
	close(fd);
	return;
    }

    timer.tv_nsec = 0;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = 0;
    lock.l_len    = 0;
    while (1) {
	if (fcntl(fd, F_SETLK, &lock) == 0) {
	    break;
	} 
	perror("aud_esl_output_update_entry");
	timer.tv_sec = 5; /* Retry locking 5 secs later. */
	dce_svc_printf(AUD_S_RETRY_LOCK_ESL_UPDT_FILE_MSG, 
				fname, timer.tv_sec);
	pthread_delay_np(&timer);
    }

    file_hdr_size = sizeof(num_entries) + sizeof(file_version);

    if (stat_buf.st_size <= file_hdr_size) {
	num_entries = 0;
	file_version = CURRENT_FILE_VERSION;
	write(fd, (char *) &num_entries, sizeof(num_entries));
	write(fd, (char *) &file_version, sizeof(file_version));
    } else {
	read(fd, (char *) &num_entries, sizeof(num_entries));
	read(fd, (char *) &file_version, sizeof(file_version));
	if (file_version != CURRENT_FILE_VERSION) {
	    dce_svc_printf(AUD_S_WRONG_VERSION_ESL_UPDT_FILE_MSG, fname);
	}
    }

    buf_p = buf;
    room = sizeof(buf);
    lseek(fd, 0, SEEK_END);
    append_data(fd, &update_operation, sizeof(update_operation));

    /* if guides_ptr = NULL, the whole filter is deleted */

    aud__esl_output_entry(fd, esl_type, subject_uuid, cell_uuid, guides_ptr);

    if (room < sizeof(buf)) {
	write(fd, buf, sizeof(buf) - room);
    }
    lseek(fd, 0, SEEK_SET);
    write(fd, (char *) &num_entries, sizeof(num_entries));

    fsync(fd);
    close(fd);
    return;
}
