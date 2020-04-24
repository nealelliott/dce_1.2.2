/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: server_obj_tsts.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:03  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:59  root]
 * 
 * Revision 1.1.2.2  1993/01/11  23:40:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:38  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      server_obj_tsts.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**  
**  This module contains definitions and preinitialized
**  objects used by test routines comtest_if_inq_id and
**  comtest_if_id_vector_free.
**
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  01-oct-91     bmartin   replace GLOBAL with globaldef.
**  23-jul-91     bmartin   shorten comtest_server_xxx names to comtst_svr_xxx.
**  11-feb-91     burton    fixed parser more than one interface
**  09-feb-91     burton    extraifs became interfaceX and interfaceY
**  09-feb-91     burton    preinitialize previous_type_uuid
**  01-feb-91     burton    add epv to comtest_command_t
**  01-feb-91     burton    add interface to struct comtest_command_t
**  28-jan-91     burton    remove longjump
**  07-jan-91     burton    add communication of pid to client
**  05-Nov-90     burton    add support for routine
**                          comtest_if_assemb_args
**  15-oct-90     burton    original
**
**  %a%private_end  
**
*/

#include    <signal.h>
#include    <comtest_server_if.h>
#include    <comtest_interface_x.h>
#include    <comtest_interface_y.h>




/*
 * COMTST_endpoints,  is a preinitialized endpoint vector
 * containing three supported network adress families.  Its
 * address is used as a default component by the preinitialized
 * interface specification, COMTST_ifspec, which is used for
 * default test data by test routine comtest_if_inq_id.
 */

static rpc_endpoint_vector_elt_t COMTST_endpoints[3] = 

{
   (unsigned_char_p_t)"ncacn_ip_tcp", (unsigned_char_p_t)"2002",
   (unsigned_char_p_t)"ncadg_ip_udp", (unsigned_char_p_t)"2002",
   (unsigned_char_p_t)"ncadg_dnet_nsp", (unsigned_char_p_t)"dntst"
};

/*
 * COMTST_stx_id, is a preinitialized syntax representation.
 * Its address is used as a default component by the preinitialized
 * interface specification, COMTST_ifspec, which is used for
 * default test data by test routine comtest_if_inq_id.
 */

static rpc_syntax_id_t COMTST_stx_id =

