/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_cf.c,v $
 * Revision 1.1.17.2  1996/02/18  19:21:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:40  marty]
 *
 * Revision 1.1.17.1  1995/12/08  21:36:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/18  20:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts15236.
 * 
 * 	HP revision /main/tatsu_s.dced_security.b0/1  1995/05/13  01:21 UTC  tatsu_s
 * 	Made thread-safe.
 * 	[1995/12/08  18:08:36  root]
 * 
 * Revision 1.1.15.12  1994/09/02  19:54:07  bowe
 * 	Better error check of rpc_ns_entry_expand_name() [CR 11843]
 * 	[1994/09/02  19:47:02  bowe]
 * 
 * Revision 1.1.15.11  1994/09/01  15:22:09  bowe
 * 	Based on feedback by Joe Pato, canonicalize cell names with
 * 	rpc_ns_* calls. (lots of small changes from this)  [CR 11843]
 * 	[1994/09/01  15:21:52  bowe]
 * 
 * Revision 1.1.15.10  1994/08/26  18:11:28  bowe
 * 	Do not downcase cell names (per Joe Pato).  Pass them through
 * 	dnsExpandStrFull() instead.  [CR 11806]
 * 	[1994/08/26  18:09:44  bowe]
 * 
 * Revision 1.1.15.9  1994/08/25  14:26:08  bowe
 * 	Better error checking when reading file (It should be OK if
 * 	cellaliases are not there).  [CR 11879]
 * 	[1994/08/25  14:25:46  bowe]
 * 
 * Revision 1.1.15.8  1994/08/24  14:38:33  bowe
 * 	Implement dce_cf_get_cell_aliases(), dce_cf_free_cell_aliases(),
 * 	and dce_cf_same_cell_name(). Check file mod time before re-reading
 * 	cache.  Adjust test code. [CR 11843]
 * 	Don't downcase DCE hostname. [CR 11806]
 * 	[1994/08/24  14:37:15  bowe]
 * 
 * Revision 1.1.15.7  1994/07/28  14:03:44  rsalz
 * 	Use dce_strdup (OT CR 11343).
 * 	[1994/07/28  13:42:20  rsalz]
 * 
 * Revision 1.1.15.6  1994/07/27  17:28:16  rsalz
 * 	Add STRICT_COMPILER_WARNINGS and cleanup resultant output.
 * 	[1994/07/27  17:27:52  rsalz]
 * 
 * Revision 1.1.15.5  1994/06/09  16:05:41  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:18  devsrc]
 * 
 * Revision 1.1.15.4  1994/01/27  21:26:38  bowe
 * 	removed #define dcelocal_path.  (it's gotten from dce.h)
 * 	[1994/01/27  21:25:15  bowe]
 * 
 * Revision 1.1.15.3  1994/01/11  20:39:02  rsalz
 * 	Use default filename for csrgyfile entry (OT CR 9696).
 * 	[1994/01/11  20:38:41  rsalz]
 * 
 * Revision 1.1.15.2  1994/01/10  20:04:23  rsalz
 * 	Remove dcecfg.{cat,msg} (OT CR 9692).
 * 	Write dce_cf_get_csrgy_filename (OT CR 9696).
 * 	[1994/01/10  19:55:25  rsalz]
 * 
 * Revision 1.1.15.1  1993/12/29  19:22:28  rsalz
 * 	Move dce_cf_XXX from config to dce/utils/dcecf (OT CR 9663).
 * 	Rewrite to fix various bugs (OT CR 9665).
 * 	[1993/12/29  16:17:54  rsalz]
 * 
 * $EndLog$
 */
/*
**  Routines to get various names (hostname, cell name, etc.) from a local
**  database.
**
**  Note that we ignore thread errors here.
*/
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dce/dce.h>
#include <dce/dce_utils.h>
#include <dce/dce_cf.h>
#include <dce/binding.h>
#include <dce/secidmap.h>

#define PRIVATE				static
#define dce_cf_c_ps_buff_size		1024
#define dce_cf_c_base_db_name		"dce_cf.db"
#define dce_cf_c_default_csrgy_filename	"/usr/lib/nls/csr/code_set_registry.db"


PRIVATE unsigned_char_t	*dce_cf_g_cellname;
PRIVATE char		*dce_cf_g_hostname;
PRIVATE char		*dce_cf_g_csrgy_filename;
PRIVATE unsigned_char_p_t	*dce_cf_g_cellaliases;
PRIVATE int		dce_cf_g_cellaliases_count;
PRIVATE pthread_mutex_t	dce_cf_g_mutex;

/* ______________________________________________________________________ */

