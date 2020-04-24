%{
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndscomp.y,v $
 * Revision 1.1.4.2  1996/03/09  23:14:33  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:31:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:06:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:28  root]
 * 
 * Revision 1.1.2.4  1994/06/21  14:47:41  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:42:55  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:56  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:03  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:16  marrek
 * 	Bug fixes.
 * 	[1994/04/29  12:36:41  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:42:46  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:26:08  keutel
 * 	creation
 * 	[1994/03/21  15:54:08  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndscomp.y,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:33 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    ndscomp.y                                                      */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module represents the parser part of the NDS-compiler.         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <ndscfg.h>

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

extern int	yylineno ;
extern signed32	svcprolog ;

%}
%union {
    int		integer ;
    char	*string ;
    }

%token <string>  CONFTITLE IFS_BEGIN IFS_END IF_SOCKET IF_XTI IF_CMXS IF_ENAME
%token <integer> NDS_PRIO_TOK
%token <string>  NDS_DEVNM_TOK
%token <string>  NDS_X121_IP_TOK NDS_X121_DCC_TOK NDS_X121_DNIC_TOK NDS_X121_TOK
%token <string>	 NDS_X121_ITOK
%token <string>  NDS_E163_IP_TOK NDS_E163_CC_TOK NDS_E163_NP_TOK NDS_E163_AC_TOK
%token <string>  NDS_E163_TOK NDS_E163_ITOK
%token <string>  NDS_E164_IP_TOK NDS_E164_CC_TOK NDS_E164_NDC_TOK NDS_E164_TOK
%token <string>  NDS_E164_ITOK
%token <string>  NDS_X21_TOK NDS_X21_ITOK NDS_PVC_TOK NDS_PVC_ITOK
%token <string>  NDS_NADDR_TOK NDS_NADDR_GRP_TOK
%token <string>  NDS_MAC_TOK NDS_MAC_ITOK NDS_SNID_TOK
%token <string>  NDS_SNID_ITOK
%token <string>	 NDS_IP_TOK NDS_IP_ITOK NDS_PORT_TOK NDS_PORT_ITOK NDS_TSELA_TOK
%token <string>  NDS_TSELH_TOK
%token <string>  NDS_PROC_TOK NDS_PROC_ITOK NDS_REG_TOK NDS_REG_ITOK
%token <string>  NDS_HOST_TOK NDS_HOST_ITOK NDS_HOST_HTOK NDS_UNI_GRP_TOK
%token <string>  NDS_UNI_GRP_ITOK
%token <string>  NDS_LANCC_TOK NDS_WANCC_TOK NDS_TPC_TOK NDS_TPI_TOK NDS_TPI_ITOK
%token <integer> RFC1006 LANSBKA OSITYPE WANNEA WANSBKA WAN3SBKA LOOPSBKA
%token <integer> NETBIOS

%%

ndsinfo:	  CONFTITLE
			{ nds_store_title ($1) ; }
		  IFS_BEGIN
			{ nds_start_ifsection () ; }
		  socketifinfo xtiifinfo cmxsifinfo
		  IFS_END
			{ nds_end_ifsection () ; }
		;

socketifinfo:	  /* empty */
		| IF_SOCKET
			{ nds_start_if (IF_SOCKET, $1) ; }
		  rfc1006profile
		  IF_ENAME
			{ nds_end_if ($4) ; }
		;

xtiifinfo:	  /* empty */
		| IF_XTI
			{ nds_start_if (IF_XTI, $1) ; }
		  comprofileinfo
		  loopsbkaprofile
		  rfc1006profile1
		  ositypeprofile
		  lansbkaprofile
		  wansbkaprofile
		  wan3sbkaprofile
		  wanneaprofile
		  netbiosprofile
		  IF_ENAME
			{ nds_end_if ($12) ; }
		;

cmxsifinfo:	  /* empty */
		| IF_CMXS
			{ nds_start_if (IF_CMXS, $1) ; }
		  comprofileinfo
		  loopsbkaprofile
		  rfc1006profile1
		  ositypeprofile
		  lansbkaprofile
		  wansbkaprofile
		  wan3sbkaprofile
		  wanneaprofile
		  IF_ENAME
			{ nds_end_if ($11) ; }
		;

comprofileinfo:	  profdevinfo X121info E163info E164info
		;

rfc1006profile:	  /* empty */
		| RFC1006
			{ nds_start_prof ($1) ; }
		  profprio tcpaddrinfo
		  RFC1006
			{ nds_end_prof ($5) ; }
		;

loopsbkaprofile:  /* empty */
		| LOOPSBKA
			{ nds_start_prof ($1) ; }
		  profprio profdevinfo loopaddrinfo
		  LOOPSBKA
			{ nds_end_prof ($6) ; }
		;

rfc1006profile1:  /* empty */
		| RFC1006
			{ nds_start_prof ($1) ; }
		  profprio profdevinfo tcpaddrinfo
		  RFC1006
			{ nds_end_prof ($6) ; }
		;

ositypeprofile:	  /* empty */
		| OSITYPE
			{ nds_start_prof ($1) ; }
		  profprio profdevinfo osiaddrinfo
		  OSITYPE
			{ nds_end_prof ($6) ; }
		;

lansbkaprofile:	  /* empty */
		| LANSBKA
			{ nds_start_prof ($1) ; }
		  profprio profdevinfo lanaddrinfo
		  LANSBKA
			{ nds_end_prof ($6) ; }
		;

wansbkaprofile:	  /* empty */
		| WANSBKA
			{ nds_start_prof ($1) ; }
		  profprio profdevinfo wanaddrinfo
		  WANSBKA
			{ nds_end_prof ($6) ; }
		;

wan3sbkaprofile:  /* empty */
		| WAN3SBKA
			{ nds_start_prof ($1) ; }
		  profprio profdevinfo wan3addrinfo
		  WAN3SBKA
			{ nds_end_prof ($6) ; }
		;

wanneaprofile:	  /* empty */
		| WANNEA
			{ nds_start_prof ($1) ; }
		  profprio profdevinfo neaaddrinfo
		  WANNEA
			{ nds_end_prof ($6) ; }
		;

netbiosprofile:	  /* empty */
		| NETBIOS
			{ nds_start_prof ($1) ; }
		  profprio profdevinfo netbiosaddrinfo
		  NETBIOS
			{ nds_end_prof ($6) ; }
		;

tcpaddrinfo:	  netaddr ipinfo portinfo
		| netaddr ipinfo portinfo tcpaddrinfo
		;

loopaddrinfo:	  netaddr
		| netaddr loopaddrinfo
		;

osiaddrinfo:	  netaddr macinfo lanccinfo
		| netaddr macinfo lanccinfo osiaddrinfo
		;

lanaddrinfo:	  netaddr macaddr snidinfo lanccinfo
		| netaddr macaddr snidinfo lanccinfo lanaddrinfo
		;

wanaddrinfo:	  netaddr wansnpainfo wanccinfo tpiinfo tpcinfo
		| netaddr wansnpainfo wanccinfo tpiinfo tpcinfo wanaddrinfo
		;

wan3addrinfo:	  netaddr wan3snpainfo wanccinfo
		| netaddr wan3snpainfo wanccinfo wan3addrinfo
		;

neaaddrinfo:	  netaddr processor region neasnpainfo wanccinfo
		| netaddr processor region neasnpainfo wanccinfo neaaddrinfo
		;

netbiosaddrinfo:  netaddr netbiosinfo
		| netaddr netbiosinfo netbiosaddrinfo
		;

wansnpainfo:	  x121number
		| e163number
		| e164number
		| X31info
		| X32info
		| PVCinfo
		;

wan3snpainfo:	  x121number
		| e164number
		| PVCinfo
		;

neasnpainfo:	  x121number
		| e163number
		| e164number
		| X21info
		| X31info
		| X32info
		| PVCinfo
		;

x121number:	  x121impnumber X121info
		| x121expnumber
		;

e163number:	  e163impnumber E163info
		| e163expnumber
		;

e164number:	  e164impnumber E164info
		| e164expnumber
		;

X31info:	| e164number x121number
		;

X32info:	  X21info x121number
		;

X121info:	  /* empty */
		| x121intprefix x121dcc
		| x121intprefix x121dnic
		;

E163info:	  /* empty */
		|  e163intprefix e163cc e163natprefix e163areacode
		;

E164info:	  /* empty */
		| e164intprefix e164cc e164ndcinfo
		;

netbiosinfo:	  nbname nbunigroupid
		;

portinfo:	  /* empty */
		  impport
		| expport tsel
		| expport tsel portinfo
		;

lanccinfo:	  /* empty */
		| lancc
		;

wanccinfo:	  /* empty */
		| wancc wanccinfo
		;

tpcinfo:	  /* empty */
		| tpclass
		;

tpiinfo:	  /* empty */
		| tpid
		;

macinfo:	  /* empty */
		| macaddr
		;

snidinfo:	  /* empty */
		| snid
		;

e164ndcinfo:	  /* empty */
		| e164ndc
		;

profdevinfo:	  /* empty */
		| profdev
		;

/* ------------------------------------------------------------------------- */

		/* profile priority */
profprio:	  NDS_PRIO_TOK
		  { nds_save_cominfo (NDS_PRIO_TOK, $1, (char *) NULL) ; }
		;

		/* profile device name */
profdev:	  NDS_DEVNM_TOK
		  { nds_save_cominfo (NDS_DEVNM_TOK, 0, $1) ; }
		;

		/* internet-address */
ipinfo:		  NDS_IP_ITOK
		  { nds_save_option (NDS_IP_OPT, NDS_IMP_OCTSTR, $1) ; }
		| NDS_IP_TOK
		  { nds_save_option (NDS_IP_OPT, NDS_EXP_OCTSTR, $1) ; }
		;

		/* TRANSDATA processor number */
processor:	  NDS_PROC_ITOK
		  { nds_save_option (NDS_PROC_OPT, NDS_IMP_INT, $1) ; }
		| NDS_PROC_TOK
		  { nds_save_option (NDS_PROC_OPT, NDS_EXP_SHORT, $1) ; }
		;

		/* TRANSDATA region number */
region:		  NDS_REG_ITOK
		  { nds_save_option (NDS_REG_OPT, NDS_IMP_INT, $1) ; }
		| NDS_REG_TOK
		  { nds_save_option (NDS_REG_OPT, NDS_EXP_SHORT, $1) ; }
		;

		/* X.121 number */
x121impnumber:	  NDS_X121_ITOK
		  { nds_save_option (NDS_X121_OPT, NDS_IMP_DIGSTR, $1) ; }
		;

x121expnumber:	  NDS_X121_TOK
		  { nds_save_option (NDS_X121_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* X.121 international prefix */
x121intprefix:	  NDS_X121_IP_TOK
		  { nds_save_option (NDS_X121_IP_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* X.121 data country code */
x121dcc:	  NDS_X121_DCC_TOK
		  { nds_save_option (NDS_X121_DCC_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* X.121 data network identification code */
x121dnic:	  NDS_X121_DNIC_TOK
		  { nds_save_option (NDS_X121_DNIC_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.163 number */
e163impnumber:	  NDS_E163_ITOK
		  { nds_save_option (NDS_E163_OPT, NDS_IMP_DIGSTR, $1) ; }
		;

e163expnumber:    NDS_E163_TOK
		  { nds_save_option (NDS_E163_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.163 international prefix */
e163intprefix:	  NDS_E163_IP_TOK
		  { nds_save_option (NDS_E163_IP_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.163 country code */
e163cc:		  NDS_E163_CC_TOK
		  { nds_save_option (NDS_E163_CC_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.163 national prefix */
e163natprefix:	  NDS_E163_NP_TOK
		  { nds_save_option (NDS_E163_NP_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.163 area code */
e163areacode:	  NDS_E163_AC_TOK
		  { nds_save_option (NDS_E163_AC_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.164 number */
e164impnumber:	  NDS_E164_ITOK
		  { nds_save_option (NDS_E164_OPT, NDS_IMP_DIGSTR, $1) ; }
		;

e164expnumber:    NDS_E164_TOK
		  { nds_save_option (NDS_E164_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.164 international prefix */
e164intprefix:	  NDS_E164_IP_TOK
		  { nds_save_option (NDS_E164_IP_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.164 country code */
e164cc:		  NDS_E164_CC_TOK
		  { nds_save_option (NDS_E164_CC_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* E.164 national destination code */
e164ndc:	  NDS_E164_NDC_TOK
		  { nds_save_option (NDS_E164_NDC_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* X.21 dialing number */
X21info:	  NDS_X21_ITOK
		  { nds_save_option (NDS_X21_OPT, NDS_IMP_DIGSTR, $1) ; }
		| NDS_X21_TOK
		  { nds_save_option (NDS_X21_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* ethernet-address */
macaddr:	  NDS_MAC_ITOK
		  { nds_save_option (NDS_MAC_OPT, NDS_IMP_OCTSTR, $1) ; }
		| NDS_MAC_TOK
		  { nds_save_option (NDS_MAC_OPT, NDS_EXP_OCTSTR, $1) ; }
		;

		/* subnet-ID */
snid:		  NDS_SNID_ITOK
		  { nds_save_option (NDS_SNID_OPT, NDS_IMP_INT, $1) ; }
		| NDS_SNID_TOK
		  { nds_save_option (NDS_SNID_OPT, NDS_EXP_SHORT, $1) ; }
		;

		/* PVC number */
PVCinfo:	  NDS_PVC_ITOK
		  { nds_save_option (NDS_PVC_OPT, NDS_IMP_INT, $1) ; }
		| NDS_PVC_TOK
		  { nds_save_option (NDS_PVC_OPT, NDS_EXP_SHORT, $1) ; }
		;

		/* net address (NSAP-address) */
netaddr:	  NDS_NADDR_GRP_TOK
		  { nds_save_netaddr (NDS_GROUP_ADDR, $1) ; }
		| NDS_NADDR_TOK
		  { nds_save_netaddr (NDS_SINGLE_ADDR, $1) ; }
		;

		/* LAN CC-specification */
lancc:		  NDS_LANCC_TOK
		  { nds_save_option (NDS_LANCC_OPT, NDS_EXP_OCTSTR, $1) ; }
		;

		/* WAN CC-specification */
wancc:		  NDS_WANCC_TOK
		  { nds_save_option (NDS_WANCC_OPT, NDS_REC_OCTSTR, $1) ; }
		;

		/* transport protocol class */
tpclass:	  NDS_TPC_TOK
		  { nds_save_option (NDS_TPC_OPT, NDS_EXP_SHORT, $1) ; }
		;

		/* transport protocol identifier */
tpid:		  NDS_TPI_ITOK
		  { nds_save_option (NDS_TPI_OPT, NDS_IMP_OCTSTR, $1) ; }
		| NDS_TPI_TOK
		  { nds_save_option (NDS_TPI_OPT, NDS_EXP_OCTSTR, $1) ; }
		;

		/* port number */
impport:	  NDS_PORT_ITOK
		  { nds_save_option (NDS_PORT_OPT, NDS_IMP_INT, $1) ; }
		;

expport:	  NDS_PORT_TOK
		  { nds_save_option (NDS_PORT_OPT, NDS_REC_SHORT, $1) ; }
		;

		/* transport selector */
tsel:		  NDS_TSELA_TOK
		  { nds_save_option (NDS_PORT_OPT, NDS_REC_ASCSTR, $1) ; }
		| NDS_TSELH_TOK
		  { nds_save_option (NDS_PORT_OPT, NDS_REC_OCTSTR, $1) ; }
		;

		/* NETBIOS host name */
nbname:		  NDS_HOST_ITOK
		  { nds_save_option (NDS_HOST_OPT, NDS_IMP_OCTSTR, $1) ; }
		| NDS_HOST_HTOK
		  { nds_save_option (NDS_HOST_OPT, NDS_EXP_OCTSTR, $1) ; }
		| NDS_HOST_TOK
		  { nds_save_option (NDS_HOST_OPT, NDS_EXP_ASCSTR, $1) ; }
		;

		/* NETBIOS unique/group ID */
nbunigroupid:	  NDS_UNI_GRP_ITOK
		  { nds_save_option (NDS_UNI_GRP_OPT, NDS_IMP_INT, $1) ; }
		| NDS_UNI_GRP_TOK
		  { nds_save_option (NDS_UNI_GRP_OPT, NDS_EXP_SHORT, $1) ; }
		;
%%

/* --------------- evaluate input line number information ------------------- */

char *nds_line (void) {
    static char	lineinfo[128] ;
    char	*lptr ;

    if (yylineno > 0 && (lptr = (char *) dce_sprintf (GDS_S_NDS_LINEINFO,
						  yylineno)) != (char *) NULL) {
	strcpy (lineinfo, lptr) ;
	free (lptr) ;
    } else {
	lineinfo[0] = '\0' ;
    }
    return (lineinfo) ;
}

/* ------------ print parser (yacc) error information ----------------------- */

yyerror (s)
char	*s ;
{
    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%s"), GDS_S_CMX,
	       svc_c_sev_fatal | svcprolog, GDS_S_NDS_YACC_ERR, s, nds_line()) ;
    exit (1) ;
}
