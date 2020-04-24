/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ddissupp.c,v $
 * Revision 1.1.2.2  1996/03/11  13:25:44  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:51  marty]
 *
 * Revision 1.1.2.1  1995/12/13  17:24:44  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:06 UTC  dat
 * 	Add new files for merge of second XIDL drop for DCE 1.2.1
 * 	[1995/12/13  17:23:52  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:49:01  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  01:25:19  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/22  22:58:05  bfc]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      ddissupp.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Routines that will be called by DDIS routines
**  Also DDIS cleanup routine to go in exception handler
**      and reporting of DDIS errors
**
*/

/*
 *  
 */

#include <dce/idlddefs.h>
#ifdef VMS
#include <ddis$def.h>
#include <ddis$msg.h>
#include <ddissupp.h>

#include <lib$routines.h>

#else
#include <ddis_def.h>
#include <ddis_msg.h>
#endif
#include <lsysdep.h>

#ifdef VMS
globaldef idl_long_int (*ddis_g_entries[])() = { 0, 0, 0, 0, 0, 0, 0 };
#endif

#if defined(DEBUG_BUFF) || defined(DEBUG_ALLOC)
#include <stdio.h>
#endif
globalref stdlparams[];

idl_void_p_t IDL_ddis_syntax_table_list[] = {(idl_void_p_t)1,
                                             (idl_void_p_t)stdlparams};

static idl_byte IDL_ddis_dummy_sos[2] = { 0x30, 0x80 };
static idl_byte IDL_ddis_dummy_eoc[2] = { 0, 0 };


#ifdef DEBUG_ALLOC
static void debug_free(void *addr)
{
    fprintf(stderr, "matching rpc_ss_ddis_data_put: free %lx\n", addr);
    fflush(stderr);
    free(addr);
}
#endif

/******************************************************************************/
/*                                                                            */
/*  Memory allocator                                                          */
/*                                                                            */
/******************************************************************************/
/*
 */
unsigned long rpc_ss_ddis_mem_alloc
#ifdef IDL_PROTOTYPES
(
    unsigned long *p_num_bytes,
    rpc_void_p_t *p_base_adr,
    unsigned long user_arg
)
#else
( p_num_bytes, p_base_adr, user_arg )
    unsigned long *p_num_bytes;
    rpc_void_p_t *p_base_adr;
    unsigned long user_arg;
#endif
{
    *p_base_adr = (rpc_void_p_t)malloc(*p_num_bytes);
#ifdef DEBUG_ALLOC
    fprintf(stderr, "rpc_ss_ddis_mem_alloc: malloc %lx (len=%d)\n",
            *p_base_adr, *p_num_bytes);
    fflush(stderr);
#endif
    if (*p_base_adr == NULL)
        return(rpc_s_no_memory);
    else
        return(DDIS$_NORMAL);
}

/******************************************************************************/
/*                                                                            */
/*  Memory de-allocator                                                       */
/*                                                                            */
/******************************************************************************/
unsigned long rpc_ss_ddis_mem_free
#ifdef IDL_PROTOTYPES
(
    unsigned long *p_num_bytes,
    rpc_void_p_t *p_base_adr,
    unsigned long user_arg
)
#else
( p_num_bytes, p_base_adr, user_arg )
    unsigned long *p_num_bytes;
    rpc_void_p_t *p_base_adr;
    unsigned long user_arg;
#endif
{
#ifdef DEBUG_ALLOC
    fprintf(stderr, "rpc_ss_ddis_mem_free: free %lx\n", *p_base_adr);
    fflush(stderr);
#endif
    free(*p_base_adr);
    return(DDIS$_NORMAL);
}

/******************************************************************************/
/*                                                                            */
/*  Write data when interpreter was called to do pickling using a DCE style   */
/*  pickling handle                                                           */
/*                                                                            */
/******************************************************************************/
static unsigned long idl_es_encode_ddis_put
#ifdef IDL_PROTOTYPES
(
    unsigned long num_bytes,    /* [in] Length of data delivered by DDIS */
    unsigned long *p_next_buf_len,  /* [out] Size of new buffer given to DDIS */
    IDL_msp_t IDL_msp
)
#else
(num_bytes, p_next_buf_len, IDL_msp)
    unsigned long num_bytes;
    unsigned long *p_next_buf_len;
    IDL_msp_t IDL_msp;
