/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pe_update.c,v $
 * Revision 1.1.4.2  1996/11/13  17:56:52  arvind
 * 	Un-munge history.
 * 	[1996/10/18  20:17 UTC  jrr  /main/DCE_1.2.2/jrr_122_6/2]
 *
 * 	Fix memory leak of realmname in scd_print_krb_conf().
 * 	*
 * 	[1996/10/18  20:11 UTC  jrr  /main/DCE_1.2.2/jrr_122_6/1]
 *
 * 	Fix CHFts19541: a replica that is "marked for deletion" causes the
 * 	pe_site file to be corrupted with the presence of two " @: " lines.
 * 	[1996/10/09  23:56 UTC  elley  /main/HPDCE02/elley_CHFts19541/1]
 *
 * 	Merge fix for dced core dump
 * 	[1996/07/09  19:37 UTC  bissen  /main/bissen_fix_19440/2]
 *
 * 	Remove HP's reentrant gethostbyaddr
 * 	[1996/06/26  13:53 UTC  bissen  /main/bissen_fix_19440/1]
 *
 * Revision 1.1.4.1  1996/08/09  11:54:18  arvind
 * 	Merge fix for dced core dump
 * 	[1996/07/09  19:37 UTC  bissen  /main/bissen_fix_19440/2]
 * 
 * 	Remove HP's reentrant gethostbyaddr
 * 	[1996/06/26  13:53 UTC  bissen  /main/bissen_fix_19440/1]
 * 
 * 	CHFts17270 dced memory leak
 * 	CHFts16600 core dump
 * 	[1996/01/30  02:26 UTC  rps  /main/HPDCE02/rps_mothra_patch/1]
 * 
 * Revision 1.1.2.2  1996/02/18  19:16:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:42  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  20:54:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:17 UTC  psn
 * 	Remove non-portable call gethostbyaddr_r() for OSF 1.2.1 drop
 * 
 * 	HP revision /main/HPDCE02/4  1995/10/10  16:57 UTC  sommerfeld
 * 	fix descriptor leak on error path.
 * 	get krb5 config file name from kerberos header files.
 * 	[1995/10/04  15:40 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/5]
 * 
 * 	Add code to update /krb5/krb.conf, too.
 * 	[1995/09/08  22:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/4]
 * 
 * 	Rename functions.
 * 	[1995/08/31  05:59 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/3]
 * 
 * 	Performance: let secidmap do the rca_site_bind iff it's needed.
 * 	[1995/08/14  15:17 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/02  20:04 UTC  aha
 * 	CHFts15825: dced pe_site update thread does not rebind
 * 
 * 	HP revision /main/HPDCE02/aha_mothra8/1  1995/08/02  18:42 UTC  aha
 * 	CHFts15825: dced pe_site update thread doesn't rebind;
 * 	 call new APIs sec_rgy_rep_admin_info_full_any() and sec_rgy_replist_read_any()
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/24  18:49 UTC  sommerfeld
 * 	Add change header.
 * 	Fix memory trashing problem (CHFts15146)
 * 	[1995/12/08  20:49:18  root]
 * 
 * $EndLog$
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dce/dce_error.h>
#include <dce/rpcpvt.h>
#include <dce/rpc.h>
#include <dcedimpl.h>
#include <dce/binding.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <dce/repadm.h>
#include <dce/uuid.h>
#include <dce/replist.h>
#include <dce/secidmap.h>

#include <krb5/osconf.h>

/*
 * alternate data structure which is more amenable to what
 * we're looking to do..
 */

struct pe_site_line 
{
    int weight;
    unsigned char *cellname;
    unsigned char *replica_name;
    unsigned char *cellid;
    unsigned char *protseq;
    unsigned char *addr;
};

struct pe_update_context 
{
    sec_rgy_name_t real_cellname;
    unsigned char *real_cellid_string;
    rs_replica_item_t	*replist;
    unsigned32 num_reps;
    struct pe_site_line *psline;
    int npslines;
    boolean32 dflag;
};


/*
 * copied from src/security/client/admin/sec_admin/sa_replist.c
 */

/*
 * Retrieve a copy of the replica list
 */
static error_status_t scd_refresh_replist(ctx, context)
    struct pe_update_context *ctx;
    sec_rgy_handle_t	context;
{
    uuid_t      marker;
    unsigned32	nreps = 50;		/* Try 50 first */
    error_status_t st;
    rs_replica_info_t rep_info;
    
    /* Clean up previous replist if it exists */
    if (ctx->replist != NULL) {
	free(ctx->replist);
    }
	

    sec_rgy_rep_admin_info_any(context, &rep_info, &st);
    if (st != error_status_ok) 
    {
	if (ctx->dflag)
	    fprintf(stderr, "rep_admin_info_any hiccup: %x\n", st);
	return st;
    }
    uuid_to_string (&rep_info.cell_sec_id, &ctx->real_cellid_string, &st);
    if (st != error_status_ok) 
    {
	return st;
    }
    
    /* Can't do anything without being able to reset the marker */
    uuid_create_nil(&marker, &st);
    if (st != error_status_ok)
	return st;

    ctx->replist = (rs_replica_item_p_t)
	malloc(nreps * sizeof(rs_replica_item_t));
    if (ctx->replist == NULL)
	return sec_s_no_memory;

    sec_rgy_replist_read_any(context, &marker, nreps,
			 &ctx->num_reps, ctx->replist, &st);
    if (st != error_status_ok) {
	free(ctx->replist);
	ctx->replist = NULL;
	ctx->num_reps = 0;
	return st;
    }
    
    if (ctx->num_reps > nreps) {
	free(ctx->replist);
	nreps = ctx->num_reps;
        ctx->replist = (rs_replica_item_p_t)
	    malloc(nreps * sizeof(rs_replica_item_t));
	if (ctx->replist == NULL) {
	    ctx->num_reps = 0;
	    return sec_s_no_memory;
	}
	sec_rgy_replist_read_any(context, &marker, nreps, &ctx->num_reps,
			     ctx->replist, &st);
	if (st != error_status_ok) {
	    free(ctx->replist);
	    ctx->replist = NULL;
	    ctx->num_reps = 0;
	    return st;
	}
    }
    return error_status_ok;
}


/*
 * qsort callback function: compare weights (computed earlier)
 */

static int compare_weights (const void *a, const void *b)
{
    struct pe_site_line *pa = (struct pe_site_line *)a;
    struct pe_site_line *pb = (struct pe_site_line *)b;

    return pb->weight - pa->weight;
}

static error_status_t scd_sort_pesite(struct pe_update_context *ctx) 
{
    int npslines = ctx->npslines;
    int i, j;
    
    /* SANITY CHECK */
    if (npslines == 0) 
    {
	if (ctx->dflag)
	    fprintf(stderr, "Empty replist!\n");
	return dced_s_empty_replist;
	
    }
    qsort (ctx->psline, npslines, sizeof(struct pe_site_line),
	   compare_weights);

    /*
     * Ok, now bias against 2nd and subsequent addresses on same host,
     * to get some round-robining going..
     */
    for (i=1; i<npslines; i++) 
    {
	for (j=0; j<i; j++) 
	{
	    if (strcmp((const char *)ctx->psline[i].replica_name,
		       (const char *)ctx->psline[j].replica_name) == 0) 
	    {
		ctx->psline[i].weight -= 2000;
	    }
	}
    }
    /* .. and re-sort */
    qsort (ctx->psline, npslines, sizeof(struct pe_site_line),
	   compare_weights);    
    
    return error_status_ok;
}

static error_status_t scd_print_pesite(struct pe_update_context *ctx, char *file)
{
    int i;
    int npslines = ctx->npslines;
    FILE *outfile = NULL, *infile;
    char *outfilename = malloc (strlen(file)+5);
    char buf[1024];
    int ret;
    error_status_t st = -1;

    int cellnamelen = strlen ((const char *)ctx->real_cellname);

    if (outfilename == NULL)
	goto bail;

    strcpy(outfilename, file);
    strcat(outfilename, ".new");

    outfile = fopen(outfilename, "w");
    if (outfile == NULL)
        goto bail;
        
    for (i=0; i < npslines; i++) 
    {
	struct pe_site_line *psl = &(ctx->psline[i]);
	fprintf(outfile, "%s %s@%s:%s %s\n", psl->cellname, psl->cellid,
		psl->protseq, psl->addr, psl->replica_name);
    }
    if (infile = fopen(file, "r")) 
    {
	while (fgets(buf, sizeof(buf), infile) != NULL) 
	{
	    if ((strncmp(buf, (const char *)ctx->real_cellname, cellnamelen) == 0) &&
		buf[cellnamelen] == ' ')
		continue;
	    /*
	     * if the first character of a pe_site line is a space, there
	     * has probably been some corruption - like buf[0]=" @: " -
	     * so we want to ignore such lines and not copy them into outfile
	     */
	    if (buf[0] == ' ')
	        continue;

	    fputs(buf, outfile);
	}
	fclose(infile);
    }
    if ((fflush(outfile) != 0) ||
	(fsync(fileno(outfile)) != 0))
	goto bail;

    (void)fclose(outfile);
    
    ret = rename(outfilename, file);
    free(outfilename);
    if (ret)
	return errno;
    else 
	return error_status_ok;

bail:
    if (outfile) {
        (void)fclose(outfile);
    }
    if (outfilename) {
        (void)unlink(outfilename);
        free(outfilename);
    }
    return st;
}

static char *ipstr_to_hostname(unsigned char *addr)
{
    struct hostent *h;
    struct in_addr ina;
    static char h_name[1024];

    ina.s_addr = inet_addr((char *)addr);
    if (h = gethostbyaddr((void *)&ina, sizeof(ina), AF_INET)) {
        strcpy(h_name, h->h_name);
	return h_name;
    } else 
	return NULL;
}

/* Update krb.conf file in place */

static error_status_t scd_print_krb_conf(struct pe_update_context *ctx, char *file)
{
    int i;
    int npslines = ctx->npslines;
    FILE *outfile = NULL, *infile;
    char *outfilename = malloc (strlen(file)+5);
    char buf[1024];
    int ret;
    extern char *sec_krb_realm_from_dce_cell();
    char *realmname = sec_krb_realm_from_dce_cell(ctx->real_cellname);
    int realmnamelen = strlen(realmname);

    if (outfilename == NULL)
	goto bail;

    strcpy(outfilename, file);
    strcat(outfilename, ".new");

    outfile = fopen(outfilename, "w");
    if (outfile == NULL)
        goto bail;
        
    if (infile = fopen(file, "r")) 
    {
	while (fgets(buf, sizeof(buf), infile) != NULL) 
	{
	    if ((strncmp(buf, realmname, realmnamelen) == 0) &&
		buf[realmnamelen] == ' ')
		continue;
	    fputs(buf, outfile);
	}
	fclose(infile);
    }
    for (i=0; i < npslines; i++) 
    {
	char *hostname;
	struct pe_site_line *psl = &(ctx->psline[i]);
	if (strcmp((char *)psl->protseq, "ncadg_ip_udp") != 0)
	    continue;
	hostname = ipstr_to_hostname(psl->addr);
	if (hostname == NULL)
	    goto bail;
	fprintf(outfile, "%s %s\n", realmname, hostname);
    }
    if ((fflush(outfile) != 0) ||
	(fsync(fileno(outfile)) != 0))
	goto bail;

    (void)fclose(outfile);
    ret = rename(outfilename, file);
    free(outfilename);
    if (ret)
	return errno;
    else 
	return error_status_ok;

bail:
    if (realmname) {
        free(realmname);
    }
    if (outfile) {
        (void)fclose(outfile);
    }
    if (outfilename) {
        (void)unlink(outfilename);
        free(outfilename);
    }
    return -1;                  /* !!! need stcode */
}

/* bias against selecting master; perhaps should be comparable to
 * weight against selecting node on different subnet..
 */
static int master_badness = 0;

static int compute_replica_weight (rs_replica_item_t *repitem) 
{
    int weight = 10000;

    if (repitem->master)
	weight -= master_badness;

    if (repitem->deleted)
	weight = -1;

    return weight;
}

/*
 * grovel through string_binding looking for likely suspects..
 * unfortunately, there aren't enough string_binding primitives, 
 * so we have to make this up as we go along..
 */

#define RPC_RANDOM_GET(lower, upper) \
	(((rpc__random_get(lower, upper)) % (upper - lower + 1)) + lower)
extern unsigned32 rpc__random_get(unsigned32, unsigned32);

static int compute_addr_weight (unsigned_char_p_t addr)
{
    int weight = 0;
    error_status_t st;
    int i;
    
    weight += RPC_RANDOM_GET(0, 100);
    
    for (i=0; my_addresses[i]; i++) 
    {
	if (strcmp((const char *)addr, (const char *)my_addresses[i]) == 0) 
	{
	    weight+= 10000;
	    break;
	}
    }
    return weight;
}

static int compute_protseq_weight(unsigned_char_p_t protseq)
{
    int weight = 0;
    if (strcmp((const char *)protseq, (const char *)"ncadg_ip_udp") == 0)
	weight += 1000;
    return weight;
}

static error_status_t scd_convert_replist(struct pe_update_context *ctx) 
{
    int i, psl;
    ctx->npslines = 0;
    
    if (ctx->num_reps < 3) master_badness = 0;
    else master_badness = 100;

    for (i=0; i<ctx->num_reps; i++) 
    {
      /* 
       * we don't want to increment the number of pe_site lines (npslines) for 
       * replicas that are "marked for deletion" (in which case the
       * deleted field equals 1) 
       */

        if (ctx->replist[i].deleted == 0)
	{
	  rs_replica_twr_vec_p_t rep_twrs = ctx->replist[i].rep_twrs;
	  ctx->npslines += rep_twrs->num_towers;
	}
    }
    ctx->psline = (struct pe_site_line *)malloc (ctx->npslines * sizeof(struct pe_site_line));
    
    /* zero memory so we can make reasonable assumptions when we free later */
    for (i=0; i<ctx->npslines; i++) 
    {
	ctx->psline[i].weight = 0;
	ctx->psline[i].cellname = NULL;
	ctx->psline[i].replica_name = NULL;
	ctx->psline[i].cellid = NULL;
	ctx->psline[i].addr = NULL;
	ctx->psline[i].protseq = NULL;
    }
	
    for (psl = 0, i=0; i<ctx->num_reps; i++) 
    {
	rs_replica_twr_vec_p_t rep_twrs = ctx->replist[i].rep_twrs;
	int j, num_twrs = rep_twrs->num_towers;

	int replica_weight = compute_replica_weight(&ctx->replist[i]);
	if (replica_weight < 0)
	    continue;
	
	/*
	 * !!! there may be memory leaks in hard-to-provoke failure
	 * paths along this loop
	 */
	for (j=0; j<num_twrs; j++) 
	{
	    unsigned char *string_binding;
	    rpc_binding_handle_t bdg;
	    error_status_t st, xst;
	    int addr_weight, protseq_weight;
	    unsigned char *addr, *protseq;
	    
	    rpc_tower_to_binding(rep_twrs->towers[j]->tower_octet_string,
				 &bdg, &st);
	    if (st != error_status_ok) 
	    {
		return st;
	    }
	    rpc_binding_to_string_binding(bdg, &string_binding, &st);
	    rpc_binding_free(&bdg, &xst);
	    if (st != error_status_ok) 
	    {
		return st;
	    }
	    rpc_string_binding_parse (string_binding, NULL, &protseq, &addr,
				      NULL, NULL, &st);
	    if (st != error_status_ok) 
	    {
		return st;
	    }
	    rpc_string_free(&string_binding, &st);	    

	    protseq_weight = compute_protseq_weight(protseq);

	    addr_weight = compute_addr_weight(addr);
	    if (addr_weight < 0) 
	    {
		rpc_string_free (&string_binding, &st);
		continue;
	    }

	    ctx->psline[psl].weight = replica_weight + addr_weight
		+ protseq_weight + ctx->npslines-psl;
	    ctx->psline[psl].cellname = ctx->real_cellname;
	    ctx->psline[psl].replica_name = ctx->replist[i].rep_name;
	    ctx->psline[psl].cellid = ctx->real_cellid_string;
	    ctx->psline[psl].protseq = protseq;
	    ctx->psline[psl].addr = addr;

	    if (ctx->dflag)
		fprintf(stderr, "%s %s: %d %d %d\n", ctx->replist[i].rep_name,
			string_binding, replica_weight, addr_weight, ctx->npslines-psl);
	    
	    psl++;
	}
    }
    return error_status_ok;
}

static void free_ctx_contents(struct pe_update_context *ctx)
{
    error_status_t st;
    int i, j;
    if (ctx->psline) 
    {
	for (i=0; i<ctx->npslines; i++) 
	{
	    if (ctx->psline[i].addr != NULL)
		rpc_string_free(&(ctx->psline[i].addr), &st);
	    if (ctx->psline[i].protseq != NULL)
		rpc_string_free(&(ctx->psline[i].protseq), &st);
	}
	free(ctx->psline);
	ctx->psline = NULL;
    }
    if (ctx->replist) 
    {
	for (i=0; i<ctx->num_reps; i++) 
	{
	    
	    for (j=0; j<ctx->replist[i].rep_twrs->num_towers; j++) 
	    {
		free(ctx->replist[i].rep_twrs->towers[j]);
	    }
	    free(ctx->replist[i].rep_name);
	    free(ctx->replist[i].rep_twrs);	    
	}
	free(ctx->replist);
    	ctx->replist = NULL;
    }
    if (ctx->real_cellid_string) 
    {
	rpc_string_free(&ctx->real_cellid_string, &st);
    }
}

error_status_t scd_update_pesite (unsigned char *cellname,
				  char *file, boolean32 debug)
{
    error_status_t st, xst;
    struct pe_update_context updc;
    struct pe_update_context *ctx = &updc;
    sec_rgy_handle_t context;
    uuid_t real_cellid;
    sec_rgy_bind_auth_info_t auth_info;

    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = rpc_c_authn_level_pkt_integrity;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    memset(&updc, 0, sizeof(updc));
    ctx->dflag = debug;

    /* get *REAL* cell name, uuid here... */
    sec_id_global_parse_name (&auth_info, cellname,
			ctx->real_cellname, &real_cellid,
			NULL, NULL, &st);
    if (st != error_status_ok) 
    {
	return st;
    }
    /* then bind to it.. */
    sec_rgy_cell_bind (ctx->real_cellname, &auth_info, &context, &st);
    if (st != error_status_ok) 
    {
	free_ctx_contents(ctx);
	return st;
    }
    st = scd_refresh_replist(ctx, context);
    if (st == error_status_ok)
	st = scd_convert_replist(ctx);
    if (st == error_status_ok)
	st = scd_sort_pesite(ctx);
    if (st == error_status_ok) 
    {
	char *krb_file = DEFAULT_CONFIG_FILENAME; /* from <krb5/osconf.h> */
	
	st = scd_print_pesite(ctx, file);
	xst = scd_print_krb_conf (ctx, krb_file);
	if (st == error_status_ok)
	    st = xst;
    }
    free_ctx_contents(ctx);
    sec_rgy_site_close(context, &xst);
    return st;
}
