/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1994, 1996 Open Software Foundation, Inc.
 * Permission is hereby granted to use, copy, modify and freely distribute  
 * the software in this file and its documentation for any purpose without  
 * fee, provided that the above copyright notice appears in all copies and  
 * that both the copyright notice and this permission notice appear in  
 * supporting documentation.  Further, provided that the name of Open  
 * Software Foundation, Inc. ("OSF") not be used in advertising or  
 * publicity pertaining to distribution of the software without prior  
 * written permission from OSF.  OSF makes no representations about the  
 * suitability of this software for any purpose.  It is provided "as is"  
 * without express or implied warranty.
 * 
 * Set message routing.
 */

/*
 * HISTORY
 * $Log: svcroute.c,v $
 * Revision 1.1.11.3  1996/02/18  19:22:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:31  marty]
 *
 * Revision 1.1.11.2  1995/12/08  21:38:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:18  root]
 * 
 * Revision 1.1.7.1  1994/10/14  18:17:29  bowe
 * 	Added LOCK/UNKLOCK to dce_svc_debug_fillin2() [CR 12608]
 * 	[1994/10/14  18:17:04  bowe]
 * 
 * Revision 1.1.5.30  1994/10/03  20:20:24  bowe
 * 	Set individual routes with '*' is specified.  [CR 12457]
 * 	[1994/10/03  20:20:11  bowe]
 * 
 * Revision 1.1.5.29  1994/10/03  16:54:06  rsalz
 * 	Add GOESTO route type (OT CR 10474).
 * 	[1994/10/03  16:33:18  rsalz]
 * 
 * Revision 1.1.5.28  1994/09/30  19:32:45  bowe
 * 	In dce_svc__debug_fillin(), dup written-to string returned by
 * 	dce_svc__get().  Also, fixed minor memory leak.  [CR 11468]
 * 	[1994/09/30  19:31:52  bowe]
 * 
 * Revision 1.1.5.27  1994/09/29  20:24:05  bowe
 * 	Make dce_svc__linefor() non-staic. [OT 11542]
 * 	[1994/09/29  20:23:35  bowe]
 * 
 * Revision 1.1.5.26  1994/09/13  18:15:40  rrizzo
 * 	Added missing first line to copyright text.
 * 	[1994/09/13  17:34:47  rrizzo]
 * 
 * Revision 1.1.5.25  1994/08/30  20:42:03  marty
 * 	CR 11923 - Change copyright to a public domain copyright.
 * 	[1994/08/30  20:34:35  marty]
 * 
 * Revision 1.1.5.24  1994/08/23  16:18:40  bowe
 * 	In dce_svc__debug_fillin(), dup written-to string. [CR 11468]
 * 	[1994/08/23  15:07:06  bowe]
 * 
 * Revision 1.1.5.23  1994/08/03  18:01:35  bowe
 * 	In dce_svc_debug_routing() save pointer to flags string to
 * 	free later. [CR 11468]
 * 	[1994/08/03  18:01:22  bowe]
 * 
 * Revision 1.1.5.22  1994/08/02  18:31:34  bowe
 * 	Don't clobber string params in dce_svc_debug_routing() [where param]
 * 	and dce_svc__really_debug_set_levels() [flags param].  [CR 11521,11468]
 * 	[1994/08/02  18:12:44  bowe]
 * 
 * Revision 1.1.5.21  1994/08/01  15:40:15  bowe
 * 	Merged with changes from 1.1.5.20
 * 	[1994/08/01  15:40:05  bowe]
 * 
 * 	Make functions cancel-safe [CR 11463]
 * 	[1994/08/01  15:34:23  bowe]
 * 
 * Revision 1.1.5.20  1994/08/01  15:30:50  rsalz
 * 	Cleanup typo's in internal dce_assert calls (OT CR 11502).
 * 	[1994/08/01  15:13:35  rsalz]
 * 
 * Revision 1.1.5.19  1994/07/27  12:28:06  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:35:34  rsalz]
 * 
 * Revision 1.1.5.18  1994/07/26  18:14:53  bowe
 * 	Set r->generations = 1 when opening route [CR 10495]
 * 	[1994/07/26  18:14:31  bowe]
 * 
 * Revision 1.1.5.17  1994/07/26  15:51:44  bowe
 * 	When setting routes [in dce_svc__open_routes() and dce_svc_routing()]
 * 	check the field values. Plug minor leak (if error, in latter). [CR 10367]
 * 	[1994/07/26  15:45:23  bowe]
 * 
 * Revision 1.1.5.16  1994/07/13  19:32:15  bowe
 * 	In dce_svc_routing(), comparing dce_svc_g_code_table entry with route
 * 	spec was wrong.  [CR 10947]
 * 	Check that component name matches svc debug env variable. [CR 10639]
 * 	[1994/07/13  19:31:25  bowe]
 * 
 * Revision 1.1.5.15  1994/07/08  21:42:39  bowe
 * 	In dce_svc_routing() increment "p" to point to right place,
 * 	fix bad test of strcmp().  [CR 11161,11059]
 * 	Also, in dce_svc__open_routes(), malloc a little extra for longer
 * 	filenames (2-digit generation numbers).
 * 	[1994/07/08  21:41:33  bowe]
 * 
 * Revision 1.1.5.14  1994/06/10  19:42:10  tom
 * 	Change argument to dce_svc_fillin so we can only set
 * 	default route to STDERR for warnings and above. (OT 10922)
 * 	[1994/06/10  19:41:33  tom]
 * 
 * Revision 1.1.5.13  1994/06/09  16:06:36  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:07  devsrc]
 * 
 * Revision 1.1.5.12  1994/06/08  18:05:35  rsalz
 * 	dce_svc__open_routes was freeing the wrong thing (OT CR 10903).
 * 	[1994/06/08  18:03:12  rsalz]
 * 
 * Revision 1.1.5.11  1994/06/08  14:53:15  rsalz
 * 	Route file is only one line so clean up the code (cf OT CR 10476).
 * 	Allow wildcard in routing file.
 * 	Fix wildcard parsing (OT CR 10894).
 * 	Set default routing to STDERR for non-debug messages (OT CR 10885).
 * 	[1994/06/08  14:35:01  rsalz]
 * 
 * Revision 1.1.5.10  1994/06/06  13:14:18  bowe
 * 	In dce_svc__open_routes(), open routes in a loop, checking for
 * 	multiple, space-separate route specifications.  [CR 10476]
 * 	[1994/06/06  13:13:51  bowe]
 * 
 * Revision 1.1.5.9  1994/05/19  13:15:52  rsalz
 * 	Add temporary dce_svc_debug_fillin2 for fast build turn-around.
 * 	[1994/05/19  13:15:27  rsalz]
 * 
 * Revision 1.1.5.8  1994/05/18  19:27:07  rsalz
 * 	Make static registration work with DCE_SVC_DEBUG (OT CR 10572)
 * 	[1994/05/18  14:31:09  rsalz]
 * 
 * Revision 1.1.5.7  1994/05/06  19:49:53  rsalz
 * 	dce_svc_debug_set_levels set no levels (OR CR 10569).
 * 	[1994/05/06  18:45:17  rsalz]
 * 
 * Revision 1.1.5.6  1994/04/19  20:01:29  rsalz
 * 	Change shorthand syntax; validate severity and how fields (OT CR 10367).
 * 	[1994/04/18  01:47:59  rsalz]
 * 
 * 	Validate sub-component and level in setting debug levels (OT CR 10368).
 * 	[1994/04/18  01:37:44  rsalz]
 * 
 * Revision 1.1.5.5  1994/02/09  17:36:47  rsalz
 * 	Remove "#include <string.h>" -- done in svcimpl.h (OT CR 9916).
 * 	[1994/02/09  17:36:29  rsalz]
 * 
 * Revision 1.1.5.4  1994/01/27  20:05:26  bowe
 * 	Removed definition of dcelocal_path.  (get it from dce/dce.h)
 * 	[1994/01/27  19:23:08  bowe]
 * 
 * Revision 1.1.5.3  1994/01/26  16:25:14  bowe
 * 	Made dce_localpath be "extern".
 * 	[1994/01/26  16:24:50  bowe]
 * 
 * Revision 1.1.5.2  1994/01/04  19:41:37  ohara
 * 	svr4 needs string.h
 * 	[1994/01/04  19:39:01  ohara]
 * 
 * Revision 1.1.5.1  1993/12/20  17:00:21  rsalz
 * 	Initialize temp pointer to NULL <Ernst.Kraemer@mch.sni.de>
 * 	[1993/12/20  16:49:11  rsalz]
 * 
 * Revision 1.1.2.4  1993/11/04  18:40:46  rsalz
 * 	Ignore any "private" routing information.
 * 	[1993/11/04  18:40:12  rsalz]
 * 
 * Revision 1.1.2.3  1993/09/02  17:42:59  rsalz
 * 	Change dce_svc_routings to take a flag indicating if it
 * 	should return standard routings or debug settings.
 * 	Turn dce_svc_g_memory into an array of structures that keep
 * 	the text and an "is a debug message" indicator.
 * 	[1993/09/02  17:42:49  rsalz]
 * 
 * 	Fix dce_svc_debug_set_levels to nip string properly.
 * 	Only store unique routes in dce_svc_g_memory.
 * 	[1993/09/01  15:06:36  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:10:37  rsalz
 * 	Initial release
 * 	[1993/08/16  18:05:09  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>


/*
**  We might get a command-line switch before registration so we
**  remember all routings we were given.
*/
int			dce_svc_g_memory_size;
dce_svc_memory_t	*dce_svc_g_memory;
static boolean		dce_svc_g_atexit_done;
dce_svc_routing_t	*dce_svc_g_routebase;



