/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * This is a primitive, low-level queue client.  Originally it did only
 * RPCs to the qmgr.  This was before the API existed.  As an afterthought,
 * I added code to use the API.  (The prog is in either RPC or API mode.)
 *
 * J.Bowe
 * ______________________________________________________________________
 *
 * $Log: main.c,v $
 * Revision 1.11  1996/11/25  18:16:19  bowe
 * Rework to also use the API (in addition to making direct RPCs.
 *
 * Revision 1.10  1996/11/11  21:55:33  bowe
 * Make interface to get_mask_list() more intuitive.
 * Set mattr.notice_opts approproately.
 *
 * Revision 1.9  1996/11/01  20:36:50  bowe
 * Added "move" function (do_move).
 *
 * Revision 1.8  1996/10/31  20:14:07  bowe
 * Accomodate changes in mos_q_get_id_list().
 * Various minor appearance inprovements.
 *
 * Revision 1.7  1996/10/23  19:43:32  bowe
 * Fix bugs in get_mask_list (add "mp++").
 *
 * Revision 1.6  1996/10/22  15:05:53  bowe
 * Better help messages.
 *
 * Revision 1.5  1996/10/16  17:35:01  bowe
 * Formatting tweaks. Better handling of queue names.
 *
 * Revision 1.4  1996/10/11  00:02:15  bowe
 * Fix minor glitch in choosing name in do_create().
 *
 * Revision 1.3  1996/10/07  20:00:23  bowe
 * Recognize minimal string for command names.
 * Implement selection masks/filters.
 * Quiet "lint" (gcc -Wall) warnings.
 *
 * Revision 1.2  1996/10/07  14:15:15  bowe
 * Completely restructure code for flexibility.
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include <dce/nbase.h>
#include <dce/assert.h>
#include <dce/aclif.h>
#include <dce/utc.h>
#include <dce/id_epac.h>

#include "dcemosmsg.h"
#include "dcemosmac.h"
#include "dcemossvc.h"

#include "mos_q.h"
#include "mos_mgmt.h"

#include "mosif.h"

#define MODE_API	0
#define MODE_RPC	1

typedef struct {
    char	*qname;
    char	*id;
    uuid_t	uuid;
    char	*exp;
    char	*idle;
    char	*notd;
    unsigned32	priority;
    unsigned32	pv;
    unsigned32	pvm;
    boolean	hex;
    boolean	all;
    boolean	force;
    boolean	wait;
    boolean	verbose;
    boolean	filter;
    boolean	mode_rpc;
    boolean	mode_api;
    /* "generic" args - 1st tokens after those with flags */
    char	*arg;
    char	*arg2;
} the_args;
static the_args null_args;

typedef void (*do_op)(the_args *);
typedef struct {
    char		*cmd;
    int			min_match;
    char		*abbrev;
    do_op		op_rpc;
    do_op		op_api;
} Command;

char			qname_buf[200];
unsigned_char_t		*DefQ;
unsigned_char_t		*ns_entry = NULL;
unsigned_char_t		*qname = NULL;
unsigned_char_t		*string_binding = NULL;
rpc_binding_handle_t	bh;


dce_svc_handle_t	mos_svc_handle;

static void set_defaults_and_parse_args(int, char **);

char *one_cmd;
static mos_mattr_t		null_mos_mattr;
static mos_qattr_t		null_mos_qattr;
static mos_datatype_bytearray_t	null_mos_body;

static mos_handle_t attached_qh, default_qh;
static boolean	am_attached = FALSE;
static int my_mode = MODE_RPC;

static char *help_msg;

/* these live in libdce, but not documented */
extern char *readline(char *);
extern void add_history(char *);

/* ______________________________________________________________________ */
/* Check that status is OK.  If not, print error message and newline. */
#define CHK_OK		0
#define CHK_EXIT	1
#define CHK_RET		2
#define CHK_CONT	3

#define CHECK_STATUS_RET(st,s) \
 if ((st) != error_status_ok) \
 { fprintf(stderr,"%s: ",s); dce_pgm_fprintf(stderr,(st)); \
   return; \
 }
#define CHECK_STATUS(st,s,x) \
 if ((st) != error_status_ok) \
 { fprintf(stderr,"%s: ",s); dce_pgm_fprintf(stderr,(st)); \
   if (x==CHK_EXIT) exit(1); \
   if (x==CHK_RET) return; \
 }
#define CHECK_STATUS2(st,s,x) \
 if ((st) != error_status_ok) \
 { fprintf(stderr,"%s: ",s); dce_pgm_fprintf(stderr,(st)); \
   if (x==CHK_EXIT) exit(1); \
   if (x==CHK_RET) return; \
   if (x==CHK_CONT) continue; \
 }

/* ______________________________________________________________________ */

#define Malloc(N,V,T)   \
    { if (!((V) = (T *)malloc((size_t)N*sizeof(T)))) { \
	perror("Malloc failed -- out of memory.  Bailing out.");  exit(1); \
    } }
#define Realloc(N,V,T)  \
    { if (!((V) = (T *)realloc(V,(size_t)N*sizeof(T)))) { \
	perror("Realloc failed -- out of memory.  Bailing out.");  exit(1); \
    } }

/* ______________________________________________________________________ */
/*  "Split" a string into tokens.  Given a string that has space-separated
 *  (space/tab) tokens, return a pointer to an array of pointers to the
 *  tokens.  Like what the shell does with *argv[].  The array can be is
 *  static or allocated.  Space can be allocated for string, or allocated.
 *  Arguments:
 *	Pointer to string to pick apart.
 *	Pointer to max number of tokens to find; actual number found is
 *	  returned. If 0 or null pointer, use a 'sane' maximum number (hard-
 *	  code). If more tokens than the number specified, make last token be
 *	  a single string composed of the rest of the tokens (includes spaces).
 *	Flag. Bit 0 says whether to make a copy of input string (since we'll
 *	  clobber parts of it).  To free the string, use the pointer to
 *	  the first token returned by the function (or *ret_value).
 *	  Bit 1 says whether to allocate the vector itself.  If not, use
 *	  (and return) a static vector.
 *  Return:
 *	Pointer to the provided string (for convenience of caller).
 */

/* bigmask/flags for the Split() function */
#define S_STRDUP	0x01
#define S_ALVEC		0x02
#define EOS		'\0'
#define NL		'\n'
#define TAB		'\t'
#define CR		'\r'
#define ANCHOR		'^'
#define IsWhite(c)	(c == ' ' || c == TAB)

char **
tokenize(
    char	*s,		/* input string */
    int		*ntok,		/* # of tokens desired (input)/found (return) */
    int		flag		/* dup string? allocate a vector? */
)
{
    int		maxnt, i=0;
    int		n_alloc;
    char	**tokens;
    static char	*local_tokens[100];

    /* Figure max number of tokens (maxnt) to find.  0 means find them all. */
    if (ntok == NULL)
	maxnt = 100;
    else {
	if (*ntok <= 0 || *ntok > 100) maxnt = 100;	/* arbitrary size */
	else maxnt = *ntok;
	*ntok = 0;
    }

    if (!s) return 0;			/* no string */

    /* Point to 1st token (there may be initial space) */
    while (*s && IsWhite(*s)) s++;	/* skip initial space, if any */
    if (*s == EOS) return 0;		/* none found? */

    /* See if caller wants us to copy the input string. */
    if (flag & S_STRDUP) s = strdup(s);

    /* See if caller wants us to allocate the returned vector. */
    if (flag & S_ALVEC) {
	n_alloc = 20;
	Malloc(n_alloc, tokens, char *);
	/* if caller did not specify max tokens to find, set to more than
	 * there will possibly ever be */
	if (!ntok || !(*ntok)) maxnt = 10000;
    }
    else tokens = local_tokens;

    i = 0;			/* index into vector */
    tokens[0] = s;		/* s already points to 1st token */
    while (i<maxnt) {
	tokens[i] = s;		/* point vector member at start of token */
	i++;
	/* If we allocated vector, see if we need more space. */
	if ((flag & S_ALVEC) && i >= n_alloc) {
	    n_alloc += 20;
	    Realloc(n_alloc, tokens, char *);
	}
	if (i >= maxnt) break;			/* is this the last one? */
	while (*s && !IsWhite(*s)) s++;		/* skip past end of token */
	if (*s == EOS) break;			/* at end of input string? */
	if (*s) *s++ = EOS;			/* terminate token string */
	while (*s && IsWhite(*s)) s++;		/* skip space - to next token */
    }
    if (ntok) *ntok = i;		/* return number of tokens found */
    tokens[i] = 0;			/* null-terminate vector */
    return tokens;
}
/* ______________________________________________________________________ */

void
bind_to_server()
{
    error_status_t	st;
    rpc_ns_handle_t	import_context;
    unsigned_char_t	*server_prin;

    if ( string_binding ) {
	rpc_binding_from_string_binding(string_binding, &bh, &st);
    }
    else {
	unsigned_char_t *resolved_name, *unresolved_name;

	if (!ns_entry) {
	    fprintf(stderr, "NS Entry of queue manager not given.\n");
	    exit(1);
	}

	rpc_ns_entry_inq_resolution(rpc_c_ns_syntax_default,
		ns_entry,
		&resolved_name, &unresolved_name, &st);
	if (st != rpc_s_partial_results) {
	    CHECK_STATUS(st, "rpc_ns_binding_import_begin", CHK_RET);
	}

	if (unresolved_name && *unresolved_name) qname = unresolved_name;
	ns_entry = resolved_name;

	rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
	    ns_entry, NULL, NULL, &import_context, &st);
	CHECK_STATUS(st, "rpc_ns_binding_import_begin", CHK_RET);
	while (1) {
	    rpc_ns_binding_import_next(import_context, &bh, &st);
	    if (st == rpc_s_no_more_entries) {
		fputs("Bind failed - can't find a server\n", stderr);
		rpc_ns_binding_import_done(&import_context, &st);
		exit(1);
	    }
	    CHECK_STATUS(st, "rpc_ns_binding_import_next", CHK_EXIT);

	    rpc_ep_resolve_binding(bh, mos_q_v1_0_c_ifspec, &st);
	    CHECK_STATUS(st, "rpc_ep_resolve_binding", CHK_EXIT);
	    break;
	}
    }

    /* needed so server know who we are */
    rpc_ep_resolve_binding(bh, mos_q_v1_0_c_ifspec, &st);
    CHECK_STATUS(st, "rpc_ep_resolve_binding", CHK_RET);
    rpc_mgmt_inq_server_princ_name(bh, rpc_c_authn_default, &server_prin, &st);
    CHECK_STATUS(st, "rpc_mgmt_inq_server_princ_name", CHK_RET);
    rpc_binding_set_auth_info(bh, server_prin, rpc_c_protect_level_default,
	rpc_c_authn_default, NULL, rpc_c_authz_dce, &st);
    CHECK_STATUS(st, "rpc_binding_set_auth_info", CHK_RET);
    rpc_string_free(&server_prin, &st);

    /* API-level stuff */
    mos_rsrc_dflt_quemgr_set(ns_entry, NULL, &default_qh, &st);

    return;
}

