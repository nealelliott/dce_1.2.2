/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: v2server.c,v $
 * Revision 1.1.540.2  1996/02/17  23:24:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:28  marty]
 *
 * Revision 1.1.540.1  1995/12/11  20:15:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:29  root]
 * 
 * Revision 1.1.538.1  1994/01/21  22:40:36  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:17:39  cbrooks]
 * 
 * Revision 1.1.4.2  1993/01/12  00:03:34  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:26:30  bbelch]
 * 
 * Revision 1.1.2.3  1992/05/05  21:19:09  mishkin
 * 	Remove prototype for main; didn't work on AIX and who needs it anyway?
 * 	[1992/05/05  13:48:02  mishkin]
 * 
 * Revision 1.1.2.2  1992/05/01  15:42:40  rsalz
 * 	Part of rpc6 code drop.
 * 	[1992/05/01  01:30:35  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:42:04  devrcs
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
**      v2server.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Server application code for system exerciser.
** 
**  The stub routine defined here simply returns any received input packets
**  back to the client for the purpose of testing the underlying packet
**  handling routines.
**
**
**
*/

#include <v2test.h>

#include <dce/id_base.h>

globalref ndr_format_t ndr_g_local_drep;

/*****************************************************************************/

/*
 * Parts of the code in this module get used in 3 different context:
 *
 * (1) Standard user-space v2test server.
 * (2) Driver for in-kernel v2test server.
 * (3) In-kernel v2test server.
 *
 * KERNEL_TEST_DRIVER is #define'd when this module is being built for
 * context (2).  _KERNEL is #define'd for context (3).  Nothing in
 * particular is #define'd for context (1).
 */

/*****************************************************************************/

static boolean do_callback = false;
static boolean debug = false;
static boolean auth = false;
static int break_every;

/*****************************************************************************/

#ifdef KERNEL_TEST_DRIVER

void v2s_make_kernel_doit
    _DCE_PROTOTYPE_((
        v2s_inargs_t    *inargs,
        v2s_outargs_t   *outargs
    ));

#else

void v2s_doit
    _DCE_PROTOTYPE_((
        v2s_inargs_t    *inargs,
        v2s_outargs_t   *outargs
    ));

void stub_null
    _DCE_PROTOTYPE_((
        handle_t h,
        rpc_call_handle_t call_h,
        rpc_iovector_elt_p_t iovec_elt_ins,
        ndr_format_p_t ndr_format,
        rpc_transfer_syntax_p_t transfer_syntax,
        rpc_mgr_epv_t epv,
        unsigned32 *st
    ));

void stub_ins 
    _DCE_PROTOTYPE_((
        handle_t h,
        rpc_call_handle_t call_h,
        rpc_iovector_elt_p_t iovec_elt_ins,
        ndr_format_p_t ndr_format,
        rpc_transfer_syntax_p_t transfer_syntax,
        rpc_mgr_epv_t epv,
        unsigned32 *st
    ));

void stub_outs
    _DCE_PROTOTYPE_((
        handle_t h,
        rpc_call_handle_t call_h,
        rpc_iovector_elt_p_t iovec_elt_ins,
        ndr_format_p_t ndr_format,
        rpc_transfer_syntax_p_t transfer_syntax,
        rpc_mgr_epv_t epv,
        unsigned32 *st
    ));

void stub_ins_outs
    _DCE_PROTOTYPE_((
        handle_t h,
        rpc_call_handle_t call_h,
        rpc_iovector_elt_p_t iovec_elt_ins,
        ndr_format_p_t ndr_format,
        rpc_transfer_syntax_p_t transfer_syntax,
        rpc_mgr_epv_t epv,
        unsigned32 *st
    ));

void stub_ctxt
    _DCE_PROTOTYPE_((
        handle_t h,
        rpc_call_handle_t call_h,
        rpc_iovector_elt_p_t iovec_elt_ins,
        ndr_format_p_t ndr_format,
        rpc_transfer_syntax_p_t transfer_syntax,
        rpc_mgr_epv_t epv,
        unsigned32 *st
    ));

void callback
    _DCE_PROTOTYPE_((
        handle_t h
    ));

void rundown
    _DCE_PROTOTYPE_((
        rpc_client_handle_t client_h
    ));

static rpc_v2_server_stub_proc_t stub_list[] = { 
    stub_null,
    stub_ins,
    stub_outs,
    stub_ins_outs,
    stub_ctxt
};

/*
 * Client specific callback and client interface representations.  The
 * name of these structures must be different from the names of equivalent
 * server structures.  We must not have name conflicts in the kernel
 * were the client and server are part of the same executable image.
 */

static rpc_if_rep_t v2test_if_s = V2TEST_IF_REP_INIT(stub_list);

static rpc_if_rep_t callback_if_s = CALLBACK_IF_REP_INIT(NULL);

#endif

/*****************************************************************************/

#ifndef _KERNEL

/*
 * M A I N
 *
 */

main(argc, argv) 
int argc;
char *argv[];
{
    unsigned32 st;
    unsigned32 max_calls;
    v2s_inargs_t inargs;
    v2s_outargs_t outargs;
    v2s_args_t args;
    int i, j;
    char *s;
    int c;
    extern int optind;
    extern char *optarg;

    inargs.num_threads = -1;
    inargs.do_callback = false;
    inargs.ep_register = false; 
    inargs.num_protseq = 0;

    for (i = 0; i < V2_MAX_IDENTS; i++) 
    {
        inargs.idents[i].aprot = rpc_c_authn_none;
    }

    inargs.break_every = -1;
    strcpy(inargs.dbgstr, "");
    
    while ((c = getopt(argc, argv, "Dcep:b:d:")) != EOF)
    {
        switch (c)
        {
        case 'c':
            inargs.do_callback = true;
            break;

        case 'e':
            inargs.ep_register = true;
            break;

        case 'p':
            i = 0;

            if ((s = (char *) strtok(optarg, ",")) == NULL)
                usage(-1);

            inargs.idents[i].aprot = atoi(s);

            while (1) 
            {
                if ((s = (char *) strtok(NULL, " ")) == NULL)
                    usage(-1);

                strcpy(inargs.idents[i].aname, s);
                i++;

                if ((s = (char *) strtok(NULL, ",")) == NULL)
                    break;

                if (i >= V2_MAX_IDENTS) 
                {
                    printf("No more than %d -p options are accepted at once\n", 
                            V2_MAX_IDENTS);
                    usage();
                }

                inargs.idents[i].aprot = atoi(s);
            }
            break;

        case 'b':
            inargs.break_every = atoi(optarg);
            break;
        
        case 'd':
        case 'D':
            strcpy(inargs.dbgstr, c == 'd' ? optarg : "0.1");
            break;

        default:
            usage();
        }
    }

    argc -= optind - 1;
    argv = &argv[optind - 1];

    if (argc < 3)
        usage();

    inargs.num_threads = atoi(argv[1]);

    for (i = 2; i < argc; i++)
    {
        if (inargs.num_protseq >= V2_MAX_PROTSEQ) {
            printf("Too many protocol sequences found (max of %d allowed)\n",
                    V2_MAX_PROTSEQ);
            usage();
        }
        strcpy(inargs.protseq[inargs.num_protseq++].s, argv[i]);
    }

#ifndef KERNEL_TEST_DRIVER
    v2s_doit(&inargs, &outargs);
#else
    v2s_make_kernel_doit(&inargs, &outargs);
#endif

    if (outargs.st != rpc_s_ok)
        printf("error %08x\n", outargs.st);
}

usage()
{
    printf("usage: v2server [-Dbce] [-d <debug switches>] [-p \"<authn proto>,<principal> ...\"]\n");
    printf("                <max calls> <protseq>...\n");
    exit(-1);
}

#endif /* _KERNEL */

/*****************************************************************************/ 

#ifdef KERNEL_TEST_DRIVER

/*****************************************************************************/ 

/*
 * V 2 S _ M A K E _ K E R N E L _ D O I T
 *
 * Used for user-space program that tells the in-kernel v2server what to
 * do.  (Context [2].)
 */

void v2s_make_kernel_doit(inargs, outargs)
v2s_inargs_t *inargs;
v2s_outargs_t *outargs;
{
    v2s_args_t args;
    int fd;

    if ((fd = open(NCSDEVICE, O_RDWR)) == -1)
    {
        printf("can't open %s ", NCSDEVICE);
        perror("");
        exit(-1);
    }

    args.in = *inargs;
    if (ioctl(fd, NIOCV2S, &args) == -1)
    {
        printf("ioctl \n");
        perror("");
        exit(-1);
    }
    *outargs = args.out;
}

/*****************************************************************************/ 

#else

/*****************************************************************************/ 

/*
 * V 2 S _ D O I T
 *
 * Used for standard user-space v2test server.  (Context [1].)
 */

void v2s_doit(inargs, outargs)
v2s_inargs_t *inargs;
v2s_outargs_t *outargs;
{
    unsigned32 *st = &outargs->st;
    unsigned32 i;
    rpc_binding_vector_p_t bv;

    if (inargs->dbgstr[0])
    {
        debug = true;
        rpc__dbg_set_switches(inargs->dbgstr, st);
        CHECK_STATUS(st, "rpc__dbg_set_switches failed");
    }

    do_callback = inargs->do_callback;

    for (i = 0; i < inargs->num_protseq; i++)
    {
        rpc_server_use_protseq((unsigned_char_p_t) inargs->protseq[i].s, 0, st);
        CHECK_STATUS(st, "rpc_server_use_protseq failed");
    }

    break_every = inargs->break_every;

    for (i = 0; i < V2_MAX_IDENTS; i++) 
    {
        if (inargs->idents[i].aprot != rpc_c_authn_none)
        {
            rpc_server_register_auth_info (inargs->idents[i].aname, inargs->idents[i].aprot, NULL, NULL, st);
            auth = true;
            CHECK_STATUS(st, "rpc_server_register_auth_info failed");
        }
    }
    
    rpc_server_inq_bindings(&bv, st);
    CHECK_STATUS(st, "rpc_server_inq_bindings failed");

    for (i = 0; i < bv->count; i++)
    {
        unsigned_char_p_t sb;

        rpc_binding_to_string_binding(bv->binding_h[i], &sb, st);
        CHECK_STATUS(st, "rpc_binding_to_string_binding failed");
        printf("Got binding: %s\n", sb);
        rpc_string_free(&sb, st);
    }
   
    if (inargs->ep_register == true)
    {                                      
#ifndef _KERNEL
        printf("registering endpoint(s) with LLBD...\n");
        rpc_ep_register((rpc_if_handle_t) &v2test_if_s, bv, NULL, 
            (unsigned_char_p_t) "Your interface name here!!", st);                    
        CHECK_STATUS(st, "rpc_ep_register failed");

        printf("unregistering endpoint(s) from the LLBD...\n");
        rpc_ep_unregister((rpc_if_handle_t) &v2test_if_s, bv, NULL, st);
        CHECK_STATUS(st, "rpc_ep_unregister failed");
                                           
        printf("exiting successfully\n");
        exit(0);
#else
        printf("rpc_ep_* not present in KERNEL\n");
#endif
    }

    rpc_server_register_if((rpc_if_handle_t) &v2test_if_s, NULL, (rpc_mgr_epv_t) NULL, st);
    CHECK_STATUS(st, "rpc_server_register_if failed");

    rpc_server_listen(inargs->num_threads, st);

    if (*st != rpc_s_ok)
        printf("network listen returned %d\n", *st);

}

void check_auth(h)
handle_t h;
{
    unsigned char *caller, *me;
    unsigned32 level, authn_svc, authz_svc;
    unsigned32 st;
    static int ncalls;
    
    authn_svc = authz_svc = level = 0;
    caller = me = NULL;
        
    rpc_binding_inq_auth_client 
        (h, (rpc_authz_handle_t *) &caller, &me, &level, &authn_svc, &authz_svc, &st);

    if (debug)
    {
	sec_id_pac_t *pac;
	int i;
	
	switch (authz_svc) {
	case rpc_c_authz_name:
	    printf("--> rpc_binding_inq_auth_client ( ..., %d, %d, %d, %s, %s, %x);\n",
		level, authn_svc, authz_svc, caller?(char *)caller:"(NULL)", me?(char *)me:"(NULL)", st);
	    break;
	case rpc_c_authz_dce:
	    printf("--> rpc_binding_inq_auth_client ( ..., %d, %d, %d, pac at %x, %s, %x);\n",
		level, authn_svc, authz_svc, caller, me?(char *)me:"(NULL)", st);
	    pac = (sec_id_pac_t *)caller;
	    printf("PAC contains: uid %d, gid %d, ngroups %d:",
		pac->principal.uuid.time_low,
		pac->group.uuid.time_low,
		pac->num_groups);
	    for (i=0; i<pac->num_groups; i++) {
		printf (" %d", pac->groups[i].uuid.time_low);
	    }
	    printf("\n");
	    break;
        default:
	    printf("Unknown authz svc!\n");
	    printf("--> rpc_binding_inq_auth_client ( ..., %d, %d, %d, %s, %x, %x);\n",
		level, authn_svc, authz_svc, caller?(char *)caller:"(NULL)", me, st);
	    break;
	}
    } else {
        /* !!! should do something here */
    }
    ncalls++;

    /*
     * Free memory allocated by inq_auth_client call.
     */
    if (me != NULL)
        rpc_string_free(&me, &st);

#ifdef KRB_AUTH
    if (break_every > 0)
    {
        if ((ncalls % break_every) == 0)
            rpc__krb_clobber_key (h, "aaaaaaaa");
    }
#endif
}

/*****************************************************************************/ 

/*
 * S T U B _ N U L L
 *
 * This routine fulfills the role of a server stub routine for performing 
 * NULL calls.
 */

void stub_null(h, call_h, iovec_elt_ins, ndr_format, transfer_syntax, epv, st)
handle_t h;
rpc_call_handle_t call_h;
rpc_iovector_elt_p_t iovec_elt_ins;
ndr_format_p_t ndr_format;
rpc_transfer_syntax_p_t transfer_syntax;
rpc_mgr_epv_t epv;
unsigned32 *st;
{
    *st = 0L;

    if (debug)
        printf("--> null call\n" );

    if (auth)
        check_auth(h);
        
    /*
     * If we are testing callbacks, do so now. 
     */
    if (do_callback)
        callback(h);
}

/*****************************************************************************/ 

/*
 * S T U B _ I N S
 *
 * This routine fulfills the role of a server stub routine for testing INS.
 */

void stub_ins(h, call_h, iovec_elt_ins, ndr_format, transfer_syntax, epv, st)
handle_t h;
rpc_call_handle_t call_h;
rpc_iovector_elt_p_t iovec_elt_ins;
ndr_format_p_t ndr_format;
rpc_transfer_syntax_p_t transfer_syntax;
rpc_mgr_epv_t epv;
unsigned32 *st;
{
    rpc_iovector_elt_t ins;
    unsigned32 total = iovec_elt_ins->data_len;

    *st = 0L;
               
    if (iovec_elt_ins->buff_dealloc != NULL && iovec_elt_ins->data_len > 0)
        (*iovec_elt_ins->buff_dealloc) (iovec_elt_ins->buff_addr);
    /*
     * Read in any remaining data 
     */
    do
    {
        rpc_call_receive(call_h, &ins, st);
        CHECK_STATUS(st, "rpc_call_receive failed");

        if (ins.data_len > 0)
        {                
            total += ins.data_len;
            if (ins.buff_dealloc != NULL)
                (*ins.buff_dealloc) (ins.buff_addr);
        }        
    } while (ins.data_len);

    /*
     * If we are testing callbacks, do so now. 
     */
    if (do_callback)
        callback(h);

    if (debug)
        printf("--> ins test (%d bytes)\n", total);

    if (auth)
        check_auth(h);
}

/*****************************************************************************/ 

/*
 * S T U B _ O U T S
 *
 * This routine fulfills the role of a server stub routine for testing OUTS.
 */

void stub_outs(h, call_h, iovec_elt_ins, ndr_format, transfer_syntax, epv, st)
handle_t h;
rpc_call_handle_t call_h;
rpc_iovector_elt_p_t iovec_elt_ins;
ndr_format_p_t ndr_format;
rpc_transfer_syntax_p_t transfer_syntax;
rpc_mgr_epv_t epv;
unsigned32 *st;
{
    rpc_iovector_t outs;
    unsigned32 num_buffs, buff_size, *int_ptr = (unsigned32 *) iovec_elt_ins->data_addr;
    char *buffer;

    *st = 0L;
              
    if (ndr_g_local_drep.int_rep == ndr_format->int_rep)
    {
        num_buffs = *int_ptr++;
        buff_size = *int_ptr;
    }
    else
    {   
        unsigned char *_s, *_d;

        _s = (unsigned char *)int_ptr++;
        _d = (unsigned char *) &num_buffs;
        _d[0]=_s[3]; _d[1]=_s[2]; _d[2]=_s[1]; _d[3]=_s[0];

        _s = (unsigned char *)int_ptr;
        _d = (unsigned char *) &buff_size;
        _d[0]=_s[3]; _d[1]=_s[2]; _d[2]=_s[1]; _d[3]=_s[0];
    }

    buffer = (char *) malloc((int) buff_size);
        
    if (debug)
        printf("--> outs test (%d buffs of %d bytes)\n", num_buffs, buff_size);

    if (auth)
        check_auth(h);
    
    /*
     * If we are testing callbacks, do so now. 
     */
    if (do_callback)
        callback(h);

    outs.num_elt = 1;
    outs.elt[0].buff_dealloc = 0;
    outs.elt[0].flags = rpc_c_iovector_elt_reused;
    outs.elt[0].buff_len = outs.elt[0].data_len = buff_size;
    outs.elt[0].buff_addr = outs.elt[0].data_addr = (byte_p_t) buffer;

    if (iovec_elt_ins->buff_dealloc != NULL)
        (*iovec_elt_ins->buff_dealloc) (iovec_elt_ins->buff_addr);

    /*
     * For outs only tests send the requested number of buffers. 
     */

    while (num_buffs--)
    {             
        rpc_call_transmit(call_h, &outs, st);
        CHECK_STATUS(st, "rpc_call_transmit failed");
    }

    free(buffer);
}

/*****************************************************************************/ 

/*
 * S T U B _ I N S _ O U T S
 *
 * This routine fulfills the role of a server stub routine for testing
 * calls with both input and output parameters.
 */

void stub_ins_outs(h, call_h, iovec_elt_ins, ndr_format, transfer_syntax, epv, st)
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
    unsigned32 total = iovec_elt_ins->data_len;

    *st = 0L;
    iovec_head.next = 0;
    iovec_head.pkt = *iovec_elt_ins;
    iovec_ptr = &iovec_head;
    outs.num_elt = 1;
    outs.elt[0].buff_dealloc = 0;
    outs.elt[0].flags = rpc_c_iovector_elt_reused;

    /*
     * Read in any remaining data 
     */

    do
    {
        rpc_call_receive(call_h, &iovec_tmp.pkt, st);
        CHECK_STATUS(st, "rpc_call_receive failed");

        if (iovec_tmp.pkt.data_len > 0)
        {                
            total += iovec_tmp.pkt.data_len ;
            iovec_ptr->next = (iovec_ll_elt_t *) malloc(sizeof(iovec_ll_elt_t));
            iovec_ptr = iovec_ptr->next;
            iovec_ptr->next = NULL;
            iovec_ptr->pkt = iovec_tmp.pkt;
        }
    } while (iovec_tmp.pkt.data_len);
            
    if (debug)                     
        printf("-->ins_outs test (%d bytes)\n", total);

    if (auth)
        check_auth(h);

    /*
     * If we are testing callbacks, do so now. 
     */
    if (do_callback)
        callback(h);

    /*
     * Return data from each received packet, releasing allocated memory as
     * we go. 
     */
                    
    iovec_ptr = &iovec_head;
       
    while (iovec_ptr != NULL)
    {             
        outs.elt[0].buff_len = outs.elt[0].data_len = iovec_ptr->pkt.data_len;
        outs.elt[0].buff_addr = outs.elt[0].data_addr = iovec_ptr->pkt.data_addr;

        rpc_call_transmit(call_h, &outs, st);
        CHECK_STATUS(st, "rpc_call_transmit failed");

        io_temp = iovec_ptr;
        iovec_ptr = iovec_ptr->next;
        if (io_temp->pkt.buff_dealloc != NULL)
            (*io_temp->pkt.buff_dealloc) (io_temp->pkt.buff_addr);
        if (io_temp != &iovec_head)
            free((char *)io_temp);
    }
}

