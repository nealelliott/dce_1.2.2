/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdsclerk.h,v $
 * Revision 1.1.13.2  1996/02/18  23:33:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:03  marty]
 *
 * Revision 1.1.13.1  1995/12/08  15:14:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:27  root]
 * 
 * Revision 1.1.11.5  1994/09/21  20:08:21  mccann
 * 	fix length of string form of attribute name
 * 	[1994/09/21  20:06:02  mccann]
 * 
 * Revision 1.1.11.4  1994/06/30  19:15:30  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:37:34  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.11.3  1994/06/09  18:38:40  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:09  devsrc]
 * 
 * Revision 1.1.11.2  1994/04/14  14:39:43  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:57:20  peckham]
 * 
 * Revision 1.1.11.1  1994/03/12  22:02:20  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:12:05  peckham]
 * 
 * Revision 1.1.7.3  1992/12/30  13:28:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:45:54  zeliff]
 * 
 * Revision 1.1.7.2  1992/09/29  19:13:47  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:12:41  weisman]
 * 
 * Revision 1.1.4.2  1992/07/06  20:14:15  mfox
 * 	Limit classnames to 31 characters to prevent server crash
 * 	[1992/07/06  20:09:34  mfox]
 * 
 * Revision 1.1  1992/01/19  15:16:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CDSCLERK_H
#define _CDSCLERK_H
/*
 * 
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 *
 * NOTE:
 *	Any changes in this file MUST be reflected in
 *	dnsclerk.h, dnsclerk_cds.h, and dns_record.h
 *
 *
 */


#include <dce/utc.h>
#include <dce/dcecdsmsg.h>
#include <dce/dnsmessage.h>
#include <dce/nbase.h>

/* ============================================================ */
/*  Constants and Data Structures				*/
/* ============================================================ */

/* ============================================================ */
/* The following constants define the maximum lengths for	*/
/* various buffers and data structures.				*/
/* ============================================================ */

/* Internal data structure lengths, include overhead */
#define CDS_OPQ_SNAME_MAX	  257 	/* CDS Opaque Simple Name Max size.*/
#define CDS_OPQ_FNAME_MAX	  402	/* CDS Opaque Full Name Max size.*/
#define CDS_OPQ_ANAME_MAX	   33	/* CDS Opaque Attribute Name Max size.*/
#define CDS_OPQ_CNAME_MAX	   33	/* CDS Opaque Class Name Max size.*/
#define CDS_CTS_LEN_MAX		   14   /* CDS Creation Time Stamp Length Max*/
#define CDS_ID_LEN_MAX		   sizeof(uuid_t)      /* UUIDs */
#define CDS_TIMEOUT_MAX            (2*sizeof(utc_t))   /* CDS Timeout Length */

/* String names, Don't include null terminator */
#define CDS_STR_SNAME_MAX          255  /* CDS String Simple Name Max size */
#define CDS_STR_FNAME_MAX         1023  /* CDS String Full Name Max size */
#define CDS_STR_ANAME_MAX          120  /* CDS String Attribute Name Max size */
#define CDS_STR_CNAME_MAX           31  /* CDS String Class Name Max size */

/* The structures defined below that use these values can contain
 * names in either string format (null terminated) or opaque format.  
 * These sizes refect the max of those 2 lengths
 */
#define CDS_FNAME_MAX		 1024	/* Largest name */
#define CDS_SNAME_MAX		  257	/* Largest name */
#define CDS_ANAME_MAX		  121	/* Largest name */
#define CDS_CNAME_MAX		   33	/* Largest name */

/* ============================================================ */
/* The following constants are used by cdsMaxSize to represent  */
/* various CDS datatypes.            				*/
/* ============================================================ */

#define CDS_OPQ_SIMPLE_NAME	1	/* CDS Opaque Simple Name */
#define CDS_OPQ_FULL_NAME	2	/* CDS Opaque Full Name */

/* ============================================================ */
/* Typedefs used in the Portable Interface routines.		*/
/* ============================================================ */

/* ==================================== */
/* Creation Time Stamp			*/
/* ==================================== */

typedef struct cds_cts  {
	unsigned char	uc[CDS_CTS_LEN_MAX];
} cds_cts_t;

typedef uuid_t cds_id_t;

/* ==================================== */
/* CDS Handle				*/
/* ==================================== */

typedef unsigned char *cds_handle_t;

/* ==================================== */
/* CDS Full Name			*/
/* ==================================== */

typedef struct {
	unsigned char	uc[CDS_FNAME_MAX];
} cds_full_name_t;

