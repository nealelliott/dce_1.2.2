/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscvtobjid.c,v $
 * Revision 1.1.13.1  1996/10/03  14:35:34  arvind
 * 	Quiet purify.
 * 	[1996/09/16  19:59 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.11.2  1996/02/18  19:33:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:43  marty]
 * 
 * Revision 1.1.11.1  1995/12/08  15:22:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:47  root]
 * 
 * Revision 1.1.8.5  1994/09/21  20:08:23  mccann
 * 	fix length of string form of attribute name
 * 	[1994/09/21  20:06:23  mccann]
 * 
 * Revision 1.1.8.4  1994/06/30  19:16:51  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:12  mccann]
 * 
 * Revision 1.1.8.3  1994/06/09  18:41:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:35  devsrc]
 * 
 * Revision 1.1.8.2  1994/04/29  15:53:05  peckham
 * 	Remove unnecessary conditionals.
 * 	Make threads permanent.
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	Eliminate use of dnstables.
 * 	Use dcelocal_path in file names.
 * 	Still to do: abort on pthread failures!!!
 * 	Still waiting for isdcepcs().
 * 	[1994/04/29  14:17:38  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  22:06:26  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:27:47  peckham]
 * 
 * Revision 1.1.6.3  1992/12/30  13:55:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:09:52  zeliff]
 * 
 * Revision 1.1.6.2  1992/09/29  18:51:41  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:15:29  weisman]
 * 
 * Revision 1.1.2.4  1992/07/06  15:35:32  m_sawyer
 * 	Verify length of attributename passed to cdsCvtStrToObjID()
 * 	[1992/07/06  15:33:47  m_sawyer]
 * 
 * Revision 1.1.2.3  1992/05/14  21:36:15  weisman
 * 		Changed up use of pthread_cleanup_push to pass RIOS compiler.
 * 	[1992/05/14  21:35:19  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  22:19:31  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:31:26  weisman]
 * 
 * Revision 1.1  1992/01/19  15:19:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULE: dnscvtobjid.c
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
 */
  
#include <dce/dce.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <dce/assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dce/cdsclerk.h>
#include <uc_clerk.h>     
#include <dns_strings.h>
#include <dce/utc.h>
#include <cds_attr_values.c>


#if !defined(isdcepcs)
# define isdcepcs(c) isascii(c)
#endif

/* local definitions and structures */

/* missing from the strings library */
#define strpspn(s1,s2) (s1 + strspn(s1, s2))

typedef struct data_s {
    char	*d_objid_p;		/* dns obj id string for attribute */
    char	*d_string_p;		/* dns internal name of attribute */
    short	d_objid_len;		/* strlen(d_objid_p) + 1 */
    short	d_string_len;		/* strlen(d_string_p) + 1 */
    char	d_type;			/* attr address type */
} data_t;

typedef struct file_s {
    char		*f_name_p;
    data_t		*f_data_p;	/* vector of attributes */
    time_t		f_mtime;	/* file modification time */
    int			f_refs;		/* reference count */
    /* followed by data from file */
} file_t;

typedef struct parsing_context_s {	/* used while parsing file contents */
    char	*pc_p;			/*  current place during parsing */
    int		pc_length;
    int		pc_terminate;		/*  null-terminate all tokens */
} parsing_context_t;

/*
 * Parsing constants
 */
static char term[] = "#\n \t";	/* line terminators */
#define WHITESPACE &term[2]	/* token terminators */

static file_t *objid_file_p;	/* most recent reading of cds_attributes */
static data_t no_data = { "", "", 1, 1, -1 };

static pthread_once_t objid_once = pthread_once_init;
static pthread_mutex_t objid_mutex;
static pthread_cond_t objid_cond;
static int objid_checking_file;

static data_t *
cdsGetAttrByString (
    file_t		*,
    char		*);

static data_t *
cdsGetAttrByObjID (
    file_t		*,
    char		*);

