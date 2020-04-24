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
 * $Log: evt_utils.c,v $
 * Revision 1.1.7.1  1996/07/08  18:32:06  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:30  marty  1.1.5.2]
 *
 * Revision 1.1.5.2  1996/02/18  00:06:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:49  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  17:41:56  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/14  18:42 UTC  jrr
 * 	Change dce_printf() to dce_svc_printf().
 * 	[1995/07/14  18:39 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/15  16:08 UTC  kline_s
 * 	Merge fix for CHFts15468: Check for NULL before freeing any malloc-ed strings.
 * 
 * 	HP revision /main/kline_s_mothra/1  1995/06/13  18:50 UTC  kline_s
 * 	Fix for CHFts15468: Check for NULL before freeing any malloc-ed strings.
 * 	[1995/12/08  17:18:15  root]
 * 
 * Revision 1.1.2.9  1994/09/27  00:27:17  weisz
 * 	CR 10703: in case of wrap, fopen flags should be wb+
 * 	[1994/09/27  00:26:41  weisz]
 * 
 * Revision 1.1.2.8  1994/09/22  21:09:04  weisz
 * 	CR 10703: handling of trail full situation.
 * 	[1994/09/22  21:07:49  weisz]
 * 
 * Revision 1.1.2.7  1994/06/06  17:43:53  weisz
 * 	CR 10829: fix some memory leaks.
 * 	[1994/06/06  15:39:39  weisz]
 * 
 * Revision 1.1.2.6  1994/03/11  22:48:43  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:31:55  luan]
 * 
 * Revision 1.1.2.5  1994/03/02  16:06:17  luan
 * 	CR 10038: Added delegation information in audit record header.
 * 	[1994/03/02  16:05:09  luan]
 * 
 * Revision 1.1.2.4  1994/02/22  16:47:34  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:42:36  luan]
 * 
 * Revision 1.1.2.3  1994/02/18  19:11:57  luan
 * 	CR 9975: Removed an extra free() call in aud__log_free_ev_info_list().
 * 	[1994/02/18  19:05:52  luan]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:42  luan
 * 	CR 9745 9746: Added concat_strns() routine
 * 	string concatenations.
 * 	[1994/01/25  04:16:48  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:35  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:44  zeliff]
 * 
 * $EndLog$
 */

/*
** evt_utils.c
** Utility event logging API, and internal routines.
*/

#include <evtp.h>
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
*/

#include <pathsp.h>

char * concat_strns (
    char    **dest,
    char    *prefix,
    char    *str,
    char    *suffix
)
{
    int   len1 = 0;
    int   len2 = 0;
    int   len3 = 0;
    int   len;
    char *p;
    char *q;

    if (prefix) {
        len1 = strlen(prefix);
    }

    if (str) {
        len2 = strlen(str);
    }

    if (suffix) {
        len3 = strlen(suffix);
    }

    len = len1 + len2 + len3;

    p = malloc(len + 1);
    if (p) {
        q = p;
        strcpy(q, prefix);
        q += len1;
        strcpy(q, str);
        q += len2;
        strcpy(q, suffix);
    }

    *dest = p;
    return p;
} 