/* ==================================== */
/* CDS Simple Name		*/
/* ==================================== */

typedef struct {
	unsigned char	uc[CDS_SNAME_MAX];
} cds_simple_name_t;

/* ==================================== */
/* CDS Attribute Name                   */
/* ==================================== */

typedef struct {
	unsigned char  uc[CDS_ANAME_MAX];
} cds_attr_name_t;

/* ==================================== */
/* CDS Timeout				*/
/* ==================================== */

typedef struct {
	utc_t expire;
	utc_t extend;
} cds_timeout_t;

/* ==================================== */
/* X.500 Attribute Entry		*/
/* ==================================== */

typedef struct {
	char		*objid;
	char		*tag;
	char		*ident;
	int		 syntax;
	int		 matchingrule;
} attrent_t;

/*
 * Syntaxes
 */

#define CES		1	/* caseExactStringSyntax */
#define CIS		2	/* caseIgnoreString */
#define PS		3	/* printableStringSyntax */
#define NS		4	/* numericStringSyntax */

/*
 * Matching Rules
 */

#define CEM		1	/* case-exact-string-matching */
#define CIM		2	/* case-ignore-string-matching */
#define PM		3	/* printable-string-matching */
#define NM		4	/* numeric-string-matching */

#define UNSPEC		0	/* Unspecified */

/* ============================================================ */
/* Manifest constants defining NULL pointers associated with 	*/
/* typedefs above and below.					*/
/* ============================================================ */

#define NULL_CDS_CTS		((cds_cts_t *) 0)
#define NULL_CDS_ID		((cds_id_t *) 0)
#define NULL_CDS_HANDLE		((cds_handle_t) 0)
#define NULL_CDS_OPQ_FNAME	((cds_full_name_t *) 0)
#define NULL_CDS_OPQ_SNAME	((cds_simple_name_t *) 0)
#define NULL_CDS_OPQ_ANAME	((cds_attr_name_t *) 0)
#define NULL_CDS_TIMEOUT	((cds_timeout_t *) 0)
#define NULL_CDSFLAGSTAT	((cdsFlagStat_t *) 0)

/* ============================================================ */
/* Version is a 2 byte number, one for major and                */ 
/* the other for minor.					        */
/* ============================================================ */

typedef struct cdsVersion {
   unsigned char                 cdsMajVer;
   unsigned char                 cdsMinVer;
   } cdsVersion_t;

/* ============================================================ */
/* Enumeration constants                                        */
/* ============================================================ */

typedef enum cdsAttrType {
    cdsEmptySet=1,                  /* An attribute set with 	*/
				    /* no members.		*/
    cdsSingle,                      /* An attribute of type 	*/
				    /* single.		 	*/
    cdsSet                          /* An attribute of type set */
    } cdsAttrType_t;

typedef enum cdsConf {
    cdsConfLow=1,                   /* Try local cache first.	*/
    cdsConfMed,                     /* Try any directory replica*/
    cdsConfHi                       /* Use only the master 	*/
				    /* replica.			*/
    } cdsConf_t;

typedef enum cdsEntries {   
    cdsDir=1,                       /* Hierarchy formed by      */
				    /* directories.		*/
    cdsObj,                         /* Objects contained in 	*/
				    /* directories.             */
    cdsChildPtr,                    /* Pointers to child 	*/
				    /* directories.		*/
    cdsSLink,                       /* Softlinks or alias 	*/
				    /* contained in directory.	*/
    cdsCH,                          /* CDS database name.	*/
    cdsAnyEntries,                  /* Any entry.		*/
    cdsDirOrObj                     /* Directory or Object	*/
    } cdsEntries_t;

typedef enum cdsGrpMbrType {
    cdsPrincipal=0,                 /* name of individual with access */
    cdsGroup                        /* name of group with access */
    } cdsGrpMbrType_t;
    
typedef enum cdsReplicaType {
     cdsMaster=1,                   /* Master copy of directory.*/
     cdsSecondary,     		    /* Writable copy of a 	*/
				    /* directory.		*/
     cdsReadOnly,                   /* Read only copy of a 	*/
			            /* directory.		*/
     cdsGda                         /* GDA referral		*/
     } cdsReplicaType_t;

typedef enum {
     cdsStrDCE=1,		    /* String DCE entry & attr names */
     cdsOpqDNA,			    /* Opaque DNA entry & attr names */
     cdsStrDNA,			    /* String DNA entry & attr names */
     cdsStrXDS			    /* String XDS entry & attr names */
     } cdsNameType_t;


