/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cprpc_util.c,v $
 * Revision 1.1.8.2  1996/02/18  19:27:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:09  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:07:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:45  root]
 * 
 * Revision 1.1.6.6  1994/08/03  19:00:23  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:38:35  mccann]
 * 
 * Revision 1.1.6.5  1994/06/30  19:15:18  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:36:21  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.6.4  1994/06/09  18:36:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:07  devsrc]
 * 
 * Revision 1.1.6.3  1994/05/12  21:11:01  peckham
 * 	Remove dnstables reference.
 * 	[1994/05/12  19:14:08  peckham]
 * 
 * Revision 1.1.6.2  1994/04/19  17:05:30  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:30:05  mccann]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:51  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:57:23  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  13:02:05  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:55  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/15  20:11:09  m_sawyer
 * 	add check for 4th special condition in cprpc_cmp_wildsimple and
 * 	add cprpc_string_match() and cprpc_cmp_wild_full()
 * 	[1992/10/15  19:58:17  m_sawyer]
 * 
 * Revision 1.1.2.5  1992/07/07  15:05:51  m_sawyer
 * 	fix loop limit in get_local_binding_handle()
 * 	[1992/07/07  14:53:25  m_sawyer]
 * 
 * Revision 1.1.2.4  1992/06/26  07:10:22  jim
 * 	Added include msgblk.h. This is not getting included on AIX 3.2
 * 	for some reason.
 * 	[1992/05/29  18:49:25  jim]
 * 
 * Revision 1.1.2.3  1992/05/13  19:14:54  m_sawyer
 * 	Replace hardwired RPC protocols with call to rpc_network_inq_protseqs ().
 * 	[1992/05/13  19:13:41  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:44:06  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:20:36  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cprpc_util.c
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
 *
 * ABSTRACT:
 *
 * This module contains miscellaneous utility routines used by
 * the control program.
 *
 */

#include <cdscp.h>
#include <assert.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dce/rpc_cvt.h>
#include <dns_malloc.h>
#include <dce/cprpcp.h>
#include <dce/utc.h>
#include <dce/dce_error.h>
#include <dce/cprpc_clerk.h>
#include <dce/cprpc_server.h>

#if defined(DCE_SEC)
#include <dce/sec_login.h>
extern int Authentication;
extern sec_login_handle_t login_context_h;
#endif


/*
 *****************************************************************************
 * Forward declarations for INTERNAL (static) function prototypes
 */

INTERNAL void cprpc_get_towerset
    (
        cds_FullName_t  *,
        cds_Set_t      **,
        error_status_t  *
    );

INTERNAL handle_t get_clh_binding_handle
    (
        int,
        cds_FullName_t *,
        error_status_t *
    );

INTERNAL handle_t get_local_binding_handle
    (
        int,
        error_status_t *
    );

#ifdef DCE_SEC
INTERNAL void authenticate_binding_handle
    (
        int,
        handle_t       *,
        error_status_t *
    );
#endif


/*
 * mgmt_error
 *   Print error from managment commands.
 */
void
mgmt_error(error_status_t status)
{
  dce_error_string_t msgbuf;
  unsigned32 sts;

  dce_error_inq_text(status, msgbuf, &sts);
  printf("%s\n", msgbuf);
}
/*
**++
**
**  ROUTINE NAME:       cprpc_emit_show_header
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**      This routine displays a "SHOW" command header.
**
**  INPUTS:
**
**      outbp           pointer to emit output block
**      msgbp           pointer to emit message block
**      keyp            pointer to message key
**      stringp         pointer to string to display
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

void
cprpc_emit_show_header (
struct op_blk *outbp,
unsigned code,
char *txt,
char *stringp)
{
    emit_hdr(outbp, CDSCP_REFHDR_OPLINE_SHOW, "SHOW");
    emit_hdr_string(outbp, code, txt, stringp);
    emit_refhdr_atline (outbp);
}


/*
**++
**
**  ROUTINE NAME:       cprpc_get_binding_handle
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**      Get an RPC binding handle.
**
**  INPUTS:
**
**      clerk_server_id 0 for clerk,  1 for server
**      ch_fullname_p   ptr to cds fullname for clearinghouse
**      status_p        ptr to return status
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

handle_t cprpc_get_binding_handle (
int             clerk_server_id,
cds_FullName_t *ch_fullname_p,
error_status_t *status_p)
{
    handle_t h;

    /*
     * either get a binding handle based on the name of the
     * designated clearinghouse or get a "default" binding handle
     */
    if (ch_fullname_p)
    {
        h = get_clh_binding_handle (clerk_server_id,
                                    ch_fullname_p,
                                    status_p);
    }
    else
    {
        h = get_local_binding_handle (clerk_server_id, status_p);
    }

    if ((*status_p != DNS_SUCCESS) && (*status_p != rpc_s_ok)) {
      mgmt_error(*status_p);
    }
    return (h);
}


