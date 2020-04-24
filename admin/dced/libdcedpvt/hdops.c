/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: hdops.c,v $
 * Revision 1.1.4.1  1996/11/13  17:55:58  arvind
 * 	fix dced acl code returns wrong error on unauthorized
 * 	[1996/10/09  19:39 UTC  bartf  /main/bartf_pk_fix/1]
 *
 * 	fix incorrect invalid_permission error to be not_authorized
 * 	[1996/02/18  19:16:12  marty  1.1.2.2]
 *
 * Revision 1.1.2.2  1996/02/18  19:16:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:09  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  20:53:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  17:49 UTC  tatsu_s
 * 	Added anonymous hostdata directory.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:30 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/1  1995/08/29  22:27 UTC  tatsu_s
 * 	Submitted the fix for OT13045.
 * 
 * 	HP revision /main/tatsu_s.psock_timeout.b0/1  1995/08/23  21:01 UTC  tatsu_s
 * 	Added no_remote_update and entry_c_data_imported flag.
 * 	[1995/12/08  20:48:57  root]
 * 
 * Revision 1.1.2.23  1994/10/03  20:15:21  pwang
 * 	Added O_CREAT and O_TRUNC to open() in hostdata_create()
 * 	[OT#12467]
 * 
 * Revision 1.1.2.22  1994/09/16  20:51:10  bowe
 * 	SVC work - cast error_status_t to (long) for dce_svc_printf [CR 11725]
 * 	[1994/09/16  20:50:40  bowe]
 * 
 * Revision 1.1.2.21  1994/09/15  15:17:48  bowe
 * 	Handle 0 cell aliases correctly in hostdata_get [CR 12182]
 * 	[1994/09/15  15:17:24  bowe]
 * 
 * Revision 1.1.2.20  1994/09/14  20:04:17  bowe
 * 	SVC work [CR 11725]
 * 	Delete ACL when deleting the object [CR 12030]
 * 	[1994/09/14  20:03:40  bowe]
 * 
 * Revision 1.1.2.19  1994/09/12  18:53:13  bowe
 * 	In hostdata_create(), don't check file existence if h=NULL [CR 12136].
 * 	[1994/09/12  18:41:08  bowe]
 * 
 * Revision 1.1.2.18  1994/09/02  20:58:46  bowe
 * 	Use dce_cf_get_cell_aliases() [CR 11843].
 * 	SVC work [CR 11725].
 * 	[1994/09/02  20:57:58  bowe]
 * 
 * Revision 1.1.2.17  1994/08/28  00:58:39  rsalz
 * 	hd_post_proc passes in the item name through dced_fork (OT CR 11895).
 * 	[1994/08/26  19:14:24  rsalz]
 * 
 * Revision 1.1.2.16  1994/06/13  20:44:37  bowe
 * 	Check that name does not exist on create. [CR 10702]
 * 	[1994/06/13  20:14:54  bowe]
 * 
 * Revision 1.1.2.15  1994/05/26  19:42:12  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:42  rsalz]
 * 
 * Revision 1.1.2.14  1994/05/13  19:21:10  bowe
 * 	Correct param to dced_authorized() in get_list
 * 	[1994/05/13  19:20:58  bowe]
 * 
 * Revision 1.1.2.13  1994/05/12  19:30:29  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:27  rsalz]
 * 
 * Revision 1.1.2.12  1994/05/11  17:08:31  bowe
 * 	Consider binary attr when returning "canned data"
 * 	[1994/05/11  17:08:12  bowe]
 * 
 * Revision 1.1.2.11  1994/05/11  15:33:01  bowe
 * 	Correct args to dce_db_std_header_init() [CR 10603]
 * 	[1994/05/11  15:31:28  bowe]
 * 
 * Revision 1.1.2.10  1994/05/10  20:15:11  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:40  rsalz]
 * 
 * Revision 1.1.2.9  1994/04/26  19:25:50  bowe
 * 	New params to dce_acl_is_client_authorized().
 * 	Set prin (owner) when creating a new entry.
 * 	[1994/04/26  19:22:52  bowe]
 * 
 * Revision 1.1.2.8  1994/04/18  15:05:38  bowe
 * 	When creating, first check if object exists (and report error).
 * 	In all ops, if handle is NULL, skip ACL check.
 * 	[1994/04/18  15:04:37  bowe]
 * 
 * Revision 1.1.2.7  1994/04/11  15:56:47  bowe
 * 	Bad use of uuid_equal() - extra !
 * 	[1994/04/11  15:56:30  bowe]
 * 
 * Revision 1.1.2.6  1994/04/09  21:22:53  bowe
 * 	In hostdata_get(): return the correct uuid (one of the 2 well-knowns).
 * 	Rewrote the part that reads the file and returns the contents.
 * 	Fixed some "gcc -Wall" warnings.
 * 	[1994/04/09  21:22:24  bowe]
 * 
 * Revision 1.1.2.5  1994/04/07  20:54:27  bowe
 * 	Allocate returned list ahead of time in hostdata_get_list().  Check
 * 	attr type in hostdata_get().  Added more checks for returned status.
 * 	[1994/04/07  20:53:49  bowe]
 * 
 * Revision 1.1.2.4  1994/03/29  22:31:30  bowe
 * 	Added name->uuid cache calls in create/delete.
 * 	Handle newlines correctly when writing hostdata objects (files).
 * 	Set return value in hostdata_get, even if error occurs.
 * 	[1994/03/29  22:26:16  bowe]
 * 
 * Revision 1.1.2.3  1994/03/18  21:28:41  bowe
 * 	Fixed size when reallocating size of list (was wrong)
 * 	[1994/03/18  21:28:27  bowe]
 * 
 * Revision 1.1.2.2  1994/03/18  19:51:28  bowe
 * 	Adapt code to use ERA attributes.
 * 	[1994/03/18  19:41:24  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:52  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:21  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <libimpl.h>
#include <hdimpl.h>

/* ______________________________________________________________________ */

/* Create a new host configuration item. */
void
hostdata_create(
  /* in */
    handle_t		h,
    dced_entry_t	*id,
    boolean32		just_import,
    sec_attr_t		*attr,
  /* out */
    error_status_t	*st
)
{
    int			fd;
    int			access_return;
    unsigned32		i;
    FILE		*fp;
    uuid_t		au;
    sec_acl_t		i_acl;
    dced_entry_data_t	id_d;
    uuid_t		Prin;	/* of caller */
    sec_acl_permset_t	perms = dced_perm_insert;

    *st = error_status_ok;

    if (h != NULL && dced_is_remote_disabled) {
	*st = sec_acl_not_authorized;
	return;
    }

    if (*(char *)id->storage_tag != '/') {
	if (h != NULL
	    && strncmp((char *)id->storage_tag, dced_c_service_hostdata "/",
		       dced_c_service_hostdata_len+1) != 0) {
	    *st = sec_acl_invalid_permission;
	    return;
	}
    }
    else if (strncmp((char *)id->storage_tag, dced_g_anonymous_path,
		     strlen(dced_g_anonymous_path)) != 0)
	perms |= dced_perm_insertpriv;

    if (!dced_authorized(h, &hd_cont_type, &hd_acl, &Prin, perms, st))
	return;

    /* See if this object exists first. */
    dce_db_fetch_by_uuid(hd_db, &id->id, &id_d, st);
    if (*st == error_status_ok ||
	    name_cache_find_uuid(hd_name_cache, (char *)id->name, &au) ) {
	*st = dced_s_already_exists;
	return;
    }

    access_return = access((char *)id->storage_tag, F_OK);
    if (just_import) {
	/* Importing - file must exist.  We do not write the file. */
	if (access_return != 0 && h != NULL) {
	    *st = dced_s_import_cant_access;
	    return;
	}
    }
    else {
	/* Not importing - file must not exist.  We'll write the file. */
	if (access_return == 0 && h != NULL) {
	    *st = dced_s_import_already_exists;
	    return;
	}

	switch (attr->attr_value.attr_encoding) {
	default:
	    *st = dced_s_unknown_attr_type;
	    return;

	case sec_attr_enc_printstring:
	    if (!(fp = fopen((char *)id->storage_tag, "w"))) {
		*st = dced_s_cant_open_storage_file;
		return;
	    }
	    fputs((char *)attr->attr_value.tagged_union.printstring, fp);
	    fputc('\n', fp);
	    fclose(fp);
	    break;

	case sec_attr_enc_bytes:
	    if ((fd = open((char *)id->storage_tag, O_WRONLY|O_CREAT|O_TRUNC,
                           0666)) < 0) {
	        *st = dced_s_cant_open_storage_file;
		return;
	    }
	    write(fd, attr->attr_value.tagged_union.bytes->data,
			attr->attr_value.tagged_union.bytes->length);
	    close(fd);
	    break;

	case sec_attr_enc_printstring_array:
	    if (!(fp = fopen((char *)id->storage_tag, "w"))) {
	    *st = dced_s_cant_open_storage_file;
		return;
	    }
	    for (i=0; i<attr->attr_value.tagged_union.string_array->num_strings;
		i++) {
		fputs((char *)attr->attr_value.tagged_union.string_array->strings[i], fp);
		fputc('\n', fp);
	    }
	    fclose(fp);
	    break;
	}
    }

    /* create an ACL for this thing */
    uuid_create(&au, st);		/* uuid of the acl */
    CHECK_STAT_RET(*st);
    /* get initial obj acl */
    dce_db_fetch_by_uuid(acl_db, &hd_do_acl, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_FETCH_IACL_MSG, "hostdata_create",
		(long)*st);
	return;
    }

    dce_db_store_by_uuid(acl_db, &au, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG, "hostdata_create", (long)*st);
	return;
    }

    dce_db_std_header_init(hd_db, &id_d.h, &id->id, &au, NULL, NULL, 0, st);
    CHECK_STAT_RET(*st);
    id_d.h.tagged_union.h.owner_id = Prin;
    id_d.flags = 0;
    if (just_import)
	id_d.flags |= entry_c_data_imported;
    id_d.data = *id;
    dce_db_store_by_uuid(hd_db, &id->id, &id_d, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG, "hostdata_create",
		(long)*st);
	return;
    }

    /* Now, check the known uuids to look for special actions to take */
    if (uuid_equal(&dced_g_uuid_hostdata_post_proc, &id->id, st)) {
	dced_hd_read_post_processors(st);
    }
    hd_post_process(id, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_HD_POSTPROC_FAILED_MSG, "hostdata_create");
    /* Fall through, since item was successfully added.
     * Status will be returned. */

    /* Last, add to cached name->uuid mapping. */
    name_cache_add(hd_name_cache, (char *)id->name, &id->id);
}