/*
**  Add a string to the memory.
*/
static void
dce_svc__store(
    char			*text,
    boolean32			debug_value,
    error_status_t		*status
)
{
    int				length;
    int				i;
    dce_svc_memory_t		*mp;

    /* Clone the string. */
    if ((text = dce_strdup(text)) == NULL) {
	*status = svc_s_no_memory;
	return;
    }
    *status = svc_s_ok;

    if (dce_svc_g_memory_size == 0)
	/* First entry -- get initial space. */
	dce_svc_g_memory =
	    (dce_svc_memory_t *)malloc(sizeof *dce_svc_g_memory);
    else {
	/* This is asserted by all our callers. */
	dce_assert(NULL, strchr(text, SVC_SEP_CHAR) != NULL);

	/* See if already there. */
	length = strchr(text, SVC_SEP_CHAR) - text;
	for (i = dce_svc_g_memory_size, mp = dce_svc_g_memory; --i >= 0; mp++)
	    if (strncmp(mp->text, text, length) == 0
	     && mp->text[length] == SVC_SEP_CHAR) {
		free(mp->text);
		mp->text = text;
		mp->is_debug = debug_value;
		return;
	    }

	/* Not there -- get space to add it. */
	dce_svc_g_memory = (dce_svc_memory_t *)realloc(dce_svc_g_memory,
			(1 + dce_svc_g_memory_size) * sizeof *dce_svc_g_memory);
    }

    if (dce_svc_g_memory == NULL) {
	free(text);
	*status = svc_s_no_memory;
	return;
    }

    mp = &dce_svc_g_memory[dce_svc_g_memory_size++];
    mp->text = text;
    mp->is_debug = debug_value;
}

