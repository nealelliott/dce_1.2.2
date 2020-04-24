/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: twr_dnet.c,v $
 * Revision 1.1.317.2  1996/02/18  00:05:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:28  marty]
 *
 * Revision 1.1.317.1  1995/12/08  00:22:22  root
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
 * 	[1995/12/08  00:00:37  root]
 * 
 * Revision 1.1.315.1  1994/01/21  22:39:23  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  22:00:07  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:55:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:13:34  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:17:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:45:14  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:05:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifdef NAF_DNET
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      twr_dnet.c
**
**  FACILITY:
**
**      Protocol Tower Services for the DECnet address family
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
#include <twrp.h>	/* Private tower services */

        
/*
 *  Include the DECnet specific socket address
 */
#ifdef unix
#include <netdnet/dn.h>
#else
#include <dn.h>
#endif

/*
**++
**
**  ROUTINE NAME:       twr_dnet_lower_flrs_from_sa
**
**  SCOPE:              PUBLIC - declared in twr.idl
**
**  DESCRIPTION:
**
**  Creates the canonical representation of a DECnet protocol tower's 
**  lower floors from a sockadddr. The canonical form can be transmitted 
**  on the wire, or included in a DNA tower.
**
**  INPUTS:
**
**      sa              DECnet-specific socket address data structure.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      lower_flrs      Returns the lower tower floors in a twr_t 
**                      structure (includes the floor count as the first
**                      field of "tower_octet_string" member).
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

PUBLIC void twr_dnet_lower_flrs_from_sa 
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
    unsigned8   protocol_id[TWR_C_NUM_DNA_LOWER_FLRS];
    unsigned16  floor_count,
                id_size = twr_c_tower_prot_id_size,
                related_data_size[TWR_C_NUM_DNA_LOWER_FLRS],
                twr_rep_16;
    unsigned32  count,
                twr_t_length,
                *related_data_ptr[TWR_C_NUM_DNA_LOWER_FLRS];
    byte_p_t    tmp_tower;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    if (sa->family == RPC_C_NAF_ID_DNET)
    {
        protocol_id[0] = TWR_C_FLR_PROT_ID_DNA;
        protocol_id[1] = TWR_C_FLR_PROT_ID_NSP;
        protocol_id[2] = TWR_C_FLR_PROT_ID_ROUTING;
        
        /* 
         * Since we now know the socket address we're dealing with, 
         * collect the sizes of each field to allocate memory, and 
         * remember the pointers to the fields so we can copy the 
         * data once we have allocated the tower string.
         */
        
        floor_count = TWR_C_NUM_DNA_LOWER_FLRS;
        
        related_data_size[0] =
        sizeof(((struct sockaddr_dn *)sa)->sdn_objnum) +
        sizeof(((struct sockaddr_dn *)sa)->sdn_objnamel) +
        ((struct sockaddr_dn *)sa)->sdn_objnamel;
        
        related_data_ptr[0] =
        (unsigned32 *)(&((struct sockaddr_dn *)sa)->sdn_objnum);
        
        /*      do we need to store flags ??? 
         *                related_data_size[1] =          
         *                   sizeof(((struct sockaddr_dn *)sa)->sdn_flags); 
         */
        related_data_size[1] = 0;
        
        related_data_ptr[1] =
        (unsigned32 *)(&((struct sockaddr_dn *)sa)->sdn_flags);
        
        related_data_size[2] =
        sizeof(((struct sockaddr_dn *)sa)->sdn_add.a_len) +
        ((struct sockaddr_dn *)sa)->sdn_add.a_len;
        
        related_data_ptr[2] =
        (unsigned32 *)(&((struct sockaddr_dn *)sa)->sdn_add);
    }
    else
    {
        *status = twr_s_unknown_sa;
        return;
    }

    /*  
     * Allocate memory and copy the data into the proper
     * locations in each of up to three floors.
     */

    /*
     * Calculate length of tower floor.
     */

    twr_t_length = twr_c_tower_flr_count_size; /* to store floor count */
    
    for ( count = 0; count < floor_count; count++ )
    {
        twr_t_length += TWR_C_FLR_OVERHEAD;
        twr_t_length += related_data_size[count];
    }
    
    /* 
     * Next allocate space for the tower structure
     */
    RPC_MEM_ALLOC (
        *lower_flrs,
        twr_p_t,
        sizeof (twr_t) + (twr_t_length - 1),
        RPC_C_MEM_TOWER,
        RPC_C_MEM_WAITOK );
    if (*lower_flrs == NULL){
	*status = rpc_s_no_memory;
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

    for (count = 0; count < floor_count; count++)
    {
        /* 
         * Copy the length of the protocol identifier field into 
         * tower octet string. (Converted before the loop.)
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
         * If there is addressing data, copy the address data field into 
         * tower octet string
         */
        if (related_data_size[count])
        {
            memcpy ((char *)tmp_tower, (char *)related_data_ptr[count], 
                    related_data_size[count] );
            
            /*
             * Set up for the next floor.
             */
            tmp_tower += related_data_size[count];
        }
    }

    *status = twr_s_ok;
}

