/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc_cvt.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:17  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:17:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:38  root]
 * 
 * Revision 1.1.4.5  1994/08/03  19:02:36  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:59:34  mccann]
 * 
 * Revision 1.1.4.4  1994/06/30  19:15:40  mccann
 * 	sams cleanup drop 2
 * 	[1994/06/30  18:01:40  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:39:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:08  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/07  10:22:25  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:11  kevins]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:03  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:18:49  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:33:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:53:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _RPC_CVT_H
#define _RPC_CVT_H
/*
 * Module: rpc_cvt.h
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
 */

#include <dce/rpc.h>

#if defined(DCE_SEC)
# include <dce/aclbase.h>
#endif


typedef struct cds_visa_s {
    enum {			/* for backwards compatibility */
	visa_all_global = 1,	/*  global fullnames provided */
	visa_all_local,		/*  local fullnames provided */
	visa_all_mixed		/*  mixed fullnames provided */
    }			visa_names;
    enum {			/* was progress for local cell? */
	visa_there = 1,		/*  progress not destined for here */
	visa_here		/*  progress destined for visa_cellname */
    }			visa_progress;
    enum {			/* was cellname stripped from last fullname? */
	visa_global = 1,	/*  unstripped global fullname */
	visa_stripped,		/*  stripped global fullname */
	visa_local		/*  local fullname */
    }			visa_fullname;
    cds_FullName_t	visa_str_cellname;
} cds_visa_t;

void
cds_visa_init (
    cds_visa_t		*,
    char		*);

int
cds_visa_local_destination (
    cds_visa_t		*);

void
cds_visa_destroy (
    cds_visa_t		*);

byte_t *
cdsExport_Timeout (
    cds_visa_t		*,
    Timeout_t		*,
    cds_TimeoutP_t	*);
#define L_to_R_Timeout(a,b) \
    cdsExport_Timeout(NULL,(a),(b))

byte_t *
cdsImport_Timeout (
    cds_visa_t		*,
    cds_Timeout_t	*,
    Timeout_t		*);
#define R_to_L_Timeout(a,b) \
    cdsImport_Timeout(NULL,(a),(b))

byte_t *
cdsExport_Timestamp (
    cds_visa_t		*,
    Timestamp_t		*,
    cds_Timestamp_t	*);
#define L_to_R_Timestamp(a,b) \
    cdsExport_Timestamp(NULL,(a),(b))

byte_t *
cdsImport_Timestamp (
    cds_visa_t		*,
    cds_Timestamp_t	*,
    Timestamp_t		*,
    int			);
#define R_to_L_Timestamp(a,b,c) \
    cdsImport_Timestamp(NULL,(a),(b),(c))

void
cdsExport_new_Progress (
    cds_visa_t		*,
    byte_t		*,
    Timeout_t		*,
    FullName_t		*, 
    FullName_t		*,
    cds_Progress_t	*);
#define Build_R_Progress(a,b,c,d,e) \
    cdsExport_new_Progress(NULL,(a),(b),(c),(d),(e))

byte_t *
cdsExport_Progress (
    cds_visa_t		*,
    Progress_t		*,
    cds_Progress_t	*,
    cds_status_t	*);
#define L_to_R_Progress(a,b,c) \
    cdsExport_Progress(NULL,(a),(b),(c))

byte_t *
cdsImport_Progress (
    cds_visa_t		*,
    cds_Progress_t	*,
    Progress_t		*,
    int			);
#define R_to_L_Progress(a,b,c) \
    cdsImport_Progress(NULL,(a),(b),(c))

byte_t *
cdsExport_FullName (
    cds_visa_t		*,
    FullName_t		*,
    cds_FullName_t	*);
#define L_to_R_FullName(a,b) \
    cdsExport_FullName(NULL,(a),(b))

byte_t *
cdsImport_FullName (
    cds_visa_t		*,
    cds_FullName_t	*,
    FullName_t		*,
    int			);
#define R_to_L_FullName(a,b,c) \
    cdsImport_FullName(NULL,(a),(b),(c))

byte_t *
cdsExport_AttributeName (
    cds_visa_t		*,
    AttributeName_t	*,
    cds_Name_t		*);
#define L_to_R_AttributeName(a,b) \
    cdsExport_AttributeName(NULL,(a),(b))

byte_t *
cdsImport_AttributeName (
    cds_visa_t		*,
    cds_Name_t		*,
    AttributeName_t	*,
    int			);
