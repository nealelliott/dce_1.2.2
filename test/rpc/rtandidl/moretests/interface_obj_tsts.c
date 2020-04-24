/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: interface_obj_tsts.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:28  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:05:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:25  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:14:46  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:06:50  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:42  devrcs
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
**      interface_obj_tsts.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**  
**  This module contains two tests for verification
**  of the communications interface objects.
**
**     1. comtest_if_inq_id
**     2. comtest_if_id_vector_free
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  14-aug-91     martin   replace RPC_MEM_ALLOC with malloc() 
**                         to better represent a user application.
**   7-nov-90     woodbury implemented use of verbose flag
**                de-TAB
**   5-Nov-90     burton   added additional routine
**                         comtest_if_assemb_args to gather
**                         args from input and insert into
**                         test ifspec.
**  22-oct-90     burton   original
**
**  %a%private_end  
**
*/


#include <dce/rpc.h>                /* Public types exported by RPC */
#include <tstdriver.h>          /* Local definitions and test driver protocol */
#include <comtest.h>            /* definitions for communications tests */
#include <interface_obj_tsts.h> /* Defs and defaults for interface tests */

extern int verbose;

/*
 * NOTE
 * ==================================================
 * Most of the following will be transfered elsewhere
 * once we conclude where.
 * ==================================================
 */

/*
 * Signatures for routines in this test module
 */
int comtest_if_inq_id ();
int comtest_if_id_vector_free ();
int comtest_if_assemb_args ();

/*
**++
**
**  ROUTINE NAME:       comtest_if_inq_id
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**  Protocol sequence vector test.  RPC routines exercised:
**
**        rpc_if_inq_id
**
**   Test the ability of rpc_if_inq_id to recover the
**   interface id, uuid from an interface specification.
**   The interface specification used may be either the
**   default, provided in interface_obj_tsts.h or replacement
**   values are supplied through the argc, argv input
**   args. comtest_if_assemb_args, is called to parse the
**   input args and initialize the interface spec. uuid_to_string
**   is called to retain the id for later comparison.
**   rpc_if_inq_id  is called to recover the interface id,
**   after converting this to a uuid string the two uuid strings,
**   are compared.  a true comparrison indicates a successful 
**   test.
**
**  INPUTS:             
**
**
**      argc            count of arg pointers passwd by argv
**
**      argv            standard 'argv' array of pointers to null
**                      terminated ascii argument strings
**                      see comment header on routine 
**                      comtest_if_assemb_args for order in which
**                      to specify input args
**
**  INPUTS/OUTPUTS:     none
**
**      
**
**  OUTPUTS:            an rpc status code
**
**      status
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/


comtest_if_inq_id (argc, argv, status)

unsigned32  argc;
char        *argv[];
unsigned32  *status;

{
    rpc_if_id_t               if_id;
    unsigned_char_p_t         if_uuid;
    unsigned_char_p_t         object_uuid;
    rpc_if_rep_p_t            loc_if_rep_pt;



    if (verbose != 0) fprintf (stdout, "Enter Test - %s\n", argv[0]);

    /*
     * get a rpc_if_rep pointer
     */
    loc_if_rep_pt = (rpc_if_rep_p_t) comtest_ifspec;

    /*
     *  gather up args from input file and place into if_spec
     */
     comtest_if_assemb_args (argc, argv, loc_if_rep_pt, status);

    /*
     * convert id to a string and save for later comparison
     */
    uuid_to_string (&(loc_if_rep_pt->id), &if_uuid, status);

    if (*status != uuid_s_ok)
    {
        return COMTEST_FAILED;
    }

    /*
     * test rpc_if_inq_id to extract the uuid from if_spec
     */
    rpc_if_inq_id (comtest_ifspec, &if_id, status);
            
    if (*status != rpc_s_ok)
    {
        return RPC_CALL_FAILED;
    }
    /*
     * convert extracted uuid to string format
     */
    uuid_to_string ((uuid_p_t)&(if_id.uuid), &object_uuid, status);

    if (*status != uuid_s_ok)
    {
        return COMTEST_FAILED;
    }
    /*
     * compare the two uuid strings
     */
    if ( strcmp ( (char *)object_uuid, (char *)if_uuid) )
    {
        /*
         * failed to match uuid strings - test failed
         */
        if (verbose != 0) 
        {
            fprintf (stdout, "\n\t\tERROR UUIDS UNEQUAL\n\t%s\n\t%s\n\n", 
                object_uuid, if_uuid);
        }
        return COMTEST_FAILED;
    }
    else
    {
        /*
         * uuid strings match - return test success
         */
        if (verbose != 0) 
        {
            fprintf (stdout, "\n\t\tUUIDS EQUAL\n\t%s\n\t%s\n\n",
                                 object_uuid, if_uuid);
        }
        return COMTEST_OK;
    }
}