/*
**++
**
**  ROUTINE NAME:       twr_dnet_lower_flrs_to_sa 
**
**  SCOPE:              PUBLIC - declared in twr.idl
**
**  DESCRIPTION:
**
**  Creates a DECnet sockaddr from the canonical representation of a 
**  DECnet protocol tower's lower floors.
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

PUBLIC void twr_dnet_lower_flrs_to_sa 
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
    unsigned8   id;
    byte_p_t    tower;
    unsigned16  count,
                floor_count,
                id_size,
                addr_size;
    unsigned32  length;


    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    /* 
     * make sure we have a pointer to some data structure
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
     * beginning of the dnet-specific lower floors.
     */
    for (count = 0; count < floor_count; count++)
    {
        /*
         * Get the length of this floor's protocol id field (don't
         * advance the pointer).
         */
        memcpy ((char *)&id_size, (char *)tower, 
                TWR_C_TOWER_FLR_LHS_COUNT_SIZE);
        RPC_RESOLVE_ENDIAN_INT16 (id_size);

        /*
         * Get the protocol id (don't advance the pointer).
         * Expect one byte; no need to convert.
         */
        memcpy ((char *)&id, (char *)(tower + TWR_C_TOWER_FLR_LHS_COUNT_SIZE),
                twr_c_tower_prot_id_size);

        /*
         * See if we support the protocol id.
         */
        if ( (id_size == twr_c_tower_prot_id_size) && 
             (id ==  TWR_C_FLR_PROT_ID_DNA) )
        {
            /*
             * Indicate we found the beginning of the dnet floors.
             */
            *status = twr_s_ok;

            break;
        }
        else
        {
            /*
             * Skip this floor.  Get the address size in order
             * to know how much to skip.
             */
            memcpy ((char *)&addr_size, 
                    (char *)(tower + TWR_C_TOWER_FLR_LHS_COUNT_SIZE +
                            id_size), TWR_C_TOWER_FLR_RHS_COUNT_SIZE);
            RPC_RESOLVE_ENDIAN_INT16 (addr_size);

            tower += TWR_C_TOWER_FLR_LHS_COUNT_SIZE + id_size +
                     TWR_C_TOWER_FLR_RHS_COUNT_SIZE + addr_size;

            /*
             * For now, assume we don't find the floors we're looking for.
             */
            *status = twr_s_unknown_tower;
        }
    }

    if (*status != twr_s_ok)
    {
        return;
    }

    /*
     * Skip the floor's protocol id field length and protocol id
     * (now move the pointer).  We already know it's TWR_C_FLR_PROT_ID_DNA.
     */
    tower += (TWR_C_TOWER_FLR_LHS_COUNT_SIZE + id_size);

    /* 
     * Allocate space for a DECnet sockaddr
     */
    length = sizeof(struct sockaddr_dn);
    
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
     * define this as a DECnet family socket
     */
    ((struct sockaddr_dn *)(*sa))->sdn_family = RPC_C_NAF_ID_DNET;    

    /* 
     * Length of end user spec 
     */
    memcpy ((char *)&addr_size, (char *)tower, RPC_C_TOWER_FLR_RHS_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (addr_size);
    tower += RPC_C_TOWER_FLR_RHS_COUNT_SIZE;

    /* 
     * End user spec 
     */
    memcpy (&((struct sockaddr_dn *)(*sa))->sdn_objnum,
            (char *)tower,
            addr_size);
    
    tower += addr_size;
    
    /*
     * Length of host transport
     */
    memcpy ((char *)&id_size, (char *)tower, 
            TWR_C_TOWER_FLR_LHS_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (id_size);
    tower  += TWR_C_TOWER_FLR_LHS_COUNT_SIZE;

    /* 
     * Transport id
     * Expect one byte; no need to convert.
     */
    memcpy ((char *)&id, (char *)tower, twr_c_tower_prot_id_size);
    tower += id_size;

    if ((id_size != twr_c_tower_prot_id_size) || 
        (id != TWR_C_FLR_PROT_ID_NSP))
    {
        *status = twr_s_unknown_tower;

        RPC_MEM_FREE (*sa, RPC_C_MEM_SOCKADDR);

        return;
    }
    
    /* 
     * Length of sdn_flags
     */
    memcpy ((char *)&addr_size, (char *)tower, RPC_C_TOWER_FLR_RHS_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (addr_size);
    tower += RPC_C_TOWER_FLR_RHS_COUNT_SIZE;

    /* 
     * this field is probably null
     */
    if (addr_size)
    {
        memcpy ( &((struct sockaddr_dn *)(*sa))->sdn_flags,
                (char *)tower,
                addr_size);
        
        tower += addr_size;
    }
    
    /* 
     * Length of routing id
     */
    memcpy ((char *)&id_size, (char *)tower, 
            TWR_C_TOWER_FLR_LHS_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (id_size);
    tower += TWR_C_TOWER_FLR_LHS_COUNT_SIZE;

    /* 
     * Protocol id
     * Expect one byte, so no need to convert.
     */
    memcpy ((char *)&id, (char *)tower, twr_c_tower_prot_id_size);
    tower += id_size;

    if ( (id_size != twr_c_tower_prot_id_size) || 
         (id != TWR_C_FLR_PROT_ID_ROUTING) )
    {
        *status = twr_s_unknown_tower;

        RPC_MEM_FREE (*sa, RPC_C_MEM_SOCKADDR);

        return;
    }
    
    /* 
     * the length of host address
     */
    memcpy ((char *)&addr_size, (char *)tower, RPC_C_TOWER_FLR_RHS_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (addr_size);
    tower += RPC_C_TOWER_FLR_RHS_COUNT_SIZE;

    memcpy (&((struct sockaddr_dn *)(*sa))->sdn_add,
            (char *)tower,
            addr_size);
    
    *status = twr_s_ok;
}
#else 
static int _naf_dnet_dummy_ = 0 ;
#endif /* NAF_DNET */
