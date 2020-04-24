/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: secidmap.c,v $
 * Revision 1.1.16.1  1996/10/03  14:48:55  arvind
 * 	silence compiler warnings (variables live across setjmp must be
 * 	volatile).
 * 	[1996/09/16  19:29 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.14.4  1996/02/18  00:07:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:51  marty]
 * 
 * Revision 1.1.14.3  1996/02/07  17:48:18  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:14:04  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.9.2  1993/08/19  15:10:46  rps
 * 	[OT7805/CHFts07506] sec__id_parse_name:
 * 	translate null input to "/.:" for tmp compatibility
 * 	[1993/08/19  14:22:27  rps]
 * 
 * Revision 1.1.7.8  1993/03/24  19:20:26  sommerfeld
 * 	[OT7502] disallow null global name in cache.
 * 	[1993/03/23  22:19:20  sommerfeld]
 * 
 * Revision 1.1.7.7  1993/02/19  23:37:57  tom
 * 	[OT7248] Don't touch cache unless we hold cache lock.
 * 	         When invalidating cache, set it in login context, not in
 * 	    binding handle (which probably toasts a valid pointer somewhere).
 * 	         Always lock cache in find_cache.
 * 	         Add assert for global name being non-null.
 * 	         Always include pthread.h so we get wrapped functions.
 * 	[1993/02/19  23:16:30  tom]
 * 
 * Revision 1.1.7.6  1993/02/11  23:17:14  sommerfeld
 * 	[OT7204] Fix bug in cache invalidation code which could result in
 * 	trashed memory.
 * 	[1993/02/11  17:33:23  sommerfeld]
 * 
 * Revision 1.1.7.5  1993/02/05  22:45:32  sommerfeld
 * 	[OT 5061] Add name cache invalidation support.
 * 	[1993/02/04  22:25:36  sommerfeld]
 * 
 * Revision 1.1.7.4  1992/12/29  13:04:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:10  zeliff]
 * 
 * Revision 1.1.7.3  1992/12/17  19:44:55  burati
 * 	CR5872 NULL -> '\0' in NEXT_FIELD macro
 * 	[1992/12/17  19:32:17  burati]
 * 
 * Revision 1.1.7.2  1992/09/29  21:16:44  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:20:29  sekhar]
 * 
 * Revision 1.1.2.5  1992/07/08  20:53:58  sommerfeld
 * 	Zero out output parameters so caller doesn't see garbage on output.
 * 	[CR4652] don't make unprotected references to static variables.
 * 	[1992/07/08  20:14:51  sommerfeld]
 * 
 * 	[CR4374] Don't cache result if server returned an error (we used to
 * 	cache it anyway, resulting in garbage results).
 * 	Also, don't do an in-memory cache of a zero-length returned string.
 * 	[1992/07/08  16:32:18  sommerfeld]
 * 
 * Revision 1.1.2.4  1992/06/18  21:19:58  sommerfeld
 * 	Call the new function "rca_expand_default_handle" rather than
 * 	calling rca_lock_handle and then unlocking it..
 * 	[1992/06/10  22:46:26  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/05/26  20:43:50  sommerfeld
 * 	Fix typo in name length checking.
 * 	Fix null rcontext handling [CR3722]
 * 	Don't crash on excessively long names.
 * 	Deal cleanly with attempts to abuse the @-hack.
 * 	[1992/05/26  17:54:02  sommerfeld]
 * 
 * 	Increase size of input buffer in read_cache to be 4*sec_rgy_name_t.
 * 	[1992/05/22  20:24:39  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/05/08  20:56:02  sommerfeld
 * 	Fix sloppy structure member reference.
 * 	[1992/04/30  16:50:23  sommerfeld]
 * 
 * 	Enable and fix name cache.
 * 	[1992/04/29  03:06:03  sommerfeld]
 * 
 * 	Disable cache for now (it's on hold).
 * 	[1992/04/17  22:19:23  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:47:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  secidmap.c V=13 12/13/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
** DCE Security - map a global name to cell/principal name and uuid
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)secidmap.c	13 - 12/13/91";
#endif

#include <pthread.h>
#include <stdio.h>
#include <sys/file.h>
#include <sec_login_pvt.h>
#include <assert.h>
#include <dce/dce_cf.h>
#include <dce/secidmap.h>
#include <rsecidmap.h>

#include <rca_pvt.h>
#include <u_str.h>
#include <un_maloc.h>

#define Crsec_id_parse_name(CH,b,c,d,e,f,g,h,i) \
        (*secidmap_v1_0_c_epv.rsec_id_parse_name) (HANDLE(CH),b,c,d,e,f,g,h,i)

#define Crsec_id_gen_name(CH,b,c,d,e,f,g,h,i) \
        (*secidmap_v1_0_c_epv.rsec_id_gen_name) (HANDLE(CH),b,c,d,e,f,g,h,i)

#define Crsec_id_parse_name_cache(CH,b,c,d,e,f,g,h,i,j) \
        (*secidmap_v1_0_c_epv.rsec_id_parse_name_cache) (HANDLE(CH),b,c,d,e,f,g,h,i,j)

#define Crsec_id_gen_name_cache(CH,b,c,d,e,f,g,h,i,j) \
        (*secidmap_v1_0_c_epv.rsec_id_gen_name_cache) (HANDLE(CH),b,c,d,e,f,g,h,i,j)

/*
 * parse bypass. !!! cut/paste sshared with rs_secidmap.c for now..
 */
typedef void (*sec_id_parse_bypass_fn_t)(
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              global_namep,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              cell_namep,
    uuid_t                      *cell_idp,
    sec_rgy_name_t              princ_namep,
    uuid_t                      *princ_idp,
    error_status_t              *status
);

typedef void (*sec_id_gen_bypass_fn_t)(
    sec_rgy_domain_t            domain,
    uuid_t                      *cell_idp,
    uuid_t                      *princ_idp,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              global_namep,
    sec_rgy_name_t              cell_namep,
    sec_rgy_name_t              princ_namep,
    error_status_t              *status
);

void sec_id_bypass (unsigned char *cell_name,
    sec_id_parse_bypass_fn_t parse,
    sec_id_gen_bypass_fn_t gen);


/*
 * Caching:
 * 
 * We take advantage of a name parsing cache in an attempt to speed
 * things up massively.
 * 
 * The name cache is attached to the login context in the rgy handle;
 * we call a sec_login_pvt entry point to fetch the pathname of the cache.
 * 
 * For simplicity, the cache is a plain ASCII text file.
 * Each cache entry is a single line in the file, with fields
 * separated by a Control-\ separator.
 * 
 * (names or results containing control-\ or \n are not cached.  
 * If this becomes an issue for i18n or ebcdic, we can revisit it).
 * 
 * Each line contains the following fields, in order:
 * timestamp    (time cache entry was added, as unix time_t expressed
 * in hex)
 * domain       ("U" or "G", for "user" or "group")
 * global name  (character string)
 * cell name    (character string)
 * cell id      (uuid, in string form)
 * princ name   (character string)
 * princ id     (uuid, in string form)
 * 
 * Absent fields are entered as null strings (e.g., nothing in between
 * the separators).
 * 
 * We read the cache once in each process on the first call to parse_name.
 * If we experience a cache miss and allocate a new entry, we append
 * to the file.
 * 
 * Note that no negative caching is done; this could be a useful
 * feature in the future.
 * 
 * We could attempt to re-parse the cache on every cache miss,
 * assuming the file changed since it was read, but that is left as a
 * future enhancement.
 * 
 * In memory, we keep the cache as a singly linked list of entries, as
 * we never need to delete entries out of the middle of the list.
 * 
 * We may also wish to ignore name->uuid translations for uuids
 * which are older than some timeout period.
 * 
 * Cache entries are allocated in a single malloced block, with
 * internal pointers.
 */

#define FS '\034'               /* ASCII "field separator" */

#define sec_rgy_domain_membership 4

typedef struct cacheentry {
    struct cacheentry *next;
    unsigned32 timestamp;
    sec_rgy_domain_t domain;
    int id_valid;               /* flag word */
#define CELL_ID_VALID           (1<<0)
#define PRINC_ID_VALID          (1<<1)
    unsigned_char_p_t global_name; 
    unsigned_char_p_t cell_name; 
    unsigned_char_p_t princ_name;
    uuid_t cell_id;
    uuid_t princ_id;
    unsigned char buf[1];
} cacheentry_t, *cacheentry_p_t;

#define SEC_CACHE_DEBUG 1
/*
 * cache control:
 */

int cache_enabled = 1;

/* 
 * Caching statistics:
 */

int cache_lookups = 0;
int cache_hits = 0;
int cache_adds = 0;

#if SEC_CACHE_DEBUG
int sec_name_cache_trace = 0;
#define SEC_DPRINTF(l,x) do {if (sec_name_cache_trace>=(l)) printf x; } while(0)
#else
#define SEC_DPRINTF(l,x)
#endif

static pthread_mutex_t cache_lock;
static pthread_once_t   cache_mutex_once = pthread_once_init;
static mtx_inited = 0;
static char *this_cell = NULL;
static error_status_t this_cell_status = sec_rgy_server_unavailable;
static cacheentry_p_t default_cache;
static char *default_cache_name;
static unsigned char *bypass_cell = NULL;
static sec_id_parse_bypass_fn_t bypass_parse = NULL;
static sec_id_gen_bypass_fn_t bypass_gen = NULL;

static unsigned char null_entry[2] = { '@', 0 };

static void cache_mutex_init(void)
{
    char *new_name;
    error_status_t st;
    char *cp;    
    pthread_mutex_init(&cache_lock, pthread_mutexattr_default);
    mtx_inited = 1;

    /*
     * Obtain the local cell name, since we don't know it yet.
     */
    dce_cf_get_cell_name(&new_name, &st);
    if (st == dce_cf_st_ok) {
	this_cell = new_name;
	this_cell_status = error_status_ok;
    }
    else this_cell_status = st;
    default_cache = NULL;
    default_cache_name = SEC_LOGIN_CREDS_DIR "/default.nc";
    cp = getenv("SEC_NAME_CACHE_DEBUG");
    if (cp) 
    {
	sec_name_cache_trace = atoi(cp);
	if (sec_name_cache_trace == 0)
	    sec_name_cache_trace = 1;
    }
}

void sec_id_bypass (unsigned char *cell,
		    sec_id_parse_bypass_fn_t parse,
		    sec_id_gen_bypass_fn_t gen)
{
    if (bypass_cell != NULL)
	return;
    bypass_cell = (unsigned char *)strdup((char *)cell);
    bypass_parse = parse;
    bypass_gen = gen;
}

#define CACHE_MUTEX_INIT() if (!mtx_inited) pthread_once(&cache_mutex_once, cache_mutex_init)
#ifdef IDL_PROTOTYPES
#define PROTOTYPE(x) x
#else
#define PROTOTYPE(x) ()
#endif


PRIVATE cacheentry_p_t allocate_namecache 
    PROTOTYPE ((
	unsigned32 timestamp,
	sec_rgy_domain_t domain,
	unsigned_char_p_t global_name,
	unsigned_char_p_t cell_name,
	uuid_t *cell_idp,
	unsigned_char_p_t princ_name,
	uuid_t *princ_idp
    ));

PRIVATE cacheentry_p_t parse_cache_line
    PROTOTYPE ((
	unsigned char *line
    ));

PRIVATE cacheentry_p_t read_cache
    PROTOTYPE ((
	char *filename
    ));

PRIVATE boolean32 cache_lookup_name
    PROTOTYPE ((
	cacheentry_p_t cache,
	sec_rgy_domain_t domain,
	rsec_id_output_selector_t selector,
	unsigned_char_p_t global_name,
	unsigned_char_p_t cell_name,
	uuid_t *cell_idp,
	unsigned_char_p_t princ_name,
	uuid_t *princ_idp
    ));

PRIVATE boolean32 cache_check_membership
    PROTOTYPE ((
	cacheentry_p_t cache,
	unsigned_char_p_t group_cell_name,
	unsigned_char_p_t group_group_name,
	unsigned_char_p_t princ_name
    ));

PRIVATE boolean32 cache_gen_name
    PROTOTYPE ((
	cacheentry_p_t cache,
	sec_rgy_domain_t domain,
	rsec_id_output_selector_t selector,
	uuid_t *cell_idp,
	uuid_t *princ_idp,
	unsigned_char_p_t global_name,
	unsigned_char_p_t cell_name,
	unsigned_char_p_t princ_name
    ));

PRIVATE boolean32 output_cache_line
    PROTOTYPE ((
	int file,  /* fileno */
	cacheentry_p_t cp
    ));

PRIVATE void add_namecache
    PROTOTYPE ((
	sec_rgy_bind_auth_info_t *auth_info,
	sec_rgy_domain_t domain,
	unsigned_char_p_t global_name,
	unsigned_char_p_t cell_name,
	uuid_t *cell_idp,
	unsigned_char_p_t princ_name,
	uuid_t *princ_idp,
	unsigned32 timestamp
    ));

PRIVATE boolean32 find_cache
    PROTOTYPE ((
	sec_rgy_bind_auth_info_t *auth_info,
	cacheentry_p_t *cachep,
	char **cachenamep
    ));

PRIVATE void free_cache_chain
    PROTOTYPE ((
	cacheentry_p_t cache
    ));


PUBLIC void sec__id_free_cache
    PROTOTYPE ((
	sec_login_handle_t *lhp
    ));




/*
 * Allocate a name cache entry, in a single malloced block.
 */

PRIVATE cacheentry_p_t allocate_namecache ( timestamp, domain, global_name, cell_name, cell_idp, princ_name, princ_idp)
    unsigned32 timestamp;
    sec_rgy_domain_t domain;
    unsigned_char_p_t global_name;
    unsigned_char_p_t cell_name;
    uuid_t *cell_idp;
    unsigned_char_p_t princ_name;
    uuid_t *princ_idp;
{
    cacheentry_p_t cp;
    int global_len=0, cell_len=0, princ_len=0;
    unsigned char *bufptr;
    int extra_space;

#define SET_LENGTH(name, len) \
    if ((name) != NULL)  \
    { \
	(len) = u_strlen(name); \
	if (len) (len)++; \
	else (name) = NULL; \
	if ((len) >= sizeof(sec_rgy_name_t)) return NULL; \
    }

#define SET_LENGTH_1(name, len) \
    if ((name) != NULL)  \
    { \
	(len) = u_strlen(name); \
	if (len) (len)++; \
	else { (name) = null_entry; len = 2; } \
	if ((len) >= sizeof(sec_rgy_name_t)) return NULL; \
    }

    SET_LENGTH(global_name, global_len);
    SET_LENGTH_1(princ_name, princ_len);
    SET_LENGTH_1(cell_name, cell_len);

#undef SET_LENGTH
    
    if (global_name == NULL) 
	return NULL;		/* disallow entries missing global name */

    extra_space = global_len + cell_len + princ_len;
    
    cp = (cacheentry_p_t) malloc (sizeof(*cp) + extra_space);

    if (!cp)
        return NULL;
    
    cp->next = NULL;
    cp->timestamp = timestamp;
    cp->domain = domain;
    cp->id_valid = 0;
    cp->global_name = 0;
    cp->cell_name = 0;
    cp->princ_name = 0;
    
    SEC_DPRINTF(3,("Allocating cache entry for %s; has %s%s%s%s\n", global_name,
	cell_idp? "cell-id ": "",
	princ_idp? "princ-id ": "",
	cell_name? "cell-name " : "",
	princ_name? "princ-name ": ""
    ));
    if (cell_idp) 
    {
        cp->id_valid |= CELL_ID_VALID;
        cp->cell_id = *cell_idp;
    }
    
    if (princ_idp) 
    {
        cp->id_valid |= PRINC_ID_VALID;
        cp->princ_id = *princ_idp;
    }
    bufptr = &cp->buf[0];
    
    if (global_name) {
        cp->global_name = bufptr;
        memcpy (bufptr, global_name, global_len);
        bufptr += global_len;
    }
    if (cell_name) {
        cp->cell_name = bufptr;
        memcpy(bufptr, cell_name, cell_len);
        bufptr += cell_len;
    }
    if (princ_name) {
        cp->princ_name = bufptr;
        memcpy(bufptr, princ_name, princ_len);
        bufptr += princ_len;
    }
    return cp;
}

/*
 * Parse a cache line.
 * Writes into "line" (to simplify coding).
 */

PRIVATE cacheentry_p_t parse_cache_line(line)
    unsigned char *line;
{
    unsigned char *cp = line;
    cacheentry_p_t cep;
    unsigned_char_p_t global_name = 0, cell_name = 0, princ_name = 0;
    uuid_p_t cell_id_p = 0;
    uuid_p_t princ_id_p = 0;
    uuid_t cell_id;
    uuid_t princ_id;
    unsigned32 timestamp;
    error_status_t st;
    sec_rgy_domain_t domain;
    
#define NEXT_FIELD     \
    do { line = cp; cp = (unsigned char *)u_strchr (line, FS); if (!cp) return NULL; *cp++ = '\0'; \
	 if (*cp==' ') cp++;  \
    } while (0)
    
    NEXT_FIELD;
    
    timestamp = strtoul((char *)line, NULL, 16);

    NEXT_FIELD;
    
    switch (*line) {
    case 'U':
        domain = sec_rgy_domain_person;
        break;
    case 'G':
        domain = sec_rgy_domain_group;
        break;
    case 'M':
	domain = sec_rgy_domain_membership;
	break;
    default:
        return NULL;
    }

    NEXT_FIELD;

    if (*line) 
        global_name = line;
    else
	return NULL;		/* disallow entries missing global name */
    
    NEXT_FIELD;
    
    if (*line)
        cell_name = line;
    
    NEXT_FIELD;
    
    if (*line)  
    {
        cell_id_p = &cell_id;
        uuid_from_string (line, cell_id_p, &st);
        if (st != error_status_ok)
            return NULL;
    }
    
    NEXT_FIELD;
    
    if (*line)
        princ_name = line;
    
    NEXT_FIELD;
    
    if (*line)  
    {
        princ_id_p = &princ_id;
        uuid_from_string (line, princ_id_p, &st);
        if (st != error_status_ok)
            return NULL;
    }
    
    return allocate_namecache (timestamp, domain, 
        global_name, cell_name, cell_id_p, princ_name, princ_id_p);
}

/* 
 * read cache file from disk.
 */

PRIVATE cacheentry_p_t read_cache(filename)
    char *filename;
{
    unsigned char buf[4 * sizeof(sec_rgy_name_t) + 256];
    cacheentry_p_t cur, prev = NULL;
    FILE *file;

    file = fopen (filename, "r");
    if (!file)
	return NULL;
    
    while (fgets((char *)buf, sizeof(buf), file) != NULL) 
    {
	cur = parse_cache_line (buf);
	if (cur) 
	{
	    cur->next = prev;
	    prev = cur;
	}
    }
    fclose (file);
    
    return prev;
}

PRIVATE boolean32 cache_lookup_name (
    cacheentry_p_t cache,
    sec_rgy_domain_t domain,
    rsec_id_output_selector_t selector,
    unsigned_char_p_t global_name,
    unsigned_char_p_t cell_name,
    uuid_t *cell_idp,
    unsigned_char_p_t princ_name,
    uuid_t *princ_idp
)
{
    cacheentry_p_t cp;
    
    cache_lookups++;

    SEC_DPRINTF(1,("Looking for %s\n", global_name));
	
    SEC_DPRINTF(2,("Fields: %s%s%s%s\n",
	FLAG_SET(selector, rsec_id_output_select_cname)?"cellname ":"",
	FLAG_SET(selector, rsec_id_output_select_cuuid)?"cellid ":"",
	FLAG_SET(selector, rsec_id_output_select_pname)?"princname ":"",
	FLAG_SET(selector, rsec_id_output_select_puuid)?"princid ":""));
    
    for (cp = cache; cp != NULL; cp = cp->next)
    {
	if (domain != cp->domain)
	    continue;

	assert(cp->global_name != NULL);

	if (u_strcmp(global_name, cp->global_name) != 0) 
	    continue;

	if (FLAG_SET(selector, rsec_id_output_select_cname))
	{
	    if (!cp->cell_name) 
		continue;

	    if (u_strcmp(cp->cell_name, "@") == 0)
		u_strcpy (cell_name, "");
	    else
		u_strcpy (cell_name, cp->cell_name);
	}
	if (FLAG_SET(selector, rsec_id_output_select_pname))
	{
	    if (!cp->princ_name) 
		continue;

	    if (u_strcmp(cp->princ_name, "@") == 0)
		u_strcpy (princ_name, "");
	    else
		u_strcpy (princ_name, cp->princ_name);
	}
	if (FLAG_SET(selector, rsec_id_output_select_cuuid))
	{
	    if (!(cp->id_valid & CELL_ID_VALID)) 
		continue;

	    *cell_idp = cp->cell_id;
	}
	if (FLAG_SET(selector, rsec_id_output_select_puuid))
	{
	    if (!(cp->id_valid & PRINC_ID_VALID)) 
		continue;

	    *princ_idp = cp->princ_id;
	}
	SEC_DPRINTF(1,("Cache hit\n"));
	cache_hits++;
	return true;
    }
    SEC_DPRINTF(1,("Cache miss\n"));
    return false;
}

PRIVATE boolean32 cache_check_membership (
    cacheentry_p_t cache,
    unsigned_char_p_t group_cell_name,
    unsigned_char_p_t group_group_name,
    unsigned_char_p_t princ_name
)
{
    cacheentry_p_t cp;
    sec_rgy_domain_t domain = sec_rgy_domain_membership;
    cache_lookups++;

    SEC_DPRINTF(1,("Looking for membership of %s in %s in cell %s \n",
		 princ_name, group_group_name, group_cell_name));
    
    for (cp = cache; cp != NULL; cp = cp->next)
    {
	if (domain != cp->domain)
	    continue;

	assert(cp->global_name != NULL);

	if (u_strcmp(group_cell_name, cp->global_name) != 0) 
	    continue;

	if (!cp->cell_name) 
	    continue;

	if (u_strcmp(group_group_name, cp->cell_name) != 0) 
	    continue;

	if (!cp->princ_name) 
	    continue;

	if (u_strcmp(princ_name, cp->princ_name) != 0) 
	    continue;

	SEC_DPRINTF(1,("Cache hit\n"));
	cache_hits++;
	return true;
    }
    SEC_DPRINTF(1,("Cache miss\n"));    
    return false;
}

PRIVATE boolean32 cache_gen_name (
    cacheentry_p_t cache,
    sec_rgy_domain_t domain,
    rsec_id_output_selector_t selector,
    uuid_t *cell_idp,
    uuid_t *princ_idp,
    unsigned_char_p_t global_name,
    unsigned_char_p_t cell_name,
    unsigned_char_p_t princ_name
)
{
    cacheentry_p_t cp;
    error_status_t xst;

    cache_lookups++;

    if (!cell_idp)
	return false;

    if ((!princ_idp) && (FLAG_SET(selector, rsec_id_output_select_pname)
			 || FLAG_SET(selector, rsec_id_output_select_gname)))
	return false;

    SEC_DPRINTF(2,("Fields: %s%s%s\n",
	FLAG_SET(selector, rsec_id_output_select_cname)?"cellname ":"",
	FLAG_SET(selector, rsec_id_output_select_pname)?"princname ":"",
	FLAG_SET(selector, rsec_id_output_select_gname)?"globalname ":""));

    for (cp = cache; cp != NULL; cp = cp->next)
    {
	if (domain != cp->domain)
	    continue;

	if (!(cp->id_valid & CELL_ID_VALID))
	    continue;

	if (!uuid_equal (cell_idp, &cp->cell_id, &xst))
	    continue;

	if (BAD_STATUS(&xst))
	    continue;
	
	if (princ_idp) 
	{
	    if (!(cp->id_valid & PRINC_ID_VALID))
		continue;

	    if (!uuid_equal (princ_idp, &cp->princ_id, &xst))
		continue;
	}

	if (BAD_STATUS(&xst))
	    continue;

	if (FLAG_SET(selector, rsec_id_output_select_gname))
	{
	    if (!cp->global_name) continue;
	    u_strcpy(global_name, cp->global_name);
	}
	if (FLAG_SET(selector, rsec_id_output_select_cname))	
	{
	    if (!cp->cell_name) 
		continue;

	    if (u_strcmp(cp->cell_name, "@") == 0)
		u_strcpy (cell_name, "");
	    else
		u_strcpy (cell_name, cp->cell_name);
	}
	if (FLAG_SET(selector, rsec_id_output_select_pname))	
	{
	    if (!cp->princ_name) 
		continue;

	    if (u_strcmp(cp->princ_name, "@") == 0)
		u_strcpy (princ_name, "");
	    else
		u_strcpy (princ_name, cp->princ_name);
	}
	cache_hits++;
	SEC_DPRINTF(1,("Cache hit\n"));	
	return true;
    }
    SEC_DPRINTF(1,("Cache miss\n"));
    return false;
}

	
/*
 * Output cache entry to a line in "file", with separators between fields.
 * 
 * If uuid_to_string fails, just omit the information from the cache.
 */

PRIVATE boolean32 output_cache_line (file, cp)
    int file; /* fileno */
    cacheentry_p_t cp;
{
    static unsigned char null[1] = { '\0' };
    error_status_t xst;
    unsigned_char_p_t cell_id_str = 0;
    unsigned_char_p_t princ_id_str = 0;
    static char buf[4 * sizeof(sec_rgy_name_t) + 256];
    int len, writelen;
    char *domstr;
    
    /* 
     * Check that FS doesn't appear in global_name,
     * cell_name, or princ_name; if so, we can cache it in core, but
     * not on disk (because we want a simple, robust cache format).
     */
    if (cp->global_name != NULL) 
    {
	if (u_strchr(cp->global_name, FS) != NULL)
	    return(0);
	if (u_strlen(cp->global_name) >= sizeof(sec_rgy_name_t))
	    return(0);
    }

    if (cp->cell_name != NULL)
    {
	if (u_strchr(cp->cell_name, FS) != NULL) 
	    return(0);
	if (u_strlen(cp->cell_name) >= sizeof(sec_rgy_name_t))
	    return(0);
    }
    if (cp->princ_name != NULL)
    {
	if (u_strchr(cp->princ_name, FS) != NULL)
	    return(0);
	
	if (u_strlen(cp->princ_name) >= sizeof(sec_rgy_name_t))
	    return(0);
    }
    switch (cp->domain) {
    case sec_rgy_domain_person:
	domstr = "U";
	break;
    case sec_rgy_domain_group:
	domstr = "G";
	break;
    case sec_rgy_domain_membership:
	domstr = "M";
	break;
    default:
	return 0;
    }
	
    if (cp->id_valid & CELL_ID_VALID )
	uuid_to_string (&cp->cell_id, &cell_id_str, &xst);
    if (xst)
	cell_id_str = NULL;
    if (cp->id_valid & PRINC_ID_VALID )
	uuid_to_string (&cp->princ_id, &princ_id_str, &xst);
    if (xst)
	princ_id_str = NULL;

    sprintf(buf, "%08x%c %s%c %s%c %s%c %s%c %s%c %s%c\n",
	    cp->timestamp, FS, 
	    domstr, FS,
	    cp->global_name?cp->global_name:null, FS,
	    cp->cell_name?cp->cell_name:null, FS,
	    cell_id_str?cell_id_str:null, FS,
	    cp->princ_name?cp->princ_name:null, FS,
	    princ_id_str?princ_id_str:null, FS);
    
    len = strlen(buf);
    lseek (file, 0L, SEEK_END);
    writelen = write (file, buf, len);

    if (cell_id_str)
	rpc_string_free(&cell_id_str, &xst);
    if (princ_id_str)
	rpc_string_free(&princ_id_str, &xst);
    return (len == writelen);
}    

/* 
 * Add entry to name cache:
 *  - allocate in-memory entry
 *  - append it to file, if exists
 *  - return new entry (with "next" link set appropriately.
 */

PRIVATE void add_namecache (
    sec_rgy_bind_auth_info_t *auth_info,
    sec_rgy_domain_t domain,
    unsigned_char_p_t global_name,
    unsigned_char_p_t cell_name,
    uuid_t *cell_idp,
    unsigned_char_p_t princ_name,
    uuid_t *princ_idp,
    unsigned32 timestamp)
{
    sec_login_context_t *lcp;
    unsigned32 now;
    cacheentry_p_t new;
    int outfile;
    char *filename;
    cacheentry_p_t cache;
    
    if (global_name == NULL)	/* disallow entries missing global name */
	return;
    
    new = allocate_namecache (timestamp, domain, global_name,
			      cell_name, cell_idp, princ_name, princ_idp);
    if (new == NULL)
	return;
    cache_adds++;

    CACHE_MUTEX_INIT();
    pthread_mutex_lock (&cache_lock);

    if (auth_info->info_type != sec_rgy_bind_auth_dce) 
    {
	new->next = default_cache;
	default_cache = new;
	filename = default_cache_name;
	lcp = NULL;
    } else {
	lcp = (sec_login_context_t *) auth_info->tagged_union.dce_info.identity;
	sec_login_pvt_get_namecache (lcp, (void **)&cache, &filename);
	new->next = cache;
	sec_login_pvt_set_namecache (lcp, (void *)new);
    }
    pthread_mutex_unlock (&cache_lock);

    if (filename != NULL)
    {
	error_status_t st;
        sec_login_pvt_open_namecache(lcp, filename, (O_CREAT | O_APPEND | O_WRONLY),
				     &outfile, &st);
	if (GOOD_STATUS(&st)) {
            output_cache_line (outfile, new);
            (void) close(outfile);
	}
    }
}

/*
 * find and load the cache, if it exists.
 * If we find the cache, we leave cache_lock locked and return true,
 * otherwise we return false.
 * If we return true, caller must release lock using
 * pthread_mutex_unlock(&cache_lock);
 */
PRIVATE boolean32 find_cache (
    sec_rgy_bind_auth_info_t    *auth_info,    
    cacheentry_p_t *cachep,
    char **cachenamep
    )
{
    sec_login_context_t *lcp = NULL;
    
    char *cachename;
    cacheentry_p_t cache;

    CACHE_MUTEX_INIT();
    pthread_mutex_lock (&cache_lock);
    if (auth_info->info_type != sec_rgy_bind_auth_dce) 
    {
	cache = default_cache;
	cachename = default_cache_name;
    } else {
	lcp = (sec_login_context_t *)auth_info->tagged_union.dce_info.identity;
	assert (lcp != NULL);
	sec_login_pvt_get_namecache (lcp, (void **)&cache, &cachename);
    }
    if ((cachename != NULL) && (cache == NULL)) 
    {
	SEC_DPRINTF(2,("%x: Reading cache\n", lcp));
	cache = read_cache (cachename);
	SEC_DPRINTF(2,("%x: Cache read in and set\n", lcp));
	if (lcp)
	    sec_login_pvt_set_namecache (lcp, cache);
	else
	    default_cache = cache;
    }
    *cachep = cache;
    if (cachenamep)
	*cachenamep = cachename;

    return true;
}

PRIVATE void free_cache_chain(cache)
    cacheentry_p_t cache;
{
    cacheentry_p_t cp, next;

    for (cp = cache; cp != NULL; cp = next) 
    {
	next = cp->next;
	free(cp);
    }
}

/* 
 * Free the cache attached to lcp, if any 
 */

PUBLIC void sec__id_free_cache(lhp)
    sec_login_handle_t *lhp;
{
    sec_login_context_t *lcp;
    char *cachename;
    cacheentry_p_t cache;
    
    CACHE_MUTEX_INIT();
    pthread_mutex_lock (&cache_lock);
    lcp=(sec_login_context_t *)lhp;
    sec_login_pvt_get_namecache (lcp, (void **)&cache, &cachename);
    sec_login_pvt_set_namecache (lcp, NULL);
    pthread_mutex_unlock (&cache_lock);    
    free_cache_chain(cache);
}

    

void invalidate_cache (sec_rgy_bind_auth_info_t *auth_info,
		       cacheentry_p_t cache,
		       unsigned_char_p_t current_cell_name,
		       char *cachename, unsigned32 *ts)
{
    cacheentry_p_t cp, prev, next;
    for (cp = cache; cp != NULL; cp = cp->next)
    {
	if ((cp->cell_name == NULL)
	    || (u_strcmp (cp->cell_name, current_cell_name) != 0))
		continue;
	
	if (cp->timestamp < ts[cp->domain]) 
	{
	    int invals = 0;
	    int outfile;
	    sec_login_context_t *lcp = NULL;

	    /*
	     * We have discovered a stale cache entry.
	     * Toss our in-memory copy and re-read the on-disk cache,
	     * which someone else may already have "fixed" for us..
	     */

	    if ((auth_info == NULL) || (auth_info->info_type != sec_rgy_bind_auth_dce))
	    {
		lcp = NULL;
		default_cache = NULL;
	    } else {
		lcp = (sec_login_context_t *) auth_info->tagged_union.dce_info.identity;
		sec_login_pvt_set_namecache (lcp, NULL);
	    }
	    free_cache_chain(cache);
	    
	    cache = read_cache (cachename);
	    
	    /*
	     * Now, do the inval for real.
	     */
	    
	    prev = NULL;
	    
	    for (cp = cache; cp != NULL; cp = next)
	    {
		next = cp->next;

		if ((cp->cell_name == NULL)
		    || (u_strcmp (cp->cell_name, current_cell_name) != 0)
		    || (cp->timestamp >= ts[cp->domain]))
		{
		    prev = cp;
		    continue;
		}

		/* this entry dies.. */
		invals++;
		if (prev)
		    prev->next = next;
		else
		    cache = next;
		free(cp);
	    }
	    if (invals > 0) 
	    {
		error_status_t st;

		if (lcp) 
		{
		    sec_login_pvt_set_namecache (lcp, cache);
		} else {
		    default_cache = cache;
		}
#if 0
		/* [1995/08/10  17:41:57  sommerfeld]
		 * This deletion is a possible fix to CHFts15856..
		 * Don't unlink here, as it will clobber the modes;
		 * O_TRUNC will truncate the file soon enough..
		 */
		(void) unlink (cachename);
#endif

		sec_login_pvt_open_namecache(lcp, cachename, (O_CREAT | O_TRUNC | O_WRONLY),
					     &outfile, &st);
		if (GOOD_STATUS(&st)) 
		{
		    for (cp = cache; cp != NULL; cp = cp->next) 
		    {
			if (!output_cache_line (outfile, cp))
			    break; /* oops... */
		    }
		    (void) close(outfile);
		}
	    }
	    break;
	}
    }
}



/*
 * s e c _ _ i d _ c h e c k _ c a c h e
 * 
 * Check the cache for stale data, if any.
 * Flush the cache if any are discovered.
 */

void sec__id_check_cache (context, cachehint)
    sec_rgy_handle_t context;
    rs_cache_data_t *cachehint;
{
    unsigned32 ts[sec_rgy_domain_last];
    cacheentry_p_t cache;
    char *cachename;
    unsigned char *current_cell_name;
    rs_cache_data_t *handle_cachehint;
    boolean32 is_locked;
    
    if (!cache_enabled)
	return;

    handle_cachehint = &(HANDLE_CACHEHINT(context));
    if ((handle_cachehint->person_dtm == cachehint->person_dtm) &&
	(handle_cachehint->group_dtm == cachehint->group_dtm) &&
	(handle_cachehint->org_dtm == cachehint->org_dtm))
    {
	return;
    }
    *handle_cachehint = *cachehint;
    
    /*
     * find cache, if any.
     */
    
    if (!find_cache (&AUTH_INFO(context), &cache, &cachename))
    {
        return;
    }

    if (!cache) 
    {
        pthread_mutex_unlock (&cache_lock);
	return;
    }
    /*
     * set up array of timestamps to make checking later on easier..
     */
    ts[sec_rgy_domain_person] = cachehint->person_dtm;
    ts[sec_rgy_domain_group] = cachehint->group_dtm;
    ts[sec_rgy_domain_org] = cachehint->org_dtm;

    current_cell_name = CELL_NAME (context);
    
    /*
     * See if we need to invalidate any cache entries..
     */
    
    invalidate_cache (&AUTH_INFO(context), cache, current_cell_name, cachename, ts);
    invalidate_cache (NULL, default_cache, current_cell_name, default_cache_name, ts);    
    pthread_mutex_unlock (&cache_lock);
}



    
/*
 * s e c _ _ i d _ p a r s e _ n a m e            AKA redbox
 *
 * Parse a global principal/group name into a cell name and a cell relative
 * principal name.  Return the IDs associated with each component.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped.
 */
PRIVATE void sec__id_parse_name (
    sec_rgy_bind_auth_info_t    * volatile auth_info,
    sec_rgy_domain_t    domain,
    unsigned char * volatile global_name,
    unsigned char * volatile cell_namep,
    uuid_t              * volatile cell_idp,
    unsigned char * volatile princ_namep,
    uuid_t              * volatile princ_idp,
    error_status_t      *status
  )
{
    sec_rgy_name_t              bogus_name;
    sec_rgy_name_t              bogus_cell_name;
    uuid_t                      bogus_uuid;
    sec_rgy_name_t		temp_global_name;
    sec_rgy_name_t		local_cell;
    volatile rsec_id_output_selector_t   selector = 0;
    unsigned long               st;
    sec_rgy_handle_t            context;
    volatile boolean32                   referral = false;
    error_status_t		xst;
    cacheentry_p_t		cache;
    volatile unsigned32			timestamp;
    sec_rgy_bind_auth_info_t	default_auth_info;
    
    CACHE_MUTEX_INIT();
    
    CLEAR_STATUS(status);

    /* initialize our output arguments to something sane */
    
    if (cell_idp)
	uuid_create_nil(cell_idp, &xst);

    if (princ_idp)
	uuid_create_nil(princ_idp, &xst);

    if (princ_namep)
	princ_namep[0] = 0;

    if (cell_namep)
	cell_namep[0] = 0;

    local_cell[0] = 0;
    
    /*
     * Preserve 1.0.1 compatibility by translating null names
     */
    if ((global_name == NULL) || (u_strlen(global_name) == 0))
	global_name = (unsigned char *) "/.:";

    if (u_strlen(global_name) >= sizeof (sec_rgy_name_t)) 
    {
	*status = sec_id_e_name_too_long;
	return;
    }
    
    if (cell_namep) {
        SET(selector, rsec_id_output_select_cname);
    } else {
        cell_namep = bogus_cell_name;
    }
    if (cell_idp) {
	uuid_create_nil(cell_idp, &xst);
        SET(selector, rsec_id_output_select_cuuid);
    } else {
        cell_idp = &bogus_uuid;
    }
    if (princ_namep) {
        SET(selector, rsec_id_output_select_pname);
    } else {
        princ_namep = bogus_name;
    }
    if (princ_idp) {
	uuid_create_nil (princ_idp, &xst);
        SET(selector, rsec_id_output_select_puuid);
    } else {
        princ_idp = &bogus_uuid;
    }

    if (selector == 0) {
        return;
    }

    /*
     * If we are only interested in the cell name, then check to see
     * if the global name names the local cell.  In this case we avoid
     * making a remote call.  We can also avoid a remote call if
     * we request a principal name, but it turns out to be the null string
     * - this happens when the global name only names the cell root.
     */
    if (FLAG_SET(selector, rsec_id_output_select_cuuid)
            || FLAG_SET(selector, rsec_id_output_select_puuid))
    {
	unsigned char *cp = (unsigned char *)u_strchr (global_name, '@');
	if (cp != NULL) 
	{
	    /* bypass the first referral stage.. */
	    u_strcpy(local_cell, "/.../");
	    u_strcat(local_cell, cp+1);
	    u_strcpy(temp_global_name, "/.../");
	    u_strcat(temp_global_name, cp+1);
	    u_strcat(temp_global_name, "/");
	    u_strncat(temp_global_name, global_name, cp-global_name);
	    global_name = temp_global_name;
	}
	
	if (this_cell == NULL) {
	    *status = this_cell_status;
	    return;
	}
	/*
	 * if we need uuids, we might as well get names, too.
	 */
	selector |= rsec_id_output_select_cname|rsec_id_output_select_pname;

	if (FLAG_SET(selector, rsec_id_output_select_puuid))
	    selector |= rsec_id_output_select_cuuid;
	    
	/*
	 * If global-name is relative, then qualify it with our idea
	 * of the local cell name, in case a cell rename is in progress,
	 * then retry the cache lookup.
	 */
        if (global_name[0] != PATH_COMPONENT_SEPARATOR) {
	    u_strcpy(temp_global_name, this_cell);
	    u_strcat(temp_global_name, "/");
	    u_strcat(temp_global_name, global_name);
	    global_name = temp_global_name;
	}
	/*
	 * Alternatively, if it starts with /.:, expand the cell name NOW
	 * so that we get correct cache behavior.
	 */
	else if ((u_strncmp(global_name,
			    CELL_DIR_ROOT, CELL_DIR_ROOT_LEN-1) == 0) &&
		 ((global_name[CELL_DIR_ROOT_LEN-1] == '\0') ||
		  (global_name[CELL_DIR_ROOT_LEN-1] == '/')))
	{
	    u_strcpy(temp_global_name, this_cell);
	    u_strcat(temp_global_name, &global_name[CELL_DIR_ROOT_LEN-1]);
	    global_name = temp_global_name;
	}
    }
    else
    {
	/*
	 * We don't need any uuids -- see if we can "cheat" and do
	 * this purely lexically.
	 */
        if (this_cell == NULL) {
	    *status = this_cell_status;
	    return;
	}
        if (u_strcmp(this_cell, global_name) == 0) {
            /*
             * The global name matches the local cell name, so simply copy
             * it to the cell name output parameter and return success.
             */
            u_strcpy(cell_namep, this_cell);
            princ_namep[0] = '\0';
            CLEAR_STATUS(status);
	    /* don't bother entering this into the cache */
            return;
        }
        /*
         * Check to see if the name is CELL_DIR_ROOT relative
         * note that the CELL_DIR_ROOT
         * constant includes the trailing separator character.
         */
        if (u_strncmp(global_name, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN-1) == 0) {
            /*
             * If the global name is simply the CELL_DIR_ROOT abbreviation
             * then return the real cell root name.
             */
            if (global_name[CELL_DIR_ROOT_LEN-1] == '\0') {
                u_strcpy(cell_namep, this_cell);
                princ_namep[0] = '\0';
                CLEAR_STATUS(status);
		/* don't bother caching this */
                return;
            }

            /*
             * If the global name includes a residual principal name pass
             * that back as well.
             */
            if (global_name[CELL_DIR_ROOT_LEN-1] 
                    == PATH_COMPONENT_SEPARATOR) {
                u_strcpy(cell_namep, this_cell);
                u_strcpy(princ_namep, &global_name[CELL_DIR_ROOT_LEN]);
                CLEAR_STATUS(status);
		/* don't bother caching this, either */
                return;
            }

            /*
             * If we reach here, the global name is of the form
             * '/.:garbage'.  This shouldn't be legal syntax, but we'll
             * let the server determine that fact.
             */
        }

        /*
         * If the global name is not an absolute name, then it is either
         * a local cell relative name, or it is a Kerberos name (contains
         * an '@' <princ>@<cell>)
         */
        if (global_name[0] != PATH_COMPONENT_SEPARATOR) {
            char    *p;

            p = u_strchr(global_name, '@');
            if (p != NULL) {
                *p = '\0';
                u_strcpy(princ_namep, global_name);
                *p = '@';
                u_strcpy(cell_namep, GLOBAL_DIR_ROOT);
                u_strcat(cell_namep, p+1);
            } else {
                u_strcpy(princ_namep, global_name);
                u_strcpy(cell_namep, this_cell);
            }
            CLEAR_STATUS(status);
	    /* don't cache this, either */
            return;
        }
    }

    if (auth_info == NULL) 
    {
	auth_info = &default_auth_info;
	auth_info->info_type = sec_rgy_bind_auth_dce;
	auth_info->tagged_union.dce_info.authn_level = rpc_c_authn_level_pkt_integrity;
	auth_info->tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
	auth_info->tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
	auth_info->tagged_union.dce_info.identity = NULL;
    }
    
    if ((auth_info->info_type == sec_rgy_bind_auth_dce) &&
	(auth_info->tagged_union.dce_info.identity == NULL))
    {
	auth_info->tagged_union.dce_info.identity = 
	    (sec_login_handle_t)sec_login_pvt_get_default_context(status);
	if (BAD_STATUS(status))
	    return;
    }
    

    if (cache_enabled) 
    {
	if (find_cache (auth_info, &cache, NULL))
	{
	    boolean32 is_there;
	    is_there = cache_lookup_name (cache, domain, selector, global_name,
					  cell_namep, cell_idp, princ_namep,
					  princ_idp);
	    pthread_mutex_unlock (&cache_lock);
	    if (is_there) 
	        return;
	}
    }

    if (local_cell[0] == 0)
	rca_get_cell_name(auth_info, local_cell);          

    if ((bypass_cell != NULL) &&
	(u_strcmp (local_cell, bypass_cell) == 0))
    {
	context = NULL;
    } else {
	rca_site_bind(local_cell, auth_info,
		      rca_NSI_cell, NULL,
		      SEC_RGY_SERVER_NAME,
		      (char *)local_cell, &context, status);

	if (BAD_STATUS(status)) 
	    return;
    }
    

    do {
	error_status_t lst;
	referral = false;
	
	if (context == NULL) 
	{
	    (*bypass_parse) (domain, global_name, selector,
			     cell_namep, cell_idp,
			     princ_namep, princ_idp,
			     status);
	    if (STATUS_EQUAL(status, sec_id_e_foreign_cell_referral)) {
		referral = true;
		CLEAR_STATUS(status);
	    }
	} else {
	    SETUP_RETRY(rca_op_read, status) {
		/*
		 * Try new call first; fall back to old call if new call not 
		 * supported.
		 */
		*status = rpc_s_op_rng_error;
		if (!(HANDLE_FLAGS(context) &
		      RCA_HANDLE_FLAGS_NO_NAME_CACHE_HINT))
		{
		    rs_cache_data_t cache_info;
		
		    Crsec_id_parse_name_cache (context, domain, global_name,
					       selector, cell_namep, cell_idp,
					       princ_namep, princ_idp,
					       &cache_info, status);
		    if (*status == error_status_ok) 
		    {
			CHECK_CACHE(context, &cache_info);
			if (domain == sec_rgy_domain_person)
			    timestamp = cache_info.person_dtm;
			else
			    timestamp = cache_info.group_dtm;
		    }
		} 
		if (*status == rpc_s_op_rng_error)
		{
		    time_t ts;
		    time(&ts);
		    timestamp = ts;
		    *status = error_status_ok;
		    Crsec_id_parse_name(context, domain, global_name,
					selector, cell_namep, cell_idp,
					princ_namep, princ_idp, status);
		    HANDLE_FLAGS(context) |=
			RCA_HANDLE_FLAGS_NO_NAME_CACHE_HINT;
		}

		NORMAL_CASE;
	    
		RETRY_CASE {
		    if (STATUS_EQUAL(status, sec_id_e_foreign_cell_referral)) {
			referral = true;
			CLEAR_STATUS(status);
			ABANDON_RETRY;  /* fall out of retry loop */
		    }
		}
	    }
	    RETRY_END;

	    /*
	     * Release the context - we won't need it anymore.  If we got
	     * another referral we will create a new context and if we
	     * finished here we'll return to the caller.  In both cases
	     * the current referral context is obsolete.
	     */
	    sec_rgy_site_close(context, &lst);
	}
	

	if (referral) {
	    /*
	     * We've gotten a refferal to a different cell.  Rebind to
	     * that cell and continue the resolution.
	     */
	    
	    rca_site_bind(cell_namep, auth_info,
			  rca_NSI_cell, NULL,
			  SEC_RGY_SERVER_NAME,
			  (char *) cell_namep, &context, status);

	    referral = GOOD_STATUS(status);
	}

    } while (referral);

    if (cache_enabled && GOOD_STATUS(status)) 
    {
	if (!FLAG_SET(selector, rsec_id_output_select_cuuid))
	    cell_idp = NULL;
	if (!FLAG_SET(selector, rsec_id_output_select_puuid))
	    princ_idp = NULL;	
	
	add_namecache (auth_info, domain, global_name, 
	    cell_namep, cell_idp, princ_namep, princ_idp, timestamp);
    }
}

/*
 * s e c _ i d _ p a r s e _ n a m e            AKA redbox
 *
 * Parse a global principal name into a cell name and a cell relative
 * principal name.  Return the IDs associated with each component.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped.
 */
PUBLIC void sec_id_parse_name (
    sec_rgy_handle_t    rcontext,
    sec_rgy_name_t      global_name,
    sec_rgy_name_t      cell_namep,
    uuid_t              *cell_idp,
    sec_rgy_name_t      princ_namep,
    uuid_t              *princ_idp,
    error_status_t      *status
  )
{
    sec_rgy_bind_auth_info_t    *auth_info = NULL;

    if (rcontext != NULL) 
    {
	rca_expand_default_handle(&rcontext, status);
	if (BAD_STATUS(status))
	    return;
	auth_info = &(AUTH_INFO(rcontext));
    }
    
    sec__id_parse_name(auth_info, sec_rgy_domain_person,
		       global_name, cell_namep, cell_idp,
		       princ_namep, princ_idp, status);
}

/*
 * s e c _ i d _ p a r s e _ n a m e            AKA redbox
 *
 * Parse a global principal name into a cell name and a cell relative
 * principal name.  Return the IDs associated with each component.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped.
 */
PUBLIC void sec_id_global_parse_name (
    sec_rgy_bind_auth_info_t    *auth_info,
    sec_rgy_name_t      global_name,
    sec_rgy_name_t      cell_namep,
    uuid_t              *cell_idp,
    sec_rgy_name_t      princ_namep,
    uuid_t              *princ_idp,
    error_status_t      *status
  )
{
    sec__id_parse_name(auth_info, sec_rgy_domain_person,
		       global_name, cell_namep, cell_idp,
		       princ_namep, princ_idp, status);
}


/*
 * s e c _ i d _ p a r s e _ g r o u p
 *
 * Parse a global principal name into a cell name and a cell relative
 * group name.  Return the IDs associated with each component.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped.
 */
 
PUBLIC void sec_id_parse_group (
    sec_rgy_handle_t    rcontext,
    sec_rgy_name_t      global_name,
    sec_rgy_name_t      cell_namep,
    uuid_t              *cell_idp,
    sec_rgy_name_t      group_namep,
    uuid_t              *group_idp,
    error_status_t      *status
  )
{
    sec_rgy_bind_auth_info_t    *auth_info = NULL;

    if (rcontext != NULL) 
    {
	rca_expand_default_handle(&rcontext, status);
	if (BAD_STATUS(status))
	    return;
	auth_info = &(AUTH_INFO(rcontext));
    }
    sec__id_parse_name(auth_info, sec_rgy_domain_group,
		       global_name, cell_namep, cell_idp,
		       group_namep, group_idp, status);
}

/*
 * s e c _ i d 1 _ p a r s e _ g r o u p
 *
 * Parse a global principal name into a cell name and a cell relative
 * group name.  Return the IDs associated with each component.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped.
 */
 
PUBLIC void sec_id_global_parse_group (
    sec_rgy_bind_auth_info_t    *auth_info,
    sec_rgy_name_t      global_name,
    sec_rgy_name_t      cell_namep,
    uuid_t              *cell_idp,
    sec_rgy_name_t      group_namep,
    uuid_t              *group_idp,
    error_status_t      *status
  )
{
    sec__id_parse_name(auth_info, sec_rgy_domain_group,
		       global_name, cell_namep, cell_idp,
		       group_namep, group_idp, status);
}

/*
 * s e c _ _ i d _ g e n _ n a m e              AKA bluebox
 *
 * Generate a global name from the cell and principal/group uuids.  Also returns
 * the name of the cell uuid and the principal uuid as independent units.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped.
 */
 
PRIVATE void sec__id_gen_name (
    sec_rgy_bind_auth_info_t    * volatile auth_info,
    sec_rgy_domain_t    domain,
    uuid_t              *cell_idp,
    uuid_t              *princ_idp,
    unsigned char * volatile global_namep,
    unsigned char * volatile cell_namep,
    unsigned char * volatile princ_namep,
    error_status_t      *status
  )
{
    sec_rgy_name_t              bogus_name;
    sec_rgy_name_t              bogus_princ_name;
    sec_rgy_name_t              bogus_cell_name;
    sec_rgy_name_t              local_cell;
    volatile rsec_id_output_selector_t   selector = 0;
    sec_rgy_handle_t            context;
    volatile boolean32                   referral = false;
    cacheentry_p_t		cache;
    volatile unsigned32			timestamp;
    sec_rgy_bind_auth_info_t	default_auth_info;

    CLEAR_STATUS(status);

    if (global_namep) {
        SET(selector, rsec_id_output_select_gname);
    } else {
        global_namep = bogus_name;
    }
    if (cell_namep) {
        SET(selector, rsec_id_output_select_cname);
    } else {
        cell_namep = bogus_cell_name;
    }
    if (princ_namep) {
        SET(selector, rsec_id_output_select_pname);
    } else {
        princ_namep = bogus_princ_name;
    }

    if (auth_info == NULL) 
    {
	auth_info = &default_auth_info;
	auth_info->info_type = sec_rgy_bind_auth_dce;
	auth_info->tagged_union.dce_info.authn_level = rpc_c_authn_level_pkt_integrity;
	auth_info->tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
	auth_info->tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
	auth_info->tagged_union.dce_info.identity = 
	    (sec_login_handle_t)sec_login_pvt_get_default_context(status);
	if (BAD_STATUS(status))
	    return;
    }

    if (cache_enabled) 
    {
	if (find_cache (auth_info, &cache, NULL))
	{
	    boolean32 is_there;
	    is_there = cache_gen_name (cache, domain, selector,
				       cell_idp, princ_idp,
				       global_namep, cell_namep, princ_namep);
	    pthread_mutex_unlock (&cache_lock);
	    if (is_there) 
	        return;
	}
    }

    if (FLAG_SET(selector, rsec_id_output_select_pname)) 
    {
	SET(selector, rsec_id_output_select_gname);
	SET(selector, rsec_id_output_select_cname);
    }

    if (FLAG_SET(selector, rsec_id_output_select_gname)) 
    {
	SET(selector, rsec_id_output_select_cname);
    }

    rca_get_cell_name(auth_info, local_cell);          

    if ((bypass_cell != NULL) &&
	(u_strcmp (local_cell, bypass_cell) == 0))
    {
	context = NULL;
    } else {
	rca_site_bind(local_cell, auth_info,
		      rca_NSI_cell, NULL,
		      SEC_RGY_SERVER_NAME,
		      (char *)local_cell, &context, status);
	if (BAD_STATUS(status))
	    return;
    }
    
    do {
	error_status_t lst;
	
	referral = false;

	if (context == NULL) 
	{
	    (*bypass_gen) (domain, cell_idp, princ_idp,
			   selector, global_namep, cell_namep, princ_namep,
			   status);
	    if (STATUS_EQUAL(status, sec_id_e_foreign_cell_referral)) {
		referral = true;
		CLEAR_STATUS(status);
	    }
	} else {
	    SETUP_RETRY(rca_op_read, status) {
		*status = rpc_s_op_rng_error;
		if (!(HANDLE_FLAGS(context) &
		      RCA_HANDLE_FLAGS_NO_NAME_CACHE_HINT))
		{
		    rs_cache_data_t cache_info;

		    Crsec_id_gen_name_cache(context, domain,
			cell_idp, princ_idp,
			selector, global_namep, cell_namep,
			princ_namep, &cache_info, status);
		    if (*status == error_status_ok)
		    {
			CHECK_CACHE(context, &cache_info);
			if (domain == sec_rgy_domain_person)
			    timestamp = cache_info.person_dtm;
			else
			    timestamp = cache_info.group_dtm;
		    }
		}
		if (*status == rpc_s_op_rng_error)
		{
		    time_t ts;
		    time(&ts);
		    timestamp = ts;
		    *status = error_status_ok;
		    Crsec_id_gen_name(context, domain, cell_idp, princ_idp,
			selector, global_namep, cell_namep,
			princ_namep, status);
		    HANDLE_FLAGS(context) |=
			RCA_HANDLE_FLAGS_NO_NAME_CACHE_HINT;
		}

		NORMAL_CASE;

		RETRY_CASE {
		    if (STATUS_EQUAL(status, sec_id_e_foreign_cell_referral)) {
			referral = true;
			CLEAR_STATUS(status);
			ABANDON_RETRY;  /* fall out of retry loop */
		    }
		}
	    }
	    RETRY_END;

	    /*
	     * Release the context - we won't need it anymore.  If we got
	     * a referral we will create a new context and if we finished
	     * here we'll return to the caller.  In both cases the current
	     * context is obsolete.
	     */
	    sec_rgy_site_close(context, &lst);
	}
	
	if (referral) {
	    /*
	     * We've gotten a referral to a different cell.  Rebind to
	     * that cell and continue the resolution.
	     */
	    
	    rca_site_bind(cell_namep, auth_info,
			  rca_NSI_cell, NULL, 
			  SEC_RGY_SERVER_NAME,
			  (char *) cell_namep, &context, status);
	    
	    referral = GOOD_STATUS(status);
	}
	
    } while (referral);

    if (cache_enabled && GOOD_STATUS(status))
    {
	/* don't cache trash */
	if (!FLAG_SET(selector, rsec_id_output_select_pname)) 
	    princ_namep = NULL;

	if (!FLAG_SET(selector, rsec_id_output_select_gname)) 
	    global_namep = NULL;

	if (!FLAG_SET(selector, rsec_id_output_select_cname)) 
	    cell_namep = NULL;

	add_namecache (auth_info, domain, global_namep,
		       cell_namep, cell_idp,
		       princ_namep, princ_idp, timestamp);
    }
}

/*
 * s e c _ i d _ g e n _ n a m e              AKA bluebox
 *
 * Generate a global name from the cell and principal uuids.  Also returns
 * the name of the cell uuid and the principal uuid as independent units.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped.
 */
 
PUBLIC void sec_id_gen_name (
    sec_rgy_handle_t    rcontext,
    uuid_t              *cell_idp,
    uuid_t              *princ_idp,
    sec_rgy_name_t      global_namep,
    sec_rgy_name_t      cell_namep,
    sec_rgy_name_t      princ_namep,
    error_status_t      *status
  )
{
    sec_rgy_bind_auth_info_t    *auth_info = NULL;

    if (rcontext != NULL) 
    {
	rca_expand_default_handle(&rcontext, status);
	if (BAD_STATUS(status))
	    return;
	auth_info = &(AUTH_INFO(rcontext));
    }
    
    sec__id_gen_name(auth_info, sec_rgy_domain_person, cell_idp, princ_idp, 
		     global_namep, cell_namep, princ_namep, status);
}

PUBLIC void sec_id_global_gen_name (
    sec_rgy_bind_auth_info_t    *auth_info,
    uuid_t              *cell_idp,
    uuid_t              *princ_idp,
    sec_rgy_name_t      global_namep,
    sec_rgy_name_t      cell_namep,
    sec_rgy_name_t      princ_namep,
    error_status_t      *status
  )
{
    sec__id_gen_name(auth_info, sec_rgy_domain_person, cell_idp, princ_idp, 
		     global_namep, cell_namep, princ_namep, status);
}

/*
 * s e c _ i d _ g e n _ g r o u p
 *
 * Generate a global name from the cell and group uuids.  Also returns
 * the name of the cell uuid and the group uuid as independent units.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped.
 */
 
PUBLIC void sec_id_gen_group (
    sec_rgy_handle_t    rcontext,
    uuid_t              *cell_idp,
    uuid_t              *group_idp,
    sec_rgy_name_t      global_namep,
    sec_rgy_name_t      cell_namep,
    sec_rgy_name_t      group_namep,
    error_status_t      *status
  )
{
    sec_rgy_bind_auth_info_t    *auth_info = NULL;

    if (rcontext != NULL) 
    {
	rca_expand_default_handle(&rcontext, status);
	if (BAD_STATUS(status))
	    return;
	auth_info = &(AUTH_INFO(rcontext));
    }
    
    sec__id_gen_name(auth_info, sec_rgy_domain_group, cell_idp, group_idp, 
		     global_namep, cell_namep,  group_namep, status);
}

PUBLIC void sec_id_global_gen_group (
    sec_rgy_bind_auth_info_t    *auth_info,
    uuid_t              *cell_idp,
    uuid_t              *group_idp,
    sec_rgy_name_t      global_namep,
    sec_rgy_name_t      cell_namep,
    sec_rgy_name_t      group_namep,
    error_status_t      *status
  )
{
    sec__id_gen_name(auth_info, sec_rgy_domain_group, cell_idp, group_idp, 
		     global_namep, cell_namep,  group_namep, status);
}

/*
 * s e c _ i d 1 _ i s _ g r o u p _ m e m b e r
 *
 * Implement a common DCE idiom: see if <princ_name>
 * is a member of <group_name>
 */
   

PUBLIC     boolean32 sec_id_global_is_group_member (
    sec_rgy_bind_auth_info_t    *auth_info,
    sec_rgy_name_t		group_name,
    sec_rgy_name_t		princ_name,
    error_status_t		*status
)
{
    boolean32 retval;
    sec_rgy_handle_t            context;
    error_status_t		ignored_status;
    /* gigundo stack frame here.. */
    sec_rgy_name_t 		princ_cell_name, princ_princ_name;
    sec_rgy_name_t 		group_cell_name, group_group_name;    
    uuid_t 			princ_cell_id, princ_princ_id;
    uuid_t 			group_cell_id, group_group_id;    
    sec_rgy_name_t 		tmp_group_name;
    unsigned32 			timestamp;
    time_t 			ts;
    cacheentry_p_t		cache;
    
    /* Figure out which cell we're talking about. */
    sec_id_global_parse_name (auth_info, princ_name,
			princ_cell_name, &princ_cell_id,
			princ_princ_name, &princ_princ_id, status);
    if (BAD_STATUS(status))
	return false;

    /* Ditto for the group.. */
    if (group_name[0] == '/') 
    {
	sec_id_global_parse_group (auth_info, group_name,
			     group_cell_name, &group_cell_id,
			     group_group_name, &group_group_id, status);
	if(BAD_STATUS(status))
	    return false;
	if (u_strcmp (group_cell_name, princ_cell_name) != 0) 
	{
	    *status = 0xdeadbeef; /* !!! */
	    return false;
	}
    } else {
	u_strcpy(group_group_name, group_name);
	u_strcpy(group_cell_name, princ_cell_name);
    }
    /* Look in the cache ... */
    if (cache_enabled) 
    {
	if (find_cache (auth_info, &cache, NULL))
	{
	    boolean32 is_there;
	    is_there = cache_check_membership (cache, group_cell_name,
					       group_group_name,
					       princ_princ_name);
	    pthread_mutex_unlock (&cache_lock);
	    if (is_there) 
	        return true;
	}
    }

    time(&ts);
    timestamp = ts;
    /* bind to the group's cell, and ask it.. */
    rca_site_bind (group_cell_name, auth_info,
		   rca_NSI_cell, NULL, 
		   SEC_RGY_SERVER_NAME,
		   (char *)group_cell_name, &context, status);
    if (BAD_STATUS(status))
	return false;

    retval = sec_rgy_pgo_is_member (context, sec_rgy_domain_group,
				    group_group_name, princ_princ_name,
				    status);

    sec_rgy_site_close (context, &ignored_status);

    if (BAD_STATUS(status))
	return false;

    if (cache_enabled) {
	add_namecache (auth_info, sec_rgy_domain_membership,
		       group_cell_name, group_group_name, NULL,
		       princ_princ_name, NULL, timestamp);
    }
    return retval;
}


PUBLIC     boolean32 sec_id_is_group_member (
    sec_rgy_handle_t    rcontext,
    sec_rgy_name_t		group_name,
    sec_rgy_name_t		princ_name,
    error_status_t		*status
)
{
    sec_rgy_bind_auth_info_t    *auth_info = NULL;

    if (rcontext != NULL) 
    {
	rca_expand_default_handle(&rcontext, status);
	if (BAD_STATUS(status))
	    return false;
	auth_info = &(AUTH_INFO(rcontext));
    }
    
    return sec_id_global_is_group_member (auth_info, group_name, princ_name, status);
}