/*
**  The pthread_once initialization function.
*/
PRIVATE void
dce_cf__once(
    void
)
{
    pthread_mutex_init(&dce_cf_g_mutex, pthread_mutexattr_default);
}

/* ______________________________________________________________________ */

/*
**  Load the cache.
*/
PRIVATE void
dce_cf__load_cache(
    error_status_t		*status
)
{
    static pthread_once_t	once = pthread_once_init;
    FILE			*F;
    unsigned char		*new;
    char			file[CELL_NAME_LEN];
    unsigned_char_p_t		*cv;
    unsigned_char_t		*expanded_name;
    struct stat			statbuf;
    static time_t		dce_cf_g_modtime;

    /* Create our mutex, just once. */
    pthread_once(&once, dce_cf__once);

    (void)sprintf(file, "%s/%s", dcelocal_path, dce_cf_c_base_db_name);
    if (stat(file, &statbuf) == -1) {
	*status = dce_cf_e_file_open;
	return;
    }
    *status = dce_cf_st_ok;

#ifndef	HPDCE_CF_THREAD_SAFE
    /* Lock others out. */
    pthread_mutex_lock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
    /* Check modify time on file.  If not new, return. */
    if (dce_cf_g_modtime != 0 && dce_cf_g_modtime == statbuf.st_mtime) {
#ifndef	HPDCE_CF_THREAD_SAFE
	pthread_mutex_unlock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
	return;
    }
    dce_cf_g_modtime = statbuf.st_mtime;

#ifdef	HPDCE_CF_THREAD_SAFE
    /* Lock others out.  See if someone set things up while we were waiting. */
    pthread_mutex_lock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */

    /* Initialize. */
    if (dce_cf_g_cellname)
	rpc_string_free(&dce_cf_g_cellname, status);
    if (dce_cf_g_hostname) {
	free(dce_cf_g_hostname);
	dce_cf_g_hostname = NULL;
    }
    if (dce_cf_g_csrgy_filename) {
	free(dce_cf_g_csrgy_filename);
	dce_cf_g_csrgy_filename = NULL;
    }
    if (dce_cf_g_cellaliases) {
	for (cv = dce_cf_g_cellaliases; *cv; cv++)
	    rpc_string_free(cv, status);
	free(dce_cf_g_cellaliases);
	dce_cf_g_cellaliases_count = 0;
	dce_cf_g_cellaliases = NULL;
    }

    /* Open config file. */
    if ((F = fopen(file, "r")) == NULL) {
	*status = dce_cf_e_file_open;
	goto Done;
    }

    /* Get the cellname */
    dce_cf_find_name_by_key(F, "cellname", (char **)&new, status);
    if (*status == dce_cf_st_ok) {
	rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce,
		(unsigned_char_t *)new, &expanded_name, status);
	if (expanded_name == NULL)
	    *status = dce_cf_e_no_match;
	if (*status == rpc_s_ok) {
	    dce_cf_g_cellname = expanded_name;
	    free(new);
	}
	else
	    dce_cf_g_cellname = NULL;
    }

    /* Get the hostname. */
    if (*status == dce_cf_st_ok) {
	rewind(F);
	dce_cf_find_name_by_key(F, "hostname", &dce_cf_g_hostname, status);
    }

    /* Get the cell aliases. */
    if (*status == dce_cf_st_ok) {
	rewind(F);
	dce_cf_find_names_by_key(F, "cellaliases",
		(char ***)&dce_cf_g_cellaliases, status);
	if (*status == dce_cf_st_ok) {
	    unsigned_char_t	*expanded_name;
	    for (cv=dce_cf_g_cellaliases,dce_cf_g_cellaliases_count=0;
		    cv && *cv; cv++) {
		rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce,
			(unsigned_char_t *)*cv, &expanded_name, status);
		if (expanded_name == NULL)
		    *status = dce_cf_e_no_match;
		if (*status == rpc_s_ok) {
		    free(*cv);
		    *cv = expanded_name;	/* this is allocated */
		    dce_cf_g_cellaliases_count++;
		}
	    }
	}
	/* Missing cellaliases is OK. */
	if (*status = dce_cf_e_no_match)
	    *status = dce_cf_st_ok;
    }

    /* Get the codeset registry filename. */
    if (*status == dce_cf_st_ok) {
	rewind(F);
	dce_cf_find_name_by_key(F, "csrgyfile", &dce_cf_g_csrgy_filename,
	    status);
	if (*status == dce_cf_e_no_match) {
	    dce_cf_g_csrgy_filename = dce_strdup(dce_cf_c_default_csrgy_filename);
	    *status = dce_cf_st_ok;
	}
    }

    /* Cleanup and exit. */
    (void)fclose(F);
