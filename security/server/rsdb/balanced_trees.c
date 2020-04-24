/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: balanced_trees.c,v $
 * Revision 1.1.66.1  1996/10/03  15:11:40  arvind
 * 	fix purify warning when reading in database.
 * 	[1996/09/16  23:18 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.64.2  1996/02/18  00:20:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:36  marty]
 * 
 * Revision 1.1.64.1  1995/12/08  17:57:55  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/12  18:44 UTC  burati
 * 	Merge from mb_mothra4
 * 
 * 	HP revision /main/mb_mothra4/4  1995/06/26  20:13 UTC  burati
 * 	Final cleanup
 * 
 * 	HP revision /main/mb_mothra4/3  1995/06/12  23:52 UTC  burati
 * 	More improvements to memory management
 * 
 * 	HP revision /main/mb_mothra4/2  1995/06/12  17:51 UTC  burati
 * 	Fix memory usage (merge Jim's 2-3tree deletion alg from glbd's symtab.c
 * 	and add memory management code).
 * 
 * 	HP revision /main/mb_mothra4/1  1995/05/26  17:20 UTC  burati
 * 	Prototype and general cleanup before memory work
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/09  10:37 UTC  sekhar
 * 	Merge to mothra.
 * 
 * 	HP revision /main/sekhar_mothra_fix/1  1995/06/01  12:31 UTC  sekhar
 * 	[ CHFts12106 ] Replaced SVC code sec_sys_errno_text with sec_sys_errno_file_text.
 * 	[1995/12/08  17:21:53  root]
 * 
 * Revision 1.1.61.3  1994/08/10  18:45:40  mdf
 * 	hasty merge's get you every time.
 * 	[1994/08/09  17:43:58  mdf]
 * 
 * 	Another fix for CR11609
 * 	[1994/08/09  17:41:35  mdf]
 * 
 * 	CR11609 Put back the 64 bit changes.
 * 	[1994/08/09  16:56:20  mdf]
 * 
 * Revision 1.1.61.2  1994/08/04  16:14:10  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:45  mdf]
 * 
 * Revision 1.1.61.1  1994/03/31  19:56:58  hanfei
 * 	work for ERA entries in policy.
 * 	[1994/03/31  19:50:48  hanfei]
 * 
 * Revision 1.1.59.1  1993/10/07  13:12:24  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/07  13:08:20  mccann]
 * 
 * Revision 1.1.4.4  1993/01/07  16:18:36  burati
 * 	CR6518 Need to check status of write()'s when saving database
 * 	[1993/01/06  19:10:06  burati]
 * 
 * Revision 1.1.4.3  1992/12/29  16:38:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:35  zeliff]
 * 
 * Revision 1.1.4.2  1992/09/29  20:13:26  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:38:43  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  balanced_trees.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1993, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Registry Server - balanced tree manager
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)balanced_trees.c	11 - 04/29/91";
#endif

/*
 * Roughly modeled on the algorithms in Aho Hopcroft and Ullman.
 * - modified so that 3 node splitting is handled by the recursive
 *   implementation of search_insert
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <un_stat.h>
#include <bstring.h>
#include <un_io.h>

#include <dce/dce.h>
#include <balanced_trees.h>
#include <rs_base.h>
#include <rs_util.h>
#include <rs_task.h>
#include <rsdb_file.h>

#include <sec_svc.h>

#define FULLWORD_SIZE sizeof(long)     /* Must be a power of 2 */

/*
 * Private types for trees
 */

typedef struct data_node {
    int     size;
    b_datum key;
    b_datum data;
} data_node, *data_handle;

typedef struct tree_node *node_handle;
typedef struct tree_node {
    node_handle lson;
    node_handle mson;
    node_handle rson;
    node_handle parent;
    data_handle lkey;
    data_handle mkey;
} tree_node;

struct tree_root {
    node_handle     tree;
};


/*
 * Static info used by update and node_allocation routines.
 */
typedef struct task_data {
    node_handle implant;
    b_datum     *key;
    b_datum     *data;
    tree_handle root;
    boolean32   insert;
    boolean32   replace;
} task_data;

typedef enum { IMPLANT, NOT_FOUND, REPLACED, DUPLICATE } result_t;

#define IS_3NODE(P) ((P)->rson != NULL)
#define IS_LEAF(P)  ((P)->lson == NULL)
#define IS_LSON(P)  ((P)->parent && ((P)->parent->lson == (P)))
#define IS_MSON(P)  ((P)->parent && ((P)->parent->mson == (P)))

