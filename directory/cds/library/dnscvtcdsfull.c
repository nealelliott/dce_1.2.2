/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscvtcdsfull.c,v $
 * Revision 1.1.12.2  1996/02/18  19:33:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:40  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:22:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:45  root]
 * 
 * Revision 1.1.10.5  1994/07/06  20:43:37  ohara
 * 	include stido.h
 * 	[1994/07/06  20:06:32  ohara]
 * 
 * Revision 1.1.10.4  1994/06/30  19:16:48  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:02  mccann]
 * 
 * Revision 1.1.10.3  1994/06/09  18:41:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:33  devsrc]
 * 
 * Revision 1.1.10.2  1994/04/29  15:53:00  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:17:26  peckham]
 * 
 * Revision 1.1.10.1  1994/03/12  22:06:22  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:27:32  peckham]
 * 
 * Revision 1.1.8.4  1993/10/25  14:22:54  peckham
 * 	CR#9224: cdscp show cell core dump.
 * 	Fix new code so that "/" is not accepted by cdsCvtFullToOpq().
 * 	It must still be acceptable by dns_CvtCDSFullToOpq() for progress
 * 	records where partial_names are allowed.
 * 	[1993/10/25  14:01:53  peckham]
 * 
 * Revision 1.1.8.3  1993/10/21  13:07:24  peckham
 * 	CR#9193: Allow trailing slashes.
 * 	This is what I get for trying to conform to spec...
 * 	[1993/10/21  02:18:46  peckham]
 * 
 * Revision 1.1.8.2  1993/10/20  20:29:07  peckham
 * 	CR#4320: Fix ":" to go to configurable junction point.
 * 	Added code under DNS_DFS contional to pick up DFS
 * 	junction point via dfs_GetJunctionName().
 * 	[1993/10/20  20:28:40  peckham]
 * 
 * Revision 1.1.8.1  1993/10/14  16:07:27  peckham
 * 	CR#3159 & CR#5925: handling of escapes & X.500 names.
 * 	Remove DNS_CMA, DOSorOS2 conditionals.
 * 	Remove use of dnstables.h definitions.
 * 	Bypass multiple slashes on front of name.
 * 	Strip multiple slashes here instead of simplename conversion.
 * 	[1993/10/10  22:01:29  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  13:55:21  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:09:31  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/09  21:35:46  mfox
 * 	Missing else clause didn't catch names beginning with /../ as illegal
 * 	[1992/12/09  21:33:58  mfox]
 * 
 * Revision 1.1.2.3  1992/06/09  14:50:03  m_sawyer
 * 	Fix usage of non-thread-safe static in cds_parse_names().
 * 	[1992/06/09  14:49:10  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/05/12  14:42:53  m_sawyer
 * 	Fix boundary condition problem when parsing "/:" alias
 * 	[1992/05/12  14:41:50  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:19:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscvtcdsfull.c
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

  
#include <uc_clerk.h>
#include <stdio.h>
#include <dns_cfg.h>
#include <dce/dce.h>
#include <dce/dce_cf.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <dce/cdsclerk.h>


/* local structures */

typedef struct {
    dns_ToSimple_t d2s;
    FullName_t	*full_p;
    char	*cellname_p;
    int		cellname_len;
    int		global_found;
    int		global_append;
    int		partial_name;
    int		state;
} data_t;

/* local data */

/* local routines */

static int
cds_cvtCDSFullToOpq (
    data_t		*);

static int
cds_parse_names (
    data_t		*);


/*
 * Strategy: The syntax of names is pretty trivial:
 *
 * NOTE: this table has not been updated!
 *
 * The state machine is simply:
 *
 * state	input		new state
 *   1		'/.:'		->  2		Strip off 1st component
 *						and replace with cellname
 *		'/:'		->  2		Strip off 1st component
 *						and replace with cellname/fs
 *              '/...'		->  2
 *              default		->  2		Prepend local cellname
 *						(all) Append /...
 *		default		->  5		If local cellname stripped
 *						or unavailable.
 * -------------------------------------
 *   2		"..."		->  3		Canonicalize "..."
 * -------------------------------------
 *   3	 	typed name	->  4		Canonicalize typed name
 *		default		->  5		Canonicalize DNS name
 * -------------------------------------
 *   4		typed name	->  4		Canonicalize typed name
 *		default		->  5		Append SimpleName
 * -------------------------------------
 *   5		<SN_cds>	->  5		Append SimpleName
 *		<SN_cdswildcard>->  5		Append SimpleName
 */

/* ----------------------------------------------------------------------
 *  Convert CDS string name to Opaque FullName
 *  Changes made here might have to be reflected in dnscvtstrfulltoopq.c!
 * ----------------------------------------------------------------------
 */
int 
cdsCvtFullToOpq (unsigned char    *StringFname_p,
                 cds_full_name_t  *OpaqueFname_p,
                 int              *OpaqueFnameLen_p)
{
    data_t	data;
    register int status;

    /*
     * Verify OpaqueFname/OpaqueFnameLen combination
     */
    if (data.full_p = (FullName_t *)OpaqueFname_p) {
	if (!OpaqueFnameLen_p) return(CDS_INVALIDARGUMENT);

	data.d2s.snm_p = (SimpleName_t *)data.full_p->fn_name;
	data.d2s.length = *OpaqueFnameLen_p;
    } else {
	if (OpaqueFnameLen_p)
	    *OpaqueFnameLen_p = 0;

	data.d2s.length = 0;
    }
    data.state = 1;
    data.d2s.noTags = 0;
    data.d2s.combine = 0;
    data.d2s.chr_p = StringFname_p;
    data.global_append = 1;
    data.partial_name = 0;
    data.cellname_p = 0;
    status = cds_cvtCDSFullToOpq(&data);
    /*
     * Pass back the computed length
     */
    if (OpaqueFnameLen_p)
	*OpaqueFnameLen_p -= data.d2s.length;

    return(status);
}

/*
 * Internal convert for server - strip global name and report.
 *
 * Misuse OpaqueFnameLen to return info on supplied name:
 *	 0 if name was a local name
 *	-1 if supplied cellName stripped off global name
 *	 1 if global name !~ cellName
 */
int 
dns_CvtCDSFullToOpq (unsigned char    *StringFname_p,
                     cds_full_name_t  *OpaqueFname_p,
                     int              *OpaqueFnameLen_p,
                     char             *CellName_p)
{
    data_t	data;
    register int status;

    if (data.cellname_p = CellName_p) {
	while (*CellName_p == '/') CellName_p++;
	data.cellname_len = strlen(data.cellname_p = CellName_p);
    }
    data.state = 1;
    data.global_append = 0;
    data.partial_name = 1;
    data.global_found = 0;
    data.full_p = (FullName_t *)OpaqueFname_p;
    data.d2s.noTags = 0;
    data.d2s.combine = 0;
    data.d2s.snm_p = (SimpleName_t *)data.full_p->fn_name;
    data.d2s.length = *OpaqueFnameLen_p;
    data.d2s.chr_p = StringFname_p;
    if ((status = cds_cvtCDSFullToOpq(&data)) == DNS_SUCCESS) {
	/*
	 * Indicate if global name supplied
	 */
	if (!data.global_found)
	    *OpaqueFnameLen_p = 0;
	else
	    *OpaqueFnameLen_p = 1;
    } else
	ZERO_FullName(OpaqueFname_p);

    return(status);
}

/*
 * Internal convert for gda & xds - replace tags with objids & stop when
 * non-X500 portion found.
 *
 * IN:	StringFname_p	   Pointer to string name
 *	Combine		   Non-zero to convert X.Y to (X * 40) + Y
 *	NoRoot		   Non-zero when called by GDA after stripping /...
 *	OpaqueFnameLen_p   Len of buffer for returned opq. name
 *
 * OUT:	OpaqueFname_p	   Caller-allocated buffer to receive opq. name
 *	OpaqueFnameLen_p   Len of returned opq. name
 *
 * RET: CDS_SUCCESS if conversion succeeds
 *	Various errors otherwise
 */
int 
dns_CvtCDSFullToGDS (unsigned char    *StringFname_p,
                     int              Combine,
                     int              NoRoot,
                     cds_full_name_t  *OpaqueFname_p,
                     int              *OpaqueFnameLen_p)
{
    data_t	data;
    register int status;

    /*
     * Initialize state to 3 if we have a name that doesn't begin
     * with /... or /.../ -- it begins with the 2nd component of the
     * global name. Otherwise start in state 2.
     * This is because the GDA strips the /... off before it calls
     * this function whereas XDS/CDS APIs call it with /... 
     */
    data.state = (NoRoot) ? 3 : 2;
    data.global_append = 0;
    data.partial_name = 0;
    data.global_found = 0;
    data.full_p = (FullName_t *)OpaqueFname_p;
    data.d2s.noTags = 1;
    data.d2s.combine = Combine;
    data.d2s.snm_p = (SimpleName_t *)data.full_p->fn_name;
    data.d2s.length = *OpaqueFnameLen_p;
    data.d2s.chr_p = StringFname_p;
    status = cds_cvtCDSFullToOpq(&data);
    return(status);
}

/*
 * Common initialization
 */
static int 
cds_cvtCDSFullToOpq (data_t *data_p)
{
    register int status;

    /*
     * Parse SimpleNames
     */
    data_p->d2s.length -= sizeof(NULLFULLNAME_u);
    data_p->d2s.wildcardAllowed = 0;
    data_p->d2s.fnLength = 0;
    /*
     * Parse the name string
     */
    if (0 <= data_p->d2s.length)
	COPY_ObjUID(cdsWildRoot, data_p->full_p->fn_root);

    status = cds_parse_names(data_p);

    if (*data_p->d2s.chr_p)		/* must be NULL terminated */
	status = CDS_INVALIDNAME;	/* unrecognized character */
    /*
     * Append trailing NullSimpleName and set FullName length.
     */
    if (0 <= data_p->d2s.length) {
	COPY_SimpleName(NullSimpleName, data_p->d2s.snm_p);
	data_p->d2s.fnLength += sizeof(NULLSIMPLENAME_t);
	INS16(data_p->full_p->fn_length, data_p->d2s.fnLength);
	if (sizeof(data_p->full_p->fn_name) < data_p->d2s.fnLength)
	  status = CDS_INVALIDNAME;
    } else if (data_p->full_p && (status == CDS_SUCCESS))
	status = CDS_NOROOM;

    return(status);
}

/*
 * Try to get our cellname from the cell config file
 */
static char *cell_name = NULL;
static pthread_once_t get_cell_name_once;

static void 
get_cell_name (void)
{
    error_status_t status;

    dce_cf_get_cell_name(&cell_name, &status);
    if (status != 0)
    {
        cell_name = NULL;
    }
}

/*
 * Parse SimpleNames
 */
static int 
cds_parse_names (data_t *data_p)
{
    int		status;

    for (;;) {
	register byte_t *cp = data_p->d2s.chr_p;
	/*
	 * Strip off slashes
	 */
	while (*cp == '/')
	    cp++;

	data_p->d2s.chr_p = cp;

	if (!*cp && data_p->state != 1)
	    break;

	switch (data_p->state) {
	case 1:
	    /*
	     * If name is global root, then it is "..." or begins ".../"
	     */
	    if (cp[0] == '.') {
		if (cp[1] == '.') {
		    if ((cp[2] == '.') && (!cp[3] || (cp[3] == '/'))) {
			data_p->global_found = 1;
			/*
			 * Append global root to fullname
			 */
			data_p->state = 2;
			data_p->d2s.chr_p = cp;
			break;
		    } else
			/*
			 * invalidate /../
			 */
			return(CDS_INVALIDNAME);
		} else if ((cp[1] == ':') && (!cp[2] || (cp[2] == '/'))) {
		    /*
		     * strip off optional local root ".:"
		     */
		    cp += 2;
		} else if (!data_p->partial_name) {
		    /*
		     * Don't prepend local cellname
		     */
		    return(CDS_INVALIDNAME);
		}
	    } else if ((cp[0] == ':') && (!cp[1] || (cp[1] == '/'))) {
#if defined(DNS_DFS)
		char *bufp;
		unsigned long dfsStatus;
		const int bufsiz = 128;
		/*
		 * Substitute "/:" -> DFS junction
		 */
		if (!cell_name) {
		    pthread_once (&get_cell_name_once, get_cell_name);
		    if (!cell_name) {
			/* should return bad status here */
			data_p->state = 5;
			break;
		    }
		}

		bufp = malloc(bufsiz);
		if (!bufp) return(CDS_NONSRESOURCES);

		dfs_GetJunctionName(cell_name, 0, bufp, bufsiz, &dfsStatus);
		if (dfsStatus == 0) {
		    /* The junction name is cell-relative */
		    data_p->d2s.chr_p = (byte_t *)cell_name;
		    status = cds_parse_names(data_p);
		    if (status == CDS_SUCCESS) {
			data_p->d2s.chr_p = (byte_t *)bufp;
			status = cds_parse_names(data_p);
		    }
		    data_p->d2s.chr_p = &cp[1];
		} else
		    status = dfsStatus;

		free(bufp);
#else
		/*
		 * Substitute "/:" -> "/.:/fs"
		 */
		data_p->d2s.chr_p = (byte_t *)"/.:/fs";
		status = cds_parse_names(data_p);
		data_p->d2s.chr_p = &cp[1];
#endif
		if (status != CDS_SUCCESS) return(status);
		/*
		 * resume parsing supplied name.
		 */
		break;
	    } else if (!data_p->partial_name) {
		/*
		 * Don't prepend local cellname
		 */
		return(CDS_INVALIDNAME);
	    } else if (!cp[0]) {
		/*
		 * This is the root-root "/".
		 */
		return(CDS_SUCCESS);
	    }
	    /*
	     * If stripping global name, don't pre-pend.
	     */
	    if (!data_p->global_append) {
		data_p->state = 5;
		continue;
	    }
	    /*
	     * Default to the local cell if we can.
	     */
	    if (!cell_name) {
                pthread_once (&get_cell_name_once, get_cell_name);
                if (!cell_name) {
                    data_p->state = 5;
                    continue;
                }
	    }
	    /*
	     * Prepend local cellname
	     */
	    data_p->d2s.chr_p = (byte_t *)cell_name;
	    data_p->state = 2;
	    status = cds_parse_names(data_p);
	    if (status != CDS_SUCCESS) return(status);
	    /*
	     * resume parsing supplied name.
	     */
	    data_p->d2s.chr_p = cp;
	    break;
	case 2:
	    /*
	     * Canonicalize "..."
	     */
	    status = cds_strToSimple(CDS_LOCAL, &data_p->d2s);
	    if (status != CDS_SUCCESS)
		return(status);

	    data_p->state = 3;
	    break;
	case 3:
	    /*
	     * Canonicalize 2nd component name -- it may be X.500 or DNS(BIND),
	     * ie not a local CDS name
	     */
	    status = cds_strToSimple(CDS_NOTLOCAL, &data_p->d2s);
	    if (status != CDS_SUCCESS)
		return(status);
	    if (data_p->d2s.isTyped)
		data_p->state = 4;
	    else
		data_p->state = 5;

	    break;
	case 4:
	    /*
	     * Canonicalize remaining typed names -- if the name doesn't
	     * contain an =, it is a local name
	     */
	    status = cds_strToSimple(CDS_TYPED, &data_p->d2s);
	    if (status != CDS_SUCCESS)
		return(status);
	    if (!data_p->d2s.isTyped)
		data_p->state = 5;

	    break;
	case 5:
	    status = cds_strToSimple(CDS_LOCAL, &data_p->d2s);
	    if (status != CDS_SUCCESS)
		return(status);
	    break;
	}
    }

    return(CDS_SUCCESS);
}


/* ----------------------------------------------------------------------
 *  Convert FullName to string
 * ----------------------------------------------------------------------
 */
int 
cdsCvtFullToStr (cds_full_name_t  *OpaqueFname_p,
                 unsigned char    *StringFname_p,
                 int              *StringFnameLen_p)
{
    dns_ToSimple_t data;
    FullName_t	*from_p = (FullName_t *)OpaqueFname_p;
    int		name_length;

    data.length = 0;
    /*
     * Verify StringFname/StringFnameLen combination
     */
    if (data.chr_p = (byte_t *)StringFname_p) {
	if (!StringFnameLen_p) return(CDS_INVALIDARGUMENT);

	data.length = *StringFnameLen_p;
    } else if (StringFnameLen_p)
	*StringFnameLen_p = 0;

    /*
     * Tack on the SimpleNames
     */
    data.fnLength = 0;		/* accumulated lengths of SimpleNames */
    data.snm_p = (SimpleName_t *)from_p->fn_name;

    if (FULLNAMEMAX < (name_length = EXT16(from_p->fn_length)))
	return(CDS_INVALIDNAME); /* FullName length out of range */

    do {
	unsigned int status;

	if (0 <= --data.length)
	    *data.chr_p++ = '/';

	status = cds_opqToSimple(&data);
	if (status != CDS_SUCCESS) return(status);

	if (name_length < data.fnLength)
	    return(CDS_INVALIDNAME);

    } while (data.snType != SN_null);
    /*
     * The accumulated SimpleName lengths and the FullName length
     * must agree.
     */
    if (name_length != data.fnLength)
	return(CDS_INVALIDNAME);
    /*
     * Tack the trailing NULL on the StringFname
     * If this is the root, leave the trailing slash,
     * otherwise overlay it with the NULL.
     */
    if (name_length == sizeof(NULLSIMPLENAME_t)) {
	if (0 <= --data.length)
	    *data.chr_p = '\0';
    } else if (0 <= data.length)
	*--data.chr_p = '\0';
    /*
     * Pass back the computed length
     */
    if (StringFnameLen_p)
	*StringFnameLen_p -= data.length;

    return((StringFname_p && (data.length < 0)) ? CDS_NOROOM : CDS_SUCCESS);
}

/*
 * Private version for server
 * If OpaqueFname is not global and CellName_p provided,
 * prepend CellName on StringFname.
 */
int 
dns_CvtCDSFullToStr (cds_full_name_t  *OpaqueFname_p,
                     unsigned char    *StringFname_p,
                     int              *StringFnameLen_p,
                     char             *CellName_p)
{
    dns_ToSimple_t data;
    FullName_t	*from_p = (FullName_t *)OpaqueFname_p;
    int		name_length;

    data.length = 0;
    /*
     * Verify StringFname/StringFnameLen combination
     */
    if (data.chr_p = (byte_t *)StringFname_p) {
	if (!StringFnameLen_p) return(CDS_INVALIDARGUMENT);

	data.length = *StringFnameLen_p;
    } else if (StringFnameLen_p)
	*StringFnameLen_p = 0;

    data.snm_p = (SimpleName_t *)from_p->fn_name;
    data.fnLength = 0;		/* accumulated lengths of SimpleNames */

    /*
     * Tack on the SimpleNames
     */

    if (FULLNAMEMAX < (name_length = EXT16(from_p->fn_length)))
	return(CDS_INVALIDNAME); /* FullName length out of range */

    do {
	unsigned int status;

	if (0 <= --data.length) {
	    *data.chr_p++ = '/';
	    *data.chr_p = '\0';		/* terminate a single slash */
	}

	status = cds_opqToSimple(&data);

	if (status != CDS_SUCCESS) return(status);

	if (name_length < data.fnLength)
	    return(CDS_INVALIDNAME);

    } while (data.snType != SN_null);
    /*
     * The accumulated SimpleName lengths and the FullName length
     * must agree.
     */
    if (name_length != data.fnLength)
	return(CDS_INVALIDNAME);
    /*
     * Tack the trailing NULL on the StringFname
     * If this is the root, leave the trailing slash,
     * otherwise overlay it with the NULL.
     */
    if (name_length == sizeof(NULLSIMPLENAME_t)) {
	if (0 <= --data.length)
	    *data.chr_p = '\0';
    } else if (0 <= data.length)
	*--data.chr_p = '\0';
    /*
     * Pass back the computed length
     */
    if (StringFnameLen_p)
	*StringFnameLen_p -= data.length;

    return((StringFname_p && (data.length < 0)) ? CDS_NOROOM : CDS_SUCCESS);
}
