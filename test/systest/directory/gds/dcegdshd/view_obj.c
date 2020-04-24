/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: view_obj.c,v $
 * Revision 1.1.6.2  1996/02/17  23:26:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:33  marty]
 *
 * Revision 1.1.6.1  1995/12/11  21:57:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:57:36  root]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:07:06  cjd]
 * 
 * Revision 1.1.4.1  1994/02/23  21:43:39  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:38:05  pellis]
 * 
 * Revision 1.1.2.3  1993/10/19  15:05:34  keutel
 * 	CR 9127
 * 	[1993/10/18  15:14:34  keutel]
 * 
 * Revision 1.1.2.2  1993/10/14  17:37:02  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:16  cmckeen]
 * 
 * Revision 1.1.2.1  1993/09/20  22:40:44  pellis
 * 	Moved from ./systest/directory/gds/*.
 * 	[1993/09/20  22:18:13  pellis]
 * 
 * Revision 1.1.2.3  1993/04/01  20:17:40  mhickey
 * 	Fixed a few pointer mismatch warnings, and cleaned
 * 	up some prints.
 * 
 * 	defect 7071
 * 	[1993/04/01  20:16:59  mhickey]
 * 
 * Revision 1.1.2.2  1993/03/31  21:57:18  mhickey
 * 	Initial checkin for defect 7071.
 * 	[1993/03/31  21:52:50  mhickey]
 * 
 * Revision 1.1.4.3  1993/02/05  15:20:18  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:40:25  cjd]
 * 
 * Revision 1.1.4.2  1992/12/09  20:00:32  mhickey
 * 	Removed the strchr declaration from the variable declarations
 * 	in the program.  This was breaking thhe build with a "redeclaration of
 * 	..." error.
 * 
 * 	Defect 6366.
 * 	[1992/12/09  19:53:38  mhickey]
 * 
 * Revision 1.1.2.4  1992/07/24  20:07:13  mhickey
 * 	Ported to latest GDS, changed usage, enabled binding to remote DSA,
 * 	added arg checking, cleanded up some (not all) warnings,
 * 	added -v (verbose) option, etc.
 * 
 * 	Defect 4817
 * 	[1992/07/24  20:04:21  mhickey]
 * 
 * Revision 1.1.2.3  1992/06/05  21:52:24  gmd
 * 	Added workspace to ds_bind(), ds_version() and ds_shutdown() calls.
 * 	[1992/06/05  21:52:01  gmd]
 * 
 * Revision 1.1.2.2  1992/03/18  01:22:45  akers
 * 	Initial code changes for Directory system test.
 * 	[1992/03/05  22:40:00  akers]
 * 
 * Revision 1.1  1992/01/19  14:35:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   	FILE_NAME: view_obj.c
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: process_command_line_arguments()
 *		   show_usage()
 *		   fill_in_structures()
 *		   fill_in_strings()
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	FOR_TEST: dcegdacl
 *
 *   	USAGE:
 *
 *		view_obj [-v] -D <DSA-spec> -N <Object-spec> -p <Phone-number> 
 *
 *		where:
 *		    -v specifies to operate in verbose mode
 *		    DSA-spec is the dsa name in the form 
 *                        /<country>/<org>/<org_unit>/dsa/<name>
 *		    Object-spec is the object(person) name in the form
 *                        /<country>/<org>/<org_unit>/<name>/<surname>
 *		    Phone-number is the phone number expected in the object
 *		    
 *
 *   	DESCRIPTION:
 *
 *		This program is a utility that will read an object out of the
 *		specified DSA.  Refer to the Application Developers Guide for 
 *		details on how this object reading is accomplished.
 *
 *
 *   	HISTORY:
 *      	05/10/91        Gerald Cantor           Initial design/code
 *
 *              7/22/92         Mark Hickey         - Ported to latest version
 *                                                    of GDS.
 *                                                  - Added arguement checking.
 *                                                  - Added ability to specify
 *                                                    a remote DSA (as long as 
 *                                                    it is known by the cache).
 *                                                  - added verbose mode.
 *                                                  - fixed up comments
 *                                                  - changed usage syntax
 */
#include <stdio.h>
#include <dce/dce.h>
#include "shd_util.h"
#include "structures.h"

