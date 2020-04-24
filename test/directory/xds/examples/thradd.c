/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: thradd.c,v $
 * Revision 1.1.6.2  1996/02/18  00:24:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:51  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:13:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:33  root]
 * 
 * Revision 1.1.4.3  1994/10/05  16:44:27  keutel
 * 	OT 12431
 * 	[1994/10/05  16:37:08  keutel]
 * 
 * Revision 1.1.4.2  1994/06/10  20:19:35  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:34  devsrc]
 * 
 * Revision 1.1.4.1  1994/02/22  16:40:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:35:42  marrek]
 * 
 * Revision 1.1.2.1  1993/10/14  19:28:33  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:26:52  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/13  12:29:10  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  12:38:10  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: thradd.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 00:24:30 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1993   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*
 * The program operates in two modes; it adds or removes entries of
 * object type organisational person to/from a directory. The information
 * about the entries is read from a file.
 *
 * The program requires that there exists a tree in the directory.
 * Therefore, each time the program runs, the following tree of 3 entries 
 * is added to or removed from the directory, according to the operation mode.
 *
 *              O  C=it
 *              |  (objectClass=Country)
 *              |
 *              O  O=sni
 *              |  (objectClass=Organization)
 *              |
 *              O  OU=ap
 *                 (objectClass=OrganizationalUnit)
 *
 * Information about the oranisational persons to be added or removed is
 * read from the input file. It may contain any number of lines, where 
 * each line must have the following syntax:
 *
 *     <common name> <surname> <phone number>
 *     Each item must be a string without a blank.
 *
 * Lines containing less than 3 strings are rejected by the program.
 * The program does not check to see if the strings conform to the 
 * appropiate attribute syntax; i.e. a wrong attribute syntax will
 * lead to an ds_add_entry error, or to a ds_remove_entry error.
 *
 *
 * Usage: thradd [-d] [-f<file name>]
 *   -d            If the option -d is set, the entries in the file and the 
 *                 tree described above are removed, otherwise they are added.
 *   -f<file name>   The option -f specifies the name of the input file.
 *                 If left out, the default "thradd.in" is used.
 *
 */

/* Step 1 */ 

/*
 * Header file for thread programming:
 */
#include <pthread.h>

#include <stdio.h>
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include <xdscds.h>
#include "thradd.h"           /* static data structures. */


/* Step  2*/

/*
 * typedef for parameter block of function add_or_remove
 * (this is necessary because start functions of a thread 
 *  takes only 1 parameter). The following 3 parameters are
 * passed to add_or_remove:
 *
 *	Input - Session object from the ds_bind call
 *	Input - Buffer with the entry information
 *	Input - "adding" or "removing" mode?
 */
typedef struct {
    OM_private_object session;         
    char              line[MAX_LINE_LEN+1];
    int               do_remove;
} pb_add_or_remove;


/*
 * static constants:
 *
 * Default name for input file containing entry information.
 */
static char fn_default[] = "thradd.in";


/*
 * function declarations: 
 */
char *own_fgets(char *s, int n, FILE *f);
void add_or_remove(pb_add_or_remove *pb);


