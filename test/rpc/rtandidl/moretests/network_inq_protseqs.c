/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: network_inq_protseqs.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:01  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:43  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:05:39  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:37:56  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:36:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:08:23  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:35:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#include <prottest.h>   /* Externals for Common Services component  */
#include <tsh_common.h>
#include <network_inq_protseqs.h>


/*
 * List of valid protocol sequence string
 * compared by comtest_net_inq_protseq
 */
char *protseq_lst [] =
 {
        rpc_protseq_ncacn_ip_tcp,
        rpc_protseq_ncacn_dnet_nsp,
        rpc_protseq_ncacn_osi_nsp,
        rpc_protseq_ncacn_osi_tp4,
        rpc_protseq_ncadg_ip_udp,
        rpc_protseq_ncadg_dds,
        0
 };

#define PROTSEQ_INVALID     0x1
#define PROTSEQ_VALID       0x0

#define	ARGS_REQUIRED	3
           
ndr_long_int 
TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
	ndr_long_int subcase, estatus, cases;
	unsigned_char_p_t bound_string;
	ndr_long_int success = 0;
	ndr_long_int fail = 1;


	/**************************************************************************/
	/* Check the number of arguments passed in.  They should be exactly the   */
	/* required number when you come into this routine. Looking for a subcase,*/
	/* a bogus expected_status, and a hostname.                               */
	/**************************************************************************/
	if (argc !=  ARGS_REQUIRED) {
		TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
                                                               ARGS_REQUIRED, argc);
		return(TSH_BAD_ARG_COUNT);
	}

	subcase = atol(argv[0]);		 /* turn subcase string into an int.	  */
	estatus = atol(argv[1]);
	cases   = atol(argv[2]);

	return(network_inq_protseqs_test(ctx, subcase, estatus, cases));
}




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

int network_inq_protseqs_test (ctx, subcase, estatus, cases)
CtxBlock *	ctx;
int		subcase;
int		estatus;
int		cases;
{
    unsigned32              i, j, num_invalid, status;
    boolean32               ival;
    rpc_protseq_vector_p_t  psvp;


    /*
     * call rpc_network_inq_protseqs to obtain a vector
     * of valid protocol sequences supported by the
     * Common Communications Service.
     */     

	switch(subcase) {
    case NORMAL_TEST:
		TSHMessage (ctx, MOTRACE, "testing for NORMAL TEST status\n");
        rpc_network_inq_protseqs (&psvp, &status);
		if (cases == PROTSEQS_FREE)
   	 		if (TSHStatus(ctx,SETUP,"rpc call failed",status,rpc_s_ok)) {
			 	TSHMessage(ctx, MOWARNING,"rpc call failed, status: %d\n", status);
       			 return(TSH_TEST_FAILED);
            }
        else 
    		if (TSHStatus(ctx, TEST,"rpc call failed",status,estatus)) {
			 	TSHMessage(ctx, MOWARNING,"rpc call failed, status: %d\n", status);
       			 return(TSH_TEST_FAILED); 
			}


    /*
     * take each protocal sequence string returned
     * by rpc_network_inq_protseqs and scan through
     * a hard-coded list of valid protocol sequences 
     * count any strings for which no match found
     */
    num_invalid = 0;
    for (i = 0; i < psvp->count; i++)
    {
        /*
         * set invalid flag and make the scan
         */
       	TSHMessage(ctx, MOTRACE, "protocol_seqs[%d]: %s\n", i, psvp->protseq[i]);
        ival = PROTSEQ_INVALID;
        for (j = 0; *protseq_lst[j]; j++)
        {  
 
            if (!strcmp((char *)psvp->protseq[i],
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
				TSHStatus(ctx, TEST, "PROTOCOL_SEQ_INVLAID", FAILURE, SUCCESS);
				TSHMessage(ctx, MOWARNING, "PROTOCOL_SEQ_INVLAID: %s\n", psvp->protseq[i]);
                num_invalid++;
        }
    }

    /*
     * Now free the protocol sequence vector.
     */
    rpc_protseq_vector_free (&psvp, &status);

    if (status != rpc_s_ok)
    {         
		if (cases == PROTSEQS_FREE) 
	    	TSHStatus(ctx, TEST,"rpc call failed",FAILURE,SUCCESS); 
        else
	    	TSHStatus(ctx, CLNUP,"rpc call failed",FAILURE,SUCCESS);
   	 	TSHMessage(ctx, MOWARNING,"rpc call failed, status: %d\n", status);
        return(TSH_TEST_FAILED);
	}     

    /*
     * report - found some invalid protseq strings
     */
    if (num_invalid)
    {
 	    	TSHStatus(ctx, TEST,"protseqs_inq rpc call failed",FAILURE,SUCCESS);
   		 	TSHMessage(ctx, MOWARNING,"%d  INVALID PROTOCOL SEQUENCES FOUND\n", num_invalid);
            return(TSH_TEST_FAILED);
    }

    	break;
	case INVALID_TEST: /* NO_SUPPORT: can't be tested */
		TSHMessage (ctx, MOTRACE, "testing for no supported protocol sequences status\n");
        rpc_network_inq_protseqs (&psvp, &status);
    	TSHStatus(ctx, TEST,"rpc call failed",status,rpc_s_no_protseqs);
    
		break;
	}
	return(SUCCESS);

}
 


