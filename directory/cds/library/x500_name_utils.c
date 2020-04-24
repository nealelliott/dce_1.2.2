/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: x500_name_utils.c,v $
 * Revision 1.1.7.1  1996/08/09  11:58:56  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:42 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Incorporate fix listed OT 12389 that fixes x500_str_to_cell.
 * 	[1996/02/18  19:35:16  marty  1.1.5.2]
 *
 * Revision 1.1.5.2  1996/02/18  19:35:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:27  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  15:27:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:52  root]
 * 
 * Revision 1.1.3.2  1994/10/06  20:25:45  agd
 * 	expand copyright
 * 	[1994/10/06  14:11:53  agd]
 * 
 * 	expand copyright
 * 
 * Revision 1.1.3.1  1994/08/16  18:27:57  jd
 * 	drop from janet
 * 	[1994/08/13  16:06:40  jd]
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
 * Contains routines used by the gda and gdsditadm to convert 
 * cell information to/from x.500 representations.
 *
 * Format of CDS_CELL attribute value:
 * <cell uuid, in ASCII>
 * 
 * Format of CDS_REPLICAS attribute values:
 * <replica type, 1 octet><clearinghouse uuid, in ASCII><0><clearinghouse name><0>
 * <towers length, 2 octets><tower #1, in ASCII><tower #2, in ASCII>...
 *
 */

#include <dce/dce.h>
#include <dcecdssvc.h>
#include <string.h>
#include <dce/x500_name_utils.h>
#include <dce/gda_common.h>
#define SHORTLEN 4
/*
 * x500_str_to_cell() - Takes a CDS_CELL attribute value (octet string)
 *                      and puts it into a x500_dns_cell_t structure.
 *                      The fields in the structure will point to positions
 *                      in the octet string.
 * Returns: 1 on success, 0 on error
 */

int 
x500_str_to_cell (unsigned char    *str,	/* IN:  Attribute value */
                  int              str_len,	/* IN:  Attr. value length */
                  x500_dns_cell_t  *cell)	/* OUT: Structure with fields*/
						/*      in printable format */
{
  if ((str == NULL) || (str_len <= 0) || (cell == NULL)) 
    {
      DCE_SVC_DEBUG((cds__svc_handle, cds_svc_gda, svc_c_debug2,
		     " <x500_str_to_cell null string passed in"));
      return(0);
    }
  cell->nsp_uuid = (char *)str;
  str += strlen((char *)str) + 1;

  if (*str == '\0')
    {
      cell->root_uuid = NULL;
      str += 1;
    }
  else
    {
      cell->root_uuid = (char *)str;
      str += strlen((char *)str) + 1;
    }

  if (*str == '\0')
    {
      cell->root_name = NULL;
    }
  else
    {
      cell->root_name = (char *)str;
    }

  return (1);
}                                  /* x500_str_to_cell() */


/*
 * x500_str_to_replica() - Takes a D2_CDS_REPLICA attribute value and extracts
 *                         the fields so that the fields in x500_replica_t
 *                         structure point to positions in the octet string.
 */

#define NUM_LEN_BYTES 4  /* no. octets holding tower length */

int 
x500_str_to_replica (unsigned char   *str,	/* IN:  Attribute value */
                     int             str_len,	/* IN:  Attr. value length */
                     x500_replica_t  *replica)	/* OUT: Structure with fields*/
						/*      in printable format */
{
  char  *cp;
  char   short_val[NUM_LEN_BYTES+1];

  if ((str == NULL)
      || (str_len <= 0)
      || (replica == NULL)) 
      {
	DCE_SVC_DEBUG((cds__svc_handle, cds_svc_gda, svc_c_debug2,
		       " <x500_str_to_replica(): Invalid input"));

	return (0);
      }

  cp = (char *)str;
  replica->type = *cp++;
  replica->ch_uuid = cp;
  while(*cp++ != '\0') {} 	/* skip to the end of ch_uuid */
  replica->ch_name = cp;
  while(*cp++ != '\0') 
      {
	/*
	 * skip to the end of ch_name 
	 */
      }
  
  /*
   * Note: memnove is used because  sscanf(%d) must have a \0 in the
   * source string after the short 
   */
  (void)memmove (short_val, cp, NUM_LEN_BYTES);
  short_val[NUM_LEN_BYTES] = '\0';
  replica->tower_len = (short)atoi(short_val);
  
  cp += NUM_LEN_BYTES;
  replica->tower = cp;

  return(1);
} 	/* x500_str_to_replica() */

