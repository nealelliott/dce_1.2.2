/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Implementation header for serviceability routines.
 */

/*
 * HISTORY
 * $Log: svcimpl.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:13  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:04  root]
 * 
 * Revision 1.1.4.17  1994/10/03  16:54:03  rsalz
 * 	Add GOESTO route type (OT CR 10474).
 * 	[1994/10/03  16:33:11  rsalz]
 * 
 * Revision 1.1.4.16  1994/09/30  20:59:45  bowe
 * 	Increase SVCSPRINTFBUF to 4096. [CR 12445]
 * 	[1994/09/30  20:59:22  bowe]
 * 
 * Revision 1.1.4.15  1994/09/29  20:24:03  bowe
 * 	Prototype for dce_svc__linefor(). [OT 11542]
 * 	[1994/09/29  20:23:26  bowe]
 * 
 * Revision 1.1.4.14  1994/09/06  21:27:34  bowe
 * 	Remove arg_index from handle. [CR 12049]
 * 	[1994/09/06  21:08:49  bowe]
 * 
 * Revision 1.1.4.13  1994/08/28  00:51:05  rsalz
 * 	Add "SVC_BRIEF" env var support (OT CR 11512).
 * 	[1994/08/27  12:37:26  rsalz]
 * 
 * Revision 1.1.4.12  1994/08/09  17:49:33  bowe
 * 	Use ';' for multiple route delimeter. [CR 11606]
 * 	[1994/08/09  17:48:45  bowe]
 * 
 * Revision 1.1.4.11  1994/07/27  12:28:02  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:35:01  rsalz]
 * 
 * Revision 1.1.4.10  1994/06/10  19:42:04  tom
 * 	Change argument to dce_svc__fillin (OT 10922)
 * 	[1994/06/10  19:41:27  tom]
 * 
 * Revision 1.1.4.9  1994/06/09  16:06:28  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:57  devsrc]
 * 
 * Revision 1.1.4.8  1994/06/06  13:14:16  bowe
 * 	Added SVC_ROUTESEP_CHAR. [CR 10476]
 * 	[1994/06/06  13:10:56  bowe]
 * 
 * Revision 1.1.4.7  1994/05/26  18:51:46  bowe
 * 	Added macros for SVC_BLOCK_SIZE and void-to-unsigned conversion.
 * 	simplified dce_svc_log_handle_s_t structure.  [CR 10483,10478]
 * 	[1994/05/26  18:21:45  bowe]
 * 
 * Revision 1.1.4.6  1994/04/26  19:21:30  rsalz
 * 	Move some field from prolog into log handle (OT CR 10461)
 * 	[1994/04/26  17:02:26  rsalz]
 * 
 * Revision 1.1.4.5  1994/04/19  20:02:10  rsalz
 * 	Add a sample SVC routing file (OT CR 10397).
 * 	[1994/04/19  16:40:42  rsalz]
 * 
 * Revision 1.1.4.4  1994/03/29  18:11:21  rsalz
 * 	Check pthread_xxx return values where possible (OT CR 8883)
 * 	[1994/03/29  15:20:26  rsalz]
 * 
 * Revision 1.1.4.3  1994/03/09  20:43:21  rsalz
 * 	Add dce_pgm_{printf,fprintf,sprintf} (OT CR 9926).
 * 	[1994/03/09  20:14:44  rsalz]
 * 
 * Revision 1.1.4.2  1993/12/20  20:50:50  rsalz
 * 	Merge msg.sams into svc.sams
 * 	[1993/12/20  20:50:23  rsalz]
 * 
 * 	Merge msg.sams into svc.sams
 * 
 * Revision 1.1.4.1  1993/12/20  17:00:18  rsalz
 * 	Include string.h
 * 	[1993/12/20  16:54:40  rsalz]
 * 
 * Revision 1.1.2.5  1993/12/01  21:18:27  rsalz
 * 	Fix pthread_once function to NOT set lock
 * 	[1993/12/01  21:18:03  rsalz]
 * 
 * Revision 1.1.2.4  1993/11/23  17:23:25  rsalz
 * 	Change SMBUF (small buffer; also used for pathnames) to 512
 * 	[1993/11/23  17:17:43  rsalz]
 * 
 * Revision 1.1.2.3  1993/09/02  17:42:58  rsalz
 * 	Change dce_svc_routings to take a flag indicating if it
 * 	should return standard routings or debug settings.
 * 	Turn dce_svc_g_memory into an array of structures that keep
 * 	the text and an "is a debug message" indicator.
 * 	[1993/09/02  17:42:33  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:09:57  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:39  rsalz]
 * 
 * $EndLog$
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <memory.h>
#include <string.h>
#include <dce/dce.h>
#include <dce/dce_utils.h>
#include <dce/utc.h>
#include <dce/assert.h>
#include <dce/dce_msg.h>
#include <dce/svcfilter.h>
#include <dce/svclog.h>
#include <dce/dcesvcmsg.h>


#define SVC_SEP_CHAR		':'
#define SVC_COMMENT_CHAR	'#'
#define SVC_FLAGSEP_CHAR	','
#define SVC_LEVELSEP_CHAR	'.'
#define SVC_ROUTESEP_CHAR	';'

#define ROUTE_BINFILE		"BINFILE"
#define ROUTE_DISCARD		"DISCARD"
#define ROUTE_FILE		"FILE"
#define ROUTE_STDERR		"STDERR"
#define ROUTE_STDOUT		"STDOUT"
#define ROUTE_TEXTFILE		"TEXTFILE"
#define ROUTE_GOESTO		"GOESTO"

#define SVC_ROUTEFILE_ENVVAR	"DCE_SVC_ROUTING_FILE"
#define SVC_ROUTEFILE_FORMAT	"%s/var/svc/routing"
#define SVC_ROUTE_ENVVAR	"SVC_%s"
#define SVC_DBGROUTE_ENVVAR	"SVC_%s_DBG"
#define SVC_BRIEF_ENVVAR	"SVC_BRIEF"

#define SMBUF			512
#define SVCSPRINTFBUF		4096


#define LOCK()		\
	do { \
	    if (!dce_svc_g_setup) \
		pthread_once(&dce_svc_g_once_block, \
		    (pthread_initroutine_t)dce_svc__init_mutex); \
	    if (pthread_mutex_lock(&dce_svc_g_mutex) < 0) \
		continue; \
	} while (0)
#define UNLOCK()	\
	do { \
	} while (pthread_mutex_unlock(&dce_svc_g_mutex) < 0)


/*
**  Block size for reading/writing binary log entries.  Make sure these agree!
*/
#define SVC_BLOCK_SIZE		64
#define SVC_FILL_TO_BLOCK(s)	((s) & 0x3F ? 64 - ((s) & 0x3F) : 0)