/* ______________________________________________________________________ */

/* Delete a host configuration item. */
void
hostdata_delete(
  /* in */
    handle_t		h,
    uuid_t		*id_uuid,
    boolean32		remove_object,
  /* out */
    error_status_t	*st
)
{
    dced_entry_data_t	id_d;
    error_status_t	st2;
    uuid_t		Prin;	/* of caller */

    *st = error_status_ok;

    /* find entry */
    dce_db_fetch(hd_db, id_uuid, &id_d, st);
    CHECK_STAT_RET(*st);
    if (h != NULL && dced_is_remote_disabled) {
	*st = sec_acl_not_authorized;
	return;
    }

    if (remove_object) {
	/* Check ACL for permission */
	if (id_d.flags & entry_c_data_imported) {
	    *st = sec_acl_invalid_permission;
	    return;
	}
	if (!dced_authorized(h, &hd_obj_type,
		&id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_purge, st))
	    return;
	/* remove object (file) itself */
	if (unlink((char *)id_d.data.storage_tag) < 0) {
	    *st = dced_s_cant_remove_storage_file;
	}
    }
    else {
	if (!dced_authorized(h, &hd_obj_type,
		&id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_delete, st))
	    return;
    }

    /* remove acl and entry */
    dce_db_delete_by_uuid(acl_db, &id_d.h.tagged_union.h.acl_uuid, &st2);
    if (st2 != error_status_ok)
	dce_svc_printf(DCED_S_CANT_DELETE_ACL_MSG, "hostdata_delete",
		(long)st2);
    dce_db_delete_by_uuid(hd_db, id_uuid, &st2);
    if (st2 != error_status_ok)
	dce_svc_printf(DCED_S_CANT_DELETE_OBJECT_MSG, "hostdata_delete",
		(long)st2);
    /* delete error more important than unlink? */
    if (st2 != error_status_ok && *st == error_status_ok)
	*st = st2;

    /* Last, delete to cached name->uuid mapping. */
    name_cache_del(hd_name_cache, (char *)id_d.data.name);
}