/* ______________________________________________________________________ */

#define bool2yn(b)	(b?'y':'n')
#define yn2bool(yn)	((yn=='n')?FALSE:TRUE)		/* default = T */
#define persist2pvm(p)	\
   ((p==mos_c_que_attr_volatile)?'v':((p==mos_c_que_attr_persistent)?'p':'m'))
#define persist2pv(p)	((p==mos_c_msg_attr_volatile)?'v':'p')
#define pvm2persist(p)	\
   ((p=='v')?mos_c_que_attr_volatile:\
     ((p=='p')?mos_c_que_attr_persistent:mos_c_que_attr_msgpersistence))
#define pv2persist(p)	\
     ((p=='p')?mos_c_msg_attr_persistent:mos_c_msg_attr_volatile)
#define mode2str(m)	((m==MODE_RPC)?"RPC":"API")


void
get_utc(char *s, utc_t *t, boolean rel)
{
    char buf[100];
    if (s) fputs(s, stdout);
    gets(buf);
    if (rel) utc_mkascreltime(t, buf);
    else utc_mkasctime(t, buf);
}

void
get_and_print_mattr(unsigned_char_t *name, uuid_t *id, boolean one,
	error_status_t *st)
{
    mos_mattr_t	attr;
    mos_q_retrieve_mattr_by_id(bh, name, id, &attr, st);
    CHECK_STATUS(*st, "mos_q_retrieve_mattr_by_id", CHK_RET);
    if (one) Print_mattr_1line(&attr);
    else Print_mattr(&attr);
}

/* same as get_and_print_mattr(), only use API */
void
get_and_print_mattr_a(unsigned_char_t *name, uuid_t *id, boolean one,
	error_status_t *st)
{
    mos_msg_attr_t	mattr;
    mos_msg_selfilter_t	sel;
    sec_id_pa_t		sender;
    char		sender_name[100];
    unsigned32		pri, nflags;
    mos_rsrc_name_t	nd;
    sec_rgy_name_t	ns;
    mos_msg_attr_msgtype_t	msgtype;
    mos_msg_attr_persistence_t	pers;

    /* get attributes */
    mos_msg_selfilter_alloc(&sel, st);
    mos_msg_selmask_add(mos_c_mask_type_msgid, id, mos_c_mask_op_equal, sel,st);
    mos_msg_attr_peek(attached_qh, 0, sel, &mattr, st);
    if (*st != error_status_ok) return;
    mos_msg_selfilter_free(&sel, st);

    /* auck out individual attributes so we can print them */
    mos_msg_attr_msgtype_get(mattr, &msgtype, st);
    mos_msg_attr_persistence_get(mattr, &pers, st);
    mos_msg_attr_priority_get(mattr, &pri, st);
    mos_msg_attr_notice_get(mattr, &nflags, nd, ns, st);

    mos_msg_attr_secid_get(mattr, &sender, st);
    Prin_id_to_name(&sender.principal.uuid, sender_name,
	    sec_rgy_domain_person);

    if (one) {
	printf("%3d   %c  %3d %3x %-12s", msgtype, persist2pv(pers),
		pri, nflags, sender_name);
	Print_uuid(" ", &id);
    }
    else {
	/* get ones for long listing */
	uuid_t		datatype;
	utc_t		enq_time, expire_time, valid_time;

	mos_msg_attr_datatype_get(mattr, &datatype, st);
	mos_msg_attr_enqtime_get(mattr, &enq_time, st);
	mos_msg_attr_ttl_get(mattr, &expire_time, st);
	mos_msg_attr_ttr_get(mattr, &valid_time, st);

	Print_uuid("ID: " , id);
	Print_uuid("  Datatype: ", &datatype);
	Print_sec_id_pa("  Sender:", &sender);
	printf("  Msg-type: %d\n", msgtype);
/*	printf("  Flags: 0x%x\n", flags);*/
	printf("  Persistence: %c\n", (persist2pv(pers)));
	printf("  Priority: %d\n", pri);
	printf("  Notice-opts: 0x%x\n", nflags);
	if (nd[0]) printf("  Reply-queue: %s\n", nd);
	Print_utc("  Enqueued: ", &enq_time, 0);
	Print_utc("  Expires: ", &expire_time, 0);
	Print_utc("  Valid Starting: ", &valid_time, 0);
/*	Print_uuid("  Queue-ID: ", &ma->qid);*/
    }
    mos_msg_attr_free(&mattr, st);
}

