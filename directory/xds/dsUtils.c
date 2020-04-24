/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsUtils.c,v $
 * Revision 1.1.79.2  1996/02/18  18:25:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:45  marty]
 *
 * Revision 1.1.79.1  1995/12/08  15:37:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:38  root]
 * 
 * Revision 1.1.77.2  1994/03/23  15:48:56  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:59  keutel]
 * 
 * Revision 1.1.77.1  1994/02/22  19:22:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:05:47  marrek]
 * 
 * Revision 1.1.75.1  1993/10/14  19:19:44  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:18:06  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:45:09  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:08:45  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:18:37  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  09:00:14  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  08:14:44  marrek
 * 	Insert <pthread.h> and add dsP_thread_id() function
 * 	[1993/02/01  14:09:34  marrek]
 * 
 * Revision 1.1.4.5  1992/12/30  23:29:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:00  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/17  09:44:31  marrek
 * 	Add verification check for object
 * 	[1992/12/16  09:09:13  marrek]
 * 
 * Revision 1.1.4.3  1992/12/04  17:36:46  marrek
 * 	November 1992 code drop
 * 	improved tracing messages
 * 	replace cmx.h by cmx_addr.h
 * 	[1992/12/04  12:14:58  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:09:50  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  14:46:28  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:15:29  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:42:40  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsUtils.c,v $ $Revision: 1.1.79.2 $ $Date: 1996/02/18 18:25:15 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : MODUL                                               */
/*                                                                    */
/* NAME         : dsUtils.c                                           */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open CAE Spec. API to Directory Services (XDS)    */
/*                X/Open CAE Spec. OSI-Abstract-Data Manip. API (XOM) */
/*                SNI XDS/XOM Design Specification November 1991      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : dsP_verify_instance() function                      */
/*                dsP_handle_omP_error() function                     */
/*                dsP_iapl2xds_error() function                       */
/*                dsP_find_ete_entry() function                       */
/*                dsP_compare_ete_index() function                    */
/*                dsP_validate_wkspace() function                     */
/*                dsP_validate_session() function                     */
/*                dsP_dec_op_num() function                           */
/*                dsP_validate_context() function                     */
/*                dsP_validate_name() function                        */
/*                dsP_validate_selection() function                   */
/*                dsP_xds2iapl_filter() function                      */
/*                dsP_count_filters() function                        */
/*                dsP_fill_filters() function                         */
/*                dsP_fill_filter_items() function                    */
/*                dsP_free_iapl_filter() function                     */
/*                dsP_thread_id() function                            */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/
 
#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <errno.h>
#include <stdlib.h>

#include <xom.h>
#include <xomip.h>
#include <alloc.h>
#include <xdsalloc.h>

#include <d2dir.h>
#include <d21iapl.h>
#include <d21.h>
#include <cmx_addr.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xdsp.h>

#include <xdsimport.h>
#include <xdscommon.h>
#include <xdstrace.h>
#include <dsUtils.h>



int	/* return type determined by bsearch */
dsP_compare_ete_index(
    const  void  *key,
    const  void  *entry
);


OM_sint
dsP_verify_instance(
    OM_object               object,
    OM_object_identifier    class
)
{
     OM_return_code rc;
             OM_boolean     instance;

    if (object) {
        if (OMP_CPUB(object)) {
            if ((object->type == OM_CLASS) &&
                ((object->syntax&OM_S_SYNTAX) == OM_S_OBJECT_IDENTIFIER_STRING))
            return(DONE);
        } else {
            if ((rc = om_instance(object, class, &instance)) == OM_SUCCESS) {
                return(instance);
			} /* endif */
		} /* endif */
    } /* endif */

    return(FALSE);
} /* end dsP_verify_instance */





DS_status
dsP_handle_omP_error(
    const OM_return_code  rc,
    const char            *fn,
    const OM_sint             ec,
    const OM_workspace    workspace
)
{
    OM_private_object   library_error;
    xds_error           *x_error;

#ifdef TRACE
    const char *why;

    switch (rc) {

        case OM_ENCODING_INVALID: 
            why = "OM_ENCODING_INVALID";
            break;

        case OM_FUNCTION_DECLINED:
            why = "OM_FUNCTION_DECLINED";
            break;

        case OM_FUNCTION_INTERRUPTED:
            why = "OM_FUNCTION_INTERRUPTED";
            break;

        case OM_MEMORY_INSUFFICIENT:
            why = "OM_MEMORY_INSUFFICIENT";
            break;

        case OM_NETWORK_ERROR:
            why = "OM_NETWORK_ERROR";
            break;

        case OM_NO_SUCH_CLASS:
            why = "OM_NO_SUCH_CLASS";
            break;

        case OM_NO_SUCH_EXCLUSION:
            why = "OM_NO_SUCH_EXCLUSION";
            break;

        case OM_NO_SUCH_MODIFICATION:
            why = "OM_NO_SUCH_MODIFICATION";
            break;

        case OM_NO_SUCH_OBJECT:
            why = "OM_NO_SUCH_OBJECT";
            break;

        case OM_NO_SUCH_RULES:
            why = "OM_NO_SUCH_RULES";
            break;

        case OM_NO_SUCH_SYNTAX:
            why = "OM_NO_SUCH_SYNTAX";
            break;

        case OM_NO_SUCH_TYPE:
            why = "OM_NO_SUCH_TYPE";
            break;

        case OM_NO_SUCH_WORKSPACE:
            why = "OM_NO_SUCH_WORKSPACE";
            break;

        case OM_NOT_AN_ENCODING:
            why = "OM_NOT_AN_ENCODING";
            break;

        case OM_NOT_CONCRETE:
            why = "OM_NOT_CONCRETE";
            break;

        case OM_NOT_PRESENT:
            why = "OM_NOT_PRESENT";
            break;

        case OM_NOT_PRIVATE:
            why = "OM_NOT_PRIVATE";
            break;

        case OM_NOT_THE_SERVICES:
            why = "OM_NOT_THE_SERVICES";
            break;

        case OM_PERMANENT_ERROR:
            why = "OM_PERMANENT_ERROR";
            break;

        case OM_POINTER_INVALID:
            why = "OM_POINTER_INVALID";
            break;

        case OM_SYSTEM_ERROR:
            why = "OM_SYSTEM_ERROR";
            break;

        case OM_TEMPORARY_ERROR:
            why = "OM_TEMPORARY_ERROR";
            break;

        case OM_TOO_MANY_VALUES:
            why = "OM_TOO_MANY_VALUES";
            break;

        case OM_VALUES_NOT_ADJACENT:
            why = "OM_VALUES_NOT_ADJACENT";
            break;

        case OM_WRONG_VALUE_LENGTH:
            why = "OM_WRONG_VALUE_LENGTH";
            break;

        case OM_WRONG_VALUE_MAKEUP:
            why = "OM_WRONG_VALUE_MAKEUP";
            break;

        case OM_WRONG_VALUE_POSITION:
            why = "OM_WRONG_VALUE_POSITION";
            break;

        case OM_WRONG_VALUE_NUMBER:
            why = "OM_WRONG_VALUE_NUMBER";
            break;

        case OM_WRONG_VALUE_SYNTAX:
            why = "OM_WRONG_VALUE_SYNTAX";
            break;

        case OM_WRONG_VALUE_TYPE:
            why = "OM_WRONG_VALUE_TYPE";
            break;

        default:
            why = "";
            break;
    } /* endswitch */

    TRACE_ERROR2("%s failed - %s\n", fn, why);
#endif /* TRACE */

    if (ec) {   
        if ((x_error = 
            ((xds_error *)OMP_XALLOC(workspace, AL_XDS_ERROR, 1))) == NULL) {
            TRACE_ERROR ("OMP_XALLOC( xds_error ) failed - OM_MEMORY_INSUFFICIENT")
            return (DS_NO_WORKSPACE);
        } /* endif */
        memset(x_error, 0, sizeof(xds_error));

        if (omP_create_handle(dsP_DS_C_LIBRARY_ERROR, workspace, x_error,
                                                &library_error) != OM_SUCCESS) {
            TRACE_ERROR ("omP_create_handle() failed - cannot create Library-Error object")
            return (DS_NO_WORKSPACE);
        } /* endif */
        x_error->val = ec;
		if ( 0 < ec && ec < DSP_ERROR_STRING_LEN) {
			TRACE_ERROR2 ("%s (%s) returned\n", "Library-Error", dsP_error_string[ec-1])
		} else {
			TRACE_ERROR2 ("%s (%d) returned\n", "unknown Library-Error", ec)
		} /* endif */
        return (library_error);
    } /* endif */
    else
        return(DS_SUCCESS);
} /* end dsP_handle_omP_error */





OM_sint
dsP_iapl2xds_error(
    dsP_error_entry     *err_tab,
    OM_sint                 entry_num,
    D2_pbhead           *pb,
    D2_error            *err,
    DS_status           *status,
    OM_workspace        workspace
)
{
    D2_acc_point        *acc;
    dsP_error_entry     *entry;
    unsigned long       indx;
    OM_string           *class;
    OM_sint                 simple = FALSE;
    OM_sint                 referral = FALSE;
    xds_error           *x_error;
    const char                *etype;
    
    if (pb->d2_errvalue == D2_ATT_ERROR)
        indx = XHASH(pb->d2_retcode, pb->d2_errclass, 
                  ((err->d2_e_problem).d2_e_atproblem)->d2_ae_prob->d2_ap_prob);
    else
        indx = XHASH(pb->d2_retcode, pb->d2_errclass, pb->d2_errvalue);

    do {
        if (dsP_find_ete_entry (err_tab, entry_num, indx, &entry) == OK) {
            switch (entry->type) {

                case XDS_REFERRAL:
                    class = &dsP_DS_C_REFERRAL; 
                    referral++;
                    break;

                case XDS_ATTRIBUTE_ERROR:
                    class = &dsP_DS_C_ATTRIBUTE_ERROR; 
                    etype = "Attribute-Error"; 
                    break;

                case XDS_NAME_ERROR:
                    class = &dsP_DS_C_NAME_ERROR; 
                    etype = "Name-Error"; 
                    break;

                case XDS_SERVICE_ERROR:
                    class = &dsP_DS_C_SERVICE_ERROR; 
                    etype = "Service-Error"; 
                    simple++;
                    break;

                case XDS_LIBRARY_ERROR:
                    class = &dsP_DS_C_LIBRARY_ERROR; 
                    etype = "Library-Error"; 
                    simple++;
                    break;

                case XDS_SECURITY_ERROR:
                    class = &dsP_DS_C_SECURITY_ERROR; 
                    etype = "Security-Error"; 
                    simple++;
                    break;

                case XDS_SYSTEM_ERROR:
                    class = &dsP_DS_C_SYSTEM_ERROR; 
                    etype = "System-Error"; 
                    entry->value = errno;
                    simple++;
                    break;

                case XDS_UPDATE_ERROR:
                    class = &dsP_DS_C_UPDATE_ERROR; 
                    etype = "Update-Error"; 
                    simple++;
                    break;

                case XDS_COMMUNICATIONS_ERROR:
                    class = &dsP_DS_C_COMMUNICATIONS_ERROR; 
                    etype = "Communications-Error"; 
                    simple++;
                    break;

                default:
                    TRACE_ERROR("invalid error type found in Error-table")
            } /* endswitch */

            CHECK_OMP_XALLOC(
                x_error,
                (xds_error *),
                XDS_ERROR,
                *status)

            if (simple)
                x_error->val = entry->value;
            else {
                if (!err)
                    TRACE_ERROR ("no xds_error returned")
                else {
                   x_error->ptr = err;
                   switch (err->d2_e_type) {
                   case D2_AT_PROBLEM: 
                      ((err->d2_e_problem).d2_e_atproblem)->d2_ae_prob->d2_ap_prob
                                                                 = entry->value;
                      break;

                   case D2_NM_PROBLEM: 
                      ((err->d2_e_problem).d2_e_nmproblem)->d2_ne_prob
                                                                 = entry->value;
                      break;

                   case D2_REF_PROBLEM: 
                       acc = ((err->d2_e_problem).d2_e_referral)->d2_acc_point;
                       while (acc) {
                           if (acc->d2_dsa_ap)
                               TRACE_WARN2("%s %s\n","referral to:", acc->d2_dsa_ap);
                           acc = acc->d2_next_ap;
                       } /* endwhile */
                       break;

                   default:
                             TRACE_WARN2 ("%s (%d)\n", "unexpected IAPL error-type encountered", err->d2_e_type);
                       return(NOTOK);
                } /* endswitch */
              } /* endif */
            } /* endif */

			if (! referral) {
				if ( 0 < entry->value && entry->value < DSP_ERROR_STRING_LEN) {
					TRACE_ERROR2 ("%s (%s) returned\n", etype, dsP_error_string[entry->value - 1])
				} else {
					TRACE_ERROR2 ("%s (%d) returned\n", etype, entry->value)
				} /* endif */
			} /* endif */

            CHECK_OMP_CALL(
                omP_create_handle,
                (*class, workspace, x_error, status),
                *status,
                DS_E_MISCELLANEOUS
            )
            return(OK);
        } /* endif */
    } while (FALSE);

    return(NOTOK);
} /* end dsP_iapl2xds_error */



OM_sint
dsP_find_ete_entry(
    dsP_error_entry     *et,
    OM_sint             entry_num,
    unsigned long       ul,
    dsP_error_entry     **entry
)
{
    dsP_error_entry     key;

    key.index = ul;
    if ((*entry = (dsP_error_entry *) bsearch(
                                    (char *)&key, 
                                    (char *)et, 
                                    entry_num, 
                                    sizeof(dsP_error_entry), 
                                    dsP_compare_ete_index)) == NULL)
        return (NOTOK);
    return (OK);    
} /* end dsP_find_ete_entry */



int
dsP_compare_ete_index(
    register const  void *pkey,
    register const  void *pentry
)
{
	 OM_uint32 key_index = ((dsP_error_entry *)pkey)->index;
	 OM_uint32 entry_index = ((dsP_error_entry *)pentry)->index;

    if (key_index > entry_index)
        return (1);
    else if (key_index < entry_index)
		return (-1);
	else
		return (0);
} /* end dsP_compare_ete_index */


OM_sint
dsP_validate_wkspace(
    OM_workspace   workspace,
    OM_sint            *index
)
{
     OM_sint    i;

    if (! workspace) {
        TRACE_ERROR("NULL workspace pointer")
        return(NOTOK);
    } /* endif */

    LOCK(dsP_wksptab_mutex)

    for (i=0; i < DSP_MAX_WKSPACES; i++)
        if (dsP_wkspace_table[i].default_session != 0)
            if (workspace== OMP_WORKSPACE(dsP_wkspace_table[i].default_session))
                break;

    UNLOCK(dsP_wksptab_mutex)

    if (i == DSP_MAX_WKSPACES) {
        TRACE_ERROR("workspace not initialized")
        return(NOTOK);
    } /* endif */

    *index = i;
    return(OK);
} /* end dsP_validate_wkspace */




OM_sint
dsP_validate_session(
    OM_private_object   session,
    OM_sint                 *bind_id,
    OM_sint                 *op_num
)
{
     OM_sint    i;

    if (dsP_verify_instance (session, dsP_DS_C_SESSION) != FALSE) {
        if (OMP_PRIVATE(session)) {

            if (! OMP_DATA(session)) {
                TRACE_ERROR("session has not been initialized")
                return(NOTOK);
            } /* endif */

            LOCK(dsP_bindtab_mutex)

            for (i=0; i < DSP_MAX_SESSIONS; i++)
                if (dsP_bind_table[i].bound_session == session) { 
                    *bind_id = dsP_bind_table[i].bind_id;
#ifdef THREADSAFE
                    *op_num = ++dsP_bind_table[i].op_num;
#endif
                    break;
                } /* endif */

            UNLOCK(dsP_bindtab_mutex)


            if (i == DSP_MAX_SESSIONS) {
                TRACE_ERROR("session not found in bind-table")
                return(NOTOK);
            } /* endif */
        } else {
            TRACE_ERROR("session is not a private object")
            return(NOTOK);
        } /* endif */
    } /* endif */
    else {
        TRACE_ERROR("session is not an instance of DS_C_SESSION")
        return(NOTOK);
    } /* endif */

    return(OK);
} /* end dsP_validate_session */





OM_sint
dsP_dec_op_num(
    OM_private_object  session
)
{
    OM_sint i;
    OM_sint rc = OK;

    LOCK(dsP_bindtab_mutex)

    for (i=0; i < DSP_MAX_SESSIONS; i++)
        if (dsP_bind_table[i].bound_session == session) {
            dsP_bind_table[i].op_num--;
            break;
        } /* endif */

    UNLOCK(dsP_bindtab_mutex)

    if (i == DSP_MAX_SESSIONS) {
        TRACE_ERROR("session not found in bind-table")
        rc = NOTOK;
    } /* endif */

    return(rc);
} /* end dsP_dec_op_num */





OM_sint
dsP_validate_context(
    OM_private_object   context
)
{

    if (dsP_verify_instance (context, dsP_DS_C_CONTEXT) != FALSE)  {
        if (OMP_PRIVATE(context)) {
            if (! OMP_DATA(context)) {
                TRACE_ERROR("context has not been initialized")
                return(NOTOK);
            } /* endif */
            TRACE_SERVICE_CONTROLS(((xds_context *)
                                             OMP_DATA(context))->d2_serv_cntrls)
        } else {
            TRACE_ERROR("context is not a private object")
            return(NOTOK);
        } /* endif */
    } else {
        TRACE_ERROR("context is not an instance of DS_C_CONTEXT")
        return(NOTOK);
    } /* endif */

    return(OK);
} /* end dsP_validate_context */





OM_sint
dsP_validate_name(
    OM_object           name,
    OM_private_object   *pri_name,
    DS_status           *status,
    OM_workspace        workspace
)
{
     OM_sint    rc = NOTOK;

    do {
        if (dsP_verify_instance (name, dsP_DS_C_NAME) != FALSE) {

            if (! (OMP_PRIVATE(name))) {

                CHECK_OMP_CALL(
                    om_create,
                    (dsP_DS_C_DS_DN, FALSE, workspace, pri_name),
                    *status,
                    DS_E_MISCELLANEOUS
                )
                rc = DONE - NOTOK;

                CHECK_OMP_CALL(
                    om_put,
                    (*pri_name, OM_REPLACE_ALL, name, OM_NO_MORE_TYPES, 0, 0),
                    *status,
                    DS_E_BAD_NAME
                )
                rc = DONE;
                TRACE_NOTE("name converted to a private object")
            } else {
                TRACE_NOTE("name is a private object")
                *pri_name = name;
                rc = OK;
            } /* endif */
        } else {
            TRACE_ERROR("name is not an instance of DS_C_NAME")

            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                *status,
                DS_E_BAD_NAME,
                DS_E_BAD_NAME
            )
        } /* endif */
    } while (FALSE);

    return(rc);
} /* end dsP_validate_name */





