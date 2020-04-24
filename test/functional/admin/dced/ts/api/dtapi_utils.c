
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: dtapi_utils.c,v $
 * Revision 1.1.8.1  1996/10/03  18:39:04  arvind
 * 	Submit fix for CHFts19700
 * 	[1996/08/14  19:45 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
 *
 * 	Merge in all changes to dced tests for new ACLs.
 * 	[1996/01/15  22:56 UTC  jss  /main/jss_dced_acl/1]
 *
 * 	Clean up potential segmentation violation.
 * 	[1994/09/30  22:55:39  baum  1.1.2.2]
 *
 * Revision 1.1.6.2  1996/02/18  00:30:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:27  marty]
 * 
 * Revision 1.1.6.1  1995/12/11  15:18:17  root
 * 	Submit
 * 	[1995/12/11  14:33:06  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:20  root]
 * 
 * Revision 1.1.2.2  1994/09/30  22:55:39  baum
 * 	Fixed a number of test things (OT 12393)
 * 	[1994/09/30  22:54:27  baum]
 * 
 * 	New secval tests plus the ability to run without TET (OT 11975)
 * 	[1994/09/30  21:14:44  baum]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:47  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:25  melman]
 * 
 * Revision 1.1.2.8  1994/09/16  20:10:50  ganni
 * 	enhancements and cleanup of srvrexec tests
 * 	[1994/09/16  20:09:54  ganni]
 * 
 * Revision 1.1.2.7  1994/08/25  14:02:35  ganni
 * 	remove the reference to the expunged member transmit_rep
 * 	[1994/08/25  14:02:10  ganni]
 * 
 * Revision 1.1.2.6  1994/07/20  19:57:49  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:38:17  ganni]
 * 
 * Revision 1.1.2.5  1994/06/17  18:44:47  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:32  devsrc]
 * 	Revision 1.1.2.4  1994/06/07  19:52:08  ganni
 * 	use the protect level _connect while authenticating
 * 	[1994/06/07  19:39:21  ganni]
 * 
 * Revision 1.1.2.3  1994/06/03  14:05:33  ganni
 * 	create the principal needed for the tests through scripts
 * 	[1994/06/03  14:03:17  ganni]
 * 
 * Revision 1.1.2.2  1994/05/25  17:13:06  ganni
 * 	change the key version number to 1 from 0
 * 	[1994/05/25  17:11:08  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:23  ganni
 * 	Initial Version
 * 	[1994/05/18  21:13:35  ganni]
 * 
 * $EndLog$
 */

/*
** Module:	dtapi_utils.c
**
** FACILITY:	DCE Host Daemon API
**
** ABSTRACT:	This module implements the dced api common functions
**
*/

#include <dtapi_impl.h>
#include <dce/dce_cf.h>
#include <dce/dce_error.h>
#include <dce/stubbase.h>
#include <unistd.h>
#include <dtapi_1_srvr.h>

#define         CONFIG_NM       (dced_string_t)"/config/"
uuid_t dced_g_uuid_fileattr = { /* 008b47dd-6ec9-1d6a-9ac7-0000c09ce054 */
    0x008b47dd, 0x6ec9, 0x1d6a, 0x9a, 0xc7,
    {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
};

/* from the idl file */
uuid_t DTAPI_IF_1_UUID  = { /* 002d2117-7062-1dd2-a9bb-0000c03cd04c */
    0x002d2117, 0x7062, 0x1dd2, 0xa9, 0xbb,
    {0x00, 0x00, 0xc0, 0x3c, 0xd0, 0x4c}
};

uuid_t DTAPI_OBJ_1_UUID  = { /* 60cc7b6a-9e48-11cd-a5d7-0000c03cd04c */
    0x60cc7b6a, 0x9e48, 0x11cd, 0xa5, 0xd7,
    {0x00, 0x00, 0xc0, 0x3c, 0xd0, 0x4c}
};
/* bad object (corrupt) uuid 00cf2d72-9dba-11cd-b000-0000c03cd04c
    0x00cf2d72, 0x9dba, 0x11cd, 0xb0, 0x00,
    {0x00, 0x00, 0xc0, 0x3c, 0xd0, 0x4c}
};
*/
/* The NDR transfer syntax id */
static rpc_syntax_id_t NIDL_ndr_id = {
	{0x8a885d04U, 0x1ceb, 0x11c9, 0x9f, 0xe8, 
	{0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}}, 2
};

char txt_msg[DCED_TEST_MSG_LEN];

static server_fixedattr_t       s_null_fixedattr;
static service_t                s_null_service;
static server_t                 s_null_server;

/* 
** ROUTINE NAME:        
**                    dced_util_printerr()
**
** DESCRIPTION:
**                    Converts status returns to string messages using
**                    dce_error_inq_text, and logs the messages to the
**                    tet journal.
**
*/
void
dced_util_printerr(
	/* [in] */	error_status_t error_status
)
{
	unsigned char error_text[dce_c_error_string_len];
	int status;


#ifdef DEBUG
	sprintf(txt_msg, "dced_util_printerr: printing text for status = %lu",
		error_status);
	tet_infoline(txt_msg);
#endif

	dce_error_inq_text(error_status, error_text, &status);
	if (status == 0)
		tet_infoline((char *)error_text);
#ifdef DEBUG
	else
	{
		sprintf(txt_msg, 
		"dced_util_printerr: dce_error_inq_text failed, status = %lu",
			error_status);
		tet_infoline(txt_msg);
	}
#endif
}

void 
print_uuid(
        /* [in] */      char		*in_str,
        /* [in] */      uuid_t		*uuid_ptr
)
{
        unsigned_char_p_t       string_uuid;
        error_status_t          st;

        uuid_to_string(uuid_ptr, &string_uuid, &st);
        sprintf(txt_msg, "%s = \"%s\"", in_str, string_uuid);
        tet_infoline(txt_msg);
        rpc_string_free(&string_uuid, &st);

}

void 
test_format_2_name(
        /* [ in ] */    dced_string_t   svc_dced,
        /* [ out ] */   char            **name_ptr
)
{
        char                    *host_nm;
        char                    *service_name;
        error_status_t          st;

        dce_cf_get_host_name( &host_nm, &st);

        *name_ptr = service_name =
                 (char *)malloc(strlen((char *)svc_dced)+strlen(host_nm)+2);

        strcpy(service_name, (char *)svc_dced);
        strcpy(service_name+strlen((char *)svc_dced), "@");
        strcpy(service_name+strlen((char *)svc_dced)+1, host_nm);

        free(host_nm);
}

void 
test_format_3_name(
        /* [ in ] */    dced_string_t   svc_dced,
        /* [ out ] */   char            **name_ptr
)
{
        char                    *host_nm;
        char                    *service_name;
        error_status_t          st;

        dce_cf_get_host_name( &host_nm, &st);

        *name_ptr = service_name =
                (char *)malloc(CELL_DIR_ROOT_LEN+strlen(host_nm)+strlen((char *)CONFIG_NM)+strlen((char *)svc_dced)+1);

        strcpy(service_name, CELL_DIR_ROOT);
        strcpy(service_name+CELL_DIR_ROOT_LEN, host_nm);
        strcpy(service_name+CELL_DIR_ROOT_LEN+strlen(host_nm), (char *)CONFIG_NM);
        strcpy(service_name+CELL_DIR_ROOT_LEN+strlen(host_nm)+strlen((char *)CONFIG_NM), (char *)svc_dced);

        free(host_nm);

}

void 
test_format_4_name(
        /* [ in ] */	dced_string_t   svc_dced,
        /* [ out ] */	char            **name_ptr
)
{
        char                    *host_nm;
        char                    *service_name;
        char                    *cell_nm;
        error_status_t          st;

        dce_cf_get_host_name( &host_nm, &st);
        dce_cf_get_cell_name( &cell_nm, &st);

        *name_ptr = service_name =
                (char *)malloc(strlen(cell_nm)+1+strlen(host_nm)+strlen((char *)CONFIG_NM)+strlen((char *)svc_dced)+2);

        strcpy(service_name, cell_nm);
        strcpy(service_name+strlen(cell_nm), "/");
        strcpy(service_name+strlen(cell_nm)+1, host_nm);
        strcpy(service_name+strlen(cell_nm)+1+strlen(host_nm), (char *)CONFIG_NM);
        strcpy(service_name+strlen(cell_nm)+1+strlen(host_nm)+strlen((char *)CONFIG_NM), (char *)svc_dced);

        free(host_nm);
        free(cell_nm);

}


/*
** ROUTINE NAME:
**                    dced_util_binding_create()
**
** DESCRIPTION:
**		      creates a binding handle to the required
**		      service in dced.
*/
void  
dced_util_binding_create(
	/* [in]  */	dced_string_t	service,
        /* [out] */     dced_binding_handle_t   *h,
        /* [out] */     error_status_t	*st
)
{
	unsigned32		protect_lvl;
	error_status_t		tmp_st;
        unsigned32              flags = 0;

/* NOTES:
        we have to worry about the name, ie. the cell name etc.
        we can get this from tetexec.cfg also.
*/
	unlink((char * )DCED_HOST_FILE);
	unlink((char *)DCED_KEY_FILE);


        dced_binding_create( service, flags, h, st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(*st);
#endif
                return;
        }
	if (!strcmp ((char *)service, (char *)KEYTAB_SVC))
		protect_lvl = rpc_c_protect_level_pkt_privacy;
	else
		protect_lvl = rpc_c_protect_level_connect;

        dced_binding_set_auth_info(*h,
                                  protect_lvl,
                                  rpc_c_authn_dce_private,
                                  NULL,
                                  rpc_c_authz_dce,
                                  st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_set_auth_info failed:");
                dced_util_printerr(*st);
#endif
		dced_binding_free( *h, &tmp_st);
                return;
        }

}

/*
** ROUTINE NAME:
**                    test_attr_init()
**
** DESCRIPTION:
**                    This routine performs attribute list initialization
**
*/
void 
test_attr_init(
	/* [in, out] */	dced_attr_list_t *hattr_ptr,
	/* [in]  */	int		num_attrs,
	/* [out] */	error_status_t	*st
)
{
	int	count_strings,  str_size;
	sec_attr_enc_str_array_t *hattr_array;


        hattr_ptr->count = 1;
        hattr_ptr->list  = (sec_attr_t *)malloc( sizeof(sec_attr_t) );

        uuid_create(&((hattr_ptr->list)->attr_id), st);

        (hattr_ptr->list)->attr_value.attr_encoding 
					= sec_attr_enc_printstring_array;
	count_strings = ((num_attrs>0)?num_attrs:1);
	str_size = sizeof(sec_attr_enc_str_array_t) +
		count_strings*sizeof(sec_attr_enc_printstring_p_t);

	(hattr_ptr->list)->attr_value.tagged_union.string_array
		= hattr_array	=(sec_attr_enc_str_array_t *)malloc(str_size);

	hattr_array->num_strings = count_strings;
	switch(count_strings)
	{
		case 1:
			hattr_array->strings[0] = DCED_HOST_ATTR0;
			break;
		case 2:
			hattr_array->strings[0] = DCED_HOST_ATTR1;
			hattr_array->strings[1] = DCED_HOST_ATTR2;
			break;
	}
}

/*
** ROUTINE NAME:
**                    test_attr_free()
**
** DESCRIPTION:
**                    This routine frees up the memory allocated
**		      for the attribute list.
**
*/
void
test_attr_free(
	/* [in] */	dced_attr_list_t *hattr_ptr,
	/* [out] */	error_status_t	*st
)
{
	switch ((hattr_ptr->list)->attr_value.attr_encoding) {

        case sec_attr_enc_printstring_array:
		free((hattr_ptr->list)->attr_value.tagged_union.string_array);
		break;
	default:
		break;
	}

	free(hattr_ptr->list);

}


/*
** ROUTINE NAME:
**                    test_attr_verify()
**
** DESCRIPTION:
**
**
*/
boolean32
test_attr_verify(
        /* [in]  */     sec_attr_t	*in_ptr,
        /* [in]  */     sec_attr_t	*out_ptr
)
{
	sec_attr_enc_str_array_t *in_array, *out_array;
	int	i;

	switch (out_ptr->attr_value.attr_encoding) {
        case sec_attr_enc_printstring_array:
		in_array = in_ptr->attr_value.tagged_union.string_array;	
		out_array = out_ptr->attr_value.tagged_union.string_array;	
#ifdef DEBUG
        	sprintf(txt_msg,"input-# of attributes: %d\n", in_array->num_strings);
		tet_infoline(txt_msg);
        	sprintf(txt_msg,"output-# of attributes: %d\n", out_array->num_strings);
		tet_infoline(txt_msg);
#endif
		if (in_array->num_strings != out_array->num_strings)
		{
#ifdef DEBUG
	            tet_infoline("test_attr_verify:\n");
	            tet_infoline("attributes do not match\n");
#endif
		    return false;
		}			
		else
		{
		    for (i=0; i<in_array->num_strings;i++)
		    {
			sprintf(txt_msg,"input_attribute[%d]: %s\n", 
				i, in_array->strings[i]);
			tet_infoline(txt_msg);
        		sprintf(txt_msg,"output_attribute[%d]: %s\n", 
				i, out_array->strings[i]);
			tet_infoline(txt_msg);

        		if (strcmp(	(char *)in_array->strings[i], 
					(char *)out_array->strings[i]))
			    return false;
		    }
		}
		break;
	default:
		break;
	}

        return true;
}

/*
** ROUTINE NAME:
**                    test_attrlist_verify()
**
** DESCRIPTION:
**
**
*/
boolean32
test_attrlist_verify(
        /* [in]  */	dced_attr_list_t *in_ptr,
        /* [in]  */	dced_attr_list_t *out_ptr
)
{
	error_status_t		 st;

#ifdef DEBUG
	print_uuid ((char *)"input uuid", &((in_ptr->list)->attr_id));
	print_uuid ((char *)"output uuid", &((out_ptr->list)->attr_id));
#endif

	if (uuid_compare(&((in_ptr->list)->attr_id),
			&((out_ptr->list)->attr_id), &st))
	{
#ifdef DEBUG
                tet_infoline("uuid_compare failed");
                dced_util_printerr(st);
#endif
                return false;
	}

	if (!test_attr_verify(in_ptr->list, out_ptr->list))
		return false;
        return true;
}


/*
** ROUTINE NAME:
**                    test_keyentry_init()
**
** DESCRIPTION:
**                    This routine performs key entry initialization
**
*/
void 
test_keyentry_init(
	/* [out] */	dced_entry_t	*key_entry_ptr,
	/* [out] */	error_status_t	*st
)
{
        key_entry_ptr->name         = DCED_KEY_USER;
        key_entry_ptr->description  = DCED_KEY_DESC;
        key_entry_ptr->storage_tag  = DCED_KEY_FILE;

        uuid_create(&(key_entry_ptr->id), st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("uuid_create failed:");
                dced_util_printerr(*st);
#endif
                tet_result(TET_UNRESOLVED);
                return;
        }
}

void
print_key(
	/* [in] */      dced_key_t	*key_ptr
)
{

	sprintf(txt_msg,"principal, version, passwd: %s %d %s\n",
		key_ptr->principal, key_ptr->version, (key_ptr->passwd)->key.tagged_union.plain);
	tet_infoline(txt_msg);
}

/*
** ROUTINE NAME:
**                    test_key_init()
**
** DESCRIPTION:
**                    This routine performs key initialization
**
*/
void
test_key_init(
	/* [in]  */	int		key_num, 
	/* [out] */	dced_key_t	*key_info_ptr
)
{
	unsigned char	*raw_pw;
	int		len;

	/* 
	 * can initialize upto information for 3 different
	 * keys. This is all static information. 
	 * Need to create this info using rgy_edit.
	 */

        key_info_ptr->version		= 1;
        key_info_ptr->authn_service	= rpc_c_authn_dce_secret;

        if (!(key_info_ptr->passwd))
                key_info_ptr->passwd  =
                        (sec_passwd_rec_t *)malloc( sizeof(sec_passwd_rec_t));

        (key_info_ptr->passwd)->key.key_type    = sec_passwd_plain;
        (key_info_ptr->passwd)->pepper          = NULL;
        (key_info_ptr->passwd)->version_number  = 1;

        switch (key_num) {

                case 3:
			len = strlen((char *)DCED_KEY_PASSWD_3)+1;
                        key_info_ptr->principal      = DCED_KEY_PRINC_3;
			(key_info_ptr->passwd)->key.tagged_union.plain = 
				raw_pw = malloc(len);
			memcpy (raw_pw, DCED_KEY_PASSWD_3, len);
			raw_pw[len]='\0';
                        break;

                case 2:
			len = strlen((char *)DCED_KEY_PASSWD_2)+1;
                        key_info_ptr->principal      = DCED_KEY_PRINC_2;
			(key_info_ptr->passwd)->key.tagged_union.plain = 
				raw_pw = malloc(len);
			memcpy (raw_pw, DCED_KEY_PASSWD_2, len);
			raw_pw[len]='\0';
                        break;

                case 1:
			len = strlen((char *)DCED_KEY_PASSWD_1)+1;
                        key_info_ptr->principal      = DCED_KEY_PRINC_1;
			(key_info_ptr->passwd)->key.tagged_union.plain = 
				raw_pw = malloc(len);
			memcpy (raw_pw, DCED_KEY_PASSWD_1, len);
			raw_pw[len]='\0';
		
		case 0:
                default:

                        break;
                }

}

/*
** ROUTINE NAME:
**                    test_keylist_init()
**
** DESCRIPTION:
**                    This routine performs key list initialization
**
*/
void 
test_keylist_init(
	/* [in, out] */	dced_key_list_t *klist_ptr,
	/* [in]  */	int		num_keys,
	/* [out] */	error_status_t	*st
)
{
	int	i;

        klist_ptr->count = num_keys;
	if (num_keys)
	{
        	klist_ptr->list  = 
			(dced_key_t *)malloc( num_keys * sizeof(dced_key_t) );
		for (i=0;i<num_keys;i++)
		{
        		klist_ptr->list[i].passwd =
                	(sec_passwd_rec_t *)malloc( sizeof(sec_passwd_rec_t) );
        		test_key_init( i+1, &(klist_ptr->list[i]) );
#ifdef DEBUG
			tet_infoline("Initialized key values:");
			print_key( &(klist_ptr->list[i]) );
#endif
		}
	}
	else
        	klist_ptr->list  = NULL;
}

/*
** ROUTINE NAME:
**                    test_key_verify()
**
** DESCRIPTION:
**
*/
boolean32
test_key_verify(
	/* [in]  */     dced_key_t	*key_in_ptr,
	/* [in]  */     dced_key_t	*key_out_ptr
)
{
        char                    *cell_nm;
        char                    *service_name;
        error_status_t          st;
        int                     cell_nm_len;

        dce_cf_get_cell_name( &cell_nm, &st);
        cell_nm_len = strlen(cell_nm);
        service_name =
           (char *)malloc(cell_nm_len+2+strlen((char *)key_in_ptr->principal));
        strcpy(service_name, cell_nm);
        strcpy(service_name+cell_nm_len, "/");
        strcpy(service_name+cell_nm_len+1, (char *)key_in_ptr->principal);
        free(cell_nm);

        if (strcmp( (char *)service_name, (char *)key_out_ptr->principal))
        {
#ifdef DEBUG
		tet_infoline("Output key values:");
		print_key( key_out_ptr );
#endif
                free( service_name );
		return false;
        }

        free( service_name );
	return true;

}
/*
** ROUTINE NAME:
**                    test_keylist_verify()
**
** DESCRIPTION:
**
*/
boolean32
test_keylist_verify(
	/* [in]  */     dced_key_list_t	*in_ptr,
	/* [in]  */     dced_key_list_t	*out_ptr
)
{
	int i;

	if (in_ptr->count == out_ptr->count)
	{
		for (i=0;i<out_ptr->count;i++)
		{
        	    if (!test_key_verify( &(in_ptr->list[i]), 
					&(out_ptr->list[i]) ))
			return false;
		}
	}
	else
	{
#ifdef DEBUG
        	sprintf(txt_msg,"input-# of keys: %d\n", in_ptr->count);
		tet_infoline(txt_msg);
        	sprintf(txt_msg,"output-# of keys: %d\n", out_ptr->count);
		tet_infoline(txt_msg);
#endif	
		return false;
	}
	return true;
}


/*
** ROUTINE NAME:
**                    test_keylist_free()
**
** DESCRIPTION:
**                    This routine frees up the memory allocated
**		      for the key list.
**
*/
void 
test_keylist_free(
	/* [in] */	dced_key_list_t	*klist_ptr,
	/* [out] */	error_status_t	*st
)
{
	int	i;

	if (klist_ptr->count)
	{
		for (i=0;i<klist_ptr->count;i++)
		{
        		free (klist_ptr->list[i].passwd);
		}
        	free (klist_ptr->list);
	}
}

/*
** ROUTINE NAME:
**                    test_keytab_init()
**
** DESCRIPTION:
**                    This routine creates the binding to 
**		      to keytab service, creates the keytable
**		      with the requested number of keys.
**
*/
void 
test_keytab_init(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */	dced_entry_t	*key_entry_ptr,
	/* [in, out] */	dced_key_list_t *klist_ptr,
	/* [in]  */	int		num_keys,
	/* [out] */	error_status_t	*st
)
{
	error_status_t	tmp_st;

        dced_util_binding_create( KEYTAB_SVC, binding_ptr, st);
        if (*st != error_status_ok)
                return;

        /*
         * Initialize the key table information and
         * a key list with information for requested number of keys.
         */
        test_keyentry_init( key_entry_ptr, st);
        test_keylist_init( klist_ptr, num_keys, st);

        dced_keytab_create( *binding_ptr, key_entry_ptr, klist_ptr, st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_keytab_create failed:");
                dced_util_printerr(*st);
#endif
		dced_binding_free( *binding_ptr, &tmp_st);
                return;
        }
	return;
}

/*
** ROUTINE NAME:
**                    test_keytab_free()
**
** DESCRIPTION:
**                    This routine deletes the keytable,
**                    frees the binding to keytab service
**
*/
void 
test_keytab_free(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ dced_entry_t	*key_entry_ptr,
	/* [in, out] */ dced_key_list_t *klist_ptr,
	/* [out] */	error_status_t	*st
)
{
	error_status_t	tmp_st;

        test_keylist_free(klist_ptr, st);
        dced_keytab_delete( *binding_ptr,&(key_entry_ptr->id), st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_keytab_delete failed:");
                dced_util_printerr(*st);
#endif
        }

        dced_binding_free( *binding_ptr, &tmp_st);
        if (tmp_st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(tmp_st);
#endif
		return;
        }

	return;
}

/*
** ROUTINE NAME:
**                    test_keytab_entry_free()
**
** DESCRIPTION:
**                    This routine deletes the keytable,
**                    frees the binding to keytab service
**                    This routine is needed to delete the entry only of
**                    an object that imported its storage file.
**
*/
void 
test_keytab_entry_free(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ dced_entry_t	*key_entry_ptr,
	/* [in, out] */ dced_key_list_t *klist_ptr,
	/* [out] */	error_status_t	*st
)
{
	error_status_t	tmp_st;

        test_keylist_free(klist_ptr, st);
	dced_entry_remove( *binding_ptr,&(key_entry_ptr->id), st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_keytab_delete failed:");
                dced_util_printerr(*st);
#endif
        }
	unlink((char *)DCED_KEY_FILE);

        dced_binding_free( *binding_ptr, &tmp_st);
        if (tmp_st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(tmp_st);
#endif
		return;
        }

	return;
}

/*
** ROUTINE NAME:
**                    test_hostentry_init()
**
** DESCRIPTION:
**                    This routine performs host entry initialization
**
*/
void 
test_hostentry_init(
	/* [out] */	dced_entry_t	*host_entry_ptr,
	/* [out] */	error_status_t	*st
)
{
        host_entry_ptr->name         = DCED_TEST_HOST;
        host_entry_ptr->description  = DCED_HOST_DESC;
        host_entry_ptr->storage_tag  = DCED_HOST_FILE;

        uuid_create(&(host_entry_ptr->id), st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("uuid_create failed:");
                dced_util_printerr(*st);
#endif
                tet_result(TET_UNRESOLVED);
                return;
        }

}


/*
** ROUTINE NAME:
**                    test_hostattr_init()
**
** DESCRIPTION:
**                    This routine performs host attribute initialization
**
*/
void 
test_hostattr_init(
	/* [in, out] */	dced_attr_list_t *hattr_ptr,
	/* [in]  */	int		num_attrs,
	/* [out] */	error_status_t	*st
)
{
	int	count_strings,  str_size;
	sec_attr_enc_str_array_t *hattr_array;


        hattr_ptr->count = 1;
        hattr_ptr->list  = (sec_attr_t *)malloc( sizeof(sec_attr_t) );

        (hattr_ptr->list)->attr_id = dced_g_uuid_fileattr;

        (hattr_ptr->list)->attr_value.attr_encoding 
					= sec_attr_enc_printstring_array;
	count_strings = ((num_attrs>0)?num_attrs:1);
	str_size = sizeof(sec_attr_enc_str_array_t) +
		count_strings*sizeof(sec_attr_enc_printstring_p_t);

	(hattr_ptr->list)->attr_value.tagged_union.string_array
		= hattr_array	=(sec_attr_enc_str_array_t *)malloc(str_size);

	hattr_array->num_strings = count_strings;
	switch(count_strings)
	{
		case 1:
			hattr_array->strings[0] = DCED_HOST_ATTR0;
			break;
		case 2:
			hattr_array->strings[0] = DCED_HOST_ATTR1;
			hattr_array->strings[1] = DCED_HOST_ATTR2;
			break;
	}
}

/*
** ROUTINE NAME:
**                    test_hostattr_free()
**
** DESCRIPTION:
**                    This routine frees up the memory allocated
**		      for the host attribute list.
**
*/
void
test_hostattr_free(
	/* [in] */	dced_attr_list_t *hattr_ptr,
	/* [out] */	error_status_t	*st
)
{
	test_attr_free(hattr_ptr, st);

}

/*
** ROUTINE NAME:
**                    test_hostattr_verify()
**
** DESCRIPTION:
**
**
*/
boolean32
test_hostattr_verify(
        /* [in]  */     sec_attr_t	*in_ptr,
        /* [in]  */     sec_attr_t	*out_ptr
)
{
	return( test_attr_verify(in_ptr, out_ptr));
}

/*
** ROUTINE NAME:
**                    test_hostdata_verify()
**
** DESCRIPTION:
**
**
*/
boolean32
test_hostdata_verify(
        /* [in]  */	dced_attr_list_t *in_ptr,
        /* [in]  */	dced_attr_list_t *out_ptr
)
{
	return( test_attrlist_verify(in_ptr, out_ptr));
}

/*
** ROUTINE NAME:
**                    test_hostdata_init()
**
** DESCRIPTION:
**                    This routine creates the binding to 
**		      to hostdata service, creates the hostdata
**		      entry with the requested number of attributes.
**
*/
void 
test_hostdata_init(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ dced_entry_t	*host_entry_ptr,
	/* [in, out] */ dced_attr_list_t *hattr_ptr,
	/* [in]  */	int		num_attrs,
	/* [out] */	error_status_t	*st
)
{
	error_status_t	tmp_st;

        dced_util_binding_create( HOST_SVC, binding_ptr, st);
        if (*st != error_status_ok)
                return;

        /*
         * Initialize the key table information and
         * a key list with information for requested number of keys.
         */
        test_hostentry_init( host_entry_ptr, st);
        test_hostattr_init( hattr_ptr, num_attrs, st);

        dced_hostdata_create( *binding_ptr, host_entry_ptr, hattr_ptr, st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_hostdata_create failed:");
                dced_util_printerr(*st);
#endif
		test_hostattr_free(hattr_ptr, &tmp_st);
		dced_binding_free( *binding_ptr, &tmp_st);
                return;
        }
	return;
}

/*
** ROUTINE NAME:
**                    test_hostdata_free()
**
** DESCRIPTION:
**                    This routine deletes the hostdata entry,
**                    frees the binding to hostdata service.
**
*/
void 
test_hostdata_free(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ dced_entry_t	*host_entry_ptr,
	/* [in, out] */ dced_attr_list_t *hattr_ptr,
	/* [out] */	error_status_t	*st
)
{
	error_status_t	tmp_st;

        test_hostattr_free(hattr_ptr, st);
        dced_hostdata_delete( *binding_ptr,&(host_entry_ptr->id), st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_hostdata_delete failed:");
                dced_util_printerr(*st);
#endif
        }

        dced_binding_free( *binding_ptr, &tmp_st);
        if (tmp_st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(tmp_st);
#endif
		return;
        }

	return;
}

/*
** ROUTINE NAME:
**                    test_hostdata_entry_free()
**
** DESCRIPTION:
**                    This routine deletes the hostdata entry,
**                    frees the binding to hostdata service.
**                    This routine is needed to delete the entry only of
**                    an object that imported its storage file.
**
*/
void 
test_hostdata_entry_free(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ dced_entry_t	*host_entry_ptr,
	/* [in, out] */ dced_attr_list_t *hattr_ptr,
	/* [out] */	error_status_t	*st
)
{
	error_status_t	tmp_st;

        test_hostattr_free(hattr_ptr, st);
	dced_entry_remove( *binding_ptr,&(host_entry_ptr->id), st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_hostdata_delete failed:");
                dced_util_printerr(*st);
#endif
        }
	unlink((char * )DCED_HOST_FILE);

        dced_binding_free( *binding_ptr, &tmp_st);
        if (tmp_st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(tmp_st);
#endif
		return;
        }

	return;
}

void 
print_cfgentry(
	/* [in] */	dced_entry_t	*he_ptr
)
{

        sprintf(txt_msg,"entry name : %s\n",he_ptr->name);
        tet_infoline(txt_msg);
        sprintf(txt_msg,"description : %s\n",he_ptr->description);
        tet_infoline(txt_msg);
        sprintf(txt_msg,"file name: %s\n",he_ptr->storage_tag);
        tet_infoline(txt_msg);
}

void 
print_cfgentry_list(
	/* [in] */	dced_entry_list_t *hel_ptr
)
{
	int 	i;

	sprintf(txt_msg, "number of entries : %d\n",hel_ptr->count);
        tet_infoline(txt_msg);
        for (i=0; i< hel_ptr->count;i++) {
		print_cfgentry((dced_entry_t *)&(hel_ptr->list[i]));
        }
}
/*
** ROUTINE NAME:
**                    test_cfgentry_verify()
**
** DESCRIPTION:
**
**
*/
boolean32
test_cfgentry_verify(
	/* [in]  */	dced_entry_t	*in_ptr,
	/* [in]  */	dced_entry_t	*out_ptr
)
{
	if (strcmp( (char *)in_ptr->name, (char *)out_ptr->name))
	{
#ifdef DEBUG
		sprintf (txt_msg, "input entry name: %s\n",in_ptr->name);
		tet_infoline(txt_msg);
		sprintf (txt_msg, "output entry name: %s\n",out_ptr->name);
		tet_infoline(txt_msg);
#endif
		return(FALSE);
	}
	return(TRUE);
}

/*
** ROUTINE NAME:
**                    test_srvr_entry_init()
**
** DESCRIPTION:
**                    This routine performs server entry initialization
**
*/
void
test_srvr_entry_init(
        /* [in, out] */ server_t        *srvr_entry_ptr,
        /* [out] */     error_status_t  *st
)
{
	service_t		*service_ptr;
	dced_tower_list_t	*tower_list;
	rpc_binding_handle_t	binding;
        unsigned_char_t		*string_binding;
        char                    *protseq_string;
	char                    *entryname;
	char                    hostname[MAX_HOSTNAME_LEN];

	rpc_if_rep_t    interface_spec = {
        	0,                  /* ifspec rep version	*/ 
        	0,                  /* operation count 		*/
        	0,                  /* interface version	*/ 
        	{ 0 },              /* interface uuid		*/
        	2,                  /* stub/rt iface version no */
        	{ 0, NULL },        /* endpoint vector		*/
        	{ 1, &NIDL_ndr_id}, /* NDR transfer syntax	*/
        	NULL,               /* server stub epv		*/
        	NULL                /* manager stub epv		*/
	};

	*srvr_entry_ptr = s_null_server;

        uuid_create(&(srvr_entry_ptr->id), st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("uuid_create failed:");
                dced_util_printerr(*st);
#endif
                tet_result(TET_UNRESOLVED);
                return;
        }

        srvr_entry_ptr->name         = DCED_TEST_SRVR;

	srvr_entry_ptr->services.count = 1;
	srvr_entry_ptr->services.list = 
		(service_t *)malloc(srvr_entry_ptr->services.count * 
						sizeof(service_t));
	service_ptr = &(srvr_entry_ptr->services.list[0]);
	*service_ptr = s_null_service;

	service_ptr->ifspec.uuid = DTAPI_IF_1_UUID;
	service_ptr->ifspec.vers_major = 1;
	service_ptr->ifspec.vers_minor = 0;

	service_ptr->ifname	= (dced_string_t)DCED_TEST_SRVR;

	gethostname(hostname, MAX_HOSTNAME_LEN);
	entryname = malloc (strlen((char *)DCED_SRVR_ENTRY) + strlen(hostname) +
			    strlen((char *)DCED_TEST_SRVR) + 2);
	strcpy (entryname, (char *)DCED_SRVR_ENTRY);
	strcat (entryname, hostname);
	strcat (entryname, "/");
	strcat (entryname, (char *)DCED_TEST_SRVR);
	
	service_ptr->entryname	= (dced_string_t)entryname;

	service_ptr->objects.count = 1;
	service_ptr->objects.list =
                (uuid_t *)malloc(service_ptr->objects.count * sizeof(uuid_t));
	service_ptr->objects.list[0] = DTAPI_OBJ_1_UUID;

	tower_list = &service_ptr->towers;
	tower_list->count = 1;
	tower_list->list =
		(sec_attr_twr_set_p_t *) malloc(tower_list->count *
					sizeof (sec_attr_twr_set_p_t));

	protseq_string = DCED_SRVR_PROTO;
        if ((protseq_string == NULL) || (*protseq_string == '\0'))
	    protseq_string = "ncadg_ip_udp";

        rpc_string_binding_compose((unsigned_char_p_t)"",
                                   (unsigned_char_p_t)protseq_string,
                                   (unsigned_char_p_t)"127.0.0.0",
                                   (unsigned_char_p_t)"",
                                   (unsigned_char_p_t)"",
                                   &string_binding,
                                   st);

        rpc_binding_from_string_binding((unsigned char *) string_binding,
                                        &binding,
                                        st);
	rpc_string_free( &string_binding, st);

	interface_spec.vers=
		(service_ptr->ifspec.vers_minor << 6) 
		| service_ptr->ifspec.vers_major;
	interface_spec.id = service_ptr->ifspec.uuid;

        rpc_tower_vector_from_binding(
                                (rpc_if_handle_t)&interface_spec,
                                binding,
                                (rpc_tower_vector_p_t *)&tower_list->list[0], 
                                st);
	rpc_binding_free( &binding, st);

        srvr_entry_ptr->fixed   = s_null_fixedattr;
/*
        srvr_entry_ptr->fixed.startupflags = 
			server_c_startup_auto;
*/
        srvr_entry_ptr->fixed.program = (dced_string_t)DCED_SRVR_PGM;
        srvr_entry_ptr->fixed.arguments.count = 2;
        srvr_entry_ptr->fixed.arguments.list =
		(dced_string_t *)malloc(srvr_entry_ptr->fixed.arguments.count * 
						sizeof(dced_string_t));
        srvr_entry_ptr->fixed.arguments.list[0] =(dced_string_t)entryname;
        srvr_entry_ptr->fixed.arguments.list[1] =(dced_string_t)DCED_SRVR_PROTO;

/* initialize the atributes?
dced_attr_list_t        attributes;
test_attr_init( &(srvr_entry_ptr->attributes), 2, st);
*/

	srvr_entry_ptr->prin_names.count = 1;
	srvr_entry_ptr->prin_names.list = 
		(dced_string_t *)malloc(srvr_entry_ptr->prin_names.count * 
						sizeof(dced_string_t));
	srvr_entry_ptr->prin_names.list[0] = 
			(dced_string_t)strdup( (char *)DCED_SRVR_PRINC_1 );
}

/*
** ROUTINE NAME:
**                    test_srvr_entry_free()
**
** DESCRIPTION:
**                    This routine performs server entry cleanup
**
*/
void
test_srvr_entry_free(
        /* [in] */	server_t        *srvr_entry_ptr,
        /* [out] */     error_status_t  *st
)
{
	service_t		*service_ptr;
	unsigned32		i,j;

	for (i=1; i<=srvr_entry_ptr->services.count;i++)
	{
	        service_ptr = &(srvr_entry_ptr->services.list[i-1]);
		free(service_ptr->objects.list);
		free(service_ptr->entryname);
		for (j=1; j<=service_ptr->towers.count;j++)
        	    rpc_tower_vector_free( (rpc_tower_vector_p_t *)
			&service_ptr->towers.list[j-1], 
			st);
		free(service_ptr->towers.list);
	}
	free(srvr_entry_ptr->services.list);

        free(srvr_entry_ptr->fixed.arguments.list);
	free(srvr_entry_ptr->prin_names.list);
}


boolean32
verify_server_t(
        /* [in] */      server_t    *i_ptr,
        /* [in] */      server_t    *o_ptr
)
{
	unsigned32		i;
        char                    *cell_nm;
        char                    *expand_name;
        int                     cell_nm_len;
	error_status_t		st;

        if (uuid_compare(&(i_ptr->id), &(o_ptr->id), &st))
        {
#ifdef DEBUG
                tet_infoline("uuid_compare (server id) failed");
                dced_util_printerr(st);
        	print_uuid ((char *)"input server uuid", &(i_ptr->id));
        	print_uuid ((char *)"output server uuid", &(o_ptr->id));
#endif
                return false;
        }

	if (strcmp( (char *)i_ptr->name, (char *)o_ptr->name))
	{
#ifdef DEBUG
		sprintf (txt_msg, "input entry name: %s\n",i_ptr->name);
		tet_infoline(txt_msg);
		sprintf (txt_msg, "output entry name: %s\n",o_ptr->name);
		tet_infoline(txt_msg);
#endif
		return false;
	}

	if (i_ptr->prin_names.count == o_ptr->prin_names.count)
	{
		for (i=1; i<=o_ptr->prin_names.count;i++)
		{
		    dce_cf_get_cell_name( &cell_nm, &st);
		    cell_nm_len = strlen(cell_nm);
		    expand_name = (char *)malloc(
			cell_nm_len+2+strlen((char *)i_ptr->prin_names.list[i-1]));
		    strcpy(expand_name, cell_nm);
		    strcpy(expand_name+cell_nm_len, "/");
		    strcpy(expand_name+cell_nm_len+1, 
				(char *)i_ptr->prin_names.list[i-1]);
		    free(cell_nm);

		    if (strcmp( (char *)expand_name,
				(char *)o_ptr->prin_names.list[i-1]))
		    {
        	    	sprintf(txt_msg, "input principals name : %s",
					expand_name);
        	    	tet_infoline(txt_msg);
        	    	sprintf(txt_msg, "output principals name : %s",
					o_ptr->prin_names.list[i-1]);
        	    	tet_infoline(txt_msg);
			free( expand_name );
			return false;
		    }
		    free( expand_name );
		}
	}
	else
	{
		sprintf (txt_msg, "input principal count: %d", 
						i_ptr->prin_names.count);
        	tet_infoline(txt_msg);
		sprintf (txt_msg, "output principal count: %d", 
						o_ptr->prin_names.count);
        	tet_infoline(txt_msg);
		return false;
	}

	return true;
}

void
print_server_t(
        /* [in] */      server_t    *o_ptr
)
{
	service_t		*service_ptr;
	unsigned32		i,j;

        sprintf(txt_msg,"server name : %s\n",o_ptr->name);
        tet_infoline(txt_msg);
	print_uuid("server uuid ", &(o_ptr->id));

	sprintf (txt_msg,
			"services count: %d", o_ptr->services.count);
        tet_infoline(txt_msg);
	for (i=1; i<=o_ptr->services.count;i++)
	{
	        service_ptr = &(o_ptr->services.list[i-1]);
        	print_uuid ((char *)"server if uuid (ip)", 
					&(service_ptr->ifspec.uuid));
		sprintf (txt_msg, "interface name: %s", service_ptr->ifname);
        	tet_infoline(txt_msg);
		sprintf (txt_msg, "entry name: %s", service_ptr->entryname);
        	tet_infoline(txt_msg);
		sprintf (txt_msg,
			"objects count: %d", service_ptr->objects.count);
        	tet_infoline(txt_msg);
		for (j=1; j<=service_ptr->objects.count;j++)
		{
		   print_uuid ((char *)"object uuid",
				&(service_ptr->objects.list[j-1]));
		}
		/* print the binding information ? */
	}
	sprintf (txt_msg, "program name: %s", o_ptr->fixed.program);
        tet_infoline(txt_msg);
	sprintf (txt_msg,
			"arguments count: %d", o_ptr->fixed.arguments.count);
        tet_infoline(txt_msg);
	for (i=1; i<=o_ptr->fixed.arguments.count;i++)
	{
        	sprintf(txt_msg,
			"value[%d]: %s",i, o_ptr->fixed.arguments.list[i-1]);
        	tet_infoline(txt_msg);
	}

	sprintf (txt_msg, "principal count: %d", o_ptr->prin_names.count);
        tet_infoline(txt_msg);
	for (i=1; i<=o_ptr->prin_names.count;i++)
	{
        	sprintf(txt_msg,
			"principals name : %s",o_ptr->prin_names.list[i-1]);
        	tet_infoline(txt_msg);
	}
	return;
}

/*
** ROUTINE NAME:
**                    test_srvrconf_init()
**
** DESCRIPTION:
**                    This routine creates the binding to 
**		      to srvrconf service, creates the server_t
**		      entry.
**
*/
void 
test_srvrconf_init(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ server_t	*srvr_entry_ptr,
	/* [out] */	error_status_t	*st
)
{
	error_status_t	tmp_st;

        dced_util_binding_create( SRVRCONF_SVC, binding_ptr, st);
        if (*st != error_status_ok)
                return;

        /*
         * Initialize the server object information
         */
        test_srvr_entry_init( srvr_entry_ptr, st);
        dced_server_create( *binding_ptr, srvr_entry_ptr, st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_server_create failed:");
                dced_util_printerr(*st);
#endif
		dced_binding_free( *binding_ptr, &tmp_st);
                return;
        }
	return;
}

/*
** ROUTINE NAME:
**                    test_srvrconf_free()
**
** DESCRIPTION:
**                    This routine deletes the serrver entry,
**                    frees the binding to srvrconf service.
**
*/
void 
test_srvrconf_free(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ server_t	*srvr_entry_ptr,
	/* [out] */	error_status_t	*st
)
{
	error_status_t	tmp_st;

        dced_server_delete( *binding_ptr,&(srvr_entry_ptr->id), st);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_server_delete failed:");
                dced_util_printerr(*st);
#endif
        }

	test_srvr_entry_free( srvr_entry_ptr, st);
        dced_binding_free( *binding_ptr, &tmp_st);
        if (tmp_st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(tmp_st);
#endif
		return;
        }

	return;
}

/*
** ROUTINE NAME:
**                    test_srvrexec_init()
**
** DESCRIPTION:
**                    This routine creates  the server object,
**		      and returns a binding handle to srvrexec.
**
*/
void 
test_srvrexec_init(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ server_t	*srvr_entry_ptr,
	/* [out] */ 	uuid_t		*proc_id_ptr,
	/* [out] */	error_status_t	*st
)
{
	dced_binding_handle_t	conf_handle;
	error_status_t tmp_st;

	test_srvrconf_init( &conf_handle, srvr_entry_ptr, st);
	sprintf(txt_msg, "leave test_srvrconf_init -stat %d", *st);
	tet_infoline ( txt_msg );
        if (*st != error_status_ok)
                return;

	uuid_create_nil( proc_id_ptr, st );
        dced_server_start( conf_handle, &(srvr_entry_ptr->id), 
			    NULL, proc_id_ptr, st);
	sprintf(txt_msg, "leave dced_server_start -stat %d", *st);
	tet_infoline ( txt_msg );
        if (*st != error_status_ok)
        {
		test_srvrconf_free( &conf_handle, srvr_entry_ptr, &tmp_st);
                return;
        }

        dced_util_binding_create( SRVREXEC_SVC, binding_ptr, st);
	sprintf(txt_msg,"leave dced_util_binding_create -stat %d", *st);
	tet_infoline ( txt_msg );
        if (*st != error_status_ok)
        {
		test_srvrconf_free( &conf_handle, srvr_entry_ptr, &tmp_st);
                return;
        }

	dced_binding_free( conf_handle, st);
	sprintf(txt_msg,"leave dced_binding_free -stat %d", *st);
	tet_infoline ( txt_msg );
	return;
}

/*
** ROUTINE NAME:
**                    test_srvrexec_free()
**
** DESCRIPTION:
**                    This routine stops the server,
**                    frees the binding to srvrexec service.
**
*/
void 
test_srvrexec_free(
	/* [in] */	dced_binding_handle_t *binding_ptr,
	/* [in, out] */ server_t	*srvr_entry_ptr,
	/* [out] */ 	uuid_t		*srvr_proc_id,
	/* [out] */	error_status_t	*st
)
{
	dced_binding_handle_t	conf_handle;
	error_status_t		tmp_st;

        dced_util_binding_create( SRVRCONF_SVC, &conf_handle, st);
	sprintf(txt_msg,"leave dced_util_binding_create -stat %d", *st);
	tet_infoline(txt_msg);
        if (*st != error_status_ok)
        {
                return;
        }

        dced_server_stop( *binding_ptr, srvr_proc_id, 
		 	  srvrexec_stop_hard, st);
	sprintf(txt_msg,"leave dced_server_stop -stat %d", *st);
	tet_infoline(txt_msg);
        if (*st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_server_stop failed:");
                dced_util_printerr(*st);
#endif
        }

	/* free the srvrexec handle */
	dced_binding_free( *binding_ptr, &tmp_st);
	/* delete the server entry and free the srvrconf handle */
	test_srvrconf_free( &conf_handle, srvr_entry_ptr, st);
	sprintf(txt_msg, "leave test_srvrconf_free -stat %d", *st);
	tet_infoline(txt_msg);

	return;
}