void aud__log_dup_acl (sec_acl_t *org_acl, sec_acl_t **dup_acl) 
{
    int               i;
    sec_id_t         *org_id, *dup_id;
    sec_id_foreign_t *org_foreign_id, *dup_foreign_id;

    *dup_acl = (sec_acl_t *) malloc(sizeof(sec_acl_t));

    (*dup_acl)->sec_acl_entries = (sec_acl_entry_t *)
	malloc(sizeof(sec_acl_entry_t) * org_acl->num_entries);

    (*dup_acl)->default_realm.uuid = org_acl->default_realm.uuid;
    (*dup_acl)->sec_acl_manager_type = org_acl->sec_acl_manager_type;
    (*dup_acl)->num_entries = org_acl->num_entries;
    (*dup_acl)->default_realm.name = (unsigned_char_t *)
	malloc(strlen((char *)org_acl->default_realm.name) + 1);
    strcpy((char *)(*dup_acl)->default_realm.name, (char *)org_acl->default_realm.name);

    for (i = 0; i < org_acl->num_entries; i++) {
	(*dup_acl)->sec_acl_entries[i].perms =
	    org_acl->sec_acl_entries[i].perms;
	(*dup_acl)->sec_acl_entries[i].entry_info.entry_type =
	    org_acl->sec_acl_entries[i].entry_info.entry_type;
	switch ((*dup_acl)->sec_acl_entries[i].entry_info.entry_type) {

	case sec_acl_e_type_user_obj:
	case sec_acl_e_type_group_obj:
	case sec_acl_e_type_other_obj:
	case sec_acl_e_type_unauthenticated:
	case sec_acl_e_type_any_other:

	    /* don't need to copy anything */

	    break;

	case sec_acl_e_type_user:
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_other:

	    /* copy id */
	    org_id =
		&(org_acl->sec_acl_entries[i].entry_info.tagged_union.id);
	    dup_id =
		&((*dup_acl)->sec_acl_entries[i].entry_info.tagged_union.id);
	    dup_id->uuid = org_id->uuid;
	    dup_id->name = (unsigned_char_t *) malloc(strlen((char *)org_id->name) + 1);
	    strcpy((char *)dup_id->name, (char *)org_id->name);
	    break;

	case sec_acl_e_type_foreign_user:
	case sec_acl_e_type_foreign_group:

	    /* copy user name and uuid */
	    org_foreign_id =
		&(org_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id);
	    dup_foreign_id =
		&((*dup_acl)->sec_acl_entries[i].entry_info.tagged_union.foreign_id);
	    dup_foreign_id->id.uuid = org_foreign_id->id.uuid;
	    dup_foreign_id->id.name = (unsigned_char_t *)
		malloc(strlen((char *)org_foreign_id->id.name) + 1);
	    strcpy((char *)dup_foreign_id->id.name, (char *)org_foreign_id->id.name);

	    /* copy cell name and uuid */
	    dup_foreign_id->realm.uuid = org_foreign_id->realm.uuid;
	    dup_foreign_id->realm.name = (unsigned_char_t *) 
	        malloc(strlen((char *) org_foreign_id->realm.name) + 1);
	    strcpy((char *)dup_foreign_id->realm.name, (char *)org_foreign_id->realm.name);

	    break;

	case sec_acl_e_type_extended:
	default:
	    break;
	}			/* end of switch */
    }				/* end of for loop */
}

void aud__log_free_acl (sec_acl_t *acl) 
{
    int               i;
    sec_id_t         *id;
    sec_id_foreign_t *foreign_id;


    for (i = 0; i < acl->num_entries; i++) {
	switch (acl->sec_acl_entries[i].entry_info.entry_type) {

	case sec_acl_e_type_user:
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_other:

	    id = &(acl->sec_acl_entries[i].entry_info.tagged_union.id);
	    if (id->name != NULL) {
		free(id->name);
	    }
	    break;

	case sec_acl_e_type_foreign_user:
	case sec_acl_e_type_foreign_group:

	    foreign_id =
		&(acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id);
	    if (foreign_id->id.name != NULL) {
		free(foreign_id->id.name);
	    }
	    if (foreign_id->realm.name != NULL) {
		free(foreign_id->realm.name);
	    }
	    break;

	default:
	    break;
	}			/* end of switch */
    }				/* end of for loop */

    free(acl->sec_acl_entries);

    if (acl->default_realm.name != NULL) {
	free(acl->default_realm.name);
    }
    free(acl);
}

void     aud__log_free_ev_info_list
                (
		                 aud_log_ev_info_list_t * tail
) {
    int                i;
    aud_log_ev_info_t *evip;

    if (tail == NULL) {
	return;
    }
    for (i = 0; i < tail->item_count; i++) {
	evip = &tail->info_item[i];
	switch (evip->format) {

	case aud_c_evt_info_hyper_int:
	    free(evip->tagged_union.hyper_int);
	    break;

	case aud_c_evt_info_uhyper_int:
	    free(evip->tagged_union.uhyper_int);
	    break;

	case aud_c_evt_info_long_float:
	    free(evip->tagged_union.long_float);
	    break;

	case aud_c_evt_info_uuid:
	    free(evip->tagged_union.uuid);
	    break;

	case aud_c_evt_info_utc:
	    free(evip->tagged_union.utc);
	    break;

	case aud_c_evt_info_acl:
	    aud__log_free_acl(evip->tagged_union.acl);
	    break;

	case aud_c_evt_info_byte_string:
	    if (evip->tagged_union.byte_string != NULL)
                free(evip->tagged_union.byte_string);
	    break;

	case aud_c_evt_info_char_string:
	    if (evip->tagged_union.char_string != NULL)
                free(evip->tagged_union.char_string);
	    break;

	default:
	    ;
	}
    }
    free(tail);
}

void dce_aud_free_header
                (
		                 dce_aud_hdr_t * header,
		                 error_status_t * status
) {
    int		id_index;

    if (header == NULL) {
	return;
    }

    if (header->addr != NULL) {
	free(header->addr);
    }

    for (id_index = header->num_client_ids - 1; id_index >= 0; id_index--) {
	if (header->client_id[id_index].num_groups != 0) {
	    free(header->client_id[id_index].groups); 
	}
    }
    free(header->client_id);
    *status = aud_s_ok;
    return;
}

