/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_tree.c,v $
 * Revision 1.1.10.2  1996/02/18  00:25:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:12:20  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:14:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:35  root]
 * 
 * Revision 1.1.8.2  1994/03/23  16:06:54  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:14:16  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:16:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:33:12  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  12:42:32  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:31:33  marrek]
 * 
 * Revision 1.1.4.2  1993/01/05  20:35:10  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:49:41  htf]
 * 
 * Revision 1.1.2.2  1992/06/11  20:02:54  melman
 * 	gds drop
 * 	[1992/06/11  19:27:00  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xt_tree.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 00:25:00 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : xt_tree.c                                           */
/*                                                                    */
/* AUTHOR       : Cahill             D AP11                           */
/* DATE         : 07.00.1987                                          */
/*                                                                    */
/* KOMPONENTE   : XDS/XOM automatic test program                      */
/*                                                                    */
/* DOK.-NR.     :                                                     */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEMABHAENGIGKEIT: -                                             */
/*                                                                    */
/* HISTORIE     :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  | Aenderungen                    | KZ | CR# FM# */
/* 1.0      | 13.07.90| original                       |br  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*          Module identifikation                                     */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*            I N C L U D E S                                         */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
	#include <pthread.h>
#endif
#include <stdio.h>
#include <string.h>
#include <xt.h>
#ifdef THREADSAFE
	#include <xt_thread.h>
#else
	#include <xt_nonthread.h>
#endif
#include <xt_tree.h>

/****************************************************************************/
/*   jmptbl - when a function name is read from the testcase file, it is    */
/*            matched against the entname field of this structure, and      */
/*            test_func is invoked                                          */
/****************************************************************************/

extern struct jump_struct jmptbl[];

extern struct parm_tbl_struct parameter_tbl[];

extern OM_descriptor        *context_tbl[];
extern OM_descriptor        *name_tbl[];
extern OM_descriptor        *entry_tbl[];
extern OM_descriptor        *ava_tbl[];
extern OM_descriptor        *changes_tbl[];
extern OM_descriptor        *new_rdn_tbl[];
extern OM_descriptor        *filter_tbl[];
extern OM_descriptor        *selection_tbl[];
extern OM_descriptor        *read_result_tbl[];
extern OM_descriptor        *status_tbl[];
extern OM_descriptor        *compare_result_tbl[];
extern OM_descriptor        *list_result_tbl[];
extern OM_descriptor        *entry_modification_list_tbl[];
extern OM_descriptor        *entry_info_selection_tbl[];
extern OM_sint              subset_tbl[];
extern OM_uint32            boolean_tbl[];
extern OM_descriptor        *search_result_tbl[];
extern DS_feature           *feature_list_tbl[];
extern OM_object_identifier     *rules_tbl[];
extern OM_return_code       return_code_tbl[];
extern OM_object        *object_tbl[];
extern struct class_tbl_struct class_tbl[];
extern OM_type          type_tbl[];
extern OM_type_list     type_list_tbl[];
extern OM_exclusions        exclusions_tbl[];
extern OM_modification      modification_tbl[];
extern OM_syntax        syntax_tbl[];



OM_IMPORT (DS_C_ATTRIBUTE_LIST)
OM_IMPORT (DS_C_ATTRIBUTE)
OM_IMPORT (DS_C_AVA)

OM_IMPORT (DS_A_OBJECT_CLASS)
OM_IMPORT (DS_A_USER_PASSWORD)
OM_IMPORT (DS_A_SURNAME)
OM_IMPORT (DS_A_PHONE_NBR)
OM_IMPORT (DS_A_BUSINESS_CATEGORY)

OM_IMPORT (DS_O_COUNTRY)
OM_IMPORT (DS_O_ORG)
OM_IMPORT (DS_O_ORG_UNIT)
OM_IMPORT (DS_O_ORG_PERSON)

char	countries[] = {

	"AD AE AF AG AI AL AN AO AQ AR AS AT AU AW "
	"BB BD BE BF BG BH BI BJ BM BN BO BR BS BT BU BV BW BY BZ "
	"CA CC CF CG CH CI CK CL CM CN CO CR CS CT CU CV CX CY "
	"DD DE DJ DK DM DO DZ "
	"EC EG EH ES ET "
	"FI FJ FK FM FO FR "
	"GA GB GD GF GH GI GL GM GN GP GQ GR GT GU GW GY "
	"HK HM HN HT HU HV "
	"ID IE IL IN IO IQ IR IS IT "
	"JM JO JP JT "
	"KE KH KI KM KN KP KR KW KY "
	"LA LB LC LI LK LR LS LU LY "
	"MA MC MG MH MI ML MN MO MP MQ MR MS MT MU MV MW MX MY MZ "
	"NA NC NE NF NG NI NL NO NP NQ NR NT NU NZ "
	"OM "
	"PA PC PE PF PG PH PK PL PM PN PR PT PU PW PY "
	"QA "
	"RE RO RW "
	"SA SB SC SD SE SG SH SJ SL SM SN SO SR ST SU SV SY SZ "
	"TC TD TF TG TH TK TN TO TP TR TT TV TW TZ "
	"UA UG UM US UY "
	"VA VC VE VG VI VN VU "
	"WF WK WS "
	"YD YE YU "
	"ZA ZM ZR ZW "
};


/* country */
static OM_descriptor    attribute_c[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
	OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_COUNTRY),
	OM_NULL_DESCRIPTOR
};


