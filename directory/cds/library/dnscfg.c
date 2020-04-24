/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscfg.c,v $
 * Revision 1.1.11.2  1996/02/18  19:32:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:24  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:21:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:16  root]
 * 
 * Revision 1.1.8.4  1994/06/30  19:16:36  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:14  mccann]
 * 
 * Revision 1.1.8.3  1994/06/09  18:41:02  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:20  devsrc]
 * 
 * Revision 1.1.8.2  1994/04/29  15:52:24  peckham
 * 	Remove unnecessary conditionals.
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  14:15:50  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  22:06:04  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:26:14  peckham]
 * 
 * Revision 1.1.6.3  1992/12/30  13:54:27  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:08:13  zeliff]
 * 
 * Revision 1.1.6.2  1992/09/29  18:51:25  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:15:22  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  22:17:36  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:30:48  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: dnscfg.c
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
 * Module to support access to alternate configuration information
 * in a given file.
 *
 * The format of the file is a subset of the X defaults file. Entries
 * are composed of the program name, a "." seperator, and then the parameter
 * name (leading "dns."/"cds." was required before generation 8). This is
 * immediately followed by ":", white space, and then the value. There may
 * be a single wildcard (*) inserted anywhere within the parameter name list,
 * which will match zero or more names. Blank lines are ignored.
 * Comments start with a "!" in the first column and end with newline.
 * This syntax is NOT guaranteed to be fully enforced.
 *
 * The access routines are patterned after the ultrix internationalization
 * routines, and are somewhat inefficient (first pass). The user first
 * opens the file using dns_cfgopen(), accesses parameters via dns_cfgtostr()
 * dns_cfgtol() and dns_cfgtoarg(), and then closes using dns_cfgclose().
 * An opaque handle is used while the file is open.
 *
 * Provision is made to define program-wide file defaults via dns_cfginit().
 * Any general service modules (e.g. threads) should use these defaults.
 *
 * New service provided to merge new values into a program-specific file,
 * motivated by a need in the cds server. When dns_cfgopen() is done, read from
 * both general file and program-specific file (e.g. cds.conf & .cdsdrc).
 * When the file has been opened by dns_cfgopen(), values to be written out
 * are defined with dns_cfgfromstr() and dns_cfgfroml() calls, which can be
 * subsequently written with the dns_cfgwrite() call before the dns_cfgclose().
 * The program-specific file is re-created with no retention of any commenting.
 *
 * TO DO:
 *	This code is not fully mutex protected.
 *	This code assumes there is only one program of a given name
 *	running on a system at any given time.
 *
 *
 */

#include <pthread.h>
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/assert.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <dns_cfg.h>
#include <dns_malloc.h>
#include <dns_strings.h>
#include <dce/dns_record.h>
#include <string_lib.h>
#include <threads_lib.h>


typedef struct cfgname_s {	/* name construction and match buffer */
    struct cfgline_s *cfn_best_p;	/* best match so far */
    char	*cfn_end_p;		/* pointer to end of name in cfn_name */
    short	cfn_names;		/* count of names in cfn_name */
    short	cfn_best;		/* names matched by wildcard */
    char	cfn_wild;		/* name contains a wildcard */
    char	cfn_name[512];		/* buffer for full name */
} cfgname_t;

typedef struct cfgline_s {	/* per parameter line in file */
    struct cfgline_s *cfl_link;		/* FIRST: link to next list */
    char	*cfl_source;		/* pointer to source line */
    char	*cfl_name1;		/* anything before wildcard "*" */
    char	*cfl_name2;		/* anything after wildcard "*" */
    char	*cfl_data;		/* pointer to text value */
    short	cfl_len1;		/* length of cfl_name1 */
    short	cfl_len2;		/* length of cfl_name2 */
    int		cfl_names;		/* count of names in cfl_name[12] */
    short	cfl_argc;		/* if data has been parsed */
    char	cfl_alloc;		/* cfl_data points to malloc() data */
    char	cfl_modified;		/* cfl_data has been changed */
} cfgline_t;

typedef struct cfgpgm_s {	/* per program in file */
    struct cfgpgm_s *cfp_link;	/* linked through cff_cfg */
    struct cfgfile_s *cfp_cff;	/* master cfgfile */
    struct cfgfile_s *cfp_cpf;	/* program cfgfile */
    int		cfp_users;	/* outstanding opens on program */
    char	cfp_pgm[256];	/* program name */
} cfgpgm_t;

