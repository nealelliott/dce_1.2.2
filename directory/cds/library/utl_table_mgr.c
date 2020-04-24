/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: utl_table_mgr.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:24  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:27:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:47  root]
 * 
 * Revision 1.1.4.3  1994/06/30  19:18:03  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:05:17  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:42:38  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:52  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:09:23  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:36:34  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:18:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module utl_table_mgr.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 * Module Description
 *
 * A simple hashed table manager.
 *
 */


#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <utl_table_mgr.h>

#define HASH_SIZE 101

struct table_key_entry {
    char *key;
    struct table_key_entry *next_collision;
    struct table_key_entry *prev_collision;
    void *data;
};

struct context_buffer {
    int hash_size;
    struct table_key_entry *hash_table[HASH_SIZE];
};

/*
 * Static function prototypes
 */
static unsigned 
hash_key (
    char        *,
    int          );

static struct table_key_entry *
get_key_entry(
    struct context_buffer *,
    char                  *);



Hashed_Table 
declare_table (int hash_size)
{
    struct context_buffer *ctx;
    ctx = (struct context_buffer *) calloc(1, sizeof(struct context_buffer));
    if (ctx == NULL) {
        return (NULL);
    }

    ctx->hash_size = HASH_SIZE;

    return ((Hashed_Table) ctx);
}

static unsigned 
hash_key (char  *key_value,
          int   hash_size)
{

    unsigned hash_value;

    hash_value = 0;

    while (*key_value != '\0') {
        hash_value = *key_value + 31 * hash_value;
        key_value++;
    }

#ifdef FORCE_COLLISION
   return (10);
#else
   return (hash_value % hash_size);
#endif

}


static struct table_key_entry 
*get_key_entry (struct context_buffer  *table,
                char                   *key_value)
{
    struct table_key_entry *work_entry;

    work_entry = table->hash_table[hash_key(key_value, table->hash_size)];

    while (work_entry != NULL) {
        if (strcmp(key_value, work_entry->key) == 0) {
            return (work_entry);
        }
        work_entry = work_entry->next_collision;
    }

    return (NULL);
}

int 
lookup_table (Hashed_Table  visible_table,
              char          *key_value,
              void          **user_data)
{
    struct context_buffer *table = (struct context_buffer *)visible_table;
    struct table_key_entry *requested_entry;

    requested_entry = get_key_entry(table, key_value);

    if (requested_entry == NULL) {
        return (FALSE);
    } else {
        if (user_data != NULL) {
            *user_data = requested_entry->data;
        }
        return (TRUE);
    }
}



int 
enter_table (Hashed_Table  visible_table,
             char          *key_value,
             void          *data)
{
    struct context_buffer *table = (struct context_buffer *)visible_table;
    struct table_key_entry *work_entry;
    unsigned hash_value;

    work_entry = get_key_entry(table, key_value);
    if (work_entry == NULL) {
        work_entry = (struct table_key_entry *) malloc(sizeof(struct table_key_entry));
        work_entry->key = (char *) malloc(strlen(key_value) + 1);
        if (work_entry->key == 0) {
            return (-1);                  /* error */
        }
        strcpy(work_entry->key, key_value);
        work_entry->data = data;
        hash_value = hash_key(key_value, table->hash_size);
        work_entry->next_collision = table->hash_table[hash_value];
        if (work_entry->next_collision != NULL) {
            work_entry->next_collision->prev_collision = work_entry;
        }
        work_entry->prev_collision = NULL;
        table->hash_table[hash_value] = work_entry;
        return (TRUE);
    } else {
        return (FALSE);
    }
}



int 
remove_table (Hashed_Table  visible_table,
              char          *key_value,
              void          **user_data)
{
    struct context_buffer *table = (struct context_buffer *)visible_table;
    struct table_key_entry *work_entry;
    unsigned hash_value;

    work_entry = get_key_entry(table, key_value);
    if (work_entry != NULL) {
        if (work_entry->prev_collision == NULL) {
            hash_value = hash_key(key_value, table->hash_size);
            table->hash_table[hash_value] = work_entry->next_collision;
            if (work_entry->next_collision != NULL) {
                work_entry->next_collision->prev_collision = NULL;
            }
        } else {
            if (work_entry->next_collision != NULL) {
                work_entry->next_collision->prev_collision =
                    work_entry->prev_collision;
            }
            work_entry->prev_collision->next_collision = work_entry->next_collision;
        }

        /* Unlinked from list */

        free(work_entry->key);
        if (user_data != NULL) {
            *user_data = work_entry->data;
        }
        free(work_entry);
        return (TRUE);
    } else {
        return (FALSE);
    }
}


