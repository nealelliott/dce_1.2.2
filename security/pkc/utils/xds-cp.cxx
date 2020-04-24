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
 * $Log: xds-cp.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:42:22  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:04:19  arvind]
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
 * 
 * AUTHOR:
 *              John Wray 
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */


extern "C" {
#include <stdio.h>
#include <stdlib.h>
#ifdef FORCE_THREADS
#include <pthread.h>
#endif
#include <ctype.h>
}

#include <dce/x500name.h>
#include <dce/xdsxx.h>


int list_command(int & done, char * params);
int delete_command(int & done, char * params);
int create_command(int & done, char * params);
int read_command(int & done, char * params);
int quit_command(int & done, char * params);
int add_command(int & done, char * params);
int help_command(int & done, char * params);

xds_t xds;

typedef struct commands_struct {
    char * str;
    int (*f)(int & done, char * params);
} commands_t;

commands_t commands[] = {
    {"exit", quit_command},
    {"quit", quit_command},
    {"q", quit_command},
    {"list", list_command},
    {"read", read_command},
    {"delete", delete_command},
    {"create", create_command},
    {"add", add_command},
    {"help", help_command},
    
    {NULL, NULL}
};

int list_command(int & done, char * params) {
    x500name name;
    xds_status_t * status;
    xds_list_result_t * list_results;
    unsigned32 error;
    buffer_t temp_buffer;
    const x500name * res_name;    
    const RelativeDistinguishedName * res_rdn;    
    const xds_list_info_item_t * result_item;

    error = name.set_value(params);
    if (error) {
	fprintf(stdout, "Error parsing name (0x%x)\n", error);
	fflush(stdout);
	return -1;
    };    

    temp_buffer.empty();
    name.display(temp_buffer);
    fprintf(stdout,
            "\nInvoking LIST on \"%.*s\"\n\n",
	    temp_buffer.data_length,
            temp_buffer.data);    
    fflush(stdout);
    temp_buffer.empty();

    
    status = xds.list(name, list_results);

    fprintf(stdout, "Returned from xds::list()\n");
    fflush(stdout);

    if (status->isError()) {
        fprintf(stderr,
                "list() returned error status, class: %s, code: %s\n",
                xds_error_class_name(status->error_class),
                xds_error_code_name(status->error_code));
        if (status->error_class == XDS_C_NAME_ERROR) {
            if (error = status->get_name(res_name)) {
                fprintf(stderr,
                        "Error %d extracting name from status\n",
                        error);
            } else {
                temp_buffer.empty();
                res_name->display(temp_buffer);
                fprintf(stderr,
                        "Matched: %.*s\n",
                        temp_buffer.data_length,
                        temp_buffer.data);
                temp_buffer.empty();
            };
        };
        fflush(stderr);
        return -1;
    } else {
// We did a successful list.  Display the results...
        if ((error = list_results->get_object_name(res_name)) == 0) {
            temp_buffer.empty();
            res_name->display(temp_buffer);
            fprintf(stdout, "Results of LIST for object %.*s:\n",
                    temp_buffer.data_length, temp_buffer.data);
            temp_buffer.empty();
        } else {
            fprintf(stderr,
                    "Error %d extracting name from top-level result\n",
                    error);
            fflush(stderr);
        };
        if ((error = list_results->get_first_item(result_item)) == 0) {
            while (error == 0) {
                if ((error = result_item->get_rdn(res_rdn)) == 0) {
                    res_rdn->display(temp_buffer);
                    fprintf(stdout, "%.*s\n",
                            temp_buffer.data_length, temp_buffer.data);
                    temp_buffer.empty();
                } else {
                    fprintf(stderr,
                            "Error %d extracting name from result-item\n",
                            error);
                    fflush(stderr);
                };
                error = list_results->get_next_item(result_item);
            };
            if (error != pkc_s_no_more_items) {
                fprintf(stderr,
                        "Error %d fetching result-item\n",
                        error);
                fflush(stderr);
            };
        } else {
            fprintf(stderr,
                    "Error %d fetching first result-item\n",
                    error);
            fflush(stderr);
        };
    };
    fprintf(stdout, "Deleting status return...\n");
    fflush(stdout);
    delete status;
    fprintf(stdout, "Deleting list results...\n");
    fflush(stdout);
    delete list_results;
    return 0;
}

