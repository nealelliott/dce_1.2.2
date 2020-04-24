/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: twr_osi.c,v $
 * Revision 1.1.319.2  1996/02/18  00:05:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:30  marty]
 *
 * Revision 1.1.319.1  1995/12/08  00:22:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:35 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:21 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/08  00:00:39  root]
 * 
 * Revision 1.1.317.1  1994/01/21  22:39:26  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  22:00:09  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:55:42  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:13:42  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:18:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:45:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:11:47  devrcs
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
**      twr_osi.c
**
**  FACILITY:
**
**      Protocol Tower Services for the osi address family
**
**  ABSTRACT:
**
**      The protocol tower service provides routines that:
**      
**      o  convert a socket address to the lower floors of 
**         a protocol tower 
**
**      o  convert a protocol tower to a socket address
**
**
*/

#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* Common communications services */

#ifdef NAF_OSI
#include <twrp.h>	/* Private tower services */

/*
 *  Include the Internet specific socket address
 */
#ifdef unix
#include <netosi/osi.h>
#else
#include <osi.h>
#endif

/*
**++
**
**  ROUTINE NAME:       twr_osi_lower_flrs_from_sa
**
**  SCOPE:              PUBLIC - declared in twr.idl
**
**  DESCRIPTION:
**
**  Creates the canonical representation of an OSI tower's lower floors 
**  from a sockadddr. The canonical form can be transmitted on the 
**  wire, or included in a DNS tower.
**
**  INPUTS:
**
**      sa              OSI-specific socket address data structure.
**                      The supported address families are:
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      lower_flrs      Returns the lower tower floors in a twr_t 
**                      structure (includes the floor count in the 
**                      "tower_octet_string" member).
**
**      status          A value indicating the return status of the routine:
**                          twr_s_ok
**                          twr_s_unknown_sa
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
**/

