/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cget.h,v $
 * Revision 1.1.12.2  1996/02/18  23:35:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:13  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:59:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:15  root]
 * 
 * Revision 1.1.10.3  1994/05/10  15:55:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:08  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:18:15  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:19  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  18:07:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:42:20  marrek]
 * 
 * Revision 1.1.8.3  1993/10/14  17:27:39  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:12  keutel]
 * 
 * Revision 1.1.8.2  1993/10/13  17:32:21  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:26:47  keutel]
 * 
 * Revision 1.1.8.1  1993/10/07  19:42:52  zeliff
 * 	Defect 8538, 9062 - Added KE_POS and KS_POS
 * 	[1993/10/07  12:55:06  zeliff]
 * 
 * Revision 1.1.6.3  1993/08/10  12:11:08  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:36:41  marrek]
 * 
 * Revision 1.1.6.2  1993/07/02  15:08:28  marrek
 * 	Changes for port to HP800.
 * 	[1993/07/02  14:42:17  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:38:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:32  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  03:44:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:40:42  zeliff]
 * 
 * $EndLog$
 */
#ifndef _CGET_H
#define _CGET_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cget_rcsid[] = "@(#)$RCSfile: cget.h,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 23:35:20 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/************************************************************************/
/*      Definitions for input/output on screen				*/
/************************************************************************/


#define TABMAX   50             /* Max. number of inputfields           */ 

#define NORMAL    0
#define HALBHELL  2
#define UNDERLINE 4             /* bits in getmode  	                */
#define INVERS   16
#define DUNKEL   32
#define WAHL     64
#define WAHLERG 128
#define ALTER   256
#define CURSOR  512

#define FKTNO     0
#define FKTF1     1             /* return values of funktionkeys        */
#define FKTF2     2
#define FKTF3     3
#define FKTF4     4
#define FKTF5     5
#define FKTF6     6
#define FKTF7     7
#define FKTF8     8
#define FKTF9     9
#define FKTF10   10
#define FKTF11   11
#define FKTF12   12
#define FKTF13   13
#define FKTF14   14
#define FKTF15   15
#define FKTF16   16
#define FKTF17   17
#define FKTF18   18
#define FKTF19   19
#define FKTF20   20
#define FKTPRINT 21
#define FKTHELP  22
#define FKTSTART 23
#define FKTEND   24
#define FKTCR    25
#define FKTMENU  26
#define FKTDEL   27
#define FKTSCU   28
#define FKTSCD   29
#define FKTCRIM  30     /* immediately return value of CR               */
#define MAXFKT   31

/************************************************************************/
/*      Exit-routine for csay/cget/cread                                */
/*                                                                      */
/*      The address of the exit-routine will be stored in field getexit */
/*      It will be called with						*/
/*                                                                      */
/*      *(getexit)(mode,pfeld,pmaske,eingabe)                           */
/*      int        mode;        entry|input|exit    		        */
/*      char      *pfeld;       given from csay/cget			*/
/*      char      *pmaske;      given from csay/cget			*/
/*      char       eingabe;     inputcharacter for mode input	        */
/*                                                                      */
/*      RETURN-Value: address of the field				*/
/************************************************************************/
#define GETFSTA  0              /* entry field                          */
#define GETFCHA  1              /* input in field                       */
#define GETFEND  2              /* exit field                           */

#define KTR     ':'             /* identification for a comment		*/

#define getfill cgetfill	/* nameconflict with libnobj.a		*/

int      getoutc  (char);       /* not released to user			*/
signed32 getentry (void);       /* not released to user			*/
char    *getform  (signed32, char *, char *, unsigned char);
				/* Standard-Exitroutine   */
signed32 getsmod  (void);       /* not released to user			*/
signed32 ungetsmod (void);	/* not released to user			*/
signed32 getadj   (void);       /* not released to user			*/
signed32 getwahl  (signed32, signed32, char *); /* not released to user	*/
signed32 getfkt   (signed32);   /* not released to user			*/
void     gettdel  (int);        /* not released to user			*/
signed32 geterror (char *);     /* not released to user			*/
signed32 cgetinit (void);       /* initialize say/get/read     		*/
signed32 cgetterm (void);       /* terminate say/get/read               */
signed32 cerase   (void);       /* erase screen				*/
signed32 cbell    (void);       /* bell                                 */
signed32 csay     (signed32, signed32, char *, char *);  /* output	*/
signed32 cget     (signed32, signed32, char *, char *);  /* input	*/
signed32 cgetfkt  (signed32);   /* get functionkey			*/
signed32 cread    (void);       /* read input				*/
signed32 cforts   (void);       /* skip to new testcase			*/
signed32 csearch_cap_str(char *cap, signed32 first, signed32 last);

#ifndef CGET_UEB                /* application program from user ??     */

extern char *pesc_GS;           /* ESC-seq. grafic on			*/
extern char *pesc_GE;           /* ESC-seq. grafic off			*/
extern char *pesc_GV;           /* grafic: Line vertical                */
extern char *pesc_GH;           /* grafic: Line horizontal              */
extern signed32 getmode;        /* outputmodus                          */
extern char getdel[];           /* border inputfield	                */
extern char getfill;            /* inputfield			        */
extern signed32  getdl;         /* Delta- line                          */
extern signed32  getdc;         /* Delta- column                        */
extern char *(*getexit)(signed32, char *, char *, char); /* Exitroutine */

#else

typedef struct {
	char            *termname;      /* name in termcap              */
	char            *esc;           /* ESC-seq. without termcap     */
	signed32        fkt;            /* function                     */
} Escap_seq;

#define FKTTAST         256
#define FKTCURS_RIGHT   MAXFKT +  1
#define FKTCURS_LEFT    MAXFKT +  2
#define FKTCURS_DOWN    MAXFKT +  3
#define FKTCURS_UP      MAXFKT +  4
#define FKTCURS_HOME    MAXFKT +  5
#define FKTTAB_LEFT     MAXFKT +  6
#define FKTTAB_RIGHT    MAXFKT +  7
#define FKTBS           MAXFKT +  8
#define FKTDELCHAR      MAXFKT +  9
#define FKTDELLINE      MAXFKT + 10
#define FKTINSCHAR      MAXFKT + 11
#endif

/* Name of the environment variable which could contain the	*/
/* adm_term_cap	file to use.					*/
#define ADM_TERM_CAP	"GDS_TERM_CAP"
/* else use this						*/
#define TERMFNMAX	100
#define TERMFNAME	"/adm/adm_term_cap"

#define CL_POS		18
#define CM_POS		19
#define BL_POS		20
#define SE_POS		21
#define AE_POS		22
#define US_POS		23
#define UE_POS		24
#define SO_POS		25
#define KS_POS		26
#define KE_POS		27

/* no of terminal capabilities used in admin.	*/
#define NO_USED_CAP	17

#define MAXCOMMENT	200	/* Max. length of comment string */

#endif /* _CGET_H */