static unsigned char *
cds_int2bytes (
    unsigned char	*,
    unsigned int	,
    int			,
    unsigned char	*);

static int
cds_CvtDottyToAsnBer (
    unsigned char	*,
    AttributeName_t	*);

static char *
cds_int2chars (
    char		*,
    unsigned int	);

static int
cds_CvtAsnBerToDotty (
    unsigned char	*,
    AttributeName_t	*);

static file_t *
objid_file_lock (void);

static void
objid_file_unlock (
    file_t	*);

static file_t *
objid_file (
    char	*);

static file_t *
objid_parse (
    file_t	*);

static char *
objid_tok (
    parsing_context_t *);

static void
objid_cleanup_fd (
    int		 *);

static void
objid_cleanup_flag (
    int		   );
/*
 * Initialize this module's threads statics
 */

static void 
objid_init (void)
{
    int th_status;

    th_status = pthread_mutex_init(&objid_mutex, pthread_mutexattr_default);
    if (th_status < 0) 
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)th_status);
    }

    th_status = pthread_cond_init(&objid_cond, pthread_condattr_default);
    if (th_status < 0) 
    {
        dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)th_status);
    }

    objid_checking_file = FALSE;
}


/*
 * ----------------------------------------------------------------------------
 *  Convert simple name (either attribute name string or objid dotty string)
 *  to the other string representation (as found in the mapping file) and
 *  optionally return the opaque name (the asn.1 ber string representation).
 * ----------------------------------------------------------------------------
 */

