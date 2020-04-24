/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: pkc_plcy_hierarchy.h,v $
 * Revision 1.1.2.1  1996/11/13  20:38:52  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:55:58  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:40 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:31 UTC  arvind  /main/arvind_capi/1]
 * 
 * $EndLog$
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 * 
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 * 
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 * 
 * FACILITY:    Certification API	
 * 
 * ABSTRACT:	Hierarchichal Policy Specific definitons.
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              Mahesh B. Girkar 
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */






#ifndef PKC_PLCY_HIERARCHY_
#define PKC_PLCY_HIERARCHY_

#ifndef __cplusplus
#error "C++ must be used for policy module registration"
#endif


extern "C" {
#include <stdlib.h>
#include <dce/dce.h>
#include <dce/dcepkcmsg.h>
#include <dce/gssapi.h>
#include <dce/sec_pk_base.h>
}


#include <dce/pkc_common.h>
#include <dce/pkc_certs.h>
#include <dce/x500name.h>

/* hierarchical policies trustbase structure; 
 * Used to return an expanded pkc_trust_list_t & cross-certs looked
 * up during establishing trust 
 */

typedef struct hierarchy_trustbase{
   pkc_trust_list_t *tr;
   struct ca_db_entry *crdb;
} hierarchy_trusbaset_t;

typedef struct hierarchy_keyinfo {
  pkc_trust_list_t *tr;
  pkc_trusted_key_list_t *keylist;
  size_t no_of_keys;
} hierarchy_keyinfo_t;



typedef struct {
void *cert;
int length;
} cert_struct;


class x500name_list_element_t {
  x500name name;
public:
  x500name_list_element_t *next;
  x500name_list_element_t *prev;
  x500name_list_element_t (x500name &t_name) {
    name = t_name;
    next = NULL;
    prev = NULL;
  };

  ~x500name_list_element_t (void) {
    return;
  };

  void get_name(x500name & ret_name) {
    ret_name = name;
  };
};

class x500name_list_t {
  x500name_list_element_t *current; /* used as a reference ptr to traverse
                                     * list 
                                     */
public:
  x500name_list_element_t *last;
  x500name_list_element_t *first;

  x500name_list_t (void) {
    current = NULL;
    first = NULL;
    last = NULL;
  };

  ~x500name_list_t() {
    x500name_list_element_t *temp;
    while (first != NULL) {
      temp = first->next;
      delete first;
      first = temp;
    };
  };

  // initialize list traversal for outside members
  void init_enumeration(void) { 
    current = first;
    return;
  };

  // search_name: 1 ==> found, 0 ==> notfound
  unsigned32 search_name(x500name & lookup) {  
    x500name_list_element_t *temp = first;						    
    x500name ret_name;
    char bool;

    // list empty ==> lookup not present in list
    if (!first)  
      return 0;

    while (temp) {
      temp->get_name(ret_name);

      compare_names(lookup, ret_name, &bool, 0, 0);
      if (bool)  // found
        return 1;
      else
        return 0;
      temp = temp->next;
    };
    return 0;
  };
      

  // will call search_name to ensure that ca name is not added twice
  unsigned32 add_name (x500name & name_to_add) {
    x500name_list_element_t *temp;

    // already present, return success
    if (search_name(name_to_add)) 
      return 0;


    temp = new x500name_list_element_t(name_to_add);

    if (first == NULL) {  /* empty list */
      first = temp;
      last = temp;
      temp->next = NULL;  /* this is a repeat, since the constructor for
                             x500name_list_element_t will set it to NULL */
      temp->prev = NULL;  /* this is a repeat for the same reason */
    } 
    else {

      last->next = temp;
      temp->next = NULL;  /* this is a repeat as well */
      temp->prev = last;
    }
    return 0;
  };

  unsigned32   get_next(x500name & ret_name) {
    x500name temp;
    if (current == NULL) {  /* reached end of list */
      // reached end  of list enumeration 
      return 0;   
    }
    
    current->get_name(ret_name);
    current = current->next;

    // probably more elements in the list
    return 1;  
  };


  unsigned32 delete_element(x500name &t_name) {
    x500name_list_element_t *temp = first;
    char names_equal;
    x500name ret_name;

    while (temp != NULL) {
      temp->get_name(ret_name);
      compare_names(t_name, ret_name,
			&names_equal, 0, 0);
      if (names_equal) { 
	// found match 
        if (temp == first) {
	  first = first->next;
          if (first) {
	    first->prev = NULL;
          } else {
            last = first;
          }
	  delete temp;
	  return 0;
	}
        if (temp == last) {
          if (last->prev)
	    last->prev->next = NULL;
	  last = last->prev;
          if (!last) {
            first = last;
          }
     
	  delete temp;
	  return 0;
	}
	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	delete temp;
	return 0;
      }
      temp = temp->next;
    }

    // not found
    return -1;  
  };  /* delete_element */

  x500name_list_t & operator=(const x500name_list_t & in) {
    x500name_list_element_t *temp = 0;
    x500name ret_name;

    // the next three assignments are probably not necessary, since the
    // constructor for x500name_list_t variable on the left hand side should
    // do the trick.. But, you never know.
    this->first = NULL;
    this->last = NULL;
    this->current = NULL;
    temp = in.first;
    while (temp) {
      temp->get_name(ret_name);
      this->add_name(ret_name);
      temp = temp->next;
    }
    return *this;
  };
};

extern pkc_policy_t pkc_hierarchy;
#endif