/* ______________________________________________________________________ */

/* Get info about a single host. */
void
hostdata_get(
  /* in */
    handle_t		h,
    uuid_t		*id_uuid,
    uuid_t		*attr_uuid,
  /* out */
    sec_attr_t		*attr,
    error_status_t	*st
)
{
    dced_entry_data_t	id_d;
    unsigned32		i;
    char		buf[500], *cp, **cv, **cvp;
    idl_byte		*ibp;
    boolean32		binfile;
    sec_attr_enc_str_array_t	*sap;
    uuid_t		Prin;	/* of caller */

    *st = error_status_ok;
    *attr = dced_g_null_sec_attr;
    attr->attr_value.attr_encoding = sec_attr_enc_void;

    /* file filename from database */
    dce_db_fetch_by_uuid(hd_db, id_uuid, &id_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_authorized(h, &hd_obj_type,
	    &id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_read, st))
	return;

    /* Check attr_uuid - should be in schema. We use well-known UUID,
     * since all we support is the two types - plain file and bin file.
     */
    if ( uuid_equal(&dced_g_uuid_fileattr, attr_uuid, st) ) {
	attr->attr_id = dced_g_uuid_fileattr;
	binfile = FALSE;
    }
    else if ( uuid_equal(&dced_g_uuid_binfileattr, attr_uuid, st) ) {
	attr->attr_id = dced_g_uuid_binfileattr;
	binfile = TRUE;
    }
    else {
	*st = dced_s_invalid_attr_type;
	return;
    }

    /* First, check for known UUIDs.  Some have special actions. */
    cp = NULL;
    if (uuid_equal(&dced_g_uuid_host_name, &id_d.data.id, st)) {
	dce_cf_get_host_name(&cp, st);
    }
    else if (uuid_equal(&dced_g_uuid_cell_name, &id_d.data.id, st)) {
	dce_cf_get_cell_name(&cp, st);
    }
    else if (uuid_equal(&dced_g_uuid_cell_aliases, &id_d.data.id, st)) {
	dce_cf_get_cell_aliases(&cv, st);
	if (binfile) {
	    attr->attr_value.attr_encoding = sec_attr_enc_bytes;
	    if (cv == NULL) {		/* no cell aliases? */
		attr->attr_value.tagged_union.bytes =
		    rpc_sm_allocate(sizeof(sec_attr_enc_bytes_t), st);
		CHECK_STAT_RET(*st);
		attr->attr_value.tagged_union.bytes->length  = 0;
		return;
	    }
	    /* This is unusual case (returning these things as binary) */
	    for (i=0,cvp=cv; *cvp; cvp++) {
		/* count bytes - strlen of each, plus spaces between */
		i += strlen(*cvp);
		i++;
	    }
	    attr->attr_value.tagged_union.bytes =
		(sec_attr_enc_bytes_t *)rpc_sm_allocate(i +
			sizeof(sec_attr_enc_bytes_t) + i, st);
	    CHECK_STAT_RET(*st);
	    attr->attr_value.tagged_union.bytes->length = i;
	    ibp = attr->attr_value.tagged_union.bytes->data;
	    for (i = 0, cvp = cv; *cvp; cvp++) {
		for (cp = *cvp; *cp; )
		    *ibp++ = *cp++;
		*ibp++ = ' ';
	    }
	    ibp--;
	    *ibp = '\n';
	    dce_cf_free_cell_aliases(cv, st);
	    return;
	}
	else {
	    char **cvp;
	    attr->attr_value.attr_encoding = sec_attr_enc_printstring_array;
	    if (cv == NULL) {		/* no cell aliases? */
		attr->attr_value.tagged_union.string_array = 
		    rpc_sm_allocate(sizeof(sec_attr_enc_str_array_t), st);
		CHECK_STAT_RET(*st);
		attr->attr_value.tagged_union.string_array->num_strings = 0;
		return;
	    }
	    for (i=0,cvp=cv; *cvp; cvp++) i++;		/* count items */
	    sap = rpc_sm_allocate( sizeof(sec_attr_enc_str_array_t) +
		sizeof(sec_attr_enc_printstring_p_t) * i, st);
	    CHECK_STAT_RET(*st);
	    attr->attr_value.tagged_union.string_array = sap;
	    sap->num_strings = 0;
	    for (i=0; *cv; cv++,i++) {
		sap->strings[i] = rpc_sm_allocate(strlen(*cv)+1, st);
		CHECK_STAT_RET(*st);
		sap->num_strings++;
		strcpy((char *)sap->strings[i], *cv);
	    }
	    dce_cf_free_cell_aliases(cv, st);
	    return;
	}
    }
    /* if a host or cell name, return a list of length 1. */
    if (cp) {
	if (binfile) {
	    /* This is unusual case (returning these things as binary) */
	    i = strlen(cp) + 1;
	    attr->attr_value.attr_encoding = sec_attr_enc_bytes;
	    attr->attr_value.tagged_union.bytes =
		(sec_attr_enc_bytes_t *)rpc_sm_allocate(i +
			sizeof(sec_attr_enc_bytes_t), st);
	    attr->attr_value.tagged_union.bytes->length = i;
	    cp[i-1] = '\n';
	    memcpy((char *)attr->attr_value.tagged_union.bytes->data, cp, i);
	    free(cp);
	    return;
	}
	else {
	    attr->attr_value.attr_encoding = sec_attr_enc_printstring_array;
	    sap = rpc_sm_allocate(sizeof(sec_attr_enc_str_array_t), st);
	    CHECK_STAT_RET(*st);
	    attr->attr_value.tagged_union.string_array = sap;
	    sap->num_strings = 1;
	    sap->strings[0] = rpc_sm_allocate(strlen(cp)+1, st);
	    CHECK_STAT_RET(*st);
	    strcpy((char *)sap->strings[0], cp);
	    free(cp);
	    return;
	}
    }

    if (!binfile) {	/* array of strings */
	FILE		*fp;
	if (!(fp=fopen((char *)id_d.data.storage_tag, "r"))) {
	    *st = dced_s_cant_open_storage_file;
	    return;
	}
	/* count lines in file so we know how much to allocate */
	i = 0;
	while ( fgets(buf, 500, fp) ) {
	    i++;
	}
	rewind(fp);
	sap = rpc_sm_allocate( (sizeof(sec_attr_enc_str_array_t) +
			sizeof(sec_attr_enc_printstring_p_t) * i), st);
	CHECK_STAT_RET(*st);
	attr->attr_value.tagged_union.string_array = sap;
	i = 0;
	while ( fgets(buf, 500, fp) ) {
	    if ((cp = strchr(buf, '\n'))) *cp = 0;	/* kill newline */
	    sap->strings[i] = rpc_sm_allocate(strlen(buf)+1, st);
	    CHECK_STAT_RET(*st);
	    strcpy((char *)sap->strings[i], buf);
	    i++;
	}
	attr->attr_value.tagged_union.string_array->num_strings = i;
	attr->attr_value.attr_encoding = sec_attr_enc_printstring_array;
	fclose(fp);
    }
    else {	/* array of bytes */
	struct stat	statbuf;
	int		fd;
	if ((fd=open((char *)id_d.data.storage_tag, O_RDONLY)) < 0) {
	    *st = dced_s_cant_open_storage_file;
	    return;
	}
	if (fstat(fd, &statbuf) != 0) {
	    *st = dced_s_cant_open_storage_file;
	    return;
	}
	attr->attr_value.tagged_union.bytes =
		(sec_attr_enc_bytes_t *)rpc_sm_allocate(statbuf.st_size +
			sizeof(sec_attr_enc_bytes_t), st);
	attr->attr_value.tagged_union.bytes->length = statbuf.st_size;
	if (read(fd, attr->attr_value.tagged_union.bytes->data,
		statbuf.st_size) < statbuf.st_size) {
	    *st = dced_s_cant_open_storage_file;
	    return;
	}
	attr->attr_value.attr_encoding = sec_attr_enc_bytes;
	close(fd);
    }
}

