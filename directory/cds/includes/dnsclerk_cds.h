/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsclerk_cds.h,v $
 * Revision 1.1.9.2  1996/02/18  23:33:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:29  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:15:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:03  root]
 * 
 * Revision 1.1.7.3  1994/06/30  19:15:39  mccann
 * 	sams cleanup drop 2
 * 	[1994/06/30  18:01:35  mccann]
 * 
 * Revision 1.1.7.2  1994/06/09  18:39:00  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:39  devsrc]
 * 
 * Revision 1.1.7.1  1994/03/12  22:02:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:14:38  peckham]
 * 
 * Revision 1.1.5.2  1992/12/30  13:30:09  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:48:22  zeliff]
 * 
 * Revision 1.1.2.3  1992/07/06  20:14:26  mfox
 * 	Limit classnames to 31 characters to prevent server crash
 * 	[1992/07/06  20:10:17  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:04:42  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:26:32  weisman]
 * 
 * Revision 1.1  1992/01/19  15:16:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNSCLERK_CDS_H
#define _DNSCLERK_CDS_H

/*
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 *
 * NOTE:
 *	Any changes in this file MUST be reflected in
 *	dnsclerk.h, cdsclerk.h, and dns_record.h
 *
 *
 */


#include <dce/cdsclerk.h>

/* ============================================================ */
/*  Constants and Data Structures				*/
/* ============================================================ */

/* ============================================================ */
/* The following constants define the maximum lengths for	*/
/* various buffers and data structures.				*/
/* ============================================================ */

#define DNS_OPQ_SNAME_MAX	  CDS_OPQ_SNAME_MAX
#define DNS_OPQ_FNAME_MAX	  CDS_OPQ_FNAME_MAX
#define DNS_OPQ_ANAME_MAX	  CDS_OPQ_ANAME_MAX
#define DNS_OPQ_CNAME_MAX	  CDS_OPQ_CNAME_MAX
#define DNS_CTS_LEN_MAX		  CDS_CTS_LEN_MAX
#define DNS_ID_LEN_MAX		  CDS_ID_LEN_MAX
#define DNS_TIMEOUT_MAX           CDS_TIMEOUT_MAX

#define DNS_STR_SNAME_MAX         CDS_STR_SNAME_MAX
#define DNS_STR_FNAME_MAX         CDS_STR_FNAME_MAX
#define DNS_STR_ANAME_MAX         CDS_STR_ANAME_MAX
#define DNS_STR_CNAME_MAX         CDS_STR_CNAME_MAX

#define DNS_FNAME_MAX		 CDS_FNAME_MAX
#define DNS_SNAME_MAX		 CDS_SNAME_MAX
#define DNS_ANAME_MAX		 CDS_ANAME_MAX
#define DNS_CNAME_MAX		 CDS_CNAME_MAX

/* ============================================================ */
/* The following constants are used by dnsMaxSize to represent  */
/* various DNS datatypes.            				*/
/* ============================================================ */

#define DNS_OPQ_SIMPLE_NAME	CDS_OPQ_SIMPLE_NAME
#define DNS_OPQ_FULL_NAME	CDS_OPQ_FULL_NAME

/* ============================================================ */
/* Typedefs used in the Portable Interface routines.		*/
/* ============================================================ */

/* ==================================== */
/* Creation Time Stamp			*/
/* ==================================== */

#define dns_cts cds_cts_t
#define dns_cts_t cds_cts_t

#define dns_id_t cds_id_t
#define dns_id	cds_id_t

/* ==================================== */
/* DNS Handle				*/
/* ==================================== */

#define dns_handle_t cds_handle_t
#define dns_handle cds_handle_t

/* ==================================== */
/* DNS Full Name			*/
/* ==================================== */

#define dns_opq_fname_t cds_full_name_t
#define dns_opq_fname cds_full_name_t
#define dns_full_name_t cds_full_name_t

/* ==================================== */
/* DNS Simple Name		*/
/* ==================================== */

