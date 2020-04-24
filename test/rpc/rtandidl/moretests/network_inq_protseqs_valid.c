/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: network_inq_protseqs_valid.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:03  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:06:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:46  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:05:45  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:38:15  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:36:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:08:37  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:35:43  devrcs
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



/*#include <prottest.h>        /* Externals for Common Services component  */
/*#include <tstdriver.h>  /* Local deffinitions and calling protocol 
                         * for test driver */
/*#include <comtest.h>    /* definitions for communications tests    */
#include <tsh_common.h>
#include <network_inq_protseqs.h>

/*
#define protseq_ncacn_ip_tcp        "cacn_ip_tcp"
#define protseq_ncacn_dnet_nsp      "cacn_dnet_nsp"
#define protseq_ncacn_osi_nsp       "cacn_osi_nsp"
#define protseq_ncacn_osi_tp4       "cacn_osi_tp4"
#define protseq_ncadg_ip_udp        "cadg_ip_udp"
#define protseq_ncadg_dds           "cadg_dds"
*/

/*
 * List of valid protocol sequence string
 * compared by comtest_net_inq_protseq
 */ 
/*
char *protseq_lst [] =
 {
        protseq_ncacn_ip_tcp,
        protseq_ncacn_dnet_nsp,
        protseq_ncacn_osi_nsp,
        protseq_ncacn_osi_tp4,
        protseq_ncadg_ip_udp,
        protseq_ncadg_dds,
        0
 };
*/

#define	ARGS_REQUIRED	4
           
ndr_long_int 
TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
	ndr_long_int subcase;
	ndr_long_int estatus;
	boolean32	 eval;
	unsigned_char_t	protseqs;
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
	eval    = (strcmp(argv[2], "1"))? false : true;
	
	/* ignore argv[1] expected status value, */
	/* each test does its own error checking.*/
     
	return(network_inq_protseqs_valid_test(ctx, subcase, estatus, eval, argv[3]));
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

int network_inq_protseqs_valid_test (ctx, subcase, estatus, eval, protseq)
CtxBlock *	ctx;
int		subcase;
int		estatus;
boolean32 eval;
unsigned_char_p_t protseq;
{
    unsigned32              i, j, num_invalid, num_valid, status;
    boolean32               ival;
    rpc_protseq_vector_p_t  psvp;

    /*
     * call rpc_network_inq_protseqs to obtain a vector
     * of valid protocol sequences supported by the
     * Common Communications Service.
     */     

	switch(subcase) {
    case NORMAL_TEST:
		TSHMessage (ctx, MOTRACE, "testing for valid test status\n");  
		if (!strcmp((char *)protseq, "0")) {
            rpc_network_inq_protseqs (&psvp, &status);
			TSHMessage (ctx, MOTRACE, "testing for protocol sequence returnd from rpc call\n");  
		    if (TSHStatus(ctx, SETUP,"rpc call failed",status,rpc_s_ok)) {
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
                      
    	    rpc_network_is_protseq_valid (psvp->protseq[i], &status);
    		TSHMessage(ctx, MOTRACE, "protocol_seqs[%d]: %s, status: %d\n", i, psvp->protseq[i], status);
        
			if (TSHStatus(ctx, TEST, "PROTOCOL_SEQ_INVLAID", estatus, status)){
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
	    	TSHStatus(ctx, CLNUP,"protseqs_free call failed",FAILURE,SUCCESS);
   		 	TSHMessage(ctx, MOWARNING,"protseqs_free call failed, status: %d\n", status);
  	        return(TSH_TEST_FAILED);  
			}

    /*
     * report - found some invalid protseq strings
     */
  		  if (num_invalid)
  		  {
 	    	TSHStatus(ctx, TEST,"protseqs_valid rpc call failed",FAILURE,SUCCESS);
   		 	TSHMessage(ctx, MOWARNING,"%d  INVALID PROTOCOL SEQUENCES FOUND\n", num_invalid);
            return(TSH_TEST_FAILED);
	       } 
	}   
	else {
     		TSHMessage (ctx, MOTRACE, "testing for protocol sequence from input\n");  
    	    ival = rpc_network_is_protseq_valid (protseq, &status);
    		TSHMessage(ctx, MOTRACE, "protocol_seqs: %s\n", protseq);
        
			if (TSHStatus(ctx, TEST, "PROTOCOL_SEQ_INVLAID", estatus, status)) {
				TSHMessage(ctx, MOWARNING, "status of PROTOCOL_SEQ_INVLAID (%s): %d\n", protseq, status); 
				return(TSH_TEST_FAILED); 
			}  
			if (TSHStatus(ctx, TEST, "PROTOCOL_SEQ_INVLAID", eval, ival)) {
				TSHMessage(ctx, MOWARNING, "Valid status of PROTOCOL_SEQ_INVLAID (%s): %d\n", protseq, ival); 
				return(TSH_TEST_FAILED); 
			}



   	   } 



    	break; 

	case INVALID_TEST:
		TSHMessage (ctx, MOTRACE, "testing for invalid protocol sequence\n");
        ival = rpc_network_is_protseq_valid (protseq, &status);
      	TSHMessage(ctx, MOTRACE, "status of invalid protocol_seqs(%s) : %d \n",  protseq, status);
             
  		if (TSHStatus(ctx, TEST, "PROTOCOL_SEQ_INVALID", estatus, status))  {
   	   		TSHMessage(ctx, MOWARNING, "PROTOCOL_SEQ_VALID: %s \n", protseq);
     	  	 return(TSH_TEST_FAILED);
		} 
	}

	return(SUCCESS);

}
 


