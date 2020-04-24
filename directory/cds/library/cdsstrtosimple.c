/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdsstrtosimple.c,v $
 * Revision 1.1.13.2  1996/02/18  19:32:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:09  marty]
 *
 * Revision 1.1.13.1  1995/12/08  15:20:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:45  root]
 * 
 * Revision 1.1.11.4  1994/06/30  19:16:26  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:38  mccann]
 * 
 * Revision 1.1.11.3  1994/06/09  18:40:40  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:07  devsrc]
 * 
 * Revision 1.1.11.2  1994/04/29  15:51:50  peckham
 * 	Rename dns*() to cds*().
 * 	[1994/04/29  13:59:31  peckham]
 * 
 * Revision 1.1.11.1  1994/03/12  22:05:41  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:24:39  peckham]
 * 
 * Revision 1.1.9.2  1993/10/14  21:27:28  jd
 * 	Tempory fix of compile problem.
 * 	[1993/10/14  21:26:40  jd]
 * 
 * Revision 1.1.9.1  1993/10/14  16:07:24  peckham
 * 	CR#3159 & CR#5925: handling of escapes & X.500 names.
 * 	Broke out individual scanning modes into seperate routines.
 * 	Pre-scan is done to determine type of name.
 * 	This version is not so lax as the previous, which would
 * 	allow illegal names to get through. There is still more
 * 	tightening to be done for 1.1.
 * 	[1993/10/10  22:07:30  peckham]
 * 
 * Revision 1.1.7.2  1993/08/27  21:06:07  jd
 * 	(8202) In cds_strToSimple check the parsed input simple component
 * 	agaist CDS_STR_SNAME_MAX not CDS_OPQ_SNAME_MAX, The later is two
 * 	bytes too large(sn_flag and sn_legth.
 * 	[1993/08/27  19:35:09  jd]
 * 
 * Revision 1.1.4.2  1992/12/30  13:53:32  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:06:39  zeliff]
 * 
 * Revision 1.1.2.4  1992/06/09  14:27:29  mfox
 * 	Fix following canonicalization bugs:
 * 	* Wasn't testing for error conditions of an imbalance of the number of
 * 	  commas and equal signs in a simple name.
 * 	* Wasn't converting any but the first tag in a multi-AVA RDN
 * 	* Was stepping on an internal table when converting an OID of the form 2.5.4.6
 * 	  to 85.4.6 -- the compressed form expected by IAPL* -- causing intermittent
 * 	  errors.
 * 	* Was allowing control characters inside of canonicalized names
 * 	[1992/06/09  14:23:56  mfox]
 * 
 * Revision 1.1.2.3  1992/06/01  15:10:07  m_sawyer
 * 	Fix bug where in_p was not being advanced if output buffer was
 * 	short in one case.
 * 	[1992/06/01  15:09:20  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/04/27  19:23:16  mfox
 * 	Allow un-escaped commas in RDNs for multi-AVA support in CDS
 * 	[1992/04/27  19:22:20  mfox]
 * 
 * Revision 1.1  1992/01/19  15:17:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE cdsstrtosimple.c
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
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_clerk.h>
#include <dce/assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/*
 * Local prototypes
 */
static dns_status_t
cdsstr_prescan (
    nametype_t,
    dns_ToSimple_t *);

static dns_status_t
cdsstr_normalToSimple (
    dns_ToSimple_t *);

static dns_status_t
cdsstr_wildToSimple (
    dns_ToSimple_t *);

static dns_status_t
cdsstr_bindToSimple (
    dns_ToSimple_t *);

static dns_status_t
cdsstr_typedToSimple (
    dns_ToSimple_t *);

static byte_t *
cdsstr_combine (
    char *,
    byte_t *);

/*
 * cds_strToSimple
 *	Converts next simple name
 *
 * Inputs:
 *	nametype = CDS_LOCAL - name syntax is intra-cell
 *		 = CDS_NOTLOCAL - syntax is DNS(BIND) or X.500
 *		 = CDS_TYPED - syntax is X.500 (has =) or intra-cell (no =)
 *	data_p->wildcardAllowed = if wildcard names allowed
 *	data_p->noTags = store ObjID, not tag, in opaque name
 *	data_p->combine = combine first two components (X*40 + Y) of ObjID
 *	data_p->chr_p = pointer to external name pointer
 *	data_p->snm_p = pointer to sname buffer
 *	data_p->length = remaining bytes in sname buffer
 *	data_p->fnLength = accumulated length of FullName
 *
 * Outputs:
 *	data_p->snType = SimpleName type for parsed name
 *	data_p->isTyped = TRUE if name is typed
 *	data_p->chr_p = points past characters parsed
 *	data_p->snm_p = points past converted internal name
 *	data_p->length = counted down for bytes used
 *	data_p->fnLength = incremented by bytes needed
 *
 * Value:
 *	(CDS_SUCCESS, CDS_INVALIDNAME)
 */
int
cds_strToSimple (
    nametype_t		 nametype,
    dns_ToSimple_t	*data_p)
{
    dns_status_t status;

    /*
     * Determine the type of name to scan.
     */
    status = cdsstr_prescan (nametype, data_p);
    if (status != CDS_SUCCESS) return(status);

    data_p->length -= sizeof(NULLSIMPLENAME_u);	/* initialize length */

    /*
     * Convert name depending on name type
     */
    if (data_p->snType == SN_cds) {
	if (data_p->isTyped) {
	    return cdsstr_typedToSimple(data_p);
	} else if (nametype == CDS_NOTLOCAL) {
	    return cdsstr_bindToSimple(data_p);
	} else {
	    return cdsstr_normalToSimple(data_p);
	}
    } else {
	dce_assert(cds__svc_handle,data_p->snType == SN_cdswildcard);
	return cdsstr_wildToSimple(data_p);
    }
}

/*
 * Do a pre-scan on the string to determine the characteristics
 * of this name
 *
 * Inputs:
 *	nametype = CDS_LOCAL - name syntax is intra-cell
 *		 = CDS_NOTLOCAL - syntax is DNS(BIND) or X.500
 *		 = CDS_TYPED - syntax is X.500 (has =) or intra-cell (no =)
 *	data_p->wildcardAllowed = if wildcard names allowed
 *	data_p->chr_p = pointer to external name pointer
 *
 * Outputs:
 *	data_p->snType = SimpleName type for parsed name
 *	data_p->isTyped = TRUE if name is typed
 *
 * Value:
 *	(CDS_SUCCESS, CDS_INVALIDNAME)
 */
static dns_status_t
cdsstr_prescan (
    nametype_t		nametype,
    dns_ToSimple_t	*data_p)
{
    register byte_t *in_p = data_p->chr_p;
    register byte_t c = in_p[0];

    data_p->snType = SN_cds;			/* and assume normal name */
    data_p->isTyped = FALSE;			/* not typed */
    /*
     * If not a local name, it could be typed - check for '='.
     */
    if (nametype != CDS_LOCAL) {
	/*
	 * Check if this name is typed by looking for a '='.
	 */
	while ((c != '/') && (c != '\0')) {
	    if (c == '\\') {
		if (in_p[1] == '\0')
		    return(CDS_INVALIDNAME);

		in_p++;
	    } else if (c == '=') {
		data_p->isTyped = TRUE;
		return(CDS_SUCCESS);
	    }
	    c = *++in_p;
	}
    }
    /*
     * If nametype does not exclude LOCAL, check for wildcard.
     */
    if (nametype != CDS_NOTLOCAL) {
	if (!data_p->wildcardAllowed)
	    return(CDS_SUCCESS);

	/*
	 * Check if this is a wildcarded name
	 */
	while ((c != '/') && (c != '\0')) {
	    if (c == '\\') {
		if (in_p[1] == '\0')
		    return(CDS_INVALIDNAME);

		in_p++;
	    } else if ((c == '?') || (c == '*')) {
		data_p->snType = SN_cdswildcard;
		return(CDS_SUCCESS);
	    }
	    c = *++in_p;
	}
    }
    return(CDS_SUCCESS);
}

/*
 * Convert normal ascii name to SimpleName
 *
 * Metacharacters:
 *  /  Seperate elements in a name
 *  \  Used to quote [/\*?]
 * Character set:
 *  a-z
 *  A-Z
 *  0-9
 *  (others)
 *
 * Inputs:
 *	data_p->chr_p = pointer to external name pointer
 *	data_p->snm_p = pointer to sname buffer
 *	data_p->length = remaining bytes in sname buffer
 *	data_p->fnLength = accumulated length of FullName
 *	data_p->snType = SimpleName type for parsed name
 *
 * Outputs:
 *	data_p->chr_p = points past characters parsed
 *	data_p->snm_p = points past converted internal name
 *	data_p->length = counted down for bytes used
 *	data_p->fnLength = incremented by bytes needed
 *
 * Value:
 *	(CDS_SUCCESS, CDS_INVALIDNAME)
 */
static dns_status_t
cdsstr_normalToSimple (
    dns_ToSimple_t	*data_p)
{
    register byte_t *in_p = data_p->chr_p;
    register byte_t *out_p = data_p->snm_p->sn_name;
    register int snLength = 0;

    /*
     * Copy characters until delimiter found looking for escapes
     */
    for (;; in_p++) {
	register byte_t c = in_p[0];
	/*
	 * Backslash means escape next character: spit out the '\' and
	 * make note of it. Only metacharacters can be escaped in V1.0
	 */
	if (c == '\\') {
	    switch (in_p[1]) {		/* metachars for CDS wildcards */
	    case '/':
	    case '\\':
	    case '?':
	    case '*':
		c = *++in_p;		/* skip the escape */
		break;
	    default:
		return(CDS_INVALIDNAME);
	    }
        } else if ((c == '/') || (c == '\0')) {
	    break;
	} else if (iscntrl(c) || (isspace(c) && (c != ' '))) {
	    /*
	     * Disallow control characters
	     */
	    return(CDS_INVALIDNAME);
	}
	/*
	 * Make sure we haven't become too long...
	 */
	if (SIMPLENAMEMAX < ++snLength)
	    return(CDS_INVALIDNAME);
	/*
	 * Copy this chacter
	 */
	if (0 <= --data_p->length)
	    *out_p++ = c;
    }
    data_p->chr_p = in_p;
    /*
     * Set simple name length and
     * update external name pointer past processed characters
     */
    data_p->fnLength += sizeof(NULLSIMPLENAME_u) + snLength;
    if (0 <= data_p->length) {
	INS8(data_p->snm_p->sn_flag, data_p->snType);
	INS8(data_p->snm_p->sn_length, snLength);
	data_p->snm_p = (SimpleName_t *)out_p;
    }
    return(snLength ? CDS_SUCCESS : CDS_INVALIDNAME);
}

/*
 * Convert wildcard ascii name to SimpleName
 *
 * Inputs:
 *	data_p->chr_p = pointer to external name pointer
 *	data_p->snm_p = pointer to sname buffer
 *	data_p->length = remaining bytes in sname buffer
 *	data_p->fnLength = accumulated length of FullName
 *	data_p->snType = SimpleName type for parsed name
 *
 * Outputs:
 *	data_p->chr_p = points past characters parsed
 *	data_p->snm_p = points past converted internal name
 *	data_p->length = counted down for bytes used
 *	data_p->fnLength = incremented by bytes needed
 *
 * Value:
 *	(CDS_SUCCESS, CDS_INVALIDNAME)
 */
static dns_status_t
cdsstr_wildToSimple (
    dns_ToSimple_t	*data_p)
{
    register byte_t *in_p = data_p->chr_p;
    register byte_t *out_p = data_p->snm_p->sn_name;
    register int snLength = 0;

    /*
     * Copy characters until delimiter found looking for escapes
     */
    for (;; in_p++) {
	register byte_t c = in_p[0];
	/*
	 * Backslash means escape next character: spit out the '\' and
	 * make note of it. Only metacharacters can be escaped in V1.0
	 */
	if (c == '\\') {
	    switch (in_p[1]) {		/* metachars for CDS wildcards */
	    case '?':
	    case '*':
		/*
		 * Make sure we haven't become too long...
		 */
		if (SIMPLENAMEMAX < ++snLength)
		    return(CDS_INVALIDNAME);

		if (0 <= --data_p->length)	/* pass the escape */
		    *out_p++ = c;
		c = *++in_p;
	      break;
	    default:
	      return(CDS_INVALIDNAME);
	    }
        } else if ((c == '/') || (c == '\0')) {
	    break;
	} else if (iscntrl(c) || (isspace(c) && (c != ' '))) {
	    /*
	     * Disallow control characters
	     */
	    return(CDS_INVALIDNAME);
	}
	/*
	 * Make sure we haven't become too long...
	 */
	if (SIMPLENAMEMAX < ++snLength)
	    return(CDS_INVALIDNAME);

	/*
	 * Copy this chacter
	 */
	if (0 <= --data_p->length)
	    *out_p++ = c;
    }
    data_p->chr_p = in_p;
    /*
     * Set simple name length and
     * update external name pointer past processed characters
     */
    data_p->fnLength += sizeof(NULLSIMPLENAME_u) + snLength;
    if (0 <= data_p->length) {
	INS8(data_p->snm_p->sn_flag, data_p->snType);
	INS8(data_p->snm_p->sn_length, snLength);
	data_p->snm_p = (SimpleName_t *)out_p;
    }
    return(snLength ? CDS_SUCCESS : CDS_INVALIDNAME);
}

/*
 * Convert bind ascii name to SimpleName
 *
 * Metacharacters:
 *  .  seperate elements of name
 * Character set:
 *  a-z
 *  A-Z
 *  0-9
 *  [._]
 *
 * Inputs:
 *	data_p->chr_p = pointer to external name pointer
 *	data_p->snm_p = pointer to sname buffer
 *	data_p->length = remaining bytes in sname buffer
 *	data_p->fnLength = accumulated length of FullName
 *	data_p->snType = SimpleName type for parsed name
 *
 * Outputs:
 *	data_p->chr_p = points past characters parsed
 *	data_p->snm_p = points past converted internal name
 *	data_p->length = counted down for bytes used
 *	data_p->fnLength = incremented by bytes needed
 *
 * Value:
 *	(CDS_SUCCESS, CDS_INVALIDNAME)
 */
static dns_status_t
cdsstr_bindToSimple (
    dns_ToSimple_t	*data_p)
{
    register byte_t *in_p = data_p->chr_p;
    register byte_t *out_p = data_p->snm_p->sn_name;
    register int snLength = 0;

    /*
     * Copy characters until delimiter found looking for escapes
     */
    for (;; in_p++) {
	register byte_t c = in_p[0];

        if (isupper(c)) {
	    c = _tolower(c);
	} else if ((c == '/') || (c == '\0')) {
	    break;
	} else if (iscntrl(c) || isspace(c)) {
	    /*
	     * Disallow control characters
	     */
	    return(CDS_INVALIDNAME);
	}
	/*
	 * Make sure we haven't become too long...
	 */
	if (SIMPLENAMEMAX < ++snLength)
	    return(CDS_INVALIDNAME);
	/*
	 * Copy this chacter
	 */
	if (0 <= --data_p->length)
	    *out_p++ = c;
    }
    data_p->chr_p = in_p;
    /*
     * Set simple name length and
     * update external name pointer past processed characters
     */
    data_p->fnLength += sizeof(NULLSIMPLENAME_u) + snLength;
    if (0 <= data_p->length) {
	INS8(data_p->snm_p->sn_flag, data_p->snType);
	INS8(data_p->snm_p->sn_length, snLength);
	data_p->snm_p = (SimpleName_t *)out_p;
    }
    return(snLength ? CDS_SUCCESS : CDS_INVALIDNAME);
}

/*
 * Convert typed ascii name to SimpleName
 *
 * Metacharacters:
 *  /  Seperate elements in a name
 *  ,  Seperate multiple type-value pairs in RND
 *  =  Seperates type from value
 *  \  Used to quote [/,=]
 * Character set:
 *  a-z
 *  A-Z
 *  0-9
 *  [.:,'+-=()?/ ]
 *
 * Inputs:
 *	data_p->chr_p = pointer to external name pointer
 *	data_p->snm_p = pointer to sname buffer
 *	data_p->length = remaining bytes in sname buffer
 *	data_p->fnLength = accumulated length of FullName
 *	data_p->snType = SimpleName type for parsed name
 *	data_p->noTags = store ObjID, not tag, in opaque name
 *	data_p->combine = combine first two components (X*40 + Y) of ObjID
 *
 * Outputs:
 *	data_p->chr_p = points past characters parsed
 *	data_p->snm_p = points past converted internal name
 *	data_p->length = counted down for bytes used
 *	data_p->fnLength = incremented by bytes needed
 *
 * Value:
 *	(CDS_SUCCESS, CDS_INVALIDNAME)
 */
static dns_status_t
cdsstr_typedToSimple (
    dns_ToSimple_t	*data_p)
{
    bytes_u name[1024+2];	/* maximum size for RDN ? */
    register byte_t *name_p = name;
    attrent_t *attr = NULL;
    register byte_t *in_p = data_p->chr_p;
    register byte_t *out_p = data_p->snm_p->sn_name;
    register int snLength = 0;

    /*
     * Copy characters until delimiter found looking for escapes
     */
    for (;; in_p++) {
	int escaped = FALSE;
	register byte_t c = in_p[0];
	/*
	 * Backslash means escape next character: spit out the '\' and
	 * make note of it. Only metacharacters can be escaped in V1.0
	 */
	if (c == '\\') {
	    switch (in_p[1]) {		/* metachars for typed names */
	    case '/':
	    case ',':
	    case '=':
	    case '\\':
		c = *++in_p;		/* skip the escape */
		escaped = TRUE;
		break;
	    default:
		return(CDS_INVALIDNAME);
	    }
        }
	/*
	 *
	 */
	if (!attr) {
	    /*
	     * Now accumulating the attribute type
	     *
	     * This MUST be terminated by '='.
	     */
	    if (!escaped) {
		switch (c) {
		case '=':
		    *name_p = '\0';

		    if (isdigit(name[0])) {
			/*
			 * User-supplied ObjID - verify legal
			 */
			attr = cdsGetXAttrByObjID(name);
			if (!attr) return(CDS_INVALIDNAME);
			name_p = (byte_t *)attr->tag;
		    } else {
			/*
			 * User-supplied Name - find corresponding ObjID
			 */
			attr = cdsGetXAttrByString(name);
			if (!attr) return(CDS_INVALIDNAME);
			if (data_p->noTags) {
			    if (data_p->combine) {
				name_p = cdsstr_combine(attr->objid, name);
			    } else
				name_p = (byte_t *)attr->objid;
			} else
			    name_p = (byte_t *)attr->tag;
		    }
		    /*
		     * Copy the name out
		     */
		    for (; name_p[0]; name_p++) {
			/*
			 * Make sure we haven't become too long...
			 */
			if (SIMPLENAMEMAX < ++snLength)
			    return(CDS_INVALIDNAME);

			if (0 <= --data_p->length)
			    *out_p++ = tolower(name_p[0]);
		    }
		    /*
		     * Make sure we haven't become too long...
		     */
		    if (SIMPLENAMEMAX < ++snLength)
			return(CDS_INVALIDNAME);

		    if (0 <= --data_p->length)
			*out_p++ = c;
		    continue;
		case '\0':
		case '/':
		case ',':
		    return(CDS_INVALIDNAME);
		}
	    }
	    if (iscntrl(c) || (isspace(c) && c != ' '))
		return(CDS_INVALIDNAME);
	    if (isupper(c)) {
		c = _tolower(c);
	    } else if (c == ' ') {
		/*
		 * Collapse spaces
		 */
		while (in_p[1] == ' ')
		    in_p++;
	    }
	    /*
	     * Make sure we don't overflow our buffer
	     */
	    if (&name[sizeof(name)-1] <= name_p)
		return(CDS_INVALIDNAME);
	    /*
	     * store next type character
	     */
	    *name_p++ = c;
	} else {
	    int finished = FALSE;

	    /*
	     * Now accumulating the attribute value
	     *
	     * This can be terminated by ',' or end-of-name.
	     */
	    if (!escaped) {
		switch (c) {
		case '\0':
		case '/':
		    finished = TRUE;
		    break;
		case ',':
		    /*
		     * Wrap up the attribute value
		     */
		    attr = NULL;
		    name_p = name;
		    break;
		}
		if (finished)
		    break;
	    }
	    if (c == ' ') {
		/*
		 * Collapse spaces
		 */
		while (in_p[1] == ' ')
		    in_p++;
	    } else if (iscntrl(c) || isspace(c)) {
		return(CDS_INVALIDNAME);
	    }
	    /*
	     * Change to lower case if required
	     */
	    if (isupper(c) && attr && (attr->matchingrule == CIM))
		c = _tolower(c);
	    /*
	     * Make sure we haven't become too long...
	     */
	    if (SIMPLENAMEMAX < ++snLength)
		return(CDS_INVALIDNAME);

	    /*
	     * Copy this chacter
	     */
	    if (0 <= --data_p->length)
		*out_p++ = c;
	}
    }
    data_p->chr_p = in_p;
    /*
     * Set simple name length and
     * update external name pointer past processed characters
     */
    data_p->fnLength += sizeof(NULLSIMPLENAME_u) + snLength;
    if (0 <= data_p->length) {
	INS8(data_p->snm_p->sn_flag, data_p->snType);
	INS8(data_p->snm_p->sn_length, snLength);
	data_p->snm_p = (SimpleName_t *)out_p;
    }
    return(snLength ? CDS_SUCCESS : CDS_INVALIDNAME);
}

/*
 * Converts an object identifier's 1st two subcomponents, X.Y, into
 * a single subcomponent equal to (X * 40) + Y (Ref: X.209, Sec. 22.4)
 *
 * Inputs:
 *	oid_p = pointer to null-terminated ObjID
 *	new_p = pointer to buffer to receive combined ObjID
 *
 * Outputs:
 *	new_p = pointer to buffer to receive combined ObjID
 *
 * Value:
 *	(CDS_SUCCESS, CDS_INVALIDNAME)
 */
static byte_t *
cdsstr_combine (
    char	*oid_p,
    byte_t	*new_p)
{
	char old[200];
	char *x = old, *y, *z;

	strcpy(old, oid_p);
	y = strchr(x, '.');
	if (y == NULL)
		return((byte_t *)oid_p);
	*y++ = 0;
	z = strchr(y, '.');
	if (z != NULL)
		*z++ = 0;
	sprintf((char *)new_p, "%d.", (atoi(x) * 40) + atoi(y));
	if (z != NULL)
		strcat((char *)new_p, z);
	return(new_p);
}
