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
 * $Log: trycase_a.cxx,v $
 * Revision 1.1.2.1  1996/11/14  17:13:40  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  15:56:37  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:33 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  22:53 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * 	Initial revision
 * 	[1996/10/15  19:28:36  pkdev  1.1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:45 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:46 UTC  arvind  /main/arvind_capi/1]
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
 * ABSTRACT:	Test program for Hierarchical Policy 
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




/* mbg : This test program goes thru each test case in the "testcases" directory,
 * executes them as per the hierarchical trust policy , and compares the expected
 * outcome against the real outcome.
 * The discussion regarding the format of each file is in the "readme" file
 * in this directory.
 */


extern "C" {
#include <ctype.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sec_bsafe.h>

#include <pthread.h>
#include <asn.h>
#include <dce/utc.h>
#include <dce/dce_cf.h>
#include <dce/cdsclerk.h>
#ifdef ENVIRONMENT_TET
#include <tet_api.h>
#endif
}


#include <dce/x500name.h>
#include <dce/xdsxx.h>
#include <dce/x509.h>

/* #include <pkc_certs.h> */
/* #include <pkc_plcy_reg.h> */
#include <dce/pkc_api.h>
#include <dce/pkc_plcy_oid.h>



#ifdef ENVIRONMENT_TET

static void startup(void)
{
return;
}
static void cleanup(void)
{
return;
}


static void startup(void);
static void cleanup(void);
static void tp(void);

void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;

struct tet_testlist tet_testlist[] = {
{tp, 1},
{NULL, 0}
};


#define HIERARCHICAL_TEST_ARGUMENTS "HIERARCHICAL_TEST_ARGUMENTS"

#endif




char *return_right_most_rdn (char *princ) ;

#include "common_defines_a.h"

#ifdef IMPLEMENTED
extern pkc_policy_t pkc_hierarchy;
#endif

extern "C" int create_dir(char *);
extern  "C" int delete_dir(char *);

static char *xds_cds_val;
static char cds_config_file_name[BUFSIZE];
static char cds_create_file_name[BUFSIZE];
static char cds_delete_file_name[BUFSIZE];
static char cds_princ_attr_file_name[BUFSIZE];



FILE *flog = 0;


 xds_t xds; 


void print_keydata (unsigned char *keydata, size_t keylength)
{
  int i, k;


  print_general_2("    Key length  = %ld\n",keylength);
  print_general_1("     * * * Key DATa =  * * * \n");
  i = 0;
  while (i < keylength) {
    print_general_1("         ");
    for (k = 0; k < 16; k++) {
      if (i == keylength) break;
      print_general_2("%02x ",keydata[i]); 
      i++;
    }
    print_general_1("\n");
  }
  print_general_1("         * * * Key DATa =  * * * \n");
}


int read_command(char * params) 
{
    x500name name;
    xds_status_t * status;
    xds_read_result_t * read_results;
    xds_selection_t selection;
    unsigned32 error;
    
    error = name.set_value(params);
    if (error) {
        print_general_3("** ERROR parsing x500 name %s\n", params);
	return -1;
    };    
    

    status = xds.read(name, selection, read_results);


    if (status->isError()) {
        return -1;
    }

    return 0;
}

int check_configuration(char  *fname)
{
  FILE *fp;
  char line [BUFSIZE];
  char filename [BUFSIZE];
  char *cellname;
  char cell_x500_name[BUFSIZE];
  int x500_flag, matched;
  unsigned32 st;

  strcpy((char *) &filename[0], TEST_DIR);
  strcat((char *) &filename[0], "/");
  strcat((char *) &filename[0],  fname);

  fp = fopen (filename, "r");
  if (!fp) {
    print_general_3("    ** ERROR   Configuration Check file %s does not exist\n",filename);
    return -1;
  }

  if (fgets(line, BUFSIZE, fp) == 0) {
    print_general_3("    ** ERROR   Configuration Check file %s is empty\n",filename);
    return -1;
  }

  strip_end_of_line (&line[0]);
  if (parse_token (line, x500_flag, cell_x500_name) != 0) {
    print_general_3("    ** ERROR   Configuration Check file %s\n",filename);
    print_general_3("               Line cannot be parsed  %s\n",line);
    return -1;
  }

  matched = 1;
  if (x500_flag == X500_FLAG) {
    if (read_command (cell_x500_name) != 0) {
      print_general_1 ("*********************************************\n");
      print_general_1 ("The existing X500 server configuration does not match\n");
      print_general_1 ("the one expected by the test suite  \n");
      print_general_1 ("*********************************************\n");
      print_general_3 ("Please check that your  X500 Root entry is %s\n",cell_x500_name);
      print_general_1 ("Verify that your X500 configuration is as per the instructions below\n");
      print_general_1 ("before running the test-hierarchy test suite as well as read the \n");
      print_general_1 ("instructions in README file \n");
      print_general_1 ("*********************************************\n");
      matched = 0;
    }
  } else {
    dce_cf_get_cell_name(&cellname, &st);     
    if (st != dce_cf_st_ok) {
      print_general_2("trycase_a: dce_cf_get_cell_name failed %ld\n",st);
      print_general_1 ("Please re-configure your DCE cell name as per the instructions below\n");
      print_general_1 ("before running the test-hierarchy test suite as well as read the \n");
      print_general_1 ("instructions in README file \n");
      matched = 0;
    } else {

      if (strcmp (cellname, cell_x500_name) != 0) {
        print_general_1 ("*********************************************\n");
        print_general_3 ("The existing DCE Cell name %s does not match  \n",cellname);
        print_general_3 ("       Expected Cell name %s   \n",cell_x500_name);
        print_general_1 ("*********************************************\n");
        print_general_1 ("Please re-configure your DCE cell name as per the instructions below\n");
        print_general_1 ("before running the test-hierarchy test suite as well as read the \n");
        print_general_1 ("instructions in README file \n");
        print_general_1 ("*********************************************\n");
        matched = 0;
      }
    }
  }

  if (matched == 1) {
    fclose (fp);
    return 0;
  }

  /* dump the next series of instructions in filename */  
  while (fgets(line, BUFSIZE, fp) != 0) {
    print_general_1 (line);
  }   

  fclose (fp);

  /* exit with a success status anyways.. */
  /* so that the rest of the test-suite in security will not be affected */
  return -1;
}





/* mbg: copied from xds_test.cxx; deletes an xds entry  */
/* params is the name of x500 entry to delete */

unsigned32 delete_command(char * params) 
{
    x500name name;
    xds_status_t * status;
    unsigned32 error;
    buffer_t temp_buffer;
    const x500name * res_name;    
    const RelativeDistinguishedName * res_rdn;    
    const xds_attribute_t * attribute;
    const OM_object_identifier * attr_oid;
    int i;

    
    name.set_value(params);
    
    status = xds.remove_entry(name);

    print_general_1("Returned from xds::remove_entry()\n");

    if(status->isError()) {
        print_general_3(
                "** ERROR ** Delete_command: remove_entry() returned error status, class: %s",
                 xds_error_class_name(status->error_class));
        print_general_3(
                "  error code = %s",  xds_error_code_name(status->error_code));
        if (status->error_class == XDS_C_NAME_ERROR) {

            if (error = status->get_name(res_name)) {
                print_general_2(
                        "Error %d extracting name from status\n",
                        error);
            } else {
                temp_buffer.empty();
                res_name->display(temp_buffer);
                temp_buffer.empty();
            };
        };
        return -1;
    } else {
        print_general_1("DELETE succeeded.\n");
    };
    delete status;
    return 0;
}