OM_sint
dsP_validate_selection(
    OM_object           selection,
    OM_private_object   *pri_selection,
    DS_status           *status,
    OM_workspace        workspace
)
{
     OM_sint    rc = NOTOK;

    do {
        if (dsP_verify_instance (selection, dsP_DS_C_ENTRY_INFO_SELECTION) != FALSE) {
            if (! (OMP_PRIVATE(selection))) {

                CHECK_OMP_CALL(
                    om_create,
                   (dsP_DS_C_ENTRY_INFO_SELECTION, 0, workspace, pri_selection),
                    *status,
                    DS_E_MISCELLANEOUS
                )
                rc = DONE - NOTOK;

                CHECK_OMP_CALL(
                    om_put,
                    (*pri_selection, OM_REPLACE_ALL, selection, 0, 0, 0),
                    *status,
                    DS_E_BAD_ARGUMENT
                )
                rc = DONE;
                TRACE_NOTE("selection converted to a private object")
            } else {
                TRACE_NOTE("selection is a private object")
                *pri_selection = selection;
                rc = OK;
                if (! OMP_DATA(selection)) {
                    TRACE_ERROR("selection has not been initialized")
                    rc = NOTOK;
                    CHECK_OMP_CREATE_ERROR(
                        dsP_DS_C_LIBRARY_ERROR,
                        *status,
                        DS_E_BAD_ARGUMENT,
                        DS_E_BAD_ARGUMENT
                    )
                } /* endif */
            } /* endif */
        } else {
            TRACE_ERROR("selection is not an instance of DS_C_ENTRY_INFO_SELECTION")

            CHECK_OMP_CREATE_ERROR(
                dsP_DS_C_LIBRARY_ERROR,
                *status,
                DS_E_BAD_ARGUMENT,
                DS_E_BAD_ARGUMENT
            )
        } /* endif */

    } while (FALSE);

    return (rc);

} /* end dsP_validate_selection */