#ifdef MIN
#undef MIN
#endif
#define MIN(A,B) ((A) < (B) ? (A) : (B))

typedef void (*node_print_f_t) (
    data_handle  mkey,
    char         *string,
    boolean32    bool
);

PRIVATE void propagate_key ( node_handle tree );
PRIVATE void output_database ( node_handle tree );
#ifdef DEBUG_BTREE
void print_tree ( node_handle tree );
#endif

/*
 * Memory Allocation Rountines
 */

PRIVATE tree_handle mem_alloc_db ( void )
{
    tree_handle db;

    db = (tree_handle) malloc(sizeof(*db));
    memset(db, 0, sizeof(*db));

    return db;
}


PRIVATE char * mem_alloc (
    tree_handle db,
    int         size,
    boolean32   roundup
  )
{
#define ALLOC_BLKSIZE (16*1024) /* Must be a power of 2 */
    char            *ret_ptr = NULL;

    /*
     * Round size up to a fullword boundary
     */
    size = (size + (FULLWORD_SIZE - 1)) & ~(FULLWORD_SIZE - 1);

    /* Round up initial block to a 16K boundary */
    if (roundup)
        size = (size + (ALLOC_BLKSIZE - 1)) & ~(ALLOC_BLKSIZE - 1);

    /* Allocate the memory; Fatal error if that fails */
    ret_ptr = malloc(size);
    if (ret_ptr == NULL) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_s_no_memory);
    }

    return ret_ptr;
}


PRIVATE void mem_tree_free (
    node_handle tree
  )
{
    if (tree == NULL)
	return;

    if (IS_LEAF(tree)) {
        if (tree->mkey != tree->lkey)
            free(tree->mkey);
        free(tree->lkey);
        return;
    }

    if (tree->lson) {
        mem_tree_free(tree->lson);
        free(tree->lson);
    }
    if (tree->mson) {
        mem_tree_free(tree->mson);
        free(tree->mson);
    }
    if (tree->rson) {
        mem_tree_free(tree->rson);
        free(tree->rson);
    }
}


/*
 * Free all memory used by the specified database
 */
PRIVATE void mem_db_free (
    tree_handle db
  )
{
    if ((db != NULL) && (db->tree != NULL)) {
        mem_tree_free(db->tree);
        db->tree = NULL;
    }
}


PRIVATE boolean32 key_less_equal (
    b_datum *k1,
    b_datum *k2
  )
{
    int result = -1;    /* An empty key is less than anything */
    unsigned char *p1;
    unsigned char *p2;
    int len;

    len = MIN(k1->dsize, k2->dsize);

    p1 = (unsigned char *) k1->dptr;
    p2 = (unsigned char *) k2->dptr;
    while (len--) {
        result = *p1++ - *p2++;
        if (result != 0)
            break;
    }

    if (result < 0)
        return true;
    if (result == 0) {
        if (k1->dsize <= k2->dsize)
            return true;
    }
    return false;
}


PRIVATE boolean32 key_less (
    b_datum *k1,
    b_datum *k2
  )
{
    int result = -1;    /* An empty key is less than anything */
    unsigned char *p1;
    unsigned char *p2;
    int  len;

    len = MIN(k1->dsize, k2->dsize);

    p1 = (unsigned char *) k1->dptr;
    p2 = (unsigned char *) k2->dptr;
    while (len--) {
        result = *p1++ - *p2++;
        if (result != 0)
            break;
    }

    if (result == 0) {
        if (k1->dsize < k2->dsize) {
            return true;
        } else {
            return false;
        }
    } else if (result > 0) {
        return false;
    }

    return true;
}


PRIVATE boolean32 key_equal (
    b_datum *k1,
    b_datum *k2
  )
{
    int result;

    if (k1->dsize != k2->dsize)
        return false;

    result = memcmp(k1->dptr, k2->dptr, k1->dsize);
    if (result != 0)
        return false;

    return true;
}


PRIVATE void store_new_data (
    data_handle new,
    b_datum     *key,
    b_datum     *data
  )
{
    new->key.dsize = key->dsize;
    new->key.dptr  = (char *) (new + 1);
    bcopy(key->dptr, new->key.dptr, new->key.dsize);
    new->data.dsize = data->dsize;
    new->data.dptr  = new->key.dptr + new->key.dsize;
    bcopy(data->dptr, new->data.dptr, new->data.dsize);
}