Done:
    pthread_mutex_unlock(&dce_cf_g_mutex);
}

/* ______________________________________________________________________ */

/*
**  Return an allocated copy of the next word from a line and a pointer
**  to where the next word might start.  Modifies the input.
*/
PRIVATE void
dce_cf__get_next_word(
    char		*start,
    char		**next,
    char		**new,
    error_status_t	*status
)
{
    char		*p;
    boolean		quoted;
    char		*dest;
    char		save;

    /* Processing quoted text? */
    quoted = *start == '"';
    if (quoted)
	start++;

    for (p = dest = start; *p; ) {
	/* Reached the right terminator? */
	if (quoted) {
	    if (*p == '"')
		break;
	}
	else if (isspace(*p))
	    break;

	/* Copy next non-special character. */
	if (*p == '\\' && p[1] == '"')
	    p++;
	*dest++ = *p++;
    }

    if (quoted && *p == '\0') {
	/* Error:  EOF in quoted text. */
	*status = dce_cf_e_no_match;
	return;
    }

    /* Mark off the word, copy it. */
    save = *dest;
    *dest = '\0';
    if ((*new = dce_strdup(start)) == NULL)
	*status = dce_cf_e_no_mem;
    else
	*status = dce_cf_st_ok;
    *dest = save;

    /* Move to start of next token. */
    if (*p)
	while (*++p && isspace(*p))
	    continue;
    *next = p;
}

/* ______________________________________________________________________ */

/*
**  Read a file looking for a line that starts with a keyword.  Return
**  an allocated copy of the next word on that line.
**
**  Lines must be less then dce_cf_c_ps_buff_size characters.
*/
void
dce_cf_find_name_by_key(
    FILE		*F,
    char		*key,
    char		**value,
    error_status_t	*status
)
{
    char		buff[dce_cf_c_ps_buff_size];
    char		*p;
    char		*newp;
    int			i;
    int			keylen;

    /* Pessimistic. */
    *status = dce_cf_e_no_match;

    for (keylen = strlen(key); fgets(buff, sizeof buff, F) != NULL; ) {
	/* Clobber trailing \n */
	if ((i = strlen(buff)) > 0 && buff[i - 1] == '\n')
	    buff[--i] = '\0';

	/* Ignore comment lines and blank lines and lines that are
	 * too short or don't match. */
	if (buff[0] == '#' || buff[0] == '\0'
	 || i < keylen
	 || strncmp(buff, key, keylen) != 0
	 || !isspace(buff[keylen]))
	    continue;

	/* Skip separating whitespace; ignore value-less lines. */
	for (p = &buff[keylen]; *++p && isspace(*p); )
	    continue;
	if (*p == '\0')
	    continue;

	dce_cf__get_next_word(p, &newp, value, status);
	break;
    }
}

/*
**  Same as above, but returns an array of values, not just one.
**
**  Lines must be less then dce_cf_c_ps_buff_size characters.
*/
void
dce_cf_find_names_by_key(
    FILE		*F,
    char		*key,
    char		***values,
    error_status_t	*status
)
{
    char		buff[dce_cf_c_ps_buff_size];
    char		*p;
    char		*newp;
    int			i;
    int			keylen;
    char		**cv;
    char		*value;
    int			n_alloc, n;

    /* Pessimistic. */
    *status = dce_cf_e_no_match;

    for (keylen = strlen(key); fgets(buff, sizeof buff, F) != NULL; ) {
	/* Clobber trailing \n */
	if ((i = strlen(buff)) > 0 && buff[i - 1] == '\n')
	    buff[--i] = '\0';

	/* Ignore comment lines and blank lines and lines that are
	 * too short or don't match. */
	if (buff[0] == '#' || buff[0] == '\0'
	 || i < keylen
	 || strncmp(buff, key, keylen) != 0
	 || !isspace(buff[keylen]))
	    continue;

	/* Skip separating whitespace; ignore value-less lines. */
	for (p = &buff[keylen]; *++p && isspace(*p); )
	    continue;
	if (*p == '\0')
	    continue;

	n_alloc = 10;
	cv = malloc(sizeof(char*) * n_alloc);
	for (n = 0; ; ) {
	    dce_cf__get_next_word(p, &newp, &value, status);
	    if (*status != dce_cf_st_ok) return;
	    if (value) {
		if ((n+1) > n_alloc) {
		    n_alloc += 4;
		    cv = realloc(cv, sizeof(char*) * n_alloc);
		}
		cv[n++] = value;
	    }
	    if (!newp || ! *newp) break;
	    p = newp;
	}
	cv[n] = NULL;
	*values = cv;
	break;
    }
}
/* ______________________________________________________________________ */

