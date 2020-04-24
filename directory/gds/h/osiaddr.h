/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: osiaddr.h,v $
 * Revision 1.1.761.2  1996/02/18  23:36:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:58  marty]
 *
 * Revision 1.1.761.1  1995/12/08  16:02:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:38  root]
 * 
 * Revision 1.1.759.1  1994/02/22  18:08:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:49:40  marrek]
 * 
 * Revision 1.1.757.2  1993/10/14  17:35:41  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:11  keutel]
 * 
 * Revision 1.1.757.1  1993/10/13  17:32:49  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:10:40  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:03:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:45:15  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:43:05  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:05  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:51:14  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:39:10  melman]
 * 
 * $EndLog$
 */
#ifndef _OSIADDR_H
#define _OSIADDR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char osiaddr_rcsid[] = "@(#)$RCSfile: osiaddr.h,v $ $Revision: 1.1.761.2 $ $Date: 1996/02/18 23:36:25 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/* ------------------------------------------------------------------------- */
/*              OSI-addressing header file                                   */
/* ------------------------------------------------------------------------- */

#include <gds.h>

/* ----------------- DEFINITION OF CONSTANTS ------------------------------- */

				/* IDI/DSP - leading digit values */
#define LZERO		'0'
#define LONE		'1'

#define	MAX_NO_SEL	3	/* max. no. of selectors inside of a PSAP-address */
#define MAX_DSP_PARTS	10	/* max. no. of components of a DSP */
#define MAX_DSP		38	/* max. size of a DSP-value (# of digits) */


#define DIGITBASE	0x30	/* basic conversion value (ASCII-values 0 - 9) */
#define XDIGITBASE	0x37	/* basic conversion value (ASCII-values A - F) */
#define xDIGITBASE	0x57	/* basic conversion value (ASCII-values a - f) */

#define ALIGN_VAL	"F"	/* octet alignment value on binary encoding */
#define ALIGN_VAL_CHAR	'F'	/* octet alignment value on binary encoding */

#define	SEL_SYNTAXMSK	3	/* selector syntax mask */
#define SEL_SYNTAXW	2	/* selector syntax field width */
#define SEL_ASCII	0	/* selector syntax (ASCII-string) */
#define SEL_ASCHEX	1	/* selector syntax (ASCII-HEX digit string) */
#define SEL_BINARY	2	/* selector syntax (BINARY) */
#define SEL_ASCEBCDIC	3	/* Changed from RESERVE to ASCII-EBCDIC */
#define SEL_P_FLD	0	/* field no. of P-selector */
#define SEL_S_FLD	1	/* field no. of S-selector */
#define SEL_T_FLD	2	/* field no. of T-selector */
#define SEL_SETSYNTAX(p, s, t) \
	(((p) << (SEL_SYNTAXW * SEL_P_FLD)) | \
	 ((s) << (SEL_SYNTAXW * SEL_S_FLD)) | \
	 ((t) << (SEL_SYNTAXW * SEL_T_FLD)))
#define SEL_GETSYNTAX(s, f) \
	(((s) >> (SEL_SYNTAXW * (f))) & SEL_SYNTAXMSK)

				/* name of the file which contains the NSAP- */
				/* address format description */
#define OSIFORM		"/nsapform.cfg"
				/* Version # of file format */
#define NSAPFORMVERS	"V1.1"

				/* NSAP-address decoding operation modes */
#define DEC_IDP_ONLY	1	/* decode IDP only */
#define DEC_DSP_ONLY	2	/* decode DSP only */
#define DEC_COMPLETE	3	/* decode complete NSAP-address */
#define DEC_DISPLAY	4	/* decode complete NSAP-address in displayable
				   form */

				/* Error values on PSAP/NSAP-en(de)coding */
#define NO_ERR			 0
#define PSAP_FORMAT_ERR		-1	/* PSAP-format error */
#define SEL_SIZE_ERR		-2	/* P-selector size error */
			     /* -3 */	/* S-selector size error */
			     /* -4 */	/* T-selector size error */