PRIVATE data_handle new_data_node (
    b_datum     *key,
    b_datum     *data,
    task_data   *td
  )
{
    data_handle new;
    int         size;

    size = sizeof(data_node) + key->dsize + data->dsize;
    new = (data_handle) mem_alloc(td->root, size, false);
    if (new == NULL) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_s_no_memory);
    }

    new->size = size;
    store_new_data(new, key, data);

    return new;
}


PRIVATE data_handle replace_data_node (
    data_handle old_data,
    b_datum     *key,
    b_datum     *data,
    task_data   *td,
    boolean32   *pk
  )
{
    int size;

    size = sizeof(data_node) + key->dsize + data->dsize;
    if (size <= old_data->size) {               /* if we can, */
        *pk = false;
        store_new_data(old_data, key, data); 	/* reuse old storage */
        return old_data;
    } else {
        *pk = true;				/* need to propagate new key */
        size = sizeof(data_node) + old_data->key.dsize + old_data->data.dsize;
        free((char *)old_data);                 /* else free old storage */
	return new_data_node(key, data, td);        /* and alloc new storage */
    }
}


PRIVATE node_handle new_tree_node (
    node_handle lson,
    node_handle mson,
    boolean32   add_data,
    task_data   *td
  )
{
    node_handle implant;

    implant = (node_handle) mem_alloc(td->root, sizeof(tree_node), false);
    implant->parent = NULL;
    implant->rson = NULL;
    implant->lson = lson;
    implant->mson = mson;
    if (add_data) {
        implant->mkey = implant->lkey = new_data_node(td->key, td->data, td);
    } else {
        implant->mkey = implant->lkey = NULL;
    }
    return implant;
}


/*
 * update is NOT reentrant for multi-tasking.  It must be called with an
 *  appropriate mutual exclusion lock in place.
 */
PRIVATE result_t update (
    node_handle tree,
    task_data   *td
  )
{
    result_t    retval;
    node_handle split_node;
    node_handle t;
    enum {left, middle, right} branch;
    boolean32   pk;

    if (tree == NULL) {
        if (td->insert) {
            td->implant = new_tree_node((node_handle) NULL, (node_handle) NULL,
					true, td);
            return IMPLANT;
        } else {
            return NOT_FOUND;
        }
    }

    /* If we've reached a leaf, it either matches the search key or we must
     * implant a new node alongside it in its parent.
     */
    if (IS_LEAF(tree)) {
        if (key_equal(td->key, &(tree->lkey->key))) {	/* Same key? */
            td->implant = tree;
            if (td->replace) {	        /* replace ok? replace data */
                tree->lkey = tree->mkey
                    = replace_data_node(tree->lkey, td->key, td->data, td,&pk);
		if (pk)
		    propagate_key(tree);
                return REPLACED;
            } else {
                return DUPLICATE;
            }
        } else {
            if (td->insert) {	/* no match: implant new node in parent */
                td->implant = new_tree_node((node_handle) NULL,
					    (node_handle) NULL, true, td);
                return IMPLANT;
            } else {            /* unless search-only */
                return NOT_FOUND;
            }
        }
    } else {	/* not a leaf; must process recursively in one of our sons */
        if (key_less_equal(td->key, &(tree->lkey->key))) {
            branch = left;
            retval = update(tree->lson,td);
        } else if (!IS_3NODE(tree)
                        || key_less_equal(td->key, &(tree->mkey->key))) {
            branch = middle;
            retval = update(tree->mson,td);
        } else {
            branch = right;
            retval = update(tree->rson,td);
        }

        /* The recursive invocation may have generated a NOT_FOUND or DUPLICATE
         * status, or if we're adding a new node, it will indicate IMPLANT
         * meaning we should implant td->implant in our node.  If we only
         * have two sons so far we can just add this in the appropriate spot.
         * If however, we already have 3, we must split into two nodes, 
         * distributing our original 3 plus the new one among them.
         */
        if (retval == IMPLANT) {
            if (IS_3NODE(tree)) {
                split_node = new_tree_node( (node_handle) NULL,
					   (node_handle) NULL, false, td);

                if (key_less_equal(&(td->implant->mkey->key),
                                            &(tree->lson->mkey->key)))
                    branch = left;
                else if (key_less_equal(&(td->implant->mkey->key),
                                            &(tree->mson->mkey->key)))
                    branch = middle;
                else
                    branch = right;

                /* If implant would go in our left or middle branch pass off
		 * our 2 rightmost branches to the new node, and make the
		 * implant our left/right son as appropriate.  Otherwise we
		 * give the new node our rightmost branch and the implant.
                 */
                if (branch == left || branch == middle) {
                    split_node->lson = tree->mson;
                    split_node->mson = tree->rson;

                    if (branch == left) {
                        tree->mson = tree->lson;
                        tree->lson = td->implant;
                    } else {
                        tree->mson = td->implant;
                    }
                } else {
                    if (key_less_equal(&(tree->rson->mkey->key),
                                            &(td->implant->mkey->key))) {
                        split_node->lson = tree->rson;
                        split_node->mson = td->implant;
                    } else {
                        split_node->lson = td->implant;
                        split_node->mson = tree->rson;
                    }
                }
                split_node->lson->parent = split_node->mson->parent =
                    split_node;
                tree->lson->parent = tree->mson->parent = tree;

                /* Recalculate maximal keys for both branches of both nodes. */
                for (t = split_node->lson; IS_3NODE(t); t = t->rson);
                split_node->lkey = t->mkey;
                for (t = split_node->mson; IS_3NODE(t); t = t->rson);
                split_node->mkey = t->mkey;

                for (t = tree->lson; IS_3NODE(t); t = t->rson);
                tree->lkey = t->mkey;
                for (t = tree->mson; IS_3NODE(t); t = t->rson);
                tree->mkey = t->mkey;

                tree->rson = NULL;
                td->implant = split_node;
                return IMPLANT;

            } else {	/* no split needed: put in this node as appropriate */
                td->implant->parent = tree;
                if (branch == left) {
                    tree->rson = tree->mson;
                    if (key_less_equal(&(tree->lson->mkey->key),
                                            &(td->implant->mkey->key))) {
                        tree->mson = td->implant;
                    } else {
                        tree->mson = tree->lson;
                        tree->lson = td->implant;
                    }
                    for (t = tree->lson; IS_3NODE(t); t = t->rson);
                    tree->lkey = t->mkey;
                    for (t = tree->mson; IS_3NODE(t); t = t->rson);
                    tree->mkey = t->mkey;
                } else if (branch == middle) {
                    if (key_less_equal(&(tree->mson->mkey->key),
                                            &(td->implant->mkey->key))) {
                        tree->rson = td->implant;
                    } else {
                        tree->rson = tree->mson;
                        tree->mson = td->implant;
                    }
                    for (t = tree->mson; IS_3NODE(t); t = t->rson);
                    tree->mkey = t->mkey;
                } else {
                    tree->rson = td->implant;
                }
                return REPLACED;
            }
        }
    }

    return retval;
} /* update */