int 
cdsCvtStrToObjID (unsigned char    *StringAName_p,
               byte_t           *AttrValueByte_p,
               unsigned char    *StrOtherName_p,
               int              *StrOtherNameLen_p,
               cds_attr_name_t  *OpaqueAName_p,
               int              *OpaqueANameLen_p,
               unsigned char    **End_pp)
{
    AttributeName_t OpqAttrName;
    unsigned char SimpleStr[CDS_STR_ANAME_MAX+1];
    unsigned char DottyStr[CDS_STR_ANAME_MAX+1];
    file_t *file_p;
    unsigned char *name_p;
    int i = 0, dottycount = 0;
    unsigned int status;
 

    if (End_pp)
	*End_pp = StringAName_p; 	/* Make sure this has a value 	*/

    /* Verify StrOtherName/StrOtherNameLen combination			*/

    if ( StrOtherName_p  && !StrOtherNameLen_p )
	return ( CDS_INVALIDARGUMENT );

    /* Verify OpaqueAName/OpaqueANameLen combination			*/

    if (OpaqueAName_p) {
	if ( !OpaqueANameLen_p )
	    return ( CDS_INVALIDARGUMENT );
    } else if (OpaqueANameLen_p)
	*OpaqueANameLen_p = 0;

    /* Determine whether string is normal simple or dotty,
       note that if it's NOT dotty, we assume it is normal simple.	*/

    name_p = StringAName_p;
    while (*name_p && isdcepcs(*name_p) && !isspace(*name_p)) {
      if (((name_p[0] == '.') && (name_p[1] != '.')) || isdigit(*name_p))
	  ++dottycount;
      if (i > CDS_STR_ANAME_MAX-1)
          return (CDS_INVALIDARGUMENT);
      DottyStr[i++] = *name_p++;
    }
    DottyStr[i] = '\0';

    if (End_pp)		/* point to terminator or first invalid char	*/
	*End_pp = name_p;
    else if (*name_p)	/* if terminator not null, invalid simple name	*/
	status = CDS_INVALIDNAME;

    /* CANCEL POINT */
    file_p = objid_file_lock();
    /*
     * NOTE: there should be no cancellation points between here
     * and the objid_file_unlock() call.
     *
     * BEGIN CANCELLATION POINT BAN
     */

    /*  if the name has parsed successfully, proceed to do one of two
	possible conversions, and if found in the mapping file, and if
	requested, pass the other form of the name to the caller.	*/

    if ( i == dottycount ) { 	/* the string was truly dotty */
	status = cds_CvtDottyToAsnBer ( DottyStr, &OpqAttrName );

	if ( status == CDS_SUCCESS ) {
	    data_t *data_p = cdsGetAttrByObjID (file_p, (char *)DottyStr);
	    if (!data_p) data_p = &no_data;

	    if ( StrOtherName_p ) {
		if ( data_p->d_string_len <= *StrOtherNameLen_p )
		    COPY_bytes(data_p->d_string_p, StrOtherName_p,
				    data_p->d_string_len);
		else
		    status = CDS_NOROOM;
	    }

	    if ( StrOtherNameLen_p )
		*StrOtherNameLen_p = data_p->d_string_len;

	    if ( AttrValueByte_p )
		*AttrValueByte_p = data_p->d_type;
	}
    } else {
	data_t *data_p;

	/* since the non-dotty string may be embedded, sez Janet, copy it
	   into an intermediate null-terminated string, 7 Mar 1991, hwt */

	strcpy ((char *) SimpleStr, (char *)DottyStr );

	if ( data_p = cdsGetAttrByString (file_p, (char *)SimpleStr)) {
	    COPY_bytes(data_p->d_objid_p, DottyStr, data_p->d_objid_len);
	    COPY_bytes(data_p->d_string_p, SimpleStr, data_p->d_string_len);

	    if ( AttrValueByte_p )
		*AttrValueByte_p = data_p->d_type;

	    status = cds_CvtDottyToAsnBer ( DottyStr, &OpqAttrName );

	    if ( status == CDS_SUCCESS ) {
		/* include null in length of returned string, 7 Mar 1991, hwt */
		const int StrLen = strlen((char *)DottyStr) + 1;
		if ( StrOtherName_p ){
		    if ( StrLen <= *StrOtherNameLen_p )
			strcpy ((char *) StrOtherName_p, (char *)DottyStr );
		    else
			status = CDS_NOROOM;
		}
		if ( StrOtherNameLen_p )
		    *StrOtherNameLen_p = StrLen;
	    }
	} else
	    status = CDS_INVALIDNAME;
    }

    objid_file_unlock(file_p);
    /*
     * END CANCELLATION POINT BAN
     */

    /*  if all the above was successful, we have an AsnBer string to pass
	to the caller as the opaque name.				*/

    if ( status == CDS_SUCCESS ) {
	const int OpqLen = LEN_SimpleName(&OpqAttrName);

	if ( OpaqueAName_p ) {		/* pass back the opaque name	  */
	    if ( OpqLen <= *OpaqueANameLen_p )
		COPY_bytes(&OpqAttrName, OpaqueAName_p, OpqLen);
	    else
		status = CDS_NOROOM;
	}

	if ( OpaqueANameLen_p )
	    *OpaqueANameLen_p = OpqLen;  /* pass back the computed length */
    }

    return ( status );
}

/*
 * ----------------------------------------------------------------------------
 *  Convert an opaque name (the asn.1 ber string representation) to a simple
 *  name (either attribute name string or dotty string or both, as found in
 *  the mapping file).
 * ----------------------------------------------------------------------------
 */