/*
** Store/set all production routes.
*/
#define sizeof_code_table \
	(sizeof dce_svc_g_code_table / sizeof dce_svc_g_code_table[0])

static void
dce_svc__store_all(
    char			*text,
    error_status_t		*status
)
{
    char			one_route[SMBUF];
    int				i;
    const char			**tp;

    /* assumes we start pointing to the '*' in (for example) "*:STDERR:-" */
    text++;	/* point to the ':' */
    for (i=0, tp=dce_svc_g_code_table; i < sizeof_code_table; i++, tp++) {
	sprintf(one_route, "%s%s", *tp, text);
	dce_svc__store(one_route, false, status);
	if (*status != error_status_ok)
	    return;
    }
}

/*
**  If we can find the string in memory, return last instance.
*/
static char *
dce_svc__get(
    const char		*name
)
{
    int			i;
    int			length;
    dce_svc_memory_t	*mp;
    char		*save;

    length = strlen(name);
    save = NULL;
    for (i = dce_svc_g_memory_size, mp = dce_svc_g_memory; --i >= 0; mp++)
	if (strncmp(mp->text, name, length) == 0
	 && mp->text[length] == SVC_SEP_CHAR) {
	    save = mp->text + length + 1;
	    break;
	}
    return save;
}


/*
**  Return an allocated copy of the appropriate line from the routine
**  file, or NULL if no routing file or error such as malloc failure.
**  Should fill in a *status.
*/
char *
dce_svc__linefor(
    const char			*name
)
{
    FILE			*F;
    char			*p;
    char			*result;
    char			buff[SMBUF];
    char			*start;
    int				cstate;		/* thread cancel state */

    /* Open the right file. */
    if ((p = getenv(SVC_ROUTEFILE_ENVVAR)) == NULL) {
	(void)sprintf(buff, SVC_ROUTEFILE_FORMAT, dcelocal_path);
	p = buff;
    }
    cstate = pthread_setcancel(CANCEL_OFF);

    if ((F = fopen(p, "r")) == NULL) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	return NULL;
    }

    /* Loop over all lines in it. */
    for (result = NULL; fgets(buff, sizeof buff, F); ) {
	/* Ignore blank lines, comment lines, and malformed lines. */
	if ((p = strchr(buff, '\n')) != NULL)
	    *p = '\0';
	for (start = buff; *start == ' ' || *start == '\t'; start++)
	    continue;
	if (*start == SVC_COMMENT_CHAR || *start == '\0')
	    continue;
	if ((p = strchr(start, SVC_SEP_CHAR)) == NULL)
	    continue;
	*p++ = '\0';

	/* Is this line for us? */
	if (strcmp(start, name) == 0 || strcmp(start, "*") == 0) {
	    result = dce_strdup(p);
	    break;
	}
    }

    /* Done. */
    (void)fclose(F);
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
    return result;
}