static OM_descriptor    attribute_list_c[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, attribute_c } },         
	OM_NULL_DESCRIPTOR
};


/* organization */
static OM_descriptor    attribute_o[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
	OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG),
	OM_NULL_DESCRIPTOR
};


static OM_descriptor    attribute_bc[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_BUSINESS_CATEGORY),
	{ DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("computer systems") },
	OM_NULL_DESCRIPTOR
};


static OM_descriptor    attribute_list_o[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, attribute_o } },                 
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, attribute_bc } },                 
	OM_NULL_DESCRIPTOR
};


/* organizational_unit */
static OM_descriptor    attribute_ou[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
	OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_UNIT),
	OM_NULL_DESCRIPTOR
};


static OM_descriptor    attribute_tn[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
	{ DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+353 1 767551") },
	OM_NULL_DESCRIPTOR
};


static OM_descriptor    attribute_list_ou[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, attribute_ou } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, attribute_tn } },
	OM_NULL_DESCRIPTOR
};


/* organizational_person */
static OM_descriptor    attribute_op[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
	OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON),
	OM_NULL_DESCRIPTOR
};


static OM_descriptor    surname1[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
	{ DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Ryan") },
	OM_NULL_DESCRIPTOR
};


static OM_descriptor    user_password1[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
	{ DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret") },
	OM_NULL_DESCRIPTOR
};


static OM_descriptor    attribute_list_op[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, attribute_op     } },     
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, surname1     } },     
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, user_password1       } },     
	OM_NULL_DESCRIPTOR 
};


/* empty attribute list */
static OM_descriptor    attribute_list_empty[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	OM_NULL_DESCRIPTOR
};


/****************************************************************************/
/*                                                                          */
/*          xt_build_tree()                     */
/*                                                                          */
/****************************************************************************/