/* ______________________________________________________________________ */

/* Set info about a single host. */
void
hostdata_set(
  /* in */
    handle_t		h,
    uuid_t		*id_uuid,
    sec_attr_t		*attr,
  /* out */
    error_status_t	*st
)
{
    dced_entry_data_t	id_d;
    FILE		*fp;
    unsigned32		i;
    uuid_t		Prin;	/* of caller */

    *st = error_status_ok;

    /* file filename from database */
    dce_db_fetch_by_uuid(hd_db, id_uuid, &id_d, st);
    CHECK_STAT_RET(*st);

    if (h != NULL && dced_is_remote_disabled) {
	*st = sec_acl_not_authorized;
	return;
    }
    if (!dced_authorized(h, &hd_obj_type,
	    &id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_write, st))
	return;
    /* write attr to file */
    if (!(fp=fopen((char *)id_d.data.storage_tag, "w"))) {
	    *st = dced_s_cant_open_storage_file;
	return;
    }
    switch (attr->attr_value.attr_encoding) {
    default:
	*st = dced_s_unknown_attr_type;
	return;
    case sec_attr_enc_printstring:
	fputs((char *)attr->attr_value.tagged_union.printstring, fp);
	fputc('\n', fp);
	break;
    case sec_attr_enc_printstring_array:
	for (i=0; i<attr->attr_value.tagged_union.string_array->num_strings; i++) {
	    fputs((char *)attr->attr_value.tagged_union.string_array->strings[i], fp);
	    fputc('\n', fp);
	}
	break;
    case sec_attr_enc_bytes:
	fwrite(attr->attr_value.tagged_union.bytes->data,
		attr->attr_value.tagged_union.bytes->length, (size_t)1, fp);
	break;
    }

    fclose(fp);

    /* Now, check the known uuids to look for special actions to take */
    if (uuid_equal(&dced_g_uuid_hostdata_post_proc, id_uuid, st)) {
	dced_hd_read_post_processors(st);
    }
    hd_post_process(&id_d.data, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_HD_POSTPROC_FAILED_MSG, "hostdata_set");
}