#define SEL_SYNTAX_ERR		-5	/* P-selector syntax error */
			     /* -6 */	/* S-selector syntax error */
			     /* -7 */	/* T-selector syntax error */
#define UNKNOWN_SELSYNTAX_ERR	-8	/* Unknown selector syntax error */
#define NO_NSAP_ERR		-9	/* No. of NSAP-addresses error */

#define NSAP_FORMAT_ERR		-10	/* NSAP-address format error */
#define NSAP_SIZE_ERR		-11	/* NSAP-address size error */
#define UNSUPPORTED_SYNTAX_ERR  -12	/* Unsupported NSAP-address syntax error */
#define UNKNOWN_SYNTAX_ERR	-13	/* Unknown NSAP-address syntax error */
#define UNKNOWN_AFI_ERR		-14	/* Unknown AFI error */
#define WRONG_AFI_ERR		-15	/* Wrong AFI error (if there is no DSP) */
#define IDI_SIZE_ERR		-16	/* IDI size error */
#define IDI_SYNTAX_ERR		-17	/* IDI syntax error */
#define DSP_NPARTS_ERR		-18	/* Wrong no. of DSP-components */
#define DSP_SIZE_ERR		-19	/* DSP-part size error */
#define DSP_SYNTAX_ERR		-20	/* DSP-part syntax error */
#define U_DSP_FORMAT_ERR	-21	/* Unknown DSP-format error */

					/* NSAP-address format description */
					/* file errors */
#define DESCFOPEN_ERR		-22	/* Can't open description file */
#define DESCVERS_ERR		-23	/* Wrong version # of description file */
#define DESCFREAD_ERR		-24	/* Can't read description file */
#define DESCMEM_ERR		-25	/* Can't get memory for storing */
					/* description file informations */
#define DESCSIZE_ERR		-26	/* Can't get size of description file */
#define PARAM_ERR		-27	/* calling parameter error */

/* DSP PART PROCESSING ERRORS					  */
#define U_DSP_PART_SYNTAX	-28	/* Unknown DSP-format error */
#define DSP_VAR_PART_ERR	-29	/* Error occured while processing 
					   variable DSP Part  */
#define	DSP_UNIT_MISMATCH	-30     /* The unit type defined in DSP part
					   flag is not allowed to occur with 
					   specified DSP Syntax */
#define	DSP_CONVERSION_FAILED	-31	/* DSP part conversion to the specified
					   DSP unittype failed */
#define	DSP_UNKNOWN_UNIT	-32	/* Invalid DSP unittype occurred   */
#define	DSP_WRONG_UNIT		-33	/* String entered in the DSP Part   */
					/* mismatch the unittype specified  */
#define	DSP_UNIT_IMPOSSIBLE	-34	/* Not possible to decode the incoming*/
					/* string to the specified unittype  */
#define	DSP_OUTOF_RANGE		-35	/* The DSP Part value is not within  */
					/* the range specified in the DSP form*/
					/* -at Description 		     */
#define	DSP_WRONG_CHAR		-36	/* Wrong characters in the DSP Part  */
					/* Description 			     */
#define	DSP_WRONG_VAR_SZ	-37	/* Error occurred while calculating  */
					/* prefix/size of variable DSP part  */
#define	DSP_WRONG_VAR_LENGTH	-38	/* Wrong DSP variable part length    */
					/* passed to function 		     */
#define	DSP_UNIT_PAD_ERR	-39     /* Error in padding the unit type    */
					/* ASCII1 or ASCII2		     */
#define	DSP_MIN_UNIT_ERR	-40     /* Atleast minimum number of units   */
					/* should occur for the DSP part     */
#define	DSP_BINARY_VAR_ERR	-41	/* Variable Size DSP's must always   */
					/* consist of integeral number of    */
				        /* octets in case of Binary DSP Syntax*/
#define	DSP_SYSTEM_ERR		-42  	/* Internal Error in system call      */
					/* maintenance staff 		     */

					/* hdladmpsap.c function return codes */
					/* Function return/error codes */