OM_sint	xt_build_tree (void)
{
	OM_sint	i = 0;
	OM_sint	retval;
	char	parm[64];
	OM_sint	parm1;
	OM_sint	is_void;

	/* read session parameter from the testcase file */
	if (xt_read_parm (XT_PARAMETER, XT_SESSION, &parm1, parm, &is_void) == XT_ERROR) {
		xt_err ("xt_build_tree", "Error reading bound_session");
		return XT_ERROR ;
	} /* endif */
	bt_session = session_tbl[parm1];
	xt_log1 ("\tsession = %s", parm);


	/* read context parameter from the testcase file */
	if (xt_read_parm (XT_PARAMETER, XT_CONTEXT, &parm1, parm, &is_void) != XT_OK) {
		xt_err ("xt_build_tree", "Error reading context parameter");
		return(XT_ERROR);
	} /* endif */
	xt_log1 ("\tcontext = %s", parm);

	if (!(parm1 == 0)) {
		OM_private_object local_bt_context = bt_context;
		if (om_put (local_bt_context, OM_REPLACE_ALL, context_tbl[parm1], (OM_type_list) NULL, ((OM_value_position) NULL), ((OM_value_position) NULL)) != OM_SUCCESS)  {
			xt_err ("xt_build_tree", "Error calling om_put for context");
			return(XT_ERROR);
		} /* endif */
	} else
		bt_context = DS_DEFAULT_CONTEXT;


	/* read the entry names and numbers from the testcase file */
	while (((retval = xt_read_equals (&bt_parms[i].entry_type,  &bt_parms[i].number)) != XT_EOF) &&  (retval != XT_END_INPUT)) {
		if (strcmp(bt_parms[i].entry_type, "c") &&  strcmp(bt_parms[i].entry_type, "C") &&  strcmp(bt_parms[i].entry_type,
		     "o") &&  strcmp(bt_parms[i].entry_type, "O") &&  strcmp(bt_parms[i].entry_type, "ou") &&  strcmp(bt_parms[i].entry_type,
		     "OU") &&  strcmp(bt_parms[i].entry_type, "op") &&  strcmp(bt_parms[i].entry_type, "OP")) {
			xt_err1 ("xt_build_tree", "type  '%s'  in testcase file, not recognised", bt_parms[i].entry_type);
			xt_log1 ("\ttype '%s' in testcase file, not recognised", bt_parms[i].entry_type);
			return(XT_ERROR);
		} /* endif */
		if ((!strcmp(bt_parms[i].entry_type, "c") ||  !strcmp(bt_parms[i].entry_type, "C")) &&  (bt_parms[i].number >
		    229)) {
			xt_err ("xt_build_tree", "max of 229 entries for 'c' allowed");
			xt_log ("\tmax of 229 entries for type 'c' allowed");
			return(XT_ERROR);
		} /* endif */
		xt_log2 ("\ttype '%s' = %s", bt_parms[i].entry_type, xt_itoa(bt_parms[i].number));
		i++;
	} /* endwhile */

	if (i == 0) {
		xt_err ("xt_build_tree", "No BUILD_TREE parameters read from file");
		return(XT_ERROR);
	} /* endif */

	bt_last = i;

	XT_PRINTF("adding entries....\n");

	if (xt_add_entries (0, "") == XT_ERROR) {
		xt_err ("xt_build_tree", "Error calling xt_add_entries[0]");
		return XT_ERROR;
	} /* endif */

	if (bt_parms[i].entry_type)
		free (bt_parms[i].entry_type);

	xt_log ("\tds_build_tree() returned OK\n");
	return (XT_OK);

} /* end xt_build_tree */


