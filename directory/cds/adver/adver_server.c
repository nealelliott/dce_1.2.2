/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_server.c,v $
 * Revision 1.1.4.2  1996/02/18  19:23:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:10:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:14  root]
 * 
 * Revision 1.1.2.8  1994/08/24  20:14:02  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:05:53  mccann]
 * 
 * Revision 1.1.2.7  1994/08/18  16:44:10  mccann
 * 	remove un-synch code ot: 11680
 * 	[1994/08/18  16:43:46  mccann]
 * 
 * Revision 1.1.2.6  1994/07/06  20:43:23  ohara
 * 	include fcntl.h
 * 	[1994/07/06  19:26:21  ohara]
 * 
 * Revision 1.1.2.5  1994/06/30  19:15:08  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:35:16  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.4  1994/06/23  18:28:06  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:08:20  mccann]
 * 
 * Revision 1.1.2.3  1994/06/09  16:06:51  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:22  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/25  22:22:18  peckham
 * 	Take operations with side-effects out of dce_assert().
 * 	Make sure threads are detached when appropriate.
 * 	[1994/05/25  22:17:45  peckham]
 * 
 * Revision 1.1.2.1  1994/05/12  21:10:16  peckham
 * 	Avoid SIGPIPE crashing advertiser.
 * 	[1994/05/12  19:11:58  peckham]
 * 
 * $EndLog$
 */
/*
 * Module: adver_server.c
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
 */

#include <adver.h>		/* adver common definitions */
#include <dce/assert.h>
#include <dns_strings.h>	/* socket locations */
#include <cache.h>
#include <sys/errno.h>		/* errno access */
#include <sys/file.h>
#include <sys/un.h>		/* unix domain sockets */
#include <sys/wait.h>
#include <fcntl.h>

#ifdef DCE_SEC
# include <dce/dce_cf.h>
#endif

/*
 * Main control structure
 */
struct server {
    dthread_mutex_t mutex;
    dthread_t       listener;
    void           *cache_p;
    void           *advertise_p;
    volatile int    s;
};

struct client {
    struct server  *server_p;
    struct sockaddr_un addr;	/* Unix IPC domain sockets */
    int             addrlen;
    int             s;
};

#define CHECKMSG(ptr, ptrmax)					\
    if (((ptr) > (ptrmax))) {					\
    dce_svc_printf(CDS_S_ADVER_BADADVER_MSG);			\
    return;							\
    }

static dthread_attr_t adver_listener_attr = {"adver_server_listener"};
static dthread_attr_t adver_client_attr = {"adver_server_client"};

static dthread_address_t
adver_server_listener(dthread_address_t);

static void
adver_listener_cleanup(void *);

static dthread_address_t
adver_server_client(dthread_address_t);

static void
adver_client_cleanup(void *);

static void
dnsProcessMsg(struct client *,
	      byte_t *,
	      byte_t *);

static void
ProcessAdvertisement (struct client *const,
		      byte_t *,
		      int,
		      struct CleFlags *);

/*
 * adver_server_new - listen for servers
 */
void *
adver_server_new (void *cache_p,
		  void *advertise_p)
{
    struct server *const server_p = (struct server *)malloc(sizeof(struct server));

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   " >adver_server_new(%p,%p)", cache_p, advertise_p));

    if (server_p)
    {
	int thstatus;

	ZERO_bytes(server_p, sizeof(*server_p));

	if ((thstatus = dthread_create_mutex(&server_p->mutex)) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
	}

	server_p->cache_p = cache_p;
	server_p->advertise_p = advertise_p;

	CDS_LOCK(server_p->mutex)

	if ((thstatus = dthread_create(&server_p->listener,
				&adver_listener_attr,
				(dthread_start_routine)adver_server_listener,
				(dthread_address_t)server_p)) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	}

	CDS_END_LOCK(server_p->mutex)
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   " <adver_server_new(%p,%p) return(%p)",
		   cache_p, advertise_p, server_p));

    return (server_p);
}

/*
 * adver_server_delete - shut down servers
 */