/* mbg: copied from xds_test.cxx; creates an xds entry  */
/* params is the name of x500 entry to delete */


unsigned32 create_command(char * params) 
{
    x500name name;
    xds_status_t * status;
    xds_read_result_t * read_results;
    xds_selection_t selection;
    unsigned32 error;
    buffer_t temp_buffer;
    const x500name * res_name;    
    int i;
    xds_value_list_t  attribute_value, sn_attr_value, null_attr_value, self_attr_value;
    asn_objectidentifier asnobject;
    xds_attribute_list_t attr_list;
    gss_OID gsoid = 0;

#define OBJECT_CLASS_OID_STR   "2.5.4.0"
#define ORG_U_OID_STR          "2.5.6.5"
#define ORGP_U_OID_STR          "2.5.6.7"
#define PER_U_OID_STR          "2.5.6.6"
#define SN_U_OID_STR          "2.5.4.4"
#define SAU_OID_STR            "2.5.6.15"
#define CA_AU_OID_STR          "2.5.6.16"
#define TOP_OID_STR            "2.5.6.0"

    print_general_1("DEBUG: Converting name for CREATE operation\n");

    name.set_value(params);

    print_general_1("DEBUG: Converted...\n");

    print_general_1("DEBUG: Calling ADD_ENTRY to commit it...\n");


    error = asnobject.set_value(TOP_OID_STR);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;

    if ((error = attribute_value.add_value(OM_S_OBJECT_IDENTIFIER_STRING,
                                           (unsigned char *) gsoid->elements,
					   gsoid->length)
        ) != 0) {
	print_general_2("Error %d adding value\n", error);
	return -1;
    };

    error = asnobject.set_value(ORG_U_OID_STR);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;

    if ((error = attribute_value.add_value(OM_S_OBJECT_IDENTIFIER_STRING,
                                           (unsigned char *) gsoid->elements,
					   gsoid->length)
        ) != 0) {
	print_general_2( "Error %d adding value\n", error);
	return -1;
    };



#if 0
    error = asnobject.set_value(PER_U_OID_STR);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;

    if ((error = attribute_value.add_value(OM_S_OBJECT_IDENTIFIER_STRING,
                                           (unsigned char *) gsoid->elements,
					   gsoid->length)
        ) != 0) {
	print_general_2("Error %d adding value\n", error);
	return -1;
    };

#endif

    error = asnobject.set_value(SAU_OID_STR);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;

    if ((error = attribute_value.add_value(OM_S_OBJECT_IDENTIFIER_STRING,
                                           (unsigned char *) gsoid->elements,
					   gsoid->length)
        ) != 0) {
	print_general_2("Error %d adding value\n", error);
	return -1;
    };


    error = asnobject.set_value(CA_AU_OID_STR);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;

    if ((error = attribute_value.add_value(OM_S_OBJECT_IDENTIFIER_STRING,
                                           (unsigned char *) gsoid->elements,
					   gsoid->length)
        ) != 0) {
	print_general_2("Error %d adding value\n", error);
	return -1;
    };





    error = asnobject.set_value(OBJECT_CLASS_OID_STR);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;
    error = attr_list.add_attribute(*gsoid, attribute_value);
    if (error != 0) return error;




#if 0
    error = asnobject.set_value(USER_CERT_OID);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;

    if ((error = null_attr_value.add_value(OM_S_ENCODING_STRING,
                                           (unsigned char *) "abc",
					   4)
        ) != 0) {
	print_general_2( "Error %d adding value\n", error);
	return -1;
    };

    error = attr_list.add_attribute(*gsoid, null_attr_value);
    if (error != 0) return error;

    error = asnobject.set_value(SN_U_OID_STR);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;

    if ((error = sn_attr_value.add_value(OM_S_TELETEX_STRING,
                                           (unsigned char *) "test",
					   5)
        ) != 0) {
	print_general_2( "Error %d adding value\n", error);
	return -1;
    };


    error = attr_list.add_attribute(*gsoid, sn_attr_value);
    if (error != 0) return error;

    
#endif





#if 0

    error = asnobject.set_value(CROSS_CERT_OID);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;
    error = attr_list.add_attribute(*gsoid, null_attr_value);
    if (error != 0) return error;

    error = asnobject.set_value(CA_CERT_OID);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;
    error = attr_list.add_attribute(*gsoid, null_attr_value);
    if (error != 0) return error;

    error = asnobject.set_value(CA_REVOCATION_LIST_OID);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;
    error = attr_list.add_attribute(*gsoid, null_attr_value);
    if (error != 0) return error;

    error = asnobject.set_value(USER_REVOCATION_LIST_OID);
    if (error != 0) return error;
    error = asnobject.get_value (gsoid);
    if (error != 0) return error;
    error = attr_list.add_attribute(*gsoid, null_attr_value);
    if (error != 0) return error;

#endif



    status = xds.add_entry(name, attr_list);

    print_general_1("Returned from xds::add_entry()\n");

    if (status->isError()) {
        print_general_3(
                "** ERROR ** add_entry() returned error status, class: %s ", 
                 xds_error_class_name(status->error_class));
        print_general_3("  , code: %s\n",  xds_error_code_name(status->error_code));
        if (status->error_class == XDS_C_NAME_ERROR) {

            if (error = status->get_name(res_name)) {
                print_general_2(
                        "Error %d extracting name from status\n",
                        error);
            } else {
                temp_buffer.empty();
                res_name->display(temp_buffer);
                temp_buffer.empty();
            };
        };
        return -1;
    } else {
        print_general_1("ADD_ENTRY succeeded\n");
    };
    delete status;
    return 0;
}

#define PRINCIPAL_DIRECTORY_OID_CONST       1
#define USER_CERT_OID_CONST                 2
#define CROSS_CERT_OID_CONST                3
#define CA_CERT_OID_CONST                   4
#define CA_REVOCATION_LIST_OID_CONST        5
#define USER_REVOCATION_LIST_OID_CONST      6


int return_oid_type (char *oid_str) 
{
  if (strcmp(oid_str,PRINCIPAL_DIRECTORY_OID) == 0)
    return PRINCIPAL_DIRECTORY_OID_CONST;
  if (strcmp(oid_str,USER_CERT_OID) == 0)
    return USER_CERT_OID_CONST;
  if (strcmp(oid_str,CROSS_CERT_OID) == 0)
    return CROSS_CERT_OID_CONST;
  if (strcmp(oid_str,CA_CERT_OID) == 0)
    return CA_CERT_OID_CONST;
  if (strcmp(oid_str,CA_REVOCATION_LIST_OID) == 0)
    return CA_REVOCATION_LIST_OID_CONST;
  if (strcmp(oid_str,USER_REVOCATION_LIST_OID) == 0)
    return USER_REVOCATION_LIST_OID_CONST;
  return -1;
}
   

/* mbg: copied from xds_test.cxx; adds attr=value1 to an xds entry,
 * and value1 etc is octet strings. params is x500 name, attr = Certificate OID,
 * and fname points to file that contains the octet string 
 */

