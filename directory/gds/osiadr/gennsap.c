/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gennsap.c,v $
 * Revision 1.1.711.2  1996/02/18  18:20:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:44  marty]
 *
 * Revision 1.1.711.1  1995/12/08  16:07:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:08  root]
 * 
 * Revision 1.1.709.4  1994/09/06  12:26:15  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:23:46  keutel]
 * 
 * Revision 1.1.709.3  1994/07/06  15:08:01  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:18:52  marrek]
 * 
 * Revision 1.1.709.2  1994/06/21  14:47:49  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:00:11  marrek]
 * 
 * Revision 1.1.709.1  1994/02/22  16:11:12  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:01:47  marrek]
 * 
 * Revision 1.1.707.2  1993/10/14  17:52:28  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:42:59  keutel]
 * 
 * Revision 1.1.707.1  1993/10/13  17:33:55  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  17:42:09  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:41:27  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:30:55  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  21:29:49  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:34:24  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:13:44  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:09:00  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  18:12:21  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:34:14  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:42:33  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:50:41  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:40:58  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gennsap.c,v $ $Revision: 1.1.711.2 $ $Date: 1996/02/18 18:20:56 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    program                                                        */
/*                                                                           */
/*   NAME:    gennsap                                                        */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The program translates the content of an ASCII-file which describes */
/*       the supported NSAP-address types resp. DSP-formats into the binary  */
/*       format which is used by the NSAP-address handling software.         */
/*       During translation the content of the ASCII-file is checked for     */
/*       plausibility.                                                       */
/*                                                                           */
/*   CALLING SYNTAX:                                                         */
/*       gennsap <ifile> <ofile>                                             */
/*                                                                           */
/*   CALLING PARAMETER:                                                      */
/*       <ifile>   = Name of the ASCII-input file                            */
/*                                                                           */
/*       <ofile>   = Name of the binary output file                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*    Author: R.Horn                                      Date: 17.12.91     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <ctype.h>
#include <dce/d2dir.h>
#include <dce/osiaddr.h>

/* --------------- DECLARATION OF CONSTANTS ------------------------------ */

#define COMMENT		'#'
#define CONTCHAR	'>'
#define DSPIDCHAR	':'
#define DSPSIZEFLAG 	":"	
#define	DSPDEL		","
#define	DSPSIZEDEL	","
#define	RNGDEL		'-'
#define	TRUE		1
#define	FALSE		0
#define	MAX_FLAG_SIZE	100
#define	MAX_BUFFER	500

#define FLAG_FORMAT_ID			'I'
#define	FLAG_VAR_PART			'V'
#define	FLAG_VAR_SIZE			'v'
#define	FLAG_UNIT_TYPE			'u'
#define	FLAG_PAD_CHAR			'p'
#define	FLAG_LEFT_JUST			'L'
#define	FLAG_OPTIONAL			'O'
#define	FLAG_RANGE			'r'
#define	FLAG_RANGE_STRING		'R'

				    /* MASKS FOR ALLOWED COMBINATIONS */
/*
1-8 bits : Padding Characters
9	 : Format ID
10	 : VAR ID
11-12	 : VAR SIZE
13-16	 : DSP Unit type
17	 : DSP Justification
18	 : DSP Part Optional
19-20	 : DSP Part Allowed Range
*/

#define	FFI_ALLOW			0x000E0E00 
#define FVP_ALLOW			0x00000100 
#define	FVS_ALLOW			0x00000100 
#define	FUT_ALLOW			0x0000F000
#define	FPC_ALLOW			0x00000E00
#define	FOP_ALLOW			0x00000100
#define	FRNG_ALLOW			0x00080100
#define	FRNGU_ALLOW			0x00040100

#define	ERRMSG0		"ERROR(%d): USAGE: %s <NSAP-format input-file> <NSAP-format output-file>\n", __LINE__
#define	ERRMSG1		"ERROR(%d): can't open '%s' (errno = %d)\n", __LINE__
#define	ERRMSG2		"ERROR(%d): Unable to allocate memory\n", __LINE__
#define	ERRMSG3		"ERROR(%d): Invalid NSAP-format description\n", __LINE__
#define	ERRMSG4		"ERROR(%d): Unknown IDP-format -> %s\n", __LINE__
#define	ERRMSG5		"ERROR(%d): IDP-format description already available -> %s\n", __LINE__
#define	ERRMSG6		"ERROR(%d): No IDP-format description available\n", __LINE__
#define	ERRMSG7		"ERROR(%d): Wrong IDI-format\n", __LINE__
#define	ERRMSG8		"ERROR(%d): Wrong size or range of IDI -> %s\n", __LINE__
#define	ERRMSG9		"ERROR(%d): IDI-value/range resp. DSP-format-ID collision -> %.*s\n", __LINE__
#define	ERRMSG10	"ERROR(%d): Wrong DSP-format -> %s\n", __LINE__
#define	ERRMSG11	"ERROR(%d): DSP-format name already exists -> %s\n", __LINE__
#define	ERRMSG12	"ERROR(%d): Wrong # of DSP-parts\n", __LINE__
#define	ERRMSG13	"ERROR(%d): Wrong DSP-format (DSP-format-ID) -> %s\n", __LINE__
#define	ERRMSG14	"ERROR(%d): Wrong size of DSP (%s) -> %d\n", __LINE__
#define	ERRMSG15	"ERROR(%d): DSP-format description required -> %s\n", __LINE__
#define	ERRMSG16	"ERROR(%d): Unable to allocate memory\n", __LINE__
#define	ERRMSG17	"ERROR(%d): no NSAP-format description found\n", __LINE__

#define	ERRMSG20	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP Part Flags = '%s')\n\tDSP Part flag combination not allowed\n",__LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG21	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP Part Flags = '%s')\n\tFlags specified in the DSP part field are not allowed \n",__LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG22	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP Part Flags = '%s')\n\tLength of Format Identifier not correct ('%d')\n",__LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG23	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP Part Flags = '%s')\n\tIf a DSP part is specified optional allthe subsequent parts must also be specified optional\n",__LINE__ ,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG24	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\tNo DSP-Flag information available\n", __LINE__,idpfmt_err,dspfmt_err

#define	ERRMSG25	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP Part Flags = '%s')\n\tNot allowed to specify flag ('%c') in the Decimal DSP Syntax\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG26	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP Part Flags = '%s')\n\tError in the DSP Range('%s'-'%s')\n", __LINE__,idpfmt_err, dspfmt_err, dsppart_err

#define	ERRMSG27	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tDSP range cannot be NULL \n", __LINE__,idpfmt_err, dspfmt_err, dsppart_err

#define	ERRMSG28	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tLower Range cannot be greater then upper Range \n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG29	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags  = '%s') With DSP Format identifier only \
unit types 'ud'  or 'uD' or 'uh' or 'uH' or 'ua' or 'uA' are allowed \n", __LINE__,idpfmt_err, dspfmt_err, dsppart_err

#define	ERRMSG30	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tInvalid DSP Part Range specified ('Low = %s') and ('High = %s') \n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG31	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tFlag value ('%c') unknown \n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG32	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tFirst character in input field cannot be same as padding character (field == %s) \n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG33	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tDSP Part size ('%d') expanded to ('%d') cannot be greater then ('%d') \n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG34	"ERROR(%d): UNKNOWN DSP UNIT TYPE >>> SYSTEM ERROR CALL MAINTENANCE TEAM ... \n", __LINE__

#define	ERRMSG35	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = %s)\n\tPadding Character ('%c') not allowed with %s DSP Syntax \n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG36	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tRanges not allowed for the specified unit type\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG37	"ERROR(%d):\n\t(IDP format name = '%s')\n\t\t(DSP format name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tLow value ('%s') in %s notation cannot be less then ('%s')\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG38	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tHigh value ('%s') in %s notation cannot be less then ('%s')\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG39	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tIn Binary DSP Syntax the total \
size of non variable, non optional DSP parts must be integeral number of octets\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG40	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tWrong characters in Dspformat ('%s')\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG41	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tNot allowed to overwrite the input file ('%s') with the output file ('%s')\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG42	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tPadding character('%c') is not compatible with the unittype\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG43	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tThe %s range cannot contain more then the characters allowed for the DSP part\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG44	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tMinimum Number of units in the \
DSP part cannot be greater then Maximum number of units allowed\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG45	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tFormat Identifier must contain atleast ('%d') units\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG46	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tInvalid characters in the DSP Part\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG47	"ERROR(%d):DSP_PART ABNORMAL EXIT >>>>\n", __LINE__

#define	ERRMSG48	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tInvalid Flag specified ('%s') in  the DSP Part Description\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG49	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tThe string type ranges are not allowed with the specified unittype\n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG50	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tRange Error ==> Number of chars\
 in low range ('%d') & Number of chars in high range ('%d') \n", __LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG51	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tDSP Partsize is mandatory field and must always be specified as Decimal digits\n",__LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG52	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tIn Binary DSP Syntax the max. \
size of variable / optional DSP part must be integral number of Octets\n",__LINE__,idpfmt_err,dspfmt_err,dsppart_err

#define	ERRMSG53	"ERROR(%d):\n\t(IDP format Name = '%s')\n\t\t(DSP format Name = '%s')\n\t\t\t(DSP part Flags = '%s')\n\tError occured while extracting the range\n",__LINE__,idpfmt_err,dspfmt_err,dsppart_err

/* --------------- DECLARATION OF LOCAL DATA ----------------------------- */

	/* NSAP-address format description table */
