
# changing any of these parameters requires a change in the server code 

set if_uuid             d361c53a-dd78-11cd-9eca-000c00b4b407

set ktab_uuid		abe3a8fc-def9-11cd-884f-000c00b4b407
set ktab_name 		dce_util_server

set conf_uuid		be8cb6d8-def9-11cd-884f-000c00b4b407

set attr_uuid		c96d2ce0-def9-11cd-884f-000c00b4b407
set attr_name		srvrconf/dce_util_server

set pname		dce_util_server
set ppwd		dce_util_server

set srvr_name		dce_util_server
set srvr_uid		0
set srvr_dir		/tmp

set fifo_name		"/tmp/dce_util_server_fifo"


#/.:/hosts/[exec hostname]/config/xattrschema/

proc tutil_setup {admin_pwd} {

	global pname ppwd ktab_uuid conf_uuid attr_uuid
	tutil_cleanup
	tutil_prin_acct_ktb_create $admin_pwd
	tutil_xattr_create
}

proc tutil_run {attr_value_0 other_args} {

	global srvr_name pname ktab_uuid conf_uuid attr_name attr_uuid srvr_uid srvr_dir if_uuid

	tutil_stop
	server create $srvr_name \
		-uuid $conf_uuid    \
		-principals $pname  \
		-keytab $ktab_uuid  \
		-program "[exec pwd]/dce_util_test_server" \
		-arguments "-p $pname -c $conf_uuid -a $attr_uuid -v \"$attr_value_0\" $other_args" \
		-uid $srvr_uid \
		-dir $srvr_dir \
		-services "{ifname foif} {annotation foif_note} \{interface \{$if_uuid 1.0\}\}"

	server modify $srvr_name -add "$attr_name \"$attr_value_0\" foo_value_1"

	server start $srvr_name
}

proc tutil_stop {} {
	global srvr_name

	set cmd	"server stop $srvr_name -method soft"
	catch $cmd
	set cmd "server delete $srvr_name"
	catch $cmd
}


proc tutil_cleanup {} {

	tutil_xattr_delete
	tutil_prin_acct_ktb_delete
}



proc tutil_prin_acct_ktb_create {mypwd} {

	global pname ppwd ktab_uuid ktab_name conf_uuid attr_uuid
	
	principal create $pname
	group add none -member $pname
	organization add none -member $pname
	account create $pname -group none -org none \
		-home /tmp \
		-client yes \
		-server yes \
		-acctvalid yes \
		-password $ppwd \
		-mypwd $mypwd \
		-acctvalid yes
	keytab create $ktab_name \
		-annotation "Test server identity $pname" \
		-storage /tmp/$ktab_name \
		-uuid $ktab_uuid \
		-data "$pname plain 1 $ppwd"
}



proc tutil_prin_acct_ktb_delete {} {

	global pname ktab_name

	set cmd "keytab delete $ktab_name"
	catch $cmd
	set cmd "account delete $pname"
	catch $cmd
	set cmd "principal delete $pname"
	catch $cmd
}



proc tutil_srvr_create {attr_value_0 other_args} {

}





proc tutil_xattr_create {} {
	global attr_name attr_uuid
	xattrschema create /.:/hosts/[exec hostname]/config/xattrschema/$attr_name \
	 -encoding stringarray -multivalued no -acl {srvrconf  r r r r} -uuid $attr_uuid
}



proc tutil_xattr_delete {} {
	global attr_name
	set cmd "xattrschema delete /.:/hosts/[exec hostname]/config/xattrschema/$attr_name"
	catch $cmd
}



