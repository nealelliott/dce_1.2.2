/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_cdslis.c,v $
 * Revision 1.1.13.2  1996/02/18  18:18:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:58  marty]
 *
 * Revision 1.1.13.1  1995/12/08  15:55:21  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:31 UTC  dtruong
 * 	Merge to DCE 1.1
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/27  20:45  shu
 * 	post-KK merge
 * 	shu's change in the routine make_result_contiguous() is not
 * 	rolled in since the SNI change (using D2_ALIGN()) essentially
 * 	provided the same fix to the alignment problem
 * 	[1995/12/08  15:11:04  root]
 * 
 * Revision 1.1.11.1  1994/02/22  18:06:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:38:07  marrek]
 * 
 * Revision 1.1.9.1  1993/11/03  20:31:50  root
 * 	CR 9307 - alignment problem
 * 	[1993/11/03  20:31:20  root]
 * 
 * Revision 1.1.6.3  1993/12/17  23:13:34  jake
 * 	GDS What string changes
 * 	[1993/12/17  23:13:17  jake]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  11:02:23  marrek]
 * 
 * Revision 1.1.6.2  1993/11/05  19:16:46  shu
 * 	list_space in the routine make_result_contiguous() has
 * 	to be kept on four-byte boundary.  Otherwise a bus error
 * 	could occur on the HP platform when running xt_test in
 * 	the test suite xds/tests.switch/valid - list3v.
 * 	[1993/11/05  19:15:37  shu]
 * 
 * 	Insert <pthread.h>
 * 	fixes for OT 6554
 * 	[1993/02/02  09:30:13  marrek]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:50  bbelch]
 * 
 * 	Remove empty #ifndef lint and #ifdef ANSIC.
 * 	Remove macro STRINGCAST(x) and use explicite cast to (char *).
 * 	[1992/11/06  11:36:04  marrek]
 * 
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:24:21  treff]
 * 
 * Revision 1.1.2.4  1993/10/19  18:19:53  root
 * 	    Loading
 * 
 * Revision 1.1.2.9  1992/07/07  21:12:50  melman
 * 	Changes by Helmut, several \001 to \003
 * 	[1992/07/07  21:11:55  melman]
 * 
 * Revision 1.1.2.8  1992/07/06  19:39:56  melman
 * 	Fixes for switch failures CR 4610, 4582
 * 	[1992/07/06  19:38:27  melman]
 * 
 * Revision 1.1.2.7  1992/07/01  17:47:49  melman
 * 	Fixed merge problem in GDS drop of 6/26/92
 * 	[1992/07/01  17:47:12  melman]
 * 
 * Revision 1.1.2.6  1992/07/01  16:53:14  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:22:10  melman]
 * 
 * Revision 1.1.2.5  1992/06/30  20:59:10  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:39:50  melman]
 * 
 * Revision 1.1.2.4  1992/06/29  22:23:35  melman
 * 	Bug 4344: Added an #include of string.h
 * 	Bug 4344: Change arguments to strcpy in enum_alias_objs().
 * 	    Was:	    strcpy (cds_filter, "*");
 * 	    Changed to:	    strcpy (cds_filter.dns_opq_sname, "*");
 * 	Bug 4344:  Changed the both of the above from ".dns_opq_sname" (which
 * 	did not compile with a "structure has no member named
 * 	`cds_simple_name_t'") to ".uc" (which did compile)
 * 	[1992/06/24  17:21:22  melman]
 * 
 * Revision 1.1.2.3  1992/06/02  03:38:02  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:39:11  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:42:59  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:38:36  weisman]
 * 
 * Revision 1.1  1992/01/19  15:13:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$Id: d21_cdslis.c,v 1.1.13.2 1996/02/18 18:18:00 marty Exp $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*---------------------------------------------------------------------

	Filename:		d21_cdslis.c

	Author:			P. Dunne
	Date:			1.02.1991

	Component:		OSF DCE Naming Switch

	Functions:	
		exported:       d21_4_switch()

		static:         enum_std_objs(),    enum_alias_objs(),	
		                enum_child_objs(),  free_list_result(),
		                size_list_result(), make_result_contiguous().	

	Version:		1.0 (initial)

	Update History:
	Version:    Date:        Comments:                By whom:
	--------------------------------------------------------------------
	1.0         1.02.91      original                 P. Dunne


 -----------------------------------------------------------------------*/

#include <gds.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>
#include <dce/dnsclerk.h>
#include <d21_cdssw.h>

/****************** local function prototypes ***************************/

static int		enum_std_objs (	
					D2_l_info			**list_head,	
					D2_l_info			**list_tail,
					dns_full_name_t		*cds_name,
					struct dnsFlagStat	*dns_flags,
					D2_pbhead			*list_error);

static int		enum_alias_objs (	
					D2_l_info			**list_head,
					D2_l_info			**list_tail,
					dns_full_name_t		*cds_name,
					struct dnsFlagStat	*dns_flags,
					D2_pbhead			*list_error);

static int		enum_child_objs (	
					D2_l_info			**list_head,
					D2_l_info			**list_tail,
					dns_opq_fname		*cds_name,
					struct dnsFlagStat	*dns_flags,
					D2_pbhead			*list_error);

static void		free_list_result (	
					D2_l_info			*list_result);

static int		size_list_result (	
					D2_l_info			*list_result);

static int		make_result_contiguous (	
					D2_l_info			**list_result,
					D2_pbhead			*list_error);

/*--------------------------------------------------------------------------*/

/************************************************************************

	d21_4_switch():

	Lists all the subordinate objects of a given object in the 
	local DNS cell. The list operation is received at the IAPL*
	interface and has to be mapped to DNSPI interface calls. 
	There are three types of object in DNS, aliases, groups and
	other objects. A seperate function must be called to list
	each of these types of object.

	The result from the DNSPI list operations is mapped to an 
	IAPL* list operation result.

 ************************************************************************/

int		d21_4_switch (
			D21_p4	*list_par_blk)
{
	dns_full_name_t		*dns_fname,		cds_alias;
	dns_opq_fname       part_result;
	struct dnsFlagStat	dns_flags;
	D2_l_info			*list_head,		*list_tail,		*head,	*tail;
	D2_pbhead			*list_error;
    D2_c_res            *c_results;
	u_char				cds_name[D2_DNL_MAX];
	int	deref_alias,	alias_found,	r_code;
	const char function_name[] = "d21_4_switch";


	DUA_TRACE_IAPL_ENTRY
	r_code = D2_NOERROR;
	list_par_blk->d21_4res = (D2_l_info *) NULL;
	list_head = (D2_l_info *) NULL;

	/*
		The pbhead and D2_c_res structures are allocted by the 
        IAPL* user.
																*/
	list_error = list_par_blk->d21_4pbhead;
    c_results = list_par_blk->d21_4cres;
    c_results->d2_al_deref = FALSE;

	if (d21_x500name2cdsname (list_par_blk->d21_4parent, cds_name) 
		== D2_ERROR) {
		DUA_TRACE_X500_TO_CDS_NAME_FAILED
		list_error->d2_retcode = D2_CALL_ERR;
        list_error->d2_errclass = D2_PAR_ERR;
        list_error->d2_errvalue = D2_NOT_SPEC;
		return (D2_ERROR);
	}

	dns_fname = (dns_full_name_t *) cds_name;

 	/*
        IAPL* common arguments are mapped to DNS flags structure.
                                                                    */
    (void) d21_cds_set_flags (&dns_flags, list_par_blk->d21_4c_a);
 	dns_flags.fsPartialRes = &part_result;
    dns_flags.fsResLength = sizeof (part_result);

	/*
		checks to see if an alias has been traversed. 
														*/
	if (d21_cds_check_for_alias (dns_fname, &cds_alias, &alias_found, 
		&dns_flags, list_error) == D2_ERROR) {

		d21_cds_error_info (list_error, dns_fname, &list_par_blk->d21_4err);
		r_code = D2_ERROR;

	} else {

		if (list_par_blk->d21_4c_a->d2_serv_cntrls & D2_DREFALIAS)
			deref_alias = FALSE;
		else 
			deref_alias = TRUE;

		if (alias_found && deref_alias) {

			/*
				Allocate space for the dereferenced base object name.
																		*/
			if ((list_par_blk->d21_4dist_name =
			    D27_MALLOC(svc_c_sev_warning,D2_DNL_MAX)) == NULL) {

				list_error->d2_retcode = D2_TEMP_ERR;
				list_error->d2_errclass = D2_SYS_ERR;
				list_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			if (d21_cdsname2x500name ((u_char *)&cds_alias,
				list_par_blk->d21_4dist_name) == D2_ERROR) {
				DUA_TRACE_CDS_TO_X500_NAME_FAILED
				list_error->d2_retcode = D2_CALL_ERR;
                list_error->d2_errclass = D2_PAR_ERR;
                list_error->d2_errvalue = D2_NOT_SPEC;
                return (D2_ERROR);
			}

			/*
				Use the dereferenced base object name from now on.
																	*/
			dns_fname = &cds_alias;
            c_results->d2_al_deref = TRUE;

		} 

		if (!(alias_found && !deref_alias)) {
	
			/*
				Enumerate subordinate objects.
												*/
			if (enum_std_objs (&head, &tail, dns_fname, &dns_flags, list_error)
				 == D2_ERROR) {

				d21_cds_error_info (list_error, dns_fname, 
												&list_par_blk->d21_4err);
				free_list_result (head);
				list_head = (D2_l_info *) NULL;
				r_code = D2_ERROR;

			} else {

				/*
					Enumerate subordinate aliases 
													*/
				list_head = head;
				list_tail = tail;

				if (enum_alias_objs (&head, &tail, dns_fname, &dns_flags, 
					list_error) == D2_ERROR) {

					d21_cds_error_info (list_error, dns_fname, 
												&list_par_blk->d21_4err);
					free_list_result (head);
					free_list_result (list_head);
					list_head = (D2_l_info *) NULL;
					r_code = D2_ERROR;

				} else {
				 
					if (list_head == (D2_l_info *) NULL) 
						list_head = head;
					else 
						list_tail->d2_l_next = head;
				
					if (tail != (D2_l_info *) NULL) 
						list_tail = tail;

					if (enum_child_objs (&head, &tail, dns_fname, &dns_flags, 
							list_error) == D2_ERROR) {
					
						d21_cds_error_info (list_error, dns_fname, 
												&list_par_blk->d21_4err);
						free_list_result (head);
						free_list_result (list_head);
						list_head = (D2_l_info *) NULL;
						r_code = D2_ERROR;

					} else {

						if (list_head == (D2_l_info *) NULL)
							list_head = head;
						else 
							list_tail->d2_l_next = head;
					}
				}
			}
		}
	}

	if (r_code != D2_ERROR) {
	
		r_code = make_result_contiguous (&list_head, list_error);
	}

	list_par_blk->d21_4res = list_head;

	DUA_TRACE_IAPL_EXIT(list_par_blk->d21_4pbhead->d2_errvalue)
    return (r_code);
}



/*-----------------------------------------------------------------------

	enum_std_objs():

	Enumerates the standard subordinate objects of a given object.

  ------------------------------------------------------------------------*/

static int		enum_std_objs (	
					D2_l_info			**list_head,	
					D2_l_info			**list_tail,
					dns_full_name_t		*cds_name,
					struct dnsFlagStat	*dns_flags,
					D2_pbhead			*list_error)
{
	dns_simple_name_t	*dns_class,	*wildcard;
	dns_handle			handle;
	D2_l_info			*obj_list;
	u_char				*gds_rdn,	cds_rdn[D2_DNL_MAX];
	char				*list_space;
	int		get_class,	r_code,	list_size,	len;
	const char function_name[] = "enum_std_objs";

	r_code = D2_NOERROR;

	*list_head = (D2_l_info *) NULL;
	*list_tail = (D2_l_info *) NULL;
	obj_list = (D2_l_info *) NULL;

	if ((handle = cdsGetHandle()) == (dns_handle) NULL) {
		DUA_TRACE_CDS_FUNCTION_FAILED("cdsGetHandle")
		list_error->d2_retcode = D2_CALL_ERR;
		list_error->d2_errclass = D2_PAR_ERR;
		list_error->d2_errvalue = D2_NO_BIND;
		r_code = D2_ERROR;

	} else {

		/*
	    	Enumerate all subordinate objects of the given object. 
			Objects of all object classes are requested.
																	*/
		wildcard = (dns_simple_name_t *) "*";
		dns_class = (dns_simple_name_t *) "*";
		get_class = FALSE;
		len = D2_DNL_MAX;

		if ((r_code = cdsEnumObj (cds_name, handle, dns_class, wildcard,
		    &get_class, (dns_simple_name_t **) NULL, cds_rdn, &len,
			(dns_simple_name_t **)NULL, (u_char *)NULL, (int *)NULL, dns_flags))
			== DNS_SUCCESS) {

			/*	
				malloc space for the list element 
													*/
			list_size = sizeof (D2_l_info);
			if ((list_space = D27_MALLOC(svc_c_sev_warning, list_size + D2_DNL_MAX)) == NULL) {

				list_error->d2_retcode = D2_TEMP_ERR;
				list_error->d2_errclass = D2_SYS_ERR;
				list_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			obj_list = (D2_l_info *) list_space;
			obj_list->d2_l_next = (D2_l_info *) NULL;

			list_space += list_size;
			gds_rdn = (D2_name_string) list_space;

			*list_head = obj_list;
			*list_tail = obj_list;
		
			/*	
				Prepend the typeless OID to the RDN. 
													*/
			strcpy ((char *)gds_rdn, DSW_TYPELESS_OID);
			strcat ((char *)gds_rdn, "\003");
			strcat ((char *)gds_rdn, (char *)cds_rdn);
			obj_list->d2_l_rdn = gds_rdn;
			obj_list->d2_l_alias = FALSE;


			if (dns_flags->fsFromCache) 
				obj_list->d2_l_fromentry = FALSE;
			else 
				obj_list->d2_l_fromentry = TRUE;

			len = D2_DNL_MAX;
			r_code = cdsEnumObj (cds_name, handle, dns_class, wildcard,
				&get_class, (dns_simple_name_t **) NULL, cds_rdn, &len,
				(dns_simple_name_t **)NULL, (u_char *)NULL, (int *)NULL,
				dns_flags);
		} 

		while (r_code == DNS_SUCCESS) {

			/*
				Malloc space for the list element.
													*/
			if ((list_space = D27_MALLOC(svc_c_sev_warning, list_size + D2_DNL_MAX)) == NULL) {

				list_error->d2_retcode = D2_TEMP_ERR;
				list_error->d2_errclass = D2_SYS_ERR;
				list_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			obj_list->d2_l_next = (D2_l_info *) list_space;
			obj_list = obj_list->d2_l_next;
			obj_list->d2_l_next = (D2_l_info *) NULL;

			list_space += list_size;
			gds_rdn = (D2_name_string) list_space;

			strcpy ((char *)gds_rdn, DSW_TYPELESS_OID);
			strcat ((char *)gds_rdn, "\003");
			strcat ((char *)gds_rdn, (char *)cds_rdn);
			obj_list->d2_l_rdn = gds_rdn;
			obj_list->d2_l_alias = FALSE;

			if (dns_flags->fsFromCache) 
				obj_list->d2_l_fromentry = FALSE;
			else  
				obj_list->d2_l_fromentry = TRUE;

			len = D2_DNL_MAX;
			r_code = cdsEnumObj (cds_name, handle, dns_class, wildcard,
				&get_class, (dns_simple_name_t **) NULL, cds_rdn, &len, 
				(dns_simple_name_t **)NULL, (u_char *)NULL, (int *)NULL, 
				dns_flags);
		}

 		if ((r_code == DNS_ERROR) || (r_code == DNS_NOROOM)) {

			d21_cds_map_error (dns_flags->fsLocStat, list_error);
			r_code = D2_ERROR;

		}

		*list_tail = obj_list;
		(void) cdsFreeHandle (handle);
	}

	return (r_code);
}



/*-----------------------------------------------------------------------

	enum_alias_objs():

	Enumerates the alias subordinate objects of a given object.

 -----------------------------------------------------------------------*/

static int		enum_alias_objs (	
					D2_l_info			**list_head,
					D2_l_info			**list_tail,
					dns_full_name_t		*cds_name,
					struct dnsFlagStat	*dns_flags,
					D2_pbhead			*list_error)
{
    dns_simple_name_t	cds_filter;
	dns_handle			handle;
	D2_l_info			*obj_list;
	u_char              *gds_rdn,   cds_rdn[D2_DNL_MAX];
	char				*list_space;
	int		r_code,		list_size,	len;
	const char function_name[] = "enum_alias_objs";

	r_code = D2_NOERROR;

	*list_head = (D2_l_info *) NULL;
	*list_tail = (D2_l_info *) NULL;
	obj_list = (D2_l_info *) NULL;

	if ((handle = cdsGetHandle()) == (dns_handle) NULL) {
		DUA_TRACE_CDS_FUNCTION_FAILED("cdsGetHandle")
		list_error->d2_retcode = D2_CALL_ERR;
		list_error->d2_errclass = D2_PAR_ERR;
		list_error->d2_errvalue = D2_NO_BIND;
		r_code = D2_ERROR;

	} else {

		/*
	    	Enumerate all subordinate alias objects. 
													*/
		len = sizeof (cds_rdn);
		strcpy ((char *)cds_filter.uc, "*");
		if ((r_code = cdsEnumSLink (cds_name, handle, &cds_filter,
		    (dns_simple_name_t **)NULL, cds_rdn, &len, dns_flags)) 
			== DNS_SUCCESS) {

			list_size =  sizeof (D2_l_info);
            if ((list_space = D27_MALLOC(svc_c_sev_warning,list_size + D2_DNL_MAX)) == NULL) {

				list_error->d2_retcode = D2_TEMP_ERR;
				list_error->d2_errclass = D2_SYS_ERR;
				list_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			obj_list = (D2_l_info *) list_space;
			obj_list->d2_l_next = (D2_l_info *) NULL;

			*list_head = obj_list;
			*list_tail = obj_list;

			list_space += list_size;
			gds_rdn = (D2_name_string) list_space;

			strcpy ((char *)gds_rdn, DSW_TYPELESS_OID);
	                strcat ((char *)gds_rdn, "\003");
			strcat ((char *)gds_rdn, (char *)cds_rdn);

			obj_list->d2_l_rdn = gds_rdn;
			obj_list->d2_l_alias = TRUE;

            if (dns_flags->fsFromCache) 
                obj_list->d2_l_fromentry = FALSE;
			else 
        		obj_list->d2_l_fromentry = TRUE;

			len = sizeof (cds_rdn);
			r_code = cdsEnumSLink (cds_name, handle, (dns_simple_name_t *)NULL,
			    (dns_simple_name_t **)NULL, cds_rdn, &len, dns_flags);

		} else {

			if (r_code == DNS_EMPTY) {

				*list_head = (D2_l_info *) NULL;
			}
		}

		while (r_code == DNS_SUCCESS) {

			/*
				Malloc space for the list element.
													*/
            if ((list_space = D27_MALLOC(svc_c_sev_warning,list_size + D2_DNL_MAX)) == NULL) {

				list_error->d2_retcode = D2_TEMP_ERR;
				list_error->d2_errclass = D2_SYS_ERR;
				list_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			obj_list->d2_l_next = (D2_l_info *) list_space;
            obj_list = obj_list->d2_l_next;
            obj_list->d2_l_next = (D2_l_info *) NULL;
             
			list_space += list_size;
            gds_rdn = (D2_name_string) list_space;
            
			strcpy ((char *)gds_rdn, DSW_TYPELESS_OID);
	                strcat ((char *)gds_rdn, "\003");
			strcat ((char *)gds_rdn, (char *)cds_rdn);

			obj_list->d2_l_rdn = gds_rdn;
			obj_list->d2_l_alias = TRUE;

            if (dns_flags->fsFromCache) 
                obj_list->d2_l_fromentry = FALSE;
			else 
                obj_list->d2_l_fromentry = TRUE;

			len = sizeof (cds_rdn);
			r_code = cdsEnumSLink (cds_name, handle, (dns_simple_name_t *) NULL,
			    (dns_simple_name_t **)NULL, cds_rdn, &len, dns_flags);

		}



		if ((r_code == DNS_ERROR) || (r_code == DNS_NOROOM)) {

			list_error->d2_retcode = D2_CALL_ERR;
			list_error->d2_errclass = D2_PAR_ERR;
			list_error->d2_errvalue = D2_NO_BIND;
			r_code = D2_ERROR;
		}

		*list_tail = obj_list;
		(void) cdsFreeHandle (handle);

	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	enum_child_objs():

	Enumerates the child subordinate objects of a given object.

 -----------------------------------------------------------------------*/

static int		enum_child_objs (	
					D2_l_info			**list_head,
					D2_l_info			**list_tail,
					dns_opq_fname		*cds_name,
					struct dnsFlagStat	*dns_flags,
					D2_pbhead			*list_error)
{
	dns_simple_name_t   *wildcard;
	dns_handle			handle;
	D2_l_info			*obj_list;
	u_char              *gds_rdn,   cds_rdn[D2_DNL_MAX];
	char				*list_space;
	int		r_code,		list_size,	len;
	const char function_name[] = "enum_child_objs";

	r_code = D2_NOERROR;

	*list_head = (D2_l_info *) NULL;
	*list_tail = (D2_l_info *) NULL;
	obj_list = (D2_l_info *) NULL;

	if ((handle = cdsGetHandle()) == (dns_handle) NULL) {
		DUA_TRACE_CDS_FUNCTION_FAILED("cdsGetHandle")
		list_error->d2_retcode = D2_CALL_ERR;
		list_error->d2_errclass = D2_PAR_ERR;
		list_error->d2_errvalue = D2_NO_BIND;
		r_code = D2_ERROR;

	} else {

		/*
	    	Enumerate all subordinate child objects. 
													*/
		wildcard = (dns_simple_name_t *) "*";
		len = D2_DNL_MAX;

		if ((r_code = cdsEnumChild (cds_name, handle, wildcard, (void *) NULL,
			cds_rdn, &len, dns_flags)) == DNS_SUCCESS) {

			/*
				malloc space for the list element.
													*/
			list_size = sizeof (D2_l_info);
			if ((list_space = D27_MALLOC(svc_c_sev_warning,list_size + D2_DNL_MAX)) == NULL) {

				list_error->d2_retcode = D2_TEMP_ERR;
				list_error->d2_errclass = D2_SYS_ERR;
				list_error->d2_errvalue = D21_ALLOC_ERROR;
				return (D2_ERROR);
			}

			obj_list = (D2_l_info *) list_space;
			obj_list->d2_l_next = (D2_l_info *) NULL;

			list_space += list_size;
			gds_rdn = (D2_name_string) list_space;

			strcpy ((char *)gds_rdn, DSW_TYPELESS_OID);
	                strcat ((char *)gds_rdn, "\003");
			strcat ((char *)gds_rdn, (char *)cds_rdn);

            obj_list->d2_l_rdn = gds_rdn;
			obj_list->d2_l_alias = FALSE;

            if (dns_flags->fsFromCache) 
                obj_list->d2_l_fromentry = FALSE;
			else  
                obj_list->d2_l_fromentry = TRUE;

			*list_head = obj_list;
			*list_tail = obj_list;

			len = D2_DNL_MAX;
			r_code = cdsEnumChild (cds_name, handle, wildcard, (void *) NULL, 
					cds_rdn, &len, dns_flags);

		} else {

			if (r_code == DNS_EMPTY) {

				*list_head = (D2_l_info *) NULL;
			}
		}

		while (r_code == DNS_SUCCESS) {

			/*
                Malloc space for the list element.
                                                    */
            if ((list_space = D27_MALLOC(svc_c_sev_warning,list_size + D2_DNL_MAX)) == NULL) {
             
				list_error->d2_retcode = D2_TEMP_ERR;
                list_error->d2_errclass = D2_SYS_ERR;
                list_error->d2_errvalue = D21_ALLOC_ERROR;
                return (D2_ERROR);
            }

			obj_list->d2_l_next = (D2_l_info *) list_space;
            obj_list = obj_list->d2_l_next;
            obj_list->d2_l_next = (D2_l_info *) NULL;
            
			list_space += list_size;
            gds_rdn = (D2_name_string) list_space;
              
			strcpy ((char *)gds_rdn, DSW_TYPELESS_OID);
	                strcat ((char *)gds_rdn, "\003");
			strcat ((char *)gds_rdn, (char *)cds_rdn);

            obj_list->d2_l_rdn = gds_rdn;
			obj_list->d2_l_alias = FALSE;

            if (dns_flags->fsFromCache) 
                obj_list->d2_l_fromentry = FALSE;
			else  
                obj_list->d2_l_fromentry = TRUE;

			len = D2_DNL_MAX;
			r_code = cdsEnumChild (cds_name, handle, wildcard, (void *) NULL, 
					cds_rdn, &len, dns_flags);
		}

		if ((r_code == DNS_ERROR) || (r_code == DNS_NOROOM)) {

			list_error->d2_retcode = D2_CALL_ERR;
			list_error->d2_errclass = D2_PAR_ERR;
			list_error->d2_errvalue = D2_NO_BIND;
			r_code = D2_ERROR;

		} if ((r_code == DNS_SUCCESS) || (r_code == DNS_EMPTY)) {

			r_code = D2_NOERROR;
		}

		*list_tail = obj_list;
		(void) cdsFreeHandle (handle);

	}

	return (r_code);
}


/*-----------------------------------------------------------------------

	free_list_result():

	Frees the list result linked list.

 -----------------------------------------------------------------------*/

static void		free_list_result (	
					D2_l_info	*list_result)
{
	if (list_result != (D2_l_info *) NULL) {

		free_list_result (list_result->d2_l_next);
		free ((char *) list_result);
	}
		
}



/*-----------------------------------------------------------------------

	size_list_result():

	Determines the space used by the list result.

 -----------------------------------------------------------------------*/

static int		size_list_result (	
					D2_l_info	*list_result)
{
	int		list_size,	result_size;

	result_size = 0;
	list_size = sizeof (D2_l_info);

	while (list_result != (D2_l_info *) NULL) {

		result_size += list_size + D2_ALIGN(strlen ((char *)list_result->d2_l_rdn) + 1);
		list_result = list_result->d2_l_next;
	}
		
	return (result_size);
}



/*-----------------------------------------------------------------------

	make_result_contiguous():

	Copies the list result to one piece of contiguous space.

 -----------------------------------------------------------------------*/

static int		make_result_contiguous (	
					D2_l_info	**list_result,
					D2_pbhead	*list_error)
{
	D2_l_info	*obj_list,	*contig_list,	*prev_list;
	char		*list_space;
	int			r_code,		result_size,	struct_size,	rdn_size;

	r_code = D2_NOERROR;

	struct_size = sizeof (D2_l_info);
	obj_list = *list_result;
	result_size = size_list_result (obj_list);

	if (obj_list != (D2_l_info *) NULL) {

		if ((list_space = D27_MALLOC(svc_c_sev_warning,result_size)) == NULL) {

			list_error->d2_retcode = D2_TEMP_ERR;
			list_error->d2_errclass = D2_SYS_ERR;
			list_error->d2_errvalue = D21_ALLOC_ERROR;
			return (D2_ERROR);
		}

		*list_result = (D2_l_info *) list_space;
		contig_list = (D2_l_info *) list_space;

		list_space += struct_size;
		contig_list->d2_l_rdn = (D2_name_string) list_space;
		strcpy ((char *)contig_list->d2_l_rdn,
			(char *)obj_list->d2_l_rdn);

		contig_list->d2_l_alias = obj_list->d2_l_alias;
		contig_list->d2_l_fromentry = obj_list->d2_l_fromentry;
		contig_list->d2_l_next = (D2_l_info *) NULL;

		while (obj_list->d2_l_next != (D2_l_info *) NULL) {

			rdn_size = strlen ((char *)obj_list->d2_l_rdn) + 1;
			list_space += D2_ALIGN(rdn_size);
			contig_list->d2_l_next = (D2_l_info *) list_space;

			prev_list = obj_list;
			contig_list = contig_list->d2_l_next;
			obj_list = obj_list->d2_l_next;

			list_space += struct_size;
			contig_list->d2_l_rdn = (D2_name_string) list_space;
			strcpy ((char *)contig_list->d2_l_rdn,
				(char *)obj_list->d2_l_rdn);

			contig_list->d2_l_alias = obj_list->d2_l_alias;
			contig_list->d2_l_fromentry = obj_list->d2_l_fromentry;
			contig_list->d2_l_next = (D2_l_info *) NULL;

			free ((char*) prev_list);
		}

		free ((char *) obj_list);
	}

	return (r_code);
}