PRIVATE result_t search_next (
    node_handle root,
    b_datum     *key,
    node_handle *result_node
  )
{
    result_t    retval;
    node_handle tree;

    if (root == NULL) {
        return NOT_FOUND;
    }

    while (true) {
        tree = root;

        while (!IS_LEAF(tree)) {
            if (key_less(key, &(tree->lkey->key))) {
                tree = tree->lson;
            } else if (!IS_3NODE(tree) || key_less(key, &(tree->mkey->key))) {
                tree = tree->mson;
            } else {
                tree = tree->rson;
            }
        }

        if (key_less(key, &(tree->lkey->key))) {
            *result_node = tree;
            return DUPLICATE;
        } else {
            return NOT_FOUND;
        }
    }

    /* NOTREACHED */
} /* search_next */


PRIVATE result_t search (
    node_handle tree,
    b_datum     *key,
    node_handle *result_node
  )
{
    result_t    retval;

    if (tree == NULL) {
        return NOT_FOUND;
    }

    while (!IS_LEAF(tree)) {
        if (key_less_equal(key, &(tree->lkey->key))) {
            tree = tree->lson;
        } else if (!IS_3NODE(tree) || key_less_equal(key, &(tree->mkey->key))) {
            tree = tree->mson;
        } else {
            tree = tree->rson;
        }
    }

    if (key_equal(key, &(tree->lkey->key))) {
        *result_node = tree;
        return DUPLICATE;
    } else {
        return NOT_FOUND;
    }

    /* NOTREACHED */
} /* search */


