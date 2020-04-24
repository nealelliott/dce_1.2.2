/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cget.c,v $
 * Revision 1.2.12.2  1996/02/18  19:40:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:22  marty]
 *
 * Revision 1.2.12.1  1995/12/08  15:16:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/17  18:37 UTC  dtruong
 * 	Merge from Hpdce02_01
 * 	[1995/12/08  14:51:42  root]
 * 
 * Revision 1.2.10.5  1994/07/06  15:06:24  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:11:16  marrek]
 * 	HP revision /main/HPDCE01/2  1994/03/16  21:33 UTC  dtruong
 * 	merge to main line
 * 
 * 	HP revision /main/HPDCE01/dtruong_mothra/1  1994/03/07  18:55 UTC  dtruong
 * 	Added routine to check hpterm
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/27  01:16  dtruong
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.7.5  1993/12/17  23:06:33  jake
 * 	GDS What string changes
 * 	[1993/12/17  23:06:16  jake]
 * 
 * Revision 1.2.10.4  1994/06/21  14:43:28  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:22:52  marrek]
 * 
 * Revision 1.2.10.3  1994/05/10  15:49:23  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:10:03  marrek]
 * 
 * Revision 1.2.10.2  1994/03/23  15:05:06  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:08:05  keutel]
 * 
 * Revision 1.2.10.1  1994/02/22  15:58:10  marrek
 * 	again
 * 	[1994/02/18  13:42:37  marrek]
 * 
 * 	February 1994 code drop
 * 	[1994/02/18  13:11:05  marrek]
 * 
 * 	REplace Fehler by error and increase LINE_LEN.
 * 	[1994/02/10  16:06:07  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:26:42  marrek]
 * 
 * Revision 1.2.8.6  1993/11/08  16:32:31  root
 * 	F5 is help key for xterm : Jochen Keutel
 * 	[1993/11/08  16:32:11  root]
 * 
 * Revision 1.1.7.4  1993/12/14  19:21:03  dtruong
 * 	<<<Fixed bug in gdsditadm. If a batch contains extra colon such as tower id
 * 	ncacn_ip_tcp:15.13.106.190[]. A colon used to seperate protocol family caused
 * 	the batch script screw up>>>
 * 	[1993/12/11  01:54:03  dtruong]
 * 
 * Revision 1.1.7.3  1993/11/04  19:06:25  dtruong
 * 	<<<Clean up hp softkeys before exit.>>>
 * 	[1993/11/04  19:01:11  dtruong]
 * 
 * Revision 1.1.7.2  1993/10/27  20:51:12  dtruong
 * 	<<<Added softkey for hp terminal>>>
 * 	[1993/10/27  20:47:35  dtruong]
 * 
 * Revision 1.1.3.4  1993/10/19  18:12:58  root
 * 	    Loading
 * 
 * Revision 1.2.8.4  1993/10/14  16:46:23  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:43:09  keutel]
 * 
 * Revision 1.2.8.4  1993/10/14  16:46:23  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  16:43:09  keutel]
 * 
 * Revision 1.2.8.3  1993/10/13  17:29:27  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  19:27:29  keutel]
 * 
 * Revision 1.2.8.2  1993/10/06  21:22:51  zeliff
 * 	Defect 8538 - fix escape sequences problem on OSF1
 * 	[1993/10/06  21:18:12  zeliff]
 * 
 * Revision 1.2.8.1  1993/09/08  14:23:43  damon
 * 	     Move code to use key pad on HP terminal to the right place.
 * 	     [1993/08/31  09:45:16  marrek]
 * 
 * Revision 1.2.6.3  1993/08/10  09:52:31  marrek
 * 	     July 1993 code drop.
 * 	     [1993/07/30  08:25:50  marrek]
 * 
 * Revision 1.2.6.2  1993/07/02  15:06:58  marrek
 * 	     Changes for port to HP800.
 * 	     [1993/07/02  14:42:39  marrek]
 * 
 * Revision 1.2.4.4  1992/12/31  17:49:00  bbelch
 * 	     Embedding copyright notice
 * 	     [1992/12/31  13:03:31  bbelch]
 * 
 * Revision 1.2.4.3  1992/12/18  08:27:15  marrek
 * 	     Various bug fixes for mask handling found at SNI
 * 	     [1992/12/17  16:26:52  marrek]
 * 
 * Revision 1.2.4.2  1992/11/27  13:07:37  marrek
 * 	     November 1992 code drop
 * 	     [1992/11/16  14:06:00  marrek]
 * 
 * Revision 1.2.2.5  1992/07/01  16:51:47  melman
 * 	     GDS drop of 6/26/92
 * 	     [1992/07/01  16:01:29  melman]
 * 
 * Revision 1.2.2.4  1992/06/01  21:51:40  zeliff
 * 	     New version of file from GDS drop
 * 	     [1992/05/28  19:14:32  zeliff]
 * 
 * Revision 1.2.2.3  1992/05/05  21:36:20  melman
 * 	     When defining UP and BC changed to do the static version if on AIX.
 * 	         Was:
 * 	         #ifndef SYS5ATT
 * 
 * 	         Changed to:
 * 	         #if !defined(SYS5ATT) && !defined(_AIX)
 * 	     [1992/05/05  21:36:00  melman]
 * 
 * Revision 1.2.2.2  1992/04/27  19:56:35  melman
 * 	     Files from Helmut's visit
 * 	     [1992/04/27  19:51:42  melman]
 * 
 * Revision 1.2  1992/01/19  22:13:05  devrcs
 * 	     Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cget.c,v $ $Revision: 1.2.12.2 $ $Date: 1996/02/18 19:40:22 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/************************************************************************/
/*      Functions for input/output on screen                            */
/************************************************************************/
/*      Compiler dependencies:                                          */
/*      -DSYS5ATT         ... System V environment in att - Universum   */
/*      without -DSYS5ATT ... else (Sinix 2.1, Sinix V sie-Universum)   */
/*      -DFLOAT         ... with float support                          */
/*      without -DFLOAT   ... without float support                     */
/************************************************************************/
#define  CGET_UEB                       /* Compiler unit cget           */


#define	issign(c)	((c == '+')||(c == '-'))

#include <gds.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include <cget.h>
#include <d2dir.h>	/* to handle defns. in d27util.h		*/
#include <d27util.h>	/* to handle D27_CLIENT_DIR			*/
#include <d20proto.h>	/* to handle D27_CLIENT_DIR			*/

#include <sys/termio.h>

#define LINE_LEN	127

#define ANY_VALUE	0	/* for non-used return values		*/

/* Function declarations */
				/* These functions should be in		*/
				/* curses.h but to include this file	*/
				/* creates new warnings/errors on some	*/
				/* platforms				*/
extern char *tgetstr(const char *, char **);
extern char *tgoto(char *, int, int);
extern int tgetent(char *, char *);
extern int tputs(register char *, int, int (*)(char));

/* Variable declarations */

static  signed16    ospeed = 0;

	signed32  getmode   = 0;             /* Output mode                  */
	char      getdel[]  = ":";           /* Limit input characters       */
	char      getfill   = ' ';           /* Character to fill input field*/
	signed32  getdl     = 0;             /* Delta- line                  */
	signed32  getdc     = 1;             /* Delta- column                */
	char      *(*getexit)(signed32, char *, char *, unsigned char) =getform;
					/* Preparation routine          */
	char comment[MAXCOMMENT];       /* Comment line for file-input         */
	signed32  rbatch     = 0;            /* 1= read of an input file     */

static	char            *pesc_se = "";
static	char            *pesc_ue = "";

static	char            *pesc_so = "";
static	char            *pesc_ae = "";
static	char            *pesc_us = "";

static	char            *pesc_cl = "";
static	char            *pesc_bl = "";
static	char            *pesc_cm = "";