int 
cdsCvtObjIDToStr (cds_attr_name_t  *OpaqueAName_p,
               byte_t           *AttrValueByte_p,
               unsigned char    *StringAName_p,
               int              *StringANameLen_p,
               unsigned char    *StrDottyName_p,
               int              *StrDottyNameLen_p)
{
    unsigned char DottyStr[CDS_STR_ANAME_MAX+1];
    unsigned int status;

    /* Verify StringAName/StringANameLen combination			*/

    if ( StringAName_p  && !StringANameLen_p )
	return ( CDS_INVALIDARGUMENT );

    /* Verify StrDottyName/StrDottyNameLen combination			*/

    if ( StrDottyName_p && !StrDottyNameLen_p )
	return ( CDS_INVALIDARGUMENT );

    /*  Verify OpaqueAName parameters; if omitted, we better be passed
	a valid dotty string.						*/

    dce_assert(cds__svc_handle,OpaqueAName_p);	/* Must be an OpaqueAName_p */

    status = cds_CvtAsnBerToDotty(DottyStr, (AttributeName_t *)OpaqueAName_p);

    if (status == CDS_SUCCESS) {
	file_t *file_p;
	data_t *data_p;
	int StrLen;

	/* CANCEL POINT */
	file_p = objid_file_lock();
	/*
	 * NOTE: there should be no cancellation points between here
	 * and the objid_file_unlock() call.
	 *
	 * BEGIN CANCELLATION POINT BAN
	 */

	data_p = cdsGetAttrByObjID (file_p, (char *)DottyStr );
	if (!data_p) data_p = &no_data;

	if ( StringAName_p ) {
	    if ( data_p->d_string_len <= *StringANameLen_p )
		COPY_bytes(data_p->d_string_p, StringAName_p,
			    data_p->d_string_len);
	    else
		status = CDS_NOROOM;
	}

	if ( StringANameLen_p )
	    *StringANameLen_p = data_p->d_string_len;

	if ( AttrValueByte_p )
	    *AttrValueByte_p = data_p->d_type;

	/* include null in length of returned string, 7 Mar 1991, hwt	*/
	StrLen = strlen((char *)DottyStr) + 1;

	if ( StrDottyName_p ){
	    if ( StrLen <= *StrDottyNameLen_p )
	        strcpy ( (char *)StrDottyName_p, (char *)DottyStr );
	    else
	        status = CDS_NOROOM;
	}
	if ( StrDottyNameLen_p )
	    *StrDottyNameLen_p = StrLen;

	objid_file_unlock(file_p);
	/*
	 * END CANCELLATION POINT BAN
	 */
    }

    return ( status );
}


/*
 * ----------------------------------------------------------------------------
 *  Convert an objid dotty string to an asn.1 ber string
 * ----------------------------------------------------------------------------
 */

static int 
cds_CvtDottyToAsnBer (unsigned char    *ObjIDStr_p,
                      AttributeName_t  *ObjIDAnmBer_p)
{
    unsigned char *in_p = ObjIDStr_p;
    unsigned char *out_p = &ObjIDAnmBer_p->an_name[2];
    unsigned char *end_p = (unsigned char *)ObjIDAnmBer_p + sizeof(*ObjIDAnmBer_p);
    unsigned int temp;
    int dot_count = 0;

    /* while scanning make sure we ONLY have dots and digits !!! */

    do {
	register unsigned int val = 0;

	/* must be at least one digit */
	if (!isdigit(*in_p))
	    return(CDS_INVALIDARGUMENT);

	/* accumulate value */
	do {
	    val *= 10;
	    val += (*in_p++ - '0');
	} while (isdigit(*in_p));

	/* must be terminated by '.' or NULL */
	if (*in_p == '.')
	    in_p++;
	else if (*in_p)
	    return(CDS_INVALIDARGUMENT);

	/*
	 * The first two values get mashed together
	 */
	switch (++dot_count) {
	case 1:
	    if (2 < val)
		return(CDS_INVALIDARGUMENT);
	    temp = val * 40;
	    continue;
	case 2:
	    if (40 <= val)
		return(CDS_INVALIDARGUMENT);
	    val += temp;
	default:
	    break;
	}

	/* inject the value into the asn string */
	out_p = cds_int2bytes (out_p, val, 0, end_p);
	if (out_p == NULL)
	    return(CDS_NOROOM);
    } while ( *in_p );

    /* No null strings */
    if (dot_count < 2)
	return(CDS_INVALIDARGUMENT);

    ObjIDAnmBer_p->an_name[0] = 0x06;	/* set the CCITT Object Identifier */
    ObjIDAnmBer_p->an_name[1] = out_p - &ObjIDAnmBer_p->an_name[2];

    /* complete the AttributeName */
    INS8(ObjIDAnmBer_p->an_length, (out_p - ObjIDAnmBer_p->an_name));
    INS8(ObjIDAnmBer_p->an_flag, SN_objectid);
    return ( CDS_SUCCESS );
}

