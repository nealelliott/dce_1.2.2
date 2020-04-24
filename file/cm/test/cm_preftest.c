/*
 *      Copyright (C) 1996, 1994 Transarc Corporation
 *      All rights reserved.
 */

/*
 * Test program for server preference module and associated parts
 */
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/tpq.h>
#include <../cm.h>
#include <../cm_server.h>
#include <../cm_cell.h>
#include <../cm_serverpref.h>
#include <../cm_site.h>
#include <../cm_rrequest.h>
#include <../cm_volume.h>
#include <../cm_stats.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/cm/test/RCS/cm_preftest.c,v 1.17 1996/11/19 20:16:09 gait Exp $")

#ifdef CM_ENABLE_COUNTERS
struct cm_stats cm_stats;
#endif /* CM_ENABLE_COUNTERS */

/*
See db8175 for explanation.
*/

#if     defined(s_type)
#undef s_type
#endif  /* s_type */

struct server_entry {
    char	*s_name;	/* hostname */
    u_long	s_address;	/* IP addr */
    u_long	s_type;		/* server type */
    u_short	s_rank;		/* server rank */
    struct server_entry *s_next;/* next in line */
};

struct pardoArg {
    struct server_entry	*p_sep;
    int			p_action;
};

#define ACTION_DISCOVER		1
#define ACTION_SETPREF		2
#define ACTION_CHECKPREF	4


/*
 * Global data
 */
void 			*threadPoolHandle;
int 			debug_level = 0;
struct server_entry	*servers = NULL;
int			server_count = 0;
struct icl_set 		*cm_iclSetp = 0;
osi_cred_t 		*osi_credp = NULL;
struct cm_cell          globalcell;
/*
 * Some cm routines need to be provided to make this test work:
 */
/* cm_subr.c */
int cm_printf(a, b, c, d, e, f, g)
  char *a, *b, *c, *d, *e, *f, *g;
{
    if (debug_level > 0) printf(a, b, c, d, e, f, g);
    return 0;
}

/* cm_volume.c */
int cm_CheckVolumeInfo (afsFid *fidp,
			struct cm_rrequest *rreqp,
			struct cm_volume *involp,
			struct cm_cell *cellp,
			int *gotNewLocP)
{
    /* do nothing */
    return 0;
}

/* cm_conn.c */
void cm_MarkBadConns(struct cm_server *serverp,
		     long pag)
{
    /* do nothing */
    return;
}

void cm_PutConn(struct cm_conn *connp)
{
    /* do nothing */
    return;
}

struct cm_conn *cm_ConnByMHosts(struct cm_server *serverp[],
				afs_hyper_t *cellIdp,
				unsigned long service,
				struct cm_rrequest *rreqp,
				struct cm_volume *volp,
				long *srvixp)
{
    /* do nothing */
    return NULL;
}

/* flprocs.c */
error_status_t VL_GetCellInfo(rpc_binding_handle_t connp,
			      vlconf_cell *MyCell)
{
    /* do nothing */
    return NULL;
}

/*
 * Load prefs in internal table into servers structures exercising
 * cm functions
 */