static struct Nsap_info {
	char	*i_nsname ;
	char	i_afi0 ;
	char	i_afi1 ;
	char	i_dspsyntax ;
	char	i_idisize ;
	char	i_dspsize ;
} nsap_info[] =    { "X121_D",  36, 52, DEC_DSP_SYNTAX, 14, 24,
		     "X121_B",  37, 53, BIN_DSP_SYNTAX, 14, 24,
		     "DCC_D",   38,  0, DEC_DSP_SYNTAX,  3, 35,
		     "DCC_B",   39,  0, BIN_DSP_SYNTAX,  3, 34,
		     "TELEX_D", 40, 54, DEC_DSP_SYNTAX,  8, 30,
		     "TELEX_B", 41, 55, BIN_DSP_SYNTAX,  8, 30,
		     "PSTN_D",  42, 56, DEC_DSP_SYNTAX, 12, 26,
		     "PSTN_B",  43, 57, BIN_DSP_SYNTAX, 12, 26,
		     "ISDN_D",  44, 58, DEC_DSP_SYNTAX, 15, 23,
		     "ISDN_B",  45, 59, BIN_DSP_SYNTAX, 15, 22,
		     "ICD_D",   46,  0, DEC_DSP_SYNTAX,  4, 34,
		     "ICD_B",   47,  0, BIN_DSP_SYNTAX,  4, 34,
		     "LOCAL_D", 48,  0, DEC_DSP_SYNTAX,  0, 38,
		     "LOCAL_B", 49,  0, BIN_DSP_SYNTAX,  0, 38,
		     "LOC_ISO", 50,  0, ISO_DSP_SYNTAX,  0, 38,
		     "LOC_NAT", 51,  0, NAT_DSP_SYNTAX,  0, 38
		   } ;

char	idpfmt_err[MAX_IDPNM+1] ;
char	dspfmt_err[MAX_DSPNM+1] ;
char    dsppart_err[MAX_FLAG_SIZE] ;

short FIRST_ENTRY=TRUE ;
short OVERLAP=FALSE ;

/* --------------- PROTOTYPING OF LOCAL FUNCTIONS ------------------------- */

static signed32 setdsp_flag (FILE *, Nsap_dspinfo *, char, signed32, signed32) ;
static signed32 setflag (char *, signed32 *, char [], char, char *, char *) ;
static signed32 extract_range (char *, char *, char *) ;
static void toupperstr (char *) ;
static signed32 validate_range_str (char *, char *, signed32) ;
static signed32 validate_set_ranges (char, signed32 *, char *, char *,
								     signed32) ;
static signed32 make_part (char, Nsap_dspinfo *, signed32, signed32, signed32,
				signed32 ) ;
static signed32 verify_dotted_range (char *, char *, char) ;
static signed32 verify_slashed_range (char *, char *, char) ;
static signed32 prepare_format_id (Nsap_dspinfo *, char *, signed32, signed32) ;
int main (int argc, char *argv[]);

/* ------------------------------------------------------------------------ */

