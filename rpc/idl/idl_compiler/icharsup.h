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
 * $Log: icharsup.h,v $
 * Revision 1.1.6.2  1996/02/18  23:45:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:13  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:18:34  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:04 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:11:50  root]
 * 
 * Revision 1.1.2.1  1995/10/31  21:15:32  bfc
 * 	may 5 version from nightly
 * 	[1995/10/31  20:44:44  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/10/31  20:42:18  bfc]
 * 
 * Revision 1.1.2.2  1993/07/07  20:45:46  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  20:43:46  ganni]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      icharsup.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Function prototypes and type definitions for international
**          character support
**
**
*/

#ifndef ICHARSUP_H
#define ICHARSUP_H

/* Description of an operation's use of I-char machinery */
typedef struct BE_cs_info_t {
    boolean cs_machinery;   /* TRUE if operation has I-char machinery */
    boolean stag_hidden;    /* TRUE if AST_HIDDEN_SET on [cs_stag] param */
    boolean stag_by_ref;    /* TRUE if passed by ref according to IDL */
    NAMETABLE_id_t  stag;
    boolean drtag_by_ref;    /* TRUE if passed by ref according to IDL */
    NAMETABLE_id_t  drtag;
    boolean rtag_by_ref;    /* TRUE if passed by ref according to IDL */
    NAMETABLE_id_t  rtag;
} BE_cs_info_t;

void BE_cs_analyze_and_spell_vars
(
#ifdef PROTO
    FILE *fid,                      /* [in] Handle for emitted C text */
    AST_operation_n_t *p_operation, /* [in] Pointer to AST operation node */
    BE_side_t side,                 /* [in] client or server */
    BE_cs_info_t *p_cs_info         /* [out] Description of I-char machinery */
#endif
);

void BE_spell_cs_state
(
#ifdef PROTO
    FILE *fid,                      /* [in] Handle for emitted C text */
    char *state_access,             /* [in] "IDL_ms." or "IDL_msp->" */
    BE_side_t side,                 /* [in] client or server */
    BE_cs_info_t *p_cs_info         /* [in] Description of I-char machinery */
#endif
);

void BE_spell_cs_tag_rtn_call
(
#ifdef PROTO
    FILE *fid,                      /* [in] Handle for emitted C text */
    char *state_access,             /* [in] "IDL_ms." or "IDL_msp->" */
    AST_operation_n_t *p_operation, /* [in] Pointer to AST operation node */
    BE_side_t side,                 /* [in] client or server */
    BE_handle_info_t *p_handle_info,/* [in] How to spell binding handle name */
    BE_cs_info_t *p_cs_info,        /* [in] Description of I-char machinery */
    boolean pickling
#endif
);

#endif
