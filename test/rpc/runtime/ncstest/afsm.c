/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: afsm.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:03  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:22  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:54:42  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:22:32  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  13:48:37  rsalz
 * 	 17-sep-91 labossiere    Use different names (prepended "_") on mgr routine
 * 	                         names to quiet compiler.
 * 	 22-aug-91 labossiere    pipe_t -> afstest_pipe_t
 * 	[1992/04/30  23:22:39  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:10  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 */
/*
 *  OSF DCE Version 1.0 
 */
/*
 * ncstest afs manager
 */


#include <ncstest.h>
#include <dce/id_base.h>
#include <afsu.h>

static error_status_t _ncstest_AFS_GetCtxtHdl ();
static error_status_t _ncstest_AFS_FreeCtxtHdl ();
static error_status_t _ncstest_AFS_GetTime ();
static error_status_t _ncstest_AFS_Symlink ();
static error_status_t _ncstest_AFS_FetchData ();
static error_status_t _ncstest_AFS_StoreData ();
static error_status_t _ncstest_AFS_BulkFetchVV ();

static ndr_byte pipe_buf[PIPE_BUF_SIZE];

#define VERBOSE(str) \
    { \
        globalref int ncstest_s_verbose; \
        if (ncstest_s_verbose) \
            printf("ncstest_afs_mgr: %s() %s\n", which_call, str); \
    }

#define ARG_FAILURE(which_arg) \
    { \
        failures++; \
        printf("ncstest_afs_mgr: %s() - bad '%s' arg received\n", \
                which_call, which_arg); \
    }

#define PIPE_FAILURE(str) \
    { \
        failures++; \
        printf("ncstest_afs_mgr: %s() - %s, totcount: %d\n", \
            which_call, str, totcount); \
    }

#define RETURN \
    if (failures == 0) \
        return error_status_ok; \
    else \
        return (0xffff0000 | failures)


/*
 * Format a pac into a static buffer.
 * The caller should acquire a mutex before calling this, but we're
 * being cheap at the moment.
 */

unsigned char *format_pac_contents(pac)
    sec_id_pac_t *pac;
{
    static unsigned char buf[1024];
    unsigned char *p = buf;
    int i;
    
    if (pac->authenticated)
        strcpy(p, "Authentic");
    else
        strcpy(p, "Unauthentic");

    strcat (p, " PAC: uid"); p += strlen(p); *p++ = ' ';

#ifndef _KERNEL
#  define SPRINT_DECIMAL(buf,n)    sprintf(buf, "%d", n) 
#else
#  define SPRINT_DECIMAL(buf,n)    sprintn(n, 10, 0, 0, buf)
#endif

    SPRINT_DECIMAL(p, pac->principal.uuid.time_low); p += strlen(p); *p++ = ' ';
    strcpy (p, "gid"); p += strlen(p); *p++ = ' ';
    SPRINT_DECIMAL(p, pac->group.uuid.time_low); p += strlen(p); *p++ = ' ';    
    if (pac->num_groups) {
        strcpy(p, "groups"); p += strlen(p); *p++ = ' ';
        for (i=0; i<pac->num_groups; i++) {
            SPRINT_DECIMAL(p, pac->groups[i].uuid.time_low); p += strlen(p); *p++ = ' ';
        }
    }
    *p++ = '\0';
    return buf;
}

/*
 * n c s t e s t _ A F S _ C h e c k C a l l e r
 *
 * This should be done under a mutex..
 */

error_status_t ncstest_AFS_CheckCaller (h)
    handle_t h;
{
    char *which_call = "ncstest_AFS_CheckCaller";
    int failures = 0;
    unsigned char *caller;
    extern unsigned char *rpc__stralloc();
    static unsigned char *last_caller = NULL;
    unsigned32 level, authn_svc, authz_svc;
    unsigned32 st;
    sec_id_pac_t *pac;
    int i;
    
    authn_svc = authz_svc = level = 0;
    caller = NULL;
        
    rpc_binding_inq_auth_client 
        (h, (rpc_authz_handle_t *) &caller, 0, &level, &authn_svc, &authz_svc, &st);
printf("binding_inq_auth_client returns st=%d\n", st);
    if (st == error_status_ok) {
        switch (authz_svc) {
        case rpc_c_authz_name:
            /* already in caller */
            break;
        case rpc_c_authz_dce:
            pac = (sec_id_pac_t *) caller;
            caller = format_pac_contents (pac);
            break;
        default:
            caller = (unsigned char *)"unknown authz svc";
        }
    } else if (st == rpc_s_binding_has_no_auth) {
        caller = (unsigned char *) "caller unauthenticated";
    } else {
        printf("%s(): code %x returned from inq_auth_client\n", which_call, st);
        failures++;
        RETURN;
    }
    if (caller == NULL) {
        printf("!Caller is NULL; authn_svc=%d, authz_svc=%d\n",
            authn_svc, authz_svc);
        failures++;
    } else if ((!last_caller) || (strcmp(caller, last_caller))) {
        unsigned32 tst;
        if (last_caller)
            rpc_string_free(&last_caller, &tst);
        printf("ncstest_afs_mgr: caller is now %s\n", caller);
        last_caller = rpc__stralloc (caller);
    }
    VERBOSE(caller);
    RETURN;
}



void AFS_ctxt_h_rundown
#ifdef NIDL_PROTOTYPES
    (rpc_ss_context_t context_handle)
#else
    (context_handle)
rpc_ss_context_t context_handle;
#endif
{
    printf("ncstest_afs_mgr: AFS_ctxt_h_rundown() invoked\n");
}


/*
 * n c s t e s t _ A F S _ G e t C t x t H d l
 */
static error_status_t _ncstest_AFS_GetCtxtHdl
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in] */ ndr_ulong_int initTime,
    /* [out] */ AFS_ctxt_h *ctxt_h
)
#else
    (h, initTime, ctxt_h)
