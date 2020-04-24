/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_list.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:50  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:18  root]
 * 
 * Revision 1.1.4.3  1994/06/23  18:30:36  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:18:39  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:53  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:33  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:17:01  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:32:10  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _GDA_LIST_H
#define _GDA_LIST_H
/*
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
 */
/* list.h: contains all of the definitions necessary to use the list module. */

#ifndef OK
#define	OK			0
#endif 

/* The l_init function failed to initialize the list. */
#define	L_INIT_FAIL		-2

/* The l_free function failed to free the list. */
#define	L_FREE_FAIL		-3

/* The l_insert function failed to insert the element. */
#define	L_INSERT_FAIL		-5

/* The l_insert function encountered a malloc problem. */
#define	L_INSERT_MALLOC		-6

/* The l_insert function failed because a list item, other than the one
	being inserted, was deleted from the list during the insertion.  If
	this error code is returned, all locks on the list have been released,
	so the calling routine must call l_first again. */
#define	L_INSERT_RETRY		-8

/* The l_delete function failed to delete the list item. */
#define	L_DELETE_FAIL		-9

/* The l_next function failed to produce the next element.  Either an internal
	error occurred or no next item is present. */
#define	L_NEXT_FAIL		-11

/* The l_first function failed to produce the first item on the list.  Either
	an internal error occurred or no items exist on the list. */
#define	L_FIRST_FAIL		-13

/* The l_first function failed therby producing a lock inconsistency. */
#define	L_FIRST_L_ERR		-14

/* The l_release_link function failed to release the link
	locks on the element. */
#define	L_RELEASE_LINK_FAIL	-15

/* The l_release_el function failed to release the link locks or acquire
	the data locks of the element. */
#define	L_RELEASE_EL_FAIL	-17

/* The l_release_data function failed to release the data locks of
	the element */
#define	L_RELEASE_DATA_FAIL	-19

/* The l_free_nl function failed to free the list. */
#define	L_FREE_NL_FAIL		-21


/* LIST_REF is used to reference the data belonging to the caller that is
	stored inside a list element. */
#define	LIST_REF(X,Y)	((X *)((element_t *)Y)->data_el)


/* element_t is a generic element type used by the list module to store data
	inside the list.  The format of element_t is meant to be hidden from
	the user of the list module. */

typedef struct element_t {
	char			*data_el;

		/* data_el points to the data that the user of the module
			wants to store in the list.  Its structure is
			unknown to the list module. */

	rwl_t			rwl_link;

		/* rwl_link is a set of readers writers locks meant to
			protect the links between elements in the list.  If
			this lock is held on element X by thread Y, neither
			the next_el or prev_el variable will be changed by
			any other thread than Y. */

	rwl_t			rwl_data;

		/* rwl_link is a set of readers writers locks meant to
			protect the data stored in the list.  If
			this lock is held on element X by thread Y, neither
			no data pointed to by data_el, or data_el itself
			 will be changed by any other thread than Y. */

	struct element_t	*next_el;

		/* next_el points to the next item in the list. */

	struct element_t	*prev_el;

		/* prev_el points to the previous item in the list. */
} element_t;

/* list_t is a generic list type used by the list module as a header for
	the elements stored in a list.  Its structure is meant to be hidden
	from the user of the list module.  See list.c for a discussion of
	list locking order. */

typedef struct list_t {
	element_t	*head;
		/* head points to the first element of the list */

	rwl_t		rwl_link;
		/* rwl_link is a set of readers writers locks meant to
			protect the links between elements in the list.  If
			this lock is held on list X by thread Y, the
			head variable will not be changed by any thread other 
			than Y. */
} list_t;

/**
 ** FUNCTION PROTOTYPES.... 
 **/

void		
l_init (
    list_t       *);

int		
l_free (
    list_t       *);

int		
l_insert (
    char        **,
    list_t       *,
    element_t    *);

int		
l_delete (
    element_t   **,
    list_t       *);

element_t *
l_next (
    element_t    *);

element_t *
l_first (
    list_t       *);

int		
l_release_link (
    element_t    *);

int		
l_release_el (
    element_t    *);

int		
l_release_data (
    element_t    *);

void		
l_init_nl (
    list_t       *);

int		
l_free_nl (
    list_t       *);

void		
l_insert_nl (
    char        **,
    list_t       *,
    element_t    *);

void		
l_delete_nl (
    element_t   **,
    list_t       *);

element_t *
l_next_nl (
    element_t    *);

element_t *
l_first_nl (
    list_t       *);

#endif  /* #ifndef _GDA_LIST_H */