#endif
{
    IDL_es_state_t *p_es_state;
    rpc_iovector_elt_t *p_iovec_elt;
    idl_ulong_int buff_size;

    p_es_state = (IDL_es_state_t *)(IDL_msp->IDL_pickling_handle);
    switch (p_es_state->IDL_style)
    {
        case IDL_incremental_k:
            /* When there is a pickle header present, we need to add it
                into the data being written */
            num_bytes += (IDL_msp->IDL_mp - IDL_msp->IDL_buff_addr);
            (*(p_es_state->IDL_write))(p_es_state->IDL_state,
                                       IDL_msp->IDL_buff_addr,
                                       (idl_ulong_int)num_bytes);
            if (IDL_msp->IDL_ddis_state != IDL_ddis_marshall_done)
            {
                /* If this is not the end of the encoding, get another
                    buffer */
                buff_size = IDL_BUFF_SIZE;
                (*(p_es_state->IDL_alloc))(p_es_state->IDL_state,
                                       &IDL_msp->IDL_buff_addr, &buff_size);
                /* IDL_mp is not used after the pickle header is generated.
                    Buffers after the first add 0 for the pickle header */
                IDL_msp->IDL_mp = IDL_msp->IDL_buff_addr;
            }
            break;
        case IDL_fixed_k:
            if (IDL_msp->IDL_ddis_state == IDL_ddis_marshall_done)
            {
                /* Normal end of encoding. Move IDL_mp from end of
                    pickle header to end of pickle */
                IDL_msp->IDL_mp += num_bytes;
                break;
            }
            else
                return(rpc_s_no_memory);
        case IDL_dynamic_k:
            p_iovec_elt = p_es_state->IDL_dyn_buff_chain_tail->IDL_p_iovec_elt;
            p_iovec_elt->data_addr = (byte_p_t)IDL_msp->IDL_data_addr;
            /* When there is a pickle header present, we need to add it
                into the data being written */
            num_bytes += (IDL_msp->IDL_mp - IDL_msp->IDL_buff_addr);
            p_iovec_elt->data_len = num_bytes;
            if (IDL_msp->IDL_ddis_state != IDL_ddis_marshall_done)
            {
                /* Allocate a new dynamic buffer */
                if (idl_es_encode_new_dyn_buff(&buff_size, IDL_msp)
                                                            != error_status_ok)
                    return(rpc_s_no_memory);
                /* IDL_mp is not used after the pickle header is generated.
                    Buffers after the first add 0 for the pickle header */
                IDL_msp->IDL_mp = IDL_msp->IDL_buff_addr;
            }
            break;
        default:
#ifdef DEBUG_INTERP
            printf("idl_es_encode_ddis_put - unknown encoding style\n");
            exit(0);
#endif
            RAISE(rpc_x_coding_error);
    }
    *p_next_buf_len = buff_size;
    return(DDIS$_NORMAL);
}

/******************************************************************************/
/*                                                                            */
/*  Write encoded data                                                        */
/*                                                                            */
/******************************************************************************/
unsigned long rpc_ss_ddis_data_put
#ifdef IDL_PROTOTYPES
(
    unsigned long put_prm,
    unsigned long *p_num_bytes,
    rpc_void_p_t buf_adr,
    unsigned long *p_next_buf_len,
    rpc_void_p_t *p_next_buf_adr
)
#else
( put_prm, p_num_bytes, buf_adr, p_next_buf_len, p_next_buf_adr )
    unsigned long put_prm;
    unsigned long *p_num_bytes;
    rpc_void_p_t buf_adr;
    unsigned long *p_next_buf_len;
    idl_byte **p_next_buf_adr;
