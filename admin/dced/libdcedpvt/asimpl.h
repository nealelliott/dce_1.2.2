/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: asimpl.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:17  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:48:49  root]
 * 
 * Revision 1.1.2.5  1994/09/27  17:14:58  pwang
 * 	Added VALID_ATTR_SCH_NAME and fixed some problems in existing
 * 	macros [OT#12321, 12322, 12324]
 * 	[1994/09/27  16:49:36  pwang]
 * 
 * Revision 1.1.2.4  1994/09/08  21:52:48  pwang
 * 	Replaced '&' with '==' in the macros [OT#11840]
 * 	[1994/09/08  21:51:16  pwang]
 * 
 * Revision 1.1.2.3  1994/09/08  20:08:15  pwang
 * 	Added macros to validate the fields of schema entry [OT#11840]
 * 	[1994/09/08  20:01:22  pwang]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:03  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:55:57  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:41  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:15  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#ifndef _ASIMPL_H
#define _ASIMPL_H

/*
 * VALID_SCH_ATTR_NAME: (n) is schema_entry->name
 * Determine if (n) is non-null name
 */  
#define VALID_SCH_ATTR_NAME(n)	(n && (*n != '\0'))

/*
 * VALID_SCH_OBJ_NAME: (n) is attr schema object name 
 * Determine if (n) is supported by dced 
 */  
#define VALID_ATTR_SCH_NAME(n)	(!n || (*n == '\0') || \
				 !strcmp((char *)n, dced_c_service_attr_sch))

/*
 * VALID_SCH_ENCODE_TYPE: (e) is of type schema_entry->encoding_type
 * Determine if (e) is a valid encoding type for the schema_entry
 * encoding_type field.
 * Note: sec_attr_enc_trig_binding is not valid in a schema entry,
 * it's only used in returned attribute instances for trigger ERAs.
 */
#define VALID_SCH_ENCODE_TYPE(e) ((e) == sec_attr_enc_any || \
    				  (e) == sec_attr_enc_void || \
			          (e) == sec_attr_enc_printstring || \
    				  (e) == sec_attr_enc_printstring_array || \
    				  (e) == sec_attr_enc_integer || \
				  (e) == sec_attr_enc_bytes || \
    				  (e) == sec_attr_enc_confidential_bytes || \
    				  (e) == sec_attr_enc_i18n_data || \
				  (e) == sec_attr_enc_uuid || \
    				  (e) == sec_attr_enc_attr_set || \
				  (e) == sec_attr_enc_binding ? TRUE : FALSE)

/*
 * VALID_ACTION: (a) is of type schema_entry->intercell_action
 * Determine if (a) is a valid intercell action type 
 * In dced, currently it is ignored.
 */
#define VALID_SCH_ACTION(a) \
			 ((a) == 0 || \
                          (a) == sec_attr_intercell_act_accept || \
			  (a) == sec_attr_intercell_act_reject || \
		          (a) == sec_attr_intercell_act_evaluate ? TRUE : FALSE)

/*
 * VALID_SCH_FLAG: (f) is of type schema_entry->schema_entry_flags
 * Determine if (f) is a valid  schema entry flag
 */
#define VALID_SCH_FLAG(f) \
  ((f) == sec_attr_sch_entry_none || \
   ((f) & sec_attr_sch_entry_unique) == sec_attr_sch_entry_unique || \
   ((f) & sec_attr_sch_entry_multi_inst) == sec_attr_sch_entry_multi_inst || \
   ((f) & sec_attr_sch_entry_reserved) == sec_attr_sch_entry_reserved || \
   ((f) & sec_attr_sch_entry_use_defaults) == \
                               sec_attr_sch_entry_use_defaults ? TRUE : FALSE) 
				

/*
 * VALID_SCH_TRIG_TYPES: (t) is of type schema_entry->trig_types
 * Determine if (t) is a valid trig type
 * In dced, currently it is ignored.
 */
#define VALID_SCH_TRIG_TYPES(t) \
 ((t) == sec_attr_trig_type_none || \
  ((t) & sec_attr_trig_type_query) == sec_attr_trig_type_query || \
  ((t) & sec_attr_trig_type_update) == sec_attr_trig_type_update ? TRUE : FALSE)

/*
 * VALID_SCH_TRIG_BINDING: (b) is of type schema_entry->trig_binding
 * Determine if (b) is a valid trig binding
 * In dced, currently it is ignored.
 */
#define VALID_SCH_TRIG_BINDING(b)	TRUE

#endif
