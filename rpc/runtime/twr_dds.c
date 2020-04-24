/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: twr_dds.c,v $
 * Revision 1.1.316.2  1996/02/18  00:05:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:27  marty]
 *
 * Revision 1.1.316.1  1995/12/08  00:22:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:35 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:20 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/08  00:00:36  root]
 * 
 * Revision 1.1.314.1  1994/01/21  22:39:22  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  22:00:06  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:55:35  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:13:29  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:17:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:45:09  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:08:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifdef NAF_DDS
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      twr_dds.c
**
**  FACILITY:
**
**      Protocol Tower Services for the dds address family
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
#include <twrp.h>	    /* Private tower services */
#include <comtwrflr.h>  /* Tower floor services */

        
/* ========================================================================= */

/* NB:  these structs should probably should be moved to a dds.h file used by 
 * /dds/dds.c and twr_dds.c
 */

struct dds_addr {
    long        network;
    long        node;
};

struct sockaddr_dds {
    short           sdds_family;
    short           sdds_port;
    struct dds_addr sdds_addr;
};

/*
 *  Number of bytes in dds_addr fields
 */
#define DDS_ADDR_C_PORT_SIZE      2
#define DDS_ADDR_C_NETWORK_SIZE   4
#define DDS_ADDR_C_NODE_SIZE      4


INTERNAL boolean32 chk_twr_dds_protocol_id _DCE_PROTOTYPE_((
        byte_p_t    * /*tower*/,
        unsigned32  /*floor*/
    ));

/* ======================================================================== */