{
   {0x8a885d04, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
   2
};

/*
 *  COMTST_ifspec is the address passed in the interface
 *  handle, 'comtest_ifspec' to test routine, 'comtest_if_inq_id'
 *  which will extract the 'id' field.  All the data initialized
 *  here serves as a default test case.  Each data field,
 *  ifspec rep version, op count, if version, id, and stub/rt
 *  if version, are replaced by the respective data fields
 *  passed in the input testscript file.
 */

static rpc_if_rep_t COMTST_ifspec =

{
   0,                      /* ifspec rep version */
   2,                      /* op count */
   1,                      /* if version */
   {0x8dc3af80, 0x3791,    /* id */
    0x11c9, 0xb6, 0x6b,
    {0x8, 0x0, 0x2b, 0xa,
     0x76, 0x16}},
   2,                      /* stub/rt if version */
   {3, COMTST_endpoints},  /* endpoint vector */
   {1, &COMTST_stx_id}     /* syntax vector */
};

/*
 * comtest_ifspec, serves as the ifspec handle argument passed
 * to test routine comtest_if_inq_id
 */

rpc_if_handle_t comtest_ifspec = (rpc_if_handle_t)&COMTST_ifspec;

/*
 * COMTST_const_if_id is a preinitialized interface id used
 * to initialize the dynamically allocated interface id
 * vector, used as test data by routine 'comtest_if_id_vector_free.
 */

static rpc_if_id_t COMTST_const_if_id =

{
   {0x8c863a04, 0x13ec, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
   2,
   3
};

/*
 * string_buf, provides buffering for comtest_cat_id
 */
char   string_buf[200];

/*
 * COMTST_server_supported_protseqs is a flag set
 * to indicate that a vector of protseqs which are
 * currently supported has been obtained.
 */
unsigned32   COMTST_svr_supported_protseqs = 0;

/*
 * COMTST_server_psvp is a pointer to
 * the rpc_protseq_vector_t of currently
 * supported protseqs.
 */
rpc_protseq_vector_p_t   COMTST_server_psvp;

/*
 * COMTST_max_calls -- used to initialize
 * calls RPC rpc_server_use_all_protseqs...,
 * etc.
 */
unsigned32   COMTST_max_calls = 5;


/*
 * COMTST_server_wait, and COMTST_server_return
 * are boolean flags to the routine, comtst_svr_wait.
 */
boolean32   COMTST_server_wait = 1;          /* call rpc_server_listen,
					      * listen for an RPC */
boolean32   COMTST_server_return = 0;        /* just return */

/*
 * bogus_obj is a flag set through
 * command line argument to force
 * the client side to send to an
 * unregistered object UUID.
 */
boolean32   bogus_obj;

/*
 * COMTST_bogus_obj_set, and COMTST_bogus_obj_clear
 * are boolean flags force the output of a bogus
 * object uuid.
 */


boolean32   COMTST_bogus_obj_set = true;     /* call rpc_server_listen,
					      * listen for an RPC */
boolean32   COMTST_bogus_obj_clear = false;  /* just return */

/*
 * COMTST_bogus_obj_uuid - sent to the client in order to emulate
 * the condition of an application trying to send an RPC to a
 * interface type UUID which is not registered.
 */
#define     COMTST_bogus_obj_uuid   "88dd7300-cb7c-11c9-9518-08002b13ec4e"


/*
 * COMTST_timeout_wait max time to wait
 * to receive an RPC from the client.
 */
#define COMTST_timeout_wait  30

/*
 * COMTST_test_complete is a GLOBAL flag indicating to
 * the signal handler that the last test has completed
 * and that upon receiving a signal it is ok for the 
 * current test to call exit.
 */
globaldef boolean32 COMTST_test_complete;

/*
 * argument buffer used by comtst_svr_parse_command
 */

typedef struct
{
    struct comtest_command_t     *next;
    uuid_t                   uuid_type;
    uuid_t                    uuid_obj;
    rpc_if_handle_t          interface;
    char                      if_desig;
    rpc_mgr_epv_t                 *epv;
} comtest_command_t, *comtest_command_p_t;

/*
 *  Hold the previous type_uuid registered for
 *  comparison with uuids specified in future
 *  command strings  - WARNING all the object
 *  uuids for a specific type uuid must be set
 *  consecutively on the input command string.
 *  Here we preinitialize so that uuid_equal
 *  will work if we specify a NIL uuid.
 */

uuid_t  previous_type_uuid =

{ /* 2b3dd760-c804-11c9-906d-08002b13ec4e */
    0x2b3dd760,
    0xc804,
    0x11c9,
    0x90,
    0x6d,
    {0x08, 0x00, 0x13, 0xec, 0x4e, 0x00} };

typedef enum { DEFAULT, GENERATED } register_if_test_type;



#define     SERVER_REGISTER_IF_ARGS         (2)
#define     SERVER_INQ_BINDINGS_ARGS        (0)
#define     SERVER_USE_ALL_PROTSEQS_ARGS    (0)
#define     SERVER_USE_ALL_PROTSEQS_IF_ARGS (0)
#define     SERVER_USE_PROTSEQ_ARGS         (2)
#define     SERVER_USE_PROTSEQ_IF_ARGS      (2)
#define     SERVER_USE_PROTSEQ_EP_ARGS      (3)


/***************************************************************************/

globaldef comtest_server_v2_0_epv_t comtest_server_epv =
{
    comtst_svr_xfr
};



extern  rpc_if_handle_t comtest_ifspec;
extern  int verbose;
extern  unsigned32 in_status;
extern  int isanum ();
extern  interface_X_v2_0_epv_t foo_extraifs_epv;

extern   rpc_mgr_epv_t * interfaceX_epv_list[];
extern   rpc_mgr_epv_t * interfaceY_epv_list[];

int comtst_svr_register_if ();
int comtst_svr_use_all_protseqs ();
int comtst_svr_use_protseq ();
int comtst_svr_use_protseq_if ();
int comtst_svr_use_protseq_ep ();
int comtst_svr_use_all_protseqs_if ();
int comtst_svr_wait ();
int comtst_svr_protseq_supported ( );
int comtst_svr_update_endpoint_vect ();
int comtst_svr_get_binding_strings ();
int comtst_svr_open_outfile ();
int comtst_svr_signal_dispatch ();
comtest_command_p_t comtst_svr_parse_command ();

