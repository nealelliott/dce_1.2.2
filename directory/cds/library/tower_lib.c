/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tower_lib.c,v $
 * Revision 1.1.10.2  1996/02/18  19:35:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:19  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:40  root]
 * 
 * Revision 1.1.6.9  1994/08/03  19:03:19  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:10:53  mccann]
 * 
 * Revision 1.1.6.8  1994/07/06  20:43:47  ohara
 * 	include sys/types.h
 * 	[1994/07/06  20:08:14  ohara]
 * 
 * Revision 1.1.6.7  1994/06/30  19:17:58  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:05:05  mccann]
 * 
 * Revision 1.1.6.6  1994/06/23  19:07:33  jd
 * 	Remove extern evtblk. It no longer exists.
 * 	[1994/06/23  15:40:12  jd]
 * 
 * Revision 1.1.6.5  1994/06/09  18:42:33  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:48  devsrc]
 * 
 * Revision 1.1.6.4  1994/04/29  15:55:47  peckham
 * 	Remove unnecessary conditionals.
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	[1994/04/29  15:47:36  peckham]
 * 
 * Revision 1.1.6.3  1994/04/28  17:19:27  ohara
 * 	include sys/types.h
 * 	[1994/04/28  16:45:30  ohara]
 * 
 * Revision 1.1.6.2  1994/03/16  23:43:35  jd
 * 	Included server.h to pick up definition of struct nsgbl_ns.
 * 	[1994/03/16  21:29:13  jd]
 * 
 * Revision 1.1.6.1  1994/03/12  22:09:04  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:35:48  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  14:17:35  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:17:37  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/07  15:32:49  mfox
 * 	Define an in_addr variable and use it when calling inet_ntoa().
 * 	Also include arpa/inet.h instead of declaring inet_ntoa().
 * 	[1992/12/07  15:31:13  mfox]
 * 
 * Revision 1.1.2.6  1992/06/26  07:03:22  jim
 * 	Merge AIX 3.2 changes with the latest level.
 * 	[1992/06/22  22:40:25  jim]
 * 
 * Revision 1.1.2.5  1992/06/26  05:36:28  jim
 * 	Added include of server.h to fix an AIX compiler message.
 * 	"/project/dce/build/dce1.0.1/src/directory/cds/library/tower_lib.c", line 97.1: 1
 * 	506-103 (S) Tag nsgbl_ns requires a complete definition before it is used.
 * 	[1992/06/23  15:52:24  jim]
 * 
 * Revision 1.1.2.4  1992/06/11  15:31:45  mfox
 * 	#include stdio.h so that it compiles without -DDEBUG
 * 	[1992/06/10  18:06:32  mfox]
 * 
 * Revision 1.1.2.3  1992/06/01  15:39:13  mfox
 * 	Fix "16 bits into an 8 bit bag". Replaced word_t (a char *) with unsigned
 * 	short. Plus a couple of additional lines of ANSI C fixups.
 * 	[1992/06/01  15:38:11  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:24:36  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:33:19  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module tower_lib.c
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
#include <sys/types.h>
#include <dce/dce_error.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <stdlib.h>
#include <string.h>
#include <dce/rpc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <dce/dnsclerk.h>
#include <dce/dns_record.h>
#include <dce/rpc.h>
#include <rpcserver.h>
#include <dce/cds_clerkserver.h>
#include <tower.h>
/*
 * Local Prototypes...
 */
int
get_binding_string (
	byte_t		*,
	char		*);


static char TCP_protocol[] = "ncacn_ip_tcp";
static char UDP_protocol[] = "ncadg_ip_udp";

rpc_if_handle_t	cds_ifspec;

#define MAX_STRING 200	/* max of bind string or tower data	*/

/*
 * towerset_compare_exact
 *    checks that the two towers have the same members.  Ignores
 *    timestamps.  This is done using the USER version of sets.
 *
 * Input:
 *     towerset1_p = pointer to first tower set
 *     towerset2_p = pointer to second tower set
 * Output:
 *
 * Value:
 *     returns TRUE if the towers are equivalent except for
 *     order and timestamps.
 *
 * Side-effect:
 *     writes in the second towerset - marks things absent that match.
 */
int 
towerset_compare_exact (Set_t  *towerset1_p,
                        Set_t  *towerset2_p)
{
    int                   num_mem1;
    int                   num_mem2;
    int                   i,j,found = FALSE;
    MemberValue_t        *member1_p;
    MemberValue_t        *member2_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug7,
        " >towerset_compare_exact()"));

    num_mem1 = NUM_Set(towerset1_p);
    num_mem2 = NUM_Set(towerset2_p);

    for (i = 0; i < num_mem1; ++i)
        {
        member1_p = (MemberValue_t *)INDEX_Set(towerset1_p, i);
        if (FLAG_IS_CLEAR(member1_p->mv_flag, MV_present)) continue;
        found = FALSE;
        for (j = 0; j < num_mem2; ++j)
            {
            member2_p = (MemberValue_t *)INDEX_Set(towerset2_p, j);
            if (FLAG_IS_CLEAR(member2_p->mv_flag, MV_present))
		continue;
	    if (EXT8(member1_p->mv_valuetype) != EXT8(member2_p->mv_valuetype))
	      continue;
            if (EXT16(member1_p->mv_length) != EXT16(member2_p->mv_length))
		continue;
            if (NE_bytes(member2_p->mv_value, member1_p->mv_value,
				EXT16(member1_p->mv_length)))
		continue;

            /* mark the member in the second set absent */
            CLEAR_FLAG(member2_p->mv_flag, MV_present);
            found = TRUE;
            break;
            }
	if (!found) return (found);  /* not a match at all */
        }

    /* check for any present members in the second list */
    for (j = 0; j < num_mem2; ++j)
        {
        member2_p = (MemberValue_t *)INDEX_Set(towerset2_p, j);
        if (FLAG_IS_SET(member2_p->mv_flag, MV_present)) found = FALSE;
        }

    return (found);
}