handle_t h;
ndr_ulong_int initTime;
AFS_ctxt_h *ctxt_h;
#endif
{
    char *which_call = "ncstest_AFS_GetCtxtHdl";
    int failures = 0;
    unsigned32 st;
    
    VERBOSE("");
    
    st = ncstest_AFS_CheckCaller (h);
    if (st != error_status_ok)
        return st;

    ncstest_afstest_init(1);

    if (initTime != 40125)
        ARG_FAILURE("initTime");

    *ctxt_h = (AFS_ctxt_h)0xdeadbeef;

    RETURN;
}


/*
 * n c s t e s t _ A F S _ F r e e C t x t H d l
 */
static error_status_t _ncstest_AFS_FreeCtxtHdl
#ifdef NIDL_PROTOTYPES
(
    /* [in,out] */ AFS_ctxt_h *ctxt_h
)
#else
    (ctxt_h)
AFS_ctxt_h *ctxt_h;
#endif
{
    char *which_call = "ncstest_AFS_FreeCtxtHdl";
    int failures = 0;

    VERBOSE("");

    if (*ctxt_h != (AFS_ctxt_h)0xdeadbeef)
        ARG_FAILURE("ctxt_h");

    *ctxt_h = (AFS_ctxt_h)0;

    RETURN;
}


/*
 * n c s t e s t _ A F S _ G e t T i m e
 */
static error_status_t _ncstest_AFS_GetTime
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ AFS_ctxt_h ctxt_h,
    /* [out] */ ndr_ulong_int *Secondsp,
    /* [out] */ ndr_ulong_int *USecondsp
)
#else
    (ctxt_h, Secondsp, USecondsp)
AFS_ctxt_h ctxt_h;
ndr_ulong_int *Secondsp;
ndr_ulong_int *USecondsp;
#endif
{
    char *which_call = "ncstest_AFS_GetTime";
    int failures = 0;

    VERBOSE("");

    *Secondsp = 345987;
    *USecondsp = 984357;

    RETURN;
}


/*
 * n c s t e s t _ A F S _ S y m l i n k
 */
static error_status_t _ncstest_AFS_Symlink
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ AFS_ctxt_h ctxt_h,
    /* [in] */ afsFid *DirFidp,
    /* [in] */ NameString_t Namep,
    /* [in] */ PathString_t LinkContentsp,
    /* [in] */ afsStoreStatus *InStatusp,
    /* [in] */ afsHyper *minVVp,
    /* [in] */ ndr_ulong_int Flags,
    /* [out] */ afsFid *OutFidp,
    /* [out] */ afsFetchStatus *OutFidStatusp,
    /* [out] */ afsFetchStatus *OutDirStatusp,
    /* [out] */ afsToken *OutTokenp,
    /* [out] */ afsVolSync *Syncp
)
#else
    (ctxt_h, DirFidp, Namep, LinkContentsp, InStatusp, minVVp, Flags,
        OutFidp, OutFidStatusp, OutDirStatusp, OutTokenp, Syncp)
