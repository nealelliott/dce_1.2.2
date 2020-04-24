/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsnormal.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:29  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:25:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:32  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:24  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:18  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:00  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:12  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:30  peckham
 * 	Remove unnecessary conditionals.
 * 	Don't use dnstables.
 * 	[1994/04/29  14:25:00  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:20  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:31:23  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:58:16  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:13:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: normal.c
 * Version: X02.12
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdssvc.h>
#include <stdlib.h>
#include <ctype.h>
#include <dce/dns_record.h>
#include <dnsnormal.h>

/*
 * Dispatch table 
 *
 * The dispatch table is used to locate the routine specific
 * to a particular attribute type which will normalize a
 * value in the syntax defined for that attribute type. 
 *
 * The proper code here would be code which accesses the schema
 * to determine which syntax is defined for each attribute type.
 * Then, a table like this would be used to dispatch to a routine
 * based on the object identifier of the syntax, not the attribute
 * type. This code should be changed when the schema design is stable.
 *
 * In addition, the routines here can only handle well known
 * syntax types. Additional code must be added to support
 * arbitrary constructed types or new primitive types.
 *
 * Notice the absence of searchGuide, telexNumber,
 * teletexTerminalIdentifier, facsimileTelephoneNumber,
 * preferredDeliveryMethod, presentationAddress.
 * These attribute types are constructed and will not appear in names, I hope.
 * The normalization code could be written for them, but this probably
 * isn't necessary.
 *
 * Also notice the absence of X.400 attributes and security attributes.
 * These should be added if needed before the schema code.
 *
 * If an attribute type isn't here the normalization routine
 * defaults to code which checks the asn.1 tag. If its a string
 * tag the default is ignore case. If its not, the default is
 * byte by byte copy.
 */

/*  Attribute Object Identifiers encoded in ASN.1 BER */

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) objectClass(0)} */
static bytes_u objectClass[] = {0x6, 0x3, 0x55, 0x4, 0x0};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) aliasedObjectName(1)} */
static bytes_u aliasedObjectName[] = {0x6, 0x3, 0x55, 0x4, 0x1};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) knowledgeInformation(2)} */
static bytes_u knowledgeInformation[] = {0x6, 0x3, 0x55, 0x4, 0x2};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) commonName(3)} */
static bytes_u commonName[] = {0x6, 0x3, 0x55, 0x4, 0x3};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) surname(4)} */
static bytes_u surname[] = {0x6, 0x3, 0x55, 0x4, 0x4};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) serialNumber(5)} */
static bytes_u serialNumber[] = {0x6, 0x3, 0x55, 0x4, 0x5};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) countryName(6)} */
static bytes_u countryName[] = {0x6, 0x3, 0x55, 0x4, 0x6};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) localityName(7)} */
static bytes_u localityName[] = {0x6, 0x3, 0x55, 0x4, 0x7};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) stateOrProvinceName(8)} */
static bytes_u stateOrProvinceName[] = {0x6, 0x3, 0x55, 0x4, 0x8};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) streetAddress(9)} */
static bytes_u streetAddress[] = {0x6, 0x3, 0x55, 0x4, 0x9};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) organizationName(10)} */
static bytes_u organizationName[] = {0x6, 0x3, 0x55, 0x4, 0xa};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) organizationalUnitName(11)} */
static bytes_u organizationalUnitName[] = {0x6, 0x3, 0x55, 0x4, 0xb};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) title(12)} */
static bytes_u title[] = {0x6, 0x3, 0x55, 0x4, 0xc};
 
    /* {joint-iso-ccitt(2) ds(5) attributeType(4) description(13)} */