/*
 * towerset_compare_contained
 *    checks that the two towers have the same members.  Ignores
 *    timestamps.  This is done using the USER version of sets.
 *    The check is just that the first set is contained in the second
 *    set.  The second set may contain more members than the first.
 *
 * Input:
 *     towerset1_p = pointer to first tower set
 *     towerset2_p = pointer to second tower set
 * Output:
 *
 * Value:
 *     returns TRUE if the first towerset is contained in the second towerset
 *     except for order and timestamps.
 */
int 
towerset_compare_contained (Set_t  *towerset1_p,
                            Set_t  *towerset2_p)
{
    int                   num_mem1;
    int                   num_mem2;
    int                   i,j,found;
    MemberValue_t        *member1_p;
    MemberValue_t        *member2_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug7,
        " >towerset_compare_contained()"));

    num_mem1 = NUM_Set(towerset1_p);
    num_mem2 = NUM_Set(towerset2_p);

    for (i = 0; i < num_mem1; ++i)
        {
        member1_p = (MemberValue_t *)INDEX_Set(towerset1_p, i);
        if (FLAG_IS_CLEAR(member1_p->mv_flag, MV_present))
	    continue;
        found = FALSE;
        for (j = 0; j < num_mem2; ++j)
            {
            member2_p = (MemberValue_t *)INDEX_Set(towerset2_p, j);
            if (FLAG_IS_CLEAR(member2_p->mv_flag, MV_present))
		continue;
	    if (EXT8(member1_p->mv_valuetype) != EXT8(member2_p->mv_valuetype))
	      continue;
            if (EXT16(member1_p->mv_length) != EXT16(member2_p->mv_length))
		continue;
            if (NE_bytes(member2_p->mv_value, member1_p->mv_value,
				EXT16(member1_p->mv_length)))
		continue;
            found = TRUE;
            break;
            }
        if (!found) break;
        }

    return (found);
}


/*
 * cds_LocalTowers
 *	This routine gives the user a copy of the local towers.
 *
 * Inputs:
 *	towerset_p = address of area to receive tower set.
 *
 * Returns:
 *	length of towerset
 */
