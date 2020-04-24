/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.cds.3_parse_cal_spec.c,v $
 * Revision 1.1.8.2  1996/02/17  23:27:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:00  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:42:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:20:01  root]
 * 
 * Revision 1.1.6.1  1994/06/10  20:50:11  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:42  devsrc]
 * 
 * Revision 1.1.4.1  1993/10/05  19:43:50  cmckeen
 * 	Put in fix for hp-ux that simplifies an assignment in rpccds3_parse_cal_spec:
 * 	Old statement:
 * 	            ((calendars_p->cal_data)->cal_lines)[i] =
 * 	                               (idl_char *)cal_lines_param_p[i++].param_val);
 * 	New statements:
 * 	            NewLine = (idl_char *)cal_lines_param_p[i].param_val;
 * 	            ((calendars_p->cal_data)->cal_lines)[i] = NewLine;
 * 
 * 	where NewLine is defined as:
 * 	   idl_char *NewLine;
 * 	[1993/10/05  19:08:00  cmckeen]
 * 
 * Revision 1.1.2.3  1993/03/12  18:56:10  mhickey
 * 	Fixing bsubmit mystery meat
 * 	[1993/03/12  18:51:32  mhickey]
 * 
 * Revision 1.1.1.2  1992/12/22  04:48:03  mhickey
 * 	Initial checkin of rpc.cds.3.
 * 
 * Revision 1.1.1.2  1992/12/03  17:49:06  mhickey
 * 	Final check-in before initial submit.
 * 
 * 	Added automatic id refreshes via timer thread that refreshes
 * 	the id one minute before it is to expire.  Added a timedwait
 * 	to the rpcsec2_cnf_str routine to make the operations take
 * 	longer so that request buffering will take place.  Added
 * 	-l option to allow setting of protection level.
 * 
 * 	Defect 5887.
 * 
 * Revision 1.1.1.4  1992/10/16  13:33:07  mhickey
 * 	 Added routine tstrpc_server_check_auth() to extract the auth
 * 	 info from the client's binding handle, and verify it against
 * 	 what we expect.  Verification is done by server name, authz
 * 	 service, authn service, protection level and client name.
 * 	 Added calls to this routine to the two entry point routines.
 * 	 Made all printf's in the routines that are executed as a
 * 	 result of client calls conditional on debug_sw to prevent
 * 	 a lot of useless noise during normal operation.
 * 
 * 	 We have authenticated RPC!!
 * 
 * Revision 1.1.1.3  1992/10/10  03:14:38  mhickey
 * 	Added tstrpc_cnf_str function and debug command line option.
 * 
 * 	Note that un/exporting and replacing obj UUIDS is still messed.
 * 
 * Revision 1.1.1.2  1992/10/06  17:30:31  mhickey
 * 	Initial checkin for base for rpc.security.2
 * 
 * $EndLog$
 */

/*---*
 *---* rpc.cds.3_parse_cal_spec.c - parses a string of calendar specs into
 *---*                              an array of calendar structs.
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the rpc.cds.3 DCE System Test
 *---*
 *---*    The rpc.cds.3 system test corresponds to test rpc.cds.3 in the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the rpc.cds.3 system test, see the test 
 *---*    plan, or the README file in this directory.
 *---* 
 *---* FUNCTION
 *---*    Reads the calendar data files specified by cal_spec from the 
 *---*    directory specified by cal_d_dir, and returns with cal_vec_p and
 *---*    ncals set to reflect the data in the calendars and the number
 *---*    of calendars, respectively.
 *---* 
 *---* CALLED BY
 *---*    rpc.cds.3_cmain.c and rpc.cds.3_smain.c
 *---* 
 *---* CALLS INTO 
 *---*    chk_stat.c  (rpcsec2_chk_stat() )
 *---*    rpc.cds.3_defs.h      (return values and data structures)
 *---*
 *---* PREREQUISITES
 *---*    Assumes a running DCE cell on the system that has been setup with 
 *---*    the script rpc.cds.3_setup.sh.
 *---*/
/*-------------------------------------------*
 *---*  Includes
 *-------------------------------------------*/
/*---*
 *---* Global include files
 *---*/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

/*---*
 *---* Private include files
 *---*/
#include <rpc_cds_3.h>
#include <rpc.cds.3_defs.h>

/*---* 
 *---* a nil uuid for initializing
 *---*/
uuid_t nil_uuid = { 0 };


/*---* parameter list for required calendar parameters *---*/
param_spec_val_t cal_params[] = {
                                  {"RPCCDS3_CAL_NAME", 0},
                                  {"RPCCDS3_CAL_LINES", 0}

/*---* enum for obvious indexing *---*/			        };
enum {
       cal_nm,
       cal_nln
     } cal_param_i;


#define ERROR_EXIT(stat) { \
                         *cal_vec_p = (cal_entry_p_t)NULL; \
                         *ncals = 0; \
                         *status = stat; \
                         return; \
                       }