extern char buf[128], header[512];
void fill_in_structures(), 
     fill_in_strings(),
     fill_in_integers();

void init_dn();
char *prog,
     *phone;
int  dir_id;

/*---* struct holding pointers to parts of a name *---*/
struct dn_desc {
     char *ctry, 
          *org,
          *org_unit_name,
          *comnam1,
          *comname;
} dsa_dn, obj_dn;


#ifdef SUG
char full_name[13];
#else
char full_name[126];
#endif /*---* SUG *---*/

int vflag;

main(argc, argv)
int argc;
char **argv;
{
	void process_command_line_arguments();
	DS_status error, st;
        OM_return_code return_code;
        OM_workspace workspace;
        OM_private_object session;
        OM_private_object result;
        OM_sint invoke_id;

	/*
         * Variables to extract the telephone number(s)
         */
        OM_type entry_list[] = {DS_ENTRY, 0};
        OM_type attributes_list[] = {DS_ATTRIBUTES, 0};
        OM_type telephone_list[] = {DS_ATTRIBUTE_VALUES, 0};
        OM_public_object entry;
        OM_public_object attributes;
        OM_public_object telephones;
        OM_descriptor *telephone;
        OM_value_position total;
	int rc;

	prog = argv[0];
	/*dsP_trace_init("./trace", "logfile", 2);*/
        make_header(prog);
     
        /*---* initialize all structs to NULL *---*/
        init_dn(&obj_dn);
        init_dn(&dsa_dn);
     
        /*---* parse the command line and load structs *---*/
        process_command_line_arguments(argc, argv, &obj_dn, &dsa_dn, &phone, 
                                                                      &dir_id);

        /*---* load the structures with the values that specify the dsa *---*/
	fill_in_strings(country, 2, DS_ATTRIBUTE_VALUES,
                           OM_S_PRINTABLE_STRING, dsa_dn.ctry);
        fill_in_strings(organization, 2, DS_ATTRIBUTE_VALUES,
                           OM_S_TELETEX_STRING, dsa_dn.org);
        fill_in_strings(organizational_unit, 2, DS_ATTRIBUTE_VALUES,
                           OM_S_TELETEX_STRING, dsa_dn.org_unit_name);
	fill_in_strings(common_name1, 2, DS_ATTRIBUTE_VALUES, 
			   OM_S_TELETEX_STRING, dsa_dn.comnam1);
	fill_in_strings(common_name, 2, DS_ATTRIBUTE_VALUES, 
			   OM_S_TELETEX_STRING, dsa_dn.comname);

	fill_in_structures(rdn1, 1, DS_AVAS, OM_S_OBJECT, country);
        fill_in_structures(rdn2, 1, DS_AVAS, OM_S_OBJECT, organization);
        fill_in_structures(rdn3, 1, DS_AVAS, OM_S_OBJECT, organizational_unit);
        fill_in_structures(rdn4, 1, DS_AVAS, OM_S_OBJECT, common_name1); 
        fill_in_structures(rdn5, 1, DS_AVAS, OM_S_OBJECT, common_name);

        fill_in_structures(dsa_name, 1, DS_RDNS, OM_S_OBJECT, rdn1);
        fill_in_structures(dsa_name, 2, DS_RDNS, OM_S_OBJECT, rdn2);
        fill_in_structures(dsa_name, 3, DS_RDNS, OM_S_OBJECT, rdn3);
        fill_in_structures(dsa_name, 4, DS_RDNS, OM_S_OBJECT, rdn4);
        fill_in_structures(dsa_name, 5, DS_RDNS, OM_S_OBJECT, rdn5);

       
	/*
         * Perform the Directory operations:
         * (1) Initialize the Directory Service and get an OM workspace
         * (2) Load the session object with the dsa name
	 * (2) Bind a directory session with the dsa 
         * (3) Get a local copy of the object
         */
         workspace = ds_initialize();
	if (workspace == NULL)
        {
                sprintf(buf, "%s(%s):  Cannot initialize workspace", header,
                        prog);
                puts(buf);
                exit(1);
        }

        /*---* 
         *---* load the DSX_C_GDS_SESSION object with the dsa name and the
         *---* directory id to access.
         *---*/
        fill_in_structures(dsa_session, 1, DS_DSA_NAME, OM_S_OBJECT, dsa_name);
        fill_in_integers(dsa_session, 2, DSX_DIR_ID, OM_S_INTEGER, dir_id);

	st = ds_version(feature_list, workspace);
	sprintf(buf, "(%s):  Cannot load GDS extensions", prog);
        gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

        if (vflag)
           printf("Attempting bind to DSA %s...", dsa_dn.comname);
        st = ds_bind(dsa_session, workspace, &session);
	sprintf(buf, "(%s):  Cannot bind to %s session", prog, dsa_dn.comname);
        gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);
       
        sprintf(full_name, "%s %s", obj_dn.comnam1, obj_dn.comname);
	fill_in_strings(country, 2, DS_ATTRIBUTE_VALUES,
                           OM_S_PRINTABLE_STRING, obj_dn.ctry);
        fill_in_strings(organization, 2, DS_ATTRIBUTE_VALUES,
                           OM_S_TELETEX_STRING, obj_dn.org);
        fill_in_strings(organizational_unit, 2, DS_ATTRIBUTE_VALUES,
                           OM_S_TELETEX_STRING, obj_dn.org_unit_name);
	fill_in_strings(common_name1, 2, DS_ATTRIBUTE_VALUES, 
			   OM_S_TELETEX_STRING, obj_dn.comnam1);

	fill_in_structures(rdn1, 1, DS_AVAS, OM_S_OBJECT, country);
        fill_in_structures(rdn2, 1, DS_AVAS, OM_S_OBJECT, organization);
        fill_in_structures(rdn3, 1, DS_AVAS, OM_S_OBJECT, organizational_unit);
        fill_in_structures(rdn4, 1, DS_AVAS, OM_S_OBJECT, common_name1);
           
        fill_in_structures(name, 1, DS_RDNS, OM_S_OBJECT, rdn1);
        fill_in_structures(name, 2, DS_RDNS, OM_S_OBJECT, rdn2);
        fill_in_structures(name, 3, DS_RDNS, OM_S_OBJECT, rdn3);
        fill_in_structures(name, 4, DS_RDNS, OM_S_OBJECT, rdn4);
       
	/*
         * Perform the Directory operations:
         * (1) Read the object from the DSA
         * (2) Read the telephone number of "name" if specified.
         * (3) Terminate the directory session.
         */
              
        if (vflag)
           printf("done\n\nAttempting read of object %s...", obj_dn.comnam1);
        st = ds_read(session, DS_DEFAULT_CONTEXT, name, selection, 
                     &result, &invoke_id);
	sprintf(buf, "(%s):  Unable to read entry from tree", prog);
	gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

	if (vflag)
           printf("done.\n\nAttempting to unbind from %s session...",
                                                               dsa_dn.comname);
        st = ds_unbind(session);
        sprintf(buf, "(%s):  Cannot unbind from session", prog);
        gds_check_status(st, DS_SUCCESS, NOT_EQUAL, buf, FALSE);
        if (vflag)
           printf("done.\n");

   	/*
         * Extract the telephone number(s) of "name" from the result.
         *
         * There are 4 stages:
         * (1) Get the Entry-Information from the Read-Result.
         * (2) Get the Attributes from the Entry-Information.
         * (3) Get the list of phone numbers
         * (4) Scan the list and print each number.
         */
   
   	if (vflag)
           printf("Attempting to get local copy of object...");
   	return_code = om_get(result, 
                             OM_EXCLUDE_ALL_BUT_THESE_TYPES
                             + OM_EXCLUDE_SUBOBJECTS, 
                             (OM_type_list)entry_list, 
                             OM_FALSE, 0, 0, &entry, &total);
   	sprintf(buf, "(%s):  om_get on read-result failed", prog);
        gds_check_status(return_code, OM_SUCCESS, NOT_EQUAL, buf, TRUE);
   
   	if (vflag)
           printf("done.\n\nAttempting to get attributes...");
        return_code = om_get(entry->value.object.object, 
                             OM_EXCLUDE_ALL_BUT_THESE_TYPES
   	              + OM_EXCLUDE_SUBOBJECTS, 
                             attributes_list, OM_FALSE, 0, 0, 
   	                     &attributes, &total);
   	sprintf(buf, "(%s):  om_get for attributes failed", prog);
        gds_check_status(return_code, DS_SUCCESS, NOT_EQUAL, buf, TRUE);


        if (vflag)
           printf("done.\n\nAttempting to get telephone number(s)...");
        return_code = om_get(attributes->value.object.object,
                       OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS,
                       telephone_list, OM_FALSE, 0, 0, &telephones, &total);
        sprintf(buf, "(%s):  om_get for attribute values failed", prog);
        gds_check_status(return_code, DS_SUCCESS, NOT_EQUAL, buf, TRUE);

        /*
         * We can now safely release all the private objects
         * and the public objects we no longer need.
         */
	if (vflag)
           printf("done.\n\nAttempting to delete session...");
        return_code = om_delete(session);
	sprintf(buf, "(%s):  Cannot delete object session", prog);
        gds_check_status(return_code, DS_SUCCESS, NOT_EQUAL, buf, FALSE);

	if (vflag)
           printf("done.\n\nAttempting to delete result...");
        return_code = om_delete(result);
	sprintf(buf, "(%s):  Cannot delete object result", prog);
        gds_check_status(return_code, DS_SUCCESS, NOT_EQUAL, buf, FALSE);

	if (vflag)
           printf("done.\n\nAttempting to delete entry...");
        return_code = om_delete(entry);
      	sprintf(buf, "(%s):  Cannot delete object entry", prog);
        gds_check_status(return_code, DS_SUCCESS, NOT_EQUAL, buf, FALSE);

   	if (vflag)
           printf("done.\n\nAttempting to delete attributes...");
        return_code = om_delete(attributes);
	sprintf(buf, "(%s):  Cannot delete object attributes", prog);
        gds_check_status(return_code, DS_SUCCESS, NOT_EQUAL, buf, FALSE);

	if (vflag)
           printf("done.\n\nAttempting to shutdown the workspace...");
        ds_shutdown(workspace);

   	telephone = telephones;
	if (vflag)
           printf("done.\n\nChecking numbers; input=%s, read=%s...",
                            phone, telephone->value.string.elements);
	rc = strcmp(phone, telephone->value.string.elements);

	if (vflag)
           printf("done(%s).\n\nAttempting to delete telephone number(s)...",
                       rc?"differed":"matched");
        return_code = om_delete(telephones);
	sprintf(buf, "(%s):  Cannot delete object attributes", prog);
        gds_check_status(return_code, DS_SUCCESS, NOT_EQUAL, buf, FALSE);
        

        if (vflag)
           printf("done.\n\nReturning %d (%s).\n",rc,rc?"failure":"success");
        exit(rc);
}