OM_sint	xt_add_entries (OM_sint last, char instr[30])
{
	DS_status       status_return;
	OM_sint	i = 0;
	char	type[4];
	char	string[10];
	char	newstr[30];
	OM_sint	number, desc;
	OM_sint     invokeid_return;
	OM_object       name;
	OM_object       attribute_list;

	if (last < bt_last) {
		strcpy(type, bt_parms[last].entry_type);
		number = bt_parms[last].number;
		i = 1;

		while (i <= number) {

			if (!strcmp(type, "c") || !strcmp(type, "C")) {
				desc = OMP_O_DS_A_COUNTRY_NAME[2];
				attribute_list = attribute_list_c;
			} /* endif */
			if (!strcmp(type, "o") || !strcmp(type, "O")) {
				desc = OMP_O_DS_A_ORG_NAME[2];
				attribute_list = attribute_list_o;
			} /* endif */
			if (!strcmp(type, "ou") || !strcmp(type, "OU")) {
				desc = OMP_O_DS_A_ORG_UNIT_NAME[2];
				attribute_list = attribute_list_ou;
			} /* endif */
			if (!strcmp(type, "op") || !strcmp(type, "OP")) {
				desc = OMP_O_DS_A_COMMON_NAME[2];
				attribute_list = attribute_list_op;
			} /* endif */

			/* mac - must have an empty attribute_list */
			/* when running under fantou */

			/* attribute_list = attribute_list_empty; */

			/* If the type is "c" then have to choose one of the predefined
               countries, from the country_tbl, the first country is "AD",
               the second "AE", to "ZW" if required. For the other types,
               "o", "ou" and "op", can concatenate an integer to the type to
               find the new value eg. "o1", "ou5", "op31".
             */
			if (!strcmp(type, "c") || !strcmp(type, "C")) {
				string[0] = countries[(i-1)*3];
				string[1] = countries[((i-1)*3)+1];
				string[2] = '\0';
			} else {
				strcpy (string, type);
				strcat (string, xt_itoa(i));
			} /* end if */

			if (!strcmp(type, "c") || !strcmp(type, "C"))
				sprintf (newstr, "%s/C=%s", instr, string);
			if (!strcmp(type, "o") || !strcmp(type, "O"))
				sprintf (newstr, "%s/O=%s", instr, string);
			if (!strcmp(type, "ou") || !strcmp(type, "OU"))
				sprintf (newstr, "%s/OU=%s", instr, string);
			if (!strcmp(type, "op") || !strcmp(type, "OP"))
				sprintf (newstr, "%s/OP=%s", instr, string);

			/* build an ava */
			if (((bt_ava[last]) =  calloc(4, sizeof(OM_descriptor))) == NULL) {
				xt_err1("xt_add_entries", "Error allocating bt_ava[%s]", xt_itoa(last));
				return XT_ERROR;
			} /* endif */
			if (xt_build_ava (bt_ava[last], string, desc) != XT_OK) {
				xt_err1("xt_add_entries", "error building bt_ava[%s]", xt_itoa(last));
				return XT_ERROR;
			} /* endif */

			/* build an rdn */
			if (((bt_rdn[last]) =  calloc(3, sizeof(OM_descriptor))) == NULL) {
				xt_err1("xt_add_entries", "Error allocating bt_rdn[%s]", xt_itoa(last));
				return XT_ERROR;
			} /* endif */
			if (xt_build_rdn(bt_rdn[last], bt_ava[last]) != XT_OK) {
				xt_err1("xt_add_entries", "error building bt_rdn[%s]", xt_itoa(last));
				return XT_ERROR;
			} /* endif */

			/* build a name object */
			if ((name = calloc(last + 3, sizeof(OM_descriptor))) == NULL) {
				xt_err("xt_add_entries", "Error allocating name");
				return XT_ERROR;
			} /* endif */
			if (xt_build_name(name, bt_rdn, last + 1, 0) != XT_OK) {
				xt_err ("xt_add_entries", "error building name");
				return(XT_ERROR);
			} /* endif */

			status_return = ds_add_entry ( bt_session, bt_context, name, attribute_list, &invokeid_return );


			if (status_return != DS_SUCCESS) {
				xt_err ("xt_add_entries", "ds_add_entry did not return DS_SUCCESS");
				xt_log1 ("\t(WARNING) cannot add %s", newstr);
				XT_PRINTF ("cannot add %s\n", newstr);
				return(XT_ERROR);
			} else {
				xt_log1 ("\tadded %s", newstr);
				XT_PRINTF ("added %s\n", newstr);
			} /* endif */

			if (name)
				free(name);

			if (xt_add_entries (last + 1, newstr) == XT_ERROR) {
				xt_err1("xt_add_entries", "error calling xt_add_entries[%s]", xt_itoa(last + 1));
				return XT_ERROR;
			} /* endif */

			if (bt_ava[last])
				free(bt_ava[last]);
			if (bt_rdn[last])
				free(bt_rdn[last]);

			i++;
		} /* endwhile */
	} /* endif */
} /* end xt_add_entries */


/****************************************************************************/
/*                                                                          */
/*          xt_remove_tree()                    */
/*                                                                          */
/****************************************************************************/