int 
cds_LocalTowers (rpc_binding_vector_t  *binding_vector,
                 byte_t                *towerset_p)
{
    int 		i, j;
    rpc_tower_vector_t	*tower_vector;
    Set_t               *st_p = (Set_t *)towerset_p;
    MemberValue_t       *mv_p;
    int                 set_off = 0;
    error_status_t      rpc_status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug7,
        " >cds_LocalTowers"));

    /*
     * Construct a CDS tower set. We get the binding vector from nsgbl_ns,
     * which is filled in by rpc_server_init, 5 Feb 1992
    */

    if ( !binding_vector )
    {
      /* Build empty set */
      ZERO_Set(towerset_p);
      DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug7,
            " <cds_LocalTowers: rpc_server_init(3rpc) left no bind vector - return(5)"));
      
	return(sizeof(NULLSET_u));
    }

    INS8(st_p->st_flag, ST_present);
    INS16(st_p->st_offsetLength, binding_vector->count*sizeof(word_u));
    mv_p = (MemberValue_t *)DATA_Set(st_p);

    for (i = 0; i < binding_vector->count; i++) {
	/*
	 * Clear out the port no. from the binding
	 */
	rpc_binding_reset (binding_vector->binding_h[i], &rpc_status);
	if (rpc_status != rpc_s_ok) 
	{
	    dce_svc_printf(CDS_S_RPCBINDINGRESET_MSG, (long)rpc_status);
  	    return(0);
	}

	/*
	 * Get the towers from each binding. Use appropriate IFSPEC
	 * to get upper floors.
	 */
	rpc_tower_vector_from_binding ( cds_ifspec,
					binding_vector->binding_h[i],
					&tower_vector,
					&rpc_status);
	if (rpc_status != rpc_s_ok) 
	{
	    dce_svc_printf(CDS_S_RPCTOWERVECTORFROMBIND_MSG, (long)rpc_status);
	    return(0);
	}

	/*
	 * For each tower, insert another set member
	 */
	for (j = 0; j < tower_vector->count; j++) 
	{
	    INS8(mv_p->mv_flag, MV_present);
	    COPY_Timestamp ( NullTimestamp, mv_p->mv_timestamp );
	    INS16(mv_p->mv_length, tower_vector->tower[j]->tower_length);
	    INS8(mv_p->mv_valuetype, VT_byte);
	    COPY_bytes(&tower_vector->tower[j]->tower_octet_string,
		       mv_p->mv_value, tower_vector->tower[j]->tower_length);
	    /*
	     * set the offset to this SetMember and skip to next
	     */
	    INS16(OFFSET_Set(st_p, set_off++), DIFF_bytes(mv_p, st_p));
	    mv_p = (MemberValue_t *)SKIP_MemberValue(mv_p);
	}
	/*
	 * Free the tower vector
	 */
	rpc_tower_vector_free(&tower_vector, &rpc_status);
    }
    /*
     * Fill in the value legth for the set, and return the length of the set
    */
    INS16(st_p->st_valueLength,  DIFF_bytes(mv_p, DATA_Set(st_p)));
    return((byte_t *)mv_p - towerset_p);
}


/*
**	cdsCvtStrToTower
**
**	Input:	Pointer to an array of pointers to individual null terminated
**		RPC string bindings. The array must be terminated by a null
**		pointer. The array and the strings need not be in contiguous
**		memory.
**
**	Output:	The Towerset is a set as defined in the DNS Architecture
**		Specification, containing a tower for each of the RPC
**		binding strings contained in Bindings.
**
**	Possible returns:
**
**	DNS_SUCCESS	The operation completed successfully. The caller
**			must free the memory allocated for the Towerset.
**
**	DNS_NOTFND	The string representations of the RPC binding
**			information are empty, i.e. contain only null strings.
**			The returned output parameter is null.
**
**	DNS_NOROOM	Insufficient memory to allocate the Towerset.
**			The returned output parameter is null.
**
**	DNS_INVALIDARGUMENT	The string representations of the RPC
**			binding information contain an invalid address or
**			contain an incorrect protocol specification (neither
**			TCP nor UDP). The returned output parameter is null.
*/

