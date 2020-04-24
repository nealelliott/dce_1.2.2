/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_vmcc.c,v $
 * Revision 1.1.8.2  1996/02/18  00:19:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:58  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:55:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:26  root]
 * 
 * Revision 1.1.6.2  1994/08/19  17:59:32  mdf
 * 	1994/08/18 sekhar
 * 	Removed "SVC_OPEN" comment ( no code changes ).
 * 	[1994/08/19  13:11:09  mdf]
 * 
 * Revision 1.1.6.1  1994/08/04  16:13:59  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:20  mdf]
 * 
 * Revision 1.1.3.6  1993/02/05  22:30:36  sommerfeld
 * 	Bugfix: fix non-ANSI compile.
 * 	[1993/02/05  20:12:45  sommerfeld]
 * 
 * 	Bugfix: "cred" arguments to krb5_cc_fetch were inverted.
 * 	[1993/02/05  00:18:17  sommerfeld]
 * 
 * 	Rewrite to support multiple credentials at once.
 * 	[1993/02/02  23:54:50  sommerfeld]
 * 
 * Revision 1.1.3.5  1993/01/26  20:55:44  ahop
 * 	bl6d: fix some more buggy cast-assignments,
 * 	           fix malloc size in import creds
 * 	[1993/01/12  19:08:03  ahop]
 * 
 * Revision 1.1.3.4  1992/12/29  16:36:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:12  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:27:44  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  22:26:29  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/16  19:24:27  emartin
 * 	bl6c: change buggy cast-assignments to memcpys in import/export ops
 * 
 * Revision 1.1.3.2  1992/10/07  20:36:40  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:01  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:25:56  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry server virtual memory krb5 credential cache.
 *
 *     This credential cache implementation is tailored to the needs
 *     of registry server propagation and should not be used for
 *     any other purpose without careful consideration.
 *
 *     The caching needs for propagation are minimal.  The cache
 *     always contains just a single credential--for the dce-rgy
 *     server principal encrypted under a replica's session key 
 *     with the KDC.  The store operation always wipes out the 
 *     existing credential, if any.
 *
 *     The cache type prefix is "DCE_SECD_VM"
 *
 *     The following cache routines are not applicable to the
 *     replication VM credential cache.
 *
 *     get_name
 *     gen_new    (we use resolve instead as there is no difference)
 *
 *     get_first
 *     get_next
 *     end_get    (sequential gets are pointless on a single-ticket cache
 *                 but we implement them anyway in case someone calls)
 *
 *     setflags
 *
 */

#include <dce/dce.h> 
#include <dce/secsts.h>
#include <pthread.h>
#include <bstring.h>
#include <rs_rep_vmcc.h>
#include <krb5/base-defs.h>
#include <krb5/krb5.h>
#include <krb5/func-proto.h>
#include <krb5/los-proto.h> 
#include <krb5/sysincl.h>
#include <macros.h>
#include <rs_util.h>

#include <sec_svc.h>

#define KRB5_OK 0

/* forward declarations of private routines */
PRIVATE  krb5_error_code  krb5_vmcc_resolve ( 
#ifdef __STDC__
   krb5_ccache  *ch,         /* [in] */
   char         *residual    /* ignored */
#endif
);

PRIVATE  krb5_error_code  krb5_vmcc_destroy ( 
#ifdef __STDC__
    krb5_ccache ch
#endif
);

PRIVATE  krb5_error_code  krb5_vmcc_close ( 
#ifdef __STDC__
    krb5_ccache ch
#endif
);
 
PRIVATE  krb5_error_code  krb5_vmcc_store ( 
#ifdef __STDC__
    krb5_ccache  ch,      /* [in, out] */
    krb5_creds   *creds   /* [in] */
#endif
);

