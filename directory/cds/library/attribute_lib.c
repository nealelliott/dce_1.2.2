/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: attribute_lib.c,v $
 * Revision 1.1.8.2  1996/02/18  19:32:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:02  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:20:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:33  root]
 * 
 * Revision 1.1.6.7  1994/08/16  18:27:56  jd
 * 	drop from janet
 * 	[1994/08/13  16:06:35  jd]
 * 
 * Revision 1.1.6.6  1994/06/30  19:16:20  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:12  mccann]
 * 
 * Revision 1.1.6.5  1994/06/09  18:40:24  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:59  devsrc]
 * 
 * Revision 1.1.6.4  1994/05/06  16:04:12  zee
 * 	     HCell BL4 support: Added CDS_ParentCellPointers attribute.
 * 	[1994/05/05  21:07:12  zee]
 * 
 * Revision 1.1.6.3  1994/04/29  15:51:31  peckham
 * 	Eliminate unnecessary conditionals.
 * 	[1994/04/29  13:59:02  peckham]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:09  griffin
 * 	HCell BL3 support: Added CDS_CellAliases attribute.
 * 	[1994/03/22  16:46:19  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:05:33  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:24:00  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:53:13  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:06:04  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/28  16:43:44  peckham
 * 	OT2515: make CDS_ObjectUUID unmodifiable like CDS_CTS.
 * 	[1992/04/28  14:47:36  peckham]
 * 
 * Revision 1.1  1992/01/19  15:19:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module attribute_lib.c
 *
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
 *
 * DNS_UPPOINTERS was  {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x23}
 *     never used in CDS so removed from table.
 * DNS_NSNICKNAME was {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x26}
 *     never used in CDS
 * DNS_NSUID was {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x2f}
 *     never used in CDS
 * 
 */


#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/dns_record.h>
#include <dce/attributes.h>

struct class_descriptor

cds_classlist[10] = { /* Leave room for expansion */
    {"\010\021CDS_Clearinghouse"},
    {"\010\011CDS_Group"}
};

#define	MIN_NETWORKADDRESS 1
#define	MIN_TOWER 2
#define	MIN_FULLNAME sizeof(NULLFULLNAME_u)
#define	DIF_FULLNAME (sizeof(FullName_u) - MIN_FULLNAME)
#define	MIN_ACE (sizeof(ACE_u) - DIF_FULLNAME)
#define	MIN_GROUPMEMBER (sizeof(GroupMember_u) - DIF_FULLNAME)
#define	MIN_REPLICAPOINTER (sizeof(ReplicaPointer_u) \
    - MAXATTRIBUTE + sizeof(NULLSET_u) - DIF_FULLNAME)
#define MIN_GDAPOINTER (sizeof(gdaPointer_u) \
    - MAXATTRIBUTE + sizeof(NULLSET_u) - DIF_FULLNAME)

