/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: preauth.h,v $
 * Revision 1.1.6.2  1996/11/13  18:04:45  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/21  16:51 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/2]
 *
 * 	Add datatypes to hold preauth protocol specific data.
 * 	[1996/10/15  13:32 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	Add decrypt_key parameter to obtain_padata to get reply-key
 * 	for third party padata protocol.
 * 	[1996/09/26  23:30 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.1  1996/10/03  14:57:42  arvind
 * 	Merge in new preauth hooks from krb5beta6
 * 	[1996/09/16  21:25 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.4.2  1996/02/18  23:02:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:31  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:43:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:20  root]
 * 
 * Revision 1.1.2.2  1994/06/17  18:42:39  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:12:02  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/11  19:18:00  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:15:57  ahop]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Author: arvind $
 * (Originally written by Glen Machin at Sandia Labs.)
 *
 * Copyright 1992 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 * Sandia National Laboratories also makes no representations about the 
 * suitability of the modifications, or additions to this software for 
 * any purpose.  It is provided "as is" without express or implied warranty.
 * 
 */

/*
 * Note: these typedefs are subject to change.... [tytso:19920903.1609EDT]
 */

#ifdef OSF_DCE_FEATURE
/*
 * preauth protocol specific free procedure for data
 * contained in krb5_pa_misc_data struct.
 */
typedef void (krb5_pa_misc_data_free_proc)
    PROTOTYPE((void *data));
/*
 * This structure holds preauth protocol specific additional
 * data.
 */
typedef struct _krb5_pa_misc_data {
    void                                *data;
    krb5_ui_2                           pa_type;
    krb5_pa_misc_data_free_proc         *free;
} krb5_pa_misc_data;
#endif

typedef krb5_error_code (*krb5_preauth_obtain_proc)
    PROTOTYPE((krb5_context,
	       krb5_pa_data *,
	       krb5_etype_info,
	       krb5_keyblock *, 
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
	       krb5_const_pointer,
	       krb5_creds *,
	       krb5_ccache,
	       krb5_kdc_req *,
#ifdef OSF_DCE_FEATURE
	       krb5_pa_misc_data *,
#endif
	       krb5_pa_data **));

typedef krb5_error_code (*krb5_preauth_process_proc)
    PROTOTYPE((krb5_context,
	       krb5_pa_data *,
	       krb5_kdc_req *,
	       krb5_kdc_rep *,
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
	       krb5_const_pointer,
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_keyblock *,
				     krb5_const_pointer,
				     krb5_kdc_rep * ),
	       krb5_keyblock **,
	       krb5_creds *,
	       krb5_ccache,
	       krb5_int32 *,
#ifdef OSF_DCE_FEATURE
	       krb5_pa_misc_data **,
#endif
	       krb5_int32 *));

typedef struct _krb5_preauth_ops {
        int     type;
	int	flags;
	krb5_preauth_obtain_proc	obtain;
	krb5_preauth_process_proc	process;
} krb5_preauth_ops;

/*
 * Preauthentication property flags
 */
#define KRB5_PREAUTH_FLAGS_ENCRYPT	0x00000001
#define KRB5_PREAUTH_FLAGS_HARDWARE	0x00000002

krb5_error_code krb5_obtain_padata
    	PROTOTYPE((krb5_context,
		krb5_pa_data **,
		krb5_error_code ( * )PROTOTYPE((krb5_context, 
						krb5_const krb5_enctype,
						krb5_data *,
						krb5_const_pointer,
						krb5_keyblock **)),
		krb5_const_pointer, 
		krb5_creds *,
		krb5_ccache,
#ifdef OSF_DCE_FEATURE
		krb5_pa_misc_data **,
#endif
		krb5_kdc_req *));

krb5_error_code krb5_process_padata
	PROTOTYPE((krb5_context,
		krb5_kdc_req *,
		krb5_kdc_rep *,
		krb5_error_code ( * )PROTOTYPE((krb5_context,
						krb5_const krb5_enctype,
						krb5_data *,
						krb5_const_pointer,
						krb5_keyblock **)),
		krb5_const_pointer,
		krb5_error_code ( * )PROTOTYPE((krb5_context,
						krb5_const krb5_keyblock *,
						krb5_const_pointer,
						krb5_kdc_rep * )),
		krb5_keyblock **, 	
		krb5_creds *, 
		krb5_ccache,
#ifdef OSF_DCE_FEATURE
		krb5_pa_misc_data **,
#endif
		krb5_int32 *));		