#define D2P_P032_ERR	       -101	/* Wrong no. of NSAP-address parts */
#define D2P_P033_ERR	       -102	/* Wrong size of NSAP-address */
#define D2P_P034_ERR	       -103	/* Invalid character in NSAP-address */
#define D2P_P035_ERR	       -104	/* Unsupported DSP-syntax */

#define D2P_S012_ERR	       -111	/* Wrong combination of different macro types */
#define D2P_S013_ERR	       -112	/* Max. no. of macro parameter exceeded */
#define D2P_S014_ERR	       -113	/* Wrong macro parameter format */
#define D2P_S015_ERR	       -114	/* Max. buffer size exceeded */

#define D2P_S022_ERR	       -121	/* Max. buffer size exceeded */

#define D2P_S032_ERR	       -131	/* Max. buffer size exceeded */

#define D2P_S042_ERR	       -141	/* Path name of installation not found */
#define D2P_S043_ERR	       -142	/* Can't open NSAP-macro file */
#define D2P_S044_ERR	       -143	/* Can't get state of NSAP-macro file */
#define D2P_S045_ERR	       -144	/* Can't get memory for storing macros */
#define D2P_S046_ERR	       -145	/* Can't get NSAP-macros */
					/* (multithreaded environment only) */
#define D2P_S047_ERR	       -146	/* Mutex-operation failed */
					/* (multithreaded environment only) */

#define D2P_S052_ERR	       -151	/* Invalid character in the input string */
#define D2P_S053_ERR	       -152	/* Input value exceeds limit */

#define D2P_S062_ERR	       -161	/* Wrong size of input string */
#define D2P_S063_ERR	       -162	/* Invalid character in the input string */

/* ----------------- DEFINITION OF DATA STRUCTURES ------------------------- */

					/* entry types */
#define IDP_ENTRY	1
#define DSP_ENTRY	2

#define SET_ETYPE(x)	(((x) & 0x03) << 0)
#define GET_ETYPE(x)	(((x) >> 0) & 0x03)

					/* IDI-types */
#define IDI_ALL		1
#define IDI_ALL_EXCL	2
#define IDI_SINGLE	3
#define IDI_RANGE	4

#define SET_IDITYPE(x)	(((x) & 0x07) << 2)
#define GET_IDITYPE(x)	(((x) >> 2) & 0x07)

					/* DSP syntax types */
#define DEC_DSP_SYNTAX	1
#define BIN_DSP_SYNTAX	2
#define ISO_DSP_SYNTAX	3
#define NAT_DSP_SYNTAX	4

#define SET_DSPSYN(x)	(((x) & 0x03) << 5)
#define GET_DSPSYN(x)	(((x) >> 5) & 0x03)


				/* Padding Characters */
#define	SET_PADCHAR(x)		(((x) & 0x07F) << 0)
#define	GET_PADCHAR(x)		(((x) >> 0) & 0x07F)

				/* Whether DSP Part is Format identifier */
				/* TRUE OR FALSE 			 */
#define	SET_DSP_FORMAT_ID(x)	(((x) & 0x01) << 8)
#define GET_DSP_FORMAT_ID(x)	(((x) >> 8) & 0x01)

				/* Whether DSP Part is Variable   	*/
			 	/* TRUE OR FALSE			*/
#define	SET_DSP_VAR_ID(x)	(((x) & 0x01) << 9)
#define GET_DSP_VAR_ID(x)	(((x) >> 9) & 0x01)


				/* If DSP Part is variable then only   	*/
				/* Size field of the DSP part		*/
#define	DSP_VAR_SIZE		1  /* In case of DECIMAL DSP SYNTAX = 1 digit */

#define	SET_DSP_VAR_SIZE(x)	(((x) & 0x03) << 10)
#define GET_DSP_VAR_SIZE(x)	(((x) >> 10) & 0x03)