static	char            *pesc_ks = "";
static	char            *pesc_ke = "";

	int		cget_linediscipline     = 0;

        Escap_seq       esc_seq [] = {
                        { "k1"  ,0      , FKTF1         },
                        { "k8"  ,0      , FKTF8         },
                        { "l0"  ,"\033>", FKTHELP       },
                        { ""    ,"\004" , FKTEND        },
                        { ""    ,"\015" , FKTCR         },
                        { "ya"  ,"\012" , FKTMENU       },
                        { ""    ,"\177" , FKTDEL        },
                        { ""    ,"\003" , FKTDEL        },
                        { "kR"  ,"\033S", FKTSCU        },
                        { "kF"  ,"\033T", FKTSCD        },
                        { "kr"  ,"\033C", FKTCURS_RIGHT },
                        { "kl"  ,"\033D", FKTCURS_LEFT  },
                        { "kd"  ,"\033B", FKTCURS_DOWN  },
                        { "ku"  ,"\033A", FKTCURS_UP    },
                        { "kb"  ,"\010" , FKTBS         },
                        { "kD"  ,"\033P", FKTDELCHAR    },
                        { "kL"  ,"\033M", FKTDELLINE    },
                        { "kI"  ,"\033Q", FKTINSCHAR    },

                        { "cl"  ,0      , 0 },  /* clear screen.        */
                        { "cm"  ,0      , 0 },  /* move cursor to r#1 and c#2 */
                        { "bl"  ,0      , 0 },  /* Audio bell.          */
                        { "se"  ,0      , 0 },  /* end standout mode.   */
                        { "ae"  ,0      , 0 },  /* end using alternate char.  */
                                                /* set.                       */
                        { "us"  ,0      , 0 },  /* begin underline mode */
                        { "ue"  ,0      , 0 },  /* end underline mode.  */
                        { "so"  ,0      , 0 },  /* begin standout mode. */
                        { "ks"  ,0      , 0 },  /* Turn keypad on, if can be */
                                                /* turned on and off.        */
                        { "ke"  ,0      , 0 }   /* Turn keypad off, if can be */
                                                /* turned on and off.         */
		};

static  Escap_seq       *pesc_beg       = esc_seq;
static  Escap_seq       *pesc_end       = esc_seq + (sizeof (esc_seq)/sizeof (Escap_seq));

#ifdef __hpux
#  define OPOST         0000001         /* Perform output processing */
#  define TCSANOW       0
     struct termios {
        tcflag_t        c_iflag;        /* Input modes */
        tcflag_t        c_oflag;        /* Output modes */
        tcflag_t        c_cflag;        /* Control modes */
        tcflag_t        c_lflag;        /* Local modes */
        tcflag_t        c_reserved;     /* Reserved for future use */
        cc_t            c_cc[16];       /* Control characters */
     };

#endif

static  struct {
	signed32     line;                   /* Line number                  */
	signed32     dl;                     /* Delta- line number           */
	signed32     coll;                   /* Column number                */
	signed32     dc;                     /* Delta- column number         */
	signed32     getmode;                /* Display mode                 */
	char    *pfeld;                 /* Pointer to output field      */
	char    *pmaske;                /* Output mask                  */
	char    *(*getexit)(signed32, char *, char *, unsigned char);
					/* Preparation routine          */
} getarr[TABMAX];

static  struct {
	signed32     line;                   /* Line number                  */
	signed32     dl;                     /* Delta- Line number           */
	signed32     coll;                   /* Column number                */
	signed32     dc;                     /* Delta- Column number         */
	signed32     getmode;                /* Display mode                 */
	char    *pfeld;                 /* Pointer on output field      */
} getwarr[TABMAX];

static  char 		getber[90];       /* Domain for exit-routines     */
static  char *getadr;                   /* Pointer in getber,...        */
static  signed32  getn       = 0;            /* Index getarr                 */
	signed32  getnl      = 0;            /* Index getarr                 */
static  signed32  getncursor = 0;            /* Index getarr cursor          */
static  signed32  getwn      = 0;            /* Index getwarr                */
        signed32  getwerg    = -1;           /* Index getwarr selection      */
static  signed32  getbl      = 0;            /* Index in raw / column        */
static  signed32  getftca[MAXFKT +1];        /* Allowed function keys        */
static  signed32  getrc      = 0;            /* Function key                 */
static  char getmstr[]  = "%s";         /* Standard for output mask     */
static  signed32  getruninit = 0;            /* cgetinit has been running    */
static  signed32  getrun     = 0;            /* cget / cread runs            */
static  signed32  getmodold  = -1;           /* Old output mode              */
static  signed32  ioctlwar   = 0;            /* Terminal settings changed    */

static  struct	{
	char 	 *cap;
	signed16  present;
	signed32  fkt;
	signed32  num_occur;
	char	 *esc_seq;
	} term_file[NO_USED_CAP] = { /* sorted table of supported caps.	*/
		{ "FKTBS", 		FALSE, 	FKTBS, 		0, 	NULL },

		{ "FKTCR", 		FALSE, 	FKTCR, 		0, 	NULL },
		{ "FKTCURS_DOWN", 	FALSE, 	FKTCURS_DOWN, 	0, 	NULL },
		{ "FKTCURS_LEFT", 	FALSE, 	FKTCURS_LEFT, 	0, 	NULL },
		{ "FKTCURS_RIGHT", 	FALSE, 	FKTCURS_RIGHT, 	0, 	NULL },
		{ "FKTCURS_UP", 	FALSE, 	FKTCURS_UP, 	0, 	NULL },
		{ "FKTDEL", 		FALSE, 	FKTDEL, 	0, 	NULL },
		{ "FKTDELCHAR", 	FALSE, 	FKTDELCHAR, 	0, 	NULL },
		{ "FKTDELLINE", 	FALSE, 	FKTDELLINE, 	0, 	NULL },
		{ "FKTEND", 		FALSE, 	FKTEND, 	0, 	NULL },
		{ "FKTF1", 	FALSE, 	FKTF1, 	0, 	NULL },
		{ "FKTF8", 	FALSE, 	FKTF8, 	0, 	NULL },
		{ "FKTHELP", 	FALSE, 	FKTHELP, 	0, 	NULL },
		{ "FKTINSCHAR",	FALSE, 	FKTINSCHAR, 	0, 	NULL },
		{ "FKTMENU", 	FALSE, 	FKTMENU, 	0, 	NULL },
		{ "FKTSCD", 	FALSE, 	FKTSCD, 	0, 	NULL },
		{ "FKTSCU", 	FALSE, 	FKTSCU, 	0, 	NULL },
	};	
	
#ifdef __OSF__
  static  struct termios   ttyold;        /* Old parameter                */
  static  int  lineold;                   /* Old line discipline          */
#else 
    static  struct termio   ttyold;      /* Old parameter                */
#endif /*  __OSF__  */

static signed16 cload_term_file(void);
static char  *cread_priv_esc(Escap_seq *);
static void  cload_esc_seq(void);

/************************************************************************/
/*      getoutc:     Function for output of escape sequences            */
/************************************************************************/
int getoutc(char c)
{
	putchar(c);
	return(0);
}

/************************************************************************/
/*      getentry: Read on terminal                                      */
/************************************************************************/
signed32  getentry (void)
{
	auto    Escap_seq       *pesc_run;
	auto    signed32         in;
	auto    char            inarray [10];
	auto    signed32        nrin;
	auto    signed32        teil;

	fflush  (stdout);
	for     (nrin = 0;nrin < sizeof (inarray);nrin++)
	{
		inarray[nrin] = 0;
	}
	nrin    = 0;

	for     (;;)
	{
		do
		{
			in              = getchar ();
		} while (in < 0);

		teil            = 0;
		inarray [nrin]  = in & 0177;
		nrin++;

		for     (pesc_run = pesc_beg;pesc_run < pesc_end;pesc_run++)
		{
			if      (pesc_run->fkt && pesc_run->esc)
			{
				if      (strcmp (pesc_run->esc,inarray) == 0)
				{
					return (FKTTAST | pesc_run->fkt);
				}
				if      (strncmp (pesc_run->esc,inarray,(size_t) nrin) == 0)
				{
					teil++;
				}
			}
		}
		if      ((teil == 0) && (nrin == 1))  return (in);
		if      ((teil == 0) || (nrin >= sizeof (inarray)))
		{
			cbell ();
			for     (nrin = 0;nrin < sizeof (inarray);nrin++)
			{
				inarray[nrin] = 0;
			}
			nrin    = 0;
		}
	}
}

