/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpdu.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:02:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:45  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:28  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:33  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:56:13  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:13  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:18:46  keutel
 * 	creation
 * 	[1994/03/21  15:52:55  keutel]
 * 
 * $EndLog$
 */
#ifndef _TPDU_H
#define _TPDU_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _tpdu_rcsid[] = { "@(#) $RCSfile: tpdu.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:29 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * Copyright(C)                                                    */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transfered.                             */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : tpdu.h                                              */
/*                                                                    */
/* AUTHOR       : V.Laurent/Johann Mueller                            */
/* DATE         : 91-11-27                                            */
/*                                                                    */
/* COMPONENT    : TPDU external interface                             */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*                - Operating system:                                 */
/*                     SINIX  : -                                     */
/*                     BS2000 : -                                     */
/*                     MS-DOS : RP0000_MicV0600                       */
/*                                                                    */
/*                - Function prototyping:                             */
/*                     used   : __STDC__                              */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)tpdu.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. |  Date   | Modifications                  | KZ | CR# FM# */
/*ROS V1.0  | 09.12.86| Original                       | V.L|         */
/*ROS V2.0  | 01.10.87| new version                    |rue |         */
/*ROS V2.0  | 21.01.88| new errorvalue filename        |rue |         */
/*ROS V2.0  | 03.08.88| new struct TP_functions        |LL  |         */
/*          | 18.08.88| tp_version changed to tpversion|rue |         */
/*          |         | tp_ret-code changed to         |    |         */
/*          |         | tpret_code                     |    |         */
/*          |         | tperr_class changed to         |    |         */
/*          |         | tperr_class                    |    |         */
/*          |         | tperr_value changed to         |    |         */
/*          |         | tperr_value                    |    |         */
/*          | 00.00.90| export's of entries            |    |         */
/*ROS V2.5  | 20.02.91| adaptation for MS-DOS/Windows  | hm |         */
/*          |         | (far pointers, function decl.) |    |         */
/*RTS V2.0  | 06.10.91| Support of position and delete |    |         */
/*          |         | functions.                     | jm |         */
/*RTS V2.0  | 06.10.91| Paramers for "number of bytes" |    |         */
/*          |         | in tp_read and tp_write set to |    |         */
/*          |         | TP_long.                       | jm |         */
/*RTS V2.0  | 27.11.91| __STDC__ used                  | hm |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Declaration of FAR and PASCAL ****************** */
/* ****************** (MS-DOS only)                 ****************** */

#ifdef  RP0000_MicV0600
#define F far
#define P pascal
#else
#define F
#define P
#endif

/* ****************** TPDU return codes ****************************** */
/* ******************                   ****************************** */

#define TP_NO_ERR     0                    /* no error             */
#define TP_CALL_ERR  -1                    /* call error           */
#define TP_PERM_ERR  -2                    /* permanent error      */
#define TP_TEMP_ERR  -99                   /* temporary error      */

/* ****************** TPDU error class values ************************ */
/* ******************                         ************************ */

#define TP_ILL_STAT  1                      /* illegal call status  */
#define TP_ILL_VERS  2                      /* illegal version      */
#define TP_NOT_SPEC  3                      /* not specified error  */
#define TP_PAR_ERR   4                      /* formal param. error  */

/* ****************** Constants ************************************** */
/* ******************           ************************************** */

#define TP_YES   1
#define TP_NO    0
#define TP_TRUE  1                            /* for type TP_bool  */
#define TP_FALSE 0

/* ****************** Error Values *********************************** */
/* ******************              *********************************** */

#define TPINTERR       1 /* internal error */
#define TPOPENID       2 /* unknown tpopenid */
#define TPINBUFFER     3 /* wrong inbuffer address */
#define TPINREQUIRED   4 /* wrong number of bytes to be read */
#define TPOUTBUFFER    5 /* wrong outbuffer address */
#define TPOUTREQUIRED  6 /* wrong number of bytes to write */
#define TPSECURE       7 /* not immedeately stored */
#define TPDELREQUIRED  8 /* wrong number of bytes to delete */
#define TPNOSUPVS      9 /* not supported version */
#define TPDIRUSE      10 /* wrong transfer type */
#define TPFILENAME    11 /* wrong file name */
#define TPNOSUPFCT    12 /* wrong file name */

/* ****************** Interface version numbers ********************** */
/* ******************                          *********************** */

#define TP2MINVERS         TP2V02       /* minimal version number TPDU */
#define TP2MAXVERS         TP2V02       /* maximal version number TPDU */
#define TPVERSION          TP2V02       /* version number TPDU         */
#define TP2V02             2            /* version number TPDU         */

/* ****************** TPDU data types ******************************** */
/* ******************                 ******************************** */

typedef  short       TP_bool   ;      /* TP_TRUE  |  TP_FALSE          */
typedef  short       TP_short  ;      /* -32768 ... +32767             */
typedef  unsigned short TP_ushort ;   /*  0 ...  +65535                */
typedef  char        TP_char   ;      /*  1 character                  */
typedef  long        TP_long   ;      /*  -2^31 ... +2^31              */


typedef enum  {TPIN,TPOUT} Tpdir;     /* transfer type :               */
                                      /* read or write                 */

/* ******************************************************************* */
/* ****************** TPDU function parameter blocks ***************** */
/* ******************************************************************* */

/* ****************** Parameter block header ************************* */
/* ******************                        ************************* */