int read_command(int & done, char * params) {
    x500name name;
    xds_status_t * status;
    xds_read_result_t * read_results;
    xds_selection_t selection;
    unsigned32 error;
    buffer_t temp_buffer;
    const x500name * res_name;    
    const RelativeDistinguishedName * res_rdn;    
    const xds_attribute_t * attribute;
    const OM_object_identifier * attr_oid;
    int i;
    int j;
    const xds_value_t * value;
    char *temp = 0, *attribute_p = 0;
    asn_objectidentifier asnobj;
    OM_object_identifier *gsoid = 0;
    OM_uint32 min_stat;

    temp = params; 
    while ((*temp == ' ') && (*temp != '\0'))
          temp++;  /* ignore leading blanks */
    if (*temp == '\0') {  
      printf("No entry specified\n");
      return -1;
    }
    

    params = temp;
    while ((*temp != ' ') && (*temp != '\0')) 
      temp++;

    if (*temp == ' ') {
      *temp = '\0';
      temp++;
    }
  
    if (*temp != '\0') {  /* we had a blank */
      while ((*temp == ' ') && (*temp != '\0')) 
           temp++;  /* ignore leading blanks */
      if (*temp != '\0')  {
         attribute_p = temp; 
         while ((*temp != ' ') && (*temp != '\0')) 
           temp++;
         *temp = '\0';
      }
    }
    
    error = name.set_value(params);
    if (error) {
	fprintf(stdout, "Error parsing name (0x%x)\n", error);
	fflush(stdout);
	return -1;
    };    
    
    if (attribute_p) {
        error = asnobj.set_value((const char *) attribute_p);
        if (error) {
            fprintf(stdout, "Error converting Attribute OID (0x%x)\n", error);
            fflush(stdout);
            return -1;
        };    

        error = asnobj.get_value(gsoid);
        if (error) {
            fprintf(stdout, "Error getting OID (0x%x)\n", error);
            fflush(stdout);
            return -1;
        };    

        selection.select_specified_attributes();
        selection.select_attribute(*gsoid);
        selection.return_types_and_values();


        gssdce_release_oid(&min_stat, &gsoid);
    }

    temp_buffer.empty();
    name.display(temp_buffer);
    fprintf(stdout,
            "\nInvoking READ on \"%.*s\"\n\n",
	    temp_buffer.data_length,
            temp_buffer.data);    
    fflush(stdout);
    temp_buffer.empty();

    status = xds.read(name, selection, read_results);

    fprintf(stdout, "Returned from xds::read()\n");
    fflush(stdout);

    if (status->isError()) {
        fprintf(stderr,
                "read() returned error status, class: %s, code: %s\n",
                xds_error_class_name(status->error_class),
                xds_error_code_name(status->error_code));
        if (status->error_class == XDS_C_NAME_ERROR) {

            if (error = status->get_name(res_name)) {
                fprintf(stderr,
                        "Error %d extracting name from status\n",
                        error);
            } else {
                temp_buffer.empty();
                res_name->display(temp_buffer);
                fprintf(stderr,
                        "Matched: %.*s\n",
                        temp_buffer.data_length,
                        temp_buffer.data);
                temp_buffer.empty();
            };
        };
        fflush(stderr);
        return -1;
    } else {
        fprintf(stdout, "READ succeeded.  Decoding results...\n");
        fflush(stdout);
// We did a successful read.  Display the results...
        if ((error = read_results->get_object_name(res_name)) == 0) {
            temp_buffer.empty();
            res_name->display(temp_buffer);
            fprintf(stdout, "Results of READ for object %.*s:\n",
                    temp_buffer.data_length, temp_buffer.data);
            temp_buffer.empty();
        } else {
            fprintf(stderr,
                    "Error %d extracting name from top-level result\n",
                    error);
            fflush(stderr);
        };
        if ((error = read_results->get_first_attribute(attribute)) == 0) {
            while (error == 0) {
                if ((error = attribute->get_type(attr_oid)) != 0) {
                    fprintf(stderr,
                            "Error %d extracting OID from attribute\n",
                            error);
                    fflush(stderr);
                    continue;
                };
                fprintf(stdout, "Attribute:\n");
                fprintf(stdout,
		        "    Type: ");
                for (i=0; i<attr_oid->length; i++) {
                    fprintf(stdout,
                        " %2.2X",
                        (unsigned)(((unsigned char *)(attr_oid->elements))[i]));
                };
		fprintf(stdout, "\n");
		fflush(stdout);
		if ((error = attribute->get_first_value(value)) != 0) {
		    fprintf(stderr,
		            "Error %d reading first attribute value\n");
		    fflush(stderr);
		} else {
		    while (error == 0) {
			switch (value->theValueType) {
			case VC_CHARSTRING:
			    fprintf(stdout,
			            "    Value (Charstring, %d bytes): ",
				    value->theString.length);
			    fprintf(stdout,
				    "%.*s\n",
				    value->theString.length,
				    value->theString.elements);
			    fflush(stdout);
			    break;
			case VC_INTEGER:
			    fprintf(stdout, "    Value (Integer): ");
			    fprintf(stdout,
				    "%ld\n",
				    value->theInteger);
			    fflush(stdout);
			    break;
			case VC_OCTETSTRING:
			    fprintf(stdout, "    Value (Octetstring): ");
			    for (i=0, j=26; i<value->theString.length; i++) {
				fprintf(stdout,
				        " %2.2X",
			    ((unsigned char *)(value->theString.elements))[i]);
				if (j >= 70) {
				    fprintf(stdout,
				            "\n                         ");
				    j = 26;
				} else j+=3;
			    };
			    fprintf(stdout, "\n");
			    fflush(stdout);
			    break;
			case VC_BOOLEAN:
			    fprintf(stdout, "    Value (Boolean): ");
			    if (value->theBoolean)
			        fprintf(stdout, "TRUE\n");
			    else fprintf(stdout, "FALSE\n");
			    fflush(stdout);
			    break;
			default:
			    fprintf(stdout,
                                  "Unexpected value-class (%d), syntax = %d\n",
                                  value->theValueType,
                                  value->theValueSyntax);
			};
			error = attribute->get_next_value(value);
		    };
		};
                error = attribute->get_next_attribute(attribute);
            };
        } else {
            fprintf(stderr,
                    "Error %d extracting first attribute from read results\n",
                    error);
            fflush(stderr);
        };
    };
    fprintf(stdout, "Deleting status return...\n");
    fflush(stdout);
    delete status;
    fprintf(stdout, "Deleting list results...\n");
    fflush(stdout);
    delete read_results;
    return 0;
}



