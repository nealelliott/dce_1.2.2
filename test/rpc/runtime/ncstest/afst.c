/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: afst.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:36  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:04  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:23  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:54:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:22:38  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  13:48:43  rsalz
 * 	 22-aug-91 labossiere    pipe_t -> afstest_pipe_t
 * 	 05-aug-91 labossiere    mods for -e switch
 * 	[1992/04/30  23:22:46  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:11  devrcs
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
 * ncstest afs client
 *
 * test ops that use misc features (see afs.idl):
 *      context_handle
 *      [in], [out] of scalars and structs by reference
 *      [in], [out] of nested large structures
 *      error_status_t function return values
 *      max_is - for arrays of structs (not for strings)
 *      string - e.g. typedef [string] char 	NameString_t[AFS_NAMEMAX];
 *      pipe of bytes
 *      ref pointers
 */


#include <ncstest.h>
#include <afsu.h>

/* a little something that we can twiddle from the debugger */
#define TEST_EVERYTHING     0xffffffff
#define TEST_GETTIME        0x00000001
#define TEST_SYMLINK        0x00000002
#define TEST_STOREDATA      0x00000004
#define TEST_FETCHDATA      0x00000008
#define TEST_BULKFETCHVV    0x00000010

#define DO_TEST(tid)  (ncstest_afs_clt_mask & (tid))

unsigned long int ncstest_afs_clt_mask = TEST_EVERYTHING;

globalref int ncstest_c_eflag;

#define VERBOSE(str) \
    { \
        globalref int ncstest_c_verbose; \
        if (ncstest_c_verbose) \
            printf("ncstest_afs_clt: %s() %s\n", which_call, str); \
    }

#define FAILURE(str) \
    { \
        failures++; \
        printf("ncstest_afs_clt: %s\n", str); \
    }

#define CALL_FAILURE \
    { \
        failures++; \
        if ((res & 0xffff0000) == 0xffff0000) \
            printf("ncstest_afs_clt: %s() - server side had %d failures\n", \
                which_call, res & 0x0000ffff); \
        else \
            printf("ncstest_afs_clt: %s() - bad return status (%x)\n", \
                which_call, res); \
    }

#define ARG_FAILURE(which_arg) \
    { \
        failures++; \
        printf("ncstest_afs_clt: %s() - bad '%s' arg returned\n", \
                which_call, which_arg); \
    }

#define PIPE_FAILURE(str) \
    { \
        failures++; \
        printf("ncstest_afs_mgr: %s() - %s, totcount: %d\n", \
            which_call, str, ps->totcount); \
    }

#ifndef _KERNEL
#define TESTCANCEL \
    TRY  { \
        pthread_testcancel(); \
    } CATCH_ALL { \
        printf("cancel received...\n"); \
        RERAISE; \
    } ENDTRY 
#else
#define TESTCANCEL {}
#endif

static afsFid           out_afsFid_1;
static afsFetchStatus   out_afsFetchStatus_1, out_afsFetchStatus_2;
static afsToken         out_afsToken_1;
static afsVolSync       out_afsVolSync_1;
static afsBulkVVs       out_afsBulkVVs_1;
static unsigned long    out_spare_4;

static ndr_byte pipe_buf[PIPE_BUF_SIZE];
typedef struct {
    ndr_byte *alloced_buf;
    long length;
    long totcount;
} pipe_state_t;

static void pipe_push
#ifdef NIDL_PROTOTYPES
    (rpc_ss_pipe_state_t state, ndr_byte *chunk_array, ndr_ulong_int  pipe_chunk_size)
#else
    (state,chunk_array,pipe_chunk_size)
rpc_ss_pipe_state_t state;
ndr_byte *chunk_array;
ndr_ulong_int  pipe_chunk_size;
#endif
{
    char *which_call = "pipe_push";
    int failures = 0;
    pipe_state_t    *ps = (pipe_state_t *) state;

    VERBOSE("");

    if (pipe_chunk_size == 0) {
        VERBOSE("end of pipe");
        return;
    }

    ps->totcount += pipe_chunk_size;
}


static void pipe_pull
#ifdef NIDL_PROTOTYPES
    (rpc_ss_pipe_state_t state, ndr_byte *chunk_array, ndr_ulong_int esize, ndr_ulong_int *ecount)
#else
    (state,chunk_array,esize,ecount)
rpc_ss_pipe_state_t state;
ndr_byte *chunk_array;
ndr_ulong_int esize;
ndr_ulong_int *ecount;
#endif
{
    char *which_call = "pipe_pull";
    int failures = 0;
    pipe_state_t    *ps = (pipe_state_t *) state;

    VERBOSE("");

    if (ps->alloced_buf != chunk_array)
        PIPE_FAILURE("pull / alloc error\n");

    if (ps->totcount == ps->length) {
        VERBOSE("end of pipe");
        *ecount = 0;
    } else {
        long resid;
        resid = ps->length - ps->totcount;
        *ecount = esize < resid ? esize : resid;
        ps->totcount += *ecount;
    }
}


static void pipe_alloc
#ifdef NIDL_PROTOTYPES
    (rpc_ss_pipe_state_t state, ndr_ulong_int bsize, ndr_byte **buf, ndr_ulong_int *bcount)
#else
    (state, bsize, buf, bcount)
rpc_ss_pipe_state_t state;
ndr_ulong_int bsize;
ndr_byte **buf;
ndr_ulong_int *bcount;
#endif
{
    char *which_call = "pipe_alloc";
    pipe_state_t    *s = (pipe_state_t *) state;

    VERBOSE("");

    s->alloced_buf = pipe_buf;

    *buf = pipe_buf;
    *bcount = sizeof pipe_buf;
}

int ncstest_afs_clt(h)
rpc_binding_handle_t h;
{
    int failures = 0;
    error_status_t  res;
    AFS_ctxt_h      ctx = (AFS_ctxt_h) 0;
    volatile char   *which_call;
    unsigned long   Sec, Usec;
    unsigned long   spare_1;
    afstest_pipe_t  pipe;
    pipe_state_t    pipe_state, *ps = &pipe_state;

    ncstest_afstest_init(0);

    TRY {
        /* ============================================= */
        which_call = "ncstest_AFS_GetCtxtHdl";
        VERBOSE("happening now...");
        VERBOSE("may cause exception due to DG runtime bug (bl09.12)");
        res = ncstest_AFS_GetCtxtHdl(h, 40125, &ctx);
        VERBOSE("completed");
        if (res != error_status_ok) {
            CALL_FAILURE;
            goto GiveUp;
        }
        if (ctx == 0) {
            ARG_FAILURE("ctx");
            goto GiveUp;
        }
        TESTCANCEL;

        if (DO_TEST(TEST_GETTIME))
        {
        /* ============================================= */
        which_call = "ncstest_AFS_GetTime";
        VERBOSE("happening now...");
        res = ncstest_AFS_GetTime(ctx, &Sec, &Usec);
        VERBOSE("completed");
        if (res != error_status_ok) {
            CALL_FAILURE;
            if (ncstest_c_eflag)
                goto GiveUp;
        } else {
            if (Sec != 345987)
                ARG_FAILURE("Sec");
            if (Usec != 984357)
                ARG_FAILURE("Usec");
        }
        TESTCANCEL;
        }

        if (DO_TEST(TEST_SYMLINK))
        {
        /* ============================================= */
        which_call = "ncstest_AFS_Symlink";
        VERBOSE("happening now...");
        res = ncstest_AFS_Symlink(ctx, &ncstest_afsFid_arg,
                    ncstest_NameString_arg, ncstest_PathString_arg,
                    &ncstest_afsStoreStatus_arg, &ncstest_afsHyper_arg,
                    143139872, 
                    &out_afsFid_1, 
                    &out_afsFetchStatus_1, &out_afsFetchStatus_2,
                    &out_afsToken_1, &out_afsVolSync_1);
        VERBOSE("completed");
        if (res != error_status_ok) {
            CALL_FAILURE;
            if (ncstest_c_eflag)
                goto GiveUp;
        } else {
            if (ncstest_afsFid_diff(&ncstest_afsFid_arg, &out_afsFid_1))
                ARG_FAILURE("out_afsFid_1");
            if (ncstest_afsFetchStatus_diff(&ncstest_afsFetchStatus_arg, 
                    &out_afsFetchStatus_1))
                ARG_FAILURE("out_afsFetchStatus_1");
            if (ncstest_afsFetchStatus_diff(&ncstest_afsFetchStatus_arg, 
                    &out_afsFetchStatus_2))
                ARG_FAILURE("out_afsFetchStatus_2");
            if (ncstest_afsToken_diff(&ncstest_afsToken_arg, 
                    &out_afsToken_1))
                ARG_FAILURE("out_afsToken_1");
            if (ncstest_afsVolSync_diff(&ncstest_afsVolSync_arg, 
                    &out_afsVolSync_1))
                ARG_FAILURE("out_afsVolSync_1");
        }
        TESTCANCEL;
        }

        if (DO_TEST(TEST_STOREDATA))
        {
        /* ============================================= */
        which_call = "ncstest_AFS_StoreData";
        ps->alloced_buf = (ndr_byte *)0;
        ps->length = PIPE_LENGTH;
        ps->totcount = 0;
        pipe.alloc = pipe_alloc;
        pipe.pull = pipe_pull;
        pipe.push = 0;
        pipe.state = (char *)ps;
        VERBOSE("happening now...");
        res = ncstest_AFS_StoreData(ctx, &ncstest_afsFid_arg,
                    &ncstest_afsStoreStatus_arg,
                    187446, 1818637, 113809,
                    &ncstest_afsHyper_arg,
                    143139872, 
                    &pipe,
                    &out_afsFetchStatus_1,
                    &out_afsVolSync_1);
        VERBOSE("completed");
        if (res != error_status_ok) {
            CALL_FAILURE;
            if (pipe_state.totcount != ps->length)
                PIPE_FAILURE("incorrect length pulled");
            if (ncstest_c_eflag)
                goto GiveUp;
        } else {
            if (pipe_state.totcount != ps->length)
                PIPE_FAILURE("incorrect pull length");
            if (ncstest_afsFetchStatus_diff(&ncstest_afsFetchStatus_arg, 
                    &out_afsFetchStatus_1))
                ARG_FAILURE("out_afsFetchStatus_1");
            if (ncstest_afsVolSync_diff(&ncstest_afsVolSync_arg, 
                    &out_afsVolSync_1))
                ARG_FAILURE("out_afsVolSync_1");
        }
        TESTCANCEL;
        }

        if (DO_TEST(TEST_FETCHDATA))
        {
        /* ============================================= */
        which_call = "ncstest_AFS_FetchData";
        ps->length = PIPE_LENGTH;
        ps->totcount = 0;
        pipe.push = pipe_push;
        pipe.alloc = pipe_alloc;
        pipe.pull = 0;
        pipe.state = (char *)ps;
        VERBOSE("happening now...");
        res = ncstest_AFS_FetchData(ctx, &ncstest_afsFid_arg,
                    &ncstest_afsHyper_arg,
                    187446, 1818637, 143139872,
                    &out_afsFetchStatus_1,
                    &out_afsToken_1,
                    &out_afsVolSync_1,
                    &pipe);
        VERBOSE("completed");
        if (res != error_status_ok) {
            CALL_FAILURE;
            if (pipe_state.totcount != ps->length)
                PIPE_FAILURE("incorrect push length");
            if (ncstest_c_eflag)
                goto GiveUp;
        } else {
            if (pipe_state.totcount != ps->length)
                PIPE_FAILURE("incorrect push length");
            if (ncstest_afsFetchStatus_diff(&ncstest_afsFetchStatus_arg, 
                    &out_afsFetchStatus_1))
                ARG_FAILURE("out_afsFetchStatus_1");
            if (ncstest_afsToken_diff(&ncstest_afsToken_arg, 
                    &out_afsToken_1))
                ARG_FAILURE("out_afsToken_1");
            if (ncstest_afsVolSync_diff(&ncstest_afsVolSync_arg, 
                    &out_afsVolSync_1))
                ARG_FAILURE("out_afsVolSync_1");
        }
        TESTCANCEL;
        }

        if (DO_TEST(TEST_BULKFETCHVV))
        {
        /* ============================================= */
        which_call = "ncstest_AFS_BulkFetchVV";
        VERBOSE("happening now...");
        res = ncstest_AFS_BulkFetchVV(ctx, 
                    &ncstest_afsHyper_arg, 
                    &ncstest_afsBulkVolIDs_arg, 
                    209785, 7825672, 1679237, 6749319,
                    &out_afsBulkVVs_1, 
                    &out_spare_4);
        VERBOSE("completed");
        if (res != error_status_ok) {
            CALL_FAILURE;
            if (ncstest_c_eflag)
                goto GiveUp;
        } else {
            extern void rpc_osc_free();

            if (ncstest_afsBulkVVs_diff(&ncstest_afsBulkVVs_arg, 
                    &out_afsBulkVVs_1))
                ARG_FAILURE("out_afsBulkVVs_1");
            if (out_spare_4 != 245795)
                ARG_FAILURE("out_spare_4");
        }
        TESTCANCEL;
        }
GiveUp:
        /* ============================================= */
        if (ctx != 0) {
            which_call = "ncstest_AFS_FreeCtxtHdl";
            VERBOSE("happening now...");
            res = ncstest_AFS_FreeCtxtHdl(&ctx);
            VERBOSE("completed");
            if (res != error_status_ok) {
                CALL_FAILURE;
            } else {
                if (ctx != 0)
                    ARG_FAILURE("ctx");
            }
        }
    } CATCH_ALL {
        printf("ncstest_afs_clt: unexpected exception during %s()\n",
            which_call);
        failures++;
    } ENDTRY

    return failures;
}