struct attribute_descriptor
cds_attlist[50] = { /* Leave room for expansion */
  {"\001\013DNS$MEMBERS",
     DNS_MEMBER, 0x7e0, AT_set, VT_GroupMember, TRUE,
     TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, 
     MIN_GROUPMEMBER, sizeof(GroupMember_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0xA}},
  {"\001\017DNS$GROUPREVOKE",
     DNS_GROUPREVOKE, 0x7de0, AT_single, VT_Timeout, TRUE,
     TRUE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE,
     sizeof(Timeout_u), sizeof(Timeout_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0xB}},
  {"\001\007DNS$CTS",
     DNS_CTS, 0x0, AT_single, VT_Timestamp, TRUE,
     FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE,
     sizeof(Timestamp_u), sizeof(Timestamp_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0xC}},
  {"\001\007DNS$UTS",
     DNS_UTS, 0x0, AT_single, VT_Timestamp, TRUE,
     FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE,
     sizeof(Timestamp_u), sizeof(Timestamp_u),
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0xD}},
  {"\001\011DNS$CLASS",
     DNS_CLASS, 0x1e0, AT_single, VT_byte, TRUE,
     FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE,
     sizeof(NULLSIMPLENAME_u), sizeof(ClassName_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0xF}},
  {"\001\020DNS$CLASSVERSION",
     DNS_CLASSVERSION, 0xfde0, AT_single, VT_Version, TRUE,
     FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE,
     sizeof(ClassVersion_u), sizeof(ClassVersion_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x10}},
  {"\001\015DNS$OBJECTUID",
     DNS_OBJECTUID, 0xff07, AT_single, VT_uuid, TRUE,
     FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE,
     sizeof(uuid_t), sizeof(uuid_t),
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x11}},
  {"\001\014DNS$REPLICAS",
     DNS_REPLICAS, 0xfe0, AT_set, VT_ReplicaPointer, TRUE,
     FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE,
     MIN_REPLICAPOINTER, MAXATTRIBUTE, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x13}},
  {"\001\013DNS$ALLUPTO",
     DNS_ALLUPTO, 0x560, AT_single, VT_Timestamp, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
     sizeof(Timestamp_u), sizeof(Timestamp_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x14}},
  {"\001\017DNS$CONVERGENCE",
     DNS_CONVERGENCE, 0x7fe0, AT_single, VT_small, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE,
     1, 1, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x15}},
  {"\001\014DNS$INCHNAME",
       DNS_INCHNAME, 0xe20, AT_single, VT_small, TRUE,
       TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE,
       sizeof(BOOLEAN_u), sizeof(BOOLEAN_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x16}},
  {"\001\021DNS$PARENTPOINTER",
       DNS_PARENTPOINTER, 0x1fbe0, AT_set, VT_ParentPointer, TRUE,
       FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, 
       sizeof(ParentPointer_u) - DIF_FULLNAME, sizeof(ParentPointer_u),
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x17}},
  {"\001\024DNS$DIRECTORYVERSION",
     DNS_DIRECTORYVERSION, 0xfdfe0, AT_single, VT_Version, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE,
     sizeof(version_u), sizeof(version_u),
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x18}},
  {"\001\015DNS$UPGRADETO",
     DNS_UPGRADETO, 0x17e0, AT_single, VT_Version, TRUE,
     TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE,
     sizeof(version_u), sizeof(version_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x19}},
  {"\001\016DNS$LINKTARGET",
     DNS_LINKTARGET, 0x3ee0, AT_single,VT_FullName, TRUE,
     FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE,
     MIN_FULLNAME, sizeof(FullName_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x1B}},
  {"\001\017DNS$LINKTIMEOUT",
     DNS_LINKTIMEOUT, 0x7ee0, AT_single, VT_Timeout, TRUE,
     TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, FALSE,
     sizeof(Timeout_u), sizeof(Timeout_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x1C}},
  {"\001\012DNA$TOWERS",
     DNA_TOWERS, 0x3e0, AT_set, VT_byte, TRUE,
     TRUE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE,
     MIN_TOWER, MAXATTRIBUTE, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x01, 0x03, 0x1E}},
  {"\001\012DNS$CHNAME",
     DNS_CHNAME, 0x380, AT_set, VT_FullName, TRUE,
     FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE,
     MIN_FULLNAME, sizeof(FullName_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x20}},
  {"\001\021DNS$CHLASTADDRESS",
     DNS_CHLASTADDRESS, 0x1fb80, AT_set, VT_byte, TRUE,
     FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE,
     MIN_NETWORKADDRESS, MAXATTRIBUTE, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x22}},
  {"\001\013DNS$CHSTATE",
     DNS_CHSTATE, 0x780, AT_single, VT_small, TRUE,
     FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
     sizeof(CHState_u), sizeof(CHState_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x24}},
  {"\001\021DNS$CHDIRECTORIES",
     DNS_CHDIRECTORIES, 0x1ff80, AT_set, VT_CHDirectory, TRUE,
     FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE,
     MIN_FULLNAME, sizeof(FullName_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x25}},
  {"\001\020DNS$REPLICASTATE",
     DNS_REPLICASTATE, 0xf7e0, AT_single, VT_small, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE,
     sizeof(ReplicaState_u), sizeof(ReplicaState_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x28}},
  {"\001\017DNS$REPLICATYPE",
     DNS_REPLICATYPE, 0x77e0, AT_single, VT_small, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE,
     sizeof(ReplicaType_u), sizeof(ReplicaType_u),
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x29}},
  {"\001\015DNS$LASTSKULK",
     DNS_LASTSKULK, 0x1ee0, AT_single, VT_Timestamp, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE,
     sizeof(Timestamp_u), sizeof(Timestamp_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x2a}},
  {"\001\016DNS$LASTUPDATE",
     DNS_LASTUPDATE, 0x3ee0, AT_single, VT_Timestamp, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE,
     sizeof(Timestamp_u), sizeof(Timestamp_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x2b}},
  {"\001\017DNS$RINGPOINTER",
     DNS_RINGPOINTER, 0x7ee0, AT_single, VT_uuid, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE,
     sizeof(uuid_t), sizeof(uuid_t), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x2c}},
  {"\001\011DNS$EPOCH",
     DNS_EPOCH, 0x1e0, AT_single, VT_uuid, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE,
     sizeof(uuid_t), sizeof(uuid_t), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x2d}},
  {"\001\022DNS$REPLICAVERSION",
     DNS_REPLICAVERSION, 0x3f7e0, AT_single, VT_Version, TRUE,
     FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE,
     sizeof(version_u), sizeof(version_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x2e}},
  {"\001\016DNS_NSCELLNAME",
     DNS_NSCELLNAME, 0x3F80, AT_single, VT_char, TRUE,
     FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE,
     3, 10000, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x30}},
  {"\001\015DNS_OBJECTACL",
     DNS_OBJECTACL, 0x0, AT_single, VT_DACL, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE,
     0, MAXATTRIBUTE, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x31}},
  {"\001\026DNS_DEFAULTOBJECTACL",
     DNS_DEFOBJECTACL, 0x0, AT_single, VT_DACL, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE,
     0, MAXATTRIBUTE, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x32}},
  {"\001\031DNS_DEFAULTCONTAINERACL",
     DNS_DEFCONTAINERACL, 0x0, AT_single, VT_DACL, FALSE,
     TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE,
     0, MAXATTRIBUTE, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x33}}
  ,{"\001\017DNS$GDAPOINTERS",
     DNS_GDAPOINTERS, 0x7F00, AT_set, VT_gdaPointer, TRUE, FALSE,
     TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE,
     MIN_GDAPOINTER, MAXATTRIBUTE, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x34}}
  ,{"\001\017DNS_CELLALIASES",
     DNS_CELLALIASES, 0x7F00, AT_set, VT_GroupMember, TRUE,
       TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE,
     MIN_GROUPMEMBER, sizeof(GroupMember_u), 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x35}}
  ,{"\001\026DNS_PARENTCELLPOINTERS",
     DNS_PARENTCELLPOINTERS, 0x7F00, AT_set, VT_ReplicaPointer, TRUE,
       TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE,
     MIN_REPLICAPOINTER, MAXATTRIBUTE, 
     {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x1, 0x3, 0x36}}
};
