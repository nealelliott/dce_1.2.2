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
 * $Log: ndscomp.l,v $
 * Revision 1.1.4.2  1996/03/09  23:14:20  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:31:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:05:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:07  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:39  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:46  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:03  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:30:11  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:42:05  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:25:32  keutel
 * 	creation
 * 	[1994/03/21  15:53:32  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndscomp.l,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:20 $";
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
/*   NAME:    ndscomp.l                                                      */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module represents the lexical analyzer part of the NDS-        */
/*       compiler.                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <y.tab.h>
#include <ndscfg.h>

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

signed32	infostate, valstate, evaluating ;
extern signed32	svcprolog ;

/* ******************* PROTOTYPING OF LOCAL FUNCTIONS ********************** */

static void nds_conv_impdspval (char *, char *) ;

%}

%o 40000
%n 3000
%a 15000
%p 50000
%e 25000
%k 120000

ws		[ \t]+
sp		[ ]+
nl		\n
comment		[ \t]*#[^\n]*
assign		=
delimiter	;
any		.
digit		[0-9]
hexdigit	[0-9a-fA-F]
hexvalue	({hexdigit}{hexdigit})
dotvalue	(([0-9]{1,2})|(1([0-9]{2}))|(2[0-4][0-9]|25[0-5]))

title		CONFIGURATION_FILE_TITLE
titleval	\"[^\"\n]+\"

begin		BEGIN
end		END
section		SECTION
interface	INTERFACE
interfaces	INTERFACES

ifname1		socket
ifname2		xti
ifename		{ifname1}|{ifname2}

profile		PROFILE
profval1	RFC1006
profval2	OSILAN
profval3	OSIWAN

impdspval	IMPLICIT_DSP{sp}([1-9]|10)(:([1-9]|10))?
impidival	IMPLICIT_IDI

profprio	PROFILE_PRIORITY
profprioval	[1-9]|1[0-6]

profdevice	PROFILE_DEVICE
profdevval	[^ \t\n;]+

x121intprefix	X121_INTERNATIONAL_PREFIX
x121ipval	{digit}|-
x121dcc		X121_DATA_COUNTRY_CODE
x121dccval	({digit}{3})|-
x121dnic	X121_DATA_NETWORK_ID_CODE
x121dnicval	({digit}{4})|-
x121number	X121_NUMBER
x121numberval	{digit}{1,15}

e163intprefix	E163_INTERNATIONAL_PREFIX
e163ipval	({digit}{2})|-
e163natprefix	E163_NATIONAL_PREFIX
e163npval	({digit}{1,2})|-
e163cc		E163_COUNTRY_CODE
e163ccval	({digit}{1,3})|-
e163areacode	E163_AREA_CODE
e163acval	({digit}{2,4})|-
e163number	E163_NUMBER
e163numberval	{digit}{1,14}

e164intprefix	E164_INTERNATIONAL_PREFIX
e164ipval	({digit}{2})|-
e164cc		E164_COUNTRY_CODE
e164ccval	({digit}{1,3})|-
e164ndstcode	E164_NATIONAL_DESTINATION_CODE
e164ndstcval	({digit}{1,15})|-
e164number	E164_NUMBER
e164numberval	{digit}{1,17}

pvcnumber	PVC_CHANNEL
pvcnoval	[1-9]([0-9]{0,2})|[1-3]([0-9]{3})|40[0-8][0-9]|409[0-6]

dialnumber	X21DIAL_NUMBER
dialnoval	{digit}{1,17}

naddress	NSAP_ADDRESS
naddrval	([^ \t\n;]{4,80})
ngrpaddress	NSAP_ADDRESS_GROUP
ngrpaddrval	([0-9A-F|\\{}()[\]\|,*+-]{4,80})

tpclass		TRANSPORT_PROTOCOL_CLASS
tpclassval	0\/0|0\/-|2\/0|2\/2

tpid		TRANSPORT_PROTOCOL_ID
tpidval		x'({hexvalue}{1,16})'

portno		PORT
portnoval	{digit}{1,5}
tselector	T_SELECTOR
tselvalascii	([^ \t\n;]{1,32})
tselvalhex	x'({hexvalue}{1,32})'

macaddr		ETHERNET_ADDRESS
macaddrval	x'({hexvalue}{6})'
snid		SUBNET_ID
snidval		{digit}{1,4}

internetaddr	INTERNET_ADDRESS
internetval	({dotvalue}\.{3}){dotvalue}

