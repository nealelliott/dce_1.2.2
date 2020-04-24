/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: v2test.h,v $
 * Revision 1.1.122.2  1996/02/18  23:07:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:03  marty]
 *
 * Revision 1.1.122.1  1995/12/11  20:15:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:31  root]
 * 
 * Revision 1.1.119.1  1994/01/21  22:40:39  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:17:49  cbrooks]
 * 
 * Revision 1.1.10.1  1993/10/15  17:00:33  tom
 * 	Bug 8303 - Move definition of timers to v2client to avoid multiple
 * 	declaration in v2test.h.
 * 	Remove dead apollo code.
 * 	[1993/10/15  16:21:04  tom]
 * 
 * Revision 1.1.8.2  1993/08/12  16:07:48  kissel
 * 	OT 8342 - Fix ioctl() parameter for everybody ('N' vs. N), just like
 * 	test/rpc/runtime/ncstest/ncstest.h.
 * 	[1993/08/11  19:22:00  kissel]
 * 
 * Revision 1.1.5.3  1993/01/12  00:03:38  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:26:47  bbelch]
 * 
 * Revision 1.1.5.2  1992/09/29  21:24:34  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:49:09  weisman]
 * 
 * Revision 1.1.2.3  1992/05/11  16:53:16  rsalz
 * 	Fix ioctl() parameter for AIX; needs to be 'N' not N.
 * 	[1992/05/11  15:13:14  rsalz]
 * 
 * Revision 1.1.2.2  1992/05/01  15:42:45  rsalz
 * 	Part of rpc6 code drop.
 * 	[1992/05/01  01:30:42  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:42:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      v2test.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Common header file for v2client & v2server.
**
**
*/

/*
 * Parts of the code in this module get used in 3 different contexts:
 *
 * (1) Standard user-space v2test.
 * (2) Driver for in-kernel v2test.
 * (3) In-kernel v2test.
 *
 * KERNEL_TEST_DRIVER is #define'd when this module is being built for
 * context (2).  _KERNEL is #define'd for context (3).  Nothing in
 * particular is #define'd for context (1).
 */

/*****************************************************************************/ 

#include <dce/rpc.h>

#ifndef _KERNEL           /* for standard user mode v2test */
#  include <math.h>
#  include <sys/types.h> 
#  include <sys/file.h>  
#  include <stdio.h>
#  include <sys/time.h>
#  ifndef KERNEL_TEST_DRIVER
#     include <pthread.h>
#  endif
#  ifdef KERNEL_TEST_DRIVER
#    include <sys/ioctl.h>  /* for user mode side of in-kernel v2test */
#    define NCSDEVICE  "/dev/ncs"
#  endif
#else
#  include <sys/time.h>
#  include <sys/ioctl.h>
#  define PTHREAD_NO_TIMESPEC
#  include <dce/ker/pthread.h>
#endif

#ifndef NCK
#  define NCK
#endif
#include <dce/stubbase.h>


#ifdef _KERNEL
/*
 * Redefine malloc and free so that we use something 
 * appropriate in a kernel environment.
 */
#include <dce/ker/idl_ss_krpc.h>
#define malloc(n)   idl_malloc_krpc(n)
#define free(p)     idl_free_krpc(p)

/*
 * Redefine gettimeofday to something useful (gettimeofday is typically
 * the kernel syscall entry point... not what we want).
 */
#define gettimeofday    nck_gettimeofday
#endif


/*****************************************************************************/ 

extern uuid_t uuid_g_nil_uuid;

/*****************************************************************************/ 

static rpc_syntax_id_t  v2test_syntax[]=
{
    {
        {0x8a885d04, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x08, 0x00, 0x02b, 0x10, 0x048, 0x60}},
        0,
    }
};

#define V2TEST_IF_REP_INIT(s_epv) \
{ \
    1, 5, 0, \
    {0x48c86bc8, 0xe000, 0xd00, 0x1, 0x8e, {0xd9, 0x0, 0x0, 0x0, 0x0, 0x0}}, \
    2, \
    {0, NULL}, \
    {1, v2test_syntax}, \
    (rpc_v2_server_stub_epv_t) s_epv, \
    (rpc_mgr_epv_t) 0xdeadbeef \
}

#define CALLBACK_IF_REP_INIT(s_epv) \
{ \
    1, 1, 0, \
    {0x58c86bc8, 0xe000, 0xd00, 0x1, 0x8e, {0xd9, 0x0, 0x0, 0x0, 0x0, 0x0}}, \
    2, \
    {0, NULL}, \
    {0, NULL}, \
    (rpc_v2_server_stub_epv_t) s_epv, \
    (rpc_mgr_epv_t) 0xdeadbeef \
}

/*****************************************************************************/ 