typedef struct cfgfile_s {	/* per configuration file */
    struct cfgfile_s *cff_link;	/* First: linked through cff_list */
    struct cfgpgm_s *cff_cfg;	/* assoctiated programs */
    time_t	cff_mtime;	/* for file modification check */
    char	*cff_data;	/* file data */
    char	*cff_end;	/* file data end */
    struct cfgline_s *cff_line;	/* pointer to line list */
    struct cfgline_s *cff_line_e; /* pointer to last line or cff_line */
    short	cff_users;	/* outstanding opens on file */
    unsigned short   cff_mode;	/* mode of file */
    char	cff_nonl;	/* last character in file is not "" */
    char	cff_modified;	/* file has been modified */
    char	cff_file[1024];	/* file name */
} cfgfile_t;

/* local routines */
static void
dns_cfgopen_cleanup (
    void	*);

static cfgfile_t *
cfg_fileopen (
    char	*);

static void
cfg_fileopen_cleanup (
    void	*);

static void
cfg_fileclose (
    cfgfile_t	*);

static void
cfg_cff_from_file (
    cfgfile_t	*);

static void
cfg_cff_from_file_cleanup (
    void	*);

static cfgpgm_t *
cfg_pgmopen (
    cfgfile_t	*,
    char	*);

static void
cfg_pgmclose (
    cfgpgm_t	*);

static char *
cfg_parse_line (
    cfgfile_t	*,
    char	*,
    int		 );

static int
cfg_defn (
    cfgpgm_t	*,
    char	*,
    char	*);

static cfgline_t *
cfg_to_cfl (
    cfgfile_t	*,
    cfgname_t	*);

static cfgname_t *
cfg_name_to_cfn (
    cfgpgm_t	*,
    cfgname_t	*,
    char	*);

static void
cfg_append (
    cfgname_t	*,
    char	*);

static char *
cfg_basename (
    char	*);

/* local data */

static char confpfx[] = CDS_CONF_PFX;
#define LEN_CONFPFX (sizeof(confpfx)-1)

static char breaks[] = " \t";
static pthread_once_t cfg_once = pthread_once_init;
static pthread_mutex_t cfg_mutex;
static pthread_cond_t cfg_cond;
static cfgfile_t *cff_list;	/* list of configuration files */
static cfgfile_t *default_cff;	/* cache the default file */
static cfgpgm_t *default_cfg;	/* cache the default program */
static int cff_busy;		/* cff_list being updated */
static char default_file[1024];
static char default_pgm[256];

#define CFG_INIT	{ pthread_once(&cfg_once, cfg_init); }
#define CFG_LOCK	{ pthread_mutex_lock(&cfg_mutex); }
#define CFG_END_LOCK	{ pthread_mutex_unlock(&cfg_mutex); }
#define CFG_PUSH(r,a)	  pthread_cleanup_push(r,a)
#define CFG_POP(e)	  pthread_cleanup_pop(e)
#define CFG_WAIT	{ CFG_PUSH(pthread_mutex_unlock, &cfg_mutex); \
			  pthread_cond_wait(&cfg_cond, &cfg_mutex); \
			  CFG_POP(0); }
#define CFG_BROADCAST	{ pthread_cond_broadcast(&cfg_cond); }

/*
 * cfg_init
 *	One-time threads initialization
 */
static void 
cfg_init (void)
{
    int expn_result;
    
    expn_result = pthread_mutex_init(&cfg_mutex, pthread_mutexattr_default);
    dce_assert(cds__svc_handle, 0 <= expn_result);

    expn_result = pthread_cond_init(&cfg_cond, pthread_condattr_default);
    dce_assert(cds__svc_handle, 0 <= expn_result);
    cff_list = NULL;
    cff_busy = 0;
}

/*
 * dns_cfginit
 *	Set up default configuration file and current program.
 *
 * Inputs:
 *	file_p = pointer to configuration file name, or zero for default.
 *	pgm_p = pointer to program name, or zero for default.
 */
void 
dns_cfginit (char  *file_p,
             char  *pgm_p)
{
    if (file_p) {
	if (*file_p != '/')
	    sprintf(default_file, CDS_ETC_DIR, dcelocal_path, file_p);
	else
	    strcpy(default_file, file_p);
    } else
	sprintf(default_file, CDS_CONF_FILE, dcelocal_path);

    if (pgm_p)
	strcpy(default_pgm, cfg_basename(pgm_p));
    else
	strcpy(default_pgm, "*no program*");
}