int
main(
    int  argc,
    char *argv[]
)
{
    OM_workspace      workspace;         /* workspace for objects            */
    OM_private_object bound_session;     /* Holds the Session object which   */
                                         /* is returned by ds_bind()         */
    FILE              *fp;               /* file pointer for input file      */
    int               do_remove = FALSE; /* option -d for remove set ?       */
    int               error     = FALSE; /* error in options ?               */
    int               is_eof    = FALSE; /* EOF in input file reached ?      */
    int               thread_count;      /* actual number of created threads */
    char              *file_name;        /* pointer to input file name       */

    /* Step  3 */

    pthread_t         threads[MAX_THREAD_NO];     /* thread table            */
    pb_add_or_remove  param_block[MAX_THREAD_NO]; /* 1 param block for start */
                                                  /* routine per thread      */
    int               dummy;
    int               c;
    int               i;
    extern char       *optarg;         /* external variable used by getopt */
    extern int        optind;          /* external variable used by getopt */



    /*
     * scan options -d and -f
     */
    file_name = fn_default;

    while ((c=getopt(argc, argv, "df:")) != EOF)
    {
        switch (c)
        {
        case 'd':
            do_remove = TRUE;
            break;
        case 'f':
            file_name = optarg;
            break;
        default:
            error = TRUE;
            break;
        }
    }

    if (error)
    {
        printf("usage: %s [-d] [-f<file name>]\n", argv[0]);
        return(FAILURE);
    }

    if (( fp = fopen(file_name, "r")) == (FILE *) NULL)
    {
        printf("fopen() error, file name: %s\n", file_name);
        return(FAILURE);
    }


    /*
     * Initialize a directory workspace for use by XOM.
     */
    if ((workspace = ds_initialize()) == (OM_workspace)0)
        printf("ds_initialize() error\n");


    /*
     * Negotiate the use of the BDCP and GDS packages.
     */
    if (ds_version(features, workspace) != DS_SUCCESS)
        printf("ds_version() error\n");


    /*
     * Add the fixed tree of entries, if adding mode
     */
    if (!do_remove)
        if (add_entries(workspace))
            printf("add_entries() error\n");


    /*
     * Bind to the default GDS server.
     * The returned session object is stored in the private object variable
     * bound_session and is used for further XDS function calls.
     */
    if (ds_bind(DS_DEFAULT_SESSION, workspace, &bound_session) != DS_SUCCESS)
        printf("ds_bind() error\n");


    /* Step  4 */

    /*
     * Add or remove entries described in input file.
     * This is done in parallel, creating up to MAX_THREAD_NO threads
     * at a time.
     */
    while (!is_eof)
    {
        for (thread_count=0; thread_count<MAX_THREAD_NO; thread_count++)
        {
            /* Step  5 */

	    /*
             * Prepare parameter block:
             */
            is_eof = (own_fgets(param_block[thread_count].line, 
                                MAX_LINE_LEN, fp) == NULL);
            if (is_eof)
                break;

            param_block[thread_count].session   = bound_session;
            param_block[thread_count].do_remove = do_remove;

            /* Step  6 */

            /*
             * Create thread with start routine add_or_remove:
             */
            if (pthread_create(&threads[thread_count], pthread_attr_default,
                               (pthread_startroutine_t) add_or_remove,
                               (pthread_addr_t) &param_block[thread_count])
                != SUCCESS)
                printf("pthread_create() error\n");
        } /* end for */

        /*
         * Wait for termination of the created threads and release resources:
         */
        for (i=0; i<thread_count; i++)
        {

            /* Step  7 */

            /*
             * Wait for termination of thread
             * (If thread has terminated already, the function returns
             * immediately):
             */
             if (pthread_join(threads[i], (pthread_addr_t) &dummy) != SUCCESS)
                printf("pthread_join() error\n");

            /* Step  8 */

            /*
             * Release resources used by the thread:
             */
             if (pthread_detach(&threads[i]) != SUCCESS)
                printf("pthread_detach() error\n");
        } /* end for */
    } /* end while */


    /*
     * Close the connection to the GDS server.
     */
    if (ds_unbind(bound_session) != DS_SUCCESS)
        printf("ds_unbind() error\n");

    if (om_delete(bound_session) != OM_SUCCESS)
        printf("om_delete() error\n");


    /*
     * Remove the tree from the Directory, if removing mode
     */
    if (do_remove)
        if (remove_entries(workspace))
            printf("remove_entries() error\n");


    /*
     * Close the directory workspace.
     */
    if (ds_shutdown(workspace) != DS_SUCCESS)
        printf("ds_shutdown() error\n");

    fclose(fp);
    return(SUCCESS);
} /* end main() */


/* Step  9 */

/*
 * Handle (add or remove) a directory entry
 */