/*****************************************************************************/ 

/*
 * S T U B _ C T X T
 *
 * This routine fulfills the role of a server stub routine for testing
 * calls which use context handles.  Client monitoring is initiated when
 * the client calls this routine with an input argument of 1.  Then client
 * then goes on to make a series of calls, followed by another call to
 * this routine with an argument of 0, indicating that liveness monitoring
 * should be ended.
 */                
#define START_MONITORING  1
#define MAX_CTXT_HANDLES 20
static rpc_client_handle_t client_handle[MAX_CTXT_HANDLES];

void stub_ctxt(h, call_h, iovec_elt_ins, ndr_format, transfer_syntax, epv, st)
handle_t h;
rpc_call_handle_t call_h;
rpc_iovector_elt_p_t iovec_elt_ins;
ndr_format_p_t ndr_format;
rpc_transfer_syntax_p_t transfer_syntax;
rpc_mgr_epv_t epv;
unsigned32 *st;
{
    unsigned32 i, ctxt_op, *int_ptr = (unsigned32 *) iovec_elt_ins->data_addr;
    rpc_client_handle_t temp_handle;

    *st = rpc_s_ok;

    if (ndr_g_local_drep.int_rep == ndr_format->int_rep)
        ctxt_op = *int_ptr;
    else
    {   
        unsigned char *_s, *_d;

        _s = (unsigned char *)int_ptr;
        _d = (unsigned char *) &ctxt_op;
        _d[0]=_s[3]; _d[1]=_s[2]; _d[2]=_s[1]; _d[3]=_s[0];
    }
        
     if (debug)
        printf("--> ctxt test - %s\n", ctxt_op == START_MONITORING ? 
                                       "starting" : "ending");

    if (auth)
        check_auth(h);

    /*
     * If we are testing callbacks, do so now. 
     */
    if (do_callback)
        callback(h);

    if (iovec_elt_ins->buff_dealloc != NULL)
        (*iovec_elt_ins->buff_dealloc) (iovec_elt_ins->buff_addr);

    if (ctxt_op == START_MONITORING)
    {      
        /*
         * First find an empty array entry...
         */
        for (i = 0; i < MAX_CTXT_HANDLES; i++)
            if (client_handle[i] == NULL)
                break;

        if (i == MAX_CTXT_HANDLES)
        {
            *st = -1;
            return;
        }
        rpc_binding_inq_client((rpc_binding_handle_t) h,
                            &client_handle[i], st);
        CHECK_STATUS(st, "rpc_binding_inq_client failed");
        rpc_network_monitor_liveness((rpc_binding_handle_t) h,
                            client_handle[i], rundown, st);  
        CHECK_STATUS(st, "rpc_network_monitor_liveness failed");
    }
    else
    {
        /*
         * First find the entry in the array...
         */
        rpc_binding_inq_client((rpc_binding_handle_t) h,
                            &temp_handle, st);
        CHECK_STATUS(st, "rpc_binding_inq_client failed");

        for (i = 0; i < MAX_CTXT_HANDLES; i++)
            if (client_handle[i] == temp_handle)
                break;

        if (i == MAX_CTXT_HANDLES)
        {
            *st = -1;
            return;
        }                       
        client_handle[i] = NULL;
        rpc_network_stop_monitoring((rpc_binding_handle_t) h,
                            temp_handle, st);  
        CHECK_STATUS(st, "rpc_network_stop_monitoring failed");
    }
}