beginifsection	{begin}{sp}{section}{sp}{interfaces}
endifsection	{end}{sp}{section}{sp}{interfaces}
beginifdesc	{begin}{sp}{interface}{sp}
endifdesc	{end}{sp}{interface}{sp}
beginprofdesc	{begin}{sp}{profile}{sp}
endprofdesc	{end}{sp}{profile}{sp}

%start	CONFTITLEVAL IFINFO IFNAME IFENAME NONE ASSIGN DELIM
%start	PROFINFO PROFSNAME PROFENAME PROFPRIOVAL PROFDEVVAL
%start	X121IPVAL X121DCCVAL X121DNICVAL X121NOVAL
%start  E163IPVAL E163CCVAL E163NPVAL E163ACVAL E163NOVAL
%start  E164IPVAL E164CCVAL E164NDCVAL E164NOVAL
%start  PVCNOVAL X21NOVAL
%start	INTERNETVAL PORTVAL TSELVAL
%start	TPCLASSVAL TPIDVAL
%start  MACADDRVAL SNIDVAL NETSADDRVAL NETGADDRVAL

%%

{title} {
	BEGIN ASSIGN ;
	valstate = CONFTITLEVAL ;
	evaluating = 1 ;
}

<CONFTITLEVAL>{titleval} {
	BEGIN DELIM ;
	infostate = IFINFO ;
	yylval.string = (char *) &yytext[1] ;
	yytext[yyleng - 1] = '\0' ;
	return RETDBG(CONFTITLE) ;
}
{beginifsection} {
	yylval.string = (char *) yytext ;
	return RETDBG(IFS_BEGIN) ;
}

{endifsection} {
	BEGIN NONE ;
	yylval.string = (char *) yytext ;
	return RETDBG(IFS_END) ;
}

{beginifdesc} {
	BEGIN IFNAME ;
}

<IFNAME>{ifname1} {
	BEGIN IFINFO;
	infostate = IFINFO ;
	yylval.string = (char *) yytext ;
	return RETDBG(IF_SOCKET) ;
}

<IFNAME>{ifname2} {
	BEGIN IFINFO;
	infostate = IFINFO ;
	yylval.string = (char *) yytext ;
	return RETDBG(IF_XTI) ;
}

{endifdesc} {
	BEGIN IFENAME ;
}

<IFENAME>{ifename} {
	BEGIN NONE;
	infostate = NONE ;
	yylval.string = (char *) yytext ;
	return RETDBG(IF_ENAME) ;
}

{beginprofdesc}	{
	BEGIN PROFSNAME ;
	infostate = PROFINFO ;
}

{endprofdesc} {
	BEGIN PROFENAME ;
	infostate = NONE ;
}

<PROFSNAME,PROFENAME>{profval1}	{
	BEGIN infostate ;
	yylval.integer = NDS_RFC1006_PROF ;
	return RETDBG(RFC1006) ;
}

<PROFSNAME,PROFENAME>{profval2}	{
	BEGIN infostate ;
	yylval.integer = NDS_OSILAN_PROF ;
	return RETDBG(OSILAN) ;
}

<PROFSNAME,PROFENAME>{profval3}	{
	BEGIN infostate ;
	yylval.integer = NDS_OSIWAN_PROF ;
	return RETDBG(OSIWAN) ;
}

<PROFINFO>{profprio} {
	BEGIN ASSIGN ;
	valstate = PROFPRIOVAL;
}

<PROFPRIOVAL>{profprioval} {
	BEGIN DELIM ;
	yylval.integer = atoi ((const char *) yytext) ;
	return RETDBG(NDS_PRIO_TOK) ;
}

<IFINFO,PROFINFO>{profdevice} {
	BEGIN ASSIGN ;
	valstate = PROFDEVVAL;
}

<PROFDEVVAL>{profdevval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_DEVNM_TOK) ;
}

<IFINFO,PROFINFO>{x121intprefix} {
	BEGIN ASSIGN ;
	valstate = X121IPVAL ;
}

<X121IPVAL>{x121ipval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_X121_IP_TOK) ;
}

<IFINFO,PROFINFO>{x121dcc} {
	BEGIN ASSIGN ;
	valstate = X121DCCVAL ;
}

<X121DCCVAL>{x121dccval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_X121_DCC_TOK) ;
}

<IFINFO,PROFINFO>{x121dnic} {
	BEGIN ASSIGN ;
	valstate = X121DNICVAL ;
}