/*
**++
**
**  ROUTINE NAME:       get_local_binding_handle
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Get a default (local) RPC binding handle.
**
**  INPUTS:
**
**      clerk_server_id 0 for clerk,  1 for server
**      status_p        ptr to return status
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

static rpc_protseq_vector_t *protseq_vectorp = NULL;

INTERNAL handle_t get_local_binding_handle (
int             clerk_server_id,
error_status_t *status_p)
{
    rpc_binding_handle_t h_p;
    byte_t *string_binding_p;
    int i;

    /*
     * get a vector of the supported protocols if we haven't already
     * done so
     */
    if (protseq_vectorp == NULL)
    {
        rpc_network_inq_protseqs (&protseq_vectorp, status_p);
        if ((protseq_vectorp == NULL) || (*status_p != rpc_s_ok))
        {
            return (NULL);
        }
    }

    /*
     * loop through supported protocols until we get one that works
     */
    for (i=0; i<protseq_vectorp->count; ++i)
    {
        rpc_string_binding_compose (NULL,
                                    protseq_vectorp->protseq[i],
				    NULL,
                                    NULL,
                                    NULL,  /* no network options */
                                    &string_binding_p,
                                    status_p);
        if (*status_p == rpc_s_ok)
        {
            rpc_binding_from_string_binding (string_binding_p, &h_p, status_p);
            if (*status_p == rpc_s_ok)
            {
                /* Release RPC allocated memory */
                rpc_string_free (&string_binding_p, status_p);
#ifdef DCE_SEC
                authenticate_binding_handle (clerk_server_id,
                                             &h_p,
                                             status_p);
/* Check for errors which won't be avoided by trying other towers,
 * and return back to caller rather than induce an unnecessary
 * delay.
*/
		if (*status_p != rpc_s_ok) {
		    switch (*status_p) {
		        case (rpc_s_auth_tkt_expired): /* expired ticket */
			case (sec_rgy_server_unavailable): /* secd not there */
			    return ((handle_t) NULL);
			default:
			    continue;
		    }
		}
#endif
                return ((handle_t) h_p);
            }
        }

        /* Release RPC allocated memory */
        rpc_string_free (&string_binding_p, status_p);
    }

    /*
     * We only get here if all the protocols in our table have
     * failed to result in a binding.
     */
    return (NULL);
}