/* ______________________________________________________________________ */

/* Retrieve the list of host configuration items. */
void
hostdata_get_list(
  /* in */
    handle_t		h,
  /* in,out */
    dced_entry_list_t	*items,
  /* out */
    error_status_t	*st
)
{
    void		*key;
    error_status_t	st2;
    unsigned32		n;
    dced_entry_data_t	id_d;
    uuid_t		Prin;	/* of caller */

    *st = error_status_ok;
    items->count = 0;

    if (!dced_authorized(h, &hd_cont_type, &hd_acl, &Prin, dced_perm_read, st))
	return;

    items->count = 0;
    dce_db_inq_count(hd_db, &n, st);
    CHECK_STAT_RET(*st);
    items->list  = rpc_sm_allocate(n * sizeof(dced_entry_t), st);
    CHECK_STAT_RET(*st);

    dce_db_iter_start(hd_db, st);
    CHECK_STAT_RET(*st);
    do {
	dce_db_iter_next(hd_db, &key, st);
	if (*st == db_s_no_more) {
	    *st = error_status_ok;
	    break;
	}
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_ITER_FAIL_MSG, "hostdata_get_list",
		(long)*st);
	    dce_db_iter_done(hd_db, &st2);
	    return;
	}
	dce_db_fetch(hd_db, key, &id_d, st);
	if (*st != error_status_ok) {
	    dce_db_iter_done(hd_db, &st2);
	    return;
	}
	items->list[items->count] = id_d.data;
	items->count++;
    } while (TRUE);
    dce_db_iter_done(hd_db, st);
}

/* ______________________________________________________________________ */

hostdata_v1_0_epv_t dced_hostdata_v1_0_epv = {
    hostdata_get_list,
    hostdata_create,
    hostdata_delete,
    hostdata_get,
    hostdata_set
};