void     dce_aud_free_ev_info
                (
		                 dce_aud_ev_info_t * info_item,
		                 error_status_t * status
) {
    *status = aud_s_ok;

    if (info_item == NULL) {
	return;
    }
    switch (info_item->format) {

    case aud_c_evt_info_acl:
	aud__log_free_acl(info_item->data.acl);
	break;

    case aud_c_evt_info_byte_string:
        if (info_item->data.byte_string != NULL)
            free(info_item->data.byte_string);
	break;

    case aud_c_evt_info_char_string:
        if (info_item->data.char_string != NULL)
            free(info_item->data.char_string);
	break;

    default:
	;
    }
    free(info_item);
    return;
}

void
                dce_aud_discard
                (
		                 dce_aud_rec_t ard,
		                 unsigned32 * status
) {
    *status = aud_s_ok;

    if (ard == NULL) {
	return;
    }
    if (ard->as != NULL) {
	free(ard->as);
    }
    if (ard->tail != NULL) {
	aud__log_free_ev_info_list(ard->tail);
    }
    
    dce_aud_free_header(&ard->header, status);
    free(ard);
    return;
}

void
                dce_aud_modify_sstrategy
                (
		                 dce_aud_trail_t at,
                                 unsigned32 strgy,
                                 error_status_t * status

) {

    if (at == NULL) {
        dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
        *status = aud_s_invalid_trail_descriptor;
        return;
    }

    *status = aud_s_ok;
   
    if ((strgy & ~aud_c_trl_ss_bits) != 0)
    {
        *status = aud_s_trl_invalid_stostrategy;
        dce_svc_printf(AUD_S_TRL_INVALID_STOSTRATEGY_MSG);
        return;
    }

    pthread_mutex_lock(&at->mutex);
    at->flags &= ~aud_c_trl_ss_bits;
    at->flags |= strgy;
    pthread_mutex_unlock(&at->mutex);
    return;

}



void aud_trl_file_flush (dce_aud_trail_t at)
{
    if (at == NULL) {
        dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
        return;
    }

    fflush(at->trail_fp);
    fflush(at->md_index_fp);
    fsync(at->trail_fd);
    fsync(at->md_index_fd);
    return;
}
    