#define dns_opq_sname_t cds_simple_name_t
#define dns_opq_sname cds_simple_name_t
#define dns_simple_name_t cds_simple_name_t

/* ==================================== */
/* DNS Attribute Name                   */
/* ==================================== */

#define dns_attr_name_t cds_attr_name_t

/* ==================================== */
/* DNS Timeout				*/
/* ==================================== */

#define dns_timeout_t cds_timeout_t
#define dns_timeout cds_timeout_t


/* ============================================================ */
/* Manifest constants defining NULL pointers associated with 	*/
/* typedefs above and below.					*/
/* ============================================================ */

#define NULL_DNS_CTS		NULL_CDS_CTS
#define NULL_DNS_ID		NULL_CDS_ID
#define NULL_DNS_HANDLE		NULL_CDS_HANDLE
#define NULL_DNS_OPQ_FNAME	NULL_CDS_OPQ_FNAME
#define NULL_DNS_OPQ_SNAME	NULL_CDS_OPQ_SNAME
#define NULL_DNS_OPQ_ANAME	NULL_CDS_OPQ_ANAME
#define NULL_DNS_TIMEOUT	NULL_CDS_TIMEOUT
#define NULL_DNSFLAGSTAT	NULL_CDSFLAGSTAT

/* ============================================================ */
/* Version is a 2 byte number, one for major and                */
/* the other for minor.					        */
/* ============================================================ */

#define dnsMajVer cdsMajVer
#define dnsMinVer cdsMinVer
#define dnsVersion cdsVersion
#define dnsVersion_t cdsVersion_t
#define dnsClassVer cdsVersion
#define dnsClassVer_t cdsVersion_t
#define dnsReplicaVer cdsVersion
#define dnsReplicaVer_t cdsVersion_t

/* ============================================================ */
/* Enumeration constants                                        */
/* ============================================================ */

#define dnsEmptySet cdsEmptySet
#define dnsSingle cdsSingle
#define dnsSet cdsSet
#define dnsAttrType cdsAttrType
#define dnsAttrType_t cdsAttrType_t

#define dnsConfLow cdsConfLow
#define dnsConfMed cdsConfMed
#define dnsConfHi cdsConfHi
#define dnsConf_t cdsConf_t
#define dnsConf cdsConf

#define dnsDir cdsDir
#define dnsObj cdsObj
#define dnsChildPtr cdsChildPtr
#define dnsSLink cdsSLink
#define dnsCH cdsCH
#define dnsAnyEntries cdsAnyEntries
#define dnsDirOrObj cdsDirOrObj
#define dnsEntries cdsEntries
#define dnsEntries_t cdsEntries_t

#define dnsPrincipal cdsPrincipal
#define dnsGroup cdsGroup
#define dnsGrpMbrType cdsGrpMbrType
#define dnsGrpMbrType_t cdsGrpMbrType_t

#define dnsMaster cdsMaster
#define dnsSecondary cdsSecondary
#define dnsReadOnly cdsReadOnly
#define dnsGda cdsGda
#define dnsReplicaType cdsReplicaType
#define dnsReplicaType_t cdsReplicaType_t

#define dnsStrDCE cdsStrDCE
#define dnsOpqDNA cdsOpqDNA
#define dnsStrDNA cdsStrDNA
#define dnsStrXDS cdsStrXDS
#define dnsNameType_t cdsNameType_t

/* ==================================== */
/* DNS Value Type			*/
/* ==================================== */

#define dns_none cds_none
#define dns_long cds_long
#define dns_short cds_short
#define dns_small cds_small
#define dns_uuid cds_uuid
#define dns_Timestamp cds_Timestamp
#define dns_Timeout cds_Timeout
#define dns_Version cds_Version
#define dns_char cds_char
#define dns_byte cds_byte
#define dns_ReplicaPointer cds_ReplicaPointer
#define dns_GroupMember cds_GroupMember
#define dns_ParentPointer cds_ParentPointer
#define dns_FullName cds_FullName
#define dns_CHDirectory cds_CHDirectory
#define dns_ASN1 cds_ASN1
#define dns_gdaPointer cds_gdaPointer
#define dns_syntax_t cds_syntax_t