#endif
{
    IDL_ms_t *p_idl_state;
    rpc_iovector_t iovec;

    p_idl_state = (IDL_ms_t *) put_prm;
    if (p_idl_state->IDL_ddis_state == IDL_ddis_cleanup)
        return(DDIS$_NORMAL);

    /*
     * The first marshall is a special case to discard the "start of sequence"
     * which is not part of the wire data.  After this first marshall, set
     * the buffer pointer and size for the first actual data buffer.
     */
    if (p_idl_state->IDL_ddis_state == IDL_ddis_first_marshall)
    {
#ifdef DEBUG_BUFF
        fprintf(stderr,
                "rpc_ss_ddis_data_put throw away: addr=%lx len=%lu data=%lx\n",
                buf_adr, *p_num_bytes, *(unsigned long *)buf_adr);
        fflush(stderr);
#endif
        p_idl_state->IDL_ddis_state = IDL_ddis_marshall;

        /* If no pickling handle, allocate the buffer via malloc */
        if (p_idl_state->IDL_pickling_handle == NULL)
        {
            p_idl_state->IDL_buff_addr = (idl_byte *)malloc(IDL_BUFF_SIZE);
#ifdef DEBUG_ALLOC
            fprintf(stderr, "rpc_ss_ddis_data_put: malloc %lx (len=%d)\n",
                    p_idl_state->IDL_buff_addr, IDL_BUFF_SIZE);
            fflush(stderr);
#endif
            if (p_idl_state->IDL_buff_addr == NULL)
            {
                RAISE(rpc_x_no_memory);
            }
            *p_next_buf_len = IDL_BUFF_SIZE;
        }
        else if (((IDL_es_state_t *)(p_idl_state->IDL_pickling_handle))->
                                        IDL_version == IDL_ES_STATE_VERSION)
        {
            *p_next_buf_adr = p_idl_state->IDL_mp;
            *p_next_buf_len = p_idl_state->IDL_left_in_buff;
            return(DDIS$_NORMAL);
        }
        else
        {
            /* MIA-style pickling */
            IDL_mia_ber_pickling_handle_t *IDL_esp = 
                (IDL_mia_ber_pickling_handle_t*)p_idl_state->IDL_pickling_handle;

            /* Initialize the total output length */
            *(IDL_esp->IDL_encoding_len) = 0;

            /* 
            ** Call application alloc routine for the first buffer.  This call
            ** uses the temporary buf_len instead of *p_next_buf_len directly
            ** because they are not type compatible on some platforms such as
            ** alpha.
            */
            {
                idl_ulong_int buf_len = *p_next_buf_len;  
                (*IDL_esp->IDL_app_alloc)(
                    &p_idl_state->IDL_buff_addr,
                    &buf_len,
                    IDL_esp->IDL_app_state);	
                *p_next_buf_len = buf_len;
            }
        }
    }
    /*
     * Remaining cases for other than the special case first buffer -
     * transmit data for normal case, call "put" routine for pickling case,
     * and set up next buffer.
     */

    /* If no pickling handle, transmit RPC buffer */
    else if (p_idl_state->IDL_pickling_handle == NULL)
    {
        /* Load up the iovector */
        iovec.num_elt = 1;
#ifdef DEBUG_ALLOC
        iovec.elt[0].buff_dealloc = (rpc_ss_dealloc_t)debug_free;
#else
        iovec.elt[0].buff_dealloc = (rpc_ss_dealloc_t)free;
#endif
        iovec.elt[0].flags = 0;
        iovec.elt[0].buff_addr = (byte_p_t)buf_adr;
        iovec.elt[0].buff_len = IDL_BUFF_SIZE;
        iovec.elt[0].data_addr = (byte_p_t)buf_adr;
        iovec.elt[0].data_len = *p_num_bytes;

        rpc_call_transmit(p_idl_state->IDL_call_h, &iovec,
                             (unsigned32*)&p_idl_state->IDL_status);
        if ( p_idl_state->IDL_status != error_status_ok)
            return(rpc_s_fault_pipe_comm_error);

        /*
        **  Allocate another buffer, unless this is the final put from the
        **  close stream call
        */
        if (p_idl_state->IDL_ddis_state != IDL_ddis_marshall_done)
        {
            *p_next_buf_len = IDL_BUFF_SIZE;
            p_idl_state->IDL_buff_addr = (idl_byte *)malloc(IDL_BUFF_SIZE);
#ifdef DEBUG_ALLOC
            fprintf(stderr, "rpc_ss_ddis_data_put: malloc %lx (len=%d)\n",
                    p_idl_state->IDL_buff_addr, IDL_BUFF_SIZE);
            fflush(stderr);
#endif
            if (p_idl_state->IDL_buff_addr == NULL)
            {
                return(rpc_s_no_memory);
            }
        }
        else
        {
            *p_next_buf_len = 0;
            p_idl_state->IDL_buff_addr = (idl_byte *)NULL;
        }
    }
    else if (((IDL_es_state_t *)(p_idl_state->IDL_pickling_handle))->IDL_version
                == IDL_ES_STATE_VERSION)
    {
        /* Doing a DCE style encoding */
        if (idl_es_encode_ddis_put(*p_num_bytes, p_next_buf_len, p_idl_state)
                                                                != DDIS$_NORMAL)
            return(rpc_s_no_memory);
    }
    else /* Doing a MIA-style encoding */
    {
        IDL_mia_ber_pickling_handle_t *IDL_esp = 
            (IDL_mia_ber_pickling_handle_t*)p_idl_state->IDL_pickling_handle;
        /* TBS -- Make sure it will use up the buffer */
        *(IDL_esp->IDL_encoding_len) += *p_num_bytes;

        /*
        **  Call the application alloc routine for another buffer, unless this
        **  is the put from the close stream
        */
        if (p_idl_state->IDL_ddis_state != IDL_ddis_marshall_done)
        {
            idl_ulong_int int_buf_len;
            (*IDL_esp->IDL_app_alloc)(
                &p_idl_state->IDL_buff_addr,
                &int_buf_len,
                IDL_esp->IDL_app_state);	
            *p_next_buf_len = int_buf_len;
        }
        else
        {
            *p_next_buf_len = 0;
            p_idl_state->IDL_buff_addr = (idl_byte *)NULL;
        }
    }

    *p_next_buf_adr = p_idl_state->IDL_buff_addr;
    return(DDIS$_NORMAL);
}