/*
**  Close an open route.
*/
void
dce_svc__close_routes(
    dce_svc_routing_block_t		*rb
)
{
    dce_svc_routing_t			*r;

    while (rb->nroutes > 0) {
	r = rb->routes[--rb->nroutes];
	switch (r->route) {
	default:
	    dce_assert(NULL, "Unknown route closed" == NULL);
	    /* FALLTHROUGH */
	case route_default:
	case route_discard:
	case route_stdout:
	case route_stderr:
	    break;
	case route_binfile:
	    free(r->data.binfile.name);
	    (void)close(r->data.binfile.fd);
	    break;
	case route_textfile:
	    free(r->data.textfile.name);
	    (void)fclose(r->data.textfile.f);
	    break;
	case route_goesto:
	    free(r->data.goesto.where);
	    break;
	}
    }
}

/*
**  We hit the message-count for this route; close it down and open
**  the next version.
*/
void
dce_svc__rotate_route(
    dce_svc_routing_t	*r
)
{
    FILE			*f;
    char			*p;
    char			*q;
    int				fd;

    /* Cycle counters. */
    if ((r->generations)++ >= r->maxgenerations)
	r->generations = 1;
    r->count = 1;

    switch (r->route) {
    default:
	dce_assert(NULL, "Unknown route rotated" == NULL);
	/* FALLTHROUGH */
    case route_discard:
    case route_default:
    case route_stdout:
    case route_stderr:
    case route_goesto:
	break;
    case route_binfile:
	q = r->data.binfile.name;
	p = strrchr(q, '.') + 1;
	(void)sprintf(p, "%d", r->generations);
	if ((fd = open(q, O_WRONLY | O_CREAT | O_TRUNC, 0666)) >= 0) {
	    (void)close(r->data.binfile.fd);
	    r->data.binfile.fd = fd;
	}
	break;
    case route_textfile:
	q = r->data.textfile.name;
	p = strrchr(q, '.') + 1;
	(void)sprintf(p, "%d", r->generations);
	if ((f = fopen(q, "w")) != NULL) {
	    (void)fclose(r->data.textfile.f);
	    r->data.textfile.f = f;
	}
	break;
    }
}