int delete_command(int & done, char * params) {
    x500name name;
    xds_status_t * status;
    unsigned32 error;
    buffer_t temp_buffer;
    const x500name * res_name;    
    const RelativeDistinguishedName * res_rdn;    
    const xds_attribute_t * attribute;
    const OM_object_identifier * attr_oid;
    int i;

    
    error = name.set_value(params);
    if (error) {
	fprintf(stdout, "Error parsing name (0x%x)\n", error);
	fflush(stdout);
	return -1;
    };    
    
    status = xds.remove_entry(name);

    fprintf(stdout, "Returned from xds::remove_entry()\n");
    fflush(stdout);

    if (status->isError()) {
        fprintf(stderr,
                "remove_entry() returned error status, class: %s, code: %s\n",
                xds_error_class_name(status->error_class),
                xds_error_code_name(status->error_code));
        if (status->error_class == XDS_C_NAME_ERROR) {

            if (error = status->get_name(res_name)) {
                fprintf(stderr,
                        "Error %d extracting name from status\n",
                        error);
            } else {
                temp_buffer.empty();
                res_name->display(temp_buffer);
                fprintf(stderr,
                        "Matched: %.*s\n",
                        temp_buffer.data_length,
                        temp_buffer.data);
                temp_buffer.empty();
            };
        };
        fflush(stderr);
        return -1;
    } else {
        fprintf(stdout, "DELETE succeeded.\n");
        fflush(stdout);
    };
    fprintf(stdout, "Deleting status return...\n");
    fflush(stdout);
    delete status;
    return 0;
}