#define R_to_L_AttributeName(a,b,c) \
    cdsImport_AttributeName(NULL,(a),(b),(c))

byte_t *
cdsExport_SimpleName (
    cds_visa_t		*,
    SimpleName_t	*,
    cds_Name_t		*);
#define L_to_R_SimpleName(a,b) \
    cdsExport_SimpleName(NULL,(a),(b))

byte_t *
cdsImport_SimpleName (
    cds_visa_t		*,
    cds_Name_t		*,
    SimpleName_t	*,
    int			);
#define R_to_L_SimpleName(a,b,c) \
    cdsImport_SimpleName(NULL,(a),(b),(c))

byte_t *
cdsExport_ClassName (
    cds_visa_t		*,
    ClassName_t		*,
    cds_Name_t		*);
#define L_to_R_ClassName(a,b) \
    cdsExport_ClassName(NULL,(a),(b))

byte_t *
cdsImport_ClassName (
    cds_visa_t		*,
    cds_Name_t		*,
    ClassName_t		*,
    int			);
#define R_to_L_ClassName(a,b,c) \
    cdsImport_ClassName(NULL,(a),(b),(c))

byte_t *
cdsExport_Set (
    cds_visa_t		*,
    Set_t		*,
    cds_SetP_t		*,
    cds_status_t	*);
#define L_to_R_Set(a,b,c) \
    cdsExport_Set(NULL,(a),(b),(c))

byte_t *
cdsImport_Set (
    cds_visa_t		*,
    cds_Set_t		*,
    Set_t		*,
    int			);
#define R_to_L_Set(a,b,c) \
    cdsImport_Set(NULL,(a),(b),(c))

byte_t *
cdsExport_MemberValuetoSet (
    cds_visa_t		*,
    MemberValue_t	*,
    cds_SetP_t		*,
    cds_status_t	*);
#define L_to_R_MemberValuetoSet(a,b,c) \
    cdsExport_MemberValuetoSet(NULL,(a),(b),(c))

byte_t *
cdsImport_MemberValue (
    cds_visa_t		*,
    cds_SetMember_t	*,
    MemberValue_t	*,
    int			);
#define R_to_L_MemberValue(a,b,c) \
    cdsImport_MemberValue(NULL,(a),(b),(c))

byte_t *
cdsExport_SetMember (
    cds_visa_t		*,
    MemberValue_t	*,
    cds_SetMember_t	*,
    cds_status_t	*);
#define L_to_R_SetMember(a,b,c) \
    cdsExport_SetMember(NULL,(a),(b),(c))

void
cdsExport_Exception (
    cds_visa_t		*,
    byte_t		*,
    cds_status_t	*);
#define L_to_R_Exception(a,b) \
    cdsExport_Exception(NULL,(a),(b))

byte_t *
cdsImport_Exception (
    cds_visa_t		*,
    cds_status_t	*,
    byte_t		*,
    int			);
#define R_to_L_Exception(a,b,c) \
    cdsImport_Exception(NULL,(a),(b),(c))

byte_t *
cdsExport_AtomicValue (
    cds_visa_t		*,
    AtomicValue_t	*,
    cds_AtomicValue_t	*,
    cds_status_t	*);
#define L_to_R_AtomicValue(a,b,c) \
    cdsExport_AtomicValue(NULL,(a),(b),(c))

byte_t *
cdsImport_AtomicValue (
    cds_visa_t		*,
    cds_AtomicValue_t	*,
    AtomicValue_t	*,
    int			);
#define R_to_L_AtomicValue(a,b,c) \
    cdsImport_AtomicValue(NULL,(a),(b),(c))

byte_t *
cdsExport_Update (
    cds_visa_t		*,
    Update_t		*,
    cds_Update_t	*,
    cds_status_t	*);
#define L_to_R_Update(a,b,c) \
    cdsExport_Update(NULL,(a),(b),(c))

byte_t *
cdsImport_Update (
    cds_visa_t		*,
    cds_Update_t	*,
    Update_t		*,
    int			);
#define R_to_L_Update(a,b,c) \
    cdsImport_Update(NULL,(a),(b),(c))

byte_t *
cdsExport_UpdatePkt (
    cds_visa_t		*,
    UpdatePacket_t	*,
    cds_UpdatePkt_t	**,
    cds_status_t	*);
#define L_to_R_UpdatePkt(a,b,c) \
    cdsExport_UpdatePkt(NULL,(a),(b),(c))

