/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: mtsbacke.c,v $
 * Revision 1.1.6.2  1996/02/18  18:52:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:39  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:20:07  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  20:50 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:04 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:09  bfc]
 * 	 *
 * 	may 95 idl drop
 * 	[1995/10/21  22:58:10  bfc]
 * 	 *
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:55  bfc]
 * 	 *
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/05  20:11 UTC  lmm
 * 	Merge idl++ fixes into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_idlxx_fixes/1  1994/12/07  16:30 UTC  lmm
 * 	add cmd_opt as param to BE_gen_c_header for no prototype fix
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/01  23:56 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_merge_idlxx/1  1994/11/01  23:52 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:32 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/24  19:41  lmm
 * 	merge kk and hpdce01
 * 	[1995/12/07  21:12:25  root]
 * 
 * 	Merged tracing functionality into new 1.0.3 backend
 * 	[1993/10/21  20:37:09  lmm]
 * 
 * Revision 1.1.2.2  1993/07/07  20:01:28  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:33:30  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1991, 1992, 1993 by
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
**  NAME:
**
**      mtsbacke.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Backend control for MTS compiler
**
*/

#include <nidl.h>
#include <ast.h>
#include <irep.h>       /* Intermediate Representation defs */
#include <command.h>
#include <ddbe.h>       /* Data Driven Backend defs */
#include <mtsbacke.h>
#include <cspeldcl.h>
#include <cspell.h>
#include <cstubmts.h>
#include <sstubmts.h>
#include <hdgen.h>
#include <driver.h>
#include <forhdgen.h>
#include <message.h>



/*
 * Global type instances and boolean
 */
boolean *BE_cmd_opt;
void    **BE_cmd_val;
AST_type_n_t *BE_ulong_int_p, *BE_ushort_int_p, *BE_pointer_p, *BE_function_p;
AST_type_n_t *BE_short_null_p, *BE_long_null_p, *BE_hyper_null_p;
boolean BE_space_opt, BE_bug_array_align, BE_bug_array_align2,
        BE_bug_boolean_def
                                                                ;

#ifdef DUMPERS
boolean BE_dump_debug, BE_dump_flat, BE_dump_mnode, BE_dump_mool,
        BE_dump_recs, BE_dump_sends, BE_dump_unode, BE_dump_uool,
        BE_zero_mem;
#endif

/*
 *  be_init
 *
 * Initialize the various backend globals
 */
static void be_init
#ifdef PROTO
(
    boolean *cmd_opt,
    void **cmd_val
)
#else
(cmd_opt, cmd_val)
    boolean *cmd_opt;
    void **cmd_val;
#endif
{
    boolean *bugs;

    /*
     * initialize various options-related globals
     */
    BE_cmd_opt = cmd_opt;
    BE_cmd_val = cmd_val;
    BE_space_opt = cmd_opt[opt_space_opt];
#ifdef DUMPERS
    BE_dump_debug = cmd_opt[opt_dump_debug];
    BE_dump_flat  = cmd_opt[opt_dump_flat];
    BE_dump_mnode = cmd_opt[opt_dump_mnode];
    BE_dump_mool  = cmd_opt[opt_dump_mool];
    BE_dump_recs  = cmd_opt[opt_dump_recs];
    BE_dump_sends = cmd_opt[opt_dump_sends];
    BE_dump_unode = cmd_opt[opt_dump_unode];
    BE_dump_uool  = cmd_opt[opt_dump_uool];
    BE_zero_mem   = (getenv("IDL_zero_mem") != NULL);
#endif

    /*
     *  Backwards compatibility marshalling
     */
    bugs = (boolean *)cmd_val[opt_do_bug];
    BE_bug_array_align = bugs[bug_array_align];
    BE_bug_array_align2 = bugs[bug_array_align2];
    BE_bug_boolean_def = bugs[bug_boolean_def];


    /*
     * initialize global type instances
     */
    BE_ulong_int_p = BE_get_type_node (AST_long_unsigned_k);
    BE_ulong_int_p->alignment_size = 4;
    BE_ulong_int_p->ndr_size = 4;

    BE_ushort_int_p = BE_get_type_node (AST_short_unsigned_k);
    BE_ushort_int_p->alignment_size = 2;
    BE_ushort_int_p->ndr_size = 2;

    BE_pointer_p = BE_get_type_node (AST_pointer_k);
    BE_pointer_p->alignment_size = 4;
    BE_pointer_p->ndr_size = 4;

    BE_function_p = BE_get_type_node (AST_function_k);
    BE_function_p->alignment_size = 0;
    BE_function_p->ndr_size = 0;

    BE_short_null_p = BE_get_type_node (AST_null_k);
    BE_short_null_p->alignment_size = 2;
    BE_short_null_p->ndr_size = 0;

    BE_long_null_p = BE_get_type_node (AST_null_k);
    BE_long_null_p->alignment_size = 4;
    BE_long_null_p->ndr_size = 0;

    BE_hyper_null_p = BE_get_type_node (AST_null_k);
    BE_hyper_null_p->alignment_size = 8;
    BE_hyper_null_p->ndr_size = 0;

}



