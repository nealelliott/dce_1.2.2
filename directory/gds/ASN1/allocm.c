/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: allocm.c,v $
 * Revision 1.2.10.2  1996/02/18  19:37:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:12  marty]
 *
 * Revision 1.2.10.1  1995/12/08  15:05:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:05  root]
 * 
 * Revision 1.2.8.4  1994/08/01  10:13:56  marrek
 * 	Remove true, false and boolean.
 * 	[1994/07/28  16:11:18  marrek]
 * 
 * Revision 1.2.8.3  1994/06/21  14:43:04  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:21:59  marrek]
 * 
 * Revision 1.2.8.2  1994/05/10  15:48:31  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:04  marrek]
 * 
 * Revision 1.2.8.1  1994/02/22  15:20:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:50:39  marrek]
 * 
 * Revision 1.2.6.2  1993/08/11  11:16:26  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:56:50  marrek]
 * 
 * Revision 1.2.4.4  1992/12/31  16:33:41  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:06  bbelch]
 * 
 * Revision 1.2.4.3  1992/12/02  14:10:02  marrek
 * 	November 1992 code drop
 * 	comment around token of #endif
 * 	[1992/12/02  14:08:56  marrek]
 * 
 * Revision 1.2.4.2  1992/11/27  16:55:20  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  14:22:35  marrek]
 * 
 * Revision 1.2.2.3  1992/06/01  20:13:43  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:12:43  zeliff]
 * 
 * Revision 1.2.2.2  1992/04/14  13:32:22  mishkin
 * 	     Don't use non-standard pthread_initroutine_t.
 * 	[1992/04/13  16:00:07  mishkin]
 * 
 * Revision 1.2  1992/01/19  22:12:59  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: allocm.c,v $ $Revision: 1.2.10.2 $ $Date: 1996/02/18 19:37:23 $";
#endif

 
/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

