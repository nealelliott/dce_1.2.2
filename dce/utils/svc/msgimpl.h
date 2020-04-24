/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Implementation header for DCE message routines.
 */

/*
 * HISTORY
 * $Log: msgimpl.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:11  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:38  root]
 * 
 * Revision 1.1.4.8  1994/09/30  19:44:53  rsalz
 * 	Implement DCE RFC 24.2 (OT CR 11929).
 * 	[1994/09/27  04:45:37  rsalz]
 * 
 * Revision 1.1.4.7  1994/09/26  14:12:03  rsalz
 * 	Tweak include order to get lib message table (OT CR 12327).
 * 	[1994/09/26  13:58:38  rsalz]
 * 
 * Revision 1.1.4.6  1994/09/07  21:05:01  bowe
 * 	Add dce_msg_g_noxpgcat and SVC_NOXPGCAT. [CR 12079]
 * 	Add #if for threads inclusions/declarations. [CR 11885]
 * 	[1994/09/07  21:03:33  bowe]
 * 
 * Revision 1.1.4.5  1994/07/27  12:27:57  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:34:07  rsalz]
 * 
 * Revision 1.1.4.4  1994/06/09  16:06:04  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:38  devsrc]
 * 
 * Revision 1.1.4.3  1994/05/12  14:06:24  rsalz
 * 	Don't append .cat to message catalog on HP (OT CR 10480).
 * 	[1994/05/12  13:48:43  rsalz]
 * 
 * Revision 1.1.4.2  1994/03/29  18:11:13  rsalz
 * 	Check pthread_xxx return values where possible (OT CR 8883)
 * 	[1994/03/29  15:20:23  rsalz]
 * 
 * Revision 1.1.4.1  1993/12/20  20:50:44  rsalz
 * 	Merge msg.sams into svc.sams
 * 	[1993/12/20  20:50:19  rsalz]
 * 
 * Revision 1.1.2.3  1993/12/01  21:18:25  rsalz
 * 	Fix pthread_once function to NOT set lock
 * 	[1993/12/01  21:18:01  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:44  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:37  rsalz]
 * 
 * $EndLog$
 */

/* For cma use of this .h file */
#if !defined(NO_INCLUDE_PTHREAD)
#include <pthread.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <nl_types.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dce_utils.h>
#include <dce/dce_error.h>
#include <dce/dcesvcmsg.h>


#define SMBUF			256
#define RAD40ALPHABET		"abcdefghijklmnopqrstuvwxyz_0123456789-+@"
#if	defined(NO_CATALOG_SUFFIX)
#define CATALOG_SUFFIX		""
#else
#define CATALOG_SUFFIX		".cat"
#endif	/* defined(NO_CATALOG_SUFFIX) */
#define SVC_NOXPGCAT_ENVVAR	"SVC_NOXPGCAT"


/*
**  Bits within a 32bit message ID.
*/
#define TECH_MASK		((unsigned32)0xF0000000u)
#define TECH_SHIFT		28
#define TECH_MASK_DCE		0x10000000u
#define TECH_MASK_DFS		0x20000000u
#define TECH_MASK_ISV1		0x40000000u
#define TECH_MASK_ISV2		0x50000000u
#define COMP_SHIFT		12
#define COMP_MASK		((unsigned32)0x0FFFF000u)
#define CODE_MASK		((unsigned32)0x00000FFFu)
#define ISV_COMP_SHIFT		14
#define ISV_COMP_MASK		((unsigned32)0x0FFFA000u)
#define ISV_CODE_MASK		((unsigned32)0x00003FFFu)
#define ISV_ID_CODE		1

#define IS_ISV_MSG(code)	\
	(((code) & TECH_MASK) == TECH_MASK_ISV1 \
	|| ((code) & TECH_MASK) == TECH_MASK_ISV2)

#define LOCK()		\
	do { \
	    if (!dce_msg_g_setup) \
		pthread_once(&dce_msg_g_once_block, \
		    (pthread_initroutine_t)dce_msg__init_mutex); \
	    if (pthread_mutex_lock(&dce_msg_g_mutex) < 0) \
		continue; \
	} while (0)
#define UNLOCK()	\
	do { \
	} while (pthread_mutex_unlock(&dce_msg_g_mutex) < 0)


/*
**  DCE provides a layer wrapped around standard XPG message catalogs.
**  This typedef is an opaque handle to be used those calls.
*/
struct dce_msg_cat_handle_s_t {
    unsigned32		top_bits;
    unsigned32		mask;
    nl_catd		cd;
};


/*
**  This is our list of tables and our current global pointer.
*/
typedef struct dce_msg_table_list_s_t {
    dce_msg_table_t	*table;
    unsigned32		size;
    unsigned32		smallest;
    unsigned32		biggest;
} dce_msg_table_list_t;


extern void dce_msg__init_mutex(
    void
);


/*
**  Private variables.
*/
extern boolean			dce_msg_g_setup;
#if !defined(NO_INCLUDE_PTHREAD)
extern pthread_mutex_t		dce_msg_g_mutex;
extern pthread_once_t		dce_msg_g_once_block;
#endif
extern dce_msg_table_list_t	dce_msg_g_compiled_table[];
extern dce_msg_table_list_t	*dce_msg_g_curr_table;
extern int			dce_msg_g_table_size;
extern int			dce_msg_g_noxpgcat;

