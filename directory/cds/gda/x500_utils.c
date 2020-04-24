/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: x500_utils.c,v $
 * Revision 1.1.4.2  1996/02/18  19:31:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:13:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:42  root]
 * 
 * Revision 1.1.2.6  1994/08/03  19:01:48  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:52:33  mccann]
 * 
 * Revision 1.1.2.5  1994/06/23  18:30:13  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:28:29  mccann]
 * 
 * Revision 1.1.2.4  1994/06/09  18:38:16  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:32  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/12  21:11:21  peckham
 * 	Eliminate dnstables reference.
 * 	[1994/05/12  19:15:24  peckham]
 * 
 * Revision 1.1.2.2  1994/03/16  23:42:29  jd
 * 	Replaced a couple of *var != NULL with *var != 0. NULL on the rios
 * 	is (void *).
 * 	[1994/03/16  21:31:50  jd]
 * 
 * Revision 1.1.2.1  1994/03/12  22:01:55  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:10:30  peckham]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1993-1994. ALL RIGHTS RESERVED.
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


/* 
 * Module: x500_utils.c
 * Contains X.500 support routines:
 * X500_name_to_object: converts X.500 string name to OM object
 * 
*/

#include <gda.h>
#include <gda_xds.h>

OM_EXPORT(DS_C_AVA)
OM_EXPORT(DS_C_DS_DN)
OM_EXPORT(DS_C_DS_RDN)

#define OM_DELETE_TEXT "om_delete"
#define OM_CREATE_TEXT "om_create"
#define OM_PUT_TEXT "om_put"

/*
 * Local prototypes
 */
static int
normalize_x500name(
		   char *,
		   char *);


/* X500_name_to_object: Convert X500 string name to OM.
 * String name may be either tagged  or oid format (e.g.
 * "/c=us/o=dec/ou=xyz" or "/2.5.4.6=us/2.5.4.10=dec/2.5.4.11=xyz"
 * NOTE:
 *     cdsCvtStrToObjID doesn't work with the compressed
 *     oid formats (85.4.6, etc.)
 *
 *     THIS ROUTINE IS NOT THREAD SAFE (uses statics)
 * When XDS becomes thread safe, the static arrays used below
 * should be dynamically allocated and provisions made for them
 * to be released by the caller.
 *
 * Returns 0 on success, non-0 on failure.
*/
int
X500_name_to_object(char *name, 		/* string name to convert */
		    OM_private_object *dn_object_p,  /* in/out: ptr to    */
		                                     /* distinguished name */ 
						/* object this should be   */
		                                /* freed by caller with    */
		                                /* om_delete(), when done */
		    OM_workspace workspace)   /* callers OM workspace   */
{
#define MAX_VALUE_LEN 128
#define MAX_RDNS 20
    static    OM_object_identifier oid[MAX_RDNS];
    static    OM_string oid_strings[MAX_RDNS];/*stores OM strings containing oids*/
    static    cds_attr_name_t cdsattrname[MAX_RDNS]; /* Stores actual oids */

    static    char value_string[MAX_RDNS][MAX_VALUE_LEN+4]; /* +1 for NULL at end*/
                                                            /* +4 to keep word aligned */

    static    OM_descriptor ava[MAX_RDNS][4], rdn_object[MAX_RDNS][3];
    OM_descriptor dn_descr_list[MAX_RDNS];
    OM_return_code return_code;


    int	rdn_count, len, i;
    char *name_ptr;
    char tmp_name[CDS_STR_SNAME_MAX];
    char oid_name[CDS_STR_FNAME_MAX];
    int cdsattrname_len = sizeof (cds_attr_name_t);
    AttributeName_t *opqattrname_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >X500_name_to_object : name = %s",
        name));

     /* 
      *Converted tags to oids, if necessary 
      */
     if (*name == '/') 
     {
	 /*	
	  * bump over leading slash, if nec. 
	  */
         ++name;
     }

     if (strncmp (name, "2.5.", 4)) 
     {
    	if (normalize_x500name (name, oid_name)) 
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug2,
                " <X500_name_to_object : bad X.500 name detected (%s)",
                name));

	    return(1);
	}
	name = oid_name;
    }

