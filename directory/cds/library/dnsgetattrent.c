/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsgetattrent.c,v $
 * Revision 1.1.11.2  1996/02/18  19:34:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:07  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:24:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:14  root]
 * 
 * Revision 1.1.8.4  1994/06/30  19:17:08  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:11  mccann]
 * 
 * Revision 1.1.8.3  1994/06/09  18:41:44  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:55  devsrc]
 * 
 * Revision 1.1.8.2  1994/04/29  15:53:47  peckham
 * 	Remove unnecessary conditionals.
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	Don't use dnstables.
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  14:19:55  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  22:06:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:29:38  peckham]
 * 
 * Revision 1.1.6.3  1992/12/30  13:56:53  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:11:50  zeliff]
 * 
 * Revision 1.1.6.2  1992/09/29  18:52:05  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:15:39  weisman]
 * 
 * Revision 9.5.1.2  92/08/25  14:42:49  hinman
 * 	[hinman for ohara] - Fold backward from DCE1TOP
 * 
 * Revision 8.1.1.4  92/08/05  17:18:12  hinman
 * 	[hinman] - Fold in from Abby
 * 
 * Revision 8.1.6.2  92/08/05  11:55:22  ohara
 * 	svr4 changes
 * 
 * Revision 9.5.1.1  92/07/24  09:21:39  hinman
 * 	Create merge branch
 * 
 * Revision 9.5  92/07/24  09:21:37  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.4  1992/05/20  02:52:41  peckham
 * 	CR 3223: A malloc()ed structure in attrent_file() was not being
 * 	completely initialized, causing SEGVs for some circumstances.
 * 	[1992/05/19  18:22:56  peckham]
 * 
 * Revision 1.1.2.3  1992/05/14  21:36:19  weisman
 * 		Changed up use of pthread_cleanup_push.
 * 	[1992/05/14  21:35:37  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  22:21:19  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:31:55  weisman]
 * 
 * Revision 1.1  1992/01/19  15:19:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULE: dnsgetattrent.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 */

#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dce/dce.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <dce/cdsclerk.h>
#include <dce/utc.h>
#include <dns_strings.h>
#include <uc_clerk.h>     

/* missing from the strings library */
#define strpspn(s1,s2) (s1 + strspn(s1, s2))

/*
 * Stack of files, the latest one first, never released
 */
typedef struct file_s {
    struct file_s	*fl_link;	/* link to older "lost" file */
    attrent_t		*fl_attrent_p;	/* vector of attributes */
    time_t		fl_mtime;	/* file modification time */
    /* followed by data from file */
} file_t;

typedef struct atype_s {		/* attribute text -> value mappings */
    int type;
    char value[4];
} atype_t;

typedef struct parsing_context_s {	/* used while parsing file contents */
    char	*pc_p;			/*  current place during parsing */
    int		pc_terminate;		/*  null-terminate all tokens */
} parsing_context_t;

/* local definitions and structures */

/*
 * Parsing constants
 */
static char term[] = "#\n \t";	/* line terminators */
#define WHITESPACE &term[2]	/* token terminators */

static atype_t syntaxes[] = {	/* attribute syntaxes: */
    { UNSPEC, "-"},		/*  unspecified */
    { CES, "CES"},		/*  caseExactString */
    { CIS, "CIS"},		/*  caseIgnoreString */
    { PS, "PS"},		/*  printableString */
    { NS, "NS"},		/*  numericString */
    { -1, 0} };
static atype_t matchingrules[] = {
    { UNSPEC, "-"},		/*  unspecified */
    { CEM, "CEM"},		/*  case exact */
    { CIM, "CIM"},		/*  case ignore */
    { PM, "PM"},		/*  print string */
    { NM, "NM"},		/*  numeric string */
    { -1, 0} };

static file_t *attrent_file_p;	/* currently active file */

static pthread_once_t attrent_once = pthread_once_init;
static pthread_mutex_t attrent_mutex;
static pthread_cond_t attrent_cond;
static int attrent_checking_file;



static attrent_t *
attrent_table (
	       void);

static file_t *
attrent_file (
	      char	*);

static file_t *
attrent_parse (
	       file_t	*);

static char *
attrent_tok (
	     parsing_context_t *);

static int
attrent_case_cmp (
    char	*const,
    char	*const);

static void
attrent_cleanup_fd (
		    int *);

static void
attrent_cleanup_flag (
		      void *);

/*
 * Initialize this module's threads statics
 */

static void 
attrent_init (void)
{
    register int th_status;

    th_status = pthread_mutex_init(&attrent_mutex, pthread_mutexattr_default);
    if (th_status < 0) 
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)th_status);
    }

    th_status = pthread_cond_init(&attrent_cond, pthread_condattr_default);
    if (th_status < 0) 
    {
        dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)th_status);
    }

}