PUBLIC boolean32 insert_data (
    boolean32   replace_ok,
    tree_handle *db,
    b_datum     *new_key,
    b_datum     *new_data
  )
{
    result_t    result;
    node_handle t;
    task_data   td;

    if (*db == NULL) {
        *db = mem_alloc_db();
    }

    td.insert  = true;
    td.implant = NULL;
    td.replace = replace_ok;
    td.root    = *db;

    td.key  = new_key;
    td.data = new_data;

    result = update(td.root->tree, &td);    /* Attempt to insert new data */
    if (result == IMPLANT) {                /* implant at level of root? */
        if (td.root->tree == NULL) {	    /* If there wasn't a root tree */
            td.root->tree = td.implant;	    /* then this is it. */
        } else {                            /* Else new root with old root */
            t = td.root->tree;              /* as one branch */
            if (key_less_equal(&(t->mkey->key), &(td.implant->mkey->key))) {
                td.root->tree = new_tree_node(t, td.implant, false, &td);
            } else {
                td.root->tree = new_tree_node(td.implant, t, false, &td);
            }
            td.implant->parent = t->parent = td.root->tree; /* set parents */

	    /* Chase rsons to determine largest key in subtrees */
            for (t = td.root->tree->lson; IS_3NODE(t); t = t->rson);
            td.root->tree->lkey = t->mkey;
            for (t = td.root->tree->mson; IS_3NODE(t); t = t->rson);
            td.root->tree->mkey = t->mkey;
        }
        return true;
    } else if (result == REPLACED) {
        return true;
    }
    return false;
} /* insert_data */


PUBLIC b_datum * fetch_data (
    tree_handle db,
    b_datum     *search_key
  )
{
    node_handle result_node;
    result_t    result;
    b_datum     *search_data = NULL;

    if (db == NULL) {
        return search_data;
    }

    result = search(db->tree, search_key, &result_node);

    if (result == DUPLICATE) {
        search_data = &(result_node->mkey->data);
    }

    return search_data;
}


PUBLIC b_datum * fetch_next (
    tree_handle db,
    b_datum     *search_key,
    b_datum     **next_key
  )
{
    node_handle result_node;
    result_t    result;
    b_datum     *search_data = NULL;

    if (db == NULL) {
        return search_data;
    }

    *next_key = NULL;
    result = search_next(db->tree, search_key, &result_node);

    if (result == DUPLICATE) {
        search_data = &(result_node->mkey->data);
        *next_key = &(result_node->mkey->key);
    }

    return search_data;
}


/* 
 * Utility routines for delete_data() below
 */

/* Find the rightmost key */
PRIVATE data_handle find_key (
    node_handle tree
)
{
    if (tree == NULL) return NULL;
    while (IS_3NODE(tree))
        tree = tree->rson;
    return tree->mkey;    
}


PRIVATE void propagate_key (
    node_handle tree
)
{
    node_handle parent;

    if (tree == NULL) return;	        /* No keys to propagate */

    /* Walk back up the tree */
    for (parent = tree->parent; parent != NULL;
	 tree=parent, parent=tree->parent) {

        if (tree == parent->lson)       /* If we're the left son */
            parent->lkey = find_key(tree);      /* set left key */
        else if (tree == parent->mson)	/* else if we're the middle son */
            parent->mkey = find_key(tree);      /* set the middle key */
    }
} /* propagate_key */


/* Delete a node, retaining balance of the tree.
 * The cases are as follows and most are straightforward:
 *
 *  1. The node is the root of the tree (only thing in the table).  free it.
 *  2. The parent of the node is a 3-node; just free this son (still balanced).
 *  3. parent (which is a 2-node) is root; make our sibling the new root, free
 *     this son and parent.
 *  4. parent is 2-node, not root.  Thus parent has a sibling, left or right.
 *     If that uncle is a 3-node, we move one of its children to our parent.
 *     Otherwise we give the uncle our lone sibling, then recursively delete
 *     our parent node, then free this node.
 */