OM_sint	xt_remove_tree () {
	OM_sint	i = 0;
	OM_sint	retval;
	char	parm[64];
	OM_sint	parm1, parm2;
	OM_sint	is_void;


	/* read session parameter from the testcase file */
	if (xt_read_parm (XT_PARAMETER, XT_SESSION, &parm1, parm, &is_void) == XT_ERROR) {
		xt_err ("xt_remove_tree", "Error reading bound_session");
		return XT_ERROR ;
	} /* endif */
	rt_session = session_tbl[parm1];
	xt_log1 ("\tsession = %s", parm);


	/* read context parameter from the testcase file */
	if (xt_read_parm (XT_PARAMETER, XT_CONTEXT, &parm1, parm, &is_void) != XT_OK) {
		xt_err ("xt_remove_tree", "Error reading context parameter");
		return(XT_ERROR);
	} /* endif */
	xt_log1 ("\tcontext = %s", parm);

	if (!(parm1 == 0)) {
		OM_private_object local_rt_context = rt_context;
		if (om_put (local_rt_context, OM_REPLACE_ALL, context_tbl[parm1], (OM_type_list) NULL, ((OM_value_position) NULL),
		     ((OM_value_position) NULL)) != OM_SUCCESS)  {
			xt_err ("xt_remove_tree", "Error calling om_put for context");
			return(XT_ERROR);
		} /* endif */
	} else
		rt_context = DS_DEFAULT_CONTEXT;

	/* read the entry names and numbers from the testcase file */
	while (((retval = xt_read_equals (&rt_parms[i].entry_type,  &rt_parms[i].number)) != XT_EOF) &&  (retval != XT_END_INPUT)) {
		if (strcmp(rt_parms[i].entry_type, "c") &&  strcmp(rt_parms[i].entry_type, "C") &&  strcmp(rt_parms[i].entry_type,
		     "o") &&  strcmp(rt_parms[i].entry_type, "O") &&  strcmp(rt_parms[i].entry_type, "ou") &&  strcmp(rt_parms[i].entry_type,
		     "OU") &&  strcmp(rt_parms[i].entry_type, "op") &&  strcmp(rt_parms[i].entry_type, "OP")) {
			xt_err1 ("xt_remove_tree", "type  '%s'  in testcase file, not recognised", rt_parms[i].entry_type);
			xt_log1 ("\ttype '%s' in testcase file, not recognised", rt_parms[i].entry_type);
			return(XT_ERROR);
		} /* endif */
		if ((!strcmp(rt_parms[i].entry_type, "c") ||  !strcmp(rt_parms[i].entry_type, "C")) &&  (rt_parms[i].number >
		    229)) {
			xt_err ("xt_remove_tree", "max of 229 entries for 'c' allowed");
			xt_log ("\tmax of 9 entries for type 'c' allowed");
			return(XT_ERROR);
		} /* endif */
		xt_log2 ("\ttype '%s' = %s", rt_parms[i].entry_type, xt_itoa(rt_parms[i].number));
		i++;
	} /* endwhile */


	if (i == 0) {
		xt_err ("xt_remove_tree", "No BUILD_TREE parameters read from file");
		return(XT_ERROR);
	} /* endif */

	rt_last = i;

	XT_PRINTF("removing entries....\n");

	if (xt_remove_entries (0, "") == XT_ERROR) {
		xt_err ("xt_remove_tree", "Error calling xt_add_entries[0]");
		return XT_ERROR;
	} /* endif */

	if (rt_parms[i].entry_type)
		free (rt_parms[i].entry_type);

	xt_log ("\tds_remove_tree() returned OK\n");
	return (XT_OK);

} /* end xt_remove_tree  */