/******************************************************************************/
/* BE Temporary-Memory Management                                             */
/******************************************************************************/
/* ABSTRACT:                                                                  */
/*   Special BE memory management routines.  The following three routines     */
/*   provides memory management in contexts (heap zones).  When entering a    */
/*   context the BE_push_malloc_ctx routine is called, upon entering a        */
/*   context all calls to BE_ctx_malloc to allocate memory will be associated */
/*   with the context.  When the context is exited by calling                 */
/*   BE_pop_malloc_ctx, all memory allocated with BE_ctx_malloc, is freed.    */
/*                                                                            */
/* NOTE:                                                                      */
/*   Memory allocated via BE_ctx_malloc, cannot be freed other than by        */
/*   exiting the current malloc context as it adds a header to the memory     */
/*   in order to keep a list of active allocations.  Calls to free() with     */
/*   allocations returned from BE_ctx_malloc, will cause heap corruption.     */
/*                                                                            */
/* ROUTINES:                                                                  */
/*   BE_cxt_malloc  -- Same interface as MALLOC, if not within a context      */
/*                     returns memory directly from malloc()                  */
/*   BE_push_malloc_ctx -- Start new temporary context                        */
/*                         is used directly and the memory never freed        */
/*   BE_push_perm_malloc_ctx -- Start new temporary context in which MALLOC   */
/*   BE_pop_malloc_ctx -- Free all memory allocated since start of context    */
/*                                                                            */
/******************************************************************************/

/*
 * Type used to add our context header to allocations returned from MALLOC
 */
typedef struct malloc_t {
      struct malloc_t *next;    /* Pointer to next allocation on this chain */
      char *data;               /* Start of memory returned to caller */
      } malloc_t;

/*
 * Type used to maintain a list of allocation contexts.
 */
typedef struct malloc_ctx_t {
      struct malloc_ctx_t *next;    /* Pointer to next context */
      malloc_t *list;               /* Head of allocation chain for this context. */
      boolean permanent;            /* If true, this is a permanent context */
      } malloc_ctx_t;

/*
 * Current malloc context, initially NULL so normal MALLOC is used
 */
static malloc_ctx_t *malloc_ctx = NULL;

/*
** BE_ctx_malloc: allocate memory in the current context.
*/
heap_mem *BE_ctx_malloc
#ifdef PROTO
(
    int size
)
#else
(size)
    int size;
#endif
{
      malloc_t *new;

      /* If no malloc context, just return memory */
      if (malloc_ctx == NULL) return MALLOC(size);

      /* If current malloc context is permanent, then just return memory */
      if (malloc_ctx->permanent == true) return MALLOC(size);

      /* Allocate memory with our context header */
      new = (malloc_t *)MALLOC(size + sizeof(malloc_t));

      /* Link the new allocation on the current context list */
      new->next = malloc_ctx->list;
      malloc_ctx->list = new;

#ifdef DUMPERS
      /* If BE_zero_mem set, initialize allocated memory to help find bugs */
      if (BE_zero_mem) memset(&new->data, 0xFF, size);
#endif

      /* Return the value after our header for use by the caller */
      return (heap_mem*)&new->data;
}

/*
** BE_push_malloc_ctx: Push a new context in which memory is allocated
*/
void BE_push_malloc_ctx
#ifdef PROTO
(
      void
)
#else
()
#endif
{
      /*
       * Allocate a malloc context block to hang allocations made in this
       * context off of.
       */
      malloc_ctx_t *new = (malloc_ctx_t*)MALLOC(sizeof(malloc_ctx_t));

      /* Link new context on the top of the context stack */
      new->next = malloc_ctx;
      new->list = (malloc_t*)NULL;
      new->permanent = false;
      malloc_ctx = new;
}

/*
** BE_pop_malloc_ctx: Pop the current context, freeing all memory allocated
** within this context (unless it was a permanent context).
*/
void BE_pop_malloc_ctx
#ifdef PROTO
(
    void
)
#else
()
#endif
{
      malloc_t *list,*curr;
      malloc_ctx_t *ctx;

      /* If we are called with an empty stack, then abort */
      if (malloc_ctx == NULL)
          error(NIDL_INTERNAL_ERROR,__FILE__,__LINE__);

      /* Loop through the context freeing all memory */
      list = malloc_ctx->list;
      while (list != NULL)
      {
          curr = list;
          list = list->next;
          free(curr);
      }

      /* Remove context from the stack, and free the context header */
      ctx = malloc_ctx;
      malloc_ctx = malloc_ctx->next;
      free(ctx);
}


/*
 *  BE_main
 */