/*
 * dns_cfgopen
 *	Open the configuration file.
 *
 * Inputs:
 *	file_p = pointer to configuration file name, or zero for default.
 *	pgm_p = pointer to program name, or zero for default.
 *
 * Result:
 *	Pointer to opaque file handle, or zero if failure.
 */
char 
*dns_cfgopen (char  *file_p,
              char  *pgm_p)
{
    register cfgpgm_t *cfp_p = NULL;
    register cfgfile_t *cff_p;

    CFG_INIT;

    /* CANCEL POINT - no local resources affected by cancel */
    if (cff_p = cfg_fileopen(file_p)) {

	/* CANCEL POINT - cff_p must be released */
	CFG_PUSH(dns_cfgopen_cleanup, cff_p);

	cfp_p = cfg_pgmopen(cff_p, cfg_basename(pgm_p));
	if (!cfp_p)
	    cfg_fileclose(cff_p);

	CFG_POP(0);
    }

    return((char *)cfp_p);
}

static void 
dns_cfgopen_cleanup (void *cff_p)
{
    cfg_fileclose(cff_p);
}

/*
 * dns_cfgwrite
 *	Merge new definitions into configuration file
 *
 * Inputs:
 *	handle = pointer to configuration handle from dns_cfgopen().
 *
 * Result:
 *	TRUE if write succeeded, otherwise no action taken.
 */
int 
dns_cfgwrite (char *handle)
{
    register cfgpgm_t *cfp_p;

    CFG_INIT;

    if (cfp_p = (cfgpgm_t *)handle) {
	cfgfile_t *cff_p = cfp_p->cfp_cpf;

	if (cff_p->cff_modified) {
	    register int fd;

	    if (0 <= (fd = open(cff_p->cff_file, O_CREAT|O_WRONLY|O_APPEND, cff_p->cff_mode))) {
		register cfgline_t *cfl_p = cff_p->cff_line;

		do {
		    if (cfl_p->cfl_modified) {
			char buffer[1000];
			char *ptr = buffer;
			int len;

			if (cff_p->cff_nonl)
			    *ptr++ = '\n';

			if (len = cfl_p->cfl_len1) {
			    COPY_bytes(cfl_p->cfl_name1, ptr, len);
			    ptr += len;
			}
			if (cfl_p->cfl_name2) {
			    *ptr++ = '*';
			    if (len = cfl_p->cfl_len2) {
				COPY_bytes(cfl_p->cfl_name2, ptr, len);
				ptr += len;
			    }
			}
			*ptr++ = ':';
			*ptr++ = ' ';
			if (len = strlen(cfl_p->cfl_data)) {
			    COPY_bytes(cfl_p->cfl_data, ptr, len);
			    ptr += len;
			}
			*ptr++ = '\n';
			len = ptr - buffer;

			if (write(fd, buffer, len) == len) {
			    cff_p->cff_nonl = FALSE;
			    cfl_p->cfl_modified = FALSE;
			}
		    }
		} while (cfl_p = cfl_p->cfl_link);

		cff_p->cff_modified = FALSE;
		close(fd);
		return(TRUE);
	    }
	} else
	    return(TRUE);
    }
    return(FALSE);
}

/*
 * dns_cfgclose
 *	Close the configuration file.
 *
 * Inputs:
 *	handle = pointer to configuration handle from dns_cfgopen().
 */
void 
dns_cfgclose (char *handle)
{
    register cfgpgm_t *cfp_p;

    CFG_INIT;

    if (cfp_p = (cfgpgm_t *)handle) {
	cfgfile_t *cff_p = cfp_p->cfp_cff;

	cfg_pgmclose(cfp_p);
	if (cff_p) cfg_fileclose(cff_p);
    }
}

/*
 * dns_cfgtostr
 *	Get parameter string from configuration file.
 *
 * Inputs:
 *	handle = pointer to configuration handle from dns_cfgopen().
 *	name_p = pointer to configuration parameter name.
 *	def_p = pointer to default string or zero.
 *
 * Result:
 *	Pointer to string, or def_p if not found. This string becomes invalid
 *	when the configuration file is closed.
 */
char 
*dns_cfgtostr (char  *handle,
               char  *name_p,
               char  *def_p)
{
#define CFP_P ((cfgpgm_t *)handle)

    cfgname_t cfn;
    register cfgline_t *cfl_p;

    if (cfg_name_to_cfn(CFP_P, &cfn, name_p)) {
	cfg_to_cfl(CFP_P->cfp_cff, &cfn);
	if (cfl_p = cfg_to_cfl(CFP_P->cfp_cpf, &cfn))
	    return(cfl_p->cfl_data);
    }

    return(def_p);
#undef CFP_P
}

