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
 * Revision 1.1.9.2  1996/03/11  13:26:19  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:21  marty]
 *
 * Revision 1.1.9.1  1995/12/08  17:56:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:28:57  root]
 * 
 * Revision 1.1.7.1  1993/10/05  22:29:48  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:03:26  mccann]
 * 
 * Revision 1.1.5.3  1992/12/29  12:37:44  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:33:49  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:14:38  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:18:27  sekhar]
 * 
 * Revision 1.1.2.2  1992/04/23  14:12:55  burati
 * 	Initial version, from original in server/rsdb.
 * 	[1992/04/23  01:18:06  burati]
 * 
 * $EndLog$
 */

/*  balanced_trees.c V=11 04/29/91
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Passwd Import - balanced tree manager
 */

/* 
* Roughly modeled on the algorithms in Aho Hopcroft and Ullman.  - modified so 
* that 3 node splitting is handled by the recursive implementation of
* search_insert
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <macros.h>
#include <un_stat.h> 
#include <bstring.h>
#include <un_io.h>

#include <balanced_trees.h>

#define FULLWORD_SIZE sizeof(long)     /* Must be a power of 2 */

typedef struct mem_buf *mem_ptr_t;
struct mem_buf {
    mem_ptr_t    next;
    char        *xfree;
    long        space_avail;
};

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
    data_handle lkey;
    data_handle mkey;
} tree_node;