/*
**  How pointers should be stored in binary logs.
*/
#define SVC_VOID_TO_UNSIGNED(v)	((unsigned32)(v))

/*
**  Number of args to allocate on the stack in dce_svc__binprint().
**  We use malloc only if we need more.
*/
#define dce_svc_c_num_values	32


/*
**  Turn a dce_svc_printf attributes parameter into an index into
**  the global routing array.
*/
#define ATTRIBUTEStoINDEX(i)	\
	    ((((i) & svc__c_mask) >> svc__c_shift) - 1)

/*
**  Routing.
*/
typedef enum _dce_route_type_t {
	route_default, route_binfile, route_textfile, route_discard,
	route_stdout, route_stderr, route_goesto
} dce_route_type_t;

struct dce_svc_routing_s_t {
    dce_svc_routing_t		*next;
    int				references;
    dce_route_type_t		route;
	/* How many files to keep, and how many entries/file. */
    int				maxgenerations;
    int				maxcount;
    int				generations;
    int				count;
    union _route_data {
	struct { char *name; int fd; }	binfile;
	struct { char *name; FILE *f; }	textfile;
	struct { char *where; } goesto;
    }				data;
};

typedef struct dce_svc_memory_s_t {
    char			*text;
    boolean32			is_debug;
} dce_svc_memory_t;


/*
**  Logs.
*/
struct dce_svc_log_handle_s_t {
    FILE			*input;
    struct dce_svc_log_prolog_s_t prolog;
    off_t			next_record;
};


/*
**  Private variables.
*/
extern boolean			dce_svc_g_setup;
extern char			dce_svc_g_progname[dce_svc_c_progname_buffsize];
extern const char		*dce_svc_g_code_table[5];
extern unsigned32		dce_svc_g_attr_table[5];
extern dce_svc_handle_t		*dce_svc_g_map;
extern dce_svc_memory_t		*dce_svc_g_memory;
extern dce_svc_routing_block_t	dce_svc_g_routes[5];
extern dce_svc_routing_t	*dce_svc_g_routebase;
extern int			dce_svc_g_map_size;
extern int			dce_svc_g_memory_size;
extern int			dce_svc_g_brief;
extern pthread_mutex_t		dce_svc_g_mutex;
extern pthread_once_t		dce_svc_g_once_block;


/*
**  Private routines.
*/

extern dce_svc_handle_t dce_svc__table_to_handle(
    dce_svc_subcomp_t*		/* table */
);

extern dce_svc_handle_t dce_svc__component_to_handle(
    unsigned char*		/* comp */
);

extern boolean dce_svc__debug_fillin(
    dce_svc_handle_t		/* handle */,
    error_status_t*		/* status */
);

extern void dce_svc__fillin(
    unsigned32			/* attributes */,
    error_status_t*		/* status */
);

extern void dce_svc__translate(
    dce_svc_handle_t		/* handle */,
    error_status_t*		/* status */
);

extern boolean dce_svc__add_to_table(
    dce_svc_handle_t		/* handle */,
    error_status_t*		/* status */
);

extern void dce_svc__rotate_route(
    dce_svc_routing_t*		/* r */
);

extern void dce_svc__close_routes(
    dce_svc_routing_block_t*	/* rb */
);

extern void dce_svc__shutdown(
    void
);

extern void *dce_svc__mapthreadid(
    pthread_t*			/* tid */
);

extern void dce_svc__init_mutex(
    void
);

extern char *dce_svc__linefor(
    const char *		/* name */
);

extern dce_svc_routing_t **dce_svc__goesto(
    char*			/* where */,
    int*			/* nroutesp */
);