/*
**  Open up a message route.  Assumes default is already set.
*/
static void
dce_svc__open_routes(
    dce_svc_routing_block_t	*rb,
    dce_route_type_t		default_type,
    char			*where,
    error_status_t		*status
)
{
    dce_svc_routing_t		*r;
    char			*p;
    char			*tail;
    char			*gens;
    char			*count;
    char			buff[SMBUF];
    char			*next;
    char			*orig;

    rb->nroutes = 0;
    for (where = orig = dce_strdup(where); where && *where; where = next) {
	/* Nip off this route if multiple routes given. */
	if ((next = strchr(where, SVC_ROUTESEP_CHAR)) != NULL)
	    for (*next++ = '\0'; *next == SVC_ROUTESEP_CHAR; next++)
		continue;

	/* Split off type and argument. */
	if ((p = strchr(where, SVC_SEP_CHAR)) == NULL) {
	    *status = svc_s_bad_routespec;
	    free(orig);
	    return;
	}
	*p++ = '\0';

	/* Ignore private part. */
	if ((tail = strchr(p, SVC_SEP_CHAR)) != NULL)
	    *tail = '\0';

	/* See if there is a generation and count subfield. */
	if ((gens = strchr(where, SVC_LEVELSEP_CHAR)) != NULL) {
	    *gens++ = '\0';
	    if ((count = strchr(gens, SVC_LEVELSEP_CHAR)) == NULL) {
		*status = svc_s_bad_routespec;
		free(orig);
		return;
	    }
	    *count++ = '\0';
	}

	if ((r = (dce_svc_routing_t *)malloc(1 * sizeof *r)) == NULL) {
	    *status = svc_s_no_memory;
	    free(orig);
	    return;
	}
	r->route = default_type;
	rb->routes[rb->nroutes++] = r;
	if (gens) {
	    if ((r->maxgenerations = atoi(gens)) > 99)
		r->maxgenerations = 99;
	    r->maxcount = atoi(count);
	}
	else
	    r->maxgenerations = r->maxcount = 0;
	r->count = 0;
	r->generations = 1;

	r->next = dce_svc_g_routebase;
	dce_svc_g_routebase = r;

	*status = svc_s_ok;

	if (strcmp(where, ROUTE_STDOUT) == 0)
	    r->route = route_stdout;
	else if (strcmp(where, ROUTE_STDERR) == 0)
	    r->route = route_stderr;
	else if (strcmp(where, ROUTE_DISCARD) == 0)
	    r->route = route_discard;
	else if (strcmp(where, ROUTE_BINFILE) == 0) {
	    (void)sprintf(buff, p, (long)getpid());
	    if (gens) {
		(void)strcat(buff, ".1");
		r->data.binfile.fd = open(buff, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	    }
	    else
		r->data.binfile.fd = open(buff, O_APPEND | O_WRONLY | O_CREAT, 0666);
	    if (r->data.binfile.fd >= 0) {
		r->route = route_binfile;
		r->data.binfile.name = dce_strdup(buff);
	    }
	}
	else if (strcmp(where, ROUTE_TEXTFILE) == 0
	      || strcmp(where, ROUTE_FILE) == 0) {
	    (void)sprintf(buff, p, (long)getpid());
	    if (gens) {
		(void)strcat(buff, ".1");
		r->data.textfile.f = fopen(buff, "w");
	    }
	    else
		r->data.textfile.f = fopen(buff, "a");
	    if (r->data.textfile.f != NULL) {
		r->route = route_textfile;
		/* Like dce_strdup(), but we need more space for longer
		 * filename extenstions (ie, 2-digit numbers). */
		r->data.textfile.name = malloc(strlen(buff) + 4);
		(void)strcpy(r->data.textfile.name, buff);
	    }
	}
	else if (strcmp(where, ROUTE_GOESTO) == 0) {
	    r->route = route_goesto;
	    r->data.goesto.where = dce_strdup(p);
	}
	else {
	    *status = svc_s_bad_routespec;
	    free(orig);
	    return;
	}
    }

    if (!dce_svc_g_atexit_done) {
	(void)atexit(dce_svc__shutdown);
	dce_svc_g_atexit_done = TRUE;
    }
    free(orig);
}


/*
**  Parse the flags for a known component.
*/
static void
dce_svc__really_debug_set_levels(
    dce_svc_handle_t		h,
    char			*flags,
    error_status_t		*status
)
{
    dce_svc_subcomp_t		*tp;
    char			*next;
    char			*level;
    int				i;
    char			*free_it;	/* mem to free at end */

    free_it = flags = dce_strdup(flags);
    for (*status = svc_s_ok; flags && *flags; flags = next) {
	if ((next = strchr(flags, SVC_FLAGSEP_CHAR)) != NULL)
	    *next++ = '\0';
	if ((level = strchr(flags, SVC_LEVELSEP_CHAR)) == NULL) {
	    *status = svc_s_bad_routespec;
	    break;
	}
	*level++ = '\0';
	if (!isdigit(*level)
	 || (i = atoi(level)) < svc_c_debug_off || i > svc_c_debug9) {
	    *status = svc_s_bad_routespec;
	    break;
	}
	if (strcmp(flags, "*") == 0)
	    for (tp = h->table; tp->sc_name; tp++)
		tp->sc_level = i;
	else {
	    for (tp = h->table; tp->sc_name; tp++)
		if (strcmp(flags, (char *)tp->sc_name) == 0) {
		    tp->sc_level = i;
		    break;
		}
	    if (tp->sc_name == NULL) {
		/* Didn't find the sub-component, error. */
		*status = svc_s_bad_routespec;
		break;
	    }
	}
    }
    free(free_it);
}


/*
**  Set the routing for an error level.
*/
void
dce_svc__fillin(
    unsigned32			attributes,
    error_status_t		*status
)
{
    int				i;
    char			buff[SMBUF];
    char			*p;
    char			*freeme;
    const char			*name;

    /* Pull config string out of memory? */
    freeme = NULL;
    i = ATTRIBUTEStoINDEX(attributes);
    name = dce_svc_g_code_table[i];
    if ((p = dce_svc__get(name)) == NULL) {
	/* No -- try environment? */
	(void)sprintf(buff, SVC_ROUTE_ENVVAR, name);
	for (p = buff; *p; p++)
	    if (islower(*p))
		*p = toupper(*p);
	if ((p = getenv(buff)) == NULL) {
	    /* No -- try file? */
	    if ((p = dce_svc__linefor(name)) == NULL) {
		/* No -- programmatically determine defaults. */
		attributes &= svc__c_mask;
		p = attributes == svc_c_sev_notice
			|| attributes == svc_c_sev_notice_verbose
		    ? "DISCARD:" : "STDERR:";
	    }
	    else
		freeme = p;
	}
    }
    else
	freeme = p = dce_strdup(p);

    dce_svc__open_routes(&dce_svc_g_routes[i], route_discard, p, status);
    if (freeme)
	free(freeme);
}


/*
**  Fill in the debug routing for a component.
*/
boolean
dce_svc__debug_fillin(
    dce_svc_handle_t		handle,
    error_status_t		*status
)
{
    char			buff[SMBUF];
    char			*p;
    char			*flags;
    char			*freeme;

    /* Add the table, translate its strings. */
    if (!dce_svc__add_to_table(handle, status))
	return FALSE;

    /* Set defaults. */
    handle->setup = TRUE;
    handle->routes.nroutes = 0;
    freeme = NULL;

    /* Pull config string out of memory? */
    if ((p = dce_svc__get(handle->component_name)) == NULL) {
	/* No -- try environment? */
	(void)sprintf(buff, SVC_DBGROUTE_ENVVAR, handle->component_name);
	for (p = buff; *p; p++)
	    if (islower(*p))
		*p = toupper(*p);
	if ((p = getenv(buff)) != NULL) {
	    int	len;
	    /* check that comp name matches name in env variable */
	    len = strlen(handle->component_name);
	    if (strncasecmp(p, handle->component_name, len)) {
		p = NULL;
		return FALSE;
	    }
	    p += len + 1;		/* skip name and ':' */
	}
	else {
	    /* No -- try file? */
	    if ((p = dce_svc__linefor(handle->component_name)) == NULL)
		/* No -- use defaults. */
		return TRUE;
	    freeme = p;
	}
    }
    else
	freeme = p = dce_strdup(p);

    /* Parse flags. */
    flags = p;
    if ((p = strchr(flags, SVC_SEP_CHAR)) == NULL) {
	if (freeme)
	    free(freeme);
	return FALSE;
    }
    *p++ = '\0';
    dce_svc__really_debug_set_levels(handle, flags, status);
    if (*status != svc_s_ok) {
	if (freeme)
	    free(freeme);
	return FALSE;
    }

    /* Open the route. */
    dce_svc__open_routes(&handle->routes, route_stderr, p, status);

    /* Cleanup. */
    if (freeme)
	free(freeme);
    return TRUE;
}


/*
**  Set the debug levels and return the specified level.
*/
unsigned32
dce_svc__debug_fillin2(
    dce_svc_handle_t		handle,
    const unsigned32		table_index
)
{
    error_status_t		st;
    boolean			ret;

    LOCK();
    ret = dce_svc__debug_fillin(handle, &st)
	    ? handle->table[table_index].sc_level : 0;
    UNLOCK();
    return (unsigned32)ret;
}


void
dce_svc_debug_set_levels(
    unsigned char		*where,
    error_status_t		*status
)
{
    dce_svc_handle_t		h;
    char			*p;

    if ((p = strchr((char *)where, SVC_SEP_CHAR)) == NULL) {
	*status = svc_s_bad_routespec;
	return;
    }

    LOCK();
    dce_svc__store((char *)where, true, status);
    *p = '\0';
    if ((h = dce_svc__component_to_handle(where)) != NULL)
	dce_svc__really_debug_set_levels(h, p + 1, status);
    UNLOCK();
}


void
dce_svc_routing(
    unsigned char		*owhere,
    error_status_t		*status
)
{
    char			*p;
    int				i, len;
    boolean			all, found;
    unsigned char		*where;

    LOCK();
    *status = svc_s_bad_routespec;
    where = (unsigned char *)dce_strdup((char *)owhere);

    /* Find the first two separators. */
    if ((p = strchr((char *)where, SVC_SEP_CHAR)) == NULL
     || (strchr(p + 1, SVC_SEP_CHAR) == NULL)) {
	UNLOCK();
	free(where);
	return;
    }

    /* Is this a wildcard route? */
    all = where[0] == '*' && where[1] == SVC_SEP_CHAR;

    /* Be sure where is one of the known values. */
    len = strchr((char *)where, SVC_SEP_CHAR) - (char *)where;
    found = FALSE;
    for (i=0; i<sizeof dce_svc_g_code_table/sizeof dce_svc_g_code_table[0]; i++)
    {
	if (strncmp((char *)where, dce_svc_g_code_table[i], len) == 0) {
	    found = TRUE;
	    break;
	}
    }
    if (!found && !all) {
	*status = svc_s_bad_routespec;
	free(where);
	UNLOCK();
	return;
    }

    if (all)
	dce_svc__store_all((char *)owhere, status);
    else
	dce_svc__store((char *)owhere, false, status);
    if (*status != svc_s_ok) {
	free(where);
	UNLOCK();
	return;
    }
    p++;

    len = strchr((char *)where, SVC_SEP_CHAR) - (char *)where;
    for (i = 0; i < sizeof dce_svc_g_code_table / sizeof dce_svc_g_code_table[0]; i++)
	if (all) {
	    dce_svc__close_routes(&dce_svc_g_routes[i]);
	    dce_svc__open_routes(&dce_svc_g_routes[i], route_stderr, p, status);
	    if (*status != svc_s_ok)
		break;
	}
	else if (strncmp((char *)where, dce_svc_g_code_table[i], len) == 0) {
	    dce_svc__close_routes(&dce_svc_g_routes[i]);
	    dce_svc__open_routes(&dce_svc_g_routes[i], route_stderr, p, status);
	    break;
	}

    UNLOCK();
    free(where);
}


void
dce_svc_debug_routing(
    unsigned char		*owhere,
    error_status_t		*status
)
{
    char			*p;
    char			*flags;
    dce_svc_handle_t		h;
    unsigned char		*where;

    /* Look up the component. */
    *status = svc_s_bad_routespec;
    where = (unsigned char *)dce_strdup((char *)owhere);
    if ((p = strchr((char *)where, SVC_SEP_CHAR)) == NULL) {
	free(where);
	return;
    }
    LOCK();
    dce_svc__store((char *)owhere, true, status);
    *p = '\0';
    h = dce_svc__component_to_handle(where);
    *p++ = SVC_SEP_CHAR;
    UNLOCK();
    if (h == NULL) {
	free(where);
	return;
    }

    /* Set the flags. */
    flags = p;
    if ((p = strchr(flags, SVC_SEP_CHAR)) == NULL) {
	free(where);
	return;
    }
    *p++ = '\0';
    dce_svc__really_debug_set_levels(h, flags, status);
    if (*status != svc_s_ok) {
	free(where);
	return;
    }

    /* Set the routing. */
    if (*p) {
	LOCK();
	dce_svc__close_routes(&h->routes);
	dce_svc__open_routes(&h->routes, route_discard, p, status);
	UNLOCK();
    }
    free(where);
}


dce_svc_routing_t **
dce_svc__goesto(
    char			*where,
    int				*nroutesp
)
{
    dce_svc_handle_t		h;
    int				i;
    error_status_t		st;

    /* See if it's a known severity. */
    for (i = 0; i < sizeof dce_svc_g_code_table / sizeof dce_svc_g_code_table[0]; i++)
	if (strcmp(where, dce_svc_g_code_table[i]) == 0) {
	    if (dce_svc_g_routes[i].nroutes == 0)
		dce_svc__fillin(dce_svc_g_attr_table[i], &st);
	    *nroutesp = dce_svc_g_routes[i].nroutes;
	    return dce_svc_g_routes[i].routes;
	}

    /* Is it a debug component? */
    if ((h = dce_svc__component_to_handle((unsigned char *)where)) == NULL
     || (!h->setup && !dce_svc__debug_fillin(h, &st))) {
	*nroutesp = 0;
	return NULL;
    }
    *nroutesp = h->routes.nroutes;
    return h->routes.routes;
}
