/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_cache.c,v $
 * Revision 1.1.4.2  1996/02/18  19:23:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:09:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:08  root]
 * 
 * Revision 1.1.2.7  1994/08/25  20:54:35  peckham
 * 	Zero out the data structure when first malloc()ed!
 * 	[1994/08/25  20:38:45  peckham]
 * 
 * Revision 1.1.2.6  1994/08/11  20:35:06  peckham
 * 	CR#9765 - cds client better disaster recovery.
 * 	Routine adver_cache_load_cached_servers() loads up the cache
 * 	from the wan file using cds_SendSolicitServer().
 * 	Flag cle_checked indicates that adver_cache_load_cached_servers()
 * 	should be called anywhere a clearinghouse may get defined.
 * 	[1994/08/11  04:44:52  peckham]
 * 
 * Revision 1.1.2.5  1994/06/30  19:14:58  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:34:35  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.4  1994/06/09  16:06:44  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:15  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/07  16:33:32  peckham
 * 	Fix bad random number generation in solicit delay.
 * 	[1994/05/31  18:53:44  peckham]
 * 
 * Revision 1.1.2.2  1994/05/25  22:22:09  peckham
 * 	Take operations with side-effects out of dce_assert().
 * 	Pointer mv_p loaded too early in adver_cache_add_cell().
 * 	[1994/05/25  22:17:20  peckham]
 * 
 * Revision 1.1.2.1  1994/05/12  21:09:55  peckham
 * 	New module to handle cds server requests.
 * 	[1994/05/12  19:20:45  peckham]
 * 
 * $EndLog$
 */
/*
 * Module: adver_cache.c
 * 
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1993. ALL RIGHTS RESERVED.
 * 
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 * 
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE AND
 * SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 * 
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 * 
 */

#include <adver.h>		/* adver common definitions */
#include <cache.h>		/* cache definitions */
#include <adver_ncl_defs.h>

struct data {
    dthread_mutex_t mutex;
    dthread_t       cache_solicitor;
    dthread_t       cache_flusher;
    int             memId;
    char	    cle_checked;
};

static dthread_attr_t adver_solicitor_attr = {"adver_cache_solicitor"};
static dthread_attr_t adver_flusher_attr = {"adver_cache_flusher"};

static dthread_address_t adver_cache_solicitor(dthread_address_t);
static dthread_address_t adver_cache_flusher(dthread_address_t);
static void adver_cache_load_cached_servers(void *);
static void make_addressHint_timeless(ReplicaPointer_t *);

void *
adver_cache_new (int cache_size)
{
    const int       the_size = (cache_size == -1) ? cache_size : cache_size * 1000;
    struct data    *data_p = (struct data *) malloc(sizeof(struct data));

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
                   " >adver_cache_new(%d)", cache_size));

    if (data_p) {
	ZERO_bytes(data_p, sizeof(*data_p));

	/* set-up the cache */
	CASetUp();

	/* and load it */
	if (0 <= (data_p->memId = CALoad(the_size))) {
	    int thstatus;

	    CAfixed_p->ca_state = Off;

	    if ((thstatus = dthread_create_mutex(&data_p->mutex)) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
	    }

	    adver_cache_set_state (data_p, Initial);
	} else {
	    dce_svc_printf(CDS_S_ADVER_CACHE_A_MSG);
	}
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
                   " <adver_cache_new(%d) return(%p)", cache_size, data_p));

    return (data_p);
}

void
adver_cache_delete (void *handle)
{
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
                   " >adver_cache_delete(%d)", handle));

    if (handle) {
	struct data *const data_p = handle;
	int thstatus;

	adver_cache_set_state(handle, Shut);

	if (CAfixed_p) {
	    CAUnLoad(data_p->memId);
	    CAfixed_p = NULL;
	}

	/* good-byte, data... */
	if ((thstatus = dthread_delete_mutex(&data_p->mutex)) < 0) {
	    dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
	}
	free(handle);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
                   " <adver_cache_delete(%d)", handle));
}

static dthread_address_t
adver_cache_solicitor (dthread_address_t arg)
{
    struct data    *const data_p = (struct data *) arg;
    utc_t now_utc;
    timespec_t now, inacc;
    long tdf;
    int secs = ((rand() % (2 * DEF_SOLICIT_VAL)) + (3 * DEF_SOLICIT_VAL)) / 4;
    int utc_status;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
                   " >adver_cache_solicitor(%p)", arg));

    /*
     * Get current time
     */
    utc_status = utc_gettime(&now_utc);
    dce_assert(cds__svc_handle, (0 <= utc_status));
    utc_status = utc_bintime(&now, &inacc, &tdf, &now_utc);
    dce_assert(cds__svc_handle, 0 <= utc_status);
    now.tv_sec += secs;

    /* InitiateRandomTimer(DEF_SOLICIT_VAL, RT34To54) */
    dthread_delay((float)secs);

    if (!data_p->cle_checked)
	adver_cache_load_cached_servers(data_p);

    cds_SendSolicit();

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
                   " <adver_cache_solicitor(%p) return(%p)", arg, NULL));

    return (NULL);
}