/******************************************************************************/
/*                                                                            */
/*  Read encoded data                                                         */
/*                                                                            */
/******************************************************************************/
unsigned long rpc_ss_ddis_data_get
#ifdef IDL_PROTOTYPES
(
    unsigned long get_prm,
    unsigned long *p_num_bytes,
    rpc_void_p_t *p_buf_adr
)
#else
( get_prm, p_num_bytes, p_buf_adr )
    unsigned long get_prm;
    unsigned long *p_num_bytes;
    rpc_void_p_t *p_buf_adr;
#endif
{
    IDL_ms_t *p_idl_state;
    IDL_es_state_t *p_es_state;
    idl_ulong_int num_bytes;    /* Intermediate value because required result
                                    may be 64-bit integer */

    p_idl_state = (IDL_ms_t *) get_prm;

    /*
     * At the start of unmarshalling we must provide the "start of sequence"
     * which is part of the ASN.1 encoding but NOT part of the wire data.
     * Change state to "first unmarshall from actual data".
     */
    if (p_idl_state->IDL_ddis_state == IDL_ddis_start_unmarshall)
    {
        *p_num_bytes = sizeof(IDL_ddis_dummy_sos);
        *p_buf_adr = (rpc_void_p_t)IDL_ddis_dummy_sos;
        p_idl_state->IDL_ddis_state = IDL_ddis_first_unmarshall;
        return(DDIS$_NORMAL);
    }

    /*
     * At the end of unmarshalling we must provide the "end of constructor"
     * which is part of the ASN.1 encoding but NOT part of the wire data.
     * Change state to "not in use" since no more data should be processed.
     */
    if (p_idl_state->IDL_ddis_state == IDL_ddis_end_unmarshall)
    {
        *p_num_bytes = sizeof(IDL_ddis_dummy_eoc);
        *p_buf_adr = (rpc_void_p_t)IDL_ddis_dummy_eoc;
        p_idl_state->IDL_ddis_state = IDL_ddis_not_in_use;
        return(DDIS$_NORMAL);
    }

    if (p_idl_state->IDL_pickling_handle != NULL)
    {
        if (p_idl_state->IDL_ddis_state == IDL_ddis_first_unmarshall)
        {
            /* Start at location following pickle header */
            *p_buf_adr = (rpc_void_p_t)(p_idl_state->IDL_mp);
            *p_num_bytes = p_idl_state->IDL_left_in_buff;
            p_idl_state->IDL_ddis_state = IDL_ddis_subsq_unmarshall;
        }
        else
        {
            p_es_state = (IDL_es_state_t *)p_idl_state->IDL_pickling_handle;
            if (p_es_state->IDL_style == IDL_incremental_k)
            {
                (*(p_es_state->IDL_read))
                    (p_es_state->IDL_state, (idl_byte **)p_buf_adr, &num_bytes);
                *p_num_bytes = num_bytes;
                return(DDIS$_NORMAL);
            }
            else    /* Fixed */
            {
                p_idl_state->IDL_status = rpc_s_ss_bad_buffer;
                return(rpc_s_fault_pipe_comm_error);
            }
        }
    }
    else    /* Not unpickling */
    {
        if (p_idl_state->IDL_ddis_state == IDL_ddis_first_unmarshall)
        {
            p_idl_state->IDL_ddis_state = IDL_ddis_subsq_unmarshall;
        }
        else
        {
            if (p_idl_state->IDL_ddis_state == IDL_ddis_cleanup)
                return(DDIS$_NORMAL);

            if (p_idl_state->IDL_elt_p->buff_dealloc
                        && p_idl_state->IDL_elt_p->data_len != 0)
            {
#ifdef DEBUG_ALLOC
                fprintf(stderr, "rpc_ss_ddis_data_get: %s %lx (len=%d)\n",
            (p_idl_state->IDL_elt_p->buff_dealloc == free) ? "free" : "dealloc",
                        p_idl_state->IDL_elt_p->buff_addr,
                        p_idl_state->IDL_elt_p->data_len);
                fflush(stderr);
#endif
                (*(p_idl_state->IDL_elt_p->buff_dealloc))
                                        (p_idl_state->IDL_elt_p->buff_addr);
                p_idl_state->IDL_elt_p->buff_dealloc = NULL;
            }
            rpc_call_receive(p_idl_state->IDL_call_h,
                            p_idl_state->IDL_elt_p,
                            (unsigned32*)&p_idl_state->IDL_status);
            if ( p_idl_state->IDL_status != error_status_ok)
                    return(rpc_s_fault_pipe_comm_error);
        }
        *p_num_bytes = p_idl_state->IDL_elt_p->data_len;
        *p_buf_adr = (rpc_void_p_t)(p_idl_state->IDL_elt_p->data_addr);
    }
    return(DDIS$_NORMAL);
}