typedef struct {
                TP_short   tpversion;          /* TPDU version number  */
                TP_short   tpret_code;         /* return code          */
                TP_short   tperr_class;        /* error class          */
                TP_short   tperr_value;        /* error value          */
               }
               TP_pbhead ;

/* ****************** TPDU open ************************************** */
/* ******************           ************************************** */

typedef struct  {
        TP_pbhead F *tp_pbhead;             /* <-> standard header        */
        TP_short  F *tpopenid;              /* <-  file identifier        */
        TP_char   F *tpdata_description;    /* ->  file name              */
        TP_ushort F *tplen_data_description;/* ->  length of the filename */
        Tpdir     F *tpdir;                 /* ->  transfer type:         */
                                            /*     read or write          */
        TP_long   F *tpfilelength;          /* <-  filelength             */
                }
                Ptp1001;

/* ****************** TPDU read ************************************** */
/* ******************           ************************************** */

typedef struct  {
        TP_pbhead F *tp_pbhead;               /* <-> standard header      */
        TP_short  F *tpopenid;                /* ->  file identifier      */
        TP_char   F *tpinbufferaddress;       /* ->  buffer address       */
        TP_long   F *tpinrequired_data_length;/* ->  number of bytes      */
                                              /*     to be read           */
        TP_long   F *tpread_data_length;      /* <-  number of bytes read */
        TP_bool   F *tpinfurther_data;        /* <-  exist further data   */
                                              /*     in this file:        */
                                              /*     TP_YES - TP_NO       */
                }
        Ptp1002;

/* ****************** TPDU write ************************************* */
/* ******************            ************************************* */

typedef struct  {
        TP_pbhead F *tp_pbhead;                 /* <-> standard header  */
        TP_short  F *tpopenid;                  /* ->  file identfier   */
        TP_char   F *tpoutbufferaddress;        /* ->  buffer address   */
        TP_long   F *tpoutrequired_data_length; /* ->  number of bytes  */
                                                /*     to be written    */
        TP_long   F *tpwritten_data_length;     /* <-  number of bytes  */
                                                /*     written          */
        TP_bool   F *tpoutfurtherdataspace;     /* <-  free space in the*/
                                                /*     file ? YES - NO  */
        TP_bool   F *tpsecure;                  /* ->  store immedeately*/
                                                /*     (TP_YES/TP_NO)   */
                }
        Ptp1003;

/* ****************** TPDU position ********************************** */
/* ******************               ********************************** */

typedef struct {
       TP_pbhead F *tp_pbhead;          /* <-> standard header          */
       TP_short  F *tpopenid;           /* ->  file identifier          */
       TP_long   F *tpoffset;           /* ->  distance relative to the */
                                        /*     position after TP_OPEN   */
               }
       Ptp1004;

/* ****************** TPDU close ************************************* */
/* ******************            ************************************* */

typedef struct  {
        TP_pbhead F *tp_pbhead;                 /* <-> standard header */
        TP_short  F *tpopenid;                  /* ->  file identifier */
                }
        Ptp1005;

/* ****************** TPDU delete ************************************ */
/* ******************             ************************************ */

typedef struct  {
        TP_pbhead F *tp_pbhead;              /* <-> standard header    */
        TP_short  F *tpopenid;               /* ->  file identifier    */
        TP_long   F *tprequired_data_length; /* ->  number of bytes    */
                                             /*     which are not      */
                                             /*     deleted.           */
        TP_long   F *tpdata_length;          /* <-  actual file length */
                }
        Ptp1006;

/* ****************** TPDU multiple TPDU ***************************** */
/* ******************                    ***************************** */

/* The following structure is needed when multiple TPDU - functions    */
/* are used.                                                           */

/* datatype written as required for GNU */
typedef struct {
  TP_short (* open)    (Ptp1001 F *);
                           /* -> ptr to RTS user impl open function    */
  TP_short (* write)   (Ptp1003 F *);
                           /* -> ptr to RTS user impl write function   */
  TP_short (* read)    (Ptp1002 F *);
                           /* -> ptr to RTS user impl read function    */
  TP_short (* position)(Ptp1004 F *);
                           /* -> ptr to RTS user impl position func.   */
  TP_short (* tpdelete)(Ptp1006 F *);
                           /* -> ptr to RTS user impl delete functions */
  TP_short (* close)   (Ptp1005 F *);
                           /* -> ptr to RTS user impl close functions  */
  TP_char  *filename;
  TP_short *file_id;
  } TP_functions;


/* ****************** Function prototyping / Function declaration **** */
/* ******************                                             **** */

#ifdef __STDC__

extern TP_short  F  P  tp2_001_open     (Ptp1001  F  *);
extern TP_short  F  P  tp2_002_read     (Ptp1002  F  *);
extern TP_short  F  P  tp2_003_write    (Ptp1003  F  *);
extern TP_short  F  P  tp2_004_position (Ptp1004  F  *);
extern TP_short  F  P  tp2_005_close    (Ptp1005  F  *);
extern TP_short  F  P  tp2_006_delete   (Ptp1006  F  *);
#else

extern TP_short  tp2_001_open    ();
extern TP_short  tp2_002_read    ();
extern TP_short  tp2_003_write   ();
extern TP_short  tp2_004_position();
extern TP_short  tp2_005_close   ();
extern TP_short  tp2_006_delete  ();
#endif

#endif	/* _TPDU_H */