/******************************************************************************
* 						    
* Allocation of memory for the decoding of strings.
* 						  
* Two data structures are used by these routines: asn1_alloc_collide_tab and
* asn1_alloc_hash_tab.
*
* asn1_alloc_collide_tab is an array of structs.  Each struct consists of a
* pointer field to point to a segment of allocated memory and an index to 
* allow the structs to be connected into lists. Each element that is in use 
* is in some list based on the hash value of the memory address stored in the 
* pointer field of the element.  The index of the head of each list is stored 
* in asn1_alloc_hash_tab based on the hash index of the pointers associated 
* with the list.  The free elements of asn1_alloc_collide_tab are also managed
* in a list.  When a memory segment is to be managed in via these tables, an
* element is retrieved from the free list, the pointer is hashed, and the 
* element is inserted into the head of the appropriate hash list (bucket).
* If there is no free list element, the asn1_alloc_collide_tab is expanded.
*
* These routines can be made re-entrant by including the ASN1_THREADED code.
* Modify access to the critical data structures is serialized. 
*
* See the manpages for a description of the routines.
\******************************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <dce/dce.h>
#include <dce/asn1.h>

struct asn1_alloc_tab {
	char * pointer_value;
	int next_index;
};

#define TAB_SIZE	1024
#define TAB_SIZE_MASK	1023

static int * asn1_alloc_hash_tab = 0;
static struct asn1_alloc_tab * asn1_alloc_collide_tab = 0;
static int asn1_alloc_tab_free = -1;
static int asn1_alloc_tab_max = 0;
static int asn1_alloc_hash (unsigned char *);

#ifdef ASN1_THREADED
#include <pthread.h>
ASN1_SEM_DECLARE(asn1_alloc_sem);
static void asn1_alloc_init(void);
#endif


static asn1_alloc_hash(y)
unsigned char * y;
{
    	register i, x=0;
	register unsigned char * z = y;

	for (i=0; i<sizeof(char *); i++){
		x = (x << 4) + x + *z++;
	}
	return(x&(TAB_SIZE_MASK));
}

void asn1_alloc_free(addr,t)
char * addr;
int t;
{	register char * x = addr;
	register i, i_previous, h;

	if (asn1_alloc_hash_tab == 0 || x == 0)
		return;
#ifdef ASN1_THREADED
	ASN1_SEM_LOCK(&asn1_alloc_sem);
#endif
	i_previous = -1,
	i = asn1_alloc_hash_tab[h = asn1_alloc_hash((unsigned char *)&addr)];

	while (i != -1){
	    	if (asn1_alloc_collide_tab[i].pointer_value == x){
			break;
		}
		i_previous = i;
		i = asn1_alloc_collide_tab[i].next_index;
	}
	if (i == -1) {
#ifdef ASN1_THREADED
	        ASN1_SEM_UNLOCK(&asn1_alloc_sem);
#endif
		return;
	}
	if (t) ASN1_FREE_NAME(x);
	if (i_previous == -1
	    && asn1_alloc_collide_tab[i].next_index == -1
	   )
		asn1_alloc_hash_tab[h] = -1;
	else if ( i_previous == -1 )
		asn1_alloc_hash_tab[h] =
		asn1_alloc_collide_tab[i].next_index;
	else	asn1_alloc_collide_tab[i_previous].next_index =
		asn1_alloc_collide_tab[i].next_index;
	asn1_alloc_collide_tab[i].next_index = asn1_alloc_tab_free;
	asn1_alloc_tab_free = i;
#ifdef ASN1_THREADED
	ASN1_SEM_UNLOCK(&asn1_alloc_sem);
#endif
}

char * asn1_alloc_malloc(len, addr)
unsigned len;
char * addr;
{	register i, h;

	if (asn1_alloc_hash_tab == 0){
#ifdef ASN1_THREADED
		asn1_alloc_init();
		ASN1_SEM_LOCK(&asn1_alloc_sem);
/* incase waiting on lock, check to see if another thread already created tab */
		if (asn1_alloc_hash_tab == 0){
#endif
/* create hash_tab and collide_tab and initialize */
		asn1_alloc_hash_tab = (int *)
		  malloc((unsigned)(sizeof(int)*TAB_SIZE));
		asn1_alloc_collide_tab = (struct asn1_alloc_tab *)
		  malloc((unsigned)(sizeof(struct asn1_alloc_tab)*TAB_SIZE));
		asn1_alloc_tab_max = TAB_SIZE;
		asn1_alloc_tab_free = 0;
		for (i=0; i<TAB_SIZE;){
			asn1_alloc_hash_tab[i] = -1;
			asn1_alloc_collide_tab[i].next_index = i+1;
			i++;
		}
		asn1_alloc_collide_tab[TAB_SIZE -1].next_index = -1;
#ifdef ASN1_THREADED
		}
		ASN1_SEM_UNLOCK(&asn1_alloc_sem);
#endif

	}

	if (addr == 0)
		if ((addr = ASN1_MALLOC_NAME(len)) == 0) return(0);
#ifdef ASN1_THREADED
	ASN1_SEM_LOCK(&asn1_alloc_sem);
#endif
/* if free list empty, add another TAB_SIZE block to collide_tab and init */
	if (asn1_alloc_tab_free == -1){
    		asn1_alloc_tab_free = i = asn1_alloc_tab_max;
		asn1_alloc_collide_tab = (struct asn1_alloc_tab *)
		  realloc((char *)asn1_alloc_collide_tab,
		    (unsigned)(sizeof(struct asn1_alloc_tab)*(i + TAB_SIZE)));
		asn1_alloc_tab_max += TAB_SIZE;
		while(i <asn1_alloc_tab_max)
			{
			asn1_alloc_collide_tab[i].next_index = i+1;
			i++;
			}
		asn1_alloc_collide_tab[i -1].next_index = -1;
	}

/* collide_tab[i] gets taken out of the free list  */
	i = asn1_alloc_tab_free;
	asn1_alloc_tab_free = asn1_alloc_collide_tab[i].next_index;
/* collide_tab[i] gets memory area pointed to by addr */
	asn1_alloc_collide_tab[i].pointer_value = addr;
/* new entry gets inserted at head of list rooted at hash_tab[h] */
	asn1_alloc_collide_tab[i].next_index =
	asn1_alloc_hash_tab[h = asn1_alloc_hash((unsigned char *)&addr)];
	asn1_alloc_hash_tab[h] = i;