void
get_qattr(mos_qattr_t *qa, boolean init)
{
    idl_char ann[100];
    idl_char	**aliases;
    char buf[400];
    unsigned32 ai;

    if (init) *qa = null_mos_qattr;

    printf("Annotation: ");
    gets((char *)ann);
    if (ann[0]) qa->annotation = (idl_char *)strdup((char *)ann);

    ai = 0;
    while (printf("alias #%ld => ", ai), gets(buf)) {
	if (aliases == NULL) aliases = malloc(20 * sizeof(idl_char *));
	if (buf[0] == '\0') break;
	aliases[ai++] = (idl_char *)strdup(buf);
    }
    qa->aliases.count = ai;
    qa->aliases.list = aliases;

    printf("\nMax len: ");
    gets(buf);
    if (buf[0]) qa->max_qlen = atoi(buf);
    printf("Max msg size: ");
    gets(buf);
    if (buf[0]) qa->max_msgsize = atoi(buf);

    printf("Enqueue enabled? [(y)/n]: ");
    gets(buf);
    qa->enq_enabled = (buf[0] != 'n');
    printf("Dequeue enabled? [(y)/n]: ");
    gets(buf);
    qa->deq_enabled = (buf[0] != 'n');

    printf("Persistence? [(m)/v/p]: ");
    gets(buf);
    if (buf[0] == 'v')		qa->persistence = mos_c_que_attr_volatile;
    else if (buf[0] == 'p')	qa->persistence = mos_c_que_attr_persistent;
    else qa->persistence = mos_c_que_attr_msgpersistence;
}

void
edit_qattr(mos_qattr_t *qa, boolean init)
{
    idl_char	ann[100], *icp;
    idl_char	**aliases = NULL;
    char	buf[400];
    unsigned32	ai;

    if (init) {
	*qa = null_mos_qattr;
	qa->enq_enabled = qa->deq_enabled = TRUE;
	qa->persistence = mos_c_que_attr_msgpersistence;
    }

    icp = qa->annotation;
    printf("Annotation [%s]: ", (icp?(char *)icp:""));
    gets((char *)ann);
    if (ann[0]) qa->annotation = (idl_char *)strdup((char *)ann);

    ai = 0;
    while (printf("alias #%ld => ", ai), gets(buf)) {
	if (aliases == NULL) aliases = malloc(20 * sizeof(idl_char *));
	if (buf[0] == '\0') break;
	aliases[ai++] = (idl_char *)strdup(buf);
    }
    qa->aliases.count = ai;
    qa->aliases.list = aliases;

    printf("\nMax len [%ld]: ", qa->max_qlen);
    gets(buf);
    if (buf[0]) qa->max_qlen = atoi(buf);
    printf("Max msg size [%ld]: ", qa->max_msgsize);
    gets(buf);
    if (buf[0]) qa->max_msgsize = atoi(buf);

    printf("Enqueue enabled? (y/n) [%c]: ", bool2yn(qa->enq_enabled));
    gets(buf);
    if (buf[0]) qa->enq_enabled = yn2bool(buf[0]);
    printf("Dequeue enabled? (y/n) [%c]: ", bool2yn(qa->deq_enabled));
    gets(buf);
    if (buf[0]) qa->deq_enabled = yn2bool(buf[0]);

    printf("Persistence? (m/v/p) [%c]: ", persist2pvm(qa->persistence));
    gets(buf);
    if (buf[0]) qa->persistence = pvm2persist(buf[0]);

    printf("Want idle timeout? (y/n) ");
    gets(buf);
    if (buf[0] && yn2bool(buf[0])) {
	get_utc("Idle timeout (relative): ", &qa->empty_timeout, 1);
	qa->flags |= mos_c_use_empty_timeout;
	/*Print_utc("Idle timeout: ", &qa->empty_timeout, 1);*/
    }
}

/* ______________________________________________________________________ */

static boolean
parse_cmd(int ac, char **av, the_args *a)
{
    int			c;
    extern int		optind;

    optind = 1;		/* reset getopt internal state */
    *a = null_args;

    while ((c = getopt(ac, av, "q:i:x:p:n:S:s:I:hafFRAv")) != EOF) {
	switch (c) {
	default:  fprintf(stderr, "Huh?\n");		break;
	case 'q': a->qname = optarg;			break;
	case 'i': a->id = optarg;			break;
	case 'I': a->idle = optarg;			break;
	case 'x': a->exp = optarg;			break;
	case 'p': a->priority = atol(optarg);		break;
	case 'n': a->notd = optarg;			break;
	case 'h': a->hex = TRUE;			break;
	case 'a': a->all = TRUE;			break;
	case 'w': a->wait = TRUE;			break;
	case 'f': a->force = TRUE;			break;
	case 'F': a->filter = TRUE;			break;
	case 's': a->pv = pv2persist(optarg[0]);	break;
	case 'S': a->pvm = pvm2persist(optarg[0]);	break;
	case 'R': a->mode_rpc = TRUE;			break;
	case 'A': a->mode_api = TRUE;			break;
	case 'v': a->verbose = TRUE;			break;
	}
    }
    /* if there are leftover args, make it the qname */
    if (!a->qname && optind < ac) a->qname = av[optind];
    if (!strcmp(a->qname, "-")) a->qname = NULL;

    /* "generic" arg - 1st token without a flag */
    if (optind < ac) a->arg = av[optind];

    /* simple error/sanity check */
    if (a->id) {
	error_status_t st;
	uuid_from_string((unsigned_char_t *)a->id, &a->uuid, &st);
	if (st != error_status_ok) {
	    fprintf(stderr,"UUID converstion failed: ");
	    dce_pgm_fprintf(stderr,st);
	    return FALSE;
	}
    }
    return TRUE;
}