PRIVATE void delete_node (
    tree_handle db,
    node_handle n
)
{
    node_handle         dad, grandpa, uncle;/* relatives */
    enum {left, right}  tilt;               /* relation of uncle to parent */

    dad = n->parent;                        /* remember parent */

    if (dad == NULL) {          /* Is this is the only thing in the tree? */
        db->tree = NULL;
        free((char *)n);
    } else if (IS_3NODE(dad)) {    /* if child of a 3-node just delete */

        /* Shift remaining siblings to the left */
        switch(IS_LSON(n) ? 1 : IS_MSON(n) ? 2 : 3) {
            case 1: dad->lson = dad->mson; dad->lkey = dad->mkey;
		/* fall through */
            case 2: dad->mson = dad->rson; dad->mkey = find_key(dad->mson); 
            case 3: dad->rson = NULL;
        }
        propagate_key(dad);         /* propagate keys up through ancestry */
        free((char *)n);
	
    } else if ((grandpa = dad->parent) == NULL) {
    /* parent is 2-node, parent is root */

	/* root becomes our sibling */
        db->tree = IS_LSON(n) ? dad->mson : dad->lson;
        db->tree->parent = NULL;
        free((char *)dad);   /* delete parent */
        free((char *)n);
    } else {                        /* 2-node parent, not root */
        if (IS_LSON(dad)) {         /* is uncle to left or right of parent? */
            uncle = grandpa->mson;
            tilt = right;
        } else if (IS_MSON(dad)) {
            uncle = grandpa->lson;
            tilt = left;
        } else {
            uncle = grandpa->mson;
            tilt = left;
        }

        if (IS_3NODE(uncle)) {      /* if uncle is a 3-node borrow a son */
            if (tilt == left) {     /* if uncle is to left of parent */
                if (IS_MSON(n)) {   /* surviving sibling may need to move */
                    dad->mson = dad->lson;  /* lson->mson */
                    dad->mkey = dad->lkey;
                } 
                dad->lson = uncle->rson;    /* appropriate cousin as sibling */
                dad->lson->parent = dad;
                uncle->rson = NULL;         /* uncle now 2-node */
                dad->lkey = find_key(dad->lson);    /* recalc key */

                propagate_key(uncle);
                propagate_key(dad);
                free((char *)n);
            } else {                        /* uncle is on right */
                if (IS_LSON(n)) {   /* surviving sibling may need to move */
                    dad->lson = dad->mson;  /* mson->lson */
                    dad->lkey = dad->mkey;
                } 
                dad->mson = uncle->lson;    /* appropriate cousin as sibling */
                dad->mson->parent = dad;
                dad->mkey = uncle->lkey;
                uncle->lson = uncle->mson;  /* rearrange remaining cousins */
                uncle->mson = uncle->rson;  
                uncle->rson = NULL;         /* uncle now 2-node */
                uncle->lkey = uncle->mkey;  /* know this key */
                uncle->mkey = find_key(uncle->mson);  /* construct this one */
                propagate_key(uncle);
                propagate_key(dad);
                free((char *)n);
            }
        } else {                            /* uncle has only 2 sons */
            if (tilt == left) {             /* if uncle is to left of dad */

		/* give sibling away */
                uncle->rson = IS_LSON(n) ? dad->mson : dad->lson;
                uncle->rson->parent = uncle;
            } else {

		/* uncle on right: move cousins over */
                uncle->rson = uncle->mson;
                uncle->mson = uncle->lson;
                uncle->mkey = uncle->lkey;
                if (IS_LSON(n)) {   /* which is the sibling to give away? */
                    uncle->lson = dad->mson;
                    uncle->lkey = dad->mkey;
                } else {
                    uncle->lson = dad->lson;
                    uncle->lkey = dad->lkey;
                }
                uncle->lson->parent = uncle;
            } /* moving sibling to uncle */
            propagate_key(uncle);       /* reflect changes in uncle's tree */
            delete_node(db, dad);       /* recursively delete parent */
            free((char *)n);
        }
    } /* end if..then..else chain of cases */

} /* delete_node */


PUBLIC boolean32 delete_data (
    tree_handle db,
    b_datum     *search_key
  )
{
    node_handle result_node;
    data_handle	data_h;
    result_t    result;

    if (db == NULL) {
        return false;
    }
    result = search(db->tree, search_key, &result_node);

    if (result == DUPLICATE) {
        data_h = result_node->lkey;         /* hold onto leaf key data */
        delete_node(db, result_node);       /* delete tree node */
        free((char *)data_h);               /* delete key once node deleted */
        return true;
    }

    return false;
} /* delete_data */


static int print_indent = 0;
static int level = 0;

PRIVATE void dump_database (
    node_handle      tree,
    node_print_f_t   node_print_f
  )
{
    char node_type;

    if (tree == NULL) {
        printf("%*s*** empty tree ***\n", print_indent, "");
        return;
    }

    if (IS_LEAF(tree)) {
        printf("%*s", print_indent, "");
        (*node_print_f) (tree->mkey, (char *) NULL, true);
    } else {
        printf("%*s---%d---\n", print_indent, "", level);
        print_indent += 4;
        level++;

        dump_database(tree->lson, node_print_f);
        if (IS_3NODE(tree)) {
            node_type = '3';
        } else {
            node_type = '2';
        }

        printf("%*s", print_indent-4, "");
        (*node_print_f) (tree->lkey, "", false);
        printf("*%c*", node_type);
        (*node_print_f) (tree->mkey, "\n", false);

        dump_database(tree->mson, node_print_f);
        if (IS_3NODE(tree)) {
            dump_database(tree->rson, node_print_f);
        }
        print_indent -= 4;
        level--;
        printf("%*s---%d---\n", print_indent, "", level);
    }
}