struct tree_root {
    mem_ptr_t    mem;
    node_handle tree;
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
#define IS_LEAF(P) ((P)->lson == NULL)
#define IS_DELETED(P) ((P)->mson == NULL)
#define NOT_DELETED(P) ((P)->mson = (P))
#define MAKE_DELETED(P) ((P)->mson = NULL)

#ifdef MIN
#undef MIN
#endif
#define MIN(A,B) ((A) < (B) ? (A) : (B))

#define alloc_tree_node() ((tree_handle) mem_alloc(sizeof(tree_node)))
 
typedef void (*node_print_f_t) (
#ifdef __STDC__
    data_handle  mkey,
    char         *string,
    boolean32    bool
#endif
);   

/*** Forward Declarations ***/
PRIVATE tree_handle mem_alloc_db(
#ifdef __STDC__
    void
#endif
);

PRIVATE char *mem_alloc(
#ifdef __STDC__
    tree_handle db,
    long        size
#endif
);

PRIVATE mem_ptr_t mem__alloc(
#ifdef __STDC__
    long    size
#endif
);

PRIVATE void mem_free(
#ifdef __STDC__
    tree_handle db
#endif
);

PRIVATE boolean32 key_less_equal(
#ifdef __STDC__
    b_datum *k1,
    b_datum *k2
#endif
);

PRIVATE boolean32 key_less(
#ifdef __STDC__
    b_datum *k1,
    b_datum *k2
#endif
);

PRIVATE boolean32 key_equal(
#ifdef __STDC__
    b_datum *k1,
    b_datum *k2
#endif
);

PRIVATE void store_new_data(
#ifdef __STDC__
    data_handle new,
    b_datum     *key,
    b_datum     *data
#endif
);

PRIVATE data_handle new_data_node(
#ifdef __STDC__
    b_datum     *key,
    b_datum     *data,
    task_data   *td
#endif
);
 
PRIVATE data_handle replace_data_node(
#ifdef __STDC__
    data_handle old_data,
    b_datum     *key,
    b_datum     *data,
    task_data   *td
#endif
);

PRIVATE node_handle new_tree_node(
#ifdef __STDC__
    node_handle lson,
    node_handle mson,
    boolean32   add_data,
    task_data   *td
#endif
);

PRIVATE result_t update(
#ifdef __STDC__
    node_handle tree,
    task_data   *td
#endif
);

PRIVATE result_t search_next(
#ifdef __STDC__
    node_handle root,
    b_datum     *key,
    node_handle *result_node
#endif
);

PRIVATE result_t search(
#ifdef __STDC__
    node_handle tree,
    b_datum     *key,
    node_handle *result_node
#endif
);

PRIVATE void dump_database(
#ifdef __STDC__
    node_handle      tree,
    node_print_f_t   node_print_f
#endif
);

/*
* Memory Allocation Rountines
*/

PRIVATE tree_handle mem_alloc_db (
#ifdef __STDC__
    void
#endif
)
{
    tree_handle db;

    db = (tree_handle) malloc(sizeof(*db));
    db->mem = NULL;
    db->tree = NULL;

    return db;
}

PRIVATE mem_ptr_t    mem__alloc
#ifndef __STDC__
    ( size ) 
    long    size;
#else
  (
    long    size
  )
#endif
{
#define ALLOC_BLKSIZE (16*1024) /* Must be a power of 2 */
    mem_ptr_t    p;
    long        alloc_size;
    char        msg[128];


    alloc_size = size + sizeof(*p);
    alloc_size = (alloc_size + (ALLOC_BLKSIZE - 1)) & ~(ALLOC_BLKSIZE - 1);

    p = (mem_ptr_t) malloc(alloc_size);
    if (p == NULL) {
        return NULL;
    }

    p->next         = NULL;
    p->space_avail  = (alloc_size) - sizeof(*p);
    p->xfree        = (char *) (p+1);

    return p;
}

PRIVATE char * mem_alloc
#ifndef __STDC__
    ( db, size ) 
    tree_handle db;
    long        size;
#else
  (
    tree_handle db,
    long        size
  )
#endif
{
    mem_ptr_t    p;
    char        *ret_ptr = NULL;

    /*
    * Round size up to a fullword boundary
    */
    size = (size + (FULLWORD_SIZE - 1)) & ~(FULLWORD_SIZE - 1);

    if (db->mem == NULL) {
        db->mem = mem__alloc(size);
        if (db->mem == NULL)
            return ret_ptr;
    }

    for (p = db->mem; p->next != NULL && p->space_avail < size; p = p->next);

    if (p->space_avail < size) {
        p->next = mem__alloc(size);
        if (p->next == NULL)
            return ret_ptr;

        p = p->next;
    }

    p->space_avail -= size;
    ret_ptr = p->xfree;
    p->xfree += size;

    return ret_ptr;
}

PRIVATE void mem_free
#ifndef __STDC__
    ( db ) 
    tree_handle db;
#else
  (
    tree_handle db
  )
#endif
{
    mem_ptr_t    p;
    mem_ptr_t    next_p;

    if (db != NULL) {
        for (p = db->mem; p != NULL; p = next_p) {
            next_p = p->next;
            free(p);
        }
        db->mem  = NULL;
        db->tree = NULL;
    }
}




PRIVATE boolean32 key_less_equal
#ifndef __STDC__
    ( k1, k2 ) 
    b_datum *k1;
    b_datum *k2;
#else
  (
    b_datum *k1,
    b_datum *k2
  )
#endif
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

PRIVATE boolean32 key_less
#ifndef __STDC__
    ( k1, k2 ) 
    b_datum *k1;
    b_datum *k2;
#else
  (
    b_datum *k1,
    b_datum *k2
  )
#endif
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

PRIVATE boolean32 key_equal
#ifndef __STDC__
    ( k1, k2 ) 
    b_datum *k1;
    b_datum *k2;
#else
  (
    b_datum *k1,
    b_datum *k2
  )
#endif
{
    int result;

    if (k1->dsize != k2->dsize)
        return false;

    result = memcmp(k1->dptr, k2->dptr, k1->dsize);
    if (result != 0)
        return false;

    return true;
}

PRIVATE void store_new_data
#ifndef __STDC__
    ( new, key, data ) 
    data_handle new;
    b_datum     *key;
    b_datum     *data;
#else
  (
    data_handle new,
    b_datum     *key,
    b_datum     *data
  )
#endif
{
    new->key.dsize = key->dsize;
    new->key.dptr  = (char *) (new + 1);
    bcopy(key->dptr, new->key.dptr, new->key.dsize);
    new->data.dsize = data->dsize;
    new->data.dptr  = new->key.dptr + new->key.dsize;
    bcopy(data->dptr, new->data.dptr, new->data.dsize);
} 

PRIVATE data_handle new_data_node
#ifndef __STDC__
    ( key, data, td ) 
    b_datum     *key;
    b_datum     *data;
    task_data   *td;
#else
  (
    b_datum     *key,
    b_datum     *data,
    task_data   *td
  )
#endif
{
    data_handle new;
    int         size;

    size = sizeof(data_node) + key->dsize + data->dsize;
    new = (data_handle) mem_alloc(td->root, size);
    new->size = size;
    store_new_data(new, key, data);

    return new;
}

PRIVATE data_handle replace_data_node
#ifndef __STDC__
    ( old_data, key, data, td ) 
    data_handle old_data;
    b_datum     *key;
    b_datum     *data;
    task_data   *td;
#else
  (
    data_handle old_data,
    b_datum     *key,
    b_datum     *data,
    task_data   *td
  )
#endif
{
    int size;

    size = sizeof(data_node) + key->dsize + data->dsize;
    if (size <= old_data->size) {
        store_new_data(old_data, key, data);
        return old_data;
    } else {
        return new_data_node(key, data, td);
    }
}

PRIVATE node_handle new_tree_node
#ifndef __STDC__
    ( lson, mson, add_data, td ) 
    node_handle lson;
    node_handle mson;
    boolean32   add_data;
    task_data   *td;
#else
  (
    node_handle lson,
    node_handle mson,
    boolean32   add_data,
    task_data   *td
  )
#endif
{
    node_handle implant;

    implant = (node_handle) mem_alloc(td->root, sizeof(tree_node));
    implant->rson = NULL;
    implant->lson = lson;
    implant->mson = mson;
    if (IS_DELETED(implant)) {
        NOT_DELETED(implant);
    }
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

PRIVATE result_t update
#ifndef __STDC__
    ( tree, td ) 
    node_handle tree;
    task_data   *td;
#else
  (
    node_handle tree,
    task_data   *td
  )
#endif
{
    result_t    retval;
    node_handle split_node;
    node_handle t;
    enum {left, middle, right} branch;

    if (tree == NULL) {
        if (td->insert) {
            td->implant = new_tree_node( (node_handle) NULL, (node_handle) NULL,
					true, td);
            return IMPLANT;
        } else {
            return NOT_FOUND;
        }
    }

    if (IS_LEAF(tree)) {
        if (key_equal(td->key, &(tree->lkey->key))) {
            td->implant = tree;
            if (td->replace || (td->insert && IS_DELETED(tree))) {
                tree->lkey = tree->mkey
                    = replace_data_node(tree->lkey, td->key, td->data, td);
                NOT_DELETED(tree);
                return REPLACED;
            } else if (IS_DELETED(tree)) {
                return NOT_FOUND;
            } else {                
                return DUPLICATE;
            }
        } else {
            if (td->insert) {
                td->implant = new_tree_node( (node_handle) NULL, 
					    (node_handle) NULL, true, td);
                return IMPLANT;
            } else {
                return NOT_FOUND;
            }
        }
    } else {
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

            } else {
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
}

PRIVATE result_t search_next
#ifndef __STDC__
    ( root, key, result_node ) 
    node_handle root;
    b_datum     *key;
    node_handle *result_node;
#else
  (
    node_handle root,
    b_datum     *key,
    node_handle *result_node
  )
#endif
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
            if (IS_DELETED(tree)) {
                key = &tree->lkey->key;
            } else {
                *result_node = tree;
                return DUPLICATE;
            }
        } else {
            return NOT_FOUND;
        }
    }