int create_command(int & done, char * params) {
    x500name name;
    xds_status_t * status;
    xds_read_result_t * read_results;
    xds_selection_t selection;
    unsigned32 error;
    buffer_t temp_buffer;
    const x500name * res_name;    
/*
    xds_attribute_list_t attribute_list;
    xds_value_list_t value;
    OM_object_identifier attr_oid;
*/
    int i;

    
    fprintf(stdout, "DEBUG: Converting name for CREATE operation\n");
    fflush(stdout);
    error = name.set_value(params);
    if (error) {
	fprintf(stdout, "Error parsing name (0x%x)\n", error);
	fflush(stdout);
	return -1;
    };    

    fprintf(stdout, "DEBUG: Converted...\n");
    fflush(stdout);

/*									    
    attr_oid.length = 4;
    attr_oid.elements = "\x2b\x0c\x1f\x1f";

    fprintf(stdout, "DEBUG: Calling ADD_VALUE to set int(1) value\n");
    fflush(stdout);
    value.add_value(OM_S_INTEGER, 1);


    fprintf(stdout, "DEBUG: Calling ADD_ATTRIBUTE to set 2b.0c.1f.1f=int(1)\n");
    fflush(stdout);
    attribute_list.add_attribute(attr_oid, value);
*/
    fprintf(stdout, "DEBUG: Calling ADD_ENTRY to commit it...\n");
    fflush(stdout);
/*
    status = xds.add_entry(name, attribute_list);
*/
    status = xds.add_entry(name);

    fprintf(stdout, "Returned from xds::add_entry()\n");
    fflush(stdout);

    if (status->isError()) {
        fprintf(stderr,
                "add_entry() returned error status, class: %s, code: %s\n",
                xds_error_class_name(status->error_class),
                xds_error_code_name(status->error_code));
        if (status->error_class == XDS_C_NAME_ERROR) {

            if (error = status->get_name(res_name)) {
                fprintf(stderr,
                        "Error %d extracting name from status\n",
                        error);
            } else {
                temp_buffer.empty();
                res_name->display(temp_buffer);
                fprintf(stderr,
                        "Matched: %.*s\n",
                        temp_buffer.data_length,
                        temp_buffer.data);
                temp_buffer.empty();
            };
        };
        fflush(stderr);
        return -1;
    } else {
        fprintf(stdout, "ADD_ENTRY succeeded\n");
        fflush(stdout);
    };
    fprintf(stdout, "Deleting status return...\n");
    fflush(stdout);
    delete status;
    return 0;
}



int help_command(int & done, char * params) {
    int i;
    fprintf(stdout, "Commands available:\n");
    for (i=0;commands[i].str != NULL;i++) {
	fprintf(stdout, "   %s\n", commands[i].str);
    }
    fprintf(stdout,"\n");

    return 0;
}

