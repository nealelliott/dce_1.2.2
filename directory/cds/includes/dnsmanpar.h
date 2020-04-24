/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsmanpar.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:30  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:04  root]
 * 
 * Revision 1.1.4.7  1994/08/19  21:10:35  zee
 * 	Add repair timestamp commands.
 * 	[1994/08/19  18:06:51  zee]
 * 
 * Revision 1.1.4.6  1994/08/03  19:02:18  mccann
 * 	includes cleanup
 * 	[1994/08/02  14:00:49  mccann]
 * 
 * Revision 1.1.4.5  1994/08/01  15:07:29  zee
 * 	     Add force background and checkpoint commands.
 * 	[1994/07/29  21:03:13  zee]
 * 
 * Revision 1.1.4.4  1994/06/09  18:39:01  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:40  devsrc]
 * 
 * Revision 1.1.4.3  1994/05/06  16:04:11  zee
 * 	     HCell BL4 support: Add code for cell inherit command (not documented).
 * 	[1994/05/05  21:06:46  zee]
 * 
 * Revision 1.1.4.2  1994/03/22  20:00:06  griffin
 * 	HCell BL3 support: Add codes for add/remove cellname.
 * 	[1994/03/22  18:25:25  griffin]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:01  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:14:53  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:30:28  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:48:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNSMANPAR_H
#define _DNSMANPAR_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */

#define FUNC_EXIT	1		/* Exit */

/* hierarchical cell commands */
#define ADDCELLPREFERCMD   81
#define ADDCELLALIASCMD    82
#define ADDCELLCHILDCMD    83
#define REMCELLCMD         84
#define ADDCELLINHERITCMD  85   /* Undocumented option */

/* force commands */
#define FORCEBACKCMD       86
#define FORCECHECKCMD      87
#define REPAIRTIMECMD      88
#define REPAIRSTATUSCMD    89

/* command function codes for cds management */
#define NULLCMD          100
#define EXITCMD          101
#define DOCMD            113
#define HELPCMD          114
#define SHOENTCMD        126
#define SETENTCMD        127
#define ADDSETCMD        128
#define REMSETCMD        129
#define SHOWSRV          130
#define SHOWCLK          131
#define SHOWCLKCLE       132
#define SHOWCLKSRV       133
#define CRECLECMD        134
#define DELCLECMD        135
#define DEFCLKSRV        136
#define DISSRV           137
#define DISCLK           138
#define SHOWDNS          139
#define SHOW500          140
#define CLEARCLE         141
#define CLEARCLKSRV      142
#define SHOWSRVCLE       144
#define GETCLHLIST       145

#define V1CREDIRCMD      155
#define V1CRELNKCMD      157
#define V1DELDIRCMD      159
#define V1DELLNKCMD      161
#define V1DELOBJCMD      162
#define V1CREOBJCMD      205
#define   CREREPCMD      206
#define   DELREPCMD      207
#define   SHOREPCMD      208
#define    SKULKCMD      209
#define NEWEPOCHCMD      210
#define TESTATTRCMD      211
#define TESTGROUPDIRCMD  212
#define TESTGROUPINDCMD  213
#define DIRDIRCMD        214
#define DIROBJCMD        215
#define DIRCLHCMD        216
#define DIRGRPCMD        217
#define DIRLNKCMD        218
#define DMPCCHCMD        224
#define CREKIDCMD        225
#define DELKIDCMD        226
#define SETCFCMD         231
#define SHOCFCMD         232
#define SETPFCCMD        252
#define SHOPFCCMD        253

#define RELOP_NO 0
#define RELOP_EQ 1

#define EXPLICIT_ATTR	0
#define NONE_ATTR	1
#define ALL_ATTR	2

/* The following codes are used to identify all the API calls made by
 * dnscp, which may result in error conditions which are reported by
 * dnscp in 'error_report.c', where the codes are converted to
 * appropriate strings.
*/

