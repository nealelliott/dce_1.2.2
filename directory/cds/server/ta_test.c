/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_test.c,v $
 * Revision 1.1.6.2  1996/02/18  19:37:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:55  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:33:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:28  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:05:04  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:32:28  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:44:15  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:53  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:13:43  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:48:26  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:25:09  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:26:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module ta_test.c
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

#include <server.h>
#include <dbdef.h>
#include <find.h>
#include <sets.h>
#include <security.h>



/*
 * ta_test_att
 *   This routine tries to find an entry.  If the entry is found
 * and the specified attribute is present, it compares the set of values
 * with the supplied value and returns the results
 * The progress record is updated.
 *
 *
 * Input:
 *      user_p = address of user_descriptor
 *      progress_p = address of progress record
 *      entry_type = {object, directory, subdirectory, clearinghouse, link}
 *      attribute_name_p = address of attribute to test
 *      value_p = address of atomic value
 *
 * Output:
 *      out_result_p = address to return result
 *
 * Function value:
 *      status
 *
 * Side effects: 
 *      Progress record updated
 */
dns_status_t 
ta_test_att (user_descriptor_t  *user_p,
             Progress_t         *progress_p,
             int                entry_type,
             SimpleName_t       *attribute_p,
             AtomicValue_t      *value_p,
             byte_t             *out_result_p)
{
    dns_status_t	status;
    db_stream_t		*stream_p;
    db_data_t		*rec_p;
    attribute_descriptor_t *dns_p;
    FullName_t		*fname_p;
    dir_hash_entry_t	*hashuid_p;
    int                 valuelen;

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ReadAttribute);
    if (status != DNS_SUCCESS) return(status);
    status = walktree(user_p, entry_type, WALKTREE_LOOKUP_OP, progress_p, stream_p);
    if (status != DNS_SUCCESS) return(status);

    if (entry_type == ET_dirOrObj)
	if (FLAG_IS_SET(progress_p->pr_flags, PR_directory))
	    entry_type = ET_directory;
	else
	    entry_type = ET_object;

    status = security_check(user_p, stream_p, Test_check, entry_type, 
			    NULL);

    if (status == DNS_UNKNOWNENTRY)
      /* Rebuild resolved name for exception */
      resolved_null(progress_p);

    /* Success if client had TEST access to attribute */
    if (status == DNS_SUCCESS)
        {
	if ((entry_type == ET_directory) || 
	    (entry_type == ET_clearinghouse))
	  rec_p = &stream_p->db_dir;
	else
	  rec_p = &stream_p->db_entry;
	    
	/* Keep in lower case for set_test */
        dns_p = find_known_attribute((AttributeName_t *)attribute_p);
        if (!dns_p)
            *out_result_p = set_test(rec_p->data_p,
			(AttributeName_t *)attribute_p,
			value_p);
        else if ((entry_type == ET_clearinghouse) && 
               (dns_p->atype == DNS_CHDIRECTORIES))
            {
            /* Make sure we have enough data */
            if ((EXT16(value_p->av_length) < (sizeof(ObjUID_u) + sizeof(NULLFULLNAME_u))) || (EXT8(value_p->av_valuetype) != VT_CHDirectory))
		{
                INS8(out_result_p, BL_false);
		}
            else            
                {
                /* Make sure name is valid */
		valuelen = EXT16(value_p->av_length);
                valuelen -= LEN_ObjUID(value_p->av_value);
                fname_p = (FullName_t *)SKIP_ObjUID(value_p->av_value);
		/* Read directory by full name */
		hashuid_p = db_hash_uid(stream_p->db_file_p,
				(ObjUID_t *)value_p->av_value);
		if (!hashuid_p)
		  {
		    INS8(out_result_p, BL_false);
		  }
		else
		  {
		    /* Check name */
		    if (dnsCmpFull((dns_opq_fname *)fname_p, 
				   (dns_opq_fname *)hashuid_p->dir_name)!=0)
		      {
			INS8(out_result_p, BL_true);
		      }
		    else
		      {
			INS8(out_result_p, BL_false);
		      }
		  }
	        } /* End valid length */                          
            }
        else if (dns_p->visable)
            *out_result_p = set_dnstest(rec_p->data_p, 
					EXT16(value_p->av_length),
					value_p->av_value, dns_p);
	else
	  status = DNS_NOTSUPPORTED;

        }

    db_close_stream(stream_p);
    return(status);
}
