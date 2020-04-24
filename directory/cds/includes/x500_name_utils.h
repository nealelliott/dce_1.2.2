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
 * $Log: x500_name_utils.h,v $
 * Revision 1.1.5.2  1996/02/18  23:34:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:58  marty]
 *
 * Revision 1.1.5.1  1995/12/08  15:19:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:09  root]
 * 
 * Revision 1.1.3.2  1994/10/06  20:25:27  agd
 * 	expand copyright
 * 	[1994/10/06  14:11:52  agd]
 * 
 * 	expand copyright
 * 
 * Revision 1.1.3.1  1994/08/16  18:27:54  jd
 * 	drop from janet
 * 	[1994/08/13  16:06:22  jd]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1994. ALL RIGHTS RESERVED.
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
 * Common types and functions
 */

typedef struct {

    char            type;                    /* Replica type: Master/readonly*/
    char           *ch_uuid;                 /* Clearinghouse uuid           */
    char           *ch_name;                 /* Clearinghouse name           */
    int             tower_len;               /* Length of tower string       */
    char  	       *tower;                   /* Tower octet string           */
    
} x500_replica_t;                            /* recurring cell information   */
                                             /* with names and uuids in      */
                                             /* printable (ASCII) format.    */

typedef struct {

    char            *nsp_uuid;               /* Namespace uuid               */
    char            *root_uuid;              /* Root dir uuid (optional)     */
    char            *root_name;              /* Root dir name (optional)     */
    int              num_replicas;           /* Number of replicas           */

} x500_dns_cell_t;                           /* non-recurring cell info. with*/
                                             /* names and uuids in printable */

int 
x500_str_to_replica (
		     unsigned char   *,
                     int             ,
                     x500_replica_t  *);

int 
x500_str_to_cell (
		  unsigned char    *,
                  int              ,
                  x500_dns_cell_t  *);

int
x500_cell_to_str(
		 x500_dns_cell_t  *,
		 unsigned char   **, 
		 short            *);

int
x500_replica_to_str(
		    x500_replica_t  *,
		    unsigned char  **,
		    short           *);