/************************************************************************/
/*      getform : Format output                                         */
/************************************************************************/
char *getform (signed32 status, char *pfeld, char *pmaske, unsigned char in)
{
	static  char     *ppmask1;
	static  char     *ppunkt;
	static  char     *pgleich;
	static  signed32 wert;
	static  signed32 wert1;
	auto    signed32 c;
	auto    char     savechar;
	auto    char     *saveppmask1;
	auto    signed16 *pfelds;
	auto    signed32 *pfeldi;
	auto    signed32 *pfeldl;

#ifdef  FLOAT
	auto    float   *pfeldf;
	auto    double  *pfeldd;
#endif

	switch ( (int) status )
	{
	case GETFSTA:
		switch (pmaske[(strlen (pmaske) -1)])
		{
		case'a':switch (pmaske[(strlen (pmaske) -2)])
			{
			case'h':pfelds = (signed16 *) pfeld;
				wert   = (signed32)   *pfelds;
				break;
			case'l':pfeldl = (signed32 *)  pfeld;
				wert   = (signed32)   *pfeldl;
				break;
			default:pfeldi = (signed32 *)   pfeld;
				wert   = (signed32)   *pfeldi;
			}
			for (ppmask1 = strchr (pmaske,'.');;)
			{
				if (!(pgleich = strchr (ppmask1 +1,'='))) break;
				if (!(ppunkt  = strchr (pgleich +1,'.'))) break;
				sscanf (ppmask1 +1,"%ld",&wert1);
				if (wert == wert1)
				{
					if (ppunkt > pgleich)
					{
						sprintf (getber,"%.*s",
							 ppunkt - pgleich -1,
							 pgleich +1);
						break;
					}
				}
				ppmask1 = ppunkt;
			}
			if      ((!(pgleich)) ||
				(!(ppunkt)))    sprintf (getber,"%c",getfill);
			break;
#ifdef  FLOAT
		case'e':
		case'f':
		case'g':
			switch (pmaske[(strlen(pmaske) -2)])
			{
			case'l':pfeldd  = (double *) pfeld;
				sprintf(getber,pmaske,(double) 0);
				wert    = strlen (getber);
				sprintf(getber,pmaske,*pfeldd);
				getber[wert] = '\0';
				return(getber);
			default:pfeldf  = (float *) pfeld;
				sprintf(getber,pmaske,(float) 0);
				wert    = strlen (getber);
				sprintf(getber,pmaske,*pfeldf);
				getber[wert] = '\0';
				return(getber);
			}
			break;
#endif
		case's':if      (pfeld != getber)
			{
				sprintf(getber,pmaske,pfeld);
			}
			break;
		case'd':switch (pmaske[(strlen(pmaske) -2)])
			{
			case'h':pfelds = (signed16 *) pfeld;
				sprintf(getber,pmaske,*pfelds);
				break;
			case'l':pfeldl = (signed32 *)  pfeld;
				sprintf(getber,pmaske,*pfeldl);
				break;
			default:pfeldi = (signed32 *)   pfeld;
				sprintf(getber,pmaske,*pfeldi);
				break;
			}
			for	(c = 0; getber[c]; c++)
			{
				if	(issign(getber[c]))
				{
					in	  = getber[c];
					getber[c] = getber[0];
					getber[0] = in;
					break;
				}
			}
			break;
		case'u':
		case'x':switch (pmaske[(strlen(pmaske) -2)])
			{
			case'h':pfelds = (signed16 *) pfeld;
				sprintf(getber,pmaske,*pfelds);
				break;
			case'l':pfeldl = (signed32 *)  pfeld;
				sprintf(getber,pmaske,*pfeldl);
				break;
			default:pfeldi = (signed32 *)   pfeld;
				sprintf(getber,pmaske,*pfeldi);
				break;
			}
			break;
		default:geterror ("Invalid csay/cget- mask");
		}
		if      ((getfill != ' ') &&
			 (getrun))
		{
			if      (*(pmaske +1) == '-')
			{
				for (c = strlen (getber) -1; c >= 0; c--)
				{
					if      ((getber[c] != getfill) &&
						(getber[c] != ' ')) break;
					getber[c] = getfill;
				}
			}
			else
			{
				for (c = 0; ;c++)
				{
					if      ((getber[c] != getfill) &&
						(getber[c] != ' ')) break;
					getber[c] = getfill;
				}
			}
		}
		return (getber);

	case GETFCHA:
		savechar = pfeld[getbl];
		switch ( pmaske[(strlen(pmaske) -1)] )
		{
		case'a':if ((in == ' ')&&(getbl == 0))
			{
				if (!(ppmask1 = strchr (ppmask1 +1,'.')))
				{
					if (!(ppmask1 = strchr (pmaske,'.'))) break;
				}
				if (!(pgleich = strchr (ppmask1 +1,'=')))
				{
					if (!(ppmask1 = strchr (pmaske,'.'))) break;
				}
				if (!(pgleich = strchr (ppmask1 +1,'='))) break;
				if (!(ppunkt  = strchr (pgleich +1,'.'))) break;
				sscanf (ppmask1 +1,"%ld",&wert1);
				if (ppunkt > pgleich)
				{
					sprintf (getber,"%.*s",
						ppunkt - pgleich -1,
						pgleich +1);
					csay (getarr[getnl].line,
					      getarr[getnl].coll,
					      getber,
					      (*(pmaske +1) == '-') ? "%-s" : "%s");
					return (getber);
				}
			}
			else
			{
				saveppmask1    = ppmask1;
				pfeld[getbl++] = in;
				for (ppmask1 = strchr (pmaske,'.');;)
				{
					if (!(pgleich = strchr (ppmask1 +1,'='))) break;
					if (!(ppunkt  = strchr (pgleich +1,'.'))) break;
					sscanf (ppmask1 +1,"%ld",&wert);
					if (ppunkt > pgleich)
					{
						if (strncmp (pfeld,pgleich +1,getbl) == 0)
						{
							wert1 = wert;
							sprintf (getber,"%.*s",
								ppunkt - pgleich -1,
								pgleich +1);
							csay (getarr[getnl].line,
							      getarr[getnl].coll,
							      getber,
							      (*(pmaske +1) == '-') ? "%-s" : "%s");
							return (getber);
						}
					}
					ppmask1 = ppunkt;
				}
				getbl--;
				pfeld[getbl] = savechar;
				ppmask1      = saveppmask1;
			}
			break;
#ifdef  FLOAT
		case'f':
			if (isdigit(in) || isspace(in) || issign(in) ||
			   (in == '.'))
			{
				pfeld[getbl++] = in;
				if      (!(getmode & DUNKEL)) putchar (in);
				return  (getber);
			}
			break;
		case'e':
		case'g':
			if (isdigit(in) || isspace(in) ||
			   (in == '-')  || (in == '+') ||
			   (in == '.')  || (in == 'e') || (in == 'E'))
			{
				pfeld[getbl++] = in;
				if      (!(getmode & DUNKEL)) putchar (in);
				return  (getber);
			}
			break;
#endif
		case's':if (isprint(in) || isspace(in))
			{
				pfeld[getbl++] = in;
				if      (!(getmode & DUNKEL)) putchar (in);
				return  (getber);
			}
			break;
		case'u':if (isdigit(in) || isspace(in))
			{
				pfeld[getbl++] = in;
				if      (!(getmode & DUNKEL)) putchar (in);
				return  (getber);
			}
			break;
		case'd':if (isdigit(in) || isspace(in))
			{
				pfeld[getbl++] = in;
				if      (!(getmode & DUNKEL)) putchar (in);
				return  (getber);
			}
			else if (issign(in))
			{
				if	(issign(pfeld[0]))
				{
					pfeld[0] = in;
					if	(!(getbl))	getbl++;
					csay (getarr[getnl].line,
					      getarr[getnl].coll,
					      pfeld, "%s");
					return(getber);
				}
				for	(c = 0; pfeld[c +1]; c++)
				{
					if	(pfeld[c] == ' ')	break;
					if	(pfeld[c] == getfill)	break;
				}
				for	(c = ((pfeld[c]) ? c : c - 1);
					 c > 1;
					 c--)
				{
					pfeld[c] = pfeld[c -1];
				}
				pfeld[0] = in;
				for	(c = 1; pfeld[c]; c++)
				{
					if	(pfeld[c] == getfill)
					{
						pfeld[c] = ' ';
					}
					else	break;
				}
				getbl++;
				csay (getarr[getnl].line,
				      getarr[getnl].coll,
				      pfeld, "%s");
				return(getber);
			}
			break;
		case'x':if (isxdigit(in) || isspace(in))
			{
				pfeld[getbl++] = in;
				if      (!(getmode & DUNKEL)) putchar (in);
				return  (getber);
			}
			break;
		default:geterror ("Invalid csay/cget- mask");
		}
		cbell  ();
		break;

	case GETFEND:
		if      ((getrun)&&(getfill != ' '))
		{
			if      (*(pmaske +1) == '-')
			{
				for     (c = strlen (getber) -1; c >= 0; c--)
				{
					if      ((getber[c] != getfill) &&
						(getber[c] != ' ')) break;
					getber[c] = ' ';
				}
			}
			else
			{
				for (c = 0; ; c++)
				{
					if      ((getber[c] != getfill) &&
						(getber[c] != ' ')) break;
					getber[c] = ' ';
				}
			}
		}
		switch ( pmaske[(strlen(pmaske) -1)] )
		{
		case'a':switch (pmaske[(strlen (pmaske) -2)])
			{
			case'h':pfelds  = (signed16 *) pfeld;
				*pfelds = (signed16)   wert1;
				return (getber);
			case'l':pfeldl  = (signed32 *) pfeld;
				*pfeldl = (signed32)   wert1;
				return (getber);
			default:pfeldi  = (signed32 *) pfeld;
				*pfeldi = (signed32)   wert1;
				return (getber);
			}
#ifdef  FLOAT
		case'e':
			switch ( pmaske[(strlen(pmaske) -2)])
			{
			case'l':pfeldd  = (double *) pfeld;
				*pfeldd = 0;
				sscanf(getber,"%le",pfeldd);
				return(getber);
			default:pfeldf  = (float *) pfeld;
				*pfeldf = 0;
				sscanf(getber,"%e",pfeldf);
				return(getber);
			}
			break;
		case'f':
			switch ( pmaske[(strlen(pmaske) -2)])
			{
			case'l':pfeldd  = (double *) pfeld;
				*pfeldd = 0;
				sscanf(getber,"%lf",pfeldd);
				return(getber);
			default:pfeldf  = (float *) pfeld;
				*pfeldf = 0;
				sscanf(getber,"%f",pfeldf);
				return(getber);
			}
			break;
		case'g':
			switch ( pmaske[(strlen(pmaske) -2)])
			{
			case'l':pfeldd  = (double *) pfeld;
				*pfeldd = 0;
				sscanf(getber,"%lg",pfeldd);
				return(getber);
			default:pfeldf  = (float *) pfeld;
				*pfeldf = 0;
				sscanf(getber,"%g",pfeldf);
				return(getber);
			}
			break;
#endif
		case's':wert1 = strlen(getber);
			wert  = wert1 - 1;
			while ((getber[wert] == ' ')&&(wert)) wert--;
			if     (getber[wert] != ' ') wert++;
			getber[wert] = '\0';
			strncpy(pfeld,getber,wert1);
			return (getber);
		case'd':for	(c = 0; getber[c]; c++)
			{
				if	(issign(getber[c]))
				{
				}
				else if	((getber[c] == ' ')		||
					 (getber[c] == getfill))
				{
					getber[c] = '0';
				}
				else	break;
			}
			switch ( pmaske[(strlen(pmaske) -2)])
			{
			case'h':pfelds  = (signed16 *) pfeld;
				*pfelds = 0;
				sscanf(getber,"%hd",pfelds);
				return(getber);
			case'l':pfeldl  = (signed32 *) pfeld;
				*pfeldl = 0;
				sscanf(getber,"%ld",pfeldl);
				return(getber);
			default:pfeldi  = (signed32 *) pfeld;
				*pfeldi = 0;
				sscanf(getber,"%d",pfeldi);
				return(getber);
			}
		case'u':switch ( pmaske[(strlen(pmaske) -2)])
			{
			case'h':pfelds  = (signed16 *) pfeld;
				*pfelds = 0;
				sscanf(getber,"%hd",pfelds);
				return(getber);
			case'l':pfeldl  = (signed32 *) pfeld;
				*pfeldl = 0;
				sscanf(getber,"%ld",pfeldl);
				return(getber);
			default:pfeldi  = (signed32 *) pfeld;
				*pfeldi = 0;
				sscanf(getber,"%d",pfeldi);
				return(getber);
			}
		case'x':switch ( pmaske[(strlen(pmaske) -2)])
			{
			case'h':pfelds  = (signed16 *) pfeld;
				*pfelds = 0;
				sscanf(getber,"%hx",pfelds);
				return(getber);
			case'l':pfeldl  = (signed32 *) pfeld;
				*pfeldl = 0;
				sscanf(getber,"%lx",pfeldl);
				return(getber);
			default:pfeldi  = (signed32 *) pfeld;
				*pfeldi = 0;
				sscanf(getber,"%x",pfeldi);
				return(getber);
			}
		default:geterror ("Invalid csay/cget- mask");
		}
		break;
	}
	return ((char *) 0);
}