/*
 * dns_cfgfromstr
 *	Set parameter string for configuration file.
 *
 * Inputs:
 *	handle = pointer to configuration handle from dns_cfgopen().
 *	name_p = pointer to configuration parameter name.
 *	val_p = pointer to new string value.
 *
 * Result:
 *	TRUE if value recorded for later dns_cfgwrite(), otherwise FALSE.
 */
int 
dns_cfgfromstr (char  *handle,
                char  *name_p,
                char  *val_p)
{
#define CFP_P ((cfgpgm_t *)handle)
    return cfg_defn(CFP_P, name_p, val_p);
#undef CFP_P
}

/*
 * dns_cfgtol
 *	Get long int parameter from configuration file.
 *
 * Inputs:
 *	handle = pointer to configuration handle from dns_cfgopen().
 *	name_p = pointer to configuration parameter name.
 *	def = default value.
 *
 * Result:
 *	Value for file, or def if not found.
 */
long 
dns_cfgtol (char  *handle,
            char  *name_p,
            long  def)
{
#define CFP_P ((cfgpgm_t *)handle)
    cfgname_t cfn;
    register cfgline_t *cfl_p;

    if (cfg_name_to_cfn(CFP_P, &cfn, name_p)) {
	cfg_to_cfl(CFP_P->cfp_cff, &cfn);
	if (cfl_p = cfg_to_cfl(CFP_P->cfp_cpf, &cfn))
	    return(atoi(cfl_p->cfl_data));
    }

    return(def);
#undef CFP_P
}

/*
 * dns_cfgfroml
 *	Set long int parameter for configuration file.
 *
 * Inputs:
 *	handle = pointer to configuration handle from dns_cfgopen().
 *	name_p = pointer to configuration parameter name.
 *	val = New value.
 *
 * Result:
 *	TRUE if value recorded for later dns_cfgwrite(), otherwise FALSE.
 */
int 
dns_cfgfroml (char  *handle,
              char  *name_p,
              long  val)
{
#define CFP_P ((cfgpgm_t *)handle)
    char valbuf[40];

    sprintf(valbuf, "%d", val);
    return cfg_defn(CFP_P, name_p, valbuf);
#undef CFP_P
}

/*
 * dns_cfgtoarg
 *	Get argc/argv parameter from configuration file.
 *
 * Inputs:
 *	handle = pointer to configuration handle from dns_cfgopen().
 *	name_p = pointer to configuration parameter name.
 *	argv = vector of character pointers
 *	argc = size of argv vector (to hold argc-1 entries)
 *
 * Result:
 *	Number of arguments, or -1 if not found.
 */
int 
dns_cfgtoarg (char  *handle,
              char  *name_p,
              char  **argv,
              int   argc)
{
    cfgname_t cfn;
    register cfgpgm_t *cfp_p = (cfgpgm_t *)handle;
    register cfgline_t *cfl_p;

    if (!cfg_name_to_cfn(cfp_p, &cfn, name_p))
	return(-1);

    cfg_to_cfl(cfp_p->cfp_cff, &cfn);
    if (cfl_p = cfg_to_cfl(cfp_p->cfp_cpf, &cfn)) {
	register char *chr_p = cfl_p->cfl_data;
	register int narg;

	/* see if data has been parsed already */
	if (narg = cfl_p->cfl_argc) {
	    /* arguments must fit vector */
	    if (argc <= narg)
		return(-1);

	    *argv++ = cfp_p->cfp_pgm;
	    /* find parsed arguments in data */
	    do {
		chr_p += strspn(chr_p, breaks);
		*argv++ = chr_p;
		chr_p = strchr(chr_p, '\0') + 1;
	    } while (--narg);

	    /* make sure vector is terminated */
	    *argv = NULL;
	} else {
	    if (--argc < 0) return(-1);
	    *argv++ = cfp_p->cfp_pgm;
	    narg++;

	    /* parse arguments out of data */
	    while (0 < argc) {
		*argv = strtok(chr_p, breaks);
		if (!*argv++) break;
		narg++;
		chr_p = NULL;	/* first time only */
		--argc;
	    }
	    cfl_p->cfl_argc = narg;
	}
	return(cfl_p->cfl_argc);
    }
    return(-1);
}

