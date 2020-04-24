/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: cma_message.c,v $
 * Revision 1.1.12.1  1996/05/10  13:38:51  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:42 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:19  truitt  /main/HPDCE01/1]
 *
 * 	Modified cma__error_inq_text() so that if catopen(<catalog>) succeeds and
 * 	catgets() fails, we retry with catopen(/usr/lib/nls/${LANG:-C}/<catalog>)
 * 	rather than give up.  This change is partly necessitated by the behavior
 * 	of the HP-UX 9.x catopen(), which opens any existent file that NLSPATH
 * 	specifies, even a directory, and returns a seemingly valid catalog
 * 	descriptor for that file.
 * 	[1993/12/15  23:14:08  mk]
 * 	 *
 *
 * 	CHFts09400: exc_report() function was not printing out proper
 * 	error message for non-thr exceptions.  Changed the variable
 * 	DEFAULT_NLSPATH to be the proper value for hpux.
 * 	[1993/11/10  20:01:40  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:42  root  1.1.4.3]
 *
 * Revision 1.1.7.3  1994/06/09  13:38:16  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:08  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/25  19:57:49  ohara
 * 	include cma_defs.h
 * 	[1994/04/21  17:22:03  ohara]
 * 
 * 	use cma__int_sprintf instead of sprintf
 * 	[1994/04/20  19:17:48  ohara]
 * 
 * Revision 1.1.7.1  1994/04/01  20:16:39  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:57  jd]
 * 
 * Revision 1.1.4.2  1993/06/24  21:01:45  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:44:09  hinman]
 * 
 * Revision 1.1.2.6  1993/01/08  22:22:37  tom
 * 	Bug 6776 - Do not call setlocale in a library routine.
 * 	Locale init should always be done in the application.
 * 	[1993/01/08  20:56:43  tom]
 * 
 * Revision 1.1.2.5  1992/12/15  22:28:52  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:39:38  alan]
 * 
 * Revision 1.1.2.4  1992/09/29  20:15:07  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:49:40  sekhar]
 * 
 * Revision 1.1.2.3  1992/09/04  20:11:32  bolinger
 * 	Fix up log.
 * 	[1992/09/04  19:52:51  bolinger]
 * 
 * 	Fix OT defect 5317:  clean up rules for lookup of message catalog in
 * 	default location when NLSPATH lookup fails, and (for AIX 3.2) perform
 * 	explicit locale initialization, without which catalog lookup will
 * 	always fail.
 * 	[1992/09/04  15:06:49  bolinger]
 * 
 * Revision 1.1.2.2  1992/09/03  14:38:24  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:04:43  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:33:13  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:32:55  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      cma_message.c
**
**  FACILITY:
**
**      Distributed Computing Environment (DCE)
**
**  ABSTRACT:
**
**  Error status management routines.
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  20-Nov-91 butenhof	Prototype, extra level of fallback for cat file.
**  19-Nov-91 harrow	Create cma version, include default values for thd messages.
**  22-oct-91 mishkin   map rad-50 0 to "a" instead of "_".
**  13-jun-91 martin    Preface rpcsts.h with dce/;
**                      remove inclusion of dce_error.h.
**  09-apr-91 janicki   change status to an int
**  27-mar-91 mishin    remove version number stuff; add support for DFS
**  08-mar-91 janicki   Change order. Full status code now
**                      <facility><component><code>
**                      Update dce_error_inq_text() signature.
**  25-jan-91 janicki   Add .cat to file name.
**                      Add second try to open message file. 
**  07-jan-91 sudama    Fix message not found to return -1
**                      Fix logic in message file version mismatch
**  29-nov-90 woodbury  Change #include paths for IBM
**  13-nov-90 sudama    Original creation.
**
**  %a%private_end  
**
*/

#include <pthread.h>		/* for catalog routine prototypes */
#include <stdio.h>          /* standard i/o */
#include <cma_defs.h>
#include <dce/cma.h>		/* for cma__int* routines */
#if _CMA_UNIX_TYPE ==  _CMA__SVR4
#include <string.h>
#else
#include <strings.h>
#endif /* _CMA__SVR4 */
#include <nl_types.h>       /* public types for NLS (I18N) routines */
#include <cma_message.h>