PRIVATE  krb5_error_code  krb5_vmcc_retrieve ( 
#ifdef __STDC__
    krb5_ccache  ch,
    krb5_flags   whichfields,   /* ignored in the VM version */
    krb5_creds   *mcreds,       /* ignore all but the client and server names */
    krb5_creds   *cred          /* [out] */
#endif
);

PRIVATE  krb5_error_code  krb5_vmcc_initialize ( 
#ifdef __STDC__
    krb5_ccache     ch,
    krb5_principal  principal
#endif
);
    
PRIVATE  krb5_error_code  krb5_vmcc_get_principal ( 
#ifdef __STDC__
    krb5_ccache     ch,
    krb5_principal  *principal
#endif
);   

PRIVATE  krb5_error_code  krb5_vmcc_remove ( 
#ifdef __STDC__
    krb5_ccache     ch,       /* [in] */
    krb5_flags      flags,    /* ignored */
    krb5_creds      *mcreds   /* [in] */
#endif
);

PRIVATE  krb5_error_code  krb5_vmcc_start_seq_get ( 
#ifdef __STDC__
    krb5_ccache ch,
    krb5_cc_cursor *cursor
#endif
);

PRIVATE  krb5_error_code  krb5_vmcc_next_cred ( 
#ifdef __STDC__
   krb5_ccache     ch,       /* [in] */
   krb5_cc_cursor  *cursor,  /* [in] */
   krb5_creds      *cred     /* [out] */
#endif
);

PRIVATE  krb5_error_code  krb5_vmcc_end_seq_get ( 
#ifdef __STDC__
    krb5_ccache ch,
    krb5_cc_cursor *cursor
#endif
);


/* 
 * op vector for propagation virtual memory credential cache.  Unsupported
 * operations are set to NULL so any attempt to use them will cause the
 * server to blow up immediately 
*/
krb5_cc_ops  krb5_vmcc_ops = {
    "DCE_SECD_VM",
    NULL,  /* krb5_vmcc_get_name */
    krb5_vmcc_resolve,
    NULL,  /* krb5_vmcc_generate_new */
    krb5_vmcc_initialize,
    krb5_vmcc_destroy,
    krb5_vmcc_close,  /* same as destroy */
    krb5_vmcc_store,
    krb5_vmcc_retrieve,
    krb5_vmcc_get_principal,
    krb5_vmcc_start_seq_get,
    krb5_vmcc_next_cred,
    krb5_vmcc_end_seq_get,
    krb5_vmcc_remove, 
    NULL,  /* krb5_vmcc_set_flags */
};

#define DOWN(ch) ((krb5_vmcc_data *)((ch)->data))

typedef struct {
    pthread_mutex_t mutex;
    krb5_principal princ;  /* cache client principal */
    int ncreds;			/* number of creds in array */
    int nroom;			/* room preallocated in array */
    krb5_creds **creds;		/* array */
} krb5_vmcc_data;

typedef struct 
{
    int seq;
} krb5_vmcc_cursor;

#define LOCK_VM_CCACHE(c) pthread_mutex_lock (&((c)->mutex))

#define UNLOCK_VM_CCACHE(c) pthread_mutex_unlock(&((c)->mutex))

/*  self-initializing stuff */
static pthread_once_t   vmcc_register_once = pthread_once_init;
static boolean32        vmcc_registered = false;

PRIVATE  void  krb5_vmcc_register
#ifndef __STDC__
    ( )
#else
  (
    void
  )
#endif
{
    if (krb5_cc_register(&krb5_vmcc_ops, false) == 0) {
        vmcc_registered = true;
    }
}

#define  CHECK_INIT() {\
    if (!vmcc_registered) { \
        pthread_once(&vmcc_register_once, krb5_vmcc_register); \
    } \
    if (!vmcc_registered) { \
        dce_svc_printf(SEC_RS_VMCC_CANT_REGISTER_MSG); \
    } \
}

PRIVATE  krb5_error_code krb5_vmcc_resolve
#ifndef __STDC__
    (ch, residual) 
    krb5_ccache  *ch;
    char         *residual;
