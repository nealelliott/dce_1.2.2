/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dua_mvr.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:16  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:22:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:13  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:34:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:44:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:07:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:29:45  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:09:47  marrek
 * 	Introduce IA5_SYNTAX
 * 	[1993/02/01  17:49:48  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:11:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:39  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:27:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  13:15:44  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:03:14  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:49:30  melman]
 * 
 * $EndLog$
 */
#ifndef _DUA_MVR_H
#define _DUA_MVR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dua_mvr_rcsid[] = "@(#)$RCSfile: dua_mvr.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:49 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*
 *
 * Module:
 *	DUA
 * Description:
 *	This file contains:
 *	. data structures directly handled by the operations of the STUB module
 *	. a set of four functions to handle the two representations
 *	  of the object identifiers.
 *
 * Reference:
 *	ISO 9594 The Directory
 *		       (Geneva, March 1988)
 */

/* INCLUDES */

#include <DUA/std.h>
#include <DUA/gdsauth.h>

/**********************************************************************/
/*                                                                    */
/*       D E K L A R A T I O N E N     Daten                          */
/*                                                                    */
/**********************************************************************/

/*****  Local  *****/


/****************************************************************************
 * constants definition.
 ****************************************************************************/
#define ATT_ERROR	ACC_ATT_ERROR
#define NAME_ERROR	ACC_NAME_ERROR
#define SERV_ERROR	ACC_SERV_ERROR
#define REFERRAL	ACC_REFERRAL
#define ABAN_ERROR 	ACC_ABAN_ERROR
#define SEC_ERROR	ACC_SEC_ERROR
#define ABAN_FAILED	ACC_ABAN_FAILED
#define UPDATE_ERROR	ACC_UPDATE_ERROR

#define OPTIONAL_UNDEF 	 	0

/****************************************************************************
 * general data structures.
 ****************************************************************************/
/* presentation address */
typedef struct {
        Boolean         psel_defined;
        octet_string    psel;
        Boolean         ssel_defined;
        octet_string    ssel;
        Boolean         tsel_defined;
        octet_string    tsel;
        int             nb_nsap;
        octet_string    * nsap;
} dua_psap;

typedef struct {
	int		d2_nb_el;
	unsigned long	* d2_subids;
} d2t_oid;

typedef struct {
	int		nb;
	unsigned char 	* contents;
} dua_octet_string;

/* Two representations are acceptable at the DUA to handle
 * the object identifiers:
 * . as a list of long integers,
 * . as an octets string already containing the ASN1 encoded ``V'' field
 */
typedef struct {
	Boolean		is_oid_ints;
	union {
		asn1_oid		oid_asn1;
		d2t_oid			oid_ints;
	} contents;
} dua_oid;

#define dua_bit_string 	dua_octet_string /* for bit strings, nb represents
					  * the position of the last
					  * significant bit	     */
#define dua_utc_time 	string

typedef asn1_oid 		dua_att_type;

#define choice_equality 	1
#define choice_substring 	2
#define choice_greaterOrEqual 	3
#define choice_lessOrEqual	4
#define choice_approximateMatch	5

typedef struct {
	int mode;
	dua_att_type  value;
} crit_item;

typedef struct {
	int   nb_crit;
struct	criteria * content;
} crit_set;

#define criteria_type 	1
#define criteria_and	2
#define criteria_or	3
#define criteria_not	4

typedef struct criteria {
	int   mode;
	union {
		crit_item  * c_type;
		crit_set   * c_and_or;
	struct	criteria   * c_not;
	} value;
} criteria;

typedef struct {
	Boolean 	obj_defined;
	dua_att_type    obj;
	criteria        crit;
} guide;

#define graphic_set 	0   /* to be used for linearizing teletex_id */
#define control_set	1   /* to be used for linearizing teletex_id */
#define page_format	2   /* to be used for linearizing teletex_id */
#define misc_format	3   /* to be used for linearizing teletex_id */
#define private_set     4   /* to be used for linearizing teletex_id */

typedef struct {
    dua_octet_string   terminal;
    dua_octet_string   control;
    dua_octet_string   graphic;
    dua_octet_string   misc;
    dua_octet_string   page;
    dua_octet_string   private;
} teletex_id;