#define DSP_UNIT_DEC1		1  /* Corresponds to flag = 'd' */ 
#define DSP_UNIT_DEC2		2  /* Corresponds to flag = 'D' */ 
#define	DSP_UNIT_HEX1		3  /* Corresponds to flag = 'h' */ 
#define	DSP_UNIT_HEX2		4  /* Corresponds to flag = 'H' */ 
#define	DSP_UNIT_ASCII1		5  /* Corresponds to flag = 'a' */ 
#define	DSP_UNIT_ASCII2		6  /* Corresponds to flag = 'A' */ 
#define	DSP_UNIT_DOT1		7  /* Corresponds to flag = 'o' */ 
#define	DSP_UNIT_DOT2		8  /* Corresponds to flag = 'O' */ 
#define	DSP_UNIT_SLASH11	9  /* Corresponds to flag = 's' for 3 digits */ 
#define	DSP_UNIT_SLASH12	10  /* Corresponds to flag= 's' for 5 digits*/ 
#define	DSP_UNIT_SLASH21	11 /* Corresponds to flag = 'S' for 2 hex digits*/ 
#define	DSP_UNIT_SLASH22	12 /* Corresponds to flag = 'S' for 4 hex digits*/ 
				   
#define	SET_DSP_UNIT(x)		(((x) & 0x0F) << 12)
#define GET_DSP_UNIT(x)		(((x) >> 12) & 0x0F)

#define	DSP_RIGHT_JUST		0  /* Default is right Justified */
#define	DSP_LEFT_JUST		1
#define	SET_DSP_JUST(x)		(((x) & 0x01) << 16)
#define	GET_DSP_JUST(x)		(((x)>>16) & 0x01)
			
				/* TRUE or FALSE */
#define	SET_DSP_OPTIONAL(x)	(((x) & 0x01) << 17)
#define	GET_DSP_OPTIONAL(x)	(((x)>>17) & 0x01)
				/* Ranges in DSP Parts */
#define	DSP_RANGE		1
#define	DSP_RANGE_STRING	2
#define	SET_DSP_RANGE(x)	(((x) & 0x03) << 18)
#define	GET_DSP_RANGE(x)	(((x)>>18) & 0x03)

#define MAX_IDI			15	/* max. size of an IDI-value */
#define MAX_IDPNM		8	/* max. size of an IDP-format name */
#define MAX_DSPNM		8	/* max. size of a DSP-format name */
#define MAX_DSPID		4	/* max. size of a DSP-format-id */
#define MAX_DSP_VAR_PREFIX	5	/* max. size of a DSP-var-part-prefix*/
#define PADCHAR			' '	/* padding character */

#define	MAX_DEC_VAR_LEN		9	/* max. length of DSP var part if DSP
					   syntax is decimal */
#define	MAX_BIN_VAR_LEN		15 	/* max. length of DSP var part if DSP
					   syntax is binary */

#define	DOT_LOW		"000"
#define	DOT_HIGH	"255"
#define	DOT_UNIT	3

#define	SLASH1_UNIT	3
#define	SLASH2_UNIT	5
#define	SLASH1_LOW	"000"
#define	SLASH2_LOW	"00000"
#define	SLASH1_HIGH	"255"
#define	SLASH2_HIGH	"65536"

					/* NSAP(IDP)-format description */

typedef struct {
	char	geninfo ;		/* general entry information */
	char	afi0 ;			/* AFI (if leading IDI-digit is 0) */
	char	afi1 ;			/* AFI (if leading IDI-digit is 1) */
	char	idp_fname[MAX_IDPNM] ;	/* name of NSAP(IDP)-format */
	char	idi_size ;		/* max. size of IDI (semi-octets) */
	char	dsp_size ;		/* max. size of DSP (semi-octets) */
	char    filler[3] ;		/* filler - structure must be well */
					/* aligned (4 bytes alignment) */
} Nsap_idpinfo ;

					/* NSAP(DSP)-format description */