PUBLIC void twr_osi_lower_flrs_from_sa 
#ifdef _DCE_PROTO_
(
    sockaddr_p_t      sa,
    twr_p_t           *lower_flrs,
    unsigned32        *status
)
#else
(sa, lower_flrs, status)
sockaddr_p_t      sa;
twr_p_t           *lower_flrs;
unsigned32        *status;
#endif
{
    unsigned8   protocol_id[TWR_C_NUM_OSI_LOWER_FLRS];
    unsigned16  id_size = twr_c_tower_prot_id_size,
                floor_count,
                related_data_size[TWR_C_NUM_OSI_LOWER_FLRS];
    unsigned32  count,
                twr_t_length,
                *related_data_ptr[TWR_C_NUM_OSI_LOWER_FLRS];
    byte_p_t    tmp_tower;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    if (sa->family == RPC_C_NAF_ID_OSI)
    {
        protocol_id[0] = TWR_C_FLR_PROT_ID_OSI;
        
        floor_count = TWR_C_NUM_OSI_LOWER_FLRS;
        
        /* 
         * here add code to setup related_data_size and related_data_ptr
         * fields when you add support for OSI.
         */
        
        *status = twr_s_not_implemented;
        return;
    }
    else
    {
        *status = twr_s_unknown_sa;
        return;
    }

#if 0
	REMOVE #ifdef WHEN OSI SUPPORT IS IMPLEMENTED
    /*  
     * Allocate memory and copy the data into the proper
     * locations in each of up to three floors.
     */
    
    twr_t_length = twr_c_tower_flr_count_size;  /* to store floor count */
    
    for ( count = 0; count < floor_count; count++ )
    {
        twr_t_length += TWR_C_FLR_OVERHEAD;
        twr_t_length += related_data_size[count];
    }
    
    /* 
     * Next allocate space for the tower structures
     */
    RPC_MEM_ALLOC (
        *lower_flrs,
        twr_p_t,
        sizeof (twr_t) + (twr_t_length - 1),
        twr_t_length,
        RPC_C_MEM_TOWER,
        RPC_C_MEM_WAITOK );
    if (*lower_flrs == NULL){
	*status = rps_s_no_memory;
	return;
    }
    
    /* 
     * Copy the length of the tower octet string into the tower structure
     */
    (*lower_flrs)->tower_length = twr_t_length;

    /* 
     * Copy the floor information into the tower octet string 
     */

    /*
     * Use a temporary for the octet string since we need
     * to increment the pointer.
     */
    tmp_tower = (*lower_flrs)->tower_octet_string;

    
    /* 
     * Copy the number of floors into the tower octet string 
     */
    twr_rep_16 = floor_count;
    RPC_RESOLVE_ENDIAN_INT16 (twr_rep_16);
    memcpy ((char *)tmp_tower, (char *)&twr_rep_16, 
            twr_c_tower_flr_count_size);

    tmp_tower += twr_c_tower_flr_count_size;

    /*
     * Convert the protocol identifier size to its proper
     * representation for use in the following loop.
     */
    RPC_RESOLVE_ENDIAN_INT16 (id_size);
    
    for ( count = 0; count < floor_count; count++ )
    {
        /* 
         * Copy the length of the protocol identifier field into 
         * tower octet string.  (Converted before the loop.)
         */
        memcpy ((char *)tmp_tower, (char *)&id_size, 
                TWR_C_TOWER_FLR_LHS_COUNT_SIZE);

        tmp_tower += TWR_C_TOWER_FLR_LHS_COUNT_SIZE;

        /* 
         * Copy the protocol identifier into tower octet string
         * (1 byte so no need to convert endian representation).
         */
        memcpy ((char *)tmp_tower, (char *)&(protocol_id[count]),
                twr_c_tower_prot_id_size);

        tmp_tower += twr_c_tower_prot_id_size;

        /* 
         * Copy the length of the address data field into 
         * tower octet string.
         */
        twr_rep_16 = related_data_size[count];
        RPC_RESOLVE_ENDIAN_INT16 (twr_rep_16);
        memcpy ((char *)tmp_tower, (char *)&twr_rep_16, 
                TWR_C_TOWER_FLR_RHS_COUNT_SIZE);
 
        tmp_tower += TWR_C_TOWER_FLR_RHS_COUNT_SIZE;

        /* 
         * If there is addressing data, copy the address data field 
         * into tower octet string.
         */
        if ( related_data_size[count] )
        {
            memcpy ((char *)tmp_tower, 
                    (char *)related_data_ptr[count], 
                    related_data_size[count]);
            
            /*
             * Set up for the next floor.
             */
            tmp_tower += related_data_size[count];
        }
    }

    *status = twr_s_ok;
#endif /* 0 */
}

/*
**++
**
**  ROUTINE NAME:       twr_osi_lower_flrs_to_sa 
**
**  SCOPE:              PUBLIC - declared in twr.idl
**
**  DESCRIPTION:
**
**  Creates an OSI sockaddr from the canonical representation of an OSI
**  protocol tower's lower floors.
**
**  INPUTS:
**
**      tower_octet_string
**                      The protocol tower to convert to a sockaddr.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      sa            Returns a pointer to the created sockaddr structure.
**      
**      sa_len          Returns the length, in bytes, of the returned 
**                      "sa" argument.
**
**      status          A value indicating the return status of the routine:
**                          twr_s_ok
**                          twr_s_unknown_tower
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
**/

PUBLIC void twr_osi_lower_flrs_to_sa 
#ifdef _DCE_PROTO_
(
    byte_p_t          tower_octet_string, 
    sockaddr_p_t      *sa,
    unsigned32        *sa_len,
    unsigned32        *status
)
#else
(tower_octet_string, sa, sa_len, status)
byte_p_t          tower_octet_string; 
sockaddr_p_t      *sa;
unsigned32        *sa_len;
unsigned32        *status;
#endif
{
    unsigned8   id;
    byte_p_t    tower;
    unsigned16  count;
                floor_count,
                id_size,
                addr_size;
    unsigned32  length;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    *status = twr_s_not_implemented;
}
#else 
static int _naf_osi_dummy_ = 0, * _naf_osi_dummy_p_ = &_naf_osi_dummy_ ;
#endif /* NAF_OSI */
