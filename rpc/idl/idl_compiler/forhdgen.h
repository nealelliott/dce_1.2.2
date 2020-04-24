/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: forhdgen.h,v $
 * Revision 1.1.7.2  1996/03/09  20:44:54  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:37  marty]
 *
 * Revision 1.1.7.1  1995/12/07  22:23:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:03 UTC  dat
 * 	New file for second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:32:16  root]
 * 
 * Revision 1.1.2.1  1995/10/31  21:15:31  bfc
 * 	may 5 version from nightly
 * 	[1995/10/31  20:39:15  bfc]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      forhdgen.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Definitions and function prototypes for forhdgen.c
**
**  MODIFICATION HISTORY:
**
**  16-Jun-92 D.Annicchiarico   Created source file.  See source library for
**                              modification history.
*/
#ifndef FORHDGEN_H
#define FORHDGEN_H

#include <ast.h>
#include <files.h>
#include <nametbl.h>

#define DDBE_FOR_MAX_DIM 7      /* Maximum array dimensions */
#define for_fprintf FSPELL_output_line

extern int FSPELL_output_line(
#ifdef PROTO
    FILE * fid,                 /* [in] File handle */
    char *format,               /* [in] Format string */
    ...                         /* [in] 0-N format arguments */
#endif
);

extern char *FSPELL_strucpy(
#ifdef PROTO
    char    *dst,   /* [io] Destination upper-case string */
    char    *src    /* [in] Source mixed-case string */
#endif
);

extern char *FSPELL_id_to_uc_string(
#ifdef PROTO
    NAMETABLE_id_t      id,             /* [in] Nametable ID */
    char                uc_string[]     /* [io] Upcased string */
#endif
);

extern char *FSPELL_str_to_uc_string(
#ifdef PROTO
    STRTAB_str_t        str,            /* [in] String handle */
    char                uc_string[]     /* [io] Upcased string */
#endif
);

extern char *FSPELL_mapchar(
#ifdef PROTO
    AST_constant_n_t *const_p,  /* [in] AST constant node used for error case */
    char c,                     /* [in] Character to map to FORTRAN format */
    boolean warning_flag        /* [in] TRUE => emit warning if doesn't map */
#endif
);

extern void FSPELL_mapoctal(
#ifdef PROTO
    AST_constant_n_t *const_p,  /* [in] AST constant node used for error case */
    char    **istring,      /* [io] IDL string */
    char    **fstring,      /* [io] FORTRAN string */
    boolean warning_flag    /* [in] TRUE => emit warning if char doesn't map */
#endif
);

extern void FSPELL_maphex(
#ifdef PROTO
    AST_constant_n_t *const_p,  /* [in] AST constant node used for error case */
    char    **istring,      /* [io] IDL string */
    char    **fstring,      /* [io] FORTRAN string */
    boolean warning_flag    /* [in] TRUE => emit warning if char doesn't map */
#endif
);

extern char *FSPELL_mapstring(
#ifdef PROTO
    AST_constant_n_t *const_p,  /* [in] AST constant node used for error case */
    char    *istring,       /* [in] IDL string */
    char    *fstring,       /* [out] FORTRAN string */
    boolean warning_flag    /* [in] TRUE => emit warning if char doesn't map */
#endif
);

extern void BE_gen_fortran_header(
#ifdef PROTO
    FILE                *fid,           /* [in] Output file handle */
    AST_interface_n_t   *ifp            /* [in] Ptr to AST interface node */
#endif
);

#endif