boolean BE_main              /* returns true on successful completion */
#ifdef PROTO
(
    boolean             *cmd_opt,   /* [in] array of cmd option flags */
    void                **cmd_val,  /* [in] array of cmd option values */
    FILE                *h_fid,     /* [in] header file handle, or NULL */
    FILE                *caux_fid,  /* [in] client aux file handle, or NULL */
    FILE                *saux_fid,  /* [in] server aux file handle, or NULL */
    FILE                *cstub_fid, /* [in] cstub file handle, or NULL */
    FILE                *sstub_fid, /* [in] sstub file handle, or NULL */
    FILE                *cxxmgr_fid,/* [in] C++ mgr class file hndle, or NULL */
    AST_interface_n_t   *int_p      /* [in] ptr to interface node */
)
#else
(cmd_opt, cmd_val, h_fid, caux_fid, saux_fid, cstub_fid, sstub_fid, cxxmgr_fid,int_p)
    boolean *cmd_opt;
    void **cmd_val;
    FILE *h_fid;
    FILE *caux_fid;
    FILE *saux_fid;
    FILE *cstub_fid;
    FILE *sstub_fid;
    FILE *cxxmgr_fid;
    AST_interface_n_t *int_p;
#endif
{
    DDBE_vectors_t      *dd_vip;    /* Data driven BE vector information ptr */

    be_init (cmd_opt, cmd_val);

    /* Generate the intermediate representation if stubs are required. */
    if (cstub_fid || sstub_fid)
        IR_gen_irep(cmd_opt, cmd_val, int_p);

    /* Print accumulated errors and warnings generated by irep, if any. */
    if (!cmd_opt[opt_confirm])
        print_errors();

#ifdef DUMPERS
    /* Dump the IREP if requested. */
    if (cmd_opt[opt_dump_flat])
        IR_dump_irep(int_p);
#endif

    /* Call the Data Driven Backend if stubs are required. */
    if (cstub_fid || sstub_fid)
        DDBE_main(cmd_opt, cmd_val, int_p, &dd_vip);

    if (h_fid)
    {
        BE_gen_c_header(h_fid, int_p,
            (boolean *)cmd_val[opt_do_bug], cmd_opt[opt_cepv]
	    );
    }

    if (cxxmgr_fid && error_count == 0)
        BE_gen_cxx_class(cxxmgr_fid, int_p, mgr_class);
    /*
     * emit client stub file if requested
     */
    if (cstub_fid && error_count == 0)
        DDBE_gen_cstub(cstub_fid, int_p, lang_c_k,
            (char *)cmd_val[opt_header], cmd_opt, cmd_val, dd_vip);

    /*
     * emit server stub file if requested
     */
    if (sstub_fid && error_count == 0)
        BE_gen_sstub (sstub_fid, int_p, lang_c_k, (char *)cmd_val[opt_header],
                         cmd_opt, cmd_val, dd_vip);

    return (error_count == 0);
}

/*
 * Output #includes needed at the start of MTS stubs
 */
void CSPELL_mts_includes
#ifdef PROTO
(
    FILE *fid,
    char header_name[]
)
#else
(fid, header_name)
    FILE *fid;
    char header_name[];
#endif
{
    /* 
    ** Due to the multitude of possible symbol/macro clashes, we must 
    ** bring in the system RPC header files, (expecially the Windows RPC 
    ** header files), BEFORE the user-provided header files.
    **
    ** This used to be done conditionally only for MS RPC, but since macro
    ** clashes could also cause conflicts on other platforms, the 
    ** conditional _MSDOS has been removed.
    */

    fprintf (fid, INCLUDE_TEMPLATE, "idlddefs.h");
    fprintf (fid, USER_INCLUDE_TEMPLATE, header_name);


}

/*
 * BE_get_name
 *
 * Returns a character string given a NAMETABLE_id_t
 */
char *BE_get_name
#ifdef PROTO
(
    NAMETABLE_id_t id
)
#else
(id)
    NAMETABLE_id_t id;
#endif
{
    char *retval;

    NAMETABLE_id_to_string(id, &retval);
    return retval;
}

/*
 * BE_get_type_node
 *
 * Allocates and returns a type node
 */
AST_type_n_t *BE_get_type_node
#ifdef PROTO
(
    AST_type_k_t kind
)
#else
(kind)
    AST_type_k_t kind;
#endif
{
    AST_type_n_t *new_type = (AST_type_n_t *)BE_ctx_malloc(sizeof(AST_type_n_t));

    new_type->fe_info = NULL;
    new_type->be_info.other = NULL;
    new_type->name = NAMETABLE_NIL_ID;
    new_type->defined_as = NULL;
    new_type->kind = kind;
    new_type->flags = 0;
    new_type->xmit_as_type = NULL;
    new_type->rep_as_type = NULL;
    new_type->cs_char_type = NULL;

    return new_type;
}

/******************************************************************************/
/*                                                                            */
/*    Dummy - Control of generation of pipe routine declarations              */
/*                                                                            */
/******************************************************************************/
void BE_gen_pipe_routine_decls
#ifdef PROTO
(
    FILE *fid,
    AST_interface_n_t *p_interface
)
#else
( fid, p_interface )
    FILE *fid;
    AST_interface_n_t *p_interface;
#endif
{
}