/*
 *  XDS Filter structure  -->  IAPL Filter structure
 */
OM_sint
dsP_xds2iapl_filter(
    xds_filter    *xds_filt,
    D2_Fi_filter **iapl_filt
)
{
     OM_sint            miscellaneous = FALSE;
     xds_filter    *x_f = xds_filt;
     D2_Fi_filter  *i_f;
    OM_sint                     i_f_nxt = 1;
    OM_sint                     f_num = 0;

    do {
        dsP_count_filters(x_f, &f_num);

        if (f_num == 0) {
            miscellaneous++;
            continue;
        } /* endif */

        if (f_num == 1) {
            *iapl_filt = NULL;
            continue;
        }      
        if ((*iapl_filt = i_f =
                   (D2_Fi_filter *)calloc(f_num+1,sizeof(D2_Fi_filter))) == 0) {
            miscellaneous++;
            continue;
        } /* endif */
        i_f[f_num].d2_fi_opmod = (short) -1;
        if (dsP_fill_filters(x_f, i_f, 0, &i_f_nxt) == NOTOK) {
            miscellaneous++;
            continue;
        } /* endif */


    } while (FALSE);

    return (miscellaneous ? NOTOK : OK);
} /* end dsP_xds2iapl_filter */





OM_sint
dsP_count_filters(
    xds_filter    *xds_filt,
    OM_sint           *count
)
{
     xds_filter  *x_f = xds_filt;
     xds_fi_item *x_fi;

    if (x_f) {
        (*count)++;
        if (x_f->fil_item) {
            x_fi = x_f->fil_item;
            if (x_fi->f_item_next)
                (*count)++;
            while (x_fi) {
                (*count)++;
                x_fi = x_fi->f_item_next;
            } /* endwhile */
        } /* endif */
        if (x_f->filter_first)
            dsP_count_filters(x_f->filter_first, count);
        if (x_f->filter_next)
            dsP_count_filters(x_f->filter_next, count);
    } /* endif */

    return(OK);
} /* end dsP_count_filters */