#define dns_syntax_t cds_syntax_t
#define dns_attrvalue_t cds_attrvalue_t

/* ==================================== */
/* DNS Replica Pointer			*/
/* ==================================== */

#define dns_replicaptr_t cds_replicaptr_t

/* ============================================================ */
/* NOTE:  When using the structure dnsFlagStat, any flag which	*/
/*	  is defined as an IN field and which is initialized to */
/*        zero by the calling program will indicate that the    */
/*        default value be taken for that particular flag.	*/
/* ============================================================ */

#define dnsFlagStat cdsFlagStat
#define fsDNSStat fsCDSStat
#define fsOpqDNS fsOpqCDS
#define dnsFlagStat_t cdsFlagStat_t

typedef enum dnsAsyncStat {
    dnsInit = 1,
    dnsWait,
    dnsDone
} dnsAsyncStat_t;

/* ============================================================ */
/* Fields used in access control routines for argument AceFlags */
/* ============================================================ */

#define DNS_ACE_GROUP		CDS_ACE_GROUP
/*
 * Define compatibility macros for CDS users of the old interface
 */

#define dnsAddAttrValue(a,b,c,d,e,f,g)	cdsAddAttrValue(a,b,c,d,e,f,g)
#define dnsIntAddAttrValue(a,b,c,d,e,f,g,h,i) \
	 cdsIntAddAttrValue(a,b,c,d,e,f,g,h,i)