static bytes_u description[] = {0x6, 0x3, 0x55, 0x4, 0xd};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) businessCategory(15)} */
static bytes_u businessCategory[] = {0x6, 0x3, 0x55, 0x4, 0xf};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) postalAddress(16)} */
static bytes_u postalAddress[] = {0x6, 0x3, 0x55, 0x4, 0x10};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) postalCode(17)} */
static bytes_u postalCode[] = {0x6, 0x3, 0x55, 0x4, 0x11};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) postOfficeBox(18)} */
static bytes_u postOfficeBox[] = {0x6, 0x3, 0x55, 0x4, 0x12};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) physicalDeliveryOfficeName(19)} */
static bytes_u physicalDeliveryOfficeName[] = {0x6, 0x3, 0x55, 0x4, 0x13};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) telephoneNumber(20)} */
static bytes_u telephoneNumber[] = {0x6, 0x3, 0x55, 0x4, 0x14};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) x121Address(24)} */
static bytes_u x121Address[] = {0x6, 0x3, 0x55, 0x4, 0x18};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) internationalISDNNumber(25)} */
static bytes_u internationalISDNNumber[] = {0x6, 0x3, 0x55, 0x4, 0x19};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) registeredAddress(26)} */
static bytes_u registeredAddress[] = {0x6, 0x3, 0x55, 0x4, 0x1a};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) destinationIndicator(27)} */
static bytes_u destinationIndicator[] = {0x6, 0x3, 0x55, 0x4, 0x1b};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) supportedApplicationContext(30)} */
static bytes_u supportedApplicationContext[] = {0x6, 0x3, 0x55, 0x4, 0x1e};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) member(31)} */
static bytes_u member[] = {0x6, 0x3, 0x55, 0x4, 0x1f};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) owner(32)} */
static bytes_u owner[] = {0x6, 0x3, 0x55, 0x4, 0x20};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) roleOccupant(33)} */
static bytes_u roleOccupant[] = {0x6, 0x3, 0x55, 0x4, 0x21};

    /* {joint-iso-ccitt(2) ds(5) attributeType(4) seeAlso(34)} */
static bytes_u seeAlso[] = {0x6, 0x3, 0x55, 0x4, 0x22};

/* 
 * Prototypes for Normalization routines 
 */
static int
distinguishedName_nr(
		     byte_t 	*,
		     byte_t	*,
		     int     	*);

static int 
objectIdentifier_nr(
		    byte_t 	*,
		    byte_t	*,
		    int     	*);

static int 
caseIgnoreString_nr(
		    byte_t 	*,
		    byte_t	*,
		    int     	*);

static int 
printableString_nr(
		   byte_t 	*,
		   byte_t	*,
		   int     	*);

static int 
numericString_nr(
		 byte_t 	*,
		 byte_t		*,
		 int     	*);

static int 
caseIgnoreList_nr(
		    byte_t 	*,
		    byte_t	*,
		    int     	*);

static int 
telephoneNumber_nr(
		   byte_t 	*,
		   byte_t	*,
		   int     	*);

static int 
default_nr(
	   byte_t 	*,
	   byte_t	*,
	   int     	*);

/*
 * single entry in the dispatch table 
 */
typedef struct oi_table_entry 
{
    byte_t	*obj_id;	/* ptr to obj id encoded in ASN.1 BER */
    int		length;		/* length of obj id */
    int		(*routine) (byte_t *, 
			    byte_t *, 
			    int    *); /* pointer to normalization routine */
} oi_table_entry_t;

/*
 * Dispatch table - in most frequently to least frequently used order 
 */
static oi_table_entry_t oi_table[] = {
    {objectClass, 5, objectIdentifier_nr},
    {aliasedObjectName, 5, distinguishedName_nr},
    {knowledgeInformation, 5, caseIgnoreString_nr},
    {commonName, 5, caseIgnoreString_nr},
    {surname, 5, caseIgnoreString_nr},
    {serialNumber, 5, printableString_nr},
    {countryName, 5, printableString_nr},
    {localityName, 5, caseIgnoreString_nr},
    {stateOrProvinceName, 5, caseIgnoreString_nr},
    {streetAddress, 5, caseIgnoreString_nr},
    {organizationName, 5, caseIgnoreString_nr},
    {organizationalUnitName, 5, caseIgnoreString_nr},
    {title, 5, caseIgnoreString_nr},
    {description, 5, caseIgnoreString_nr},
    {businessCategory, 5, caseIgnoreString_nr},
    {postalAddress, 5, caseIgnoreList_nr},
    {postalCode, 5, caseIgnoreString_nr},
    {postOfficeBox, 5, caseIgnoreString_nr},
    {physicalDeliveryOfficeName, 5, caseIgnoreString_nr},
    {telephoneNumber, 5, telephoneNumber_nr},
    {x121Address, 5, numericString_nr},
    {internationalISDNNumber, 5, numericString_nr},
    {registeredAddress, 5, caseIgnoreList_nr},
    {destinationIndicator, 5, printableString_nr},
    {supportedApplicationContext, 5, objectIdentifier_nr},
    {member, 5, distinguishedName_nr},
    {owner, 5, distinguishedName_nr},
    {roleOccupant, 5, distinguishedName_nr},
    {seeAlso, 5, distinguishedName_nr},
    {0, 0, default_nr}
};