main (int argc, char *argv[]) {

    FILE		*fd ;
    char		tmpbuf[250], *bptr, *eptr ;
    char		idp_fname[MAX_IDPNM+1], dsp_fname[MAX_DSPNM+1] ;
    char		idilow[MAX_IDI+1], idihigh[MAX_IDI+1], delim, dummy ;
    signed32		result, tsidi, sidi, max_entry = 50 ;
    signed32		i, nparts, poff, soff, lidisize ;
    signed32		dspsize, nxtchar, hidisize, actdspsize ;
    Nsap_idpinfo	*pptr, *tpptr ;
    Nsap_dspinfo	*sptr, *tsptr ;
    struct Nsap_info	*iptr ;
    char		DSP_OPTIONAL_FLAG = 0 ;

    setlocale (LC_ALL, "") ;

    /* verify # of calling parameter */
    if (argc != 3) {
	fprintf (stderr, ERRMSG0, argv[0]) ;
	exit (1) ;
    }
    if (strcmp (argv[1], argv[2])==0) {
	fprintf (stderr, ERRMSG41, argv[1], argv[2]) ;
	exit (1) ;
    }

    /* open NSAP-format description input file */
    if ((fd = fopen (argv[1], "r")) == (FILE *) NULL) {
	fprintf (stderr, ERRMSG1, argv[1], errno) ;
	exit (1) ;
    }

    /* allocate temporary buffer */
    if ((bptr = (char *) malloc (max_entry * sizeof (Nsap_dspinfo))) ==
								(char *) NULL) {
	fprintf (stderr, ERRMSG2) ;
	exit (1) ;
    }
    eptr = bptr + (max_entry * sizeof (Nsap_dspinfo)) ;
    sptr = (Nsap_dspinfo *) bptr ;

    /* read and convert NSAP-format description */
    do {
	/* skip spaces, tabs and empty lines */
	if ((result = fscanf (fd, "%160[ \t\n]", tmpbuf)) == EOF) {
	    break ;
	}
	if ((nxtchar = getc (fd)) == EOF) {
	    result = EOF ;
	    continue ;
	}
	if (nxtchar == COMMENT) {
	    /* skip comment line */
	    if (fgets (tmpbuf, sizeof (tmpbuf), fd) == (char *) NULL)
		result = EOF ;	
	    continue ;
	}
	if (nxtchar != CONTCHAR) {
	    /* input is IDP-format description */
	    ungetc (nxtchar, fd) ;
	
	    /* get IDP-format name */
	    pptr = (Nsap_idpinfo *) sptr ;
	    if ((result = fscanf (fd, "%8s", idp_fname)) != 1) {
		fprintf (stderr, ERRMSG3) ;
		exit (1) ;
	    }

	    /* verify IDP-format name */
	    for (iptr = nsap_info; iptr < (struct Nsap_info *) ((char *) nsap_info +
						  sizeof (nsap_info)); iptr++)
		if (strcmp (idp_fname, iptr->i_nsname) == 0)
		    break ;
	    if (iptr == (struct Nsap_info *) ((char *) nsap_info +
							  sizeof (nsap_info))) {
		fprintf (stderr, ERRMSG4, idp_fname) ;
		exit (1) ;
	    }
	    strcpy (idpfmt_err, idp_fname) ;
	    for (tpptr = (Nsap_idpinfo *) bptr; tpptr < pptr;) {
		if (GET_ETYPE(tpptr->geninfo) == IDP_ENTRY) {
		    if (strncmp (tpptr->idp_fname, iptr->i_nsname,
						     (size_t) MAX_IDPNM) == 0) {
			fprintf (stderr, ERRMSG5, idp_fname) ;
			exit (1) ;
		    }
		    tpptr++ ;
		} else {
		    tpptr = (Nsap_idpinfo *) ((Nsap_dspinfo *) tpptr + 1) ;
		}
	    }

	    /* occupy new IDP-format description entry */
	    pptr->geninfo = SET_ETYPE(IDP_ENTRY) | SET_DSPSYN(iptr->i_dspsyntax) ;
	    pptr->afi0 = iptr->i_afi0 ;
	    pptr->afi1 = iptr->i_afi1 ;
	    strncpy (pptr->idp_fname, iptr->i_nsname, (size_t) MAX_IDPNM) ;
	    pptr->idi_size = iptr->i_idisize ;
	    pptr->dsp_size = iptr->i_dspsize ;

	    sptr = (Nsap_dspinfo *) (pptr + 1) ;
	}

	if (pptr->idi_size > 0) {
	    /* global IDP-format - get IDI-information */
	    result = fscanf (fd, "%*[ \t]%160[*+-0-9]", tmpbuf) ;
	} else {
	    /* local IDP-format - no IDI-information available */
	    strcpy (tmpbuf, "*") ;
	    result = 1 ;
	}
	if (result == 1) {
	    if (sptr == (Nsap_dspinfo *) bptr) {
		fprintf (stderr, ERRMSG6) ;
		exit (1) ;
	    }
	    /* DSP-format description available */
	    sptr->geninfo = SET_ETYPE(DSP_ENTRY) ;
	    memset ((void *) sptr->idi_lowval, (int) PADCHAR, (size_t) MAX_IDI) ;
	    memset ((void *) sptr->idi_highval, (int) PADCHAR, (size_t) MAX_IDI) ;
	    /* evaluate IDI-type */
	    if (strlen (tmpbuf) == (size_t) 1 && tmpbuf[0] == '*') {
		sptr->geninfo |= SET_IDITYPE(IDI_ALL) ;
	    } else {
		if (strlen (tmpbuf) == (size_t) 1 && tmpbuf[0] == '+') {
		    sptr->geninfo |= SET_IDITYPE(IDI_ALL_EXCL) ;
		} else {
		    idihigh[0] = '\0' ;
		    switch (sscanf (tmpbuf, "%15[0-9]%c%15[0-9]%c",
					 idilow, &delim, idihigh, &dummy)) {
		    case 1:
			sptr->geninfo |= SET_IDITYPE(IDI_SINGLE) ;
			break ;
		    case 3:
			sptr->geninfo |= SET_IDITYPE(IDI_RANGE) ;
			if ((hidisize = strlen (idihigh)) > iptr->i_idisize) {
			    fprintf (stderr, ERRMSG8, tmpbuf) ;
			    exit (1) ;
			}
			memcpy (&sptr->idi_highval[iptr->i_idisize - hidisize],
							    idihigh, hidisize) ;
			break ;
		    default:
			fprintf (stderr, ERRMSG7) ;
			exit (1) ;
			break ;
		    }
		    if ((lidisize = strlen (idilow)) > (size_t) iptr->i_idisize) {
			fprintf (stderr, ERRMSG8, tmpbuf) ;
			exit (1) ;
		    }
		    memcpy ((void *) &sptr->idi_lowval[iptr->i_idisize - lidisize],
					   (void *) idilow, (size_t) lidisize) ;
		    if (GET_IDITYPE(sptr->geninfo) == IDI_RANGE &&
			strncmp (sptr->idi_lowval, sptr->idi_highval,
					       (size_t) iptr->i_idisize) >= 0) {
			fprintf (stderr, ERRMSG8, tmpbuf) ;
			exit (1) ;
		    }
		}
	    }
	    /* get DSP-format name resp. # of DSP-parts */
	    if ((result = fscanf (fd, "%8s%d", dsp_fname, &nparts)) != 2) {
		fprintf (stderr, ERRMSG10, idp_fname) ;
		exit (1) ;
	    }
	    /* verify DSP-format name */
	    for (tsptr = (Nsap_dspinfo *) (pptr + 1); tsptr < sptr; tsptr++) {
		if (strncmp (dsp_fname, tsptr->dsp_fname, MAX_DSPNM) == 0) {
		    fprintf (stderr, ERRMSG11, dsp_fname) ;
		}
	    }
	    /* verify # of DSP-parts */
	    if (nparts < 1 || nparts > MAX_DSP_PARTS) {
		fprintf (stderr, ERRMSG12) ;
		exit (1) ;
	    }
	    strncpy (sptr->dsp_fname, dsp_fname, (size_t) MAX_DSPNM) ;
	    strncpy (dspfmt_err, dsp_fname, (size_t) MAX_DSPNM) ;
	    dspfmt_err[MAX_DSPNM] = '\0' ;
	    sptr->dsp_parts = nparts ;

	     /* All the major modifications to take care of new DSP format  */
	     /* description is done in the following "for" loop . In case of*/
	     /* any problems first debugging should be done on this loop    */

	    /* get size(s) of DSP-part(s) */
	    DSP_OPTIONAL_FLAG = 0 ;
	    for (dspsize = actdspsize = i = 0; i < nparts; i++) {
		if (setdsp_flag (fd, sptr, iptr->i_dspsyntax, i, nparts) != 0) {
		    fprintf (stderr, ERRMSG47) ;
		    exit (1); 
	         }
		 if (DSP_OPTIONAL_FLAG) {
		    if (!GET_DSP_OPTIONAL (sptr->dspflag[i])) {
		 	fprintf (stderr, ERRMSG23) ;
			exit (1) ;
		    }
		    if (GET_DSP_VAR_ID (sptr->dspflag[i]) ||
			GET_DSP_VAR_SIZE (sptr->dspflag[i]) ) {
		 	fprintf (stderr, ERRMSG23) ;
			exit (1) ;
		    }
		 }
		else if (GET_DSP_OPTIONAL(sptr->dspflag[i])) 
		     DSP_OPTIONAL_FLAG = 1 ;
		 dspsize += sptr->dsp_partsize[i] ;
		 if (!GET_DSP_OPTIONAL(sptr->dspflag[i]) && 
		     !GET_DSP_VAR_ID(sptr->dspflag[i])) 
		  	actdspsize += sptr->dsp_partsize[i] ;

		/* If DSP part is defined as variable and variable size id  */
		/* is not defined and DSP is not the last part of the DSP   */
		/* structure then set the variable id to default */
		if (GET_DSP_VAR_ID(sptr->dspflag[i]) && 
		   (!GET_DSP_VAR_SIZE(sptr->dspflag[i])) && (i != (nparts-1)))
		   sptr->dspflag[i] |= SET_DSP_VAR_SIZE(DSP_VAR_SIZE) ;

	        FIRST_ENTRY = TRUE ;
	    } /* END OF FOR LOOP */
	    if ((dspsize > iptr->i_dspsize) || (dspsize > MAX_DSP)) {
		fprintf (stderr, ERRMSG14, dsp_fname,
		  (iptr->i_dspsyntax == BIN_DSP_SYNTAX) ? dspsize/2 : dspsize) ;
		exit (1) ;
	    }
	    if (iptr->i_dspsyntax == BIN_DSP_SYNTAX && (actdspsize % 2) != 0) {
		fprintf (stderr, ERRMSG39) ;
		exit (1) ;
	    }
	    sptr++ ;
	} else {
	    /* no DSP-format description available */
	    if (pptr->idi_size == 0) {
		fprintf (stderr, ERRMSG15, idp_fname) ;
		exit (1) ;
	    }
	}
	if (eptr < (char *) (sptr + 2)) {
	    /* insufficient buffer space -> allocate additionally memory */
	    poff = (signed32) ((char *) pptr - bptr) ;
	    soff = (signed32) ((char *) sptr - bptr) ;
	    max_entry *= 2 ;
	    if ((bptr = (char *) realloc ((void *) bptr, 
	      (size_t) (max_entry * sizeof (Nsap_dspinfo)))) == (char *) NULL) {
		fprintf (stderr, ERRMSG16) ;
		exit (1) ;
	    }
	    eptr = bptr + (max_entry * sizeof (Nsap_dspinfo)) ;
	    pptr = (Nsap_idpinfo *) (bptr + poff) ;
	    sptr = (Nsap_dspinfo *) (bptr + soff) ;
	}
    } while (result != EOF) ;
    fclose (fd) ;

    if ((bptr == (char *) pptr) && (bptr == (char *) sptr)) {
	fprintf (stderr, ERRMSG17) ;
	exit (1) ;
    }
    eptr = (char *) sptr ;
    /* verify DSP-format description information */
    pptr = (Nsap_idpinfo *) bptr ;
    while (pptr < (Nsap_idpinfo *) eptr) {
	sptr = (Nsap_dspinfo *) (pptr + 1) ;
	while (sptr < (Nsap_dspinfo *) eptr &&
				       GET_ETYPE (sptr->geninfo) == DSP_ENTRY) {
	    tsptr = sptr + 1 ;
	    sidi = GET_IDITYPE(sptr->geninfo) ;
	    while (tsptr < (Nsap_dspinfo *) eptr &&
				      GET_ETYPE (tsptr->geninfo) == DSP_ENTRY) {
		if ((tsidi = GET_IDITYPE(tsptr->geninfo)) == IDI_ALL ||
		    sidi == IDI_ALL ||
		    (tsidi == IDI_ALL_EXCL && sidi == IDI_ALL_EXCL) ||
		    (tsidi == IDI_SINGLE && sidi == IDI_SINGLE &&
		    strncmp (tsptr->idi_lowval, sptr->idi_lowval,
					       (size_t) pptr->idi_size) == 0) ||
		    (tsidi == IDI_SINGLE && sidi == IDI_RANGE &&
		    (strncmp (tsptr->idi_lowval, sptr->idi_lowval,
						(size_t) pptr->idi_size) >= 0 &&
		    strncmp (tsptr->idi_lowval, sptr->idi_highval,
					      (size_t) pptr->idi_size) <= 0)) ||
		    (tsidi == IDI_RANGE && sidi == IDI_SINGLE &&
		    (strncmp (sptr->idi_lowval, tsptr->idi_lowval,
						(size_t) pptr->idi_size) >= 0 &&
		    strncmp (sptr->idi_lowval, tsptr->idi_highval,
					      (size_t) pptr->idi_size) <= 0)) ||
		    (tsidi == IDI_RANGE && sidi == IDI_RANGE &&
		    ((strncmp (sptr->idi_lowval, tsptr->idi_highval,
						(size_t) pptr->idi_size) <= 0 &&
		    strncmp (sptr->idi_lowval, tsptr->idi_lowval,
					       (size_t) pptr->idi_size) >= 0) ||
		    (strncmp (sptr->idi_highval, tsptr->idi_lowval,
						(size_t) pptr->idi_size) >= 0 &&
		    strncmp (sptr->idi_highval, tsptr->idi_highval,
					     (size_t) pptr->idi_size) <= 0)))) {
		   OVERLAP = FALSE ;
	  	   for (i=0;i<sptr->dsp_parts && i<tsptr->dsp_parts;i++)
		    if (!(sptr->dsp_id[i][0] != '\0'  && 
			  tsptr->dsp_id[i][0] != '\0' &&
			  sptr->dsp_partsize[i] == tsptr->dsp_partsize[i] &&
			  strcmp (sptr->dsp_id[i], tsptr->dsp_id[i]) != 0 &&
			  sptr->dspflag[i] == tsptr->dspflag[i])) {
				 OVERLAP = TRUE ;
		    }
		    else {
				OVERLAP = FALSE ;
				break ;
		    }
		   if (OVERLAP) {
			fprintf (stderr, ERRMSG9, MAX_IDPNM, pptr->idp_fname) ;
			exit (0) ;
		   }
	   	} 
		tsptr++ ;
	    }
	    sptr++ ;
	}
	pptr = (Nsap_idpinfo *) sptr ;
    }

    /* open binary NSAP-format description output file */
    if ((fd = fopen (argv[2], "w")) == (FILE *) NULL) {
	fprintf (stderr, ERRMSG1, argv[2], errno) ;
	exit (1) ;
    }

    /* write version # into file */
    fwrite ((void *) NSAPFORMVERS, (size_t) 1,
				 (size_t) D2_ALIGN(sizeof (NSAPFORMVERS)), fd) ;

    /* write binary NSAP-format description into file */
    fwrite ((void *) bptr, (size_t) 1, (size_t) (eptr - bptr), fd) ;

    fclose (fd) ;

    exit (0) ;
}