OM_sint
dsP_fill_filters(
    xds_filter    *x_f,
    D2_Fi_filter  *i_f,
    OM_sint            i_f_cur,
    OM_sint           *i_f_nxt
)
{
     xds_filter  *x_f2;
     xds_fi_item *x_fi;
     OM_sint         i_f_nxt2;
             OM_sint         i_f_nxt3;
     OM_sint         num, numi;
     OM_sint         miscellaneous = FALSE;
     OM_sint         single_item;

    do {
        if (x_f) {
            switch (x_f->fi_type) {
                case DS_AND:
                case DS_OR:
                    i_f[i_f_cur].d2_fi_opmod = 
                                      (x_f->fi_type == DS_AND ? D2_AND : D2_OR);
                    i_f[i_f_cur].d2_fi_v.fi_and_or.d2_fi_filter= &i_f[*i_f_nxt];
                    break;

                case DS_NOT:
                    i_f[i_f_cur].d2_fi_opmod = D2_NOT; 
                    i_f[i_f_cur].d2_fi_v.d2_fi_not = &i_f[*i_f_nxt]; 
                    if ((x_f->fil_item && x_f->filter_first) ||
                        (x_f->fil_item && x_f->fil_item->f_item_next) ||
                        (x_f->filter_first && x_f->filter_first->filter_next)) {
                        miscellaneous++;
                        continue;
                    } /* endif */
                    break;

                default:
                    miscellaneous++;
                    continue;
                    break;
            } /* endswitch */

            if (x_fi = x_f->fil_item) {
                for (numi = 1; x_fi->f_item_next; numi++, x_fi = x_fi->f_item_next);
                if ((numi == 1) &&
                    ((!x_f->filter_first) &&
                     ((i_f[i_f_cur].d2_fi_opmod == D2_AND) ||
                      (i_f[i_f_cur].d2_fi_opmod == D2_OR)))) {
                    single_item = TRUE;
                    i_f_nxt3 = i_f_cur;
                } else {
                    single_item = FALSE;
                    i_f_nxt3 = *i_f_nxt;
                } /* endif */

                if (dsP_fill_filter_items(x_f->fil_item,i_f,&i_f_nxt3)!=OK){
                        miscellaneous++;
                        continue;
                } /* endif */
                if (!single_item) {
                    if ((i_f[i_f_cur].d2_fi_opmod == D2_AND) ||
                        (i_f[i_f_cur].d2_fi_opmod == D2_OR)) {
                        i_f[i_f_cur].d2_fi_v.fi_and_or.d2_fi_no_fi = numi;
                    } /* endif */
                    *i_f_nxt = i_f_nxt3;
                } /* endif */
            } /* endif */

            if (x_f2 = x_f->filter_first) {
                for (num = 1; x_f2->filter_next; num++,x_f2 = x_f2->filter_next)
                    ;

                i_f_nxt2 = *i_f_nxt;
                *i_f_nxt += num;

                if ((i_f[i_f_cur].d2_fi_opmod == D2_AND) ||
                    (i_f[i_f_cur].d2_fi_opmod == D2_OR))
                    i_f[i_f_cur].d2_fi_v.fi_and_or.d2_fi_no_fi += num;

                dsP_fill_filters(x_f->filter_first, i_f, i_f_nxt2, i_f_nxt);
            } /* endif */

            i_f_cur++;

            if (x_f->filter_next) {
                x_f = x_f->filter_next;
                dsP_fill_filters(x_f, i_f, i_f_cur, i_f_nxt);
            } /* endif */

        } /* endif */
    } while (FALSE);

    return (miscellaneous ? NOTOK : OK);
} /* end dsP_fill_filters */