/* dns_normalize_fname
 *
 * Normalize a full name for comparisons.
 *
 * A full name can contain any combination of simple name
 * name types, binary, simple, quoted, or typed. For each
 * simple name, apply the proper normalization routine.
 *
 * Input:
 *	in_p = pointer to input fullname
 *	real_out_p = pointer to output fullname
 *      max_length = max space for output
 *
 * Return:
 *      CDS_SUCCESS - if ok
 *      CDS_INVALIDNAME - if error
 */
int 
dns_normalize_fname (FullName_t  *in_p,
                     FullName_t  *real_out_p,
                     int         max_length)
{
    FullName_u temp;
    FullName_t *out_p;
    register int size;
    int return_value = CDS_SUCCESS;

    if (EMPTY_FullName(in_p))
        COPY_FullName(in_p, real_out_p);
    else
    {
        register SimpleName_t *snamein_p = (SimpleName_t *)in_p->fn_name;
        SimpleName_t *snameout_p;
        int running_length;

        if (in_p == real_out_p) /* name might grow, can't copy in place */
            out_p = (FullName_t *)temp;
        else
            out_p = real_out_p;

        snameout_p = (SimpleName_t *)out_p->fn_name;
        COPY_ObjUID(in_p->fn_root, out_p->fn_root);
        INS16(out_p->fn_length, 0);
        running_length = max_length - sizeof(FullName_overhead);
	for (; (1==1); snamein_p = (SimpleName_t *)SKIP_SimpleName(snamein_p),
		     snameout_p = (SimpleName_t *)SKIP_SimpleName(snameout_p))
	{
	    switch (EXT8(snamein_p->sn_flag))
	    {
	    case SN_normal:
	    case SN_quoted:
	    case SN_wildcard:
		COPY_byte(snamein_p->sn_flag, snameout_p->sn_flag);
		COPY_byte(snamein_p->sn_length, snameout_p->sn_length);

		if (size = EXT8(snamein_p->sn_length))
		    {
		    byte_t *source = (byte_t *)snamein_p->sn_name;
		    byte_t *target = (byte_t *)snameout_p->sn_name;

		    do
			{
			*target++ = isupper(*source) ? *source : toupper(*source);
			}
		    while (source++, --size);
		    }

		continue;		/* for () loop */
	    case SN_binary:
            case SN_cds:
	    case SN_cdswildcard:
	    case SN_objectid:
	    case SN_ellipsis:
		COPY_SimpleName(snamein_p, snameout_p);
		continue;		/* for () loop */
	    case SN_typed:
                if ((return_value = dns_normalize_sname(snamein_p,
				snameout_p, running_length)) != CDS_SUCCESS)
		    break;
                running_length -= (EXT8(snameout_p->sn_length) +
                    sizeof(SimpleName_overhead));
		continue;		/* for () loop */
	    case SN_null:
		break;			/* out of for () loop */
	    default:
		return_value = CDS_INVALIDNAME;
	    }
	    break;
	}

        if (return_value == CDS_SUCCESS)
        {
            INS8(snameout_p->sn_flag, 0);
            INS8(snameout_p->sn_length, 0);
            INS16(out_p->fn_length, (bytes_u *)SKIP_SimpleName(snameout_p) -
                (bytes_u *)out_p->fn_name);
            if (in_p == real_out_p)
                COPY_FullName((FullName_t *)temp, real_out_p);
        }
    }
    return(return_value);
}

/* dns_normalize_sname
 *
 * Normalize a typed simple name for comparisons.
 *
 * Each different attribute may require a different normalization
 * routine. If a normalization routine cannot be found for a particular
 * attribute, the null normalization routine is used.
 *
 * It is assumed that the supplied typed name is valid. As a result
 * it is assumed that the object identifier value length can fit
 * in a longword.
 *
 * Input:
 *	in_p = pointer to input simplename
 *	real_out_p = pointer to output simplename
 *      running_length = space for output
 *
 * Return:
 *      CDS_SUCCESS - if OK
 *      CDS_INVALIDNAME - if bad name
 */