int add_command(char * params, char *attr_str, char *fname, int forstr) 
{
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
    char octetstring_value[4096];
    size_t os_length;
    int c;
    unsigned char length_bytes[4];
    buffer_t oid_str;
    xds_changes_t change_list;
    OM_object om;
    Certificate cert;
    r_buffer_t from_asn;
    CrossCertificatePair crpair;
    SignedCertificateList crl;
            
    name.set_value(params);

    if (forstr == 1) {
      os_length = 4096;
      if (read_bytes_from_file(fname, octetstring_value, os_length) != 0)
        return -1;
    } else {
      os_length = strlen(fname)+1;
      memcpy (octetstring_value,fname,os_length);
    }

    if ((error = attribute_value.add_value(OM_S_ENCODING_STRING,
                                        (unsigned char *) octetstring_value,
                			 os_length)
            ) != 0) {
         print_general_2("Error %d adding value\n", error);
         return -1;
     };




    strcpy(oid_name,attr_str);

    if ((error = attr.set_value(oid_name)) != 0) {
      print_general_2("Error: OID conversion returned %d\n", error);
    };


    if ((error = attr.calculate_value(oid_str)) != 0) {
	print_general_2("Error %d encoding OID\n", error);
	return -1;
    };

    attr_oid.length = oid_str.data_length;
    attr_oid.elements = oid_str.data;
    print_general_1("DEBUG: Calling ADD_ATTRIBUTE\n");
    change_list.add_attribute(attr_oid, attribute_value);

    print_general_1("DEBUG: Calling MODIFY to commit it...\n");
    status = xds.modify(name,
                        change_list);

    print_general_1("Returned from xds::modify()\n");

    if (status->isError()) {
        print_general_3(
                "add_entry() returned error status, class: %s, ",
                 xds_error_class_name(status->error_class));
        print_general_3(
                " code: %s\n",  xds_error_code_name(status->error_code));
        if (status->error_class == XDS_C_NAME_ERROR) {

            if (error = status->get_name(res_name)) {
                print_general_2(
                        "Error %d extracting name from status\n",
                        error);
            } else {
                temp_buffer.empty();
                res_name->display(temp_buffer);
                temp_buffer.empty();
            };
        };
        return -1;
    } else {
        print_general_1("MODIFY succeeded\n");
    };
    delete status;
    return 0;
}



void display_error(const char * routine_name,
                  unsigned32 status) 
{
    print_general_3("Error returned by %s\n", routine_name);
    print_general_2("Error code: %lx\n", (unsigned long)status);
}

     
    
  

/* for each certificate listed in line, exports the relevant
 * certificate to an xds entry as per the convention for filenames
 * specified in the readme file
 */


unsigned32 export_certificates(char *line)
{
  char *p_str;
  char *only_princ;
  char file_name[BUFSIZE];
  unsigned32 status;
  char certfname[BUFSIZE],loc[BUFSIZE];
  int attr_flag, xds_flag;



  if (parse_cert_loc_token(line, attr_flag,certfname,xds_flag,loc) != 0) {
    print_general_3("Error in parsing cert loc token %s\n",line);
    return -1;
  }


  /* now, add a certificate to the entry */
  strcpy(file_name, CERTS_DIR);
  strcat(file_name,"/");
  strcat(file_name,certfname);

  switch (attr_flag) {
    case USER_CERT_OID_FLAG:
      if (xds_flag == X500_FLAG)  
        status = add_command((char *) &loc[0], USER_CERT_OID, file_name, 1);    
      else
        status = create_attr_value_from_file((char *) &loc[0], USER_CERT_OID, file_name);    

      break;

    case CA_CERT_OID_FLAG:
      if (xds_flag == X500_FLAG)  
        status = add_command((char *) &loc[0], CA_CERT_OID, file_name, 1);    
      else
        status = create_attr_value_from_file((char *) &loc[0], CA_CERT_OID, file_name);    

      break;

    case CROSS_CERT_OID_FLAG:
      if (xds_flag == X500_FLAG)  
        status = add_command((char *) &loc[0], CROSS_CERT_OID, file_name, 1);    
      else
        status = create_attr_value_from_file((char *) &loc[0], CROSS_CERT_OID, file_name);    

      break;

    case USER_REVOKE_OID_FLAG:
      if (xds_flag == X500_FLAG)  
        status = add_command((char *) &loc[0], USER_REVOCATION_LIST_OID, file_name, 1);    
      else
        status = create_attr_value_from_file((char *) &loc[0], USER_REVOCATION_LIST_OID, file_name);    

      break;

    case CA_REVOKE_OID_FLAG:
      if (xds_flag == X500_FLAG)  
        status = add_command((char *) &loc[0], CA_REVOCATION_LIST_OID, file_name, 1);    
      else
        status = create_attr_value_from_file((char *) &loc[0], CA_REVOCATION_LIST_OID, file_name);    

      break;

    default:
      return -1;
  }

  return status;
}

 
/*
 * form_key: princ is of the form i_j, where i is princindex and j is keyindex
 */


unsigned32 form_key(char *princ,  trustitem_t *key, char * princ_name) 
{

  char buf[BUFSIZE], *p;
  char public_key_fname[MAX_FILE_LEN];
  FILE *fp;
  char *x;
  unsigned char keydata[5000];
  size_t keylength;
  SubjectPublicKeyInfo s;
  r_buffer_t from_asn;  // instance of buffer class
  unsigned32 status;

  strcpy(buf, princ);

  strcpy(public_key_fname, KEYS_DIR);
  strcat(public_key_fname,"/");
  strcat(public_key_fname,buf);

  fp = fopen(public_key_fname, "r");
  if (!fp) { 
    print_general_3("Problem opening file %s", public_key_fname);
    print_general_2(" errno = %ld \n",errno); 
    return errno;
  }
 
  keylength = fread(keydata, 1, sizeof(keydata), fp);

  fclose(fp);
  if (keylength == sizeof(keydata)) {
    print_general_3("length of key in file %s is longer than 5000\n",public_key_fname);
    return -1;
  }

  key->type = IS_KEY;
  key->u.k.version = 0;
  key->u.k.key =   (unsigned char *) malloc(keylength);
  memcpy(key->u.k.key, keydata, keylength);
  key->u.k.size = keylength;
  key->u.k.ca = (unsigned char *) malloc(strlen(princ_name)+1);
  strcpy((char *) key->u.k.ca, princ_name);

  return 0; 
} 
  

static char *print_ascii_time(utc_t *time)
{
  static char buf[BUFSIZE];
  int flag;

  strcpy (buf," ** ERRROR **");
  utc_ascgmtime((char *) buf, BUFSIZE, time);
  return ((char *) buf);
}
  
