/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: uc_clerk.h,v $
 * Revision 1.1.9.2  1996/02/18  23:34:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:47  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:18:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:02  root]
 * 
 * Revision 1.1.7.4  1994/06/30  19:15:46  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:50:18  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  18:39:44  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:26  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/14  14:39:52  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:58:08  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:04:47  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:21:10  peckham]
 * 
 * Revision 1.1.5.2  1992/12/30  13:51:34  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:55:39  zeliff]
 * 
 * Revision 1.1.2.3  1992/07/06  20:14:33  mfox
 * 	Limit classnames to 31 characters to prevent server crash
 * 	[1992/07/06  20:10:55  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:12:46  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:29:22  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _UC_CLERK_H
#define _UC_CLERK_H
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
 *
 */
#include <dce/deb_ascii.h>


#define dns_status               cds_status
#define dns_status_clerk         cds_status_clerk
#define dns_status_unix          cds_status_unix
#define dns_validate_AttrType    cds_validate_AttrType
#define dns_validate_Entries     cds_validate_Entries
#define dns_validate_GrpMbrType  cds_validate_GrpMbrType
#define dns_validate_ReplicaType cds_validate_ReplicaType
#define dns_validate_fname       cds_validate_fname
#define dns_validate_fsConf      cds_validate_fsConf
#define dns_validate_fsMaybeMore cds_validate_fsMaybeMore
#define dns_validate_fsWait      cds_validate_fsWait
#define dns_validate_sname       cds_validate_sname
#define dns_validate_cname       cds_validate_cname

#include <dce/dnsclerk.h>
#include <dns_malloc.h>
#include <dce/dns_record.h>
#include <dce/id_base.h>

#ifndef	MAX_MSG_SIZE
# define MAX_MSG_SIZE		(16*1024)
#endif

/* Subcode for Op_Local operations */
#define op_creatects 1
#define op_noop      2
#define op_cacheid   3
 

typedef enum {				/* Type of post-processing */
	dns_retNone,			/*  initial value */
	dns_retAttrACE,			/*  parse AttributeContents/ACE */
	dns_retAttrCon,			/*  parse AttributeContents */
	dns_retReplCon,			/*  parse AttributeContents */
	dns_retAttrMbr,			/*  parse AttributeContents/GroupMbr */
	dns_retAttrSpc,			/*  parse set of AttributeSpecifiers */
	dns_retSetNam,			/*  parse set of SimpleNames */
	dns_retSetNamCls		/*  parse set of NameAndClass */
} dns_return_type_t;



typedef struct dns_ReadHandle {
    unsigned char	*dns_ReadBuf_p;        
    unsigned char	*dns_Context_p;
    int			(*dns_parse)(struct dnsFlagStat   *,
                                     dns_handle            );
    int 		dns_SetIndex;
    int                 dns_status;    /* may be stale indicator */
    dns_return_type_t   dns_Return;
    dns_attrvalue_t	dns_AttrValue;
    address_u		remote_handle;  /* PC implementation uses */
    unsigned char	dns_ReturnClass;
    unsigned char	dns_outLinked;	/* Saved value of outLinked */
    unsigned char	dns_fromCache;	/* Saved value of fromCache */
    unsigned char	dns_done;	/* if TRUE, this is last set */
    union {				/* Storage area for return pointers */
	struct {			/*  Return ACE data */
	    int 		*hcp_flags_p;
	    int 		*hcp_rights_p;
	    dns_full_name_t	**hcp_identifier_pp;
	    unsigned char	*hcp_identifier_p;
	    int 		*hcp_identifier_len_p;
	}		hcp_AttrACE;
	struct {			/*  Return GroupMember data */
	    dns_full_name_t	**hcp_member_pp;
	    enum dnsGrpMbrType	*hcp_isaGroup_p;
	    unsigned char	*hcp_member_p;
	    int 		*hcp_member_len_p;
	}		hcp_AttrMbr;
	struct {			/*  Return value pointer and length */
	    dns_attrvalue_t	*hcp_value_p;
	    dns_cts		**hcp_CTS_pp;
	}		hcp_AttrCon;
	struct {			/*  Return Replica Contents */
	    unsigned char	*hcp_value_type_p;
	    int			*hcp_value_len_p;
	    unsigned char	**hcp_value_pp;
	    dns_cts		**hcp_CTS_pp;
	}		hcp_ReplCon;
	struct {			/*  Return AttributeSpecifier */
	    dns_attr_name_t	**hcp_OpqName_pp;
	    enum dnsAttrType	*hcp_AttrType_p;
	    unsigned char	*hcp_StrName_p;
	    int 		*hcp_StrName_len_p;
	}		hcp_AttrSpc;
	struct {			/*  Return SimpleName */
	    dns_simple_name_t	**hcp_OpqName_pp;
	    unsigned char	*hcp_StrName_p;
	    int 		*hcp_StrName_len_p;
	}		hcp_SetNam;
	struct {			/*  Return SimpleName and ClassName */
	    dns_simple_name_t	**hcp_OpqName_pp;
	    unsigned char	*hcp_StrName_p;
	    int 		*hcp_StrName_len_p;
	    dns_simple_name_t	**hcp_OpqClass_pp;
	    unsigned char	*hcp_StrClass_p;
	    int 		*hcp_StrClass_len_p;
	    int 		*hcp_ReturnClass_p;
	}		hcp_SetNamCls;
    }			dns_hcp;
} dns_ReadHandle_t;

typedef struct {
    unsigned char	*chr_p;
    SimpleName_t	*snm_p;
    int 		length;
    int 		wildcardAllowed;
    int 		snType;
    int 		fnLength;
    int			isTyped;
    int			noTags;
    int			combine;
} dns_ToSimple_t;


/*
 * Name types for cds_strToSimple()
 */
typedef enum {
    CDS_LOCAL,		/* Name stored in a CDS server */
    CDS_TYPED,		/* X.500 typed name or CDS name */
    CDS_NOTLOCAL	/* X.500 or Domain Name System name */
} nametype_t;


/*
 * Use of fsOpqDNS
 *
 * NOTE: include assert() to insure that these are available.
 */
#define OPQLINK	0	/* queue link */
#define OPQSEQ	1	/* sequence number */
#define OPQMSGP	2	/* request message pointer */
#define OPQCND0	3	/* CMA - condition variable */
#define OPQCND1	4	/* CMA - also for condition variable */
#define OPQ_SIZ (sizeof(((dnsFlagStat_t *)NULL)->fsOpqDNS)/sizeof(((dnsFlagStat_t *)NULL)->fsOpqDNS[0]))

/*
 * The following prototypes are not in dnsclerk.h
 * The routines are used primarily by the server.
 */

/* dnscombine.c */

int
dnsCombine (
    dns_id		*,
    unsigned char	*,
    int			,
    dns_opq_fname	*,
    dns_cts		*,
    dns_cts		*,
    dns_id		*,
    dns_id		*,
    unsigned char	*,
    int			,
    struct dnsFlagStat	*);

/* dnscreatechild.c */


int
cdsCreateChild (
    dns_opq_fname	*,
    sec_id_foreign_t	*,
    dns_id		*,
    dns_opq_fname	*,
    int			,
    dns_id		*,
    struct dnsFlagStat	*);


/* dnsdelchild.c */

int
cdsDelChild (
    dns_opq_fname	*,
    struct dnsFlagStat	*);

/* dnsdoupdate.c */

int
cdsDoUpdate (
    unsigned char	*,
    int			,
    dns_id		*,
    unsigned char	*,
    int			,
    unsigned char	*,
    struct dnsFlagStat	*);

/* dnsgetinfo.c */

int
cdsGetInfo (
     long		*,
     struct dnsFlagStat	*);

/* dnslinkreplica.c */

int
cdsLinkReplica (
    dns_opq_fname	*,
    dns_opq_fname	*,
    dns_id		*,
    unsigned char	*,
    int			,
    dns_id		*,
    dns_id		*,
    dnsVersion_t	*,
    struct dnsFlagStat	*);

/* dnsmodifyreplica.c */

int
cdsModifyReplica (
    dns_id		*,
    unsigned char	*,
    int			,
    unsigned char	*,
    int			,
    struct dnsFlagStat	*);

/* dnsreadattrset.c */

int
cdsReadAttrSet (
    dns_opq_fname	*,
    unsigned char	*,
    dns_opq_sname	*,
    enum dnsEntries	,
    dns_opq_fname	*,
    unsigned char	*,
    int			,
    unsigned char	*,
    struct dnsFlagStat	*);

/* dnsreadreplica.c */


int
cdsReadReplica (
    dns_opq_fname	*,
    dns_id		*,
    dns_handle		,
    dns_attr_name_t	*,
    dns_opq_fname	*,
    unsigned char	*,
    unsigned char	**,
    int			*,
    dns_cts		**,
    unsigned char	*,
    int			*,
    struct dnsFlagStat	*);


/*
 * Internal function prototypes
 *
 * dnsbind.c
 */

int
cds_bind (
    void);

/*
 * dnshandleattrace.c
 * dnshandleattraddr.c
 * dnshandleattrcon.c
 * dnshandleattrmbr.c
 * dnshandleattrspc.c
 * dnshandlereplcon.c
 * dnshandlesetnam.c
 * dnshandlesetnamcls.c
 */

int
dns_handleAttrACE (
    dns_handle		,
    dns_full_name_t	**,
    int 		*,
    int 		*,
    unsigned char	*,
    int 		*,
    dnsFlagStat_t	*);

int
dns_handleAttrAddr (
    dns_handle		,
    unsigned char	**,
    dnsFlagStat_t	*);


int
cds_handleAttrCon (
    dns_handle		,
    dns_attrvalue_t	*,
    dns_cts		**,
    dnsFlagStat_t	*);


int
cds_handleAttrMbr (
    dns_handle		,
    dns_full_name_t	**,
    enum dnsGrpMbrType	*,
    unsigned char	*,
    int 		*,
    dnsFlagStat_t	*);

int
cds_handleAttrSpc (
    dns_handle		,
    enum dnsAttrType	*,
    dns_attr_name_t	**,
    unsigned char	*,
    int 		*,
    dnsFlagStat_t	*);

int
cds_handleReplCon (
    dns_handle		,
    unsigned char	*,
    unsigned char	**,
    int			*,
    dns_cts		**,
    dnsFlagStat_t	*);

int
cds_handleSetNam (
    dns_handle		,
    dns_simple_name_t	**,
    unsigned char	*,
    int 		*,
    dnsFlagStat_t	*);

int
cds_handleSetNamCls (
    dns_handle		,
    dns_simple_name_t	**,
    unsigned char	*,
    int 		*,
    int 		*,
    dns_simple_name_t	**,
    unsigned char	*,
    int 		*,
    dnsFlagStat_t	*);

/*
 * dnsnickname.c
 */

int
dns_nicknameToTimestamp (
    unsigned char	*,
    int			,
    unsigned char	*);

int
dns_nicknameToTimestamp_fast (
    unsigned char	*,
    int			,
    unsigned char	*,
    int			*);

int
dns_nicknameFromTimestamp (
    unsigned char	*,
    int			*,
    unsigned char	*,
    unsigned char	*);

unsigned char *
dns_localName (
    unsigned char	*,
    int			);

unsigned char *
dns_localRoot (
    void);

unsigned char *
dns_localName_fast (
    unsigned char	*,
    int			,
    int			*);

unsigned char *
dns_localRoot_fast (
    int			*);

/*
 * dnsopqtosimple.c
 */

int
dns_opqToSimple (
    dns_ToSimple_t	*);

/*
 * dnsread.c
 */

int
cds_read_push (
    dnsFlagStat_t	*);

void
cds_read (
    dnsFlagStat_t	*);

void
cds_read_pop (
    dnsFlagStat_t	*);

int
cds_unbind (
    void);

/*
 * dnssend.c
 */

int
cds_send (
    dnsFlagStat_t	*,
    byte_t		*);

/*
 * dnsstatus.c
 */

int
dns_status (
    dnsFlagStat_t	*);

/*
 * dnsstatusclerk.c
 */

int
dns_status_clerk (
    dnsFlagStat_t	*,
    int			);

/*
 * dnsstatusunix.c
 */

int
dns_status_unix (
    dnsFlagStat_t	*,
    int			);

/*
 * dnsstrtosimple.c
 */

int
dns_strToSimple (
    dns_ToSimple_t	*);

/*
 * dnsvalidate.c
 */

int
dns_validate_fsConf (
    enum dnsConf );

int 
dns_validate_VT_type (
    int);

int
dns_validate_fsWait (
    int );

int
dns_validate_fsMaybeMore (
    int );

int
dns_validate_AttrType (
    enum dnsAttrType );

int
dns_validate_Entries (
    enum dnsEntries );

int
dns_validate_GrpMbrType (
    enum dnsGrpMbrType );

int
dns_validate_ReplicaType (
    enum dnsReplicaType );

unsigned char *
dns_validate_sname (
    void *);

unsigned char *
dns_validate_fname (
    void *,
    int      );

/*
 * dnswrite.c
 */

void 
cdsInit (
    void);

int
cds_write (
    dnsFlagStat_t	*,
    byte_t		*,
    int			);

/*
 * dnsattrval.c
 */

int
dns_AttrVal2TLV (
    dns_attrvalue_t	*,
    unsigned char	*,
    int			*,
    void		**);

void
dns_TLV2AttrVal (
    int			,
    int			,
    void		*,
    dns_attrvalue_t	*);

/*
 * cdsstrtosimple..c
 */

int
cds_strToSimple (
    nametype_t		,
    dns_ToSimple_t	*);

/*
 * cdsopqtosimple.c
 */

int
cds_opqToSimple (
    dns_ToSimple_t      *);

/*
 * cdsstrings.c
 */

dns_full_name_t *
cds_fulltoopq (
    dns_full_name_t *,
    dns_full_name_t *,
    dnsFlagStat_t   *);

dns_simple_name_t *
cds_simpletoopq (
    dns_simple_name_t *,
    int               ,
    dns_simple_name_t *,
    dnsFlagStat_t     *);

dns_attr_name_t *
cds_attrtoopq (
    dns_attr_name_t *,
    dns_attr_name_t *,
    dnsFlagStat_t   *);

int
cds_convert_rpc_status (
    error_status_t	);



/*
 * dnsuser.c
 */

SimpleName_t *
dns_user_name (
    int         ,
    int         );

#endif  /* #ifndef _UC_CLERK_H */