#else
  (
    krb5_ccache  *ch,
    char         *residual
  )
#endif
{
    krb5_ccache       ccache = NULL;
    krb5_vmcc_data  *vmcp;

    CHECK_INIT();
    
    ccache = (krb5_ccache) malloc(sizeof(struct _krb5_ccache));
    if (ccache == NULL)
        return KRB5_CC_NOMEM;
    
    ccache->ops = &krb5_vmcc_ops;
    
    ccache->data = (krb5_pointer) malloc(sizeof(krb5_vmcc_data));
    if (ccache->data == NULL) {
	free(ccache);
	return KRB5_CC_NOMEM;
    }
    vmcp = DOWN(ccache);

    pthread_mutex_init(&vmcp->mutex, pthread_mutexattr_default);
    vmcp->princ = NULL;
    vmcp->ncreds = 0;
    vmcp->nroom = 0;
    vmcp->creds = NULL;

    *ch = ccache;
    return KRB5_OK;
}   


PRIVATE void  krb5_vmcc_flush_contents(vmcp)
    krb5_vmcc_data *vmcp;
{
    int i;
    
    if (vmcp->princ) 
    {
	krb5_free_principal (vmcp->princ);
	vmcp->princ = NULL;
    }
    
    if (vmcp->creds) 
    {
	for (i=0; i<vmcp->nroom; i++) 
	{
	    krb5_creds *credp = vmcp->creds[i];
	    vmcp->creds[i] = NULL;
	    if (credp)
		krb5_free_creds(credp);
	}

	free (vmcp->creds);
	vmcp->creds = NULL;
    }
    vmcp->ncreds = 0;
    vmcp->nroom = 0;
}
    
    

PRIVATE krb5_error_code  krb5_vmcc_destroy
#ifndef __STDC__
    ( ch ) 
    krb5_ccache ch;
#else
  (
    krb5_ccache ch
  )
#endif
{
    krb5_vmcc_data   *vmcp = DOWN(ch);

    LOCK_VM_CCACHE(vmcp);
    krb5_vmcc_flush_contents(vmcp);
    UNLOCK_VM_CCACHE(vmcp);
    pthread_mutex_destroy (&vmcp->mutex);
    return KRB5_OK;
}



/* 
 * close is the same as destroy in the vm version
 */
PRIVATE  krb5_error_code  krb5_vmcc_close
#ifndef __STDC__
    ( ch ) 
   krb5_ccache ch;
#else
  (
   krb5_ccache ch
  )
#endif
{
   return krb5_vmcc_destroy(ch);
}


PRIVATE  krb5_error_code  krb5_vmcc_store
#ifndef __STDC__
    ( ch, cred ) 
    krb5_ccache  ch;      /* [in, out] */
    krb5_creds   *cred;   /* [in] */
#else
  (
    krb5_ccache  ch,      /* [in, out] */
    krb5_creds   *cred	  /* [in] */
  )
#endif
{
    krb5_vmcc_data   *vmcp  = DOWN(ch);
    int new;
    int next;
    int i;
    krb5_error_code ret;
    krb5_creds **creds;
    
    LOCK_VM_CCACHE(vmcp);
    
    new = vmcp->ncreds;
    creds = vmcp->creds;
    next = new + 1;
    
    if (next > vmcp->nroom) 
    {
	krb5_creds **ncreds;
	int last = next * 2;
	ncreds = (krb5_creds **)malloc (last * sizeof(krb5_creds *));
	
	if (ncreds == NULL) 
	{
	    UNLOCK_VM_CCACHE(vmcp);
	    return KRB5_CC_NOMEM;
	}

	if (creds != NULL) 
	{
	    for (i = 0; i<new; i++) 
		ncreds[i] = creds[i];
	}

	for (i=new; i<last; i++)
	    ncreds[i] = NULL;
	vmcp->creds = ncreds;
	vmcp->nroom = last;

	if (creds != NULL)
	    free(creds);
	creds = ncreds;
    }
    ret = krb5_copy_creds (cred, &creds[new]);

    if (ret == KRB5_OK) 
	vmcp->ncreds = next;

    UNLOCK_VM_CCACHE(vmcp);
    return ret;
}


