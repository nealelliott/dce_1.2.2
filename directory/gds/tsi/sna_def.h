/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: sna_def.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:49  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:49:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:06  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:42  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:11  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:16:02  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:09:33  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:38  marrek]
 * 
 * $EndLog$
 */
#ifndef _SNA_DEF_H
#define _SNA_DEF_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char sna_def_rcsid[] = "@(#)$RCSfile: sna_def.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:49 $";
#endif

/* #ident " @(#)sna_def.h	311.4 93/08/24 CMX (NC1)" */
/* @(#) /usr/snadmin/SCCS/Src/LU0/cmx/s.sna_def.h 8.1.1.1 91/04/15 */
/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1990
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */
 
/******************************************************************************/
/*                                                                            */
/*                                                                            */
/*      I C M X ( L )  < - >  S N A   V I A   T R A N S I T - L U 0           */
/*                                                                            */
/*                  C O M M O N    D E F I N E S                              */
/*                                                                            */
/*                                                                            */
/*      Subject  : Header file for cmxsna.c subroutines and TRANSIT-LU0 APs   */
/*                                                                            */
/*      sna_def.h: summarizes all common defines, data-formates and messages  */
/*                 (ie. structures) applicable to this interface.             */
/*                                                                            */
/*    !!caution  : any changes have to be agreed between STO NC and CGK TA2   */
/*               : line 1 contains SCCS-ID string for TRANSIT(SINIX) SCCS-sys */
/*               : line 2 contains SCCS-ID string for CMX SCCS-system         */
/*                                                                            */
/******************************************************************************/
 
/*
 *     #defines  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */
 
/*
 *  cmxsna.c uses _sna.... as function identifier
 *  cmx3k.c  uses _t30i... as function identifier (couple module extension
 *           flexibility )
 */
 
#define _snaattach      t_300attach
#define _snadetach      t_301detach
#define _snaconrq       t_302conrq
#define _snaconin       t_303conin
#define _snaconrs       t_304conrs
#define _snaconcf       t_305concf
#define _snadatarq      t_306datarq
#define _snadatain      t_307datain
#define _snadatago      t_308datago
#define _snadatastop    t_309datastop
#define _snaredrq       t_310redrq
#define _snaredin       t_311redin
#define _snaxdatrq      t_312xdatrq
#define _snaxdatin      t_313xdatin
#define _snaxdatgo      t_314xdatgo
#define _snaxdatstop    t_315xdatstop
#define _snadisrq       t_316disrq
#define _snadisin       t_317disin
#define _snaevent       t_318event
#define _snaerror       t_319error
#define _snainfo        t_320info
#define _snavdatarq     t_321vdatarq
#define _snavdatain     t_322vdatain
 
/*    (FF) */
 
/*      sna subroutine call identifier  <-----------------------------------  */
 
#define S_FATTACH        0
#define S_FDETACH        1
#define S_FCONRQ         2
#define S_FCONIN         3
#define S_FCONRS         4
#define S_FCONCF         5
#define S_FDATARQ        6
#define S_FDATAIN        7
#define S_FDATAGO        8
#define S_FDATASTOP      9
#define S_FREDRQ        10
#define S_FXDATRQ       11
#define S_FXDATIN       12
#define S_FXDATGO       13
#define S_FXDATSTOP     14
#define S_FREDIN        15
#define S_FDISRQ        16
#define S_FDISIN        17
#define S_FEVENT        18
#define S_FERROR        19
#define S_FINFO         20
#define S_FVDATRQ       21
#define S_FVDATIN       22
 
/*      limits  <------------------------------------------------------------ */
 
#define MAXANW          2                          /* maxnr of APs, processes */
#define FREE            0                         /* TSAP element free/in-use */
#define MYNAMLNG      124                 /* max-length of structure "myname" */
#define LUNAMLNG        8                                /* maxlength LU-name */
 
#define ON              1                              /* break LOOP criteria */
#define S_TRUE          1
#define S_FALSE         0
#define ERROR          -1
#define NOERROR         0
 
#define S_WAIT         35           /* timeout value attach (CMX/LU reaction) */
#define TMP_SIZE       50                  /* size internal user error buffer */
 
/*      controls named pipe access  <--------------------------------------   */
 
#define P_MODE    0010666                 /* mask bits for mknod "named pipe" */
#define RD_ONLY         0                                        /* read only */
#define WR_ONLY         1                                       /* write only */
 
#define PIPEFIXL       32                    /* pipe entry length of fix part */
#define DATMAXL      4096                   /* pipe entry maxlen of data part */
#define CRQUDMAXL      32             /* pipe entry maxlen userdata for conrq */
#define IRQUDMAXL      64             /* pipe entry maxlen userdata for disrq */
 