OM_sint
dsP_fill_filter_items(
    xds_fi_item  *x_fi,
    D2_Fi_filter *i_f,
    OM_sint          *i_f_nxt
)
{
     D2_Fi_item *i_fi;
     OM_sint         inx = *i_f_nxt;
     OM_sint         i;
     OM_sint         miscellaneous = FALSE;

    while ((x_fi) && (!miscellaneous)) {

        i_f[inx].d2_fi_opmod = D2_ITEM;
        i_fi = &(i_f[inx].d2_fi_v.d2_fi_item);


        i_fi->d2_fi_match           = x_fi->match_type;
        i_fi->d2_fi_type.d2_typ_len = x_fi->f_item_att.d2_a_type.d2_typ_len;

        /* TBD: adjust pointer (or copy bytes) */
        i_fi->d2_fi_type.d2_type = x_fi->f_item_att.d2_a_type.d2_type;

        switch (x_fi->match_type) {

            case D2_EQUAL:
            case D2_GTEQ:
            case D2_LTEQ:
            case D2_APPROX_MATCH:
                if ((x_fi->f_item_att.d2_a_no_val != 1) || 
                    (! x_fi->f_item_att.d2_a_val)) {
                    miscellaneous++;
                    continue;
                } /* endif */
                memcpy (&i_fi->d2_fi_val, x_fi->f_item_att.d2_a_val,
                                                            sizeof(D2_a_value));
                break;

            case D2_PRESENT:
                break;

            case D2_SUBSTRINGS:
            {
                 OM_sint            offset = 0;
                 OM_sint            str_num = -1;
                 OM_sint            val_num = x_fi->f_item_att.d2_a_no_val;
                 D2_Fi_substr   *i_fs=&i_fi->d2_fi_val.d2_fi_substrings;
                 D2_Fi_substr   *i_fs_blk;
                 D2_a_value     *x_av = x_fi->f_item_att.d2_a_val;

                if (x_fi->f_ini_sstr.d2_a_v_len) str_num++;
                if (x_fi->f_lst_sstr.d2_a_v_len) str_num++;
                if ((str_num += val_num) > 0) {

                    if ((i_fs_blk =
                   (D2_Fi_substr *)calloc(str_num,sizeof(D2_Fi_substr))) == 0) {
                        miscellaneous++;
                        continue;
                    } /* endif */
                    i_fs->d2_fi_next = i_fs_blk;
                } /* endif */
                else
                    i_fs->d2_fi_next = NULL;

                if (x_fi->f_ini_sstr.d2_a_v_len) {
                    i_fs->d2_fi_pos = D2_INITIAL;
                    memcpy (&i_fs->d2_fi_str_val, &x_fi->f_ini_sstr,
                                                            sizeof(D2_a_value));
                    i_fs = i_fs->d2_fi_next;
                    offset++;
                } /* endif */

                for (i = 0; i < val_num; i++, i_fs = i_fs->d2_fi_next) {
                    
                    i_fs->d2_fi_pos = D2_ANY;
                    memcpy (&i_fs->d2_fi_str_val, &x_av[i], sizeof(D2_a_value));
                    if (str_num) 
                        i_fs->d2_fi_next = &i_fs_blk[i+offset];
                    else
                        i_fs->d2_fi_next = NULL;
                } /* endfor */
/* VR           if (str_num)
                    i_fs = i_fs->d2_fi_next; */


                if (x_fi->f_lst_sstr.d2_a_v_len) {
                    i_fs->d2_fi_pos = D2_FINAL;
                    memcpy (&i_fs->d2_fi_str_val, &x_fi->f_lst_sstr,
                                                            sizeof(D2_a_value));
                    i_fs->d2_fi_next = NULL;
                } /* endif */
                if (str_num < 0)
                    miscellaneous++;
                else if (str_num)
                    i_fs_blk[str_num-1].d2_fi_next = NULL;
                break;
            } /* endblock */

            default:
                miscellaneous++;
                continue;
        } /* endswitch */

        (*i_f_nxt)++; inx++;
        x_fi = x_fi->f_item_next;
    } /* endwhile */

    return (miscellaneous ? NOTOK : OK);
} /* end dsP_fill_filter_items */