<X121DNICVAL>{x121dnicval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_X121_DNIC_TOK) ;
}

<PROFINFO>{x121number} {
	BEGIN ASSIGN ;
	valstate = X121NOVAL ;
}

<X121NOVAL>{x121numberval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_X121_TOK) ;
}

<X121NOVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_X121_ITOK) ;
}

<X121NOVAL>{impidival} {
	BEGIN DELIM ;
	yylval.string = "0" ;
	return RETDBG(NDS_X121_ITOK) ;
}

<IFINFO,PROFINFO>{e163intprefix} {
	BEGIN ASSIGN ;
	valstate = E163IPVAL ;
}

<E163IPVAL>{e163ipval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E163_IP_TOK) ;
}

<IFINFO,PROFINFO>{e163cc} {
	BEGIN ASSIGN ;
	valstate = E163CCVAL ;
}

<E163CCVAL>{e163ccval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E163_CC_TOK) ;
}

<IFINFO,PROFINFO>{e163natprefix} {
	BEGIN ASSIGN ;
	valstate = E163NPVAL ;
}

<E163NPVAL>{e163npval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E163_NP_TOK) ;
}

<IFINFO,PROFINFO>{e163areacode} {
	BEGIN ASSIGN ;
	valstate = E163ACVAL ;
}

<E163ACVAL>{e163acval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E163_AC_TOK) ;
}

<PROFINFO>{e163number} {
	BEGIN ASSIGN ;
	valstate = E163NOVAL ;
}

<E163NOVAL>{e163numberval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E163_TOK) ;
}

<E163NOVAL>{impidival} {
	BEGIN DELIM ;
	yylval.string = "0" ;
	return RETDBG(NDS_E163_ITOK) ;
}

<E163NOVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E163_ITOK) ;
}

<IFINFO,PROFINFO>{e164intprefix} {
	BEGIN ASSIGN ;
	valstate = E164IPVAL ;
}

<E164IPVAL>{e164ipval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E164_IP_TOK) ;
}

<IFINFO,PROFINFO>{e164cc} {
	BEGIN ASSIGN ;
	valstate = E164CCVAL ;
}

<E164CCVAL>{e164ccval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E164_CC_TOK) ;
}

<IFINFO,PROFINFO>{e164ndstcode} {
	BEGIN ASSIGN ;
	valstate = E164NDCVAL ;
}

<E164NDCVAL>{e164ndstcval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E164_NDC_TOK) ;
}

<PROFINFO>{e164number} {
	BEGIN ASSIGN ;
	valstate = E164NOVAL ;
}

<E164NOVAL>{e164numberval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E164_TOK) ;
}

<E164NOVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_E164_ITOK) ;
}

<E164NOVAL>{impidival} {
	BEGIN DELIM ;
	yylval.string = "0" ;
	return RETDBG(NDS_E164_ITOK) ;
}

<IFINFO,PROFINFO>{naddress} {
	BEGIN ASSIGN ;
	valstate = NETSADDRVAL ;
}

<IFINFO,PROFINFO>{ngrpaddress} {
	BEGIN ASSIGN ;
	valstate = NETGADDRVAL ;
}

<NETSADDRVAL>{naddrval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_NADDR_TOK) ;
}

<NETGADDRVAL>{ngrpaddrval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_NADDR_GRP_TOK) ;
}

<PROFINFO>{tpclass} {
	BEGIN ASSIGN ;
	valstate = TPCLASSVAL ;
}

<TPCLASSVAL>{tpclassval} {
	signed32	pref_tpc, alt_tpc = 0 ;

	BEGIN DELIM ;

	sscanf ((char *) yytext, "%d%*c%d", &pref_tpc, &alt_tpc) ;
	sprintf ((char *) yytext, "%d", (pref_tpc << 4) | alt_tpc) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_TPC_TOK) ;
}

<PROFINFO>{tpid} {
	BEGIN ASSIGN ;
	valstate = TPIDVAL ;
}

<TPIDVAL>{tpidval} {
	BEGIN DELIM ;
	yylval.string = (char *) &yytext[2] ;
	yytext[yyleng - 1] = '\0' ;
	return RETDBG(NDS_TPI_TOK) ;
}

<TPIDVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_TPI_ITOK) ;
}

<PROFINFO>{portno} {
	BEGIN ASSIGN ;
	valstate = PORTVAL ;
}