OM_sint	xt_remove_entries (OM_sint last, char instr[30])
{
	DS_status       status_return;
	OM_sint	i = 0;
	OM_sint	retval;
	char	type[4];
	char	string[10];
	char	newstr[30];
	OM_sint	number, desc;
	OM_sint     invokeid_return;
	OM_object       name;

	if (last < rt_last) {
		strcpy(type, rt_parms[last].entry_type);
		number = rt_parms[last].number;
		i = 1;

		while (i <= number) {

			if (!strcmp(type, "c") || !strcmp(type, "C"))
				desc = OMP_O_DS_A_COUNTRY_NAME[2];
			if (!strcmp(type, "o") || !strcmp(type, "O"))
				desc = OMP_O_DS_A_ORG_NAME[2];
			if (!strcmp(type, "ou") || !strcmp(type, "OU"))
				desc = OMP_O_DS_A_ORG_UNIT_NAME[2];
			if (!strcmp(type, "op") || !strcmp(type, "OP"))
				desc = OMP_O_DS_A_COMMON_NAME[2];

			/* If the type is "c" then have to choose one of the predefined countries,
   from the country_tbl, the first country is "AD", the second "AE", to "ZW"
   if required. For the other types, "o", "ou" and "op", can concatenate an
   integer to the type to find the new value eg. "o1", "ou5", "op31".
 */
			if (!strcmp(type, "c") || !strcmp(type, "C")) {
				string[0] = countries[(i-1)*3];
				string[1] = countries[((i-1)*3)+1];
				string[2] = '\0';
			} else {
				strcpy (string, type);
				strcat (string, xt_itoa(i));
			} /* endif */

			if (!strcmp(type, "c") || !strcmp(type, "C"))
				sprintf (newstr, "%s/C=%s", instr, string);
			if (!strcmp(type, "o") || !strcmp(type, "O"))
				sprintf (newstr, "%s/O=%s", instr, string);
			if (!strcmp(type, "ou") || !strcmp(type, "OU"))
				sprintf (newstr, "%s/OU=%s", instr, string);
			if (!strcmp(type, "op") || !strcmp(type, "OP"))
				sprintf (newstr, "%s/OP=%s", instr, string);

			/* build an ava */
			if (((rt_ava[last]) =  calloc(4, sizeof(OM_descriptor))) == NULL) {
				xt_err1("xt_remove_entries", "Error allocating rt_ava[%s]", xt_itoa(last));
				return XT_ERROR;
			} /* endif */
			if (xt_build_ava (rt_ava[last], string, desc) != XT_OK) {
				xt_err1("xt_remove_entries", "error building rt_ava[%s]", xt_itoa(last));
				return XT_ERROR;
			} /* endif */

			/* build an rdn */
			if (((rt_rdn[last]) =  calloc(3, sizeof(OM_descriptor))) == NULL) {
				xt_err1("xt_remove_entries", "Error allocating rt_rdn[%s]", xt_itoa(last));
				return XT_ERROR;
			} /* endif */
			if (xt_build_rdn(rt_rdn[last], rt_ava[last]) != XT_OK) {
				xt_err1("xt_remove_entries", "error building rt_rdn[%s]", xt_itoa(last));
				return XT_ERROR;
			} /* endif */

			/* build a name object */
			if ((name = calloc(last + 3, sizeof(OM_descriptor))) == NULL) {
				xt_err("xt_remove_entries", "Error allocating name");
				return XT_ERROR;
			} /* endif */
			if (xt_build_name(name, rt_rdn, last + 1, 0) != XT_OK) {
				xt_err ("xt_remove_entries", "error building name");
				return(XT_ERROR);
			} /* endif */

			if (xt_remove_entries (last + 1, newstr) == XT_ERROR) {
				xt_err1("xt_remove_entries", "error calling xt_remove_entries[%s]", xt_itoa(last + 1));
				return XT_ERROR;
			} /* endif */

			status_return = ds_remove_entry ( rt_session, rt_context, name, &invokeid_return );


			if (status_return != DS_SUCCESS) {
				xt_log1 ("\t(WARNING) cannot remove %s", newstr);
				XT_PRINTF ("cannot find/remove %s\n", newstr);
			} else {
				xt_log1 ("\tremoved %s", newstr);
				XT_PRINTF ("removed %s\n", newstr);
			} /* endif */

			if (name)
				free(name);
			if (rt_ava[last])
				free(rt_ava[last]);
			if (rt_ava[last])
				free(rt_rdn[last]);

			i++;
		} /* endwhile */
	} /* endif */
} /* end xt_remove_entries  */
