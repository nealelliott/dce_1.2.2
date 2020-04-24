/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24cstub.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:00:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:40  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:49:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:44:32  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:37:03  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:39:33  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:40:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:02:37  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:41:53  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D24CSTUB_H
#define _D24CSTUB_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d24cstub_rcsid[] = "@(#)$RCSfile: d24cstub.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:39 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*
* daton **************************************************************
*                                                                    *
* TYPE         :  INCLUDE                                            *
*                                                                    *
* NAME         :  d24cstub.h  -  C-Stub Type-Definitions             *
*                                                                    *
* AUTHOR       :  Albert Biber                                       *
* DATE         :  05.09.88                                           *
*                                                                    *
* COMPONENT    :  Directory Service-V2 / C-Stub                      *
*                                                                    *
* DOK.-REF.    :  Design-Specification C-Stub                        *
*                                                                    *
* PRD#/VERS.   :                                                     *
*                                                                    *
* DESCRIPTION  :  Contain all C-Stub data- and typedef-defines       *
*                                                                    *
* SYSTEM-DEPENDENCIES:                                               *
*                                                                    *
*                                                                    *
* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   *
*    0.1   | 88-01-08 | Erstellung                  | ab |           *
*    0.2   | 89-02-23 | update for V201A            | jf |           *
* datoff *************************************************************
*/


/*
**********************************************************************
*                                                                    *
*           D E F I N E S                                            *
*                                                                    *
**********************************************************************
*/

/* ************** for V201A ************ */

/* status of ros-association */

#define NO_ROS_BIND             1
#define W_BIND                  2
#define W_REQUEST               3
#define W_RESULT                4
#define W_UNBIND                5
#define ABORTED                 6

/* status of ipc-association */

#define W_IPC_REQUEST           0
#define RET_REUSE_BIND          7
#define REQ_ROS_BIND            8
#define R_REQUEST               9
#define R_UNBIND                10

#define D24_MAX_ROS_ASS         16
#define D24_MAX_IPC_ASS         2 * D24_MAX_ROS_ASS

typedef struct {
	Bool            used ;          /* in_use indicator */
	Bool            crash ;         /* ipc crashed */
	unsigned short          ass_id ;        /* ipc_ass-id for return of result */
	int             invid ;         /* invoke-id */
	short           oprid ;         /* operation-id from ipcs */
	int             status ;        /* status of ipc_tab-entry */
	char            *res_buffer ;   /* area for result */
	char            *req_apdu ;     /* area of (complete) request */
	unsigned int    req_len ;       /* length of request_apdu */
	int             ros_assindex ;  /* index of associated ROS assoc. */
	unsigned short          cache_fctid ;   /* function-id for cache_update */
	unsigned short          cache_dirid ;   /* directory_id for cache-update */
	long            cache_sctrl ;   /* service-controls */
	short           file_id ;       /* file id used in tpdu-routines */
	char            *tp_read_data ; /* data-area used by tp_read */
	int             tp_read_len ;   /* length of read_area */
	int             tp_read_off ;   /* offset in tp_read_area */
	char            *tp_write_data; /* data area used by tp_write */
	long            tp_write_off ;  /* offset in tp_write_area */
	} D24_ipc_tab ;

typedef struct {
	R_idtype        ros_assid ;     /* ROS association id */
	int             ros_status ;    /* status */
	int             ros_acount ;    /* usage count  */
	int             ros_user ;      /* current user of association */
	char            *u_name ;       /* user name */
	unsigned int    u_name_len ;    /* length of user name */
	char            *u_pw ;         /* password */
	unsigned int    u_pw_len ;      /* length of user password */
	char            *dsa_info ;     /* dsa-info */
	unsigned int    dsa_info_len ;  /* length of dsa-info */
	char            *dsa_name ;     /* name of dsa */
	unsigned int    dsa_name_len ;  /* length of dsa-name */
	short		appl_cont_id ;	/* application context id */
	short		abs_synt_id ;	/* abstract syntax id */
	short		trs_synt_id ;	/* transfer syntax id */
	} D24_ros_tab ;

/* ************** end V201A ************ */

/*****   Common defines   *****/

#define   NOINIT	(111) 	     /* Status: no attach                */
#define   ATTACH	(222) 	     /* Status: client/server-attached   */
#define   NOBIND	(333) 	     /* Status: client no bind           */
#define   BINDED	(444) 	     /* Status: client-binded            */
#define   INVOKE	(555) 	     /* Status: client request-invoked   */

#define   D24_ERROR     (-1)	     /* Function has been performed with */
                                     /* failure                          */
#define   D24_MAX_SVR	(2)          /* Max. Number of IPCS-Server       */
#define   D24_MEMSIZE	(4000)       /* Length of IPCS Memory-Segment    */
#define   D24_PATHNAM	(100)        /* Length pathname of Trasnfer-File */
#define   D24_EVTINFO	(2048)       /* Length of Event-Info Block       */
#define   D24_GT_RETRY  (15)         /* Retries to get STUB PSAP-Address */ 
#define   D24_GT_DELAY	(60)         /* Delay in seconds between Retries */
#define   D24_AT_DELAY	(2)          /* Delay in seconds between Retries */


/*****   Error - Values   *****/

/************************      Call Errors     ***************************/

#define   D24_LIMASS    (11001)	     /* err_value: ROS Association-Limit */
                                     /*            exceeded              */
#define   D24_OPIDIL    (11002)	     /* err_value: Invoke Operation-ID   */
                                     /*            illegal               */
#define   D24_DRIDIL    (11003)	     /* err_value: Used Directory-ID     */
                                     /*            illegal               */
#define   D24_RBNDIL    (11004)      /* err_value: Invalid Bind-ID in    */
                                     /*            Invoke-APDU           */