/*
 * We don't do a lot of fancy checking.  We just make sure that the
 * there is a credential in the cache, that it matches the client and
 * server in mcreds, and that the ticket hasn't expired.
 */
PRIVATE  krb5_error_code  krb5_vmcc_retrieve
#ifndef __STDC__
    ( ch, whichfields, mcreds, cred ) 
    krb5_ccache  ch;
    krb5_flags   whichfields;
    krb5_creds   *mcreds;
    krb5_creds   *cred;
#else
  (
    krb5_ccache  ch,
    krb5_flags   whichfields,
    krb5_creds   *mcreds,
    krb5_creds   *cred
  )
#endif
{
    krb5_vmcc_data  *vmcp  = DOWN(ch);
    int i, size;
    krb5_creds **creds;
    krb5_timestamp now;
    krb5_error_code ret;

    LOCK_VM_CCACHE(vmcp);
    ret = krb5_timeofday(&now);
    if (ret != KRB5_OK) 
    {
	UNLOCK_VM_CCACHE(vmcp);
	return ret;
    }
    
    creds = vmcp->creds;
    size = vmcp->ncreds;

    for (i=0; i<size; i++) 
    {
	if (krb5_cc_match (now, whichfields, mcreds, creds[i])) 
	{
	    krb5_creds *tcred;
	    ret = krb5_copy_creds (creds[i], &tcred);
	    if (ret == KRB5_OK)
	    {
		*cred = *tcred;
		free(tcred);
	    }
	    UNLOCK_VM_CCACHE(vmcp);
	    
	    return ret;
	}
    }
    UNLOCK_VM_CCACHE(vmcp);    
    return KRB5_CC_NOTFOUND;
}

PRIVATE  krb5_error_code  krb5_vmcc_initialize
#ifndef __STDC__
    ( ch, principal ) 
     krb5_ccache     ch;
     krb5_principal  principal;
#else
  (
     krb5_ccache     ch,
     krb5_principal  principal
  )
#endif
{
    krb5_vmcc_data    *vmcp  = DOWN(ch);
    krb5_error_code ret;
    
    LOCK_VM_CCACHE(vmcp);
    
    krb5_vmcc_flush_contents(vmcp);
    
    ret = krb5_copy_principal (principal, &vmcp->princ);
    
    UNLOCK_VM_CCACHE(vmcp);
    
    return ret;
}


PRIVATE krb5_error_code  krb5_vmcc_get_principal
#ifndef __STDC__
    (ch, principal) 
    krb5_ccache     ch;
    krb5_principal  *principal;
#else
  (
    krb5_ccache     ch,
    krb5_principal  *principal
  )
#endif
{
    krb5_vmcc_data  *vmcp  = DOWN(ch);
    krb5_error_code ret;
    
    LOCK_VM_CCACHE (vmcp);
    
    *principal = NULL;
    
    ret = krb5_copy_principal (vmcp->princ, principal);

    UNLOCK_VM_CCACHE(vmcp);
    
    return ret;
}


/*
 * check that the client and server principal in mcreds match what's
 * stored in VM, and. if so, blow the VM credentials away
 */
PRIVATE krb5_error_code  krb5_vmcc_remove
#ifndef __STDC__
    ( ch, flags, mcreds ) 
    krb5_ccache     ch;       /* [in] */
    krb5_flags      flags;    /* ignored */
    krb5_creds      *mcreds;  /* [in] */
#else
  (
    krb5_ccache     ch,       /* [in] */
    krb5_flags      flags,    /* ignored */
    krb5_creds      *mcreds
  )