/*
**  Get the local DCE hostname from the cached configuration file.
**  Returns an allocated copy.
*/
void
dce_cf_get_host_name(
    char		**hostname,
    error_status_t	*status
)
{
    dce_cf__load_cache(status);
    if (*status != dce_cf_st_ok)
	return;
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_lock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
    if ((*hostname = dce_strdup(dce_cf_g_hostname)) == NULL)
	*status = dce_cf_e_no_mem;
    else
	*status = dce_cf_st_ok;
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_unlock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
}

/* ______________________________________________________________________ */

/*
**  Get the local codeset registry filename from the cached configuration file.
**  Returns an allocated copy.
*/
void
dce_cf_get_csrgy_filename(
    char		**hostname,
    error_status_t	*status
)
{
    dce_cf__load_cache(status);
    if (*status != dce_cf_st_ok)
	return;
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_lock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
    if ((*hostname = dce_strdup(dce_cf_g_csrgy_filename)) == NULL)
	*status = dce_cf_e_no_mem;
    else
	*status = dce_cf_st_ok;
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_unlock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
}

/* ______________________________________________________________________ */

/*
**  Get the local DCE hostname from the cached configuration file.
**  Returns an allocated copy.
*/
void
dce_cf_get_cell_name(
    char		**cellname,
    error_status_t	*status
)
{
    dce_cf__load_cache(status);
    if (*status != dce_cf_st_ok)
	return;
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_lock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
    if ((*cellname = dce_strdup((char *)dce_cf_g_cellname)) == NULL)
	*status = dce_cf_e_no_mem;
    else
	*status = dce_cf_st_ok;
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_unlock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
}

/* ______________________________________________________________________ */

/*
**  Glue together two namespace entries; the hostname defaults to
**  the local hostname.
*/
PRIVATE void
dce_cf__glue(
    char		*hostname,
    char		*part2,
    char		**value,
    error_status_t	*status
)
{
    dce_cf__load_cache(status);
    if (*status != dce_cf_st_ok)
	return;
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_lock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
    if (hostname == NULL)
	hostname = dce_cf_g_hostname;
    if ((*value = malloc(strlen(hostname) + strlen(part2) + 1)) == NULL)
	*status = dce_cf_e_no_mem;
    else {
	*status = dce_cf_st_ok;
	strcpy(*value, hostname);
	strcat(*value, part2);
    }
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_unlock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
}

/* ______________________________________________________________________ */

/*
**  Return the host principal for a DCE hostname.
*/
void
dce_cf_prin_name_from_host(
    char		*hostname,
    char		**value,
    error_status_t	*status
)
{
    dce_cf__glue(hostname, "/self", value, status);
}

/* ______________________________________________________________________ */

/*
**  Return the endpoint binding name for a DCE hostname.
*/
void
dce_cf_binding_entry_from_host(
    char		*hostname,
    char		**value,
    error_status_t	*status
)
{
    dce_cf__glue(hostname, "/self", value, status);
}

/* ______________________________________________________________________ */

/*
**  Return the profile entry for a DCE hostname.
*/
void
dce_cf_profile_entry_from_host(
    char		*hostname,
    char		**value,
    error_status_t	*status
)
{
    dce_cf__glue(hostname, "/profile", value, status);
}

/* ______________________________________________________________________ */

/*
**  Return the dced binding name for a DCE hostname.
*/
void
dce_cf_dced_entry_from_host(
    char		*hostname,
    char		**value,
    error_status_t	*status
)
{
    dce_cf__glue(hostname, "/config", value, status);
}

/* ______________________________________________________________________ */

/*
**  Return a list of aliases for this cell.  List is an allocated, null-
**  terminated array of pointers.  The strings are allocated, as is the
**  list itself.  If there are no aliases, return NULL for array.
*/
void
dce_cf_get_cell_aliases(
  /* in */
    char			 ***aliases,
  /* out */
    error_status_t		*status
)
{
    unsigned_char_p_t		*cv;
    char			**a;

    dce_cf__load_cache(status);
    if (*status != dce_cf_st_ok)
	return;
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_lock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
    if (dce_cf_g_cellaliases_count == 0) {
	*aliases = NULL;
#ifndef	HPDCE_CF_THREAD_SAFE
	pthread_mutex_unlock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
	return;
    }
    *aliases = malloc(sizeof(char*) * (dce_cf_g_cellaliases_count+1));
#ifndef	HPDCE_CF_THREAD_SAFE
    if (*aliases == NULL)
	*status = dce_cf_e_no_mem;
    else {
#endif	/* HPDCE_CF_THREAD_SAFE */
    a = *aliases;
    for (cv = dce_cf_g_cellaliases; *cv; cv++,a++)
	*a = dce_strdup((char *)*cv);
    *a = NULL;
#ifndef	HPDCE_CF_THREAD_SAFE
    }
#endif	/* HPDCE_CF_THREAD_SAFE */
#ifndef	HPDCE_CF_THREAD_SAFE
    pthread_mutex_unlock(&dce_cf_g_mutex);
#endif	/* HPDCE_CF_THREAD_SAFE */
}

