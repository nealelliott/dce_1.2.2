/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: find_lib.c,v $
 * Revision 1.1.6.2  1996/02/18  19:36:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:52  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:31:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:24  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:04:18  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:25:41  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:43:32  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:05  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:11:50  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:43:36  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:22:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:22:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: find_lib.c
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
#include <back.h>	
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <uid.h>


/*
 *
 * find_last_value
 *    This routine returns a pointer to the last valid attribute value.
 *  If none are found, NULL is returned.
 *
 * Input:
 *     set_p = address of attribute set
 *
 * Value:
 *     val_p = pointer to last valid value
 */
DBSetMember_t 
*find_last_value (const DBSet_t *const set_p)
{
    register int cnt = NUM_DBSet(set_p);

    while (0 <= --cnt)
        {
	register const DBSetMember_t *const val_p =
				(DBSetMember_t *)INDEX_DBSet(set_p, cnt);

        if (EXT8(val_p->dm_valid) && EXT8(val_p->dm_value_present))
	    return((DBSetMember_t *)val_p);
        }
    return(NULL);
}

/*
 * find_known_attribute
 *     This routine searches the list of known attributes and returns
 * a pointer.
 *
 * Input:
 *     attribute_p = address of attribute to look for
 *
 * Value:
 *     pointer if found, null otherwise.
 *
 */
attribute_descriptor_t 
*find_known_attribute (const AttributeName_t *const attribute_p)
{
    register int lower, upper, pos;
    register int result;

    lower = -1;
    upper = DNS_MAXATTRIBUTES;
    pos = upper/2;

    /* Binary search through alphabatized list */
    while ((pos < upper) && (pos > lower))
        {
        result = dnsCmpSimple((dns_opq_sname_t *)attribute_p,
			      (dns_opq_sname_t *)dns_AttributeName(pos));

        /* match was found */
        if (result == 0)
            return(dns_Attribute(pos));

        /* attribute is greater then current position */
        if (result > 0)
            {
            lower = pos;
            pos += (upper-lower+1)/2;
            }
        else 
        /* attribute is less then current position */
            {
            upper =pos;
            pos -= (upper-lower+1)/2;
            }
        }
    return((attribute_descriptor_t *)0);
}

/*
 * find_attribute
 *   This routine searches a record for a specific attribute set.
 * It searches through the set of attributes within the record.
 *
 *
 * Input:
 *      rec_p = address of db record
 *      attribute_p = address of attribute simple name
 *      
 * Output:
 *      set_pp = address to return pointer to attribute found, 
 *               pointing past name, or pointer to where attribute name belongs.
 *
 *      index_p = address to return index of first entry >= to input.
 *
 * Function value:
 *      DNS_SUCCESS - attribute found and present
 *      DNS_ABSENT - attribute found but not present
 *      DNS_UNKNOWNENTRY - attribute name not found, 
 */
dns_status_t 
find_attribute (const data_record_t *const    rec_p,
                const AttributeName_t *const  attribute_p,
                DBSet_t                       **set_pp,
                int                           *index_p)
{
    register DBSet_t *main_p = (DBSet_t *)rec_p->dbr_data;
    const int top = NUM_DBSet(main_p);
    register int lower = -1;
    register int upper = top;
    register int pos = upper/2;
    register int result;

    ALIGN_CHECK(main_p);

    if (upper == 0)
        {
        *index_p = 0;
        *set_pp = (DBSet_t *)INDEX_DBSet(main_p, 0);
        return(DNS_UNKNOWNENTRY);
        }


    /* Binary search through alphabatized list */
    while ((lower < pos) && (pos < upper))
        {
	AttributeName_t *name_p = (AttributeName_t *)INDEX_DBSet(main_p, pos);
        result = dnsCmpSimple((dns_opq_sname_t *)attribute_p,
			      (dns_opq_sname_t *)name_p);

        if (result == 0)    /* Match was found, return pointers */
            {
            *index_p = pos;
            *set_pp = (DBSet_t *)SKIP_AttributeName(name_p);
	    ALIGN_CHECK(*set_pp);
            if (FLAG_IS_SET((*set_pp)->ds_flag, DS_present))
                return(DNS_SUCCESS);
            else
                return(DNS_ABSENT);
            }

        if (result > 0)     /* Attribute is greater then current position */
            {
            lower = pos;
            pos += (upper-lower+1)/2;
            }
        else 
            {               /* Attribute is less then current position */
            upper =pos;
            pos -= (upper-lower+1)/2;
            }
        }

    /* Return pointer where attribute name should go. */
    *index_p = upper;
    *set_pp = (DBSet_t *)((upper < top)
	? INDEX_DBSet(main_p, upper)
	: SKIP_DBSet(main_p));
    return(DNS_UNKNOWNENTRY);
}

/*
 * find_ts_position
 *   This routine searches an attribute set the position in a set
 * where a timestamp should go.  Entries must be stored in increasing 
 * time order.  Matches are considered as less then.
 *
 *
 * Input:
 *      set_p = address of attribute set
 *      ts_p = address of timestamp
 *      
 * Output:
 *      retvalue_pp = address to return pointer to where it should go.
 *      index_p = address to return index of first entry 
 *
 */
void 
find_ts_position (const DBSet_t *const      set_p,
                  const Timestamp_t *const  ts_p,
                  DBSetMember_t             **retvalue_pp,
                  int                       *index_p)
{
    const int top = NUM_DBSet(set_p);
    register int lower = -1;
    register int upper = top;
    register int pos = upper/2;
    register int result;

    /* Binary search through alphabatized list */
    while ((pos < upper) && (pos > lower))
        {
        *retvalue_pp = (DBSetMember_t *)INDEX_DBSet(set_p, pos);
	result = dnsCmpCTS((dns_cts *)ts_p, (dns_cts *)(*retvalue_pp)->dm_ts);

        if (result > 0)         /* Timestamp is greater then current position */
            {
            lower = pos;
            pos += (upper-lower+1)/2;
            }
        else if (result < 0)
            {                   /* Timestamp is less then current position */
            upper =pos;
            pos -= (upper-lower+1)/2;
            }
        else /* Equal */
            { /* Find the first one I am less then, may be multiple matches */
            lower = pos;
            pos++;
            }
        }

    /* Return where new value should go */
    *index_p = upper;
    *retvalue_pp = (DBSetMember_t *)((upper < top)
	? INDEX_DBSet(set_p, upper)
	: SKIP_DBSet(set_p));
}
