/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: genmsg.c,v $
 * Revision 1.1.11.2  1996/02/18  18:19:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:06  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:58:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:06  root]
 * 
 * Revision 1.1.9.1  1994/02/22  18:07:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:41:38  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  14:59:42  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:00:05  marrek]
 * 
 * Revision 1.1.5.4  1992/12/31  20:38:30  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:21  bbelch]
 * 
 * Revision 1.1.5.3  1992/12/17  23:13:21  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	Also check argv[1] before call atoi on it.
 * 	[1992/12/17  23:08:37  tom]
 * 
 * Revision 1.1.5.2  1992/09/29  21:11:42  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:26:35  treff]
 * 
 * Revision 1.1.2.2  1992/06/01  19:41:57  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:50:13  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:39:35  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: genmsg.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:19:20 $";
#endif

/****************************************************************************/
/*                                                                          */
/*               TESTPROGRAM: IPC - MESSAGE GENERATOR                       */
/*               ====================================                       */
/*                                                                          */
/*    CALLING SYNTAX:                                                       */
/*    ---------------                                                       */
/*                                                                          */
/*                  genmsg <server-id> <input file> [-a] [<output file>]    */
/*                                                                          */
/*                  <server-id> =  identifier of the server to which the    */
/*                                 service requests will be directed.       */
/*                                 0 = DUA-CACHE                            */
/*                                 1 = CLIENT-STUB                          */
/*                                 2 = Short_Id_Handler                     */
/*                                 .............                            */
/*                        D23_MAXSVR = DSA No. n                            */
/*                                                                          */
/*                  <input file>    = name of the file which contains in-   */
/*                                    formations for generating IPC-messages*/
/*                                    for service requests.                 */
/*                  [-a]            = Parameter for asn1 apdu's (only for   */
/*                                    DS_ABANDON)                           */ 
/*                  [<output file>] = name of the file to which the results */
/*                                    of the service requests should be     */
/*                                    written (optional -> default device   */
/*                                    is the standard output).              */
/*                                                                          */
/*                                                                          */
/*    FORMAT OF THE INPUT FILE:                                             */
/*    -------------------------                                             */
/*                                                                          */
/*    ------------------------------------------------------------------    */
/*    for service request DS_BIND:                                          */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (10)> <directory-id>                                    */
/*    <kind of creds:                                                       */
/*     "NO_CRED"                                                            */
/*     "SIMPLE_CRED": <["Validity:" ["Time1:" <tm1>] ["Time2:" <tm2>]       */
/*                                  ["Random1:" <rd1>] ["Random2:" <rd2>]]> */
/*     "STRONG_CRED"                                                        */
/*     "EXTERNAL_CRED">                                                     */
/*    <user name (write "anonym" if the user is anonym)>                    */
/*    <user password (only if user name is not "anonym")>                   */
/*    <DSA name>                                                            */
/*    ------------------------------------------------------------------    */
/*    for service request DS_ADD_OBJECT:                                    */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (6)> <directory-id> <bind-id>                           */
/*    <storage class (512 = resident, 1024 = privileged, 2048 = normal)>    */
/*    <DN of requestor>                                                     */
/*    <DN of new object>                                                    */
/*    <no. of attributes (1 - n)>                                           */
/*    <attribute type of 1. attribute>                                      */
/*    <attribute syntax of 1. attribute>                                    */
/*    <no. of attribute values (1 - m) of 1. attribute>                     */
/*    <1. attribute value of 1. attribute>                                  */
/*     ...                                                                  */
/*    <m. attribute value of 1. attribute>                                  */
/*     ...                                                                  */
/*    <attribute type of n. attribute>                                      */
/*    <attribute syntax of n. attribute>                                    */
/*    <no. of attribute values (1 - m) of n. attribute>                     */
/*    <1. attribute value of n. attribute>                                  */
/*     ...                                                                  */
/*    <m. attribute value of n. attribute>                                  */
/*    ------------------------------------------------------------------    */
/*    for service request DS_REMOVE_OBJECT:                                 */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (7)> <directory-id> <bind-id>                           */
/*    <storage class (512 = resident, 1024 = privileged, 2048 = normal)>    */
/*    <DN of requestor>                                                     */
/*    <DN of object which should be removed>                                */
/*    ------------------------------------------------------------------    */
/*    for service request DS_READ:                                          */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (1)> <directory-id> <bind-id>                           */
/*    <storage class (512 = resident, 1024 = privileged, 2048 = normal)>    */
/*    <DN of requestor>                                                     */
/*    <DN of object which should be read>                                   */
/*    <no. of requested attributes (-1 = return distinguished name only,    */
/*                                   0 = return all available attributes,   */
/*                                  >0 = return attributes which types      */
/*                                       follows)>                          */
/*    <req. return value:                                                   */
/*     "TYP"     = return attribute types only                              */
/*     "VAL_TYP" = return attribute types and attribute value(s)>           */
/*    <attribute type of 1. attribute (only if no. of req. attributes > 0)> */
/*     ...                                                                  */
/*    <attribute type of n. attribute (only if no. of req. attributes > 0)> */
/*    ------------------------------------------------------------------    */
/*    for service request DS_LIST:                                          */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (4)> <directory-id> <bind-id>                           */
/*    <storage class (512 = resident, 1024 = privileged, 2048 = normal)>    */
/*    <DN of requestor>                                                     */
/*    <DN of object which should be listed>                                 */
/*    ------------------------------------------------------------------    */
/*    for service request DS_COMPARE:                                       */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (2)> <directory-id> <bind-id>                           */
/*    <storage class (512 = resident, 1024 = privileged, 2048 = normal)>    */
/*    <DN of requestor>                                                     */
/*    <distinguished name>                                                  */
/*    <attribute type>                                                      */
/*    <attribute syntax>                                                    */
/*    <attribute value>                                                     */
/*    ------------------------------------------------------------------    */
/*    for service request DS_SEARCH:                                        */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (5)> <directory-id> <bind-id>                           */
/*    <storage class (512 = resident, 1024 = privileged, 2048 = normal)>    */
/*    <DN of requestor>                                                     */
/*    <base object subset:                                                  */
/*     "BASE_OBJECT"   = single object                                      */
/*     "ONE_LEVEL"     = one level                                          */
/*     "WHOLE_SUBTREE" = whole subtree>                                     */
/*    <distinguished name of the base object (write "root" for root obj.)>  */
/*    <no. of requested attributes ( 0 = return distinguished name only,    */
/*                                  -1 = return all available attributes,   */
/*                                  >0 = return attributes which types      */
/*                                       follows)>                          */
/*    <req. return value:                                                   */
/*     "TYP"     = return attribute types only                              */
/*     "VAL_TYP" = return attribute types and attribute value(s)>           */
/*    <attribute type of 1. attribute (only if no. of req. attributes > 0)> */
/*     ...                                                                  */
/*    <attribute type of n. attribute (only if no. of req. attributes > 0)> */
/*    <operation mode for filters:                                          */
/*     "AND"  = and                                                         */
/*     "OR"   = or                                                          */
/*     "NOT"  = not                                                         */
/*     "ITEM" = item>                                                       */
/*    if "AND" or "OR":                                                     */
/*        <no. of filters>                                                  */
/*        <operation mode for the 1. filter ... >                           */
/*         ...                                                              */
/*        <operation mode for the n. filter ... >                           */
/*    if "NOT":                                                             */
/*        <operation mode for the filter ... >                              */
/*    if "ITEM":                                                            */
/*        <filter matching rule:                                            */
/*         "EQUAL"      = equal                                             */
/*         "APPROX"     = approximate matching                              */
/*         "SUBSTRINGS" = substrings                                        */
/*         "PRESENT"    = present>                                          */
/*        <filter syntax>                                                   */
/*        <attribute type of filter>                                        */
/*        if "EQUAL" or "APPROX":                                           */
/*            <attribute value of filter>                                   */
/*        if "SUBSTRINGS":                                                  */
/*            <no. of substrings>                                           */
/*            <position of the 1. substring:                                */
/*             "INITIAL" = initial                                          */
/*             "ANY"     = any                                              */
/*             "FINAL"   = final>                                           */
/*            <value of the 1. substring>                                   */
/*               ...                                                        */
/*            <position of the n. substring:                                */
/*             "INITIAL" = initial                                          */
/*             "ANY"     = any                                              */
/*             "FINAL"   = final>                                           */
/*            <value of the n. substring>                                   */
/*    ------------------------------------------------------------------    */
/*    for servive request DS_REPLACE_RDN:                                   */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (9)> <directory-id> <bind_id>                           */
/*    <storage class (512 = resident, 1024 = privileged, 2048 = normal)>    */
/*    <DN of requestor>                                                     */
/*    <distinguished name>                                                  */
/*    <new rdn>                                                             */
/*    ------------------------------------------------------------------    */
/*    for servive request DS_MODIFY_ENTRY:                                  */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (08)> <directory-id> <bind_id>                          */
/*    <storage class (512 = resident, 1024 = privileged, 2048 = normal)>    */
/*    <DN of requestor>                                                     */
/*    <distinguished name>                                                  */
/*    <type of modification of 1. attribute:                                */
/*          "REP_AV" = replace attribute value                              */
/*          "ADDATT" = add attribute                                        */
/*          "ADD_AV" = add attribute value                                  */
/*          "DELATT" = delete attribute                                     */
/*          "DEL_AV" = delete attribute value                               */
/*    if "REP_AV":                                                          */
/*          <attribute type>                                                */
/*          <old value>                                                     */
/*          <new value>                                                     */
/*    if "ADDATT":                                                          */
/*          <no. of attributes>                                             */
/*          <type of 1. attribute>                                          */
/*          <no. of values of 1. attribute>                                 */
/*          <1. value of 1. attribute>                                      */
/*             ...                                                          */
/*          <n. value of 1. attribute>                                      */
/*             ...                                                          */
/*          <type of m. attribute>                                          */
/*          <no. of values of m. attribute>                                 */
/*          <1. value of m. attribute>                                      */
/*             ...                                                          */
/*          <n. value of m. attribute>                                      */
/*    if "DELATT":                                                          */
/*          <no. of attributes>                                             */
/*          <type of 1. attribute>                                          */
/*             ...                                                          */
/*          <type of m. attribute>                                          */
/*    if "ADD_AV" or "DEL_AV":                                              */
/*          <type of attribute>                                             */
/*          <no. of values of the attribute>                                */
/*          <1. value of the attribute>                                     */
/*             ...                                                          */
/*          <n. value of the attribute>                                     */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*    for servive request DS_ABANDON:                                       */
/*                                                                          */
/*    <text-string (written to the output file) for this service request>   */
/*    <operation-id (3)> <directory-id> <bind_id>                           */
/*    <invoke id>                                                           */
/*    ------------------------------------------------------------------    */
/*  AUTHOR: R.Horn                                        DATE: 11.1.88     */
/****************************************************************************/

