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
 * $Log: v2client.c,v $
 * Revision 1.1.840.2  1996/02/17  23:24:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:26  marty]
 *
 * Revision 1.1.840.1  1995/12/11  20:15:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:27  root]
 * 
 * Revision 1.1.837.1  1994/01/21  22:40:32  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:17:28  cbrooks]
 * 
 * Revision 1.1.7.1  1993/10/15  17:00:31  tom
 * 	Bug 8303 - Move definition of timers here to avoid multiple
 * 	declaration in v2test.h.
 * 	[1993/10/15  16:20:59  tom]
 * 
 * Revision 1.1.5.4  1993/01/21  16:21:47  weisman
 * 	Fix for OT 6296: moved call to rpc_network_maintain_liveness to
 * 	immediately follow rpc_call_start.
 * 	[1993/01/20  20:48:27  weisman]
 * 
 * Revision 1.1.5.3  1993/01/12  00:03:22  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:26:21  bbelch]
 * 
 * Revision 1.1.5.2  1992/09/29  21:24:25  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:49:04  weisman]
 * 
 * Revision 1.1.2.4  1992/05/08  20:58:36  rsalz
 * 	Add missing PROTOTYPE for end_timer.
 * 	[1992/05/08  20:41:09  rsalz]
 * 
 * Revision 1.1.2.3  1992/05/05  20:10:34  mishkin
 * 	Remove prototype for main; didn't work on AIX and who needs it anyway?
 * 	[1992/05/05  13:47:11  mishkin]
 * 
 * Revision 1.1.2.2  1992/05/01  15:42:32  rsalz
 * 	Part of rpc6 code drop.
 * 	[1992/05/01  01:30:29  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:42:00  devrcs
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
**      v2client.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Client application code for system exerciser.
**
**
*/

#include <v2test.h>

struct timer {
    unsigned32 count;
    unsigned32 total_us;
    unsigned32 started;
    struct timeval timer;
} timers[3];


/*****************************************************************************/ 

/*
 * Parts of the code in this module get used in 3 different context:
 *
 * (1) Standard user-space v2test client.
 * (2) Driver for in-kernel v2test client.
 * (3) In-kernel v2test client.
 *
 * KERNEL_TEST_DRIVER is #define'd when this module is being built for
 * context (2).  _KERNEL is #define'd for context (3).  Nothing in
 * particular is #define'd for context (1).
 */

/*****************************************************************************/ 

static boolean debug = false;

/*****************************************************************************/

#ifndef KERNEL_TEST_DRIVER

static struct timespec delay60sec = {60, 0};
static struct timespec delay5sec =  {5, 0};

#endif

/*****************************************************************************/

#ifndef KERNEL_TEST_DRIVER

extern void callback_routine
    _DCE_PROTOTYPE_((
        handle_t h, 
        rpc_call_handle_t call_h,
        rpc_iovector_elt_p_t iovec_elt_ins,
        ndr_format_p_t ndr_format, 
        rpc_transfer_syntax_p_t transfer_syntax,
        rpc_mgr_epv_t epv,
        unsigned32 *st
    ));

static rpc_v2_server_stub_proc_t callback_list[] = { 
    callback_routine 
};

/*
 * Client specific callback and client interface representations.  The
 * name of these structures must be different from the names of equivalent
 * server structures.  We must not have name conflicts in the kernel
 * were the client and server are part of the same executable image.
 */ 


static rpc_if_rep_t callback_if_c = CALLBACK_IF_REP_INIT(callback_list);

static rpc_if_rep_t v2test_if_c = V2TEST_IF_REP_INIT(NULL);

#endif

/*****************************************************************************/

#ifdef KERNEL_TEST_DRIVER

extern v2c_make_kernel_doit
    _DCE_PROTOTYPE_((
        v2c_inargs_t    *inargs,
        v2c_outargs_t   *outargs
    ));

#else

extern v2c_doit
    _DCE_PROTOTYPE_((
        v2c_inargs_t    *inargs,
        v2c_outargs_t   *outargs
    ));

extern handle_t bind_to_name
    _DCE_PROTOTYPE_((
        char *name
    ));

#endif

static end_timing
    _DCE_PROTOTYPE_((
	struct timer *timer
    ));

/*****************************************************************************/

/*
 * U S A G E
 */

static void usage()
{
    printf("usage: v2client [-D] [-d <debug switches>]\n");
    printf("                [-p <authn proto>,<authz proto>,<level>,<principal>]\n");
    printf("                i|o|io|n <string binding> <num calls> <num buffs> <buff_size>\n");
    printf("                <call opts>\n");
    exit(-1);
}

/*****************************************************************************/ 

/*
 * M A I N
 */

#ifndef _KERNEL

main(argc, argv) 
int argc;
char *argv[];
{
    v2c_inargs_t inargs;
    v2c_outargs_t outargs;
    char *s;
    int c;
    extern int optind;
    extern char *optarg;
    
    if (argc < 7)
        usage();

    strcpy(inargs.dbgstr, "");
    strcpy(inargs.aname, "");
    inargs.aprot = rpc_c_authn_none;
    inargs.break_every = -1;

    while ((c = getopt(argc, argv, "Dcp:b:d:")) != EOF)
    {
        switch (c)
        {
        case 'd':
        case 'D':
            strcpy(inargs.dbgstr, c == 'd' ? optarg : "0.1");
            break;
        
        case 'p':
            if ((s = (char *) strtok(optarg, ",")) == NULL)
                usage(-1);
            inargs.aprot = atoi(s);
 
            if ((s = (char *) strtok(NULL, ",")) == NULL)
                usage(-1);
            inargs.azprot = atoi(s);

            if ((s = (char *) strtok(NULL, ",")) == NULL)
                inargs.alevel = rpc_c_authn_level_default;
            else {
                inargs.alevel = atoi(s);

                if ((s = (char *) strtok(NULL, " ")) == NULL) 
                    strcpy(inargs.aname, "");
                else 
                    strcpy(inargs.aname, s);
            }
            break;

        case 'b':
            inargs.break_every = atoi(optarg);
            break;

        default:
            usage();
        }
    }

    argc -= optind - 1;
    argv = &argv[optind - 1];

    if (argc < 7)
        usage();

    if (strcmp(argv[1], "n") == 0)
        inargs.op_num = NULL_TEST;
    else if (strcmp(argv[1], "i") == 0)
        inargs.op_num = INS_TEST;
    else if (strcmp(argv[1], "o") == 0)
        inargs.op_num = OUTS_TEST;
    else if (strcmp(argv[1], "io") == 0)
        inargs.op_num = INS_OUTS_TEST;
    else 
    {
        printf("unrecognized operation - %s\n", argv[1]);
        exit(-1);
    }

    strcpy(inargs.sname, argv[2]);

    inargs.num_calls = atoi(argv[3]);
    inargs.num_buffs = atoi(argv[4]);
    inargs.buff_size = atoi(argv[5]); 
    inargs.call_opts = atoi(argv[6]);
    
#ifndef KERNEL_TEST_DRIVER
    v2c_doit(&inargs, &outargs);
#else
    v2c_make_kernel_doit(&inargs, &outargs);
#endif

    if (outargs.st != rpc_s_ok)
        printf("error %08x\n", outargs.st); 
}

#endif /* _KERNEL */

/*****************************************************************************/ 

#ifdef KERNEL_TEST_DRIVER

/*****************************************************************************/ 

/*
 * V 2 C _ M A K E _ K E R N E L _ D O I T
 *
 * Used for user-space program that tells the in-kernel v2client what to
 * do.  (Context [2].)
 */

v2c_make_kernel_doit(inargs, outargs)
v2c_inargs_t    *inargs;
v2c_outargs_t   *outargs;
{
    v2c_args_t args;
    int fd;

    if ((fd = open(NCSDEVICE, O_RDWR)) == -1)
    {
        printf("can't open %s ", NCSDEVICE);
        perror("");
        exit(-1);
    }

    args.in = *inargs;
    if (ioctl(fd, NIOCV2C, &args) == -1)
    {
        printf("ioctl ");
        perror("");
        exit(-1);
    }
    *outargs = args.out;
}

/*****************************************************************************/ 

#else

/*****************************************************************************/

/*
 * V 2 C _ D O I T
 *
 * Used for standard user-space v2test client.  (Context [1].)
 */

v2c_doit(inargs, outargs)
v2c_inargs_t    *inargs;
v2c_outargs_t   *outargs;
{
    rpc_call_handle_t call;
    rpc_iovector_t ins;
    rpc_iovector_elt_t outs;
    byte_p_t ptr;
    handle_t hand;
    crc_bits crc_val;
    unsigned32 total_size;
    unsigned32 i, j, loopcnt, outlen, *data_buff;
    unsigned32 context_arg;
    boolean maintain_liveness = false;
    unsigned32 *st = &outargs->st;
    rpc_transfer_syntax_t syntax;
    ndr_format_t ndr_format;


    if (inargs->dbgstr[0])
    {
        debug = true;
        rpc__dbg_set_switches(inargs->dbgstr, st);
        CHECK_STATUS(st, "rpc__dbg_set_switches failed");
    }

    
    
    if ((inargs->call_opts & 0x8) != 0)
    {
        maintain_liveness = true;
        inargs->call_opts &= 0x7;
    }

    hand = bind_to_name(inargs->sname);
    if (hand == NULL) {
	    printf("v2c_doit: error - NULL handle returned by bind_to_name\n");
	    printf("v2c_doit: aborting\n");
	    return;
    }

    rpc_binding_set_auth_info 
        (hand, strlen(inargs->aname) > 0 ? inargs->aname : NULL,
            inargs->alevel, inargs->aprot, NULL, 
            inargs->azprot, st);
    CHECK_STATUS(st, "rpc_binding_set_auth_info  failed");
        
    /*
     * Register the test interface, in case the server wants to call us back. 
     *
     */
    rpc_server_register_if((rpc_if_handle_t) &callback_if_c, NULL, 
                     (rpc_mgr_epv_t) NULL, st);
    CHECK_STATUS(st, "rpc_server_register_if failed");

    /*
     * If doing the xmit/recv test, Alloc up the buffer to be used to
     * transmit and receive the data. Fill it with random values, and
     * then do a checksum.
     */
              
    switch (inargs->op_num)
    {
        case NULL_TEST :
            total_size = inargs->buff_size = 0;
            inargs->num_buffs = 1;
            ptr = NULL;
            data_buff = NULL;
            break;

        case INS_TEST :
            total_size = inargs->num_buffs * inargs->buff_size;

            data_buff = (unsigned32 *) malloc((int) inargs->buff_size);
            CHECK_POINTER(data_buff); 

            ptr = (byte_p_t) data_buff;
            if (inargs->num_buffs < 1){
                inargs->num_buffs = 1; 
                inargs->buff_size = 0;
                }
            break;

        case OUTS_TEST :
            if (inargs->buff_size < 8 || inargs->num_buffs == 0)
            {
                inargs->num_buffs = 1;
                inargs->buff_size = 8;
            }
            total_size = inargs->num_buffs * inargs->buff_size;

            data_buff = (unsigned32 *) malloc(8);
            CHECK_POINTER(data_buff); 

            ptr = (byte_p_t) data_buff;
            data_buff[0] = inargs->num_buffs;
            data_buff[1] = inargs->buff_size;
            break;

        case INS_OUTS_TEST :
            if (inargs->buff_size == 0 || inargs->num_buffs == 0)
            {
                inargs->num_buffs = 1;
                inargs->buff_size = 0;
            }

            inargs->buff_size = (inargs->buff_size + 3) & 0xFFFFFFFC;
            total_size = inargs->num_buffs * inargs->buff_size;

            data_buff = (unsigned32 *) malloc((int) total_size);
            CHECK_POINTER(data_buff); 

            ptr = (byte_p_t) data_buff;
#ifndef _KERNEL
            srand(1);
#endif
            crc_init();
            for (j = 0; j < total_size / 4; j++)
#ifndef _KERNEL
                *(data_buff + j) = j;
#else
                *(data_buff + j) = rand();
#endif
            crc_val = crc((unsigned char *) data_buff, total_size);
            break;
    }
     
    if (maintain_liveness)
    {
        rpc_call_start(hand, 2, (rpc_if_handle_t) &v2test_if_c, 4, &call, 
                       &syntax, st);
        CHECK_STATUS(st, "rpc_call_start failed");

        rpc_network_maintain_liveness(hand, st);
        CHECK_STATUS(st, "rpc_network_maintain_liveness failed");

        ins.num_elt = 1;
        ins.elt[0].buff_dealloc = NULL;
        ins.elt[0].flags = rpc_c_iovector_elt_reused;
        ins.elt[0].buff_len = ins.elt[0].data_len = 4;
        context_arg = 1;
        ins.elt[0].buff_addr = ins.elt[0].data_addr = (byte_p_t) &context_arg;

        rpc_call_transceive(call, &ins, &outs, &ndr_format, st);
        CHECK_STATUS(st, "rpc_call_transceive failed");

        rpc_call_end(&call, st);
        CHECK_STATUS(st, "rpc_call_end failed");
    }

    /*
     * Do a call start/end pair to remove the one time startup costs
     * from the measurement of the call loop.  We must also do a transceive
     * to allow the runtime's end of call flag to be set (so that call_end()
     * does not fail).
     */

    rpc_call_start(hand, inargs->call_opts, (rpc_if_handle_t) &v2test_if_c, 
                            NULL_TEST, &call, &syntax, st);
    CHECK_STATUS(st, "rpc_call_start failed");

    ins.num_elt = 1;
    ins.elt[0].buff_dealloc = NULL;
    ins.elt[0].flags = rpc_c_iovector_elt_reused;
    ins.elt[0].buff_len = ins.elt[0].data_len = 0;
    context_arg = 0;
    ins.elt[0].buff_addr = ins.elt[0].data_addr = (byte_p_t) NULL;

    rpc_call_transceive(call, &ins, &outs, &ndr_format, st);
    CHECK_STATUS(st, "rpc_call_transceive failed");

    rpc_call_end(&call, st);
    CHECK_STATUS(st, "rpc_call_end failed");

    /*
     * Get a start time for the whole loop.
     */

    BEGIN_TIMING(CALL_TIME);

    for (loopcnt = inargs->num_calls; loopcnt > 0; loopcnt--)
    {
#ifdef KRB_AUTH
        if (inargs->break_every > 0)
        {
            if ((loopcnt % inargs->break_every) == 0)
                rpc__krb_clobber_key (hand, "aaaaaaaa");
        }
#endif
        rpc_call_start(hand, inargs->call_opts, (rpc_if_handle_t) &v2test_if_c, 
                                (unsigned32) inargs->op_num, &call, &syntax, st);
        CHECK_STATUS(st, "rpc_call_start failed");

        /*
         * Send the input buffers, if any, to the server. 
         */

        if (inargs->op_num == INS_OUTS_TEST)
            BEGIN_TIMING(XMIT_TIME);
         
        ins.num_elt = 1;
        ins.elt[0].buff_dealloc = NULL;
        ins.elt[0].flags = rpc_c_iovector_elt_reused;
        ins.elt[0].buff_len = ins.elt[0].data_len = (inargs->op_num == OUTS_TEST) ? 8 : inargs->buff_size;
         
        /*
         * Send all but the last buffer.  The last one will be sent in transceive.
         */

        for (i = 0; inargs->op_num != OUTS_TEST && i < inargs->num_buffs-1; i++)
        {
            ins.elt[0].buff_addr = 
                ins.elt[0].data_addr = 
                   inargs->op_num == INS_TEST ? (byte_p_t) ptr : (byte_p_t) (ptr + (i * inargs->buff_size));

            rpc_call_transmit(call, &ins, st);
            CHECK_STATUS(st, "rpc_call_transmit failed");
         }

        if (inargs->op_num == INS_OUTS_TEST)
            END_TIMING(XMIT_TIME);
                       
        /*
         * If we're sending client data, then send the last block here.  Otherwise,
         * we need to send at least the first packet w/ the buffer information, which
         * was pointed at before the send loop.  NOTE: we're relying on the value of i
         * from the loop above.
         */

        ins.elt[0].buff_addr = 
            ins.elt[0].data_addr = 
                (inargs->op_num == INS_OUTS_TEST) ? (byte_p_t) (ptr + (i * inargs->buff_size)) : (byte_p_t) ptr;

        rpc_call_transceive(call, &ins, &outs, &ndr_format, st);
        CHECK_STATUS(st, "rpc_call_transceive failed");

        /*
         * Process any received data. 
         */

        if (outs.data_len != 0)
        {                  
            outlen = 0;
            if (inargs->op_num == INS_OUTS_TEST)
                bcopy(outs.data_addr, ptr, outs.data_len);
            if (outs.buff_dealloc)
                (*outs.buff_dealloc) (outs.buff_addr);
            outlen = outs.data_len;

            /*
             * Read in any remaining data 
             */
         
            if (inargs->op_num == INS_OUTS_TEST)
                BEGIN_TIMING(RECV_TIME);
            do
            {
                rpc_call_receive(call, &outs, st);
                CHECK_STATUS(st, "rpc_call_receive failed");

                if (outs.data_len)
                {
                    if (inargs->op_num == INS_OUTS_TEST)
                        bcopy(outs.data_addr, ptr + outlen, outs.data_len);
                    if (outs.buff_dealloc)
                        (*outs.buff_dealloc) (outs.buff_addr);
                    outlen += outs.data_len;    
                }
            } while (outs.data_len);

            if (inargs->op_num == INS_OUTS_TEST)
                END_TIMING(RECV_TIME);
        }

        /*
         * Let runtime know we're done. 
         */

        rpc_call_end(&call, st);
        CHECK_STATUS(st, "rpc_call_end failed");

        if (maintain_liveness)
            pthread_delay_np(&delay60sec);

    }

    END_TIMING(CALL_TIME);
                        
    if (maintain_liveness)
    {
        rpc_network_stop_maintaining(hand, st);
        CHECK_STATUS(st, "rpc_network_stop_maintaining failed");

        rpc_call_start(hand, 2, (rpc_if_handle_t) &v2test_if_c, 4, &call, &syntax, st);
        CHECK_STATUS(st, "rpc_call_start failed");

        ins.num_elt = 1;
        ins.elt[0].buff_dealloc = NULL;
        ins.elt[0].flags = rpc_c_iovector_elt_reused;
        ins.elt[0].buff_len = ins.elt[0].data_len = 4;
        context_arg = 0;
        ins.elt[0].buff_addr = ins.elt[0].data_addr = (byte_p_t) &context_arg;

        rpc_call_transceive(call, &ins, &outs, &ndr_format, st);
        CHECK_STATUS(st, "rpc_call_transceive failed");

        rpc_call_end(&call, st);
        CHECK_STATUS(st, "rpc_call_end failed");
    }

    /*
     * Report time spent making calls. 
     */
        
    printf("Call times: %ld.%03ld ms/call    ", 
           (timers[CALL_TIME].total_us / inargs->num_calls) / 1000,
           (timers[CALL_TIME].total_us / inargs->num_calls) % 1000);                                    

    if (inargs->op_num == INS_OUTS_TEST)
    {
        unsigned32 xmit_time = timers[XMIT_TIME].total_us / 1000;
        unsigned32 recv_time = timers[RECV_TIME].total_us / 1000;
        unsigned32 data_size = inargs->num_calls * (inargs->num_buffs-1) * inargs->buff_size;

        if (xmit_time == 0)
            printf("xmit time too small, ");
        else 
            printf("xmit %ld kb/sec, ", data_size / xmit_time);

        if (recv_time == 0)
            printf("recv time too small\n");
        else 
            printf("recv %ld kb/sec\n", data_size / recv_time);
    }
    else if (inargs->op_num != NULL_TEST &&  inargs->buff_size > 0 && inargs->num_buffs > 0)
    {
        unsigned32 call_time = timers[CALL_TIME].total_us / 1000;
        unsigned32 data_size = inargs->num_calls * inargs->num_buffs  * inargs->buff_size;

        if (call_time == 0)
            printf("call time too small\n");
        else
            printf("%s %ld kb/sec\n", 
                inargs->op_num == INS_TEST ? "xmit" : "recv", data_size  / call_time);
    }

    printf("\n");
 
    /*
     * Make sure the data made it back correctly 
     */

    if (inargs->op_num == INS_OUTS_TEST 
        && crc_val != crc((unsigned char *) data_buff, total_size))
    {
        printf("CRC of returned data doesn't match.\n");
        abort();
    }

    if (inargs->op_num != NULL_TEST)
        free(data_buff);

    if (debug)
    {
        printf("\nwaiting 5 seconds for (potential) ACK processing...\n");
            pthread_delay_np(&delay5sec);
    }

    rpc_server_unregister_if((rpc_if_handle_t) &callback_if_c, NULL, st);
    CHECK_STATUS(st, "rpc_server_unregister_if failed");

    rpc_binding_free(&hand, st);
    CHECK_STATUS(st, "rpc_binding_free failed");
}

/*****************************************************************************/

/*
 * B I N D _ T O _ N A M E
 *
 * Turn a string binding into a binding.
 */

handle_t bind_to_name(name) 
char *name;
{
    unsigned32 st;
    handle_t hand;

    rpc_binding_from_string_binding((ndr_char *) name, &hand, &st);
    CHECK_STATUS(&st, "rpc_binding_from_string_binding failed");

    return (hand);
}

/*****************************************************************************/ 

/*
 * C A L L B A C K _ R O U T I N E 
 *
 * This routine fulfills the role of a server stub routine.  It reads
 * in all of the input packets associated with the current call, but
 * instead of passing them on to a real manager routine it just ships
 * them back to the client.
 */

void callback_routine(h, call_h, iovec_elt_ins, ndr_format, transfer_syntax, epv, st)
handle_t h;
rpc_call_handle_t call_h;
rpc_iovector_elt_p_t iovec_elt_ins;
ndr_format_p_t ndr_format;
rpc_transfer_syntax_p_t transfer_syntax;
rpc_mgr_epv_t epv;
unsigned32 *st;
{
    iovec_ll_elt_t iovec_head, iovec_tmp, *iovec_ptr, *io_temp;
    rpc_iovector_t outs;

    *st = 0L;
    iovec_head.next = 0;
    iovec_head.pkt = *iovec_elt_ins;
    iovec_ptr = &iovec_head;
    outs.num_elt = 1;
    outs.elt[0].buff_dealloc = 0;
    outs.elt[0].flags = rpc_c_iovector_elt_reused;
  
    if (debug)
        printf("(v2client) starting server callback\n");

    /*
     * Read in any remaining data 
     */
    do
    {
        rpc_call_receive(call_h, &iovec_tmp.pkt, st);
        CHECK_STATUS(st, "rpc_call_receive failed");

        if (iovec_tmp.pkt.data_len)
        {
            iovec_ptr->next = (iovec_ll_elt_t *) malloc(sizeof(iovec_ll_elt_t));
            CHECK_POINTER(iovec_ptr->next);

            iovec_ptr = iovec_ptr->next;
            iovec_ptr->next = NULL;
            iovec_ptr->pkt = iovec_tmp.pkt;
        }
    } while (iovec_tmp.pkt.data_len);

    /*
     * Return data from each received packet, releasing allocated memory as
     * we go. 
     */

    for (iovec_ptr = &iovec_head; iovec_ptr;)
    {
        outs.elt[0].buff_len = outs.elt[0].data_len = iovec_ptr->pkt.data_len;
        outs.elt[0].buff_addr = outs.elt[0].data_addr = iovec_ptr->pkt.data_addr;

        rpc_call_transmit(call_h, &outs, st);
        CHECK_STATUS(st, "rpc_call_transmit failed");

        io_temp = iovec_ptr;
        iovec_ptr = iovec_ptr->next;
        if (io_temp->pkt.buff_dealloc)
            (*io_temp->pkt.buff_dealloc) (io_temp->pkt.buff_addr);
        if (io_temp != &iovec_head)
            free(io_temp);
    }
}

/*****************************************************************************/ 

#if ! defined(apollo) || defined(_KERNEL)

/*
 * E N D _ T I M I N G
 *
 */
 
static end_timing(timer)
struct timer *timer;
{
    struct timeval t; 

#ifdef SNI_SVR4
    gettimeofday(&t);
#else
    gettimeofday(&t, (struct timezone *) 0);
#endif
    if (t.tv_usec < timer->timer.tv_usec)
    {
        t.tv_sec--;
        t.tv_usec += 1000000;
    }
    t.tv_usec -= timer->timer.tv_usec;
    t.tv_sec -= timer->timer.tv_sec;

    timer->total_us = (t.tv_sec * 1000000) + t.tv_usec;
    timer->count++;
}

#endif

/*****************************************************************************/ 

#endif  /* #ifndef KERNEL_TEST_DRIVER */