/*
 * dns_cfgfromarg
 *	Set parameter string for configuration file.
 *
 * Inputs:
 *	handle = pointer to configuration handle from dns_cfgopen().
 *	line_p = pointer to configuration parameter name/value.
 *
 * Result:
 *	TRUE if value recorded for later dns_cfgwrite(), otherwise FALSE.
 */
int 
dns_cfgfromarg (char  *handle,
                char  *line_p)
{
#define CFP_P ((cfgpgm_t *)handle)

    if (CFP_P && CFP_P->cfp_cpf)
	if(*cfg_parse_line(CFP_P->cfp_cpf, line_p, TRUE) == '\0')
	    return(TRUE);

    return(FALSE);
#undef CFP_P
}

/*
 * cfg_fileopen
 *	Allocate and open a configuration file.
 */
static cfgfile_t 
*cfg_fileopen (char *file_p)
{
    register cfgfile_t *cff_p;

    /* get the configuration file name */
    if (!file_p) {
	if (cff_p = default_cff)
	    return(cff_p);

	file_p = default_file;
    }

    CFG_LOCK {
	/* CANCEL POINT - no local resources affected by cancel */

	/* if someone is loading a file, wait for completion */
	while (cff_busy) CFG_WAIT;

	/* look for requested file */
	if (cff_p = cff_list)
	    do
		if (strcmp(file_p, cff_p->cff_file) == 0) {
		    cff_p->cff_users++;
		    break;
		}
	    while (cff_p = cff_p->cff_link);

	/* if the file wasn't found, we have to create it */
	if (!cff_p) cff_busy = 1;
    } CFG_END_LOCK;

    /* return file if found */
    if (cff_p) return(cff_p);

    /*
     * Allocate the file block and inititalize.
     */
    if (cff_p = (cfgfile_t *)dns_malloc(sizeof(*cff_p))) {
	ZERO_bytes(cff_p, sizeof(*cff_p));
	strcpy(cff_p->cff_file, file_p);
	cff_p->cff_mode = 0600;
	cff_p->cff_line_e = (cfgline_t *)&cff_p->cff_line;
	cff_p->cff_users = 1;

	/* CANCEL POINT - cff_p and cff_busy must be released */
	CFG_PUSH(cfg_fileopen_cleanup, cff_p);

	cfg_cff_from_file(cff_p);

	CFG_POP(0);
    }

    /* add file to the list and signal done */
    CFG_LOCK {
	if (cff_p) {
	    cff_p->cff_link = cff_list;
	    cff_list = cff_p;
	    if (file_p == default_file)
		default_cff = cff_p;
	}
	cff_busy = 0;
	CFG_BROADCAST;
    } CFG_END_LOCK;

    return(cff_p);
}

static void 
cfg_fileopen_cleanup (void *cff_p)
{
    dns_free(cff_p);

    /* signal done */
    CFG_LOCK {
	cff_busy = 0;
	CFG_BROADCAST;
    } CFG_END_LOCK;
}

static void 
cfg_fileclose (cfgfile_t *cff_p)
{
    /* someday add a janitor thread to clean these up */
    CFG_LOCK {
	--cff_p->cff_users;
    } CFG_END_LOCK;
}

/*
 * cfg_cff_from_file
 *	Read configuration data from a file
 */
static void 
cfg_cff_from_file (cfgfile_t *cff_p)
{
    register int fd;

    /*
     * Open the file and find out how big it is.
     * We will read the entire file into a buffer appended
     * to the cfgfile data block.
     *
     * CANCEL POINT - no local resources affected by cancel
     */
    if (0 <= (fd = open(cff_p->cff_file, O_RDONLY, 0))) {
	struct stat buf;

	/* CANCEL POINT - no local resources affected by cancel */
	if (0 <= fstat(fd, &buf)) {
	    register char *this_p;
            register int   read_count;
            register int   read_total = 0;

	    cff_p->cff_mtime = buf.st_mtime;
	    cff_p->cff_mode = buf.st_mode;

	    if ((0 < buf.st_size) && (this_p = dns_malloc(buf.st_size + 2))) {
		/* CANCEL POINT - this_p must be released */
		CFG_PUSH(cfg_cff_from_file_cleanup, this_p);
                do {
                    read_count = read(fd, &this_p[read_total], 
                                      (buf.st_size - read_total));
                    read_total += read_count;
                } while (read_count > 0 && read_total < buf.st_size);
		if (read_total > 0) {
		    cff_p->cff_data = this_p;
		    cff_p->cff_end = &this_p[read_total];
		    if (cff_p->cff_end[-1] != '\n') {
			cff_p->cff_nonl = TRUE;
			*cff_p->cff_end++ = '\n'; /* terminate last line */
		    }
		    *cff_p->cff_end = '\0';	/* make sure a trailing NULL */

		    do {
			this_p = cfg_parse_line(cff_p, this_p, FALSE);
			while (*this_p++ != '\n') ;
		    } while (*this_p);
		}
		CFG_POP(0);
	    }
	}
	/* CANCEL POINT - no local resources affected by cancel */
	close(fd);
    }
}