/* ______________________________________________________________________ */
/* print mask list */
void
Print_mask_list(mos_mask_list_t *ml)
{
    mos_mask_t	*mp;
    int		i;
    char	*s, *r;

    printf("Selection mask list:\n");
    for (mp=ml->list,i=0; i<ml->count; mp++,i++) {
	switch(mp->mask_info.mask_type) {
	    case mos_c_mask_type_msgid:		s = "MsgID";		break;
	    case mos_c_mask_type_msgtype:	s = "MsgType";		break;
	    case mos_c_mask_type_priority:	s = "Priority";		break;
	    case mos_c_mask_type_datatype:	s = "DataType";		break;
	    case mos_c_mask_type_sender_prin:	s = "Sender-Prin";	break;
	    case mos_c_mask_type_protectlvl:	s = "Protect-Lvl";	break;
	    case mos_c_mask_type_ttr:		s = "TTL";		break;
	}
	
	switch(mp->relation) {
	    case mos_c_mask_op_equal:		r = "==";	break;
	    case mos_c_mask_op_not_equal:	r = "!=";	break;
	    case mos_c_mask_op_less_than:	r = "< ";	break;
	    case mos_c_mask_op_eq_less_than:	r = "<=";	break;
	    case mos_c_mask_op_greater_than:	r = "> ";	break;
	    case mos_c_mask_op_eq_greater_than:	r = ">=";	break;
	    case mos_c_mask_op_none:		r = "<?>";	break;
	}
	printf("    %d: %-11s %s ", i, s, r);
	switch(mp->mask_info.mask_type) {
	    case mos_c_mask_type_msgid:
	    case mos_c_mask_type_datatype:
	    case mos_c_mask_type_sender_prin:
		Print_uuid(NULL, mp->mask_info.tagged_union.id);
		break;
	    case mos_c_mask_type_msgtype:
		printf("%s\n", (mp->mask_info.tagged_union.msgtype ==
			mos_c_msg_attr_msgtype_data ? "data" : "notice") );
		break;
	    case mos_c_mask_type_priority:
	    case mos_c_mask_type_protectlvl:
		printf("%ld\n", mp->mask_info.tagged_union.priority);
		break;
	}
    }
}

/* returns pointer to statically-allocated masks struct */
mos_mask_list_t *
get_mask_list(the_args *a)
{
    static mos_mask_list_t	masks;
    static mos_mask_t		masklist[20], *mp;
    error_status_t		st;

    masks.count = 0;
    masks.list = masklist;
    mp = masklist;

    /* see what was mentioned on the cmd line.  Assume they are masks... */
    if (a->priority) {
	mp->mask_info.tagged_union.priority = a->priority;
	mp->mask_info.mask_type = mos_c_mask_type_priority;
	mp->relation = mos_c_mask_op_equal;
	masks.count++;
	mp++;
    }

    if (a->id) {
	mp->mask_info.tagged_union.id = a->uuid;
	mp->mask_info.mask_type = mos_c_mask_type_msgid;
	mp->relation = mos_c_mask_op_equal;
	masks.count++;
	mp++;
    }
    if (!isatty(0) || !a->filter) {		/* 0 = stdin */
	if (a->verbose) Print_mask_list(&masks);
	return &masks;
    }

    while (1) {
	char buf[80];
	mos_msg_selmask_type_t t;
	uuid_t	id;

	printf("Mask item #%ld:\n", masks.count);
	ask_mask:
	printf("  Mask type (impdsP l?): ");
	gets(buf);
	if (!buf[0]) break;
	switch (buf[0]) {
	    default:  goto ask_mask;				break;
	    case 'i': t = mos_c_mask_type_msgid;		break;
	    case 'm': t = mos_c_mask_type_msgtype;		break;
	    case 'p': t = mos_c_mask_type_priority;		break;
	    case 'd': t = mos_c_mask_type_datatype;		break;
	    case 's': t = mos_c_mask_type_sender_prin;		break;
	    case 'P': t = mos_c_mask_type_protectlvl;		break;
	    case 'l': Print_mask_list(&masks); goto ask_mask;	break;
	    case '?': puts("i=id, m=msgtype, p=priority, d=datatype, "
			"s=sender-uuid, P=protectlvl\nl=list current mask");
		    goto ask_mask;
		    break;
	}
	mp->mask_info.mask_type = t;

	ask_value:
	switch (t) {
	    case mos_c_mask_type_priority:
		printf("  Mask value (u32): ");
		gets(buf);
		mp->mask_info.tagged_union.priority = atol(buf);
		break;
	    case mos_c_mask_type_msgid:
	    case mos_c_mask_type_datatype:
	    case mos_c_mask_type_sender_prin:
		printf("  Mask value (uuid): ");
		gets(buf);
		uuid_from_string((unsigned_char_t *)buf, &id, &st);
		if (st != error_status_ok) goto ask_value;
		mp->mask_info.tagged_union.id = id;
		    /* works the same for datatype, sender */
		break;
	    case mos_c_mask_type_msgtype:
		printf("  Mask value (msg type [(d)/n]): ");
		gets(buf);
		if (buf[0] != 'n')
		    mp->mask_info.tagged_union.msgtype =
			    mos_c_msg_attr_msgtype_data;
		break;
	    case mos_c_mask_type_protectlvl:
		printf("  Mask value (protectlvl (int)): ");
		gets(buf);
		mp->mask_info.tagged_union.protectlvl = atol(buf);
		break;
	    case mos_c_mask_type_ttr:
		break;
	}

	if (t == mos_c_mask_type_priority) {
	    printf("Relation (==, !=, <, <=, >, >=): ");
	    gets(buf);
	    if (!strcmp(buf, "!="))
		mp->relation = mos_c_mask_op_not_equal;
	    else if (!strcmp(buf, "<"))
		mp->relation = mos_c_mask_op_less_than;
	    else if (!strcmp(buf, "<="))
		mp->relation = mos_c_mask_op_eq_less_than;
	    else if (!strcmp(buf, ">"))
		mp->relation = mos_c_mask_op_greater_than;
	    else if (!strcmp(buf, ">="))
		mp->relation = mos_c_mask_op_eq_greater_than;
	    else	
		mp->relation = mos_c_mask_op_equal;
	}
	else {
	    printf("Relation (==, !=): ");
	    gets(buf);
	    if (!strcmp(buf, "!=")) mp->relation = mos_c_mask_op_not_equal;
	    else mp->relation = mos_c_mask_op_equal;
	}
	masks.count++;
	mp++;
    }

    if (a->verbose) Print_mask_list(&masks);
    return &masks;
}

/* ______________________________________________________________________ */
/* ______________________________________________________________________ */

void
do_mode(the_args *a)
{
    if (a->arg) {
	if (*a->arg == 'a') my_mode = MODE_API;
	else if (*a->arg == 'r') my_mode = MODE_RPC;
	else printf("mode value must be 'api' or 'rpc'\n");
    }
}

/* ______________________________________________________________________ */
/* ______________________________________________________________________ */

