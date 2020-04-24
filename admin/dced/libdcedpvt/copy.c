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
 * $Log: copy.c,v $
 * Revision 1.1.2.2  1996/02/18  19:16:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:05  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:10  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:39 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/12/08  20:48:50  root]
 * 
 * Revision 1.1.2.5  1994/08/24  19:51:52  bowe
 * 	Merged with changes from 1.1.2.4
 * 	[1994/08/24  19:51:33  bowe]
 * 
 * 	Made string_copy() non-staic. [CR 11791]
 * 	[1994/08/24  19:39:23  bowe]
 * 
 * Revision 1.1.2.4  1994/08/23  19:24:05  rsalz
 * 	Add entryname field to server_t (OT CR 11396).
 * 	[1994/08/23  19:18:57  rsalz]
 * 
 * Revision 1.1.2.3  1994/08/17  13:57:52  bowe
 * 	Pass proper args to sec_attr_util_alloc_copy() [CR 11728]
 * 	[1994/08/17  13:57:42  bowe]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:25  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:15:43  annie]
 * 
 * 	expand copyright with OSF copyright text
 * 
 * 	expand copyright with OSF copyright text
 * 
 * Revision 1.1.2.1  1994/07/29  19:56:03  rsalz
 * 	Add copy.c, split out from misc.c
 * 	[1994/07/29  19:36:25  rsalz]
 * 
 * $EndLog$
 */
#include <libimpl.h>
#include <dce/sec_attr_util.h>


void *
dced_copy_via_malloc(
    idl_size_t		size
)
{
    return malloc(size);
}


void *
dced_copy_for_stub(
    idl_size_t		size
)
{
    error_status_t	st;
    void		*p;

    p = rpc_sm_allocate(size, &st);
    if (st != error_status_ok)
	return NULL;
    return p;
}


idl_char *
strdup_for_client(
    idl_char		*p,
    error_status_t	*st
)
{
    idl_char		*new;

    new = rpc_sm_allocate(strlen((char *)p) + 1, st);
    if (*st != error_status_ok)
	return NULL;
    return (idl_char *)strcpy((char *)new, (char *)p);
}


void
string_copy(
    dced_string_t	*new,
    dced_string_t	*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    if (*old == NULL)
	*new = NULL;
    else {
	*new = (*alloc)(strlen((char *)*old) + 1);
	strcpy((char *)*new, (char *)*old);
    }
    *st = error_status_ok;
}


void
string_list_copy(
    dced_string_list_t	*new,
    dced_string_list_t	*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    unsigned32		i;

    if ((i = new->count = old->count) != 0) {
	for (new->list = (*alloc)(i * sizeof *new->list); i; ) {
	    i--;
	    string_copy(&new->list[i], &old->list[i], alloc, st);
	}
    }
}

static void
uuid_list_copy(
    uuid_list_t		*new,
    uuid_list_t		*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    unsigned32		i;

    *st = error_status_ok;
    if ((i = new->count = old->count) != 0) {
	new->list = (*alloc)(i * sizeof *new->list);
	memcpy(new->list, old->list, i * sizeof *new->list);
    }
}

void
fixedattr_copy(
    server_fixedattr_t	*new,
    server_fixedattr_t	*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    *st = error_status_ok;
    *new = *old;
    string_copy(&new->program, &old->program, alloc, st);
    string_list_copy(&new->arguments, &old->arguments, alloc, st);
    uuid_list_copy(&new->prerequisites, &old->prerequisites, alloc, st);
    uuid_list_copy(&new->keytables, &old->keytables, alloc, st);
    string_copy(&new->posix_dir, &old->posix_dir, alloc, st);
}

void
attr_list_copy(
    dced_attr_list_t	*new,
    dced_attr_list_t	*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    unsigned32		i;

    *st = error_status_ok;
    if ((i = new->count = old->count) != 0) {
	for (new->list = (*alloc)(i * sizeof *new->list); i; ) {
	    i--;
	    sec_attr_util_alloc_copy(alloc, &old->list[i], &new->list[i], st);
	}
    }
}