int 
dns_normalize_sname (SimpleName_t  *in_p,
                     SimpleName_t  *real_out_p,
                     int           running_length)
{
    register byte_t *source;		/* pointer to original name */
    register byte_t *target;		/* pointer to normalized name */
    register int value_length;		/* length of value part */
    int return_value = CDS_INVALIDNAME;

    switch (EXT8(in_p->sn_flag))
    {
    case SN_typed:
	{
	SimpleName_u temp;
	SimpleName_t *out_p;
        int obj_id_length;              /* length of obj id part */
        int max_length;
	int i;

	/* can't normalize in place */
	if (in_p == real_out_p)
	    out_p = (SimpleName_t *)temp;
	else
	    out_p = real_out_p;

        source = in_p->sn_name;
        target = out_p->sn_name;

        /* extract and normalize the object id from the source sname */
        switch (*source)
        {
	case 0x06:			/* can only normalize tag=6 */
            if (SIMPLENAMEMAX < running_length)
                max_length = SIMPLENAMEMAX;
            else
                max_length = running_length;

            obj_id_length = objectIdentifier_nr(source, target, &max_length);

            /* use extracted obj id to index the normalization routine tbl */
            if (obj_id_length == 0)
		break;

	    source = SKIP_bytes(source, max_length);   /* skip obj id */
	    target = SKIP_bytes(target, obj_id_length); /* skip obj id */

	    for (i = 0; oi_table[i].length != 0; i++) /* search for entry */
		if ((obj_id_length == (oi_table[i].length)) &&
		    EQ_bytes(out_p->sn_name, oi_table[i].obj_id, obj_id_length))
			break;

	    if (SIMPLENAMEMAX < running_length)
		max_length = SIMPLENAMEMAX - obj_id_length;
	    else
		max_length = running_length - obj_id_length;

	    value_length = oi_table[i].routine(source, target, &max_length);

	    if (value_length <= 0)
		break;

	    INS8(out_p->sn_length, obj_id_length + value_length);
	    INS8(out_p->sn_flag, SN_typed);

	    if (in_p == real_out_p)
		COPY_SimpleName(out_p, real_out_p);

	    return_value = CDS_SUCCESS;
	    break;
        }
	}
	break;
    case SN_normal:
    case SN_quoted:
    case SN_wildcard:
	COPY_byte(in_p->sn_flag, real_out_p->sn_flag);
	COPY_byte(in_p->sn_length, real_out_p->sn_length);

	if (value_length = EXT8(in_p->sn_length))
	    {
	    source = (byte_t *)in_p->sn_name;
	    target = (byte_t *)real_out_p->sn_name;

	    do
		{
		*target++ = isupper(*source) ? *source : toupper(*source);
		}
	    while (source++, --value_length);
	    }

	return_value = CDS_SUCCESS;
	break;
    case SN_binary:
    case SN_cds:
    case SN_cdswildcard:
    case SN_objectid:
    case SN_ellipsis:
    case SN_null:
	if (in_p != real_out_p)
	    COPY_SimpleName(in_p, real_out_p);

	return_value = CDS_SUCCESS;
	break;
    }

    return(return_value);
}

/* default_nr
 *
 * Normalize a value in ASN.1 BER whose syntax is unknown
 *	Check the tag. If the tag is the tag for a well known
 *	character string syntax, then do a case ignore normalization.
 *	Otherwise, do a byte by byte copy. 
 *
 * Input:
 *      in_p = pointer to input value
 *      out_p = pointer to output value
 *      max_length = max output length
 *
 * Output:
 *      max_length = length of parsed input
 *
 * Return:
 *      output length
 */
static int 
default_nr (byte_t  *in_p,
            byte_t  *out_p,
            int     *max_length)
{
    return 0;
}

/* caseIgnoreString_nr
 *
 * Normalize a value in ASN.1 BER caseIgnoreString syntax
 *
 * Input:
 *	in_p = pointer to input value
 *	out_p = pointer to output value
 *      max_length = max output length
 *
 * Output:
 *      max_length = length of parsed input
 *
 * Return:
 *      output length
 */
static int 
caseIgnoreString_nr (byte_t  *in_p,
                     byte_t  *out_p,
                     int     *max_length)
{
    return 0;
}

/* printableString_nr
 *
 * Normalize a value in ASN.1 BER printableString syntax
 *
 * Input:
 *	in_p = pointer to input value
 *	out_p = pointer to output value
 *      max_length = max output length
 *
 * Output:
 *      max_length = length of parsed input
 *
 * Return:
 *      output length
 */
static int 
printableString_nr (byte_t  *in_p,
                    byte_t  *out_p,
                    int     *max_length)
{
    return 0;
}

/* numericString_nr
 *
 * Normalize a value in ASN.1 BER numericString syntax
 *
 * Input:
 *	in_p = pointer to input value
 *	out_p = pointer to output value
 *      max_length = max output length
 *
 * Output:
 *      max_length = length of parsed input
 *
 * Return:
 *      output length
 */