/************************************************************************/
/*      ungetsmod: 							*/
/************************************************************************/
signed32 ungetsmod (void)
{

	if (ospeed > B9600) {
		fputs (pesc_se,stdout);
		fputs (pesc_ue,stdout);
	}
	else {
		tputs (pesc_se,1,getoutc);
		tputs (pesc_ue,1,getoutc);
	}

	fflush   (stdout);
}

/************************************************************************/
/*      getsmod: Set output mode                                        */
/************************************************************************/
signed32 getsmod (void)
{
	auto    signed32     c;

	c       = (getmode & (HALBHELL | UNDERLINE | INVERS));

 	if      ((getmodold == -1) || (getmodold != c) ||
 		 (getmodold == INVERS)) {

		if      (ospeed > B9600) {
			fputs (pesc_se,stdout);
			fputs (pesc_ue,stdout);
		} else {
			tputs (pesc_se,1,getoutc);
			tputs (pesc_ue,1,getoutc);
		}

		if      (c & INVERS)
		{
			if      (ospeed > B9600) fputs (pesc_so,stdout);
			else                     tputs (pesc_so,1,getoutc);
		}
		else if (c & HALBHELL)
		{
			if      (ospeed > B9600) fputs (pesc_ae,stdout);
			else                     tputs (pesc_ae,1,getoutc);
		}
		else if (c & UNDERLINE)
		{
			if      (ospeed > B9600) fputs (pesc_us,stdout);
			else                     tputs (pesc_us,1,getoutc);
		}
		getmodold       = c;
	}
}

/************************************************************************/
/*      getadj: Adjust input field                                      */
/************************************************************************/
signed32 getadj (void)
{
	if (getnl < getn)
	{
		getdl   = getarr[getnl].dl;
		getdc   = getarr[getnl].dc;
		getmode = getarr[getnl].getmode;
		getexit = getarr[getnl].getexit;
		getsmod ();
		if      (getmode & WAHLERG)
		{
			getwahl (0,0,getarr[getnl].pfeld);
		}
		getadr  = (*getexit)(GETFSTA,getarr[getnl].pfeld,
				    getarr[getnl].pmaske,' ');
	}
	else
	{
		getdl   = 0;
		getdc   = 1;
		getmode = NORMAL;
		getexit = getform;
		sprintf ( getber, " " );
		getadr  = getber;
	}
}

