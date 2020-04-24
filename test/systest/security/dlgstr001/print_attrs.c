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
 * $Log: print_attrs.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:35  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:45  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:44:39  agd
 * 	expand copyright
 * 	[1994/10/06  14:32:18  agd]
 * 
 * Revision 1.1.2.1  1994/09/19  18:18:55  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:51:22  bhaim]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <dce/binding.h>
#include <dce/sec_rgy_attr_sch.h>
#include "common.h"

#include <dce/sec_attr_tools.h>

void attr_setup
(
	unsigned32 num_types,
	sec_attr_t attrs[],
        error_status_t *status
)
{

    if (num_types >= 5) {
        attrs[0].attr_id = pre_auth_req_id;
        attrs[1].attr_id = pwd_val_type_id;
        attrs[2].attr_id = test_integer_id;
        attrs[3].attr_id = test_void_id;
        attrs[4].attr_id = test_printstring_id;

        attrs[0].attr_value.attr_encoding = sec_attr_enc_integer;
        attrs[1].attr_value.attr_encoding = sec_attr_enc_integer;
        attrs[2].attr_value.attr_encoding = sec_attr_enc_integer;
        attrs[3].attr_value.attr_encoding = sec_attr_enc_void;
        attrs[4].attr_value.attr_encoding = sec_attr_enc_printstring;
    }
    else
        *status = TRUE;

}

void print_attrs
(
	sec_attr_t	*attr_p,
	error_status_t	*st_p 
)
{
   int	i,n, j;
   unsigned_char_t 		*uuid_string;
   sec_attr_schema_entry_t	schema;
   int no_name = FALSE;

   sec_rgy_attr_sch_lookup_by_id(sec_rgy_default_handle, 
                               (sec_attr_component_name_t) "/0",
                               &attr_p->attr_id, &schema, st_p);
   if (BAD_STATUS(st_p)) { 
	printf("Unable to look up Schema Name...  ");
        no_name = TRUE;
   }
   switch(SA_ATTR_ENCODING(attr_p)) {
   case sec_attr_enc_void:
        if (!no_name)
            printf("\t\t%s\n", schema.attr_name);
    	break;
   case sec_attr_enc_integer:
        if (no_name)
    	    printf("integer = %d\n", (SA_ATTR_INTEGER(attr_p)));
        else
            printf("\t\t%s = %d\n", schema.attr_name, 
                                  (SA_ATTR_INTEGER(attr_p)));
   	break;
   case sec_attr_enc_printstring:
        if (no_name)
    	    printf("printstring = %s\n", 
    	        	(SA_ATTR_PRINTSTRING_P(attr_p)));
        else
            printf("\t\t%s = %s\n", schema.attr_name,
    	        	(SA_ATTR_PRINTSTRING_P(attr_p)));
    	break;
    default:
    	break;
    } /* end enc_type switch */
}