void
adver_server_delete (void *arg)
{
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >adver_server_delete(%p)", arg));

    if (arg)
    {
	struct server *const server_p = arg;
	dthread_address_t join_status;

	(void) dthread_cancel(server_p->listener);
	(void) dthread_join(server_p->listener, &join_status);
	(void) dthread_detach(&server_p->listener);

	dthread_delete_mutex(&server_p->mutex);
	free(arg);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <adver_server_delete(%p)", arg));
}

static dthread_address_t
adver_server_listener (dthread_address_t arg)
{
    struct sockaddr_un sockaddr_un;	/* Unix IPC domain sockets */
    struct server *const server_p = arg;
    int s;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   " >adver_server_listener(%p)", arg));

    ZERO_bytes(&sockaddr_un, sizeof(sockaddr_un));
    sockaddr_un.sun_family = AF_UNIX;
    sprintf(sockaddr_un.sun_path, CDS_ADVER_SOCK, dcelocal_path);

    if (0 <= (s = adver_open_socket(&sockaddr_un, 2))) {
	server_p->s = s;
	dthread_cleanup_push(adver_listener_cleanup, server_p);

	/* close this socket on exec() */
	(void) fcntl(s, F_SETFD, 1);
	(void) chmod(sockaddr_un.sun_path, 0600);

	for (;;) {
	    dthread_t       client;
	    struct client  *client_p = (struct client *) malloc(sizeof(struct client));
	    int thstatus;

	    if (!client_p)
		if (0 <= dthread_delay(1.0))
		    continue;
		else
		    break;

	    ZERO_bytes(client_p, sizeof(*client_p));
	    client_p->server_p = server_p;
	    client_p->addrlen = sizeof(client_p->addr);
	    client_p->s = adver_accept_socket(s,
					      &client_p->addr,
					      &client_p->addrlen);

	    if (client_p->s < 0) {
		free((char *) client_p);
		break;
	    }

	    if ((thstatus = dthread_create(&client,
				&adver_client_attr,
				(dthread_start_routine)adver_server_client,
				(dthread_address_t)client_p)) < 0)
	    {
		dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	    }

	    (void) dthread_detach(&client);
	}
	dthread_cleanup_pop(1);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   " <adver_server_listener(%p) return(%p)", arg, NULL));

    return (NULL);
}

static void
adver_listener_cleanup (void *arg)
{
    struct server  *const server_p = arg;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >adver_listener_cleanup(%p)", arg));

    dthread_close(server_p->s);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <adver_listener_cleanup(%p)", arg));
}

static dthread_address_t
adver_server_client (dthread_address_t arg)
{
    struct sigaction newsig;
    struct client  *const client_p = arg;
    struct server  *const server_p = client_p->server_p;
    const int       s = client_p->s;
    int		    sig_status;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   " >adver_server_client(%p)", arg));

    /*
     * Make sure sigpipe does not stop us.
     */
    newsig.sa_handler = SIG_IGN;
    sigemptyset(&newsig.sa_mask);
    newsig.sa_flags = 0;
    sig_status = sigaction(SIGPIPE, &newsig, NULL);
    dce_assert(cds__svc_handle, 0 <= sig_status);

    /* set up rpc context */
    rpc_ss_enable_allocate();

    dthread_cleanup_push(adver_client_cleanup, client_p);

    for (;;) {
	union {
	    char            buf[sizeof(BChldReqMsg_t)];
	    int             size;
	    AdvertiseRequest_t msg;
	} in;
	dns_status_t    status;
	int             total;

	total = adver_read_socket(s, in.buf, AdReqHdrSize);

	if (total <= 0)
	    goto cleanup_client;

	while (total < in.msg.msgLen) {
	    const int cc = adver_read_socket(s, &in.buf[total],
						in.msg.msgLen-total);

	    if (cc <= 0)
		goto cleanup_client;

	    total += cc;
	}

	switch (adver_cache_get_state(server_p->cache_p)) {
	case On:
	case Initial:
	    switch (in.msg.advType) {
	    case an802Msg:
		/* Add this to the cache */
		dnsProcessMsg(client_p, (byte_t *)in.msg.msg,
			      (byte_t *)SKIP_bytes(in.buf, in.msg.msgLen));

		/* Add this to the advertisement list */
		L_to_rpcList_AdvertiseMessage_V2((byte_t *)in.msg.msg);
	    case PingMsg:
		/* advertise everything in list */
		cds_SendAdvertisements();
		break;
	    case ResetMsg:
	    case QuitMsg:
		/* Clear out the advertisement list */
		cds_clear_rpcList();
		break;
	    default:
		/*
		 * A Bug !!!
		 */
		/* force abort */
		dce_assert(cds__svc_handle, 1==2);
		goto cleanup_client;
	    }
	    break;
	default:
	    break;
	}
    }

cleanup_client:
    dthread_cleanup_pop(1);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   " <adver_server_client(%p) return(%p)", arg, NULL));

    return (NULL);
}

static void
adver_client_cleanup (void *arg)
{
    struct client  *const client_p = arg;
    struct server  *const server_p = client_p->server_p;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >adver_client_cleanup(%p)", arg));

    dthread_close(client_p->s);

    free(arg);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <adver_client_cleanup(%p)", arg));
}

/*
 * dnsProcessMsg
 * 
 * description: processes a message from the ethernet: solicitation or
 * advertisement in version 1 (ethernet) or version 2 (802.3) format
 * 
 * in: event block
 * 
 * out: none
 * 
 * returns: void
 * 
 * side-effects: if an advertisement is received, will add to cache if a
 * solicit is received, may advertise
 */