int add_command(int & done, char * params) {
    x500name name;
    xds_status_t * status;
    xds_read_result_t * read_results;
    xds_selection_t selection;
    unsigned32 error;
    buffer_t temp_buffer;
    const x500name * res_name;    
    xds_value_list_t attribute_value;
    OM_object_identifier attr_oid;
    char oid_name[256];
    char attr_syntax_name[256];
    int i;
    asn_objectidentifier attr;
    int op_done;
    int show_help;
    int from_file = 0;
    char attr_value[512];
    FILE * file;
    long int_value;
    unsigned char octetstring_value[4096];
    size_t os_length;
    unsigned char length_bytes[4];
    buffer_t oid_str;
    xds_changes_t change_list;
            
//    fprintf(stdout, "DEBUG: Converting name for ADD_ATTRIBUTE operation\n");
//    fflush(stdout);

    error = name.set_value(params);
    if (error) {
	fprintf(stdout, "Error parsing name (0x%x)\n", error);
	fflush(stdout);
	return -1;
    };    

//    fprintf(stdout, "DEBUG: Converted...\n");
//    fflush(stdout);

// Prompt the user for an attribute OID and a value...
//

    op_done = 0;
    while (!op_done) {
	op_done = 1;
	fprintf(stdout, "Attribute OID ('Q' to quit): ");
	fflush(stdout);
	fgets(oid_name, sizeof(oid_name), stdin);
	if (toupper(oid_name[0]) == 'Q') return -1;

	if ((error = attr.set_value(oid_name)) != 0) {
	    fprintf(stderr, "Error: OID conversion returned %d\n", error);
	    fflush(stderr);
	    op_done = 0;
	};
    };


    op_done = 0; show_help = 0;
    while (!op_done) {
	op_done = 1;

	if (show_help) {
	    fprintf(stdout, "Supported attribute syntaxes:\n");
	    fprintf(stdout, "   I : Integer\n");
	    fprintf(stdout, "   B : Boolean (T or F)\n");
	    fprintf(stdout, "   P : Printable string\n");
	    fprintf(stdout, "   T : Teletex (T.61) string\n");
//	    fprintf(stdout, "   O : Octet String (typed at prompt)\n");
	    fprintf(stdout, "   F : Octet String (read from a file)\n");
	};
	fprintf(stdout, "Attribute syntax ('?' for list, 'Q' to quit): ");
	fflush(stdout);
	fgets(attr_syntax_name, sizeof(attr_syntax_name), stdin);
	switch (toupper(attr_syntax_name[0])) {
	case 'Q': return -1;
	case 'I':
	    fprintf(stdout, "Value: ");
	    fflush(stdout);
	    fgets(attr_value, sizeof(attr_value), stdin);
	    if (sscanf(attr_value, "%ld", &int_value) != 1) {
		fprintf(stderr,
		        "Error parsing \"%s\" as integer\n",
			attr_value);
		fflush(stderr);
		return 1;
	    };
	    if ((error = attribute_value.add_value(OM_S_INTEGER, int_value)
	        ) != 0) {
		fprintf(stderr, "Error %d adding value\n", error);
		fflush(stderr);
		return -1;
	    };
	    break;
/*									    
	case 'O':
	    fprintf(stdout, "Value: ");
	    fflush(stdout);
	    fgets(attr_value, sizeof(attr_value), stdin);
	    if (sscanf(attr_value, "%ld", &int_value) != 1) {
		fprintf(stderr,
		        "Error parsing \"%s\" as integer\n",
			attr_value);
		fflush(stderr);
		return 1;
	    };
	    if ((error = attribute_value.add_value(OM_S_INTEGER, int_value)
	        ) != 0) {
		fprintf(stderr, "Error %d adding value\n", error);
		fflush(stderr);
		return -1;
	    };
	    break;
*/
	case 'F':
	    fprintf(stdout,
"The file must contain the octetstring, preeceeded by a 4-byte length field.\n");
	    fprintf(stdout, "File (CR to quit): ");
	    fflush(stdout);
	    fgets(attr_value, sizeof(attr_value), stdin);
	    if (attr_value[0] == '\n') return -1;
            if (attr_value[strlen(attr_value)-1] ==  '\n')
              attr_value[strlen(attr_value)-1] = '\0';
	    if ((file = fopen(attr_value, "rb")) == NULL) {
		fprintf(stderr, "Couldn't open file\n");
		fflush(stderr);
		op_done = 0; continue;		
	    };


            os_length = 0;
            while (fgetc (file) != EOF) {
              os_length++;
            }
            fclose(file);

            if (os_length > sizeof(octetstring_value)) {
              fprintf(stderr,
	      "Error: value too long (%ld bytes)\n",
	      (long)os_length);
              fflush(stderr);
              fclose(file);
              op_done = 0; continue;		
	    }
 
	    if (os_length > sizeof(octetstring_value)) {
		fprintf(stderr,
			"Error: value too long (%ld bytes)\n",
			(long)os_length);
		fflush(stderr);
		op_done = 0; continue;		
	    };

	    if ((file = fopen(attr_value, "rb")) == NULL) {
		fprintf(stderr, "Couldn't open file again\n");
		fflush(stderr);
		op_done = 0; continue;		
	    };

            if (fread((void *) octetstring_value, 1, (size_t) os_length, file) != 
                (size_t) os_length) {
		fprintf(stderr, "Couldn't read os_length bytes from file\n");
		fflush(stderr);
		op_done = 0; continue;		
             }

            fclose(file);
	    if ((error = attribute_value.add_value(OM_S_OCTET_STRING,
                                                   octetstring_value,
						   os_length)
	        ) != 0) {
		fprintf(stderr, "Error %d adding value\n", error);
		fflush(stderr);
		return -1;
	    };
	    break;
	case 'V':
	case 'P':
	    fprintf(stdout, "Value: ");
	    fflush(stdout);
	    fgets(attr_value, sizeof(attr_value), stdin);
	    attr_value[strlen(attr_value)-1] = 0; // remove trailing '\n'
	    if ((error =
		attribute_value.add_value((toupper(attr_syntax_name[0]) == 'P'
		                           ?OM_S_PRINTABLE_STRING
					   :OM_S_VIDEOTEX_STRING),
					  attr_value)
	        ) != 0) {
		fprintf(stderr, "Error %d adding value\n", error);
		fflush(stderr);
		return -1;
	    };
	    break;
	case 'B':
	    fprintf(stdout, "Value (T or F): ");
	    fflush(stdout);
	    fgets(attr_value, sizeof(attr_value), stdin);
	    if (toupper(attr_value[0]) == 'T') int_value = 1;
	    else if (toupper(attr_value[0]) == 'F') int_value = 0;
	    else {
		fprintf(stderr,
			"Error: Invalid value (%c)\n",
			attr_value[0]);
		fflush(stderr);
		op_done = 0; continue;		
	    
	    };
	    if ((error = attribute_value.add_value(OM_S_BOOLEAN, int_value)
	        ) != 0) {
		fprintf(stderr, "Error %d adding value\n", error);
		fflush(stderr);
		return -1;
	    };
	    break;
	default:
	    fprintf(stderr,
	            "Error: '%c' not recognized\n",
		    attr_syntax_name[0]);
	    fflush(stderr);
	case '?':
	    show_help = 1;
	    op_done = 0;
	    break;
	};
    };

    



    if ((error = attr.calculate_value(oid_str)) != 0) {
	fprintf(stderr, "Error %d encoding OID\n", error);
	fflush(stderr);
	return -1;
    };
    attr_oid.length = oid_str.data_length;
    attr_oid.elements = oid_str.data;
    fprintf(stdout, "DEBUG: Calling ADD_ATTRIBUTE\n");
    fflush(stdout);
    change_list.add_attribute(attr_oid, attribute_value);

    fprintf(stdout, "DEBUG: Calling MODIFY to commit it...\n");
    fflush(stdout);
    status = xds.modify(name,
                        change_list);

    fprintf(stdout, "Returned from xds::modify()\n");
    fflush(stdout);

    if (status->isError()) {
        fprintf(stderr,
                "add_entry() returned error status, class: %s, code: %s\n",
                xds_error_class_name(status->error_class),
                xds_error_code_name(status->error_code));
        if (status->error_class == XDS_C_NAME_ERROR) {

            if (error = status->get_name(res_name)) {
                fprintf(stderr,
                        "Error %d extracting name from status\n",
                        error);
            } else {
                temp_buffer.empty();
                res_name->display(temp_buffer);
                fprintf(stderr,
                        "Matched: %.*s\n",
                        temp_buffer.data_length,
                        temp_buffer.data);
                temp_buffer.empty();
            };
        };
        fflush(stderr);
        return -1;
    } else {
        fprintf(stdout, "MODIFY succeeded\n");
        fflush(stdout);
    };
    fprintf(stdout, "Deleting status return...\n");
    fflush(stdout);
    delete status;
    return 0;
}