static dthread_address_t
adver_cache_flusher (dthread_address_t arg)
{
    struct data    *const data_p = (struct data *) arg;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
                   " >adver_cache_flusher(%p)", arg));

    for (;;) {
	dthread_delay((float) DEF_FLUSH_CACHE_VAL);

	if (CAfixed_p) {

	    if (!data_p->cle_checked)
		adver_cache_load_cached_servers(data_p);

	    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		"  adver_cache_flusher() Flushing cache to disk...\n"));

	    CAWriteLockCache();

	    pthread_cleanup_push(CAWriteUnlockCache, NULL);

	    CASysWrite();

	    pthread_cleanup_pop(0);

	    CAWriteUnlockCache();
	}
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
                   " <adver_cache_flusher(%p) return(%p)", arg, NULL));

    return (NULL);
}

ClerkStates
adver_cache_get_state (void *handle)
{
    ClerkStates state = Off;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
                   " >adver_cache_get_state(%p)", handle));

    if (handle && CAfixed_p) {
	struct data *const data_p = handle;

	CDS_LOCK(data_p->mutex)

	state = CAfixed_p->ca_state;

	CDS_END_LOCK(data_p->mutex);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
                   " <adver_cache_get_state(%p) return(%d)", handle, state));

    return(state);
}

void
adver_cache_set_state (void *handle,
		       ClerkStates newstate)
{
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
                   " >adver_cache_set_state(%p,%d)", handle, newstate));

    if (handle && CAfixed_p) {
	struct data *const data_p = handle;
	ClerkStates oldstate;

	CDS_LOCK(data_p->mutex);

	oldstate = CAfixed_p->ca_state;
	CAfixed_p->ca_state = newstate;

	CDS_END_LOCK(data_p->mutex);

	if (oldstate != newstate) {
	    dthread_address_t join_status;
	    int thstatus;

	    if (oldstate == Initial) {
		/* make sure the solicitor is dead and buried */
		(void) dthread_cancel(data_p->cache_solicitor);
		(void) dthread_join(data_p->cache_solicitor, &join_status);
		(void) dthread_detach(&data_p->cache_solicitor);
	    } else if (oldstate == On) {
		/* make sure the flusher is dead and buried */
		(void) dthread_cancel(data_p->cache_flusher);
		(void) dthread_join(data_p->cache_flusher, &join_status);
		(void) dthread_detach(&data_p->cache_flusher);
	    }

	    if (newstate == Initial) {
		if ((thstatus = dthread_create(&data_p->cache_solicitor,
				   &adver_solicitor_attr,
				   (dthread_start_routine)adver_cache_solicitor,
				   (dthread_address_t)data_p)) < 0) {
		    dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
		}
	    } else if (newstate == On) {
		if ((thstatus = dthread_create(&data_p->cache_flusher,
				   &adver_flusher_attr,
				   (dthread_start_routine)adver_cache_flusher,
				   (dthread_address_t)data_p)) < 0) {
		    dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
		}
	    }
	}
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
                   " <adver_cache_set_state(%p,%d)", handle, newstate));
}

/*
 *
 */
void
adver_cache_add_cle (void *handle,
		     FullName_t * name_p,
		     ObjUID_t * cleUID_p,
		     Set_t * rpset_p,
		     struct CleFlags *flags_p,
		     SimpleName_t * mgmt_p)
{
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_ObjUID(cleUIDBuf)
    DEB_ASCII_BUF_SimpleName(mgmtBuf)

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
                   " >adver_cache_add_cle(%p,%s,%s,%p,,%s)",
		   handle, deb_ascii_FullName(nameBuf,name_p),
		   deb_ascii_ObjUID(cleUIDBuf,cleUID_p), rpset_p,
		   deb_ascii_SimpleName(mgmtBuf,mgmt_p)));

    if (handle) {
	struct data *const data_p = handle;

	if (!data_p->cle_checked)
	    adver_cache_load_cached_servers(data_p);

	(void) CAInsertClearinghouse(name_p, cleUID_p, flags_p, CA_CNT_NONE, rpset_p, mgmt_p);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
                   " <adver_cache_add_cle(%p,%s,%s,%p,,%s)",
		   handle, deb_ascii_FullName(nameBuf,name_p),
		   deb_ascii_ObjUID(cleUIDBuf,cleUID_p), rpset_p,
		   deb_ascii_SimpleName(mgmtBuf,mgmt_p)));
}

void
adver_cache_add_rep (void *handle,
		     FullName_t * name_p,
		     ObjUID_t * dirUID_p,
		     Set_t * rpset_p,
		     int root)
{
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_ObjUID(dirUIDBuf)
    DEB_ASCII_BUF_SimpleName(mgmtBuf)

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
                   " >adver_cache_add_rep(%p,%s,%s,%p,%d)",
		   handle, deb_ascii_FullName(nameBuf,name_p),
		   deb_ascii_ObjUID(dirUIDBuf,dirUID_p), rpset_p, root));

    if (handle)
	(void) CAAddReplicas(name_p, dirUID_p, rpset_p, root);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
                   " <adver_cache_add_rep(%p,%s,%s,%p,%d)",
		   handle, deb_ascii_FullName(nameBuf,name_p),
		   deb_ascii_ObjUID(dirUIDBuf,dirUID_p), rpset_p, root));
}

