/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: update_lib.c,v $
 * Revision 1.1.6.2  1996/02/18  19:37:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:09  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:34:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:44  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:05:14  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:33:28  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:44:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:04  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:14:11  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:49:17  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:25:47  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:26:49  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: update_lib.c
 * Version: X02.13
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
#include <dce/assert.h>
#include <names.h>
#include <uid.h>


/*
 * update_delete_build
 *	This routine builds an update structure to delete an entry
 *
 * Input:
 *	upd_p = pointer to update buffer
 *	ts_p = pointer to timestamp or 0 to generate one
 */
int 
update_delete_build (Update_t     *upd_p,
                     Timestamp_t  *ts_p)
{
    register byte_t *next_p;

    INS8(upd_p->ud_operation, UD_absent);
    COPY_Timestamp(ts_p, upd_p->ud_timestamp);

    next_p = upd_p->ud_attribute;

    /* Fill in attribute specifier */
    PUT8(next_p, AT_null);
    PUT8(next_p, BL_false);
    return DIFF_bytes(next_p, upd_p);
}

/*
 * update_build
 *	This routine builds an update structure to add a value to an
 *	attribute
 *
 * Input:
 *	upd_p = pointer to update buffer
 *	op = operation (present or absent)
 *	ts_p = pointer to timestamp or 0 to generate one
 *	type = type of attribute
 *	att_p = pointer to attribute name
 *	case_p = pointer to casemask
 *	value_present = is value givin
 *	len = length of data
 *	data_p = pointer to data
 */
int 
update_build (Update_t         *upd_p,
              int              op,
              Timestamp_t      *ts_p,
              int              type,
              AttributeName_t  *att_p,
              unsigned int     *case_p,
              int              value_present,
              int              len,
              int              valuetype,
              byte_t           *data_p)
{
    register byte_t *next_p;
    dns_status_t status;

    INS8(upd_p->ud_operation, op);

    /* Copy or create timestamp */
    if (ts_p == NULL)
    {
        status = ts_new((Timestamp_t *)upd_p->ud_timestamp);
	dce_assert(cds__svc_handle, status == DNS_SUCCESS);
    }
    else
        COPY_Timestamp(ts_p, upd_p->ud_timestamp);

    next_p = upd_p->ud_attribute;

    /* Fill in attribute specifier */
    PUT8(next_p, type);

    if (type == AT_null)
	{
	/* No attribute name here, assert(!value_present); */
	PUT8(next_p, BL_false);
	return DIFF_bytes(next_p, upd_p);
	}

    /* Reapply casemask */
    names_simple_casemask((SimpleName_t *)att_p,
	(SimpleNameMask_t *)case_p, (SimpleName_t *)next_p);
    next_p = SKIP_AttributeName(next_p);

    /* Insert value */
    PUT8(next_p, value_present);
    if (!value_present)
	return DIFF_bytes(next_p, upd_p);

    /*
     * Atomic value output is dependent on the settings of len and data_p:
     *     len      data_p        action
     *      0         0           Return structure up to the value_present flag
     *     !0         0           Add length word and type
     *      0        !0           Add length word of value 0,type and
     *                                no data copied.
     *     !0        !0           Add length word, type plus data.
     *
     * This set of combinations provides for flexibility when appending
     * data to an update.
     */

    /* Always insert valuetype */
    if (data_p)
        {
        PUT16(next_p, len);
        PUT8(next_p, valuetype);
        COPY_bytes(data_p, next_p, len);
        return DIFF_bytes(SKIP_bytes(next_p, len), upd_p);
        }
    else if (len != 0)             /* data_p == nil is implied here */
        {
        PUT16(next_p, len);
        PUT8(next_p, valuetype);
        }
    else {
	/* Always add type here so we don't have to ifdef all the
         * places we build the data outside of this routine
	 * valuepresent=true, data_p = 0, len=0
	 */
        INS8(((AtomicValue_t *)next_p)->av_valuetype, valuetype);
	}

    return DIFF_bytes(next_p, upd_p);
}

/*
 * update_dns_build
 *       Build an update packet for known DNS attributes
 */
int 
update_dns_build (Update_t                *upd_p,
                  int                     op,
                  Timestamp_t             *ts_p,
                  attribute_descriptor_t  *dns_p,
                  int                     value_present,
                  int                     len,
                  byte_t                  *data_p)
{
  return(update_build(upd_p, op, ts_p, dns_p->type, 
		      (AttributeName_t *)dns_p->aname, 
		      &dns_p->casemask, value_present, len, 
		      dns_p->vtype, data_p));
}