unsigned32 test_retrieve_key(char *line, int cline, char *testfname)
{
  char *p, *t, *q, *buf;
  int j, princ_index, test_case, done;
  utc_t notBefore;
  utc_t notAfter;
  x500name subject;
  char complete_princ_name[BUFSIZE], x500_user[BUFSIZE];
  unsigned char *keydata = 0;
  size_t keylength;
  keyinfo_t *keyinfohandle = 0;
  int count_error = 0;

 
  /* ask john.. thse are used in pkc_retrieve_key at the end */
  /* pkc_key_usage_t des_usage;  */
  char initial_explicit_policy_required;
  /* pkc_key_information_t  ret_key; */
  trustitem_t key;

  unsigned32 status;
  char temp[100];
  int count = 0;

  trustlist_t *tr_list = 0;
  trustitem_t tr_item;
  trustbase_t *tbase;
  trusted_key_t *out_keys;
   size_t no_of_keys;
  int x500_flag;

  status = pkc_init_trustlist(&tr_list);
  if (status != 0) {
    print_general_1("problem doing pkc_init_trustlist\n");
   return -1;  /* try the next line */
  }


  /* make public keys valid for sometime ?? */

  status =                        /* try to parse it */
     utc_mkasctime(              /* result is gregorian based */
            &notBefore,
            START_TIME);

 if (status != 0) {
      display_error(" utc_mkasctime", status);
      return status;
    }

  status =                        /* try to parse it */
     utc_mkasctime(              /* result is gregorian based */
            &notAfter,
            END_TIME);

 if (status != 0) {
      display_error(" utc_mkasctime", status);
      return status;
    }

  buf = (char *) malloc (strlen(line)+1);
  if (!buf) {
    print_general_1("Ran out of memory\n");
    return -1;
  }

  strcpy (buf, line);

  /* first token points to the principal whose key is to be retrieved */
  p = find_next_non_blank(buf);
  if (!p) {
    print_general_3("Problem parsing token %s\n",line);
    free(buf);
    return -1;
  }
  q = p;
  p = find_next_blank(p);
  if (!p) {
    print_general_3("Problem parsing token %s\n",line);
    free(buf);
    return -1;
  }
  *p = '\0';
  p++;
  if (parse_token (q, x500_flag,&complete_princ_name[0])) {
    print_general_3("Problem parsing token %s\n",line);
    free(buf);
    return -1;
  }

  princ_index = find_princ_index (&complete_princ_name[0]);
  if (princ_index == -1)  {
    print_general_3("Principal specified in test line %s does not exist in principals file\n",line);
    print_general_1("Aborting testcase line\n");
    free(buf);
    return -1;
  }



  print_general_1("=================================================================\n");
  print_general_3("==================  Forming initial trustkeys for %s ========\n",testfname);
  print_general_1("================================================================\n\n");
  
  /* now parse all remaining token until we hit testcases to try */
  /* in this loop, we create the initial trustlist */
  done = 0;
  while (!done) {

    p = find_next_non_blank(p);
    if (!p) {
      print_general_3("Problem parsing token %s\n",line);
    free(buf);
      return -1;
    }
    q = p;
    p = find_next_blank_or_char(p,':');
    if (!p) {
      print_general_3("Problem parsing token %s\n",line);
    free(buf);
      return -1;
    }

    if (*p == ':') {  
      done = 1;
    }
    

    *p = '\0';
    p++;
    /* q is of the form i_j, where i is the princ index, and j is the key index */

    if (*q == '\0')   /* will happen if there was a space before : */
      break;

    if (parse_princ_key_index (q, x500_user ) != 0) {
      print_general_3("Problem parsing token %s\n",line);
    free(buf);
      return -1;
    }

    status = form_key(q,  &key, x500_user);
    key.u.k.startDate = notBefore;
    key.u.k.endDate = notAfter;
    if (status) {
      print_general_1("Problem creating trusted key \n");
    free(buf);
      return status;
    }

    
    status = pkc_append_to_trustlist(&tr_list, &key,1);
    if (status != 0) {
      display_error(" pkc_append_to_trustlist", status);
    free(buf);
      return status;
    }
   
     

  }  /* !done */



  /* NOw establish trustbase and see if that works; no point processing
   * testcases if establish trustbase fails
   * Also, finsih of the retrieve_keyinfo so you canstart on testcases.
   */


  {
    char *at_the_time_of_running = "1996-10-02-13:01:02";
    utc_t date;  
    int utc_status;
    utc_status = utc_mkasctime(&date, at_the_time_of_running);
    if (utc_status) {
    print_general_2("Error: Can't convert at_the_time_of_running: %d\n", utc_status);
    free(buf);
       return ((error_status_t) utc_status);
    };

  print_general_1("=================================================================\n");
  print_general_3("==================  Establishing trustbase for %s ========\n",testfname);
  print_general_1("================================================================\n\n");

    status = pkc_init_trustbase(&tr_list,
               (gss_OID) &pkc_c_policy_dce_hierarchy,
               &date,
               0,
              &tbase);
    if (status != 0) { 
#ifdef DEBUG
     print_general_2("init trustbase failed %ld\n",status);
#endif
    free(buf);
     return -1;  /* try next retrieve key */
    }

  print_general_1("=================================================================\n");
  print_general_3("==================  Retrieving Keyinfo for %s ========\n",testfname);
  print_general_1("================================================================\n\n");


    status = pkc_retrieve_keyinfo (tbase, 
                                   (char *) &complete_princ_name[0], 
                                    0, 
                                    0, 
                                    NULL, // Any domain
                                    NULL, // All usages,
                                    &keyinfohandle);
    if (status != 0) {
#ifdef DEBUG
     print_general_2("pkc_retrieve_keyinfo failed %ld\n",status);
#endif
    free(buf);
     return -1;  /* try next retrieve key */
    }


  }




  /* now process each individual testcase to try */
  done = 0;
  print_general_1("<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  print_general_3("<<<<<<<<<<<<< TEST RESULTS [%s] >>>>>>>>>>>>>>>\n", testfname);
  print_general_2("         Test Line [%ld] \n", cline);
  while (!done) {
    p = find_next_non_blank(p);
    if (!p) {
      print_general_3("Ended testcases for line %s\n",line);
    free(buf);
      return 0;
    }

    q = p;
    p = find_next_blank_or_char(p,'\0');
    if (!p) {
      print_general_3("Problem parsing token during testcase evaluation %s\n",line);
    free(buf);
      return -1;
    }

    if (*p == '\0') {  
      done = 1;
    }
    

    *p = '\0';
    p++;

    if (parse_test_case_token (q, test_case) != 0) {
      print_general_3("Problem parsing token %s\n",line);
    free(buf);
      return -1;
    }

    switch (test_case) {
      int expected_status, expected_key_count, key_index, cert_index;
      int  expected_m_crl, expected_m_revoked, expected_trust_type;
      int ca_count, expected_ca_count;
      utc_t expected_certstart, expected_certexp, expected_crllastseen,
            expected_nextcrlexpected;
      utc_t certstart, certexp, crllastseen,
            nextcrlexpected;
      char expected_certfname[BUFSIZE], *certfname;
      certification_flags_t cert_flags;
      char is_crl_valid;
      int i, j, sp_flag;
      uuid_t domain;
      unsigned long usages;

      case TEST_KEY_COUNT_FLAG:

        print_general_3 ("======= Testing pkc_get_key_count : token = %s\n", q); 

        if (parse_test_case_token_key_count (q, expected_status, expected_key_count) != 0) {
          print_general_3("       Problem parsing token %s\n",line);
    free(buf);
          return -1;
        }

        status = pkc_get_key_count (keyinfohandle, &no_of_keys);
        if (status != expected_status) {
           print_general_3("       Test KEY COUNT failed for testtoken %s\n", q);
           print_general_4("       expected status = %ld, returned status = %ld\n",
                                           expected_status, status);
           count_error++;
           break;
        }

        if (status == 0 && no_of_keys != expected_key_count) {
          print_general_3("       Test failed to return correct keycount for testtoken %s\n",q);
          print_general_4("       expected count(%ld), returned keycount(%ld)\n",
                               expected_key_count,no_of_keys);
           count_error++;
           break;
        }
 
        print_general_1 ("          Testing Succeded\n"); 

        break;

      case TEST_KEY_DATA_FLAG: 
        print_general_3 ("=== Testing pkc_get_key_data : token = %s\n", q); 

        if (parse_test_case_token_key_data (q, expected_status, key_index) != 0) {
          print_general_3("        Problem parsing token %s\n",line);
    free(buf);
          return -1;
        }

        status = pkc_get_key_data (keyinfohandle, key_index, &keydata, &keylength);

        if (status != expected_status) {
           print_general_3("          Test KEY DATA failed for testtoken %s\n", q);
           print_general_4("           expected status = %ld, returned status = %ld\n",
                                           expected_status, status);
           count_error++;
           break;
        }

        if (status == 0) {
          unsigned char princkeydata[BUFSIZE];
          size_t princkeylen = BUFSIZE;
          if (keydata) {
            int i,k;

            // for now, the keyversion number if always zero 
            // the more generaal testing with multiple version of 
            // keys is a bit more complicated, since we don't know
            // in what order CDS/XDS will return keys.

            
            if (compare_keydata(keydata,keylength, princ_index, 0, (unsigned char *) princkeydata, princkeylen) != 0) {

              count_error++;
              print_general_1("        Test KEY DATA failed to match up keydata in retreived key\n");
              print_general_1("        Returned key follows\n");


              print_keydata (keydata, keylength);
              print_general_1("        Principal Key should be the following\n");
              if (princkeylen != BUFSIZE)
                print_keydata (princkeydata, princkeylen);

            }  else { /* compare key data */
              print_general_1("        Test KEY DATA matched up retreived key succesfully\n");
            }
          } else  {  /* no key data */
             count_error++;
             print_general_1("        Test KEY DATA failed to match up keydata in retreived key\n");
             print_general_1("         * * * Key DATa =  * * * is EMPTY\n");
             print_general_2("        Key length returned = %ld\n",keylength);
             print_general_1("        Principal Key should be the following\n");
             if (princkeylen != BUFSIZE)
               print_keydata (princkeydata, princkeylen);
          } 
       }  /* if status == 0 */

       break;

      case TEST_KEY_TRUST_INFO_FLAG:
        print_general_3 ("=== Testing pkc_get_key_trust_info : token = %s\n", q); 
        if (parse_test_case_token_key_trust_info 
              (q, expected_status, key_index, expected_m_crl,
               expected_m_revoked, expected_trust_type) != 0) {
          print_general_3("          Problem parsing token %s\n",line);
    free(buf);
          return -1;
        }

        status = pkc_get_key_trust_info (keyinfohandle, 
                                         key_index,
                                         &cert_flags,
                                         &domain,
                                        &usages);
        if (status != expected_status) {
           count_error++;
           print_general_3("         Test KEY TRUST INFO failed for testtoken %s\n", q);
           print_general_4("         expected status = %ld, returned status = %ld\n",
                                           expected_status, status);
           break;
        }

        if (status == 0) {
          if (cert_flags.revoked != expected_m_revoked) {
             count_error++;
             print_general_3("       Test KEY TRUST INFO failed for testtoken %s\n", q);
             print_general_4("       expected revoked = %ld, returned revoked = %ld\n",
                                           expected_m_revoked, cert_flags.revoked);
           break;
          }
    
          if (cert_flags.missing_crls != expected_m_crl) {
             print_general_3("       Test KEY TRUST INFO failed for testtoken %s\n", q);
             print_general_4("       expected CRL value = %ld, returned crl value = %ld\n",
                                             expected_m_crl, cert_flags.missing_crls);
           break;
          }

          if (cert_flags.trust_type != expected_trust_type) {
             count_error++;
             print_general_3("       Test KEY TRUST INFO failed for testtoken %s\n", q);
             print_general_4("       expected trusttype = %ld, returned trusttype = %ld\n",
                                           expected_trust_type, cert_flags.trust_type);
           break;
          }
        } 

        print_general_1("        Test KEY TRUST INFO  succeded \n");
        break;

      case TEST_KEY_CERT_COUNT_FLAG:
        print_general_3 ("=== Testing pkc_get_key_certifier_count : token = %s\n", q); 

        if (parse_test_case_token_key_cert_count 
                  (q, expected_status, key_index, expected_ca_count) != 0) {
          print_general_3("          Problem parsing token %s\n",line);
    free(buf);
          return -1;
        }

        status = pkc_get_key_certifier_count(keyinfohandle,
                               key_index, (size_t *) &ca_count);
        if (status != expected_status) {
           count_error++;
           print_general_3("          Test KEY CERT COUNT failed for testtoken %s\n", q);
           print_general_4("          expected status = %ld, returned status = %ld\n",
                                           expected_status, status);
           break;
        }

        if (status != 0) {
          print_general_1("        Test KEY CERT COUNT  succeded \n");
          break;
        }

        if (ca_count != expected_ca_count) {
           count_error++;
           print_general_3("         Test KEY CERT COUNT failed to compare cacount %s\n", q);
           print_general_4("         expected ca_count = %ld, returned ca_count = %ld\n",
                                           expected_ca_count, ca_count);
           break;
        }

        print_general_1("        Test KEY CERT COUNT  succeded \n");
        break;

      case TEST_KEY_CERT_INFO_FLAG:
        print_general_3 ("=== Testing pkc_get_key_certifier_info : token = %s\n", q); 
     
        if (parse_test_case_token_key_cert_info
            (q, expected_status,key_index,cert_index,
              &expected_certstart, &expected_certexp,
              &expected_crllastseen, &expected_nextcrlexpected,
              (char *)expected_certfname) != 0) {
          print_general_3("       Problem parsing token %s\n",line);
    free(buf);
          return -1;
        }


        status = pkc_get_key_certifier_info(keyinfohandle,key_index,
                    cert_index, &certfname,
                    &certstart,&certexp,&is_crl_valid,&crllastseen,&nextcrlexpected);


        if (status != expected_status) {
           count_error++;
             print_general_3("      Test KEY CERT INFO failed for testtoken %s\n", q);
             print_general_4("      expected status = %ld, returned status = %ld\n",
                                           expected_status, status);
           break;
        }

        if (status != 0)  {
          print_general_1("        Test KEY CERT INFO  succeded \n");
          break;
        }

        { 
          unsigned32 st1, st2; 
          x500name t1, t2;

          st1 = t1.set_value (expected_certfname);
          st2 = t2.set_value (certfname);
          if (st1 != 0 || st2 != 0) {
            count_error++;
            if (st1 != 0) {
              print_general_3("      Test KEY CERT INFO failed for creating x500name %s\n", certfname);
             }
            if (st2 != 0) {
              print_general_3("      Test KEY CERT INFO failed for creating x500name %s\n", expected_certfname);
             }
             break;
           }

           if (t1 == t2) {
             print_general_1("       Test KEY CERT INFO succeded in matching certifner names\n");
           } else { 
             print_general_1("       Test KEY CERT INFO failed in matching certifner names\n");
             count_error++;
             break;
           }
        }

#if 0
        if (strcmp (expected_certfname,certfname) != 0) {
           count_error++;
             print_general_3("       Test KEY CERT INFO failed for cacertname %s\n", q);
             print_general_3("       expected certname = %s ", expected_certfname);
             print_general_3(" returned certname = %s\n",  certfname);
               break;
        }
#endif
  
        { 

          utc_cmptype relation;
          status = utc_cmpintervaltime(&relation, &certstart, &expected_certstart);
          if ((relation != utc_equalTo) && (relation != utc_indeterminate)) {
           count_error++;
             print_general_3("       Test KEY CERT INFO failed for certstart %s\n", q);
             print_general_3("        ... Expected Cert Start time = %s\n",
                            print_ascii_time(&expected_certstart));
             print_general_3("        ... Cert Start time = %s\n",
                            print_ascii_time(&certstart));
               break;
          }
          status = utc_cmpmidtime(&relation, &certexp, &expected_certexp);
          if ((relation != utc_equalTo) && (relation != utc_indeterminate)) {
           count_error++;
             print_general_3("       Test KEY CERT INFO failed for certexp %s\n", q);
             print_general_3("        ... Expected Cert End  time = %s\n",
                            print_ascii_time(&expected_certexp));
             print_general_3("        ... Cert End time = %s\n",
                            print_ascii_time(&certexp));
               break;
          }
         if (is_crl_valid) {
            status = utc_cmpmidtime(&relation, &crllastseen, &expected_crllastseen);
            if ((relation != utc_equalTo) && (relation != utc_indeterminate)) {
               count_error++;
               print_general_3("       Test KEY CERT INFO failed for crllastseen %s\n", q);
               print_general_3("        ... Expected CRL last seen time = %s\n",
                            print_ascii_time(&expected_crllastseen));
               print_general_3("        ... CRL last seen time = %s\n",
                            print_ascii_time(&crllastseen));
               break;
            }
            status = utc_cmpmidtime(&relation, &nextcrlexpected, &expected_nextcrlexpected);
            if ((relation != utc_equalTo) && (relation != utc_indeterminate)) {
               count_error++;
               print_general_3("       Test KEY CERT INFO failed for next crl expected %s\n", q);
               print_general_3("        ... Expected Next crl expected = %s\n",
                            print_ascii_time(&expected_nextcrlexpected));
               print_general_3("        ... Next crl expected = %s\n",
                            print_ascii_time(&nextcrlexpected));
               break;
            }
          }
        }


        print_general_1("        Test KEY CERT INFO  succeded \n");
        break;


     case TEST_KEY_CERT_ALL_COUNT_FLAG:

        sp_flag = 0;
        print_general_1 ("    Test KEY_CERT_ALL_COUNT...\n");
        print_general_1 ("    Doing a DUMP of all CertPaths...\n");
        status = pkc_get_key_count (keyinfohandle, &no_of_keys);
        if (status != 0) {
           print_general_2("        pkc_get_key_count failed for testtoken %ld\n", status);
           count_error++;
           break;
        }

        print_general_1 ("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        print_general_2("        pkc_get_key_count returned keycount= %ld\n", no_of_keys);
        if (no_of_keys == 0) break;

        for (i = 0; i < no_of_keys; i++) {
          print_general_1 ("oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
          status = pkc_get_key_trust_info (keyinfohandle, i,
                       &cert_flags,
                       &domain,
                       &usages);
          if (status != 0) {
            print_general_2("        pkc_get_key_trust_info failed for testtoken %ld\n", status);
            print_general_2("         key index = %ld\n", i);
            sp_flag++;
            break;
          }

          if (cert_flags.missing_crls == 0) {
             print_general_1 ("       cert_flags.Missing CRL is FALSE\n");
          } else {
             print_general_1 ("       cert_flags.Missing CRL is TRUE\n");
          }

          if (cert_flags.revoked == 0) {
             print_general_1 ("       cert_flags.Revoked Flag is FALSE\n");
          } else {
             print_general_1 ("       cert_flags.Revoked Flag is TRUE\n");
          }

          if (cert_flags.trust_type == UNTRUSTED) {
             print_general_1 ("       cert_flags.trust_type is UNTRUSTED\n");
          } else {
            if (cert_flags.trust_type == DIRECT_TRUST) {
              print_general_1 ("       cert_flags.trust_type  is DIRECT_TRUST\n");
            } else {
              if (cert_flags.trust_type == CERTIFIED_TRUST) {
                print_general_1 ("       cert_flags.trust_type is CERTIFIED_TRUST\n");
              } else {
                 print_general_1 ("       cert_flags.trust_type  is BOGUS\n");
                 sp_flag++;
                 break;
              }
            }
          }
             
 

          status = pkc_get_key_certifier_count(keyinfohandle,
                                 i, (size_t *) &ca_count);
          if (status != 0) {
            print_general_2("        pkc_get_key_certifier_count failed for testtoken %ld\n", status);
            print_general_2("         key index = %ld\n", i);
            sp_flag++;
            break;
          }

          print_general_2 ("    Printing CA PATH for keyindex %ld\n",i);

          for (j = 0; j < ca_count; j++) {
            print_general_1 ("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
            status = pkc_get_key_certifier_info(keyinfohandle,i,
                        j, &certfname,
                        &certstart,&certexp,&is_crl_valid,&crllastseen,&nextcrlexpected);
            if (status != 0) {
              print_general_2("        pkc_get_key_certifier_count failed for testtoken %ld\n", status);
              print_general_2("         key index = %ld\n", i);
              sp_flag++;
              break;
            }
            print_general_2("          Key index = %ld", i);
            print_general_2("          Cert index = %ld", j);
            print_general_3("            Certfname = %s\n",certfname);
            print_general_3("            Cert Start Date = %s\n",
                       print_ascii_time(&certstart));
            print_general_3("            Cert Exp Date = %s\n",
                       print_ascii_time(&certexp));
            if (is_crl_valid) {
              print_general_1 ("        Crl is Valid \n");
              print_general_3("         CRL last seen = %s\n",
                         print_ascii_time(&crllastseen));
              print_general_3("         CRL next expected = %s\n",
                         print_ascii_time(&nextcrlexpected));
            } else {
              print_general_1 ("        Crl is NOT Valid \n");
            }
              
          }

          if (sp_flag) break;
        }

        if (sp_flag) count_error++;

        break;


     default:
       print_general_3("        Problem parsing token %s, Unexpected testcase token\n",q);
    free(buf);
       return -1;
    }  /* switch testcase */
     

  }  /* !done */


  status = pkc_free_trustlist (&tr_list);
  if (status != 0) {
   print_general_2("init trustbase failed %ld\n",status);
    free(buf);
   return -1;  /* try next retrieve key */
  }

  status = pkc_free_keyinfo (&keyinfohandle);
  if (status != 0) {
   print_general_2("free keyinfo failed %ld\n",status);
    free(buf);
   return -1;  /* try next retrieve key */
  }

  status = pkc_free_trustbase(&tbase);
  if (status != 0) {
   print_general_2("init trustbase failed %ld\n",status);
    free(buf);
   return -1;  /* try next retrieve key */
  }

 free(buf);
 if (count_error != 0) {
    print_general_2(" =======  A total of %ld failures \n",count_error);
    print_general_3("                in  testcase %s  \n",testfname);
    return -1;
 }
 return 0;

}   



/*
 * create_princ_attr_entries_in_cds
 */

unsigned32 create_princ_attr_entries_in_cds(void)
{
  FILE *fp;
  char filename[BUFSIZE], value[BUFSIZE], 
       line1[BUFSIZE], line[BUFSIZE], act_name[BUFSIZE];
  int flag, x500_flag;
  size_t len;

 strcpy((char *) &filename[0], TEST_DIR);
 strcat((char *) &filename[0], "/");
 strcat((char *) &filename[0], (char *) cds_princ_attr_file_name);

  fp = fopen( (char *) &filename[0] , "r");
  if (!fp) {
    print_general_3("problem opening file %s\n",filename);
    return -1;
  }

  while (fgets(line1, BUFSIZE, fp) != 0) {

    strip_end_of_line (&line1[0]);

    if (line_is_empty(line1))
      continue;

    flag = get_loc_and_princ_attr_value (&line1[0], &line[0], &value[0]);
    if (flag != 0)
      return -1;

    if (parse_token(&line[0], x500_flag, act_name) != 0)
      return -1;

    len = strlen (value)+1;
    switch (x500_flag) {
      case X500_FLAG:

        flag = add_command(&act_name[0], PRINCIPAL_DIRECTORY_OID, &value[0], 0);  /* x500 API to create entry */
        if (flag == 0) {
          print_general_3("Added entry %s using XDS \n", act_name);
        }
        else {
          print_general_3("Error adding entry %s using XDS",act_name);
          print_general_2(" error code = %ld\n",  flag);
          fclose(fp);
          return -1;
        }
        break;

      case CDS_FLAG:

        flag = create_attr_value (&act_name[0], PRINCIPAL_DIRECTORY_OID, (unsigned char *) &value[0],len);  
        if ( flag == CDS_SUCCESS) {
          print_general_3("Added entry %s in cds \n", line);
        }
        else {
          print_general_3("Error adding entry %s in cds", line);
          print_general_2(" error code = %ld\n",  flag);
          fclose(fp);
          return -1;
        }
        break;
      default:
        print_general_1("Error xds flag returned from parse_token\n");
        return -1;
    } /* end of SWITCH */
  }  /* end of while */

  return 0;
}  /* create_entries_in_cds */
  
/*
 * create_entries_in_cds
 */

unsigned32 create_entries_in_cds(void)
{
  FILE *fp;
  char filename[BUFSIZE], line[BUFSIZE], act_name[BUFSIZE];
  int flag, x500_flag;

 strcpy((char *) &filename[0], TEST_DIR);
 strcat((char *) &filename[0], "/");
 strcat((char *) &filename[0], (char *) cds_create_file_name);

  fp = fopen( (char *) &filename[0] , "r");
  if (!fp) {
    print_general_3("problem opening file %s\n",filename);
    return -1;
  }

  while (fgets(line, BUFSIZE, fp) != 0) {

    strip_end_of_line (&line[0]);
    if (parse_token(&line[0], x500_flag, act_name) != 0)
      return -1;

    switch (x500_flag) {
      case X500_FLAG:

        flag = create_command(&act_name[0]);  /* x500 API to create entry */
        if (flag == 0) {
          print_general_3("Added entry %s using XDS \n", act_name);
        }
        else {
          print_general_3("Error adding entry %s using XDS", act_name);
          print_general_2(" error code = %ld\n",  flag);
          fclose(fp);
          return -1;
        }
        
        break;

      case CDS_FLAG:
        flag = create_dir (&act_name[0]);  /* CDS API to create entry */
        if ( (flag == CDS_SUCCESS) || (flag == CDS_ENTRYEXISTS)) {
          print_general_3("Added entry %s in cds \n", line);
        }
        else {
          print_general_3("Error adding entry %s in cds", line);
          print_general_2(" error code = %ld\n",  flag);
          fclose(fp);
          return -1;
        }
        break;
      default:
        print_general_1("Error xds flag returned from parse_token\n");
        return -1;
    } /* end of SWITCH */
  }  /* end of while */

  return 0;
}  /* create_entries_in_cds */
      
/*
 * delete_entries_in_cds
 *
 * deletes entries in xds/cds during cleanup
 */

unsigned32 delete_entries_in_cds(void)
{
  FILE *fp;
  char filename[BUFSIZE], line[BUFSIZE], act_name[BUFSIZE];
  int flag, x500_flag;
  int error_count = 0;

 strcpy((char *) &filename[0], TEST_DIR);
 strcat((char *) &filename[0], "/");
 strcat((char *) &filename[0], (char *) cds_delete_file_name);

  fp = fopen( (char *) &filename[0] , "r");
  if (!fp) {
    print_general_3("problem opening file %s\n",filename);
    return -1;
  }

  while (fgets(line, BUFSIZE, fp) != 0) {

    strip_end_of_line (&line[0]);
    if (parse_token(&line[0], x500_flag, act_name) != 0) {
      fclose (fp);
      return -1;
    }

    switch (x500_flag) {
      case X500_FLAG:

        flag = delete_command(&act_name[0]);  /* x500 API to create entry */
        if (flag == 0) {
          print_general_3("Deleted entry %s using XDS \n", act_name);
        }
        else {
          print_general_3("Error deleting entry %s using XDS",act_name);
          print_general_2(" error code = %ld\n",  flag);
          error_count++;
        }
        break;

      case CDS_FLAG:

        flag = delete_dir (&act_name[0]);  /* CDS API to create entry */
        if ( (flag == CDS_SUCCESS) || (flag == CDS_ENTRYEXISTS)) {
          print_general_3("deleting entry %s in cds \n", line);
        }
        else {
          print_general_3("Error deleting entry %s in cds", line);
          print_general_2(" error code = %ld\n",  flag);
          error_count++;
        }
        break;
      default:
        print_general_1("Error xds flag returned from parse_token\n");
        return -1;
    } /* end of SWITCH */
  }  /* end of while */

  if (error_count) return -1;
  return 0;
}  /* delete_entries_in_cds */


int execute_test_case(FILE *fp, char *filename)
{
  char line[BUFSIZE];
  unsigned32 status;
  int flag;
  char *p;
  int cline;
  char *tp;

  /* first, read the dummy line.. This line points to a file that contains
   * directories to be created, if xds_cds_env is set while running the tests.
   * Otherwise, it is a dummy line which is ignored.
   */

  if (fgets(line, BUFSIZE, fp) == 0) {
      display_error(" Execute_Test_Case ",  (unsigned32) errno);
      return -1;
  }

  strip_end_of_line (&line[0]);
  flag = get_cds_init_fnames(&line[0], 
                             &cds_config_file_name[0],
                             &cds_create_file_name[0],
                             &cds_delete_file_name[0], 
                             &cds_princ_attr_file_name[0]
                             );
  if (flag != 0) {
     print_general_3(" ** ERROR  parsing line %s for config delete .. filenames \n",line);
    return  -1;
  }
  print_general_1("=================================================================\n");
  print_general_1("Checking for Validity of Configuration Before running test component\n");
  if (check_configuration (cds_config_file_name) != 0) {
    print_general_1("DCE or X500 Configuration is not a Valid configuration for running test\n");
    print_general_1("** WARNING ** Returning Success so other test-suites in security will\n"); 
    print_general_1("not fail because of this problem.\n");
    print_general_1("=================================================================\n");
    return 0;
  } else {
    print_general_1("DCE or X500 Configuration is a Valid configuration for running test\n");
  }

  print_general_1("=================================================================\n");
  print_general_3("==================  Cleaning up Test Entries in Namespace in %s==============\n",filename);
  print_general_1("                    Ignore ANY errors Here...\n\n");
  print_general_1("================================================================\n\n");

  status = delete_entries_in_cds();

  print_general_1("\n\n****************************************************************\n");
  print_general_3("******************  TestCase = %s                 ***************\n\n",filename);
  print_general_1("=================================================================\n");
  print_general_3("==================  Creating Entries in Namespace in %s==============\n",filename);
  print_general_1("================================================================\n\n");

  status = create_entries_in_cds();
  if (status != 0) {
    print_general_1("Problem creating cds entries for principals\n");
      display_error(" Export_Certificates", status);
      goto endit;

  }

  print_general_1("=================================================================\n");
  print_general_3("==================  Creating Principal Dir Attrs in %s==============\n",filename);
  print_general_1("================================================================\n\n");

  status = create_princ_attr_entries_in_cds();
  if (status != 0) {
    print_general_1("Problem creating cds princ attr entries for principals\n");
      display_error(" Export_Certificates", status);
      goto endit;
  }
  
  print_general_1("=================================================================\n");
  print_general_3("==================  Exporting Certificates in %s ==============\n",filename);
  print_general_1("================================================================\n\n");

  while (fgets(line, BUFSIZE, fp) != 0) {

    if (line[0] == '#') { /* reached end of lines asking to export certificates */ 
      break;
    }
   
    strip_end_of_line (&line[0]);

    /* Now, export each certificate in the appropriate entry in XDS */
  

    print_general_3("    ---> Exporting Certificate specified in line%s\n", line);
    status = export_certificates(line);
    if (status != 0) {
      display_error(" Export_Certificates", status);
      goto endit;
    }
  } 

  /* now, for each of the remaining lines , run the hierarchichal retrieval policy
   * rule to obtain public keys.
   */

  print_general_1("=================================================================\n");
  print_general_3("==================  Trying Individual Testcases in %s ========\n",filename);
  print_general_1("================================================================\n\n");

  tp = 0;
  cline = 1;
  while (fgets(line, BUFSIZE, fp) != 0) {

    strip_end_of_line (&line[0]);

    if (line[0] == '#') { /* go to next testcase */ 
      if (!tp) {
        print_general_1("No Testcase on this line.. Continuing to the next testcase line\n");
        continue;
      }
 
      status = test_retrieve_key(tp, cline, filename);
      if (status != 0) {
        display_error(" Retrieve_key", status);
        goto endit;
      }
      free (tp);
      tp = 0; 
      cline++;
      continue;
    }

    if (tp) { /* already allocated once, so realloc */
      tp = (char *) realloc (tp, strlen(tp)+strlen(line)+2);
      if (!tp) {
        print_general_1("execute_test_case: ran out of memory\n");
        return  -1;
      }
      strcat (tp," ");
      strcat (tp,line);
    } else {
      tp = (char *) malloc (strlen(line)+1);
      if (!tp) {
        print_general_1("execute_test_case: ran out of memory\n");
        return  -1;
      }
      strcpy (tp, line);
    }  
    

  }

endit:


  /* delete the directory entries created, if any */

  { unsigned32 status1;
  status1 = delete_entries_in_cds();
  if (status != 0) {
    print_general_1("Problem creating cds entries for principals\n");
    return -1;
  }
  }

  if (status != 0) return -1;
  return 0;

}
    

#ifndef ENVIRONMENT_TET
int main (int argc, char **argv)
#else
int loc_main (int argc, char **argv)
#endif

{
 int i;
 unsigned32 status;
 unsigned char filename[MAX_FILE_LEN], buf[MAX_FILE_LEN];
 FILE *fp;
 int exec_flag = 0, howmany_failed = 0;
 char failed_tests [BUFSIZE];
 char succeded_tests [BUFSIZE];
 char total_tests [BUFSIZE];
 int total_input_test_files = 0;
 
 failed_tests[0] = '\0';
 succeded_tests[0] = '\0';
 total_tests[0] = '\0';
 if (read_principals() != 0) {
   print_general_1("Problem reading principals \n");
   return -1;
 }
   

 if (argc > 1) {
   for (i = 1; i < argc; i++) {
     total_input_test_files++;

     strcpy((char *) &filename[0], TEST_DIR);
     strcat((char *) &filename[0], (char *) "/");
     strcat((char *) &filename[0], (char *) argv[i]);

     strcat (total_tests, " ");
     strcat (total_tests, argv[i]);

     fp = 0;
     fp = fopen((char *) &filename[0], "r");
     if (!fp) {
       print_general_3("**** ERROR in Input -- Testcase File %s does not exist\n", 
                 (const char *) &filename[0]);
       print_general_1("     Continuing to the next one\n"); 
       strcat (failed_tests, " ");
       strcat (failed_tests, argv[i]);
       howmany_failed++;
       continue;
     }

     exec_flag = execute_test_case(fp, (char *) filename);
     if (exec_flag != 0) {
       strcat (failed_tests, " ");
       strcat (failed_tests, argv[i]);
       howmany_failed++;
     } else {
       strcat (succeded_tests, " ");
       strcat (succeded_tests, argv[i]);
     } 
     fclose(fp);
   }
 } else {
   i  = 1;
   while (1) {   /* while there are files in the testcases directory */
     strcpy((char *) &filename[0], TEST_DIR);
     sprintf((char *) &buf[0], "/tc%d", i);
     strcat((char *) &filename[0], (char *) &buf[0]);
  

     fp = 0;
     fp = fopen((char *) &filename[0], "r");
     if (!fp) {
       break;  /* done */
     }
  
     strcat (total_tests, " ");
     strcat (total_tests, (char *) filename);
     total_input_test_files++;

     exec_flag = execute_test_case(fp, (char *) filename);
     if (exec_flag != 0) {
       strcat (failed_tests, " ");
       strcat (failed_tests, argv[i]);
       howmany_failed++;
     } else {
       strcat (succeded_tests, " ");
       strcat (succeded_tests, argv[i]);
     } 
     fclose(fp);
     i++;
   }
  }

 if (total_input_test_files == 0) {
   print_general_1(" ******** Trycase : NO INPUT TEST FILES, Returning error\n");
   return -1;
 }

 print_general_3(" ******** Executive Trycase summary for tests = %s  *****************\n",total_tests);

 if (succeded_tests[0] == '\0')
   print_general_1 ("     All tests failed\n");
 else
   print_general_3("        Succeded %s \n", succeded_tests);
 
 if (failed_tests[0] == '\0')
   print_general_1 ("     All tests Ran succesfully\n");
 else
   print_general_3("        Failed %s \n", failed_tests);
 if (howmany_failed != 0)
   return -1; 

 return 0;
}

#ifdef ENVIRONMENT_TET

static void tp(void) {

    char *argv[128];
    int argc;
    char * progname = "trycase_a";
    char arg_buffer[256];
    char * arglist = NULL;
    char * cp1;
    char * cp2;
    int result;
    
    tet_infoline("Retrieving test-driver argument-list");
    arglist = getenv(HIERARCHICAL_TEST_ARGUMENTS);

    argc = 1;
    argv[0] = progname;

    if (arglist != NULL) {
	tet_infoline("Parsing argument-list");
	strncpy(arg_buffer, arglist, sizeof(arg_buffer));
	arg_buffer[sizeof(arg_buffer)-1] = 0;    
	cp1 = arg_buffer;
		
	while (cp1[0] != 0) {
/* Advance cp1 over leading spaces...					    */
	    while (cp1[0] == ' ') cp1++;
	    if (cp1[0] == 0) continue;
/* Now cp1 points to the start of the next argument...			    */
 	    cp2 = cp1;
/* Advance cp2 over the argument...					    */
	    while (cp2[0] != ' ' && cp2[0] != 0) cp2++;
/* Now cp1 points to the first character of the argument, cp2 points to one */
/* past the end.							    */
	    argv[argc++] = cp1;
	    if (cp2[0] == 0) {
		cp1 = cp2;
	    } else {
		cp1 = cp2+1;
		cp2[0] = 0;
	    };
	};
    } else {
	tet_infoline("No arguments found");
	tet_infoline("Running for testcases tc1,tc2,tc3, etc");
    };

    tet_infoline("Invoking main test program");

    result = loc_main(argc, argv);    

    tet_infoline("Returned from main test program");

    if (result) {
	tet_infoline("Test failed");
	tet_result(TET_FAIL);
    } else {
	tet_infoline("Tests passed");
	tet_result(TET_PASS);
    };
    return;    
}
#endif