int quit_command(int & done, char * params) {
    done = 1;
    return 0;
}

int process_command(int & done, char * line) {
    int fields_read;
    char command[256];
    int comm_end;
    int i;

    
    for (i=0; i < 256 && line[i]!=0 && line[i]!=' ' && line[i]!='\n'; i++) {
	command[i] = tolower(line[i]);
    };

    command[comm_end = i] = 0;
    if (i==0) return 0; // Ignore null commands

    while (line[comm_end] == ' ' || line[comm_end] == '\n') comm_end++;
    for (i=0;;i++) {
        if (commands[i].str == NULL) break;
        if (strcmp(command, commands[i].str) == 0) {
            return commands[i].f(done, line + comm_end);
        };
    };
    fprintf(stdout, "Invalid command (not recognized): \"%s\"\n", command);
    return 0;
}

#ifdef FORCE_THREADS
static void * thread_routine (void * addr) {
    return NULL;
}
#endif

int main(int argc, char * argv[]) {
    char line[256];
    int done;
    int i;
#ifdef FORCE_THREADS
    pthread_t the_thread;

    pthread_create(&the_thread,
                   pthread_attr_default,
                   thread_routine,
                   NULL);
#endif
    
    if (argc <= 1) {

	if (xds.cds_supported()) fprintf(stdout, "CDS operations supported\n");
	else fprintf(stdout, "CDS operations unavailable\n");

	if (xds.gds_supported()) fprintf(stdout, "GDS operations supported\n");
	else fprintf(stdout, "GDS operations unavailable\n");

        done = 0;
        while(!done) {
            pkc_malloc_status;
            fprintf(stdout, "XDS-CP> "); fflush(stdout);
            fgets(line, sizeof(line), stdin);
            for(i=0;line[i]!=0;i++) {
                if (line[i]=='\n') {
                    line[i] = 0;
                    break;
                };
            };
            process_command(done, line);
        };
        
    } else {
        fprintf(stderr, "Non-interactive use not yet implemented\n");
        fflush(stderr);
        return EXIT_FAILURE;
    };
}