/*
**++
**
**  ROUTINE NAME:       twr_dds_lower_flrs_from_sa
**
**  SCOPE:              PUBLIC - declared in twr.idl
**
**  DESCRIPTION:
**
**  Creates the canonical representation of a dds protocol tower's 
**  lower floors from a sockadddr. The canonical form can be transmitted 
**  on the wire, or included in a DNS tower.
**
**  INPUTS:
**
**      trans_prot      Integer value specifying the transport layer 
**                      protocol for the dds address family. 
**                      For address family RPC_C_NAF_ID_DDS specify:
**
**                         RPC_C_NETWORK_PROTOCOL_ID_DDS
**
**      sa              dds-specific socket address data 
**                      structure.
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

PUBLIC void twr_dds_lower_flrs_from_sa 
#ifdef _DCE_PROTO_
(
    unsigned32        trans_prot,
    sockaddr_p_t      sa,
    twr_p_t           *lower_flrs,
    unsigned32        *status
)
#else
(trans_prot, sa, lower_flrs, status)
unsigned32        trans_prot;
sockaddr_p_t      sa;
twr_p_t           *lower_flrs;
unsigned32        *status;
#endif
{
    unsigned16  id_size,
                floor_count,
                related_data_size[TWR_C_NUM_DDS_LOWER_FLRS],
                twr_rep_16;
    unsigned32  count,
                twr_length;
    byte_p_t    related_data_ptr[TWR_C_NUM_DDS_LOWER_FLRS];
    idl_byte    dds_addr_str[DDS_ADDR_C_NETWORK_SIZE + DDS_ADDR_C_NODE_SIZE];
    byte_p_t    dap;
    byte_p_t    tower;
    idl_byte    prot_id_prefix = RPC_C_PROT_ID_PREFIX;
    uuid_t      tower_uuid;
    static uuid_t  twr_dds_protocol_id[TWR_C_NUM_DDS_LOWER_FLRS] =
    {
        {0x9865a080, 0xbb73, 0x11c9, 0x96, 0x3c, 
            {0x08,0x00, 0x2b, 0x13, 0xec, 0x4e}},
        {0x9b86b6a0, 0xbb73, 0x11c9, 0xb8, 0x89, 
            {0x08, 0x00, 0x2b, 0x13, 0xec, 0x4e}}
    };

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    /*
     * all depends on the network family to which this socket belongs
     */
    if (sa->family != RPC_C_NAF_ID_DDS)
    {
        *status = twr_s_unknown_sa;
        return;
    }

    /*
     * Since we now know the socket address we're dealing with, 
     * collect the sizes of each field to allocate memory, and 
     * remember the pointers to the fields so we can copy the 
     * data once we have allocated the tower string.
     */
    floor_count = TWR_C_NUM_DDS_LOWER_FLRS;
            
    related_data_size[0] = DDS_ADDR_C_PORT_SIZE;

    related_data_ptr[0] = 
        (byte_p_t) (&((struct sockaddr_dds *)sa)->sdds_port);
            
    related_data_size[1] = DDS_ADDR_C_NETWORK_SIZE + DDS_ADDR_C_NODE_SIZE;
            
    dap = dds_addr_str;
    memcpy((char *) dap, (char *) (&((struct sockaddr_dds *)sa)->sdds_addr.network), 
        DDS_ADDR_C_NETWORK_SIZE);
    dap += DDS_ADDR_C_NETWORK_SIZE;
    memcpy((char *) dap, (char *) (&((struct sockaddr_dds *)sa)->sdds_addr.node),
        DDS_ADDR_C_NODE_SIZE);
    related_data_ptr[1] = dds_addr_str;


    /*  
     * Calculate the length of the tower floors.
     */
     
    twr_length = twr_c_tower_flr_count_size;  /* to store floor count */

    for ( count = 0; count < floor_count; count++ )
    {
        twr_length += (TWR_C_FLR_OVERHEAD + TWR_C_TOWER_UUID_SIZE);
        twr_length += related_data_size[count];
    }

    /* 
     * Next allocate space for the tower structure
     */
    RPC_MEM_ALLOC (
        *lower_flrs,
        twr_p_t,
        sizeof (twr_t) + (twr_length - 1),
        RPC_C_MEM_TOWER,
        RPC_C_MEM_WAITOK );
    if (*lower_flrs == NULL){
	*status = rpc_s_no_memory;
	return;
    }

    /* 
     * Copy the length of the tower octet string into the tower structure
     */
    (*lower_flrs)->tower_length = twr_length;
    
    /* 
     * Copy the floor information into the tower octet string 
     */

    /*
     * Use a temporary for the octet string since we need
     * to increment the pointer.
     */
    tower = (*lower_flrs)->tower_octet_string;

    /* 
     * Copy the number of floors into the tower octet string 
     */
    twr_rep_16 = floor_count;
    RPC_RESOLVE_ENDIAN_INT16 (twr_rep_16);
    memcpy ((char *)tower, (char *)&twr_rep_16, 
            twr_c_tower_flr_count_size);

    tower += twr_c_tower_flr_count_size;

    /* 
     * Convert the protocol identifier size to its proper
     * representation for use in the following loop.
     */
    id_size = twr_c_tower_prot_id_size + TWR_C_TOWER_UUID_SIZE;
    RPC_RESOLVE_ENDIAN_INT16 (id_size);

    for ( count = 0; count < floor_count; count++ )
    {
        /* 
         * Copy the length of the protocol identifier field into 
         * tower octet string.  (Converted before the loop.)
         */
        memcpy ((char *)tower, (char *)&id_size, 
                TWR_C_TOWER_FLR_LHS_COUNT_SIZE);

        tower += TWR_C_TOWER_FLR_LHS_COUNT_SIZE;

        /* 
         * Copy the protocol identifier prefix into the tower octet string.
         * (1 byte so no resolve_endian needed)  
         */
        memcpy ((char *)tower, (char *)(&prot_id_prefix),
                twr_c_tower_prot_id_size);

        tower += twr_c_tower_prot_id_size;

        /*
         * Convert the UUID of interest to little endian and copy it.
         */
        tower_uuid = twr_dds_protocol_id[count];
        RPC_RESOLVE_ENDIAN_UUID (tower_uuid);
    
        memcpy ((char *)tower, (char *)&tower_uuid, TWR_C_TOWER_UUID_SIZE);
        tower += TWR_C_TOWER_UUID_SIZE;

        /* 
         * Copy the length of the address data field into 
         * tower octet string.
         */
        twr_rep_16 = related_data_size[count];
        RPC_RESOLVE_ENDIAN_INT16 (twr_rep_16);
        memcpy ((char *)tower, (char *)&twr_rep_16,
                TWR_C_TOWER_FLR_RHS_COUNT_SIZE);

        tower += TWR_C_TOWER_FLR_RHS_COUNT_SIZE;

        /* 
         * If there is addressing data, copy the address data field into 
         * tower octet string
         */
        if (related_data_size[count])
        {
            memcpy ((char *)tower, (char *)related_data_ptr[count], 
                    related_data_size[count]);

            /*
             * Set up for the next floor.
             */
            tower += related_data_size[count];
        }
    }

    *status = twr_s_ok;
}