int 
cdsCvtStrToTower (char  *binding_p[],
                  char  **towerset_p)
{
    int i, j, tower_count;
    Set_t *set_p;
    MemberValue_t *mv_p;
    error_status_t rpc_status;
    rpc_binding_handle_t binding;
    rpc_tower_vector_t *tower_vector;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug7,
        " >cdsCvtStrToTower"));

    /*
     * first count required towers, and allocate required space  
     */

    *towerset_p = NULL;	/* just in case we fail...	*/
    for ( tower_count = 0; binding_p[tower_count] != 0; tower_count++ )
	;

    if ( !tower_count )
    {
	return ( DNS_NOTFND );
    }

    if ( !( set_p = (Set_t *) malloc (tower_count * MAX_STRING)))
    {
	return ( DNS_NOROOM );
    }

    /*
     * build the TowerSet header, set pointer to offsets	
     */

    INS8 ( set_p->st_flag, ST_present );
    INS16 ( set_p->st_offsetLength, tower_count * sizeof(word_u));
    mv_p = (MemberValue_t *)DATA_Set(set_p);

    for ( i=0; i<tower_count; i++ ) 
    {
	rpc_binding_from_string_binding ((unsigned char *)binding_p[i],
					 &binding,
					 &rpc_status);
	if (rpc_status != rpc_s_ok) 
	{

	    dce_svc_printf(CDS_S_RPCBINDINGFROMSTRING_MSG, (long)rpc_status);
	    free ((char *)set_p);
	    return (DNS_INVALIDARGUMENT);
	}
	rpc_tower_vector_from_binding ( cds_ifspec, binding,
					&tower_vector, &rpc_status);
	if (rpc_status != rpc_s_ok) 
	{
	    dce_svc_printf(CDS_S_RPCTOWERVECTORFROMBIND_MSG, (long)rpc_status);
	    free ((char *)set_p);
	    rpc_binding_free ( &binding, &rpc_status);
	    return (DNS_INVALIDARGUMENT);
	}

	rpc_binding_free ( &binding, &rpc_status);

	/*
	 * For each tower, insert another set member
	 */
	for (j = 0; j < tower_vector->count; j++) {
		INS8(mv_p->mv_flag, MV_present);
		COPY_Timestamp ( NullTimestamp, mv_p->mv_timestamp );
		INS16(mv_p->mv_length, tower_vector->tower[j]->tower_length);
		INS8(mv_p->mv_valuetype, VT_byte);
		COPY_bytes(&tower_vector->tower[j]->tower_octet_string,
		    mv_p->mv_value, tower_vector->tower[j]->tower_length);
		/*
		 * set the offset to this SetMember and skip to next
		 */
		INS16(OFFSET_Set(set_p, i), DIFF_bytes(mv_p, set_p));
		mv_p = (MemberValue_t *)SKIP_MemberValue(mv_p);
	}
	/*
	 * Free the tower vector
	 */
	rpc_tower_vector_free(&tower_vector, &rpc_status);
    }
    /* Fill in the value legth for the set    */
    INS16 ( set_p->st_valueLength,  DIFF_bytes(mv_p, DATA_Set(set_p)));
    *towerset_p = (char *)set_p;	/* pass pointer to caller */
    return ( DNS_SUCCESS );
}


/*
**	cdsCvtTowerToStr
**
**	Input:	Pointer to an array of pointers to individual null terminated
**		RPC string bindings. The array must be terminated by a null
**		pointer. The array and the strings need not be in contiguous
**		memory.
**
**	Output:	The Towerset is a set as defined in the DNS Architecture
**		Specification, containing a tower for each of the RPC
**		binding strings contained in Bindings.
**
**	Possible returns:
**
**	DNS_SUCCESS	The operation completed successfully. The caller
**			must free the memory allocated for the Towerset.
**
**	DNS_NOTFND	The string representations of the RPC binding
**			information are empty, i.e. contain only null strings.
**			The returned output parameter is null.
**
**	DNS_NOROOM	Insufficient memory to allocate the Towerset.
**			The returned output parameter is null.
**
**	DNS_INVALIDARGUMENT	The string representations of the RPC
**			binding information contain an invalid address or
**			contain an incorrect protocol specification (neither
**			TCP nor UDP). The returned output parameter is null.
*/

int 
cdsCvtTowerToStr (void           *tower_p,
                  unsigned char  **binding_p)
{
	char **array;
	char *pointer;
	int i, count, asize, tsize;
	byte_t *next_tower;
	MemberValue_t *mv;

	*binding_p = NULL;		/* just in case we fail...	*/

	count = NUM_Set(tower_p);
	asize = (count + 1) * sizeof(char *) + count * MAX_STRING;
	if ( !(*binding_p = malloc(asize)))
		return ( DNS_NOROOM );

	array = (char **)*binding_p;
	ZERO_bytes(*binding_p, asize);
	pointer = (char *)array + (count+1) * sizeof(char *);
	for (i = 0; i < count; i++) {
		array[i] = pointer;
		mv = (MemberValue_t *)INDEX_Set(tower_p, i);
		next_tower = mv->mv_value;
		tsize = EXT16(mv->mv_length);
		asize = get_binding_string ( next_tower, array[i] );
		if (asize)
			pointer += asize;
		else {
			free ((char *)*binding_p);
			*binding_p = NULL;
			return ( DNS_INVALIDARGUMENT );
		}
	}

	/* verify that we build at least one RPC binding string		*/

	if (pointer == ((char *)array + (count+1) * sizeof(char *)) + count ){
		free ((char *)*binding_p);
		*binding_p = NULL;
		return ( DNS_NOTFND );
	}
	else return(DNS_SUCCESS);
}


