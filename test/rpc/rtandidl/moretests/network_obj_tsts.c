/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: network_obj_tsts.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:06  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:47  root]
 * 
 * Revision 1.1.2.2  1993/01/11  23:36:29  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:08:42  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:45  devrcs
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
**      network_obj_tsts.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**  
**  This module contains two tests for verification
**  of the network communications objects.
**
**     1. comtest_net_inq_protseq
**     2. comtest_net_is_protseq_valid
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  05-feb-91 pinkoski  replace osi protseqs with dnasession protseq
**  13-nov-90 burton    fixed return definitions
**  07-nov-90 woodbury  implement use of verbose flag
**                      remove reference to test protocol 
**                      (rpc_protseq_ncatp_ip_tcp)
**                      de-TABBED
**  15-oct-90 burton    original
**
**  %a%private_end  
**
*/


#include <dce/rpc.h>    /* Common internals for RPC Runtime system  */
#include <tstdriver.h>  /* Local deffinitions and calling protocol 
                         * for test driver */
#include <comtest.h>    /* definitions for communications tests    */

extern int verbose;
/*
 * NOTE
 * ==================================================
 * Most of the following will be transfered elsewhere
 * once we conclude where.
 * ==================================================
 */

/*
 * Signitures for routines in this test module
 */
int comtest_net_inq_protseq ();
int comtest_net_is_protseq_valid ();

/*
 * These definitoins are taken straight from com.h so 
 * this can build as a user application.
 */
#define rpc_protseq_ncacn_ip_tcp        "ncacn_ip_tcp"
#define rpc_protseq_ncacn_dnet_nsp      "ncacn_dnet_nsp"
#define rpc_protseq_ncadg_ip_udp        "ncadg_ip_udp"
#define rpc_protseq_ncadg_dds           "ncadg_dds"

/*
 * List of valid protocol sequence string
 * compared by comtest_net_inq_protseq
 */
char *protseq_lst [] =
 {
        rpc_protseq_ncacn_ip_tcp,
        rpc_protseq_ncacn_dnet_nsp,
        rpc_protseq_ncadg_ip_udp,
        rpc_protseq_ncadg_dds,
        0
 };

#define PROTSEQ_INVALID     0x1
#define PROTSEQ_VALID       0x0


/*
**++
**
**  ROUTINE NAME:       comtest_net_inq_protseq
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**  Protocol sequence vector test.  RPC routines exercised:
**
**        rpc_network_inq_protseqs
**
**        rpc_protseq_vector_free
**  
**   First,  rpc_network_inq_protseqs is called to test
**   the ability of this routine to allocate space for a
**   protocol sequence vector. If this succeeds, a search
**   made through a table of protocol sequence strings.
**   The search is made for each string returned by
**   rpc_network_inq_protseqs.  If no match is found for
**   any string it is reported to be invalid.
**   Finally,  rpc_protseq_vector_free is called to exercise
**   the system's ability to deallocate the space for the
**   protocol sequence vector obtained by previous calls
**   to rpc_network_inq_protseqs.
**      
**
**  INPUTS:             
**
**      none    all test parameters are obtained internally,
**              these argument falues are retained only for
**              calling consistency
**
**      argc    retained only for conformance with calling
**              protocol
**
**      argv    retained only for conformance with calling
**              protocol
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

comtest_net_inq_protseq (argc, argv, status)

unsigned32 argc;
char *argv[];
unsigned32              *status;

{
    unsigned32              i, j, num_invalid;
    boolean32               ival;
    rpc_protseq_vector_p_t  psvp;

    

    if (verbose != 0) fprintf (stdout, "Enter Test - %s\n\n", argv[0]);

    /*
     * call rpc_network_inq_protseqs to obtain a vector
     * of valid protocol sequences supported by the
     * Common Communications Service.
     */
    rpc_network_inq_protseqs (&psvp, status);

    if (*status != rpc_s_ok)
    {
        return RPC_CALL_FAILED;
    }

    /*
     * take each protocal sequence string returned
     * by rpc_network_inq_protseqs and scan through
     * a hard-coded list of valid protocol sequences 
     * cound any strings for which no match found
     */
    num_invalid = 0;
    for (i = 0; i < psvp->count; i++)
    {
        /*
         * set invalid flag and make the scan
         */
        ival = PROTSEQ_INVALID;
        for (j = 0; *protseq_lst[j]; j++)
        {
            if (!strcmp(psvp->protseq[i],
                            protseq_lst[j]))
            {
                /*
                 * found it
                 */
                ival = PROTSEQ_VALID;
                break;
            }
        }
        /*
         * report - end of list without a match
         */
        if ( ival == PROTSEQ_INVALID )
        {
                if (verbose != 0) fprintf(stdout, "PROTOCOL_SEQ_INVLAID - %s\n",
                psvp->protseq[i]);
                num_invalid++;
        }
    }

    /*
     * Now free the protocol sequence vector.
     */
    rpc_protseq_vector_free (&psvp, status);

    if (*status != rpc_s_ok)
    {
        return RPC_CALL_FAILED;
    }

    /*
     * report - found some invalid protseq strings
     */
    if (num_invalid)
    {
        if (verbose != 0)
        {
            fprintf(stdout,
                "%d  INVALID PROTOCOL SEQUENCES FOUND\n", num_invalid);
        }
        *status = -1;
    }
    else
    {
        /*
         * everything ok
         */
        if (verbose != 0) 
        {
            fprintf (stdout, "Test - %s - Success\n\n", argv[0]);
        }
        *status = rpc_s_ok;
        return COMTEST_OK;
    }
}