<PORTVAL>{portnoval} {
	BEGIN DELIM ;
	if (atoi ((char *) yytext) > 32767) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_LEX_ERR2,
			atoi ((char *) yytext), 32767, nds_line()) ;
	    exit (1) ;
	}
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_PORT_TOK) ;
}

<PORTVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_PORT_ITOK) ;
}

<PROFINFO>{tselector} {
	BEGIN ASSIGN ;
	valstate = TSELVAL ;
}

<TSELVAL>{tselvalascii} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_TSELA_TOK) ;
}

<TSELVAL>{tselvalhex} {
	BEGIN DELIM ;
	yylval.string = (char *) &yytext[2] ;
	yytext[yyleng - 1] = '\0' ;
	return RETDBG(NDS_TSELH_TOK) ;
}

<PROFINFO>{macaddr} {
	BEGIN ASSIGN ;
	valstate = MACADDRVAL ;
}

<MACADDRVAL>{macaddrval} {
	BEGIN DELIM ;
	yylval.string = (char *) &yytext[2] ;
	yytext[yyleng - 1] = '\0' ;
	return RETDBG(NDS_MAC_TOK) ;
}

<MACADDRVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_MAC_ITOK) ;
}

<PROFINFO>{snid} {
	BEGIN ASSIGN ;
	valstate = SNIDVAL ;
}

<SNIDVAL>{snidval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_SNID_TOK) ;
}

<SNIDVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_SNID_ITOK) ;
}

<PROFINFO>{internetaddr} {
	 BEGIN ASSIGN ;
	valstate = INTERNETVAL ;
}

<INTERNETVAL>{internetval} {
	signed32 ipval[4] ;

	BEGIN DELIM ;
	/* convert IP-address from dotted notion into internal format */
	sscanf ((char *) yytext, "%d%*c%d%*c%d%*c%d", ipval, &ipval[1],
							 &ipval[2], &ipval[3]) ;
	sprintf ((char *) yytext, "%02x%02x%02x%02x", ipval[0], ipval[1],
							   ipval[2], ipval[3]) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_IP_TOK) ;
}

<INTERNETVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_IP_ITOK) ;
}

<PROFINFO>{pvcnumber} {
	BEGIN ASSIGN ;
	valstate = PVCNOVAL ;
}

<PVCNOVAL>{pvcnoval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_PVC_TOK) ;
}

<PVCNOVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_PVC_ITOK) ;
}

<PROFINFO>{dialnumber} {
	BEGIN ASSIGN ;
	valstate = X21NOVAL ;
}

<X21NOVAL>{dialnoval} {
	BEGIN DELIM ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_X21_TOK) ;
}

<X21NOVAL>{impdspval} {
	BEGIN DELIM ;
	nds_conv_impdspval ((char *) yytext, (char *) yytext) ;
	yylval.string = (char *) yytext ;
	return RETDBG(NDS_X21_ITOK) ;
}

<NONE,IFINFO,PROFINFO>{comment} {
	/* comment line detected */
}

^{comment} {
	if (evaluating) {
	    REJECT ;
	}
	/* comment line detected */
}

{ws}				;

{sp}				;

<ASSIGN>{assign} {
	BEGIN valstate ;
}

<DELIM>{delimiter} {
	BEGIN infostate ;
}

{nl}	;

{any} { 
    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
		svc_c_sev_fatal | svcprolog, GDS_S_NDS_LEX_ERR, nds_line()) ;
    exit (1) ;
}
%%
/* ------------------- convert DSP-part value ------------------------------ */

static void nds_conv_impdspval (char *istr, char *ostr) {
    signed32	dspno1, dspno2 = 0 ;
    char	*hlpref ;

    hlpref = (char *) strrchr ((const char *) istr, (int) ' ') ;
    dspno1 = atoi (hlpref + 1) ;
    if ((hlpref = (char *) strrchr ((const char *) istr, (int) ':')) !=
								(char *) NULL) {
	/* second DSP-part specified */
	dspno2 = atoi (hlpref + 1) ;
	if (dspno2 == dspno1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
		  svc_c_sev_fatal | svcprolog, GDS_S_NDS_LEX_ERR1, nds_line()) ;
	    exit (1) ;
	}
    }
    sprintf (ostr, "%d", (dspno2 << 4) | dspno1) ;
}

/* ------------------------------------------------------------------------- */

#ifdef DEBUGLEX
signed32 nds_lexdebug (signed32 val, signed32 line) {

    dce_fprintf (stdout, GDS_S_NDS_LEXTOKEN, val, line) ;
    return (val) ;
}
#endif