attrent_t *
cdsGetXAttrByString (unsigned char *nam_p)
{
    register attrent_t *h_p = attrent_table();

    if (h_p)
	for (; h_p->objid; h_p++)
	    if (attrent_case_cmp(h_p->tag, (char *)nam_p) == 0)
		return(h_p);

    return(NULL);
}

attrent_t *
cdsGetXAttrByObjID (unsigned char *objid_p)
{
    register attrent_t *h_p = attrent_table();

    if (h_p)
	for (; h_p->objid; h_p++)
	    if (strcmp(h_p->objid, (char *)objid_p) == 0)
		return(h_p);

    return(NULL);
}

/*
 */

static attrent_t *
attrent_table (void)
{
    char filename[_DNS_MAXPATHLEN+1];
    file_t *f_p;

    /* first time thru, initialize threads structs */
    pthread_once(&attrent_once, attrent_init);

    pthread_mutex_lock(&attrent_mutex); {
	while (TRUE) {
	    /*
	     * If noone else os verifying if file has changed, volunteer.
	     */
	    if (!attrent_checking_file) {
		attrent_checking_file = TRUE;
		f_p = NULL;
		break;
	    }
	    /*
	     * Someone else is checking - use old one if there.
	     */
	    f_p = attrent_file_p;
	    if (f_p)
		break;

	    /*
	     * Wait for other to finish checking.
	     */
	    pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock,
				 (pthread_addr_t)&attrent_mutex);
		pthread_cond_wait(&attrent_cond, &attrent_mutex);
	    pthread_cleanup_pop(0);
	}
    } pthread_mutex_unlock(&attrent_mutex);

    /*
     * Use old values if new ones aren't available yet.
     */
    if (f_p) return(f_p->fl_attrent_p);

    sprintf(filename, CDS_GDS_ATTR_FILE, dcelocal_path);
    /*
     * Make sure cancel won't leave things locked.
     */
    pthread_cleanup_push((void (*)(void *))attrent_cleanup_flag, NULL);

    /*
     * If we have read the file once, check if it has been
     * modified since our last reading.
     */
    f_p = attrent_file_p;
    if (f_p) {
	struct stat filestat;

	if (stat(filename, &filestat) < 0)
	    f_p = NULL;

	if (f_p->fl_mtime != filestat.st_mtime)
	    f_p = NULL;
    }

    /*
     * If it has been modified (or is new), read the file and parse it.
     */
    if (!f_p) {
	f_p = attrent_file(filename);
	if (f_p)
	    f_p = attrent_parse(f_p);
    }

    /* we will release attrent_checking_file ourselves */
    pthread_cleanup_pop(0);

    pthread_mutex_lock(&attrent_mutex); {
	if (f_p) {
	    /*
	     * Insert new copy if there is one.
	     */
	    if (f_p != attrent_file_p) {
		f_p->fl_link = attrent_file_p;
		attrent_file_p = f_p;
	    }
	} else
	    /*
	     * Use old copy if there is one.
	     */
	    f_p = attrent_file_p;

	attrent_checking_file = FALSE;
	pthread_cond_signal(&attrent_cond);
    } pthread_mutex_unlock(&attrent_mutex);

    if (f_p)
	return(f_p->fl_attrent_p);
    else
	return(NULL);
}

/*
 * Allocate memory for file header and data,
 * and read in the data.
 */
static file_t *
attrent_file (char *file_p)
{
    file_t *f_p = NULL;
    int fd;

    /* CANCEL POINT */
    if (0 <= (fd = open(file_p, O_RDONLY, 0))) {
	struct stat filestat;

	/*
	 * Make sure fd is recovered if cancelled.
	 */
	pthread_cleanup_push((void (*)(void *))attrent_cleanup_fd,
			     (pthread_addr_t)&fd);

	/* CANCEL POINT ?? */
	if (0 <= fstat(fd, &filestat)) {
	    f_p = (file_t *)malloc(sizeof(file_t)+filestat.st_size+2);
	    if (f_p) {
		char *const buf_p = (char *)&f_p[1];
		int size = filestat.st_size;

		f_p->fl_attrent_p = 0;
		f_p->fl_mtime = filestat.st_mtime;

		if (size) {
		    char *next_p = buf_p;
		    int status;

		    /*
		     * Make sure buffer is recovered if cancelled.
		     */
		    pthread_cleanup_push((void (*)(void *))free,
					 (pthread_addr_t)f_p);

		    do {
			/* CANCEL POINT */
			if ((status = read(fd, next_p, size)) < 0) {
			    free(f_p);
			    f_p = NULL;
			    break;
			}
		    } while (next_p += status, size -= status);

		    /* no danger of cancellation now */
		    pthread_cleanup_pop(0);

		    if (f_p) {
			if (next_p[-1] != '\n')
			    *next_p++ = '\n';

			*next_p = '\0';
		    }
		} else
		    *buf_p = '\0';
	    }
	}
	pthread_cleanup_pop(0);
	(void)close(fd);
    }
    return(f_p);
}