/**************	system-includes				 ****************/

#ifdef STOP_TIME
#include <sys/types.h>
#include <sys/time.h>
#endif /* STOP_TIME */
#include <stdio.h>
#include <malloc.h>
#include <locale.h>

/************** DS-external includes                     ****************/

#ifdef INO
static ino_split1() {}  /* split statement blocks (maximum 32K)       */
#endif

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <dce/asn1.h>
#include <d2aapcod.h>

/************** global functions  (import)                    ****************/
/* functions which create the message */
IMPORT  void    mk_bind_msg();
IMPORT  void    mk_add_obj_msg();
IMPORT  void    mk_rem_obj_msg();
IMPORT  void    mk_read_msg();
IMPORT  void    mk_list_msg();
IMPORT  void    mk_compare_msg();
IMPORT  void    mk_search_msg();
IMPORT  void    mk_mod_msg();
IMPORT  void    mk_repl_rdn_msg();
IMPORT  void    mk_abandon_msg();

/* functions which check the result of the message */
IMPORT  void    rs_bind_msg();
IMPORT  void    rs_add_obj_msg();
IMPORT  void    rs_rem_obj_msg();
IMPORT  void    rs_read_msg();
IMPORT  void    rs_list_msg();
IMPORT  void    rs_compare_msg();
IMPORT  void    rs_search_msg();
IMPORT  void    rs_mod_msg();
IMPORT  void    rs_repl_rdn_msg();
IMPORT  void    rs_abandon_msg();