#define FACILITY_CODE_MASK          0xF0000000u
#define FACILITY_CODE_SHIFT         28

#define COMPONENT_CODE_MASK         0x0FFFF000u
#define COMPONENT_CODE_SHIFT        12

#define STATUS_CODE_MASK            0x00000FFFu
#define STATUS_CODE_SHIFT           0

#define NO_MESSAGE                  "THIS IS NOT A MESSAGE"

/*
 * Define the default location for the catalog files.
 */
#ifdef AIX32
#define DEFAULT_NLSPATH "/usr/lib/nls/msg/En_US/%s"
#else
#ifdef __hpux
#define DEFAULT_NLSPATH "/usr/lib/nls/%s/%s.cat"
#else
#define DEFAULT_NLSPATH "/usr/lib/nls/msg/en_US.88591/%s"
#endif
#endif


/*
**++
**
**  ROUTINE NAME:       cma__error_inq_text
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**      
**  Returns a text string in a user provided buffer associated with a given 
**  error status code. In the case of errors a text string will also be 
**  returned indicating the nature of the error.
**
**  INPUTS:
**
**      status_to_convert   A DCE error status code to be converted to 
**                          text form.
**
**  INPUTS/OUTPUTS:         None.
**
**  OUTPUTS:
**
**      error_text          A user provided buffer to hold the text
**                          equivalent of status_to_convert or
**                          a message indicating what error occurred.
**                          
**
**      status              The result of the operation. One of:
**                           0  -  success
**                          -1  -  failure
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/
void
cma__error_inq_text(
        unsigned long	status_to_convert,
	unsigned char	*error_text,
	int		*status)
{
    unsigned short  facility_code;
    unsigned short  component_code;
    unsigned short  status_code;
    unsigned short  i;
    nl_catd     catd;
    char        component_name[4];
    char        *facility_name;
    char        filename_prefix[7];
    char        nls_filename[11];
    char        alt_filename[80];
    char        *message;
    char        *lang;
    char        *thd_message = NULL;
    static char *facility_names[] = {
        "dce",
        "dfs"
    };
    static char default_lang[] = "C";

    /*
     * set up output status for future error returns
     */
    if (status != NULL)
    {
        *status = -1;
    }
    
    /*
     * check for ok input status
     */
    if (status_to_convert == 0)
    {
        if (status != NULL)
        {
            *status = 0;
        }
        strcpy ((char *)error_text, "successful completion");
        return;
    }

    /*
     * extract the component, facility and status codes
     */
    facility_code = (status_to_convert & FACILITY_CODE_MASK)
        >> FACILITY_CODE_SHIFT;
        
    component_code = (status_to_convert & COMPONENT_CODE_MASK)
        >> COMPONENT_CODE_SHIFT;

    status_code = (status_to_convert & STATUS_CODE_MASK)
        >> STATUS_CODE_SHIFT;

    /*
     * see if this is a recognized facility
     */
    if (facility_code == 0 || facility_code > sizeof (facility_names) / sizeof (char *))
    {
        cma__int_sprintf ((char *) error_text,
		"status %08x (unknown facility)", status_to_convert);
        return; 
    }

    facility_name = facility_names[facility_code - 1];

    /*
     * Convert component name from RAD-50 component code.  (Mapping is:
     * 0 => 'a', ..., 25 => 'z', 26 => '{', 27 => '0', ..., 36 => '9'.)
     */

    component_name[3] = 0;
    component_name[2] = component_code % 40;
    component_code /= 40;
    component_name[1] = component_code % 40;
    component_name[0] = component_code / 40;
    
    for (i = 0; i < 3; i++)
    {
        component_name[i] += (component_name[i] <= 26) ? 'a' : ('0' - 27);
    }

    cma__int_sprintf (filename_prefix, "%3s%3s", facility_name, component_name);

    /* 
     * Add ".cat" to prefix to obtain the full message file name
     */
     
    cma__int_sprintf (nls_filename, "%s", filename_prefix);

    /*
     * Open the message catalog, using NLSPATH.
     */
     
    catd = (nl_catd) catopen (nls_filename, 0);

    if (catd != (nl_catd) -1)
    {
        message = (char *) catgets (catd, 1, status_code, NO_MESSAGE);
        catclose (catd);
	if (strcmp (message, NO_MESSAGE) != 0)
	{
            cma__int_sprintf ((char *) error_text, "%s (%s / %s)", 
                    message, facility_name, component_name);
	    if (status != NULL)
	    {
		*status = 0;
	    }
            return;
	}
    }    

    /*
     * Either we failed to open a catalog or we opened a catalog and failed
     * to get a good message from it.  We try a well-known location, without
     * using NLSPATH.
     *
     * We retry without NLSPATH after a failed catgets() partly to work
     * around the willingness of HP-UX 9.x catopen() to return a catalog
     * descriptor for any file it can open, even a directory.  A frequent
     * misuse of NLSPATH, setting it to a directory name, therefore can
     * yield a seemingly-valid but actually-unusable descriptor.
     */

    /*
     * Use LANG if possible.
     */
    lang = getenv ("LANG");
    cma__int_sprintf (alt_filename,
        DEFAULT_NLSPATH, 
        (lang == NULL) ? default_lang : lang,
        filename_prefix);

    catd = (nl_catd) catopen (alt_filename, 0);
        
    if (catd != (nl_catd) -1)
    {
        message = (char *) catgets (catd, 1, status_code, NO_MESSAGE);
        catclose (catd);
	if (strcmp (message, NO_MESSAGE) != 0)
	{
            cma__int_sprintf ((char *) error_text, "%s (%s / %s)", 
                    message, facility_name, component_name);
	    if (status != NULL)
	    {
		*status = 0;
	    }
            return;
	}
    }

    /*
     * We failed again, so we give up and return (for known CMA statuses)
     * a hardwired message or (for other statuses) a numerical status code.
     */
    switch (status_to_convert)
    {
        case exc_s_exception       : thd_message="Address exception raised";break;
        case exc_s_exccop          : thd_message="Status exception raised";break;
        case exc_s_uninitexc       : thd_message="Uninitialized exception raised";break;
        case exc_s_illaddr         : thd_message="Invalid memory address";break;
        case exc_s_exquota         : thd_message="Operation failed due to insufficient quota";break;
        case exc_s_insfmem         : thd_message="Insufficient virtual memory for requested operation";break;
        case exc_s_nopriv          : thd_message="Insufficent privilege for requested operation";break;
        case exc_s_normal          : thd_message="Normal successful completion";break;
        case exc_s_illinstr        : thd_message="Illegal instruction";break;
        case exc_s_resaddr         : thd_message="Reserved addressing error";break;
        case exc_s_privinst        : thd_message="Privileged instruction error";break;
        case exc_s_resoper         : thd_message="Reserved operand error";break;
        case exc_s_SIGTRAP         : thd_message="Trace or breakpoint trap";break;
        case exc_s_SIGIOT          : thd_message="IOT trap";break;
        case exc_s_SIGEMT          : thd_message="EMT trap";break;
        case exc_s_aritherr        : thd_message="Arithmetic error";break;
        case exc_s_SIGSYS          : thd_message="Bad system call";break;
        case exc_s_SIGPIPE         : thd_message="Broken pipe";break;
        case exc_s_excpu           : thd_message="CPU-time limit exceeded";break;
        case exc_s_exfilsiz        : thd_message="File-size limit exceeded";break;
        case exc_s_intovf          : thd_message="Integer overflow";break;
        case exc_s_intdiv          : thd_message="Integer divide by zero";break;
        case exc_s_fltovf          : thd_message="Floating point overflow";break;
        case exc_s_fltdiv          : thd_message="Floating point/decimal divide by zero";break;
        case exc_s_fltund          : thd_message="Floating point underflow";break;
        case exc_s_decovf          : thd_message="Decimal overflow";break;
        case exc_s_subrng          : thd_message="Subscript out of range";break;
        case cma_s_alerted         : thd_message="Thread has been canceled";break;
        case cma_s_assertion       : thd_message="A thread assertion check has failed";break;
        case cma_s_badparam        : thd_message="Invalid parameter to operation";break;
        case cma_s_bugcheck        : thd_message="Internal error detected in thread library";break;
        case cma_s_exit_thread     : thd_message="Current thread has been requested to exit";break;
        case cma_s_existence       : thd_message="Object referenced does not currently exist";break;
        case cma_s_in_use          : thd_message="Object referenced is already in use";break;
        case cma_s_use_error       : thd_message="Operation inappropriate for specified object";break;
        case cma_s_wrongmutex      : thd_message="Wrong mutex specified in condition wait";break;
        case cma_s_stackovf        : thd_message="Attempted stack overflow was detected";break;
        case cma_s_nostackmem      : thd_message="No space is currently available to create a new stack";break;
        case cma_s_notcmastack     : thd_message="The current stack was not allocated by thread library";break;
        case cma_s_timed_out       : thd_message="Timed condition wait expired";break;
        case cma_s_unimp           : thd_message="Unimplemented feature";break;
        case cma_s_inialrpro       : thd_message="Thread library initialization is already in progress";break;
        case cma_s_defer_q_full    : thd_message="No space currently available to process interrupt request";break;
        case cma_s_signal_q_full   : thd_message="Unable to process condition signal from interrupt level";break;
        case cma_s_alert_nesting   : thd_message="Improper nesting of alert scope";break;
        case exc_s_unkstatus       : thd_message="Unknown status code 0x%1$08X";break;
#if _CMA_UNIX_TYPE ==  _CMA__SVR4
        case exc_s_illopc: thd_message="Illegal opcode"; break;
        case exc_s_illopn: thd_message="Illegal operand"; break;
        case exc_s_illadr: thd_message="Illegal addressing mode"; break;
        case exc_s_illtrp: thd_message="Illegal trap"; break;
        case exc_s_prvopc: thd_message="privileged opcode"; break;
        case exc_s_prvreg: thd_message="Privileged register"; break;
        case exc_s_coproc: thd_message=" Co-processor"; break;
        case exc_s_badstk: thd_message="Bad Stack"; break;
        case exc_s_brkpt : thd_message="Process breakpoint"; break;
        case exc_s_trace : thd_message="Process trace"; break;
        case exc_s_fltinv: thd_message="Invalid floating point operation"; break;
        case exc_s_maperr: thd_message="Address not mapped to object"; break;
        case exc_s_accerr: thd_message="Invalid permissions"; break;
        case exc_s_adraln: thd_message="Invalid address alignment"; break;
        case exc_s_adrerr: thd_message="Non-existent physical address"; break;
        case exc_s_objerr: thd_message="Object specific"; break;
#endif
        default:
        {
            cma__int_sprintf (
                (char *)error_text,
                "status %08x (%s / %s)",
                status_to_convert,
                facility_name,
                component_name);

            /*
             * Returning good status in this case is questionable and
             * is inconsistent with dce_error_inq_text(), but we do so
             * to preserve historic behavior.
             */
            *status = 0;
        }
    }

    if (thd_message != NULL)
    {
        cma__int_sprintf (
            (char *)error_text,
            "%s (%s / %s)",
            thd_message,
            facility_name,
            component_name);
        *status = 0;
    }

    return;
}        
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_MESSAGE.C */
/*  *6     9-JAN-1992 10:27:22 BUTENHOF "Remove ""Exception: "" from builtin messages" */
/*  *5    22-NOV-1991 11:56:28 BUTENHOF "Integrate dce message formatting" */
/*  *4    24-SEP-1991 16:27:35 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *3     2-JUL-1991 16:47:17 BUTENHOF "Use normal printf on Tin" */
/*  *2    13-JUN-1991 19:33:12 BUTENHOF "fix history" */
/*  *1    13-JUN-1991 19:29:11 BUTENHOF "Message catalog support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_MESSAGE.C */