/*
 * Turn an RPC TowerSet member into a single binding string
 */
int 
get_binding_string (byte_t  *data_p,
                    char    *binding_p)
{
  rpc_binding_handle_t binding;
  unsigned char *string;
  error_status_t dce_status, rpc_status;
  int status = 0;

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug7,
            " >get_binding_string()"));

  rpc_tower_to_binding (data_p, &binding, &rpc_status);
  if (( rpc_status == rpc_s_not_rpc_tower )
      && get_cds_binding_string ( data_p, binding_p )) 
      {
	status = strlen(binding_p) + 1;
      }
  else if ( rpc_status == rpc_s_protseq_not_supported ) 
      {
	dce_error_inq_text(rpc_status, binding_p, &dce_status);
	status = strlen(binding_p) + 1;
      }
  else if ( rpc_status != rpc_s_ok )
      {
	dce_svc_printf(CDS_S_RPCTOWERTOBINDING_MSG, (long)rpc_status);
      }
  else
      {
	rpc_binding_to_string_binding(binding, &string, &rpc_status);
	rpc_binding_free (&binding, &dce_status);

	if (rpc_status != rpc_s_ok) 
	    {
	      dce_svc_printf(CDS_S_RPCSTRINGTOBINDING_MSG, (long)rpc_status);
	      
	    }
	else
	    {
	      strcpy(binding_p, (char *)string);
	      rpc_string_free((unsigned char **)&string, &dce_status);
	      status = strlen(binding_p) + 1;
	    }
      }

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug7,
            " <get_binding_string() return(%d)",
	    status));

  return(status);
}


/*
 * Turn a non-RPC TowerSet member into a single binding string
 */
int 
get_cds_binding_string (byte_t  *data_p,
                        char    *binding_p)
{
	char	*str_p = binding_p;
	char	*Saddress_p;
	char	*prot_p;
	byte_t	id;
	unsigned short	cnt, len;
	struct  in_addr addr;

	*str_p = '\0';			/* at minimum, return a null string */
	cnt = GETL16(data_p); /* get the number of tower floors */

	for (/* cnt - assigned on previous line. gcc(1) compiler   */ 
             /* gives "stmt with no effect if left just as "cnt;"  */
	     ;
             cnt>0;  
	     --cnt) 
	{
	    len = GETL16(data_p);
	    id = EXTL8(data_p);
	    data_p += len;

	    /* skip non-protocol tower floors				*/

	    if ((len == 1) && ((id == TCP_ID) || (id == UDP_ID))) 
	    {
		/*
		 * We use the RPC port, so this value is not interesting 
		 */
		len = GETL16(data_p); /* Stored little endian */
		data_p += len;

		/* Really want to get IP (host) address from the next floor.
		 * Point of no return.  If anything fails here return FALSE.
		 * this code assumes the 2 floors are in order.  Since we
		 * start parsing the 2nd floor we cannot return to the for
		 * loop without the floor count being incorrect.	*/

		len = GETL16(data_p);
		if (EXTL8(data_p) == IP_ID) {
		    data_p += len;
		    len = GETL16(data_p);
		    if (len == sizeof(addr)) {

			/* Build string binding representation of address */

			if (id == UDP_ID)
			prot_p = UDP_protocol;
			else
			prot_p = TCP_protocol;
			while (*prot_p != '\0') { /* copy the protocol seq */
			    *(str_p++) = *(prot_p++);
			}
			*(str_p++) = ':';	/* add the separator	*/

			/* Convert network address to string */

			COPY_bytes(data_p, &addr, len);
			Saddress_p = (char *)inet_ntoa(addr);
			strcpy(str_p, Saddress_p);   /* copy the address */
			return ( strlen(binding_p) + 1 ); /* success !!! */

		    }
		    else return ( FALSE ); /* length != sizeof(addr) */
	        }
		else return ( FALSE ); /* id != ID_IP, floors not in order */
	    }
	    else {
	        len = GETL16(data_p);	/* Skip over value field */
	        data_p += len;
	    }
	}		/* End for loop */
	return ( TRUE );
}