/* ------------------------------------------------------------------------ */

/* Will set the value of the dspflag and the FORMAT IDENTIFIER */

static signed32 setdsp_flag (FILE *fd, Nsap_dspinfo *sptr, char dspsyntax,
					signed32 dspnum, signed32 maxdspnum) {

    signed32	ret, partsz=0, partszmin=0 ;
    char	*flagptr, *ptrsz=(char *) NULL;
    char	*ptrminsz = (char *) NULL, *ptrmaxsz ;
    char	unittype ;
    char	inputstr[MAX_FLAG_SIZE] ;
    char	flags[MAX_FLAG_SIZE] ;
    char	size[MAX_FLAG_SIZE] ;
    char	dsp_id[MAX_DSP], *ptr, delim ;
	
    memset ((void *) sptr->dsp_id[dspnum], (int) '\0', (size_t) MAX_DSP) ;
    memset ((void *) sptr->dsp_lowval[dspnum], (int) '\0', (size_t) MAX_DSP); 
    memset ((void *) sptr->dsp_highval[dspnum], (int) '\0', (size_t) MAX_DSP) ;
    memset ((void *) inputstr, (int) '\0', (size_t) MAX_FLAG_SIZE) ;
    memset ((void *) flags, (int) '\0', (size_t) MAX_FLAG_SIZE) ;
    delim = '\0' ;

    /* Get the DSP Partsize. It is mandatory that each part must be  */
    /* have maximum field width in terms of unittype if defined */
    if (fscanf (fd, "%s", inputstr) != 1) {
	sptr->dsp_partsize[dspnum] = 0;  
	fprintf (stderr, ERRMSG24) ;
	return (-1) ;
    }

    ptrsz = strtok (inputstr, DSPSIZEFLAG) ;
    if (ptrsz == (char *) NULL) {
	sptr->dsp_partsize[dspnum] = 0;  
	fprintf (stderr, ERRMSG24) ;
	return (-1) ;
    }
    strcpy (size, ptrsz) ;
    flagptr = strtok ((char*) NULL, DSPSIZEFLAG) ;
    /* No flag description is available */
    if (flagptr != (char *) NULL) 
	strcpy (flags, flagptr) ;

    ptrminsz = strtok (size, DSPSIZEDEL) ;
    ptrmaxsz = strtok ((char *) NULL, DSPSIZEDEL) ;
    if (ptrmaxsz == (char *) NULL){
	ptrmaxsz = ptrminsz ;
	ptrminsz = (char *) NULL ;
    }
    if (ptrmaxsz != (char *) NULL && 
	isdigitstr (ptrmaxsz)) partsz = atoi (ptrmaxsz) ;
    else {
	fprintf (stderr, ERRMSG51) ;
	return (-1) ;
    }

    if (ptrminsz != (char *) NULL && isdigitstr (ptrminsz))
	partszmin = atoi (ptrminsz) ;
    else
	partszmin = 0 ;

    if (partszmin > partsz) {
	fprintf (stderr, ERRMSG44) ;
	return (-1) ;
    }
    strcpy (dsppart_err, flags) ;
    sptr->dspflag[dspnum] &= 0x00000000 ;
    if (flags[0] != '\0') {
 	ptr = strtok (flags, DSPDEL) ;
	if ((ret = setflag (ptr, &sptr->dspflag[dspnum], dsp_id, dspsyntax,
		   sptr->dsp_lowval[dspnum], sptr->dsp_highval[dspnum])) != 0) {
	    if (ret == -1)
		fprintf (stderr, ERRMSG20) ;
	    else
		if (ret == 1)
		    fprintf (stderr, ERRMSG21) ;
	    return (-1) ;
	}
    } /* End of ret == 3   */

    /* Since it is mandatory to specify the unit type with all the type */
    /* of parts , the unit type will be set to default	           */
    if (GET_DSP_UNIT(sptr->dspflag[dspnum])==0) {
	if (dspsyntax == BIN_DSP_SYNTAX)
	    sptr->dspflag[dspnum] |= SET_DSP_UNIT(DSP_UNIT_HEX2) ;
	else
	    sptr->dspflag[dspnum] |= SET_DSP_UNIT(DSP_UNIT_DEC1) ;
    }
    unittype = GET_DSP_UNIT(sptr->dspflag[dspnum]) ;

    /* This subroutine will make the size of DSP part field 	    */
    /* in number of digits or number of semi octets , it will also      */
    /* validate the padding character inputted by the user and set      */
    /* default padding characters if no padding characters are inputted */
    /* by the user 							    */
    if (make_part (dspsyntax, sptr, partsz, partszmin, dspnum, maxdspnum) < 0)
	return (-1) ;
    if (sptr->dsp_partsize[dspnum] > MAX_DSP) {
	fprintf (stderr, ERRMSG33, partsz, sptr->dsp_partsize[dspnum], MAX_DSP) ;
	return (-1) ;
    }
		
    /* With Format identifier it allowed to specify only the  */
    /* following unit types :             */
    /*			DSP_UNIT_DEC1     */
    /*			DSP_UNIT_DEC2     */
    /*			DSP_UNIT_HEX1     */
    /*			DSP_UNIT_HEX2     */
    /*			DSP_UNIT_ASCII1   */
    /*			DSP_UNIT_ASCII2   */
    /*	The default values of unit types will be same as for */
    /*	other parts of DSP. The following will make the validations  */
    /*	of unit type specified for Format identifier */
    if (GET_DSP_FORMAT_ID(sptr->dspflag[dspnum])) {
	unittype = GET_DSP_UNIT(sptr->dspflag[dspnum]) ;
	if ((unittype != DSP_UNIT_HEX1) && 
	    (unittype != DSP_UNIT_HEX2) && 
	    (unittype != DSP_UNIT_DEC1) &&
	    (unittype != DSP_UNIT_DEC2) &&
	    (unittype != DSP_UNIT_ASCII1) &&
	    (unittype != DSP_UNIT_ASCII2)) {
	    fprintf (stderr, ERRMSG29) ;
	    return (-1) ;
	}
    }
    if (GET_DSP_FORMAT_ID(sptr->dspflag[dspnum])) {
	if (prepare_format_id (sptr, dsp_id, dspnum, partsz) < 0)
	    return (-1) ;
    }

    if (validate_set_ranges (dspsyntax, &(sptr->dspflag[dspnum]), 
	   sptr->dsp_lowval[dspnum], sptr->dsp_highval[dspnum],
				      (signed32)sptr->dsp_partsize[dspnum]) < 0)
	return (-1) ;
    return (0) ;
}

/* ------------------------------------------------------------------------ */

/* This is recursive routine which will go into recursion and set all the */
/* given flags 								  */