/************************************************************************/
/*      getwahl: Check choice input                                     */
/************************************************************************/
signed32 getwahl (signed32 dirlin, signed32 dircol, char *new)
{
	auto    char      savber[90];
	auto    char     *savadr;
	auto    signed32  savlen          = 0;
	register signed32 x               = 0;
	register char    *y               = (char *) 0;
	register char    *z               = (char *) 0;
	auto    signed32  getwold         = 0;
	auto    signed32  getw1           = 0;
	auto    signed32  getw2           = 0;
	auto    signed32  getw3           = 0;
	auto    signed32  getw4           = 0;
	auto    signed32  getwhor         = 0;
	auto    signed32  getwvert        = 0;
	auto    signed32  getwlin         = 0;
	auto    signed32  getwcol         = 0;

	strncpy (savber,getber,sizeof (savber));
	getrun  = 1;
	if      (new)
	{
		savadr  = (*getexit) (GETFSTA,new,
				      getarr[getnl].pmaske,' ');
	}
	else
	{
		savadr  = (*getexit) (GETFSTA,getarr[getnl].pfeld,
				      getarr[getnl].pmaske,' ');
	}
	savlen  = strlen (savadr);

	getwold = getwerg;
	getw1   = -1;
	getw2   = -1;
	getw3   = -1;
	getw4   = -1;
	getwhor = 0;
	getwvert = 0;

	if	((new)&&(savlen)&&(getwerg >= 0) &&(getwerg < getwn))
	{
		x       = 0;
		y       = savadr;
		z       = getwarr[getwerg].pfeld;

		for     (;(*y) == (*z);x++,y++,z++);
		for     (;(*y == getfill)||(*y == ' ');y++);
		for     (;(*z == getfill)||(*z == ' ');z++);
		if      (( (!(*y)) && (!(*z)) ) || (x >= savlen))
		{
			getw1 = getwerg;
			getwold = -1;
			goto alreadyFound;
		}
	}

	for     (getwerg = 0;getwerg < getwn;getwerg++)
	{
		getwlin = getwarr[getwerg].line;
		getwcol = getwarr[getwerg].coll;

		if      ((new)&&(savlen))
		{
			x       = 0;
			y       = savadr;
			z       = getwarr[getwerg].pfeld;

			for     (;(*y) == (*z);x++,y++,z++);
			for     (;(*y == getfill)||(*y == ' ');y++);
			for     (;(*z == getfill)||(*z == ' ');z++);
			if      (( (!(*y)) && (!(*z)) ) || (x >= savlen))
			{
				if       (getw1 < 0)                                     getw1 = getwerg;
				else if  (getwlin <  getwarr[getw1].line)  getw1 = getwerg;
				else if ((getwlin == getwarr[getw1].line) &&
					 (getwcol <  getwarr[getw1].coll)) getw1 = getwerg;
			}
		}
		else if ((dirlin > 0) && (getwold >= 0))
		{
			if      ((getwcol == getwarr[getwold].coll) &&
				 (getwlin >  getwarr[getwold].line))
			{
				if      (getw1 < 0)     getw1 = getwerg;
				else if (getwlin < getwarr[getw1].line)
				{
					getw1 = getwerg;
				}
			}
			if      (getwlin > getwarr[getwold].line)
			{
				if      (getw2 < 0)     getw2 = getwerg;
				else if ((getwlin <   getwarr[getw2].line) ||
					 ((getwlin == getwarr[getw2].line) &&
					  (getwcol <  getwarr[getw2].coll)))
				{
					getw2 = getwerg;
				}
			}
			if      ((getwcol == getwarr[getwold].coll) &&
				 (getwlin >  getwarr[getwold].line))
			{
				if      (getw3 < 0)     getw3 = getwerg;
				else if (getwlin < getwarr[getw3].line)
				{
					getw3 = getwerg;
				}
			}
		}
		else if ((dirlin < 0) && (getwold >= 0))
		{
			if      ((getwcol == getwarr[getwold].coll) &&
				 (getwlin <  getwarr[getwold].line))
			{
				if      (getw1 < 0)     getw1 = getwerg;
				else if (getwlin > getwarr[getw1].line)
				{
					getw1 = getwerg;
				}
			}
			if      (getwlin < getwarr[getwold].line)
			{
				if      (getw2 < 0)     getw2 = getwerg;
				else if ((getwlin > getwarr[getw2].line) ||
					((getwlin == getwarr[getw2].line) &&
					 (getwcol <  getwarr[getw2].coll)))
				{
					getw2 = getwerg;
				}
			}
			if      ((getwcol == getwarr[getwold].coll) &&
				 (getwlin <  getwarr[getwold].line))
			{
				if      (getw3 < 0)     getw3 = getwerg;
				else if (getwlin > getwarr[getw3].line)
				{
					getw3 = getwerg;
				}
			}
		}
		else if ((dircol > 0) && (getwold >= 0))
		{
			if      ((getwlin == getwarr[getwold].line) &&
				 (getwcol >  getwarr[getwold].coll))
			{
				if      (getw1 < 0)     getw1 = getwerg;
				else if (getwcol < getwarr[getwold].coll)
				{
					getw1 = getwerg;
				}
			}
			if      (getwlin > getwarr[getwold].line)
			{
				if      (getw2 < 0)     getw2 = getwerg;
				else if ((getwlin < getwarr[getw2].line) ||
					 ((getwlin == getwarr[getw2].line) &&
					  (getwcol <  getwarr[getw2].coll)))
				{
					getw2 = getwerg;
				}
			}
		}
		else if ((dircol < 0) && (getwold >= 0))
		{
			if      ((getwlin == getwarr[getwold].line) &&
				 (getwcol <  getwarr[getwold].coll))
			{
				if      (getw1 < 0)     getw1 = getwerg;
				else if (getwcol > getwarr[getw1].coll)
				{
					getw1 = getwerg;
				}
			}
			if      (getwlin < getwarr[getwold].line)
			{
				if      (getw2 < 0)     getw2 = getwerg;
				else if ((getwlin > getwarr[getw2].line) ||
					 ((getwlin == getwarr[getw2].line) &&
					  (getwcol >  getwarr[getw2].coll)))
				{
					getw2 = getwerg;
				}
			}
		}

		if      ((dirlin > 0) || (dircol > 0))
		{
			if      (getw4 < 0)     getw4 = getwerg;
			else if ((getwlin < getwarr[getw4].line) ||
				 ((getwlin == getwarr[getw4].line) &&
				  (getwcol <  getwarr[getw4].coll)))
			{
					getw4 = getwerg;
			}
		}
		else if ((dirlin < 0) || (dircol < 0))
		{
			if      (getw4 < 0)     getw4 = getwerg;
			else if ((getwlin > getwarr[getw4].line) ||
				 ((getwlin == getwarr[getw4].line) &&
				  (getwcol >  getwarr[getw4].coll)))
			{
				getw4 = getwerg;
			}
		}

		if      (getwlin != getwarr[0].line) getwvert = 1;
		if      (getwcol != getwarr[0].coll) getwhor  = 1;
	}

alreadyFound:

	strncpy (getber,savber,sizeof (getber));
	getwerg = getwold;
	if      ((getwn > 1)&&(getwvert == 0)&&(dirlin != 0))   return (0);
	if      ((getwn > 1)&&(getwhor  == 0)&&(dircol != 0))   return (0);

	if      (getw1 >= 0)    getwerg         = getw1;
	else if (getw2 >= 0)    getwerg         = getw2;
	else if (getw3 >= 0)    getwerg         = getw3;
	else if (getw4 >= 0)    getwerg         = getw4;
	else                    getwerg         = -1;

	if      (getwerg != getwold)
	{
		if      (getwold >= 0)
		{
			getrun  = 0;
			getdl   = getwarr[getwold].dl;
			getdc   = getwarr[getwold].dc;
			getmode = getwarr[getwold].getmode;
			csay (getwarr[getwold].line,
			      getwarr[getwold].coll,
			      getwarr[getwold].pfeld,
			      "%s");
		}

		if      (getwerg >= 0)
		{
			getrun  = 0;
			getdl   = getwarr[getwerg].dl;
			getdc   = getwarr[getwerg].dc;
			getmode = INVERS;
			csay (getwarr[getwerg].line,
			      getwarr[getwerg].coll,
			      getwarr[getwerg].pfeld,
			      "%s");
			getrun  = 1;
			for     (x = 0;x < savlen;x++)
			{
				if      (*(getadr + x) == '\0')
				{
					for     (;x < savlen;x++) *(getadr + x) = getfill;
				}
			}
			*(getadr + x) = '\0';
			(*getexit) (GETFEND,getarr[getnl].pfeld,
				    getarr[getnl].pmaske,' ');
		}
		getrun  = 1;
		getdl   = getarr[getnl].dl;
		getdc   = getarr[getnl].dc;
		getmode = getarr[getnl].getmode;
		csay    (getarr[getnl].line,
			 getarr[getnl].coll,
			 getarr[getnl].pfeld,
			 getarr[getnl].pmaske);
	}
	getrun  = 1;

	if      (getwerg != getwold)    return (1);
	else                            return (0);
}

/************************************************************************/
/*      getfkt: Received functions keys                                 */
/************************************************************************/
signed32  getfkt (signed32 fkt)
{
	auto    signed32     c;

	switch ((int) fkt)
	{
	case    FKTCURS_RIGHT:
		if      ((getnl < getn)&&(getmode & WAHLERG)&&(getwn))
		{
			if      (getwahl (0,1,NULL))    return(ANY_VALUE);
		}
		c = getarr[getnl].dc;
		(c != 0) ? (c = abs(c)/c):(c = 0);
		(c != 0) ? (getbl += c)  :(getbl = strlen (getadr));
		return(ANY_VALUE);

	case    FKTCURS_LEFT:
		if      ((getnl < getn)&&(getmode & WAHLERG)&&(getwn))
		{
			if      (getwahl (0,-1,NULL))   return(ANY_VALUE);
		}
		c = getarr[getnl].dc;
		(c != 0) ? (c = abs(c)/c):(c = 0);
		(c != 0) ? (getbl -= c)  :(getbl = -1);
		return(ANY_VALUE);

	case    FKTCURS_DOWN:
		if      ((getnl < getn)&&(getmode & WAHLERG)&&(getwn))
		{
			if      (getwahl (1,0,NULL))    return(ANY_VALUE);
		}
		c = getarr[getnl].dl;
		(c != 0) ? (c = abs(c)/c):(c = 0);
		(c != 0) ? (getbl += c)  :(getbl = strlen (getadr));
		return(ANY_VALUE);

	case    FKTCURS_UP:
		if      ((getnl < getn)&&(getmode & WAHLERG)&&(getwn))
		{
			if      (getwahl (-1,0,NULL))   return(ANY_VALUE);
		}
		c = getarr[getnl].dl;
		(c != 0) ? (c = abs(c)/c):(c = 0);
		(c != 0) ? (getbl -= c)  :(getbl = -2);
		return(ANY_VALUE);

	case    FKTCURS_HOME:
		if (getnl < getn)
		{
			(*getexit) (GETFEND,getarr[getnl].pfeld,
				    getarr[getnl].pmaske,' ');
			if      (getmode & WAHLERG)
			{
				getwahl (0,0,getarr[getnl].pfeld);
			}
			csay(getarr[getnl].line, getarr[getnl].coll,
			     getarr[getnl].pfeld, getarr[getnl].pmaske);
		}
		getnl = 0;
		getbl = 0;
		getadj ();
		return(ANY_VALUE);

	case    FKTTAB_LEFT:
		(getbl > 0) ? (getbl = 0):(getbl = -2);
		return(ANY_VALUE);

	case    FKTTAB_RIGHT:
		getbl = strlen (getadr);
		return(ANY_VALUE);

	case    FKTBS:
		if      (getnl >= getn    ) break;
		if      (getmode & ALTER  ) break;
		getbl--;
		if      (getbl >= 0)
		{
			for     (c = getbl; c < (signed32)(strlen (getadr)-1);)
			{
			      *(getadr +c) = *(getadr +c +1);
			      c++;
			}
			*(getadr +c)    = getfill;
		}
		csay (getarr[getnl].line,
		      getarr[getnl].coll,
		      getadr,
		      (*(getarr[getnl].pmaske +1) == '-') ? "%-s" : "%s");
		return(ANY_VALUE);

	case    FKTDELCHAR:
		if      (getnl >= getn    ) break;
		if      (getmode & ALTER  ) break;
		for     (c = getbl; c < (signed32)(strlen (getadr)-1);)
		{
		      *(getadr +c) = *(getadr +c +1);
		      c++;
		}
		*(getadr +c)    = getfill;
		csay (getarr[getnl].line,
		      getarr[getnl].coll,
		      getadr,
		      (*(getarr[getnl].pmaske +1) == '-') ? "%-s" : "%s");
		return(ANY_VALUE);

	case    FKTDELLINE:
		if      (getnl >= getn    ) break;
		if      (getmode & ALTER  ) return(ANY_VALUE);
		for     (c = 0; c < (signed32)(strlen (getadr));)
		{
		      *(getadr +c) = getfill;
		      c++;
		}
		csay (getarr[getnl].line,
		      getarr[getnl].coll,
		      getadr,
		      (*(getarr[getnl].pmaske +1) == '-') ? "%-s" : "%s");
		getbl = 0;
		return(ANY_VALUE);

	case    FKTINSCHAR:
		if      (getnl >= getn    ) break;
		if      (getmode & ALTER  ) break;
		for     (c = (strlen (getadr) -1);c > getbl;)
		{
			*(getadr +c) = *(getadr +c -1);
			c--;
		}
		*(getadr +c) = ' ';
		csay (getarr[getnl].line,
		      getarr[getnl].coll,
		      getadr,
		      (*(getarr[getnl].pmaske +1) == '-') ? "%-s" : "%s");
		return(ANY_VALUE);

	default:
		if      (fkt < MAXFKT)
		{
			if      ((fkt == FKTCR) &&
				 (getftca[FKTCRIM]))
			{
				getbl   = strlen (getadr);
				getrc	= FKTCRIM;
				return(ANY_VALUE);
			}

			if      ((fkt == FKTCR)       &&
				 (getnl < (getn -1)))
			{
				getbl   = strlen (getadr);
				return(ANY_VALUE);
			}

			if      (getftca[fkt])
			{
				getbl   = strlen (getadr);
				getrc   = fkt;
				return(ANY_VALUE);
			}

			if	(fkt == FKTCR)
			{
				getbl	= strlen (getadr);
				return(ANY_VALUE);
			}
		}
	}

	cbell ();
	return(ANY_VALUE);
}

