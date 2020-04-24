/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rdconf.c,v $
 * Revision 1.1.6.2  1996/02/17  23:27:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:52  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:42:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:19:43  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:49:30  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:22  devsrc]
 * 
 * Revision 1.1.2.2  1993/03/12  04:26:13  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:38:37  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* History from before the name change
 *---*
 * Revision 1.1.1.2  1993/01/29  20:33:20  mhickey
 * 	Initial check in for the rpc.cds.3 system test.
 *
 * Revision 1.1.1.2  1992/12/03  18:00:49  mhickey
 * 	Inital check-in.
 * 
 * 	Defect 5887.
 *---*
 *---*/

/*---*
 *---* rdconf.c - Reads specified values from a config file
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the DCE 1.0.2 System Tests
 *---*
 *---*    The DCE 1.0.2 system tests corresponds to the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the DCE 1.0.2 system tests, see the test 
 *---*    plan.
 *---* 
 *---* FUNCTION
 *---*    Takes an array of parameter specs, each containing a variable 
 *---*    name for which a value is to be found in the configuration file,
 *---*    and the file name of the config file.  Opens the config file, and 
 *---*    reads lines of the forms XX=YY and XX="YY ZZ...".  Any lines not in
 *---*    this form are ignored, as are any lines or parts of lines starting
 *---*    with '"'.  If a string assigning a value to one of the specified
 *---*    variables is found, then the prarmeter spec for the variable is
 *---*    filled in with the value.  If all the variables for all of the 
 *---*    parameter specs is not found, then the routine returns an error.
 *---*
 *---* CALLED BY
 *---*    Anybody
 *---* 
 *---* CALLS INTO 
 *---*    'C' library
 *---*
 *---* PREREQUISITES
 *---*    None.
 *---*/

/*---*
 *---* rd_conf.c - read config file and set values
 *---*  
 *---*     This file is part of the rpc.sec.2 system test.
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

/*---*
 *---* Private include files
 *---*/
#include <rdconf.h>

/*---*
 *---* rd_conf(filename, param_val array, nparams, *status)
 *---*
 *---*     Open config file specified by filename, and look for values 
 *---*     for the paramaters specified by the entries in the param_val array.
 *---*     Note that the lines of the file may contain '#' delimited comments,
 *---*     and are expected to be in the form string=string.  Leading and 
 *---*     trailing whitespace is allowed with respect to whole expression; 
 *---*     Whitespace is allowed in the right hand side of the assignment.
 *---*     Double quotes can be used to surround values.
 *---*     The configuration file is also parsed by the shell scripts doing
 *---*     setup.  As a result, there may be lines assigning lists to
 *---*     variables; eg: xx="a b c".  This syntax is not allowed for 
 *---*     lines that will be parsed by this routine as the routine now 
 *---*     functions (though this could be added later if desirable).  At this
 *---*     point, the strings on either side of the '=' sign in any line to be
 *---*     read by this routine must NOT contain whitespace.  If either does,
 *---*     the entire line will be ignored (instead of causing an error return
 *---*     value) on the assumption that the line is used some other part of
 *---*     the test.
 *---*/