typedef struct {
	dua_octet_string t_number;
	dua_octet_string c_code;
	dua_octet_string answer;
} tlx_number;

typedef struct {
	dua_octet_string tel_nr;
	int G3_par;
} faxNumber;

#define NUMERIC 	1
#define PRINTABLE 	2
#define T61 		3

typedef struct str_PT_choice {
	int mode;    /* string can be Printable, T61  */
	string str_val;
} str_PT_choice;

typedef struct str_NP_choice {
	int mode;    /* string can be Numeric, Printable  */
	string str_val;
} str_NP_choice;

typedef struct {
	int 		   nb;
	str_PT_choice * v;
} t61_pr_seq;

typedef struct {
	int 		   nb_item;
	dua_octet_string * t61_item;
} t61_seq;

typedef struct {
	int 		   nb_item;
	dua_octet_string * print_item;
} print_seq;

#define any_delivery_method	0
#define mhs_delivery		1
#define physical_delivery	2
#define telex_delivery		3
#define teletex_delivery	4
#define g3_fax_delivery		5
#define g4_fax_delivery		6
#define ia5_terminal_delivery	7
#define videotex_delivery	8
#define telephone_delivery	9

typedef struct {
	int   nb_item;
	int * int_item;
} int_seq;  /* also used for perferredDeliveryMethod   */


typedef struct S_Attrs {
	str_NP_choice * c_name;       /* country Name */
	str_NP_choice * a_d_name;     /* administrativeDomainName */ 
	string    	n_addr;       /* NetworkAddress */
	string    	t_id;         /* TerminalIdentifier */
	str_NP_choice * p_d_name;     /* PrivateDomainName */
	string    	org_name;     /* OrganizationName */
	string    	u_id;         /* NumericUserIdentifier */
	struct P_Name * p_name;       /* Personal Name */
	struct O_U_Names * o_u_names; /* OrganisationUnitNames */
} S_Attrs;

typedef struct ORAddress {
	S_Attrs 	 s_attrs;      /* StandardAttributes */
	struct D_D_Attrs * d_d_attrs;  /* domain_defined_attributes */
	struct E_Attrs   * e_attrs;    /* extension_attributes   */
} ORAddress;

typedef union {
	asn1_field 		a_val;
	struct dua_dist_name  * name;
} OR_dist_name;

typedef struct ORName {
	ORAddress address;
	OR_dist_name  d_name;
} ORName;

typedef struct P_Name {   	/*  PersonalName  */
	string surname;
	string given_name;
	string initials;
	string g_qualifier;
} P_Name;

typedef struct O_U_Names {    /*  OrganizationalUnitNames  */
	int n;
#define  UB_ORG_UNITS	4
	string v[UB_ORG_UNITS]; /* PrintableString */
} O_U_Names;

typedef struct D_D_Attr  {  	/* DomainDefinedAttribute */
	string type;
	string value;
} D_D_Attr;

typedef struct D_D_Attrs  {	/* DomainDefinedAttributes  */
	int n;
#define   UB_DOM_DEF_ATTRS	4
	D_D_Attr v[UB_DOM_DEF_ATTRS]; /* PrintableString */
} D_D_Attrs;

#define PDCName      str_NP_choice	/* PhysicalDeliveryCountryName */

#define PostalCode   str_NP_choice

#define PDSName      str_NP_choice

typedef struct PDSParameter {
	string printable;
	string t61;
} PDSParameter;

typedef struct UnformattedPostalAddress {
	struct PostalAddress_Lines * p_address;
	string teletex_string;
} UnformattedPostalAddress;

typedef ORName ORNamePattern;

typedef struct DLSubmitPermission {
#define INDIVIDUAL	1
#define MEMBER_OF_DL	2
#define PATTERN_MATCH	3
#define MEMBER_OF_GROUP	4
	int typ;
	union {
		ORName individual;
		ORName member_of_dl;
		ORNamePattern pattern_match;
		OR_dist_name member_of_group;
	} v;
} DLSubmitPermission;