/*
**++
**
**  ROUTINE NAME:       get_clh_binding_handle
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**      Get an RPC binding handle.
**
**  INPUTS:
**
**      clerk_server_id 0 for clerk,  1 for server
**      ch_fullname_p   ptr to cds fullname for clearinghouse
**      status_p        ptr to return status
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

INTERNAL handle_t get_clh_binding_handle (
int             clerk_server_id,
cds_FullName_t *ch_fullname_p,
error_status_t *status_p)
{
    rpc_binding_handle_t h_p;
    int i;
    cds_Set_t *towerset_p = NULL;
    unsigned_char_t **string_bindings;

    /*
     * only do the following if we've actually been given a
     * clearinghouse
     */
    if (!ch_fullname_p)
    {
        *status_p = DNS_ERROR;
        return (NULL);
    }
    
    /*
     * get the towerset for this clearinghouse
     */
    cprpc_get_towerset (ch_fullname_p, &towerset_p, status_p);
    if (*status_p != DNS_SUCCESS)
        return (NULL);
    
    /*
     * convert the towerset to an array of string bindings and
     * free the towerset
     */
    *status_p = dnsCvtTowerToStr (towerset_p, &string_bindings);
    dns_free (towerset_p);
    if (*status_p != DNS_SUCCESS)
        return (NULL);
    
    /*
     * loop through our table of string bindings until we get one that
     * works
     */
    for (i=0; string_bindings[i] != NULL; ++i)
    {
	rpc_binding_from_string_binding (string_bindings[i], &h_p, status_p);
	if (*status_p == rpc_s_ok)
	{
#ifdef DCE_SEC
	    authenticate_binding_handle (clerk_server_id, &h_p, status_p);
	    if (*status_p != rpc_s_ok)
		continue;
#endif
	    free (string_bindings);
	    return ((handle_t) h_p);
	}
    }
    /*
     * We only get here if we have failed to get a binding handle.
     */
    free (string_bindings);
    return (NULL);
}


/*
**++
**
**  ROUTINE NAME:       authenticate_binding_handle
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Authenticate an RPC binding handle.
**
**  INPUTS:
**
**      clerk_server_id 0 for clerk,  1 for server
**      h_pp            ptr to ptr to binding handle
**      status_p        ptr to return status
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

INTERNAL void authenticate_binding_handle (
int             clerk_server_id,
handle_t       *h_pp,
error_status_t *status_p)
{
    rpc_if_handle_t sec_ifspec;
#ifdef DCE_SEC
/*
 * Secure the binding handle by setting dce authentication and authorization
 * info on it.
 */
    if (Authentication) {

      /*  endpoint must be fully bound before setting authentication */
      sec_ifspec = (clerk_server_id == 0) ? 
	  DNScpClerk_v1_0_c_ifspec : DNScpServer_v1_0_c_ifspec;
	rpc_ep_resolve_binding(*h_pp, 
			       sec_ifspec,
			       status_p);
        rpc_binding_set_auth_info (*h_pp,
                                   NULL, /* don't know/care server principal name */
                                   rpc_c_authn_level_default,
                                   rpc_c_authn_dce_private,
                                   (rpc_auth_identity_handle_t)login_context_h,
                                   rpc_c_authz_dce,
                                   status_p);
        if (*status_p != rpc_s_ok) {
	  /* Ignore 'protocol unsupported errors' - caller should try other bindings
*/
	  if (*status_p != rpc_s_proto_unsupp_by_auth)
	      mgmt_error(*status_p);
	  *h_pp = NULL;
        }
    }
#endif
}

/*
**++
**
**  ROUTINE NAME:       cprpc_names_pointlast
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**  Return a pointer to the simple name preceeding the last simple name
**  of a fullname if there is one.  Otherwise point at the last simple
**  name.
**
**  Note: This routine was snarfed from names_lib.c in the server.
**
**  INPUTS:
**
**      fname_p         ptr to fullname
**
** INPUTS/OUTPUTS:      none
**
** OUTPUTS:             none
**
** IMPLICIT INPUTS:     none
**
** IMPLICIT OUTPUTS:    none
**
** FUNCTION VALUE:
**
**      pointer to simple name
**
**  SIDE EFFECTS:       none
**
**--
*/

SimpleName_t * cprpc_names_pointlast (
FullName_t *fname_p)
{
    SimpleName_t *current_p;
    SimpleName_t *previous_p;

    if (!EMPTY_FullName(fname_p))
    {
        current_p = (SimpleName_t *)fname_p->fn_name;
	do
        {
	    previous_p = current_p;
	    current_p = (SimpleName_t *)SKIP_SimpleName(previous_p);
        }
	while (EXT8(current_p->sn_flag) != SN_null);

	return(previous_p);
    }
    else
    {
        return((SimpleName_t *)fname_p->fn_name);
    }
}