static void 
cfg_cff_from_file_cleanup (void *this_p)
{
    dns_free(this_p);
}

/*
 * cfg_pgmopen
 *	Allocate a program database
 */
static cfgpgm_t 
*cfg_pgmopen (cfgfile_t  *cff_p,
              char       *pgm_p)
{
    register cfgpgm_t *cfp_p;

    /* get the program name */
    if (!pgm_p) {
	if ((cfp_p = default_cfg) && (cff_p == default_cff))
	    return(cfp_p);

	pgm_p = default_pgm;
    } else
	pgm_p = cfg_basename(pgm_p);

    CFG_LOCK {
	/* look for requested program */
	if (cfp_p = cff_p->cff_cfg)
	    do
		if (strcmp(pgm_p, cfp_p->cfp_pgm) == 0) {
		    cfp_p->cfp_users++;
		    break;
		}
	    while (cfp_p = cfp_p->cfp_link);

	/* if the program wasn't found, we have to create it */
	if (!cfp_p)
	    if (cfp_p = (cfgpgm_t *)dns_malloc(sizeof(*cfp_p))) {
		ZERO_bytes(cfp_p, sizeof(*cfp_p));
		strcpy(cfp_p->cfp_pgm, pgm_p);
		cfp_p->cfp_users = 1;
		cfp_p->cfp_cff = cff_p;
		cfp_p->cfp_link = cff_p->cff_cfg;
		cff_p->cff_cfg = cfp_p;

		if ((cff_p == default_cff) && (pgm_p == default_pgm))
		    default_cfg = cfp_p;
	    }
    } CFG_END_LOCK;

    /* now try to open the program-specific file */
    if (!cfp_p->cfp_cpf) {
	char pgmfile[1024];
	char *file_p;

	strcpy(pgmfile, cff_p->cff_file);
	file_p = cfg_basename(pgmfile);
	*file_p++ = '.';
	strcpy(file_p, pgm_p);
	strcat(pgmfile, "rc");
	/* CANCEL POINT - no local resources affected by cancel */
	if (cff_p = cfg_fileopen(pgmfile))
	    cfp_p->cfp_cpf = cff_p;
    }

    return(cfp_p);
}

static void 
cfg_pgmclose (cfgpgm_t *cfp_p)
{
    /* someday add a janitor thread to clean these up */
    CFG_LOCK {
	--cfp_p->cfp_users;
    } CFG_END_LOCK;
}

/*
 * Parse a configuration file line into a cfgline vector
 * Ignore comments & blank lines.
 * Returns a pointer to terminating character.
 *
 * A departure from X11 resources:
 *	ignore/remove spaces within the name.
 *	reject lines without ":" to divide name from data.
 *	reject names with more than one wildcard.
 *	octal escapes in data can be less than 3 digits.
 */