AFS_ctxt_h ctxt_h;
afsFid *DirFidp;
NameString_t Namep;
PathString_t LinkContentsp;
afsStoreStatus *InStatusp;
afsHyper *minVVp;
ndr_ulong_int Flags;
afsFid *OutFidp;
afsFetchStatus *OutFidStatusp;
afsFetchStatus *OutDirStatusp;
afsToken *OutTokenp;
afsVolSync *Syncp;
#endif
{
    char *which_call = "ncstest_AFS_Symlink";
    int failures = 0;

    VERBOSE("");

    if (ncstest_afsFid_diff(&ncstest_afsFid_arg, DirFidp))
        ARG_FAILURE("DirFidp");
    if (ncstest_NameString_diff(ncstest_NameString_arg, Namep))
        ARG_FAILURE("Namep");
    if (ncstest_PathString_diff(ncstest_PathString_arg, LinkContentsp))
        ARG_FAILURE("Linkcontentsp");
    if (ncstest_afsStoreStatus_diff(&ncstest_afsStoreStatus_arg, InStatusp))
        ARG_FAILURE("InStatusp");
    if (ncstest_afsHyper_diff(&ncstest_afsHyper_arg, minVVp))
        ARG_FAILURE("minVVp");
    if (Flags != 143139872)
        ARG_FAILURE("Flags");

    *OutFidp = ncstest_afsFid_arg;
    *OutFidStatusp = ncstest_afsFetchStatus_arg;
    *OutDirStatusp = ncstest_afsFetchStatus_arg;
    *OutTokenp = ncstest_afsToken_arg;
    *Syncp = ncstest_afsVolSync_arg;

    RETURN;
}


/*
 * n c s t e s t _ A F S _ F e t c h D a t a
 */
static error_status_t _ncstest_AFS_FetchData
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ AFS_ctxt_h ctxt_h,
    /* [in] */ afsFid *Fidp,
    /* [in] */ afsHyper *minVVp,
    /* [in] */ ndr_long_int Position,
    /* [in] */ ndr_long_int Length,
    /* [in] */ ndr_ulong_int Flags,
    /* [out] */ afsFetchStatus *OutStatusp,
    /* [out] */ afsToken *OutTokenp,
    /* [out] */ afsVolSync *Syncp,
    /* [out] */ afstest_pipe_t *fetchStream
)
#else
    (ctxt_h, Fidp, minVVp, Position, Length, Flags, 
        OutStatusp, OutTokenp, Syncp, fetchStream)
AFS_ctxt_h ctxt_h;
afsFid *Fidp;
afsHyper *minVVp;
ndr_long_int Position;
ndr_long_int Length;
ndr_ulong_int Flags;
afsFetchStatus *OutStatusp;
afsToken *OutTokenp;
afsVolSync *Syncp;
afstest_pipe_t *fetchStream;
#endif
{
    char *which_call = "ncstest_AFS_FetchData";
    int failures = 0;
    volatile long totcount = 0;
    long resid, pushcnt;

    VERBOSE("");

    if (ncstest_afsFid_diff(&ncstest_afsFid_arg, Fidp))
        ARG_FAILURE("Fidp");
    if (ncstest_afsHyper_diff(&ncstest_afsHyper_arg, minVVp))
        ARG_FAILURE("minVVp");
    if (Position != 187446)
        ARG_FAILURE("Position");
    if (Length != 1818637)
        ARG_FAILURE("Length");
    if (Flags != 143139872)
        ARG_FAILURE("Flags");

    TRY {
        totcount = 0;
        while (totcount < PIPE_LENGTH) {
            VERBOSE("calling pipe push");
            resid = PIPE_LENGTH - totcount;
            pushcnt = PIPE_BUF_SIZE < resid ? PIPE_BUF_SIZE : resid;
            totcount += pushcnt;
            (*(fetchStream->push)) (fetchStream->state, pipe_buf, pushcnt);
        };
        VERBOSE("calling pipe push");
        (*(fetchStream->push)) (fetchStream->state, pipe_buf, 0);
    } CATCH_ALL {
        PIPE_FAILURE("unexpected exception while pushing");
    } ENDTRY

    if (totcount != PIPE_LENGTH)
        PIPE_FAILURE("incorrect push length");

    *OutStatusp = ncstest_afsFetchStatus_arg;
    *OutTokenp = ncstest_afsToken_arg;
    *Syncp = ncstest_afsVolSync_arg;

    VERBOSE("completed");

    RETURN;
}


/*
 * n c s t e s t _ A F S _ S t o r e D a t a
 */
static error_status_t _ncstest_AFS_StoreData
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ AFS_ctxt_h ctxt_h,
    /* [in] */ afsFid *Fidp,
    /* [in] */ afsStoreStatus *InStatusp,
    /* [in] */ ndr_long_int Position,
    /* [in] */ ndr_long_int Length,
    /* [in] */ ndr_long_int FileLength,
    /* [in] */ afsHyper *minVVp,
    /* [in] */ ndr_ulong_int Flags,
    /* [in] */ afstest_pipe_t *storeStream,
    /* [out] */ afsFetchStatus *OutStatusp,
    /* [out] */ afsVolSync *Syncp
)
#else
    (ctxt_h, Fidp, InStatusp, Position, Length, FileLength, minVVp, Flags, 
        storeStream, OutStatusp, Syncp)