static int 
numericString_nr (byte_t  *in_p,
                  byte_t  *out_p,
                  int     *max_length)
{
    return 0;
}

/* telephoneNumber_nr
 *
 * Normalize a value in ASN.1 BER telephoneNumber syntax
 *
 * Input:
 *	in_p = pointer to input value
 *	out_p = pointer to output value
 *      max_length = max output length
 *
 * Output:
 *      max_length = length of parsed input
 *
 * Return:
 *      output length
 */
static int 
telephoneNumber_nr (byte_t  *in_p,
                    byte_t  *out_p,
                    int     *max_length)
{
    return 0;
}

/* distinguishedName_nr
 *
 * Normalize a value in ASN.1 BER distinguishedName syntax
 *
 * Input:
 *	in_p = pointer to input value
 *	out_p = pointer to output value
 *      max_length = max output length
 *
 * Output:
 *      max_length = length of parsed input
 *
 * Return:
 *      output length
 */
static int 
distinguishedName_nr (byte_t  *in_p,
                      byte_t  *out_p,
                      int     *max_length)
{
    return 0;
}

/* objectIdentifier_nr
 *
 * Normalize a value in ASN.1 BER objectIdentifier syntax
 *
 * Input:
 *	in_p = pointer to input value
 *	out_p = pointer to output value
 *      max_length = max output length
 *
 * Output:
 *      max_length = length of parsed input
 *
 * Return:
 *      output length
 */
static int 
objectIdentifier_nr (byte_t  *in_p,
                     byte_t  *out_p,
                     int     *max_length)
{
    byte_t *source;              /* source of unnormalized obj id */
    byte_t *target;              /* where to put normalized obj id */
    int running_length = *max_length;   /* space left in target */
    unsigned long length;               /* length of the value part of TLV */
    int return_value;

    /* normalize tag */
    if (running_length-- <= 0)
        return 0;                       /* no room left */
    source = in_p;                      /* start after tag */
    source++;
    target = out_p;                     /* start at first byte */
    *target++ = 0x6;                    /* copy the tag */

    /* normalize length */
    if (*source & 0x80)         /* long or short form? */
    { /* long form length source */
        int length_length;
        length = 0;
        length_length = *source++ & 0x7f; /* number of octets in length */
        if (length_length > 4) return 0;  /* check for overflow */
        while (length_length != 0)      /* compute value len */
        {
            length = (length<<8) + *source++;
            length_length--;
        }

        if (length <= 127)      /* short form length target */
        {
            if (running_length-- <= 0) return 0; /* out of room */
            *target++ = length;          /* store new length */
        }
        else
        {                   /* long form length target */
            byte_t *length_ptr;
            length_ptr = target++;
            if (running_length-- <= 0) return 0; /* out of room */
            *length_ptr = 1;
            if (length > 0x00ffffff)
            { /* write out most significant byte */
                if (running_length-- <= 0) return 0;
                *target++ = length>>24;
                (*length_ptr)++;
            }
            if (length > 0x0000ffff)
            { /* write out next most significant byte */
                if (running_length-- <= 0) return 0;
                *target++ = (length>>16) | 0xff;
                (*length_ptr)++;
            }
            if (length > 0x000000ff)
            { /* write out next most significant byte */
                if (running_length-- <= 0) return 0;
                *target++ = (length>>8) | 0xff;
                (*length_ptr)++;
            }
            if (running_length-- <= 0) return 0;
            *target++ = length | 0xff; /* least significant byte */
        }
    }
    else
    {  /* short form length source and target */
        length = *source;
        if (running_length-- <= 0) return 0; /* out of room */
        *target++ = *source++;
    }

    /* normalize value - according to ASN.1, its must be normalized already */
    if (length > running_length) return 0;
    COPY_bytes(source, target, length);
    return_value = length + *max_length - running_length;
    *max_length = source - in_p + length;
    return return_value;
}

/* caseIgnoreList_nr
 *
 * Normalize a value in ASN.1 BER caseIgnoreList syntax
 *
 * Input:
 *	in_p = pointer to input value
 *	out_p = pointer to output value
 *      max_length = max output length
 *
 * Return:
 *      output length
 */
static int 
caseIgnoreList_nr (byte_t  *in_p,
                   byte_t  *out_p,
                   int     *max_length)
{
    return 0;
}
