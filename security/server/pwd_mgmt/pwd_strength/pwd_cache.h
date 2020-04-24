/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pwd_cache.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:47:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:25  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:04  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:22  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:00:58  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:13  mdf]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
*/

#ifndef __pwd_cache_h_included__
#define __pwd_cache_h_included__

#include <dce/passwd.h>
#include <dce/rsec_pwd_mgmt.h>
#include <pthread.h>
#include <time.h>

/*
 * cache_size is the size of the table
 * 100 is the default.
 */
extern signed32 cache_size;

/*
 * timeout is the maximum number of seconds a generated
 * password should stay in the cache. Any strength checks
 * on a generated password longer than this timeout should
 * automatically fail!
 */
extern signed32 timeout;

/*
 * cache_mutex is a pthread mutex lock used when a thread
 * is accessing the cache. It is used for writing *and*
 * reading because writing while someone is reading can
 * be disasterous.
 */
extern pthread_mutex_t cache_mutex;

/*
 * struct which holds the generated passwords
 */
typedef struct field {
    int              num_pwds;
    sec_passwd_rec_t **pwd;
} *field_t;

typedef field_t *field_p_t;

/*
 * struct which holds a record containing generated
 * passwords for a user.
 */
typedef struct record {
    char                    *who;
    time_t                  last_update;
    unsigned32              num_pwds;
    sec_passwd_rec_t        *gen_pwd_set;
    struct record           *next;    /* for duplicate hashvals */
} rec_t;

typedef rec_t *rec_p_t;

/*
 * table holds all of the password records
 */
rec_p_t *table;

/* function prototypes */
void create_table(error_status_t *);
static unsigned hash(const char *);
void create_record (
    char *, 
    unsigned32,
    sec_passwd_rec_t *, 
    error_status_t *
);
void cleanup_table(error_status_t *);
boolean32 check_cache(
    char *, 
    sec_passwd_rec_t *, 
    boolean32 *, 
    error_status_t *
);

#endif