void
do_rsrc_attach(the_args *a)
{
    error_status_t	st;

    if (attached_qh) {
	mos_rsrc_detach(&attached_qh, &st);
	if (st != error_status_ok) {
	    dce_pgm_fprintf(stderr,st);
	    return;
	}
    }
    if (a->qname) strcpy(qname_buf, (a->qname));
    DefQ = (unsigned_char_t *)qname_buf;
    mos_rsrc_attach((idl_char *)a->qname, NULL, &attached_qh, &st);
    if (st != error_status_ok) {
	dce_pgm_fprintf(stderr,st);
	return;
    }
}


void
do_rsrc_detach(the_args *a)
{
    error_status_t	st;

    if (attached_qh) {
	mos_rsrc_detach(&attached_qh, &st);
	if (st != error_status_ok) {
	    dce_pgm_fprintf(stderr,st);
	    return;
	}
    }
    if (a->qname) strcpy(qname_buf, "");
    DefQ = (unsigned_char_t *)qname_buf;
}

/* ______________________________________________________________________ */
/* ______________________________________________________________________ */

void
do_r_enqueue(the_args *a)
{
    error_status_t	st;
    char		msgbuf[5000], buf[80];
    mos_mattr_t		mattr;
    mos_datatype_bytearray_t	body;
    uuid_t		id;
    unsigned_char_t	*ucp;

    mattr = null_mos_mattr;
    body  = null_mos_body;

    if (a->all) {
	char *cp = msgbuf;
	printf("Message to enqueue in '%s' (multiple lines - end with ^D):\n",
		a->qname);
	while (fgets(cp, 200, stdin)) {
	    while (*cp) cp++;
	}
    }
    else {
	printf("Message to enqueue in '%s' (1-line):\n", a->qname);
	gets(msgbuf);
    }
    body.size = strlen(msgbuf) + 1;
    body.data = (unsigned char *)msgbuf;
    mattr.priority = a->priority;
    mattr.persistence = a->pv;
    if (a->notd) {
	mattr.notice_dest = (idl_char *)a->notd;
	printf("Notice Opts (1=enq,2=deq,3=both): ");
	gets(buf);
	mattr.notice_opts = atoi(buf);
    }
    if (a->exp) {
	utc_mkasctime(&mattr.expire_time, a->exp);
	mattr.flags |= mos_c_use_expire_time;
    }

    mos_q_enqueue(bh, (idl_char *)a->qname, &mattr, &body, 0, &id, &st);
    CHECK_STATUS(st, "mos_q_enqueue", CHK_RET);
    uuid_to_string(&id, &ucp, &st);
    printf("Sent %ld bytes\n[%s]\nUUID: %s\n", body.size, body.data, ucp);
    free(ucp);
}

void
do_a_enqueue(the_args *a)
{
    error_status_t	st;
    uuid_t		id;
    mos_msg_attr_t	mattr;
    mos_msg_buf_t	mbuf;
    mos_datatype_bytearray_t	body;
    char		msgbuf[2000], *cp;

    mos_msg_attr_alloc(&mattr, &st);
    if (a->all) {
	char *cp = msgbuf;
	printf("Message to enqueue in '%s' (multiple lines - end with ^D):\n",
		a->qname);
	while (fgets(cp, 200, stdin)) {
	    while (*cp) cp++;
	}
    }
    else {
	printf("Message to enqueue in '%s' (1-line):\n", a->qname);
	gets(msgbuf);
    }
    body.size = strlen(msgbuf) + 1;
    body.data = (unsigned char *)msgbuf;
    if (a->notd) {
	char buf[80];
	printf("Notice Opts (1=enq,2=deq,3=both): ");
	gets(buf);
	mos_msg_attr_notice_set(atoi(buf), (idl_char *)a->notd, NULL, mattr,
		NULL, &st);
    }
    if (a->exp) {
	utc_t t;
	utc_mkasctime(&t, a->exp);
	mos_msg_attr_ttl_set(&t, mattr, &st);
    }
    mos_msg_attr_priority_set(a->priority, mattr, &st);
    mos_msg_attr_persistence_set(a->pv, mattr, &st);
    mos_msg_enqueue(attached_qh, 0, mattr, &body, &id, &st);
    mos_msg_attr_free(&mattr, &st);
}

/* _______________________________________ */

void
do_r_next(the_args *a)
{
    error_status_t	st;
    mos_mattr_t		mattr;
    mos_datatype_bytearray_t	body;

    mos_q_dequeue_next(bh, (idl_char *)a->qname, &mattr, &body, &st);
    CHECK_STATUS(st, "mos_q_dequeue_next", CHK_RET);
    if (a->verbose) {
	Print_mattr(&mattr);
	printf("Retrieved %ld bytes\n", body.size);
    }
    Print_body(&body, (a->hex || !uuid_is_nil(&mattr.datatype,&st)));
}

void
do_a_next(the_args *a)
{
    error_status_t	st;
    mos_msg_attr_t	mattr;
    mos_msg_buf_t	mbody;
    mos_datatype_bytearray_t	*bp;
    uuid_t		datatype;

    mos_msg_dequeue(attached_qh, 0, NULL, &mattr, &mbody, &st);
    CHECK_STATUS(st, "mos_msg_dequeue", CHK_RET);
    mos_msg_attr_datatype_get(mattr, &datatype, &st);
    Print_body(mbody, (a->hex || !uuid_is_nil(&datatype, &st)));
}

/* _______________________________________ */

void
do_r_dequeue(the_args *a)
{
    error_status_t	st;
    mos_mattr_t		mattr;
    mos_datatype_bytearray_t	body;
    mos_mask_list_t	*ml = NULL;
    unsigned32		flag;

    ml = get_mask_list(a);
    flag = (a->wait && !a->all) ? mos_c_deqmode_wait : 0;
    while (1) {
	mos_q_dequeue(bh, (idl_char *)a->qname, ml, flag, &mattr, &body, &st);
	if (a->all && st == mos_empty_queue) break;
	CHECK_STATUS(st, "mos_q_dequeue", CHK_RET);
	if (a->verbose) {
	    Print_mattr(&mattr);
	    printf("Retrieved %ld bytes\n", body.size);
	}
	Print_body(&body, (a->hex || !uuid_is_nil(&mattr.datatype,&st)));
	if (!a->all) break;
    }
}

