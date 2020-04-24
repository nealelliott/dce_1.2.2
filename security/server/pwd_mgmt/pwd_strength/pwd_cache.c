/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pwd_cache.c,v $
 * Revision 1.1.4.2  1996/02/18  00:16:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:47:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:24  root]
 * 
 * Revision 1.1.2.4  1994/08/10  19:22:03  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:21  annie]
 * 
 * Revision 1.1.2.3  1994/08/04  16:13:13  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:34  mdf]
 * 
 * Revision 1.1.2.2  1994/07/17  20:22:30  baum
 * 	Added sec_passwd_c_des_key_size as len field to memcmp() call to fix broken build
 * 	[1994/07/17  20:22:18  baum]
 * 
 * Revision 1.1.2.1  1994/07/15  15:00:56  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:10  mdf]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Virtual memory password caching routines 
 */

#include <pwd_cache.h>

#include <stdlib.h>
#include <string.h>		/* For memcmp() */
#include <dce/secsts.h>

/*
 * create_table initializes the table
 */
void create_table (
    error_status_t  *stp
)
{
    int x;

    *stp = error_status_ok;

    table = (rec_p_t *) calloc(cache_size, sizeof(rec_p_t));
    if (table == NULL) {
        *stp = sec_s_no_memory;
        return;
    }
    for (x = 0; x < cache_size; x++)
        table[x] = NULL;
}

/*
 * delete_table() deletes all the records in a table
 * from memory.
 */
int delete_table (
    int size
)
{
    int x;

    /* XXX - Do we need to free what is in each table entry? */

    /* free all memory */
    for (x = 0; x < size; x++)
        if (table[x] != NULL)
            free(table[x]);

    return 0;
}

static void free_pwd (
    sec_passwd_rec_t  pwd
)
{
    unsigned char     *ucp;

    switch (pwd.key.key_type) {
    case sec_passwd_plain:
        if (pwd.key.tagged_union.plain)
            free(pwd.key.tagged_union.plain);
        break;
    case sec_passwd_des:
        memset(pwd.key.tagged_union.des_key, 0, sizeof(sec_passwd_des_key_t));
        break;
    }
}

static void free_pwd_set (
    unsigned32       num_pwds,
    sec_passwd_rec_t *pwd_set
)
{
    int x;

    if (!pwd_set)
        return;

    for (x = 0; x < num_pwds; x++)
        free_pwd(pwd_set[x]);

    free(pwd_set);
}

static void free_record (
    rec_p_t          rp
)
{
    if (!rp)
        return;

    if (rp->who)
        free(rp->who);

    free_pwd_set(rp->num_pwds, rp->gen_pwd_set);

    free(rp);
}

static void copy_pwd (
    sec_passwd_rec_t *to,
    sec_passwd_rec_t from,
    error_status_t   *stp
)
{
    *stp = error_status_ok;

    to->version_number = from.version_number;
    to->pepper = NULL;
    to->key.key_type = from.key.key_type;
    switch (from.key.key_type) {
    case sec_passwd_plain:
        to->key.tagged_union.plain = (unsigned char *)
                 malloc(strlen(from.key.tagged_union.plain)+1);
        if (to->key.tagged_union.plain == NULL) {
            *stp = sec_s_no_memory;
            return;
        }
        (void) strcpy(to->key.tagged_union.plain,
                      from.key.tagged_union.plain);
        break;
    case sec_passwd_des:
        memcpy(to->key.tagged_union.des_key, from.key.tagged_union.des_key,
               sizeof(from.key.tagged_union.des_key));
        break;
    }
}

/*
 * create_record() creates a record entry in the table.
 */
void create_record (
    char                    *who,
    unsigned32              num_pwds,
    sec_passwd_rec_t        gen_pwd_set[],
    error_status_t          *stp
)
{
    rec_p_t    rp;
    unsigned   hashval;
    int        duplicate = 0;
    int        x;

    *stp = error_status_ok;

    if (pthread_mutex_lock(&cache_mutex) < 0) {
        *stp = sec_pwd_mgmt_svr_error;
        error(NULL, "Unable to acquire mutex lock");
        /* XXX - should we terminate server? */
        return;
    }

    /* Use hash algorithm to find unique location */
    hashval = hash(who);
    for (rp = table[hashval]; rp != NULL; rp = rp->next) {
        if (strcmp(who, rp->who) == 0) {
            /*
             * duplicate entry! However, not a failure because
             * we will just replace this record with the current
             * one.
             */
            duplicate = 1;
            break;
        }
    } 

    if (!duplicate) {
        rp = (rec_p_t) malloc(sizeof(struct record));
        if (rp == NULL) {
            *stp = sec_s_no_memory;
        }
        if (*stp == error_status_ok) {
            rp->who = (char *) malloc(strlen(who) + 1);
            if (rp->who == NULL) {
                *stp = sec_s_no_memory;
                free(rp);
            } else
                (void) strcpy(rp->who, who);
        }
        if (*stp == error_status_ok) {
            rp->num_pwds = num_pwds;
            rp->gen_pwd_set = (sec_passwd_rec_t *) 
                calloc(num_pwds, sizeof(sec_passwd_rec_t));
            if (rp->gen_pwd_set == NULL) {
                *stp = sec_s_no_memory;
                free(rp->gen_pwd_set);
                free(rp);
            }
        }
        if (*stp == error_status_ok) {
            for (x = 0; x < num_pwds; x++) {
                copy_pwd(&rp->gen_pwd_set[x], gen_pwd_set[x], stp);
                if (*stp != error_status_ok) {
                    /* free previously allocated passwords */
                    free_pwd_set(x, rp->gen_pwd_set);
                    free(rp);
                }
            }
            if (*stp == error_status_ok) {
                rp->next = table[hashval];
                table[hashval] = rp;
            }
        }
    } else {
        /* XXX how do you prevent evil people from calling generator
           continuously and wiping out passwords */
        /* free password set */
        free_pwd_set(rp->num_pwds, rp->gen_pwd_set);
        rp->num_pwds = num_pwds;
        rp->gen_pwd_set = (sec_passwd_rec_t *)
            calloc(num_pwds, sizeof(sec_passwd_rec_t));
        if (rp->gen_pwd_set == NULL) {
            *stp = sec_s_no_memory;
            free(rp->gen_pwd_set);
        }
        if (*stp == error_status_ok) {
            for (x = 0; x < num_pwds; x++) {
                copy_pwd(&rp->gen_pwd_set[x], gen_pwd_set[x], stp);
                if (*stp != error_status_ok) {
                    /* free previously allocated passwords */
                    free_pwd_set(x, rp->gen_pwd_set);
                }
            }
        }
    }

    if (*stp == error_status_ok) {
        rp->last_update = time((time_t *) NULL);
    }

    if (pthread_mutex_unlock(&cache_mutex) < 0) {
        *stp = sec_pwd_mgmt_svr_error;
        error(NULL, "Unable to release mutex lock");
        /* XXX - should we terminate server? */
        return;
    }
}