AFS_ctxt_h ctxt_h;
afsFid *Fidp;
afsStoreStatus *InStatusp;
ndr_long_int Position;
ndr_long_int Length;
ndr_long_int FileLength;
afsHyper *minVVp;
ndr_ulong_int Flags;
afstest_pipe_t *storeStream;
afsFetchStatus *OutStatusp;
afsVolSync *Syncp;
#endif
{
    char *which_call = "ncstest_AFS_StoreData";
    int failures = 0;
    volatile long totcount = 0;
    unsigned long count;

    VERBOSE("");

    if (ncstest_afsFid_diff(&ncstest_afsFid_arg, Fidp))
        ARG_FAILURE("Fidp");
    if (ncstest_afsStoreStatus_diff(&ncstest_afsStoreStatus_arg, InStatusp))
        ARG_FAILURE("InStatusp");
    if (Position != 187446)
        ARG_FAILURE("Position");
    if (Length != 1818637)
        ARG_FAILURE("Length");
    if (FileLength != 113809)
        ARG_FAILURE("FileLength");
    if (ncstest_afsHyper_diff(&ncstest_afsHyper_arg, minVVp))
        ARG_FAILURE("minVVp");
    if (Flags != 143139872)
        ARG_FAILURE("Flags");

    TRY {
        do {
            VERBOSE("calling pipe pull");
            (*(storeStream->pull)) (storeStream->state, pipe_buf, 
                                    sizeof pipe_buf, &count);
            totcount += count;
        } while (count != 0);
    } CATCH_ALL {
        PIPE_FAILURE("unexpected exception while pulling");
    } ENDTRY

    if (totcount != PIPE_LENGTH)
        PIPE_FAILURE("incorrect pull length");

    *OutStatusp = ncstest_afsFetchStatus_arg;
    *Syncp = ncstest_afsVolSync_arg;

    VERBOSE("completed");

    RETURN;
}


/*
 * n c s t e s t _ A F S _ B u l k F e t c h V V
 */
static error_status_t _ncstest_AFS_BulkFetchVV
#ifdef NIDL_PROTOTYPES
(
    /* [in] */ AFS_ctxt_h ctxt_h,
    /* [in] */ afsHyper *cellIdp,
    /* [in] */ afsBulkVolIDs *VolIDsp,
    /* [in] */ ndr_ulong_int NumVols,
    /* [in] */ ndr_ulong_int Flags,
    /* [in] */ ndr_ulong_int spare1,
    /* [in] */ ndr_ulong_int spare2,
    /* [out] */ afsBulkVVs *VolVVsp,
    /* [out] */ ndr_ulong_int *spare4
)
#else
    (ctxt_h, cellIdp, VolIDsp, NumVols, Flags, spare1, spare2, VolVVsp, spare4)
AFS_ctxt_h ctxt_h;
afsHyper *cellIdp;
afsBulkVolIDs *VolIDsp;
ndr_ulong_int NumVols;
ndr_ulong_int Flags;
ndr_ulong_int spare1;
ndr_ulong_int spare2;
afsBulkVVs *VolVVsp;
ndr_ulong_int *spare4;
#endif
{
    char *which_call = "ncstest_AFS_BulkFetchVV";
    int i, failures = 0;

    VERBOSE("");

    if (ncstest_afsHyper_diff(&ncstest_afsHyper_arg, cellIdp))
        ARG_FAILURE("cellIdp");
    if (ncstest_afsBulkVolIDs_diff(&ncstest_afsBulkVolIDs_arg, VolIDsp))
        ARG_FAILURE("VolIDsp");
    if (NumVols != 209785)
        ARG_FAILURE("NumVols");
    if (Flags != 7825672)
        ARG_FAILURE("Flags");
    if (spare1 != 1679237)
        ARG_FAILURE("spare1");
    if (spare2 != 6749319)
        ARG_FAILURE("spare2");

    ncstest_afs_BulkVVs_3_init(VolVVsp);

    *spare4 = 245795;

    VERBOSE("completed");

    RETURN;
}



globaldef AFS4Int_v3_0_epv_t ncstest_afs_manager_epv = {
    _ncstest_AFS_GetCtxtHdl,
    _ncstest_AFS_FreeCtxtHdl,
    _ncstest_AFS_GetTime,
    _ncstest_AFS_Symlink,
    _ncstest_AFS_FetchData,
    _ncstest_AFS_StoreData,
    _ncstest_AFS_BulkFetchVV     
};