void show_usage()
{
        sprintf(buf, "Usage:  %s [OPTIONS]", prog);
        puts(buf);
        puts("Where OPTIONS are:");
        puts("-d <directory_id>        Directory Id used for testing");
        puts("-D <distinguished_name>  Distinguished name of target DSA");
        puts("                         Form: /cntry/org/org_unit/comname/comname");
        puts("-N <distinguished_name>  Distinguished name and surname of person");
        puts("                         Form: /cntry/org/org_unit/comname/surname");
        puts("-p <phone #>             Phone number to check for.");
        puts("-v (optional)            verbose mode; report progress, etc");
        puts("All options except for '-v' are required");
        puts("Exit status of 0 indicates success; ie: all components match.");
}

void process_command_line_arguments(int ac, char **av, 
                                    struct dn_desc *obj_dn_p,
                                    struct dn_desc *dsa_dn_p,
                                    char **phone_pp, int *dir_id_p )
{
        int i, c, err=0, gotdsa=0, gotname=0, gotphone=0;
        char *cp, *cp1, **str_p[5];
        struct dn_desc *dn_p;
        void show_usage();
        extern char *optarg;

       
        while ((c = getopt(ac, av, "vN:D:p:d:hH")) != EOF)
        {
                switch(c)
                {
                        case 'd': /*---* the directory id *---*/
                                  if (isdigit(*optarg))
				  {
                                     sscanf(optarg, "%d", dir_id_p);
                                     if (vflag)
                                        printf("Got directory id as %d...\n", 
                                                                    *dir_id_p);
				  }
                                  else
				  {
                                     printf("Bad value for -d (%s).\n",optarg);
                                     show_usage();
                                     exit(1);
				  }
                                  break;

                        case 'N':
			case 'D': /*---* 
                                   *---* break up the spec and assign values.
                                   *---* if this is a DSA, assign the dsa name,
                                   *---* else assign the surname.
                                   *---*/
                                  /*---* 
                                   *---* set a generic pointer to the right struct
                                   *---*/
                                  dn_p = c == 'N'? obj_dn_p: dsa_dn_p;
                                  if (c == 'N')  
                                     gotname = 1;
                                  else
                                     gotdsa = 1;
                                  /*---* 
                                   *---* set up an array of pointers to the struct
                                   *---* members to ease assignment of values.
                                   *---*/
                                  str_p[0] = &(dn_p->ctry);
                                  str_p[1] = &(dn_p->org);
                                  str_p[2] = &(dn_p->org_unit_name);
                                  str_p[3] = &(dn_p->comnam1);
                                  str_p[4] = &(dn_p->comname);
          
                                  /*---* set pointers to pieces of the name *---*/
                                  cp= optarg;
                                  for (i=0; i<5; i++)
                                  {
                                     if ((cp1=strchr(cp,'/')) == (char *)NULL)
                                     {
                                         err=1;
                                         break;
                                     }
                                     else
                                     {
                                         if (cp1 != cp)
                                            *cp1 = '\0';
                                         *str_p[i] = ++cp1;
                                         cp=cp1;
                                     }
                                  }
            
                                  /*---* if err is set, print usage and exit *---*/
                                  if (err)
	                          {
                                     show_usage();
                                     exit(1);
				  }
                     
                                  /*---* 
                                   *---* an extra check to be sure all pieces of 
                                   *---* name are present.
                                   *---*/
                                  if (dn_p->ctry == (char *)NULL ||
                                      dn_p->org == (char *)NULL ||
                                      dn_p->org_unit_name == (char *)NULL ||
                                      dn_p->comnam1 == (char *)NULL ||
                                      dn_p->comname == (char *)NULL)
				  {
                                     show_usage();
                                     exit(1);
				  }
                                 
                                  if (vflag) 
				  {
                                      if (c == 'D')
				      {
                                          printf("Got DSA dn as /C=%s/O=%s/OU=%s/CN=%s/CN=%s\n",
                                                  dn_p->ctry,dn_p->org,
                                                  dn_p->org_unit_name,
                                                  dn_p->comnam1,dn_p->comname);
                                      }
                                      else
				      {
                                          printf("Got Object dn as /C=%s/O=%s/OU=%s/CN=%s/CN=%s\n",
                                                  dn_p->ctry,dn_p->org,
                                                  dn_p->org_unit_name,
                                                  dn_p->comnam1,dn_p->comname);
                                      }
                                  }

                                  break;

                        case 'p':
                                gotphone=1;
                                *phone_pp = optarg;
                                break;

                        case 'v':
                                vflag++;
                                break;
                        case 'h':
                        case 'H':
                                show_usage();
                                exit(0);
		}
	}

        if (!gotdsa || !gotname || !gotphone)
        {
           show_usage();
           exit(1);
        }
}

void init_dn(dn)
struct dn_desc *dn;
{
     dn->ctry = (char *)NULL;
     dn->org = (char *)NULL;
     dn->org_unit_name = (char *)NULL;
     dn->comnam1 = (char *)NULL;
     dn->comname = (char *)NULL;
}


void fill_in_structures(desc, index, type, syntax, value)
OM_object desc;
int index;
OM_type type;
OM_syntax syntax;
OM_object value;
{
        desc[index].type = type;
        desc[index].syntax = syntax;
        desc[index].value.object.padding = 0;
        desc[index].value.object.object = value;
}

void fill_in_strings(desc, index, type, syntax, value)
OM_object desc;
int index;
OM_type type;
OM_syntax syntax;
char *value;
{
	int length = strlen(value);

        desc[index].type = type;
        desc[index].syntax = syntax;
	desc[index].value.string.length = (OM_string_length)length;
        desc[index].value.string.elements = value;
}

void fill_in_integers(desc, index, type, syntax, value)
OM_object desc;
int index;
OM_type type;
OM_syntax syntax;
int value;
{
        desc[index].type = type;
        desc[index].syntax = syntax;
	desc[index].value.integer = (OM_integer)value;
}
