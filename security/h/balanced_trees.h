/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: balanced_trees.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:47  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:27  root
 * 	Submit
 * 	[1995/12/11  15:14:12  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:06:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:13  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  balanced_trees.h V=6 04/29/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - balanced tree manager
 *
 * 
 *
 */

#ifndef __balanced_trees_included__
#define __balanced_trees_included__

#include <dce/nbase.h>


typedef struct b_datum {
    char    *dptr;
    int     dsize;
} b_datum;

typedef struct tree_root * tree_handle;


boolean32 insert_data (
#ifdef __STDC__
    boolean32   replace_ok,
    tree_handle *tree,
    b_datum     *new_key,
    b_datum     *new_data
#endif
);

b_datum * fetch_data (
#ifdef __STDC__
    tree_handle tree,
    b_datum     *search_key
#endif
);

b_datum * fetch_next (
#ifdef __STDC__
    tree_handle tree,
    b_datum     *search_key,
    b_datum     **next_key
#endif
);

boolean32 delete_data (
#ifdef __STDC__
    tree_handle tree,
    b_datum     *search_key
#endif
);

#ifdef DEBUG_BTREE
void print_tree(
#ifdef __STDC__
    tree_handle tree
#endif
);
#endif /* DEBUG_BTREE */  

int save_database ( 
#ifdef __STDC__
    char        *fname,
    tree_handle db
#endif
);

void clear_database ( 
#ifdef __STDC__
    tree_handle *db
#endif
);  

void input_database ( 
#ifdef __STDC__
    char        *fname,
    tree_handle *db
#endif
);


#endif /* __balanced_trees_included__ */