/************************************************************************/
/*      gettdel: Receive DEL-key                                        */
/************************************************************************/
void gettdel (int signo)
{
	signal (signo, gettdel);
}

/************************************************************************/
/*      geterror: Write and close an error message                      */
/************************************************************************/
signed32 geterror (char *msg)
{
	cgetterm ();

        fputs    (pesc_cl, stdout);

	fflush   (stdout);
	fprintf  (stderr,"cget-error: %s\n",msg);
	exit     (1);
}

/************************************************************************/
/*      cgetinit: Initialise csay cget cread                            */
/************************************************************************/
signed32 cgetinit (void)
{
	auto    signed32             c;


#       if defined(__OSF__)
	  auto    struct termios  ttysave;
#       else
	  auto    struct termio   ttysave;
#       endif /* __OSF__ */

	if      ((isatty (0) == 0) || (isatty (1) == 0)) rbatch = 1;
	else
	{

#               if defined(__OSF__)
		  ioctl (1,TIOCGETD,&lineold);
		  ioctl (1,TIOCGETA,&ttysave);
#               else
		  ioctl (1,TCGETA,&ttysave);
#               endif /* __OSF__ */

		rbatch          = 0;
		ioctlwar        = 1;
		ttyold          = ttysave;
		ospeed          = (ttysave.c_cflag & CBAUD);
		ttysave.c_iflag = (ttysave.c_iflag & ~(INLCR  | ICRNL | IGNCR | IUCLC));
		ttysave.c_lflag = (ttysave.c_lflag & ~(ICANON | ISIG  | ECHO));

#               if !defined(__OSF__)
		  ttysave.c_line      = cget_linediscipline;
#               endif /* __OSF__ */

		ttysave.c_cc[VMIN]  = 1;
		ttysave.c_cc[VTIME] = 0;

#               if defined(__OSF__)
		  ioctl (1,TIOCSETD,&cget_linediscipline);
		  ioctl (1,TIOCSETAF,&ttysave);
#               else
		  ioctl (1,TCSETAF,&ttysave);
#               endif /* __OSF__ */

	}

	cload_esc_seq();

	for  (c = 0; c <= MAXFKT; c++)  getftca [c] = 0;
	getftca[0]      = 1;
	gettdel (SIGINT);
	getmodold       = -1;
	getruninit      = 1;

	/* set terminal to use key pad */
	fputs    (pesc_ks, stdout);
}

/************************************************************************/
/*      cgetterm: Exit csay cget cread                                  */
/************************************************************************/
signed32 cgetterm (void)
{

#       if defined(__OSF__)
	  if (ioctlwar)      ioctl (1,TIOCSETD,&lineold);
	  if (ioctlwar)      ioctl (1,TIOCSETAF,&ttyold);
#       else
	  if (ioctlwar)      ioctl (1,TCSETAF,&ttyold);
#       endif /* __OSF__ */

	getmode = NORMAL;
	getsmod ();

	fflush  (stdout);
	getruninit      = 0;

	/* reset terminal to turn key-pad off */
	fputs    (pesc_ke, stdout);
}

/************************************************************************/
/*      cerase: Clear screen                                            */
/************************************************************************/
signed32 cerase (void)
{
	if     (!(getruninit)) geterror ("cgetinit has not been called");
	fputs  (pesc_cl,stdout);
}

/************************************************************************/
/*      cbell: Acoustic alarm                                           */
/************************************************************************/
signed32 cbell (void)
{
	if     (!(getruninit)) geterror ("cgetinit has not been called");
	fputs  (pesc_bl,stdout);
	fflush (stdout);
}

/************************************************************************/
/*      csay: Output to display                                         */
/************************************************************************/
signed32  csay ( signed32  line,        /* Line number                  */
	         signed32  coll,        /* Column number                */
	         char *pfeld,           /* Address of output field      */
	         char *pmaske )         /* Address of output mask       */
{
	auto    char    *adr;

	if      (!(getruninit))  geterror ("cgetinit has not been called");
	if      (pmaske == NULL) pmaske = getmstr;

	getadr  = (*getexit) (GETFSTA,pfeld,pmaske,' ');
	if      (getmode & WAHL)
	{
		if  (getwn >= TABMAX) geterror ("Max. number of selections exceeded");
		getwarr[getwn].line     = line;
		getwarr[getwn].dl       = getdl;
		getwarr[getwn].coll     = coll;
		getwarr[getwn].dc       = getdc;
		getwarr[getwn].getmode  = getmode & ~WAHL;
		getwarr[getwn].pfeld    = (char *)malloc (strlen (getadr) +1);
		strcpy (getwarr[getwn].pfeld,getadr);
		getwn++;
	}

	if (ospeed > B9600) fputs (tgoto (pesc_cm, coll-1, line-1) ,stdout);
	else                tputs (tgoto (pesc_cm, coll-1, line-1),1,getoutc);

	getsmod ();

	if      ((!(getmode & DUNKEL)) && (getdl == 0) && (getdc == 1))
	{
		fputs (getadr,stdout);
	}
	else
	{
		adr = getadr;
		while   (*adr != '\0')
		{
			if      (!(getmode & DUNKEL)) putchar (*adr++);
			else                          adr++;
			line += getdl;
			coll += getdc;
			if      (ospeed > B9600) fputs (tgoto (pesc_cm,coll-1,line-1),stdout);
			else                     tputs (tgoto (pesc_cm,coll-1,line-1),1,getoutc);
		}
	}

	ungetsmod();
}