byte_t *
cdsImport_UpdatePkt (
    cds_visa_t		*,
    cds_UpdatePkt_t	*,
    UpdatePacket_t	*,
    cds_status_t	*);
#define R_to_L_UpdatePkt(a,b,c) \
    cdsImport_UpdatePkt(NULL,(a),(b),(c))

void
R_Set_Free (
     cds_Set_t		*);

void
R_Timeout_Free (
    cds_Timeout_t	*);

void
R_UpdatePkt_Free (
     cds_UpdatePkt_t	*);

void
R_AtomicValue_Free (
     cds_AtomicValue_t	*);

#ifdef DCE_SEC

void
R_AclEntry_Free (
    sec_acl_entry_t 	*);

void
R_Acl_Free (
    sec_acl_t 	        *,
    int                 );

#endif

byte_t *
RPTowerUnpack (
    byte_t		*,
    Set_t		*);

byte_t *
RPTowerPack (
    Set_t		 *,
    cds_ReplicaPointer_t *);

/*
 * Extensions for display interpreters
 */

typedef struct {
    unsigned long val;
    unsigned long mask;
    char *name;
} deb_ascii_bits_t;

char *
deb_ascii_bits (
    char             *,
    unsigned long    ,
    deb_ascii_bits_t *);

char *
deb_ascii_PDU (
    char          *,
    unsigned long );
#define DEB_ASCII_LEN_PDU DEB_ASCII_LEN_u_long
#define DEB_ASCII_BUF_PDU(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_PDU)

char *
deb_ascii_cds_Name (
    char       *,
    cds_Name_t *);
#define DEB_ASCII_BUF_cds_Name(nam) DEB_ASCII_BUF_AttributeName(nam)

char *
deb_ascii_cds_Update (
    char         *,
    cds_Update_t *);
#define DEB_ASCII_LEN_cds_Update \
	(9+DEB_ASCII_LEN_AttributeName)
#define DEB_ASCII_BUF_cds_Update(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_cds_Update)

char *
deb_ascii_cds_FullName (
    char           *,
    cds_FullName_t *);
#define DEB_ASCII_LEN_cds_FullName \
	(DEB_ASCII_LEN_ObjUID+1+1023)
#define DEB_ASCII_BUF_cds_FullName(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_cds_FullName)

char *
deb_ascii_cds_status (
    char         *,
    cds_status_t *);
#define DEB_ASCII_LEN_cds_status \
	(DEB_ASCII_LEN_PDU+DEB_ASCII_LEN_cds_FullName+1)
#define DEB_ASCII_BUF_cds_status(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_cds_status)

char *
deb_ascii_cds_ReplicaPointer (
    char                 *,
    cds_ReplicaPointer_t *);
#define DEB_ASCII_LEN_cds_ReplicaPointer \
	(DEB_ASCII_LEN_cds_FullName+14)
#define DEB_ASCII_BUF_cds_ReplicaPointer(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_cds_ReplicaPointer)

char *
deb_ascii_cds_AtomicValue (
    char              *,
    cds_AtomicValue_t *);
#define DEB_ASCII_LEN_cds_AtomicValue \
	DEB_ASCII_LEN_cds_ReplicaPointer
#define DEB_ASCII_BUF_cds_AtomicValue(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_cds_AtomicValue)

char *
deb_ascii_cds_SetMember (
    char            *,
    cds_SetMember_t *);
#define DEB_ASCII_LEN_cds_SetMember \
	DEB_ASCII_LEN_cds_AtomicValue
#define DEB_ASCII_BUF_cds_SetMember(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_cds_SetMember)

char *
deb_ascii_cds_Set (
    char      *,
    cds_Set_t *);
#define DEB_ASCII_MAX_cds_Set 4
#define DEB_ASCII_LEN_cds_Set \
	(DEB_ASCII_MAX_cds_Set*DEB_ASCII_LEN_cds_SetMember+9)
#define DEB_ASCII_BUF_cds_Set(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_cds_Set)

char *
deb_ascii_cds_Progress (
    char           *,
    cds_Progress_t *);
#define DEB_ASCII_LEN_cds_Progress \
	(DEB_ASCII_LEN_u_long+2*DEB_ASCII_LEN_cds_FullName+DEB_ASCII_LEN_cds_Set+46)
#define DEB_ASCII_BUF_cds_Progress(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_cds_Progress)

#endif  /* #ifndef _RPC_CVT_H */