/*
**++
**
**  ROUTINE NAME:       comtest_if_id_vector_free
**
**  SCOPE:              global - defined in tstdriver.h 
**
**  DESCRIPTION:
**
**  Protocol sequence string validity test.  RPC routines exercised:
**
**        rpc_if_id_vector_free
**
**
**   First allocate space for an rpc interface id vector.
**   Then test call rpc_if_id_vector_free to free the space.
**      
**
**  INPUTS:             
**
**      argc            retained only for conformance with calling
**                        protocol
**
**      argv            retained only for conformance with calling
**                        protocol
**
**  INPUTS/OUTPUTS:     none
**      
**
**  OUTPUTS:            an rpc status code
**
**      status
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_if_id_vector_free (argc, argv, status)

unsigned32 argc;
char *argv[];
unsigned32              *status;

{
    rpc_if_id_vector_p_t    if_id_vector;
    unsigned32              if_count = 1;
    unsigned32              i;
    
    
    if (verbose != 0) fprintf (stdout, "Enter - Test %s\n\n", argv[0]);
    
    /*
     * get the count of interface id's to allocate from
     */
    if_count = (unsigned32) atoi ( argv[1] );

    /*
     * allocate space for a vector to hold the if id's
     */
    if_id_vector = (rpc_if_id_vector_p_t ) malloc ((sizeof (if_count) +
                    (if_count * sizeof (rpc_if_id_p_t))));

    if (if_id_vector == NULL)
    {
        *status = rpc_s_no_memory;
        return COMTEST_FAILED;
    }
    
    /*
     * set the count field in the vector
     */
    (if_id_vector)->count = if_count;
    
    /*
     * allocate if_count number of rpc_if_id_t and link
     * with the interface id vector
     */
    for (i = 0; i < if_count; i++)
    {
        /*
         * allocate memory to hold the if id
         */
        (if_id_vector)->if_id[i] =
                (rpc_if_id_p_t) malloc (sizeof (rpc_if_id_t));
        
        
        /*
         * if it failed try to delete it and return
         */
        if ((if_id_vector)->if_id[i] == NULL)
        {
            rpc_if_id_vector_free (&if_id_vector, status);
            *status = rpc_s_no_memory;
            return COMTEST_FAILED;
        }
        else
        {      /*
                *  allocated ok -  insert preallocated interface id
                */  
            *((if_id_vector)->if_id[i]) = COMTST_const_if_id;
        }        
    }
    
    /*
     * Now go test rpc_if_id_vector_free to deallocate the interface if
     */

    rpc_if_id_vector_free (&if_id_vector, status);

    /*
     *  call to rpc_if_id_vector_free failed
     */
    if (*status != rpc_s_ok)
    {
        return RPC_CALL_FAILED;
    }
    /*
     * test succeeded
     */
    return COMTEST_OK;
}


/*
**++
**
**  ROUTINE NAME:       comtest_if_assemb_args
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**  Take routine takes a series of asci-string args,
**  passed by the argv, argc arguments.
**  These are assumed to be in the correct order and
**  converts each argument into the proper format and
**  inserts it into the interface spec pointed to by
**  argument, loc_if_rep_pt.  The input argument strings
**  must be in input in the following order:
**         argument               string format
**  ------------------------------------------------
**    1.  ifspec rep version            integer
**    2.  op count                      integer
**    3.  if version                    integer
**    4.  id                   asci uuid string
**    5.  stub/rt if version            integer
**
**      
**
**  INPUTS:             
**
**      argc            count of arg pointers passwd by argv
**
**      argv            standard 'argv' array of pointers to null
**                      terminated ascii argument strings
**
**      loc_if_rep_pt   pointer to an rpc_if_rep_t which is to be
**                      initialized by arguments passed in 'argc',
**                      'argv'.
**
**
**  INPUTS/OUTPUTS:     none
**
**      
**
**  OUTPUTS:            an rpc status code
**
**      status
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_if_assemb_args (argc, argv, loc_if_rep_pt, status)

unsigned32      argc;
char            *argv[];
rpc_if_rep_p_t  loc_if_rep_pt;
unsigned32      *status;

{
    unsigned32    arg_cnt;
    uuid_t        uuid;
    
    
    for ( arg_cnt = 0;  arg_cnt < argc;  arg_cnt++ )
    {
        switch ( arg_cnt )
        {
            case 0:
                break;
            case 1:
                loc_if_rep_pt->ifspec_vers =
                               (unsigned16) atoi ( argv[arg_cnt] );
                break;
            case 2:
                loc_if_rep_pt->opcnt =
                               (unsigned16) atoi ( argv[arg_cnt] );
                break;
            case 3:
                loc_if_rep_pt->vers =
                               (unsigned32) atoi ( argv[arg_cnt] );
                break;
            case 4:
                uuid_from_string (argv[arg_cnt], &uuid, status);
                if (*status == uuid_s_ok)
                {
                    loc_if_rep_pt->id = uuid;
                }
                else
                {
                    if (verbose != 0)
                    {
                        fprintf (stdout, "id - %s\n", argv[arg_cnt]);
                    }
                }
                break;
            case 5:
                loc_if_rep_pt->stub_rtl_if_vers =
                               (unsigned16) atoi ( argv[arg_cnt] );
                break;

            default:
            if (verbose != 0) 
            {
                    fprintf (stdout, "BAD ARG  - %s\n", argv[arg_cnt]);
            }
                break;
        }
    }
}