typedef struct ExtendedNetworkAddress {
#define  E163_ADDRESS_TYPE	1
#define  PSAP_ADDRESS_TYPE	2
	int typ;
	union {
		struct E163_4_address * e163_4_address;
		dua_psap * psap_address;
	} v;
} ExtendedNetworkAddress;

typedef struct PostalAddress_Lines {
	int n;
#define UB_POSTAL_STRING            30
#define   UB_PDS_ADDR_LINES	6
	string v[UB_PDS_ADDR_LINES];
} PostalAddress_Lines;

typedef struct E163_4_address {
	string number;
	string sub_address;
} E163_4_address;

#define TerminalType int

typedef union {
	string 			T_str; 		/* Teletex String */
	string			P_str; 		/* Printable String */
	string			N_str; 		/* Numeric String */
	ExtendedNetworkAddress	en_addr;	/* Extended Network Address */
	UnformattedPostalAddress up_addr;	/* Unformated Postal Address */
	PDSParameter 		pds_val;	/* PhysicalDeliverySystemPar. */
	TerminalType		ter_val;	/* Terminal Type */
	D_D_Attrs		dd_val; 	/* Teletex DomDefAttr */
	P_Name			pn_val;		/* Teletex Pers-Name  */
	O_U_Names		ou_val;		/* Teletex Org. Unit Names */
	str_NP_choice		NP_val;		/* PostalCode / PDCountryName */
	asn1_field 		a_val;
} E_att_value;

typedef struct E_Attr {		/* ExtensionAttribute */
	int 	e_type;		/*  ExtensionAttributeType */
	E_att_value e_value; 	/*  ExtensionAttributeValue */
} E_Attr;

typedef struct E_Attrs {	/* ExtensionAttributes */
	int n;
#define    UB_MAX_EXT_ATTRS	23
	E_Attr v[UB_MAX_EXT_ATTRS];
} E_Attrs;

typedef union {
	int 			int_value;
	string 			str_value;
	dua_utc_time		utc_time_value;
	dua_octet_string 	oct_value;
	struct dua_dist_name  * name_value;
	asn1_field		asn1_value;
	dua_psap 		psap_value;
	guide                   guide_value;
	faxNumber               fax_value;
	t61_pr_seq		post_value;
	t61_seq                 t61_seq_value;
	print_seq               pr_seq_value;
	tlx_number              tlx_value;
	teletex_id              ttxid_value;
	int_seq                 int_seq_value;
	ORName			or_name;
	ORAddress		or_address;
	DLSubmitPermission	dls_perm;
	dua_oid			oid_value;
	duaCertificate		certificate;
	duaCertPair		cert_pair;
	duaCertList		cert_list;
} int_att_value1;

typedef struct {
	int mode;
	int_att_value1 value;
} dua_att_value1;

typedef union {
	int 			int_value;
	string 			str_value;
	dua_utc_time		utc_time_value;
	dua_octet_string 	oct_value;
	struct dua_dist_name  * name_value;
	asn1_field		asn1_value;
	dua_psap 		psap_value;
	guide                 * guide_value;
	faxNumber             * fax_value;
	t61_pr_seq	      *	post_value;
	t61_seq               * t61_seq_value;
	print_seq             * pr_seq_value;
	tlx_number            * tlx_value;
	teletex_id            * ttxid_value;
	ORName		        or_name;
	ORAddress	        or_address;
	DLSubmitPermission      dls_perm;
	int_seq                 int_seq_value;
	dua_oid			oid_value;
	duaCertificate		certificate;
	duaCertPair		cert_pair;
	duaCertList		cert_list;
} int_att_value;

typedef struct {
	int mode;
	int_att_value value;
} dua_att_value;

typedef struct {
	dua_att_type 	type;
	dua_att_value 	value;
} dua_assertion;

typedef struct {
	int 		nb_assertions;
	dua_assertion 	* assertions;
} dua_rdname;

typedef struct dua_dist_name {
	int 		nb_rdnames;
	dua_rdname 	* rdnames;
} dua_dist_name;

typedef dua_dist_name dua_name;

#endif /* _DUA_MVR_H */