#endif
{
    krb5_creds        creds;
    krb5_vmcc_data  *vmcp  = DOWN(ch);
    
    LOCK_VM_CCACHE(vmcp);
    dce_svc_printf(SEC_RS_VMCC_CANT_REMOVE_MSG);
    UNLOCK_VM_CCACHE(vmcp);
    
    return KRB5_OK;
}


PRIVATE  krb5_error_code  krb5_vmcc_start_seq_get
#ifndef __STDC__
    (ch, cursor ) 
    krb5_ccache ch;
    krb5_cc_cursor *cursor;
#else
  (
    krb5_ccache ch,
    krb5_cc_cursor *cursor
  )
#endif
{
    krb5_vmcc_cursor *c;
    
    c = (krb5_vmcc_cursor *)  malloc (sizeof(krb5_vmcc_cursor));
    if (c == NULL)
	return KRB5_CC_NOMEM;
    c->seq = 0;

    *cursor = (krb5_cc_cursor) c;
    return KRB5_OK;
}


PRIVATE  krb5_error_code  krb5_vmcc_next_cred
#ifndef __STDC__
    (ch, cursor, cred) 
   krb5_ccache     ch;       /* [in] */
   krb5_cc_cursor  *cursor;  /* [in] */
   krb5_creds      *cred;   /* [out] */
#else
  (
   krb5_ccache     ch,       /* [in] */
   krb5_cc_cursor  *cursor,  /* [in] */
   krb5_creds      *cred
  )
#endif
{
    krb5_vmcc_data  *vmcp  = DOWN(ch);
    krb5_vmcc_cursor *c;
    krb5_error_code ret;
    krb5_creds *tcred;
    int seq;
    

    LOCK_VM_CCACHE(vmcp);
    c = (krb5_vmcc_cursor *)*cursor;
    seq = c->seq;
    
    if (seq >= vmcp->ncreds) 
    {
	UNLOCK_VM_CCACHE(vmcp);
	return KRB5_CC_END;
    }
    
    ret = krb5_copy_creds (vmcp->creds[seq], &tcred);
    if (ret == KRB5_OK)
    {
	*cred = *tcred;
	free(tcred);
    }
    c->seq = seq + 1;
    
    UNLOCK_VM_CCACHE(vmcp);
    
    return ret;
}

PRIVATE  krb5_error_code  krb5_vmcc_end_seq_get
#ifndef __STDC__
    (ch, cursor ) 
    krb5_ccache ch;
    krb5_cc_cursor *cursor;
#else
  (
    krb5_ccache ch,
    krb5_cc_cursor *cursor
  )
#endif
{
    krb5_vmcc_cursor *c = (krb5_vmcc_cursor *)*cursor;

    free(c);

    /* Invalidate the cursor */
    *cursor = (krb5_cc_cursor) 0;
    return error_status_ok;
}
       

/*
 * PUBLIC routines
 */


PUBLIC  void  * rs_rep_vmcc_get_ccache
#ifndef __STDC__
    ( status ) 
    error_status_t  *status;
#else
  (
    error_status_t  *status
  )
#endif
{
    krb5_ccache     ccache;

    CHECK_INIT();
    *status = krb5_cc_resolve("DCE_SECD_VM:", &ccache);
    if (BAD_STATUS(status)) {
        ccache = NULL;
    }
    return ccache;
}


#ifdef SECD_VMCC_TEST

krb5_data client1 = {
#define DATA "client1-comp1"
     sizeof(DATA),
     DATA,
#undef DATA
};

krb5_data client2 = {
#define DATA "client1-comp2"
     sizeof(DATA),
     DATA,
#undef DATA
};

krb5_data server1 = {
#define DATA "server1-comp1"
     sizeof(DATA),
     DATA,
#undef DATA
};