/*
 * parse cds_globalnames
 */
static file_t *
attrent_parse (file_t *f_p)
{
    parsing_context_t pc;
    int size = 0;

    pc.pc_terminate = 0;	/* don't terminate tokens on first pass */

    while (TRUE) {
	char *p = (char *)&f_p[1];	/* beginning of buffer */
	attrent_t *ae_p = f_p->fl_attrent_p; /* maybe attrent vector */

	while (*p) {
	    attrent_t ae;
	    register char *tp;
	    int i;

	    pc.pc_p = p;
	    p = strchr(p, '\n') + 1;	/* set to next line */
	    if (*pc.pc_p == '#')	/* fast ignore of comment lines */
		continue;

	    ae.objid = attrent_tok(&pc);	/* OID */
	    ae.tag = attrent_tok(&pc);		/* LABEL */
	    ae.ident = attrent_tok(&pc);	/* ASN.1-IDENTIFIER */
	    tp = attrent_tok(&pc);		/* SYNTAX */
	    if (tp) {
		for (i = 0; 0 <= syntaxes[i].type; i++)
		    if (!attrent_case_cmp(tp, syntaxes[i].value))
			break;

		ae.syntax = syntaxes[i].type;
	    }
	    tp = attrent_tok(&pc);		/* MATCHING */
	    if (tp) {
		for (i = 0; 0 <= matchingrules[i].type; i++)
		    if (!attrent_case_cmp(tp, matchingrules[i].value))
			break;

		ae.matchingrule = matchingrules[i].type;
	    }
	    /*
	     * If any parsing failed, the pointer will have been nullified.
	     */
	    if (pc.pc_p)
		/*
		 * First pass - figure size of attrent vector.
		 * Second pass - copy into list.
		 */
		if (!ae_p)
		    size += sizeof(*ae_p);
		else
		    *ae_p++ = ae;
	}
	if (ae_p) {
	    /*
	     * Last entry has a null objid.
	     */
	    ae_p->objid = 0;
	    return(f_p);
	}
	/*
	 * End of first pass - allocate attrent vector.
	 */
	f_p->fl_attrent_p = (attrent_t *)malloc(size + sizeof(*ae_p));
	if (f_p->fl_attrent_p) {
	    pc.pc_terminate = 1;	/* terminate tokens now */
	    continue;
	}
	/*
	 * Sour grapes - can't get attrent vector, so toss the whole thing out!
	 */
	free(f_p);
	return(NULL);
    }
}

/*
 * Parse tokens and optionally terminate with null.
 */
static char *
attrent_tok (parsing_context_t *pc_p)
{
    register char *p, *q = pc_p->pc_p;

    /* quit now if previous parsing failed */
    if (!q) return(0);

    /* skip leading WHITESPACE */
    p = strpspn(q, WHITESPACE);

    switch (*p) {
    /* error if line terminator */
    case '#':
    case '\n':
    case '\0':
	return(pc_p->pc_p = 0);
    default:
	/* token bounded by next terminator or whilespace */
	if ((q = strpbrk(p, term)) && pc_p->pc_terminate)
	    *q++ = '\0';

	/* save new position */
	pc_p->pc_p = q;
    }
    return(p);
}

/*
 * Our case-newtral comparison
 */
static int 
attrent_case_cmp (char *const  str1,
                  char *const  str2)
{
    register unsigned char *p1 = (unsigned char *)str1;
    register unsigned char *p2 = (unsigned char *)str2;

    do {
	const char c1 = isupper(*p1) ? *p1 : toupper(*p1);
	const char c2 = isupper(*p2) ? *p2 : toupper(*p2);
	const int value = c1 - c2;

	if (value)
	    return(value);
    } while (p1++, *p2++);

    return(0);
}

static void 
attrent_cleanup_fd (int *fd)
{
    (void)close(*fd);
}

static void 
attrent_cleanup_flag (void *unused)
{
    pthread_mutex_lock(&attrent_mutex); {
	attrent_checking_file = FALSE;
	pthread_cond_signal(&attrent_cond);
    } pthread_mutex_unlock(&attrent_mutex);
}