/************** global variables  (export)                    ****************/

EXPORT  FILE       *fdin ;
EXPORT  char       *eptr ;
EXPORT  D23_wtrspb  resclnt ;
EXPORT  D2_a_type  *oid;
EXPORT  char       *str_out;

#ifdef STOP_TIME
EXPORT  struct      timeval start, stop ;
#endif /* STOP_TIME */

#ifdef MSGTEST
EXPORT  char        msg_arr[10000];
#endif
	D23_inrqpb	reqclnt ;



/* ------------------------- M A I N --------------------------------------- */

main (argc, argv)
int	argc ;
char	*argv[] ;
{
	int             len ;
	int             op ;
	char            reqmsg[80] ;
	char            address[100] ;
	D23_atclpb	attclnt ;
	D23_bindpb	bindclnt ;
	Ushort          dir_id ;
	short           i, k, serverid ;
	char            dirinfbas[10];
	char            env[100];
	char            conf_dir[120];
	Bool            asn1 = 0;
	asn1_field      asn1_id;

	setlocale(LC_ALL, "");

	if (argc > 1)
		serverid = atoi (argv[1]) ;

	/* verify calling syntax */
	if (argc < 3 || argc > 5 || serverid < D23_DCASID || serverid > D23_MAXSVR) {
		printf ("wrong calling syntax -> %s <server-id (0-%d)> <input file> /<outputfile>/\n",
						 argv[0], D23_MAXSVR) ;
		exit (1) ;
	}

	/* open test input file */
	if ((fdin = fopen (argv[2], "r")) == NULL) {
		printf ("error on fopen\n") ;
		exit (1) ;
	}
	/* open test output file */
	if (argc == 4  && strcmp(argv[3],"-a") == 0) {
		asn1 = 1;
		if (argc == 5 && freopen (argv[4], "w", stdout) == NULL) {
			 printf ("error on fopen\n") ;
			 exit (1) ;
		}
	}
	else
	if (argc == 4 && freopen (argv[3], "w", stdout) == NULL) {
		printf ("error on fopen\n") ;
		exit (1) ;
	}

	if ((oid = (D2_a_type *) malloc (sizeof(D2_a_type))) == NULL)
	{       printf ("error while allocating memory\n");
		exit (1) ;
	}

	if ((oid->d2_type = (Octet *) calloc(1, D2_OBJ_IDL_MAX)) == NULL)
	{       printf ("error while allocating memory\n");
		exit (1) ;
	}

strcpy(dirinfbas,"DIRINFBAS");
if ((strcpy(env, (Char *) getenv(dirinfbas))) == D2_NULL)
{      printf("Error: no variable %s", dirinfbas);
} else
{      sprintf(conf_dir,"%s%s", env, "/conf");
}
if ((str_out = (Char *) malloc (sizeof(D2_DNL_MAX))) == NULL)
{       printf ("error while allocating memory\n");
	exit (1) ;
}
if (d27_307_init_att(conf_dir) == D2_ERROR)
{      printf("Error: d27_307_init_att");
}

#ifndef MSGTEST
	attclnt.d23_9hdinfo.d2_version = D23_V02 ;
	attclnt.d23_9evmode = D23_SEVM(D23_SYNC_EM);
	if (d23_attclt (&attclnt) == D23_ERROR) {
		printf ("error on IPC-client attach (%d)\n", attclnt.d23_9hdinfo.d2_errvalue) ;
		exit (1) ;
	}

	bindclnt.d23_4hdinfo.d2_version = D23_V02 ;
	bindclnt.d23_4svrid = serverid ;
	if (d23_bind (&bindclnt) == D23_ERROR) {
		printf ("error on IPC-client bind (%d)\n", bindclnt.d23_4hdinfo.d2_errvalue) ;
		exit (1) ;
	}

#endif
	reqclnt.d23_6hdinfo.d2_version = D23_V02 ;
	reqclnt.d23_6usrid =
	reqclnt.d23_6assid = bindclnt.d23_4assid ;
	reqclnt.d23_6contid =
	       D23_SAS (D27_DAP_AS) | D23_SACM (D27_LOC_AC) |
	       D23_SAC(D27_DAP_AC) | D23_STS(D27_PRIV_TS);
	reqclnt.d23_6imdat = D23_LASTFG ;

	/* --- service request loop --- */

	for (;;) {
		if ((i = fscanf (fdin, "%s", reqmsg)) != 1) {
			if (i == EOF)
				break ;
			fprintf (stdout, "error on fscanf -> request message\n") ;
			fflush (stdout) ;
			exit (1) ;
		}
		if (fscanf (fdin, "%d %d", &op, &dir_id) != 2) {
			fprintf (stdout, "error on fscanf -> operation-id, directory-id\n") ;
			fflush (stdout) ;
			exit (1) ;
		}

		/* generate invoke message */
#ifdef MSGTEST
		reqclnt.d23_6rfidt = eptr = msg_arr ;
#else
		reqclnt.d23_6rfidt = eptr = bindclnt.d23_4refms ;
#endif
		reqclnt.d23_6oprid = op ;
		switch (reqclnt.d23_6oprid) {
		case D23_ABANDON:
			if (asn1 == 1) {
	     			reqclnt.d23_6contid =
	       			D23_SAS (D27_DAP_AS) | D23_SACM (D27_REM_AC) |
	       			D23_SAC(D27_DAP_AC) | D23_STS(D27_ASN1_TS) ;
			} else  {
	     			reqclnt.d23_6contid =
	       			D23_SAS (D27_DAP_AS) | D23_SACM (D27_LOC_AC) |
	       			D23_SAC(D27_DAP_AC) | D23_STS(D27_PRIV_TS);
			}
			mk_abandon_msg(dir_id) ;
			len = eptr - reqclnt.d23_6rfidt ;
			if (asn1 == 1)
			  if (d2a000_apdu_asn1(D2A_ABAN_ARG, D27_DAP_AS,
				   eptr, len, &asn1_id, eptr) == D2_ERROR)
			    { fprintf(stderr,
				"Error in converting ABAN_ARG to ASN1 \n");
			      exit(1);
			     }
			break ;
		case D23_BIND:
			mk_bind_msg(dir_id) ;
			break ;
		case D23_READ:
			mk_read_msg(dir_id) ;
			break ;
		case D23_LIST:
			mk_list_msg(dir_id) ;
			break ;
		case D23_SEARCH:
			mk_search_msg(dir_id) ;
			break ;
		case D23_COMPARE:
			mk_compare_msg(dir_id) ;
			break ;
		case D23_ADDOBJT:
			mk_add_obj_msg(dir_id) ;
			break ;
		case D23_REMOBJT:
			mk_rem_obj_msg(dir_id) ;
			break ;
		case D23_REPLRDN:
			mk_repl_rdn_msg(dir_id) ;
			break;
		case D23_MODENTR:
			mk_mod_msg(dir_id) ;
			break;
		default:
			fprintf (stdout, "error -> unknown operation-id (%d)\n", reqclnt.d23_6oprid) ;
			fflush (stdout) ;
			exit (1) ;
			break ;
		}

		if (op == D23_ABANDON && asn1 == 1) {
			memcpy(reqclnt.d23_6rfidt, asn1_id.v, asn1_id.l);
		     	reqclnt.d23_6lnidt = asn1_id.l;
		} else
		     reqclnt.d23_6lnidt = eptr - reqclnt.d23_6rfidt ;
		d22_printmsg (stdout, reqclnt.d23_6rfidt, reqclnt.d23_6lnidt) ;
#ifdef STOP_TIME
		gettimeofday (&start) ;
#endif /* STOP_TIME */
#ifndef MSGTEST
		if (d23_invoke (&reqclnt) == D23_ERROR) {
			fprintf (stdout, "error on IPC-client invoke (%d)\n", reqclnt.d23_6hdinfo.d2_errvalue) ;
			fflush (stdout) ;
			exit (1) ;
		}

		resclnt.d23_7hdinfo.d2_version = D23_V02 ;
		resclnt.d23_7assid = bindclnt.d23_4assid ;
		resclnt.d23_7wtime = D23_INFIN ;
		if (d23_waitrs (&resclnt) == D23_ERROR) {
			fprintf (stdout, "error on IPC-client wait result (%d)\n", resclnt.d23_7hdinfo.d2_errvalue) ;
			fflush (stdout) ;
			exit (1) ;
		}
#endif /* MSGTEST */
#ifdef STOP_TIME
		gettimeofday (&stop) ;
#endif /* STOP_TIME */
		fprintf (stdout,"\n%s\n", reqmsg) ;
#ifdef STOP_TIME
		if (stop.tv_usec >= start.tv_usec)
			fprintf (stdout, "Duration of service request: %ld:%03d sec\n",
				 stop.tv_sec - start.tv_sec, (stop.tv_usec - start.tv_usec)/1000L) ;
		else
			fprintf (stdout, "Duration of service request: %ld:%03d sec\n",
				 stop.tv_sec - start.tv_sec - 1L, 1000L - (start.tv_usec - stop.tv_usec)/1000L) ;
#endif /* STOP_TIME */
		d22_printmsg (stdout, resclnt.d23_7rfrdt, resclnt.d23_7lnrdt) ;
		if (resclnt.d23_7rltid == D23_RERROR) {
			fprintf (stdout, "error: code: %d class: %d value: %d\n",
				((D23_rserror *) resclnt.d23_7rfrdt)->d23_Zretcod,
				((D23_rserror *) resclnt.d23_7rfrdt)->d23_Zerrcls,
				((D23_rserror *) resclnt.d23_7rfrdt)->d23_Zerrval) ;
			fflush (stdout) ;
			continue ;
		}
		eptr = resclnt.d23_7rfrdt ;
				
		switch ((int)reqclnt.d23_6oprid) {
		case D23_ABANDON:
			rs_abandon_msg();
			break;
		case D23_REPLRDN:
			rs_repl_rdn_msg();
			break;
		case D23_MODENTR:
			rs_mod_msg();
			break;
		case D23_REMOBJT:
			rs_rem_obj_msg();
			break ;
		case D23_COMPARE:
			rs_compare_msg();
			break ;
		case D23_ADDOBJT:
			rs_add_obj_msg();
			break ;
		case D23_BIND:
			rs_bind_msg();
			break ;
		case D23_READ:
			rs_read_msg();
			break ;
		case D23_LIST:
			rs_list_msg();
			break ;
		case D23_SEARCH:
			rs_search_msg();
			break ;
		default:
			fprintf (stdout, "error -> unknown operation-id (%d)\n", reqclnt.d23_6oprid) ;
			fflush (stdout) ;
			exit (1) ;
			break ;
		}
		fflush (stdout) ;
	}
}