/******************************************************************************/
/*                                                                            */
/*  Clean up DDIS - routine for exception handler                             */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ddis_clean_up
#ifdef IDL_PROTOTYPES
(
    IDL_ms_t *p_idl_state
)
#else
( p_idl_state )
    IDL_ms_t *p_idl_state;
#endif
{
    idl_ulong_int stat;

#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_CLOSE_STREAM] == 0)
    {
   	stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_close_stream_desc,
				     &ddis_g_entries[IDL_DDIS_CLOSE_STREAM],
				     0);
   	if ((stat & 1) != 1) 
	   RAISE (rpc_x_coding_error);
    }
#endif

    if ((p_idl_state->IDL_ddis_state == IDL_ddis_marshall) ||
	 (p_idl_state->IDL_ddis_state == IDL_ddis_marshall_done))
    {
        p_idl_state->IDL_ddis_state = IDL_ddis_cleanup;
#ifdef VMS
	(*(ddis_g_entries[IDL_DDIS_CLOSE_STREAM]))(
        		&p_idl_state->IDL_ddis_stream_handle );
#else
        ddis$close_stream( &p_idl_state->IDL_ddis_stream_handle );
#endif
	/* Free buffer only if we allocated it ourselves */
        if ((p_idl_state->IDL_buff_addr != NULL) &&
	    (p_idl_state->IDL_pickling_handle == NULL))
#ifdef DEBUG_ALLOC
        {
            fprintf(stderr, "rpc_ss_ddis_clean_up: free %lx\n",
                    p_idl_state->IDL_buff_addr);
            fflush(stderr);
#endif
            free( p_idl_state->IDL_buff_addr );
#ifdef DEBUG_ALLOC
        }
#endif
    }
    else if ((p_idl_state->IDL_ddis_state == IDL_ddis_first_unmarshall)
                || (p_idl_state->IDL_ddis_state == IDL_ddis_subsq_unmarshall))
    {
        p_idl_state->IDL_ddis_state = IDL_ddis_cleanup;
#ifdef VMS
	(*(ddis_g_entries[IDL_DDIS_CLOSE_STREAM]))(
        		&p_idl_state->IDL_ddis_stream_handle );
#else
        ddis$close_stream( &p_idl_state->IDL_ddis_stream_handle );
#endif
        if (p_idl_state->IDL_pickling_handle == NULL)
        {
            if (p_idl_state->IDL_elt_p->buff_dealloc
                    && p_idl_state->IDL_elt_p->data_len != 0)
            {
#ifdef DEBUG_ALLOC
                fprintf(stderr, "rpc_ss_ddis_clean_up: %s %lx (len=%d)\n",
            (p_idl_state->IDL_elt_p->buff_dealloc == free) ? "free" : "dealloc",
                        p_idl_state->IDL_elt_p->buff_addr,
                        p_idl_state->IDL_elt_p->data_len);
                fflush(stderr);
#endif
                (*(p_idl_state->IDL_elt_p->buff_dealloc))
                        (p_idl_state->IDL_elt_p->buff_addr);
                p_idl_state->IDL_elt_p->buff_dealloc = NULL;
            }
        }
    }
}

/******************************************************************************/
/*                                                                            */
/*  Report a DDIS error by raising an exception                               */
/*                                                                            */
/******************************************************************************/
void rpc_ss_raise_ddis_error
#ifdef IDL_PROTOTYPES
(
    idl_ulong_int ddis_status
)
#else
( ddis_status )
    idl_ulong_int ddis_status;
#endif
{
    switch (ddis_status)
    {
        case rpc_s_no_memory:
            RAISE( rpc_x_no_memory );
        case rpc_s_fault_pipe_comm_error:
            RAISE( rpc_x_ss_pipe_comm_error );
        default:
#ifdef VMS
            lib$signal( ddis_status );
#endif
            RAISE( rpc_x_coding_error );
    }
}