#define   D24_RBEXST    (11005)      /* err_value: Bind to corresponding */
                                     /*            process already exist */
#define   D24_RBNEXT    (11006)      /* err_value: Bind to correpsonding */
                                     /*            process does't exist  */
#define   D24_ASEXST    (11007)      /* err_value: ROS-Association to    */
                                     /*            S-Stub already exist  */

/***********************   Temporary  Errors   ***************************/

#define   D24_EVTPIL    (12001)      /* err_value: Event-Type of the     */
                                     /*            received ROS-Event    */
                                     /*            not expected          */
#define   D24_IVIDIL    (12002)      /* err_value: Invoke-ID of the      */
                                     /*            received ROS-Event    */
                                     /*            invalid               */
#define   D24_ASIDIL    (12003)      /* err_value: Association-ID of the */
                                     /*            received ROS-Event    */
                                     /*            invalid               */
#define   D24_MMTPIL    (12004)      /* err_value: Memory-Type of the    */
                                     /*            received ROS-Event    */
                                     /*            invalid               */
#define   D24_RFNMIL    (12005)      /* err_value: File-Name of the      */
                                     /*            received ROS-Event    */
                                     /*            invalid               */
#define   D24_RSLNIL    (12006)      /* err_value: Length of the         */
                                     /*            received ROS Result-  */
                                     /*            Data invalid          */
#define   D24_ERLNIL    (12007)      /* err_value: Length of the         */
                                     /*            received ROS Error-   */
                                     /*            Data invalid          */
#define   D24_BRLNIL    (12008)      /* err_value: Length of the         */
                                     /*            received ROS Bind-    */
                                     /*            Response Data invalid */

#define   D24_ABUSER    (12009)      /* err_value: ROS-Request aborted   */
                                     /*            by the user (remote)  */
#define   D24_ABPROV    (12010)      /* err_value: ROS-Request aborted   */
                                     /*            by the ROS-provider   */


/***********************   Permanent  Errors   ***************************/

#define   D24_TRFOPN    (13100)      /* err_value: could not open the    */
                                     /*            ROS Transfer-File     */
#define   D24_TRFRED    (13200)      /* err_value: could not read the    */
                                     /*            ROS Transfer-File     */
#define   D24_TRFWRT    (13300)      /* err_value: could not write the   */
                                     /*            ROS Transfer-File     */
#define   D24_TRFCLS    (13400)      /* err_value: could not close the   */
                                     /*            ROS Transfer-File     */
#define   D24_TRFULK    (13500)      /* err_value: could not unlink the  */
                                     /*            ROS Transfer-File     */

#define   D24_CACERR    (18200)      /* err_value: no CACHE process for  */
                                     /*            this installation     */
#define   D24_DSNERR    (18400)      /* err_value: no correct STUB-Name  */
                                     /*            entry in the CACHE    */
/*****   ROS defines   *****/

#define   D24_MAX_ROSASS    (5)          /* ROS Association-Maximum      */
#define   D24_ROS_TIMOUT    (60)         /* ROS Check-Wait Timeout       */
#define   D24_RSLT_LENG     R_INFIN      /* ROS Result Length            */

#define   D24_ROS_VERSN     (1)          /* ROS Version Number           */
#define   D24_ROS_OCLAS     R_OPC2       /* ROS Operation-Class          */
#define   D24_ROS_ACLAS     R_ACL3       /* ROS Association-Class        */


/*****  Trace Defines  *****/

#define   D24_TRACESIZE     (128)       /* Max. size of trace file in KB */
#define   D24_TRACEFILE     "/trace_"
					/* Name of the C-Stub trace file */
#define   D24_TRNS_FILE     "/ros_"
                                        /* Name of the ROS Transfer-File */
#define   D24_NOTRACE	0		/* trace-level :  no trace       */
#define   D24_SHTRACE	1		/* trace-level :  short trace    */
#define   D24_TMTRACE   2               /* trace-level :  time trace     */
#define   D24_FUTRACE   3               /* trace-level :  full trace     */

#define   D24_1TRMSG    "\n*** --- C-STUB SERVICE REQUEST --- ***\n"
#define   D24_2TRMSG    "> OPERATION-ID: %d INVOKE-ID: %d INVOKE-TIME: %s"
#define   D24_3TRMSG    "> %d. PART OF THE REQUEST MESSAGE RECEIVED\n"
#define   D24_4TRMSG    "> CONTENT OF THE REQUEST MESSAGE:\n"
#define   D24_5TRMSG    "< %d. PART OF THE RESULT MESSAGE TRANSMITTED\n"
#define   D24_6TRMSG    "< SERVICE ERROR (code = %d class = %d value = %d)\n"
#define   D24_7TRMSG	"< SERVICE REQUEST TERMINATED\n"
#define   D24_8TRMSG	"< CONTENT OF THE RESULT MESSAGE:\n"
#define   D24_9TRMSG    "< \nTIMETRACE, %s, diff: %1.0f msec\n"



/*
**********************************************************************
*                                                                    *
*          T Y P E D E F  -  D E F I N I T I O N S                   *
*                                                                    *
**********************************************************************
*/

typedef struct
  {
    char       *tpin_fname;      /* -->  Pathname of working-file       */
    char       *tpin_bfref;      /* -->  Pointer to the Input-Data      */
    unsigned short	tpin_bflen;      /* -->  Length of the Input-Data       */

    R_mtype    *tprs_mtype;      /* <--  Memory-Type of the Result-Data */
  } D27_tpros_info;

typedef struct
  {
    unsigned short	ptadr_len;        /* Length of the PSAP-Address */
    char        ptadr_buf[80];    /* Current PSAP-Address */
  } D24_ptref;

#endif /* _D24CSTUB_H */