typedef struct {
	char	geninfo ;		/* general entry information */
	char	idi_lowval[MAX_IDI] ;	/* (lowest) IDI value */
	char	idi_highval[MAX_IDI] ;	/* highest IDI value (optional) */
	char	dsp_lowval[MAX_DSP_PARTS][MAX_DSP] ;/* (lowest) DSP value */
	char	dsp_highval[MAX_DSP_PARTS][MAX_DSP] ;/* highest DSP value (optional) */
	char	dsp_fname[MAX_DSPNM] ;	/* name of NSAP(DSP)-format */
	signed32 dspflag[MAX_DSP_PARTS];	/* DSP STRUCTURE FORMAT FLAG */
	char	dsp_id[MAX_DSP_PARTS][MAX_DSP] ;   /* NSAP(DSP)-format */
	char	dsp_parts ;		/* # of DSP parts */
					/* size of DSP parts (semi-octets) */
	char	dsp_partsize_min[MAX_DSP_PARTS] ; /*Minimum length if
						    specified else zero */
	char	dsp_partsize[MAX_DSP_PARTS] ; /* Maximum length  */ 
} Nsap_dspinfo ;

				/* Selector description */
typedef struct {
	signed16 sel_type ;	/* selector type */
	signed16 sel_vers ;	/* Version no. of the selector information */
	signed16 sel_size ;	/* max. size of the selector (octets) */
} Sel_description ;

				/* PSAP-address information description */
typedef struct {
	signed32 psap_infolng ;
	char	 *psap_infoval ;
} Psap_info ;

/* --------------- function prototyping ------------------------------------ */

signed32 d2p_p01_str_to_bin_psap (char *, char *, char *, signed32, char *[],
								       byte *) ;
signed32 d2p_d01_bin_to_str_psap (byte *, char *, char *, char *, signed32,
							 char *[], signed32 *) ;
boolean d2p_psap_addr_match (byte *, byte *) ;
signed32 d2p_decode_psap_addr (Psap_info *, Psap_info *, Psap_info *,
				  signed32 *, signed32 *, Psap_info *, byte *) ;
signed32 d2p_encode_psap_addr (Psap_info *, Psap_info *, Psap_info *, signed32,
						signed32, Psap_info *, byte *) ;
signed32 d2p_decode_nsap_addr (signed32, signed32 *, char *, signed32 *,
				      char *[], signed32 [], byte *, signed32) ;
signed32 d2p_encode_nsap_addr (signed32, char *, signed32, char *[], byte *) ;
signed32 d2p_load_addr_formats (void) ;
signed32 d2p_srch_nsap_form (signed16, Nsap_idpinfo **, signed32, char *,
					      Nsap_dspinfo **, char *, void *) ;
signed32 encode_semi_octets (char *, signed32, byte *) ;
void decode_semi_octets (char *, signed32, signed32, byte *) ;
signed32 hexdec_to_dotdec (char *, char *t, signed32, signed32, signed32,
								     signed32) ;
signed32 hexdec_to_slashdec (char *, char *, char, signed32, signed32,
								     signed32) ;
signed32 hexdec_to_asci (char *, char *, signed32, signed32) ;
signed32 dotdec_to_hexdec (char *, char *, char, char, char *, char *,
								     signed32) ;
signed32 slashdec_to_hexdec (char *, char *, char, char, char *, char *,
								     signed32) ;
signed32 encode_var_part_prefix (char *, signed32, char, signed32, signed32) ;
signed32 asci_to_hexdec (char *, char *, char, char) ;
signed32 pad (char *, char, signed32, signed32) ;
signed32 padunit (char *, char *, signed32, signed32) ;
signed32 vardsppart (char *, signed32, char, char **, char *, signed32) ;
boolean	isdigitstr (char *) ;
boolean	isprintstr (char *) ;
boolean	ishexstr (char *) ;
boolean	isASCIIstr (char *) ;
boolean	dotted_dec_str (char *) ;
boolean slashed_dec_str (char *) ;
void removepad (char *, char, char, char) ;
void removepadstr (char *, char *, signed32) ;
void asc_ebc (char *, char *, signed32) ;
void ebc_asc (char *, char *, signed32) ;

/* Log information */

#ifndef NODEBUG
#define DEV	stderr
#define DBGPRINT(x) { \
	fprintf (DEV, "%s (%d): ", __FILE__, __LINE__) ; \
	fprintf  x ; \
	fflush (DEV) ; \
	}
#define DBGPRINTC(x) { \
	fprintf  x ; \
	fflush (DEV) ; \
	}
#else
#define DBGPRINT(x)
#define DBGPRINTC(x)
#endif
#endif /* _OSIADDR_H */