static void
opnum_list_copy(
    dced_opnum_list_t	*new,
    dced_opnum_list_t	*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    unsigned32		i;

    *st = error_status_ok;
    if ((i = new->count = old->count) != 0) {
	new->list = (*alloc)(i * sizeof *new->list);
	memcpy(new->list, old->list, i * sizeof *new->list);
    }
}

void
dced_tower_copy(
    twr_t		**new,
    twr_t		*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    *st = error_status_ok;
    *new = (*alloc)(sizeof (twr_t)
			+ (old->tower_length - 1) * sizeof (byte));
    (*new)->tower_length = old->tower_length;
    memcpy((*new)->tower_octet_string, old->tower_octet_string,
	old->tower_length);
}

static void
twr_set_copy(
    sec_attr_twr_set_p_t	*new,
    sec_attr_twr_set_p_t	old,
    dced_alloc_func_t		alloc,
    error_status_t		*st
)
{
    unsigned32			i;

    *new = (*alloc)(sizeof (sec_attr_twr_set_t)
	+ (old->count - 1) * sizeof (sec_attr_twr_ref_t));
    (*new)->count = old->count;
    for (i = old->count; i; ) {
	i--;
	dced_tower_copy(&(*new)->towers[i], old->towers[i], alloc, st);
    }
}

void
tower_list_copy(
    dced_tower_list_t	*new,
    dced_tower_list_t	*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    unsigned32		i;

    *st = error_status_ok;
    if ((i = new->count = old->count) != 0) {
	for (new->list = (*alloc)(sizeof *new->list); i; ) {
	    i--;
	    new->list[i] = (*alloc)(sizeof *new->list[i]);
	    twr_set_copy(&new->list[i], old->list[i], alloc, st);
	}
    }
}


static void
service_copy(
    service_t		*new,
    service_t		*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    *st = error_status_ok;

    *new = *old;
    string_copy(&new->ifname, &old->ifname, alloc, st);
    string_copy(&new->annotation, &old->annotation, alloc, st);
    string_copy(&new->entryname, &old->entryname, alloc, st);
    uuid_list_copy(&new->objects, &old->objects, alloc, st);
    opnum_list_copy(&new->operations, &old->operations, alloc, st);
    tower_list_copy(&new->towers, &old->towers, alloc, st);
}


static void
service_list_copy(
    service_list_t	*new,
    service_list_t	*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    unsigned32		i;

    *st = error_status_ok;

    if ((i = new->count = old->count) != 0) {
	for (new->list = (*alloc)(i * sizeof *new->list); i; ) {
	    i--;
	    service_copy(&new->list[i], &old->list[i], alloc, st);
	}
    }
}


void
server_copy(
    server_t		*new,
    server_t		*old,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    *st = error_status_ok;

    *new = *old;
    string_copy(&new->name, &old->name, alloc, st);
    string_copy(&new->entryname, &old->entryname, alloc, st);
    service_list_copy(&new->services, &old->services, alloc, st);
    fixedattr_copy(&new->fixed, &old->fixed, alloc, st);
    attr_list_copy(&new->attributes, &old->attributes, alloc, st);
    string_list_copy(&new->prin_names, &old->prin_names, alloc, st);
}


void
string_list_append(
    dced_string_list_t	*base,
    dced_string_list_t	*adds,
    dced_alloc_func_t	alloc,
    error_status_t	*st
)
{
    dced_string_t	*new;
    unsigned32		i;

    dce_assert(dhd_svc_handle,
	alloc == dced_copy_via_malloc || alloc == dced_copy_for_stub);
    new = (*alloc)((base->count + adds->count) * sizeof *new);
    memcpy(new, base->list, base->count * sizeof *new);
    for (i = 0; i < adds->count; i++)
	string_copy(&new[base->count++], &adds->list[i], alloc, st);
    if (alloc == dced_copy_via_malloc)
	free(base->list);
    base->list = new;
}