/*
 * rec_del_after() deletes the node after p from the linked list.
 */
static void rec_del_after (
    rec_p_t    p
)
{
    rec_p_t    q;

    /* check if there is nothing to delete */
    if ((p == NULL) || (p->next == NULL))
        return;

    q = p->next;
    p->next = q->next;
    free_record(q);
}


void cleanup_table (
    error_status_t    *stp
)
{
    int               x;
    rec_p_t           prev, cur;
    time_t            current_time;

    *stp = error_status_ok;

    if (pthread_mutex_lock(&cache_mutex) < 0) {
        *stp = sec_pwd_mgmt_svr_error;
        error(NULL, "Unable to acquire mutex lock");
        return;
    }

    current_time = time((time_t *) NULL);

    /* loop through all entries in table */
    for (x = 0; x < cache_size; x++) {
        if (table[x] != NULL) {
            prev = NULL;
            cur = table[x];
            /* loop through all records with same hashval */
            while (cur != NULL) {

                /* check last update time */
                if ((current_time - cur->last_update) > timeout) {
                    /* delete record */
                    cur = cur->next;
                    if (prev == NULL) {
                        free_record(table[x]);
                        table[x] = cur;
                    } else {
                        rec_del_after(prev);
                        prev = cur;
                    }
                } else {
                    prev = cur;
                    cur = cur->next;
                }
            }
        }
    }

    if (pthread_mutex_unlock(&cache_mutex) < 0) {
        *stp = sec_pwd_mgmt_svr_error;
        error(NULL, "Unable to release mutex lock");
        return;
    }
}


/*
 * check_cache() searches the table for a principal
 * and password pair. If it finds it, it returns true.
 */
boolean32 check_cache (
    char              *who,
    sec_passwd_rec_t  *pwd,
    boolean32         *record_exists,
    error_status_t    *stp
)
{
    rec_p_t           cur, prev;
    unsigned          hashval;
    time_t            current_time;
    boolean32         found = false;
    int               x;
    
    *record_exists = false;
    *stp = error_status_ok;

    if (pthread_mutex_lock(&cache_mutex) < 0) {
        *stp = sec_pwd_mgmt_svr_error;
        error(NULL, "Unable to acquire mutex lock");
        /* XXX - should we terminate server? */
        return false;
    }

    prev = NULL;
    hashval = hash(who);

    /* search for record */
    for (cur = table[hashval]; cur != NULL; cur = cur->next) {
        if (strcmp(who, cur->who) == 0) {  /* found the record */

            *record_exists = true;

            /* check last update time */
            current_time = time((time_t *) NULL);
            if ((current_time - cur->last_update) > timeout) {
                /* delete record */
                cur = cur->next;
                if (prev == NULL) {
                    free_record(table[hashval]);
                    table[hashval] = cur;
                } else
                    rec_del_after(prev);

                found = false;
                break;
            }

            /* search for password */
            for (x = 0; x < cur->num_pwds; x++) {
                if (pwd->key.key_type != 
                        cur->gen_pwd_set[x].key.key_type) {
                    continue;
                }
                switch (pwd->key.key_type) {
                case sec_passwd_plain:
                    if (strcmp(pwd->key.tagged_union.plain,
                        cur->gen_pwd_set[x].key.tagged_union.plain) == 0) {
                        found = true;
                    }
                    break;
                case sec_passwd_des:
                    if (memcmp(pwd->key.tagged_union.des_key,
                        cur->gen_pwd_set[x].key.tagged_union.des_key,
			sec_passwd_c_des_key_size) == 0) {
                        found = true;
                    }
                    break;
                }

                if (found == true)
                    break;
            }

            /* 
             * break out of for loop because there is no point
             * to continuing search. Do not delete record because
             * a malicious user could force deletes by sending
             * bad passwords to the strength checker.
             */
            break;
        } else
            prev = cur;
    }

    if (pthread_mutex_unlock(&cache_mutex) < 0) {
        *stp = sec_pwd_mgmt_svr_error;
        error(NULL, "Unable to release mutex lock");
        /* XXX - should we terminate server? */
        return false;
    }

    return found;
}

/*
 * hash() returns an index based on s.
 */
static unsigned hash (
    const char *s
)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 *hashval;
    return hashval % cache_size;
}