/*
**++
**
**  ROUTINE NAME:       cprpc_string_match
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**  This routine compares two text strings and returns boolean TRUE
**  if the two strings match. The second string (patterh) may be
**  contain wildcard characters.
**
**      ? - matches any single character
**      * - matches any string of zero or more characters
**      \ - causes both '?' and '*' to be treated literally
**
**  INPUTS:
**
**      text_p          ptr to text string
**      pattern_p       ptr to text string (may contain wildcards)
**
** INPUTS/OUTPUTS:      none
**
** OUTPUTS:             none
**
** IMPLICIT INPUTS:     none
**
** IMPLICIT OUTPUTS:    none
**
** FUNCTION VALUE:
**
**      Returns TRUE if the two strings match.
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE int cprpc_string_match (
unsigned char *text_p,
unsigned char *pattern_p)
{
    assert((text_p != NULL) && (pattern_p != NULL));

    if ((*text_p == '\0') || (*pattern_p == '\0'))
    {
        return (*text_p == *pattern_p);
    }

    /*
     * for all char's in pattern 'pattern_p'
     */
    for ( ; *pattern_p; pattern_p++)
    {
        switch (*pattern_p)
        {
            /*
             * the simple case is the 'exact' match
             */
            default:    if (*text_p++ == *pattern_p) break;
                        return (FALSE);

            /*
             * escaped characters are taken literally
             */
            case '\\':  if (*text_p++ == *++pattern_p) break;
                        return (FALSE);

            /*
             * next check for a single wildcard match
             */
            case '?':   if (*text_p++) break;
                        return (FALSE);

            /*
             * now for the more involved wildcard match
             */
            case '*':
            {
                /*
                 * the simple case here is if this is the last
                 * character of the pattern in which case it is a
                 * match
                 */
                if (*(pattern_p+1) == '\0')
                {
                    return (TRUE);
                }

                /*
                 * see if there is a match to the next part of the
                 * pattern, if not, then see if the next part of the
                 * text can be matched
                 */
                if ( cprpc_string_match (text_p, pattern_p+1) )
                {
                    return (TRUE);
                }
                else
                {
                    return ( cprpc_string_match (text_p+1, pattern_p) );
                }
            }
        }
    }

    return ( *text_p == *pattern_p );
}