static signed32 setflag (char *ptr, signed32 *dspflag, char dsp_id[MAX_DSP],
				  char dspsyntax, char *dsplow, char *dsphigh) {

    signed32	ret, pchar ;

    /* For the first entry the function should not be called because */
    /* it is already called one time in the calling routine */

    if (!FIRST_ENTRY) {
	if ((ptr = strtok ((char *) NULL, DSPDEL)) == (char *) NULL)
	    return (0) ;
    } else
	FIRST_ENTRY=FALSE ;

    switch (ptr[0]) {
    case FLAG_FORMAT_ID:
	if (*dspflag & (FFI_ALLOW))
	    return (-1); 
	*dspflag |= SET_DSP_FORMAT_ID(TRUE) ;
	if ((strlen (&(ptr[1])) > (size_t) MAX_DSP) ||
					     strlen (&(ptr[1])) <= (size_t) 0) {
	    fprintf (stderr, ERRMSG22, strlen (&(ptr[1]))) ;
	    return (-2) ;
	}
	strcpy (dsp_id, &(ptr[1])) ;
	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	    return (ret) ;
	break ;
    case FLAG_VAR_PART:
        if (strlen (ptr) > (size_t) 1) {
	    fprintf (stderr, ERRMSG48, ptr) ;
	    return (-2) ;
	}
	if ((*dspflag)&(FVP_ALLOW))
	    return (-1) ;
	*dspflag |= SET_DSP_VAR_ID(TRUE) ;
	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	    return (ret) ;
	break ;
    case FLAG_VAR_SIZE:
        if (strlen (ptr) > (size_t) 1) {
	    fprintf (stderr, ERRMSG48, ptr) ;
	    return (-2) ;
	}
	if ((*dspflag)&(FVS_ALLOW))
	    return (-1); 
	*dspflag |= SET_DSP_VAR_SIZE(DSP_VAR_SIZE) ;
	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	   return (ret) ;
	break ;
    case FLAG_UNIT_TYPE:
        if (strlen (ptr) != (size_t) 2 && ptr[1] != 's' && ptr[1] != 'S') {
	    fprintf (stderr, ERRMSG48, ptr) ;
	    return (-2) ;
	}
	if ((*dspflag)&(FUT_ALLOW))
	    return (-1) ;
	if (ptr[1] == 'd')
	    *dspflag |= SET_DSP_UNIT(DSP_UNIT_DEC1) ;
	else
	    if ((ptr[1] == 'D') && (dspsyntax == BIN_DSP_SYNTAX))
		*dspflag |= SET_DSP_UNIT(DSP_UNIT_DEC2) ;
	    else
		if ((ptr[1] == 'h') && (dspsyntax == BIN_DSP_SYNTAX))
		    *dspflag |= SET_DSP_UNIT(DSP_UNIT_HEX1) ;
		else
		    if ((ptr[1] == 'H') && (dspsyntax == BIN_DSP_SYNTAX))
			*dspflag |= SET_DSP_UNIT(DSP_UNIT_HEX2) ;
		    else
			if (ptr[1] == 'a') 
			    *dspflag |= SET_DSP_UNIT(DSP_UNIT_ASCII1) ;
			else
			    if ((ptr[1] == 'A') && (dspsyntax == BIN_DSP_SYNTAX))
				*dspflag |= SET_DSP_UNIT(DSP_UNIT_ASCII2) ;
			    else
				if (ptr[1] == 'o')
				    *dspflag |= SET_DSP_UNIT(DSP_UNIT_DOT1) ;
				else
				    if ((ptr[1] == 'O') && 
						  (dspsyntax == BIN_DSP_SYNTAX))
					*dspflag |= SET_DSP_UNIT(DSP_UNIT_DOT2) ;
				    else
					if (ptr[1] == 's') {
					    if (strlen (ptr) != (size_t) 3) {
						fprintf (stderr, ERRMSG48, ptr) ;
						return (-2) ;
					    }
					    if (ptr[2] == '1')
						*dspflag |=
						 SET_DSP_UNIT(DSP_UNIT_SLASH11) ;
					    else
						if (ptr[2] == '2')
						    *dspflag |=
						    SET_DSP_UNIT(DSP_UNIT_SLASH12) ;
						else {
						    fprintf (stderr, ERRMSG48, ptr) ;
			        		    return (-2) ;
						}
			   		} else
					    if ((ptr[1] == 'S') && 
						(dspsyntax == BIN_DSP_SYNTAX)) {
		        			if (strlen (ptr) != (size_t) 3) {
						    fprintf (stderr, ERRMSG48, ptr) ;
			        		    return (-2) ;
						}
						if (ptr[2] == '1')
						    *dspflag |=
						    SET_DSP_UNIT(DSP_UNIT_SLASH21) ;
						else
						    if (ptr[2] == '2')
							*dspflag |=
							SET_DSP_UNIT(DSP_UNIT_SLASH22) ;
						    else {
							fprintf (stderr, ERRMSG48, ptr) ;
			        			return (-2) ;
						    }
		   	  		    } else
						if ((dspsyntax == DEC_DSP_SYNTAX) &&
							(ptr[1] == 'D') ||
							(ptr[1] == 'H') ||
							(ptr[1] == 'A') ||
							(ptr[1] == 'O') ||
							(ptr[1] == 'S')) {
						    fprintf (stderr, ERRMSG25, ptr[1]) ;
						    return (-2) ;
			   			} else
						    return (1) ;

	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	    return (ret) ;
	break ;
    case FLAG_PAD_CHAR:
        if (strlen (ptr) > (size_t) 5) {
	    fprintf (stderr, ERRMSG48, ptr) ;
	    return (-2) ;
	}
	if ((*dspflag) & (FPC_ALLOW))
	    return (-1) ;
	if (ptr[1] == '\\') {
	    sscanf (&(ptr[2]), "%3d", &pchar) ;
	    *dspflag |= SET_PADCHAR(pchar) ;
	} else
	    *dspflag |= SET_PADCHAR(ptr[1]) ;
	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	    return (ret) ;
	break ;
    case FLAG_LEFT_JUST: 
        if (strlen (ptr) > (size_t) 1) {
	    fprintf (stderr, ERRMSG48, ptr) ;
	    return (-2) ;
	}
	*dspflag |= SET_DSP_JUST(DSP_LEFT_JUST) ;
	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	    return (ret); 
	break ;
    case FLAG_OPTIONAL:
        if (strlen (ptr) > (size_t) 1) {
	    fprintf (stderr, ERRMSG48, ptr) ;
	    return (-2) ;
	}
	if ((*dspflag)&(FOP_ALLOW))
	    return (-1) ;
	*dspflag  |= SET_DSP_OPTIONAL(TRUE) ;
	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	    return (ret) ;
	break ;
    case FLAG_RANGE:
	if ((*dspflag)&(FRNG_ALLOW))
	    return (-1); 
	*dspflag  |= SET_DSP_RANGE(DSP_RANGE) ;
	if (!extract_range (&(ptr[1]), dsplow, dsphigh)) {
	    fprintf (stderr, ERRMSG53) ;
	    return (-2) ;
	}
	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	    return (ret) ;
	break ;
    case FLAG_RANGE_STRING:
	if ((ret = extract_range (&(ptr[1]), dsplow, dsphigh)) == 1) {
	    if ((*dspflag)&(FFI_ALLOW))
		return (-1); 
	    *dspflag |= SET_DSP_FORMAT_ID(TRUE) ;
	    *dspflag |= SET_PADCHAR('-') ;
	    if ((strlen (dsplow) > (size_t) MAX_DSP) ||
						strlen (dsplow) <= (size_t) 0) {
		fprintf (stderr, ERRMSG22, strlen (&(ptr[1]))) ;
		return (-2) ;
	    }
	    strcpy (dsp_id, dsplow) ;
	    dsplow[0] = '\0' ;
	    dsphigh[0] = '\0' ;
	} else
	    if (ret == -1) { 
	 	fprintf (stderr, ERRMSG53) ;
		return (-2) ;
	    } else {
		if ((*dspflag)&(FRNGU_ALLOW))
		    return (-1); 
		*dspflag  |= SET_DSP_RANGE(DSP_RANGE_STRING) ;
	    }
	if ((ret = setflag (ptr, dspflag, dsp_id, dspsyntax, dsplow,
								 dsphigh)) != 0)
	    return (ret) ;
	break ;
    default:
	fprintf (stderr, ERRMSG31, ptr[0]) ;
	return (1) ;
    }
    return (0) ;
}

/* ------------------------------------------------------------------------ */

/* This function will extract the range from the strings of format 234-567 */

static signed32 extract_range (char *rng_str, char *dsplow, char *dsphigh) {

    signed16	i = 0, j = 0 ;

    if (strlen (rng_str) == (size_t) 0) { 
	fprintf (stderr, ERRMSG27) ;
	return (-1) ;
    }
    do {
	dsplow[i++] = rng_str[j++] ;
    } while ((rng_str[j] != '\0') && (rng_str[j] != RNGDEL) && 
					      j < (signed16) strlen (rng_str)) ;
    dsplow[i] = '\0' ;
    i = 0 ;
    if (rng_str[j] == '\0')
	return (1) ;
    j++ ;
    do {
	dsphigh[i++] = rng_str[j++] ;
    } while ((rng_str[j] != '\0') && j < (signed16) strlen (rng_str)) ;
    dsphigh[i] = '\0' ;
    return (2) ;
}

/* ------------------------------------------------------------------------ */

/* This function will convert the string in lower case to upper case         */

static void toupperstr (char *str) {

    signed32	i, l ;

    l = (signed32) strlen (str) ;
    for (i = 0; i < l; i++)
	str[i] = toupper (str[i]) ;
}

/* ------------------------------------------------------------------------ */

/* This function validates the lower and upper string ranges 		     */

static signed32 validate_range_str (char *lrange, char *hrange,
							      signed32 partsz) {

    if (strlen (lrange) <  (size_t) partsz) 
    	pad (lrange, LZERO, partsz, DSP_RIGHT_JUST) ;
    else
	if (strlen (lrange) != (size_t) partsz) {
	    fprintf (stderr, ERRMSG43, "lower") ;
	    return (-1) ;
        }
    toupperstr (lrange) ;
    if (strlen (hrange) < (size_t) partsz) 
	pad (hrange, LZERO, partsz, DSP_RIGHT_JUST) ;
    else
	if (strlen (hrange) != (size_t) partsz) {
	    fprintf (stderr, ERRMSG43, "higher") ;
	    return (-1) ;
        }
    toupperstr (hrange) ;
    if (strcmp (lrange, hrange) > 0) {
	fprintf (stderr, ERRMSG28) ;
	return (-1) ;
    }
    return (0) ;
}

/* ------------------------------------------------------------------------ */

/* This function will validate the normal DSP-part ranges (not unit ranges!!)*/