static unsigned char 
*cds_int2bytes (unsigned char  *p,
                unsigned int   val,
                int            flag,
		unsigned char  *end_p)
{
    int data_byte = (val & 0x7F) | flag;
    val >>= 7;
    if (val != 0)
	p = cds_int2bytes (p, val, 0x80, end_p);
    if (p == NULL)
	return(NULL);
    if (p >= end_p)
	return(NULL);
    *p++ = data_byte;
    return(p);
}

/*
 * ----------------------------------------------------------------------------
 *  Convert an asn.1 ber string to an objid dotty string
 * ----------------------------------------------------------------------------
 */

static int 
cds_CvtAsnBerToDotty (unsigned char    *ObjIDStr_p,
                      AttributeName_t  *ObjIDAnmBer_p)
{
    register unsigned char *in_p = ObjIDAnmBer_p->an_name;
    register char *out_p = (char *)ObjIDStr_p;
    int bytes, first = -1, value = 0;

    /* Validate AttributeName */
    if ( EXT8(ObjIDAnmBer_p->an_flag) != SN_objectid )
	return ( CDS_INVALIDARGUMENT );

    /* must be enough room for type/length and at least one byte data */
    bytes = EXT8(ObjIDAnmBer_p->an_length) - 2;
    if ( bytes < 1 )
	return ( CDS_INVALIDARGUMENT );

    /* CCITT Object Identifier */
    if ( *in_p++ != 0x06 )
	return ( CDS_INVALIDARGUMENT );

    /* lengths must concur */
    if ( bytes != *in_p++ )
	return ( CDS_INVALIDARGUMENT );

    do {
	value = ( value << 7 ) + ( *in_p & 0X7F );
	if ( !( *in_p++ & 0X80 )){
	    if ( first < 0 ) {
		/* split the first value */
		first = value / 40;
		if (2 < first) first = 2;
		out_p = cds_int2chars(out_p, first);
		value -= first * 40;
	    }
	    *out_p++ = '.';
	    out_p = cds_int2chars(out_p, value);
	    value = 0;
	}
    } while (--bytes);

    *out_p = '\0';

    return ( CDS_SUCCESS );
}

static char 
*cds_int2chars (char          *p,
                unsigned int  val)
{
    const char this = '0' + (val % 10);

    if (val /= 10)
	p = cds_int2chars(p, val);

    *p++ = this;
    return(p);
}


static data_t 
*cdsGetAttrByString (file_t  *file_p,
                     char    *nam)
{
    if (file_p) {
	register data_t *h_p = file_p->f_data_p;

	for (; h_p->d_objid_p; h_p++)
	    if (strcmp(h_p->d_string_p, nam) == 0)
		return(h_p);
    }
    return(NULL);
}

static data_t 
*cdsGetAttrByObjID (file_t  *file_p,
                    char    *addr)
{
    if (file_p) {
	register data_t *h_p = file_p->f_data_p;

	for (; h_p->d_objid_p; h_p++)
	    if (strcmp(h_p->d_objid_p, addr) == 0)
		return(h_p);
    }
    return(NULL);
}

/*
 * If this is the first use, open the file if we can,
 * otherwise check if file has been modified (if no one else is checking).
 * New or modified files must be opened and parsed.
 * Superceeded files which are in use will be released by the last user.
 */