OM_sint
dsP_free_iapl_filter(
        D2_Fi_filter    *iapl_filt)
{
     D2_Fi_filter     *i_f = iapl_filt;

    for (; 
         (i_f->d2_fi_opmod != (short) -1); 
         i_f++)

        if ((i_f->d2_fi_opmod == D2_ITEM) &&
            (i_f->d2_fi_v.d2_fi_item.d2_fi_match == D2_SUBSTRINGS) &&
        (i_f->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_next))
    free(i_f->d2_fi_v.d2_fi_item.d2_fi_val.d2_fi_substrings.d2_fi_next);

    free(iapl_filt);
} /* end dsP_free_iapl_filter */


void
dsP_init_once(
    void)
{
    char *evarstr;

#ifdef THREADSAFE
    if (pthread_mutex_init(&dsP_wksptab_mutex, pthread_mutexattr_default) != 0)
        TRACE_ERROR("cannot initialize Workspace-table mutex")

    if (pthread_mutex_init(&dsP_bindtab_mutex, pthread_mutexattr_default) != 0)
        TRACE_ERROR("cannot initialize Bind-table mutex")
#endif /* THREADSAFE */

    if ((evarstr = getenv("XDS_DEF_AUTH_MECH")) != NULL)
        dsP_def_auth_mech = atoi(evarstr);

} /* end dsP_init_once */


#ifdef THREADSAFE

/* return the thread-id (integer) of the calling thread */
OM_sint
dsP_thread_id(
    void)
{
    pthread_t me;

    me = pthread_self();
    return(pthread_getunique_np(&me));
} /* end dsP_thread_id */
#endif