/************************************************************************/
/*      cget: Output with wait for input                                */
/************************************************************************/
signed32  cget ( signed32  line,        /* Line number                  */
	         signed32  coll,        /* Column number                */
	         char *pfeld,           /* Adress of output field       */
	         char *pmaske )         /* Adress of output mask        */
{
	auto    signed32     c, l;

	if      (!(getruninit))  geterror ("cgetinit has not been called");
	if      (getn >= TABMAX) geterror ("Max. number of input requests exceeded");
	if      (pmaske == NULL) pmaske = getmstr;
	if      (l = strlen (getdel))
	{
		csay ( line, coll, getdel, NULL );
		for ( c = 0; c < l; c++ ) line += getdl;
		for ( c = 0; c < l; c++ ) coll += getdc;
	}

	getarr[getn].line     = line;
	getarr[getn].dl       = getdl;
	getarr[getn].coll     = coll;
	getarr[getn].dc       = getdc;
	if (*(pmaske + strlen (pmaske) -1) == 'a')
	{
		getarr[getn].getmode = getmode |  ALTER;
	}
	else    getarr[getn].getmode = getmode & ~ALTER;
	getarr[getn].pfeld    = pfeld;
	getarr[getn].pmaske   = (char *)malloc (strlen (pmaske) +1);
	strcpy( getarr[getn].pmaske, pmaske );
	getarr[getn].getexit  = getexit;
	if      (getmode & CURSOR)
	{
		getncursor = getn;
	}
 
	getrun          = 1;
	csay  ( line, coll, pfeld , pmaske );
	getrun          = 0;

	if      (strlen (getdel))
	{
		l   = strlen ((*getexit) (GETFSTA,pfeld,pmaske,' '));
		for ( c = 0; c < l; c++) line += getdl;
		for ( c = 0; c < l; c++) coll += getdc;
		csay ( line, coll, getdel, NULL );
	}
	getn++;
	getdl                 = 0;
	getdc                 = 1;
}

/************************************************************************/
/*      cgetfkt: Ask for input of a function key                         */
/************************************************************************/
signed32 cgetfkt ( signed32 fkt )
{
	if      (!(getruninit)) geterror ("cgetinit has not been called");
	if      (fkt >= MAXFKT) geterror ("Invalid function key request");
	if      (!(rbatch))
	{
		getftca[ 0 ] = 0;
		getftca[fkt] = 1;
	}
}

/************************************************************************/
/*      cread: Read the input                                           */
/************************************************************************/
signed32 cread (void)
{
	auto    signed32             in, c, d, m;
#ifdef  __OSF__
	auto    struct termios   ttysave;
#else
	  auto    struct termio   ttysave;
#endif

	if      (!(getruninit)) geterror ("cgetinit has not been called");
	getrun  = 1;
	getrc   = 0;
/*
	getwerg = -1;
*/
	getnl   = getncursor;
	getbl   = 0;

	if      (!rbatch)
	{
		  ioctl (1,TCGETA,&ttysave);
		  ioctl (1,TCSETAW,&ttysave);
	}

	do
	{
		getadj ();
		if     (getbl == -1)    getbl = strlen (getadr) -1;
		else                    getbl = 0;

		do
		{
			if ( getnl < getn )
			{
				c = getarr[getnl].line + getbl * getdl;
				d = getarr[getnl].coll + getbl * getdc;
				if      (ospeed > B9600) fputs (tgoto (pesc_cm,d-1,c-1),stdout);
				else                     tputs (tgoto (pesc_cm,d-1,c-1), 1, getoutc);
			}

			if      (!(rbatch))
			{
				in      = getentry ();
				if (getmodold == 0) getmodold = -1;

				if ((getmode & WAHLERG) && (getmode & DUNKEL))
					{
					if (in & FKTTAST)
						{
						switch (in & ~FKTTAST)
							{
							case FKTBS:
							case FKTDELCHAR:
							case FKTDELLINE:
							case FKTINSCHAR:
								cbell();
								break;

							default:
								getfkt(in & ~FKTTAST);
								break;
							}
						}
					else
						cbell();
					}
				else
					if      (in & FKTTAST)  
						getfkt (in & ~FKTTAST);
					else if (getnl < getn)
						{
						if      (in >= ' ')
							{
							(*getexit) 
							(GETFCHA,getadr,
						    	getarr[getnl].pmaske,
							in);
							}
						else    
							cbell ();
						}
						else    
							getnl = 0;
			}
			else
			{
				in = getchar();

				switch (in)
				{
				case EOF:               /*  EOF   */
					getbl = strlen (getadr);
					cerase();
					cgetterm();
#ifdef __hpux
                                        if ((term_type()) == 1) reset_hp_term();
#endif
					exit(0);
					break;
				case '\012':            /* MENU         */
					getbl = 80;
					break;
				case KTR:
					/* Kommentar bei Dateieingabe  */
					switch(getchar())
					{
					   case '*':   /* Kommentareintrag */
						     for (m=0;(in=getchar())
							!=EOF&&in!='\n'; m++)
							comment[m] = in;
						     comment[m] = '\0';
						     break;
					   default:     /* skip Kommentar */
						     while ((in=getchar())
							    !=EOF&& in!=KTR)
							    ;
						     break;
					}
					break;
					/* Ende Kommentar bei Dateieingabe */
				default:
#ifdef __hpux
					/* substitude "?" to ":" */
					if (in == 63) in = 58;
#endif
					if      (getnl >= getn)     cbell ();
					else if (getbl < (signed32)strlen (getadr))
					{
						(*getexit) (GETFCHA,getadr,
						    getarr[getnl].pmaske,in);
					}
					break;
				}
			}
		} while ((getbl >= 0)&&(getbl < (rbatch ? 80 : (int)strlen (getadr))));

		if      (getnl < getn)
		{
			(*getexit) (GETFEND,getarr[getnl].pfeld,
				    getarr[getnl].pmaske,' ');
			if      (getmode & WAHLERG)
			{
				getwahl (0,0,getarr[getnl].pfeld);
			}
			csay    (getarr[getnl].line,
				 getarr[getnl].coll,
				 getarr[getnl].pfeld,
				 getarr[getnl].pmaske);
		}

		if     (getbl < 0)
		{
			if      (getnl > 0)             getnl--;
			else if (getn  > 0)             getnl = getn -1;
			else                            getnl = 0;
		}
		else
		{
			if      ((getnl +1) < getn)     getnl++;
			else if (getftca[getrc])        getnl = getn;
			else                            getnl = 0;
		}
	} while ((getrc == 0) &&
		 ((getnl < getn)||(getftca[getrc] == 0)));

	if      (ospeed > B9600) fputs (tgoto (pesc_cm,79,23),stdout);
	else                     tputs (tgoto (pesc_cm,79,23),1,getoutc);

	fflush (stdout);
	while (--getn  >= 0) free (getarr [getn ].pmaske);
	while (--getwn >= 0) free (getwarr[getwn].pfeld);
	for    ( c = 0; c <= MAXFKT; c++) getftca[c] = 0;
	getftca[0] = 1;

	getmode    = 0;                 /* Output mode                  */
	getdl      = 0;                 /* Delta- Line                  */
	getdc      = 1;                 /* Delta- Column                */
	getexit    = getform;           /* Exit routine                 */
	getn       = 0;                 /* Number of output field       */
	getncursor = 0;                 /* Cursor field                 */
	getwn      = 0;                 /* Number of choice field       */
	getrun     = 0;                 /* cget / cread do not run      */
	if     (rbatch) sleep (1);      /* Wait by batch input          */
	return (getrc);
}
 
/************************************************************************/
/*      cforts: Continue to next test case by file input                */
/************************************************************************/

signed32 cforts (void)
{
	static  signed16   bstat;  /* Test-selector */
	auto    char    in;
	auto    signed32     m;

	bstat = 0;
	while (!bstat && (in = getchar()) != EOF)
	{
		if (in==KTR)
		{
		    switch(getchar())
		    {
			case '*':       /* comment to write out       */
				 for (m=0; (in=getchar())!=EOF&&in!='\n';m++)
				     comment[m] = in;
				 comment[m] = '\0';
				 bstat = 1;
				 break;
			default:
				 break;
		    }
		}
	}
	if (in==EOF)            /*  EOF   */
	{
		cerase();
		cgetterm();
#ifdef __hpux
    if ((term_type()) == 1) reset_hp_term();
#endif
		exit(1);
	}
}


