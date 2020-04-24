/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27util.h,v $
 * Revision 1.1.12.2  1996/02/18  23:35:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:10  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:01:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:00  root]
 * 
 * Revision 1.1.10.4  1994/06/21  14:47:12  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:51:28  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  15:55:54  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:28:44  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:13  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:18:27  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:49  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  18:51:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:46:03  marrek]
 * 
 * Revision 1.1.8.1  1993/11/09  15:00:01  keutel
 * 	additional message types
 * 	[1993/11/09  13:33:34  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:43:53  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:41:04  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:41:11  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:50  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:10:19  marrek
 * 	November 1992 code drop
 * 	[1992/11/27  19:03:42  marrek]
 * 
 * Revision 1.1.2.3  1992/06/02  04:06:20  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:38  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:08:23  jim
 * 	Changes for AIX 3.2
 * 	[1992/04/09  19:43:09  jim]
 * 
 * Revision 1.1  1992/01/19  15:05:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _D27UTIL_H
#define _D27UTIL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d27util_rcsid[] = "@(#)$RCSfile: d27util.h,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 23:35:55 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : d27util.h                                           */
/*                                                                    */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 15. 12. 1987                                        */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOK.-REF.    : DS-Design-Specification                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                used for directory interfaces (DUA, DSA, C-Stub,    */
/*                S-Stub, ...)                                        */
/*                                                                    */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*                                                                    */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 87-12-15| Original                       | WS |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <stdio.h>
#include <signal.h>
#include <d23apdu.h>
#include <dce/asn1.h>
#include <dce/d2dir.h>

typedef enum {D27_CS_UNDEF,D27_CS_103,D27_CS_87,D27_CS_126} D27_codeset;

#define SINIX     3             /* used for "malgcfe.h" */
#define OPSYSTEM  SINIX
#define L_BENID   5

#define D27_NOOFLOGREC  2000  /* Number of Log-Records  */

#define D27_CHECK_KEY_ERROR 24          /* error from check_key   */
#define D27_START_ERROR     25          /* process cannot start   */

#define D2_EVER   ;;                    /* for EVER Schleife          */

/***************************************/
/* Defines for syntax-check            */
/***************************************/

#define D27_0_ALLOWED    1      /* T61String including '\0'           */
#define D27_0_NOTALLOW   0      /* T61String without   '\0'           */

/******************************************/
/* other defines                          */
/******************************************/

#define D27_LINST_NAME  (5 * 15 + 1)    /* max. length of a path name of  */
					/* an installation directory      */
#define D27_LFILE_NAME  (D27_LINST_NAME + 3 * 15)
					/* max. length of a path name to  */
					/* a file within the directory    */
					/* installation                   */

#define D27_CACHE_DIR   "/cache"        /* subdirectory for DUA-Cache     */
#define D27_SUBCACHE_DIR "/CACHEDIR"
#define D27_DUA_DIR     "/dua"          /* subdirectory for DUA           */
#define D27_DSA_DIR     "/dsa"          /* subdirectory for DSA           */
#define D27_SUBDSA_DIR  "/dir"
#define D27_DSANAME     "/dsaname"      /* DSA name file                  */
#define D27_DSA_D_VERS  "/dversion"     /* DSA data version file          */
#define D27_DSA_CONF    "/conf"         /* DSA configuration file         */
#define D27_ASN1_ATTR   "/asn1_attr"    /* file containing information    */
					/* about ASN1 representation for  */
					/* attributes                     */

#define D27_GDSCONFIG	"/gdsconfig"	/* file containing information    */
					/* about the Directory            */
					/* configuration                  */
					/* The following informations     */
					/* about gdsconfig is used        */
					/* in gdsditadm to retrieve       */
					/* the auth. mech. supported      */
					/* by the current Dir-Id          */
#define D27_GDSCONFIG_NEW	"/gdsconfig.new" /* Temporary file        */
#define GDSCONFIG_LINE_LEN	256	/* Max. line lengh in gdsconfig   */
#define DIR_ID_KEY	"DIR-ID:"	/* Keyword for Dir-Id             */
#define UPDATE_KEY	"UPDATE:"	/* Keyword for update shadow job  */
#define UPDATE_ON	'*'		/* Value for update on            */
#define UPDATE_OFF	'-'		/* Value for update off           */
#define AUTH_MECH_KEY	"AUTH-MECH:"	/* Keyword for Auth.mech.         */

#define D27_CSTUB_DIR   "/cstub"        /* subdirectory for C-STUB        */
#define D27_SSTUB_DIR   "/sstub"        /* subdirectory for S-STUB        */
#define D27_IPC_DIR     "/ipc"          /* subdirectory for IPC           */
#define D27_ADM_DIR     "/adm"          /* subdirectory for ADM           */
#define D27_BIN_DIR     "/bin"          /* subdirectory for binaries      */
#define D27_CONF_DIR    "/conf"         /* subdirectory for configuration */
#define D27_ETC_DIR     "/etc"          /* subdirectory for etc           */
#define D27_SERVER_DIR  "/var/directory/gds"    /* subdirectory for server*/
						/* components             */
#define D27_CLIENT_DIR  "/var/adm/directory/gds"/* subdirectory for client*/
						/* components             */
#define D27_DCE_ADM        "/adm"          /* subdirectory under SERVER   */

#define D27_LOGFNM      "/log_"         /* Logfile-Name                   */

#define D27_PRINTABLE	"PRINTABLE"	/* environment variable to use print-*/
					/* able representation instead of  */
					/* T61.				   */
#define D27_LOG         "D2_LOG"        /* name of the environment variable*/
					/* which defines whether logging ..*/
#define D27_PFMLOG      "D2_PFMLOG"     /* name of the environment variable*/
					/* which defines whether           */
					/* performance logging ...         */
					/* must be done or not:            */
#define D27_LMODE       "on"            /* value = on logging must be done;*/
#define D27_NLMODE      "off"           /* value = off without logging     */

				    /* name of schema object:         */
				    /* "85.4.3^Cschema"               */
#define D2_NAME_SCHEME   "\070\065\056\064\056\063\004\
\163\143\150\145\155\141"
				    /* name of local DSA:             */
				  /* "43.12.2.1107.1.3.4.9^ALOCAL-DSA"*/
#define D2_N_LOC_DSA     "\064\063\056\061\062\056\062\056\061\061\060\067\056\061\056\063\056\064\056\071\003\
\114\117\103\101\114\055\104\123\101"
				    /* name of default DSA in cache:  */
				/* "43.12.2.1107.1.3.4.8^ADEFAULT-DSA"*/
#define D2_N_DEF_DSA     "\064\063\056\061\062\056\062\056\061\061\060\067\056\061\056\063\056\064\056\070\003\
\104\105\106\101\125\114\124\055\104\123\101"
				    /* name of client in cache:       */
				  /* "43.12.2.1107.1.3.4.10^ACLIENT"  */
#define D2_N_CLIENT      "\064\063\056\061\062\056\062\056\061\061\060\067\056\061\056\063\056\064\056\061\060\003\
\103\114\111\105\116\124"

				    /* delay and retry times for dsa  */
				    /* and c-stub                     */
#define D27_AT_DELAY    1           /* delay for bind                 */
#define D27_DT_RETRY   10           /* maximum retries for bind       */
#define D27_GT_DELAY   60           /* delay for read dsaname         */

				    /* characters defining ranges     */
#define D27_CAPITAL_A 0x41
#define D27_CAPITAL_Z 0x5a
#define D27_SMALL_A  0x61
#define D27_SMALL_Z  0x7a

				    /* bitlist for country-syntax     */
#define D27_CNTRY_BITLIST_LEN  (26 * 26 / 8 + 1)

/************** constants for decoding and encoding functions     *****/

#define D27_DAP_AC       1  /* application context is DAP             */
#define D27_DSP_AC       2  /* application context is DSP             */
#define D27_LOC_AC       1  /* local access                           */
#define D27_REM_AC       2  /* remote access                          */
#define D27_PRIV_MIG_TS  0  /* transfer syntax is private (migration) */
#define D27_ASN1_TS      1  /* transfer syntax is ASN1                */
#define D27_PRIV_TS      2  /* transfer syntax is private             */
#define D27_ASN1_ADM_TS  3  /* transfer syntax is ASN1; for administration */

#define D27_ACSE_AS      1  /* abstract syntax is ACSE                */
#define D27_DAP_AS       2  /* abstract syntax is DAP                 */
#define D27_DSP_AS       3  /* abstract syntax is DSP                 */

/************** Macros                                          *******/

#define D27_ATTCMP(x,y)   ((x)->d2_typ_len != (y)->d2_typ_len ||    \
		      memcmp((x)->d2_type, (y)->d2_type, (x)->d2_typ_len))
			       /* compares two attribute types        */
			       /* param: addresses of struct D2_a_type*/
#ifdef THREADSAFE
#define SIGACTION d27_sigactionmap
extern int d27_sigactionmap (int sig, const struct sigaction *act,
							struct sigaction *oact);
#else
#define SIGACTION sigaction
#endif

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        DATA                 */
/*                                                                    */
/**********************************************************************/

			/* exported by d27util.c                      */
extern int          d27_pid;        /* process id of current process  */
extern char         d27_proc_name[];/* Name of current process        */

			/* exported by d27cbl.c                       */
extern byte         d27_country_bitlist[];

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

			/* exported by d27util.c                      */
extern void     d27_000_log_signal(int signal);
extern void     d27_010_set_error_apdu(D23_rserror *error, signed16 ret_code,
	signed16 err_class, signed16 err_value, signed32 *l_err);
extern void     d27_013_get_utc_time(char *string);

#ifdef THREADSAFE
extern unsigned32 d27_thread_id ( void );
#endif

			/* exported by d27close.c                     */
extern void     d27_011_close_unused_files(boolean close_all);

			/* exported by d27sort.c                      */
extern void     d27_007_norm_name(char *inp, signed16 length, char *out);
extern signed16 d27_012_remove_blanks(char *inp, signed16 length, char *out);
extern signed16 d27_018_sort_name(D2_name_string dname);
extern void     d27_031_norm_pr_string(byte *in, byte *out, signed16 len);

			/* exported by d27shid.c                      */
extern void     d27_002_set_error_code(D2_pbhead *head, signed16 ret_code,
	signed16 err_class, signed16 err_value);
extern char     *d27_201_fill_msg_dn(D2_name_string name,
	D23_ds_name *dsname_ptr);

			/* exported by d27obid.c                      */
extern signed32 d27_301_oid_str(D2_obj_id *oid,char *s);
extern signed32 d27_302_str_oid(char *s, D2_obj_id *oid);
extern signed32 d27_303_cmp_obj_id(D2_obj_id *type1, D2_obj_id *type2);
extern signed32 d27_307_init_att(char *p_env);
extern signed32 recover_att_syntax(asn1_oid *oid);

			/* exported by d27syn2rep.c                   */
extern signed16 d27_401_syntax_to_rep(signed16 syntax);

			/* exported by d27copy.c                      */
extern void     d27_501_copy_common_arguments(byte **from, byte **to);
extern void     d27_502_copy_distinguished_name(byte **from, byte **to);
extern void     d27_503_copy_security_parameters(byte **from, byte **to);

			/* exported by d27sntx.c                      */
#ifdef PERMANENT
extern signed32 d27_601_is_T61String(byte *val, signed16 len,
	unsigned16 incl_0);
extern signed32 d27_602_is_graph_char(byte *val, unsigned16 incl_0);
extern signed32 d27_603_is_diacr_comb(byte *val);
#endif

extern signed32 d27_605_is_PrtString(byte *val, signed16 len);
extern signed32 d27_606_is_IA5String(byte *val, signed16 len);
extern signed32 d27_608_is_country(byte *val);

			/* exported by d27cbl.c                       */
extern signed32 d27_609_compute_cntry_bitlist(FILE *fp);

			/* exported by d27t61.c                       */
extern signed32 d27_601_is_T61String(byte *val, signed16 len);
extern void     d27_651_norm_T61String(byte *in, byte *out, signed16 len);
extern void     d27_681_norm_102_char(byte **in, byte **out);
extern signed16 d27_691_t61grep(byte **str, signed16 *len,
    D27_codeset *codeset, signed16 position, byte *substr, signed16 sublen);

			/* exported by d27dme.c                       */
extern d2_ret_val gds_decode_alt_addr (const D2_str *, D2_alt_addr **);
extern d2_ret_val gds_encode_alt_addr (const D2_alt_addr *, D2_str **);

			/* exported by d27prmsg.c                       */
extern void d27_printmsg (FILE * , byte * , signed32 );

#endif /* _D27UTIL_H */