/*
**++
**
**  ROUTINE NAME:       comtest_net_is_protseq_valid
**
**  SCOPE:              global - defined in tstdriver.h 
**
**  DESCRIPTION:
**
**  Protocol sequence string validity test.  RPC routines exercised:
**
**        rpc_network_inq_protseqs
**
**        rpc_network_is_protseq_valid
**
**        rpc_protseq_vector_free
**
**   First,  rpc_network_inq_protseqs is called to obtain a
**   protocol sequence vector containig a current list of
**   the network protocol sequence family strings known to
**   the RPC run time system; rpc_network_is_protseq_valid
**   is called to exercise the this routine's ability to
**   verify the validity of known protocol sequence strings.
**   Finally,  rpc_protseq_vector_free is called to exercise
**   the system's ability to deallocate the space for the
**   protocol sequence vector obtained by previous calls
**   to rpc_network_inq_protseqs.
**      
**
**  INPUTS:             
**
**      none    all test parameters are obtained internally,
**              these argument falues are retained only for
**              calling consistency
**
**      argc    retained only for conformance with calling
**              protocol
**
**      argv    retained only for conformance with calling
**              protocol
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

comtest_net_is_protseq_valid (argc, argv, status)

unsigned32 argc;
char *argv[];
unsigned32              *status;

{
    unsigned32                  i, num_invalid;
    rpc_protseq_vector_p_t      psvp;

    

    if (verbose != 0) fprintf (stdout, "Enter - Test %s\n\n", argv[0]);

    /*
     * Get a vector of valid protocol sequences supported by the
     * Common Communications Service.
     */
    rpc_network_inq_protseqs (&psvp, status);

    if (*status != rpc_s_ok)
    {
        return RPC_CALL_FAILED;
    }

    /*
     * scan through protocol sequence vector and call
     * 'rpc_network_is_protseq_valid' for each protocol
     * sequence string to check its validity.
     * If an invalid string is found report error an
     * continue to search.
     */
    num_invalid = 0;
    for (i = 0; i < psvp->count; i++)
    
    {

        if (verbose != 0) 
        {
            fprintf (stdout, "(%d) - %s\n",(i + 1),
                  psvp->protseq[i]);
        }
        rpc_network_is_protseq_valid (psvp->protseq[i], status);
        if (*status != rpc_s_ok)
        {

            if (verbose != 0) fprintf(stdout, "PROTOCOL_SEQ_INVALID - %s\n",
            psvp->protseq[i]);
            num_invalid++;
        }
    }

    /*
     * Now release the protocol sequence vector.
     */
    rpc_protseq_vector_free (&psvp, status);

    if (*status != rpc_s_ok)
    {
        return RPC_CALL_FAILED;
    }


    /*
     * report - any invalid protseq strings
     */
    if (num_invalid)
    {

        if (verbose != 0) 
        {
            fprintf(stdout, "%d  INVALID PROTOCOL SEQUENCES FOUND\n", 
                num_invalid);
        }
        *status = -1;
        return COMTEST_FAILED;
    }
    else
    {
        /*
         * everything ok
         */
        if (verbose != 0) 
        {
            fprintf (stdout, "Test - %s - Success\n\n", argv[0]);
        }
        *status = rpc_s_ok;
        return COMTEST_OK;
    }

}
