/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_rpc.h,v $
 * Revision 1.1.8.2  1996/02/18  23:11:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:22  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:06:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:49  root]
 * 
 * Revision 1.1.6.2  1994/08/23  20:31:52  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:40  cbrooks]
 * 
 * Revision 1.1.6.1  1994/05/12  15:17:10  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:26  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  20:39:13  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:07:38  htf]
 * 
 * Revision 1.1.2.2  1992/06/26  17:04:01  pellis
 * 	Change DATAGRAM_ONLY macro value to 0, to allow TCP_IP connections
 * 	[1992/06/26  16:44:43  pellis]
 * 
 * Revision 1.1  1992/01/19  15:32:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright (c) 1990 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:	RPC header file
 *
 * ENVIRONMENT:	ULTRIX
 *
 * AUTHORS:
 *	        Cesar Cortes
 *		Distributed Processing Engineering (DPE) 
 * 
 * DATE:	October 1990
 *
 */


#ifndef _DTSS_RPC_H
#define _DTSS_RPC_H 1

#include <dce/rpc.h>        /* RPC definitions and function prototypes */
#include <dce/nbase.h>      /* types used by RPC and NIDL */
#if CONFIG_SECURITY
# include <dce/sec_login.h>
# include <dce/rgybase.h>
#endif /* CONFIG_SECURITY */

#include <dce/dce_error.h>  /* signature and constants for dce_error_inq_text */
#include <dce/dce_cf.h>     /* dce configuration stuff */

/*
 * The system is configured to run datagram only if this constant is
 * is turned on. 
 */
#define DATAGRAM_ONLY 0

/*
 * NOTE: DTS_ENTITY_PRINCIPAL is defined here for code that we're not going to 
 *       include in DCE V1.0.  This affects the code that operates on
 *       behalf of the dts server principal (/.:/hosts/<nodename>/dts-entity).
 *       For DCE V1.0 the dts server will run as the machine principal, 
 *       therefore the ticket gets refreshed and the identity validated by
 *       the sec_clientd process.
 */
#define DTS_ENTITY_PRINCIPAL 0


#define TIME_SERVICE_UUID  "019ee420-682d-11c9-a607-08002b0dea7a"

#define DEF_SERVER_ENTRY_NAME "/dts-entity" /* TS entry name in CDS */

#define CELL_RELATIVE_PREFIX "/.:/"  /* needed for CDS names */


/*
 * Well-known cell profile name
 */
#define CELLPROFILE_NAME "/.:/cell-profile"
                                             
/* 
 * Default principal name for this host's Time Service.  
 */

#if DTS_ENTITY_PRINCIPAL
#    define DEF_TSERV_PRINC_NAME  "/dts-entity"
#else
#    define DEF_TSERV_PRINC_NAME  "/self"
#endif

/* 
 * Default name of the time servers group and the time service 
 * administration group.
 */

#define DEF_TSERV_GROUP_NAME "subsys/dce/dts-servers"
#define DEF_TSERV_ADMIN_GROUP_NAME "subsys/dce/dts-admin"

#define K_MAX_ENTRY_LENGTH      (1024)  /* Max length of entry in cds */
#define K_RPC_MAX_CALLS         (10)    /* max # of concurrent calls */
                                        /* accepted by RPC */

#define LOCAL_HOST    "127.0.0.1"        /* inet address of local host */

/*
 * This macro invokes the dce api routine to extract an error message
 * given the status code returned from a routine call.
 */

#define PrintRPCerror(st, message)			           \
   {                                                               \
	int _locstat;			                           \
	unsigned char       errMessage[dce_c_error_string_len];    \
							           \
	dce_error_inq_text((st), errMessage, &_locstat);           \
	(void)fprintf(stderr, "%s - %s\n", (message), errMessage); \
   }


/*
 * The possible protocol sequences supported by RCP are:
 *
 *     ncacn_ip_tcp   -> Tranmission Control Protocol (TCP)
 *     ncacn_osi_tp4  -> ISO Transport Protocol Class 4 (DECnet Phase V)
 *     ncacn_osi_nsp  -> Network Services Protocol (DECnet Phase V)
 *     ncacn_dnet_nsp -> Network Services Protocol (DECnet Phase IV)
 *     ncadg_ip_udp   -> User Datagram Protocol
 *     ncadg_dds      -> NCA Datagram over Domain Datagram Service
 */

#define PROT_SEQS 6


/*
 * This macro selects a protocol sequence based on a priority order
 * and availability on the system.
 */