/*
 * Use a linked-list to queue up packets received from the runtime.
 */
typedef struct iovec_ll_elt_t {
    struct iovec_ll_elt_t       *next;
    rpc_iovector_elt_t          pkt;
} iovec_ll_elt_t;

/*****************************************************************************/ 

#ifndef _KERNEL

#define CHECK_STATUS(st, mesg) \
     if (*(st)) { \
        printf("%s; status %lx (file: %s, line: %d)\n", \
                mesg, *(st), __FILE__, __LINE__); \
        return; \
    }

#define CHECK_POINTER(p) \
    if ((p) == NULL) { \
        printf("Can't alloc heap storage (file: %s, line: %d)\n", \
                __FILE__, __LINE__); \
        abort(); \
    }

#else

#define CHECK_STATUS(st, mesg) \
    if (*(st)) { \
        printf(mesg, *(st)); \
        printf("%s; status %lx (file: %s, line: %d)\nnck resources likely stranded...\n", \
                mesg, *(st), __FILE__, __LINE__); \
        return; \
    }

#define CHECK_POINTER(p) \
    if ((p) == NULL) { \
        printf("Can't alloc heap storage (file: %s, line: %d)\nnck resources likely stranded...\n", \
                __FILE__, __LINE__); \
        abort(); \
    }

#endif

/*****************************************************************************/ 

/*
 * The following timer stuff is included until v2timer.h becomes stable.
 */
     
#define CALL_TIME   0
#define XMIT_TIME   1
#define RECV_TIME   2

#ifdef SNI_SVR4
#define BEGIN_TIMING(index)  gettimeofday(&timers[index].timer); 
#else
#define BEGIN_TIMING(index)  gettimeofday(&timers[index].timer, NULL); 
#endif

#define END_TIMING(index)    end_timing(&timers[index])

/*****************************************************************************/ 

/*
 * v2s/v2c_execute structures and definitions.
 */

#define V2_MAX_DBGSTR   20
#define V2_MAX_SNAME    128
#define V2_MAX_ANAME    128     /* authentication name */
#define V2_MAX_PROTSEQ  5
#define V2_MAX_IDENTS   5
/*****************************************************************************/ 

/*
 * v2client arguments
 */

typedef enum {NULL_TEST, INS_TEST, OUTS_TEST, INS_OUTS_TEST} v2c_op_t;

typedef struct {
    char            dbgstr[V2_MAX_DBGSTR];
    char            sname[V2_MAX_SNAME];
    v2c_op_t        op_num;
    int             call_opts;  /* rpc_call_opts_t */
    int             num_calls;
    int             num_buffs;
    int             buff_size;
    unsigned32      alevel;
    unsigned32      aprot;
    unsigned32      azprot;
    unsigned char   aname[V2_MAX_ANAME];
    int             break_every;
} v2c_inargs_t;

typedef struct {
    unsigned32  st;
} v2c_outargs_t;

typedef union
{
    v2c_inargs_t    in;
    v2c_outargs_t   out;
} v2c_args_t;

/*****************************************************************************/ 

/*
 * v2server arguments
 */

typedef struct
{
    char                s[20];
} rpc_protseq_array_t;


typedef struct {
    char            dbgstr[V2_MAX_DBGSTR];
    int             num_threads;
    boolean         do_callback;
    boolean         ep_register;
    int             num_protseq;
    rpc_protseq_array_t   protseq[V2_MAX_PROTSEQ];
    struct {
        unsigned32      aprot;
        unsigned char   aname[V2_MAX_ANAME];
    } idents[V2_MAX_IDENTS];
    int             break_every;
} v2s_inargs_t;

typedef struct {
    unsigned32  st;
} v2s_outargs_t;

typedef union
{
    v2s_inargs_t    in;
    v2s_outargs_t   out;
} v2s_args_t;

/*****************************************************************************/ 

/*
 * NCS device test jig operations for v2test (see ncsdev.h).
 */

#if defined(_KERNEL) || defined(KERNEL_TEST_DRIVER)
#undef NIOC
#  ifdef OLD_STYLE_IOWR
#    define NIOC    N
#  else
#    define NIOC    'N'
#  endif

#  define NIOCV2C           _IOWR(NIOC, 0, v2c_args_t)     /* run v2client */
#  define NIOCV2S           _IOWR(NIOC, 1, v2s_args_t)     /* run v2server */
#endif

/*****************************************************************************/ 

#ifdef CRC_16
typedef unsigned short crc_bits;
#define NBITS 16
#else
typedef unsigned long crc_bits;
#define NBITS 32
#endif

extern void crc_init
    _DCE_PROTOTYPE_((
        void
    ));

extern crc_bits crc
    _DCE_PROTOTYPE_((
        unsigned char *buf,
        int len
    ));

/*****************************************************************************/ 