    /* NOTREACHED */
}

PRIVATE result_t search
#ifndef __STDC__
    ( tree, key, result_node ) 
    node_handle tree;
    b_datum     *key;
    node_handle *result_node;
#else
  (
    node_handle tree,
    b_datum     *key,
    node_handle *result_node
  )
#endif
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
        if (IS_DELETED(tree)) {
            return NOT_FOUND;
        } else {
            *result_node = tree;
            return DUPLICATE;
        }
    } else {
        return NOT_FOUND;
    }

    /* NOTREACHED */
}

PUBLIC boolean32 insert_data
#ifndef __STDC__
    ( replace_ok, db, new_key, new_data ) 
    boolean32   replace_ok;
    tree_handle *db;
    b_datum     *new_key;
    b_datum     *new_data;
#else
  (
    boolean32   replace_ok,
    tree_handle *db,
    b_datum     *new_key,
    b_datum     *new_data
  )
#endif
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

    result = update(td.root->tree, &td);
    if (result == IMPLANT) {
        if (td.root->tree == NULL) {
            td.root->tree = td.implant;
        } else {
            t = td.root->tree;
            if (key_less_equal(&(t->mkey->key), &(td.implant->mkey->key))) {
                td.root->tree = new_tree_node(t, td.implant, false, &td);
            } else {
                td.root->tree = new_tree_node(td.implant, t, false, &td);
            }
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
}

PUBLIC b_datum * fetch_data
#ifndef __STDC__
    ( db, search_key ) 
    tree_handle db;
    b_datum     *search_key;
#else
  (
    tree_handle db,
    b_datum     *search_key
  )
#endif
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

PUBLIC b_datum * fetch_next
#ifndef __STDC__
    ( db, search_key, next_key ) 
    tree_handle db;
    b_datum     *search_key;
    b_datum     **next_key;
#else
  (
    tree_handle db,
    b_datum     *search_key,
    b_datum     **next_key
  )
#endif
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

PUBLIC boolean32 delete_data
#ifndef __STDC__
    ( db, search_key ) 
    tree_handle db;
    b_datum     *search_key;
#else
  (
    tree_handle db,
    b_datum     *search_key
  )
#endif
{
    node_handle result_node;
    result_t    result;

    if (db == NULL) {
        return false;
    }
    result = search(db->tree, search_key, &result_node);

    if (result == DUPLICATE) {
        MAKE_DELETED(result_node);
        return true;
    }

    return false;
}

static int print_indent = 0;
static int level = 0;

PRIVATE void dump_database
#ifndef __STDC__
    ( tree, node_print_f ) 
    node_handle      tree;
    node_print_f_t   node_print_f;
#else
  (
    node_handle      tree,
    node_print_f_t   node_print_f
  )
#endif
{
    char node_type;

    if (tree == NULL) {
        printf("%*s*** empty tree ***\n", print_indent, "");
        return;
    }

    if (IS_LEAF(tree)) {
        printf("%*s", print_indent, "");
        (*node_print_f) (tree->mkey, (char *) NULL, true);
        printf("%s\n", IS_DELETED(tree) ? " *Deleted*" : "");
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

void dump_database_root
#ifndef __STDC__
    ( root, node_print_f ) 
    tree_handle     root;
    node_print_f_t  node_print_f;
#else
  (
    tree_handle     root,
    node_print_f_t  node_print_f
  )
#endif
{
    dump_database(root->tree, node_print_f);
}

PUBLIC void clear_database
#ifndef __STDC__
    ( db ) 
    tree_handle *db;
#else
  (
    tree_handle *db
  )
#endif
{
    if ((*db) != NULL) {
        mem_free(*db);
    }
}

#ifdef DEBUG_BTREE
PUBLIC void print_tree
#ifndef __STDC__
    ( tree ) 
    node_handle tree;
#else
  (
    node_handle tree
  )
#endif
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