#define CMX_SIG   SIGTERM                  /* SIGNAL15 used for catch routine */
 
/*    (FF) */
 
/*
 *      structures  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */
 
/*      TSAP-Elementes  <---------------------------------------------------  */
 
typedef struct
        {
        short   e_kenn;                               /* element free or used */
        short   apmode;                                     /* attach ap_mode */
        char    tsapnam[MYNAMLNG];                /* area for struct "myname" */
        char    lunam[LUNAMLNG];                                   /* lu-name */
        } enam;
 
 
/*      "write"-pipe path name  <-------------------------------------------  */
 
typedef struct
        {
        char    wr_kenn[9];                           /* fix part '/tmp/sna_' */
        char    loc_adr[3];                              /* 'locaddr' (ASCII) */
        char    wr_num;                               /* internal number (=0) */
        char    end;                                                   /* EOS */
        char    wr_fill[2];                                      /* alignment */
        } wr_pipe_nam;
 
/*      "read"-pipe path name  <--------------------------------------------  */
 
typedef struct
        {
        char    rd_kenn[9];                           /* fix part '/tmp/sna_' */
        char    prozid[8];                      /* (own) process-ID hex-value */
        char    end;                                                   /* EOS */
        char    rd_fill[2];                                      /* alignment */
        } rd_pipe_nam;
 
/*      messages and data formats  <----------------------------------------  */
 
typedef struct
        {
        char    e_lu_name[8];                                  /* own LU-name */
        char    e_local_adr;                                   /* sna locaddr */
        char    t_fill;                                          /* alignment */
        char    p_lu_name[8];                           /* (partner-) LU-name */
        char    p_reg;                                       /* regionsnumber */
        char    p_pro;                                     /* processornummer */
        } trans_adr;
 
#define PIPEADDRL        (PIPEFIXL + sizeof (trans_adr))
 
typedef struct
        {
        unsigned short len_ges;                        /* total entry lengths */
        unsigned short fkt_aufruf;                /* function call identifier */
        int   proz_id;                                    /* process id (pid) */
	int   transref;                /* transport reference, CMX/LU assignes */
        int   param;                                      /* call parameter 0 */
        int   param1;                                     /*      parameter 1 */
        int   param2;                                     /*      parameter 2 */
        int   param3;                                     /*      parameter 3 */
        int   param4;                            /*      parameter 4 reserved */
        trans_adr transadr;              /* own and partner transport address */
        char  data[DATMAXL];                                     /* data area */
        } pipe_buf;
 
/*    (FF) */
 
/*
 *      macros for repetitive code reduction  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */
 
/*
 *      set specific error code  into _t_error  prior return to cmx2k.c
 */
 
#define RETURN_ERROR(a)         {                                       \
        _t_error = a;                                                   \
        return (T_ERROR);                                               \
}
 
/*
 *      check if at least one attach has been performed successfully
 */
 
#define CHECK_ATTACH()                                                  \
        if (_sna_att_flg == 0)                                          \
                RETURN_ERROR (T_WREQUEST);
 
/*
 *      check if *tref is valid
 */
 
#define CHECK_TREF()                                                    \
        if (*tref == 0)                                                 \
                RETURN_ERROR (T_WTREF);
 
/*
 *      write into named pipe, check for error, make trace entry
 */
 
#define WRITE_PIPE() {                                                  \
        while ((_sna_rw_length = write                                  \
        (_sna_fdw, &_sna_pipebuf, _sna_pipebuf.len_ges)) == ERROR       \
                && errno == EINTR);                                     \
        t_trc (T_FWRITE, 0, _sna_fdw, 0, _sna_pipebuf.len_ges,          \
                                                _sna_rw_length,errno);  \
        if (_sna_rw_length == ERROR) {                                  \
                s_erract ();    /* close pipes */                       \
                RETURN_ERROR (errno)                                    \
        }                                                               \
        t_trc (T_FSNAPIPE, 0, &_sna_pipebuf);                           \
}
 
/*
 *      clear read pipe (to recover from uncertain named pipe effects,
 *            or "impossible" interruption of previous pipe read)
 */
 
#define CLEAR_READ_PIPE(a) {                                            \
        if (s_rdpchk () == ERROR) {                                     \
                t_trc (T_FSTRING, 0, "RETURN_ERROR_CLEAR_READ_PIPE");   \
                return (T_ERROR);                                       \
        }                                                               \
        _sna_pipebuf. len_ges = PIPEFIXL;                               \
        _sna_pipebuf. fkt_aufruf = a;                                   \
        _sna_pipebuf. proz_id = t_pid;                                  \
}
#endif /* _SNA_DEF_H */