static signed32 validate_set_ranges (char dspsyntax, signed32 *dspflag,
				  char *lrange, char *hrange, signed32 partsz) {

    char	unittype, range ;

    unittype = GET_DSP_UNIT(*dspflag) ;
    range    = GET_DSP_RANGE(*dspflag) ;
    switch (unittype) {
    case DSP_UNIT_DEC1:
	if ((range != DSP_RANGE) && (range != DSP_RANGE_STRING)) {
	    lrange[0] = '\0' ;
	    hrange[0] = '\0' ;
	    return (0) ;
	}
	if ((!isdigitstr (lrange)) || (!isdigitstr (hrange))) {
	    fprintf (stderr, ERRMSG30, lrange, hrange) ;
	    return (-2) ;
	}
	if (range == DSP_RANGE) {
	    if ((strlen (lrange) != (size_t) 1) ||
						strlen (hrange) != (size_t) 1) {
		fprintf (stderr, ERRMSG50, strlen (lrange), strlen (hrange)) ;
		return (-2) ;
	    }
	    if (strcmp (lrange, hrange) > 0) {
		fprintf (stderr, ERRMSG28) ;
		return (-2) ;
	    }
	} else
	    if (validate_range_str (lrange, hrange, partsz) < 0)
		return (-1) ;
	break ;
   case DSP_UNIT_DEC2:
	if ((range != DSP_RANGE) && (range != DSP_RANGE_STRING)) {
	    lrange[0] = '\0' ;
	    hrange[0] = '\0' ;
	    return (0) ;
	}
	if ((!isdigitstr (lrange)) || (!isdigitstr (hrange))) {
	    fprintf (stderr, ERRMSG30, lrange, hrange) ;
	    return (-2) ;
	}
	if (range == DSP_RANGE) {
	   if ((strlen (lrange) != (size_t) 2) || strlen (hrange) !=(size_t) 2) {
		fprintf (stderr, ERRMSG50, strlen (lrange), strlen (hrange)) ;
		return (-2) ;
	    }
	    if (strcmp (lrange, hrange) > 0) {
		fprintf (stderr, ERRMSG28) ;
		return (-2) ;
	    }
	} else
	    if (validate_range_str (lrange, hrange, partsz) < 0)
		return (-1) ;
	break ;
    case DSP_UNIT_HEX1:
	if ((range != DSP_RANGE) && (range != DSP_RANGE_STRING)) {
	    lrange[0] = '\0' ;
	    hrange[0] = '\0' ;
	    return (0) ;
	}
	if ((!ishexstr (lrange)) || (!ishexstr (hrange))) {
	    fprintf (stderr, ERRMSG30, lrange, hrange) ;
	    return (-2) ;
	}
	if (range == DSP_RANGE) {
	    if ((strlen (lrange) != (size_t) 1) ||
						strlen (hrange) != (size_t) 1) {
		fprintf (stderr, ERRMSG50, strlen (lrange), strlen (hrange)) ;
		return (-2) ;
	    }
	    toupperstr (lrange) ;
	    toupperstr (hrange) ;
	    if (strcmp (lrange, hrange) > 0) {
		fprintf (stderr, ERRMSG28) ;
		return (-2) ;
	    }
	} else
	    if (validate_range_str (lrange, hrange, partsz) < 0)
		return (-1) ;
	break ;
    case DSP_UNIT_HEX2:
	if ((range != DSP_RANGE) && (range != DSP_RANGE_STRING)) {
	    lrange[0] = '\0' ;
	    hrange[0] = '\0' ;
	    return (0) ;
	}
	if ((!ishexstr (lrange)) || (!ishexstr (hrange))) {
	    fprintf (stderr, ERRMSG30, lrange, hrange) ;
	    return (-2) ;
	}
	if (range == DSP_RANGE) {
	    if ((strlen (lrange) != (size_t) 2) ||
						strlen (hrange) != (size_t) 2) {
	 	 fprintf (stderr, ERRMSG50, strlen (lrange), strlen (hrange)) ;
		 return (-2) ;
	    }
	    toupperstr (lrange) ;
	    toupperstr (hrange) ;
	    if (strcmp (lrange, hrange) > 0) {
		fprintf (stderr, ERRMSG28) ;
		return (-2) ;
	    }
	} else
	    if (validate_range_str (lrange, hrange, partsz) < 0)
		return (-1) ;
	break ;
    case DSP_UNIT_ASCII1:
    case DSP_UNIT_ASCII2:
	if (!range) {
	    lrange[0] = '\0' ;
	    hrange[0] = '\0' ;
	    return (0) ;
	}
	fprintf (stderr, ERRMSG36) ;
	return (-1) ;
    case DSP_UNIT_DOT1:
	if (range == DSP_RANGE_STRING) {
	    fprintf (stderr, ERRMSG49) ;
	    return (-1) ;
	}
	if (range != DSP_RANGE) {
	    *dspflag |= SET_DSP_RANGE(DSP_RANGE) ;
	    strcpy (lrange, DOT_LOW) ;
	    strcpy (hrange, DOT_HIGH) ;
	    return (0) ;
	} else
	    if ((!isdigitstr (lrange)) || (!isdigitstr (hrange))) {
		fprintf (stderr, ERRMSG26, lrange, hrange) ;
		return (-1) ;
	    }
	if (verify_dotted_range (lrange, hrange, DSP_UNIT_DOT1) < 0)
	    return (-1) ;
	break ;
    case DSP_UNIT_DOT2:
	if (range == DSP_RANGE_STRING) {
	    fprintf (stderr, ERRMSG49) ;
	    return (-1) ;
        }
	if (range != DSP_RANGE) {
	    *dspflag |= SET_DSP_RANGE(DSP_RANGE) ;
	    strcpy (lrange, DOT_LOW) ;
	    strcpy (hrange, DOT_HIGH) ;
	    return (0) ;
	} else
	    if ((!isdigitstr (lrange)) || (!isdigitstr (hrange))) {
		fprintf (stderr, ERRMSG26, lrange, hrange) ;
		return (-1) ;
	    }
	if (verify_dotted_range (lrange, hrange, DSP_UNIT_DOT2) < 0)
	    return (-1) ;
	break ;
    case DSP_UNIT_SLASH11:
	if (range == DSP_RANGE_STRING) {
	    fprintf (stderr, ERRMSG49) ;
	    return (-1) ;
	}
	if (range != DSP_RANGE) {
	    *dspflag |= SET_DSP_RANGE(DSP_RANGE) ;
	    strcpy (lrange, SLASH1_LOW) ;
	    strcpy (hrange, SLASH1_HIGH) ;
	    return (0) ;
	} else
	    if ((!isdigitstr (lrange)) || (!isdigitstr (hrange))) {
		fprintf (stderr, ERRMSG26, lrange, hrange) ;
		return (-1) ;
	    }
	if (verify_slashed_range (lrange, hrange, DSP_UNIT_SLASH11) < 0)
	    return (-1) ;
	break ;
    case DSP_UNIT_SLASH12:
	if (range == DSP_RANGE_STRING) {
	    fprintf (stderr, ERRMSG49) ;
	    return (-1) ;
	}
	if (range != DSP_RANGE) {
	    *dspflag |= SET_DSP_RANGE(DSP_RANGE) ;
	    strcpy (lrange, SLASH2_LOW) ;
	    strcpy (hrange, SLASH2_HIGH) ;
	    return (0) ;
	} else
	    if ((!isdigitstr (lrange)) || (!isdigitstr (hrange))) {
		fprintf (stderr, ERRMSG26, lrange, hrange) ;
		return (-1) ;
	    }
	if (verify_slashed_range (lrange, hrange, DSP_UNIT_SLASH12) < 0)
	    return (-1) ;
	break ;
    case DSP_UNIT_SLASH21:
	if (range == DSP_RANGE_STRING) {
	    fprintf (stderr, ERRMSG49) ;
	    return (-1) ;
	}
	if (range != DSP_RANGE) {
	    *dspflag |= SET_DSP_RANGE(DSP_RANGE) ;
	    strcpy (lrange, SLASH1_LOW) ;
	    strcpy (hrange, SLASH1_HIGH) ;
	    return (0) ;
	} else
	    if ((!isdigitstr (lrange)) || (!isdigitstr (hrange))) {
		fprintf (stderr, ERRMSG26, lrange, hrange) ;
		return (-1) ;
	    }
	if (verify_slashed_range (lrange, hrange, DSP_UNIT_SLASH21) < 0)
	    return (-1) ;
	break ;
    case DSP_UNIT_SLASH22:
	if (range == DSP_RANGE_STRING) {
	    fprintf (stderr, ERRMSG49) ;
	    return (-1) ;
	}
	if (range != DSP_RANGE) {
	    *dspflag |= SET_DSP_RANGE(DSP_RANGE) ;
	    strcpy (lrange, SLASH2_LOW) ;
	    strcpy (hrange, SLASH2_HIGH) ;
	    return (0) ;
	} else
	    if ((!isdigitstr (lrange)) || (!isdigitstr (hrange))) {
		fprintf (stderr, ERRMSG26, lrange, hrange) ;
		return (-1) ;
	    }
	if (verify_slashed_range (lrange, hrange, DSP_UNIT_SLASH22) < 0)
	    return (-1) ;
	break ;
    default:
	break ;
    }
    return (0) ;
}

/* ------------------------------------------------------------------------ */

/* This function will convert the actual partsize (which will be in unittype  */
/* to number of digits in case of Decimal DSP Syntax and number of Semi       */
/* Octets in case of the Binary DSP Syntax 				      */