static char 
*cfg_parse_line (cfgfile_t  *cff_p,
                 char       *ch_p,
                 int        mod)
{
    struct {
	cfgline_t	b_cfl;
	char		b_data[1000];
    } b;
    cfgline_t	*cfl_p;
    register char ch = *ch_p;
    register char *data_p = b.b_data;
    register int len = 0;

    ZERO_bytes(&b.b_cfl, sizeof(b.b_cfl));
    b.b_cfl.cfl_source = ch_p;

    /* comments start with "!" as first character */
    if (ch == '!') return(ch_p);

    /* skip whitespace */
    for (;isspace(ch); ch = *++ch_p)
	if (ch == '\n')
	    return(ch_p);

    /* toss empty lines or non-standard comment */
    if (ch == '#') return(ch_p);

    /* scan to end of name */
    b.b_cfl.cfl_name1 = data_p;
    for (;; ch = *++ch_p) {
	switch (ch) {
	default:
	    /* collapse all spaces */
	    if (isspace(ch)) continue;

	    /* seperate from previous name */
	    if (len < 0) {
		len = 0;
		*data_p++ = '.';
	    }

	    /* and transfer this name */
	    *data_p++ = ch;
	    len++;
	    continue;
	case '*' :
	    /* record our one (and only) wildcard */
	    if (!b.b_cfl.cfl_name2) {
		if (b.b_cfl.cfl_name1 < data_p)
		    /* include seperator in length */
		    b.b_cfl.cfl_len1 = &data_p[1] - b.b_cfl.cfl_name1;

		b.b_cfl.cfl_name2 = data_p;
	    } else
		/* unacceptable name */
		return(ch_p);
	case '.' :
	    /* collapse multiple seperators */
	    if (len <= 0) continue;

	    len = -1;	/* insert seperator before next name */
	    b.b_cfl.cfl_names++;
	    continue;
	case ':' :
	    if (0 < len)
		b.b_cfl.cfl_names++;

	    if (!b.b_cfl.cfl_name2)
		b.b_cfl.cfl_len1 = data_p - b.b_cfl.cfl_name1;
	    else if (len = (&data_p[-1] - b.b_cfl.cfl_name2))
		b.b_cfl.cfl_len2 = 1+len;
	    else
		*data_p++ = '.';

	    *data_p++ = '\0';
	    break;
	case '\0':
	case '\n':
	    return(ch_p);
	}
	break;
    }
    ch = *++ch_p;

    /* skip/remove old-style prefix if present */
    if ((LEN_CONFPFX <= b.b_cfl.cfl_len1)
	    && EQ_bytes(confpfx, b.b_cfl.cfl_name1, LEN_CONFPFX)) {
	b.b_cfl.cfl_name1 += LEN_CONFPFX;
	b.b_cfl.cfl_len1 -= LEN_CONFPFX;
    }

    /* skip whitespace */
    for (; (ch != '\n') && isspace(ch); ch = *++ch_p) ;

    /* scan to end of value */
    b.b_cfl.cfl_data = data_p;
    for (; ch && (ch != '\n'); ch = *++ch_p) {
	if (ch == '\\') {
	    ch = *++ch_p;
	    if (ch == 'n') {
		/* insert newline in data */
		ch = '\n';
	    } else if (ch == '\n') {
		/* toss quoted newline */
		continue;
	    } else if (('0' <= ch) && (ch < '8')) {
		/* construct character from octal */
		len = (ch - '0');
		while (('0' <= ch_p[1]) && (ch_p[1] < '8')) {
		    if ((1<<5) <= len) break;
		    len <<= 3;
		    ch = *++ch_p;
		    len += (ch - '0');
		}
		ch = len;
	    } else if (!ch) {
		/* what? a "\" with NULL terminator: ignore */
		--ch_p;
		continue;
	    }
	}
	/* copy data */
	*data_p++ = ch;
    }
    *data_p++ = '\0';

    if (cfl_p = (cfgline_t *)dns_malloc(len = data_p - (char *)&b)) {
	COPY_bytes((char *)&b, (char *)cfl_p, len);
	if (cfl_p->cfl_modified = mod)
	    cff_p->cff_modified = TRUE;

	/* readjust pointers */
	cfl_p->cfl_name1 = b.b_cfl.cfl_name1 - (char *)&b + (char *)cfl_p;
	if (cfl_p->cfl_name2)
	    cfl_p->cfl_name2 = b.b_cfl.cfl_name2 - (char *)&b + (char *)cfl_p;
	cfl_p->cfl_data = b.b_cfl.cfl_data - (char *)&b + (char *)cfl_p;

	/* link into file's list */
	cff_p->cff_line_e->cfl_link = cfl_p;
	cff_p->cff_line_e = cfl_p;
	return(ch_p);
    }

    /* allocation failed - reject entire definition */
    return(b.b_cfl.cfl_source);
}