void aud_trl_file_write
                (
		                 dce_aud_trail_t at,
		                 idl_byte * ep,
		                 idl_ulong_int esize,
				 unsigned32 options,
		                 unsigned32 * status
) {
    boolean32 have_to_sync = FALSE;
    char     *open_flags;
    char      buf[FILE_NAME_LENGTH];
    char      time_buf[FILE_NAME_LENGTH];
    char      backup_trail[FILE_NAME_LENGTH];
    char      backup_trail_index[FILE_NAME_LENGTH];
    char      index_file[FILE_NAME_LENGTH];
    FILE     *trail_fp, *md_index_fp;
    utc_t     utctime;
    struct    tm tmTime;
    struct    tm tmInacc;
    long      tns;
    long      ins;
    long      trail_file_pos;
    long      index_file_pos;
    int       rc = 0;
    struct    stat stbuf;
    
    if (at == NULL) {
        dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
        *status = aud_s_invalid_trail_descriptor;
        return;
    }

    *status = aud_s_ok;

    if ((options & aud_c_evt_commit_sync_no_wait) != 0 ||
	(options & aud_c_evt_commit_sync) != 0) {	
	have_to_sync = TRUE;
    }

    pthread_mutex_lock(&at->mutex);

    if (at->trail_cursor + esize > at->file_size_limit_value)
    {
       if ((at->flags & aud_c_trl_ss_wrap) != 0)
       {
          aud_trl_file_flush(at);

          trail_file_pos = ftell(at->trail_fp);
          if (trail_file_pos != -1)
          {
             rc = ftruncate(at->trail_fd, trail_file_pos);
             rc = fseek(at->trail_fp, 0, SEEK_SET);
             at->trail_cursor = 0;
          }

          index_file_pos = ftell(at->md_index_fp);
          if (index_file_pos != -1)
          {
             rc = ftruncate(at->md_index_fd, index_file_pos);
             rc = fseek(at->md_index_fp, 0, SEEK_SET);
          }
       }
       else
       {
          fclose(at->trail_fp);
          fclose(at->md_index_fp);

          if (utc_gettime(&utctime) != 0)
          {
             dce_svc_printf(AUD_S_CANNOT_GETTIME_MSG);
             *status = aud_s_cannot_gettime;
             pthread_mutex_unlock(&at->mutex);
             return;
          }

          rc =  utc_gmtime(&tmTime, &tns, NULL, NULL, &utctime);
          if ( rc == -1)
          {
             dce_svc_printf(AUD_S_CANNOT_GETGMTIME_MSG);
             *status = aud_s_cannot_getgmtime;
             pthread_mutex_unlock(&at->mutex);
             return;
          }
          else
          {
             sprintf(time_buf,"%04d-%02d-%02d-%02d-%02d-%02d",
                     tmTime.tm_year+1900,
                     ++tmTime.tm_mon,
                     tmTime.tm_mday,
                     tmTime.tm_hour,
                     tmTime.tm_min,
                     tmTime.tm_sec);

             strcpy(backup_trail, at->trail_file);
             strcat(backup_trail, ".");
             strcat(backup_trail, time_buf);
          }

          if (rename(at->trail_file, backup_trail) == -1)
          {
              dce_svc_printf(AUD_S_RENAME_TRAIL_FILE_MSG, at->trail_file, backup_trail);
              *status = aud_s_rename_trail_file_rc;
              pthread_mutex_unlock(&at->mutex);
              return;
          }

          open_flags = "ab+";
          trail_fp = fopen(at->trail_file, open_flags);
          if (trail_fp == NULL)
          {
             dce_svc_printf(AUD_S_CANNOT_REOPEN_TRAIL_FILE_MSG, at->trail_file);
             *status = aud_s_cannot_reopen_trail_file_rc;
             pthread_mutex_unlock(&at->mutex);
             return;
          }

          at->trail_fp = trail_fp;
          at->trail_fd = fileno(trail_fp);
          at->trail_cursor = 0;

          strcpy(index_file, at->trail_file);
          strcat(index_file, ".md_index");
          strcpy(backup_trail_index, backup_trail);
          strcat(backup_trail_index, ".md_index");

          if (rename(index_file, backup_trail_index) == -1)
          {
              dce_svc_printf(AUD_S_RENAME_TRAIL_INDEX_FILE_MSG, index_file, backup_trail_index);
              *status = aud_s_rename_trail_index_file_rc;
              pthread_mutex_unlock(&at->mutex);
              return;
          }

          if ((md_index_fp = fopen(index_file, open_flags)) == NULL)
          {
             dce_svc_printf(AUD_S_CANNOT_REOPEN_TRAIL_INDEX_FILE_MSG, index_file);
             *status = aud_s_cannot_reopen_trail_index_file_rc;
              pthread_mutex_unlock(&at->mutex);
             return;
          }
         
          at->md_index_fp = md_index_fp;
          at->md_index_fd = fileno(md_index_fp);

          dce_svc_printf(AUD_S_TRL_FILE_SIZE_LIMIT_MSG, backup_trail, backup_trail_index);
      }
   }
 
   /* MUST reset the current file pointer if the file is rewound */
   /* by an application which is running in different process    */
   fstat(at->trail_fd, &stbuf);
   if (stbuf.st_size != at->trail_cursor)
       at->trail_cursor = stbuf.st_size;

   if (fwrite(ep, sizeof(idl_byte), esize, at->trail_fp) != esize) {
       dce_svc_printf(AUD_S_TRL_WRITE_FAILURE_MSG, at->trail_file);
      *status = aud_s_trl_write_failure;
       pthread_mutex_unlock(&at->mutex);
       return;
   }

   /* monitor the resources */
   if (ferror(at->trail_fp)) {
       dce_svc_printf(AUD_S_TRL_FULL_MSG, at->trail_file);
      *status = aud_s_trl_full;
       pthread_mutex_unlock(&at->mutex);
       return;
   }
   fwrite((void *)&at->trail_cursor, sizeof(idl_byte), sizeof(unsigned32),
           at->md_index_fp);
   /* monitor the resources */
   if (ferror(at->md_index_fp)) {
       dce_svc_printf(AUD_S_TRL_FULL_MSG, at->index_file);
      *status = aud_s_trl_full;
       pthread_mutex_unlock(&at->mutex);
       return;
   }

#ifdef DCE_AUD_DEBUG
   DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
        "trail cursor = %ld, esize = %ld\n", at->trail_cursor, esize));
#endif
   at->trail_cursor += esize;
   fwrite((void *)&esize, sizeof(idl_byte), sizeof(unsigned32),
               at->md_index_fp);
   /* monitor the resources */
   if (ferror(at->md_index_fp)) {
       dce_svc_printf(AUD_S_TRL_FULL_MSG, at->index_file);
      *status = aud_s_trl_full;
       pthread_mutex_unlock(&at->mutex);
       return;
   }

   if (have_to_sync) {
       aud_trl_file_flush(at);
   }


   pthread_mutex_unlock(&at->mutex);
   return;
}