void
add_or_remove( 
    pb_add_or_remove    *pb   /* see typedef for parameter information    */
)
{

    /*
     * further local variables:
     */
    char               common_name[MAX_AT_LEN+1];
    char               phone_num[MAX_AT_LEN+1]; 
    char               surname[MAX_AT_LEN+1];  
    OM_sint            invoke_id;             
    
    /* Step 10 */

    /*
     * local variables for descriptors for objects read from file
     */
    OM_descriptor    ava_genop[] = {
        OM_OID_DESC(OM_CLASS, DS_C_AVA),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
        OM_NULL_DESCRIPTOR,   /* place holder */
        OM_NULL_DESCRIPTOR
    };
    
    OM_descriptor    rdn_genop[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
        OM_NULL_DESCRIPTOR,           /* place holder */
        OM_NULL_DESCRIPTOR
    };
    
    OM_descriptor dn_genop[] = {
        OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
        {DS_RDNS,OM_S_OBJECT,{0,rdn_it}},
        {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
        {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
        OM_NULL_DESCRIPTOR,           /* place holder */
        OM_NULL_DESCRIPTOR
    };
    
    OM_descriptor    att_phone_num[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
        OM_NULL_DESCRIPTOR,           /* place holder */
        OM_NULL_DESCRIPTOR
    };
     
    OM_descriptor    att_surname[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
        OM_NULL_DESCRIPTOR,           /* place holder */
        OM_NULL_DESCRIPTOR
    };
     
    OM_descriptor    alist_OP[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
        {DS_ATTRIBUTES, OM_S_OBJECT, {0, obj_class_OP} },
        OM_NULL_DESCRIPTOR,           /* place holder */
        OM_NULL_DESCRIPTOR,           /* place holder */
        OM_NULL_DESCRIPTOR
    };

    rdn_genop[1].type = DS_AVAS;
    rdn_genop[1].syntax = OM_S_OBJECT;
    rdn_genop[1].value.object.padding = 0;
    rdn_genop[1].value.object.object = ava_genop;

    dn_genop[4].type = DS_RDNS;
    dn_genop[4].syntax = OM_S_OBJECT;
    dn_genop[4].value.object.padding = 0;
    dn_genop[4].value.object.object = rdn_genop;

    alist_OP[2].type = DS_ATTRIBUTES;
    alist_OP[2].syntax = OM_S_OBJECT;
    alist_OP[2].value.object.padding = 0;
    alist_OP[2].value.object.object = att_surname;

    alist_OP[3].type = DS_ATTRIBUTES;
    alist_OP[3].syntax = OM_S_OBJECT;
    alist_OP[3].value.object.padding = 0;
    alist_OP[3].value.object.object = att_phone_num;
    
    if (sscanf(pb->line, "%s %s %s", common_name, surname, phone_num) != 3)
    {
        printf("invalid input line: >%s<\n", pb->line);
        return;
    }
    /*
     * Fill descriptor for common name 
     */
    ava_genop[2].type = DS_ATTRIBUTE_VALUES;
    ava_genop[2].syntax = OM_S_PRINTABLE_STRING;
    ava_genop[2].value.string.length = (OM_string_length)strlen(common_name);
    ava_genop[2].value.string.elements = common_name;

    if (!pb->do_remove)     /* add */
    {
        /*
         * Fill descriptors for surname and phone number
         */
        att_surname[2].type = DS_ATTRIBUTE_VALUES;
        att_surname[2].syntax = OM_S_TELETEX_STRING;
        att_surname[2].value.string.length = 
                                 (OM_string_length)strlen(surname);
        att_surname[2].value.string.elements = surname;

        att_phone_num[2].type = DS_ATTRIBUTE_VALUES;
        att_phone_num[2].syntax = OM_S_PRINTABLE_STRING;
        att_phone_num[2].value.string.length = 
                                 (OM_string_length)strlen(phone_num);
        att_phone_num[2].value.string.elements = phone_num;

        /*
         * add entry
         */
        if (ds_add_entry(pb->session, DS_DEFAULT_CONTEXT, dn_genop, 
                         alist_OP, &invoke_id) != DS_SUCCESS)
            printf("ds_add_entry() error: %s %s %s\n", common_name, 
                                                       surname, phone_num);
        else
            printf("entry added: %s %s %s\n", common_name, surname, phone_num);
    }
    else      /* remove */
    {
        /*
         * remove entry
         */
        if (ds_remove_entry(pb->session, DS_DEFAULT_CONTEXT, dn_genop,
                                             &invoke_id) != DS_SUCCESS)
            printf("ds_remove_entry() error: %s\n", common_name);
        else
            printf("entry removed: %s\n", common_name);
    } /* end if */
} /* end add_or_remove() */



/*
 * Add the tree of entries described above.
 */
int
add_entries(
    OM_workspace workspace    /* In--XDS workspace                      */
)
{
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_sint           invoke_id;
    int               error = FALSE;


    /* Bind (without credentials) to the default GDS server */

    if (ds_bind(DS_DEFAULT_SESSION, workspace, &bound_session) != DS_SUCCESS)
        error = TRUE;


    /* Add entries to the GDS server */

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, dn_it, alist_C, 
                                                 &invoke_id) != DS_SUCCESS)
        error = TRUE;

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, dn_sni, alist_O,
                                                 &invoke_id) != DS_SUCCESS)
        error = TRUE;

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, dn_ap, alist_OU,
                                                 &invoke_id) != DS_SUCCESS)
        error = TRUE;


    /* Close the connection to the GDS server */

    if (ds_unbind(bound_session) != DS_SUCCESS)
        error = TRUE;

    if (om_delete(bound_session) != OM_SUCCESS)
        error = TRUE;

    return (error);
}





/*
 * Remove the tree of entries described above.
 */
int
remove_entries(
    OM_workspace workspace     /* In--XDS workspace                     */
)
{
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_sint           invoke_id;
    int               error = FALSE;


    /* Bind to the default GDS server */

    if (ds_bind(DS_DEFAULT_SESSION, workspace, &bound_session) != DS_SUCCESS)
        error = TRUE;


    /* Remove entries from the GDS server */

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_ap,
                                                 &invoke_id) != DS_SUCCESS)
        error = TRUE;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_sni,
                                                 &invoke_id) != DS_SUCCESS)
        error = TRUE;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_it,
                                                 &invoke_id) != DS_SUCCESS)
        error = TRUE;


    /* Close the connection to the GDS server */

    if (ds_unbind(bound_session) != DS_SUCCESS)
        error = TRUE;

    if (om_delete(bound_session) != OM_SUCCESS)
        error = TRUE;

    return (error);
}
 


/*
 * read one line with fgets and overwrite new line by a null character
 */

char *
own_fgets(
    char *s,   /* OUT--string read                        */
    int  n,    /* IN---maximum number of chars to be read */
    FILE *f    /* IN---input file                         */
)
{
    char *result;
    int  i = 0;

    result = fgets(s, n, f);
    if (result != NULL)
    {
        i = strlen(s);
	if (s[i-1] == '\n')
            s[i-1] = '\0';
    }
    return (result);
}