void
adver_cache_add_cell (void *handle,
		      SimpleName_t * name_p,
		      ObjUID_t * dirUID_p,
		      byte_t * replicas_p)
{
    DEB_ASCII_BUF_SimpleName(nameBuf)
    DEB_ASCII_BUF_ObjUID(dirUIDBuf)
    FullName_t      cellname;
    unsigned char   nickname[SIMPLENAMEMAX+1];
    byte_t          rpset[MAXATTRIBUTE], *rp_p = replicas_p;
    Set_t          *rpset_p = (Set_t *)rpset;
    MemberValue_t  *mv_p;
    const int       ch_count = *rp_p++;
    int             cellnameLen = sizeof(cellname);
    int             cnt;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
                   " >adver_cache_add_cell(%p,%s,%s,%p)",
		   handle, deb_ascii_SimpleName(nameBuf,name_p),
		   deb_ascii_ObjUID(dirUIDBuf,dirUID_p), replicas_p));


    cnt = EXT8(name_p->sn_length);
    COPY_bytes(name_p->sn_name, nickname, cnt);
    nickname[cnt] = '\0';
    (void) dnsCvtCDSFullToOpq(nickname,
		(dns_full_name_t *) & cellname, &cellnameLen);
    /*
     * need a Set of ReplicaPointers
     */
    INS8(rpset_p->st_flag, ST_present);
    INS16(rpset_p->st_offsetLength, ch_count * sizeof(word_u));
    mv_p = (MemberValue_t *) DATA_Set(rpset_p);
    for (cnt = 0; cnt < ch_count; cnt++) {
	INS16(OFFSET_Set(rpset_p, cnt), DIFF_bytes(mv_p, rpset_p));
	INS8(mv_p->mv_flag, MV_present);
	ZERO_Timestamp(mv_p->mv_timestamp);
	INS16(mv_p->mv_length, LEN_ReplicaPointer(rp_p));
	INS8(mv_p->mv_valuetype, VT_ReplicaPointer);
	COPY_ReplicaPointer(rp_p, mv_p->mv_value);
	make_addressHint_timeless((ReplicaPointer_t *) mv_p->mv_value);
	rp_p = SKIP_ReplicaPointer(rp_p);
	mv_p = (MemberValue_t *) SKIP_MemberValue(mv_p);
    }
    INS16(rpset_p->st_valueLength, DIFF_bytes(mv_p, DATA_Set(rpset_p)));

    adver_cache_add_rep(handle, &cellname, dirUID_p, rpset_p, TRUE);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
                   " <adver_cache_add_cell(%p,%s,%s,%p)",
		   handle, deb_ascii_SimpleName(nameBuf,name_p),
		   deb_ascii_ObjUID(dirUIDBuf,dirUID_p), replicas_p));
}

/*
 * adver_cache_load_cached_servers
 *
 * Go through the wan file and update any missing cached servers
 * missing from the cache.
 */
static void
adver_cache_load_cached_servers(void *handle)
{
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
                   " >adver_cache_load_cached_servers(%p)", handle));

    if (handle) {
	struct data *const data_p = handle;
	void *srv_handle;

	if ((srv_handle = CASysServersOpen(0)) != NULL) {
	    cds_Name_t tag;
	    char towers[MAXATTRIBUTE];
	    char name[CDS_STR_SNAME_MAX+1];

	    data_p->cle_checked = TRUE;

	    while (CASysServersRead(srv_handle, name, towers)) {
		char *tower_p = towers;
		int cnt = *tower_p++;
		int name_len = sizeof(tag.nm_name);

		DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
			       "  adver_cache_load_cached_servers(%p) tag(%s)",
			       handle, name));

		cdsCvtSimpleToOpq((unsigned char *)name, 0,
				  (cds_simple_name_t *)tag.nm_name,
				  &name_len);
		tag.nm_length = name_len;

		while (TRUE) {
		    switch (cds_SendSolicitServer((unsigned_char_t *)tower_p, &tag)) {
		    default:
			if (--cnt) {
			    tower_p = strchr(tower_p, '\0') + 1;
			    continue;
			}
			data_p->cle_checked = FALSE;
		    case CDS_SUCCESS:
		    case CDS_CSCLHEXISTS:
			break;
		    }
		    break;
		}
	    }

	    CASysServersClose(srv_handle);
	}
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
                   " <adver_cache_load_cached_servers(%p)", handle));
}

static void 
make_addressHint_timeless (ReplicaPointer_t *rp_p)
{
    Set_t *set_p = (Set_t *)rp_p->rp_addressHint;
    register MemberValue_t *mv_p = (MemberValue_t *)DATA_Set(set_p);
    register int num_set;

    if ((num_set = NUM_Set(set_p)) != 0)
	do {
	    ZERO_Timestamp(mv_p->mv_timestamp);
	    mv_p = (MemberValue_t *)SKIP_MemberValue(mv_p);
	} while (--num_set);
}