void rd_conf(char *fn, param_spec_val_t params[], int nelements,
                     int *status)
{
   int nptrs = 0,       /*---* number of pointers to specs *---*/
       nxtchk = 0,      /*---* number of array entries checked *---*/
       matched = 0,     /*---* number of elements matched *---*/
       i = 0, found=0;


   char *cp,            /*---* utility pointer                        *---*/
        *valptr,        /*---* pointer to value part of expression    *---*/
        *specptr,       /*---* pointer to specification part of expr. *---*/
        inbuf[RD_CNF_INBUFSZ]; /*---* input buffer                    *---*/

   FILE *fp, *fopen();
 
   param_spec_val_p_t *pvalbase_pp, /*---* pointer to array of param_val 
                                     *---* pointers
                                     *---*/
                      *pval_pp;     /*---* index pointer to param_val pointer
                                     *---* array
                                     *---*/

   /*---* open the file.  if we get nothing back...  *---*/
   if (!(fp = fopen(fn, "r")))
   {
      /*---* print a message *---*/
      perror("Opening config file");
      /*---* set return value *---*/
      *status = rdconf_cantopen;
      /*---* and return *---*/
      return;
   }
   
   /*---* malloc out space for pointers... if no space... *---*/
   if (!(pvalbase_pp = 
           (param_spec_val_t **)malloc(sizeof(param_spec_val_t *) * nelements)))
   {
      /*---* report error *---*/
      perror("Getting pointer array space");
      /*---* set status *---*/
      *status = rdconf_nospace;
      fclose(fp);
      return;
   }

   /*---* 
    *---* file open, pointer array ready.  While we have lines and 
    *---* unmatched values...
    *---*/
   while ((fgets(inbuf, RD_CNF_INBUFSZ, fp) != (char *)NULL) && matched != nelements)
   {
      /*---* look for the start of a comment *---*/
      cp = strchr(inbuf, '#');
      
      /*---* if we found one... *---*/
      if (cp)
         /*---* replace the '#' with a null to strip the comment *---*/
         *cp = '\0';
      else
         /*---* else no comment char, go to end of line *---*/
         cp = inbuf + strlen(inbuf);

      /*---* look for the '=' sign *---*/
      valptr = strchr(inbuf, '=');

      /*---* if we didn't get one... *---*/
      if (!valptr)
         /*---* then skip this line *---*/
         continue;
      /*---* else if there is whitespace on either side of the '='... *---*/
      else if (isspace(*(valptr-1)) || isspace(*(valptr+1)))
      {
         /*---* syntax error... set return value and return *---*/
         *status = rdconf_syntax; 
         fclose(fp);
         return;
      }
      else
         /*---* 
          *---* got a  good assignment.  Null the '=' to terminate the 
          *---* specifier part and inc valptr to point to the start of the 
          *---* value part.
          *---*/
         *valptr++ = '\0';
     
      /*---* strip trailing whitespace *---*/
      while(isspace(*--cp));

      /*---* strip any trailing '"' *---*/
      if (*cp == '"') 
         --cp; 
      /*---* null terminate the value part *---*/
      *++cp = '\0';

      /*---* strip leading whitespace *---*/
      for (specptr = inbuf; isspace(*specptr); specptr++);
      
      /*---* look for whitespace in our spec *---*/
      for (cp = specptr; *cp && !isspace(*cp); cp++);
      /*---* if we are not right next to the value pointer... *---*/
      if (++cp != valptr)
         /*---* 
          *---* we assume some other kind of line than one we can read,
          *---* so continue.
          *---*/
         continue;
      
      /*---* if valptr is pointing at a ", get rid of it *---*/
      if (*valptr == '"')
          ++valptr;

      /*---* 
       *---* at this point we have an acceptable line, so start looking for a
       *---* match in the list of specs that we were passed.
       *---*
       *---* for each of the pointer entries we have into our spec list...
       *---*/
      found = 0;
      for(pval_pp = pvalbase_pp; !found && pval_pp - pvalbase_pp < nptrs; 
                                                                   pval_pp++)
         /*---*
          *---* if the spec in the line matches that in the entry the 
          *---* pointer refers to...
          *---*/
         if (!strcmp((*pval_pp)->param_spec, specptr))

	 {
            /*---* get space for the value, if we can't get space *---*/
            if (!((*pval_pp)->param_val = (char *)malloc(strlen(valptr) + 1)))
	    {
               /*---* print error... *---*/
               perror("Trying to get space for value");
               /*---* set error return value... *---*/
               *status = rdconf_nospace;
               /*---* and return *---*/
               fclose(fp);
               return;
	    }
            
            /*---* else got the space, so copy the value into it *---*/
            strcpy((*pval_pp)->param_val, valptr);
            
            /*---*
             *---* now fix the pointer array to close the hole.  Copy the nth
             *---* pointer to this one....
             *---*/
            *pval_pp = *(pvalbase_pp + nptrs - 1);
     
            /*---* Null the nth spot... *---*/
            *(pvalbase_pp + nptrs - 1) = NULL;

            /*---* decrement the pointer counter... *---*/
            nptrs--;
            /*---* increment the match count.. *---*/
            matched++;
            /*---* 
             *---* set the found flag to indicate that we are done with 
             *---* this line...
             *---*/
            found = 1;
         }
      /*---* END of for (pval_pp...) *---*/
      
      /*---* 
       *---* At this point, if we still have no match, look in the passed
       *---* array directly.  We start the search through the array at the 
       *---* entry after the last entry we looked at (nxtchk).  All the array 
       *---* entries before nxtchk have either been matched or have been 
       *---* assigned a pointer in the pointer array and ergo were checked in 
       *---* the loop through the pointer array.
       *---*
       *---* For all the entries in the array, or until a match is found...
       *---*/
      for (i = nxtchk; !found && i < nelements; i++)
      { 
         /*---* if the spec in the line matches the spec in the array... *---*/
         if (!strcmp(params[i].param_spec, specptr))
	 {
            /*---* get space for the value, if we can't get space *---*/
            if (!(params[i].param_val = (char *)malloc(strlen(valptr) + 1)))
	    {
               /*---* print error... *---*/
               perror("Trying to get space for value");
               /*---* set error return value... *---*/
               *status = rdconf_nospace;
               /*---* and return *---*/
               fclose(fp);
               return;
	    }
            
            /*---* got the space, copy the value. *---*/
            strcpy(params[i].param_val, valptr);
    
            /*---* increment match counter *---*/
            matched++;
            /*---* increment nxtchk *---*/
            nxtchk++;
            /*---* done with this line *---*/
            break;
	 }
         else
	 {
            /*---* 
             *---* else if no match, point the next available pointer at 
             *---* this struct.
             *---*/
            *(pvalbase_pp+nptrs++) = params+i;
            nxtchk++;
	 }
      } /*---* END of for (i=0...)  *---*/
   }  /*---* end of while (fgets...) *---*/
   
   /*---* if the matches don't match what we wanted ... *---*/
   if (matched != nelements)
   {
      /*---* didn't find all needed values, so set error return... *---*/
      *status = rdconf_noparam;
      fclose(fp);
      return;
   }
   *status = rdconf_ok;
   fclose(fp);
}