/*
**++
**
**  ROUTINE NAME:       twr_ip_lower_flrs_to_sa 
**
**  SCOPE:              PUBLIC - declared in twr.idl
**
**  DESCRIPTION:
**
**  Creates an internet sockaddr from the canonical representation of an 
**  internet protocol tower's lower floors.
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

PUBLIC void twr_dds_lower_flrs_to_sa 
#ifdef _DCE_PROTO_
(
    byte_p_t          tower_octet_string, 
    sockaddr_p_t      *sa,
    unsigned32        *sa_len,
    unsigned32        *status
)
#else
( tower_octet_string, sa, sa_len, status )
byte_p_t          tower_octet_string; 
sockaddr_p_t      *sa;
unsigned32        *sa_len;
unsigned32        *status;
#endif
{
    byte_p_t    tower;
    unsigned16  count,
                floor_count,
                addr_size;
    unsigned32  length;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    /* 
     * Make sure we have a pointer to some data structure.
     */
    if ( !(tower = tower_octet_string)) 
    {
        *status = twr_s_unknown_tower;
        return;
    }

    /* 
     * Get the tower floor count 
     */
    memcpy ((char *)&floor_count, (char *)tower, twr_c_tower_flr_count_size);
    RPC_RESOLVE_ENDIAN_INT16 (floor_count);

    tower += twr_c_tower_flr_count_size;

    /*
     * Skip over the (application's) upper floors while we look for the 
     * beginning of the dds-specific lower floors.
     */

    /*
     * For now, assume we don't find the floors we're looking for.
     */
    *status = twr_s_unknown_tower;

    for ( count = 0; count < floor_count; count++ )
    {
        if (chk_twr_dds_protocol_id(&tower, 0))
        {
            *status = twr_s_ok;
            break;
        }

        memcpy ((char *)&addr_size, (char *) tower, TWR_C_TOWER_FLR_RHS_COUNT_SIZE);
        RPC_RESOLVE_ENDIAN_INT16 (addr_size);

        tower += (TWR_C_TOWER_FLR_RHS_COUNT_SIZE + addr_size);
    }

    /*
     *  If didn't find DDS floors, error return
     */
    if (*status != twr_s_ok)
    {
        return;
    }

    /* 
     * Allocate space for DDS sockaddr
     */
    length = sizeof(struct sockaddr_dds);
                    
    RPC_MEM_ALLOC (
        *sa,
        sockaddr_p_t,
        length,
        RPC_C_MEM_SOCKADDR,
        RPC_C_MEM_WAITOK );
    if (*sa == NULL){
	*status = rpc_s_no_memory;
	return;
    }
    
    *sa_len = length;
                    
    /*
     * make sure unused bytes are null 
     */
    memset ((char *) *sa, 0, length);
                    
    /* 
     * define this as an dds family socket
     */
    ((struct sockaddr_dds *)(*sa))->sdds_family = RPC_C_NAF_ID_DDS;
                    
    /* 
     * Get the length of dds port
     */
    memcpy ((char *)&addr_size, (char *)tower, RPC_C_TOWER_FLR_RHS_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (addr_size);
    tower += RPC_C_TOWER_FLR_RHS_COUNT_SIZE;

    if (addr_size != DDS_ADDR_C_PORT_SIZE)
    {
        *status = rpc_s_invalid_rpc_floor;
        return;
    }

    /*
     * Copy the port number to the sockaddr.
     */
    memcpy ( &((struct sockaddr_dds *)(*sa))->sdds_port, tower, addr_size);
                    
    tower += addr_size;
                    
    /*
     *  Check that next floor is DDS network service
     */
    if (! chk_twr_dds_protocol_id(&tower, 1))
    {
        *status = twr_s_unknown_tower;
        return;
    }

    /* 
     * Get the length of DDS network address
     */
    memcpy ((char *)&addr_size, (char *)tower, RPC_C_TOWER_FLR_RHS_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (addr_size);
    tower += RPC_C_TOWER_FLR_RHS_COUNT_SIZE;

    if (addr_size != (DDS_ADDR_C_NETWORK_SIZE + DDS_ADDR_C_NODE_SIZE))
    {
        *status = rpc_s_invalid_rpc_floor;
        return;
    }

    /*
     * Copy the host network and node address to the sockaddr
     */
    memcpy (&((struct sockaddr_dds *)(*sa))->sdds_addr.network,
            (char *)tower, DDS_ADDR_C_NETWORK_SIZE);
    tower += DDS_ADDR_C_NETWORK_SIZE;
                    
    memcpy (&((struct sockaddr_dds *)(*sa))->sdds_addr.node,
            (char *)tower, DDS_ADDR_C_NODE_SIZE);
    tower += DDS_ADDR_C_NODE_SIZE;
                    
    *status = twr_s_ok;
}

/*
 *  Return true if lhs of tower's current contains a 
 *  DDS protocol id, false otherwise.
 *  Move tower ptr to rhs of current floor
 */

INTERNAL boolean32 chk_twr_dds_protocol_id
#ifdef _DCE_PROTO_
(
    byte_p_t    *tower,
    unsigned32  floor
)
#else
(tower, floor)
byte_p_t    *tower;
unsigned32  floor;
#endif
{
    boolean32       twr_dds_protid_found;
    byte_p_t        tp;
    unsigned8       id;
    unsigned16      id_size;
    uuid_t          tower_uuid;
    error_status_t  tmp_st;
    static uuid_t  twr_dds_protocol_id[TWR_C_NUM_DDS_LOWER_FLRS] =
    {
        {0x9865a080, 0xbb73, 0x11c9, 0x96, 0x3c, 
            {0x08,0x00, 0x2b, 0x13, 0xec, 0x4e}},
        {0x9b86b6a0, 0xbb73, 0x11c9, 0xb8, 0x89, 
            {0x08, 0x00, 0x2b, 0x13, 0xec, 0x4e}}
    };

    twr_dds_protid_found = false;
    tp = *tower;
                
    /*
     * Get the length of this floor's protocol id field 
     */
    memcpy ((char *)&id_size, (char *)tp,
            TWR_C_TOWER_FLR_LHS_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (id_size);

    tp += TWR_C_TOWER_FLR_LHS_COUNT_SIZE;

    /*
     * Get the protocol id prefix (don't advance the pointer).
     * Expect one byte; no need to convert.
     */
    memcpy ((char *)&id, (char *)tp, twr_c_tower_prot_id_size);

    /*
     * See if the protocol id is DDS protocol id for floor
     */
    if ( (id_size == (twr_c_tower_prot_id_size + TWR_C_TOWER_UUID_SIZE)) && 
         (id == RPC_C_PROT_ID_PREFIX) )
    {
        /*  Get possible DDS protocol uuid
         */
        memcpy((char *)&tower_uuid, (char *) (tp + twr_c_tower_prot_id_size), 
            TWR_C_TOWER_UUID_SIZE);
        RPC_RESOLVE_ENDIAN_UUID (tower_uuid);

        if (uuid_equal(&tower_uuid, &twr_dds_protocol_id[floor], &tmp_st))
        {
            /* 
             *  DDS protocol id
             */
            twr_dds_protid_found = true;
        }
    }

    /*
     * Move tower ptr to rhs of floor
     */
    tp += id_size;
    *tower = tp;

    return(twr_dds_protid_found);
}
#else 
static int _naf_dds_dummy_ = 0 ;
#endif /*NAF_DDS*/