int set_prefs()
{
    struct sockaddr_in	tsock;
    struct cm_server	*serverp;
    struct server_entry	*sep;
    unsigned long 	servers_sr=0, servers_ss=0, servers_g=0, errors=0;
    unsigned 		create, i;
    extern long         cm_random();

    bzero((char *)&tsock, sizeof(struct sockaddr_in));
    tsock.sin_family = AF_INET;
    tsock.sin_port = IPPORT_USERRESERVED + 1;

    for (sep=servers; sep; sep=sep->s_next) {
	tsock.sin_addr.s_addr = sep->s_address;
	create = cm_random() % 2;
	if (create) {
	    serverp = cm_GetServer(&globalcell, sep->s_name, sep->s_type,
				   NULL, &tsock, 1);
	    servers_ss++;
	} else {
	    cm_SiteAddrSetRankAll(&tsock, sep->s_type, sep->s_rank);
	    servers_sr++;
	}
	if (debug_level > 1) {
	    printf("\tAdding %s %s %d %d ",
		   (create)?"server":"request",
		   sep->s_name, sep->s_type, sep->s_rank);
	    osi_printIPaddr("[", sep->s_address, "]\n");
	}
    }
    printf("Finished with %d servers %d requests\n", servers_ss, servers_sr);

    /*
     * Now go back through and find all servers, and verify the rank
     * value and type.  Also stuff data into the server table.
     */
    for (sep=servers; sep; sep=sep->s_next) {
	tsock.sin_addr.s_addr = sep->s_address;
	serverp = cm_GetServer(&globalcell, sep->s_name, sep->s_type,
			       NULL, &tsock, 1);
	if (serverp == NULL) {
	    fprintf(stderr, "Failed to find server %s!\n", sep->s_name);
	    errors++;
	    continue;
	}

	/*
	 * Check the type (FL, FX or REP)
	 * Check the rank for righteousity
	 */
	if (serverp->sType != sep->s_type) {
	    fprintf(stderr, "Server Type mismatch %s %u %u!\n",
		    sep->s_name, sep->s_type, serverp->sType);
	    errors++;
	    continue;
	}
	if (serverp->sitep->addrCurp->rank != sep->s_rank) {
	    fprintf(stderr, "Server Rank mismatch %s %u %u!\n",
		    sep->s_name, sep->s_rank, serverp->sitep->addrCurp->rank);
	    errors++;
	    continue;
	}
	servers_g++;
    }
    printf("Verified: %d servers set, %d servers found, %d errors\n",
	   servers_ss+servers_sr, servers_g, errors);
    return(errors);
}

/*
 * Read database file into internal table
 */
int load_server_table(char * fileName)
{
    FILE		*dbfile;
    char		name[osi_BUFFERSIZE];
    unsigned long	rank, stype, errors = 0;
    struct hostent	*thost;
    struct server_entry	*sep = NULL, *prev = NULL;

    /* open test cell database file */
    if (!(dbfile = fopen(fileName, "r"))) {
	fprintf(stderr, "Error: could not open file %s\n",fileName);
	return(1);
    }

    while (fscanf(dbfile, "%79s%ld%ld", name, &rank, &stype) != EOF) {
	thost = gethostbyname(name);
	if (!thost) {
	    fprintf(stderr, "could not resolve host name %s.\n", name);
	    errors++;
	    continue;
	}
	/*
	 * Allocate space and link it in to servers list
	 */
	sep = (struct server_entry *)osi_Alloc(sizeof(struct server_entry));
	if (servers == NULL)
	    servers = sep;
	else
	    prev->s_next = sep;
	sep->s_name = (char *)osi_Alloc(strlen(thost->h_name) + 1);
	strcpy(sep->s_name, thost->h_name);
	sep->s_address = *(u_long *)thost->h_addr;
	sep->s_type = stype;
	sep->s_rank = rank;
	sep->s_next = NULL;
	prev = sep;
	server_count++;
    }
    fclose(dbfile);
    return(errors);
}


/*
 * Verify the correctness of one server entry
 */
int verify_pref(struct server_entry *sep)
{
    struct cm_server	*srvp;
    struct sockaddr_in	tsock;

    tsock.sin_addr.s_addr = sep->s_address;
    srvp = cm_GetServer(&globalcell, sep->s_name, sep->s_type,
			NULL, &tsock, 1);
    if (srvp == NULL) {
	fprintf(stderr, "Failed to find server %s!\n", sep->s_name);
	return(1);
    }

    /*
     * Check the type of the server (FLDB, FX or REP).
     * Check the rank for righteousity
     */
    if (srvp->sType != sep->s_type) {
	fprintf(stderr, "Server Type mismatch %u %u!\n",
		sep->s_type, srvp->sType);
	return(1);
    }

    if (srvp->sitep->addrCurp->rank != sep->s_rank) {
	fprintf(stderr, "Server Rank mismatch %u %u!\n",
		sep->s_rank, srvp->sitep->addrCurp->rank);
	return(1);
    }

    return(0);
}


/*
 * Each thread runs this routine with different args.
 */