static void cload_esc_seq(void)
{

static   char            escbuff  [2048];
auto     char           *pescbuff        = escbuff;
auto     char            termarea [2048];
auto     char           *pesc;
auto     Escap_seq      *pesc_run;
signed16 priv_term_file;


if (tgetent (termarea,getenv ("TERM")) <= 0)
	geterror ("Reading of TERM entry fails (tgetent == 0)");

priv_term_file = cload_term_file();

for (pesc_run = pesc_beg; pesc_run < pesc_end; pesc_run++)
	{
	if ((pesc = tgetstr (pesc_run->termname, &pescbuff)) == (char *)NULL)
		{
		if (priv_term_file)
			{
			if ((pesc = cread_priv_esc(pesc_run)) == (char *)NULL)
				pesc = pesc_run->esc;
			}
			
		else
			pesc = pesc_run->esc;
		}

	pesc_run->esc   = pesc;

	}

if (esc_seq[CL_POS].esc != (char *)NULL)
	pesc_cl        = esc_seq[CL_POS].esc;

if (esc_seq[CM_POS].esc != (char *)NULL)
	pesc_cm        = esc_seq[CM_POS].esc;

if (esc_seq[BL_POS].esc != (char *)NULL)
	pesc_bl        = esc_seq[BL_POS].esc;

if (esc_seq[SE_POS].esc != (char *)NULL)
	pesc_se        = esc_seq[SE_POS].esc;

if (esc_seq[AE_POS].esc != (char *)NULL)
	pesc_ae        = esc_seq[AE_POS].esc;

if (esc_seq[US_POS].esc != (char *)NULL)
	pesc_us        = esc_seq[US_POS].esc;

if (esc_seq[UE_POS].esc != (char *)NULL)
	pesc_ue        = esc_seq[UE_POS].esc;

if (esc_seq[SO_POS].esc != (char *)NULL)
	pesc_so        = esc_seq[SO_POS].esc;

if (esc_seq[KS_POS].esc != (char *)NULL)
	pesc_ks        = esc_seq[KS_POS].esc;

if (esc_seq[KE_POS].esc != (char *)NULL)
	pesc_ke        = esc_seq[KE_POS].esc;

}


static signed16	cload_term_file(void)
{

FILE	*fp;
char	line[LINE_LEN];
static  char	dummy_esc_seq_space[2048];
char	*dummy_esc_seq_ptr = dummy_esc_seq_space;

/*assuming one escape seq. could be max. of length 10 and there could be      */
/*max. of 10 repetitions for the same function key constant (e.g FKTTAB_LEFT).*/

static  char	act_esc_seq_space[10*NO_USED_CAP*10];
char	*act_esc_seq_ptr = act_esc_seq_space;

char	*tmp_esc_seq_ptr;
char	cap[20], sav_cap[20];
signed32 index;
char	sym_name[20];
char	termfile[TERMFNMAX] ;
boolean	use_own_adm_term_cap = FALSE;
char	*termfile_ptr;

signed32	i;

    char        env_dir[D27_LFILE_NAME];
    char      	*p_env = env_dir;

/* Check if there is an environment variable ADM_TERM_CAP */
use_own_adm_term_cap = FALSE;
if ((termfile_ptr = (char *)getenv(ADM_TERM_CAP)) != (char *) NULL)
{
    sprintf (termfile, "%s", termfile_ptr) ;

    /* Yeah, it exists ! Check if such a file can be read. */
    if ((fp = fopen(termfile, "r")) != (FILE *)NULL)
    {
	/* It has succeed, let's use this ! */
	use_own_adm_term_cap = TRUE;
    }
    /* else */				/* This can't be read :-(	*/
					/* Let's take the standard one	*/
	/* use_own_adm_term_cap = FALSE; */    /* Already set */
}

if(!use_own_adm_term_cap)
{
	    sprintf(p_env, "%s%s", dcelocal_path, D27_CLIENT_DIR);

    /* generate admin terminal capability file name */
    sprintf (termfile, "%s%s", p_env, TERMFNAME) ;

    if ((fp = fopen(termfile, "r")) == (FILE *)NULL)
	    return(FALSE);
}

/* this table should be initialised every time since cgetinit could be called */
/* after executing SHELL.						      */
for (i=0; i<NO_USED_CAP; i++)
	{
	term_file[i].present = FALSE;
	term_file[i].num_occur = 0;
	term_file[i].esc_seq = NULL;
	}

sav_cap[0] = '\0';

while (fgets(line, LINE_LEN, fp) != (char *)NULL)
	{
	if (line[0] == '#')
		continue;

	if (sscanf(line, "%s %s", cap, sym_name) != 2)
		{
		fclose(fp);
		geterror("Any non-commented line of private term_cap file must have at least two strings");
		}

	if (strcmp(cap, sav_cap) == 0)	
		term_file[index].num_occur++;
	else
		{
		strcpy(sav_cap, cap);

		/* look for capability in the array */
		index = csearch_cap_str(cap, 0, NO_USED_CAP);
	
		if (index < 0)
			{
			fclose(fp);
			geterror("Invalid terminal capability string encountered while loading private term_cap file");
			}

		if (term_file[index].num_occur != 0)
			{
			fclose(fp);
			geterror("Same terminal capability strings must appear together in private term_cap file");
			}

		term_file[index].present = TRUE;
		term_file[index].num_occur = 1;
		term_file[index].esc_seq = act_esc_seq_ptr;
		}

	if ((tmp_esc_seq_ptr = tgetstr (sym_name,&dummy_esc_seq_ptr)) == 
								(char *)NULL)
		{
		*act_esc_seq_ptr = '\0';
		act_esc_seq_ptr++;
		}
	else
		{
		strcpy(act_esc_seq_ptr, tmp_esc_seq_ptr);
		act_esc_seq_ptr += strlen(act_esc_seq_ptr) + 1;
		}
		
	}

fclose(fp);
return(TRUE);

}

signed32	csearch_cap_str(char *cap, signed32 first, signed32 last)
{
signed32	middle, comp;

    middle = (first + last)/2;

    comp = strcmp(term_file[middle].cap, cap);

    if (comp == 0)
	    return(middle);

    if (first == last)
	    return(-1);

    if (comp > 0)
	    return(csearch_cap_str(cap, first, middle - 1));
    else
	    return(csearch_cap_str(cap, middle + 1, last));
}

static char *cread_priv_esc(Escap_seq *pesc_spec)
{
Escap_seq      *pesc_run;
signed32	rep_cnt = 0; /* repetition count for a function key. integ. */
char	       *esc_seq;
signed32 	i;

for     (pesc_run = pesc_beg; pesc_run < pesc_end; pesc_run++)
	{
	if (pesc_run->fkt == pesc_spec->fkt)
		rep_cnt++;

	if (pesc_run == pesc_spec)
		break;
	}

for (i=0; i<NO_USED_CAP; i++)
	if (term_file[i].fkt == pesc_spec->fkt)
		{
		if (term_file[i].present != TRUE)
			i = NO_USED_CAP;

		break;
		}

if (i < NO_USED_CAP)
	{
	/* skip (rep_cnt-1) escape sequences in the i'th entry of term_file[] */

	if (term_file[i].num_occur >= rep_cnt)
		{
		esc_seq = term_file[i].esc_seq;

		for (i=1; i<rep_cnt; i++)
			esc_seq += strlen(esc_seq) + 1;

		if (*esc_seq == '\0')
			esc_seq = (char *)NULL;
		}
	else
		esc_seq = (char *)NULL;
		
	}
else
	esc_seq = (char *)NULL;

return(esc_seq);

}
#ifdef __hpux
/************************************************************************/
/*      define_keys: this function setup HP's function softkeys         */
/************************************************************************/
define_keys(key, attrib, label, defn)
int key, attrib;
char *label, *defn;
{
   static struct termios buff;
   char deflabel[6], defdefn[3];

   if (key < 1 || key > 8) {
      strcpy(deflabel, "   f1");       /* set up default keys */
      strcpy(defdefn, "\033p");
      for (key = 1; key <= 8; key++) {
         deflabel[4] = '0' + key;
         defdefn[1] = 'p' + key - 1;
         fprintf(stdout,"\033&f%da%dk%dd%dL%s%s",
           attrib, key, strlen(deflabel), strlen(defdefn), deflabel, defdefn);
      }
      fflush   (stdout);
      fputs  ("\033&j@", stdout);

      tcgetattr(0, &buff);
      buff.c_oflag = (ONLCR | OPOST); /* reset stty terminal to original */
      tcsetattr(0,TCSANOW, &buff);
   }
   else {
      fprintf(stdout,"\033&f%da%dk%dd%dL%s%s",
           attrib, key, strlen(label), strlen(defn), label, defn);
      fputs  ("\033&jB", stdout);
      fflush   (stdout);
   }
}

/************************************************************************/
/*      set_stty: this function setup stty to NO mapping NL to CR       */
/************************************************************************/
set_stty()
{

   struct termios buff;

   tcgetattr(0, &buff);
   buff.c_oflag &= ~(ONLCR); /* set "stty -onlcr" means no mapping NL to CR */
   tcsetattr(0,TCSANOW, &buff);
}

/************************************************************************/
reset_hp_term()
/************************************************************************/
{
        define_keys(0, 2, NULL, NULL); /* reset to default keys */
        fputs ("\033&j@",stdout); /* reset HP terminal */
}

/************************************************************************/
term_type()
/************************************************************************/
{
   char *term, *getenv();

   if ((term = getenv ("TERM")) != NULL) {
	if ( (strcmp(term, "hp") == 0) || (strcmp(term, "hpterm") == 0) )
		return(1);	/* Yes, it's hp terminal */
	else
		return(0);	/* No, it's NOT hp terminal */
   }
}
#endif /* __hpux*/
