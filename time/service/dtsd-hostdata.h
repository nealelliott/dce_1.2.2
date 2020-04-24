/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtsd-hostdata.h,v $
 * Revision 1.1.4.2  1996/02/18  23:12:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:13:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:42  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:48:18  agd
 * 	expand copyright
 * 	[1994/10/06  14:33:27  agd]
 * 
 * Revision 1.1.2.1  1994/08/05  20:47:38  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:37  cbrooks]
 * 
 * $EndLog$
 */
/*
 * interfaces and structures for retrieved hostdata information 
 * from the dced 
 */

#include <dce/dce.h>
#include <dce/dtss_mgmt_support.h>
#include <dce/utctypes.h>

typedef enum {
    e_client_only=1, e_server_only=2, e_client_and_server=3
}  ValidRoles;

typedef int (*ValidationFunction)(void *);

/*
 * A union type, used to return 
 */

typedef struct varvalues_s_t
{
    signed32 attribute_type ;	/* attribute type */
    boolean valid_value ;	/* valid value returned? */
    void * datap ;		/* pointer to data value to be set  */
    union 
    {
	unsigned32 uintvalue ;
	char * stringvalue ;
	utc_t reltime ;
	unsigned32 enumvalue;
    } u;			/* the value itself  */
} VarValues ;

struct SetVars {
    char * variable_name;	/* variable name */
    ValidRoles valid_for_role;		/* 0 = server, 1 = both */
    signed32 vtype;			/* variable type */
    char * valset;			/* value restrictions */
    signed32 attribute ;	/* attribute identifier */
    void * data_element_p;	/* the data element to be updated */
    ValidationFunction F;	/* the validation function */
};

#define UNKNOWN_ATTRIBUTE (~0)
#define UNKNOWN_ENUMERATION (~0UL)

extern void dts_dtsd_parse_entry(const char [], char [], VarValues *);
extern void dts_dtsd_set_entry( const VarValues * ) ;