void dump_database_root (
    tree_handle     root,
    node_print_f_t  node_print_f
  )
{
    dump_database(root->tree, node_print_f);
}

static  int total_size;
static  int num_nodes;
static  int fd;
static  char buf[1024];
static  char *buf_p;
static  int size;
static  int room;

typedef struct file_header_t {
    int     num_nodes;
    short   read_version;
    short   write_version;
} file_header_t;

#define CURRENT_READ_VERSION 1
#define CURRENT_WRITE_VERSION 1


PUBLIC int save_database (
    char        *fname,
    tree_handle db
  )
{
    char            msg[256];
    file_header_t   hdr;

    fd = rsdb_file_open(fname, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_errno_file_text,
                       fname,
                       strerror(errno));
    }

    total_size          = 0;
    num_nodes           = 0;
    hdr.num_nodes       = num_nodes;
    hdr.read_version    = CURRENT_READ_VERSION;
    hdr.write_version   = CURRENT_WRITE_VERSION;

    if (write(fd, (char *) &hdr, sizeof(hdr)) != sizeof(hdr)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       sec_sys_file_write_error,
                       fd);
        dce_svc_printf(SEC_RSDB_DB_WRITE_FAIL_MSG);
    }

    if ( db != NULL ) {
    /* there is no data for this data file, skip the writing */
        buf_p = buf;
        room = sizeof(buf);
        output_database(db->tree);

        if (room < sizeof(buf)) {
            if (write(fd, buf, sizeof(buf) - room) != (sizeof(buf) - room)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                               sec_s_rs_rsdb,
                               svc_c_sev_error,
                               sec_sys_file_write_error,
                               fd);
                dce_svc_printf(SEC_RSDB_DB_WRITE_FAIL_MSG);
            }
        }

        hdr.num_nodes = num_nodes;
        rsdb_file_position(fd, rsdb_file_position_beginning);
        if (write(fd, (char *) &hdr, sizeof(hdr)) != sizeof(hdr)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                           sec_s_rs_rsdb,
                           svc_c_sev_error,
                           sec_sys_file_write_error,
                           fd);
            dce_svc_printf(SEC_RSDB_DB_WRITE_FAIL_MSG);
        }
    }

    rsdb_file_fsync(fd);
    rsdb_file_close(fd);
    fd = -1;

    return total_size;
}


/*
 * This routine relies on datanodes being contiguous with the data
 */
PRIVATE void output_database (
    node_handle tree
  )
{
    if (tree == NULL) {
        return;
    }

    if (IS_LEAF(tree)) {
        int save_size;

        num_nodes++;
        size = sizeof(data_node) + tree->mkey->key.dsize 
                    + tree->mkey->data.dsize;
        /*
         * Round buffer size up to a word boundary
         */
        size = (size + (FULLWORD_SIZE - 1)) & ~(FULLWORD_SIZE - 1);

        if (size != tree->mkey->size) {
            save_size = tree->mkey->size;
            tree->mkey->size = size;
        } else {
            save_size = size;
        }

        if ((size > room) && (room < sizeof(buf))) {
            if (write(fd, buf, sizeof(buf) - room) != (sizeof(buf) - room)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                               sec_s_rs_rsdb,
                               svc_c_sev_error,
                               sec_sys_file_write_error,
                               fd);
                dce_svc_printf(SEC_RSDB_DB_WRITE_FAIL_MSG);
            }
            room = sizeof(buf);
            buf_p = buf;
        }

        if (size > room) {
            if (write(fd, (char *) tree->mkey, size) != size) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                               sec_s_rs_rsdb,
                               svc_c_sev_error,
                               sec_sys_file_write_error,
                               fd);
                dce_svc_printf(SEC_RSDB_DB_WRITE_FAIL_MSG);
            }
        } else {
            bcopy( (Pointer) tree->mkey, buf_p, size);
            buf_p += size;
            room -= size;
        }

        total_size += size;
        if (size != save_size) {
            tree->mkey->size = save_size;
        }

    } else {
        output_database(tree->lson);
        output_database(tree->mson);
        if (IS_3NODE(tree)) {
            output_database(tree->rson);
        }
    }
}