/* ==================================== */
/* CDS Value Type			*/
/* ==================================== */

typedef enum {
	cds_none = 0,		/* NULL value */
	cds_long = 1,		/* signed 32-bit integer */
	cds_short = 2,		/* signed 16-bit */
	cds_small = 3,		/* signed 8-bit */
	cds_uuid = 4,		/* Universal unique identifier */
	cds_Timestamp = 5,	/* see above */
	cds_Timeout = 6,	/* see above */
	cds_Version = 7,	/* class & replica version */
	cds_char = 8,		/* counted string of unsigned characters */
	cds_byte = 9,		/* counted string of untranslated octets */
	cds_ReplicaPointer = 10, /* Internal */
	cds_GroupMember = 11,	/* Internal */
	cds_ParentPointer = 12,	/* Internal */
	cds_FullName = 13,	/* full name */
	cds_CHDirectory = 14,	/* Internal */
	cds_ASN1 = 15,
	cds_gdaPointer = 18	/* Internal */
} cds_syntax_t;

typedef struct {
	cds_syntax_t	val_syntax;		/* discriminator */
	union {
		char		val_small;	/* cds_small */
		short		val_short;	/* cds_short */
		long		val_long;	/* cds_long */
		struct {
			unsigned short	length;
			char	       *char_p;
		} val_char;			/* cds_char */
		struct {
			unsigned short	length;
			void	       *byte_p;
		} val_byte;			/* cds_byte,
						   cds_ReplicaPointer,
						   cds_GroupMember,
						   cds_Fullname,
						   cds_ParentPointer,
						   cds_CHDirectory,
						   cds_ASN1,
						   cds_gdaPointer */
		cds_timeout_t	val_timeout;	/* cds_Timeout */
		cdsVersion_t 	val_version;	/* cds_Version */
		cds_id_t	val_uuid;	/* cds_uuid */
		cds_cts_t	val_timestamp;	/* cds_Timestamp */
	} val_u;
} cds_attrvalue_t;

/* ==================================== */
/* CDS Replica Pointer			*/
/* ==================================== */

typedef struct {
	cdsReplicaType_t	 rp_type;	/* Master, secondary, r/o */
	cds_id_t		 rp_chid;	/* Clearinghouse ID */
	cds_full_name_t		 rp_chname;	/* Clearinghouse name */
	int			 rp_towerlen;	/* Length of tower set */
	void			*rp_towers;	/* Tower set */
} cds_replicaptr_t;

/* ============================================================ */
/* NOTE:  When using the structure cdsFlagStat, any flag which	*/
/*	  is defined as an IN field and which is initialized to */
/*        zero by the calling program will indicate that the    */
/*        default value be taken for that particular flag.	*/
/* ============================================================ */

typedef struct cdsFlagStat { 
    int                 fsMaybeMore; /* IN. non-zero means read all    */
				     /* attrs.			       */
    cdsConf_t		fsConf;      /* IN. Confidence. Default is     */
				     /* cdsConfLow.		       */
    int                 fsWait;      /* IN. Nbr of seconds to wait.    */
				     /* Default set through management */
    int 		fsDontCache; /* IN. If non-zero then clerk     */
				     /* won't cache the result	       */
    unsigned int        fsUser[5];   /* IN. User specified data        */

    cdsNameType_t	fsNameType;  /* IN. Names are opaque (DNA) or  */
				     /* string (DCE)		       */
    int			fsUnAuth;    /* IN. non-zero means to use      */
				     /* unauthenticated access         */
    int			fsTrustAll;  /* IN. non-zero means trust any   */
				     /* server			       */
    unsigned int        fsCacheTimeout; /* Seconds old data in cache can be */
    cds_full_name_t    *fsPartialRes;/* IN. Buffer for recognized part */
				     /* of an unknown name	       */
    int 		fsResLength; /* IN/OUT. Length of above buffer */
    int                 fsOutLink;   /* OUT. If non-zero a soft link   */
				     /* was traversed.		       */	
    int 		fsFromCache; /* OUT. If non-zero the results   */
				     /* were returned from the cache   */
    int                 fsLocStat;   /* OUT. Local system status.      */
    int                 fsCDSStat;   /* OUT. Architecture numbers.     */    
    unsigned long       fsOpqCDS[5]; /* OPAQUE - for use by library    */
} cdsFlagStat_t;
/* ============================================================ */
/* Group flag							*/
/* ============================================================ */

#define CDS_ACE_GROUP		0x8	/* This is a group.     */

/*
 * Now for the external definitions/prototypes
 */