static signed32 make_part (char dspsyntax, Nsap_dspinfo *sptr, signed32 partsz, 
			 signed32 partsz_min, signed32 dspnum, 
			 signed32 maxdspnum)
{
  int	tmp ;
  char    unittype, pchar, varpart, varsize ;

  unittype	= GET_DSP_UNIT(sptr->dspflag[dspnum]) ;
  pchar		= GET_PADCHAR(sptr->dspflag[dspnum]) ;
  varpart	= GET_DSP_VAR_ID(sptr->dspflag[dspnum]) ;
  varsize	= GET_DSP_VAR_SIZE(sptr->dspflag[dspnum]) ;
  if ((partsz == 1) && (pchar == '\0'))
	pchar = '-' ;
  switch (unittype) {
  case DSP_UNIT_DEC1 :
	sptr->dsp_partsize[dspnum] = (char)partsz;
	sptr->dsp_partsize_min[dspnum] = (char)partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if ((((signed32)(pchar) < 0x30) || 
	    ((signed32)(pchar) > 0x39)) &&
	    (pchar != '-')) {
     		fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;
  case DSP_UNIT_DEC2 :
	sptr->dsp_partsize[dspnum] = (char)2*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)2*partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if ((((signed32)(pchar) < 0x30) || ((signed32)(pchar) > 0x39)) &&
				   			 (pchar != '-')) {
		fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;
  case DSP_UNIT_HEX1   :
	sptr->dsp_partsize[dspnum] = (char)partsz;
	sptr->dsp_partsize_min[dspnum] = (char)partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if (((((signed32)(pchar) < 0x30) || ((signed32)(pchar)  > 0x39)) && 
	    (((signed32)(pchar) < 0x41) || ((signed32)(pchar)  > 0x5A))  &&
	    (((signed32)(pchar) < 0x61) || ((signed32)(pchar)  > 0x7A))) &&
				    			(pchar != '-')) {
	 	     	fprintf(stderr,ERRMSG42,pchar);
			return(-2);
	}
	break;
  case DSP_UNIT_HEX2   :
	sptr->dsp_partsize[dspnum] = (char)2*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)2*partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if (((((signed32)(pchar) < 0x30) || ((signed32)(pchar) > 0x39)) && 
	     (((signed32)(pchar) < 0x41) || ((signed32)(pchar) > 0x5A)) &&
	     (((signed32)(pchar) < 0x61) || ((signed32)(pchar) > 0x7A))) &&
				    			(pchar != '-')) {
	 	fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;
  case DSP_UNIT_ASCII1 :
	sptr->dsp_partsize[dspnum] = (char)3*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)3*partsz_min;
	break;
  case DSP_UNIT_ASCII2 :
	sptr->dsp_partsize[dspnum] = (char)2*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)2*partsz_min;
	break;
  case DSP_UNIT_DOT1   :
	sptr->dsp_partsize[dspnum] = (char)3*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)3*partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if ((((signed32)(pchar) < 0x30) || ((signed32)(pchar) > 0x39)) &&
				    			(pchar != '-')) {
	 	fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;
  case DSP_UNIT_DOT2   :
	sptr->dsp_partsize[dspnum] = (char)2*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)2*partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if ((((signed32)(pchar) < 0x30) || ((signed32)(pchar) > 0x39)) &&
				     			(pchar != '-')) {
	 	fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;
  case DSP_UNIT_SLASH11:
	sptr->dsp_partsize[dspnum] = (char)3*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)3*partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if ((((signed32)(pchar) < 0x30) || ((signed32)(pchar) > 0x39)) &&
				     			(pchar != '-')) {
	 	fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;
  case DSP_UNIT_SLASH12:
	sptr->dsp_partsize[dspnum] = (char)5*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)5*partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if ((((signed32)(pchar) < 0x30) || ((signed32)(pchar) > 0x39)) && 
				      			(pchar != '-')) {
	 	fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;
  case DSP_UNIT_SLASH21:
	sptr->dsp_partsize[dspnum] = (char)2*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)2*partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if ((((signed32)(pchar) < 0x30) || ((signed32)(pchar) > 0x39)) &&
				     			(pchar != '-')) {
	 	fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;
  case DSP_UNIT_SLASH22:
	sptr->dsp_partsize[dspnum]= (char)4*partsz;
	sptr->dsp_partsize_min[dspnum] = (char)4*partsz_min;
        if (pchar == '\0')
	    pchar = '0';
    	if ((((signed32)(pchar) < 0x30) || ((signed32)(pchar) > 0x39)) &&
				     			(pchar != '-')) {
	 	fprintf(stderr,ERRMSG42,pchar);
		return(-2);
	}
	break;

  default 	:
        fprintf(stderr,ERRMSG34);
	break;
  }
  /* This part of code is added to include the size of prefix in the  */
  /* the size of DSP part */
  tmp = (signed32)(sptr->dsp_partsize[dspnum]) ;
  if (((varpart) && (dspnum != (maxdspnum-1))) || ((varpart) && (varsize))){
	if (dspsyntax == BIN_DSP_SYNTAX) {
		if ( partsz <= MAX_BIN_VAR_LEN ) 
			tmp += 1 ;
		else 
			tmp += 2 ;
	}
	else {
		if ( partsz <= MAX_DEC_VAR_LEN )
			tmp += 1 ;
		else
			tmp += 2 ;
	}
	sptr->dsp_partsize[dspnum] = (char)tmp ;
  }
  if  (!isdigit(pchar) && (pchar != '-') && (dspsyntax == DEC_DSP_SYNTAX) && 
					    (unittype != DSP_UNIT_ASCII1)) {
	fprintf(stderr,ERRMSG35,pchar,"DECIMAL");
	return(-1); 
  }
  else 
    if ((!isxdigit(pchar)) && (pchar != '-') && 
		(dspsyntax == BIN_DSP_SYNTAX) &&
	     (unittype != DSP_UNIT_ASCII1) && (unittype != DSP_UNIT_ASCII2)) {
		fprintf(stderr,ERRMSG35,pchar,"BINARY");
		return(-1); 
	   }
  sptr->dspflag[dspnum] |= SET_PADCHAR(pchar); /* will reset the padding							  character   */
 return(0);
}

/* ------------------------------------------------------------------------ */

/* Will verify the range of the dotted inputs... opmode will indicate the   */
/* slashed format							    */

static signed32 verify_dotted_range (char *lowval, char *highval,
								char unittype) {

    signed32 range, ret ;

    if (lowval[0]==0)
	strcpy (lowval, DOT_LOW) ;
    if (highval[0]==0)
	strcpy (highval, DOT_HIGH) ;
    if (strlen (lowval) > (size_t) DOT_UNIT) {
	fprintf (stderr, ERRMSG37, lowval, "dotted", DOT_LOW) ;
	return (-1) ;
    } else
	if (strlen (lowval) < (size_t) DOT_UNIT) 
	    pad (lowval, LZERO, DOT_UNIT, DSP_RIGHT_JUST) ;

    if (strlen (highval) > (size_t) DOT_UNIT) {
	fprintf (stderr, ERRMSG37, lowval, "dotted", DOT_HIGH) ;
	return (-1) ;
    } else
	if (strlen (highval) < (size_t) DOT_UNIT)
	    pad (highval, LZERO, DOT_UNIT, DSP_RIGHT_JUST) ;

    if ((ret = strcmp (DOT_LOW, lowval)) > 0) {
	fprintf (stderr, ERRMSG37, lowval, "dotted", DOT_LOW) ;
	return (-1) ;
    }
    if ((ret = strcmp (highval, DOT_HIGH)) > 0) {
	fprintf (stderr, ERRMSG38, highval, "dotted", DOT_HIGH) ;
	return (-1) ;
    }
    if (unittype == DSP_UNIT_DOT2) {
	sscanf (lowval, "%3d", &range) ;
	sprintf (lowval, "%02X", range) ;
	sscanf (highval, "%3d", &range) ;
	sprintf (highval, "%02X", range) ;
    } else {
	pad (lowval, LZERO, DOT_UNIT, DSP_RIGHT_JUST) ;
	pad (highval, LZERO, DOT_UNIT, DSP_RIGHT_JUST) ;
    }
    return (0) ;
}

/* ------------------------------------------------------------------------ */

/* Will verify the range of the slashed inputs... opmode will indicate the  */
/* slashed format							    */

static signed32 verify_slashed_range (char *lowval, char *highval,
								char unittype) {

    signed32 range ;

    if (lowval[0]==0)
	strcpy (lowval, ((unittype == DSP_UNIT_SLASH11) ||
		    (unittype == DSP_UNIT_SLASH21)) ? SLASH1_LOW : SLASH2_LOW) ;
    if (highval[0]==0)
	strcpy (highval, ((unittype == DSP_UNIT_SLASH11) ||
		  (unittype == DSP_UNIT_SLASH21)) ? SLASH1_HIGH : SLASH2_HIGH) ;

    if ((strlen (lowval) > (size_t) ((unittype == DSP_UNIT_SLASH11) ||
	    (unittype == DSP_UNIT_SLASH21)) ? SLASH1_UNIT : SLASH2_UNIT) < 0 ) {
	fprintf (stderr, ERRMSG37, lowval, "slashed",
	    ((unittype == DSP_UNIT_SLASH11) || (unittype == DSP_UNIT_SLASH21))? 
						     SLASH1_LOW : SLASH2_LOW ) ;
	return (-1) ;
    } else
	if ((strlen (lowval) < (size_t) ((unittype == DSP_UNIT_SLASH11) ||
	      (unittype == DSP_UNIT_SLASH21)) ? SLASH1_UNIT : SLASH2_UNIT) < 0 )
	    pad (lowval, LZERO, ((unittype == DSP_UNIT_SLASH11) ||
			 	(unittype == DSP_UNIT_SLASH21)) ? 
		   		    SLASH1_UNIT : SLASH2_UNIT, DSP_RIGHT_JUST) ;

    if ((strlen (highval) > (size_t) ((unittype == DSP_UNIT_SLASH11) ||
				(unittype == DSP_UNIT_SLASH21)) ?
		   			      SLASH1_UNIT : SLASH2_UNIT) < 0 ) {
	fprintf (stderr, ERRMSG37, highval, "slashed",
		   ((unittype == DSP_UNIT_SLASH11) ||
		    (unittype == DSP_UNIT_SLASH21))? SLASH1_LOW : SLASH2_LOW ) ;
	return (-1) ;
    } else
	if ((strlen (highval) < (size_t) ((unittype == DSP_UNIT_SLASH11) ||
	      (unittype == DSP_UNIT_SLASH21)) ? SLASH1_UNIT : SLASH2_UNIT) < 0 )
	    pad (highval, LZERO, ((unittype == DSP_UNIT_SLASH11) ||
				(unittype == DSP_UNIT_SLASH21)) ?
		   		    SLASH1_UNIT : SLASH2_UNIT, DSP_RIGHT_JUST) ;

    if (strcmp (lowval, ((unittype == DSP_UNIT_SLASH11) || 
	       (unittype == DSP_UNIT_SLASH21)) ? SLASH1_LOW : SLASH2_LOW) < 0) {
	fprintf (stderr, ERRMSG37, lowval, "slashed",
		   ((unittype == DSP_UNIT_SLASH11) ||
		    (unittype == DSP_UNIT_SLASH21))? SLASH1_LOW : SLASH2_LOW ) ;
	return (-1) ;
    }

    if (strcmp (highval, ((unittype == DSP_UNIT_SLASH11) ||
	      (unittype == DSP_UNIT_SLASH21))? SLASH1_HIGH : SLASH2_HIGH) > 0) {
	fprintf (stderr, ERRMSG38, highval, "slashed", 
		 ((unittype == DSP_UNIT_SLASH11) ||
		  (unittype == DSP_UNIT_SLASH21))? SLASH1_HIGH : SLASH2_HIGH ) ;
	return (-1) ;
    }
    if (unittype == DSP_UNIT_SLASH11) {
	pad (lowval, LZERO, SLASH1_UNIT, DSP_RIGHT_JUST) ;
	pad (highval, LZERO, SLASH1_UNIT, DSP_RIGHT_JUST) ;
    } else
	if (unittype == DSP_UNIT_SLASH12) {
	    pad (lowval, LZERO, SLASH2_UNIT, DSP_RIGHT_JUST) ;
	    pad (highval, LZERO, SLASH2_UNIT, DSP_RIGHT_JUST) ;
	} else
	    if (unittype == DSP_UNIT_SLASH21) {
		sscanf (lowval, "%3d", &range) ;
		sprintf (lowval, "%.2X", range) ;
		sscanf (highval, "%3d", &range) ;
		sprintf (highval, "%.2X", range) ;
	    } else
		if (unittype == DSP_UNIT_SLASH22) {
		    sscanf (lowval, "%5d", &range) ;
		    sprintf (lowval, "%.4X", range) ;
		    sscanf (highval, "%5d", &range) ;
		    sprintf (highval, "%.4X", range) ;
		}
    return (0) ;
}