void rpccds3_parse_cal_spec(char *cal_d_dir, char *cal_spec, 
                            cal_entry_p_t *cal_vec_p, int *ncals, 
                            int debug_sw, int *status)
{
   
   char *cp, *cp1, *cp2,
       **dfiles, **dfiles_p,
        *cal_full_name;
   cal_entry_p_t calendars, calendars_p;
   int i, maxlines, nlines, stat;
   param_spec_val_p_t cal_lines_param_p;
   extern char *rd_conf_errs[];
   idl_char *NewLine;
   

   /*---------------------------------------------------*
    *---* Set up the calendars array;
    *---*     this means:
    *---*         1) parse the list of calendar data files, loading an array
    *---*            of pointers to the names.
    *---*         2) For each file, set up an arra of param_val stucts to get
    *---*            the calendar name and number of lines.
    *---*         3) use the information from step 2 to allocate a new calendar
    *---*            struct of the proper length.
    *---*         4) set up an array of param val_structs to get the rest of 
    *---*            the data from the data file.
    *---*         5) point the calendar struct data area at the returned data
    *---*         6) free the unused parts of the param array.
    *---*
    *---* 1) parse the list of file names
    *---*
    *---*/
    if (!cal_spec)
    {
       if (debug_sw)
          printf("    no calendar spec passed\n");
       ERROR_EXIT(rpccds3_parse_cal_spec_no_spec)
    }
    else
       cp1 = cal_spec;

    *ncals=0;

    /*---* strip spaces *---*/
    for (cp = cp1; isspace(*cp1); cp1++);
    /*---* now go through the string, counting files *---*/
    for (cp2 = cp = cp1; *cp1; cp1++)       
    {
       if (isspace(*cp1) && cp1 > cp2)
       {
          (*ncals)++;
          for (cp2 = cp1; *cp1 && isspace(*cp1); cp1++);

          if (! *cp1) 
             cp1--;
          else 
             cp2 = cp1;
       }
    }
    if ((!isspace(*cp2)) && cp1 > cp2)
       (*ncals)++;

    if (debug_sw)
       printf("    found %d calendar file names...\n", *ncals);
 
    if (! (dfiles = (char **)malloc(*ncals * sizeof(char *))))
    {
       if (debug_sw)
          printf("    can't get space for calendar file name pointers\n");
       ERROR_EXIT(rpccds3_parse_cal_spec_no_space)
    }

    /*---* 
     *---* get space for the buffer to hold the concatenated calendar file 
     *---* directory name and calendar name 
     *---*/
    if (! (cal_full_name = (char *)malloc(strlen(cal_d_dir) + 50)))
    {
       if (debug_sw)
          printf("    can't get space for calendar full name\n");
       ERROR_EXIT(rpccds3_parse_cal_spec_no_space)
    }
    
    for (cp1 = cp2 = cp, dfiles_p = dfiles, 
         printf("    processing calendar files %s...\n", cp1),
         *dfiles_p++ = cp1; *cp1; cp1++) 
    {
       if (isspace(*cp1))
       { 
          /*---* null terminate the name we just ran through *---*/
          *cp1++ = '\0';
          /*---* skip intervening whitespace to find start of next name *---*/
          while(*cp1 && isspace(*cp1)) 
             cp1++;
          /*---* 
           *---* if we hit the end of the list, backup one to let loop exit via
           *---* the loop condition.
           *---*/
          if (! *cp1) 
             cp1--;

          if (debug_sw)
             printf("    got calendar file %s...\n", *(dfiles_p-1));

          if (*(cp1+1)) 
             *dfiles_p++ = cp1;

       }
    }

    if (debug_sw && cp1 - *dfiles == strlen(*dfiles))
       printf("    got calendar file %s...\n", *dfiles_p);

    /*---* 
     *---* malloc the pointers to the calendar structs 
     *---*/
    if (! (calendars=calendars_p=
              (cal_entry_p_t)malloc(sizeof(cal_entry_t) * *ncals)))
    {
       if (debug_sw)
          printf("   can't get space for calendar vector pointers...\n");
       ERROR_EXIT(rpccds3_parse_cal_spec_no_space)
    }

    if (debug_sw)
       printf("    reading names and sizes of calendars...\n");

    for (dfiles_p=dfiles, maxlines=0; dfiles_p-dfiles < *ncals; dfiles_p++,
                                                              calendars_p++)
    {
       /*---*
        *---* now call rd_conf to get the number of lines and the name
        *---* for each calendar
        *---*/
       if (cal_d_dir)
          sprintf(cal_full_name, "%s/%s", cal_d_dir, *dfiles_p);
       else
         sprintf(cal_full_name, "%s", *dfiles_p);

       if (debug_sw)
          printf("    reading name and size from calendar file %s...",
                                                              cal_full_name);
       rd_conf(cal_full_name, cal_params, 
                        sizeof(cal_params)/sizeof(param_spec_val_t), &stat);

       if (stat != rdconf_ok)
       {
          if (debug_sw)
             /*---* report error *---*/
             printf("\n\n    %s (%s).\n", rd_conf_errs[stat],
                                                              cal_full_name);
          ERROR_EXIT(rpccds3_parse_cal_spec_bad_file)
       }
       /*---* report success *---*/
       printf("done.\n\n");
   
       sscanf(PVAL(cal_params, cal_param_i,  cal_nln), "%d", &nlines);
       maxlines= maxlines<nlines? nlines: maxlines;

       /*---* 
        *---* now that we know what we will be reading, get space for a 
        *---* calendar struct with the right number of lines.
        *---*/
       if (! (calendars_p->cal_data = 
      (calndr_data_p_t)malloc(sizeof(calndr_data_t)+ (sizeof(char *) * (nlines-1)))))
       {
          if (debug_sw)
             printf("    no space for calendar data structure (%s)\n",
                                                               cal_full_name);
          ERROR_EXIT(rpccds3_parse_cal_spec_no_space)
       }
      
       /*---* load the name and size into the new struct *---*/
       (calendars_p->cal_data)->cal_name = 
                            (idl_char *)PVAL(cal_params,cal_param_i, cal_nm);
       (calendars_p->cal_data)->cal_nlines = nlines; 
       /*---* be paraniod. null the uuid *---*/
       calendars_p->cal_uuid = nil_uuid;
       free(PVAL(cal_params, cal_param_i, cal_nln));

       if (debug_sw)
          printf(
             "    created data structure for calendar %s (%d lines)...\n",
                           (calendars_p->cal_data)->cal_name, 
                           (calendars_p->cal_data)->cal_nlines);
    }

    /*---* 
     *---* now that we have all the calendar structs allocated, step through
     *---* the structs, reading all the lines for each file, pointing the 
     *---* pointers in the calendar structs at the returned lines, and
     *---* allocating a uuid for the calendar.
     *---*/
    /*---* first we alloc space for the param structs *---*/
    if (debug_sw)
        printf("    trying to get space for line params array..."); 
    if (! (cal_lines_param_p = 
           (param_spec_val_p_t)malloc(maxlines*sizeof(param_spec_val_t))))
    {
       printf("    can't get space for calendar params array\n");
       ERROR_EXIT(rpccds3_parse_cal_spec_no_space)
    }

    if (debug_sw)
    {
       printf("done\n\n"); 
       printf("    trying to get space for the param specs...");
    }

    if (! ((*cal_lines_param_p).param_spec = (char *)malloc(maxlines* (strlen("RPCCDS3_CAL_LINE") + 5))))
    {
       printf("    can't get space for calendar params\n");
       ERROR_EXIT(rpccds3_parse_cal_spec_no_space)
    }
 
    if (debug_sw)
       printf ("done.\n\n");

    /*---* 
     *---* set the param struct ptrs to point into the space we got and 
     *---* create the parameter spec strings
     *---*/ 
    for (i=0; i < maxlines; i++)
    {
       sprintf(cal_lines_param_p[i].param_spec,"%s%d","RPCCDS3_CAL_LINE",
                                                                        i+1);
       if (i+1 != maxlines)
       {
          cal_lines_param_p[i+1].param_spec = cal_lines_param_p[i].param_spec + (strlen(cal_lines_param_p[i].param_spec) + 1);
       }
    }
    if (debug_sw)
    {
       printf("    parameter array for %d lines is:\n", maxlines);
       for (i=0; i< maxlines; 
                       printf("     %s\n",cal_lines_param_p[i++].param_spec));
    } 

    /*---* now get the lines and set pointers in the calendar structs *---*/
    for (calendars_p=calendars, dfiles_p=dfiles;
           calendars_p - calendars < *ncals; calendars_p++, dfiles_p++)
    {
        int locstat=0;

        /*---* to prevent lots of deref-ing... *---*/
        nlines = (calendars_p->cal_data)->cal_nlines;
       
        if (cal_d_dir)
           sprintf(cal_full_name, "%s/%s", cal_d_dir, *dfiles_p);
        else
           sprintf(cal_full_name, "%s", *dfiles_p);
     
        if (debug_sw)
           printf("    reading data for calendar %s from %s...", 
                                        (calendars_p->cal_data)->cal_name, 
                                                               cal_full_name);
        rd_conf(cal_full_name, cal_lines_param_p, nlines, &locstat);

        if (locstat != rdconf_ok)
        {
           /*---* report error *---*/
           printf("    %s (%s).\n", rd_conf_errs[locstat], cal_full_name);
           ERROR_EXIT(rpccds3_parse_cal_spec_bad_data)
        }
        /*---* report success *---*/
        printf("done.\n\n");
    
        for (i=0; i < nlines; i++)
	{  
           NewLine = (idl_char *)cal_lines_param_p[i].param_val;
           ((calendars_p->cal_data)->cal_lines)[i] = NewLine;
        }
          
        if (debug_sw)
        {
           printf("    data for calendar %s is:\n", 
                                      (calendars_p->cal_data)->cal_name);
           printf("        lines: %d\n", 
                                   (calendars_p->cal_data)->cal_nlines);
           for (i=0; i <  nlines;  
            printf("     line %d: %s\n", i, 
                               ((calendars_p->cal_data)->cal_lines)[i++]));
        }
    }
    *cal_vec_p = calendars;
    *status = rpccds3_parse_cal_spec_ok;
    return;

}
