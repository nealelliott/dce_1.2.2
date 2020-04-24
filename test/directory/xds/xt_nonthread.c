/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_nonthread.c,v $
 * Revision 1.1.6.2  1996/02/18  00:24:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:59  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:13:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:55  root]
 * 
 * Revision 1.1.4.1  1994/02/22  19:15:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:29:26  marrek]
 * 
 * Revision 1.1.2.2  1993/08/12  12:25:03  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:26:46  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xt_nonthread.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 00:24:40 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <xt.h>
#include <xt_thread_struct.h>



extern const OM_descriptor *master_session_tbl[];
extern const OM_workspace  *master_workspace_tbl[];
extern const OM_private_object master_private_object_tbl[];
extern const OM_public_object master_public_object_tbl[];
extern const OM_string master_string_tbl[];


struct thread_data_struct xt_nonthread_block;

void    xt_initialize_global_data(void);
void    xt_nonthread_init(void);
void    xt_check_string_tbl(void);

void    xt_initialize_global_data(void)
{
    OM_sint index;

    /*  initialize workspace_tbl: */
    for (index = 0; index < XT_LEN_WORKSPACE_TBL; index++) {
        xt_nonthread_block.workspace_tbl[index]  = master_workspace_tbl[index];
    } /* end for */

    /* initialize session_tbl: */
    for (index = 0; index < XT_LEN_SESSION_TBL; index++) {
        xt_nonthread_block.session_tbl[index] =     master_session_tbl[index];
    } /* end for */

    /*  initialize public_object_tbl: */
    for (index = 0; index < XT_LEN_PUBLIC_OBJECT_TBL; index++) {
        xt_nonthread_block.public_object_tbl[index]  = master_public_object_tbl[index];
    } /* end for */

    /*  initialize private_object_tbl: */
    for (index = 0; index < XT_LEN_PRIVATE_OBJECT_TBL ; index++) {
        xt_nonthread_block.private_object_tbl[index] = master_private_object_tbl[index];
    } /* end for */

    /*  initialize string_tbl: */
    for (index = 0; index < XT_LEN_STRING_TBL ; index++) {
        xt_nonthread_block.string_tbl[index] = master_string_tbl[index];
    } /* end for */

} /* end xt_initialize_global_data */


void    xt_nonthread_init(void)
{
    xt_initialize_global_data();
} /* end xt_nonthread_init */


/* for debugging only */
static void    xt_do_break(const char *message)
{
    if (xt_verbose)  {
        printf ("%s\n", message);
#if USE_GUARDS
        check_guards();
#endif  /* USE_GUARDS */
    } /* endif */
} /* end xt_do_break */


void    xt_check_string_tbl(void)
{
    extern const OM_string master_string_tbl[];
    OM_sint index;
    BOOLEAN error_found  = FALSE;

    for ( index  = 0; index < XT_LEN_STRING_TBL; index++) {
        if (xt_nonthread_block.string_tbl[index].elements != master_string_tbl[index].elements)  {
            printf("string_tbl[%i].elements was %s, while " "master_string_tbl[%i].elements was %s\n", index,
                                                xt_nonthread_block.string_tbl[index].elements, index, master_string_tbl[index].elements);
            error_found  = TRUE;
        } else if (strcmp(xt_nonthread_block.string_tbl[index].elements, master_string_tbl[index].elements)) {
            printf("*string_tbl[%i].elements was %s, while " "*master_string_tbl[%i].elements was %s\n",
                                index, xt_nonthread_block.string_tbl[index].elements, index,     master_string_tbl[index].elements);
            error_found  = TRUE;
        } /* endif */
        if ( xt_nonthread_block.string_tbl[index].length != master_string_tbl[index].length) {
            printf("string_tbl[%i].length was %i, while " "master_string_tbl[%i].length was %i\n", index, xt_nonthread_block.string_tbl[index].length, index, master_string_tbl[index].length);
            error_found  = TRUE;
        } /* endif */
    } /* endfor */
    if ( error_found == TRUE) {
        xt_do_break("check string tbl found error");
    } /* endif */
} /* end xt_check_string_tbl */