static int 
cfg_defn (cfgpgm_t  *cfp_p,
          char      *name_p,
          char      *val_p)
{
    cfgname_t cfn;

    if (cfg_name_to_cfn(cfp_p, &cfn, name_p)) {
	cfgfile_t *cff_p = cfp_p->cfp_cpf;
	register cfgline_t *cfl_p;
	register int len_val = strlen(val_p), len_nam;

	if ((cfl_p = cfg_to_cfl(cff_p, &cfn))
		&& !cfl_p->cfl_name2) {
	    register char *data_p;
    
	    /* exact match: replace data value */
	    if (data_p = (char *)dns_malloc(len_val)) {

		if (cfl_p->cfl_alloc)
		    dns_free(cfl_p->cfl_data);
		else
		    cfl_p->cfl_alloc = TRUE;

		strcpy(data_p, val_p);
		cfl_p->cfl_data = data_p;
		cfl_p->cfl_modified = TRUE;
		return(TRUE);
	    } else
		return(FALSE);
	}
    
	/* create cfgline_t and attach to end of list */
	len_nam = cfn.cfn_end_p - cfn.cfn_name;
	if (cfl_p = (cfgline_t *)dns_malloc(sizeof(*cfl_p)+len_nam+len_val+2)) {
	    register char *text_p = (char *)&cfl_p[1];
    
	    ZERO_bytes(cfl_p, sizeof(*cfl_p));
	    cfl_p->cfl_name1 = text_p;
	    COPY_bytes(cfn.cfn_name, text_p, len_nam);
	    text_p += len_nam;
	    *text_p++ = '\0';
	    cfl_p->cfl_len1 = len_nam;
	    cfl_p->cfl_names = cfn.cfn_names;
	    strcpy(text_p, val_p);
	    cfl_p->cfl_data = text_p;
	    cfl_p->cfl_modified = TRUE;

	    /* link into file's list */
	    cff_p->cff_line_e->cfl_link = cfl_p;
	    cff_p->cff_line_e = cfl_p;
	    cff_p->cff_modified = TRUE;
	    return(TRUE);
	}
    }
    return(FALSE);
}

/*
 * cfg_to_cfl
 *	Find line entry for program parameter
 */
static cfgline_t 
*cfg_to_cfl (cfgfile_t  *cff_p,
             cfgname_t  *cfn_p)
{
    if (cff_p) {
	register char *name_p = cfn_p->cfn_name;
	register char *end_p = cfn_p->cfn_end_p;
	register cfgline_t *cfl_p = cff_p->cff_line;

	if (!cfl_p) return(cfn_p->cfn_best_p);

	do {
	    register char *nm_p = cfl_p->cfl_name1;
	    register int len1, len2;
	    
	    if ((len1 = cfl_p->cfl_len1) && NE_bytes(nm_p, name_p, len1))
		continue;

	    if (nm_p = cfl_p->cfl_name2) {
		if ((len2 = cfl_p->cfl_len2) && 
			   NE_bytes(nm_p, &end_p[-len2], len2))
		    continue;

		/* make sure the matches don't overlap (except ".") */
		if (&end_p[1-len2] < &name_p[len1])
		    continue;
	    } else if (end_p != &name_p[len1])
		continue;

	    /* better if less wildcard matches, or none at all! */
	    if ((cfn_p->cfn_names - cfl_p->cfl_names) <= cfn_p->cfn_best) {
		cfn_p->cfn_best = cfn_p->cfn_names - cfl_p->cfl_names;
		cfn_p->cfn_best_p = cfl_p;
	    }
	} while (cfl_p = cfl_p->cfl_link);
    }
    return(cfn_p->cfn_best_p);
}

/*
 * cfg_name_to_cfn
 *	Create a fully qualified name in name_p and return pointer to it.
 */
static cfgname_t 
*cfg_name_to_cfn (cfgpgm_t   *cfp_p,
                  cfgname_t  *cfn_p,
                  char       *name_p)
{
    if (cfp_p) {
	ZERO_bytes(cfn_p, sizeof(*cfn_p));
	cfn_p->cfn_end_p = cfn_p->cfn_name;
	cfg_append(cfn_p, cfp_p->cfp_pgm);
	cfg_append(cfn_p, ".");
	cfg_append(cfn_p, name_p);
	cfg_append(cfn_p, ":");
	cfn_p->cfn_best = sizeof(cfn_p->cfn_name);
	return(cfn_p);
    }
    return(NULL);
}

/*
 * cfg_append
 *	Append a string to name.
 */
static void 
cfg_append (cfgname_t  *cfn_p,
            char       *from_p)
{
    register char *to_p = cfn_p->cfn_end_p;

    for (; *to_p = *from_p++; to_p++) {
	switch (*to_p) {
	case '*':
	    cfn_p->cfn_wild = TRUE;
	case '.':
	    cfn_p->cfn_names++;
	default:
	    continue;
	case ':':
	    cfn_p->cfn_names++;
	    break;
	}
	break;
    }

    cfn_p->cfn_end_p = to_p;
}

/*
 * return the basename of a given filename
 *
 */
static char 
*cfg_basename (char *file_p)
{
    register char *this_p;

    if (file_p)
	if (this_p = strrchr(file_p, '/'))
	    file_p = &this_p[1];

    return(file_p);
}
