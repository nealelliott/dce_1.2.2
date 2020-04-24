/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: print_attrs.c,v $
 * Revision 1.1.4.2  1996/02/17  23:11:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:53:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:12  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:37:19  agd
 * 	expand copyright
 * 	[1994/10/06  14:30:19  agd]
 * 
 * Revision 1.1.2.1  1994/08/04  16:15:59  mdf
 * 	Initial checkin
 * 	[1994/08/03  19:19:46  mdf]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <dce/rpc.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_attr_tools.h>

#define PROTECT_LEVEL_MAX    7
#define AUTHN_MAX    4
#define AUTHZ_MAX    3

static char *protect_level_name[] = { "default",
				      "none",
				      "connect",
				      "call",
				      "pkt",
				      "pkt_integ",
				      "pkt_privacy" };
static unsigned32 protect_level_val[] = { rpc_c_protect_level_default,
					  rpc_c_protect_level_none,
					  rpc_c_protect_level_connect,
					  rpc_c_protect_level_call,
					  rpc_c_protect_level_pkt,
					  rpc_c_protect_level_pkt_integ,
					  rpc_c_protect_level_pkt_privacy };

static char *authn_name[] = { "none", "dce_secret", "dce_public", "default" };
static unsigned32 authn_val[] = { rpc_c_authn_none, rpc_c_authn_dce_secret,
				rpc_c_authn_dce_public, rpc_c_authn_default };

static char *authz_name[] = { "none", "name", "dce" };
static unsigned32 authz_val[] = { rpc_c_authz_none, rpc_c_authz_name,
				rpc_c_authz_dce };

void print_attrs
(
    sec_attr_t		*attr_p,
    error_status_t	*st_p
)
{
   int	i,n, j;
   unsigned_char_t 		*uuid_string;

   switch(SA_ATTR_ENCODING(attr_p)) {
   case sec_attr_enc_any:
   case sec_attr_enc_void:
    	break;
   case sec_attr_enc_integer:
    	printf("\tinteger = %d\n", (SA_ATTR_INTEGER(attr_p)));
   	break;
   case sec_attr_enc_printstring:
    	printf("\tprintstring = %s\n", 
    		(SA_ATTR_PRINTSTRING_P(attr_p)));
    	break;
    case sec_attr_enc_printstring_array:
    	printf("\tprintstring array: \n");
    	if(SA_ATTR_STR_ARRAY_P(attr_p)) {
    	    for(i = 0; i < SA_ATTR_STR_ARRAY_NUM(attr_p); i++) {
    		    printf("\t\t%s\n", 
    			(SA_ATTR_STR_ARRAY_ELT_P(attr_p, i)));
    	    }
    	}
    	break;
    case sec_attr_enc_bytes:
    	printf("\tbytes:\n");
    	for(i = 0; i < SA_ATTR_BYTES_LEN(attr_p); i++) {
    	    printf("\t\t%x\n", (SA_ATTR_BYTES_DATA(attr_p, i)));	
    	}
    	break;
    case sec_attr_enc_confidential_bytes:
    	printf("\tconfidential bytes:\n");
    	for(i = 0; i < SA_ATTR_BYTES_LEN(attr_p); i++) {
    	    printf("\t\t%x ", (SA_ATTR_BYTES_DATA(attr_p, i)));	
    	}
	printf("\n");
    	break;
    case sec_attr_enc_i18n_data:
    	printf("\ti18n data:\n");
    	if(SA_ATTR_IDATA_P(attr_p)) {
    	    printf("\t\tcodeset = %x\n", 
    			(SA_ATTR_IDATA_CODESET(attr_p)));
    	    for(i = 0; i < SA_ATTR_IDATA_LEN(attr_p); i++) {
    	        printf("\t\t%x ", (SA_ATTR_IDATA_DATA(attr_p, i)));	
    	    }
	    printf("\n");
    	}
    	break;
    case sec_attr_enc_uuid:
    	uuid_to_string(&SA_ATTR_UUID(attr_p), &uuid_string, st_p);
    	printf("\tuuid = %s\n", uuid_string);
    	break;
    case sec_attr_enc_attr_set:
    	printf("\tattr set:\n");
    	if(SA_ATTR_SET_P(attr_p))  {
    	    for(i = 0; i < SA_ATTR_SET_NUM(attr_p); i++) {
    		uuid_to_string(&SA_ATTR_SET_MEMBERS(attr_p, i), &uuid_string, 
			st_p);
    		printf("\t\t%s\n", uuid_string);
    	    }
	}
    	break;
    case sec_attr_enc_binding:
    	printf("\tbinding:\n");
	printf("\t\tauth info: ");
    	if(SA_ATTR_BND_INFO_P(attr_p)) {
    	    if(SA_ATTR_BND_AUTH_INFO_TYPE(attr_p) == sec_attr_bind_auth_dce) {
		printf("sec_attr_bind_auth_dce\n");
    		printf("\t\t\t svr_pname: %s\n",
    			(SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p)));
    	        for ( i=0; i<PROTECT_LEVEL_MAX; i++ ) 
        	    if ( SA_ATTR_BND_AUTH_PROT_LEVEL(attr_p) == protect_level_val[i] ) {
             	        printf("\t\t\t rpc_c_protect_level_%s (%d)\n", 
				protect_level_name[i], protect_level_val[i]);
             	        break;
        	    }
    	        for ( i=0; i<AUTHN_MAX; i++ ) 
        	    if ( SA_ATTR_BND_AUTH_AUTHN_SVC(attr_p) == authn_val[i] ) {
             	        printf("\t\t\t rpc_c_authn_%s (%d)\n", authn_name[i],
				authn_val[i]);
             	        break;
        	    }
    	        for ( i=0; i<AUTHZ_MAX; i++ ) 
        	    if ( SA_ATTR_BND_AUTH_AUTHZ_SVC(attr_p) == authz_val[i] ) {
             	        printf("\t\t\t rpc_c_authz_%s (%d)\n", authz_name[i],
				authz_val[i]);
             	        break;
        	    }
	    }
	    else {
		printf("sec_attr_bind_auth_none\n");
	    }
    	    for(i = 0; i < SA_ATTR_BND_NUM(attr_p); i++) {
    	        switch(SA_ATTR_BND_TYPE(attr_p, i)) {
    	        case sec_attr_bind_type_string:
		    if (SA_ATTR_BND_STRING_P(attr_p, i)) {
	    	        printf("\t\t%s\n", (SA_ATTR_BND_STRING_P(attr_p, i)));
		    }
    	            break;
    	        case sec_attr_bind_type_twrs:
    	            for(j=0; j < SA_ATTR_BND_TWRSET_COUNT(attr_p, i); j++) {
			for (n=0; n < SA_ATTR_BND_TWR_LEN(attr_p, i, j); n++) {
    	                    printf("\t\t%x ", 
				SA_ATTR_BND_TWR_OCTETS(attr_p, i, j)[n]);
			}
			printf("\n");
    	            }
    	            break;
    	        case sec_attr_bind_type_svrname:
		    if (SA_ATTR_BND_SVRNAME_P(attr_p, i)->name) {
			printf("\t\t svrname %s\n",
    				(SA_ATTR_BND_SVRNAME_P(attr_p,i))->name);
		    }
    		    break;
    	        } /* end bind_type switch */
    	    }
    	} /* end if bind_info present */
    	break;
    default:
    	break;
    } /* end enc_type switch */
}
