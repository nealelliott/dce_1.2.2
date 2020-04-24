/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eedouble.c,v $
 * Revision 1.1.8.2  1996/02/18  18:53:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:15  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:25:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:14:12  root]
 * 
 * Revision 1.1.4.2  1993/07/07  20:05:43  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:36:02  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      eedouble.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      Callee marshalling and unmarshalling of pointed_at double's
**
**  VERSION: DCE 1.0
**
*/

/* The ordering of the following 3 includes should NOT be changed! */
#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <lsysdep.h>

void


rpc_ss_me_long_float
#ifdef IDL_PROTOTYPES
(
    idl_long_float *p_node,
    rpc_ss_node_type_k_t NIDL_node_type,
    rpc_ss_marsh_state_t *NIDL_msp
)
#else
( p_node, NIDL_node_type, NIDL_msp)
    idl_long_float *p_node;
    rpc_ss_node_type_k_t NIDL_node_type;
    rpc_ss_marsh_state_t *NIDL_msp;
#endif
{
  long NIDL_already_marshalled;
  unsigned long NIDL_represent_pointer_as;
  unsigned long space_for_node;
  rpc_mp_t mp;
  rpc_op_t op;

  if(p_node==NULL)return;
  if (NIDL_node_type == rpc_ss_mutable_node_k) {
      rpc_ss_register_node(NIDL_msp->node_table,(byte_p_t)p_node,idl_true,&NIDL_already_marshalled);
      if(NIDL_already_marshalled)return;
  }
  space_for_node=((8))+7;
  if (space_for_node > NIDL_msp->space_in_buff)
  {
    rpc_ss_marsh_change_buff(NIDL_msp,space_for_node);
  }
  mp = NIDL_msp->mp;
  op = NIDL_msp->op;
  rpc_align_mop(mp, op, 8);
  rpc_marshall_long_float(mp, (*p_node));
  rpc_advance_mop(mp, op, 8);
  NIDL_msp->space_in_buff -= (op - NIDL_msp->op);
  NIDL_msp->mp = mp;
  NIDL_msp->op = op;
}

void


rpc_ss_ue_long_float
#ifdef IDL_PROTOTYPES
(
    idl_long_float **p_referred_to_by,
    rpc_ss_node_type_k_t NIDL_node_type,
    rpc_ss_marsh_state_t *p_unmar_params
)
#else
( p_referred_to_by,NIDL_node_type,p_unmar_params )
    idl_long_float **p_referred_to_by;
    rpc_ss_node_type_k_t NIDL_node_type;
    rpc_ss_marsh_state_t *p_unmar_params;
#endif
{
  idl_long_float  *p_node = NULL;
  long NIDL_already_unmarshalled = 0;
  unsigned long *p_stored_node_number;
  unsigned long node_size;
  unsigned long node_number;
  unsigned long NIDL_represent_pointer_as;
  ndr_boolean buffer_changed;

  if ( NIDL_node_type == rpc_ss_unique_node_k )
  {
    if (*p_referred_to_by == NULL) return;
    else if (*p_referred_to_by != (idl_long_float *)RPC_SS_NEW_UNIQUE_NODE) p_node = *p_referred_to_by;
  }

  if ( NIDL_node_type == rpc_ss_mutable_node_k )
  {
    node_number = (unsigned long)*p_referred_to_by;
    if(node_number==0)return;
  }
  if ( NIDL_node_type == rpc_ss_old_ref_node_k )
   p_node = *p_referred_to_by;
  else if ( p_node == NULL )
  {
    node_size = sizeof(idl_long_float );
    if (NIDL_node_type == rpc_ss_mutable_node_k)
    p_node = (idl_long_float *)rpc_ss_return_pointer_to_node(
        p_unmar_params->node_table, node_number, node_size,
        NULL, &NIDL_already_unmarshalled, (long *)NULL);
    else
    p_node = (idl_long_float *)rpc_ss_mem_alloc(
        p_unmar_params->p_mem_h, node_size );
    *p_referred_to_by = p_node;
    if (NIDL_already_unmarshalled) return;
  }
  if ( NIDL_node_type == rpc_ss_alloc_ref_node_k )
  {
    return;
  }
  rpc_align_mop(p_unmar_params->mp, p_unmar_params->op, 8);
  if ((byte_p_t)p_unmar_params->mp - p_unmar_params->p_rcvd_data->data_addr >= p_unmar_params->p_rcvd_data->data_len)
  {
    rpc_ss_new_recv_buff(p_unmar_params->p_rcvd_data, p_unmar_params->call_h, &(p_unmar_params->mp), &(*p_unmar_params->p_st));
  }
  rpc_convert_long_float(p_unmar_params->src_drep, ndr_g_local_drep, p_unmar_params->mp, (*p_node));
  rpc_advance_mop(p_unmar_params->mp, p_unmar_params->op, 8);
}
