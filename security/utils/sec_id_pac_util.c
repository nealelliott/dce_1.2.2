/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_id_pac_util.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:12  root]
 * 
 * Revision 1.1.2.3  1993/02/05  22:00:50  sommerfeld
 * 	[OT 7133] Protect better against null pointer dereference in sec_id_pac_free.
 * 	[1993/02/05  21:33:09  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/29  16:41:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:32:17  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sec_id_pac_util.c V=1 09/26/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *
 */
 
#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)sec_id_pac_util.c	1 - 09/26/91";
#endif

#include <un_maloc.h>
#include <u_str.h>
    
#include <sec_id_pac_util.h>

#include <dce/lbase.h>
#include <dce/secsts.h>


/* s e c _ id _ p a c _ u t i l _ f r e e
 * 
 * Release dynamic storage associated with a PAC.
 */
void sec_id_pac_util_free
#ifndef __STDC__
    ( p ) 
    sec_id_pac_t *p;
#else
  (
    sec_id_pac_t *p
  )
#endif
{
    int i;

    if (p->realm.name) {
        u_free(p->realm.name);
        p->realm.name = NULL;
    }
    if (p->principal.name) {
        u_free(p->principal.name);
        p->principal.name = NULL;
    }
    if (p->group.name) {
        u_free(p->group.name);
        p->group.name = NULL;
    }

    if (p->groups) {
	for (i = 0; i < p->num_groups; i++) {
	    if (p->groups[i].name) {
		u_free(p->groups[i].name);
		p->groups[i].name = NULL;
	    }
	}
	p->num_groups = 0;
        u_free(p->groups);
        p->groups = NULL;
    }

    if (p->foreign_groups) {
	for (i = 0; i < p->num_foreign_groups; i++) {
	    if (p->foreign_groups[i].id.name) {
		u_free(p->foreign_groups[i].id.name);
		p->foreign_groups[i].id.name = NULL;
	    }
	    if (p->foreign_groups[i].realm.name) {
		u_free(p->foreign_groups[i].realm.name);
		p->foreign_groups[i].realm.name = NULL;
	    }
	}
	p->num_foreign_groups = 0;
        u_free(p->foreign_groups);
        p->foreign_groups = NULL;
    }
}


/*
 * sec_id_pac_util_copy - copy a pac, allocating storage for subunits.
 */

error_status_t sec_id_pac_util_copy (to, from)
    sec_id_pac_t *to;
    sec_id_pac_t *from;
{
    int i;
    int ngrp;
    
#define CLEAR_ID(idp) ((idp)->name = NULL)

#define COPY_ID(to__, from__) { \
    sec_id_t *to_ = (to__), *from_ = (from__); \
    to_->uuid = from_->uuid; \
    if (from_->name == NULL) { \
        to_->name = NULL; \
    } else { \
        int len = strlen((char *)(from_->name)) + 1; \
        to_->name = (unsigned char *) malloc (len); \
        if (to_->name == NULL) goto nomem; \
        memcpy(to_->name, from_->name, len); \
        } \
    }
        
    /*
     * partially initialize destination pac, so we can call
     * sec_id_pac_util_free at any point from now on.
     */
    
    CLEAR_ID(&to->realm);
    CLEAR_ID(&to->principal);
    CLEAR_ID(&to->group);
    to->groups = NULL;
    to->foreign_groups = NULL;

    /*
     * Allocate and copy subfields
     */
    to->pac_type = from->pac_type;
    to->authenticated = from->authenticated;
    to->num_groups = from->num_groups;
    to->num_foreign_groups = from->num_foreign_groups;

    COPY_ID (&to->realm, &from->realm);
    COPY_ID (&to->principal, &from->principal);
    COPY_ID (&to->group, &from->group);

    ngrp = to->num_groups;
    if (ngrp == 0) {
        to->groups = NULL;
    }
    else {
        to->groups = (sec_id_t *)malloc(sizeof(sec_id_t) * ngrp);
        if (to->groups == NULL)
            goto nomem;

        for (i=0; i<ngrp; i++) {
            CLEAR_ID(&to->groups[i]);
        }

        for (i=0; i<ngrp; i++) {
            COPY_ID(&to->groups[i], &from->groups[i]);
        }
    }
    ngrp = to->num_foreign_groups;

    if (ngrp == 0) {
        to->foreign_groups = NULL;
    } else {
        to->foreign_groups =
            (sec_id_foreign_t *)malloc(sizeof(sec_id_foreign_t) * ngrp);
        if (to->foreign_groups == NULL)
            goto nomem;

        for (i=0; i<ngrp; i++) {
            CLEAR_ID(&to->foreign_groups[i].id);
            CLEAR_ID(&to->foreign_groups[i].realm);
        }
        for (i=0; i<ngrp; i++) {
            COPY_ID(&to->foreign_groups[i].id, &from->foreign_groups[i].id);
            COPY_ID(&to->foreign_groups[i].realm, &from->foreign_groups[i].realm);
        }
    }
    return error_status_ok;

nomem:
    sec_id_pac_util_free(to);
    return sec_rgy_cant_allocate_memory;

#undef CLEAR_ID
#undef COPY_ID
    
}