/*
**++
**
**  ROUTINE NAME:       cprpc_cmp_wildsimple
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**  This routine compares two simplenames and returns TRUE if they
**  match.  The second simplename may be contain wildcards.
**
**  INPUTS:
**
**      sname1_p        ptr to simplename #1
**      sname2_p        ptr to simplename #2
**
** INPUTS/OUTPUTS:      none
**
** OUTPUTS:             none
**
** IMPLICIT INPUTS:     none
**
** IMPLICIT OUTPUTS:    none
**
** FUNCTION VALUE:
**
**      Returns TRUE if the two simplenames match.
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE int cprpc_cmp_wildsimple (
SimpleName_t *sname1_p,
SimpleName_t *sname2_p)
{
    unsigned_char_t  sname1_str[CDS_STR_SNAME_MAX+1];
    unsigned_char_t  sname2_str[CDS_STR_SNAME_MAX+1];
    int              len1 = sizeof(sname1_str);
    int              len2 = sizeof(sname2_str);
    error_status_t   status;

    status = cdsCvtSimpleToStr ((cds_simple_name_t *)sname1_p, 
                                sname1_str, &len1);
    if (status != DNS_SUCCESS)
    {
      mgmt_error(status);
      return (FALSE);
    }

    status = cdsCvtSimpleToStr ((cds_simple_name_t *)sname2_p,
                                sname2_str, &len2);
    if (status != DNS_SUCCESS)
    {
      mgmt_error(status);
      return (FALSE);
    }

    return (cprpc_string_match (sname1_str, sname2_str));
}

/*
**++
**
**  ROUTINE NAME:       cprpc_cmp_wildfull
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**  This routine compares two fullnames and returns TRUE if they
**  match.  The second fullname may be contain wildcards.
**
**  INPUTS:
**
**      fname1_p        ptr to fullname #1
**      fname2_p        ptr to fullname #2
**
** INPUTS/OUTPUTS:      none
**
** OUTPUTS:             none
**
** IMPLICIT INPUTS:     none
**
** IMPLICIT OUTPUTS:    none
**
** FUNCTION VALUE:
**
**      Returns TRUE if the two fullnames match.
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE int cprpc_cmp_wildfull (
FullName_t *fname1_p,
FullName_t *fname2_p)
{
    unsigned_char_t  fname1_str[CDS_STR_FNAME_MAX+1];
    unsigned_char_t  fname2_str[CDS_STR_FNAME_MAX+1];
    int              len1 = sizeof(fname1_str);
    int              len2 = sizeof(fname2_str);
    error_status_t   status;

    status = cdsCvtFullToStr ((cds_full_name_t *)fname1_p, 
                              fname1_str, &len1);
    if (status != DNS_SUCCESS)
    {
      mgmt_error(status);
      return (FALSE);
    }

    status = cdsCvtFullToStr ((cds_full_name_t *)fname2_p,
                              fname2_str, &len2);
    if (status != DNS_SUCCESS)
    {
      mgmt_error(status);
      return (FALSE);
    }

    return (cprpc_string_match (fname1_str, fname2_str));
}

/*
**++
**
**  ROUTINE NAME:       cprpc_get_towerset
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**  This routine takes the fullname of a clearinghouse and returns
**  the towerset for same.
**
**  INPUTS:
**
**      ch_fullname_p   ptr to fullname of clearinghouse
**
**  INPUTS/OUTPUTS:     none
**
**      towerset_pp     ptr to ptr to towerset
**
**  OUTPUTS:            none
**
**      status_p        ptr to return status
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

void dnsInitFlagStat (struct dnsFlagStat *);

INTERNAL void cprpc_get_towerset (
cds_FullName_t  *ch_fullname_p,
cds_Set_t      **towerset_pp,
error_status_t  *status_p)
{
    FullName_t       fname;
    dns_handle       handle;
    int              read_status, attr_index, attr_num, total_attr_bytes;
    dnsFlagStat_t    flags;
    dns_attrvalue_t *attrval_p;

    /*
     * arbitrarily large array of attribute values in the special
     * form of MemberHeader and AtomicValue
     */
    MemberValue_t   *MemberValues[16];