#define SelectStrBinding(status, strBindName)                         \
   {                                                                  \
        char                    *protseq_select[PROT_SEQS];           \
        int                     _indx, _p_indx, _selected;            \
	rpc_protseq_vector_p_t  _protseq_vector;                      \
	unsigned32              _locStat;                             \
                                                                      \
	protseq_select[0] = "ncacn_ip_tcp";                           \
	protseq_select[1] = "ncacn_osi_tp4";                          \
	protseq_select[2] = "ncacn_osi_nsp";                          \
	protseq_select[3] = "ncacn_dnet_nsp";                         \
	protseq_select[4] = "ncadg_ip_udp";                           \
	protseq_select[5] = "ncadg_dds";                              \
                                                                      \
	rpc_network_inq_protseqs (&_protseq_vector,                   \
				  (&status));                         \
	if (status == rpc_s_ok){                                      \
	   _selected = 0;                                             \
	   for (_p_indx=0; _p_indx < PROT_SEQS;){                     \
	       for (_indx=0; _indx < _protseq_vector->count;){        \
		   if (strcmp(protseq_select[_p_indx],                \
			(char *)_protseq_vector->protseq[_indx++])==0){ \
		      _selected = 1;                                  \
		      break;                                          \
		   }                                                  \
	       }                                                      \
	       if (_selected)                                         \
		   break;                                             \
	       else                                                   \
		   _p_indx++;                                         \
	   }                                                          \
		                                                      \
           rpc_string_binding_compose ((unsigned_char_p_t)NULL,       \
                         (unsigned_char_p_t)protseq_select[_p_indx],  \
                                       (unsigned_char_p_t)LOCAL_HOST, \
                                       (unsigned_char_p_t)NULL,       \
                                       (unsigned_char_p_t)NULL,       \
                                       (&strBindName),                \
                                       (&status));                    \
	}                                                             \
        rpc_protseq_vector_free (&_protseq_vector, &_locStat);        \
   }

/*
 * This macro converts a binding handle into a string binding and 
 * prints it.  Use for debugging when service trace is turned on.
 *
 */

#define displayBinding(binding_h, message)                      \
   {                                                            \
       unsigned_char_p_t _strBinding =(unsigned_char_p_t)NULL;  \
       unsigned32        _status;                               \
	                                                        \
       rpc_binding_to_string_binding ((binding_h),&_strBinding, \
		         	      &_status);                \
       fprintf(stderr, "%s -> %s \n", (message), _strBinding);  \
       if (_strBinding != NULL)                                 \
           rpc_string_free (&_strBinding, &_status);            \
   }




/*
 * RPC and Security definitions pertaining to the local host.
 * The fields are:
 *
 * bindingVector ->  binding vector handles over which a server
 *                   application can receive remote procedure calls.
 *
 * objUUIDvector ->  vector of pointers to object uuids (actually one) 
 *                   that uniquely identify an instance of the Time Service 
 *                   on a particular node.
 *
 * TServiceObjUUID ->  Time Service UUID referenced in the objUUIDvector above.
 *
 * TServicePrincUUID  ->  The UUID corresponding to the time server principal
 *                        running on a particular node.  This UUID is assigned
 *                        by the security server when the principal is created
 *                        by the administrator.  This UUID is used for server 
 *                        discrimination on cache population and courier role
 *                        selection (for servers only).
 *
 * logincontext_h ->  login context handle for a principal doing authentication
 *                    calls.
 *
 * secContext -> binding context to a security server within the cell.
 *
 * my_cell ->  Name and UUID of the cell to which the host belongs.
 *
 * host_principal ->  principal name of the host (otherwise known as 
 *                    machine principal.  Its well known name is of the 
 *                    form:  /.:/hosts/<host_name>/self
 *
 * hostName ->  The name of this host's entry in the cell.  Each host has
 *              a directory in which RPC server entries, groups and profiles
 *              associated with this host are stored.  This name is of the
 *              form:  /.:/hosts/<host_name>
 *
 * TServiceEntryName ->   This entry contains the server bindings for a 
 *                        time server.  It is also used by the ACL editor
 *                        to manage the ACL interface on a server or a clerk.
 *
 * DefServPrincName ->  Default principal name of a particular time server.
 *                      This is a cell-relative name that looks like this:
 *                         /.:/hosts/<hostname>/time-service
 *
 * cellProfile ->  Name of the master default profile for the cell.
 *                 Its well known name is : /.:/cell-profile
 *
 * lanProfile ->  LAN services profile used by the time service.  This is 
 *                the profile that contains all the local time servers.
 *                A particular host configured as a time server will enter
 *                its name here.
 *                Its default name is: /.:/lan-profile
 *
 */

typedef struct RpcBuf
{
    rpc_binding_vector_p_t bindingVector;
    uuid_vector_t          objUUIDvector;
    uuid_t                 TServiceObjUUID;
    uuid_t                 TServicePrincUUID;

#if CONFIG_SECURITY
    sec_login_handle_t     loginContext_h;
    sec_rgy_handle_t       secContext;
    sec_id_t		   my_cell;
    sec_id_t		   host_principal;
#endif /* CONFIG_SECURITY */
    unsigned_char_t        *hostName;
    unsigned_char_t        TServiceEntryName[K_MAX_ENTRY_LENGTH];
    unsigned_char_t        DefServPrincName[K_MAX_ENTRY_LENGTH];
    unsigned_char_t        cellProfile[K_MAX_ENTRY_LENGTH];
    unsigned_char_t        *lanProfile;
} RpcBuf;


#endif  /* _DTSS_RPC_H */