krb5_data server2 = {
#define DATA "server1-comp2"
     sizeof(DATA),
     DATA,
#undef DATA
};    

krb5_address  addr1 = {
     0,
#define DATA "addr1"
     sizeof(DATA),
     (krb5_octet *) DATA,
#undef DATA
};                 

krb5_address  addr2 = {
     0, 
#define DATA "addr2"
     sizeof(DATA),
     (krb5_octet *) DATA,
#undef DATA
};


krb5_authdata  authdata1 = {
     0,
#define DATA "authdata1"
     sizeof(DATA),
     (krb5_octet *) DATA,
#undef DATA
};                 

krb5_authdata  authdata2 = {
     0, 
#define DATA "authdata2"
     sizeof(DATA),
     (krb5_octet *) DATA,
#undef DATA
};



krb5_creds test_creds = {
     NULL,
     NULL,
     {
      1,
      1,
      (unsigned char *) "1"
     },
     {
      1111,
      2222,
      99999999999,
      4444
     },
     1,
     5555,
     NULL,
     {
#define TICKET "This is ticket 1"
     sizeof(TICKET),
     TICKET,
#undef TICKET
     },
     {
#define TICKET "This is ticket 2"
     sizeof(TICKET),
     TICKET,
#undef TICKET
     },   
     NULL
};

void init_test_cred()
{
     test_creds.client = (krb5_principal) malloc(sizeof(krb5_data *)*3);
     test_creds.client[0] = &client1;
     test_creds.client[1] = &client2;
     test_creds.client[2] = NULL;

     test_creds.server = (krb5_principal) malloc(sizeof(krb5_data *)*3);
     test_creds.server[0] = &server1;
     test_creds.server[1] = &server2;
     test_creds.server[2] = NULL;

     test_creds.addresses = (krb5_address **) calloc(3, sizeof(krb5_address *));
     test_creds.addresses[0] = &addr1; 
     test_creds.addresses[1] = &addr2; 
   
     test_creds.authdata = (krb5_authdata **) calloc(3, sizeof(krb5_authdata *));
     test_creds.authdata[0] = &authdata1; 
     test_creds.authdata[1] = &authdata2; 
}

#define CHECK(kret,msg) \
     if (BAD_STATUS(&kret)) {\
      printf("%s returned %d\n", msg, kret);\
     };
                        
void vmcc_test()
{
     krb5_ccache id;
     krb5_creds creds;
     error_status_t kret;
     krb5_cc_cursor cursor;

     init_test_cred();


     id = krb5_vmcc_get_ccache(&kret);
     CHECK(kret, "get_ccache1");
     kret = krb5_cc_initialize(id, test_creds.client);
     CHECK(kret, "initialize");
     kret = krb5_cc_store_cred(id, &test_creds);
     CHECK(kret, "store1");
     kret = krb5_cc_retrieve_cred(id, 0, &test_creds, &creds);
     CHECK(kret, "retrieve1");
     kret = krb5_cc_store_cred(id, &test_creds);
     CHECK(kret, "store2");
     kret = krb5_cc_retrieve_cred(id, 0, &test_creds, &creds);
     CHECK(kret, "retrieve2");

     kret = krb5_cc_start_seq_get(id, &cursor);
     CHECK(kret, "start_seq_get");
     kret = 0;
     while (kret != KRB5_CC_END) {
      printf("Calling next_cred\n");
      kret = krb5_cc_next_cred(id, &cursor, &creds);
      CHECK(kret, "next_cred");
     }

     kret = krb5_cc_end_seq_get(id, &cursor);
     CHECK(kret, "end_seq_get");

     kret = krb5_cc_destroy(id);
     CHECK(kret, "destroy");
     id = krb5_vmcc_get_ccache(&kret);
     CHECK(kret, "get_ccache2");
     kret = krb5_cc_close(id);
     CHECK(kret, "close");
}

#endif  /* SECD_VMCC_TESTING */