static file_t 
*objid_file_lock (void)
{
    char filename[_DNS_MAXPATHLEN+1];
    file_t *file_p;

    /* first time thru, initialize threads structs */
    pthread_once(&objid_once, objid_init);

    pthread_mutex_lock(&objid_mutex); {
	while (TRUE) {
	    /*
	     * If noone else os verifying if file has changed, volunteer.
	     */
	    if (!objid_checking_file) {
		objid_checking_file = TRUE;
		file_p = NULL;
		break;
	    } else if (file_p = objid_file_p) {
		/*
		 * Someone else is checking - use old one if there.
		 */
		file_p->f_refs++;
		break;
	    }
	    /*
	     * Wait for other to finish checking.
	     */
	    pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock,
				 (pthread_addr_t)&objid_mutex);
		pthread_cond_wait(&objid_cond, &objid_mutex);
	    pthread_cleanup_pop(0);
	}
    } pthread_mutex_unlock(&objid_mutex);

    /*
     * Use old values if new ones aren't available yet.
     */
    if (file_p) return(file_p);

    sprintf(filename, CDS_CDS_ATTR_FILE, dcelocal_path);
    /*
     * Make sure cancel won't leave things locked.
     */
    pthread_cleanup_push((void (*)(void *))objid_cleanup_flag, NULL);

    /*
     * If we have read the file once, check if it has been
     * modified since our last reading.
     */
    if (file_p = objid_file_p) {
	struct stat filestat;

	if (stat(filename, &filestat) < 0)
	    file_p = NULL;
	else if (file_p->f_mtime != filestat.st_mtime)
	    file_p = NULL;
    }

    /*
     * If it has been modified (or is new), read the file and parse it.
     */
    if (!file_p)
	if (file_p = objid_file(filename))
	    file_p = objid_parse(file_p);

    pthread_cleanup_pop(0); /* we will release objid_checking_file ourselves */

    pthread_mutex_lock(&objid_mutex); {
	if (file_p) {
	    if (file_p != objid_file_p) {
		/*
		 * Release old file if no users.
		 */
		if (objid_file_p && !objid_file_p->f_refs) {
		    free((char *)objid_file_p->f_data_p);
		    free((char *)objid_file_p);
		}
		/*
		 * Insert new copy if there is one.
		 */
		objid_file_p = file_p;
	    }
	    file_p->f_refs++;
	} else
	    /*
	     * Use old copy if there is one.
	     */
	    if (file_p = objid_file_p)
		file_p->f_refs++;

	objid_checking_file = FALSE;
	pthread_cond_signal(&objid_cond);
    } pthread_mutex_unlock(&objid_mutex);

    return(file_p);
}

/*
 * Release the file if this is the last user
 * and the file has been superceeded.
 */
static void 
objid_file_unlock (file_t *file_p)
{
    if (file_p) {
	pthread_mutex_lock(&objid_mutex); {
	    if ((--file_p->f_refs == 0) && (file_p != objid_file_p)) {
		free((char *)file_p->f_data_p);
		free((char *)file_p);
	    }
	} pthread_mutex_unlock(&objid_mutex);
    }
}

/*
 * Allocate memory for file header and data,
 * and read in the data.
 */
static file_t 
*objid_file (char *name_p)
{
    file_t *file_p = NULL;
    int fd;

    /* CANCEL POINT */
    if (0 <= (fd = open(name_p, O_RDONLY, 0))) {
	struct stat filestat;

	/*
	 * Make sure fd is recovered if cancelled.
	 */
	pthread_cleanup_push((void (*)(void *))objid_cleanup_fd,
			     (pthread_addr_t) &fd);

	/* CANCEL POINT ?? */
	if (0 <= fstat(fd, &filestat)) {
	    if (file_p = (file_t *)malloc(sizeof(file_t)+filestat.st_size+2)) {
		char *const buf_p = (char *)&file_p[1];
		int size;

		file_p->f_name_p = name_p;
		file_p->f_data_p = 0;
		file_p->f_mtime = filestat.st_mtime;
		file_p->f_refs = 0;

		if (size = filestat.st_size) {
		    char *next_p = buf_p;
		    int status;

		    /*
		     * Make sure buffer is recovered if cancelled.
		     */
		    pthread_cleanup_push((void (*)(void *))free,
					 (pthread_addr_t)file_p);

		    do {
			/* CANCEL POINT */
			if ((status = read(fd, next_p, size)) < 0) {
			    free((char *)file_p);
			    file_p = NULL;
			    break;
			}
		    } while (next_p += status, size -= status);

		    pthread_cleanup_pop(0); /* no danger of cancellation now */

		    if (file_p) {
			if (next_p[-1] != '\n')
			    *next_p++ = '\n';

			*next_p = '\0';
		    }
		} else
		    *buf_p = '\0';
	    }
	}
	pthread_cleanup_pop(0);
	close(fd);
    }
    return(file_p);
}