#define dnsAddMbr(a,b,c,d)		cdsAddMbr(a,b,c,d)
#define dnsAddReplica(a,b,c,d)		cdsAddReplica(a,b,c,d)
#define dnsAllowCH(a,b)			cdsAllowCH(a,b)
#define dnsCacheDir(a,b,c,d)		cdsCacheDir(a,b,c,d)
#define dnsCancel			cdsCancel
#define dnsClearHandle(a)		cdsClearHandle(a)
#define dnsCmpCTS(a,b)			cdsCmpCTS(a,b)
#define dnsCmpFull(a,b)			cdsCmpFull(a,b)
#define dnsCmpSimple(a,b)		cdsCmpSimple(a,b)
#define dnsCountSimple(a)		cdsCountSimple(a)
#define dnsCreateCTS(a)			cdsCreateCTS(a)
#define dnsCreateDir(a,b,c,d)		cdsCreateDir(a,b,c,d)
#define dnsCreateGrp(a,b)		cdsCreateGrp(a,b)
#define dnsCreateObj(a,b,c,d,e,f)	cdsCreateObj(a,b,c,d,e,f)
#define dnsCreateSLink(a,b,c,d,e)	cdsCreateSLink(a,b,c,d,e)
#define dnsCvtBinaryToOpqSimple(a,b,c,d) cdsCvtBinaryToOpqSimple(a,b,c,d)
#define dnsCvtCDSFullToOpq(a,b,c)	cdsCvtFullToOpq(a,b,c)
#define dnsCvtCDSFullToStr(a,b,c)	cdsCvtFullToStr(a,b,c)
#define dnsCvtCDSSimpleToStr(a,b,c)	cdsCvtSimpleToStr(a,b,c)
#define dnsCvtCDSSimpleToOpq(a,b,c,d)	cdsCvtSimpleToOpq(a,b,c,d)
#define dnsCvtObjIDToStr(a,b,c,d,e,f)	cdsCvtObjIDToStr(a,b,c,d,e,f)
#define dnsCvtStrToObjID(a,b,c,d,e,f,g)	cdsCvtStrToObjID(a,b,c,d,e,f,g)
#define dnsCvtStrToTower(a,b)		cdsCvtStrToTower(a,b)
#define dnsCvtStrToTs(a,b,c)		cdsCvtStrToTS(a,b,c)
#define dnsCvtTowerToStr(a,b)		cdsCvtTowerToStr(a,b)
#define dnsCvtTsToStr(a,b)		cdsCvtTsToStr(a,b)
#define dnsDelDir(a,b)			cdsDelDir(a,b)
#define dnsDelGrp(a,b)			cdsDelGrp(a,b)
#define dnsDelObj(a,b)			cdsDelObj(a,b)
#define dnsDelSLink(a,b)		cdsDelSLink(a,b)
#define dnsDisallowCH(a,b)		cdsDisallowCH(a,b)
#define dnsEnumAttr(a,b,c,d,e,f,g,h)	cdsEnumAttr(a,b,c,d,e,f,g,h)
#define dnsEnumChild(a,b,c,d,e,f,g)	cdsEnumChild(a,b,c,d,e,f,g)
#define dnsEnumGrp(a,b,c,d,e,f,g)	cdsEnumGrp(a,b,c,d,e,f,g)
#define dnsEnumObj(a,b,c,d,e,f,g,h,i,j,k,l) cdsEnumObj(a,b,c,d,e,f,g,h,i,j,k,l)
#define dnsEnumSLink(a,b,c,d,e,f,g)	cdsEnumSLink(a,b,c,d,e,f,g)
#define dnsExpandStrFull(a,b,c)		cdsExpandStrFull(a,b,c)
#define dnsFreeHandle(a)		cdsFreeHandle(a)
#define dnsGather(a,b,c,d,e)		cdsGather(a,b,c,d,e)
#define dnsGetHandle			cdsGetHandle
#define dnsGetMsg(a,b,c)		cdsGetMsg(a,b,c)
#define dnsGetOpqFullLen(a)		cdsGetOpqFullLen(a)
#define dnsGetOpqSimpleLen(a)		cdsGetOpqSimpleLen(a)
#define dnsGetXAttrByObjID(a)		cdsGetXAttrByObjID(a)
#define dnsGetXAttrByString		cdsGetXAttrByString
#define dnsGetXAttrEntry		cdsGetXAttrEntry
#define dnsMaxSize(a)			cdsMaxSize(a)
#define dnsNewEpoch(a,b,c,d,e)		cdsNewEpoch(a,b,c,d,e)
#define dnsReadAttrValue(a,b,c,d,e,f,g,h,i,j) cdsReadAttrValue(a,b,c,d,e,f,g,h,i,j)
#define dnsReadMbr(a,b,c,d,e,f,g)	cdsReadMbr(a,b,c,d,e,f,g)
#define dnsResolveName(a,b,c,d)		cdsResolveName(a,b,c,d)
#define dnsRmAttr(a,b,c,d,e,f)		cdsRmAttr(a,b,c,d,e,f)
#define dnsRmAttrValue(a,b,c,d,e,f)	cdsRmAttrValue(a,b,c,d,e,f)
#define dnsIntRmAttrValue(a,b,c,d,e,f,g,h) cdsIntRmAttrValue(a,b,c,d,e,f,g,h)
#define dnsRmMbr(a,b,c,d)		cdsRmMbr(a,b,c,d)
#define dnsRmReplica(a,b,c)		cdsRmReplica(a,b,c)
#define dnsScatter(a,b,c,d,e,f,g,h)	cdsScatter(a,b,c,d,e,f,g,h)
#define dnsSkulkDir(a,b)		cdsSkulkDir(a,b)
#define dnsTestAttrValue(a,b,c,d,e)	cdsTestAttrValue(a,b,c,d,e)
#define dnsIntTestAttrValue(a,b,c,d,e,f,g) cdsIntTestAttrValue(a,b,c,d,e,f,g)
#define dnsTestMbr(a,b,c,d,e,f)		cdsTestMbr(a,b,c,d,e,f)

#endif  /* #ifndef _DNSCLERK_CDS_H */