int 
cdsAddAttrValue (
    cds_full_name_t	*,
    cds_attr_name_t	*,
    enum cdsAttrType	,
    enum cdsEntries	,
    cds_attrvalue_t	*,
    cds_cts_t		*,
    cdsFlagStat_t	*);

int 
cdsIntAddAttrValue (
    cds_full_name_t	*,
    cds_attr_name_t	*,
    enum cdsAttrType	,
    enum cdsEntries	,
    int			,
    unsigned char	*,
    int			,
    cds_cts_t		*,
    cdsFlagStat_t	*);

int 
cdsAddMbr (
    cds_full_name_t	*,
    cds_full_name_t	*,
    enum cdsGrpMbrType	,
    cdsFlagStat_t	*);

int 
cdsAddReplica (
    cds_full_name_t	*,
    cds_full_name_t	*,
    enum cdsReplicaType	,
    cdsFlagStat_t	*);

int 
cdsAllowCH (
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int
cdsCacheDir (
    cds_full_name_t	*,
    unsigned char       *,
    unsigned char      **,
    cdsFlagStat_t	*);

int
cdsCancel (
    void); 

void
cdsClearHandle (
    cds_handle_t	);

int 
cdsCmpCTS (
    cds_cts_t		*,
    cds_cts_t		*);

int 
cdsCmpFull (
    cds_full_name_t	*,
    cds_full_name_t	*);

int 
cdsCmpSimple (
    cds_simple_name_t	*,
    cds_simple_name_t	*);

int 
cdsCountSimple (
    cds_full_name_t	*);

int
cdsCreateCTS (
    cds_cts_t		*);

int 
cdsCreateDir (
    cds_full_name_t	*,
    cds_full_name_t	*,
    cds_id_t		*,
    cdsFlagStat_t	*);

int 
cdsCreateGrp (
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int 
cdsCreateObj (
    cds_full_name_t	*,
    cds_simple_name_t	*,
    cdsVersion_t	*,
    cds_id_t		*,
    cds_cts_t		*,
    cdsFlagStat_t	*);

int 
cdsCreateSLink (
    cds_full_name_t	*,
    cds_full_name_t	*,
    cds_timeout_t	*,
    cds_cts_t	        *,
    cdsFlagStat_t	*);

int 
cdsCvtBinaryToOpqSimple (
    unsigned char	*,
    int 		,
    cds_simple_name_t	*,
    int 		*);

int
cdsCvtFullToOpq (
    unsigned char	*,
    cds_full_name_t	*,
    int			*);

int
cdsCvtFullToStr (
    cds_full_name_t	*,
    unsigned char	*,
    int			*);

int
cdsCvtObjIDToStr (
    cds_attr_name_t	*,
    unsigned char	*,
    unsigned char	*,
    int			*,
    unsigned char	*,
    int			*);

int 
cdsCvtSimpleToStr (
    cds_simple_name_t	*,
    unsigned char	*,
    int 		*);


int 
cdsCvtSimpleToOpq (
    unsigned char	*,
    int 		,
    cds_simple_name_t	*,
    int 		*);

int
cdsCvtStrToObjID (
    unsigned char	*,
    unsigned char	*,
    unsigned char	*,
    int			*,
    cds_attr_name_t	*,
    int 		*,
    unsigned char	**);

int
cdsCvtStrToTower (
    char		*[],
    char		**);

int
cdsCvtStrToTs (
    unsigned char        *,
    cds_cts_t            *,
    unsigned char        **);

void
cdsTowerToStr (
    char		*,
    char		**);

void 
cdsCvtTsToStr (
    cds_cts_t       *,
    unsigned char   *);

int 
cdsDelDir (
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int 
cdsDelGrp (
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int 
cdsDelObj (
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int 
cdsDelSLink (
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int 
cdsDisallowCH (
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int 
cdsEnumAttr (
    cds_full_name_t	*,
    cds_handle_t        ,
    cdsEntries_t	,
    cdsAttrType_t	*,
    cds_attr_name_t	**,
    unsigned char	*,
    int 		*,
    cdsFlagStat_t	*);

int 
cdsEnumChild (
    cds_full_name_t	*,
    cds_handle_t	,
    cds_simple_name_t	*,
    cds_simple_name_t	**,
    unsigned char	*,
    int 		*,
    cdsFlagStat_t	*);

int 
cdsEnumGrp (
    cds_full_name_t	*,
    cds_handle_t	,
    cds_simple_name_t	*,
    cds_simple_name_t	**,
    unsigned char	*,
    int 		*,
    cdsFlagStat_t	*);

int 
cdsEnumObj (
    cds_full_name_t	*,
    cds_handle_t	,
    cds_simple_name_t	*,
    cds_simple_name_t	*,
    int 		*,
    cds_simple_name_t	**,
    unsigned char	*,
    int 		*,
    cds_simple_name_t	**,
    unsigned char	*,
    int 		*,
    cdsFlagStat_t	*);

int 
cdsEnumSLink (
    cds_full_name_t	*,
    cds_handle_t	,
    cds_simple_name_t	*,
    cds_simple_name_t	**,
    unsigned char	*,
    int 		*,
    cdsFlagStat_t	*);

int 
cdsExpandStrFull (
    unsigned char	*,
    unsigned char	*,
    int 		*);

void
cdsFreeHandle (
    cds_handle_t		);

int 
cdsGather (
    cds_full_name_t	*,
    cds_simple_name_t	*,
    cds_full_name_t	*,
    cds_full_name_t	*,
    int 		*);

cds_handle_t
cdsGetHandle (
    void);

int 
cdsGetMsg (
    int 		,
    char		*,
    int 		*);

int 
cdsGetOpqFullLen (
    cds_full_name_t	*);

int 
cdsGetOpqSimpleLen (
    cds_simple_name_t	*);

attrent_t *
cdsGetXAttrByObjID (
    unsigned char	*);

attrent_t *
cdsGetXAttrByString (
    unsigned char	*);

attrent_t *
cdsGetXAttrEntry (
    void);

int 
cdsMaxSize (
    int 		);

int 
cdsNewEpoch (
    cds_full_name_t	*,
    cds_full_name_t	*,
    cds_full_name_t	*[],
    cds_full_name_t	*[],
    cdsFlagStat_t	*);

int 
cdsReadAttrValue (
    cds_full_name_t	*,
    cds_handle_t	,
    cds_attr_name_t	*,
    enum cdsEntries	,
    cds_full_name_t     *,
    cds_attrvalue_t	**,
    cds_cts_t		**,
    unsigned char       *,
    int 		*,
    cdsFlagStat_t	*);

int 
cdsReadMbr (
    cds_full_name_t	*,
    cds_handle_t        ,
    cds_full_name_t	**,
    enum cdsGrpMbrType	*,
    unsigned char	*,
    int 		*,
    cdsFlagStat_t	*);

int 
cdsResolveName (
    cds_full_name_t	*,
    cds_full_name_t	*,
    int 		*,
    cdsFlagStat_t	*);

int 
cdsRmAttr (
    cds_full_name_t	*,
    cds_attr_name_t	*,
    cdsAttrType_t	,
    cdsEntries_t	,
    cds_cts_t           *,
    cdsFlagStat_t	*);

int 
cdsRmAttrValue (
    cds_full_name_t	*,
    cds_attr_name_t	*,
    cdsEntries_t	,
    cds_attrvalue_t	*,
    cds_cts_t		*,
    cdsFlagStat_t	*);

int 
cdsIntRmAttrValue (
    cds_full_name_t	*,
    cds_attr_name_t	*,
    enum cdsEntries	,
    int			,
    unsigned char	*,
    int			,
    cds_cts_t		*,
    cdsFlagStat_t	*);

int 
cdsRmMbr (
    cds_full_name_t	*,
    cds_full_name_t	*,
    enum cdsGrpMbrType	,
    cdsFlagStat_t	*);

int 
cdsRmReplica (
    cds_full_name_t	*,
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int 
cdsScatter (
    cds_full_name_t	*,
    int 		,
    cds_full_name_t	*,
    int 		*,
    cds_simple_name_t	*,
    int 		*,
    cds_full_name_t	*,
    int 		*);

int 
cdsSkulkDir (
    cds_full_name_t	*,
    cdsFlagStat_t	*);

int 
cdsTestAttrValue (
    cds_full_name_t	*,
    cds_attr_name_t	*,
    enum cdsEntries	,
    cds_attrvalue_t	*,
    cdsFlagStat_t	*);

int
cdsIntTestAttrValue (
    cds_full_name_t     *,
    cds_attr_name_t     *,
    enum cdsEntries     ,
    int			,
    unsigned char	*,
    int			,
    cdsFlagStat_t       *);

int 
cdsTestMbr (
    cds_full_name_t	*,
    cds_full_name_t	*,
    int                 *,
    cds_timeout_t	*,
    cds_id_t            *,
    cdsFlagStat_t	*);

#endif  /* #ifndef _CDSCLERK_H */