/*
 * parse cds_globalnames
 */
static file_t 
*objid_parse (file_t *file_p)
{
    parsing_context_t pc;
    int size = 0;

    pc.pc_terminate = 0;	/* don't terminate tokens on first pass */
    pc.pc_length = 0;		/* silence purify UMC */
    
    while (TRUE) {
	char *p = (char *)&file_p[1];	/* beginning of buffer */
	data_t *data_p = file_p->f_data_p; /* maybe data vector */

	while (*p) {
	    data_t data;
	    register char *tp;
	    int i;

	    memset(&data, 0, sizeof(data)); /* silence purify UMC */
	    pc.pc_p = p;
	    p = strchr(p, '\n') + 1;	/* set to next line */
	    if (*pc.pc_p == '#')	/* fast ignore of comment lines */
		continue;

	    if (data.d_objid_p = objid_tok(&pc))	/* OID */
		data.d_objid_len = pc.pc_length;
	    if (data.d_string_p = objid_tok(&pc))	/* LABEL */
		data.d_string_len = pc.pc_length;
	    if (tp = objid_tok(&pc)) {			/* SYNTAX */
		data.d_type = -1;
		for (i = 0; *cds_val[i]; i++)
		    if (!strcmp(tp, cds_val[i])) {
			data.d_type = i;
			break;
		    }
	    }
	    /*
	     * If any parsing failed, the pointer will have been nullified.
	     */
	    if (pc.pc_p)
		/*
		 * First pass - figure size of data vector.
		 * Second pass - copy into list.
		 */
		if (!data_p)
		    size += sizeof(*data_p);
		else
		    *data_p++ = data;
	}
	if (data_p) {
	    /*
	     * Last entry has a null objid.
	     */
	    data_p->d_objid_p = 0;
	    return(file_p);
	}
	/*
	 * End of first pass - allocate data vector.
	 */
	if (file_p->f_data_p = (data_t *)malloc(size + sizeof(*data_p))) {
	    pc.pc_terminate = 1;	/* terminate tokens now */
	    continue;
	}
	/*
	 * Sour grapes - can't get data vector, so toss the whole thing out!
	 */
	free((char *)file_p);
	return(NULL);
    }
}

/*
 * Parse tokens and optionally terminate with null.
 */
static char 
*objid_tok (parsing_context_t *pc_p)
{
    register char *p, *q = pc_p->pc_p;

    /* quit now if previous parsing failed */
    if (!q) return(0);

    /* skip leading whitespace */
    p = strpspn(q, WHITESPACE);

    switch (*p) {
    /* error if line terminator */
    case '#':
    case '\n':
    case '\0':
	return(pc_p->pc_p = 0);
    default:
	/* token bounded by next terminator or whilespace */
	if ((q = strpbrk(p, term)) && pc_p->pc_terminate) {
	    *q++ = '\0';
	    pc_p->pc_length = q - p;
	}
	/* save new position */
	pc_p->pc_p = q;
	return(p);
    }
}

static void 
objid_cleanup_fd (int *fd)
{
    close(*fd);
}

static void 
objid_cleanup_flag (int unused)
{
    pthread_mutex_lock(&objid_mutex); {
	objid_checking_file = 0;
	pthread_cond_signal(&objid_cond);
    } pthread_mutex_unlock(&objid_mutex);
}