#define DNSREADATTRVALUE 0 /* dnsReadAttrValue */
#define DNSCREDIR 1        /* dnsCreateDir */
#define DNSDELDIR 2	/* dnsDelDir */
#define DNSCREOBJ 3	/* dnsCreateObj */
#define DNSDELOBJ 4 	/* dnsDelObj */
#define DNSCRESL  5	/*dnsCreateSLink*/
#define DNSDELSL  6	/* dnsDelSLink */
#define DNSENUMCHILD 7	/* dnsEnumChild */
#define DNSENUMOBJ 8	/* dnsEnumObj */
#define DNSENUMATTR 9	/* dnsEnumAttr */
#define DNSENUMSLINK 10 /* dnsEnumSLink */
#define DNSNEWEPOCH 11	/* dnsNewEpoch */
#define DNSSKULKDIR 12  /* dnsSkulkDir */
#define DNSCREGRP 13	/* dnsCreateGrp */
#define DNSDELGRP 14	/* dnsDelGrp */
#define DNSDELSLINK 15	/* dnsDelSLink */
#define DNSADDREPLICA 16 /* dnsAddReplica */
#define DNSRMREPLICA 17 /* dnsRmReplica */
#define DNSCOUNTSIMPLE 18 /* dnsCountSimple */
#define DNSADDATTRVALUE 19 /* dnsAddAttrValue */
#define DNSRMATTRVALUE 20 /* dnsRmAttrValue */
#define DNSALLOWCH		21	/* dnsAllowCH			*/
#define DNSDISALLOWCH		22	/* dnsDisallowCH		*/
#define DNSADDACC		23	/* dnsAddAcc			*/
#define DNSADDMBR		24	/* dnsAddMbr			*/
#define DNSBUILDADDR		25	/* dnsBuildAddr			*/
#define DNSCMPCTS		26	/* dnsCmpCTS			*/
#define DNSCMPFULL		27	/* dnsCmpFull			*/
#define DNSCMPSIMPLE		28	/* dnsCmpSimple			*/
#define DNSCVTADDRTOPHIV	29	/* dnsCvtAddrToPhIV		*/
#define DNSCVTBINARYTOOPQSIMPLE	30	/* dnsCvtBinaryToOpqSimple	*/
#define DNSCVTOPQFULLTOSTR	31	/* dnsCvtOpqFullToStr		*/
#define DNSCVTOPQSIMPLETOSTR	32	/* dnsCvtOpqSimpleToStr		*/
#define DNSCVTPHIVTOPRINCIPAL	33	/* dnsCvtPhIVToPrincipal	*/
#define DNSCVTSTRFULLTOOPQ	34	/* dnsCvtStrFullToOpq		*/
#define DNSCVTSTRSIMPLETOOPQ	35	/* dnsCvtStrSimpleToOpq		*/
#define DNSCVTTIMEOUTOTUTC	36	/* dnsCvtTimeoutToUTC		*/
#define DNSCVTUTCTOTIMEOUT	37	/* dnsCvtUTCToTimeout		*/
#define DNSENUMGRP		38	/* dnsEnumGrp			*/
#define DNSSCATTER		39	/* dnsScatter			*/
#define DNSGATHER		40	/* dnsGather			*/
#define DNSGETADDRLEN		41	/* dnsGetAddrLen		*/
#define DNSGETOPQFULLLEN	42	/* dnsGetOpqFullLen		*/
#define DNSGETOPQSIMPLELEN	43	/* dnsGetOpqSimpleLen		*/
#define DNSMAXSIZE		44	/* dnsMaxSize			*/
#define DNSPRESELECT		45	/* dnsPreSelect			*/
#define DNSPOSTSELECT		46	/* dnsPostSelect		*/
#define DNSREADACC		47	/* dnsReadAcc			*/
#define DNSREADMBR		48	/* dnsReadMbr			*/
#define DNSRESOLVENAME		49	/* dnsResolveName		*/
#define DNSRMACC		50	/* dnsRmAcc			*/
#define DNSRMATTR		51	/* dnsRmAttr			*/
#define DNSRMMBR		52	/* dnsRmMbr			*/
#define DNSTESTATTRVALUE	53	/* dnsTestAttrValue		*/
#define DNSTESTMBR		54	/* dnsTestMbr			*/
#define DNSDELCHILD		55	/* dnsDelChild			*/
#define DNSCRECHILD		56	/* dnsCreateChild		*/
#define DNSDELATTRVALUE		57	/* dnsDelAttrValue		*/
#endif  /* #ifndef _DNSMANPAR_H */
