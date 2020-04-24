/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscvtts.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:47  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:23:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:51  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:53  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:20  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:26  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:38  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:09  peckham
 * 	Eliminate use of dnstables.
 * 	[1994/04/29  14:17:43  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:31  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:27:59  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:55:39  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:10:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULE dnscvtts.c
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
 *
 */

#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <ctype.h>
#include <dce/dns_record.h>
#include <uc_clerk.h>

static const char to_hex[] = "0123456789ABCDEF";

/* ----------------------------------------------------------------------
 *  Convert string to Timestamp
 * ----------------------------------------------------------------------
 */
int 
cdsCvtStrToTS (byte_t     *string_p,
               cds_cts_t  *cts_p,
               byte_t     **end_pp)
{
    Timestamp_u notnormalized;
    byte_t *buf_p = (byte_t *)notnormalized;
    const int  strLen = strlen((char *)string_p);

    /* insist on xx-xx-xx or xxxxxx */
    if (strLen < ((sizeof(cds_cts_t)*3) - 1))
	if (strLen < (sizeof(cds_cts_t)*2))
	    return(CDS_ERROR);

    while (buf_p < &notnormalized[sizeof(notnormalized)]) {
	char nibbles[4], *nibble_p = nibbles, *end_p;
	long value;

	if (!isxdigit(*string_p)) {
	  if (end_pp) 
	    *end_pp = string_p;
	  return(CDS_ERROR);
	}
	/* Move value to high niblet */
	*nibble_p++ = *string_p++;

	if (!isxdigit(*string_p)) {
	  if (end_pp) 
	    *end_pp = string_p;
	  return(CDS_ERROR);
	}

	*nibble_p++ = *string_p++;
	*nibble_p = '\0';
	value = strtol(nibbles, &end_p, 16);
	if ((end_p != nibble_p) || (value < 0) || (255 < value)) {
	  if (end_pp) 
	    *end_pp = string_p;
	  return(CDS_ERROR);
	}
	*buf_p++ = value;

	if (*string_p == '-') {
	    string_p++; 
	}
    }

    if (cts_p) {
	/* Strings are in little endian byte format.  Convert for big endian */
#if BYTE_ORDER == LITTLE_ENDIAN
	COPY_Timestamp(notnormalized, cts_p);
#else
	/* Swap bytes */
	Timestamp_t *const in_p = (Timestamp_t *)&notnormalized;
	Timestamp_t *const out_p = (Timestamp_t *)cts_p;
	const int half = sizeof(in_p->ts_time)/2;
	register int i;

	/* Not id remains the same */
	COPY_Node(in_p->ts_node, out_p->ts_node);

	/* Swap bytes of low longword */
	for (i=0; i<half; i++) 
	    out_p->ts_time[i] = in_p->ts_time[half-1-i];

	/* Swap bytes of hi longword */
	for (i=0; i<half; i++)
	    out_p->ts_time[half+i] = in_p->ts_time[2*half-1-i];
#endif
    }
    if (end_pp) 
      *end_pp = string_p;

    return(CDS_SUCCESS);
}
/* ----------------------------------------------------------------------
 *  Convert Timestamp to Hex String
 * ----------------------------------------------------------------------
 */
void 
cdsCvtTsToStr (cds_cts_t  *cts_p,
               byte_t     *string_p)
{
    Timestamp_u normalized;
    Timestamp_t *const new_p = (Timestamp_t *)normalized;
    Timestamp_t *const oldcts_p = (Timestamp_t *)cts_p;
    byte_t *buf_p = (byte_t *)normalized;
    register int  i;

#if BYTE_ORDER == LITTLE_ENDIAN
    COPY_Timestamp(oldcts_p, new_p);
#else
    const int half = sizeof(oldcts_p->ts_time)/2;

    COPY_Node(oldcts_p->ts_node, new_p->ts_node);

    /* Swap bytes of low longword */
    for (i=0; i<half; i++) 
	new_p->ts_time[i] = oldcts_p->ts_time[half-1-i];

    /* Swap bytes of high longword */
    for (i=0; i<half; i++) 
      new_p->ts_time[half+i] = oldcts_p->ts_time[2*half-1-i];

#endif

    for (i = 0; i < sizeof(*cts_p); buf_p++,i++) {
	*string_p++ = to_hex[((*buf_p)>>4)&0x0F];
	*string_p++ = to_hex[(*buf_p)&0x0F];
	*string_p++ = '-';
    }
    string_p[-1] = '\0';
}

/* ----------------------------------------------------------------------
 *  Convert Timestamp to Ascii String
 * ----------------------------------------------------------------------
 */
void 
dnsCvtTsToAsc (cds_cts_t  *cts_p,
               byte_t     *string_p)
{
    format_ts ((Timestamp_t *)cts_p, (unsigned char *)string_p);
}