#define MAX_ATTR sizeof(MemberValues)/sizeof(MemberValue_t *)

    /*
     * sanity check our towerset_pp input, we always return
     * something
     */
    if (!towerset_pp)
    {
        *status_p = DNS_ERROR;
        return;
    }

    /*
     * our defaults
     */
    *towerset_pp = NULL;
    *status_p = DNS_SUCCESS;
    dnsInitFlagStat (&flags);
    ZERO_bytes (MemberValues, sizeof(MemberValues));

    /*
     * don't process a null fullname
     */
    if (!ch_fullname_p) return;

    /*
     * import the clearinghouse fullname
     */
    cdsImport_FullName (NULL, ch_fullname_p, &fname, 0);

    /*
     * get a handle for reading attributes
     */
    handle = dnsGetHandle();
    if (!handle)
    {
        *status_p = DNS_ERROR;
        return;
    }

    /*
     * read all the CDS_CHLastAddress attributes into an array of
     * dns attribute values
     */
    for (attr_index = attr_num = total_attr_bytes = 0,
         read_status = DNS_SUCCESS;
         (read_status == DNS_SUCCESS) &&
         (*status_p == DNS_SUCCESS)   &&
         (attr_index < MAX_ATTR);
         attr_index++)
    {
        /*
         * read an attribute value
         */
        read_status =
            dnsReadAttrValue ((dns_full_name_t *)&fname,
                              handle,
                              (dns_attr_name_t *)dns_AttributeName(DNS_CHLASTADDRESS),
                              dnsCH,
                              NULL,     /* no clearinghouse */
                              &attrval_p,
                              NULL,     /* no CTS */
                              NULL,     /* no address buffer */
                              NULL,     /* no address buffer length */
                              &flags
                              );

        /*
         * only process if successful read
         */
        if (read_status == DNS_SUCCESS)
        {
            int need_bytes, attr_bytes;

            /*
             * we only know about byte values
             */
            if (attrval_p->val_syntax != dns_byte)
            {
                *status_p = DNS_ERROR;
                continue;
            }

            /*
             * allocate some space
             */
            attr_bytes = attrval_p->val_u.val_byte.length;
            need_bytes = sizeof(MemberValue_overhead) + attr_bytes;
            MemberValues[attr_index] =
                (MemberValue_t *) dns_malloc (need_bytes);

            if (MemberValues[attr_index] == NULL)
            {
                *status_p = DNS_NONSRESOURCES;
                continue;
            }
            ZERO_bytes (MemberValues[attr_index], need_bytes);

            /*
             * make a copy of this attribute and bump the total
             * number of attribute bytes that we have accumulated
             * and the number of attributes that we have read
             * successfully
             */
            SET_FLAG(MemberValues[attr_index]->mv_flag, MV_present);
            INS16(MemberValues[attr_index]->mv_length, attr_bytes);
            INS8(MemberValues[attr_index]->mv_valuetype, dns_byte);
            COPY_bytes (attrval_p->val_u.val_byte.byte_p,
                        MemberValues[attr_index]->mv_value,
                        attr_bytes);
            total_attr_bytes += attr_bytes;
            attr_num++;
        }
    }

    /*
     * check that we successfully read attributes, if we didn't,
     * then return in failure
     */
    if (attr_num == 0)
    {
        if (*status_p == DNS_SUCCESS)
        {
            *status_p = read_status == DNS_SUCCESS ? DNS_ERROR : read_status;
        }
        return;
    }
    
    /*
     * now that we have accumulated all the attribute values, build
     * a towerset of all the tower members
     */
    if (*status_p == DNS_SUCCESS)
    {
        Set_t  *towerset_p;
        byte_t *towerset_datap;
        byte_t *towerset_offsetp;
        int     towerset_offset;
        int     need_bytes, copy_bytes, value_bytes;

        /*
         * allocate enough space for the towerset
         */
        need_bytes = sizeof (Set_overhead) + 
                     (sizeof (word_u) * attr_num) +
                     (sizeof (MemberValue_overhead) * attr_num) +
                     total_attr_bytes;
        towerset_p = (Set_t *) dns_malloc (need_bytes);
        if (towerset_p == NULL)
        {
            *status_p = DNS_NONSRESOURCES;
            goto cleanup;
        }

        /*
         * initialize some towerset overhead
         */
        SET_FLAG(towerset_p->st_flag, ST_present);
        INS16(towerset_p->st_offsetLength, attr_num*sizeof(word_u));
        value_bytes = attr_num*sizeof(MemberValue_overhead) +
                      total_attr_bytes;
        INS16(towerset_p->st_valueLength, value_bytes);

        /*
         * now add each of the individual attributes to the towerset
         */
        towerset_datap = DATA_Set((Set_u *)towerset_p);
        towerset_offset = towerset_datap - (byte_t *)towerset_p;
        for (attr_index=0; attr_index<attr_num; attr_index++)
        {
            towerset_offsetp = OFFSET_Set((Set_u *)towerset_p, attr_index);
            INS16(towerset_offsetp, towerset_offset);
            copy_bytes = sizeof(MemberValue_overhead) +
                         EXT16(MemberValues[attr_index]->mv_length);
            COPY_bytes (MemberValues[attr_index],
                        towerset_datap,
                        copy_bytes);
            towerset_datap  += copy_bytes;
            towerset_offset += copy_bytes;
        }

        /*
         * return a pointer to the towerset to our caller
         */
         *towerset_pp = (cds_Set_t *)towerset_p;
    }

    /*
     * cleanup by freeing the 'read' handle and release any memory
     * we allocated for our MemberValue array
     */
cleanup:
    dnsFreeHandle (handle);
    for (attr_index=0; attr_index < attr_num; attr_index++)
    {
        dns_free (MemberValues[attr_index]);
    }
}