/*
 * x500_cell_to_str() - Takes non-recurring CDS cell information in
 *                      structure form with printable string fields, and
 *                      constructs a D2_DNS_CELL attribute value octet string.
 *                      This octet string can then be stored in X.500.
 *  IN:
 *    cell : fields in printable format
 *  OUT
 *    str  : D2_DNS_CELL value.  Caller must free
 *    str_len : string length needed for x.500
 */
int
x500_cell_to_str(
		 x500_dns_cell_t  *cell,
		 unsigned char   **str, 
		 short            *str_len)

{
  char  *cp;
  short  total;

  if ((cell == NULL)
      || (cell->nsp_uuid == NULL)
      || (cell->num_replicas <= 0)) {

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_gda, svc_c_debug2,
		   "  x500_cell_to_str(): Invalid field in cell structure"));
    return(0);
  }

  /* Calculate total length of string     */
  total = strlen(cell->nsp_uuid) +1;

  if (cell->root_uuid == NULL)
      total++;
  else
      total += (strlen(cell->root_uuid) +1);

  if (cell->root_name == NULL)
      total++;
  else
      total += (strlen(cell->root_name) +1);

  if ((*str = (unsigned char *)malloc(total)) == NULL) {

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_gda,  svc_c_debug7,
		   "  x500_cell_to_str(): Malloc failed."));
    return(0);
  }

  *str_len = total;
  /* copy fields from the cell structure */
  /* into the octet string               */
  cp = (char *)*str;

  (void)strcpy(cp, cell->nsp_uuid);
  cp += (strlen(cell->nsp_uuid) +1);

  if (cell->root_uuid == NULL) {
    *cp++ = '\0';
  }
  else {
    (void)strcpy(cp, cell->root_uuid);
    cp += (strlen(cell->root_uuid) +1);
  }
  if (cell->root_name == NULL) {
    *cp = '\0';
  }
  else {
    (void)strcpy(cp, cell->root_name);
  }
  
  return(1);
}                                  /* x500_cell_to_str()                   */

/*
 * x500_replica_to_str() - Takes a structure which holds a single occurance
 *                         of the recurring type of CDS cell information
 *                         (Replica). The function constructs a D2_DNS_REPLICA
 *                         attribute value (octet string), which can
 *                         subsequently be stored in the X.500 directory.
 * IN:
 *   replica : Fields for attribute value
 * OUT:
 *   str     :  D2_DNS_REPLICA value. Caller must free
 *   str_len :  stirng length needed for x.500
 */

int
x500_replica_to_str(
		    x500_replica_t  *replica,
		    unsigned char  **str,
		    short           *str_len)
{
  char *cp;

  if ((replica == NULL)
      || ((replica->type != MASTER_REPLICA)
	  && (replica->type != READ_ONLY_REPLICA))
      || (replica->ch_uuid == NULL)
      || (replica->ch_name == NULL)
      || (replica->tower_len <= 0)
      || (replica->tower == NULL)) {

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_gda, svc_c_debug2,
	   "  x500_replica_to_str(): Invalid field in replica structure"));
    return(0);
  }

  /* Calculate total length of string     */
  *str_len = (sizeof(char)
	      + strlen(replica->ch_uuid) +1
	      + strlen(replica->ch_name) +1
	      + SHORTLEN
	      + replica->tower_len);

  if ((*str = (unsigned char *)malloc(*str_len)) == NULL) {
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_gda, svc_c_debug2,
		   "  x500_replica_to_str(): Malloc failed."));
    return(0);
  }

  /* copy fields from the structure to   */
  /* the octet string                    */
  cp = (char *)*str;

  *cp++ = replica->type;

  (void)strcpy(cp, replica->ch_uuid);
  cp += (strlen(replica->ch_uuid) +1);

  (void)strcpy(cp, replica->ch_name);
  cp += (strlen(replica->ch_name) +1);

  sprintf(cp, "%4d", replica->tower_len);
  cp += SHORTLEN;

  memmove( cp, (char *)(replica->tower), replica->tower_len);
  
  return(1);
}                                  /* x500_replica_to_str()                */