/* ______________________________________________________________________ */

/*
**  Free a list of aliases for this cell.  List is a null-terminated array
**  of pointers.  Also frees the array.
*/
void
dce_cf_free_cell_aliases(
  /* in */
    char			**aliases,
  /* out */
    error_status_t		*status
)
{
    char	**cv;

    *status = dce_cf_st_ok;
    if (!aliases)
	return;
    for (cv = aliases; *cv; cv++)
	free(*cv);
    free(aliases);
}

/* ______________________________________________________________________ */

/*
**  See if the two cell names are indeed the same cell.
*/
void
dce_cf_same_cell_name(
  /* in */
    char		*cellname1,
    char		*cellname2,
  /* out */
    boolean32		*result,
    error_status_t	*st
)
{
    sec_rgy_handle_t		rh;
    sec_rgy_bind_auth_info_t	auth_info;
    uuid_t			u1, u2;
    char			*freeme=NULL;
    error_status_t		st2;

    *st = dce_cf_st_ok;
    *result = FALSE;
    if (cellname1 == NULL && cellname2 == NULL) {
	/* 2 nulls - a match */
	*result = TRUE;
	return;
    }
    if (cellname1 == NULL) {
	dce_cf_get_cell_name(&cellname1, st);
	if (*st != dce_cf_st_ok) return;
	freeme = cellname1;
    }
    else if (cellname2 == NULL) {
	dce_cf_get_cell_name(&cellname2, st);
	if (*st != dce_cf_st_ok) return;
	freeme = cellname2;
    }

    /* Get UUIDs for both cell names. */
    auth_info.info_type = sec_rgy_bind_auth_none;
    sec_rgy_site_bind((unsigned_char_t *)cellname1, &auth_info, &rh, st);
    if (*st != error_status_ok)
	return;
    sec_id_parse_name(rh, (unsigned_char_t *)cellname1, NULL, &u1, NULL, NULL, st);
    sec_rgy_site_close(rh, &st2);
    if (*st != error_status_ok)
	return;

    auth_info.info_type = sec_rgy_bind_auth_none;
    sec_rgy_site_bind((unsigned_char_t *)cellname2, &auth_info, &rh, st);
    if (*st != error_status_ok)
	return;
    sec_id_parse_name(rh, (unsigned_char_t *)cellname2, NULL, &u2, NULL, NULL, st);
    sec_rgy_site_close(rh, &st2);
    if (*st != error_status_ok)
	return;
    if (freeme)
	free(freeme);

    /* Compare UUIDs.  If they're the same, then it's the same cell. */
    if (uuid_equal(&u1, &u2, st)) {
	*result = TRUE;
	*st = dce_cf_st_ok;
	return;
    }
    *result = FALSE;
    *st = dce_cf_e_no_match;
}


/* ______________________________________________________________________ */

#if	defined(TEST)
void
tryit(void)
{
    error_status_t	st;
    char		*hostname;
    char		*cellname;
    char		**aliases, **a;

    dce_cf_get_host_name(&hostname, &st);
    if (st != dce_cf_st_ok)
	printf("status code = 0x%8.8lx\n", st);
    else
	printf("host=%s\n", hostname);
    dce_cf_get_cell_name(&cellname, &st);
    if (st != dce_cf_st_ok)
	printf("status code = 0x%8.8lx\n", st);
    else
	printf("cell=%s\n", cellname);
    dce_cf_get_cell_aliases(&aliases, &st);
    if (st != dce_cf_st_ok)
	dce_pgm_printf(st);
    else {
	if (aliases) {
	    printf("aliases=\n");
	    a = aliases;
	    while (*a) printf("  %s\n", *a++);
	    dce_cf_free_cell_aliases(aliases, &st);
	}
    }
}

int
main(void)
{
    char		buff[256];
    error_status_t	st;

    dce_svc_set_progname("dce_cf-test", &st);
    tryit();
    printf("Hit return to try again:");
    fflush(stdout);
    gets(buff);
    tryit();
    return 0;
}
#endif	/* defined(TEST) */