PUBLIC void clear_database (
    tree_handle *db
  )
{
    if ((*db) != NULL) {
        mem_db_free(*db);
    }
}


/*
 * pass through the newly build tree setting everybody's parent pointers.
 */
PRIVATE void set_parent ( tree, parent )
    node_handle         tree;
    node_handle         parent;
{
    if (tree == NULL) return;

    tree->parent = parent;
    if (IS_LEAF(tree)) return;

    set_parent(tree->lson, tree);
    set_parent(tree->mson, tree);
    set_parent(tree->rson, tree);

} /* set_parent */


PUBLIC void input_database (
    char        *fname,
    tree_handle *db
  )
{
    struct stat stat_buf;
    char            msg[256];
    int             size;
    int             fd;
    int             num_nodes, nodes_left;
    data_handle     data_p, tdata_p;
    file_header_t   hdr;

    fd = rsdb_file_open(fname, O_RDONLY, 0);
    if (fd == -1) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_errno_file_text,
		       fname,
                       strerror(errno));
    }

    if (rsdb_file_fstat(fd, &stat_buf) < 0) {
        dce_svc_printf(SEC_RSDB_FILE_STAT_FAIL_MSG, fd);
    }

    if (stat_buf.st_size <= sizeof(hdr)) {
        return;
    }

    size = stat_buf.st_size - sizeof(hdr);

    read(fd, (char *) &hdr, sizeof(hdr));
    if (hdr.read_version != CURRENT_READ_VERSION) {
        dce_svc_printf(SEC_RSDB_READVER_FAIL_MSG, hdr.read_version);
    }
    num_nodes = hdr.num_nodes;

    if ((*db) == NULL) {
        *db = mem_alloc_db();
    } else {
        mem_db_free(*db);
    }

    tdata_p = data_p = (data_handle) mem_alloc(*db, size, true);
    if (data_p == NULL) {
	dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
		       sec_s_rs_rsdb,
		       svc_c_sev_fatal | svc_c_action_abort,
		       sec_s_no_memory);
    }

    /*
     * Avoid task time slice problems - read in chunks instead of one large
     * gobble.
     */
    {
        int     tsize;
        char    *p;
        int     read_size = (64*1024);

        p = (char *) data_p;

        for (tsize = size; tsize > 0; ) {
	    int rs = MIN(read_size, tsize);
            read(fd, p, rs);
            p += rs;
	    tsize -= rs;
            rs_task_yield();
        }
    }
    rsdb_file_close(fd);

    /* Build the tree from the data nodes we got from disk */
    for (nodes_left = num_nodes; nodes_left > 0; --nodes_left) {
        data_p->key.dptr = (char *) (data_p + 1);
        data_p->data.dptr = data_p->key.dptr + data_p->key.dsize;
	if (! insert_data(false, db, &data_p->key, &data_p->data)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                sec_s_rs_rsdb, svc_c_sev_fatal | svc_c_action_abort,
                sec_s_pgmerr);
        }
        data_p = (data_handle) (((unsigned) data_p) + data_p->size);
    }
    free((char *)tdata_p);          /* Done with initial chunk of memory */

    set_parent((*db)->tree, NULL);  /* pass through filling in parents */
}


#ifdef DEBUG_BTREE
void print_tree (
    node_handle tree
  )
{
    char node_type;

    if (tree == NULL) {
        printf("%*s*** empty tree ***\n", print_indent, "");
        return;
    }

    if (IS_LEAF(tree)) {
        printf("%*s%.*s\n", print_indent, "", tree->mkey->key.dsize,
                                             tree->mkey->key.dptr);
    } else {
        printf("%*s---%d---\n", print_indent, "", level);
        print_indent += 4;
        level++;

        print_tree(tree->lson);
        if (IS_3NODE(tree)) {
            node_type = '3';
        } else {
            node_type = '2';
        }

        printf("%*s%.*s%c%.*s\n", print_indent-4, "",
                    tree->lkey->key.dsize, tree->lkey->key.dptr,
                    node_type,
                    tree->mkey->key.dsize, tree->mkey->key.dptr);

        print_tree(tree->mson);
        if (IS_3NODE(tree)) {
            print_tree(tree->rson);
        }
        print_indent -= 4;
        level--;
        printf("%*s---%d---\n", print_indent, "", level);
    }
}
#endif /* DEBUG_BTREE */