void
do_a_dequeue(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_list(the_args *a)
{
    error_status_t	st;
    mos_uuid_list_t	idlist;
    int			i;
    mos_mask_list_t	*ml = NULL;

    if (a->all)
	mos_q_get_id_list_all(bh, (idl_char *)a->qname, &idlist, &st);
    else {
	ml = get_mask_list(a);
	mos_q_get_id_list(bh, (idl_char *)a->qname, ml, &idlist, &st);
    }
    CHECK_STATUS(st, "mos_q_get_id_list", CHK_RET);
    if (!a->all && idlist.count > 0) Print_mattr_1line_head();
    for (i=0; i<idlist.count; i++)
	get_and_print_mattr((idl_char *)a->qname, &idlist.list[i],
		(!a->verbose), &st);
}

void
do_a_list(the_args *a)
{
    uuid_t		idlist[100];
    unsigned32		i, n, nl;
    error_status_t	st;

    mos_que_mgmt_msgid_list(attached_qh, NULL, 100, &n, idlist, &nl, &st);
    if (!a->all && n > 0) Print_mattr_1line_head();
    for (i=0; i<n; i++)
	get_and_print_mattr_a((idl_char *)a->qname, &idlist[i],
		(!a->verbose), &st);
/*	Print_uuid(NULL, &idlist[i]);*/
}

/* _______________________________________ */

void
do_r_peek(the_args *a)
{
    error_status_t	st;
    mos_mattr_t		mattr;
    mos_datatype_bytearray_t	body;
    mos_mask_list_t	*ml = NULL;

    unsigned32 flag;
    flag = (a->wait || !a->all) ? mos_c_deqmode_wait : 0;

    ml = get_mask_list(a);
    mos_q_peek(bh, (idl_char *)a->qname, ml, flag, &mattr, &body, &st);
    CHECK_STATUS(st, "mos_q_peek", CHK_RET);
    if (a->verbose) {
	Print_mattr(&mattr);
	printf("Retrieved %ld bytes\n", body.size);
    }
    Print_body(&body, (a->hex || !uuid_is_nil(&mattr.datatype,&st)));
}

void
do_a_peek(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_purge(the_args *a)
{
    error_status_t	st;

    if (!a->id || !a->qname) {
	fprintf(stderr, "You must specify a queue and message ID\n");
	return;
    }
    mos_q_purge(bh, (idl_char *)a->qname, &a->uuid, &st);
    CHECK_STATUS(st, "mos_q_pourge", CHK_RET);
}

void
do_a_purge(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_ping(the_args *a)
{
    error_status_t	st;
    uuid_t		id;

    mos_q_ping(bh, (idl_char *)a->qname, &id, &st);
    if (st == error_status_ok)
	Print_uuid("  Queue exists: ", &id);
    else if (st == mos_queue_not_found)
	printf("queue '%s' does not exist\n", a->qname);
    else {
	fprintf(stderr,"mos_q_ping returned error: ");
	dce_pgm_fprintf(stderr,st);
    }
}

void
do_a_ping(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_mattr(the_args *a)
{
    error_status_t	st;
    uuid_t		id;
    mos_mattr_t		mattr;
    mos_mask_list_t	*ml = NULL;

    if (a->id) {
	uuid_from_string((unsigned_char_t *)a->id, &id, &st);
	CHECK_STATUS(st, "uuid_from_string", CHK_RET);
	get_and_print_mattr((idl_char *)a->qname, &id, FALSE, &st);
    }
    else {
	ml = get_mask_list(a);
	mos_q_retrieve_mattr(bh, (idl_char *)a->qname, ml, 0, &mattr, &st);
	CHECK_STATUS(st, "mos_q_retrieve_mattr", CHK_RET);
	Print_mattr(&mattr);
    }
}

void
do_a_mattr(the_args *a)
{
    mos_msg_attr_t	ma;
    uuid_t		id;
    unsigned32		i;
    boolean		b1, b2;
    utc_t		t;
    sec_id_pa_t		sender;
    mos_rsrc_name_t	dest;
    sec_rgy_name_t	secname;
    mos_msg_attr_msgtype_t mt;
    mos_msg_attr_persistence_t pv;
    error_status_t	st;

    mos_msg_attr_peek(attached_qh, 0, NULL, &ma, &st);

    mos_msg_attr_msgid_get(ma, &id, &st);
    Print_uuid("ID:" , &id);
    mos_msg_attr_datatype_get(ma, &id, &st);
    Print_uuid("  Datatype: ", &id);
    mos_msg_attr_secid_get(ma, &sender, &st);
    Print_sec_id_pa("  Sender:", &sender);
    mos_msg_attr_msgtype_get(ma, &mt, &st);
    printf("  Msg-type: %d\n", mt);
    mos_msg_attr_persistence_get(ma, &pv, &st);
    printf("  Persistence: %c\n", persist2pv(pv));
    mos_msg_attr_priority_get(ma, &i, &st);
    printf("  Priority: %d\n", i);
    mos_msg_attr_notice_get(ma, &i, dest, secname, &st);
    printf("  Notice-opts: 0x%x\n", i);
    if (dest) printf("  Reply-queue: %s\n", dest);
    mos_msg_attr_enqtime_get(ma, &t, &st);
    Print_utc("  Enqueued:", &t, 0);
    mos_msg_attr_ttl_get(ma, &t, &st);
    Print_utc("  Expires:", &t, 0);
    mos_msg_attr_protectlvl_get(ma, &i, &st);
/*    mos_msg_attr_ttr_get(ma, Z, &st);*/
/*    Print_utc("  Valid Starting:", &t, 0);*/

    mos_msg_attr_free(&ma, &st);
}

/* _______________________________________ */

void
do_r_delete(the_args *a)
{
    error_status_t	st;

    if (!a->qname) {
	printf("You must specify queue name.\n");
	return;
    }
    mos_q_delete_queue(bh, (idl_char *)a->qname,
	    (a->force ? mos_c_delop_force : mos_c_delop_preserve), &st);
    CHECK_STATUS(st, "mos_q_delete_queue", CHK_RET);
    printf("Deleted %s\n", a->qname);
}

void
do_a_delete(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_qattr(the_args *a)
{
    error_status_t	st;
    mos_qattr_t		qattr;

    mos_q_get_qattr_by_name(bh, (idl_char *)a->qname, &qattr, &st);
    CHECK_STATUS(st, "mos_q_get_qattr_by_name", CHK_RET);
    Print_qattr(&qattr);
}

void
do_a_qattr(the_args *a)
{
    mos_que_attr_t	qa;
    error_status_t	st;
    uuid_t		id;
    unsigned32		i;
    boolean		b1, b2;
    utc_t		t;
    mos_que_attr_persistence_t p;
    mos_que_attr_annotation_t ann;

    mos_que_attr_peek(attached_qh, &qa, &st);
    CHECK_STATUS(st, "mos_que_attr_peek", CHK_RET);

    mos_que_attr_queid_get(qa, &id, &st);
    Print_uuid("  ID: ", &id);
    mos_que_attr_annotation_get(qa, ann, &st);
    printf("  Annotation: %s\n", ann);
    mos_que_attr_quemaxsize_get(qa, &i, &st);
    printf("  Max-Length: %d\n", i);
    mos_que_attr_msgmaxsize_get(qa, &i, &st);
    printf("  Max-Msg-Size: %d\n", i);
    mos_que_attr_quecursize_get(qa, &i, &st);
    printf("  Length: %d\n", i);
    mos_que_attr_persistence_get(qa, &p, &st);
    printf("  Persistence: %c\n", persist2pvm(p));
    mos_que_attr_allowenq_get(qa, &b1, &st);
    mos_que_attr_allowdeq_get(qa, &b2, &st);
    printf("  Enabled-enq/deq: %c/%c\n", (bool2yn(b1)), (bool2yn(b2)));
    mos_que_attr_createtime_get(qa, &t, &st);
    Print_utc("  Created: ", &t, 0);
    mos_que_attr_activetime_get(qa, &t, &st);
    Print_utc("  Last-Activity: ", &t, 0);
    mos_que_attr_idletimeout_get(qa, &t, &st);
    Print_utc("  Idle-Timeout: ", &t, 1);

    mos_que_attr_free(&qa, &st);
}
/* _______________________________________ */

void
do_r_edit_qattr(the_args *a)
{
    error_status_t	st;
    mos_qattr_t		qattr;

    mos_q_get_qattr_by_name(bh, (idl_char *)a->qname, &qattr, &st);
    CHECK_STATUS(st, "mos_q_get_qattr_by_name", CHK_RET);
    printf("++++++++++ Current attributes:\n");
    Print_qattr(&qattr);
    printf("\nInput new values:\n");
    edit_qattr(&qattr, FALSE);
    printf("++++++++++ New attributes:\n");
    Print_qattr(&qattr);
    mos_q_set_qattr_by_name(bh, (idl_char *)a->qname, &qattr, &st);
    CHECK_STATUS(st, "mos_q_set_qattr_by_name", CHK_RET);
}

void
do_a_edit_qattr(the_args *a)
{
    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_queues(the_args *a)
{
    error_status_t	st;
    mos_string_list_t	names;
    mos_qattr_t		qattr;
    unsigned32		i;

    mos_mgmt_get_name_list(bh, &names, &st);
    if (names.count == 0) return;
    if (!a->all) Print_qattr_1line_head();
    for (i=0; i<names.count; i++) {
	if (a->all) printf("==============\n");
	mos_q_get_qattr_by_name(bh, (idl_char *)names.list[i], &qattr, &st);
	CHECK_STATUS(st, "mos_q_get_qattr_by_name", CHK_RET);
	if (!a->all) Print_qattr_1line(&qattr);
	else Print_qattr(&qattr);
    }
}

void
do_a_queues(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_create(the_args *a)
{
    error_status_t	st;
    idl_char		*qn;
    char		buf[100];
    mos_qattr_t		qattr;

    if (a->qname) qn = (idl_char *)a->qname;
    else {
	printf("New queue name: ");
	gets(qname_buf);
	qn = (idl_char *)qname_buf;
    }
    edit_qattr(&qattr, TRUE);
    qattr.name = qn;
    mos_mgmt_create_queue(bh, &qattr, &st);
    if (st == mos_queue_already_exists) {
	printf("Queue by that name or alias already exists:\n");
	Print_uuid(buf, &qattr.id);
	if (a->verbose) Print_qattr(&qattr);
    }
    CHECK_STATUS(st, "mos_mgmt_create_queue", CHK_RET);
    sprintf(buf, "Created %s - ", (char *)qattr.name);
    Print_uuid(buf, &qattr.id);
    if (a->verbose) Print_qattr(&qattr);
}

void
do_a_create(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_rename(the_args *a)
{
    error_status_t	st;
    idl_char newname[100], oldname[100];

    printf("Old name: ");
    gets((char *)oldname);
    printf("New name: ");
    gets((char *)newname);
    mos_mgmt_rename(bh, (idl_char *)oldname, newname, &st);
    CHECK_STATUS(st, "mos_mgmt_rename", CHK_RET);
}

void
do_a_rename(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_info(the_args *a)
{
    error_status_t	st;
    mos_kv_pair_list_t	info;
    unsigned32		i;

    mos_mgmt_info(bh, &info, &st);
    CHECK_STATUS(st, "mos_mgmt_rename", CHK_RET);
    puts("Local (client side):");
    printf("Default queue: %s\n", DefQ);
    printf("NS entry: %s\n", ns_entry);
    puts("--------");
    puts("Queue manager:");
    for (i=0; i<info.count; i++) {
	printf("%-20s ", info.list[i].key);
	switch (info.list[i].value.type) {
	    case mos_c_info_string:
		printf("%s\n", info.list[i].value.tagged_union.s);
		break;
	    case mos_c_info_u32:
		printf("%ld\n", info.list[i].value.tagged_union.i);
		break;
	    case mos_c_info_uuid:
		Print_uuid("", &info.list[i].value.tagged_union.uuid);
		break;
	    case mos_c_info_utc:
		Print_utc("", &info.list[i].value.tagged_union.t, 0);
		break;
	}
    }
}

void
do_a_info(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_r_catalog(the_args *a)
{
    char		buf[200];
    error_status_t	st;
    mos_uuid_list_t	idlist;
    mos_string_list_t	names;
    unsigned32		i;

    mos_mgmt_get_id_list(bh, &idlist, &st);
    CHECK_STATUS(st, "mos_mgmt_get_id_list", CHK_RET);
    mos_mgmt_get_name_list(bh, &names, &st);
    CHECK_STATUS(st, "mos_mgmt_get_name_list", CHK_RET);
    if (idlist.count == 0) {
	printf("No queues\n");
	return;
    }
    printf("NN %-38s UUID\n", "Queue Name");
    for (i=0; i<names.count; i++) {
	sprintf(buf, "%2ld %-38.38s ", i, names.list[i]);
	Print_uuid(buf, &idlist.list[i]);
    }
}

void
do_a_catalog(the_args *a)
{
    mos_rsrc_name_t	qnames[100];
    unsigned32		i, n, n_left;
    error_status_t	st;

    mos_que_mgmt_quename_list(attached_qh, 100, &n, qnames,
	    &n_left, &st);
    CHECK_STATUS(st, "mos_que_mgmt_quename_list", CHK_RET);
    for (i=0; i<n; i++) {
	printf("%2ld %-38.38s\n", i, qnames[i]);
    }
}
/* _______________________________________ */

void
do_r_move(the_args *a)
{
    char		from[80], to[80];
    error_status_t	st;
    mos_uuid_list_t	idlist;
    mos_string_list_t	names;
    unsigned32		i;

    printf("Move items:\n  from queue: ");
    gets(from);
    printf("  to queue: ");
    gets(to);
    mos_mgmt_move(bh, (idl_char *)from, (idl_char *)to, &st);
    CHECK_STATUS(st, "mos_mgmt_move", CHK_RET);
}

void
do_a_move(the_args *a)
{
    error_status_t	st;

    printf("Not yet implemented for this mode (%s).\n", mode2str(my_mode));
}

/* _______________________________________ */

void
do_rebind(the_args *a)
{
    bind_to_server();
}

/* _______________________________________ */

void
do_help(the_args *a)
{
    fputs(help_msg, stderr);
}

/* _______________________________________ */

void
do_defaultq(the_args *a)
{
    if (a->qname) strcpy(qname_buf, (a->qname));
    else {
	printf("Default queue name: ");
	gets(qname_buf);
    }
    DefQ = (unsigned_char_t *)qname_buf;
}

/* _______________________________________ */

void
do_stop_server(the_args *a)
{
    error_status_t	st;
    rpc_mgmt_stop_server_listening(bh, &st);
    CHECK_STATUS(st, "rpc_mgmt_stop_server_listening", CHK_OK);
    printf("Server stopped listening\n");
    return;
}

void
not_applic(the_args *a)
{
    printf("Not applicable to the mode your are in (%s)\n", mode2str(my_mode));
}

/* _______________________________________ */

Command command[] = {
	/* command   min_match abbr     function */
	{ "enqueue",	2,	"e",	do_r_enqueue,	do_a_enqueue	},
	{ "next",	2,	"n",	do_r_next,	do_a_next	},
	{ "dequeue",	3,	"d",	do_r_dequeue,	do_a_dequeue	},
	{ "peek",	2,	"k",	do_r_peek,	do_a_peek	},
	{ "purge",	2,	"x",	do_r_purge,	do_a_purge	},
	{ "ping",	2,	"p",	do_r_ping,	do_a_ping	},
	{ "list",	2,	"l",	do_r_list,	do_a_list	},
	{ "mattr",	2,	"a",	do_r_mattr,	do_a_mattr	},
	{ "delete",	3,	"D",	do_r_delete,	do_a_delete	},
	{ "qattr",	2,	"A",	do_r_qattr,	do_a_qattr	},
	{ "edit",	2,	"E",	do_r_edit_qattr, do_a_edit_qattr},
	{ "queues",	2,	"*",	do_r_queues,	do_a_queues	},

	{ "catalog",	2,	"L",	do_r_catalog,	do_a_catalog	},
	{ "move",	3,	"M",	do_r_move,	do_a_move	},
	{ "create",	2,	"C",	do_r_create,	do_a_create	},
	{ "rename",	3,	"N",	do_r_rename,	do_a_rename	},
	{ "info",	2,	"I",	do_r_info,	do_a_info	},

	{ "default",	3,	"Q",	do_defaultq,	do_defaultq	},
	{ "rebind",	3,	"B",	do_rebind,	do_rebind	},
	{ "help",	2,	"?",	do_help	,	do_help		},
	{ "stop",	2,	"X",	do_stop_server,	do_stop_server	},
	{ "mode",	3,	NULL,	do_mode	,	do_mode		},

	{ "attach",	3,	NULL,	not_applic,	do_rsrc_attach	},
	{ "detach",	3,	NULL,	not_applic,	do_rsrc_detach	},

	{ NULL,		0,	NULL,	NULL, 	NULL	}
};

/* _______________________________________ */

void
do_one_cmd(char *cmdbuf)
{
    int			argc, l;
    char		**argv;
    the_args		a;
    Command		*cmd;

    argc = 0;
    argv = tokenize(cmdbuf, &argc, 0);
    if (argc == 0) return;
    parse_cmd(argc, argv, &a);

    if (!a.qname) a.qname = (char *)DefQ;

    for (cmd=command; cmd->cmd; cmd++) {
	l = strlen(argv[0]);
	if (cmd->min_match > l) l = cmd->min_match;
	if (!strncmp(argv[0], cmd->cmd, l) ||
		( cmd->abbrev && !strcmp(argv[0], cmd->abbrev)) ) {
	    if (my_mode == MODE_RPC || a.mode_rpc)
		cmd->op_rpc(&a);
	    else
		cmd->op_api(&a);
	    return;
	}
    }
    printf("unknown command: %s\n", argv[0]);
}


int
main(int ac, char **av)
{
    error_status_t	st;
    char		*path, prompt[80];

    path = av[0];

    mos_svc_handle = dce_svc_register(mos_svc_table, (idl_char *)"mos", &st);
    dce_svc_set_progname(path, &st);
    dce_msg_define_msg_table(mos_svc_g_table,
	sizeof mos_svc_g_table / sizeof mos_svc_g_table[0], &st);

    set_defaults_and_parse_args(ac, av);

    bind_to_server();
    if (one_cmd) {
	if (qname) { DefQ = qname; }
	do_one_cmd(one_cmd);
    }
    else {
	char		cmdbuf[80], *cp, *modestring, *q;

	if (qname) { DefQ = qname; }
	else {
	    DefQ = (unsigned_char_t *)"default";
	}
	while (1) {
	    if (my_mode == MODE_RPC) {
		modestring = "RPC";
		q = (char *)DefQ;
	    }
	    else {
		modestring = "API";
		q = (attached_qh != NULL) ? (char *)DefQ : "UNATTACHED";
	    }
	    sprintf(prompt, "%s %s => ", modestring, q);
	    cp = readline(prompt);
	    if (!cp || (cp[0] == 'q' && cp[1] == '\0')) break;
	    add_history(cp);
	    strcpy(cmdbuf, cp);
	    do_one_cmd(cmdbuf);
	}
    }
    return 0;
}

/* ______________________________________________________________________ */

static void
set_defaults_and_parse_args(int ac, char **av)
{
    int c;

    /* Defaults */

    /* Args */

    while ((c = getopt(ac, av, "q:e:b:c:h")) != EOF) {
	switch (c) {
	default:  fprintf(stderr, "Huh?\n");		break;
	case 'e': ns_entry = (idl_char *)optarg;	break;
	case 'q': qname = (idl_char *)optarg;		break;
	case 'b': string_binding = (idl_char *)optarg;	break;
	case 'c': one_cmd = optarg;			break;
	case 'h': printf(
	    "  -e ns_entry   NS entry to bind to\n"
	    "  -q qname      default queue name\n"
	    "  -c cmd        single command to run, the exit\n"
	    "  -b string     string binding\n"
	    "  -h            print this help message\n" );
	    break;
	}
    }
}

/* ______________________________________________________________________ */

static char *help_msg =
"Commands:         (abbreviations in parentheses, all take [-q qname] arg):\n"
"Per queue:\n"
"  enqueue (e) [-p pri] [-n notd] [-x expire] [-s pv]\n"
"                              -  enqueue\n"
"  dequeue (d) [-hwavF]        -  dequeue (a=do all, h=hex dump, w=wait)\n"
"  next    (n) [-hv]           -  dequeue next (h=hex dump)\n"
"  peek    (k) [-hvF] [-i id]  -  peek at queue item (h=hex dump)\n"
"  ping    (p)                 -  ping queue (see if it exists)\n"
"  purge   (x)                 -  purge item in queue\n"
"  list    (l) [-a]            -  list IDs of messages in queue (a=all)\n"
"  mattr   (a) [-F] [-i id]    -  show attributes of an item in queue\n"
"  qattr   (A)                 -  show attributes of queue\n"
"  delete  (D) [-f]            -  delete queue (f=force)\n"
"  edit    (E)                 -  edit attributes of queue\n"
"  queues  (*) [-a]            -  show info on all queues (a=all info)\n"
"        -a = all, -F = filter, -h = hex dump, -v = verbose\n"
"Management:\n"
"  create  (C) [-S pvm]        -  create queue\n"
"  catalog (L)                 -  list IDs of queues\n"
"  move    (M)                 -  move items from one queue to another\n"
"  info    (I)                 -  info about all queues\n"
"  rename  (N)                 -  rename a queue\n"
"Misc:\n"
"  default (Q)                 -  set default queue\n"
"  verbose (V)                 -  toggle verbose mode\n"
"  rebind  (B)                 -  rebind to server\n"
"  stop    (X)                 -  server exit\n";

/* ______________________________________________________________________ */