static void
dnsProcessMsg (struct client *client_p,
	       byte_t * buf_p,
	       byte_t * end_p)
{
    struct server *const server_p = client_p->server_p;
    SAMsg_t         message;
    struct CleFlags cleFlags;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " >dnsProcessMsg(%p)", client_p));

    /* if the clerk isn't on, drop the message */
    switch (adver_cache_get_state(server_p->cache_p)) {
    case Initial:
	adver_cache_set_state(server_p->cache_p, On); /* we're talkin' now */
    case On:
	COPY_protocolVersion(buf_p, &message.header.version);
	buf_p = SKIP_protocolVersion(buf_p);

	message.header.msgtype.mt_value[0] = GET8(buf_p);

	/* set up flags appropriate for on-LAN clearinghouse */
	ZERO_bytes(&cleFlags, sizeof(cleFlags));
	cleFlags.status.flags.onLAN = 1;
	cleFlags.status.flags.addressused = 1;
	cleFlags.status.flags.sticky = 0;
	cleFlags.status.flags.ok = 1;

	ProcessAdvertisement(client_p, buf_p, end_p - buf_p, &cleFlags);
	break;
    default:
	break;
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " <dnsProcessMsg(%p)", client_p));
}

/*
 * ProcessAdvertisement
 * 
 * Description: parse an advertisement and stick the info in the cache. Also
 * make calls to create mgmt. entries for ns and ch
 * 
 * Input:
 * message to (finish) parsing msgLen: length of remaining inMsg flagsP:
 * address of desired CleFlags settings cleListP.
 * 
 * Output: 
 * 
 * Returns: none
 * 
 */
static void
ProcessAdvertisement (struct client *const client_p,
		      byte_t * inMsg,
		      int msgLen,
		      struct CleFlags *flagsP)
{
    DEB_ASCII_BUF_SimpleName(nicknameBuf)
    SimpleName_t	cellname;
    ObjUID_t		nsUID, dirUID;
    struct server *const server_p = client_p->server_p;
    Set_t		*addressP;
    byte_t		addressBuf[sizeof(Set_overhead) +
				sizeof(SetMember_overhead) +
				MAXATTRIBUTE];	/* Approximate tower size */
    int			addressLen;
    FullName_t		*clearingHouseName;
    ObjUID_t		*clearingHouseUID;
    byte_t		*const msgMax = &inMsg[msgLen];
    int			ch_count, i;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " >ProcessAdvertisement(%p,%p,%d,)",
		   client_p, inMsg, msgLen));

    /*
     * If clerk state = initializing, cancel the dns_solicit holdown timer
     */

    adver_cache_set_state(server_p->cache_p, On);

    /*
     * pick off the nsuid 
     */
    COPY_ObjUID(inMsg, &nsUID);
    inMsg = SKIP_ObjUID(inMsg);
    CHECKMSG(inMsg, msgMax);

    /*
     * get the name 
     */
    COPY_SimpleName(inMsg, &cellname);
    inMsg = SKIP_SimpleName(inMsg);
    CHECKMSG(inMsg, msgMax);

    if (cellname.sn_name[0] == '/') 
    {
	COPY_ObjUID(&nsUID, &dirUID);
	COPY_ObjUID(cdsWildRoot, &nsUID);
    }


    /*
     * If this namespace is a cellname (leading "/"), then insert the
     * cellname in the directory cache with the given ReplicaPointers.
     */
    if (cellname.sn_name[0] == '/')
	adver_cache_add_cell(server_p->cache_p, &cellname, &dirUID, inMsg);

    /*
     * get the clearinghouse count 
     */
    ch_count = GET8(inMsg);
    CHECKMSG(inMsg, msgMax);

    /*
     * now look at the clearinghouse (or hice, as the case may be) 
     */
    for (i = 0; i < ch_count; i++)  {
      DEB_ASCII_BUF_FullName(clearingHouseNameBuf)

      clearingHouseUID = (ObjUID_t *)inMsg;
      inMsg = SKIP_ObjUID(inMsg);
      CHECKMSG(inMsg, msgMax);
      addressP = (Set_t *)inMsg;
      inMsg = SKIP_Set(inMsg);
      CHECKMSG(inMsg, msgMax);
      inMsg = SKIP_ReplicaType(inMsg);
      CHECKMSG(inMsg, msgMax);
      clearingHouseName = (FullName_t *)inMsg;

      DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
          "  ProcessAdvertisement Clearinghouse %s",
	    deb_ascii_FullName(clearingHouseNameBuf,clearingHouseName)));

      inMsg = SKIP_FullName(inMsg);
      CHECKMSG(inMsg, msgMax);
      
      /* 
       * We call CAInsertClearinghouse whether this is a new
       * clearinghouse or not - it could be an address update
       * to a previously cached clearinghouse.
       */
      (void)CAInsertClearinghouse(clearingHouseName, clearingHouseUID, 
				  flagsP, CA_CNT_NONE, addressP, NULL);
    } /* end for each ch in advertisement */
    
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " <ProcessAdvertisement(%p,%p,%d)",
		   client_p, inMsg,  msgLen));
}
