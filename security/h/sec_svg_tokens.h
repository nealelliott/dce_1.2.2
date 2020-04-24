/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg_tokens.h,v $
 * Revision 1.1.14.2  1996/10/03  14:52:46  arvind
 * 	CHFts19728: support salvage of krbtgt private key values
 * 	[1996/08/27  16:02 UTC  aha  /main/DCE_1.2.2/aha_dbpk1_1/1]
 *
 * 	CHFts19728: support salvaging of krbtgt private key
 * 	[1996/08/27  15:33 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	Modifications to allow salvage of krbtgt private key.
 * 	[1996/05/24  23:15 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:46 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	multi-cell group work: add token prt_Foreign_Member_Name and prt_Princ_UUID.
 * 	[1996/03/14  18:46 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:50  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/05/15  21:03 UTC  greg  /main/HPDCE02/3]
 *
 * 	merge
 * 	HP 	[1995/01/17  17:52 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/1]
 *
 * 	add prt_Replist_Name.
 * 	HP 	[1994/10/27  15:31 UTC  hanfei  /main/HPDCE02/1]
 *
 * 	merge
 * 	HP 	[1994/10/27  13:47 UTC  hanfei  /main/hanfei_svg_bl1/1]
 *
 * 	work for ERA entries.
 * 	[1995/12/08  16:54:29  root]
 *
 * Revision 1.1.14.1  1996/05/10  13:18:31  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/14  18:46 UTC  hanfei
 * 	multi-cell group work: add token prt_Foreign_Member_Name and prt_Princ_UUID.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	merge.
 * 	[1995/04/07  15:22 UTC  hanfei  /main/HPDCE02/SEC_migrate/3]
 * 
 * 	add print and reconstruct for xattrschema acls
 * 	[1995/04/07  15:04 UTC  hanfei  /main/HPDCE02/SEC_migrate/hanfei_migrate_bl2/1]
 * 
 * 	merge
 * 	[1995/02/22  17:57 UTC  hanfei  /main/HPDCE02/SEC_migrate/2]
 * 
 * 	tokens added for new prop types.
 * 	[1995/02/22  17:52 UTC  hanfei  /main/HPDCE02/hanfei_comp_bl2/1]
 * 
 * 	work for new data fields defined by migration codes
 * 	[1995/02/08  15:54 UTC  hanfei  /main/HPDCE02/SEC_migrate/1]
 * 
 * 	merge
 * 	[1995/01/17  18:27 UTC  hanfei  /main/HPDCE02/2]
 * 
 * 	add prt_Replist_Name.
 * 	[1995/01/17  17:52 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/1]
 * 
 * 	merge
 * 	[1994/10/27  15:31 UTC  hanfei  /main/HPDCE02/1]
 * 
 * 	work for ERA entries.
 * 	[1994/10/27  13:47 UTC  hanfei  /main/hanfei_svg_bl1/1]
 * 
 * Revision 1.1.8.1  1994/10/26  15:17:29  hanfei
 * 	ERA enhancement work.
 * 	[1994/10/25  18:31:21  hanfei]
 * 
 * Revision 1.1.6.2  1994/06/17  18:42:13  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:51  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/31  19:56:55  hanfei
 * 	work for ERA entries in policy.
 * 	[1994/03/31  19:51:48  hanfei]
 * 
 * Revision 1.1.3.3  1993/03/23  23:07:48  hanfei
 * 	add codes for salvager reconstruct
 * 	[1993/03/23  22:51:30  hanfei]
 * 
 * Revision 1.1.3.2  1993/03/05  18:30:02  frisco
 * 	initial salvager functionality
 * 	[1993/03/02  21:14:04  frisco]
 * 
 * Revision 1.1.1.2  1992/11/25  21:10:09  frisco
 * 	Initial Revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

/*
 *      Registry Database Salvager Print Interface Include file
 */

#ifndef sec_salvage_tokens__included
#define sec_salvage_tokens__included

/*  printing tokens  */

#define prt_Null ""
#define prt_Record_Number "Record_Number"
#define prt_Object_Type "Object_Type"
#define prt_Name "Name"
#define prt_UUID "UUID"
#define prt_Obj_Acl_Def_Cell_ "Obj_Acl_Def_Cell_"
#define prt_Obj_Acl_Def_Cell_Name "Obj_Acl_Def_Cell_Name"
#define prt_Obj_Acl_Def_Cell_UUID "Obj_Acl_Def_Cell_UUID"
#define prt_Num_Acl_Entries "Num_Acl_Entries"
#define prt_Obj_Acl_Entry "Obj_Acl_Entry"
#define prt_Init_Obj_Acl_Def_Cell_ "Init_Obj_Acl_Def_Cell_"
#define prt_Init_Obj_Acl_Def_Cell_Name "Init_Obj_Acl_Def_Cell_Name"
#define prt_Init_Obj_Acl_Def_Cell_UUID "Init_Obj_Acl_Def_Cell_UUID"
#define prt_Init_Obj_Acl_Entry "Init_Obj_Acl_Entry"
#define prt_Init_Cont_Acl_Def_Cell_ "Init_Cont_Acl_Def_Cell_"
#define prt_Init_Cont_Acl_Def_Cell_Name "Init_Cont_Acl_Def_Cell_Name"
#define prt_Init_Cont_Acl_Def_Cell_UUID "Init_Cont_Acl_Def_Cell_UUID"
#define prt_Init_Cont_Acl_Entry "Init_Cont_Acl_Entry"
#define prt_Unix_ID "Unix_ID"
#define prt_Is_Alias_Flag "Is_Alias_Flag"
#define prt_Is_Required_Flag "Is_Required_Flag"
#define prt_Quota "Quota"
#define prt_Fullname "Fullname"
#define prt_Member_Name "Member_Name"
#define prt_Foreign_Member_Name "Foreign_Member_Name"
#define prt_Acct_Group_Name "Acct_Group_Name"
#define prt_Acct_Org_Name "Acct_Org_Name"
#define prt_Acct_Creator_Name "Acct_Creator_Name"
#define prt_Acct_Creator_Cell_UUID "Acct_Creator_Cell_UUID"
#define prt_Acct_Creator_Person_UUID "Acct_Creator_Person_UUID"
#define prt_Acct_Creation_Time "Acct_Creation_Time"
#define prt_Acct_Changer_Name "Acct_Changer_Name"
#define prt_Acct_Changer_Cell_UUID "Acct_Changer_Cell_UUID"
#define prt_Acct_Changer_Person_UUID "Acct_Changer_Person_UUID"
#define prt_Acct_Change_Time "Acct_Change_Time"
#define prt_Acct_Expire_Time "Acct_Expire_Time"
#define prt_Acct_Good_Since_Time "Acct_Good_Since_Time"
#define prt_Acct_Valid_For_Login_Flag "Acct_Valid_For_Login_Flag"
#define prt_Acct_Valid_As_Server_Flag "Acct_Valid_As_Server_Flag"
#define prt_Acct_Valid_As_Client_Flag "Acct_Valid_As_Client_Flag"
#define prt_Acct_Post_Dated_Cert_Ok_Flag "Acct_Post_Dated_Cert_Ok_Flag"
#define prt_Acct_Forwardable_Cert_Ok_Flag "Acct_Forwardable_Cert_Ok_Flag"
#define prt_Acct_TGT_Auth_Cert_Ok_Flag "Acct_TGT_Auth_Cert_Ok_Flag"
#define prt_Acct_Renewable_Cert_Ok_Flag "Acct_Renewable_Cert_Ok_Flag"
#define prt_Acct_Proxiable_Cert_Ok_Flag "Acct_Proxiable_Cert_Ok_Flag"
#define prt_Acct_Dup_Session_Key_Ok_Flag "Acct_Dup_Session_Key_Ok_Flag"
#define prt_Unix_Key "Unix_Key"
#define prt_Plaintext_Passwd "Plaintext_Passwd"
#define prt_Home_Dir "Home_Dir"
#define prt_Shell "Shell"
#define prt_Gecos "Gecos"
#define prt_Passwd_Valid_Flag "Passwd_Valid_Flag"
#define prt_Passwd_Change_Time "Passwd_Change_Time"
#define prt_Max_Certificate_Lifetime "Max_Certificate_Lifetime"
#define prt_Max_Renewable_Lifetime "Max_Renewable_Lifetime"
#define prt_Master_Key_Version "Master_Key_Version"
#define prt_Num_Auth_Keys "Num_Auth_Keys"
#define prt_Auth_Key_Version "Auth_Key_Version"
#define prt_Auth_Key_Type "Auth_Key_Type"
#define prt_Auth_Key_Pepper "Auth_Key_Pepper"
#define prt_Auth_Key_Len "Auth_Key_Len"
#define prt_Auth_Key "Auth_Key"
#define prt_Auth_Key_Expire_Time "Auth_Key_Expire_Time"
#define prt_Projlist_Ok_Flag "Projlist_Ok_Flag"
#define prt_Master_Key_Keytype "Master_Key_Keytype"
#define prt_Master_Key_Length "Master_Key_Length"
#define prt_Master_Key "Master_Key"
#define prt_Rgy_Policy_File_Version "Rgy_Policy_File_Version"
#define prt_Prop_Read_Version "Prop_Read_Version"
#define prt_Prop_Write_Version "Prop_Write_Version"
#define prt_Min_Certificate_Lifetime "Min_Certificate_Lifetime"
#define prt_Default_Certificate_Lifetime "Default_Certificate_Lifetime"
#define prt_Low_Unix_ID_Principal "Low_Unix_ID_Principal"
#define prt_Low_Unix_ID_Group "Low_Unix_ID_Group"
#define prt_Low_Unix_ID_Org "Low_Unix_ID_Org"
#define prt_Max_Unix_ID "Max_Unix_ID"
#define prt_Rgy_Readonly_Flag "Rgy_Readonly_Flag"
#define prt_Auth_Certificate_Unbound_Flag "Auth_Certificate_Unbound_Flag"
#define prt_Shadow_Passwd_Flag "Shadow_Passwd_Flag"
#define prt_Embedded_Unix_ID_Flag "Embedded_Unix_ID_Flag"
#define prt_Realm_Name "Realm_Name"
#define prt_Realm_UUID "Realm_UUID"
#define prt_Unauthenticated_Quota "Unauthenticated_Quota"
#define prt_Acct_Lifetime "Acct_Lifetime"
#define prt_Passwd_Min_Len "Passwd_Min_Len"
#define prt_Passwd_Lifetime "Passwd_Lifetime"
#define prt_Passwd_Expire_Date "Passwd_Expire_Date"
#define prt_Passwd_All_Spaces_Ok "Passwd_All_Spaces_Ok"
#define prt_Passwd_All_Alphanumeric_Ok "Passwd_All_Alphanumeric_Ok"
#define prt_Princ_Cache_State "Princ_Cache_State"
#define prt_Group_Cache_State "Group_Cache_State"
#define prt_Org_Cache_State "Org_Cache_State"
#define prt_Policy_Num_Attrs"Policy_Num_Attrs"
#define prt_Replist_Num_Attrs "Replist_Num_Attrs"
#define prt_Attr_Schema_Num_Attrs "Attr_Schema_Num_Attrs"
#define prt_My_Name "My_Name"
#define prt_Rgy_State_File_Version "Rgy_State_File_Version"
#define prt_Replica_State "Replica_State"
#define prt_Replica_Next_State "Replica_Next_State"
#define prt_Cell_UUID "Cell_UUID"
#define prt_Princ_UUID "Princ_UUID"
#define prt_Server_UUID "Server_UUID"
#define prt_Initialization_UUID "Initialization_UUID"
#define prt_Software_Version "Software_Version"
#define prt_Master_File_Version "Master_File_Version"
#define prt_Master_Known_Flag "Master_Known_Flag"
#define prt_Master_UUID "Master_UUID"
#define prt_Master_Seqno "Master_Seqno"
#define prt_Master_Global_Software_Rev "Master_Global_Software_Rev"
#define prt_Replica_UUID "Replica_UUID"
#define prt_Replica_Name "Replica_Name"
#define prt_Propagation_Type "Propagation_Type"
#define prt_Prev_Propagation_Type "Prev_Propagation_Type"
#define prt_Num_Towers "Num_Towers"
#define prt_Tower_Length "Tower_Length"
#define prt_Tower "Tower"
#define prt_Schema_Entry_Len "Schema_Entry_Len"
#define prt_Schema_Name_Len "Schema_Name_Len"
#define prt_Attr_Encoding "Attr_Encoding"
#define prt_Num_Attr_Acl_Mgr_Entries "Num_Attr_Acl_Mgr_Entries"
#define prt_Attr_Acl_Mgr_Query_Entry "Attr_Acl_Mgr_Query_Entry"
#define prt_Attr_Acl_Mgr_Update_Entry "Attr_Acl_Mgr_Update_Entry"
#define prt_Attr_Acl_Mgr_Delete_Entry "Attr_Acl_Mgr_Delete_Entry"
#define prt_Attr_Acl_Mgr_Test_Entry "Attr_Acl_Mgr_Test_Entry"
#define prt_Attr_Acl_Mgr_Type "Attr_Acl_Mgr_Type"
#define prt_Attr_Schema_Entry_Flag "Attr_Schema_Entry_Flag"
#define prt_Attr_Intercell_Action "Attr_Intercell_Action"
#define prt_Attr_Trig_Type "Attr_Trig_Type"
#define prt_Attr_Bind_Auth_Type "Attr_Bind_Auth_Type"
#define prt_Attr_Svr_Princ_Name "Attr_Svr_Princ_Name"
#define prt_Attr_Auth_Protect_Level "Attr_Auth_Protect_Level"
#define prt_Attr_Auth_Authn "Attr_Auth_Authn"
#define prt_Attr_Auth_Authz "Attr_Auth_Authz"
#define prt_Attr_Num_Binding "Attr_Num_Binding"
#define prt_Attr_Binding_String "Attr_Binding_String"
#define prt_Attr_Binding_Svr_Name_Syntax "Attr_Binding_Svr_Name_Syntax"
#define prt_Attr_Binding_Svr_Name "Attr_Binding_Svr_Name"
#define prt_Attr_Schema_Scope "Attr_Schema_Scope"
#define prt_Attr_Schema_Comment "Attr_Schema_Comment"
#define prt_Num_Attr_List_Entries "Num_Attr_List_Entries"
#define prt_Num_Attr_Instance_Entries "Num_Attr_Instance_Entries"
#define prt_Attr_UUID "Attr_UUID"
#define prt_Attr_Value "Attr_Value"
#define prt_Attr_Value_Type "Attr_Value_Type"
#define prt_Attr_Value_Any "Attr_Value_Any"
#define prt_Attr_Value_Void "Attr_Value_Void"
#define prt_Attr_Value_Integer "Attr_Value_Integer"
#define prt_Attr_Value_Printstring "Attr_Value_Printstring"
#define prt_Attr_Value_Printstring_Array "Attr_Value_Printstring_Array"
#define prt_Num_Attr_Printstring_Array_Entries "Num_Attr_Printstring_Array_Entries"
#define prt_Attr_Value_Encoding_Byte "Attr_Value_Encoding_Byte"
#define prt_Attr_Value_Confidential_Byte "Attr_Value_Confidential_Byte"
#define prt_Attr_Value_I18n_Data "Attr_Value_I18n_Data"
#define prt_Attr_I18n_Codeset "Attr_I18n_Codeset"
#define prt_Attr_Value_UUID "Attr_Value_UUID"
#define prt_Attr_Value_Set "Attr_Value_Set"
#define prt_Attr_Value_Binding "Attr_Value_Binding"
#define prt_Num_Attr_Set "Num_Attr_Set"
#define prt_Replist_Name "Replist_Name"
#define prt_Xattrschema_Name "Xattrschema_Name"

#define tower_lead "        "

/*  .prt file key words */

#define prt_PRINC "PRINC"
#define prt_DIR "DIR"
#define prt_GROUP "GROUP"
#define prt_ORG "ORG"
#define prt_true "true"
#define prt_false "false"
#define prt_none "none"
#define prt_default_policy "default-policy"
#define prt_unlimited "unlimited"
#define prt_forever "forever"
#define prt_des "des"
#define prt_keytype_encrypt_rsa_priv "encrypt_rsa_priv"

#define prt_unknown_to_master "unknown_to_master"
#define prt_uninitialized "uninitialized"
#define prt_initializing "initializing"
#define prt_in_service "in_service"
#define prt_renaming "renaming"
#define prt_copying_database "copying_database"
#define prt_in_maintenance "in_maintenance"
#define prt_master_key_changing "master_key_changing"
#define prt_becoming_master "becoming_master"
#define prt_closed "closed"
#define prt_deleted "deleted"

#define prt_init "init"
#define prt_initing "initing"
#define prt_update "update"
#define prt_delete "delete"
#define prt_unknown "unknown"
#define prt_bad_sw_rev "bad_sw_rev"

#define prt_attr_enc_any "prt_attr_enc_any"
#define prt_attr_enc_void "sec_attr_enc_void"
#define prt_attr_enc_integer "attr_enc_integer"
#define prt_attr_enc_printstring "attr_enc_printstring"
#define prt_attr_enc_printstring_array "attr_enc_printstring_array"
#define prt_attr_enc_bytes "attr_enc_bytes"
#define prt_attr_enc_confidential_bytes "attr_enc_confidential_bytes"
#define prt_attr_enc_i18n_data "attr_enc_i18n_data"
#define prt_attr_enc_uuid "attr_enc_uuid"
#define prt_attr_enc_attr_set "attr_enc_attr_set"
#define prt_attr_enc_binding "attr_enc_binding"
#define prt_attr_enc_trig_binding "attr_enc_trig_binding"

#define prt_attr_sch_entry_none "attr_sch_entry_none"
#define prt_attr_sch_entry_unique "attr_sch_entry_unique"
#define prt_attr_sch_entry_multi_inst "attr_sch_entry_multi_inst"
#define prt_attr_sch_entry_reserved "attr_sch_entry_reserved"
#define prt_attr_sch_entry_use_defaults "attr_sch_entry_use_defaults"

#define prt_accept "accept"
#define prt_reject "reject"
#define prt_evaluate "evaluate"
#define prt_none "none"
#define prt_query "query"
#define prt_update "update"
#define prt_bind_auth_none "bind_auth_none"
#define prt_bind_auth_dce "bind_auth_dce"
#define prt_rpc_c_protect_level_default "rpc_c_protect_level_default"
#define prt_rpc_c_protect_level_none "rpc_c_protect_level_none"
#define prt_rpc_c_protect_level_connect "rpc_c_protect_level_connect"
#define prt_rpc_c_protect_level_call "rpc_c_protect_level_call"
#define prt_rpc_c_protect_level_pkt "rpc_c_protect_level_pkt"
#define prt_rpc_c_protect_level_pkt_integ "rpc_c_protect_level_pkt_integ"
#define prt_rpc_c_protect_level_pkt_privacy "rpc_c_protect_level_pkt_privacy"
#define prt_rpc_c_authn_none "rpc_c_authn_none"
#define prt_rpc_c_authn_dce_secret "rpc_c_authn_dce_secret"
#define prt_rpc_c_authn_dce_public "rpc_c_authn_dce_public"
#define prt_rpc_c_authn_default "rpc_c_authn_default"
#define prt_rpc_c_authz_none "rpc_c_authz_none"
#define prt_rpc_c_authz_name "rpc_c_authz_name"
#define prt_rpc_c_authz_dce "rpc_c_authz_dce"

#endif