/* ------------------------------------------------------------------------ */

/* This function will validate, convert in internal format and pad the      */
/* character in the format identification inputted by the user		    */
/* Inputs : unittype : Part unittype 				            */
/* 	    dspid_in : Unprocessed ( raw /uncooked !!) DSP format id        */
/* 	    pchar    : Padding character				    */
/*	    partsz   : Expected Size of DSP Part			    */
/* Outputs : dspid_out : Will contain the prepared (and ready to serve !!!) */
/*			 DSP format identification			    */

static signed32 prepare_format_id (Nsap_dspinfo *sptr, char *dspid_in,
					 signed32 dspnum, signed32 unitpartsz) {

    char	unittype, just, pchar, pstr[10], dspid_out[MAX_DSP] ;
    signed32	partsz, partsz_min, res ;

    just 	= GET_DSP_JUST(sptr->dspflag[dspnum]) ;
    unittype  	= GET_DSP_UNIT(sptr->dspflag[dspnum]) ;
    pchar  	= GET_PADCHAR(sptr->dspflag[dspnum]) ;
    partsz  	= (int)sptr->dsp_partsize[dspnum] ;
    partsz_min  = (int)sptr->dsp_partsize_min[dspnum] ;

    memset ((void *) pstr, (int) '\0', (size_t) 10) ;
    if ((pchar != '-') && (pchar == dspid_in[0])) {
	fprintf (stderr, ERRMSG32, dspid_in) ;
	return (-1) ;
    }
    switch (unittype) {
    case DSP_UNIT_DEC1:
	if (!isdigitstr (dspid_in)) {
	    fprintf (stderr, ERRMSG40, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if (strlen (dspid_in) > (size_t) partsz) {
	    fprintf (stderr, ERRMSG13, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if (strlen (dspid_in) < (size_t) partsz_min) {
	    fprintf (stderr, ERRMSG45, partsz_min) ;
	    return (-1) ;
	}
   	if (strlen (dspid_in) < (size_t) partsz)
    	    pad (dspid_in, pchar, partsz, just) ;
   	else
	    if (strlen (dspid_in) != (size_t) partsz) {
	   	fprintf (stderr, ERRMSG22, strlen (dspid_in)) ;
	   	return (-1) ;
    	    }
	strcpy (dspid_out, dspid_in) ;
	break ;
    case DSP_UNIT_DEC2:
	if (!isdigitstr (dspid_in)) {
	    fprintf (stderr, ERRMSG40, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if ((strlen (dspid_in) % (size_t) 2) != (size_t) 0 ||
					  strlen (dspid_in) > (size_t) partsz) {
	    fprintf (stderr, ERRMSG13, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if (strlen (dspid_in) < (size_t) partsz_min) {
	    fprintf (stderr, ERRMSG45, partsz_min) ;
	    return (-1) ;
	}
   	if (strlen (dspid_in) < (size_t) partsz)
    	    pad (dspid_in, pchar, partsz, just) ;
   	else
	    if (strlen (dspid_in) != (size_t) partsz) {
	   	fprintf (stderr, ERRMSG22, strlen (dspid_in)) ;
	   	return (-1) ;
    	    }
	strcpy (dspid_out, dspid_in) ;
	break ;
    case DSP_UNIT_HEX1:
	if (!ishexstr (dspid_in)) { 
	    fprintf (stderr, ERRMSG40, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if ((strlen (dspid_in) % (size_t) 2) != (size_t) 0 ||
					    strlen (dspid_in) > (size_t) partsz)
	    if (strlen (dspid_in) > (size_t) partsz) {
		fprintf (stderr, ERRMSG13, sptr->dsp_fname) ;
		return (-1) ;
	    }
	if (strlen (dspid_in) < (size_t) partsz_min) {
	    fprintf (stderr, ERRMSG45, partsz_min) ;
	    return (-1) ;
	}
   	if (strlen (dspid_in) < (size_t) partsz)
    	    pad (dspid_in, pchar, partsz, just) ;
   	else
	    if (strlen (dspid_in) != (size_t) partsz) {
	   	fprintf (stderr, ERRMSG22, strlen (dspid_in)) ;
	   	return (-1) ;
    	    }
	strcpy (dspid_out, dspid_in) ;
	break ;
    case DSP_UNIT_HEX2:
	if (!ishexstr (dspid_in)) { 
	    fprintf (stderr, ERRMSG40, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if ((strlen (dspid_in) % (size_t) 2) != (size_t) 0 ||
					  strlen (dspid_in) > (size_t) partsz) {
	    fprintf (stderr, ERRMSG13, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if (strlen (dspid_in) < (size_t) partsz_min) {
	    fprintf (stderr, ERRMSG45, partsz_min) ;
	    return (-1) ;
	}
   	if (strlen (dspid_in) < (size_t) partsz)
    	    pad (dspid_in, pchar, partsz, just) ;
   	else
	    if (strlen (dspid_in) != (size_t) partsz) {
	   	fprintf (stderr, ERRMSG22, strlen (dspid_in)) ;
	   	return (-1) ;
    	    }
	strcpy (dspid_out, dspid_in) ;
	break ;
    case DSP_UNIT_ASCII1:
	if (!isASCIIstr (dspid_in)) {
	    fprintf (stderr, ERRMSG40, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if ((res = asci_to_hexdec (dspid_in, dspid_out, DSP_UNIT_ASCII1,
							     partsz_min)) < 0) {
	    if (res == DSP_MIN_UNIT_ERR)  
	  	fprintf (stderr, ERRMSG45, partsz_min/3) ;
	    else
	        fprintf (stderr, ERRMSG46) ;
	    return (-1) ;
	}
	sprintf (pstr, "%03d", (int)pchar) ;
   	if (strlen (dspid_out) < (size_t) partsz)
	    if (padunit (dspid_out, pstr, partsz, just)<0)
		return (-1) ;
   	else
	    if (strlen (dspid_out) != (size_t) partsz) {
	   	fprintf (stderr, ERRMSG22, strlen (dspid_out)) ;
	   	return (-1) ;
    	    }
	break ;
    case DSP_UNIT_ASCII2:
	if (!isASCIIstr (dspid_in)) {
	    fprintf (stderr, ERRMSG40, sptr->dsp_fname) ;
	    return (-1) ;
	}
	if ((res = asci_to_hexdec (dspid_in, dspid_out, DSP_UNIT_ASCII2,
							     partsz_min)) < 0) {
	    if (res == DSP_MIN_UNIT_ERR)  
		fprintf (stderr, ERRMSG45, partsz_min/3) ;
	    else
		fprintf (stderr, ERRMSG46) ;
	    return (-1) ;
	}
	if (strlen (dspid_out) < (size_t) partsz_min) {
	    fprintf (stderr, ERRMSG45, partsz_min/2) ;
	    return (-1) ;
	}
	sprintf (pstr, "%02x", (int)pchar) ;
   	if (strlen (dspid_out) < (size_t) partsz)
    	    if (padunit (dspid_out, pstr, partsz, just)<0)
		return (-1) ;
   	else
	    if (strlen (dspid_out) != (size_t) partsz) {
	   	fprintf (stderr, ERRMSG22, strlen (dspid_out)) ;
	   	return (-1) ;
    	    }
	break ;
    default:
	printf (">>>>>>>SYSTEM ERROR \n") ;
	break ;
    }
    strcpy (sptr->dsp_id[dspnum], dspid_out) ;
    return (0) ;
}