/*****************************************************************************/ 

/*
 * C A L L B A C K
 *
 * Make a call back to a client using the server's binding handle. 
 */

void callback(h)
handle_t h;
{
    unsigned32 st;
    rpc_call_handle_t call;
    rpc_iovector_t ins;
    rpc_iovector_elt_t outs;
    char val = (char) 5;
    rpc_transfer_syntax_t syntax;
    ndr_format_t ndr_format;

    /*
     * Start the call. 
     */

    rpc_call_start(h, 0, (rpc_if_handle_t) &callback_if_s, 0, &call, &syntax, &st);
    CHECK_STATUS(&st, "rpc_call_start failed");

    /*
     * Send the data, and get the results back. 
     */

    ins.num_elt = 1;
    ins.elt[0].buff_dealloc = NULL;
    ins.elt[0].flags = rpc_c_iovector_elt_reused;
    ins.elt[0].buff_addr = ins.elt[0].data_addr = (byte_p_t) & val;
    ins.elt[0].buff_len = ins.elt[0].data_len = sizeof val;

    rpc_call_transceive(call, &ins, &outs, &ndr_format, &st);
    CHECK_STATUS(&st, "rpc_call_transceive failed");

    /*
     * Check the results, and end the call. 
     */

    if (debug)                     
        printf("-->making callback\n");

    if (outs.data_len == 0 || outs.data_addr[0] != 5)
        printf("Callback failed!!!\n");

    if (outs.buff_dealloc != NULL)
        (*outs.buff_dealloc) (outs.buff_addr);

    rpc_call_end(&call, &st);
    CHECK_STATUS(&st, "rpc_call_end failed");
}

/*****************************************************************************/ 

/*
 * R U N D O W N
 *
 * Function to call if we lose contact with a client with which we are
 * monitoring liveness.
 */

void rundown(client_h)
rpc_client_handle_t client_h;                       
{                            
    unsigned32 i;

    for (i = 0; i < MAX_CTXT_HANDLES; i++)
        if (client_handle[i] == client_h)
            break;

    if (i < MAX_CTXT_HANDLES)
        client_handle[i] = NULL;

    printf("--> rundown, client_h %x\n", client_h);
}

/*****************************************************************************/ 

#endif /* #ifndef KERNEL_TEST_DRIVER */