#ifdef ASN1_THREADED
	ASN1_SEM_UNLOCK(&asn1_alloc_sem);
#endif
	return(addr);
}

char * asn1_realloc(addr,len)
unsigned len;
char * addr;
{	register char * x;

	x = ASN1_REALLOC_NAME(addr,len);
	if (x != addr){
		asn1_alloc_free(addr,0);
		(void)asn1_alloc_malloc(len, x);
	}
	return(x);
}

int asn1_alloc_snap_shot(addr)
char *** addr;
{
	register nb_points, i;

	if (asn1_alloc_hash_tab == 0){
		if (addr){
			*addr = (char**) ASN1_MALLOC_NAME
			((unsigned)sizeof(char *));
		}
		return(0);
	}

#ifdef ASN1_THREADED
	ASN1_SEM_LOCK(&asn1_alloc_sem);
#endif
	nb_points = asn1_alloc_tab_max;
	for (i=asn1_alloc_tab_free; i != -1;
	i = asn1_alloc_collide_tab[i].next_index){
		asn1_alloc_collide_tab[i].pointer_value = 0;
		nb_points--;
	}

	if (addr){
		*addr = (char**) ASN1_MALLOC_NAME
		((unsigned)(sizeof(char *)*(nb_points+1)));
		nb_points = 0;
		for (i=0; i<asn1_alloc_tab_max; i++){
			if (((*addr)[nb_points] = 
			asn1_alloc_collide_tab[i].pointer_value) != 0)
				nb_points++;
		}
	}
#ifdef ASN1_THREADED
	ASN1_SEM_UNLOCK(&asn1_alloc_sem);
#endif
	return(nb_points);
}




#ifdef ASN1_THREADED

/*
**  ABSTRACT:
**
**  ASN1 Initialization module for re-entrant memory allocation routines.
**  Creates mutex used to protect critical data structures.
**
**
*/

static void asn1_alloc_init_once ( void );

/*
 * The structure that defines the one-time initialization code. This
 * structure will be associated with alloc_init_once.
 */
static pthread_once_t asn1_alloc_init_once_block = pthread_once_init;

/*
 * The value that indicates whether or not the asn1 memory allocation routines
 * are currently being initialized.
 */
static boolean        asn1_alloc_init_in_progress = FALSE;

/*
 * The id of the thread that is executing (has executed) the asn1 alloc
 * initialization code.
 */
pthread_t      asn1_g_alloc_init_thread;

/*
**  ROUTINE NAME:       asn1_alloc_init
**
**  SCOPE:              static - declared above
**
**  DESCRIPTION:
**      
**  asn1_alloc_init() is called to create and intialize the mutex.
**  Prevent asn1_alloc_init() (actually pthread_once() of 
**  asn1_alloc_init_once()) recursive call deadlocks by the thread that is 
**  already performing the initialization (see asn1_alloc_init_once()).
**
**/

static void asn1_alloc_init(void)
{
    pthread_t       current_thread;
    

    if (asn1_alloc_init_in_progress)
    {
        current_thread = pthread_self();
        
        if (pthread_equal(asn1_g_alloc_init_thread, current_thread))
        {
            /*
             * We're the thread in the middle of initialization 
             * (asn1_alloc_init_once).
             * Assume it knows what it's doing and prevent a deadlock.
             */
            return;
        }
    }

    pthread_once (&asn1_alloc_init_once_block, asn1_alloc_init_once);
}


/*
**
**  ROUTINE NAME:       asn1_alloc_init_once
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  asn1_alloc_init_once() is invoked only once. It creates the mutex
**  used to serialize access to memory allocation tables. 
**
**/

static void asn1_alloc_init_once(void)

{

    /*
     * These two operations (and their order) are critical to creating
     * a deadlock-safe environment for init processing.
     */
    asn1_g_alloc_init_thread = pthread_self();
    asn1_alloc_init_in_progress = TRUE;

    ASN1_SEM_CREATE(&asn1_alloc_sem, pthread_mutexattr_default);

    asn1_alloc_init_in_progress = FALSE;
}
#endif /* ASN1_THREADED */