void parallel_set_prefs(void *argP,  void *ignoredTpqOpHandle)
{
    struct cm_server	*serverp;
    struct sockaddr_in	tsock;
    int 		action = ((struct pardoArg *)argP)->p_action;
    struct server_entry *sep = ((struct pardoArg *)argP)->p_sep;

    bzero((char *)&tsock, sizeof(struct sockaddr_in));
    tsock.sin_family = AF_INET;
    tsock.sin_port = IPPORT_USERRESERVED + 1;
    tsock.sin_addr.s_addr = sep->s_address;

    if (action == ACTION_DISCOVER) {
	/*
	 * do a "get server" op just to be sure the cm_server struct
	 * is/gets created.
	 */
	serverp = cm_GetServer(&globalcell, sep->s_name, sep->s_type,
			       NULL, &tsock, 1);
	if (serverp == NULL) {
	    fprintf(stderr,
		    "parallel_set_prefs: Failed to discover server %s [%d]\n",
		    sep->s_name, sep->s_type);
	}
    } else if (action == ACTION_SETPREF) {
	/*
	 * do a "set preference" op to either alter the existent cm_server
	 * or to queue a request.
	 */
	cm_SiteAddrSetRankAll(&tsock, sep->s_type, sep->s_rank);
    } else if (action == ACTION_CHECKPREF) {
	verify_pref(sep);
    } else {
	fprintf(stderr, "parallel_set_prefs: illegal action %d\n", action);
    }
    return;
}

/*
 * Parallel test preferences.  This routine makes two threads; one that
 * "discovers" a server and one that sets the server's preference.
 * For an empty server hash, this stresses the server structure adding code
 * and the "delayed" preference setting.  For a full server hash table,
 * this stresses the interlocking with getting the struct and
 * finding/adjusting the rank.
 */
ParallelPrefs()
{
    struct server_entry	*sep;
    tpq_pardo_clientop_t clientOpsP[2];
    struct pardoArg 	argsP[2];
    unsigned long	errors=0;

    bzero( (caddr_t) clientOpsP, 2 * sizeof(tpq_pardo_clientop_t));

    clientOpsP[0].op = clientOpsP[1].op = parallel_set_prefs;
    clientOpsP[0].argP = &(argsP[0]);
    clientOpsP[1].argP = &(argsP[1]);

    argsP[0].p_action = ACTION_DISCOVER;
    argsP[1].p_action = ACTION_SETPREF;

    for (sep=servers; sep; sep=sep->s_next) {
	argsP[0].p_sep = argsP[1].p_sep = sep;
	if (debug_level > 1)
	    printf("Adjusting server %s\n", sep->s_name);

	tpq_Pardo(threadPoolHandle, clientOpsP, 2, TPQ_HIGHPRI, 0);
	errors += verify_pref(sep);
    }
    printf("Finished whole database with %d errors\n", errors);
    return(errors);
}

/*
 * Free resources
 */
void delete_database()
{
    struct server_entry *del, *sep = servers;

    while(sep) {
	osi_Free(sep->s_name, strlen(sep->s_name)+1);
	del = sep;
	sep = sep->s_next;
	osi_Free(del, sizeof(struct server_entry));
    }
}

void dump_traces() {

    fprintf(stderr, "Dumping trace log ... \n");
    icl_DumpUserToFile(stderr);
}


main(int argc, char **argv)
{
    int error = 0;
    struct icl_log *logp;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s <data file>\n", argv[0]);
	exit(1);
    }

    error = icl_CreateLog("cmfx", 60*1024, &logp);
    if (error == 0)
        error = icl_CreateSet("cm", logp, (struct icl_log *) 0, &cm_iclSetp);
    if (error)
        fprintf(stderr,
		"cm: warning: can't init icl for cmfx, code %d continuing\n",
		error);

    printf("\n\nLoading server database...");
    error = load_server_table(argv[1]);
    printf("done %d errors\n", error);

    printf("\n\nStarting sequential preferences test...\n");
    error = set_prefs();
    if (error)
	dump_traces();

    threadPoolHandle = tpq_Init(2,2,2,30, "preftest");
    printf("\n\nStarting parallel preferences test..\n");
    while(!error)
	error = ParallelPrefs();

    dump_traces();

    delete_database();
    exit(0);
}