/**
 ** build a list of RDN's
 **
 **  scan name string...
 **/
    rdn_count = 0;
    name_ptr = name;

    while (*name_ptr != 0) {  /* until the end... */

      if (rdn_count == MAX_RDNS)  return (1); /* too long.. */
	/*
	 * next character must be "/"
	 */
      if ((rdn_count > 0) && (*name_ptr++ != '/')) return (1);

	/*
	 * okay, just past '/', scan until '='
	 */
      if (strchr(name_ptr, '=') == NULL) return (1);
      len = strcspn(name_ptr, "=");	/* length of string */
      /* Convert dotty string to binary */
      strncpy(tmp_name, name_ptr, len);
      *(tmp_name+len) = '\0';
      if (cdsCvtStrToObjID((unsigned char *)tmp_name,
 		       NULL,        /* don't want attr value */
		       NULL,        /* don't want cds string name */
		       0,
		       &cdsattrname[rdn_count],   /* just get opaque name */
		       &cdsattrname_len,
		       NULL) != CDS_SUCCESS)
	  return (1);
      /* the asn1 name is embedded in a an oqaque name structure */
      opqattrname_p = (AttributeName_t *)&cdsattrname[rdn_count];
      /* make an OM_string object out of it */
      oid_strings[rdn_count].length = 
	  opqattrname_p->an_name[1]; /* the real length */
      oid_strings[rdn_count].elements = (void *)
	  &opqattrname_p->an_name[2]; /* this is the real asn1 part */
      oid[rdn_count] = oid_strings[rdn_count]; /* ptr to OID string object */
      name_ptr += len+1;
/*
 * now pointing at value
 *
 * value terminated by '/' or NULL  NO SUPPORT of quotes right now
 */
	if (strchr(name_ptr, '/') == NULL) { /* rest of string is value */
	    if (strlen(name_ptr)>MAX_VALUE_LEN) return (1);
	    strcpy(value_string[rdn_count], name_ptr);
	    rdn_count++;
	    break;		/* all done with scan */
	}
/*
 * else there is more after this..
 */
	len = strcspn(name_ptr, "/");	/* length of string */
	if (len>MAX_VALUE_LEN) return (1);
	memcpy (value_string[rdn_count], name_ptr, len); 
	value_string[rdn_count][len] = 0;
	rdn_count++;

	name_ptr += len;

    } /* end while (*name_ptr != NULL) */

/*
 * okay, build DN object
 */
    for (i = 0; i < rdn_count; i++) {

/*
 * build AVA
 */
	OMX_CLASS_DESC(	    ava[i][0], DS_C_AVA );
	OMX_ATTR_TYPE_DESC( ava[i][1], DS_ATTRIBUTE_TYPE, oid[i]);
	OMX_ZSTRING_DESC(   ava[i][2], OM_S_PRINTABLE_STRING, 
				       DS_ATTRIBUTE_VALUES,
				       value_string[i]);
        OMX_OM_NULL_DESC(   ava[i][3]);
/* 
 * build RDN object
 */
	OMX_CLASS_DESC(   rdn_object[i][0], DS_C_DS_RDN);
	OMX_OBJECT_DESC(  rdn_object[i][1], DS_AVAS, ava[i]);
	OMX_OM_NULL_DESC( rdn_object[i][2]);

    /*
     * add to growing DN descriptor list 
     */
        OMX_OBJECT_DESC ( dn_descr_list[i+1], DS_RDNS, rdn_object[i]);

    } /* end for (all rdn's) */
/*
 * build RDN descriptor list
 */
    OMX_CLASS_DESC (	dn_descr_list[0], DS_C_DS_DN);
    OMX_OM_NULL_DESC(	dn_descr_list[rdn_count+1]);

/* 
 * create DN object
 */
    CHECK_OM_CALL (OM_CREATE_TEXT,
		   om_create (	DS_C_DS_DN,	/* class */
				OM_FALSE,	/* don't initialize */
				workspace,
				dn_object_p));


/* 
 * add RDN list to DN object
 */

    CHECK_OM_CALL (OM_PUT_TEXT,
		   om_put (  *dn_object_p,
			    OM_REPLACE_ALL,
			    dn_descr_list,
			    0,
			    0,
			    0));
	    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <X500_name_to_object successful return"));

    return (0);
}


/********************************************************************/

/* normalize_x500name: convert X.500 cell name from form:
 * "/c=us/o=dec/ou=lkg" to form: "85.4.6=us/85.4.10=dec/85.4.11=lkg"
 * return 0 on success, 1 on error
*/
static int 
normalize_x500name (char  *x500name,
                    char  *oid_x500name)
{
  char tmp_x500name[CDS_STR_FNAME_MAX];
  char            *cp, *str_name_p;
  cds_full_name_t  temp_opq_name;
  int		 isEscaped;
  int              fname_len;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_gda,
      svc_c_debug2,
      " >normalize_x500name : name = %s",
      x500name));

/* convert string to internal */
  fname_len = sizeof(temp_opq_name);
  if (dns_CvtCDSFullToGDS((unsigned char *)x500name, 0, 1,
			&temp_opq_name, &fname_len) != DNS_SUCCESS) {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <normalize_x500name : failed to convert from cds name to gds name"));
      return(1);
  }

/*
 * And back to string.
 */
  fname_len = sizeof(tmp_x500name);
  if (cdsCvtFullToStr(&temp_opq_name, (unsigned char *)tmp_x500name,
			 &fname_len) != DNS_SUCCESS) {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <normalize_x500name : failed to convert from gds full name to string"));
      return(1);
  }
/*
 * Swallow the leading '/' and any escapes in the resolved X.500 name
 * (with object ids) while copying it to the caller's buffer
 */
  str_name_p = tmp_x500name;
  if (*str_name_p == '/')
      str_name_p++;		/* advance past the / */

  isEscaped = 0;
  for (cp = str_name_p; *cp; cp++) {
    if (*cp == '\\' && !isEscaped)
       	isEscaped = 1;
    else {
      *oid_x500name++ = *cp;
      isEscaped = 0;
    }
  }
  *oid_x500name = '\0';		/* nul terminator */
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_gda,
      svc_c_debug2,
      " <normalize_x500name : normalized = %s",
      oid_x500name));
  return(0);
}


